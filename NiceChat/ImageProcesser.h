#pragma once
#include <opencv2\opencv.hpp>


class ImageProcesser
{
private:
	ImageProcesser();
	~ImageProcesser();
	BITMAPINFOHEADER initBmpInfoHeader;
	BITMAPINFO initBmpInfo;
public:
	static const ImageProcesser* GetInstance();
	HBITMAP ConvertCVMatToHBITMAP(cv::Mat mat) const;
	HBITMAP GetBitmapFromData(const uchar *data) const;
};