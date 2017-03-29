// Calibration_OpenCV.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//����궨
	cv::Size boardSize(9, 6);
	Cc.addChessboardPoints(filelist, boardSize);
	Cc.calibrate(image.size());
	//���
	cout << Cc.getCameraMatrix() << endl;
	cout << Cc.getDistCoeffs() << endl;
	//ȥ����
	Mat image2 = imread("image\\1.bmp");
	Mat uImage2 = Cc.remap(image2);
	imshow("ԭͼ��", image2);
	imshow("ȥ����", uImage2);
	waitKey(0);

	//�ֲ�ȥ�������
	Mat image3 = image2(Rect(350,40,750,120));
	Mat maprect1 = Cc.map1(Rect(350, 40, 750, 120))-350;
	Mat maprect2 = Cc.map2(Rect(350, 40, 750, 120))-40;
	Mat uImage3;
	double time;
	time = (double)getTickCount();
	cv::remap(image3, uImage3, maprect1, maprect2, INTER_LINEAR);
	time = 1000 * ((double)getTickCount() - time) / getTickFrequency();
cout << time << endl;
	imshow("ԭͼ������", image3);
	imshow("ȥ����������", uImage3);

	waitKey(0);
	system("pause");
	return 0;
}
