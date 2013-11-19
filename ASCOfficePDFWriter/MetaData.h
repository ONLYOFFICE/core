#ifndef _INFO_H
#define _INFO_H

#include "Objects.h"
#include "Utils.h"

static const char  *c_asInfoItemNames[] = 
{
    "CreationDate",
    "ModDate",
    "Author",
    "Creator",
    "Producer",
    "Title",
    "Subject",
    "Keywords",
    NULL
};

static const char* InfoTypeToName       (InfoType eType)
{
    unsigned int nIndex = (unsigned int)eType;

    return c_asInfoItemNames[ nIndex ];
}

unsigned long      InfoSetInfoAttr      (Dict pInfo, InfoType eType, const char *sValue, Encoder pEncoder)
{
    const char* sName = InfoTypeToName( eType );

    if ( eType <= InfoModaDate )
		return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0 );

	return DictAdd( pInfo, sName, StringNew( pInfo->oMMgr, sValue, pEncoder ) );
}

const char*        InfoGetInfoAttr      (Dict pInfo, InfoType eType)
{
    const char* sName = InfoTypeToName( eType );
    
	String oString = NULL;

    if ( !pInfo )
        return NULL;

    oString = (String)DictGetItem( pInfo, sName, OCLASS_STRING );

    if ( !oString )
        return NULL;
    else
		return (const char*)oString->pValue;
}

unsigned long      InfoSetInfoDateAttr  (Dict pInfo, InfoType eType, TDate oDate)
{
    char sTemp[DATE_TIME_STR_LEN + 1];
    char* pTemp = NULL;
    const char* sName = InfoTypeToName( eType );

    if ( eType > InfoModaDate)
		return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);

    UtilsMemSet( sTemp, 0, DATE_TIME_STR_LEN + 1 );

	if ( oDate.nMonth < 1    || 12 < oDate.nMonth   ||
		 oDate.nDay   < 1    ||
		 23 < oDate.nHour    || 59 < oDate.nMinutes || 59 < oDate.nSeconds ||
		 (oDate.nInd != '+'  && oDate.nInd != '-'   && oDate.nInd != 'Z'   && oDate.nInd != ' ') ||
		 23 < oDate.nOffHour || 59 < oDate.nOffMinutes ) 
	{
		return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME, 0);
    }

	switch ( oDate.nMonth ) 
	{
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
			if ( oDate.nDay > 31 )
                return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME, 0);

            break;
        case 4:
        case 6:
        case 9:
        case 11:
			if ( oDate.nDay > 30 )
                return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME, 0);

            break;
        case 2:
			if ( oDate.nDay > 29 || ( oDate.nDay == 29 && ( oDate.nYear % 4 != 0 || ( oDate.nYear % 100 == 0 && oDate.nYear % 400 != 0 ) ) ) )
                return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME, 0);

            break;
        default:
            return SetError( pInfo->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME, 0);
    }

    pTemp = (char*)UtilsMemCpy( (BYTE*)sTemp, (BYTE*)"D:", 2);
	pTemp = UtilsIToA2( pTemp, oDate.nYear,    5);
	pTemp = UtilsIToA2( pTemp, oDate.nMonth,   3);
	pTemp = UtilsIToA2( pTemp, oDate.nDay,     3);
	pTemp = UtilsIToA2( pTemp, oDate.nHour,    3);
	pTemp = UtilsIToA2( pTemp, oDate.nMinutes, 3);
	pTemp = UtilsIToA2( pTemp, oDate.nSeconds, 3);

	if ( ' ' != oDate.nInd ) 
	{
        *pTemp++ = oDate.nInd;
		pTemp = UtilsIToA2( pTemp, oDate.nOffHour,    3);
        *pTemp++ = '\'';
		pTemp = UtilsIToA2( pTemp, oDate.nOffMinutes, 3);
        *pTemp++ = '\'';
    }
    *pTemp = 0;

	return DictAdd( pInfo, sName, StringNew( pInfo->oMMgr, sTemp, NULL ) );
}

#endif /* _INFO_H */

