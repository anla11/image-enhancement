#ifndef _HISTOGRAM_
#define _HISTOGRAM

#include "Header.h"
#include "convert_ColorModel.h"

namespace  MyHis
{
	void    drawHistogram(Mat img, char *name);
	Mat    equalizeHis(Mat src);
	Mat    histogramEqualization(Mat img);
	Mat    adaptiveHistogram(Mat img, int TileSz, int contrastLimit);
}
#endif // _HISTOGRAM_

