#ifndef	_RESOURCE_H
#define	_RESOURCE_H

/*======================================
  资源管理头文件
======================================*/

typedef struct {
	U32	ResLen;			/* 资源串总长度 */
	U16	ResId;			/* 资源串id */
	U16	ItmCnt;			/* 资源项个数 */
	U16	ItmLen;			/* 资源项长度(0表示资源项不等长，后面会有资源项索引序列) */
	U8	ResKey;			/* 资源串加密钥匙(加密算法统一使用+ResKey) */
	U8	Reserved;		/* 保留 */
}RCHEAD;				/* 资源串头定义 */

typedef struct {
	U16	offset;			/* 资源项相对资源起始地址的偏移地址 */
	U16	rlen;			/* 资源项的数据长度 */
}RIDX;					/* 资源项索引结构 */

/*------------------------
  函数
  ------------------------*/
 U16 ResGetItemLen(U16 ResId,U8 idx);
 U8 *ResLoadToCon(U16 ResId,U8 idx,U8 *cbnk);
 U8 ResLoadToMem(U16 ResId,U8 idx,U8 *ptr);
 U8 ResLoadToMemOld(U16 ResId,U8 idx,U8 *ptr);
 U8 ResLoadToMemNew(U16 ResId,U8 idx,U8 *ptr);
 //U8 ResStringLoadToMem(U16 ResId,const U8 *idx,U8 *ptr);
 //U8 *ResConstLoadToCon(U16 ResId,const U8 *idx,U8 *cbnk);
 U8 *ResLoadToConOld(U16 ResId,U8 idx,U8 *cbnk);
 U8 *ResLoadToConNew(U16 ResId,U8 idx,U8 *cbnk);

#endif	/* _RESOURCE_H */
