#pragma once
#include "Basic.h"
#include "RtfProperty.h"


class RtfMath : public IDocumentElement, public ItemContainer<IDocumentElementPtr>
{
private: 
	CString				m_sRtfName;
	CString				m_sOOXName;
public: 
	bool				m_bHeader; //чтобы отличать заголовок от вложенных групп

	bool				m_bIsVal;
	bool				m_bIsBool;
	
	RtfMathSpecProp		m_oProperty;
	RtfParagraph		m_oVal;
	RtfShapePtr			m_oPicture;

	RtfMath( )
	{
		m_bHeader = true;
		m_bIsVal = m_bIsBool = false;
	}
	RtfMath(const RtfMath& oMath )
	{
		(*this) = oMath;
	}
	RtfMath& operator=(const RtfMath& oMath )
	{
		m_bHeader	= oMath.m_bHeader;
		m_bIsVal	= oMath.m_bIsVal;
		m_bIsBool	= oMath.m_bIsBool;

		m_sRtfName	= oMath.m_sRtfName;
		m_sOOXName	= oMath.m_sOOXName;
	
		m_aArray.clear();
		m_aArray	= oMath.m_aArray;

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
	static bool IsRtfControlPropertyBool( CString sControlW )
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
	static bool IsRtfControlProperty( CString sControlW )
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
	static bool IsRtfControlWord( CString sControlW )
	{
		const TCHAR* mc_aRtfControlWords[]		= { L"moMath", L"moMathPara", L"moMathParaPr", L"maccPr", L"macc", 
		L"me", L"mctrlPr", L"margPr", L"mbarPr", L"mbar", L"mborderBoxPr", L"mborderBox", L"mboxPr", L"mbox"/*, L"mbrk"*/, 
		L"mdPr", L"md", L"meqArrPr", L"meqArr", L"mfPr", L"mf", L"mfuncPr", L"mfunc", 
		L"mgroupChrPr", L"mgroupChr", L"mlimLowPr", L"mlimLow",L"mlimUppPr", L"mlimUpp", L"mmathPr", L"mmcPr", L"mmc", 
		L"mmcs", L"mmPr", L"mmr", L"mm", L"mnaryPr", L"mnary", L"mphantPr", L"mphant", L"mmRun", L"mmDel", 
		L"mmIns", L"mradPr", L"mrad", /*MRPr",*/ L"msPrePr", L"msPre", L"msSubPr", L"msSub", L"msSubSupPr", 
		L"msSubSup", L"msSupPr", L"msSup", L"msub", L"msup", L"mden", L"mlim", L"mnum", L"mdeg"/*mmText",*/, L"mfName" };

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

        sResult.AppendFormat(_T("{\\%ls"), m_sRtfName.GetBuffer());
		RenderParameter oNewParameter = oRenderParameter;

		oNewParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;
		sResult.Append( m_oProperty.RenderToRtf( oNewParameter ) );

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
            sResult.AppendFormat(_T("}"), m_sRtfName.GetBuffer());
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		CString sContent;
		CString sProp;
		CString sVal;

		RenderParameter oNewParam = oRenderParameter;
	
		oNewParam.nType = RENDER_TO_OOX_PARAM_TEXT;
		sProp			= m_oProperty.RenderToOOX( oNewParam );
			
		//oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
		//sVal			= m_oVal.RenderToOOX( oNewParam );

		if (m_bIsVal || m_bIsBool)
		{
			oNewParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
			for( int i = 0; i < (int)m_aArray.size(); i++ )
			{
				sVal += m_aArray[i]->RenderToOOX(oNewParam);
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
};

typedef boost::shared_ptr<RtfMath> RtfMathPtr;
