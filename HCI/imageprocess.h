#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include"CrabCut.h"
#include"DataProcess.h"
class ImageProcess : public QObject
{
	Q_OBJECT
	const int SCREEN_WIDTH = 1680;         //ÑÛ¶¯ÒÇÆÁÄ»size
	const int SCREEN_HEIGH = 1050;
	int frame_width = 0;
	int frame_height = 0;
public:
	ImageProcess();
	~ImageProcess();
	void preProcessVideo(QString path);
	void genImageFromFrame(QString filename, QVector<Point *>* fixpoints, IplImage *frame);
private:
	
};
extern ImageProcess * imgpro;

#endif // IMAGEPROCESS_H
