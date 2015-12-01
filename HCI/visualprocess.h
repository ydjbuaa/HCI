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
	//Mat  curImage;   //��ǰͼ��
	VisualProcess();
	~VisualProcess();
	void setErrorMessage(QString msg);
	void setPainterSize(int w, int h);                  //����ͼƬ��Ƶ����ʾ��SIZE
	bool setAndLoadFile(QString filename);     //�����ļ�������
	Mat getCurImageMat();                                 //��ȡ��ǰ��Image
	Mat getCurVisualImageMat();    //��ȡ��ǰ���ӻ���ͼ��
	void getCurCutImageMat();   //��ȡ��ǰcutͼ��
	IplImage *getKFrame(int k);
	void addVideoFrame(IplImage *frame);  //���һ����Ƶ֡
	bool saveFile(QString filename);
	QString curFileName;   //��ǰ�ļ�
	QString saveFileName;
	QString curPathName;   //��ǰ�ļ�·��
	WorkState curWorkState;
	FileStatue filestatue;
	QString errmsg;
	bool fixDataLoaded = false;            // ������Ƶ�ļ���fixdata load
	int window_width;
	int window_height;
	bool isImageCut = false;
	Mat cutImage;
    //void InitLoadFile(QString filename);
private:

	Mat  curImage;   //��ǰͼ��
	
	bool isFixRected=false;
	
	Rect fixrect;       //��ǰ��ע��Ŀ�
	CvCapture  * curCapture;  //��ǰ��Ƶ
	QVector<Point *> * fpoints;
	QVector< IplImage *> * videoframes;
	//��Ƶ��Ϣ
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
