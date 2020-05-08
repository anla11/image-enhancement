#ifndef _CONVERT_COLORMODEL_
#define _CONVERT_COLORMODEL_

#include "Header.h"

namespace	MyCvtClr{
	// ---------------------------------RGB to GRAY -----------------------------------
	Mat     convert_RGB2GRAY(Mat src);
	// ---------------------------------RGB and HSV -----------------------------------
	Mat     convert_RGB2HSV(Mat img);
	Mat     convert_HSV2RGB(Mat img);
	// ---------------------------------RGB and CMYK -----------------------------------
	Mat   convert_RGB2CYMK(Mat img);
}

#endif // _CONVERT_COLORMODEL_



