#pragma once
#include "../stdafx.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Styles/Fonts.h"

namespace BinXlsxRW {
	static TCHAR* gc_sNoNameFont = _T("NoNameFont");
	static TCHAR* gc_sDefaultFontName = _T("Arial");

	class FontProcessor {
		ASCGraphics::IASCFontManager* m_pFontManager;
		std::map<CString, CString> m_mapFontMap;

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
			for (int i = 0, length = oFonts.m_arrItems.size(); i < length; ++i)
			{
					OOX::Spreadsheet::CFont* pFont = oFonts.m_arrItems[i];
					if(NULL != pFont)
						addToFontMap(*pFont);
			}
		}
		ASCGraphics::IASCFontManager* getFontManager()
		{
			return m_pFontManager;
		}
		CString getFont(const CString& name)
		{
			CString fontName = gc_sDefaultFontName;
			std::map<CString, CString>::const_iterator pPair = m_mapFontMap.find( name );
			if ( m_mapFontMap.end() == pPair )
			{
				if(!name.IsEmpty())
				{
					OOX::Spreadsheet::CFont oFont;
					oFont.m_oRFont.Init();
					oFont.m_oRFont->m_sVal = name;
					addToFontMap(oFont);
					pPair = m_mapFontMap.find( name );
					if (m_mapFontMap.end() != pPair)
						fontName = pPair->second;
				}
			}
			else
				fontName = pPair->second;
			return fontName;
		}		
	private:
		void initFontManager()
		{
			RELEASEINTERFACE(m_pFontManager);
			CoCreateInstance(ASCGraphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, ASCGraphics::IID_IASCFontManager, (void**) &m_pFontManager);

			VARIANT var;
			var.vt = VT_BSTR;
			var.bstrVal = m_sFontDir.AllocSysString();
			m_pFontManager->SetAdditionalParam(L"InitializeFromFolder", var);
			RELEASESYSSTRING(var.bstrVal);

#ifdef BUILD_CONFIG_FULL_VERSION
			CString defaultFontName = gc_sDefaultFontName;
			BSTR defFontName = defaultFontName.AllocSysString();
			m_pFontManager->SetDefaultFont(defFontName);
			SysFreeString(defFontName);
#endif
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
			BSTR familyName;
			long index = 0;
			BSTR bstrParams = params.AllocSysString();

#ifdef BUILD_CONFIG_FULL_VERSION
			m_pFontManager->GetWinFontByParams(bstrParams, &fontPath, &index);
			int status = m_pFontManager->LoadFontFromFile(fontPath, 12, 72, 72, index);
			SysFreeString(fontPath);
			m_pFontManager->GetFamilyName(&familyName);
#else
			m_pFontManager->GetWinFontByParams(bstrParams, &familyName, &fontPath, NULL, &index);
#endif
			
			CString resFontName = familyName;
			SysFreeString(fontPath);
			SysFreeString(familyName);
			SysFreeString(bstrParams);

			m_mapFontMap[sFontName] = resFontName;
		}	
	};

}