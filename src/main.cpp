#include "Demo.h"
#include "DemoCombine.h"

int cntWrt = 0;

int		readImage(Mat &img);
void	editImage();
void	combineImage();
void	writeImage(const Mat &img);

string	defaultPath1 = "Image/lena.jpg";
string	defaultPath2 = "Image/highPassFilter.jpg";
string  _default;

int main()
{
	const char* selectArr[2] = {"Edit an image", "Combine Images"};
	while (1)
	{
		int selection = getSelection("Welcome", 2, selectArr);
		if (selection == 0)
			break;
		switch(selection)
		{
			case 1: editImage(); break;
			case 2: combineImage(); break;
		}
	}
	return 0;
}

int		readImage(Mat &img)
{
	string path;
	const char* selectArr[2] = {"Use default image", "Use your image"};
	while (1)
	{
		int selection = getSelection("Open Image", 2, selectArr);
		if (selection == 0)
			return 0;
		if (selection == 1)
			path = _default;
		else
		{
			cout << "Enter your image path: ";
			cin >> path;
		}
		cout << "Reading "<< path <<"...";
		img = imread(path.c_str());
		if (img.data == NULL)
		{
			cout << " error!\n";
			wrtCrss;
		}
		else
		{
			cout << " done!\n";
			break;
		}
	}

	if (img.channels() > 1)
	{
		cout << "Auto convert image to Gray Scale.\n";
		cvtColor(img, img, CV_BGR2GRAY);
	}
	wrtCrss;

	return 1;
}

void	writeImage(const Mat &img)
{
	string name;
	cout << "Image will be written at Output.\n";
	cout << "Enter name: ";
	cin >> name;
	char path[300];
	sprintf(path, "Output/%s.png", name.c_str());
	cout << "Write your image at ../" << path << "\n";
	imwrite(path, img);
	wrtCrss;
}

void	editImage()
{
	Mat img;
	_default = defaultPath1;
	if (readImage(img) == 0) 
		return;

	vector<Mat> saveImg;
	saveImg.push_back(img);
	int cur = 0;	

	Mat res = img.clone();
	int cntWrt = 0;

	while (1)
	{
		viewImage(res);
		const char* selectArr[5] = {"Use Tools", "Undo", "Redo", "Cancel All", "Write Image"};
		int selection = getSelection("Enhancement Image", 5, selectArr);
		if (selection == 0)
			break;
		switch (selection)
		{
			case 1:		
			{
				Mat tmp = editImage(res); 
				if (isdiffer(tmp, res))
				{
					res = tmp;
					while (cur != saveImg.size() - 1)
						saveImg.pop_back();
					saveImg.push_back(res);
					++cur;
				}
				break;
			}
			case 2:
			{
				if (saveImg.size() <= 1)
				{
					cout << "Cannot undo. It's your original image!\n";
					wrtCrss;
				}
				else
					res = saveImg[--cur];
				break;
			}
			case 3:
			{
				if (cur == saveImg.size() -1)
				{
					cout << "Cannot redo.\n";
					wrtCrss;
				}
				else
					res = saveImg[++cur];
				break;
			}
			case 4:
			{
				res = saveImg[0];
				cur = 0;
				while (saveImg.size() > 1)
					saveImg.pop_back();
				break;
			}
			case 5:
			{
				writeImage(res);
				break;
			}
		}
	}
	destroyAllWindows();
	wrtCrss;
}

void	combineImage()
{
	const char* selectArr[2] = {"Demo combine images", "Combine 2 images with lambda"};
	while (1)
	{
		int selection = getSelection("Combine Image", 2, selectArr);
		if (selection == 0)
			break;
		switch(selection)
		{
			case 1:
			{
				Mat img;
				_default = defaultPath1;
				if (readImage(img) == 0)
					break;
				wrtCrss;
				combine(img);
				break;
			}
			case 2:
			{
				Mat img1, img2;
				_default = defaultPath1;
				if (readImage(img1) == 0)
					break;
				_default = defaultPath2;
				if (readImage(img2) == 0)
					break;
				Mat res = combine(img1, img2);
				cv::imshow("Combine Images", res);
				writeImage(res);
				wait();
				cv::destroyWindow("Combine Images");
				break;
			}

		}
		wrtCrss;
	}
}
