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

| Example | Description |
| ---     | ---         |
| [navigation_1.xml](./plans/navigation_1.xml) | In this example (0.5,2) point is reachable. And the robot moves to that point. |
| [navigation_2.xml](./plans/navigation_2.xml) | In this example, (0.5,2) (1,2) (2,0.5) (-2,0) points are reachable. And the robot moves through those points. |
| [navigation_3.xml](./plans/navigation_3.xml) | In this example, (0.5,2) (1,2) (-2,0) are reachable, (2,-3) point is not reachable. Because of this, the robot moves to the (0,0.5) as a recovery action. |
| [navigation_4.xml](./plans/navigation_4.xml) | In this example, (0.5,2) (1,2) (2,1) are reachable, (2,-3), (0, -3) points are not reachable.Because of this, the robot moves to the (0,0.5) as a recovery action. (1,2) (2,1) points also have recovery actions linked, but they are not triggered as the point is accessible.
 |

To run the examples, first make sure that the simulation is running and then on seperate terminals run,

```bash
source install/setup.bash
ros2 launch capabilities2_server capabilities2_server.launch.py
```

```bash
source install/setup.bash
ros2 launch nav2_capabilities system.launch.py filename:=navigation_1.xml
```

Change `filename:=navigation_1.xml` to match the correct plan