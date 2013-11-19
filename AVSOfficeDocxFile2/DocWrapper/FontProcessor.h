#pragma once
#include "../stdafx.h"

#include <map>
//#include "DocWrapper/Base.h"

namespace DocWrapper {
	
	class FontProcessor {
		AVSGraphics::IAVSFontManager* fontManager;
		std::map<CString, CString> fontMap;

		CString fontDir;

	public:
		static bool useSystemFonts;

	public:
		FontProcessor();
		~FontProcessor();
		
		void setFontDir(const CString& fontDir);
		void setFontTable(OOX::CFontTable* fontTable);
		
		CString getFont(const CString& name);
		void getFonts(CAtlArray<CString>& fonts);

		AVSGraphics::IAVSFontManager* getFontManager() {
			return fontManager;
		}
		
	private:
		void initFontManager();
		
		void addToFontMap(OOX::CFont& font);
		
		bool checkRange(OOX::CFont& font);
		void ToXmlString(CString& strVal);
	};
	
}