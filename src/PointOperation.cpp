#include "PointOperation.h"

Mat     MyPO::quantization(const Mat &img, int bpx)    //bit(s) per pixel
{
    int range = pow(2, 8 - bpx);

    vector<Mat> channels;
    channels.clear();
    split(img, channels);

    Mat res = Mat(img.size(), CV_8U);

    for (int c = 0, num_channel = channels.size(); c < num_channel; ++c)
    {
        channels[c] = (channels[c] - range/2) / range;  //opencv always round to nearest number
        channels[c] = range * channels[c] + range - 1;
    }

    merge(channels, res);
    return res;
}

Mat     MyPO::linearChange(const Mat &img, double alpha, double beta)
{
    Mat imgF, res;
    img.convertTo(imgF, CV_32F);
    imgF = imgF * alpha + beta;
    imgF.convertTo(res, CV_8U);
    return res;
}

Mat     MyPO::powerChange(const Mat &img, double gamma)
{
	gamma = (gamma + 1) / 10; //advoid division by zero
    Mat imgF, res;
    img.convertTo(imgF, CV_32F);
    pow(imgF, 1/gamma, imgF);
	normalize(imgF, imgF, 0, 255, CV_MINMAX);
    imgF.convertTo(res, CV_8U);
    return res;
}

Mat		MyPO::sigmoid(const Mat& img, double gain, double cutoff)
{
	Mat res, imgF;
	img.convertTo(imgF, CV_32F);
	normalize(imgF, imgF, 0, 1, CV_MINMAX);
	exp(gain * (cutoff - imgF), imgF);
	imgF += 1;
	imgF = 1 / imgF;
	normalize(imgF, imgF, 0, 255, CV_MINMAX);
    imgF.convertTo(res, CV_8U);
    return res;
}

Mat     MyPO::applyThreshold(const Mat &img, int threshold){
    Mat gray;
    if (img.channels() == 1)
        gray = img.clone();
    else
        gray = MyCvtClr::convert_RGB2GRAY(img);
	for (int i = 0, row = gray.rows; i < row; ++i)
		for (int j = 0, col = gray.cols; j < col; ++j)
			if (gray.at<uint8_t>(i, j) > threshold)
				gray.at<uint8_t>(i, j) = 255;
			else
				gray.at<uint8_t>(i, j) = 0;
    return gray;
}

int     MyPO::OtsuThreshold(const Mat &img)
{
    if (img.channels() != 1)
        return -1;

    double His[MAX_SIZE];
    double mean_level[MAX_SIZE];
    double P[MAX_SIZE];
    double sum[MAX_SIZE];

    int row = img.rows;
    int col = img.cols;

    int deltaHis = 1;
    int L = MAX_SIZE;

    for (int i = 0; i < L; ++i)
        His[i] = 0;

    for (int i = 0; i < row; ++i)
        for (int j = 0; j< col; ++j)
            ++His[img.at<uint8_t>(i, j)];

    sum[0] = His[0];

    mean_level[0] = 0;

    His[0] /= (row * col * deltaHis);
    for (int i = 1; i < L; ++i)
    {
        His[i] /= (row * col * deltaHis);
        sum[i] = sum[i - 1] + His[i];
        mean_level[i] = mean_level[i - 1] + i * His[i];
    }

    double variance = -1;
    int res = 0;
    for (int i = 1; i < L - 1; ++i)
    {
        P[i] = sum[i] / sum[L - 1];
        double tu = mean_level[L - 1] * P[i] - mean_level[i];
        double mau = P[i] * (1 - P[i]);
        if (variance < tu / mau * tu)
        {
            variance = tu / mau * tu;
            res = i;
        }
    }
    return res;
}

Mat		MyPO::adaptiveThreshold(const Mat &img)
{
    Mat gray, res;
    if (img.channels() == 1)
        gray = img.clone();
    else
        gray = MyCvtClr::convert_RGB2GRAY(img);
    int threshold = OtsuThreshold(gray);
	return applyThreshold(gray, threshold);
}

Mat		MyPO::constrastStretching(const Mat &img, int in_low, int in_high, int out_low, int out_high)
{
	in_low += (in_low == 0);
	if (in_high - in_low == 0)
	{
		if (in_low < 254)
			++in_high;
		else
			--in_low;
	}
	Mat res = Mat(img.size(), CV_8UC1);
	for (int i = 0, row = img.rows; i < row; ++i)
		for (int j = 0, col = img.cols; j < col; ++j)
		{
			int t = img.at<uint8_t>(i, j);
			if (t <= in_low)
				t =	float(out_low)/in_low * t;
			else
			{
				if (t > in_high)
					t = (float(255 - out_high)/(255 - in_high)) * (t - in_high) + out_high;
				else
					t = (float(out_high - out_low)/(in_high - in_low))*(t - in_low) + out_low;
			}
			res.at<uint8_t>(i, j)= t;
		}
	return res;
}