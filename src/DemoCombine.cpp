#include "DemoCombine.h"

char *window_AvgNoiseReducing = "Average Image for Noise Reducing";
char *window_Combine2Image = "Combine 2 images with lambda";
Mat img_avg, _img1, _img2;

void    show_AvgNoiseReducing(int nImage, void *)
{
	if (nImage == 0)
	{
		imshow(window_AvgNoiseReducing, img_avg);
		return;
	}
    Mat imgF, res;
    img_avg.convertTo(imgF, CV_32F);
    Mat avg = Mat(imgF.size(), imgF.type());

    for (int i = 0; i < nImage; ++i){
        //Generating noise
        srand(time(NULL));
        Mat noise = Mat(imgF.size(), imgF.type());
        randn(noise, 0, 50);
        avg = avg + noise;
    }
    avg = avg/nImage + imgF;

	normalize(avg, avg, 0, 255, CV_MINMAX);
    avg.convertTo(res, CV_8U);

    imshow(window_AvgNoiseReducing, res);
}

//----------------use Average Image for Noise Reducing--------------

void 	combine(const Mat &src)
{
	cout << "Demo combine image by using average image for noise reducing.\n";
	cout << "1. Choose number of image (10*n).\n";
	cout << "2. Generating noise for each image.\n";
	cout << "3. Average all images for noise reducing.\n\n";

	img_avg = src.clone();
    int nImage = 1;
	int maxNImage = 40;

    namedWindow(window_AvgNoiseReducing, CV_WINDOW_AUTOSIZE);
    imshow(window_AvgNoiseReducing, img_avg);
    createTrackbar("n", window_AvgNoiseReducing, &nImage, maxNImage, show_AvgNoiseReducing);

	wait();
	destroyWindow(window_AvgNoiseReducing);
}


void	show_CombineImage(int lambda, void*)
{
	Mat res = float(lambda) / 100 * _img1 + float(100 - lambda) / 100 * _img2;
	res.convertTo(res, CV_8U);
	imshow(window_Combine2Image, res);
}

Mat		combine(const Mat& img_a, const Mat& img_b){
	cout << "Combine 2 images: lamda% * img_a + (100 - lambda%) * img_b.\n";

	img_a.convertTo(_img1, CV_32F);
	img_b.convertTo(_img2, CV_32F);
    int lambda = 100;
	int maxLambda = 100;

    namedWindow(window_Combine2Image, CV_WINDOW_AUTOSIZE);
    imshow(window_Combine2Image, img_a);
    createTrackbar("lambda (%)", window_Combine2Image, &lambda, maxLambda, show_CombineImage);

	wait();
	destroyWindow(window_Combine2Image);

	Mat res = float(lambda) / 100 * _img1 + float(100 - lambda) / 100 * _img2;
	res.convertTo(res, CV_8U);
	return res;
}







