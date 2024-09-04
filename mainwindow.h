#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"
#include <QPainter>
#include <QString>
#include <QResizeEvent>
#include "mythread.h"
#include <stdbool.h>
#include <QGestureEvent>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QUrl>
//#include <QFile>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QListView>
#include <QStringListModel>
#include "tcpclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPainter* getPainter();
    QPainter* getPainterMem();
    QPainter* getPainterStr();
    void SendMessage(int KeyCode);
    void copyToDc();
    void copyToDcStr();
    void copyFromDcToStr();
    void OpenAndroidMenu();
    void SetFontSize();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);

    void resizeEvent(QResizeEvent *event);

    void keyPressEvent(QKeyEvent *event);

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool gestureEvent(QGestureEvent *event);
    //void panTriggered(QPanGesture *gesture);
    //void tapTriggered(QTapGesture *gesture);
    void pinchTriggered(QPinchGesture *gesture);
    void tapAndHoldTriggered(QTapAndHoldGesture *gesture);



public slots:
    void timeEvent();
    void KeyIni();
    void WindowIni();
    void OnlineHelp();
    void WhatsNew();
    void KeyHelp();
    void About();
    void ExitGame();
    void ExportSave();
    void ImportSave();
    void VersionBlack();
    void VersionColor();
    void SetColor();
    void SetDirection();
    void ImportOldSaves();
    void HighAtt();
    void UseDotFont();
    void SetDirectionA();
    void SetDirectionB();
    void LargerKey();
    void SmallerKey();
    void Back1();
    void Back2();
    void Back3();
    void Back4();
    void Back5();
    void AutoSave();
    void AllowUpdate();
    void CanTouch();
    void Faster();
    void Slower();
    void LayerKey();
    void FullScreen();
    void CheckUpdate();
    void StatusBar();

    void  ScaleScreen(int scale);
    //void replyFinished(QNetworkReply*);

    void DealBuff(char *c_rcvbuf,QString path);

    //void ZGZBMOD();
    void OpenMod();
    void DownloadMod();
    void CloseMod();
    void AboutMod();
    void GameMenu(QMenu *qmenu);
    void Game_Open(int rowid);
    void selectedDownFile();
    void selectedOpenFile();

    void UploadSaves();
    void DownloadSaves();
    void ManageSaves();
    void UserLogin();
    void UserLogout();
    void UserRegister();
private:

    int MouseStartX,MouseStartY;
    int MouseEndX,MouseEndY;
    int mousePressTime;

    QPixmap* mPixmap;
    QPainter* mPainter;

    QPixmap* mPixmapMem;
    QPainter* mPainterMem;

    QPixmap* mPixmapStr;
    QPainter* mPainterStr;

    MyThread myThread;
    QStringList dateList;
    QMenu *gameMenu;

    QNetworkReply *reply;
    QFile *xmlfile;
    QFile *downloadfile;


    QStringList rpgGameList;
    QXmlStreamReader reader;
    QListView *list;
    QListView *openlist;



    QAction *actionAutoSave;
    QAction *actionAllowUpdate ;
    QAction *actionUseDotFont;
    QAction *actionHighAtt;

    QAction *actionBack1;
    QAction *actionBack2;
    QAction *actionBack3;
    QAction *actionBack4;
    QAction *actionBack5;

    QAction *actionOpenMod;
    QAction *actionDownloadMod;
    QAction *actionCloseMod;
    QAction *actionAboutMod;
    //QFile *file;
    //QNetworkReply *reply;

    void readConfigElement();
    //读取Files节点
    void readFilesElement();
    //读取File节点);
    void readFileElement();
    //无关节点跳过
    void skipUnknownElement();


};





#endif // MAINWINDOW_H
