
#ifndef	_COMPA_H
#define	_COMPA_H

/*======================================
  兼容性宏
======================================*/
#define	LCD_CODE_H	0		/* 屏幕编码方式——横 */
#define	LCD_CODE_S	1		/* 屏幕编码方式——纵 */

//typedef int bool;

/*------------------------
  A系列电子词典的宏替换
  ------------------------*/
#ifdef	_EDICT_SERIES_A_
/*+++++++++++++++
  通用函数的宏替换
  +++++++++++++++*/
/* 随机函数 */
//#define	gam_srand(seed)		srand(seed)		/* 替换A系列系统中的随机初始化函数 */
//#define	rand()  rand()			/* 替换A系列系统中的随机函数 */
/* 内存函数 */
#define	gam_memcpy(a,s,l)	memcpy(a,s,l)			/* 替换A系列系统中常驻的内存拷贝函数 */
#define	gam_memset(buf,val,len)	memset(buf,val,len)			/* 替换A系列系统中的内存填充函数 */
#define	gam_malloc(len)		(U8*)malloc(len)			/* 替换A系列系统中堆申请函数 */
#define	gam_free(ptr)		free(ptr)				/* 替换A系列系统中堆释放函数 */
/* 字符串函数 */
#define	gam_strcmp(str1,str2)	strcmp(str1,str2)
#define	gam_strcat(str1,str2)	strcat((char*)str1,(char*)str2)
#define	gam_strlen(str)		strlen((char*)str)
#define	gam_itoa(i,str,bdh)	itoa_my(i,str,bdh)
#define	gam_ltoa(l,str,bdh)	ltoa_my(l,str,bdh)
#define	gam_atoi(str)		atoi(str)
#define	gam_atol(str)		atol(str)
/* 屏幕操作函数 */
#define	gam_clrlcd(l,t,r,b)	SysLcdPartClear(l,t,r,b)		/* 清除屏幕矩形 */
#define	gam_clrvscr(l,t,r,b,v)	GamePictureDummy(l,t,r,b,(U8*)NULL,v,4)	/* 清空虚拟屏幕指定区域 */
#define	gam_clslcd()		SysLcdPartClear(0,0,SCR_WID-1,SCR_HGT-1)/* 清除整个屏幕 */
#define	gam_revlcd(l,t,r,b)	SysLcdReverse(l,t,r,b)			/* 反显屏幕 */
#define	gam_putpixel(x,y,c)	SysPutPixel(x,y,c)			/* 画点函数 */
#define	gam_line(l,t,r,b)	SysLine(l,t,r,b)			/* 显示直线 */
#define	gam_linec(l,t,r,b)	SysLineClear(l,t,r,b)			/* 消隐直线 */
#define	gam_rect(l,t,r,b)	SysRect(l,t,r,b)			/* 显示矩形 */
#define	gam_rectc(l,t,r,b)	SysRectClear(l,t,r,b)			/* 消隐矩形 */
#define	gam_getkey()		SysGetKey()				/* 有按键获取，无按键返回 */

/*+++++++++++++++
  系统专有函数的替换(这些宏只能由中间层来调用，游戏引擎的上层程序不允许调用这些宏)
  +++++++++++++++*/
#ifdef	_BE_CHANGED_
#define	gam_Picture(l,t,r,b,p)	SysPicture(l,t,r,b,p,0)			/* 替换A系列系统中的图片显示程序 */
#endif	/* _BE_CHANGED_ */

#if	(GAM_PHY_SET == 4980)
	#define	LCD_CODE	LCD_CODE_H
#endif

#if	(GAM_PHY_SET == 3288)
	#define	LCD_CODE	LCD_CODE_H
#endif

#if	(GAM_PHY_SET == 3980)
	#define	LCD_CODE	LCD_CODE_S
#endif

#if	(GAM_PHY_SET == 6980)
	#define	LCD_CODE	LCD_CODE_S
#endif

#endif	/* _EDICT_SERIES_A_ */

/*------------------------
  符号定义
  ------------------------*/
#ifndef	U8
typedef	unsigned char	U8;
typedef	unsigned short	U16;
typedef	unsigned int	U32;
#endif


#ifndef	true
#define	true	1
#define	false	0
#endif

#ifndef	NULL
#define	NULL	0
#endif

#endif	/* _COMPA_H */


