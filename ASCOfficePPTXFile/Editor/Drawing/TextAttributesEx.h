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
#pragma once

#if !defined(_WIN32) && !defined (_WIN64)
    #include "../../../DesktopEditor/graphics/aggplustypes.h"
#else
    #include <windows.h>
#endif

#include "Structures.h"
#include "TextStructures.h"

namespace PPT_FORMAT
{
	class CTheme;
	class CLayout;
	
	class CTextAttributesEx
	{
	public:
		LONG	m_lTextMasterType; // only ppt property
		_UINT32	m_lTextType;
		
		int		m_lStyleThemeIndex;
		
		int		m_lPlaceholderType;
		int		m_lPlaceholderID;

		// граница
        Aggplus::RECT m_oBounds;

		// настройки по-умолчанию
		CTextAttributes			m_oAttributes;
		std::vector<CParagraph>	m_arParagraphs;

        bool	m_bVertical;
		bool	m_bAutoFit;

		int 	m_nTextFlow;


		int		m_lWrapMode; // 0 - square, default; 1 - none wrap


		// для ппт. чтобы не менять счас ничего
		CTextRuler				m_oRuler;

		CTextStyles				m_oLayoutStyles;
		CTextStyles				m_oStyles;

		// из пптх
		bool					m_bIsSlideFontRef;
		int						m_lFontRef;

		CTextAttributesEx() :
			m_oAttributes(),
			m_arParagraphs(),
			m_oRuler(),
			m_oLayoutStyles(),
			m_oStyles()
		{
			m_lTextType			= -1;
			m_lPlaceholderType	= -1;
			m_lPlaceholderID	= -1;

			m_lStyleThemeIndex	= -1;

			m_lFontRef			= -1;
			m_bIsSlideFontRef	= false;
					
			m_oBounds.left		= 0;
			m_oBounds.top		= 0;
			m_oBounds.right		= 50;
			m_oBounds.bottom	= 50;

			m_bVertical			= false;
			m_bAutoFit			= false;
			m_lWrapMode			= 0;
			m_nTextFlow			= -1;

			m_lTextMasterType	= -1;
		}
		CTextAttributesEx& operator =(const CTextAttributesEx& oSrc)
		{
			m_oBounds		= oSrc.m_oBounds;

			m_lTextType			= oSrc.m_lTextType;
			m_lPlaceholderType	= oSrc.m_lPlaceholderType;
			m_lPlaceholderID	= oSrc.m_lPlaceholderID;

			m_lFontRef			= oSrc.m_lFontRef;
			m_bIsSlideFontRef	= oSrc.m_bIsSlideFontRef;

			m_oAttributes	= oSrc.m_oAttributes;
			m_bVertical		= oSrc.m_bVertical;
			m_lWrapMode		= oSrc.m_lWrapMode;
			m_bAutoFit		= oSrc.m_bAutoFit;
			m_nTextFlow		= oSrc.m_nTextFlow;

			m_arParagraphs.insert(m_arParagraphs.end(), oSrc.m_arParagraphs.begin(), oSrc.m_arParagraphs.end());
			m_oRuler = oSrc.m_oRuler;

			m_oLayoutStyles		= oSrc.m_oLayoutStyles;
			m_oStyles			= oSrc.m_oStyles;

			m_lTextMasterType = oSrc.m_lTextMasterType;

			return *this;
		}

		CTextAttributesEx(const CTextAttributesEx& oSrc)
		{
			*this = oSrc;
		}

		~CTextAttributesEx()
		{
			m_arParagraphs.clear();
		}

        inline void NormalizeString(std::wstring& strText)
		{
            strText = XmlUtils::EncodeXmlString(strText);
		}

		void RecalcParagraphsPPT();
		void ApplyThemeStyle	(CTheme* pTheme = NULL);
		
		void ApplyRuler			(CTheme* pTheme);
		void ApplyRuler			(CTextPFRun* pPar, WORD lIndentLevel);
		bool IsEmptyText()
		{
			return (0 == m_arParagraphs.size()) ? true : false;
		}
	};
}
