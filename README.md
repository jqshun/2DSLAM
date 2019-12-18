How to Connect
===
**Jetson Nano**

```xml
ssh nvidia-nano@192.168.3.10  or 192.168.2.10

password: 000000
```

**I9 Desktop**

```xml
ssh ntut-mit@192.168.3.20      or 192.168.2.20    

password: 000000
```

# **In .bashrc :** 

```xml

**MASTER**

export ROS_MASTER_URI=http://192.168.3.10:11311

export ROS_IP=192.168.3.10
```
```xml

**SLAVE**

export ROS_MASTER_URI=http://192.168.3.10:11311        (master-ip)

export ROS_IP=192.168.3.3                             (self-ip)
```


# **Run in Jetson-Nano**

	roslaunch autolabor_pro1_driver autolabor_driver.launch

# **Run in Laptop**

```xml
roslaunch p3dx_navigation move.launch 
	
rviz	
```

# **To save map for gmapping**

	rosrun map_server map_saver -f mymap3
