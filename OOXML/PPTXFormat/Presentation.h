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
#include "FileContainer.h"
#include "FileTypes.h"

#include "Presentation/PresentationChildElements.h"
#include "CommentAuthors.h"

#include "Limit/Conformance.h"
#include "Logic/TextListStyle.h"
#include "Logic/ClrMap.h"
#include "Logic/ExtP.h"
#include "Theme/ClrScheme.h"
#include "Comments.h"

#include "../DocxFormat/Media/VbaProject.h"
#include "../DocxFormat/Media/JsaProject.h"


namespace PPTX
{
	namespace nsPresentation
	{
		class Kinsoku;
		class NotesSz;
		class PhotoAlbum;
		class SldSz;
		class SectionLst;
	}

	class Presentation : public WrapperFile, public PPTX::FileContainer
	{
	public:
		Presentation(OOX::Document *pMain);
		Presentation(OOX::Document *pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~Presentation();
		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		virtual const OOX::FileType type() const
		{
			if (m_bMacroEnabled)	return OOX::Presentation::FileTypes::PresentationMacro;
			else					return OOX::Presentation::FileTypes::Presentation;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		nullable<Logic::TextListStyle>				defaultTextStyle;
		std::vector<nsPresentation::EmbeddedFont>	embeddedFontLst;
		std::vector<Logic::XmlId>					handoutMasterIdLst;
		nullable<nsPresentation::Kinsoku>			kinsoku;
		//modifyVerifier (Modification Verifier)
		std::vector<Logic::XmlId>					notesMasterIdLst;
		nullable<nsPresentation::NotesSz>			notesSz;
		nullable<nsPresentation::PhotoAlbum>		photoAlbum;
		std::vector<Logic::XmlId>					sldIdLst;
		std::vector<Logic::XmlId>					sldMasterIdLst;
		std::vector<OOX::RId>						custDataLst;
		nullable<nsPresentation::CustShowLst>		custShowLst;
		nullable<nsPresentation::SldSz>				sldSz;
		nullable<nsPresentation::SectionLst>		sectionLst;
		//smartTags (Smart Tags)

		nullable_bool							attrAutoCompressPictures;
		nullable_int							attrBookmarkIdSeed;
		nullable_bool							attrCompatMode;
		nullable_limit<Limit::Conformance>		attrConformance;
		nullable_bool							attrEmbedTrueTypeFonts;
		nullable_int							attrFirstSlideNum;
		nullable_bool							attrRemovePersonalInfoOnSave;
		nullable_bool							attrRtl;
		nullable_bool							attrSaveSubsetFonts;
		nullable_string							attrServerZoom;
		nullable_bool							attrShowSpecialPlsOnTitleSld;
		nullable_bool							attrStrictFirstAndLastChars;

		smart_ptr<PPTX::Authors>	commentAuthors;
		smart_ptr<PPTX::Comments>	comments;

		bool						m_bMacroEnabled;
		smart_ptr<OOX::VbaProject>	m_pVbaProject;
		smart_ptr<OOX::JsaProject>	m_pJsaProject;		

	private:
		void Normalize();
	};
} // namespace PPTX
