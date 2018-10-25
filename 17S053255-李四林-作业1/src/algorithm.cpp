//
// Created by lee on 18-9-24.
//

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


/*
 * 算法思想：
 * 如果测试点的两边点的个数都是奇数个则该测试点在多边形内，否则在多边形外。
 * 注意：此算法对于顺逆时针的点集都可以解决
 * input: target_points 目标点集 vec_point 多边形点集
 * output: vector<string>
 */
vector<string> GetPointInPolygon(const vector<Point> &target_points, vector<Point> &vec_point) {
    vec_point.push_back(vec_point.front());//最后一个点也是第一个点
    auto npts = target_points.size();
    vector<string> ret_str(npts);

    for (int j = 0; j < npts; ++j) {
        Point P = target_points[j];
        auto n = vec_point.size() - 1;//number of the polygon points-1
        int cn = 0;    // the  crossing number counter

        // loop through all edges of the polygon
        for (int i = 0; i < n; i++) {    // edge from vec_point[i]  to vec_point[i+1]
            //point is one of the vec_point
            if (P == vec_point[i] || P == vec_point[i + 1]) {
                ret_str[j] = string("yes");
                cn = 1;//奇数, inside
                break;
            }

            if (((vec_point[i].y <= P.y) && (vec_point[i + 1].y > P.y))     // an upward crossing逆时针画
                || ((vec_point[i].y > P.y) && (vec_point[i + 1].y <= P.y))) { // a downward crossing顺时针画
                // compute  the actual edge-ray intersect x-coordinate
                float vt = (float) (P.y - vec_point[i].y) / (vec_point[i + 1].y - vec_point[i].y);
                if (P.x < vec_point[i].x + vt * (vec_point[i + 1].x - vec_point[i].x)) // P.x < intersect
                    ++cn;   // a valid crossing of y=P.y right of P.x
            }
        }
        // 0 if even (outside), and 1 if  odd (inside), save result of current point
        (cn & 1) ? ret_str[j] = string("yes") : ret_str[j] = string("no");
    }

    return ret_str;
}


void TestGetPointInPolygon() {
    vector<Point> target_points = {Point(50, 70), Point(100, 100), Point(30, 30), Point(50, 80), Point(90, 90)};
    vector<Point> vec_point = {Point(100, 100), Point(60, 60), Point(150, 80), Point(100, 60), Point(90, 75)};
    vector<string> ret_str = GetPointInPolygon(target_points, vec_point);

    //draw frame
    Mat frame = cv::Mat(cv::Size(200, 200), CV_8UC3);
    frame = cv::Scalar(255, 255, 255);

    //draw polygon
    auto n = vec_point.size();
    for (int i = 1; i < n; ++i) {
        line(frame, vec_point[i - 1], vec_point[i], Scalar(0, 0, 255));
    }
    line(frame, vec_point.front(), vec_point.back(), Scalar(0, 0, 255));
    // draw target points
    auto npts = target_points.size();
    for (int j = 0; j < npts; ++j) {
        if (ret_str[j] == "yes")//yes 绿色
            circle(frame, target_points[j], 1, Scalar(0, 255, 0), 2);
        else
            circle(frame, target_points[j], 1, Scalar(0, 0, 0), 2);//no 黑色
    }
    flip(frame, frame, 0); // code = 0 - x axis; 1 - y ; -1 - both
    imshow("GetPointInPolygon", frame);
}