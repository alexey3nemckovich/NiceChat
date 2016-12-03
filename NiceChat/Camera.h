#pragma once


#include "CaptureDevice.h"
#include "WindowManager.h"
#include <opencv2\opencv.hpp>
#include <mutex>
#include <thread>


struct CamFrame
{
	const uchar *data;
	int size;
};


class Camera
{
private:
	int capIndex;
	int camsCount;
	int frameWidth;
	int frameHeight;
	void Init();
	std::mutex camLock;
	cv::Mat lastMat;
	cv::VideoCapture capture;
	bool isOpened;
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