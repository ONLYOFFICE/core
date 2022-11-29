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

#include "ThreadedComments.h"
#include "../Xlsx.h"
#include "../Worksheets/Worksheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CPerson::CPerson()
		{
		}
		CPerson::~CPerson()
		{
		}
		void CPerson::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPerson::toXML() const
		{
			return L"";
		}
		void CPerson::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<person");
				WritingStringNullableAttrEncodeXmlString(L"displayName", displayName, *displayName);
				WritingStringNullableAttrString(L"id", id, *id);
				WritingStringNullableAttrString(L"userId", userId, *userId);
				WritingStringNullableAttrEncodeXmlString(L"providerId", providerId, *providerId);
			writer.WriteString(L"/>");
		}
		void CPerson::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"extLst" == sName )
				{
					extLst = oReader;
				}
			}
		}
		EElementType CPerson::getType () const
		{
			return et_x_ThreadedComment;
		}
		void CPerson::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"displayName",	displayName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"userId",		userId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"providerId",	providerId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",			id )
				WritingElement_ReadAttributes_End( oReader )
			}

		CPersonList::CPersonList(OOX::Document* pMain) : OOX::File(pMain)
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && xlsx->m_pWorkbook)
			{
				xlsx->m_pWorkbook->m_pPersonList = this;
			}
		}
		CPersonList::CPersonList(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && xlsx->m_pWorkbook)
			{
				xlsx->m_pWorkbook->m_pPersonList = this;
			}
			read( oRootPath, oPath );
		}
		CPersonList::~CPersonList()
		{
			ClearItems();
		}
		void CPersonList::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CPersonList::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( L"personList" == sName )
			{
				fromXML(oReader);
			}
		}
		void CPersonList::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPersonList::toXML() const
		{
			return L"";
		}
		void CPersonList::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<personList ");
				writer.WriteString(L" xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2018/threadedcomments\"");
				writer.WriteString(L" xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"");
			writer.WriteString(L">");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</personList>");
		}
		void CPersonList::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"person" == sName )
				{
					m_arrItems.push_back(new CPerson(oReader));
				}
			}
		}
		EElementType CPersonList::getType () const
		{
			return et_x_PersonList;
		}
		void CPersonList::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

			toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		}
		const OOX::FileType CPersonList::type() const
		{
			return OOX::Spreadsheet::FileTypes::Persons;
		}
		const CPath CPersonList::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CPersonList::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CPersonList::GetReadPath()
		{
			return m_oReadPath;
		}
		const nullable<std::unordered_map<std::wstring, CPerson*>> CPersonList::GetPersonList()
		{
			nullable<std::unordered_map<std::wstring, CPerson*>> mapPersonList;
			mapPersonList.Init();
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i]->id.IsInit())
				{
					(*mapPersonList)[m_arrItems[i]->id.get()] = m_arrItems[i];
				}
			}
			return mapPersonList;
		}
		void CPersonList::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CThreadedCommentMention::CThreadedCommentMention()
		{
		}
		CThreadedCommentMention::~CThreadedCommentMention()
		{
		}
		void CThreadedCommentMention::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CThreadedCommentMention::toXML() const
		{
			return L"";
		}
		void CThreadedCommentMention::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<mention");
				WritingStringNullableAttrString(L"mentionpersonId", mentionpersonId, mentionpersonId->ToString());
				WritingStringNullableAttrString(L"mentionId", mentionId, mentionId->ToString());
				WritingStringNullableAttrInt(L"startIndex", startIndex, startIndex->GetValue());
				WritingStringNullableAttrInt(L"length", length, length->GetValue());
			writer.WriteString(L"/>");
		}
		void CThreadedCommentMention::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CThreadedCommentMention::getType () const
		{
			return et_x_ThreadedCommentMention;
		}
		void CThreadedCommentMention::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"mentionpersonId",  mentionpersonId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"mentionId",		mentionId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"startIndex",		startIndex )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"length",			length )

				WritingElement_ReadAttributes_End( oReader )
			}

		CThreadedCommentMentions::CThreadedCommentMentions()
		{
		}
		CThreadedCommentMentions::~CThreadedCommentMentions()
		{
		}
		void CThreadedCommentMentions::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CThreadedCommentMentions::toXML() const
		{
			return L"";
		}
		void CThreadedCommentMentions::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<mentions>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</mentions>");
		}
		void CThreadedCommentMentions::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("mention") == sName )
					m_arrItems.push_back(new CThreadedCommentMention(oReader));
			}
		}
		EElementType CThreadedCommentMentions::getType () const
		{
			return et_x_ThreadedCommentMentions;
		}
		void CThreadedCommentMentions::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CThreadedComment::CThreadedComment()
		{
		}
		CThreadedComment::~CThreadedComment()
		{
		}
		void CThreadedComment::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CThreadedComment::toXML() const
		{
			return L"";
		}
		void CThreadedComment::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<threadedComment");
				WritingStringNullableAttrEncodeXmlString(L"ref", ref, ref.get());
				WritingStringNullableAttrString(L"dT", dT, dT->ToString());
				WritingStringNullableAttrString(L"personId", personId, personId->ToString());
				WritingStringNullableAttrString(L"id", id, id->ToString());
				WritingStringNullableAttrString(L"parentId", parentId, parentId->ToString());
				WritingStringNullableAttrBool2(L"done", done);
			writer.WriteString(L">");

			if(m_oText.IsInit())
			{
				writer.WriteString(_T("<text xml:space=\"preserve\">"));
				writer.WriteEncodeXmlStringHHHH(m_oText->m_sText);
				//last '\n' not in format but excel add it
				writer.WriteString(_T("\n"));//todo \r?
				writer.WriteString(_T("</text>"));
			}
			if(m_oMentions.IsInit())
			{
				m_oMentions->toXML(writer);
			}
			writer.WriteString(L"</threadedComment>");
		}
		void CThreadedComment::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				else if ( _T("mentions") == sName )
					m_oMentions = oReader;
			}
			PrepareText();
		}
		EElementType CThreadedComment::getType () const
		{
			return et_x_ThreadedComment;
		}
		bool CThreadedComment::operator < (const CThreadedComment& elem) const
		{
			if(dT.IsInit() && elem.dT.IsInit())
			{
				return dT->GetValue() < elem.dT->GetValue();
			}
			return true;
		}
		void CThreadedComment::PrepareText ()
		{
			//last '\n' not in format but excel add it
			if(m_oText.IsInit())
			{
				if(m_oText->m_sText.length() > 0 && '\n' == m_oText->m_sText[m_oText->m_sText.length() - 1])
					m_oText->m_sText.pop_back();
				if(m_oText->m_sText.length() > 0 && '\r' == m_oText->m_sText[m_oText->m_sText.length() - 1])
					m_oText->m_sText.pop_back();
			}
		}
		bool CThreadedComment::Compare (CThreadedComment* val1, CThreadedComment* val2)
		{
			return *val1 < *val2;
		}
		void CThreadedComment::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"ref",      ref )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"dT",		dT )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"personId", personId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",		id )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"parentId",	parentId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"done",		done )


				WritingElement_ReadAttributes_End( oReader )

				//todo IsZero() is added to fix comments with zero ids(5.4.0)(bug 42947). Remove after few releases
				if(id.IsInit() && id->IsZero())
				{
					id = L"{" + XmlUtils::GenerateGuid() + L"}";
				}
			}

		CThreadedComments::CThreadedComments(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
			{
				xlsx->m_arWorksheets.back()->m_pThreadedComments = this;
			}
		}
		CThreadedComments::CThreadedComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
			{
				xlsx->m_arWorksheets.back()->m_pThreadedComments = this;
			}
			read( oRootPath, oPath );
		}
		CThreadedComments::~CThreadedComments()
		{
		}
		void CThreadedComments::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CThreadedComments::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if ( L"ThreadedComments" == sName )
			{
				fromXML(oReader);
			}
		}
		void CThreadedComments::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CThreadedComments::toXML() const
		{
			return L"";
		}
		void CThreadedComments::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<ThreadedComments");
			writer.WriteString(L" xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2018/threadedcomments\"");
			writer.WriteString(L" xmlns:x=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"");
			writer.WriteString(L">");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</ThreadedComments>");
		}
		void CThreadedComments::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"threadedComment" == sName )
				{
					CThreadedComment* pThreadedComment = new CThreadedComment(oReader);
					m_arrItems.push_back(pThreadedComment);
				}
			}
			PrepareTopLevelComments();
		}
		void CThreadedComments::PrepareTopLevelComments ()
		{
			//find TopLevelComments
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				CThreadedComment* pThreadedComment = m_arrItems[i];
				if(pThreadedComment->id.IsInit() && !pThreadedComment->parentId.IsInit())
				{
					m_mapTopLevelThreadedComments[pThreadedComment->id->ToString()] = pThreadedComment;
				}
			}
			//to remove reply duplicates
			std::unordered_map<std::wstring, bool> mapUniqueue;
			//add Replies
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				CThreadedComment* pThreadedComment = m_arrItems[i];
				if(pThreadedComment->parentId.IsInit())
				{
					//todo IsZero() is added to fix comments with zero ids(5.4.0)(bug 42947). Remove after few releases
					if(pThreadedComment->parentId->IsZero() && pThreadedComment->ref.IsInit())
					{
						if (pThreadedComment->dT.IsInit() && mapUniqueue.find(pThreadedComment->dT->ToString()) == mapUniqueue.end())
						{
							mapUniqueue[pThreadedComment->dT->ToString()] = true;
							//find parents by ref
							for (std::unordered_map<std::wstring, CThreadedComment*>::const_iterator it = m_mapTopLevelThreadedComments.begin(); it != m_mapTopLevelThreadedComments.end(); ++it)
							{
								if (it->second->ref.IsInit() && pThreadedComment->ref.get() == it->second->ref.get())
								{
									it->second->m_arrReplies.push_back(pThreadedComment);
									break;
								}
							}
						}
					}
					else
					{
						std::unordered_map<std::wstring, CThreadedComment*>::const_iterator oFind = m_mapTopLevelThreadedComments.find(pThreadedComment->parentId->ToString());
						if(m_mapTopLevelThreadedComments.end() != oFind)
						{
							oFind->second->m_arrReplies.push_back(pThreadedComment);
						}
					}
				}
			}
			//sort Replies
			for (std::unordered_map<std::wstring, CThreadedComment*>::const_iterator it = m_mapTopLevelThreadedComments.begin(); it != m_mapTopLevelThreadedComments.end(); ++it)
			{
				std::sort (it->second->m_arrReplies.begin(), it->second->m_arrReplies.end(), CThreadedComment::Compare);
			}
		}
		EElementType CThreadedComments::getType () const
		{
			return et_x_ThreadedComments;
		}
		void CThreadedComments::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

			toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		}
		const OOX::FileType CThreadedComments::type() const
		{
			return OOX::Spreadsheet::FileTypes::ThreadedComments;
		}
		const CPath CThreadedComments::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CThreadedComments::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CThreadedComments::GetReadPath()
		{
			return m_oReadPath;
		}
		void CThreadedComments::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
