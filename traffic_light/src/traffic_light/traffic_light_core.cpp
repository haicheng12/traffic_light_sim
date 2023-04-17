#include "traffic_light/traffic_light.h"

namespace TrafficLightNS
{
  TrafficLight::TrafficLight() : is_in_traffic_light_(false)
  {
    nh_.getParam("traffic_light_1_pose_x", traffic_light_1_pose_x_);
    nh_.getParam("traffic_light_1_pose_y", traffic_light_1_pose_y_);
    nh_.getParam("traffic_light_2_pose_x", traffic_light_2_pose_x_);
    nh_.getParam("traffic_light_2_pose_y", traffic_light_2_pose_y_);
    nh_.getParam("traffic_light_radius", traffic_light_radius_);

    light_pub_ = nh_.advertise<visualization_msgs::Marker>("traffic_light", 10);
    light_pub2_ = nh_.advertise<visualization_msgs::Marker>("traffic_light2", 10);
    msg_pub_ = nh_.advertise<std_msgs::Int32>("traffic_light_msg", 1);

    sim_pose_sub_ = nh_.subscribe("/sim_pose", 10, &TrafficLight::currentPoseCallback, this);

    key_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&TrafficLight::key, this))); // 创建线程
  }

  TrafficLight::~TrafficLight()
  {
  }

  void TrafficLight::currentPoseCallback(const geometry_msgs::PoseStampedConstPtr &msg) // 回调当前位置
  {
    geometry_msgs::PoseStamped sim_pose = *msg;

    double dis = sqrt(pow(traffic_light_1_pose_x_ - sim_pose.pose.position.x, 2) +
                      pow(traffic_light_1_pose_y_ - sim_pose.pose.position.y, 2)); // 计算第一个红绿灯到小车当前位置的距离
    //ROS_INFO("dis [%f]", dis);

    if (dis <= traffic_light_radius_) // 在红绿灯半径为多少的范围内
    {
      is_in_traffic_light_ = true;
    }
    else
    {
      is_in_traffic_light_ = false;
    }
  }

  void TrafficLight::key()
  {
    ros::Rate loop_rate(1);

    // 获取从键盘输入的数据
    printf("请输入红绿灯的数据：\n");
    printf("按 r 为红灯\n");
    printf("按 g 为绿灯\n");
    char buf[1024];
    char red_data[] = "r";
    char green_data[] = "g";

    ssize_t size = read(STDIN_FILENO, buf, sizeof(buf));

    printf("红绿灯的数据: %s\n", buf);

    while (ros::ok())
    {
      std::cin >> buf;
      std::cout << "键盘输入的数据为：" << std::endl;
      std::cout << buf << std::endl;

      // 给颜色
      if (strcmp(buf, red_data) == 0 && is_in_traffic_light_)
      {
        printf("显示红灯\n");
        traffic_light_.color.a = 1.0; // 红色
        traffic_light_.color.r = 1.0;
        traffic_light_.color.g = 0.0;
        traffic_light_.color.b = 0.0;

        traffic_light2_.color.a = 1.0; // 红色
        traffic_light2_.color.r = 1.0;
        traffic_light2_.color.g = 0.0;
        traffic_light2_.color.b = 0.0;

	//if(is_in_traffic_light_) // 为红色并且小车在红绿灯范围内
        //{
	    printf("小车在红灯范围内，小车停止\n");
            traffic_light_msg_.data = 1; // 红灯为1
	//}
      }
      else if (strcmp(buf, green_data) == 0) 
      {
        printf("显示绿灯\n");
        traffic_light_.color.a = 1.0; // 绿色
        traffic_light_.color.r = 0.0;
        traffic_light_.color.g = 1.0;
        traffic_light_.color.b = 0.0;

        traffic_light2_.color.a = 1.0; // 绿色
        traffic_light2_.color.r = 0.0;
        traffic_light2_.color.g = 1.0;
        traffic_light2_.color.b = 0.0;

	//if(!is_in_traffic_light_)// 为绿色并且小车不在红绿灯范围内
	//{
	    printf("小车在绿灯范围内，小车前进\n");
            traffic_light_msg_.data = 2; // 绿灯为2
	//}
      }
      else
      {
        printf("输入数据不正确，请重新输入\n");
        traffic_light_msg_.data = 0;
      }

      ros::spinOnce();
      loop_rate.sleep();
    }
  }

  void TrafficLight::pubMarker()
  {
    traffic_light_.header.frame_id = "/map";
    traffic_light_.header.stamp = ros::Time::now();
    traffic_light_.ns = "traffic_light";
    traffic_light_.id = 0;
    traffic_light_.type = visualization_msgs::Marker::CYLINDER;
    traffic_light_.action = visualization_msgs::Marker::ADD;

    // 给位置
    traffic_light_.pose.position.x = traffic_light_1_pose_x_;
    traffic_light_.pose.position.y = traffic_light_1_pose_y_;
    traffic_light_.pose.position.z = 0.0;
    traffic_light_.pose.orientation.x = 0.0;
    traffic_light_.pose.orientation.y = 0.0;
    traffic_light_.pose.orientation.z = 0.0;
    traffic_light_.pose.orientation.w = 1.0;

    // 给大小
    traffic_light_.scale.x = 0.8;
    traffic_light_.scale.y = 0.8;
    traffic_light_.scale.z = 0.4;

    traffic_light2_.header.frame_id = "/map";
    traffic_light2_.header.stamp = ros::Time::now();
    traffic_light2_.ns = "traffic_light2";
    traffic_light2_.id = 1;
    traffic_light2_.type = visualization_msgs::Marker::CYLINDER;
    traffic_light2_.action = visualization_msgs::Marker::ADD;

    // 给位置
    traffic_light2_.pose.position.x = traffic_light_2_pose_x_;
    traffic_light2_.pose.position.y = traffic_light_2_pose_y_;
    traffic_light2_.pose.position.z = 0.0;
    traffic_light2_.pose.orientation.x = 0.0;
    traffic_light2_.pose.orientation.y = 0.0;
    traffic_light2_.pose.orientation.z = 0.0;
    traffic_light2_.pose.orientation.w = 1.0;

    // 给大小
    traffic_light2_.scale.x = 0.8;
    traffic_light2_.scale.y = 0.8;
    traffic_light2_.scale.z = 0.4;

    light_pub_.publish(traffic_light_);
    light_pub2_.publish(traffic_light2_);
    msg_pub_.publish(traffic_light_msg_);
  }

  void TrafficLight::MainLoop()
  {
    ros::Rate loop_rate(20);

    while (ros::ok())
    {
      pubMarker();

      ros::spinOnce();
      loop_rate.sleep();
    }
  }

}
