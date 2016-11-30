#pragma once
#include "CaptureDevice.h"
#include "WindowManager.h"
#include <opencv2\opencv.hpp>
#include <mutex>
#include <thread>


class Camera
{
private:
	HWND hParentWnd;
	HWND hCapWnd;
	int capIndex;
	int camsCount;
	int frameWidth;
	int frameHeight;
	void Init();
	std::mutex camLock;
	cv::Mat lastFrame;
	cv::VideoCapture capture;
	bool isOpened;
public:
	Camera(int capIndex, int frameWidth, int frameHeight, HWND hWnd);
	~Camera();
	int GetCapDeviceNumber()
	{
		return capIndex;
	}
	void SetCapDeviceIndex(int index);
	cv::Mat GetFrame();
	void Open();
	void Close();
	static int GetCamsCount();
	static std::vector<CaptureDevice> GetListFreeCaps();
};