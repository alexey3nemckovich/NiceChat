#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	capIndex = 0;
	Init();
}


void Camera::Init()
{
	isOpened = false;
	capture = cv::VideoCapture();
	img = cv::Mat::zeros(CAM_FRAME_HEIGHT, CAM_FRAME_WIDTH, CV_8UC1);
	if (!img.isContinuous())
	{
		img = img.clone();
		imgGray = imgGray.clone();
	}
	imgSize = img.total() * img.elemSize();
}


Camera::~Camera()
{
	if (isOpened)
	{
		Close();
	}
}


Camera* Camera::GetInstance()
{
	static Camera camera;
	return &camera;
}


void Camera::Open()
{
	capture.open(capIndex);
	capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	capture.set(CV_CAP_PROP_FRAME_WIDTH, CAM_FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_FRAME_HEIGHT);
	isOpened = true;
}


void Camera::Close()
{
	capture.release();
	isOpened = false;
}


bool Camera::IsAvailable()
{
	cv::VideoCapture vc;
	if (vc.open(capIndex))
	{
		vc.release();
		return true;
	}
	return false;
}


CamFrame Camera::GetFrame()
{
	static CamFrame frame{imgGray, imgSize};
	std::lock_guard<std::mutex> lock(camLock);
	capture >> img;
	cv::cvtColor(img, imgGray, CV_BGR2GRAY);
	return frame;
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


std::vector<int> Camera::GetListFreeCapsIndexes()
{
	std::vector<int> freeCapsIndexes;
	cv::VideoCapture vc;
	for (int i = 0; i < 10; i++)
	{
		if (vc.open(i))
		{
			freeCapsIndexes.push_back(i);
		}
	}
	return freeCapsIndexes;
}