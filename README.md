# traffic_light_sim

# 红绿灯仿真开发记录
```
开发人员：杨工

工作记录：
——2023.4.17
1、代码重新整理
2、提取红绿灯的位置参数出来，可设置红绿灯的有效半径范围
3、autoware.ai的twist_filter.cpp文件有更改，配合红绿灯使用
4、修改的文件和地图数据放在other_file
```
**整体功能**

基于autoware仿真，键盘按下'r'为绿灯，小车前进，键盘按下'g'为红灯，并且红灯在设定范围内，小车停止。

<launch>

  <param name="traffic_light_1_pose_x" type="double" value="75.0" /> <!--1号红绿灯的x位置，单位：米-->
  <param name="traffic_light_1_pose_y" type="double" value="-58.0" /> <!--1号红绿灯的y位置，单位：米-->
  <param name="traffic_light_2_pose_x" type="double" value="76.0" /> <!--2号红绿灯的x位置，单位：米-->
  <param name="traffic_light_2_pose_y" type="double" value="-57.0" /> <!--2号红绿灯的y位置，单位：米-->
  <param name="traffic_light_radius" type="double" value="30.0" /> <!--红绿灯的半径，小车在这个范围内停车，单位：米-->
  <node name="traffic_light" pkg="traffic_light" type="traffic_light" output="screen"/>

</launch>

**下载autoware.ai依赖环境**
```
sudo apt-get install ros-melodic-jsk-rviz-plugins 
sudo apt-get install ros-melodic-nmea-msgs 
sudo apt-get install ros-melodic-automotive-platform-msgs 
sudo apt-get install ros-melodic-automotive-navigation-msgs 
sudo apt-get install ros-melodic-pacmod-msgs 
sudo apt-get install ros-melodic-velodyne-pointcloud 
sudo apt-get install ros-melodic-grid-map-ros 
sudo apt-get install libglew-dev 
sudo apt-get install ros-melodic-gps-common 
sudo apt install -y python-catkin-pkg python-rosdep ros-$ROS_DISTRO-catkin 
sudo apt install -y python3-pip python3-colcon-common-extensions python3-setuptools python3-vcstool 
pip3 install -U setuptools 
sudo apt-get install ros-melodic-nmea-msgs 
sudo apt-get install freeglut3-dev 
sudo apt-get install mesa-utils 
sudo apt-get install ros-melodic-tf2-geometry-msgs 
sudo apt-get install ros-melodic-gazebo-msgs 
sudo apt-get install ros-melodic-joint-state-controller 
sudo apt install ros-melodic-controller-manager
```

**编译autoware**
```
cd autoware.ai
catkin_make
```

**启动autoware**
```
roscore
rosrun runtime_manager runtime_manager_dialog.py
```

**autoware花屏问题**

去下载wxPython-4.0.7.post2-cp27-cp27mu-linux_x86_64.whl
```
https://extras.wxpython.org/wxPython4/extras/linux/gtk2/ubuntu-18.04/
```
卸载原来的
```
pip uninstall wxPython
```

安装新的
```
sudo pip install wxPython-4.0.7.post2-cp27-cp27mu-linux_x86_64.whl
```

找到autoware.ai/src/autoware/utilities/runtime_manager/scripts中的runtime_manager_dialog.py文件
在文件中添加 import wx.adv
把文件中所有的 wx.HyperlinkCtrl 替换成 wx.adv.HyperlinkCtrl
把文件中所有的 wx.EVT_HYPERLINK 替换成 wx.adv.EVT_HYPERLINK

**仿真步骤**

详情参考doc文件夹里面的文档

**RVIZ效果**

![Image text](https://github.com/haicheng12/traffic_light_sim/blob/main/img/sim.png)

