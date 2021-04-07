#include "DocxConverter.h"

//#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/DocxFlat.h"

namespace Oox2Odf
{
	void DocxConverter::convert(OOX::Logic::COMath *oox_math)
	{
		if (!oox_math) return;

		for (size_t i = 0; i < oox_math->m_arrItems.size(); ++i)
		{
			convert(oox_math->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CMathPr *oox_math_pr)
	{
		if (!oox_math_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::COMathPara *oox_math_para)
	{
		if (!oox_math_para) return;

		for (size_t i = 0; i < oox_math_para->m_arrItems.size(); ++i)
		{
			convert(oox_math_para->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::COMathParaPr *oox_math_para_pr)
	{
		if (!oox_math_para_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CCtrlPr *oox_ctrl_pr)
	{
		if (!oox_ctrl_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CAcc *oox_acc)
	{
		if (!oox_acc) return;
	}

	void DocxConverter::convert(OOX::Logic::CAccPr	*oox_acc_pr)
	{
		if (!oox_acc_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CArgPr	*oox_arg_pr)
	{
		if (!oox_arg_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CBar	*oox_bar)
	{
		if (!oox_bar) return;
	}

	void DocxConverter::convert(OOX::Logic::CBarPr	*oox_bar_pr)
	{
		if (!oox_bar_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CBorderBox *oox_border_box)
	{
		if (!oox_border_box) return;
	}

	void DocxConverter::convert(OOX::Logic::CBorderBoxPr *oox_border_box_pr)
	{
		if (!oox_border_box_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CBox *oox_box)
	{
		if (!oox_box) return;
	}

	void DocxConverter::convert(OOX::Logic::CBoxPr *oox_box_pr)
	{
		if (!oox_box_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CBrk *oox_brk)
	{
		if (!oox_brk) return;
	}

	void DocxConverter::convert(OOX::Logic::CDelimiter *oox_del)
	{
		if (!oox_del) return;
	}

	void DocxConverter::convert(OOX::Logic::CDelimiterPr *oox_del_pr)
	{
		if (!oox_del_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CEqArr *oox_eq_arr)
	{
		if (!oox_eq_arr) return;
	}

	void DocxConverter::convert(OOX::Logic::CEqArrPr *oox_eq_arr_pr)
	{
		if (!oox_eq_arr_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CFraction *oox_fraction)
	{
		if (!oox_fraction) return;
	}
	
	void DocxConverter::convert(OOX::Logic::CFPr *oox_f_pr)
	{
		if (!oox_f_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CFunc *oox_func)
	{
		if (!oox_func) return;
	}

	void DocxConverter::convert(OOX::Logic::CFuncPr *oox_func_pr)
	{
		if (!oox_func_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CGroupChr *oox_group_ch)
	{
		if (!oox_group_ch) return;
	}

	void DocxConverter::convert(OOX::Logic::CGroupChrPr	*oox_group_ch_pr)
	{
		if (!oox_group_ch_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CLimLow *oox_lim_low)
	{
		if (!oox_lim_low) return;
	}

	void DocxConverter::convert(OOX::Logic::CLimLowPr *oox_lim_low_pr)
	{
		if (!oox_lim_low_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CLimUpp *oox_lim_upp)
	{
		if (!oox_lim_upp) return;
	}

	void DocxConverter::convert(OOX::Logic::CLimUppPr *oox_lim_upp_pr)
	{
		if (!oox_lim_upp_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CMathFont *oox_math_font)
	{
		if (!oox_math_font) return;
	}

	void DocxConverter::convert(OOX::Logic::CMatrix *oox_matrix)
	{
		if (!oox_matrix) return;
	}

	void DocxConverter::convert(OOX::Logic::CMc	*oox_mc)
	{
		if (!oox_mc) return;
	}

	void DocxConverter::convert(OOX::Logic::CMcPr *oox_mc_pr)
	{
		if (!oox_mc_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CMcs *oox_mcs)
	{
		if (!oox_mcs) return;
	}

	void DocxConverter::convert(OOX::Logic::CMDel *oox_m_del)
	{
		if (!oox_m_del) return;
	}

	void DocxConverter::convert(OOX::Logic::CMIns *oox_m_ins)
	{
		if (!oox_m_ins) return;
	}

	void DocxConverter::convert(OOX::Logic::CMPr *oox_m_pr)
	{
		if (!oox_m_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CMr	*oox_mr)
	{
		if (!oox_mr) return;
	}

	void DocxConverter::convert(OOX::Logic::CMRun *oox_mrun)
	{
		if (!oox_mrun) return;
	}

	void DocxConverter::convert(OOX::Logic::CMText *oox_text)
	{
		if (!oox_text) return;
	}

	void DocxConverter::convert(OOX::Logic::CNary *oox_nary)
	{
		if (!oox_nary) return;
	}
	
	void DocxConverter::convert(OOX::Logic::CNaryPr *oox_nary_pr)
	{
		if (!oox_nary_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CPhant *oox_phant)
	{
		if (!oox_phant) return;
	}

	void DocxConverter::convert(OOX::Logic::CPhantPr *oox_phant_pr)
	{
		if (!oox_phant_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CRad *oox_rad)
	{
		if (!oox_rad) return;
	}

	void DocxConverter::convert(OOX::Logic::CRadPr *oox_rad_pr)
	{
		if (!oox_rad_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CSPre *oox_s_pre)
	{
		if (!oox_s_pre) return;
	}

	void DocxConverter::convert(OOX::Logic::CSPrePr *oox_s_pre_pr)
	{
		if (!oox_s_pre_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSub *oox_ssub)
	{
		if (!oox_ssub) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSubPr*oox_ssub_pr)
	{
		if (!oox_ssub_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSubSup *oox_ssub_sup)
	{
		if (!oox_ssub_sup) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSubSupPr *oox_ssub_sup_pr)
	{
		if (!oox_ssub_sup_pr) return;
	}

	void DocxConverter::convert(OOX::Logic::CSSup *oox_ssup)
	{
		if (!oox_ssup) return;
		//oox_ssup->m_oElement
	}

	void DocxConverter::convert(OOX::Logic::CSSupPr*oox_ssup_pr)
	{
		if (!oox_ssup_pr) return;
	}
}