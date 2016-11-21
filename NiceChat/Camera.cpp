#include "stdafx.h"
#include "Camera.h"


Camera::Camera(CaptureDevice capDevice)
{
	this->capDevice = capDevice;
}


Camera::~Camera()
{

}


std::vector<CaptureDevice> Camera::GetListCaps()
{
	std::vector<CaptureDevice> listCaps(0);
	CaptureDevice capDevice;
	for (int i = 0; i < 10; i++)
	{
		if (capGetDriverDescription(i, (TCHAR*)capDevice.lpszName, capDevice.cbName - 1, (TCHAR*)capDevice.lpszDescription, capDevice.cbVer - 1))
		{
			capDevice.wIndex = i;
			listCaps.push_back(capDevice);
		}
	}
	return listCaps;
}