URG_lidar.launch
===
## Settings

sometimes is /dev/ttyACM0 use **ls /dev** to check
```xml
<param name="serial_port" value="/dev/ttyACM1"/>     
```


In radian,  Value*M_PI/180 to get angle
```xml
<param name="angle_min" value="-1.5707963"/>     
<param name="angle_max" value="1.5707963"/>     
```

0.15 is x, 0.18 is z.
```xml
<node pkg="tf" type="static_transform_publisher" name="base_to_laser2" args="0.15 0.0 0.18 0 0 0 base_link laser 10" /> 
```

HOKUYO is /scan1, Velodyne is /scan
```xml
<remap from="/scan" to ="/scan1"/>
```
