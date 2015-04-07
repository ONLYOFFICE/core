#pragma once
#ifndef OOX_EXTERNALLINKS_FILE_INCLUDE_H_
#define OOX_EXTERNALLINKS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CExternalBook : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalBook)
			CExternalBook()
			{
			}
			virtual ~CExternalBook()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
				
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
				}
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				CString str = toXML();
				writer.WriteString(str);
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<externalBook");

				if(m_oRid.IsInit())
				{
					CString sVal; sVal.Format(_T(" r:id=\"%ls\""), m_oRid->GetValue());
					sResult += sVal;
					sResult += _T(" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" ");
				}

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::Spreadsheet::et_ExternalBook;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("r:id"), m_oRid )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			nullable<SimpleTypes::CRelationshipId > m_oRid;
		};

		class CExternalLink : public OOX::FileGlobalEnumerated, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CExternalLink()
			{
			}
			CExternalLink(const CPath& oRootPath, const CPath& oPath)
			{
				read( oRootPath, oPath );
			}
			virtual ~CExternalLink()
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
				if ( _T("externalLink") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = XmlUtils::GetNameNoNS(oReader.GetName());

							if ( _T("externalBook") == sName )
							{
								m_oExternalBook = oReader;
							}
						}
					}
				}		

			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				XmlUtils::CStringWriter sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
				sXml.WriteString(_T("<externalLink xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"));
			
				if(m_oExternalBook.IsInit())
				{
					m_oExternalBook->toXML(sXml);
				}
				
				sXml.WriteString(_T("</externalLink>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}

			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::ExternalLinks;
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
			nullable<CExternalBook>		m_oExternalBook;
		};
	} //ExternalLink
} // namespace OOX

#endif // OOX_EXTERNALLINKS_FILE_INCLUDE_H_
