#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
const Scalar RED = Scalar(0, 0, 255);
const Scalar PINK = Scalar(230, 130, 255);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar LIGHTBLUE = Scalar(255, 255, 160);
const Scalar GREEN = Scalar(0, 255, 0);

const int BGD_KEY = CV_EVENT_FLAG_CTRLKEY;  //Ctrl¼ü
const int FGD_KEY = CV_EVENT_FLAG_SHIFTKEY; //Shift¼ü
class CrabCut
{

	const string* winName;
	//const
	Mat mask;
	Mat bgdModel, fgdModel;

	uchar rectState, lblsState, prLblsState;
	bool isInitialized;

	Rect rect;
	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
	int iterCount;
	Mat* image;
public:
	
	CrabCut();
	~CrabCut();
	enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
	Mat cutImage(Mat _mat, Rect rect);
	static const int radius = 2;
	static const int thickness = -1;
	void setImageMat(const Mat  &_mat);
	void setFgPxls(vector<Point> * fdpxls);
	void reset();
	void setRect(int x1, int y1, int x2, int y2);
	void setImageAndWinName(Mat& _image, const string& _winName);
	Mat showImage() const;
	void mouseClick(int event, int x, int y, int flags, void* param);
	int nextIter();
	int getIterCount() const { return iterCount; }
private:
	void setRectInMask();
	void setLblsInMask(int flags, Point p, bool isPr);
};
extern CrabCut * crabcut;
