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
			nullable<CString> m_sAuthor;
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
		class CAuthors : public WritingElementWithChilds<CString>
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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(CString("<authors>"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
				{
					CString sAuthor;
					sAuthor.Append(_T("<author>"));
					sAuthor.Append(XmlUtils::EncodeXmlString(*m_arrItems[i]));
					sAuthor.Append(_T("</author>"));
					writer.WriteString(sAuthor);
				}
				writer.WriteString(CString("</authors>"));
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
						m_arrItems.push_back(new CString(oReader.GetText2()));
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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oRef.IsInit() && m_oAuthorId.IsInit() && m_oText.IsInit())
				{
					writer.WriteString(CString("<comment"));
					if(m_oRef.IsInit())
					{
						CString sRef;sRef.Format(_T(" ref=\"%ls\""), XmlUtils::EncodeXmlString(m_oRef->GetValue()));
						writer.WriteString(sRef);
					}
					if(m_oAuthorId.IsInit())
					{
						CString sAuthorId;sAuthorId.Format(_T(" authorId=\"%d\""), m_oAuthorId->GetValue());
						writer.WriteString(sAuthorId);
					}
					writer.WriteString(CString(">"));
					
					writer.WriteString(CString("<text>"));
					m_oText->toXML2(writer);
					writer.WriteString(CString("</text>"));
					writer.WriteString(CString("</comment>"));
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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(CString("<commentList>"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
				{
					m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(CString("</commentList>"));
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
				XmlUtils::CStringWriter sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><comments xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"));
				if(m_oAuthors.IsInit())
					m_oAuthors->toXML(sXml);
				if(m_oCommentList.IsInit())
					m_oCommentList->toXML(sXml);
				sXml.WriteString(_T("</comments>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
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
			CPath									m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CAuthors > m_oAuthors;
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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oId.IsInit())
				{
					CString sVal;sVal.Format(_T("<legacyDrawing r:id=\"%ls\"/>"), m_oId->GetValue());
					writer.WriteString(sVal);
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
				return et_FromTo;
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
