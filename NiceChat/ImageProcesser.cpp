#include "stdafx.h"
#include "ImageProcesser.h"


ImageProcesser::ImageProcesser()
{

}


ImageProcesser::~ImageProcesser()
{

}


const ImageProcesser* ImageProcesser::GetInstance()
{
	static ImageProcesser imageProcesser = ImageProcesser();
	return &imageProcesser;
}


int CvDepthToBitsCount(int cvDepth)
{
	ushort bitsCount;
	switch (cvDepth)
	{
	case CV_8U:
	case CV_8S:
		bitsCount = 8;
		break;
	case CV_16U:
	case CV_16S:
		bitsCount = 16;
		break;
	case CV_32S:
	case CV_32F:
		bitsCount = 32;
		break;
	case CV_64F:
		bitsCount = 64;
		break;
	default:
		bitsCount = 0;
		break;
	}
	return bitsCount;
}


HBITMAP ImageProcesser::ConvertCVMatToHBITMAP(cv::Mat mat) const
{
	BITMAPINFOHEADER bmpInfoHeader = BITMAPINFOHEADER();
	ZeroMemory(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	auto matSize = mat.size();
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = matSize.width;
	bmpInfoHeader.biHeight = -matSize.height;
	bmpInfoHeader.biPlanes = 1;
	int bitsPerColor = CvDepthToBitsCount(mat.depth());
	bmpInfoHeader.biBitCount = mat.channels() * bitsPerColor;
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(bmpInfo));
	bmpInfo.bmiHeader = bmpInfoHeader;
	HDC hBuffDC = GetDC(NULL);
	HBITMAP hBmp = CreateDIBitmap(hBuffDC, &bmpInfoHeader, CBM_INIT, mat.datastart, &bmpInfo, DIB_RGB_COLORS);
	return hBmp;
}