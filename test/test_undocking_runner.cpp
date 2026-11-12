#include <gtest/gtest.h>

#include <nav2_capabilities/undocking_runner.hpp>

class TestUndockingRunner : public capabilities2_runner::UndockingRunner
{
public:
  using UndockingRunner::generate_goal;
};

TEST(UndockingRunner, UsesDefaultGoalParameters)
{
  TestUndockingRunner runner;
  capabilities2_events::EventParameters parameters;

  const auto goal = runner.generate_goal(parameters);

  EXPECT_TRUE(goal.dock_type.empty());
  EXPECT_FLOAT_EQ(goal.max_undocking_time, 30.0f);
}

TEST(UndockingRunner, UsesExplicitGoalParameters)
{
  TestUndockingRunner runner;
  capabilities2_events::EventParameters parameters;
  parameters.set_value("dock_type", std::string("omron"), capabilities2_events::OptionType::STRING);
  parameters.set_value("max_undocking_time", 12.5, capabilities2_events::OptionType::DOUBLE);

  const auto goal = runner.generate_goal(parameters);

  EXPECT_EQ(goal.dock_type, "omron");
  EXPECT_FLOAT_EQ(goal.max_undocking_time, 12.5f);
}