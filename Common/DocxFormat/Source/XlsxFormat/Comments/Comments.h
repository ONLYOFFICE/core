/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../CommonInclude.h"
#include "../SharedStrings/Si.h"

#include "../../DocxFormat/IFileContainer.h"

#include <map>

namespace OOX
{
	namespace Spreadsheet
	{
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
			nullable<double> m_dLeftMM;
			nullable<double> m_dTopMM;
			nullable<double> m_dWidthMM;
			nullable<double> m_dHeightMM;
			nullable<std::wstring> m_sAuthor;
			nullable<unsigned int> m_nRow;
			nullable<unsigned int> m_nCol;
			nullable<bool> m_bMove;
			nullable<bool> m_bSize;
			nullable<CSi> m_oText;
			nullable<CString> m_sGfxdata;
			CCommentItem()
			{
			}
			bool IsValid()
			{
				return m_nRow.IsInit() && m_nCol.IsInit() && m_sAuthor.IsInit();
			}
		};
		class CAuthors : public WritingElementWithChilds<std::wstring>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CAuthors)
			CAuthors()
			{
			}
			virtual ~CAuthors()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<authors>");
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
				{
					writer.WriteString(L"<author>");
					writer.WriteEncodeXmlString(*m_arrItems[i]);
					writer.WriteString(L"</author>");
				}
				writer.WriteString(L"</authors>");
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("author") == sName )
						m_arrItems.push_back(new std::wstring(oReader.GetText3()));
				}
			}

			virtual EElementType getType () const
			{
				return et_Authors;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CComment : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CComment)
			CComment()
			{
			}
			virtual ~CComment()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oRef.IsInit() && m_oAuthorId.IsInit() && m_oText.IsInit())
				{
					writer.WriteString(L"<comment");
					WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef->ToString2());
					WritingStringNullableAttrInt(L"authorId", m_oAuthorId, m_oAuthorId->GetValue());
					writer.WriteString(L">");
					
					writer.WriteString(L"<text>");
					m_oText->toXML2(writer);
					writer.WriteString(L"</text>");
					writer.WriteString(L"</comment>");
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("text") == sName )
						m_oText  =oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_Comment;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("authorId"),      m_oAuthorId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oAuthorId;

			nullable<CSi> m_oText;
		};
		class CCommentList : public WritingElementWithChilds<CComment>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCommentList)
			CCommentList()
			{
			}
			virtual ~CCommentList()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<commentList>");
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
				{
					m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(L"</commentList>");
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("comment") == sName )
						m_arrItems.push_back(new CComment(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_CommentList;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CComments : public OOX::FileGlobalEnumerated, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CComments()
			{
			}
			CComments(const CPath& oRootPath, const CPath& oPath)
			{
				read( oRootPath, oPath );
			}
			virtual ~CComments()
			{
			}
		public:

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

				CString sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( _T("comments") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("authors") == sName )
								m_oAuthors = oReader;
							else if ( _T("commentList") == sName )
								m_oCommentList = oReader;
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><comments xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"));
				if(m_oAuthors.IsInit())
					m_oAuthors->toXML(sXml);
				if(m_oCommentList.IsInit())
					m_oCommentList->toXML(sXml);
				sXml.WriteString(_T("</comments>"));

				CString sPath = oPath.GetPath();
				NSFile::CFileBinary::SaveToFile(sPath.GetBuffer(), sXml.GetData());
				sPath.ReleaseBuffer();
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
			WritingElementSpreadsheet_AdditionConstructors(CLegacyDrawingWorksheet)
			CLegacyDrawingWorksheet()
			{
			}
			virtual ~CLegacyDrawingWorksheet()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oId.IsInit())
				{
					writer.WriteString(L"<legacyDrawing r:id=\"");
					writer.WriteString(m_oId->ToString2());
					writer.WriteString(L"\"/>");					
				}
				
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_LegacyDrawingWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_XLSXCOMMENTS_FILE_INCLUDE_H_
