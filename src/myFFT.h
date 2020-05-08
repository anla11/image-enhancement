#ifndef _myFFT_
#define _myFFT_

#include "processFrq.h"
#include <complex>

typedef complex<float> cplx;

namespace	myFFT
{
	void    filter(Mat frqSrc, Mat &frqDst, float thrhd, int type);
	// frqSrc is FFT value of Image, 2 channels: real (fr), imag (fim)
	// frqDst is FFT filterd value of Image, 2 channels: real (r), imag (im)

	void	fft(const Mat &src, Mat &r, Mat &im, int inverse = -1);
	void    fft(const Mat &fr, const Mat &fim, Mat &r, Mat &im, int inverse = -1);
	//input of FFT (or inverse FFT) can be Mat_<float> or Mat_<float, float>

}
#endif // _myFFT_
