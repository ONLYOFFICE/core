
#include "GLOBALS.h"
#include <Logic/Biff_records/DefColWidth.h>

#include <Logic/Biff_records/Protect.h>
#include <Logic/Biff_records/CalcMode.h>
#include <Logic/Biff_records/CalcCount.h>
#include <Logic/Biff_records/CalcRefMode.h>
#include <Logic/Biff_records/CalcIter.h>
#include <Logic/Biff_records/CalcDelta.h>
#include <Logic/Biff_records/CalcSaveRecalc.h>
#include <Logic/Biff_records/PrintRowCol.h>
#include <Logic/Biff_records/PrintGrid.h>
#include <Logic/Biff_records/GridSet.h>
#include <Logic/Biff_records/Guts.h>
#include <Logic/Biff_records/DefaultRowHeight.h>
#include <Logic/Biff_records/WsBool.h>
#include <Logic/Biff_records/Sync.h>
#include <Logic/Biff_records/LPr.h>
#include <Logic/Biff_records/HorizontalPageBreaks.h>
#include <Logic/Biff_records/VerticalPageBreaks.h>

#include <simple_xml_writer.h>

namespace XLS
{;


GLOBALS::GLOBALS(const bool is_dialog_sheet)
:	is_dialog(is_dialog_sheet)
{
}


GLOBALS::~GLOBALS()
{
}


BaseObjectPtr GLOBALS::clone()
{
	return BaseObjectPtr(new GLOBALS(*this));
}


/*
GLOBALS = CalcMode CalcCount CalcRefMode CalcIter CalcDelta CalcSaveRecalc PrintRowCol PrintGrid GridSet 
			Guts DefaultRowHeight WsBool [Sync] [LPr] [HorizontalPageBreaks] [VerticalPageBreaks]
*/
const bool GLOBALS::loadContent(BinProcessor& proc)
{
// 	if(!proc.mandatory<CalcMode>())
// 	{
// 		return false;
// 	}
	proc.optional<Protect>();
	proc.optional<CalcMode>(); // OpenOffice Calc stored files workaround (CalcMode is mandatory according to [MS-XLS])	
	proc.mandatory<CalcCount>();
	proc.mandatory<CalcRefMode>();
	proc.mandatory<CalcIter>();
	proc.mandatory<CalcDelta>();
	proc.mandatory<CalcSaveRecalc>();
	proc.optional<PrintRowCol>();// OpenOffice Calc stored files workaround (PrintRowCol is mandatory according to [MS-XLS])
	proc.optional<PrintGrid>();// OpenOffice Calc stored files workaround (PrintGrid is mandatory according to [MS-XLS])
	proc.optional<GridSet>();// OpenOffice Calc stored files workaround (GridSet is mandatory according to [MS-XLS])
	
	if (proc.mandatory<Guts>())
	{
		m_Guts = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<DefaultRowHeight>())
	{
		m_DefaultRowHeight = elements_.back();
		elements_.pop_back();
	}
	
	proc.mandatory(WsBool(is_dialog)); // The most interesting
	proc.optional<Sync>();
	proc.optional<LPr>();
	proc.optional<HorizontalPageBreaks>();
	proc.optional<VerticalPageBreaks>();
	proc.optional<PrintRowCol>();// OpenOffice Calc stored files workaround (PrintRowCol is mandatory according to [MS-XLS])
	proc.optional<PrintGrid>();// OpenOffice Calc stored files workaround (PrintGrid is mandatory according to [MS-XLS])
	proc.optional<GridSet>();// OpenOffice Calc stored files workaround (GridSet is mandatory according to [MS-XLS])
	return true;
}

int GLOBALS::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sheetFormatPr")
		{
			if (m_DefaultRowHeight)
			{
				DefaultRowHeight* def_row = dynamic_cast<DefaultRowHeight*>(m_DefaultRowHeight.get());
				if (def_row)
				{
					if(!def_row->fDyZero.value() || ((def_row->fDyZero.value()) && (!def_row->fDyZero)))
						CP_XML_ATTR(L"defaultRowHeight", def_row->miyRw / 20.);
					else 
					{
						CP_XML_ATTR(L"zeroHeight", true);
						CP_XML_ATTR(L"defaultRowHeight", def_row->miyRwHidden /20.);
					}
					if ((def_row->fExAsc.value()) && (def_row->fExAsc))	CP_XML_ATTR(L"thickTop", true);
					if ((def_row->fExDsc.value()) && (def_row->fExDsc))	CP_XML_ATTR(L"thickBottom", true);
				}

			}
			else
			{
				CP_XML_ATTR(L"defaultRowHeight", 14.4);
			}
			if (m_DefColWidth)
			{
				DefColWidth* def_col = dynamic_cast<DefColWidth*>(m_DefColWidth.get());
				CP_XML_ATTR(L"defaultColWidth", def_col->cchdefColWidth);
			}
			if (m_Guts)
			{
				Guts * guts= dynamic_cast<Guts*>(m_Guts.get());

				if ((guts->iLevelRwMac.value()) && (guts->iLevelRwMac > 0)) 
					CP_XML_ATTR(L"outlineLevelRow", guts->iLevelRwMac);
				
				if ((guts->iLevelColMac.value()) && (guts->iLevelColMac > 0)) 
					CP_XML_ATTR(L"outlineLevelCol", guts->iLevelColMac);
			}
		}
	}
	return 0;
}
} // namespace XLS

