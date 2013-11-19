#ifndef _PAGE_LABEL_H
#define _PAGE_LABEL_H

#include "Utils.h"
#include "Document.h"

Dict PageLabelNew(Doc pPDF, PageNumStyle eStyle, int nFirstPage, const char *sPrefix)
{
    Dict pObj = DictNew( pPDF->oMMgr );

    if ( !pObj )
        return NULL;

    switch ( eStyle ) 
	{
        case PageNumStyleDecimal:
            if ( OK != DictAddName( pObj, "S", "D" ) )
			{
				DictFree( pObj );
				return NULL;
			}
            break;
        case PageNumStyleUpperRoman:
            if ( OK != DictAddName( pObj, "S", "R" ) )
			{
				DictFree( pObj );
				return NULL;
			}
            break;
        case PageNumStyleLowerRoman:
            if ( OK != DictAddName( pObj, "S", "r" ) )
			{
				DictFree( pObj );
				return NULL;
			}
            break;
        case PageNumStyleUpperLetters:
            if ( OK != DictAddName( pObj, "S", "A" ) )
			{
				DictFree( pObj );
				return NULL;
			}
            break;
        case PageNumStyleLowerLetters:
            if ( OK != DictAddName( pObj, "S", "a" ) )
			{
				DictFree( pObj );
				return NULL;
			}
            break;
        default:
			SetError( &pPDF->oError, AVS_OFFICEPDFWRITER_ERROR_PAGE_NUM_STYLE_OUT_OF_RANGE, (unsigned long)eStyle);
			DictFree( pObj );
			return NULL;
    }

    if ( sPrefix && 0 != sPrefix[0] )
	{
		if ( OK != DictAdd( pObj, "P", StringNew( pPDF->oMMgr, sPrefix, pPDF->pDefEncoder ) ) )
			{
				DictFree( pObj );
				return NULL;
			}
	}
    if ( 0 != nFirstPage )
	{
        if ( OK != DictAddNumber( pObj, "St", nFirstPage) )
			{
				DictFree( pObj );
				return NULL;
			}
	}
    return pObj;

}


#endif /* _PAGE_LABEL_H */

