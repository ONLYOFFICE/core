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
#include "Writer/OOXWriter.h"
#include "IdGenerator.h"

CString RtfDocumentProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	switch( m_eCodePage )
	{
		case cp_ansi: 
			{
				sResult += _T("\\ansi");
				if( PROP_DEF != m_nAnsiCodePage )
					sResult.AppendFormat( _T("\\ansicpg%d"), m_nAnsiCodePage );

				break;
			}
		case cp_mac:	sResult += _T("\\mac" );	break;
		case cp_pc:		sResult += _T("\\pc" );		break;
		case cp_pca:	sResult += _T("\\pca" );	break;
	}
	RENDER_RTF_BOOL( m_bHypCaps, sResult, _T("hyphcaps") )
	RENDER_RTF_BOOL( m_bAutoHyp, sResult, _T("hyphauto") )
	RENDER_RTF_INT( m_nMaxHypen, sResult, _T("hyphconsec") )
	RENDER_RTF_INT( m_nHyphenationRight, sResult, _T("hyphhotz") )
	RENDER_RTF_INT( m_nTabWidth, sResult, _T("deftab") )
	RENDER_RTF_INT( m_nZoom,	sResult, _T("viewscale") )

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

	RENDER_RTF_BOOL( m_bGutterAtTop,	sResult, _T("gutterprl") )
	RENDER_RTF_BOOL( m_bSwitchMargins,	sResult, _T("margmirror") )
	RENDER_RTF_BOOL( m_bFacingPage,		sResult, _T("facingp") )

//Compatibility Options
	RENDER_RTF_INT( m_nDisplayBackground,	sResult, _T("viewbksp") )
	RENDER_RTF_BOOL( m_bUseTabAlignment,	sResult, _T("useltbaln") )

	//footnote, endnote property
	if( PROP_DEF != m_nFootnoteNumberingFormat )
		sResult += _T("\\") + GetRtfFormat(m_nFootnoteNumberingFormat, true);
	RENDER_RTF_INT( m_nFootnoteStart, sResult, _T("ftnstart") )
	if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage:		sResult += _T("ftnrstpg") ;		break;
		case fr_EachSection:	sResult += _T("ftnrestart");	break;
		case fr_Continuous:		sResult += _T("ftnrstcont");	break;
		}
	}
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnoteRestart )
		{
		case fp_EndSection:		sResult += _T("endnotes");	break;
		case fp_EndDocument:	sResult += _T("enddoc");	break;
		case fp_BeneathText:	sResult += _T("ftntj");		break;
		case fp_BottomPage:		sResult += _T("ftnbj");		break;
		}
	}
	if( PROP_DEF != m_nEndnoteNumberingFormat )
		sResult += _T("\\") + GetRtfFormat(m_nEndnoteNumberingFormat, false);

	RENDER_RTF_INT( m_nEndnoteStart, sResult, _T("aftnstart") )
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case er_EachSection:	sResult += _T("aftnrestart"); break;
		case er_Continuous:		sResult += _T("aftnrstcont"); break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case ep_EndSection:		sResult += _T("aendnotes"); break;
		case ep_EndDocument:	sResult += _T("aenddoc");	break;
		case ep_BeneathText:	sResult += _T("aftntj");	break;
		case ep_BottomPage:		sResult += _T("aftnbj");	break;
		}
	}

//для схоранения в RTF
	RENDER_RTF_INT( m_bSplitPageBrake, sResult, _T("spltpgpar") )

	if( 0 != m_bHtmlAutoSpace )
		sResult += _T("\\htmautsp");

	return sResult;
}
CString RtfDocumentProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_bHypCaps )
	{
		if( 0 == m_bHypCaps )
			sResult += _T("<w:doNotHyphenateCaps w:val=\"true\"/>");
		else
			sResult += _T("<w:doNotHyphenateCaps w:val=\"false\"/>");
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
			sResult += _T("<w:bordersDoNotSurroundHeader w:val=\"true\"/>");
		else
			sResult += _T("<w:bordersDoNotSurroundHeader w:val=\"false\"/>");
	}
	if( PROP_DEF != m_bDorderSurroundFotter )
	{
		if( 0 == m_bDorderSurroundFotter )
			sResult += _T("<w:bordersDoNotSurroundFooter w:val=\"true\"/>");
		else
			sResult += _T("<w:bordersDoNotSurroundFooter w:val=\"false\"/>");
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
	for( int i = 0; i < m_aSpecialFootnotes.size(); i++ )
	{
		sFootnoteProp.AppendFormat( _T("<w:footnote w:id=\"%d\" />"), m_aSpecialFootnotes[i] );
	}
	
	if( PROP_DEF != m_nFootnoteNumberingFormat )
	{
        sFootnoteProp += _T("<w:numRestart w:val=\"");
		sFootnoteProp += RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat );
		sFootnoteProp += _T("\" />");
	}
	RENDER_OOX_INT( m_nFootnoteStart, sFootnoteProp, _T("w:numStart") );
	
	if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage:		sFootnoteProp += _T("<w:numRestart w:val=\"eachPage\"/>");		break;
		case fr_EachSection:	sFootnoteProp += _T("<w:numRestart w:val=\"eachSect\"/>");		break;
		case fr_Continuous:		sFootnoteProp += _T("<w:numRestart w:val=\"continuous\"/>");	break;
		}
	}
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnotePlacement )
		{
		case fp_EndSection:		sFootnoteProp += _T("<w:pos w:val=\"sectEnd\"/>");		break;
		case fp_EndDocument:	sFootnoteProp += _T("<w:pos w:val=\"docEnd\"/>");		break;
		case fp_BeneathText:	sFootnoteProp += _T("<w:pos w:val=\"beneathText\"/>");	break;
		case fp_BottomPage:		sFootnoteProp += _T("<w:pos w:val=\"pageBottom\"/>");	break;
		}
	}
	if( !sFootnoteProp.IsEmpty() && !m_aSpecialFootnotes.empty())
        sResult += _T("<w:footnotePr>") + sFootnoteProp + _T("</w:footnotePr>");

	CString sEndnoteProp;
    //todooo проверить id
    for( int i = 0; i < m_aSpecialEndnotes.size(); i++ )
	{
        sEndnoteProp.AppendFormat( _T("<w:endnote w:id=\"%i\"/>"), m_aSpecialEndnotes[i] );
	}
	if( PROP_DEF != m_nFootnoteNumberingFormat )
	{
        sFootnoteProp += _T("<w:numRestart w:val=\"");
		sFootnoteProp += RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat );
		sFootnoteProp += _T("\"/>");
	}
	RENDER_OOX_INT( m_nEndnoteStart, sEndnoteProp, _T("w:numStart") );
	
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case fr_EachSection:	sEndnoteProp += _T("<w:numRestart w:val=\"eachSect\"/>" );		break;
		case fr_Continuous:		sEndnoteProp += _T("<w:numRestart w:val=\"continuous\"/>" );	break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case fp_EndSection:		sEndnoteProp += _T("<w:pos w:val=\"sectEnd\"/>" );		break;
		case fp_EndDocument:	sEndnoteProp += _T("<w:pos w:val=\"docEnd\"/>" );		break;
		case fp_BeneathText:	sEndnoteProp += _T("<w:pos w:val=\"beneathText\"/>" );	break;
		case fp_BottomPage:		sEndnoteProp += _T("<w:pos w:val=\"pageBottom\"/>" );	break;
		}
	}
	if( !sEndnoteProp.IsEmpty() && !m_aSpecialEndnotes.empty())
        sResult += _T("<w:endnotePr>") + sEndnoteProp + _T("</w:endnotePr>");

	CString sCompat;
	if( 1 != m_bHtmlAutoSpace )
		sCompat += _T("<w:doNotUseHTMLParagraphAutoSpacing/>");
	if( 0 == m_bUseTabAlignment )
		sCompat += _T("<w:forgetLastTabAlignment/>");
	
	if( !sCompat.IsEmpty() )
	{
        sResult += _T("<w:compat>") + sCompat + _T("</w:compat>");
	}

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

	sResult += _T("\\sectd");
	RENDER_RTF_BOOL( m_bBidi, sResult, _T("endnhere") )
	RENDER_RTF_INT( m_nPaperSourceFirst, sResult, _T("binfsxn") )
	RENDER_RTF_INT( m_nPaperSourceOther, sResult, _T("binsxn") )
	RENDER_RTF_BOOL( m_bRtlGutter, sResult, _T("rtlgutter") )
	RENDER_RTF_BOOL( m_bEndnotes, sResult, _T("endnhere") )
	RENDER_RTF_INT( m_nStyle, sResult, _T("ds") )
	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbknone:	sResult += _T("\\sbknone" );	break;
		case sb_sbkcol:		sResult += _T("\\sbkcol" );		break;
		case sb_sbkpage:	sResult += _T("\\sbkpage" );	break;
		case sb_sbkeven:	sResult += _T("\\sbkeven" );	break;
		case sb_sbkodd:		sResult += _T("\\sbkodd" );		break;
	}
	//Columns
	RENDER_RTF_INT( m_nColumnNumber, sResult, _T("cols") )
	RENDER_RTF_INT( m_nColumnSpace, sResult, _T("colsx") )
	RENDER_RTF_BOOL( m_bColumnLineBetween, sResult, _T("linebetcol") )
	int nCollFormCount = (int)m_oCollumnProperty.m_aCollumnProperty.size();
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
		case fj_sftntj: sResult += _T("\\sftntj" );	break;
		case fj_sftnbj: sResult += _T("\\sftnbj" );	break;
	}
	RENDER_RTF_INT( m_nFootnotesStart, sResult, _T("sftnstart") )
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg:		sResult += _T("\\sftnrstpg" );		break;
		case fr_sftnrestart:	sResult += _T("\\sftnrestart" );	break;
		case fr_sftnrstcont:	sResult += _T("\\sftnrstcont" );	break;
	}
	switch( m_eFootnotesFormat )
	{
		case ff_sftnnar:		sResult += _T("\\sftnnar" );		break;
		case ff_sftnnalc:		sResult += _T("\\sftnnalc" );		break;
		case ff_sftnnauc:		sResult += _T("\\sftnnauc" );		break;
		case ff_sftnnrlc:		sResult += _T("\\sftnnrlc" );		break;
		case ff_sftnnruc:		sResult += _T("\\sftnnruc" );		break;
		case ff_sftnnchi:		sResult += _T("\\sftnnchi" );		break;
		case ff_sftnnchosung:	sResult += _T("\\sftnnchosung" );	break;
		case ff_sftnncnum:		sResult += _T("\\sftnncnum" );		break;
		case ff_sftnndbnum:		sResult += _T("\\sftnndbnum" );		break;
		case ff_sftnndbnumd:	sResult += _T("\\sftnndbnumd" );	break;
		case ff_sftnndbnumt:	sResult += _T("\\sftnndbnumt" );	break;
		case ff_sftnndbnumk:	sResult += _T("\\sftnndbnumk" );	break;
		case ff_sftnndbar:		sResult += _T("\\sftnndbar" );		break;
		case ff_sftnnganada:	sResult += _T("\\sftnnganada" );	break;
		case ff_sftnngbnum:		sResult += _T("\\sftnngbnum" );		break;
		case ff_sftnngbnumd:	sResult += _T("\\sftnngbnumd" );	break;
		case ff_sftnngbnuml:	sResult += _T("\\sftnngbnuml" );	break;
		case ff_sftnngbnumk:	sResult += _T("\\sftnngbnumk" );	break;
		case ff_sftnnzodiac:	sResult += _T("\\sftnnzodiac" );	break;
		case ff_sftnnzodiacd:	sResult += _T("\\sftnnzodiacd" );	break;
		case ff_sftnnzodiacl:	sResult += _T("\\sftnnzodiacl" );	break;
	}
	RENDER_RTF_INT( m_nEndnotesStart, sResult, _T("saftnstart") )
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart: sResult += _T("\\saftnrestart" );		break;
		case er_saftnrstcont: sResult += _T("\\saftnrstcont" );		break;
	}
	switch( m_eEndnotesFormat )
	{
		case ef_saftnnar:		sResult += _T("\\saftnnar" );		break;
		case ef_saftnnalc:		sResult += _T("\\saftnnalc" );		break;
		case ef_saftnnauc:		sResult += _T("\\saftnnauc" );		break;
		case ef_saftnnrlc:		sResult += _T("\\saftnnrlc" );		break;
		case ef_saftnnruc:		sResult += _T("\\saftnnruc" );		break;
		case ef_saftnnchi:		sResult += _T("\\saftnnchi" );		break;
		case ef_saftnnchosung:	sResult += _T("\\saftnnchosung" );	break;
		case ef_saftnncnum:		sResult += _T("\\saftnncnum" );		break;
		case ef_saftnndbnum:	sResult += _T("\\saftnndbnum" );	break;
		case ef_saftnndbnumd:	sResult += _T("\\saftnndbnumd" );	break;
		case ef_saftnndbnumt:	sResult += _T("\\saftnndbnumt" );	break;
		case ef_saftnndbnumk:	sResult += _T("\\saftnndbnumk" );	break;
		case ef_saftnndbar:		sResult += _T("\\saftnndbar" );		break;
		case ef_saftnnganada:	sResult += _T("\\saftnnganada" );	break;
		case ef_saftnngbnum:	sResult += _T("\\saftnngbnum" );	break;
		case ef_saftnngbnumd:	sResult += _T("\\saftnngbnumd" );	break;
		case ef_saftnngbnuml:	sResult += _T("\\saftnngbnuml" );	break;
		case ef_saftnngbnumk:	sResult += _T("\\saftnngbnumk" );	break;
		case ef_saftnnzodiac:	sResult += _T("\\saftnnzodiac" );	break;
		case ef_saftnnzodiacd:	sResult += _T("\\saftnnzodiacd" );	break;
		case ef_saftnnzodiacl:	sResult += _T("\\saftnnzodiacl" );	break;
	}

	//Line Numbering
	RENDER_RTF_INT( m_nLineModulus, sResult, _T("linemod") )
	RENDER_RTF_INT( m_nLineX, sResult, _T("linex") )
	RENDER_RTF_INT( m_nLineStart, sResult, _T("linestarts") )
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart:	sResult += _T("\\linerestart" );break;
		case lnr_lineppage:		sResult += _T("\\lineppage" );	break;
		case lnr_linecont:		sResult += _T("\\linecont" );	break;
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
	RENDER_RTF_INT( m_nFooterBottom, sResult, _T("footery") )

	//Page Numbers
	RENDER_RTF_INT( m_nPageNumberStart, sResult, _T("pgnstarts") )
	RENDER_RTF_BOOL( m_bPageNumberContinuos, sResult, _T("pgncont") )
	RENDER_RTF_BOOL( m_bPageNumberRestart, sResult, _T("pgnrestart") )
	RENDER_RTF_INT( m_nPageNumberX, sResult, _T("pgnx") )
	RENDER_RTF_INT( m_nPageNumberY, sResult, _T("pgny") )
	switch( m_ePageNumberFormat )
	{
		case pnf_pgndec:	sResult += _T("\\pgndec" );		break;
		case pnf_pgnucrm:	sResult += _T("\\pgnucrm" );	break;
		case pnf_pgnlcrm:	sResult += _T("\\pgnlcrm" );	break;
	}
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt: sResult += _T("\\vertalt" );	break;
		case va_vertalb: sResult += _T("\\vertalb" );	break;
		case va_vertalc: sResult += _T("\\vertalc" );	break;
		case va_vertalj: sResult += _T("\\vertalj" );	break;

	}
	//Text Flow
	RENDER_RTF_INT( m_nTextFollow, sResult, _T("stextflow") )

	//Page Borders
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += _T("\\pgbrdrl");
		sResult += m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += _T("\\pgbrdrt");
		sResult += m_oBorderTop.RenderToRtf( oRenderParameter );
	}
		
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += _T("\\pgbrdrr");
		sResult += m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += _T("\\pgbrdrb");
		sResult += m_oBorderBottom.RenderToRtf( oRenderParameter );
	}
	RENDER_RTF_INT( m_nBorderArt, sResult, _T("brdrart") )
	RENDER_RTF_INT( m_nBorderMeasure, sResult, _T("pgbrdropt") )
	RENDER_RTF_INT( m_nBorderAlign, sResult, _T("pgbrdrsnap") )

	//footer - header
	if( 0 != m_oHeaderRight )
	{
		sResult += _T("{\\headerr");
		sResult += m_oHeaderRight->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	if( 0 != m_oHeaderLeft )
	{
		sResult += _T("{\\headerl");
		sResult += m_oHeaderLeft->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	if( 0 != m_oHeaderFirst )
	{
		sResult += _T("{\\headerf");
		sResult += m_oHeaderFirst->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	if( 0 != m_oFooterRight )
	{
		sResult += _T("{\\footerr");
		sResult += m_oFooterRight->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	if( 0 != m_oFooterLeft )
	{
		sResult += _T("{\\footerl");
		sResult += m_oFooterLeft->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	if( 0 != m_oFooterFirst )
	{
		sResult += _T("{\\footerf");
		sResult += m_oFooterFirst->RenderToRtf( oRenderParameter );
		sResult += _T("\\par");
		sResult += _T("}");
	}
	return sResult;
}
CString RtfSectionProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult += _T("<w:sectPr>");
	if( 1 == m_bBidi )
		sResult += _T("<w:bidi/>");
	CString sPaperSource;
	if( PROP_DEF != m_nPaperSourceFirst )
		sPaperSource.AppendFormat( _T(" w:first=\"%d\""), m_nPaperSourceFirst );
	if( PROP_DEF != m_nPaperSourceFirst )
		sPaperSource.AppendFormat( _T(" w:other=\"%d\""), m_nPaperSourceOther );
	
	if( !sPaperSource.IsEmpty() )
        sResult += _T("<w:paperSrc ") + sPaperSource + _T("/>");
	
	if( 1 == m_bRtlGutter )
		sResult += _T("<w:rtlGutter/>");

	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbkcol:		sResult += _T("<w:type w:val=\"nextColumn\"/>");	break;
		case sb_sbkpage:	sResult += _T("<w:type w:val=\"nextPage\"/>");		break;
		case sb_sbkeven:	sResult += _T("<w:type w:val=\"evenPage\"/>");		break;
		case sb_sbkodd:		sResult += _T("<w:type w:val=\"oddPage\"/>");		break;
		case sb_sbknone:	
		default:
			sResult += _T("<w:type w:val=\"continuous\"/>");				
			break;
	}
	//Columns
	if( PROP_DEF != m_nColumnNumber ) 
	{
		CString sCollumnFormating;
		bool bEqualWidth = false;
		if( m_nColumnNumber != m_oCollumnProperty.m_aCollumnProperty.size() )
			bEqualWidth = true;
		else
			for( int i = 0; i < (int)m_oCollumnProperty.m_aCollumnProperty.size(); i++ )
				if( PROP_DEF == m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
				{
					bEqualWidth = true;
					break;
				}
		sCollumnFormating.AppendFormat( _T("<w:cols w:num=\"%d\""), m_nColumnNumber );
		if( PROP_DEF != m_nColumnSpace )
			sCollumnFormating.AppendFormat( _T(" w:space=\"%d\""), m_nColumnSpace );
		if( true == bEqualWidth )
			sCollumnFormating += _T(" w:equalWidth=\"true\"/>" );
		else
		{
			sCollumnFormating += _T(" w:equalWidth=\"false\">");
			for( int i = 0; i < (int)m_oCollumnProperty.m_aCollumnProperty.size(); i++ )
			{
				sCollumnFormating += _T( "<w:col" );
				if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
					sCollumnFormating.AppendFormat( _T(" w:w=\"%d\""), m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth );
				if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol )
					sCollumnFormating.AppendFormat( _T(" w:space=\"%d\""), m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol );
				sCollumnFormating += _T( "/>" );
			}
			sCollumnFormating += _T( "</w:cols>" );
		}
		if( false == sCollumnFormating.IsEmpty() )
			sResult += sCollumnFormating;
	}

	//Footnotes and Endnotes
	CString sFootnotesProp;
	switch( m_eFootnotesJust )
	{
		case fj_sftntj:	sFootnotesProp += _T("<w:pos w:val=\"beneathText\"/>" );	break;
		case fj_sftnbj:	sFootnotesProp += _T("<w:pos w:val=\"pageBottom\"/>" );		break;
	}
	RENDER_OOX_INT( m_nFootnotesStart, sFootnotesProp, _T("w:numStart") )
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg:		sFootnotesProp += _T("<w:numRestart w:val=\"eachPage\"/>" );	break;
		case fr_sftnrestart:	sFootnotesProp += _T("<w:numRestart w:val=\"eachSect\"/>" );	break;
		case fr_sftnrstcont:	sFootnotesProp += _T("<w:numRestart w:val=\"continuous\"/>" );	break;
	}
	switch( m_eFootnotesFormat )
	{
		case ff_sftnnar:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnalc:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnauc:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnrlc:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnruc:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnchi:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnchosung:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnncnum:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnndbnum:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnndbnumd:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnndbnumt:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnndbnumk:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnndbar:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnganada:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnngbnum:		sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnngbnumd:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnngbnuml:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnngbnumk:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnzodiac:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnzodiacd:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ff_sftnnzodiacl:	sFootnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
	}
	if(!sFootnotesProp.IsEmpty() )
        sResult +=  _T("<w:footnotePr>") + sFootnotesProp + _T("</w:footnotePr>");

	CString sEndnotesProp;
	RENDER_OOX_INT( m_nEndnotesStart, sEndnotesProp, _T("w:numStart") )
	
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart:	sEndnotesProp += _T("<w:numRestart w:val=\"eachPage\"/>" );		break;
		case er_saftnrstcont:	sEndnotesProp += _T("<w:numRestart w:val=\"continuous\"/>" );	break;
	}
	switch( m_eEndnotesFormat )
	{
		case ef_saftnnar:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnalc:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnauc:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnrlc:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnruc:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnchi:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnchosung:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnncnum:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnndbnum:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnndbnumd:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnndbnumt:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnndbnumk:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnndbar:		sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnganada:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnngbnum:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnngbnumd:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnngbnuml:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnngbnumk:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnzodiac:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnzodiacd:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
		case ef_saftnnzodiacl:	sEndnotesProp += _T("<w:numFmt w:val=\"upperLetter\" />" );	break;
	}
	if(!sEndnotesProp.IsEmpty() )
        sResult += _T("<w:endnotePr>") + sEndnotesProp + _T("</w:endnotePr>");

	//Line Numbering
	CString sLineNumbering;
	if( PROP_DEF != m_nLineModulus )
		sLineNumbering.AppendFormat( _T(" w:countBy=\"%d\""), m_nLineModulus );
	if( PROP_DEF != m_nLineX && 0 != m_nLineX)
		sLineNumbering.AppendFormat( _T(" w:distance=\"%d\""), m_nLineX );
	if( PROP_DEF != m_nLineStart )
		sLineNumbering.AppendFormat( _T(" w:start=\"%d\""), m_nLineStart );
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart:	sLineNumbering += _T(" w:restart=\"newSection\"");	break; 
		case lnr_lineppage:		sLineNumbering += _T(" w:restart=\"newPage\"");		break; 
		case lnr_linecont:		sLineNumbering += _T(" w:restart=\"continuous\"");	break; 
	}
	if( false == sLineNumbering.IsEmpty() )
        sResult += _T("<w:lnNumType ") + sLineNumbering+ _T("/>");

	//Page Information
	CString sPageSize;
	if( PROP_DEF != m_nPageWidth )
		sPageSize.AppendFormat(_T(" w:w=\"%d\""),m_nPageWidth);
	if( PROP_DEF != m_nPageHeight )
		sPageSize.AppendFormat(_T(" w:h=\"%d\""),m_nPageHeight);
	if( 1 == m_bLandscapeFormat )
		sPageSize.Append(_T(" w:orient=\"landscape\""));
	if( false == sPageSize.IsEmpty() )
        sResult += _T("<w:pgSz ") + sPageSize + _T("/>");

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
	if( PROP_DEF != m_nFooterBottom )
		sMargin.AppendFormat(_T(" w:footer=\"%d\""), m_nFooterBottom);
	if( false == sMargin.IsEmpty() )
        sResult += _T("<w:pgMar ") + sMargin + _T("/>");
	if( 1 == m_bTitlePage )
		sResult += _T("<w:titlePg/>");

	//Page Numbers
	CString sPageNumber;
	if( PROP_DEF != m_nPageNumberStart )
		sPageNumber.AppendFormat( _T(" w:start=\"%d\""), m_nPageNumberStart );
	if( false == sPageNumber.IsEmpty() )
        sResult += _T("<w:pgNumType ") + sPageNumber + _T("/>");
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt:	sResult += _T("<w:vAlign w:val=\"top\" />");	break;
		case va_vertalb:	sResult += _T("<w:vAlign w:val=\"bottom\" />");	break;
		case va_vertalc:	sResult += _T("<w:vAlign w:val=\"center\" />");	break;
		case va_vertalj:	sResult += _T("<w:vAlign w:val=\"both\" />");	break;
	}
	//Text Flow
	switch( m_nTextFollow )
	{
		case 0: sResult += _T("<w:textDirection w:val=\"lrTb\" />" );	break;
		case 1: sResult += _T("<w:textDirection w:val=\"tbRl\" />" );	break;
		case 2: sResult += _T("<w:textDirection w:val=\"lrBt\" />" );	break;
		case 3: sResult += _T("<w:textDirection w:val=\"rlTb\" />" );	break;
		case 4: sResult += _T("<w:textDirection w:val=\"lrTbV\" />" );	break;
		case 5: sResult += _T("<w:textDirection w:val=\"tbLrV\" />" );	break;
	}
	//Page Borders
	CString sBorders;
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_TAG;
	if( true == m_oBorderLeft.IsValid() )
	{
		oNewParam.sValue = _T("w:left");
		sBorders += m_oBorderLeft.RenderToOOX( oNewParam );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		oNewParam.sValue = _T("w:top");
		sBorders += m_oBorderTop.RenderToOOX( oNewParam ) ;
	}
	if( true == m_oBorderRight.IsValid() )
	{
		oNewParam.sValue = _T("w:right");
		sBorders += m_oBorderRight.RenderToOOX( oNewParam );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		oNewParam.sValue = _T("w:bottom");
		sBorders += m_oBorderBottom.RenderToOOX( oNewParam );
	}
	if( false == sBorders.IsEmpty() )
	{
		sResult += _T("<w:pgBorders");
		if( PROP_DEF != m_nBorderMeasure )
		{
			switch( m_nBorderMeasure & 3 )
			{
				case 0: sResult += _T(" w:display=\"allPages\"" );		break;
				case 1: sResult += _T(" w:display=\"firstPage\"" );		break;
				case 2: sResult += _T(" w:display=\"notFirstPage\"" );	break;
			}
			if( 0 != (m_nBorderMeasure & 8) )
				sResult += _T(" w:zOrder=\"back\"");
			else
				sResult += _T(" w:zOrder=\"front\"");
			if( 0 != (m_nBorderMeasure & 32) )
				sResult += _T(" w:offsetFrom=\"page\"");
			else
				sResult += _T(" w:offsetFrom=\"text\"");
		}
		sResult += _T(">");
		sResult += sBorders;
		sResult += _T("</w:pgBorders>");
	}
	if( 0 != m_oHeaderLeft )
	{
		CString sId = SaveFile(m_oHeaderLeft,oRenderParameter, true);
        sResult += _T("<w:headerReference w:type=\"even\" r:id=\"") + sId + _T("\"/>");
	}
	if( 0 != m_oHeaderFirst )
	{
		CString sId = SaveFile(m_oHeaderFirst,oRenderParameter, true);
        sResult += _T("<w:headerReference w:type=\"first\" r:id=\"") + sId + _T("\"/>");
	}
	if( 0 != m_oHeaderRight )
	{
		CString sId = SaveFile(m_oHeaderRight,oRenderParameter, true);
        sResult += _T("<w:headerReference w:type=\"default\" r:id=\"") + sId + _T("\"/>");
	}
	if( 0 != m_oFooterLeft )
	{
		CString sId = SaveFile(m_oFooterLeft,oRenderParameter, false);
        sResult += _T("<w:footerReference w:type=\"even\" r:id=\"") + sId + _T("\"/>");
	}
	if( 0 != m_oFooterFirst )
	{
		CString sId = SaveFile(m_oFooterFirst,oRenderParameter, false);
        sResult += _T("<w:footerReference w:type=\"first\" r:id=\"") + sId + _T("\"/>");
	}
	if( 0 != m_oFooterRight )
	{
		CString sId = SaveFile(m_oFooterRight,oRenderParameter, false);
        sResult += _T("<w:footerReference w:type=\"default\" r:id=\"") + sId + _T("\"/>");
	}
	sResult.Append(_T("</w:sectPr>"));
	return sResult;
}
CString RtfSectionProperty::SaveFile( TextItemContainerPtr oTarget, RenderParameter oRenderParameter, bool bHeader)
{
	OOXWriter* poOOXWriter		= static_cast<OOXWriter*>	( oRenderParameter.poWriter );
	RtfDocument* poRtfDocument	= static_cast<RtfDocument*> ( oRenderParameter.poDocument );

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
	sContent += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
    sContent += _T("<");
	sContent += sRootName;
	sContent += _T(" xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\">");
	
	RenderParameter oNewParameter	= oRenderParameter;
	OOXRelsWriterPtr oNewRelsWr		= OOXRelsWriterPtr( new OOXRelsWriter( sFilename, *poRtfDocument ) );
	poOOXWriter->m_oCustomRelsWriter.push_back( oNewRelsWr );
	oNewParameter.poRels			= oNewRelsWr.get();
	
	sContent += oTarget->RenderToOOX(oNewParameter);
    sContent += _T("</") + sRootName + _T(">");

    std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sContent.GetBuffer());
	
	CString sFilePath = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + _T("word") + FILE_SEPARATOR_STR + sFilename;
	
	CFile file;
	
    if (file.CreateFile(sFilePath) == S_OK)
	{            
        file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());
		file.CloseFile();
	}

	CString sContentTarget = _T("/word/")  +  sFilename;
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
