#include "DataProcess.h"
#include"qstring.h"
#include"qfile.h"
#include"qtextstream.h"
#include"qvector.h"
DataProcess::DataProcess()
{
	fixpoints = new QVector<QVector<QPoint>*>();
}


DataProcess::~DataProcess()
{
}
QVector<QPoint>* DataProcess::getPreprocessedPoints(QString filename)
{
	QVector<QPoint> *points=new QVector<QPoint>();
	QFile f(filename);
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		QString lineStr;
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			if (!lineStr.startsWith("##"))
			{
				break;
			}
		}
		lineStr = txtInput.readLine();
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			QStringList list = lineStr.split("\t");
			//+		lineStr	7943777265	SMP	1	1561.08	297.82	Fixation	88.avi	QString

			try{
				float x = list.at(3).toFloat();
				float y = list.at(4).toFloat();

				QPoint point(x, y);
				points->push_back(point);
			}
			catch (QString e)
			{
				//do nothing
			}
		}
		f.close();
	}
	
	fixpoints->push_back(points);
	return points;
}
QPoint * DataProcess::getCenterFixPoint(int frame_num, int index )
{
	if (fixpoints->size() < index)
		return NULL;
	QVector<QPoint > * vpoints = fixpoints->at(index - 1);
	int x = 0;
	int y = 0;
	int count = 0;
    
	for (int i = 50 * (frame_num - 1) / 3; i <= 50 * frame_num / 3&&i<vpoints->size(); i++)
	{
		count++;
		float  sx= vpoints->at(i).x();
		float  sy= vpoints->at(i).y();
		float scale_rate_x = float(SCREEN_WIDTH) / frame_width;
		float scale_rate_y = float(SCREEN_HEIGH) / frame_height;
		if (scale_rate_x > scale_rate_y)
		{
			sx = sx - (SCREEN_WIDTH - frame_width) / 2;
			sx = (sx > 0) ? (sx / scale_rate_y) : 0;
			sy = sy / scale_rate_y;
		}
		else{
			sx = sx / scale_rate_x;
			sy = sy - (SCREEN_HEIGH - frame_height) / 2;
			sy = (sy > 0) ? (sy / scale_rate_x) : 0;
		}
		x += sx;
		y += sy;
	}
	if (count == 0)
		count++;
	x /= count;
	y /= count;
	return new QPoint(x, y);
}
void DataProcess::setFrameSize(int w, int h)
{
	this->frame_width = w;
	this->frame_height = h; 
}
QVector<Point *> * DataProcess::getFixPointsFromFile(QString filename)
{
	QVector<Point *> *fpoints = new QVector<Point *>();
	QFile f(filename);
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		QString lineStr;
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			QStringList list = lineStr.split("\t");
			//+		lineStr	7943777265	SMP	1	1561.08	297.82	Fixation	88.avi	QString
				Point *point=new Point(list.at(0).toInt(), list.at(1).toInt());
				fpoints->push_back(point);
			}
		}
	 f.close();
	 return fpoints;
}
QVector <Point *>* DataProcess::getDuringFixPoints(int frame_num)
{
	int being_index = (frame_num - 9) * 50 / 3;
	being_index = (being_index > 0) ? being_index : 0;
	QVector<Point *> * durfixpoints = new QVector<Point*>();
	for (int i = 0; i < fixpoints->size(); i++)
	{
		QVector<QPoint> *vp = fixpoints->at(i);
		for (int j = being_index; j < being_index + 300 && j < vp->size(); j++)
		{
			float sx = vp->at(j).x();
			float sy = vp->at(j).y();
			float scale_rate_x = float(SCREEN_WIDTH) / frame_width;
			float scale_rate_y = float(SCREEN_HEIGH) / frame_height;
			if (scale_rate_x > scale_rate_y)
			{
				sx = sx - (SCREEN_WIDTH - frame_width) / 2;
				sx = (sx > 0) ? (sx / scale_rate_y) : 0;
				sy = sy / scale_rate_y;
			}
			else{
				sx = sx / scale_rate_x;
				sy = sy - (SCREEN_HEIGH - frame_height) / 2;
				sy = (sy > 0) ? (sy / scale_rate_x) : 0;
			}
			int x = int(sx);
			int y = int(sy);
			durfixpoints->push_back(new Point(x, y));
		}
	}
	return durfixpoints;
}