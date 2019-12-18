Mapping and Navigation
===
# **Settings in In Folder**

**maps** - keep the yaml file and pgm file here

**msg**  - message type for LED strip

**src**  - Neopixel.cpp to configure LED strip 

# **launch**
> move_base_rosaria.launch

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

```xml
footprint: [[0.175, 0.15], [0.175, -0.15], [-0.175, -0.15], [-0.175, 0.15]]
observation_sources: laser_scan_sensor
aser_scan_sensor: {sensor_frame: laser, topic: /scan1, data_type: LaserScan, clearing: true, marking: true}
```


> global_costmap/local_costmap params.yaml

	set inflation radius


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





