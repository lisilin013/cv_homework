/*
  ******************************************************************************
  * @file    : cali_camera.cpp
  * @author  : Silin Li
  * @version : V1.0
  * @date    : 2016.07.10
  * @note	 : calibrate camera using linear camera calibration method
  * @history :
  *
  ******************************************************************************
*/
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using namespace std;


void ImagePreProcess(const cv::Mat& src_img, const int kCornerRowNum,const int kCornerColNum,
                    const int kLeftCornerNum,const int kGridSize,
                    vector<cv::Point2d> &image_plane,vector<cv::Point3d> &object_plane);
Eigen::Matrix<double, 2, 11> AssignMatrixbyPoints(const cv::Point2d x_image, const cv::Point3d x_object);
void LinearCameraCalibration(const vector<cv::Point2d> &image_plane, const vector<cv::Point3d> &object_plane,
                             Eigen::Matrix<double, 3, 4> &M, Eigen::Matrix3d &K, Eigen::Matrix3d &R,
                             Eigen::Vector3d &t);
double CalcuReprojectionError(const vector<cv::Point2d>& image_plane, const vector<cv::Point3d>& object_plane,
            const Eigen::Matrix<double, 3, 4> &M, cv::Mat &src_img);

struct ImageLabel
{
    cv::Mat img;
    int kCornerRowNum;
    int kCornerColNum;
    int kLeftCornerNum; //including the middle line's corner
    int kGridSize;//unit: mm
    ImageLabel(const cv::Mat& _img, int _row, int _col, int _left_num, int _grid_size):
    img(_img),kCornerRowNum(_row),kCornerColNum(_col),kLeftCornerNum(_left_num),kGridSize(_grid_size){};
    ~ImageLabel(){};
};


int main(int argc, char *argv[]) {
    vector<ImageLabel> imgs;
    imgs.push_back(ImageLabel(cv::imread("../1.jpg"),10,8,5,50));
    imgs.push_back(ImageLabel(cv::imread("../2.jpg"),10,8,5,50));

    int cnt = 1;
    for (ImageLabel& elem:imgs)
    {
        cout<<"#################### On Processing The "<<cnt <<"th Image ####################\n";
        cv::Mat src_img = elem.img;
        if (src_img.empty()) 
        {
            cout << "read img failed!\n";
            exit(0);
        }
        vector<cv::Point2d> image_plane;
        vector<cv::Point3d> object_plane;

        
        ImagePreProcess(src_img, elem.kCornerRowNum,elem.kCornerColNum,
                        elem.kLeftCornerNum,elem.kGridSize,
                        image_plane,object_plane);

        // calibration
        Eigen::Matrix3d K, R;
        Eigen::Matrix<double, 3, 4> M;
        Eigen::Vector3d t;
        LinearCameraCalibration(image_plane, object_plane, M, K, R, t);

        //cal error
        double reproject_error = CalcuReprojectionError(image_plane, object_plane, M, src_img);
        // show results
        cout << "Calibration Result: " << "\n"
             << "Intrinsic Matrix: \n" << K << "\n\n"
             << "Rotation Matrix: \n" << R << "\n\n"
             << "Translation matrix: \n" << t << "\n\n"
             << "Average Reprojection Error = " << reproject_error << endl;

        cv::imshow("show cali board", src_img);
        cv::waitKey(0);
        cout<<"----------------- The "<<cnt++<<"th Image Calibrated Done!-----------------\n\n\n\n";
    }
    
    cv::waitKey(0);
    return 0;
}


void ImagePreProcess(const cv::Mat& src_img, const int kCornerRowNum,const int kCornerColNum,
                    const int kLeftCornerNum,const int kGridSize,
                    vector<cv::Point2d> &image_plane,vector<cv::Point3d> &object_plane)
{
    cv::Mat gray_img;
    cv::cvtColor(src_img, gray_img, CV_RGB2GRAY);

    cv::Size pattern_size(kCornerRowNum, kCornerColNum); //interior number of corners
    vector<cv::Point2f> corners; //this will be filled by the detected corners
    bool pattern_found = cv::findChessboardCorners(gray_img, pattern_size, corners, cv::CALIB_CB_ADAPTIVE_THRESH);
    if (pattern_found)
        cv::cornerSubPix(gray_img, corners, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    cv::drawChessboardCorners(src_img, pattern_size, cv::Mat(corners), pattern_found);

    //assign coordinate for image plane and world object_plane
    for (int i = 0; i < kCornerColNum; ++i) {
        for (int j = 0; j < kCornerRowNum; ++j) {
            image_plane.push_back(corners[kCornerRowNum * i + j]);
            cv::Point3d object_point;
            if (j < kLeftCornerNum)//left
            {
                object_point.x = kGridSize * (kLeftCornerNum - j-1);
                object_point.y = 0.0;
                object_point.z = kGridSize * (kCornerColNum - i - 1);
            }
            else//right
            {
                object_point.x = 0.0;
                object_point.y = kGridSize * (j + 1 - kLeftCornerNum);
                object_point.z = kGridSize * (kCornerColNum - i - 1);
            }
            object_plane.push_back(object_point);
        }
    }
}

Eigen::Matrix<double, 2, 11> AssignMatrixbyPoints(const cv::Point2d x_image, const cv::Point3d x_object) {
    Eigen::Matrix<double, 2, 11> m = Eigen::Matrix<double, 2, 11>::Zero();
    m(0, 0) = x_object.x;
    m(0, 1) = x_object.y;
    m(0, 2) = x_object.z;
    m(0, 3) = 1;
    m(0, 8) = -x_image.x * x_object.x;
    m(0, 9) = -x_image.x * x_object.y;
    m(0, 10) = -x_image.x * x_object.z;

    m(1, 4) = x_object.x;
    m(1, 5) = x_object.y;
    m(1, 6) = x_object.z;
    m(1, 7) = 1;
    m(1, 8) = -x_image.y * x_object.x;
    m(1, 9) = -x_image.y * x_object.y;
    m(1, 10) = -x_image.y * x_object.z;

    return m;
}

void LinearCameraCalibration(const vector<cv::Point2d> &image_plane, const vector<cv::Point3d> &object_plane,
                             Eigen::Matrix<double, 3, 4> &M, Eigen::Matrix3d &K, Eigen::Matrix3d &R,
                             Eigen::Vector3d &t) {
    if (image_plane.size() != object_plane.size()) {
        cout << "image point num not equal object point num!\n";
        exit(0);
    }
    size_t point_num = image_plane.size();
    Eigen::MatrixXd p_mat(2 * point_num, 11);//2N*11
    Eigen::MatrixXd b_mat(2 * point_num, 1);//2N*1
    for (int i = 0; i < point_num; ++i) {
        p_mat.block<2, 11>(i * 2, 0) = AssignMatrixbyPoints(image_plane[i], object_plane[i]);
        b_mat(i * 2, 0) = image_plane[i].x;
        b_mat(i * 2 + 1, 0) = image_plane[i].y;
    }

    // cout<<b_mat<<endl;
    //solve equation p_mat*m=b_mat by SVD method
    Eigen::VectorXd m = p_mat.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b_mat);

    //estimate intrinsic param, use Linear camera calibration II
    Eigen::Vector3d m1 = {m(0), m(1), m(2)};
    Eigen::Vector3d m2 = {m(4), m(5), m(6)};
    Eigen::Vector3d m3 = {m(8), m(9), m(10)};
    Eigen::Vector3d b = {m(3), m(7), 1};//m(11)=1
    double m34 = 1 / m3.norm();//recover real m34 in Matrix m
    Eigen::Vector3d m1m3_cross = m1.cross(m3);
    Eigen::Vector3d m2m3_cross = m2.cross(m3);

    Eigen::Vector3d r3 = m34 * m3;
    double alpha = m34 * m34 * m1m3_cross.norm();
    double beta = m34 * m34 * m2m3_cross.norm();
    double u0 = m34 * m34 * m1.dot(m3);
    double v0 = m34 * m34 * m2.dot(m3);
    Eigen::Vector3d r1 = m34 / alpha * (m1 - u0 * m3);
    Eigen::Vector3d r2 = m34 / beta * (m2 - v0 * m3);

    M << m(0), m(1), m(2), m(3),
            m(4), m(5), m(6), m(7),
            m(8), m(9), m(10), 1;
    K << alpha, 0, u0,
            0, beta, v0,
            0, 0, 1;
    t = m34 * K.inverse() * b;

    R.col(0) = r1;
    R.col(1) = r2;
    R.col(2) = r3;
}

double CalcuReprojectionError(const vector<cv::Point2d>& image_plane, const vector<cv::Point3d>& object_plane,
            const Eigen::Matrix<double, 3, 4> &M, cv::Mat &src_img)
{
    double reproject_error = 0;
    for (int i = 0; i < image_plane.size(); ++i) {
        Eigen::Vector3d reproject_image_point =
                M * Eigen::Vector4d(object_plane[i].x, object_plane[i].y, object_plane[i].z, 1);
        cv::Point2d real_image_point(reproject_image_point(0)/reproject_image_point(2),
            reproject_image_point(1)/reproject_image_point(2));
        reproject_error += pow((real_image_point-image_plane[i]).x,2) + pow((real_image_point-image_plane[i]).y,2);

        cv::circle(src_img, real_image_point, 3, cv::Scalar(0, 0, 255), 2);
    }
    reproject_error /= image_plane.size();

    return reproject_error;
}




