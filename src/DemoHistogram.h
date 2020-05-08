#ifndef _DEMO_HISTOGRAM_
#define _DEMO_HISTOGRAM_

#include "Histogram.h"
using namespace MyHis;

extern Mat img_3;
extern Mat img_2;
extern Mat img_1;
extern Mat img;

Mat     equalizeHistogram(Mat src = img_3);
Mat     adaptiveEqualization(Mat src = img);

#endif