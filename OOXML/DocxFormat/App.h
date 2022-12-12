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

#include "Docx.h"
#include "File.h"
#include "../Base/Nullable.h"
#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"
#include "../../DesktopEditor/common/SystemUtils.h"

namespace PPTX
{
	class App;
}
namespace OOX
{
	class CApp : public OOX::File
	{
	public:
		CApp(OOX::Document* pMain);
		CApp(OOX::Document* pMain, const CPath& oPath);
		virtual ~CApp();

		virtual void read(const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		void SetDefaults();
		void SetRequiredDefaults();

		PPTX::App* ToPptxApp();
		void FromPptxApp(PPTX::App* pApp);

		// TO DO: DigSig
		//        HeadingPairs
		//        HLinks
		//        TitlesOfParts

        nullable_string	m_sApplication;
		nullable_string	m_sAppVersion;
		nullable_int	m_nCharacters;
		nullable_int	m_nCharactersWithSpaces;
        nullable_string	m_sCompany;
		nullable_int	m_nDocSecurity;
		nullable_int	m_nHiddenSlides;
        nullable_string	m_sHyperlinkBase;
		nullable_bool	m_bHyperlinksChanged;
		nullable_int	m_nLines;
		nullable_bool	m_bLinksUpToDate;
        nullable_string	m_sManager;
		nullable_int	m_nMMClips;
		nullable_int	m_nNotes;
		nullable_int	m_nPages;
		nullable_int	m_nParagraphs;
        nullable_string	m_sPresentationForm;
		nullable_bool	m_bScaleCrop;
		nullable_bool	m_bSharedDoc;
		nullable_int	m_nSlides;
        nullable_string	m_sTemplate;
		nullable_int	m_nTotalTime;
		nullable_int	m_nWords;
	};
} // namespace OOX

