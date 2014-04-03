#pragma once
#ifndef OOX_CHARTPLOTAREA_FILE_INCLUDE_H_
#define OOX_CHARTPLOTAREA_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "Title.h"
#include "BasicChart.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartCatAx : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartCatAx)
			CChartCatAx() {}
			virtual ~CChartCatAx() {}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			void toXML2(CString& sDelete, CString& sMajorGrid, CString& sAxTitle, CString& sTxPr) const
			{
				if(m_oDelete.IsInit())
					sDelete.Format(_T("<c:delete val=\"%s\"/>"), m_oDelete->m_oVal.ToString2(SimpleTypes::onofftostring1));
				if(m_oAxPos.IsInit())
				{
					CString sAxPos;
					sAxPos.Format(_T("<c:axPos %s/>"), m_oAxPos->ToString());
				}
				if(m_oMajorGridlines.IsInit() && true == m_oMajorGridlines->ToBool())
					sMajorGrid = _T("<c:majorGridlines/>");
				if(m_oTitle.IsInit())
				{
					XmlUtils::CStringWriter temp;
					m_oTitle->toXML(temp);
					sAxTitle = temp.GetData();
				}

				//CStringWriter temp;
				//if(m_oNumFmt.IsInit())
				//	m_oNumFmt->toXML2(temp, CString(_T("c:numFmt")));
				//sAxNumFmt = temp.GetData();
				if(m_oTxPr.IsInit() && m_oTxPr->m_oXml.IsInit())
				{
					sTxPr = m_oTxPr->m_oXml.get();
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

					if ( _T("c:title") == sName )
						m_oTitle = oReader;
					else if ( _T("c:delete") == sName )
						m_oDelete = oReader;
					else if ( _T("c:majorGridlines") == sName )
					{
						m_oMajorGridlines.Init();
						m_oMajorGridlines->SetValue(SimpleTypes::onoffTrue);
					}
					else if ( _T("c:axPos") == sName )
						m_oAxPos = oReader;
					else if ( _T("c:numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("c:txPr") == sName )
						m_oTxPr = oReader;
				}
			}
			virtual EElementType getType() const
			{
				return et_c_CatAx;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
			nullable<CChartTitle> m_oTitle;
			nullable<ComplexTypes::Spreadsheet::COnOff2<>> m_oDelete;
			nullable<SimpleTypes::COnOff<>> m_oMajorGridlines;
			nullable<ComplexTypes::Spreadsheet::CChartAxPos> m_oAxPos;
			nullable<OOX::Spreadsheet::CNumFmt> m_oNumFmt;
			nullable<OOX::Spreadsheet::CChartRich> m_oTxPr;
		};
		class CChartPlotArea : public WritingElementWithChilds<CChartCatAx>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartPlotArea)
			CChartPlotArea()
			{
			}
			virtual ~CChartPlotArea()
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
			void toXML2(bool bScatter, bool bLine, CString& sBarDir, CString& sGrouping, CString& sSeries, CString& sDataLabels, CString& sOverlap, CString& sCatAxDelete, CString& sCatAxMajorGrid, CString& sCatAxTitle, CString& sCatTxPr, CString& sValAxDelete, CString& sValAxMajorGrid, CString& sValAxTitle, CString& sValTxPr) const
			{
				if(m_oBasicChart.IsInit())
					m_oBasicChart->toXML2(bScatter, bLine, sBarDir, sGrouping, sSeries, sDataLabels, sOverlap);
				if(m_oCatAx.IsInit())
				{
					m_oCatAx->toXML2(sCatAxDelete, sCatAxMajorGrid, sCatAxTitle, sCatTxPr);
					if(m_arrItems.GetSize() > 0)
						m_arrItems[0]->toXML2(sValAxDelete, sValAxMajorGrid, sValAxTitle, sValTxPr);
				}
				else
				{
					if(m_arrItems.GetSize() > 0)
						m_arrItems[0]->toXML2(sCatAxDelete, sCatAxMajorGrid, sCatAxTitle, sCatTxPr);
					if(m_arrItems.GetSize() > 1)
						m_arrItems[1]->toXML2(sValAxDelete, sValAxMajorGrid, sValAxTitle, sValTxPr);
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

					if ( _T("c:barChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicBarChart);
					}
					else if ( _T("c:bar3DChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicBar3DChart);
					}
					else if ( _T("c:lineChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicLineChart);
					}
					else if ( _T("c:line3DChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicLine3DChart);
					}
					else if ( _T("c:areaChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicAreaChart);
					}
					else if ( _T("c:area3DChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicArea3DChart);
					}
					else if ( _T("c:pieChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicPieChart);
					}
					else if ( _T("c:pie3DChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicPie3DChart);
					}
					else if ( _T("c:bubbleChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicBubbleChart);
					}
					else if ( _T("c:scatterChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicScatterChart);
					}
					else if ( _T("c:radarChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicRadarChart);
					}
					else if ( _T("c:doughnutChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicDoughnutChart);
					}
					else if ( _T("c:stockChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicStockChart);
					}
					else if ( _T("c:surfaceChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicSurfaceChart);
					}
					else if ( _T("c:surface3DChart") == sName )
					{
						m_oBasicChart = oReader;
						m_oBasicChart->setBasicType(OOX::Spreadsheet::chartbasicSurface3DChart);
					}
					else if ( _T("c:catAx") == sName || _T("c:dateAx") == sName )
						m_oCatAx = oReader;
					else if ( _T("c:valAx") == sName )
						m_arrItems.Add( new CChartCatAx(oReader));
					else if ( _T("c:serAx") == sName )
						m_oSerAx = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_c_PlotArea;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CChartCatAx>		m_oCatAx;
			nullable<CChartCatAx>		m_oSerAx;
			nullable<CChartBasicChart>	m_oBasicChart;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CHARTPLOTAREA_FILE_INCLUDE_H_