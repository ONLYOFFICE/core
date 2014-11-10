#pragma once
#ifndef OOX_GRAPHICFRAME_FILE_INCLUDE_H_
#define OOX_GRAPHICFRAME_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// 20.1.2.2.5 cNvGraphicFramePr (Non-Visual Graphic Frame Drawing Properties)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualGraphicFrameProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnectionNonVisualGraphicFrameProps)
			CConnectionNonVisualGraphicFrameProps()
			{
			}
			virtual ~CConnectionNonVisualGraphicFrameProps()
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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("picLocks") == sName )
						m_oPicLocks = oReader;
					else if ( _T("extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_ConnectionNonVisualGraphicFrameProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), m_oPreferRelativeResize )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue> m_oPreferRelativeResize;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>         m_oPicLocks;
		};
		//--------------------------------------------------------------------------------
		// 20.1.2.2.26 nvGraphicFramePr (Non-Visual Properties for a Graphic Frame)
		//--------------------------------------------------------------------------------	
		class CGraphicFrameNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicFrameNonVisual)
			CGraphicFrameNonVisual()
			{
			}
			virtual ~CGraphicFrameNonVisual()
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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cNvGraphicFramePr") == sName )
						m_oCNvGraphicFramePr = oReader;
					else if ( _T("cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_GraphicFrameNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<CConnectionNonVisualGraphicFrameProps>	m_oCNvGraphicFramePr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};

		//"c:chart"  - нужно перенести в общую часть 
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
				return et_GraphicChart;
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

		//--------------------------------------------------------------------------------
		// 20.1.2.2.17 graphicData (Graphic Object Data)
		//--------------------------------------------------------------------------------	
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("chart") == sName )
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
			//Any element in any namespace
		};
		
		//--------------------------------------------------------------------------------
		// 20.1.2.2.16 graphic (Graphic Object)
		//--------------------------------------------------------------------------------		
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("graphicData") == sName )
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

		//--------------------------------------------------------------------------------
		// 20.1.2.2.18 graphicFrame (Graphic Frame)
		//--------------------------------------------------------------------------------			
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
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				CString sRes;
				if(m_oChartGraphic.IsInit() && m_oChartGraphic->m_oGraphicData.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId.IsInit())
					sRes.Format(_T("<xdr:graphicFrame macro=\"\"><xdr:nvGraphicFramePr><xdr:cNvPr id=\"1\" name=\"diagram\"/><xdr:cNvGraphicFramePr/></xdr:nvGraphicFramePr><xdr:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/></xdr:xfrm><a:graphic><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"><c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"%s\"/></a:graphicData></a:graphic></xdr:graphicFrame><xdr:clientData/>"), m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId->ToString());
				writer.WriteString(sRes);
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
					CString sName = XmlUtils::GetNameNoNS(oSubReader.GetName());

					if ( _T("graphic") == sName )
						m_oChartGraphic = oSubReader;
					else if ( _T("nvGraphicFramePr") == sName )
						m_oNvGraphicFramePr = oSubReader;
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
			nullable<CChartGraphic>				m_oChartGraphic;
			nullable<CGraphicFrameNonVisual>	m_oNvGraphicFramePr;
			//xfrm
			//extLst
			nullable<CString>					m_sXml;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_GRAPHICFRAME_FILE_INCLUDE_H_