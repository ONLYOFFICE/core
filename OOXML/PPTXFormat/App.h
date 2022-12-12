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
