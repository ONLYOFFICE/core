/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
//#include "../stdafx.h"

#include <map>

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
//#include "DocWrapper/Base.h"
namespace NSCommon{
	template<class Type> class nullable;
}
namespace ComplexTypes{
    namespace Spreadsheet{
        class CString_;
    }
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
		CFontManager* m_pFontManager;
		std::map<CString, CString> fontMap;

	public:
		FontProcessor();
		~FontProcessor();
		
		void setFontManager(CFontManager* pFontManager);
		CFontManager* getFontManager();
		void setFontTable(OOX::CFontTable* fontTable);
		
		CString getFont(const CString& name);
		CString getFont(const NSCommon::nullable<OOX::Spreadsheet::CFontScheme>& oScheme, const NSCommon::nullable<ComplexTypes::Spreadsheet::CString_>& oRFont, const NSCommon::nullable<OOX::Spreadsheet::CCharset>& oCharset, const NSCommon::nullable<OOX::Spreadsheet::CFontFamily >& oFamily, OOX::CTheme* pTheme);		
	private:
		void addToFontMap(OOX::CFont& font);
	};
	
}
