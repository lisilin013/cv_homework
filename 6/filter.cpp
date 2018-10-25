/*
  ******************************************************************************
  * @file   : filter.cpp
  * @author : Silin Li
  * @version: V1.0
  * @date   : 2018.10.21
  * @note   : implementation for mean-filter, median-filter and gaussian-filter
  * @history:
  *
  ******************************************************************************
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;
using namespace cv;


/**
  * @brief : mean filter with normalize kernel
  * @retval: dst
  */
void MeanFilter(const Mat &src, Mat &dst, const Size &size) {
    //get kernel
    Mat ker;
    ker = Mat::ones(size, CV_8U);
    ker.convertTo(ker, CV_32F);
    ker = ker / (size.width * size.height);

    //don not care edge
    int rows = src.rows;
    int cols = src.cols;
    dst = src.clone();
    for (int i = 0; i < rows - size.height; ++i) {
        for (int j = 0; j < cols - size.width; ++j) {
            Mat sub_mat = src(Rect(Point(j, i), ker.size())); //width , height
            sub_mat.convertTo(sub_mat, CV_32F); //convert to float
            dst.at<uchar>(i, j) = sub_mat.dot(ker); //converlution
        }
    }
}


/**
  * @brief : Get the mid value of a full mat
  * @retval: none
  */
uchar GetMidValofMat(const Mat &mat) {
    vector<uchar> nums(mat.rows * mat.cols, 0);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            nums[i * mat.rows + j] = mat.at<uchar>(i, j);
        }
    }

    std::sort(nums.begin(), nums.end());
    return nums[nums.size() / 2 + 1];
}


/**
  * @brief : median filter
  * @retval: dst
  */
void MedianFilter(const Mat &src, Mat &dst, const Size &size) {
    dst = src.clone();
    int dt = size.height / 2;
    int rows = src.rows;
    int cols = src.cols;

    for (int i = 0; i < rows - size.height; ++i) {
        for (int j = 0; j < cols - size.width; ++j) {
            Mat sub_mat = src(Rect(Point(j, i), size)); //width , height
            double mid_val = GetMidValofMat(sub_mat);
            dst.at<uchar>(i, j) = mid_val;
        }
    }
}


/**
  * @brief : Get gaussian kernel
  * @retval: ker
  */
void GetGaussianFilterKernelFast(Mat &ker, int ksize, double sigma) {
    ker = Mat::zeros(ksize, 1, CV_32F);
    static const double pi = 3.1415926;
    int center = ksize / 2; // 模板的中心位置，也就是坐标的原点
    double x2;
    for (int i = 0; i < ksize; i++) {
        x2 = pow(i - center, 2);
        double g = exp(-(x2) / (2 * sigma * sigma)) / (sqrt(2 * pi) * sigma);
        ker.at<float>(i, 0) = g;
    }

    float sum = 0; // normalize
    for (int i = 0; i < ksize; i++) {
        sum += ker.at<float>(i, 0);
    }
    ker = ker / sum;
}


/**
  * @brief : gaussian filter to do converlution independently with each direction x and y
  * @retval: dst
  */
void GaussianFilterFaster(const Mat &src, Mat &dst, const int ksize, double sigmaX, double sigmaY = 0) {
    double epsilon = 0.000001;
    dst = src.clone();

    // get gaussian kernel
    Mat kerx, kery;
    GetGaussianFilterKernelFast(kerx, ksize, sigmaX);
    cout << "Gaussian Kernel in direction X is: \n";
    cout << kerx << endl;
//    cout<<getGaussianKernel(ksize, sigmaX, CV_32F)<<endl;
    if (abs(sigmaY) < epsilon) {
        kery = kerx.clone();
    }
    else {
        GetGaussianFilterKernelFast(kery, ksize, sigmaY);
    }
    cout << "Gaussian Kernel in direction Y is: \n";
    cout << kery << endl;

    //don not care edge
    int rows = src.rows;
    int cols = src.cols;
    for (int i = 0; i < rows - ksize; ++i) {
        for (int j = 0; j < cols - ksize; ++j) {
            Mat sub_mat = src(Rect(Point(j, i), Size(ksize, ksize))); //width , height
            sub_mat.convertTo(sub_mat, CV_32F);
            dst.at<uchar>(i, j) = kery.dot(sub_mat * kerx);
        }
    }
}


/**
  * @brief : main
  * @retval: 
  */
int main(int argc, char *argv[]) {
    Mat src_img = cv::imread("../lenna.png", 0);//read one channel
    if (src_img.empty()) {
        cout << "read img failed!\n";
        exit(0);
    }

    Mat mean_img, median_img, gauss_img;
    Mat mean_img1, median_img1, gauss_img1;
    Mat mean_img2, median_img2, gauss_img2;

    //opencv filter
    blur(src_img, mean_img, Size(3, 3));
    medianBlur(src_img, median_img, 3);
    GaussianBlur(src_img, gauss_img, Size(3, 3), 1.5, 5);

    //self filter size 3*3
    MeanFilter(src_img, mean_img1, Size(3, 3));
    MedianFilter(src_img, median_img1, Size(3, 3));
    GaussianFilterFaster(src_img, gauss_img1, 3, 1.5, 5);

    //self filter size 5*5
    MeanFilter(src_img, mean_img2, Size(5, 5));
    MedianFilter(src_img, median_img2, Size(5, 5));
    GaussianFilterFaster(src_img, gauss_img2, 5, 1.5, 5);

    cv::imshow("src img", src_img);
    cv::imshow("mean5*5 img", mean_img2);
    cv::imshow("mean3*3 img", mean_img1);
    cv::imshow("median5*5 img", median_img2);
    cv::imshow("median3*3 img", median_img1);
    cv::imshow("gauss5*5 img", gauss_img2);
    cv::imshow("gauss3*3 img", gauss_img1);

    cv::imwrite("../result_img/mean5*5.jpg", mean_img2);
    cv::imwrite("../result_img/mean3*3.jpg", mean_img1);
    cv::imwrite("../result_img/median5*5.jpg", median_img2);
    cv::imwrite("../result_img/median3*3.jpg", median_img1);
    cv::imwrite("../result_img/gauss5*5.jpg", gauss_img2);
    cv::imwrite("../result_img/gauss3*3.jpg", gauss_img1);

    cv::waitKey(0);
    return 0;
}

