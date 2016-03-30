
#include "WorksheetSubstream.h"
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Index.h>
#include <Logic/Biff_unions/GLOBALS.h>
#include <Logic/Biff_unions/PAGESETUP.h>
#include <Logic/Biff_records/Dimensions.h>
#include <Logic/Biff_records/HFPicture.h>
#include <Logic/Biff_records/Note.h>
#include <Logic/Biff_records/DxGCol.h>
#include <Logic/Biff_records/MergeCells.h>
#include <Logic/Biff_records/LRng.h>
#include <Logic/Biff_records/CodeName.h>
#include <Logic/Biff_records/WebPub.h>
#include <Logic/Biff_records/Window1.h>
#include <Logic/Biff_records/CellWatch.h>
#include <Logic/Biff_records/SheetExt.h>
#include <Logic/Biff_records/EOF.h>
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/DefaultRowHeight.h>

#include <Logic/Biff_unions/BACKGROUND.h>
#include <Logic/Biff_unions/BIGNAME.h>
#include <Logic/Biff_unions/PROTECTION_COMMON.h> // special name to exclude ambiguous meaning
#include <Logic/Biff_unions/COLUMNS.h>
#include <Logic/Biff_unions/SCENARIOS.h>
#include <Logic/Biff_unions/SORTANDFILTER.h>
#include <Logic/Biff_unions/CELLTABLE.h>
#include <Logic/Biff_unions/OBJECTS.h>
#include <Logic/Biff_unions/PIVOTVIEW.h>
#include <Logic/Biff_unions/DCON.h>
#include <Logic/Biff_unions/WINDOW.h>
#include <Logic/Biff_unions/CUSTOMVIEW.h>
#include <Logic/Biff_unions/SORT.h>
#include <Logic/Biff_unions/QUERYTABLE.h>
#include <Logic/Biff_unions/PHONETICINFO.h>
#include <Logic/Biff_unions/CONDFMTS.h>
#include <Logic/Biff_unions/HLINK.h>
#include <Logic/Biff_unions/DVAL.h>
#include <Logic/Biff_unions/FEAT.h>
#include <Logic/Biff_unions/FEAT11.h>
#include <Logic/Biff_unions/RECORD12.h>
#include <Logic/Biff_unions/SHFMLA_SET.h>

namespace XLS
{;


WorksheetSubstream::WorksheetSubstream(const size_t ws_index)
:	ws_index_(ws_index)
{
}


WorksheetSubstream::~WorksheetSubstream()
{
}


BaseObjectPtr WorksheetSubstream::clone()
{
	return BaseObjectPtr(new WorksheetSubstream(*this));
}


/*
WORKSHEETCONTENT = [Uncalced] Index GLOBALS PAGESETUP [HeaderFooter] [BACKGROUND] *BIGNAME [PROTECTION] 
					COLUMNS [SCENARIOS] SORTANDFILTER Dimensions [CELLTABLE] OBJECTS *HFPicture *Note 
					*PIVOTVIEW [DCON] 1*WINDOW *CUSTOMVIEW *2SORT [DxGCol] *MergeCells [LRng] *QUERYTABLE 
					[PHONETICINFO] CONDFMTS *HLINK [DVAL] [CodeName] *WebPub *CellWatch [SheetExt] *FEAT 
					*FEAT11 *RECORD12 EOF
WORKSHEET = BOF WORKSHEETCONTENT
*/
const bool WorksheetSubstream::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();
	
	GlobalWorkbookInfo::_sheet_size_info sheet_size_info;
	global_info->sheet_size_info.push_back(sheet_size_info);
	global_info->current_sheet = global_info->sheet_size_info.size();

	global_info->cmt_rules	= 0;

	int count = 0;
	std::vector<CellRangeRef>	shared_formulas_locations;
	
	if(!proc.mandatory<BOF>())
	{
		return false;
    }

	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE || type == rt_BOF) //следующий пошел??
			break;
		if (type == rt_EOF) 
		{
			proc.mandatory<EOF_T>();
			break;
		}

		switch(type)
		{
			case rt_Uncalced:		proc.optional<Uncalced>();		break;
			case rt_Index:			proc.optional<Index>();			break;
			case rt_CalcRefMode:
			case rt_CalcMode:
			{
				GLOBALS globals(false);
				if (proc.mandatory(globals))
				{
					m_GLOBALS = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Dimensions:
			{
				if (proc.optional<Dimensions>())
				{
					m_Dimensions = elements_.back();
					elements_.pop_back();
				}		
			}break;
			case rt_Window2:
			{
				count = proc.repeated<WINDOW>(0, 0);
				while(count > 0)
				{
					m_arWINDOW.insert(m_arWINDOW.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_DefColWidth:
			case rt_ColInfo:
			{
				if (proc.optional<COLUMNS>())
				{
					if (!m_COLUMNS)//???
						m_COLUMNS = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_DefaultRowHeight:
			{
				if (proc.optional<DefaultRowHeight>())
				{
					m_DefaultRowHeight = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Header:
			case rt_Footer:		
			case rt_BottomMargin:
			case rt_TopMargin:
			case rt_LeftMargin:
			case rt_RightMargin:
			{
				if (proc.mandatory<PAGESETUP>())
				{
					if (!m_PAGESETUP)
						m_PAGESETUP = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_BkHim:
			{
				if (proc.optional<BACKGROUND>())
				{
					m_BACKGROUND = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_BigName:		proc.repeated<BIGNAME>(0, 0);		break;
			case rt_Protect:		proc.optional<PROTECTION_COMMON>();	break;
			case rt_ScenMan:		proc.optional<SCENARIOS>();			break;	
			case rt_Sort:
			case rt_AutoFilterInfo:
			{
				if (proc.optional<SORTANDFILTER>())// Let it be optional
				{
					m_SORTANDFILTER = elements_.back();
					elements_.pop_back();
				}	
			}break;
			case rt_Row:
			{
				CELLTABLE        cell_table(shared_formulas_locations);
				if (proc.optional(cell_table))
				{
					m_CELLTABLE = elements_.back();
					elements_.pop_back();
				}
				if(0 != shared_formulas_locations.size())
				{
					SHFMLA_SET shfmla_set(shared_formulas_locations);
			       
					if (proc.optional(shfmla_set))
					{
						m_SHFMLA_SET = elements_.back();
						elements_.pop_back();
					}
				}
			}break;
			case rt_Obj:
			case rt_MsoDrawing:
			{
				OBJECTS objects(false);
				if (proc.optional(objects))
				{
					if (!m_OBJECTS) m_OBJECTS = elements_.back();
					else
					{
						Log::warning(L"Double set OBJECTS!!!");
					}
					elements_.pop_back();
				}
			}break;
			case rt_HFPicture:		proc.repeated<HFPicture>(0, 0);		break;
			case rt_Note:
			{
				count = proc.repeated<Note>(0, 0);
				while(count > 0)
				{
					m_arNote.insert(m_arNote.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_SxView:			proc.repeated<PIVOTVIEW>(0, 0);		break;
			case rt_DCon:			proc.optional<DCON>		();			break;
			case rt_UserSViewBegin:
			{
				count = proc.repeated<CUSTOMVIEW>(0, 0);
				while(count > 0)
				{
					m_arCUSTOMVIEW.insert(m_arCUSTOMVIEW.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_RRSort:
			{
				count = proc.repeated<SORT>(0, 2);
				while(count > 0)
				{
					m_arSORT.insert(m_arSORT.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_DxGCol:
			{				
				if (proc.optional<DxGCol>())
				{
					m_DxGCol = elements_.back();
					elements_.pop_back(); 
					
					DxGCol* dx = dynamic_cast<DxGCol*>(m_DxGCol.get());
					global_info->sheet_size_info.back().defaultColumnWidth = dx->dxgCol / 256.;
				}
			}break;				
			case rt_MergeCells:
			{
				count = proc.repeated<MergeCells>(0, 0);
				while(count > 0)
				{
					MergeCells* m = dynamic_cast<MergeCells*>(elements_.back().get());
					if ((m) && (m->rgref.size() > 0))
					{
						m_arMergeCells.insert(m_arMergeCells.begin(), elements_.back());
					}
					elements_.pop_back();
					count--;
				}
			}break;
				
			case rt_LRng:			proc.optional<LRng>			();			break;
			case rt_Qsi:			proc.repeated<QUERYTABLE>	(0, 0);		break;
			case rt_PhoneticInfo:	proc.optional<PHONETICINFO>	();			break;			
			case rt_CondFmt:
			case rt_CondFmt12:
			{				
				if (proc.optional<CONDFMTS>())  // Let it be optional
				{
					m_CONDFMTS = elements_.back();
					elements_.pop_back();
				}
			}break;				
			case rt_HLink:
			{				
				count = proc.repeated<HLINK>(0, 0) ;
				while(count > 0)
				{
					m_arHLINK.insert(m_arHLINK.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;				
			case rt_DVal:			proc.optional<DVAL>();			break;
			case rt_CodeName:
			{					
				if (proc.optional<CodeName>	())
				{
					m_CodeName = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_WebPub:			proc.repeated<WebPub>	(0, 0);	break;
			case rt_CellWatch:		proc.repeated<CellWatch>(0, 0);	break;
			//case ExternCount:0x16
			//	{
			//	}break;
			case rt_SheetExt:
			{				
				if (proc.optional<SheetExt>())
				{
					m_SheetExt = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_FeatHdr:
			{
				count = proc.repeated<FEAT>		(0, 0);
				while(count > 0)
				{
					m_arFEAT.insert(m_arFEAT.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_FeatHdr11:
			{
				count = proc.repeated<FEAT11>	(0, 0);
				while(count > 0)
				{
					m_arFEAT11.insert(m_arFEAT11.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_HeaderFooter:		proc.repeated<RECORD12>	(0, 0);		break;

			default://unknown .... skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}	

	return true;
}


} // namespace XLS

