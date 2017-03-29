// Calibration_OpenCV.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include "Calibration.h"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

int main()
{
	CameraCalibrator Cc;
	Mat image;
	vector<string> filelist;
	//namedWindow("Image");
	for (int i = 1; i <= 5; i++)
	{
		string tmp_str;
		tmp_str = "image\\" + to_string(i) + ".bmp";
		filelist.push_back(tmp_str);
		image = imread(tmp_str);
		imshow("image", image);
		waitKey(100);
	}
	//image = imread("image\\1.bmp");
	//vector<Point2f> imageConers;
	//bool found = findChessboardCorners(image, Size(9, 6), imageConers);
	//相机标定
	cv::Size boardSize(9, 6);
	Cc.addChessboardPoints(filelist, boardSize);
	Cc.calibrate(image.size());
	//输出
	cout << Cc.getCameraMatrix() << endl;
	cout << Cc.getDistCoeffs() << endl;
	//去畸变
	Mat image2 = imread("image\\1.bmp");
	Mat uImage2 = Cc.remap(image2);
	imshow("原图像", image2);
	imshow("去畸变", uImage2);
	waitKey(0);

	//局部去畸变测试
	Mat image3 = image2(Rect(350,40,750,120));
	Mat maprect1 = Cc.map1(Rect(350, 40, 750, 120))-350;
	Mat maprect2 = Cc.map2(Rect(350, 40, 750, 120))-40;
	Mat uImage3;
	double time;
	time = (double)getTickCount();
	cv::remap(image3, uImage3, maprect1, maprect2, INTER_LINEAR);
	time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
cout << time << endl;
	imshow("原图像区域", image3);
	imshow("去畸变后的区域", uImage3);

	waitKey(0);
	system("pause");
	return 0;
}
