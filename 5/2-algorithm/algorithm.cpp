//std
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
// opencv
#include <opencv2/opencv.hpp>
#include <unordered_map>


using namespace std;
using namespace cv;


class FillArea
{
public:
	int id_region;
	unordered_map<int, vector<Point>> regions;
	Mat img;
	Mat pixel_region_id;
	// vector<vector<int>> mat;
public:
FillArea(Mat& _bin_img):img(_bin_img)//can write the img
{
	pixel_region_id = Mat::zeros(img.size(), CV_32SC1);
	id_region = 1;
	// mat = vector<vector<int>>(img.rows, vector<int>(img.cols,0));
}
~FillArea(){}

int getPixel(int row, int col)
{
	return int(img.at<unsigned char>(row,col))>0?1:0;
}
int getPixelUp(int row, int col)
{
	if(row>1)
		return int(img.at<unsigned char>(row-1,col))>0?1:0;
	else
		return -1;
}
int getPixelLeft(int row, int col)
{
	if(col>1)
		return int(img.at<unsigned char>(row,col-1))>0?1:0;
	else
		return-1;
}
void setPixelRegionID(int row, int col,unsigned int id)
{
	pixel_region_id.at<signed int>(row,col) = id;
}

int getPixelReigionID(int row, int col)
{
	int id = pixel_region_id.at<signed int>(row,col);
	return id;
}

int findArea()
{
	size_t rows = img.rows;
	size_t cols = img.cols;
	// int area_cnt = 0;
	//first pass
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			int pixel = getPixel(i,j);
			if(pixel)//+
			{
				if(getPixelUp(i,j)<=0 && getPixelLeft(i,j)<=0)//non-exist or pixel is zero
				{
					// regions.insert(td::make_pair<int, vector<Point>>(id_region,6.0), vector<Point>(Point(i,j)));
					regions[id_region] = vector<Point>(1,Point(i,j));
					setPixelRegionID(i,j,id_region);
					id_region++;
					cout<<"id_region: "<<id_region<<endl;
				}
				else if(getPixelUp(i,j)<=0 && getPixelLeft(i,j)>0)
				{
					int reg_id = getPixelReigionID(i, j-1);//left
					regions[reg_id].push_back(Point(i,j));
					setPixelRegionID(i,j,reg_id);
				}
				else if(getPixelUp(i,j)>0 && getPixelLeft(i,j)<=0)
				{
					int reg_id = getPixelReigionID(i-1, j);//up
					regions[reg_id].push_back(Point(i,j));
					setPixelRegionID(i,j,reg_id);
				}
				else
				{
					int reg_left_id = getPixelReigionID(i, j-1);//left
					int reg_up_id = getPixelReigionID(i-1, j);//up
					int reg_id = min(reg_left_id, reg_up_id);
					regions[reg_id].push_back(Point(i,j));
					if(reg_id == reg_up_id)//put to up region
					{
						for (auto& elem: regions[reg_left_id])
						{
							regions[reg_id].push_back(elem);
							setPixelRegionID(i,j,reg_id);
						}
						regions.erase(reg_left_id);
					}
					else
					{
						for (auto& elem: regions[reg_up_id])
						{
							regions[reg_id].push_back(elem);
							setPixelRegionID(i,j,reg_id);
						}
						regions.erase(reg_up_id);
					}
				}

			}
		}
	}

	return regions.size();
}
};


int main()
{
	Mat gray_img, bin_img;
	gray_img = imread("../lenna.jpg",0);	//将读入的彩色图像直接以灰度图像读入
	imshow("src img", gray_img);
	bin_img = gray_img.clone();
	//进行二值化处理，选择30，200.0为阈值
	threshold(gray_img, bin_img, 100, 255, CV_THRESH_BINARY);

	//algorithm
	FillArea find_area(bin_img);
	cout<<"\nThe num of area is: "<<find_area.findArea()<<endl;

	imshow("bin img", bin_img);
	waitKey(0);
	return 0;
}


