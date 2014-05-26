// FontMaps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinFont.h"
#include "File.h"

class CFontInfoJS
{
public:		
	CString	m_sName;

	LONG	m_lIndexR;
	LONG	m_lFaceIndexR;
	CAtlArray<CString> namesR;

	LONG	m_lIndexI;
	LONG	m_lFaceIndexI;
	CAtlArray<CString> namesI;

	LONG	m_lIndexB;
	LONG	m_lFaceIndexB;
	CAtlArray<CString> namesB;

	LONG	m_lIndexBI;
	LONG	m_lFaceIndexBI;
	CAtlArray<CString> namesBI;

	CFontInfoJS()
	{
		m_sName			= _T("");

		m_lIndexR		= -1;
		m_lFaceIndexR	= -1;
		m_lIndexI		= -1;
		m_lFaceIndexI	= -1;
		m_lIndexB		= -1;
		m_lFaceIndexB	= -1;
		m_lIndexBI		= -1;
		m_lFaceIndexBI	= -1;
	}

	CFontInfoJS(const CFontInfoJS& oSrc)
	{
		*this = oSrc;
	}

	CFontInfoJS& operator=(const CFontInfoJS& oSrc)
	{
		m_sName			= oSrc.m_sName;

		m_lIndexR	= oSrc.m_lIndexR;
		m_lIndexI	= oSrc.m_lIndexI;
		m_lIndexB	= oSrc.m_lIndexB;
		m_lIndexBI	= oSrc.m_lIndexBI;

		m_lFaceIndexR	= oSrc.m_lFaceIndexR;
		m_lFaceIndexI	= oSrc.m_lFaceIndexI;
		m_lFaceIndexB	= oSrc.m_lFaceIndexB;
		m_lFaceIndexBI	= oSrc.m_lFaceIndexBI;

		namesR.RemoveAll();
		namesR.Copy(oSrc.namesR);

		namesI.RemoveAll();
		namesI.Copy(oSrc.namesI);

		namesB.RemoveAll();
		namesB.Copy(oSrc.namesB);

		namesBI.RemoveAll();
		namesBI.Copy(oSrc.namesBI);

		return *this;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	FT_Library pLibrary = NULL;
					
	CString strFolder = _T("\\\\mediaserver\\Exchange\\Korshul\\Fonts");
	CWinFontList* m_pList = NULL;

	if (!FT_Init_FreeType( &pLibrary ))
	{
		if (_T("") == strFolder)
			m_pList = new CWinFontList(pLibrary);
		else
			m_pList = new CWinFontList(pLibrary, strFolder);

		FT_Done_FreeType( pLibrary );
	}

	CString strFontDictionaryPath = _T("X:\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\DesktopEditor\\freetype_names\\FontMaps\\FontDictionary.h");

	int nCount = m_pList->GetFonts()->GetLength();

	// сначала строим массив всех файлов шрифтов
	CAtlMap<CString, LONG> mapFontFiles;
	CAtlMap<LONG, CString> mapFontFiles2;
	LONG lFontFiles = 0;
	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pList->GetByIndex(i);

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
	CAtlMap<CString, CFontInfoJS> mapFontsUnicodes;
	CAtlArray<CString> arrFonts;
	CAtlArray<CString> arrFontsUnicodes;

	CAtlMap<CString, BOOL> mapMainAscii;
	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pList->GetByIndex(i);
		CString strPath = (CString)pInfo->m_wsFontPath;
		CString strName = (CString)pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		CAtlMap<CString, LONG>::CPair* pPairFontFiles = mapFontFiles.Lookup(strPath);
		lFontIndex = pPairFontFiles->m_value;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

		mapMainAscii.SetAt(pInfo->m_wsFontName, TRUE);

		CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(pInfo->m_wsFontName);
		if (NULL != pPair)
		{
			pPair->m_value.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexBI = lFontIndex;
				pPair->m_value.m_lFaceIndexBI = lFaceIndex;

				pPair->m_value.namesBI.RemoveAll();
				pPair->m_value.namesBI.Copy(pInfo->names);
			}
			else if (pInfo->m_bBold)
			{
				pPair->m_value.m_lIndexB = lFontIndex;
				pPair->m_value.m_lFaceIndexB = lFaceIndex;

				pPair->m_value.namesB.RemoveAll();
				pPair->m_value.namesB.Copy(pInfo->names);
			}
			else if (pInfo->m_bItalic)
			{
				pPair->m_value.m_lIndexI = lFontIndex;
				pPair->m_value.m_lFaceIndexI = lFaceIndex;

				pPair->m_value.namesI.RemoveAll();
				pPair->m_value.namesI.Copy(pInfo->names);
			}
			else
			{
				pPair->m_value.m_lIndexR = lFontIndex;
				pPair->m_value.m_lFaceIndexR = lFaceIndex;

				pPair->m_value.namesR.RemoveAll();
				pPair->m_value.namesR.Copy(pInfo->names);
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

				fontInfo.namesBI.RemoveAll();
				fontInfo.namesBI.Copy(pInfo->names);
			}
			else if (pInfo->m_bBold)
			{
				fontInfo.m_lIndexB = lFontIndex;
				fontInfo.m_lFaceIndexB = lFaceIndex;

				fontInfo.namesB.RemoveAll();
				fontInfo.namesB.Copy(pInfo->names);
			}
			else if (pInfo->m_bItalic)
			{
				fontInfo.m_lIndexI = lFontIndex;
				fontInfo.m_lFaceIndexI = lFaceIndex;

				fontInfo.namesI.RemoveAll();
				fontInfo.namesI.Copy(pInfo->names);
			}
			else
			{
				fontInfo.m_lIndexR = lFontIndex;
				fontInfo.m_lFaceIndexR = lFaceIndex;

				fontInfo.namesR.RemoveAll();
				fontInfo.namesR.Copy(pInfo->names);
			}

			mapFonts.SetAt(fontInfo.m_sName, fontInfo);
			arrFonts.Add(fontInfo.m_sName);
		}
	}

	// additional names
	for (int i = 0; i < nCount; ++i)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pList->GetByIndex(i);
		CString strPath = (CString)pInfo->m_wsFontPath;
		CString strName = (CString)pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		CAtlMap<CString, LONG>::CPair* pPairFontFiles = mapFontFiles.Lookup(strPath);
		lFontIndex = pPairFontFiles->m_value;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

		int nNamesAdditional = pInfo->names.GetCount();
		for (int j = 0; j < nNamesAdditional; ++j)
		{
			CString strNameA = pInfo->names[j];

			CAtlMap<CString, BOOL>::CPair* pPairMain = mapMainAscii.Lookup(strNameA);
			if (NULL != pPairMain)
				continue;

			WCHAR* pBufferA = strNameA.GetBuffer();
			int len = strNameA.GetLength();
			
			CAtlMap<CString, CFontInfoJS>* pMap = &mapFonts;
			CAtlArray<CString>* pArrFonts = &arrFonts;

			for (int k = 0; k < len; ++k)
			{
				if (pBufferA[k] > 255)
				{
					pMap = &mapFontsUnicodes;
					pArrFonts = &arrFontsUnicodes;
					break;
				}
			}
			
			CAtlMap<CString, CFontInfoJS>::CPair* pPair = pMap->Lookup(strNameA);
			if (NULL != pPair)
			{
				pPair->m_value.m_sName = strNameA;

				if (pInfo->m_bBold && pInfo->m_bItalic)
				{
					pPair->m_value.m_lIndexBI = lFontIndex;
					pPair->m_value.m_lFaceIndexBI = lFaceIndex;

					pPair->m_value.namesBI.RemoveAll();
					pPair->m_value.namesBI.Copy(pInfo->names);
				}
				else if (pInfo->m_bBold)
				{
					pPair->m_value.m_lIndexB = lFontIndex;
					pPair->m_value.m_lFaceIndexB = lFaceIndex;

					pPair->m_value.namesB.RemoveAll();
					pPair->m_value.namesB.Copy(pInfo->names);
				}
				else if (pInfo->m_bItalic)
				{
					pPair->m_value.m_lIndexI = lFontIndex;
					pPair->m_value.m_lFaceIndexI = lFaceIndex;

					pPair->m_value.namesI.RemoveAll();
					pPair->m_value.namesI.Copy(pInfo->names);
				}
				else
				{
					pPair->m_value.m_lIndexR = lFontIndex;
					pPair->m_value.m_lFaceIndexR = lFaceIndex;

					pPair->m_value.namesR.RemoveAll();
					pPair->m_value.namesR.Copy(pInfo->names);
				}
			}
			else
			{
				CFontInfoJS fontInfo;

				fontInfo.m_sName = strNameA;

				if (pInfo->m_bBold && pInfo->m_bItalic)
				{
					fontInfo.m_lIndexBI = lFontIndex;
					fontInfo.m_lFaceIndexBI = lFaceIndex;

					fontInfo.namesBI.RemoveAll();
					fontInfo.namesBI.Copy(pInfo->names);
				}
				else if (pInfo->m_bBold)
				{
					fontInfo.m_lIndexB = lFontIndex;
					fontInfo.m_lFaceIndexB = lFaceIndex;

					fontInfo.namesB.RemoveAll();
					fontInfo.namesB.Copy(pInfo->names);
				}
				else if (pInfo->m_bItalic)
				{
					fontInfo.m_lIndexI = lFontIndex;
					fontInfo.m_lFaceIndexI = lFaceIndex;

					fontInfo.namesI.RemoveAll();
					fontInfo.namesI.Copy(pInfo->names);
				}
				else
				{
					fontInfo.m_lIndexR = lFontIndex;
					fontInfo.m_lFaceIndexR = lFaceIndex;

					fontInfo.namesR.RemoveAll();
					fontInfo.namesR.Copy(pInfo->names);
				}

				pMap->SetAt(fontInfo.m_sName, fontInfo);
				pArrFonts->Add(fontInfo.m_sName);
			}
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

	size_t nCountFontsU = arrFontsUnicodes.GetCount();
	for (size_t i = 0; i < nCountFontsU; ++i)
	{
		for (size_t j = i + 1; j < nCountFontsU; ++j)
		{
			if (arrFontsUnicodes[i] > arrFontsUnicodes[j])
			{
				CString temp = arrFontsUnicodes[i];
				arrFontsUnicodes[i] = arrFontsUnicodes[j];
				arrFontsUnicodes[j] = temp;
			}
		}
	}

#if 0
	CFile oFile;
	oFile.CreateFile(_T("c:\\fonts.txt"));

	BYTE bom[3];
	bom[0] = 0xEF;
	bom[1] = 0xBB;
	bom[2] = 0xBF;
	oFile.WriteFile((void*)&bom, 3);

	CString strInfos = _T("");
	
	for (int index = 0; index < nCountFonts; ++index)
	{
		const CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(arrFonts[index]);

		CString strFontInfo = pPair->m_value.m_sName + _T(": [");

		for (size_t i = 0; i < pPair->m_value.namesR.GetCount(); ++i)
		{
			strFontInfo += pPair->m_value.namesR[i];
			strFontInfo += _T(",");
		}
		strFontInfo += _T(";");

		for (size_t i = 0; i < pPair->m_value.namesI.GetCount(); ++i)
		{
			strFontInfo += pPair->m_value.namesI[i];
			strFontInfo += _T(",");
		}
		strFontInfo += _T(";");

		for (size_t i = 0; i < pPair->m_value.namesB.GetCount(); ++i)
		{
			strFontInfo += pPair->m_value.namesB[i];
			strFontInfo += _T(",");
		}
		strFontInfo += _T(";");

		for (size_t i = 0; i < pPair->m_value.namesBI.GetCount(); ++i)
		{
			strFontInfo += pPair->m_value.namesBI[i];
			strFontInfo += _T(",");
		}
		
		strFontInfo += _T("]\n");

		strInfos += strFontInfo;
	}

	oFile.WriteStringUTF8(strInfos);

	oFile.CloseFile();
#endif

	CFile oFileW;
	oFileW.CreateFile(strFontDictionaryPath);

	BYTE bom[3];
	bom[0] = 0xEF;
	bom[1] = 0xBB;
	bom[2] = 0xBF;
	oFileW.WriteFile((void*)&bom, 3);

	CString strAll = _T("");

	CString strConstant1 = _T("#ifndef _FONT_DICTIONARY_H\n\n\
typedef struct FD_FontMapRec_\n\
{\n\
	const char*		m_name;\n\
\n\
	int				m_index_r;\n\
	int				m_index_i;\n\
	int				m_index_b;\n\
	int				m_index_bi;\n\
} FD_FontMapRec;\n\n\
typedef struct FD_FontMapRecW_\n\
{\n\
	const wchar_t*	m_name;\n\
\n\
	int				m_index_r;\n\
	int				m_index_i;\n\
	int				m_index_b;\n\
	int				m_index_bi;\n\
} FD_FontMapRecW;\n\n");

	strAll += strConstant1;

	int nAsciiNamesCount = (int)arrFonts.GetCount();
	CString sAsciiNames = _T("");
	sAsciiNames.Format(_T("#define FONTS_DICT_ASCII_NAMES_COUNT %d\n"), nAsciiNamesCount);
	sAsciiNames += _T("static const FD_FontMapRec FD_Ascii_Names[FONTS_DICT_ASCII_NAMES_COUNT] = \n{\n");

	for (int k = 0; k < nAsciiNamesCount; ++k)
	{
		CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFonts.Lookup(arrFonts[k]);

		sAsciiNames += _T("\t{ \"");
		sAsciiNames += pPair->m_value.m_sName;
		sAsciiNames += _T("\", ");

		CString strP = _T("");

		if (k != (nAsciiNamesCount - 1))
		{
			strP.Format(_T("%d, %d, %d, %d },\n"), pPair->m_value.m_lIndexR,
				pPair->m_value.m_lIndexI, pPair->m_value.m_lIndexB, pPair->m_value.m_lIndexBI);
		}
		else
		{
			strP.Format(_T("%d, %d, %d, %d }\n"), pPair->m_value.m_lIndexR,
				pPair->m_value.m_lIndexI, pPair->m_value.m_lIndexB, pPair->m_value.m_lIndexBI);
		}

		sAsciiNames += strP;
	}

	sAsciiNames += _T("};\n\n");

	strAll += sAsciiNames;

	int _offsets[256];
	for (int t = 0; t < 256; ++t)
		_offsets[t] = -1;

	int nCurChar = -1;
	for (int k = 0; k < nAsciiNamesCount; ++k)
	{
		int nChar = (int)arrFonts[k].GetAt(0);
		nChar = max(0, min(nChar, 255));

		if (nChar != nCurChar)
		{
			_offsets[nChar] = k;
		}
		nCurChar = nChar;
	}

	CString strAsciiOffsets = _T("static const int FD_Ascii_Names_Offsets[256] =\n{\n");

	for (int k = 0; k < 256; ++k)
	{
		CString sMem = _T("");
		sMem.Format(_T("%d"), _offsets[k]);

		while (sMem.GetLength() < 4)
			sMem = (_T(" ") + sMem);
		
		if (0 == k % 32)
			sMem = _T("\t") + sMem;

		if (k != 255)
			sMem += _T(",");
		
		if (0 == (k + 1) % 32)
			sMem += _T("\n");

		strAsciiOffsets += sMem;
	}

	strAsciiOffsets += _T("};\n\n");

	strAll += strAsciiOffsets;

	int nUnicodeNamesCount = (int)arrFontsUnicodes.GetCount();
	CString sUnicodeNames = _T("");
	sUnicodeNames.Format(_T("#define FONTS_DICT_UNICODE_NAMES_COUNT %d\n"), nUnicodeNamesCount);
	sUnicodeNames += _T("static const FD_FontMapRecW FD_Unicode_Names[FONTS_DICT_UNICODE_NAMES_COUNT] = \n{\n");

	for (int k = 0; k < nUnicodeNamesCount; ++k)
	{
		CAtlMap<CString, CFontInfoJS>::CPair* pPair = mapFontsUnicodes.Lookup(arrFontsUnicodes[k]);

		sUnicodeNames += _T("\t{ L\"");
		sUnicodeNames += pPair->m_value.m_sName;
		sUnicodeNames += _T("\", ");

		CString strP = _T("");

		if (k != (nAsciiNamesCount - 1))
		{
			strP.Format(_T("%d, %d, %d, %d },\n"), pPair->m_value.m_lIndexR,
				pPair->m_value.m_lIndexI, pPair->m_value.m_lIndexB, pPair->m_value.m_lIndexBI);
		}
		else
		{
			strP.Format(_T("%d, %d, %d, %d }\n"), pPair->m_value.m_lIndexR,
				pPair->m_value.m_lIndexI, pPair->m_value.m_lIndexB, pPair->m_value.m_lIndexBI);
		}

		sUnicodeNames += strP;
	}

	sUnicodeNames += _T("};\n\n");

	strAll += sUnicodeNames;

	CString strConstant2 = _T("typedef struct FD_Font_Rec\n\
{\n\
	const char*		m_name;\n\
	int				m_name_index;\n\
\n\
	long			m_lIndex;\n\
\n\
	unsigned char	m_bBold;\n\
	unsigned char	m_bItalic;\n\
	unsigned char	m_bIsFixed;\n\
\n\
	unsigned char	m_aPanose[10];\n\
	unsigned long	m_ulUnicodeRange1;\n\
	unsigned long	m_ulUnicodeRange2;\n\
	unsigned long	m_ulUnicodeRange3;\n\
	unsigned long	m_ulUnicodeRange4;\n\
\n\
	unsigned long	m_ulCodePageRange1;\n\
	unsigned long	m_ulCodePageRange2;\n\
\n\
	unsigned short	m_usWeigth;\n\
	unsigned short	m_usWidth;\n\
\n\
	short			m_sFamilyClass;\n\
	unsigned char	m_eFontFormat;\n\
\n\
	short			m_shAvgCharWidth;\n\
	short			m_shAscent;\n\
	short			m_shDescent;\n\
	short			m_shLineGap;\n\
	short			m_shXHeight;\n\
	short			m_shCapHeight;\n\
} FD_Font;\n\n");

	strAll += strConstant2;

	int nAllFontsCount = (int)nCount;
	CString sAllFontsNames = _T("");
	sAllFontsNames.Format(_T("#define FONTS_DICT_ASCII_FONTS_COUNT %d\n"), nCount);
	sAllFontsNames += _T("static const FD_Font FD_Ascii_Files[FONTS_DICT_ASCII_FONTS_COUNT] = \n{\n");

	for (int k = 0; k < nCount; ++k)
	{
		CWinFontInfo* pInfo = (CWinFontInfo*)m_pList->GetByIndex(k);
		
		int nFontName = 0;

		CString sMem = _T("");
		sMem.Format(_T("\", %d, %d, %d, %d, %d, { %d, %d, %d, %d, %d, %d, %d, %d, %d, %d }, %u, %u, %u, %u, %u, %u, %u, %u, %d, %d, %d, %d, %d, %d, %d, %d }"),
			nFontName,
			pInfo->m_lIndex,
			pInfo->m_bBold,
			pInfo->m_bItalic,
			pInfo->m_bIsFixed,
			pInfo->m_aPanose[0],
			pInfo->m_aPanose[1],
			pInfo->m_aPanose[2],
			pInfo->m_aPanose[3],
			pInfo->m_aPanose[4],
			pInfo->m_aPanose[5],
			pInfo->m_aPanose[6],
			pInfo->m_aPanose[7],
			pInfo->m_aPanose[8],
			pInfo->m_aPanose[9],
			pInfo->m_ulUnicodeRange1,
			pInfo->m_ulUnicodeRange2,
			pInfo->m_ulUnicodeRange3,
			pInfo->m_ulUnicodeRange4,
			pInfo->m_ulCodePageRange1,
			pInfo->m_ulCodePageRange2,
			pInfo->m_usWeigth,
			pInfo->m_usWidth,
			pInfo->m_sFamilyClass,
			pInfo->m_eFontFormat,
			pInfo->m_shAvgCharWidth,
			pInfo->m_shAscent,
			pInfo->m_shDescent,
			pInfo->m_shLineGap,
			pInfo->m_shXHeight,
			pInfo->m_shCapHeight);

		sAllFontsNames += _T("\t{\"");
		sAllFontsNames += pInfo->m_wsFontName;
		sAllFontsNames += sMem;
		if (k != (nCount - 1))
			sAllFontsNames += _T(",\n");
		else
			sAllFontsNames += _T("\n");
	}

	sAllFontsNames += _T("};\n\n");

	strAll += sAllFontsNames;

	strAll += _T("#endif /* _FONT_DICTIONARY_H */");

	oFileW.WriteStringUTF8(strAll);
	oFileW.CloseFile();

	if (NULL != m_pList)
		delete m_pList;

	return 0;
}

