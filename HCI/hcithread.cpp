#include "hcithread.h"
#include"qdir.h"
#include"qfileinfo.h"
#include"qmessagebox.h"
#include"hciwindow.h"
#include"DataProcess.h"
HCIThread::HCIThread()
{

}

HCIThread::~HCIThread()
{

}
void HCIThread::avifileload()
{
	QDir dir(visualthread->curPathName);
	if (!dir.exists())
	{
		//QMessageBox::warning(hciwindow, tr("Load File"), tr("The Curent Path Does Not Exist Any Eye Fixation Data"));
		return;
	}
	datapro->fixpoints->clear();
	dir.setFilter(QDir::Dirs | QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo fileInfo = list.at(i);
		QString filename = fileInfo.fileName();
		if (fileInfo.fileName().endsWith(".txt"))
		{
			datapro->getPreprocessedPoints(fileInfo.absoluteFilePath());			//fixpoints->push_back(point);
		}
	}
	visualthread->fixDataLoaded = true;
}

void HCIThread::savevideofile()
{
	if (visualthread->curWorkState == WorkState::VIDEOVSHOW)
	{
		//要保存的是可视化video文件
		if (!saveVisualVideoFile())
		{
			visualthread->setErrorMessage(tr("Save File Failed"));
		}
		else{
			//向GUI 发送信号，表示save 已经完成
			emit finishsavefilesig();
			//
		}
	}
	else{
		if (saveCutVideoFile())
		{
			//要保存的是cut视频
			visualthread->setErrorMessage(tr("Save File Failed"));
		}
		else{
			//
			emit finishsavefilesig();
		}
	}
}
bool HCIThread::saveVisualVideoFile()
{

	const string source = visualthread->curFileName.toStdString();            // the source file name
	//const bool askOutputType = argv[3][0] == 'Y';  // If false it will use the inputs codec type
	const bool askOutputType = false;
	VideoCapture inputVideo(source);        // Open input
	if (!inputVideo.isOpened())
	{
		//cout << "Could not open the input video." << source << endl;
		return   false;
	}

	const string NAME = visualthread->saveFileName.toStdString();
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
		return  false;
	}
	union { int v; char c[5]; } uEx;
	uEx.v = ex;                              // From Int to char via union
	uEx.c[4] = '\0';

	//检查眼动仪数据是否加载 ，没有的话Load it  for video
	if (!visualthread->fixDataLoaded)
	{
		avifileload();
	}
	Mat src, res;
	vector<Mat> spl;
	int curIntFrame = 0;
	Point *lastpoint1 = new Point(visualthread->window_width / 2, visualthread->window_height / 2);
	Point *lastpoint2 = new Point(visualthread->window_width / 2, visualthread->window_height / 2);
	int fixCount1 = 1;
	int fixCount2 = 1;
	while (true) //Show the image captured in the window and repeat
	{
		inputVideo >> src;              // read
		if (src.empty()) break;         // check if at end

		curIntFrame++;

		QPoint * cpoint = datapro->getCenterFixPoint(curIntFrame);
		if ((cpoint->x() - lastpoint1->x)*(cpoint->x() - lastpoint1->x) + ((cpoint->y() - lastpoint1->y)*(cpoint->y() - lastpoint1->y)) < 12 * 12)
		{
			fixCount1++;
			circle(src, Point(lastpoint1->x, lastpoint1->y), int(12*sqrt(fixCount1)), RED, 2);
		}
		else{
			line(src, *lastpoint1, Point(cpoint->x(), cpoint->y()), RED, 2);
			circle(src, Point(cpoint->x(), cpoint->y()), 2, RED, 2);
			circle(src, Point(cpoint->x(), cpoint->y()), 12, RED, 2);
		
			lastpoint1 = new Point(cpoint->x(), cpoint->y());
			fixCount1 = 1;
		}
		cpoint = datapro->getCenterFixPoint(curIntFrame,3);
		if ((cpoint->x() - lastpoint2->x)*(cpoint->x() - lastpoint2->x) + ((cpoint->y() - lastpoint2->y)*(cpoint->y() - lastpoint2->y)) < 12 * 12)
		{
			fixCount2++;
			circle(src, Point(lastpoint2->x, lastpoint2->y), int(12*sqrt(fixCount2)), BLUE, 2);
		}
		else{
			line(src, *lastpoint2, Point(cpoint->x(), cpoint->y()), BLUE, 2);
			circle(src, Point(cpoint->x(), cpoint->y()), 2, BLUE, 2);
			circle(src, Point(cpoint->x(), cpoint->y()), 12, BLUE, 2);
			
			lastpoint2 = new Point(cpoint->x(), cpoint->y());
			fixCount2 = 1;
		}
       
		//outputVideo.write(res); //save or
		outputVideo << src;
	}

	//释放资源
	inputVideo.release();
	outputVideo.release();
	//cvReleaseVideoWriter(&writer);
	return  true;
}
bool HCIThread::saveCutVideoFile()
{
	const string source = visualthread->curFileName.toStdString();            // the source file name
	//const bool askOutputType = argv[3][0] == 'Y';  // If false it will use the inputs codec type
	const bool askOutputType = false;
	VideoCapture inputVideo(source);        // Open input
	if (!inputVideo.isOpened())
	{
		//cout << "Could not open the input video." << source << endl;
		return   false;
	}

	const string NAME = visualthread->saveFileName.toStdString();
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
		return  false;
	}
	union { int v; char c[5]; } uEx;
	uEx.v = ex;                              // From Int to char via union
	uEx.c[4] = '\0';

	//检查眼动仪数据是否加载 ，没有的话Load it  for video
	if (!visualthread->fixDataLoaded)
	{
		avifileload();
	}
	Mat src, res;
	vector<Mat> spl;
	int curIntFrame = 0;
	Point *lastpoint = new Point(visualthread->window_width / 2, visualthread->window_height / 2);
	int fixCount = 1;
	//int intFrameCount = 0;
	while (true) //Show the image captured in the window and repeat
	{
		inputVideo >> src;              // read
		if (src.empty()) break;         // check if at end
		
		//if (curIntFrame == 5)break;      
		 curIntFrame++;
		QVector<Point*> * durfixpoints=datapro->getDuringFixPoints(curIntFrame);
		//mshow("src", src);
		res = cutTheImage(src, durfixpoints);
		//imshow("cut", res);

		//imwrite(("cut_frame_"+QString::number(curIntFrame, 10)).toStdString(), res);
		//split(src, spl);                 // process - extract only the correct channel
		//merge(spl, res);
		//outputVideo.write(res); //save or
		outputVideo << res;
		//break;
	}

	//释放资源
	inputVideo.release();
	outputVideo.release();
	//cvReleaseVideoWriter(&writer);
	return  true;
}
Mat HCIThread::cutTheImage(Mat  &imat, QVector<Point *> *fpoints)
{
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
			//circle(imat, *point, 3, GREEN, 1);
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
		scraed_witdh = imat.cols * 1 / 2;
		scraed_height = imat.cols * 1 / 2;
	}
	else{
		scraed_witdh = sqrt(imat.cols*imat.rows) * 1 / 2;
		scraed_height = imat.rows * 1 / 2;
	}
	float t1 = sqrt((float(mx2 - m_x)) / (mx2 - mx1));
	float t2 = sqrt((float(m_x - mx1)) / (mx2 - mx1));
	float t3 = sqrt((float(my2 - m_y)) / (my2 - my1));
	float t4 = sqrt((float(m_y - my1)) / (my2 - my1));

	int SX = m_x - scraed_witdh*(t1)* 2 / 3;
	int SY = m_y - scraed_height*(t3)* 2 / 3 + 10;
	//rectangle(imat, Rect(SX, SY, scraed_witdh*(t1 + t2), scraed_height*(t3 + t4)), BLUE, 2);
	//set fixrect
	SX = SX > 0 ? (SX) : 0;
	SY = SY > 0 ? (SY) : 0;
	int SW = scraed_witdh*(t1 + t2);
	int SH = scraed_height*(t3 + t4);
	SW = (SW < imat.cols - SX) ? (SW) : (imat.cols - SX);
	SH = (SH < imat.rows - SY) ? (SH) : (imat.rows - SY);
	Rect fixrect = Rect(SX, SY, SW, SH);
	Mat s = imat.clone();
	//rectangle(s, fixrect, GREEN, 2);
	//imshow("cut", s);
	return crabcut->cutImage(imat, fixrect);
}