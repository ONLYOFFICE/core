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
#include "WrapperWritingElement.h"
#include "../Base/Unit.h"
#include "Logic/TxBody.h"

namespace PPTX
{
	namespace Logic
	{
		class CommentReply : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CommentReply)

			nullable_string id;
			nullable_int authorId;
			nullable_string status;
			nullable_string created;

			nullable<TxBody> txBody;
			//extLst

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds();
		};

		class CommentReplyList : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CommentReplyList)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds();

			std::vector<CommentReply> m_arReply;
		};
		class AnyMonikerList : public WrapperWritingElement
		{
		public:
			AnyMonikerList() {}
			virtual ~AnyMonikerList();

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) {}
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}

			std::vector<WrapperWritingElement*> m_arrItems;
		};
		class TextCharRangeMonikerList : public AnyMonikerList
		{
		public:
			PPTX_LOGIC_BASE(TextCharRangeMonikerList)

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class SlideMonikerList : public AnyMonikerList
		{
		public:
			PPTX_LOGIC_BASE(SlideMonikerList)

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class DrawingElementMonikerList : public AnyMonikerList
		{
		public:
			PPTX_LOGIC_BASE(DrawingElementMonikerList)

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class Comment : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Comment)

			nullable_int	authorId;
			nullable_int	idx;
			nullable_string dt;

			nullable_int	pos_x;
			nullable_int	pos_y;

			nullable_string	text;
			
			nullable_int	parentAuthorId;
			nullable_int	parentCommentId;

			nullable_string additional_data; // teamlab editor information!!!

			nullable_int timeZoneBias;
//------------------------------------------------------------------------------------------
//modern
			nullable_string id;
			nullable_string created;
			nullable_string status;
			nullable_string startDate;
			nullable_string dueDate;
			nullable_string assignedTo;
			nullable_uint complete;
			nullable_string title;

			nullable<TxBody> txBody;
			nullable<CommentReplyList> replyLst;
			nullable<TextCharRangeMonikerList > txMkLst;
			nullable<SlideMonikerList> sldMkLst;
			nullable<DrawingElementMonikerList> deMkLst;

			bool bModern = false;
//------------------------------------------------------------------------------------------
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds();
		};
	}
	
	class Comments : public WrapperFile
	{
	public:
		std::vector<PPTX::Logic::Comment> m_arComments;
		bool bModern = false;

		Comments(OOX::Document* pMain);
		Comments(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~Comments();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
	};
} // namespace PPTX
