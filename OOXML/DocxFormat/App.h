/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "File.h"
#include "WritingElement.h"
#include "../Base/Nullable.h"

#include "../PPTXFormat/Logic/PartTitle.h"
#include "../PPTXFormat/Logic/HeadingVariant.h"

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

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		// TO DO: DigSig
		//        HLinks

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

		std::vector<PPTX::Logic::HeadingVariant> HeadingPairs;
		std::vector<PPTX::Logic::PartTitle> TitlesOfParts;
	private:
		nullable_int m_VectorSize;
		nullable_int m_Headings;
	};

} // namespace OOX

