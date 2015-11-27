
#include "ChartSheetSubstream.h"
#include <Logic/Biff_records/WriteProtect.h>
#include <Logic/Biff_records/SheetExt.h>
#include <Logic/Biff_records/WebPub.h>
#include <Logic/Biff_records/HFPicture.h>
#include <Logic/Biff_records/PrintSize.h>
#include <Logic/Biff_records/HeaderFooter.h>
#include <Logic/Biff_records/Fbi.h>
#include <Logic/Biff_records/Fbi2.h>
#include <Logic/Biff_records/ClrtClient.h>
#include <Logic/Biff_records/Palette.h>
#include <Logic/Biff_records/SXViewLink.h>
#include <Logic/Biff_records/PivotChartBits.h>
#include <Logic/Biff_records/SBaseRef.h>
#include <Logic/Biff_records/MsoDrawingGroup.h>
#include <Logic/Biff_records/Units.h>
#include <Logic/Biff_records/CodeName.h>
#include <Logic/Biff_records/EOF.h>
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/AreaFormat.h>

#include <Logic/Biff_unions/PAGESETUP.h>
#include <Logic/Biff_unions/BACKGROUND.h>
#include <Logic/Biff_unions/PROTECTION_COMMON.h>
#include <Logic/Biff_unions/OBJECTS.h>
#include <Logic/Biff_unions/CHARTFOMATS.h>
#include <Logic/Biff_unions/SERIESDATA.h>
#include <Logic/Biff_unions/WINDOW.h>
#include <Logic/Biff_unions/CUSTOMVIEW.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>

#include <simple_xml_writer.h>

namespace XLS
{;


ChartSheetSubstream::ChartSheetSubstream()
{
}


ChartSheetSubstream::~ChartSheetSubstream()
{
}


BaseObjectPtr ChartSheetSubstream::clone()
{
	return BaseObjectPtr(new ChartSheetSubstream(*this));
}


/*
CHARTSHEETCONTENT = [WriteProtect] [SheetExt] [WebPub] *HFPicture PAGESETUP PrintSize [HeaderFooter] 
					[BACKGROUND] *Fbi *Fbi2 [ClrtClient] [PROTECTION] [Palette] [SXViewLink] 
					[PivotChartBits] [SBaseRef] [MsoDrawingGroup] OBJECTS Units CHARTFOMATS SERIESDATA 
					*WINDOW *CUSTOMVIEW [CodeName] [CRTMLFRT] EOF 
CHARTSHEET = BOF CHARTSHEETCONTENT
*/
const bool ChartSheetSubstream::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<BOF>())
	{
		return false;
	}
	int count = 0 ;
	
	proc.optional<WriteProtect>();
	proc.optional<SheetExt>();
	proc.optional<WebPub>();
	proc.repeated<HFPicture>(0, 0);
	proc.mandatory<PAGESETUP>();
	proc.mandatory<PrintSize>();
	proc.optional<HeaderFooter>();

	if (proc.optional<BACKGROUND>())
	{
		m_BACKGROUND = elements_.back();
		elements_.pop_back();
	}

	count = proc.repeated<Fbi>(0, 0);
	while(count > 0)
	{
		m_arFbi.insert(m_arFbi.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	count = proc.repeated<Fbi2>(0, 0);

	while(count > 0 && m_arFbi.empty())//??? разделить
	{
		m_arFbi.insert(m_arFbi.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	proc.optional<ClrtClient>();
	proc.optional<PROTECTION_COMMON>();
	proc.optional<Palette>();
	proc.optional<SXViewLink>();
	proc.optional<PivotChartBits>();
	proc.optional<SBaseRef>();

    MsoDrawingGroup mso_drawing_group(true);
    proc.optional(mso_drawing_group);

    OBJECTS objects(true);
    if (proc.mandatory(objects))
	{
		m_OBJECTSCHART = elements_.back(); 
		elements_.pop_back();
	}

	proc.mandatory<Units>();
	if ( proc.mandatory<CHARTFORMATS>() )
	{
		m_CHARTFORMATS = elements_.back();
		elements_.pop_back();
	}
	if ( proc.mandatory<SERIESDATA>() )
	{
		m_SERIESDATA = elements_.back();
		elements_.pop_back();
	}
	proc.repeated<WINDOW>(0, 0);
	proc.repeated<CUSTOMVIEW>(0, 0);
	proc.optional<CodeName>();
	proc.optional<CRTMLFRT>();

#pragma message("####################### Some trash records may be skipped here")
	proc.SeekToEOF(); // Thus we skip problems with the trash at the end of the stream (found in Domens.xls)
	
	proc.mandatory<EOF_T>();

	return true;
}

int ChartSheetSubstream::serialize (std::wostream & _stream)
{
	AreaFormat		*chart_area_format	= NULL;
	CHARTFORMATS	*chart_formats		= dynamic_cast<CHARTFORMATS*>(m_CHARTFORMATS.get());
	if (!chart_formats) return 0;

	FRAME			*chart_frame		= dynamic_cast<FRAME*>(chart_formats->m_FRAME.get());
	if (chart_frame)
		chart_area_format				= dynamic_cast<AreaFormat*>(chart_frame->m_AreaFormat.get());
	
	CP_XML_WRITER(_stream)    
	{
		if ((chart_area_format) && (chart_area_format->fInvertNeg))
		{
			CP_XML_NODE(L"c:roundedCorners")
			{
				CP_XML_ATTR(L"val",1);
			}
		}
		CP_XML_NODE(L"c:chart")
		{
			serialize_title (CP_XML_STREAM(), chart_formats->find_label(1), chart_formats->find_default_text(2));
		
			//plot_area_.oox_serialize(CP_XML_STREAM());

			CP_XML_NODE(L"c:legend")
			{
				for (int i = 0 ; i < chart_formats->m_arAXISPARENT.size(); i++)
				{
					//chart_formats->m_arAXISPARENT[i].get()

				}
			}

			CP_XML_NODE(L"c:plotVisOnly")
			{
				CP_XML_ATTR(L"val",1);
			}	
		}
		if (chart_frame)
			chart_frame->serialize(_stream);
		else
		{
			//default spPr ???
		}
	}
	return 0;
}

int ChartSheetSubstream::serialize_title (std::wostream & _stream, const BaseObjectPtr & attached_label, const BaseObjectPtr & dft_text)
{
	ATTACHEDLABEL	*label		= dynamic_cast<ATTACHEDLABEL *>	( attached_label.get() );

	if (label == NULL) return 0;

	//AI *ai	= dynamic_cast<AI *>( label->m_AI.get());
	//if (ai == NULL) return 0;
	//SeriesText * seriesText = dynamic_cast<SeriesText *>(ai->m_SeriesText.get());
	//if ((!seriesText ) || (seriesText->stText.str().empty()) {//тута ветка в дефолтным текстом - подставить ???}

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:title")
		{
			label->serialize(CP_XML_STREAM());
		}	
	}
	return 0;
}

} // namespace XLS

