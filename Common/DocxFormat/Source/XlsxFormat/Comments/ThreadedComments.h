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

#include "../Xlsx.h"
#include "../Worksheets/Worksheet.h"
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
					WritingStringNullableAttrString(L"userId", userId, *userId);
					WritingStringNullableAttrString(L"providerId", providerId, *providerId);
					WritingStringNullableAttrString(L"id", id, *id);
				writer.WriteString(L">");
					//writer.WriteString(L"<text>");
					//m_oText->toXML2(writer);
					//writer.WriteString(L"</text>");
				writer.WriteString(L"</person>");
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
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pPersonList = this;
				}
			}
			CPersonList(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
			{
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pPersonList = this;
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
                        m_arrItems.push_back(new CPerson(oReader));
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
		private:
			CPath m_oReadPath;
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
					WritingStringNullableAttrEncodeXmlString(L"ref", ref, ref->ToString());
					WritingStringNullableAttrString(L"personId", personId, *personId);
					WritingStringNullableAttrString(L"id", id, *id);
					WritingStringNullableAttrString(L"dT", dT, *dT);
				writer.WriteString(L">");
					
					writer.WriteString(L"<text>");
					m_oText->toXML2(writer);
					writer.WriteString(L"</text>");
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
					WritingElement_ReadAttributes_Read_if		( oReader, L"ref",      ref )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"personId", personId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",		id )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"dT",		dT )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId>	ref;
			nullable_string							personId;
			nullable_string							id;
			nullable_string							dT;

			nullable<CSi> m_oText;
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
						m_arrItems.push_back(new CThreadedComment(oReader));
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
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_XLSXTREADEDCOMMENTS_FILE_INCLUDE_H_
