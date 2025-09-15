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

#include "Comments.h"

#include "../Xlsx.h"
#include "../../XlsbFormat/Xlsb.h"

#include "../../XlsbFormat/CommentsStream.h"

#include "../../XlsbFormat/Biff12_unions/COMMENTS.h"
#include "../../XlsbFormat/Biff12_unions/COMMENTAUTHORS.h"
#include "../../XlsbFormat/Biff12_unions/COMMENTLIST.h"
#include "../../XlsbFormat/Biff12_unions/COMMENT.h"

#include "../../XlsbFormat/Biff12_records/CommentAuthor.h"
#include "../../XlsbFormat/Biff12_records/BeginComment.h"
#include "../../XlsbFormat/Biff12_records/CommentText.h"
#include "../../XlsbFormat/Biff12_records/LegacyDrawing.h"

#include "../SharedStrings/Si.h"

#include "../../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/OBJECTS.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/OBJ.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/TEXTOBJECT.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Note.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Obj.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/TxO.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/MsoDrawing.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/XLUnicodeRichExtendedString.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CCommentItem::CCommentItem()
		{
			m_pThreadedComment = NULL;
			m_bThreadedCommentCopy = false;
		}
		bool CCommentItem::IsValid()
			{
				return m_nRow.IsInit() && m_nCol.IsInit() && m_sAuthor.IsInit();
			}

		CAuthors::CAuthors()
		{
		}
		CAuthors::~CAuthors()
		{
			ClearItems();
		}
		void CAuthors::ClearItems()
		{
			m_arrItems.clear();
		}
		void CAuthors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CAuthors::toXML() const
		{
			return L"";
		}
		void CAuthors::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<authors>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				writer.WriteString(L"<author>");
					writer.WriteEncodeXmlString(m_arrItems[i]);
				writer.WriteString(L"</author>");
			}
			writer.WriteString(L"</authors>");
		}
		void CAuthors::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"author" == sName )
				{
					m_arrItems.push_back(oReader.GetText3());
				}
			}
		}
		void CAuthors::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::COMMENTAUTHORS*>(obj.get());
			if (ptr != nullptr)
			{
				for(auto author : ptr->m_arBrtCommentAuthor)
				{
					m_arrItems.push_back(static_cast<XLSB::CommentAuthor*>(author.get())->author.value());
				}
			}
		}
		XLS::BaseObjectPtr CAuthors::toBin()
		{
			auto ptr(new XLSB::COMMENTAUTHORS);
			XLS::BaseObjectPtr objectPtr(ptr);
			for(auto i:m_arrItems)
			{
				auto author(new XLSB::CommentAuthor);
				author->author = i;
				ptr->m_arBrtCommentAuthor.push_back(XLS::BaseObjectPtr{author});
			}
			return objectPtr;
		}
		EElementType CAuthors::getType () const
		{
			return et_x_Authors;
		}
		void CAuthors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		CComment::CComment()
		{
		}
		CComment::~CComment()
		{
		}
		void CComment::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CComment::toXML() const
		{
			return L"";
		}
		void CComment::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_oRef.IsInit() && m_oAuthorId.IsInit() && m_oText.IsInit())
			{
				writer.WriteString(L"<comment");
				WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef->ToString());
				WritingStringNullableAttrInt(L"authorId", m_oAuthorId, m_oAuthorId->GetValue());
				WritingStringNullableAttrString(L"xr:uid", m_oUid, m_oUid->ToString());
				writer.WriteString(L">");

				writer.WriteString(L"<text>");
				m_oText->toXML2(writer);
				writer.WriteString(L"</text>");
				writer.WriteString(L"</comment>");
			}
		}
		void CComment::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("text") == sName )
					m_oText = oReader;
			}
		}
		void CComment::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::COMMENT*>(obj.get());
			if (ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginComment);

				auto ptrCommentText = static_cast<XLSB::CommentText*>(ptr->m_BrtCommentText.get());
				if(ptrCommentText != nullptr)
				{
					CSi* pItem = new CSi();
					pItem->fromBin(ptrCommentText->text, true);
					//auto text = new CText();
					//text->fromBin(ptrCommentText->text.str.value());
					//pItem->m_arrItems.push_back(text);
					m_oText = pItem;
				}
			}
		}
		XLS::BaseObjectPtr CComment::toBin()
		{
			auto ptr(new XLSB::COMMENT);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto ptr1(new XLSB::BeginComment);
			ptr->m_BrtBeginComment = XLS::BaseObjectPtr{ptr1};

			if(m_oRef.IsInit())
				ptr1->rfx = m_oRef->GetValue();
			if(m_oAuthorId.IsInit())
				ptr1->iauthor = m_oAuthorId->GetValue();
            else
                ptr1->iauthor = 0;
			if(m_oUid.IsInit())
				ptr1->guid = m_oUid->ToString();
            else
                ptr1->guid = L"";
            if(m_oText.IsInit())
			{
				auto type = m_oText->getType();
				auto text(new XLSB::CommentText);
				if(type == OOX::et_x_Si)
				{
					text->text.fExtStr = false;
					text->text.fRichStr = false;
					text->text.str = m_oText->ToString();
				}
				else if(type == OOX::et_x_rPh)
				{
					text->text.fExtStr = true;
					text->text.fRichStr = false;
					text->text.phoneticStr = m_oText->ToString();
				}
				else if(type == OOX::et_x_rPr)
				{
					text->text.fExtStr = false;
					text->text.fRichStr = true;
				}
                text->text.str = m_oText->ToString();
				ptr->m_BrtCommentText = XLS::BaseObjectPtr{text};
			}

			return objectPtr;
		}
		XLS::BaseObjectPtr CComment::toXLS(XLS::BaseObjectPtr objectsPointer, unsigned int id)
		{
			auto ptr = new XLS::Note;
			if(m_oRef.IsInit())
			{
				XLS::Ref8 ref;
				ref = m_oRef->GetValue();
				ptr->note_sh.col = ref.columnFirst;
				ptr->note_sh.row = ref.rowFirst;

			}

			ptr->note_sh.idObj = id;

			auto objectsPtr = static_cast<XLS::OBJECTS*>(objectsPointer.get());
			{
				auto objUnion = new XLS::OBJ(boost::dynamic_pointer_cast<XLS::MsoDrawing>(objectsPtr->m_arrObject.back().first));
				//object writing
				auto objPtr = new XLS::Obj(objUnion->mso_drawing_);
				objUnion->m_Obj = XLS::BaseObjectPtr(objPtr);

				objPtr->cmo.ot = 0x19;
				objPtr->cmo.id = id;
				if(m_oUid.IsInit())
				{
					objPtr->nts.guid = m_oUid->ToString();
				}

				objectsPtr->m_arrObject.back().second.push_back(XLS::BaseObjectPtr(objUnion));

				//txo writing
				auto textUnion = new XLS::TEXTOBJECT(objUnion->mso_drawing_);
				objectsPtr->m_arrObject.back().second.push_back(XLS::BaseObjectPtr(textUnion));
				auto textPtr = new XLS::TxO(objUnion->mso_drawing_);
				textUnion->m_TxO = XLS::BaseObjectPtr(textPtr);
				if(m_oText.IsInit())
				{
					auto extendedText  = m_oText->toXLS();
					auto castedText = static_cast<XLS::XLUnicodeRichExtendedString*>(extendedText.get());
					textPtr->rawText = castedText->str_;
					textPtr->TxOruns.lastRun.cchText = castedText->str_.size();
					for(auto i : castedText->rgRun)
					{
						XLS::RunPtr TextRun(new XLS::Run);
						TextRun->formatRun.ich = i.ich;
						TextRun->formatRun.ifnt = i.ifnt;
						textPtr->TxOruns.rgTxoRuns.push_back(TextRun);
					}

				}

			}
			return  XLS::BaseObjectPtr(ptr);
		}
		EElementType CComment::getType () const
		{
			return et_x_Comment;
		}
		void CComment::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"ref",      m_oRef )
				WritingElement_ReadAttributes_Read_if ( oReader, L"authorId", m_oAuthorId )
				WritingElement_ReadAttributes_Read_if ( oReader, L"xr:uid", m_oUid )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CComment::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginComment*>(obj.get());
			if (ptr != nullptr)
			{
				m_oRef      = ptr->rfx.toString(true, true);
				m_oAuthorId = ptr->iauthor;
				m_oUid      = ptr->guid;
			}
		}

		CCommentList::CCommentList()
		{
		}
		CCommentList::~CCommentList()
		{
		}
		void CCommentList::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCommentList::toXML() const
		{
			return L"";
		}
		void CCommentList::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<commentList>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</commentList>");
		}
		void CCommentList::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"comment" == sName )
				{
					CComment* pComment = new CComment();
					*pComment = oReader;
					m_arrItems.push_back(pComment);
				}
			}
		}
		void CCommentList::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::COMMENTLIST*>(obj.get());
			if (ptr != nullptr)
			{
				for(auto comment : ptr->m_arCOMMENT)
				{
					m_arrItems.push_back(new CComment(comment));
				}
			}
		}
		XLS::BaseObjectPtr CCommentList::toBin()
		{
			auto ptr(new XLSB::COMMENTLIST);
			XLS::BaseObjectPtr objectPtr(ptr);
			for(auto i:m_arrItems)
				ptr->m_arCOMMENT.push_back(i->toBin());
			return objectPtr;
		}
		EElementType CCommentList::getType () const
		{
			return et_x_CommentList;
		}
		void CCommentList::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		CComments::CComments(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
			{
				xlsx->m_arWorksheets.back()->m_pComments = this;
			}
		}
		CComments::CComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
			{
				xlsx->m_arWorksheets.back()->m_pComments = this;
			}
			read( oRootPath, oPath );
		}
		CComments::~CComments()
		{
		}
		void CComments::readBin(const CPath& oPath)
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if (xlsb)
			{
				XLSB::CommentsStreamPtr commentsStream(new XLSB::CommentsStream);

				xlsb->ReadBin(oPath, commentsStream.get());

				if (commentsStream != nullptr)
				{
					auto ptr = static_cast<XLSB::COMMENTS*>(commentsStream->m_COMMENTS.get());
					if (ptr != nullptr)
					{
						if(ptr->m_COMMENTAUTHORS != nullptr)
							m_oAuthors = ptr->m_COMMENTAUTHORS;

						if(ptr->m_COMMENTLIST != nullptr)
							m_oCommentList = ptr->m_COMMENTLIST;
					}
				}

				//commentsStream.reset();
			}
		}
		XLS::BaseObjectPtr CComments::WriteBin() const
		{
			XLSB::CommentsStreamPtr commentsStream(new XLSB::CommentsStream);
			auto ptr(new XLSB::COMMENTS);
			commentsStream->m_COMMENTS = XLS::BaseObjectPtr{ptr};

			if(m_oAuthors.IsInit())
				ptr->m_COMMENTAUTHORS = m_oAuthors->toBin();

			if(m_oCommentList.IsInit())
				ptr->m_COMMENTLIST = m_oCommentList->toBin();

			return XLS::BaseObjectPtr{commentsStream};
		}
		std::vector<XLS::BaseObjectPtr> CComments::toXLS(XLS::BaseObjectPtr objectsPointer) const
		{
			std::vector<XLS::BaseObjectPtr> objectVector;
			if(m_oCommentList.IsInit())
			{
				auto objectsPtr = static_cast<XLS::OBJECTS*>(objectsPointer.get());
				std::pair<XLS::BaseObjectPtr, std::vector<XLS::BaseObjectPtr>> objPair;

				auto drawingPtr = new XLS::MsoDrawing(false);
				drawingPtr->prepareComment(1);
				objPair.first = XLS::BaseObjectPtr(drawingPtr);
				objectsPtr->m_arrObject.push_back(objPair);
				unsigned int id = 1;
				for(auto i : m_oCommentList->m_arrItems)
				{
					objectVector.push_back(i->toXLS(objectsPointer, id));
					id++;
				}
			}
			return objectVector;
		}
		void CComments::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CComments::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			if( m_oReadPath.GetExtention() == _T(".bin"))
			{
				readBin(m_oReadPath);
				return;
			}

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( L"comments" == sName )
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
				{
					int nStylesDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
					{
						sName = XmlUtils::GetNameNoNS(oReader.GetName());

						if ( L"authors" == sName )
							m_oAuthors = oReader;
						else if ( L"commentList" == sName )
							m_oCommentList = oReader;
					}
				}
			}
		}
		void CComments::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				XLS::BaseObjectPtr object = WriteBin();
				xlsb->WriteBin(oPath, object.get());
			}
			else
			{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<comments xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:xr=\"http://schemas.microsoft.com/office/spreadsheetml/2014/revision\" \
mc:Ignorable=\"xr\">");
			if(m_oAuthors.IsInit())
				m_oAuthors->toXML(sXml);
			if(m_oCommentList.IsInit())
				m_oCommentList->toXML(sXml);
			sXml.WriteString(L"</comments>");

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
			}
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		const OOX::FileType CComments::type() const
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				return OOX::SpreadsheetBin::FileTypes::CommentsBin;
			}
			return OOX::Spreadsheet::FileTypes::Comments;
		}
		const CPath CComments::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CComments::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CComments::GetReadPath()
		{
			return m_oReadPath;
		}
		void CComments::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CLegacyDrawingWorksheet::CLegacyDrawingWorksheet()
		{
		}
		CLegacyDrawingWorksheet::~CLegacyDrawingWorksheet()
		{
		}
		void CLegacyDrawingWorksheet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CLegacyDrawingWorksheet::toXML() const
		{
			return L"";
		}
		void CLegacyDrawingWorksheet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_oId.IsInit())
			{
				writer.WriteString(L"<legacyDrawing r:id=\"");
				writer.WriteString(m_oId->ToString());
				writer.WriteString(L"\"/>");
			}

		}
		void CLegacyDrawingWorksheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CLegacyDrawingWorksheet::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CLegacyDrawingWorksheet::toBin()
		{
			auto castedPtr(new XLSB::LegacyDrawing);
			XLS::BaseObjectPtr ptr(castedPtr);
				if(m_oId.IsInit())
					castedPtr->stRelId.value = m_oId->GetValue();
            return ptr;
		}
        void CLegacyDrawingWorksheet::toBin(XLS::StreamCacheWriterPtr& writer)
        {
            auto record = writer->getNextRecord(XLSB::rt_LegacyDrawing);
            XLSB::RelID stRelId;
            if(m_oId.IsInit())
                stRelId = m_oId->GetValue();
            else
                stRelId.value.setSize(0xFFFFFFFF);
            *record << stRelId;
            writer->storeNextRecord(record);
        }
		EElementType CLegacyDrawingWorksheet::getType () const
		{
			return et_x_LegacyDrawingWorksheet;
		}
		void CLegacyDrawingWorksheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"id", m_oId )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void CLegacyDrawingWorksheet::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::LegacyDrawing*>(obj.get());
				if(ptr != nullptr)
					m_oId = ptr->stRelId.value.value();
			}

	} //Spreadsheet
} // namespace OOX
