/*
    Line Strip（线条）：Line Strip由一系列连接的线段组成，线段之间按照顺序连接。在RViz中，
    它们通常用于表示路径、轨迹或连续的线条。每个线段都由两个点定义，相邻线段之间共享一个点。
    相邻两点连成线段，第一个点连第二个点，第二个连第三个



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

    ros::init(argc,argv,"visul02_line_strip");

    ros::NodeHandle nh;

    ros::Publisher line_strip_pub=nh.advertise<visualization_msgs::Marker>("line_strip_marker",1);

    visualization_msgs::Marker line_strip;

    // 集的头
    line_strip.header.frame_id="map";
    line_strip.header.stamp=ros::Time::now();

    line_strip.ns="shh";
    line_strip.id=0;
    line_strip.type=visualization_msgs::Marker::LINE_STRIP;

    // 增加点
    line_strip.action=visualization_msgs::Marker::ADD;

    line_strip.pose.orientation.w=1;

    // 线宽
    line_strip.scale.x=0.01;


    // 点的颜色，透明度
    line_strip.color.a=1;
    line_strip.color.r=1;
    line_strip.color.g=0;
    line_strip.color.b=0;




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
                line_strip.points.push_back(p);
            }
        }
    else
    {
        cout << "文件打开失败" << endl;
    }
    while (ros::ok())
    {
        path_pub.publish(path_msg);
        line_strip_pub.publish(line_strip);
        rate.sleep();
        ros::spinOnce();
    }
    

    //关闭文件   
    ifs.close();
    return 0;
}
