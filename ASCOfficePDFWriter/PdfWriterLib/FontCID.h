#ifndef _FONT_CID_H
#define _FONT_CID_H

#pragma once

#include "Utils.h"
#include "FontTypes.h"

#include "Font.h"

//----- CIDTypeFontDef вспомогательные функции ------------------------------------------------------
void CIDFontDefFreeWidth  (FontDef pFontDef);
void CIDFontDefFreeFunc   (FontDef pFontDef);
//----- CIDTypeFontDef основные функции -------------------------------------------------------------
FontDef       CIDFontDefNew         (MMgr oMMgr, char *sName, FontDefInitFunc pInitFn);
unsigned long CIDFontDefLoadFontData(FontDef pFontDef, std::wstring  wsFilePath, CIDFontFileType eCIDType);
void          CIDFontDefSetInfoSys  (FontDef pFontDef, char *sRegistry, char *sOrdering, int nSupplement);
void          CIDFontDefSetDW       (FontDef pFontDef, int nDW);
void          CIDFontDefSetDW2      (FontDef pFontDef, int anDW2[2]);
short         CIDFontDefGetCIDWidth (FontDef pFontDef, unsigned short nCid);
unsigned long CIDFontDefAddWidth    (FontDef pFontDef, const CIDWidth *pWidths);
unsigned long CIDFontDefAddWidth    (FontDef pFontDef, CIDWidth oWidth);
unsigned long CIDFontDefAddWidth2   (FontDef pFontDef, const CIDWidth2 *pWidths);
unsigned long CIDFontDefAddWidth2   (FontDef pFontDef, CIDWidth2 oWidths2);
unsigned long CIDFontDefSetCIDtoGID (FontDef pFontDef, char *sName);
unsigned long CIDFontDefSetCIDtoGID (FontDef pFontDef, BYTE *pBuffer, int nSize);
unsigned long CIDFontDefChangeStyle (FontDef pFontDef, BOOL bBold, BOOL bItalic);
//----- CIDTypeFont вспомогательные функции ---------------------------------------------------------
//static unsigned long CIDFontType2BeforeWriteFunc  (Dict pObj)
//{
//    FontAttr pFontAttr = (FontAttr)pObj->pAttr;
//    FontDef pDef = pFontAttr->pFontDef;
//    TTFontDefAttr pDefAttr = (TTFontDefAttr)pDef->pAttr;
//    unsigned long nRet = OK;
//
//    if ( pFontAttr->pMapStream )
//        pFontAttr->pMapStream->nFilter = pObj->nFilter;
//
//    if ( pFontAttr->pCMapStream )
//        pFontAttr->pCMapStream->nFilter = pObj->nFilter;
//
//	if ( !pFontAttr->pFontDef->pDescriptor ) 
//	{
//		Dict pDescriptor = DictNew( pObj->oMMgr );
//
//        if ( !pDescriptor )
//			return ErrorGetCode( pObj->oError );
//
//		if ( pDefAttr->bEmbedding ) 
//		{
//			Dict pFontData = DictStreamNew( pObj->oMMgr, pFontAttr->pXref );
//
//            if ( !pFontData )
//				return ErrorGetCode( pObj->oError );
//
//			if ( OK != TTFontDefSaveFontData( pFontAttr->pFontDef, pFontData->pStream ) )
//                return ErrorGetCode( pObj->oError );
//
//            nRet += DictAdd( pDescriptor, "FontFile2", pFontData );
//			nRet += DictAddNumber( pFontData, "Length1", pDefAttr->nLength1 );
//            nRet += DictAddNumber( pFontData, "Length2", 0);
//            nRet += DictAddNumber( pFontData, "Length3", 0);
//
//			pFontData->nFilter = pObj->nFilter;
//
//            if ( OK != nRet )
//                return ErrorGetCode( pObj->oError );
//        }
//
//		nRet += XrefAdd( pFontAttr->pXref, pDescriptor );
//        nRet += DictAddName( pDescriptor, "Type", "FontDescriptor" );
//		nRet += DictAddNumber( pDescriptor, "Ascent", pDef->nAscent );
//		nRet += DictAddNumber( pDescriptor, "Descent", pDef->nDescent );
//		nRet += DictAddNumber( pDescriptor, "Flags", pDef->nFlags );
//
//		Array oArray = BoxArrayNew( pObj->oMMgr, pDef->oFontBox );
//        nRet += DictAdd( pDescriptor, "FontBBox", oArray );
//
//		nRet += DictAddName( pDescriptor, "FontName", pDefAttr->sBaseFont );
//		nRet += DictAddNumber( pDescriptor, "ItalicAngle", pDef->nItalicAngle );
//		nRet += DictAddNumber( pDescriptor, "StemV", pDef->nStemV );
//		nRet += DictAddNumber( pDescriptor, "XHeight", pDef->nXHeight );
//
//        if ( OK != nRet )
//			return ErrorGetCode( pObj->oError );
//
//		pFontAttr->pFontDef->pDescriptor = pDescriptor;
//    }
//
//	if ( OK != ( nRet = DictAddName( pObj, "BaseFont", pDefAttr->sBaseFont ) ) )
//        return nRet;
//
//	if ( OK != ( nRet = DictAddName( pFontAttr->pDescendantFont, "BaseFont", pDefAttr->sBaseFont ) ) )
//        return nRet;
//
//	return DictAdd( pFontAttr->pDescendantFont, "FontDescriptor", pFontAttr->pFontDef->pDescriptor );
//}
//static FontDict      CIDFontTypeNew               (FontDict pParent, Xref pXref)
//{
//    unsigned long nRet = OK;
//    FontAttr pAttr = (FontAttr)pParent->pAttr;
//    FontDef pFontDef = pAttr->pFontDef;
//    CIDFontDefAttr pFontDefAttr = (CIDFontDefAttr)pFontDef->pAttr;
//	EncoderRecPtr pEncoder = pAttr->pEncoder;
//	CMapEncoderStreamAttr pEncoderAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;
//
//	FontDict pFont = DictNew( pParent->oMMgr );
//    if ( !pFont )
//        return NULL;
//
//    if ( OK != XrefAdd( pXref, pFont ) )
//        return NULL;
//
//    nRet += DictAddName( pFont, "Type", "Font" );
//	if ( cidType0 == pFontDefAttr->eType || cidType0C == pFontDefAttr->eType || cidType0COT == pFontDefAttr->eType )
//		nRet += DictAddName( pFont, "Subtype", "CIDFontType0" );
//	else// if ( cidType2 == pFontDefAttr->eType || cidType2OT == pFontDefAttr->eType )
//		nRet += DictAddName( pFont, "Subtype", "CIDFontType2" );
//
//	nRet += DictAddName( pFont, "BaseFont", pFontDef->sBaseFont );
//
//	nRet += DictAddNumber( pFont, "DW", pFontDefAttr->nDW );
//    if ( OK != nRet )
//        return NULL;
//
//    // Добавим элемент 'DW2'.
//	Array oArray = ArrayNew( pParent->oMMgr );
//    if ( !oArray )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "DW2", oArray ) )
//        return NULL;
//
//	nRet += ArrayAddNumber( oArray, pFontDefAttr->anDW2[0]);
//	nRet += ArrayAddNumber( oArray, pFontDefAttr->anDW2[1]);
//
//    if ( OK != nRet )
//        return NULL;
//
//	if ( pFontDefAttr->pW->nCount > 0 )
//	{
//		// Добавим элемент  'W'.
//		oArray = ArrayNew( pParent->oMMgr );
//		if ( !oArray )
//			return NULL;
//
//		if ( OK != DictAdd( pFont, "W", oArray ) )
//			return NULL;
//
//		unsigned short nSaveCid = 0;
//		Array oSubArray = NULL;
//		for ( unsigned int nIndex = 0; nIndex < pFontDefAttr->pW->nCount; nIndex++) 
//		{
//			CIDWidth *pCurWidth = (CIDWidth *)ListItemAt( pFontDefAttr->pW, nIndex );
//
//			if ( pCurWidth->unCid_First != pCurWidth->unCid_Last )
//			{
//				// Формат с1 ... с2 w
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_Last );
//				nRet += ArrayAddNumber( oArray, pCurWidth->shWidth );
//
//				continue;
//			}
//
//			if ( pCurWidth->unCid_First != nSaveCid + 1 || !oSubArray ) 
//			{
//				oSubArray = ArrayNew( pParent->oMMgr );
//				if ( !oSubArray )
//					return NULL;
//
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//				nRet += ArrayAdd( oArray, oSubArray );
//			}
//
//			nRet += ArrayAddNumber( oSubArray, pCurWidth->shWidth );
//			nSaveCid = pCurWidth->unCid_First;
//
//			if ( OK != nRet )
//				return NULL;
//		}
//	}
//
//	if ( pFontDefAttr->pW2->nCount > 0 )
//	{
//		// Добавим элемент  'W2'.
//		oArray = ArrayNew( pParent->oMMgr );
//		if ( !oArray )
//			return NULL;
//
//		if ( OK != DictAdd( pFont, "W2", oArray ) )
//			return NULL;
//
//		unsigned short nSaveCid = 0;
//		Array oSubArray = NULL;
//		for ( unsigned int nIndex = 0; nIndex < pFontDefAttr->pW2->nCount; nIndex++) 
//		{
//			CIDWidth2 *pCurWidth = (CIDWidth2 *)ListItemAt( pFontDefAttr->pW2, nIndex );
//
//			if ( pCurWidth->unCid_First != pCurWidth->unCid_Last )
//			{
//				// Формат с1 ... с2 W Vx Vy
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_Last );
//				nRet += ArrayAddNumber( oArray, pCurWidth->shW );
//				nRet += ArrayAddNumber( oArray, pCurWidth->shVx );
//				nRet += ArrayAddNumber( oArray, pCurWidth->shVy );
//
//				continue;
//			}
//
//			if ( pCurWidth->unCid_First != nSaveCid + 1 || !oSubArray ) 
//			{
//				oSubArray = ArrayNew( pParent->oMMgr );
//				if ( !oSubArray )
//					return NULL;
//
//				nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//				nRet += ArrayAdd( oArray, oSubArray );
//			}
//
//			nRet += ArrayAddNumber( oSubArray, pCurWidth->shW );
//			nRet += ArrayAddNumber( oSubArray, pCurWidth->shVx );
//			nRet += ArrayAddNumber( oSubArray, pCurWidth->shVy );
//			nSaveCid = pCurWidth->unCid_First;
//
//			if ( OK != nRet )
//				return NULL;
//		}
//	}
//
//
//	Dict pDescriptor = DictNew( pParent->oMMgr );
//    if ( !pDescriptor )
//        return NULL;
//
//    if ( OK != XrefAdd( pXref, pDescriptor ) )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "FontDescriptor", pDescriptor ) )
//        return NULL;
//
//    nRet += DictAddName( pDescriptor, "Type", "FontDescriptor");
//	nRet += DictAddName( pDescriptor, "FontName", pFontDef->sBaseFont );
//	nRet += DictAddNumber( pDescriptor, "Ascent", pFontDef->nAscent );
//	nRet += DictAddNumber( pDescriptor, "Descent", pFontDef->nDescent );
//	nRet += DictAddNumber( pDescriptor, "CapHeight", pFontDef->nCapHeight );
//	nRet += DictAddNumber( pDescriptor, "MissingWidth", pFontDef->nMissingWidth );
//	nRet += DictAddNumber( pDescriptor, "Flags", pFontDef->nFlags );
//
//    if ( OK != nRet )
//        return NULL;
//
//	oArray = BoxArrayNew( pParent->oMMgr, pFontDef->oFontBox );
//	if ( !oArray )
//        return NULL;
//
//	nRet += DictAdd( pDescriptor, "FontBBox", oArray );
//	nRet += DictAddNumber( pDescriptor, "ItalicAngle", pFontDef->nItalicAngle );
//	nRet += DictAddNumber( pDescriptor, "StemV", pFontDef->nStemV );
//
//	if ( pFontDefAttr->pFontData ) 
//	{
//		Dict pFontData = DictStreamNew( pParent->oMMgr, pXref );
//
//		if ( !pFontData )
//			return NULL;
//
//		if ( OK != StreamWriteToStream( pFontDefAttr->pFontData, pFontData->pStream, STREAM_FILTER_NONE, NULL ) )
//			return NULL;
//
//		if ( cidType0 == pFontDefAttr->eType )
//			nRet += DictAdd( pDescriptor, "FontFile", pFontData );
//		else if ( cidType0C == pFontDefAttr->eType )
//		{
//			nRet += DictAdd( pDescriptor, "FontFile3", pFontData );
//			nRet += DictAddName( pFontData, "Subtype", "CIDFontType0C" );
//		}
//		else if ( cidType2 == pFontDefAttr->eType )
//		{
//			nRet += DictAdd( pDescriptor, "FontFile2", pFontData );
//		}
//		else // if ( cidType0COT == pFontDefAttr->eType || cidType2OT == pFontDefAttr->eType )
//		{
//			nRet += DictAdd( pDescriptor, "FontFile3", pFontData );
//			nRet += DictAddName( pFontData, "Subtype", "OpenType" );
//		}
//
//		pFontData->nFilter = pFont->nFilter;
//	}
//
//    if ( OK != nRet )
//        return NULL;
//
//	if ( strlen( pFontDefAttr->sRegistry ) > 0 && strlen( pFontDefAttr->sOrdering ) > 0 ) 
//	{
//		Dict pCIDSystemInfo = DictNew( pParent->oMMgr );
//		if ( !pCIDSystemInfo )
//			return NULL;
//
//		if ( OK != DictAdd( pFont, "CIDSystemInfo", pCIDSystemInfo ) )
//			return NULL;
//
//		nRet += DictAdd( pCIDSystemInfo, "Registry", StringNew( pParent->oMMgr, pFontDefAttr->sRegistry, NULL ) );
//		nRet += DictAdd( pCIDSystemInfo, "Ordering", StringNew( pParent->oMMgr, pFontDefAttr->sOrdering, NULL ) );
//		nRet += DictAddNumber( pCIDSystemInfo, "Supplement", pFontDefAttr->nSupplement );
//
//		if ( OK != nRet )
//			return NULL;
//	}
//
//	if ( 0 == pFontDefAttr->nCIDtoGIDtype )
//	{
//		nRet += DictAddName( pFont, "CIDToGIDMap", pFontDefAttr->sCIDtoGIDMap );
//	}
//	else if ( 1 == pFontDefAttr->nCIDtoGIDtype )
//	{
//		Dict pCIDtoGID = DictStreamNew( pParent->oMMgr, pXref );
//		if ( !pCIDtoGID )
//			return NULL;
//
//		if ( OK != DictAdd( pFont, "CIDToGIDMap", pCIDtoGID ) )
//			return NULL;
//
//		if ( OK != StreamWriteToStream( pFontDefAttr->pCIDtoGIDMap, pCIDtoGID->pStream, STREAM_FILTER_NONE, NULL ) )
//			return NULL;
//
//#ifdef FILTER_FLATE_DECODE_ENABLED
//		pCIDtoGID->nFilter |= STREAM_FILTER_FLATE_DECODE;
//#endif
//	}
//
//    return pFont;
//}
//static FontDict      CIDFontType0New              (FontDict pParent, Xref pXref)
//{
//    unsigned long nRet = OK;
//    FontAttr pAttr = (FontAttr)pParent->pAttr;
//    FontDef pFontDef = pAttr->pFontDef;
//    CIDFontDefAttr pFontDefAttr = (CIDFontDefAttr)pFontDef->pAttr;
//	EncoderRecPtr pEncoder = pAttr->pEncoder;
//	CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pEncoder->pAttr;
//
//	FontDict pFont = DictNew( pParent->oMMgr );
//    if ( !pFont )
//        return NULL;
//
//    if ( OK != XrefAdd( pXref, pFont ) )
//        return NULL;
//
//    nRet += DictAddName( pFont, "Type", "Font" );
//    nRet += DictAddName( pFont, "Subtype", "CIDFontType0" );
//	nRet += DictAddNumber( pFont, "DW", pFontDefAttr->nDW );
//	nRet += DictAddName( pFont, "BaseFont", pFontDef->sBaseFont );
//    if ( OK != nRet )
//        return NULL;
//
//    // Добавим элемент 'DW2'.
//	Array oArray = ArrayNew( pParent->oMMgr );
//    if ( !oArray )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "DW2", oArray ) )
//        return NULL;
//
//	nRet += ArrayAddNumber( oArray, pFontDefAttr->anDW2[0]);
//	nRet += ArrayAddNumber( oArray, pFontDefAttr->anDW2[1]);
//
//    if ( OK != nRet )
//        return NULL;
//
//    // Добавим элемент  'W'.
//	oArray = ArrayNew( pParent->oMMgr );
//    if ( !oArray )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "W", oArray ) )
//        return NULL;
//
//	unsigned short nSaveCid = 0;
//    Array oSubArray = NULL;
//	for ( unsigned int nIndex = 0; nIndex < pFontDefAttr->pW->nCount; nIndex++) 
//	{
//		CIDWidth *pCurWidth = (CIDWidth *)ListItemAt( pFontDefAttr->pW, nIndex );
// 
//		if ( pCurWidth->unCid_First != pCurWidth->unCid_Last )
//		{
//			// Формат с1 ... с2 w
//			nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//			nRet += ArrayAddNumber( oArray, pCurWidth->unCid_Last );
//			nRet += ArrayAddNumber( oArray, pCurWidth->shWidth );
//		}
//
//		if ( pCurWidth->unCid_First != nSaveCid + 1 || !oSubArray ) 
//		{
//			oSubArray = ArrayNew( pParent->oMMgr );
//            if ( !oSubArray )
//                return NULL;
//
//			nRet += ArrayAddNumber( oArray, pCurWidth->unCid_First );
//            nRet += ArrayAdd( oArray, oSubArray );
//        }
//
//		nRet += ArrayAddNumber( oSubArray, pCurWidth->shWidth );
//		nSaveCid = pCurWidth->unCid_First;
//
//        if ( OK != nRet )
//            return NULL;
//    }
//
//	Dict pDescriptor = DictNew( pParent->oMMgr );
//    if ( !pDescriptor )
//        return NULL;
//
//    if ( OK != XrefAdd( pXref, pDescriptor ) )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "FontDescriptor", pDescriptor ) )
//        return NULL;
//
//    nRet += DictAddName( pDescriptor, "Type", "FontDescriptor");
//	nRet += DictAddName( pDescriptor, "FontName", pFontDef->sBaseFont );
//	nRet += DictAddNumber( pDescriptor, "Ascent", pFontDef->nAscent );
//	nRet += DictAddNumber( pDescriptor, "Descent", pFontDef->nDescent );
//	nRet += DictAddNumber( pDescriptor, "CapHeight", pFontDef->nCapHeight );
//	nRet += DictAddNumber( pDescriptor, "MissingWidth", pFontDef->nMissingWidth );
//	nRet += DictAddNumber( pDescriptor, "Flags", pFontDef->nFlags );
//
//    if ( OK != nRet )
//        return NULL;
//
//	oArray = BoxArrayNew( pParent->oMMgr, pFontDef->oFontBox );
//	if ( !oArray )
//        return NULL;
//
//	nRet += DictAdd( pDescriptor, "FontBBox", oArray );
//	nRet += DictAddNumber( pDescriptor, "ItalicAngle", pFontDef->nItalicAngle );
//	nRet += DictAddNumber( pDescriptor, "StemV", pFontDef->nStemV );
//
//    if ( OK != nRet )
//        return NULL;
//
//	Dict pCIDSystemInfo = DictNew( pParent->oMMgr );
//    if ( !pCIDSystemInfo )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "CIDSystemInfo", pCIDSystemInfo ) )
//        return NULL;
//
//	nRet += DictAdd( pCIDSystemInfo, "Registry", StringNew( pParent->oMMgr, pEncoderAttr->sRegistry, NULL ) );
//	nRet += DictAdd( pCIDSystemInfo, "Ordering", StringNew( pParent->oMMgr, pEncoderAttr->sOrdering, NULL ) );
//	nRet += DictAddNumber( pCIDSystemInfo, "Supplement", pEncoderAttr->nSuppliment );
//
//    if ( OK != nRet )
//        return NULL;
//
//    return pFont;
//}
//static FontDict      CIDFontType2New              (FontDict pParent, Xref pXref)
//{
//    unsigned long nRet = OK;
//    FontAttr pAttr = (FontAttr)pParent->pAttr;
//    FontDef pFontDef = pAttr->pFontDef;
//	TTFontDefAttr pFontDefAttr = (TTFontDefAttr)pFontDef->pAttr;
//	EncoderRecPtr pEncoder = pAttr->pEncoder;
//	CMapEncoderAttr pEncoderAttr = (CMapEncoderAttr)pEncoder->pAttr;
//
//    unsigned short anTempMap[65536];
//
//    unsigned short nMax = 0;
//
//	FontDict pFont = DictNew( pParent->oMMgr );
//    if ( !pFont )
//        return NULL;
//
//    if ( OK != XrefAdd( pXref, pFont ) )
//        return NULL;
//
//	pParent->pBeforeWriteFn = CIDFontType2BeforeWriteFunc;
//
//	nRet += DictAddName( pFont, "Type", "Font");
//	nRet += DictAddName( pFont, "Subtype", "CIDFontType2");
//	nRet += DictAddNumber( pFont, "DW", pFontDef->nMissingWidth );
//    if ( OK != nRet )
//        return NULL;
//
//    // Добавим элемент 'DW2'.
//	Array oArray = ArrayNew( pFont->oMMgr );
//    if ( !oArray )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "DW2", oArray ) )
//        return NULL;
//
//	nRet += ArrayAddNumber( oArray, pFontDef->oFontBox.fBottom );
//	nRet += ArrayAddNumber( oArray, pFontDef->oFontBox.fBottom - pFontDef->oFontBox.fTop );
//
//    UtilsMemSet( anTempMap, 0, sizeof(unsigned short) * 65536);
//
//    if ( OK != nRet )
//        return NULL;
//
//    for ( unsigned int nI = 0; nI < 256; nI++) 
//	{
//        for ( unsigned int nJ = 0; nJ < 256; nJ++) 
//		{
//			unsigned short nCid = pEncoderAttr->anCIDMap[nI][nJ];
//            if ( 0 != nCid ) 
//			{
//				unsigned short nUnicode = pEncoderAttr->anUnicodeMap[nI][nJ];
//                unsigned short nGid = TTFontDefGetGlyphID( pFontDef, nUnicode );
//                anTempMap[nCid] = nGid;
//                if ( nMax < nCid )
//                    nMax = nCid;
//            }
//        }
//    }
//
//    if ( nMax > 0 ) 
//	{
//		short nDW = pFontDef->nMissingWidth;
//        unsigned short *pnTempMap = anTempMap;
//        Array oTempArray = NULL;
//
//        // Добавим элемент  'W'.
//		oArray = ArrayNew( pFont->oMMgr );
//        if ( !oArray )
//            return NULL;
//
//        if ( OK != DictAdd( pFont, "W", oArray ) )
//            return NULL;
//
//        for ( unsigned int nIndex = 0; nIndex < nMax; nIndex++, pnTempMap++) 
//		{
//            int nWidth = TTFontDefGetGidWidth( pFontDef, *pnTempMap);
//
//            if ( nWidth != nDW ) 
//			{
//                if ( !oTempArray ) 
//				{
//                    if ( OK != ( nRet = ArrayAddNumber( oArray, nIndex ) ) )
//                        return NULL;
//
//					oTempArray = ArrayNew( pFont->oMMgr );
//                    if ( !oTempArray )
//                        return NULL;
//
//                    if ( OK != ( nRet = ArrayAdd( oArray, oTempArray ) ) )
//                        return NULL;
//                }
//
//                if ( OK != ( nRet = ArrayAddNumber( oTempArray, nWidth ) ) )
//                    return NULL;
//            } 
//			else
//				oTempArray = NULL;
//        }
//
//        // Создаем данные "CIDToGIDMap".
//		if ( pFontDefAttr->bEmbedding ) 
//		{
//			pAttr->pMapStream = DictStreamNew( pFont->oMMgr, pXref );
//			if ( !pAttr->pMapStream )
//                return NULL;
//
//			if ( OK != ( nRet = DictAdd( pFont, "CIDToGIDMap", pAttr->pMapStream ) ) )
//                return NULL;
//
//            for ( unsigned int nIndex = 0; nIndex < nMax; nIndex++) 
//			{
//                BYTE anUnicodeTemp[2];
//                unsigned short nGid = anTempMap[nIndex];
//
//                anUnicodeTemp[0] = nGid >> 8;
//                anUnicodeTemp[1] = nGid;
//
//                UtilsMemCpy( (BYTE *)(anTempMap + nIndex), anUnicodeTemp, 2 );
//            }
//
//			if ( OK != ( nRet = StreamWrite( pAttr->pMapStream->pStream, (BYTE *)anTempMap, nMax * 2 ) ) )
//                return NULL;
//        }
//    } 
//	else 
//	{
//		SetError( pFont->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_DATA, 0);
//        return 0;
//    }
//
//	Dict pCIDSystemInfo = DictNew( pParent->oMMgr );
//    if ( !pCIDSystemInfo )
//        return NULL;
//
//    if ( OK != DictAdd( pFont, "CIDSystemInfo", pCIDSystemInfo ) )
//        return NULL;
//
//	nRet += DictAdd( pCIDSystemInfo, "Registry", StringNew( pParent->oMMgr, pEncoderAttr->sRegistry, NULL ) );
//	nRet += DictAdd( pCIDSystemInfo, "Ordering", StringNew( pParent->oMMgr, pEncoderAttr->sOrdering, NULL ) );
//	nRet += DictAddNumber( pCIDSystemInfo, "Supplement", pEncoderAttr->nSuppliment );
//
//    if ( OK != nRet )
//        return NULL;
//
//    return pFont;
//}
//
//
//static TextWidth     CIDTextWidth                 (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
//{
//    TextWidth oTextWidth = {0, 0, 0, 0};
//	return oTextWidth;
//
//    FontAttr pAttr = (FontAttr)pFont->pAttr;
//    ParseTextRec oParseState;
//	EncoderRecPtr pEncoder = pAttr->pEncoder;
//    unsigned int nIndex = 0;
//    int nDW2 = 0;
//    BYTE nChar = 0;
//
//	if ( FontDefCID == pAttr->pFontDef->eType ) 
//	{
//		CIDFontDefAttr pCIDFontDefAttr = (CIDFontDefAttr)pAttr->pFontDef->pAttr;
//		nDW2 = pCIDFontDefAttr->anDW2[1];
//    } 
//	else 
//	{
//		nDW2 = pAttr->pFontDef->oFontBox.fBottom - pAttr->pFontDef->oFontBox.fTop;
//    }
//
//    EncoderSetParseText( pEncoder, &oParseState, pText, nLen);
//
//    while ( nIndex < nLen ) 
//	{
//        ByteType eBType = CMapEncoderByteType( pEncoder, &oParseState );
//        unsigned short nCid = 0;
//        unsigned short nUnicode = 0;
//        unsigned int nWidth = 0;
//
//        nChar = *pText++;
//        unsigned short nCode = nChar;
//
//        if ( ByteTypeLead == eBType ) 
//		{
//            nCode <<= 8;
//            nCode += *pText;
//        }
//
//        if ( ByteTypeTrial != eBType ) 
//		{
//			if ( WModeHorizontal == pAttr->eWritingMode ) 
//			{
//				if ( FontDefCID == pAttr->pFontDef->eType ) 
//				{
//                    // cid-based font
//					nCid   = CMapEncoderToCID( pEncoder, nCode );
//					nWidth = CIDFontDefGetCIDWidth( pAttr->pFontDef, nCid );
//                } 
//				else 
//				{
//                    // unicode-based font
//					nUnicode = CMapEncoderToUnicode( pEncoder, nCode );
//					nWidth   = TTFontDefGetCharWidth( pAttr->pFontDef, nUnicode );
//                }
//            } 
//			else 
//			{
//				nWidth = -nDW2;
//            }
//
//            oTextWidth.nNumchars++;
//        }
//
//		if ( IS_WHITE_SPACE(nCode) ) 
//		{
//            oTextWidth.nNumwords++;
//            oTextWidth.nNumspace++;
//        }
//
//        oTextWidth.nWidth += nWidth;
//        nIndex++;
//    }
//
//    if ( IS_WHITE_SPACE(nChar) )
//        ; // ничего не делаем
//    else
//        oTextWidth.nNumwords++;
//
//    return oTextWidth;
//}
//static unsigned int  CIDMeasureText               (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth)
//{
//    float fTempWidth = 0;
//    unsigned int nTempLen = 0;
//    FontAttr pAttr = (FontAttr)pFont->pAttr;
//    ByteType eLastBType = ByteTypeTrial;
//    EncoderRecPtr pEncoder = pAttr->pEncoder;
//    ParseTextRec  oParseState;
//    int nDW2 = 0;
//
//	return 0;
//
//    if ( FontDefCID == pAttr->pFontDef->eType ) 
//	{
//        CIDFontDefAttr pCIDFontDefAttr = (CIDFontDefAttr)pAttr->pFontDef->pAttr;
//        nDW2 = pCIDFontDefAttr->anDW2[1];
//    } 
//	else 
//	{
//        nDW2 = pAttr->pFontDef->oFontBox.fBottom - pAttr->pFontDef->oFontBox.fTop;
//    }
//
//    EncoderSetParseText( pEncoder, &oParseState, pText, nLen);
//
//    for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
//	{
//        BYTE nByte  = *pText++;
//        BYTE nByte2 = *pText;  // следующий байт
//        ByteType eBType = EncoderByteType( pEncoder, &oParseState );
//        unsigned short nUnicode = 0;
//        unsigned short nCode = nByte;
//        unsigned short nTempWidth = 0;
//
//        if ( ByteTypeLead == eBType ) 
//		{
//            nCode <<= 8;
//            nCode += nByte2;
//        }
//
//        if ( !bWordWrap ) 
//		{
//            if ( IS_WHITE_SPACE(nByte) ) 
//			{
//                nTempLen = nIndex + 1;
//                if ( pfRealWidth )
//                    *pfRealWidth = fTempWidth;
//            } 
//			else if ( ByteTypeSingle == eBType || ByteTypeLead == eBType ) 
//			{
//                nTempLen = nIndex;
//                if ( pfRealWidth )
//                    *pfRealWidth = fTempWidth;
//            }
//        } 
//		else 
//		{
//            if ( IS_WHITE_SPACE(nByte) ) 
//			{
//                nTempLen = nIndex + 1;
//                if ( pfRealWidth )
//                    *pfRealWidth = fTempWidth;
//            } 
//			else if ( ByteTypeTrial == eLastBType || ( ByteTypeLead == eBType && ByteTypeSingle == eLastBType ) )  
//			{
//                if ( !EncoderCheckJWWLineHead( pEncoder, nCode ) ) 
//				{
//                    nTempLen = nIndex;
//					if ( pfRealWidth )
//						*pfRealWidth = fTempWidth;
//                }
//            }
//        }
//
//        if ( IS_WHITE_SPACE(nByte) ) 
//		{
//            fTempWidth += fWordSpace;
//        }
//
//        if ( ByteTypeTrial != eBType ) 
//		{
//            if ( WModeHorizontal == pAttr->eWritingMode ) 
//			{
//                if ( FontDefCID == pAttr->pFontDef->eType ) 
//				{
//                    // cid-based font
//                    unsigned short nCid = CMapEncoderToCID( pEncoder, nCode );
//                    nTempWidth = CIDFontDefGetCIDWidth( pAttr->pFontDef, nCid );
//                } 
//				else 
//				{
//                    // unicode-based font
//                    nUnicode = CMapEncoderToUnicode( pEncoder, nCode );
//                    nTempWidth = TTFontDefGetCharWidth( pAttr->pFontDef, nUnicode );
//                }
//            } 
//			else 
//			{
//                nTempWidth = -nDW2;
//            }
//
//            if ( nIndex > 0 )
//                fTempWidth += fCharSpace;
//        }
//
//        fTempWidth += (double)nTempWidth * fFontSize / 1000;
//
//        if ( fTempWidth > fWidth || 0x0A == nByte )
//            return nTempLen;
//
//        if ( IS_WHITE_SPACE(nByte) )
//            eLastBType = ByteTypeTrial;
//        else
//            eLastBType = eBType;
//    }
//
//    if ( pfRealWidth )
//        *pfRealWidth = fTempWidth;
//
//    return nLen;
//}
//static TextWidth     CIDTextWidth2                (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID)
//{
//    TextWidth oTextWidth = {0, 0, 0, 0};
//    FontAttr pAttr = (FontAttr)pFont->pAttr;
//
//	if ( 0 == nLenCID || NULL == pCIDs )
//		return oTextWidth;
//
//    ParseTextRec oParseState;
//	EncoderRecPtr pEncoder = pAttr->pEncoder;
//	CIDFontDefAttr pCIDFontDefAttr = (CIDFontDefAttr)pAttr->pFontDef->pAttr;
//	int nDW2 = pCIDFontDefAttr->anDW2[1];
//	int nIndex = 0;
//
//    while ( nIndex < nLenCID ) 
//	{
//		int nWidth = 0;
//		if ( WModeHorizontal == pAttr->eWritingMode ) 
//		{
//			nWidth = CIDFontDefGetCIDWidth( pAttr->pFontDef, pCIDs[nIndex] );
//		} 
//		else 
//		{
//			nWidth = -nDW2;
//		}
//
//		oTextWidth.nNumchars++;
//
//		if ( IS_WHITE_SPACE(pCIDs[nIndex]) ) 
//		{
//            oTextWidth.nNumwords++;
//            oTextWidth.nNumspace++;
//        }
//
//        oTextWidth.nWidth += nWidth;
//        nIndex++;
//    }
//
//    return oTextWidth;
//}
//static unsigned int  CIDMeasureText2              (FontDict pFont, const BYTE *pText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, float fFontSize, float fCharSpace, float fWordSpace, BOOL bWordWrap, float *pfRealWidth)
//{
//	// TO DO : Релизовать данную функцию
//	return 0;
//
//	//float fTempWidth = 0;
//	//unsigned int nTempLen = 0;
//	//FontAttr pAttr = (FontAttr)pFont->pAttr;
//	//ByteType eLastBType = ByteTypeTrial;
//	//EncoderRecPtr pEncoder = pAttr->pEncoder;
//	//ParseTextRec  oParseState;
//	//int nDW2 = 0;
//
//
//	//if ( FontDefCID == pAttr->pFontDef->eType ) 
//	//{
//	//	CIDFontDefAttr pCIDFontDefAttr = (CIDFontDefAttr)pAttr->pFontDef->pAttr;
//	//	nDW2 = pCIDFontDefAttr->anDW2[1];
//	//} 
//	//else 
//	//{
//	//	nDW2 = pAttr->pFontDef->oFontBox.fBottom - pAttr->pFontDef->oFontBox.fTop;
//	//}
//
//	//EncoderSetParseText( pEncoder, &oParseState, pText, nLen);
//
//	//for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
//	//{
//	//	BYTE nByte  = *pText++;
//	//	BYTE nByte2 = *pText;  // следующий байт
//	//	ByteType eBType = EncoderByteType( pEncoder, &oParseState );
//	//	unsigned short nUnicode = 0;
//	//	unsigned short nCode = nByte;
//	//	unsigned short nTempWidth = 0;
//
//	//	if ( ByteTypeLead == eBType ) 
//	//	{
//	//		nCode <<= 8;
//	//		nCode += nByte2;
//	//	}
//
//	//	if ( !bWordWrap ) 
//	//	{
//	//		if ( IS_WHITE_SPACE(nByte) ) 
//	//		{
//	//			nTempLen = nIndex + 1;
//	//			if ( pfRealWidth )
//	//				*pfRealWidth = fTempWidth;
//	//		} 
//	//		else if ( ByteTypeSingle == eBType || ByteTypeLead == eBType ) 
//	//		{
//	//			nTempLen = nIndex;
//	//			if ( pfRealWidth )
//	//				*pfRealWidth = fTempWidth;
//	//		}
//	//	} 
//	//	else 
//	//	{
//	//		if ( IS_WHITE_SPACE(nByte) ) 
//	//		{
//	//			nTempLen = nIndex + 1;
//	//			if ( pfRealWidth )
//	//				*pfRealWidth = fTempWidth;
//	//		} 
//	//		else if ( ByteTypeTrial == eLastBType || ( ByteTypeLead == eBType && ByteTypeSingle == eLastBType ) )  
//	//		{
//	//			if ( !EncoderCheckJWWLineHead( pEncoder, nCode ) ) 
//	//			{
//	//				nTempLen = nIndex;
//	//				if ( pfRealWidth )
//	//					*pfRealWidth = fTempWidth;
//	//			}
//	//		}
//	//	}
//
//	//	if ( IS_WHITE_SPACE(nByte) ) 
//	//	{
//	//		fTempWidth += fWordSpace;
//	//	}
//
//	//	if ( ByteTypeTrial != eBType ) 
//	//	{
//	//		if ( WModeHorizontal == pAttr->eWritingMode ) 
//	//		{
//	//			if ( FontDefCID == pAttr->pFontDef->eType ) 
//	//			{
//	//				// cid-based font
//	//				unsigned short nCid = CMapEncoderToCID( pEncoder, nCode );
//	//				nTempWidth = CIDFontDefGetCIDWidth( pAttr->pFontDef, nCid );
//	//			} 
//	//			else 
//	//			{
//	//				// unicode-based font
//	//				nUnicode = CMapEncoderToUnicode( pEncoder, nCode );
//	//				nTempWidth = TTFontDefGetCharWidth( pAttr->pFontDef, nUnicode );
//	//			}
//	//		} 
//	//		else 
//	//		{
//	//			nTempWidth = -nDW2;
//	//		}
//
//	//		if ( nIndex > 0 )
//	//			fTempWidth += fCharSpace;
//	//	}
//
//	//	fTempWidth += (double)nTempWidth * fFontSize / 1000;
//
//	//	if ( fTempWidth > fWidth || 0x0A == nByte )
//	//		return nTempLen;
//
//	//	if ( IS_WHITE_SPACE(nByte) )
//	//		eLastBType = ByteTypeTrial;
//	//	else
//	//		eLastBType = eBType;
//	//}
//
//	//if ( pfRealWidth )
//	//	*pfRealWidth = fTempWidth;
//
//	//return nLen;
//}
//static char*         UINT16ToHex                  (char *sStr, unsigned short nValue, char *pEndPointer)
//{
//    BYTE anTemp[2];
//    char nChar = 0;
//
//    if (pEndPointer - sStr < 7)
//        return sStr;
//
//    UtilsMemCpy( anTemp, (BYTE *)&nValue, 2);
//    unsigned short nTempValue = (unsigned short)((unsigned short)anTemp[0] << 8 | (unsigned short)anTemp[1]);
//
//    UtilsMemCpy( anTemp, (BYTE *)&nTempValue, 2);
//
//    *sStr++ = '<';
//
//    if ( 0 != anTemp[0] ) 
//	{
//        nChar = anTemp[0] >> 4;
//        if ( nChar <= 9 )
//            nChar += 0x30;
//        else
//            nChar += 0x41 - 10;
//        *sStr++ = nChar;
//
//        nChar = anTemp[0] & 0x0f;
//        if ( nChar <= 9 )
//            nChar += 0x30;
//        else
//            nChar += 0x41 - 10;
//        *sStr++ = nChar;
//    }
//
//    nChar = anTemp[1] >> 4;
//    if ( nChar <= 9 )
//        nChar += 0x30;
//    else
//        nChar += 0x41 - 10;
//    *sStr++ = nChar;
//
//    nChar = anTemp[1] & 0x0f;
//    if ( nChar <= 9 )
//        nChar += 0x30;
//    else
//        nChar += 0x41 - 10;
//    *sStr++ = nChar;
//
//    *sStr++ = '>';
//    *sStr = 0;
//
//    return sStr;
//}
//static Dict          CreateCMap                   (EncoderRecPtr pEncoder, Xref pXref)
//{
//    unsigned long nRet = OK;
//	Dict pCMap = DictStreamNew( pEncoder->oMMgr, pXref );
//    CMapEncoderAttr pAttr = (CMapEncoderAttr)pEncoder->pAttr;
//    char sBuffer[TEMP_BUFFER_SIZE];
//    char *pBufPointer;
//    char *pEndBufPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
//
//	if ( !pCMap )
//        return NULL;
//
//    nRet += DictAddName( pCMap, "Type", "CMap");
//    nRet += DictAddName( pCMap, "CMapName", pEncoder->sName );
//
//	Dict pSysInfo = DictNew( pEncoder->oMMgr );
//    if ( !pSysInfo )
//        return NULL;
//
//    if ( OK != DictAdd( pCMap, "CIDSystemInfo", pSysInfo ) )
//        return NULL;
//
//	nRet += DictAdd( pSysInfo, "Registry", StringNew( pEncoder->oMMgr, pAttr->sRegistry, NULL ) );
//	nRet += DictAdd( pSysInfo, "Ordering", StringNew( pEncoder->oMMgr, pAttr->sOrdering, NULL ) );
//	nRet += DictAddNumber( pSysInfo, "Supplement", pAttr->nSuppliment );
//    nRet += DictAddNumber( pCMap, "WMode", (unsigned int)pAttr->eWritingMode );
//
//    // создаем CMap данные из закодированных данных
//	nRet += StreamWriteStr( pCMap->pStream, "%!PS-Adobe-3.0 Resource-CMap\r\n" ); 
//    nRet += StreamWriteStr( pCMap->pStream, "%%DocumentNeededResources: ProcSet (CIDInit)\r\n" );
//    nRet += StreamWriteStr( pCMap->pStream, "%%IncludeResource: ProcSet (CIDInit)\r\n" );
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "%%BeginResource: CMap (", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, pEncoder->sName, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, ")\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "%%Title: (", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, pEncoder->sName, pEndBufPointer);
//    *pBufPointer++ = ' ';
//	pBufPointer = (char*)UtilsStrCpy( pBufPointer, pAttr->sRegistry, pEndBufPointer);
//    *pBufPointer++ = ' ';
//	pBufPointer = (char*)UtilsStrCpy( pBufPointer, pAttr->sOrdering, pEndBufPointer);
//    *pBufPointer++ = ' ';
//	pBufPointer = UtilsIToA( pBufPointer, pAttr->nSuppliment, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, ")\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//    nRet += StreamWriteStr( pCMap->pStream, "%%Version: 1.0\r\n");
//    nRet += StreamWriteStr( pCMap->pStream, "%%EndComments\r\n");
//    nRet += StreamWriteStr( pCMap->pStream, "/CIDInit /ProcSet findresource begin\r\n\r\n");
//
//    // Спецификации файлов Adobe CMap и CIDFont рекомендуют выделить мето для
//    // 5 дополнительных элементов в данной директории.
//
//	nRet += StreamWriteStr( pCMap->pStream, "12 dict begin\r\n\r\n");
//    nRet += StreamWriteStr( pCMap->pStream, "begincmap\r\n\r\n");
//    nRet += StreamWriteStr( pCMap->pStream, "/CIDSystemInfo 3 dict dup begin\r\n");
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "  /Registry (", pEndBufPointer);
//	pBufPointer = (char*)UtilsStrCpy( pBufPointer, pAttr->sRegistry, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, ") def\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "  /Ordering (", pEndBufPointer);
//	pBufPointer = (char*)UtilsStrCpy( pBufPointer, pAttr->sOrdering, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, ") def\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "  /Supplement ", pEndBufPointer);
//	pBufPointer = UtilsIToA( pBufPointer, pAttr->nSuppliment, pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, " def\r\n", pEndBufPointer);
//    UtilsStrCpy( pBufPointer, "end def\r\n\r\n", pEndBufPointer);
//	nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "/CMapName /", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, pEncoder->sName, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, " def\r\n", pEndBufPointer);
//	nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    nRet += StreamWriteStr( pCMap->pStream, "/CMapVersion 1.0 def\r\n");
//    nRet += StreamWriteStr( pCMap->pStream, "/CMapType 1 def\r\n\r\n");
//
//	if ( pAttr->nUidOffset >= 0 ) 
//	{
//        pBufPointer = (char*)UtilsStrCpy( sBuffer, "/UIDOffset ", pEndBufPointer);
//		pBufPointer = UtilsIToA( pBufPointer, pAttr->nUidOffset, pEndBufPointer);
//        UtilsStrCpy( pBufPointer, " def\r\n\r\n", pEndBufPointer);
//        nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//    }
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "/XUID [", pEndBufPointer);
//	pBufPointer = UtilsIToA( pBufPointer, pAttr->anXUid[0], pEndBufPointer);
//    *pBufPointer++ = ' ';
//    pBufPointer = UtilsIToA( pBufPointer, pAttr->anXUid[1], pEndBufPointer);
//    *pBufPointer++ = ' ';
//    pBufPointer = UtilsIToA( pBufPointer, pAttr->anXUid[2], pEndBufPointer);
//    UtilsStrCpy( pBufPointer, "] def\r\n\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    pBufPointer = (char*)UtilsStrCpy( sBuffer, "/WMode ", pEndBufPointer);
//    pBufPointer = UtilsIToA( pBufPointer, (unsigned int)pAttr->eWritingMode, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, " def\r\n\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    // add code-space-range
//	pBufPointer = UtilsIToA( sBuffer, pAttr->pCodeSpaceRange->nCount, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, " begincodespacerange\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//	for ( unsigned int nIndex = 0; nIndex < pAttr->pCodeSpaceRange->nCount; nIndex++) 
//	{
//		CIDRangeRec *pRange = (CIDRangeRec*)ListItemAt( pAttr->pCodeSpaceRange, nIndex);
//
//		pBufPointer = UINT16ToHex( sBuffer, pRange->nFrom, pEndBufPointer);
//        *pBufPointer++ = ' ';
//		pBufPointer = UINT16ToHex( pBufPointer, pRange->nTo, pEndBufPointer);
//        UtilsStrCpy( pBufPointer, "\r\n", pEndBufPointer);
//
//        nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//        if ( OK != nRet )
//            return NULL;
//    }
//
//    UtilsStrCpy( sBuffer, "endcodespacerange\r\n\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//    if ( OK != nRet )
//        return NULL;
//
//    // add not-def-range
//	pBufPointer = UtilsIToA( sBuffer, pAttr->pNotDefRange->nCount, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, " beginnotdefrange\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//	for ( unsigned int nIndex = 0; nIndex < pAttr->pNotDefRange->nCount; nIndex++) 
//	{
//		CIDRangeRec *pRange = (CIDRangeRec*)ListItemAt( pAttr->pNotDefRange, nIndex );
//
//		pBufPointer = UINT16ToHex( sBuffer, pRange->nFrom, pEndBufPointer);
//        *pBufPointer++ = ' ';
//		pBufPointer = UINT16ToHex( pBufPointer, pRange->nTo, pEndBufPointer);
//        *pBufPointer++ = ' ';
//		pBufPointer = UtilsIToA( pBufPointer, pRange->nCID, pEndBufPointer);
//        UtilsStrCpy( pBufPointer, "\r\n", pEndBufPointer);
//
//        nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//        if ( OK != nRet )
//            return NULL;
//    }
//
//    UtilsStrCpy( sBuffer, "endnotdefrange\r\n\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//    if ( OK != nRet )
//        return NULL;
//
//    // add cid-range
//	unsigned int nPhase = pAttr->pCMAPRange->nCount / 100;
//	unsigned int nOdd   = pAttr->pCMAPRange->nCount % 100;
//    if ( nPhase > 0 )
//        pBufPointer = UtilsIToA( sBuffer, 100, pEndBufPointer);
//    else
//        pBufPointer = UtilsIToA( sBuffer, nOdd, pEndBufPointer);
//    UtilsStrCpy( pBufPointer, " begincidrange\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//	for ( unsigned int nIndex = 0; nIndex < pAttr->pCMAPRange->nCount; nIndex++) 
//	{
//		CIDRangeRec *pRange = (CIDRangeRec*)ListItemAt( pAttr->pCMAPRange, nIndex );
//
//		pBufPointer = UINT16ToHex( sBuffer, pRange->nFrom, pEndBufPointer);
//        *pBufPointer++ = ' ';
//		pBufPointer = UINT16ToHex( pBufPointer, pRange->nTo, pEndBufPointer);
//        *pBufPointer++ = ' ';
//		pBufPointer = UtilsIToA( pBufPointer, pRange->nCID, pEndBufPointer);
//        UtilsStrCpy( pBufPointer, "\r\n", pEndBufPointer);
//
//        nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//
//        if ( 0 == (nIndex + 1) %100 ) 
//		{
//            nPhase--;
//            pBufPointer = (char*)UtilsStrCpy( sBuffer, "endcidrange\r\n\r\n", pEndBufPointer);
//
//            if ( nPhase > 0 )
//                pBufPointer = UtilsIToA( pBufPointer, 100, pEndBufPointer);
//            else
//                pBufPointer = UtilsIToA( pBufPointer, nOdd, pEndBufPointer);
//
//            UtilsStrCpy( pBufPointer, " begincidrange\r\n", pEndBufPointer);
//
//            nRet += StreamWriteStr( pCMap->pStream, sBuffer );
//        }
//
//        if ( OK != nRet )
//            return NULL;
//    }
//
//    if ( nOdd > 0 )
//        pBufPointer = (char*)UtilsStrCpy( sBuffer, "endcidrange\r\n", pEndBufPointer);
//
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, "endcmap\r\n", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, "CMapName currentdict /CMap defineresource pop\r\n", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, "end\r\n", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, "end\r\n\r\n", pEndBufPointer);
//    pBufPointer = (char*)UtilsStrCpy( pBufPointer, "%%EndResource\r\n", pEndBufPointer);
//    UtilsStrCpy( pBufPointer, "%%EOF\r\n", pEndBufPointer);
//    nRet += StreamWriteStr( pCMap->pStream, sBuffer);
//
//    if ( OK != nRet )
//        return NULL;
//
//    return pCMap;
//}
//static unsigned long CIDBeforeWrite               (Dict pObj)
//{
//	//unsigned long nRet = OK;
//
//	//if ( ( nRet = CreateDescriptor( pObj ) ) != OK )
//	//	return nRet;
//
//	//if ( ( nRet = CreateToUnicode( pObj ) ) != OK )
//	//	return nRet;
//
//	return OK;
//}
//static unsigned long CIDOnWrite                   (Dict pObj, StreamRecPtr pStream)
//{
//	return OK;
// //   FontAttr pAttr = (FontAttr)pObj->pAttr;
//
//	//if ( true == pAttr->oExternWidths.bUse )
//	//{
//	//	unsigned long nRet = OK;
//
//	//	char sBuffer[TEMP_BUFFER_SIZE];
//	//	char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
//
//	//	char *pBufPointer;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pAttr->oExternWidths.nFirstChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pAttr->oExternWidths.nLastChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//
//	//	char buf[128];
//	//	char *pbuf = buf;
//	//	char *eptr = buf + 127;
//
//	//	if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
//	//		return nRet;
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
//	//		return nRet;
//
//	//	for ( unsigned int nIndex = 0; nIndex <= pAttr->oExternWidths.nLastChar - pAttr->oExternWidths.nFirstChar; nIndex++) 
//	//	{
//
//	//		pbuf = UtilsIToA ( pbuf, pAttr->oExternWidths.anWidths[nIndex], eptr);
//	//		*pbuf++ = ' ';
//
//	//		if ( 0 == (nIndex + 1) % 16 ) 
//	//		{
//	//			UtilsStrCpy(pbuf, "\012", eptr );
//	//			if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//				return nRet;
//	//			pbuf = buf;
//	//		}
//
//	//	}
//
//	//	UtilsStrCpy( pbuf, "]\012", eptr );
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//		return nRet;
//
//	//	if ( pAttr->pEncoder )
//	//		nRet += pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
//	//	return nRet;
//	//}
//	//if ( (NULL == pAttr->pToUnicode) || (ToUnicodeBuilt != pAttr->pToUnicode->eType) )
//	//{
//	//	BasicEncoderAttr pEncoderAttr = (BasicEncoderAttr)pAttr->pEncoder->pAttr;
//
//	//	unsigned long nRet = OK;
//
//	//	char sBuffer[TEMP_BUFFER_SIZE];
//	//	char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
//
//	//	char *pBufPointer;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nFirstChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pEncoderAttr->nLastChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//	//	char buf[128];
//	//	char *pbuf = buf;
//	//	char *eptr = buf + 127;
//
//	//	if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
//	//		return nRet;
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
//	//		return nRet;
//
//	//	for ( unsigned int nIndex = pEncoderAttr->nFirstChar; nIndex <= pEncoderAttr->nLastChar; nIndex++) 
//	//	{
//
//	//		pbuf = UtilsIToA ( pbuf, pAttr->anWidths[nIndex], eptr);
//	//		*pbuf++ = ' ';
//
//	//		if ( 0 == (nIndex + 1) % 16 ) 
//	//		{
//	//			UtilsStrCpy(pbuf, "\012", eptr );
//	//			if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//				return nRet;
//	//			pbuf = buf;
//	//		}
//
//	//	}
//
//	//	UtilsStrCpy( pbuf, "]\012", eptr );
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//		return nRet;
//
//	//	return pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
//	//}
//	//else
//	//{
//	//	unsigned long nRet = OK;
//
//	//	// Значит кодировка у нас задана в ToUnicode
//	//	FontDef pDef = pAttr->pFontDef;
//	//	ToUnicodeBuiltAttr pToUnicodeAttr = (ToUnicodeBuiltAttr)pAttr->pToUnicode->pAttr;
//
//
//	//	char sBuffer[TEMP_BUFFER_SIZE];
//	//	char *pEndPointer = sBuffer + TEMP_BUFFER_SIZE - 1;
//
//	//	char *pBufPointer;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/FirstChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pToUnicodeAttr->nFirstChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//	//	pBufPointer = (char*)UtilsStrCpy( sBuffer, "/LastChar ", pEndPointer);
//	//	pBufPointer = UtilsIToA( pBufPointer, pToUnicodeAttr->nLastChar, pEndPointer);
//	//	UtilsStrCpy( pBufPointer, "\012", pEndPointer);
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, sBuffer ) ) )
//	//		return nRet;
//
//	//	char buf[128];
//	//	char *pbuf = buf;
//	//	char *eptr = buf + 127;
//
//	//	if ( OK != ( nRet = StreamWriteEscapeName( pStream, "Widths" ) ) )
//	//		return nRet;
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, " [\012" ) ) )
//	//		return nRet;
//
//	//	int nCodesCount;
//	//	for ( nCodesCount = 0; nCodesCount < 256 && pToUnicodeAttr->anUnicodeMap[nCodesCount][0] != 0xffff; nCodesCount++ );
//
//	//	for ( unsigned int nIndex = 0; nIndex < nCodesCount; nIndex++ ) 
//	//	{
//	//		unsigned short unUnicode = pAttr->pToUnicode->pToUnicodeFn( pAttr->pToUnicode, nIndex );
//	//		short nCharWidth = 0;
//
//	//		nCharWidth = TTFontDefGetCharWidth( pDef, unUnicode );
//
//	//		pbuf = UtilsIToA ( pbuf, nCharWidth, eptr);
//	//		*pbuf++ = ' ';
//
//	//		if ( 0 == (nIndex + 1) % 16 ) 
//	//		{
//	//			UtilsStrCpy(pbuf, "\012", eptr );
//	//			if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//				return nRet;
//	//			pbuf = buf;
//	//		}
//
//	//	}
//
//	//	UtilsStrCpy( pbuf, "]\012", eptr );
//
//	//	if ( OK != ( nRet = StreamWriteStr( pStream, buf ) ) )
//	//		return nRet;
//
//	//	if ( NULL != pAttr->pEncoder )
//	//		pAttr->pEncoder->pWriteFn( pAttr->pEncoder, pStream );
//	//}
//	//return OK;
//}
//static void          CIDOnFree                    (Dict pObj)
//{
//	FontAttr pAttr = (FontAttr)pObj->pAttr;
//
//	if ( pAttr ) 
//	{
//		if ( pAttr->anWidths ) 
//			FreeMem( pObj->oMMgr, pAttr->anWidths );
//
//		if ( pAttr->oExternWidths.anWidths )
//			FreeMem( pObj->oMMgr, pAttr->oExternWidths.anWidths );
//
//		FreeMem( pObj->oMMgr, pAttr );
//	}
//}
//----- CIDTypeFont основные функции ----------------------------------------------------------------
FontDict Type0FontNew  (MMgr oMMgr, FontDef pFontDef, EncoderRecPtr pEncoder, Xref pXref, PToUnicode pToUnicode = NULL);
FontDict Type0FontNew2 (MMgr oMMgr, FontDef pFontDef, EncoderRecPtr pEncoder, Xref pXref, PToUnicode pToUnicode = NULL);
#endif /* _FONT_CID_H */
