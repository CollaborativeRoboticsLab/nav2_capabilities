# nav2_capabilities

Provides capabilities2 runners for Nav2 stack. These runners utilize the action and topic interfaces provided by the Nav2 stack to execute navigation tasks.

## Supported runners

| Runner                | Focus      | Description  |
| ---                   | ---        | ---          |
| waypoint runner       | Navigation | utilizes follow_waypoint action from nav2 stack |
| robot pose runner     | Navigation | extracts robots pose based on odom and tf2  |

For information about these runners' interfaces, please refer to [Interface Information](./docs/interface.md) section.

## Examples

Examples depend on [CollaborativeRoboticsLab/capabilities2](https://github.com/CollaborativeRoboticsLab/capabilities2) and Navigation Stack. The `Nav2LaunchRunner` provider starts Nav2 as an on-demand dependency of waypoint and robot-pose runners, and waits for the navigation lifecycle nodes and action server. Ensure the robot or simulation, sensors, map, and robot-specific parameters are configured before running a plan. Do not start a second Nav2 instance for the same robot.

But for best functionality, its recommended to use a robot-specific navigation stack based launch runner and semantic interfaces customized for that robot. For example, if you are using turtlebot4, you can use the [CollaborativeRoboticsLab/turtlebot_capabilities](https://github.com/CollaborativeRoboticsLab/turtlebot_capabilities) which is specialized for turtlebot4 stack.


| Example | Description |
| ---     | ---         |
| [navigation_1.xml](./plans/navigation_1.xml) | In this example (0.5,2) point is reachable. And the robot moves to that point. |
| [navigation_2.xml](./plans/navigation_2.xml) | In this example, (0.5,2) (1,2) (2,0.5) (-2,0) points are reachable. And the robot moves through those points. |
| [navigation_3.xml](./plans/navigation_3.xml) | In this example, (0.5,2) (1,2) (-2,0) are reachable, (2,-3) point is not reachable. Because of this, the robot moves to the (0,0.5) as a recovery action. |
| [navigation_4.xml](./plans/navigation_4.xml) | In this example, (0.5,2) (1,2) (2,1) are reachable, (2,-3), (0, -3) points are not reachable.Because of this, the robot moves to the (0,0.5) as a recovery action. (1,2) (2,1) points also have recovery actions linked, but they are not triggered as the point is accessible. |


> Note: The examples are has been tested with the turtlebot3 world in Gazebo, but on the assumption that the start position of the robot is (0.0, 0.5) in gazebo simulation. In ROS `humble` this is the default, but in `jazzy` you might need to change the initial position of the robot in the world file to match this. If you use [CollaborativeRoboticsLab/turtlebot3-docker](https://github.com/CollaborativeRoboticsLab/turtlebot3-docker), it is already set to this position.

## Setup

Above examples depend on the capabilities2 and prompt_tools packages. You can clone these packages in your workspace and build them using colcon build.

```bash
cd ~/colcon_ws/src
git clone https://github.com/CollaborativeRoboticsLab/capabilities2.git
git clone https://github.com/CollaborativeRoboticsLab/fabric.git
git clone https://github.com/CollaborativeRoboticsLab/nav2_capabilities.git

cd ~/colcon_ws
colcon build --symlink-install
```

To setup the simulation, you can use the turtlebot3 world in Gazebo. You can launch the simulation using the following command,

```bash
git clone https://github.com/CollaborativeRoboticsLab/turtlebot3-docker.git
cd turtlebot3-docker/docker
docker compose pull
xhost +local:root
docker compose up
```

## Running the examples
To run the examples, first start the robot or simulation. The system wrapper starts Fabric and Capabilities2; the waypoint plan then starts its Nav2 launch dependency automatically.

```bash
source install/setup.bash
ros2 launch nav2_capabilities system.launch.py filename:=navigation_1.xml
```

Change `filename:=navigation_1.xml` to match the correct plan