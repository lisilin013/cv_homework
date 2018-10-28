#ifndef __MAIN_H__
#define __MAIN_H__ 

#include <opencv2/opencv.hpp>
#include <string>

enum SkinDetectType
{
	IMAGE,
	VEDIO
};
class SkinDetector
{
public:
	explicit SkinDetector(std::string path, SkinDetectType type):_path(path),_type(type)
	{}
	~SkinDetector(){}
	void run();//just call this
private:
	std::string _path;
	SkinDetectType _type;
	void detectSkinHSV(const cv::Mat& srcImage, cv::Mat &dstImage);
};

#endif