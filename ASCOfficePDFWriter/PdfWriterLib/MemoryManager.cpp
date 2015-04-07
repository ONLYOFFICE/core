#include "MemoryManager.h"

static void *InternalGetMem (unsigned int nSize )
{
    return malloc( nSize );
}

static void  InternalFreeMem(void*  pMem)
{
    free( pMem );
}



//MMgrNew

//сзодает новые MMgr объект. Если при выделении памяти возникают ошибки,
//возвращает NULL и создается функци ошибки(error handling function).
//Если nBufSize не нулевой, MMgr формуируется для выделения памяти

MMgr MMgrNew (Error oError, unsigned int nBufSize, AllocFunc  pAllocFn, FreeFunc pFreeFn)
{
    MMgr oMMgr;

    if ( pAllocFn )
        oMMgr = ( MMgr ) pAllocFn (sizeof( MMgrRec ));
    else
        oMMgr = ( MMgr )InternalGetMem (sizeof( MMgrRec ));

    if (oMMgr != NULL) 
	{
        // инициализируем oMMgr объект 
		oMMgr->oError = oError;

        // Если pAllocFn и pFreeFn определены, тогда используются они. 
		// А если нет, используются стандартные функции "malloc" и "free".
        
		if ( pAllocFn && pFreeFn ) 
		{
			oMMgr->pAllocFn = pAllocFn;
            oMMgr->pFreeFn  = pFreeFn;
        } 
		else 
		{
			oMMgr->pAllocFn = InternalGetMem;
            oMMgr->pFreeFn  = InternalFreeMem;
        }

		//Если nBufSize не нулевой, MMgr формурируется для выделения памяти        
		
		if ( !nBufSize)
			oMMgr->pMPool = NULL;
        else 
		{
            MPoolNode pNode;

			pNode = (MPoolNode)oMMgr->pAllocFn( sizeof(MPoolNodeRec) + nBufSize );

            if ( NULL == pNode ) 
			{
                SetError( oError, AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, NOERROR);
				oMMgr->pFreeFn( oMMgr );
                oMMgr = NULL;
            } 
			else 
			{
				oMMgr->pMPool    = pNode;
				pNode->pBuffer   = (BYTE *)pNode + sizeof( MPoolNodeRec );
				pNode->nSize     = nBufSize;
				pNode->nUsedSize = 0;
				pNode->pNextNode = NULL;
            }
        }

		oMMgr->nBufSize = nBufSize;
		oMMgr->pDoc = NULL;
    } 
	else
        SetError( oError, AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, NOERROR);

    return oMMgr;
}


void MMgrFree(MMgr  oMMgr )
{
    MPoolNode pNode;

    if ( oMMgr == NULL )
        return;

	pNode = oMMgr->pMPool;

    //  удаляем все вершины рекурсивно
    while ( NULL != pNode ) 
	{
        MPoolNode pTmp = pNode;
		pNode = pTmp->pNextNode;

		oMMgr->pFreeFn( pTmp );
    }
	oMMgr->pFreeFn( oMMgr );
}

void*GetMem  (MMgr  oMMgr, unsigned int nSize )
{
    void *ptr;

	if ( oMMgr->pMPool ) 
	{
        MPoolNode pNode = oMMgr->pMPool;

#ifdef ALINMENT_SIZ
        nSize = ( nSize + (ALINMENT_SIZ - 1) ) / ALINMENT_SIZ;
        nSize *= ALINMENT_SIZ;
#endif

		if ( pNode->nSize - pNode->nUsedSize >= nSize ) 
		{
			ptr = (BYTE*)pNode->pBuffer + pNode->nUsedSize;
			pNode->nUsedSize += nSize;
            return ptr;
        } 
		else 
		{
			unsigned int nTmpBufSize = ( oMMgr->nBufSize < nSize) ?  nSize : oMMgr->nBufSize;

			pNode = (MPoolNode)oMMgr->pAllocFn( sizeof(MPoolNodeRec) + nTmpBufSize );

            if ( !pNode ) 
			{
				SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, NOERROR);
                return NULL;
            }

			pNode->nSize = nTmpBufSize;
        }

		pNode->pNextNode = oMMgr->pMPool;
        oMMgr->pMPool = pNode;
		pNode->nUsedSize = nSize;
		pNode->pBuffer = (BYTE*)pNode + sizeof(MPoolNodeRec);
		ptr = pNode->pBuffer;
    } 
	else 
	{
		ptr = oMMgr->pAllocFn( nSize );

        if ( NULL == ptr )
			SetError(oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, NOERROR);
    }

    return ptr;
}

void FreeMem (MMgr  oMMgr, void *aptr)
{
    if ( !aptr )
        return;

	if ( !oMMgr->pMPool ) 
	{
		oMMgr->pFreeFn( aptr );
    }
    return;
}
