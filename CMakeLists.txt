cmake_minimum_required(VERSION 3.10)
project(nav2_capabilities)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(pluginlib REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(capabilities2_msgs REQUIRED)
find_package(capabilities2_runner REQUIRED)
find_package(capabilities2_events REQUIRED)

include_directories(
  include
)

add_library(${PROJECT_NAME} SHARED
  src/nav2_runners.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  rclcpp_action
  pluginlib
  nav2_msgs
  tf2
  tf2_ros
  geometry_msgs
  capabilities2_msgs
  capabilities2_runner
  capabilities2_events
)

pluginlib_export_plugin_description_file(capabilities2_runner plugins.xml)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY interfaces
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY providers
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY plans
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(undocking_runner_test test/test_undocking_runner.cpp)
  if(TARGET undocking_runner_test)
    ament_target_dependencies(undocking_runner_test
      rclcpp
      rclcpp_action
      nav2_msgs
      capabilities2_msgs
      capabilities2_runner
      capabilities2_events
    )
  endif()
endif()

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()
