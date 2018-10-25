//
// Created by lee on 18-9-24.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <config.h>
#include <control_plane.h>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]) {
    ControlPlane ctrl_plane("ControlPlane");
    VideoCapture video_capture("../video/1.avi");
    VideoCapture camera_capture(0);
    Mat frame_video;
    Mat frame_cam;
    Mat gragimg;
    Mat frame_resize;
    int x = 0, y = 0, cnt = 0;

    bool show_gray = false;
    bool show_video = false;
    bool show_camera = false;
    int show_resize = -1;
    bool test_algorithm = false;


    while (true) {
        //////////////////////////////////////////
        // 创建并读取各个按钮状态
        //////////////////////////////////////////
        if (ctrl_plane.getButtonStatus(30, 30, 100, 30, "ShowVideo")) {
            show_video = !show_video;
        }
        if (ctrl_plane.getButtonStatus(30, 90, 100, 30, "ShowCamera")) {
            show_camera = !show_camera;
        }
        if (ctrl_plane.getButtonStatus(30, 150, 100, 30, "ConvertGray")) {
            show_gray = !show_gray;
        }
        if (ctrl_plane.getButtonStatus(230, 30, 100, 30, "ResizeWin")) {
            ++show_resize;
        }
        if (ctrl_plane.getButtonStatus(230, 90, 100, 30, "TestAlgorithm")) {
            test_algorithm = true;
        }
        if (ctrl_plane.getButtonStatus(ctrl_plane.m_frame.cols - 300, ctrl_plane.m_frame.rows / 2, 100, 30, "&Quit")) {
            break;
        }


        //////////////////////////////////////////
        // 判断各个按钮状态，进行对应的显示
        //////////////////////////////////////////

        // read from video
        if (show_video) {
            video_capture >> frame_video;
            if (frame_video.empty()) {
                cout << "vedio comes to the end!" << endl;
                break;
            }
            imshow("video_capture", frame_video);
        }


        //read from camera
        if (show_camera) {
            camera_capture >> frame_cam;
            if (frame_cam.empty())
                break;
            //fisrt go here, init these vars
            static bool first_flg = true;
            if (first_flg) {
                first_flg = false;
                x = frame_cam.rows;
                y = frame_cam.cols;
                namedWindow(COLOR_WIN);
                setMouseCallback(COLOR_WIN, OnMouseCallback, (void *) &frame_cam);
            }
            //draw circle
            DrawFilledCircle(frame_cam, Point(50, 50));
            // draw scrolling text on the color frame
            if (cnt % 5 == 0) {
                if (x > 20)
                    x -= 20;
                else
                    x = frame_cam.rows;
            }
            putText(frame_cam, "Hello, TA!", Point(x, y / 5), FONT_HERSHEY_COMPLEX, 2, Scalar(0, 0, 255));

            if (g_bDrawingBox)// draw rect
                DrawRectangle(frame_cam, g_rectangle);

            imshow(COLOR_WIN, frame_cam);
            cnt++;
        }

        // use button to convert the gray frame
        if (show_gray) {
            if (!frame_cam.empty()) {
                cvtColor(frame_cam, gragimg, CV_BGR2GRAY);// convert gray
                imshow(GRAY_WIN, gragimg);
            } else
                cout << "please press ShowCamera button first!" << endl;
        } else {
            gragimg.release();
            destroyWindow(GRAY_WIN);
        }

        //resize the video window
        if (show_resize >= 0) {
            if (!frame_video.empty()) {
                switch (show_resize % 2) {
                    case 0:
                        resize(frame_video, frame_resize, frame_video.size() / 2);
                        imshow("ResizeWin", frame_resize);
                        break;
                    case 1:
                        frame_resize.release();
                        destroyWindow("ResizeWin");
                    default:
                        break;
                }

            } else
                cout << "please press ShowVideo button first!" << endl;

        }

        if (test_algorithm)
            TestGetPointInPolygon();
        //update
        ctrl_plane.update();
    }

    return 0;
}