.include sysAsm.var

;=================================
;汇编常量定义
SCR_WBYTE	.equ	20	;屏幕中每行的字节个数 159/8(图片为横向编码)
SCR_WIDTH	.equ	159	;屏幕大小
SCR_HEIGHT	.equ	96	;屏幕大小
;=================================
;汇编变量定义
gam_PicLine	.equ TEMP_PAGE0_START+0			;构造一行图片数据	20
gam_PicStartX	.equ TEMP_PAGE0_START+20		;图片显示x起始坐标	1
gam_PicStartY	.equ TEMP_PAGE0_START+21		;图片显示y起始坐标	1
gam_PicEndX	.equ TEMP_PAGE0_START+22		;图片显示x终止坐标	1
gam_PicEndY	.equ TEMP_PAGE0_START+23		;图片显示y终止坐标	1
gam_VSWidth	.equ TEMP_PAGE0_START+24		;贴图宽度尺寸		1
gam_VSHeight	.equ TEMP_PAGE0_START+25		;贴图高度尺寸		1
gam_PicWidth	.equ TEMP_PAGE0_START+24		;图片宽度尺寸		1
gam_PicHeight	.equ TEMP_PAGE0_START+25		;图片高度尺寸		1
gam_PicPtr	.equ TEMP_PAGE0_START+26		;图片数据指针		2
gam_VSPtr	.equ TEMP_PAGE0_START+28		;虚拟屏幕指针变量	2
gam_ShowFlag	.equ TEMP_PAGE0_START+30		;运算方式		2
gam_BitOff	.equ TEMP_PAGE0_START+31		;字节位偏移		1
gam_EndLineOff	.equ TEMP_PAGE0_START+32		;末字节剩余位		1
gam_MulTmp	.equ TEMP_PAGE0_START+33		;乘法临时变量		1
gam_Counter1	.equ TEMP_PAGE0_START+34		;临时变量		2
gam_Counter2	.equ TEMP_PAGE0_START+36		;临时变量		2
gam_LineEndByte	.equ TEMP_PAGE0_START+38		;行末字节		1
gam_PicTmp	.equ TEMP_PAGE0_START+39		;临时变量		1
gam_MaskTmp	.equ TEMP_PAGE0_START+40		;临时变量		1

;=================================
;全局变量引用
	.extern	__stack_ptr		;
	.extern	__addr_reg		;
	.extern __oper1			;
	.extern	__uns_oper1_char_to_long;
	.extern	__store_char_funct_arg	;
	.extern	__store_int_funct_arg	;
	.extern	__store_long_funct_arg	;
	.extern	__banked_function_call	;
	.extern __uns_oper1_int_to_long	;
