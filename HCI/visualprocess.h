#ifndef VISUALPROCESS_H
#define VISUALPROCESS_H

#include <QObject>
#include<qthread.h>
#include"CrabCut.h"
#include"qimage.h"
#include"CrabCut.h"
enum FileStatue{
	EMPTY = 0,
    IMAGE=1,
	VIDEO=2,
};
enum WorkState{
	FREE=0,
	IMAGELOAD=1,
	IMAGEVSHOW=2,
	IMAGECUT=3,
	VIDEOLOAD=4,
	VIDEOPLAY=5,
    VIDEOVSHOW=6,
	VIDEOCUT=7,
};
class VisualProcess :  public QObject
{
	Q_OBJECT

public:
	//QImage qImg;
	//Mat  curImage;   //当前图像
	VisualProcess();
	~VisualProcess();
	void setErrorMessage(QString msg);
	void setPainterSize(int w, int h);                  //设置图片视频可显示的SIZE
	bool setAndLoadFile(QString filename);     //读入文件并设置
	Mat getCurImageMat();                                 //获取当前的Image
	Mat getCurVisualImageMat();    //获取当前可视化的图像
	void getCurCutImageMat();   //获取当前cut图像
	IplImage *getKFrame(int k);
	void addVideoFrame(IplImage *frame);  //添加一个视频帧
	bool saveFile(QString filename);
	QString curFileName;   //当前文件
	QString saveFileName;
	QString curPathName;   //当前文件路径
	WorkState curWorkState;
	FileStatue filestatue;
	QString errmsg;
	bool fixDataLoaded = false;            // 对于视频文件，fixdata load
	int window_width;
	int window_height;
	bool isImageCut = false;
	Mat cutImage;
    //void InitLoadFile(QString filename);
private:

	Mat  curImage;   //当前图像
	
	bool isFixRected=false;
	
	Rect fixrect;       //当前关注点的框
	CvCapture  * curCapture;  //当前视频
	QVector<Point *> * fpoints;
	QVector< IplImage *> * videoframes;
	//视频信息
	int frameH ;
	int frameW ;
	int fps ;
	int numFrames ;
signals:
	void imageloadsig();
	void imagecutsig();
	void videoloadsig();
	void errorfindsig();


};
extern VisualProcess * visualthread; 
#endif // VISUALPROCESS_H
