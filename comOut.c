/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	comOut.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		封装引擎对系统显示程序调用的入口
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：本程序和界面无关，无兼容模式。
*	3、使用本封装程序的引擎要移植到其他环境中，系统的显示部分只需在此修改
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
/* 声明本文件可以直接调用系统底层函数宏 */
#define		_BE_CHANGED_
#include "stdsys.h"
#include "comm.h"
#include "enghead.h"

/* 当前所在文件 */
#define		IN_FILE		21

extern int g_showPicType;
extern int g_PicId;
extern U8 fontByte[163840+1];
int g_noUseStrMem = 0;
extern double g_screentimes;
extern int g_useDotFont;

/*本体函数声明*/

/*------------------------------------------*/
U32	CountHZMAddrOff(U16 Hz);
void	GamResumeSet(U16 bakBnk);
void	GamAscii(U8 x,U8 y,U8 asc);
void	GamChinese(U8 x,U8 y,U16 Hz);
void	GamStrShow(U8 x,U8 y,const char *buf);
void	GamStrShowT(U8 x,U8 y,U8 *buf);
void	GetExcHZMCode(U16 Hz,U8 *hzmCode);

/***********************************************************************
 * 说明:     游戏系统信息框(不保存背景)
 * 输入参数: buf-信息内容	delay-延时(秒)
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamMsgBox(const char *buf,U8 delay)
{
    U8	h,s;
	U8	slen;

	c_Sx = WK_SX + 5;
	c_Ex = WK_EX - 4;
	slen = gam_strlen(buf);
	h = (WK_EX - WK_SX) / ASC_WID - 2;
	s = slen / h + 1;
	if(s > (WK_EY - WK_SY) / ASC_HGT - 1)
	{
		c_Sy = WK_SY+5;
		c_Ey = WK_EY-5;
	}
	else
	{
		s *= ASC_HGT;
		c_Sy = (WK_EY - WK_SY - s) >> 1;
		c_Sy += WK_SY;
		c_Ey = c_Sy + s;
	}
	gam_clrlcd(c_Sx-3,c_Sy-3,c_Ex+3,c_Ey+2);
	gam_rect(c_Sx-3,c_Sy-3,c_Ex+3,c_Ey+2);
	GamStrShowS(c_Sx,c_Sy,buf);
    if(delay == 0)
        return;
    GamDelay(delay*100,true);
}
/***********************************************************************
 * 说明:     显示虚拟屏幕到屏幕
 * 输入参数: vscr-虚拟屏幕指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamShowFrame(U8 *vscr)
{
     SysCopyToDc();
    //gam_Picture(0,0,SCR_WID-1,SCR_HGT-1,vscr);
}
/***********************************************************************
 * 说明:     显示图片到屏幕
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	pic-图片数据
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamPicShowS(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic)
{
     if(g_showPicType > 0)
     {
         wid*=5;
         hgt*=5;
     }
    wid-=1;
	hgt-=1;
    gam_Picture(x,y,x+wid,y+hgt,pic);
}
/***********************************************************************
 * 说明:     显示图片到虚拟屏幕
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	pic-图片数据
 *	   : vscr-虚拟屏幕指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamPicShowV(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr)
{
     if(g_showPicType > 0)
     {
         wid*=5;
         hgt*=5;
     }
    wid-=1;
    hgt-=1;
    GamePictureDummy(x,y,x+wid,y+hgt,pic,vscr,0);			/* 正常显示模式 */
}
/***********************************************************************
 * 说明:     显示mask图片到屏幕
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	pic-图片数据
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamMPicShowS(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic)
{
     if(g_showPicType > 0)
     {
         wid*=5;
         hgt*=5;
     }
    gam_Picture(x,y,x+wid,y+hgt,pic);
}
/***********************************************************************
 * 说明:     显示mask图片到虚拟屏幕
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	pic-图片数据
 *	   : vscr-虚拟屏幕指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamMPicShowV(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr)
{
    U32 pLen;
    if(g_showPicType > 0)
    {
        wid*=5;
        hgt*=5;
    }
    GamePictureDummy(x,y,x+wid-1,y+hgt-1,pic,vscr,1);		/* &显示模式*/
    if(g_showPicType == 0)
    {
        pLen=wid>>3;
        if(wid&0x07) pLen+=1;
        pLen*=hgt;
    }
    else if(g_showPicType == 1)
    {

        int width2 = ( wid * 24 + 31 )/32 * 4;//每行字节数
        pLen = hgt * width2;

    }
    else if(g_showPicType == 2)
    {
        pLen  = ((PictureHeadTypeJPG *)pic)->size;
        pic +=4;
    }
    //g_PicId = pLen;

    GamePictureDummy(x,y,x+wid-1,y+hgt-1,(U8 *)(pic+pLen),vscr,2);	/* |显示模式 */
}
/***********************************************************************
 * 说明:     显示图片到屏幕(功能扩展——可显示图片上面的部分)
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	idx-图片序号(从0开始)
 *	  : pic-图片数据(包括图片头)
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamPicShowExS(U8 x,U8 y,U16 wid,U16 hgt,U8 idx,U8 *pic)
{
    U8	mask;
    U16	pwid,phgt;
    U32	pLen;
	
    g_PicId = idx;
	pwid = ((PictureHeadType *)pic)->wid;
	phgt = ((PictureHeadType *)pic)->hig;
	mask = ((PictureHeadType *)pic)->mask;
    if(g_showPicType == 0)
    {
        pLen = pwid >> 3;
        if(pwid&0x07) pLen += 1;
        pLen *= phgt;
        pic += pLen * idx + PICHEAD_LEN;
    }
    else if(g_showPicType == 1)
    {
        wid*=5;
        hgt*=5;
        int width2 = ( pwid * 24 + 31 )/32 * 4;//每行字节数
        pLen = phgt * width2;
        pic += pLen * idx + PICHEAD_LEN;
    }
    else if(g_showPicType == 2)//JPG
    {
        wid*=5;
        hgt*=5;

        pic += PICHEAD_LEN;

        if(mask == 0)
        {
            int i = 0 ;
            for(i = 0  ; i < idx; i ++)
            {
                pLen = ((PictureHeadTypeJPG *)pic)->size;
                pic += pLen;
                pic += 4;
            }
        }

    }


	if(HV_MASK == mask)
		return;
    else
    {
        if(g_showPicType == 0)
        {
            GamPicShowS(x,y,wid,hgt,pic);
        }
        else
        {
            GamPicShowS(x,y,wid/5,hgt/5,pic);
        }
    }

}
/***********************************************************************
 * 说明:     显示图片到虚拟屏幕(功能扩展——可显示图片上面的部分)
 * 输入参数: x,y-显示坐标	wid,hgt-图片尺寸	idx-图片序号(从0开始)
 *	  : pic-图片数据(包括图片头)	vscr-虚拟屏幕指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamPicShowExV(U8 x,U8 y,U16 wid,U16 hgt,U8 idx,U8 *pic,U8 *vscr)
{
    U8	mask;
    U16	pwid,phgt;
    U32	pLen;
	
    g_PicId = idx;
	pwid = ((PictureHeadType *)pic)->wid;
	phgt = ((PictureHeadType *)pic)->hig;
	mask = ((PictureHeadType *)pic)->mask & 0x01;
    if(g_showPicType == 0)
    {
        pLen = pwid >> 3;
        if(pwid&0x07) pLen += 1;
        pLen *= phgt;
        pic += (pLen << mask) * idx + PICHEAD_LEN;
    }
    else if(g_showPicType == 1)
    {
        wid*=5;
        hgt*=5;
        int width2 = ( pwid * 24 + 31 )/32 * 4;//每行字节数
        pLen = phgt * width2;

        pic += (pLen << mask) * idx + PICHEAD_LEN;
    }
    else if(g_showPicType == 2)//JPG
    {
        wid*=5;
        hgt*=5;

        pic += PICHEAD_LEN;

        if(mask == 0)
        {
            int i = 0 ;
            for(i = 0 ; i < idx; i ++)
            {
                pLen = ((PictureHeadTypeJPG *)pic)->size;
                pic += pLen;
                pic += 4;
            }
        }
        else
        {
            int i = 0 ;
            for(i = 0 ; i < idx*2; i ++)
            {
                pLen = ((PictureHeadTypeJPG *)pic)->size;
                pic += pLen;
                pic += 4;
            }
        }


    }
	if(HV_MASK == mask)
    {
        GamePictureDummy(x,y,x+wid-1,y+hgt-1,pic,vscr,1);	/* &显示模式*/
        if(g_showPicType == 2)
        {
            pLen = ((PictureHeadTypeJPG *)pic)->size;//该分割图文件长度 如果是MASK的话是第一张图长度
            pic += 4;
        }
		pic += pLen;
        GamePictureDummy(x,y,x+wid-1,y+hgt-1,pic,vscr,2);	/* |显示模式*/
	}
	else
    {
        if(g_showPicType == 0)
        {
            GamPicShowV(x,y,wid,hgt,pic,vscr);
        }
        else// if(g_showPicType == 1)
        {
            GamPicShowV(x,y,wid/5,hgt/5,pic,vscr);
        }
    }
}
/***********************************************************************
 * 说明:     显示12字符串到屏幕
 * 输入参数: x,y-显示坐标	str-数据缓冲
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void GamStrShowS(U8 x,U8 y,const char *str)
{
    c_VisScr=(U8 *)NULL;
    //g_noUseStrMem = 1;
    GamStrShow(x,y,str);
}
/***********************************************************************
 * 说明:     显示12字符串到虚拟屏幕
 * 输入参数: x,y-显示坐标	str-数据缓冲	vscr-虚拟屏幕
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void GamStrShowV(U8 x,U8 y,U8 *str,U8 *vscr)
{

	c_VisScr=vscr;
	GamStrShow(x,y,(const char*)str);

}
/***********************************************************************
 * 说明:     初始化游戏引擎所在的机型环境
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void GamResumeSet(U16 bakBnk)
{

    //恢复字符串显示区域
	if(c_ReFlag)
	{
		c_Sx = WK_SX;
		c_Sy = WK_SY;
		c_Ex = WK_EX;
		c_Ey = WK_EY;
    }
}
/***********************************************************************
 * 说明:     显示12字符串
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
void GamStrShowT(U8 x,U8 y,U8 *buf)
{

    //SysAscii(x*g_screentimes,y*g_screentimes,buf);
    //return;
    U8	i,wid;
    U16	hzCode;
    U8	hzCode1[2+1];

    c_VisScr=(U8 *)NULL;

    //if(c_VisScr==NULL)
    if(c_VisScr==NULL && g_noUseStrMem == 0)
    {
        SysCopyFromDcToStr();
    }

    for(i=0;buf[i]-0!='\0';i++)
    {
        hzCode=buf[i];
        if(hzCode=='\n')
        {
            x=c_Sx;
            y+=HZ_HGT;
            if(y>c_Ey)
            {
                if(c_VisScr==NULL && g_noUseStrMem == 0)
                {
                    SysCopyToDcStr();
                }
                return;
            }
            continue;
        }
        if(hzCode<0x80)
            wid=ASC_WID-1;
        else
            wid=HZ_WID-1;
        if(x+wid>c_Ex)
            x+=wid;
        if(x>c_Ex)
        {
            x=c_Sx;
            y+=HZ_HGT;
            if(y + HZ_HGT>c_Ey)
            {
                if(c_VisScr==NULL && g_noUseStrMem == 0)
                {
                    SysCopyToDcStr();
                }
                return;
            }
        }
        if(hzCode<0x80)
        {
            hzCode1[0] = buf[i];
            hzCode1[1] = '\0';
            if(g_useDotFont)
            {
                GamAscii(x,y,hzCode);
            }
            else
            {
                SysAscii(x,y,hzCode1,1);
            }
            x+=6;
        }
        else
        {
            i+=1;
            hzCode<<=8;
            hzCode+=buf[i];
            hzCode1[0] = buf[i-1];
            hzCode1[1] = buf[i];
            hzCode1[2] = '\0';
            if(g_useDotFont)
            {
                GamChinese(x,y,hzCode);
            }
            else
            {
                SysChinese(x,y,hzCode1,1);
            }

            x+=12;
        }
    }
    if(c_VisScr==NULL)
    if(c_VisScr==NULL && g_noUseStrMem == 0)
    {
        SysCopyToDcStr();
    }
}
void GamStrShow(U8 x,U8 y,const char *buf)
{

    //SysAscii(x*g_screentimes,y*g_screentimes,buf);
    //return;
    U8	i,wid;
	U16	hzCode;
    U8	hzCode1[2+1];

    //if(c_VisScr==NULL)
    if(c_VisScr==NULL && g_noUseStrMem == 0)
    {
        SysCopyFromDcToStr();
    }

	for(i=0;buf[i]-0!='\0';i++)
	{
		hzCode=buf[i];
		if(hzCode=='\n')
		{
			x=c_Sx;
			y+=HZ_HGT;
            if(y>c_Ey)
            {
                if(c_VisScr==NULL && g_noUseStrMem == 0)
                {
                    SysCopyToDcStr();
                }
                return;
            }
			continue;
		}
		if(hzCode<0x80)
			wid=ASC_WID-1;
		else
			wid=HZ_WID-1;
		if(x+wid>c_Ex)
			x+=wid;
		if(x>c_Ex)
		{
			x=c_Sx;
			y+=HZ_HGT;
            if(y + HZ_HGT>c_Ey)
            {
                if(c_VisScr==NULL && g_noUseStrMem == 0)
                {
                    SysCopyToDcStr();
                }
                return;
            }
		}
        if(hzCode<0x80)
		{
            hzCode1[0] = buf[i];
            hzCode1[1] = '\0';
            if(g_useDotFont)
            {
                GamAscii(x,y,hzCode);
            }
            else
            {
                SysAscii(x,y,hzCode1,0);
            }
			x+=6;
		}
		else
		{
			i+=1;
            hzCode<<=8;
			hzCode+=buf[i];
            hzCode1[0] = buf[i-1];
            hzCode1[1] = buf[i];
            hzCode1[2] = '\0';
            if(g_useDotFont)
            {
                GamChinese(x,y,hzCode);
            }
            else
            {
                SysChinese(x,y,hzCode1,0);
            }

			x+=12;
		}
    }
    if(c_VisScr==NULL)
    if(c_VisScr==NULL && g_noUseStrMem == 0)
    {
        SysCopyToDcStr();
    }
}
/***********************************************************************
 * 说明:     显示12*12点阵GB2312汉字
 * 输入参数: x,y	->显示坐标	Hz	->要显示的汉字内码
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2004.6.2        基本完成
***********************************************************************/
void GamChinese(U8 x,U8 y,U16 Hz)
{
    U8 zmCode[24];
	
	GetExcHZMCode(Hz,zmCode);
    if(c_VisScr==NULL)
    //if(g_noUseStrMem == 1)
        SysPicture(x,y,x+HZ_WID-1,y+HZ_HGT-1,zmCode,9);
    else
        GamePictureDummy(x,y,x+HZ_WID-1,y+HZ_HGT-1,zmCode,c_VisScr,9);
}
/***********************************************************************
 * 说明:     显示12*12点阵GB2312AscII
 * 输入参数: x,y	->显示坐标	asc	->要显示的AscII
 * 返回值  : 
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2004.6.2        基本完成
***********************************************************************/
void GamAscii(U8 x,U8 y,U8 asc)
{
    U16 ascCode;
	U8  i,zmCode[24];
		
    //memset(zmCode,0,24);
    if(asc <= ' ')
        memset(zmCode,0,24);
	else
	{
		ascCode=GAM_FONT_ASC_QU;
		ascCode<<=8;
        ascCode+=asc-0x21+0xA1;		/* 汉字字模从'!'开始*/
			
		GetExcHZMCode(ascCode,zmCode);
		for(i=0;i<12;i++)
			zmCode[i]=zmCode[i<<1];
	}

    if(c_VisScr==NULL)
    //if(g_noUseStrMem == 1)
        SysPicture(x,y,x+ASC_WID-1,y+ASC_HGT-1,zmCode,9);
    else
        GamePictureDummy(x,y,x+ASC_WID-1,y+ASC_HGT-1,zmCode,c_VisScr,9);
}
/***********************************************************************
 * 说明:     获取扩充后的汉字字模数据(18->24)
 * 输入参数: Hz	->汉字内码	hzmCode	->扩充后的数据buf
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2004.6.2        基本完成
***********************************************************************/
void GetExcHZMCode(U16 Hz,U8 *hzmCode)
{
    /*字模页号偏移 是否跨bank*/
	U8  buf[18];
	U8  i,j,k;
	U32 hzmAddr;

    /* 当前要显示的汉字不是2312GB中的汉字，调试模式下显示黑块，释放模式下显示白块*/
	if((U8)(Hz>>8) < 0xA1)
	{
	#if	GAM_VER==GAM_DEBUG_MODE
        memset(hzmCode,0xFF,24);
		return;
	#else
        memset(hzmCode,0,24);
		return;
	#endif
	}
	else
	{
		hzmAddr=CountHZMAddrOff(Hz);
        //gam_fseek(g_FontFp,hzmAddr,SEEK_SET);
        //gam_fread(buf,1,18,g_FontFp);
        memcpy((char *)buf,fontByte+hzmAddr,18);
	}

    //转换数据
	for(i=0;i<6;i++)
	{
		j=i<<2;
		k=(i<<1)+i;
		hzmCode[j]=buf[k];
		hzmCode[j+1]=buf[k+1]&0xf0;
		hzmCode[j+2]=(buf[k+1]&0x0f)<<4;
		hzmCode[j+2]+=(buf[k+2]&0xf0)>>4;
		hzmCode[j+3]=(buf[k+2]&0x0f)<<4;
    }
}
/***********************************************************************
 * 说明:     计算要显示的汉字字模地址偏移
 * 输入参数: Hz	->汉字内码
 * 返回值  : 该汉字字模在字库中的地址偏移
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2004.6.2        基本完成
***********************************************************************/
U32 CountHZMAddrOff(U16 Hz)
{
    /*计算公式:(94*(HCode-0xA1)+(LCode-0xA1))*18*/
	U32 offset,pCode;
	
	pCode=(U8)(Hz>>8)-0xA1;
    offset=pCode<<6;			/* offset=pCode*64*/
    offset+=pCode<<5;			/* offset=pCode*64+pCode*32*/
    offset=offset-(pCode<<1);		/* offset=pCode*94*/
    offset+=(U8)(Hz)-0xA1;			/* offset=94*(HCode-0xA1)+(LCode-0xA1)*/
	pCode=offset;
	offset<<=4;
	offset+=pCode<<1;
    return offset;
}

/******************************************************************************
* 函数名:GamAsciiS
* 说  明:封装函数，显示12*12点阵GB2312AscII到屏幕
*
* 入口参数：x,y	->显示坐标	asc	->要显示的AscII
*
* 出口参数：无
*
* 修改历史:
*		姓名		日期			说明
*		----		----			-----------
*		陈泽伟		2005-6-23 16:13	基本功能完成
******************************************************************************/
 void GamAsciiS(U8 x,U8 y,U8 asc)
{
    c_VisScr=(U8 *)NULL;
    //GamAscii(x,y,asc);
    if(g_useDotFont)
    {
        GamAscii(x,y,asc);
    }
    else
    {
        U8 hzCode1[2+1];
        hzCode1[0] = asc;
        hzCode1[1] = '\0';
        SysAscii(x,y,hzCode1,0);
    }

}

void GamPicSave(U16 x,U16 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr,U8 type)
{
     if(g_showPicType > 0)
     {
         wid*=5;
         hgt*=5;
     }
    wid-=1;
    hgt-=1;
    GamePictureSave(x,y,x+wid,y+hgt,pic,vscr,type);//0-city 1-fight
}
