#pragma once
#include "../stdafx.h"

#include <map>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
//#include "DocWrapper/Base.h"
namespace NSCommon{
	template<class Type> class nullable;
}
namespace OOX
{
	class CFont;
	class CFontTable;
	class CTheme;
	namespace Spreadsheet
	{
		class CFont;
		class CFontTable;
		class CFontScheme;
		class CString_;
		class CCharset;
		class CFontFamily;
	}
}

namespace DocWrapper {
	
	class FontProcessor {
		CApplicationFonts m_oApplicationFonts;
		CFontManager* m_pFontManager;
		ASCGraphics::IASCFontManager* fontManager;
		std::map<CString, CString> fontMap;

		CString fontDir;

	public:
		FontProcessor();
		~FontProcessor();
		
		void setFontDir(const CString& fontDir);
		void setFontTable(OOX::CFontTable* fontTable);
		
		CString getFont(const CString& name);
		CString getFont(const NSCommon::nullable<OOX::Spreadsheet::CFontScheme>& oScheme, const NSCommon::nullable<ComplexTypes::Spreadsheet::CString_>& oRFont, const NSCommon::nullable<OOX::Spreadsheet::CCharset>& oCharset, const NSCommon::nullable<OOX::Spreadsheet::CFontFamily >& oFamily, OOX::CTheme* pTheme);

		ASCGraphics::IASCFontManager* getFontManager() {
			return fontManager;
		}
		
	private:
		void initFontManager();
		
		void addToFontMap(OOX::CFont& font);
	};
	
}