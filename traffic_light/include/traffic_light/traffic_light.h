#ifndef TRAFFIC_LIGHT_H_
#define TRAFFIC_LIGHT_H_

#include "ros/ros.h"
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <visualization_msgs/Marker.h>
#include "std_msgs/Int32.h"
#include "geometry_msgs/PoseStamped.h"

#include <cstdio>
#include <cstring>

namespace TrafficLightNS
{
  class TrafficLight
  {
  protected:
    // ROS messages (topics)
    ros::NodeHandle nh_;
    ros::NodeHandle private_nh_;

    ros::Subscriber sim_pose_sub_;

    ros::Publisher light_pub_;
    ros::Publisher light_pub2_;
    ros::Publisher msg_pub_;

    void currentPoseCallback(const geometry_msgs::PoseStampedConstPtr &msg); // 回调当前位置

  public:
    TrafficLight();
    ~TrafficLight();

    void MainLoop();

    void key();

  private:
    void pubMarker();

    boost::shared_ptr<boost::thread> key_thread_; // 创建线程

    visualization_msgs::Marker traffic_light_;
    visualization_msgs::Marker traffic_light2_;
    // 定义红绿灯的消息类型
    std_msgs::Int32 traffic_light_msg_;

    bool is_in_traffic_light_;

    // 配置的参数
    double traffic_light_1_pose_x_;
    double traffic_light_1_pose_y_;
    double traffic_light_2_pose_x_;
    double traffic_light_2_pose_y_;
    double traffic_light_radius_;
  };
}

#endif
