#include "CrabCut.h"
CrabCut::CrabCut()
{
}

CrabCut::~CrabCut()
{

}
static void getBinMask(const Mat& comMask, Mat& binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		CV_Error(CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;  //得到mask的最低位,实际上是只保留确定的或者有可能的前景点当做mask
}
void CrabCut::reset()
{
	if (!mask.empty())
		mask.setTo(Scalar::all(GC_BGD));
	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear();  prFgdPxls.clear();

	isInitialized = false;
	rectState = NOT_SET;    //NOT_SET == 0
	lblsState = NOT_SET;
	prLblsState = NOT_SET;
	iterCount = 0;


}
/*给类的成员变量赋值而已*/
void CrabCut::setImageAndWinName(Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	mask.create(image->size(), CV_8UC1);
	reset();
}

/*显示4个点，一个矩形和图像内容，因为后面的步骤很多地方都要用到这个函数，所以单独拿出来*/
Mat CrabCut::showImage() const
{
	Mat res;
	if (image->empty())
		return res;


	Mat binMask;
	if (!isInitialized)
		image->copyTo(res);
	else
	{
		getBinMask(mask, binMask);
		//return binMask;
	  /*uchar *  p = mask.data;
		
		int a[1000][1000];
		for (int i = 0; i < mask.rows; i++)
		{
			for (int j = 0; j < mask.cols; j++)
			{
				a[i][j] = *(p+i*mask.rows + j);
			}
		}
		*/
		image->copyTo(res, binMask);  //按照最低位是0还是1来复制，只保留跟前景有关的图像，比如说可能的前景，可能的背景
	}

	vector<Point>::const_iterator it;
	/*下面4句代码是将选中的4个点用不同的颜色显示出来*/
	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)  //迭代器可以看成是一个指针
		circle(res, *it, radius, BLUE, thickness);
	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)  //确定的前景用红色表示
		circle(res, *it, radius, RED, thickness);
	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
		circle(res, *it, radius, LIGHTBLUE, thickness);
	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
		circle(res, *it, radius, PINK, thickness);

	/*画矩形*/
	if (rectState == IN_PROCESS || rectState == SET)
		rectangle(res, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);
	return res;
	//imshow("Cut", res);
}

/*该步骤完成后，mask图像中rect内部是3，外面全是0*/
void CrabCut::setRectInMask()
{
	assert(!mask.empty());
	mask.setTo(GC_BGD);   //GC_BGD == 0
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image->cols - rect.x);
	rect.height = min(rect.height, image->rows - rect.y);
	(mask(rect)).setTo(Scalar(GC_PR_FGD));    //GC_PR_FGD == 3，矩形内部,为可能的前景点
}
void CrabCut::setFgPxls(vector<Point> * fdpxls)
{
	vector<Point>  *fpxls;
	fpxls = &fgdPxls;
	for (int i = 0; i < fdpxls->size(); i++)
	{
		Point p = fdpxls->at(i);
		fpxls->push_back(p);
		circle(mask, p, radius, 1, thickness);   //该点处为1
	}
	lblsState == SET;
	
}
void CrabCut::setLblsInMask(int flags, Point p, bool isPr)
{
	vector<Point> *bpxls, *fpxls;
	uchar bvalue, fvalue;
	if (!isPr) //确定的点
	{
		bpxls = &bgdPxls;
		fpxls = &fgdPxls;
		bvalue = GC_BGD;    //0
		fvalue = GC_FGD;    //1
	}
	else    //概率点
	{
		bpxls = &prBgdPxls;
		fpxls = &prFgdPxls;
		bvalue = GC_PR_BGD; //2
		fvalue = GC_PR_FGD; //3
	}
	if (flags & BGD_KEY)
	{
		bpxls->push_back(p);
		circle(mask, p, radius, bvalue, thickness);   //该点处为2
	}
	if (flags & FGD_KEY)
	{
		fpxls->push_back(p);
		circle(mask, p, radius, fvalue, thickness);   //该点处为3
	}
}

/*鼠标响应函数，参数flags为CV_EVENT_FLAG的组合*/
void CrabCut::mouseClick(int event, int x, int y, int flags, void*)
{
	// TODO add bad args check
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
	{
								   bool isb = (flags & BGD_KEY) != 0,
									   isf = (flags & FGD_KEY) != 0;
								   if (rectState == NOT_SET && !isb && !isf)//只有左键按下时
								   {
									   rectState = IN_PROCESS; //表示正在画矩形
									   rect = Rect(x, y, 1, 1);
								   }
								   if ((isb || isf) && rectState == SET) //按下了alt键或者shift键，且画好了矩形，表示正在画前景背景点
									   lblsState = IN_PROCESS;
	}
		break;
	case CV_EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
	{
								   bool isb = (flags & BGD_KEY) != 0,
									   isf = (flags & FGD_KEY) != 0;
								   if ((isb || isf) && rectState == SET) //正在画可能的前景背景点
									   prLblsState = IN_PROCESS;
	}
		break;
	case CV_EVENT_LBUTTONUP:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));   //矩形结束
			rectState = SET;
			setRectInMask();
			assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();
		}
		if (lblsState == IN_PROCESS)   //已画了前后景点
		{
			setLblsInMask(flags, Point(x, y), false);    //画出前景点
			lblsState = SET;
			showImage();
		}
		break;
	case CV_EVENT_RBUTTONUP:
		if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true); //画出背景点
			prLblsState = SET;
			showImage();
		}
		break;
	case CV_EVENT_MOUSEMOVE:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();    //不断的显示图片
		}
		else if (lblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), false);
			showImage();
		}
		else if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			showImage();
		}
		break;
	}
}

void CrabCut::setRect(int x1, int y1, int x2, int y2)
{

	mask.create(image->size(), CV_8UC1);
	reset();
	/**
	rect = Rect(Point(100, 120), Point(399, 497));
	rectState = SET;
	setRectInMask();
	setLblsInMask(0, Point(399, 497), false);    //画出前景点
	lblsState = SET;
	*/
	
	rect = Rect(Point(x1, y1), Point(x2, y2));
	rectState = SET;
	setRectInMask();
	setLblsInMask(0, Point(x2, y2), false);    //画出前景点
	lblsState = SET;
	/*
	mask.create(image->size(), CV_8UC1);
	reset();
	rect = Rect(Point(x1, y1), Point(x2, y2));   //矩形结束
	rectState = SET;
	setRectInMask();
	assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
	*/
}
/*该函数进行grabcut算法，并且返回算法运行迭代的次数*/
int CrabCut::nextIter()
{
	if (isInitialized)
		//使用grab算法进行一次迭代，参数2为mask，里面存的mask位是：矩形内部除掉那些可能是背景或者已经确定是背景后的所有的点，且mask同时也为输出
		//保存的是分割后的前景图像
		grabCut(*image, mask, rect, bgdModel, fgdModel, 1);
	else
	{
		if (rectState != SET)
			return iterCount;

		if (lblsState == SET || prLblsState == SET)
			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);
		else
			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

		isInitialized = true;
	}
	iterCount++;

	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear(); prFgdPxls.clear();

	return iterCount;
}

void CrabCut::setImageMat(const Mat & _mat)
{
	image = new Mat(_mat);
}
Mat CrabCut::cutImage(Mat _mat, Rect _rect)
{
	image = new Mat(_mat);
	mask.create(image->size(), CV_8UC1);
	reset();
	rect = _rect;
	rectState = SET;
	setRectInMask();
	setLblsInMask(0, Point(_rect.x+_rect.width,_rect.x+_rect.height ), false);    //画出前景点
	lblsState = SET;
	nextIter();
	return showImage();
}
