#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

class CameraCalibrator
{
private:
	//世界坐标
	vector <vector<Point3f >> objectPoints;
	//图像坐标
	vector <vector<Point2f>> imagePoints;
	//输出矩阵
	Mat camerMatirx;
	Mat disCoeffs;
	//标记
	int flag;
	//去畸变参数
	Mat map1, map2;
	//是否去畸变
	bool mustInitUndistort;

	//保存点数据
	void addPoints(const vector<Point2f> & imageConers, const vector<Point3f> &objectConers)
	{
		imagePoints.push_back(imageConers);
		objectPoints.push_back(objectConers);
	}

public:
	CameraCalibrator():flag(0),mustInitUndistort(true){}

	//打开棋盘图片，提取角点
	int addChessboardPoints(const vector<string> &filelist, Size &boardSize)
	{
		vector<Point2f> imageConers;
		vector<Point3f> objectConers;
		//输入角点的世界坐标 
		for (int i = 0; i < boardSize.height; i++)
		{
			for (int j = 0; j < boardSize.width; j++)
			{
				objectConers.push_back(cv::Point3f(i, j, 0.0f));
			}
		}
		//计算角点在图像中的坐标
		Mat image;
		int success = 0;
		for (int i = 0; i < filelist.size(); i++)
		{
			image = imread(filelist[i]);
			Mat gray;
			cvtColor(image, gray, CV_BGR2GRAY);
			//找到角点坐标
			bool found = findChessboardCorners(gray, boardSize, imageConers);
			cornerSubPix(gray, imageConers, Size(5, 5), Size(-1, -1),
				TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS,30,0.1));
			if (imageConers.size() == boardSize.area())
			{
				addPoints(imageConers, objectConers);
				success++;
			}
			//画出角点
			drawChessboardCorners(image, boardSize, imageConers, found);
			imshow("Corners on Chessboard", image);
			waitKey(100);
		}
		return success;
	}
	//相机标定
	double calibrate(Size &imageSize)
	{
		mustInitUndistort = true;
		vector<Mat> rvecs, tvecs;
		//相机标定
		return calibrateCamera(objectPoints, imagePoints, imageSize, camerMatirx, disCoeffs, rvecs, tvecs, flag);
	}
	//去畸变
	Mat remap(const Mat image)
	{
		Mat undistorted;
		if (mustInitUndistort)
		{
			//计算畸变参数
			initUndistortRectifyMap(camerMatirx, disCoeffs, Mat(), Mat(), image.size(), CV_32FC1, map1, map2);
			mustInitUndistort = false;
		}
		cv::remap(image, undistorted, map1, map2, INTER_LINEAR);
		return undistorted;
	}
	//常成员函数，获得相机内参数矩阵，投影矩阵数据
	Mat getCameraMatrix() const { return camerMatirx; }
	Mat getDistCoeffs() const { return disCoeffs; }
};