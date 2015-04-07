
#include "Annotation.h"

//------ AnnotationObject -------------------------------------------------------------------------

const static char  *ANNOT_TYPE_NAMES[] = 
{
	"Text",
	"Link",
	"Sound",
	"FreeText",
	"Stamp",
	"Square",
	"Circle",
	"StrikeOut",
	"Highlight",
	"Underline",
	"Ink",
	"FileAttachment",
	"Popup"
};

const static char  *ANNOT_ICON_NAMES_NAMES[] = 
{                          
	"Comment",
	"Key",
	"Note",
	"Help",
	"NewParagraph",
	"Paragraph",
	"Insert"
};

//------ Внутренние функции -----------------------------------------------------------------------
Annotation    AnnotationNew             (MMgr oMMgr, Xref pXref, AnnotType eType, TRect oRect)
{
    Annotation pAnnot = NULL;
    Array oArray;
    unsigned long nRet = OK;
    float fTemp = 0.0;

    pAnnot = DictNew( oMMgr );

    if ( !pAnnot )
        return NULL;

    if ( OK != XrefAdd( pXref, pAnnot ) )
        return NULL;

    oArray = ArrayNew( oMMgr );
    
	if ( !oArray )
        return NULL;

    if ( OK != DictAdd( pAnnot, "Rect", oArray) )
        return NULL;

	if ( oRect.fTop < oRect.fBottom ) 
	{
		fTemp = oRect.fTop;
		oRect.fTop = oRect.fBottom;
		oRect.fBottom = fTemp;
    }

	nRet += ArrayAddReal( oArray, oRect.fLeft);
	nRet += ArrayAddReal( oArray, oRect.fBottom);
	nRet += ArrayAddReal( oArray, oRect.fRight);
	nRet += ArrayAddReal( oArray, oRect.fTop);

    nRet += DictAddName( pAnnot, "Type", "Annot" );
    nRet += DictAddName( pAnnot, "Subtype", ANNOT_TYPE_NAMES[ (int)eType ] );

    if ( OK != nRet )
        return NULL;

	pAnnot->pHeader.nObjClass |= OSUBCLASS_ANNOTATION;

    return pAnnot;
}


Annotation    LinkAnnotNew              (MMgr oMMgr, Xref pXref, TRect oRect, Destination pDst)
{
    Annotation pAnnot = NULL;

    pAnnot = AnnotationNew( oMMgr, pXref, AnnotLink, oRect);

	if ( !pAnnot )
        return NULL;

    if ( OK != DictAdd( pAnnot, "Dest", pDst ) )
        return NULL;

    return pAnnot;
}


Annotation    URILinkAnnotNew           (MMgr oMMgr, Xref pXref, TRect oRect, const char *sUri)
{
    Annotation pAnnot = NULL;
    Dict pAction;
    unsigned long nRet = 0;

    pAnnot = AnnotationNew( oMMgr, pXref, AnnotLink, oRect);

	if ( !pAnnot )
        return NULL;

    pAction = DictNew( oMMgr );

	if ( !pAction )
        return NULL;

    nRet = DictAdd( pAnnot, "A", pAction );
    if ( OK != nRet )
        return NULL;

    nRet += DictAddName( pAction, "Type", "Action");
    nRet += DictAddName( pAction, "S", "URI");
    nRet += DictAdd( pAction, "URI", StringNew( oMMgr, sUri, NULL ) );

    if ( OK != nRet )
        return NULL;

    return pAnnot;
}

Annotation    TextAnnotNew              (MMgr oMMgr, Xref pXref, TRect oRect, const char *sText, EncoderRecPtr pEncoder)
{
    Annotation pAnnot = NULL;
    String oString;

    pAnnot = AnnotationNew( oMMgr, pXref, AnnotTextNotes, oRect );

	if ( !pAnnot )
        return NULL;

    oString = StringNew( oMMgr, sText, pEncoder);
    if ( !oString )
        return NULL;

    if ( OK != DictAdd( pAnnot, "Contents", oString ) )
        return NULL;

    return pAnnot;
}


BOOL          AnnotationValidate        (Annotation  pAnnot )
{
    if ( !pAnnot )
        return FALSE;

    if ( pAnnot->pHeader.nObjClass != (OSUBCLASS_ANNOTATION | OCLASS_DICT) )
        return FALSE;

    return TRUE;
}
static BOOL   CheckSubType              (Annotation  pAnnot, AnnotType eType )
{
    Name oSubtype;

    if ( !AnnotationValidate( pAnnot ) )
        return FALSE;

    oSubtype = (Name)DictGetItem( pAnnot, "Subtype", OCLASS_NAME );

	if ( !oSubtype || UtilsStrCmp( oSubtype->sValue, ANNOT_TYPE_NAMES[ (int)eType ] ) != 0 ) 
	{
		RaiseError( pAnnot->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION, 0 );
        return FALSE;
    }

    return TRUE;
}


//------ Внешние функции --------------------------------------------------------------------------
unsigned long AnnotationSetBorderStyle  (Annotation  pAnnot, BSSubtype eSubtype, float fWidth, unsigned short nDashOn, unsigned short nDashOff, unsigned short nDashPhase)
{
    Dict pBorderStyle = NULL;
    Array oDash;
    unsigned long nRet = OK;

    pBorderStyle = DictNew( pAnnot->oMMgr );
    if ( !pBorderStyle )
        return ErrorGetCode( pAnnot->oError );

    if ( OK != ( nRet = DictAdd( pAnnot, "BS", pBorderStyle ) ) )
        return nRet;

    if ( BSDashed  == eSubtype ) 
	{
		oDash = ArrayNew( pAnnot->oMMgr );
        if ( !oDash )
			return ErrorGetCode( pAnnot->oError );

        if ( OK != ( nRet = DictAdd( pBorderStyle, "D", oDash ) ) )
            return nRet;

        nRet += DictAddName( pBorderStyle, "Type", "Border" );
        nRet += ArrayAddReal( oDash, nDashOn );
        nRet += ArrayAddReal( oDash, nDashOff );

        if ( 0 != nDashPhase )
            nRet += ArrayAddReal( oDash, nDashOff );
    }

    switch ( eSubtype )  
	{
        case BSSolid:
            nRet += DictAddName( pBorderStyle, "S", "S");
            break;
        case BSDashed:
            nRet += DictAddName( pBorderStyle, "S", "D");
            break;
        case BSBeveled:
            nRet += DictAddName( pBorderStyle, "S", "B");
            break;
        case BSInset:
            nRet += DictAddName( pBorderStyle, "S", "I");
            break;
        case BSUnderlined:
            nRet += DictAddName( pBorderStyle, "S", "U");
            break;
        default:
			return  SetError( pAnnot->oError, AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_BORDER_STYLE, 0);
    }

    if ( BS_DEF_WIDTH != fWidth )
        nRet += DictAddReal( pBorderStyle, "W", fWidth );

    if ( OK != nRet )
		return ErrorGetCode( pAnnot->oError );

    return OK;
}


unsigned long LinkAnnotSetBorderStyle   (Annotation  pAnnot, float fWidth, unsigned short nDashOn, unsigned short nDashOff )
{
    Array oArray;
    unsigned long nRet = OK;

    if ( !CheckSubType( pAnnot, AnnotLink ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION;

    if ( fWidth < 0 )
		return RaiseError( pAnnot->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0 );

	oArray = ArrayNew( pAnnot->oMMgr );

	if ( !oArray )
		return CheckError( pAnnot->oError );

    if ( OK != ( nRet = DictAdd( pAnnot, "Border", oArray ) ) )
		return CheckError( pAnnot->oError );

    nRet += ArrayAddNumber( oArray, 0 );
    nRet += ArrayAddNumber( oArray, 0 );
    nRet += ArrayAddReal( oArray, fWidth );

    if ( OK != nRet )
		return CheckError( pAnnot->oError );

    if ( nDashOn && nDashOff ) 
	{
		Array oDash = ArrayNew( pAnnot->oMMgr );
        if ( !oDash )
			return CheckError( pAnnot->oError );

        if ( OK != ( nRet = ArrayAdd( oArray, oDash ) ) )
			return CheckError( pAnnot->oError );

        nRet += ArrayAddNumber( oDash, nDashOn );
        nRet += ArrayAddNumber( oDash, nDashOff );
    }

    if ( OK != nRet )
		return CheckError( pAnnot->oError );

    return nRet;
}

unsigned long LinkAnnotSetHighlightMode (Annotation  pAnnot, AnnotHighlightMode eMode)
{
    unsigned long nRet = OK;

    if ( !CheckSubType( pAnnot, AnnotLink ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION;

    switch ( eMode ) 
	{
        case AnnotNoHighlight:
            nRet = DictAddName( pAnnot, "H", "N");
            break;
        case AnnotInvertBorder:
            nRet = DictAddName( pAnnot, "H", "O");
            break;
        case AnnotDownAppearance:
            nRet = DictAddName( pAnnot, "H", "P");
            break;
        default:  /* AnnotInvertBox */
            /* default value */
            DictRemoveElement( pAnnot, "H");
            nRet = OK;
    }

    if ( OK != nRet )
		return CheckError( pAnnot->oError );

    return nRet;
}

unsigned long TextAnnotSetIcon          (Annotation  pAnnot, AnnotIcon eIcon)
{
    if ( !CheckSubType( pAnnot, AnnotTextNotes ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION;

    if ( eIcon < 0 || eIcon >= AnnotIconEOF )
		return RaiseError( pAnnot->oError, AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_ICON, (unsigned long)eIcon);

    if ( DictAddName( pAnnot, "Name", ANNOT_ICON_NAMES_NAMES[ (int)eIcon ] ) != OK )
		return CheckError( pAnnot->oError );

    return OK;
}

unsigned long TextAnnotSetOpened        (Annotation  pAnnot, BOOL bOpened)
{
    BooleanRecPtr oBool;

	if ( !CheckSubType( pAnnot, AnnotTextNotes ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION;

	oBool = BooleanNew( pAnnot->oMMgr, bOpened );
    if ( !oBool )
		return CheckError( pAnnot->oError );

    return DictAdd( pAnnot, "Open", oBool );
}
