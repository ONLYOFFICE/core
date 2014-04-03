#pragma once
#ifndef OOX_CHARTLEGEND_FILE_INCLUDE_H_
#define OOX_CHARTLEGEND_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartManualLayout : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartManualLayout)
			CChartManualLayout() {}
			virtual ~CChartManualLayout() {}

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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("c:h") == sName )
						m_oH = oReader;
					else if ( _T("c:hMode") == sName )
						m_oHMode = oReader;
					else if ( _T("c:layoutTarget") == sName )
						m_oLayoutTarget = oReader;
					else if ( _T("c:w") == sName )
						m_oW = oReader;
					else if ( _T("c:wMode") == sName )
						m_oWMode = oReader;
					else if ( _T("c:x") == sName )
						m_oX = oReader;
					else if ( _T("c:xMode") == sName )
						m_oXMode = oReader;
					else if ( _T("c:y") == sName )
						m_oY = oReader;
					else if ( _T("c:yMode") == sName )
						m_oYMode = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_ManualLayout;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:

			// Attributes
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oH;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oHMode;
			nullable<ComplexTypes::Spreadsheet::CChartLayoutTarget> m_oLayoutTarget;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oW;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oWMode;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oX;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oXMode;
			nullable<ComplexTypes::Spreadsheet::CDouble> m_oY;
			nullable<ComplexTypes::Spreadsheet::CChartHMode> m_oYMode;
		};
		class CChartLayout : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartLayout)
			CChartLayout() {}
			virtual ~CChartLayout() {}

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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("c:manualLayout") == sName )
						m_oManualLayout = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_Layout;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
			//Childs
			nullable<CChartManualLayout> m_oManualLayout;
		};
		class CChartLegendPos : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartLegendPos)
			CChartLegendPos() {}
			virtual ~CChartLegendPos() {}

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
			virtual EElementType getType() const
			{
				return et_c_LegendPos;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVal )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::Spreadsheet::CChartLegendPos<> > m_oVal;
		};
		class CChartLegendEntry : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartLegendEntry)
			CChartLegendEntry() {}
			virtual ~CChartLegendEntry() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oIndex.IsInit() && m_oIndex->m_oVal.IsInit() && (m_oDelete.IsInit() || m_oTxPr.IsInit()))
				{
					writer.WriteString(_T("<c:legendEntry>"));
					if(m_oIndex.IsInit())
					{
						CString sXml;
						sXml.Format(_T("<c:idx val=\"%d\"/>"), m_oIndex->m_oVal->GetValue());
						writer.WriteString(sXml);
					}
					if(m_oDelete.IsInit())
					{
						CString sXml;
						sXml.Format(_T("<c:delete val=\"%s\"/>"), m_oDelete->m_oVal.ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sXml);
					}
					if(m_oTxPr.IsInit() && m_oTxPr->m_oXml.IsInit())
						writer.WriteString(m_oTxPr->m_oXml.get2());
					writer.WriteString(_T("</c:legendEntry>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("c:idx") == sName )
						m_oIndex = oReader;
					else if ( _T("c:delete") == sName )
						m_oDelete = oReader;
					else if ( _T("c:txPr") == sName )
						m_oTxPr = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_LegendEntry;
			}
		public:
			nullable<ComplexTypes::Spreadsheet::CDecimalNumber> m_oIndex;
			nullable<ComplexTypes::Spreadsheet::COnOff2<>> m_oDelete;
			nullable<OOX::Spreadsheet::CChartRich > m_oTxPr;
		};
		class CChartLegend : public WritingElementWithChilds<CChartLegendEntry>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartLegend)
			CChartLegend()
			{
			}
			virtual ~CChartLegend()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(CString(_T("<c:legend>")));
				CString sLegendPos = _T("r");
				if(m_oLegendPos.IsInit() && m_oLegendPos->m_oVal.IsInit())
					sLegendPos = m_oLegendPos->m_oVal->ToString();
				CString sLegendPosXml;sLegendPosXml.Format(_T("<c:legendPos val=\"%s\"/>"), sLegendPos);
				for(int i = 0 , length = m_arrItems.GetSize(); i < length; ++i)
				{
					m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(sLegendPosXml);
				writer.WriteString(CString(_T("<c:layout/>")));
				CString sOverlay = _T("0");
				if(m_oOverlay.IsInit())
					sOverlay = m_oOverlay->m_oVal.ToString2(SimpleTypes::onofftostring1);
				CString sOverlayXml;sOverlayXml.Format(_T("<c:overlay val=\"%s\"/>"), sOverlay);
				writer.WriteString(sOverlayXml);
				if(m_oTxPr.IsInit())
					m_oTxPr->toXML(writer);
				writer.WriteString(CString(_T("</c:legend>")));
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

					if ( _T("c:overlay") == sName )
						m_oOverlay = oReader;
					else if ( _T("c:layout") == sName )
						m_oLayout = oReader;
					else if ( _T("c:legendPos") == sName )
						m_oLegendPos = oReader;
					else if ( _T("c:legendEntry") == sName )
						m_arrItems.Add(new CChartLegendEntry(oReader));
					else if ( _T("c:txPr") == sName )
						m_oTxPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_c_Legend;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2<>>	m_oOverlay;
			nullable<CChartLegendPos>						m_oLegendPos;
			nullable<CChartLayout>							m_oLayout;
			nullable<CChartRich>							m_oTxPr;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CHARTLEGEND_FILE_INCLUDE_H_