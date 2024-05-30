/*
    LINE_LIST类型在每对点外创建一个不相连的线，也就是0连接1,2连接3，等等。
    Line List（线段列表）：Line List由多个独立的线段组成，每个线段由两个点定义。在RViz中，
    它们通常用于表示离散的线段集合，每个线段之间都是独立的。可以使用Line List来可视化多个不相连的线段。

    1,2,3,4
    1-2为一个线段 3-4为一个线段 2和3之间没有链接


*/


#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "visul04_text");
    ros::NodeHandle nh;

    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("text_marker", 10);

    // 创建Marker消息
    visualization_msgs::Marker text_marker;
    text_marker.header.frame_id = "map";  // 设置坐标系
    text_marker.header.stamp = ros::Time::now();
    text_marker.ns = "shh";
    text_marker.action = visualization_msgs::Marker::ADD;
    text_marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;

    text_marker.pose.position.x = 1.0;  // 设置文字位置
    text_marker.pose.position.y = 1.0;
    text_marker.pose.position.z = 0.0;
    text_marker.pose.orientation.w = 1.0;


    text_marker.scale.z = 0.5;  // 设置文字大小


    text_marker.color.r = 0.0;  // 设置颜色为红色
    text_marker.color.g = 1.0;
    text_marker.color.b = 1.0;
    text_marker.color.a = 1.0;
    text_marker.text = "Hello, RViz!";  // 设置要显示的文字内容

    ros::Rate rate(10);  // 发布频率为10Hz

    while (ros::ok()) {
        // 发布Marker消息
        marker_pub.publish(text_marker);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
