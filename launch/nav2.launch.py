from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
	bringup_launch_path = (
		get_package_share_directory('nav2_bringup') + '/launch/bringup_launch.py'
	)

	namespace = LaunchConfiguration('namespace')
	use_namespace = LaunchConfiguration('use_namespace')
	slam = LaunchConfiguration('slam')
	map_yaml_file = LaunchConfiguration('map')
	use_localization = LaunchConfiguration('use_localization')
	use_sim_time = LaunchConfiguration('use_sim_time')
	params_file = LaunchConfiguration('params_file')
	autostart = LaunchConfiguration('autostart')
	use_composition = LaunchConfiguration('use_composition')
	use_respawn = LaunchConfiguration('use_respawn')
	log_level = LaunchConfiguration('log_level')

	return LaunchDescription([
		DeclareLaunchArgument(
			'namespace',
			default_value='',
			description='Top-level namespace for the Nav2 stack.',
		),
		DeclareLaunchArgument(
			'use_namespace',
			default_value='false',
			description='Whether to apply the namespace to the Nav2 stack.',
		),
		DeclareLaunchArgument(
			'slam',
			default_value='False',
			description='Whether to run SLAM instead of localization.',
		),
		DeclareLaunchArgument(
			'map',
			default_value='',
			description='Full path to the map yaml file used for localization.',
		),
		DeclareLaunchArgument(
			'use_localization',
			default_value='True',
			description='Whether to start localization along with navigation.',
		),
		DeclareLaunchArgument(
			'use_sim_time',
			default_value='false',
			description='Use simulation clock if true.',
		),
		DeclareLaunchArgument(
			'params_file',
			default_value=(
				get_package_share_directory('nav2_bringup') + '/params/nav2_params.yaml'
			),
			description='Full path to the Nav2 parameters file.',
		),
		DeclareLaunchArgument(
			'autostart',
			default_value='true',
			description='Automatically transition the Nav2 lifecycle nodes.',
		),
		DeclareLaunchArgument(
			'use_composition',
			default_value='True',
			description='Whether to use composed bringup.',
		),
		DeclareLaunchArgument(
			'use_respawn',
			default_value='False',
			description='Whether to respawn crashed Nav2 nodes when composition is disabled.',
		),
		DeclareLaunchArgument(
			'log_level',
			default_value='info',
			description='Log level for the Nav2 stack.',
		),
		IncludeLaunchDescription(
			PythonLaunchDescriptionSource(bringup_launch_path),
			launch_arguments={
				'namespace': namespace,
				'use_namespace': use_namespace,
				'slam': slam,
				'map': map_yaml_file,
				'use_localization': use_localization,
				'use_sim_time': use_sim_time,
				'params_file': params_file,
				'autostart': autostart,
				'use_composition': use_composition,
				'use_respawn': use_respawn,
				'log_level': log_level,
			}.items(),
		),
	])
