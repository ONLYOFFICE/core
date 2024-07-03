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
#include "PtgList.h"

#include "OperatorPtgs.h"
#include "SyntaxPtg.h"
#include "Rgce.h"
#include "RgbExtra.h"
#include "FutureFunctionParser.h"

#include <boost/regex.hpp>

namespace XLS
{


// The algorithm has been copied to EWSEditor. All code changes shall be synchronized.
const bool StringPtgParser::parseToPtgs(const std::wstring& assembled_formula, Rgce& rgce, RgbExtra& rgb, const std::wstring & tag_name)
{
    PtgPtr last_ptg;
    bool operand_expected = true; // This would help distinguish unary and binary and determine if an argument to a function is missed.
    bool union_expected = false;

    for(std::wstring::const_iterator it = assembled_formula.begin(), itEnd = assembled_formula.end(); it != itEnd;)
    {
        #pragma region Operators
        if(SyntaxPtg::is_operators(it, itEnd) || SyntaxPtg::is_PtgIsect(it, itEnd))
        {
            OperatorPtgPtr found_operator;
            if(SyntaxPtg::extract_PtgAdd(it, itEnd, operand_expected))
            {
                found_operator.reset(new PtgAdd);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgUplus(it, itEnd, operand_expected))
            {
                found_operator.reset(new PtgUplus);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgSub(it, itEnd, operand_expected))
            {
                found_operator.reset(new PtgSub);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgUminus(it, itEnd, operand_expected))
            {
                found_operator.reset(new PtgUminus);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgPercent(it, itEnd, operand_expected))
            {
                found_operator.reset(new PtgPercent);
                operand_expected = false;
            }
            else if(SyntaxPtg::extract_PtgMul(it, itEnd))
            {
                found_operator.reset(new PtgMul);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgDiv(it, itEnd))
            {
                found_operator.reset(new PtgDiv);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgPower(it, itEnd))
            {
                found_operator.reset(new PtgPower);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgEq(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgEq);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgNe(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgNe);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgLe(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgLe);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgLt(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgLt);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgGe(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgGe);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgGt(it, itEnd)) // Sequence is important
            {
                found_operator.reset(new PtgGt);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgConcat(it, itEnd))
            {
                found_operator.reset(new PtgConcat);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgUnion(it, itEnd))
            {
                found_operator.reset(new PtgUnion);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgRange(it, itEnd))
            {
                found_operator.reset(new PtgRange);
                operand_expected = true;
            }
            else if(SyntaxPtg::extract_PtgIsect(it, itEnd)) // Must be the last of operators
            {
                found_operator.reset(new PtgIsect);
                operand_expected = true;
            }
            else
            {
                // EXCEPT::RT::WrongFormulaString("Unknown operator format in formula.", assembled_formula);
            }
            OperatorPtgPtr operator_top;
            while(ptg_stack.size() && (operator_top = boost::dynamic_pointer_cast<OperatorPtg>(ptg_stack.top())) &&
                (found_operator->isRightAssociative() ? (operator_top->getOperatorPriority() < found_operator->getOperatorPriority()) :
                (operator_top->getOperatorPriority() <= found_operator->getOperatorPriority())))
            {
                rgce.addPtg(ptg_stack.top());
                ptg_stack.pop();
            }
            ptg_stack.push(found_operator);
            last_ptg = found_operator;
        }
        #pragma endregion
        #pragma region Parenthesis
        else if(SyntaxPtg::extract_LeftParenthesis(it, itEnd))
        {
            PtgFuncPtr func;
             // Check if the parenthesis is the left one of a function without parameters
            operand_expected = (ptg_stack.size() && (func = boost::dynamic_pointer_cast<PtgFunc>(ptg_stack.top())) && !func->getParametersNum()) ? false : true;
            ptg_stack.push(last_ptg = PtgPtr(new PtgParen));
        }
        else if(SyntaxPtg::extract_RightParenthesis(it, itEnd))
        {
            PtgParenPtr left_p;
            if(ptg_stack.size() && (left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())) && operand_expected)
            {
                if(left_p->getParametersNum()) // This means there is at least one comma inside the parenthesis
                {
                    rgce.addPtg(PtgPtr(new PtgMissArg));
                }
                else
                {
                    left_p->decrementParametersNum();
                }
            }
            else
            {
                while(ptg_stack.size() && !(left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())))
                {
                    rgce.addPtg(ptg_stack.top());
                    ptg_stack.pop();
                }
            }
            if(!ptg_stack.size() || !left_p)
            {
                // EXCEPT::RT::WrongParenthesisSequence(assembled_formula);
            }
            if(!ptg_stack.empty())
                ptg_stack.pop(); // pop PtgParen that is now stored in left_p
            last_ptg = left_p;
            PtgFuncVarPtr func_var;
            if(ptg_stack.size() && boost::dynamic_pointer_cast<PtgFunc>(ptg_stack.top()))
            {
                last_ptg = ptg_stack.top();
                ptg_stack.pop(); // pop PtgFunc
            }
            else if(ptg_stack.size() && (func_var = boost::dynamic_pointer_cast<PtgFuncVar>(ptg_stack.top())))
            {
                size_t num_parameters = left_p->getParametersNum() + 1;
                if(num_parameters > 255)
                {
                    // EXCEPT::RT::WrongFormulaString("Too many parameters to a function", assembled_formula);
                }
                func_var->setParamsNum(static_cast<unsigned char>(num_parameters));
                last_ptg = ptg_stack.top();
                ptg_stack.pop(); // pop PtgFuncVar
            }
            else if(left_p)// If there is no function name before the left parenthesis
            {
                for (size_t i = 0; i < left_p->getParametersNum(); ++i)
                {
                    rgce.addPtg(PtgPtr(new PtgUnion));
                }
            }
            if(last_ptg)
                rgce.addPtg(last_ptg);
            operand_expected = false;
        }
        #pragma endregion
        #pragma region Comma and PtgUnion
        else if(SyntaxPtg::extract_comma(it, itEnd))
        {
            SyntaxPtg::remove_extraSymbols(it, itEnd);
            PtgParenPtr left_p;
            if(ptg_stack.size() && (left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())) && operand_expected)
            {
                rgce.addPtg(PtgPtr(new PtgMissArg));
            }
            else
            {
                while(ptg_stack.size() && !(left_p = boost::dynamic_pointer_cast<PtgParen>(ptg_stack.top())))
                {
                    rgce.addPtg(ptg_stack.top());
                    ptg_stack.pop();
                }
            }
            if(!ptg_stack.size() || !left_p)
            {
                operand_expected = true;
                union_expected = true;
                continue;// EXCEPT::RT::WrongParenthesisSequence(assembled_formula);
            }
            left_p->incrementParametersNum(); // The count of parameters will be transferred to PtgFuncVar
            last_ptg = left_p; // PtgParen. Mostly to differ unary and binary minuses and pluses
            operand_expected = true;
        }
        #pragma endregion
        #pragma region Operands
        else
        {
            OperandPtgPtr found_operand;
            std::wstring operand_str;
            unsigned int number;
            unsigned short ixti;
			PtgList ptgList(PtgList::fixed_id);
            ptgList.type_ = 0x00;

            if(SyntaxPtg::extract_PtgBool(it, itEnd, operand_str))
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgBool(operand_str)));
            }
            else if(SyntaxPtg::extract_PtgStr(it, itEnd, operand_str))
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgStr(operand_str.c_str())));
            }
            else if(SyntaxPtg::extract_PtgRefErr(it, itEnd)) // Sequence is important (in pair with PtgErr)
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgRefErr(OperandPtg::ptg_REFERENCE))); // Checked with SIN(val) - OK
            }
            else if(SyntaxPtg::extract_PtgErr(it, itEnd, operand_str)) // Sequence is important (in pair with PtgRefErr)
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgErr(operand_str.c_str())));
            }
            else if(SyntaxPtg::extract_3D_part(it, itEnd, ixti)) // Shall be placed strongly before extract_PtgName
            {
                if(SyntaxPtg::extract_PtgArea(it, itEnd, operand_str))
                {
                    rgce.addPtg(found_operand = OperandPtgPtr(new PtgArea3d(ixti, operand_str, OperandPtg::ptg_REFERENCE, rgce.getLocation())));
                }
                else if(SyntaxPtg::extract_PtgRef(it, itEnd, operand_str))
                {
                    auto pos = std::find_if(XLS::GlobalWorkbookInfo::arXti_External_static.cbegin(), XLS::GlobalWorkbookInfo::arXti_External_static.cend(),
                            [&](XLS::GlobalWorkbookInfo::_xti i) {
                        return i.iSup == ixti;
                    });
                    if(pos->itabFirst == pos->itabLast)
                        rgce.addPtg(found_operand = OperandPtgPtr(new PtgRef3d(ixti, operand_str, OperandPtg::ptg_VALUE, rgce.getLocation())));
                    else
                        rgce.addPtg(found_operand = OperandPtgPtr(new PtgRef3d(ixti, operand_str, OperandPtg::ptg_REFERENCE, rgce.getLocation())));
                }
                else if(SyntaxPtg::extract_PtgRefErr(it, itEnd))
                {
                    rgce.addPtg(found_operand = OperandPtgPtr(new PtgRefErr3d(ixti, OperandPtg::ptg_VALUE)));
                }
				else if (SyntaxPtg::extract_PtgList(it, itEnd, ptgList, ixti))// Shall be placed strongly before PtgArea and PtgRef
				{
                    if((ptgList.rowType == 0x10 || ptgList.rowType == 0x08 || ptgList.rowType == 0x02)
                        && ptgList.columns == 0x01)
                    ptgList.type_ = 0x01;
					rgce.addPtg(found_operand = OperandPtgPtr(new PtgList(ptgList)));
				}
				else if (SyntaxPtg::extract_PtgName(it, itEnd, number))// Shall be placed strongly before PtgArea and PtgRef
				{
					rgce.addPtg(found_operand = OperandPtgPtr(new PtgNameX(ixti, number, OperandPtg::ptg_REFERENCE)));
				}
                else
                {
                    // EXCEPT::RT::WrongFormulaString("Unknown format of 3D reference in formula.", assembled_formula);
                }
            }
            else if(SyntaxPtg::extract_PtgName(it, itEnd, number))// Shall be placed strongly before PtgArea and PtgRef
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgName(number, OperandPtg::ptg_REFERENCE)));
            }
			else if (SyntaxPtg::extract_PtgList(it, itEnd, ptgList))// Shall be placed strongly before PtgArea and PtgRef
			{
				rgce.addPtg(found_operand = OperandPtgPtr(new PtgList(ptgList)));
			}
            else if(SyntaxPtg::extract_PtgArea(it, itEnd, operand_str)) // Sequence is important (in pair with PtgRef)
            {
                if(L"SharedParsedFormula" == tag_name || L"CFParsedFormulaNoCCE" == tag_name)
                {
                    found_operand = OperandPtgPtr(new PtgAreaN(operand_str, OperandPtg::ptg_REFERENCE, rgce.getLocation()));
                }
                else
                {
                    found_operand = OperandPtgPtr(new PtgArea(operand_str, OperandPtg::ptg_REFERENCE));
                }
                rgce.addPtg(found_operand);
            }
            else if(SyntaxPtg::extract_PtgRef(it, itEnd, operand_str)) // Sequence is important (in pair with PtgArea)
            {
                if(L"SharedParsedFormula" == tag_name || L"CFParsedFormulaNoCCE" == tag_name)
                {
                    found_operand = OperandPtgPtr(new PtgRefN(operand_str, OperandPtg::ptg_REFERENCE, rgce.getLocation()));
                }
                else
                {
                    found_operand = OperandPtgPtr(new PtgRef(operand_str, OperandPtg::ptg_REFERENCE));
                }
                rgce.addPtg(found_operand);
            }
            else if(SyntaxPtg::extract_PtgInt(it, itEnd, operand_str)) // Shall be placed after PtgArea but before UndefinedName
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgInt(operand_str)));
            }
            else if(SyntaxPtg::extract_PtgNum(it, itEnd, operand_str)) // Shall be placed after PtgArea but before UndefinedName
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgNum(operand_str)));
            }
            else if(SyntaxPtg::extract_PtgFunc(it, itEnd, operand_str))
            {
                PtgPtr func;
                if((func = PtgFunc::create(operand_str, OperandPtg::ptg_REFERENCE)) ||
                    (func = PtgFuncVar::create(operand_str, OperandPtg::ptg_REFERENCE)))
                {
                    ptg_stack.push(func);
                }
                else if(SyntaxPtg::extract_FutureFunction(operand_str, number))
                {
                    func = PtgFuncVar::create(L"USER_DEFINED_FUNCTION", OperandPtg::ptg_REFERENCE);
                    auto funcPtr = dynamic_cast<PtgFuncVar*>(func.get());
                    funcPtr->setFutureFuncName(operand_str);
    
                    PtgPtr FuncName = PtgPtr(new PtgName(number, OperandPtg::ptg_REFERENCE));
                    ptg_stack.push(func);
                    rgce.addPtg(FuncName);
                }
                else
                {
                    SyntaxPtg::extract_CustomFunction(operand_str, number);
                    func = PtgFuncVar::create(L"USER_DEFINED_FUNCTION", OperandPtg::ptg_REFERENCE);
                    if(!func)
                    {
                        // EXCEPT::LE::WhatIsTheFuck("Ftab_Cetab doesn't contain info about user-defined function (0xFF).", __FUNCTION__);
                    }
                    ptg_stack.push(func);
                    rgce.addPtg(PtgPtr(new PtgName(number,  OperandPtg::ptg_REFERENCE)));
                }
            }
            else if(SyntaxPtg::extract_UndefinedName(it, itEnd)) // Shall be placed strongly after extract_PtgName
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgErr(L"#REF!")));
            }

            else if(SyntaxPtg::extract_PtgArray(it, itEnd, operand_str))
            {
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgArray(OperandPtg::ptg_ARRAY)));
                rgb.addPtg(PtgPtr(new PtgExtraArray(operand_str)));
            }

            else
            {
               //add error name to prevent endless formula conversion
                rgce.sequence.clear();
                rgce.addPtg(found_operand = OperandPtgPtr(new PtgErr(L"#NAME?")));
                break;
                // EXCEPT::RT::WrongFormulaString("Unknown operand format in formula.", assembled_formula);
            }
            last_ptg = found_operand;
            operand_expected = false;
            if(union_expected)
            {
                rgce.addPtg(PtgPtr(new PtgUnion));
                union_expected = false;
            }
        }
        #pragma endregion
    }

    while(ptg_stack.size())
    {
        OperatorPtgPtr operator_ptg = boost::dynamic_pointer_cast<OperatorPtg>(ptg_stack.top());
        if(operator_ptg)
        {
            rgce.addPtg(ptg_stack.top());
            ptg_stack.pop();
        }
        else
        {
            return false;
        }
    }
    parsePtgTypes(rgce);
    return true;
}

void SetPtgType(PtgPtr ptg, const char type)
{
    if(ptg->ptg_id.get() > 0x1F && ptg->ptg_id.get() <= 0x7D )
    {
        SETBITS(ptg->ptg_id.get(),5,6,type);
    }
    else if(ptg->ptg_id.get() == 0x1918)
    {
        auto list = static_cast<XLS::PtgList*>(ptg.get());
        list->type_ = type - 1;
    }
}

const void StringPtgParser::parsePtgTypes(Rgce& rgce)
{
    PtgVector functionStack;
    for(auto i:rgce.sequence)
    {
        if(!i->ptg_id.is_initialized())
            continue;
        auto ptgId = i->ptg_id.get();
        
        auto untypedId = GETBITS(ptgId, 0, 4);
        if(ptgId > 21)
        {
            if(untypedId == 1)
            {
                auto funcPtr = dynamic_cast<PtgFunc*>(i.get());
                auto paramsNum = funcPtr->getParametersNum();
                auto refArgs = PosValArgs(funcPtr->getFuncIndex());
                for(auto j = paramsNum-1; j >= 0; j--)
                {
                    if(!functionStack.empty())
                    {
                        if(refArgs.size() > j && refArgs.at(j))
                        SetPtgType(functionStack.back(), 3);
                        functionStack.pop_back();
                    }
                }
                ///check and change fixed num of args
            }
            else if(untypedId == 2)
            {
                auto funcPtr = dynamic_cast<PtgFuncVar*>(i.get());
                auto paramsNum = funcPtr->getParamsNum();
                auto FuncIndex = funcPtr->getFuncIndex();
                std::vector<bool> refArgs;
                if(FuncIndex != 0x00FF)
                    refArgs = PosValArgs(FuncIndex);
                else if(funcPtr->getFutureFuncName() != L"")
                {
                    auto futureFuncName = funcPtr->getFutureFuncName();
                    XLS::FutureFunctionParser::GetFutureFunction(futureFuncName);
                    refArgs = XLS::FutureFunctionParser::GetArgumentList(futureFuncName);
                }
                for(auto j = paramsNum-1; j >= 0; j--)
                {
                    if(!functionStack.empty())
                    {
                        if(refArgs.size() > j && refArgs.at(j))
                        SetPtgType(functionStack.back(), 3);
                        functionStack.pop_back();
                    }
                }
            }
            functionStack.push_back(i);
        }
        else if(ptgId > 1 && ptgId < 15)
        {
            if(!functionStack.empty())
            {
                SetPtgType(functionStack.back(), 3);
                functionStack.pop_back();
            }
            if(!functionStack.empty())
            {
                SetPtgType(functionStack.back(), 3);
            }
        }
        else if(ptgId > 14 && ptgId < 18)
        {
            if(!functionStack.empty())
            {
                SetPtgType(functionStack.back(), 1);
                functionStack.pop_back();
            }
            if(!functionStack.empty())
            {
                SetPtgType(functionStack.back(), 1);
            }
        }
        else if(ptgId > 17 && ptgId < 21 && !functionStack.empty())
        {
           SetPtgType(functionStack.back(), 3);
        }
        else if(ptgId == 21)
        {
            continue;
        }
        else
        {
            functionStack.push_back(i);
        }

    }
}

std::vector<bool> StringPtgParser::PosValArgs(const unsigned int &index) const
{
	std::vector<bool> argVector;
	switch(index)
	{
		case 0x0001: case 0x0002: case 0x0003: case 0x000F: case 0x000B: 
        case 0x0010: case 0x0011: case 0x0012: case 0x0014: case 0x0015: case 0x0016: case 0x0017: case 0x0018:  case 0x001A: case 0x001C:
        case 0x0020: case 0x0021: case 0x0026: case 0x0036: 
        case 0x0040: case 0x0043: case 0x0044: case 0x0045: case 0x0047: case 0x0048: case 0x0049: case 0x004F:
        case 0x0051: case 0x0053: case 0x0054: case 0x0056: case 0x0057: case 0x0058: 
        case 0x0060: case 0x0062: case 0x0063: case 0x0064: case 0x006F:
        case 0x0070: case 0x0071: case 0x0072: case 0x0076: case 0x0079: case 0x007B:case 0x007D: case 0x007E:case 0x007F:
        case 0x0080: case 0x0081: case 0x0085: case 0x0086: case 0x0087: case 0x008C: case 0x008D:
        case 0x0096: case 0x0097: case 0x009D:
        case 0x00A2: case 0x00A3: case 0x00A4: case 0x00AC:
        case 0x00B1: case 0x00B3: case 0x00B4: case 0x00B5: case 0x00B8: case 0x00B9: case 0x00BA: case 0x00BE:
        case 0x00C0: case 0x00C6: case 0x00C8: case 0x00C9:
        case 0x00D6: case 0x00D7:
        case 0x00E0: case 0x00E2: case 0x00E5: case 0x00E6: case 0x00E7: case 0x00E8: case 0x00E9: case 0x00EA: case 0x00ED:
        case 0x00F4: case 0x00F8: case 0x00FB: case 0x00FE:
        case 0x0100: case 0x0101: case 0x0105: case 0x0106: case 0x0107: case 0x010F:
        case 0x0117: case 0x011B: case 0x011C:
        case 0x0126: case 0x0128: case 0x012A:
        case 0x0156: case 0x0157: case 0x0158:
        case 0x0160:
        case 0x0170: case 0x0171: case 0x0172: case 0x0173: case 0x0174: case 0x0175: case 0x0176: case 0x0177: case 0x0178: case 0x0179: case 0x017A:  case 0x017C:
        case 0x01DF:
        case 0x01E0:
			argVector.push_back(true);
			break;
		case 0x000D: case 0x001B: case 0x001E: case 0x0027: case 0x0030: case 0x0046: case 0x005B: case 0x005D:
        case 0x0061: case 0x0067: case 0x006B: case 0x006D: case 0x0073: case 0x0074: case 0x0075:
        case 0x0084: case 0x0088: case 0x0089: case 0x008A: case 0x008B: case 0x0093: case 0x0094: case 0x00A5: case 0x00AF:
        case 0x00B0: case 0x00B2: case 0x00BB: case 0x00BC: case 0x00C5: case 0x00CC:
        case 0x00D0: case 0x00D1: case 0x00D4: case 0x00D5: case 0x00EF: case 0x00FD: case 0x0102: case 0x0108: case 0x010C: 
        case 0x0112: case 0x0113: case 0x0114: case 0x011D: case 0x0120: case 0x012B: case 0x012F:
        case 0x0130: case 0x0131: case 0x0132: case 0x0133: case 0x0134: case 0x0136: case 0x0137: case 0x013A: case 0x013B:
        case 0x014C: case 0x0151: case 0x0153: case 0x015C: case 0x0161: case 0x0162: case 0x0165: case 0x0167:
            argVector.push_back(true);
            argVector.push_back(true);
            break;
        case 0x000E: case 0x001F: case 0x0041: case 0x0042: case 0x0052: case 0x007A: case 0x007C: case 0x008E: case 0x0091:
        case 0x009E: case 0x00A0: case 0x00CD: case 0x00CE: case 0x00D2: case 0x00D3: case 0x00DC: case 0x0103: case 0x0104:
        case 0x0109: case 0x010A: case 0x010B: case 0x0115: case 0x0116: case 0x0118: case 0x0119: case 0x011A: case 0x011F:
        case 0x0122: case 0x0123: case 0x0124: case 0x0127: case 0x0129: case 0x012C: case 0x012D:
        case 0x0135: case 0x014F: case 0x0154: case 0x015F: case 0x017E:
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            break;
        case 0x006E: case 0x0077: case 0x0078: case 0x008F: case 0x009F: case 0x00AB: case 0x00B6: case 0x00CF: case 0x00F2: case 0x00F3:
        case 0x0111: case 0x011E: case 0x0121: case 0x0125: case 0x012E: case 0x013C: case 0x013D: case 0x014E: case 0x0155: case 0x015E:
        case 0x0163: case 0x017F: case 0x01DD:
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            break;
        case 0x0038: case 0x0039: case 0x003A: case 0x003B: case 0x0090: case 0x009A: case 0x009B: case 0x009C: case 0x00DB:
        case 0x00F6: case 0x00F7: case 0x010E: case 0x0110: case 0x0164:
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
            break;
        case 0x001D:
            argVector.push_back(false);
            argVector.push_back(true);
            argVector.push_back(true);
            argVector.push_back(true);
        case 0x0159: case 0x0145: case 0x0146: case 0x0147: case 0x0148:
            argVector.push_back(false);
            argVector.push_back(true);
            break;
        case 0x0066: case 0x0065:
            argVector.push_back(true);
            argVector.push_back(false);
            argVector.push_back(false);
            argVector.push_back(true);
            break;
        case 0x01E1:
            for(auto i = 0; i < 127; i++)
            {
                argVector.push_back(false);
                argVector.push_back(true);
            }
		default:
			break;
	}
	return argVector;
}

} // namespace XLS

