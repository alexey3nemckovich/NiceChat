#include "stdafx.h"
#include "Camera.h"


Camera::Camera(int capIndex, int frameWidth, int frameHeight, HWND hWnd)
{
	this->capIndex = capIndex;
	this->hParentWnd = hWnd;
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	Init();
}


void Camera::Init()
{
	isOpened = false;
	capture = cv::VideoCapture();	
}


Camera::~Camera()
{
	if (isOpened) 
	{
		Close();
	}
}


void Camera::Open()
{
	capture.open(capIndex);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
	isOpened = true;
}


cv::Mat Camera::GetFrame()
{
	std::lock_guard<std::mutex> lock(camLock);
	capture >> lastFrame;
	return lastFrame;
}


void Camera::SetCapDeviceIndex(int index)
{
	if (capIndex != index)
	{
		std::lock_guard<std::mutex> lock(camLock);
		capIndex = index;
		if (isOpened)
		{
			Open();
		}
	}
}


void Camera::Close()
{
	capture.release();
	isOpened = false;
}


int Camera::GetCamsCount()
{
	cv::VideoCapture vc;
	int index = -1;
	bool notSuchIndexCam = false;
	while (!notSuchIndexCam)
	{
		index++;
		if (!vc.open(index))
		{
			notSuchIndexCam = true;
		}
	}
	return index;
}


std::vector<CaptureDevice> Camera::GetListCaps()
{
	std::vector<CaptureDevice> listCaps(0);
	CaptureDevice capDevice;
	for (int i = 0; i < 10; i++)
	{

	}
	return listCaps;
}