#pragma once
#ifndef OOX_SHAREDSTRINGS_FILE_INCLUDE_H_
#define OOX_SHAREDSTRINGS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Si.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработанные child:
		class CSharedStrings : public OOX::File, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CSharedStrings()
			{
			}
			CSharedStrings(const CPath& oPath)
			{
				read( oPath );
			}
			virtual ~CSharedStrings()
			{
				ClearItems();
			}
		public:

			virtual void read(const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				CWCharWrapper sName = oReader.GetName();
				if ( _T("sst") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nSharedStringsDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nSharedStringsDepth ) )
						{
							sName = oReader.GetName();

							WritingElement *pItem = NULL;

							if ( _T("si") == sName )
								pItem = new CSi( oReader );

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				XmlUtils::CStringWriter sXml;
				sXml.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\""));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					sXml.WriteString(sVal);
				}
				if(m_oUniqueCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" uniqueCount=\"%d\""), m_oUniqueCount->GetValue());
					sXml.WriteString(sVal);
				}
				sXml.WriteString(_T(">"));

				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(sXml);

				sXml.WriteString(_T("</sst>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetData() );
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::SharedStrings;
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
			const int AddSi(CSi* pSi)
			{
				int nIndex = m_arrItems.size();
				m_arrItems.push_back( pSi );
				return nIndex;
			}
		private:
			CPath									m_oReadPath;
			void ClearItems()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}
				m_arrItems.clear();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("uniqueCount"),      m_oUniqueCount )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oUniqueCount;

			std::vector<WritingElement *>         m_arrItems;

		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHAREDSTRINGS_FILE_INCLUDE_H_