#pragma once
#include "stdafx.h"
#include "ASCFontManager.h"
#include "../Interfaces/ASCGraphicsRenderer.h"

void CASCFontManager::DumpToJSEditor(CString strDirectory, bool bIsUnionFamily)
{
	CreateDirectory(strDirectory, NULL);
	CString strFonts = _T("");

	int nCount = m_pFontEngine->GetInstalledFontsCount();

	// сначала строим массив всех файлов шрифтов
	CAtlMap<CString, LONG> mapFontFiles;
	CAtlMap<LONG, CString> mapFontFiles2;
	LONG lFontFiles = 0;
	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = m_pFontEngine->GetInstalledFont(i);

		CString strPath = (CString)pInfo->m_wsFontPath;
		CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.Lookup(strPath);

		if (NULL == pPair)
		{
			mapFontFiles.SetAt(strPath, lFontFiles);
			mapFontFiles2.SetAt(lFontFiles, strPath);
			++lFontFiles;
		}
	}
	// -----------------------------------------

	// теперь строим массив всех шрифтов по имени
	CAtlMap<CString, CFontInfoJS> mapFonts;
	CAtlArray<CString> arrFonts;

	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = m_pFontEngine->GetInstalledFont(i);
		CString strPath = (CString)pInfo->m_wsFontPath;
		CString strName = (CString)pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		CAtlMap<CString, LONG>::CPair* pPairFontFiles = mapFontFiles.Lookup(strPath);
		lFontIndex = pPairFontFiles->m_value;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

		CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(pInfo->m_wsFontName);
		if (NULL != pPair)
		{
			pPair->m_value.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexBI = lFontIndex;
				pPair->m_value.m_lFaceIndexBI = lFaceIndex;
			}
			else if (pInfo->m_bBold)
			{
				pPair->m_value.m_lIndexB = lFontIndex;
				pPair->m_value.m_lFaceIndexB = lFaceIndex;
			}
			else if (pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexI = lFontIndex;
				pPair->m_value.m_lFaceIndexI = lFaceIndex;
			}
			else
			{
				pPair->m_value.m_lIndexR = lFontIndex;
				pPair->m_value.m_lFaceIndexR = lFaceIndex;
			}
		}
		else
		{
			CFontInfoJS fontInfo;

			fontInfo.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
				fontInfo.m_lIndexBI = lFontIndex;
				fontInfo.m_lFaceIndexBI = lFaceIndex;
			}
			else if (pInfo->m_bBold)
			{
				fontInfo.m_lIndexB = lFontIndex;
				fontInfo.m_lFaceIndexB = lFaceIndex;
			}
			else if (pInfo->m_bItalic)
			{
				fontInfo.m_lIndexI = lFontIndex;
				fontInfo.m_lFaceIndexI = lFaceIndex;
			}
			else
			{
				fontInfo.m_lIndexR = lFontIndex;
				fontInfo.m_lFaceIndexR = lFaceIndex;
			}

			mapFonts.SetAt(fontInfo.m_sName, fontInfo);
			arrFonts.Add(fontInfo.m_sName);
		}
	}
	// -------------------------------------------

	// теперь сортируем шрифты по имени ----------
	size_t nCountFonts = arrFonts.GetCount();
	for (size_t i = 0; i < nCountFonts; ++i)
	{
		for (size_t j = i + 1; j < nCountFonts; ++j)
		{
			if (arrFonts[i] > arrFonts[j])
			{
				CString temp = arrFonts[i];
				arrFonts[i] = arrFonts[j];
				arrFonts[j] = temp;
			}
		}
	}
	// -------------------------------------------

	// создаем картинку для табнейлов
	double dW_mm = 80;
	LONG lH1_px = LONG(7 * 96 / 25.4);
	LONG lWidthPix = (LONG)(dW_mm * 96 / 25.4);
	LONG lHeightPix = (LONG)(nCountFonts * lH1_px);
	MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
	CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

	pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
	pFrame->put_Width( lWidthPix );
	pFrame->put_Height( lHeightPix );
	pFrame->put_AspectRatioX( lWidthPix );
	pFrame->put_AspectRatioY( lHeightPix );
	pFrame->put_Interlaced( VARIANT_FALSE );
	pFrame->put_Stride( 0, 4 * lWidthPix );
	pFrame->AllocateBuffer( -1 );

	BYTE* pBuffer = NULL;
	pFrame->get_Buffer(&pBuffer);
	memset(pBuffer, 0xFF, 4 * lWidthPix * lHeightPix);

	for (LONG i = 3; i < lWidthPix * lHeightPix * 4; i += 4)
	{
		pBuffer[i] = 0;
	}

	IASCGraphicsRenderer* pRenderer = NULL;
	CoCreateInstance(__uuidof(CASCGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IASCGraphicsRenderer), (void**)&pRenderer);
	//ставим FontManager

	IASCFontManager* man = NULL;
	CoCreateInstance(__uuidof(CASCFontManager), NULL, CLSCTX_INPROC, __uuidof(IASCFontManager), (void**)&man);
	man->Initialize(L"");
	man->SetDefaultFont( L"Arial" );
	
	IASCFontManager2* man2 = NULL;
	man->QueryInterface(__uuidof(IASCFontManager2), (void**)&man2);
	man2->UseDefaultFont(TRUE);
	RELEASEINTERFACE(man2);

	VARIANT vtVariant;
	vtVariant.vt = VT_UNKNOWN;
	vtVariant.punkVal = (IUnknown*)man;
	pRenderer->SetAdditionalParam( L"FontManager", vtVariant );

	pRenderer->put_Width(dW_mm);
	pRenderer->put_Height(lHeightPix * 25.4 / 96);
	pRenderer->CreateFromMediaData((IUnknown*)pFrame, 0, 0, lWidthPix, lHeightPix);

	// скидываем файлы шрифтов (в виде скриптов)
	POSITION pos = mapFontFiles.GetStartPosition();
	while (NULL != pos)
	{
		const CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.GetNext(pos);
		DumpJSFontFile(pPair->m_key, strDirectory, pPair->m_value);
	}
	// -------------------------------------------
	
	// скидывам все названия шрифтов (чтобы легко вставить в комбобокс)
	if (TRUE)
	{
		for (size_t i = 0; i < nCountFonts; ++i)
		{
			strFonts += arrFonts[i];
			strFonts += _T("\n");
		}

		CStringA strA = (CStringA)strFonts;
		CFile oFileNames;
		oFileNames.CreateFile(strDirectory  + _T("\\Fonts_JS") + _T("\\AllFontNames.txt"));
		oFileNames.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileNames.CloseFile();
	}

	// и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
	// все объекты, которые позволят не знать о существующих фонтах
	if (TRUE)
	{
#if 0
		BOOL bIsUSE_ORIGINAL_FONTS = TRUE;

		CStringWriter oWriterJS;

		//var g_fontFiles = new Array();
		CString strArrayFonts = _T("");
		strArrayFonts.Format(_T("var _files = new Array(%d);\n"), lFontFiles);
		oWriterJS.WriteString(strArrayFonts);
		
		// сначала все файлы
		pos = mapFontFiles.GetStartPosition();
		while (NULL != pos)
		{
			const CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.GetNext(pos);
			
			CString strFontId = pPair->m_key;
			CString strNum = _T("");
			strNum.Format(_T("%d"), pPair->m_value);

			strFontId.Replace(_T("\\\\"), _T("\\"));
			strFontId.Replace(_T("/"), _T("\\"));
			int nStart = strFontId.ReverseFind('\\');
			int nEnd = strFontId.ReverseFind('.');

			if (bIsUSE_ORIGINAL_FONTS)
				strFontId = strFontId.Mid(nStart + 1);
			else
			{
				strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);
				strFontId += _T(".js");
			}

			CString str1 = _T("_files[") + strNum + _T("] = new CFontFileLoader(\"") + strFontId + _T("\");\n");
			
			oWriterJS.WriteString(str1);
		}
		oWriterJS.WriteStringC(_T("\n"));
		
		CString strArrayInit = _T("");
		strArrayInit.Format(_T("var _infos = new Array(%d);\n"), nCountFonts);
		oWriterJS.WriteString(strArrayInit);

		for (int index = 0; index < nCountFonts; ++index)
		{
			const CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(arrFonts[index]);

			CString str1 = _T("");
			str1.Format(_T("_infos[%d] = new CFontInfo(\""), index);
			str1 += pPair->m_value.m_sName;
			str1 += _T("\"");

			CString strParams = _T("");
			strParams.Format(_T(",%d,0,%d,%d,%d,%d,%d,%d,%d,%d);\n"), index, pPair->m_value.m_lIndexR, pPair->m_value.m_lFaceIndexR,
									pPair->m_value.m_lIndexI, pPair->m_value.m_lFaceIndexI,
									pPair->m_value.m_lIndexB, pPair->m_value.m_lFaceIndexB,
									pPair->m_value.m_lIndexBI, pPair->m_value.m_lFaceIndexBI);

			oWriterJS.WriteString(str1);
			oWriterJS.WriteString(strParams);

			// thumbnail
			LONG lFontIndex = 0;
			LONG lFaceIndex = 0;
			if (pPair->m_value.m_lIndexR != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexR;
				lFaceIndex = pPair->m_value.m_lFaceIndexR;
			}
			else if (pPair->m_value.m_lIndexI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexI;
				lFaceIndex = pPair->m_value.m_lFaceIndexI;
			}
			else if (pPair->m_value.m_lIndexI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexB;
				lFaceIndex = pPair->m_value.m_lFaceIndexB;
			}
			else if (pPair->m_value.m_lIndexBI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexBI;
				lFaceIndex = pPair->m_value.m_lFaceIndexBI;
			}

			CString strFontPath = _T("");
			CAtlMap<LONG, CString>::CPair* _pair = mapFontFiles2.Lookup(lFontIndex);
			if (NULL != _pair)
				strFontPath = _pair->m_value;

			BSTR bsFontPath = strFontPath.AllocSysString();
			LoadFontFromFile(bsFontPath, 10, 72, 72, lFaceIndex);
			BOOL bIsSymbol = (-1 != ((CFreeTypeFont*)m_pFont)->GetSymbolic()) ? TRUE : FALSE;
			
			if (bIsSymbol)
			{
				SysFreeString(bsFontPath);
				strFontPath = _T("C:\\Windows\\Fonts\\cour.ttf");
				bsFontPath = strFontPath.AllocSysString();
			}

			pRenderer->put_BrushColor1(0);
			BSTR bsText = pPair->m_value.m_sName.AllocSysString();
			pRenderer->put_FontPath(bsFontPath);
			pRenderer->put_FontSize(14);
			pRenderer->put_FontStringGID(0);
			pRenderer->put_FontCharSpace(0);
			pRenderer->CommandDrawText(bsText, 5, 25.4 * (index * lH1_px + lH1_px) / 96 - 2, 0, 0, 0);
			SysFreeString(bsText);
			SysFreeString(bsFontPath);
			// endthumbnail
		}
		oWriterJS.WriteStringC(_T("\n"));

		oWriterJS.WriteStringC(_T("var _maps = {};\n"));
		for (int i = 0; i < nCountFonts; ++i)
		{
			CString sNum = _T("");
			sNum.Format(_T("\"] = %d;\n"), i);
			oWriterJS.WriteStringC(_T("_maps[\""));
			oWriterJS.WriteStringC(arrFonts[i]);
			oWriterJS.WriteStringC(sNum);
		}
#else
		CStringWriter oWriterJS;

		// сначала все файлы
		size_t nCountFiles = mapFontFiles.GetCount();
		if (nCountFiles == 0)
			oWriterJS.WriteStringC(_T("window[\"__fonts_files\"] = []; \n\n"));
		else
		{
			pos = mapFontFiles.GetStartPosition();
			CString* pMassFiles = new CString[nCountFiles];
			while (NULL != pos)
			{
				const CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.GetNext(pos);
				
				CString strFontId = pPair->m_key;
				strFontId.Replace(_T("\\\\"), _T("\\"));
				strFontId.Replace(_T("/"), _T("\\"));
				int nStart = strFontId.ReverseFind('\\');
				strFontId = strFontId.Mid(nStart + 1);

				pMassFiles[pPair->m_value] = strFontId;
			}

			oWriterJS.WriteStringC(_T("window[\"__fonts_files\"] = [\n"));
			for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
			{
				oWriterJS.WriteStringC(_T("\""));
				oWriterJS.WriteString(pMassFiles[nIndex]);
				if (nIndex != (nCountFiles - 1))
					oWriterJS.WriteStringC(_T("\",\n"));
				else
					oWriterJS.WriteStringC(_T("\""));
			}
			oWriterJS.WriteStringC(_T("\n];\n\n"));

			RELEASEARRAYOBJECTS(pMassFiles);
		}
		
		CString strArrayInit = _T("");
		strArrayInit.Format(_T("window[\"__fonts_infos\"] = [\n"), nCountFonts);
		oWriterJS.WriteString(strArrayInit);

		for (int index = 0; index < (int)nCountFonts; ++index)
		{
			const CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(arrFonts[index]);

			CString str1 = _T("");
			str1.Format(_T("[\""), index);
			str1 += pPair->m_value.m_sName;
			
			CString strParams = _T("");
			strParams.Format(_T("\",%d,%d,%d,%d,%d,%d,%d,%d]"), pPair->m_value.m_lIndexR, pPair->m_value.m_lFaceIndexR,
									pPair->m_value.m_lIndexI, pPair->m_value.m_lFaceIndexI,
									pPair->m_value.m_lIndexB, pPair->m_value.m_lFaceIndexB,
									pPair->m_value.m_lIndexBI, pPair->m_value.m_lFaceIndexBI);

			oWriterJS.WriteString(str1);
			oWriterJS.WriteString(strParams);

			if (index != (nCountFonts - 1))
				oWriterJS.WriteStringC(_T(",\n"));
			else
				oWriterJS.WriteStringC(_T("\n"));
			
			// thumbnail
			LONG lFontIndex = 0;
			LONG lFaceIndex = 0;
			if (pPair->m_value.m_lIndexR != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexR;
				lFaceIndex = pPair->m_value.m_lFaceIndexR;
			}
			else if (pPair->m_value.m_lIndexI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexI;
				lFaceIndex = pPair->m_value.m_lFaceIndexI;
			}
			else if (pPair->m_value.m_lIndexI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexB;
				lFaceIndex = pPair->m_value.m_lFaceIndexB;
			}
			else if (pPair->m_value.m_lIndexBI != -1)
			{
				lFontIndex = pPair->m_value.m_lIndexBI;
				lFaceIndex = pPair->m_value.m_lFaceIndexBI;
			}

			CString strFontPath = _T("");
			CAtlMap<LONG, CString>::CPair* _pair = mapFontFiles2.Lookup(lFontIndex);
			if (NULL != _pair)
				strFontPath = _pair->m_value;

			BSTR bsFontPath = strFontPath.AllocSysString();
			LoadFontFromFile(bsFontPath, 10, 72, 72, lFaceIndex);
			BOOL bIsSymbol = (-1 != ((CFreeTypeFont*)m_pFont)->GetSymbolic()) ? TRUE : FALSE;
			
			if (bIsSymbol)
			{
				SysFreeString(bsFontPath);
				strFontPath = _T("C:\\Windows\\Fonts\\cour.ttf");
				bsFontPath = strFontPath.AllocSysString();
			}

			pRenderer->put_BrushColor1(0);
			BSTR bsText = pPair->m_value.m_sName.AllocSysString();
			pRenderer->put_FontPath(bsFontPath);
			pRenderer->put_FontSize(14);
			pRenderer->put_FontStringGID(0);
			pRenderer->put_FontCharSpace(0);
			pRenderer->CommandDrawText(bsText, 5, 25.4 * (index * lH1_px + lH1_px) / 96 - 2, 0, 0, 0);
			SysFreeString(bsText);
			SysFreeString(bsFontPath);
			// endthumbnail
		}
		oWriterJS.WriteStringC(_T("];\n\n"));
#endif
	
		// скинем табнейл
		CString strThumbnailPath = strDirectory + _T("\\Fonts_JS") + _T("\\thumbnail.png");
		ImageStudio::SaveImageAsPNG((IUnknown*)pFrame, strThumbnailPath);	

		RELEASEINTERFACE(pRenderer);
		RELEASEINTERFACE(pFrame);

		CFile oImageFile;
		oImageFile.OpenFile(strThumbnailPath);
		int nInputLen = (int)oImageFile.GetFileSize();
		BYTE* pData = new BYTE[nInputLen];
		oImageFile.ReadFile(pData, nInputLen);
		oImageFile.CloseFile();

		int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
		BYTE* pOutput = new BYTE[nOutputLen];
		Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

		CString _s((char*)pOutput, nOutputLen);

		oWriterJS.WriteStringC(_T("window[\"g_standart_fonts_thumbnail\"] = \"data:image/png;base64,"));
		oWriterJS.WriteStringC(_s);
		oWriterJS.WriteStringC(_T("\";\n"));

		CStringA strA = (CStringA)oWriterJS.GetCString();
		CFile oFileFontsJS;
		oFileFontsJS.CreateFile(strDirectory + _T("\\Fonts_JS") + _T("\\AllFonts.js"));
		oFileFontsJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileFontsJS.CloseFile();	
	}
}

CString CASCFontManager::GetAllFontsJS()
{
	int nCount = m_pFontEngine->GetInstalledFontsCount();

	// сначала строим массив всех файлов шрифтов
	CAtlMap<CString, LONG> mapFontFiles;
	CAtlMap<LONG, CString> mapFontFiles2;
	LONG lFontFiles = 0;
	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pFontEngine->GetInstalledFont(i);

		CString strPath = (CString)pInfo->m_wsFontPath;
		CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.Lookup(strPath);

		if (NULL == pPair)
		{
			mapFontFiles.SetAt(strPath, lFontFiles);
			mapFontFiles2.SetAt(lFontFiles, strPath);
			++lFontFiles;
		}
	}
	// -----------------------------------------

	// теперь строим массив всех шрифтов по имени
	CAtlMap<CString, CFontInfoJS> mapFonts;
	CAtlArray<CString> arrFonts;

	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pFontEngine->GetInstalledFont(i);
		CString strPath = (CString)pInfo->m_wsFontPath;
		CString strName = (CString)pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		CAtlMap<CString, LONG>::CPair* pPairFontFiles = mapFontFiles.Lookup(strPath);
		lFontIndex = pPairFontFiles->m_value;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

		CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(pInfo->m_wsFontName);
		if (NULL != pPair)
		{
			pPair->m_value.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexBI = lFontIndex;
				pPair->m_value.m_lFaceIndexBI = lFaceIndex;
			}
			else if (pInfo->m_bBold)
			{
				pPair->m_value.m_lIndexB = lFontIndex;
				pPair->m_value.m_lFaceIndexB = lFaceIndex;
			}
			else if (pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexI = lFontIndex;
				pPair->m_value.m_lFaceIndexI = lFaceIndex;
			}
			else
			{
				pPair->m_value.m_lIndexR = lFontIndex;
				pPair->m_value.m_lFaceIndexR = lFaceIndex;
			}
		}
		else
		{
			CFontInfoJS fontInfo;

			fontInfo.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
				fontInfo.m_lIndexBI = lFontIndex;
				fontInfo.m_lFaceIndexBI = lFaceIndex;
			}
			else if (pInfo->m_bBold)
			{
				fontInfo.m_lIndexB = lFontIndex;
				fontInfo.m_lFaceIndexB = lFaceIndex;
			}
			else if (pInfo->m_bItalic)
			{
				fontInfo.m_lIndexI = lFontIndex;
				fontInfo.m_lFaceIndexI = lFaceIndex;
			}
			else
			{
				fontInfo.m_lIndexR = lFontIndex;
				fontInfo.m_lFaceIndexR = lFaceIndex;
			}

			mapFonts.SetAt(fontInfo.m_sName, fontInfo);
			arrFonts.Add(fontInfo.m_sName);
		}
	}
	// -------------------------------------------

	// теперь сортируем шрифты по имени ----------
	size_t nCountFonts = arrFonts.GetCount();
	for (size_t i = 0; i < nCountFonts; ++i)
	{
		for (size_t j = i + 1; j < nCountFonts; ++j)
		{
			if (arrFonts[i] > arrFonts[j])
			{
				CString temp = arrFonts[i];
				arrFonts[i] = arrFonts[j];
				arrFonts[j] = temp;
			}
		}
	}
	// -------------------------------------------

	// и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
	// все объекты, которые позволят не знать о существующих фонтах
	if (TRUE)
	{
		CStringWriter oWriterJS;

		// сначала все файлы
		size_t nCountFiles = mapFontFiles.GetCount();
		if (nCountFiles == 0)
			oWriterJS.WriteStringC(_T("window[\"__fonts_files\"] = []; \n\n"));
		else
		{
			POSITION pos = mapFontFiles.GetStartPosition();
			CString* pMassFiles = new CString[nCountFiles];
			while (NULL != pos)
			{
				const CAtlMap<CString, LONG>::CPair* pPair = mapFontFiles.GetNext(pos);
				
				CString strFontId = pPair->m_key;
				strFontId.Replace(_T("/"), _T("\\"));
				strFontId.Replace(_T("\\\\"), _T("\\"));
				strFontId.Replace(_T("\\\\"), _T("\\"));

				strFontId.Replace(_T("\\"), _T("\\\\"));
				
				//int nStart = strFontId.ReverseFind('\\');
				//strFontId = strFontId.Mid(nStart + 1);

				pMassFiles[pPair->m_value] = strFontId;
			}

			oWriterJS.WriteStringC(_T("window[\"__fonts_files\"] = [\n"));
			for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
			{
				oWriterJS.WriteStringC(_T("\""));
				oWriterJS.WriteString(pMassFiles[nIndex]);
				if (nIndex != (nCountFiles - 1))
					oWriterJS.WriteStringC(_T("\",\n"));
				else
					oWriterJS.WriteStringC(_T("\""));
			}
			oWriterJS.WriteStringC(_T("\n];\n\n"));

			RELEASEARRAYOBJECTS(pMassFiles);
		}
		
		CString strArrayInit = _T("");
		strArrayInit.Format(_T("window[\"__fonts_infos\"] = [\n"), nCountFonts);
		oWriterJS.WriteString(strArrayInit);

		for (int index = 0; index < (int)nCountFonts; ++index)
		{
			const CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(arrFonts[index]);

			CString str1 = _T("");
			str1.Format(_T("[\""), index);
			str1 += pPair->m_value.m_sName;
			
			CString strParams = _T("");
			strParams.Format(_T("\",%d,%d,%d,%d,%d,%d,%d,%d]"), pPair->m_value.m_lIndexR, pPair->m_value.m_lFaceIndexR,
									pPair->m_value.m_lIndexI, pPair->m_value.m_lFaceIndexI,
									pPair->m_value.m_lIndexB, pPair->m_value.m_lFaceIndexB,
									pPair->m_value.m_lIndexBI, pPair->m_value.m_lFaceIndexBI);

			oWriterJS.WriteString(str1);
			oWriterJS.WriteString(strParams);

			if (index != (nCountFonts - 1))
				oWriterJS.WriteStringC(_T(",\n"));
			else
				oWriterJS.WriteStringC(_T("\n"));
		}
		oWriterJS.WriteStringC(_T("];\n\n"));

		return oWriterJS.GetCString();
	}
}