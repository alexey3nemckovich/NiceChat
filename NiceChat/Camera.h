#pragma once


#define CAM_FRAME_WIDTH 640
#define CAM_FRAME_HEIGHT 480


#include "CaptureDevice.h"
#include "WindowManager.h"
#include <opencv2\opencv.hpp>
#include <mutex>
#include <thread>


class Camera
{
private:
	int capIndex;
	int camsCount;
	int frameWidth;
	int frameHeight;
	void Init();
	std::mutex camLock;
	cv::Mat lastFrame;
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
	cv::Mat GetFrame();
	int GetCapDeviceNumber()
	{
		return capIndex;
	}
	void SetCapDeviceIndex(int index);
	static std::vector<int> GetListFreeCapsIndexes();
	static Camera* GetInstance();
};