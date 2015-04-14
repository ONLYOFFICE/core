#include "Catalog.h"

Catalog       CatalogNew                (MMgr oMMgr, Xref pXref)
{
    Catalog pCatalog;
    unsigned long nRet = OK;

    pCatalog = DictNew( oMMgr );
    if ( !pCatalog )
        return NULL;

	pCatalog->pHeader.nObjClass |= OSUBCLASS_CATALOG;

    if ( OK != XrefAdd( pXref, pCatalog ) )
        return NULL;

    // добавляем необходимые элементы 
    nRet += DictAddName( pCatalog, "Type", "Catalog");
    nRet += DictAdd( pCatalog, "Pages", PagesNew( oMMgr, NULL, pXref ) );

    if ( OK != nRet )
        return NULL;

    return pCatalog;
}


Pages         CatalogGetRoot            (Catalog pCatalog)
{
    Dict pPages;

    if ( !pCatalog )
        return NULL;

    pPages = (Dict)DictGetItem( pCatalog, "Pages", OCLASS_DICT);
	if ( !pPages || pPages->pHeader.nObjClass != (OSUBCLASS_PAGES | OCLASS_DICT) )
		SetError( pCatalog->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_GET_ROOT_PAGES, 0 );

    return pPages;
}


PageLayout    CatalogGetPageLayout      (Catalog pCatalog)
{
    Name oLayout;
    unsigned int nIndex = 0;

    oLayout = (Name)DictGetItem( pCatalog, "PageLayout", OCLASS_NAME);
    if ( !oLayout )
        return PLEOF;

    while ( PAGE_LAYOUT_NAMES[nIndex] ) 
	{
		if ( UtilsStrCmp( oLayout->sValue, PAGE_LAYOUT_NAMES[nIndex] ) == 0 )
            return (PageLayout)nIndex;
        nIndex++;
    }

    return PLEOF;
}



unsigned long CatalogSetPageLayout      (Catalog pCatalog, PageLayout eLayout)
{
    return DictAddName( pCatalog, "PageLayout", PAGE_LAYOUT_NAMES[ (int)eLayout ] );
}

PageMode      CatalogGetPageMode        (Catalog pCatalog)
{
    Name oMode;
    unsigned int nIndex = 0;

    oMode = (Name)DictGetItem( pCatalog, "PageMode", OCLASS_NAME );
    if ( !oMode )
        return PageModeUseNone;

    while ( PAGE_MODE_NAMES[nIndex] ) 
	{
		if ( UtilsStrCmp( oMode->sValue, PAGE_MODE_NAMES[nIndex] ) == 0 )
            return (PageMode)nIndex;
        nIndex++;
    }

    return PageModeUseNone;
}


unsigned long CatalogSetPageMode        (Catalog pCatalog, PageMode eMode)
{
    return DictAddName( pCatalog, "PageMode", PAGE_MODE_NAMES[ (int)eMode ] );
}
unsigned long CatalogSetOpenAction      (Catalog pCatalog, Destination pOpenAction)
{
    if ( !pOpenAction ) 
	{
        DictRemoveElement( pCatalog, "OpenAction" );
        return OK;
    }

    return DictAdd( pCatalog, "OpenAction", pOpenAction );
}


unsigned long CatalogAddPageLabel       (Catalog pCatalog, unsigned int nPageNum, Dict pPageLabel)
{
    unsigned long nRet = OK;
    Array oNums;
    Dict pLabels = (Dict)DictGetItem( pCatalog, "PageLabels", OCLASS_DICT);

    if ( !pLabels ) 
	{
		pLabels = DictNew( pCatalog->oMMgr );

        if ( !pLabels )
			return pCatalog->oError->nErrorNo;

        if ( OK != ( nRet = DictAdd( pCatalog, "PageLabels", pLabels ) ) )
            return nRet;
    }

    oNums = (Array)DictGetItem( pLabels, "Nums", OCLASS_ARRAY );

    if ( !oNums ) 
	{
		oNums = ArrayNew( pCatalog->oMMgr );

        if ( !oNums )
			return pCatalog->oError->nErrorNo;

        if ( OK != ( nRet = DictAdd( pLabels, "Nums", oNums ) ) )
            return nRet;
    }

    if ( OK != ( nRet = ArrayAddNumber( oNums, nPageNum ) ) )
        return nRet;

    return ArrayAdd( oNums, pPageLabel);
}

unsigned int  CatalogGetViewerPreference(Catalog pCatalog)
{
    Dict pPreferences;
    unsigned int nValue = 0;
    BooleanRecPtr oBool;

    pPreferences = (Dict)DictGetItem( pCatalog, "ViewerPreferences", OCLASS_DICT);

    if ( !pPreferences )
        return 0;

    oBool = (BooleanRecPtr)DictGetItem( pPreferences, "HideToolbar", OCLASS_BOOLEAN);
    if ( oBool ) 
	{
		if ( oBool->bValue )
            nValue += HIDE_TOOLBAR;
    }

    oBool = (BooleanRecPtr)DictGetItem( pPreferences, "HideMenubar", OCLASS_BOOLEAN);
    if ( oBool ) 
	{
		if ( oBool->bValue )
            nValue += HIDE_MENUBAR;
    }

    oBool = (BooleanRecPtr)DictGetItem( pPreferences, "HideWindowUI", OCLASS_BOOLEAN);
    if ( oBool ) 
	{
		if ( oBool->bValue )
            nValue += HIDE_WINDOW_UI;
    }

    oBool = (BooleanRecPtr)DictGetItem( pPreferences, "FitWindow", OCLASS_BOOLEAN);
    if ( oBool ) 
	{
		if ( oBool->bValue )
            nValue += FIT_WINDOW;
    }

    oBool = (BooleanRecPtr)DictGetItem( pPreferences, "CenterWindow", OCLASS_BOOLEAN);
    if ( oBool ) 
	{
		if ( oBool->bValue )
            nValue += CENTER_WINDOW;
    }

    return nValue;
}


unsigned long CatalogSetViewerPreference(Catalog pCatalog, unsigned int nValue)
{
    unsigned long nRet = OK;
    Dict pPreferences = NULL;

    if ( !nValue ) 
	{
        nRet = DictRemoveElement( pCatalog, "ViewerPreferences" );

        if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND == nRet )
            nRet = OK;

        return nRet;
    }

	pPreferences = DictNew( pCatalog->oMMgr );
    if ( !pPreferences )
		return pCatalog->oError->nErrorNo;

    if ( OK != ( nRet = DictAdd( pCatalog, "ViewerPreferences", pPreferences ) ) )
        return nRet;

    /*  */

    if ( nValue & HIDE_TOOLBAR) 
	{
        if ( OK != ( nRet = DictAddBoolean( pPreferences, "HideToolbar", true ) ) )
            return nRet;
    } 
	else 
	{
        if ( OK != ( nRet = DictRemoveElement( pPreferences, "HideToolbar" ) ) )
            if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND != nRet )
                return nRet;
    }

    if ( nValue & HIDE_MENUBAR) 
	{
        if ( OK != ( nRet = DictAddBoolean( pPreferences, "HideMenubar", true) ) )
            return nRet;
    } 
	else 
	{
        if ( OK != ( nRet = DictRemoveElement( pPreferences, "HideMenubar" ) ) )
            if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND != nRet )
                return nRet;
    }

    if ( nValue & HIDE_WINDOW_UI ) 
	{
        if ( OK != ( nRet = DictAddBoolean( pPreferences, "HideWindowUI", true ) ) )
            return nRet;
    } 
	else 
	{
        if ( OK != ( nRet = DictRemoveElement( pPreferences, "HideWindowUI" ) ) )
            if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND != nRet )
                return nRet;
    }

    if ( nValue & FIT_WINDOW ) 
	{
        if ( OK != ( nRet = DictAddBoolean( pPreferences, "FitWindow", true ) ) )
            return nRet;
    } 
	else 
	{
        if ( OK != ( nRet = DictRemoveElement( pPreferences, "FitWindow" ) ) )
            if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND != nRet )
                return nRet;
    }

    if ( nValue & CENTER_WINDOW ) 
	{
        if ( OK != ( nRet = DictAddBoolean( pPreferences, "CenterWindow", true ) ) )
            return nRet;
    } 
	else 
	{
        if ( OK != ( nRet = DictRemoveElement( pPreferences, "CenterWindow" ) ) )
            if ( AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND != nRet )
                return nRet;
    }

    return OK;
}


bool    CatalogValidate           (Catalog pCatalog)
{
    if ( !pCatalog )
        return false;

	if ( pCatalog->pHeader.nObjClass != (OSUBCLASS_CATALOG | OCLASS_DICT) ) 
	{
		SetError( pCatalog->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return false;
    }

    return true;
}
