//
// Created by lee on 18-9-23.
//

#ifndef LEARNING_OPENCV_CONFIG_H
#define LEARNING_OPENCV_CONFIG_H

#include <opencv2/opencv.hpp>

#define COLOR_WIN "camera capture"
#define GRAY_WIN "gray capture"

extern cv::Rect g_rectangle;
extern bool g_bDrawingBox;//是否进行绘制

void DrawFilledCircle(cv::Mat img, cv::Point center);

void DrawRectangle(cv::Mat &img, cv::Rect box);

void OnMouseCallback(int event, int x, int y, int flags, void *param);

void TestGetPointInPolygon();

#endif //LEARNING_OPENCV_CONFIG_H
