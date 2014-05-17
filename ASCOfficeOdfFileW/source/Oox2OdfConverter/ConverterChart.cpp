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

	if (ct_layout->m_manualLayout->m_x && ct_layout->m_manualLayout->m_xMode)
		odf_context()->chart_context()->set_layout_x(ct_layout->m_manualLayout->m_x->m_val,*ct_layout->m_manualLayout->m_xMode->m_val);
	if (ct_layout->m_manualLayout->m_y && ct_layout->m_manualLayout->m_yMode)
		odf_context()->chart_context()->set_layout_y(ct_layout->m_manualLayout->m_y->m_val,*ct_layout->m_manualLayout->m_yMode->m_val);
	if (ct_layout->m_manualLayout->m_w && ct_layout->m_manualLayout->m_wMode)
		odf_context()->chart_context()->set_layout_w(ct_layout->m_manualLayout->m_w->m_val,*ct_layout->m_manualLayout->m_wMode->m_val);
	if (ct_layout->m_manualLayout->m_h && ct_layout->m_manualLayout->m_hMode)
		odf_context()->chart_context()->set_layout_h(ct_layout->m_manualLayout->m_h->m_val,*ct_layout->m_manualLayout->m_hMode->m_val);
}

void OoxConverter::convert(OOX::Spreadsheet::CT_Title* ct_title)
{
	if (ct_title == NULL)return;

	odf_context()->chart_context()->start_title();
		convert(ct_title->m_oSpPr.GetPointer());
		convert(ct_title->m_oTxPr.GetPointer());
		convert(ct_title->m_layout);
	///////////////////////////////
		convert(ct_title->m_tx);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Legend* ct_legend)
{
	if (ct_legend == NULL)return;

	odf_context()->chart_context()->start_legend();
		convert(ct_legend->m_oSpPr.GetPointer());
		convert(ct_legend->m_oTxPr.GetPointer());
		convert(ct_legend->m_layout);
		if ((ct_legend->m_legendPos) && (ct_legend->m_legendPos->m_val))
			odf_context()->chart_context()->set_legend_position(*ct_legend->m_legendPos->m_val);
		
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
		for (long i=0; i< ct_plotArea->m_Items.GetCount(); i++)
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
		if (ct_plotArea->m_dTable)
		{
		}
	odf_context()->chart_context()->end_element();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Spreadsheet::CT_CatAx* ct_catAx)
{
	if (ct_catAx == NULL)return;

	odf_context()->chart_context()->start_axis();
		convert(ct_catAx->m_oSpPr.GetPointer());
		convert(ct_catAx->m_oTxPr.GetPointer());
	///////////////////
		convert(ct_catAx->m_title);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DateAx* ct_dateAx)
{
	if (ct_dateAx == NULL)return;

	odf_context()->chart_context()->start_axis();
		convert(ct_dateAx->m_oSpPr.GetPointer());
		convert(ct_dateAx->m_oTxPr.GetPointer());
	//////////////////
		convert(ct_dateAx->m_title);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SerAx* ct_serAx)
{
	if (ct_serAx == NULL)return;

	odf_context()->chart_context()->start_axis();
		convert(ct_serAx->m_oSpPr.GetPointer());
		convert(ct_serAx->m_oTxPr.GetPointer());
	///////////////////////////
		convert(ct_serAx->m_title);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ValAx* ct_valAx)
{
	if (ct_valAx == NULL)return;

	odf_context()->chart_context()->start_axis();
		convert(ct_valAx->m_oSpPr.GetPointer());
		convert(ct_valAx->m_oTxPr.GetPointer());
	/////////////////////////////
		convert(ct_valAx->m_title);
	odf_context()->chart_context()->end_element();
}
////////////////////////////////////////////////////////////////////////////////////////////
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
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Bar3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bar");
	odf_context()->chart_context()->set_3D(true);
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bar");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Line3DChart *chart)
{
	odf_context()->chart_context()->set_type_chart(L"line");
	odf_context()->chart_context()->set_3D(true);
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"line");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Pie3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	odf_context()->chart_context()->set_3D(true);
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_Surface3DChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"surface");
	odf_context()->chart_context()->set_3D(true);
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"surface");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"bubble");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_DoughnutChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"ring");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"scatter");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"radar");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_StockChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"stock");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_OfPieChart *chart)
{
	if (chart == NULL)return;

	odf_context()->chart_context()->set_type_chart(L"circle");
	
	for (long i=0; i< chart->m_ser.GetCount(); i++)
	{
		convert(chart->m_ser[i]);
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_AreaSer* ser)
{
	odf_context()->chart_context()->start_series(L"area");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BubbleSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bubble");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_xVal, 1);
		convert(ser->m_yVal, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_SurfaceSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"surface");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_PieSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"circle");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_BarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"bar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_ScatterSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"scatter");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_xVal, 1);
		convert(ser->m_yVal, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_RadarSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"radar");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_LineSer* ser)
{
	if (ser == NULL)return;

	odf_context()->chart_context()->start_series(L"line");
		convert(ser->m_oSpPr.GetPointer());
		convert(ser->m_cat, 1);
		convert(ser->m_val, 2);
	odf_context()->chart_context()->end_element();
}
void OoxConverter::convert(OOX::Spreadsheet::CT_AxDataSource* cat, int category)
{
	if (cat == NULL)return;

	if (cat->m_strRef)
	{
		if (cat->m_strRef->m_f)odf_context()->chart_context()->set_series_formula(category,string2std_string(*cat->m_strRef->m_f));
		if (cat->m_strRef->m_strCache){}
	}
	else if (cat->m_numRef)
	{
		if (cat->m_numRef->m_f)odf_context()->chart_context()->set_series_formula(category,string2std_string(*cat->m_numRef->m_f));
		if (cat->m_numRef->m_numCache){}
	}
	else if (cat->m_numLit)
	{
	}
	else if (cat->m_strLit)
	{
	}
}
void OoxConverter::convert(OOX::Spreadsheet::CT_NumDataSource* val, int category)
{
	if (val == NULL)return;
	if (val->m_numRef)
	{
		if (val->m_numRef->m_f)odf_context()->chart_context()->set_series_formula(category,string2std_string(*val->m_numRef->m_f));
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