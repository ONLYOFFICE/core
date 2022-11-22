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

#include "StringPtgParser.h"
#include "PtgInt.h"
#include "PtgNum.h"
#include "PtgBool.h"
#include "PtgStr.h"
#include "PtgName.h"
#include "PtgRefErr.h"
#include "PtgErr.h"
#include "PtgArea.h"
#include "PtgRef.h"
#include "PtgRef3d.h"
#include "PtgArea3d.h"
#include "PtgRefErr3d.h"
#include "PtgArray.h"
#include "PtgExtraArray.h"
#include "PtgParen.h"
#include "PtgFunc.h"
#include "PtgFuncVar.h"
#include "PtgMissArg.h"
#include "PtgNameX.h"
#include "PtgRefN.h"
#include "PtgAreaN.h"

#include "OperatorPtgs.h"
#include "SyntaxPtg.h"
#include "Rgce.h"
#include "RgbExtra.h"

#include <boost/regex.hpp>

namespace XLS
{


// The algorithm has been copied to EWSEditor. All code changes shall be synchronized.
//const bool StringPtgParser::parseToPtgs(const std::wstring& assembled_formula, Rgce& rgce, RgbExtra& rgb, const std::wstring & tag_name)
//{
//	PtgPtr last_ptg;
//	bool operand_expected = true; // This would help distinguish unary and binary and determine if an argument to a function is missed.
//	
//	for(std::wstring::const_iterator it = assembled_formula.begin(), itEnd = assembled_formula.end(); it != itEnd;)
//	{
//		#pragma region Operators
//		if(SyntaxPtg::is_operators(it, itEnd) || SyntaxPtg::is_PtgIsect(it, itEnd))
//		{
//			OperatorPtgPtr found_operator;
//			if(SyntaxPtg::extract_PtgAdd(it, itEnd, operand_expected))
//			{
//				found_operator.reset(new PtgAdd);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgUplus(it, itEnd, operand_expected))
//			{
//				found_operator.reset(new PtgUplus);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgSub(it, itEnd, operand_expected))
//			{
//				found_operator.reset(new PtgSub);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgUminus(it, itEnd, operand_expected))
//			{
//				found_operator.reset(new PtgUminus);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgPercent(it, itEnd, operand_expected))
//			{
//				found_operator.reset(new PtgPercent);
//				operand_expected = false;
//			}
//			else if(SyntaxPtg::extract_PtgMul(it, itEnd))
//			{
//				found_operator.reset(new PtgMul);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgDiv(it, itEnd))
//			{
//				found_operator.reset(new PtgDiv);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgPower(it, itEnd))
//			{
//				found_operator.reset(new PtgPower);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgEq(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgEq);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgNe(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgNe);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgLe(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgLe);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgLt(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgLt);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgGe(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgGe);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgGt(it, itEnd)) // Sequence is important
//			{
//				found_operator.reset(new PtgGt);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgConcat(it, itEnd))
//			{
//				found_operator.reset(new PtgConcat);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgUnion(it, itEnd))
//			{
//				found_operator.reset(new PtgUnion);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgRange(it, itEnd))
//			{
//				found_operator.reset(new PtgRange);
//				operand_expected = true;
//			}
//			else if(SyntaxPtg::extract_PtgIsect(it, itEnd)) // Must be the last of operators
//			{
//				found_operator.reset(new PtgIsect);
//				operand_expected = true;
//			}
//			else
//			{
//				// EXCEPT::RT::WrongFormulaString("Unknown operator format in formula.", assembled_formula);
//			}
//			OperatorPtgPtr operator_top;
//			while(ptg_stack.size() && (operator_top = boost::dynamic_pointer_cast<OperatorPtg>(ptg_stack.top())) &&
//				(found_operator->isRightAssociative() ? (operator_top->getOperatorPriority() < found_operator->getOperatorPriority()) :
//				(operator_top->getOperatorPriority() <= found_operator->getOperatorPriority())))
//			{
//				rgce.addPtg(ptg_stack.top());
//				ptg_stack.pop();
//			}
//			ptg_stack.push(found_operator);
//			last_ptg = found_operator;
//		}
//		#pragma endregion
//		#pragma region Parenthesis
//		else if(SyntaxPtg::extract_LeftParenthesis(it, itEnd))
//		{
//			PtgFuncPtr func;
//			 // Check if the parenthesis is the left one of a function without parameters
//			operand_expected = (ptg_stack.size() && (func = boost::dynamic_pointer_cast<PtgFunc>(ptg_stack.top())) && !func->getParametersNum()) ? false : true;
//			ptg_stack.push(last_ptg = PtgPtr(new PtgParen));
//		}
//		else if(SyntaxPtg::extract_RightParenthesis(it, itEnd))
//		{
//			PtgParenPtr left_p;
//			if(ptg_stack.size() && (left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())) && operand_expected)
//			{
//				if(left_p->getParametersNum()) // This means there is at least one comma inside the parenthesis
//				{
//					rgce.addPtg(PtgPtr(new PtgMissArg));
//				}
//				else
//				{
//					left_p->decrementParametersNum();
//				}
//			}
//			else
//			{
//				while(ptg_stack.size() && !(left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())))
//				{
//					rgce.addPtg(ptg_stack.top());
//					ptg_stack.pop();
//				}
//			}
//			if(!ptg_stack.size() || !left_p)
//			{
//				// EXCEPT::RT::WrongParenthesisSequence(assembled_formula);
//			}
//			ptg_stack.pop(); // pop PtgParen that is now stored in left_p
//			last_ptg = left_p;
//			PtgFuncVarPtr func_var;
//			if(ptg_stack.size() && boost::dynamic_pointer_cast<PtgFunc>(ptg_stack.top()))
//			{
//				last_ptg = ptg_stack.top();
//				ptg_stack.pop(); // pop PtgFunc
//			}
//			else if(ptg_stack.size() && (func_var = boost::dynamic_pointer_cast<PtgFuncVar>(ptg_stack.top())))
//			{
//				size_t num_parameters = left_p->getParametersNum() + 1;
//				if(num_parameters > 255)
//				{
//					// EXCEPT::RT::WrongFormulaString("Too many parameters to a function", assembled_formula);
//				}
//				func_var->setParamsNum(static_cast<unsigned char>(num_parameters));
//				last_ptg = ptg_stack.top();
//				ptg_stack.pop(); // pop PtgFuncVar
//			}
//			else // If there is no function name before the left parenthesis
//			{
//				for (size_t i = 0; i < left_p->getParametersNum(); ++i)
//				{
//					rgce.addPtg(PtgPtr(new PtgUnion));
//				}
//			}
//			rgce.addPtg(last_ptg);
//			operand_expected = false;
//		}
//		#pragma endregion
//		#pragma region Comma and PtgUnion
//		else if(SyntaxPtg::extract_comma(it, itEnd))
//		{
//			PtgParenPtr left_p;
//			if(ptg_stack.size() && (left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())) && operand_expected)
//			{
//				rgce.addPtg(PtgPtr(new PtgMissArg));
//			}
//			else
//			{
//				while(ptg_stack.size() && !(left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())))
//				{
//					rgce.addPtg(ptg_stack.top());
//					ptg_stack.pop();
//				}
//			}
//			if(!ptg_stack.size() || !left_p)
//			{
//				// EXCEPT::RT::WrongParenthesisSequence(assembled_formula);
//			}
//			left_p->incrementParametersNum(); // The count of parameters will be transferred to PtgFuncVar
//			last_ptg = left_p; // PtgParen. Mostly to differ unary and binary minuses and pluses
//			operand_expected = true;
//		}
//		#pragma endregion
//		#pragma region Operands
//		else
//		{
//			OperandPtgPtr found_operand;
//			std::wstring operand_str;
//			unsigned int number;
//			unsigned short ixti;
//
//			if(SyntaxPtg::extract_PtgBool(it, itEnd, operand_str))
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgBool(operand_str)));
//			}
//			else if(SyntaxPtg::extract_PtgStr(it, itEnd, operand_str))
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgStr(operand_str.c_str())));
//			}
//			else if(SyntaxPtg::extract_PtgRefErr(it, itEnd)) // Sequence is important (in pair with PtgErr)
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgRefErr(OperandPtg::pdt_REFERENCE))); // Checked with SIN(val) - OK
//			}
//			else if(SyntaxPtg::extract_PtgErr(it, itEnd, operand_str)) // Sequence is important (in pair with PtgRefErr)
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgErr(operand_str.c_str())));
//			}
//			else if(SyntaxPtg::extract_3D_part(it, itEnd, ixti)) // Shall be placed strongly before extract_PtgName
//			{
//				if(SyntaxPtg::extract_PtgArea(it, itEnd, operand_str))
//				{
//					rgce.addPtg(found_operand = OperandPtgPtr(new PtgArea3d(ixti, operand_str, OperandPtg::pdt_VALUE, rgce.getLocation())));
//				}
//				else if(SyntaxPtg::extract_PtgRef(it, itEnd, operand_str))
//				{
//					rgce.addPtg(found_operand = OperandPtgPtr(new PtgRef3d(ixti, operand_str, OperandPtg::pdt_VALUE, rgce.getLocation())));
//				}
//				else if(SyntaxPtg::extract_PtgRefErr(it, itEnd))
//				{
//					rgce.addPtg(found_operand = OperandPtgPtr(new PtgRefErr3d(ixti, OperandPtg::pdt_VALUE)));
//				}
//				else
//				{
//					// EXCEPT::RT::WrongFormulaString("Unknown format of 3D reference in formula.", assembled_formula);
//				}
//			}
//			else if(SyntaxPtg::extract_PtgName(it, itEnd, number))// Shall be placed strongly before PtgArea and PtgRef
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgName(number, OperandPtg::pdt_VALUE)));
//			}
//			else if(SyntaxPtg::extract_PtgArea(it, itEnd, operand_str)) // Sequence is important (in pair with PtgRef)
//			{
//				if(L"SharedParsedFormula" == tag_name || L"CFParsedFormulaNoCCE" == tag_name)
//				{
//					found_operand = OperandPtgPtr(new PtgAreaN(operand_str, OperandPtg::pdt_VALUE, rgce.getLocation()));
//				}
//				else
//				{
//					found_operand = OperandPtgPtr(new PtgArea(operand_str, OperandPtg::pdt_VALUE));
//				}
//				rgce.addPtg(found_operand);
//			}
//			else if(SyntaxPtg::extract_PtgRef(it, itEnd, operand_str)) // Sequence is important (in pair with PtgArea)
//			{
//				if(L"SharedParsedFormula" == tag_name || L"CFParsedFormulaNoCCE" == tag_name)
//				{
//					found_operand = OperandPtgPtr(new PtgRefN(operand_str, OperandPtg::pdt_VALUE, rgce.getLocation()));
//				}
//				else
//				{
//					found_operand = OperandPtgPtr(new PtgRef(operand_str, OperandPtg::pdt_VALUE));
//				}
//				rgce.addPtg(found_operand);
//			}
//			else if(SyntaxPtg::extract_PtgInt(it, itEnd, operand_str)) // Shall be placed after PtgArea but before UndefinedName
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgInt(operand_str)));
//			}
//			else if(SyntaxPtg::extract_PtgNum(it, itEnd, operand_str)) // Shall be placed after PtgArea but before UndefinedName
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgNum(operand_str)));
//			}
//
//			else if(SyntaxPtg::extract_UndefinedName(it, itEnd)) // Shall be placed strongly after extract_PtgName
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgErr(L"#REF!")));
//			}
//
//			else if(SyntaxPtg::extract_PtgArray(it, itEnd, operand_str))
//			{
//				rgce.addPtg(found_operand = OperandPtgPtr(new PtgArray(OperandPtg::pdt_ARRAY)));
//				rgb.addPtg(PtgPtr(new PtgExtraArray(operand_str)));
//			}
//			else if(SyntaxPtg::extract_PtgFunc(it, itEnd, operand_str))
//			{
//				PtgPtr func;
//				if((func = PtgFunc::create(operand_str, OperandPtg::pdt_VALUE)) ||
//					(func = PtgFuncVar::create(operand_str, OperandPtg::pdt_VALUE)))
//				{
//					ptg_stack.push(func);
//				}
//				else
//				{
//					func = PtgFuncVar::create(L"USER_DEFINED_FUNCTION", OperandPtg::pdt_VALUE);
//					if(!func)
//					{
//						// EXCEPT::LE::WhatIsTheFuck("Ftab_Cetab doesn't contain info about user-defined function (0xFF).", __FUNCTION__);
//					}
//					ptg_stack.push(func);
//					rgce.addPtg(PtgPtr(new PtgNameX(operand_str,  OperandPtg::pdt_VALUE)));
//				}
//			}
//			else
//			{
//				// EXCEPT::RT::WrongFormulaString("Unknown operand format in formula.", assembled_formula);
//			}
//			last_ptg = found_operand;
//			operand_expected = false;
//		}
//		#pragma endregion
//	}
//
//	while(ptg_stack.size())
//	{
//		OperatorPtgPtr operator_ptg = boost::dynamic_pointer_cast<OperatorPtg>(ptg_stack.top());
//		if(operator_ptg)
//		{
//			rgce.addPtg(ptg_stack.top());
//			ptg_stack.pop();
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//
//
} // namespace XLS

