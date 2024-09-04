#ifndef	_ENG_HEAD_H
#define	_ENG_HEAD_H


#include "consdef.h"
#include "graph.h"
#include "fsys.h"
#include "errcode.h"
#include "attribute.h"
#include "order.h"
#include "paccount.h"
#include "fight.h"
#include "extern.h"
#include "datman.h"
#include "fundec.h"
#include "sconst.h"
#include "sharefun.h"
#include "gameview.h"

void GamBaYeEng(void);

U8 *itoa_my(int value,U8 *string,int radix);
U8 *ltoa_my(long value,U8 *string,int radix);
#endif	/* _ENG_HEAD_H */
