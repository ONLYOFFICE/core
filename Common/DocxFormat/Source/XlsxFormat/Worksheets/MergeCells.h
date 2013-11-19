#pragma once
#ifndef OOX_MERGECELLS_FILE_INCLUDE_H_
#define OOX_MERGECELLS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CMergeCell : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CMergeCell)
			CMergeCell()
			{
			}
			virtual ~CMergeCell()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<mergeCell"));
				if(m_oRef.IsInit())
				{
					CString sVal; sVal.Format(_T(" ref=\"%s\""), XmlUtils::EncodeXmlString(m_oRef.get()));
					writer.WriteStringC(sVal);
				}
				writer.WriteStringC(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_MergeCell;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<CString>						m_oRef;
		};

		class CMergeCells  : public WritingElementWithChilds<CMergeCell>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CMergeCells)
			CMergeCells()
			{
			}
			virtual ~CMergeCells()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				if(m_arrItems.GetSize() > 0)
				{
					writer.WriteStringC(_T("<mergeCells"));
					if(m_oCount.IsInit())
					{
						CString sVal; sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
						writer.WriteStringC(sVal);
					}
					writer.WriteStringC(_T(">"));
					for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteStringC(_T("</mergeCells>"));
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
					CWCharWrapper sName = oReader.GetName();

					if ( _T("mergeCell") == sName )
						m_arrItems.Add( new CMergeCell( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_MergeCells;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_MERGECELLS_FILE_INCLUDE_H_