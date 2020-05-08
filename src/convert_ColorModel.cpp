#include "convert_ColorModel.h"

// ---------------------------------RGB to GRAY -----------------------------------
Mat     MyCvtClr::convert_RGB2GRAY(Mat src)
// intensity = 0.2989*red + 0.5870*green + 0.1140*blue
{
//    int height = src.size().height;
//    int width = src.size().width;
    Mat gray = Mat(src.size(), CV_8U);
    vector<Mat> img_rgb;
    split(src, img_rgb);

    img_rgb[0] = img_rgb[0] * 0.1140;
    img_rgb[1] = img_rgb[1] * 0.5870;
    img_rgb[2] = img_rgb[2] * 0.2989;

    gray = img_rgb[0] + img_rgb[1] + img_rgb[2];

//    Mat gray = Mat(height, width, CV_8UC1);
//    for (int i = 0; i < height; ++i)
//        for (int j = 0; j <width; ++j)
//            gray.at<uint8_t>(i, j) = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2]) / 3;
    return gray;
}


// ---------------------------------RGB and HSV -----------------------------------
Mat     MyCvtClr::convert_RGB2HSV(Mat img)
{
    int height = img.size().height;
    int width = img.size().width;
    Mat res = Mat(height, width, CV_8UC3);

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
        {
            int R = img.at<Vec3b>(i, j)[2];
            int G = img.at<Vec3b>(i, j)[1];
            int B = img.at<Vec3b>(i, j)[0];

            int H, S, V;

            int M = max(R, max(G, B));
            int m = min(R, min(G, B));

            int C = M - m;

            V = M;
            if (V == 0 || C == 0)
                S = 0;
            else
                S = float(C) / V * 255;
            if (C == 0)
                H = 0;
            else
            {
                float H_tmp;
                if (M == R)
                    H_tmp = float(G - B)/ C;
                else
                {
                    if (M == G)
                        H_tmp = float(B - R) / C + 2;
                    else
                        H_tmp = float(R - G) / C + 4;
                }
                if (H_tmp < 0) H_tmp += 6;
                H = H_tmp * 30;
            }

            res.at<Vec3b>(i, j)[2] = V;
            res.at<Vec3b>(i, j)[1] = S;
            res.at<Vec3b>(i ,j)[0] = H;
        }
    return res;
}

//Suy ra từ hàm RGB2HSV
Mat     MyCvtClr::convert_HSV2RGB(Mat img)
{
    int height = img.size().height;
    int width = img.size().width;

//    Mat img = Mat(height, width, CV_8UC3);
    Mat res = Mat(height, width, CV_8UC3);

    vector<Mat> img_rgb;
    split(img, img_rgb);

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
        {
            float H, S, V;
            if (img.type() == 16)
            {
                V = float(img.at<Vec3b>(i, j)[2]) / 255;
                S = float(img.at<Vec3b>(i, j)[1]) / 255;
                H = float(img.at<Vec3b>(i, j)[0]) * 2;
            }
            else
            {
                V = img.at<Vec3f>(i, j)[2];
                S = img.at<Vec3f>(i, j)[1];
                H = img.at<Vec3f>(i, j)[0];
            }

            float C = V * S;
            float Min = V - C;

            H /= 60;

            float R2, G2, B2;
            if (0 <= H && H < 1)
                R2 = C, G2 = H * C, B2 = 0;
            else if (1 <= H && H < 2)
                R2 = (2 - H) * C, G2 = C, B2 = 0;
            else if (2 <= H && H < 3)
                R2 = 0, G2 = C, B2 = (H - 2) * C;
            else if (3 <= H && H < 4)
                R2 = 0, G2 = (4 - H) * C , B2 = C;
            else if (4 <= H && H < 5)
                R2 = (H - 4) * C, G2 = 0, B2 = C;
            else if (5 <= H && H < 6)
                R2 = C, G2 = 0, B2 = (6 - H) * C;

            res.at<Vec3b>(i, j)[0] = uint8_t((B2 + Min) * 255);
            res.at<Vec3b>(i, j)[1] = uint8_t((G2 + Min) * 255);
            res.at<Vec3b>(i, j)[2] = uint8_t((R2 + Min ) * 255);
        }
    return res;
}

// ---------------------------------RGB and CMYK -----------------------------------

Mat   MyCvtClr::convert_RGB2CYMK(Mat img)
{
    int width = img.size().width;
    int height = img.size().height;
    Mat res = Mat(height, width, CV_8UC4, Scalar_<uint8_t>(0, 0, 0, 0));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int C = 255 - img.at<Vec3b>(i, j)[2];
            int M = 255 - img.at<Vec3b>(i, j)[1];
            int Y = 255 - img.at<Vec3b>(i, j)[0];
            int K = double(min(min(C, M), Y)) / 2.55;
            res.at<Vec4b>(i, j)[3] = uint8_t(K);
            if (K == 100)
                C = M = Y = 255;
            else
            {
                C = int((double(C)/2.55 - K) * 255 / (100 - K));
                M = int((double(M)/2.55 - K) * 255 / (100 - K));
                Y = int((double(Y)/2.55 - K) * 255 / (100 - K));
            }
            res.at<Vec4b>(i, j)[2] = uint8_t(C);
            res.at<Vec4b>(i, j)[1] = uint8_t(M);
            res.at<Vec4b>(i, j)[0] = uint8_t(Y);
        }
    }
    return res;
}

