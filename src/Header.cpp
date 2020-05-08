#include "Header.h"

int		getSelection(char* name, int nArr, const char** selectArr)
{
	int selection = -1;
	while (selection < 0 || selection > nArr)
	{
		cout << name << ": " << endl;
		cout << "\t 0. Exit.\n";
		for (int i = 0; i < nArr; ++i)
			printf("\t%2.d. %s\n", i+1, selectArr[i]);
			//cout << "\t" << i + 1 << ". " << selectArr[i] << ".\n";
		cout << "Your selection: ";
		cin >> selection;
		wrtCrss;

		if (selection == 0)
			break;
	}
	return selection;
}

int		getSave()
{
	char save;
	do
	{
		cout << "Do you want to save your changes (y/n)? ";
		cin >> save;
	} while (save != 'y' && save != 'Y' && save != 'n' && save != 'N');
	wrtCrss;
	if (save == 'Y' || save == 'y')
		return 1;
	return 0;
}

void	viewImage(Mat src)
{
	imshow("Image", src);
	waitKey(30);
}

int		isdiffer(const Mat &a, const Mat &b){
	if (a.size() != b.size() || a.depth() != b.depth() || a.channels() != b.channels())
		return 1;
	Mat tmp = abs(a - b);
	if (sum(tmp) == Scalar(0, 0, 0, 0))
		return 0;
	return 1;
}

void	wait()
{
	cout << "Press Enter in Image to Finish.\n";
	waitKey();
}