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

#include "WorksheetSubstream.h"

#include "Biff_unions/BACKGROUND.h"
#include "Biff_unions/BIGNAME.h"
#include "Biff_unions/PROTECTION_COMMON.h" 
#include "Biff_unions/COLUMNS.h"
#include "Biff_unions/SCENARIOS.h"
#include "Biff_unions/SORTANDFILTER.h"
#include "Biff_unions/CELLTABLE.h"
#include "Biff_unions/OBJECTS.h"
#include "Biff_unions/PIVOTVIEW.h"
#include "Biff_unions/DCON.h"
#include "Biff_unions/WINDOW.h"
#include "Biff_unions/CUSTOMVIEW.h"
#include "Biff_unions/SORT.h"
#include "Biff_unions/QUERYTABLE.h"
#include "Biff_unions/PHONETICINFO.h"
#include "Biff_unions/CONDFMTS.h"
#include "Biff_unions/HLINK.h"
#include "Biff_unions/DVAL.h"
#include "Biff_unions/FEAT.h"
#include "Biff_unions/FEAT11.h"
#include "Biff_unions/RECORD12.h"
#include "Biff_unions/SHFMLA_SET.h"

#include "Biff_unions/FORMATTING.h"
#include "Biff_unions/XFS.h"
#include "Biff_unions/STYLES.h"

#include "Biff_records/Uncalced.h"
#include "Biff_records/Index.h"
#include "Biff_unions/GLOBALS.h"
#include "Biff_unions/PAGESETUP.h"
#include "Biff_records/Dimensions.h"
#include "Biff_records/HFPicture.h"
#include "Biff_records/Note.h"
#include "Biff_records/DxGCol.h"
#include "Biff_records/MergeCells.h"
#include "Biff_records/LRng.h"
#include "Biff_records/CodeName.h"
#include "Biff_records/WebPub.h"
#include "Biff_records/Window1.h"
#include "Biff_records/CellWatch.h"
#include "Biff_records/SheetExt.h"
#include "Biff_records/EOF.h"
#include "Biff_records/BOF.h"
#include "Biff_records/DefaultRowHeight.h"
#include "Biff_records/Label.h"
#include "Biff_records/List12.h"
#include "Biff_records/PLV.h"
#include "Biff_records/CFEx.h"
#include "Biff_records/Country.h"
#include "Biff_records/WsBool.h"
#include "Biff_records/ExternSheet.h"
#include "Biff_records/SXAddl.h"
#include "Biff_records/CodePage.h"
#include "Biff_records/XF.h"
#include "Biff_records/Format.h"
#include "Biff_records/Font.h"

#include "Biff_structures/ODRAW/OfficeArtDgContainer.h"

namespace XLS
{;

WorksheetSubstream::WorksheetSubstream(const size_t ws_index) :	CommonSubstream(ws_index)
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
	global_info_ = proc.getGlobalWorkbookInfo();
	global_info_->current_sheet = ws_index_ + 1; 

	global_info_->cmt_rules	= 0;

	int count = 0;
	std::vector<CellRangeRef>	shared_formulas_locations;
	
	if(proc.mandatory<BOF>())
	{
		if (!global_info_->Version) // sheet, not workbook
		{
			BOF *bof = dynamic_cast<BOF*>(elements_.back().get());
			global_info_->Version = bof->vers;
			global_info_->CodePage = 0;

			m_Formating = BaseObjectPtr(new FORMATTING);
			FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());
			if (fmts) fmts->global_info = global_info_;
		}
		elements_.pop_back();
	}
	else return false;

	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE || proc.isBOF(type)) //следующий пошел??
			break;
		if (type == rt_EOF) 
		{
			proc.mandatory<EOF_T>();
			elements_.pop_back();
			break;
		}

		switch(type)
		{
			case rt_Uncalced:
			{
				if (proc.optional<Uncalced>())
				{
					elements_.pop_back();
				}
			}break;
			case rt_Index:
			{
				if (proc.optional<Index>())
				{
					elements_.pop_back();
				}
			}break;
			case rt_CalcRefMode://todooo сделать вариативно по всем проверку
			case rt_CalcMode:
			case rt_PrintRowCol:
			{
				if (proc.mandatory<GLOBALS>())
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
			case rt_ColumnDefault:
			case rt_DefColWidth:
			case rt_ColInfo:
			case rt_ColWidth:
			{
				if (proc.optional<COLUMNS>())
				{
					COLUMNS *columns = dynamic_cast<COLUMNS*>(m_COLUMNS.get());
					if (!columns || (columns && columns->elements_.empty()))
					{
						m_COLUMNS = elements_.back();
					}
					elements_.pop_back();
				}
			}break;
			case rt_DefaultRowHeight_BIFF2:
			{
				if (proc.optional<DefaultRowHeight_BIFF2>())
				{
					m_DefaultRowHeight = elements_.back();
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
			case rt_VCenter:
			case rt_HCenter:
			case rt_Setup:
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
			case rt_BigName:		
			{
				count = proc.repeated<BIGNAME>(0, 0);
				while(count > 0)
				{
					m_arBIGNAME.insert(m_arBIGNAME.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Protect:		
			case rt_ScenarioProtect:		
			case rt_ObjProtect:		
			case rt_Password:		
			{
				if (proc.optional<PROTECTION_COMMON>())
				{
					m_PROTECTION = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_ScenMan:
			{
				if (proc.optional<SCENARIOS>())
				{
					elements_.pop_back();
				}
			}break;
			case rt_Sort:
			case rt_AutoFilterInfo:
			case rt_FilterMode:
			case rt_SortData:
			case rt_DropDownObjIds:
			{
				if (proc.optional<SORTANDFILTER>())// Let it be optional
				{
					if (!m_SORTANDFILTER)
						m_SORTANDFILTER = elements_.back();
					elements_.pop_back();
				}	
			}break;
			case rt_LabelSst://order_history.xls
			case rt_Label://file(6).xls
			case rt_Label_BIFF2:
			case rt_Row:
			case rt_Row_BIFF2:
			case rt_Blank:
			{
				CELLTABLE cell_table_temlate(shared_formulas_locations);
				if (proc.optional(cell_table_temlate))
				{
					if (m_CELLTABLE)
					{//Daily Flash Report - Demand_Store_427.xls
						CELLTABLE *cell_table = dynamic_cast<CELLTABLE*>(elements_.back().get());
						cell_table->isConcatinate_ = true;
					}
					else m_CELLTABLE = elements_.back(); 
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
				OBJECTS objects_template(false);
				if (proc.optional(objects_template))
				{
					if (!m_OBJECTS) m_OBJECTS = elements_.back();
					else
					{
						Log::warning(L"Double set OBJECTS!!!");
						OBJECTS *objects = dynamic_cast<OBJECTS*>(elements_.back().get());
						OBJECTS *prev_objects = dynamic_cast<OBJECTS*>(m_OBJECTS.get());

						prev_objects->elements_.insert(prev_objects->elements_.end(), objects->elements_.begin(), objects->elements_.end());

					}
					elements_.pop_back();
				}
			}break;
			case rt_HFPicture:		
			{
				count = proc.repeated<HFPicture>(0, 0);		
				while(count > 0)
				{
					m_arHFPicture.insert(m_arHFPicture.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_CommentText:
			{
				count = proc.repeated<CommentText>(0, 0);
				while(count > 0)
				{
					m_arNote.insert(m_arNote.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
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
			case rt_SxView:			
			{
				count = proc.repeated<PIVOTVIEW>(0, 0);		
				while(count > 0)
				{
					m_arPIVOTVIEW.insert(m_arPIVOTVIEW.begin(), elements_.back());
					elements_.pop_back();
					count--;

					PIVOTVIEW *view = dynamic_cast<PIVOTVIEW*>(m_arPIVOTVIEW.back().get());
					global_info_->mapPivotTableViews.insert(std::make_pair(view->name, m_arPIVOTVIEW.back()));
				}
			}break;
			case rt_DCon:
			{
				if (proc.optional<DCON>())
				{
					m_DCON = elements_.back();
					elements_.pop_back();
				}
			}break;
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
					global_info_->sheets_info.back().defaultColumnWidth = dx->dxgCol / 256.;
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
				
			case rt_LRng:
			{
				if (proc.optional<LRng>())
				{
					m_LRng = elements_.back();
					elements_.pop_back(); 
				}
			}break;
			case rt_Qsi:			
			{
				count = proc.repeated<QUERYTABLE>(0, 0);
				while(count > 0)
				{
					m_arQUERYTABLE.insert(m_arQUERYTABLE.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_PhoneticInfo:	proc.optional<PHONETICINFO>	();			break;			
			case rt_CondFmt:
			case rt_CondFmt12:
			{				
				if (proc.optional<CONDFMTS>())
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
			case rt_DVal:	
			{
				if (proc.optional<DVAL>())
				{
					m_DVAL = elements_.back();
					elements_.pop_back();
				}
			}break;
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
			case rt_List12://LCA BI - Financial Report Usage2010.xls ??
			{
				count = proc.repeated<List12>	(0, 0);
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
			case rt_CFEx:	//Calculadora.xls не в FORMATING
			{
				count = proc.repeated<CFEx>	(0, 0);
			}break;
			case rt_PLV:	//Calculadora.xls не в FORMATING
			{
				if (proc.optional<PLV>())
				{
					m_PLV = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_HeaderFooter:		
			{
				count = proc.repeated<RECORD12>	(0, 0);		
				while(count > 0)
				{
					m_arRECORD12.insert(m_arRECORD12.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Window1:	//biff5
			{
				count = proc.repeated<Window1>(0, 0);
				while(count > 0)
				{
					//m_arWindow1.insert(m_arWindow1.begin(), elements_.back());
					elements_.pop_back();
					count--;
				}
			}break;
			case rt_Country:	//biff5
			{
				if (proc.optional<Country>())
				{
					Country *Country_ = dynamic_cast<Country*>(elements_.back().get());
					if (Country_)
					{
						int countryDef = Country_->iCountryDef;
						int countryWinIni = Country_->iCountryWinIni;

						global_info_->CodePage;
					}

					//m_Country = elements_.back();
					elements_.pop_back();
				}	
			}break;
			case rt_WsBool:	//biff5
			{
				bool is_dialog;
				WsBool wsbool_template(is_dialog);
				if (proc.optional(wsbool_template)) // The most interesting
				{
					//m_WsBool = elements_.back();
					elements_.pop_back();
				}
			}break;	
			case rt_ExternSheet:	//biff5
			{
				if (proc.optional<ExternSheet>())
				{
					//m_ExternSheet = elements_.back(); 
					elements_.pop_back();
				}
			}break;
			case rt_CodePage:
			{
				if (proc.optional<CodePage>())				
				{
					CodePage *CodePage_ = dynamic_cast<CodePage*>(elements_.back().get());

					if ((CodePage_) && (CodePage_->cv != 0/* && CodePage_->cv != 1200*/))
						global_info_->CodePage = CodePage_->cv;

					if (global_info_->CodePage == 0x8001 &&
						(global_info_->Version == 0x0200 || global_info_->Version == 0x0300))
						global_info_->CodePage = 1252;

					elements_.pop_back();
				}
			}break;
			case rt_XF_BIFF2:
			case rt_XF_BIFF3:
			case rt_XF_BIFF4:
			{
				size_t cell_xf_current_id = 0;
				size_t style_xf_current_id = 0;
				int count = 0;
				
				if (type == rt_XF_BIFF4)
				{
					XF_BIFF4 xf(cell_xf_current_id, style_xf_current_id);
					count = proc.repeated(xf, 0, 0);
				}
				else if (type == rt_XF_BIFF3)
				{
					XF_BIFF3 xf(cell_xf_current_id, style_xf_current_id);
					count = proc.repeated(xf, 0, 0);
				}
				else if (type == rt_XF_BIFF2)
				{
					XF_BIFF2 xf(cell_xf_current_id, style_xf_current_id);
					count = proc.repeated(xf, 0, 0);
				}
				XFS* xfs = new XFS();
				int ind = 0;
				while (count > 0 && !elements_.empty())
				{
					XF* xf = dynamic_cast<XF*>(elements_.front().get());
					if (xf)
					{
						xf->ind_xf = ind++;

						if (xf->fStyle)	xfs->m_arCellStyles.push_back(elements_.front());
						else			xfs->m_arCellXFs.push_back(elements_.front());
					}
					elements_.pop_front();
					count--;
				}
				global_info_->cellXfs_count = m_arCellXFs.size();
				global_info_->cellStyleXfs_count = m_arCellStyles.size();

				FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());
				if (fmts)
					fmts->m_XFS = BaseObjectPtr(xfs);
			}break;
			case rt_Font:
			case rt_Font_BIFF34:
			{
				elements_.clear();
				count = (type == rt_Font) ? proc.repeated<Font>(0, 0) : proc.repeated<Font_BIFF34>(0, 0);
				while (count > 0)
				{
					global_info_->m_arFonts.push_back(elements_.front());
					elements_.pop_front();
					count--;
				}
			}break;
			case rt_Format:
			case rt_Format_BIFF23:
			{
				elements_.clear();
				if (type == rt_Format_BIFF23)	count = proc.repeated<Format_BIFF23>(0, 0);
				else							count = proc.repeated<Format>(0, 0);

				while (count > 0)
				{
					Format *fmt = dynamic_cast<Format *>(elements_.front().get());
					if ((fmt) && (fmt->ifmt == 0xffff))
					{
						std::map<std::wstring, int>::iterator pFind = global_info_->mapDefaultFormatCode.find(fmt->stFormat);
						if (pFind != global_info_->mapDefaultFormatCode.end())
						{
							fmt->ifmt = pFind->second;
						}
						else
						{
							fmt->ifmt = 168 + global_info_->m_arNumFormats.size();
						}
					}
					global_info_->m_arNumFormats.push_back(elements_.front());
					elements_.pop_front();
					count--;
				} 
			}break;
			case rt_Style:
			{
				if (proc.optional<STYLES>())
				{
					FORMATTING* fmts = dynamic_cast<FORMATTING*>(m_Formating.get());
					if (fmts)
						fmts->m_Styles = elements_.back();
					elements_.pop_back();
				}
			}break;
			default://unknown .... skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}	

	LoadHFPicture();

	return true;
}

} // namespace XLS

