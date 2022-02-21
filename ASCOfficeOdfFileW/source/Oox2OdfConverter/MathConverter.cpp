﻿#include "Converter.h"

//#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/DocxFlat.h"

#include "../OdfFormat/odf_conversion_context.h"
#include "../../ASCOfficeOdfFileW/source/OdfFormat/math_layout_elements.h"
#include "../../ASCOfficeOdfFileW/source/OdfFormat/math_limit_elements.h"
#include "../../ASCOfficeOdfFileW/source/OdfFormat/math_token_elements.h"
#include <set>
#include <vector>
#include <fstream>
#include <string>
namespace Oox2Odf
{
	std::vector<std::vector<std::wstring>>& OoxConverter::brackets()
	{
		return odf_context()->math_context()->brackets;
	}

	int& OoxConverter::lvl_of_me()
	{
		return odf_context()->math_context()->lvl_of_me;
	}

	std::vector<int>& OoxConverter::end_counter()
	{
		return odf_context()->math_context()->end_counter;
	}

	void OoxConverter::mrow() // обертка для тега <mrow>
	{
		CREATE_MATH_TAG(L"mrow");
		OPEN_MATH_TAG(elm);		
	}

	void OoxConverter::endOfMrow() // закрывашка тега <mrow>
	{
		CLOSE_MATH_TAG
	}

	void OoxConverter::resizeBrackets()
	{
		lvl_of_me()++;
		brackets().resize(brackets().size() + 1);
	}

	void OoxConverter::convert(OOX::Logic::COMath *oox_math)
	{
		if (!oox_math) return;
		
		brackets().resize(1);

		bool bStart = odf_context()->start_math();

		
		for (size_t i = 0; i < oox_math->m_arrItems.size(); ++i)
		{
			convert(oox_math->m_arrItems[i]);
		}		

		if (bStart) odf_context()->end_math();
	}

	void OoxConverter::convert(OOX::Logic::CMathPr *oox_math_pr)
	{
		if (!oox_math_pr) return;

//		for (size_t i = 0; i < oox_math_pr->m_arrItems.size(); ++i)
//		{
//			convert(oox_math_pr->m_arrItems[i]);
//		}
        convert(oox_math_pr->m_oBrkBin.GetPointer());
        convert(oox_math_pr->m_oBrkBinSub.GetPointer());
        convert(oox_math_pr->m_oDefJc.GetPointer());
        convert(oox_math_pr->m_oDispDef.GetPointer());
        convert(oox_math_pr->m_oInterSp.GetPointer());
        convert(oox_math_pr->m_oIntLim.GetPointer());
        convert(oox_math_pr->m_oIntraSp.GetPointer());
        convert(oox_math_pr->m_oLMargin.GetPointer());
        convert(oox_math_pr->m_oMathFont.GetPointer());
        convert(oox_math_pr->m_oNaryLim.GetPointer());
        convert(oox_math_pr->m_oPostSp.GetPointer());
        convert(oox_math_pr->m_oPreSp.GetPointer());
        convert(oox_math_pr->m_oRMargin.GetPointer());
        convert(oox_math_pr->m_oSmallFrac.GetPointer());
        convert(oox_math_pr->m_oWrapIndent.GetPointer());
        convert(oox_math_pr->m_oWrapRight.GetPointer());
    }

	void OoxConverter::convert(OOX::Logic::COMathPara *oox_math_para)
	{
		if (!oox_math_para) return;

		odf_context()->start_math();

		for (size_t i = 0; i < oox_math_para->m_arrItems.size(); ++i)
		{
			convert(oox_math_para->m_arrItems[i]);
		}
		odf_context()->end_math();
	}

	void OoxConverter::convert(OOX::Logic::COMathParaPr *oox_math_para_pr)
	{
		if (!oox_math_para_pr) return;
	}

	bool OoxConverter::convert(OOX::Logic::CCtrlPr *oox_ctrl_pr)
	{
		if (!oox_ctrl_pr) return false;		

		convert(oox_ctrl_pr->m_oARPr.GetPointer());
		convert(oox_ctrl_pr->m_oDel.GetPointer());
		convert(oox_ctrl_pr->m_oIns.GetPointer());		
		return true;
	}

	void OoxConverter::convert(OOX::Logic::CAcc *oox_acc)
	{
		if (!oox_acc) return;		

		CREATE_MATH_TAG(L"mover");

		/*typedef odf_writer::math_mover* T;
		T tmp = dynamic_cast<T>(elm.get());
		if (tmp)
		{
			tmp->accent = true;
		}*/
		
		OPEN_MATH_TAG(elm);

		convert(oox_acc->m_oElement.GetPointer());
		//convert(oox_acc->m_oAccPr.GetPointer());
		std::wstring diakSymbol = (oox_acc->m_oAccPr->m_oChr.IsInit()) ? oox_acc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"̂";
		
		std::map<std::wstring, std::wstring>& map = odf_context()->math_context()->diakSymbols;
		std::wstring symbol;		
		
		symbol = (map[diakSymbol]);
		{
			CREATE_MATH_TAG(L"mo");
			/*typedef odf_writer::math_mo* T;
			T tmp = dynamic_cast<T>(elm.get());
			if (tmp)
			{
				tmp->stretchy_ = false;
			}*/
			elm->add_text(symbol);
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;

		}	
		CLOSE_MATH_TAG;
	}

	void OoxConverter::convert(OOX::Logic::CAccPr	*oox_acc_pr)
	{
		if (!oox_acc_pr) return;	

		convert(oox_acc_pr->m_oChr.GetPointer());
		convert(oox_acc_pr->m_oCtrlPr.GetPointer());
		
	}

	void OoxConverter::convert(OOX::Logic::CArgPr *oox_arg_pr)
	{
		if (!oox_arg_pr) return;		

		convert(oox_arg_pr->m_oArgSz.GetPointer());		
	}


	void OoxConverter::convert(OOX::Logic::CArgSz *oox_arg_sz)
	{
		if (!oox_arg_sz) return;
	}

	void OoxConverter::convert(OOX::Logic::CBar *oox_bar)
	{
		if (!oox_bar) return;
		
		bool flag = convert(oox_bar->m_oBarPr.GetPointer());
		std::wstring tag;
		if (flag) tag = L"mover";
		else	  tag = L"munder";
		CREATE_MATH_TAG(tag.c_str());
		OPEN_MATH_TAG(elm);
		{
			convert(oox_bar->m_oElement.GetPointer());
			CREATE_MATH_TAG(L"mo");
			if (flag) elm->add_text(L"¯");
			else	  elm->add_text(L"&#713;");
			
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;
		}

		CLOSE_MATH_TAG;

	}

	bool OoxConverter::convert(OOX::Logic::CBarPr	*oox_bar_pr)
	{
		if (!oox_bar_pr) return false;	

		convert(oox_bar_pr->m_oCtrlPr.GetPointer());
		return convert(oox_bar_pr->m_oPos.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CBorderBox *oox_border_box)
	{
		if (!oox_border_box) return;

		convert(oox_border_box->m_oBorderBoxPr.GetPointer());
		convert(oox_border_box->m_oElement.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CBorderBoxPr *oox_border_box_pr)
	{
		if (!oox_border_box_pr) return;


		convert(oox_border_box_pr->m_oCtrlPr.GetPointer());
		convert(oox_border_box_pr->m_oHideBot.GetPointer());
		convert(oox_border_box_pr->m_oHideLeft.GetPointer());
		convert(oox_border_box_pr->m_oHideRight.GetPointer());
		convert(oox_border_box_pr->m_oHideTop.GetPointer());
		convert(oox_border_box_pr->m_oStrikeBLTR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeH.GetPointer());
		convert(oox_border_box_pr->m_oStrikeTLBR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeV.GetPointer());
		
	}

	void OoxConverter::convert(OOX::Logic::CBox *oox_box)
	{
		if (!oox_box) return;


		convert(oox_box->m_oBoxPr.GetPointer());
		convert(oox_box->m_oElement.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CBoxPr *oox_box_pr)
	{
		if (!oox_box_pr) return;


		convert(oox_box_pr->m_oAln.GetPointer());
		convert(oox_box_pr->m_oBrk.GetPointer());
		convert(oox_box_pr->m_oCtrlPr.GetPointer());
		convert(oox_box_pr->m_oDiff.GetPointer());
		convert(oox_box_pr->m_oNoBreak.GetPointer());
		convert(oox_box_pr->m_oOpEmu.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CNoBreak *oox_no_break)
	{
		if (!oox_no_break) return;
	}

	void OoxConverter::convert(OOX::Logic::COpEmu *oox_op_emu)
	{
		if (!oox_op_emu) return;
	}

	void OoxConverter::convert(OOX::Logic::CDiff *oox_diff)
	{
		if (!oox_diff) return;
	}

	void OoxConverter::convert(OOX::Logic::CBrk *oox_brk)
	{
		if (!oox_brk) return;
	}

	void OoxConverter::convert(OOX::Logic::CDelimiter *oox_del)
	{
		if (!oox_del) return;
		
		std::pair<std::wstring, std::wstring> begEndChrs;
		for (size_t i = 0; i < oox_del->m_arrItems.size(); ++i)
		{
			if(oox_del->m_arrItems[i]->getType() == OOX::et_m_dPr)
				begEndChrs = convert((OOX::Logic::CDelimiterPr*)(oox_del->m_arrItems[i]));
		}
		mrow();
		{
			CREATE_MATH_TAG(L"mo");
			elm->add_text(begEndChrs.first);
			typedef odf_writer::math_mo* T;

			T tmp = dynamic_cast<T>(elm.get());
			if (tmp)
			{
				tmp->fence_ = true;
				tmp->form_ = L"prefix";
				tmp->stretchy_ = true;
			}
			OPEN_MATH_TAG(elm);
			
			CLOSE_MATH_TAG;
		}
	
		for (size_t i = 0; i < oox_del->m_arrItems.size(); ++i)
		{
			if (!(oox_del->m_arrItems[i]->getType() == OOX::et_m_dPr))
				convert(oox_del->m_arrItems[i]);
		}

		{
			CREATE_MATH_TAG(L"mo");
			elm->add_text(begEndChrs.second);
			typedef odf_writer::math_mo* T;

			T tmp = dynamic_cast<T>(elm.get());
			if (tmp)
			{
				tmp->fence_ = true;
				tmp->form_ = L"postfix";
				tmp->stretchy_ = true;
			}
			OPEN_MATH_TAG(elm)
			
			CLOSE_MATH_TAG
		}	
		endOfMrow();
	}

	std::pair<std::wstring, std::wstring> OoxConverter::convert(OOX::Logic::CDelimiterPr *oox_del_pr)
	{
		std::pair<std::wstring, std::wstring> result(L"(", L")");

		if (!oox_del_pr) return result;


		result.first = convert(oox_del_pr->m_oBegChr.GetPointer());
		convert(oox_del_pr->m_oCtrlPr.GetPointer());
		convert(oox_del_pr->m_oGrow.GetPointer());
		convert(oox_del_pr->m_oSepChr.GetPointer());
		convert(oox_del_pr->m_oShp.GetPointer());
		result.second = convert(oox_del_pr->m_oEndChr.GetPointer());
		return result;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CBegChr * oox_beg_chr)
	{
		if (!oox_beg_chr) return L"(";

		std::wstring s_val = oox_beg_chr->m_val->GetValue();
		brackets()[lvl_of_me()].push_back(s_val);
		return s_val;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CEndChr * oox_end_chr)
	{
		if (!oox_end_chr) return L")";

		std::wstring s_val = oox_end_chr->m_val->GetValue();
		brackets()[lvl_of_me()].push_back(s_val);
		return s_val;
	}

	void OoxConverter::convert(OOX::Logic::CEqArr *oox_eq_arr)
	{
		if (!oox_eq_arr) return;
		
		if(oox_eq_arr->m_arrItems[0]->getType() == OOX::EElementType::et_m_eqArrPr)
			convert(oox_eq_arr->m_arrItems[0]);

		CREATE_MATH_TAG(L"mtable");
		OPEN_MATH_TAG(elm);
		{
			
			for (size_t i = 1; i < oox_eq_arr->m_arrItems.size(); ++i)
			{
				CREATE_MATH_TAG(L"mtr");
				OPEN_MATH_TAG(elm);
				{
					CREATE_MATH_TAG(L"mtd");
					OPEN_MATH_TAG(elm);
					mrow();
					convert(oox_eq_arr->m_arrItems[i]);
					endOfMrow();
					CLOSE_MATH_TAG;
				}
				CLOSE_MATH_TAG;
			}			
		}
		CLOSE_MATH_TAG;
	}

	void OoxConverter::convert(OOX::Logic::CEqArrPr *oox_eq_arr_pr)
	{
		if (!oox_eq_arr_pr) return;

		convert(oox_eq_arr_pr->m_oBaseJc.GetPointer());
		convert(oox_eq_arr_pr->m_oCtrlPr.GetPointer());
		convert(oox_eq_arr_pr->m_oMaxDist.GetPointer());
		convert(oox_eq_arr_pr->m_oObjDist.GetPointer());
		convert(oox_eq_arr_pr->m_oRSp.GetPointer());
		convert(oox_eq_arr_pr->m_oRSpRule.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CBaseJc *oox_base_js)
	{
		if (!oox_base_js) return;
	}
	
	void OoxConverter::convert(OOX::Logic::CMaxDist *oox_max_dist)
	{
		if (!oox_max_dist) return;
	}
	
	void OoxConverter::convert(OOX::Logic::CObjDist *oox_obj_dist) // !!! )
	{
		if (!oox_obj_dist) return;
	}
	
	void OoxConverter::convert(OOX::Logic::CRSp *oox_r_sp)
	{
		if (!oox_r_sp) return;
	}

	void OoxConverter::convert(OOX::Logic::CRSpRule *oox_r_sp_rule)
	{
		if (!oox_r_sp_rule) return;
	}

	void OoxConverter::convert(OOX::Logic::CFraction *oox_fraction)
	{
		if (!oox_fraction) return;		


		std::wstring val = convert(oox_fraction->m_oFPr.GetPointer());

		if (val == L"lin")
		{
			mrow();
				convert(oox_fraction->m_oNum.GetPointer());
			endOfMrow();
			CREATE_MATH_TAG(L"mo");
			OPEN_MATH_TAG(elm);
			elm->add_text(L"/");
			CLOSE_MATH_TAG;
			mrow();
				convert(oox_fraction->m_oDen.GetPointer());
			endOfMrow();
		}
		else if (val == L"noBar")
		{
			CREATE_MATH_TAG(L"mtable");
			OPEN_MATH_TAG(elm);
			{
				CREATE_MATH_TAG(L"mtr");
				OPEN_MATH_TAG(elm);
				{
					CREATE_MATH_TAG(L"mtd");
					OPEN_MATH_TAG(elm);
					mrow();
						convert(oox_fraction->m_oNum.GetPointer());
					endOfMrow();
					CLOSE_MATH_TAG;
				}
				CLOSE_MATH_TAG;
			}
			{
				CREATE_MATH_TAG(L"mtr");
				OPEN_MATH_TAG(elm);
				{
					CREATE_MATH_TAG(L"mtd");
					OPEN_MATH_TAG(elm);
					mrow();
						convert(oox_fraction->m_oDen.GetPointer());
					endOfMrow();
					CLOSE_MATH_TAG;
				}
				CLOSE_MATH_TAG;
			}
			CLOSE_MATH_TAG;
		}
		else
		{
			CREATE_MATH_TAG(L"mfrac");

			typedef odf_writer::math_mfrac* T;			

			T tmp = dynamic_cast<T>(elm.get());
			

			if (tmp)
			{
				if (val == L"skw")
				{
					
					tmp->bevelled = true;
				}
			}

			OPEN_MATH_TAG(elm);
			mrow();
				convert(oox_fraction->m_oNum.GetPointer());
			endOfMrow();
			mrow();
				convert(oox_fraction->m_oDen.GetPointer());
			endOfMrow();
			CLOSE_MATH_TAG;
		}	
	}

	std::wstring OoxConverter::convert(OOX::Logic::CFPr *oox_f_pr)
	{
		if (!oox_f_pr) return L"";
		
		std::wstring result = convert(oox_f_pr->m_oType.GetPointer());
		convert(oox_f_pr->m_oCtrlPr.GetPointer());
		return result;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CType *oox_type)
	{
		if (!oox_type) return L"";

		std::wstring val  = oox_type->m_val->ToString();

		return val;
	}

	void OoxConverter::convert(OOX::Logic::CNum *oox_num)
	{
		if (!oox_num) return;

		for (size_t i = 0; i < oox_num->m_arrItems.size(); ++i)
		{
			convert(oox_num->m_arrItems[i]);
		}
	}

	void OoxConverter::convert(OOX::Logic::CDen *oox_den)
	{
		if (!oox_den) return;	

		for (size_t i = 0; i < oox_den->m_arrItems.size(); ++i)
		{
			convert(oox_den->m_arrItems[i]);
		}
	}

	void OoxConverter::convert(OOX::Logic::CFunc *oox_func)
	{
		if (!oox_func) return;

		convert(oox_func->m_oFuncPr.GetPointer());
		convert(oox_func->m_oFName.GetPointer());

		
		convert(oox_func->m_oElement.GetPointer());	
	}

	void OoxConverter::convert(OOX::Logic::CFuncPr *oox_func_pr)
	{
		if (!oox_func_pr) return;

		convert(oox_func_pr->m_oCtrlPr.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CFName *oox_fname)
	{
		if (!oox_fname) return;

		for (size_t i = 0; i < oox_fname->m_arrItems.size(); ++i)
			convert(oox_fname->m_arrItems[i]);

	}

	void OoxConverter::convert(OOX::Logic::CGroupChr *oox_group_ch)
	{
		if (!oox_group_ch) return;		

		bool flag = convert(oox_group_ch->m_oGroupChrPr.GetPointer());
		std::wstring tag;
		if (flag) tag = L"mover";
		else tag = L"munder";
		
		
		CREATE_MATH_TAG(tag.c_str());
		OPEN_MATH_TAG(elm);
		convert(oox_group_ch->m_oElement.GetPointer());
		if(flag)
			convert(oox_group_ch->m_oGroupChrPr->m_oChr.GetPointer());
		else
		{
			if (!oox_group_ch->m_oGroupChrPr->m_oChr.IsInit())
			{
				CREATE_MATH_TAG(L"mo");
				elm->add_text(L" ⏟ ");
				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}				
			else
				convert(oox_group_ch->m_oGroupChrPr->m_oChr.GetPointer());
			
		}
		CLOSE_MATH_TAG;	
	}

	bool OoxConverter::convert(OOX::Logic::CGroupChrPr	*oox_group_ch_pr)
	{
		if (!oox_group_ch_pr) return false;	
		
		bool flag = false;

		//convert(oox_group_ch_pr->m_oChr.GetPointer());
		convert(oox_group_ch_pr->m_oCtrlPr.GetPointer());
		flag = convert(oox_group_ch_pr->m_oPos.GetPointer());
		convert(oox_group_ch_pr->m_oVertJc.GetPointer());	

		return flag;
	}

	bool OoxConverter::convert(OOX::Logic::CPos *oox_pos)
	{
		if (!oox_pos) return false;

		if (oox_pos->m_val->ToString() == L"top") return true;
		else return false;
	}

	void OoxConverter::convert(OOX::Logic::CVertJc *oox_vert_jc)
	{
		if (!oox_vert_jc) return;
	}

	void OoxConverter::convert(OOX::Logic::CLimLow *oox_lim_low)
	{
		if (!oox_lim_low) return;

		mrow();

			
		CREATE_MATH_TAG(L"munder");
		OPEN_MATH_TAG(elm);

		mrow();
			convert(oox_lim_low->m_oElement.GetPointer());
		endOfMrow();
		convert(oox_lim_low->m_oLimLowPr.GetPointer());
		convert(oox_lim_low->m_oLim.GetPointer());

			CLOSE_MATH_TAG

		endOfMrow();
	}

	void OoxConverter::convert(OOX::Logic::CLim *oox_lim)
	{
		if (!oox_lim) return;

		mrow();

		for (size_t i = 0; i < oox_lim->m_arrItems.size(); ++i)
			convert(oox_lim->m_arrItems[i]);

		endOfMrow();
	}

	void OoxConverter::convert(OOX::Logic::CLimLowPr *oox_lim_low_pr)
	{
		if (!oox_lim_low_pr) return;

		convert(oox_lim_low_pr->m_oCtrlPr.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CLimUpp *oox_lim_upp)
	{
		if (!oox_lim_upp) return;

		
		CREATE_MATH_TAG(L"mover");
		OPEN_MATH_TAG(elm);

		mrow();
			convert(oox_lim_upp->m_oElement.GetPointer());
		endOfMrow();
		convert(oox_lim_upp->m_oLimUppPr.GetPointer());
		convert(oox_lim_upp->m_oLim.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CLimUppPr *oox_lim_upp_pr)
	{
		if (!oox_lim_upp_pr) return;

		convert(oox_lim_upp_pr->m_oCtrlPr.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CMathFont *oox_math_font)
	{
		if (!oox_math_font) return;		
	}

	void OoxConverter::convert(OOX::Logic::CMatrix *oox_matrix)
	{
		if (!oox_matrix) return;

		CREATE_MATH_TAG(L"mtable");
		OPEN_MATH_TAG(elm);

		for (size_t i = 0; i < oox_matrix->m_arrItems.size(); ++i)
		{
			convert(oox_matrix->m_arrItems[i]);
		}
		CLOSE_MATH_TAG;

	}

	void OoxConverter::convert(OOX::Logic::CMc	*oox_mc)
	{
		if (!oox_mc) return;

		convert(oox_mc->m_oMcPr.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CMcPr *oox_mc_pr)
	{
		if (!oox_mc_pr) return;
		
		convert(oox_mc_pr->m_oCount.GetPointer());
		convert(oox_mc_pr->m_oMcJc.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CCount *oox_count)
	{
		if (!oox_count) return;
	}

	void OoxConverter::convert(OOX::Logic::CMcJc *oox_mc_jc)
	{
		if (!oox_mc_jc) return;
	}

	void OoxConverter::convert(OOX::Logic::CMcs *oox_mcs)
	{
		if (!oox_mcs) return;

		for (size_t i = 0; i < oox_mcs->m_arrItems.size(); ++i)
		{
			convert(oox_mcs->m_arrItems[i]);
		}
	}

	void OoxConverter::convert(OOX::Logic::CMDel *oox_m_del)
	{
		if (!oox_m_del) return;

		
		CREATE_MATH_TAG(L"mathMDel")
		OPEN_MATH_TAG(elm)

		convert(oox_m_del->m_oRun.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CMIns *oox_m_ins)
	{
		if (!oox_m_ins) return;
		
		CREATE_MATH_TAG(L"mathMIns")
		OPEN_MATH_TAG(elm)
		
		convert(oox_m_ins->m_oRun.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CMPr *oox_m_pr)
	{
		if (!oox_m_pr) return;		

		convert(oox_m_pr->m_oBaseJc.GetPointer());
		convert(oox_m_pr->m_oCGp.GetPointer());
		convert(oox_m_pr->m_oCGpRule.GetPointer());
		convert(oox_m_pr->m_oCSp.GetPointer());
		convert(oox_m_pr->m_oCtrlPr.GetPointer());
		convert(oox_m_pr->m_oMcs.GetPointer());
		convert(oox_m_pr->m_oMcs.GetPointer());
		convert(oox_m_pr->m_oPlcHide.GetPointer());
		convert(oox_m_pr->m_oRSp.GetPointer());
		convert(oox_m_pr->m_oRSpRule.GetPointer());		
	}

	void OoxConverter::convert(OOX::Logic::CPlcHide *oox_plc_hide)
	{
		if (!oox_plc_hide) return;
	}

	void OoxConverter::convert(OOX::Logic::CMr	*oox_mr)
	{
		if (!oox_mr) return;

		CREATE_MATH_TAG(L"mtr");
		OPEN_MATH_TAG(elm);

		for (size_t i = 0; i < oox_mr->m_arrItems.size(); ++i)
		{
			CREATE_MATH_TAG(L"mtd");
			OPEN_MATH_TAG(elm);
			convert(oox_mr->m_arrItems[i]);
			CLOSE_MATH_TAG;
		}

		CLOSE_MATH_TAG;
	}

	void OoxConverter::convert(OOX::Logic::CMRun *oox_mrun)
	{
		if (!oox_mrun) return;	

		convert(oox_mrun->m_oAnnotationRef.GetPointer());
		convert(oox_mrun->m_oARPr.GetPointer());
		convert(oox_mrun->m_oBr.GetPointer());
		convert(oox_mrun->m_oCommentReference.GetPointer());
		convert(oox_mrun->m_oContentPart.GetPointer());
		convert(oox_mrun->m_oContinuationSeparator.GetPointer());
		convert(oox_mrun->m_oCr.GetPointer());
		convert(oox_mrun->m_oDayLong.GetPointer());
		convert(oox_mrun->m_oDayShort.GetPointer());
		convert(oox_mrun->m_oDel.GetPointer());
		convert(oox_mrun->m_oDelInstrText.GetPointer());
		convert(oox_mrun->m_oDelText.GetPointer());
		convert(oox_mrun->m_oDrawing.GetPointer());
		convert(oox_mrun->m_oEndnoteRef.GetPointer());
		convert(oox_mrun->m_oEndnoteReference.GetPointer());
		convert(oox_mrun->m_oEndnoteReference.GetPointer());
		convert(oox_mrun->m_oFldChar.GetPointer());
		convert(oox_mrun->m_oFootnoteRef.GetPointer());
		convert(oox_mrun->m_oFootnoteReference.GetPointer());
		convert(oox_mrun->m_oIns.GetPointer());
		convert(oox_mrun->m_oInstrText.GetPointer());
		convert(oox_mrun->m_oLastRenderedPageBreak.GetPointer());
		convert(oox_mrun->m_oMonthLong.GetPointer());
		convert(oox_mrun->m_oMonthShort.GetPointer());
		//convert(oox_mrun->m_oMRPr.GetPointer());
		bool clrFlag = convert(oox_mrun->m_oRPr.GetPointer());
		convert(oox_mrun->m_oMText.GetPointer());
		convert(oox_mrun->m_oNoBreakHyphen.GetPointer());
		convert(oox_mrun->m_oObject.GetPointer());
		convert(oox_mrun->m_oPgNum.GetPointer());
		convert(oox_mrun->m_oPtab.GetPointer());
		convert(oox_mrun->m_oRuby.GetPointer());
		convert(oox_mrun->m_oSeparator.GetPointer());
		convert(oox_mrun->m_oSoftHyphen.GetPointer());
		convert(oox_mrun->m_oSym.GetPointer());
		convert(oox_mrun->m_oTab.GetPointer());
		convert(oox_mrun->m_oText.GetPointer());
		convert(oox_mrun->m_oYearLong.GetPointer());
		convert(oox_mrun->m_oYearShort.GetPointer());
		if (clrFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	bool OoxConverter::convert(OOX::Logic::CRunProperty *oox_r_pr)
	{
		if (!oox_r_pr) return false;

		//if (oox_r_pr->m_oColor.IsInit())
		//{
		//	if (oox_r_pr->m_oColor->m_oVal.IsInit())
		//	{
		//		std::wstring clr = oox_r_pr->m_oColor->m_oVal.GetPointer()->ToString();
		//		std::wstring clr2(L"#");
		//		clr.erase(0, 2);
		//		clr2 += clr;
		//		CREATE_MATH_TAG(L"mstyle");
		//		typedef odf_writer::math_mstyle * T;

		//		T tmp = dynamic_cast<T>(elm.get());
		//		tmp->color_ = clr2;
		//		OPEN_MATH_TAG(elm);
		//		return true;
		//	}
		//}
		return false;
	}

	void OoxConverter::convert(OOX::Logic::CMText *oox_text)
	{
		if (!oox_text) return;

		std::set<wchar_t>& mo = odf_context()->math_context()->mo;		

		std::wstring s_val = oox_text->m_sText;
		std::wstring sub_s_val = L"";
		
		for (size_t i = 0; i < s_val.size(); i++)
		{
			wchar_t w_val = s_val[i];			

			if (w_val == L'=')
			{
				if (sub_s_val.size() != 0)
				{
					CREATE_MATH_TAG(L"mi");
					elm->add_text(sub_s_val);
					OPEN_MATH_TAG(elm);
					CLOSE_MATH_TAG;
					sub_s_val.clear();
				}
				CREATE_MATH_TAG(L"mtext");

				elm->add_text(std::wstring(1, s_val[i]));

				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}

			else if (w_val <= 57 && w_val >= 48)
			{
				if (sub_s_val.size() != 0)
				{
					CREATE_MATH_TAG(L"mi");
					elm->add_text(sub_s_val);
					OPEN_MATH_TAG(elm);
					CLOSE_MATH_TAG;
					sub_s_val.clear();
				}
				
				CREATE_MATH_TAG(L"mn");

				elm->add_text(std::wstring(1, s_val[i]));

				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}
			else if (mo.find(w_val) != mo.end())
			{
				if (sub_s_val.size() != 0)
				{
					CREATE_MATH_TAG(L"mi");
					elm->add_text(sub_s_val);
					OPEN_MATH_TAG(elm);
					CLOSE_MATH_TAG;
					sub_s_val.clear();
				}
				
				CREATE_MATH_TAG(L"mo");

				elm->add_text(std::wstring(1, s_val[i]));

				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}
			else // <mi>
			{				
				sub_s_val += s_val[i];
			}		

			if ((i == s_val.size() - 1) && (sub_s_val.size() != 0))
			{				
				CREATE_MATH_TAG(L"mi");
				elm->add_text(sub_s_val);
				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}
		}	
	}

	void OoxConverter::convert(OOX::Logic::CNary *oox_nary)
	{
		if (!oox_nary) return;
		nullable<SimpleTypes::CHexColor<>>* ref = NULL;
		
		if (oox_nary->m_oNaryPr->m_oCtrlPr.IsInit() &&
			oox_nary->m_oNaryPr->m_oCtrlPr->m_oRPr.IsInit() &&
			oox_nary->m_oNaryPr->m_oCtrlPr->m_oRPr->m_oColor.IsInit())
		{
			ref = &(oox_nary->m_oNaryPr->m_oCtrlPr->m_oRPr->m_oColor->m_oVal);
		}
		bool flag_color = false;
		if (ref != NULL)
		{
			/*std::wstring clr = ref->GetPointer()->ToString();
			std::wstring clr2(L"#");
			clr.erase(0, 2);
			clr2 += clr;
			CREATE_MATH_TAG(L"mstyle");
			typedef odf_writer::math_mstyle* T;

			T tmp = dynamic_cast<T>(elm.get());
			tmp->color_ = clr2;
			OPEN_MATH_TAG(elm);	
			flag_color = true;*/
		}

		mrow();

		bool flag_nary = false; // TODO REFAC
		if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() != 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() != 0))
		{
			CREATE_MATH_TAG(L"munderover");
			OPEN_MATH_TAG(elm);
			flag_nary = true;
		}
		else if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() != 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() == 0))
		{
			CREATE_MATH_TAG(L"munder");
			OPEN_MATH_TAG(elm);
			flag_nary = true;
		}

		else if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() == 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() != 0))
		{
			CREATE_MATH_TAG(L"mover");
			OPEN_MATH_TAG(elm);
			flag_nary = true;
		}		
		
		std::pair<bool,bool>	flags;
		flags = convert(oox_nary->m_oNaryPr.GetPointer());

		
		if (!flags.first)
		{
			//mrow();
			convert(oox_nary->m_oSub.GetPointer());
			//endOfMrow();
		}
		if (!flags.second)
		{
			//mrow();
			convert(oox_nary->m_oSup.GetPointer());
			//endOfMrow();
		}


		if (flag_nary)
		{
			CLOSE_MATH_TAG;
		}		

		if (flag_color)
		{
			CLOSE_MATH_TAG;
		}
		convert(oox_nary->m_oElement.GetPointer());

		endOfMrow();
	}

	std::pair<bool, bool> OoxConverter::convert(OOX::Logic::CNaryPr *oox_nary_pr)
	{
		std::pair<bool, bool> result = { false, false};
		if (!oox_nary_pr) return result;

		convert(oox_nary_pr->m_oCtrlPr.GetPointer());
		convert(oox_nary_pr->m_oChr.GetPointer());		
		convert(oox_nary_pr->m_oGrow.GetPointer());		
		result.first = convert(oox_nary_pr->m_oSubHide.GetPointer());
		result.second = convert(oox_nary_pr->m_oSupHide.GetPointer());

		return result;		
	}

	void OoxConverter::convert(OOX::Logic::CGrow* oox_grow)
	{
		if (!oox_grow) return;
	}

	bool OoxConverter::convert(OOX::Logic::CSubHide *oox_subHide)
	{
		if (!oox_subHide) return false;

		bool result = oox_subHide->m_val->ToBool();
		return result;
	}

	bool OoxConverter::convert(OOX::Logic::CSupHide *oox_supHide)
	{
		if (!oox_supHide) return false;
		bool result = oox_supHide->m_val->ToBool();
		return result;
	}

	void OoxConverter::convert(OOX::Logic::CChr * oox_chr)
	{		
		//if (!oox_chr) return;
		CREATE_MATH_TAG(L"mo");
		typedef odf_writer::math_mo* T;

		T tmp = dynamic_cast<T>(elm.get());
		if (tmp)
		{
			tmp->stretchy_ = false;
		}
		
		if (!oox_chr)		
			elm->add_text(L"∫");		
		else
		{
			/*std::wstring ws = L"&#";
			std::wstringstream ss;			
			ss << (oox_chr->m_val->GetValue()[0] & 0xFFFF);
			ws += ss.str();
			elm->add_text(ws);*/			
			elm->add_text(oox_chr->m_val->GetValue());
		}
		OPEN_MATH_TAG(elm);
		CLOSE_MATH_TAG;
	}

	void OoxConverter::convert(OOX::Logic::CPhant *oox_phant)
	{
		if (!oox_phant) return;
		
		CREATE_MATH_TAG(L"mathPhant")
		OPEN_MATH_TAG(elm)

		convert(oox_phant->m_oElement.GetPointer());
		convert(oox_phant->m_oPhantPr.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CPhantPr *oox_phant_pr)
	{
		if (!oox_phant_pr) return;
		
		CREATE_MATH_TAG(L"mathPhantPr")
		OPEN_MATH_TAG(elm)

		convert(oox_phant_pr->m_oCtrlPr.GetPointer());
		convert(oox_phant_pr->m_oShow.GetPointer());
		convert(oox_phant_pr->m_oTransp.GetPointer());
		convert(oox_phant_pr->m_oZeroAsc.GetPointer());
		convert(oox_phant_pr->m_oZeroDesc.GetPointer());
		convert(oox_phant_pr->m_oZeroWid.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CRad *oox_rad)
	{
		if (!oox_rad) return;

		nullable<ComplexTypes::Word::CColor> p;
		
		if(oox_rad->m_oRadPr.IsInit() && oox_rad->m_oRadPr->m_oCtrlPr.IsInit() && oox_rad->m_oRadPr->m_oCtrlPr->m_oRPr.IsInit())
			p = oox_rad->m_oRadPr->m_oCtrlPr->m_oRPr->m_oColor;
		if (p.IsInit())
		{
			/*std::wstring clr = p->m_oVal.GetPointer()->ToString();
			std::wstring clr2(L"#");
			clr.erase(0, 2);
			clr2 += clr;
			CREATE_MATH_TAG(L"mstyle");
			typedef odf_writer::math_mstyle * T;

			T tmp = dynamic_cast<T>(elm.get());
			tmp->color_ = clr2;
			OPEN_MATH_TAG(elm);*/
		}

		{
			bool flag = convert(oox_rad->m_oRadPr.GetPointer());
			if (flag)
			{
				CREATE_MATH_TAG(L"msqrt");
				OPEN_MATH_TAG(elm);
				mrow();
					convert(oox_rad->m_oElement.GetPointer());
				endOfMrow();
				CLOSE_MATH_TAG;
			}
			else
				convert(oox_rad->m_oDeg.GetPointer(), oox_rad->m_oElement.GetPointer());
		}

		if (p.IsInit())
		{
			//CLOSE_MATH_TAG;
		}

	}

	bool OoxConverter::convert(OOX::Logic::CRadPr *oox_rad_pr)
	{
		if (!oox_rad_pr) return false;

		bool flag = convert(oox_rad_pr->m_oDegHide.GetPointer());
		convert(oox_rad_pr->m_oCtrlPr.GetPointer());
		return flag;
	}

	bool OoxConverter::convert(OOX::Logic::CDegHide *oox_deg_hide)
	{
		if (!oox_deg_hide) return false;	
		
		return oox_deg_hide->m_val->ToBool();
	}

	void OoxConverter::convert(OOX::Logic::CDeg *oox_deg, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_deg) return;
		if (oox_deg->m_arrItems.size() == 0) return;

		
		CREATE_MATH_TAG(L"mroot")
		OPEN_MATH_TAG(elm)		
		convert(oox_elm);

		mrow();

			for (size_t i = 0; i < oox_deg->m_arrItems.size(); ++i)
				convert(oox_deg->m_arrItems[i]);

		endOfMrow();

		
		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CSPre *oox_s_pre)
	{
		if (!oox_s_pre) return;

		convert(oox_s_pre->m_oSPrePr.GetPointer());
		
		CREATE_MATH_TAG(L"mmultiscripts")
		OPEN_MATH_TAG(elm)

		convert(oox_s_pre->m_oElement.GetPointer());
		{
			CREATE_MATH_TAG(L"mprescripts")
			OPEN_MATH_TAG(elm)
			CLOSE_MATH_TAG
			convert(oox_s_pre->m_oSub.GetPointer());
			convert(oox_s_pre->m_oSup.GetPointer());
		}

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CSPrePr *oox_s_pre_pr)
	{
		if (!oox_s_pre_pr) return;
	
		convert(oox_s_pre_pr->m_oCtrlPr.GetPointer());
		
	}

	void OoxConverter::convert(OOX::Logic::CSup *oox_sup, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_sup) return;
		
		CREATE_MATH_TAG(L"msup");
		OPEN_MATH_TAG(elm);		
		
		mrow();
		convert(oox_elm);	
		endOfMrow();

		mrow();
		for (size_t i = 0; i < oox_sup->m_arrItems.size(); ++i)
		{
			convert(oox_sup->m_arrItems[i]);
		}
		endOfMrow();

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CSub *oox_sub, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_sub) return;
		
		CREATE_MATH_TAG(L"msub")
		OPEN_MATH_TAG(elm)	

		convert(oox_elm);

		for (size_t i = 0; i < oox_sub->m_arrItems.size(); ++i)
		{
			convert(oox_sub->m_arrItems[i]);

		}

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CSub *oox_csub)
	{
		mrow();
		for (size_t i = 0; i < oox_csub->m_arrItems.size(); ++i)
		{
			convert(oox_csub->m_arrItems[i]);
		}
		endOfMrow();
	}

	void OoxConverter::convert(OOX::Logic::CSup *oox_csup)
	{
		mrow();
		for (size_t i = 0; i < oox_csup->m_arrItems.size(); ++i)
		{
			convert(oox_csup->m_arrItems[i]);
		}	
		endOfMrow();
	}

	void OoxConverter::convert(OOX::Logic::CSSub *oox_ssub)
	{
		if (!oox_ssub) return;		

		convert(oox_ssub->m_oSSubPr.GetPointer());
		convert(oox_ssub->m_oSub.GetPointer(), oox_ssub->m_oElement.GetPointer());		
	}

	void OoxConverter::convert(OOX::Logic::CSSubPr*oox_ssub_pr)
	{
		if (!oox_ssub_pr) return;		

		convert(oox_ssub_pr->m_oCtrlPr.GetPointer());
		
	}

	void OoxConverter::convert(OOX::Logic::CSSubSup *oox_ssub_sup)
	{
		if (!oox_ssub_sup) return;

		convert(oox_ssub_sup->m_oSSubSupPr.GetPointer());
		
		CREATE_MATH_TAG(L"msubsup")
		OPEN_MATH_TAG(elm)

		convert(oox_ssub_sup->m_oElement.GetPointer());
		convert(oox_ssub_sup->m_oSub.GetPointer());
		convert(oox_ssub_sup->m_oSup.GetPointer());

		CLOSE_MATH_TAG
	}

	void OoxConverter::convert(OOX::Logic::CSSubSupPr *oox_ssub_sup_pr)
	{
		if (!oox_ssub_sup_pr) return;


		convert(oox_ssub_sup_pr->m_oAlnScr.GetPointer());
		convert(oox_ssub_sup_pr->m_oCtrlPr.GetPointer());

	}

	void OoxConverter::convert(OOX::Logic::CAlnScr* oox_aln_scr)
	{
		if (!oox_aln_scr) return;
	}

	void OoxConverter::convert(OOX::Logic::CSSup *oox_ssup)
	{
		if (!oox_ssup) return;	
		
		convert(oox_ssup->m_oSSupPr.GetPointer());

		convert(oox_ssup->m_oSup.GetPointer(), oox_ssup->m_oElement.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CSSupPr *oox_ssup_pr)
	{
		if (!oox_ssup_pr) return;

		convert(oox_ssup_pr->m_oCtrlPr.GetPointer());
	}

	void OoxConverter::convert(OOX::Logic::CElement *oox_elm)
	{
		if (!oox_elm) return;

		resizeBrackets();

		if (!brackets()[lvl_of_me()].empty())
		{
			for (size_t i = 0; i < brackets()[lvl_of_me()].size() / 2; ++i)
			{
				
				CREATE_MATH_TAG(L"mo")
				elm->add_text(brackets()[lvl_of_me()][i]);
				OPEN_MATH_TAG(elm)
				CLOSE_MATH_TAG				
			}
		}

		for (size_t i = 0; i < oox_elm->m_arrItems.size(); ++i)
		{
			convert(oox_elm->m_arrItems[i]);
		}

		if (!brackets()[lvl_of_me()].empty())
		{
			for (size_t i = brackets()[lvl_of_me()].size() / 2; i < brackets()[lvl_of_me()].size(); ++i)
			{				
				CREATE_MATH_TAG(L"mo")
				elm->add_text(brackets()[lvl_of_me()][i]);
				OPEN_MATH_TAG(elm)
				CLOSE_MATH_TAG
			}
		}
		brackets()[lvl_of_me()].clear();

		if (lvl_of_me() != 0)
		{
			lvl_of_me()--;
			brackets().pop_back();
		}

		if (!end_counter().empty())
		{
			for (size_t i = 0; i < end_counter()[end_counter().size() - 1]; ++i)
				CLOSE_MATH_TAG

			end_counter().pop_back();
		}
	}
}
