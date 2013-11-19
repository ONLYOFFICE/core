#pragma once
#ifndef OOX_GRAPHICFRAME_FILE_INCLUDE_H_
#define OOX_GRAPHICFRAME_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CGraphicChart : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicChart)
			CGraphicChart()
			{
			}
			virtual ~CGraphicChart()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
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
				return et_Blip;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId>				m_oRId;
		};
		class CGraphicData : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicData)
			CGraphicData()
			{
			}
			virtual ~CGraphicData()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
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

					if ( _T("c:chart") == sName )
						m_oChart = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicData;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CGraphicChart>		m_oChart;
		};
		class CChartGraphic : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartGraphic)
			CChartGraphic()
			{
			}
			virtual ~CChartGraphic()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
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

					if ( _T("a:graphicData") == sName )
						m_oGraphicData = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicFrame;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CGraphicData>		m_oGraphicData;
		};
		class CGraphicFrame : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicFrame)
			CGraphicFrame()
			{
			}
			virtual ~CGraphicFrame()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				CString sRes;
				if(m_oChartGraphic.IsInit() && m_oChartGraphic->m_oGraphicData.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId.IsInit())
					sRes.Format(_T("<xdr:graphicFrame macro=\"\"><xdr:nvGraphicFramePr><xdr:cNvPr id=\"1\" name=\"diagram\"/><xdr:cNvGraphicFramePr/></xdr:nvGraphicFramePr><xdr:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/></xdr:xfrm><a:graphic><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"><c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"%s\"/></a:graphicData></a:graphic></xdr:graphicFrame><xdr:clientData/>"), m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId->ToString());
				writer.WriteStringC(sRes);
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				m_sXml.Init();
				m_sXml->Append(oReader.GetOuterXml());
				CString sXml;
				sXml.Format(_T("<root xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">%s</root>"), m_sXml.get());
				XmlUtils::CXmlLiteReader oSubReader;
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//GraphicFrame

				int nCurDepth = oSubReader.GetDepth();
				while( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oSubReader.GetName();

					if ( _T("a:graphic") == sName )
						m_oChartGraphic = oSubReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicFrame;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CChartGraphic>		m_oChartGraphic;
			nullable<CString> m_sXml;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_GRAPHICFRAME_FILE_INCLUDE_H_