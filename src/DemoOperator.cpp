#include "DemoOperator.h"

char *window_Gauss   = "Gauss";
char *window_Median = "Median";
char *window_Mean   = "Mean";

Mat _img;
int _slct;
char *window;

Mat		filter(const Mat &src, int type, int sz)
{
	if (sz == 0)
		return src;
	if (abs(type) != 1)
		sz += (sz % 2 == 0);
	Mat res;
	switch(abs(type))
	{
		case 1: blur(_img, res, Size(sz, sz), Point(-1, -1)); break;
		case 2: medianBlur(_img, res, sz); break;
		case 3: GaussianBlur(_img, res, Size(sz, sz), 0, 0); break;
	}
	if (type > 0)
		addWeighted(_img, 1.5, res, -0.5, 0, res);
	return res;
}

void	onBar(int sz, void *)
{
	Mat res = filter(_img, _slct, sz);
	imshow(window, res);
}

Mat		filter(const Mat &src, int type)
{
	_img = src.clone();
	_slct = type;
	switch(abs(type)){
		case 1: window = window_Mean; break;
		case 2: window = window_Median;break;
		case 3: window = window_Gauss; break;
	}

	int sz = 0;
	int maxSz = min(src.rows, src.cols) /2;
	namedWindow(window, CV_WINDOW_AUTOSIZE);
    imshow(window, src);
    createTrackbar("Kernel Size", window, &sz, maxSz, onBar);
	wait();

	destroyWindow(window);
	return filter(_img, type, sz);
}

