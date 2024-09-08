#include "gameview.h"
#include "global.h"
//extern PaintWidget paintWidget;
#include "PicConverter.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QList>
#include <QUuid>
#include <sys/time.h>
#include "tcpclient.h"
#include <QDir>
#include <QTextCodec>
#define OK          0
#define TCPERROR    1
#define FILEERROR   2
#define DATAERROR   3
#define OTHERERROR  4


#if defined (Q_OS_ANDROID)
#include<QtAndroidExtras/QAndroidJniObject>
#include<QtAndroidExtras/QAndroidJniEnvironment>
#include<QtAndroidExtras/QtAndroid>
#endif
extern int g_screenwidth;
extern int g_screenheight;
extern double g_screentimes;
extern int g_showPicType;

extern int g_ResId;
extern int g_PicId;
extern int g_PicStyle;
extern int g_Fontsize;
extern QColor g_backColor;

int g_StoredResId[1500];
int g_StoredPicId[1500];
int g_StoredPicStyle[1500];
QImage g_StoredQImage[1500];
QImage * g_cityMap;
QImage * g_fightMap;
int g_StoredMax = 0;
extern U8	*c_VisScr;		/*虚拟屏幕指针*/

TcpClient *tcpclient;

typedef struct PictureHeadJPG
{
    U32 size;		/*图片大小*/
}PictureHeadTypeJPG;

U8 SysLcdPartClear(U8 l,U8 t,U8 r,U8 b)
{
    if(g_showPicType == 0)
    {
        getGlobalWidget()->getPainter()->setBrush(g_backColor);
        getGlobalWidget()->getPainter()->setPen(QPen(g_backColor, 1));
        getGlobalWidget()->getPainter()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
    }
    else
    {
        getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81));
        getGlobalWidget()->getPainter()->setPen(QPen(QColor(198,151,81), 1));
        getGlobalWidget()->getPainter()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);

    }
    return 0;
}

QImage GetQImage(uchar *pic,U8 wid,U8 hgt)
{
    int picStored = 0;
    for(int t_i = 0; t_i < g_StoredMax ; t_i ++)
    {
        if(g_ResId == g_StoredResId[t_i] && g_PicId == g_StoredPicId[t_i] && g_PicStyle == g_StoredPicStyle[t_i])
        {
            picStored = t_i;
            break;
        }
    }

    if(picStored == 0)
    {
        PicConverter *picConverter = new PicConverter();
        QImage image = picConverter->BytesToHBitmap(pic,wid,hgt);
        g_StoredQImage[g_StoredMax] = image.copy();
        g_StoredResId[g_StoredMax] = g_ResId;
        g_StoredPicId[g_StoredMax] = g_PicId;
        g_StoredPicStyle[g_StoredMax] = g_PicStyle;
        g_StoredMax ++;
        if(g_StoredMax>=1500)g_StoredMax = 0;
        delete(picConverter);
        return image;
    }
    else
    {
        QImage image = g_StoredQImage[picStored];
        return image;
    }
}

QImage GetQImage_Color(uchar *pic,U16 wid,U16 hgt)
{
    int picStored = 0;
   // DebugTime(71);
    for(int t_i = 0; t_i < g_StoredMax ; t_i ++)
    {
        if(g_ResId == g_StoredResId[t_i] && g_PicId == g_StoredPicId[t_i] && g_PicStyle == g_StoredPicStyle[t_i])
        {
            picStored = t_i;
            break;
        }
    }
//DebugTime(72);
    if(picStored == 0)
    {
        PicConverter *picConverter = new PicConverter();
        QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);
        //DebugTime(73);
        g_StoredQImage[g_StoredMax] = image.copy();
        g_StoredResId[g_StoredMax] = g_ResId;
        g_StoredPicId[g_StoredMax] = g_PicId;
        g_StoredPicStyle[g_StoredMax] = g_PicStyle;
        g_StoredMax ++;
        if(g_StoredMax>=1500)g_StoredMax = 0;
        delete(picConverter);
        //DebugTime(74);
        return image;
    }
    else
    {
        QImage image = g_StoredQImage[picStored];
        //DebugTime(75);
        return image;
    }
}

QImage GetQImage_JPG(uchar *pic,U32 size)
{
    int picStored = 0;
   // DebugTime(71);
    for(int t_i = 0; t_i < g_StoredMax ; t_i ++)
    {
        if(g_ResId == g_StoredResId[t_i] && g_PicId == g_StoredPicId[t_i] && g_PicStyle == g_StoredPicStyle[t_i])
        {
            picStored = t_i;
            break;
        }
    }
//DebugTime(72);
    if(picStored == 0)
    {
        PicConverter *picConverter = new PicConverter();
        QImage image = picConverter->BytesToHBitmap_JPG(pic,size);
        //DebugTime(73);
        g_StoredQImage[g_StoredMax] = image.copy();
        g_StoredResId[g_StoredMax] = g_ResId;
        g_StoredPicId[g_StoredMax] = g_PicId;
        g_StoredPicStyle[g_StoredMax] = g_PicStyle;
        g_StoredMax ++;
        if(g_StoredMax>=1500)g_StoredMax = 0;
        delete(picConverter);
        //DebugTime(74);
        return image;
    }
    else
    {
        QImage image = g_StoredQImage[picStored];
        //DebugTime(75);
        return image;
    }
}

U8 GamePictureDummy(U16 l,U16 t,U16 r,U16 b,U8* pic,U8* vscr,U8 mode)
{
    //mode = 0 正常 1  & 2 | 4 clear 9 文字
    //QPicture qpic = new QPicture();
    U16 wid = r - l + 1;
    U16 hgt = b - t + 1;
    if(g_showPicType == 0)
    {
        if(mode == 0)
        {
            //PicConverter *picConverter = new PicConverter();
            //picConverter = new PicConverter();
            g_PicStyle = 0;
            QImage image = GetQImage(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            //delete(picConverter);
        }
        else if(mode == 2)// &
        {
            //PicConverter picConverter;
            //PicConverter *picConverter = new PicConverter();
            //picConverter = new PicConverter();
            g_PicStyle = 2;
            QImage image = GetQImage(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceAndDestination);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //delete(picConverter);
        }
        else if(mode == 1) //|
        {
            //PicConverter picConverter;
            //PicConverter *picConverter = new PicConverter();
            //picConverter = new PicConverter();
            g_PicStyle = 1;
            QImage image = GetQImage(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceOrDestination);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //delete(picConverter);
        }
        else if(mode== 4)//清屏
        {
            getGlobalWidget()->getPainterMem()->setBrush(g_backColor);
            getGlobalWidget()->getPainterMem()->setPen(QPen(g_backColor, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
        }
        else if(mode == 9)//文字
        {
            U8  tmpc[30+1];
            memcpy(tmpc+6,pic,24);
            tmpc[0] = 0x10;
            tmpc[1] = 0;
            tmpc[2] = 0x0c;
            tmpc[3] = 0;
            tmpc[4] = 0;
            tmpc[5] = 0;
            //PicConverter picConverter;
            PicConverter *picConverter = new PicConverter();
            QImage image = picConverter->BytesToHBitmap_Str(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            delete(picConverter);
        }
        else if(mode == 10)
        {
            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*g_screentimes,(b+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_cityMap->copy(l*g_screentimes,t*g_screentimes,(r+1)*g_screentimes,(b+1)*g_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
        else if(mode == 11)
        {
            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*g_screentimes,(b+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_fightMap->copy(l*g_screentimes,t*g_screentimes,(r+1)*g_screentimes,(b+1)*g_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
    }
    else if(g_showPicType == 1)
    {

        double t_screentimes = g_screentimes/5;
        if(mode == 0)
        {
            //PicConverter picConverter;
            PicConverter *picConverter = new PicConverter();
            g_PicStyle = 0;
            QImage image = GetQImage_Color(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);
            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);;
            delete(picConverter);
        }
        else if(mode == 2)// &
        {
            //PicConverter picConverter;
            //DebugTime(66);
            //PicConverter *picConverter = new PicConverter();
            g_PicStyle = 2;
            //DebugTime(67);
            QImage image = GetQImage_Color(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片

//DebugTime(68);
            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceAndDestination);
            //DebugTime(69);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            //DebugTime(610);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //DebugTime(611);
            //delete(picConverter);
        }
        else if(mode == 1) //|
        {
            //PicConverter picConverter;
            //DebugTime(61);
            //PicConverter *picConverter = new PicConverter();
            g_PicStyle = 1;
            QImage image = GetQImage_Color(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片

//DebugTime(62);
            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceOrDestination);
           //DebugTime(63);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            //DebugTime(64);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //DebugTime(65);
            //delete(picConverter);
        }
        else if(mode== 4)//清屏
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));
            getGlobalWidget()->getPainterMem()->setPen(QPen(QColor(198,151,81), 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
        }
        else if(mode == 9)//文字
        {
            U8  tmpc[30+1];
            memcpy(tmpc+6,pic,24);
            tmpc[0] = 0x10;
            tmpc[1] = 0;
            tmpc[2] = 0x0c;
            tmpc[3] = 0;
            tmpc[4] = 0;
            tmpc[5] = 0;
            //PicConverter picConverter;
            PicConverter *picConverter = new PicConverter();
            QImage image = picConverter->BytesToHBitmap_Str(pic,wid,hgt);
            QImage mask = image.createMaskFromColor(qRgb(255,255,255),Qt::MaskOutColor);
            image.setAlphaChannel(mask);
            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawRect(rect);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            delete(picConverter);
        }
        else if(mode == 10)
        {
            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*t_screentimes,(b+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_cityMap->copy(l*t_screentimes,t*t_screentimes,(r+1)*t_screentimes,(b+1)*t_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
        else if(mode == 11)
        {
            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*t_screentimes,(b+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_fightMap->copy(l*t_screentimes,t*t_screentimes,(r+1)*t_screentimes,(b+1)*t_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
    }
    else if(g_showPicType == 2)//jpg
    {

        double t_screentimes = g_screentimes/5;
        if(mode == 0)
        {
            //U32 size = (U32)l*10000 + (U32)t;//图片大小
            U32 size  = ((PictureHeadTypeJPG *)pic)->size;//size不包含4
            pic +=4;
            //PicConverter picConverter;
            PicConverter *picConverter = new PicConverter();
            g_PicStyle = 0;
            QImage image = GetQImage_JPG(pic,size);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);
            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);;
            delete(picConverter);
        }
        else if(mode == 2)// &
        {
            //U32 size = (U32)l*10000 + (U32)t;//图片大小
            U32 size  = ((PictureHeadTypeJPG *)pic)->size;//size不包含4
            pic +=4;
            //PicConverter picConverter;
            //DebugTime(66);
            //PicConverter *picConverter = new PicConverter();
            g_PicStyle = 2;
            //DebugTime(67);
            QImage image = GetQImage_JPG(pic,size);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片

//DebugTime(68);
            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceAndDestination);
            //DebugTime(69);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            //DebugTime(610);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //DebugTime(611);
            //delete(picConverter);
        }
        else if(mode == 1) //|
        {
            //U32 size = (U32)l*10000 + (U32)t;//图片大小
            U32 size  = ((PictureHeadTypeJPG *)pic)->size;//size不包含4
            pic +=4;
            //PicConverter picConverter;
            //DebugTime(61);
            //PicConverter *picConverter = new PicConverter();
            g_PicStyle = 1;
            QImage image = GetQImage_JPG(pic,size);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片

//DebugTime(62);
            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes,(b-t+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::RasterOp_SourceOrDestination);
           //DebugTime(63);
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            //DebugTime(64);
            getGlobalWidget()->getPainterMem()->setCompositionMode(QPainter::CompositionMode_SourceOver);
            //DebugTime(65);
            //delete(picConverter);
        }
        else if(mode== 4)//清屏
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));
            getGlobalWidget()->getPainterMem()->setPen(QPen(QColor(198,151,81), 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
        }
        else if(mode == 9)//文字
        {
            U8  tmpc[30+1];
            memcpy(tmpc+6,pic,24);
            tmpc[0] = 0x10;
            tmpc[1] = 0;
            tmpc[2] = 0x0c;
            tmpc[3] = 0;
            tmpc[4] = 0;
            tmpc[5] = 0;
            //PicConverter picConverter;
            PicConverter *picConverter = new PicConverter();
            QImage image = picConverter->BytesToHBitmap_Str(pic,wid,hgt);
            QImage mask = image.createMaskFromColor(qRgb(255,255,255),Qt::MaskOutColor);
            image.setAlphaChannel(mask);
            //QImage image1;
            //image1.load("05.sbp");     //用QImage导入图片


            //QRect rect(l,t,r-l+1,b-t+1);  //创建绘图区域
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterMem()->drawRect(rect);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
            delete(picConverter);
        }
        else if(mode == 10)
        {

            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*t_screentimes,(b+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_cityMap->copy(l*t_screentimes,t*t_screentimes,(r+1)*t_screentimes,(b+1)*t_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
        else if(mode == 11)
        {
            //PicConverter picConverter;
            g_PicStyle = 0;

            QRect rect(0,0,(r+1)*t_screentimes,(b+1)*t_screentimes);
            //image.save("a.bmp","BMP");
            getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            QImage image = g_fightMap->copy(l*t_screentimes,t*t_screentimes,(r+1)*t_screentimes,(b+1)*t_screentimes);
            getGlobalWidget()->getPainterMem()->drawImage(rect,image);
        }
    }
    //getGlobalWidget()->getPainter()->drawImage(rect,image1);
    //getGlobalWidget()->getPainter()->setCompositionMode(QPainter::CompositionMode_SourceIn);
    return 0;
}

U8 SysLcdReverse(U8 l,U8 t,U8 r,U8 b)
{

    getGlobalWidget()->getPainter()->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    getGlobalWidget()->getPainter()->setPen(QPen(Qt::black, 1,Qt::NoPen));
    if(g_showPicType > 0)
    {
        getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81));
    }
    else
    {
        getGlobalWidget()->getPainter()->setBrush(g_backColor);
    }
    getGlobalWidget()->getPainter()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
    getGlobalWidget()->getPainter()->setCompositionMode(QPainter::CompositionMode_SourceOver);
    //getGlobalWidget()->getPainter()->
    return 0;
}

U8 SysPutPixel(U8 x,U8 y,U8 c)
{
    if(g_showPicType > 0)
    {
        getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81));
    }
    else
    {
        getGlobalWidget()->getPainter()->setBrush(g_backColor);
    }
    getGlobalWidget()->getPainter()->drawPoint(x*g_screentimes,y*g_screentimes);
    return 0;
}

U8 SysLine(U8 l,U8 t,U8 r,U8 b)
{
    getGlobalWidget()->getPainter()->drawLine(l*g_screentimes,t*g_screentimes,r*g_screentimes,b*g_screentimes);
    return 0;
}

U8 SysLineClear(U8 l,U8 t,U8 r,U8 b)
{
    getGlobalWidget()->getPainter()->setPen(QPen(g_backColor, g_screentimes));
    getGlobalWidget()->getPainter()->drawLine(l*g_screentimes,t*g_screentimes,r*g_screentimes,b*g_screentimes);
    return 0;
}

U8 SysRect(U8 l,U8 t,U8 r,U8 b)
{
    if(g_showPicType == 0)
    {
        getGlobalWidget()->getPainter()->setPen(QPen(Qt::black, g_screentimes));
        getGlobalWidget()->getPainter()->setCompositionMode(QPainter::RasterOp_SourceAndDestination);
        getGlobalWidget()->getPainter()->setBrush(g_backColor);
    }
    else
    {
        getGlobalWidget()->getPainter()->setPen(QPen(QColor(180,106,41), g_screentimes));
        getGlobalWidget()->getPainter()->setCompositionMode(QPainter::RasterOp_SourceAndDestination);
        getGlobalWidget()->getPainter()->setBrush(Qt::NoBrush);
    }

    //getGlobalWidget()->getPainter()->setBrush(Qt::transparent);

    getGlobalWidget()->getPainter()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
    getGlobalWidget()->getPainter()->setCompositionMode(QPainter::CompositionMode_SourceOver);
    return 0;
}

U8 SysRectClear(U8 l,U8 t,U8 r,U8 b)
{

    if(g_showPicType > 0)
    {
        getGlobalWidget()->getPainter()->setPen(QPen(QColor(198,151,81), g_screentimes));
        getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81));
    }
    else
    {
        getGlobalWidget()->getPainter()->setPen(QPen(g_backColor, g_screentimes));
        getGlobalWidget()->getPainter()->setBrush(g_backColor);
    }
    getGlobalWidget()->getPainter()->drawRect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
    return 0;
}

U8 SysGetKey()
{
    return 0;
}

U8 SysCopyToDc()
{
    getGlobalWidget()->copyToDc();
    return 0;
}

U8 SysCopyToDcStr()
{
    getGlobalWidget()->copyToDcStr();
    return 0;
}

U8 SysCopyFromDcToStr()
{
    getGlobalWidget()->copyFromDcToStr();
    return 0;
}

U8 SysPicture(U16 l,U16 t,U16 r,U16 b,U8 * pic,U8 zero)
{
    U16 wid = r - l + 1;
    U16 hgt = b - t + 1;
    //QPicture qpic = new QPicture();
    //PicConverter *picConverter = new PicConverter();
    //picConverter = new PicConverter();
    if(zero == 9)//文字
    {
        U8  tmpc[30+1];
        memcpy(tmpc+6,pic,24);
        tmpc[0] = 0x10;
        tmpc[1] = 0;
        tmpc[2] = 0x0c;
        tmpc[3] = 0;
        tmpc[4] = 0;
        tmpc[5] = 0;
        PicConverter *picConverter = new PicConverter();
        QImage image = picConverter->BytesToHBitmap_Str(pic,wid,hgt);

        //image.load("05.sbp");     //用QImage导入图片
        QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);

        if(g_showPicType >0)
        {
            QImage mask = image.createMaskFromColor(qRgb(255,255,255),Qt::MaskOutColor);
            image.setAlphaChannel(mask);

            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81));
            getGlobalWidget()->getPainterStr()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainterStr()->drawRect(rect);
        }

        getGlobalWidget()->getPainterStr()->drawImage(rect,image);
        delete(picConverter);
    }
    else
    {
        if(g_showPicType == 0)
        {
            g_PicStyle = 0;
            QImage image = GetQImage(pic,wid,hgt);
           // QImage image = picConverter->BytesToHBitmap(pic,wid,hgt);

            //image.load("05.sbp");     //用QImage导入图片
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes,(b-t+1)*g_screentimes);
            getGlobalWidget()->getPainter()->setPen(QPen(Qt::black, 1,Qt::NoPen));
            getGlobalWidget()->getPainter()->drawImage(rect,image);
            }
        else if(g_showPicType == 1)
        {
            g_PicStyle = 0;
            QImage image = GetQImage_Color(pic,wid,hgt);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //image.load("05.sbp");     //用QImage导入图片
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes/5,(b-t+1)*g_screentimes/5);
            getGlobalWidget()->getPainter()->setPen(QPen(Qt::black, 1,Qt::NoPen));

            getGlobalWidget()->getPainter()->drawImage(rect,image);

        }
        else if(g_showPicType == 2)
        {
            g_PicStyle = 0;
            U32 size  = ((PictureHeadTypeJPG *)pic)->size;
            pic+=4;
            QImage image = GetQImage_JPG(pic,size);
            //QImage image = picConverter->BytesToHBitmap_Color(pic,wid,hgt);

            //image.load("05.sbp");     //用QImage导入图片
            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes/5,(b-t+1)*g_screentimes/5);
            getGlobalWidget()->getPainter()->setPen(QPen(Qt::black, 1,Qt::NoPen));

            getGlobalWidget()->getPainter()->drawImage(rect,image);

        }
    }
    return 0;
}
//--------------------------------

U8 SysAscii(U32 posx,U32 posy,U8 * ascii,U8 trans)
{
    if(c_VisScr==NULL)
    {
        QString str = QString::fromLocal8Bit((char *)ascii);

        QFont font;
        font.setPointSize(g_Fontsize);
        getGlobalWidget()->getPainterStr()->setFont(font);

        QRect rect(posx*g_screentimes,posy*g_screentimes,(6)*g_screentimes,(12)*g_screentimes);
        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81,0));

        }
        else if(g_showPicType >0)
        {
            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81));

        }
        else
        {
            getGlobalWidget()->getPainterStr()->setBrush(g_backColor);
        }

        getGlobalWidget()->getPainterStr()->setPen(QPen(Qt::black, 1,Qt::NoPen));
        getGlobalWidget()->getPainterStr()->drawRect(rect);

        getGlobalWidget()->getPainterStr()->setPen(Qt::black);
        getGlobalWidget()->getPainterStr()->drawText(rect,str);
    }
    else
    {
        QString str = QString::fromLocal8Bit((char *)ascii);

        QFont font;
        font.setPointSize(g_Fontsize);
        getGlobalWidget()->getPainterMem()->setFont(font);

        QRect rect(posx*g_screentimes,posy*g_screentimes,(6)*g_screentimes,(12)*g_screentimes);
        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81,0));

        }
        else if(g_showPicType >0)
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));

        }
        else
        {
            getGlobalWidget()->getPainterMem()->setBrush(g_backColor);
        }

        getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
        getGlobalWidget()->getPainterMem()->drawRect(rect);
        getGlobalWidget()->getPainterMem()->setPen(Qt::black);
        getGlobalWidget()->getPainterMem()->drawText(rect,str);


    }
    return 0;
}

U8 SysChinese(U32 posx,U32 posy,U8 * ascii,U8 trans)
{
    if(c_VisScr==NULL)
    {
        //QString::from






        if(ascii[0] == 162 && ascii[1] == 227)//李【傕】
        {
            ascii[0] = 0x82 ;
            ascii[1] = 0xe0;
        }
        if(ascii[0] == 162 && ascii[1] == 228)//荀【彧】
        {
            ascii[0] = 0x8f ;
            ascii[1] = 0xaa;
        }
        if(ascii[0] == 162 && ascii[1] == 239)//夏侯【惇】
        {
            ascii[0] = 0x90 ;
            ascii[1] = 0xaa;
        }
        if(ascii[0] == 165 && ascii[1] == 247)//费【祎】
        {
            ascii[0] = 0xb5 ;
            ascii[1] = 0x74;
        }
        if(ascii[0] == 165 && ascii[1] == 248)//张【郃】
        {
            ascii[0] = 0xe0 ;
            ascii[1] = 0x41;
        }
        if(ascii[0] == 165 && ascii[1] == 249)//毛【玠】
        {
            ascii[0] = 0xab ;
            ascii[1] = 0x64;
        }
        if(ascii[0] == 165 && ascii[1] == 250)//严【畯】
        {
            ascii[0] = 0xae ;
            ascii[1] = 0x8f;
        }



        QString str = QString::fromLocal8Bit((char *)ascii);

        QFont font;
        font.setPointSize(g_Fontsize);
        getGlobalWidget()->getPainterStr()->setFont(font);

        QRect rect(posx*g_screentimes,posy*g_screentimes,(12)*g_screentimes,(12)*g_screentimes);

        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81,0));

        }
        else if(g_showPicType >0)
        {


            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81));

        }
        else
        {
            getGlobalWidget()->getPainterStr()->setBrush(g_backColor);
        }
        getGlobalWidget()->getPainterStr()->setPen(QPen(Qt::black, 1,Qt::NoPen));
        getGlobalWidget()->getPainterStr()->drawRect(rect);
        getGlobalWidget()->getPainterStr()->setPen(Qt::black);
        getGlobalWidget()->getPainterStr()->drawText(rect,str);
    }
    else
    {
        if(ascii[0] == 162 && ascii[1] == 227)//李【傕】
        {
            ascii[0] = 0x82 ;
            ascii[1] = 0xe0;
        }
        if(ascii[0] == 162 && ascii[1] == 228)//荀【彧】
        {
            ascii[0] = 0x8f ;
            ascii[1] = 0xaa;
        }
        if(ascii[0] == 162 && ascii[1] == 239)//夏侯【惇】
        {
            ascii[0] = 0x90 ;
            ascii[1] = 0xaa;
        }
        if(ascii[0] == 165 && ascii[1] == 247)//费【祎】
        {
            ascii[0] = 0xb5 ;
            ascii[1] = 0x74;
        }
        if(ascii[0] == 165 && ascii[1] == 248)//张【郃】
        {
            ascii[0] = 0xe0 ;
            ascii[1] = 0x41;
        }
        if(ascii[0] == 165 && ascii[1] == 249)//毛【玠】
        {
            ascii[0] = 0xab ;
            ascii[1] = 0x64;
        }
        if(ascii[0] == 165 && ascii[1] == 250)//严【畯】
        {
            ascii[0] = 0xae ;
            ascii[1] = 0x8f;
        }

        QString str = QString::fromLocal8Bit((char *)ascii);

        QFont font;
        font.setPointSize(g_Fontsize);

        getGlobalWidget()->getPainterMem()->setFont(font);

        QRect rect(posx*g_screentimes,posy*g_screentimes,(12)*g_screentimes,(12)*g_screentimes);

        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81,0));

        }
        else if(g_showPicType >0)
        {


            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));

        }
        else
        {
            getGlobalWidget()->getPainterMem()->setBrush(g_backColor);
        }
        getGlobalWidget()->getPainterMem()->setPen(QPen(Qt::black, 1,Qt::NoPen));
        getGlobalWidget()->getPainterMem()->drawRect(rect);
        getGlobalWidget()->getPainterMem()->setPen(Qt::black);
        getGlobalWidget()->getPainterMem()->drawText(rect,str);


    }
    return 0;

}

U8 FileNum(U8 filetype,U16 * filenum)
{
    int i_filenum = 1;
    if(filetype==GAME_SAVE)
    {
        i_filenum = 3;
    }
    filenum =  (U16 *)i_filenum;
    return 0;
}

U8 FileSearch(U8 filetype,U8 fileno,U8 * filename,U8 *out_filename)
{
#if defined(Q_OS_IOS)
    QString qfname = QString(QLatin1String((const char *)filename));
    qfname = QDir::homePath() + "/Documents/"+qfname;
    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    FILE *fp = fopen(tcharname,"r");//二进制读，追加
    if(fp == NULL)
    {
      //out_filename = '\0';
    }
    fclose(fp);
#elif defined(Q_OS_ANDROID)
    QString qfname = QString(QLatin1String((const char *)filename));

    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    qfname = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) + qfname;

    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    FILE *fp = fopen(tcharname,"r");//二进制读，追加
    if(fp == NULL)
    {
      //out_filename = '\0';
    }
    fclose(fp);
#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    FILE *fp = fopen((char *)filename,"r");//二进制读，追加
    if(fp == NULL)
    {
      //out_filename = '\0';
    }
    fclose(fp);
#endif
    return 0;
}

FILE *FileOpen(U8 * filename,U8 filetype,const char *openmode,FILE *filehandle,U32 *filelen)
{
    //FileOpen(tmpName,GAME_SAVE,mode,&pFHandle,&pLen);
#if defined(Q_OS_IOS)
    QString qfname = QString(QLatin1String((const char *)filename));
    qfname = QDir::homePath() + "/Documents/"+qfname;

    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    filehandle=fopen(tcharname,(const char *)openmode);
#elif defined(Q_OS_ANDROID)
    QString qfname = QString(QLatin1String((const char *)filename));
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    qfname = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) + qfname;

    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    filehandle=fopen(tcharname,(const char *)openmode);
#elif defined(Q_OS_LINUX)
    QString qfname = QString(QLatin1String((const char *)filename));
    qfname = QDir::currentPath() + "/" +qfname;

    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    filehandle=fopen(tcharname,(const char *)openmode);

#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode= QDir::currentPath() + "/" + utf8Codec->toUnicode((char *)filename);
    QByteArray ByteGb2312= gb2312Codec ->fromUnicode(strUnicode);
    char * aname=  ByteGb2312.data();
    //filename
    filehandle=fopen(aname,(const char *)openmode);

    //filehandle=fopen((const char *)filename,(const char *)openmode);

#endif
    if(filehandle == NULL)return NULL;
    fseek((FILE *)filehandle,0L,SEEK_END);
    int len = ftell((FILE *)filehandle);
    fseek((FILE *)filehandle,0L,SEEK_SET);
    * filelen = (U32)len;
    return filehandle;
}

U8 FileDel(gam_FILE * fhandle)
{
    fclose((FILE *)fhandle->handle);
#if defined(Q_OS_IOS)
    QString qfname = QString(QLatin1String((const char *)fhandle->fname));
    qfname = QDir::homePath() + "/Documents/"+qfname;
    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    remove((char *)tcharname);
#elif defined(Q_OS_ANDROID)
    QString qfname = QString(QLatin1String((const char *)fhandle->fname));
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    qfname = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) + qfname;
    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    remove((char *)tcharname);
#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    remove((char *)fhandle->fname);
#endif
    return 0;
}

FILE *FileCreat(U8 filetype,U32 filelen,U8 *filename,FILE *filehandle)
{
#if defined(Q_OS_IOS)
    QString qfname = QString(QLatin1String((const char *)filename));
    qfname = QDir::homePath() + "/Documents/"+qfname;
    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    filehandle=fopen(tcharname,"wb+");
#elif defined(Q_OS_ANDROID)
    QString qfname = QString(QLatin1String((const char *)filename));
    QAndroidJniObject externalStorageDir = QAndroidJniObject::callStaticObjectMethod(
                    "android/os/Environment",
                    "getExternalStorageDirectory",
                    "()Ljava/io/File;"
                    );

    qfname = QString("%1/skyside3gby3/").arg(externalStorageDir.toString()) + qfname;
    QByteArray ba = qfname.toLatin1(); // must
    char *tcharname;
    tcharname=ba.data();
    filehandle=fopen(tcharname,"wb+");
#elif defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    filehandle=fopen((const char *)filename,"wb+");
#endif
    //FileCreat(GAME_SAVE,pLen,fname,&tmpName,&pFHandle);

    fseek((FILE *)filehandle,0L,SEEK_END);
    int len = ftell((FILE *)filehandle);
    fseek((FILE *)filehandle,0L,SEEK_SET);
    filelen = (U32)len;
    return filehandle;
}

U8 FileClose(gam_FILE * fhandle)
{
    fclose((FILE *)fhandle->handle);
    return 0;
}


U8 FileRead(FILE * fhandle,U8 len,U8* outptr)
{
    fread(outptr,1,(int)len,fhandle);
    return 0;
}

U8 FileWrite(FILE * fhandle,U8 len,U8* outptr)
{
    //FILE *fp;
    //fp = fopen("a","ab+");
    //fwrite((char *)outptr,1,(int)len,fp);
    fwrite((char *)outptr,1,(int)len,fhandle);
    //fclose(fp);
    return 0;
}

U8 FileChangeInf(gam_FILE *file,U8* newname)
{
    rename((char *)file->fname,(char *)newname);
    return 0;
}

void DebugTime(int a)
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    char sss[100+1];
    sprintf(sss,"time %ld:%ld",tv.tv_sec,tv.tv_usec);

    qDebug()<<a<<" "<<sss<<"s";
}

U8 GamePictureSave(U16 l,U16 t,U16 r,U16 b,U8* pic,U8* vscr,U8 mode)
{
    //mode = 0 正常 1  & 2 | 4 clear 9 文字
    //QPicture qpic = new QPicture();
    U16 wid = r - l + 1;
    U16 hgt = b - t + 1;

    if(g_showPicType == 0)
    {
        if(mode == 0)
        {
            QImage image = GetQImage(pic,wid,hgt);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes+1,(b-t+1)*g_screentimes+1);

            QPainter citypainter(g_cityMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }
        else if(mode == 1)
        {
            QImage image = GetQImage(pic,wid,hgt);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*g_screentimes+1,(b-t+1)*g_screentimes+1);

            QPainter citypainter(g_fightMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }

    }
    else if(g_showPicType == 1)
    {
        double t_screentimes = g_screentimes/5;
        if(mode == 0)
        {
            QImage image = GetQImage_Color(pic,wid,hgt);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes+1,(b-t+1)*t_screentimes+1);

            QPainter citypainter(g_cityMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }
        else if(mode == 1)
        {
            QImage image = GetQImage_Color(pic,wid,hgt);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes+1,(b-t+1)*t_screentimes+1);

            QPainter citypainter(g_fightMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }

    }
    else if(g_showPicType == 2)
    {
        U32 size  = ((PictureHeadTypeJPG *)pic)->size;
        pic +=4;

        double t_screentimes = g_screentimes/5;
        if(mode == 0)
        {
            QImage image = GetQImage_JPG(pic,size);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes+1,(b-t+1)*t_screentimes+1);

            QPainter citypainter(g_cityMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }
        else if(mode == 1)
        {
            QImage image = GetQImage_JPG(pic,size);

            QRect rect(l*g_screentimes,t*g_screentimes,(r-l+1)*t_screentimes+1,(b-t+1)*t_screentimes+1);

            QPainter citypainter(g_fightMap);
            citypainter.drawImage(rect,image);
            citypainter.end();
        }

    }
    //getGlobalWidget()->getPainter()->drawImage(rect,image1);
    //getGlobalWidget()->getPainter()->setCompositionMode(QPainter::CompositionMode_SourceIn);
    return 0;
}

U8 NewTCPClient()
{
    tcpclient = new TcpClient();
    return 0;
}

U8 TCPisConnected()
{
    if(tcpclient==NULL)
    {
        return TCPERROR;
    }
    if( tcpclient->isConnected())
    {
        return OK;
    }
    return TCPERROR;
}


U8 TCPWriteData(char * senddata,int dataLen)
{
    tcpclient->writeData(senddata,dataLen);
    return 0;
}

U8 TCPReadData(char * rcvdata,int dataLen)
{
    char tmpdata[65536];
    memset(tmpdata,0,sizeof(tmpdata));
    int curlen = 0;
    QByteArray qbar = tcpclient->readData();
    curlen = qbar.length();
    memcpy(tmpdata,(char *)qbar.data(),qbar.length());
    while(curlen < dataLen)
    {

        qbar = tcpclient->readData();

        memcpy(tmpdata+curlen,(char *)qbar.data(),qbar.length());
        curlen +=qbar.length();
    }

    memcpy(rcvdata,tmpdata,dataLen);
    return 0;
}


int TCPconnectToServer()
{
    return tcpclient->connectToServer();
}

int TCPdisconnectServer()
{
    tcpclient->close();
    return 0;
}

U8 TCPgetHostIpAddress(char *outip)
{
    QString strIpAddress;
    #if defined(Q_OS_WIN32)||defined(Q_OS_WASM)
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
    #endif
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

     QByteArray ba = strIpAddress.toLatin1(); // must
     char * ch=ba.data();

     strcpy(outip,ch);
     return 0;
}

char * NewUUID(char * buffer)
{
    return (char *)QUuid::createUuid().toString().toStdString().c_str();
}

void SetBrush(U8 type,U8 trans)//TYPE 0-original 1-mem 2-str
{
    if(type == 0)
    {
        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81,0));
        }
        else if(g_showPicType >0)
        {
            getGlobalWidget()->getPainter()->setBrush(QColor(198,151,81));
        }
        else
        {
            getGlobalWidget()->getPainter()->setBrush(g_backColor);
        }
    }
    else if(type ==1)
    {

        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81,0));
        }
        else if(g_showPicType >0)
        {
            getGlobalWidget()->getPainterMem()->setBrush(QColor(198,151,81));
        }
        else
        {
            getGlobalWidget()->getPainterMem()->setBrush(g_backColor);
        }

    }
    else
    {
        if(g_showPicType >0 && trans == 1)
        {
            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81,0));
        }
        else if(g_showPicType >0)
        {
            getGlobalWidget()->getPainterStr()->setBrush(QColor(198,151,81));
        }
        else
        {
            getGlobalWidget()->getPainterStr()->setBrush(g_backColor);
        }
    }
}
