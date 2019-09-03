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
#ifndef OOX_DOCX_INCLUDE_H_
#define OOX_DOCX_INCLUDE_H_

#include "../Base/SmartPtr.h"

#include "Rels.h"
#include "IFileContainer.h"
#include "FileTypes.h"
#include "../../../../DesktopEditor/common/Directory.h"

#if !defined(_WIN32) && !defined (_WIN64)
#include <sys/stat.h>
#endif
namespace PPTX
{
	class Theme;
}
namespace OOX
{
	class CApp;
	class CCore;
	class CDocument;
	class CFontTable;
	class CNumbering;
	class CStyles;
	class CFootnotes;
	class CEndnotes;
	class CSettings;
	class CComments;
	class CCommentsExt;
	class CCommentsIds;
	class CPeople;
	class CDocumentComments;
	class CDocumentCommentsExt;
	class CDocumentPeople;
	class CDocumentCommentsIds;
	class VbaProject;
	class JsaProject;
	class CHdrFtr;

	class CDocx : public OOX::Document, public OOX::IFileContainer
	{
	public:

		CDocx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
		{
			init();
		}
		CDocx(const CPath& oFilePath) : OOX::IFileContainer(this)
		{
			init();
			Read( oFilePath );
		}
        bool Read(const CPath& oFilePath);
		void FixAfterRead();
        bool Write(const CPath& oFilePath)
		{

			// TO DO: Запись надо править. Она НЕ РАБОТАЕТ!!!! Проблемы в IFileContainer.

            //return false;

			// Создаем папку
			NSDirectory::CreateDirectory(oFilePath.GetPath());

			OOX::CRels         oRels;
			OOX::CContentTypes oContent;

			IFileContainer::Write( oRels, oFilePath, OOX::CPath( L"" ), oContent );
			oRels.Write( oFilePath / FILE_SEPARATOR_STR );
			oContent.Write( oFilePath );

            return true;
		}

		OOX::CHdrFtr *GetHeaderOrFooter(const OOX::RId& rId) const;
		const std::wstring GetCustomSettings() const;
		
		OOX::CApp			*m_pApp;
		OOX::CCore			*m_pCore;

		OOX::CDocument		*m_pDocument;   // Основной документ word/document.xml
		OOX::CFontTable		*m_pFontTable;  // Таблица шрифтов   word/fonttable.xml
		OOX::CNumbering		*m_pNumbering;  // Нумерации         word/numbering.xml
		OOX::CStyles		*m_pStyles;     // Стили             word/styles.xml
		OOX::CFootnotes		*m_pFootnotes;  // Сноски            word/footnotes.xml
		OOX::CEndnotes		*m_pEndnotes;   // Сноски            word/endnotes.xml
		OOX::CSettings		*m_pSettings;   // Настройки         word/settings.xml
		OOX::CComments		*m_pComments;   //			         word/comments.xml
		OOX::CCommentsExt	*m_pCommentsExt;//					word/commentsExtended.xml
		OOX::CCommentsIds	*m_pCommentsIds;//					word/commentsIds.xml
		OOX::CPeople		*m_pPeople;		//					word/people.xml
		OOX::CDocumentComments		*m_pDocumentComments;   //			         word/documentComments.xml
		OOX::CDocumentCommentsExt	*m_pDocumentCommentsExt;//					word/documentCommentsExtended.xml
		OOX::CDocumentPeople		*m_pDocumentPeople;		//					word/documentPeople.xml
		OOX::CDocumentCommentsIds	*m_pDocumentCommentsIds;//					word/documentCommentsIds.xml
		
		OOX::VbaProject		*m_pVbaProject;
		OOX::JsaProject		*m_pJsaProject;
		
		PPTX::Theme			*m_pTheme;

private:
		void init()
		{
			m_pDocument  = NULL;
			m_pFontTable = NULL;
			m_pNumbering = NULL;
			m_pStyles    = NULL;
			m_pApp       = NULL;
			m_pCore      = NULL;
			m_pEndnotes  = NULL;
			m_pFootnotes = NULL;
			m_pSettings  = NULL;
			m_pTheme     = NULL;
			m_pComments  = NULL;
			m_pCommentsExt	= NULL;
			m_pCommentsIds = NULL;
			m_pPeople		= NULL;
			m_pDocumentComments  = NULL;
			m_pDocumentCommentsExt	= NULL;
			m_pDocumentPeople		= NULL;
			m_pDocumentCommentsIds = NULL;
			m_pVbaProject	= NULL;
			m_pJsaProject	= NULL;
		}
	};
} // OOX

#endif // OOX_DOCX_INCLUDE_H_
