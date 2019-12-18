#include<ros/ros.h>
#include<tf/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Twist.h>   
#include<geometry_msgs/PoseWithCovarianceStamped.h>
#include<math.h>
#include<stdio.h>
#include<vector>
#include<iostream>
#include<sensor_msgs/Joy.h>
#include "p3dx_navigation/AdaNeopixel.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

ros::Subscriber sub;
ros::Subscriber sub2;
ros::Publisher pub;
nav_msgs::Odometry odom;
boost::shared_ptr<tf::TransformBroadcaster> odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;
geometry_msgs::Twist cmdvel;
ros::Time odom_stamp;
int x;
int count2=0;
int flag=0;
int previousflag=0;
ros::Time current_time, last_time;
p3dx_navigation::AdaNeopixel LEDmsg;
p3dx_navigation::AdaNeopixel compared;
sensor_msgs::Joy compared2;

void controlLED(const geometry_msgs::Twist & msg){  
    
    current_time = ros::Time::now();
    
    if(msg.angular.z<-0.25){
      // LorR,func,r,g,b,w,brightness,startPix,Len,frequency
      //array[0]= 2 (right)
      //array[0]= 3 (left)
      // switch(func):
      //case 1 :fillAll(r,g,b,w,brightness);
      //case 2: colorWipe(r,g,b,w,brightness); 
      //case 3: breathAll(r,g,b,w,brightness,1);  
      //case 4: whiteOverRainbow(brightness,speed,len);
      //case 5: rainbowFade2White(brightness);   

      //turn left
      LEDmsg.ledcommand = {3,0,255,255,0,10,100,0,72,3};
      flag=1;
    }
    else if(msg.angular.z>0.25){
      //turn right
      LEDmsg.ledcommand = {2,0,255,255,0,10,100,0,72,3};
      flag=2;
    }
    else if(msg.linear.x<0 && msg.angular.z<0.25 && msg.angular.z>-0.25){
       //reverse
      LEDmsg.ledcommand = {0,3,150,0,0,0,100,0,34,1};
      flag=3;
    }
    else if(msg.linear.x>0 && msg.angular.z<0.25 && msg.angular.z>-0.25){
      //go straight
      LEDmsg.ledcommand = {0,1,0,50,255,0,100,0,0,1};
      flag=4;
    }
    else if(msg.linear.x==0 && msg.angular.z==0){
      //warning
      LEDmsg.ledcommand = {0,3,255,128,0,0,100,0,0,3};
      flag=5;
    }

    x=LEDmsg.ledcommand[0];
     ROS_INFO("LEDmsg %i",x);
    
    if(previousflag!=flag){
      if(count2==0){
      pub.publish(LEDmsg);
      }else{
        pub.publish(LEDmsg);
        sleep_until(system_clock::now() + milliseconds(100));
        pub.publish(LEDmsg);
      }    
      count2+=1;  
      previousflag=flag;
    }
    
    last_time = ros::Time::now();
    
}

void controlLED2(const sensor_msgs::Joy & joymsg){  
    
    current_time = ros::Time::now();
    
    if(joymsg.buttons[1]==1){
      // LorR,func,r,g,b,w,brightness,startPix,Len,frequency
      //array[0]= 2 (right)
      //array[0]= 3 (left)
      // switch(func):
      //case 1 :fillAll(r,g,b,w,brightness);
      //case 2: colorWipe(r,g,b,w,brightness); 
      //case 3: breathAll(r,g,b,w,brightness,1);  
      //case 4: whiteOverRainbow(brightness,speed,len);
      //case 5: rainbowFade2White(brightness);   

      //turn left
      LEDmsg.ledcommand = {2,0,255,255,0,10,100,0,10,3};
    }
    else if(joymsg.buttons[2]==1){
      //turn right
      LEDmsg.ledcommand = {3,0,255,255,0,10,100,0,10,3};
    }
    else if(joymsg.buttons[0]==1){
       //reverse
      LEDmsg.ledcommand = {0,3,150,0,0,0,100,134,10,1};
    }
    else if(joymsg.buttons[3]==1){
      //go straight
      LEDmsg.ledcommand = {0,1,0,50,255,0,100,0,0,1};
    }
    else if(joymsg.buttons[6]==1){
      //warning
      LEDmsg.ledcommand = {0,3,255,128,0,0,100,0,0,3};
    }
    else if(joymsg.buttons[5]==1){
      //snake
      LEDmsg.ledcommand = {0,2,100,255,22,0,10,0,0,1};
    }
    else if(joymsg.buttons[7]==1){
      //goal reached
      LEDmsg.ledcommand = {0,4,0,0,0,0,200,0,2,1};
    }
    else{
      LEDmsg.ledcommand = {0,1,0,255,10,0,100,0,0,1};
    }

    x=LEDmsg.ledcommand[0];
     ROS_INFO("LEDmsg %i",x);
    if(compared2.buttons != joymsg.buttons){
      compared2.buttons= joymsg.buttons;
      if(count2==0){
      pub.publish(LEDmsg);
      }else{
        pub.publish(LEDmsg);
        sleep_until(system_clock::now() + milliseconds(100));
        pub.publish(LEDmsg);
      }    
      count2+=1;  
    }
    
    last_time = ros::Time::now();
    
}

int main(int argc,char **argv) {

  ros::init(argc, argv, "Neopixel");
  
  ros::NodeHandle prv_nh;
  //odom_broadcaster.reset(new tf::TransformBroadcaster);
  sub2 = prv_nh.subscribe("/joy", 50, &controlLED2);
  sub = prv_nh.subscribe("/yocs_cmd_vel_mux/output/cmd_vel", 50, &controlLED);//p2os provides the car's pose throughodometry.
  pub =prv_nh.advertise<p3dx_navigation::AdaNeopixel>("Neopixel", 100);
  ros::Rate r(10.0);
  ROS_INFO("LEDmsg2 %i",x);
  while(ros::ok())
    {  
       ros::spinOnce();    
       r.sleep();
    }
}
