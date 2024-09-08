/***********************************************************************
*Copyright (c)2005 , 东莞步步高教育电子分公司
*All rights reserved.
**
文件名称：	fsys.c
*文件标识：	步步高电子词典的游戏引擎模块
*摘要：		游戏系统对小机文件系统的封装
**
*移植性声明:
*	1、符合标准：《游戏设计标准V1.0》
*	2、兼容模式：本程序和界面无关，无兼容模式。
*	3、小机环境发生变化是，需要修改本文件到相应的文件系统
*修改历史：
*	版本    日期     作者     改动内容和原因
*   ----------------------------------------------------
*	1.0    2005.5.16  高国军     基本的功能完成
***********************************************************************/
#include "stdsys.h"
#include "comm.h"
#include "enghead.h"
#include <stdio.h>
#include <string.h>
#ifdef	_EDICT_SERIES_A_

extern U8 libByte[300000+1];
extern U8 fontByte[163840+1];
extern U8 rlbByte[50000000+1];
//U8 libByteA[300000+1];
//U8 rlbByteA[50000000+1];
//U8 fontByteA[163840+1];

extern int g_ResId;
extern U8 g_startName_char[200];
/***********************************************************************
 * 说明:     将当前文件系统的文件打开函数封装为标准的fopen函数
 * 输入参数: fname-文件名	mode-操作方式
 * 返回值  : 文件操作句柄
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 gam_FILE *gam_fopen(U8 *fname,U8 pmode)
{
    U8	err,mode;
    FILE * pFHandle;
    U8	tmpbuf[200],pInf[12];
	U16	pfNum,i,tmpName;
	U32	pLen;
	gam_FILE *tmpFile;

    memset(tmpbuf,0,sizeof (tmpbuf));
	tmpFile=(gam_FILE *)gam_malloc(sizeof(gam_FILE)+4);
	if(NULL == tmpFile)
		return (gam_FILE *)NULL;
	if(pmode=='w')
		mode=OPEN_W;
	else if(pmode=='r')
		mode=OPEN_R;
	else	/* if(pmode=='a') */
		mode=OPEN_ADD;
	
    gam_memcpy(tmpFile->fname,fname,200);
	tmpFile->curset=0;
	tmpFile->openmode=mode;	
	tmpFile->flen=0;

    gam_memcpy(tmpbuf,GAM_FONT_FNAME,10);
    if(!strcmp((const char*)tmpbuf,(const char*)fname))
    {
        tmpFile->ftype=FILE_FONT;
        pFHandle = FileOpen(tmpbuf,FILE_FONT,"rb",pFHandle,&pLen);
        tmpFile->handle=pFHandle;
        tmpFile->flen=pLen;

        fseek(pFHandle,0,SEEK_SET);
        fread((char *)fontByte,1,(int)pLen,pFHandle);

        return (gam_FILE *)tmpFile;
    }

    if(g_useMOD == 0)
    {
        gam_memcpy(tmpbuf,GAM_LIB_FNAME,10);


        if(!strcmp((const char*)tmpbuf,(const char*)fname))
        {
            tmpFile->ftype=FILE_LIB;
            pFHandle = FileOpen(tmpbuf,FILE_LIB,"rb",pFHandle,&pLen);

            tmpFile->handle=pFHandle;
            tmpFile->flen=pLen;

            fseek(pFHandle,0,SEEK_SET);
            fread((char *)libByte,1,(int)pLen,pFHandle);

            return (gam_FILE *)tmpFile;
        }


        gam_memcpy(tmpbuf,GAM_RLB_FNAME,10);

        if(!strcmp((const char*)tmpbuf,(const char*)fname))
        {
            tmpFile->ftype=FILE_RLB;
            pFHandle = FileOpen(tmpbuf,FILE_RLB,"rb",pFHandle,&pLen);
            tmpFile->handle=pFHandle;
            tmpFile->flen=pLen;

            fseek(pFHandle,0,SEEK_SET);
            fread((char *)rlbByte,1,(int)pLen,pFHandle);

            return (gam_FILE *)tmpFile;
        }
    }
    else
    {
        gam_memcpy(tmpbuf,g_startName_char,200);

        if(!strcmp((const char*)tmpbuf,(const char*)fname))
        {
            tmpFile->ftype=FILE_MOD;
            pFHandle = FileOpen(tmpbuf,FILE_MOD,"rb",pFHandle,&pLen);
            tmpFile->handle=pFHandle;
            tmpFile->flen=pLen;

            fseek(pFHandle,0,SEEK_SET);
            fread((char *)libByte,1,(int)300000,pFHandle);

            fseek(pFHandle,0,SEEK_SET+300000);
            fread((char *)rlbByte,1,(int)(pLen-300000),pFHandle);

            return (gam_FILE *)tmpFile;
        }
    }


	
	/* 正常文件 */
	tmpFile->ftype=FILE_NOR;

    /*FileNum(GAME_SAVE,&pfNum);
    err = 1;
    for (i = 1;i <= pfNum;i ++)
    {
        gam_memset(pInf,0,12);
        FileSearch(GAME_SAVE,i,&tmpName,pInf);
        err = gam_strcmp(fname,pInf);
        if(!err) break;
    }*/

    //文件存在
    err = 0;//20180304不再判断
    /*FILE *fp=NULL;
    fp = fopen(fname,"rb");//二进制读，追加
    if(fp == NULL)
    {
       err = 1;

    }
    else
    {
        err = 0;
        fclose(fp);
    }*/

	/* 没找到文件，且不是写模式 */	
	if(err && OPEN_W != mode)
	{
		gam_free((U8 *)tmpFile);
		return (gam_FILE *)NULL;
	}
	/* 找到了文件 */
    if(!err && OPEN_W != mode)//20180304非写模式
	{
        pFHandle = FileOpen(fname,GAME_SAVE,"rb+",pFHandle,&pLen);
        if(pFHandle == NULL)//20180304判不存在
        {
            gam_free((U8 *)tmpFile);
            return (gam_FILE *)NULL;
        }
		if(OPEN_W != mode)
		{
			tmpFile->handle=pFHandle;
			tmpFile->flen=pLen;
			return (gam_FILE *)tmpFile;
		}
        tmpFile->handle=pFHandle;
        FileDel((gam_FILE *)tmpFile);
	}
	/* 写模式下，新建文件 */
	pLen = 0x1000;
    pFHandle = FileCreat(GAME_SAVE,pLen,fname,pFHandle);
    if(pFHandle == NULL)
    {
       err = 0;

    }
    else
    {
        err = 1;
    }

	if(!err)
	{
		gam_free((U8 *)tmpFile);
		return (gam_FILE *)NULL;
	}
	tmpFile->handle=pFHandle;
	tmpFile->flen=pLen;
	return (gam_FILE *)tmpFile;
}
/***********************************************************************
 * 说明:     将当前文件系统的文件关闭函数封装为标准的fclose函数
 * 输入参数: fhandle-文件操作句柄
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 gam_fclose(gam_FILE *fhandle)
{
	if(fhandle->ftype==FILE_NOR)
        FileClose((gam_FILE *)fhandle);
	gam_free((U8 *)fhandle);
}
/***********************************************************************
 * 说明:     将当前文件系统的文件指针定位函数封装为标准的fseek函数
 * 输入参数: fhandle-操作句柄	offset-偏移	fromwhere-起始
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 gam_fseek (gam_FILE *fhandle, U32 offset, U8 fromwhere)
{
	U32	tmp;
		
	tmp=fhandle->curset;
	if(!fromwhere)
		tmp=offset;
	if(fromwhere==SEEK_CUR)
		tmp+=offset;	
	if(fromwhere==SEEK_END)
		tmp=fhandle->flen-offset;
	fhandle->curset=tmp;
}
/***********************************************************************
 * 说明:     将当前文件系统的文件读取函数封装为标准的fread函数
 * 输入参数: buf-缓冲	size-项长	count-项数	fhandle-句柄
 * 返回值  : 0-操作成功	!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U32 gam_fread(U8 *buf, U8 size, U16 count, gam_FILE *fhandle)
{
	U8	*ptr,tmp;
	U16	i,lp,bnk;
	U32	pLen,roff,addr;
	
	if(fhandle->openmode!=OPEN_R)
		return 0;
	pLen = size;
	pLen *= count;
	
	addr=fhandle->curset;
	bnk=(U16)(addr>>14);
	bnk<<=2;
    //bnk+=c_FontBnkS;
	switch(fhandle->ftype)
	{
	case FILE_LIB:
    {


        //FILE *fp=NULL;
        //fp = fopen(GAM_LIB_FNAME,"rb+");//二进制读，追加
        //if(!fp)return -1;//出错处理
        //fseek(fp,fhandle->curset,SEEK_SET);
        //\fseek(fhandle->handle,fhandle->curset,SEEK_SET);
        //len = ftell(fp);
        //fseek(fp,0L,SEEK_SET);
        //\fread(buf,1,(int)pLen,fhandle->handle);
        memcpy(buf,libByte + fhandle->curset,pLen);
        //fclose(fp);

        //gam_memcpy(buf,ptr,(U16)pLen);
		break;
    }
	case FILE_FONT:		
    {

        //FILE *fp1=NULL;
        //fp1 = fopen(GAM_FONT_FNAME,"rb+");//二进制读，追加
        //if(!fp1)return -1;//出错处理
        //fseek(fp1,fhandle->curset,SEEK_SET);
        //\fseek(fhandle->handle,fhandle->curset,SEEK_SET);
        //\fread(buf,1,(int)pLen,fhandle->handle);
        memcpy(buf,fontByte + fhandle->curset,pLen);
        //fread(buf,1,18,fp1);
        //fclose(fp1);

        //lp=(U16)(addr&0x00003fff);
        //if(0x4000-lp<18)
        //	tmp=0x4000-lp;			/* 跨bnk取数据 */
        //else
        //	tmp=18;
        //gam_memcpy(buf,(U8 *)(lp+DAT_ADDR),tmp);
        //if(tmp<18)				/* 跨页面字模再补取数据 */
        //{
        //	gam_memcpy((U8 *)(buf+tmp),(U8 *)DAT_ADDR,18-tmp);
        //}
		break;
    }
    case FILE_RLB:
    {
        //\fseek(fhandle->handle,fhandle->curset,SEEK_SET);
        //\fread(buf,1,(int)pLen,fhandle->handle);
        memcpy(buf,rlbByte + fhandle->curset,pLen);
        break;
    }

	case FILE_NOR:
		ptr = (U8 *)&g_GenPos;			/* 复用战斗模块的全局变量 */
		lp = pLen / 100;
		roff = 0;
		for(i = 0;i < lp;i += 1)
		{
			FileRead(fhandle->handle,100,ptr);
			gam_memcpy(buf+roff,ptr,100);
			roff += 100;
		}
		lp = (U8)(pLen - roff);
		FileRead(fhandle->handle,lp,ptr);
		gam_memcpy(buf+roff,ptr,lp);
		break;
	}
	fhandle->curset+=pLen;
	return pLen;
}
/***********************************************************************
 * 说明:     将当前文件系统的文件写入函数封装为标准的fwrite函数
 * 输入参数: fname-文件名		mode-操作方式
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U32 gam_fwrite(U8 *buf, U8 size, U16 count, gam_FILE *fhandle)
{
	U8	*ptr;
	U16	i,lp;
	U32	pLen,pOff;

	if(fhandle->ftype!=FILE_NOR)
		return 1;

	ptr = (U8 *)&g_GenPos;			/* 复用战斗模块的全局变量 */
	pLen=count;
	pLen*=size;
	lp=pLen / 100;
	pOff=0;
	for(i=0;i<lp;i++)
	{
		gam_memcpy(ptr,buf+pOff,100);
		pOff+=100;
		FileWrite(fhandle->handle,100,ptr);
	}
	lp = pLen-pOff;
	gam_memcpy(ptr,buf+pOff,lp);
	FileWrite(fhandle->handle,(U8)lp,ptr);
	fhandle->curset+=pLen;
	return 0;
}
/***********************************************************************
 * 说明:     将当前文件系统的文件指针查询函数封装为标准的ftell函数
 * 输入参数: fname-文件名	mode-操作方式
 * 返回值  : 资源项的数据长度
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U32 gam_ftell(gam_FILE *fhandle)
{
	return (U32) fhandle->curset;
}
/***********************************************************************
 * 说明:     增强功能函数—按照页面方式将指定地址下的数据载入到常量页
 * 输入参数: bptr-常量页地址	addr-指定地址	fhandle-文件句柄
 * 返回值  : 被载入数据指针
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 *gam_fload(U8 *bptr, U32 addr, gam_FILE *fhandle)
{
    U8	* ptr,type;
	U16	bnk,lp;

	type=fhandle->ftype;
	if(FILE_NOR==type)
		return (U8 *)NULL;
	
    //lp=(U16)(addr&0x00003fff);
    ptr=(U8 *)(bptr);
    //bnk=(U16)(addr>>14);
    //bnk<<=2;
    //bnk+=c_FontBnkS;
    //if(FILE_LIB==type)
    //	bnk+=GAM_FONT_BNK_NUM;

    /*fhandle->curset = addr;
    fseek(fhandle->handle,(int)fhandle->curset,SEEK_SET);
    int len = (int)fhandle->flen - (int)fhandle->curset;*/



    //fread((char *)ptr,1,len,fhandle->handle);

    if(FILE_LIB==type)
    {
        //fseek(fhandle->handle,0,SEEK_SET);
        //fread((char *)libByte,1,(int)fhandle->flen,fhandle->handle);
        ptr = libByte+addr;
    }
    else if(FILE_RLB==type)
    {
        //fseek(fhandle->handle,0,SEEK_SET);
        //fread((char *)rlbByte,1,(int)fhandle->flen,fhandle->handle);
        ptr = rlbByte+addr;
    }




	return (U8 *)ptr;
}

 U8 *gam_fload_mem(U8 *bptr, U32 addr, gam_FILE *fhandle)
{
    U8	* ptr,type;
    U16	bnk,lp;

    type=fhandle->ftype;
    ptr=(U8 *)(bptr);
    if(FILE_LIB==type)
    {
        //int len = (int)fhandle->flen - (int)addr;
        //if(len > 20000) len = 20000;
        //memcpy((char *)ptr,libByte+addr,len);
        ptr = libByte+addr;
    }
    else if(FILE_RLB==type)
    {
        //int len = (int)fhandle->flen - (int)addr;
        /*if(g_ResId >= 48 && g_ResId <= 51)
        {
            if(len > 8700000) len = 8700000;
        }
        else
        {
            if(len > 2200000) len = 2200000;
        }
        //memcpy((char *)ptr,rlbByte+addr,len);*/
        //memcpy((char *)ptr,rlbByte+addr,g_ResIdLen[g_ResId]);
        ptr = rlbByte+addr;
    }

    return (U8 *)ptr;
}
/***********************************************************************
 * 说明:     增强功能函数—修改文件名
 * 输入参数: fhandle-文件句柄	newname-新的文件名
 * 返回值  : 0-操作成功		!0-出错代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 gam_frename(gam_FILE *fhandle,U8 *newname)
{
	if(fhandle->ftype!=FILE_NOR)
		return 1;
    FileChangeInf(fhandle,newname);
	return 0;
}
/***********************************************************************
 * 说明:     增强功能函数—文件删除
 * 输入参数: fhandle-文件句柄
 * 返回值  : 0-操作成功		!0-出错代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.18       完成基本功能
***********************************************************************/
 U8 gam_fdelete(gam_FILE *fhandle)
{
	if(fhandle->ftype!=FILE_NOR)
		return 1;
    FileDel((gam_FILE *)fhandle);
}
#endif	/* _EDICT_SERIES_A_ */

