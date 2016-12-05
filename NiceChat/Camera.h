#pragma once


#include "CaptureDevice.h"
#include "WindowManager.h"
#include <opencv2\opencv.hpp>
#include <mutex>
#include <thread>


struct CamFrame
{
	cv::Mat img;
	int size;
};


class Camera
{
private:
	int capIndex;
	std::mutex camLock;
	int imgSize;
	cv::Mat img;
	cv::Mat imgGray;
	cv::VideoCapture capture;
	bool isOpened;
	void Init();
	Camera();
	~Camera();
public:
	void Open();
	void Close();
	bool IsOpened()
	{
		return isOpened;
	}
	bool IsAvailable();
	CamFrame GetFrame();
	int GetCapDeviceNumber()
	{
		return capIndex;
	}
	void SetCapDeviceIndex(int index);
	static std::vector<int> GetListFreeCapsIndexes();
	static Camera* GetInstance();
};