#include<ros/ros.h>
#include<tf/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Twist.h>   
#include<geometry_msgs/PoseWithCovarianceStamped.h>
#include<math.h>
#include<stdio.h>
#include<vector>
#include<iostream>

 
using namespace std;
ros::Subscriber odom_sub;
ros::Publisher odom_pub;
nav_msgs::Odometry odom;
boost::shared_ptr<tf::TransformBroadcaster> odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;
geometry_msgs::Twist cmdvel;
ros::Time odom_stamp;
double odom_poseYaw, odom_poseX, odom_poseY;
ros::Time current_time, last_time;
 
void odom_cb(const nav_msgs::Odometry & odom_data)
{  
    odom_stamp = odom_data.header.stamp;
    //convert from quaternion into radium
    odom_poseX = odom_data.pose.pose.position.x;     //poseX;
    odom_poseY =odom_data.pose.pose.position.y;    //poseY;
    odom_poseYaw =tf::getYaw(odom_data.pose.pose.orientation);  //poseYaw;
    //cap to (-2PI, 2PI)
    while (odom_poseYaw >= 2*M_PI)    odom_poseYaw -= 2*M_PI;
    while (odom_poseYaw <   0)       odom_poseYaw += 2*M_PI;
    ROS_INFO("odom %f %f %f",odom_poseX, odom_poseY, odom_poseYaw);
    current_time = ros::Time::now();
 
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id ="odom";
    odom_trans.child_frame_id ="base_link";
 
    odom_trans.transform.translation.x =odom_poseX;
    odom_trans.transform.translation.y =odom_poseY;
    odom_trans.transform.translation.z = 0.0;
    //since all odometry is 6DOF we'll need aquaternion created from yaw
    geometry_msgs::Quaternion odom_quat =tf::createQuaternionMsgFromYaw(odom_poseYaw);
    odom_trans.transform.rotation = odom_quat;
 
    //send the transform
   odom_broadcaster->sendTransform(odom_trans);
 
    //next, we'll publish the odometry messageover ROS
 
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    odom_trans.child_frame_id ="base_link";
 
    //set the position
    odom.pose.pose.position.x = odom_poseX;
    odom.pose.pose.position.y = odom_poseY;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;
  
    //set the velocity
    odom.child_frame_id ="base_link";
    odom.twist.twist.linear.x =odom_data.twist.twist.linear.x;
    odom.twist.twist.linear.y = odom_data.twist.twist.linear.y;
    odom.twist.twist.angular.z =odom_data.twist.twist.angular.z;
 
    //publish the message
    odom_pub.publish(odom);
    last_time = ros::Time::now();
}
int main(int argc,char **argv) 
{
  ros::init(argc, argv, "odom_tf");
  
  ros::NodeHandle prv_nh;
  odom_broadcaster.reset(new tf::TransformBroadcaster);
  odom_sub = prv_nh.subscribe("/RosAria/pose", 50, &odom_cb);//p2os provides the car's pose throughodometry.
  odom_pub =prv_nh.advertise<nav_msgs::Odometry>("odom", 50);
  ros::Rate r(40.0);
  ROS_INFO("odom %f %f %f",odom_poseX, odom_poseY, odom_poseYaw); //odom_pose
  while(ros::ok())
    {
       ros::spinOnce();    
       r.sleep();
    }
}
