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

#include "../Logic/XmlId.h"
#include "./../Limit/FrameShape.h"
#include "./../Limit/AlbumLayout.h"
#include "./../Limit/SlideSize.h"
#include "./../Logic/TextFont.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class Section : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Section)

			nullable_string				id;
			nullable_string				name;
			std::vector<Logic::XmlId>	arSldIdLst;

			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
		};

		class SectionLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SectionLst)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::vector<Section> arSectionLst;

		protected:
			virtual void FillParentPointersForChilds() {};
		};

		class SldSz : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SldSz)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		public:
			int cx;
			int cy;
			nullable_limit<Limit::SlideSize> type;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();
		};
		class PhotoAlbum : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PhotoAlbum)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_bool							bw;
			nullable_limit<Limit::FrameShape>		frame;
			nullable_limit<Limit::AlbumLayout>		layout;
			nullable_bool							showCaptions;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class NotesSz : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NotesSz)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		public:
			int cx;
			int cy;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();
		};
		class Kinsoku : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Kinsoku)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

		public:
			std::wstring				invalEndChars;
			std::wstring				invalStChars;
			nullable_string				lang;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class EmbeddedFontDataId : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EmbeddedFontDataId)

				virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			std::wstring rid;
			std::wstring m_name;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class EmbeddedFont : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EmbeddedFont)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

		public:
			nullable<EmbeddedFontDataId> Bold;
			nullable<EmbeddedFontDataId> BoldItalic;
			nullable<EmbeddedFontDataId> Italic;
			nullable<EmbeddedFontDataId> Regular;
			Logic::TextFont Font;

		protected:
			virtual void FillParentPointersForChilds();
		};
		class CustShow : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CustShow)

			std::vector<OOX::RId>	arSldIdLst;

			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
		};
		class CustShowLst : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CustShowLst)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::vector<CustShow> arCustShowLst;

		protected:
			virtual void FillParentPointersForChilds() {};
		};
	}
} // namespace PPTX
