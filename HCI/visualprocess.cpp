#include "visualprocess.h"
#include"qimage.h"
#include"DataProcess.h"
VisualProcess::VisualProcess()
{
	filestatue = FileStatue::EMPTY;    //set 
	fpoints = new QVector<Point *>();
	videoframes = new QVector< IplImage *>();

}

VisualProcess::~VisualProcess()
{

}
void VisualProcess::setPainterSize(int w, int h)
{

	window_width = w;
	window_height = h;
}
//读入文件并设置当前文件的工作状态
bool VisualProcess::setAndLoadFile(QString filename)
{
	//set curfilename and curpath
	curFileName = filename;
	curPathName = curFileName.left(curFileName.lastIndexOf("/") + 1);
	
	//fix data is not loaded
	fixDataLoaded = false;
	//image fix rect is not got
	isFixRected = false;
	// image cut has not  done
	isImageCut = false;
	//Video file
	if (curFileName.endsWith("avi"))
	{
		//初始化一个视频文件捕捉器
		CvCapture* capture = cvCreateFileCapture(curFileName.toStdString().c_str());
		//获取视频信息
		if (capture)
		{
			//获取视频信息
			frameH = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
			frameW = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
			fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
			numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		}
		//
		cvReleaseCapture(&capture);
		filestatue = FileStatue::VIDEO;
		curWorkState = WorkState::VIDEOLOAD;
		return true;
	}
	//Image
	else{		
	    curImage = imread(curFileName.toStdString(), 1);	
		if (!curImage.empty())
		{
			emit imageloadsig();
			filestatue = FileStatue::IMAGE;
			curWorkState = WorkState::IMAGELOAD;
			QString fixfile = curFileName.left(curFileName.lastIndexOf(".jpg")) + ".txt";
			//fixdata is very small we can do it directly
			fpoints = datapro->getFixPointsFromFile(fixfile);
			return true;	
		}
	}
	return false;
}

Mat  VisualProcess::getCurImageMat()
{
	return curImage;
}
Mat VisualProcess::getCurVisualImageMat()
{
	Mat imat = curImage.clone();
	int * arrayX = new int[fpoints->size()];
	int * arrayY = new int[fpoints->size()];
	int sx = 0;
	int sy = 0;
	int count = 0;
	for (int i = 0; i < fpoints->size(); i++)
	{
		Point * point = fpoints->at(i);
		if (point->x >= 5 && point->y >= 5 && point->x <= imat.cols - 5 && point->y <= imat.rows - 5)
		{
			arrayX[count] = point->x;
			arrayY[count] = point->y;
			sx += point->x;
			sy += point->y;
			circle(imat, *point, 3, GREEN, 1);
			count++;
		}

	}
	qSort(arrayX, arrayX + count);
	qSort(arrayY, arrayY + count);

	int num = count * 4 / 10;
	int mx1 = arrayX[count / 2 - num];
	int mx2 = arrayX[count / 2 + num];
	int my1 = arrayY[count / 2 - num];
	int my2 = arrayY[count / 2 + num];
	int m_x = arrayX[count / 2];
	int m_y = arrayY[count / 2];

	int scraed_witdh;
	int scraed_height;
	if (imat.cols < imat.rows)
	{
		scraed_witdh = imat.cols * 1/2;
		scraed_height = imat.cols *1/2 ;
	}
	else{
		scraed_witdh = sqrt(imat.cols*imat.rows) * 1 / 2;
		scraed_height = imat.rows * 1 / 2;
	}
	float t1 = sqrt((float(mx2 - m_x)) / (mx2 - mx1));
	float t2 = sqrt((float(m_x - mx1)) / (mx2 - mx1));
	float t3 = sqrt((float(my2 - m_y)) / (my2 - my1));
	float t4 = sqrt((float(m_y - my1)) / (my2 - my1));

	int SX = m_x - scraed_witdh*(t1)*2/3;
	int SY = m_y - scraed_height*(t3)*2/3+10;
	if (SX < 0)
	{
		SX = 0;
	}
	if (SY < 0)
	{
		SY = 0;
	}
	rectangle(imat, Rect(SX, SY, scraed_witdh*(t1 + t2), scraed_height*(t3 + t4)), BLUE, 2);
	//set fixrect
	fixrect=Rect(SX, SY, scraed_witdh*(t1 + t2),  scraed_height*(t3 + t4));

	isFixRected = true;

	return imat;
}
void VisualProcess::getCurCutImageMat()
{
	if (isImageCut)
	{
		emit imagecutsig();
		return;
		//return cutImage;
	}
	cutImage = crabcut->cutImage(curImage.clone(), fixrect);
	isImageCut = true;
	emit imagecutsig();
	return ;
}
IplImage * VisualProcess::getKFrame(int k)
{
	if (k<videoframes->size())
	return videoframes->at(k);
	return NULL;
}
void VisualProcess::addVideoFrame(IplImage * frame)
{
	videoframes->push_back(frame);
	if (videoframes->size() == 10)
	{
		imshow("yframe180", Mat(frame));
	}
}
bool VisualProcess::saveFile(QString filename)
{

	const string source =curFileName.toStdString();            // the source file name
	//const bool askOutputType = argv[3][0] == 'Y';  // If false it will use the inputs codec type
	const bool askOutputType = false;
	VideoCapture inputVideo(source);        // Open input
	if (!inputVideo.isOpened())
	{
		//cout << "Could not open the input video." << source << endl;
		return false;
	}

	const string NAME = filename.toStdString();
	//string::size_type pAt = source.find_last_of('.');   // Find extension point

	//const string NAME = source.substr(0, pAt) +"_visual"+ ".avi";   // Form the new name with container
	int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

	// Transform from int to char via Bitwise operators
	char EXT[] = { ex & 0XFF, (ex & 0XFF00) >> 8, (ex & 0XFF0000) >> 16, (ex & 0XFF000000) >> 24, 0 };

	Size S = Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    //Acquire input size
		(int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

	VideoWriter outputVideo;                                        // Open the output
	if (askOutputType)
		outputVideo.open(NAME, ex = -1, inputVideo.get(CV_CAP_PROP_FPS), S, true);
	else
		outputVideo.open(NAME, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		//cout << "Could not open the output video for write: " << source << endl;
		return false;
	}

	union { int v; char c[5]; } uEx;
	uEx.v = ex;                              // From Int to char via union
	uEx.c[4] = '\0';

	////cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
	//	<< " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
      //	cout << "Input codec type: " << EXT << endl;
	/*
	int channel = 2;    // Select the channel to save
	switch (argv[2][0])
	{
	case 'R': {channel = 2; break; }
	case 'G': {channel = 1; break; }
	case 'B': {channel = 0; break; }
	}
	*/
	Mat src, res;
	vector<Mat> spl;
	int curIntFrame = 0;
	Point *lastpoint = new Point(window_width/2, window_height/2);
	int fixCount = 1;
	while (true) //Show the image captured in the window and repeat
	{
			inputVideo >> src;              // read
		if (src.empty()) break;         // check if at end
	
		curIntFrame++;

		QPoint * cpoint = datapro->getCenterFixPoint(curIntFrame);

		if ((cpoint->x() - lastpoint->x)*(cpoint->x() - lastpoint->x) + ((cpoint->y() - lastpoint->y)*(cpoint->y() - lastpoint->y)) < 20 * 20)
		{
			fixCount++;
			circle(src, Point(lastpoint->x, lastpoint->y), int(sqrt(20 * fixCount)), RED, 1);
		}
		else{
			circle(src, Point(cpoint->x(), cpoint->y()), 2, RED, 1);
			circle(src, Point(cpoint->x(), cpoint->y()), 20, RED, 1);
			line(src, *lastpoint, Point(cpoint->x(), cpoint->y()), RED, 1);
			lastpoint = new Point(cpoint->x(), cpoint->y());
			fixCount = 1;
		}

		split(src, spl);                 // process - extract only the correct channel
		merge(spl, res);
		//outputVideo.write(res); //save or
		outputVideo << res;
	}
	outputVideo.release();
	//cvReleaseVideoWriter(&writer);
	return true;
//	cout << "Finished writing" << endl;
	CvSize size = cvSize(window_width, window_height);
	//	(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter *writer = cvCreateVideoWriter(filename.toStdString().c_str(), CV_FOURCC('M', 'J', 'P', 'G'),
		fps, size);
	//转化完毕，用cvReleaseVideoWriter函数释放CvVideoWriter*类型变量所占内存空间  
	for (int i = 10; i < videoframes->size(); i++)
	{
		IplImage* f = videoframes->at(i);
		if (i == 10)
		{
			imshow("10", Mat(f));
		}
		cvWriteFrame(writer, f);
	}
	cvReleaseVideoWriter(&writer);
	return true;
	//用cvReleaseImage函数释放IplImage*类型变量所占内存空间  
	//cvReleaseImage(&logpolar_frame);
	//用cvReleaseCapture函数释放CvCapture*类型变量所占内存空间  
	//cvReleaseCapture(&capture)
}

void VisualProcess::setErrorMessage(QString msg)
{
	errmsg = msg;
	//向GUI 发出出现错误信号
	emit errorfindsig();
}