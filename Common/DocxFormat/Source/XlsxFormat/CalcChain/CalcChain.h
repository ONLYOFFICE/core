#pragma once
#ifndef OOX_CALCCHAIN_FILE_INCLUDE_H_
#define OOX_CALCCHAIN_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CCalcCell : public WritingElementWithChilds<>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCalcCell)
			CCalcCell()
			{
			}
			virtual ~CCalcCell()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_CalcCell;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("a"),      m_oArray )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("i"),      m_oSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("l"),      m_oDependencyLevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("s"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("t"),      m_oRef )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>					m_oArray;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oSheetId;
			nullable<SimpleTypes::COnOff<>>					m_oDependencyLevel;
			nullable<CString>								m_oRef;
			nullable<SimpleTypes::COnOff<>>					m_oChildChain;
			nullable<SimpleTypes::COnOff<>>					m_oNewThread;
		};
		//необработанные child:
		//<extLst>
		class CCalcChain : public OOX::File, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CCalcChain()
			{
			}
			CCalcChain(const CPath& oPath)
			{
				read( oPath );
			}
			virtual ~CCalcChain()
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
				if ( _T("calcChain") == sName )
				{
					ReadAttributes( oReader );

					if ( !oReader.IsEmptyNode() )
					{
						int nStylesDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
						{
							sName = oReader.GetName();

							if ( _T("c") == sName )
								m_arrItems.push_back(new CCalcCell(oReader));
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::CalcChain;
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
			void ClearItems()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.clear();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
			std::vector<CCalcCell *>         m_arrItems;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CALCCHAIN_FILE_INCLUDE_H_