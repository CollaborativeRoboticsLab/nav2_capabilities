#pragma once

#include <thread>
#include <chrono>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>

#include <capabilities2_runner/action_runner.hpp>

using namespace std::chrono_literals;

namespace capabilities2_runner
{

/**
 * @brief waypoint runner class
 *
 * Class to run waypointfollower action based capability
 *
 */
class WayPointRunner : public ActionRunner<nav2_msgs::action::NavigateToPose>
{
public:
  WayPointRunner() : ActionRunner()
  {
  }

  /**
   * @brief Starter function for starting the action runner
   *
   * @param node shared pointer to the capabilities node. Allows to use ros node related functionalities
   * @param run_config runner configuration loaded from the yaml file
   * @param bond_id unique identifier for the group of connections associated with this runner trigger event
   */
  virtual void start(rclcpp::Node::SharedPtr node, const runner_opts& run_config, const std::string& bond_id) override
  {
    init_action(node, run_config, "/navigate_to_pose");

    // emit start event
    emit_started(bond_id, "", param_on_started());
  }

protected:
  /**
   * @brief This generate goal function overrides the generate_goal() function from ActionRunner()
   * @param parameters XMLElement that contains parameters in the format
   '<Event name=follow_waypoints provider=WaypointRunner x='$value' y='$value' />'
   * @return ActionT::Goal the generated goal
   */
  virtual nav2_msgs::action::NavigateToPose::Goal
  generate_goal(capabilities2_events::EventParameters& parameters) override
  {
    double x, y, z, qx, qy, qz, qw;

    x = std::any_cast<double>(parameters.get_value("x", 0.0));
    y = std::any_cast<double>(parameters.get_value("y", 0.0));
    z = std::any_cast<double>(parameters.get_value("z", 0.0));
    qx = std::any_cast<double>(parameters.get_value("qx", 0.0));
    qy = std::any_cast<double>(parameters.get_value("qy", 0.0));
    qz = std::any_cast<double>(parameters.get_value("qz", 0.0));
    qw = std::any_cast<double>(parameters.get_value("qw", 1.0));

    RCLCPP_INFO(node_->get_logger(), "goal consist of x: %f, y: %f, z: %f, qx: %f, qy: %f, qz: %f, qw: %f", x, y, z, qx,
                qy, qz, qw);

    nav2_msgs::action::NavigateToPose::Goal goal_msg;
    geometry_msgs::msg::PoseStamped pose_msg;

    pose_msg.header.frame_id = "map";
    pose_msg.header.stamp.sec = 0;
    pose_msg.header.stamp.nanosec = 0;

    pose_msg.pose.position.x = x;
    pose_msg.pose.position.y = y;
    pose_msg.pose.position.z = z;
    pose_msg.pose.orientation.x = qx;
    pose_msg.pose.orientation.y = qy;
    pose_msg.pose.orientation.z = qz;
    pose_msg.pose.orientation.w = qw;

    goal_msg.pose = pose_msg;

    return goal_msg;
  }

  /**
   * @brief This generate feedback function overrides the generate_feedback() function from ActionRunner()
   *
   * @param msg feedback message from the action server
   * @return std::string of feedback information
   */
  virtual std::string
  generate_feedback(const typename nav2_msgs::action::NavigateToPose::Feedback::ConstSharedPtr msg) override
  {
    return "";
  }

protected:
  std::string global_frame_;     /**The global frame of the robot*/
  std::string robot_base_frame_; /**The frame of the robot base*/

  double x, y; /**Coordinate frame parameters*/
};

}  // namespace capabilities2_runner
