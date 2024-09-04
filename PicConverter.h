/***********************************************************************
*Copyright (c)2012 , 天际边工作室
*All rights reserved.
**
文件名称：	PicConverter.cpp
*摘要：		游戏引擎类头文件
**
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2012-10-5  wstjb     基本的功能完成
***********************************************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>
//#include "PublicFun.h"
//#include "libOperator.h"
#include <QImage>
#include "compa.h"
class PicConverter
{
public:

	void Init();
	
    void KeyDown(int KeyCode);
    uchar BytesToImage();
    QImage BytesToHBitmap(uchar *pic,U8 wid,U8 high);
    QImage BytesToHBitmap_Str(uchar *pic,U8 wid,U8 high);
    QImage BytesToHBitmapTrans(uchar * pic,U8 wid,U8 high);
    QImage BytesToHBitmapMask(uchar * pic,U8 wid,U8 high);
    QImage BytesToHBitmap_Color(uchar *pic,U16 wid,U16 high);
    QImage BytesToHBitmap_JPG(uchar *pic,U32 size);
	//void BufferTobiImageBuf(int type,int restype,int index);
    //int GetResNumFromType(int type,int restype,int index);
    //QImage BytesToHBitmapCG(int picnum,int resnum,int frameCount);
    //void GetCGByte(int picnum,int resnum,int frameCount,int transtype);
    //QImage BytesToHBitmapCGMask(int picnum,int resnum,int frameCount,int &transflag);
    //QImage BytesToHBitmapCGTrans(int picnum,int resnum,int frameCount);
    int GetPicNumInPic(uchar * pic);//获取PIC中存储的图片数量
    int GetDefaultWidth(uchar * pic);
    int GetDefaultHeight(uchar * pic);

    int BytesToInt4(uchar * bytes );
    int BytesToInt2(uchar * bytes );
    int BytesToInt1(uchar * bytes );
    void  IntToBytes4(int i,uchar *bytes);
    void  IntToBytes2(int i,uchar *bytes);
    void  IntToBytes1(int i,uchar *bytes);

//private:
	//BMP文件头14
    uchar bfType[2];
    uchar bfSize[4];
    uchar bfReserved1[2];
    uchar bfReserved2[2];
    uchar bfOffBits[4];
	
	//信息头40
    uchar biSize[4]; // 本结构所占用字节数
    uchar biWidth[4]; // 位图的宽度，以像素为单位
    uchar biHeight[4]; // 位图的高度，以像素为单位
    uchar biPlanes[2]; // 目标设备的级别，必须为1
    uchar biBitCount[2];// 每个像素所需的位数，必须是1(双色),4(16色)，8(256色)或24(真彩色)之一
    uchar biCompression[4]; // 位图压缩类型，必须是 0(不压缩)
    uchar biSizeImage[4]; // 位图的大小，以字节为单位.必须为4倍数
    uchar biXPelsPerMeter[4]; // 位图水平分辨率，每米像素数
    uchar biYPelsPerMeter[4]; // 位图垂直分辨率，每米像素数
    uchar biClrUsed[4];// 位图实际使用的颜色表中的颜色数
    uchar biClrImportant[4];// 位图显示过程中重要的颜色数
	
	//调色板8
    uchar bmiColors[2];
	
	//位图数据 0-黑 1-白 Height(图像高度)*(Width+8-Width%8)/8
    uchar biImage[10485760];//16384];

	//LibOperator *libOperator;
	
    /*struct BitMAPINFO
    {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD          bmiColors[2];
    } bi;*/

	int cgByteLen;
    uchar cgByte[16384];

};
