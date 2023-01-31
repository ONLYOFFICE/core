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
#include "../OOXml/Writer/OOXWriter.h"
#include "IdGenerator.h"

RtfDocumentProperty::RtfDocumentProperty()
{
	SetDefault();
}
int RtfDocumentProperty::GetType()
{
	return TYPE_RTF_PROPERTY_DOCUMENT;
}
bool RtfDocumentProperty::IsValid()
{
	return true;
}
void RtfDocumentProperty::SetDefaultRtf()
{
	SetDefault();
	m_bUseTabAlignment = 0;
}
void RtfDocumentProperty::SetDefaultOOX()
{
	SetDefault();
}
void RtfDocumentProperty::SetDefault()
{
	m_eCodePage					= cp_ansi;
	m_nTabWidth					= 720; //todo -1
	m_nDeffFont					= PROP_DEF;
	m_nDeffMathFont				= PROP_DEF;
	m_bHypCaps					= PROP_DEF;
	m_bAutoHyp					= PROP_DEF;
	m_nMaxHypen					= PROP_DEF;
	m_nHyphenationRight			= PROP_DEF;
	m_nZoom						= 100;
	m_nAnsiCodePage				= CP_ACP;

	m_bDorderSurroundHeader		= PROP_DEF;
	m_bDorderSurroundFotter		= PROP_DEF;
	m_bAlignBordersAndEdges		= PROP_DEF;

	m_bRtlGutter				= PROP_DEF;
	m_bRtl						= PROP_DEF;

	m_nThemelang				= PROP_DEF;
	m_nThemelangfe				= PROP_DEF;
	m_nThemelangcs				= PROP_DEF;

	m_nPaperWidth				= 12240;
	m_nPaperHeight				= 15840;
	m_nMarginLeft				= 1701;
	m_nMarginRight				= 850;
	m_nMarginTop				= 1134;
	m_nMarginBottom				= 1134;
	m_bFacingPage				= PROP_DEF;
	m_nGutterWidth				= 0;
	m_nGutterWidthOutside		= PROP_DEF;
	m_bGutterAtTop				= PROP_DEF;
	m_bSwitchMargins			= PROP_DEF;
	m_bLandScape				= PROP_DEF;
	m_nBeginningPageNumber		= PROP_DEF;
	m_nDisplayBackground		= PROP_DEF;
	m_bUseTabAlignment			= PROP_DEF;

	m_nDrawingGridHorizontalSpacing		= PROP_DEF;
	m_nDrawingGridVerticalSpacing		= PROP_DEF;
	m_nDrawingGridHorizontalOrigin		= PROP_DEF;
	m_nDrawingGridVerticalOrigin		= PROP_DEF;
	m_nDisplayHorizontalDrawingGridEvery= PROP_DEF;		//def = 3
	m_nDisplayVerticalDrawingGridEvery	= PROP_DEF;		//def = 0

	m_aSpecialFootnotes.clear();
	m_nFootnoteNumberingFormat	= PROP_DEF;
	m_nFootnoteStart			= PROP_DEF;
	m_eFootnoteRestart			= fr_None;
	m_eFootnotePlacement		= fp_None;

	m_aSpecialEndnotes.clear();
	m_nEndnoteNumberingFormat	= PROP_DEF;
	m_nEndnoteStart				= PROP_DEF;
	m_eEndnoteRestart			= er_None;
	m_eEndnotePlacement			= ep_None;

	m_bSplitPageBrake			= 1;
	m_bHtmlAutoSpace			= PROP_DEF;
}
std::wstring RtfDocumentProperty::GetRtfFormat( int nFormat, bool bFootnote )
{
	if( PROP_DEF == nFormat )
	{
		if( true == bFootnote )	return L"ftnnar";
		else					return L"aftnnar";
	}
	if( true == bFootnote )
	{
		 switch( nFormat )
		 {
		 case 0:	return L"ftnnar";
		 case 4:	return L"ftnnalc";
		 case 3:	return L"ftnnauc";
		 case 2:	return L"ftnnrlc";
		 case 1:	return L"ftnnruc";
		 case 70:	return L"ftnnchi";
		 case 25:	return L"ftnnchi";
		 case 18:	return L"ftnncnum";
		 case 10:	return L"ftnndbnum";
		 case 11:	return L"ftnndbnumd";
		 case 16:	return L"ftnndbnumt";
		 case 17:	return L"ftnndbnumk";
		 case 20:	return L"ftnndbar";
		 case 24:	return L"ftnnganada";
		 case 26:	return L"ftnngbnum";
		 case 27:	return L"ftnngbnumd";
		 case 28:	return L"ftnngbnuml";
		 case 29:	return L"ftnngbnumk";
		 case 30:	return L"ftnnzodiac";
		 case 31:	return L"ftnnzodiacd";
		 case 32:	return L"ftnnzodiacl";
		 }
	}
	else
	{
		switch( nFormat )
		{
			case 0:		return L"aftnnar";
			case 4:		return L"aftnnalc";
			case 3:		return L"aftnnauc";
			case 2:		return L"aftnnrlc";
			case 1:		return L"aftnnruc";
			case 70:	 return L"aftnnchi";
			case 25:	return L"aftnnchi";
			case 18:	return L"aftnncnum";
			case 10:	return L"aftnndbnum";
			case 11:	return L"aftnndbnumd";
			case 16:	return L"aftnndbnumt";
			case 17:	return L"aftnndbnumk";
			case 20:	return L"aftnndbar";
			case 24:	return L"aftnnganada";
			case 26:	return L"aftnngbnum";
			case 27:	return L"aftnngbnumd";
			case 28:	return L"aftnngbnuml";
			case 29:	return L"aftnngbnumk";
			case 30:	return L"aftnnzodiac";
			case 31:	return L"aftnnzodiacd";
			case 32:	return L"aftnnzodiacl";
		}
	}
	if( true == bFootnote )	return L"ftnnar";
	else					return L"aftnnar";
}
std::wstring RtfDocumentProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	switch( m_eCodePage )
	{
		case cp_ansi: 
			{
				sResult += L"\\ansi";
				if( PROP_DEF != m_nAnsiCodePage )
                    sResult += L"\\ansicpg" + std::to_wstring( m_nAnsiCodePage );

				break;
			}
		case cp_mac:	sResult += L"\\mac";	break;
		case cp_pc:		sResult += L"\\pc";		break;
		case cp_pca:	sResult += L"\\pca";	break;
	}
	RENDER_RTF_BOOL	( m_bHypCaps,			sResult, L"hyphcaps" )
	RENDER_RTF_BOOL	( m_bAutoHyp,			sResult, L"hyphauto" )
	RENDER_RTF_INT	( m_nMaxHypen,			sResult, L"hyphconsec" )
	RENDER_RTF_INT	( m_nHyphenationRight,	sResult, L"hyphhotz" )
	RENDER_RTF_INT	( m_nTabWidth,			sResult, L"deftab" )
	RENDER_RTF_INT	( m_nZoom,				sResult, L"viewscale" )

//Page Borders
	RENDER_RTF_BOOL	( m_bDorderSurroundHeader, sResult, L"pgbrdrhead" )
	RENDER_RTF_BOOL	( m_bDorderSurroundFotter, sResult, L"pgbrdrfoot" )

	RENDER_RTF_BOOL	( m_bAlignBordersAndEdges, sResult, L"pgbrdrsnap" )

//Page Information
//эти свойства пишутся в первой секции
//public: int m_nPaperWidth; // paperwN	Paper width in twips (default is 12,240).
//public: int m_nPaperHeight; // paperhN	Paper height in twips (default is 15,840).
//public: int m_nMarginLeft; // marglN	Left margin in twips (default is 1800).
//public: int m_nMarginRight; // margrN	Right margin in twips (default is 1800).
//public: int m_nMarginTop; // margtN	Top margin in twips (default is 1440).
//public: int m_nMarginBottom; // margbN	Bottom margin in twips (default is 1440).
//public: int m_bFacingPage; //(mirrorMargins)\facingp	Facing pages (activates odd/even headers and gutters).
//public: int m_nGutterWidth; // gutterN	Gutter width in twips (default is 0).
//public: int m_nGutterWidthOutside; // ogutterN	Outside gutter width (default is 0; not used by Word, but in 1987 RTF Spec)
//	RENDER_RTF_BOOL( m_bLandScape, sResult, L"landscape" )

	RENDER_RTF_BOOL( m_bGutterAtTop,	sResult, L"gutterprl" )
	RENDER_RTF_BOOL( m_bSwitchMargins,	sResult, L"margmirror" )
	RENDER_RTF_BOOL( m_bFacingPage,		sResult, L"facingp" )

//Compatibility Options
	RENDER_RTF_INT( m_nDisplayBackground,	sResult, L"viewbksp" )
	RENDER_RTF_BOOL( m_bUseTabAlignment,	sResult, L"useltbaln" )

	//footnote, endnote property
	if( PROP_DEF != m_nFootnoteNumberingFormat )
		sResult += L"\\" + GetRtfFormat(m_nFootnoteNumberingFormat, true);
	
	RENDER_RTF_INT( m_nFootnoteStart, sResult, L"ftnstart" )
	
		if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage:		sResult += L"\\ftnrstpg" ;		break;
		case fr_EachSection:	sResult += L"\\ftnrestart";	break;
		case fr_Continuous:		sResult += L"\\ftnrstcont";	break;
		}
	}
	
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnoteRestart )
		{
		case fp_EndSection:		sResult += L"\\endnotes";	break;
		case fp_EndDocument:	sResult += L"\\enddoc";	break;
		case fp_BeneathText:	sResult += L"\\ftntj";		break;
		case fp_BottomPage:		sResult += L"\\ftnbj";		break;
		}
	}
	
	if( PROP_DEF != m_nEndnoteNumberingFormat )
		sResult += L"\\" + GetRtfFormat(m_nEndnoteNumberingFormat, false);

	RENDER_RTF_INT( m_nEndnoteStart, sResult, L"aftnstart" )
	
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case er_EachSection:	sResult += L"\\aftnrestart"; break;
		case er_Continuous:		sResult += L"\\aftnrstcont"; break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case ep_EndSection:		sResult += L"\\aendnotes"; break;
		case ep_EndDocument:	sResult += L"\\aenddoc";	break;
		case ep_BeneathText:	sResult += L"\\aftntj";	break;
		case ep_BottomPage:		sResult += L"\\aftnbj";	break;
		}
	}

//для схоранения в RTF
	RENDER_RTF_INT( m_bSplitPageBrake, sResult, L"spltpgpar" )

	if( 0 != m_bHtmlAutoSpace )
		sResult += L"\\htmautsp";

	return sResult;
}
std::wstring RtfDocumentProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( PROP_DEF != m_bHypCaps )
	{
		if( 0 == m_bHypCaps )
			sResult += L"<w:doNotHyphenateCaps w:val=\"true\"/>";
		else
			sResult += L"<w:doNotHyphenateCaps w:val=\"false\"/>";
	}
	RENDER_OOX_BOOL( m_bAutoHyp, sResult, L"w:autoHyphenation" )
	RENDER_OOX_INT( m_nMaxHypen, sResult, L"w:consecutiveHyphenLimit" )
	RENDER_OOX_INT( m_nHyphenationRight, sResult, L"w:hyphenationZone" )
	RENDER_OOX_INT( m_nTabWidth, sResult, L"w:defaultTabStop" )
	
	RENDER_OOX_INT( m_nDrawingGridHorizontalSpacing, sResult, L"w:drawingGridHorizontalSpacing" )
	RENDER_OOX_INT( m_nDrawingGridVerticalSpacing, sResult, L"w:drawingGridVerticalSpacing" )
	RENDER_OOX_INT( m_nDrawingGridHorizontalOrigin, sResult, L"w:drawingGridVerticalOrigin" )
	RENDER_OOX_INT( m_nDrawingGridVerticalOrigin, sResult, L"w:drawingGridVerticalOrigin" )
	RENDER_OOX_INT( m_nDisplayHorizontalDrawingGridEvery, sResult, L"w:displayHorizontalDrawingGridEvery" )
	RENDER_OOX_INT( m_nDisplayVerticalDrawingGridEvery, sResult, L"w:displayVerticalDrawingGridEvery" )

	if( PROP_DEF != m_nZoom )
        sResult += L"<w:zoom w:percent=\"" + std::to_wstring(m_nZoom) + L"\" />";
//Page Borders
	if( PROP_DEF != m_bDorderSurroundHeader )
	{
		if( 0 == m_bDorderSurroundHeader )
			sResult += L"<w:bordersDoNotSurroundHeader w:val=\"true\"/>";
		else
			sResult += L"<w:bordersDoNotSurroundHeader w:val=\"false\"/>";
	}
	if( PROP_DEF != m_bDorderSurroundFotter )
	{
		if( 0 == m_bDorderSurroundFotter )
			sResult += L"<w:bordersDoNotSurroundFooter w:val=\"true\"/>";
		else
			sResult += L"<w:bordersDoNotSurroundFooter w:val=\"false\"/>";
	}
	RENDER_OOX_BOOL( m_bAlignBordersAndEdges, sResult, L"w:alignBordersAndEdges" )

//Page Information
	RENDER_OOX_BOOL( m_bGutterAtTop,	sResult, L"w:gutterAtTop" )
	RENDER_OOX_BOOL( m_bSwitchMargins,	sResult, L"w:mirrorMargins" )
	RENDER_OOX_BOOL( m_bFacingPage,		sResult, L"w:evenAndOddHeaders" )

//Compatibility Options
	RENDER_OOX_BOOL( m_nDisplayBackground, sResult, L"w:displayBackgroundShape" );

//footnote, endnote property
    std::wstring sFootnoteProp;
	for (size_t i = 0; i < m_aSpecialFootnotes.size(); i++ )
	{
        sFootnoteProp += L"<w:footnote w:id=\"" + std::to_wstring(m_aSpecialFootnotes[i]) + L"\" />";
	}
	
	if( PROP_DEF != m_nFootnoteNumberingFormat )
	{
        sFootnoteProp += L"<w:numRestart w:val=\"";
		sFootnoteProp += RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat );
		sFootnoteProp += L"\"/>";
	}
	RENDER_OOX_INT( m_nFootnoteStart, sFootnoteProp, L"w:numStart" );
	
	if( PROP_DEF != m_eFootnoteRestart )
	{
		switch( m_eFootnoteRestart )
		{
		case fr_EachPage:		sFootnoteProp += L"<w:numRestart w:val=\"eachPage\"/>";		break;
		case fr_EachSection:	sFootnoteProp += L"<w:numRestart w:val=\"eachSect\"/>";		break;
		case fr_Continuous:		sFootnoteProp += L"<w:numRestart w:val=\"continuous\"/>";	break;
		}
	}
	if( PROP_DEF != m_eFootnotePlacement )
	{
		switch( m_eFootnotePlacement )
		{
		case fp_EndSection:		sFootnoteProp += L"<w:pos w:val=\"sectEnd\"/>";		break;
		case fp_EndDocument:	sFootnoteProp += L"<w:pos w:val=\"docEnd\"/>";		break;
		case fp_BeneathText:	sFootnoteProp += L"<w:pos w:val=\"beneathText\"/>";	break;
		case fp_BottomPage:		sFootnoteProp += L"<w:pos w:val=\"pageBottom\"/>";	break;
		}
	}
    if( !sFootnoteProp.empty() && !m_aSpecialFootnotes.empty())
        sResult += L"<w:footnotePr>" + sFootnoteProp + L"</w:footnotePr>";

    std::wstring sEndnoteProp;
    //todooo проверить id
    for (size_t i = 0; i < m_aSpecialEndnotes.size(); i++ )
	{
        sEndnoteProp += L"<w:endnote w:id=\"" + std::to_wstring(m_aSpecialEndnotes[i]) + L"\"/>";
	}
	if( PROP_DEF != m_nFootnoteNumberingFormat )
	{
        sFootnoteProp += L"<w:numRestart w:val=\"";
		sFootnoteProp += RtfListLevelProperty::GetFormat( m_nFootnoteNumberingFormat );
		sFootnoteProp += L"\"/>";
	}
	RENDER_OOX_INT( m_nEndnoteStart, sEndnoteProp, L"w:numStart" );
	
	if( PROP_DEF != m_eEndnoteRestart )
	{
		switch( m_eEndnoteRestart )
		{
		case fr_EachSection:	sEndnoteProp += L"<w:numRestart w:val=\"eachSect\"/>";		break;
		case fr_Continuous:		sEndnoteProp += L"<w:numRestart w:val=\"continuous\"/>";	break;
		}
	}
	if( PROP_DEF != m_eEndnotePlacement )
	{
		switch( m_eEndnotePlacement )
		{
		case fp_EndSection:		sEndnoteProp += L"<w:pos w:val=\"sectEnd\"/>";		break;
		case fp_EndDocument:	sEndnoteProp += L"<w:pos w:val=\"docEnd\"/>";		break;
		case fp_BeneathText:	sEndnoteProp += L"<w:pos w:val=\"beneathText\"/>";	break;
		case fp_BottomPage:		sEndnoteProp += L"<w:pos w:val=\"pageBottom\"/>";	break;
		}
	}
    if( !sEndnoteProp.empty() && !m_aSpecialEndnotes.empty())
        sResult += L"<w:endnotePr>" + sEndnoteProp + L"</w:endnotePr>";

    std::wstring sCompat = L"<w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"11\"/>";
	
	if( 1 != m_bHtmlAutoSpace )
		sCompat += L"<w:doNotUseHTMLParagraphAutoSpacing/>";
	if( 0 == m_bUseTabAlignment )
		sCompat += L"<w:forgetLastTabAlignment/>";
	
    if( !sCompat.empty() )
	{
        sResult += L"<w:compat>" + sCompat + L"</w:compat>";
	}

	return sResult;
}
std::wstring RtfSectionProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;

	if( RENDER_TO_OOX_PARAM_FIRST_SECTION == oRenderParameter.nType ) //Пишем как свойства документа
	{
		//Page Information
		RENDER_RTF_INT	( m_nPageWidth,			sResult, L"paperw" )
		RENDER_RTF_INT	( m_nPageHeight,		sResult, L"paperh" )
		RENDER_RTF_INT	( m_nMarginLeft,		sResult, L"margl" )
		RENDER_RTF_INT	( m_nMarginRight,		sResult, L"margr" )
		RENDER_RTF_INT	( m_nMarginTop,			sResult, L"margt" )
		RENDER_RTF_INT	( m_nMarginBottom,		sResult, L"margb" )
		RENDER_RTF_INT	( m_nGutterMarginWidth, sResult, L"gutter" )
		RENDER_RTF_BOOL	( m_bSwitchMargin,		sResult, L"margmirror" )
		RENDER_RTF_BOOL	( m_bLandscapeFormat,	sResult, L"landscape" )
	}

	sResult += L"\\sectd";
	RENDER_RTF_BOOL	( m_bBidi,				sResult, L"endnhere" )
	RENDER_RTF_INT	( m_nPaperSourceFirst,	sResult, L"binfsxn" )
	RENDER_RTF_INT	( m_nPaperSourceOther,	sResult, L"binsxn" )
	RENDER_RTF_BOOL	( m_bRtlGutter,			sResult, L"rtlgutter" )
	RENDER_RTF_BOOL	( m_bEndnotes,			sResult, L"endnhere" )
	RENDER_RTF_INT	( m_nStyle,				sResult, L"ds" )
	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbknone:	sResult += L"\\sbknone";	break;
		case sb_sbkcol:		sResult += L"\\sbkcol";		break;
		case sb_sbkpage:	sResult += L"\\sbkpage";	break;
		case sb_sbkeven:	sResult += L"\\sbkeven";	break;
		case sb_sbkodd:		sResult += L"\\sbkodd";		break;
	}
	//Columns
	RENDER_RTF_INT	( m_nColumnNumber,		sResult, L"cols" )
	RENDER_RTF_INT	( m_nColumnSpace,		sResult, L"colsx" )
	RENDER_RTF_BOOL	( m_bColumnLineBetween, sResult, L"linebetcol" )

	if (m_nColumnNumber > 1)
	{	
		for( size_t i = 0; i < m_oCollumnProperty.m_aCollumnProperty.size(); i++ )
		{
			sResult += L"\\colno" + std::to_wstring( (int)i + 1 );
			RENDER_RTF_INT( m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol, sResult, L"colsr" )
			RENDER_RTF_INT( m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth, sResult, L"colw" )
		}
	}

	//Footnotes and Endnotes
	switch( m_eFootnotesJust )
	{
		case fj_sftntj: sResult += L"\\sftntj";	break;
		case fj_sftnbj: sResult += L"\\sftnbj";	break;
	}
	RENDER_RTF_INT( m_nFootnotesStart, sResult, L"sftnstart" )
	
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg:		sResult += L"\\sftnrstpg";		break;
		case fr_sftnrestart:	sResult += L"\\sftnrestart";	break;
		case fr_sftnrstcont:	sResult += L"\\sftnrstcont";	break;
	}
	switch( m_eFootnotesFormat )
	{
		case ff_sftnnar:		sResult += L"\\sftnnar";		break;
		case ff_sftnnalc:		sResult += L"\\sftnnalc";		break;
		case ff_sftnnauc:		sResult += L"\\sftnnauc";		break;
		case ff_sftnnrlc:		sResult += L"\\sftnnrlc";		break;
		case ff_sftnnruc:		sResult += L"\\sftnnruc";		break;
		case ff_sftnnchi:		sResult += L"\\sftnnchi";		break;
		case ff_sftnnchosung:	sResult += L"\\sftnnchosung";	break;
		case ff_sftnncnum:		sResult += L"\\sftnncnum";		break;
		case ff_sftnndbnum:		sResult += L"\\sftnndbnum";		break;
		case ff_sftnndbnumd:	sResult += L"\\sftnndbnumd";	break;
		case ff_sftnndbnumt:	sResult += L"\\sftnndbnumt";	break;
		case ff_sftnndbnumk:	sResult += L"\\sftnndbnumk";	break;
		case ff_sftnndbar:		sResult += L"\\sftnndbar";		break;
		case ff_sftnnganada:	sResult += L"\\sftnnganada";	break;
		case ff_sftnngbnum:		sResult += L"\\sftnngbnum";		break;
		case ff_sftnngbnumd:	sResult += L"\\sftnngbnumd";	break;
		case ff_sftnngbnuml:	sResult += L"\\sftnngbnuml";	break;
		case ff_sftnngbnumk:	sResult += L"\\sftnngbnumk";	break;
		case ff_sftnnzodiac:	sResult += L"\\sftnnzodiac";	break;
		case ff_sftnnzodiacd:	sResult += L"\\sftnnzodiacd";	break;
		case ff_sftnnzodiacl:	sResult += L"\\sftnnzodiacl";	break;
	}
	RENDER_RTF_INT( m_nEndnotesStart, sResult, L"saftnstart" )
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart: sResult += L"\\saftnrestart";		break;
		case er_saftnrstcont: sResult += L"\\saftnrstcont";		break;
	}
	switch( m_eEndnotesFormat )
	{
		case ef_saftnnar:		sResult += L"\\saftnnar";		break;
		case ef_saftnnalc:		sResult += L"\\saftnnalc";		break;
		case ef_saftnnauc:		sResult += L"\\saftnnauc";		break;
		case ef_saftnnrlc:		sResult += L"\\saftnnrlc";		break;
		case ef_saftnnruc:		sResult += L"\\saftnnruc";		break;
		case ef_saftnnchi:		sResult += L"\\saftnnchi";		break;
		case ef_saftnnchosung:	sResult += L"\\saftnnchosung";	break;
		case ef_saftnncnum:		sResult += L"\\saftnncnum";		break;
		case ef_saftnndbnum:	sResult += L"\\saftnndbnum";	break;
		case ef_saftnndbnumd:	sResult += L"\\saftnndbnumd";	break;
		case ef_saftnndbnumt:	sResult += L"\\saftnndbnumt";	break;
		case ef_saftnndbnumk:	sResult += L"\\saftnndbnumk";	break;
		case ef_saftnndbar:		sResult += L"\\saftnndbar";		break;
		case ef_saftnnganada:	sResult += L"\\saftnnganada";	break;
		case ef_saftnngbnum:	sResult += L"\\saftnngbnum";	break;
		case ef_saftnngbnumd:	sResult += L"\\saftnngbnumd";	break;
		case ef_saftnngbnuml:	sResult += L"\\saftnngbnuml";	break;
		case ef_saftnngbnumk:	sResult += L"\\saftnngbnumk";	break;
		case ef_saftnnzodiac:	sResult += L"\\saftnnzodiac";	break;
		case ef_saftnnzodiacd:	sResult += L"\\saftnnzodiacd";	break;
		case ef_saftnnzodiacl:	sResult += L"\\saftnnzodiacl";	break;
	}

	//Line Numbering
	RENDER_RTF_INT( m_nLineModulus, sResult, L"linemod" )
	RENDER_RTF_INT( m_nLineX, sResult, L"linex" )
	RENDER_RTF_INT( m_nLineStart, sResult, L"linestarts" )
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart:	sResult += L"\\linerestart";break;
		case lnr_lineppage:		sResult += L"\\lineppage";	break;
		case lnr_linecont:		sResult += L"\\linecont";	break;
	}

	if( RENDER_TO_OOX_PARAM_FIRST_SECTION != oRenderParameter.nType )//Пишем как свойства секции
	{
		//Page Information
		RENDER_RTF_INT( m_nPageWidth,			sResult, L"pgwsxn" )
		RENDER_RTF_INT( m_nPageHeight,			sResult, L"pghsxn" )
		RENDER_RTF_INT( m_nMarginLeft,			sResult, L"marglsxn" )
		RENDER_RTF_INT( m_nMarginRight,			sResult, L"margrsxn" )
		RENDER_RTF_INT( m_nMarginTop,			sResult, L"margtsxn" )
		RENDER_RTF_INT( m_nMarginBottom,		sResult, L"margbsxn" )
		RENDER_RTF_INT( m_nGutterMarginWidth,	sResult, L"guttersxn" )
		RENDER_RTF_BOOL( m_bSwitchMargin,		sResult, L"margmirsxn" )
		RENDER_RTF_BOOL( m_bLandscapeFormat,	sResult, L"lndscpsxn" )
	}

	RENDER_RTF_BOOL	( m_bTitlePage,		sResult, L"titlepg" )
	RENDER_RTF_INT	( m_nHeaderTop,		sResult, L"headery" )
	RENDER_RTF_INT	( m_nFooterBottom,	sResult, L"footery" )

	//Page Numbers
	RENDER_RTF_INT	( m_nPageNumberStart,		sResult, L"pgnstarts" )
	RENDER_RTF_BOOL	( m_bPageNumberContinuos,	sResult, L"pgncont" )
	RENDER_RTF_BOOL	( m_bPageNumberRestart,		sResult, L"pgnrestart" )
	RENDER_RTF_INT	( m_nPageNumberX,			sResult, L"pgnx" )
	RENDER_RTF_INT	( m_nPageNumberY,			sResult, L"pgny" )
	
	switch( m_ePageNumberFormat )
	{
		case pnf_pgndec:	sResult += L"\\pgndec";		break;
		case pnf_pgnucrm:	sResult += L"\\pgnucrm";	break;
		case pnf_pgnlcrm:	sResult += L"\\pgnlcrm";	break;
	}
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt: sResult += L"\\vertalt";	break;
		case va_vertalb: sResult += L"\\vertalb";	break;
		case va_vertalc: sResult += L"\\vertalc";	break;
		case va_vertalj: sResult += L"\\vertalj";	break;

	}
	//Text Flow
	RENDER_RTF_INT( m_nTextFollow, sResult, L"stextflow" )

	//Page Borders
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += L"\\pgbrdrl";
		sResult += m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += L"\\pgbrdrt";
		sResult += m_oBorderTop.RenderToRtf( oRenderParameter );
	}
		
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += L"\\pgbrdrr";
		sResult += m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += L"\\pgbrdrb";
		sResult += m_oBorderBottom.RenderToRtf( oRenderParameter );
	}
	RENDER_RTF_INT( m_nBorderArt,		sResult, L"brdrart" )
	RENDER_RTF_INT( m_nBorderMeasure,	sResult, L"pgbrdropt" )
	RENDER_RTF_INT( m_nBorderAlign,		sResult, L"pgbrdrsnap" )

	//footer - header
	if( 0 != m_oHeaderRight )
	{
		sResult += L"{\\headerr";
		sResult += m_oHeaderRight->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}
	if( 0 != m_oHeaderLeft )
	{
		sResult += L"{\\headerl";
		sResult += m_oHeaderLeft->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}
	if( 0 != m_oHeaderFirst )
	{
		sResult += L"{\\headerf";
		sResult += m_oHeaderFirst->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}
	if( 0 != m_oFooterRight )
	{
		sResult += L"{\\footerr";
		sResult += m_oFooterRight->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}
	if( 0 != m_oFooterLeft )
	{
		sResult += L"{\\footerl";
		sResult += m_oFooterLeft->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}
	if( 0 != m_oFooterFirst )
	{
		sResult += L"{\\footerf";
		sResult += m_oFooterFirst->RenderToRtf( oRenderParameter );
		sResult += L"\\par";
		sResult += L"}";
	}

	RENDER_RTF_INT( m_nSrAuth,		sResult, L"srauth" )
	RENDER_RTF_INT( m_nSrDate,		sResult, L"srdate" )

	if (m_pOldSectionProp)
	{
        sResult += L"{\\*\\oldpprops\\pard";
		sResult += m_pOldSectionProp->RenderToRtf(oRenderParameter);
		sResult += L"}";
	}
	return sResult;
}
std::wstring RtfSectionProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);

    std::wstring sResult;
	sResult += L"<w:sectPr>";
	
	if( 1 == m_bBidi )
		sResult += L"<w:bidi/>";
	
    std::wstring sPaperSource;
	if( PROP_DEF != m_nPaperSourceFirst )
        sPaperSource += L" w:first=\"" + std::to_wstring(m_nPaperSourceFirst) + L"\"";
	if( PROP_DEF != m_nPaperSourceFirst )
        sPaperSource += L" w:other=\"" + std::to_wstring(m_nPaperSourceOther) + L"\"";
	
    if( !sPaperSource.empty() )
        sResult += L"<w:paperSrc " + sPaperSource + L"/>";
	
	if( 1 == m_bRtlGutter )
		sResult += L"<w:rtlGutter/>";

	//Section Break
	switch( m_eSectionBreak )
	{
		case sb_sbkcol:		sResult += L"<w:type w:val=\"nextColumn\"/>";	break;
		case sb_sbkpage:	sResult += L"<w:type w:val=\"nextPage\"/>";		break;
		case sb_sbkeven:	sResult += L"<w:type w:val=\"evenPage\"/>";		break;
		case sb_sbkodd:		sResult += L"<w:type w:val=\"oddPage\"/>";		break;
		case sb_sbknone:	
		default:
			sResult += L"<w:type w:val=\"continuous\"/>";				
			break;
	}
	//Columns
	if( PROP_DEF != m_nColumnNumber ) 
	{
        std::wstring sCollumnFormating;
		bool bEqualWidth = false;
		if( m_nColumnNumber != m_oCollumnProperty.m_aCollumnProperty.size() )
			bEqualWidth = true;
		else
			for (size_t i = 0; i < m_oCollumnProperty.m_aCollumnProperty.size(); i++ )
			{
				if( PROP_DEF == m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
				{
					bEqualWidth = true;
					break;
				}
			}
        sCollumnFormating += L"<w:cols w:num=\"" + std::to_wstring(m_nColumnNumber) + L"\"";

		if( PROP_DEF != m_nColumnSpace )
            sCollumnFormating += L" w:space=\"" + std::to_wstring(m_nColumnSpace) + L"\"";

		if (m_nColumnNumber > 1)
		{					
			if (PROP_DEF != m_bColumnLineBetween)
				sCollumnFormating += std::wstring(L" w:sep=\"") + (m_bColumnLineBetween ? L"1" : L"0") + L"\"";
			
			if( true == bEqualWidth )
				sCollumnFormating += L" w:equalWidth=\"true\"/>";
			else
			{
				sCollumnFormating += L" w:equalWidth=\"false\">";

				for (size_t i = 0; i < m_oCollumnProperty.m_aCollumnProperty.size(); i++ )
				{
					sCollumnFormating += L"<w:col";
					if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth )
						sCollumnFormating += L" w:w=\"" + std::to_wstring(m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnWidth) + L"\"";
					if( PROP_DEF != m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol )
						sCollumnFormating += L" w:space=\"" + std::to_wstring(m_oCollumnProperty.m_aCollumnProperty[i].m_nColumnSpaceToRightOfCol ) + L"\"";
					sCollumnFormating += L"/>";
				}
				sCollumnFormating += L"</w:cols>";
			}
		}
		else
		{
			sCollumnFormating += L"/>";
		}
        if( false == sCollumnFormating.empty() )
			sResult += sCollumnFormating;
	}

	//Footnotes and Endnotes
    std::wstring sFootnotesProp;
	switch( m_eFootnotesJust )
	{
		case fj_sftntj:	sFootnotesProp += L"<w:pos w:val=\"beneathText\"/>";	break;
		case fj_sftnbj:	sFootnotesProp += L"<w:pos w:val=\"pageBottom\"/>";		break;
	}
	RENDER_OOX_INT( m_nFootnotesStart, sFootnotesProp, L"w:numStart" )
	switch( m_eFootnotesRestart )
	{
		case fr_sftnrstpg:		sFootnotesProp += L"<w:numRestart w:val=\"eachPage\"/>";	break;
		case fr_sftnrestart:	sFootnotesProp += L"<w:numRestart w:val=\"eachSect\"/>";	break;
		case fr_sftnrstcont:	sFootnotesProp += L"<w:numRestart w:val=\"continuous\"/>";	break;
	}
	switch( m_eFootnotesFormat )
	{
		case ff_sftnnar:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnalc:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnauc:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnrlc:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnruc:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnchi:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnchosung:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnncnum:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnndbnum:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnndbnumd:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnndbnumt:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnndbnumk:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnndbar:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnganada:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnngbnum:		sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnngbnumd:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnngbnuml:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnngbnumk:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnzodiac:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnzodiacd:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ff_sftnnzodiacl:	sFootnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
	}
    if(!sFootnotesProp.empty() )
        sResult +=  L"<w:footnotePr>" + sFootnotesProp + L"</w:footnotePr>";

    std::wstring sEndnotesProp;
	RENDER_OOX_INT( m_nEndnotesStart, sEndnotesProp, L"w:numStart" )
	
	switch( m_eEndnotesRestart )
	{
		case er_saftnrestart:	sEndnotesProp += L"<w:numRestart w:val=\"eachPage\"/>";		break;
		case er_saftnrstcont:	sEndnotesProp += L"<w:numRestart w:val=\"continuous\"/>";	break;
	}
	switch( m_eEndnotesFormat )
	{
		case ef_saftnnar:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnalc:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnauc:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnrlc:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnruc:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnchi:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnchosung:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnncnum:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnndbnum:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnndbnumd:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnndbnumt:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnndbnumk:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnndbar:		sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnganada:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnngbnum:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnngbnumd:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnngbnuml:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnngbnumk:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnzodiac:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnzodiacd:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
		case ef_saftnnzodiacl:	sEndnotesProp += L"<w:numFmt w:val=\"upperLetter\" />";	break;
	}
    if(!sEndnotesProp.empty() )
        sResult += L"<w:endnotePr>" + sEndnotesProp + L"</w:endnotePr>";

	//Line Numbering
    std::wstring sLineNumbering;
	if( PROP_DEF != m_nLineModulus )
        sLineNumbering += L" w:countBy=\"" + std::to_wstring(m_nLineModulus) + L"\"";
	if( PROP_DEF != m_nLineX && 0 != m_nLineX)
        sLineNumbering += L" w:distance=\"" + std::to_wstring(m_nLineX) + L"\"";
	if( PROP_DEF != m_nLineStart )
        sLineNumbering += L" w:start=\"" + std::to_wstring(m_nLineStart) + L"\"";
	switch( m_eLineNumberRestart )
	{
		case lnr_linerestart:	sLineNumbering += L" w:restart=\"newSection\"";	break; 
		case lnr_lineppage:		sLineNumbering += L" w:restart=\"newPage\"";	break; 
		case lnr_linecont:		sLineNumbering += L" w:restart=\"continuous\"";	break; 
	}
    if( false == sLineNumbering.empty() )
        sResult += L"<w:lnNumType " + sLineNumbering+ L"/>";

	//Page Information
    std::wstring sPageSize;
	if( PROP_DEF != m_nPageWidth )
        sPageSize += L" w:w=\"" + std::to_wstring(m_nPageWidth) + L"\"";
	if( PROP_DEF != m_nPageHeight )
        sPageSize += L" w:h=\"" + std::to_wstring(m_nPageHeight) + L"\"";
	if( 1 == m_bLandscapeFormat )
        sPageSize += L" w:orient=\"landscape\"";
    if( false == sPageSize.empty() )
        sResult += L"<w:pgSz " + sPageSize + L"/>";

    std::wstring sMargin;
	if( PROP_DEF != m_nMarginLeft )
        sMargin += L" w:left=\"" + std::to_wstring(m_nMarginLeft) + L"\"";
	if( PROP_DEF != m_nMarginRight )
        sMargin += L" w:right=\"" + std::to_wstring(m_nMarginRight) + L"\"";
	if( PROP_DEF != m_nMarginTop )
        sMargin += L" w:top=\"" + std::to_wstring(m_nMarginTop) + L"\"";
	if( PROP_DEF != m_nMarginBottom )
        sMargin += L" w:bottom=\"" + std::to_wstring(m_nMarginBottom) + L"\"";
	if( PROP_DEF != m_nGutterMarginWidth )
        sMargin += L" w:gutter=\"" + std::to_wstring(m_nGutterMarginWidth) + L"\"";
	if( PROP_DEF != m_nHeaderTop )
        sMargin += L" w:header=\"" + std::to_wstring(m_nHeaderTop) + L"\"";
	if( PROP_DEF != m_nFooterBottom )
        sMargin += L" w:footer=\"" + std::to_wstring(m_nFooterBottom) + L"\"";
    if( false == sMargin.empty() )
        sResult += L"<w:pgMar " + sMargin + L"/>";
	if( 1 == m_bTitlePage )
		sResult += L"<w:titlePg/>";

	//Page Numbers
    std::wstring sPageNumber;
	if( PROP_DEF != m_nPageNumberStart )
        sPageNumber += L" w:start=\"" + std::to_wstring(m_nPageNumberStart) + L"\"";
    if( false == sPageNumber.empty() )
        sResult += L"<w:pgNumType " + sPageNumber + L"/>";
	//Vertical Alignment
	switch( m_eVerticalAlignment )
	{
		case va_vertalt:	sResult += L"<w:vAlign w:val=\"top\" />";		break;
		case va_vertalb:	sResult += L"<w:vAlign w:val=\"bottom\" />";	break;
		case va_vertalc:	sResult += L"<w:vAlign w:val=\"center\" />";	break;
		case va_vertalj:	sResult += L"<w:vAlign w:val=\"both\" />";		break;
	}
	//Text Flow
	switch( m_nTextFollow )
	{
		case 0: sResult += L"<w:textDirection w:val=\"lrTb\" />";	break;
		case 1: sResult += L"<w:textDirection w:val=\"tbRl\" />";	break;
		case 2: sResult += L"<w:textDirection w:val=\"lrBt\" />";	break;
		case 3: sResult += L"<w:textDirection w:val=\"rlTb\" />";	break;
		case 4: sResult += L"<w:textDirection w:val=\"lrTbV\" />";	break;
		case 5: sResult += L"<w:textDirection w:val=\"tbLrV\" />";	break;
	}
	//Page Borders
    std::wstring sBorders;
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_TAG;
	if( true == m_oBorderLeft.IsValid() )
	{
		oNewParam.sValue = L"w:left";
		sBorders += m_oBorderLeft.RenderToOOX( oNewParam );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		oNewParam.sValue = L"w:top";
		sBorders += m_oBorderTop.RenderToOOX( oNewParam ) ;
	}
	if( true == m_oBorderRight.IsValid() )
	{
		oNewParam.sValue = L"w:right";
		sBorders += m_oBorderRight.RenderToOOX( oNewParam );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		oNewParam.sValue = L"w:bottom";
		sBorders += m_oBorderBottom.RenderToOOX( oNewParam );
	}
    if( false == sBorders.empty() )
	{
		sResult += L"<w:pgBorders";
		if( PROP_DEF != m_nBorderMeasure )
		{
			switch( m_nBorderMeasure & 3 )
			{
				case 0: sResult += L" w:display=\"allPages\"";		break;
				case 1: sResult += L" w:display=\"firstPage\"";		break;
				case 2: sResult += L" w:display=\"notFirstPage\"";	break;
			}
			if( 0 != (m_nBorderMeasure & 8) )
				sResult += L" w:zOrder=\"back\"";
			else
				sResult += L" w:zOrder=\"front\"";
			if( 0 != (m_nBorderMeasure & 32) )
				sResult += L" w:offsetFrom=\"page\"";
			else
				sResult += L" w:offsetFrom=\"text\"";
		}
		sResult += L">";
		sResult += sBorders;
		sResult += L"</w:pgBorders>";
	}
	if( 0 != m_oHeaderLeft )
	{
        std::wstring sId = SaveFile(m_oHeaderLeft,oRenderParameter, true);
        sResult += L"<w:headerReference w:type=\"even\" r:id=\"" + sId + L"\"/>";
	}
	if( 0 != m_oHeaderFirst )
	{
        std::wstring sId = SaveFile(m_oHeaderFirst,oRenderParameter, true);
        sResult += L"<w:headerReference w:type=\"first\" r:id=\"" + sId + L"\"/>";
	}
	if( 0 != m_oHeaderRight )
	{
        std::wstring sId = SaveFile(m_oHeaderRight,oRenderParameter, true);
        sResult += L"<w:headerReference w:type=\"default\" r:id=\"" + sId + L"\"/>";
	}
	if( 0 != m_oFooterLeft )
	{
        std::wstring sId = SaveFile(m_oFooterLeft,oRenderParameter, false);
        sResult += L"<w:footerReference w:type=\"even\" r:id=\"" + sId + L"\"/>";
	}
	if( 0 != m_oFooterFirst )
	{
        std::wstring sId = SaveFile(m_oFooterFirst,oRenderParameter, false);
        sResult += L"<w:footerReference w:type=\"first\" r:id=\"" + sId + L"\"/>";
	}
	if( 0 != m_oFooterRight )
	{
        std::wstring sId = SaveFile(m_oFooterRight,oRenderParameter, false);
        sResult += L"<w:footerReference w:type=\"default\" r:id=\"" + sId + L"\"/>";
	}
	if (m_pOldSectionProp)
	{
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nSrAuth);
        std::wstring sDate(RtfUtility::convertDateTime(m_nSrDate).c_str());

		RenderParameter oRenderParameterNew = oRenderParameter;
		oRenderParameterNew.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		
		sResult += L"<w:sectPrChange w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			sResult += m_pOldSectionProp->RenderToOOX(oRenderParameterNew);
		sResult += L"</w:sectPrChange>";
	}
	sResult += L"</w:sectPr>";
	return sResult;
}
std::wstring RtfSectionProperty::SaveFile( TextItemContainerPtr oTarget, RenderParameter oRenderParameter, bool bHeader)
{
	OOXWriter* poOOXWriter		= static_cast<OOXWriter*>	( oRenderParameter.poWriter );
	RtfDocument* poRtfDocument	= static_cast<RtfDocument*> ( oRenderParameter.poDocument );

    std::wstring sFilename;
	if ( true == bHeader )
        sFilename += L"header" + std::to_wstring(poRtfDocument->m_oIdGenerator.Generate_HeaderNumber()) + L".xml";
	else
        sFilename += L"footer" + std::to_wstring(poRtfDocument->m_oIdGenerator.Generate_FooterNumber()) + L".xml";

    std::wstring sContent;
    std::wstring sRootName;
	if( true == bHeader )
		sRootName = L"w:hdr";
	else
		sRootName = L"w:ftr";
	sContent += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n";
    sContent += L"<";
	sContent += sRootName;
	sContent += L" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" \
xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 w16se wp14\">";

	RenderParameter oNewParameter = oRenderParameter;
	
	OOXRelsWriterPtr oNewRelsWr ( new OOXRelsWriter( sFilename, *poRtfDocument ) );
	poOOXWriter->m_oCustomRelsWriter.push_back( oNewRelsWr );
	oNewParameter.poRels = oNewRelsWr.get();
	
	sContent += oTarget->RenderToOOX(oNewParameter);
    sContent += L"</" + sRootName + L">";

    std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sContent);
	
    std::wstring sFilePath = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + sFilename;
	
	NSFile::CFileBinary file;
	
    if (file.CreateFile(sFilePath) == true)
	{            
        file.WriteFile((void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());
		file.CloseFile();
	}

    std::wstring sContentTarget = L"/word/"  +  sFilename;
    std::wstring srID;
	if( true == bHeader )
	{
		poOOXWriter->m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml", sContentTarget);
		srID = poOOXWriter->m_oDocRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header", sFilename );
	}
	else
	{
		poOOXWriter->m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml", sContentTarget);
		srID = poOOXWriter->m_oDocRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", sFilename );
	}
	return srID;
}

RtfSectionProperty::RtfSectionProperty()
{
	SetDefault();

	//footer - header
	m_oHeaderLeft	= TextItemContainerPtr();
	m_oHeaderFirst	= TextItemContainerPtr();
	m_oHeaderRight	= TextItemContainerPtr();
	m_oFooterLeft	= TextItemContainerPtr();
	m_oFooterFirst	= TextItemContainerPtr();
	m_oFooterRight	= TextItemContainerPtr();
}
int RtfSectionProperty::GetType()
{
	return TYPE_RTF_PROPERTY_SECTION;
}
void RtfSectionProperty::SetDefaultRtf()
{
	SetDefault();

	DEFAULT_PROPERTY_DEF( m_eSectionBreak, sb_sbkpage )
	DEFAULT_PROPERTY_DEF( m_nColumnSpace, 720);
}
void RtfSectionProperty::SetDefaultOOX()
{
	SetDefault();

	m_nPageWidth			= 11906;
	m_nPageHeight			= 16838;

	m_nMarginLeft			= 210;
	m_nMarginRight			= 215;
	m_nMarginTop			= 204;
	m_nMarginBottom			= 204;

	m_nHeaderTop			= 709;
	m_nFooterBottom			= 709;
	m_nGutterMarginWidth	= 0;

	m_nColumnSpace			= 708;
}
void RtfSectionProperty::SetDefault()
{
	DEFAULT_PROPERTY	( m_bBidi )
	DEFAULT_PROPERTY	( m_nPaperSourceFirst )
	DEFAULT_PROPERTY	( m_nPaperSourceOther )
	DEFAULT_PROPERTY	( m_bRtlGutter )
	DEFAULT_PROPERTY	( m_bEndnotes )
	DEFAULT_PROPERTY	( m_nStyle )
	DEFAULT_PROPERTY_DEF( m_eSectionBreak, sb_none )

	//Columns
	m_nColumnNumber = 1;
	DEFAULT_PROPERTY	( m_nColumnSpace )
	DEFAULT_PROPERTY	( m_bColumnLineBetween )

	//Footnotes and Endnotes
	DEFAULT_PROPERTY_DEF( m_eFootnotesJust,		fj_none )
	DEFAULT_PROPERTY	( m_nFootnotesStart )
	DEFAULT_PROPERTY_DEF( m_eFootnotesRestart,	fr_none )
	DEFAULT_PROPERTY_DEF( m_eFootnotesFormat,	ff_none )
	DEFAULT_PROPERTY	( m_nEndnotesStart )
	DEFAULT_PROPERTY_DEF( m_eEndnotesRestart,	er_none )
	DEFAULT_PROPERTY_DEF( m_eEndnotesFormat,	ef_none )

	//Line Numbering
	DEFAULT_PROPERTY	( m_nLineModulus )
	DEFAULT_PROPERTY	( m_nLineX )
	DEFAULT_PROPERTY	( m_nLineStart )
	DEFAULT_PROPERTY_DEF( m_eLineNumberRestart, lnr_none )

	//Page Information
	DEFAULT_PROPERTY	( m_nPageWidth )
	DEFAULT_PROPERTY	( m_nPageHeight )
	DEFAULT_PROPERTY	( m_nMarginLeft )
	DEFAULT_PROPERTY	( m_nMarginRight )
	DEFAULT_PROPERTY	( m_nMarginTop )
	DEFAULT_PROPERTY	( m_nMarginBottom )
	DEFAULT_PROPERTY	( m_nGutterMarginWidth )
	DEFAULT_PROPERTY	( m_bSwitchMargin )
	DEFAULT_PROPERTY	( m_bLandscapeFormat )
	DEFAULT_PROPERTY	( m_bTitlePage )
	DEFAULT_PROPERTY_DEF( m_nHeaderTop, 720 )
	DEFAULT_PROPERTY_DEF( m_nFooterBottom, 720 )

	//Page Numbers
	DEFAULT_PROPERTY	( m_nPageNumberStart )
	DEFAULT_PROPERTY	( m_bPageNumberContinuos )
	DEFAULT_PROPERTY	( m_bPageNumberRestart )
	DEFAULT_PROPERTY	( m_nPageNumberX )
	DEFAULT_PROPERTY	( m_nPageNumberY )
	DEFAULT_PROPERTY_DEF( m_ePageNumberFormat, pnf_none )

	//Vertical Alignment
	DEFAULT_PROPERTY_DEF( m_eVerticalAlignment, va_none )

	//Text Flow
	DEFAULT_PROPERTY	( m_nTextFollow )

	//Page Borders
	m_oBorderLeft.SetDefault();
	m_oBorderTop.SetDefault();
	m_oBorderRight.SetDefault();
	m_oBorderBottom.SetDefault();

	DEFAULT_PROPERTY	( m_nBorderArt )
	DEFAULT_PROPERTY	( m_nBorderMeasure )
	DEFAULT_PROPERTY	( m_nBorderAlign )

	//footer - header надо обнулять вручную
	//m_oHeaderLeft = TextItemContainerPtr();
	//m_oHeaderFirst = TextItemContainerPtr();
	//m_oHeaderRight = TextItemContainerPtr();
	//m_oFooterLeft = TextItemContainerPtr();
	//m_oFooterFirst = TextItemContainerPtr();
	//m_oFooterRight = TextItemContainerPtr();

	DEFAULT_PROPERTY( m_nSrAuth)
	DEFAULT_PROPERTY( m_nSrDate)

	m_pOldSectionProp = RtfSectionPropertyPtr();
}

RtfSectionProperty::ColumnProperty::ColumnProperty()
{
}
RtfSectionProperty::ColumnProperty::ColumnProperty( const RtfSectionProperty::ColumnProperty& oColumnProperty )
{
	(*this) = oColumnProperty;
}
RtfSectionProperty::ColumnProperty& RtfSectionProperty::ColumnProperty::operator=( const RtfSectionProperty::ColumnProperty& oColumnProperty )
{
	m_aCollumnProperty.clear();
	m_aCollumnProperty = oColumnProperty.m_aCollumnProperty ;
	return (*this);
}
void RtfSectionProperty::ColumnProperty::SetDefault()
{
	m_aCollumnProperty.clear();
}

RtfSection::RtfSection()
{
	m_bFinalize = false;
}
int RtfSection::GetType()
{
	return TYPE_RTF_SECTION;
}
std::wstring RtfSection::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult = m_oProperty.RenderToRtf(  oRenderParameter );

	if( RENDER_TO_RTF_PARAM_NO_SECT != oRenderParameter.nType )
		sResult += L"\\sectd";

	for( size_t i = 0; i < m_aArray.size(); i++ )
	{
		sResult += m_aArray[i]->RenderToRtf( oRenderParameter );
	}
	return sResult;
}
std::wstring RtfSection::RenderToOOX(RenderParameter oRenderParameter)
{
	if (!m_bFinalize) return L"";

	m_bFinalize = false; // тут .. чтобы не повторилось при конвертации колонтитулов

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	return m_oProperty.RenderToOOX(oNewParam);
}
