#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <tf/transform_listener.h>


#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl_ros/impl/transforms.hpp>


ros::Publisher pub;

void 
cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
  tf::TransformListener listener;
  tf::StampedTransform transform;

  ros::Rate rate(50.0);

//  listener.lookupTransform("/world", "/uav1/velodyne", ros::Time(0), transform);
try {
    listener.waitForTransform("/world", "/uav1/velodyne", ros::Time(0), ros::Duration(10.0) );
    listener.lookupTransform("/world", "/uav1/velodyne", ros::Time(0), transform);
} catch (tf::TransformException ex) {
    ROS_ERROR("%s",ex.what());
}
  // Create a container for the data.
  sensor_msgs::PointCloud2 output;

  // Do data processing here...
//  output = *input;
  pcl_ros::transformPointCloud("/world", transform, *input, output);

  // Publish the data.
  pub.publish (output);
}

int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("input", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin ();
}
