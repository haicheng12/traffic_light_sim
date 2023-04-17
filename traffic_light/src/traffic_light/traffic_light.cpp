#include "traffic_light/traffic_light.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "traffic_light");

  TrafficLightNS::TrafficLight traffic_light;
  traffic_light.MainLoop();

  return 0;
}
