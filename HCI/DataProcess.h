#pragma once
#include"qpoint.h"
#include<qobject.h>
#include"CrabCut.h"
class DataProcess
{
	//_OBJECT
private:
	
	const int SCREEN_WIDTH = 1680;         //ÑÛ¶¯ÒÇÆÁÄ»size
	const int SCREEN_HEIGH = 1050;
	int frame_width = 0;
	int frame_height = 0;
public:
	QVector < QVector<QPoint>* > *fixpoints;
	DataProcess();
	~DataProcess();
	void setFrameSize(int w, int h);
	QVector<QPoint> *getPreprocessedPoints(QString filename);
	QPoint * getCenterFixPoint(int frame_num,int index=1);
	QVector<QPoint> * getAllCenterFixPoint(int frame_num);
	QVector <Point *> *getDuringFixPoints(int frame_num);
	QVector<Point *> * getFixPointsFromFile(QString filename);
	
};
extern DataProcess * datapro;

