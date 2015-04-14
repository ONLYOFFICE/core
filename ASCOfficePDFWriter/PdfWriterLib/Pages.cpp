#include "Pages.h"
//--- Определения функий из PageOperations.h------
unsigned long PageEndPath (Page pPage);
unsigned long PageEndText (Page pPage);
unsigned long PageGRestore(Page pPage);
//------------------------------------------------

static unsigned int  GetPageCount    (Dict pPages)
{
    unsigned int nCount = 0;
    Array oKids = (Array)DictGetItem( pPages, "Kids", OCLASS_ARRAY);

    if ( !oKids )
        return 0;

	for (unsigned int nIndex = 0; nIndex < oKids->pList->nCount; nIndex++) 
	{
        void *pObj = ArrayGetItem( oKids, nIndex, OCLASS_DICT);
        ObjHeader *pHeader = (ObjHeader *)pObj;

		if ( pHeader->nObjClass == (OCLASS_DICT | OSUBCLASS_PAGES) )
            nCount += GetPageCount( (Dict)pObj );
		else if ( pHeader->nObjClass == (OCLASS_DICT | OSUBCLASS_PAGE) )
            nCount += 1;
    }

    return nCount;
}


static unsigned long PagesBeforeWrite(Dict pObj)
{
    Array oKids = (Array )DictGetItem( pObj, "Kids", OCLASS_ARRAY );
    Number oCount = (Number)DictGetItem( pObj, "Count", OCLASS_NUMBER);

	unsigned long nRet = OK;

    if ( !oKids )
		return SetError( pObj->oError, AVS_OFFICEPDFWRITER_ERROR_PAGES_MISSING_KIDS_ENTRY, 0 );

    if ( oCount )
		oCount->nValue = GetPageCount( pObj );
    else 
	{
		oCount = NumberNew( pObj->oMMgr, GetPageCount( pObj ) );
        if ( !oCount )
			return ErrorGetCode( pObj->oError );

        if ( OK != ( nRet = DictAdd( pObj, "Count", oCount ) ) )
            return nRet;
    }

    return OK;
}

static unsigned long PageBeforeWrite (Dict pObj)
{
    unsigned long nRet = OK;
    Page pPage = (Page)pObj;
	PageAttr pAttr = (PageAttr)pObj->pAttr;

	if ( GMODE_PATH_OBJECT == pAttr->nGMode ) 
	{
        if ( OK != ( nRet = PageEndPath( pPage ) ) )
           return nRet;
    }

	if ( GMODE_TEXT_OBJECT == pAttr->nGMode ) 
	{
        if ( OK != ( nRet = PageEndText( pPage ) ) )
            return nRet;
    }

	if ( pAttr->pGState )
		while ( pAttr->pGState->pPrev ) 
		{
            if ( OK != ( nRet = PageGRestore( pPage ) ) )
                return nRet;
        }

    return OK;
}

static void          PageOnFree      (Dict pObj)
{
    PageAttr pAttr = (PageAttr)pObj->pAttr;

    if (pAttr) 
	{
		if (pAttr->pGState)
			GStateFree( pObj->oMMgr, pAttr->pGState);

		FreeMem( pObj->oMMgr, pAttr);
    }
}
static unsigned long AddResource     (Page pPage)
{
    unsigned long nRet = OK;
	Dict pResource = DictNew( pPage->oMMgr );
    Array oProcset;

    if ( !pResource )
		return ErrorGetCode( pPage->oError );

    // Не смотря на то, что ProcSet - устаревший объект, добавляем
    // его для совместимости

    nRet += DictAdd( pPage, "Resources", pResource );

	oProcset = ArrayNew( pPage->oMMgr );

	if ( !oProcset )
		return ErrorGetCode( pPage->oError );

    nRet += DictAdd( pResource, "ProcSet", oProcset );
	nRet += ArrayAdd( oProcset, NameNew( pPage->oMMgr, "PDF" ) );
    nRet += ArrayAdd( oProcset, NameNew( pPage->oMMgr, "Text" ) );
    nRet += ArrayAdd( oProcset, NameNew( pPage->oMMgr, "ImageB" ) );
    nRet += ArrayAdd( oProcset, NameNew( pPage->oMMgr, "ImageC" ) );
    nRet += ArrayAdd( oProcset, NameNew( pPage->oMMgr, "ImageI" ) );

    return nRet;
}


static unsigned long AddAnnotation   (Page pPage, Annotation pAnnot)
{
    Array oArray;
    unsigned long nRet = OK;

	// ищем вхождение "Annots"
    oArray = (Array)DictGetItem( pPage, "Annots", OCLASS_ARRAY );

    if ( !oArray ) 
	{
		oArray = ArrayNew( pPage->oMMgr );
        if ( !oArray )
			return ErrorGetCode( pPage->oError );

        nRet = DictAdd( pPage, "Annots", oArray );
        if ( OK != nRet )
            return nRet;
    }

    return ArrayAdd( oArray, pAnnot );
}


//----- PagesObject -----------------------------------------------------------
unsigned long PagesAddKids     (Pages pParent, Dict pKid)
{
    Array oKids;
    unsigned long nRet = OK;

    if ( DictGetItem( pKid, "Parent", OCLASS_DICT ) )
		return SetError( pParent->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_SET_PARENT, 0);

    if ( OK != ( nRet = DictAdd( pKid, "Parent", pParent ) ) )
        return nRet;

    oKids = (Array)DictGetItem( pParent, "Kids", OCLASS_ARRAY );
    if ( !oKids )
		return SetError( pParent->oError, AVS_OFFICEPDFWRITER_ERROR_PAGES_MISSING_KIDS_ENTRY, 0 );

	if ( pKid->pHeader.nObjClass == (OCLASS_DICT | OSUBCLASS_PAGE) ) 
	{
		PageAttr pAttr = (PageAttr)pKid->pAttr;

		pAttr->pParent = pParent;
    }

    return ArrayAdd( oKids, pKid );
}


Pages         PagesNew         (MMgr  oMMgr, Pages pParent, Xref pXref)
{
    unsigned long nRet = OK;
    Pages pPages = (Pages)DictNew( oMMgr );
    if ( !pPages )
        return NULL;

	pPages->pHeader.nObjClass |= OSUBCLASS_PAGES;
	pPages->pBeforeWriteFn = PagesBeforeWrite;

    if ( OK != XrefAdd( pXref, pPages ) )
        return NULL;

    // добавляем необходимые элементы 
    nRet += DictAddName( pPages, "Type", "Pages" );
	nRet += DictAdd( pPages, "Kids", ArrayNew( pPages->oMMgr ) );
	nRet += DictAdd( pPages, "Count", NumberNew( pPages->oMMgr, 0 ) );

    if ( OK == nRet && pParent )
        nRet += PagesAddKids( pParent, pPages );

    if ( OK != nRet )
        return NULL;

    return pPages;
}


bool          PagesValidate    (Pages pPages)
{
    ObjHeader *pHeader = (ObjHeader *)pPages;

	if ( !pPages || pHeader->nObjClass != (OCLASS_DICT | OSUBCLASS_PAGES) )
        return false;

    return true;
}

unsigned long PageInsertBefore (Page  pPage, Page pTarget)
{
    Page pParent = NULL;
    Array oKids = NULL;
    unsigned long nRet = OK;
	PageAttr pAttr = (PageAttr )pTarget->pAttr;
	pParent = pAttr->pParent;

    if ( !pParent )
		return SetError( pParent->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_SET_PARENT, 0 );

    if ( DictGetItem( pPage, "Parent", OCLASS_DICT ) )
		return SetError( pParent->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_SET_PARENT, 0 );

    if ( OK != ( nRet = DictAdd( pPage, "Parent", pParent ) ) )
        return nRet;

    oKids = (Array )DictGetItem( pParent, "Kids", OCLASS_ARRAY );

	if ( !oKids )
		return SetError( pParent->oError, AVS_OFFICEPDFWRITER_ERROR_PAGES_MISSING_KIDS_ENTRY, 0 );

	pAttr = (PageAttr)pPage->pAttr;
	pAttr->pParent = pParent;

    return ArrayInsert( oKids, pTarget, pPage);
}

//----- PageObject ------------------------------------------------------------
bool          PageValidate            (Page pPage)
{
    ObjHeader *pHeader = (ObjHeader *)pPage;

	if ( !pPage || !pPage->pAttr)
        return false;

	if ( pHeader->nObjClass != (OCLASS_DICT | OSUBCLASS_PAGE) )
        return false;

    return true;
}

Page          PageNew                 (MMgr oMMgr, Xref pXref)
{
    unsigned long nRet = OK;
    PageAttr pAttr = NULL;
    Page pPage = NULL;

    pPage = DictNew( oMMgr );
    if ( !pPage )
        return NULL;

	pPage->pHeader.nObjClass |= OSUBCLASS_PAGE;
	pPage->pFreeFn = PageOnFree;
    pPage->pBeforeWriteFn = PageBeforeWrite;

	pAttr = (PageAttr)GetMem( pPage->oMMgr, sizeof(PageAttrRec) );

	if ( !pAttr ) 
	{
        DictFree( pPage );
        return NULL;
    }

	pPage->pAttr = pAttr;
    UtilsMemSet( pAttr, 0, sizeof(PageAttrRec) );
	pAttr->nGMode   = GMODE_PAGE_DESCRIPTION;
	pAttr->oCurPos  = ToPoint( 0, 0);
	pAttr->oTextPos = ToPoint( 0, 0);

    nRet = XrefAdd( pXref, pPage);
    if ( OK != nRet )
        return NULL;

	pAttr->pGState   = GStateNew( pPage->oMMgr, NULL );
	pAttr->pContents = DictStreamNew( pPage->oMMgr, pXref );

	if ( !pAttr->pGState || !pAttr->pContents )
        return NULL;

	pAttr->pStream = pAttr->pContents->pStream;
	pAttr->pXref = pXref;

    // добавляем необходимые элементы 
    nRet += DictAddName( pPage, "Type", "Page");
	nRet += DictAdd( pPage, "MediaBox", BoxArrayNew( pPage->oMMgr, ToBox( 0, 0, DEF_PAGE_WIDTH, DEF_PAGE_HEIGHT ) ) );
	nRet += DictAdd( pPage, "Contents", pAttr->pContents );

    nRet += AddResource ( pPage );

    if ( OK != nRet )
        return NULL;

    return pPage;
}


void*         PageGetInheritableItem  (Page pPage, const char *sKey, unsigned short nObjClass)
{
    bool bCheck = false;
    int nIndex = 0;
    void * pObj = NULL;

    // проверяем поле sKey
    while ( c_asInheritableEntries[nIndex] ) 
	{
        if ( UtilsStrCmp( sKey, c_asInheritableEntries[nIndex] ) == 0 ) 
		{
            bCheck = true;
            break;
        }
        nIndex++;
    }

    // значение sKey не допустимо
    if ( true != bCheck )
	{
		SetError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);
        return NULL;
    }

    pObj = DictGetItem( pPage, sKey, nObjClass );

    // Если объект Resources нулевой, тогда ищем Resources у родительского объекта рекурсивно
    if ( !pObj ) 
	{
        Pages pPages = (Pages)DictGetItem( pPage, "Parent", OCLASS_DICT);
        while ( pPages ) 
		{
            pObj = DictGetItem( pPage, sKey, nObjClass);

            if ( pObj )
                break;

            pPages = (Pages)DictGetItem( pPages, "Parent", OCLASS_DICT);
        }
    }

    return pObj;
}


const char*   PageGetXObjectName      (Page pPage, XObject pXobj)
{
	PageAttr pAttr = (PageAttr)pPage->pAttr;
    const char *sKey;

	if ( !pAttr->pXObjects ) 
	{
        Dict pResources = (Dict)PageGetInheritableItem( pPage, "Resources", OCLASS_DICT);
        Dict pXobjects = NULL;

        if ( !pResources )
            return NULL;

		pXobjects = DictNew( pPage->oMMgr );
        if ( !pXobjects )
            return NULL;

        if ( OK != DictAdd( pResources, "XObject", pXobjects) )
            return NULL;

		pAttr->pXObjects = pXobjects;
    }

    // ищем объект xobject из xobject-resource
	sKey = DictGetKeyByObj( pAttr->pXObjects, pXobj );
    if ( !sKey) 
	{
        // Если xobject не зарегистрирован в xobject-resource, регистрируем
        // xobject в xobject-resource.

		char sXObjName[LIMIT_MAX_NAME_LEN + 1];
        char *pPointer;
        char *pEndPointer = sXObjName + LIMIT_MAX_NAME_LEN;

        pPointer = (char*)UtilsStrCpy( sXObjName, "X", pEndPointer);
		UtilsIToA( pPointer, pAttr->pXObjects->pList->nCount + 1, pEndPointer);

		if ( OK != DictAdd( pAttr->pXObjects, sXObjName, pXobj ) )
            return NULL;

		sKey = DictGetKeyByObj( pAttr->pXObjects, pXobj );
    }

    return sKey;
}


const char*   PageGetLocalFontName    (Page pPage, FontDict pFont)
{
    PageAttr pAttr = (PageAttr)pPage->pAttr;
    const char *sKey = NULL;

	// проверяем существует ли объект font-resources, если нет, то создаем его

    if ( !pAttr->pFonts ) 
	{
        Dict pResources = (Dict)PageGetInheritableItem( pPage, "Resources", OCLASS_DICT );

		if ( !pResources )
            return NULL;

		Dict pFonts = DictNew( pPage->oMMgr );
        if ( !pFonts )
            return NULL;

        if ( OK != DictAdd( pResources, "Font", pFonts ) )
            return NULL;

        pAttr->pFonts = pFonts;
    }

    // ищем объект font в font-resource
	sKey = DictGetKeyByObj( pAttr->pFonts, pFont );
    if ( !sKey ) 
	{
        // если фонт не зарегистрирован в ресурсах, тогда регистрируем его

        char sFontName[LIMIT_MAX_NAME_LEN + 1];
        char *pPointer = NULL;
        char *pEndPointer = sFontName + LIMIT_MAX_NAME_LEN;

        pPointer = (char*)UtilsStrCpy( sFontName, "F", pEndPointer);
		UtilsIToA( pPointer, pAttr->pFonts->pList->nCount + 1, pEndPointer);

		if ( OK != DictAdd( pAttr->pFonts, sFontName, pFont ) )
            return NULL;

		sKey = DictGetKeyByObj( pAttr->pFonts, pFont );
    }

    return sKey;
}
const char*   PageGetExtGStateName    (Page pPage, ExtGState pState)
{
	PageAttr pAttr = (PageAttr)pPage->pAttr;
    const char *sKey;

	if ( !pAttr->pExtGStates ) 
	{
        Dict pResources = NULL;
        Dict pExtGStates = NULL;

        pResources = (Dict)PageGetInheritableItem( pPage, "Resources", OCLASS_DICT);
        if ( !pResources )
            return NULL;

		pExtGStates = DictNew( pPage->oMMgr );
        if ( !pExtGStates )
            return NULL;

        if ( OK != DictAdd( pResources, "ExtGState", pExtGStates ) )
            return NULL;

		pAttr->pExtGStates = pExtGStates;
    }

    // ищем объект ext_gstate в ext_gstate-resource
	sKey = DictGetKeyByObj( pAttr->pExtGStates, pState);
    
	if ( !sKey ) 
	{
        // Если ext_gstate не зарегистрирован в ext_gstate-resource, регистрируем
        // ext_gstate в ext_gstate-resource.

		char sExtGStateName[LIMIT_MAX_NAME_LEN + 1];
        char *pPointer;
        char *pEndPointer = sExtGStateName + LIMIT_MAX_NAME_LEN;

        pPointer = (char*)UtilsStrCpy( sExtGStateName, "E", pEndPointer);
		UtilsIToA( pPointer, pAttr->pExtGStates->pList->nCount + 1, pEndPointer);

		if ( OK != DictAdd( pAttr->pExtGStates, sExtGStateName, pState) )
            return NULL;

		sKey = DictGetKeyByObj( pAttr->pExtGStates, pState);
    }

    return sKey;
}




Box           PageGetMediaBox         (Page pPage)
{
    Box oMediaBox = {0, 0, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        Array oArray = (Array)PageGetInheritableItem( pPage, "MediaBox", OCLASS_ARRAY);

        if ( oArray ) 
		{
            Real oTemp;

            oTemp = (Real)ArrayGetItem( oArray, 0, OCLASS_REAL );
            if ( oTemp )
				oMediaBox.fLeft = oTemp->fValue;

            oTemp = (Real)ArrayGetItem( oArray, 1, OCLASS_REAL );
            if ( oTemp )
				oMediaBox.fBottom = oTemp->fValue;

            oTemp = (Real)ArrayGetItem( oArray, 2, OCLASS_REAL );
            if ( oTemp )
				oMediaBox.fRight = oTemp->fValue;

            oTemp = (Real)ArrayGetItem( oArray, 3, OCLASS_REAL );
            if ( oTemp )
				oMediaBox.fTop = oTemp->fValue;

			CheckError( pPage->oError );
        } 
		else 
			RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_FIND_OBJECT, 0 );
    }

    return oMediaBox;
}



unsigned long PageSetBoxValue         (Page pPage, const char *sName, unsigned int nIndex, float fValue)
{
    Real oReal;
    Array oArray;

    if ( !PageValidate( pPage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

    oArray = (Array)PageGetInheritableItem( pPage, sName, OCLASS_ARRAY);
    if ( !oArray )
		return SetError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_FIND_OBJECT, 0);

    oReal = (Real)ArrayGetItem( oArray, nIndex, OCLASS_REAL);
    if ( !oReal )
		return SetError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_INDEX, 0);

	oReal->fValue = fValue;

    return OK;
}




void          PageSetFilter           (Page pPage, unsigned int nFilter)
{
	PageAttr pAttr = (PageAttr)pPage->pAttr;
	pAttr->pContents->nFilter = nFilter;
}



unsigned long PageCheckState          (Page pPage, unsigned int nMode)
{
    if ( !pPage )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;

    if ( pPage->pHeader.nObjClass != (OSUBCLASS_PAGE | OCLASS_DICT) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

	if ( !( ((PageAttr)pPage->pAttr)->nGMode & nMode ) )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_GMODE, 0 );

    return OK;
}

//------ Внешние функции ------------------------------------------------------
float             PageTextWidth            (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
{
    //unsigned int nLen = UtilsStrLen( sText, LIMIT_MAX_STRING_LEN + 1 );

	if ( !PageValidate( pPage ) || 0 == nLen )
        return 0;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    TextWidth oTextWidth;
    float fRes = 0;


  	if ( !pAttr->pGState->pFont ) 
	{
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0);
        return 0;
    }

	oTextWidth = FontTextWidth( pAttr->pGState->pFont, (BYTE*)sText, nLen, pCIDs, nLenCID);

	fRes += pAttr->pGState->fWordSpace * oTextWidth.nNumspace;
	fRes += oTextWidth.nWidth * pAttr->pGState->fFontSize  / 1000;
	fRes += pAttr->pGState->fCharSpace * oTextWidth.nNumchars;

	CheckError( pPage->oError );

    return fRes;
}
unsigned int      PageMeasureText          (Page pPage, const char *sText, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, bool bWordWrap, float *fRealWidth)
{
    unsigned int nLen = UtilsStrLen( sText, LIMIT_MAX_STRING_LEN + 1 );

	if ( !PageValidate( pPage ) || 0 == nLen )
        return 0;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	unsigned int nRet = 0;

	if ( !pAttr->pGState->pFont ) 
	{
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND, 0);
        return 0;
    }

	nRet = FontMeasureText( pAttr->pGState->pFont, (BYTE*)sText, nLen, pCIDs, nLenCID, fWidth, pAttr->pGState->fFontSize, pAttr->pGState->fCharSpace, pAttr->pGState->fWordSpace, bWordWrap, fRealWidth);

	CheckError( pPage->oError );

    return nRet;
}
float             PageGetWidth             (Page pPage)
{
	return PageGetMediaBox( pPage ).fRight;
}
float             PageGetHeight            (Page pPage)
{
	return PageGetMediaBox( pPage ).fTop;
}

FontDict          PageGetCurrentFont       (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->pFont;
    } 
	else
		return NULL;
}
float             PageGetCurrentFontSize   (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
        return ( pAttr->pGState->pFont ) ? pAttr->pGState->fFontSize : 0;
    } 
	else
        return 0;
}
TransMatrix       PageGetTransMatrix       (Page pPage)
{
    TransMatrix oDefaultMatrix = {1, 0, 0, 1, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->oTransMatrix;
    } 
	else
        return oDefaultMatrix;
}
float             PageGetLineWidth         (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
        return pAttr->pGState->fLineWidth;
    } 
	else
        return DEF_LINEWIDTH;
}

LineCapStyle      PageGetLineCap           (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
        return pAttr->pGState->eLineCap;
    } 
	else
        return DEF_LINECAP;
}
LineJoinStyle     PageGetLineJoin          (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
		PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->eLineJoin;
    } 
	else
        return DEF_LINEJOIN;
}
float             PageGetMiterLimit        (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
		PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fMiterLimit;
    } 
	else
        return DEF_MITERLIMIT;
}
DashMode          PageGetDash              (Page pPage)
{
    DashMode oMode = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
		PageAttr pAttr = (PageAttr)pPage->pAttr;
		oMode = pAttr->pGState->oDashMode;
    }

    return oMode;
}


float             PageGetFlat              (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
		PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fFlatness;
    } 
	else
        return DEF_FLATNESS;
}


float             PageGetWordSpace         (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
		PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fWordSpace;
    } 
	else
        return DEF_WORDSPACE;
}


float             PageGetCharSpace         (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fCharSpace;
    } 
	else
        return DEF_CHARSPACE;
}
float             PageGetHorizontalScalling(Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fHScalling;
    }
	else
        return DEF_HSCALING;
}
float             PageGetTextLeading       (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fTextLeading;
    } 
	else
        return DEF_LEADING;
}
TextRenderingMode PageGetTextRenderingMode (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->eRenderingMode;
    } 
	else
        return DEF_RENDERING_MODE;
}


float             PageGetTextRise          (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		return pAttr->pGState->fTextRise;
    } 
	else
        return DEF_RISE;
}
RGBColor          PageGetRGBFill           (Page pPage)
{
    RGBColor oDefaultRGBColor = {0, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

		if ( CSDeviceRGB == pAttr->pGState->eCSFill )
			return pAttr->pGState->oRGBFill;
    }

    return oDefaultRGBColor;
}
RGBColor          PageGetRGBStroke         (Page pPage)
{
    RGBColor oDefaultRGBColor = {0, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

        if ( CSDeviceRGB == pAttr->pGState->eCSStroke )
			return pAttr->pGState->oRGBStroke;
    }

    return oDefaultRGBColor;
}

CMYKColor         PageGetCMYKFill          (Page pPage)
{
    CMYKColor oDefaultCMYKColor = {0, 0, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

        if ( CSDeviceCMYK == pAttr->pGState->eCSFill )
            return pAttr->pGState->oCMYKFill;
    }

    return oDefaultCMYKColor;
}


CMYKColor         PageGetCMYKStroke        (Page pPage)
{
    CMYKColor oDefaultCMYKColor = {0, 0, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

		if ( CSDeviceCMYK == pAttr->pGState->eCSStroke )
			return pAttr->pGState->oCMYKStroke;
    }

    return oDefaultCMYKColor;
}


float             PageGetGrayFill          (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

        if ( CSDeviceGray == pAttr->pGState->eCSFill )
            return pAttr->pGState->fGrayFill;
    }

    return 0;
}
float             PageGetGrayStroke        (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

		if ( CSDeviceGray == pAttr->pGState->eCSStroke )
			return pAttr->pGState->fGrayStroke;
    }

    return 0;
}


ColorSpace        PageGetStrokingColorSpace(Page pPage)
{
    if ( PageValidate( pPage ) )
		return ( (PageAttr)pPage->pAttr)->pGState->eCSStroke;

    return CSEOF;
}


ColorSpace        PageGetFillingColorSpace (Page pPage)
{
    if ( PageValidate( pPage ) )
		return ( (PageAttr)pPage->pAttr)->pGState->eCSFill;

    return CSEOF;
}


TransMatrix       PageGetTextMatrix        (Page pPage)
{
    TransMatrix oDefaultMatrix = {1, 0, 0, 1, 0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
        return pAttr->oTextMatrix;
    } 
	else
        return oDefaultMatrix;
}


unsigned int      PageGetGStateDepth       (Page pPage)
{
    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
        return pAttr->pGState->nDepth;
    } 
	else
        return 0;
}
unsigned short    PageGetGMode             (Page pPage)
{
    if ( PageValidate( pPage ) )
        return ( (PageAttr)pPage->pAttr)->nGMode;

    return 0;
}

TPoint            PageGetCurrentPos        (Page pPage)
{
    TPoint oPos = {0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;
		if ( pAttr->nGMode & GMODE_PATH_OBJECT )
			oPos = pAttr->oCurPos;
    }

    return oPos;
}
unsigned long     PageGetCurrentPos2       (Page pPage, TPoint *poPos)
{
    if ( !PageValidate( pPage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;
    
	PageAttr pAttr = (PageAttr)pPage->pAttr;

    poPos->fX = 0;
    poPos->fY = 0;

    if ( pAttr->nGMode & GMODE_PATH_OBJECT )
        *poPos = pAttr->oCurPos;

    return OK;
}
TPoint            PageGetCurrentTextPos    (Page pPage)
{
    TPoint oPos = {0, 0};

    if ( PageValidate( pPage ) ) 
	{
        PageAttr pAttr = (PageAttr)pPage->pAttr;

        if ( pAttr->nGMode & GMODE_TEXT_OBJECT )
            oPos = pAttr->oTextPos;
    }
    
    return oPos;
}


unsigned long     PageGetCurrentTextPos2   (Page pPage, TPoint *poPos)
{
    if ( !PageValidate( pPage ))
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    poPos->fX = 0;
    poPos->fY = 0;


    if ( pAttr->nGMode & GMODE_TEXT_OBJECT )
        *poPos = pAttr->oTextPos;

    return OK;
}
unsigned long     PageSetRotate            (Page pPage, unsigned short nAngle)
{
    Number oNumber = NULL;
    
	unsigned long nRet = OK;

    if ( !PageValidate( pPage ))
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

    if ( 0 != nAngle % 90 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_ROTATE_VALUE, (unsigned long)nAngle);

    oNumber = (Number)PageGetInheritableItem( pPage, "Rotate", OCLASS_NUMBER );

    if ( !oNumber )
        nRet = DictAddNumber( pPage, "Rotate", nAngle );
    else
		oNumber->nValue = nAngle;

    return nRet;
}
unsigned long     PageSetWidth             (Page pPage, float fValue)
{
    if ( fValue < 1 || fValue > 14400 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_SIZE, 0);

    if ( OK != PageSetBoxValue( pPage, "MediaBox", 2, fValue) )
		return CheckError( pPage->oError );

    return OK;
}
unsigned long     PageSetHeight            (Page pPage, float fValue)
{
    if ( fValue < 1 || fValue > 14400 )
		return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_SIZE, 0);

    if ( OK != PageSetBoxValue( pPage, "MediaBox", 3, fValue) )
		return CheckError( pPage->oError);

    return OK;
}
unsigned long     PageSetSize              (Page pPage, PageSizes eSize, PageDirection eDirection)
{
    unsigned long nRet = OK;

    if ( !PageValidate( pPage ))
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE;

    if ( eSize < 0 || eSize >= PageSizeEOF )
        return RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_SIZE, (unsigned long)eSize );

    if ( PageLandscape == eDirection ) 
	{
		nRet += PageSetHeight( pPage, c_asPredefinedPageSizes[ (unsigned int)eSize ].fX );
		nRet += PageSetWidth ( pPage, c_asPredefinedPageSizes[ (unsigned int)eSize ].fY );
    } 
	else if ( PagePortrait == eDirection ) 
	{
		nRet += PageSetHeight( pPage, c_asPredefinedPageSizes[ (unsigned int)eSize ].fY );
		nRet += PageSetWidth ( pPage, c_asPredefinedPageSizes[ (unsigned int)eSize ].fX );
    } 
	else
		nRet = SetError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DIRECTION, (unsigned long)eDirection );

    if ( OK != nRet )
		return CheckError( pPage->oError );

    return OK;
}
Destination       PageCreateDestination    (Page pPage)
{
    if ( !PageValidate( pPage ) )
        return NULL;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    Destination pDest = DestinationNew( pPage->oMMgr, pPage, pAttr->pXref );

	if ( !pDest )
		CheckError( pPage->oError );

    return pDest;
}
Annotation        PageCreateTextAnnot      (Page pPage, TRect oRect, const char *sText, EncoderRecPtr pEncoder)
{
    if ( !PageValidate( pPage ) )
        return NULL;

	PageAttr pAttr = (PageAttr)pPage->pAttr;
    if ( pEncoder && !EncoderValidate( pEncoder ) ) 
	{
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER, 0);
        return NULL;
    }
	Annotation pAnnot = TextAnnotNew( pPage->oMMgr, pAttr->pXref, oRect, sText, pEncoder);
    if ( pAnnot )  
	{
        if ( OK != AddAnnotation( pPage, pAnnot ) ) 
		{
			CheckError( pPage->oError );
            pAnnot = NULL;
        }
    } 
	else
		CheckError( pPage->oError );

    return pAnnot;
}
Annotation        PageCreateLinkAnnot      (Page pPage, TRect oRect, Destination pDest)
{
    if ( !PageValidate( pPage ) )
        return NULL;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

	if ( !DestinationValidate( pDest ) ) 
	{
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION, 0);
        return NULL;
    }

	Annotation pAnnot = LinkAnnotNew( pPage->oMMgr, pAttr->pXref, oRect, pDest );
    
	if ( pAnnot ) 
	{
        if ( OK != AddAnnotation( pPage, pAnnot ) ) 
		{
			CheckError( pPage->oError );
            pAnnot = NULL;
        }
    } 
	else
		CheckError( pPage->oError );

    return pAnnot;
}

Annotation        PageCreateUriLinkAnnot   (Page pPage, TRect oRect, const char *sUri)
{
    if ( !PageValidate( pPage ) )
        return NULL;

	PageAttr pAttr = (PageAttr)pPage->pAttr;

    if ( UtilsStrLen( sUri, LIMIT_MAX_STRING_LEN ) <= 0 ) 
	{
		RaiseError( pPage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_URL, 0);
        return NULL;
    }

	Annotation pAnnot = URILinkAnnotNew( pPage->oMMgr, pAttr->pXref, oRect, sUri);

    if ( pAnnot ) 
	{
        if ( OK != AddAnnotation( pPage, pAnnot) ) 
		{
			CheckError( pPage->oError );
            pAnnot = NULL;
        }
    } 
	else
        CheckError( pPage->oError );

    return pAnnot;
}
