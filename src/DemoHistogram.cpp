#include "DemoHistogram.h"


char *window_Equalization    = "Equalization Histogram";
char *window_AdaptiveEqualization  = "Adaptive Equalization Histogram";

Mat img_3  = imread("Image/bay.jpg");
Mat img_2  = imread("Image/brain.jpg");
Mat img_1  = imread("Image/parrot.jpg");
Mat img = img_1;

Mat    equalizeHistogram(Mat src)
{
    drawHistogram(src, window_Equalization);
    Mat res = histogramEqualization(src);
    drawHistogram(res, window_Equalization);
    imshow("Equalization Histogram", res);
    wait();
	destroyWindow(window_Equalization);
	return res;
}

int TileSize, contrastLimit;
void    showAdaptiveEqualization(int , void *)
{
    Mat res = adaptiveHistogram(img, TileSize, contrastLimit);
    imshow(window_AdaptiveEqualization, res);
}

Mat    adaptiveEqualization(Mat src)
{
    img = src.clone();
    TileSize = 0;
    contrastLimit = 255;

    namedWindow(window_AdaptiveEqualization, CV_WINDOW_AUTOSIZE);
    imshow(window_AdaptiveEqualization, img);
    createTrackbar("Tile Size", window_AdaptiveEqualization, &TileSize, 32, showAdaptiveEqualization);
    createTrackbar("Contrast Limit", window_AdaptiveEqualization, &contrastLimit, 255, showAdaptiveEqualization);

    wait();
	destroyWindow(window_AdaptiveEqualization);
	return adaptiveHistogram(img, TileSize, contrastLimit);
}
