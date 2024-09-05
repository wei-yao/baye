#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>
#include "windowinidialog.h"
#if defined(Q_OS_ANDROID)
#include<QtAndroidExtras/QAndroidJniObject>
#include<QtAndroidExtras/QAndroidJniEnvironment>
#include<QtAndroidExtras/QtAndroid>
#include <QDir>
//#include <QtAndroidExtras>

using namespace QtAndroid;

#elif defined (Q_OS_IOS)
#include "ios-sources/src/viewRoute.h"
#include <QDir>
#endif
//PaintWidget qobj;
int g_screenwidth = 0;
int g_screenheight = 0;
double g_screentimes = 1.0;
int g_gameviewleft = 0;
int g_gameviewright = 0;
int g_gameviewtop = 0;
int g_gameviewbottom = 0;
int g_gameviewlefta = 0;
int g_gameviewrighta = 0;

#if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
int g_gameviewdirect = 0;//0;//0-横屏 1-竖屏
#else
int g_gameviewdirect = 1;//0;//0-横屏 1-竖屏
#endif
int g_Fontsize = 0;//字体大小
extern int g_showPicType;
extern int g_useDotFont;
extern int g_HighAtt;
extern int g_autoSave;
extern int g_allowUpdate;
extern int g_touchEnable ;
extern int g_layerKey;
extern int g_fullScreen;

extern QColor g_backColor;
extern int g_useMOD;

QString androidFont;
int version_now = 180701300;
QString title = QObject::tr("天际边三国霸业");
QString startUpIniPath;
QString versionIniPath;
//QString modverIniPath;

int main(int argc, char *argv[])
{

    QTextCodec *codec = QTextCodec::codecForName("GB18030");//情况2
    QTextCodec::setCodecForLocale(codec);



#if defined(Q_OS_ANDROID)


     //20180609 从内部存储到SD卡
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    QString fullPath = QString("%1/skyside3gby3/").arg(externalStorageDir.toString());


    QDir dir(fullPath);
    if(!dir.exists())
    {
        dir.mkdir(fullPath);
    }

    QString fontPath = QString("%1/skyside3gby3/font.bin").arg(externalStorageDir.toString());

    if(!QFile::exists(fontPath))
    {
        QFile::copy("assets:/font.bin",fontPath);
        QFile fileDest(fontPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }

    QString datlibPath = QString("%1/skyside3gby3/dat.lib").arg(externalStorageDir.toString());

    if(!QFile::exists(datlibPath))
    {
        QFile::copy("assets:/dat.lib",datlibPath);
        QFile fileDest(datlibPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }

    QString resrlbPath = QString("%1/skyside3gby3/res.rlb").arg(externalStorageDir.toString());

    //if(!QFile::exists("res.rlb"))//下个版本需要取消注释
    {
        QFile::copy("assets:/res.rlb",resrlbPath);
        QFile fileDest2(resrlbPath);
        fileDest2.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }

    QString datmodPath = QString("%1/skyside3gby3/dat.mod").arg(externalStorageDir.toString());

    /*if(!QFile::exists("dat.mod"))//下个版本需要取消注释
    {
        QFile::copy("assets:/dat.mod",datmodPath);
        QFile fileDest3(datmodPath);
        fileDest3.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    }*/

    QString startupiniPath = QString("%1/skyside3gby3/startup.ini").arg(externalStorageDir.toString());

    if(!QFile::exists(startupiniPath))
    {
        QFile::copy("assets:/startup.ini",startupiniPath);
        QFile fileDest(startupiniPath);
        fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    }

    QString versioniniPath = QString("%1/skyside3gby3/version.ini").arg(externalStorageDir.toString());
    if(QFile::exists(versioniniPath))//下个版本需要取消注释
    {
        QFile::remove(versioniniPath);
    }
    QFile::copy("assets:/version.ini",versioniniPath);
    QFile fileDest(versioniniPath);
    fileDest.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);

    //QString modveriniPath = QString("%1/skyside3gby3/modver.ini").arg(externalStorageDir.toString());
    //QFile::copy("assets:/modver.ini",versioniniPath);
    //QFile fileDest1(modveriniPath);
    //fileDest1.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::WriteGroup|QFile::ExeGroup|QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);


#endif
   QApplication a(argc, argv);
    //MainWindow m;
    //m.show();

   int fontId = QFontDatabase::addApplicationFont(":/fonts/DroidSansFallback.ttf");
   //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
   androidFont = QFontDatabase::applicationFontFamilies(fontId).at(0);
   QFont font(androidFont);
   QGuiApplication::setFont(font);

#if defined(Q_OS_ANDROID)
   startUpIniPath = startupiniPath ;
   versionIniPath = versioniniPath ;
   //modverIniPath = modveriniPath ;
#elif defined(Q_OS_IOS)
   startUpIniPath = QDir::homePath() +"/Documents/srartup.ini";
   versionIniPath = QDir::homePath() +"/Documents/version.ini";
   //modverIniPath = "modver.ini";
#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
   startUpIniPath ="startup.ini";
   versionIniPath ="version.ini";
   //modverIniPath = "modver.ini";
#endif



    QSettings *configIniStartUp = new QSettings(startUpIniPath, QSettings::IniFormat);
    QSettings *configIniVersion = new QSettings(versionIniPath, QSettings::IniFormat);
    configIniVersion->setIniCodec("GB2312");


    //20190502先读取StartUp中mod配置文件
    g_useMOD = 0;//configIniStartUp->value("/Mod/UseMod",0).toInt();


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
    g_layerKey = configIniStartUp->value("/Startup/LayerKey",0).toInt();
    g_fullScreen = configIniStartUp->value("/Startup/FullScreen",0).toInt();

#if defined(Q_OS_ANDROID)
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>( "getRequestedOrientation" );
    if(env->ExceptionCheck())
    {

        env->ExceptionClear();
    }


    activity.callMethod<void>("setRequestedOrientation", "(I)V", g_gameviewdirect);
    if(env->ExceptionCheck())
    {

        env->ExceptionClear();
    }

#elif defined(Q_OS_IOS)
    routeView(g_gameviewdirect);

#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    g_gameviewdirect = 0;//0;//0-横屏 1-竖屏
#endif
    delete configIniStartUp;
    delete configIniVersion;



    #if defined(Q_OS_WIN32)||defined(Q_OS_WASM)


        QString version;
        version.sprintf(" ver%d.%01d.%01d",version_now%1000/100,version_now%100/10,version_now%10);
        title = title + version;

        QIcon icon(":/images/icon1.ico");//"Res/icon1.ico");
        getGlobalWidget()->setWindowIcon(icon);
        getGlobalWidget()->setWindowTitle(title);

        //窗口大小
        if(screen == 0)
        {
            g_screenwidth = widthx;
            g_screenheight = heighty;
            getGlobalWidget()->setFixedSize(g_screenwidth,g_screenheight);
        }
        else
        {
            QDesktopWidget *deskWgt = QApplication::desktop();
            if (deskWgt)
            {
                    g_screenwidth = deskWgt->width();
                    g_screenheight = deskWgt->height()-100;
            }
            getGlobalWidget()->setFixedSize(g_screenwidth,g_screenheight);

            getGlobalWidget()->showFullScreen();
        }


    #elif defined(Q_OS_ANDROID)



        QDesktopWidget *deskWgt = QApplication::desktop();
        if (deskWgt)
        {
                g_screenwidth = deskWgt->width();
                g_screenheight = deskWgt->height();//-100;//去除状态栏影响
        }

        if(g_gameviewdirect == 0)
        {
            int tmp1 = g_screenwidth;
            g_screenwidth = g_screenheight;
            g_screenheight = tmp1;
        }

         getGlobalWidget()->setFixedSize(g_screenwidth,g_screenheight);



    #elif defined(Q_OS_IOS)
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
        getGlobalWidget()->setFixedSize(g_screenwidth,g_screenheight);
    #endif



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
        g_gameviewleft += (screen * 50);//20180713
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

        //20180810
        g_gameviewlefta = g_gameviewleft;
        g_gameviewrighta = g_gameviewright;

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
        g_gameviewleft += (screen * 50);//20180713
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


        /*
        if(g_screenwidth *3/5>= g_screenheight)
        {
            g_gameviewdirect = 0;
            g_gameviewleft = (g_screenwidth - g_screenheight*5/3 )/2;
            if(g_gameviewleft < 100) g_gameviewleft = 100;
            g_gameviewright = g_screenwidth - g_gameviewleft;

            g_screentimes = (double)g_screenheight/96.0;//左右有空间
            g_gameviewtop =0;
            g_gameviewbottom = g_screenheight;
        }
        else
        {
            g_gameviewdirect = 1;
            g_screentimes = (double)g_screenwidth/160.0;//上下有空间
            g_gameviewleft = 0;
            g_gameviewright = g_screenwidth;
            g_gameviewtop = 0;
            g_gameviewbottom = g_screenwidth*3/5;
        }*/

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


    getGlobalWidget()->show();
    getGlobalWidget()->grabGesture(Qt::PinchGesture);
    getGlobalWidget()->grabGesture(Qt::TapAndHoldGesture);

     NewMenuWindow mainWindow;
    mainWindow.resize(400, 300);
    mainWindow.show();

    return a.exec();
}
