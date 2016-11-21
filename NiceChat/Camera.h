#pragma once
#include <Vfw.h>
#include "CaptureDevice.h"


class Camera
{
private:
	CaptureDevice capDevice;
public:
	Camera(CaptureDevice capDevice);
	~Camera();
	CaptureDevice GetCapDevice()
	{
		return capDevice;
	}
	static std::vector<CaptureDevice> GetListCaps();
};