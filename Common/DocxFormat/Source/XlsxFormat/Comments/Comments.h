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
#ifndef OOX_XLSXCOMMENTS_FILE_INCLUDE_H_
#define OOX_XLSXCOMMENTS_FILE_INCLUDE_H_

#include "../Xlsx.h"
#include "../Worksheets/Worksheet.h"
#include "../SharedStrings/Si.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CThreadedComment;
		class CCommentItem
		{
		public:
			nullable<unsigned int> m_nLeft;
			nullable<unsigned int> m_nTop;
			nullable<unsigned int> m_nRight;
			nullable<unsigned int> m_nBottom;
			nullable<unsigned int> m_nLeftOffset;
			nullable<unsigned int> m_nTopOffset;
			nullable<unsigned int> m_nRightOffset;
			nullable<unsigned int> m_nBottomOffset;
			nullable_double	m_dLeftMM;
			nullable_double m_dTopMM;
			nullable_double m_dWidthMM;
			nullable_double m_dHeightMM;
			nullable_string	m_sAuthor;
			nullable<unsigned int> m_nRow;
			nullable<unsigned int> m_nCol;
			nullable_bool	m_bMove;
			nullable_bool	m_bSize;
			nullable<CSi>	m_oText;
			CThreadedComment*	m_pThreadedComment;
			bool m_bThreadedCommentCopy;
			nullable_string m_sGfxdata;
			nullable_bool	m_bVisible;
			nullable_string m_sFillColorRgb;
			CCommentItem()
			{
				m_pThreadedComment = NULL;
				m_bThreadedCommentCopy = false;
			}
			bool IsValid()
			{
				return m_nRow.IsInit() && m_nCol.IsInit() && m_sAuthor.IsInit();
			}
		};
		class CAuthors : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAuthors)
			CAuthors()
			{
			}
			virtual ~CAuthors()
			{
				ClearItems();
			}
			virtual void ClearItems()
			{
				m_arrItems.clear();
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
				writer.WriteString(L"<authors>");

				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					writer.WriteString(L"<author>");
						writer.WriteEncodeXmlString(m_arrItems[i]);
					writer.WriteString(L"</author>");
				}
				writer.WriteString(L"</authors>");
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

					if ( L"author" == sName )
					{
						m_arrItems.push_back(oReader.GetText3());
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Authors;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			std::vector<std::wstring>  m_arrItems;
		};
		class CComment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CComment)
			CComment()
			{
			}
			virtual ~CComment()
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
						m_oText  =oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Comment;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"ref",      m_oRef )
					WritingElement_ReadAttributes_Read_if ( oReader, L"authorId", m_oAuthorId )
					WritingElement_ReadAttributes_Read_if ( oReader, L"xr:uid", m_oUid )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oAuthorId;
			nullable<SimpleTypes::CGuid > m_oUid;

			nullable<CSi> m_oText;
		};
		class CCommentList : public WritingElementWithChilds<CComment>
		{
		public:
			WritingElement_AdditionConstructors(CCommentList)
			CCommentList()
			{
			}
			virtual ~CCommentList()
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"comment" == sName )
						m_arrItems.push_back(new CComment(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_CommentList;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CComments : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CComments(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pComments = this;
				}
			}
			CComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if ((xlsx) && (!xlsx->m_arWorksheets.empty()))
				{
					xlsx->m_arWorksheets.back()->m_pComments = this;
				}
				read( oRootPath, oPath );
			}
			virtual ~CComments()
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><comments xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:xr=\"http://schemas.microsoft.com/office/spreadsheetml/2014/revision\" mc:Ignorable=\"xr\">");
				if(m_oAuthors.IsInit())
					m_oAuthors->toXML(sXml);
				if(m_oCommentList.IsInit())
					m_oCommentList->toXML(sXml);
				sXml.WriteString(L"</comments>");

				std::wstring sPath = oPath.GetPath();
				NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Comments;
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
		public:
			nullable<CAuthors >		m_oAuthors;
			nullable<CCommentList > m_oCommentList;
		};
		class CLegacyDrawingWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLegacyDrawingWorksheet)
			CLegacyDrawingWorksheet()
			{
			}
			virtual ~CLegacyDrawingWorksheet()
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
				if(m_oId.IsInit())
				{
					writer.WriteString(L"<legacyDrawing r:id=\"");
					writer.WriteString(m_oId->ToString());
					writer.WriteString(L"\"/>");					
				}
				
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_LegacyDrawingWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"r:id", m_oId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_XLSXCOMMENTS_FILE_INCLUDE_H_
