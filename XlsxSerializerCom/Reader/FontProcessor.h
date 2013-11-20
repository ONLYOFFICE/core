#pragma once
#include "../stdafx.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Styles/Fonts.h"

namespace BinXlsxRW {
	static TCHAR* gc_sNoNameFont = _T("NoNameFont");
	static TCHAR* gc_sDefaultFontName = _T("Arial");

	class FontProcessor {
		AVSGraphics::IAVSFontManager* m_pFontManager;
		CAtlMap<CString, CString> m_mapFontMap;

		CString m_sFontDir;

	public:
		FontProcessor():m_pFontManager(NULL){}
		~FontProcessor()
		{
			RELEASEINTERFACE(m_pFontManager);
		}

		void setFontDir(const CString& fontDir)
		{
			this->m_sFontDir = fontDir;
			initFontManager();
		}
		CString getFontDir()
		{
			return this->m_sFontDir;
		}
		void setFontTable(const OOX::Spreadsheet::CFonts& oFonts)
		{
			for (int i = 0, length = oFonts.m_arrItems.GetSize(); i < length; ++i)
			{
					OOX::Spreadsheet::CFont* pFont = oFonts.m_arrItems[i];
					if(NULL != pFont)
						addToFontMap(*pFont);
			}
		}
		AVSGraphics::IAVSFontManager* getFontManager()
		{
			return m_pFontManager;
		}
		CString getFont(const CString& name)
		{
			CString fontName = gc_sDefaultFontName;
			CAtlMap<CString, CString>::CPair* pPair = m_mapFontMap.Lookup( name );
			if ( NULL == pPair )
			{
				if(!name.IsEmpty())
				{
					OOX::Spreadsheet::CFont oFont;
					oFont.m_oRFont.Init();
					oFont.m_oRFont->m_sVal = name;
					addToFontMap(oFont);
					pPair = m_mapFontMap.Lookup( name );
					if (NULL != pPair)
						fontName = pPair->m_value;
				}
			}
			else
				fontName = pPair->m_value;
			return fontName;
		}		
	private:
		void initFontManager()
		{
			RELEASEINTERFACE(m_pFontManager);
			CoCreateInstance(__uuidof(AVSGraphics::CAVSFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSFontManager), (void**) &m_pFontManager);

			VARIANT var;
			var.vt = VT_BSTR;
			var.bstrVal = m_sFontDir.AllocSysString();
			m_pFontManager->SetAdditionalParam(L"InitializeFromFolder", var);
			RELEASESYSSTRING(var.bstrVal);

			CString defaultFontName = gc_sDefaultFontName;
			BSTR defFontName = defaultFontName.AllocSysString();
			m_pFontManager->SetDefaultFont(defFontName);
			SysFreeString(defFontName);
		}

		void addToFontMap(OOX::Spreadsheet::CFont& font)
		{
			CString parw;
			parw += _T("<FontProperties>");
			if(font.m_oCharset.IsInit() && font.m_oCharset->m_oCharset.IsInit())
			{
				SimpleTypes::Spreadsheet::EFontCharset eCharset = font.m_oCharset->m_oCharset->GetValue();
				//на серверве на берем в расчет fontcharsetANSI и fontcharsetDefault, потому что он зависит от локали, а dll работает на сервере
				if(SimpleTypes::fontcharsetANSI !=  eCharset && SimpleTypes::fontcharsetDefault != eCharset)
					parw += _T("<Charset value='") + font.m_oCharset->m_oCharset->ToHexString() + _T("'/>");
			}
			CString sFontName;
			if(font.m_oScheme.IsInit() && font.m_oScheme->m_oFontScheme.IsInit())
			{
				//берем шрифт из темы
				const SimpleTypes::Spreadsheet::EFontScheme eFontScheme = font.m_oScheme->m_oFontScheme->GetValue();
				if(SimpleTypes::Spreadsheet::fontschemeNone != eFontScheme)
				{
					//todo
				}
			}
			if(sFontName.IsEmpty() && font.m_oRFont.IsInit() && font.m_oRFont->m_sVal.IsInit())
				sFontName = font.m_oRFont->ToString2();
			if(sFontName.IsEmpty())
				parw += _T("<Name value='") + CString(gc_sNoNameFont) + _T("'/>");
			else
			{
				//ImageStudio::Serialize::Paint::Common::ToXmlString(sFontName);
				parw += _T("<Name value='")+ sFontName + _T("'/>");
			}
			if(font.m_oFamily.IsInit() && font.m_oFamily->m_oFontFamily.IsInit())
			{
				parw += _T("<FamilyClass name='") + font.m_oFamily->m_oFontFamily->ToString() + _T("'/>");
			}
			parw += _T("</FontProperties>");
			CString params = parw;

			BSTR fontPath;
			long index = 0;
			BSTR bstrParams = params.AllocSysString();
			m_pFontManager->GetWinFontByParams(bstrParams, &fontPath, &index);
			SysFreeString(bstrParams);
			int status = m_pFontManager->LoadFontFromFile(fontPath, 12, 72, 72, index);
			SysFreeString(fontPath);

			BSTR familyName;
			m_pFontManager->GetFamilyName(&familyName);
			CString resFontName = familyName;
			SysFreeString(familyName);

			m_mapFontMap.SetAt(sFontName, resFontName);
		}	
	};

}