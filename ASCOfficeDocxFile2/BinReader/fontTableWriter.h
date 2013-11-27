#pragma once
#include "Common.h"

namespace Writers
{
	static CString g_string_ft_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" mc:Ignorable=\"w14\">");
	static CString g_string_ft_End = _T("</w:fonts>");

	class FontTableWriter
	{
		CStringWriter	m_oWriter;
		CString	m_sDir;
		ASCGraphics::IASCFontManager* m_pFontManager;
	public:
		CAtlMap<CString, int> m_mapFonts;
	public:
		FontTableWriter(CString sDir, CString sFontDir):m_sDir(sDir)
		{
			m_pFontManager = NULL;
			if(!sFontDir.IsEmpty())
			{
				CoCreateInstance(ASCGraphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, __uuidof(ASCGraphics::IASCFontManager), (void**)&m_pFontManager);
				if(NULL != m_pFontManager)
				{
					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = sFontDir.AllocSysString();
					m_pFontManager->SetAdditionalParam(L"InitializeFromFolder", var);
					RELEASESYSSTRING(var.bstrVal);

#ifdef BUILD_CONFIG_FULL_VERSION
					CString defaultFontName = _T("Arial");
					BSTR defFontName = defaultFontName.AllocSysString();
					m_pFontManager->SetDefaultFont(defFontName);
					SysFreeString(defFontName);
#endif
				}
			}
		}
		~FontTableWriter()
		{
			RELEASEINTERFACE(m_pFontManager);
		}

		void Write()
		{
			m_oWriter.WriteString(g_string_ft_Start);

			//Те шрифты которые всегда пишем в FontTable
			bool bCalibri = false;
			bool bTimes = false;
			bool bCambria = false;
			POSITION pos = m_mapFonts.GetStartPosition();
			while(NULL != pos)
			{
				CAtlMap<CString, int>::CPair* pair = m_mapFonts.GetNext(pos);
				if(NULL != pair)
				{
					const CString& sFontName = pair->m_key;
					if(_T("Calibri") == sFontName)
						bCalibri = true;
					else if(_T("Times New Roman") == sFontName)
						bTimes = true;
					else if(_T("Cambria") == sFontName)
						bCambria = true;
					WriteFont(sFontName);
				}
			}
			if(false == bCalibri)
				WriteFont(_T("Calibri"));
			if(false == bTimes)
				WriteFont(_T("Times New Roman"));
			if(false == bCambria)
				WriteFont(_T("Cambria"));

			m_oWriter.WriteString(g_string_ft_End);

			CFile oFile;
			oFile.CreateFile(m_sDir + _T("\\word\\fontTable.xml"));
			oFile.WriteStringUTF8(m_oWriter.GetData());
			oFile.CloseFile();
		}
		void WriteFont(CString sFontName)
		{
			CString sPanose;
			if(NULL != m_pFontManager)
			{
				long index = 0;
				BSTR bstrFontName = sFontName.AllocSysString();
				SAFEARRAY *psaArray = NULL;
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
				m_pFontManager->GetParamsByFontName(bstrFontName, &psaArray, NULL);
#else
				m_pFontManager->LoadFontByName(bstrFontName, 12, 0, 72, 72);
				m_pFontManager->GetPanose(&psaArray);
#endif
				SysFreeString(bstrFontName);
				if(NULL != psaArray)
				{
					unsigned char* pData = static_cast<unsigned char*>(psaArray->pvData);
					for(int i = 0; i < psaArray->rgsabound->cElements; ++i)
					{
						unsigned char cElem = pData[i];
						if(cElem > 0xF)
							sPanose.AppendFormat(_T("%X"), cElem);
						else
							sPanose.AppendFormat(_T("0%X"), cElem);
					}
				}
				RELEASEARRAY(psaArray);
			}

			CorrectString(sFontName);
			m_oWriter.WriteString(_T("<w:font w:name=\"") + sFontName + _T("\">"));
			if(!sPanose.IsEmpty())
				m_oWriter.WriteString(_T("<w:panose1 w:val=\"")+sPanose+_T("\"/>"));
			m_oWriter.WriteString(CString(_T("</w:font>")));
		}
	};
}