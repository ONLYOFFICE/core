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
#include "RtfMath.h"

#include "../../OOXML/DocxFormat/WritingElement.h"

RtfMath::RtfMath( )
{
	m_bHeader = true;
	m_bIsVal = m_bIsChar = m_bIsBool = false;
}
RtfMath::RtfMath(const RtfMath& oMath )
{
	(*this) = oMath;
}
RtfMath& RtfMath::operator=(const RtfMath& oMath )
{
	m_bHeader	= oMath.m_bHeader;
	m_bIsVal	= oMath.m_bIsVal;
	m_bIsChar	= oMath.m_bIsChar;
	m_bIsBool	= oMath.m_bIsBool;

	m_sRtfName	= oMath.m_sRtfName;
	m_sOOXName	= oMath.m_sOOXName;

	m_aArray.clear();
	m_aArray	= oMath.m_aArray;

	return (*this);
}
bool RtfMath::IsEmpty()
{
	return m_sRtfName.empty();
}
bool RtfMath::IsValid()
{
	return (false == m_sRtfName.empty());
}

bool RtfMath::IsRtfControlPropertyBool( std::string sControl )
{
    const char* mc_aRtfControlWords[]		= { "maln", "malnScr", "mdegHide", "mdiff", "mdispDef",
    "mgrow", "mhideBot", "mhideLeft", "mhideRight", "mhideTop", "mlit", "mmaxDist", "mnoBreak", "mnor",
    "mobjDist", "mopEmu", "mplcHide", "mshow", "msmallFracN", "mstrikeBLTR", "mstrikeH", "mstrikeTLBR",
    "mstrikeV", "msubHide", "msupHide", "mtransp", "mwrapRight", "mzeroAsc", "mzeroDesc", "mzeroWid"};

    int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( char* );

	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
        if( mc_aRtfControlWords[ i ] == sControl )
			return true;
	return false;
}
bool RtfMath::IsRtfControlProperty( std::string sControl )
{
    const char* mc_aRtfControlWords[]		= {"margSz", "mbaseJc", "mbegChr", "mbrkBin", "mbrkBinSub", "mcGp",
    "mcGpRule", "mchr", "mcount", "mcSp", "mdefJc", "mendChr", "minterSp", "mintLim", "mintraSp", "mmJc",
    "mlimLoc", "mlMargin", "mmcJc", "mnaryLim", "mpos", "mpostSp", "mpreSp", "mrMargin", "mrSp", "mrSpRule",
    "msepChr", "mshp", "mtype", "mvertJc", "mwrapIndent", "mmathFont"};

    int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( char* );
	
	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
	{
        if( mc_aRtfControlWords[ i ] == sControl )
			return true;
	}
	return false;
}
bool RtfMath::IsRtfControlWord( std::string sControl )
{
    const char* mc_aRtfControlWords[]		= { "moMath", "moMathPara", "moMathParaPr", "maccPr", "macc",
    "me", "mctrlPr", "margPr", "mbarPr", "mbar", "mborderBoxPr", "mborderBox", "mboxPr", "mbox"/*, "mbrk"*/,
    "mdPr", "md", "meqArrPr", "meqArr", "mfPr", "mf", "mfuncPr", "mfunc",
    "mgroupChrPr", "mgroupChr", "mlimLowPr", "mlimLow","mlimUppPr", "mlimUpp", "mmathPr", "mmcPr", "mmc",
    "mmcs", "mmPr", "mmr", "mm", "mnaryPr", "mnary", "mphantPr", "mphant", "mmRun", "mmDel",
    "mmIns", "mradPr", "mrad", /*MRPr",*/ "msPrePr", "msPre", "msSubPr", "msSub", "msSubSupPr",
    "msSubSup", "msSupPr", "msSup", "msub", "msup", "mden", "mlim", "mnum", "mdeg"/*mmText",*/, "mfName", "mscr", "mrPr" };

    int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( char* );
	
	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
	{
        if( mc_aRtfControlWords[ i ] == sControl )
			return true;
	}
	return false;
}
std::wstring RtfMath::RenderToRtf(RenderParameter oRenderParameter)
{
    if (m_sRtfName.empty())
		return L"";

    std::wstring sResult;
	if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
		sResult += L"{\\mmath";

    sResult += L"{\\" + std::wstring(m_sRtfName.begin(), m_sRtfName.end());
	RenderParameter oNewParameter = oRenderParameter;

    std::wstring sVal;

	if (m_bIsVal)
	{
		if (false == m_bIsChar)
		{
			sVal += L" " + m_sVal;
		}
		else
		{
			oNewParameter.nType = RENDER_TO_RTF_PARAM_CHAR;
			sVal = L" " + m_oVal.RenderToRtf(oNewParameter);
		}
	}
	else
	{
		oNewParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;
		for( int i = 0; i < m_oVal.GetCount(); i++ )
		{
			sVal += L" " + m_oVal[i]->RenderToRtf( oNewParameter );
		}
	}
    if (!sVal.empty())
	{
		sResult += sVal;
	}
	//else
	{
		oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;
		for (size_t i = 0; i < m_aArray.size(); i++ )
			sResult += m_aArray[i]->RenderToRtf( oNewParameter );
	}
	sResult += L"}";

	if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
        sResult += L"}";// m_sRtfName
	return sResult;
}
std::wstring RtfMath::RenderToOOX(RenderParameter oRenderParameter)
{
    if (m_sOOXName.empty()) return L"";

    std::wstring sResult;
    std::wstring sContent;
    std::wstring sProp;
    std::wstring sVal;

	RenderParameter oNewParam = oRenderParameter;

	if (m_bIsVal || m_bIsBool)
	{
		oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sVal += m_aArray[i]->RenderToOOX(oNewParam);
		}

        if (sVal.empty())
		{
			oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
			sVal = m_oVal.RenderToOOX(oNewParam);
		}
	}
	else
	{
		oNewParam.nType = RENDER_TO_OOX_PARAM_MATH;
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sContent += m_aArray[i]->RenderToOOX(oNewParam);
		}
	}
    std::wstring wsOOXName(m_sOOXName.begin(), m_sOOXName.end());

    sResult += L"<" + wsOOXName;
    if( false == sVal.empty() )
	{
		if (m_bIsBool)
		{
			if (sVal == L"on")	sVal = L"1";
			else				sVal = L"0";
		}
		sResult += L" m:val=\"";
		sResult += sVal;
		sResult += L"\"";
	}
	sResult += L">";

	sResult += sProp; //??

	sResult += sContent;

    sResult += L"</" + wsOOXName + L">";

//альтернативная картинка
//	if( NULL != m_oPicture )
//		sResult += m_oPicture->RenderToOOX(oRenderParameter);

	return sResult;
}
void RtfMath::SetOOXType(int type)
{
	OOX::EElementType type_ms = (OOX::EElementType)type;
	switch(type_ms)
	{
        case OOX::et_m_acc:			m_sRtfName = "macc";			break;
        case OOX::et_m_accPr:		m_sRtfName = "maccPr";			break;
        case OOX::et_m_aln:			m_sRtfName = "maln";			break;
        case OOX::et_m_alnScr:		m_sRtfName = "malnScr";         break;
        case OOX::et_m_argPr:		m_sRtfName = "margPr";			break;
        case OOX::et_m_argSz:		m_sRtfName = "margSz";			break;
        case OOX::et_m_bar:			m_sRtfName = "mbar";			break;
        case OOX::et_m_barPr:		m_sRtfName = "mbarPr";			break;
        case OOX::et_m_baseJc:		m_sRtfName = "mbaseJc";         break;
        case OOX::et_m_begChr:		m_sRtfName = "mbegChr";         break;
        case OOX::et_m_borderBox:	m_sRtfName = "mborderBox";		break;
        case OOX::et_m_borderBoxPr:	m_sRtfName = "mborderBoxPr";	break;
        case OOX::et_m_box:			m_sRtfName = "mbox";			break;
        case OOX::et_m_boxPr:		m_sRtfName = "mboxPr";			break;
        case OOX::et_m_brk:			m_sRtfName = "mbrk";			break;
        case OOX::et_m_brkBin:		m_sRtfName = "mbrkBin";         break;
        case OOX::et_m_brkBinSub:	m_sRtfName = "mbrkBinSub";		break;
        case OOX::et_m_cGp:			m_sRtfName = "mcGp";			break;
        case OOX::et_m_cGpRule:		m_sRtfName = "mcGpRule";		break;
        case OOX::et_m_chr:			m_sRtfName = "mchr";			break;
        case OOX::et_m_count:		m_sRtfName = "mcount";			break;
        case OOX::et_m_cSp:			m_sRtfName = "mcSp";			break;
        case OOX::et_m_ctrlPr:		m_sRtfName = "mctrlPr";         break;
        case OOX::et_m_d:			m_sRtfName = "md";				break;
        case OOX::et_m_defJc:		m_sRtfName = "mdefJc";			break;
        case OOX::et_m_deg:			m_sRtfName = "mdeg";			break;
        case OOX::et_m_degHide:		m_sRtfName = "mdegHide";		break;
        case OOX::et_m_den:			m_sRtfName = "mden";			break;
        case OOX::et_m_diff:		m_sRtfName = "mdiff";			break;
        case OOX::et_m_dispDef:		m_sRtfName = "mdispDef";		break;
        case OOX::et_m_dPr:			m_sRtfName = "mdPr";			break;
        case OOX::et_m_e:			m_sRtfName = "me";				break;
        case OOX::et_m_endChr:		m_sRtfName = "mendChr";         break;
        case OOX::et_m_eqArr:		m_sRtfName = "meqArr";			break;
        case OOX::et_m_eqArrPr:		m_sRtfName = "meqArrPr";		break;
        case OOX::et_m_f:			m_sRtfName = "mf";				break;
        case OOX::et_m_fName:		m_sRtfName = "mfName";			break;
        case OOX::et_m_fPr:			m_sRtfName = "mfPr";			break;
        case OOX::et_m_func:		m_sRtfName = "mfunc";			break;
        case OOX::et_m_funcPr:		m_sRtfName = "mfuncPr";         break;
        case OOX::et_m_groupChr:	m_sRtfName = "mgroupChr";		break;
        case OOX::et_m_groupChrPr:	m_sRtfName = "mgroupChrPr";     break;
        case OOX::et_m_grow:		m_sRtfName = "mgrow";			break;
        case OOX::et_m_hideBot:		m_sRtfName = "mhideBot";		break;
        case OOX::et_m_hideLeft:	m_sRtfName = "mhideLeft";		break;
        case OOX::et_m_hideRight:	m_sRtfName = "mhideRight";		break;
        case OOX::et_m_hideTop:		m_sRtfName = "mhideTop";		break;
        case OOX::et_m_interSp:		m_sRtfName = "minterSp";		break;
        case OOX::et_m_intLim:		m_sRtfName = "mintLim";         break;
        case OOX::et_m_intraSp:		m_sRtfName = "mintraSp";		break;
        case OOX::et_m_jc:			m_sRtfName = "mjc";             break;
        case OOX::et_m_lim:			m_sRtfName = "mlim";			break;
        case OOX::et_m_limLoc:		m_sRtfName = "mlimLoc";         break;
        case OOX::et_m_limLow:		m_sRtfName = "mlimLow";         break;
        case OOX::et_m_limLowPr:	m_sRtfName = "mlimLowPr";		break;
        case OOX::et_m_limUpp:		m_sRtfName = "mlimUpp";         break;
        case OOX::et_m_limUppPr:	m_sRtfName = "mlimUppPr";		break;
        case OOX::et_m_lit:			m_sRtfName = "mlit";			break;
        case OOX::et_m_lMargin:		m_sRtfName = "mlMargin";		break;
        case OOX::et_m_m:			m_sRtfName = "mm";				break;
        case OOX::et_m_mathFont:	m_sRtfName = "mmathFont";		break;
        case OOX::et_m_mathPr:		m_sRtfName = "mmathPr";         break;
        case OOX::et_m_maxDist:		m_sRtfName = "mmaxDist";		break;
        case OOX::et_m_mc:			m_sRtfName = "mmc";             break;
        case OOX::et_m_mcJc:		m_sRtfName = "mmcJc";			break;
        case OOX::et_m_mcPr:		m_sRtfName = "mmcPr";			break;
        case OOX::et_m_mcs:			m_sRtfName = "mmcs";			break;
        case OOX::et_m_mPr:			m_sRtfName = "mmPr";			break;
        case OOX::et_m_mr:			m_sRtfName = "mmr";             break;
        case OOX::et_m_nary:		m_sRtfName = "mnary";			break;
        case OOX::et_m_naryLim:		m_sRtfName = "mnaryLim";		break;
        case OOX::et_m_naryPr:		m_sRtfName = "mnaryPr";         break;
        case OOX::et_m_noBreak:		m_sRtfName = "mnoBreak";		break;
        case OOX::et_m_nor:			m_sRtfName = "mnor";			break;
        case OOX::et_m_num:			m_sRtfName = "mnum";			break;
        case OOX::et_m_objDist:		m_sRtfName = "mobjDist";		break;
        case OOX::et_m_oMath:		m_sRtfName = "moMath";			break;
        case OOX::et_m_oMathPara:	m_sRtfName = "moMathPara";		break;
        case OOX::et_m_oMathParaPr:	m_sRtfName = "moMathParaPr";	break;
        case OOX::et_m_opEmu:		m_sRtfName = "mopEmu";			break;
        case OOX::et_m_phant:		m_sRtfName = "mphant";			break;
        case OOX::et_m_phantPr:		m_sRtfName = "mphantPr";		break;
        case OOX::et_m_plcHide:		m_sRtfName = "mplcHide";		break;
        case OOX::et_m_pos:			m_sRtfName = "mpos";			break;
        case OOX::et_m_postSp:		m_sRtfName = "mpostSp";         break;
        case OOX::et_m_preSp:		m_sRtfName = "mpreSp";			break;
        case OOX::et_m_r:			m_sRtfName = "mr";				break;
        case OOX::et_m_rad:			m_sRtfName = "mrad";			break;
        case OOX::et_m_radPr:		m_sRtfName = "mradPr";			break;
        case OOX::et_m_rMargin:		m_sRtfName = "mrMargin";		break;
        case OOX::et_m_rPr:			m_sRtfName = "mrPr";			break;
        case OOX::et_m_rSp:			m_sRtfName = "mrSp";			break;
        case OOX::et_m_rSpRule:		m_sRtfName = "mrSpRule";		break;
        case OOX::et_m_scr:			m_sRtfName = "mscr";			break;
        case OOX::et_m_sepChr:		m_sRtfName = "msepChr";         break;
        case OOX::et_m_show:		m_sRtfName = "mshow";			break;
        case OOX::et_m_shp:			m_sRtfName = "mshp";			break;
        case OOX::et_m_smallFrac:	m_sRtfName = "msmallFrac";		break;
        case OOX::et_m_sPre:		m_sRtfName = "msPre";			break;
        case OOX::et_m_sPrePr:		m_sRtfName = "msPrePr";         break;
        case OOX::et_m_sSub:		m_sRtfName = "msSub";			break;
        case OOX::et_m_sSubPr:		m_sRtfName = "msSubPr";         break;
        case OOX::et_m_sSubSup:		m_sRtfName = "msSubSup";		break;
        case OOX::et_m_sSubSupPr:	m_sRtfName = "msSubSupPr";		break;
        case OOX::et_m_sSup:		m_sRtfName = "msSup";			break;
        case OOX::et_m_sSupPr:		m_sRtfName = "msSupPr";         break;
        case OOX::et_m_strikeBLTR:	m_sRtfName = "mstrikeBLTR";     break;
        case OOX::et_m_strikeH:		m_sRtfName = "mstrikeH";		break;
        case OOX::et_m_strikeTLBR:	m_sRtfName = "mstrikeTLBR";     break;
        case OOX::et_m_strikeV:		m_sRtfName = "mstrikeV";		break;
        case OOX::et_m_sty:			m_sRtfName = "msty";			break;
        case OOX::et_m_sub:			m_sRtfName = "msub";			break;
        case OOX::et_m_subHide:		m_sRtfName = "msubHide";		break;
        case OOX::et_m_sup:			m_sRtfName = "msup";			break;
        case OOX::et_m_supHide:		m_sRtfName = "msupHide";		break;
        case OOX::et_m_t:			m_sRtfName = "mt";				break;
        case OOX::et_m_transp:		m_sRtfName = "mtransp";         break;
        case OOX::et_m_type:		m_sRtfName = "mtype";			break;
        case OOX::et_m_vertJc:		m_sRtfName = "mvertJc";         break;
        case OOX::et_m_wrapIndent:	m_sRtfName = "mwrapIndent";     break;
        case OOX::et_m_wrapRight:	m_sRtfName = "mwrapRight";		break;
        case OOX::et_m_zeroAsc:		m_sRtfName = "mzeroAsc";		break;
        case OOX::et_m_zeroDesc:	m_sRtfName = "mzeroDesc";		break;
        case OOX::et_m_zeroWid:		m_sRtfName = "mzeroWid";		break;
		default:
		{
            m_sRtfName = "";
			break;
		}
	}
}
void RtfMath::SetRtfName(std::string sName)
{
	m_sRtfName = sName;

	m_sOOXName = sName;
	m_sOOXName.insert(m_sOOXName.begin() + 1, L':');
}
