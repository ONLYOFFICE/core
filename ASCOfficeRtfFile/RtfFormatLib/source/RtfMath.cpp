/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../../Common/DocxFormat/Source/DocxFormat/WritingElement.h"

bool RtfMath::IsRtfControlPropertyBool( CString sControlW )
{
	const TCHAR* mc_aRtfControlWords[]		= { L"maln", L"malnScr", L"mdegHide", L"mdiff", L"mdispDef", 
	L"mgrow", L"mhideBot", L"mhideLeft", L"mhideRight", L"mhideTop", L"mlit", L"mmaxDist", L"mnoBreak", L"mnor", 
	L"mobjDist", L"mopEmu", L"mplcHide", L"mshow", L"msmallFracN", L"mstrikeBLTR", L"mstrikeH", L"mstrikeTLBR", 
	L"mstrikeV", L"msubHide", L"msupHide", L"mtransp", L"mwrapRight", L"mzeroAsc", L"mzeroDesc", L"mzeroWid"};

	int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( TCHAR* );

	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
		if( mc_aRtfControlWords[ i ] == sControlW )
			return true;
	return false;
}
bool RtfMath::IsRtfControlProperty( CString sControlW )
{
	const TCHAR* mc_aRtfControlWords[]		= {L"margSz", L"mbaseJc", L"mbegChr", L"mbrkBin", L"mbrkBinSub", L"mcGp", 
	L"mcGpRule", L"mchr", L"mcount", L"mcSp", L"mdefJc", L"mendChr", L"minterSp", L"mintLim", L"mintraSp", L"mmJc", 
	L"mlimLoc", L"mlMargin", L"mmcJc", L"mnaryLim", L"mpos", L"mpostSp", L"mpreSp", L"mrMargin", L"mrSp", L"mrSpRule",
	L"msepChr", L"mshp", L"mtype", L"mvertJc", L"mwrapIndent", L"mmathFont"};

	int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( TCHAR* );
	
	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
		if( mc_aRtfControlWords[ i ] == sControlW )
			return true;
	return false;
}

bool RtfMath::IsRtfControlWord( CString sControlW )
{
	const TCHAR* mc_aRtfControlWords[]		= { L"moMath", L"moMathPara", L"moMathParaPr", L"maccPr", L"macc", 
	L"me", L"mctrlPr", L"margPr", L"mbarPr", L"mbar", L"mborderBoxPr", L"mborderBox", L"mboxPr", L"mbox"/*, L"mbrk"*/, 
	L"mdPr", L"md", L"meqArrPr", L"meqArr", L"mfPr", L"mf", L"mfuncPr", L"mfunc", 
	L"mgroupChrPr", L"mgroupChr", L"mlimLowPr", L"mlimLow",L"mlimUppPr", L"mlimUpp", L"mmathPr", L"mmcPr", L"mmc", 
	L"mmcs", L"mmPr", L"mmr", L"mm", L"mnaryPr", L"mnary", L"mphantPr", L"mphant", L"mmRun", L"mmDel", 
	L"mmIns", L"mradPr", L"mrad", /*MRPr",*/ L"msPrePr", L"msPre", L"msSubPr", L"msSub", L"msSubSupPr", 
	L"msSubSup", L"msSupPr", L"msSup", L"msub", L"msup", L"mden", L"mlim", L"mnum", L"mdeg"/*mmText",*/, L"mfName", L"mscr", L"mrPr" };

	int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( TCHAR* );
	
	//можно бинарный поиск вставить
	for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
		if( mc_aRtfControlWords[ i ] == sControlW )
			return true;
	return false;
}

CString RtfMath::RenderToRtf(RenderParameter oRenderParameter)
{
	if (m_sRtfName.IsEmpty())
		return L"";

	CString sResult;
	if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
		sResult.Append(_T("{\\mmath"));

    sResult.AppendFormat(_T("{\\%ls"), m_sRtfName.GetBuffer());
	RenderParameter oNewParameter = oRenderParameter;

	if (m_sRtfName == L"mctrlPr")
	{
		m_bIsVal = m_bIsVal;
	}

	CString sVal;

	if (m_bIsVal)
	{
		oNewParameter.nType = RENDER_TO_RTF_PARAM_CHAR;
		sVal = m_oVal.RenderToRtf( oNewParameter ) ;
		if (!sVal.IsEmpty())
			sVal = _T(" ") + sVal;
	}
	else
	{
		oNewParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;
		for( int i = 0; i < m_oVal.GetCount(); i++ )
		{
			sVal.Append( _T(" ") + m_oVal[i]->RenderToRtf( oNewParameter ) );
		}
	}
	if (!sVal.IsEmpty())
	{
		sResult += sVal;
	}
	//else
	{
		oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			sResult.Append(m_aArray[i]->RenderToRtf( oNewParameter ));
	}
	sResult.Append(_T("}"));

	if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
        sResult.AppendFormat(_T("}"), m_sRtfName.GetBuffer());
	return sResult;
}
CString RtfMath::RenderToOOX(RenderParameter oRenderParameter)
{
	if (m_sOOXName.IsEmpty()) return L"";

	CString sResult;
	CString sContent;
	CString sProp;
	CString sVal;

	RenderParameter oNewParam = oRenderParameter;

	if (m_bIsVal || m_bIsBool)
	{
		oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
			sVal += m_aArray[i]->RenderToOOX(oNewParam);
		}

		if (sVal.IsEmpty())
		{
			oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
			sVal = m_oVal.RenderToOOX(oNewParam);
		}
	}
	else
	{
		oNewParam.nType = RENDER_TO_OOX_PARAM_MATH;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
		{
			sContent += m_aArray[i]->RenderToOOX(oNewParam);
		}
	}

	sResult.Append( _T("<") );
	sResult += m_sOOXName;

	if( false == sVal.IsEmpty() )
	{
		if (m_bIsBool)
		{
			if (sVal == L"on")	sVal = L"1";
			else				sVal = L"0";
		}
		sResult.Append( _T(" m:val=\"") );
		sResult += sVal;
		sResult.Append( _T("\"") );
	}
	sResult.Append( _T(">") );

	sResult += sProp;

	sResult += sContent;

	sResult.Append(_T("</") );
		sResult += m_sOOXName;
	sResult.Append(_T(">"));

//альтернативная картинка
//	if( NULL != m_oPicture )
//		sResult.Append( m_oPicture->RenderToOOX(oRenderParameter) );

	return sResult;
}
void RtfMath::SetOOXType(int type)
{
	OOX::EElementType type_ms = (OOX::EElementType)type;
	switch(type_ms)
	{
		case OOX::et_m_acc:			m_sRtfName = L"macc";			break;
		case OOX::et_m_accPr:		m_sRtfName = L"maccPr";			break;
		case OOX::et_m_aln:			m_sRtfName = L"maln";			break;
		case OOX::et_m_alnScr:		m_sRtfName = L"malnScr";		break;
		case OOX::et_m_argPr:		m_sRtfName = L"margPr";			break;
    	case OOX::et_m_argSz:		m_sRtfName = L"margSz";			break;
		case OOX::et_m_bar:			m_sRtfName = L"mbar";			break;
		case OOX::et_m_barPr:		m_sRtfName = L"mbarPr";			break;
		case OOX::et_m_baseJc:		m_sRtfName = L"mbaseJc";		break;
		case OOX::et_m_begChr:		m_sRtfName = L"mbegChr";		break;
		case OOX::et_m_borderBox:	m_sRtfName = L"mborderBox";		break;
		case OOX::et_m_borderBoxPr:	m_sRtfName = L"mborderBoxPr";	break;
		case OOX::et_m_box:			m_sRtfName = L"mbox";			break;
		case OOX::et_m_boxPr:		m_sRtfName = L"mboxPr";			break;
		case OOX::et_m_brk:			m_sRtfName = L"mbrk";			break;
		case OOX::et_m_brkBin:		m_sRtfName = L"mbrkBin";		break;
		case OOX::et_m_brkBinSub:	m_sRtfName = L"mbrkBinSub";		break;
		case OOX::et_m_cGp:			m_sRtfName = L"mcGp";			break;
		case OOX::et_m_cGpRule:		m_sRtfName = L"mcGpRule";		break;
		case OOX::et_m_chr:			m_sRtfName = L"mchr";			break;
		case OOX::et_m_count:		m_sRtfName = L"mcount";			break;
		case OOX::et_m_cSp:			m_sRtfName = L"mcSp";			break;
		case OOX::et_m_ctrlPr:		m_sRtfName = L"mctrlPr";		break;
		case OOX::et_m_d:			m_sRtfName = L"md";				break;
		case OOX::et_m_defJc:		m_sRtfName = L"mdefJc";			break;
		case OOX::et_m_deg:			m_sRtfName = L"mdeg";			break;
		case OOX::et_m_degHide:		m_sRtfName = L"mdegHide";		break;
		case OOX::et_m_den:			m_sRtfName = L"mden";			break;
		case OOX::et_m_diff:		m_sRtfName = L"mdiff";			break;
		case OOX::et_m_dispDef:		m_sRtfName = L"mdispDef";		break;
		case OOX::et_m_dPr:			m_sRtfName = L"mdPr";			break;
		case OOX::et_m_e:			m_sRtfName = L"me";				break;
		case OOX::et_m_endChr:		m_sRtfName = L"mendChr";		break;
		case OOX::et_m_eqArr:		m_sRtfName = L"meqArr";			break;
		case OOX::et_m_eqArrPr:		m_sRtfName = L"meqArrPr";		break;
		case OOX::et_m_f:			m_sRtfName = L"mf";				break;
		case OOX::et_m_fName:		m_sRtfName = L"mfName";			break;
		case OOX::et_m_fPr:			m_sRtfName = L"mfPr";			break;
		case OOX::et_m_func:		m_sRtfName = L"mfunc";			break;
		case OOX::et_m_funcPr:		m_sRtfName = L"mfuncPr";		break;
		case OOX::et_m_groupChr:	m_sRtfName = L"mgroupChr";		break;
		case OOX::et_m_groupChrPr:	m_sRtfName = L"mgroupChrPr";	break;
		case OOX::et_m_grow:		m_sRtfName = L"mgrow";			break;
		case OOX::et_m_hideBot:		m_sRtfName = L"mhideBot";		break;
		case OOX::et_m_hideLeft:	m_sRtfName = L"mhideLeft";		break;
		case OOX::et_m_hideRight:	m_sRtfName = L"mhideRight";		break;
		case OOX::et_m_hideTop:		m_sRtfName = L"mhideTop";		break;
		case OOX::et_m_interSp:		m_sRtfName = L"minterSp";		break;
		case OOX::et_m_intLim:		m_sRtfName = L"mintLim";		break;
		case OOX::et_m_intraSp:		m_sRtfName = L"mintraSp";		break;
		case OOX::et_m_jc:			m_sRtfName = L"mjc";			break;
		case OOX::et_m_lim:			m_sRtfName = L"mlim";			break;
		case OOX::et_m_limLoc:		m_sRtfName = L"mlimLoc";		break;
		case OOX::et_m_limLow:		m_sRtfName = L"mlimLow";		break;
		case OOX::et_m_limLowPr:	m_sRtfName = L"mlimLowPr";		break;
		case OOX::et_m_limUpp:		m_sRtfName = L"mlimUpp";		break;
		case OOX::et_m_limUppPr:	m_sRtfName = L"mlimUppPr";		break;
		case OOX::et_m_lit:			m_sRtfName = L"mlit";			break;
		case OOX::et_m_lMargin:		m_sRtfName = L"mlMargin";		break;
		case OOX::et_m_m:			m_sRtfName = L"mm";				break;
		case OOX::et_m_mathFont:	m_sRtfName = L"mmathFont";		break;
		case OOX::et_m_mathPr:		m_sRtfName = L"mmathPr";		break;
		case OOX::et_m_maxDist:		m_sRtfName = L"mmaxDist";		break;
		case OOX::et_m_mc:			m_sRtfName = L"mmc";			break;
		case OOX::et_m_mcJc:		m_sRtfName = L"mmcJc";			break;
		case OOX::et_m_mcPr:		m_sRtfName = L"mmcPr";			break;
		case OOX::et_m_mcs:			m_sRtfName = L"mmcs";			break;
		case OOX::et_m_mPr:			m_sRtfName = L"mmPr";			break;
		case OOX::et_m_mr:			m_sRtfName = L"mmr";			break;
		case OOX::et_m_nary:		m_sRtfName = L"mnary";			break;
		case OOX::et_m_naryLim:		m_sRtfName = L"mnaryLim";		break;
		case OOX::et_m_naryPr:		m_sRtfName = L"mnaryPr";		break;
		case OOX::et_m_noBreak:		m_sRtfName = L"mnoBreak";		break;
		case OOX::et_m_nor:			m_sRtfName = L"mnor";			break;
		case OOX::et_m_num:			m_sRtfName = L"mnum";			break;
		case OOX::et_m_objDist:		m_sRtfName = L"mobjDist";		break;
		case OOX::et_m_oMath:		m_sRtfName = L"moMath";			break;
		case OOX::et_m_oMathPara:	m_sRtfName = L"moMathPara";		break;
		case OOX::et_m_oMathParaPr:	m_sRtfName = L"moMathParaPr";	break;
		case OOX::et_m_opEmu:		m_sRtfName = L"mopEmu";			break;
		case OOX::et_m_phant:		m_sRtfName = L"mphant";			break;
		case OOX::et_m_phantPr:		m_sRtfName = L"mphantPr";		break;
		case OOX::et_m_plcHide:		m_sRtfName = L"mplcHide";		break;
		case OOX::et_m_pos:			m_sRtfName = L"mpos";			break;
		case OOX::et_m_postSp:		m_sRtfName = L"mpostSp";		break;
		case OOX::et_m_preSp:		m_sRtfName = L"mpreSp";			break;
		case OOX::et_m_r:			m_sRtfName = L"mr";				break;
		case OOX::et_m_rad:			m_sRtfName = L"mrad";			break;
		case OOX::et_m_radPr:		m_sRtfName = L"mradPr";			break;
		case OOX::et_m_rMargin:		m_sRtfName = L"mrMargin";		break;
		case OOX::et_m_rPr:			m_sRtfName = L"mrPr";			break;
		case OOX::et_m_rSp:			m_sRtfName = L"mrSp";			break;
		case OOX::et_m_rSpRule:		m_sRtfName = L"mrSpRule";		break;
		case OOX::et_m_scr:			m_sRtfName = L"mscr";			break;
		case OOX::et_m_sepChr:		m_sRtfName = L"msepChr";		break;
		case OOX::et_m_show:		m_sRtfName = L"mshow";			break;
		case OOX::et_m_shp:			m_sRtfName = L"mshp";			break;
		case OOX::et_m_smallFrac:	m_sRtfName = L"msmallFrac";		break;
		case OOX::et_m_sPre:		m_sRtfName = L"msPre";			break;
		case OOX::et_m_sPrePr:		m_sRtfName = L"msPrePr";		break;
		case OOX::et_m_sSub:		m_sRtfName = L"msSub";			break;
		case OOX::et_m_sSubPr:		m_sRtfName = L"msSubPr";		break;
		case OOX::et_m_sSubSup:		m_sRtfName = L"msSubSup";		break;
		case OOX::et_m_sSubSupPr:	m_sRtfName = L"msSubSupPr";		break;
		case OOX::et_m_sSup:		m_sRtfName = L"msSup";			break;
		case OOX::et_m_sSupPr:		m_sRtfName = L"msSupPr";		break;
		case OOX::et_m_strikeBLTR:	m_sRtfName = L"mstrikeBLTR";	break;
		case OOX::et_m_strikeH:		m_sRtfName = L"mstrikeH";		break;
		case OOX::et_m_strikeTLBR:	m_sRtfName = L"mstrikeTLBR";	break;
		case OOX::et_m_strikeV:		m_sRtfName = L"mstrikeV";		break;
		case OOX::et_m_sty:			m_sRtfName = L"msty";			break;
		case OOX::et_m_sub:			m_sRtfName = L"msub";			break;
		case OOX::et_m_subHide:		m_sRtfName = L"msubHide";		break;
		case OOX::et_m_sup:			m_sRtfName = L"msup";			break;
		case OOX::et_m_supHide:		m_sRtfName = L"msupHide";		break;
		case OOX::et_m_t:			m_sRtfName = L"mt";				break;
		case OOX::et_m_transp:		m_sRtfName = L"mtransp";		break;
		case OOX::et_m_type:		m_sRtfName = L"mtype";			break;
		case OOX::et_m_vertJc:		m_sRtfName = L"mvertJc";		break;
		case OOX::et_m_wrapIndent:	m_sRtfName = L"mwrapIndent";	break;
		case OOX::et_m_wrapRight:	m_sRtfName = L"mwrapRight";		break;
		case OOX::et_m_zeroAsc:		m_sRtfName = L"mzeroAsc";		break;
		case OOX::et_m_zeroDesc:	m_sRtfName = L"mzeroDesc";		break;
		case OOX::et_m_zeroWid:		m_sRtfName = L"mzeroWid";		break;
		default:
		{
			m_sRtfName = L"";
			break;
		}
	}
}