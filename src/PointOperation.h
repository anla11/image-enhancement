#ifndef _POINT_OPERATIONS_
#define _POINT_OPERATIONS_

#include "Header.h"
#include "convert_ColorModel.h"

namespace	MyPO
{
	Mat    quantization(const Mat &img, int bpx);
	Mat    linearChange(const Mat &img, double alpha, double beta);
	Mat    powerChange(const Mat &img, double gamma);
	Mat	   sigmoid(const Mat &img, double gain, double cutoff);
	Mat    applyThreshold(const Mat &img, int threshold);
	int	   OtsuThreshold(const Mat &img);
	Mat	   adaptiveThreshold(const Mat &img);
	Mat	   constrastStretching(const Mat &img, int in_low, int in_high, int out_low, int out_high);
}


#endif // _POINT_OPERATIONS_
