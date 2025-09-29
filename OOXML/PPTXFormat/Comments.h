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
	//todooo base_type - >const or define ????
	namespace Logic
	{
//------------------------------------------------------------------------------------------------------------
		class TextCharRangeContext : public WrapperWritingElement //context  
		{
		public:
			PPTX_LOGIC_BASE(TextCharRangeContext)

			nullable_uint len;
			nullable_uint hash;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const { return L""; }

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
		};
//------------------------------------------------------------------------------------------------------------
		class TextCharRangeMoniker : public WrapperWritingElement //txMk  
		{
		public:
			PPTX_LOGIC_BASE(TextCharRangeMoniker)

			nullable_uint cp;
			nullable_uint len;

			nullable<TextCharRangeContext> context;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const { return L""; }

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
			//base_type = 25;
		};
//------------------------------------------------------------------------------------------------------------
		class DrawingMoniker : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE2(DrawingMoniker)

			nullable_uint id;
			nullable_string creationId;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const { return L""; }

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
		protected:
			int base_type = 0xff;
			std::wstring node_name;
		};
//------------------------------------------------------------------------------------------------------------
		class GraphicFrameMoniker : public DrawingMoniker   
		{
		public:
			GraphicFrameMoniker() { base_type = 1; node_name = L"ac:graphicFrameMk";}
			PPTX_LOGIC_BASE_NC(GraphicFrameMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class GroupShapeMoniker : public DrawingMoniker 
		{
		public:
			GroupShapeMoniker() { base_type = 2; node_name = L"ac:grpSpMk";}
			PPTX_LOGIC_BASE_NC(GroupShapeMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class PictureMoniker : public DrawingMoniker
		{
		public:
			PictureMoniker() { base_type = 3; node_name = L"ac:picMk";}
			PPTX_LOGIC_BASE_NC(PictureMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class ShapeMoniker : public DrawingMoniker 
		{
		public:
			ShapeMoniker() { base_type = 4; node_name = L"ac:spMk";}
			PPTX_LOGIC_BASE_NC(ShapeMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class ConnectorMoniker : public DrawingMoniker 
		{
		public:
			ConnectorMoniker() { base_type = 5; node_name = L"ac:cxnSpMk";}
			PPTX_LOGIC_BASE_NC(ConnectorMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class InkMoniker : public DrawingMoniker 
		{
		public:
			InkMoniker() { base_type = 6; node_name = L"ac:inkMk";}
			PPTX_LOGIC_BASE_NC(InkMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class SlideBaseMoniker : public WrapperWritingElement 
		{
		public:
			PPTX_LOGIC_BASE2(SlideBaseMoniker)

			nullable_uint cId;
			nullable_uint sldId;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const { return L""; }

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
		protected:
			int base_type = 0xff;
			std::wstring node_name;
		};
//------------------------------------------------------------------------------------------------------------
		class SlideMasterMoniker : public SlideBaseMoniker 
		{
		public:
			SlideMasterMoniker() { base_type = 11; node_name = L"pc:sldMasterMk"; }
			PPTX_LOGIC_BASE_NC(SlideMasterMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class SlideLayoutMoniker : public SlideBaseMoniker 
		{
		public:
			SlideLayoutMoniker() { base_type = 12; node_name = L"pc:sldLayoutMk";}
			PPTX_LOGIC_BASE_NC(SlideLayoutMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class SlideMoniker : public SlideBaseMoniker 
		{
		public:
			SlideMoniker() { base_type = 10; node_name = L"pc:sldMk";}
			PPTX_LOGIC_BASE_NC(SlideMoniker)
		};
//------------------------------------------------------------------------------------------------------------
		class DocumentMoniker : public WrapperWritingElement //docMk
		{
		public:
			PPTX_LOGIC_BASE(DocumentMoniker)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const { return L""; }

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}
			//base_type = 20;
		};
//------------------------------------------------------------------------------------------------------------
		class CommentReply : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CommentReply)

			nullable_string id;
			nullable_string authorId;
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
//------------------------------------------------------------------------------------------------------------
		class AnyMonikerList : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnyMonikerList)

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) {}
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void FillParentPointersForChilds() {}

			std::vector<smart_ptr<WrapperWritingElement>> m_arrItems;
			int pptyType() const { return baseType; }
		protected:			
			int baseType = 0xff;
		};
		class TextCharRangeMonikerList : public AnyMonikerList //txMkLst
		{//TEXTCHARRANGEMONIKERLIST = DRAWINGMONIKERLIST TEXTCHARRANGEMONIKER
		public:
			TextCharRangeMonikerList() { baseType = 3; }
			PPTX_LOGIC_BASE_NC(TextCharRangeMonikerList)
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class SlideMonikerList : public AnyMonikerList //sldMkLst
		{//SLIDEMONIKERLIST = DOCUMENTMONIKERLIST SLIDEMONIKER
		public:
			SlideMonikerList() { baseType = 4; }
			PPTX_LOGIC_BASE_NC(SlideMonikerList)
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
		class DrawingElementMonikerList : public AnyMonikerList //deMkLst
		{
		public:
			DrawingElementMonikerList() { baseType = 5; }
			PPTX_LOGIC_BASE_NC(DrawingElementMonikerList)
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		};
//------------------------------------------------------------------------------------------------------------
		class Comment : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Comment)

			nullable_int	authorIdx;
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
			nullable_string id;  //Guid
			nullable_string	authorId; //Guid
			nullable_string created;
			nullable_string status;
			nullable_string startDate;
			nullable_string dueDate;
			nullable_string assignedTo;
			nullable_uint complete;
			nullable_string title;

			nullable<TxBody> txBody;
			nullable<CommentReplyList> replyLst;

			//nullable<TextCharRangeMonikerList > txMkLst;
			//nullable<SlideMonikerList> sldMkLst;
			//nullable<DrawingElementMonikerList> deMkLst;
			smart_ptr<AnyMonikerList> anyMkLst;

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
