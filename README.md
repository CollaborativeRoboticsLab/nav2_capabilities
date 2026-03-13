# nav2_capabilities

Provides runners for Nav2 stack.

## Supported runners

| Runner | Description  |
| ---    | ---          |
| waypoint runner | utilizes follow_waypoint action from nav2 stack |
| occupancygrid runner | extracts occupancy grid data from the related topic  |
| robot pose runner | extracts robots pose based on odom and tf2  |

## Examples

Examples depend on [CollaborativeRoboticsLab/capabilities2](https://github.com/CollaborativeRoboticsLab/capabilities2) and Navigation Stack. Following examples have been tested against turtlebot3 simulation using [CollaborativeRoboticsLab/turtlebot3-docker](https://github.com/CollaborativeRoboticsLab/turtlebot3-docker)

## Navigation

| &nbsp; Example &nbsp; | Description |
| ---     | ---         |
| [Example 1](./docs/example1.md) | Implements the basic fabric triggering that moves the robot from one point to another. |
| [Example 2](./docs/example2.md) | Implements navigating through 5 points using 'sequential' control functionality. |
| [Example 3](./docs/example3.md) | Implements navigating through 5 points inluding 1 inaccessible point (1 recovery point) using `sequential` and `recovery` control functionality. |
| [Example 4](./docs/example4.md) | Implements navigating through 5 points inluding 4 inaccessible point (4 recovery point) using `sequential` and `recovery` control functionality. |

To run the examples, first make sure that the simulation is running and then on seperate terminals run,

```bash
source install/setup.bash
ros2 launch capabilities2_server capabilities2_server.launch.py
```

```bash
source install/setup.bash
ros2 launch nav2_capabilities system.launch.py filename:=navigation_1.xml
```