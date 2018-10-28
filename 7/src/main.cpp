/*
  ******************************************************************************
  * @file   : main.cpp
  * @author : Silin Li
  * @version: V1.0
  * @date   : 2018.10.28
  * @note   : implmentation for skin detect with img and vedio
  * @history:
  *
  ******************************************************************************
*/

#include <opencv2/opencv.hpp>
#include <vector>
#include "skin_detect.h"
using namespace std;
using namespace cv;

int main()
{
	//process img
	vector<string> srcimage_path{
		"../../data/1.png",
		"../../data/2.png"
	};
	Mat src, dst;
	cout << "begin process image " << endl;
	for (auto img_path : srcimage_path)
	{
		SkinDetector skin_detect(img_path, IMAGE);
		skin_detect.run();
	}

	//processe vedio
	cout << "begin process video " << endl;
	SkinDetector skin_detect("../../data/Liyifeng.avi", VEDIO);	
	skin_detect.run();

	return 0;
}
