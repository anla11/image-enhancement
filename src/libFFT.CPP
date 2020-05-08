#include "opencv2/imgproc/imgproc_c.h"
#include "libFFT.h"

void    lib_FFT(Mat src, Mat &dst){
    if (src.channels() > 1)
        cvtColor(src, src, CV_BGR2GRAY);
    Mat fImg;
    src.convertTo(fImg, CV_32F);
    cv::dft(fImg, dst, cv::DFT_COMPLEX_OUTPUT);
    rotate_FromCenter(dst);
}

void    lib_FFT(Mat src, Mat &r, Mat &im)
{
    if (src.channels() > 1)
        cvtColor(src, src, CV_BGR2GRAY);
    Mat magI, complexI;
    vector<Mat> planes;

    lib_FFT(src, complexI);
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    r = planes[0].clone();
    im = planes[1].clone();
}

void    lib_inverseFFT(Mat complexI, Mat &invFrq){
//    Mat imgF;
//    complexI.convertTo(imgF, CV_32F);
    dft(complexI, invFrq, cv::DFT_INVERSE);
}

void    lib_inverseFFT(Mat complexI, Mat &invR, Mat &invIm){
    Mat invFrq;
    lib_inverseFFT(complexI, invFrq);
    vector<Mat> planes;
    planes.clear();
    split(invFrq, planes);
    invR = planes[0].clone();
    invIm = planes[1].clone();
}


void    lib_inverseFFT(Mat r, Mat im, Mat &invR, Mat &invIm){
    Mat complexI;
    vector<Mat> planes;
    planes.push_back(r);
    planes.push_back(im);
    merge(planes, complexI);
    Mat invFrq;
    lib_inverseFFT(complexI, invFrq);
    planes.clear();
    split(invFrq, planes);
    invR = planes[0].clone();
    invIm = planes[1].clone();
}

void	lib_filter(const Mat &frqSrc, Mat &frqDst){	//laplace filter
	Point centroid = Point(frqSrc.cols/2, frqSrc.rows/2);
	Mat H = (Mat_<float>(3,3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
	filter2D(frqSrc, frqDst, frqSrc.depth(), H);
}

