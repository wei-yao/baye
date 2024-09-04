/***********************************************************************
*Copyright (c)2012 , 天际边工作室
*All rights reserved.
**
文件名称：	LibOperator.cpp
*摘要：		游戏引擎类，开始游戏界面
**
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2012-10-5  wstjb     基本的功能完成
***********************************************************************/

#include "PicConverter.h"
#include "gameview.h"

extern int flag;
extern int tasknum;

//uchar resByte[2731][16384];//存储lib资源
//int resByteLen[2731];
//int resPos[2731][6];//存储lib地址和编号 第六位为长度
//int resCount;
uchar g_buffer[1152000+1];//10485760];

extern int tmp_picMax;//20170102
extern QColor g_backColor;


void PicConverter::Init()
{

}

void PicConverter::KeyDown(int KeyCode)
{


}

uchar PicConverter::BytesToImage()
{
	int count = BytesToInt1(bfSize);
	int width = BytesToInt1(biWidth);
	int height = BytesToInt1(biHeight);
	//byte buffer[count];
	//byte buffer1[count-62];
    uchar buffer[16384];
    uchar buffer1[16384];

	buffer[0] = 0x42;
	buffer[1] = 0x4d;
	buffer[2] = bfSize[0];
	buffer[3] = bfSize[1];
	buffer[4] = bfSize[2];
	buffer[5] = bfSize[3];
	buffer[6] = 0;
	buffer[7] = 0;
	buffer[8] = 0;
	buffer[9] = 0;
	buffer[10] = 0x3e;
	buffer[11] = 0;
	buffer[12] = 0;
	buffer[13] = 0;
	buffer[14] = 0x28;
	buffer[15] = 0;
	buffer[16] = 0;
	buffer[17] = 0;
	buffer[18]= biWidth[0];
	buffer[19] = biWidth[1];
	buffer[20] = biWidth[2];
	buffer[21] = biWidth[3];
	buffer[22] = biHeight[0];
	buffer[23] = biHeight[1];
	buffer[24] = biHeight[2];
	buffer[25] = biHeight[3];
	buffer[26] = 1;
	buffer[27] = 0;
	buffer[28] = 1;
	buffer[29] = 0;
	buffer[30] = 0;
	buffer[31] = 0;
	buffer[32] = 0;
	buffer[33] = 0;
	buffer[34] = biSizeImage[0];
	buffer[35] = biSizeImage[1];
	buffer[36] = biSizeImage[2];
	buffer[37] = biSizeImage[3];
	buffer[38]= 0xc4;
	buffer[39] = 0x0e;
	buffer[40] = 0;
	buffer[41] = 0;
	buffer[42] = 0xc4;
	buffer[43] = 0x0e;
	buffer[44] = 0;
	buffer[45] = 0;
	buffer[46] = 0;
	buffer[47] = 0;
	buffer[48] = 0;
	buffer[49] = 0;
	buffer[50] = 0;
	buffer[51] = 0;
	buffer[52] = 0;
	buffer[53] = 0;
	buffer[54] = 0;
	buffer[55] = 0;
	buffer[56] = 0;
	buffer[57] = 0;
	buffer[58] = 0xff;
	buffer[59] = 0xff;
	buffer[60] = 0xff;
	buffer[61] = 0;


	width = (width + 8 - width % 8) / 8;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			buffer1[(height - i - 1) * width + j] = biImage[i * width + j];
		}
	}

	for (int j = 62; j < count; j++)
	{
		//buffer[count - i + 61 ] = biImage[i - 62];
		buffer[j] = buffer1[j - 62];
	}



	return *buffer;
}

/*int PicConverter::GetResNumFromType(int type,int restype,int index)
{
	for (int ii = 0; ii < resCount / 3; ii++)//原有
	{
		if (resPos[ii][0] == type && resPos[ii][1] == restype && resPos[ii][2] == index)
		{
			return ii;
		}
	}
	return -1;
}*/

QImage PicConverter::BytesToHBitmap_Str(uchar *resByte,U8 wid,U8 high)
{
    //U8	wid,high,mode;
    U16	off;


    //wid = resByte[0];
    //high = resByte[2];
    //mode = resByte[5] & 0x01;
    off = wid >> 3;
    if(wid & 0x07)
        off += 1;
    off *= high;
    //off <<= mode;
    //off +=6;

    int resByteLen = off;
    int widtha = 0;

    for (int i = 0; i < resByteLen; i++)
    {
        int tmp = 255 - BytesToInt1(&resByte[i]);
        IntToBytes1(tmp,&biImage[i]);
    }


    IntToBytes4(62 + resByteLen,bfSize);
    IntToBytes4(resByteLen,biSizeImage);
    memcpy(biWidth,&resByte[0],2);
    memcpy(biHeight,&resByte[2],2);



    //BytesToImage();

    int count = BytesToInt1(bfSize);
    int width = wid;
    widtha = width;
    int height = high;

    uchar buffer[2000];
    memset(buffer,0,sizeof(buffer));

    //QImage  image1 = QImage(width, height, QImage::Format_Mono);
    QVector<QRgb>  colorTable;
    colorTable.push_back( qRgb(0,0,0) );
    colorTable.push_back( qRgb(255,255,255) );
    //image1.setColorTable(colorTable);
    //image1.setPixel(1, 1, value);
    //int  W_1 = image1.bytesPerLine();


    int width1 = width + 8 - width % 8;
    int tmp = 0;
    if(( width1 - width )%8 ==0)
    {
        tmp = 1;
    }


    width = (width + 8 - width % 8) / 8 ;


    int width2 = 0;
    if((width-tmp) %4 != 0)
    {
        width2 = (width - tmp)  + 4 - (width - tmp) % 4;
    }
    else
    {
        width2 = width - tmp;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            buffer[i * width2  + j] = biImage[i * (width - tmp) + j];
        }
    }

    QImage  hBitmap = QImage((uchar *)buffer,widtha, height, QImage::Format_Mono);
    hBitmap.setColorTable(colorTable);
    //return image1;


    QImage  hBitmap1 = hBitmap.convertToFormat(QImage::Format_RGB888);

    int bmpWidth = hBitmap1.width();
    int bmpHeight = hBitmap1.height();
    //透明颜色

    for(int i=0;i< bmpWidth;++i)
    {
        for(int j=0;j<bmpHeight;++j)
        {
            //如果身份证背景色等于 Color(254,254,254,255),则设置为透明色 Color(254,254,254,0)
            if(hBitmap1.pixelColor(i,j)==Qt::white)
            {
                hBitmap1.setPixelColor(i,j,g_backColor);
            }
        }
    }

    return hBitmap1;

}

QImage PicConverter::BytesToHBitmap(uchar *resByte,U8 wid,U8 high)
{
    //U8	wid,high,mode;
    U16	off;


    //wid = resByte[0];
    //high = resByte[2];
    //mode = resByte[5] & 0x01;
    off = wid >> 3;
    if(wid & 0x07)
        off += 1;
    off *= high;
    //off <<= mode;
    //off +=6;

    int resByteLen = off;
    int widtha = 0;

    for (int i = 0; i < resByteLen; i++)
	{
        int tmp = 255 - BytesToInt1(&resByte[i]);
        IntToBytes1(tmp,&biImage[i]);
	}


    IntToBytes4(62 + resByteLen,bfSize);
    IntToBytes4(resByteLen,biSizeImage);
    memcpy(biWidth,&resByte[0],2);
    memcpy(biHeight,&resByte[2],2);



	//BytesToImage();

	int count = BytesToInt1(bfSize);
    int width = wid;
    widtha = width;
    int height = high;

    uchar buffer[16384*4];
	memset(buffer,0,sizeof(buffer));
	
    //QImage  image1 = QImage(width, height, QImage::Format_Mono);
    QVector<QRgb>  colorTable;
    colorTable.push_back( qRgb(0,0,0) );
    colorTable.push_back( qRgb(255,255,255) );
    //image1.setColorTable(colorTable);
    //image1.setPixel(1, 1, value);
    //int  W_1 = image1.bytesPerLine();


	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}

	
	width = (width + 8 - width % 8) / 8 ;
	

	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            buffer[i * width2  + j] = biImage[i * (width - tmp) + j];
        }
	}

    QImage  hBitmap = QImage((uchar *)buffer,widtha, height, QImage::Format_Mono);
    hBitmap.setColorTable(colorTable);
    //return image1;

    QImage  hBitmap1 = hBitmap.convertToFormat(QImage::Format_RGB888);

    int bmpWidth = hBitmap1.width();
    int bmpHeight = hBitmap1.height();
    //透明颜色

    for(int i=0;i< bmpWidth;++i)
    {
        for(int j=0;j<bmpHeight;++j)
        {
            //如果身份证背景色等于 Color(254,254,254,255),则设置为透明色 Color(254,254,254,0)
            if(hBitmap1.pixelColor(i,j)==Qt::white)
            {
                hBitmap1.setPixelColor(i,j,g_backColor);
            }
        }
    }

    return hBitmap1;


	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
    //HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    //return hBitmap;

    /*for(int i=0;  i<height;  ++i)
    {
        //现在可以按照通常的理解，取每行
        lineNum_1 = i * W_1;               //注意，这里不再需要乘倍数了(4, 3等)
        for(int j=0; j<width; ++j)
        {
            int r_1 = imagebits_1[ lineNum_1 + j * 3];        //注意区别32位的图


        }
    }*/

}

QImage PicConverter::BytesToHBitmapTrans(uchar * resByte,U8 wid,U8 high)//透明
{
    //U8	wid,high,mode;
    U16	off;


    //wid = resByte[0];
    //high = resByte[2];
    //mode = (resByte[5]) & 0x01;
    off = wid >> 3;
    if(wid & 0x07)
        off += 1;
    off *= high;
    off <<= 1;
    //off +=6;

    int resByteLen = off;
    int widtha = 0;
    for (int i = 0; i < resByteLen; i++)
	{
        int tmp = BytesToInt1(&resByte[i]);
        IntToBytes1(tmp,&biImage[i]);
	}


    IntToBytes4(62 + resByteLen,bfSize);
    IntToBytes4(resByteLen,biSizeImage);
    //memcpy(biWidth,&resByte[0],2);
    //memcpy(biHeight,&resByte[2],2);

	//BytesToImage();

    int count = BytesToInt1(bfSize);
    int width = wid;
    widtha = width;
    int height = high;

    uchar buffer[16384];
	memset(buffer,0,sizeof(buffer));

    QVector<QRgb>  colorTable;
    colorTable.push_back( qRgb(0,0,0) );
    colorTable.push_back( qRgb(255,255,255) );

	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}
	width = (width + 8 - width % 8) / 8 ;
	
	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}


	//增加对no判断
    for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width ; j++)
			{
                buffer[i * width2  + j] =
					(0x40 & biImage[(i * (width - tmp) + j)*2])<< 1 | (0x10 & biImage[(i * (width - tmp) + j)*2]) << 2 | (0x4 & biImage[(i * (width - tmp) + j)*2])<< 3 | (0x1 & biImage[(i * (width - tmp) + j)*2])<< 4 |
					(0x40 & biImage[(i * (width - tmp) + j)*2+1])>>3 | (0x10 & biImage[(i * (width - tmp) + j)*2+1])>>2 | (0x4 & biImage[(i * (width - tmp) + j)*2+1])>> 1 | (0x1 & biImage[(i * (width - tmp) + j)*2+1]);
				int tmp = 255 - BytesToInt1(&buffer[(height - i - 1) * width2  + j]);
				IntToBytes1(tmp,&buffer[(height - i - 1) * width2  + j]);
			}
		}

	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
    //HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    QImage  hBitmap = QImage((uchar *)buffer,widtha, height, QImage::Format_Mono);
    hBitmap.setColorTable(colorTable);



    return hBitmap;
}

QImage PicConverter::BytesToHBitmapMask(uchar * resByte,U8 wid,U8 high)//透明mask
{
   // U8	wid,high,mode;
    U16	off;


    //wid = resByte[0];
    //high = resByte[2];
    //mode = resByte[5] & 0x01;
    off = wid >> 3;
    if(wid & 0x07)
        off += 1;
    off *= high;
    off <<= 1;
    //off +=6;

    int resByteLen = off;
    int widtha = 0;

    for (int i = 0; i < resByteLen; i++)
	{
        int tmp =  BytesToInt1(&resByte[i]);
        IntToBytes1(tmp,&biImage[i]);
	}


    IntToBytes4(62 + resByteLen,bfSize);
    IntToBytes4(resByteLen,biSizeImage);
    //memcpy(biWidth,&resByte[0],2);
    //memcpy(biHeight,&resByte[2],2);

	//BytesToImage();

	int count = BytesToInt1(bfSize);
    int width = wid;
    widtha = width;
    int height = high;

    uchar buffer[16384];
	memset(buffer,0,sizeof(buffer));
	
    QVector<QRgb>  colorTable;
    colorTable.push_back( qRgb(0,0,0) );
    colorTable.push_back( qRgb(255,255,255) );

	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}
	width = (width + 8 - width % 8) / 8 ;
	
	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}

	//增加对no判断

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width ; j++)
			{
                buffer[i * width2  + j] =
					(0x80 & biImage[(i * (width - tmp) + j)*2]) | (0x20 & biImage[(i * (width - tmp) + j)*2]) << 1 | (0x8 & biImage[(i * (width - tmp) + j)*2])<< 2 | (0x2 & biImage[(i * (width - tmp) + j)*2])<< 3 |
					(0x80 & biImage[(i * (width - tmp) + j)*2+1])>>4 | (0x20 & biImage[(i * (width - tmp) + j)*2+1])>>3 | (0x8 & biImage[(i * (width - tmp) + j)*2+1])>> 2 | (0x2 & biImage[(i * (width - tmp) + j)*2+1])>> 1;
				int tmp = 255 - BytesToInt1(&buffer[(height - i - 1) * width2  + j]);
				IntToBytes1(tmp,&buffer[(height - i - 1) * width2  + j]);
			}
		}


	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
    //HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    QImage  hBitmap = QImage((uchar *)buffer,widtha, height, QImage::Format_Mono);
    hBitmap.setColorTable(colorTable);
    return hBitmap;
}

QImage PicConverter::BytesToHBitmap_Color(uchar *resByte,U16 wid,U16 high)
{
    //wid*=5;
    //high*=5;

    int width2 = ( wid * 24 + 31 )/32 * 4;//每行字节数
    int byteCount = high * width2;


    //byteCount = width2;

    //byteCount *= high;

    int resByteLen = byteCount;
    int widtha = 0;

    for (int i = 0; i < resByteLen; i++)
    {
        int tmp = BytesToInt1(&resByte[i]);
        IntToBytes1(tmp,&biImage[i]);
    }


    IntToBytes4(54 + resByteLen,bfSize);
    IntToBytes4(resByteLen,biSizeImage);
    memcpy(biWidth,&resByte[0],2);
    memcpy(biHeight,&resByte[2],2);



    //BytesToImage();

    int count = BytesToInt1(bfSize);
    int width = wid;
    widtha = width;
    int height = high;

    //uchar buffer[byteCount+1];//10485760];
    memset(g_buffer,0,sizeof(g_buffer));



    int width1 = width;//图片宽像素



    width = width * 3 ;//3字节24bit RGB


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width2 ; j++)
        {
            g_buffer[i * width2  + j] = biImage[i * width2 + j];
        }
    }

    //QByteArray imageByteArray = QByteArray( (const char*)buffer,  byteCount );
    //uchar*  transData = (unsigned char*)imageByteArray.data();

    QImage  hBitmap = QImage((uchar *)g_buffer,widtha, height, QImage::Format_RGB888);
    //hBitmap.setColorTable(colorTable);
    //return image1;
    QImage mirror_image = hBitmap.mirrored(false, true);
    QImage finished = mirror_image.rgbSwapped();
    return finished;




}

QImage PicConverter::BytesToHBitmap_JPG(uchar *resByte,U32 size)
{
    //wid*=5;
    //high*=5;

    QImage hBitmap = QImage::fromData(resByte,(int)size,"JPG");

    //hBitmap.setColorTable(colorTable);
    //return image1;
    //QImage mirror_image = hBitmap.mirrored(false, true);
    //QImage finished = mirror_image.rgbSwapped();

    return hBitmap;




}

/*void PicConverter::GetCGByte(int picnum,int resnum,int frameCount,int transtype)
{
	int width = 0;
	int width1 = 0;
	int height = 0;
	int bytecount = 0;
	int j = 0;
	int bytestartpos = 0;
	for (int i = 6 + frameCount*5 ; i < resByteLen[resnum];)
	{
		
		width = BytesToInt1(&resByte[resnum][i+2]);
		width1 = width + 8 - width % 8;
		if(width1 - width == 8 )width1 -= 8;
		height = BytesToInt1(&resByte[resnum][i+3]);
		bytecount = (resByte[resnum][i+5] == 1)?(width1 * height /8):(width1 * height /4);
		if(j == picnum ) 
		{
			bytestartpos = i;
			break;
		}
		j++;
		i = i + bytecount + 6 ;
	}
	
	cgByteLen = bytecount + 6;
	for(int k = 0 ; k < cgByteLen ; k++)
	{
		cgByte[k] = resByte[resnum][bytestartpos + k];
	}

}*/

/*QImage PicConverter::BytesToHBitmapCG(int picnum,int resnum,int frameCount)
{
	GetCGByte(picnum,resnum,frameCount,1);


	for (int i = 6 ; i < cgByteLen; i++)
	{
		int tmp = 255 - BytesToInt1(&cgByte[i]);
		IntToBytes1(tmp,&biImage[i - 6]);
	}


	IntToBytes4(62 + cgByteLen - 6,bfSize);
	IntToBytes4(cgByteLen - 6,biSizeImage);
	memcpy(biWidth,&cgByte[2],1);
	memcpy(biHeight,&cgByte[3],1);

	//BytesToImage();

	int count = BytesToInt1(bfSize);
	int width = BytesToInt1(biWidth);
	int height = BytesToInt1(biHeight);

	byte buffer[16384];
	memset(buffer,0,sizeof(buffer));
	
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader); // size of this struct
	bi.bmiHeader.biBitCount = 1;
	bi.bmiHeader.biCompression = 0;// rgb = 3 components
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = width; // width of window
	bi.bmiHeader.biHeight = height; // height of window
	bi.bmiHeader.biClrUsed = 0;

	int i;
	for (i=0; i<2; i++)
	{
		bi.bmiColors[i].rgbRed  = i;
		bi.bmiColors[i].rgbGreen= i;
		bi.bmiColors[i].rgbBlue = i;
	}

	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}
	width = (width + 8 - width % 8) / 8 ;
	
	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width ; j++)
		{
			buffer[(height - i - 1) * width2  + j] = biImage[i * (width - tmp) + j];
		}
	}

	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
	HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	return hBitmap;
}

QImage PicConverter::BytesToHBitmapCGMask(int picnum,int resnum,int frameCount,int &transflag)
{
	GetCGByte(picnum,resnum,frameCount,2);
	if(cgByte[5]  == 1)
	{
		transflag = 1;
		return NULL;
	}
	transflag = 2;
	for (int i = 6 ; i < cgByteLen; i++)
	{
		int tmp = 255 - BytesToInt1(&cgByte[i]);
		IntToBytes1(tmp,&biImage[i - 6]);
	}


	IntToBytes4(62 + cgByteLen - 6,bfSize);
	IntToBytes4(cgByteLen - 6,biSizeImage);
	memcpy(biWidth,&cgByte[2],1);
	memcpy(biHeight,&cgByte[3],1);

	//BytesToImage();

	int count = BytesToInt1(bfSize);
	int width = BytesToInt1(biWidth);
	int height = BytesToInt1(biHeight);

	byte buffer[16384];
	memset(buffer,0,sizeof(buffer));
	
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader); // size of this struct
	bi.bmiHeader.biBitCount = 1;
	bi.bmiHeader.biCompression = 0;// rgb = 3 components
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = width; // width of window
	bi.bmiHeader.biHeight = height; // height of window
	bi.bmiHeader.biClrUsed = 0;

	int i;
	for (i=0; i<2; i++)
	{
		bi.bmiColors[i].rgbRed  = i;
		bi.bmiColors[i].rgbGreen= i;
		bi.bmiColors[i].rgbBlue = i;
	}

	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}
	width = (width + 8 - width % 8) / 8 ;
	
	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width ; j++)
		{
			buffer[(height - i - 1) * width2  + j] = 
				(0x80 & biImage[(i * (width - tmp) + j)*2]) | (0x20 & biImage[(i * (width - tmp) + j)*2]) << 1 | (0x8 & biImage[(i * (width - tmp) + j)*2])<< 2 | (0x2 & biImage[(i * (width - tmp) + j)*2])<< 3 |
				(0x80 & biImage[(i * (width - tmp) + j)*2+1])>>4 | (0x20 & biImage[(i * (width - tmp) + j)*2+1])>>3 | (0x8 & biImage[(i * (width - tmp) + j)*2+1])>> 2 | (0x2 & biImage[(i * (width - tmp) + j)*2+1])>> 1;
			int tmp = BytesToInt1(&buffer[(height - i - 1) * width2  + j]);
			IntToBytes1(tmp,&buffer[(height - i - 1) * width2  + j]);
		}
	}

	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
	HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	return hBitmap;
}

QImage PicConverter::BytesToHBitmapCGTrans(int picnum,int resnum,int frameCount)
{
	GetCGByte(picnum,resnum,frameCount,2);


	for (int i = 6 ; i < cgByteLen; i++)
	{
		int tmp = 255 - BytesToInt1(&cgByte[i]);
		IntToBytes1(tmp,&biImage[i - 6]);
	}


	IntToBytes4(62 + cgByteLen - 6,bfSize);
	IntToBytes4(cgByteLen - 6,biSizeImage);
	memcpy(biWidth,&cgByte[2],1);
	memcpy(biHeight,&cgByte[3],1);

	//BytesToImage();

	int count = BytesToInt1(bfSize);
	int width = BytesToInt1(biWidth);
	int height = BytesToInt1(biHeight);

	byte buffer[16384];
	memset(buffer,0,sizeof(buffer));
	
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader); // size of this struct
	bi.bmiHeader.biBitCount = 1;
	bi.bmiHeader.biCompression = 0;// rgb = 3 components
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = width; // width of window
	bi.bmiHeader.biHeight = height; // height of window
	bi.bmiHeader.biClrUsed = 0;

	int i;
	for (i=0; i<2; i++)
	{
		bi.bmiColors[i].rgbRed  = i;
		bi.bmiColors[i].rgbGreen= i;
		bi.bmiColors[i].rgbBlue = i;
	}

	int width1 = width + 8 - width % 8;
	int tmp = 0;
	if(( width1 - width )%8 ==0)
	{
		tmp = 1;
	}
	width = (width + 8 - width % 8) / 8 ;
	
	int width2 = 0;
	if((width-tmp) %4 != 0)
	{
		width2 = (width - tmp)  + 4 - (width - tmp) % 4;
	}
	else
	{
		width2 = width - tmp;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width ; j++)
		{
			buffer[(height - i - 1) * width2  + j] = 
				(0x40 & biImage[(i * (width - tmp) + j)*2])<< 1 | (0x10 & biImage[(i * (width - tmp) + j)*2]) << 2 | (0x4 & biImage[(i * (width - tmp) + j)*2])<< 3 | (0x1 & biImage[(i * (width - tmp) + j)*2])<< 4 |
				(0x40 & biImage[(i * (width - tmp) + j)*2+1])>>3 | (0x10 & biImage[(i * (width - tmp) + j)*2+1])>>2 | (0x4 & biImage[(i * (width - tmp) + j)*2+1])>> 1 | (0x1 & biImage[(i * (width - tmp) + j)*2+1]);
			int tmp = BytesToInt1(&buffer[(height - i - 1) * width2  + j]);
			IntToBytes1(tmp,&buffer[(height - i - 1) * width2  + j]);
		}
	}

	//HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
	HBITMAP hBitmap = CreateDIBitmap(NULL, &bi.bmiHeader, CBM_INIT, buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	return hBitmap;
}*/

int PicConverter::GetPicNumInPic(uchar * resByte)//获取pic number
{

    int tmp = BytesToInt1(&resByte[4]);
	
	return tmp;
}

int PicConverter::GetDefaultWidth(uchar * resByte)
{

    memcpy(biWidth,&resByte[0],2);
	
    int width = BytesToInt2(biWidth);
	
	return width;
}


int PicConverter::GetDefaultHeight(uchar * resByte)
{

	
    memcpy(biHeight,&resByte[2],2);

	
    int height = BytesToInt2(biHeight);

	return height;
}



int PicConverter::BytesToInt4(uchar * bytes)
{
    int a = bytes[0] & 0xFF;
    a |= ((bytes[1] << 8) & 0xFF00);
    a |= ((bytes[2] << 16) & 0xFF0000);
    a |= ((bytes[3] << 24) & 0xFF000000);
    return a;
}

int PicConverter::BytesToInt2(uchar * bytes)
{
    int a = bytes[0] & 0xFF;
    a |= ((bytes[1] << 8) & 0xFF00);
    return a;
}

int PicConverter::BytesToInt1(uchar * bytes)
{
    int a = bytes[0] & 0xFF;
    return a;
}

void  PicConverter::IntToBytes4(int i,uchar *bytes)
{
    memset(bytes,0,sizeof(uchar) *  4);
    bytes[0] = (uchar) (0xff & i);
    bytes[1] = (uchar) ((0xff00 & i) >> 8);
    bytes[2] = (uchar) ((0xff0000 & i) >> 16);
    bytes[3] = (uchar) ((0xff000000 & i) >> 24);
    return ;
}

void  PicConverter::IntToBytes2(int i,uchar *bytes)
{
    memset(bytes,0,sizeof(uchar) *  2);
    bytes[0] = (uchar) (0xff & i);
    bytes[1] = (uchar) ((0xff00 & i) >> 8);
    return ;
}

void  PicConverter::IntToBytes1(int i,uchar *bytes)
{
    memset(bytes,0,sizeof(uchar) );
    bytes[0] = (uchar) (0xff & i);
    return ;
}
