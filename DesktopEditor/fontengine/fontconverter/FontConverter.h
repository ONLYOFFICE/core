/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
// FontConverter.h : Declaration of the CFontConverter

#pragma once
#include "resource.h"       // main symbols

#include FT_XFREE86_H
#include FT_TRUETYPE_TABLES_H
#include "FontFileTrueType.h"
#include "FontFileType1C.h"
#include "FontFileType1.h"
#include "Consts.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IFontConverter
[
	object,
	uuid("F92BB671-8B39-4D4F-AC43-D697A20CE0CA"),
	dual,	helpstring("IFontConverter Interface"),
	pointer_default(unique)
]
__interface IFontConverter : IDispatch
{

	[id(101)]	HRESULT ToOTF([in] BSTR bsInFontFile, [in] BSTR pbsFontFileOut, [in, satype("unsigned short")] SAFEARRAY *pUnicode, [in] BSTR bsName, [in] long nFlag );
	[id(102)]	HRESULT ToOTF2([in] BSTR bsInFontFile, [in, satype("unsigned short")] SAFEARRAY *pUnicode, [in] BSTR bsName, [in] long nFlag, [in] long lSrcFaceIndex, [out, satype("BYTE")] SAFEARRAY** ppFontData);

//----- For additional functions ----------------------------------------------------------------

	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);

};



// CFontConverter

[
	coclass,
	default(IFontConverter),
	threading(apartment),
	vi_progid("AVSFontConverter.FontConverter"),
	progid("AVSFontConverter.FontConverter.1"),
	version(1.0),
	uuid("3B756386-2427-461C-B979-3AB33F2A057E"),
	helpstring("FontConverter Class")
]
class ATL_NO_VTABLE CFontConverter : public IFontConverter
{
public:
	CFontConverter()
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(ToOTF)(BSTR bsFontIn, BSTR bsFontOut, SAFEARRAY *pUnicodeArray, BSTR bsName, long nFlag)
	{
		FT_Library pLibrary = NULL;
		if ( FT_Init_FreeType( &pLibrary ) ) 
			return S_FALSE;

		FT_Face pFace = NULL;

		// open file
		HANDLE hFile = CreateFile( (LPCWSTR)bsFontIn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			return NULL; // Cannot open file

		// map this file to memory - this way data is read faster from the file
		DWORD nFileSize = GetFileSize(hFile, NULL);
		HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
		if (NULL == hMapFile)
		{
			CloseHandle( hFile );
			return NULL; // Cannot create file mapping
		}

		void *pBaseAddress = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
		if ( !pBaseAddress )
		{
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return NULL;
		}

		FT_Open_Args oOpenArgs;
		oOpenArgs.flags = FT_OPEN_MEMORY;
		oOpenArgs.memory_base = (BYTE*)pBaseAddress;
		oOpenArgs.memory_size = (FT_Long)nFileSize;

		CFontFileTrueType* pTTF = CFontFileTrueType::LoadFromFile( bsFontIn );
		FT_Error oerrr;
		if ( oerrr = FT_Open_Face( pLibrary, &oOpenArgs, 0, &pFace ) )
		{
			FT_Done_FreeType( pLibrary );
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			UnmapViewOfFile( pBaseAddress );

			return S_FALSE;
		}

		CString sFontFormat( FT_Get_X11_Font_Format( pFace ) );

		// Check conversion flag and source font format
		bool bNeedConvert = false;

		if ( nFlag == c_lFromAll || ( _T("TrueType") == sFontFormat && nFlag & c_lFromTT ) || ( _T("CFF") == sFontFormat && nFlag & c_lFromCFF ) || ( _T("Type 1") == sFontFormat && nFlag & c_lFromT1 ) )
			bNeedConvert = true;

		bool bIsGids = (c_lFlagsGids & nFlag);

		if ( bNeedConvert )
		{
			if ( _T("CFF") == sFontFormat || _T("Type 1") == sFontFormat )
			{
				TCharBuffer oCFF;
				CFontFileType1C *pT1C = NULL;
				if ( _T("Type 1") == sFontFormat )
				{
					// First convert Type1 to CFF
					CFontFileType1* pT1 = CFontFileType1::LoadFromFile( bsFontIn );
					pT1->ToCFF( &CharBufferWrite, &oCFF );
					delete pT1;

					// Convert CFF to OpenTypeCFF
					pT1C = CFontFileType1C::LoadFromBuffer( oCFF.sBuffer, oCFF.nLen );
				}
				else
				{
					// FreeType returns CFF font type when the file is actually OpenType(CFF).
					// If that's the case, we don't need to do anything with the file.
					pT1C = CFontFileType1C::LoadFromFile( bsFontIn );
				}

				if ( pT1C )
				{
					FILE* pFile = _wfopen( bsFontOut, _T("wb+") );
					pT1C->ToOpenTypeCFF( &FileWrite, pFile, pFace );
					::fclose( pFile );
				}

				delete pT1C;
			}
			else if ( _T("TrueType") == sFontFormat && ( pUnicodeArray != NULL || bsName != NULL ) )
			{
				CFontFileTrueType* pTTF = CFontFileTrueType::LoadFromFile( bsFontIn );
				if ( pTTF )
				{
					USES_CONVERSION;
					char *sName = ( NULL == bsName ? NULL : W2A( bsName ) );
					unsigned char *pUseGlyfs = NULL;
					long lGlyfsCount = pFace->num_glyphs;

					if ( pUnicodeArray )
					{		
						// First compile a list of GIDs we need
						LONG lCount = pUnicodeArray->rgsabound[0].cElements;
						unsigned short* pUnicode = (unsigned short*)pUnicodeArray->pvData;
						unsigned short* pGIDs = new unsigned short[lCount];
						int nCMapIndex = 0;

						int nSymbolicIndex = GetSymbolicCmapIndex(pFace);

						if (!bIsGids)
						{
							for ( int nIndex = 0; nIndex < lCount; nIndex++ )
							{
								pGIDs[nIndex] = SetCMapForCharCode( pFace, pUnicode[nIndex], &nCMapIndex  );

								if ((pGIDs[nIndex] == 0) && (-1 != nSymbolicIndex) && (pUnicode[nIndex] < 0xF000))
								{
									pGIDs[nIndex] = SetCMapForCharCode( pFace, pUnicode[nIndex] + 0xF000, &nCMapIndex  );
								}
							}
						}
						else
						{
							memcpy(pGIDs, pUnicode, lCount * sizeof(unsigned short));
						}

						pUseGlyfs = new unsigned char[lGlyfsCount];
						::memset( pUseGlyfs, 0x00, lGlyfsCount * sizeof(unsigned char) );
						pUseGlyfs[0] = 1; // always write the zero glyph
						for ( int nGID = 1; nGID < lGlyfsCount; nGID++ )
						{
							if ( 1 != pUseGlyfs[nGID] )
							{
								bool bFound = false;
								for ( int nIndex = 0; nIndex < lCount; nIndex++ )
								{
									if ( nGID == pGIDs[nIndex] )
									{
										bFound = true;
										break;
									}
								}

								// If this glyph is composite (CompositeGlyf), we must account for all its child glyphs (subglyfs)
								if ( bFound && 0 == FT_Load_Glyph( pFace, nGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE ) )
								{
									for ( int nSubIndex = 0; nSubIndex < pFace->glyph->num_subglyphs; nSubIndex++ )
									{
										FT_Int       nSubGID;
										FT_UInt      unFlags;
										FT_Int       nArg1;
										FT_Int       nArg2;
										FT_Matrix    oMatrix;
										FT_Get_SubGlyph_Info( pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix );

										if ( nSubGID < lGlyfsCount )
											pUseGlyfs[nSubGID] = 1;
									}
								}

								if ( bFound )							
									pUseGlyfs[nGID] = 1;
							}
						}
					}

					FILE* pFile = _wfopen( bsFontOut, _T("wb+") );
					pTTF->WriteTTF( &FileWrite, pFile, sName, NULL, pUseGlyfs, lGlyfsCount );
					::fclose( pFile );
				}
				else
				{
					// error parse font
					// Just copy the file
					CopyFile( bsFontIn, bsFontOut, FALSE );
				}
			}
		}
		else
		{
			// Just copy the file
			CopyFile( bsFontIn, bsFontOut, FALSE );
		}

		FT_Done_Face( pFace );
		FT_Done_FreeType( pLibrary );

		CloseHandle( hMapFile );
		CloseHandle( hFile );
		UnmapViewOfFile( pBaseAddress );

		return S_OK;
	}

	STDMETHOD(ToOTF2)(BSTR bsFontIn, SAFEARRAY *pUnicodeArray, BSTR bsName, long nFlag, long lFaceIndex, SAFEARRAY** ppData)
	{
		// function is simply copied and slightly modified. all due to lack of time.

		FT_Library pLibrary = NULL;
		if ( FT_Init_FreeType( &pLibrary ) ) 
			return S_FALSE;

		FT_Face pFace = NULL;

		// open file
		HANDLE hFile = CreateFile( (LPCWSTR)bsFontIn, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile || NULL == ppData)
			return NULL; // Cannot open file

		// map this file to memory - this way data is read faster from the file
		DWORD nFileSize = GetFileSize(hFile, NULL);
		HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
		if (NULL == hMapFile)
		{
			CloseHandle( hFile );
			return NULL; // Cannot create file mapping
		}

		void *pBaseAddress = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
		if ( !pBaseAddress )
		{
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return NULL;
		}

		FT_Open_Args oOpenArgs;
		oOpenArgs.flags = FT_OPEN_MEMORY;
		oOpenArgs.memory_base = (BYTE*)pBaseAddress;
		oOpenArgs.memory_size = (FT_Long)nFileSize;

		CFontFileTrueType* pTTF = CFontFileTrueType::LoadFromFile( bsFontIn );
		FT_Error oerrr;
		if ( oerrr = FT_Open_Face( pLibrary, &oOpenArgs, lFaceIndex, &pFace ) )
		{
			FT_Done_FreeType( pLibrary );
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			UnmapViewOfFile( pBaseAddress );

			return S_FALSE;
		}

		CString sFontFormat( FT_Get_X11_Font_Format( pFace ) );

		// Check conversion flag and source font format
		bool bNeedConvert = false;

		if ( nFlag == c_lFromAll || ( _T("TrueType") == sFontFormat && nFlag & c_lFromTT ) || ( _T("CFF") == sFontFormat && nFlag & c_lFromCFF ) || ( _T("Type 1") == sFontFormat && nFlag & c_lFromT1 ) )
			bNeedConvert = true;

		bool bIsGids = (c_lFlagsGids & nFlag);

		TCharBuffer* pCharBuffer = NULL;

		if ( bNeedConvert )
		{
			pCharBuffer = new TCharBuffer(100000); // ~100Kb
			if ( _T("CFF") == sFontFormat || _T("Type 1") == sFontFormat )
			{
				TCharBuffer oCFF;
				CFontFileType1C *pT1C = NULL;
				if ( _T("Type 1") == sFontFormat )
				{
					// First convert Type1 to CFF
					CFontFileType1* pT1 = CFontFileType1::LoadFromFile( bsFontIn );
					pT1->ToCFF( &CharBufferWrite, &oCFF );
					delete pT1;

					// Convert CFF to OpenTypeCFF
					pT1C = CFontFileType1C::LoadFromBuffer( oCFF.sBuffer, oCFF.nLen );
				}
				else
				{
					// FreeType returns CFF font type when the file is actually OpenType(CFF).
					// If that's the case, we don't need to do anything with the file.
					pT1C = CFontFileType1C::LoadFromFile( bsFontIn );
				}

				if ( pT1C )
				{
					pT1C->ToOpenTypeCFF( &CharBufferWrite, pCharBuffer, pFace );					
				}

				delete pT1C;
			}
			else if ( _T("TrueType") == sFontFormat && ( pUnicodeArray != NULL || bsName != NULL ) )
			{
				CFontFileTrueType* pTTF = CFontFileTrueType::LoadFromFile( bsFontIn );
				if ( pTTF )
				{
					USES_CONVERSION;
					char *sName = ( NULL == bsName ? NULL : W2A( bsName ) );
					unsigned char *pUseGlyfs = NULL;
					long lGlyfsCount = pFace->num_glyphs;

					if ( pUnicodeArray )
					{		
						// First compile a list of GIDs we need
						LONG lCount = pUnicodeArray->rgsabound[0].cElements;
						unsigned short* pUnicode = (unsigned short*)pUnicodeArray->pvData;
						unsigned short* pGIDs = new unsigned short[lCount];
						int nCMapIndex = 0;

						int nSymbolicIndex = GetSymbolicCmapIndex(pFace);

						if (!bIsGids)
						{
							for ( int nIndex = 0; nIndex < lCount; nIndex++ )
							{
								pGIDs[nIndex] = SetCMapForCharCode( pFace, pUnicode[nIndex], &nCMapIndex  );

								if ((pGIDs[nIndex] == 0) && (-1 != nSymbolicIndex) && (pUnicode[nIndex] < 0xF000))
								{
									pGIDs[nIndex] = SetCMapForCharCode( pFace, pUnicode[nIndex] + 0xF000, &nCMapIndex  );
								}
							}
						}
						else
						{
							memcpy(pGIDs, pUnicode, lCount * sizeof(unsigned short));
						}
						
						pUseGlyfs = new unsigned char[lGlyfsCount];
						::memset( pUseGlyfs, 0x00, lGlyfsCount * sizeof(unsigned char) );
						pUseGlyfs[0] = 1; // always write the zero glyph
						for ( int nGID = 1; nGID < lGlyfsCount; nGID++ )
						{
							if ( 1 != pUseGlyfs[nGID] )
							{
								bool bFound = false;
								for ( int nIndex = 0; nIndex < lCount; nIndex++ )
								{
									if ( nGID == pGIDs[nIndex] )
									{
										bFound = true;
										break;
									}
								}

								// If this glyph is composite (CompositeGlyf), we must account for all its child glyphs (subglyfs)
								if ( bFound && 0 == FT_Load_Glyph( pFace, nGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE ) )
								{
									for ( int nSubIndex = 0; nSubIndex < pFace->glyph->num_subglyphs; nSubIndex++ )
									{
										FT_Int       nSubGID;
										FT_UInt      unFlags;
										FT_Int       nArg1;
										FT_Int       nArg2;
										FT_Matrix    oMatrix;
										FT_Get_SubGlyph_Info( pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix );

										if ( nSubGID < lGlyfsCount )
											pUseGlyfs[nSubGID] = 1;
									}
								}

								if ( bFound )							
									pUseGlyfs[nGID] = 1;
							}
						}
					}

					pTTF->WriteTTF( &CharBufferWrite, pCharBuffer, sName, NULL, pUseGlyfs, lGlyfsCount );					
				}
			}

			LONG nSizeFont = pCharBuffer->nLen;
			
			SAFEARRAYBOUND rgsab;
			rgsab.lLbound	= 0;
			rgsab.cElements	= nSizeFont;

			SAFEARRAY* pArray = SafeArrayCreate(VT_UI1, 1, &rgsab);
			memcpy(pArray->pvData, pCharBuffer->sBuffer, nSizeFont);
			
			RELEASEOBJECT(pCharBuffer);

			*ppData = pArray;
		}
		else
		{
			SAFEARRAYBOUND rgsab;
			rgsab.lLbound	= 0;
			rgsab.cElements	= nFileSize;

			SAFEARRAY* pArray = SafeArrayCreate(VT_UI1, 1, &rgsab);
			memcpy(pArray->pvData, pBaseAddress, nFileSize);

			*ppData = pArray;
		}

		FT_Done_Face( pFace );
		FT_Done_FreeType( pLibrary );

		CloseHandle( hMapFile );
		CloseHandle( hFile );
		UnmapViewOfFile( pBaseAddress );

		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR bsParamName, VARIANT vParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR bsParamName, VARIANT *pvParamValue)
	{
		return S_OK;
	}

};

