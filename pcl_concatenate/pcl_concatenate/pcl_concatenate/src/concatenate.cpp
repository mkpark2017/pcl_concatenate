#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
//#include <geometry_msgs/Point32.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

ros::Publisher pub;
sensor_msgs::PointCloud2 output1;


void merge_data_vector(const std::vector<uint8_t>& vin, std::vector<uint8_t>& vout)
{
  vout.reserve(vout.size() + vin.size());
  vout.insert(vout.end(), vin.begin(), vin.end());
}


void cloud_cb1 (const sensor_msgs::PointCloud2ConstPtr& input)
{
  output1 = *input;
}

void cloud_cb2 (const sensor_msgs::PointCloud2ConstPtr& input)
{
ros::Rate rate(50.0);
  sensor_msgs::PointCloud2 output2;
  sensor_msgs::PointCloud2 output3;
//sensor_msgs::PointCloud2* pointer = &output;
  output2 = *input;

  merge_data_vector(output1.data, output3.data);
  merge_data_vector(output2.data, output3.data);

output3.header = output1.header;
output3.height = output1.height;
output3.width = output1.width + output2.width;
output3.fields = output1.fields;
output3.point_step = output1.point_step;
output3.row_step = output1.row_step;
output3.is_dense = output1.is_dense;
  std::cout << &output1.data << std::endl;
//ROS_INFO("%p", pointer);

  pub.publish (output3);
}

int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "concatenate");
  ros::NodeHandle nh;

//sensor_msgs::PointCloud2* pointer;
  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/output1", 1, cloud_cb1);
  ros::Subscriber sub2 = nh.subscribe ("/output2", 1, cloud_cb2);
  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin ();
}
