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

//Generated code
#include "ChartSerialize.h"
#include "../../../DesktopEditor/common/StringExt.h"

namespace OOX{
	namespace Spreadsheet{

		bool FromXml_ST_PageSetupOrientation(std::wstring& val, ST_PageSetupOrientation& eOut)
		{
			bool bRes = true;
			if(_T("default")==val)
				eOut=st_pagesetuporientationDEFAULT;
			else if(_T("portrait")==val)
				eOut=st_pagesetuporientationPORTRAIT;
			else if(_T("landscape")==val)
				eOut=st_pagesetuporientationLANDSCAPE;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_PageSetupOrientation(ST_PageSetupOrientation& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_pagesetuporientationDEFAULT==val)
				sOut=_T("default");
			else if(st_pagesetuporientationPORTRAIT==val)
				sOut=_T("portrait");
			else if(st_pagesetuporientationLANDSCAPE==val)
				sOut=_T("landscape");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_DispBlanksAs(std::wstring& val, ST_DispBlanksAs& eOut)
		{
			bool bRes = true;
			if(_T("span")==val)
				eOut=st_dispblanksasSPAN;
			else if(_T("gap")==val)
				eOut=st_dispblanksasGAP;
			else if(_T("zero")==val)
				eOut=st_dispblanksasZERO;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_DispBlanksAs(ST_DispBlanksAs& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_dispblanksasSPAN==val)
				sOut=_T("span");
			else if(st_dispblanksasGAP==val)
				sOut=_T("gap");
			else if(st_dispblanksasZERO==val)
				sOut=_T("zero");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_LegendPos(std::wstring& val, ST_LegendPos& eOut)
		{
			bool bRes = true;
			if(_T("b")==val)
				eOut=st_legendposB;
			else if(_T("tr")==val)
				eOut=st_legendposTR;
			else if(_T("l")==val)
				eOut=st_legendposL;
			else if(_T("r")==val)
				eOut=st_legendposR;
			else if(_T("t")==val)
				eOut=st_legendposT;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_LegendPos(ST_LegendPos& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_legendposB==val)
				sOut=_T("b");
			else if(st_legendposTR==val)
				sOut=_T("tr");
			else if(st_legendposL==val)
				sOut=_T("l");
			else if(st_legendposR==val)
				sOut=_T("r");
			else if(st_legendposT==val)
				sOut=_T("t");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_LayoutTarget(std::wstring& val, ST_LayoutTarget& eOut)
		{
			bool bRes = true;
			if(_T("inner")==val)
				eOut=st_layouttargetINNER;
			else if(_T("outer")==val)
				eOut=st_layouttargetOUTER;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_LayoutTarget(ST_LayoutTarget& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_layouttargetINNER==val)
				sOut=_T("inner");
			else if(st_layouttargetOUTER==val)
				sOut=_T("outer");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_LayoutMode(std::wstring& val, ST_LayoutMode& eOut)
		{
			bool bRes = true;
			if(_T("edge")==val)
				eOut=st_layoutmodeEDGE;
			else if(_T("factor")==val)
				eOut=st_layoutmodeFACTOR;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_LayoutMode(ST_LayoutMode& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_layoutmodeEDGE==val)
				sOut=_T("edge");
			else if(st_layoutmodeFACTOR==val)
				sOut=_T("factor");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_Orientation(std::wstring& val, ST_Orientation& eOut)
		{
			bool bRes = true;
			if(_T("maxMin")==val)
				eOut=st_orientationMAXMIN;
			else if(_T("minMax")==val)
				eOut=st_orientationMINMAX;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_Orientation(ST_Orientation& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_orientationMAXMIN==val)
				sOut=_T("maxMin");
			else if(st_orientationMINMAX==val)
				sOut=_T("minMax");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_AxPos(std::wstring& val, ST_AxPos& eOut)
		{
			bool bRes = true;
			if(_T("b")==val)
				eOut=st_axposB;
			else if(_T("l")==val)
				eOut=st_axposL;
			else if(_T("r")==val)
				eOut=st_axposR;
			else if(_T("t")==val)
				eOut=st_axposT;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_AxPos(ST_AxPos& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_axposB==val)
				sOut=_T("b");
			else if(st_axposL==val)
				sOut=_T("l");
			else if(st_axposR==val)
				sOut=_T("r");
			else if(st_axposT==val)
				sOut=_T("t");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_TickMark(std::wstring& val, ST_TickMark& eOut)
		{
			bool bRes = true;
			if(_T("cross")==val)
				eOut=st_tickmarkCROSS;
			else if(_T("in")==val)
				eOut=st_tickmarkIN;
			else if(_T("none")==val)
				eOut=st_tickmarkNONE;
			else if(_T("out")==val)
				eOut=st_tickmarkOUT;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_TickMark(ST_TickMark& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_tickmarkCROSS==val)
				sOut=_T("cross");
			else if(st_tickmarkIN==val)
				sOut=_T("in");
			else if(st_tickmarkNONE==val)
				sOut=_T("none");
			else if(st_tickmarkOUT==val)
				sOut=_T("out");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_TickLblPos(std::wstring& val, ST_TickLblPos& eOut)
		{
			bool bRes = true;
			if(_T("high")==val)
				eOut=st_ticklblposHIGH;
			else if(_T("low")==val)
				eOut=st_ticklblposLOW;
			else if(_T("nextTo")==val)
				eOut=st_ticklblposNEXTTO;
			else if(_T("none")==val)
				eOut=st_ticklblposNONE;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_TickLblPos(ST_TickLblPos& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_ticklblposHIGH==val)
				sOut=_T("high");
			else if(st_ticklblposLOW==val)
				sOut=_T("low");
			else if(st_ticklblposNEXTTO==val)
				sOut=_T("nextTo");
			else if(st_ticklblposNONE==val)
				sOut=_T("none");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_Crosses(std::wstring& val, ST_Crosses& eOut)
		{
			bool bRes = true;
			if(_T("autoZero")==val)
				eOut=st_crossesAUTOZERO;
			else if(_T("max")==val)
				eOut=st_crossesMAX;
			else if(_T("min")==val)
				eOut=st_crossesMIN;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_Crosses(ST_Crosses& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_crossesAUTOZERO==val)
				sOut=_T("autoZero");
			else if(st_crossesMAX==val)
				sOut=_T("max");
			else if(st_crossesMIN==val)
				sOut=_T("min");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_TimeUnit(std::wstring& val, ST_TimeUnit& eOut)
		{
			bool bRes = true;
			if(_T("days")==val)
				eOut=st_timeunitDAYS;
			else if(_T("months")==val)
				eOut=st_timeunitMONTHS;
			else if(_T("years")==val)
				eOut=st_timeunitYEARS;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_TimeUnit(ST_TimeUnit& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_timeunitDAYS==val)
				sOut=_T("days");
			else if(st_timeunitMONTHS==val)
				sOut=_T("months");
			else if(st_timeunitYEARS==val)
				sOut=_T("years");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_LblAlgn(std::wstring& val, ST_LblAlgn& eOut)
		{
			bool bRes = true;
			if(_T("ctr")==val)
				eOut=st_lblalgnCTR;
			else if(_T("l")==val)
				eOut=st_lblalgnL;
			else if(_T("r")==val)
				eOut=st_lblalgnR;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_LblAlgn(ST_LblAlgn& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_lblalgnCTR==val)
				sOut=_T("ctr");
			else if(st_lblalgnL==val)
				sOut=_T("l");
			else if(st_lblalgnR==val)
				sOut=_T("r");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_BuiltInUnit(std::wstring& val, ST_BuiltInUnit& eOut)
		{
			bool bRes = true;
			if(_T("hundreds")==val)
				eOut=st_builtinunitHUNDREDS;
			else if(_T("thousands")==val)
				eOut=st_builtinunitTHOUSANDS;
			else if(_T("tenThousands")==val)
				eOut=st_builtinunitTENTHOUSANDS;
			else if(_T("hundredThousands")==val)
				eOut=st_builtinunitHUNDREDTHOUSANDS;
			else if(_T("millions")==val)
				eOut=st_builtinunitMILLIONS;
			else if(_T("tenMillions")==val)
				eOut=st_builtinunitTENMILLIONS;
			else if(_T("hundredMillions")==val)
				eOut=st_builtinunitHUNDREDMILLIONS;
			else if(_T("billions")==val)
				eOut=st_builtinunitBILLIONS;
			else if(_T("trillions")==val)
				eOut=st_builtinunitTRILLIONS;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_BuiltInUnit(ST_BuiltInUnit& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_builtinunitHUNDREDS==val)
				sOut=_T("hundreds");
			else if(st_builtinunitTHOUSANDS==val)
				sOut=_T("thousands");
			else if(st_builtinunitTENTHOUSANDS==val)
				sOut=_T("tenThousands");
			else if(st_builtinunitHUNDREDTHOUSANDS==val)
				sOut=_T("hundredThousands");
			else if(st_builtinunitMILLIONS==val)
				sOut=_T("millions");
			else if(st_builtinunitTENMILLIONS==val)
				sOut=_T("tenMillions");
			else if(st_builtinunitHUNDREDMILLIONS==val)
				sOut=_T("hundredMillions");
			else if(st_builtinunitBILLIONS==val)
				sOut=_T("billions");
			else if(st_builtinunitTRILLIONS==val)
				sOut=_T("trillions");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_CrossBetween(std::wstring& val, ST_CrossBetween& eOut)
		{
			bool bRes = true;
			if(_T("between")==val)
				eOut=st_crossbetweenBETWEEN;
			else if(_T("midCat")==val)
				eOut=st_crossbetweenMIDCAT;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_CrossBetween(ST_CrossBetween& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_crossbetweenBETWEEN==val)
				sOut=_T("between");
			else if(st_crossbetweenMIDCAT==val)
				sOut=_T("midCat");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_SizeRepresents(std::wstring& val, ST_SizeRepresents& eOut)
		{
			bool bRes = true;
			if(_T("area")==val)
				eOut=st_sizerepresentsAREA;
			else if(_T("w")==val)
				eOut=st_sizerepresentsW;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_SizeRepresents(ST_SizeRepresents& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_sizerepresentsAREA==val)
				sOut=_T("area");
			else if(st_sizerepresentsW==val)
				sOut=_T("w");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_MarkerStyle(std::wstring& val, ST_MarkerStyle& eOut)
		{
			bool bRes = true;
			if(_T("circle")==val)
				eOut=st_markerstyleCIRCLE;
			else if(_T("dash")==val)
				eOut=st_markerstyleDASH;
			else if(_T("diamond")==val)
				eOut=st_markerstyleDIAMOND;
			else if(_T("dot")==val)
				eOut=st_markerstyleDOT;
			else if(_T("none")==val)
				eOut=st_markerstyleNONE;
			else if(_T("picture")==val)
				eOut=st_markerstylePICTURE;
			else if(_T("plus")==val)
				eOut=st_markerstylePLUS;
			else if(_T("square")==val)
				eOut=st_markerstyleSQUARE;
			else if(_T("star")==val)
				eOut=st_markerstyleSTAR;
			else if(_T("triangle")==val)
				eOut=st_markerstyleTRIANGLE;
			else if(_T("x")==val)
				eOut=st_markerstyleX;
			else if(_T("auto")==val)
				eOut=st_markerstyleAUTO;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_MarkerStyle(ST_MarkerStyle& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_markerstyleCIRCLE==val)
				sOut=_T("circle");
			else if(st_markerstyleDASH==val)
				sOut=_T("dash");
			else if(st_markerstyleDIAMOND==val)
				sOut=_T("diamond");
			else if(st_markerstyleDOT==val)
				sOut=_T("dot");
			else if(st_markerstyleNONE==val)
				sOut=_T("none");
			else if(st_markerstylePICTURE==val)
				sOut=_T("picture");
			else if(st_markerstylePLUS==val)
				sOut=_T("plus");
			else if(st_markerstyleSQUARE==val)
				sOut=_T("square");
			else if(st_markerstyleSTAR==val)
				sOut=_T("star");
			else if(st_markerstyleTRIANGLE==val)
				sOut=_T("triangle");
			else if(st_markerstyleX==val)
				sOut=_T("x");
			else if(st_markerstyleAUTO==val)
				sOut=_T("auto");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_PictureFormat(std::wstring& val, ST_PictureFormat& eOut)
		{
			bool bRes = true;
			if(_T("stretch")==val)
				eOut=st_pictureformatSTRETCH;
			else if(_T("stack")==val)
				eOut=st_pictureformatSTACK;
			else if(_T("stackScale")==val)
				eOut=st_pictureformatSTACKSCALE;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_PictureFormat(ST_PictureFormat& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_pictureformatSTRETCH==val)
				sOut=_T("stretch");
			else if(st_pictureformatSTACK==val)
				sOut=_T("stack");
			else if(st_pictureformatSTACKSCALE==val)
				sOut=_T("stackScale");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_DLblPos(std::wstring& val, ST_DLblPos& eOut)
		{
			bool bRes = true;
			if(_T("bestFit")==val)
				eOut=st_dlblposBESTFIT;
			else if(_T("b")==val)
				eOut=st_dlblposB;
			else if(_T("ctr")==val)
				eOut=st_dlblposCTR;
			else if(_T("inBase")==val)
				eOut=st_dlblposINBASE;
			else if(_T("inEnd")==val)
				eOut=st_dlblposINEND;
			else if(_T("l")==val)
				eOut=st_dlblposL;
			else if(_T("outEnd")==val)
				eOut=st_dlblposOUTEND;
			else if(_T("r")==val)
				eOut=st_dlblposR;
			else if(_T("t")==val)
				eOut=st_dlblposT;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_DLblPos(ST_DLblPos& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_dlblposBESTFIT==val)
				sOut=_T("bestFit");
			else if(st_dlblposB==val)
				sOut=_T("b");
			else if(st_dlblposCTR==val)
				sOut=_T("ctr");
			else if(st_dlblposINBASE==val)
				sOut=_T("inBase");
			else if(st_dlblposINEND==val)
				sOut=_T("inEnd");
			else if(st_dlblposL==val)
				sOut=_T("l");
			else if(st_dlblposOUTEND==val)
				sOut=_T("outEnd");
			else if(st_dlblposR==val)
				sOut=_T("r");
			else if(st_dlblposT==val)
				sOut=_T("t");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_TrendlineType(std::wstring& val, ST_TrendlineType& eOut)
		{
			bool bRes = true;
			if(_T("exp")==val)
				eOut=st_trendlinetypeEXP;
			else if(_T("linear")==val)
				eOut=st_trendlinetypeLINEAR;
			else if(_T("log")==val)
				eOut=st_trendlinetypeLOG;
			else if(_T("movingAvg")==val)
				eOut=st_trendlinetypeMOVINGAVG;
			else if(_T("poly")==val)
				eOut=st_trendlinetypePOLY;
			else if(_T("power")==val)
				eOut=st_trendlinetypePOWER;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_TrendlineType(ST_TrendlineType& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_trendlinetypeEXP==val)
				sOut=_T("exp");
			else if(st_trendlinetypeLINEAR==val)
				sOut=_T("linear");
			else if(st_trendlinetypeLOG==val)
				sOut=_T("log");
			else if(st_trendlinetypeMOVINGAVG==val)
				sOut=_T("movingAvg");
			else if(st_trendlinetypePOLY==val)
				sOut=_T("poly");
			else if(st_trendlinetypePOWER==val)
				sOut=_T("power");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_ErrDir(std::wstring& val, ST_ErrDir& eOut)
		{
			bool bRes = true;
			if(_T("x")==val)
				eOut=st_errdirX;
			else if(_T("y")==val)
				eOut=st_errdirY;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_ErrDir(ST_ErrDir& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_errdirX==val)
				sOut=_T("x");
			else if(st_errdirY==val)
				sOut=_T("y");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_ErrBarType(std::wstring& val, ST_ErrBarType& eOut)
		{
			bool bRes = true;
			if(_T("both")==val)
				eOut=st_errbartypeBOTH;
			else if(_T("minus")==val)
				eOut=st_errbartypeMINUS;
			else if(_T("plus")==val)
				eOut=st_errbartypePLUS;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_ErrBarType(ST_ErrBarType& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_errbartypeBOTH==val)
				sOut=_T("both");
			else if(st_errbartypeMINUS==val)
				sOut=_T("minus");
			else if(st_errbartypePLUS==val)
				sOut=_T("plus");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_ErrValType(std::wstring& val, ST_ErrValType& eOut)
		{
			bool bRes = true;
			if(_T("cust")==val)
				eOut=st_errvaltypeCUST;
			else if(_T("fixedVal")==val)
				eOut=st_errvaltypeFIXEDVAL;
			else if(_T("percentage")==val)
				eOut=st_errvaltypePERCENTAGE;
			else if(_T("stdDev")==val)
				eOut=st_errvaltypeSTDDEV;
			else if(_T("stdErr")==val)
				eOut=st_errvaltypeSTDERR;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_ErrValType(ST_ErrValType& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_errvaltypeCUST==val)
				sOut=_T("cust");
			else if(st_errvaltypeFIXEDVAL==val)
				sOut=_T("fixedVal");
			else if(st_errvaltypePERCENTAGE==val)
				sOut=_T("percentage");
			else if(st_errvaltypeSTDDEV==val)
				sOut=_T("stdDev");
			else if(st_errvaltypeSTDERR==val)
				sOut=_T("stdErr");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_SplitType(std::wstring& val, ST_SplitType& eOut)
		{
			bool bRes = true;
			if(_T("auto")==val)
				eOut=st_splittypeAUTO;
			else if(_T("cust")==val)
				eOut=st_splittypeCUST;
			else if(_T("percent")==val)
				eOut=st_splittypePERCENT;
			else if(_T("pos")==val)
				eOut=st_splittypePOS;
			else if(_T("val")==val)
				eOut=st_splittypeVAL;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_SplitType(ST_SplitType& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_splittypeAUTO==val)
				sOut=_T("auto");
			else if(st_splittypeCUST==val)
				sOut=_T("cust");
			else if(st_splittypePERCENT==val)
				sOut=_T("percent");
			else if(st_splittypePOS==val)
				sOut=_T("pos");
			else if(st_splittypeVAL==val)
				sOut=_T("val");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_OfPieType(std::wstring& val, ST_OfPieType& eOut)
		{
			bool bRes = true;
			if(_T("pie")==val)
				eOut=st_ofpietypePIE;
			else if(_T("bar")==val)
				eOut=st_ofpietypeBAR;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_OfPieType(ST_OfPieType& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_ofpietypePIE==val)
				sOut=_T("pie");
			else if(st_ofpietypeBAR==val)
				sOut=_T("bar");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_BarDir(std::wstring& val, ST_BarDir& eOut)
		{
			bool bRes = true;
			if(_T("bar")==val)
				eOut=st_bardirBAR;
			else if(_T("col")==val)
				eOut=st_bardirCOL;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_BarDir(ST_BarDir& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_bardirBAR==val)
				sOut=_T("bar");
			else if(st_bardirCOL==val)
				sOut=_T("col");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_BarGrouping(std::wstring& val, ST_BarGrouping& eOut)
		{
			bool bRes = true;
			if(_T("percentStacked")==val)
				eOut=st_bargroupingPERCENTSTACKED;
			else if(_T("clustered")==val)
				eOut=st_bargroupingCLUSTERED;
			else if(_T("standard")==val)
				eOut=st_bargroupingSTANDARD;
			else if(_T("stacked")==val)
				eOut=st_bargroupingSTACKED;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_BarGrouping(ST_BarGrouping& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_bargroupingPERCENTSTACKED==val)
				sOut=_T("percentStacked");
			else if(st_bargroupingCLUSTERED==val)
				sOut=_T("clustered");
			else if(st_bargroupingSTANDARD==val)
				sOut=_T("standard");
			else if(st_bargroupingSTACKED==val)
				sOut=_T("stacked");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_Shape(std::wstring& val, ST_Shape& eOut)
		{
			bool bRes = true;
			if(_T("cone")==val)
				eOut=st_shapeCONE;
			else if(_T("coneToMax")==val)
				eOut=st_shapeCONETOMAX;
			else if(_T("box")==val)
				eOut=st_shapeBOX;
			else if(_T("cylinder")==val)
				eOut=st_shapeCYLINDER;
			else if(_T("pyramid")==val)
				eOut=st_shapePYRAMID;
			else if(_T("pyramidToMax")==val)
				eOut=st_shapePYRAMIDTOMAX;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_Shape(ST_Shape& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_shapeCONE==val)
				sOut=_T("cone");
			else if(st_shapeCONETOMAX==val)
				sOut=_T("coneToMax");
			else if(st_shapeBOX==val)
				sOut=_T("box");
			else if(st_shapeCYLINDER==val)
				sOut=_T("cylinder");
			else if(st_shapePYRAMID==val)
				sOut=_T("pyramid");
			else if(st_shapePYRAMIDTOMAX==val)
				sOut=_T("pyramidToMax");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_ScatterStyle(std::wstring& val, ST_ScatterStyle& eOut)
		{
			bool bRes = true;
			if(_T("none")==val)
				eOut=st_scatterstyleNONE;
			else if(_T("line")==val)
				eOut=st_scatterstyleLINE;
			else if(_T("lineMarker")==val)
				eOut=st_scatterstyleLINEMARKER;
			else if(_T("marker")==val)
				eOut=st_scatterstyleMARKER;
			else if(_T("smooth")==val)
				eOut=st_scatterstyleSMOOTH;
			else if(_T("smoothMarker")==val)
				eOut=st_scatterstyleSMOOTHMARKER;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_ScatterStyle(ST_ScatterStyle& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_scatterstyleNONE==val)
				sOut=_T("none");
			else if(st_scatterstyleLINE==val)
				sOut=_T("line");
			else if(st_scatterstyleLINEMARKER==val)
				sOut=_T("lineMarker");
			else if(st_scatterstyleMARKER==val)
				sOut=_T("marker");
			else if(st_scatterstyleSMOOTH==val)
				sOut=_T("smooth");
			else if(st_scatterstyleSMOOTHMARKER==val)
				sOut=_T("smoothMarker");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_RadarStyle(std::wstring& val, ST_RadarStyle& eOut)
		{
			bool bRes = true;
			if(_T("standard")==val)
				eOut=st_radarstyleSTANDARD;
			else if(_T("marker")==val)
				eOut=st_radarstyleMARKER;
			else if(_T("filled")==val)
				eOut=st_radarstyleFILLED;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_RadarStyle(ST_RadarStyle& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_radarstyleSTANDARD==val)
				sOut=_T("standard");
			else if(st_radarstyleMARKER==val)
				sOut=_T("marker");
			else if(st_radarstyleFILLED==val)
				sOut=_T("filled");
			else
				bRes=false;
			return bRes;
		}
		bool FromXml_ST_Grouping(std::wstring& val, ST_Grouping& eOut)
		{
			bool bRes = true;
			if(_T("percentStacked")==val)
				eOut=st_groupingPERCENTSTACKED;
			else if(_T("standard")==val)
				eOut=st_groupingSTANDARD;
			else if(_T("stacked")==val)
				eOut=st_groupingSTACKED;
			else
				bRes=false;
			return bRes;
		}
		bool ToXml_ST_Grouping(ST_Grouping& val, std::wstring& sOut)
		{
			bool bRes = true;
			if(st_groupingPERCENTSTACKED==val)
				sOut=_T("percentStacked");
			else if(st_groupingSTANDARD==val)
				sOut=_T("standard");
			else if(st_groupingSTACKED==val)
				sOut=_T("stacked");
			else
				bRes=false;
			return bRes;
		}
		CT_extLst::CT_extLst()
		{
		}
		CT_extLst::~CT_extLst()
		{
            for(size_t i = 0; i < m_ext.size(); ++i)
				delete m_ext[i];
            m_ext.clear();
		}
		void CT_extLst::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("ext") == sName)
				{
					CT_Extension* pNewElem = new CT_Extension;
					pNewElem->fromXML(oReader);
                    m_ext.push_back(pNewElem);
				}
			}
		}
		void CT_extLst::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_ext.size(); ++i)
			{
				CT_Extension* pElem = m_ext[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ext");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_extLst::getType(){return et_ct_extlst;}
		CT_ChartSpace::CT_ChartSpace()
		{
			m_date1904 = NULL;
			m_lang = NULL;
			m_roundedCorners = NULL;
			m_AlternateContent = NULL;
			m_style = NULL;
			m_pivotSource = NULL;
			m_protection = NULL;
			m_chart = NULL;
			m_externalData = NULL;
			m_printSettings = NULL;
			m_userShapes = NULL;
			m_extLst = NULL;
		}
		CT_ChartSpace::~CT_ChartSpace()
		{
			if(NULL != m_date1904)
				delete m_date1904;
			if(NULL != m_lang)
				delete m_lang;
			if(NULL != m_roundedCorners)
				delete m_roundedCorners;
			if(NULL != m_AlternateContent)
				delete m_AlternateContent;
			if(NULL != m_style)
				delete m_style;
			if(NULL != m_pivotSource)
				delete m_pivotSource;
			if(NULL != m_protection)
				delete m_protection;
			if(NULL != m_chart)
				delete m_chart;
			if(NULL != m_externalData)
				delete m_externalData;
			if(NULL != m_printSettings)
				delete m_printSettings;
			if(NULL != m_userShapes)
				delete m_userShapes;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ChartSpace::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("date1904") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_date1904 = pNewElem;
				}
				else if(_T("lang") == sName)
				{
					CT_TextLanguageID* pNewElem = new CT_TextLanguageID;
					pNewElem->fromXML(oReader);
					m_lang = pNewElem;
				}
				else if(_T("roundedCorners") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_roundedCorners = pNewElem;
				}
				else if(_T("AlternateContent") == sName)
				{
					AlternateContent* pNewElem = new AlternateContent;
					pNewElem->fromXML(oReader);
					m_AlternateContent = pNewElem;
				}
				else if(_T("style") == sName)
				{
					m_style = new CT_Style;
					m_style->fromXML(oReader);
				}
				else if(_T("clrMapOvr") == sName)
				{
                    m_oClrMapOvr = oReader;
				}
				else if(_T("pivotSource") == sName)
				{
					CT_PivotSource* pNewElem = new CT_PivotSource;
					pNewElem->fromXML(oReader);
					m_pivotSource = pNewElem;
				}
				else if(_T("protection") == sName)
				{
					CT_Protection* pNewElem = new CT_Protection;
					pNewElem->fromXML(oReader);
					m_protection = pNewElem;
				}
				else if(_T("chart") == sName)
				{
					CT_Chart* pNewElem = new CT_Chart;
					pNewElem->fromXML(oReader);
					m_chart = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("externalData") == sName)
				{
					CT_ExternalData* pNewElem = new CT_ExternalData;
					pNewElem->fromXML(oReader);
					m_externalData = pNewElem;
				}
				else if(_T("printSettings") == sName)
				{
					CT_PrintSettings* pNewElem = new CT_PrintSettings;
					pNewElem->fromXML(oReader);
					m_printSettings = pNewElem;
				}
				else if(_T("userShapes") == sName)
				{
					CT_RelId* pNewElem = new CT_RelId;
					pNewElem->fromXML(oReader);
					m_userShapes = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ChartSpace::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<" + sNodeName + L" \
xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:c14=\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\"");

			if (m_userShapes)
			{
				writer.WriteString(L" xmlns:c16r2=\"http://schemas.microsoft.com/office/drawing/2015/06/chart\"");
			}
			writer.WriteString(L">");
			if(NULL != m_date1904)
			{
                std::wstring sNodeName = _T("c:date1904");
                m_date1904->toXML(sNodeName, writer);
			}
			if(NULL != m_lang)
			{
                std::wstring sNodeName = _T("c:lang");
                m_lang->toXML(sNodeName, writer);
			}
			if(NULL != m_roundedCorners)
			{
                std::wstring sNodeName = _T("c:roundedCorners");
                m_roundedCorners->toXML(sNodeName, writer);
			}
			if(NULL != m_AlternateContent)
			{
                std::wstring sNodeName = _T("mc:AlternateContent");
                m_AlternateContent->toXML(sNodeName, writer);
			}
			if(NULL != m_style)
			{
                m_style->toXML(writer);
			}
			if(m_oClrMapOvr.IsInit())
			{
				writer.WriteString(m_oClrMapOvr->toXML());
			}
			if(NULL != m_pivotSource)
			{
                std::wstring sNodeName = _T("c:pivotSource");
                m_pivotSource->toXML(sNodeName, writer);
			}
			if(NULL != m_protection)
			{
                std::wstring sNodeName = _T("c:protection");
                m_protection->toXML(sNodeName, writer);
			}
			if(NULL != m_chart)
			{
                m_chart->toXML(writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_externalData)
			{
                std::wstring sNodeName = _T("c:externalData");
                m_externalData->toXML(sNodeName, writer);
			}
			if(NULL != m_printSettings)
			{
                std::wstring sNodeName = _T("c:printSettings");
                m_printSettings->toXML(sNodeName, writer);
			}
			if(NULL != m_userShapes)
			{
                std::wstring sNodeName = _T("c:userShapes");
                m_userShapes->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ChartSpace::getType(){return et_ct_ChartSpace;}
		CT_Boolean::CT_Boolean()
		{
			/*m_val = new bool;
			std::wstring s_m_val("True");
			{
			std::wstring sValLower = s_m_val.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_val = false;
			else
			*m_val = true;
			}*/
			m_val = NULL;
		}
		CT_Boolean::~CT_Boolean()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Boolean::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Boolean::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sVal;
				if(*m_val)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"val", sVal);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Boolean::getType(){return et_ct_boolean;}
		void CT_Boolean::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_RelId::CT_RelId()
		{
			m_id = NULL;
		}
		CT_RelId::~CT_RelId()
		{
			if(NULL != m_id)
				delete m_id;
		}
		void CT_RelId::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_RelId::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_id)
			{
				WritingStringAttrEncodeXmlString(L"r:id", *m_id);
			}
			writer.WriteString(L" xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:c14=\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\"");
			writer.WriteString(L"/>");
		}
		EElementType CT_RelId::getType(){return et_ct_relid;}
		void CT_RelId::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("id") == wsName)
				{
					//todo
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_id = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_PageSetup::CT_PageSetup()
		{
			/*m_paperSize = new unsigned long;
			std::wstring s_m_paperSize("1");
			{
            *m_paperSize = XmlUtils::GetInteger(s_m_paperSize);
			}*/
			m_paperSize = NULL;
			m_paperHeight = NULL;
			m_paperWidth = NULL;
			/*m_firstPageNumber = new unsigned long;
			std::wstring s_m_firstPageNumber("1");
			{
            *m_firstPageNumber = XmlUtils::GetInteger(s_m_firstPageNumber);
			}*/
			m_firstPageNumber = NULL;
			/*m_orientation = new ST_PageSetupOrientation;
			*m_orientation = st_pagesetuporientationDEFAULT;*/
			m_orientation = NULL;
			/*m_blackAndWhite = new bool;
			std::wstring s_m_blackAndWhite("False");
			{
			std::wstring sValLower = s_m_blackAndWhite.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_blackAndWhite = false;
			else
			*m_blackAndWhite = true;
			}*/
			m_blackAndWhite = NULL;
			/*m_draft = new bool;
			std::wstring s_m_draft("False");
			{
			std::wstring sValLower = s_m_draft.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_draft = false;
			else
			*m_draft = true;
			}*/
			m_draft = NULL;
			/*m_useFirstPageNumber = new bool;
			std::wstring s_m_useFirstPageNumber("False");
			{
			std::wstring sValLower = s_m_useFirstPageNumber.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_useFirstPageNumber = false;
			else
			*m_useFirstPageNumber = true;
			}*/
			m_useFirstPageNumber = NULL;
			/*m_horizontalDpi = new long;
			std::wstring s_m_horizontalDpi("600");
			{
            *m_horizontalDpi = XmlUtils::GetInteger(s_m_horizontalDpi);
			}*/
			m_horizontalDpi = NULL;
			/*m_verticalDpi = new long;
			std::wstring s_m_verticalDpi("600");
			{
            *m_verticalDpi = XmlUtils::GetInteger(s_m_verticalDpi);
			}*/
			m_verticalDpi = NULL;
			/*m_copies = new unsigned long;
			std::wstring s_m_copies("1");
			{
            *m_copies = XmlUtils::GetInteger(s_m_copies);
			}*/
			m_copies = NULL;
		}
		CT_PageSetup::~CT_PageSetup()
		{
			if(NULL != m_paperSize)
				delete m_paperSize;
			if(NULL != m_paperHeight)
				delete m_paperHeight;
			if(NULL != m_paperWidth)
				delete m_paperWidth;
			if(NULL != m_firstPageNumber)
				delete m_firstPageNumber;
			if(NULL != m_orientation)
				delete m_orientation;
			if(NULL != m_blackAndWhite)
				delete m_blackAndWhite;
			if(NULL != m_draft)
				delete m_draft;
			if(NULL != m_useFirstPageNumber)
				delete m_useFirstPageNumber;
			if(NULL != m_horizontalDpi)
				delete m_horizontalDpi;
			if(NULL != m_verticalDpi)
				delete m_verticalDpi;
			if(NULL != m_copies)
				delete m_copies;
		}
		void CT_PageSetup::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_PageSetup::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_paperSize)
			{
				WritingStringAttrInt(L"paperSize", *m_paperSize);
			}
			if(NULL != m_paperHeight)
			{
				WritingStringAttrEncodeXmlString(L"paperHeight", *m_paperHeight);
			}
			if(NULL != m_paperWidth)
			{
				WritingStringAttrEncodeXmlString(L"paperWidth", *m_paperWidth);
			}
			if(NULL != m_firstPageNumber)
			{
				WritingStringAttrInt(L"firstPageNumber", *m_firstPageNumber);
			}
			if(NULL != m_orientation)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_PageSetupOrientation(*m_orientation, sEnumVal))
				{
					WritingStringAttrString(L"orientation", sEnumVal);
				}
			}
			if(NULL != m_blackAndWhite)
			{
				std::wstring sVal;
				if(*m_blackAndWhite)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"blackAndWhite", sVal);
			}
			if(NULL != m_draft)
			{
				std::wstring sVal;
				if(*m_draft)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"draft", sVal);
			}
			if(NULL != m_useFirstPageNumber)
			{
				std::wstring sVal;
				if(*m_useFirstPageNumber)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"useFirstPageNumber", sVal);
			}
			if(NULL != m_horizontalDpi)
			{
				WritingStringAttrInt(L"horizontalDpi", *m_horizontalDpi);
			}
			if(NULL != m_verticalDpi)
			{
				WritingStringAttrInt(L"verticalDpi", *m_verticalDpi);
			}
			if(NULL != m_copies)
			{
				WritingStringAttrInt(L"copies", *m_copies);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PageSetup::getType(){return et_ct_pagesetup;}
		void CT_PageSetup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("paperSize") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_paperSize = pNewElem;
				}
				else if(_T("paperHeight") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_paperHeight = pNewElem;
				}
				else if(_T("paperWidth") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_paperWidth = pNewElem;
				}
				else if(_T("firstPageNumber") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_firstPageNumber = pNewElem;
				}
				else if(_T("orientation") == wsName)
				{
					ST_PageSetupOrientation* pNewElem = new ST_PageSetupOrientation;
					ST_PageSetupOrientation eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_PageSetupOrientation(sNodeName, eVal))
						*pNewElem = eVal;
					m_orientation = pNewElem;
				}
				else if(_T("blackAndWhite") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_blackAndWhite = pNewElem;
				}
				else if(_T("draft") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_draft = pNewElem;
				}
				else if(_T("useFirstPageNumber") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_useFirstPageNumber = pNewElem;
				}
				else if(_T("horizontalDpi") == wsName)
				{
					long* pNewElem = new long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_horizontalDpi = pNewElem;
				}
				else if(_T("verticalDpi") == wsName)
				{
					long* pNewElem = new long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_verticalDpi = pNewElem;
				}
				else if(_T("copies") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_copies = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_PageMargins::CT_PageMargins()
		{
			m_l = NULL;
			m_r = NULL;
			m_t = NULL;
			m_b = NULL;
			m_header = NULL;
			m_footer = NULL;
		}
		CT_PageMargins::~CT_PageMargins()
		{
			if(NULL != m_l)
				delete m_l;
			if(NULL != m_r)
				delete m_r;
			if(NULL != m_t)
				delete m_t;
			if(NULL != m_b)
				delete m_b;
			if(NULL != m_header)
				delete m_header;
			if(NULL != m_footer)
				delete m_footer;
		}
		void CT_PageMargins::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_PageMargins::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_l)
			{
				WritingStringAttrDouble(L"l", *m_l);
			}
			if(NULL != m_r)
			{
				WritingStringAttrDouble(L"r", *m_r);
			}
			if(NULL != m_t)
			{
				WritingStringAttrDouble(L"t", *m_t);
			}
			if(NULL != m_b)
			{
				WritingStringAttrDouble(L"b", *m_b);
			}
			if(NULL != m_header)
			{
				WritingStringAttrDouble(L"header", *m_header);
			}
			if(NULL != m_footer)
			{
				WritingStringAttrDouble(L"footer", *m_footer);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PageMargins::getType(){return et_ct_pagemargins;}
		void CT_PageMargins::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("l") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_l = pNewElem;
				}
				else if(_T("r") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_r = pNewElem;
				}
				else if(_T("t") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_t = pNewElem;
				}
				else if(_T("b") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_b = pNewElem;
				}
				else if(_T("header") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_header = pNewElem;
				}
				else if(_T("footer") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_footer = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_HeaderFooter::CT_HeaderFooter()
		{
			m_oddHeader = NULL;
			m_oddFooter = NULL;
			m_evenHeader = NULL;
			m_evenFooter = NULL;
			m_firstHeader = NULL;
			m_firstFooter = NULL;
			/*m_alignWithMargins = new bool;
			std::wstring s_m_alignWithMargins("True");
			{
			std::wstring sValLower = s_m_alignWithMargins.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_alignWithMargins = false;
			else
			*m_alignWithMargins = true;
			}*/
			m_alignWithMargins = NULL;
			/*m_differentOddEven = new bool;
			std::wstring s_m_differentOddEven("False");
			{
			std::wstring sValLower = s_m_differentOddEven.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_differentOddEven = false;
			else
			*m_differentOddEven = true;
			}*/
			m_differentOddEven = NULL;
			/*m_differentFirst = new bool;
			std::wstring s_m_differentFirst("False");
			{
			std::wstring sValLower = s_m_differentFirst.MakeLower();
			if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
			*m_differentFirst = false;
			else
			*m_differentFirst = true;
			}*/
			m_differentFirst = NULL;
		}
		CT_HeaderFooter::~CT_HeaderFooter()
		{
			if(NULL != m_oddHeader)
				delete m_oddHeader;
			if(NULL != m_oddFooter)
				delete m_oddFooter;
			if(NULL != m_evenHeader)
				delete m_evenHeader;
			if(NULL != m_evenFooter)
				delete m_evenFooter;
			if(NULL != m_firstHeader)
				delete m_firstHeader;
			if(NULL != m_firstFooter)
				delete m_firstFooter;
			if(NULL != m_alignWithMargins)
				delete m_alignWithMargins;
			if(NULL != m_differentOddEven)
				delete m_differentOddEven;
			if(NULL != m_differentFirst)
				delete m_differentFirst;
		}
		void CT_HeaderFooter::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("oddHeader") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_oddHeader = pNewElem;
					}
					else if(_T("oddFooter") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_oddFooter = pNewElem;
					}
					else if(_T("evenHeader") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_evenHeader = pNewElem;
					}
					else if(_T("evenFooter") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_evenFooter = pNewElem;
					}
					else if(_T("firstHeader") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_firstHeader = pNewElem;
					}
					else if(_T("firstFooter") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_firstFooter = pNewElem;
					}
				}
		}
		void CT_HeaderFooter::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_alignWithMargins)
			{
				std::wstring sVal;
				if(*m_alignWithMargins)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"alignWithMargins", sVal);
			}
			if(NULL != m_differentOddEven)
			{
				std::wstring sVal;
				if(*m_differentOddEven)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"differentOddEven", sVal);
			}
			if(NULL != m_differentFirst)
			{
				std::wstring sVal;
				if(*m_differentFirst)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"differentFirst", sVal);
			}
			writer.WriteString(L">");
			if(NULL != m_oddHeader)
			{
				if (std::wstring::npos != m_oddHeader->find(' ') || std::wstring::npos != m_oddHeader->find('\n'))
					writer.WriteString(L"<c:oddHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:oddHeader>");
				writer.WriteEncodeXmlString(*m_oddHeader);
				writer.WriteString(L"</c:oddHeader>");
			}
			if(NULL != m_oddFooter)
			{
				if (std::wstring::npos != m_oddFooter->find(' ') || std::wstring::npos != m_oddFooter->find('\n'))
					writer.WriteString(L"<c:oddFooter xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:oddFooter>");
				writer.WriteEncodeXmlString(*m_oddFooter);
				writer.WriteString(L"</c:oddFooter>");
			}
			if(NULL != m_evenHeader)
			{
				if (std::wstring::npos != m_evenHeader->find(' ') || std::wstring::npos != m_evenHeader->find('\n'))
					writer.WriteString(L"<c:evenHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:evenHeader>");
				writer.WriteEncodeXmlString(*m_evenHeader);
				writer.WriteString(L"</c:evenHeader>");
			}
			if(NULL != m_evenFooter)
			{
				if (std::wstring::npos != m_evenFooter->find(' ') || std::wstring::npos != m_evenFooter->find('\n'))
					writer.WriteString(L"<c:evenFooter xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:evenFooter>");
				writer.WriteEncodeXmlString(*m_evenFooter);
				writer.WriteString(L"</c:evenFooter>");
			}
			if(NULL != m_firstHeader)
			{
				if (std::wstring::npos != m_firstHeader->find(' ') || std::wstring::npos != m_firstHeader->find('\n'))
					writer.WriteString(L"<c:firstHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:firstHeader>");
				writer.WriteEncodeXmlString(*m_firstHeader);
				writer.WriteString(L"</c:firstHeader>");
			}
			if(NULL != m_firstFooter)
			{
				if (std::wstring::npos != m_firstFooter->find(' ') || std::wstring::npos != m_firstFooter->find('\n'))
					writer.WriteString(L"<c:firstFooter xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:firstFooter>");
				writer.WriteEncodeXmlString(*m_firstFooter);
				writer.WriteString(L"</c:firstFooter>");
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_HeaderFooter::getType(){return et_ct_headerfooter;}
		void CT_HeaderFooter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("alignWithMargins") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_alignWithMargins = pNewElem;
				}
				else if(_T("differentOddEven") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_differentOddEven = pNewElem;
				}
				else if(_T("differentFirst") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_differentFirst = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_PrintSettings::CT_PrintSettings()
		{
			m_headerFooter = NULL;
			m_pageMargins = NULL;
			m_pageSetup = NULL;
		}
		CT_PrintSettings::~CT_PrintSettings()
		{
			if(NULL != m_headerFooter)
				delete m_headerFooter;
			if(NULL != m_pageMargins)
				delete m_pageMargins;
			if(NULL != m_pageSetup)
				delete m_pageSetup;
		}
		void CT_PrintSettings::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("headerFooter") == sName)
				{
					CT_HeaderFooter* pNewElem = new CT_HeaderFooter;
					pNewElem->fromXML(oReader);
					m_headerFooter = pNewElem;
				}
				else if(_T("pageMargins") == sName)
				{
					CT_PageMargins* pNewElem = new CT_PageMargins;
					pNewElem->fromXML(oReader);
					m_pageMargins = pNewElem;
				}
				else if(_T("pageSetup") == sName)
				{
					CT_PageSetup* pNewElem = new CT_PageSetup;
					pNewElem->fromXML(oReader);
					m_pageSetup = pNewElem;
				}
			}
		}
		void CT_PrintSettings::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_headerFooter)
			{
                std::wstring sNodeName = _T("c:headerFooter");
                m_headerFooter->toXML(sNodeName, writer);
			}
			if(NULL != m_pageMargins)
			{
                std::wstring sNodeName = _T("c:pageMargins");
                m_pageMargins->toXML(sNodeName, writer);
			}
			if(NULL != m_pageSetup)
			{
                std::wstring sNodeName = _T("c:pageSetup");
                m_pageSetup->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PrintSettings::getType(){return et_ct_printsettings;}
		CT_ExternalData::CT_ExternalData()
		{
			m_autoUpdate = NULL;
			m_id = NULL;
		}
		CT_ExternalData::~CT_ExternalData()
		{
			if(NULL != m_autoUpdate)
				delete m_autoUpdate;
			if(NULL != m_id)
				delete m_id;
		}
		void CT_ExternalData::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("autoUpdate") == sName)
					{
						CT_Boolean* pNewElem = new CT_Boolean;
						pNewElem->fromXML(oReader);
						m_autoUpdate = pNewElem;
					}
				}
		}
		void CT_ExternalData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_id)
			{
				WritingStringAttrEncodeXmlString(L"r:id", *m_id);
			}
			writer.WriteString(L">");
			if(NULL != m_autoUpdate)
			{
                std::wstring sNodeName = _T("c:autoUpdate");
                m_autoUpdate->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ExternalData::getType(){return et_ct_externaldata;}
		void CT_ExternalData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("id") == wsName)
				{
					//todo
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_id = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DispBlanksAs::CT_DispBlanksAs()
		{
			/*m_val = new ST_DispBlanksAs;
			*m_val = st_dispblanksasZERO;*/
			m_val = NULL;
		}
		CT_DispBlanksAs::~CT_DispBlanksAs()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_DispBlanksAs::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_DispBlanksAs::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_DispBlanksAs(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_DispBlanksAs::getType(){return et_ct_dispblanksas;}
		void CT_DispBlanksAs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_DispBlanksAs* pNewElem = new ST_DispBlanksAs;
					ST_DispBlanksAs eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_DispBlanksAs(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_LegendEntry::CT_LegendEntry()
		{
			m_idx = NULL;
			m_delete = NULL;
			m_extLst = NULL;
		}
		CT_LegendEntry::~CT_LegendEntry()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_delete)
				delete m_delete;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_LegendEntry::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_delete = pNewElem;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_LegendEntry::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_delete)
			{
                std::wstring sNodeName = _T("c:delete");
                m_delete->toXML(sNodeName, writer);
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LegendEntry::getType(){return et_ct_LegendEntry;}
		CT_UnsignedInt::CT_UnsignedInt()
		{
			m_val = NULL;
		}
		CT_UnsignedInt::~CT_UnsignedInt()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_UnsignedInt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
        void CT_UnsignedInt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_UnsignedInt::getType(){return et_ct_unsignedint;}
		void CT_UnsignedInt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Extension::CT_Extension()
		{
			m_Any = NULL;
			m_uri = NULL;
		}
		CT_Extension::~CT_Extension()
		{
			if(NULL != m_Any)
				delete m_Any;
			if(NULL != m_uri)
				delete m_uri;
		}
		void CT_Extension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("Any") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_Any = pNewElem;
					}
				}
		}
		void CT_Extension::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_uri)
			{
				WritingStringAttrEncodeXmlString(L"uri", *m_uri);
			}
			writer.WriteString(L">");
			if(NULL != m_Any)
			{
				if (std::wstring::npos != m_Any->find(' ') || std::wstring::npos != m_Any->find('\n'))
					writer.WriteString(L"<c:Any xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:Any>");
				writer.WriteEncodeXmlString(*m_Any);
				writer.WriteString(L"</c:Any>");
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Extension::getType(){return et_ct_extension;}
		void CT_Extension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("uri") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_uri = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_LegendPos::CT_LegendPos()
		{
			/*m_val = new ST_LegendPos;
			*m_val = st_legendposR;*/
			m_val = NULL;
		}
		CT_LegendPos::~CT_LegendPos()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LegendPos::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LegendPos::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_LegendPos(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LegendPos::getType(){return et_ct_LegendPos;}
		void CT_LegendPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_LegendPos* pNewElem = new ST_LegendPos;
					ST_LegendPos eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_LegendPos(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Legend::CT_Legend()
		{
			m_legendPos = NULL;
			m_layout = NULL;
			m_overlay = NULL;
			m_extLst = NULL;
		}
		CT_Legend::~CT_Legend()
		{
			if(NULL != m_legendPos)
				delete m_legendPos;
            for(size_t i = 0; i < m_legendEntry.size(); ++i)
				delete m_legendEntry[i];
            m_legendEntry.clear();
			if(NULL != m_layout)
				delete m_layout;
			if(NULL != m_overlay)
				delete m_overlay;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Legend::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("legendPos") == sName)
				{
					CT_LegendPos* pNewElem = new CT_LegendPos;
					pNewElem->fromXML(oReader);
					m_legendPos = pNewElem;
				}
				else if(_T("legendEntry") == sName)
				{
					CT_LegendEntry* pNewElem = new CT_LegendEntry;
					pNewElem->fromXML(oReader);
                    m_legendEntry.push_back(pNewElem);
				}
				else if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if(_T("overlay") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_overlay = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Legend::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_legendPos)
			{
                std::wstring sNodeName = _T("c:legendPos");
                m_legendPos->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_legendEntry.size(); ++i)
			{
				CT_LegendEntry* pElem = m_legendEntry[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:legendEntry");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_layout)
			{
                std::wstring sNodeName = _T("c:layout");
                m_layout->toXML(sNodeName, writer);
			}
			if(NULL != m_overlay)
			{
                std::wstring sNodeName = _T("c:overlay");
                m_overlay->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Legend::getType(){return et_ct_Legend;}
		CT_Layout::CT_Layout()
		{
			m_manualLayout = NULL;
			m_extLst = NULL;
		}
		CT_Layout::~CT_Layout()
		{
			if(NULL != m_manualLayout)
				delete m_manualLayout;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Layout::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("manualLayout") == sName)
				{
					CT_ManualLayout* pNewElem = new CT_ManualLayout;
					pNewElem->fromXML(oReader);
					m_manualLayout = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Layout::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_manualLayout)
			{
                std::wstring sNodeName = _T("c:manualLayout");
                m_manualLayout->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Layout::getType(){return et_ct_layout;}
		CT_ManualLayout::CT_ManualLayout()
		{
			m_layoutTarget = NULL;
			m_xMode = NULL;
			m_yMode = NULL;
			m_wMode = NULL;
			m_hMode = NULL;
			m_x = NULL;
			m_y = NULL;
			m_w = NULL;
			m_h = NULL;
			m_extLst = NULL;
		}
		CT_ManualLayout::~CT_ManualLayout()
		{
			if(NULL != m_layoutTarget)
				delete m_layoutTarget;
			if(NULL != m_xMode)
				delete m_xMode;
			if(NULL != m_yMode)
				delete m_yMode;
			if(NULL != m_wMode)
				delete m_wMode;
			if(NULL != m_hMode)
				delete m_hMode;
			if(NULL != m_x)
				delete m_x;
			if(NULL != m_y)
				delete m_y;
			if(NULL != m_w)
				delete m_w;
			if(NULL != m_h)
				delete m_h;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ManualLayout::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("layoutTarget") == sName)
				{
					CT_LayoutTarget* pNewElem = new CT_LayoutTarget;
					pNewElem->fromXML(oReader);
					m_layoutTarget = pNewElem;
				}
				else if(_T("xMode") == sName)
				{
					CT_LayoutMode* pNewElem = new CT_LayoutMode;
					pNewElem->fromXML(oReader);
					m_xMode = pNewElem;
				}
				else if(_T("yMode") == sName)
				{
					CT_LayoutMode* pNewElem = new CT_LayoutMode;
					pNewElem->fromXML(oReader);
					m_yMode = pNewElem;
				}
				else if(_T("wMode") == sName)
				{
					CT_LayoutMode* pNewElem = new CT_LayoutMode;
					pNewElem->fromXML(oReader);
					m_wMode = pNewElem;
				}
				else if(_T("hMode") == sName)
				{
					CT_LayoutMode* pNewElem = new CT_LayoutMode;
					pNewElem->fromXML(oReader);
					m_hMode = pNewElem;
				}
				else if(_T("x") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_x = pNewElem;
				}
				else if(_T("y") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_y = pNewElem;
				}
				else if(_T("w") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_w = pNewElem;
				}
				else if(_T("h") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_h = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ManualLayout::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_layoutTarget)
			{
                std::wstring sNodeName = _T("c:layoutTarget");
                m_layoutTarget->toXML(sNodeName, writer);
			}
			if(NULL != m_xMode)
			{
                std::wstring sNodeName = _T("c:xMode");
                m_xMode->toXML(sNodeName, writer);
			}
			if(NULL != m_yMode)
			{
                std::wstring sNodeName = _T("c:yMode");
                m_yMode->toXML(sNodeName, writer);
			}
			if(NULL != m_wMode)
			{
                std::wstring sNodeName = _T("c:wMode");
                m_wMode->toXML(sNodeName, writer);
			}
			if(NULL != m_hMode)
			{
                std::wstring sNodeName = _T("c:hMode");
                m_hMode->toXML(sNodeName, writer);
			}
			if(NULL != m_x)
			{
                std::wstring sNodeName = _T("c:x");
                m_x->toXML(sNodeName, writer);
			}
			if(NULL != m_y)
			{
                std::wstring sNodeName = _T("c:y");
                m_y->toXML(sNodeName, writer);
			}
			if(NULL != m_w)
			{
                std::wstring sNodeName = _T("c:w");
                m_w->toXML(sNodeName, writer);
			}
			if(NULL != m_h)
			{
                std::wstring sNodeName = _T("c:h");
                m_h->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ManualLayout::getType(){return et_ct_manuallayout;}
		CT_LayoutTarget::CT_LayoutTarget()
		{
			/*m_val = new ST_LayoutTarget;
			*m_val = st_layouttargetOUTER;*/
			m_val = NULL;
		}
		CT_LayoutTarget::~CT_LayoutTarget()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LayoutTarget::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LayoutTarget::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_LayoutTarget(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LayoutTarget::getType(){return et_ct_layouttarget;}
		void CT_LayoutTarget::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_LayoutTarget* pNewElem = new ST_LayoutTarget;
					ST_LayoutTarget eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_LayoutTarget(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_LayoutMode::CT_LayoutMode()
		{
			/*m_val = new ST_LayoutMode;
			*m_val = st_layoutmodeFACTOR;*/
			m_val = NULL;
		}
		CT_LayoutMode::~CT_LayoutMode()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LayoutMode::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LayoutMode::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_LayoutMode(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LayoutMode::getType(){return et_ct_layoutmode;}
		void CT_LayoutMode::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_LayoutMode* pNewElem = new ST_LayoutMode;
					ST_LayoutMode eVal;

                    std::wstring sNodeName = oReader.GetText();

                    if(FromXml_ST_LayoutMode(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Double::CT_Double()
		{
			m_val = NULL;
		}
		CT_Double::~CT_Double()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Double::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Double::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrDouble(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Double::getType(){return et_ct_double;}
		void CT_Double::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText(); // Yp2bbamFbt1iHeX3WM0_.pptx - value = "-"
                    *pNewElem = 0;
                    try
                    {
                        *pNewElem = std::stod(sVal.c_str());
                    }
                    catch(...)
                    {
                    }

					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DTable::CT_DTable()
		{
			m_showHorzBorder = NULL;
			m_showVertBorder = NULL;
			m_showOutline = NULL;
			m_showKeys = NULL;
			m_extLst = NULL;
		}
		CT_DTable::~CT_DTable()
		{
			if(NULL != m_showHorzBorder)
				delete m_showHorzBorder;
			if(NULL != m_showVertBorder)
				delete m_showVertBorder;
			if(NULL != m_showOutline)
				delete m_showOutline;
			if(NULL != m_showKeys)
				delete m_showKeys;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DTable::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("showHorzBorder") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showHorzBorder = pNewElem;
				}
				else if(_T("showVertBorder") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showVertBorder = pNewElem;
				}
				else if(_T("showOutline") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showOutline = pNewElem;
				}
				else if(_T("showKeys") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showKeys = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DTable::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_showHorzBorder)
			{
                std::wstring sNodeName = _T("c:showHorzBorder");
                m_showHorzBorder->toXML(sNodeName, writer);
			}
			if(NULL != m_showVertBorder)
			{
                std::wstring sNodeName = _T("c:showVertBorder");
                m_showVertBorder->toXML(sNodeName, writer);
			}
			if(NULL != m_showOutline)
			{
                std::wstring sNodeName = _T("c:showOutline");
                m_showOutline->toXML(sNodeName, writer);
			}
			if(NULL != m_showKeys)
			{
                std::wstring sNodeName = _T("c:showKeys");
                m_showKeys->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DTable::getType(){return et_ct_dtable;}
		CT_SerAx::CT_SerAx()
		{
			m_axId = NULL;
			m_scaling = NULL;
			m_delete = NULL;
			m_axPos = NULL;
			m_majorGridlines = NULL;
			m_minorGridlines = NULL;
			m_title = NULL;
			m_numFmt = NULL;
			m_majorTickMark = NULL;
			m_minorTickMark = NULL;
			m_tickLblPos = NULL;
			m_crossAx = NULL;
			m_crosses = NULL;
			m_crossesAt = NULL;
			m_tickLblSkip = NULL;
			m_tickMarkSkip = NULL;
			m_extLst = NULL;
		}
		CT_SerAx::~CT_SerAx()
		{
			if(NULL != m_axId)
				delete m_axId;
			if(NULL != m_scaling)
				delete m_scaling;
			if(NULL != m_delete)
				delete m_delete;
			if(NULL != m_axPos)
				delete m_axPos;
			if(NULL != m_majorGridlines)
				delete m_majorGridlines;
			if(NULL != m_minorGridlines)
				delete m_minorGridlines;
			if(NULL != m_title)
				delete m_title;
			if(NULL != m_numFmt)
				delete m_numFmt;
			if(NULL != m_majorTickMark)
				delete m_majorTickMark;
			if(NULL != m_minorTickMark)
				delete m_minorTickMark;
			if(NULL != m_tickLblPos)
				delete m_tickLblPos;
			if(NULL != m_crossAx)
				delete m_crossAx;
			if(NULL != m_crosses)
				delete m_crosses;
			if(NULL != m_crossesAt)
				delete m_crossesAt;
			if(NULL != m_tickLblSkip)
				delete m_tickLblSkip;
			if(NULL != m_tickMarkSkip)
				delete m_tickMarkSkip;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_SerAx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_axId = pNewElem;
				}
				else if(_T("scaling") == sName)
				{
					CT_Scaling* pNewElem = new CT_Scaling;
					pNewElem->fromXML(oReader);
					m_scaling = pNewElem;
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_delete = pNewElem;
				}
				else if(_T("axPos") == sName)
				{
					CT_AxPos* pNewElem = new CT_AxPos;
					pNewElem->fromXML(oReader);
					m_axPos = pNewElem;
				}
				else if(_T("majorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_majorGridlines = pNewElem;
				}
				else if(_T("minorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_minorGridlines = pNewElem;
				}
				else if(_T("title") == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if(_T("majorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_majorTickMark = pNewElem;
				}
				else if(_T("minorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_minorTickMark = pNewElem;
				}
				else if(_T("tickLblPos") == sName)
				{
					CT_TickLblPos* pNewElem = new CT_TickLblPos;
					pNewElem->fromXML(oReader);
					m_tickLblPos = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("crossAx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_crossAx = pNewElem;
				}
				else if(_T("crosses") == sName)
				{
					CT_Crosses* pNewElem = new CT_Crosses;
					pNewElem->fromXML(oReader);
					m_crosses = pNewElem;
				}
				else if(_T("crossesAt") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_crossesAt = pNewElem;
				}
				else if(_T("tickLblSkip") == sName)
				{
					CT_Skip* pNewElem = new CT_Skip;
					pNewElem->fromXML(oReader);
					m_tickLblSkip = pNewElem;
				}
				else if(_T("tickMarkSkip") == sName)
				{
					CT_Skip* pNewElem = new CT_Skip;
					pNewElem->fromXML(oReader);
					m_tickMarkSkip = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_SerAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_axId)
			{
                std::wstring sNodeName = _T("c:axId");
                m_axId->toXML(sNodeName, writer);
			}
			if(NULL != m_scaling)
			{
                std::wstring sNodeName = _T("c:scaling");
                m_scaling->toXML(sNodeName, writer);
			}
			if(NULL != m_delete)
			{
                std::wstring sNodeName = _T("c:delete");
                m_delete->toXML(sNodeName, writer);
			}
			if(NULL != m_axPos)
			{
                std::wstring sNodeName = _T("c:axPos");
                m_axPos->toXML(sNodeName, writer);
			}
			if(NULL != m_majorGridlines)
			{
                std::wstring sNodeName = _T("c:majorGridlines");
                m_majorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_minorGridlines)
			{
                std::wstring sNodeName = _T("c:minorGridlines");
                m_minorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_title)
			{
                std::wstring sNodeName = _T("c:title");
                m_title->toXML(sNodeName, writer);
			}
			if(NULL != m_numFmt)
			{
                std::wstring sNodeName = _T("c:numFmt");
                m_numFmt->toXML(sNodeName, writer);
			}
			if(NULL != m_majorTickMark)
			{
                std::wstring sNodeName = _T("c:majorTickMark");
                m_majorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_minorTickMark)
			{
                std::wstring sNodeName = _T("c:minorTickMark");
                m_minorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblPos)
			{
                std::wstring sNodeName = _T("c:tickLblPos");
                m_tickLblPos->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_crossAx)
			{
                std::wstring sNodeName = _T("c:crossAx");
                m_crossAx->toXML(sNodeName, writer);
			}
			if(NULL != m_crosses)
			{
                std::wstring sNodeName = _T("c:crosses");
                m_crosses->toXML(sNodeName, writer);
			}
			if(NULL != m_crossesAt)
			{
                std::wstring sNodeName = _T("c:crossesAt");
                m_crossesAt->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblSkip)
			{
                std::wstring sNodeName = _T("c:tickLblSkip");
                m_tickLblSkip->toXML(sNodeName, writer);
			}
			if(NULL != m_tickMarkSkip)
			{
                std::wstring sNodeName = _T("c:tickMarkSkip");
                m_tickMarkSkip->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SerAx::getType(){return et_ct_serax;}
		CT_Scaling::CT_Scaling()
		{
			m_logBase = NULL;
			m_orientation = NULL;
			m_max = NULL;
			m_min = NULL;
			m_extLst = NULL;
		}
		CT_Scaling::~CT_Scaling()
		{
			if(NULL != m_logBase)
				delete m_logBase;
			if(NULL != m_orientation)
				delete m_orientation;
			if(NULL != m_max)
				delete m_max;
			if(NULL != m_min)
				delete m_min;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Scaling::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("logBase") == sName)
				{
					CT_LogBase* pNewElem = new CT_LogBase;
					pNewElem->fromXML(oReader);
					m_logBase = pNewElem;
				}
				else if(_T("orientation") == sName)
				{
					CT_Orientation* pNewElem = new CT_Orientation;
					pNewElem->fromXML(oReader);
					m_orientation = pNewElem;
				}
				else if(_T("max") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_max = pNewElem;
				}
				else if(_T("min") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_min = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Scaling::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_logBase)
			{
                std::wstring sNodeName = _T("c:logBase");
                m_logBase->toXML(sNodeName, writer);
			}
			if(NULL != m_orientation)
			{
                std::wstring sNodeName = _T("c:orientation");
                m_orientation->toXML(sNodeName, writer);
			}
			if(NULL != m_max)
			{
                std::wstring sNodeName = _T("c:max");
                m_max->toXML(sNodeName, writer);
			}
			if(NULL != m_min)
			{
                std::wstring sNodeName = _T("c:min");
                m_min->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Scaling::getType(){return et_ct_scaling;}
		CT_LogBase::CT_LogBase()
		{
			m_val = NULL;
		}
		CT_LogBase::~CT_LogBase()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LogBase::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LogBase::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrDouble(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LogBase::getType(){return et_ct_logbase;}
		void CT_LogBase::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Orientation::CT_Orientation()
		{
			/*m_val = new ST_Orientation;
			*m_val = st_orientationMINMAX;*/
			m_val = NULL;
		}
		CT_Orientation::~CT_Orientation()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Orientation::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Orientation::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_Orientation(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Orientation::getType(){return et_ct_orientation;}
		void CT_Orientation::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_Orientation* pNewElem = new ST_Orientation;
					ST_Orientation eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_Orientation(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_AxPos::CT_AxPos()
		{
			m_val = NULL;
		}
		CT_AxPos::~CT_AxPos()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_AxPos::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_AxPos::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_AxPos(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_AxPos::getType(){return et_ct_axpos;}
		void CT_AxPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_AxPos* pNewElem = new ST_AxPos;
					ST_AxPos eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_AxPos(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_ChartLines::CT_ChartLines()
		{
		}
		CT_ChartLines::~CT_ChartLines()
		{
		}
		void CT_ChartLines::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
			}
		}
		void CT_ChartLines::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ChartLines::getType(){return et_ct_chartlines;}
		CT_Title::CT_Title()
		{
			m_tx = NULL;
			m_layout = NULL;
			m_overlay = NULL;
			m_extLst = NULL;
		}
		CT_Title::~CT_Title()
		{
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_layout)
				delete m_layout;
			if(NULL != m_overlay)
				delete m_overlay;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Title::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("tx") == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if(_T("overlay") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_overlay = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Title::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if(NULL != m_layout)
			{
                std::wstring sNodeName = _T("c:layout");
                m_layout->toXML(sNodeName, writer);
			}
			if(NULL != m_overlay)
			{
                std::wstring sNodeName = _T("c:overlay");
                m_overlay->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Title::getType(){return et_ct_title;}
		CT_Tx::CT_Tx()
		{
			m_strRef = NULL;
		}
		CT_Tx::~CT_Tx()
		{
			if(NULL != m_strRef)
				delete m_strRef;
		}
		void CT_Tx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("rich") == sName)
				{
					m_oRich = oReader;
				}
				else if(_T("strRef") == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
			}
		}
		void CT_Tx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(m_oRich.IsInit())
			{
				m_oRich->m_name = L"c:rich";
				writer.WriteString(m_oRich->toXML());
			}
			if(NULL != m_strRef)
			{
                std::wstring sNodeName = _T("c:strRef");
                m_strRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Tx::getType(){return et_ct_tx;}
		CT_StrRef::CT_StrRef()
		{
			m_f = NULL;
			m_strCache = NULL;
			m_extLst = NULL;
		}
		CT_StrRef::~CT_StrRef()
		{
			if(NULL != m_f)
				delete m_f;
			if(NULL != m_strCache)
				delete m_strCache;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_StrRef::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("f") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_f = pNewElem;
				}
				else if(_T("strCache") == sName)
				{
					CT_StrData* pNewElem = new CT_StrData;
					pNewElem->fromXML(oReader);
					m_strCache = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_StrRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_f)
			{
				if (std::wstring::npos != m_f->find(' ') || std::wstring::npos != m_f->find('\n'))
					writer.WriteString(L"<c:f xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:f>");
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(L"</c:f>");
			}
			if(NULL != m_strCache)
			{
                std::wstring sNodeName = _T("c:strCache");
                m_strCache->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StrRef::getType(){return et_ct_strref;}
		CT_StrData::CT_StrData()
		{
			m_ptCount = NULL;
			m_extLst = NULL;
		}
		CT_StrData::~CT_StrData()
		{
			if(NULL != m_ptCount)
				delete m_ptCount;
            for(size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
            m_pt.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_StrData::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("ptCount") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_ptCount = pNewElem;
				}
				else if(_T("pt") == sName)
				{
					CT_StrVal* pNewElem = new CT_StrVal;
					pNewElem->fromXML(oReader);
                    m_pt.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_StrData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_ptCount)
			{
                std::wstring sNodeName = _T("c:ptCount");
                m_ptCount->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_StrVal* pElem = m_pt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:pt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StrData::getType(){return et_ct_strdata;}
		CT_StrVal::CT_StrVal()
		{
			m_v = NULL;
			m_idx = NULL;
		}
		CT_StrVal::~CT_StrVal()
		{
			if(NULL != m_v)
				delete m_v;
			if(NULL != m_idx)
				delete m_idx;
		}
		void CT_StrVal::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("v") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_v = pNewElem;
					}
				}
		}
		void CT_StrVal::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_idx)
			{
				WritingStringAttrInt(L"idx", *m_idx);
			}
			writer.WriteString(L">");
			if(NULL != m_v)
			{
				if (std::wstring::npos != m_v->find(' ') || std::wstring::npos != m_v->find('\n'))
					writer.WriteString(L"<c:v xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:v>");
				writer.WriteEncodeXmlString(*m_v);
				writer.WriteString(L"</c:v>");
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StrVal::getType(){return et_ct_strval;}
		void CT_StrVal::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("idx") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_idx = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_NumFmt::CT_NumFmt()
		{
			m_formatCode = NULL;
			m_sourceLinked = NULL;
		}
		CT_NumFmt::~CT_NumFmt()
		{
			if(NULL != m_formatCode)
				delete m_formatCode;
			if(NULL != m_sourceLinked)
				delete m_sourceLinked;
		}
		void CT_NumFmt::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_NumFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_formatCode)
			{
				WritingStringAttrEncodeXmlString(L"formatCode", *m_formatCode);
			}
			if(NULL != m_sourceLinked)
			{
				std::wstring sVal;
				if(*m_sourceLinked)
					sVal = _T("1");
				else
					sVal = _T("0");
				WritingStringAttrString(L"sourceLinked", sVal);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_NumFmt::getType(){return et_ct_numfmt;}
		void CT_NumFmt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("formatCode") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_formatCode = pNewElem;
				}
				else if(_T("sourceLinked") == wsName)
				{
					bool* pNewElem = new bool;
					std::wstring sVal = oReader.GetText();
                    std::wstring sValLower = sVal;
					NSStringExt::ToLower(sValLower);
					if(_T("false") == sValLower || _T("0") == sValLower || _T("off") == sValLower)
						*pNewElem = false;
					else
						*pNewElem = true;
					m_sourceLinked = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_TickMark::CT_TickMark()
		{
			/*m_val = new ST_TickMark;
			*m_val = st_tickmarkCROSS;*/
			m_val = NULL;
		}
		CT_TickMark::~CT_TickMark()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_TickMark::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_TickMark::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_TickMark(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_TickMark::getType(){return et_ct_tickmark;}
		void CT_TickMark::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_TickMark* pNewElem = new ST_TickMark;
					ST_TickMark eVal;
                    std::wstring sNodeName = oReader.GetText();

                    if(FromXml_ST_TickMark(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_TickLblPos::CT_TickLblPos()
		{
			/*m_val = new ST_TickLblPos;
			*m_val = st_ticklblposNEXTTO;*/
			m_val = NULL;
		}
		CT_TickLblPos::~CT_TickLblPos()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_TickLblPos::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_TickLblPos::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_TickLblPos(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_TickLblPos::getType(){return et_ct_ticklblpos;}
		void CT_TickLblPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_TickLblPos* pNewElem = new ST_TickLblPos;
					ST_TickLblPos eVal;
                    std::wstring sNodeName = oReader.GetText();

                    if(FromXml_ST_TickLblPos(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Crosses::CT_Crosses()
		{
			m_val = NULL;
		}
		CT_Crosses::~CT_Crosses()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Crosses::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Crosses::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_Crosses(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Crosses::getType(){return et_ct_crosses;}
		void CT_Crosses::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_Crosses* pNewElem = new ST_Crosses;
					ST_Crosses eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_Crosses(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Skip::CT_Skip()
		{
			m_val = NULL;
		}
		CT_Skip::~CT_Skip()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Skip::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Skip::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Skip::getType(){return et_ct_skip;}
		void CT_Skip::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_TimeUnit::CT_TimeUnit()
		{
			/*m_val = new ST_TimeUnit;
			*m_val = st_timeunitDAYS;*/
			m_val = NULL;
		}
		CT_TimeUnit::~CT_TimeUnit()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_TimeUnit::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_TimeUnit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_TimeUnit(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_TimeUnit::getType(){return et_ct_timeunit;}
		void CT_TimeUnit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_TimeUnit* pNewElem = new ST_TimeUnit;
					ST_TimeUnit eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_TimeUnit(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DateAx::CT_DateAx()
		{
			m_axId = NULL;
			m_scaling = NULL;
			m_delete = NULL;
			m_axPos = NULL;
			m_majorGridlines = NULL;
			m_minorGridlines = NULL;
			m_title = NULL;
			m_numFmt = NULL;
			m_majorTickMark = NULL;
			m_minorTickMark = NULL;
			m_tickLblPos = NULL;
			m_crossAx = NULL;
			m_crosses = NULL;
			m_crossesAt = NULL;
			m_auto = NULL;
			m_lblOffset = NULL;
			m_baseTimeUnit = NULL;
			m_majorUnit = NULL;
			m_majorTimeUnit = NULL;
			m_minorUnit = NULL;
			m_minorTimeUnit = NULL;
			m_extLst = NULL;
		}
		CT_DateAx::~CT_DateAx()
		{
			if(NULL != m_axId)
				delete m_axId;
			if(NULL != m_scaling)
				delete m_scaling;
			if(NULL != m_delete)
				delete m_delete;
			if(NULL != m_axPos)
				delete m_axPos;
			if(NULL != m_majorGridlines)
				delete m_majorGridlines;
			if(NULL != m_minorGridlines)
				delete m_minorGridlines;
			if(NULL != m_title)
				delete m_title;
			if(NULL != m_numFmt)
				delete m_numFmt;
			if(NULL != m_majorTickMark)
				delete m_majorTickMark;
			if(NULL != m_minorTickMark)
				delete m_minorTickMark;
			if(NULL != m_tickLblPos)
				delete m_tickLblPos;
			if(NULL != m_crossAx)
				delete m_crossAx;
			if(NULL != m_crosses)
				delete m_crosses;
			if(NULL != m_crossesAt)
				delete m_crossesAt;
			if(NULL != m_auto)
				delete m_auto;
			if(NULL != m_lblOffset)
				delete m_lblOffset;
			if(NULL != m_baseTimeUnit)
				delete m_baseTimeUnit;
			if(NULL != m_majorUnit)
				delete m_majorUnit;
			if(NULL != m_majorTimeUnit)
				delete m_majorTimeUnit;
			if(NULL != m_minorUnit)
				delete m_minorUnit;
			if(NULL != m_minorTimeUnit)
				delete m_minorTimeUnit;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DateAx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_axId = pNewElem;
				}
				else if(_T("scaling") == sName)
				{
					CT_Scaling* pNewElem = new CT_Scaling;
					pNewElem->fromXML(oReader);
					m_scaling = pNewElem;
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_delete = pNewElem;
				}
				else if(_T("axPos") == sName)
				{
					CT_AxPos* pNewElem = new CT_AxPos;
					pNewElem->fromXML(oReader);
					m_axPos = pNewElem;
				}
				else if(_T("majorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_majorGridlines = pNewElem;
				}
				else if(_T("minorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_minorGridlines = pNewElem;
				}
				else if(_T("title") == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if(_T("majorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_majorTickMark = pNewElem;
				}
				else if(_T("minorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_minorTickMark = pNewElem;
				}
				else if(_T("tickLblPos") == sName)
				{
					CT_TickLblPos* pNewElem = new CT_TickLblPos;
					pNewElem->fromXML(oReader);
					m_tickLblPos = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("crossAx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_crossAx = pNewElem;
				}
				else if(_T("crosses") == sName)
				{
					CT_Crosses* pNewElem = new CT_Crosses;
					pNewElem->fromXML(oReader);
					m_crosses = pNewElem;
				}
				else if(_T("crossesAt") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_crossesAt = pNewElem;
				}
				else if(_T("auto") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_auto = pNewElem;
				}
				else if(_T("lblOffset") == sName)
				{
					CT_LblOffset* pNewElem = new CT_LblOffset;
					pNewElem->fromXML(oReader);
					m_lblOffset = pNewElem;
				}
				else if(_T("baseTimeUnit") == sName)
				{
					CT_TimeUnit* pNewElem = new CT_TimeUnit;
					pNewElem->fromXML(oReader);
					m_baseTimeUnit = pNewElem;
				}
				else if(_T("majorUnit") == sName)
				{
					CT_AxisUnit* pNewElem = new CT_AxisUnit;
					pNewElem->fromXML(oReader);
					m_majorUnit = pNewElem;
				}
				else if(_T("majorTimeUnit") == sName)
				{
					CT_TimeUnit* pNewElem = new CT_TimeUnit;
					pNewElem->fromXML(oReader);
					m_majorTimeUnit = pNewElem;
				}
				else if(_T("minorUnit") == sName)
				{
					CT_AxisUnit* pNewElem = new CT_AxisUnit;
					pNewElem->fromXML(oReader);
					m_minorUnit = pNewElem;
				}
				else if(_T("minorTimeUnit") == sName)
				{
					CT_TimeUnit* pNewElem = new CT_TimeUnit;
					pNewElem->fromXML(oReader);
					m_minorTimeUnit = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DateAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_axId)
			{
                std::wstring sNodeName = _T("c:axId");
                m_axId->toXML(sNodeName, writer);
			}
			if(NULL != m_scaling)
			{
                std::wstring sNodeName = _T("c:scaling");
                m_scaling->toXML(sNodeName, writer);
			}
			if(NULL != m_delete)
			{
                std::wstring sNodeName = _T("c:delete");
                m_delete->toXML(sNodeName, writer);
			}
			if(NULL != m_axPos)
			{
                std::wstring sNodeName = _T("c:axPos");
                m_axPos->toXML(sNodeName, writer);
			}
			if(NULL != m_majorGridlines)
			{
                std::wstring sNodeName = _T("c:majorGridlines");
                m_majorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_minorGridlines)
			{
                std::wstring sNodeName = _T("c:minorGridlines");
                m_minorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_title)
			{
                std::wstring sNodeName = _T("c:title");
                m_title->toXML(sNodeName, writer);
			}
			if(NULL != m_numFmt)
			{
                std::wstring sNodeName = _T("c:numFmt");
                m_numFmt->toXML(sNodeName, writer);
			}
			if(NULL != m_majorTickMark)
			{
                std::wstring sNodeName = _T("c:majorTickMark");
                m_majorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_minorTickMark)
			{
                std::wstring sNodeName = _T("c:minorTickMark");
                m_minorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblPos)
			{
                std::wstring sNodeName = _T("c:tickLblPos");
                m_tickLblPos->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_crossAx)
			{
                std::wstring sNodeName = _T("c:crossAx");
                m_crossAx->toXML(sNodeName, writer);
			}
			if(NULL != m_crosses)
			{
                std::wstring sNodeName = _T("c:crosses");
                m_crosses->toXML(sNodeName, writer);
			}
			if(NULL != m_crossesAt)
			{
                std::wstring sNodeName = _T("c:crossesAt");
                m_crossesAt->toXML(sNodeName, writer);
			}
			if(NULL != m_auto)
			{
                std::wstring sNodeName = _T("c:auto");
                m_auto->toXML(sNodeName, writer);
			}
			if(NULL != m_lblOffset)
			{
                std::wstring sNodeName = _T("c:lblOffset");
                m_lblOffset->toXML(sNodeName, writer);
			}
			if(NULL != m_baseTimeUnit)
			{
                std::wstring sNodeName = _T("c:baseTimeUnit");
                m_baseTimeUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_majorUnit)
			{
                std::wstring sNodeName = _T("c:majorUnit");
                m_majorUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_majorTimeUnit)
			{
                std::wstring sNodeName = _T("c:majorTimeUnit");
                m_majorTimeUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_minorUnit)
			{
                std::wstring sNodeName = _T("c:minorUnit");
                m_minorUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_minorTimeUnit)
			{
                std::wstring sNodeName = _T("c:minorTimeUnit");
                m_minorTimeUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DateAx::getType(){return et_ct_dateax;}
		CT_LblOffset::CT_LblOffset()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("100%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_LblOffset::~CT_LblOffset()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LblOffset::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LblOffset::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LblOffset::getType(){return et_ct_lbloffset;}
		void CT_LblOffset::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_AxisUnit::CT_AxisUnit()
		{
			m_val = NULL;
		}
		CT_AxisUnit::~CT_AxisUnit()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_AxisUnit::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_AxisUnit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrDouble(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_AxisUnit::getType(){return et_ct_axisunit;}
		void CT_AxisUnit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_LblAlgn::CT_LblAlgn()
		{
			m_val = NULL;
		}
		CT_LblAlgn::~CT_LblAlgn()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_LblAlgn::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_LblAlgn::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_LblAlgn(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_LblAlgn::getType(){return et_ct_lblalgn;}
		void CT_LblAlgn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_LblAlgn* pNewElem = new ST_LblAlgn;
					ST_LblAlgn eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_LblAlgn(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_CatAx::CT_CatAx()
		{
			m_axId = NULL;
			m_scaling = NULL;
			m_delete = NULL;
			m_axPos = NULL;
			m_majorGridlines = NULL;
			m_minorGridlines = NULL;
			m_title = NULL;
			m_numFmt = NULL;
			m_majorTickMark = NULL;
			m_minorTickMark = NULL;
			m_tickLblPos = NULL;
			m_crossAx = NULL;
			m_crosses = NULL;
			m_crossesAt = NULL;
			m_auto = NULL;
			m_lblAlgn = NULL;
			m_lblOffset = NULL;
			m_tickLblSkip = NULL;
			m_tickMarkSkip = NULL;
			m_noMultiLvlLbl = NULL;
			m_extLst = NULL;
		}
		CT_CatAx::~CT_CatAx()
		{
			if(NULL != m_axId)
				delete m_axId;
			if(NULL != m_scaling)
				delete m_scaling;
			if(NULL != m_delete)
				delete m_delete;
			if(NULL != m_axPos)
				delete m_axPos;
			if(NULL != m_majorGridlines)
				delete m_majorGridlines;
			if(NULL != m_minorGridlines)
				delete m_minorGridlines;
			if(NULL != m_title)
				delete m_title;
			if(NULL != m_numFmt)
				delete m_numFmt;
			if(NULL != m_majorTickMark)
				delete m_majorTickMark;
			if(NULL != m_minorTickMark)
				delete m_minorTickMark;
			if(NULL != m_tickLblPos)
				delete m_tickLblPos;
			if(NULL != m_crossAx)
				delete m_crossAx;
			if(NULL != m_crosses)
				delete m_crosses;
			if(NULL != m_crossesAt)
				delete m_crossesAt;
			if(NULL != m_auto)
				delete m_auto;
			if(NULL != m_lblAlgn)
				delete m_lblAlgn;
			if(NULL != m_lblOffset)
				delete m_lblOffset;
			if(NULL != m_tickLblSkip)
				delete m_tickLblSkip;
			if(NULL != m_tickMarkSkip)
				delete m_tickMarkSkip;
			if(NULL != m_noMultiLvlLbl)
				delete m_noMultiLvlLbl;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_CatAx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_axId = pNewElem;
				}
				else if(_T("scaling") == sName)
				{
					CT_Scaling* pNewElem = new CT_Scaling;
					pNewElem->fromXML(oReader);
					m_scaling = pNewElem;
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_delete = pNewElem;
				}
				else if(_T("axPos") == sName)
				{
					CT_AxPos* pNewElem = new CT_AxPos;
					pNewElem->fromXML(oReader);
					m_axPos = pNewElem;
				}
				else if(_T("majorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_majorGridlines = pNewElem;
				}
				else if(_T("minorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_minorGridlines = pNewElem;
				}
				else if(_T("title") == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if(_T("majorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_majorTickMark = pNewElem;
				}
				else if(_T("minorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_minorTickMark = pNewElem;
				}
				else if(_T("tickLblPos") == sName)
				{
					CT_TickLblPos* pNewElem = new CT_TickLblPos;
					pNewElem->fromXML(oReader);
					m_tickLblPos = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("crossAx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_crossAx = pNewElem;
				}
				else if(_T("crosses") == sName)
				{
					CT_Crosses* pNewElem = new CT_Crosses;
					pNewElem->fromXML(oReader);
					m_crosses = pNewElem;
				}
				else if(_T("crossesAt") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_crossesAt = pNewElem;
				}
				else if(_T("auto") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_auto = pNewElem;
				}
				else if(_T("lblAlgn") == sName)
				{
					CT_LblAlgn* pNewElem = new CT_LblAlgn;
					pNewElem->fromXML(oReader);
					m_lblAlgn = pNewElem;
				}
				else if(_T("lblOffset") == sName)
				{
					CT_LblOffset* pNewElem = new CT_LblOffset;
					pNewElem->fromXML(oReader);
					m_lblOffset = pNewElem;
				}
				else if(_T("tickLblSkip") == sName)
				{
					CT_Skip* pNewElem = new CT_Skip;
					pNewElem->fromXML(oReader);
					m_tickLblSkip = pNewElem;
				}
				else if(_T("tickMarkSkip") == sName)
				{
					CT_Skip* pNewElem = new CT_Skip;
					pNewElem->fromXML(oReader);
					m_tickMarkSkip = pNewElem;
				}
				else if(_T("noMultiLvlLbl") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_noMultiLvlLbl = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_CatAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_axId)
			{
                std::wstring sNodeName = _T("c:axId");
                m_axId->toXML(sNodeName, writer);
			}
			if(NULL != m_scaling)
			{
                std::wstring sNodeName = _T("c:scaling");
                m_scaling->toXML(sNodeName, writer);
			}
			if(NULL != m_delete)
			{
                std::wstring sNodeName = _T("c:delete");
                m_delete->toXML(sNodeName, writer);
			}
			if(NULL != m_axPos)
			{
                std::wstring sNodeName = _T("c:axPos");
                m_axPos->toXML(sNodeName, writer);
			}
			if(NULL != m_majorGridlines)
			{
                std::wstring sNodeName = _T("c:majorGridlines");
                m_majorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_minorGridlines)
			{
                std::wstring sNodeName = _T("c:minorGridlines");
                m_minorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_title)
			{
                std::wstring sNodeName = _T("c:title");
                m_title->toXML(sNodeName, writer);
			}
			if(NULL != m_numFmt)
			{
                std::wstring sNodeName = _T("c:numFmt");
                m_numFmt->toXML(sNodeName, writer);
			}
			if(NULL != m_majorTickMark)
			{
                std::wstring sNodeName = _T("c:majorTickMark");
                m_majorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_minorTickMark)
			{
                std::wstring sNodeName = _T("c:minorTickMark");
                m_minorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblPos)
			{
                std::wstring sNodeName = _T("c:tickLblPos");
                m_tickLblPos->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_crossAx)
			{
                std::wstring sNodeName = _T("c:crossAx");
                m_crossAx->toXML(sNodeName, writer);
			}
			if(NULL != m_crosses)
			{
                std::wstring sNodeName = _T("c:crosses");
                m_crosses->toXML(sNodeName, writer);
			}
			if(NULL != m_crossesAt)
			{
                std::wstring sNodeName = _T("c:crossesAt");
                m_crossesAt->toXML(sNodeName, writer);
			}
			if(NULL != m_auto)
			{
                std::wstring sNodeName = _T("c:auto");
                m_auto->toXML(sNodeName, writer);
			}
			if(NULL != m_lblAlgn)
			{
                std::wstring sNodeName = _T("c:lblAlgn");
                m_lblAlgn->toXML(sNodeName, writer);
			}
			if(NULL != m_lblOffset)
			{
                std::wstring sNodeName = _T("c:lblOffset");
                m_lblOffset->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblSkip)
			{
                std::wstring sNodeName = _T("c:tickLblSkip");
                m_tickLblSkip->toXML(sNodeName, writer);
			}
			if(NULL != m_tickMarkSkip)
			{
                std::wstring sNodeName = _T("c:tickMarkSkip");
                m_tickMarkSkip->toXML(sNodeName, writer);
			}
			if(NULL != m_noMultiLvlLbl)
			{
                std::wstring sNodeName = _T("c:noMultiLvlLbl");
                m_noMultiLvlLbl->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_CatAx::getType(){return et_ct_catax;}
		CT_DispUnitsLbl::CT_DispUnitsLbl()
		{
			m_layout = NULL;
			m_tx = NULL;
		}
		CT_DispUnitsLbl::~CT_DispUnitsLbl()
		{
			if(NULL != m_layout)
				delete m_layout;
			if(NULL != m_tx)
				delete m_tx;
		}
		void CT_DispUnitsLbl::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
			}
		}
		void CT_DispUnitsLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_layout)
			{
                std::wstring sNodeName = _T("c:layout");
                m_layout->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DispUnitsLbl::getType(){return et_ct_dispunitslbl;}
		CT_BuiltInUnit::CT_BuiltInUnit()
		{
			/*m_val = new ST_BuiltInUnit;
			*m_val = st_builtinunitTHOUSANDS;*/
			m_val = NULL;
		}
		CT_BuiltInUnit::~CT_BuiltInUnit()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_BuiltInUnit::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_BuiltInUnit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_BuiltInUnit(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_BuiltInUnit::getType(){return et_ct_builtinunit;}
		void CT_BuiltInUnit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_BuiltInUnit* pNewElem = new ST_BuiltInUnit;
					ST_BuiltInUnit eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_BuiltInUnit(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DispUnits::CT_DispUnits()
		{
			m_builtInUnit = NULL;
			m_custUnit = NULL;
			m_dispUnitsLbl = NULL;
			m_extLst = NULL;
		}
		CT_DispUnits::~CT_DispUnits()
		{
			if(NULL != m_builtInUnit)
				delete m_builtInUnit;
			if(NULL != m_custUnit)
				delete m_custUnit;
			if(NULL != m_dispUnitsLbl)
				delete m_dispUnitsLbl;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DispUnits::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("builtInUnit") == sName)
				{
					CT_BuiltInUnit* pNewElem = new CT_BuiltInUnit;
					pNewElem->fromXML(oReader);
					m_builtInUnit = pNewElem;
				}
				else if(_T("custUnit") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_custUnit = pNewElem;
				}
				else if(_T("dispUnitsLbl") == sName)
				{
					CT_DispUnitsLbl* pNewElem = new CT_DispUnitsLbl;
					pNewElem->fromXML(oReader);
					m_dispUnitsLbl = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DispUnits::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_builtInUnit)
			{
                std::wstring sNodeName = _T("c:builtInUnit");
                m_builtInUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_custUnit)
			{
                std::wstring sNodeName = _T("c:custUnit");
                m_custUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_dispUnitsLbl)
			{
                std::wstring sNodeName = _T("c:dispUnitsLbl");
                m_dispUnitsLbl->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DispUnits::getType(){return et_ct_dispunits;}
		CT_CrossBetween::CT_CrossBetween()
		{
			m_val = NULL;
		}
		CT_CrossBetween::~CT_CrossBetween()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_CrossBetween::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_CrossBetween::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_CrossBetween(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_CrossBetween::getType(){return et_ct_crossbetween;}
		void CT_CrossBetween::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_CrossBetween* pNewElem = new ST_CrossBetween;
					ST_CrossBetween eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_CrossBetween(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_ValAx::CT_ValAx()
		{
			m_axId = NULL;
			m_scaling = NULL;
			m_delete = NULL;
			m_axPos = NULL;
			m_majorGridlines = NULL;
			m_minorGridlines = NULL;
			m_title = NULL;
			m_numFmt = NULL;
			m_majorTickMark = NULL;
			m_minorTickMark = NULL;
			m_tickLblPos = NULL;
			m_crossAx = NULL;
			m_crosses = NULL;
			m_crossesAt = NULL;
			m_crossBetween = NULL;
			m_majorUnit = NULL;
			m_minorUnit = NULL;
			m_dispUnits = NULL;
			m_extLst = NULL;
		}
		CT_ValAx::~CT_ValAx()
		{
			if(NULL != m_axId)
				delete m_axId;
			if(NULL != m_scaling)
				delete m_scaling;
			if(NULL != m_delete)
				delete m_delete;
			if(NULL != m_axPos)
				delete m_axPos;
			if(NULL != m_majorGridlines)
				delete m_majorGridlines;
			if(NULL != m_minorGridlines)
				delete m_minorGridlines;
			if(NULL != m_title)
				delete m_title;
			if(NULL != m_numFmt)
				delete m_numFmt;
			if(NULL != m_majorTickMark)
				delete m_majorTickMark;
			if(NULL != m_minorTickMark)
				delete m_minorTickMark;
			if(NULL != m_tickLblPos)
				delete m_tickLblPos;
			if(NULL != m_crossAx)
				delete m_crossAx;
			if(NULL != m_crosses)
				delete m_crosses;
			if(NULL != m_crossesAt)
				delete m_crossesAt;
			if(NULL != m_crossBetween)
				delete m_crossBetween;
			if(NULL != m_majorUnit)
				delete m_majorUnit;
			if(NULL != m_minorUnit)
				delete m_minorUnit;
			if(NULL != m_dispUnits)
				delete m_dispUnits;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ValAx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_axId = pNewElem;
				}
				else if(_T("scaling") == sName)
				{
					CT_Scaling* pNewElem = new CT_Scaling;
					pNewElem->fromXML(oReader);
					m_scaling = pNewElem;
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_delete = pNewElem;
				}
				else if(_T("axPos") == sName)
				{
					CT_AxPos* pNewElem = new CT_AxPos;
					pNewElem->fromXML(oReader);
					m_axPos = pNewElem;
				}
				else if(_T("majorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_majorGridlines = pNewElem;
				}
				else if(_T("minorGridlines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_minorGridlines = pNewElem;
				}
				else if(_T("title") == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if(_T("majorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_majorTickMark = pNewElem;
				}
				else if(_T("minorTickMark") == sName)
				{
					CT_TickMark* pNewElem = new CT_TickMark;
					pNewElem->fromXML(oReader);
					m_minorTickMark = pNewElem;
				}
				else if(_T("tickLblPos") == sName)
				{
					CT_TickLblPos* pNewElem = new CT_TickLblPos;
					pNewElem->fromXML(oReader);
					m_tickLblPos = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("crossAx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_crossAx = pNewElem;
				}
				else if(_T("crosses") == sName)
				{
					CT_Crosses* pNewElem = new CT_Crosses;
					pNewElem->fromXML(oReader);
					m_crosses = pNewElem;
				}
				else if(_T("crossesAt") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_crossesAt = pNewElem;
				}
				else if(_T("crossBetween") == sName)
				{
					CT_CrossBetween* pNewElem = new CT_CrossBetween;
					pNewElem->fromXML(oReader);
					m_crossBetween = pNewElem;
				}
				else if(_T("majorUnit") == sName)
				{
					CT_AxisUnit* pNewElem = new CT_AxisUnit;
					pNewElem->fromXML(oReader);
					m_majorUnit = pNewElem;
				}
				else if(_T("minorUnit") == sName)
				{
					CT_AxisUnit* pNewElem = new CT_AxisUnit;
					pNewElem->fromXML(oReader);
					m_minorUnit = pNewElem;
				}
				else if(_T("dispUnits") == sName)
				{
					CT_DispUnits* pNewElem = new CT_DispUnits;
					pNewElem->fromXML(oReader);
					m_dispUnits = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ValAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_axId)
			{
                std::wstring sNodeName = _T("c:axId");
                m_axId->toXML(sNodeName, writer);
			}
			if(NULL != m_scaling)
			{
                std::wstring sNodeName = _T("c:scaling");
                m_scaling->toXML(sNodeName, writer);
			}
			if(NULL != m_delete)
			{
                std::wstring sNodeName = _T("c:delete");
                m_delete->toXML(sNodeName, writer);
			}
			if(NULL != m_axPos)
			{
                std::wstring sNodeName = _T("c:axPos");
                m_axPos->toXML(sNodeName, writer);
			}
			if(NULL != m_majorGridlines)
			{
                std::wstring sNodeName = _T("c:majorGridlines");
                m_majorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_minorGridlines)
			{
                std::wstring sNodeName = _T("c:minorGridlines");
                m_minorGridlines->toXML(sNodeName, writer);
			}
			if(NULL != m_title)
			{
                std::wstring sNodeName = _T("c:title");
                m_title->toXML(sNodeName, writer);
			}
			if(NULL != m_numFmt)
			{
                std::wstring sNodeName = _T("c:numFmt");
                m_numFmt->toXML(sNodeName, writer);
			}
			if(NULL != m_majorTickMark)
			{
                std::wstring sNodeName = _T("c:majorTickMark");
                m_majorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_minorTickMark)
			{
                std::wstring sNodeName = _T("c:minorTickMark");
                m_minorTickMark->toXML(sNodeName, writer);
			}
			if(NULL != m_tickLblPos)
			{
                std::wstring sNodeName = _T("c:tickLblPos");
                m_tickLblPos->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_crossAx)
			{
                std::wstring sNodeName = _T("c:crossAx");
                m_crossAx->toXML(sNodeName, writer);
			}
			if(NULL != m_crosses)
			{
                std::wstring sNodeName = _T("c:crosses");
                m_crosses->toXML(sNodeName, writer);
			}
			if(NULL != m_crossesAt)
			{
                std::wstring sNodeName = _T("c:crossesAt");
                m_crossesAt->toXML(sNodeName, writer);
			}
			if(NULL != m_crossBetween)
			{
                std::wstring sNodeName = _T("c:crossBetween");
                m_crossBetween->toXML(sNodeName, writer);
			}
			if(NULL != m_majorUnit)
			{
                std::wstring sNodeName = _T("c:majorUnit");
                m_majorUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_minorUnit)
			{
                std::wstring sNodeName = _T("c:minorUnit");
                m_minorUnit->toXML(sNodeName, writer);
			}
			if(NULL != m_dispUnits)
			{
                std::wstring sNodeName = _T("c:dispUnits");
                m_dispUnits->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ValAx::getType(){return et_ct_valax;}
		CT_SizeRepresents::CT_SizeRepresents()
		{
			/*m_val = new ST_SizeRepresents;
			*m_val = st_sizerepresentsAREA;*/
			m_val = NULL;
		}
		CT_SizeRepresents::~CT_SizeRepresents()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_SizeRepresents::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_SizeRepresents::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_SizeRepresents(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_SizeRepresents::getType(){return et_ct_sizerepresents;}
		void CT_SizeRepresents::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_SizeRepresents* pNewElem = new ST_SizeRepresents;
					ST_SizeRepresents eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_SizeRepresents(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_BubbleScale::CT_BubbleScale()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("100%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_BubbleScale::~CT_BubbleScale()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_BubbleScale::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_BubbleScale::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_BubbleScale::getType(){return et_ct_bubblescale;}
		void CT_BubbleScale::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_BubbleSer::CT_BubbleSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_invertIfNegative = NULL;
			m_dLbls = NULL;
			m_xVal = NULL;
			m_yVal = NULL;
			m_bubbleSize = NULL;
			m_bubble3D = NULL;
			m_extLst = NULL;
		}
		CT_BubbleSer::~CT_BubbleSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_invertIfNegative)
				delete m_invertIfNegative;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
            m_trendline.clear();
            for(size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
            m_errBars.clear();
			if(NULL != m_xVal)
				delete m_xVal;
			if(NULL != m_yVal)
				delete m_yVal;
			if(NULL != m_bubbleSize)
				delete m_bubbleSize;
			if(NULL != m_bubble3D)
				delete m_bubble3D;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_BubbleSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("invertIfNegative") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_invertIfNegative = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("trendline") == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
                    m_trendline.push_back(pNewElem);
				}
				else if(_T("errBars") == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
                    m_errBars.push_back(pNewElem);
				}
				else if(_T("xVal") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_xVal = pNewElem;
				}
				else if(_T("yVal") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_yVal = pNewElem;
				}
				else if(_T("bubbleSize") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_bubbleSize = pNewElem;
				}
				else if(_T("bubble3D") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_bubble3D = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_BubbleSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_invertIfNegative)
			{
                std::wstring sNodeName = _T("c:invertIfNegative");
                m_invertIfNegative->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size();++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:trendline");
                    pElem->toXML(sNodeName, writer);
				}
			}
            for(size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:errBars");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_xVal)
			{
                std::wstring sNodeName = _T("c:xVal");
                m_xVal->toXML(sNodeName, writer);
			}
			if(NULL != m_yVal)
			{
                std::wstring sNodeName = _T("c:yVal");
                m_yVal->toXML(sNodeName, writer);
			}
			if(NULL != m_bubbleSize)
			{
                std::wstring sNodeName = _T("c:bubbleSize");
                m_bubbleSize->toXML(sNodeName, writer);
			}
			if(NULL != m_bubble3D)
			{
                std::wstring sNodeName = _T("c:bubble3D");
                m_bubble3D->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BubbleSer::getType(){return et_ct_bubbleser;}
		CT_SerTx::CT_SerTx()
		{
			m_strRef = NULL;
			m_v = NULL;
		}
		CT_SerTx::~CT_SerTx()
		{
			if(NULL != m_strRef)
				delete m_strRef;
			if(NULL != m_v)
				delete m_v;
		}
		void CT_SerTx::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("strRef") == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
				else if(_T("v") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_v = pNewElem;
				}
			}
		}
		void CT_SerTx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_strRef)
			{
                std::wstring sNodeName = _T("c:strRef");
                m_strRef->toXML(sNodeName, writer);
			}
			if(NULL != m_v)
			{
				if (std::wstring::npos != m_v->find(' ') || std::wstring::npos != m_v->find('\n'))
					writer.WriteString(_T("<c:v xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:v>"));
				writer.WriteEncodeXmlString(*m_v);
				writer.WriteString(_T("</c:v>"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SerTx::getType(){return et_ct_sertx;}
		CT_DPt::CT_DPt()
		{
			m_idx = NULL;
			m_invertIfNegative = NULL;
			m_marker = NULL;
			m_bubble3D = NULL;
			m_explosion = NULL;
			m_pictureOptions = NULL;
			m_extLst = NULL;
		}
		CT_DPt::~CT_DPt()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_invertIfNegative)
				delete m_invertIfNegative;
			if(NULL != m_marker)
				delete m_marker;
			if(NULL != m_bubble3D)
				delete m_bubble3D;
			if(NULL != m_explosion)
				delete m_explosion;
			if(NULL != m_pictureOptions)
				delete m_pictureOptions;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DPt::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("invertIfNegative") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_invertIfNegative = pNewElem;
				}
				else if(_T("marker") == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("bubble3D") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_bubble3D = pNewElem;
				}
				else if(_T("explosion") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_explosion = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("pictureOptions") == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DPt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_invertIfNegative)
			{
                std::wstring sNodeName = _T("c:invertIfNegative");
                m_invertIfNegative->toXML(sNodeName, writer);
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
			if(NULL != m_bubble3D)
			{
                std::wstring sNodeName = _T("c:bubble3D");
                m_bubble3D->toXML(sNodeName, writer);
			}
			if(NULL != m_explosion)
			{
                std::wstring sNodeName = _T("c:explosion");
                m_explosion->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_pictureOptions)
			{
                std::wstring sNodeName = _T("c:pictureOptions");
                m_pictureOptions->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DPt::getType(){return et_ct_dpt;}
		CT_Marker::CT_Marker()
		{
			m_symbol = NULL;
			m_size = NULL;
			m_extLst = NULL;
		}
		CT_Marker::~CT_Marker()
		{
			if(NULL != m_symbol)
				delete m_symbol;
			if(NULL != m_size)
				delete m_size;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Marker::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("symbol") == sName)
				{
					CT_MarkerStyle* pNewElem = new CT_MarkerStyle;
					pNewElem->fromXML(oReader);
					m_symbol = pNewElem;
				}
				else if(_T("size") == sName)
				{
					CT_MarkerSize* pNewElem = new CT_MarkerSize;
					pNewElem->fromXML(oReader);
					m_size = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Marker::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_symbol)
			{
                std::wstring sNodeName = _T("c:symbol");
                m_symbol->toXML(sNodeName, writer);
			}
			if(NULL != m_size)
			{
                std::wstring sNodeName = _T("c:size");
                m_size->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Marker::getType(){return et_ct_marker;}
		CT_MarkerStyle::CT_MarkerStyle()
		{
			m_val = NULL;
		}
		CT_MarkerStyle::~CT_MarkerStyle()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_MarkerStyle::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_MarkerStyle::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_MarkerStyle(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_MarkerStyle::getType(){return et_ct_markerstyle;}
		void CT_MarkerStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_MarkerStyle* pNewElem = new ST_MarkerStyle;
					ST_MarkerStyle eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_MarkerStyle(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_MarkerSize::CT_MarkerSize()
		{
			/*m_val = new unsigned char;
			std::wstring s_m_val("5");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_MarkerSize::~CT_MarkerSize()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_MarkerSize::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_MarkerSize::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_MarkerSize::getType(){return et_ct_markersize;}
		void CT_MarkerSize::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned char* pNewElem = new unsigned char;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_PictureOptions::CT_PictureOptions()
		{
			m_applyToFront = NULL;
			m_applyToSides = NULL;
			m_applyToEnd = NULL;
			m_pictureFormat = NULL;
			m_pictureStackUnit = NULL;
		}
		CT_PictureOptions::~CT_PictureOptions()
		{
			if(NULL != m_applyToFront)
				delete m_applyToFront;
			if(NULL != m_applyToSides)
				delete m_applyToSides;
			if(NULL != m_applyToEnd)
				delete m_applyToEnd;
			if(NULL != m_pictureFormat)
				delete m_pictureFormat;
			if(NULL != m_pictureStackUnit)
				delete m_pictureStackUnit;
		}
		void CT_PictureOptions::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("applyToFront") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_applyToFront = pNewElem;
				}
				else if(_T("applyToSides") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_applyToSides = pNewElem;
				}
				else if(_T("applyToEnd") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_applyToEnd = pNewElem;
				}
				else if(_T("pictureFormat") == sName)
				{
					CT_PictureFormat* pNewElem = new CT_PictureFormat;
					pNewElem->fromXML(oReader);
					m_pictureFormat = pNewElem;
				}
				else if(_T("pictureStackUnit") == sName)
				{
					CT_PictureStackUnit* pNewElem = new CT_PictureStackUnit;
					pNewElem->fromXML(oReader);
					m_pictureStackUnit = pNewElem;
				}
			}
		}
		void CT_PictureOptions::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_applyToFront)
			{
                std::wstring sNodeName = _T("c:applyToFront");
                m_applyToFront->toXML(sNodeName, writer);
			}
			if(NULL != m_applyToSides)
			{
                std::wstring sNodeName = _T("c:applyToSides");
                m_applyToSides->toXML(sNodeName, writer);
			}
			if(NULL != m_applyToEnd)
			{
                std::wstring sNodeName = _T("c:applyToEnd");
                m_applyToEnd->toXML(sNodeName, writer);
			}
			if(NULL != m_pictureFormat)
			{
                std::wstring sNodeName = _T("c:pictureFormat");
                m_pictureFormat->toXML(sNodeName, writer);
			}
			if(NULL != m_pictureStackUnit)
			{
                std::wstring sNodeName = _T("c:pictureStackUnit");
                m_pictureStackUnit->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PictureOptions::getType(){return et_ct_pictureoptions;}
		CT_PictureFormat::CT_PictureFormat()
		{
			m_val = NULL;
		}
		CT_PictureFormat::~CT_PictureFormat()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_PictureFormat::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_PictureFormat::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_PictureFormat(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PictureFormat::getType(){return et_ct_pictureformat;}
		void CT_PictureFormat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_PictureFormat* pNewElem = new ST_PictureFormat;
					ST_PictureFormat eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_PictureFormat(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_PictureStackUnit::CT_PictureStackUnit()
		{
			m_val = NULL;
		}
		CT_PictureStackUnit::~CT_PictureStackUnit()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_PictureStackUnit::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_PictureStackUnit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrDouble(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PictureStackUnit::getType(){return et_ct_picturestackunit;}
		void CT_PictureStackUnit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					double* pNewElem = new double;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = std::stod(sVal.c_str());
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DLbls::CT_DLbls()
		{
			m_extLst = NULL;
		}
		CT_DLbls::~CT_DLbls()
		{
            for(size_t i = 0; i < m_dLbl.size(); ++i)
				delete m_dLbl[i];
            m_dLbl.clear();
            for(size_t i = 0, length = m_Items.size(), length2 = m_ItemsElementName0.size(); i < length && i < length2; ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
            m_Items.clear();
            for(size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
            m_ItemsElementName0.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DLbls::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("dLbl") == sName)
				{
					CT_DLbl* pNewElem = new CT_DLbl;
					pNewElem->fromXML(oReader);
                    m_dLbl.push_back(pNewElem);
				}
				else if(_T("dLblPos") == sName)
				{
					CT_DLblPos* pNewElem = new CT_DLblPos;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3DLBLPOS;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3DELETE;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("leaderLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3LEADERLINES;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3NUMFMT;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("separator") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SEPARATOR;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showBubbleSize") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWBUBBLESIZE;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showCatName") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWCATNAME;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showLeaderLines") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWLEADERLINES;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showLegendKey") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWLEGENDKEY;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showPercent") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWPERCENT;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showSerName") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWSERNAME;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showVal") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWVAL;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("spPr") == sName)
				{
					PPTX::Logic::SpPr* pNewElem = new PPTX::Logic::SpPr;
					pNewElem->fromXML(oReader);
                    m_Items.push_back(pNewElem);

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SPPR;
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("txPr") == sName)
				{
					PPTX::Logic::TxBody* pNewElem = new PPTX::Logic::TxBody;
					pNewElem->fromXML(oReader);
                    m_Items.push_back(pNewElem);

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3TXPR;
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DLbls::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_dLbl.size(); ++i)
			{
				CT_DLbl* pElem = m_dLbl[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dLbl");
                    pElem->toXML(sNodeName, writer);
				}
			}
            for(size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType3 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i]);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DLbls::getType(){return et_ct_dlbls;}
		void CT_DLbls::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType3 eType, void* pVal) const{
			switch(eType)
			{
			case itemschoicetype3DLBLPOS:
				{
					CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:dLblPos");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3DELETE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:delete");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3LEADERLINES:
				{
					CT_ChartLines* pTypeVal = static_cast<CT_ChartLines*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:leaderLines");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3NUMFMT:
				{
					CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:numFmt");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SEPARATOR:
				{
					std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
					if(NULL != pTypeVal)
					{
						if (true == bIsAttribute)
						{
							WritingStringAttrEncodeXmlString(L"c:separator", *pTypeVal);
						}
						else
						{
							if (std::wstring::npos != pTypeVal->find(' ') || std::wstring::npos != pTypeVal->find('\n'))
								writer.WriteString(_T("<c:separator xml:space=\"preserve\">"));
							else
								writer.WriteString(_T("<c:separator>"));
							writer.WriteEncodeXmlString(*pTypeVal);
							writer.WriteString(_T("</c:separator>"));
						}
					}
				}
				break;
			case itemschoicetype3SHOWBUBBLESIZE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showBubbleSize");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWCATNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showCatName");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWLEADERLINES:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showLeaderLines");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWLEGENDKEY:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showLegendKey");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWPERCENT:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showPercent");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWSERNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showSerName");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SHOWVAL:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showVal");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype3SPPR:
				{
					PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
					if(NULL != pTypeVal)
					{
						writer.WriteString(pTypeVal->toXML());
					}
				}
				break;
			case itemschoicetype3TXPR:
				{
					PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
					if(NULL != pTypeVal)
					{
						pTypeVal->m_name = L"c:txPr";
						writer.WriteString(pTypeVal->toXML());
					}
				}
				break;
			}
		}
		void CT_DLbls::toDelete(ItemsChoiceType3 eType, void* pVal){
			switch(eType)
			{
			case itemschoicetype3DLBLPOS:
				{
					CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3DELETE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3LEADERLINES:
				{
					CT_ChartLines* pTypeVal = static_cast<CT_ChartLines*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3NUMFMT:
				{
					CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SEPARATOR:
				{
					std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWBUBBLESIZE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWCATNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWLEADERLINES:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWLEGENDKEY:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWPERCENT:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWSERNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SHOWVAL:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3SPPR:
				{
					PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype3TXPR:
				{
					PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			}
		}
		CT_DLbl::CT_DLbl()
		{
			m_idx = NULL;
			m_extLst = NULL;
		}
		CT_DLbl::~CT_DLbl()
		{
			if(NULL != m_idx)
				delete m_idx;
			for(size_t i = 0; i < m_Items.size(), i < m_ItemsElementName0.size(); ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
            m_Items.clear();
            for(size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
            m_ItemsElementName0.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DLbl::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("dLblPos") == sName)
				{
					CT_DLblPos* pNewElem = new CT_DLblPos;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4DLBLPOS;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("delete") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4DELETE;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4LAYOUT;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4NUMFMT;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("separator") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SEPARATOR;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showBubbleSize") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWBUBBLESIZE;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showCatName") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWCATNAME;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showLegendKey") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWLEGENDKEY;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showPercent") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWPERCENT;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showSerName") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWSERNAME;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("showVal") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWVAL;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("spPr") == sName)
				{
					PPTX::Logic::SpPr* pNewElem = new PPTX::Logic::SpPr;
					pNewElem->fromXML(oReader);
                    m_Items.push_back(pNewElem);

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SPPR;
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("tx") == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);
					
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4TX;                    
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("txPr") == sName)
				{
					PPTX::Logic::TxBody* pNewElem = new PPTX::Logic::TxBody;
					pNewElem->fromXML(oReader);
                    m_Items.push_back(pNewElem);

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4TXPR;
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType4 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i]);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DLbl::getType(){return et_ct_dlbl;}
		void CT_DLbl::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType4 eType, void* pVal) const{
			switch(eType)
			{
			case itemschoicetype4DLBLPOS:
				{
					CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:dLblPos");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4DELETE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:delete");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4LAYOUT:
				{
					CT_Layout* pTypeVal = static_cast<CT_Layout*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:layout");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4NUMFMT:
				{
					CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:numFmt");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SEPARATOR:
				{
					std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
					if(NULL != pTypeVal)
					{
						if (true == bIsAttribute)
						{
							WritingStringAttrEncodeXmlString(L"c:separator", *pTypeVal);
						}
						else
						{
							if (std::wstring::npos != pTypeVal->find(' ') || std::wstring::npos != pTypeVal->find('\n'))
								writer.WriteString(_T("<c:separator xml:space=\"preserve\">"));
							else
								writer.WriteString(_T("<c:separator>"));
							writer.WriteEncodeXmlString(*pTypeVal);
							writer.WriteString(_T("</c:separator>"));
						}
					}
				}
				break;
			case itemschoicetype4SHOWBUBBLESIZE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showBubbleSize");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SHOWCATNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showCatName");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SHOWLEGENDKEY:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showLegendKey");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SHOWPERCENT:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showPercent");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SHOWSERNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showSerName");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SHOWVAL:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:showVal");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4SPPR:
				{
					PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
					if(NULL != pTypeVal)
					{
						writer.WriteString(pTypeVal->toXML());
					}
				}
				break;
			case itemschoicetype4TX:
				{
					CT_Tx* pTypeVal = static_cast<CT_Tx*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:tx");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype4TXPR:
				{
					PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
					if(NULL != pTypeVal)
					{
						pTypeVal->m_name = L"c:txPr";
						writer.WriteString(pTypeVal->toXML());
					}
				}
				break;
			}
		}
		void CT_DLbl::toDelete(ItemsChoiceType4 eType, void* pVal){
			switch(eType)
			{
			case itemschoicetype4DLBLPOS:
				{
					CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4DELETE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4LAYOUT:
				{
					CT_Layout* pTypeVal = static_cast<CT_Layout*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4NUMFMT:
				{
					CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SEPARATOR:
				{
					std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWBUBBLESIZE:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWCATNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWLEGENDKEY:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWPERCENT:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWSERNAME:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SHOWVAL:
				{
					CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4SPPR:
				{
					PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4TX:
				{
					CT_Tx* pTypeVal = static_cast<CT_Tx*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype4TXPR:
				{
					PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			}
		}
		CT_DLblPos::CT_DLblPos()
		{
			m_val = NULL;
		}
		CT_DLblPos::~CT_DLblPos()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_DLblPos::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_DLblPos::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_DLblPos(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_DLblPos::getType(){return et_ct_dlblpos;}
		void CT_DLblPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_DLblPos* pNewElem = new ST_DLblPos;
					ST_DLblPos eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_DLblPos(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Trendline::CT_Trendline()
		{
			m_name = NULL;
			m_trendlineType = NULL;
			m_order = NULL;
			m_period = NULL;
			m_forward = NULL;
			m_backward = NULL;
			m_intercept = NULL;
			m_dispRSqr = NULL;
			m_dispEq = NULL;
			m_trendlineLbl = NULL;
			m_extLst = NULL;
		}
		CT_Trendline::~CT_Trendline()
		{
			if(NULL != m_name)
				delete m_name;
			if(NULL != m_trendlineType)
				delete m_trendlineType;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_period)
				delete m_period;
			if(NULL != m_forward)
				delete m_forward;
			if(NULL != m_backward)
				delete m_backward;
			if(NULL != m_intercept)
				delete m_intercept;
			if(NULL != m_dispRSqr)
				delete m_dispRSqr;
			if(NULL != m_dispEq)
				delete m_dispEq;
			if(NULL != m_trendlineLbl)
				delete m_trendlineLbl;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Trendline::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("name") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_name = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("trendlineType") == sName)
				{
					CT_TrendlineType* pNewElem = new CT_TrendlineType;
					pNewElem->fromXML(oReader);
					m_trendlineType = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_Order* pNewElem = new CT_Order;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("period") == sName)
				{
					CT_Period* pNewElem = new CT_Period;
					pNewElem->fromXML(oReader);
					m_period = pNewElem;
				}
				else if(_T("forward") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_forward = pNewElem;
				}
				else if(_T("backward") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_backward = pNewElem;
				}
				else if(_T("intercept") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_intercept = pNewElem;
				}
				else if(_T("dispRSqr") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_dispRSqr = pNewElem;
				}
				else if(_T("dispEq") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_dispEq = pNewElem;
				}
				else if(_T("trendlineLbl") == sName)
				{
					CT_TrendlineLbl* pNewElem = new CT_TrendlineLbl;
					pNewElem->fromXML(oReader);
					m_trendlineLbl = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Trendline::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_name)
			{
				if (std::wstring::npos != m_name->find(' ') || std::wstring::npos != m_name->find('\n'))
					writer.WriteString(_T("<c:name xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:name>"));
				writer.WriteEncodeXmlString(*m_name);
				writer.WriteString(_T("</c:name>"));
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_trendlineType)
			{
                std::wstring sNodeName = _T("c:trendlineType");
                m_trendlineType->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_period)
			{
                std::wstring sNodeName = _T("c:period");
                m_period->toXML(sNodeName, writer);
			}
			if(NULL != m_forward)
			{
                std::wstring sNodeName = _T("c:forward");
                m_forward->toXML(sNodeName, writer);
			}
			if(NULL != m_backward)
			{
                std::wstring sNodeName = _T("c:backward");
                m_backward->toXML(sNodeName, writer);
			}
			if(NULL != m_intercept)
			{
                std::wstring sNodeName = _T("c:intercept");
                m_intercept->toXML(sNodeName, writer);
			}
			if(NULL != m_dispRSqr)
			{
                std::wstring sNodeName = _T("c:dispRSqr");
                m_dispRSqr->toXML(sNodeName, writer);
			}
			if(NULL != m_dispEq)
			{
                std::wstring sNodeName = _T("c:dispEq");
                m_dispEq->toXML(sNodeName, writer);
			}
			if(NULL != m_trendlineLbl)
			{
                std::wstring sNodeName = _T("c:trendlineLbl");
                m_trendlineLbl->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Trendline::getType(){return et_ct_trendline;}
		CT_TrendlineType::CT_TrendlineType()
		{
			/*m_val = new ST_TrendlineType;
			*m_val = st_trendlinetypeLINEAR;*/
			m_val = NULL;
		}
		CT_TrendlineType::~CT_TrendlineType()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_TrendlineType::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_TrendlineType::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_TrendlineType(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_TrendlineType::getType(){return et_ct_trendlinetype;}
		void CT_TrendlineType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_TrendlineType* pNewElem = new ST_TrendlineType;
					ST_TrendlineType eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_TrendlineType(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Order::CT_Order()
		{
			/*m_val = new unsigned char;
			std::wstring s_m_val("2");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_Order::~CT_Order()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Order::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Order::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Order::getType(){return et_ct_order;}
		void CT_Order::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned char* pNewElem = new unsigned char;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Period::CT_Period()
		{
			/*m_val = new unsigned long;
			std::wstring s_m_val("2");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_Period::~CT_Period()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Period::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Period::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Period::getType(){return et_ct_period;}
		void CT_Period::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_TrendlineLbl::CT_TrendlineLbl()
		{
			m_layout = NULL;
			m_tx = NULL;
			m_numFmt = NULL;
			m_extLst = NULL;
		}
		CT_TrendlineLbl::~CT_TrendlineLbl()
		{
			if(NULL != m_layout)
				delete m_layout;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_numFmt)
				delete m_numFmt;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_TrendlineLbl::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("numFmt") == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_TrendlineLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_layout)
			{
                std::wstring sNodeName = _T("c:layout");
                m_layout->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if(NULL != m_numFmt)
			{
                std::wstring sNodeName = _T("c:numFmt");
                m_numFmt->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_TrendlineLbl::getType(){return et_ct_trendlinelbl;}
		CT_ErrBars::CT_ErrBars()
		{
			m_errDir = NULL;
			m_errBarType = NULL;
			m_errValType = NULL;
			m_noEndCap = NULL;
			m_plus = NULL;
			m_minus = NULL;
			m_val = NULL;
			m_extLst = NULL;
		}
		CT_ErrBars::~CT_ErrBars()
		{
			if(NULL != m_errDir)
				delete m_errDir;
			if(NULL != m_errBarType)
				delete m_errBarType;
			if(NULL != m_errValType)
				delete m_errValType;
			if(NULL != m_noEndCap)
				delete m_noEndCap;
			if(NULL != m_plus)
				delete m_plus;
			if(NULL != m_minus)
				delete m_minus;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ErrBars::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("errDir") == sName)
				{
					CT_ErrDir* pNewElem = new CT_ErrDir;
					pNewElem->fromXML(oReader);
					m_errDir = pNewElem;
				}
				else if(_T("errBarType") == sName)
				{
					CT_ErrBarType* pNewElem = new CT_ErrBarType;
					pNewElem->fromXML(oReader);
					m_errBarType = pNewElem;
				}
				else if(_T("errValType") == sName)
				{
					CT_ErrValType* pNewElem = new CT_ErrValType;
					pNewElem->fromXML(oReader);
					m_errValType = pNewElem;
				}
				else if(_T("noEndCap") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_noEndCap = pNewElem;
				}
				else if(_T("plus") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_plus = pNewElem;
				}
				else if(_T("minus") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_minus = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ErrBars::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_errDir)
			{
                std::wstring sNodeName = _T("c:errDir");
                m_errDir->toXML(sNodeName, writer);
			}
			if(NULL != m_errBarType)
			{
                std::wstring sNodeName = _T("c:errBarType");
                m_errBarType->toXML(sNodeName, writer);
			}
			if(NULL != m_errValType)
			{
                std::wstring sNodeName = _T("c:errValType");
                m_errValType->toXML(sNodeName, writer);
			}
			if(NULL != m_noEndCap)
			{
                std::wstring sNodeName = _T("c:noEndCap");
                m_noEndCap->toXML(sNodeName, writer);
			}
			if(NULL != m_plus)
			{
                std::wstring sNodeName = _T("c:plus");
                m_plus->toXML(sNodeName, writer);
			}
			if(NULL != m_minus)
			{
                std::wstring sNodeName = _T("c:minus");
                m_minus->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ErrBars::getType(){return et_ct_errbars;}
		CT_ErrDir::CT_ErrDir()
		{
			m_val = NULL;
		}
		CT_ErrDir::~CT_ErrDir()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_ErrDir::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_ErrDir::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_ErrDir(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_ErrDir::getType(){return et_ct_errdir;}
		void CT_ErrDir::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_ErrDir* pNewElem = new ST_ErrDir;
					ST_ErrDir eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_ErrDir(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_ErrBarType::CT_ErrBarType()
		{
			/*m_val = new ST_ErrBarType;
			*m_val = st_errbartypeBOTH;*/
			m_val = NULL;
		}
		CT_ErrBarType::~CT_ErrBarType()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_ErrBarType::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_ErrBarType::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_ErrBarType(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_ErrBarType::getType(){return et_ct_errbartype;}
		void CT_ErrBarType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_ErrBarType* pNewElem = new ST_ErrBarType;
					ST_ErrBarType eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_ErrBarType(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_ErrValType::CT_ErrValType()
		{
			/*m_val = new ST_ErrValType;
			*m_val = st_errvaltypeFIXEDVAL;*/
			m_val = NULL;
		}
		CT_ErrValType::~CT_ErrValType()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_ErrValType::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_ErrValType::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_ErrValType(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_ErrValType::getType(){return et_ct_errvaltype;}
		void CT_ErrValType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_ErrValType* pNewElem = new ST_ErrValType;
					ST_ErrValType eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_ErrValType(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_NumDataSource::CT_NumDataSource()
		{
			m_numLit = NULL;
			m_numRef = NULL;
		}
		CT_NumDataSource::~CT_NumDataSource()
		{
			if(NULL != m_numLit)
				delete m_numLit;
			if(NULL != m_numRef)
				delete m_numRef;
		}
		void CT_NumDataSource::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("numLit") == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numLit = pNewElem;
				}
				else if(_T("numRef") == sName)
				{
					CT_NumRef* pNewElem = new CT_NumRef;
					pNewElem->fromXML(oReader);
					m_numRef = pNewElem;
				}
			}
		}
		void CT_NumDataSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_numLit)
			{
                std::wstring sNodeName = _T("c:numLit");
                m_numLit->toXML(sNodeName, writer);
			}
			if(NULL != m_numRef)
			{
                std::wstring sNodeName = _T("c:numRef");
                m_numRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumDataSource::getType(){return et_ct_numdatasource;}
		CT_NumData::CT_NumData()
		{
			m_formatCode = NULL;
			m_ptCount = NULL;
			m_extLst = NULL;
		}
		CT_NumData::~CT_NumData()
		{
			if(NULL != m_formatCode)
				delete m_formatCode;
			if(NULL != m_ptCount)
				delete m_ptCount;
            for(size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
            m_pt.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_NumData::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("formatCode") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_formatCode = pNewElem;
				}
				else if(_T("ptCount") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_ptCount = pNewElem;
				}
				else if(_T("pt") == sName)
				{
					CT_NumVal* pNewElem = new CT_NumVal;
					pNewElem->fromXML(oReader);
                    m_pt.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_NumData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_formatCode)
			{
				if (std::wstring::npos != m_formatCode->find(' ') || std::wstring::npos != m_formatCode->find('\n'))
					writer.WriteString(_T("<c:formatCode xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:formatCode>"));
				writer.WriteEncodeXmlString(*m_formatCode);
				writer.WriteString(_T("</c:formatCode>"));
			}
			if(NULL != m_ptCount)
			{
                std::wstring sNodeName = _T("c:ptCount");
                m_ptCount->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_NumVal* pElem = m_pt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:pt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumData::getType(){return et_ct_numdata;}
		CT_NumVal::CT_NumVal()
		{
			m_v = NULL;
			m_idx = NULL;
			m_formatCode = NULL;
		}
		CT_NumVal::~CT_NumVal()
		{
			if(NULL != m_v)
				delete m_v;
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_formatCode)
				delete m_formatCode;
		}
		void CT_NumVal::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("v") == sName)
					{
						std::wstring* pNewElem = new std::wstring;
						std::wstring sVal = oReader.GetText3();
						*pNewElem = sVal;
						m_v = pNewElem;
					}
				}
		}
		void CT_NumVal::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_idx)
			{
				WritingStringAttrInt(L"idx", *m_idx);
			}
			if(NULL != m_formatCode)
			{
				WritingStringAttrEncodeXmlString(L"formatCode", *m_formatCode);
			}
			writer.WriteString(L">");
			if(NULL != m_v)
			{
				if (std::wstring::npos != m_v->find(' ') || std::wstring::npos != m_v->find('\n'))
					writer.WriteString(_T("<c:v xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:v>"));
				writer.WriteEncodeXmlString(*m_v);
				writer.WriteString(_T("</c:v>"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumVal::getType(){return et_ct_numval;}
		void CT_NumVal::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("idx") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_idx = pNewElem;
				}
				else if(_T("formatCode") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_formatCode = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_NumRef::CT_NumRef()
		{
			m_f = NULL;
			m_numCache = NULL;
			m_extLst = NULL;
		}
		CT_NumRef::~CT_NumRef()
		{
			if(NULL != m_f)
				delete m_f;
			if(NULL != m_numCache)
				delete m_numCache;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_NumRef::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("f") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_f = pNewElem;
				}
				else if(_T("numCache") == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numCache = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_NumRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_f)
			{
				if (std::wstring::npos != m_f->find(' ') || std::wstring::npos != m_f->find('\n'))
					writer.WriteString(_T("<c:f xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:f>"));
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(_T("</c:f>"));
			}
			if(NULL != m_numCache)
			{
                std::wstring sNodeName = _T("c:numCache");
                m_numCache->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumRef::getType(){return et_ct_numref;}
		CT_AxDataSource::CT_AxDataSource()
		{
			m_multiLvlStrRef = NULL;
			m_numLit = NULL;
			m_numRef = NULL;
			m_strLit = NULL;
			m_strRef = NULL;
		}
		CT_AxDataSource::~CT_AxDataSource()
		{
			if(NULL != m_multiLvlStrRef)
				delete m_multiLvlStrRef;
			if(NULL != m_numLit)
				delete m_numLit;
			if(NULL != m_numRef)
				delete m_numRef;
			if(NULL != m_strLit)
				delete m_strLit;
			if(NULL != m_strRef)
				delete m_strRef;
		}
		void CT_AxDataSource::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("multiLvlStrRef") == sName)
				{
					CT_MultiLvlStrRef* pNewElem = new CT_MultiLvlStrRef;
					pNewElem->fromXML(oReader);
					m_multiLvlStrRef = pNewElem;
				}
				else if(_T("numLit") == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numLit = pNewElem;
				}
				else if(_T("numRef") == sName)
				{
					CT_NumRef* pNewElem = new CT_NumRef;
					pNewElem->fromXML(oReader);
					m_numRef = pNewElem;
				}
				else if(_T("strLit") == sName)
				{
					CT_StrData* pNewElem = new CT_StrData;
					pNewElem->fromXML(oReader);
					m_strLit = pNewElem;
				}
				else if(_T("strRef") == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
			}
		}
		void CT_AxDataSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_multiLvlStrRef)
			{
                std::wstring sNodeName = _T("c:multiLvlStrRef");
                m_multiLvlStrRef->toXML(sNodeName, writer);
			}
			if(NULL != m_numLit)
			{
                std::wstring sNodeName = _T("c:numLit");
                m_numLit->toXML(sNodeName, writer);
			}
			if(NULL != m_numRef)
			{
                std::wstring sNodeName = _T("c:numRef");
                m_numRef->toXML(sNodeName, writer);
			}
			if(NULL != m_strLit)
			{
                std::wstring sNodeName = _T("c:strLit");
                m_strLit->toXML(sNodeName, writer);
			}
			if(NULL != m_strRef)
			{
                std::wstring sNodeName = _T("c:strRef");
                m_strRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AxDataSource::getType(){return et_ct_axdatasource;}
		CT_MultiLvlStrRef::CT_MultiLvlStrRef()
		{
			m_f = NULL;
			m_multiLvlStrCache = NULL;
			m_extLst = NULL;
		}
		CT_MultiLvlStrRef::~CT_MultiLvlStrRef()
		{
			if(NULL != m_f)
				delete m_f;
			if(NULL != m_multiLvlStrCache)
				delete m_multiLvlStrCache;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_MultiLvlStrRef::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("f") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_f = pNewElem;
				}
				else if(_T("multiLvlStrCache") == sName)
				{
					CT_MultiLvlStrData* pNewElem = new CT_MultiLvlStrData;
					pNewElem->fromXML(oReader);
					m_multiLvlStrCache = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_MultiLvlStrRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_f)
			{
				if (std::wstring::npos != m_f->find(' ') || std::wstring::npos != m_f->find('\n'))
					writer.WriteString(_T("<c:f xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:f>"));
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(_T("</c:f>"));
			}
			if(NULL != m_multiLvlStrCache)
			{
                std::wstring sNodeName = _T("c:multiLvlStrCache");
                m_multiLvlStrCache->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_MultiLvlStrRef::getType(){return et_ct_multilvlstrref;}
		CT_lvl::CT_lvl()
		{
		}
		CT_lvl::~CT_lvl()
		{
            for(size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
            m_pt.clear();
		}
		void CT_lvl::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("pt") == sName)
				{
					CT_StrVal* pNewElem = new CT_StrVal;
					pNewElem->fromXML(oReader);
                    m_pt.push_back(pNewElem);
				}
			}
		}
		void CT_lvl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_StrVal* pElem = m_pt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:pt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_lvl::getType(){return et_ct_lvl;}
		CT_MultiLvlStrData::CT_MultiLvlStrData()
		{
			m_ptCount = NULL;
			m_extLst = NULL;
		}
		CT_MultiLvlStrData::~CT_MultiLvlStrData()
		{
			if(NULL != m_ptCount)
				delete m_ptCount;
            for(size_t i = 0; i < m_lvl.size(); ++i)
				delete m_lvl[i];
            m_lvl.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_MultiLvlStrData::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("ptCount") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_ptCount = pNewElem;
				}
				else if(_T("lvl") == sName)
				{
					CT_lvl* pNewElem = new CT_lvl;
					pNewElem->fromXML(oReader);
                    m_lvl.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_MultiLvlStrData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_ptCount)
			{
                std::wstring sNodeName = _T("c:ptCount");
                m_ptCount->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_lvl.size(); ++i)
			{
				CT_lvl* pElem = m_lvl[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:lvl");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_MultiLvlStrData::getType(){return et_ct_multilvlstrdata;}
		CT_BubbleChart::CT_BubbleChart()
		{
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_bubble3D = NULL;
			m_bubbleScale = NULL;
			m_showNegBubbles = NULL;
			m_sizeRepresents = NULL;
			m_extLst = NULL;
		}
		CT_BubbleChart::~CT_BubbleChart()
		{
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_bubble3D)
				delete m_bubble3D;
			if(NULL != m_bubbleScale)
				delete m_bubbleScale;
			if(NULL != m_showNegBubbles)
				delete m_showNegBubbles;
			if(NULL != m_sizeRepresents)
				delete m_sizeRepresents;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_BubbleChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_BubbleSer* pNewElem = new CT_BubbleSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("bubble3D") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_bubble3D = pNewElem;
				}
				else if(_T("bubbleScale") == sName)
				{
					CT_BubbleScale* pNewElem = new CT_BubbleScale;
					pNewElem->fromXML(oReader);
					m_bubbleScale = pNewElem;
				}
				else if(_T("showNegBubbles") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showNegBubbles = pNewElem;
				}
				else if(_T("sizeRepresents") == sName)
				{
					CT_SizeRepresents* pNewElem = new CT_SizeRepresents;
					pNewElem->fromXML(oReader);
					m_sizeRepresents = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_BubbleChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BubbleSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_bubble3D)
			{
                std::wstring sNodeName = _T("c:bubble3D");
                m_bubble3D->toXML(sNodeName, writer);
			}
			if(NULL != m_bubbleScale)
			{
                std::wstring sNodeName = _T("c:bubbleScale");
                m_bubbleScale->toXML(sNodeName, writer);
			}
			if(NULL != m_showNegBubbles)
			{
                std::wstring sNodeName = _T("c:showNegBubbles");
                m_showNegBubbles->toXML(sNodeName, writer);
			}
			if(NULL != m_sizeRepresents)
			{
                std::wstring sNodeName = _T("c:sizeRepresents");
                m_sizeRepresents->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BubbleChart::getType(){return et_ct_bubblechart;}
		CT_bandFmts::CT_bandFmts()
		{
		}
		CT_bandFmts::~CT_bandFmts()
		{
            for(size_t i = 0; i < m_bandFmt.size(); ++i)
				delete m_bandFmt[i];
            m_bandFmt.clear();
		}
		void CT_bandFmts::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("bandFmt") == sName)
				{
					CT_BandFmt* pNewElem = new CT_BandFmt;
					pNewElem->fromXML(oReader);
                    m_bandFmt.push_back(pNewElem);
				}
			}
		}
		void CT_bandFmts::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_bandFmt.size(); ++i)
			{
				CT_BandFmt* pElem = m_bandFmt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:bandFmt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_bandFmts::getType(){return et_ct_bandfmts;}
		CT_Surface3DChart::CT_Surface3DChart()
		{
			m_wireframe = NULL;
			m_bandFmts = NULL;
			m_extLst = NULL;
		}
		CT_Surface3DChart::~CT_Surface3DChart()
		{
			if(NULL != m_wireframe)
				delete m_wireframe;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_bandFmts)
				delete m_bandFmts;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Surface3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("wireframe") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_wireframe = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("bandFmts") == sName)
				{
					CT_bandFmts* pNewElem = new CT_bandFmts;
					pNewElem->fromXML(oReader);
					m_bandFmts = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Surface3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_wireframe)
			{
                std::wstring sNodeName = _T("c:wireframe");
                m_wireframe->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_SurfaceSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_bandFmts)
			{
                std::wstring sNodeName = _T("c:bandFmts");
                m_bandFmts->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Surface3DChart::getType(){return et_ct_surface3dchart;}
		CT_SurfaceSer::CT_SurfaceSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_extLst = NULL;
		}
		CT_SurfaceSer::~CT_SurfaceSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_SurfaceSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_SurfaceSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SurfaceSer::getType(){return et_ct_surfaceser;}
		CT_BandFmt::CT_BandFmt()
		{
			m_idx = NULL;
		}
		CT_BandFmt::~CT_BandFmt()
		{
			if(NULL != m_idx)
				delete m_idx;
		}
		void CT_BandFmt::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
			}
		}
		void CT_BandFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BandFmt::getType(){return et_ct_bandfmt;}
		CT_SurfaceChart::CT_SurfaceChart()
		{
			m_wireframe = NULL;
			m_bandFmts = NULL;
			m_extLst = NULL;
		}
		CT_SurfaceChart::~CT_SurfaceChart()
		{
			if(NULL != m_wireframe)
				delete m_wireframe;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_bandFmts)
				delete m_bandFmts;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_SurfaceChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("wireframe") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_wireframe = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("bandFmts") == sName)
				{
					CT_bandFmts* pNewElem = new CT_bandFmts;
					pNewElem->fromXML(oReader);
					m_bandFmts = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_SurfaceChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_wireframe)
			{
                std::wstring sNodeName = _T("c:wireframe");
                m_wireframe->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_SurfaceSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_bandFmts)
			{
                std::wstring sNodeName = _T("c:bandFmts");
                m_bandFmts->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SurfaceChart::getType(){return et_ct_surfacechart;}
		CT_SecondPieSize::CT_SecondPieSize()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("75%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_SecondPieSize::~CT_SecondPieSize()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_SecondPieSize::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_SecondPieSize::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_SecondPieSize::getType(){return et_ct_secondpiesize;}
		void CT_SecondPieSize::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_SplitType::CT_SplitType()
		{
			/*m_val = new ST_SplitType;
			*m_val = st_splittypeAUTO;*/
			m_val = NULL;
		}
		CT_SplitType::~CT_SplitType()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_SplitType::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_SplitType::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_SplitType(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_SplitType::getType(){return et_ct_splittype;}
		void CT_SplitType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_SplitType* pNewElem = new ST_SplitType;
					ST_SplitType eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_SplitType(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_OfPieType::CT_OfPieType()
		{
			/*m_val = new ST_OfPieType;
			*m_val = st_ofpietypePIE;*/
			m_val = NULL;
		}
		CT_OfPieType::~CT_OfPieType()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_OfPieType::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_OfPieType::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_OfPieType(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_OfPieType::getType(){return et_ct_ofpietype;}
		void CT_OfPieType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_OfPieType* pNewElem = new ST_OfPieType;
					ST_OfPieType eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_OfPieType(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_custSplit::CT_custSplit()
		{
		}
		CT_custSplit::~CT_custSplit()
		{
            for(size_t i = 0; i < m_secondPiePt.size(); ++i)
				delete m_secondPiePt[i];
            m_secondPiePt.clear();
		}
		void CT_custSplit::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("secondPiePt") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_secondPiePt.push_back(pNewElem);
				}
			}
		}
		void CT_custSplit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_secondPiePt.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_secondPiePt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:secondPiePt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_custSplit::getType(){return et_ct_custsplit;}
		CT_OfPieChart::CT_OfPieChart()
		{
			m_ofPieType = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_gapWidth = NULL;
			m_splitType = NULL;
			m_splitPos = NULL;
			m_custSplit = NULL;
			m_secondPieSize = NULL;
			m_extLst = NULL;
		}
		CT_OfPieChart::~CT_OfPieChart()
		{
			if(NULL != m_ofPieType)
				delete m_ofPieType;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_gapWidth)
				delete m_gapWidth;
			if(NULL != m_splitType)
				delete m_splitType;
			if(NULL != m_splitPos)
				delete m_splitPos;
			if(NULL != m_custSplit)
				delete m_custSplit;
			if(NULL != m_secondPieSize)
				delete m_secondPieSize;
            for(size_t i = 0; i < m_serLines.size(); ++i)
				delete m_serLines[i];
            m_serLines.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_OfPieChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("ofPieType") == sName)
				{
					CT_OfPieType* pNewElem = new CT_OfPieType;
					pNewElem->fromXML(oReader);
					m_ofPieType = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("gapWidth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapWidth = pNewElem;
				}
				else if(_T("splitType") == sName)
				{
					CT_SplitType* pNewElem = new CT_SplitType;
					pNewElem->fromXML(oReader);
					m_splitType = pNewElem;
				}
				else if(_T("splitPos") == sName)
				{
					CT_Double* pNewElem = new CT_Double;
					pNewElem->fromXML(oReader);
					m_splitPos = pNewElem;
				}
				else if(_T("custSplit") == sName)
				{
					CT_custSplit* pNewElem = new CT_custSplit;
					pNewElem->fromXML(oReader);
					m_custSplit = pNewElem;
				}
				else if(_T("secondPieSize") == sName)
				{
					CT_SecondPieSize* pNewElem = new CT_SecondPieSize;
					pNewElem->fromXML(oReader);
					m_secondPieSize = pNewElem;
				}
				else if(_T("serLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
                    m_serLines.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_OfPieChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_ofPieType)
			{
                std::wstring sNodeName = _T("c:ofPieType");
                m_ofPieType->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_gapWidth)
			{
                std::wstring sNodeName = _T("c:gapWidth");
                m_gapWidth->toXML(sNodeName, writer);
			}
			if(NULL != m_splitType)
			{
                std::wstring sNodeName = _T("c:splitType");
                m_splitType->toXML(sNodeName, writer);
			}
			if(NULL != m_splitPos)
			{
                std::wstring sNodeName = _T("c:splitPos");
                m_splitPos->toXML(sNodeName, writer);
			}
			if(NULL != m_custSplit)
			{
                std::wstring sNodeName = _T("c:custSplit");
                m_custSplit->toXML(sNodeName, writer);
			}
			if(NULL != m_secondPieSize)
			{
                std::wstring sNodeName = _T("c:secondPieSize");
                m_secondPieSize->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_serLines.size(); ++i)
			{
				CT_ChartLines* pElem = m_serLines[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:serLines");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_OfPieChart::getType(){return et_ct_ofpiechart;}
		CT_PieSer::CT_PieSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_explosion = NULL;
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_extLst = NULL;
		}
		CT_PieSer::~CT_PieSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_explosion)
				delete m_explosion;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_PieSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("explosion") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_explosion = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_PieSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_explosion)
			{
                std::wstring sNodeName = _T("c:explosion");
                m_explosion->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PieSer::getType(){return et_ct_pieser;}
		CT_GapAmount::CT_GapAmount()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("150%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_GapAmount::~CT_GapAmount()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_GapAmount::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_GapAmount::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_GapAmount::getType(){return et_ct_gapamount;}
		void CT_GapAmount::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Bar3DChart::CT_Bar3DChart()
		{
			m_barDir = NULL;
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_gapWidth = NULL;
			m_gapDepth = NULL;
			m_shape = NULL;
			m_extLst = NULL;
		}
		CT_Bar3DChart::~CT_Bar3DChart()
		{
			if(NULL != m_barDir)
				delete m_barDir;
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_gapWidth)
				delete m_gapWidth;
			if(NULL != m_gapDepth)
				delete m_gapDepth;
			if(NULL != m_shape)
				delete m_shape;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Bar3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("barDir") == sName)
				{
					CT_BarDir* pNewElem = new CT_BarDir;
					pNewElem->fromXML(oReader);
					m_barDir = pNewElem;
				}
				else if(_T("grouping") == sName)
				{
					CT_BarGrouping* pNewElem = new CT_BarGrouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_BarSer* pNewElem = new CT_BarSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("gapWidth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapWidth = pNewElem;
				}
				else if(_T("gapDepth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapDepth = pNewElem;
				}
				else if(_T("shape") == sName)
				{
					CT_Shape* pNewElem = new CT_Shape;
					pNewElem->fromXML(oReader);
					m_shape = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Bar3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_barDir)
			{
                std::wstring sNodeName = _T("c:barDir");
                m_barDir->toXML(sNodeName, writer);
			}
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BarSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_gapWidth)
			{
                std::wstring sNodeName = _T("c:gapWidth");
                m_gapWidth->toXML(sNodeName, writer);
			}
			if(NULL != m_gapDepth)
			{
                std::wstring sNodeName = _T("c:gapDepth");
                m_gapDepth->toXML(sNodeName, writer);
			}
			if(NULL != m_shape)
			{
                std::wstring sNodeName = _T("c:shape");
                m_shape->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Bar3DChart::getType(){return et_ct_bar3dchart;}
		CT_BarDir::CT_BarDir()
		{
			/*m_val = new ST_BarDir;
			*m_val = st_bardirCOL;*/
			m_val = NULL;
		}
		CT_BarDir::~CT_BarDir()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_BarDir::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_BarDir::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_BarDir(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_BarDir::getType(){return et_ct_bardir;}
		void CT_BarDir::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_BarDir* pNewElem = new ST_BarDir;
					ST_BarDir eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_BarDir(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_BarGrouping::CT_BarGrouping()
		{
			/*m_val = new ST_BarGrouping;
			*m_val = st_bargroupingCLUSTERED;*/
			m_val = NULL;
		}
		CT_BarGrouping::~CT_BarGrouping()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_BarGrouping::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_BarGrouping::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_BarGrouping(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_BarGrouping::getType(){return et_ct_bargrouping;}
		void CT_BarGrouping::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_BarGrouping* pNewElem = new ST_BarGrouping;
					ST_BarGrouping eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_BarGrouping(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_BarSer::CT_BarSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_invertIfNegative = NULL;
			m_pictureOptions = NULL;
			m_dLbls = NULL;
			m_errBars = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_shape = NULL;
			m_extLst = NULL;
		}
		CT_BarSer::~CT_BarSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_invertIfNegative)
				delete m_invertIfNegative;
			if(NULL != m_pictureOptions)
				delete m_pictureOptions;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
            m_trendline.clear();
			if(NULL != m_errBars)
				delete m_errBars;
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_shape)
				delete m_shape;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_BarSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("invertIfNegative") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_invertIfNegative = pNewElem;
				}
				else if(_T("pictureOptions") == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("trendline") == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
                    m_trendline.push_back(pNewElem);
				}
				else if(_T("errBars") == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars = pNewElem;
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("shape") == sName)
				{
					CT_Shape* pNewElem = new CT_Shape;
					pNewElem->fromXML(oReader);
					m_shape = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_BarSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_invertIfNegative)
			{
                std::wstring sNodeName = _T("c:invertIfNegative");
                m_invertIfNegative->toXML(sNodeName, writer);
			}
			if(NULL != m_pictureOptions)
			{
                std::wstring sNodeName = _T("c:pictureOptions");
                m_pictureOptions->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:trendline");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_errBars)
			{
                std::wstring sNodeName = _T("c:errBars");
                m_errBars->toXML(sNodeName, writer);
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_shape)
			{
                std::wstring sNodeName = _T("c:shape");
                m_shape->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BarSer::getType(){return et_ct_barser;}
		CT_Shape::CT_Shape()
		{
			/*m_val = new ST_Shape;
			*m_val = st_shapeBOX;*/
			m_val = NULL;
		}
		CT_Shape::~CT_Shape()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Shape::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Shape::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_Shape(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Shape::getType(){return et_ct_shape;}
		void CT_Shape::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_Shape* pNewElem = new ST_Shape;
					ST_Shape eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_Shape(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Overlap::CT_Overlap()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("0%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_Overlap::~CT_Overlap()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Overlap::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Overlap::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Overlap::getType(){return et_ct_overlap;}
		void CT_Overlap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_BarChart::CT_BarChart()
		{
			m_barDir = NULL;
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_gapWidth = NULL;
			m_overlap = NULL;
			m_extLst = NULL;
		}
		CT_BarChart::~CT_BarChart()
		{
			if(NULL != m_barDir)
				delete m_barDir;
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_gapWidth)
				delete m_gapWidth;
			if(NULL != m_overlap)
				delete m_overlap;
            for(size_t i = 0; i < m_serLines.size(); ++i)
				delete m_serLines[i];
            m_serLines.clear();
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_BarChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("barDir") == sName)
				{
					CT_BarDir* pNewElem = new CT_BarDir;
					pNewElem->fromXML(oReader);
					m_barDir = pNewElem;
				}
				else if(_T("grouping") == sName)
				{
					CT_BarGrouping* pNewElem = new CT_BarGrouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_BarSer* pNewElem = new CT_BarSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("gapWidth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapWidth = pNewElem;
				}
				else if(_T("overlap") == sName)
				{
					CT_Overlap* pNewElem = new CT_Overlap;
					pNewElem->fromXML(oReader);
					m_overlap = pNewElem;
				}
				else if(_T("serLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
                    m_serLines.push_back(pNewElem);
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_BarChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_barDir)
			{
                std::wstring sNodeName = _T("c:barDir");
                m_barDir->toXML(sNodeName, writer);
			}
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BarSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_gapWidth)
			{
                std::wstring sNodeName = _T("c:gapWidth");
                m_gapWidth->toXML(sNodeName, writer);
			}
			if(NULL != m_overlap)
			{
                std::wstring sNodeName = _T("c:overlap");
                m_overlap->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_serLines.size(); ++i)
			{
				CT_ChartLines* pElem = m_serLines[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:serLines");
                    pElem->toXML(sNodeName, writer);
				}
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BarChart::getType(){return et_ct_barchart;}
		CT_HoleSize::CT_HoleSize()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("10%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_HoleSize::~CT_HoleSize()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_HoleSize::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_HoleSize::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_HoleSize::getType(){return et_ct_holesize;}
		void CT_HoleSize::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DoughnutChart::CT_DoughnutChart()
		{
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_firstSliceAng = NULL;
			m_holeSize = NULL;
			m_extLst = NULL;
		}
		CT_DoughnutChart::~CT_DoughnutChart()
		{
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_firstSliceAng)
				delete m_firstSliceAng;
			if(NULL != m_holeSize)
				delete m_holeSize;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_DoughnutChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("firstSliceAng") == sName)
				{
					CT_FirstSliceAng* pNewElem = new CT_FirstSliceAng;
					pNewElem->fromXML(oReader);
					m_firstSliceAng = pNewElem;
				}
				else if(_T("holeSize") == sName)
				{
					CT_HoleSize* pNewElem = new CT_HoleSize;
					pNewElem->fromXML(oReader);
					m_holeSize = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_DoughnutChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_firstSliceAng)
			{
                std::wstring sNodeName = _T("c:firstSliceAng");
                m_firstSliceAng->toXML(sNodeName, writer);
			}
			if(NULL != m_holeSize)
			{
                std::wstring sNodeName = _T("c:holeSize");
                m_holeSize->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DoughnutChart::getType(){return et_ct_doughnutchart;}
		CT_FirstSliceAng::CT_FirstSliceAng()
		{
			/*m_val = new unsigned long;
			std::wstring s_m_val("0");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_FirstSliceAng::~CT_FirstSliceAng()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_FirstSliceAng::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_FirstSliceAng::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_FirstSliceAng::getType(){return et_ct_firstsliceang;}
		void CT_FirstSliceAng::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Pie3DChart::CT_Pie3DChart()
		{
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_extLst = NULL;
		}
		CT_Pie3DChart::~CT_Pie3DChart()
		{
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Pie3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Pie3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Pie3DChart::getType(){return et_ct_pie3dchart;}
		CT_PieChart::CT_PieChart()
		{
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_firstSliceAng = NULL;
			m_extLst = NULL;
		}
		CT_PieChart::~CT_PieChart()
		{
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_firstSliceAng)
				delete m_firstSliceAng;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_PieChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("firstSliceAng") == sName)
				{
					CT_FirstSliceAng* pNewElem = new CT_FirstSliceAng;
					pNewElem->fromXML(oReader);
					m_firstSliceAng = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_PieChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_firstSliceAng)
			{
                std::wstring sNodeName = _T("c:firstSliceAng");
                m_firstSliceAng->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PieChart::getType(){return et_ct_piechart;}
		CT_ScatterSer::CT_ScatterSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_marker = NULL;
			m_dLbls = NULL;
			m_xVal = NULL;
			m_yVal = NULL;
			m_smooth = NULL;
			m_extLst = NULL;
		}
		CT_ScatterSer::~CT_ScatterSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_marker)
				delete m_marker;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
            m_trendline.clear();
            for(size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
            m_errBars.clear();
			if(NULL != m_xVal)
				delete m_xVal;
			if(NULL != m_yVal)
				delete m_yVal;
			if(NULL != m_smooth)
				delete m_smooth;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ScatterSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("marker") == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("trendline") == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
                    m_trendline.push_back(pNewElem);
				}
				else if(_T("errBars") == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
                    m_errBars.push_back(pNewElem);
				}
				else if(_T("xVal") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_xVal = pNewElem;
				}
				else if(_T("yVal") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_yVal = pNewElem;
				}
				else if(_T("smooth") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_smooth = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ScatterSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:trendline");
                    pElem->toXML(sNodeName, writer);
				}
			}
            for(size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:errBars");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_xVal)
			{
                std::wstring sNodeName = _T("c:xVal");
                m_xVal->toXML(sNodeName, writer);
			}
			if(NULL != m_yVal)
			{
                std::wstring sNodeName = _T("c:yVal");
                m_yVal->toXML(sNodeName, writer);
			}
			if(NULL != m_smooth)
			{
                std::wstring sNodeName = _T("c:smooth");
                m_smooth->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ScatterSer::getType(){return et_ct_scatterser;}
		CT_ScatterStyle::CT_ScatterStyle()
		{
			/*m_val = new ST_ScatterStyle;
			*m_val = st_scatterstyleMARKER;*/
			m_val = NULL;
		}
		CT_ScatterStyle::~CT_ScatterStyle()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_ScatterStyle::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_ScatterStyle::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_ScatterStyle(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_ScatterStyle::getType(){return et_ct_scatterstyle;}
		void CT_ScatterStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_ScatterStyle* pNewElem = new ST_ScatterStyle;
					ST_ScatterStyle eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_ScatterStyle(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_ScatterChart::CT_ScatterChart()
		{
			m_scatterStyle = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_extLst = NULL;
		}
		CT_ScatterChart::~CT_ScatterChart()
		{
			if(NULL != m_scatterStyle)
				delete m_scatterStyle;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_ScatterChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("scatterStyle") == sName)
				{
					CT_ScatterStyle* pNewElem = new CT_ScatterStyle;
					pNewElem->fromXML(oReader);
					m_scatterStyle = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_ScatterSer* pNewElem = new CT_ScatterSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_ScatterChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_scatterStyle)
			{
                std::wstring sNodeName = _T("c:scatterStyle");
                m_scatterStyle->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_ScatterSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ScatterChart::getType(){return et_ct_scatterchart;}
		CT_RadarSer::CT_RadarSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_marker = NULL;
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_extLst = NULL;
		}
		CT_RadarSer::~CT_RadarSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_marker)
				delete m_marker;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_RadarSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("marker") == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_RadarSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_RadarSer::getType(){return et_ct_radarser;}
		CT_RadarStyle::CT_RadarStyle()
		{
			/*m_val = new ST_RadarStyle;
			*m_val = st_radarstyleSTANDARD;*/
			m_val = NULL;
		}
		CT_RadarStyle::~CT_RadarStyle()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_RadarStyle::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_RadarStyle::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_RadarStyle(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_RadarStyle::getType(){return et_ct_radarstyle;}
		void CT_RadarStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_RadarStyle* pNewElem = new ST_RadarStyle;
					ST_RadarStyle eVal;
                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_RadarStyle(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_RadarChart::CT_RadarChart()
		{
			m_radarStyle = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_extLst = NULL;
		}
		CT_RadarChart::~CT_RadarChart()
		{
			if(NULL != m_radarStyle)
				delete m_radarStyle;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_RadarChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("radarStyle") == sName)
				{
					CT_RadarStyle* pNewElem = new CT_RadarStyle;
					pNewElem->fromXML(oReader);
					m_radarStyle = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_RadarSer* pNewElem = new CT_RadarSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_RadarChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_radarStyle)
			{
                std::wstring sNodeName = _T("c:radarStyle");
                m_radarStyle->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_RadarSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_RadarChart::getType(){return et_ct_radarchart;}
		CT_StockChart::CT_StockChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_hiLowLines = NULL;
			m_upDownBars = NULL;
			m_extLst = NULL;
		}
		CT_StockChart::~CT_StockChart()
		{
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_dropLines)
				delete m_dropLines;
			if(NULL != m_hiLowLines)
				delete m_hiLowLines;
			if(NULL != m_upDownBars)
				delete m_upDownBars;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_StockChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("ser") == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("dropLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if(_T("hiLowLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_hiLowLines = pNewElem;
				}
				else if(_T("upDownBars") == sName)
				{
					CT_UpDownBars* pNewElem = new CT_UpDownBars;
					pNewElem->fromXML(oReader);
					m_upDownBars = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_StockChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_dropLines)
			{
                std::wstring sNodeName = _T("c:dropLines");
                m_dropLines->toXML(sNodeName, writer);
			}
			if(NULL != m_hiLowLines)
			{
                std::wstring sNodeName = _T("c:hiLowLines");
                m_hiLowLines->toXML(sNodeName, writer);
			}
			if(NULL != m_upDownBars)
			{
                std::wstring sNodeName = _T("c:upDownBars");
                m_upDownBars->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StockChart::getType(){return et_ct_stockchart;}
		CT_LineSer::CT_LineSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_marker = NULL;
			m_dLbls = NULL;
			m_errBars = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_smooth = NULL;
			m_extLst = NULL;
		}
		CT_LineSer::~CT_LineSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_marker)
				delete m_marker;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
            m_trendline.clear();
			if(NULL != m_errBars)
				delete m_errBars;
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_smooth)
				delete m_smooth;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_LineSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("marker") == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("trendline") == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
                    m_trendline.push_back(pNewElem);
				}
				else if(_T("errBars") == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars = pNewElem;
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("smooth") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_smooth = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_LineSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:trendline");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_errBars)
			{
                std::wstring sNodeName = _T("c:errBars");
                m_errBars->toXML(sNodeName, writer);
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_smooth)
			{
                std::wstring sNodeName = _T("c:smooth");
                m_smooth->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LineSer::getType(){return et_ct_lineser;}
		CT_UpDownBars::CT_UpDownBars()
		{
			m_gapWidth = NULL;
			m_upBars = NULL;
			m_downBars = NULL;
			m_extLst = NULL;
		}
		CT_UpDownBars::~CT_UpDownBars()
		{
			if(NULL != m_gapWidth)
				delete m_gapWidth;
			if(NULL != m_upBars)
				delete m_upBars;
			if(NULL != m_downBars)
				delete m_downBars;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_UpDownBars::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("gapWidth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapWidth = pNewElem;
				}
				else if(_T("upBars") == sName)
				{
					CT_UpDownBar* pNewElem = new CT_UpDownBar;
					pNewElem->fromXML(oReader);
					m_upBars = pNewElem;
				}
				else if(_T("downBars") == sName)
				{
					CT_UpDownBar* pNewElem = new CT_UpDownBar;
					pNewElem->fromXML(oReader);
					m_downBars = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_UpDownBars::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_gapWidth)
			{
                std::wstring sNodeName = _T("c:gapWidth");
                m_gapWidth->toXML(sNodeName, writer);
			}
			if(NULL != m_upBars)
			{
                std::wstring sNodeName = _T("c:upBars");
                m_upBars->toXML(sNodeName, writer);
			}
			if(NULL != m_downBars)
			{
                std::wstring sNodeName = _T("c:downBars");
                m_downBars->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_UpDownBars::getType(){return et_ct_updownbars;}
		CT_UpDownBar::CT_UpDownBar()
		{
		}
		CT_UpDownBar::~CT_UpDownBar()
		{
		}
		void CT_UpDownBar::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
			}
		}
		void CT_UpDownBar::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_UpDownBar::getType(){return et_ct_updownbar;}
		CT_Line3DChart::CT_Line3DChart()
		{
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_gapDepth = NULL;
			m_extLst = NULL;
		}
		CT_Line3DChart::~CT_Line3DChart()
		{
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_dropLines)
				delete m_dropLines;
			if(NULL != m_gapDepth)
				delete m_gapDepth;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Line3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("grouping") == sName)
				{
					CT_Grouping* pNewElem = new CT_Grouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("dropLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if(_T("gapDepth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapDepth = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Line3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_dropLines)
			{
                std::wstring sNodeName = _T("c:dropLines");
                m_dropLines->toXML(sNodeName, writer);
			}
			if(NULL != m_gapDepth)
			{
                std::wstring sNodeName = _T("c:gapDepth");
                m_gapDepth->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Line3DChart::getType(){return et_ct_line3dchart;}
		CT_Grouping::CT_Grouping()
		{
			/*m_val = new ST_Grouping;
			*m_val = st_groupingSTANDARD;*/
			m_val = NULL;
		}
		CT_Grouping::~CT_Grouping()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Grouping::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Grouping::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				std::wstring sEnumVal;
				if(ToXml_ST_Grouping(*m_val, sEnumVal))
				{
					WritingStringAttrString(L"val", sEnumVal);
				}
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Grouping::getType(){return et_ct_grouping;}
		void CT_Grouping::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					ST_Grouping* pNewElem = new ST_Grouping;
					ST_Grouping eVal;

                    std::wstring sNodeName = oReader.GetText();
                    if(FromXml_ST_Grouping(sNodeName, eVal))
						*pNewElem = eVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_LineChart::CT_LineChart()
		{
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_hiLowLines = NULL;
			m_upDownBars = NULL;
			m_marker = NULL;
			m_smooth = NULL;
			m_extLst = NULL;
		}
		CT_LineChart::~CT_LineChart()
		{
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_dropLines)
				delete m_dropLines;
			if(NULL != m_hiLowLines)
				delete m_hiLowLines;
			if(NULL != m_upDownBars)
				delete m_upDownBars;
			if(NULL != m_marker)
				delete m_marker;
			if(NULL != m_smooth)
				delete m_smooth;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_LineChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("grouping") == sName)
				{
					CT_Grouping* pNewElem = new CT_Grouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("dropLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if(_T("hiLowLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_hiLowLines = pNewElem;
				}
				else if(_T("upDownBars") == sName)
				{
					CT_UpDownBars* pNewElem = new CT_UpDownBars;
					pNewElem->fromXML(oReader);
					m_upDownBars = pNewElem;
				}
				else if(_T("marker") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("smooth") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_smooth = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_LineChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_dropLines)
			{
                std::wstring sNodeName = _T("c:dropLines");
                m_dropLines->toXML(sNodeName, writer);
			}
			if(NULL != m_hiLowLines)
			{
                std::wstring sNodeName = _T("c:hiLowLines");
                m_hiLowLines->toXML(sNodeName, writer);
			}
			if(NULL != m_upDownBars)
			{
                std::wstring sNodeName = _T("c:upDownBars");
                m_upDownBars->toXML(sNodeName, writer);
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
			if(NULL != m_smooth)
			{
                std::wstring sNodeName = _T("c:smooth");
                m_smooth->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LineChart::getType(){return et_ct_linechart;}
		CT_Area3DChart::CT_Area3DChart()
		{
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_gapDepth = NULL;
			m_extLst = NULL;
		}
		CT_Area3DChart::~CT_Area3DChart()
		{
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_dropLines)
				delete m_dropLines;
			if(NULL != m_gapDepth)
				delete m_gapDepth;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Area3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("grouping") == sName)
				{
					CT_Grouping* pNewElem = new CT_Grouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_AreaSer* pNewElem = new CT_AreaSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("dropLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if(_T("gapDepth") == sName)
				{
					CT_GapAmount* pNewElem = new CT_GapAmount;
					pNewElem->fromXML(oReader);
					m_gapDepth = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Area3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_AreaSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_dropLines)
			{
                std::wstring sNodeName = _T("c:dropLines");
                m_dropLines->toXML(sNodeName, writer);
			}
			if(NULL != m_gapDepth)
			{
                std::wstring sNodeName = _T("c:gapDepth");
                m_gapDepth->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Area3DChart::getType(){return et_ct_area3dchart;}
		CT_AreaSer::CT_AreaSer()
		{
			m_idx = NULL;
			m_order = NULL;
			m_tx = NULL;
			m_pictureOptions = NULL;
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
			m_extLst = NULL;
		}
		CT_AreaSer::~CT_AreaSer()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_order)
				delete m_order;
			if(NULL != m_tx)
				delete m_tx;
			if(NULL != m_pictureOptions)
				delete m_pictureOptions;
            for(size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
            m_dPt.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
            for(size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
            m_trendline.clear();
            for(size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
            m_errBars.clear();
			if(NULL != m_cat)
				delete m_cat;
			if(NULL != m_val)
				delete m_val;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_AreaSer::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("order") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_order = pNewElem;
				}
				else if(_T("tx") == sName)
				{
					CT_SerTx* pNewElem = new CT_SerTx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("pictureOptions") == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if(_T("dPt") == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
                    m_dPt.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("trendline") == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
                    m_trendline.push_back(pNewElem);
				}
				else if(_T("errBars") == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
                    m_errBars.push_back(pNewElem);
				}
				else if(_T("cat") == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if(_T("val") == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_AreaSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if(NULL != m_order)
			{
                std::wstring sNodeName = _T("c:order");
                m_order->toXML(sNodeName, writer);
			}
			if(NULL != m_tx)
			{
                std::wstring sNodeName = _T("c:tx");
                m_tx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_pictureOptions)
			{
                std::wstring sNodeName = _T("c:pictureOptions");
                m_pictureOptions->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:dPt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:trendline");
                    pElem->toXML(sNodeName, writer);
				}
			}
            for(size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:errBars");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_cat)
			{
                std::wstring sNodeName = _T("c:cat");
                m_cat->toXML(sNodeName, writer);
			}
			if(NULL != m_val)
			{
                std::wstring sNodeName = _T("c:val");
                m_val->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AreaSer::getType(){return et_ct_areaser;}
		CT_AreaChart::CT_AreaChart()
		{
			m_grouping = NULL;
			m_varyColors = NULL;
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_extLst = NULL;
		}
		CT_AreaChart::~CT_AreaChart()
		{
			if(NULL != m_grouping)
				delete m_grouping;
			if(NULL != m_varyColors)
				delete m_varyColors;
            for(size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
            m_ser.clear();
			if(NULL != m_dLbls)
				delete m_dLbls;
			if(NULL != m_dropLines)
				delete m_dropLines;
            for(size_t i = 0; i < m_axId.size(); ++i)
				delete m_axId[i];
            m_axId.clear();
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_AreaChart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("grouping") == sName)
				{
					CT_Grouping* pNewElem = new CT_Grouping;
					pNewElem->fromXML(oReader);
					m_grouping = pNewElem;
				}
				else if(_T("varyColors") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_varyColors = pNewElem;
				}
				else if(_T("ser") == sName)
				{
					CT_AreaSer* pNewElem = new CT_AreaSer;
					pNewElem->fromXML(oReader);
                    m_ser.push_back(pNewElem);
				}
				else if(_T("dLbls") == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if(_T("dropLines") == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if(_T("axId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
                    m_axId.push_back(pNewElem);
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_AreaChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_grouping)
			{
                std::wstring sNodeName = _T("c:grouping");
                m_grouping->toXML(sNodeName, writer);
			}
			if(NULL != m_varyColors)
			{
                std::wstring sNodeName = _T("c:varyColors");
                m_varyColors->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_AreaSer* pElem = m_ser[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:ser");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_dLbls)
			{
                std::wstring sNodeName = _T("c:dLbls");
                m_dLbls->toXML(sNodeName, writer);
			}
			if(NULL != m_dropLines)
			{
                std::wstring sNodeName = _T("c:dropLines");
                m_dropLines->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_axId.size(); ++i)
			{
				CT_UnsignedInt* pElem = m_axId[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:axId");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AreaChart::getType(){return et_ct_areachart;}
		CT_PlotArea::CT_PlotArea()
		{
			m_layout = NULL;
			m_dTable = NULL;
			m_extLst = NULL;
		}
		CT_PlotArea::~CT_PlotArea()
		{
			if(NULL != m_layout)
				delete m_layout;
			for(size_t i = 0; i < m_Items.size() && i < m_ItemsElementName0.size(); ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
            m_Items.clear();
            for(size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
            m_ItemsElementName0.clear();
			for(size_t i = 0; i < m_Items1.size() && i < m_ItemsElementName1.size(); ++i)
				toDelete(*m_ItemsElementName1[i], m_Items1[i]);
            m_Items1.clear();
            for(size_t i = 0; i < m_ItemsElementName1.size(); ++i)
				delete m_ItemsElementName1[i];
            m_ItemsElementName1.clear();
			if(NULL != m_dTable)
				delete m_dTable;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_PlotArea::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("layout") == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if(_T("area3DChart") == sName)
				{
					CT_Area3DChart* pNewElem = new CT_Area3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5AREA3DCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("areaChart") == sName)
				{
					CT_AreaChart* pNewElem = new CT_AreaChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5AREACHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("bar3DChart") == sName)
				{
					CT_Bar3DChart* pNewElem = new CT_Bar3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BAR3DCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("barChart") == sName)
				{
					CT_BarChart* pNewElem = new CT_BarChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BARCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("bubbleChart") == sName)
				{
					CT_BubbleChart* pNewElem = new CT_BubbleChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BUBBLECHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("doughnutChart") == sName)
				{
					CT_DoughnutChart* pNewElem = new CT_DoughnutChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5DOUGHNUTCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("line3DChart") == sName)
				{
					CT_Line3DChart* pNewElem = new CT_Line3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5LINE3DCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("lineChart") == sName)
				{
					CT_LineChart* pNewElem = new CT_LineChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5LINECHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("ofPieChart") == sName)
				{
					CT_OfPieChart* pNewElem = new CT_OfPieChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5OFPIECHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("pie3DChart") == sName)
				{
					CT_Pie3DChart* pNewElem = new CT_Pie3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5PIE3DCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("pieChart") == sName)
				{
					CT_PieChart* pNewElem = new CT_PieChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5PIECHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("radarChart") == sName)
				{
					CT_RadarChart* pNewElem = new CT_RadarChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5RADARCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("scatterChart") == sName)
				{
					CT_ScatterChart* pNewElem = new CT_ScatterChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SCATTERCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("stockChart") == sName)
				{
					CT_StockChart* pNewElem = new CT_StockChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5STOCKCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("surface3DChart") == sName)
				{
					CT_Surface3DChart* pNewElem = new CT_Surface3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SURFACE3DCHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("surfaceChart") == sName)
				{
					CT_SurfaceChart* pNewElem = new CT_SurfaceChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SURFACECHART;
                    m_Items.push_back(pNewElem);
                    m_ItemsElementName0.push_back(eElemtype);
				}
				else if(_T("catAx") == sName)
				{
					CT_CatAx* pNewElem = new CT_CatAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6CATAX;
                    m_Items1.push_back(pNewElem);
                    m_ItemsElementName1.push_back(eElemtype);
				}
				else if(_T("dateAx") == sName)
				{
					CT_DateAx* pNewElem = new CT_DateAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6DATEAX;
                    m_Items1.push_back(pNewElem);
                    m_ItemsElementName1.push_back(eElemtype);
				}
				else if(_T("serAx") == sName)
				{
					CT_SerAx* pNewElem = new CT_SerAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6SERAX;
                    m_Items1.push_back(pNewElem);
                    m_ItemsElementName1.push_back(eElemtype);
				}
				else if(_T("valAx") == sName)
				{
					CT_ValAx* pNewElem = new CT_ValAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6VALAX;
                    m_Items1.push_back(pNewElem);
                    m_ItemsElementName1.push_back(eElemtype);
				}
				else if(_T("dTable") == sName)
				{
					CT_DTable* pNewElem = new CT_DTable;
					pNewElem->fromXML(oReader);
					m_dTable = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_PlotArea::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_layout)
			{
                std::wstring sNodeName = _T("c:layout");
                m_layout->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType5 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i]);
			}
            for(size_t i = 0; i < m_Items1.size(); ++i)
			{
				ItemsChoiceType6 eType = *m_ItemsElementName1[i];
				toXML(writer, false, eType, m_Items1[i]);
			}
			if(NULL != m_dTable)
			{
                std::wstring sNodeName = _T("c:dTable");
                m_dTable->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PlotArea::getType(){return et_ct_PlotArea;}
		void CT_PlotArea::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType5 eType, void* pVal) const{
			switch(eType)
			{
			case itemschoicetype5AREA3DCHART:
				{
					CT_Area3DChart* pTypeVal = static_cast<CT_Area3DChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:area3DChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5AREACHART:
				{
					CT_AreaChart* pTypeVal = static_cast<CT_AreaChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:areaChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5BAR3DCHART:
				{
					CT_Bar3DChart* pTypeVal = static_cast<CT_Bar3DChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:bar3DChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5BARCHART:
				{
					CT_BarChart* pTypeVal = static_cast<CT_BarChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:barChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5BUBBLECHART:
				{
					CT_BubbleChart* pTypeVal = static_cast<CT_BubbleChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:bubbleChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5DOUGHNUTCHART:
				{
					CT_DoughnutChart* pTypeVal = static_cast<CT_DoughnutChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:doughnutChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5LINE3DCHART:
				{
					CT_Line3DChart* pTypeVal = static_cast<CT_Line3DChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:line3DChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5LINECHART:
				{
					CT_LineChart* pTypeVal = static_cast<CT_LineChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:lineChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5OFPIECHART:
				{
					CT_OfPieChart* pTypeVal = static_cast<CT_OfPieChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:ofPieChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5PIE3DCHART:
				{
					CT_Pie3DChart* pTypeVal = static_cast<CT_Pie3DChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:pie3DChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5PIECHART:
				{
					CT_PieChart* pTypeVal = static_cast<CT_PieChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:pieChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5RADARCHART:
				{
					CT_RadarChart* pTypeVal = static_cast<CT_RadarChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:radarChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5SCATTERCHART:
				{
					CT_ScatterChart* pTypeVal = static_cast<CT_ScatterChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:scatterChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5STOCKCHART:
				{
					CT_StockChart* pTypeVal = static_cast<CT_StockChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:stockChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5SURFACE3DCHART:
				{
					CT_Surface3DChart* pTypeVal = static_cast<CT_Surface3DChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:surface3DChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype5SURFACECHART:
				{
					CT_SurfaceChart* pTypeVal = static_cast<CT_SurfaceChart*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:surfaceChart");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			}
		}
		void CT_PlotArea::toDelete(ItemsChoiceType5 eType, void* pVal){
			switch(eType)
			{
			case itemschoicetype5AREA3DCHART:
				{
					CT_Area3DChart* pTypeVal = static_cast<CT_Area3DChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5AREACHART:
				{
					CT_AreaChart* pTypeVal = static_cast<CT_AreaChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5BAR3DCHART:
				{
					CT_Bar3DChart* pTypeVal = static_cast<CT_Bar3DChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5BARCHART:
				{
					CT_BarChart* pTypeVal = static_cast<CT_BarChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5BUBBLECHART:
				{
					CT_BubbleChart* pTypeVal = static_cast<CT_BubbleChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5DOUGHNUTCHART:
				{
					CT_DoughnutChart* pTypeVal = static_cast<CT_DoughnutChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5LINE3DCHART:
				{
					CT_Line3DChart* pTypeVal = static_cast<CT_Line3DChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5LINECHART:
				{
					CT_LineChart* pTypeVal = static_cast<CT_LineChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5OFPIECHART:
				{
					CT_OfPieChart* pTypeVal = static_cast<CT_OfPieChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5PIE3DCHART:
				{
					CT_Pie3DChart* pTypeVal = static_cast<CT_Pie3DChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5PIECHART:
				{
					CT_PieChart* pTypeVal = static_cast<CT_PieChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5RADARCHART:
				{
					CT_RadarChart* pTypeVal = static_cast<CT_RadarChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5SCATTERCHART:
				{
					CT_ScatterChart* pTypeVal = static_cast<CT_ScatterChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5STOCKCHART:
				{
					CT_StockChart* pTypeVal = static_cast<CT_StockChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5SURFACE3DCHART:
				{
					CT_Surface3DChart* pTypeVal = static_cast<CT_Surface3DChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype5SURFACECHART:
				{
					CT_SurfaceChart* pTypeVal = static_cast<CT_SurfaceChart*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			}
		}
		void CT_PlotArea::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType6 eType, void* pVal) const{
			switch(eType)
			{
			case itemschoicetype6CATAX:
				{
					CT_CatAx* pTypeVal = static_cast<CT_CatAx*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:catAx");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype6DATEAX:
				{
					CT_DateAx* pTypeVal = static_cast<CT_DateAx*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:dateAx");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype6SERAX:
				{
					CT_SerAx* pTypeVal = static_cast<CT_SerAx*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:serAx");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			case itemschoicetype6VALAX:
				{
					CT_ValAx* pTypeVal = static_cast<CT_ValAx*>(pVal);
					if(NULL != pTypeVal)
					{
						if(false == bIsAttribute)
                        {
                            std::wstring sNodeName = _T("c:valAx");
                            pTypeVal->toXML(sNodeName, writer);
                        }
					}
				}
				break;
			}
		}
		void CT_PlotArea::toDelete(ItemsChoiceType6 eType, void* pVal){
			switch(eType)
			{
			case itemschoicetype6CATAX:
				{
					CT_CatAx* pTypeVal = static_cast<CT_CatAx*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype6DATEAX:
				{
					CT_DateAx* pTypeVal = static_cast<CT_DateAx*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype6SERAX:
				{
					CT_SerAx* pTypeVal = static_cast<CT_SerAx*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			case itemschoicetype6VALAX:
				{
					CT_ValAx* pTypeVal = static_cast<CT_ValAx*>(pVal);
					RELEASEOBJECT(pTypeVal);
				}
				break;
			}
		}
		CT_Thickness::CT_Thickness()
		{
			m_val = NULL;
		}
		CT_Thickness::~CT_Thickness()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Thickness::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Thickness::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Thickness::getType(){return et_ct_thickness;}
		void CT_Thickness::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_Surface::CT_Surface()
		{
			m_thickness = NULL;
			m_pictureOptions = NULL;
			m_extLst = NULL;
		}
		CT_Surface::~CT_Surface()
		{
			if(NULL != m_thickness)
				delete m_thickness;
			if(NULL != m_pictureOptions)
				delete m_pictureOptions;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Surface::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("thickness") == sName)
				{
					CT_Thickness* pNewElem = new CT_Thickness;
					pNewElem->fromXML(oReader);
					m_thickness = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("pictureOptions") == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Surface::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_thickness)
			{
                std::wstring sNodeName = _T("c:thickness");
                m_thickness->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if(NULL != m_pictureOptions)
			{
                std::wstring sNodeName = _T("c:pictureOptions");
                m_pictureOptions->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Surface::getType(){return et_ct_surface;}
		CT_Perspective::CT_Perspective()
		{
			/*m_val = new unsigned char;
			std::wstring s_m_val("30");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_Perspective::~CT_Perspective()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Perspective::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_Perspective::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Perspective::getType(){return et_ct_perspective;}
		void CT_Perspective::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned char* pNewElem = new unsigned char;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_DepthPercent::CT_DepthPercent()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("100%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_DepthPercent::~CT_DepthPercent()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_DepthPercent::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_DepthPercent::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_DepthPercent::getType(){return et_ct_depthpercent;}
		void CT_DepthPercent::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_RotY::CT_RotY()
		{
			/*m_val = new unsigned long;
			std::wstring s_m_val("0");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_RotY::~CT_RotY()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_RotY::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_RotY::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_RotY::getType(){return et_ct_roty;}
		void CT_RotY::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned long* pNewElem = new unsigned long;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_HPercent::CT_HPercent()
		{
			/*m_val = new std::wstring;
			std::wstring s_m_val("100%");
			{
			*m_val = s_m_val;
			}*/
			m_val = NULL;
		}
		CT_HPercent::~CT_HPercent()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_HPercent::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_HPercent::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);;
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_HPercent::getType(){return et_ct_hpercent;}
		void CT_HPercent::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_RotX::CT_RotX()
		{
			/*m_val = new char;
			std::wstring s_m_val("0");
			{
            *m_val = XmlUtils::GetInteger(s_m_val);
			}*/
			m_val = NULL;
		}
		CT_RotX::~CT_RotX()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_RotX::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_RotX::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_RotX::getType(){return et_ct_rotx;}
		void CT_RotX::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					signed char* pNewElem = new signed char;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_View3D::CT_View3D()
		{
			m_rotX = NULL;
			m_hPercent = NULL;
			m_rotY = NULL;
			m_depthPercent = NULL;
			m_rAngAx = NULL;
			m_perspective = NULL;
			m_extLst = NULL;
		}
		CT_View3D::~CT_View3D()
		{
			if(NULL != m_rotX)
				delete m_rotX;
			if(NULL != m_hPercent)
				delete m_hPercent;
			if(NULL != m_rotY)
				delete m_rotY;
			if(NULL != m_depthPercent)
				delete m_depthPercent;
			if(NULL != m_rAngAx)
				delete m_rAngAx;
			if(NULL != m_perspective)
				delete m_perspective;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_View3D::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("rotX") == sName)
				{
					CT_RotX* pNewElem = new CT_RotX;
					pNewElem->fromXML(oReader);
					m_rotX = pNewElem;
				}
				else if(_T("hPercent") == sName)
				{
					CT_HPercent* pNewElem = new CT_HPercent;
					pNewElem->fromXML(oReader);
					m_hPercent = pNewElem;
				}
				else if(_T("rotY") == sName)
				{
					CT_RotY* pNewElem = new CT_RotY;
					pNewElem->fromXML(oReader);
					m_rotY = pNewElem;
				}
				else if(_T("depthPercent") == sName)
				{
					CT_DepthPercent* pNewElem = new CT_DepthPercent;
					pNewElem->fromXML(oReader);
					m_depthPercent = pNewElem;
				}
				else if(_T("rAngAx") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_rAngAx = pNewElem;
				}
				else if(_T("perspective") == sName)
				{
					CT_Perspective* pNewElem = new CT_Perspective;
					pNewElem->fromXML(oReader);
					m_perspective = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_View3D::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_rotX)
			{
                std::wstring sNodeName = _T("c:rotX");
                m_rotX->toXML(sNodeName, writer);
			}
			if(NULL != m_hPercent)
			{
                std::wstring sNodeName = _T("c:hPercent");
                m_hPercent->toXML(sNodeName, writer);
			}
			if(NULL != m_rotY)
			{
                std::wstring sNodeName = _T("c:rotY");
                m_rotY->toXML(sNodeName, writer);
			}
			if(NULL != m_depthPercent)
			{
                std::wstring sNodeName = _T("c:depthPercent");
                m_depthPercent->toXML(sNodeName, writer);
			}
			if(NULL != m_rAngAx)
			{
                std::wstring sNodeName = _T("c:rAngAx");
                m_rAngAx->toXML(sNodeName, writer);
			}
			if(NULL != m_perspective)
			{
                std::wstring sNodeName = _T("c:perspective");
                m_perspective->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_View3D::getType(){return et_ct_view3d;}
		CT_PivotFmt::CT_PivotFmt()
		{
			m_idx = NULL;
			m_marker = NULL;
			m_dLbl = NULL;
			m_extLst = NULL;
		}
		CT_PivotFmt::~CT_PivotFmt()
		{
			if(NULL != m_idx)
				delete m_idx;
			if(NULL != m_marker)
				delete m_marker;
			if(NULL != m_dLbl)
				delete m_dLbl;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_PivotFmt::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("idx") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_idx = pNewElem;
				}
				else if(_T("spPr") == sName)
				{
					m_oSpPr = oReader;
				}
				else if(_T("txPr") == sName)
				{
					m_oTxPr = oReader;
				}
				else if(_T("marker") == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if(_T("dLbl") == sName)
				{
					CT_DLbl* pNewElem = new CT_DLbl;
					pNewElem->fromXML(oReader);
					m_dLbl = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_PivotFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_idx)
			{
                std::wstring sNodeName = _T("c:idx");
                m_idx->toXML(sNodeName, writer);
			}
			if (m_oSpPr.IsInit())
			{
				writer.WriteString(m_oSpPr->toXML());
			}
			if (m_oTxPr.IsInit())
			{
				m_oTxPr->m_name = L"c:txPr";
				writer.WriteString(m_oTxPr->toXML());
			}
			if(NULL != m_marker)
			{
                std::wstring sNodeName = _T("c:marker");
                m_marker->toXML(sNodeName, writer);
			}
			if(NULL != m_dLbl)
			{
                std::wstring sNodeName = _T("c:dLbl");
                m_dLbl->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PivotFmt::getType(){return et_ct_pivotfmt;}
		CT_pivotFmts::CT_pivotFmts()
		{
		}
		CT_pivotFmts::~CT_pivotFmts()
		{
            for(size_t i = 0; i < m_pivotFmt.size(); ++i)
				delete m_pivotFmt[i];
            m_pivotFmt.clear();
		}
		void CT_pivotFmts::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("pivotFmt") == sName)
				{
					CT_PivotFmt* pNewElem = new CT_PivotFmt;
					pNewElem->fromXML(oReader);
                    m_pivotFmt.push_back(pNewElem);
				}
			}
		}
		void CT_pivotFmts::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_pivotFmt.size(); ++i)
			{
				CT_PivotFmt* pElem = m_pivotFmt[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:pivotFmt");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_pivotFmts::getType(){return et_ct_pivotfmts;}
		CT_Chart::CT_Chart()
		{
			m_title = NULL;
			m_autoTitleDeleted = NULL;
			m_pivotFmts = NULL;
			m_view3D = NULL;
			m_floor = NULL;
			m_sideWall = NULL;
			m_backWall = NULL;
			m_plotArea = NULL;
			m_legend = NULL;
			m_plotVisOnly = NULL;
			m_dispBlanksAs = NULL;
			m_showDLblsOverMax = NULL;
			m_extLst = NULL;
		}
		CT_Chart::~CT_Chart()
		{
			if(NULL != m_title)
				delete m_title;
			if(NULL != m_autoTitleDeleted)
				delete m_autoTitleDeleted;
			if(NULL != m_pivotFmts)
				delete m_pivotFmts;
			if(NULL != m_view3D)
				delete m_view3D;
			if(NULL != m_floor)
				delete m_floor;
			if(NULL != m_sideWall)
				delete m_sideWall;
			if(NULL != m_backWall)
				delete m_backWall;
			if(NULL != m_plotArea)
				delete m_plotArea;
			if(NULL != m_legend)
				delete m_legend;
			if(NULL != m_plotVisOnly)
				delete m_plotVisOnly;
			if(NULL != m_dispBlanksAs)
				delete m_dispBlanksAs;
			if(NULL != m_showDLblsOverMax)
				delete m_showDLblsOverMax;
			if(NULL != m_extLst)
				delete m_extLst;
		}
		void CT_Chart::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("title") == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if(_T("autoTitleDeleted") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_autoTitleDeleted = pNewElem;
				}
				else if(_T("pivotFmts") == sName)
				{
					CT_pivotFmts* pNewElem = new CT_pivotFmts;
					pNewElem->fromXML(oReader);
					m_pivotFmts = pNewElem;
				}
				else if(_T("view3D") == sName)
				{
					CT_View3D* pNewElem = new CT_View3D;
					pNewElem->fromXML(oReader);
					m_view3D = pNewElem;
				}
				else if(_T("floor") == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_floor = pNewElem;
				}
				else if(_T("sideWall") == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_sideWall = pNewElem;
				}
				else if(_T("backWall") == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_backWall = pNewElem;
				}
				else if(_T("plotArea") == sName)
				{
					CT_PlotArea* pNewElem = new CT_PlotArea;
					pNewElem->fromXML(oReader);
					m_plotArea = pNewElem;
				}
				else if(_T("legend") == sName)
				{
					CT_Legend* pNewElem = new CT_Legend;
					pNewElem->fromXML(oReader);
					m_legend = pNewElem;
				}
				else if(_T("plotVisOnly") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_plotVisOnly = pNewElem;
				}
				else if(_T("dispBlanksAs") == sName)
				{
					CT_DispBlanksAs* pNewElem = new CT_DispBlanksAs;
					pNewElem->fromXML(oReader);
					m_dispBlanksAs = pNewElem;
				}
				else if(_T("showDLblsOverMax") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_showDLblsOverMax = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
					m_extLst = pNewElem;
				}
			}
		}
		void CT_Chart::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<c:chart>");

			if(NULL != m_title)
			{
                std::wstring sNodeName = _T("c:title");
                m_title->toXML(sNodeName, writer);
			}
			if(NULL != m_autoTitleDeleted)
			{
                std::wstring sNodeName = _T("c:autoTitleDeleted");
                m_autoTitleDeleted->toXML(sNodeName, writer);
			}
			if(NULL != m_pivotFmts)
			{
                std::wstring sNodeName = _T("c:pivotFmts");
                m_pivotFmts->toXML(sNodeName, writer);
			}
			if(NULL != m_view3D)
			{
                std::wstring sNodeName = _T("c:view3D");
                m_view3D->toXML(sNodeName, writer);
			}
			if(NULL != m_floor)
			{
                std::wstring sNodeName = _T("c:floor");
                m_floor->toXML(sNodeName, writer);
			}
			if(NULL != m_sideWall)
			{
                std::wstring sNodeName = _T("c:sideWall");
                m_sideWall->toXML(sNodeName, writer);
			}
			if(NULL != m_backWall)
			{
                std::wstring sNodeName = _T("c:backWall");
                m_backWall->toXML(sNodeName, writer);
			}
			if(NULL != m_plotArea)
			{
                std::wstring sNodeName = _T("c:plotArea");
                m_plotArea->toXML(sNodeName, writer);
			}
			if(NULL != m_legend)
			{
                std::wstring sNodeName = _T("c:legend");
                m_legend->toXML(sNodeName, writer);
			}
			if(NULL != m_plotVisOnly)
			{
                std::wstring sNodeName = _T("c:plotVisOnly");
                m_plotVisOnly->toXML(sNodeName, writer);
			}
			if(NULL != m_dispBlanksAs)
			{
                std::wstring sNodeName = _T("c:dispBlanksAs");
                m_dispBlanksAs->toXML(sNodeName, writer);
			}
			if(NULL != m_showDLblsOverMax)
			{
                std::wstring sNodeName = _T("c:showDLblsOverMax");
                m_showDLblsOverMax->toXML(sNodeName, writer);
			}
			if(NULL != m_extLst)
			{
                std::wstring sNodeName = _T("c:extLst");
                m_extLst->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</c:chart>");
		}
		EElementType CT_Chart::getType(){return et_ct_Chart;}
		CT_Protection::CT_Protection()
		{
			m_chartObject = NULL;
			m_data = NULL;
			m_formatting = NULL;
			m_selection = NULL;
			m_userInterface = NULL;
		}
		CT_Protection::~CT_Protection()
		{
			if(NULL != m_chartObject)
				delete m_chartObject;
			if(NULL != m_data)
				delete m_data;
			if(NULL != m_formatting)
				delete m_formatting;
			if(NULL != m_selection)
				delete m_selection;
			if(NULL != m_userInterface)
				delete m_userInterface;
		}
		void CT_Protection::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("chartObject") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_chartObject = pNewElem;
				}
				else if(_T("data") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_data = pNewElem;
				}
				else if(_T("formatting") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_formatting = pNewElem;
				}
				else if(_T("selection") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_selection = pNewElem;
				}
				else if(_T("userInterface") == sName)
				{
					CT_Boolean* pNewElem = new CT_Boolean;
					pNewElem->fromXML(oReader);
					m_userInterface = pNewElem;
				}
			}
		}
		void CT_Protection::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_chartObject)
			{
                std::wstring sNodeName = _T("c:chartObject");
                m_chartObject->toXML(sNodeName, writer);
			}
			if(NULL != m_data)
			{
                std::wstring sNodeName = _T("c:data");
                m_data->toXML(sNodeName, writer);
			}
			if(NULL != m_formatting)
			{
                std::wstring sNodeName = _T("c:formatting");
                m_formatting->toXML(sNodeName, writer);
			}
			if(NULL != m_selection)
			{
                std::wstring sNodeName = _T("c:selection");
                m_selection->toXML(sNodeName, writer);
			}
			if(NULL != m_userInterface)
			{
                std::wstring sNodeName = _T("c:userInterface");
                m_userInterface->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Protection::getType(){return et_ct_protection;}
		CT_PivotSource::CT_PivotSource()
		{
			m_name = NULL;
			m_fmtId = NULL;
		}
		CT_PivotSource::~CT_PivotSource()
		{
			if(NULL != m_name)
				delete m_name;
			if(NULL != m_fmtId)
				delete m_fmtId;
            for(size_t i = 0; i < m_extLst.size(); ++i)
				delete m_extLst[i];
            m_extLst.clear();
		}
		void CT_PivotSource::fromXML(XmlUtils::CXmlLiteReader& oReader){
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("name") == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_name = pNewElem;
				}
				else if(_T("fmtId") == sName)
				{
					CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
					pNewElem->fromXML(oReader);
					m_fmtId = pNewElem;
				}
				else if(_T("extLst") == sName)
				{
					CT_extLst* pNewElem = new CT_extLst;
					pNewElem->fromXML(oReader);
                    m_extLst.push_back(pNewElem);
				}
			}
		}
		void CT_PivotSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_name)
			{
				if (std::wstring::npos != m_name->find(' ') || std::wstring::npos != m_name->find('\n'))
					writer.WriteString(_T("<c:name xml:space=\"preserve\">"));
				else
					writer.WriteString(_T("<c:name>"));
				writer.WriteEncodeXmlString(*m_name);
				writer.WriteString(_T("</c:name>"));
			}
			if(NULL != m_fmtId)
			{
                std::wstring sNodeName = _T("c:fmtId");
                m_fmtId->toXML(sNodeName, writer);
			}
            for(size_t i = 0; i < m_extLst.size(); ++i)
			{
				CT_extLst* pElem = m_extLst[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("c:extLst");
                    pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PivotSource::getType(){return et_ct_pivotsource;}
		CT_Style::CT_Style() : m_val(NULL), m_namespace(L"c")
		{
		}
		CT_Style::~CT_Style()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_Style::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
			ReadAttributes(oReader);

			if(!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_Style::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<" + m_namespace + L":style");
			if(NULL != m_val)
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Style::getType(){return et_ct_style;}
		void CT_Style::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					unsigned char* pNewElem = new unsigned char;
                    std::wstring sVal = oReader.GetText();
                    *pNewElem = XmlUtils::GetInteger(sVal);
					m_val = pNewElem;
				}
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		CT_TextLanguageID::CT_TextLanguageID()
		{
			m_val = NULL;
		}
		CT_TextLanguageID::~CT_TextLanguageID()
		{
			if(NULL != m_val)
				delete m_val;
		}
		void CT_TextLanguageID::fromXML(XmlUtils::CXmlLiteReader& oReader){
				ReadAttributes(oReader);

				if(!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
		}
		void CT_TextLanguageID::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_val)
			{
				WritingStringAttrEncodeXmlString(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_TextLanguageID::getType(){return et_ct_textlanguageid;}
		void CT_TextLanguageID::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("val") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_val = pNewElem;
				}
				WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		AlternateContent::AlternateContent()
		{
			m_Fallback = NULL;
		}
		AlternateContent::~AlternateContent()
		{
            for(size_t i = 0; i < m_Choice.size(); ++i)
				delete m_Choice[i];
            m_Choice.clear();
			if(NULL != m_Fallback)
				delete m_Fallback;
		}
		void AlternateContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("Choice") == sName)
				{
					AlternateContentChoice* pNewElem = new AlternateContentChoice;
					pNewElem->fromXML(oReader);
                    m_Choice.push_back(pNewElem);
				}
				else if(_T("Fallback") == sName)
				{
					AlternateContentFallback* pNewElem = new AlternateContentFallback;
					pNewElem->fromXML(oReader);
					m_Fallback = pNewElem;
				}
			}
		}
		void AlternateContent::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
            for(size_t i = 0; i < m_Choice.size(); ++i)
			{
				AlternateContentChoice* pElem = m_Choice[i];
				if(NULL != pElem)
				{
                    std::wstring sNodeName = _T("mc:Choice");
                    pElem->toXML(sNodeName, writer);
				}
			}
			if(NULL != m_Fallback)
			{
                std::wstring sNodeName = _T("mc:Fallback");
                m_Fallback->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType AlternateContent::getType()
		{
			return et_ct_alternatecontent;
		}
		AlternateContentChoice::AlternateContentChoice()
		{
			m_style = NULL;
			m_Requires = NULL;
		}
		AlternateContentChoice::~AlternateContentChoice()
		{
			if(NULL != m_style)
				delete m_style;
			if(NULL != m_Requires)
				delete m_Requires;
		}
		void AlternateContentChoice::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
				ReadAttributes(oReader);

					if ( oReader.IsEmptyNode() )
						return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if(_T("style") == sName)
					{
						m_style = new CT_Style;
						m_style->fromXML(oReader);
					}
				}
		}
		void AlternateContentChoice::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if(NULL != m_Requires)
			{
				WritingStringAttrEncodeXmlString(L"Requires", *m_Requires);
			}
			writer.WriteString(L">");
			if(NULL != m_style)
			{
				m_style->m_namespace = L"c14";
                m_style->toXML(writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType AlternateContentChoice::getType()
		{
			return et_ct_alternatecontentchoice;
		}
		void AlternateContentChoice::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				if(_T("Requires") == wsName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText();
					*pNewElem = sVal;
					m_Requires = pNewElem;
				}
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		AlternateContentFallback::AlternateContentFallback()
		{
			m_style = NULL;
		}
		AlternateContentFallback::~AlternateContentFallback()
		{
			if(NULL != m_style)
				delete m_style;
		}
		void AlternateContentFallback::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if(_T("style") == sName)
				{
					m_style = new CT_Style;
					m_style->fromXML(oReader);
				}
			}
		}
		void AlternateContentFallback::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if(NULL != m_style)
			{
                m_style->toXML(writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType AlternateContentFallback::getType()
		{
			return et_ct_alternatecontentfallback;
		}
	}
}
