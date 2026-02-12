#include <pluginlib/class_list_macros.hpp>
#include <capabilities2_runner/runner_base.hpp>
#include <nav2_capabilities/waypoint_runner.hpp>
#include <nav2_capabilities/robotpose_runner.hpp>

// register runner plugins
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::WayPointRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::RobotPoseRunner, capabilities2_runner::RunnerBase)
