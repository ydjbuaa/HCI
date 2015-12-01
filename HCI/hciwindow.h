#ifndef HCIWINDOW_H
#define HCIWINDOW_H

#include <QObject>
#include<qlabel.h>
#include <QtWidgets/QMainWindow>
#include"CrabCut.h"
#include"DataProcess.h"
#include"hcithread.h"
#include"qthread"
#include"visualprocess.h"
class HCIWindow : public QMainWindow
{
	Q_OBJECT
	const int _WINDOW_HEIGHT = 800;
	const int _WINDOW_WINDTH = 1000;
public:
	HCIWindow();
	~HCIWindow();
	//QVector<QPoint*>*fixpoints;
	private slots:
	void setfileloaded();
	void open();
	void save();
	void icut();
	void ishow();  
	void vshow();
	void vcut();
	void about();
	void imageload();
	void videoload();
	void imagecut();
	void nextFrame();
	void showStatues();
	void finishsavefile();
private:
	int fixCount1 = 1;
	int fixCount2 = 1;
	//bool fixfileLoaded;
	//bool isimagecut = false;
	//int workTime = 0;
	//int fileType = 0;
	//int workState = 0;            // 0 not word 1 icut
	int curIntFrame = 0;
	void createActions();
	void createMenus();
	void transMatToQImage(Mat & mat);
	void displayVideo();
	void  disVisualFixVideo(CvCapture * capture);
	void playvideo();
	//void saveFile(QString filename);
	//QVector<QVector<QPoint>*>* fixpoints;
	
	bool isSavingFile = false;

	CvCapture * capture;          //��Ƶ��ͼ����ʾ����
	Mat   imat;                         //
	Point *lastpoint1;
	Point * lastpoint2;
	IplImage *iplImg;
	IplImage *frame;
	QImage *qImg;
	QTimer *timer;

	//QString curFile;                   //��ǰ�ļ�·��
	//QString curFilePath;            //·����

	QLabel * label;                    //
	QStatusBar *statusBar;
	QMenuBar * menuBar;
	QWidget * centralWidget;
	QMenu *fileMenu;              //�ļ������˵�
	QMenu *editMenu;             //�ļ��༭�˵�
	QMenu *helpMenu;            // �����˵�

	//��Ӧ����
	QAction * openAct;             //���ļ�����
	QAction * saveAct;             // �����ļ�
	QAction * exitAct;              // �˳�
	QAction * icutAct;              //crabcut ��ȡ�ӵ��ע for image
	QAction * vcutAct;             // crabcut ��ȡ�ӵ��ע for video
	QAction * ishowAct;          //ͼƬ�Ŀ��ӻ���ʾ
	QAction * vshowAct;         //video �Ŀ��ӻ���ʾ
	QAction *aboutAct;        //��ʾ�����۶��ǵ���Ϣ

protected:
	void paintEvent(QPaintEvent *e);
signals:
	void fileloadsig();
	void savefilesig();
};
//extern HCIWindow * hciwindow;
#endif // HCIWINDOW_H
