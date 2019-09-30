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
#ifndef OOX_XLSXTREADEDCOMMENTS_FILE_INCLUDE_H_
#define OOX_XLSXTREADEDCOMMENTS_FILE_INCLUDE_H_

#include <unordered_map>

#include "../Xlsx.h"
#include "../Workbook/Workbook.h"
#include "../SharedStrings/Si.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPerson : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPerson)
			CPerson()
			{
			}
			virtual ~CPerson()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<person");
					WritingStringNullableAttrEncodeXmlString(L"displayName", displayName, *displayName);
					WritingStringNullableAttrString(L"id", id, *id);
					WritingStringNullableAttrString(L"userId", userId, *userId);
					WritingStringNullableAttrEncodeXmlString(L"providerId", providerId, *providerId);
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_ThreadedComment;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"displayName",	displayName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"userId",		userId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"providerId",	providerId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",			id )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:		
			nullable_string displayName;
			nullable_string userId;
			nullable_string providerId;
			nullable_string id;

			nullable<OOX::Drawing::COfficeArtExtensionList> extLst;
		};

		class CPersonList : public OOX::File, public WritingElementWithChilds<CPerson>
		{
		public:
			CPersonList(OOX::Document* pMain) : OOX::File(pMain)
			{
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && xlsx->m_pWorkbook)
				{
					xlsx->m_pWorkbook->m_pPersonList = this;
				}
			}
			CPersonList(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
			{
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && xlsx->m_pWorkbook)
				{
					xlsx->m_pWorkbook->m_pPersonList = this;
				}
				read( oRootPath, oPath );
			}
			virtual ~CPersonList()
			{
				ClearItems();
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
						CPerson* pPerson = new CPerson(oReader);
						if(pPerson->id.IsInit())
						{
							m_mapPersonList[pPerson->id.get()] = pPerson;
						}
						m_arrItems.push_back(pPerson);
					}
				}
			}
			virtual EElementType getType () const
			{
				return et_x_PersonList;
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

				toXML(sXml);

				std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Persons;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
		public:
			std::unordered_map<std::wstring, CPerson*> m_mapPersonList;
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CThreadedCommentMention : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CThreadedCommentMention)
			CThreadedCommentMention()
			{
			}
			virtual ~CThreadedCommentMention()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<mention");
					WritingStringNullableAttrString(L"mentionpersonId", mentionpersonId, mentionpersonId->ToString());
					WritingStringNullableAttrString(L"mentionId", mentionId, mentionId->ToString());
					WritingStringNullableAttrInt(L"startIndex", startIndex, startIndex->GetValue());
					WritingStringNullableAttrInt(L"length", length, length->GetValue());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_ThreadedCommentMention;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"mentionpersonId",  mentionpersonId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"mentionId",		mentionId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"startIndex",		startIndex )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"length",			length )

				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CGuid>						mentionpersonId;
			nullable<SimpleTypes::CGuid>						mentionId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		startIndex;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		length;
		};
		class CThreadedCommentMentions : public WritingElementWithChilds<CThreadedCommentMention>
		{
		public:
			WritingElement_AdditionConstructors(CThreadedCommentMentions)
			CThreadedCommentMentions()
			{
			}
			virtual ~CThreadedCommentMentions()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_ThreadedCommentMentions;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CThreadedComment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CThreadedComment)
			CThreadedComment()
			{
			}
			virtual ~CThreadedComment()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_ThreadedComment;
			}
			bool operator < (const CThreadedComment& elem) const
			{
				if(dT.IsInit() && elem.dT.IsInit())
				{
					return dT->GetValue() < elem.dT->GetValue();
				}
				return true;
			}
			void PrepareText ()
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
			static bool Compare (CThreadedComment* val1, CThreadedComment* val2)
			{
				return *val1 < *val2;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		public:
			nullable_string						ref;
			nullable<SimpleTypes::CDateTime>	dT;
			nullable<SimpleTypes::CGuid>		personId;
			nullable<SimpleTypes::CGuid>		id;
			nullable<SimpleTypes::CGuid>		parentId;
			nullable_bool						done;

			nullable<CText>						m_oText;
			nullable<CThreadedCommentMentions>	m_oMentions;

			std::vector<CThreadedComment*>  m_arrReplies;
		};
        class CThreadedComments : public OOX::FileGlobalEnumerated, public OOX::IFileContainer, public WritingElementWithChilds<CThreadedComment>
		{
		public:
            CThreadedComments(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
			{
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pThreadedComments = this;
				}
			}
            CThreadedComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
			{
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pThreadedComments = this;
				}
				read( oRootPath, oPath );
			}
			virtual ~CThreadedComments()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void PrepareTopLevelComments ()
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
			virtual EElementType getType () const
			{
				return et_x_ThreadedComments;
			}

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

				toXML(sXml);

				std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::ThreadedComments;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
		public:
			std::unordered_map<std::wstring, CThreadedComment*> m_mapTopLevelThreadedComments;
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_XLSXTREADEDCOMMENTS_FILE_INCLUDE_H_
