//
// Created by lee on 18-9-24.
//

#ifndef LEARNING_OPENCV_CONTROL_PLANE_H
#define LEARNING_OPENCV_CONTROL_PLANE_H

#include <string>
#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION

#include "cvui.h"

class ControlPlane {
public:
    ControlPlane(std::string winname) : m_name_window(winname) {
        cv::namedWindow(m_name_window);

        // Init cvui and tell it to use a value of 20 for cv::waitKey()
        // because we want to enable keyboard shortcut for
        // all components, e.g. button with label "&Quit".
        // If cvui has a value for waitKey, it will call
        // waitKey() automatically for us within cvui::update().
        cvui::init(m_name_window, 30);

        m_frame = cv::Mat(cv::Size(650, 200), CV_8UC3);
        m_frame = cv::Scalar(49, 52, 49);
    }

    ~ControlPlane() {}

    void update() {
        // Since cvui::init() received a param regarding waitKey,
        // there is no need to call cv::waitKey() anymore. cvui::update()
        // will do it automatically.
        cvui::update();
        cv::imshow(m_name_window, m_frame);
    }

    bool getButtonStatus(int theX, int theY, int theWidth, int theHeight, const cv::String &theLabel) {
        return cvui::button(m_frame, theX, theY, theWidth, theHeight, theLabel);
    }

private:
    const std::string m_name_window;
public:
    cv::Mat m_frame;
};


#endif //LEARNING_OPENCV_CONTROL_PLANE_H
