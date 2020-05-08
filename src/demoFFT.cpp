#include "demoFFT.h"

using namespace myFFT;

string LowFilterName[3] = {"Ideal Low Pass Filter", "Butter Worth Low Pass Filter", "Gaussian Low Pass Filter"};
string HighFilterName[4] = {"Ideal High Pass Filter", "Butter Worth High Pass Filter", "Gaussian High Pass Filter", "Laplace High Pass Filter"};

Mat _frq;
int _type;
int _isLib;
string _passFilterStr;

void    onBar_FilterFrqFFT(int thrhd, void*){
	Mat sptr, img_out, filtered, invFrq;

	filter(_frq, filtered, thrhd, _type);

    findSpecTrum(filtered, sptr);
    lib_inverseFFT(filtered, invFrq);
    recoverImage(invFrq, img_out);

    Mat viewSptr, viewOut;
    int space = 30;
    int viewRow = min(sptr.rows, 500);
    int viewCol = float(viewRow) / sptr.rows * sptr.cols;
    resize(sptr, viewSptr, Size(viewCol, viewRow));
    resize(img_out, viewOut, Size(viewCol, viewRow));
    Mat view = Mat(Size(viewCol* 2 + space, viewRow), CV_32FC1, Scalar_<float>(1));

    for (int i = 0; i < view.rows; ++i)
        for (int j = 0; j < view.cols; ++j)
        {
            if (j >= viewOut.cols + space)
                view.at<float>(i, j) = viewSptr.at<float>(i, j - viewOut.cols - space);
            else if (j < viewOut.cols)
                view.at<float>(i, j) = viewOut.at<float>(i, j);
        }
    imshow(_passFilterStr.c_str(), view);
}

Mat     FilterFrq(Mat srcfrq, int typePass, string nameType)
{
    //set global variables
    _frq = srcfrq;
    _type = typePass;
    _passFilterStr = nameType;

    //create demo
    namedWindow(_passFilterStr.c_str(), 1);

    int thrhd = 0, maxThrhd;
	maxThrhd = sqrt(pow(_frq.rows/2, 2) + pow(_frq.cols/2, 2));
	if (typePass == 4)
		maxThrhd = 10;
    char TrackbarName[100];
    sprintf(TrackbarName, "Radius", maxThrhd);

    createTrackbar( TrackbarName, _passFilterStr.c_str(), &thrhd, maxThrhd, onBar_FilterFrqFFT);

    /// Show some stuff
    onBar_FilterFrqFFT(thrhd, 0);
    wait();
    destroyWindow(_passFilterStr.c_str());
	
	Mat filtered, sptr, invFrq, img_out;
	filter(_frq, filtered, thrhd, _type);
	findSpecTrum(filtered, sptr);
    lib_inverseFFT(filtered, invFrq);
    recoverImage(invFrq, img_out);
	return img_out;
}

Mat 	demoFFTFilter(const Mat &src){
    Mat frq;
    if (src.channels() > 1)
        cvtColor(src, src, CV_BGR2GRAY);

	Mat res = src.clone();
	lib_FFT(res, frq);

	int selection = 0; 
	while (1)
	{
		viewImage(res);
		cout << " Select fillter function: " << endl;
		cout << "\t0    : Exit. " << endl;
		cout << "\t1/-1 : Ideal High/Low Pass Filter. " << endl;
		cout << "\t2/-2 : Butter Worth High/Low Pass Filter." << endl;
		cout << "\t3/-3 : Gaussian High/Low Pass Filter." << endl;
		cout << " Your selection: ";
		cin >> selection;
		wrtCrss;

		Mat tmp;
		_isLib = 0;

		if (selection < -3 || selection > 3 || selection == 0) 
			break;
		if (selection < 0)
			tmp = FilterFrq(frq, selection, LowFilterName[abs(selection) - 1]);
		else
			tmp = FilterFrq(frq, selection, HighFilterName[selection - 1]);
		viewImage(tmp);
		if	(getSave())
			res = tmp;
		destroyAllWindows();
	} 
	//normalize(res, res, 255, CV_MINMAX);
	normalize(res, res, 0, 255, CV_MINMAX);
	res.convertTo(res, CV_8U);
	return res;
}