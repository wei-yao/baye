/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	PublicFun.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		游戏引擎公共模块
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：平移兼容|缩放兼容
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
#include "stdsys.h"
#include "comm.h"
#undef	PublicFun
#define	PublicFun
#include "enghead.h"
#define		IN_FILE	1	/* 当前文件位置 */

extern U8 SHARE_MEM[8000000+1];//2097152+1];
extern int g_showPicType;
extern int g_PicId;
extern int g_stat;
extern int g_noUseStrMem;

/*本体函数声明*/
/*------------------------------------------*/
U8 *PlcItemShowS(U8 sx,U8 sy,U8 ex,U8 ey,U8 *buf);
U8 *strlchr(U8 *buf,U8 len,U8 ch);
U8 *ltoa_my(long value,U8 * string,int radix);

/***********************************************************************
 * 说明:     播放指定的动画
 * 输入参数: speid 指定播放的对象,keyflag (00000010|00000001|00000100)背景处理|响应键盘|反转播放
 *           startfrm 起始帧,endfrm 结束帧, x,y 播放坐标
 * 返回值  : 0xff -- 非按键退出 ；其他值 -- 按键退出，按键的扫描码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             陈泽伟          2005-7-8 18:59       完成基本功能
***********************************************************************/
 U8 PlcMovie(U16 speid,U8 startfrm,U8 endfrm,U8 keyflag,U8 x,U8 y)
{

    U8 **dat,*srsptr;
    U8	 count;			/*桢数*/
	U8	 picmax;		/*最大图片序号*/
    U16 wid,high;//,picdatlen,maxdatlen
    U32 picdatlen,maxdatlen;
    U16 mode,lenspe;
	U8 i,clsflag,showflag;
	U8 mcount;
	U8 keynum;
	U8 x1,y1;
	U8 ymount,*spem,*spec;
	SPEUNIT  *spe;

	lenspe = 0;
	clsflag = 1;
	showflag = 1;
	maxdatlen = 0;

	srsptr = ResLoadToCon(speid,1,g_CBnkPtr);
	if (NULL == srsptr)
	{
		gamTraceP(speid);
		return(0xff);
	}
	
	count  = *(srsptr+2);
	picmax = *(srsptr+3);
	srsptr+=6;
	dat = (U8 **) SHARE_MEM;
	spem = (U8 *) (SHARE_MEM + 200);
	spec = (U8 *) (SHARE_MEM + 200 + 100);
	spe = (SPEUNIT *) srsptr;
	
	lenspe = count;
	srsptr += sizeof(SPEUNIT) * lenspe;
	for (i = 0;i <= endfrm - startfrm;i ++)
	{
		spem[i] = spe[i + startfrm].ndelay;
		spec[i] = spe[i + startfrm].cdelay;
	}
	for(i=0;i<picmax;i++)
	{
		wid = *(srsptr);
        wid += *(srsptr + 1)<<8;
		high = *(srsptr + 2);
        high += *(srsptr + 3)<<8;
		mode = *(srsptr + 5);

        if(g_showPicType ==0)
        {
            picdatlen=wid/8;
            if(wid%8)
                picdatlen++;
            picdatlen*=high;
            picdatlen*=(mode+1);
            picdatlen+=sizeof(PictureHeadType);
            dat[i] = srsptr;
            srsptr += picdatlen;
        }
        else if(g_showPicType ==1)
        {
            int width2 = ( wid * 24 + 31 )/32 * 4;//每行字节数
            picdatlen = high * width2;
            picdatlen+=sizeof(PictureHeadType);

            dat[i] = srsptr;
            srsptr += picdatlen;
        }
        else if(g_showPicType == 2)//jpg
        {
            picdatlen = *(srsptr + 6);
            picdatlen += *(srsptr + 7)<<8;
            picdatlen += *(srsptr + 8)<<16;
            picdatlen += *(srsptr + 9)<<24;

            picdatlen += 4;

            picdatlen+=sizeof(PictureHeadType);

            dat[i] = srsptr;
            srsptr += picdatlen;

            if(mode == 1)
            {
                picdatlen = *(srsptr + 0);
                picdatlen += *(srsptr + 1)<<8;
                picdatlen += *(srsptr + 2)<<16;
                picdatlen += *(srsptr + 3)<<24;

                picdatlen += 4;

                //picdatlen+=sizeof(PictureHeadType);

                //dat[i] = srsptr;
                srsptr += picdatlen;
            }
        }
	}
	mcount = 0;
	ymount = mcount;
	while (1)
        {
		if ((keyflag & 0x01) == 0x01)
		{
            keynum = gam_getkey();
            //if (keynum != 0xff)
            //	return(keynum);
		}
		for (i = 0;i <= mcount;i ++)
		{
			if (spec[i] == 1)
			{
				if ((keyflag & 0x02) != 0x02)
				{
					wid = *(dat[spe[i + startfrm].picIdx]);
                    wid += *(dat[spe[i + startfrm].picIdx] + 1)<<8;
					high = *(dat[spe[i + startfrm].picIdx] + 2);
                    high += *(dat[spe[i + startfrm].picIdx] + 3)<<8;
					x1 = x + spe[i + startfrm].x;
					y1 = y + spe[i + startfrm].y;

                    if(g_showPicType > 0)
                    {
                        wid/=5;
                        high/=5;
                    }

					if (y1 >= 0x80)
					{
						high = high - ((0xff ^ y1) + 1);
						y1 = 0;
					}
					high = y1 + high <= 96 ? high : (96 - y1);
        	
                    if (high < 0x80 && high + 1 > 1)// && g_showPicType == 0)
						gam_clrvscr(x1,y1,x1+wid-1,y1+high-1,g_VisScr);
				}
				clsflag = 1;
			}
			if (spec[i] + 1 != 1)
				spec[i] --;
		}
		if (clsflag == 1)
		{
			for (i = 0;i <= ymount;i ++)
			{
				if (spec[i] + 1 != 1)
				{
                    wid = *(dat[spe[i + startfrm].picIdx]);
                    wid += *(dat[spe[i + startfrm].picIdx] + 1)<<8;
                    high = *(dat[spe[i + startfrm].picIdx] + 2);
                    high += *(dat[spe[i + startfrm].picIdx] + 3)<<8;
					mode = *(dat[spe[i + startfrm].picIdx] + 5);
					x1 = x + spe[i + startfrm].x;
					y1 = y + spe[i + startfrm].y;
                    if(g_showPicType > 0)
                    {
                        wid/=5;
                        high/=5;
                    }

                    g_PicId = spe[i + startfrm].picIdx;

					if(mode==1)
						GamMPicShowV(x1,y1,wid,high,dat[spe[i + startfrm].picIdx] + sizeof(PictureHeadType),g_VisScr);
					else
						GamPicShowV(x1,y1,wid,high,dat[spe[i + startfrm].picIdx] + sizeof(PictureHeadType),g_VisScr);
				}
			}
		}
		if (showflag == 1)
		{
			for (i = ymount + 1;i <= mcount;i ++)
			if (spec[i] + 1 != 1)
			{
			wid = *(dat[spe[i + startfrm].picIdx]);
            wid += *(dat[spe[i + startfrm].picIdx] + 1)<<8;
			high = *(dat[spe[i + startfrm].picIdx] + 2);
            high += *(dat[spe[i + startfrm].picIdx] + 3)<<8;
			mode = *(dat[spe[i + startfrm].picIdx] + 5);
            if(g_showPicType > 0)
            {
                wid/=5;
                high/=5;
            }
			x1 = x + spe[i + startfrm].x;
			y1 = y + spe[i + startfrm].y;

             g_PicId = spe[i + startfrm].picIdx;

			if(mode==1)
				GamMPicShowV(x1,y1,wid,high,dat[spe[i + startfrm].picIdx] + sizeof(PictureHeadType),g_VisScr);
			else
				GamPicShowV(x1,y1,wid,high,(dat[spe[i + startfrm].picIdx] + sizeof(PictureHeadType)),g_VisScr);
			}
			ymount = mcount;
		}
		if (showflag == 1 || clsflag == 1)
		{
            GamShowFrame(g_VisScr);
			showflag = 0;
			clsflag = 0;
		}
		else
			GamDelay(1,false);				/* 延时1%秒 */
		if (spem[mcount] >= 1)
			spem[mcount] --;
		while ((spem[mcount] <= 1) && (mcount + startfrm < endfrm))
		{
			mcount ++;
			showflag = 1;
		}
		if (mcount + startfrm >= endfrm)
		{
			for (i = 0;i <= mcount;i ++)
			{
				if(spec[i] > 1)
					break;
			}
			if (i > mcount)
				break;
		}
        }
	return(0xff);
}
/***********************************************************************
 * 说明:     简单的菜单控件
 * 输入参数: pRect-菜单显示范围	pIdx-初始选中项	buf-菜单内容缓冲
 * 返回值  : 0xFF-跳出		其他-菜单选项
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 PlcSplMenu(RECT *pRect,U8 pIdx,U8 *buf)
{
	U8	sy,ty,tflag,cflag;
	U8	pLen;	/* 每个菜单项字符长度 */
	U8	pItm;	/* 菜单项总数 */
	U8	pSIdx;	/* 显示起始菜单项 */
	U8	pICnt;	/* 菜单显示区能显示的项数 */
	U16	poff;	/* 数据偏移 */
	float	tcot;
	GMType	pMsg;	/* 消息 */
	
	/* 初始化显示范围 */
	c_ReFlag = false;	
	c_Sx = pRect->sx;
	c_Ex = pRect->ex;
	c_Sy = pRect->sy;
	c_Ey = pRect->ey;
	/* 计算参数 */
	pLen = (c_Ex - c_Sx) / ASC_WID;
	pICnt = (c_Ey - c_Sy) / ASC_HGT;
	pItm = gam_strlen(buf) / pLen;
	pSIdx = 0;
	if(pICnt < pItm)
		pSIdx = pIdx;
	/* 初始化界面 */
	gam_clrlcd(c_Sx - 3,c_Sy - 3,c_Ex + 2,c_Ey + 2);
	gam_rect(c_Sx - 3,c_Sy - 3,c_Ex + 2,c_Ey + 2);
	if(pItm > pICnt)
	{
		gam_clrlcd(c_Ex + 2,c_Sy - 3,c_Ex + 6,c_Ey + 2);
		gam_rect(c_Ex + 2,c_Sy - 3,c_Ex + 6,c_Ey + 2);
		gam_rect(c_Ex + 2,c_Sy - 1,c_Ex + 6,c_Ey);
		if(pSIdx + pICnt > pItm)
			pSIdx = pItm - pICnt;
	}	
	poff = pSIdx;
	poff *= pLen;
	GamStrShowS(c_Sx,c_Sy,buf + poff);
	/* 初始化光标 */
	sy = c_Sy + (pIdx - pSIdx) * ASC_HGT;
	gam_revlcd(c_Sx,sy,c_Ex,sy + ASC_HGT);
	ty = c_Sy;
	cflag = true;
	while(1)
	{
        //usleep(1000);
		if(pItm > pICnt)
		{
			if(cflag)
			{	/* 驱动滚动条 */
				gam_rectc(c_Ex + 3,ty,c_Ex + 5,ty + 2);
				gam_putpixel(c_Ex + 4,ty + 1,COLOR_WHITE);
				tcot = pIdx;
				tcot /= pItm - 1;
				ty = tcot * (c_Ey - c_Sy - 3) + c_Sy;
				gam_rect(c_Ex + 3,ty,c_Ex + 5,ty + 2);
				cflag = false;
			}
			gam_revlcd(c_Ex + 3,ty,c_Ex + 5,ty + 2);
		}
			
		tflag = false;		
		GamGetMsg(&pMsg);

		if(VM_CHAR_FUN != pMsg.type)
			continue;
		gam_revlcd(c_Sx,sy,c_Ex,sy + ASC_HGT);
		switch(pMsg.param)
		{
		case VK_UP:
			if(!pIdx)
			{
				pIdx = pItm - 1;
				pSIdx = pItm - pICnt;
				poff = pSIdx * pLen;
				tflag = true;
				cflag = true;
				break;
			}
			if(pIdx)
			{
				pIdx -= 1;
				cflag = true;
			}
			if(pIdx < pSIdx)
			{
				pSIdx -= 1;
				poff -= pLen;
				tflag = true;
			}
			break;
		case VK_DOWN:
			if(pIdx == pItm - 1)
			{
				pIdx = 0;
				pSIdx = 0;
				poff = 0;
				tflag = true;
				cflag = true;
				break;
			}
			if(pIdx < pItm - 1)
			{
				pIdx += 1;
				cflag = true;
			}
			if(pIdx - pSIdx >= pICnt)
			{
				pSIdx += 1;
				poff += pLen;
				tflag = true;
			}
			break;
		case VK_EXIT:
			pIdx = MNU_EXIT;
		case VK_ENTER:
			c_ReFlag = true;
			c_Sx = WK_SX;
			c_Sy = WK_SY;
			c_Ex = WK_EX;
			c_Ey = WK_EY;
            g_stat = pIdx;
			return pIdx;
        }
        g_stat = pIdx;
		if(tflag)
			GamStrShowS(c_Sx,c_Sy,buf + poff);
		sy = c_Sy + (pIdx - pSIdx) * ASC_HGT;
		gam_revlcd(c_Sx,sy,c_Ex,sy + ASC_HGT);
	}
}
/***********************************************************************
 * 说明:     居中显示指定字符串
 * 输入参数: x,y-指定位置		buf-字符串
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void PlcMidShowStr(U8 x,U8 y,U8 *buf)
{
	U8	pLen;
	
	pLen = gam_strlen(buf) * ASC_WID;
	pLen >>= 1;
	x -= pLen;
	GamStrShowS(x,y,buf);
}
/***********************************************************************
 * 说明:     显示图形边框的消息框(只能显示一行)
 * 输入参数: buf-显示的消息	delay-显示延时(秒)
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void PlcGraMsgBox(U8 *buf,U8 delay,U8 line)
{
    U8	x,y;
    U16	w,h;
	U8	*ptr;
	
	ptr = ResLoadToCon(MSGBOX_PIC,1,g_CBnkPtr);
    if(g_showPicType == 0)
    {
        w = ptr[0];
        h = ptr[2];
    }
    else
    {
        w = ptr[0];
        w += ptr[1]<<8;
        h = ptr[2];
        h += ptr[3]<<8;
        w/=5;
        h/=5;
    }




	x = (WK_EX - WK_SX - w) >> 1;
	x += WK_SX;
	if(line != 0xFF)
		y = WK_SY + line * HZ_HGT;
	else
	{
		y = (WK_EY - WK_SY - h) >> 1;
		y += WK_SY;
	}
	GamPicShowS(x,y,w,h,ptr + sizeof(PictureHeadType));
    GamStrShowT(x + 10,y + 6,buf);
    GamDelay(delay * 100,true);
}
/***********************************************************************
 * 说明:     获取一个255以内的平方根(整数)
 * 输入参数: num-返回值的平方
 * 返回值  : 平方根
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U8 PlcExtract(U16 num)
{
	U8	gen,step;
	U8	tcnt;
	
	gen = 0;
	step = 10;
	while(1)
	{
		if(!step) break;
		tcnt = gen + step;
		if((U16)tcnt * tcnt < num)
			gen = tcnt;
		else
			step -= 1;		
	}
	return gen;
}
/***********************************************************************
 * 说明:     显示数字到虚拟屏幕
 * 输入参数: x,y-显示坐标	num-显示数字	dig-显示位数
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
const U32 FgtDigMask[10]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
 void PlcNumShow(U8 x,U8 y,U32 num,U8 dig,U8 *vs)
{
	U8	slen,*ptr,buf[12],numb[12];
	U32	tnum;
	
	ptr = buf;
	gam_memset(buf,0x30,12);
	tnum = num % FgtDigMask[dig];
    gam_ltoa(tnum,numb,10);
    slen = gam_strlen(numb);
	if(slen <= dig && num >= FgtDigMask[dig])
		ptr += dig - slen;

	gam_memcpy(ptr,numb,slen + 1);
	if(vs == NULL)
		GamStrShowS(x,y,buf);
	else
		GamStrShowV(x,y,buf,vs);
}
/***********************************************************************
 * 说明:     显示指定的资源图片到虚拟屏幕
 * 输入参数: id,idx-指定要显示的对象	x,y-显示的坐标	flag-true屏幕 false内存
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void PlcRPicShow(U16 id,U8 idx,U8 x,U8 y,U8 flag)
{
	U8	*pic;
    U16	wid,high;
    U8 mode;
    U32	off;

    //DebugTime(51);
	pic = ResLoadToCon(id,1,g_CBnkPtr);
	if(NULL == pic)
		return;
	if(idx > pic[4])
		return;
    g_PicId = idx;
    //DebugTime(52);
    if(g_showPicType == 0)
    {
        wid = pic[0];
        high = pic[2];
        mode = pic[5] & 0x01;
        off = wid >> 3;
        if(wid & 0x07)
            off += 1;
        off *= high;
        off <<= mode;
        off *= idx - 1;
        pic += off;
        pic += PICHEAD_LEN;
    }
    else if(g_showPicType == 1)
    {
        wid = pic[0];
        wid += pic[1]<<8;
        high = pic[2];
        high += pic[3]<<8;
        mode = pic[5] & 0x01;

        int width2 = ( wid * 24 + 31 )/32 * 4;//每行字节数
        off = high * width2;
        off <<= mode;

        off *= idx - 1;
        pic += off;
        pic += PICHEAD_LEN;
        //DebugTime(53);

    }
    else if(g_showPicType == 2)
    {
        wid = pic[0];
        wid += pic[1]<<8;
        high = pic[2];
        high += pic[3]<<8;
        mode = pic[5] & 0x01;

        off = PICHEAD_LEN;
        if(mode == 0)
        {
            int ii = 0 ;
            for(ii = 0 ; ii < (idx -1); ii ++)
            {
                U32 size  = ((PictureHeadTypeJPG *)(pic+off))->size;
                off += (size + 4);
            }
        }
        else
        {
            int ii = 0 ;
            for(ii = 0 ; ii < (idx -1) * 2; ii ++)
            {
                U32 size  = ((PictureHeadTypeJPG *)(pic+off))->size;
                off += (size + 4);
            }
        }



        pic += off;
        //DebugTime(53);

    }

    if(g_showPicType > 0)
    {
        wid/=5;
        high/=5;
    }
	if(!flag)
	{
		if(mode)
			GamMPicShowV(x,y,wid,high,pic,g_VisScr);
		else
			GamPicShowV(x,y,wid,high,pic,g_VisScr);
	}
	else
	{
		if(mode)
			GamMPicShowS(x,y,wid,high,pic);
		else
			GamPicShowS(x,y,wid,high,pic);
	}
    //DebugTime(54);
}
/***********************************************************************
 * 说明:     获取指定将领的兵力上限
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 U16 PlcArmsMax(U8 id)
{
	U16	up;
	PersonType	*per;
	
	per = &g_Persons[id];
	up = (U16)(per->Force + per->IQ) * 10 + (U16)per->Level * 100;
	
	return (U16)up;
}
/***********************************************************************
 * 说明:     显示指定字符串到big内small外的区域中
 * 输入参数: big,small-指定区域	buf-字符串
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
 void PlcStrShowS(RECT *big,RECT *small,U8 *buf)
{
	U8	*ptr,i,tmp;
	U8	sx[4],sy[4],ex[4],ey[4];
	U16	sLen;
	
	tmp = big->sx;
	sx[0] = tmp;
	sx[1] = tmp;
	sx[3] = tmp;
	sx[2] = small->ex;
	sy[0] = big->sy;
	tmp = small->sy;
	sy[1] = tmp;
	sy[2] = tmp;
	sy[3] = small->ey;
	tmp = big->ex;
	ex[0] = tmp;
	ex[2] = tmp;
	ex[3] = tmp;
	ex[1] = small->sx;
	ey[0] = small->sy;
	tmp = small->ey;
	ey[1] = tmp;
	ey[2] = tmp;
	ey[3] = big->ey;
	
	ptr = buf;
    sLen = gam_strlen(buf);

    g_noUseStrMem = 1;
    SysCopyFromDcToStr();
	for(i = 0;i < 4;i += 1)
	{
		ptr = PlcItemShowS(sx[i],sy[i],ex[i],ey[i],ptr);
		if(NULL == ptr)
        {
            g_noUseStrMem = 0;
            SysCopyToDcStr();
			return;
        }
	}
    g_noUseStrMem = 0;
    SysCopyToDcStr();
}
/***********************************************************************
 * 说明:     根据当前范围按照项截取显示指定的字符串
 * 输入参数: sx,sy,ex,ey-指定范围		buf-要显示的项字符串(项的分割符是'|')
 * 返回值  : 下次要显示时的数据指针
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 *PlcItemShowS(U8 sx,U8 sy,U8 ex,U8 ey,U8 *buf)
{
	U8	*ptr,len;
	
	len = (ex - sx) / ASC_WID;
	len = len * ((ey - sy) / HZ_HGT);
    if(gam_strlen(buf) < len)
		ptr = (U8 *)NULL;
	else
	{
		ptr = strlchr(buf,len,'|');
		if(NULL == ptr)
		{
			if(!len)
				return buf;
			else
				ptr = buf + len;
		}
		else
		{
			*ptr = '\0';
			ptr += 1;
		}
	}
	c_Sx = sx;
	c_Sy = sy;
	c_Ex = ex;
	c_Ey = ey;
	GamStrShowS(sx,sy,buf);
	return ptr;
}
/***********************************************************************
 * 说明:     从len处向左查找字符串buf中给定的字符ch，并返回指针。
 * 输入参数: sx,sy,ex,ey-指定范围		buf-要显示的项字符串(项的分割符是'|')
 * 返回值  : !NULL-查找成功	NULL-查找失败
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
***********************************************************************/
U8 *strlchr(U8 *buf,U8 len,U8 ch)
{
	U8	i;
	
	for(i = len;i-0 > 0;i -= 1)
	{
		if(buf[i] == ch)
			return (buf + i);
	}
	return ((U8 *)NULL);
}

U8 *itoa_my(int value,U8 *string,int radix)
{
    char zm[37]="0123456789abcdefghijklmnopqrstuvwxyz";
    char aa[100]={0};

    int sum=value;
    char *cp=(char*)string;
    int i=0;

    if(sum ==0)
    {
        *cp=48;
        *(cp+1)='\0';
        return string;
    }

    if(radix<2||radix>36)//增加了对错误的检测
    {
        return string;
    }

    if(value<0)
    {
        return string;
    }


    while(sum>0)
    {
        aa[i++]=zm[sum%radix];
        sum/=radix;
    }
    int  j;
    for(j=i-1;j>=0;j--)
    {
        *cp++=aa[j];
    }
    *cp='\0';
    return string;
}

U8 *ltoa_my(long value,U8 * string,int radix)
{
    char zm[37]="0123456789abcdefghijklmnopqrstuvwxyz";
    char aa[100]={0};

    long sum=value;
    char *cp=(char*)string;
    int i=0;

    if(sum ==0)
    {
        *cp=48;
        *(cp+1)='\0';
        return string;
    }

    if(radix<2||radix>36)//增加了对错误的检测
    {
        return string;
    }

    if(value<0)
    {
        return string;
    }


    while(sum>0)
    {
        aa[i++]=zm[sum%radix];
        sum/=radix;
    }

    int j;
    for(j=i-1;j>=0;j--)
    {
        *cp++=aa[j];
    }
    *cp='\0';
    return string;
}

