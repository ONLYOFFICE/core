/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "FutureFunctionParser.h"
#include <map>

namespace XLS
{

    const bool FutureFunctionParser::GetFutureFunction(std::wstring& functionName)
    {
        std::wstring tempName = functionName;
        if(tempName.substr(0, 6) != L"_xlfn.")
        {
            tempName = L"_xlfn." + tempName;
        }
        auto functions = init();
        if(functions.FutureFunctions.find(tempName) != functions.FutureFunctions.end())
        {
            functionName = tempName;
            return true;
        }
        return false;
    }

    std::vector<bool> FutureFunctionParser::GetArgumentList( const std::wstring& functionName)
    {
        std::vector<bool> argVector;
        auto functions = init();
        auto findedFunc = functions.FutureFunctions.find(functionName);
        if(findedFunc != functions.FutureFunctions.end())
        {
            argVector = findedFunc->second;
        }
        return argVector;
    }

    FutureFunctionParser& FutureFunctionParser::init()
    {
        static FutureFunctionParser parser;
        return parser;
    }

    FutureFunctionParser::FutureFunctionParser()
    {
        std::vector<bool> tempVector;
        FutureFunctions.emplace (L"_xlfn.DAYS", std::vector<bool>{0,1,1});
        FutureFunctions.emplace(L"_xlfn.ACOT", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.ACOTH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.AGGREGATE", std::vector<bool>{0, 1, 1, 1, 0});
        FutureFunctions.emplace(L"_xlfn.ARABIC", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.BASE", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BETA.DIST", std::vector<bool>{0, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BETA.INV", std::vector<bool>{0, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BINOM.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BINOM.DIST.RANGE", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BINOM.INV", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BITAND", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BITLSHIFT", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BITOR", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BITRSHIFT", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BITXOR", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.BYCOL", std::vector<bool>{0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.BYROW", std::vector<bool>{0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.CEILING.MATH", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CEILING.PRECISE", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CHISQ.DIST", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CHISQ.DIST.RT", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CHISQ.INV", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CHISQ.INV.RT", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CHISQ.TEST", std::vector<bool>{0, 1, 1});
        tempVector = std::vector<bool>{0};
        for (auto i = 0; i<508; i++)
        {
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.CHOOSECOLS", tempVector);
        FutureFunctions.emplace(L"_xlfn.CHOOSEROWS", tempVector);
        FutureFunctions.emplace(L"_xlfn.COMBINA", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CONCAT", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.CONFIDENCE.NORM", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CONFIDENCE.T", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.COT", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.COTH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.COVARIANCE.P", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.COVARIANCE.S", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.CSC", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.CSCH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.DECIMAL", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.DROP", std::vector<bool>{0, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.ECMA.CEILING", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.ERF.PRECISE", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.ERFC.PRECISE", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.EXPAND", std::vector<bool>{0, 1, 1, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.EXPON.DIST", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.F.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.F.DIST.RT", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.F.INV", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.F.INV.RT", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FIELDVALUE", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FILTERXML", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FLOOR.MATH", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FLOOR.PRECISE", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FORMULATEXT", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.GAMMA", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.GAMMA.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.GAMMA.INV", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.GAMMALN.PRECISE", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.GAUSS", std::vector<bool>{0, 1});
        tempVector = std::vector<bool>{0};
        for (auto i = 0; i<254; i++)
        {
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.HSTACK", tempVector);

        FutureFunctions.emplace(L"_xlfn.HYPGEOM.DIST", std::vector<bool>{0, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.IFNA", std::vector<bool>{0, 1, 0});
        tempVector = std::vector<bool>{0, 1, 0};
        for (auto i = 0; i<126; i++)
        {
            tempVector.push_back(1);
            tempVector.push_back(0);
        }
        FutureFunctions.emplace(L"_xlfn.", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMCOSH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMCOT", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMCSC", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMCSCH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMSEC", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMSECH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMSINH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.IMTAN", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.ISFORMULA", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.ISO.CEILING", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.ISOMITTED", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.ISOWEEKNUM", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.EXPON.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.LOGNORM.INV", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.MAKEARRAY", std::vector<bool>{0, 0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.MAP", std::vector<bool>{0, 0});
        tempVector = std::vector<bool>{0, 0, 0, 1};
        for (auto i = 0; i<125; i++)
        {
            tempVector.push_back(0);
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.MAXIFS", tempVector);
        FutureFunctions.emplace(L"_xlfn.MINIFS", tempVector);
        tempVector = std::vector<bool>{0};
        for (auto i = 0; i<254; i++)
        {
            tempVector.push_back(0);
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.MODE.MULT", tempVector);
        FutureFunctions.emplace(L"_xlfn.MODE.SNGL", tempVector);

        FutureFunctions.emplace(L"_xlfn.MUNIT", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.NEGBINOM.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.NETWORKDAYS.INTL", std::vector<bool>{0, 0, 0, 1, 0});
        FutureFunctions.emplace(L"_xlfn.NORM.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.NORM.INV", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.NORM.S.DIST", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.NORM.S.INV", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.NUMBERVALUE", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PDURATION", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PERCENTILE.EXC", std::vector<bool>{0, 0, 1});
        FutureFunctions.emplace(L"_xlfn.PERCENTILE.INC", std::vector<bool>{0, 0, 1});
        FutureFunctions.emplace(L"_xlfn.PERCENTRANK.EXC", std::vector<bool>{0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PERCENTRANK.INC", std::vector<bool>{0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PERMUTATIONA", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PHI", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.POISSON.DIST", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.PYTHON_STR", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.PYTHON_TYPE", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.PYTHON_TYPENAME", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.QUARTILE.EXC", std::vector<bool>{0, 0, 1});
        FutureFunctions.emplace(L"_xlfn.QUARTILE.INC", std::vector<bool>{0, 0, 1});
        tempVector = std::vector<bool>{0, 1, 1};
        for (auto i = 0; i<126; i++)
        {
            tempVector.push_back(1);
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.QUERYSTRING", tempVector);
        FutureFunctions.emplace(L"_xlfn.RANDARRAY", std::vector<bool>{0, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.RANK.AVG", std::vector<bool>{0, 1, 0, 1});
        FutureFunctions.emplace(L"_xlfn.RANK.EQ", std::vector<bool>{0, 1, 0, 1});
        FutureFunctions.emplace(L"_xlfn.RRI", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.SEC", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.SECH", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.SEQUENCE", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.SHEET", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.SHEETS", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.SKEW.P", std::vector<bool>{0, 0});
        tempVector = std::vector<bool>{0, 1, 1, 1};
        for (auto i = 0; i<125; i++)
        {
            tempVector.push_back(1);
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.SORTBY", tempVector);
        FutureFunctions.emplace(L"_xlfn.STDEV.P", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.STDEV.S", std::vector<bool>{0, 0});
        FutureFunctions.emplace(L"_xlfn.SWITCH", std::vector<bool>{0, 1, 0, 0});
        FutureFunctions.emplace(L"_xlfn.T.DIST", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.T.DIST.2T", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.T.DIST.RT", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.T.INV", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.T.INV.2T", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.T.TEST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TAKE", std::vector<bool>{0, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TEXTAFTER", std::vector<bool>{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TEXTBEFORE", std::vector<bool>{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TEXTJOIN", std::vector<bool>{0, 0, 1, 0});
        FutureFunctions.emplace(L"_xlfn.TEXTSPLIT", std::vector<bool>{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TOCOL", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.TOROW", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.UNICHAR", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.UNICODE", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.UNIQUE", std::vector<bool>{0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.VAR.P", std::vector<bool>{0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.VAR.S", std::vector<bool>{0, 0, 0});
        tempVector = std::vector<bool>{0};
        for (auto i = 0; i<254; i++)
        {
            tempVector.push_back(1);
        }
        FutureFunctions.emplace(L"_xlfn.VSTACK", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.WEBSERVICE", std::vector<bool>{0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.WEIBULL.DIST", std::vector<bool>{0, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.WORKDAY.INTL", std::vector<bool>{0, 0, 1, 0});
        FutureFunctions.emplace(L"_xlfn.WRAPCOLS", std::vector<bool>{0, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.WRAPROWS", std::vector<bool>{0, 1, 1, 1, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.XLOOKUP", std::vector<bool>{0, 1, 0, 0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.XOR", std::vector<bool>{0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.Z.TEST", std::vector<bool>{0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FORECAST.ETS", std::vector<bool>{0, 1, 0, 0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.FORECAST.ETS.CONFINT", std::vector<bool>{0, 0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FORECAST.ETS.SEASONALITY", std::vector<bool>{0, 0, 0, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FORECAST.ETS.STAT", std::vector<bool>{0, 1, 0, 0, 1, 1, 1});
        FutureFunctions.emplace(L"_xlfn.FORECAST.LINEAR", std::vector<bool>{0, 1, 1, 1});

        FutureFunctions.emplace(L"_xlfn.PQSOURCE", std::vector<bool>{0, 1});
        FutureFunctions.emplace(L"_xlfn.LET", std::vector<bool>{0, 0, 0, 0});
        FutureFunctions.emplace(L"_xlfn.LAMBDA", std::vector<bool>{0, 0, 0, 0});

    }

} // namespace XLS
