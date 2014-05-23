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
	CAtlArray<CString> arrFonts;

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

	if (NULL != m_pList)
		delete m_pList;

	return 0;
}

