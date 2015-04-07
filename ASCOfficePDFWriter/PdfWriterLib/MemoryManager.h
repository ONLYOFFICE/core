#ifndef _MMGR_H
#define _MMGR_H

#pragma once 

#include "Errors.h"
#include "Utils.h"

typedef struct  TDocRec *Doc;
typedef struct  TMPoolNodeRec  *MPoolNode;

typedef struct  TMPoolNodeRec 
{
    BYTE*        pBuffer;
    unsigned int nSize;
    unsigned int nUsedSize;
    MPoolNode    pNextNode;
} MPoolNodeRec;


typedef struct  TMMgrRec  *MMgr;

typedef struct  TMMgrRec 
{
    Error        oError;
    AllocFunc    pAllocFn;
    FreeFunc     pFreeFn;
    MPoolNode    pMPool;
    unsigned int nBufSize;

	Doc          pDoc;
} MMgrRec;

//static void *InternalGetMem (unsigned int nSize )
//{
//    return malloc( nSize );
//}
//
//static void  InternalFreeMem(void*  pMem)
//{
//    free( pMem );
//}
//
//

  //MMgrNew

  //сзодает новые MMgr объект. Если при выделении памяти возникают ошибки,
  //возвращает NULL и создается функци ошибки(error handling function).
  //Если nBufSize не нулевой, MMgr формуируется для выделения памяти

MMgr MMgrNew (Error oError, unsigned int nBufSize, AllocFunc  pAllocFn, FreeFunc pFreeFn);
void MMgrFree(MMgr  oMMgr );
void*GetMem  (MMgr  oMMgr, unsigned int nSize );
void FreeMem (MMgr  oMMgr, void *aptr);
#endif /* _MMGR_H */