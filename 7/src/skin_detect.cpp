#include "skin_detect.h"
using namespace std;
using namespace cv;


void SkinDetector::run()
{
	Mat src, dst;
	if(_type==IMAGE)
	{
		Mat src = imread(_path);
		if (!src.empty())
		{
			detectSkinHSV(src, dst);
			imshow(_path, dst);
			imwrite(_path.substr(0,10)+"_output/"+_path.substr(11,1)+"_output.png",dst);
			waitKey(0);
		}
		else {
			cout << "read src image error" << endl;
		}
	}
	else if(_type==VEDIO)
	{
		VideoCapture capture(_path);
		if (!capture.isOpened())
		{
			cout << "read vedio error" << endl;
		}
		// get rate of the video
		double rate = capture.get(CV_CAP_PROP_FPS);
		namedWindow("vedio_output");
		VideoWriter video("../../data_output/vedio_output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, dst.size());
		int delay = 1000 / rate;
		while (cv::waitKey(1) < 0)
		{
			if (!capture.read(src))
			{
				cout << "capture image failed!" << endl;
				break;
			}
			detectSkinHSV(src, dst);
			//video.write(dst);
			imshow("vedio_output", dst);
		}
	}
	else
	{
		cout<<"SkinDetectType is wrong!\n";
		exit(0);
	}
}
void SkinDetector::detectSkinHSV(const Mat& srcImage, Mat &dstImage)
{
	int param_hsv1[][3] = {
		{ 3,13,50 },
		{ 63,172,200 },
		{ 97,246,255 } };
	int param_hsv2[][3] = {
		{ 3,14,50 },
		{ 44,99,200 },
		{ 85,243,255 } };
	int BilateralFilterVal = 13;
	int MorphExVal = 1;
	int MorphExVal2 = 3;
	int Morphtimes = 5;
	
	//1. Filt image with Bilateral
	Mat morph_struct = getStructuringElement(MORPH_RECT, Size(MorphExVal * 2 + 1, MorphExVal * 2 + 1));
	Mat filter_result, morph_result, transform_3, hsv_result, temp;
	bilateralFilter(srcImage, filter_result, BilateralFilterVal, BilateralFilterVal * 2, BilateralFilterVal / 2);
	
	//2. Do MORPH_OPEN and MORPH_CLOSE operate to filtered image
	morphologyEx(filter_result, temp, MORPH_OPEN, morph_struct);
	morphologyEx(temp, morph_result, MORPH_CLOSE, morph_struct);
	hsv_result = Mat::zeros(morph_result.size(), CV_32FC3);

	//3. Convert BGR-Space to HSV-Space
	cvtColor(morph_result, hsv_result, CV_BGR2HSV);

	//4. Use color segmentation to get the mask
	dstImage = Mat::zeros(hsv_result.size(), CV_8UC3);
	Mat mask, mask_;
	inRange(hsv_result, Scalar(param_hsv1[0][0], param_hsv1[1][0], param_hsv1[2][0]),
		Scalar(param_hsv1[0][1], param_hsv1[1][1], param_hsv1[2][1]), mask);
	
	//5. Do MORPH_CLOSE and MORPH_OPEN operate to mask image
	morph_struct = getStructuringElement(MORPH_RECT, Size(MorphExVal2 * 2 + 1, MorphExVal2 * 2 + 1));
	morphologyEx(mask, mask, MORPH_CLOSE, morph_struct);
	morphologyEx(mask, mask, MORPH_OPEN, morph_struct);

	//6. Get dst image by mask
	srcImage.copyTo(dstImage, mask);
}
