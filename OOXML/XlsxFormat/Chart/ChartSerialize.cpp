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

//Generated code
#include "ChartSerialize.h"
#include "../../../DesktopEditor/common/StringExt.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/SERIESFORMAT.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Series.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BRAI.h"

namespace OOX
{
namespace Spreadsheet 
{

		bool FromXml_ST_PageSetupOrientation(const std::wstring& val, ST_PageSetupOrientation& eOut)
		{
			bool bRes = true;
			if (L"default" == val)
				eOut = st_pagesetuporientationDEFAULT;
			else if (L"portrait" == val)
				eOut = st_pagesetuporientationPORTRAIT;
			else if (L"landscape" == val)
				eOut = st_pagesetuporientationLANDSCAPE;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_PageSetupOrientation(ST_PageSetupOrientation val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_pagesetuporientationDEFAULT == val)
				sOut = L"default";
			else if (st_pagesetuporientationPORTRAIT == val)
				sOut = L"portrait";
			else if (st_pagesetuporientationLANDSCAPE == val)
				sOut = L"landscape";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_DispBlanksAs(const std::wstring& val, ST_DispBlanksAs& eOut)
		{
			bool bRes = true;
			if (L"span" == val)
				eOut = st_dispblanksasSPAN;
			else if (L"gap" == val)
				eOut = st_dispblanksasGAP;
			else if (L"zero" == val)
				eOut = st_dispblanksasZERO;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_DispBlanksAs(ST_DispBlanksAs val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_dispblanksasSPAN == val)
				sOut = L"span";
			else if (st_dispblanksasGAP == val)
				sOut = L"gap";
			else if (st_dispblanksasZERO == val)
				sOut = L"zero";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_LegendPos(const std::wstring& val, ST_LegendPos& eOut)
		{
			bool bRes = true;
			if (L"b" == val)
				eOut = st_legendposB;
			else if (L"tr" == val)
				eOut = st_legendposTR;
			else if (L"l" == val)
				eOut = st_legendposL;
			else if (L"r" == val)
				eOut = st_legendposR;
			else if (L"t" == val)
				eOut = st_legendposT;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_LegendPos(ST_LegendPos val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_legendposB == val)
				sOut = L"b";
			else if (st_legendposTR == val)
				sOut = L"tr";
			else if (st_legendposL == val)
				sOut = L"l";
			else if (st_legendposR == val)
				sOut = L"r";
			else if (st_legendposT == val)
				sOut = L"t";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_LayoutTarget(const std::wstring& val, ST_LayoutTarget& eOut)
		{
			bool bRes = true;
			if (L"inner" == val)
				eOut = st_layouttargetINNER;
			else if (L"outer" == val)
				eOut = st_layouttargetOUTER;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_LayoutTarget(ST_LayoutTarget val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_layouttargetINNER == val)
				sOut = L"inner";
			else if (st_layouttargetOUTER == val)
				sOut = L"outer";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_LayoutMode(const std::wstring& val, ST_LayoutMode& eOut)
		{
			bool bRes = true;
			if (L"edge" == val)
				eOut = st_layoutmodeEDGE;
			else if (L"factor" == val)
				eOut = st_layoutmodeFACTOR;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_LayoutMode(ST_LayoutMode val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_layoutmodeEDGE == val)
				sOut = L"edge";
			else if (st_layoutmodeFACTOR == val)
				sOut = L"factor";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_Orientation(const std::wstring& val, ST_Orientation& eOut)
		{
			bool bRes = true;
			if (L"maxMin" == val)
				eOut = st_orientationMAXMIN;
			else if (L"minMax" == val)
				eOut = st_orientationMINMAX;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_Orientation(ST_Orientation val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_orientationMAXMIN == val)
				sOut = L"maxMin";
			else if (st_orientationMINMAX == val)
				sOut = L"minMax";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_AxPos(const std::wstring& val, ST_AxPos& eOut)
		{
			bool bRes = true;
			if (L"b" == val)
				eOut = st_axposB;
			else if (L"l" == val)
				eOut = st_axposL;
			else if (L"r" == val)
				eOut = st_axposR;
			else if (L"t" == val)
				eOut = st_axposT;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_AxPos(ST_AxPos val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_axposB == val)
				sOut = L"b";
			else if (st_axposL == val)
				sOut = L"l";
			else if (st_axposR == val)
				sOut = L"r";
			else if (st_axposT == val)
				sOut = L"t";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_TickMark(const std::wstring& val, ST_TickMark& eOut)
		{
			bool bRes = true;
			if (L"cross" == val)
				eOut = st_tickmarkCROSS;
			else if (L"in" == val)
				eOut = st_tickmarkIN;
			else if (L"none" == val)
				eOut = st_tickmarkNONE;
			else if (L"out" == val)
				eOut = st_tickmarkOUT;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_TickMark(ST_TickMark val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_tickmarkCROSS == val)
				sOut = L"cross";
			else if (st_tickmarkIN == val)
				sOut = L"in";
			else if (st_tickmarkNONE == val)
				sOut = L"none";
			else if (st_tickmarkOUT == val)
				sOut = L"out";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_TickLblPos(const std::wstring& val, ST_TickLblPos& eOut)
		{
			bool bRes = true;
			if (L"high" == val)
				eOut = st_ticklblposHIGH;
			else if (L"low" == val)
				eOut = st_ticklblposLOW;
			else if (L"nextTo" == val)
				eOut = st_ticklblposNEXTTO;
			else if (L"none" == val)
				eOut = st_ticklblposNONE;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_TickLblPos(ST_TickLblPos val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_ticklblposHIGH == val)
				sOut = L"high";
			else if (st_ticklblposLOW == val)
				sOut = L"low";
			else if (st_ticklblposNEXTTO == val)
				sOut = L"nextTo";
			else if (st_ticklblposNONE == val)
				sOut = L"none";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_Crosses(const std::wstring& val, ST_Crosses& eOut)
		{
			bool bRes = true;
			if (L"autoZero" == val)
				eOut = st_crossesAUTOZERO;
			else if (L"max" == val)
				eOut = st_crossesMAX;
			else if (L"min" == val)
				eOut = st_crossesMIN;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_Crosses(ST_Crosses val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_crossesAUTOZERO == val)
				sOut = L"autoZero";
			else if (st_crossesMAX == val)
				sOut = L"max";
			else if (st_crossesMIN == val)
				sOut = L"min";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_TimeUnit(const std::wstring& val, ST_TimeUnit& eOut)
		{
			bool bRes = true;
			if (L"days" == val)
				eOut = st_timeunitDAYS;
			else if (L"months" == val)
				eOut = st_timeunitMONTHS;
			else if (L"years" == val)
				eOut = st_timeunitYEARS;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_TimeUnit(ST_TimeUnit val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_timeunitDAYS == val)
				sOut = L"days";
			else if (st_timeunitMONTHS == val)
				sOut = L"months";
			else if (st_timeunitYEARS == val)
				sOut = L"years";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_LblAlgn(const std::wstring& val, ST_LblAlgn& eOut)
		{
			bool bRes = true;
			if (L"ctr" == val)
				eOut = st_lblalgnCTR;
			else if (L"l" == val)
				eOut = st_lblalgnL;
			else if (L"r" == val)
				eOut = st_lblalgnR;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_LblAlgn(ST_LblAlgn val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_lblalgnCTR == val)
				sOut = L"ctr";
			else if (st_lblalgnL == val)
				sOut = L"l";
			else if (st_lblalgnR == val)
				sOut = L"r";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_BuiltInUnit(const std::wstring& val, ST_BuiltInUnit& eOut)
		{
			bool bRes = true;
			if (L"hundreds" == val)
				eOut = st_builtinunitHUNDREDS;
			else if (L"thousands" == val)
				eOut = st_builtinunitTHOUSANDS;
			else if (L"tenThousands" == val)
				eOut = st_builtinunitTENTHOUSANDS;
			else if (L"hundredThousands" == val)
				eOut = st_builtinunitHUNDREDTHOUSANDS;
			else if (L"millions" == val)
				eOut = st_builtinunitMILLIONS;
			else if (L"tenMillions" == val)
				eOut = st_builtinunitTENMILLIONS;
			else if (L"hundredMillions" == val)
				eOut = st_builtinunitHUNDREDMILLIONS;
			else if (L"billions" == val)
				eOut = st_builtinunitBILLIONS;
			else if (L"trillions" == val)
				eOut = st_builtinunitTRILLIONS;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_BuiltInUnit(ST_BuiltInUnit val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_builtinunitHUNDREDS == val)
				sOut = L"hundreds";
			else if (st_builtinunitTHOUSANDS == val)
				sOut = L"thousands";
			else if (st_builtinunitTENTHOUSANDS == val)
				sOut = L"tenThousands";
			else if (st_builtinunitHUNDREDTHOUSANDS == val)
				sOut = L"hundredThousands";
			else if (st_builtinunitMILLIONS == val)
				sOut = L"millions";
			else if (st_builtinunitTENMILLIONS == val)
				sOut = L"tenMillions";
			else if (st_builtinunitHUNDREDMILLIONS == val)
				sOut = L"hundredMillions";
			else if (st_builtinunitBILLIONS == val)
				sOut = L"billions";
			else if (st_builtinunitTRILLIONS == val)
				sOut = L"trillions";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_CrossBetween(const std::wstring& val, ST_CrossBetween& eOut)
		{
			bool bRes = true;
			if (L"between" == val)
				eOut = st_crossbetweenBETWEEN;
			else if (L"midCat" == val)
				eOut = st_crossbetweenMIDCAT;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_CrossBetween(ST_CrossBetween val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_crossbetweenBETWEEN == val)
				sOut = L"between";
			else if (st_crossbetweenMIDCAT == val)
				sOut = L"midCat";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_SizeRepresents(const std::wstring& val, ST_SizeRepresents& eOut)
		{
			bool bRes = true;
			if (L"area" == val)
				eOut = st_sizerepresentsAREA;
			else if (L"w" == val)
				eOut = st_sizerepresentsW;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_SizeRepresents(ST_SizeRepresents val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_sizerepresentsAREA == val)
				sOut = L"area";
			else if (st_sizerepresentsW == val)
				sOut = L"w";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_MarkerStyle(const std::wstring& val, ST_MarkerStyle& eOut)
		{
			bool bRes = true;
			if (L"circle" == val)
				eOut = st_markerstyleCIRCLE;
			else if (L"dash" == val)
				eOut = st_markerstyleDASH;
			else if (L"diamond" == val)
				eOut = st_markerstyleDIAMOND;
			else if (L"dot" == val)
				eOut = st_markerstyleDOT;
			else if (L"none" == val)
				eOut = st_markerstyleNONE;
			else if (L"picture" == val)
				eOut = st_markerstylePICTURE;
			else if (L"plus" == val)
				eOut = st_markerstylePLUS;
			else if (L"square" == val)
				eOut = st_markerstyleSQUARE;
			else if (L"star" == val)
				eOut = st_markerstyleSTAR;
			else if (L"triangle" == val)
				eOut = st_markerstyleTRIANGLE;
			else if (L"x" == val)
				eOut = st_markerstyleX;
			else if (L"auto" == val)
				eOut = st_markerstyleAUTO;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_MarkerStyle(ST_MarkerStyle val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_markerstyleCIRCLE == val)
				sOut = L"circle";
			else if (st_markerstyleDASH == val)
				sOut = L"dash";
			else if (st_markerstyleDIAMOND == val)
				sOut = L"diamond";
			else if (st_markerstyleDOT == val)
				sOut = L"dot";
			else if (st_markerstyleNONE == val)
				sOut = L"none";
			else if (st_markerstylePICTURE == val)
				sOut = L"picture";
			else if (st_markerstylePLUS == val)
				sOut = L"plus";
			else if (st_markerstyleSQUARE == val)
				sOut = L"square";
			else if (st_markerstyleSTAR == val)
				sOut = L"star";
			else if (st_markerstyleTRIANGLE == val)
				sOut = L"triangle";
			else if (st_markerstyleX == val)
				sOut = L"x";
			else if (st_markerstyleAUTO == val)
				sOut = L"auto";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_PictureFormat(const std::wstring& val, ST_PictureFormat& eOut)
		{
			bool bRes = true;
			if (L"stretch" == val)
				eOut = st_pictureformatSTRETCH;
			else if (L"stack" == val)
				eOut = st_pictureformatSTACK;
			else if (L"stackScale" == val)
				eOut = st_pictureformatSTACKSCALE;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_PictureFormat(ST_PictureFormat val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_pictureformatSTRETCH == val)
				sOut = L"stretch";
			else if (st_pictureformatSTACK == val)
				sOut = L"stack";
			else if (st_pictureformatSTACKSCALE == val)
				sOut = L"stackScale";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_DLblPos(const std::wstring& val, ST_DLblPos& eOut)
		{
			bool bRes = true;
			if (L"bestFit" == val)
				eOut = st_dlblposBESTFIT;
			else if (L"b" == val)
				eOut = st_dlblposB;
			else if (L"ctr" == val)
				eOut = st_dlblposCTR;
			else if (L"inBase" == val)
				eOut = st_dlblposINBASE;
			else if (L"inEnd" == val)
				eOut = st_dlblposINEND;
			else if (L"l" == val)
				eOut = st_dlblposL;
			else if (L"outEnd" == val)
				eOut = st_dlblposOUTEND;
			else if (L"r" == val)
				eOut = st_dlblposR;
			else if (L"t" == val)
				eOut = st_dlblposT;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_DLblPos(ST_DLblPos val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_dlblposBESTFIT == val)
				sOut = L"bestFit";
			else if (st_dlblposB == val)
				sOut = L"b";
			else if (st_dlblposCTR == val)
				sOut = L"ctr";
			else if (st_dlblposINBASE == val)
				sOut = L"inBase";
			else if (st_dlblposINEND == val)
				sOut = L"inEnd";
			else if (st_dlblposL == val)
				sOut = L"l";
			else if (st_dlblposOUTEND == val)
				sOut = L"outEnd";
			else if (st_dlblposR == val)
				sOut = L"r";
			else if (st_dlblposT == val)
				sOut = L"t";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_TrendlineType(const std::wstring& val, ST_TrendlineType& eOut)
		{
			bool bRes = true;
			if (L"exp" == val)
				eOut = st_trendlinetypeEXP;
			else if (L"linear" == val)
				eOut = st_trendlinetypeLINEAR;
			else if (L"log" == val)
				eOut = st_trendlinetypeLOG;
			else if (L"movingAvg" == val)
				eOut = st_trendlinetypeMOVINGAVG;
			else if (L"poly" == val)
				eOut = st_trendlinetypePOLY;
			else if (L"power" == val)
				eOut = st_trendlinetypePOWER;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_TrendlineType(ST_TrendlineType val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_trendlinetypeEXP == val)
				sOut = L"exp";
			else if (st_trendlinetypeLINEAR == val)
				sOut = L"linear";
			else if (st_trendlinetypeLOG == val)
				sOut = L"log";
			else if (st_trendlinetypeMOVINGAVG == val)
				sOut = L"movingAvg";
			else if (st_trendlinetypePOLY == val)
				sOut = L"poly";
			else if (st_trendlinetypePOWER == val)
				sOut = L"power";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_ErrDir(const std::wstring& val, ST_ErrDir& eOut)
		{
			bool bRes = true;
			if (L"x" == val)
				eOut = st_errdirX;
			else if (L"y" == val)
				eOut = st_errdirY;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_ErrDir(ST_ErrDir val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_errdirX == val)
				sOut = L"x";
			else if (st_errdirY == val)
				sOut = L"y";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_ErrBarType(const std::wstring& val, ST_ErrBarType& eOut)
		{
			bool bRes = true;
			if (L"both" == val)
				eOut = st_errbartypeBOTH;
			else if (L"minus" == val)
				eOut = st_errbartypeMINUS;
			else if (L"plus" == val)
				eOut = st_errbartypePLUS;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_ErrBarType(ST_ErrBarType val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_errbartypeBOTH == val)
				sOut = L"both";
			else if (st_errbartypeMINUS == val)
				sOut = L"minus";
			else if (st_errbartypePLUS == val)
				sOut = L"plus";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_ErrValType(const std::wstring& val, ST_ErrValType& eOut)
		{
			bool bRes = true;
			if (L"cust" == val)
				eOut = st_errvaltypeCUST;
			else if (L"fixedVal" == val)
				eOut = st_errvaltypeFIXEDVAL;
			else if (L"percentage" == val)
				eOut = st_errvaltypePERCENTAGE;
			else if (L"stdDev" == val)
				eOut = st_errvaltypeSTDDEV;
			else if (L"stdErr" == val)
				eOut = st_errvaltypeSTDERR;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_ErrValType(ST_ErrValType val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_errvaltypeCUST == val)
				sOut = L"cust";
			else if (st_errvaltypeFIXEDVAL == val)
				sOut = L"fixedVal";
			else if (st_errvaltypePERCENTAGE == val)
				sOut = L"percentage";
			else if (st_errvaltypeSTDDEV == val)
				sOut = L"stdDev";
			else if (st_errvaltypeSTDERR == val)
				sOut = L"stdErr";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_SplitType(const std::wstring& val, ST_SplitType& eOut)
		{
			bool bRes = true;
			if (L"auto" == val)
				eOut = st_splittypeAUTO;
			else if (L"cust" == val)
				eOut = st_splittypeCUST;
			else if (L"percent" == val)
				eOut = st_splittypePERCENT;
			else if (L"pos" == val)
				eOut = st_splittypePOS;
			else if (L"val" == val)
				eOut = st_splittypeVAL;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_SplitType(ST_SplitType val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_splittypeAUTO == val)
				sOut = L"auto";
			else if (st_splittypeCUST == val)
				sOut = L"cust";
			else if (st_splittypePERCENT == val)
				sOut = L"percent";
			else if (st_splittypePOS == val)
				sOut = L"pos";
			else if (st_splittypeVAL == val)
				sOut = L"val";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_OfPieType(const std::wstring& val, ST_OfPieType& eOut)
		{
			bool bRes = true;
			if (L"pie" == val)
				eOut = st_ofpietypePIE;
			else if (L"bar" == val)
				eOut = st_ofpietypeBAR;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_OfPieType(ST_OfPieType val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_ofpietypePIE == val)
				sOut = L"pie";
			else if (st_ofpietypeBAR == val)
				sOut = L"bar";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_BarDir(const std::wstring& val, ST_BarDir& eOut)
		{
			bool bRes = true;
			if (L"bar" == val)
				eOut = st_bardirBAR;
			else if (L"col" == val)
				eOut = st_bardirCOL;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_BarDir(ST_BarDir val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_bardirBAR == val)
				sOut = L"bar";
			else if (st_bardirCOL == val)
				sOut = L"col";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_BarGrouping(const std::wstring& val, ST_BarGrouping& eOut)
		{
			bool bRes = true;
			if (L"percentStacked" == val)
				eOut = st_bargroupingPERCENTSTACKED;
			else if (L"clustered" == val)
				eOut = st_bargroupingCLUSTERED;
			else if (L"standard" == val)
				eOut = st_bargroupingSTANDARD;
			else if (L"stacked" == val)
				eOut = st_bargroupingSTACKED;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_BarGrouping(ST_BarGrouping val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_bargroupingPERCENTSTACKED == val)
				sOut = L"percentStacked";
			else if (st_bargroupingCLUSTERED == val)
				sOut = L"clustered";
			else if (st_bargroupingSTANDARD == val)
				sOut = L"standard";
			else if (st_bargroupingSTACKED == val)
				sOut = L"stacked";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_Shape(const std::wstring& val, ST_Shape& eOut)
		{
			bool bRes = true;
			if (L"cone" == val)
				eOut = st_shapeCONE;
			else if (L"coneToMax" == val)
				eOut = st_shapeCONETOMAX;
			else if (L"box" == val)
				eOut = st_shapeBOX;
			else if (L"cylinder" == val)
				eOut = st_shapeCYLINDER;
			else if (L"pyramid" == val)
				eOut = st_shapePYRAMID;
			else if (L"pyramidToMax" == val)
				eOut = st_shapePYRAMIDTOMAX;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_Shape(ST_Shape val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_shapeCONE == val)
				sOut = L"cone";
			else if (st_shapeCONETOMAX == val)
				sOut = L"coneToMax";
			else if (st_shapeBOX == val)
				sOut = L"box";
			else if (st_shapeCYLINDER == val)
				sOut = L"cylinder";
			else if (st_shapePYRAMID == val)
				sOut = L"pyramid";
			else if (st_shapePYRAMIDTOMAX == val)
				sOut = L"pyramidToMax";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_ScatterStyle(const std::wstring& val, ST_ScatterStyle& eOut)
		{
			bool bRes = true;
			if (L"none" == val)
				eOut = st_scatterstyleNONE;
			else if (L"line" == val)
				eOut = st_scatterstyleLINE;
			else if (L"lineMarker" == val)
				eOut = st_scatterstyleLINEMARKER;
			else if (L"marker" == val)
				eOut = st_scatterstyleMARKER;
			else if (L"smooth" == val)
				eOut = st_scatterstyleSMOOTH;
			else if (L"smoothMarker" == val)
				eOut = st_scatterstyleSMOOTHMARKER;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_ScatterStyle(ST_ScatterStyle val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_scatterstyleNONE == val)
				sOut = L"none";
			else if (st_scatterstyleLINE == val)
				sOut = L"line";
			else if (st_scatterstyleLINEMARKER == val)
				sOut = L"lineMarker";
			else if (st_scatterstyleMARKER == val)
				sOut = L"marker";
			else if (st_scatterstyleSMOOTH == val)
				sOut = L"smooth";
			else if (st_scatterstyleSMOOTHMARKER == val)
				sOut = L"smoothMarker";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_RadarStyle(const std::wstring& val, ST_RadarStyle& eOut)
		{
			bool bRes = true;
			if (L"standard" == val)
				eOut = st_radarstyleSTANDARD;
			else if (L"marker" == val)
				eOut = st_radarstyleMARKER;
			else if (L"filled" == val)
				eOut = st_radarstyleFILLED;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_RadarStyle(ST_RadarStyle val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_radarstyleSTANDARD == val)
				sOut = L"standard";
			else if (st_radarstyleMARKER == val)
				sOut = L"marker";
			else if (st_radarstyleFILLED == val)
				sOut = L"filled";
			else
				bRes = false;
			return bRes;
		}
		bool FromXml_ST_Grouping(const std::wstring& val, ST_Grouping& eOut)
		{
			bool bRes = true;
			if (L"percentStacked" == val)
				eOut = st_groupingPERCENTSTACKED;
			else if (L"standard" == val)
				eOut = st_groupingSTANDARD;
			else if (L"stacked" == val)
				eOut = st_groupingSTACKED;
			else
				bRes = false;
			return bRes;
		}
		bool ToXml_ST_Grouping(ST_Grouping val, std::wstring& sOut)
		{
			bool bRes = true;
			if (st_groupingPERCENTSTACKED == val)
				sOut = L"percentStacked";
			else if (st_groupingSTANDARD == val)
				sOut = L"standard";
			else if (st_groupingSTACKED == val)
				sOut = L"stacked";
			else
				bRes = false;
			return bRes;
		}

//-----------------------------------------------------------------------------------------------------------

		CT_ChartSpace::CT_ChartSpace()
		{
			m_AlternateContent = NULL;
			m_style = NULL;
			m_pivotSource = NULL;
			m_protection = NULL;
			m_chart = NULL;
			m_externalData = NULL;
			m_printSettings = NULL;
			m_userShapes = NULL;
		}
		CT_ChartSpace::~CT_ChartSpace()
		{
			if (NULL != m_AlternateContent)
				delete m_AlternateContent;
			if (NULL != m_style)
				delete m_style;
			if (NULL != m_pivotSource)
				delete m_pivotSource;
			if (NULL != m_protection)
				delete m_protection;
			if (NULL != m_chart)
				delete m_chart;
			if (NULL != m_externalData)
				delete m_externalData;
			if (NULL != m_printSettings)
				delete m_printSettings;
			if (NULL != m_userShapes)
				delete m_userShapes;
		}
		void CT_ChartSpace::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"date1904" == sName)
					m_date1904 = oReader;
				else if (L"lang" == sName)
					m_lang = oReader;
				else if (L"roundedCorners" == sName)
					m_roundedCorners = oReader;
				else if (L"AlternateContent" == sName)
				{
					AlternateContent* pNewElem = new AlternateContent;
					pNewElem->fromXML(oReader);
					m_AlternateContent = pNewElem;
				}
				else if (L"style" == sName)
				{
					m_style = new CT_Style;
					m_style->fromXML(oReader);
				}
				else if (L"clrMapOvr" == sName)
				{
					m_oClrMapOvr = oReader;
				}
				else if (L"pivotSource" == sName)
				{
					CT_PivotSource* pNewElem = new CT_PivotSource;
					pNewElem->fromXML(oReader);
					m_pivotSource = pNewElem;
				}
				else if (L"protection" == sName)
				{
					CT_Protection* pNewElem = new CT_Protection;
					pNewElem->fromXML(oReader);
					m_protection = pNewElem;
				}
				else if (L"chart" == sName)
				{
					CT_Chart* pNewElem = new CT_Chart;
					pNewElem->fromXML(oReader);
					m_chart = pNewElem;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"externalData" == sName)
				{
					CT_ExternalData* pNewElem = new CT_ExternalData;
					pNewElem->fromXML(oReader);
					m_externalData = pNewElem;
				}
				else if (L"printSettings" == sName)
				{
					CT_PrintSettings* pNewElem = new CT_PrintSettings;
					pNewElem->fromXML(oReader);
					m_printSettings = pNewElem;
				}
				else if (L"userShapes" == sName)
				{
					CT_RelId* pNewElem = new CT_RelId;
					pNewElem->fromXML(oReader);
					m_userShapes = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_ChartSpace::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<" + sNodeName + L" \
xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:c15=\"http://schemas.microsoft.com/office/drawing/2012/chart\" \
xmlns:c14=\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\" \
xmlns:c16r2=\"http://schemas.microsoft.com/office/drawing/2015/06/chart\"");

			writer.WriteString(L">");
			
			m_date1904.toXML(L"c:date1904", writer);
			
			m_lang.toXML(L"c:lang", writer);

			m_roundedCorners.toXML(L"c:roundedCorners", writer);

			if (NULL != m_AlternateContent)
			{
				m_AlternateContent->toXML(L"mc:AlternateContent", writer);
			}
			if (NULL != m_style)
			{
				m_style->toXML(writer);
			}
			if (m_oClrMapOvr.IsInit())
			{
				writer.WriteString(m_oClrMapOvr->toXML());
			}
			if (NULL != m_pivotSource)
			{
				m_pivotSource->toXML(L"c:pivotSource", writer);
			}
			if (NULL != m_protection)
			{
				m_protection->toXML(L"c:protection", writer);
			}
			if (NULL != m_chart)
			{
				m_chart->toXML(writer);
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (NULL != m_externalData)
			{
				m_externalData->toXML(L"c:externalData", writer);
			}
			if (NULL != m_printSettings)
			{
				m_printSettings->toXML(L"c:printSettings", writer);
			}
			if (NULL != m_userShapes)
			{
				m_userShapes->toXML(L"c:userShapes", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ChartSpace::getType() { return et_ct_ChartSpace; }

		CT_RelId::CT_RelId()
		{
		}
		CT_RelId::~CT_RelId()
		{
		}
		void CT_RelId::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_RelId::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			
			if ( m_id.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"r:id", *m_id);
			}
			writer.WriteString(L" xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:c14=\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\"");
			writer.WriteString(L"/>");
		}
		EElementType CT_RelId::getType() { return et_ct_relid; }
		
		void CT_RelId::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				if (L"id" == wsName)
				{
					m_id = oReader.GetText();
					break;
				}
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_PageSetup::CT_PageSetup()
		{
		}
		CT_PageSetup::~CT_PageSetup()
		{
		}
		void CT_PageSetup::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_PageSetup::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (m_paperSize.IsInit())
			{
				WritingStringAttrInt(L"paperSize", *m_paperSize);
			}
			if (m_paperHeight.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"paperHeight", *m_paperHeight);
			}
			if (m_paperWidth.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"paperWidth", *m_paperWidth);
			}
			if (m_firstPageNumber.IsInit())
			{
				WritingStringAttrInt(L"firstPageNumber", *m_firstPageNumber);
			}
			if (m_orientation.IsInit())
			{
				WritingStringAttrString(L"orientation", m_orientation->ToString());
			}
			if (m_blackAndWhite.IsInit())
			{
				std::wstring sVal;
				if (*m_blackAndWhite)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"blackAndWhite", sVal);
			}
			if (m_draft.IsInit())
			{
				std::wstring sVal;
				if (*m_draft)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"draft", sVal);
			}
			if (m_useFirstPageNumber.IsInit())
			{
				std::wstring sVal;
				if (*m_useFirstPageNumber)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"useFirstPageNumber", sVal);
			}
			if (m_horizontalDpi.IsInit())
			{
				WritingStringAttrInt(L"horizontalDpi", *m_horizontalDpi);
			}
			if (m_verticalDpi.IsInit())
			{
				WritingStringAttrInt(L"verticalDpi", *m_verticalDpi);
			}
			if (m_copies.IsInit())
			{
				WritingStringAttrInt(L"copies", *m_copies);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PageSetup::getType() { return et_ct_pagesetup; }
		void CT_PageSetup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"paperSize", m_paperSize)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"paperHeight", m_paperHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"paperWidth", m_paperWidth)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"firstPageNumber", m_firstPageNumber)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"orientation", m_orientation)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"blackAndWhite", m_blackAndWhite)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"draft", m_draft)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"useFirstPageNumber", m_useFirstPageNumber)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"horizontalDpi", m_horizontalDpi)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"verticalDpi", m_draft)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"copies", m_copies)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_PageMargins::CT_PageMargins()
		{
		}
		CT_PageMargins::~CT_PageMargins()
		{
		}
		void CT_PageMargins::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_PageMargins::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (NULL != m_l.IsInit())
			{
				WritingStringAttrDouble(L"l", *m_l);
			}
			if (m_r.IsInit())
			{
				WritingStringAttrDouble(L"r", *m_r);
			}
			if (m_t.IsInit())
			{
				WritingStringAttrDouble(L"t", *m_t);
			}
			if (m_b.IsInit())
			{
				WritingStringAttrDouble(L"b", *m_b);
			}
			if (m_header.IsInit())
			{
				WritingStringAttrDouble(L"header", *m_header);
			}
			if (m_footer.IsInit())
			{
				WritingStringAttrDouble(L"footer", *m_footer);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_PageMargins::getType() { return et_ct_pagemargins; }

		void CT_PageMargins::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"l", m_l)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"r", m_r)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"t", m_t)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"b", m_b)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"header", m_header)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"footer", m_footer)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_HeaderFooter::CT_HeaderFooter()
		{
		}
		CT_HeaderFooter::~CT_HeaderFooter()
		{
		}
		void CT_HeaderFooter::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"oddHeader" == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_oddHeader = pNewElem;
				}
				else if (L"oddFooter" == sName)
				{
					m_oddFooter = oReader.GetText3();
				}
				else if (L"evenHeader" == sName)
				{
					m_evenHeader = oReader.GetText3();
				}
				else if (L"evenFooter" == sName)
				{
					m_evenFooter = oReader.GetText3();
				}
				else if (L"firstHeader" == sName)
				{
					m_firstHeader = oReader.GetText3();
				}
				else if (L"firstFooter" == sName)
				{
					m_firstFooter = oReader.GetText3();
				}
			}
		}
		void CT_HeaderFooter::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			
			if (m_alignWithMargins.IsInit())
			{
				std::wstring sVal;
				if (*m_alignWithMargins)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"alignWithMargins", sVal);
			}
			if (m_differentOddEven.IsInit())
			{
				std::wstring sVal;
				if (*m_differentOddEven)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"differentOddEven", sVal);
			}
			if (m_differentFirst.IsInit())
			{
				std::wstring sVal;
				if (*m_differentFirst)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"differentFirst", sVal);
			}
			writer.WriteString(L">");
			if (m_oddHeader.IsInit())
			{
				if (std::wstring::npos != m_oddHeader->find(' ')&&(  m_oddHeader->at(0) == ' ' || m_oddHeader->at(m_oddHeader->size()-1) == ' ')
				 || std::wstring::npos != m_oddHeader->find('\n')&&(  m_oddHeader->at(0) == '\n' || m_oddHeader->at(m_oddHeader->size()-1) == '\n'))
					writer.WriteString(L"<c:oddHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:oddHeader>");
				writer.WriteEncodeXmlString(*m_oddHeader);
				writer.WriteString(L"</c:oddHeader>");
			}
			if (m_oddFooter.IsInit())
			{
				if (std::wstring::npos != m_oddFooter->find(' ')&&(  m_oddFooter->at(0) == ' ' || m_oddFooter->at(m_oddFooter->size()-1) == ' ')
				 || std::wstring::npos != m_oddFooter->find('\n')&&(  m_oddFooter->at(0) == '\n' || m_oddFooter->at(m_oddFooter->size()-1) == '\n'))
					writer.WriteString(L"<c:oddFooter xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:oddFooter>");
				writer.WriteEncodeXmlString(*m_oddFooter);
				writer.WriteString(L"</c:oddFooter>");
			}
			if (m_evenHeader.IsInit())
			{
				if (std::wstring::npos != m_evenHeader->find(' ')&&(  m_evenHeader->at(0) == ' ' || m_evenHeader->at(m_evenHeader->size()-1) == ' ')
				 || std::wstring::npos != m_evenHeader->find('\n')&&(  m_evenHeader->at(0) == '\n' || m_evenHeader->at(m_evenHeader->size()-1) == '\n'))
					writer.WriteString(L"<c:evenHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:evenHeader>");
				writer.WriteEncodeXmlString(*m_evenHeader);
				writer.WriteString(L"</c:evenHeader>");
			}
			if ( m_evenFooter.IsInit())
			{
				if (std::wstring::npos != m_evenFooter->find(' ')&&(  m_evenFooter->at(0) == ' ' || m_evenFooter->at(m_evenFooter->size()-1) == ' ')
				 || std::wstring::npos != m_evenFooter->find('\n')&&(  m_evenFooter->at(0) == '\n' || m_evenFooter->at(m_evenFooter->size()-1) == '\n'))
					writer.WriteString(L"<c:evenFooter xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:evenFooter>");
				writer.WriteEncodeXmlString(*m_evenFooter);
				writer.WriteString(L"</c:evenFooter>");
			}
			if (m_firstHeader.IsInit())
			{
				if (std::wstring::npos != m_firstHeader->find(' ')&&(  m_firstHeader->at(0) == ' ' || m_firstHeader->at(m_firstHeader->size()-1) == ' ')
				 || std::wstring::npos != m_firstHeader->find('\n')&&(  m_firstHeader->at(0) == '\n' || m_firstHeader->at(m_firstHeader->size()-1) == '\n'))
					writer.WriteString(L"<c:firstHeader xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:firstHeader>");
				writer.WriteEncodeXmlString(*m_firstHeader);
				writer.WriteString(L"</c:firstHeader>");
			}
			if (m_firstFooter.IsInit())
			{
				if (std::wstring::npos != m_firstFooter->find(' ')&&(  m_firstFooter->at(0) == ' ' || m_firstFooter->at(m_firstFooter->size()-1) == ' ')
				 || std::wstring::npos != m_firstFooter->find('\n')&&(  m_firstFooter->at(0) == '\n' || m_firstFooter->at(m_firstFooter->size()-1) == '\n'))
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
		EElementType CT_HeaderFooter::getType() { return et_ct_headerfooter; }
		
		void CT_HeaderFooter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"alignWithMargins", m_alignWithMargins)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"differentOddEven", m_differentOddEven)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"differentFirst", m_differentFirst)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_PrintSettings::CT_PrintSettings()
		{
			m_headerFooter = NULL;
			m_pageMargins = NULL;
			m_pageSetup = NULL;
		}
		CT_PrintSettings::~CT_PrintSettings()
		{
			if (NULL != m_headerFooter)
				delete m_headerFooter;
			if (NULL != m_pageMargins)
				delete m_pageMargins;
			if (NULL != m_pageSetup)
				delete m_pageSetup;
		}
		void CT_PrintSettings::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"headerFooter" == sName)
				{
					CT_HeaderFooter* pNewElem = new CT_HeaderFooter;
					pNewElem->fromXML(oReader);
					m_headerFooter = pNewElem;
				}
				else if (L"pageMargins" == sName)
				{
					CT_PageMargins* pNewElem = new CT_PageMargins;
					pNewElem->fromXML(oReader);
					m_pageMargins = pNewElem;
				}
				else if (L"pageSetup" == sName)
				{
					CT_PageSetup* pNewElem = new CT_PageSetup;
					pNewElem->fromXML(oReader);
					m_pageSetup = pNewElem;
				}
			}
		}
		void CT_PrintSettings::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_headerFooter)
			{
				std::wstring sNodeName = L"c:headerFooter";
				m_headerFooter->toXML(sNodeName, writer);
			}
			if (NULL != m_pageMargins)
			{
				std::wstring sNodeName = L"c:pageMargins";
				m_pageMargins->toXML(sNodeName, writer);
			}
			if (NULL != m_pageSetup)
			{
				std::wstring sNodeName = L"c:pageSetup";
				m_pageSetup->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PrintSettings::getType() { return et_ct_printsettings; }
		
		CT_ExternalData::CT_ExternalData()
		{
		}
		CT_ExternalData::~CT_ExternalData()
		{
		}
		void CT_ExternalData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"autoUpdate" == sName)
				{
					m_autoUpdate = oReader;
				}
			}
		}
		void CT_ExternalData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (m_id.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"r:id", *m_id);
			}
			writer.WriteString(L">");
			m_autoUpdate.toXML(L"c:autoUpdate", writer);

			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ExternalData::getType() { return et_ct_externaldata; }
		void CT_ExternalData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				if (L"id" == wsName)
				{
					m_id = oReader.GetText();
				}
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}

		CT_LegendEntry::CT_LegendEntry()
		{
		}
		CT_LegendEntry::~CT_LegendEntry()
		{
		}
		void CT_LegendEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"idx" == sName)
				{
					m_idx = oReader;
				}
				else if (L"delete" == sName)
				{
					m_delete = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_LegendEntry::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_idx.toXML(L"c:idx", writer);

			m_delete.toXML(L"c:delete", writer);

			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LegendEntry::getType() { return et_ct_LegendEntry; }
		
		ST_DispBlanksAs CDispBlanksAs::FromString(const std::wstring &sValue)
		{
			FromXml_ST_DispBlanksAs(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CDispBlanksAs::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_DispBlanksAs(m_eValue, sEnumVal);
			return sEnumVal;
		}
		ST_PageSetupOrientation CPageSetupOrientation::FromString(const std::wstring &sValue)
		{
			FromXml_ST_PageSetupOrientation(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CPageSetupOrientation::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_PageSetupOrientation(m_eValue, sEnumVal);
			return sEnumVal;
		}		
		ST_LegendPos CLegendPos::FromString(const std::wstring &sValue)
		{
			FromXml_ST_LegendPos(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CLegendPos::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_LegendPos(m_eValue, sEnumVal);
			return sEnumVal;
		}		
		ST_SizeRepresents CSizeRepresents::FromString(const std::wstring &sValue)
		{
			FromXml_ST_SizeRepresents(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CSizeRepresents::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_SizeRepresents(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_Legend::CT_Legend()
		{
			m_layout = NULL;
		}
		CT_Legend::~CT_Legend()
		{
			for (size_t i = 0; i < m_legendEntry.size(); ++i)
				delete m_legendEntry[i];
			m_legendEntry.clear();
			
			if (NULL != m_layout)
				delete m_layout;
		}
		void CT_Legend::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"legendPos" == sName)
				{
					m_legendPos = oReader;
				}
				else if (L"legendEntry" == sName)
				{
					CT_LegendEntry* pNewElem = new CT_LegendEntry;
					pNewElem->fromXML(oReader);
					m_legendEntry.push_back(pNewElem);
				}
				else if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if (L"overlay" == sName)
				{
					m_overlay = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Legend::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");

			m_legendPos.toXML(L"c:legendPos", writer);

			for (size_t i = 0; i < m_legendEntry.size(); ++i)
			{
				CT_LegendEntry* pElem = m_legendEntry[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:legendEntry";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_layout)
			{
				std::wstring sNodeName = L"c:layout";
				m_layout->toXML(sNodeName, writer);
			}
			m_overlay.toXML(L"c:overlay", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Legend::getType() { return et_ct_Legend; }
		CT_Layout::CT_Layout()
		{
			m_manualLayout = NULL;
		}
		CT_Layout::~CT_Layout()
		{
			if (NULL != m_manualLayout)
				delete m_manualLayout;
		}
		void CT_Layout::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"manualLayout" == sName)
				{
					CT_ManualLayout* pNewElem = new CT_ManualLayout;
					pNewElem->fromXML(oReader);
					m_manualLayout = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Layout::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_manualLayout)
			{
				std::wstring sNodeName = L"c:manualLayout";
				m_manualLayout->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Layout::getType() { return et_ct_layout; }
		CT_ManualLayout::CT_ManualLayout()
		{
		}
		CT_ManualLayout::~CT_ManualLayout()
		{
		}
		void CT_ManualLayout::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"layoutTarget" == sName)
				{
					m_layoutTarget = oReader;
				}
				else if (L"xMode" == sName)
				{
					m_xMode = oReader;
				}
				else if (L"yMode" == sName)
				{
					m_yMode = oReader;
				}
				else if (L"wMode" == sName)
				{
					m_wMode = oReader;
				}
				else if (L"hMode" == sName)
				{
					m_hMode = oReader;
				}
				else if (L"x" == sName)
				{
					m_x = oReader;
				}
				else if (L"y" == sName)
				{
					m_y = oReader;
				}
				else if (L"w" == sName)
				{
					m_w = oReader;
				}
				else if (L"h" == sName)
				{
					m_h = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_ManualLayout::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_layoutTarget.toXML(L"c:layoutTarget", writer);
			m_xMode.toXML(L"c:xMode", writer);
			m_yMode.toXML(L"c:yMode", writer);
			m_wMode.toXML(L"c:wMode", writer);
			m_hMode.toXML(L"c:hMode", writer);
			m_x.toXML(L"c:x", writer);
			m_y.toXML(L"c:y", writer);
			m_w.toXML(L"c:w", writer);
			m_h.toXML(L"c:h", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ManualLayout::getType() { return et_ct_manuallayout; }
		
		ST_LayoutTarget CLayoutTarget::FromString(const std::wstring &sValue)
		{
			if ((L"inner") == sValue) this->m_eValue = st_layouttargetINNER;
			else if ((L"outer") == sValue) this->m_eValue = st_layouttargetOUTER;
			else this->m_eValue = st_layouttargetINNER;

			return this->m_eValue;
		}

		std::wstring CLayoutTarget::ToString() const
		{
			switch (this->m_eValue)
			{
			case st_layouttargetINNER: return (L"inner");
			case st_layouttargetOUTER: return (L"outer");
			default: return (L"inner");
			}
		}

		std::wstring CLayoutMode::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_LayoutMode(m_eValue, sEnumVal);
			return sEnumVal;
		}		
		ST_LayoutMode CLayoutMode::FromString(const std::wstring &sValue)
		{
			FromXml_ST_LayoutMode(sValue,  this->m_eValue);
			return this->m_eValue;
		}

		CT_DTable::CT_DTable()
		{
		}
		CT_DTable::~CT_DTable()
		{
		}
		void CT_DTable::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"showHorzBorder" == sName)
				{
					m_showHorzBorder = oReader;
				}
				else if (L"showVertBorder" == sName)
				{
					m_showVertBorder = oReader;
				}
				else if (L"showOutline" == sName)
				{
					m_showOutline = oReader;
				}
				else if (L"showKeys" == sName)
				{
					m_showKeys = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_DTable::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_showHorzBorder.toXML(L"c:showHorzBorder", writer);
			m_showVertBorder.toXML(L"c:showVertBorder", writer);
			m_showOutline.toXML(L"c:showOutline", writer);
			m_showKeys.toXML(L"c:showKeys", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				std::wstring sNodeName = L"c:extLst";
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DTable::getType() { return et_ct_dtable; }

		CBaseAx::CBaseAx()
		{
			m_scaling = NULL;
			m_majorGridlines = NULL;
			m_minorGridlines = NULL;
			m_title = NULL;
			m_numFmt = NULL;
		}
		CBaseAx::~CBaseAx()
		{
			if (NULL != m_scaling)
				delete m_scaling;
			if (NULL != m_majorGridlines)
				delete m_majorGridlines;
			if (NULL != m_minorGridlines)
				delete m_minorGridlines;
			if (NULL != m_title)
				delete m_title;
			if (NULL != m_numFmt)
				delete m_numFmt;
		}
		bool CBaseAx::fromXML(const std::wstring & sName, XmlUtils::CXmlLiteReader& oReader)
		{
			bool res = true;
			if (L"axId" == sName)
			{
				m_axId = oReader;
			}
			else if (L"scaling" == sName)
			{
				CT_Scaling* pNewElem = new CT_Scaling;
				pNewElem->fromXML(oReader);
				m_scaling = pNewElem;
			}
			else if (L"delete" == sName)
			{
				m_delete = oReader;
			}
			else if (L"axPos" == sName)
			{
				m_axPos = oReader;
			}
			else if (L"majorGridlines" == sName)
			{
				CT_ChartLines* pNewElem = new CT_ChartLines;
				pNewElem->fromXML(oReader);
				m_majorGridlines = pNewElem;
			}
			else if (L"minorGridlines" == sName)
			{
				CT_ChartLines* pNewElem = new CT_ChartLines;
				pNewElem->fromXML(oReader);
				m_minorGridlines = pNewElem;
			}
			else if (L"title" == sName)
			{
				CT_Title* pNewElem = new CT_Title;
				pNewElem->fromXML(oReader);
				m_title = pNewElem;
			}
			else if (L"numFmt" == sName)
			{
				CT_NumFmt* pNewElem = new CT_NumFmt;
				pNewElem->fromXML(oReader);
				m_numFmt = pNewElem;
			}
			else if (L"majorTickMark" == sName)
			{
				m_majorTickMark = oReader;
			}
			else if (L"minorTickMark" == sName)
			{
				m_minorTickMark = oReader;
			}
			else if (L"tickLblPos" == sName)
			{
				m_tickLblPos = oReader;
			}
			else if (L"spPr" == sName)
			{
				m_spPr = oReader;
			}
			else if (L"txPr" == sName)
			{
				m_txPr = oReader;
			}
			else if (L"crossAx" == sName)
			{
				m_crossAx = oReader;
			}
			else if (L"crosses" == sName)
			{
				m_crosses = oReader;
			}
			else if (L"crossesAt" == sName)
			{
				m_crossesAt = oReader;
			}
			else if (L"extLst" == sName)
			{
				m_extLst = oReader;
			}
			else
				res = false;
			return res;
		}
		void CBaseAx::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_axId.toXML(L"c:axId", writer);
			m_scaling->toXML(L"c:scaling", writer);
			m_delete.toXML(L"c:delete", writer);
			m_axPos.toXML(L"c:axPos", writer);

			if (NULL != m_majorGridlines)
			{
				m_majorGridlines->toXML(L"c:majorGridlines", writer);
			}
			if (NULL != m_minorGridlines)
			{
				m_minorGridlines->toXML(L"c:minorGridlines", writer);
			}
			if (NULL != m_title)
			{
				std::wstring sNodeName = L"c:title";
				m_title->toXML(sNodeName, writer);
			}
			if (NULL != m_numFmt)
			{
				std::wstring sNodeName = L"c:numFmt";
				m_numFmt->toXML(sNodeName, writer);
			}
			m_majorTickMark.toXML(L"c:majorTickMark", writer);
			m_minorTickMark.toXML(L"c:minorTickMark", writer);
			m_tickLblPos.toXML(L"c:tickLblPos", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			m_crossAx.toXML(L"c:crossAx", writer);
			m_crosses.toXML(L"c:crosses", writer);
			m_crossesAt.toXML(L"c:crossesAt", writer);
		}
		CT_SerAx::CT_SerAx()
		{
		}
		CT_SerAx::~CT_SerAx()
		{
		}
		void CT_SerAx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (CBaseAx::fromXML(sName, oReader))
				{

				}
				else if (L"tickLblSkip" == sName)
				{
					m_tickLblSkip = oReader;
				}
				else if (L"tickMarkSkip" == sName)
				{
					m_tickMarkSkip = oReader;
				}
			}
		}
		void CT_SerAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseAx::toXML(writer);
			
			if (m_tickLblSkip.IsInit())
			{
				std::wstring sNodeName = L"c:tickLblSkip";
				m_tickLblSkip.toXML(sNodeName, writer);
			}
			if (m_tickMarkSkip.IsInit())
			{
				std::wstring sNodeName = L"c:tickMarkSkip";
				m_tickMarkSkip.toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SerAx::getType() { return et_ct_serax; }
		CT_Scaling::CT_Scaling()
		{
		}
		CT_Scaling::~CT_Scaling()
		{
		}
		void CT_Scaling::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"logBase" == sName)
				{
					m_logBase = oReader;
				}
				else if (L"orientation" == sName)
				{
					m_orientation = oReader;
				}
				else if (L"max" == sName)
				{
					m_max = oReader;
				}
				else if (L"min" == sName)
				{
					m_min = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Scaling::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_logBase.toXML(L"c:logBase", writer);
			m_orientation.toXML(L"c:orientation", writer);
			m_max.toXML(L"c:max", writer);
			m_min.toXML(L"c:min", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Scaling::getType() { return et_ct_scaling; }

		ST_Orientation COrientation::FromString(const std::wstring &sValue)
		{
			FromXml_ST_Orientation(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring COrientation::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_Orientation(m_eValue, sEnumVal);
			return sEnumVal;
		}

		ST_AxPos CAxPos::FromString(const std::wstring &sValue)
		{
			FromXml_ST_AxPos(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CAxPos::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_AxPos(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_ChartLines::CT_ChartLines()
		{
		}
		CT_ChartLines::~CT_ChartLines()
		{
		}
		void CT_ChartLines::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
			}
		}
		void CT_ChartLines::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ChartLines::getType() { return et_ct_chartlines; }
		CT_Title::CT_Title()
		{
			m_tx = NULL;
			m_layout = NULL;
		}
		CT_Title::~CT_Title()
		{
			if (NULL != m_tx)
				delete m_tx;
			if (NULL != m_layout)
				delete m_layout;
		}
		void CT_Title::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"tx" == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if (L"overlay" == sName)
				{
					m_overlay = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Title::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_tx)
			{
				std::wstring sNodeName = L"c:tx";
				m_tx->toXML(sNodeName, writer);
			}
			if (NULL != m_layout)
			{
				std::wstring sNodeName = L"c:layout";
				m_layout->toXML(sNodeName, writer);
			}
			m_overlay.toXML(L"c:overlay", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Title::getType() { return et_ct_title; }
		CT_Tx::CT_Tx()
		{
			m_strRef = NULL;
		}
		CT_Tx::~CT_Tx()
		{
			if (NULL != m_strRef)
				delete m_strRef;
		}
		std::wstring CT_Tx::toXML() const
		{
			return L"";
		}
		void CT_Tx::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML(L"c:tx", writer);
		}
		void CT_Tx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"rich" == sName)
				{
					m_oRich = oReader;
				}
				else if (L"strRef" == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
			}
		}
		void CT_Tx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_oRich.IsInit())
			{
				m_oRich->m_name = L"c:rich";
				writer.WriteString(m_oRich->toXML());
			}
			if (NULL != m_strRef)
			{
				std::wstring sNodeName = L"c:strRef";
				m_strRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Tx::getType() { return et_ct_tx; }
		CT_StrRef::CT_StrRef()
		{
			m_strCache = NULL;
		}
		CT_StrRef::~CT_StrRef()
		{
			if (NULL != m_strCache)
				delete m_strCache;
		}
		void CT_StrRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"f" == sName)
				{
					m_f = oReader.GetText3();
				}
				else if (L"strCache" == sName)
				{
					CT_StrData* pNewElem = new CT_StrData;
					pNewElem->fromXML(oReader);
					m_strCache = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_StrRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_f.IsInit())
			{
				if (std::wstring::npos != m_f->find(' ')&&(  m_f->at(0) == ' ' || m_f->at(m_f->size()-1) == ' ')
				 || std::wstring::npos != m_f->find('\n')&&(  m_f->at(0) == '\n' || m_f->at(m_f->size()-1) == '\n'))
					writer.WriteString(L"<c:f xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:f>");
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(L"</c:f>");
			}
			if (NULL != m_strCache)
			{
				std::wstring sNodeName = L"c:strCache";
				m_strCache->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StrRef::getType() { return et_ct_strref; }
		CT_StrData::CT_StrData()
		{
		}
		CT_StrData::~CT_StrData()
		{
			for (size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
			m_pt.clear();
		}
		void CT_StrData::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"ptCount" == sName)
				{
					m_ptCount = oReader;
				}
				else if (L"pt" == sName)
				{
					CT_StrVal* pNewElem = new CT_StrVal;
					pNewElem->fromXML(oReader);
					m_pt.push_back(pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		std::wstring CT_StrData::toXML() const
		{
			return L"";
		}
		void CT_StrData::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML(L"c:strData", writer);
		}
		void CT_StrData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_ptCount.toXML(L"c:ptCount", writer);

			for (size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_StrVal* pElem = m_pt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:pt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StrData::getType() { return et_ct_strdata; }
		CT_StrVal::CT_StrVal()
		{
		}
		CT_StrVal::~CT_StrVal()
		{
		}
		void CT_StrVal::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"v" == sName)
				{
					m_v = oReader.GetText3();
				}
			}
		}
		void CT_StrVal::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (m_idx.IsInit())
			{
				WritingStringAttrInt(L"idx", *m_idx);
			}
			writer.WriteString(L">");
			
			if (m_v.IsInit())
			{
				if (std::wstring::npos != m_v->find(' ')&&(  m_v->at(0) == ' ' || m_v->at(m_v->size()-1) == ' ')
				 || std::wstring::npos != m_v->find('\n')&&(  m_v->at(0) == '\n' || m_v->at(m_v->size()-1) == '\n'))
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
		EElementType CT_StrVal::getType() { return et_ct_strval; }
		void CT_StrVal::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				if (L"idx" == wsName)
				{
					m_idx = oReader.GetText();
				}
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_NumFmt::CT_NumFmt()
		{
		}
		CT_NumFmt::~CT_NumFmt()
		{
		}
		void CT_NumFmt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_NumFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (m_formatCode.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"formatCode", *m_formatCode);
			}
			if (m_sourceLinked.IsInit())
			{
				std::wstring sVal;
				if (*m_sourceLinked)
					sVal = L"1";
				else
					sVal = L"0";
				WritingStringAttrString(L"sourceLinked", sVal);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_NumFmt::getType() { return et_ct_numfmt; }
		void CT_NumFmt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				if (L"formatCode" == wsName)
				{
					m_formatCode = oReader.GetText();
				}
				else if (L"sourceLinked" == wsName)
				{
					m_sourceLinked = oReader.GetText();
				}
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		ST_TickMark CTickMark::FromString(const std::wstring &sValue)
		{
			FromXml_ST_TickMark(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CTickMark::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_TickMark(m_eValue, sEnumVal);
			return sEnumVal;
		}
		ST_TickLblPos CTickLblPos::FromString(const std::wstring &sValue)
		{
			FromXml_ST_TickLblPos(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CTickLblPos::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_TickLblPos(m_eValue, sEnumVal);
			return sEnumVal;
		}
		ST_Crosses CCrosses::FromString(const std::wstring &sValue)
		{
			FromXml_ST_Crosses(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CCrosses::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_Crosses(m_eValue, sEnumVal);
			return sEnumVal;
		}
		ST_TimeUnit CTimeUnit::FromString(const std::wstring &sValue)
		{
			FromXml_ST_TimeUnit(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CTimeUnit::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_TimeUnit(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_DateAx::CT_DateAx()
		{
		}
		CT_DateAx::~CT_DateAx()
		{
		}
		void CT_DateAx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (CBaseAx::fromXML(sName, oReader))
				{

				}
				else if (L"auto" == sName)
				{
					m_auto = oReader;
				}
				else if (L"lblOffset" == sName)
				{
					m_lblOffset = oReader;
				}
				else if (L"baseTimeUnit" == sName)
				{
					m_baseTimeUnit = oReader;
				}
				else if (L"majorUnit" == sName)
				{
					m_majorUnit = oReader;
				}
				else if (L"majorTimeUnit" == sName)
				{
					m_majorTimeUnit = oReader;
				}
				else if (L"minorUnit" == sName)
				{
					m_minorUnit = oReader;
				}
				else if (L"minorTimeUnit" == sName)
				{
					m_minorTimeUnit = oReader;
				}
			}
		}
		void CT_DateAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");

			CBaseAx::toXML(writer);
			
			m_auto.toXML(L"c:auto", writer);
			m_lblOffset.toXML(L"c:lblOffset", writer);
			m_baseTimeUnit.toXML(L"c:baseTimeUnit", writer);
			m_majorUnit.toXML(L"c:majorUnit", writer);
			m_majorTimeUnit.toXML(L"c:majorTimeUnit", writer);
			m_minorUnit.toXML(L"c:minorUnit", writer);
			m_minorTimeUnit.toXML(L"c:minorTimeUnit", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DateAx::getType() { return et_ct_dateax; }
		
		ST_LblAlgn CLblAlgn::FromString(const std::wstring &sValue)
		{
			FromXml_ST_LblAlgn(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CLblAlgn::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_LblAlgn(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_CatAx::CT_CatAx()
		{
		}
		CT_CatAx::~CT_CatAx()
		{
		}
		void CT_CatAx::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (CBaseAx::fromXML(sName, oReader))
				{

				}
				else if (L"auto" == sName)
				{
					m_auto = oReader;
				}
				else if (L"lblAlgn" == sName)
				{
					m_lblAlgn = oReader;
				}
				else if (L"lblOffset" == sName)
				{
					m_lblOffset = oReader;
				}
				else if (L"tickLblSkip" == sName)
				{
					m_tickLblSkip = oReader;
				}
				else if (L"tickMarkSkip" == sName)
				{
					m_tickMarkSkip = oReader;
				}
				else if (L"noMultiLvlLbl" == sName)
				{
					m_noMultiLvlLbl = oReader;
				}
			}
		}
		void CT_CatAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseAx::toXML(writer);

			m_auto.toXML(L"c:auto", writer);
			m_lblAlgn.toXML(L"c:lblAlgn", writer);
			m_lblOffset.toXML(L"c:lblOffset", writer);
			if (m_tickLblSkip.IsInit())
			{
				m_tickLblSkip.toXML(L"c:tickLblSkip", writer);
			}
			if (m_tickMarkSkip.IsInit())
			{
				m_tickMarkSkip.toXML(L"c:tickMarkSkip", writer);
			}
			m_noMultiLvlLbl.toXML(L"c:noMultiLvlLbl", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_CatAx::getType() { return et_ct_catax; }
		CT_DispUnitsLbl::CT_DispUnitsLbl()
		{
			m_layout = NULL;
			m_tx = NULL;
		}
		CT_DispUnitsLbl::~CT_DispUnitsLbl()
		{
			if (NULL != m_layout)
				delete m_layout;
			if (NULL != m_tx)
				delete m_tx;
		}
		void CT_DispUnitsLbl::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if (L"tx" == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
			}
		}
		void CT_DispUnitsLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_layout)
			{
				std::wstring sNodeName = L"c:layout";
				m_layout->toXML(sNodeName, writer);
			}
			if (NULL != m_tx)
			{
				std::wstring sNodeName = L"c:tx";
				m_tx->toXML(sNodeName, writer);
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DispUnitsLbl::getType() { return et_ct_dispunitslbl; }
		
		ST_BuiltInUnit CBuiltInUnit::FromString(const std::wstring &sValue)
		{
			FromXml_ST_BuiltInUnit(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CBuiltInUnit::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_BuiltInUnit(m_eValue, sEnumVal);
			return sEnumVal;
		}		
		
		CT_DispUnits::CT_DispUnits()
		{
			m_dispUnitsLbl = NULL;
		}
		CT_DispUnits::~CT_DispUnits()
		{
			if (NULL != m_dispUnitsLbl)
				delete m_dispUnitsLbl;
		}
		void CT_DispUnits::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"builtInUnit" == sName)
				{
					m_builtInUnit = oReader;
				}
				else if (L"custUnit" == sName)
				{
					m_custUnit = oReader;
				}
				else if (L"dispUnitsLbl" == sName)
				{
					CT_DispUnitsLbl* pNewElem = new CT_DispUnitsLbl;
					pNewElem->fromXML(oReader);
					m_dispUnitsLbl = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_DispUnits::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");

			m_builtInUnit.toXML(L"c:builtInUnit", writer);
			m_custUnit.toXML(L"c:custUnit", writer);

			if (NULL != m_dispUnitsLbl)
			{
				m_dispUnitsLbl->toXML(L"c:dispUnitsLbl", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DispUnits::getType() { return et_ct_dispunits; }
		
		ST_CrossBetween CCrossBetween::FromString(const std::wstring &sValue)
		{
			FromXml_ST_CrossBetween(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CCrossBetween::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_CrossBetween(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_ValAx::CT_ValAx()
		{
			m_dispUnits = NULL;
		}
		CT_ValAx::~CT_ValAx()
		{
			if (m_dispUnits)
				delete m_dispUnits;
		}
		void CT_ValAx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (CBaseAx::fromXML(sName, oReader))
				{

				}
				else if (L"crossBetween" == sName)
				{
					m_crossBetween = oReader;
				}
				else if (L"majorUnit" == sName)
				{
					m_majorUnit = oReader;
				}
				else if (L"minorUnit" == sName)
				{
					m_minorUnit = oReader;
				}
				else if (L"dispUnits" == sName)
				{
					CT_DispUnits* pNewElem = new CT_DispUnits;
					pNewElem->fromXML(oReader);
					m_dispUnits = pNewElem;
				}
			}
		}
		void CT_ValAx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseAx::toXML(writer);

			m_crossBetween.toXML(L"c:crossBetween", writer);
			m_majorUnit.toXML(L"c:majorUnit", writer);
			m_minorUnit.toXML(L"c:minorUnit", writer);
			if (NULL != m_dispUnits)
			{
				m_dispUnits->toXML(L"c:dispUnits", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ValAx::getType() { return et_ct_valax; }

		CBaseSer::CBaseSer()
		{
			m_tx = NULL;
		}
		CBaseSer::~CBaseSer()
		{
			if (NULL != m_tx)
				delete m_tx;
		}
		bool CBaseSer::fromXML(const std::wstring sName, XmlUtils::CXmlLiteReader& oReader)
		{
			bool res = true;
			if (L"idx" == sName)
			{
				m_idx = oReader;
			}
			else if (L"order" == sName)
			{
				m_order = oReader;
			}
			else if (L"tx" == sName)
			{
				m_tx = new CT_SerTx;
				m_tx->fromXML(oReader);
			}
			else if (L"spPr" == sName)
			{
				m_spPr = oReader;
			}
			else if (L"extLst" == sName)
			{
				m_extLst = oReader;
			}
			else
				res = false;
			return res;
		}
		void CBaseSer::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_idx.toXML(L"c:idx", writer);
			m_order.toXML(L"c:order", writer);

			if (NULL != m_tx)
			{
				std::wstring sNodeName = L"c:tx";
				m_tx->toXML(sNodeName, writer);
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
		}

		CT_BubbleSer::CT_BubbleSer()
		{
			m_dLbls = NULL;
			m_xVal = NULL;
			m_yVal = NULL;
			m_bubbleSize = NULL;
		}
		CT_BubbleSer::~CT_BubbleSer()
		{
			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
			for (size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
			m_trendline.clear();
			for (size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
			m_errBars.clear();
			if (NULL != m_xVal)
				delete m_xVal;
			if (NULL != m_yVal)
				delete m_yVal;
			if (NULL != m_bubbleSize)
				delete m_bubbleSize;
		}
		void CT_BubbleSer::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"invertIfNegative" == sName)
				{
					m_invertIfNegative = oReader;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"trendline" == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
					m_trendline.push_back(pNewElem);
				}
				else if (L"errBars" == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars.push_back(pNewElem);
				}
				else if (L"xVal" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_xVal = pNewElem;
				}
				else if (L"yVal" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_yVal = pNewElem;
				}
				else if (L"bubbleSize" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_bubbleSize = pNewElem;
				}
				else if (L"bubble3D" == sName)
				{
					m_bubble3D = oReader;
				}
			}
		}
		void CT_BubbleSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");

			CBaseSer::toXML(writer);
			
			m_invertIfNegative.toXML(L"c:invertIfNegative", writer);
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dPt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:trendline";
					pElem->toXML(sNodeName, writer);
				}
			}
			for (size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:errBars";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_xVal)
			{
				std::wstring sNodeName = L"c:xVal";
				m_xVal->toXML(sNodeName, writer);
			}
			if (NULL != m_yVal)
			{
				std::wstring sNodeName = L"c:yVal";
				m_yVal->toXML(sNodeName, writer);
			}
			if (NULL != m_bubbleSize)
			{
				std::wstring sNodeName = L"c:bubbleSize";
				m_bubbleSize->toXML(sNodeName, writer);
			}
			m_bubble3D.toXML(L"c:bubble3D", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BubbleSer::getType() { return et_ct_bubbleser; }
		CT_SerTx::CT_SerTx()
		{
			m_strRef = NULL;
		}
		CT_SerTx::~CT_SerTx()
		{
			if (NULL != m_strRef)
				delete m_strRef;
		}
		void CT_SerTx::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"strRef" == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
				else if (L"v" == sName)
				{
					m_v = oReader.GetText3();
				}
			}
		}
		void CT_SerTx::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_strRef)
			{
				std::wstring sNodeName = L"c:strRef";
				m_strRef->toXML(sNodeName, writer);
			}
			if (m_v.IsInit())
			{
				if (std::wstring::npos != m_v->find(' ')&&(  m_v->at(0) == ' ' || m_v->at(m_v->size()-1) == ' ')
				 || std::wstring::npos != m_v->find('\n')&&(  m_v->at(0) == '\n' || m_v->at(m_v->size()-1) == '\n'))
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
		EElementType CT_SerTx::getType() { return et_ct_sertx; }
		
		CT_DPt::CT_DPt()
		{
			m_marker = NULL;
			m_pictureOptions = NULL;
		}
		CT_DPt::~CT_DPt()
		{
			if (NULL != m_marker)
				delete m_marker;
			if (NULL != m_pictureOptions)
				delete m_pictureOptions;
		}
		void CT_DPt::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"idx" == sName)
				{
					m_idx = oReader;
				}
				else if (L"invertIfNegative" == sName)
				{
					m_invertIfNegative = oReader;
				}
				else if (L"marker" == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if (L"bubble3D" == sName)
				{
					m_bubble3D = oReader;
				}
				else if (L"explosion" == sName)
				{
					m_explosion = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"pictureOptions" == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_DPt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_idx.toXML(L"c:idx", writer);
			m_invertIfNegative.toXML(L"c:invertIfNegative", writer);

			if (NULL != m_marker)
			{
				m_marker->toXML(L"c:marker", writer);
			}
			m_bubble3D.toXML(L"c:bubble3D", writer);
			m_explosion.toXML(L"c:explosion", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (NULL != m_pictureOptions)
			{
				m_pictureOptions->toXML(L"c:pictureOptions", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DPt::getType() { return et_ct_dpt; }
		CT_Marker::CT_Marker()
		{
		}
		CT_Marker::~CT_Marker()
		{
		}
		void CT_Marker::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"symbol" == sName)
				{
					m_symbol = oReader;
				}
				else if (L"size" == sName)
				{
					m_size = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		std::wstring CT_Marker::toXML() const
		{
			return L"";
		}
		void CT_Marker::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML(L"c:marker", writer);
		}
		void CT_Marker::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_symbol.toXML(L"c:symbol", writer);
			m_size.toXML(L"c:size", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Marker::getType() { return et_ct_marker; }

		ST_MarkerStyle CMarkerStyle::FromString(const std::wstring &sValue)
		{
			FromXml_ST_MarkerStyle(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CMarkerStyle::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_MarkerStyle(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_PictureOptions::CT_PictureOptions()
		{
		}
		CT_PictureOptions::~CT_PictureOptions()
		{
		}
		void CT_PictureOptions::fromXML(XmlUtils::CXmlLiteReader& oReader) {
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"applyToFront" == sName)
				{
					m_applyToFront = oReader;
				}
				else if (L"applyToSides" == sName)
				{
					m_applyToSides = oReader;
				}
				else if (L"applyToEnd" == sName)
				{
					m_applyToEnd = oReader;
				}
				else if (L"pictureFormat" == sName)
				{
					m_pictureFormat = oReader;
				}
				else if (L"pictureStackUnit" == sName)
				{
					m_pictureStackUnit = oReader;
				}
			}
		}
		void CT_PictureOptions::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");			
			
			m_applyToFront.toXML(L"c:applyToFront", writer);
			m_applyToSides.toXML(L"c:applyToSides", writer);
			m_applyToEnd.toXML(L"c:applyToEnd", writer);
			m_pictureFormat.toXML(L"c:pictureFormat", writer);
			m_pictureStackUnit.toXML(L"c:pictureStackUnit", writer);

			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PictureOptions::getType() { return et_ct_pictureoptions; }		
		
		ST_PictureFormat CPictureFormat::FromString(const std::wstring &sValue)
		{
			FromXml_ST_PictureFormat(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CPictureFormat::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_PictureFormat(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_DLbls::CT_DLbls()
		{
		}
		CT_DLbls::~CT_DLbls()
		{
			for (size_t i = 0; i < m_dLbl.size(); ++i)
				delete m_dLbl[i];
			m_dLbl.clear();
			
			for (size_t i = 0, length = m_Items.size(), length2 = m_ItemsElementName0.size(); i < length && i < length2; ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
			m_Items.clear();
			
			for (size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
			m_ItemsElementName0.clear();
		}
		void CT_DLbls::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"dLbl" == sName)
				{
					CT_DLbl* pNewElem = new CT_DLbl;
					pNewElem->fromXML(oReader);
					m_dLbl.push_back(pNewElem);
				}
				else if (L"dLblPos" == sName)
				{
					nullableComplexVal<CDLblPos> pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3DLBLPOS;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"delete" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3DELETE;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"leaderLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3LEADERLINES;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"numFmt" == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3NUMFMT;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"separator" == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SEPARATOR;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showBubbleSize" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWBUBBLESIZE;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showCatName" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWCATNAME;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showLeaderLines" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWLEADERLINES;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showLegendKey" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWLEGENDKEY;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showPercent" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWPERCENT;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showSerName" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWSERNAME;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showVal" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SHOWVAL;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"spPr" == sName)
				{
					PPTX::Logic::SpPr* pNewElem = new PPTX::Logic::SpPr;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3SPPR;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"txPr" == sName)
				{
					PPTX::Logic::TxBody* pNewElem = new PPTX::Logic::TxBody;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);

					ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
					*eElemtype = itemschoicetype3TXPR;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_DLbls::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			for (size_t i = 0; i < m_dLbl.size(); ++i)
			{
				CT_DLbl* pElem = m_dLbl[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dLbl";
					pElem->toXML(sNodeName, writer);
				}
			}
			for (size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType3 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i]);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DLbls::getType() { return et_ct_dlbls; }
		
		void CT_DLbls::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType3 eType, void* pVal) const 
		{
			switch (eType)
			{
				case itemschoicetype3DLBLPOS:
				{
					CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:dLblPos";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + pTypeVal->ToString() + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3DELETE:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:delete";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3LEADERLINES:
				{
					CT_ChartLines* pTypeVal = static_cast<CT_ChartLines*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:leaderLines";
							pTypeVal->toXML(sNodeName, writer);
						}
					}
				}break;				
				case itemschoicetype3NUMFMT:
				{
					CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:numFmt";
							pTypeVal->toXML(sNodeName, writer);
						}
					}
				}break;				
				case itemschoicetype3SEPARATOR:
				{
					std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
					if (NULL != pTypeVal)
					{
						if (true == bIsAttribute)
						{
							WritingStringAttrEncodeXmlString(L"c:separator", *pTypeVal);
						}
						else
						{
							if (std::wstring::npos != pTypeVal->find(' ')&&(  pTypeVal->at(0) == ' ' || pTypeVal->at(pTypeVal->size()-1) == ' ')
				 			|| std::wstring::npos != pTypeVal->find('\n')&&(  pTypeVal->at(0) == '\n' || pTypeVal->at(pTypeVal->size()-1) == '\n'))
								writer.WriteString(L"<c:separator xml:space=\"preserve\">");
							else
								writer.WriteString(L"<c:separator>");
							writer.WriteEncodeXmlString(*pTypeVal);
							writer.WriteString(L"</c:separator>");
						}
					}
				}break;				
				case itemschoicetype3SHOWBUBBLESIZE:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showBubbleSize";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWCATNAME:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showCatName";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWLEADERLINES:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showLeaderLines";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWLEGENDKEY:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showLegendKey";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWPERCENT:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showPercent";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWSERNAME:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showSerName";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SHOWVAL:
				{
					bool* pTypeVal = static_cast<bool*>(pVal);
					if (NULL != pTypeVal)
					{
						if (false == bIsAttribute)
						{
							std::wstring sNodeName = L"c:showVal";
							writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
						}
					}
				}break;				
				case itemschoicetype3SPPR:
				{
					PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
					if (NULL != pTypeVal)
					{
						writer.WriteString(pTypeVal->toXML());
					}
				}break;				
				case itemschoicetype3TXPR:
				{
					PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
					if (NULL != pTypeVal)
					{
						pTypeVal->m_name = L"c:txPr";
						writer.WriteString(pTypeVal->toXML());
					}
				}break;				
			}
		}
		void CT_DLbls::toDelete(ItemsChoiceType3 eType, void* pVal) 
		{
			switch (eType)
			{
			case itemschoicetype3DLBLPOS:
			{
				CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3DELETE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
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
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWCATNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWLEADERLINES:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWLEGENDKEY:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWPERCENT:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWSERNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}
			break;
			case itemschoicetype3SHOWVAL:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
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
	//ext
			m_leaderLines = NULL;
		}
		CT_DLbl::~CT_DLbl()
		{
		
			if (NULL != m_leaderLines)
				delete m_leaderLines;

			for (size_t i = 0; i < m_Items.size(), i < m_ItemsElementName0.size(); ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
			m_Items.clear();
			
			for (size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
			m_ItemsElementName0.clear();
		}
		void CT_DLbl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"idx" == sName)
				{
					m_idx = oReader;
				}
				else if (L"dLblPos" == sName)
				{
					nullableComplexVal<CDLblPos> pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4DLBLPOS;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"delete" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4DELETE;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4LAYOUT;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"numFmt" == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4NUMFMT;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"separator" == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SEPARATOR;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showBubbleSize" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWBUBBLESIZE;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showCatName" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWCATNAME;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showLegendKey" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWLEGENDKEY;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showPercent" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWPERCENT;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showSerName" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWSERNAME;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"showVal" == sName)
				{
					nullableBoolVal pNewElem = oReader;
					m_Items.push_back(pNewElem.GetPointerEmptyNullable());

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SHOWVAL;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"spPr" == sName)
				{
					PPTX::Logic::SpPr* pNewElem = new PPTX::Logic::SpPr;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4SPPR;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"tx" == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4TX;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"txPr" == sName)
				{
					PPTX::Logic::TxBody* pNewElem = new PPTX::Logic::TxBody;
					pNewElem->fromXML(oReader);
					m_Items.push_back(pNewElem);

					ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
					*eElemtype = itemschoicetype4TXPR;
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
				else if (L"xForSave" == sName)
				{
					m_xForSave = oReader;
				}
				else if (L"showDataLabelsRange" == sName)
				{
					m_showDataLabelsRange = oReader;
				}
				else if (L"showLeaderLines" == sName)
				{
					m_showLeaderLines = oReader;
				}
				else if (L"leaderLines" == sName)
				{
					m_leaderLines = new CT_ChartLines;
					m_leaderLines->fromXML(oReader);
				}
			}
		}
		void CT_DLbl::toXMLEntry(const std::wstring& sNodeNS, NSStringUtils::CStringBuilder& writer) const
		{
			if (m_idx.IsInit())
			{
				if (*m_idx > 0x7fffffff)
				{
					writer.WriteString(L"<" + sNodeNS + L":idx val=\"" + std::to_wstring((_INT32)*m_idx) + L"\"/>");
				}
				else
					m_idx.toXML(sNodeNS + L":idx", writer);
			}

			for (size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType4 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i], sNodeNS);
			}
			m_xForSave.toXML(sNodeNS + L":xForSave", writer);
			m_showDataLabelsRange.toXML(sNodeNS + L":showDataLabelsRange", writer);
			m_showLeaderLines.toXML(sNodeNS + L":showLeaderLines", writer);
			
			if (m_leaderLines)
				m_leaderLines->toXML(sNodeNS + L":leaderLines", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(sNodeNS + L":"));
			}
		}
		std::wstring CT_DLbl::toXML() const
		{
			return L"";
		}
		void CT_DLbl::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML(L"c:dlbl", writer);
		}
		void CT_DLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");

			toXMLEntry(L"c", writer);

			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DLbl::getType() { return et_ct_dlbl; }

		void CT_DLbl::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType4 eType, void* pVal, const std::wstring & nodeNS) const
		{
			switch (eType)
			{
			case itemschoicetype4DLBLPOS:
			{
				CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":dLblPos";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + pTypeVal->ToString() + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4DELETE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":delete";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4LAYOUT:
			{
				CT_Layout* pTypeVal = static_cast<CT_Layout*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":layout";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype4NUMFMT:
			{
				CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":numFmt";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype4SEPARATOR:
			{
				std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
				if (NULL != pTypeVal)
				{
					if (true == bIsAttribute)
					{
						WritingStringAttrEncodeXmlString(L"c:separator", *pTypeVal);
					}
					else
					{
						if (std::wstring::npos != pTypeVal->find(' ')&&(  pTypeVal->at(0) == ' ' || pTypeVal->at(pTypeVal->size()-1) == ' ')
				 			|| std::wstring::npos != pTypeVal->find('\n')&&(  pTypeVal->at(0) == '\n' || pTypeVal->at(pTypeVal->size()-1) == '\n'))
							writer.WriteString(L"<c:separator xml:space=\"preserve\">");
						else
							writer.WriteString(L"<c:separator>");
						writer.WriteEncodeXmlString(*pTypeVal);
						writer.WriteString(L"</c:separator>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWBUBBLESIZE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showBubbleSize";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWCATNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showCatName";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWLEGENDKEY:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showLegendKey";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWPERCENT:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showPercent";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWSERNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showSerName";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SHOWVAL:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":showVal";
						writer.WriteString(L"<" + sNodeName + L" val=\"" + (*pTypeVal ? L"1" : L"0") + L"\"/>");
					}
				}
			}
			break;
			case itemschoicetype4SPPR:
			{
				PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
				if (NULL != pTypeVal)
				{
					pTypeVal->m_namespace = nodeNS;
					writer.WriteString(pTypeVal->toXML());
				}
			}
			break;
			case itemschoicetype4TX:
			{
				CT_Tx* pTypeVal = static_cast<CT_Tx*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = nodeNS + L":tx";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype4TXPR:
			{
				PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
				if (NULL != pTypeVal)
				{
					pTypeVal->m_name = nodeNS + L":txPr";
					writer.WriteString(pTypeVal->toXML());
				}
			}
			break;
			}
		}
		void CT_DLbl::toDelete(ItemsChoiceType4 eType, void* pVal) 
		{
			switch (eType)
			{
			case itemschoicetype4DLBLPOS:
			{
				CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4DELETE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4LAYOUT:
			{
				CT_Layout* pTypeVal = static_cast<CT_Layout*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4NUMFMT:
			{
				CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SEPARATOR:
			{
				std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWBUBBLESIZE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWCATNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWLEGENDKEY:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWPERCENT:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWSERNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SHOWVAL:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4SPPR:
			{
				PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4TX:
			{
				CT_Tx* pTypeVal = static_cast<CT_Tx*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			case itemschoicetype4TXPR:
			{
				PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
				RELEASEOBJECT(pTypeVal);
			}break;			
			}
		}
		
		ST_DLblPos CDLblPos::FromString(const std::wstring &sValue)
		{
			FromXml_ST_DLblPos(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CDLblPos::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_DLblPos(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_Trendline::CT_Trendline()
		{
			m_trendlineLbl = NULL;
		}
		CT_Trendline::~CT_Trendline()
		{
			if (NULL != m_trendlineLbl)
				delete m_trendlineLbl;
		}
		void CT_Trendline::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"name" == sName)
				{
					std::wstring* pNewElem = new std::wstring;
					std::wstring sVal = oReader.GetText3();
					*pNewElem = sVal;
					m_name = pNewElem;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"trendlineType" == sName)
				{
					m_trendlineType = oReader;
				}
				else if (L"order" == sName)
				{
					m_order = oReader;
				}
				else if (L"period" == sName)
				{
					m_period = oReader;
				}
				else if (L"forward" == sName)
				{
					m_forward = oReader;
				}
				else if (L"backward" == sName)
				{
					m_backward = oReader;
				}
				else if (L"intercept" == sName)
				{
					m_intercept = oReader;
				}
				else if (L"dispRSqr" == sName)
				{
					m_dispRSqr = oReader;
				}
				else if (L"dispEq" == sName)
				{
					m_dispEq = oReader;
				}
				else if (L"trendlineLbl" == sName)
				{
					CT_TrendlineLbl* pNewElem = new CT_TrendlineLbl;
					pNewElem->fromXML(oReader);
					m_trendlineLbl = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Trendline::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			if (m_name.IsInit())
			{
				if (std::wstring::npos != m_name->find(' ')&&(  m_name->at(0) == ' ' || m_name->at(m_name->size()-1) == ' ')
				 			|| std::wstring::npos != m_name->find('\n')&&(  m_name->at(0) == '\n' || m_name->at(m_name->size()-1) == '\n'))
					writer.WriteString(L"<c:name xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:name>");
				writer.WriteEncodeXmlString(*m_name);
				writer.WriteString(L"</c:name>");
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			m_trendlineType.toXML(L"c:trendlineType", writer);
			m_order.toXML(L"c:order", writer);
			m_period.toXML(L"c:period", writer);
			m_forward.toXML(L"c:forward", writer);
			m_backward.toXML(L"c:backward", writer);
			m_intercept.toXML(L"c:intercept", writer);
			m_dispRSqr.toXML(L"c:dispRSqr", writer);
			m_dispEq.toXML(L"c:dispEq", writer);

			if (NULL != m_trendlineLbl)
			{
				m_trendlineLbl->toXML(L"c:trendlineLbl", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Trendline::getType() { return et_ct_trendline; }
		
		ST_TrendlineType CTrendlineType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_TrendlineType(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CTrendlineType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_TrendlineType(m_eValue, sEnumVal);
			return sEnumVal;
		}	

		CT_TrendlineLbl::CT_TrendlineLbl()
		{
			m_layout = NULL;
			m_tx = NULL;
			m_numFmt = NULL;
		}
		CT_TrendlineLbl::~CT_TrendlineLbl()
		{
			if (NULL != m_layout)
				delete m_layout;
			if (NULL != m_tx)
				delete m_tx;
			if (NULL != m_numFmt)
				delete m_numFmt;
		}
		void CT_TrendlineLbl::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if (L"tx" == sName)
				{
					CT_Tx* pNewElem = new CT_Tx;
					pNewElem->fromXML(oReader);
					m_tx = pNewElem;
				}
				else if (L"numFmt" == sName)
				{
					CT_NumFmt* pNewElem = new CT_NumFmt;
					pNewElem->fromXML(oReader);
					m_numFmt = pNewElem;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_TrendlineLbl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_layout)
			{
				std::wstring sNodeName = L"c:layout";
				m_layout->toXML(sNodeName, writer);
			}
			if (NULL != m_tx)
			{
				std::wstring sNodeName = L"c:tx";
				m_tx->toXML(sNodeName, writer);
			}
			if (NULL != m_numFmt)
			{
				std::wstring sNodeName = L"c:numFmt";
				m_numFmt->toXML(sNodeName, writer);
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_TrendlineLbl::getType() { return et_ct_trendlinelbl; }
		
		CT_ErrBars::CT_ErrBars()
		{
			m_plus = NULL;
			m_minus = NULL;
		}
		CT_ErrBars::~CT_ErrBars()
		{
			if (NULL != m_plus)
				delete m_plus;
			if (NULL != m_minus)
				delete m_minus;
		}
		void CT_ErrBars::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"errDir" == sName)
				{
					m_errDir = oReader;
				}
				else if (L"errBarType" == sName)
				{
					m_errBarType = oReader;
				}
				else if (L"errValType" == sName)
				{
					m_errValType = oReader;
				}
				else if (L"noEndCap" == sName)
				{
					m_noEndCap = oReader;
				}
				else if (L"plus" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_plus = pNewElem;
				}
				else if (L"minus" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_minus = pNewElem;
				}
				else if (L"val" == sName)
				{
					m_val = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_ErrBars::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_errDir.toXML(L"c:errDir", writer);
			m_errBarType.toXML(L"c:errBarType", writer);
			m_errValType.toXML(L"c:errValType", writer);
			m_noEndCap.toXML(L"c:noEndCap", writer);

			if (NULL != m_plus)
			{
				m_plus->toXML(L"c:plus", writer);
			}
			if (NULL != m_minus)
			{
				m_minus->toXML(L"c:minus", writer);
			}
			m_val.toXML(L"c:val", writer);
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ErrBars::getType() { return et_ct_errbars; }
		
		ST_ErrDir CErrDir::FromString(const std::wstring &sValue)
		{
			FromXml_ST_ErrDir(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CErrDir::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_ErrDir(m_eValue, sEnumVal);
			return sEnumVal;
		}
		ST_ErrBarType CErrBarType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_ErrBarType(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CErrBarType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_ErrBarType(m_eValue, sEnumVal);
			return sEnumVal;
		}

		ST_ErrValType CErrValType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_ErrValType(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CErrValType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_ErrValType(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_NumDataSource::CT_NumDataSource()
		{
			m_numLit = NULL;
			m_numRef = NULL;
		}
		CT_NumDataSource::~CT_NumDataSource()
		{
			if (NULL != m_numLit)
				delete m_numLit;
			if (NULL != m_numRef)
				delete m_numRef;
		}
		void CT_NumDataSource::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"numLit" == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numLit = pNewElem;
				}
				else if (L"numRef" == sName)
				{
					CT_NumRef* pNewElem = new CT_NumRef;
					pNewElem->fromXML(oReader);
					m_numRef = pNewElem;
				}
			}
		}
		void CT_NumDataSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_numLit)
			{
				std::wstring sNodeName = L"c:numLit";
				m_numLit->toXML(sNodeName, writer);
			}
			if (NULL != m_numRef)
			{
				std::wstring sNodeName = L"c:numRef";
				m_numRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumDataSource::getType() { return et_ct_numdatasource; }
		
		CT_NumData::CT_NumData()
		{
		}
		CT_NumData::~CT_NumData()
		{
			for (size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
			m_pt.clear();
		}
		void CT_NumData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"formatCode" == sName)
				{
					m_formatCode = oReader.GetText3();
				}
				else if (L"ptCount" == sName)
				{
					m_ptCount = oReader;
				}
				else if (L"pt" == sName)
				{
					CT_NumVal* pNewElem = new CT_NumVal;
					pNewElem->fromXML(oReader);
					m_pt.push_back(pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_NumData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_formatCode.IsInit())
			{
				if (std::wstring::npos != m_formatCode->find(' ')&&(  m_formatCode->at(0) == ' ' || m_formatCode->at(m_formatCode->size()-1) == ' ')
				 || std::wstring::npos != m_formatCode->find('\n')&&(  m_formatCode->at(0) == '\n' || m_formatCode->at(m_formatCode->size()-1) == '\n'))
					writer.WriteString(L"<c:formatCode xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:formatCode>");
				writer.WriteEncodeXmlString(*m_formatCode);
				writer.WriteString(L"</c:formatCode>");
			}
			m_ptCount.toXML(L"c:ptCount", writer);

			for (size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_NumVal* pElem = m_pt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:pt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumData::getType() { return et_ct_numdata; }
		
		CT_NumVal::CT_NumVal()
		{
		}
		CT_NumVal::~CT_NumVal()
		{
		}
		void CT_NumVal::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"v" == sName)
				{
					m_v = oReader.GetText3();
				}
			}
		}
		void CT_NumVal::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			if (m_idx.IsInit())
			{
				WritingStringAttrInt(L"idx", *m_idx);
			}
			if (m_formatCode.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"formatCode", *m_formatCode);
			}
			writer.WriteString(L">");
			if (m_v.IsInit())
			{
				if (std::wstring::npos != m_v->find(' ')&&(  m_v->at(0) == ' ' || m_v->at(m_v->size()-1) == ' ')
				 || std::wstring::npos != m_v->find('\n')&&(  m_v->at(0) == '\n' || m_v->at(m_v->size()-1) == '\n'))
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
		EElementType CT_NumVal::getType() { return et_ct_numval; }
		
		void CT_NumVal::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"idx", m_idx)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"formatCode", m_formatCode)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		CT_NumRef::CT_NumRef()
		{
			m_numCache = NULL;
		}
		CT_NumRef::~CT_NumRef()
		{
			if (NULL != m_numCache)
				delete m_numCache;
		}
		void CT_NumRef::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"f" == sName)
				{
					m_f = oReader.GetText3();
				}
				else if (L"numCache" == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numCache = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_NumRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			if (m_f.IsInit())
			{
				if (std::wstring::npos != m_f->find(' ')&&(  m_f->at(0) == ' ' || m_f->at(m_f->size()-1) == ' ')
				 || std::wstring::npos != m_f->find('\n')&&(  m_f->at(0) == '\n' || m_f->at(m_f->size()-1) == '\n'))
					writer.WriteString(L"<c:f xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:f>");
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(L"</c:f>");
			}
			if (NULL != m_numCache)
			{
				std::wstring sNodeName = L"c:numCache";
				m_numCache->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_NumRef::getType() { return et_ct_numref; }
		
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
			if (NULL != m_multiLvlStrRef)
				delete m_multiLvlStrRef;
			if (NULL != m_numLit)
				delete m_numLit;
			if (NULL != m_numRef)
				delete m_numRef;
			if (NULL != m_strLit)
				delete m_strLit;
			if (NULL != m_strRef)
				delete m_strRef;
		}
		void CT_AxDataSource::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"multiLvlStrRef" == sName)
				{
					CT_MultiLvlStrRef* pNewElem = new CT_MultiLvlStrRef;
					pNewElem->fromXML(oReader);
					m_multiLvlStrRef = pNewElem;
				}
				else if (L"numLit" == sName)
				{
					CT_NumData* pNewElem = new CT_NumData;
					pNewElem->fromXML(oReader);
					m_numLit = pNewElem;
				}
				else if (L"numRef" == sName)
				{
					CT_NumRef* pNewElem = new CT_NumRef;
					pNewElem->fromXML(oReader);
					m_numRef = pNewElem;
				}
				else if (L"strLit" == sName)
				{
					CT_StrData* pNewElem = new CT_StrData;
					pNewElem->fromXML(oReader);
					m_strLit = pNewElem;
				}
				else if (L"strRef" == sName)
				{
					CT_StrRef* pNewElem = new CT_StrRef;
					pNewElem->fromXML(oReader);
					m_strRef = pNewElem;
				}
			}
		}
		std::wstring CT_AxDataSource::toXML() const
		{
			return L"";
		}
		void CT_AxDataSource::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML(L"c:cat", writer);
		}
		void CT_AxDataSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_multiLvlStrRef)
			{
				std::wstring sNodeName = L"c:multiLvlStrRef";
				m_multiLvlStrRef->toXML(sNodeName, writer);
			}
			if (NULL != m_numLit)
			{
				std::wstring sNodeName = L"c:numLit";
				m_numLit->toXML(sNodeName, writer);
			}
			if (NULL != m_numRef)
			{
				std::wstring sNodeName = L"c:numRef";
				m_numRef->toXML(sNodeName, writer);
			}
			if (NULL != m_strLit)
			{
				std::wstring sNodeName = L"c:strLit";
				m_strLit->toXML(sNodeName, writer);
			}
			if (NULL != m_strRef)
			{
				std::wstring sNodeName = L"c:strRef";
				m_strRef->toXML(sNodeName, writer);
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AxDataSource::getType() { return et_ct_axdatasource; }

		CT_MultiLvlStrRef::CT_MultiLvlStrRef()
		{
			m_multiLvlStrCache = NULL;
		}
		CT_MultiLvlStrRef::~CT_MultiLvlStrRef()
		{
			if (NULL != m_multiLvlStrCache)
				delete m_multiLvlStrCache;
		}
		void CT_MultiLvlStrRef::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"f" == sName)
				{
					m_f = oReader.GetText3();
				}
				else if (L"multiLvlStrCache" == sName)
				{
					CT_MultiLvlStrData* pNewElem = new CT_MultiLvlStrData;
					pNewElem->fromXML(oReader);
					m_multiLvlStrCache = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_MultiLvlStrRef::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			if (m_f.IsInit())
			{
				if (std::wstring::npos != m_f->find(' ')&&(  m_f->at(0) == ' ' || m_f->at(m_f->size()-1) == ' ')
				 			|| std::wstring::npos != m_f->find('\n')&&(  m_f->at(0) == '\n' || m_f->at(m_f->size()-1) == '\n'))
					writer.WriteString(L"<c:f xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:f>");
				writer.WriteEncodeXmlString(*m_f);
				writer.WriteString(L"</c:f>");
			}
			if (NULL != m_multiLvlStrCache)
			{
				std::wstring sNodeName = L"c:multiLvlStrCache";
				m_multiLvlStrCache->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_MultiLvlStrRef::getType() { return et_ct_multilvlstrref; }
		
		CT_lvl::CT_lvl()
		{
		}
		CT_lvl::~CT_lvl()
		{
			for (size_t i = 0; i < m_pt.size(); ++i)
				delete m_pt[i];
			m_pt.clear();
		}
		void CT_lvl::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"pt" == sName)
				{
					CT_StrVal* pNewElem = new CT_StrVal;
					pNewElem->fromXML(oReader);
					m_pt.push_back(pNewElem);
				}
			}
		}
		void CT_lvl::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			for (size_t i = 0; i < m_pt.size(); ++i)
			{
				CT_StrVal* pElem = m_pt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:pt";
					pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_lvl::getType() { return et_ct_lvl; }
		
		CT_MultiLvlStrData::CT_MultiLvlStrData()
		{
		}
		CT_MultiLvlStrData::~CT_MultiLvlStrData()
		{
			for (size_t i = 0; i < m_lvl.size(); ++i)
				delete m_lvl[i];
			m_lvl.clear();
		}
		void CT_MultiLvlStrData::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"ptCount" == sName)
				{
					m_ptCount = oReader;
				}
				else if (L"lvl" == sName)
				{
					CT_lvl* pNewElem = new CT_lvl;
					pNewElem->fromXML(oReader);
					m_lvl.push_back(pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_MultiLvlStrData::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_ptCount.toXML(L"c:ptCount", writer);

			for (size_t i = 0; i < m_lvl.size(); ++i)
			{
				CT_lvl* pElem = m_lvl[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:lvl";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_MultiLvlStrData::getType() { return et_ct_multilvlstrdata; }
		
		CT_BubbleChart::CT_BubbleChart()
		{
			m_dLbls = NULL;
		}
		CT_BubbleChart::~CT_BubbleChart()
		{
			if (NULL != m_dLbls)
				delete m_dLbls;

			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
		}
		void CT_BubbleChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_BubbleSer* pNewElem = new CT_BubbleSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					m_dLbls = new CT_DLbls;
					m_dLbls->fromXML(oReader);
				}
				else if (L"bubble3D" == sName)
				{
					m_bubble3D = oReader;
				}
				else if (L"bubbleScale" == sName)
				{
					m_bubbleScale = oReader;
				}
				else if (L"showNegBubbles" == sName)
				{
					m_showNegBubbles = oReader;
				}
				else if (L"sizeRepresents" == sName)
				{
					m_sizeRepresents = oReader;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_BubbleChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BubbleSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_bubble3D.toXML(L"c:bubble3D", writer);
			m_bubbleScale.toXML(L"c:bubbleScale", writer);
			m_showNegBubbles.toXML(L"c:showNegBubbles", writer);
			m_sizeRepresents.toXML(L"c:sizeRepresents", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BubbleChart::getType() { return et_ct_bubblechart; }
		
		CT_bandFmts::CT_bandFmts()
		{
		}
		CT_bandFmts::~CT_bandFmts()
		{
			for (size_t i = 0; i < m_bandFmt.size(); ++i)
				delete m_bandFmt[i];
			m_bandFmt.clear();
		}
		void CT_bandFmts::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"bandFmt" == sName)
				{
					CT_BandFmt* pNewElem = new CT_BandFmt;
					pNewElem->fromXML(oReader);
					m_bandFmt.push_back(pNewElem);
				}
			}
		}
		void CT_bandFmts::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			for (size_t i = 0; i < m_bandFmt.size(); ++i)
			{
				CT_BandFmt* pElem = m_bandFmt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:bandFmt";
					pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_bandFmts::getType() { return et_ct_bandfmts; }
		
		CT_Surface3DChart::CT_Surface3DChart()
		{
			m_bandFmts = NULL;
		}
		CT_Surface3DChart::~CT_Surface3DChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			if (NULL != m_bandFmts)
				delete m_bandFmts;
		}
		void CT_Surface3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"wireframe" == sName)
				{
					m_wireframe = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"bandFmts" == sName)
				{
					CT_bandFmts* pNewElem = new CT_bandFmts;
					pNewElem->fromXML(oReader);
					m_bandFmts = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Surface3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_wireframe.toXML(L"c:wireframe", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_SurfaceSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			m_bandFmts->toXML(L"c:bandFmts", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");				
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Surface3DChart::getType() { return et_ct_surface3dchart; }
		
		CT_SurfaceSer::CT_SurfaceSer()
		{
			m_cat = NULL;
			m_val = NULL;
		}
		CT_SurfaceSer::~CT_SurfaceSer()
		{
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_SurfaceSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
			}
		}
		void CT_SurfaceSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			if (NULL != m_cat)
			{
				std::wstring sNodeName = L"c:cat";
				m_cat->toXML(sNodeName, writer);
			}
			if (NULL != m_val)
			{
				std::wstring sNodeName = L"c:val";
				m_val->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SurfaceSer::getType() { return et_ct_surfaceser; }
		
		CT_BandFmt::CT_BandFmt()
		{
		}
		CT_BandFmt::~CT_BandFmt()
		{
		}
		void CT_BandFmt::fromXML(XmlUtils::CXmlLiteReader& oReader) {
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"idx" == sName)
				{
					m_idx = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
			}
		}
		void CT_BandFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_idx.toXML(L"c:idx", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BandFmt::getType() { return et_ct_bandfmt; }
		
		CT_SurfaceChart::CT_SurfaceChart()
		{
			m_bandFmts = NULL;
		}
		CT_SurfaceChart::~CT_SurfaceChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_bandFmts)
				delete m_bandFmts;
		}
		void CT_SurfaceChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"wireframe" == sName)
				{
					m_wireframe = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"bandFmts" == sName)
				{
					CT_bandFmts* pNewElem = new CT_bandFmts;
					pNewElem->fromXML(oReader);
					m_bandFmts = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_SurfaceChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_wireframe.toXML(L"c:wireframe", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_SurfaceSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_bandFmts)
			{
				m_bandFmts->toXML(L"c:bandFmts", writer);
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_SurfaceChart::getType() { return et_ct_surfacechart; }
		
		ST_SplitType CSplitType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_SplitType(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CSplitType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_SplitType(m_eValue, sEnumVal);
			return sEnumVal;
		}

		ST_OfPieType COfPieType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_OfPieType(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring COfPieType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_OfPieType(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_custSplit::CT_custSplit()
		{
		}
		CT_custSplit::~CT_custSplit()
		{
		}
		void CT_custSplit::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"secondPiePt" == sName)
				{
					nullableUintVal pNewElem = oReader;
					m_secondPiePt.push_back(*pNewElem);
				}
			}
		}
		void CT_custSplit::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			for (size_t i = 0; i < m_secondPiePt.size(); ++i)
			{
				writer.WriteString(L"<c:secondPiePt val=\"" + std::to_wstring(m_secondPiePt[i]) + L"\"/>");
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_custSplit::getType() { return et_ct_custsplit; }
		
		CT_OfPieChart::CT_OfPieChart()
		{
			m_dLbls = NULL;
			m_custSplit = NULL;
		}
		CT_OfPieChart::~CT_OfPieChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;

			if (NULL != m_custSplit)
				delete m_custSplit;

			for (size_t i = 0; i < m_serLines.size(); ++i)
				delete m_serLines[i];
			m_serLines.clear();
		}
		void CT_OfPieChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"ofPieType" == sName)
				{
					m_ofPieType = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"gapWidth" == sName)
				{
					m_gapWidth = oReader;
				}
				else if (L"splitType" == sName)
				{
					m_splitType = oReader;
				}
				else if (L"splitPos" == sName)
				{
					m_splitPos = oReader;
				}
				else if (L"custSplit" == sName)
				{
					CT_custSplit* pNewElem = new CT_custSplit;
					pNewElem->fromXML(oReader);
					m_custSplit = pNewElem;
				}
				else if (L"secondPieSize" == sName)
				{
					m_secondPieSize = oReader;
				}
				else if (L"serLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_serLines.push_back(pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_OfPieChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_ofPieType.toXML(L"c:ofPieType", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_gapWidth.toXML(L"c:gapWidth", writer);
			m_splitType.toXML(L"c:splitType", writer);
			m_splitPos.toXML(L"c:splitPos", writer);

			if (NULL != m_custSplit)
			{
				m_custSplit->toXML(L"c:custSplit", writer);
			}
			m_secondPieSize.toXML(L"c:secondPieSize", writer);

			for (size_t i = 0; i < m_serLines.size(); ++i)
			{
				CT_ChartLines* pElem = m_serLines[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:serLines", writer);
				}
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_OfPieChart::getType() { return et_ct_ofpiechart; }
		
		CT_PieSer::CT_PieSer()
		{
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
		}
		CT_PieSer::~CT_PieSer()
		{
			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_PieSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"explosion" == sName)
				{
					m_explosion = oReader;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
			}
		}
		void CT_PieSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			m_explosion.toXML(L"c:explosion", writer);

			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:dPt", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			if (NULL != m_cat)
			{
				std::wstring sNodeName = L"c:cat";
				m_cat->toXML(sNodeName, writer);
			}
			if (NULL != m_val)
			{
				std::wstring sNodeName = L"c:val";
				m_val->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PieSer::getType() { return et_ct_pieser; }
		
		CT_Bar3DChart::CT_Bar3DChart()
		{
			m_dLbls = NULL;
		}
		CT_Bar3DChart::~CT_Bar3DChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_Bar3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"barDir" == sName)
				{
					m_barDir = oReader;
				}
				else if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_BarSer* pNewElem = new CT_BarSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"gapWidth" == sName)
				{
					m_gapWidth = oReader;
				}
				else if (L"gapDepth" == sName)
				{
					m_gapDepth = oReader;
				}
				else if (L"shape" == sName)
				{
					m_shape = oReader;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Bar3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_barDir.toXML(L"c:barDir", writer);
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BarSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_gapWidth.toXML(L"c:gapWidth", writer);
			m_gapDepth.toXML(L"c:gapDepth", writer);
			m_shape.toXML(L"c:shape", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Bar3DChart::getType() { return et_ct_bar3dchart; }
		
		ST_BarDir CBarDir::FromString(const std::wstring &sValue)
		{
			FromXml_ST_BarDir(sValue, this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CBarDir::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_BarDir(m_eValue, sEnumVal);
			return sEnumVal;
		}

		ST_BarGrouping CBarGrouping::FromString(const std::wstring &sValue)
		{
			FromXml_ST_BarGrouping(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CBarGrouping::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_BarGrouping(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_BarSer::CT_BarSer()
		{
			m_pictureOptions = NULL;
			m_dLbls = NULL;
			m_errBars = NULL;
			m_cat = NULL;
			m_val = NULL;
		}
		CT_BarSer::~CT_BarSer()
		{
			if (NULL != m_pictureOptions)
				delete m_pictureOptions;
			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			for (size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
			m_trendline.clear();
			if (NULL != m_errBars)
				delete m_errBars;
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_BarSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"invertIfNegative" == sName)
				{
					m_invertIfNegative = oReader;
				}
				else if (L"pictureOptions" == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"trendline" == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
					m_trendline.push_back(pNewElem);
				}
				else if (L"errBars" == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars = pNewElem;
				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if (L"shape" == sName)
				{
					m_shape = oReader;
				}
			}
		}
		void CT_BarSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			m_invertIfNegative.toXML(L"c:invertIfNegative", writer);

			if (NULL != m_pictureOptions)
			{
				m_pictureOptions->toXML(L"c:pictureOptions", writer);
			}
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:dPt", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			for (size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:trendline", writer);
				}
			}
			if (NULL != m_errBars)
			{
				m_errBars->toXML(L"c:errBars", writer);
			}
			if (NULL != m_cat)
			{
				m_cat->toXML(L"c:cat", writer);
			}
			if (NULL != m_val)
			{
				m_val->toXML(L"c:val", writer);
			}
			m_shape.toXML(L"c:shape", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		XLS::BaseObjectPtr CT_BarSer::GetXLSFormat() const
		{
			auto seriesFormat = new XLS::SERIESFORMAT;
			{
				auto ai1 = new XLS::BRAI;
				seriesFormat->m_arAI.push_back(XLS::BaseObjectPtr(ai1));
			}
			auto series = new XLS::Series;
			series->sdtX = 1;
			if(m_val != nullptr && m_val->m_numRef != nullptr)
			{
				if(m_val->m_numRef->m_numCache != nullptr)
				{
					series->cValx = m_val->m_numRef->m_numCache->m_pt.size();
					series->cValy = m_val->m_numRef->m_numCache->m_pt.size();
				}
				{
					auto ai2 = new XLS::BRAI;
					ai2->id = 1;
					if(m_val->m_numRef->m_f.IsInit())
					{
						ai2->rt = 2;
						ai2->formula.parseStringFormula(m_val->m_numRef->m_f.get(), L"");
					}
					seriesFormat->m_arAI.push_back(XLS::BaseObjectPtr(ai2));
					auto ai3 = new XLS::BRAI;
					ai3->id = 2;
					auto ai4 = new XLS::BRAI;
					ai4->id = 3;
					seriesFormat->m_arAI.push_back(XLS::BaseObjectPtr(ai3));
					seriesFormat->m_arAI.push_back(XLS::BaseObjectPtr(ai4));
				}
			}
			seriesFormat->m_Series = XLS::BaseObjectPtr(series);

			return XLS::BaseObjectPtr(seriesFormat);
		}
		EElementType CT_BarSer::getType() { return et_ct_barser; }
		
		ST_Shape CShapeType::FromString(const std::wstring &sValue)
		{
			FromXml_ST_Shape(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CShapeType::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_Shape(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_BarChart::CT_BarChart()
		{
			m_dLbls = NULL;
		}
		CT_BarChart::~CT_BarChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;

			for (size_t i = 0; i < m_serLines.size(); ++i)
				delete m_serLines[i];
			m_serLines.clear();
		}
		void CT_BarChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"barDir" == sName)
				{
					m_barDir = oReader;
				}
				else if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_BarSer* pNewElem = new CT_BarSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"gapWidth" == sName)
				{
					m_gapWidth = oReader;
				}
				else if (L"overlap" == sName)
				{
					m_overlap = oReader;
				}
				else if (L"serLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_serLines.push_back(pNewElem);
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_BarChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_barDir.toXML(L"c:barDir", writer);
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_BarSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_gapWidth.toXML(L"c:gapWidth", writer);
			m_overlap.toXML(L"c:overlap", writer);

			for (size_t i = 0; i < m_serLines.size(); ++i)
			{
				CT_ChartLines* pElem = m_serLines[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:serLines", writer);
				}
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_BarChart::getType() { return et_ct_barchart; }
		
		CT_DoughnutChart::CT_DoughnutChart()
		{
			m_dLbls = NULL;
		}
		CT_DoughnutChart::~CT_DoughnutChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_DoughnutChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"firstSliceAng" == sName)
				{
					m_firstSliceAng = oReader;
				}
				else if (L"holeSize" == sName)
				{
					m_holeSize = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_DoughnutChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_firstSliceAng.toXML(L"c:firstSliceAng", writer);
			m_holeSize.toXML(L"c:holeSize", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_DoughnutChart::getType() { return et_ct_doughnutchart; }

		CT_Pie3DChart::CT_Pie3DChart()
		{
			m_dLbls = NULL;
		}
		CT_Pie3DChart::~CT_Pie3DChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_Pie3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Pie3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Pie3DChart::getType() { return et_ct_pie3dchart; }
		
		CT_PieChart::CT_PieChart()
		{
			m_dLbls = NULL;
		}
		CT_PieChart::~CT_PieChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_PieChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_PieSer* pNewElem = new CT_PieSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"firstSliceAng" == sName)
				{
					m_firstSliceAng = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_PieChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_PieSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			m_firstSliceAng.toXML(L"c:firstSliceAng", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PieChart::getType() { return et_ct_piechart; }
		
		CT_ScatterSer::CT_ScatterSer()
		{
			m_marker = NULL;
			m_dLbls = NULL;
			m_xVal = NULL;
			m_yVal = NULL;
		}
		CT_ScatterSer::~CT_ScatterSer()
		{
			if (NULL != m_marker)
				delete m_marker;

			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
			for (size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
			m_trendline.clear();
			for (size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
			m_errBars.clear();
			if (NULL != m_xVal)
				delete m_xVal;
			if (NULL != m_yVal)
				delete m_yVal;
		}
		void CT_ScatterSer::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"marker" == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"trendline" == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
					m_trendline.push_back(pNewElem);
				}
				else if (L"errBars" == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars.push_back(pNewElem);
				}
				else if (L"xVal" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_xVal = pNewElem;
				}
				else if (L"yVal" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_yVal = pNewElem;
				}
				else if (L"smooth" == sName)
				{
					m_smooth = oReader;
				}
			}
		}
		void CT_ScatterSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			if (NULL != m_marker)
			{
				std::wstring sNodeName = L"c:marker";
				m_marker->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dPt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:trendline";
					pElem->toXML(sNodeName, writer);
				}
			}
			for (size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:errBars";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_xVal)
			{
				std::wstring sNodeName = L"c:xVal";
				m_xVal->toXML(sNodeName, writer);
			}
			if (NULL != m_yVal)
			{
				std::wstring sNodeName = L"c:yVal";
				m_yVal->toXML(sNodeName, writer);
			}
			m_smooth.toXML(L"c:smooth", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ScatterSer::getType() { return et_ct_scatterser; }
		
		ST_ScatterStyle CScatterStyle::FromString(const std::wstring &sValue)
		{
			FromXml_ST_ScatterStyle(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CScatterStyle::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_ScatterStyle(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_ScatterChart::CT_ScatterChart()
		{
			m_dLbls = NULL;
		}
		CT_ScatterChart::~CT_ScatterChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_ScatterChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"scatterStyle" == sName)
				{
					m_scatterStyle = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_ScatterSer* pNewElem = new CT_ScatterSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_ScatterChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_scatterStyle.toXML(L"c:scatterStyle", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_ScatterSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_ScatterChart::getType() { return et_ct_scatterchart; }
		CT_RadarSer::CT_RadarSer()
		{
			m_marker = NULL;
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
		}
		CT_RadarSer::~CT_RadarSer()
		{
			if (NULL != m_marker)
				delete m_marker;

			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_RadarSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"marker" == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
			}
		}
		void CT_RadarSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			if (NULL != m_marker)
			{
				std::wstring sNodeName = L"c:marker";
				m_marker->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dPt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			if (NULL != m_cat)
			{
				std::wstring sNodeName = L"c:cat";
				m_cat->toXML(sNodeName, writer);
			}
			if (NULL != m_val)
			{
				std::wstring sNodeName = L"c:val";
				m_val->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_RadarSer::getType() { return et_ct_radarser; }
		
		ST_RadarStyle CRadarStyle::FromString(const std::wstring &sValue)
		{
			FromXml_ST_RadarStyle(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CRadarStyle::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_RadarStyle(m_eValue, sEnumVal);
			return sEnumVal;
		}

		CT_RadarChart::CT_RadarChart()
		{
			m_dLbls = NULL;
		}
		CT_RadarChart::~CT_RadarChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;
		}
		void CT_RadarChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"radarStyle" == sName)
				{
					m_radarStyle = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_RadarSer* pNewElem = new CT_RadarSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal val = oReader;
					m_axId.push_back(*val);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_RadarChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_radarStyle.toXML(L"c:radarStyle", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_RadarSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_RadarChart::getType() { return et_ct_radarchart; }
		CT_StockChart::CT_StockChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_hiLowLines = NULL;
			m_upDownBars = NULL;
		}
		CT_StockChart::~CT_StockChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_dropLines)
				delete m_dropLines;
			if (NULL != m_hiLowLines)
				delete m_hiLowLines;
			if (NULL != m_upDownBars)
				delete m_upDownBars;
		}
		void CT_StockChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"ser" == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"dropLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if (L"hiLowLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_hiLowLines = pNewElem;
				}
				else if (L"upDownBars" == sName)
				{
					CT_UpDownBars* pNewElem = new CT_UpDownBars;
					pNewElem->fromXML(oReader);
					m_upDownBars = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_StockChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:ser";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			if (NULL != m_dropLines)
			{
				std::wstring sNodeName = L"c:dropLines";
				m_dropLines->toXML(sNodeName, writer);
			}
			if (NULL != m_hiLowLines)
			{
				std::wstring sNodeName = L"c:hiLowLines";
				m_hiLowLines->toXML(sNodeName, writer);
			}
			if (NULL != m_upDownBars)
			{
				std::wstring sNodeName = L"c:upDownBars";
				m_upDownBars->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_StockChart::getType() { return et_ct_stockchart; }
		CT_LineSer::CT_LineSer()
		{
			m_marker = NULL;
			m_dLbls = NULL;
			m_errBars = NULL;
			m_cat = NULL;
			m_val = NULL;
		}
		CT_LineSer::~CT_LineSer()
		{
			if (NULL != m_marker)
				delete m_marker;
			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			for (size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
			m_trendline.clear();
			if (NULL != m_errBars)
				delete m_errBars;
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_LineSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"marker" == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"trendline" == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
					m_trendline.push_back(pNewElem);
				}
				else if (L"errBars" == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars = pNewElem;
				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if (L"smooth" == sName)
				{
					m_smooth = oReader;
				}
			}
		}
		void CT_LineSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			if (NULL != m_marker)
			{
				std::wstring sNodeName = L"c:marker";
				m_marker->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dPt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:trendline";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_errBars)
			{
				std::wstring sNodeName = L"c:errBars";
				m_errBars->toXML(sNodeName, writer);
			}
			if (NULL != m_cat)
			{
				std::wstring sNodeName = L"c:cat";
				m_cat->toXML(sNodeName, writer);
			}
			if (NULL != m_val)
			{
				std::wstring sNodeName = L"c:val";
				m_val->toXML(sNodeName, writer);
			}
			
			m_smooth.toXML(L"c:smooth", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LineSer::getType() { return et_ct_lineser; }
		CT_UpDownBars::CT_UpDownBars()
		{
			m_upBars = NULL;
			m_downBars = NULL;
		}
		CT_UpDownBars::~CT_UpDownBars()
		{
			if (NULL != m_upBars)
				delete m_upBars;
			if (NULL != m_downBars)
				delete m_downBars;
		}
		void CT_UpDownBars::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"gapWidth" == sName)
				{
					m_gapWidth = oReader;
				}
				else if (L"upBars" == sName)
				{
					CT_UpDownBar* pNewElem = new CT_UpDownBar;
					pNewElem->fromXML(oReader);
					m_upBars = pNewElem;
				}
				else if (L"downBars" == sName)
				{
					CT_UpDownBar* pNewElem = new CT_UpDownBar;
					pNewElem->fromXML(oReader);
					m_downBars = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_UpDownBars::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_gapWidth.toXML(L"c:gapWidth", writer);

			if (NULL != m_upBars)
			{
				std::wstring sNodeName = L"c:upBars";
				m_upBars->toXML(sNodeName, writer);
			}
			if (NULL != m_downBars)
			{
				std::wstring sNodeName = L"c:downBars";
				m_downBars->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_UpDownBars::getType() { return et_ct_updownbars; }
		CT_UpDownBar::CT_UpDownBar()
		{
		}
		CT_UpDownBar::~CT_UpDownBar()
		{
		}
		void CT_UpDownBar::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
			}
		}
		void CT_UpDownBar::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_UpDownBar::getType() { return et_ct_updownbar; }
		
		CT_Line3DChart::CT_Line3DChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
		}
		CT_Line3DChart::~CT_Line3DChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			
			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_dropLines)
				delete m_dropLines;
		}
		void CT_Line3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"dropLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if (L"gapDepth" == sName)
				{
					m_gapDepth = oReader;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Line3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			if (NULL != m_dropLines)
			{
				m_dropLines->toXML(L"c:dropLines", writer);
			}
			m_gapDepth.toXML(L"c:gapDepth", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Line3DChart::getType() { return et_ct_line3dchart; }
		
		ST_Grouping CGrouping::FromString(const std::wstring &sValue)
		{
			FromXml_ST_Grouping(sValue,  this->m_eValue);
			return this->m_eValue;
		}
		std::wstring CGrouping::ToString() const
		{
			std::wstring sEnumVal;
			ToXml_ST_Grouping(m_eValue, sEnumVal);
			return sEnumVal;
		}
		CT_LineChart::CT_LineChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
			m_hiLowLines = NULL;
			m_upDownBars = NULL;
		}
		CT_LineChart::~CT_LineChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();

			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_dropLines)
				delete m_dropLines;
			if (NULL != m_hiLowLines)
				delete m_hiLowLines;
			if (NULL != m_upDownBars)
				delete m_upDownBars;
		}
		void CT_LineChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_LineSer* pNewElem = new CT_LineSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"dropLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if (L"hiLowLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_hiLowLines = pNewElem;
				}
				else if (L"upDownBars" == sName)
				{
					CT_UpDownBars* pNewElem = new CT_UpDownBars;
					pNewElem->fromXML(oReader);
					m_upDownBars = pNewElem;
				}
				else if (L"marker" == sName)
				{
					m_marker = oReader;
				}
				else if (L"smooth" == sName)
				{
					m_smooth = oReader;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_LineChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_LineSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			if (NULL != m_dropLines)
			{
				m_dropLines->toXML(L"c:dropLines", writer);
			}
			if (NULL != m_hiLowLines)
			{
				m_hiLowLines->toXML(L"c:hiLowLines", writer);
			}
			if (NULL != m_upDownBars)
			{
				m_upDownBars->toXML(L"c:upDownBars", writer);
			}
			m_marker.toXML(L"c:marker", writer);
			m_smooth.toXML(L"c:smooth", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_LineChart::getType() { return et_ct_linechart; }
		
		CT_Area3DChart::CT_Area3DChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
		}
		CT_Area3DChart::~CT_Area3DChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_dropLines)
				delete m_dropLines;
		}
		void CT_Area3DChart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_AreaSer* pNewElem = new CT_AreaSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"dropLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if (L"gapDepth" == sName)
				{
					m_gapDepth = oReader;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Area3DChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");			
			
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_AreaSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			if (NULL != m_dropLines)
			{
				m_dropLines->toXML(L"c:dropLines", writer);
			}
			m_gapDepth.toXML(L"c:gapDepth", writer);

			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Area3DChart::getType() { return et_ct_area3dchart; }
		CT_AreaSer::CT_AreaSer()
		{
			m_pictureOptions = NULL;
			m_dLbls = NULL;
			m_cat = NULL;
			m_val = NULL;
		}
		CT_AreaSer::~CT_AreaSer()
		{
			if (NULL != m_pictureOptions)
				delete m_pictureOptions;
			for (size_t i = 0; i < m_dPt.size(); ++i)
				delete m_dPt[i];
			m_dPt.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			for (size_t i = 0; i < m_trendline.size(); ++i)
				delete m_trendline[i];
			m_trendline.clear();
			for (size_t i = 0; i < m_errBars.size(); ++i)
				delete m_errBars[i];
			m_errBars.clear();
			if (NULL != m_cat)
				delete m_cat;
			if (NULL != m_val)
				delete m_val;
		}
		void CT_AreaSer::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				
				if (CBaseSer::fromXML(sName, oReader))
				{

				}
				else if (L"pictureOptions" == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if (L"dPt" == sName)
				{
					CT_DPt* pNewElem = new CT_DPt;
					pNewElem->fromXML(oReader);
					m_dPt.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"trendline" == sName)
				{
					CT_Trendline* pNewElem = new CT_Trendline;
					pNewElem->fromXML(oReader);
					m_trendline.push_back(pNewElem);
				}
				else if (L"errBars" == sName)
				{
					CT_ErrBars* pNewElem = new CT_ErrBars;
					pNewElem->fromXML(oReader);
					m_errBars.push_back(pNewElem);
				}
				else if (L"cat" == sName)
				{
					CT_AxDataSource* pNewElem = new CT_AxDataSource;
					pNewElem->fromXML(oReader);
					m_cat = pNewElem;
				}
				else if (L"val" == sName)
				{
					CT_NumDataSource* pNewElem = new CT_NumDataSource;
					pNewElem->fromXML(oReader);
					m_val = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_AreaSer::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			CBaseSer::toXML(writer);

			if (NULL != m_pictureOptions)
			{
				std::wstring sNodeName = L"c:pictureOptions";
				m_pictureOptions->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_dPt.size(); ++i)
			{
				CT_DPt* pElem = m_dPt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:dPt";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_dLbls)
			{
				std::wstring sNodeName = L"c:dLbls";
				m_dLbls->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_trendline.size(); ++i)
			{
				CT_Trendline* pElem = m_trendline[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:trendline";
					pElem->toXML(sNodeName, writer);
				}
			}
			for (size_t i = 0; i < m_errBars.size(); ++i)
			{
				CT_ErrBars* pElem = m_errBars[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:errBars";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_cat)
			{
				std::wstring sNodeName = L"c:cat";
				m_cat->toXML(sNodeName, writer);
			}
			if (NULL != m_val)
			{
				std::wstring sNodeName = L"c:val";
				m_val->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AreaSer::getType() { return et_ct_areaser; }
		
		CT_AreaChart::CT_AreaChart()
		{
			m_dLbls = NULL;
			m_dropLines = NULL;
		}
		CT_AreaChart::~CT_AreaChart()
		{
			for (size_t i = 0; i < m_ser.size(); ++i)
				delete m_ser[i];
			m_ser.clear();
			if (NULL != m_dLbls)
				delete m_dLbls;
			if (NULL != m_dropLines)
				delete m_dropLines;
		}
		void CT_AreaChart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"grouping" == sName)
				{
					m_grouping = oReader;
				}
				else if (L"varyColors" == sName)
				{
					m_varyColors = oReader;
				}
				else if (L"ser" == sName)
				{
					CT_AreaSer* pNewElem = new CT_AreaSer;
					pNewElem->fromXML(oReader);
					m_ser.push_back(pNewElem);
				}
				else if (L"dLbls" == sName)
				{
					CT_DLbls* pNewElem = new CT_DLbls;
					pNewElem->fromXML(oReader);
					m_dLbls = pNewElem;
				}
				else if (L"dropLines" == sName)
				{
					CT_ChartLines* pNewElem = new CT_ChartLines;
					pNewElem->fromXML(oReader);
					m_dropLines = pNewElem;
				}
				else if (L"axId" == sName)
				{
					nullableIntVal pNewElem = oReader;
					m_axId.push_back(*pNewElem);
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_AreaChart::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_grouping.toXML(L"c:grouping", writer);
			m_varyColors.toXML(L"c:varyColors", writer);

			for (size_t i = 0; i < m_ser.size(); ++i)
			{
				CT_AreaSer* pElem = m_ser[i];
				if (NULL != pElem)
				{
					pElem->toXML(L"c:ser", writer);
				}
			}
			if (NULL != m_dLbls)
			{
				m_dLbls->toXML(L"c:dLbls", writer);
			}
			if (NULL != m_dropLines)
			{
				m_dropLines->toXML(L"c:dropLines", writer);
			}
			for (size_t i = 0; i < m_axId.size(); ++i)
			{
				writer.WriteString(L"<c:axId  val=\"" + std::to_wstring(m_axId[i]) + L"\"/>");
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_AreaChart::getType() { return et_ct_areachart; }
		CT_PlotArea::CT_PlotArea()
		{
			m_layout = NULL;
			m_dTable = NULL;
		}
		CT_PlotArea::~CT_PlotArea()
		{
			if (NULL != m_layout)
				delete m_layout;
			for (size_t i = 0; i < m_Items.size() && i < m_ItemsElementName0.size(); ++i)
				toDelete(*m_ItemsElementName0[i], m_Items[i]);
			m_Items.clear();
			for (size_t i = 0; i < m_ItemsElementName0.size(); ++i)
				delete m_ItemsElementName0[i];
			m_ItemsElementName0.clear();
			for (size_t i = 0; i < m_Items1.size() && i < m_ItemsElementName1.size(); ++i)
				toDelete(*m_ItemsElementName1[i], m_Items1[i]);
			m_Items1.clear();
			for (size_t i = 0; i < m_ItemsElementName1.size(); ++i)
				delete m_ItemsElementName1[i];
			m_ItemsElementName1.clear();
			if (NULL != m_dTable)
				delete m_dTable;
		}
		void CT_PlotArea::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"layout" == sName)
				{
					CT_Layout* pNewElem = new CT_Layout;
					pNewElem->fromXML(oReader);
					m_layout = pNewElem;
				}
				else if (L"area3DChart" == sName)
				{
					CT_Area3DChart* pNewElem = new CT_Area3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5AREA3DCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"areaChart" == sName)
				{
					CT_AreaChart* pNewElem = new CT_AreaChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5AREACHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"bar3DChart" == sName)
				{
					CT_Bar3DChart* pNewElem = new CT_Bar3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BAR3DCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"barChart" == sName)
				{
					CT_BarChart* pNewElem = new CT_BarChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BARCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"bubbleChart" == sName)
				{
					CT_BubbleChart* pNewElem = new CT_BubbleChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5BUBBLECHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"doughnutChart" == sName)
				{
					CT_DoughnutChart* pNewElem = new CT_DoughnutChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5DOUGHNUTCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"line3DChart" == sName)
				{
					CT_Line3DChart* pNewElem = new CT_Line3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5LINE3DCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"lineChart" == sName)
				{
					CT_LineChart* pNewElem = new CT_LineChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5LINECHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"ofPieChart" == sName)
				{
					CT_OfPieChart* pNewElem = new CT_OfPieChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5OFPIECHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"pie3DChart" == sName)
				{
					CT_Pie3DChart* pNewElem = new CT_Pie3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5PIE3DCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"pieChart" == sName)
				{
					CT_PieChart* pNewElem = new CT_PieChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5PIECHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"radarChart" == sName)
				{
					CT_RadarChart* pNewElem = new CT_RadarChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5RADARCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"scatterChart" == sName)
				{
					CT_ScatterChart* pNewElem = new CT_ScatterChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SCATTERCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"stockChart" == sName)
				{
					CT_StockChart* pNewElem = new CT_StockChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5STOCKCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"surface3DChart" == sName)
				{
					CT_Surface3DChart* pNewElem = new CT_Surface3DChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SURFACE3DCHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"surfaceChart" == sName)
				{
					CT_SurfaceChart* pNewElem = new CT_SurfaceChart;
					pNewElem->fromXML(oReader);
					ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
					*eElemtype = itemschoicetype5SURFACECHART;
					m_Items.push_back(pNewElem);
					m_ItemsElementName0.push_back(eElemtype);
				}
				else if (L"catAx" == sName)
				{
					CT_CatAx* pNewElem = new CT_CatAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6CATAX;
					m_Items1.push_back(pNewElem);
					m_ItemsElementName1.push_back(eElemtype);
				}
				else if (L"dateAx" == sName)
				{
					CT_DateAx* pNewElem = new CT_DateAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6DATEAX;
					m_Items1.push_back(pNewElem);
					m_ItemsElementName1.push_back(eElemtype);
				}
				else if (L"serAx" == sName)
				{
					CT_SerAx* pNewElem = new CT_SerAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6SERAX;
					m_Items1.push_back(pNewElem);
					m_ItemsElementName1.push_back(eElemtype);
				}
				else if (L"valAx" == sName)
				{
					CT_ValAx* pNewElem = new CT_ValAx;
					pNewElem->fromXML(oReader);
					ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
					*eElemtype = itemschoicetype6VALAX;
					m_Items1.push_back(pNewElem);
					m_ItemsElementName1.push_back(eElemtype);
				}
				else if (L"dTable" == sName)
				{
					CT_DTable* pNewElem = new CT_DTable;
					pNewElem->fromXML(oReader);
					m_dTable = pNewElem;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_PlotArea::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const {
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			if (NULL != m_layout)
			{
				std::wstring sNodeName = L"c:layout";
				m_layout->toXML(sNodeName, writer);
			}
			for (size_t i = 0; i < m_Items.size(); ++i)
			{
				ItemsChoiceType5 eType = *m_ItemsElementName0[i];
				toXML(writer, false, eType, m_Items[i]);
			}
			for (size_t i = 0; i < m_Items1.size(); ++i)
			{
				ItemsChoiceType6 eType = *m_ItemsElementName1[i];
				toXML(writer, false, eType, m_Items1[i]);
			}
			if (NULL != m_dTable)
			{
				std::wstring sNodeName = L"c:dTable";
				m_dTable->toXML(sNodeName, writer);
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PlotArea::getType() { return et_ct_PlotArea; }
		void CT_PlotArea::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType5 eType, void* pVal) const {
			switch (eType)
			{
			case itemschoicetype5AREA3DCHART:
			{
				CT_Area3DChart* pTypeVal = static_cast<CT_Area3DChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:area3DChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5AREACHART:
			{
				CT_AreaChart* pTypeVal = static_cast<CT_AreaChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:areaChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5BAR3DCHART:
			{
				CT_Bar3DChart* pTypeVal = static_cast<CT_Bar3DChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:bar3DChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5BARCHART:
			{
				CT_BarChart* pTypeVal = static_cast<CT_BarChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:barChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5BUBBLECHART:
			{
				CT_BubbleChart* pTypeVal = static_cast<CT_BubbleChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:bubbleChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5DOUGHNUTCHART:
			{
				CT_DoughnutChart* pTypeVal = static_cast<CT_DoughnutChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:doughnutChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5LINE3DCHART:
			{
				CT_Line3DChart* pTypeVal = static_cast<CT_Line3DChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:line3DChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5LINECHART:
			{
				CT_LineChart* pTypeVal = static_cast<CT_LineChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:lineChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5OFPIECHART:
			{
				CT_OfPieChart* pTypeVal = static_cast<CT_OfPieChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:ofPieChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5PIE3DCHART:
			{
				CT_Pie3DChart* pTypeVal = static_cast<CT_Pie3DChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:pie3DChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5PIECHART:
			{
				CT_PieChart* pTypeVal = static_cast<CT_PieChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:pieChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5RADARCHART:
			{
				CT_RadarChart* pTypeVal = static_cast<CT_RadarChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:radarChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5SCATTERCHART:
			{
				CT_ScatterChart* pTypeVal = static_cast<CT_ScatterChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:scatterChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5STOCKCHART:
			{
				CT_StockChart* pTypeVal = static_cast<CT_StockChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:stockChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5SURFACE3DCHART:
			{
				CT_Surface3DChart* pTypeVal = static_cast<CT_Surface3DChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:surface3DChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype5SURFACECHART:
			{
				CT_SurfaceChart* pTypeVal = static_cast<CT_SurfaceChart*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:surfaceChart";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			}
		}
		void CT_PlotArea::toDelete(ItemsChoiceType5 eType, void* pVal) {
			switch (eType)
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
		void CT_PlotArea::toXML(NSStringUtils::CStringBuilder& writer, bool bIsAttribute, ItemsChoiceType6 eType, void* pVal) const {
			switch (eType)
			{
			case itemschoicetype6CATAX:
			{
				CT_CatAx* pTypeVal = static_cast<CT_CatAx*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:catAx";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype6DATEAX:
			{
				CT_DateAx* pTypeVal = static_cast<CT_DateAx*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:dateAx";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype6SERAX:
			{
				CT_SerAx* pTypeVal = static_cast<CT_SerAx*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:serAx";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			case itemschoicetype6VALAX:
			{
				CT_ValAx* pTypeVal = static_cast<CT_ValAx*>(pVal);
				if (NULL != pTypeVal)
				{
					if (false == bIsAttribute)
					{
						std::wstring sNodeName = L"c:valAx";
						pTypeVal->toXML(sNodeName, writer);
					}
				}
			}
			break;
			}
		}
		void CT_PlotArea::toDelete(ItemsChoiceType6 eType, void* pVal) {
			switch (eType)
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

		CT_Surface::CT_Surface()
		{
			m_pictureOptions = NULL;
		}
		CT_Surface::~CT_Surface()
		{
			if (NULL != m_pictureOptions)
				delete m_pictureOptions;
		}
		void CT_Surface::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"thickness" == sName)
				{
					m_thickness = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"pictureOptions" == sName)
				{
					CT_PictureOptions* pNewElem = new CT_PictureOptions;
					pNewElem->fromXML(oReader);
					m_pictureOptions = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Surface::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_thickness.toXML(L"c:thickness", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (NULL != m_pictureOptions)
			{
				std::wstring sNodeName = L"c:pictureOptions";
				m_pictureOptions->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Surface::getType() { return et_ct_surface; }
		
		CT_View3D::CT_View3D()
		{
		}
		CT_View3D::~CT_View3D()
		{
		}
		void CT_View3D::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"rotX" == sName)
				{
					m_rotX = oReader;
				}
				else if (L"hPercent" == sName)
				{
					m_hPercent = oReader;
				}
				else if (L"rotY" == sName)
				{
					m_rotY = oReader;
				}
				else if (L"depthPercent" == sName)
				{
					m_depthPercent = oReader;
				}
				else if (L"rAngAx" == sName)
				{
					m_rAngAx = oReader;
				}
				else if (L"perspective" == sName)
				{
					m_perspective = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_View3D::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_rotX.toXML(L"c:rotX", writer);
			m_hPercent.toXML(L"c:hPercent", writer);
			m_rotY.toXML(L"c:rotY", writer);
			m_depthPercent.toXML(L"c:depthPercent", writer);
			m_rAngAx.toXML(L"c:rAngAx", writer);
			m_perspective.toXML(L"c:perspective", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_View3D::getType() { return et_ct_view3d; }
		
		CT_PivotFmt::CT_PivotFmt()
		{
			m_marker = NULL;
			m_dLbl = NULL;
		}
		CT_PivotFmt::~CT_PivotFmt()
		{
			if (NULL != m_marker)
				delete m_marker;
			if (NULL != m_dLbl)
				delete m_dLbl;
		}
		void CT_PivotFmt::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"idx" == sName)
				{
					m_idx = oReader;
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"txPr" == sName)
				{
					m_txPr = oReader;
				}
				else if (L"marker" == sName)
				{
					CT_Marker* pNewElem = new CT_Marker;
					pNewElem->fromXML(oReader);
					m_marker = pNewElem;
				}
				else if (L"dLbl" == sName)
				{
					CT_DLbl* pNewElem = new CT_DLbl;
					pNewElem->fromXML(oReader);
					m_dLbl = pNewElem;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_PivotFmt::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_idx.toXML(L"c:idx", writer);

			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			if (m_txPr.IsInit())
			{
				m_txPr->m_name = L"c:txPr";
				writer.WriteString(m_txPr->toXML());
			}
			if (NULL != m_marker)
			{
				std::wstring sNodeName = L"c:marker";
				m_marker->toXML(sNodeName, writer);
			}
			if (NULL != m_dLbl)
			{
				std::wstring sNodeName = L"c:dLbl";
				m_dLbl->toXML(sNodeName, writer);
			}
			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PivotFmt::getType() { return et_ct_pivotfmt; }
		
		CT_pivotFmts::CT_pivotFmts()
		{
		}
		CT_pivotFmts::~CT_pivotFmts()
		{
			for (size_t i = 0; i < m_pivotFmt.size(); ++i)
				delete m_pivotFmt[i];
			m_pivotFmt.clear();
		}
		void CT_pivotFmts::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"pivotFmt" == sName)
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
			for (size_t i = 0; i < m_pivotFmt.size(); ++i)
			{
				CT_PivotFmt* pElem = m_pivotFmt[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"c:pivotFmt";
					pElem->toXML(sNodeName, writer);
				}
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_pivotFmts::getType() { return et_ct_pivotfmts; }
		
		CT_Chart::CT_Chart()
		{
			m_title = NULL;
			m_pivotFmts = NULL;
			m_view3D = NULL;
			m_floor = NULL;
			m_sideWall = NULL;
			m_backWall = NULL;
			m_plotArea = NULL;
			m_legend = NULL;
		}
		CT_Chart::~CT_Chart()
		{
			if (NULL != m_title)
				delete m_title;
			if (NULL != m_pivotFmts)
				delete m_pivotFmts;
			if (NULL != m_view3D)
				delete m_view3D;
			if (NULL != m_floor)
				delete m_floor;
			if (NULL != m_sideWall)
				delete m_sideWall;
			if (NULL != m_backWall)
				delete m_backWall;
			if (NULL != m_plotArea)
				delete m_plotArea;
			if (NULL != m_legend)
				delete m_legend;
		}
		void CT_Chart::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"title" == sName)
				{
					CT_Title* pNewElem = new CT_Title;
					pNewElem->fromXML(oReader);
					m_title = pNewElem;
				}
				else if (L"autoTitleDeleted" == sName)
				{
					m_autoTitleDeleted = oReader;
				}
				else if (L"pivotFmts" == sName)
				{
					CT_pivotFmts* pNewElem = new CT_pivotFmts;
					pNewElem->fromXML(oReader);
					m_pivotFmts = pNewElem;
				}
				else if (L"view3D" == sName)
				{
					CT_View3D* pNewElem = new CT_View3D;
					pNewElem->fromXML(oReader);
					m_view3D = pNewElem;
				}
				else if (L"floor" == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_floor = pNewElem;
				}
				else if (L"sideWall" == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_sideWall = pNewElem;
				}
				else if (L"backWall" == sName)
				{
					CT_Surface* pNewElem = new CT_Surface;
					pNewElem->fromXML(oReader);
					m_backWall = pNewElem;
				}
				else if (L"plotArea" == sName)
				{
					CT_PlotArea* pNewElem = new CT_PlotArea;
					pNewElem->fromXML(oReader);
					m_plotArea = pNewElem;
				}
				else if (L"legend" == sName)
				{
					CT_Legend* pNewElem = new CT_Legend;
					pNewElem->fromXML(oReader);
					m_legend = pNewElem;
				}
				else if (L"plotVisOnly" == sName)
				{
					m_plotVisOnly = oReader;
				}
				else if (L"dispBlanksAs" == sName)
				{
					m_dispBlanksAs = oReader;
				}
				else if (L"showDLblsOverMax" == sName)
				{
					m_showDLblsOverMax = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_Chart::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<c:chart>");

			if (NULL != m_title)
			{
				m_title->toXML(L"c:title", writer);
			}
			m_autoTitleDeleted.toXML(L"c:autoTitleDeleted", writer);

			if (NULL != m_pivotFmts)
			{
				m_pivotFmts->toXML(L"c:pivotFmts", writer);
			}
			if (NULL != m_view3D)
			{
				m_view3D->toXML(L"c:view3D", writer);
			}
			if (NULL != m_floor)
			{
				m_floor->toXML(L"c:floor", writer);
			}
			if (NULL != m_sideWall)
			{
				m_sideWall->toXML(L"c:sideWall", writer);
			}
			if (NULL != m_backWall)
			{
				m_backWall->toXML(L"c:backWall", writer);
			}
			if (NULL != m_plotArea)
			{
				m_plotArea->toXML(L"c:plotArea", writer);
			}
			if (NULL != m_legend)
			{
				m_legend->toXML(L"c:legend", writer);
			}
			m_plotVisOnly.toXML(L"c:plotVisOnly", writer);
			m_dispBlanksAs.toXML(L"c:dispBlanksAs", writer);
			m_showDLblsOverMax.toXML(L"c:showDLblsOverMax", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</c:chart>");
		}
		EElementType CT_Chart::getType() { return et_ct_Chart; }
		
		CT_Protection::CT_Protection()
		{
		}
		CT_Protection::~CT_Protection()
		{
		}
		void CT_Protection::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"chartObject" == sName)
				{
					m_chartObject = oReader;
				}
				else if (L"data" == sName)
				{
					m_data = oReader;
				}
				else if (L"formatting" == sName)
				{
					m_formatting = oReader;
				}
				else if (L"selection" == sName)
				{
					m_selection = oReader;
				}
				else if (L"userInterface" == sName)
				{
					m_userInterface = oReader;
				}
			}
		}
		void CT_Protection::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			m_chartObject.toXML(L"c:chartObject", writer);
			m_data.toXML(L"c:data", writer);
			m_formatting.toXML(L"c:formatting", writer);
            m_selection.toXML(L"c:selection", writer);
			m_userInterface.toXML(L"c:userInterface", writer);

			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_Protection::getType() { return et_ct_protection; }
		
		CT_PivotSource::CT_PivotSource()
		{
		}
		CT_PivotSource::~CT_PivotSource()
		{
		}
		void CT_PivotSource::fromXML(XmlUtils::CXmlLiteReader& oReader) 
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"name" == sName)
				{
					m_name = oReader.GetText3();
				}
				else if (L"fmtId" == sName)
				{
					m_fmtId = oReader;
				}
				else if (L"extLst" == sName)
				{
					m_extLst = oReader;
				}
			}
		}
		void CT_PivotSource::toXML(const std::wstring& sNodeName, NSStringUtils::CStringBuilder& writer) const 
		{
			writer.WriteString(L"<");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
			
			if (m_name.IsInit())
			{
				if (std::wstring::npos != m_name->find(' ')&&(  m_name->at(0) == ' ' || m_name->at(m_name->size()-1) == ' ')
				 			|| std::wstring::npos != m_name->find('\n')&&(  m_name->at(0) == '\n' || m_name->at(m_name->size()-1) == '\n'))
					writer.WriteString(L"<c:name xml:space=\"preserve\">");
				else
					writer.WriteString(L"<c:name>");
				writer.WriteEncodeXmlString(*m_name);
				writer.WriteString(L"</c:name>");
			}
			m_fmtId.toXML(L"c:fmtId", writer);

			if (m_extLst.IsInit())
			{
				writer.WriteString(m_extLst->toXMLWithNS(L"c:"));
			}
			writer.WriteString(L"</");
			writer.WriteString(sNodeName);
			writer.WriteString(L">");
		}
		EElementType CT_PivotSource::getType() { return et_ct_pivotsource; }
		
		CT_Style::CT_Style() : m_namespace(L"c")
		{
		}
		CT_Style::~CT_Style()
		{
		}
		void CT_Style::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CT_Style::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<" + m_namespace + L":style");
			if (m_val.IsInit())
			{
				WritingStringAttrInt(L"val", *m_val);
			}
			writer.WriteString(L"/>");
		}
		EElementType CT_Style::getType() { return et_ct_style; }		
		void CT_Style::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"val", m_val)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}

		AlternateContent::AlternateContent()
		{
			m_Fallback = NULL;
		}
		AlternateContent::~AlternateContent()
		{
			for (size_t i = 0; i < m_Choice.size(); ++i)
				delete m_Choice[i];
			m_Choice.clear();
			if (NULL != m_Fallback)
				delete m_Fallback;
		}
		void AlternateContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"Choice" == sName)
				{
					AlternateContentChoice* pNewElem = new AlternateContentChoice;
					pNewElem->fromXML(oReader);
					m_Choice.push_back(pNewElem);
				}
				else if (L"Fallback" == sName)
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
			for (size_t i = 0; i < m_Choice.size(); ++i)
			{
				AlternateContentChoice* pElem = m_Choice[i];
				if (NULL != pElem)
				{
					std::wstring sNodeName = L"mc:Choice";
					pElem->toXML(sNodeName, writer);
				}
			}
			if (NULL != m_Fallback)
			{
				std::wstring sNodeName = L"mc:Fallback";
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
		}
		AlternateContentChoice::~AlternateContentChoice()
		{
			if (NULL != m_style)
				delete m_style;
		}
		void AlternateContentChoice::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"style" == sName)
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
			if (m_Requires.IsInit())
			{
				WritingStringAttrEncodeXmlString(L"Requires", *m_Requires);
			}
			writer.WriteString(L">");
			if (NULL != m_style)
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
			WritingElement_ReadAttributes_Start_No_NS(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"Requires", m_Requires)
			WritingElement_ReadAttributes_End_No_NS(oReader)
		}
		AlternateContentFallback::AlternateContentFallback()
		{
			m_style = NULL;
		}
		AlternateContentFallback::~AlternateContentFallback()
		{
			if (NULL != m_style)
				delete m_style;
		}
		void AlternateContentFallback::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"style" == sName)
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
			if (NULL != m_style)
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
//-------------------------------------------------------------------------------------------
		CSeriesDataLabelsRange::CSeriesDataLabelsRange() {}
		CSeriesDataLabelsRange::~CSeriesDataLabelsRange()
		{
		}
		void CSeriesDataLabelsRange::fromXML(XmlUtils::CXmlNode& node) {}
		void CSeriesDataLabelsRange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"f" == sName)
				{
					m_f = oReader.GetText2();
				}
				else if (L"dlblRangeCache" == sName)
				{
					m_dlblRangeCache = oReader;
				}
			}
		}
		void CSeriesDataLabelsRange::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<c15:datalabelsRange>");

			if (m_f.IsInit())
			{
				writer.WriteString(L"<c15:f>" + *m_f + L"</c15:f>");
			}
			if (m_dlblRangeCache.IsInit())
			{
				m_dlblRangeCache->toXML(L"c15:dlblRangeCache", writer);
			}
			writer.WriteString(L"</c15:datalabelsRange>");
		}
		std::wstring CSeriesDataLabelsRange::toXML() const
		{
			NSStringUtils::CStringBuilder writer;
			toXML(writer);
			return writer.GetData();
		}
		EElementType CSeriesDataLabelsRange::getType() const
		{
			return et_ct_SeriesDataLabelsRange;
		}
		CCategoryFilterException::CCategoryFilterException() {}
		CCategoryFilterException::~CCategoryFilterException()
		{
		}
		void CCategoryFilterException::fromXML(XmlUtils::CXmlNode& node) {}
		void CCategoryFilterException::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"sqref" == sName)
				{
					m_sqref = oReader.GetText2();
				}
				else if (L"spPr" == sName)
				{
					m_spPr = oReader;
				}
				else if (L"explosion" == sName)
				{
					m_explosion = oReader;
				}
				else if (L"marker" == sName)
				{
					m_marker = oReader;
				}
				else if (L"invertIfNegative" == sName)
				{
					m_invertIfNegative = oReader;
				}
				else if (L"bubble3D" == sName)
				{
					m_bubble3D = oReader;
				}
				else if (L"dLbl" == sName)
				{
					m_dLbl = oReader;
				}
			}
		}
		void CCategoryFilterException::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<c15:categoryFilterException>");

			if (m_sqref.IsInit())
			{
				writer.WriteString(L"<c15:sqref>" + *m_sqref + L"</c15:sqref>");
			}
			if (m_spPr.IsInit())
			{
				writer.WriteString(m_spPr->toXML());
			}
			m_explosion.toXML(L"c15:explosion", writer);
			m_invertIfNegative.toXML(L"c15:invertIfNegative", writer);
			m_bubble3D.toXML(L"c15:bubble3D", writer);
			if (m_marker.IsInit())
			{
				m_marker->toXML(L"c15:marker", writer);
			}
			if (m_dLbl.IsInit())
			{
				m_dLbl->toXML(L"c15:dLbl", writer);
			}
			writer.WriteString(L"</c15:categoryFilterException>");
		}
		std::wstring CCategoryFilterException::toXML() const
		{
			NSStringUtils::CStringBuilder writer;
			toXML(writer);
			return writer.GetData();
		}
		EElementType CCategoryFilterException::getType() const
		{
			return et_ct_CategoryFilterException;
		}
		CCategoryFilterExceptions::CCategoryFilterExceptions() {}
		CCategoryFilterExceptions::~CCategoryFilterExceptions()
		{
		}
		void CCategoryFilterExceptions::fromXML(XmlUtils::CXmlNode& node) {}
		void CCategoryFilterExceptions::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"categoryFilterException" == sName)
				{
					CCategoryFilterException* pException = new CCategoryFilterException();
					pException->fromXML(oReader);
					m_arrItems.push_back(pException);
				}
			}
		}
		void CCategoryFilterExceptions::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_arrItems.empty()) return;
			
			writer.WriteString(L"<c15:categoryFilterExceptions>");
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</c15:categoryFilterExceptions>");
		}
		std::wstring CCategoryFilterExceptions::toXML() const
		{
			NSStringUtils::CStringBuilder writer;
			toXML(writer);
			return writer.GetData();
		}
		EElementType CCategoryFilterExceptions::getType() const
		{
			return et_ct_CategoryFilterExceptions;
		}
		CSeriesFiltering::CSeriesFiltering() {}
		CSeriesFiltering::~CSeriesFiltering()
		{
		}
		void CSeriesFiltering::fromXML(XmlUtils::CXmlNode& node) {}
		void CSeriesFiltering::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"datalabelsRange" == sName)
				{
					m_dataLabelsRange = oReader;
				}
				else if (L"filteredSeriesTitle" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"tx" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredSeriesTitle = oReader;
				}
				else if (L"filteredCategoryTitle" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"cat" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredCategoryTitle = oReader;
				}
				else if (L"filteredLineSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredLineSeries = oReader;
				}
				else if (L"filteredScatterSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredScatterSeries = oReader;
				}
				else if (L"filteredBarSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredBarSeries = oReader;
				}
				else if (L"filteredAreaSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredAreaSeries = oReader;
				}
				else if (L"filteredBubbleSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredBubbleSeries = oReader;
				}
				else if (L"filteredSurfaceSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredSurfaceSeries = oReader;
				}
				else if (L"filteredPieSeries" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"ser" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_filteredPieSeries = oReader;
				}
				else if (L"fullRef" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"sqref" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_fullRef = oReader.GetText2();
				}
				else if (L"levelRef" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"sqref" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_levelRef = oReader.GetText2();
				}
				else if (L"formulaRef" == sName)
				{
					if (oReader.ReadNextSiblingNode(nParentDepth + 1) && L"sqref" == XmlUtils::GetNameNoNS(oReader.GetName()))
						m_formulaRef = oReader.GetText2();
				}
				else if (L"categoryFilterExceptions" == sName)
				{
					m_categoryFilterExceptions = oReader;
				}
			}
		}
		void CSeriesFiltering::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if (m_dataLabelsRange.IsInit())
			{
				m_dataLabelsRange->toXML(writer);
			}
			if (m_filteredSeriesTitle.IsInit())
			{
				writer.WriteString(L"<c15:filteredSeriesTitle>");
					m_filteredSeriesTitle->toXML(L"c15:tx", writer);
				writer.WriteString(L"</c15:filteredSeriesTitle>");
			}
			if (m_filteredCategoryTitle.IsInit())
			{
				writer.WriteString(L"<c15:filteredCategoryTitle>");
					m_filteredCategoryTitle->toXML(L"c15:cat", writer);
				writer.WriteString(L"</c15:filteredCategoryTitle>");
			}
			if (m_filteredLineSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredLineSeries>");
				m_filteredLineSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredLineSeries>");
			}
			if (m_filteredScatterSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredScatterSeries>");
				m_filteredScatterSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredScatterSeries>");
			}
			if (m_filteredRadarSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredRadarSeries>");
				m_filteredRadarSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredRadarSeries>");
			}
			if (m_filteredBarSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredBarSeries>");
				m_filteredBarSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredBarSeries>");
			}
			if (m_filteredAreaSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredAreaSeries>");
				m_filteredAreaSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredAreaSeries>");
			}
			if (m_filteredBubbleSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredBubbleSeries>");
				m_filteredBubbleSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredBubbleSeries>");
			}
			if (m_filteredSurfaceSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredSurfaceSeries>");
				m_filteredSurfaceSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredSurfaceSeries>");
			}
			if (m_filteredPieSeries.IsInit())
			{
				writer.WriteString(L"<c15:filteredPieSeries>");
				m_filteredPieSeries->toXML(L"c15:ser", writer);
				writer.WriteString(L"</c15:filteredPieSeries>");
			}
			if (m_fullRef.IsInit())
			{
				writer.WriteString(L"<c15:fullRef>");
				writer.WriteString(L"<c15:sqref>" + *m_fullRef + L"</c15:sqref>");
				writer.WriteString(L"</c15:fullRef>");
			}
			if (m_levelRef.IsInit())
			{
				writer.WriteString(L"<c15:levelRef>");
				writer.WriteString(L"<c15:sqref>" + *m_levelRef + L"</c15:sqref>");
				writer.WriteString(L"</c15:levelRef>");
			}
			if (m_formulaRef.IsInit())
			{
				writer.WriteString(L"<c15:formulaRef>");
				writer.WriteString(L"<c15:sqref>" + *m_formulaRef + L"</c15:sqref>");
				writer.WriteString(L"</c15:formulaRef>");
			}
			if (m_categoryFilterExceptions.IsInit())
			{
				m_categoryFilterExceptions->toXML(writer);
			}
		}
		std::wstring CSeriesFiltering::toXML() const
		{
			NSStringUtils::CStringBuilder writer;
			toXML(writer);
			return writer.GetData();
		}
		EElementType CSeriesFiltering::getType() const
		{
			return et_ct_SeriesFiltering;
		}
	}
}
