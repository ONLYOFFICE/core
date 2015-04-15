#pragma once
#include "Basic.h"
#include "RtfProperty.h"


class RtfMath : public IDocumentElement, public ItemContainer<IDocumentElementPtr>
{
private: 
	CString		m_sRtfName;
	CString		m_sOOXName;
public: 
	RtfMathSpecProp oProperty;
	RtfParagraph m_oVal;
	RtfShapePtr m_oPicture;

	RtfMath( )
	{
	}
	RtfMath(const RtfMath& oMath )
	{
		(*this) = oMath;
	}
	RtfMath& operator=(const RtfMath& oMath )
	{
		m_sRtfName = oMath.m_sRtfName;
		m_sOOXName = oMath.m_sOOXName;
		m_aArray.clear();
		m_aArray = oMath.m_aArray;
		//for( int i = 0; i < oMath.m_aArray.size(); i++ )
		//{
		//	RtfMath* oCopyMath = new RtfMath();
		//	(*oCopyMath) = (*oMath.m_aArray[i]);
		//	m_aArray.push_back( oCopyMath );
		//}
		return (*this);
	}
	bool IsEmpty()
	{
		return m_sRtfName.IsEmpty();
	}
	bool IsValid()
	{
		return (false == m_sRtfName.IsEmpty());
	}
	static bool IsRtfControlWord( CString sControlW )
	{
		const TCHAR* mc_aRtfControlWords[]		= {_T("macc"),_T("maccPr"),_T("maln"),_T("malnScr"),_T("margPr"),_T("margSz"),_T("mbar"),_T("mbarPr"),_T("mbaseJc"),_T("mbegChr"),_T("mborderBox"),_T("mborderBoxPr"),_T("mbox"),_T("mboxPr"),_T("mbrkN"),_T("mbrkBinN"),_T("mbrkBinSubN"),_T("mcGpN"),_T("mcGpRuleN"),_T("mchr"),_T("mcount"),_T("mcSpN"),_T("mctrlPr"),_T("md"),_T("mdefJcN"),_T("mdeg"),_T("mdegHide"),_T("mden"),_T("mdiff"),_T("mdiffStyN"),_T("mdispDefN"),_T("mdPr"),_T("me"),_T("mendChr"),_T("meqArr"),_T("meqArrPr"),_T("mf"),_T("mfName"),_T("mfPr"),_T("mfunc"),_T("mfuncPr"),_T("mgroupChr"),_T("mgroupChrPr"),_T("mgrow"),_T("mhideBot"),_T("mhideLeft"),_T("mhideRight"),_T("mhideTop"),_T("minterSpN"),_T("mintLimN"),_T("mintraSpN"),_T("mjcN"),_T("mlim"),_T("mlimLoc"),_T("mlimLow"),_T("mlimLowPr"),_T("mlimUpp"),_T("mlimUppPr"),_T("mlit"),_T("mlMarginN"),_T("mm"),_T("mmath"),_T("mmathFontN"),_T("mmathPict"),_T("mmathPr"),_T("mmaxDist"),_T("mmc"),_T("mmcJc"),_T("mmcPr"),_T("mmcs"),_T("mmPr"),_T("mmr"),_T("mnary"),_T("mnaryLimN"),_T("mnaryPr"),_T("mnoBreak"),_T("mnor"),_T("mnum"),_T("mobjDist"),_T("moMath"),_T("moMathPara"),_T("moMathParaPr"),_T("mopEmu"),_T("mphant"),_T("mphantPr"),_T("mplcHide"),_T("mpos"),_T("mpostSpN"),_T("mpreSpN"),_T("mr"),_T("mrad"),_T("mradPr"),_T("mrMarginN"),_T("mrPr"),_T("mrSpN"),_T("mrSpRuleN"),_T("mscrN"),_T("msepChr"),_T("mshow"),_T("mshp"),_T("msmallFracN"),_T("msPre"),_T("msPrePr"),_T("msSub"),_T("msSubPr"),_T("msSubSup"),_T("msSubSupPr"),_T("msSup"),_T("msSupPr"),_T("mstrikeBLTR"),_T("mstrikeH"),_T("mstrikeTLBR"),_T("mstrikeV"),_T("mstyN"),_T("msub"),_T("msubHide"),_T("msup"),_T("msupHide"),_T("mtransp"),_T("mtype"),_T("mvertJc"),_T("mwrapIndentN"),_T("mwrapRightN"),_T("mzeroAsc"),_T("mzeroDesc"),_T("mzeroWid")};
		int mc_nRtfControlWordsSize	=  sizeof( mc_aRtfControlWords ) / sizeof( TCHAR* );
		
		//можно бинарный поиск вставить
		for( int i = 0; i < mc_nRtfControlWordsSize; i++ )
			if( mc_aRtfControlWords[ i ] == sControlW )
				return true;
		return false;
	}
	static bool IsOOXControlWord( CString sControlW )
	{
		const TCHAR* mc_aOOXControlWords[]		= {_T("m:acc"),_T("m:accPr"),_T("m:aln"),_T("m:alnScr"),_T("m:argPr"),_T("m:argSz"),_T("m:bar"),_T("m:barPr"),_T("m:baseJc"),_T("m:begChr"),_T("m:borderBox"),_T("m:borderBoxPr"),_T("m:box"),_T("m:boxPr"),_T("m:brkN"),_T("m:brkBinN"),_T("m:brkBinSubN"),_T("m:cGpN"),_T("m:cGpRuleN"),_T("m:chr"),_T("m:count"),_T("m:cSpN"),_T("m:ctrlPr"),_T("m:d"),_T("m:defJcN"),_T("m:deg"),_T("m:degHide"),_T("m:den"),_T("m:diff"),_T("m:diffStyN"),_T("m:dispDefN"),_T("m:dPr"),_T("m:e"),_T("m:endChr"),_T("m:eqArr"),_T("m:eqArrPr"),_T("m:f"),_T("m:fName"),_T("m:fPr"),_T("m:func"),_T("m:funcPr"),_T("m:groupChr"),_T("m:groupChrPr"),_T("m:grow"),_T("m:hideBot"),_T("m:hideLeft"),_T("m:hideRight"),_T("m:hideTop"),_T("m:interSpN"),_T("m:intLimN"),_T("m:intraSpN"),_T("m:jcN"),_T("m:lim"),_T("m:limLoc"),_T("m:limLow"),_T("m:limLowPr"),_T("m:limUpp"),_T("m:limUppPr"),_T("m:lit"),_T("m:lMarginN"),_T("m:m"),_T("m:math"),_T("m:mathFontN"),_T("m:mathPict"),_T("m:mathPr"),_T("m:maxDist"),_T("m:mc"),_T("m:mcJc"),_T("m:mcPr"),_T("m:mcs"),_T("m:mPr"),_T("m:mr"),_T("m:nary"),_T("m:naryLimN"),_T("m:naryPr"),_T("m:noBreak"),_T("m:nor"),_T("m:num"),_T("m:objDist"),_T("m:oMath"),_T("m:oMathPara"),_T("m:oMathParaPr"),_T("m:opEmu"),_T("m:phant"),_T("m:phantPr"),_T("m:plcHide"),_T("m:pos"),_T("m:postSpN"),_T("m:preSpN"),_T("m:r"),_T("m:rad"),_T("m:radPr"),_T("m:rMarginN"),_T("m:rPr"),_T("m:rSpN"),_T("m:rSpRuleN"),_T("m:scrN"),_T("m:sepChr"),_T("m:show"),_T("m:shp"),_T("m:smallFracN"),_T("m:sPre"),_T("m:sPrePr"),_T("m:sSub"),_T("m:sSubPr"),_T("m:sSubSup"),_T("m:sSubSupPr"),_T("m:sSup"),_T("m:sSupPr"),_T("m:strikeBLTR"),_T("m:strikeH"),_T("m:strikeTLBR"),_T("m:strikeV"),_T("m:styN"),_T("m:sub"),_T("m:subHide"),_T("m:sup"),_T("m:supHide"),_T("m:transp"),_T("m:type"),_T("m:vertJc"),_T("m:wrapIndentN"),_T("m:wrapRightN"),_T("m:zeroAsc"),_T("m:zeroDesc"),_T("m:zeroWid")};
		int mc_nOOXControlWordsSize	=  sizeof( mc_aOOXControlWords ) / sizeof( TCHAR* );
		
		//можно бинарный поиск вставить
		for( int i = 0; i < mc_nOOXControlWordsSize; i++ )
			if( mc_aOOXControlWords[ i ] == sControlW )
				return true;
		return false;
	}
	void SetRtfName(CString sName)
	{
		m_sRtfName = sName;
		m_sOOXName = sName;
		m_sOOXName.Insert(1, _T(":"));
	}
	void SetOOXName(CString sName)
	{
		m_sOOXName = sName;
		m_sRtfName = sName;
		m_sRtfName.Remove(':');
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
			sResult.Append(_T("{\\mmath"));

		sResult.AppendFormat(_T("{\\%ls"), m_sRtfName);
		RenderParameter oNewParameter = oRenderParameter;

		oNewParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;
		sResult.Append( oProperty.RenderToRtf( oNewParameter ) );

		//if( _T("mr") == m_sRtfName )
		//{
		//	oNewParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;
		//	sResult.Append( _T(" ") );
		//	for( int i = 0; i < m_oVal.size(); i++ )
		//	sResult.Append( _T(" ") + m_oVal[i]->RenderToRtf( oNewParameter ) );
		//}
		//else
		{
			oNewParameter.nType = RENDER_TO_RTF_PARAM_CHAR;
			sResult.Append( _T(" ") + m_oVal.RenderToRtf( oNewParameter ) );
		}
		
		oNewParameter.nType = RENDER_TO_RTF_PARAM_NESTED;
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			sResult.Append(m_aArray[i]->RenderToRtf( oNewParameter ));
		sResult.Append(_T("}"));

		if( RENDER_TO_RTF_PARAM_NESTED != oRenderParameter.nType )
			sResult.AppendFormat(_T("}"), m_sRtfName);
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
	#ifdef RTF_MATH_OOX
		RenderParameter oNewParam = oRenderParameter;
		sResult.AppendFormat( _T("<%ls"), m_sOOXName );
		oNewParam.nType = RENDER_TO_OOX_PARAM_MATH;
		CString sVal = m_oVal.RenderToOOX( oNewParam );
		if( false == sVal.IsEmpty() )
			sResult.AppendFormat( _T(" w:val=\"%ls\""), sVal );
		sResult.Append( _T(">") );

		oNewParam.nType = RENDER_TO_OOX_PARAM_TEXT;
		sResult.Append( oProperty.RenderToOOX( oNewParam ) );
		for( int i = 0; i < (int)m_aArray.size(); i++ )
			sResult.Append(m_aArray[i]->RenderToOOX(oNewParam));

		sResult.AppendFormat(_T("</%ls>"), m_sOOXName);

	#else

		if( NULL != m_oPicture )
			sResult.Append( m_oPicture->RenderToOOX(oRenderParameter) );

	#endif
		return sResult;
	}
};

typedef boost::shared_ptr<RtfMath> RtfMathPtr;