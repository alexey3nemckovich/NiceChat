#pragma once
#include <opencv2\opencv.hpp>


class ImageProcesser
{
private:
	ImageProcesser();
	~ImageProcesser();
public:
	static const ImageProcesser* GetInstance();
	HBITMAP ConvertCVMatToHBITMAP(cv::Mat mat) const;
};