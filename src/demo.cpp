#include "demo.h"

Mat	 demoPO(const Mat &img);
Mat	 demoHisEqua(const Mat &img);
Mat	 demoFilter(const Mat &img);
Mat  demoFFT(const Mat &img);

Mat		editImage(const Mat &img){
	Mat res = img.clone();
	const char* selectArr[4] = {"Point Operation", "Histogram Equalization", "Operator Filter", "Frequency Filter"};	
	while (1)
	{
		viewImage(res);
		int selection = getSelection("Tools", 4, selectArr);
		if (selection == 0)
			break;
		switch (selection)
		{
			case 1:  res = demoPO(res); break;
			case 2:	 res = demoHisEqua(res); break;
			case 3:  res = demoFilter(res); break;
			case 4:  res = demoFFT(res); break;
		}
		destroyAllWindows();
	}
	return res;
}

Mat		demoPO(const Mat &img)
{
	const char* selectArr[7] = {"Quantization (New!)", "Linear Adjust", "Power Adjust", "Sigmoid Adjust (New!)", "Applying Threshold", "Constrast Stretching (New!)", "Adaptive Threshold (New!)"};				
	Mat res = img.clone();
	while (1)
	{
		viewImage(res);
		int selection = getSelection("Point Operation", 7, selectArr);
		if (selection == 0)
			break;
		Mat tmp = res.clone();
		switch (selection)
		{
			case 1:		tmp = quantization(tmp); break;
			case 2:		tmp = LinearAdjust(tmp); break;
			case 3:		tmp = powerAdjust(tmp); break;
			case 4:		tmp = sigmoidAdjust(tmp); break;
			case 5:		tmp = applyThreshold(tmp); break;
			case 6:		tmp = contrastStretching(tmp); break;
			case 7:		tmp = adaptiveThreshold(tmp); break;
		}
		viewImage(tmp);
		if	(getSave())
			res = tmp;
		destroyAllWindows();
	}
	return res;
}

Mat	 demoHisEqua(const Mat &img){
	const char* selectArr[2] = {"Global Histogram Equalization", "Contrast Limit Adaptive Histogram Equalization (New!)"};		
	Mat res = img.clone();
	while (1)
	{
		viewImage(res);
		int selection = getSelection("Histogram Equalization", 2, selectArr);

		if (selection == 0)
			break;
		Mat tmp = res.clone();
		switch (selection)
		{
			case 1:		tmp = histogramEqualization(tmp); break;
			case 2:		tmp = adaptiveEqualization(tmp); break;
		}
		viewImage(tmp);
		if	(getSave())
			res = tmp;
		destroyAllWindows();
	}
	return res;

}

Mat		demoFilter(const Mat &src){
	const char* selectArr[3] = {"Mean", "Median", "Gauss"};
	Mat res = src.clone();
	while (1)
	{
		viewImage(src);	
		int selection = getSelection("Operator Filter", 3, selectArr);
		if (selection == 0)
			break;
		const char* selectArr_2[2] = {"Smooth", "Sharpen"};
		int selection_2 = getSelection("Your task", 2, selectArr_2);
		selection_2 = (selection_2 == 2) * 2 -1;
		Mat tmp = filter(src, selection * selection_2);
		if (getSave())
			res = tmp;
	}
	return res;
}

Mat  demoFFT(const Mat &img)
{
	Mat tmp;
	if (img.channels() > 1)
		tmp = MyCvtClr::convert_RGB2GRAY(img);
	else
		tmp = img.clone();
	return demoFFTFilter(img);
}