#ifndef	_FSYS_H
#define	_FSYS_H

#include <stdio.h>
/*======================================
  文件系统再封装
======================================*/

/*------------------------
  文件结构体指针
  ------------------------*/
typedef	struct {
    FILE *handle;			/* 文件操作句柄 */
	U8	ftype;			/* 文件类型 */
	U8	openmode;		/* 打开方式 */
    U8	fname[200];		/* 文件名 */
	U32	curset;			/* 当前文件指针位置 */
	U32	flen;			/* 文件长度 */
}gam_FILE;				/* 文件指针类型 */

/*------------------------
  文件fseek是起始位置宏
  ------------------------*/
#define	SEEK_SET	0		/* 从文件开头 */
#define	SEEK_CUR	1		/* 从文件指针的现行位置 */
#define	SEEK_END	2		/* 从文件末尾 */

#define	OPEN_R		1		/* 读模式 */
#define	OPEN_W		2		/* 写模式 */
#define	OPEN_ADD	3		/* 追加模式 */

#define	FILE_FONT	1		/* 字库文件 */
#define	FILE_LIB	2		/* 资源库文件 */
#define	FILE_NOR	3		/* 普通文件 */
#define	FILE_RLB	4		/* 资源库文件 */
#define	FILE_MOD	5		/* MOD文件 */
#define	DAT_ADDR	0x9000		/* 数据页面起始地址 */

#define	GAME_SAVE	9		/*游戏存盘文件*/
#define GAME_TEMP	23		/*游戏临时文件*/


/*------------------------
  函数声明
  ------------------------*/
 gam_FILE *gam_fopen(U8 *fname,U8 pmode);
 U8 gam_fclose(gam_FILE *fhandle);
 U8 gam_fseek (gam_FILE *fhandle, U32 offset, U8 fromwhere);
 U32 gam_fread(U8 *buf, U8 size, U16 count, gam_FILE *fhandle);
 U32 gam_fwrite(U8 *buf, U8 size, U16 count, gam_FILE *fhandle);
 U32 gam_ftell(gam_FILE *fhandle);
 U8 *gam_fload(U8 *bptr, U32 addr, gam_FILE *fhandle);
 U8 *gam_fload_mem(U8 *bptr, U32 addr, gam_FILE *fhandle);
 U8 gam_frename(gam_FILE *fhandle,U8 *newname);
 U8 gam_fdelete(gam_FILE *fhandle);

#endif	/* _FSYS_H */
