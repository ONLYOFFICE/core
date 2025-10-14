/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
