#include "hciwindow.h"
#include"qmenubar.h"
#include"qstatusbar.h"
#include"qfiledialog.h"
#include"qmessagebox.h"
#include"qpainter.h"
#include"qtimer.h"
#include"qdir.h"
#include"qfileinfo.h"
#include"cv.h"
#include"qthread.h"

HCIWindow::HCIWindow()
{
	//fixpoints = new QVector<QPoint*>();
	this->resize(_WINDOW_WINDTH, _WINDOW_HEIGHT);
	
	isSavingFile = false;

	menuBar = new QMenuBar(this);
	menuBar->setObjectName(QStringLiteral("menuBar"));
	menuBar->setGeometry(QRect(0, 0, _WINDOW_WINDTH, 40));

	centralWidget = new QWidget(this);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	centralWidget->setGeometry(QRect(0, 40, _WINDOW_WINDTH, _WINDOW_HEIGHT - 80));

	statusBar = new QStatusBar(this);
	statusBar->setObjectName(QStringLiteral("statusBar"));
	statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
	this->setStatusBar(statusBar);
	statusBar->setGeometry(QRect(0, _WINDOW_HEIGHT-40, _WINDOW_WINDTH, 40));

	label = new QLabel(this);
	label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	label->setAlignment(Qt::AlignCenter);
	label->setBuddy(centralWidget);
	label->setGeometry(QRect(0, 40, _WINDOW_WINDTH, _WINDOW_HEIGHT - 80));
	//设置label透明
	label->setAttribute(Qt::WA_TranslucentBackground);
	//不行就用这个
	label->setStyleSheet("background:transparent");

	this->setMenuBar(menuBar);

	createActions();
	createMenus();

	
	visualthread->setPainterSize(_WINDOW_WINDTH, _WINDOW_HEIGHT - 80);
	//visualthread->moveToThread(&thread);
	connect(visualthread, SIGNAL(imageloadsig()), this, SLOT(imageload()));
	connect(visualthread, SIGNAL(imagecutsig()), this, SLOT(imagecut()));
	//connect(visualthread, SIGNAL(videoloadsig()), this, SLOT(videoload()));

	//thread.start();
}
void HCIWindow::vshow()
{
	//释放之前所占的内存空间
	if (visualthread->filestatue != FileStatue::VIDEO)
	{
		QMessageBox::warning(this, tr("Visual Show"), tr("No Video Loaded"));
		return;
	}
		
	cvReleaseCapture(&capture);
	capture = cvCreateFileCapture(visualthread->curFileName.toStdString().c_str());    //读取视频文件
	//等待load fixdata
	while (!visualthread->fixDataLoaded)
	{
		_sleep(10);         
	}
	displayVideo();
	visualthread->curWorkState = WorkState::VIDEOVSHOW;
}
void HCIWindow::open()
{
	QString selectfilename = QFileDialog::getOpenFileName(this, tr("Select Image or Video"), "../data/", tr("Images (*.png *.bmp *.jpg *.tif *.GIF *.avi)"));
	if (selectfilename.isEmpty())
		return;
	if (visualthread->filestatue == FileStatue::VIDEO)
	{
		cvReleaseCapture(&capture);
	}
	else if(visualthread->filestatue == FileStatue::IMAGE)
	{
		
		label->clear();
	}
	if (visualthread->setAndLoadFile(selectfilename))
	{
		// load file successfully 
		if (visualthread->filestatue == FileStatue::VIDEO)
		{
			capture = cvCreateFileCapture(visualthread->curFileName.toStdString().c_str());    //读取视频文件
			//通知hcithead to load fix data
			emit fileloadsig();
			
			displayVideo();
			//显示视频
			visualthread->curWorkState = WorkState::VIDEOPLAY;
			
		}
		else{
			visualthread->curWorkState = WorkState::VIDEOLOAD;
		}
	}
	return;
	
}
void HCIWindow::vcut()
{
	//
	if (visualthread->filestatue != FileStatue::VIDEO)
	{
		QMessageBox::warning(this, tr("Video Cut"), tr("No Video Loaded"));
	}
	visualthread->curWorkState = WorkState::VIDEOCUT;
	QString videoCutName = visualthread->curFileName.left(visualthread->curFileName.lastIndexOf(".avi")) + "_cut.avi";
	if (QFile::exists(videoCutName))
	{
		cvReleaseCapture(&capture);
		capture = cvCreateFileCapture(videoCutName.toStdString().c_str());
		displayVideo();
		return;
	}
	QMessageBox::StandardButton rb = QMessageBox::question(this, "Run Video Cut", "The Video Cut  Needs Long Time To Work,It Will Be Done In Background,Do You Want To Continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (rb == QMessageBox::Yes)
	{
		QString fileName = videoCutName;
		//QString fileName = QFileDialog::getSaveFileName(this);
		if (QFile::exists(fileName)) {
			int r = QMessageBox::warning(this, tr("Image Pro"),
				tr("File %1 already exists.\n"
				"Do you want to overwrite it?")
				.arg(QDir::toNativeSeparators(fileName)),
				QMessageBox::Yes | QMessageBox::No);
			if (r == QMessageBox::No)
				return;
		}
		//	visualthread->setAndLoadFile(curFile);
		if (!fileName.isEmpty())
		{
			//设置要保存的文件名
			visualthread->saveFileName = fileName;
			//通知hcithread线程去做
			emit savefilesig();
		}
	}
}
void HCIWindow::playvideo()
{
	//
}
void HCIWindow::transMatToQImage(Mat &mat)
{
	cv::Mat rgb;
	QImage * img;
	//画圈cvCircle(CvArr* img, CvPoint center, int radius, CvScalar color, int thickness=1, int lineType=8, int shift=0)
	//circle(mat, Point(500, 500), 30, RED,2);
	if (mat.channels() == 3)
	{
		//cvt Mat BGR 2 QImage RGB
		cvtColor(mat, rgb, CV_BGR2RGB);
		img = new QImage((const unsigned char*)(rgb.data),
			rgb.cols, rgb.rows,
			rgb.cols*rgb.channels(),
			QImage::Format_RGB888);
	}
	else
	{
		img = new  QImage((const unsigned char*)(mat.data),
			mat.cols, mat.rows,
			mat.cols*mat.channels(),
			QImage::Format_RGB888);
	}
	//QPainter painter(this);
	//painter.drawImage(QPoint(40, 40), *qImg);
	int x1 = label->width();
	int y1 = label->height();
	int xx = label->pos().x();
	int yy = label->pos().y();
	QImage* imgScaled = new QImage;
	*imgScaled = img->scaled(label->width(),label->height(),Qt::KeepAspectRatio);
	label->setPixmap(QPixmap::fromImage(*imgScaled));
	
}

void HCIWindow::save()
{
	isSavingFile = true;
	//disconnect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
	if (!(visualthread->curWorkState==WorkState::IMAGECUT||visualthread->curWorkState==WorkState::IMAGEVSHOW||visualthread->curWorkState==WorkState::VIDEOVSHOW))
	{
		QMessageBox::warning(this, tr("Save File"), tr("No File To Save Or The File  Already Exists"));
		isSavingFile = false;
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this);
	if (QFile::exists(fileName)) {
		int r = QMessageBox::warning(this, tr("Image Pro"),
			tr("File %1 already exists.\n"
			"Do you want to overwrite it?")
			.arg(QDir::toNativeSeparators(fileName)),
			QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
			return ;
	}
  //	visualthread->setAndLoadFile(curFile);
	if (!fileName.isEmpty())
	{
		if (visualthread->filestatue == FileStatue::VIDEO)
		{
			//设置要保存的文件名
			visualthread->saveFileName = fileName;
			//通知hcithread线程去做
			isSavingFile = true;
			emit savefilesig();
			//connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
		}
		else {		
			// 如果是图片的话直接处理
			isSavingFile = false;
			if (visualthread->curWorkState == WorkState::IMAGEVSHOW)
			{
				statusBar->showMessage(tr("Save Visual Image Is Running,Waiting ......"), 1000);
				imwrite(fileName.toStdString(), visualthread->getCurVisualImageMat());
				statusBar->showMessage(tr("Save Visual Image Is Done !"), 1000);
			}
			else if (visualthread->curWorkState==WorkState::IMAGECUT)
			{
				statusBar->showMessage(tr("Save Cut Image Is Running,Waiting ......"), 1000);
				imwrite(fileName.toStdString(), visualthread->cutImage);
				statusBar->showMessage(tr("Save Visual Image Is Done!"), 1000);
			}
		}
	} 
	else{
		isSavingFile = false;
	}
}
void HCIWindow::finishsavefile()
{
	isSavingFile = false;
	statusBar->showMessage(tr("Saving Work Is Done"), 100);
	//QMessageBox::warning(this, tr("Save File"), "Save File Succedd");
	//QMessageBox::show(this, "Save","Save File Succeed");
}
HCIWindow::~HCIWindow()
{

}
void HCIWindow::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));


	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	icutAct = new QAction(tr("&iCut"), this);
	//icutAct->setShortcuts(QKeySequence::Cut);
	icutAct->setStatusTip(tr("Cut The Image"));
	connect(icutAct, SIGNAL(triggered()), this, SLOT(icut()));

	vcutAct = new QAction(tr("&vCut"), this);
	//icutAct->setShortcuts(QKeySequence::Cut);
	vcutAct->setStatusTip(tr("Cut The Video"));
	connect(vcutAct, SIGNAL(triggered()), this, SLOT(vcut()));

	vshowAct = new QAction(tr("&vShow"), this);
	//icutAct->setShortcuts(QKeySequence::Cut);
	vshowAct->setStatusTip(tr("Show The video"));
	connect(vshowAct, SIGNAL(triggered()), this, SLOT(vshow()));

	ishowAct = new QAction(tr("&iShow"), this);
	//icutAct->setShortcuts(QKeySequence::Cut);
	ishowAct->setStatusTip(tr("Show The Image"));
	connect(ishowAct, SIGNAL(triggered()), this, SLOT(ishow()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("About Eye Tracker"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}
/*添加菜单按钮*/
void HCIWindow::createMenus()
{
	fileMenu = menuBar->addMenu(tr("&File"));
	//fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	//fileMenu->addAction(saveAsAct);
	fileMenu->setGeometry(QRect(0, 0, 60, 40));
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar->addMenu(tr("&Edit"));
	editMenu->addAction(ishowAct);
	editMenu->addAction(vshowAct);
	editMenu->addAction(icutAct);
	editMenu->addAction(vcutAct);

	menuBar->addSeparator();

	helpMenu = menuBar->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
}
void HCIWindow::about()
{
	QMessageBox::about(this, tr("About  Eye Tracker"),
		tr("<h2>Image Pro 1.1</h2>"
		"<p>Copyright &copy; 2008 Software Inc."
		"<p>Image Pro is a small application that demonstrates "
		"signal-slot connections across threads."));
}
void HCIWindow::showStatues()
{
	//workTime += 30;
	this->update();
}
void HCIWindow::ishow()
{
	visualthread->curWorkState = WorkState::IMAGEVSHOW;
	transMatToQImage(visualthread->getCurVisualImageMat());
}
void HCIWindow::imagecut()
{
	if (visualthread->filestatue != FileStatue::IMAGE)
	{
		QMessageBox::warning(this, tr("No Image"), tr("No Image Loaded"));
	}
	
	while(!visualthread->isImageCut)
	{
		_sleep(10);	
	}
	statusBar->showMessage("The Image Cut Is Done, Loading ....", 1000);
	//显示cut image
	transMatToQImage(visualthread->cutImage);
}
void HCIWindow::imageload()
{
	transMatToQImage(visualthread->getCurImageMat());
	return;
	//QImage  img = visualthread->getCurImage();
  // label->setPixmap(QPixmap::fromImage(visualthread->qImg));
	return;
	QPainter painter(this);
	//this->resize(qImg->size());
//	if (img)
	//{
		//int xx = (_WINDOW_WINDTH - img->width()) / 2 > 0 ? ((_WINDOW_WINDTH - img->width()) / 2) : 0;
		//int yy = (_WINDOW_HEIGHT - img->height()) / 2 > 40 ? ((_WINDOW_HEIGHT - img->height()) / 2) : 40;
		//painter.drawImage(QPoint(xx, yy), *img);
	//}
}
void HCIWindow::videoload()
{
	emit fileloadsig();
	playvideo();
}
void HCIWindow::icut()              //使用crabcut算法算出关注点
{
	if (visualthread->filestatue != FileStatue::IMAGE)
	{
		QMessageBox::warning(this, tr("No Image"), tr("No Image Loaded"));
	}
	//imshow("Before Cut", *(crabcut->image));
	visualthread->curWorkState = WorkState::IMAGECUT;
	statusBar->showMessage("The Image Cut Is Runing, Waiting ....", 100);
	visualthread->getCurCutImageMat();
	//transMatToQImage(visualthread->getCurCutImageMat());
	return;
	//crabcut->setImageMat(imat);
	//crabcut->setRect(0, 0, 0, 0);
	crabcut->nextIter();
	//crabcut->showImage();
	//imshow("After Cut", *(crabcut->image));
    transMatToQImage(crabcut->showImage());
}
void HCIWindow::disVisualFixVideo(CvCapture * cap)
{

}
void HCIWindow::displayVideo()
{
	curIntFrame = 0;
	if (capture)
	{
		frame = cvQueryFrame(capture);
		curIntFrame++;
		if (frame)
			datapro->setFrameSize(frame->width, frame->height);    //set frame_witdh and frame height
		//visualthread->addVideoFrame(frame);
		qImg = new QImage(QSize(frame->width, frame->height),
			QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(cvSize(frame->width, frame->height),
			8, 3);
		iplImg->imageData = (char*)qImg->bits();

		fixCount1 = 1;
		fixCount2 = 1;
		lastpoint1 = new Point(frame->width, frame->height);
		lastpoint2 = new Point(frame->width, frame->height);
		
		timer = new QTimer(this);
		int fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		timer->setInterval(1000/fps);
		//timer->setInterval(1000);
		connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
		timer->start();

		visualthread->curWorkState = WorkState::VIDEOPLAY;
	}
}
void HCIWindow::setfileloaded()
{
	visualthread->fixDataLoaded = true;
}
void HCIWindow::nextFrame()
{
	if (!(visualthread->curWorkState == WorkState::VIDEOPLAY || visualthread->curWorkState == WorkState::VIDEOVSHOW||visualthread->curWorkState==WorkState::VIDEOCUT))
		return;
	
	frame = cvQueryFrame(capture);
	if (frame)
	{
		curIntFrame++;
         // 如果要可视化显示
		if (visualthread->curWorkState == WorkState::VIDEOVSHOW)
		{
			QPoint * cpoint = datapro->getCenterFixPoint(curIntFrame);
	     	//	QPoint *cpoint2 = datapro->getCenterFixPoint(curIntFrame, 2);
			//QPoint *cpoint = datapro->getCenterFixPoint(curIntFrame, 2);
			Point *lastpoint = lastpoint1;
			if ((cpoint->x() - lastpoint->x)*(cpoint->x() - lastpoint->x) + ((cpoint->y() - lastpoint->y)*(cpoint->y() - lastpoint->y)) < 12 * 12)
			{
				fixCount1++;
				cvCircle(frame, Point(lastpoint->x, lastpoint->y), int(12*sqrt(fixCount1)), RED, 2);
			}
			else{
				cvLine(frame, *lastpoint, Point(cpoint->x(), cpoint->y()), RED, 2);
				cvCircle(frame, Point(cpoint->x(), cpoint->y()), 2, RED, 2);
				cvCircle(frame, Point(cpoint->x(), cpoint->y()), 12, RED, 2);
				
				lastpoint1 = new Point(cpoint->x(), cpoint->y());
				fixCount1 = 1;
			}
		   cpoint = datapro->getCenterFixPoint(curIntFrame,3);
			//	QPoint *cpoint2 = datapro->getCenterFixPoint(curIntFrame, 2);
			//QPoint *cpoint = datapro->getCenterFixPoint(curIntFrame, 2);
			lastpoint = lastpoint2;
			if ((cpoint->x() - lastpoint->x)*(cpoint->x() - lastpoint->x) + ((cpoint->y() - lastpoint->y)*(cpoint->y() - lastpoint->y)) < 12 * 12)
			{
				fixCount2++;
				cvCircle(frame, Point(lastpoint->x, lastpoint->y), int(12*sqrt( fixCount2)), BLUE, 2);
			}
			else{
				cvLine(frame, *lastpoint, Point(cpoint->x(), cpoint->y()), BLUE, 2);
				cvCircle(frame, Point(cpoint->x(), cpoint->y()), 2,BLUE , 2);
				cvCircle(frame, Point(cpoint->x(), cpoint->y()), 12, BLUE, 2);
				
				lastpoint2 = new Point(cpoint->x(), cpoint->y());
				fixCount2 = 1;
			}
			qImg = new QImage(QSize(frame->width, frame->height),
				QImage::Format_RGB888);
			iplImg = cvCreateImageHeader(cvSize(frame->width, frame->height),
				8, 3);
			iplImg->imageData = (char*)qImg->bits();
		}
		if (frame->origin == IPL_ORIGIN_TL)
		{
			cvCopy(frame, iplImg, 0);
		}
		else
		{
			cvFlip(frame, iplImg, 0);
		}
		cvCvtColor(iplImg, iplImg, CV_BGR2RGB);
		this->update();
    }
	else{
	    //	free(capture);
		qImg = new QImage(*qImg);
		cvReleaseCapture(&capture);
		//reset the work state
		//visualthread->curWorkState = WorkState::VIDEOLOAD;
		//cancel the connection to play the video
		//disconnect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
	}
	
}
void HCIWindow::paintEvent(QPaintEvent *e)
{
	

	//if play video
     if (visualthread->curWorkState==WorkState::VIDEOPLAY||visualthread->curWorkState==WorkState::VIDEOVSHOW)  
	{
		QPainter painter(this);
		if (qImg)
		{
			int xx = (_WINDOW_WINDTH - qImg->width()) / 2 > 0 ? ((_WINDOW_WINDTH - qImg->width()) / 2) : 0;
			int yy = (_WINDOW_HEIGHT - qImg->height()) / 2 > 40 ? ((_WINDOW_HEIGHT - qImg->height()) / 2) : 40;
			painter.drawImage(QPoint(xx, yy), *qImg);
		}
	}
	 if (isSavingFile)
	 {
		 statusBar->showMessage(tr("The Saving Work Is Runing, Waiting"), 10);
	 }
}