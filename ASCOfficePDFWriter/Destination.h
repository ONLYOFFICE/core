#ifndef _DESTINATION_H
#define _DESTINATION_H

#include "Objects.h"
#include "Utils.h"

const char *c_asDestinationType[] = 
{
	"XYZ",
	"Fit",
	"FitH",
	"FitV",
	"FitR",
	"FitB",
	"FitBH",
	"FitBV",
	NULL
};

BOOL          PageValidate            (Page pPage);

//----- DestinationObject -----------------------------------------------------------------------

Destination   DestinationNew       (MMgr oMMgr, Page pTarget, Xref pXref)
{
    Destination pDst = NULL;

    if ( !PageValidate( pTarget) ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0 );
        return NULL;
    }

    pDst = ArrayNew( oMMgr );

	if ( !pDst )
        return NULL;

	pDst->pHeader.nObjClass |= OSUBCLASS_DESTINATION;

    if ( OK != XrefAdd( pXref, pDst ) )
        return NULL;

    // первый элемент массива должен быть страницей, которой принадлежит объект
    if ( OK != ArrayAdd( pDst, pTarget ) )
        return NULL;

    // default type = Fit 
    if ( OK != ArrayAddName( pDst, c_asDestinationType[ (int)Fit ] ) )
        return NULL;

    return pDst;
}

BOOL          DestinationValidate  (Destination pDst)
{
    ObjHeader *pHeader = (ObjHeader *)pDst;
    
	Page pTarget = NULL;

    if ( !pDst || pHeader->nObjClass != (OCLASS_ARRAY | OSUBCLASS_DESTINATION) )
        return FALSE;

    // тип Destination не определен.
	if ( pDst->pList->nCount < 2 )
        return FALSE;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

    if ( !PageValidate( pTarget ) )
		return SetError( pDst->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE, 0 );

    return TRUE;
}

//----- Внешние функции --------------------------------------------------------------------------
unsigned long DestinationSetXYZ    (Destination pDst, float fLeft, float fTop, float fZoom)
{
    unsigned long nRet = OK;
    Page pTarget = NULL;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

	// Если параметр приближения задан некорректно, тогда оставляем его нетронутым(что соответствует значению 0)
	if ( fZoom < 0.08 || fZoom > 32 )
		fZoom = 0;

    if ( fLeft < 0 || fTop < 0 )
		return RaiseError( pDst->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0 );

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)XYZ ] );
    nRet += ArrayAddReal( pDst, fLeft);
    nRet += ArrayAddReal( pDst, fTop);
    nRet += ArrayAddReal( pDst, fZoom);

    if ( OK != nRet )
		return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFit    (Destination pDst)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)Fit ] );

    if ( OK != nRet )
		return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitH   (Destination pDst, float fTop)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitH ] );
    nRet += ArrayAddReal( pDst, fTop );

    if ( OK != nRet )
        return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitV   (Destination pDst, float fLeft)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitV ] );
    nRet += ArrayAddReal( pDst, fLeft );

    if ( OK != nRet )
        return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitR   (Destination pDst, float fLeft, float fBottom, float fRight, float fTop)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitR ] );
    nRet += ArrayAddReal( pDst, fLeft);
    nRet += ArrayAddReal( pDst, fBottom);
    nRet += ArrayAddReal( pDst, fRight);
    nRet += ArrayAddReal( pDst, fTop);

    if ( OK != nRet )
		return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitB   (Destination pDst)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitB ] );

    if ( OK  != nRet )
		return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitBH  (Destination pDst, float fTop)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitBH ] );
    nRet += ArrayAddReal( pDst, fTop );

    if ( OK != nRet )
		return CheckError( pDst->oError );

    return OK;
}

unsigned long DestinationSetFitBV  (Destination pDst, float fLeft)
{
    unsigned long nRet = OK;
    
	Page pTarget;

    if ( !DestinationValidate( pDst ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION;

    pTarget = (Page)ArrayGetItem( pDst, 0, OCLASS_DICT );

	if ( pDst->pList->nCount > 1 ) 
	{
        ArrayClear( pDst );
        nRet += ArrayAdd( pDst, pTarget );
    }

    nRet += ArrayAddName( pDst, c_asDestinationType[ (int)FitBV ] );
    nRet += ArrayAddReal( pDst, fLeft );

    if ( OK != nRet )
		return CheckError( pDst->oError );

    return OK;

}


#endif /* _DESTINATION_H */

