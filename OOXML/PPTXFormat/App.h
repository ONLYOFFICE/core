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

#include "WrapperFile.h"
#include "FileTypes.h"

#include "Logic/PartTitle.h"
#include "Logic/HeadingVariant.h"

namespace PPTX
{
	class App : public WrapperFile
	{
	public:
		App(OOX::Document* pMain);
		App(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~App();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		nullable_string						Template;				// (Name of Document Template)
		nullable_int						TotalTime;				// (Total Edit Time Metadata Element)
		nullable_int						Words;					// (Word Count)
		nullable_string						Application;			// (Application Name)
		nullable_string						PresentationFormat;		// (Intended Format of Presentation)
		nullable_int						Paragraphs;				// (Total Number of Paragraphs)
		nullable_int						Slides;					// (Slides Metadata Element)
		nullable_int						Notes;					// (Number of Slides Containing Notes)
		nullable_int						HiddenSlides;			// (Number of Hidden Slides)
		nullable_int						MMClips;				// (Total Number of Multimedia Clips)
		nullable_bool						ScaleCrop;				// (Thumbnail Display Mode)
		std::vector<Logic::HeadingVariant>	HeadingPairs;			// (Heading Pairs)
		std::vector<Logic::PartTitle>		TitlesOfParts;			// (Part Titles)
		nullable_string						Company;				// (Name of Company)
		nullable_bool						LinksUpToDate;			// (Links Up-to-Date)
		nullable_bool						SharedDoc;				// (Shared Document)
		nullable_bool						HyperlinksChanged;		// (Hyperlinks Changed)
		nullable_string						AppVersion;				// (Application Version)

		nullable_int						Characters;				// (Total Number of Characters)
		nullable_int						CharactersWithSpaces;	// (Number of Characters (With Spaces))
		nullable_int						DocSecurity;			// (Document Security)
		nullable_string						HyperlinkBase;			// (Relative Hyperlink Base)
		nullable_int						Lines;					// (Number of Lines)
		nullable_string						Manager;				// (Name of Manager)
		nullable_int						Pages;					// (Total Number of Pages)

		//DigSig (Digital Signature)
		//std::list<std::string> - ??? HLinks;										// (Hyperlink List)

	private:
		nullable_int						m_VectorSize;
		nullable_int						m_Headings;

		void Normalize();
	};
} // namespace PPTX
