#include "stdafx.h"
#include "Camera.h"


Camera::Camera(int capIndex, HWND hWnd)
{
	this->capIndex = capIndex;
	this->hParentWnd = hWnd;
	Init();
}


void Camera::Init()
{
	capture = cv::VideoCapture();
}


Camera::~Camera()
{

}


void Camera::Open()
{
	capture.open(capIndex);
}


cv::Mat Camera::GetFrame()
{
	static HBITMAP hBmp;
	capture >> lastFrame;
	return lastFrame;
}


void Camera::Close()
{
	capture.release();
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