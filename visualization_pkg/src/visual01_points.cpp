/*
    通过visualization_msgs/Marker，在rviz中显示点
    本文加载了一个txt文本的路径点

*/


#include "ros/ros.h"
#include "visualization_msgs/Marker.h"

#include "ros/ros.h"

#include "tf2/LinearMath/Quaternion.h"

#include "tf2_ros/transform_listener.h"

#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

#include "geometry_msgs/PointStamped.h"

#include "geometry_msgs/TransformStamped.h"

#include<ros/package.h>

#include <tf2/LinearMath/Matrix3x3.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "nav_msgs/Path.h"
#include <sstream>
#include <vector>

using namespace std;

std::vector<std::string> split(const std::string &text, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(text);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    ros::init(argc,argv,"visual01_points");

    ros::NodeHandle nh;

    ros::Publisher pub=nh.advertise<visualization_msgs::Marker>("points_marker",1);

    visualization_msgs::Marker point;

    // 点集的头
    point.header.frame_id="map";
    point.header.stamp=ros::Time::now();

    point.ns="shh";
    point.id=0;
    point.type=visualization_msgs::Marker::POINTS;

    // 增加点
    point.action=visualization_msgs::Marker::ADD;

    // 点的尺寸大小
    point.scale.x=0.02;
    point.scale.y=0.02;
    point.scale.z=0.02;

    // 点的颜色，透明度
    point.color.a=1;
    point.color.r=1;
    point.color.g=0;
    point.color.b=0;




    nav_msgs::Path path_msg;
    geometry_msgs::PoseStamped pose_msg;


    ifstream ifs;

    stringstream linestream;
    string t;
   
    ros::Publisher path_pub=nh.advertise<nav_msgs::Path>("/target_path",1,true);

    ifs.open("/home/shh/shh_ros/shh_car/shh_diff_ws/src/shh_pure_pursuit/config/shh_record_path.txt",ios::in);
    ros::Rate rate(20);

    if (ifs.is_open())
        {
            cout << "文件打开成功" << endl;
            
            string buf;
            while (getline(ifs,buf))
            {
                char delimiter = ' ';
                std::vector<std::string> tokens = split(buf, delimiter);
                pose_msg.header.stamp=ros::Time::now();
                pose_msg.header.frame_id="map";


                pose_msg.pose.position.x=atof(tokens[0].c_str());
                pose_msg.pose.position.y=atof(tokens[1].c_str());
                pose_msg.pose.position.z=0.0;

                path_msg.header=pose_msg.header;
                path_msg.poses.push_back(pose_msg);

                tokens.clear();
                // ROS_INFO("x=%f,y=%f", pose_msg.pose.position.x,pose_msg.pose.position.y);
                
                // 点的位置
                geometry_msgs::Point p;
                p.x=atof(tokens[0].c_str());
                p.y=atof(tokens[1].c_str());
                p.z=0;

                // 将读取到的点放到point集中，point中有几个点，就显示几个点
                point.points.push_back(p);
            }
        }
    else
    {
        cout << "文件打开失败" << endl;
    }
    while (ros::ok())
    {
        path_pub.publish(path_msg);
        pub.publish(point);
        rate.sleep();
        ros::spinOnce();
    }
    

    //关闭文件   
    ifs.close();
    return 0;
}

/*
rviz中一格是1m
*/
