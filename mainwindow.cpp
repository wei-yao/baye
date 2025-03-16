#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QTimer>
//#include <QSound>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QProcess>
#include <QFileDialog>
#include <QSettings>
#include <QSignalMapper>
#include <math.h>
#include <unistd.h>
#include "mainwindow.h"
#include "stdlib.h"
#include "compa.h"
#include "fundef.h"
#include "sources.h"
#include "comm.h"
#include 	"enghead.h"
#include "windowinidialog.h"
#include "keyinidialog.h"
#include "aboutdialog.h"
//#include <qdatetime.h>
#include "savechange.h"
#include <QTextCodec>
#include "logindialog.h"
#include "tcpdatastruct.h"

#if defined(Q_OS_ANDROID)
#include<QtAndroidExtras/QAndroidJniObject>
#include<QtAndroidExtras/QAndroidJniEnvironment>
#include<QtAndroidExtras/QtAndroid>
//#include <QtAndroidExtras>

using namespace QtAndroid;

#elif defined (Q_OS_IOS)
#include "ios-sources/src/viewRoute.h"
#endif


//extern "C" void * GamBaYeEng(void *p);
//extern "C" void GamBaYeEng();

extern int g_keyCode;
extern int g_keyCodeList[100];
extern int g_keyCodeListMax;
extern int g_screenwidth;
extern int g_screenheight;
extern double g_screentimes;
extern int g_gameviewleft;
extern int g_gameviewright;
extern int g_gameviewtop;
extern int g_gameviewbottom;
extern int g_gameviewdirect;//20180304
extern int g_gameviewlefta;
extern int g_gameviewrighta;
extern int g_timerTick;
extern int g_showPicType;
extern int g_StoredMax;
int g_HighAtt;
int g_timerAdd = 0;
int g_useDotFont;
int reflushflag = 1;
int g_reflushflag = 0;
int g_autoSave = 0;
int g_allowUpdate = 0;
int g_viewadd = 0;//20180713用于界面大小控制
int g_touchEnable  = 0;//20180714 触屏使能
int g_delayTimes  = 10 ;//20180725 延时倍数
int g_layerKey = 0;//20180810
int g_fullScreen = 0;//20180810
int g_statusBar = 0;//20181002
U8 g_modName[20];
int g_modVer = 100;
int titleUpdate = 1;

QColor g_backColor;//0-白色 1-原色 2-护眼色 3-黑色
QString g_startName;
int g_startFlag;
U8 g_startName_char[200];

extern int g_stat;
extern int ng;//用于状态(触屏用)
extern int nga;
extern int ngb;
extern int ngc;
extern int ngsave;
extern int ngsecond1;
extern int ngthird1;
extern int flagwar;
extern int flagstat;
extern int ngthird2;
extern int ngthird3;
extern int ngthird4;
extern int ngf;//flagshowskill;//战斗技能菜单ngf2
extern int operateflag;
extern int g_Fontsize;
extern QString androidFont;
extern int version_now;
extern QImage * g_cityMap;
extern QImage * g_fightMap;
extern QString startUpIniPath;
extern QString versionIniPath;
extern QString title;

extern int g_useMOD;
extern Player g_player;
extern int g_modsupportColor;

#define MAINLEFTARROWX 0
#define MAINLEFTARROWY 224
#define MAINRIGHTARROWX 640
#define MAINRIGHTARROWY 224
#define MAINUPARROWX 320
#define MAINUPARROWY 0
#define MAINDOWNARROWX 320
#define MAINDOWNARROWY 448
#define MAINMENUX 690
#define MAINMENUY 420

#define FIGHTLEFTARROWX 0
#define FIGHTLEFTARROWY 185
#define FIGHTRIGHTARROWX 760
#define FIGHTRIGHTARROWY 185
#define FIGHTUPARROWX 380
#define FIGHTUPARROWY 0
#define FIGHTDOWNARROWX 380
#define FIGHTDOWNARROWY 370
#define FIGHTMENUX 590
#define FIGHTMENUY 420
#define FIGHTBACKX 690
#define FIGHTBACKY 420


// -------------------- //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*mPixmap = new QPixmap(800, 600);
    mPainter = new QPainter(mPixmap);
    mPixmapMem = new QPixmap(800, 600);
    mPainterMem = new QPainter(mPixmapMem);
    mPixmapStr = new QPixmap(800, 600);
    mPainterStr = new QPainter(mPixmapStr);*/
    mPixmap = new QPixmap(g_screenwidth, g_screenheight);
    mPainter = new QPainter(mPixmap);
    mPixmapMem = new QPixmap(g_screenwidth, g_screenheight);
    mPainterMem = new QPainter(mPixmapMem);
    mPixmapStr = new QPixmap(g_screenwidth, g_screenheight);
    mPainterStr = new QPainter(mPixmapStr);

    g_cityMap = new QImage(32*80,32*80,QImage::Format_RGB888);
    g_fightMap = new QImage(32*80,32*80,QImage::Format_RGB888);

#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)


    // 添加菜单

    //Qt::Key_Menu
    QMenu *gameMenu = menuBar()->addMenu(tr("文件"));

    QMenu *gameMenuSave  = gameMenu->addMenu(tr("存档"));

    QAction *actionExportSave = new QAction("本地存档导出", this);
    connect(actionExportSave, SIGNAL(triggered()), this, SLOT(ExportSave()));
    gameMenuSave->addAction(actionExportSave);

    QAction *actionImportSave = new QAction("本地存档导入", this);
    connect(actionImportSave, SIGNAL(triggered()), this, SLOT(ImportSave()));
    gameMenuSave->addAction(actionImportSave);

    QAction *actionImportOldSaves = new QAction("旧版本存档导入", this);
    connect(actionImportOldSaves, SIGNAL(triggered()), this, SLOT(ImportOldSaves()));
    gameMenuSave->addAction(actionImportOldSaves);


    QMenu *gameMenuMod  = gameMenu->addMenu(tr("MOD"));
    actionOpenMod = new QAction("加载", this);
    connect(actionOpenMod, SIGNAL(triggered()), this, SLOT(OpenMod()));
    gameMenuMod->addAction(actionOpenMod);

    actionDownloadMod = new QAction("下载", this);
    connect(actionDownloadMod, SIGNAL(triggered()), this, SLOT(DownloadMod()));
    gameMenuMod->addAction(actionDownloadMod);

    actionCloseMod = new QAction("关闭", this);
    connect(actionCloseMod, SIGNAL(triggered()), this, SLOT(CloseMod()));
    gameMenuMod->addAction(actionCloseMod);

    actionAboutMod = new QAction("说明", this);
    connect(actionAboutMod, SIGNAL(triggered()), this, SLOT(AboutMod()));
    gameMenuMod->addAction(actionAboutMod);

    //QAction *actionMOD = new QAction("MOD", this);
    //connect(actionMOD, SIGNAL(triggered()), this, SLOT(MOD()));
    //gameMenu->addAction(actionMOD);

    QAction *actionExit = new QAction("退出", this);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(ExitGame()));
    gameMenu->addAction(actionExit);


    QMenu *gameMenu1 = menuBar()->addMenu(tr("选项"));
    QAction *actionWindowIni = new QAction("窗口配置", this);
    connect(actionWindowIni, SIGNAL(triggered()), this, SLOT(WindowIni()));
    gameMenu1->addAction(actionWindowIni);

    QAction *actionKeyIni = new QAction("按键配置", this);
    connect(actionKeyIni, SIGNAL(triggered()), this, SLOT(KeyIni()));
    gameMenu1->addAction(actionKeyIni);

    QAction *actionBlack = new QAction("切换黑白版", this);
    connect(actionBlack, SIGNAL(triggered()), this, SLOT(VersionBlack()));
    gameMenu1->addAction(actionBlack);

    QAction *actionColor = new QAction("切换彩色版", this);
    connect(actionColor, SIGNAL(triggered()), this, SLOT(VersionColor()));
    gameMenu1->addAction(actionColor);

    QMenu *gameMenuBackColor  = gameMenu1->addMenu(tr("背景颜色"));
    actionBack1 = new QAction("白色", this);
    connect(actionBack1, SIGNAL(triggered()), this, SLOT(Back1()));
    actionBack1->setCheckable(true);
    actionBack1->setChecked(false);
    if(g_backColor == Qt::white)actionBack1->setChecked(true);
    gameMenuBackColor->addAction(actionBack1);

    actionBack2 = new QAction("原色", this);
    connect(actionBack2, SIGNAL(triggered()), this, SLOT(Back2()));
    actionBack2->setCheckable(true);
    actionBack2->setChecked(false);
    if(g_backColor == QColor(132,132,132))actionBack2->setChecked(true);
    gameMenuBackColor->addAction(actionBack2);

    actionBack3 = new QAction("护眼色", this);
    connect(actionBack3, SIGNAL(triggered()), this, SLOT(Back3()));
    actionBack3->setCheckable(true);
    actionBack3->setChecked(false);
    if(g_backColor == QColor(204,232,207))actionBack3->setChecked(true);
    gameMenuBackColor->addAction(actionBack3);

    actionBack4 = new QAction("天蓝色", this);
    connect(actionBack4, SIGNAL(triggered()), this, SLOT(Back4()));
    actionBack4->setCheckable(true);
    actionBack4->setChecked(false);
    if(g_backColor == QColor(148,198,221))actionBack4->setChecked(true);
    gameMenuBackColor->addAction(actionBack4);

    actionBack5 = new QAction("棕色", this);
    connect(actionBack5, SIGNAL(triggered()), this, SLOT(Back5()));
    actionBack5->setCheckable(true);
    actionBack5->setChecked(false);
    if(g_backColor == QColor(198,151,81))actionBack5->setChecked(true);
    gameMenuBackColor->addAction(actionBack5);

    //20180725 add
    QMenu *gameMenuDelayTimes  = gameMenu1->addMenu(tr("延时调整"));
    QAction *actionFast = new QAction("减少延时", this);
    connect(actionFast, SIGNAL(triggered()), this, SLOT(Faster()));
    gameMenuDelayTimes->addAction(actionFast);
    QAction *actionSlow = new QAction("增加延时", this);
    connect(actionSlow, SIGNAL(triggered()), this, SLOT(Slower()));
    gameMenuDelayTimes->addAction(actionSlow);


    QMenu *gameMenuOther = gameMenu1->addMenu(tr("其他设置"));
    actionAutoSave = new QAction("实时存档", this);
    actionAutoSave->setCheckable(true);
    actionAutoSave->setChecked(false);
    if(g_autoSave == 1)actionAutoSave->setChecked(true);
    connect(actionAutoSave, SIGNAL(triggered()), this, SLOT(AutoSave()));
    gameMenuOther->addAction(actionAutoSave);

    actionAllowUpdate = new QAction("允许自动更新", this);
    actionAllowUpdate->setCheckable(true);
    actionAllowUpdate->setChecked(false);
    if(g_allowUpdate == 1)actionAllowUpdate->setChecked(true);
    connect(actionAllowUpdate, SIGNAL(triggered()), this, SLOT(AllowUpdate()));
    gameMenuOther->addAction(actionAllowUpdate);

    actionUseDotFont = new QAction("使用点阵字体", this);
    actionUseDotFont->setCheckable(true);
    actionUseDotFont->setChecked(false);
    if(g_useDotFont == 1)actionUseDotFont->setChecked(true);
    connect(actionUseDotFont, SIGNAL(triggered()), this, SLOT(UseDotFont()));
    gameMenuOther->addAction(actionUseDotFont);

    /*QMenu *gameMenu4 = menuBar()->addMenu(tr("用户"));
    QAction *actionUserLogin = new QAction("登录", this);
    connect(actionUserLogin, SIGNAL(triggered()), this, SLOT(UserLogin()));
    gameMenu4->addAction(actionUserLogin);

    QAction *actionUserRegister = new QAction("注册", this);
    connect(actionUserRegister, SIGNAL(triggered()), this, SLOT(UserRegister()));
    gameMenu4->addAction(actionUserRegister);

    QAction *actionUploadSaves = new QAction("云存档上传", this);
    connect(actionUploadSaves, SIGNAL(triggered()), this, SLOT(UploadSaves()));
    gameMenu4->addAction(actionUploadSaves);

    QAction *actionDownloadSaves = new QAction("云存档下载", this);
    connect(actionDownloadSaves, SIGNAL(triggered()), this, SLOT(DownloadSaves()));
    gameMenu4->addAction(actionDownloadSaves);

    QAction *actionManageSaves = new QAction("云存档管理", this);
    connect(actionManageSaves, SIGNAL(triggered()), this, SLOT(ManageSaves()));
    gameMenu4->addAction(actionManageSaves);

    QAction *actionUserLogout = new QAction("注销", this);
    connect(actionUserLogout, SIGNAL(triggered()), this, SLOT(UserLogout()));
    gameMenu4->addAction(actionUserLogout);*/

    QMenu *gameMenu2 = menuBar()->addMenu(tr("模式"));
    actionHighAtt = new QAction("高攻击概率模式", this);
    actionHighAtt->setCheckable(true);
    actionHighAtt->setChecked(false);
    if(g_HighAtt == 1)actionHighAtt->setChecked(true);
    connect(actionHighAtt, SIGNAL(triggered()), this, SLOT(HighAtt()));
    gameMenu2->addAction(actionHighAtt);

    QAction *actionDebugMode = new QAction("调试模式", this);
    //connect(actionDebugMode, SIGNAL(triggered()), this, SLOT(DebugMode()));
    gameMenu2->addAction(actionDebugMode);


    QMenu *gameMenu3 = menuBar()->addMenu(tr("帮助"));
    QAction *actionWhatsNew = new QAction("更新记录", this);
    connect(actionWhatsNew, SIGNAL(triggered()), this, SLOT(WhatsNew()));
    gameMenu3->addAction(actionWhatsNew);

    QAction *actionKeyHelp = new QAction("按键指南", this);
    connect(actionKeyHelp, SIGNAL(triggered()), this, SLOT(KeyHelp()));
    gameMenu3->addAction(actionKeyHelp);

    QAction *actionOnlineHelp = new QAction("在线帮助", this);
    connect(actionOnlineHelp, SIGNAL(triggered()), this, SLOT(OnlineHelp()));
    gameMenu3->addAction(actionOnlineHelp);

    QAction *actionAbout = new QAction("关于", this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    gameMenu3->addAction(actionAbout);
#elif defined(Q_OS_IOS)


    // 添加菜单

    //Qt::Key_Menu
    QMenu *gameMenu = menuBar()->addMenu(tr("文件"));

    QMenu *gameMenuMod  = gameMenu->addMenu(tr("MOD"));
    actionOpenMod = new QAction("加载", this);
    connect(actionOpenMod, SIGNAL(triggered()), this, SLOT(OpenMod()));
    gameMenuMod->addAction(actionOpenMod);

    actionDownloadMod = new QAction("下载", this);
    connect(actionDownloadMod, SIGNAL(triggered()), this, SLOT(DownloadMod()));
    gameMenuMod->addAction(actionDownloadMod);

    actionCloseMod = new QAction("关闭", this);
    connect(actionCloseMod, SIGNAL(triggered()), this, SLOT(CloseMod()));
    gameMenuMod->addAction(actionCloseMod);

    actionAboutMod = new QAction("说明", this);
    connect(actionAboutMod, SIGNAL(triggered()), this, SLOT(AboutMod()));
    gameMenuMod->addAction(actionAboutMod);

    //QAction *actionZGZBMOD = new QAction("战国争霸MOD", this);
    //connect(actionZGZBMOD, SIGNAL(triggered()), this, SLOT(ZGZBMOD()));
    //gameMenu->addAction(actionZGZBMOD);


    QAction *actionExit = new QAction("退出", this);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(ExitGame()));
    gameMenu->addAction(actionExit);


    QMenu *gameMenu1 = menuBar()->addMenu(tr("选项"));
    //QMenu *gameMenuOriention = gameMenu1->addMenu(tr("屏幕方向"));
    QAction *actionOrientionPortrait = new QAction("切换为竖屏", this);
    connect(actionOrientionPortrait, SIGNAL(triggered()), this, SLOT(SetDirectionA()));
    gameMenu1->addAction(actionOrientionPortrait);

    QAction *actionOrientionLandscape = new QAction("切换为横屏", this);
    connect(actionOrientionLandscape, SIGNAL(triggered()), this, SLOT(SetDirectionB()));
    gameMenu1->addAction(actionOrientionLandscape);

    //QAction *actionWindowIni = new QAction("窗口配置", this);
    //connect(actionWindowIni, SIGNAL(triggered()), this, SLOT(WindowIni()));
    //gameMenu1->addAction(actionWindowIni);

    //QAction *actionKeyIni = new QAction("按键配置", this);
    //connect(actionKeyIni, SIGNAL(triggered()), this, SLOT(KeyIni()));
    //gameMenu1->addAction(actionKeyIni);

    QAction *actionBlack = new QAction("切换黑白版", this);
    connect(actionBlack, SIGNAL(triggered()), this, SLOT(VersionBlack()));
    gameMenu1->addAction(actionBlack);

    QAction *actionColor = new QAction("切换彩色版", this);
    connect(actionColor, SIGNAL(triggered()), this, SLOT(VersionColor()));
    gameMenu1->addAction(actionColor);

    QMenu *gameMenuScreen = gameMenu1->addMenu(tr("屏幕设置"));
    QAction *actionEnableTouch = new QAction("允许触屏", this);
    actionEnableTouch->setCheckable(true);
    actionEnableTouch->setChecked(false);
    if(g_touchEnable == 1)actionEnableTouch->setChecked(true);
    //connect(actionEnableTouch, SIGNAL(triggered()), this, SLOT(UseDotFont()));
    gameMenuScreen->addAction(actionEnableTouch);

    QAction *actionLargerKey = new QAction("增大按键尺寸", this);
    connect(actionLargerKey, SIGNAL(triggered()), this, SLOT(LargerKey()));
    gameMenuScreen->addAction(actionLargerKey);

    QAction *actionSmallerKey = new QAction("减小按键尺寸", this);
    connect(actionSmallerKey, SIGNAL(triggered()), this, SLOT(SmallerKey()));
    gameMenuScreen->addAction(actionSmallerKey);

    QMenu *gameMenuBackColor  = gameMenu1->addMenu(tr("背景颜色"));
    actionBack1 = new QAction("白色", this);
    connect(actionBack1, SIGNAL(triggered()), this, SLOT(Back1()));
    gameMenuBackColor->addAction(actionBack1);

    actionBack2 = new QAction("原色", this);
    connect(actionBack2, SIGNAL(triggered()), this, SLOT(Back2()));
    gameMenuBackColor->addAction(actionBack2);

    actionBack3 = new QAction("护眼色", this);
    connect(actionBack3, SIGNAL(triggered()), this, SLOT(Back3()));
    gameMenuBackColor->addAction(actionBack3);

    actionBack4 = new QAction("天蓝色", this);
    connect(actionBack4, SIGNAL(triggered()), this, SLOT(Back4()));
    gameMenuBackColor->addAction(actionBack4);

    actionBack5 = new QAction("棕色", this);
    connect(actionBack5, SIGNAL(triggered()), this, SLOT(Back5()));
    gameMenuBackColor->addAction(actionBack5);

    //20180725 add
    QMenu *gameMenuDelayTimes  = gameMenu1->addMenu(tr("延时调整"));
    QAction *actionFast = new QAction("减少延时", this);
    connect(actionFast, SIGNAL(triggered()), this, SLOT(Faster()));
    gameMenuDelayTimes->addAction(actionFast);
    QAction *actionSlow = new QAction("增加延时", this);
    connect(actionSlow, SIGNAL(triggered()), this, SLOT(Slower()));
    gameMenuDelayTimes->addAction(actionSlow);


    QMenu *gameMenuOther = gameMenu1->addMenu(tr("其他设置"));
    QAction *actionAutoSave = new QAction("实时存档", this);
    actionAutoSave->setCheckable(true);
    actionAutoSave->setChecked(false);
    if(g_autoSave == 1)actionAutoSave->setChecked(true);
    connect(actionAutoSave, SIGNAL(triggered()), this, SLOT(AutoSave()));
    gameMenuOther->addAction(actionAutoSave);

    actionAllowUpdate = new QAction("允许自动更新", this);
    actionAllowUpdate->setCheckable(true);
    actionAllowUpdate->setChecked(false);
    if(g_allowUpdate == 1)actionAllowUpdate->setChecked(true);
    connect(actionAllowUpdate, SIGNAL(triggered()), this, SLOT(AllowUpdate()));
    gameMenuOther->addAction(actionAllowUpdate);

    actionUseDotFont = new QAction("使用点阵字体", this);
    actionUseDotFont->setCheckable(true);
    actionUseDotFont->setChecked(false);
    if(g_useDotFont == 1)actionUseDotFont->setChecked(true);
    connect(actionUseDotFont, SIGNAL(triggered()), this, SLOT(UseDotFont()));
    gameMenuOther->addAction(actionUseDotFont);


    /*QAction *actionImportOldSaves = new QAction("导入旧版本存档", this);
    connect(actionImportOldSaves, SIGNAL(triggered()), this, SLOT(ImportOldSaves()));
    gameMenu1->addAction(actionImportOldSaves);*/

    /*QMenu *gameMenu4 = menuBar()->addMenu(tr("用户"));
    QAction *actionUserLogin = new QAction("登录", this);
    connect(actionUserLogin, SIGNAL(triggered()), this, SLOT(UserLogin()));
    gameMenu4->addAction(actionUserLogin);

    QAction *actionUserRegister = new QAction("注册", this);
    connect(actionUserRegister, SIGNAL(triggered()), this, SLOT(UserRegister()));
    gameMenu4->addAction(actionUserRegister);

    QAction *actionUploadSaves = new QAction("云存档上传", this);
    connect(actionUploadSaves, SIGNAL(triggered()), this, SLOT(UploadSaves()));
    gameMenu4->addAction(actionUploadSaves);

    QAction *actionDownloadSaves = new QAction("云存档下载", this);
    connect(actionDownloadSaves, SIGNAL(triggered()), this, SLOT(DownloadSaves()));
    gameMenu4->addAction(actionDownloadSaves);

    QAction *actionManageSaves = new QAction("云存档管理", this);
    connect(actionManageSaves, SIGNAL(triggered()), this, SLOT(ManageSaves()));
    gameMenu4->addAction(actionManageSaves);

    QAction *actionUserLogout = new QAction("注销", this);
    connect(actionUserLogout, SIGNAL(triggered()), this, SLOT(UserLogout()));
    gameMenu4->addAction(actionUserLogout);*/

    QMenu *gameMenu2 = menuBar()->addMenu(tr("模式"));
    actionHighAtt = new QAction("高攻击概率模式", this);
    actionHighAtt->setCheckable(true);
    actionHighAtt->setChecked(false);
    if(g_HighAtt == 1)actionHighAtt->setChecked(true);
    connect(actionHighAtt, SIGNAL(triggered()), this, SLOT(HighAtt()));
    gameMenu2->addAction(actionHighAtt);

    QAction *actionDebugMode = new QAction("调试模式", this);
    //connect(actionDebugMode, SIGNAL(triggered()), this, SLOT(DebugMode()));
    gameMenu2->addAction(actionDebugMode);


    QMenu *gameMenu3 = menuBar()->addMenu(tr("帮助"));
    QAction *actionWhatsNew = new QAction("更新记录", this);
    connect(actionWhatsNew, SIGNAL(triggered()), this, SLOT(WhatsNew()));
    gameMenu3->addAction(actionWhatsNew);

    QAction *actionKeyHelp = new QAction("按键指南", this);
    connect(actionKeyHelp, SIGNAL(triggered()), this, SLOT(KeyHelp()));
    gameMenu3->addAction(actionKeyHelp);

    QAction *actionOnlineHelp = new QAction("在线帮助", this);
    connect(actionOnlineHelp, SIGNAL(triggered()), this, SLOT(OnlineHelp()));
    gameMenu3->addAction(actionOnlineHelp);

    QAction *actionAbout = new QAction("关于", this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    gameMenu3->addAction(actionAbout);
#endif

#if defined(Q_OS_ANDROID)
    //jni ANDROID版本更新
    //QAndroidJniObject javaToast = QAndroidJniObject::fromString("1");
    //QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","checkUpdate","(Ljava/lang/String;)V",javaToast.object<jstring>());
#endif

#if defined(Q_OS_IOS)


     //20180609 从bundle到document卡

    QString fontPath = QDir::homePath() + "/Documents/font.bin";

    if(!QFile::exists(fontPath))
    {
        QFile::copy("font.bin",fontPath);
        QFile fileDest(fontPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }

    QString datlibPath = QDir::homePath() + "/Documents/dat.lib";

    if(!QFile::exists(datlibPath))
    {
        QFile::copy("dat.lib",datlibPath);
        QFile fileDest(datlibPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }

    QString resrlbPath = QDir::homePath() + "/Documents/res.rlb";

    if(QFile::exists("res.rlb"))//下个版本需要取消注释
    {
        QFile::remove(resrlbPath);
    }
    {
        QFile::copy("res.rlb",resrlbPath);
        QFile fileDest2(resrlbPath);
        fileDest2.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }
    QString startupiniPath = QDir::homePath() + "/Documents/startup.ini";

    if(!QFile::exists(startupiniPath))
    {
        QFile::copy("startup.ini",startupiniPath);
        QFile fileDest(startupiniPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    }

    QString versioniniPath = QDir::homePath() + "/Documents/version.ini";
    if(QFile::exists(versioniniPath))//下个版本需要取消注释
    {
        QFile::remove(versioniniPath);
    }
    QFile::copy("version.ini",versioniniPath);
    QFile fileDest(versioniniPath);
    fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

   startUpIniPath =startupiniPath;
   versionIniPath =versioniniPath;

   QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
   QSettings *configIniVersion = new QSettings(versionIniPath, QSettings::IniFormat);
   configIniVersion->setIniCodec("GB2312");
   //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
   int widthx = configIniStartUp->value("/Startup/Width",800).toInt();
   int heighty = configIniStartUp->value("/Startup/Height",600).toInt();
   version_now = configIniVersion->value("/Startup/Version",180701300).toInt();
   int color=configIniStartUp->value("/Startup/Color",0).toInt();
   g_showPicType = color;
   if(g_showPicType == 1)g_showPicType = 2;

   int backcolor=configIniStartUp->value("/Startup/BackColor",0).toInt();
   if(backcolor == 0)
   {
       g_backColor = Qt::white;
   }
   else if(backcolor == 1)
   {
       g_backColor = QColor(132,132,132);
   }
   else if(backcolor == 2)
   {
       g_backColor = QColor(204,232,207);
   }
   else if(backcolor == 3)
   {
       g_backColor = QColor(148,198,221);
   }
   else if(backcolor == 4)
   {
       g_backColor = QColor(198,151,81);
   }


   int screen = configIniStartUp->value("/Startup/Screen",0).toInt();

   int KeyReturn=configIniStartUp->value("/Key/Return",13).toInt();
   int KeyEsc=configIniStartUp->value("/Key/Esc",27).toInt();
   int KeySpace=configIniStartUp->value("/Key/Space",32).toInt();
   int KeyHome=configIniStartUp->value("/Key/Home",36).toInt();
   int KeyUp0=configIniStartUp->value("/Key/Up",38).toInt();
   int KeyDown0=configIniStartUp->value("/Key/Down",40).toInt();
   int KeyLeft0=configIniStartUp->value("/Key/Left",37).toInt();
   int KeyRight0=configIniStartUp->value("/Key/Right",39).toInt();



   int KeyEnable = configIniStartUp->value("/Startup/UseKey",1).toInt();
   int MouseEnable = configIniStartUp->value("/Startup/UseMouse",1).toInt();

   int DebugEnable = configIniStartUp->value("/Debug/Enable",0).toInt();

   title = configIniVersion->value("/Startup/Title",QObject::tr("天际边三国霸业")).toString();


   g_useDotFont = configIniStartUp->value("/Startup/UseDotFont",0).toInt();
   g_HighAtt = configIniStartUp->value("/Startup/HighAtt",0).toInt();
   g_autoSave = configIniStartUp->value("/Startup/AutoSave",1).toInt();
   g_gameviewdirect = configIniStartUp->value("/Startup/ShowWay",1).toInt();
   g_allowUpdate = configIniStartUp->value("/Startup/AllowUpdate",1).toInt();
   g_touchEnable = configIniStartUp->value("/Startup/UseMouse",1).toInt();

   routeView(g_gameviewdirect);

   QDesktopWidget *deskWgt = QApplication::desktop();
   if (deskWgt)
   {
           g_screenwidth = deskWgt->width();
           g_screenheight = deskWgt->height();//去除菜单栏影响
   }
   if(g_gameviewdirect == 0)
   {
       int tmp1 = g_screenwidth;
       g_screenwidth = g_screenheight;
       g_screenheight = tmp1;
   }
   this->setFixedSize(g_screenwidth,g_screenheight);

   if(g_gameviewdirect == 0)//横屏
   {

   //先划分区域比例为1：6：1
       int t_widthm8 = g_screenwidth/8;
       g_gameviewleft = t_widthm8;
       if(g_gameviewleft < 100) g_gameviewleft = 100;
       t_widthm8 = g_gameviewleft;
       g_gameviewright = g_screenwidth - g_gameviewleft;
       int t_width = g_screenwidth - 2 * g_gameviewleft;

       int t_height = t_width/5*3;
       while(t_height>g_screenheight)
       {
           g_gameviewleft+=10;
           g_gameviewright-=10;
           t_width = g_screenwidth - 2 * g_gameviewleft;
           t_height = t_width/5*3;
       }

       g_gameviewtop = 30;
       g_gameviewbottom = g_gameviewtop + t_height;
       g_screentimes = (double)t_width/160.0;
   }
   else if (g_gameviewdirect == 1)//竖屏 默认
   {

       g_gameviewleft = 0;
       g_gameviewright = g_screenwidth;
       g_gameviewtop = 30;//已去除状态栏
       g_gameviewbottom = g_screenwidth*3/5;

       g_screentimes = (double)g_screenwidth/160.0;
   }
   g_Fontsize = 1;
   int heightOfBt = 14 *g_screentimes ;
   while(true)
   {
       QFont tmp(androidFont, g_Fontsize, QFont::Bold);
       tmp.setPointSize(g_Fontsize);
       QFontMetricsF fm(tmp);
       qreal pixelsHigh = fm.height();
       if (pixelsHigh == heightOfBt)
           break;

       if (pixelsHigh > heightOfBt)
       {
           --g_Fontsize;
           break;
       }

       ++g_Fontsize;
   }

#endif


    // 开始游戏

    // GamBaYeEng();
    //QtConcurrent::run(GamBaYeEng);
    /*pthread_t tids; //线程id
    int ret = pthread_create( &tids, NULL, GamBaYeEng, NULL );
    if( ret != 0 ) //创建线程成功返回0
    {
        //err
    }*/
    //pthread_exit( NULL ); //等待各个线程退出后，进程才结束，否则进程强制结束，线程处于未终止的状态
    //thread1.start(GamBaYeEng);
    myThread.start();

    QTimer *testTimer = new QTimer(this);
    connect( testTimer, SIGNAL(timeout()), this, SLOT(timeEvent()) );
    testTimer->start(100);

}

/*void EngStart()
{
     GamBaYeEng();
}*/

void MainWindow::timeEvent()
{

    g_timerAdd ++;
    if(g_timerAdd >= 5)
    {
        g_timerTick = 1;
        g_timerAdd = 0;

    }

    //g_timerTick = 1;
    update();
#if defined(Q_OS_WIN32)
    if(g_useMOD >0 && titleUpdate == 0)
    {
         titleUpdate = 1;
         QString title1;
         title1 = title;
         title =QString::fromLocal8Bit((char *)g_modName);  //QString(QLatin1String((char *)g_modName));
         version_now = g_modVer;

         QString version;
         version.sprintf(" ver%d.%01d.%01d",version_now%1000/100,version_now%100/10,version_now%10);
         title = title + version;//title1 + "  MOD:" +title + version;

         setWindowTitle(title);
    }
#endif
    if(g_startFlag ==2)
    {

        //myThread.stop();
        //myThread.quit();
       // myThread.terminate();

        //myThread.exit(0);
        //myThread.wait();
        //sleep(1);
        //g_startFlag = 0;

        //myThread.start();
        //titleUpdate = 0;

    }
    if(g_startFlag ==1)
    {
        //g_startFlag = 0;
        //myThread.stop();
        //myThread.quit();
        //myThread.terminate();
        //myThread.exit(0);
        //sleep(1);
/*        g_useMOD = 1;
        QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
        configIniStartUp->setValue("/Mod/UseMod",QVariant(g_useMOD));
        delete configIniStartUp;

        QString downloadedFilePath = "";
        QByteArray fileNamearray;
        downloadedFilePath = "/download/";
        fileNamearray = (downloadedFilePath + g_startName).toUtf8();//toLocal8Bit();

        char * ftname = fileNamearray.data();
        memcpy(g_startName_char,ftname,200);

        g_startFlag = 3;
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况1
        QTextCodec::setCodecForLocale(codec);

        QString fileName = g_startName;
        //复制文件
        QString downloadedFilePath;
        QString datModPath;

        if(QFile::exists(datModPath))
        {
            QFile::remove(datModPath);
        }

        int rt = 0;
        while(rt == 0)
        {
            QFile::remove(datModPath);
            rt = QFile::copy(downloadedFilePath,datModPath);
        }
        QFile fileDest(datModPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);






#if defined(Q_OS_ANDROID)
        //QMessageBox::about(this, "提示", "本功能需要重启游戏才能生效！！！");
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("本功能需要重启游戏才能生效！！！");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
        QMessageBox::about(this, "提示", "本功能需要重启游戏才能生效！！！");
#endif*/



        //myThread.start();
        //titleUpdate = 0;
    }

    if(!myThread.isRunning())
    {
        exit(0);
    }
}

void MainWindow::WindowIni()
{
    WindowIniDialog dialog(this);
    dialog.exec();

}

void MainWindow::KeyIni()
{
    KeyIniDialog dialog(this);
    dialog.exec();

}

void MainWindow::Back1()
{
    g_backColor = Qt::white;
    reflushflag = 1;
    g_reflushflag = 1;
    g_StoredMax = 0;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/BackColor",QVariant(0));
    delete configIniStartUp;

#if defined(Q_OS_ANDROID)
#else
    actionBack1->setChecked(true);
    actionBack2->setChecked(false);
    actionBack3->setChecked(false);
    actionBack4->setChecked(false);
    actionBack5->setChecked(false);
#endif
}

void MainWindow::Back2()
{
    g_backColor = QColor(132,132,132);
    reflushflag = 1;
    g_reflushflag = 1;
    g_StoredMax = 0;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/BackColor",QVariant(1));
    delete configIniStartUp;
#if defined(Q_OS_ANDROID)
#else
    actionBack1->setChecked(false);
    actionBack2->setChecked(true);
    actionBack3->setChecked(false);
    actionBack4->setChecked(false);
    actionBack5->setChecked(false);
#endif
}

void MainWindow::Back3()
{
    g_backColor = QColor(204,232,207);
    reflushflag = 1;
    g_reflushflag = 1;
    g_StoredMax = 0;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/BackColor",QVariant(2));
    delete configIniStartUp;
#if defined(Q_OS_ANDROID)
#else
    actionBack1->setChecked(false);
    actionBack2->setChecked(false);
    actionBack3->setChecked(true);
    actionBack4->setChecked(false);
    actionBack5->setChecked(false);
#endif
}

void MainWindow::Back4()
{
    g_backColor = QColor(148,198,221);
    reflushflag = 1;
    g_reflushflag = 1;
    g_StoredMax = 0;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/BackColor",QVariant(3));
    delete configIniStartUp;
#if defined(Q_OS_ANDROID)
#else
    actionBack1->setChecked(false);
    actionBack2->setChecked(false);
    actionBack3->setChecked(false);
    actionBack4->setChecked(true);
    actionBack5->setChecked(false);
#endif
}

void MainWindow::Back5()
{
    g_backColor = QColor(198,151,81);
    reflushflag = 1;
    g_reflushflag = 1;
    g_StoredMax = 0;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/BackColor",QVariant(4));
    delete configIniStartUp;
#if defined(Q_OS_ANDROID)
#else
    actionBack1->setChecked(false);
    actionBack2->setChecked(false);
    actionBack3->setChecked(false);
    actionBack4->setChecked(false);
    actionBack5->setChecked(true);
#endif
}

void MainWindow::Faster()
{
    g_delayTimes = g_delayTimes - g_delayTimes/2;
    if(g_delayTimes==0)g_delayTimes = 1;
}

void MainWindow::Slower()
{
    g_delayTimes  = g_delayTimes + g_delayTimes/2;
    if(g_delayTimes>1000)g_delayTimes = 1000;
}

void MainWindow::AutoSave()
{
    //GamSaveRcd(0);
    if(g_autoSave == 0)
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "实时存档开启成功，每次策略结束后自动覆盖第一存档");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("实时存档开启成功，每次策略结束后自动覆盖第一存档");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "实时存档开启成功，每次策略结束后自动覆盖第一存档");
    actionAutoSave->setChecked(true);
#endif
        g_autoSave = 1;

    }
    else
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "实时存档关闭成功");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("实时存档关闭成功");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "实时存档关闭成功");
    actionAutoSave->setChecked(false);
#endif
        g_autoSave = 0;
    }

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_autoSave == 1)
    {
        configIniStartUp->setValue("/Startup/AutoSave",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/AutoSave",QVariant(0));
    }
    delete configIniStartUp;
}

void MainWindow::AllowUpdate()
{
    //GamSaveRcd(0);
    if(g_allowUpdate == 0)
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "实时存档开启成功，每次策略结束后自动覆盖第一存档");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("自动更新开启");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "自动更新开启");
    actionAllowUpdate->setChecked(true);
#endif
        g_allowUpdate = 1;

    }
    else
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "实时存档关闭成功");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("自动更新关闭");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "自动更新关闭");
    actionAllowUpdate->setChecked(false);
#endif
        g_allowUpdate = 0;
    }

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_allowUpdate == 1)
    {
        configIniStartUp->setValue("/Startup/AllowUpdate",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/AllowUpdate",QVariant(0));
    }
    delete configIniStartUp;
}

void MainWindow::VersionBlack()
{
    g_showPicType = 0;
    g_StoredMax = 0;
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Color",QVariant(g_showPicType));
    delete configIniStartUp;
}

void MainWindow::VersionColor()
{
    if(g_useMOD == 1 && g_modsupportColor != 255)
    {
        QMessageBox::about(this, "提示", "本MOD不支持彩色版");
        return;
    }
    g_showPicType = 2;//1;
    g_StoredMax = 0;
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Color",QVariant(g_showPicType));
    delete configIniStartUp;
}
void MainWindow::ExitGame()
{
    exit(0);
}

void MainWindow::OnlineHelp()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://3gby.skysidestudio.com/help/")));
}

void MainWindow::KeyHelp()
{
    //KeyHelpDialog dialog(this);
    //dialog.exec();
#if defined(Q_OS_ANDROID)
    QMessageBox::about(this, "按键说明", "实际按键控制以使用按键配置修改后的信息为准，以下为初始按键设置\n按键操作：\n方向键——移动 \nEnter（回车）——确认 \nEsc——退出、返回或菜单 \nSpace（空格）——战争界面可查看武将信息 \nHome——地图界面显示城市连接信息，战争界面显示当前战场情况 \n\n鼠标操作：\n左键点击选择或在菜单外关闭活动菜单\n滚轮滚动列表及菜单");
#else
    QMessageBox::about(this, "按键说明", "实际按键控制以使用按键配置修改后的信息为准，以下为初始按键设置\n按键操作：\n方向键——移动 \nEnter（回车）——确认 \nEsc——退出、返回或菜单 \nSpace（空格）——战争界面可查看武将信息 \nHome——地图界面显示城市连接信息，战争界面显示当前战场情况 \n\n鼠标操作：\n左键点击选择或在菜单外关闭活动菜单\n滚轮滚动列表及菜单");
#endif

}


void MainWindow::WhatsNew()
{
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    QProcess process;
    process.execute(tr("notepad.exe whatsnew.txt"));
#elif defined(Q_OS_ANDROID)

    //读取txt文本信息
    if(QFile::exists("whatsnew.txt"))
    {
        QFile::remove("whatsnew.txt");
    }
    QFile::copy("assets:/whatsnew.txt","whatsnew.txt");
    QFile file("whatsnew.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString allline = "";
    while (!in.atEnd())
    {
        QString line = in.readLine();
        allline +=line;
        allline +="\n";

    }

    allline+="";
    QMessageBox::about(this, "更新记录", allline);
#elif defined(Q_OS_IOS)

    //读取txt文本信息
    QString whatsnewpath = QDir::homePath() + "/Documents/whatsnew.txt";

    if(QFile::exists(whatsnewpath))
    {
        QFile::remove(whatsnewpath);
    }

    QFile::copy("whatsnew.txt",whatsnewpath);

    QFile file("whatsnew.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString allline = "";
    while (!in.atEnd())
    {
        QString line = in.readLine();
        allline +=line;
        allline +="\n";

    }

    allline+="";
    QMessageBox::about(this, "更新记录", allline);


#endif
}

void MainWindow::About()
{

#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    AboutDialog dialog(this);
    dialog.exec();
#elif defined(Q_OS_ANDROID)
    QString aboutstr;
    aboutstr.sprintf("天际边三国霸业 ver%d.%01d.%01d\n天际边工作室荣誉出品\n天际边工作室\nhttp://www.skysidestudio.com\n官方网站\nhttp://3gby.skysidestudio.com",version_now%1000/100,version_now%100/10,version_now%10);
    QMessageBox::about(this, "关于", aboutstr);
#elif defined(Q_OS_IOS)
    QString aboutstr;
    aboutstr.sprintf("天际边三国霸业 ver%d.%01d.%01d\n天际边工作室荣誉出品\n天际边工作室\nhttp://www.skysidestudio.com\n官方网站\nhttp://3gby.skysidestudio.com",version_now%1000/100,version_now%100/10,version_now%10);
    QMessageBox::about(this, "关于", aboutstr);
#endif
}

void MainWindow::ExportSave()
{
    QString dicName = QFileDialog::getExistingDirectory(this,tr("导出"),".");
    if (!dicName.isNull())
    {
        QFile::copy("sango0.sav", dicName+ "/" +"sango0.sav");
        QFile::copy("sango1.sav", dicName+ "/" +"sango1.sav");
        QFile::copy("sango2.sav", dicName+ "/" +"sango2.sav");
        QFile::copy("sango3.sav", dicName+ "/" +"sango3.sav");
        QFile::copy("sango4.sav", dicName+ "/" +"sango4.sav");
        QFile::copy("sango5.sav", dicName+ "/" +"sango5.sav");
        QMessageBox::about(this, "提示", "导出成功");
    }

}

void MainWindow::ImportSave()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("导入"),".","*.sav",0);
    if (!fileName.isNull())
    {
        if(fileName.contains("0"))
        {
            QFile::copy(fileName, "sango0.sav");
            QFile::copy(fileName, "sango1.sav");
        }
        else if(fileName.contains("1"))
        {
            QFile::copy(fileName, "sango0.sav");
            QFile::copy(fileName, "sango1.sav");
        }
        else if(fileName.contains("2"))
        {
            QFile::copy(fileName, "sango2.sav");
            QFile::copy(fileName, "sango3.sav");
        }
        else if(fileName.contains("3"))
        {
            QFile::copy(fileName, "sango2.sav");
            QFile::copy(fileName, "sango3.sav");
        }
        else if(fileName.contains("4"))
        {
            QFile::copy(fileName, "sango4.sav");
            QFile::copy(fileName, "sango5.sav");
        }
        else if(fileName.contains("5"))
        {
            QFile::copy(fileName, "sango4.sav");
            QFile::copy(fileName, "sango5.sav");
        }
    }
}

void MainWindow::CloseMod()
{
    g_useMOD = 0;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Mod/UseMod",QVariant(g_useMOD));
    delete configIniStartUp;

    g_startName = "dat.lib";
    g_startFlag = 2;

}

void MainWindow::OpenMod()
{
    QString fullPath;
#if defined(Q_OS_ANDROID)
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    fullPath = QString("%1/skyside3gby3/download/").arg(externalStorageDir.toString());
#elif defined(Q_OS_IOS)
    fullPath = QDir::homePath() + "/Documents/download/";
#else
    QDir dir1;
    fullPath = dir1.currentPath() + "/download/";
#endif

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    QTextCodec::setCodecForLocale(codec);

    QDir dir(fullPath);
    if(!dir.exists())
    {
        dir.mkdir(fullPath);
    }

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    //listWidget = new QListWidget;

    //QVBoxLayout* layout = new QVBoxLayout;
    QFileInfoList list = dir.entryInfoList();
    dateList.clear();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName().contains(".mod"))
        {
            dateList<<fileInfo.fileName();//.filePath();
        }
    }

    dateList <<"关闭";


    openlist = new QListView(this);
    QStringListModel* slm = new QStringListModel(this);

    QFont list_font;
    list_font.setPointSize(25);
    list_font.setBold(true);
    list_font.setWeight(50);

    slm->setStringList(dateList);
    openlist->setModel(slm);
    openlist->setSelectionMode(QAbstractItemView::SingleSelection);
    openlist->setAutoFillBackground(true);
    openlist->setFont(list_font);

    QTextCodec *codec1 = QTextCodec::codecForName("GB18030");//情况2
    QTextCodec::setCodecForLocale(codec1);

    connect(openlist,SIGNAL(clicked(const QModelIndex&)),this,SLOT(selectedOpenFile()));

    //QDockWidget *dock = new QDockWidget(tr("DockWindow1"),this);
    //dock->setFeatures(QDockWidget::DockWidgetFloatable); //指定停靠窗体的样式，此处为可移动
    //dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    setCentralWidget(openlist);
    //addDockWidget(Qt::AllDockWidgetAreas,dock);


}

void MainWindow:: GameMenu(QMenu *qmenu)
{
    QString fullPath;
#if defined(Q_OS_ANDROID)
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    fullPath = QString("%1/skyside3gby3/download/").arg(externalStorageDir.toString());
#elif defined(Q_OS_IOS)
    fullPath = QDir::homePath() + "/Documents/download/";
#else
    fullPath = "";
#endif

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    QTextCodec::setCodecForLocale(codec);

    QDir dir(fullPath);
    if(!dir.exists())
    {
        dir.mkdir(fullPath);
    }

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    //listWidget = new QListWidget;

    //QVBoxLayout* layout = new QVBoxLayout;
    QFileInfoList list = dir.entryInfoList();
    dateList.clear();

    QSignalMapper *signalMapper = new QSignalMapper(this);

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        dateList<<fileInfo.fileName();//.filePath();
        //listWidget->addItem(fileInfo.fileName());



        //QByteArray t2 = fileInfo.fileName().toUtf8();
        //char * name = t2.data();

        //QString fileNameUTF8 = QString::fromLocal8Bit(fileInfo.fileName().toUtf8());


        QAction *actionGame = new QAction(fileInfo.fileName(), this);
        //QAction *actionGame = new QAction(fileNameUTF8, this);
        connect(actionGame, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(actionGame, i);
        qmenu->addAction(actionGame);

    }

    QTextCodec *codec1 = QTextCodec::codecForName("GB18030");//情况2
    QTextCodec::setCodecForLocale(codec1);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(Game_Open(int)));
}


void MainWindow::Game_Open(int rowid)
{

    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    //QTextCodec::setCodecForLocale(codec);

    g_StoredMax = 0;//20181003
    QString fileName = dateList.at(rowid);
            //"dat.gam";//QFileDialog::getOpenFileName(this,tr("打开"),".","lib文件(*.lib);;gam文件(*.gam)",0);

    if(!fileName.isNull())
    {
        //PubOpenGame(fileName);
        g_startName = fileName;
        //g_startFlag = 1;


        QString downloadedFilePath = "";
        QByteArray fileNamearray;
        downloadedFilePath = "/download/";
        fileNamearray = (downloadedFilePath + g_startName).toUtf8();//toLocal8Bit();

        char * ftname = fileNamearray.data();
        memcpy(g_startName_char,ftname,200);

        g_useMOD = 1;
        QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
        configIniStartUp->setValue("/Mod/UseMod",QVariant(g_useMOD));
        delete configIniStartUp;

        g_startFlag = 3;

    }


}

void MainWindow::selectedOpenFile()
{
    QItemSelectionModel *selmodel = openlist->selectionModel();
    if (!selmodel)
    {
        takeCentralWidget();
        return;
    }

    QModelIndexList indexlist = selmodel->selectedIndexes();
    //int count1 = indexlist.size();
    int idx = indexlist.at(0).row();

    takeCentralWidget();

    int max = dateList.count();
    if(idx == max - 1)//关闭
    {

        return;
    }

    Game_Open(idx);



}

void MainWindow::AboutMod()
{

    QString aboutstr;
    aboutstr.sprintf("MOD加载请将.mod文件放至download文件夹\nMOD开发教程及工具请查看官网http://3gby.skysidestudio.com\nMOD发布请按官网联系方式联系wstjb");
    QMessageBox::about(this, "说明", aboutstr);

}

void MainWindow::DownloadMod()
{
#if defined(Q_OS_ANDROID)
    QString xmlPath = "ClentFile.xml";
#elif defined(Q_OS_IOS)
    QString xmlPath = QDir::homePath() + "/Documents/ClentFile.xml";
#else
    QString xmlPath = "ClentFile.xml";
#endif
    if(QFile::exists(xmlPath))
    {
        QFile::remove(xmlPath);
    }

    xmlfile = new QFile(xmlPath);

    xmlfile->open(QIODevice::WriteOnly);

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url("http://www.example.com/UpDate/3gbymod/ClentFile.xml");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    reply = accessManager->get(request);


    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(accessManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    // 获取响应信息
    QByteArray bytes = reply->readAll();
    //qDebug() << bytes;

    xmlfile->write(bytes);

    xmlfile->close();


    //xml解析

    rpgGameList.clear();

    if(!xmlfile->open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,tr("错误"),tr("文件读取异常"));
        return;
    }
    qDebug()<<QDir::currentPath();
    //将file设置为QXmlStreamReader的设备(Stream从file中读取内容)
    //Sets the current device to device.
    //Setting the device resets the stream to its initial state.
    reader.setDevice(xmlfile);
    //判断文件是否读到末尾
    while(!reader.atEnd())
    {
        //判断是否是开始元素
        if(reader.isStartElement())
        {
            //判断读取的根标签是否是bookindex
            if(reader.name() == "Config")
            {
                //是bookindex 往下读取
                readConfigElement();
            }
            else
            {
                reader.raiseError(tr("Not a valid Config"));
            }
        }
        else
        {
            reader.readNext();
        }
    }
    //文件记得关闭
    xmlfile->close();

    if(reader.hasError())
    {
        QMessageBox::information(this,tr("错误"),tr("请确认网络连接是否正常"));
        //QMessageBox::information(this,tr("Error"),tr("Filed to parse file %1").arg("fileName"));
        return ;
    }
    else if(xmlfile->error() != QFile::NoError)
    {
        QMessageBox::information(this,tr("错误"),tr("文件读取异常"));
        return ;
    }

    rpgGameList <<"关闭";

    //QStringList -> QListView
    list = new QListView(this);
    QStringListModel* slm = new QStringListModel(this);

    QFont list_font;
    list_font.setPointSize(25);
    list_font.setBold(true);
    list_font.setWeight(50);

    slm->setStringList(rpgGameList);
    list->setModel(slm);
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setAutoFillBackground(true);
    list->setFont(list_font);


    connect(list,SIGNAL(clicked(const QModelIndex&)),this,SLOT(selectedDownFile()));

    //QDockWidget *dock = new QDockWidget(tr("DockWindow1"),this);
    //dock->setFeatures(QDockWidget::DockWidgetFloatable); //指定停靠窗体的样式，此处为可移动
    //dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    setCentralWidget(list);
    //addDockWidget(Qt::AllDockWidgetAreas,dock);



    //this->addDockWidget(Qt::AllDockWidgetAreas,list);



    //rpgGameList <<"";
    //bool b = rpgGameList.contains("123");

}

void MainWindow::selectedDownFile()
{
    QItemSelectionModel *selmodel = list->selectionModel();
    if (!selmodel)
    {
        takeCentralWidget();
        return;
    }

    QModelIndexList indexlist = selmodel->selectedIndexes();
    //int count1 = indexlist.size();
    int idx = indexlist.at(0).row();
    QString txt = rpgGameList.at(idx);
    //qDebug()<<"selectedDwnfile"<<txt;
    takeCentralWidget();

    int max = rpgGameList.count();
    if(idx == max - 1)//关闭
    {

        return;
    }

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    QTextCodec::setCodecForLocale(codec);

    QString fullPath;
#if defined(Q_OS_ANDROID)
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    fullPath = QString("%1/skyside3gby3/download/").arg(externalStorageDir.toString());
#elif defined(Q_OS_IOS)
    fullPath = QDir::homePath() + "/Documents/download/";
#else
    QDir dir1;
    fullPath = dir1.currentPath() + "/download/";
#endif

    QDir dir(fullPath);
    if(!dir.exists())
    {
        dir.mkdir(fullPath);
    }

    if(QFile::exists(fullPath+txt))
    {
        //QMessageBox::about(NULL, "说明", "该文件已存在，请不要重复下载");
        if (QMessageBox::Ok == QMessageBox::question(this, "提示","该文件已存在，是否重新下载并替换？",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
        {
            QFile::remove(fullPath+txt);
        }
    }


    downloadfile = new QFile(fullPath+txt);

    downloadfile->open(QIODevice::WriteOnly);

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url("http://www.example.com/UpDate/3gbymod/"+txt);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    reply = accessManager->get(request);


    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(accessManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    // 获取响应信息
    QByteArray bytes = reply->readAll();
    //qDebug() << bytes;

    downloadfile->write(bytes);

    downloadfile->close();

    QTextCodec *codec1 = QTextCodec::codecForName("GB18030 ");//情况2
    QTextCodec::setCodecForLocale(codec1);

    QMessageBox::about(NULL, "提示", "该文件已下载成功");


}

void MainWindow::readConfigElement()
{
    //Q_ASSERT()加断言  若进函数的时候 StartElement状态，或者说标签不是 bookindex，就认为出错
    Q_ASSERT(reader.isStartElement() && reader.name()=="Config");
    //往下读取
    reader.readNext();
    while (!reader.atEnd())
    {
        //判断是结束节点，跳出
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if(reader.isStartElement())
        {
            //(bookindex的子元素为entry)开始处理entry
            if(reader.name() == "Files")
            {
                //读取entry节点，(隐藏根节点)
                readFilesElement();
            }
            else
            {
                //没读到entry节点 跳过
                skipUnknownElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
}

//接受一个QTreeWidgetItem指针作为根节点.这个节点被当做这个 entry 标签在QTreeWidget中的根节点。
void MainWindow::readFilesElement()
{

    reader.readNext();

    while (!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if(reader.isStartElement())
        {
            //读到entry节点
            if(reader.name() == "File")
            {
                readFileElement();
                //page节点
            }
            else
            {
                //无节点  跳过
                skipUnknownElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
}

void MainWindow::readFileElement()
{
    //获取文本
    QString gameName = reader.attributes().value("Name").toString();
    rpgGameList <<gameName;

    reader.readNext();
    if(reader.isEndElement())
    {
        reader.readNext();
    }


}

void MainWindow::skipUnknownElement()
{
    reader.readNext();
    //没有结束
    while (!reader.atEnd())
    {
        //结束
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        //继续跳过读取。
        if(reader.isStartElement())
        {
            skipUnknownElement();
        }
        else
        {
            reader.readNext();
        }
    }
}

void MainWindow::ImportOldSaves()
{
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "提示", "本功能用于导入原windows版ver1.00至ver1.70存档，原windows版存档位置位于游戏安装目录Res子文件夹下，存档名为save0或save1或save2.ssv,导入时默认将覆盖当前游戏进度，但不覆盖当前3个存档，是否进行导入操作", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString fileName = QFileDialog::getOpenFileName(this,tr("导入"),".","*.ssv",0);
        QByteArray fileNamearray = fileName.toLocal8Bit();
        char *fileName_char = fileNamearray.data();
        OldSaveFileRead(fileName_char);
    }
#elif defined(Q_OS_ANDROID)
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "提示", "本功能用于导入原android版ver1.00至ver1.70存档，原android版存档位置位于[SD卡的skyside3gby子文件夹]下，存档名为save0或save1或save2.ssv,导入时默认将覆盖当前游戏进度，但不覆盖当前3个存档，是否进行导入操作", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QMessageBox::StandardButton rb1 = QMessageBox::question(NULL, "提示", "是否导入原存档一，选择yes开始导入，选择cancel进行后续选择", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb1 == QMessageBox::Yes)
        {

            QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                            "android/os/Environment",
                            "getExternalStorageDirectory",
                            "()Ljava/io/File;"
                            );
            QString fileName = QString("%1/skyside3gby/save0.ssv").arg(externalStorageDir.toString());

            //QString fileName = QFileDialog::getOpenFileName(this,tr("导入"),".","*.ssv",0);
            QByteArray fileNamearray = fileName.toLocal8Bit();
            char *fileName_char = fileNamearray.data();
            OldSaveFileRead_A(fileName_char);
            return;
        }
        QMessageBox::StandardButton rb2 = QMessageBox::question(NULL, "提示", "是否导入原存档二，选择yes开始导入，选择cancel进行后续选择", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb1 == QMessageBox::Yes)
        {

            QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                            "android/os/Environment",
                            "getExternalStorageDirectory",
                            "()Ljava/io/File;"
                            );
            QString fileName = QString("%1/skyside3gby/save1.ssv").arg(externalStorageDir.toString());

            //QString fileName = QFileDialog::getOpenFileName(this,tr("导入"),".","*.ssv",0);
            QByteArray fileNamearray = fileName.toLocal8Bit();
            char *fileName_char = fileNamearray.data();
            OldSaveFileRead_A(fileName_char);
            return;
        }
        QMessageBox::StandardButton rb3 = QMessageBox::question(NULL, "提示", "是否导入原存档三，选择yes开始导入，选择cancel进行后续选择", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb1 == QMessageBox::Yes)
        {

            QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                            "android/os/Environment",
                            "getExternalStorageDirectory",
                            "()Ljava/io/File;"
                            );
            QString fileName = QString("%1/skyside3gby/save2.ssv").arg(externalStorageDir.toString());

            //QString fileName = QFileDialog::getOpenFileName(this,tr("导入"),".","*.ssv",0);
            QByteArray fileNamearray = fileName.toLocal8Bit();
            char *fileName_char = fileNamearray.data();
            OldSaveFileRead_A(fileName_char);
            return;
        }
    }
#elif defined(Q_OS_IOS)
    QMessageBox::about(this, "提示", "抱歉，本版不支持存档导入");
#endif
}

void MainWindow::HighAtt()
{
    if(g_HighAtt == 1)
    {
        g_HighAtt = 0;

#if defined(Q_OS_ANDROID)
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("取消高攻击概率成功");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
        //QMessageBox::about(this, "提示", "取消高攻击概率成功");
#else
        actionHighAtt->setChecked(false);
        QMessageBox::about(this, "提示", "取消高攻击概率成功");
#endif
    }
    else
    {
        g_HighAtt = 1;

#if defined(Q_OS_ANDROID)
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("设置高攻击概率成功");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
        //QMessageBox::about(this, "提示", "设置高攻击概率成功");
#else
        actionHighAtt->setChecked(true);
        QMessageBox::about(this, "提示", "设置高攻击概率成功");
#endif
    }

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_HighAtt == 1)
    {
        configIniStartUp->setValue("/Startup/HighAtt",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/HighAtt",QVariant(0));
    }

    delete configIniStartUp;
}

void MainWindow::UseDotFont()
{
    if(g_useDotFont == 1)
    {
        g_useDotFont = 0;

#if defined(Q_OS_ANDROID)
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("取消使用点阵字体成功");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
        //QMessageBox::about(this, "提示", "取消使用点阵字体成功");
#else
        QMessageBox::about(this, "提示", "取消使用点阵字体成功");
        actionUseDotFont->setChecked(false);
#endif
    }
    else
    {
        g_useDotFont = 1;

#if defined(Q_OS_ANDROID)
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("设置使用点阵字体成功");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
        //QMessageBox::about(this, "提示", "设置使用点阵字体成功");
#else
        QMessageBox::about(this, "提示", "设置使用点阵字体成功");
        actionUseDotFont->setChecked(true);
#endif
    }

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_useDotFont == 1)
    {
        configIniStartUp->setValue("/Startup/UseDotFont",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/UseDotFont",QVariant(0));
    }
    delete configIniStartUp;
}

void MainWindow::StatusBar()
{
#if defined(Q_OS_ANDROID)
    if(g_gameviewdirect == 0)//横屏
    {
        QAndroidJniObject javaToast = QAndroidJniObject::fromString("横屏禁止显示状态栏");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());

        return;
    }
    if(g_statusBar == 0)
    {

        g_statusBar = 1;
         g_screenheight -= 50;//暂时50
        QAndroidJniObject javaStatusBar = QAndroidJniObject::fromString(g_statusBar+"");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","statusBarAdd","(Ljava/lang/String;)V",javaStatusBar.object<jstring>());


    }
    else
    {
        g_statusBar = 0;
         g_screenheight += 50;//暂时50
        QAndroidJniObject javaStatusBar = QAndroidJniObject::fromString(g_statusBar+"");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","statusBar","(Ljava/lang/String;)V",javaStatusBar.object<jstring>());
    }




    this->setFixedSize(g_screenwidth,g_screenheight);


    g_gameviewtop = 0;//已去除状态栏
    g_gameviewbottom = g_screenwidth*3/5;

    g_screentimes = (double)g_screenwidth/160.0;


    g_Fontsize = 1;
    int heightOfBt = 16 *g_screentimes ;
    while(true)
    {
        QFont tmp(androidFont, g_Fontsize, QFont::Bold);
        tmp.setPointSize(g_Fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsHigh = fm.height();
        if (pixelsHigh == heightOfBt)
            break;

        if (pixelsHigh > heightOfBt)
        {
            --g_Fontsize;
            break;
        }

        ++g_Fontsize;
    }
    reflushflag = 1;
    g_reflushflag = 1;

#endif
}

void MainWindow::CheckUpdate()
{
#if defined(Q_OS_ANDROID)
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("1");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","checkUpdate","(Ljava/lang/String;)V",javaToast.object<jstring>());
#endif
}

void MainWindow::UploadSaves()
{


    if(g_player.logined == 1)
    {
        if(TCPisConnected()==OK)
        {

        }
        else
        {
            QMessageBox::about(NULL,"提示","登录状态异常，请重新登录");
            g_player.logined = 0;
            TCPdisconnectServer();
        }

        QString filename = "sango0.sav";
        QString filepath = "";
        char filedata[17872+1];

    #if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
        filepath = "";
    #elif defined(Q_OS_ANDROID)
        QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

        filepath = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) ;

    #elif defined(Q_OS_IOS)
        filepath = QDir::homePath() + "/Documents/";

    #endif

        int pos = 0;

        for(int i = 0 ; i < 6 ; i ++)//6个文件
        {
            filename = QString("sango")+QString::number(i)+QString(".sav");
            QFile *file = new QFile(filepath + filename);

            if(file->exists())
            {
                if(file->open(QIODevice::ReadOnly))
                {
                    QByteArray byte_file = file->readAll();

                    int byte_size = byte_file.length();

                    memcpy(filedata+pos,byte_file.data(),byte_size);
                    pos+=byte_size;

                    //报文头 文件名+长度
                    //tcpclient->writeData(byte_file);
                }
            }
            else if(i == 0 || i == 2 ||i == 4)
            {
                pos+=3245;
            }
            else if(i == 1 || i == 3 ||i == 5)
            {
                pos+=2708;
            }

        }


        UploadSave(filedata);

        C data[65535];
        if(ClientRcvMsg(TRANS_UPLOADSAV,data,1)>0)
        {

            if(data[0] == 0x00)
            {

                QMessageBox::about(NULL,"提示","存档上传成功");
            }
            else
            {
                QMessageBox::about(NULL,"提示","存档上传失败");
            }
        }
    }
    else
    {
        QMessageBox::about(NULL,"提示","请先登录");
    }


}

void MainWindow::DownloadSaves()
{
    //判断是否登录
    if(g_player.logined == 1)
    {
        if(TCPisConnected()==OK)
        {

        }
        else
        {
            QMessageBox::about(NULL,"提示","登录状态异常，请重新登录");
            g_player.logined = 0;
            TCPdisconnectServer();
        }

        DownloadSave();//ClientSndMsg(TRANS_SHAREHAND,data,0);
        C data[65535];
        if(ClientRcvMsg(TRANS_DOWNLOADSAV,data,1)>0)
        {
            if(data[0] == 0x00)
            {
                char UUID[32+1];
                memcpy(UUID,data+1,32);

                //存档数据转存
                QString filename = "sango0.sav";
                QString filepath = "";

            #if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
                filepath = "";
            #elif defined(Q_OS_ANDROID)
                QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                            "android/os/Environment",
                            "getExternalStorageDirectory",
                            "()Ljava/io/File;"
                            );

                filepath = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) ;

            #elif defined(Q_OS_IOS)
                filepath = QDir::homePath() + "/Documents/";

            #endif

                int pos = 1;

                for(int i = 0 ; i < 6 ; i ++)//6个文件
                {

                    filename = QString("sango")+QString::number(i)+QString(".sav");
                    QFile::remove(filepath+filename);
                    QFile *file = new QFile(filepath + filename);

                    file->open(QIODevice::WriteOnly);
                    if(i == 0 || i == 2 ||i == 4)
                    {
                        file->write((char *)(data+pos),3245);
                        pos+=3245;
                    }
                    else if(i == 1 || i == 3 ||i == 5)
                    {
                        file->write((char *)(data+pos),2708);
                        pos+=2708;
                    }
                    file->close();


                }






                QMessageBox::about(NULL,"提示","下载成功");
            }
            else
            {
                QMessageBox::about(NULL,"提示","下载失败");
            }
        }
    }
    else
    {
        QMessageBox::about(NULL,"提示","请先登录");
    }

}

void MainWindow::ManageSaves()
{
    QMessageBox::about(NULL,"提示","暂不开放");
    //QDesktopServices::openUrl(QUrl(QLatin1String("http://www.example.com/GameSave/3gby.html")));
}

void MainWindow::UserRegister()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://www.example.com/User/userRegister.html")));

}

void MainWindow::UserLogin()
{
    LoginDialog dialog(this);
    dialog.exec();
}

void MainWindow::UserLogout()
{
    if(TCPisConnected() == TCPERROR)
    {
        QMessageBox::about(NULL,"提示","您未登录,无法注销");
        return;
    }
    Logout();

    C data[65535];
    if(ClientRcvMsg(TRANS_LOGOUT,data,1)>0)
    {
        if(data[0] == 0x00)
        {
            QMessageBox::about(NULL,"提示","注销成功");
        }
        else
        {
            QMessageBox::about(NULL,"提示","注销完成");
        }
        g_player.logined = 0;
    }
    TCPdisconnectServer();
}

void MainWindow::DealBuff(char *c_rcvbuf,QString path)
{

    char fileName0[20];
    char fileName1[20];
    char fileName2[20];
    char fileBuff[100000];
    int pos = 22;

    int fileLen[6];

    memset(fileName0,0,sizeof(fileName0));
    memset(fileName1,0,sizeof(fileName1));
    memset(fileName2,0,sizeof(fileName2));

    for(int i = 0 ; i < 6 ;i ++)
    {
        memset(fileBuff,0,sizeof(fileBuff));

        memcpy(&fileLen[i],c_rcvbuf+pos,4);
        pos+=4;
        //ui->receiveList->addItem("fileLen"+QString::number(fileLen[i])) ;
        memcpy(fileBuff,c_rcvbuf+pos,fileLen[i]);
        pos+=fileLen[i];

        //buff = obj->readAll();//文件名
        QString fileName = QString("sango")+QString::number(i)+QString(".sav");
        QFile *file = new QFile(path+"\\"+fileName);
        if(!file->open(QIODevice::WriteOnly))
        {
          //qDebug()<<"打开失败";
        }

        file->write(fileBuff,fileLen[i]);
        file->close();


    }


}

MainWindow::~MainWindow()
{
    myThread.stop();
    myThread.terminate();
}

QPainter* MainWindow::getPainter()
{
    return mPainter;
}

QPainter* MainWindow::getPainterStr()
{
    return mPainterStr;//用于str缓存显示
}

QPainter* MainWindow::getPainterMem()
{
    return mPainterMem;
}

void MainWindow::copyFromDcToStr()
{
    mPainterStr->drawPixmap(0,0,*mPixmap);
}
void MainWindow::copyToDcStr()
{
    //mPixmapMem->save("2.bmp","BMP");
    //* mPixmap = mPixmapMem->copy();
    mPainter->drawPixmap(0,0,*mPixmapStr);
}

void MainWindow::copyToDc()
{
    //mPixmapMem->save("2.bmp","BMP");
    //* mPixmap = mPixmapMem->copy();
    mPainter->drawPixmap(0,0,*mPixmapMem);
}

void MainWindow::SetColor()
{
    if(g_showPicType == 2)//1)
    {
        g_showPicType = 0;
        g_StoredMax = 0;
    }
    else
    {
        g_showPicType = 2;//1;
        g_StoredMax = 0;
    }
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Color",QVariant(g_showPicType));
    delete configIniStartUp;
}

void MainWindow::SetDirection()
{
    if(g_gameviewdirect == 1)
    {
        g_gameviewdirect = 0;

    }
    else
    {
        g_gameviewdirect = 1;

    }

#if defined(Q_OS_ANDROID)
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>( "getRequestedOrientation" );
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }


    activity.callMethod<void>("setRequestedOrientation", "(I)V", g_gameviewdirect);
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }
#elif defined(Q_OS_IOS)
    routeView(g_gameviewdirect);
#endif

    int tmp1 = g_screenwidth;
    g_screenwidth = g_screenheight;
    g_screenheight = tmp1;
    this->setFixedSize(g_screenwidth,g_screenheight);

    if(g_gameviewdirect == 0)//横屏
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
        g_screentimes = (double)t_width/160.0;
#elif defined(Q_OS_ANDROID)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

        /*g_gameviewleft = (g_screenwidth - g_screenheight*5/3 )/2;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop =0;
        g_gameviewbottom = t_width/5*3;//g_screenheight;*/
#elif defined(Q_OS_IOS)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

#endif
    }
    else if (g_gameviewdirect == 1)//竖屏 默认
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)//windows不存在竖屏。如果有 按横屏处理
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
#elif defined(Q_OS_ANDROID)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 0;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#elif defined(Q_OS_IOS)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 30;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#endif
    }


    g_Fontsize = 1;
    int heightOfBt = 14 *g_screentimes ;
    while(true)
    {
        QFont tmp(androidFont, g_Fontsize, QFont::Bold);
        tmp.setPointSize(g_Fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsHigh = fm.height();
        if (pixelsHigh == heightOfBt)
            break;

        if (pixelsHigh > heightOfBt)
        {
            --g_Fontsize;
            break;
        }

        ++g_Fontsize;
    }
    reflushflag = 1;
    g_reflushflag = 1;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/ShowWay",QVariant(g_gameviewdirect));
    delete configIniStartUp;

}

void MainWindow::SetDirectionA()
{
    if(g_gameviewdirect == 1)return;
    g_gameviewdirect = 1;


#if defined(Q_OS_ANDROID)
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>( "getRequestedOrientation" );
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }


    activity.callMethod<void>("setRequestedOrientation", "(I)V", g_gameviewdirect);
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }
#elif defined(Q_OS_IOS)
    routeView(g_gameviewdirect);
#endif

    int tmp1 = g_screenwidth;
    g_screenwidth = g_screenheight;
    g_screenheight = tmp1;
    this->setFixedSize(g_screenwidth,g_screenheight);

    if(g_gameviewdirect == 0)//横屏
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
        g_screentimes = (double)t_width/160.0;
#elif defined(Q_OS_ANDROID)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

        /*g_gameviewleft = (g_screenwidth - g_screenheight*5/3 )/2;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop =0;
        g_gameviewbottom = t_width/5*3;//g_screenheight;*/
#elif defined(Q_OS_IOS)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

#endif
    }
    else if (g_gameviewdirect == 1)//竖屏 默认
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)//windows不存在竖屏。如果有 按横屏处理
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
#elif defined(Q_OS_ANDROID)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 0;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#elif defined(Q_OS_IOS)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 30;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#endif
    }

    g_Fontsize = 1;
    int heightOfBt = 14 *g_screentimes ;
    while(true)
    {
        QFont tmp(androidFont, g_Fontsize, QFont::Bold);
        tmp.setPointSize(g_Fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsHigh = fm.height();
        if (pixelsHigh == heightOfBt)
            break;

        if (pixelsHigh > heightOfBt)
        {
            --g_Fontsize;
            break;
        }

        ++g_Fontsize;
    }
    reflushflag = 1;
    g_reflushflag = 1;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/ShowWay",QVariant(g_gameviewdirect));
    delete configIniStartUp;

}

void MainWindow::SetDirectionB()
{
    if(g_gameviewdirect == 0)return;
    g_gameviewdirect = 0;


#if defined(Q_OS_ANDROID)
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>( "getRequestedOrientation" );
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }


    activity.callMethod<void>("setRequestedOrientation", "(I)V", g_gameviewdirect);
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }
#elif defined(Q_OS_IOS)
    routeView(g_gameviewdirect);
#endif

    int tmp1 = g_screenwidth;
    g_screenwidth = g_screenheight;
    g_screenheight = tmp1;
    this->setFixedSize(g_screenwidth,g_screenheight);

    if(g_gameviewdirect == 0)//横屏
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
        g_screentimes = (double)t_width/160.0;
#elif defined(Q_OS_ANDROID)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

        /*g_gameviewleft = (g_screenwidth - g_screenheight*5/3 )/2;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop =0;
        g_gameviewbottom = t_width/5*3;//g_screenheight;*/
#elif defined(Q_OS_IOS)
        //先划分区域比例为1：6：1
        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;
        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

#endif
    }
    else if (g_gameviewdirect == 1)//竖屏 默认
    {
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)//windows不存在竖屏。如果有 按横屏处理
        g_gameviewleft = 0;
        g_gameviewtop = 50;
        g_gameviewright = g_screenwidth;
        int t_width = g_screenwidth;
        g_gameviewbottom = t_width/5*3+50;//g_screenheight;
        g_screentimes = (double)g_screenwidth/160.0;
#elif defined(Q_OS_ANDROID)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 0;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#elif defined(Q_OS_IOS)
        g_gameviewleft = 0;
        g_gameviewright = g_screenwidth;
        g_gameviewtop = 30;//已去除状态栏
        g_gameviewbottom = g_screenwidth*3/5;

        g_screentimes = (double)g_screenwidth/160.0;
#endif
    }

    g_Fontsize = 1;
    int heightOfBt = 14 *g_screentimes ;
    while(true)
    {
        QFont tmp(androidFont, g_Fontsize, QFont::Bold);
        tmp.setPointSize(g_Fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsHigh = fm.height();
        if (pixelsHigh == heightOfBt)
            break;

        if (pixelsHigh > heightOfBt)
        {
            --g_Fontsize;
            break;
        }

        ++g_Fontsize;
    }
    reflushflag = 1;
    g_reflushflag = 1;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/ShowWay",QVariant(g_gameviewdirect));
    delete configIniStartUp;

}


void MainWindow::SetFontSize()
{




    g_Fontsize = 1;
    int heightOfBt = 14 *g_screentimes ;
    while(true)
    {
        QFont tmp(androidFont, g_Fontsize, QFont::Bold);
        tmp.setPointSize(g_Fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsHigh = fm.height();
        if (pixelsHigh == heightOfBt)
            break;

        if (pixelsHigh > heightOfBt)
        {
            --g_Fontsize;
            break;
        }

        ++g_Fontsize;
    }
}

void MainWindow::CanTouch()
{
    if(g_touchEnable == 0)
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作开启");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("触屏操作开启");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "触屏操作开启");
    //actionAllowUpdate->setChecked(true);
#endif
        g_touchEnable = 1;

    }
    else
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作关闭");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("触屏操作关闭");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "触屏操作关闭");
    //actionAllowUpdate->setChecked(false);
#endif
        g_touchEnable = 0;
    }

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_touchEnable == 1)
    {
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(0));
    }
    delete configIniStartUp;
}


void MainWindow::LayerKey()
{
    if(g_layerKey == 0)
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作开启");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("浮层键盘开启,触屏功能将自动关闭");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "浮层键盘开启,触屏功能将自动关闭");
    //actionAllowUpdate->setChecked(true);
#endif
        g_layerKey = 1;
        g_touchEnable = 0;
        g_fullScreen = 0;

        int t_widthm8 = g_screenwidth/8;
        g_gameviewlefta = t_widthm8;

        if(g_gameviewlefta < 100) g_gameviewlefta = 100;
        //g_gameviewlefta += (screen * 50);//20180713
        t_widthm8 = g_gameviewlefta;
        g_gameviewrighta = g_screenwidth - g_gameviewlefta;
        int t_widtha = g_screenwidth - 2 * g_gameviewlefta;

        int t_heighta = t_widtha/5*3;
        while(t_heighta>g_screenheight)
        {
            g_gameviewlefta+=10;
            g_gameviewrighta-=10;
            t_widtha = g_screenwidth - 2 * g_gameviewlefta;
            t_heighta = t_widtha/5*3;
        }





        g_gameviewleft =0;
        //g_viewadd ++;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;

        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();




    }
    else
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作关闭");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("浮层键盘关闭");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "浮层键盘关闭");
    //actionAllowUpdate->setChecked(false);
#endif
        g_layerKey = 0;

        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;

        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

        SetFontSize();


    }
    g_reflushflag = 1;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_layerKey == 1)
    {
        configIniStartUp->setValue("/Startup/LayerKey",QVariant(1));
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(0));
        configIniStartUp->setValue("/Startup/FullScreen",QVariant(0));
    }
    else
    {
        configIniStartUp->setValue("/Startup/LayerKey",QVariant(0));
    }
    delete configIniStartUp;
}

void MainWindow::FullScreen()
{
    if(g_fullScreen == 0)
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作开启");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("全面触屏开启");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "全面触屏开启");
    //actionAllowUpdate->setChecked(true);
#endif
        g_fullScreen = 1;
        g_touchEnable = 1;
        g_layerKey = 0;
        reflushflag = 1;
        g_gameviewleft =0;
        //g_viewadd ++;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;

        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();




    }
    else
    {
#if defined(Q_OS_ANDROID)
    //QMessageBox::about(this, "说明", "触屏操作关闭");
    QAndroidJniObject javaToast = QAndroidJniObject::fromString("全面触屏关闭");
    QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
#else
    QMessageBox::about(this, "说明", "全面触屏关闭");
    //actionAllowUpdate->setChecked(false);
#endif
        g_fullScreen = 0;

        reflushflag = 1;
        g_viewadd = 0;

        int t_widthm8 = g_screenwidth/8;
        g_gameviewleft = t_widthm8;

        if(g_gameviewleft < 100) g_gameviewleft = 100;
        t_widthm8 = g_gameviewleft;
        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;

        int t_height = t_width/5*3;
        while(t_height>g_screenheight)
        {
            g_gameviewleft+=10;
            g_gameviewright-=10;
            t_width = g_screenwidth - 2 * g_gameviewleft;
            t_height = t_width/5*3;
        }

        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;
        g_screentimes = (double)t_width/160.0;

        SetFontSize();
    }

    g_reflushflag = 1;

    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    if(g_fullScreen == 1)
    {
        configIniStartUp->setValue("/Startup/FullScreen",QVariant(1));
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(1));
        configIniStartUp->setValue("/Startup/LayerKey",QVariant(0));
    }
    else
    {
        configIniStartUp->setValue("/Startup/FullScreen",QVariant(0));
    }
    delete configIniStartUp;
}
void MainWindow::LargerKey()
{
#if defined(Q_OS_ANDROID)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft +=50;
        g_viewadd ++;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#elif defined(Q_OS_IOS)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft +=50;
        g_viewadd ++;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#endif
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Screen",QVariant(g_viewadd));//screen 在windows版控制全屏 a及I控制屏幕大小比例缩放
    delete configIniStartUp;
}

void MainWindow::SmallerKey()
{
#if defined(Q_OS_ANDROID)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft -=50;
        g_viewadd --;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#elif defined(Q_OS_IOS)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft -=50;
        g_viewadd --;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#endif
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Screen",QVariant(g_viewadd));//screen 在windows版控制全屏 a及I控制屏幕大小比例缩放
    delete configIniStartUp;
}

void MainWindow::ScaleScreen(int scale)
{
#if defined(Q_OS_ANDROID)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft +=scale;
        if(g_gameviewleft<0)g_gameviewleft = 0;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = (g_screenheight - t_height)/2;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#elif defined(Q_OS_IOS)
    if(g_gameviewdirect == 0)//横屏
    {
        reflushflag = 1;
        g_gameviewleft +=scale;
        if(g_gameviewleft<0)g_gameviewleft = 0;

        g_gameviewright = g_screenwidth - g_gameviewleft;
        int t_width = g_screenwidth - 2 * g_gameviewleft;
        int t_height = t_width/5*3;
        g_screentimes = (double)t_width/160.0;
        g_gameviewtop = 30;
        g_gameviewbottom = g_gameviewtop + t_height;

        SetFontSize();
    }
#endif
    g_reflushflag = 1;
    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    configIniStartUp->setValue("/Startup/Screen",QVariant(g_viewadd));//screen 在windows版控制全屏 a及I控制屏幕大小比例缩放
    delete configIniStartUp;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    if(event->size().width() > mPixmap->width() || event->size().height() > mPixmap->height())
    {
        QPixmap* pixmap = new QPixmap((std::max)(event->size().width(), mPixmap->width()), (std::max)(event->size().height(), mPixmap->height()));
        delete mPainter;
        mPainter = new QPainter(pixmap);
        mPainter->drawPixmap(0, 0, *mPixmap);
        delete mPixmap;
        mPixmap = pixmap;

        QPixmap* pixmap1 = new QPixmap((std::max)(event->size().width(), mPixmap->width()), (std::max)(event->size().height(), mPixmap->height()));
        delete mPainterMem;
        mPainterMem = new QPainter(pixmap1);
        mPainterMem->drawPixmap(0, 0, *mPixmapMem);
        delete mPixmapMem;
        mPixmapMem = pixmap1;

        QPixmap* pixmap2 = new QPixmap((std::max)(event->size().width(), mPixmap->width()), (std::max)(event->size().height(), mPixmap->height()));
        delete mPainterStr;
        mPainterStr = new QPainter(pixmap2);
        mPainterStr->drawPixmap(0, 0, *mPixmapStr);
        delete mPixmapStr;
        mPixmapStr = pixmap2;

        delete g_cityMap;
        delete g_fightMap;
        g_cityMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);
        g_fightMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);


    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    //QDateTime current_date_time = QDateTime::currentDateTime();
    //QString current_date = current_date_time.toString("yyyyMMddhhmmsszzz");

    //mPixmap->save("1\\"+current_date+".bmp","BMP");
    painter.drawPixmap(g_gameviewleft,g_gameviewtop, *mPixmap);

#elif defined(Q_OS_ANDROID)


    //if(reflushflag == 1)
    {
        painter.fillRect(QRect(0,0,g_screenwidth,g_screenheight),QBrush(g_backColor));
    }
    painter.drawPixmap(g_gameviewleft,g_gameviewtop, *mPixmap);



    if(g_gameviewdirect == 0 )//横屏
    {
        int t_height = g_screenheight;

        if(g_layerKey == 1)//20180810
        {




            //painter.drawRect(QRect(0,g_gameviewbottom,g_gameviewright,g_screenheight));
            painter.drawImage(QRect(0,0,g_gameviewlefta,t_height/5),QImage(":/images/btn_pgup2.png"));
            painter.drawImage(QRect(0,t_height/5,g_gameviewlefta,t_height/5),QImage(":/images/btn_up2.png"));
            painter.drawImage(QRect(0,t_height*2/5,g_gameviewlefta/2,t_height/5),QImage(":/images/btn_left2.png"));
            painter.drawImage(QRect(g_gameviewlefta/2,t_height*2/5,g_gameviewlefta/2,t_height/5),QImage(":/images/btn_right2.png"));
            painter.drawImage(QRect(0,t_height*3/5,g_gameviewlefta,t_height/5),QImage(":/images/btn_down2.png"));
            painter.drawImage(QRect(0,t_height*4/5,g_gameviewlefta,t_height/5),QImage(":/images/btn_pgdn2.png"));
            painter.drawImage(QRect(g_gameviewrighta,0,g_gameviewlefta,t_height/4),QImage(":/images/btn_map2.png"));
            painter.drawImage(QRect(g_gameviewrighta,t_height/4,g_gameviewlefta,t_height/4),QImage(":/images/btn_find2.png"));
            painter.drawImage(QRect(g_gameviewrighta,t_height*2/4,g_gameviewlefta,t_height/4),QImage(":/images/btn_cancel2.png"));
            painter.drawImage(QRect(g_gameviewrighta,t_height*3/4,g_gameviewlefta,t_height/4),QImage(":/images/btn_confirm2.png"));
        }
        else if(g_fullScreen == 1)//20180810
        {
            int tmp = 50/5* g_screentimes ;
            painter.drawImage(QRect(0,0,g_screenwidth,tmp),QImage(":/images/btn_up3.png"));
            painter.drawImage(QRect(0,g_screenheight-tmp,g_screenwidth,tmp),QImage(":/images/btn_down3.png"));
            painter.drawImage(QRect(0,0,tmp,g_screenheight),QImage(":/images/btn_left3.png"));
            painter.drawImage(QRect(g_screenwidth-tmp,0,tmp,g_screenheight),QImage(":/images/btn_right3.png"));

        }
        else
        {
            //painter.drawRect(QRect(0,g_gameviewbottom,g_gameviewright,g_screenheight));
            painter.drawImage(QRect(0,0,g_gameviewleft,t_height/5),QImage(":/images/btn_pgup1.png"));
            painter.drawImage(QRect(0,t_height/5,g_gameviewleft,t_height/5),QImage(":/images/btn_up1.png"));
            painter.drawImage(QRect(0,t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_left1.png"));
            painter.drawImage(QRect(g_gameviewleft/2,t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_right1.png"));
            painter.drawImage(QRect(0,t_height*3/5,g_gameviewleft,t_height/5),QImage(":/images/btn_down1.png"));
            painter.drawImage(QRect(0,t_height*4/5,g_gameviewleft,t_height/5),QImage(":/images/btn_pgdn1.png"));
            painter.drawImage(QRect(g_gameviewright,0,g_gameviewleft,t_height/4),QImage(":/images/btn_map1.png"));
            painter.drawImage(QRect(g_gameviewright,t_height/4,g_gameviewleft,t_height/4),QImage(":/images/btn_find1.png"));
            painter.drawImage(QRect(g_gameviewright,t_height*2/4,g_gameviewleft,t_height/4),QImage(":/images/btn_cancel1.png"));
            painter.drawImage(QRect(g_gameviewright,t_height*3/4,g_gameviewleft,t_height/4),QImage(":/images/btn_confirm1.png"));
        }
    }
    else
    {
        int t_height = g_gameviewbottom - g_gameviewtop;
        int t_unitwidth = g_screenwidth/6;
        int t_unitheight = t_height/4;
        int t_startx = 0;
        int t_starty = (g_screenheight - 50) - t_height;//-100去除状态栏影响
        if(t_starty < g_gameviewbottom)
        {
            t_starty = g_gameviewbottom;
            t_height = (g_screenheight - 50) - g_gameviewbottom;
            t_unitheight = t_height/4;
        }

        painter.drawImage(QRect(t_startx+t_unitwidth*2,t_starty+t_unitheight*1,t_unitwidth,t_unitheight),QImage(":/images/btn_up1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*1,t_starty+t_unitheight*2,t_unitwidth,t_unitheight),QImage(":/images/btn_left1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*3,t_starty+t_unitheight*2,t_unitwidth,t_unitheight),QImage(":/images/btn_right1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*1,t_unitwidth,t_unitheight),QImage(":/images/btn_pgup1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_pgdn1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*2,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_down1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*5,t_starty+t_unitheight*0,t_unitwidth,t_unitheight),QImage(":/images/btn_map1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*0,t_unitwidth,t_unitheight),QImage(":/images/btn_find1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*0,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_cancel1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*5,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_confirm1.png"));
    }

    if(reflushflag == 1)
    {
        reflushflag = 0;

        delete g_cityMap;
        delete g_fightMap;
        g_cityMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);
        g_fightMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);

        mPixmapStr->fill(g_backColor);
        mPixmapMem->fill(g_backColor);
        //painter.fillRect(QRect(0,0,12800,12800),QBrush(g_backColor));
        SendMessage(99);
    }

#elif defined(Q_OS_IOS)


    painter.drawPixmap(g_gameviewleft,g_gameviewtop, *mPixmap);




    if(g_gameviewdirect == 0 )//横屏
    {
        int t_height = g_screenheight - 50;
        if(g_layerKey == 1)//20180810
        {
            //painter.drawRect(QRect(0,g_gameviewbottom,g_gameviewright,g_screenheight));
            painter.drawImage(QRect(0,g_gameviewtop,g_gameviewleft,t_height/5),QImage(":/images/btn_pgup2.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height/5,g_gameviewleft,t_height/5),QImage(":/images/btn_up2.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_left2.png"));
            painter.drawImage(QRect(g_gameviewleft/2,g_gameviewtop+t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_right2.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*3/5,g_gameviewleft,t_height/5),QImage(":/images/btn_down2.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*4/5,g_gameviewleft,t_height/5),QImage(":/images/btn_pgdn2.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop,g_gameviewleft,t_height/4),QImage(":/images/btn_map2.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height/4,g_gameviewleft,t_height/4),QImage(":/images/btn_find2.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height*2/4,g_gameviewleft,t_height/4),QImage(":/images/btn_cancel2.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height*3/4,g_gameviewleft,t_height/4),QImage(":/images/btn_confirm2.png"));
        }
        else if(g_fullScreen == 1)//20180810
        {

        }
        else
        {
            //painter.drawRect(QRect(0,g_gameviewbottom,g_gameviewright,g_screenheight));
            painter.drawImage(QRect(0,g_gameviewtop,g_gameviewleft,t_height/5),QImage(":/images/btn_pgup1.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height/5,g_gameviewleft,t_height/5),QImage(":/images/btn_up1.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_left1.png"));
            painter.drawImage(QRect(g_gameviewleft/2,g_gameviewtop+t_height*2/5,g_gameviewleft/2,t_height/5),QImage(":/images/btn_right1.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*3/5,g_gameviewleft,t_height/5),QImage(":/images/btn_down1.png"));
            painter.drawImage(QRect(0,g_gameviewtop+t_height*4/5,g_gameviewleft,t_height/5),QImage(":/images/btn_pgdn1.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop,g_gameviewleft,t_height/4),QImage(":/images/btn_map1.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height/4,g_gameviewleft,t_height/4),QImage(":/images/btn_find1.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height*2/4,g_gameviewleft,t_height/4),QImage(":/images/btn_cancel1.png"));
            painter.drawImage(QRect(g_gameviewright,g_gameviewtop+t_height*3/4,g_gameviewleft,t_height/4),QImage(":/images/btn_confirm1.png"));
        }
    }
    else
    {
        int t_height = g_gameviewbottom - g_gameviewtop;
        int t_unitwidth = g_screenwidth/6;
        int t_unitheight = t_height/4;
        int t_startx = 0;
        int t_starty = (g_screenheight - 50) - t_height;//-100去除状态栏影响
        if(t_starty < g_gameviewbottom)
        {
            t_starty = g_gameviewbottom;
            t_height = (g_screenheight - 50) - g_gameviewbottom;
            t_unitheight = t_height/4;
        }

        painter.drawImage(QRect(t_startx+t_unitwidth*2,t_starty+t_unitheight*1,t_unitwidth,t_unitheight),QImage(":/images/btn_up1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*1,t_starty+t_unitheight*2,t_unitwidth,t_unitheight),QImage(":/images/btn_left1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*3,t_starty+t_unitheight*2,t_unitwidth,t_unitheight),QImage(":/images/btn_right1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*1,t_unitwidth,t_unitheight),QImage(":/images/btn_pgup1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_pgdn1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*2,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_down1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*5,t_starty+t_unitheight*0,t_unitwidth,t_unitheight),QImage(":/images/btn_map1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*4,t_starty+t_unitheight*0,t_unitwidth,t_unitheight),QImage(":/images/btn_find1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*0,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_cancel1.png"));
        painter.drawImage(QRect(t_startx+t_unitwidth*5,t_starty+t_unitheight*3,t_unitwidth,t_unitheight),QImage(":/images/btn_confirm1.png"));
    }

    if(reflushflag == 1)
    {
        reflushflag = 0;

        delete g_cityMap;
        delete g_fightMap;
        g_cityMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);
        g_fightMap = new QImage(32*16*g_screentimes,32*16*g_screentimes,QImage::Format_RGB888);

        mPixmapStr->fill(g_backColor);
        mPixmapMem->fill(g_backColor);
        //painter.fillRect(QRect(0,0,g_screenwidth,g_screenheight),QBrush(g_backColor));
        SendMessage(99);
    }
#endif

    //g_gameviewleft = 100;
    //g_gameviewright = 700;
    //g_gameviewtop = 0;
    //g_gameviewbottom = 480;

}


bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool MainWindow::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pan = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pan));
    else if (QGesture *pan = event->gesture(Qt::TapAndHoldGesture))
        tapAndHoldTriggered(static_cast<QTapAndHoldGesture *>(pan));
    return true;
}

void MainWindow::tapAndHoldTriggered(QTapAndHoldGesture *gesture)
{
#if defined(Q_OS_ANDROID)
    if(g_fullScreen == 1)
    {
        OpenAndroidMenu();
    }
#endif
}


void MainWindow::pinchTriggered(QPinchGesture *gesture)
{
    if(g_fullScreen == 1)
    {
        FullScreen();
        return;
    }
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
    {
        qreal value = gesture->scaleFactor();
        if(fabs(value-1.0)>0.05)
        {

            if(value>1.0)
            {
                ScaleScreen(-5);
            }
            else
            {
                ScaleScreen(5);
            }
        }


    }


}

void  MainWindow::SendMessage(int KeyCode)
{
    if(g_keyCode == 0)
    {
        g_keyCode = KeyCode;
    }
    else
    {
        g_keyCodeList[g_keyCodeListMax] = KeyCode;
        g_keyCodeListMax ++;
    }

    /*while(g_keyCode)
    {

    }*/
    //update();
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    this->setFocus();
    int keyasciia[10];

    QSettings *configIniStartUp = new QSettings("startup.ini", QSettings::IniFormat);
    configIniStartUp->setIniCodec("GB2312");

    keyasciia[0]=configIniStartUp->value("/Key/Return",13).toInt();
    keyasciia[2]=configIniStartUp->value("/Key/Esc",27).toInt();
    keyasciia[1]=configIniStartUp->value("/Key/Space",32).toInt();
    keyasciia[3]=configIniStartUp->value("/Key/Home",36).toInt();
    keyasciia[4]=configIniStartUp->value("/Key/Up",38).toInt();
    keyasciia[7]=configIniStartUp->value("/Key/Down",40).toInt();
    keyasciia[5]=configIniStartUp->value("/Key/Left",37).toInt();
    keyasciia[6]=configIniStartUp->value("/Key/Right",39).toInt();
    keyasciia[8]=configIniStartUp->value("/Key/PgUp",73).toInt();
    keyasciia[9]=configIniStartUp->value("/Key/PgDn",81).toInt();

    delete configIniStartUp;

    if(event->key() == keyasciia[4])
    {
        SendMessage(VK_UP);
    }
    if(event->key() == keyasciia[7])
    {
        SendMessage(VK_DOWN);
    }
    if(event->key() == keyasciia[5])
    {
        SendMessage(VK_LEFT);
    }
    if(event->key() == keyasciia[6])
    {
        SendMessage(VK_RIGHT);
    }
    if(event->key() == keyasciia[0])
    {
        SendMessage(VK_ENTER);
    }
    if(event->key() == keyasciia[2])
    {
        SendMessage(VK_EXIT);
    }
    if(event->key() == keyasciia[8])
    {
        SendMessage(VK_PGUP);
    }
    if(event->key() == keyasciia[9])
    {
        SendMessage(VK_PGDN);
    }
    if(event->key() == keyasciia[1])
    {
        SendMessage(VK_HELP);
    }
    if(event->key() == keyasciia[3])
    {
        SendMessage(VK_SEARCH);
    }

#if defined(Q_OS_ANDROID)
    switch (event->key())
    {
    /*case Qt::Key_Up:
        SendMessage(VK_UP);
        break;
    case Qt::Key_Down:
        SendMessage(VK_DOWN);
        break;
    case Qt::Key_Left:
        SendMessage(VK_LEFT);
        break;
    case Qt::Key_Right:
        SendMessage(VK_RIGHT);
        break;
    case Qt::Key_Return:
        SendMessage(VK_ENTER);
        break;
    case Qt::Key_Escape:
        SendMessage(VK_EXIT);
        break;
    case Qt::Key_PageUp:
        SendMessage(VK_PGUP);
        break;
    case Qt::Key_PageDown:
        SendMessage(VK_PGDN);
        break;
    case Qt::Key_Space:
        SendMessage(VK_HELP);
        break;
    case Qt::Key_Home:
        SendMessage(VK_SEARCH);
        break;*/

    case Qt::Key_Menu:
    {
        OpenAndroidMenu();
        break;
    }
    case Qt::Key_Back:
    {
        QAndroidJniObject javaExit = QAndroidJniObject::fromString("text");
        QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity",
                                                  "exitApp",
                                                  "(Ljava/lang/String;)V",
                                                  javaExit.object<jstring>());
        break;
    }

    }
    #endif
    /*while(g_keyCode)
    {

    }*/
    //update();

}

void MainWindow::OpenAndroidMenu()
{


    //QAndroidJniObject activity = androidActivity();
    //activity.callMethod<void>("openOptionsMenu");

    //QAndroidJniObject javaToast = QAndroidJniObject::fromString("菜单暂时无法使用，请在开始界面点击HOME切换彩色黑白版，点击空格切换横竖屏");
    //QAndroidJniObject::callStaticMethod<void>("com/skysidestudio/skyside3gby/MainActivity","makeToast","(Ljava/lang/String;)V",javaToast.object<jstring>());
    QMenu *menu1  = new QMenu();
    QMenu *gameMenu = menu1->addMenu(tr("文件"));
    //QAction *actionExportSave = new QAction("本地存档导出", this);
    //connect(actionExportSave, SIGNAL(triggered()), this, SLOT(ExportSave()));
    //gameMenu->addAction(actionExportSave);

    //QAction *actionImportSave = new QAction("本地存档导入", this);
    //connect(actionImportSave, SIGNAL(triggered()), this, SLOT(ImportSave()));
    //gameMenu->addAction(actionImportSave);

    QAction *actionImportOldSaves = new QAction("旧版本存档导入", this);
    connect(actionImportOldSaves, SIGNAL(triggered()), this, SLOT(ImportOldSaves()));
    gameMenu->addAction(actionImportOldSaves);

    QMenu *gameMenuMod  = gameMenu->addMenu(tr("MOD"));
    actionOpenMod = new QAction("加载", this);
    connect(actionOpenMod, SIGNAL(triggered()), this, SLOT(OpenMod()));
    gameMenuMod->addAction(actionOpenMod);

    actionDownloadMod = new QAction("下载", this);
    connect(actionDownloadMod, SIGNAL(triggered()), this, SLOT(DownloadMod()));
    gameMenuMod->addAction(actionDownloadMod);

    actionCloseMod = new QAction("关闭", this);
    connect(actionCloseMod, SIGNAL(triggered()), this, SLOT(CloseMod()));
    gameMenuMod->addAction(actionCloseMod);

    actionAboutMod = new QAction("说明", this);
    connect(actionAboutMod, SIGNAL(triggered()), this, SLOT(AboutMod()));
    gameMenuMod->addAction(actionAboutMod);

    //QAction *actionZGZBMOD = new QAction("战国争霸MOD", this);
    //connect(actionZGZBMOD, SIGNAL(triggered()), this, SLOT(ZGZBMOD()));
    //gameMenu->addAction(actionZGZBMOD);


    QAction *actionExit = new QAction("退出", this);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(ExitGame()));
    gameMenu->addAction(actionExit);


    QMenu *gameMenu1 = menu1->addMenu(tr("选项"));
    //QMenu *gameMenuOriention = gameMenu1->addMenu(tr("屏幕方向"));
    QAction *actionOrientionPortrait = new QAction("切换为竖屏", this);
    connect(actionOrientionPortrait, SIGNAL(triggered()), this, SLOT(SetDirectionA()));
    gameMenu1->addAction(actionOrientionPortrait);

    QAction *actionOrientionLandscape = new QAction("切换为横屏", this);
    connect(actionOrientionLandscape, SIGNAL(triggered()), this, SLOT(SetDirectionB()));
    gameMenu1->addAction(actionOrientionLandscape);

    //QAction *actionWindowIni = new QAction("窗口配置", this);
    //connect(actionWindowIni, SIGNAL(triggered()), this, SLOT(WindowIni()));
    //gameMenu1->addAction(actionWindowIni);

    //QAction *actionKeyIni = new QAction("按键配置", this);
    //connect(actionKeyIni, SIGNAL(triggered()), this, SLOT(KeyIni()));
    //gameMenu1->addAction(actionKeyIni);

    QAction *actionBlack = new QAction("切换黑白版", this);
    connect(actionBlack, SIGNAL(triggered()), this, SLOT(VersionBlack()));
    gameMenu1->addAction(actionBlack);

    QAction *actionColor = new QAction("切换彩色版", this);
    connect(actionColor, SIGNAL(triggered()), this, SLOT(VersionColor()));
    gameMenu1->addAction(actionColor);

    QMenu *gameMenuScreen = gameMenu1->addMenu(tr("屏幕设置"));
    QAction *actionEnableTouch = new QAction("允许触屏", this);
    actionEnableTouch->setCheckable(true);
    actionEnableTouch->setChecked(false);
    if(g_touchEnable == 1)actionEnableTouch->setChecked(true);
    connect(actionEnableTouch, SIGNAL(triggered()), this, SLOT(CanTouch()));
    gameMenuScreen->addAction(actionEnableTouch);

    //20180810
    QAction *actionLayerKey = new QAction("浮层键盘", this);
    actionLayerKey->setCheckable(true);
    actionLayerKey->setChecked(false);
    if(g_layerKey == 1)actionLayerKey->setChecked(true);
    connect(actionLayerKey, SIGNAL(triggered()), this, SLOT(LayerKey()));
    gameMenuScreen->addAction(actionLayerKey);

    QAction *actionFullScreen = new QAction("全面触屏", this);
    actionFullScreen->setCheckable(true);
    actionFullScreen->setChecked(false);
    if(g_fullScreen == 1)actionFullScreen->setChecked(true);
    connect(actionFullScreen, SIGNAL(triggered()), this, SLOT(FullScreen()));
    gameMenuScreen->addAction(actionFullScreen);

    QAction *actionLargerKey = new QAction("增大按键尺寸", this);
    connect(actionLargerKey, SIGNAL(triggered()), this, SLOT(LargerKey()));
    gameMenuScreen->addAction(actionLargerKey);

    QAction *actionSmallerKey = new QAction("减小按键尺寸", this);
    connect(actionSmallerKey, SIGNAL(triggered()), this, SLOT(SmallerKey()));
    gameMenuScreen->addAction(actionSmallerKey);

    QMenu *gameMenuBackColor  = gameMenu1->addMenu(tr("背景颜色"));
    QAction *actionBack1 = new QAction("白色", this);
    actionBack1->setCheckable(true);
    actionBack1->setChecked(false);
    if(g_backColor == Qt::white)actionBack1->setChecked(true);
    connect(actionBack1, SIGNAL(triggered()), this, SLOT(Back1()));
    gameMenuBackColor->addAction(actionBack1);

    QAction *actionBack2 = new QAction("原色", this);
    actionBack2->setCheckable(true);
    actionBack2->setChecked(false);
    if(g_backColor == QColor(132,132,132))actionBack2->setChecked(true);
    connect(actionBack2, SIGNAL(triggered()), this, SLOT(Back2()));
    gameMenuBackColor->addAction(actionBack2);

    QAction *actionBack3 = new QAction("护眼色", this);
    actionBack3->setCheckable(true);
    actionBack3->setChecked(false);
    if(g_backColor == QColor(204,232,207))actionBack3->setChecked(true);
    connect(actionBack3, SIGNAL(triggered()), this, SLOT(Back3()));
    gameMenuBackColor->addAction(actionBack3);

    QAction *actionBack4 = new QAction("天蓝色", this);
    actionBack4->setCheckable(true);
    actionBack4->setChecked(false);
    if(g_backColor == QColor(148,198,221))actionBack4->setChecked(true);
    connect(actionBack4, SIGNAL(triggered()), this, SLOT(Back4()));
    gameMenuBackColor->addAction(actionBack4);

    QAction *actionBack5 = new QAction("棕色", this);
    actionBack5->setCheckable(true);
    actionBack5->setChecked(false);
    if(g_backColor == QColor(198,151,81))actionBack5->setChecked(true);
    connect(actionBack5, SIGNAL(triggered()), this, SLOT(Back5()));
    gameMenuBackColor->addAction(actionBack5);

    //20180725 add
    QMenu *gameMenuDelayTimes  = gameMenu1->addMenu(tr("延时调整"));
    QAction *actionFast = new QAction("减少延时", this);
    connect(actionFast, SIGNAL(triggered()), this, SLOT(Faster()));
    gameMenuDelayTimes->addAction(actionFast);
    QAction *actionSlow = new QAction("增加延时", this);
    connect(actionSlow, SIGNAL(triggered()), this, SLOT(Slower()));
    gameMenuDelayTimes->addAction(actionSlow);

    QMenu *gameMenuOther = gameMenu1->addMenu(tr("其他设置"));
    QAction *actionAutoSave = new QAction("实时存档", this);
    actionAutoSave->setCheckable(true);
    actionAutoSave->setChecked(false);
    if(g_autoSave == 1)actionAutoSave->setChecked(true);
    connect(actionAutoSave, SIGNAL(triggered()), this, SLOT(AutoSave()));
    gameMenuOther->addAction(actionAutoSave);

    actionAllowUpdate = new QAction("允许自动更新", this);
    actionAllowUpdate->setCheckable(true);
    actionAllowUpdate->setChecked(false);
    if(g_allowUpdate == 1)actionAllowUpdate->setChecked(true);
    connect(actionAllowUpdate, SIGNAL(triggered()), this, SLOT(AllowUpdate()));
    gameMenuOther->addAction(actionAllowUpdate);

    actionUseDotFont = new QAction("使用点阵字体", this);
    actionUseDotFont->setCheckable(true);
    actionUseDotFont->setChecked(false);
    if(g_useDotFont == 1)actionUseDotFont->setChecked(true);
    connect(actionUseDotFont, SIGNAL(triggered()), this, SLOT(UseDotFont()));
    gameMenuOther->addAction(actionUseDotFont);

    QAction *actionStatusBar = new QAction("切换状态栏", this);
    connect(actionStatusBar, SIGNAL(triggered()), this, SLOT(StatusBar()));
    gameMenuOther->addAction(actionStatusBar);

    QAction *actionCheckUpdate = new QAction("检测更新", this);
    connect(actionCheckUpdate, SIGNAL(triggered()), this, SLOT(CheckUpdate()));
    gameMenuOther->addAction(actionCheckUpdate);

    /*QMenu *gameMenu4 = menu1->addMenu(tr("用户"));
    QAction *actionUserLogin = new QAction("登录", this);
    connect(actionUserLogin, SIGNAL(triggered()), this, SLOT(UserLogin()));
    gameMenu4->addAction(actionUserLogin);

    QAction *actionUserRegister = new QAction("注册", this);
    connect(actionUserRegister, SIGNAL(triggered()), this, SLOT(UserRegister()));
    gameMenu4->addAction(actionUserRegister);

    QAction *actionUploadSaves = new QAction("云存档上传", this);
    connect(actionUploadSaves, SIGNAL(triggered()), this, SLOT(UploadSaves()));
    gameMenu4->addAction(actionUploadSaves);

    QAction *actionDownloadSaves = new QAction("云存档下载", this);
    connect(actionDownloadSaves, SIGNAL(triggered()), this, SLOT(DownloadSaves()));
    gameMenu4->addAction(actionDownloadSaves);

    QAction *actionManageSaves = new QAction("云存档管理", this);
    connect(actionManageSaves, SIGNAL(triggered()), this, SLOT(ManageSaves()));
    gameMenu4->addAction(actionManageSaves);

    QAction *actionUserLogout = new QAction("注销", this);
    connect(actionUserLogout, SIGNAL(triggered()), this, SLOT(UserLogout()));
    gameMenu4->addAction(actionUserLogout);*/

    QMenu *gameMenu2 = menu1->addMenu(tr("模式"));
    actionHighAtt = new QAction("高攻击概率模式", this);
    actionHighAtt->setCheckable(true);
    actionHighAtt->setChecked(false);
    if(g_HighAtt == 1)actionHighAtt->setChecked(true);
    connect(actionHighAtt, SIGNAL(triggered()), this, SLOT(HighAtt()));
    gameMenu2->addAction(actionHighAtt);

    QAction *actionDebugMode = new QAction("调试模式", this);
    //connect(actionDebugMode, SIGNAL(triggered()), this, SLOT(DebugMode()));
    gameMenu2->addAction(actionDebugMode);


    QMenu *gameMenu3 = menu1->addMenu(tr("帮助"));
    QAction *actionWhatsNew = new QAction("更新记录", this);
    connect(actionWhatsNew, SIGNAL(triggered()), this, SLOT(WhatsNew()));
    gameMenu3->addAction(actionWhatsNew);

    QAction *actionKeyHelp = new QAction("按键指南", this);
    connect(actionKeyHelp, SIGNAL(triggered()), this, SLOT(KeyHelp()));
    gameMenu3->addAction(actionKeyHelp);

    QAction *actionOnlineHelp = new QAction("在线帮助", this);
    connect(actionOnlineHelp, SIGNAL(triggered()), this, SLOT(OnlineHelp()));
    gameMenu3->addAction(actionOnlineHelp);

    QAction *actionAbout = new QAction("关于", this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    gameMenu3->addAction(actionAbout);

    menu1->setStyleSheet(
                "QMenu {\
                background-color: black;\
            border: 1px solid black;\
}\
QMenu::item {\
    font-size: 15pt; \
color: rgb(225,225,225);\
    background-color: transparent;\
padding:8px 80px;\
margin:2px 8px;\
    border-bottom:2px solid #DBDBDB;\
}\
QMenu::item:selected {\
                color: rgb(0,0,0);\
background-color: #afe4f3;\
}");
//menu1->setStyleSheet("QMenu::item{ padding:30px;}");
menu1->exec(QPoint(0,0));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{   
    // 通过鼠标的hover确定落子的标记


    // 存了坐标后也要重绘
    //update();
}

void MainWindow::wheelEvent(QWheelEvent * event)
{
    if(event->delta() > 0)
    {
        SendMessage(VK_UP);
    }
    else
    {
        SendMessage(VK_DOWN);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int MouseX = event->x();
    int MouseY = event->y();

#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    MouseStartX = (int)(((double)(MouseX) * 5.0 / g_screentimes));
    MouseStartY = (int)(((double)(MouseY) * 5.0 / g_screentimes));

    MouseEndY -=50;//win32
#elif defined(Q_OS_ANDROID)
    MouseStartX = (int)(((double)(MouseX-g_gameviewleft)) * 5.0 / g_screentimes);
    MouseStartY = (int)(((double)(MouseY-g_gameviewtop)) * 5.0 / g_screentimes);
#elif defined(Q_OS_IOS)
    MouseStartX = (int)(((double)(MouseX-g_gameviewleft)) * 5.0 / g_screentimes);
    MouseStartY = (int)(((double)(MouseY-g_gameviewtop)) * 5.0 / g_screentimes);
#endif
    mousePressTime =QDateTime::currentDateTime().toTime_t();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int MouseX = event->x();
    int MouseY = event->y();
    int mouseReleaseTime =QDateTime::currentDateTime().toTime_t();
    int t_gameviewleft = 0;
    int t_gameviewright = 0;


#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)


    MouseEndX = (int)(((double)(MouseX-g_gameviewleft) * 5.0 / g_screentimes));
    MouseEndY = (int)(((double)(MouseY-g_gameviewtop) * 5.0 / g_screentimes));

    //MouseEndY -=g_gameviewtop;//win32 50

#elif defined(Q_OS_ANDROID)



    MouseEndX = (int)(((double)(MouseX-g_gameviewleft)) * 5.0 / g_screentimes);
    MouseEndY = (int)(((double)(MouseY-g_gameviewtop)) * 5.0 / g_screentimes);


    if(g_layerKey == 1)
    {
        t_gameviewleft = g_gameviewleft;
        g_gameviewleft = g_gameviewlefta;
        t_gameviewright = g_gameviewright;
        g_gameviewright = g_gameviewrighta;
    }

    if(g_gameviewdirect ==0 && g_fullScreen == 0)
    {
        int t_height = g_screenheight;

        if(MouseX < g_gameviewleft  && MouseY < t_height/5 )
        {
            SendMessage(VK_PGUP);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*2/5 )
        {
            SendMessage(VK_UP);
        }
        else if(MouseX < g_gameviewleft/2  && MouseY < t_height*3/5 )
        {
            SendMessage(VK_LEFT);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*3/5 )
        {
            SendMessage(VK_RIGHT);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*4/5 )
        {
            SendMessage(VK_DOWN);
        }
        else if(MouseX < g_gameviewleft  && MouseY > t_height*4/5 )
        {
            SendMessage(VK_PGDN);
        }
        else if(MouseX > g_gameviewright  && MouseY < t_height/4 )
        {
            SendMessage(VK_SEARCH);
        }
        else if(MouseX > g_gameviewright  && MouseY < t_height*2/4 )
        {
            if(flagwar == 0)
            {
                OpenAndroidMenu();

            }
            else
            {
                SendMessage(VK_HELP);
            }
        }
        else if(MouseX > g_gameviewright  && MouseY < t_height*3/4 )
        {
            SendMessage(VK_EXIT);
        }
        else if(MouseX > g_gameviewright  && MouseY > t_height*3/4 )
        {
            SendMessage(VK_ENTER);
        }
    }
    else
    {
        int t_height = g_gameviewbottom - g_gameviewtop;
        int t_unitwidth = g_screenwidth/6;
        int t_unitheight = t_height/4;
        int t_startx = 0;
        int t_starty = (g_screenheight - 50) - t_height;//-100去除状态栏影响
        if(t_starty < g_gameviewbottom)
        {
            t_starty = g_gameviewbottom;
            t_height = (g_screenheight - 50) - g_gameviewbottom;
            t_unitheight = t_height/4;
        }

        if(MouseX > (t_startx+t_unitwidth*2)  && MouseY > (t_starty+t_unitheight*1)
                && MouseX < (t_startx+t_unitwidth*3)  && MouseY < (t_starty+t_unitheight*2))
        {
            SendMessage(VK_UP);
        }
        else if(MouseX > (t_startx+t_unitwidth*1)  && MouseY > (t_starty+t_unitheight*2)
                && MouseX < (t_startx+t_unitwidth*2)  && MouseY < (t_starty+t_unitheight*3))
        {
            SendMessage(VK_LEFT);
        }
        else if(MouseX > (t_startx+t_unitwidth*3)  && MouseY > (t_starty+t_unitheight*2)
                && MouseX < (t_startx+t_unitwidth*4)  && MouseY < (t_starty+t_unitheight*3))
        {
            SendMessage(VK_RIGHT);
        }
        else if(MouseX > (t_startx+t_unitwidth*2)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*3)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_DOWN);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*1)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*2))
        {
            SendMessage(VK_PGUP);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_PGDN);
        }
        else if(MouseX > (t_startx+t_unitwidth*5)  && MouseY > (t_starty+t_unitheight*0)
                && MouseX < (t_startx+t_unitwidth*6)  && MouseY < (t_starty+t_unitheight*1))
        {
            SendMessage(VK_SEARCH);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*0)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*1))
        {
            if(flagwar == 0)
            {
                OpenAndroidMenu();

            }
            else
            {
                SendMessage(VK_HELP);

            }
        }
        else if(MouseX > (t_startx+t_unitwidth*0)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*1)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_EXIT);
        }
        else if(MouseX > (t_startx+t_unitwidth*5)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*6)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_ENTER);
        }
    }
    if(g_layerKey == 1)
    {
        g_gameviewleft = t_gameviewleft;
        g_gameviewright = t_gameviewright;
    }

#elif defined(Q_OS_IOS)



    MouseEndX = (int)(((double)(MouseX-g_gameviewleft)) * 5.0 / g_screentimes);
    MouseEndY = (int)(((double)(MouseY-g_gameviewtop)) * 5.0 / g_screentimes);

    if(g_layerKey == 1)
    {
        t_gameviewleft = g_gameviewleft;
        g_gameviewleft = g_gameviewlefta;
        t_gameviewright = g_gameviewright;
        g_gameviewright = g_gameviewrighta;
    }


    if(g_gameviewdirect ==0 && g_fullScreen == 0)
    {
        int t_height = g_screenheight - 50;
        if(MouseX < g_gameviewleft  && MouseY < t_height/5 )
        {
            SendMessage(VK_PGUP);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*2/5 )
        {
            SendMessage(VK_UP);
        }
        else if(MouseX < g_gameviewleft/2  && MouseY < t_height*3/5 )
        {
            SendMessage(VK_LEFT);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*3/5 )
        {
            SendMessage(VK_RIGHT);
        }
        else if(MouseX < g_gameviewleft  && MouseY < t_height*4/5 )
        {
            SendMessage(VK_DOWN);
        }
        else if(MouseX < g_gameviewleft  && MouseY > t_height*4/5 )
        {
            SendMessage(VK_PGDN);
        }
        else if(MouseX > g_gameviewright  && MouseY < t_height/4 )
        {
            SendMessage(VK_SEARCH);
        }
        else if(MouseX > g_gameviewright  && MouseY < t_height*2/4 )
        {
            SendMessage(VK_HELP);

        }
        else if(MouseX > g_gameviewright  && MouseY < t_height*3/4 )
        {
            SendMessage(VK_EXIT);
        }
        else if(MouseX > g_gameviewright  && MouseY > t_height*3/4 )
        {
            SendMessage(VK_ENTER);
        }
    }
    else
    {

        int t_height = g_gameviewbottom - g_gameviewtop;
        int t_unitwidth = g_screenwidth/6;
        int t_unitheight = t_height/4;
        int t_startx = 0;
        int t_starty = (g_screenheight - 50) - t_height;//-100去除状态栏影响
        if(t_starty < g_gameviewbottom)
        {
            t_starty = g_gameviewbottom;
            t_height = (g_screenheight - 50) - g_gameviewbottom;
            t_unitheight = t_height/4;
        }

        if(MouseX > (t_startx+t_unitwidth*2)  && MouseY > (t_starty+t_unitheight*1)
                && MouseX < (t_startx+t_unitwidth*3)  && MouseY < (t_starty+t_unitheight*2))
        {
            SendMessage(VK_UP);
        }
        else if(MouseX > (t_startx+t_unitwidth*1)  && MouseY > (t_starty+t_unitheight*2)
                && MouseX < (t_startx+t_unitwidth*2)  && MouseY < (t_starty+t_unitheight*3))
        {
            SendMessage(VK_LEFT);
        }
        else if(MouseX > (t_startx+t_unitwidth*3)  && MouseY > (t_starty+t_unitheight*2)
                && MouseX < (t_startx+t_unitwidth*4)  && MouseY < (t_starty+t_unitheight*3))
        {
            SendMessage(VK_RIGHT);
        }
        else if(MouseX > (t_startx+t_unitwidth*2)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*3)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_DOWN);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*1)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*2))
        {
            SendMessage(VK_PGUP);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_PGDN);
        }
        else if(MouseX > (t_startx+t_unitwidth*5)  && MouseY > (t_starty+t_unitheight*0)
                && MouseX < (t_startx+t_unitwidth*6)  && MouseY < (t_starty+t_unitheight*1))
        {
            SendMessage(VK_SEARCH);
        }
        else if(MouseX > (t_startx+t_unitwidth*4)  && MouseY > (t_starty+t_unitheight*0)
                && MouseX < (t_startx+t_unitwidth*5)  && MouseY < (t_starty+t_unitheight*1))
        {
            SendMessage(VK_HELP);


        }
        else if(MouseX > (t_startx+t_unitwidth*0)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*1)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_EXIT);
        }
        else if(MouseX > (t_startx+t_unitwidth*5)  && MouseY > (t_starty+t_unitheight*3)
                && MouseX < (t_startx+t_unitwidth*6)  && MouseY < (t_starty+t_unitheight*4))
        {
            SendMessage(VK_ENTER);
        }
    }

    if(g_layerKey == 1)
    {
        g_gameviewleft = t_gameviewleft;
        g_gameviewright = t_gameviewright;
    }
#endif

    if(g_fullScreen == 0)
    {
        if(MouseEndX <0 ||MouseEndY < 0)return;
        if(MouseEndX >800 ||MouseEndY > 480)return;

    }
    if(g_touchEnable == 0)return;

    if(event->button()==Qt::RightButton)
    {
        SendMessage(VK_EXIT);
        return;
    }
    if((mouseReleaseTime-mousePressTime)>=1 )
    {
        if((MouseEndY - MouseStartY) < 10 && (MouseEndX - MouseStartX) < 10)
        {
            SendMessage(VK_EXIT);
            return;
        }
    }

    if(MouseX > g_gameviewleft && MouseX < g_gameviewright)//界面上
    {


        if(ng == 0)//choiceshow
        {
            if(MouseEndX >= 25 && (MouseEndY+50) >= 220 && MouseEndX <= (25 + 355) && (MouseEndY+50) <= (220+155) )
            {
                if(g_stat == 1)SendMessage(VK_UP);
                if(g_stat == 2)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat == 3)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 410 && (MouseEndY+50) >= 220 && MouseEndX <= (410 + 355) && (MouseEndY+50) <= (220+155) )
            {
                if(g_stat == 0)SendMessage(VK_DOWN);
                if(g_stat == 2)SendMessage(VK_UP);
                if(g_stat == 3)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 25 && (MouseEndY+50) >= 350 && MouseEndX <= (25 + 355) && (MouseEndY+50) <= (350+155) )
            {
                if(g_stat == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat == 1)SendMessage(VK_DOWN);
                if(g_stat == 3)SendMessage(VK_UP);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 410 && (MouseEndY+50) >= 350 && MouseEndX <= (410 + 355) && (MouseEndY+50) <= (350+155) )
            {
                if(g_stat == 0)SendMessage(VK_UP);
                if(g_stat == 1)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat == 2)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }

        }
        else if((ng == 1||ng == 3) && nga == 0)//新游戏选择历史时期界面 读入存档
        {
            if(MouseEndX >= 8 && (MouseEndY+50) >= 120 && MouseEndX <= (8 + 390) && (MouseEndY+50) <= (120+165) )
            {
                if(g_stat == 1)SendMessage(VK_UP);
                if(g_stat == 2)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat == 3)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 390) && (MouseEndY+50) <= (120+165) )
            {
                if(g_stat == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat == 1)SendMessage(VK_DOWN);
                if(g_stat == 3)SendMessage(VK_UP);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 8 && (MouseEndY+50) >= 305 && MouseEndX <= ( 8 + 390) && (MouseEndY+50) <= (305+165) )
            {
                if(g_stat == 0)SendMessage(VK_DOWN);
                if(g_stat == 2)SendMessage(VK_UP);
                if(g_stat == 3)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 410 && (MouseEndY+50) >= 305 && MouseEndX <= (410 + 390) && (MouseEndY+50) <= (305+165) )
            {
                if(g_stat == 0)SendMessage(VK_UP);
                if(g_stat == 1)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat == 2)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 745 && (MouseEndY+50) >= 50 && MouseEndX <= (745 + 45) && (MouseEndY+50) <= (50+45) )
            {
                SendMessage(VK_EXIT);
            }
        }
        else if(ng == 2 )//读档界面
        {
            if(MouseEndX >= 155 && (MouseEndY+50) >= 220 && MouseEndX <= (155 + 480) && (MouseEndY+50) <= (220+70) )
            {
                if(g_stat == 1)SendMessage(VK_UP);
                if(g_stat == 2)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 155 && (MouseEndY+50) >= 290 && MouseEndX <= (155 + 480) && (MouseEndY+50) <= (290+70) )
            {
                if(g_stat == 0)SendMessage(VK_DOWN);
                if(g_stat == 2)SendMessage(VK_UP);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 155 && (MouseEndY+50) >= 360 && MouseEndX <= ( 155 + 480) && (MouseEndY+50) <= (360+70) )
            {
                if(g_stat == 0)SendMessage(VK_UP);
                if(g_stat == 1)SendMessage(VK_DOWN);
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 745 && (MouseEndY+50) >= 50 && MouseEndX <= (745 + 45) && (MouseEndY+50) <= (50+45) )
            {
                SendMessage(VK_EXIT);
            }
        }
        else if(ng == 10 )
        {
            SendMessage(VK_ENTER);
        }
        else if((ng == 1||ng == 3) && nga == 1 && ngb == 0)//who界面
        {
            if(MouseEndX >= 80 && (MouseEndY+50) >= 120 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (120+60) )
            {
                if(g_stat  == 1)
                {
                    SendMessage(VK_UP);
                }
                if(g_stat  == 2)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 3)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 4)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 5)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 80 && (MouseEndY+50) >= 180 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (180+60) )
            {
                if(g_stat  == 0)
                {
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 2)
                {
                    SendMessage(VK_UP);
                }
                if(g_stat  == 3)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 4)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 5)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 80 && (MouseEndY+50) >= 240 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (240+60) )
            {
                if(g_stat  == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 1)
                {
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 3)
                {
                    SendMessage(VK_UP);
                }
                if(g_stat  == 4)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                if(g_stat  == 5)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 80 && (MouseEndY+50) >= 300 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (300+60) )
            {
                if(g_stat  == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 1)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 2)
                {
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 4)
                {
                    SendMessage(VK_UP);
                }
                if(g_stat  == 5)
                {
                    SendMessage(VK_UP);
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 80 && (MouseEndY+50) >= 360 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (360+60) )
            {
                if(g_stat  == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 1)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 2)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 3)
                {
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 5)
                {
                    SendMessage(VK_UP);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 80 && (MouseEndY+50) >= 420 && MouseEndX <= (80 + 180) && (MouseEndY+50) <= (420+60) )
            {
                if(g_stat  == 0)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 1)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 2)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 3)
                {
                    SendMessage(VK_DOWN);
                    SendMessage(VK_DOWN);
                }
                if(g_stat  == 4)
                {
                    SendMessage(VK_DOWN);
                }
                SendMessage(VK_ENTER);
            }
            else if(MouseEndX >= 745 && (MouseEndY+50) >= 50 && MouseEndX <= (745 + 45) && (MouseEndY+50) <= (50+45) )
            {
                SendMessage(VK_EXIT);
            }
        }
        else if((ng == 1||ng == 3) && nga == 1 && ngb == 1)//地图主界面
        {
            if(operateflag > 0)//400,100,530,230//operate
            {
                if(0)//who.gamemap.operate.style == -1)
                {
                    if(MouseEndX >= 400 && (MouseEndY+50) >= 100 && MouseEndX <= (530) && (MouseEndY+50) <= (230) )
                    {
                        SendMessage(VK_ENTER);
                    }
                    else
                    {
                        SendMessage(VK_EXIT);
                    }
                }
                else//200,200,600,400
                {
                    if(MouseEndX >= 200 && (MouseEndY+50) >= 200 && MouseEndX <= (600) && (MouseEndY+50) <= (400) )
                    {
                        SendMessage(VK_ENTER);
                    }
                    else
                    {
                        SendMessage(VK_EXIT);
                    }
                }
            }
            else if(flagwar == 1 && ngf == 2)//战斗技能菜单
            {
                int max = 2;//useableskillnum2;
                if(max >=1 && MouseEndX >= 420 && (MouseEndY+50) >= 220 && MouseEndX <= (420 + 120) && (MouseEndY+50) <= (220+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(max >=2 && MouseEndX >= 420 && (MouseEndY+50) >= (220+60) && MouseEndX <= (420 + 120) && (MouseEndY+50) <= (220+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }


                    SendMessage(VK_ENTER);
                }

                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 && ngf==3)//hero菜单
            {
                if(MouseEndX >= 400 && (MouseEndY+50) >= 200 && MouseEndX <= (400 + 240) && (MouseEndY+50) <= (200+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    if(g_stat == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 400 && (MouseEndY+50) >= (200+60) && MouseEndX <= (400 + 240) && (MouseEndY+50) <= (200+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }


                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 400 && (MouseEndY+50) >=(200+120) && MouseEndX <= (400 + 240) && (MouseEndY+50) <= (200+180))
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }


                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 400 && (MouseEndY+50) >= (200+180) && MouseEndX <= (400 + 240) && (MouseEndY+50) <= (200+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }

                    SendMessage(VK_ENTER);
                }


                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 && ngf == 4)//menushow
            {
                if(MouseEndX >= 290 && (MouseEndY+50) >= 200 && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 290 && (MouseEndY+50) >= (200+60) && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }


                    SendMessage(VK_ENTER);
                }

                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 && ngf == 5)//menuspeed
            {
                if(MouseEndX >= 290 && (MouseEndY+50) >= 200 && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 290 && (MouseEndY+50) >= (200+60) && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }


                    SendMessage(VK_ENTER);
                }

                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 && ngf == 6)//menumove
            {
                if(MouseEndX >= 290 && (MouseEndY+50) >= 200 && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 290 && (MouseEndY+50) >= (200+60) && MouseEndX <= (290 + 120) && (MouseEndY+50) <= (200+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }


                    SendMessage(VK_ENTER);
                }

                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 &&  ngf == 1)//战斗主菜单
            {
                if(MouseEndX >= 30 && (MouseEndY+50) >= 80 && MouseEndX <= (30 + 300) && (MouseEndY+50) <= (80+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    if(g_stat == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 30 && (MouseEndY+50) >= (80+60) && MouseEndX <= (30 + 300) && (MouseEndY+50) <= (80+120) )
                {
                    if(g_stat == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 30 && (MouseEndY+50) >=(80+120) && MouseEndX <= (30 + 300) && (MouseEndY+50) <= (80+180))
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 30 && (MouseEndY+50) >= (80+180) && MouseEndX <= (30 + 300) && (MouseEndY+50) <= (80+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 30 && (MouseEndY+50) >= (80+240) && MouseEndX <= (30 + 300) && (MouseEndY+50) <= (80+300) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                    }

                    SendMessage(VK_ENTER);
                }

                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(flagwar == 1 && flagstat == 0 && ngf == 0)//战斗地图
            {
                if(g_fullScreen == 1)
                {
                    if(MouseEndX <= 25  )
                    {
                        SendMessage(VK_LEFT);
                    }
                    else if(MouseEndX >= 775 )
                    {
                        SendMessage(VK_RIGHT);
                    }
                    else if(MouseEndY <= 25 )
                    {
                        SendMessage(VK_UP);
                    }
                    else if(MouseEndY >= 455 )
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(MouseEndX >= FIGHTMENUX && (MouseEndY+50) >= (FIGHTMENUY+50) && MouseEndX <= (FIGHTMENUX + 200) && (MouseEndY+50) <= (FIGHTMENUY+100) )
                    {
                        SendMessage(VK_EXIT);
                    }
                    else
                    {
                        int posx = g_FoucsX - g_MapSX;
                        int posy = g_FoucsY - g_MapSY;
                        int t_posx = MouseEndX/80;
                        int t_posy = MouseEndY/80;

                        if(t_posx == posx && t_posy == posy)
                        {
                            SendMessage(VK_ENTER);
                        }

                        if(posx < t_posx)
                        {
                            for(int i = 0 ; i < (t_posx-posx);i++)
                            {
                                SendMessage(VK_RIGHT);
                            }
                        }
                        if(posx > t_posx)
                        {
                            for(int i = 0 ; i < (posx-t_posx);i++)
                            {
                                SendMessage(VK_LEFT);
                            }
                        }
                        if(posy < t_posy)
                        {
                            for(int i = 0 ; i < (t_posy-posy);i++)
                            {
                                SendMessage(VK_DOWN);
                            }
                        }
                        if(posy > t_posy)
                        {
                            for(int i = 0 ; i < (posy-t_posy);i++)
                            {
                                SendMessage(VK_UP);
                            }
                        }

                    }
                    return;
                }
                if(MouseEndX >= FIGHTLEFTARROWX && (MouseEndY+50) >= (FIGHTLEFTARROWY+50) && MouseEndX <= (FIGHTLEFTARROWX + 24) && (MouseEndY+50) <= (FIGHTLEFTARROWY+50+32) )
                {
                    SendMessage(VK_LEFT);
                }
                else if(MouseEndX >= FIGHTRIGHTARROWX && (MouseEndY+50) >= (FIGHTRIGHTARROWY+50) && MouseEndX <= (FIGHTRIGHTARROWX + 24) && (MouseEndY+50) <= (FIGHTRIGHTARROWY+50+32) )
                {
                    SendMessage(VK_RIGHT);
                }
                else if(MouseEndX >= FIGHTUPARROWX && (MouseEndY+50) >= (FIGHTUPARROWY+50) && MouseEndX <= (FIGHTUPARROWX + 32) && (MouseEndY+50) <= (FIGHTUPARROWY+50+24) )
                {
                    SendMessage(VK_UP);
                }
                else if(MouseEndX >= FIGHTDOWNARROWX && (MouseEndY+50) >= (FIGHTDOWNARROWY+50) && MouseEndX <= (FIGHTDOWNARROWX + 32) && (MouseEndY+50) <= (FIGHTDOWNARROWY+50+24) )
                {
                    SendMessage(VK_DOWN);
                }
                else if(MouseEndX >= FIGHTMENUX && (MouseEndY+50) >= (FIGHTMENUY+50) && MouseEndX <= (FIGHTMENUX + 200) && (MouseEndY+50) <= (FIGHTMENUY+100) )
                {
                    SendMessage(VK_EXIT);
                }
                else
                {
                    int posx = g_FoucsX - g_MapSX;
                    int posy = g_FoucsY - g_MapSY;
                    int t_posx = MouseEndX/80;
                    int t_posy = MouseEndY/80;

                    if(t_posx == posx && t_posy == posy)
                    {
                        SendMessage(VK_ENTER);
                    }

                    if(posx < t_posx)
                    {
                        for(int i = 0 ; i < (t_posx-posx);i++)
                        {
                            SendMessage(VK_RIGHT);
                        }
                    }
                    if(posx > t_posx)
                    {
                        for(int i = 0 ; i < (posx-t_posx);i++)
                        {
                            SendMessage(VK_LEFT);
                        }
                    }
                    if(posy < t_posy)
                    {
                        for(int i = 0 ; i < (t_posy-posy);i++)
                        {
                            SendMessage(VK_DOWN);
                        }
                    }
                    if(posy > t_posy)
                    {
                        for(int i = 0 ; i < (posy-t_posy);i++)
                        {
                            SendMessage(VK_UP);
                        }
                    }

                }
            }
            else if(flagwar == 0 && ngsave == 0 && ngc == 0)//主地图
            {
                if(g_fullScreen == 1)//全屏特殊下的情况
                {
                    if(MouseEndX <= 25  )
                    {
                        SendMessage(VK_LEFT);
                    }
                    else if(MouseEndX >= 775 )
                    {
                        SendMessage(VK_RIGHT);
                    }
                    else if(MouseEndY <= 25 )
                    {
                        SendMessage(VK_UP);
                    }
                    else if(MouseEndY >= 455 )
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(MouseEndX >= MAINMENUX && (MouseEndY+50) >= (MAINMENUY+50) && MouseEndX <= (MAINMENUX + 100) && (MouseEndY+50) <= (MAINMENUY+100) )
                    {
                        SendMessage(VK_EXIT);
                    }
                    else
                    {
                        int posx = g_CityPos.setx-g_CityPos.x;
                        int posy = g_CityPos.sety-g_CityPos.y;
                        int t_posx = MouseEndX/80;
                        int t_posy = MouseEndY/80;

                        if(t_posx>=8 || t_posy>=6)
                        {

                        }
                        else
                        {

                            if(t_posx == posx && t_posy == posy)
                            {
                                SendMessage(VK_ENTER);
                            }

                            if(posx < t_posx)
                            {
                                for(int i = 0 ; i < (t_posx-posx);i++)
                                {
                                    SendMessage(VK_RIGHT);
                                }
                            }
                            if(posx > t_posx)
                            {
                                for(int i = 0 ; i < (posx-t_posx);i++)
                                {
                                    SendMessage(VK_LEFT);
                                }
                            }
                            if(posy < t_posy)
                            {
                                for(int i = 0 ; i < (t_posy-posy);i++)
                                {
                                    SendMessage(VK_DOWN);
                                }
                            }
                            if(posy > t_posy)
                            {
                                for(int i = 0 ; i < (posy-t_posy);i++)
                                {
                                    SendMessage(VK_UP);
                                }
                            }
                            //SendMessage(VK_ENTER);
                        }
                    }
                    return;
                }
                //判断起始及结束坐标
                if((MouseEndX - MouseStartX) > 30
                        || (MouseStartX - MouseEndX) > 30
                        || (MouseEndY - MouseStartY) > 30
                        || (MouseStartY - MouseEndY) > 30)
                {
                    if((MouseEndX - MouseStartX) > 30)//滚动
                    {
                        int times = (MouseEndX - MouseStartX)/30;
                        for(int i = 0 ; i < times;i++)
                        {
                            SendMessage(VK_RIGHT);
                        }
                    }
                    else if((MouseStartX - MouseEndX) > 30)//滚动
                    {
                        int times = (MouseStartX - MouseEndX)/30;
                        for(int i = 0 ; i < times;i++)
                        {
                            SendMessage(VK_LEFT);
                        }
                    }
                    if((MouseEndY - MouseStartY) > 30)//滚动
                    {
                        int times = (MouseEndY - MouseStartY)/30;
                        for(int i = 0 ; i < times;i++)
                        {
                            SendMessage(VK_DOWN);
                        }
                    }
                    else if((MouseStartY - MouseEndY) > 30)//滚动
                    {
                        int times = (MouseStartY - MouseEndY)/30;
                        for(int i = 0 ; i < times;i++)
                        {
                            SendMessage(VK_UP);
                        }
                    }
                }
                /*else if(MouseEndX >= MAINLEFTARROWX && (MouseEndY+50) >= (MAINLEFTARROWY+50) && MouseEndX <= (MAINLEFTARROWX + 24) && (MouseEndY+50) <= (MAINLEFTARROWY+50+32) )
                {
                    SendMessage(VK_LEFT);
                }
                else if(MouseEndX >= MAINRIGHTARROWX && (MouseEndY+50) >= (MAINRIGHTARROWY+50) && MouseEndX <= (MAINRIGHTARROWX + 24) && (MouseEndY+50) <= (MAINRIGHTARROWY+50+32) )
                {
                    SendMessage(VK_RIGHT);
                }
                else if(MouseEndX >= MAINUPARROWX && (MouseEndY+50) >= (MAINUPARROWY+50) && MouseEndX <= (MAINUPARROWX + 32) && (MouseEndY+50) <= (MAINUPARROWY+50+24) )
                {
                    SendMessage(VK_UP);
                }
                else if(MouseEndX >= MAINDOWNARROWX && (MouseEndY+50) >= (MAINDOWNARROWY+50) && MouseEndX <= (MAINDOWNARROWX + 32) && (MouseEndY+50) <= (MAINDOWNARROWY+50+24) )
                {
                    SendMessage(VK_DOWN);
                }
                */else if(MouseEndX >= MAINMENUX && (MouseEndY+50) >= (MAINMENUY+50) && MouseEndX <= (MAINMENUX + 100) && (MouseEndY+50) <= (MAINMENUY+100) )
                {
                    SendMessage(VK_EXIT);
                }
                else
                {
                    int posx = g_CityPos.setx-g_CityPos.x;
                    int posy = g_CityPos.sety-g_CityPos.y;
                    int t_posx = MouseEndX/80;
                    int t_posy = MouseEndY/80;

                    if(t_posx>=8 || t_posy>=6)
                    {

                    }
                    else
                    {

                        if(t_posx == posx && t_posy == posy)
                        {
                            SendMessage(VK_ENTER);
                        }

                        if(posx < t_posx)
                        {
                            for(int i = 0 ; i < (t_posx-posx);i++)
                            {
                                SendMessage(VK_RIGHT);
                            }
                        }
                        if(posx > t_posx)
                        {
                            for(int i = 0 ; i < (posx-t_posx);i++)
                            {
                                SendMessage(VK_LEFT);
                            }
                        }
                        if(posy < t_posy)
                        {
                            for(int i = 0 ; i < (t_posy-posy);i++)
                            {
                                SendMessage(VK_DOWN);
                            }
                        }
                        if(posy > t_posy)
                        {
                            for(int i = 0 ; i < (posy-t_posy);i++)
                            {
                                SendMessage(VK_UP);
                            }
                        }
                        //SendMessage(VK_ENTER);
                    }
                }

            }
            else  if(ngc == 1 && ngsave == 0)//escmenu
            {
                if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+120) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+180) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    SendMessage(VK_ENTER);
                }
                else //if(MouseEndX >= 0 && (MouseEndY+50) >= 500 && MouseEndX <= (0 + 100) && (MouseEndY+50) <= (500+50) )
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(ngsave == 1 )
            {
                if(MouseEndX >= 155 && (MouseEndY+50) >= 220 && MouseEndX <= (155 + 480) && (MouseEndY+50) <= (220+70) )
                {
                    if(g_stat == 1)SendMessage(VK_UP);
                    if(g_stat == 2)SendMessage(VK_DOWN);
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 155 && (MouseEndY+50) >= 290 && MouseEndX <= (155 + 480) && (MouseEndY+50) <= (290+70) )
                {
                    if(g_stat == 0)SendMessage(VK_DOWN);
                    if(g_stat == 2)SendMessage(VK_UP);
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 155 && (MouseEndY+50) >= 360 && MouseEndX <= ( 155 + 480) && (MouseEndY+50) <= (360+70) )
                {
                    if(g_stat == 0)SendMessage(VK_UP);
                    if(g_stat == 1)SendMessage(VK_DOWN);
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 745 && (MouseEndY+50) >= 50 && MouseEndX <= (745 + 45) && (MouseEndY+50) <= (50+45) )
                {
                    SendMessage(VK_EXIT);
                }
            }
            else  if(ngc == 2 && ngsecond1 == 0)//citymenu
            {
                if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+120) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+180) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 300 && (MouseEndY+50) >= 100 && MouseEndX <= (300 + 240) && (MouseEndY+50) <= (100+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    SendMessage(VK_ENTER);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }

            }
            else  if(ngc == 2 && ngsecond1 == 1 && ngthird1 == 0)//neizhengmenu
            {
                if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+120) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+180) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+300) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+360) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_DOWN);
                    }
                    SendMessage(VK_ENTER);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else  if(ngc == 2 && ngsecond1 == 2 && ngthird1 == 0)//外交menu
            {
                if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+120) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+180) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                    }


                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+300) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                    }

                    SendMessage(VK_ENTER);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }

            }

            else  if(ngc == 2 && ngsecond1 == 3 && ngthird1 == 0)//军备menu
            {
                if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+60) )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+120) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+180) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+240) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }

                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+300) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(MouseEndX >= 410 && (MouseEndY+50) >= 120 && MouseEndX <= (410 + 240) && (MouseEndY+50) <= (120+360) )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_DOWN);
                    }
                    SendMessage(VK_ENTER);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else  if(ngc == 3)//城市状态
            {
                if(MouseEndX >= 630 && (MouseEndY+50) >= 50 && MouseEndX <= (630 + 50) && (MouseEndY+50) <= (50+50) )
                {
                    SendMessage(VK_EXIT);
                }
                else if(MouseEndX >= 140 && (MouseEndY+50) >= 50 && MouseEndX <= (140 + 690) && (MouseEndY+50) <= (50+450) )
                {
                    SendMessage(VK_DOWN);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }
            }
            else if(ngthird1 >0)//武将列表
            {
                //10,55,785,heronu>5?475:(heronu+1)*60+55
                int heronu1 = 6;//20180226who.gamemap.herolist.heronu;
                int max = heronu1>5?6:heronu1;
                //(heronu1>5?475:(heronu1+1)*60+55);
                if(max >= 1 && MouseEndX >= 10 && (MouseEndY+50) >= 115 && MouseEndX <= 785 && (MouseEndY+50) <= 175 )
                {
                    if(g_stat  == 1)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(max >= 2 && MouseEndX >= 10 && (MouseEndY+50) >= 175 && MouseEndX <= 785 &&  (MouseEndY+50) <= 235  )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(max >= 3 && MouseEndX >= 10 && (MouseEndY+50) >= 235 && MouseEndX <= 785 &&  (MouseEndY+50) <= 295  )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(max >= 4 && MouseEndX >= 10 && (MouseEndY+50) >= 295 && MouseEndX <= 785 &&  (MouseEndY+50) <= 355  )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_UP);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(max >= 5 && MouseEndX >= 10 && (MouseEndY+50) >= 355 && MouseEndX <= 785 &&  (MouseEndY+50) <= 415  )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 5)
                    {
                        SendMessage(VK_UP);
                    }
                    SendMessage(VK_ENTER);
                }
                else if(max >= 6 && MouseEndX >= 10 && (MouseEndY+50) >= 415 && MouseEndX <= 785 &&  (MouseEndY+50) <= 475  )
                {
                    if(g_stat  == 0)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 1)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 2)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 3)
                    {
                        SendMessage(VK_DOWN);
                        SendMessage(VK_DOWN);
                    }
                    else if(g_stat  == 4)
                    {
                        SendMessage(VK_DOWN);
                    }
                    SendMessage(VK_ENTER);
                }
                else
                {
                    SendMessage(VK_EXIT);
                }
            }
        }
    }



}

#include <QDebug>


