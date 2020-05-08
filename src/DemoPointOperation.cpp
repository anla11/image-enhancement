#include "DemoPointOperation.h"

using namespace MyPO;

char *window_Quantization     = "Quantization";
char *window_BrightnessAdjust = "Linear Adjust";
char *window_ContrastAdjust   = "Power Adjust";
char *window_sigmoidAdjust	  = "Sigmoid Adjust";

char *window_ApplyThreshold     = "Apply Threshold";
char *window_AdaptiveThreshold  = "Adaptive Threshold - Otsu's Method";
char *window_DeblurAndThreshold = "Deblur And Apply Threshold";

char *window_constrastStreching = "Constrast Strectching";

Mat img_quatization;
Mat img_adjust;
Mat img_thrshd;
Mat img_ctrst;

Mat		buildView(Mat transform, Mat img)
{
	Mat graph = Mat(Size(256, 256), CV_8UC1, Scalar_<uint8_t>(255));
	
	for (int i = 0; i < 256; ++i)
	{
		int j = transform.at<uint8_t>(i, 0);
		graph.at<uint8_t>(255-j, i) = 0;
		graph.at<uint8_t>(0, i) = graph.at<uint8_t>(i, 0) = 
			graph.at<uint8_t>(255, i) = graph.at<uint8_t>(i, 255) = 0;
	}
	resize(graph, graph, Size(300, 300));
	resize(img, img, Size(img.rows * (400.0/img.cols), 400));
	Mat view = Mat(Size(800, 400), CV_8UC1, Scalar_<uint8_t>(255));

	graph.copyTo(view(Rect(50, 50, 300, 300)));
	img.copyTo(view(Rect(800 - img.cols, 0, img.cols, img.rows)));

	return view;
}


//-----------------------------------Quantization----------------------------------

void    showQuantization(int bpx, void*)
{
    Mat res = quantization(img_quatization, bpx);
    imshow(window_Quantization, res);
}

Mat     quantization(const Mat &src){
    img_quatization = src.clone();
    int bpx = 8;
    int max_bpx = 8;

    namedWindow(window_Quantization, CV_WINDOW_AUTOSIZE);
    imshow(window_Quantization, img_quatization);
    createTrackbar( "Bit(s)", window_Quantization, &bpx, max_bpx, showQuantization);
	wait();

	destroyWindow(window_Quantization);
	return quantization(img_quatization, bpx);
}

//------------use Linear Equation to change brightness and contrast of image----------
int alpha, beta;
void    showLinearAdjust(int , void *)
{
    Mat res = linearChange(img_adjust, double(alpha) / 50, beta - 127);
	Mat transform = Mat(Size(1, 256), CV_8UC1);
	for (int i = 0; i < 256; ++i)
		transform.at<uint8_t>(i, 0) = i;
	transform = linearChange(transform, double(alpha) / 50, beta - 127);

    imshow(window_BrightnessAdjust, buildView(transform, res));
}

Mat		LinearAdjust(const Mat &src)
{
	cout << "Transform function: g = f * (alpha/50) + (beta - 127).\n";
	cout << "Orignal function: g = f * alpha + beta.\n";
	cout << "Use alpha / 50 and beta - 127 because the trackbar can not take non-interger values.\n";
 
    img_adjust = src.clone();
    alpha = 50, beta = 127;

    namedWindow(window_BrightnessAdjust, CV_WINDOW_AUTOSIZE);
    imshow(window_BrightnessAdjust, img_adjust);
    createTrackbar("alpha", window_BrightnessAdjust, &alpha, 100, showLinearAdjust);
    createTrackbar("beta", window_BrightnessAdjust, &beta, 255, showLinearAdjust);

    wait();
	destroyWindow(window_BrightnessAdjust);
	return linearChange(img_adjust, double(alpha) / 50, beta - 127);
}

//------------use Linear Equation to change brightness and contrast of image----------
int gain, cutoff;
void    showSigmoid(int , void *)
{
    Mat res = sigmoid(img_adjust, gain, double(cutoff)/256);
	Mat transform = Mat(Size(1, 256), CV_8UC1);
	for (int i = 0; i < 256; ++i)
		transform.at<uint8_t>(i, 0) = i;
	transform = sigmoid(transform, gain, double(cutoff)/256);

    imshow(window_sigmoidAdjust, buildView(transform, res));
}

Mat		sigmoidAdjust(const Mat &src)
{
	cout << "Transform function: g = sigmoid( gain * (f - cutoff/255) ) .\n";
	cout << "Orignal function: g = sigmoid( gain * (f - cutoff) )\n";
	cout << "Use cutoff/255 because the trackbar can not take non-interger values.\n";
 
    img_adjust = src.clone();
    gain = 10, cutoff = 127;

    namedWindow(window_sigmoidAdjust, CV_WINDOW_AUTOSIZE);
    //imshow(window_sigmoidAdjust, img_adjust);
    createTrackbar("gain", window_sigmoidAdjust, &gain, 30, showSigmoid);
    createTrackbar("cutoff", window_sigmoidAdjust, &cutoff, 255, showSigmoid);

    wait();
	destroyWindow(window_sigmoidAdjust);
	return sigmoid(img_adjust, gain, double(cutoff)/255);
}

//---------------------use gamma to change contrast of image-----------------
void    showContrastImage(int gamma, void *)
{
    Mat res = powerChange(img_adjust, gamma);
	Mat transform = Mat(Size(1, 256), CV_8UC1);
	for (int i = 0; i < 256; ++i)
		transform.at<uint8_t>(i, 0) = i;
	transform = powerChange(transform, gamma);

    imshow(window_ContrastAdjust, buildView(transform, res));
}

Mat     powerAdjust(const Mat &src)
{
	cout << "Transfer function: g = f ^ (10/gamma)\n";
	cout << "Note: Original function is g = f ^ (1/gamma).\n";
	cout << "Use 10 * gamma because of the trackbar can not take non-integer value.\n";
    img_adjust = src.clone();
    int gamma = 0;
    namedWindow(window_ContrastAdjust, CV_WINDOW_AUTOSIZE);
    //imshow(window_ContrastAdjust, img_adjust);
    createTrackbar("gamma", window_ContrastAdjust, &gamma, 100, showContrastImage);

    wait();

	destroyWindow(window_ContrastAdjust);
	return powerChange(img_adjust, gamma);
}

//---------------------------Aplly Threshold-----------------------------
int _select;

void	onbarThrshld(int sz, void *)
{
	if (sz < 3) sz = 3;
	sz += (sz % 2 == 0);
	Mat res;
	if (_select == 2)
			adaptiveThreshold(img_thrshd, res, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, sz, 2);
		else
			adaptiveThreshold(img_thrshd, res, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, sz, 2);
	imshow(window_DeblurAndThreshold, res);
}

Mat		adaptiveThreshold(const Mat &src)
{
	Mat res = src;
	const char* selectArr[3] = {"Otu's Binarization", "Mean Deblur and Otsu", "Gauss Deblur and Otsu"};   
	int selection = getSelection("Adaptive Theshold", 3, selectArr);
	if (selection == 0)
		return src;
	if (selection == 1)
	{
		res = MyPO::adaptiveThreshold(src);
		imshow(window_AdaptiveThreshold, res);
		wait();
	}
	else
	{
		img_thrshd = src.clone();
		int sz = 3;
		_select = selection;

	    namedWindow(window_DeblurAndThreshold, CV_WINDOW_AUTOSIZE);
		createTrackbar("Kernel Size", window_DeblurAndThreshold, &sz, 35, onbarThrshld);
	
		wait();
		if (sz < 3) sz = 3;
		sz += (sz % 2 == 0);
		if (_select == 2)
			adaptiveThreshold(img_thrshd, res, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, sz, 2);
		else
			adaptiveThreshold(img_thrshd, res, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, sz, 2);
	}

	destroyWindow(window_AdaptiveThreshold);
	wrtCrss;
	return res;
}

void    showApplyThreshold(int threshold, void *)
{
    Mat res = MyPO::applyThreshold(img_thrshd, threshold);
    imshow(window_ApplyThreshold, res);
}

Mat    applyThreshold(const Mat &src)
{
	if (src.channels() > 1)
		img_thrshd = MyCvtClr::convert_RGB2GRAY(src);
	else
		img_thrshd = src.clone();
    int threshold = 255;

    namedWindow(window_ApplyThreshold, CV_WINDOW_AUTOSIZE);
    imshow(window_ApplyThreshold, img_thrshd);
    createTrackbar("Threshold", window_ApplyThreshold, &threshold, 255, showApplyThreshold);

    wait();
	destroyWindow("window_ApplyThreshold");
	return  MyPO::applyThreshold(img_thrshd, threshold);
}


//--------------------------Contrast Stretching------------------------
int in_low, in_high, out_low, out_high;

void    show_contrastStretching(int , void *)
{
    Mat res = constrastStretching(img_ctrst, in_low, in_high, out_low, out_high);
	Mat transform = Mat(Size(1, 256), CV_8UC1);
	for (int i = 0; i < 256; ++i)
		transform.at<uint8_t>(i, 0) = i;
	transform = constrastStretching(transform, in_low, in_high, out_low, out_high);
    imshow(window_constrastStreching, buildView(transform, res));
}

Mat		contrastStretching(const Mat &src)
{
    img_ctrst = src.clone();
    
	double Min, Max;
	minMaxLoc(src, &Min, &Max);
	out_low = in_low  = 0;
	out_high = in_high = 255;

    namedWindow(window_constrastStreching, CV_WINDOW_AUTOSIZE);
    //imshow(window_constrastStreching, img_ctrst);
    createTrackbar("In_Low", window_constrastStreching, &in_low, 253, show_contrastStretching);
    createTrackbar("In_High", window_constrastStreching, &in_high, 254, show_contrastStretching);
    createTrackbar("Out_Low", window_constrastStreching, &out_low, 254, show_contrastStretching);
    createTrackbar("Out_High", window_constrastStreching, &out_high, 255, show_contrastStretching);

    wait();
	destroyWindow(window_constrastStreching);
	return constrastStretching(img_ctrst, in_low, in_high, out_low, out_high);
}