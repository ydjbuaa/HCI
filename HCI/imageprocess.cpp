#include "imageprocess.h"
#include"qdir.h"
#include"qfileinfo.h"
#include"qtextstream.h"
#include"CrabCut.h"
#include"qvector.h"
ImageProcess::ImageProcess()
{

}

ImageProcess::~ImageProcess()
{

}
void ImageProcess::preProcessVideo(QString path)
{
	QDir dir(path);
	CvCapture *cap;
	if (!dir.exists())
	{
		//QMessageBox::warning(hciwindow, tr("Load File"), tr("The Curent Path Does Not Exist Any Eye Fixation Data"));
		return;
	}
	dir.setFilter(QDir::Dirs | QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	QString genFileName;
	for (int i = 0; i < list.size(); i++)
	{
		QFileInfo fileInfo = list.at(i);
		QString filename = fileInfo.fileName();
		if (fileInfo.fileName().endsWith(".txt"))
		{
			datapro->getPreprocessedPoints(fileInfo.absoluteFilePath());
		}
		else if (fileInfo.fileName().endsWith(".avi"))
		{
			cap = cvCreateFileCapture(fileInfo.absoluteFilePath().toStdString().c_str());
			genFileName = fileInfo.absolutePath();
		}
	}
	if (!cap)
	{
		return;
	}
	QDir *imgdir = new QDir;
	genFileName += "/images";
	if (!imgdir->exists(genFileName))
	{
		if (!imgdir->mkdir(genFileName))
		{
			return;
		}
	}
	IplImage *frame = cvQueryFrame(cap);
	
	//设置当前视频帧的宽和高
	frame_width = frame->width;
	frame_height = frame->height;
	
	datapro->setFrameSize(frame_width, frame_height);
	int curFrameCount = 1;
	while (frame)
	{
		if (curFrameCount % 30 == 0)
		{
			genImageFromFrame(genFileName + "/frame_" + QString::number(curFrameCount, 10), datapro->getDuringFixPoints(curFrameCount), frame);
		}
		frame = cvQueryFrame(cap);
		curFrameCount++;
	}
}
void ImageProcess::genImageFromFrame(QString filename, QVector<Point *>* fixpoints, IplImage *frame)
{
	cvSaveImage((filename+".jpg").toStdString().c_str(), frame);
	QFile file(filename + ".txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		//cout << "Open failed." << endl;
		return ;
	}

	QTextStream txtOutput(&file);
	for (int i = 0; i < fixpoints->size(); i++)
	{
		float sx = fixpoints->at(i)->x;
		float sy =fixpoints->at(i)->y;
		int x = int(sx);
		int y = int(sy);
		txtOutput << x << "\t" << y<< endl;
	}
	file.close();
}

