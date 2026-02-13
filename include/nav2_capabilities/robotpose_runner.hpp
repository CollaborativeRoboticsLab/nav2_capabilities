#pragma once

#include <rclcpp/rclcpp.hpp>

#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

#include "geometry_msgs/msg/transform_stamped.hpp"

#include <capabilities2_runner/topic_runner.hpp>
#include <capabilities2_runner/threadtrigger_runner.hpp>

namespace capabilities2_runner
{

/**
 * @brief odometry runner class
 *
 * Capability Class to grab odometry data
 *
 */
class RobotPoseRunner : public ThreadTriggerRunner
{
public:
  RobotPoseRunner() : ThreadTriggerRunner()
  {
  }

  /**
   * @brief Starter function for starting the subscription runner
   *
   * @param node shared pointer to the capabilities node. Allows to use ros node related functionalities
   * @param run_config runner configuration loaded from the yaml file
   * @param bond_id bond id for the runner instance, used for event emission and tracking
   */
  virtual void start(rclcpp::Node::SharedPtr node, const runner_opts& run_config, const std::string& bond_id) override
  {
    // initialize the runner base by storing node pointer and run config
    init_base(node, run_config);

    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(node_->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // emit start event
    emit_started(bond_id, param_on_started());
  }

  /**
   * @brief Trigger process to be executed.
   *
   * This method utilizes paramters set via the trigger() function
   *
   * @param parameters pointer to tinyxml2::XMLElement that contains parameters
   */
  virtual void execution(capabilities2_events::EventParameters parameters, const std::string& thread_id) override
  {
    // split thread_id to get bond_id and trigger_id (format: "bond_id/trigger_id")
    std::string bond_id = ThreadTriggerRunner::bond_from_thread_id(thread_id);
    std::string trigger_id = ThreadTriggerRunner::trigger_from_thread_id(thread_id);

    std::string map = "map", odom = "odom", robot = "base_link";

    if (parameters.has_value("map"))
      map = std::any_cast<std::string>(parameters.get_value("map"));
    else
      RCLCPP_WARN(node_->get_logger(), "missing parameter: map, defaulting to 'map'");

    if (parameters.has_value("odom"))
      odom = std::any_cast<std::string>(parameters.get_value("odom"));
    else
      RCLCPP_WARN(node_->get_logger(), "missing parameter: odom, defaulting to 'odom'");

    if (parameters.has_value("robot"))
      robot = std::any_cast<std::string>(parameters.get_value("robot"));
    else
      RCLCPP_WARN(node_->get_logger(), "missing parameter: robot, defaulting to 'base_link'");

    RCLCPP_INFO(node_->get_logger(), "Transforming from %s to %s or %s to %s", map.c_str(), robot.c_str(), odom.c_str(),
                robot.c_str());

    // Try to use map -> robot first
    try
    {
      transform_ = tf_buffer_->lookupTransform(map, robot, tf2::TimePointZero);

      // trigger the events related to on_success state
      emit_succeeded(bond_id, param_on_success());

      RCLCPP_INFO(node_->get_logger(), "Transformation received. Thread closing for event %s", trigger_id.c_str());
      return;
    }
    catch (tf2::TransformException& ex)
    {
      RCLCPP_INFO(node_->get_logger(), "Trying to transform from %s to %s", odom.c_str(), robot.c_str());
    }

    // Fall back to odom -> robot
    try
    {
      transform_ = tf_buffer_->lookupTransform(odom, robot, tf2::TimePointZero);

      // trigger the events related to on_success state
      emit_succeeded(bond_id, param_on_success());

      RCLCPP_INFO(node_->get_logger(), "Transformation received. Thread closing for event %s", trigger_id.c_str());
    }
    catch (tf2::TransformException& ex)
    {
      RCLCPP_INFO(node_->get_logger(), "Could not transform from odom to robot: %s", ex.what());

      // trigger the events related to on_failure state
      emit_failed(bond_id, param_on_failure());

      RCLCPP_INFO(node_->get_logger(), "Transformation not received. Thread closing for event %s", trigger_id.c_str());
    }
  }

  /**
   * @brief stop function to cease functionality and shutdown
   *
   */
  virtual void stop(const std::string& bond_id) override
  {
    // if the node pointer is empty then throw an error
    // this means that the runner was not started and is being used out of order

    if (!node_)
      throw runner_exception("cannot stop runner that was not started");

    // throw an error if the service client is null
    // this can happen if the runner is not able to find the action resource

    if (!tf_listener_)
      throw runner_exception("cannot stop runner subscriber that was not started");

    // Trigger on_stopped event if defined
    emit_stopped(bond_id, param_on_stopped());
  }

protected:
  /**
   * @brief Update on_success event parameters with new data if avaible.
   *
   * This function is used to inject new data into the XMLElement containing
   * parameters related to the on_success trigger event
   *
   * @return EventParameters updated parameters for on_success event
   */
  virtual capabilities2_events::EventParameters param_on_success() override
  {
    capabilities2_events::EventParameters parameters;
    parameters.set_value("x", transform_.transform.translation.x, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("y", transform_.transform.translation.y, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("z", transform_.transform.translation.z, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("qx", transform_.transform.rotation.x, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("qy", transform_.transform.rotation.y, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("qz", transform_.transform.rotation.z, capabilities2_events::OptionType::DOUBLE);
    parameters.set_value("qw", transform_.transform.rotation.w, capabilities2_events::OptionType::DOUBLE);

    return parameters;
  };

  std::shared_ptr<tf2_ros::TransformListener> tf_listener_{ nullptr };
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  geometry_msgs::msg::TransformStamped transform_;
};
}  // namespace capabilities2_runner