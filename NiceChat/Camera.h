#pragma once
#include "CaptureDevice.h"
#include "WindowManager.h"
#include <opencv2\opencv.hpp>


class Camera
{
private:
	HWND hParentWnd;
	HWND hCapWnd;
	int capIndex;
	int camsCount;
	void Init();
	cv::Mat lastFrame;
	cv::VideoCapture capture;
public:
	Camera(int capIndex, HWND hWnd);
	~Camera();
	int GetCapDeviceNumber()
	{
		return capIndex;
	}
	cv::Mat GetFrame();
	void Open();
	void Close();
	static int GetCamsCount();
	static std::vector<CaptureDevice> GetListCaps();
};