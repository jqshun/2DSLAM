Mapping and Navigation
===
# **Settings in In Folder**

**maps** - keep the yaml file and pgm file here

**msg**  - message type for LED strip

**src**  - Neopixel.cpp to configure LED strip 

# **launch**
> move_base_rosaria.launch

优先权控制：

参考网页: https://www.cnblogs.com/21207-iHome/p/8228356.html

```xml
<remap from="/cmd_vel" to="/yocs_cmd_vel_mux/input/navigation" />
```

> move.launch

If gmapping, uncomment gmapping, comment move_base.
If move_base, uncomment move_base, comment gmapping.

> amcl_diff_rosaria.launch

	Set initial pose of car in map

```xml
<param name="initial_pose_x" value="0.0" />
<param name="initial_pose_y" value="0.0" /> 
<param name="initial_pose_a" value="0.0" />  
```

# **config**

> costmap_common_params.yaml

	
Can be pointcloud instead of laser
	
Footprint 是车子的身形，目前是轮子到轮子的距离，形成一个四方形，可是建议根据车子的铁框形状设置。
	
Observation source: 可以是Pointcloud2 3D, 也可以是laser_scan_sensor 2D
	
目前有两个laser sensor, hokuyo(/scan1) 和 Velodyne(/scan) 所以为了local 避障读取hokuyo的/scan1
	
参考网页: https://www.twblogs.net/a/5c7c1960bd9eee339918c427

```xml
footprint: [[0.175, 0.15], [0.175, -0.15], [-0.175, -0.15], [-0.175, 0.15]]
observation_sources: laser_scan_sensor
laser_scan_sensor: {sensor_frame: laser, topic: /scan1, data_type: LaserScan, clearing: true, marking: true}
```


> global_costmap/local_costmap params.yaml

	set inflation radius
	Inflation radius 是对障碍物膨胀/安全距离，这个参数需要根据车子的速度来设置，速度越快，安全距离应该越大。


> move_base_params:

	Set speed and acceleration
```xml
max_vel_x: 8.8
min_vel_x: 0.3
max_rotational_vel: 3.0
min_in_place_vel_theta: 0.6
acc_lim_theta: 0.2
acc_lim_x: 0.30
```





