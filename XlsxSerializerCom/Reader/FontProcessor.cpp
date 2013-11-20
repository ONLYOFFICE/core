#include "FontProcessor.h"

namespace XlsxReader {
		
	FontProcessor::FontProcessor() 
			: m_pFontManager(NULL) {}

	FontProcessor::~FontProcessor() {
		RELEASEINTERFACE(m_pFontManager);
	}
	
	void FontProcessor::setFontDir(const CString& fontDir) {
		this->m_sFontDir = fontDir;
		initFontManager();
	}
	void FontProcessor::setFontTable(const OOX::Spreadsheet::CFonts& oFonts) {
		for (int i = 0, length = oFonts.m_arrItems.GetSize(); i < length; ++i)
		{
			OOX::Spreadsheet::WritingElement* we = oFonts.m_arrItems[i];
			if(OOX::Spreadsheet::et_Font == we->getType())
			{
				OOX::Spreadsheet::CFont* pFont = static_cast<OOX::Spreadsheet::CFont*>(we);
				if(NULL != pFont)
					addToFontMap(pFont);
			}
		}
	}
	
	CString FontProcessor::getFont(const CString& name) {
		CString fontName = _T("Arial");
		if (fontMap.find(name) != fontMap.end())
			fontName = fontMap[name];
		else
		{
			OOX::CFont font;
			font.m_sName = name;
			addToFontMap(font);
			if (fontMap.find(name) != fontMap.end())
				fontName = fontMap[name];
		}
		return fontName;
	}
	
	void FontProcessor::initFontManager() {
		RELEASEINTERFACE(m_pFontManager);
		CoCreateInstance(__uuidof(AVSGraphics::CAVSFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSFontManager), (void**) &m_pFontManager);

		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = m_sFontDir.AllocSysString();
		m_pFontManager->SetAdditionalParam(L"InitializeFromFolder", var);
		RELEASESYSSTRING(var.bstrVal);

		CString defaultFontName = _T("Arial");
		BSTR defFontName = defaultFontName.AllocSysString();
		fontManager->SetDefaultFont(defFontName);
		SysFreeString(defFontName);
	}

	void FontProcessor::addToFontMap(OOX::Spreadsheet::CFont* pFont) {
		CString parw;
		parw += _T("<FontProperties>");
		if(pFont->m_oCharset.IsInit() && pFont->m_oCharset->m_oCharset.IsInit())
		{
			SimpleTypes::EFontCharset eCharset = pFont->m_oCharset->m_oCharset->GetValue();
			//на серверве на берем в расчет fontcharsetANSI и fontcharsetDefault, потому что он зависит от локали, а dll работает на сервере
			if(SimpleTypes::fontcharsetANSI !=  eCharset && SimpleTypes::fontcharsetDefault != eCharset)
				parw += _T("<Charset value='") + pFont->m_oCharset->m_oCharset->ToHexString() + _T("'/>");
		}
		CString sFontName;
		if(pFont->m_oRFont.IsInit())
			sFontName = pFont->m_oRFont->ToString2();
		if(sFontName.IsEmpty())
			parw += _T("<Name value='") + CString(gc_sNoNameFont) + _T("'/>");
		else
		{
			//ImageStudio::Serialize::Paint::Common::ToXmlString(sFontName);
			parw += _T("<Name value='")+ sFontName + _T("'/>");
		}
		if(pFont->m_oScheme->m_oFontScheme.IsInit())
		{
		}
		parw += _T("<FamilyClass name='") + font.m_oFamily.ToString() + _T("'/>");
		if(font.m_oPanose.IsInit())
			parw += _T("<Panose value='") + font.m_oPanose->ToString() + _T("'/>");
		if (font.m_oPitch.GetValue() == SimpleTypes::pitchFixed)
			parw += _T("<FixedWidth value='1'/>");
		else
			parw += _T("<FixedWidth value='0'/>");
		parw += _T("<UnicodeRange ");
		if (font.m_oUsb0.IsInit())
			parw += _T("range1='") + font.m_oUsb0->ToString() + _T("' ");
		if (font.m_oUsb1.IsInit())
			parw += _T("range2='") + font.m_oUsb1->ToString() + _T("' ");
		if (font.m_oUsb2.IsInit())
			parw += _T("range3='") + font.m_oUsb2->ToString() + _T("' ");
		if (font.m_oUsb3.IsInit())
			parw += _T("range4='") + font.m_oUsb3->ToString() + _T("' ");
		if (font.m_oCsb0.IsInit())
			parw += _T("coderange1='") + font.m_oCsb0->ToString() + _T("' ");
		if (font.m_oCsb1.IsInit())
			parw += _T("coderange2='") + font.m_oCsb1->ToString() + _T("' ");
		parw += _T("/>");
		parw += _T("</FontProperties>");
		CString params = parw.GetCString();
		
		BSTR fontPath;
		long index = 0;
		BSTR bstrParams = params.AllocSysString();
		fontManager->GetWinFontByParams(bstrParams, &fontPath, &index);
		SysFreeString(bstrParams);
		int status = fontManager->LoadFontFromFile(fontPath, 12, 72, 72, index);
		SysFreeString(fontPath);

		BSTR familyName;
		fontManager->GetFamilyName(&familyName);
		CString resFontName = familyName;
		SysFreeString(familyName);

		fontMap[font.m_sName] = resFontName;
	}	
}