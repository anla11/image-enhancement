#ifndef _LIBFFT_
#define _LIBFFT_

#include "processFrq.h"

using namespace cv;
using namespace std;

void    lib_FFT(Mat src, Mat &complexI);		//complexI is Mat of 2 channels: real (r) and imag (im)
void    lib_FFT(Mat src, Mat &r, Mat &im);

void    lib_inverseFFT(Mat complexI, Mat &invFrq);	//invFrq is Mat of 2 channels: real (invR), imag (invIm)
void    lib_inverseFFT(Mat r, Mat im, Mat &invR, Mat &invIm);
void    lib_inverseFFT(Mat complexI, Mat &invR, Mat &invIm);

void	lib_filter(const Mat &frqSrc, Mat &frqDst);	//laplace filter


#endif // _DEMO_LIBFFT_
