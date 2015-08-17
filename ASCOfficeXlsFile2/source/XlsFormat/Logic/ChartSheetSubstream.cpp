
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

#include <Logic/Biff_unions/PAGESETUP.h>
#include <Logic/Biff_unions/BACKGROUND.h>
#include <Logic/Biff_unions/PROTECTION_COMMON.h>
#include <Logic/Biff_unions/OBJECTS.h>
#include <Logic/Biff_unions/CHARTFOMATS.h>
#include <Logic/Biff_unions/SERIESDATA.h>
#include <Logic/Biff_unions/WINDOW.h>
#include <Logic/Biff_unions/CUSTOMVIEW.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

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
	proc.optional<WriteProtect>();
	proc.optional<SheetExt>();
	proc.optional<WebPub>();
	proc.repeated<HFPicture>(0, 0);
	proc.mandatory<PAGESETUP>();
	proc.mandatory<PrintSize>();
	proc.optional<HeaderFooter>();
	proc.optional<BACKGROUND>();
	proc.repeated<Fbi>(0, 0);
	proc.repeated<Fbi2>(0, 0);
	proc.optional<ClrtClient>();
	proc.optional<PROTECTION_COMMON>();
	proc.optional<Palette>();
	proc.optional<SXViewLink>();
	proc.optional<PivotChartBits>();
	proc.optional<SBaseRef>();

    MsoDrawingGroup mso_drawing_group(true);
    proc.optional(mso_drawing_group);

    OBJECTS objects(true);
    proc.mandatory(objects);

	proc.mandatory<Units>();
	proc.mandatory<CHARTFOMATS>();
	proc.mandatory<SERIESDATA>();
	proc.repeated<WINDOW>(0, 0);
	proc.repeated<CUSTOMVIEW>(0, 0);
	proc.optional<CodeName>();
	proc.optional<CRTMLFRT>();

#pragma message("####################### Some trash records may be skipped here")
	proc.SeekToEOF(); // Thus we skip problems with the trash at the end of the stream (found in Domens.xls)
	
	proc.mandatory<EOF_T>();

	return true;
}

} // namespace XLS

