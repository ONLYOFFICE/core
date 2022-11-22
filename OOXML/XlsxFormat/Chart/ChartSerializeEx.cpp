/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "ChartSerializeEx.h"
#include "../../../DesktopEditor/common/StringExt.h"
#include "../../DocxFormat/Drawing/DrawingExt.h"

namespace OOX{
namespace Spreadsheet{
namespace ChartEx{

	void CChartSpace::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"chartData" == sName)
			{
				m_chartData.fromXML(oReader);
			}
			else if( L"clrMapOvr" == sName)
			{
                m_oClrMapOvr = oReader;
			}
			else if( L"chart" == sName)
			{
				m_chart.fromXML(oReader);
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			//else if( L"printSettings" == sName)
			//{
			//	m_printSettings = new CT_PrintSettings;
			//	m_printSettings->fromXML(oReader);
			//}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CChartSpace::toXML(NSStringUtils::CStringBuilder& writer) const{
		writer.WriteString(L"<cx:chartSpace \
xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"");
		writer.WriteString(L">");

		m_chartData.toXML(writer);
		m_chart.toXML(writer);

		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if (m_oClrMapOvr.IsInit())
		{
			m_oClrMapOvr->m_name = L"cx:clrMapOvr";
			writer.WriteString(m_oClrMapOvr->toXML());
		}
		//if(NULL != m_printSettings)
		//{
  //          m_printSettings->toXML(L"cx:printSettings", writer);
		//}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:chartSpace>");
	}
	EElementType CChartSpace::getType() const {return et_ct_ChartSpace;}

//----------------------------------------------------------------------------------------------
	void CChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"title" == sName)
			{
				m_title = new CTitle;
				m_title->fromXML(oReader);
			}
			else if( L"plotArea" == sName)
			{
				m_plotArea.fromXML(oReader);
			}
			else if( L"legend" == sName)
			{
				m_legend = new CLegend;
				m_legend->fromXML(oReader);
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CChart::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:chart>");

		if(m_title.IsInit())
		{
            m_title->toXML(writer);
		}
	
		m_plotArea.toXML(writer);

		if(m_legend.IsInit())
		{
            m_legend->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:chart>");
	}
	EElementType CChart::getType() const {return et_ct_Chart;}
//----------------------------------------------------------------------------------------------
	CChartData::~CChartData()
	{
		for(size_t i = 0; i < m_arData.size(); ++i)
			delete m_arData[i];
        m_arData.clear();
	}
	void CChartData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"data" == sName)
			{
				CData *item = new CData(oReader);
				m_arData.push_back(item);
			}
			else if (L"externalData" == sName)
			{
				m_externalData = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CChartData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:chartData>");

		if (m_externalData.IsInit())
		{
			m_externalData->toXML(writer);
		}  
		for (size_t i = 0; i < m_arData.size(); ++i)
		{
			m_arData[i]->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:chartData>");
	}
	EElementType CChartData::getType() const {return et_ct_ChartData;}
//----------------------------------------------------------------------------------------------
	CNumDimension::CNumDimension()
	{
	}
	CNumDimension::~CNumDimension()
	{
		for(size_t i = 0; i < m_levelData.size(); ++i)
			delete m_levelData[i];
        m_levelData.clear();
	}
	void CNumDimension::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"f" == sName)
			{
				m_f = oReader;
			}
			else if( L"nf" == sName)
			{
				m_nf = oReader.GetText3();
			}
			else if( L"lvl" == sName)
			{
				CNumericLevel *item = new CNumericLevel(oReader);
				m_levelData.push_back(item);
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CNumDimension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"type", m_type)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CNumDimension::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:numDim");
			WritingStringNullableAttrString(L"type", m_type, m_type->ToString());
		writer.WriteString(L">");

		if(m_f.IsInit())
		{
			m_f->toXML(writer);
			//if (std::wstring::npos != m_f->find(' ') || std::wstring::npos != m_f->find('\n'))
			//	writer.WriteString(L"<c:f xml:space=\"preserve\">");
			//else
			//	writer.WriteString(L"<cx:f>");
			//writer.WriteEncodeXmlString(*m_f);
			//writer.WriteString(L"</cx:f>");
		}
		if(m_nf.IsInit())
		{
			if (std::wstring::npos != m_nf->find(' ') || std::wstring::npos != m_nf->find('\n'))
				writer.WriteString(L"<c:nf xml:space=\"preserve\">");
			else
				writer.WriteString(L"<cx:nf>");
			writer.WriteEncodeXmlString(*m_nf);
			writer.WriteString(L"</cx:nf>");
		}
        for (size_t i = 0; i < m_levelData.size(); ++i)
		{
			m_levelData[i]->toXML(writer);
		}
		if( m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:numDim>");
	}
	EElementType CNumDimension::getType() const {return et_ct_NumDimension;}
//----------------------------------------------------------------------------------------------
	CStrDimension::CStrDimension()
	{
	}
	CStrDimension::~CStrDimension()
	{
		for(size_t i = 0; i < m_levelData.size(); ++i)
			delete m_levelData[i];
        m_levelData.clear();
	}
	void CStrDimension::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"f" == sName)
			{
				m_f = oReader;
			}
			else if( L"nf" == sName)
			{
				m_nf = oReader.GetText3();
			}
			else if( L"lvl" == sName)
			{
				CStringLevel *item = new CStringLevel(oReader);
				m_levelData.push_back(item);
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CStrDimension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"type", m_type)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CStrDimension::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:strDim");
			WritingStringNullableAttrString(L"type", m_type, m_type->ToString());
		writer.WriteString(L">");

		if(m_f.IsInit())
		{
			m_f->toXML(writer);
			//if (std::wstring::npos != m_f->find(' ') || std::wstring::npos != m_f->find('\n'))
			//	writer.WriteString(L"<c:f xml:space=\"preserve\">");
			//else
			//	writer.WriteString(L"<cx:f>");
			//writer.WriteEncodeXmlString(*m_f);
			//writer.WriteString(L"</cx:f>");
		}
		if(m_nf.IsInit())
		{
			if (std::wstring::npos != m_nf->find(' ') || std::wstring::npos != m_nf->find('\n'))
				writer.WriteString(L"<c:nf xml:space=\"preserve\">");
			else
				writer.WriteString(L"<cx:nf>");
			writer.WriteEncodeXmlString(*m_nf);
			writer.WriteString(L"</cx:nf>");
		}
        for (size_t i = 0; i < m_levelData.size(); ++i)
		{
			m_levelData[i]->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:strDim>");
	}
	EElementType CStrDimension::getType() const {return et_ct_StrDimension;}
//----------------------------------------------------------------------------------------------
	CData::~CData()
	{
		for (size_t i = 0; i < m_arDimension.size(); ++i)
			delete m_arDimension[i];
        m_arDimension.clear();
	}
	void CData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"numDim" == sName)
			{
				CNumDimension *item = new CNumDimension(oReader);
				m_arDimension.push_back(dynamic_cast<CDimension*>(item));
			}
			else if( L"strDim" == sName)
			{
				CStrDimension *item = new CStrDimension(oReader);
				m_arDimension.push_back(dynamic_cast<CDimension*>(item));
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"id", m_id)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:data");
		WritingStringNullableAttrUInt(L"id", m_id, *m_id);
		writer.WriteString(L">");

        for (size_t i = 0; i < m_arDimension.size(); ++i)
		{
			m_arDimension[i]->toXML(writer);
		}
		if( m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:data>");
	}
	EElementType CData::getType() const {return et_ct_Data;}
//----------------------------------------------------------------------------------------------
	void CExternalData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}
	void CExternalData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"id", m_id)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"autoUpdate", m_autoUpdate)
		WritingElement_ReadAttributes_End_No_NS(oReader)
	}
	void CExternalData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:externalData");
			WritingStringNullableAttrString(L"r:id", m_id, *m_id);
			WritingStringNullableAttrBool2(L"cx:autoUpdate", m_autoUpdate);
		writer.WriteString(L"/>");
	}
	EElementType CExternalData::getType() const { return et_ct_ExternalData; }
//------------------------------------------------------------------------------------------------
	CPlotArea::~CPlotArea()
	{
		for(size_t i = 0; i < m_arAxis.size(); ++i)
			delete m_arAxis[i];
        m_arAxis.clear();
	}
	void CPlotArea::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if( L"plotAreaRegion" == sName)
			{
				m_plotAreaRegion.fromXML(oReader);
			}
			else if( L"axis" == sName)
			{
				CAxis *item = new CAxis(oReader);
				m_arAxis.push_back(item);
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CPlotArea::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:plotArea>");

		m_plotAreaRegion.toXML(writer);

        for(size_t i = 0; i < m_arAxis.size(); ++i)
		{
			m_arAxis[i]->toXML(writer);
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:plotArea>");
	}
	EElementType CPlotArea::getType() const {return et_ct_PlotArea;}
//----------------------------------------------------------------------------------------------
	CPlotAreaRegion::~CPlotAreaRegion()
	{
		for(size_t i = 0; i < m_arSeries.size(); ++i)
			delete m_arSeries[i];
        m_arSeries.clear();

	}
	void CPlotAreaRegion::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if( L"plotSurface" == sName)
			{
				m_plotSurface = oReader;
			}
			else if( L"series" == sName)
			{
				CSeries *item = new CSeries(oReader);
				m_arSeries.push_back(item);
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CPlotAreaRegion::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:plotAreaRegion>");

		if(m_plotSurface.IsInit())
		{
            m_plotSurface->toXML(writer);
		}
        for(size_t i = 0; i < m_arSeries.size(); ++i)
		{
			m_arSeries[i]->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:plotAreaRegion>");
	}
	EElementType CPlotAreaRegion::getType() const {return et_ct_PlotAreaRegion;}
//----------------------------------------------------------------------------------------------
	void CPlotSurface::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CPlotSurface::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:plotSurface>");

		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:plotSurface>");
	}
	EElementType CPlotSurface::getType() const {return et_ct_PlotSurface;}
//------------------------------------------------------------------------------------------------
	void CAxis::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if (oReader, L"id", m_id)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"hidden", m_hidden)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CAxis::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if(_T("catScaling") == sName)
			{
				m_catScaling = new CCatScaling;
				m_catScaling->fromXML(oReader);
			}
			else if(_T("valScaling") == sName)
			{
				m_valScaling = new CValScaling;
				m_valScaling->fromXML(oReader);
			}
			//else if(_T("majorGridlines") == sName)
			//{
			//	m_majorGridlines = new CT_ChartLines;
			//	m_majorGridlines->fromXML(oReader);
			//}
			//else if(_T("minorGridlines") == sName)
			//{
			//	m_minorGridlines = new CT_ChartLines;
			//	m_minorGridlines->fromXML(oReader);
			//}
			else if(_T("title") == sName)
			{
				m_title = oReader;
			}
			//else if(_T("numFmt") == sName)
			//{
			//	m_numFmt = new CT_NumFmt;
			//	m_numFmt->fromXML(oReader);
			//}
			//else if(_T("majorTickMark") == sName)
			//{
			//	m_majorTickMark = new CT_TickMark;
			//	m_majorTickMark->fromXML(oReader);
			//}
			//else if(_T("minorTickMark") == sName)
			//{
			//	m_minorTickMark = new CT_TickMark;
			//	m_minorTickMark->fromXML(oReader);
			//}
			else if(_T("tickLabels") == sName)
			{
				m_tickLabels = true;
			}
			else if(_T("spPr") == sName)
			{
				m_oSpPr = oReader;
			}
			else if(_T("txPr") == sName)
			{
				m_oTxPr = oReader;
			}
			else if(_T("units") == sName)
			{
				m_units = oReader;
			}
			else if(_T("extLst") == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CAxis::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:axis");
			WritingStringNullableAttrUInt(L"id", m_id, *m_id);
			WritingStringNullableAttrString(L"hidden", m_hidden, *m_hidden ? L"true" : L"false");
		writer.WriteString(L">");
		if(m_valScaling.IsInit())
		{
            m_valScaling->toXML(writer);
		}
		if(m_catScaling.IsInit())
		{
            m_catScaling->toXML(writer);
		}
		if(m_majorGridlines.IsInit())
		{
			m_majorGridlines->m_name = L"cx:majorGridlines";
            m_majorGridlines->toXML( writer);
		}
		if(m_minorGridlines.IsInit())
		{
 			m_minorGridlines->m_name = L"cx:minorGridlines";
			m_minorGridlines->toXML(writer);
		}
		if(m_title.IsInit())
		{
            m_title->toXML(writer);
		}
		if(m_numFmt.IsInit())
		{
            m_numFmt->toXML(writer);
		}
		if(m_majorTickMarks.IsInit())
		{
 			m_majorTickMarks->m_name = L"cx:majorTickMark";
            m_majorTickMarks->toXML(writer);
		}
		if(m_minorTickMarks.IsInit())
		{
  			m_minorTickMarks->m_name = L"cx:minorTickMark";
			m_minorTickMarks->toXML(writer);
		}
		if(m_tickLabels.IsInit())
		{
			writer.WriteString(L"<cx:tickLabels/>");
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if(m_units.IsInit())
		{
            m_units->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:axis>");
	}
	EElementType CAxis::getType() const {return et_ct_Axis;}
//-------------------------------------------------------------------------------------
	void CAxisUnit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"unit", m_unit)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CAxisUnit::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if(_T("unitsLabel") == sName)
			{
				m_unitsLabel = oReader;
			}
			else if(_T("extLst") == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CAxisUnit::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:units");
			WritingStringNullableAttrString(L"unit", m_unit, m_unit->ToString());
		writer.WriteString(L">");

		if(m_unitsLabel.IsInit())
		{
            m_unitsLabel->toXML(writer);
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:units>");
	}
	EElementType CAxisUnit::getType() const {return et_ct_AxisUnit;}
//----------------------------------------------------------------------------------------
	void CAxisUnitsLabel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"tx" == sName)
			{
				m_tx = oReader;
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CAxisUnitsLabel::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:unitsLabel>");
		if(m_tx.IsInit())
		{
            m_tx->toXML(writer);
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if(m_oExtLst.IsInit())
		{
            writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:unitsLabel>");
	}
	EElementType CAxisUnitsLabel::getType() const {return et_ct_AxisUnitsLabel;}
//-------------------------------------------------------------------------------------
	void CValScaling::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"min"		, m_min)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"max"		, m_max)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"majorUnit"	, m_majorUnit)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"minorUnit"	, m_minorUnit)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CValScaling::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;
	}
	void CValScaling::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:valScaling");
			WritingStringNullableAttrString(L"min", m_min, m_min->ToString())
			WritingStringNullableAttrString(L"max", m_max, m_max->ToString())
			WritingStringNullableAttrString(L"majorUnit", m_max, m_majorUnit->ToString())
			WritingStringNullableAttrString(L"minorUnit", m_max, m_minorUnit->ToString())
		writer.WriteString(L"/>");

	}
	EElementType CValScaling::getType() const {return et_ct_ValScaling;}
//-------------------------------------------------------------------------------------
	void CCatScaling::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"gapWidth", m_gapWidth)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CCatScaling::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;
	}
	void CCatScaling::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:catScaling");
			WritingStringNullableAttrString(L"gapWidth", m_gapWidth, m_gapWidth->ToString())
		writer.WriteString(L"/>");

	}
	EElementType CCatScaling::getType() const {return et_ct_CatScaling;}
//----------------------------------------------------------------------------------------
	CSeries::~CSeries()
	{
		for(size_t i = 0; i < m_arDataPt.size(); ++i)
			delete m_arDataPt[i];
        m_arDataPt.clear();
	}
	void CSeries::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"layoutId"	, m_oLayoutId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"hidden"		, m_bHidden)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"ownerIdx"	, m_nOwnerIdx)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"uniqueId"	, m_sUniqueId)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"formatIdx"	, m_nFormatIdx)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CSeries::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"dataId" == sName)
			{
				m_dataId = oReader;
			}
			else if( L"tx" == sName)
			{
				m_tx = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
			else if( L"layoutPr" == sName)
			{
				m_layoutPr = oReader;
			}	
			else if( L"dataLabels" == sName)
			{
				m_dataLabels = oReader;
			}	
			else if( L"dataPt" == sName)
			{
				CDataPoint *item = new CDataPoint(oReader);
				m_arDataPt.push_back(item);
			}
			else if( L"axisId" == sName)
			{
				std::wstring s = oReader.GetText3(); 
				m_arAxisId.push_back(XmlUtils::GetInteger(s));
			}
		}
	}
	void CSeries::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:series");
			WritingStringNullableAttrString(L"layoutId", m_oLayoutId, m_oLayoutId->ToString())
			WritingStringNullableAttrString(L"hidden", m_bHidden, m_bHidden.get() ? L"true": L"false")
			WritingStringNullableAttrInt(L"ownerIdx", m_nOwnerIdx, m_nOwnerIdx.get())
			WritingStringNullableAttrString(L"uniqueId", m_sUniqueId, m_sUniqueId.get())
			WritingStringNullableAttrInt(L"formatIdx", m_nFormatIdx, m_nFormatIdx.get())
		writer.WriteString(L">");
		if (m_tx.IsInit())
		{
			m_tx->toXML(writer);
		}
		for (size_t i = 0; i < m_arDataPt.size(); i++)
		{
			m_arDataPt[i]->toXML(writer);
		}	
		if (m_dataLabels.IsInit())
		{
			m_dataLabels->toXML(writer);
		}
		if (m_dataId.IsInit())
		{
			writer.WriteString(L"<cx:dataId ");
			writer.WriteString(m_dataId->ToString());
			writer.WriteString(L"/>");
		}
		if (m_layoutPr.IsInit())
		{
			m_layoutPr->toXML(writer);
		}
		for (size_t i = 0; i < m_arAxisId.size(); i++)
		{
			writer.WriteString(L"<cx:axisId>");
			writer.WriteString(std::to_wstring(m_arAxisId[i]));
			writer.WriteString(L"</cx:axisId>");
		}

		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}		
		if (m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:series>");
	}
	EElementType CSeries::getType() const {return et_ct_Series;}
//----------------------------------------------------------------------------------------------
	void CFormula::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		m_content = oReader.GetText3();
		
		if ( oReader.IsEmptyNode() )
			return;
	}
	void CFormula::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"dir", m_dir)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CFormula::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:f");
			WritingStringNullableAttrString(L"dir", m_dir, m_dir->ToString())
		writer.WriteString(L">");

		writer.WriteString(m_content);

		writer.WriteString(L"</cx:f>");
	}
	EElementType CFormula::getType() const {return et_ct_Formula;}
//----------------------------------------------------------------------------------------------
	void CTextData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"f" == sName)
			{
				m_oF = oReader;
			}
			else if( L"v" == sName)
			{
				m_oV = oReader.GetText3();
			}
		}
	}
	void CTextData::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:txData>");

		if (m_oF.IsInit())
		{
			m_oF->toXML(writer);
		}
		if (m_oV.IsInit())
		{
			writer.WriteString(L"<cx:v>");
				writer.WriteString(*m_oV);
			writer.WriteString(L"</cx:v>");
		}

		writer.WriteString(L"</cx:txData>");
	}
	EElementType CTextData::getType() const {return et_ct_TextData;}
//----------------------------------------------------------------------------------------------
	void CText::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"txData" == sName)
			{
				m_txData = oReader;
			}
			else if( L"rich" == sName)
			{
				m_oRich = oReader;
			}
		}	
	}
	void CText::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:tx>");

		if (m_oRich.IsInit())
		{
			m_oRich->m_name = L"cx:rich";
			writer.WriteString(m_oRich->toXML());
		}
		if (m_txData.IsInit())
		{
			m_txData->toXML(writer);
		}

		writer.WriteString(L"</cx:tx>");
	}
	EElementType CText::getType() const {return et_ct_Text;}
//----------------------------------------------------------------------------------------------
	void CTitle::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"tx" == sName)
			{
				m_tx = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}	
	}
	void CTitle::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:title");
			WritingStringNullableAttrString(L"pos", m_pos, m_pos->ToString())
			WritingStringNullableAttrString(L"align", m_align, m_align->ToString())
			WritingStringNullableAttrString(L"overlay", m_overlay, *m_overlay ? L"1" : L"0")
		writer.WriteString(L">");
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if (m_tx.IsInit())
		{
			m_tx->toXML(writer);
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS(L"cx:"));
		}
		writer.WriteString(L"</cx:title>");
	}
	void CTitle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if (oReader, L"pos", m_pos)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"align", m_align)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"overlay", m_overlay)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	EElementType CTitle::getType() const {return et_ct_Title;}
//----------------------------------------------------------------------------------------------
	CNumericLevel::~CNumericLevel()
	{
		for(size_t i = 0; i < m_arPt.size(); ++i)
			delete m_arPt[i];
        m_arPt.clear();
	}
	void CNumericLevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"pt" == sName)
			{
				CNumericValue* item = new CNumericValue(oReader);
				m_arPt.push_back(item);
			}
		}	
	}
	void CNumericLevel::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:lvl");
			WritingStringNullableAttrInt(L"ptCount", m_ptCount, *m_ptCount)
			WritingStringNullableAttrString(L"formatCode", m_formatCode, *m_formatCode)
			WritingStringNullableAttrString(L"name", m_name, *m_name)
		writer.WriteString(L">");

        for(size_t i = 0; i < m_arPt.size(); ++i)
		{
			m_arPt[i]->toXML(writer);
		}
		writer.WriteString(L"</cx:lvl>");
	}
	void CNumericLevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if (oReader, L"ptCount", m_ptCount)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"formatCode", m_formatCode)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"name", m_name)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	EElementType CNumericLevel::getType() const {return et_ct_NumericLevel;}
//----------------------------------------------------------------------------------------------
	void CNumericValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		std::wstring s = oReader.GetText3();
		if (false == s.empty())
			m_content = XmlUtils::GetDouble(s);
		
		if ( oReader.IsEmptyNode() )
			return;
	}
	void CNumericValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"idx", m_idx)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CNumericValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:pt");
			WritingStringNullableAttrInt(L"idx", m_idx, *m_idx)
		writer.WriteString(L">");

		if (m_content.IsInit())
		{
			writer.WriteString(XmlUtils::ToString(*m_content));
		}

		writer.WriteString(L"</cx:pt>");
	}
	EElementType CNumericValue::getType() const {return et_ct_NumericValue;}
//----------------------------------------------------------------------------------------------
	CStringLevel::~CStringLevel()
	{
		for(size_t i = 0; i < m_arPt.size(); ++i)
			delete m_arPt[i];
        m_arPt.clear();
	}
	void CStringLevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"pt" == sName)
			{
				CStringValue* item = new CStringValue(oReader);
				m_arPt.push_back(item);
			}
		}	
	}
	void CStringLevel::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:lvl");
			WritingStringNullableAttrInt(L"ptCount", m_ptCount, *m_ptCount)
			WritingStringNullableAttrString(L"name", m_name, *m_name)
		writer.WriteString(L">");

        for(size_t i = 0; i < m_arPt.size(); ++i)
		{
			m_arPt[i]->toXML(writer);
		}
		writer.WriteString(L"</cx:lvl>");
	}
	void CStringLevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if (oReader, L"ptCount", m_ptCount)
			WritingElement_ReadAttributes_Read_else_if (oReader, L"name", m_name)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	EElementType CStringLevel::getType() const {return et_ct_StringLevel;}
//----------------------------------------------------------------------------------------------
	void CStringValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		m_content = oReader.GetText3();
		
		if ( oReader.IsEmptyNode() )
			return;
	}
	void CStringValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"idx", m_idx)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CStringValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:pt");
			WritingStringNullableAttrInt(L"idx", m_idx, *m_idx)
		writer.WriteString(L">");

		writer.WriteString(m_content);

		writer.WriteString(L"</cx:pt>");
	}
	EElementType CStringValue::getType() const {return et_ct_StringValue;}
//----------------------------------------------------------------------------------------------
	void CSeriesElementVisibilities::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CSeriesElementVisibilities::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"connectorLines",	m_connectorLines)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"meanLine",			m_meanLine)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"meanMarker",		m_meanMarker)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"nonoutliers",		m_nonoutliers)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"outliers",			m_outliers)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CSeriesElementVisibilities::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:visibility");
			WritingStringNullableAttrString(L"connectorLines",	m_connectorLines,	*m_connectorLines	? L"1" : L"0")
			WritingStringNullableAttrString(L"meanLine",		m_meanLine,			*m_meanLine			? L"1" : L"0")
			WritingStringNullableAttrString(L"meanMarker",		m_meanMarker,		*m_meanMarker		? L"1" : L"0")
			WritingStringNullableAttrString(L"nonoutliers",		m_nonoutliers,		*m_nonoutliers		? L"1" : L"0")
			WritingStringNullableAttrString(L"outliers",		m_outliers,			*m_outliers			? L"1" : L"0")
		writer.WriteString(L"/>");
	}
	EElementType CSeriesElementVisibilities::getType() const {return et_ct_SeriesElementVisibilities;}
//----------------------------------------------------------------------------------------------
	void CDataLabelVisibilities::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CDataLabelVisibilities::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"seriesName",	m_seriesName)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"categoryName",	m_categoryName)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"value",			m_value)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CDataLabelVisibilities::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:visibility");
			WritingStringNullableAttrString(L"seriesName",		m_seriesName,	*m_seriesName	? L"1" : L"0")
			WritingStringNullableAttrString(L"categoryName",	m_categoryName,	*m_categoryName	? L"1" : L"0")
			WritingStringNullableAttrString(L"value",			m_value,		*m_value		? L"1" : L"0")
		writer.WriteString(L"/>");
	}
	EElementType CDataLabelVisibilities::getType() const {return et_ct_DataLabelVisibilities;}
//----------------------------------------------------------------------------------------------
	void CBinning::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"binSize" == sName)
			{
				std::wstring s = oReader.GetText3();
				m_binSize = XmlUtils::GetDouble(s);
			}
			else if( L"binCount" == sName)
			{
				std::wstring s = oReader.GetText3();
				m_binCount = XmlUtils::GetInteger(s);
			}
		}
	}
	void CBinning::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"intervalClosed",m_intervalClosed)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"underflow",		m_underflow)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"overflow",		m_overflow)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CBinning::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:binning");
			WritingStringNullableAttrString(L"intervalClosed",	m_intervalClosed,	m_intervalClosed->ToString())
			WritingStringNullableAttrString(L"underflow",		m_underflow,		m_underflow->ToString())
			WritingStringNullableAttrString(L"overflow",		m_overflow,			m_overflow->ToString())
		writer.WriteString(L">");
			if (m_binCount.IsInit())
			{
				writer.WriteString(L"<cx:binCount>");
					writer.WriteString(XmlUtils::ToString(*m_binCount));
				writer.WriteString(L"</cx:binCount>");
			}
			else if (m_binSize.IsInit())
			{
				writer.WriteString(L"<cx:binSize>");
					writer.WriteString(std::to_wstring(*m_binSize));
				writer.WriteString(L"</cx:binSize>");
			}
		writer.WriteString(L"</cx:binning>");
	}
	EElementType CBinning::getType() const {return et_ct_Binning;}
//----------------------------------------------------------------------------------------------
	void CStatistics::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CStatistics::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle (oReader, L"quartileMethod", m_quartileMethod)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CStatistics::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:statistics");
			WritingStringNullableAttrString(L"quartileMethod",	m_quartileMethod, m_quartileMethod->ToString())
		writer.WriteString(L"/>");
	}
	EElementType CStatistics::getType() const {return et_ct_Statistics;}
//----------------------------------------------------------------------------------------------
	void CSubtotals::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"idx" == sName)
			{
				std::wstring s = oReader.GetText3();
				m_arIdx.push_back(XmlUtils::GetInteger(s));
			}
		}
	}
	void CSubtotals::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:subtotals>");

		for (size_t i= 0; i < m_arIdx.size(); i++)
		{
			writer.WriteString(L"<cx:idx>");
			writer.WriteString(std::to_wstring(m_arIdx[i]));
			writer.WriteString(L"</cx:idx>");
		}
		writer.WriteString(L"</cx:subtotals>");
	}
	EElementType CSubtotals::getType() const {return et_ct_Subtotals;}
//----------------------------------------------------------------------------------------------
	void CSeriesLayoutProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"visibility" == sName)
			{
				m_visibility = oReader;
			}
			else if( L"aggregation" == sName)
			{
				m_aggregation = true;
			}
			else if( L"binning" == sName)
			{
				m_binning = oReader;
			}
			else if( L"statistics" == sName)
			{
				m_statistics = oReader;
			}
			else if( L"subtotals" == sName)
			{
				m_subtotals = oReader;
			}			
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
			else if( L"parentLabelLayout" == sName)
			{
				m_parentLabelLayout = oReader;
			}			
			else if( L"regionLabelLayout" == sName)
			{
				m_regionLabelLayout = oReader;
			}
		}
	}
	void CSeriesLayoutProperties::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:layoutPr>");

		if (m_regionLabelLayout.IsInit())
		{
			writer.WriteString(L"<cx:regionLabelLayout " + m_regionLabelLayout->ToString() + L"/>");
		}
		if (m_parentLabelLayout.IsInit())
		{
			writer.WriteString(L"<cx:parentLabelLayout " + m_parentLabelLayout->ToString() + L"/>");
		}
		if (m_visibility.IsInit())
		{
			m_visibility->toXML(writer);
		}
		if (m_aggregation.IsInit())
		{
			writer.WriteString(L"<cx:aggregation/>");
		}
		if (m_binning.IsInit())
		{
			m_binning->toXML(writer);
		}
		if (m_statistics.IsInit())
		{
			m_statistics->toXML(writer);
		}
		if (m_subtotals.IsInit())
		{
			m_subtotals->toXML(writer);
		}
		writer.WriteString(L"</cx:layoutPr>");
	}
	EElementType CSeriesLayoutProperties::getType() const {return et_ct_SeriesLayoutProperties;}
//----------------------------------------------------------------------------------------------
	void CNumberFormat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CNumberFormat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"formatCode",	m_formatCode)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"sourceLinked",	m_sourceLinked)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CNumberFormat::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:numFmt");
			WritingStringNullableAttrString(L"formatCode",	m_formatCode,	*m_formatCode)
			WritingStringNullableAttrString(L"sourceLinked",m_sourceLinked,	*m_sourceLinked	? L"1" : L"0")
		writer.WriteString(L"/>");
	}
	EElementType CNumberFormat::getType() const {return et_ct_NumberFormat;}
//----------------------------------------------------------------------------------------------
	void CDataLabelHidden::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CDataLabelHidden::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle(oReader, L"idx",	m_idx)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CDataLabelHidden::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:dataLabelHidden");
			WritingStringNullableAttrInt(L"idx", m_idx,	*m_idx)
		writer.WriteString(L"/>");
	}
	EElementType CDataLabelHidden::getType() const {return et_ct_DataLabelHidden;}
//----------------------------------------------------------------------------------------------
	void CDataLabel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"numFmt" == sName)
			{
				m_numFmt = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"visibility" == sName)
			{
				m_visibility = oReader;
			}
			else if( L"separator" == sName)
			{
				m_separator = oReader.GetText3();
			}			
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
		}
	}
	void CDataLabel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"idx",	m_idx)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"dataLabelPos",	m_dataLabelPos)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CDataLabel::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:dataLabel");
			WritingStringNullableAttrInt(L"idx", m_idx,	*m_idx)
			WritingStringNullableAttrString(L"dataLabelPos", m_dataLabelPos, m_dataLabelPos->ToString())
		writer.WriteString(L">");

		if (m_numFmt.IsInit())
		{
			m_numFmt->toXML(writer);
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_visibility.IsInit())
		{
			m_visibility->toXML(writer);
		}
		if (m_separator.IsInit())
		{
			writer.WriteString(L"<cx:separator>");
				writer.WriteString(*m_separator);
			writer.WriteString(L"</cx:separator>");
		}
		writer.WriteString(L"</cx:dataLabel>");
	}
	EElementType CDataLabel::getType() const {return et_ct_DataLabel;}
//----------------------------------------------------------------------------------------------
	CDataLabels::~CDataLabels()
	{
		for(size_t i = 0; i < m_arDataLabel.size(); ++i)
			delete m_arDataLabel[i];
        m_arDataLabel.clear();

		for(size_t i = 0; i < m_arDataLabelHidden.size(); ++i)
			delete m_arDataLabelHidden[i];
        m_arDataLabelHidden.clear();
	}
	void CDataLabels::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"pos", m_pos)
		WritingElement_ReadAttributes_End_No_NS(oReader)
	}
	void CDataLabels::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"numFmt" == sName)
			{
				m_numFmt = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			else if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"visibility" == sName)
			{
				m_visibility = oReader;
			}
			else if( L"separator" == sName)
			{
				m_separator = oReader.GetText3();
			}			
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
			else if( L"dataLabel" == sName)
			{
				CDataLabel* item = new CDataLabel(oReader);
				m_arDataLabel.push_back(item);
			}
			else if( L"dataLabelHidden" == sName)
			{
				CDataLabelHidden* item = new CDataLabelHidden(oReader);
				m_arDataLabelHidden.push_back(item);			}
		}
	}

	void CDataLabels::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:dataLabels");

		WritingStringNullableAttrString(L"pos", m_pos, m_pos->ToString())
		writer.WriteString(L">");

		if (m_numFmt.IsInit())
		{
			m_numFmt->toXML(writer);
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_visibility.IsInit())
		{
			m_visibility->toXML(writer);
		}
		if (m_separator.IsInit())
		{
			writer.WriteString(L"<cx:separator>");
				writer.WriteString(*m_separator);
			writer.WriteString(L"</cx:separator>");
		}
		writer.WriteString(L"</cx:dataLabels>");
	}
	EElementType CDataLabels::getType() const {return et_ct_DataLabels;}
//----------------------------------------------------------------------------------------------
	void CDataPoint::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}	
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
		}
	}
	void CDataPoint::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle(oReader, L"idx",	m_idx)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CDataPoint::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:dataPt");
			WritingStringNullableAttrInt(L"idx", m_idx,	*m_idx)
		writer.WriteString(L">");

		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		writer.WriteString(L"</cx:dataPt>");
	}
	EElementType CDataPoint::getType() const {return et_ct_DataPoint;}
//----------------------------------------------------------------------------------------------
	void CGridlines::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}	
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
		}
	}
	void CGridlines::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<" + m_name + L">");

		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		writer.WriteString(L"</" + m_name + L">");
	}
	EElementType CGridlines::getType() const {return et_ct_Gridlines;}
//----------------------------------------------------------------------------------------------
	void CTickMarks::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}	
		}
	}
	void CTickMarks::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_ReadSingle(oReader, L"type", m_type)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
	void CTickMarks::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<" + m_name);
			WritingStringNullableAttrString(L"type", m_type, m_type->ToString())
		writer.WriteString(L"/>");
	}
	EElementType CTickMarks::getType() const {return et_ct_TickMarks;}
//----------------------------------------------------------------------------------------------
	void CLegend::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if ( oReader.IsEmptyNode() )
			return;
		
		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
			if( L"spPr" == sName)
			{
				m_oSpPr = oReader;
			}
			else if( L"txPr" == sName)
			{
				m_oTxPr = oReader;
			}
			else if( L"extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}
	void CLegend::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		(oReader, L"pos",		m_pos)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"align",		m_align)
			WritingElement_ReadAttributes_Read_else_if	(oReader, L"overlay",	m_overlay)
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}	
	void CLegend::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cx:legend");
			WritingStringNullableAttrString(L"pos",		m_pos,		m_pos->ToString())
			WritingStringNullableAttrString(L"align",	m_align,	m_align->ToString())
			WritingStringNullableAttrString(L"overlay",	m_overlay,	*m_overlay ? L"1" : L"0")
		writer.WriteString(L">");
		if (m_oSpPr.IsInit())
		{
			m_oSpPr->m_namespace = L"cx";
			writer.WriteString(m_oSpPr->toXML());
		}
		if (m_oTxPr.IsInit())
		{
			m_oTxPr->m_name = L"cx:txPr";
			writer.WriteString(m_oTxPr->toXML());
		}
		writer.WriteString(L"</cx:legend>");
	}
	EElementType CLegend::getType() const {return et_ct_Legend;}

}
}
}
