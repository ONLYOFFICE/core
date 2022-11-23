/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#ifndef _XPS_XPSLIB_PAGE_H
#define _XPS_XPSLIB_PAGE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "FontList.h"
#include "Utils.h"
#include "ContextState.h"

#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

namespace XPS
{
	class CDocument;
	class CStaticResource;

	class Page
	{
	public:
        Page(const std::wstring& wsFile, IFolder* Path, CFontList* pFontList, NSFonts::IFontManager* pFontManager, CDocument* pDocument);
		~Page();

		void GetSize(int& nW, int& nH) const;
		void Draw(IRenderer* pRenderer, bool* pbBreak);

        NSWasm::CPageLink m_oLinks;

	private:

		void DrawCanvas      (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		bool ReadResource    (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawGlyph       (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawPath        (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool StrokeToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void ReadPathData    (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTranform);
		
		bool ClipToRenderer     (const wchar_t* wsString, CContextState* pState);
		bool TransformToRenderer(const wchar_t* wsString, CContextState* pState);

	private:

        std::wstring            m_wsPagePath;
        IFolder*                m_wsRootPath;
        CFontList*              m_pFontList;
        NSFonts::IFontManager*  m_pFontManager;
        CDocument*              m_pDocument;
	};
}

#endif // _XPS_XPSLIB_PAGE_H
