#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

class CameraCalibrator
{
private:
	//��������
	vector <vector<Point3f >> objectPoints;
	//ͼ������
	vector <vector<Point2f>> imagePoints;
	//�������
	Mat camerMatirx;
	Mat disCoeffs;
	//���
	int flag;
	//ȥ�������
	Mat map1, map2;
	//�Ƿ�ȥ����
	bool mustInitUndistort;

	//���������
	void addPoints(const vector<Point2f> & imageConers, const vector<Point3f> &objectConers)
	{
		imagePoints.push_back(imageConers);
		objectPoints.push_back(objectConers);
	}

public:
	CameraCalibrator():flag(0),mustInitUndistort(true){}

	//������ͼƬ����ȡ�ǵ�
	int addChessboardPoints(const vector<string> &filelist, Size &boardSize)
	{
		vector<Point2f> imageConers;
		vector<Point3f> objectConers;
		//����ǵ���������� 
		for (int i = 0; i < boardSize.height; i++)
		{
			for (int j = 0; j < boardSize.width; j++)
			{
				objectConers.push_back(cv::Point3f(i, j, 0.0f));
			}
		}
		//����ǵ���ͼ���е�����
		Mat image;
		int success = 0;
		for (int i = 0; i < filelist.size(); i++)
		{
			image = imread(filelist[i]);
			Mat gray;
			cvtColor(image, gray, CV_BGR2GRAY);
			//�ҵ��ǵ�����
			bool found = findChessboardCorners(gray, boardSize, imageConers);
			cornerSubPix(gray, imageConers, Size(5, 5), Size(-1, -1),
				TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS,30,0.1));
			if (imageConers.size() == boardSize.area())
			{
				addPoints(imageConers, objectConers);
				success++;
			}
			//�����ǵ�
			drawChessboardCorners(image, boardSize, imageConers, found);
			imshow("Corners on Chessboard", image);
			waitKey(100);
		}
		return success;
	}
	//����궨
	double calibrate(Size &imageSize)
	{
		mustInitUndistort = true;
		vector<Mat> rvecs, tvecs;
		//����궨
		return calibrateCamera(objectPoints, imagePoints, imageSize, camerMatirx, disCoeffs, rvecs, tvecs, flag);
	}
	//ȥ����
	Mat remap(const Mat image)
	{
		Mat undistorted;
		if (mustInitUndistort)
		{
			//����������
			initUndistortRectifyMap(camerMatirx, disCoeffs, Mat(), Mat(), image.size(), CV_32FC1, map1, map2);
			mustInitUndistort = false;
		}
		cv::remap(image, undistorted, map1, map2, INTER_LINEAR);
		return undistorted;
	}
	//����Ա�������������ڲ�������ͶӰ��������
	Mat getCameraMatrix() const { return camerMatirx; }
	Mat getDistCoeffs() const { return disCoeffs; }
};