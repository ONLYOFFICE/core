#include "stdAfx.h"

#include "Converter.h"
#include "../utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "odf_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

#include "style_text_properties.h"
#include "style_paragraph_properties.h"

#include <XlsxFormat\Xlsx.h>


namespace Oox2Odf
{
void OoxConverter::convert(OOX::Spreadsheet::CT_ChartSpace  *oox_chart)
{
	if (!oox_chart)return;

	convert(oox_chart->m_oSpPr.GetPointer());
	convert(oox_chart->m_oTxPr.GetPointer());

	convert(oox_chart->m_chart->m_title);
	convert(oox_chart->m_chart->m_legend);
	convert(oox_chart->m_chart->m_plotArea);

	convert(oox_chart->m_chart->m_sideWall, 2);
	convert(oox_chart->m_chart->m_backWall, 3);
	convert(oox_chart->m_chart->m_floor, 1);
}

void OoxConverter::convert(OOX::Spreadsheet::CRichText* rich)
{
	if (rich == NULL)return;

	odf_context()->chart_context()->start_text();
	OoxConverter::convert(rich->m_oBodyPr.GetPointer());
	
	for (long i=0 ; i < rich->m_arrItems.GetSize();i++)
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
	
	for (long i=0 ; i < txPr->m_arrItems.GetSize();i++)
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
		if (ct_legend->m_legendEntry.GetCount() > 0)
		{
			convert(ct_legend->m_legendEntry[0]); // в odf нет в легенде множественности стилей
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

	odf_context()->chart_context()->start_plot_area();
		convert(ct_plotArea->m_oSpPr.GetPointer());
		convert(ct_plotArea->m_layout);
	///////////////////////
		for (long i=0; i< ct_plotArea->m_Items1.GetCount(); i++)
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
		for (long i=0; i< ct_plotArea->m_Items.GetCount(); i++)//
		{
			if (!ct_plotArea->m_ItemsElementName0[i]) continue;
			switch(*ct_plotArea->m_ItemsElementName0[i])
			{
				case OOX::Spreadsheet::itemschoicetype5AREA3DCHART:		convert((OOX::Spreadsheet::CT_Area3DChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5AREACHART:		convert((OOX::Spreadsheet::CT_AreaChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5BAR3DCHART:		convert((OOX::Spreadsheet::CT_Bar3DChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5BARCHART:		convert((OOX::Spreadsheet::CT_BarChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5BUBBLECHART:		convert((OOX::Spreadsheet::CT_BubbleChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5DOUGHNUTCHART:	convert((OOX::Spreadsheet::CT_DoughnutChart*)	ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5LINE3DCHART:		convert((OOX::Spreadsheet::CT_Line3DChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5LINECHART:		convert((OOX::Spreadsheet::CT_LineChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5OFPIECHART:		convert((OOX::Spreadsheet::CT_OfPieChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5PIE3DCHART:		convert((OOX::Spreadsheet::CT_Pie3DChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5PIECHART:		convert((OOX::Spreadsheet::CT_PieChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5RADARCHART:		convert((OOX::Spreadsheet::CT_RadarChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5SCATTERCHART:	convert((OOX::Spreadsheet::CT_ScatterChart*)	ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5STOCKCHART:		convert((OOX::Spreadsheet::CT_StockChart*)		ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5SURFACE3DCHART:	convert((OOX::Spreadsheet::CT_Surface3DChart*)	ct_plotArea->m_Items[i]);break;
				case OOX::Spreadsheet::itemschoicetype5SURFACECHART:	convert((OOX::Spreadsheet::CT_SurfaceChart*)	ct_plotArea->m_Items[i]);break;			
			}
		}
		if (ct_plotArea->m_dTable)
		{
		}
	odf_context()->chart_context()->end_element();
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
		if (axis->m_majorTickMark)odf_context()->chart_context()->set_axis_label(1);
		if (axis->m_minorTickMark)odf_context()->chart_context()->set_axis_label(2);
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
		if (axis->m_majorTickMark)odf_context()->chart_context()->set_axis_label(1);
		if (axis->m_minorTickMark)odf_context()->chart_context()->set_axis_label(2);
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
		if (axis->m_majorTickMark)odf_context()->chart_context()->set_axis_label(1);
		if (axis->m_minorTickMark)odf_context()->chart_context()->set_axis_label(2);
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
		if (axis->m_majorTickMark)odf_context()->chart_context()->set_axis_label(1);
		if (axis->m_minorTickMark)odf_context()->chart_context()->set_axis_label(2);
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
void OoxConverter::convert(OOX::Spreadsheet::CT_ChartLines *grid, int type)
{
	if (grid == NULL)return;

	odf_context()->chart_context()->start_grid(type);
		convert(grid->m_oSpPr.GetPointer());	
	odf_context()->chart_context()->end_element();
}

void OoxConverter::convert(OOX::Spreadsheet::CT_Area3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"area");
	odf_context()->chart_context()->set_3D(true);

	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_AreaChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"area");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Bar3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bar");
	odf_context()->chart_context()->set_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bar");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);

		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Line3DChart *chart)
{
	odf_context()->chart_context()->set_type_chart(L"line");
	odf_context()->chart_context()->set_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"line");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Pie3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	odf_context()->chart_context()->set_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.GetCount(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.GetCount(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Surface3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"surface");
	odf_context()->chart_context()->set_3D(true);
	
	odf_context()->chart_context()->start_group_series();
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"surface");
	
	odf_context()->chart_context()->start_group_series();
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bubble");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DoughnutChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"ring");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.GetCount(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"scatter");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"radar");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}		
		//for (long i=0; i< chart->m_axId.GetCount(); i++)
		//{
		//	odf_context()->chart_context()->add_axis_group_series(**chart->m_axId[i]->m_val->m_val);
		//}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_StockChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"stock");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		for (long i=0; i< chart->m_axId.GetCount(); i++)
		{
			odf_context()->chart_context()->add_axis_group_series(*chart->m_axId[i]->m_val);
		}		
	odf_context()->chart_context()->end_group_series();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_OfPieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	
	odf_context()->chart_context()->start_group_series();
		convert(chart->m_dLbls);
		for (long i=0; i< chart->m_ser.GetCount(); i++)
		{
			convert(chart->m_ser[i]);
		}
		//for (long i=0; i< chart->m_axId.GetCount(); i++)
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
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bubble");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_xVal);
		convert(ser->m_yVal);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"surface");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"circle");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"scatter");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_xVal);
		convert(ser->m_yVal);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"radar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"line");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_dLbls);
		convert(ser->m_cat);
		convert(ser->m_val);
		convert(ser->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DLbls* ser_lbls)
{
	if (ser_lbls == NULL)return;

	for (long i=0; i< ser_lbls->m_Items.GetCount(); i++)
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
		if (ser_tx->m_strRef->m_strCache){}
	}
	//if (ser_tx->m_v)odf_context()->chart_context()->set_series_name(string2std_string(*ser_tx->m_v));

}
void OoxConverter::convert(OOX::Spreadsheet::CT_AxDataSource* cat)
{
	if (cat == NULL)
	{
		odf_context()->chart_context()->set_category_axis_formula(L"");
		return;
	}

	if (cat->m_strRef)
	{
		if (cat->m_strRef->m_f)odf_context()->chart_context()->set_category_axis_formula(string2std_string(*cat->m_strRef->m_f));
		if (cat->m_strRef->m_strCache){}
	}
	else if (cat->m_numRef)
	{
		if (cat->m_numRef->m_f)odf_context()->chart_context()->set_category_axis_formula(string2std_string(*cat->m_numRef->m_f));
		if (cat->m_numRef->m_numCache){}
	}
	else if (cat->m_numLit)
	{
	}
	else if (cat->m_strLit)
	{
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_NumDataSource* val)
{
	if (val == NULL)return;
	if (val->m_numRef)
	{
		if (val->m_numRef->m_f)odf_context()->chart_context()->set_series_value_formula(string2std_string(*val->m_numRef->m_f));
		if (val->m_numRef->m_numCache){}
	}
	else if (val->m_numLit)
	{
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Spreadsheet::CT_Surface* ct_surface, int type)
{
	//floor, side, back
}
}