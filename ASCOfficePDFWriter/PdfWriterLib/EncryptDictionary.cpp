#include "EncryptDictionary.h"

//-------- EncryptDict - Основные функции ------------------------------------------------
void          EncryptDictOnFree     (Dict pObj)
{
    EncryptRecPtr pAttr = (EncryptRecPtr)pObj->pAttr;
    if ( pAttr )
        FreeMem( pObj->oMMgr, pAttr);
}
EncryptDict   EncryptDictNew        (MMgr oMMgr, Xref pXref)
{
	EncryptDict pDict = (EncryptDict)DictNew( oMMgr );
    if ( !pDict )
        return NULL;

	pDict->pHeader.nObjClass |= OSUBCLASS_ENCRYPT;
	pDict->pFreeFn = EncryptDictOnFree;

	EncryptRecPtr pAttr = (EncryptRecPtr)GetMem( pDict->oMMgr, sizeof(EncryptRec) );
    if ( !pAttr ) 
	{
        DictFree( pDict );
        return NULL;
    }

	pDict->pAttr = pAttr;
    EncryptInit( pAttr );

    if ( OK != XrefAdd( pXref, pDict ) )
        return NULL;

    return pDict;
}
#if defined(_WIN32) || defined(_WIN64)
	#include <time.h>
#endif
void          EncryptDictCreateID   (EncryptDict pDict, Dict pInfo, Xref pXref)
{
    MD5Context oContext;
    EncryptRecPtr pAttr = (EncryptRecPtr)pDict->pAttr;
   
	MD5Init( &oContext );

    time_t oTime = time(NULL);
    MD5Update( &oContext, (BYTE*)&oTime, sizeof(oTime) );

    // Создаем идентификатор файла по элементам библиотеки Info.
	if ( pInfo ) 
	{
        const char *sTemp = NULL;
        unsigned int nLen = 0;

        // Author
        sTemp = InfoGetInfoAttr( pInfo, InfoAuthor );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );

        // Creator
		sTemp = InfoGetInfoAttr( pInfo, InfoCreator );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );

        // Producer   
        sTemp = InfoGetInfoAttr( pInfo, InfoProducer );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );
 
        // Title   
		sTemp = InfoGetInfoAttr( pInfo, InfoTitle );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );

        // Subject   
		sTemp = InfoGetInfoAttr( pInfo, InfoSubject );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );

        // Keywords   
        sTemp = InfoGetInfoAttr( pInfo, InfoKeyWords );
        if ( ( nLen = UtilsStrLen( sTemp, -1 ) ) > 0 )
            MD5Update( &oContext, (const BYTE *)sTemp, nLen );

		MD5Update( &oContext, (const BYTE *)&( pXref->pEntries->nCount ), sizeof(unsigned int) );

    }
    MD5Final( pAttr->anEncryptID, &oContext );
}
unsigned long EncryptDictSetPassword(EncryptDict pDict, const char *sOwnerPassword, const char *sUserPassword)
{
	EncryptRecPtr pAttr = (EncryptRecPtr)pDict->pAttr;

    if ( 0 == UtilsStrLen( sOwnerPassword, 2 ) )
		return SetError( pDict->oError, AVS_OFFICEPDFWRITER_ERROR_ENCRYPT_INVALID_PASSWORD, 0 );

    if ( sOwnerPassword && sUserPassword && 0 == UtilsStrCmp( sOwnerPassword, sUserPassword ) )
		return SetError( pDict->oError, AVS_OFFICEPDFWRITER_ERROR_ENCRYPT_INVALID_PASSWORD, 0 );

	PadOrTrancatePassword( sOwnerPassword, pAttr->anOwnerPassword );
	PadOrTrancatePassword( sUserPassword,  pAttr->anUserPassword );

    return OK;
}
BOOL          EncryptDictValidate   (EncryptDict pDict)
{
    ObjHeader *pHeader = (ObjHeader *)pDict;

    if ( !pDict || !pDict->pAttr )
        return FALSE;

    if ( pHeader->nObjClass != (OCLASS_DICT | OSUBCLASS_ENCRYPT) )
        return FALSE;

    return TRUE;
}
unsigned long EncryptDictPrepare    (EncryptDict pDict, Dict pInfo, Xref pXref)
{
    unsigned long nRet = OK;
    EncryptRecPtr pAttr = (EncryptRecPtr)pDict->pAttr;

    EncryptDictCreateID( pDict, pInfo, pXref );
    EncryptCreateOwnerKey( pAttr );
    EncryptCreateEncryptionKey( pAttr );
    EncryptCreateUserKey( pAttr );

	Binary pOwnerKey = BinaryNew( pDict->oMMgr, pAttr->anOwnerKey, PASSWD_LEN );
    if ( !pOwnerKey )
		return ErrorGetCode( pDict->oError );

    if ( OK != ( nRet = DictAdd( pDict, "O", pOwnerKey ) ) )
        return nRet;

	Binary pUserKey = BinaryNew( pDict->oMMgr, pAttr->anUserKey, PASSWD_LEN );
    if ( !pUserKey )
		return ErrorGetCode( pDict->oError );

    if ( OK != ( nRet = DictAdd( pDict, "U", pUserKey ) ) )
        return nRet;

    nRet += DictAddName( pDict, "Filter", "Standard" );

	if ( EncryptR2 == pAttr->eMode ) 
	{
        nRet += DictAddNumber( pDict, "V", 1 );
        nRet += DictAddNumber( pDict, "R", 2 );
    } 
	else if ( EncryptR3 == pAttr->eMode ) 
	{
        nRet += DictAddNumber( pDict, "V", 2);
        nRet += DictAddNumber( pDict, "R", 3);
		nRet += DictAddNumber( pDict, "Length", pAttr->nKeyLen * 8 );
    }

	nRet += DictAddNumber( pDict, "P", pAttr->nPermission );

    if ( OK != nRet )
		return ErrorGetCode( pDict->oError );

    return OK;
}
EncryptRecPtr       EncryptDictGetAttr    (EncryptDict pDict)
{
    ObjHeader *pHeader = (ObjHeader *)pDict;

    if ( pDict && pDict->pAttr && ( pHeader->nObjClass == (OCLASS_DICT | OSUBCLASS_ENCRYPT) ) )
        return (EncryptRecPtr)pDict->pAttr;

    return NULL;
}
