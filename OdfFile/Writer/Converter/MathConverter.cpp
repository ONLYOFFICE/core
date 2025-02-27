/*
* (c) Copyright Ascensio System SIA 2010-2023
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
* You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "Converter.h"

//#include "../utils.h"

#include "../../Reader/Converter/StarMath2OOXML/cooxml2odf.h"

#include "../../OOXML/DocxFormat/DocxFlat.h"
#include "../../OOXML/DocxFormat/Math/OMath.h"
#include "../../OOXML/DocxFormat/Math/oMathContent.h"
#include "../../OOXML/DocxFormat/Math/oMathPara.h"
 
#include "../Format/odf_conversion_context.h"
#include "../Format/math_layout_elements.h"
#include "../Format/math_limit_elements.h"
#include "../Format/math_token_elements.h"
#include "../Format/style_text_properties.h"

#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

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

	void OoxConverter::lvl_up_counter_increace(double val)
	{
		double& lvl_max = odf_context()->math_context()->lvl_max;
		double& lvl_up_counter = odf_context()->math_context()->lvl_up_counter;
		lvl_up_counter += val;
		if (lvl_max < lvl_up_counter)
			lvl_max = lvl_up_counter;
	}

	void OoxConverter::lvl_up_counter_decreace(double val)
	{		
		double& lvl_up_counter = odf_context()->math_context()->lvl_up_counter;
		lvl_up_counter -= val;
	}

	void OoxConverter::lvl_down_counter_decreace(double val)
	{
		double& lvl_min = odf_context()->math_context()->lvl_min;
		double& lvl_down_counter = odf_context()->math_context()->lvl_down_counter;
		lvl_down_counter -= val;
		if (lvl_min > lvl_down_counter)
			lvl_min = lvl_down_counter;
	}

	void OoxConverter::lvl_down_counter_increace(double val)
	{
		double& lvl_down_counter = odf_context()->math_context()->lvl_down_counter;
		lvl_down_counter += val;
	}

	std::vector<int> hexToIntColor(std::wstring clr)
	{
		std::vector<int> rgb;

		std::wstringstream ss;
		for (size_t i = 0; i < 3; i++)
		{
			std::wstring substr;
			substr += clr[i * 2];
			substr += clr[i * 2 + 1];
			ss << std::hex << substr;
			int i_clr;
			ss >> i_clr;
			ss.clear();
			rgb.push_back(i_clr);
		}
		return rgb;
	}

	double dist(std::vector<int> point_1, std::vector<int> point_2)
	{
		double sum = 0;
		for(size_t i = 0; i < 3; i++)
		{
			sum += pow((point_1[i] - point_2[i]), 2);
		}
		return sqrt(sum);
	}

	std::wstring coordToColor(std::vector<int> rgb)
	{
		std::wstring arrColor[8] = { L"black", L"red", L"green", L"blue", L"magenta", L"cyan", L"yellow", L"white" };
		std::vector<std::vector<int>> coord = { {0,0,0}, {255,0,0}, {0,255,0}, {0,0,255}, {255, 0, 255}, {0,255,255}, {255,255,0}, {255,255,255} };
		std::vector<double> distArr;
		for (size_t i = 0; i < 8; i++)
		{
			distArr.push_back(dist(coord[i], rgb));
		}
		double min = distArr[0];
		int index = 0;
		for (size_t i = 1; i < 8; i++)
		{
			if (min > distArr[i])
			{
				min = distArr[i];
				index = i;
			}
		}
		return arrColor[index];
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

		int base_font_size = current_font_size.empty() ? 12 : current_font_size.back();
		std::wstring base_font_color;

		if (odf_context()->is_child_text_context() && odf_context()->drawing_context())
		{
			if (odf_context()->drawing_context()->get_text_properties())
			{
				if (odf_context()->drawing_context()->get_text_properties()->fo_color_)
				{
					base_font_color = odf_context()->drawing_context()->get_text_properties()->fo_color_->get_hex_value();
				}
			}
		}
		else
		{
			if (odf_context()->text_context()->get_text_properties())
			{
				if (odf_context()->text_context()->get_text_properties()->fo_color_)
				{
					base_font_color = odf_context()->drawing_context()->get_text_properties()->fo_color_->get_hex_value();
				}
			}
		}
		bool bStart = odf_context()->start_math(base_font_size, base_font_color);
		
		for (size_t i = 0; i < oox_math->m_arrItems.size(); ++i)
		{
			convert(oox_math->m_arrItems[i]);
		}

		if (bStart)
		{
			StarMath::COOXml2Odf starMathConverter;
			starMathConverter.SetBaseAttribute(base_font_color,base_font_size);
			starMathConverter.StartConversion(oox_math);

			std::wstring annotation_text = starMathConverter.GetAnnotation();

			if (false == annotation_text.empty())
			{
				CREATE_MATH_TAG(L"annotation");
				typedef odf_writer::math_annotation* T;
				T tmp = dynamic_cast<T>(elm.get());
				if (tmp)
				{
					tmp->encoding_ = L"StarMath 5.0";
				}
				elm->add_text(annotation_text);

				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}

			odf_context()->end_math();
		}
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

		int base_font_size = current_font_size.empty() ? 12 : current_font_size.back();
		std::wstring base_font_color;

		if (odf_context()->is_child_text_context() && odf_context()->drawing_context())
		{
			if (odf_context()->drawing_context()->get_text_properties())
			{
				if (odf_context()->drawing_context()->get_text_properties()->fo_color_)
				{
					base_font_color = odf_context()->drawing_context()->get_text_properties()->fo_color_->get_hex_value();
				}
			}
		}
		else
		{
			if (odf_context()->text_context()->get_text_properties())
			{
				if (odf_context()->text_context()->get_text_properties()->fo_color_)
				{
					base_font_color = odf_context()->drawing_context()->get_text_properties()->fo_color_->get_hex_value();
				}
			}
		}
		bool bStart = odf_context()->start_math(base_font_size, base_font_color);

		for (size_t i = 0; i < oox_math_para->m_arrItems.size(); ++i)
		{
			convert(oox_math_para->m_arrItems[i]);
		}
		if (bStart)
		{
			StarMath::COOXml2Odf starMathConverter;
			starMathConverter.SetBaseAttribute(base_font_color,base_font_size);
			starMathConverter.StartConversion(oox_math_para);

			std::wstring annotation_text = starMathConverter.GetAnnotation();

			if (false == annotation_text.empty())
			{
				CREATE_MATH_TAG(L"annotation");
				typedef odf_writer::math_annotation* T;
				T tmp = dynamic_cast<T>(elm.get());
				if (tmp)
				{
					tmp->encoding_ = L"StarMath 5.0";
				}
				elm->add_text(annotation_text);

				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
			}
			odf_context()->end_math();
		}
	}

	void OoxConverter::convert(OOX::Logic::COMathParaPr *oox_math_para_pr)
	{
		if (!oox_math_para_pr) return;
	}

	returnValues OoxConverter::convert(OOX::Logic::CCtrlPr *oox_ctrl_pr)
	{
		returnValues result;
		if (!oox_ctrl_pr)
		{
			result.auxFlag = false;
			return result;
		}

		//convert(oox_ctrl_pr->m_oARPr.GetPointer());
		//convert(oox_ctrl_pr->m_oDel.GetPointer());
		//convert(oox_ctrl_pr->m_oIns.GetPointer());
		//result.colorFlag = convert(oox_ctrl_pr->m_oRPr.GetPointer());
		return result;
	}

	void OoxConverter::convert(OOX::Logic::CAcc *oox_acc)
	{
		if (!oox_acc) return;		

		returnValues values = convert(oox_acc->m_oAccPr.GetPointer());

		CREATE_MATH_TAG(L"mover");				
		OPEN_MATH_TAG(elm);		
		lvl_up_counter_increace(1);
		std::wstring diakSymbol = (oox_acc->m_oAccPr->m_oChr.IsInit()) ? oox_acc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"";
		
		std::map<std::wstring, std::wstring>& map = odf_context()->math_context()->diak_symbols;
		std::wstring symbol;		
		
		symbol = (map[diakSymbol]);

		convert(oox_acc->m_oElement.GetPointer());

		{
			CREATE_MATH_TAG(L"mo");
			elm->add_text(symbol);
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;
		}	
		
		CLOSE_MATH_TAG;
		lvl_up_counter_decreace(1);
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CAccPr	*oox_acc_pr)
	{
		returnValues values;
		if (!oox_acc_pr) return values;

		//convert(oox_acc_pr->m_oChr.GetPointer());
		values = convert(oox_acc_pr->m_oCtrlPr.GetPointer());
		return values;
		
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
		
		returnValues values = convert(oox_bar->m_oBarPr.GetPointer());
		std::wstring tag;
		if (values.auxFlag)
		{
			tag = L"mover";
			lvl_up_counter_increace(1);
		}
		else
		{
			tag = L"munder";
			lvl_down_counter_decreace(1);
		}
		CREATE_MATH_TAG(tag.c_str());
		OPEN_MATH_TAG(elm);
		{
			convert(oox_bar->m_oElement.GetPointer());
			CREATE_MATH_TAG(L"mo");
			if (values.auxFlag)	elm->add_text(L"¯");
			else		elm->add_text(L"&#713;");			
			
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;
		}
		CLOSE_MATH_TAG;
		if (values.auxFlag)
			lvl_up_counter_decreace(1);
		else
			lvl_down_counter_increace(1);
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CBarPr	*oox_bar_pr)
	{
		returnValues values;
		if (!oox_bar_pr)
		{
			values.auxFlag = false;
			return values;
		}

		values.colorFlag = convert(oox_bar_pr->m_oCtrlPr.GetPointer()).colorFlag;
		values.auxFlag = convert(oox_bar_pr->m_oPos.GetPointer());
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CBorderBox *oox_border_box)
	{
		if (!oox_border_box) return;

		returnValues values = convert(oox_border_box->m_oBorderBoxPr.GetPointer());
		convert(oox_border_box->m_oElement.GetPointer());
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CBorderBoxPr *oox_border_box_pr)
	{
		returnValues values;
		if (!oox_border_box_pr) return values;


		values.colorFlag = convert(oox_border_box_pr->m_oCtrlPr.GetPointer()).colorFlag;
		convert(oox_border_box_pr->m_oHideBot.GetPointer());
		convert(oox_border_box_pr->m_oHideLeft.GetPointer());
		convert(oox_border_box_pr->m_oHideRight.GetPointer());
		convert(oox_border_box_pr->m_oHideTop.GetPointer());
		convert(oox_border_box_pr->m_oStrikeBLTR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeH.GetPointer());
		convert(oox_border_box_pr->m_oStrikeTLBR.GetPointer());
		convert(oox_border_box_pr->m_oStrikeV.GetPointer());
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CBox *oox_box)
	{
		if (!oox_box) return;
		returnValues val = convert(oox_box->m_oBoxPr.GetPointer());

		if (val.auxFlag)
		{
			//annotation() += L"\"";
			convert(oox_box->m_oElement.GetPointer());
			//annotation() += L"\"";
		}
		else
			convert(oox_box->m_oElement.GetPointer());
		if (val.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CBoxPr *oox_box_pr)
	{
		returnValues values;		
		if (!oox_box_pr)
		{
			values.auxFlag = false;
			return values;
		}

		convert(oox_box_pr->m_oAln.GetPointer());
		convert(oox_box_pr->m_oBrk.GetPointer());
		values.colorFlag = convert(oox_box_pr->m_oCtrlPr.GetPointer()).colorFlag;
		convert(oox_box_pr->m_oDiff.GetPointer());
		convert(oox_box_pr->m_oNoBreak.GetPointer());
		values.auxFlag = convert(oox_box_pr->m_oOpEmu.GetPointer());
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CNoBreak *oox_no_break)
	{
		if (!oox_no_break) return;
	}

	bool OoxConverter::convert(OOX::Logic::COpEmu *oox_op_emu)
	{
		if (!oox_op_emu) return false;
		if (!oox_op_emu->m_val.IsInit()) return false;

		if (oox_op_emu->m_val->ToBool())// == L"true") ||(oox_op_emu->m_val == L"1"))
			return true;
		else
			return false;
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
		returnValues values;
		for (size_t i = 0; i < oox_del->m_arrItems.size(); ++i)
		{
			if(oox_del->m_arrItems[i]->getType() == OOX::et_m_dPr)
				values = convert((OOX::Logic::CDelimiterPr*)(oox_del->m_arrItems[i]));
		}
		mrow();
		{
			CREATE_MATH_TAG(L"mo");
			elm->add_text(values.begEndChrs.first);
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
			elm->add_text(values.begEndChrs.second);
			typedef odf_writer::math_mo* T;

			T tmp = dynamic_cast<T>(elm.get());
			if (tmp)
			{
				tmp->fence_ = true;
				tmp->form_ = L"postfix";
				tmp->stretchy_ = true;
			}
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;
		}	
		endOfMrow();
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CDelimiterPr *oox_del_pr)
	{
		returnValues values;

		if (!oox_del_pr) return values;


		values.begEndChrs.first = convert(oox_del_pr->m_oBegChr.GetPointer());
		values.colorFlag = convert(oox_del_pr->m_oCtrlPr.GetPointer()).colorFlag;
		convert(oox_del_pr->m_oGrow.GetPointer());
		convert(oox_del_pr->m_oSepChr.GetPointer());
		convert(oox_del_pr->m_oShp.GetPointer());
		values.begEndChrs.second = convert(oox_del_pr->m_oEndChr.GetPointer());
		return values;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CBegChr * oox_beg_chr)
	{
		if (!oox_beg_chr) return L"(";
		if (false == oox_beg_chr->m_val.IsInit())  return L"(";

		std::wstring s_val = oox_beg_chr->m_val->GetValue();

		//brackets()[lvl_of_me()].push_back(s_val);

		return s_val;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CEndChr * oox_end_chr)
	{
		if (!oox_end_chr) return L")";
		if (false == oox_end_chr->m_val.IsInit())  return L")";

		std::wstring s_val = oox_end_chr->m_val->GetValue();

		//brackets()[lvl_of_me()].push_back(s_val);
		return s_val;
	}

	void OoxConverter::convert(OOX::Logic::CEqArr *oox_eq_arr)
	{
		if (!oox_eq_arr) return;
		returnValues values;
		if(oox_eq_arr->m_arrItems[0]->getType() == OOX::EElementType::et_m_eqArrPr)
			values = convert((OOX::Logic::CEqArrPr*)(oox_eq_arr->m_arrItems[0]));

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
		if(values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CEqArrPr *oox_eq_arr_pr)
	{
		returnValues values;
		if (!oox_eq_arr_pr) return values;

		convert(oox_eq_arr_pr->m_oBaseJc.GetPointer());
		values.colorFlag = convert(oox_eq_arr_pr->m_oCtrlPr.GetPointer()).colorFlag;
		convert(oox_eq_arr_pr->m_oMaxDist.GetPointer());
		convert(oox_eq_arr_pr->m_oObjDist.GetPointer());
		convert(oox_eq_arr_pr->m_oRSp.GetPointer());
		convert(oox_eq_arr_pr->m_oRSpRule.GetPointer());
		return values;
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


		returnValues val = convert(oox_fraction->m_oFPr.GetPointer());

		if (val.str == L"lin")
		{
			mrow();
				convert(oox_fraction->m_oNum.GetPointer());
			endOfMrow();

			CREATE_MATH_TAG(L"mo");
			elm->add_text(L"/");
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;
			mrow();
				convert(oox_fraction->m_oDen.GetPointer());
			endOfMrow();
		}
		else if (val.str == L"skw")
		{
			CREATE_MATH_TAG(L"mfrac");
			lvl_up_counter_increace(1);
			lvl_down_counter_decreace(1);
			typedef odf_writer::math_mfrac* T;
			T tmp = dynamic_cast<T>(elm.get());

			if (tmp)
			{				
				tmp->bevelled = true;				
			}
			OPEN_MATH_TAG(elm);
			mrow();
				convert(oox_fraction->m_oNum.GetPointer());
			endOfMrow();
			mrow();
				convert(oox_fraction->m_oDen.GetPointer());
			endOfMrow();
			CLOSE_MATH_TAG;
			lvl_up_counter_decreace(1);
			lvl_down_counter_increace(1);
		}
		else if (val.str == L"noBar")
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
			lvl_up_counter_increace(1);
			lvl_down_counter_decreace(1);

			OPEN_MATH_TAG(elm);
			mrow();
				convert(oox_fraction->m_oNum.GetPointer());
			endOfMrow();

			mrow();
				convert(oox_fraction->m_oDen.GetPointer());
			endOfMrow();

			CLOSE_MATH_TAG;
			lvl_up_counter_decreace(1);
			lvl_down_counter_increace(1);
		}
		if (val.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CFPr *oox_f_pr)
	{
		returnValues result;
		result.str = L"";
		if (!oox_f_pr) return result;
		
		result.str = convert(oox_f_pr->m_oType.GetPointer());
		result.colorFlag = convert(oox_f_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return result;
	}

	std::wstring OoxConverter::convert(OOX::Logic::CType *oox_type)
	{
		if (!oox_type) return L"";
		if (!oox_type->m_val.IsInit()) return L"";

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

		returnValues values = convert(oox_func->m_oFuncPr.GetPointer());
		bool flag = convert(oox_func->m_oFName.GetPointer(), oox_func->m_oElement.GetPointer());

		if(!flag)
			convert(oox_func->m_oElement.GetPointer());

		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CFuncPr *oox_func_pr)
	{
		returnValues values;
		if (!oox_func_pr) return values;

		values = convert(oox_func_pr->m_oCtrlPr.GetPointer());
		return values;
	}

	bool OoxConverter::convert(OOX::Logic::CFName *oox_fname, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_fname) return false;
		
		bool result = false;
		for (size_t i = 0; i < oox_fname->m_arrItems.size(); ++i)
		{
			if (oox_fname->m_arrItems[i]->getType() == OOX::et_m_limLow)
			{
				convert(dynamic_cast<OOX::Logic::CLimLow*>(oox_fname->m_arrItems[i]), oox_elm);
				result = true;
			}
			else
			{
				convert(oox_fname->m_arrItems[i]);						
				result = false;
			}
		}
		return result;
	}

	void OoxConverter::convert(OOX::Logic::CGroupChr* oox_group_ch, OOX::Logic::CLim* oox_lim)
	{
		if (!oox_group_ch) return;		

		returnValues values = convert(oox_group_ch->m_oGroupChrPr.GetPointer());
		std::wstring tag;
		if (values.auxFlag)
		{
			tag = L"mover";
			lvl_up_counter_increace(1);
		}
		else
		{
			tag = L"munder";
			lvl_down_counter_decreace(1);
		}
		
		
		CREATE_MATH_TAG(tag.c_str());
		if (values.auxFlag)
		{
			typedef odf_writer::math_mover* T;
			T tmp = dynamic_cast<T>(elm.get());
			tmp->accent = true;
		}
		else
		{
			typedef odf_writer::math_munder* T;
			T tmp = dynamic_cast<T>(elm.get());
			tmp->accentunder = true;
		}
		
		OPEN_MATH_TAG(elm);
		convert(oox_group_ch->m_oElement.GetPointer());
		if (values.auxFlag)
		{
			if (oox_group_ch->m_oGroupChrPr->m_oChr.IsInit() && oox_group_ch->m_oGroupChrPr->m_oChr->m_val->GetValue() == L"⏞")
			{
				if (oox_lim)
				{
					convert(oox_lim);
				}
			}
			else
			{
				convert(oox_group_ch->m_oGroupChrPr->m_oChr.GetPointer());
			}
		}
		else
		{
			if (oox_group_ch->m_oGroupChrPr->m_oChr.IsInit())
			{
				convert(oox_group_ch->m_oGroupChrPr->m_oChr.GetPointer());
			}
			else
			{
				CREATE_MATH_TAG(L"mo");
				elm->add_text(L" ⏟ ");
				OPEN_MATH_TAG(elm);
				CLOSE_MATH_TAG;
				
				if (oox_lim)
				{
					convert(oox_lim);
				}
			}			
		}
		CLOSE_MATH_TAG;
		if (values.auxFlag)
			lvl_up_counter_decreace(1);
		else
			lvl_down_counter_increace(1);
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CGroupChrPr	*oox_group_ch_pr)
	{
		returnValues values;
		if (!oox_group_ch_pr) return values;
			
		//convert(oox_group_ch_pr->m_oChr.GetPointer());
		values.colorFlag = convert(oox_group_ch_pr->m_oCtrlPr.GetPointer()).colorFlag;
		values.auxFlag = convert(oox_group_ch_pr->m_oPos.GetPointer());
		convert(oox_group_ch_pr->m_oVertJc.GetPointer());	

		return values;
	}

	bool OoxConverter::convert(OOX::Logic::CPos *oox_pos)
	{
		if (!oox_pos) return false;
		if (!oox_pos->m_val.IsInit()) return false;

		if (oox_pos->m_val->ToString() == L"top") return true;
		else return false;
	}

	void OoxConverter::convert(OOX::Logic::CVertJc *oox_vert_jc)
	{
		if (!oox_vert_jc) return;
	}

	void OoxConverter::convert(OOX::Logic::CLimLow *oox_lim_low, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_lim_low) return;
		
		returnValues values = convert(oox_lim_low->m_oLimLowPr.GetPointer());
		mrow();

		CREATE_MATH_TAG(L"munder");
		OPEN_MATH_TAG(elm);
		lvl_down_counter_decreace(1);
		if (oox_lim_low->m_oElement->m_arrItems[0]->getType() == OOX::EElementType::et_m_groupChr)
		{

			convert(dynamic_cast<OOX::Logic::CGroupChr*>(oox_lim_low->m_oElement->m_arrItems[0]), oox_lim_low->m_oLim.GetPointer());
		}
		else
		{
			mrow();
			convert(oox_lim_low->m_oElement.GetPointer());
			endOfMrow();

			convert(oox_lim_low->m_oLim.GetPointer());
		}

		CLOSE_MATH_TAG;
		lvl_down_counter_increace(1);
		if (oox_elm)
			convert(oox_elm);

		endOfMrow();
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	void OoxConverter::convert(OOX::Logic::CLim *oox_lim)
	{
		if (!oox_lim) return;
		
		mrow();

		for (size_t i = 0; i < oox_lim->m_arrItems.size(); ++i)
			convert(oox_lim->m_arrItems[i]);

		endOfMrow();		
	}

	returnValues OoxConverter::convert(OOX::Logic::CLimLowPr *oox_lim_low_pr)
	{
		returnValues values;
		if (!oox_lim_low_pr) return values;

		values.colorFlag = convert(oox_lim_low_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CLimUpp *oox_lim_upp)
	{
		if (!oox_lim_upp) return;
		
		returnValues values = convert(oox_lim_upp->m_oLimUppPr.GetPointer());
		mrow();
		CREATE_MATH_TAG(L"mover");
		OPEN_MATH_TAG(elm);
		lvl_up_counter_increace(1);
		if (oox_lim_upp->m_oElement->m_arrItems[0]->getType() == OOX::EElementType::et_m_groupChr)
		{
			
			convert(dynamic_cast<OOX::Logic::CGroupChr*>(oox_lim_upp->m_oElement->m_arrItems[0]), oox_lim_upp->m_oLim.GetPointer());
		}
		else
		{
			mrow();
			convert(oox_lim_upp->m_oElement.GetPointer());
			endOfMrow();

			convert(oox_lim_upp->m_oLim.GetPointer());
		}


		CLOSE_MATH_TAG;
		lvl_up_counter_decreace(1);
		endOfMrow();
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CLimUppPr *oox_lim_upp_pr)
	{
		returnValues values;
		if (!oox_lim_upp_pr) return values;

		values.colorFlag = convert(oox_lim_upp_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return values;

	}

	void OoxConverter::convert(OOX::Logic::CMathFont *oox_math_font)
	{
		if (!oox_math_font) return;		
	}

	void OoxConverter::convert(OOX::Logic::CMatrix *oox_matrix)
	{
		if (!oox_matrix) return;
		int& matrix_row_counter = odf_context()->math_context()->matrix_row_counter;
		returnValues values;
		if (oox_matrix->m_arrItems[0]->getType() == OOX::EElementType::et_m_mPr)
		{
			matrix_row_counter = oox_matrix->m_arrItems.size() - 1;
			values = convert((OOX::Logic::CMPr*)(oox_matrix->m_arrItems[0]));
		}
		else
			matrix_row_counter = oox_matrix->m_arrItems.size();

		CREATE_MATH_TAG(L"mtable");
		OPEN_MATH_TAG(elm);

		for (size_t i = 0; i < oox_matrix->m_arrItems.size(); ++i)
		{
			if(oox_matrix->m_arrItems[i]->getType() != OOX::EElementType::et_m_mPr)
				convert(oox_matrix->m_arrItems[i]);
		}
		CLOSE_MATH_TAG;

		odf_context()->math_context()->matrix_row_counter = 0;

		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
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

	returnValues OoxConverter::convert(OOX::Logic::CMPr *oox_m_pr)
	{
		returnValues values;
		if (!oox_m_pr) return values;

		convert(oox_m_pr->m_oBaseJc.GetPointer());
		convert(oox_m_pr->m_oCGp.GetPointer());
		convert(oox_m_pr->m_oCGpRule.GetPointer());
		convert(oox_m_pr->m_oCSp.GetPointer());
		values.colorFlag = convert(oox_m_pr->m_oCtrlPr.GetPointer()).colorFlag;
		convert(oox_m_pr->m_oMcs.GetPointer());
		convert(oox_m_pr->m_oMcs.GetPointer());
		convert(oox_m_pr->m_oPlcHide.GetPointer());
		convert(oox_m_pr->m_oRSp.GetPointer());
		convert(oox_m_pr->m_oRSpRule.GetPointer());		
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CPlcHide *oox_plc_hide)
	{
		if (!oox_plc_hide) return;
	}

	void OoxConverter::convert(OOX::Logic::CMr	*oox_mr)
	{
		if (!oox_mr) return;
		bool color_flag = false;
		for (size_t i = 0; i < oox_mr->m_arrItems.size(); ++i)
		{
			if (oox_mr->m_arrItems[i]->getType() == OOX::et_w_rPr)
				color_flag = convert((OOX::Logic::CRunProperty*)(oox_mr->m_arrItems[i]));
		}
		int& matrix_row_counter = odf_context()->math_context()->matrix_row_counter;

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

		matrix_row_counter--;

		if(color_flag)
		{
			CLOSE_MATH_TAG;
		}
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

		if (odf_context()->math_context()->style_flag)
		{
			odf_context()->math_context()->style_flag = false;

			odf_context()->settings_context()->start_view();
				if (oox_r_pr->m_oSz.IsInit() && oox_r_pr->m_oSz->m_oVal.IsInit())
				{
					odf_context()->math_context()->font_size = oox_r_pr->m_oSz->m_oVal->GetValue();
				}

				odf_context()->settings_context()->add_config_content_item(L"BaseFontHeight", L"short", std::to_wstring((int)odf_context()->math_context()->font_size));
				if (!odf_context()->math_context()->font_color.empty())
				{
					odf_context()->settings_context()->add_config_content_item(L"BaseFontColor", L"string", L"#" + odf_context()->math_context()->font_color);
				}
				
				if (oox_r_pr->m_oRFonts.IsInit() && oox_r_pr->m_oRFonts->m_sAscii.IsInit())
				{
					odf_context()->math_context()->font_name = *oox_r_pr->m_oRFonts->m_sAscii;

					odf_context()->settings_context()->add_config_content_item(L"FontNameFunctions", L"string", *oox_r_pr->m_oRFonts->m_sAscii);
					odf_context()->settings_context()->add_config_content_item(L"FontNameNumbers", L"string", *oox_r_pr->m_oRFonts->m_sAscii);
					odf_context()->settings_context()->add_config_content_item(L"FontNameText", L"string", *oox_r_pr->m_oRFonts->m_sAscii);
					odf_context()->settings_context()->add_config_content_item(L"FontNameVariables", L"string", *oox_r_pr->m_oRFonts->m_sAscii);
				}
			odf_context()->settings_context()->end_view();
		}


		if (oox_r_pr->m_oColor.IsInit() && oox_r_pr->m_oColor->m_oVal->ToString() != L"auto")
		{
			if (oox_r_pr->m_oColor->m_oVal.IsInit())
			{
				std::wstring clr = oox_r_pr->m_oColor->m_oVal.GetPointer()->ToString();
				std::wstring clr2(L"#");
				clr.erase(0, 2);
				clr2 += clr;

				CREATE_MATH_TAG(L"mstyle");
				typedef odf_writer::math_mstyle * T;
				T tmp = dynamic_cast<T>(elm.get());
				tmp->color_ = clr2;
				OPEN_MATH_TAG(elm);

				clr2.erase(0, 1);
				std::vector<int> rgb = hexToIntColor(clr2);
				boost::to_upper(clr2);

				return true;
			}
		}
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
		mrow();

		bool flag_nary = false;
		std::wstring tag;
		if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() != 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() != 0))
		{
			tag = L"munderover";			
			flag_nary = true;
			
			lvl_up_counter_increace(1);
			lvl_down_counter_decreace(1);
		}
		else if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() != 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() == 0))
		{
			tag = L"munder";
			flag_nary = true;
			lvl_down_counter_decreace(1);
		}
		else if ((oox_nary->m_oSub.GetPointer()->m_arrItems.size() == 0) && (oox_nary->m_oSup.GetPointer()->m_arrItems.size() != 0))
		{
			tag = L"mover";
			flag_nary = true;
			lvl_up_counter_increace(1);
		}

		if (flag_nary)
		{
			CREATE_MATH_TAG(tag);
			OPEN_MATH_TAG(elm);
		}

		returnValues values = convert(oox_nary->m_oNaryPr.GetPointer());
		
std::wstring str1, str2;
		if (values.naryChr)
		{
			str1 = L" from {";
			str2 = L" to {";
		}
		else
		{
			str1 = L" csub {";
			str2 = L" csup {";
		}

		if (!values.narySubHide)
		{
			//mrow();
			convert(oox_nary->m_oSub.GetPointer());
			//endOfMrow();			

		}
		if (!values.narySupHide)
		{
			//mrow();
			convert(oox_nary->m_oSup.GetPointer());
			//endOfMrow();			
		}

		if (flag_nary)
		{
			CLOSE_MATH_TAG;
			if (tag == L"munderover")
			{
				lvl_up_counter_decreace(1);
				lvl_down_counter_increace(1);
			}
			else if (tag == L"mover")
				lvl_up_counter_decreace(1);
			else if (tag == L"munder")
				lvl_down_counter_increace(1);
		}
		convert(oox_nary->m_oElement.GetPointer());
		endOfMrow();

		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CNaryPr *oox_nary_pr)
	{
		returnValues values;		
		if (!oox_nary_pr)
		{
			values.auxFlag = false;
			return values;
		}

		values.colorFlag = convert(oox_nary_pr->m_oCtrlPr.GetPointer()).colorFlag;
		values.naryChr = convert(oox_nary_pr->m_oChr.GetPointer());	
		convert(oox_nary_pr->m_oGrow.GetPointer());		
		values.narySubHide = convert(oox_nary_pr->m_oSubHide.GetPointer());
		values.narySupHide = convert(oox_nary_pr->m_oSupHide.GetPointer());

		return values;		
	}

	void OoxConverter::convert(OOX::Logic::CGrow* oox_grow)
	{
		if (!oox_grow) return;
	}

	bool OoxConverter::convert(OOX::Logic::CSubHide *oox_subHide)
	{
		if (!oox_subHide) return false;
		if (!oox_subHide->m_val.IsInit()) return false;

		bool result = oox_subHide->m_val->ToBool();
		return result;
	}

	bool OoxConverter::convert(OOX::Logic::CSupHide *oox_supHide)
	{
		if (!oox_supHide) return false;
		if (!oox_supHide->m_val.IsInit()) return false;

		bool result = oox_supHide->m_val->ToBool();
		return result;
	}

	bool OoxConverter::convert(OOX::Logic::CChr * oox_chr)
	{	
		bool flag = false;
		//if (!oox_chr) return;
		CREATE_MATH_TAG(L"mo");
		typedef odf_writer::math_mo* T;

		T tmp = dynamic_cast<T>(elm.get());
		if (tmp)
		{
			tmp->stretchy_ = false;
		}
		
		if (!oox_chr || (oox_chr && !oox_chr->m_val.IsInit()))
		{
			elm->add_text(L"∫");
		}
		else
		{
			std::wstring val = oox_chr->m_val->GetValue();
			elm->add_text(val);
		}
		OPEN_MATH_TAG(elm);
		CLOSE_MATH_TAG;
		return flag;
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
		
		returnValues val = convert(oox_rad->m_oRadPr.GetPointer());
		if (val.auxFlag)
		{
			CREATE_MATH_TAG(L"msqrt");
			OPEN_MATH_TAG(elm);

			mrow();
				convert(oox_rad->m_oElement.GetPointer());
			endOfMrow();

			CLOSE_MATH_TAG;
			odf_context()->math_context()->symbol_counter++;
		}
		else
			convert(oox_rad->m_oDeg.GetPointer(), oox_rad->m_oElement.GetPointer());
		

		if (val.colorFlag)
		{
			CLOSE_MATH_TAG;
		}

	}

	returnValues OoxConverter::convert(OOX::Logic::CRadPr *oox_rad_pr)
	{
		returnValues result;
		if (!oox_rad_pr) result.auxFlag = false;

		result.auxFlag = convert(oox_rad_pr->m_oDegHide.GetPointer());
		result.colorFlag = convert(oox_rad_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return result;
	}

	bool OoxConverter::convert(OOX::Logic::CDegHide *oox_deg_hide)
	{
		if (!oox_deg_hide) return false;	
		if (!oox_deg_hide->m_val.IsInit()) return false;

		return oox_deg_hide->m_val->ToBool();
	}

	void OoxConverter::convert(OOX::Logic::CDeg *oox_deg, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_deg) return;
		if (oox_deg->m_arrItems.size() == 0) return;
		
		CREATE_MATH_TAG(L"mroot");
		OPEN_MATH_TAG(elm);

		convert(oox_elm);

		mrow();
			for (size_t i = 0; i < oox_deg->m_arrItems.size(); ++i)
				convert(oox_deg->m_arrItems[i]);
		endOfMrow();
		
		CLOSE_MATH_TAG;
	}

	void OoxConverter::convert(OOX::Logic::CSPre *oox_s_pre)
	{
		if (!oox_s_pre) return;

		returnValues values = convert(oox_s_pre->m_oSPrePr.GetPointer());
		
		CREATE_MATH_TAG(L"mmultiscripts");
		OPEN_MATH_TAG(elm);
		lvl_up_counter_increace(0.4);
		lvl_down_counter_decreace(0.4);
		mrow();
			convert(oox_s_pre->m_oElement.GetPointer());
		endOfMrow();

		{
			CREATE_MATH_TAG(L"mprescripts");
			OPEN_MATH_TAG(elm);
			CLOSE_MATH_TAG;			

			mrow();
				convert(oox_s_pre->m_oSub.GetPointer());
			endOfMrow();

			mrow();
				convert(oox_s_pre->m_oSup.GetPointer());
			endOfMrow();
		}
		CLOSE_MATH_TAG;
		lvl_up_counter_decreace(0.4);
		lvl_down_counter_increace(0.4);
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CSPrePr *oox_s_pre_pr)
	{
		returnValues values;
		if (!oox_s_pre_pr) return values;
	
		values = convert(oox_s_pre_pr->m_oCtrlPr.GetPointer());
		return values;		
	}

	void OoxConverter::convert(OOX::Logic::CSup *oox_sup, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_sup) return;
		
		CREATE_MATH_TAG(L"msup");
		OPEN_MATH_TAG(elm);		
		lvl_up_counter_increace(0.4);

		mrow();
		convert(oox_elm);	
		endOfMrow();

		mrow();
		for (size_t i = 0; i < oox_sup->m_arrItems.size(); ++i)
		{
			convert(oox_sup->m_arrItems[i]);
		}
		endOfMrow();

		CLOSE_MATH_TAG;
		lvl_up_counter_decreace(0.4);
	}

	void OoxConverter::convert(OOX::Logic::CSub *oox_sub, OOX::Logic::CElement *oox_elm)
	{
		if (!oox_sub) return;
		
		CREATE_MATH_TAG(L"msub");
		OPEN_MATH_TAG(elm);
		lvl_down_counter_decreace(0.4);
		
		mrow();
		convert(oox_elm);
		endOfMrow();

		for (size_t i = 0; i < oox_sub->m_arrItems.size(); ++i)
		{
			convert(oox_sub->m_arrItems[i]);

		}
		CLOSE_MATH_TAG;
		lvl_down_counter_increace(0.4);
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

		returnValues values = convert(oox_ssub->m_oSSubPr.GetPointer());
		convert(oox_ssub->m_oSub.GetPointer(), oox_ssub->m_oElement.GetPointer());	
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CSSubPr*oox_ssub_pr)
	{
		returnValues values;
		if (!oox_ssub_pr) return values;

		values.colorFlag = convert(oox_ssub_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return values;		
	}

	void OoxConverter::convert(OOX::Logic::CSSubSup *oox_ssub_sup)
	{
		if (!oox_ssub_sup) return;

		returnValues values = convert(oox_ssub_sup->m_oSSubSupPr.GetPointer());
		{
			CREATE_MATH_TAG(L"msubsup");
			OPEN_MATH_TAG(elm);
			lvl_up_counter_increace(0.4);
			lvl_down_counter_decreace(0.4);

			mrow();
			convert(oox_ssub_sup->m_oElement.GetPointer());
			endOfMrow();

			mrow();
			convert(oox_ssub_sup->m_oSub.GetPointer());
			endOfMrow();

			mrow();
			convert(oox_ssub_sup->m_oSup.GetPointer());
			endOfMrow();

			CLOSE_MATH_TAG;
			lvl_up_counter_decreace(0.4);
			lvl_down_counter_increace(0.4);
		}
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CSSubSupPr *oox_ssub_sup_pr)
	{
		returnValues values;
		if (!oox_ssub_sup_pr) return values;


		convert(oox_ssub_sup_pr->m_oAlnScr.GetPointer());
		values.colorFlag = convert(oox_ssub_sup_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return values;
	}

	void OoxConverter::convert(OOX::Logic::CAlnScr* oox_aln_scr)
	{
		if (!oox_aln_scr) return;
	}

	void OoxConverter::convert(OOX::Logic::CSSup *oox_ssup)
	{
		if (!oox_ssup) return;	
		
		returnValues values = convert(oox_ssup->m_oSSupPr.GetPointer());

		convert(oox_ssup->m_oSup.GetPointer(), oox_ssup->m_oElement.GetPointer());
		if (values.colorFlag)
		{
			CLOSE_MATH_TAG;
		}
	}

	returnValues OoxConverter::convert(OOX::Logic::CSSupPr *oox_ssup_pr)
	{
		returnValues values;
		if (!oox_ssup_pr) return values;

		values.colorFlag = convert(oox_ssup_pr->m_oCtrlPr.GetPointer()).colorFlag;
		return values;
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
		mrow();
		for (size_t i = 0; i < oox_elm->m_arrItems.size(); ++i)
		{
			convert(oox_elm->m_arrItems[i]);
		}
		endOfMrow();

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
