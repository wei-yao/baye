#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#ifndef	_COMM_H
#define	_COMM_H

/*======================================
  引擎程序相对于环境独立性
======================================*/
//#define bool unsigned char

/*------------------------
  字库常量定义
  ------------------------*/
#define	GAM_FONT_BNK_NUM	40			/* 游戏字库占用4K的页面个数 */
#define	GAM_FONT_ASC_QU		0xA4			/* Ascii字模在游戏字库中存放的区 */

/*------------------------
  外部内存常量
  ------------------------*/
#define	EXTMEM_BNKNUM		4			/* 外部内存使用空间为内存第4页面(地址 16K~20K) */
//#define	VS_PTR			(U8 *)0x4800		/* 虚拟屏幕缓冲 */
#define	BVS_PTR			(U8 *)0x4000		/* 二级虚拟屏幕缓冲 */
//#define	SHARE_MEM		(U8 *)0x4000		/* 共享临时内存 */
//#define FIGHTERS_IDX		(U8 *)0x4190		/* 出征武将队列索引(30个字节) */
//#define FIGHTERS		(U8 *)0x41B0		/* 出征武将队列(30*10=300个字节) */
//#define	ORDERQUEUE		0x42E0			/* 命令队列(12*100=1200个字节) */

/*------------------------
  消息结构定义
  ------------------------*/
typedef	struct{
	U8	type;					/* 消息类型 */
	U16	param;					/* 消息的值 */
}GMType;

/*------------------------
  消息类型定义
  ------------------------*/
#define		VM_DUMMY		0x00		/* 无效消息 */
#define		VM_KEY			0x01		/* 按键值，只有行列信息 */
#define		VM_CHAR_ASC		0x02		/* 输入 ASCII码 */
#define		VM_CHAR_HZ		0x03		/* 输入汉字 */
#define		VM_CHAR_MATH		0x04		/* 数学功能键 */
#define		VM_CHAR_FUN		0x05		/* 输入功能键 */
#define		VM_TIMER		0x06		/* 定时到 */
#define		VM_COM			0x07		/* 串口接收到数据 */
#define		VM_POWER		0x08		/* 电源 */
#define		VM_ALERT		0x09		/* 闹钟到 ，行程到 */
#define		VM_COMMAND		0x0a		/* 界面相关 */	

/*------------------------
  转换后的扩展键  ---  功能键
  说明：功能键对应消息类型WM_CHAR_FUN,取消息值(param)的低字节
  ------------------------*/
#define		VK_PGUP				0x21
#define		VK_PGDN				0x22
#define		VK_UP				0x26
#define		VK_DOWN				0x28
#define		VK_LEFT				0x25
#define		VK_RIGHT			0x27
#define		VK_HELP				0x2F
#define		VK_ENTER			0x0D
#define		VK_EXIT				0x1B

#define		VK_INSERT			0x2D
#define		VK_DEL				0x2E
#define		VK_MODIFY			0x32
#define		VK_SEARCH			0x33
#define		VK_SHIFT			0x10			/* shift */
#define		VK_SYM				0x35			/*  符号 */

/*------------------------
  其它结构体定义
  ------------------------*/
typedef	struct {
	U8	sx;			/* 矩形左 */
	U8	sy;			/* 矩形上 */
	U8	ex;			/* 矩形右 */
	U8	ey;			/* 矩形下 */
}RECT;


/*------------------------
  函数
  ------------------------*/
  #ifdef __cplusplus
extern "C" {
#endif
    U8	GamConInit(void);						/* 游戏系统环境初始化，根据不同环境调用不同的初始化函数 */
    U8	GamConRst(void);						/* 游戏系统环境恢复，根据不同环境调用不同的恢复函数 */
    U8	GamGetMsg(GMType *pMsg);					/* 封装系统的输入，并将其转换成游戏的消息机制 */
    U8	GamDelay(U16 dly,bool keyflag);					/* 1%秒延时 */
    void	GamMsgBox(U8 *buf,U8 delay);					/* 信息框 */
    void	GamShowFrame(U8 *vscr);						/* 将虚拟屏幕show到屏幕上 */
    void	GamPicShowS(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic);			/* 显示图片到屏幕 */
    void	GamPicShowV(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr);		/* 显示图片到虚拟屏幕 */
    void	GamMPicShowS(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic);			/* 显示mask图片到屏幕 */
    void	GamMPicShowV(U8 x,U8 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr);		/* 显示mask图片到虚拟屏幕 */
    void	GamPicShowExS(U8 x,U8 y,U16 wid,U16 hgt,U8 idx,U8 *pic);		/* 显示图片到屏幕(功能扩展——可显示图片的部分) */
    void	GamPicShowExV(U8 x,U8 y,U16 wid,U16 hgt,U8 idx,U8 *pic,U8 *vscr);	/* 显示图片到虚拟屏幕(功能扩展——可显示图片的部分) */
    void	GamStrShowS(U8 x,U8 y,U8 *str);					/* 显示12汉字到屏幕 */
    void	GamStrShowT(U8 x,U8 y,U8 *str);					/* 显示12汉字到屏幕 */
    void	GamAsciiS(U8 x,U8 y,U8 asc);					/* 显示6x12ASCII字符到屏幕 */
    void	GamStrShowV(U8 x,U8 y,U8 *str,U8 *vscr);			/* 显示12汉字到虚拟屏幕 */
    //void	GamePictureDummy(U8 l,U8 t,U8 r,U8 b,U8* p,U8* s,U8 f);		/* 显示图片到虚拟屏幕 */
    void	GamPicSave(U16 x,U16 y,U16 wid,U16 hgt,U8 *pic,U8 *vscr,U8 type);		/* 显示图片到虚拟屏幕 */
    void ModAddonReplace();
    extern U8 VS_PTR[15360+1];
    extern U8 SHARE_MEM[8000000+1];//2097152+1];76958913
    extern U8 FIGHTERS_IDX[30+1];
    extern U8 FIGHTERS[300+1];
    extern U8 ORDERQUEUE[1200+1];
#ifdef __cplusplus
}
#endif
#endif	/* _COMM_H */
