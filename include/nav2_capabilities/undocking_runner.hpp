#pragma once

#include <string>

#include <nav2_msgs/action/undock_robot.hpp>
#include <capabilities2_runner/action_runner.hpp>

namespace capabilities2_runner
{

class UndockingRunner : public ActionRunner<nav2_msgs::action::UndockRobot>
{
public:
	void start(rclcpp::Node::SharedPtr node, const runner_opts& run_config, const std::string& bond_id) override
	{
		init_action(node, run_config, "/undock_robot", "nav2_msgs::action::UndockRobot");
		emit_started(bond_id, "", param_on_started());
	}

protected:
	nav2_msgs::action::UndockRobot::Goal generate_goal(capabilities2_events::EventParameters& parameters) override
	{
		nav2_msgs::action::UndockRobot::Goal goal;
		goal.dock_type = std::any_cast<std::string>(parameters.get_value("dock_type", std::string("")));
		goal.max_undocking_time = std::any_cast<double>(parameters.get_value("max_undocking_time", 30.0));
		return goal;
	}

	std::string generate_feedback(const nav2_msgs::action::UndockRobot::Feedback::ConstSharedPtr) override
	{
		return "";
	}

	std::string result_diagnostic() const override
	{
		return result_ ? result_->error_msg : "";
	}
};

}  // namespace capabilities2_runner
