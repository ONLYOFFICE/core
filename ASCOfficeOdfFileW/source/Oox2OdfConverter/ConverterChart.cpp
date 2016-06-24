/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Converter.h"
#include "../utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "../OdfFormat/odf_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"

#include "../../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"


namespace Oox2Odf
{
void OoxConverter::convert(OOX::Spreadsheet::CT_ChartSpace  *oox_chart)
{
	if (!oox_chart)return;

	convert(oox_chart->m_externalData);

	convert(oox_chart->m_oSpPr.GetPointer());	
	convert(oox_chart->m_oTxPr.GetPointer());

	convert(oox_chart->m_chart->m_title);
	convert(oox_chart->m_chart->m_legend);

	odf_context()->chart_context()->start_plot_area();
	{
		convert(oox_chart->m_chart->m_plotArea); 
			
		bool chart3D = (oox_chart->m_chart->m_view3D) ? true : false;
		convert(oox_chart->m_chart->m_floor,	1, chart3D);
		if (chart3D) convert(oox_chart->m_chart->m_backWall, 2, chart3D);
		//convert(oox_chart->m_chart->m_sizeWall, 3, chart3D);		
	}
	odf_context()->chart_context()->end_plot_area();
}

void OoxConverter::convert(OOX::Spreadsheet::CRichText* rich)
{
	if (rich == NULL)return;

	odf_context()->chart_context()->start_text();
	OoxConverter::convert(rich->m_oBodyPr.GetPointer());
	
	for (unsigned int i=0 ; i < rich->m_arrItems.size();i++)
	{
		OoxConverter::convert(rich->m_arrItems[i]);
	}
	odf_context()->chart_context()->end_text();
}
void OoxConverter::convert(OOX::Spreadsheet::CTextProperties* txPr)
{
	if (txPr == NULL)return;

	odf_context()->chart_context()->start_text();
	OoxConverter::convert(txPr->m_oBodyPr.GetPointer());
	
	for (unsigned int i=0 ; i < txPr->m_arrItems.size();i++)
	{
		OoxConverter::convert(txPr->m_arrItems[i]);
	}
	odf_context()->chart_context()->end_text();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Tx* ct_tx)
{
	if (ct_tx == NULL)return;

	convert(ct_tx->m_oRich.GetPointer());
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Layout* ct_layout)
{
	if (ct_layout == NULL)return;
	if (ct_layout->m_manualLayout == NULL)return;

	int m_xMode=0, m_yMode=0, m_wMode=0,m_hMode=0;

	if (ct_layout->m_manualLayout->m_xMode && ct_layout->m_manualLayout->m_xMode->m_val) m_xMode = *ct_layout->m_manualLayout->m_xMode->m_val;
	if (ct_layout->m_manualLayout->m_yMode && ct_layout->m_manualLayout->m_yMode->m_val) m_yMode = *ct_layout->m_manualLayout->m_yMode->m_val;
	if (ct_layout->m_manualLayout->m_hMode && ct_layout->m_manualLayout->m_hMode->m_val) m_hMode = *ct_layout->m_manualLayout->m_hMode->m_val;
	if (ct_layout->m_manualLayout->m_wMode && ct_layout->m_manualLayout->m_wMode->m_val) m_wMode = *ct_layout->m_manualLayout->m_wMode->m_val;

	if (ct_layout->m_manualLayout->m_x)	odf_context()->chart_context()->set_layout_x(ct_layout->m_manualLayout->m_x->m_val,m_xMode);
	if (ct_layout->m_manualLayout->m_y)	odf_context()->chart_context()->set_layout_y(ct_layout->m_manualLayout->m_y->m_val,m_yMode);
	if (ct_layout->m_manualLayout->m_w)	odf_context()->chart_context()->set_layout_w(ct_layout->m_manualLayout->m_w->m_val,m_wMode);
	if (ct_layout->m_manualLayout->m_h)	odf_context()->chart_context()->set_layout_h(ct_layout->m_manualLayout->m_h->m_val,m_hMode);
}

void OoxConverter::convert(OOX::Spreadsheet::CT_Title* ct_title)
{
	if (ct_title == NULL)return;

	odf_context()->chart_context()->start_title();
		convert(ct_title->m_oSpPr.GetPointer());
		convert(ct_title->m_layout);
		convert(ct_title->m_oTxPr.GetPointer());
	///////////////////////////////
		convert(ct_title->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Legend* ct_legend)
{
	if (ct_legend == NULL)return;

	odf_context()->chart_context()->start_legend();
		convert(ct_legend->m_oSpPr.GetPointer());
		convert(ct_legend->m_layout);
		if ((ct_legend->m_legendPos) && (ct_legend->m_legendPos->m_val))
			odf_context()->chart_context()->set_legend_position(*ct_legend->m_legendPos->m_val);
		
		convert(ct_legend->m_oTxPr.GetPointer());
		if (ct_legend->m_legendEntry.size() > 0)
		{
			convert(ct_legend->m_legendEntry[0]); // в odf_writer нет в легенде множественности стилей
		}
	//////////////////////
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LegendEntry* ct_legend)
{
	if (ct_legend == NULL)return;

	convert(ct_legend->m_oTxPr.GetPointer());
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PlotArea* ct_plotArea)
{
	if (ct_plotArea == NULL)return;

	bool chart3D = false;

	convert(ct_plotArea->m_layout);
///////////////////////
	for (unsigned int i=0; i< ct_plotArea->m_Items1.size(); i++)
	{
		if (!ct_plotArea->m_ItemsElementName1[i]) continue;
		switch(*ct_plotArea->m_ItemsElementName1[i])
		{		
			case OOX::Spreadsheet::itemschoicetype6CATAX:	convert((OOX::Spreadsheet::CT_CatAx*)ct_plotArea->m_Items1[i]);break;
			case OOX::Spreadsheet::itemschoicetype6DATEAX:	convert((OOX::Spreadsheet::CT_DateAx*)ct_plotArea->m_Items1[i]);break;
			case OOX::Spreadsheet::itemschoicetype6SERAX:	convert((OOX::Spreadsheet::CT_SerAx*)ct_plotArea->m_Items1[i]);break;
			case OOX::Spreadsheet::itemschoicetype6VALAX:	convert((OOX::Spreadsheet::CT_ValAx*)ct_plotArea->m_Items1[i]);break;				
		}
	}
	for (unsigned int i=0; i< ct_plotArea->m_Items.size(); i++)//
	{
		if (!ct_plotArea->m_ItemsElementName0[i]) continue;
		switch(*ct_plotArea->m_ItemsElementName0[i])
		{
			case OOX::Spreadsheet::itemschoicetype5AREA3DCHART:		convert((OOX::Spreadsheet::CT_Area3DChart*)		ct_plotArea->m_Items[i]); chart3D = true; break;
			case OOX::Spreadsheet::itemschoicetype5AREACHART:		convert((OOX::Spreadsheet::CT_AreaChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5BAR3DCHART:		convert((OOX::Spreadsheet::CT_Bar3DChart*)		ct_plotArea->m_Items[i]); chart3D = true; break;
			case OOX::Spreadsheet::itemschoicetype5BARCHART:		convert((OOX::Spreadsheet::CT_BarChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5BUBBLECHART:		convert((OOX::Spreadsheet::CT_BubbleChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5DOUGHNUTCHART:	convert((OOX::Spreadsheet::CT_DoughnutChart*)	ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5LINE3DCHART:		convert((OOX::Spreadsheet::CT_Line3DChart*)		ct_plotArea->m_Items[i]); chart3D = true; break;
			case OOX::Spreadsheet::itemschoicetype5LINECHART:		convert((OOX::Spreadsheet::CT_LineChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5OFPIECHART:		convert((OOX::Spreadsheet::CT_OfPieChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5PIE3DCHART:		convert((OOX::Spreadsheet::CT_Pie3DChart*)		ct_plotArea->m_Items[i]); chart3D = true; break;
			case OOX::Spreadsheet::itemschoicetype5PIECHART:		convert((OOX::Spreadsheet::CT_PieChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5RADARCHART:		convert((OOX::Spreadsheet::CT_RadarChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5SCATTERCHART:	convert((OOX::Spreadsheet::CT_ScatterChart*)	ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5STOCKCHART:		convert((OOX::Spreadsheet::CT_StockChart*)		ct_plotArea->m_Items[i]);break;
			case OOX::Spreadsheet::itemschoicetype5SURFACE3DCHART:	convert((OOX::Spreadsheet::CT_Surface3DChart*)	ct_plotArea->m_Items[i]); chart3D = true; break;
			case OOX::Spreadsheet::itemschoicetype5SURFACECHART:	convert((OOX::Spreadsheet::CT_SurfaceChart*)	ct_plotArea->m_Items[i]);break;			
		}
	}
	if (ct_plotArea->m_dTable)
	{
	}
	if (chart3D == false)
	{
		odf_context()->chart_context()->start_wall();
		{
			convert(ct_plotArea->m_oSpPr.GetPointer());
		}	
		odf_context()->chart_context()->end_element();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Spreadsheet::CT_CatAx* axis)
{
	if (axis == NULL)return;

	odf_context()->chart_context()->start_axis();
		odf_context()->chart_context()->set_axis_dimension(1);
		if (axis->m_axId && axis->m_axId->m_val)
			odf_context()->chart_context()->set_axis_id(*axis->m_axId->m_val);
		convert(axis->m_oSpPr.GetPointer());
		if (axis->m_scaling)
		{
			if (axis->m_scaling->m_logBase)
				odf_context()->chart_context()->set_axis_logarithmic(true);
			if (axis->m_scaling->m_orientation && axis->m_scaling->m_orientation->m_val)
				odf_context()->chart_context()->set_axis_orientation(*axis->m_scaling->m_orientation->m_val);
			if (axis->m_scaling->m_min && axis->m_scaling->m_min->m_val) odf_context()->chart_context()->set_axis_min(*axis->m_scaling->m_min->m_val);
			if (axis->m_scaling->m_max && axis->m_scaling->m_max->m_val) odf_context()->chart_context()->set_axis_max(*axis->m_scaling->m_max->m_val);
		}
		if (axis->m_majorTickMark && axis->m_majorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_major(*axis->m_majorTickMark->m_val);
		
		if (axis->m_minorTickMark && axis->m_minorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_minor(*axis->m_minorTickMark->m_val);

		if (axis->m_tickLblPos)
		{
			if (axis->m_tickLblPos->m_val)odf_context()->chart_context()->set_display_label_position(*axis->m_tickLblPos->m_val);
			odf_context()->chart_context()->set_display_label(true);
		}
		if (axis->m_axPos && axis->m_axPos->m_val)
			odf_context()->chart_context()->set_axis_position(*axis->m_axPos->m_val);
		if (axis->m_tickLblPos && axis->m_tickLblPos->m_val)
			odf_context()->chart_context()->set_axis_label_position(*axis->m_tickLblPos->m_val);

	///////////////////
		convert(axis->m_oTxPr.GetPointer());
		convert(axis->m_title);
		convert(axis->m_majorGridlines, 1);
		convert(axis->m_minorGridlines, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DateAx* axis)
{
	if (axis == NULL)return;

	odf_context()->chart_context()->start_axis();
		odf_context()->chart_context()->set_axis_dimension(1);
		if (axis->m_axId && axis->m_axId->m_val)
			odf_context()->chart_context()->set_axis_id(*axis->m_axId->m_val);
		convert(axis->m_oSpPr.GetPointer());
		if (axis->m_scaling)
		{
			if (axis->m_scaling->m_logBase)
				odf_context()->chart_context()->set_axis_logarithmic(true);
			if (axis->m_scaling->m_orientation && axis->m_scaling->m_orientation->m_val)
				odf_context()->chart_context()->set_axis_orientation(*axis->m_scaling->m_orientation->m_val);
			if (axis->m_scaling->m_min && axis->m_scaling->m_min->m_val) odf_context()->chart_context()->set_axis_min(*axis->m_scaling->m_min->m_val);
			if (axis->m_scaling->m_max && axis->m_scaling->m_max->m_val) odf_context()->chart_context()->set_axis_max(*axis->m_scaling->m_max->m_val);

		}
		if (axis->m_majorTickMark && axis->m_majorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_major(*axis->m_majorTickMark->m_val);
		
		if (axis->m_minorTickMark && axis->m_minorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_minor(*axis->m_minorTickMark->m_val);

		if (axis->m_tickLblPos)
		{
			if (axis->m_tickLblPos->m_val)odf_context()->chart_context()->set_display_label_position(*axis->m_tickLblPos->m_val);
			odf_context()->chart_context()->set_display_label(true);
		}
		if (axis->m_axPos && axis->m_axPos->m_val)
			odf_context()->chart_context()->set_axis_position(*axis->m_axPos->m_val);
	//////////////////
		convert(axis->m_oTxPr.GetPointer());
		convert(axis->m_title);
		convert(axis->m_majorGridlines, 1);
		convert(axis->m_minorGridlines, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SerAx* axis)
{
	if (axis == NULL)return;

	odf_context()->chart_context()->start_axis();
		odf_context()->chart_context()->set_axis_dimension(1);
		if (axis->m_axId && axis->m_axId->m_val)
			odf_context()->chart_context()->set_axis_id(*axis->m_axId->m_val);
		convert(axis->m_oSpPr.GetPointer());
		if (axis->m_scaling)
		{
			if (axis->m_scaling->m_logBase)
				odf_context()->chart_context()->set_axis_logarithmic(true);
			if (axis->m_scaling->m_orientation && axis->m_scaling->m_orientation->m_val)
				odf_context()->chart_context()->set_axis_orientation(*axis->m_scaling->m_orientation->m_val);
			if (axis->m_scaling->m_min && axis->m_scaling->m_min->m_val) odf_context()->chart_context()->set_axis_min(*axis->m_scaling->m_min->m_val);
			if (axis->m_scaling->m_max && axis->m_scaling->m_max->m_val) odf_context()->chart_context()->set_axis_max(*axis->m_scaling->m_max->m_val);
		}
		if (axis->m_majorTickMark && axis->m_majorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_major(*axis->m_majorTickMark->m_val);
		
		if (axis->m_minorTickMark && axis->m_minorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_minor(*axis->m_minorTickMark->m_val);

		if (axis->m_tickLblPos)
		{
			if (axis->m_tickLblPos->m_val)odf_context()->chart_context()->set_display_label_position(*axis->m_tickLblPos->m_val);
			odf_context()->chart_context()->set_display_label(true);
		}

		if (axis->m_axPos && axis->m_axPos->m_val)
			odf_context()->chart_context()->set_axis_position(*axis->m_axPos->m_val);
	///////////////////////////
		convert(axis->m_oTxPr.GetPointer());
		convert(axis->m_title);
		convert(axis->m_majorGridlines, 1);
		convert(axis->m_minorGridlines, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ValAx* axis)
{
	if (axis == NULL)return;

	odf_context()->chart_context()->start_axis();
		odf_context()->chart_context()->set_axis_dimension(2);
		if (axis->m_axId && axis->m_axId->m_val)
			odf_context()->chart_context()->set_axis_id(*axis->m_axId->m_val);
		convert(axis->m_oSpPr.GetPointer());
		if (axis->m_scaling)
		{
			if (axis->m_scaling->m_logBase)
				odf_context()->chart_context()->set_axis_logarithmic(true);
			if (axis->m_scaling->m_orientation && axis->m_scaling->m_orientation->m_val)
				odf_context()->chart_context()->set_axis_orientation(*axis->m_scaling->m_orientation->m_val);
			if (axis->m_scaling->m_min && axis->m_scaling->m_min->m_val)odf_context()->chart_context()->set_axis_min(*axis->m_scaling->m_min->m_val);
			if (axis->m_scaling->m_max && axis->m_scaling->m_max->m_val)odf_context()->chart_context()->set_axis_max(*axis->m_scaling->m_max->m_val);
		}
		if (axis->m_majorTickMark && axis->m_majorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_major(*axis->m_majorTickMark->m_val);
		
		if (axis->m_minorTickMark && axis->m_minorTickMark->m_val)
			odf_context()->chart_context()->set_axis_tick_minor(*axis->m_minorTickMark->m_val);

		if (axis->m_tickLblPos)
		{
			if (axis->m_tickLblPos->m_val)odf_context()->chart_context()->set_display_label_position(*axis->m_tickLblPos->m_val);
			odf_context()->chart_context()->set_display_label(true);
		}
		if (axis->m_axPos && axis->m_axPos->m_val)
			odf_context()->chart_context()->set_axis_position(*axis->m_axPos->m_val);
	/////////////////////////////
		convert(axis->m_oTxPr.GetPointer());
		convert(axis->m_title);
		convert(axis->m_majorGridlines, 1);
		convert(axis->m_minorGridlines, 2);
	odf_context()->chart_context()->end_element();
}
////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Spreadsheet::CT_ChartLines *line, int type)
{
	if (line == NULL)return;

	if (type <3)//grid lines
	{
		odf_context()->chart_context()->start_grid(type);
			convert(line->m_oSpPr.GetPointer());	
		odf_context()->chart_context()->end_element();
	}
	//if (type == 3)//drop line
	//{
	//	odf_context()->chart_context()->s
	//}
	if (type == 4)//Hi-Lo line
	{
		odf_context()->chart_context()->start_stock_range_line();
			convert(line->m_oSpPr.GetPointer());
		odf_context()->chart_context()->end_element();
	}
}

void OoxConverter::convert(OOX::Spreadsheet::CT_Area3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"area");
	odf_context()->chart_context()->set_chart_3D(true);

	convert (chart->m_dropLines, 3);

	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_grouping(*chart->m_grouping->m_val);

	for (unsigned int i=0; i< chart->m_ser.size(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_AreaChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"area");
	
	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_grouping(*chart->m_grouping->m_val);
	
	convert (chart->m_dropLines, 3);

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Bar3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"bar");
	odf_context()->chart_context()->set_chart_3D(true);

	if (chart->m_shape && chart->m_shape->m_val)
		odf_context()->chart_context()->set_chart_bar_type(*chart->m_shape->m_val);
	
	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_bar_grouping(*chart->m_grouping->m_val);
	
	if (chart->m_barDir && chart->m_barDir->m_val)
		odf_context()->chart_context()->set_chart_bar_direction(*chart->m_barDir->m_val);

	if (chart->m_gapWidth && chart->m_gapWidth->m_val)
		odf_context()->chart_context()->set_chart_bar_gap_width(string2std_string(*chart->m_gapWidth->m_val));
	
	//if (chart->m_overlap && chart->m_overlap->m_val)
	//	odf_context()->chart_context()->set_chart_bar_overlap(string2std_string(*chart->m_overlap->m_val));

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"bar");
	
	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_bar_grouping(*chart->m_grouping->m_val);
	
	if (chart->m_barDir && chart->m_barDir->m_val)
		odf_context()->chart_context()->set_chart_bar_direction(*chart->m_barDir->m_val);

	if (chart->m_gapWidth && chart->m_gapWidth->m_val)
		odf_context()->chart_context()->set_chart_bar_gap_width(string2std_string(*chart->m_gapWidth->m_val));

	if (chart->m_overlap && chart->m_overlap->m_val)
		odf_context()->chart_context()->set_chart_bar_overlap(string2std_string(*chart->m_overlap->m_val));

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Line3DChart *chart)
{
	odf_context()->chart_context()->set_chart_type(L"line");
	odf_context()->chart_context()->set_chart_3D(true);
	
	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_grouping(*chart->m_grouping->m_val);

	convert (chart->m_dropLines, 3);

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"line");
	
	if (chart->m_grouping && chart->m_grouping->m_val)
		odf_context()->chart_context()->set_chart_grouping(*chart->m_grouping->m_val);

	if (chart->m_marker && chart->m_marker->m_val && (*chart->m_marker->m_val == true))
		odf_context()->chart_context()->set_marker_type(12); //auto type

	convert (chart->m_dropLines, 3);

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Pie3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"circle");
	odf_context()->chart_context()->set_chart_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.size(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"circle");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.size(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Surface3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"surface");
	odf_context()->chart_context()->set_chart_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"surface");
	
	odf_context()->chart_context()->start_group_series();
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"bubble");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DoughnutChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"ring");

	//m_holeSize
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"scatter");
		convert(chart->m_dLbls);
		if (chart->m_varyColors && chart->m_varyColors->m_val)
			odf_context()->chart_context()->set_chart_colored(*chart->m_varyColors->m_val);
		if (chart->m_scatterStyle && chart->m_scatterStyle->m_val)
			odf_context()->chart_context()->set_chart_scatter_type(*chart->m_scatterStyle->m_val);	
	

	odf_context()->chart_context()->start_group_series();
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"radar");
		if (chart->m_radarStyle && chart->m_radarStyle->m_val)
			odf_context()->chart_context()->set_chart_radar_type(*chart->m_radarStyle->m_val);
		//odf_context()->chart_context()->set_chart_grouping(2);???
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}		
		//for (long i=0; i< chart->m_axId.size(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(**chart->m_axId[i]->m_val->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_StockChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"stock");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (unsigned int i=0; i< chart->m_axId.size(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
		convert (chart->m_dropLines ,3);			
		if (chart->m_upDownBars)
		{
			odf_context()->chart_context()->set_chart_stock_candle_stick(true);
		
			std::wstring gap_width;
			if (chart->m_upDownBars->m_gapWidth && chart->m_upDownBars->m_gapWidth->m_val)
			{
				gap_width = string2std_string(*chart->m_upDownBars->m_gapWidth->m_val);
				/*odf_context()->chart_context()->set_chart_bar_gap_width(gap_width);*/
			}
			
			if (chart->m_upDownBars->m_upBars)
			{
				odf_context()->chart_context()->start_stock_gain_marker();
					convert(chart->m_upDownBars->m_upBars->m_oSpPr.GetPointer());
					if (gap_width.length() > 0) 
						odf_context()->chart_context()->set_chart_bar_gap_width(gap_width);
				odf_context()->chart_context()->end_element();
			}
			if (chart->m_upDownBars->m_downBars)
			{
				odf_context()->chart_context()->start_stock_loss_marker();
					convert(chart->m_upDownBars->m_downBars->m_oSpPr.GetPointer());
					if (gap_width.length() > 0) 
						odf_context()->chart_context()->set_chart_bar_gap_width(gap_width);
				odf_context()->chart_context()->end_element();
			}
		}
		convert (chart->m_hiLowLines, 4);
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_OfPieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_chart_type(L"circle");

	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (unsigned int i=0; i< chart->m_ser.size(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.size(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_AreaSer* ser)
{
	odf_context()->chart_context()->start_series(L"area");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bubble");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_xVal, 2);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();

	odf_context()->chart_context()->start_series(L"bubble");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_yVal);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();

	odf_context()->chart_context()->start_series(L"bubble");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_bubbleSize);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();	
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"surface");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"circle");
		if (ser->m_explosion && ser->m_explosion->m_val)
			odf_context()->chart_context()->set_series_pie_explosion(*ser->m_explosion->m_val);
		convert(ser->m_oSpPr.GetPointer());			
		convert(ser->m_dLbls);
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
		if (ser->m_dPt.size() > 0)
		{
			convert(NULL, ser->m_dPt);
		}
	if (ser->m_shape && ser->m_shape->m_val)
		odf_context()->chart_context()->set_chart_bar_type(*ser->m_shape->m_val);
	else
		odf_context()->chart_context()->set_chart_bar_type(-1);
	//m_trendline;
	//m_errBars
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"scatter");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_xVal, 2);//domain
		convert(ser->m_tx);
		if (ser->m_marker || ser->m_dPt.size() > 0)
		{
			convert(ser->m_marker, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();

	odf_context()->chart_context()->start_series(L"scatter");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_yVal);
		convert(ser->m_tx);
		if (ser->m_marker || ser->m_dPt.size() > 0)
		{
			convert(ser->m_marker, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"radar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
		if (ser->m_marker || ser->m_dPt.size() > 0)
		{
			convert(ser->m_marker, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"line");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat,1);
		convert(ser->m_val);
		convert(ser->m_tx);
		if (ser->m_marker || ser->m_dPt.size() > 0)
		{
			convert(ser->m_marker, ser->m_dPt);
		}
	odf_context()->chart_context()->end_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Marker* marker, std::vector<OOX::Spreadsheet::CT_DPt*> & dPt)
{	
	if (dPt.size() <1) return convert(marker);

	long count_point = odf_context()->chart_context()->get_count_data_points_series();
	long current_point = 0;
	long set_point;
	for (unsigned int i =0 ;i < dPt.size(); i++)
	{
		if (dPt[i] == NULL)continue;

		if (dPt[i]->m_idx && dPt[i]->m_idx->m_val) set_point = *dPt[i]->m_idx->m_val;

		if (set_point - current_point > 0)
		{
			odf_context()->chart_context()->start_data_point_series(set_point - current_point);
				convert(marker);
			odf_context()->chart_context()->end_element();
		}
		odf_context()->chart_context()->start_data_point_series(1);
			convert(dPt[i]->m_oSpPr.GetPointer());
			convert(dPt[i]->m_marker);
			if (dPt[i]->m_explosion && dPt[i]->m_explosion->m_val)
					odf_context()->chart_context()->set_series_pie_explosion(*dPt[i]->m_explosion->m_val);
		odf_context()->chart_context()->end_element();
		
		current_point = set_point+1;			
	}
	if (count_point - current_point >0)
	{
		odf_context()->chart_context()->start_data_point_series(count_point - current_point);
			convert(marker);
		odf_context()->chart_context()->end_element();
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Marker* marker)
{
	if (marker == NULL) return;

	if (marker->m_symbol && marker->m_symbol->m_val)
			odf_context()->chart_context()->set_marker_type(*marker->m_symbol->m_val);
	if (marker->m_size && marker->m_size->m_val)
			odf_context()->chart_context()->set_marker_size(*marker->m_size->m_val);
	convert(marker->m_oSpPr.GetPointer());
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DLbls* ser_lbls)
{
	if (ser_lbls == NULL)return;

	for (unsigned int i=0; i< ser_lbls->m_Items.size(); i++)
	{
		bool boolVal = false;

		OOX::Spreadsheet::CT_Boolean * ct_boolean = (OOX::Spreadsheet::CT_Boolean*)ser_lbls->m_Items[i];
		if (ct_boolean && ct_boolean->m_val) boolVal = * ct_boolean->m_val;
		
		switch(*ser_lbls->m_ItemsElementName0[i])
		{
			case OOX::Spreadsheet::itemschoicetype3DELETE :
				odf_context()->chart_context()->set_label_delete(boolVal); break;	
			case OOX::Spreadsheet::itemschoicetype3SHOWBUBBLESIZE:
				odf_context()->chart_context()->set_label_show_bubble_size(boolVal); break;	
			case OOX::Spreadsheet::itemschoicetype3SHOWCATNAME:
				odf_context()->chart_context()->set_label_show_cat_name(boolVal); break;
			case OOX::Spreadsheet::itemschoicetype3SHOWLEADERLINES:
				odf_context()->chart_context()->set_label_show_leader_line(boolVal); break;
			case OOX::Spreadsheet::itemschoicetype3SHOWLEGENDKEY:
				odf_context()->chart_context()->set_label_show_legend_key(boolVal); break;
			case OOX::Spreadsheet::itemschoicetype3SHOWPERCENT:	
				odf_context()->chart_context()->set_label_show_percent(boolVal); break;
			case OOX::Spreadsheet::itemschoicetype3SHOWSERNAME:	
				odf_context()->chart_context()->set_label_show_ser_name(boolVal); break;
			case OOX::Spreadsheet::itemschoicetype3SHOWVAL:	
				odf_context()->chart_context()->set_label_show_values(boolVal); break;
			
			//case OOX::Spreadsheet::itemschoicetype3SPPR:			convert((OOX::Spreadsheet:: *)ser_lbls->m_Items[i]); break;
			//case OOX::Spreadsheet::itemschoicetype3TXPR:			convert((OOX::Spreadsheet:: *)ser_lbls->m_Items[i]); break;
			
				//case OOX::Spreadsheet::itemschoicetype3DLBLPOS:			convert((OOX::Spreadsheet:: *)ser_lbls->m_Items[i]); break;
			//case OOX::Spreadsheet::itemschoicetype3LEADERLINES:		convert((OOX::Spreadsheet::CT_ChartLines *)ser_lbls->m_Items[i]); break;
			//case OOX::Spreadsheet::itemschoicetype3NUMFMT:			convert((OOX::Spreadsheet:: *)ser_lbls->m_Items[i]); break;
			//case OOX::Spreadsheet::itemschoicetype3SEPARATOR:		convert((OOX::Spreadsheet:: *)ser_lbls->m_Items[i]); break;
		}
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SerTx* ser_tx)
{
	if (ser_tx == NULL)return;
	
	if (ser_tx->m_strRef)
	{
		if (ser_tx->m_strRef->m_f)odf_context()->chart_context()->set_series_label_formula(string2std_string(*ser_tx->m_strRef->m_f));
		
		convert(ser_tx->m_strRef->m_strCache,true);
	}
	//if (ser_tx->m_v)odf_context()->chart_context()->set_series_name(string2std_string(*ser_tx->m_v));

}
void OoxConverter::convert(OOX::Spreadsheet::CT_AxDataSource* cat, int type)
{
	if (cat == NULL)
	{
		odf_context()->chart_context()->set_category_axis_formula(L"",type);
		return;
	}
	if (cat->m_numLit)
	{
		odf_context()->chart_context()->set_series_value_formula(L"");
		
		convert(cat->m_numLit);	
	}
	else if (cat->m_strLit)
	{
		odf_context()->chart_context()->set_category_axis_formula(L"", type);
		convert(cat->m_strLit,true);
	}
	else if (cat->m_strRef)
	{
		if (cat->m_strRef->m_f)odf_context()->chart_context()->set_category_axis_formula(string2std_string(*cat->m_strRef->m_f),type);
		
		convert(cat->m_strRef->m_strCache,true/*, type == 1 ? true:false*/);
	}
	else if (cat->m_numRef)
	{
		if (cat->m_numRef->m_f)odf_context()->chart_context()->set_category_axis_formula(string2std_string(*cat->m_numRef->m_f),type);
		
		convert(cat->m_numRef->m_numCache);
	}

}
void OoxConverter::convert(OOX::Spreadsheet::CT_NumDataSource* val)
{
	if (val == NULL)return;
	
	if (val->m_numLit)
	{
		odf_context()->chart_context()->set_series_value_formula(L"");
		
		convert(val->m_numLit);
	}
	else if (val->m_numRef)
	{
		if (val->m_numRef->m_f)odf_context()->chart_context()->set_series_value_formula(string2std_string(*val->m_numRef->m_f));
		
		convert(val->m_numRef->m_numCache);
	}
	
}
///////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Spreadsheet::CT_Surface* ct_surface, int type, bool chart3D)
{
	if (ct_surface == NULL && chart3D == false)return;
	//floor, side, back
	if (type == 1)odf_context()->chart_context()->start_floor();
	if (type == 2)odf_context()->chart_context()->start_wall();
	//if (type == 3)odf_context()->chart_context()->start_back_wall();

	if (ct_surface)
	{
		convert(ct_surface->m_oSpPr.GetPointer());
	}
	else if (chart3D)//тока для 3D так - хз почему
	{
		odf_context()->chart_context()->set_no_fill(true);
	}
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ExternalData *external_data)
{
	if (external_data == NULL)return;
	//данные для диаграммы внутренние !!!

	if (external_data->m_id == NULL)return;

	//CString pathEmbeddings = GetEmbeddings(*external_data->m_id);
	//oO
	//unpack

	//get/check Format - xlsx
	//конверт sheets -> table:table

	//или
	//convert xlsx -> ods & read tables???

	odf_context()->chart_context()->set_local_table(true);//пока пользуем кэш ....
}
void OoxConverter::convert(OOX::Spreadsheet::CT_NumData	*num_data)
{
	if (num_data == NULL)return;

	//std::vector<double> data;
	std::vector<std::wstring> data;

	for (unsigned int i=0; i < num_data->m_pt.size(); i++)
	{

		if (num_data->m_pt[i] && num_data->m_pt[i]->m_v)
		{
			data.push_back(string2std_string(*num_data->m_pt[i]->m_v));
			//double val=0;
			//try
			//{		
			//	val = boost::lexical_cast<double>(num_data->m_pt[i]->m_v->GetBuffer());
			//}catch(...)
			//{
			//}
			//data.push_back(val);
		}
	}
	std::wstring format;
	if (num_data->m_formatCode) format = string2std_string(*num_data->m_formatCode);
	
	odf_context()->chart_context()->set_cash(format, data);
}
void OoxConverter::convert(OOX::Spreadsheet::CT_StrData *str_data, bool label)
{
	if (str_data == NULL)return;
	std::vector<std::wstring> data;

	for (unsigned int i=0; i < str_data->m_pt.size(); i++)
	{
		if (str_data->m_pt[i] && str_data->m_pt[i]->m_v)
			data.push_back(string2std_string(*str_data->m_pt[i]->m_v));

	}
	std::wstring format;

	odf_context()->chart_context()->set_cash(format, data,label);
}
}
