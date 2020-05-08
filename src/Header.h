#ifndef _HEADER_
#define _HEADER_

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const double PI = acos(-1);
const int MAX_SIZE = 300;
#define wrtCrss { cout << "----------------------------------------------------------------------------\n";}

int		getSelection(char* name, int nArr, const char** selectArr);
int		getSave();
void	viewImage(Mat src);
int		isdiffer(const Mat &a, const Mat &b);
void	wait();

#endif //_HEADER_