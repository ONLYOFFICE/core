#include "stdafx.h"
#include "RtfSection.h"
#include "OOXWriter.h"
#include "IdGenerator.h"

CString RtfDocumentProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	switch( m_eCodePage )
	{
		case cp_ansi: 
			{
				sResult.Append( _T("\\ansi") );
				if( PROP_DEF != m_nAnsiCodePage )
					sResult.AppendFormat( _T("\\ansicpg%d"), m_nAnsiCodePage );

				break;
			}
		case cp_mac: sResult.Append( _T("\\mac") );break;
		case cp_pc: sResult.Append( _T("\\pc") );break;
		case cp_pca: sResult.Append( _T("\\pca") );break;
	}
	RENDER_RTF_BOOL( m_bHypCaps, sResult, _T("hyphcaps") )
	RENDER_RTF_BOOL( m_bAutoHyp, sResult, _T("hyphauto") )
	RENDER_RTF_INT( m_nMaxHypen, sResult, _T("hyphconsec") )
	RENDER_RTF_INT( m_nHyphenationRight, sResult, _T("hyphhotz") )
	RENDER_RTF_INT( m_nTabWidth, sResult, _T("deftab") )
	RENDER_RTF_INT( m_nZoom, sResult, _T("viewscale") )

//Page Borders
	RENDER_RTF_BOOL( m_bDorderSurroundHeader, sResult, _T("pgbrdrhead") )
	RENDER_RTF_BOOL( m_bDorderSurroundFotter, sResult, _T("pgbrdrfoot") )

	RENDER_RTF_BOOL( m_bAlignBordersAndEdges, sResult, _T("pgbrdrsnap") )

//Page Information
//эти свойства пишутся в первой секции
//public: int m_nPaperWidth; //\paperwN	Paper width in twips (default is 12,240).
//public: int m_nPaperHeight; //\paperhN	Paper height in twips (default is 15,840).
//public: int m_nMarginLeft; //\marglN	Left margin in twips (default is 1800).
//public: int m_nMarginRight; //\margrN	Right margin in twips (default is 1800).
//public: int m_nMarginTop; //\margtN	Top margin in twips (default is 1440).
//public: int m_nMarginBottom; //\margbN	Bottom margin in twips (default is 1440).
//public: int m_bFacingPage; //(mirrorMargins)\facingp	Facing pages (activates odd/even headers and gutters).
//public: int m_nGutterWidth; //\gutterN	Gutter width in twips (default is 0).
//public: int m_nGutterWidthOutside; //\ogutterN	Outside gutter width (default is 0; not used by Word, but in 1987 RTF Spec)
//	RENDER_RTF_BOOL( m_bLandScape, sResult, _T("landscape") )

	RENDER_RTF_BOOL( m_bGutterAtTop, sResult, _T("gutterprl") )
	RENDER_RTF_BOOL( m_bSwitchMargins, sResult, _T("margmirror") )
	RENDER_RTF_BOOL( m_bFacingPage, sResult, _T("facingp") )

//Compatibility Options
	RENDER_RTF_INT( m_nDisplayBackground, sResult, _T("viewbksp") )
	RENDER_RTF_BOOL( m_bUseTabAlignment, sResult, _T("useltbaln") )

	//footnote, endnote property
	if( PROP_DEF != m_nFootnoteNumberingFormat )
		sResult.Append( _T("\\") + GetRtfFormat(m_nFootnoteNumberingFormat, true) );
	RENDER_RTF_INT( m_nFootnoteStart, sResult, _T("ftnstart") )
	if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage: sResult.Append( _T("ftnrstpg") ); break;
		case fr_EachSection: sResult.Append( _T("ftnrestart") ); break;
		case fr_Continuous: sResult.Append( _T("ftnrstcont") ); break;
		}
	}
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnoteRestart )
		{
		case fp_EndSection: sResult.Append( _T("endnotes") ); break;
		case fp_EndDocument: sResult.Append( _T("enddoc") ); break;
		case fp_BeneathText: sResult.Append( _T("ftntj") ); break;
		case fp_BottomPage: sResult.Append( _T("ftnbj") ); break;
		}
	}
	if( PROP_DEF != m_nEndnoteNumberingFormat )
		sResult.Append( _T("\\") + GetRtfFormat(m_nEndnoteNumberingFormat, false) );
	RENDER_RTF_INT( m_nEndnoteStart, sResult, _T("aftnstart") )
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case er_EachSection: sResult.Append( _T("aftnrestart") ); break;
		case er_Continuous: sResult.Append( _T("aftnrstcont") ); break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case ep_EndSection: sResult.Append( _T("aendnotes") ); break;
		case ep_EndDocument: sResult.Append( _T("aenddoc") ); break;
		case ep_BeneathText: sResult.Append( _T("aftntj") ); break;
		case ep_BottomPage: sResult.Append( _T("aftnbj") ); break;
		}
	}

//для схоранения в RTF
	RENDER_RTF_INT( m_bSplitPageBrake, sResult, _T("spltpgpar") )

	if( 0 != m_bHtmlAutoSpace )
		sResult.Append( _T("\\htmautsp") );

	return sResult;
}
CString RtfDocumentProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_bHypCaps )
	{
		if( 0 == m_bHypCaps )
			sResult.Append( _T("<w:doNotHyphenateCaps w:val=\"true\"/>") );
		else
			sResult.Append( _T("<w:doNotHyphenateCaps w:val=\"false\"/>") );
	}
	RENDER_OOX_BOOL( m_bAutoHyp, sResult, _T("w:autoHyphenation") )
	RENDER_OOX_INT( m_nMaxHypen, sResult, _T("w:consecutiveHyphenLimit") )
	RENDER_OOX_INT( m_nHyphenationRight, sResult, _T("w:hyphenationZone") )
	RENDER_OOX_INT( m_nTabWidth, sResult, _T("w:defaultTabStop") )
	if( PROP_DEF != m_nZoom )
		sResult.AppendFormat( _T("<w:zoom w:percent=\"%d\" />"), m_nZoom );
//Page Borders
	if( PROP_DEF != m_bDorderSurroundHeader )
	{
		if( 0 == m_bDorderSurroundHeader )
			sResult.Append( _T("<w:bordersDoNotSurroundHeader w:val=\"true\"/>") );
		else
			sResult.Append( _T("<w:bordersDoNotSurroundHeader w:val=\"false\"/>") );
	}
	if( PROP_DEF != m_bDorderSurroundFotter )
	{
		if( 0 == m_bDorderSurroundFotter )
			sResult.Append( _T("<w:bordersDoNotSurroundFooter w:val=\"true\"/>") );
		else
			sResult.Append( _T("<w:bordersDoNotSurroundFooter w:val=\"false\"/>") );
	}
	RENDER_OOX_BOOL( m_bAlignBordersAndEdges, sResult, _T("w:alignBordersAndEdges") )

//Page Information
	RENDER_OOX_BOOL( m_bGutterAtTop, sResult, _T("w:gutterAtTop") )
	RENDER_OOX_BOOL( m_bSwitchMargins, sResult, _T("w:mirrorMargins") );
	RENDER_OOX_BOOL( m_bFacingPage, sResult, _T("w:evenAndOddHeaders") );

//Compatibility Options
	RENDER_OOX_BOOL( m_nDisplayBackground, sResult, _T("w:displayBackgroundShape") );

//footnote, endnote property
	CString sFootnoteProp;
	for( int i = 0; i < (int)m_aSpecialFootnotes.GetCount(); i++ )
		sFootnoteProp.AppendFormat( _T("<w:footnote w:id=\"%d\" />"), m_aSpecialFootnotes[i] );
	if( PROP_DEF != m_nFootnoteNumberingFormat )
		sFootnoteProp.AppendFormat( _T("<w:numRestart w:val=\"%s\" />"), RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat ) );
	RENDER_OOX_INT( m_nFootnoteStart, sFootnoteProp, _T("w:numStart") );
	if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage: sFootnoteProp.Append( _T("<w:numRestart w:val=\"eachPage\" />") ); break;
		case fr_EachSection: sFootnoteProp.Append( _T("<w:numRestart w:val=\"eachSect\" />") ); break;
		case fr_Continuous: sFootnoteProp.Append( _T("<w:numRestart w:val=\"continuous\" />") ); break;
		}
	}
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnotePlacement )
		{
		case fp_EndSection: sFootnoteProp.Append( _T("<w:pos w:val=\"sectEnd\" />") ); break;
		case fp_EndDocument: sFootnoteProp.Append( _T("<w:pos w:val=\"docEnd\" />") ); break;
		case fp_BeneathText: sFootnoteProp.Append( _T("<w:pos w:val=\"beneathText\" />") ); break;
		case fp_BottomPage: sFootnoteProp.Append( _T("<w:pos w:val=\"pageBottom\" />") ); break;
		}
	}
	if( false == sFootnoteProp.IsEmpty() )
		sResult.AppendFormat( _T("<w:footnotePr>%s</w:footnotePr>"), sFootnoteProp );

	CString sEndnoteProp;
	for( int i = 0; i < (int)m_aSpecialEndnotes.GetCount(); i++ )
		sEndnoteProp.AppendFormat( _T("<w:endnote w:id=\"%d\" />"), m_aSpecialEndnotes[i] );
	if( PROP_DEF != m_nFootnoteNumberingFormat )
		sFootnoteProp.AppendFormat( _T("<w:numRestart w:val=\"%s\" />"), RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat ) );
	RENDER_OOX_INT( m_nEndnoteStart, sEndnoteProp, _T("w:numStart") );
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case fr_EachSection: sEndnoteProp.Append( _T("<w:numRestart w:val=\"eachSect\" />") ); break;
		case fr_Continuous: sEndnoteProp.Append( _T("<w:numRestart w:val=\"continuous\" />") ); break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case fp_EndSection: sEndnoteProp.Append( _T("<w:pos w:val=\"sectEnd\" />") ); break;
		case fp_EndDocument: sEndnoteProp.Append( _T("<w:pos w:val=\"docEnd\" />") ); break;
		case fp_BeneathText: sEndnoteProp.Append( _T("<w:pos w:val=\"beneathText\" />") ); break;
		case fp_BottomPage: sEndnoteProp.Append( _T("<w:pos w:val=\"pageBottom\" />") ); break;
		}
	}
	if( false == sEndnoteProp.IsEmpty() )
		sResult.AppendFormat( _T("<w:endnotePr>%s</w:endnotePr>"), sEndnoteProp );

	CString sCompat;
	if( 1 != m_bHtmlAutoSpace )
		sCompat.Append( _T("<w:doNotUseHTMLParagraphAutoSpacing/>") );
	if( 0 == m_bUseTabAlignment )
		sCompat.Append( _T("<w:forgetLastTabAlignment/>") );
	if( false == sCompat.IsEmpty() )
		sResult.AppendFormat( _T("<w:compat>%s</w:compat>"), sCompat );

	return sResult;
}
CString RtfSectionProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;

	if( RENDER_TO_OOX_PARAM_FIRST_SECTION == oRenderParameter.nType ) //Пишем как свойства документа
	{
		//Page Information
		RENDER_RTF_INT( m_nPageWidth, sResult, _T("paperw") )
		RENDER_RTF_INT( m_nPageHeight, sResult, _T("paperh") )
		RENDER_RTF_INT( m_nMarginLeft, sResult, _T("margl") )
		RENDER_RTF_INT( m_nMarginRight, sResult, _T("margr") )
		RENDER_RTF_INT( m_nMarginTop, sResult, _T("margt") )
		RENDER_RTF_INT( m_nMarginBottom, sResult, _T("margb") )
		RENDER_RTF_INT( m_nGutterMarginWidth, sResult, _T("gutter") )
		RENDER_RTF_BOOL( m_bSwitchMargin, sResult, _T("margmirror") )
		RENDER_RTF_BOOL( m_bLandscapeFormat, sResult, _T("landscape") )
	}

	sResult.Append( _T("\\sectd") );
	RENDER_RTF_BOOL( m_bBidi, sResult, _T("endnhere") )
	RENDER_RTF_INT( m_nPaperSourceFirst, sResult, _T("binfsxn") )
	RENDER_RTF_INT( m_nPaperSourceOther, sResult, _T("binsxn") )
	RENDER_RTF_BOOL( m_bRtlGutter, sResult, _T("rtlgutter") )
	RENDER_RTF_BOOL( m_bEndnotes, sResult, _T("endnhere") )
	RENDER_RTF_INT( m_nStyle, sResult, _T("ds") )
	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbknone: sResult.Append( _T("\\sbknone") );break;
		case sb_sbkcol: sResult.Append( _T("\\sbkcol") );break;
		case sb_sbkpage: sResult.Append( _T("\\sbkpage") );break;
		case sb_sbkeven: sResult.Append( _T("\\sbkeven") );break;
		case sb_sbkodd: sResult.Append( _T("\\sbkodd") );break;
	}
	//Columns
	RENDER_RTF_INT( m_nColumnNumber, sResult, _T("cols") )
	RENDER_RTF_INT( m_nColumnSpace, sResult, _T("colsx") )
	RENDER_RTF_BOOL( m_bColumnLineBetween, sResult, _T("linebetcol") )
	int nCollFormCount = (int)m_oCollumnProperty.m_aCollumnProperty.GetCount();
	if( nCollFormCount > 0 )
	{
		for( int i = 0; i < nCollFormCount; i++ )
		{
			sResult.AppendFormat( _T("\\colno%d"), i + 1 );
			RENDER_RTF_INT( m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol, sResult, _T("colsr") )
			RENDER_RTF_INT( m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth, sResult, _T("colw") )
		}
	}

	//Footnotes and Endnotes
	switch( m_eFootnotesJust )
	{
		case fj_sftntj: sResult.Append( _T("\\sftntj") );break;
		case fj_sftnbj: sResult.Append( _T("\\sftnbj") );break;
	}
	RENDER_RTF_INT( m_nFootnotesStart, sResult, _T("sftnstart") )
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg: sResult.Append( _T("\\sftnrstpg") );break;
		case fr_sftnrestart: sResult.Append( _T("\\sftnrestart") );break;
		case fr_sftnrstcont: sResult.Append( _T("\\sftnrstcont") );break;
	}
	switch( m_eFootnotesFormat )
	{
		case ff_sftnnar: sResult.Append( _T("\\sftnnar") );break;
		case ff_sftnnalc: sResult.Append( _T("\\sftnnalc") );break;
		case ff_sftnnauc: sResult.Append( _T("\\sftnnauc") );break;
		case ff_sftnnrlc: sResult.Append( _T("\\sftnnrlc") );break;
		case ff_sftnnruc: sResult.Append( _T("\\sftnnruc") );break;
		case ff_sftnnchi: sResult.Append( _T("\\sftnnchi") );break;
		case ff_sftnnchosung: sResult.Append( _T("\\sftnnchosung") );break;
		case ff_sftnncnum: sResult.Append( _T("\\sftnncnum") );break;
		case ff_sftnndbnum: sResult.Append( _T("\\sftnndbnum") );break;
		case ff_sftnndbnumd: sResult.Append( _T("\\sftnndbnumd") );break;
		case ff_sftnndbnumt: sResult.Append( _T("\\sftnndbnumt") );break;
		case ff_sftnndbnumk: sResult.Append( _T("\\sftnndbnumk") );break;
		case ff_sftnndbar: sResult.Append( _T("\\sftnndbar") );break;
		case ff_sftnnganada: sResult.Append( _T("\\sftnnganada") );break;
		case ff_sftnngbnum: sResult.Append( _T("\\sftnngbnum") );break;
		case ff_sftnngbnumd: sResult.Append( _T("\\sftnngbnumd") );break;
		case ff_sftnngbnuml: sResult.Append( _T("\\sftnngbnuml") );break;
		case ff_sftnngbnumk: sResult.Append( _T("\\sftnngbnumk") );break;
		case ff_sftnnzodiac: sResult.Append( _T("\\sftnnzodiac") );break;
		case ff_sftnnzodiacd: sResult.Append( _T("\\sftnnzodiacd") );break;
		case ff_sftnnzodiacl: sResult.Append( _T("\\sftnnzodiacl") );break;
	}
	RENDER_RTF_INT( m_nEndnotesStart, sResult, _T("saftnstart") )
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart: sResult.Append( _T("\\saftnrestart") );break;
		case er_saftnrstcont: sResult.Append( _T("\\saftnrstcont") );break;
	}
	switch( m_eEndnotesFormat )
	{
		case ef_saftnnar: sResult.Append( _T("\\saftnnar") );break;
		case ef_saftnnalc: sResult.Append( _T("\\saftnnalc") );break;
		case ef_saftnnauc: sResult.Append( _T("\\saftnnauc") );break;
		case ef_saftnnrlc: sResult.Append( _T("\\saftnnrlc") );break;
		case ef_saftnnruc: sResult.Append( _T("\\saftnnruc") );break;
		case ef_saftnnchi: sResult.Append( _T("\\saftnnchi") );break;
		case ef_saftnnchosung: sResult.Append( _T("\\saftnnchosung") );break;
		case ef_saftnncnum: sResult.Append( _T("\\saftnncnum") );break;
		case ef_saftnndbnum: sResult.Append( _T("\\saftnndbnum") );break;
		case ef_saftnndbnumd: sResult.Append( _T("\\saftnndbnumd") );break;
		case ef_saftnndbnumt: sResult.Append( _T("\\saftnndbnumt") );break;
		case ef_saftnndbnumk: sResult.Append( _T("\\saftnndbnumk") );break;
		case ef_saftnndbar: sResult.Append( _T("\\saftnndbar") );break;
		case ef_saftnnganada: sResult.Append( _T("\\saftnnganada") );break;
		case ef_saftnngbnum: sResult.Append( _T("\\saftnngbnum") );break;
		case ef_saftnngbnumd: sResult.Append( _T("\\saftnngbnumd") );break;
		case ef_saftnngbnuml: sResult.Append( _T("\\saftnngbnuml") );break;
		case ef_saftnngbnumk: sResult.Append( _T("\\saftnngbnumk") );break;
		case ef_saftnnzodiac: sResult.Append( _T("\\saftnnzodiac") );break;
		case ef_saftnnzodiacd: sResult.Append( _T("\\saftnnzodiacd") );break;
		case ef_saftnnzodiacl: sResult.Append( _T("\\saftnnzodiacl") );break;
	}

	//Line Numbering
	RENDER_RTF_INT( m_nLineModulus, sResult, _T("linemod") )
	RENDER_RTF_INT( m_nLineX, sResult, _T("linex") )
	RENDER_RTF_INT( m_nLineStart, sResult, _T("linestarts") )
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart: sResult.Append( _T("\\linerestart") );break;
		case lnr_lineppage: sResult.Append( _T("\\lineppage") );break;
		case lnr_linecont: sResult.Append( _T("\\linecont") );break;
	}

	if( RENDER_TO_OOX_PARAM_FIRST_SECTION != oRenderParameter.nType )//Пишем как свойства секции
	{
		//Page Information
		RENDER_RTF_INT( m_nPageWidth, sResult, _T("pgwsxn") )
		RENDER_RTF_INT( m_nPageHeight, sResult, _T("pghsxn") )
		RENDER_RTF_INT( m_nMarginLeft, sResult, _T("marglsxn") )
		RENDER_RTF_INT( m_nMarginRight, sResult, _T("margrsxn") )
		RENDER_RTF_INT( m_nMarginTop, sResult, _T("margtsxn") )
		RENDER_RTF_INT( m_nMarginBottom, sResult, _T("margbsxn") )
		RENDER_RTF_INT( m_nGutterMarginWidth, sResult, _T("guttersxn") )
		RENDER_RTF_BOOL( m_bSwitchMargin, sResult, _T("margmirsxn") )
		RENDER_RTF_BOOL( m_bLandscapeFormat, sResult, _T("lndscpsxn") )
	}

	RENDER_RTF_BOOL( m_bTitlePage, sResult, _T("titlepg") )
	RENDER_RTF_INT( m_nHeaderTop, sResult, _T("headery") )
	RENDER_RTF_INT( m_nFotterBottom, sResult, _T("footery") )

	//Page Numbers
	RENDER_RTF_INT( m_nPageNumberStart, sResult, _T("pgnstarts") )
	RENDER_RTF_BOOL( m_bPageNumberContinuos, sResult, _T("pgncont") )
	RENDER_RTF_BOOL( m_bPageNumberRestart, sResult, _T("pgnrestart") )
	RENDER_RTF_INT( m_nPageNumberX, sResult, _T("pgnx") )
	RENDER_RTF_INT( m_nPageNumberY, sResult, _T("pgny") )
	switch( m_ePageNumberFormat )
	{
		case pnf_pgndec: sResult.Append( _T("\\pgndec") );break;
		case pnf_pgnucrm: sResult.Append( _T("\\pgnucrm") );break;
		case pnf_pgnlcrm: sResult.Append( _T("\\pgnlcrm") );break;
	}
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt: sResult.Append( _T("\\vertalt") );break;
		case va_vertalb: sResult.Append( _T("\\vertalb") );break;
		case va_vertalc: sResult.Append( _T("\\vertalc") );break;
		case va_vertalj: sResult.Append( _T("\\vertalj") );break;

	}
	//Text Flow
	RENDER_RTF_INT( m_nTextFollow, sResult, _T("stextflow") )

	//Page Borders
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult.Append( _T("\\pgbrdrl") );
		sResult.Append( m_oBorderLeft.RenderToRtf( oRenderParameter ) );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		sResult.Append( _T("\\pgbrdrt") );
		sResult.Append( m_oBorderTop.RenderToRtf( oRenderParameter ) );
	}
		
	if( true == m_oBorderRight.IsValid() )
	{
		sResult.Append( _T("\\pgbrdrr") );
		sResult.Append( m_oBorderRight.RenderToRtf( oRenderParameter ) );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult.Append( _T("\\pgbrdrb") );
		sResult.Append( m_oBorderBottom.RenderToRtf( oRenderParameter ) );
	}
	RENDER_RTF_INT( m_nBorderArt, sResult, _T("brdrart") )
	RENDER_RTF_INT( m_nBorderMeasure, sResult, _T("pgbrdropt") )
	RENDER_RTF_INT( m_nBorderAlign, sResult, _T("pgbrdrsnap") )

	//footer - header
	if( 0 != m_oHeaderRight )
	{
		sResult.Append( _T("{\\headerr") );
		sResult.Append( m_oHeaderRight->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	if( 0 != m_oHeaderLeft )
	{
		sResult.Append( _T("{\\headerl") );
		sResult.Append( m_oHeaderLeft->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	if( 0 != m_oHeaderFirst )
	{
		sResult.Append( _T("{\\headerf") );
		sResult.Append( m_oHeaderFirst->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	if( 0 != m_oFooterRight )
	{
		sResult.Append( _T("{\\footerr") );
		sResult.Append( m_oFooterRight->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	if( 0 != m_oFooterLeft )
	{
		sResult.Append( _T("{\\footerl") );
		sResult.Append( m_oFooterLeft->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	if( 0 != m_oFooterFirst )
	{
		sResult.Append( _T("{\\footerf") );
		sResult.Append( m_oFooterFirst->RenderToRtf(  oRenderParameter  ));
		sResult.Append( _T("\\par") );
		sResult.Append( _T("}") );
	}
	return sResult;
}
CString RtfSectionProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult.Append(_T("<w:sectPr>"));
	if( 1 == m_bBidi )
		sResult.Append(_T("<w:bidi/>"));
	CString sPaperSource;
	if( PROP_DEF != m_nPaperSourceFirst )
		sPaperSource.AppendFormat( _T(" w:first=\"%d\""), m_nPaperSourceFirst );
	if( PROP_DEF != m_nPaperSourceFirst )
		sPaperSource.AppendFormat( _T(" w:other=\"%d\""), m_nPaperSourceOther );
	if( false == sPaperSource.IsEmpty() )
		sResult.AppendFormat(_T("<w:paperSrc %s/>"), sPaperSource );
	if( 1 == m_bRtlGutter )
		sResult.Append(_T("<w:rtlGutter/>"));

	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbknone: sResult.Append(_T("<w:type w:val=\"continuous\"/>") );break;
		case sb_sbkcol: sResult.Append(_T("<w:type w:val=\"nextColumn\"/>") );break;
		case sb_sbkpage: sResult.Append(_T("<w:type w:val=\"nextPage\"/>") );break;
		case sb_sbkeven: sResult.Append(_T("<w:type w:val=\"evenPage\"/>") );break;
		case sb_sbkodd: sResult.Append(_T("<w:type w:val=\"oddPage\"/>") );break;
	}
	//Columns
	if( PROP_DEF != m_nColumnNumber )
	{
		CString sCollumnFormating;
		bool bEqualWidth = false;
		if( m_nColumnNumber != m_oCollumnProperty.m_aCollumnProperty.GetCount() )
			bEqualWidth = true;
		else
			for( int i = 0; i < (int)m_oCollumnProperty.m_aCollumnProperty.GetCount(); i++ )
				if( PROP_DEF == m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
				{
					bEqualWidth = true;
					break;
				}
		sCollumnFormating.AppendFormat( _T("<w:cols w:num=\"%d\""), m_nColumnNumber );
		if( PROP_DEF != m_nColumnSpace )
			sCollumnFormating.AppendFormat( _T(" w:space=\"%d\""), m_nColumnSpace );
		if( true == bEqualWidth )
			sCollumnFormating.Append( _T(" w:equalWidth=\"true\"/>") );
		else
		{
			sCollumnFormating.Append( _T(" w:equalWidth=\"false\">") );
			for( int i = 0; i < (int)m_oCollumnProperty.m_aCollumnProperty.GetCount(); i++ )
			{
				sCollumnFormating.Append( _T("<w:col") );
				if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
					sCollumnFormating.AppendFormat( _T(" w:w=\"%d\""), m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth );
				if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol )
					sCollumnFormating.AppendFormat( _T(" w:space=\"%d\""), m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol );
				sCollumnFormating.Append( _T("/>") );
			}
			sCollumnFormating.Append( _T("</w:cols>") );
		}
		if( false == sCollumnFormating.IsEmpty() )
			sResult.Append( sCollumnFormating );
	}

	//Footnotes and Endnotes
	CString sFootnotesProp;
	switch( m_eFootnotesJust )
	{
		case fj_sftntj:sFootnotesProp.Append( _T("<w:pos w:val=\"beneathText\"/>") );break; 
		case fj_sftnbj:sFootnotesProp.Append( _T("<w:pos w:val=\"pageBottom\"/>") );break; 
	}
	RENDER_OOX_INT( m_nFootnotesStart, sFootnotesProp, _T("w:numStart") )
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg:sFootnotesProp.Append( _T("<w:numRestart w:val=\"eachPage\"/>") );break; 
		case fr_sftnrestart:sFootnotesProp.Append( _T("<w:numRestart w:val=\"eachSect\"/>") );break; 
		case fr_sftnrstcont:sFootnotesProp.Append( _T("<w:numRestart w:val=\"continuous\"/>") );break; 
	}
	switch( m_eFootnotesRestart )
	{
		case ff_sftnnar:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnalc:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnauc:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnrlc:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnruc:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnchi:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnchosung:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnncnum:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnndbnum:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnndbnumd:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnndbnumt:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnndbnumk:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnndbar:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnganada:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnngbnum:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnngbnumd:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnngbnuml:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnngbnumk:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnzodiac:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnzodiacd:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ff_sftnnzodiacl:sFootnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
	}
	if( false == sFootnotesProp.IsEmpty() )
		sResult.AppendFormat( _T("<w:footnotePr>%s</w:footnotePr>"), sFootnotesProp );

	CString sEndnotesProp;
	RENDER_OOX_INT( m_nEndnotesStart, sEndnotesProp, _T("w:numStart") )
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart:sEndnotesProp.Append( _T("<w:numRestart w:val=\"eachPage\"/>") );break; 
		case er_saftnrstcont:sEndnotesProp.Append( _T("<w:numRestart w:val=\"continuous\"/>") );break; 
	}
	switch( m_eEndnotesRestart )
	{
		case ef_saftnnar:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnalc:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnauc:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnrlc:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnruc:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnchi:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnchosung:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnncnum:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnndbnum:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnndbnumd:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnndbnumt:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnndbnumk:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnndbar:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnganada:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnngbnum:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnngbnumd:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnngbnuml:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnngbnumk:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnzodiac:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnzodiacd:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
		case ef_saftnnzodiacl:sEndnotesProp.Append( _T("<w:numFmt w:val=\"upperLetter\" />") );break; 
	}
	if( false == sEndnotesProp.IsEmpty() )
		sResult.AppendFormat( _T("<w:endnotePr>%s</w:endnotePr>"), sEndnotesProp );

	//Line Numbering
	CString sLineNumbering;
	if( PROP_DEF != m_nLineModulus )
		sLineNumbering.AppendFormat( _T(" w:countBy=\"%d\""), m_nLineModulus );
	if( PROP_DEF != m_nLineX )
		sLineNumbering.AppendFormat( _T(" w:distance=\"%d\""), m_nLineX );
	if( PROP_DEF != m_nLineStart )
		sLineNumbering.AppendFormat( _T(" w:start=\"%d\""), m_nLineStart );
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart:sLineNumbering.Append( _T(" w:restart=\"newSection\"") );break; 
		case lnr_lineppage:sLineNumbering.Append( _T(" w:restart=\"newPage\"") );break; 
		case lnr_linecont:sLineNumbering.Append( _T(" w:restart=\"continuous\"") );break; 
	}
	if( false == sLineNumbering.IsEmpty() )
		sResult.AppendFormat( _T("<w:lnNumType %s/>"), sLineNumbering );

	//Page Information
	CString sPageSize;
	if( PROP_DEF != m_nPageWidth )
		sPageSize.AppendFormat(_T(" w:w=\"%d\""),m_nPageWidth);
	if( PROP_DEF != m_nPageHeight )
		sPageSize.AppendFormat(_T(" w:h=\"%d\""),m_nPageHeight);
	if( 1 == m_bLandscapeFormat )
		sPageSize.Append(_T(" w:orient=\"landscape\""));
	if( false == sPageSize.IsEmpty() )
		sResult.AppendFormat(_T("<w:pgSz %s/>"), sPageSize );

	CString sMargin;
	if( PROP_DEF != m_nMarginLeft )
		sMargin.AppendFormat(_T(" w:left=\"%d\""), m_nMarginLeft);
	if( PROP_DEF != m_nMarginRight )
		sMargin.AppendFormat(_T(" w:right=\"%d\""), m_nMarginRight);
	if( PROP_DEF != m_nMarginTop )
		sMargin.AppendFormat(_T(" w:top=\"%d\""), m_nMarginTop);
	if( PROP_DEF != m_nMarginBottom )
		sMargin.AppendFormat(_T(" w:bottom=\"%d\""), m_nMarginBottom);
	if( PROP_DEF != m_nGutterMarginWidth )
		sMargin.AppendFormat(_T(" w:gutter=\"%d\""), m_nGutterMarginWidth);
	if( PROP_DEF != m_nHeaderTop )
		sMargin.AppendFormat(_T(" w:header=\"%d\""), m_nHeaderTop);
	if( PROP_DEF != m_nFotterBottom )
		sMargin.AppendFormat(_T(" w:footer=\"%d\""), m_nFotterBottom);
	if( false == sMargin.IsEmpty() )
		sResult.AppendFormat(_T("<w:pgMar %s/>"), sMargin);
	if( 1 == m_bTitlePage )
		sResult.Append(_T("<w:titlePg/>"));

	//Page Numbers
	CString sPageNumber;
	if( PROP_DEF != m_nPageNumberStart )
		sPageNumber.AppendFormat( _T(" w:start=\"%d\""), m_nPageNumberStart );
	if( false == sPageNumber.IsEmpty() )
		sResult.AppendFormat( _T("<w:pgNumType %s/>"), sPageNumber );
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt:sResult.Append(_T("<w:vAlign w:val=\"top\" />"));break;
		case va_vertalb:sResult.Append(_T("<w:vAlign w:val=\"bottom\" />"));break;
		case va_vertalc:sResult.Append(_T("<w:vAlign w:val=\"center\" />"));break;
		case va_vertalj:sResult.Append(_T("<w:vAlign w:val=\"both\" />"));break;
	}
	//Text Flow
	switch( m_nTextFollow )
	{
		case 0: sResult.Append( _T("<w:textDirection w:val=\"lrTb\" />") );break;
		case 1: sResult.Append( _T("<w:textDirection w:val=\"tbRl\" />") );break;
		case 2: sResult.Append( _T("<w:textDirection w:val=\"lrBt\" />") );break;
		case 3: sResult.Append( _T("<w:textDirection w:val=\"rlTb\" />") );break;
		case 4: sResult.Append( _T("<w:textDirection w:val=\"lrTbV\" />") );break;
		case 5: sResult.Append( _T("<w:textDirection w:val=\"tbLrV\" />") );break;
	}
	//Page Borders
	CString sBorders;
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_TAG;
	if( true == m_oBorderLeft.IsValid() )
	{
		oNewParam.sValue = _T("w:left");
		sBorders.Append( m_oBorderLeft.RenderToOOX( oNewParam ) );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		oNewParam.sValue = _T("w:top");
		sBorders.Append( m_oBorderTop.RenderToOOX( oNewParam ) );
	}
	if( true == m_oBorderRight.IsValid() )
	{
		oNewParam.sValue = _T("w:right");
		sBorders.Append( m_oBorderRight.RenderToOOX( oNewParam ) );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		oNewParam.sValue = _T("w:bottom");
		sBorders.Append( m_oBorderBottom.RenderToOOX( oNewParam ) );
	}
	if( false == sBorders.IsEmpty() )
	{
		sResult.Append( _T("<w:pgBorders") );
		if( PROP_DEF != m_nBorderMeasure )
		{
			switch( m_nBorderMeasure & 3 )
			{
				case 0: sResult.Append( _T(" w:display=\"allPages\"") );break;
				case 1: sResult.Append( _T(" w:display=\"firstPage\"") );break;
				case 2: sResult.Append( _T(" w:display=\"notFirstPage\"") );break;
			}
			if( 0 != (m_nBorderMeasure & 8) )
				sResult.Append( _T(" w:zOrder=\"back\"") );
			else
				sResult.Append( _T(" w:zOrder=\"front\"") );
			if( 0 != (m_nBorderMeasure & 32) )
				sResult.Append( _T(" w:offsetFrom=\"page\"") );
			else
				sResult.Append( _T(" w:offsetFrom=\"text\"") );
		}
		sResult.Append( _T(">") );
		sResult.Append( sBorders );
		sResult.Append( _T("</w:pgBorders>") );
	}
	if( 0 != m_oHeaderLeft )
	{
		CString sId = SaveFile(m_oHeaderLeft,oRenderParameter, true);
		sResult.AppendFormat(_T("<w:headerReference w:type=\"even\" r:id=\"%s\"/>"), sId);
	}
	if( 0 != m_oHeaderFirst )
	{
		CString sId = SaveFile(m_oHeaderFirst,oRenderParameter, true);
		sResult.AppendFormat(_T("<w:headerReference w:type=\"first\" r:id=\"%s\"/>"), sId);
	}
	if( 0 != m_oHeaderRight )
	{
		CString sId = SaveFile(m_oHeaderRight,oRenderParameter, true);
		sResult.AppendFormat(_T("<w:headerReference w:type=\"default\" r:id=\"%s\"/>"), sId);
	}
	if( 0 != m_oFooterLeft )
	{
		CString sId = SaveFile(m_oFooterLeft,oRenderParameter, false);
		sResult.AppendFormat(_T("<w:footerReference w:type=\"even\" r:id=\"%s\"/>"), sId);
	}
	if( 0 != m_oFooterFirst )
	{
		CString sId = SaveFile(m_oFooterFirst,oRenderParameter, false);
		sResult.AppendFormat(_T("<w:footerReference w:type=\"first\" r:id=\"%s\"/>"), sId);
	}
	if( 0 != m_oFooterRight )
	{
		CString sId = SaveFile(m_oFooterRight,oRenderParameter, false);
		sResult.AppendFormat(_T("<w:footerReference w:type=\"default\" r:id=\"%s\"/>"), sId);
	}
	sResult.Append(_T("</w:sectPr>"));
	return sResult;
}
CString RtfSectionProperty::SaveFile( TextItemContainerPtr oTarget, RenderParameter oRenderParameter, bool bHeader)
	{
		OOXWriter* poOOXWriter = static_cast<OOXWriter*>( oRenderParameter.poWriter );
		RtfDocument* poRtfDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );

		CString sFilename;
		if( true == bHeader )
			sFilename.AppendFormat( _T("header%d.xml"), poRtfDocument->m_oIdGenerator.Generate_HeaderNumber() );
		else
			sFilename.AppendFormat( _T("footer%d.xml"), poRtfDocument->m_oIdGenerator.Generate_FooterNumber() );

		CString sContent;
		CString sRootName;
		if( true == bHeader )
			sRootName = _T("w:hdr");
		else
			sRootName = _T("w:ftr");
		sContent.AppendFormat( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n") );
		sContent.AppendFormat( _T("<%s xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\">"), sRootName );
		RenderParameter oNewParameter = oRenderParameter;
		OOXRelsWriterPtr oNewRelsWr = OOXRelsWriterPtr( new OOXRelsWriter( sFilename, *poRtfDocument ) );
		poOOXWriter->m_oCustomRelsWriter.Add( oNewRelsWr );
		oNewParameter.poRels = oNewRelsWr.get();
		sContent.Append( oTarget->RenderToOOX(oNewParameter) );
		sContent.AppendFormat( _T("</%s>"), sRootName );
		CStringA sContentUTF = Convert::UnicodeToUtf8( sContent );

		
		CString sFilePath = poOOXWriter->m_sTargetFolder + _T("\\") +poOOXWriter->m_sDocumentFolder+ _T("\\")+sFilename;
		DWORD dwBytesWritten = 0;
		HANDLE hFile = ::CreateFile(sFilePath,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		::WriteFile(hFile, sContentUTF, sContentUTF.GetLength(), &dwBytesWritten, NULL);
		CloseHandle( hFile );

		CString sContentTarget = _T("/") + poOOXWriter->m_sDocumentFolder + _T("/") +  sFilename;
		CString srID;
		if( true == bHeader )
		{
			poOOXWriter->m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml"), sContentTarget);
			srID = poOOXWriter->m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/header"), sFilename );
		}
		else
		{
			poOOXWriter->m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml"), sContentTarget);
			srID = poOOXWriter->m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer"), sFilename );
		}
		return srID;
	}