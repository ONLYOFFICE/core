#ifndef _OUTLINE_H
#define _OUTLINE_H

#include "Objects.h"
#include "Utils.h"
#include "Destination.h"

#define OUTLINE_CLOSED     0
#define OUTLINE_OPENED     1


static unsigned long AddChild    (Outline pParent, Outline pItem);
static unsigned int  CountChild  (Outline pOutline);
static unsigned long BeforeWrite (Dict pObj);

//----- OutlineObject ---------------------------------------------------------

Outline OutlineRootNew  (MMgr oMMgr, Xref pXref)
{
    Outline pOutline = NULL;
    unsigned long nRet = OK;
    Number oOpenFlag = NULL;

    pOutline = DictNew( oMMgr );
    if ( !pOutline )
        return NULL;

	pOutline->pBeforeWriteFn = BeforeWrite;

    if ( OK != XrefAdd( pXref, pOutline ) )
        return NULL;

    oOpenFlag = NumberNew( oMMgr, OUTLINE_OPENED);
    if ( !oOpenFlag )
        return NULL;

	oOpenFlag->pHeader.nObjId |= OTYPE_HIDDEN;

    nRet += DictAdd( pOutline, "_OPENED", oOpenFlag);
    nRet += DictAddName( pOutline, "Type", "Outline" );

    if ( OK != nRet )
        return NULL;

	pOutline->pHeader.nObjClass |= OSUBCLASS_OUTLINE;

    return pOutline;
}



Outline OutlineNew      (MMgr oMMgr, Outline pParent, const char *sTitle, Encoder pEncoder, Xref pXref)
{
    Outline pOutline = NULL;
    String oString = NULL;
    unsigned long nRet = OK;
    Number oOpenFlag = NULL;

    if ( !oMMgr || !pParent || !pXref )
        return NULL;

    pOutline = DictNew( oMMgr );
    if ( !pOutline )
        return NULL;

	pOutline->pBeforeWriteFn = BeforeWrite;

    if ( OK != XrefAdd( pXref, pOutline ) )
        return NULL;

    oString = StringNew( oMMgr, sTitle, pEncoder);
    if ( !oString )
        return NULL;
    else
        nRet += DictAdd( pOutline, "Title", oString );

    oOpenFlag = NumberNew( oMMgr, OUTLINE_OPENED );
    if ( !oOpenFlag )
        return NULL;

	oOpenFlag->pHeader.nObjId |= OTYPE_HIDDEN;
    nRet += DictAdd( pOutline, "_OPENED", oOpenFlag);
    nRet += DictAddName( pOutline, "Type", "Outline");
    nRet += AddChild( pParent, pOutline);

    if ( OK != nRet )
        return NULL;

	pOutline->pHeader.nObjClass |= OSUBCLASS_OUTLINE;

    return pOutline;
}

Outline OutlineGetFirst (Outline pOutline)
{
    return (Outline)DictGetItem( pOutline, "First", OCLASS_DICT);
}




Outline OutlineGetLast  (Outline pOutline)
{
    return (Outline)DictGetItem( pOutline, "Last", OCLASS_DICT);
}

Outline OutlineGetPrev  (Outline pOutline)
{
    return (Outline)DictGetItem( pOutline, "Prev", OCLASS_DICT);
}


Outline OutlineGetNext  (Outline pOutline)
{
    return (Outline)DictGetItem( pOutline, "Next", OCLASS_DICT );
}


Outline OutlineGetParent(Outline pOutline)
{
    return (Outline)DictGetItem( pOutline, "Parent", OCLASS_DICT);
}
BOOL    OutlineGetOpened(Outline pOutline)
{
    Number oNumber = (Number)DictGetItem( pOutline, "_OPENED", OCLASS_NUMBER);

    if ( !oNumber )
        return FALSE;

	return (BOOL)oNumber->nValue;
}
BOOL    OutlineValidate (Outline pOutline)
{
    if ( !pOutline )
        return FALSE;

    if ( pOutline->pHeader.nObjClass != (OSUBCLASS_OUTLINE | OCLASS_DICT) )
        return FALSE;

    return TRUE;
}



//----------------------------------------------------------------------------
unsigned long OutlineSetDestination(Outline pOutline, Destination pDst)
{
    if ( !OutlineValidate( pOutline ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OUTLINE;

    if ( !DestinationValidate( pDst ) )
		return RaiseError( pOutline->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION, 0);

    if ( NULL == pDst )
        return DictRemoveElement( pOutline, "Dest" );

    if ( OK != DictAdd( pOutline, "Dest", pDst ) )
		return CheckError( pOutline->oError );

    return OK;
}


unsigned long OutlineSetOpened     (Outline pOutline, BOOL bOpened)
{
    Number oNumber = NULL;

    if ( !OutlineValidate( pOutline ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OUTLINE;

    oNumber = (Number)DictGetItem( pOutline, "_OPENED", OCLASS_NUMBER);

    if ( !oNumber ) 
	{
		oNumber = NumberNew( pOutline->oMMgr, (int)bOpened );
        if ( !oNumber || DictAdd( pOutline, "_OPENED", oNumber ) != OK )
			return CheckError( pOutline->oError );
    } 
	else
		oNumber->nValue = (int)bOpened;

    return OK;
}
//----------------------------------------------------------------------------
static unsigned long AddChild    (Outline pParent, Outline pItem)
{
    Outline pFirst = (Outline)DictGetItem( pParent, "First", OCLASS_DICT);
    Outline pLast  = (Outline)DictGetItem( pParent, "Last",  OCLASS_DICT);

	unsigned long nRet = OK;

    if ( !pFirst )
        nRet += DictAdd( pParent, "First", pItem);

    if ( pLast) 
	{
        nRet += DictAdd( pLast, "Next", pItem);
        nRet += DictAdd( pItem, "Prev", pLast);
    }

    nRet += DictAdd( pParent, "Last", pItem);
    nRet += DictAdd( pItem, "Parent", pParent);

    if ( OK != nRet )
		return ErrorGetCode( pItem->oError );

    return OK;
}


static unsigned int  CountChild  (Outline pOutline)
{
    Outline pChild = OutlineGetFirst( pOutline );
    unsigned int nCount = 0;

    while ( pChild ) 
	{
        nCount++;

        if ( OutlineGetOpened( pChild ) )
            nCount += CountChild( pChild );

        pChild = OutlineGetNext( pChild );
    }

    return nCount;
}


static unsigned long BeforeWrite (Dict pObj)
{
    Number oNumber = (Number)DictGetItem( pObj, "Count", OCLASS_NUMBER);
    unsigned int nCount = CountChild( (Outline)pObj );

    if ( 0 == nCount && oNumber )
        return DictRemoveElement( pObj, "Count");

    if ( !OutlineGetOpened( (Outline)pObj ) )
        nCount *= -1;

    if ( oNumber )
		oNumber->nValue = nCount;
    else
        if ( nCount )
            return DictAddNumber( pObj, "Count", nCount );

    return OK;
}





#endif /* _OUTLINE_H */

