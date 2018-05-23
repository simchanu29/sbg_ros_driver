#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "ellipse_msg.h"
#include "ellipse.h"

class ImuTranslator{
public:
  ImuTranslator(){
    imu_pub = node.advertise<sensor_msgs::Imu>("imu", 1);
    sub_imu_data = node.subscribe("imu_data", 1, &ImuTranslator::updateImuData, this);
    sub_quat = node.subscribe("ekf_quat", 1, &ImuTranslator::updateQuat, this);

    quat.quaternion.x = 0.0;
    quat.quaternion.y = 0.0;
    quat.quaternion.z = 0.0;
    quat.quaternion.w = 0.0;

    // imu_data
  }

  void updateImuData(const sbg_driver::SbgImuData::ConstPtr& msg){
    imu.angular_velocity = msg->gyro;
    imu.linear_acceleration = msg->accel;
  }

  void updateQuat(const sbg_driver::SbgEkfQuat::ConstPtr& msg){
    imu.orientation = msg->quaternion;
    imu.header.frame_id = "imu";
    imu_pub.publish(imu);
  }

private:
  ros::NodeHandle node;

  // Subscriber
  ros::Subscriber sub_imu_data;
  ros::Subscriber sub_quat;

  // Publishers
  ros::Publisher imu_pub;

  // Internal variables
  sensor_msgs::Imu imu;
  sbg_driver::SbgEkfQuat quat;
  sbg_driver::SbgImuData imu_data;

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sbg_imu_translator");
  ImuTranslator imu_translator;
  ros::spin();
}
