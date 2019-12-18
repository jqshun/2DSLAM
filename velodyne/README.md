VLP_16.launch
===
## Settings
> In /PJCH/velodyne/velodyne_pointcloud/launch/VLP16_points.launch

Added this line for transform, z is 1 metre.

```xml
<node pkg="tf" type="static_transform_publisher" name="base_to_laser" args="0 0 1 0 0 0 base_link velodyne 10" />
```
