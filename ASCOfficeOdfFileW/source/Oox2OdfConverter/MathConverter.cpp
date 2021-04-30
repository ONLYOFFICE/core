#include "DocxConverter.h"

//#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/DocxFlat.h"

#include "../OdfFormat/odf_conversion_context.h"

namespace Oox2Odf
{
	void DocxConverter::convert(OOX::Logic::COMath *oox_math)
	{
		if (!oox_math) return;

		bool bStart = odf_context()->start_math(); //למזוע בûעü מעהוכüםמ מע COMathPara 

		for (size_t i = 0; i < oox_math->m_arrItems.size(); ++i)
		{
			convert(oox_math->m_arrItems[i]);
		}
		
		if (bStart) odf_context()->end_math();
	}

	void DocxConverter::convert(OOX::Logic::CMathPr *oox_math_pr)
	{
		if (!oox_math_pr) return;

		for (size_t i = 0; i < oox_math_pr->m_arrItems.size(); ++i)
		{
			convert(oox_math_pr->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::COMathPara *oox_math_para)
	{
		if (!oox_math_para) return;

		odf_context()->start_math();

		for (size_t i = 0; i < oox_math_para->m_arrItems.size(); ++i)
		{
			convert(oox_math_para->m_arrItems[i]);
		}
		odf_context()->end_math();
	}

	void DocxConverter::convert(OOX::Logic::COMathParaPr *oox_math_para_pr)
	{
		if (!oox_math_para_pr) return;
		
		/*odf_writer::office_element_ptr elm;

		odf_writer::create_element(L"math", L"mathParaPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_math_para_pr->m_oMJc.GetPointer());

		odf_context()->math_context()->end_element();*/
	}

	void DocxConverter::convert(OOX::Logic::CCtrlPr *oox_ctrl_pr)
	{
		if (!oox_ctrl_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathCtrlPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ctrl_pr->m_oARPr.GetPointer());
		convert(oox_ctrl_pr->m_oDel.GetPointer());
		convert(oox_ctrl_pr->m_oIns.GetPointer());
		convert(oox_ctrl_pr->m_oRPr.GetPointer());

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CAcc *oox_acc)
	{
		if (!oox_acc) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathAcc", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_acc->m_oAccPr.GetPointer());
		convert(oox_acc->m_oElement.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CAccPr	*oox_acc_pr)
	{
		if (!oox_acc_pr) return;
		
		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathAccPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_acc_pr->m_oChr.GetPointer());
		convert(oox_acc_pr->m_oCtrlPr.GetPointer());		

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CArgPr	*oox_arg_pr)
	{
		if (!oox_arg_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathArgPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_arg_pr->m_oArgSz.GetPointer());		

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBar	*oox_bar)
	{
		if (!oox_bar) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBar", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_bar->m_oBarPr.GetPointer());
		convert(oox_bar->m_oElement.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBarPr	*oox_bar_pr)
	{
		if (!oox_bar_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBarPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_bar_pr->m_oCtrlPr.GetPointer());
		convert(oox_bar_pr->m_oPos.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBorderBox *oox_border_box)
	{
		if (!oox_border_box) return;
		
		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBorderBox", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_border_box->m_oBorderBoxPr.GetPointer());
		convert(oox_border_box->m_oElement.GetPointer());
		

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBorderBoxPr *oox_border_box_pr)
	{
		if (!oox_border_box_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBorderBoxPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_border_box_pr->m_oCtrlPr.GetPointer());
		convert(oox_border_box_pr->m_oHideBot.GetPointer());
		convert(oox_border_box_pr->m_oHideLeft.GetPointer());
		convert(oox_border_box_pr->m_oHideRight.GetPointer());
		convert(oox_border_box_pr->m_oHideTop.GetPointer());
		convert(oox_border_box_pr->m_oStrikeBLTR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeH.GetPointer());
		convert(oox_border_box_pr->m_oStrikeTLBR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeV.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBox *oox_box)
	{
		if (!oox_box) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBox", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_box->m_oBoxPr.GetPointer());
		convert(oox_box->m_oElement.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBoxPr *oox_box_pr)
	{
		if (!oox_box_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBoxPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_box_pr->m_oAln.GetPointer());
		convert(oox_box_pr->m_oBrk.GetPointer());
		convert(oox_box_pr->m_oCtrlPr.GetPointer());
		convert(oox_box_pr->m_oDiff.GetPointer());
		convert(oox_box_pr->m_oNoBreak.GetPointer());
		convert(oox_box_pr->m_oOpEmu.GetPointer());		

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CBrk *oox_brk)
	{
		if (!oox_brk) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathBrk", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		//convert(oox_brk->m_alnAt.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CDelimiter *oox_del)
	{
		if (!oox_del) return;

		for (size_t i = 0; i < oox_del->m_arrItems.size(); ++i)
		{
			convert(oox_del->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CDelimiterPr *oox_del_pr)
	{
		if (!oox_del_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathDelPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_del_pr->m_oBegChr.GetPointer());
		convert(oox_del_pr->m_oCtrlPr.GetPointer());
		convert(oox_del_pr->m_oEndChr.GetPointer());
		convert(oox_del_pr->m_oGrow.GetPointer());
		convert(oox_del_pr->m_oSepChr.GetPointer());
		convert(oox_del_pr->m_oShp.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CEqArr *oox_eq_arr)
	{
		if (!oox_eq_arr) return;

		for (size_t i = 0; i < oox_eq_arr->m_arrItems.size(); ++i)
		{
			convert(oox_eq_arr->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CEqArrPr *oox_eq_arr_pr)
	{
		if (!oox_eq_arr_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathEqArrPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_eq_arr_pr->m_oBaseJc.GetPointer());
		convert(oox_eq_arr_pr->m_oCtrlPr.GetPointer());
		convert(oox_eq_arr_pr->m_oMaxDist.GetPointer());
		convert(oox_eq_arr_pr->m_oObjDist.GetPointer());
		convert(oox_eq_arr_pr->m_oRSp.GetPointer());
		convert(oox_eq_arr_pr->m_oRSpRule.GetPointer());

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CFraction *oox_fraction)
	{
		if (!oox_fraction) return;
		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathFr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_fraction->m_oDen.GetPointer());
		convert(oox_fraction->m_oNum .GetPointer());
		// TODO m_oFr

		odf_context()->math_context()->end_element();		
	}
	
	void DocxConverter::convert(OOX::Logic::CFPr *oox_f_pr)
	{
		if (!oox_f_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathFPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_f_pr->m_oCtrlPr.GetPointer());
		convert(oox_f_pr->m_oType.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CFunc *oox_func)
	{
		if (!oox_func) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathFunc", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_func->m_oElement.GetPointer());
		convert(oox_func->m_oFName.GetPointer());
		convert(oox_func->m_oFuncPr.GetPointer());
	
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CFuncPr *oox_func_pr)
	{
		if (!oox_func_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathFuncPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_func_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CGroupChr *oox_group_ch)
	{
		if (!oox_group_ch) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathGroupCh", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_group_ch->m_oElement.GetPointer());
		convert(oox_group_ch->m_oGroupChrPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CGroupChrPr	*oox_group_ch_pr)
	{
		if (!oox_group_ch_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathGroupChPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_group_ch_pr->m_oChr.GetPointer());
		convert(oox_group_ch_pr->m_oCtrlPr.GetPointer());
		convert(oox_group_ch_pr->m_oPos.GetPointer());
		convert(oox_group_ch_pr->m_oVertJc.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CLimLow *oox_lim_low)
	{
		if (!oox_lim_low) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathLimLow", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_lim_low->m_oElement.GetPointer());
		convert(oox_lim_low->m_oLim.GetPointer());
		convert(oox_lim_low->m_oLimLowPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CLimLowPr *oox_lim_low_pr)
	{
		if (!oox_lim_low_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathLimLowPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_lim_low_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CLimUpp *oox_lim_upp)
	{
		if (!oox_lim_upp) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathLimUpp", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_lim_upp->m_oElement.GetPointer());
		convert(oox_lim_upp->m_oLim.GetPointer());
		convert(oox_lim_upp->m_oLimUppPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CLimUppPr *oox_lim_upp_pr)
	{
		if (!oox_lim_upp_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathLimUppPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_lim_upp_pr->m_oCtrlPr.GetPointer());
	
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMathFont *oox_math_font)
	{
		if (!oox_math_font) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathFont", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		//convert(oox_math_font->m_val.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMatrix *oox_matrix)
	{
		if (!oox_matrix) return;

		for (size_t i = 0; i < oox_matrix->m_arrItems.size(); ++i)
		{
			convert(oox_matrix->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CMc	*oox_mc)
	{
		if (!oox_mc) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathMc", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_mc->m_oMcPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMcPr *oox_mc_pr)
	{
		if (!oox_mc_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathMcPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_mc_pr->m_oCount.GetPointer());
		convert(oox_mc_pr->m_oMcJc.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMcs *oox_mcs)
	{
		if (!oox_mcs) return;

		for (size_t i = 0; i < oox_mcs->m_arrItems.size(); ++i)
		{
			convert(oox_mcs->m_arrItems[i]);
		}
	}

	void DocxConverter::convert(OOX::Logic::CMDel *oox_m_del)
	{
		if (!oox_m_del) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathMDel", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		//convert(oox_m_del->m_oDate.GetPointer());
		//convert(oox_m_del->m_oId.GetPointer());
		convert(oox_m_del->m_oRun.GetPointer());
		

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMIns *oox_m_ins)
	{
		if (!oox_m_ins) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathMIns", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		//convert(oox_m_ins->m_oDate.GetPointer());
		//convert(oox_m_ins->m_oId.GetPointer());
		convert(oox_m_ins->m_oRun.GetPointer());


		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMPr *oox_m_pr)
	{
		if (!oox_m_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathMPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

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

		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMr	*oox_mr)
	{
		if (!oox_mr) return;

		for (size_t i = 0; i < oox_mr->m_arrItems.size(); ++i)
		{
			convert(oox_mr->m_arrItems[i]);
		}		
	}

	void DocxConverter::convert(OOX::Logic::CMRun *oox_mrun)
	{
		if (!oox_mrun) return;
		// TODO <mi/mo/mn>
		
		/*odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"m", L"mathRun", elm, odf_context());
		odf_context()->math_context()->start_element(elm);*/

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
		convert(oox_mrun->m_oMRPr.GetPointer());
		convert(oox_mrun->m_oMText.GetPointer());
		convert(oox_mrun->m_oNoBreakHyphen.GetPointer());
		convert(oox_mrun->m_oObject.GetPointer());
		convert(oox_mrun->m_oPgNum.GetPointer());
		convert(oox_mrun->m_oPtab.GetPointer());
		//convert(oox_mrun->m_oRPr.GetPointer());
		convert(oox_mrun->m_oRuby.GetPointer());
		convert(oox_mrun->m_oSeparator.GetPointer());
		convert(oox_mrun->m_oSoftHyphen.GetPointer());
		convert(oox_mrun->m_oSym.GetPointer());
		convert(oox_mrun->m_oTab.GetPointer());
		convert(oox_mrun->m_oText.GetPointer());
		convert(oox_mrun->m_oYearLong.GetPointer());
		convert(oox_mrun->m_oYearShort.GetPointer());
		
		//odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CMText *oox_text)
	{
		if (!oox_text) return;

		wchar_t wch_value = *(oox_text->m_sText.c_str());
		
		odf_writer::office_element_ptr elm;

		if (wch_value <= 57 && wch_value >= 48)
		{
			odf_writer::create_element(L"math", L"mn", elm, odf_context());
		}
		else if(wch_value == '=' || wch_value == '*')
		{
			odf_writer::create_element(L"math", L"mo", elm, odf_context());
		}
		else
		{
			odf_writer::create_element(L"math", L"mi", elm, odf_context());
		}		

		odf_context()->math_context()->start_element(elm);
		odf_context()->math_context()->end_element();
		//convert(oox_text->m_oSpace.GetPointer());	
	}

	void DocxConverter::convert(OOX::Logic::CNary *oox_nary)
	{
		if (!oox_nary) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathNary", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_nary->m_oElement.GetPointer());
		convert(oox_nary->m_oNaryPr.GetPointer());
		convert(oox_nary->m_oSub.GetPointer());
		convert(oox_nary->m_oSup.GetPointer());
		
		odf_context()->math_context()->end_element();
	}
	
	void DocxConverter::convert(OOX::Logic::CNaryPr *oox_nary_pr)
	{
		if (!oox_nary_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathNaryPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_nary_pr->m_oChr.GetPointer());
		convert(oox_nary_pr->m_oCtrlPr.GetPointer());
		convert(oox_nary_pr->m_oGrow.GetPointer());
		convert(oox_nary_pr->m_oLimLoc.GetPointer());
		convert(oox_nary_pr->m_oSubHide.GetPointer());
		convert(oox_nary_pr->m_oSupHide.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CPhant *oox_phant)
	{
		if (!oox_phant) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathPhant", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_phant->m_oElement.GetPointer());
		convert(oox_phant->m_oPhantPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CPhantPr *oox_phant_pr)
	{
		if (!oox_phant_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathPhantPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_phant_pr->m_oCtrlPr.GetPointer());
		convert(oox_phant_pr->m_oShow.GetPointer());
		convert(oox_phant_pr->m_oTransp.GetPointer());
		convert(oox_phant_pr->m_oZeroAsc.GetPointer());
		convert(oox_phant_pr->m_oZeroDesc.GetPointer());
		convert(oox_phant_pr->m_oZeroWid.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CRad *oox_rad)
	{
		if (!oox_rad) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathRad", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_rad->m_oDeg.GetPointer());
		convert(oox_rad->m_oElement.GetPointer());
		convert(oox_rad->m_oRadPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CRadPr *oox_rad_pr)
	{
		if (!oox_rad_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathRadPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_rad_pr->m_oCtrlPr.GetPointer());
		convert(oox_rad_pr->m_oDegHide.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSPre *oox_s_pre)
	{
		if (!oox_s_pre) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSPre", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_s_pre->m_oElement.GetPointer());
		convert(oox_s_pre->m_oSPrePr.GetPointer());
		convert(oox_s_pre->m_oSub.GetPointer());
		convert(oox_s_pre->m_oSup.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSPrePr *oox_s_pre_pr)
	{
		if (!oox_s_pre_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSPrePr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_s_pre_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSSub *oox_ssub)
	{
		if (!oox_ssub) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSub", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssub->m_oElement.GetPointer());
		convert(oox_ssub->m_oSSubPr.GetPointer());
		convert(oox_ssub->m_oSub.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSSubPr*oox_ssub_pr)
	{
		if (!oox_ssub_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSubPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssub_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSSubSup *oox_ssub_sup)
	{
		if (!oox_ssub_sup) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSubSup", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssub_sup->m_oElement.GetPointer());
		convert(oox_ssub_sup->m_oSSubSupPr.GetPointer());
		convert(oox_ssub_sup->m_oSub.GetPointer());
		convert(oox_ssub_sup->m_oSup.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSSubSupPr *oox_ssub_sup_pr)
	{
		if (!oox_ssub_sup_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSubSupPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssub_sup_pr->m_oAlnScr.GetPointer());
		convert(oox_ssub_sup_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}

	void DocxConverter::convert(OOX::Logic::CSSup *oox_ssup)
	{
		if (!oox_ssup) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSup", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssup->m_oElement.GetPointer());
		convert(oox_ssup->m_oSSupPr.GetPointer());
		convert(oox_ssup->m_oSup.GetPointer());
		
		odf_context()->math_context()->end_element();		
	}

	void DocxConverter::convert(OOX::Logic::CSSupPr*oox_ssup_pr)
	{
		if (!oox_ssup_pr) return;

		odf_writer::office_element_ptr elm;
		odf_writer::create_element(L"math", L"mathSSupPr", elm, odf_context());
		odf_context()->math_context()->start_element(elm);

		convert(oox_ssup_pr->m_oCtrlPr.GetPointer());
		
		odf_context()->math_context()->end_element();
	}
}