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
#pragma once
#include "Basic.h"
#include "RtfParagraph.h"

class RtfDocumentProperty: public IRenderableProperty
{
public: 
	enum _CodePage
	{
			cp_none,
			cp_ansi,	// ansi ANSI (the default)
			cp_mac,		// mac Apple Macintosh
			cp_pc,		// pc IBM PC code page 437
			cp_pca,		// pca IBM PC code page 850, used by IBM Personal System/2 (not implemented in version 1 of Microsoft Word for OS/2)
	}	m_eCodePage;

    int m_nAnsiCodePage;
	int m_nDefFont;
	int m_nDefMathFont;
	int m_nDefLang;
	int m_nDefLangAsian;
	
	int m_bRtlGutter; 
	int m_bRtl;
	
	int m_nThemelang;
	int m_nThemelangfe;
	int m_nThemelangcs;

	int m_bHypCaps;					//(doNotHyphenateCaps)\hyphcaps*	Switches hyphenation of capitalized words (default is on). Append 1 or leave control word by itself to toggle property on; append 0 to turn it off.
	int m_bAutoHyp;					//(autoHyphenation)\hyphauto*	Switches automatic hyphenation (default is off). Append 1 or leave control word by itself to toggle property on; append 0 to turn it off.
	int m_nMaxHypen;				//(consecutiveHyphenLimit)\hyphconsecN	N is maximum number of consecutive lines that are allowed to end in a hyphen. 0 means no limit.
	int m_nHyphenationRight;		//(hyphenationZone)\hyphhotzN	Hyphenation hot zone in twips (amount of space at right margin in which words are hyphenated).
	int m_nTabWidth;				//(defaultTabStop)\deftabN	Default tab width in twips (default is 720, i.e., 0.5").
	int m_nZoom;					// viewscaleN 	Zoom level of the document; the N argument is a value representing a percentage (default is 100).

	int m_nDrawingGridHorizontalSpacing;
	int m_nDrawingGridVerticalSpacing;
	int m_nDrawingGridHorizontalOrigin;
	int m_nDrawingGridVerticalOrigin;
	int m_nDisplayHorizontalDrawingGridEvery;	//def = 3
	int m_nDisplayVerticalDrawingGridEvery;		//def = 0

//Page Borders
	int m_bDorderSurroundHeader; //(bordersDoNotSurroundHeader)\pgbrdrhead	Page border surrounds header.
	int m_bDorderSurroundFotter; //(bordersDoNotSurroundFooter)\pgbrdrfoot	Page border surrounds footer.

	int m_bAlignBordersAndEdges; //(alignBordersAndEdges)\pgbrdrsnap	Align paragraph borders and table edges with page border.

//Page Information
	int m_nPaperWidth;			// paperwN	Paper width in twips (default is 12,240).
	int m_nPaperHeight;			// paperhN	Paper height in twips (default is 15,840).
	int m_nMarginLeft;			// marglN	Left margin in twips (default is 1800).
	int m_nMarginRight;			// margrN	Right margin in twips (default is 1800).
	int m_nMarginTop;			// margtN	Top margin in twips (default is 1440).
	int m_nMarginBottom;		// margbN	Bottom margin in twips (default is 1440).
	int m_bFacingPage;			//(mirrorMargins)\facingp	Facing pages (activates odd/even headers and gutters).
	int m_nGutterWidth;			// gutterN	Gutter width in twips (default is 0).
	int m_nGutterWidthOutside;	// ogutterN	Outside gutter width (default is 0; not used by Word, but in 1987 RTF Spec)
	int m_bGutterAtTop;			//(gutterAtTop)\gutterprl	Parallel gutter.

	int m_bSwitchMargins;		// margmirror	Switches margin definitions on left and right pages. Used in conjunction with \facingp.
	int m_bLandScape;			// landscape	Landscape format.
	int m_nBeginningPageNumber; // pgnstartN	Beginning page number (default is 1).

//Compatibility Options
	int m_nDisplayBackground;//(displayBackgroundShape)\viewbkspN	Boolean:0	Background shapes will not show in Page Layout View (default if omitted).1	Background shapes will show in Page Layout View.
	int m_bUseTabAlignment;// useltbaln	Do not forget last tab alignment.

//footnote, endnote property
	std::vector<int> m_aSpecialFootnotes;
	int m_nFootnoteNumberingFormat;
	int m_nFootnoteStart; // ftnstartN	Beginning footnote number (default is 1).
		
	enum _FootnoteRestart
	{ 
		fr_None,
		fr_EachPage, // ftnrstpg	Restart footnote numbering each page.
		fr_EachSection, // ftnrestart	Footnote numbers restart at each section. Microsoft Word for the Macintosh uses this control to restart footnote numbering at each page.
		fr_Continuous // ftnrstcont	Continuous footnote numbering (the default).
	}m_eFootnoteRestart;

	enum _FootnotePlacement
	{ 
		fp_None,
		fp_EndSection,		// endnotes	Footnotes at the end of the section (the default).
		fp_EndDocument,		// enddoc	Footnotes at the end of the document.
		fp_BeneathText,		// ftntj	Footnotes beneath text (top justified).
		fp_BottomPage		// ftnbj	Footnotes at the bottom of the page (bottom justified).
	} m_eFootnotePlacement;

	std::vector<int> m_aSpecialEndnotes;
	int m_nEndnoteNumberingFormat;
	int m_nEndnoteStart;				// aftnstartN	Beginning endnote number (default is 1).
		
	enum _EndnoteRestart
	{ 
		er_None,
		er_EachSection,		// aftnrestart	Restart endnote numbering each section.
		er_Continuous		// aftnrstcont	Continuous endnote numbering (the default).
	}m_eEndnoteRestart;
	
	enum _EndnotePlacement
	{ 
		ep_None,
		ep_EndSection,		// aendnotes	Endnotes at end of section (the default).
		ep_EndDocument,		// aenddoc	Endnotes at end of document.
		ep_BeneathText,		// aftntj	Endnotes beneath text (top justified).
		ep_BottomPage		// aftnbj	Endnotes at bottom of page (bottom justified).
	}m_eEndnotePlacement;

//для схоранения в RTF
	int m_bSplitPageBrake;	// spltpgpar
	int m_bHtmlAutoSpace;	// htmautsp	Use HTML paragraph auto spacing.

	RtfDocumentProperty();

	int GetType();
	bool IsValid();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);

private: 
	std::wstring GetRtfFormat( int nFormat, bool bFootnote );
};

class RtfSectionProperty;
typedef boost::shared_ptr<RtfSectionProperty>	RtfSectionPropertyPtr;

class RtfSectionProperty: public IRenderableProperty
{
//Bidirectional Controls
public: 
	int m_bBidi; // rtlsect	This section will snake (newspaper style) columns from right to left.
	int m_nPaperSourceFirst; // binfsxnN	N is the printer bin used for the first page of the section. If this control is not defined, then the first page uses the same printer bin as defined by the \binsxnN control.
	int m_nPaperSourceOther; // binsxnN	N is the printer bin used for the pages of the section.
	int m_bRtlGutter; // rtlgutter	Gutter is positioned on the right.

	int m_bEndnotes; // \endnhere	Endnotes included in the section.
	int m_nStyle; // \dsN	Designates section style. If a section style is specified, style properties must be specified with the section.

	//Section Break
	enum _SectionBreak{
		sb_none,
		sb_sbknone, // sbknone	No section break.
		sb_sbkcol, // sbkcol	Section break starts a new column.
		sb_sbkpage,  // sbkpage	Section break starts a new page (the default).
		sb_sbkeven,  // sbkeven	Section break starts at an even page.
		sb_sbkodd,  // sbkodd	Section break starts at an odd page.
	}m_eSectionBreak;

//Columns
	int m_nColumnNumber; // colsN	Number of columns for "snaking" (default is 1).
	int m_nColumnSpace;  // colsxN	Space between columns in twips (default is 720).
	int m_bColumnLineBetween; // linebetcol	Line between columns.
	
	class ColumnProperty
	{
	public: 
		struct CollumnVar
		{
			int m_nColumnSpaceToRightOfCol	= PROP_DEF;		// colsrN 	Space to right of column in twips; used to specify formatting for variable-width columns.
			int m_nColumnWidth				= PROP_DEF;		// colwN 	Width of column in twips; used to override the default constant width setting for variable-width columns.
		};

		std::vector< CollumnVar > m_aCollumnProperty;

		ColumnProperty();
		ColumnProperty( const ColumnProperty& oColumnProperty );

		ColumnProperty& operator=( const ColumnProperty& oColumnProperty );
		void SetDefault();
	};

	ColumnProperty m_oCollumnProperty;

//Footnotes and Endnotes
	enum _FootnotesJust
	{
		fj_none,
		fj_sftntj,			// sftntj	Footnotes beneath text (top justified).
		fj_sftnbj,			// sftnbj	Footnotes at the bottom of the page (bottom justified).
	}	m_eFootnotesJust;	
	
	int m_nFootnotesStart; // sftnstartN	Beginning footnote number (default is 1).
	
	enum _FootnotesRestart
	{
		fr_none,
		fr_sftnrstpg,		// sftnrstpg	Restart footnote numbering each page.
		fr_sftnrestart,		// sftnrestart	Footnote numbers restart at each section. Microsoft Word for the Macintosh uses this control to restart footnote numbering at each page.
		fr_sftnrstcont,		// sftnrstcont	Continuous footnote numbering (the default).
	}	m_eFootnotesRestart;
	
	enum _FootnotesFormat
	{
		ff_none,
		ff_sftnnar,			// sftnnar	Footnote numbering—Arabic numbering (1, 2, 3, …).
		ff_sftnnalc,		// sftnnalc	Footnote numbering—Alphabetical lowercase (a, b, c, …).
		ff_sftnnauc,		// sftnnauc	Footnote numbering—Alphabetical uppercase (A, B, C, …).
		ff_sftnnrlc,		// sftnnrlc	Footnote numbering—Roman lowercase (i, ii, iii, …).
		ff_sftnnruc,		// sftnnruc	Footnote numbering—Roman uppercase (I, II, III, …).
		ff_sftnnchi,		// sftnnchi	Footnote numbering—Chicago Manual of Style (*, †, ‡, §).
		ff_sftnnchosung,	// sftnnchosung	Footnote Korean numbering 1 (CHOSUNG).
		ff_sftnncnum,		// sftnncnum	Footnote Circle numbering (CIRCLENUM).
		ff_sftnndbnum,		// sftnndbnum	Footnote kanji numbering without the digit character (DBNUM1).
		ff_sftnndbnumd,		// sftnndbnumd	Footnote kanji numbering with the digit character (DBNUM2).
		ff_sftnndbnumt,		// sftnndbnumt	Footnote kanji numbering 3 (DBNUM3).
		ff_sftnndbnumk,		// sftnndbnumk	Footnote kanji numbering 4 (DBNUM4).
		ff_sftnndbar,		// sftnndbar	Footnote double-byte numbering (DBCHAR).
		ff_sftnnganada,		// sftnnganada	Footnote Korean numbering 2 (GANADA).
		ff_sftnngbnum,		// sftnngbnum	Footnote Chinese numbering 1 (GB1).
		ff_sftnngbnumd,		// sftnngbnumd	Footnote Chinese numbering 2 (GB2).
		ff_sftnngbnuml,		// sftnngbnuml	Footnote Chinese numbering 3 (GB3).
		ff_sftnngbnumk,		// sftnngbnumk	Footnote Chinese numbering 4 (GB4).
		ff_sftnnzodiac,		// sftnnzodiac	Footnote numbering—Chinese Zodiac numbering 1 (ZODIAC1).  
		ff_sftnnzodiacd,	// sftnnzodiacd	Footnote numbering—Chinese Zodiac numbering 2 (ZODIAC2). 
		ff_sftnnzodiacl,	// sftnnzodiacl	Footnote numbering—Chinese Zodiac numbering 3 (ZODIAC3).
	}	m_eFootnotesFormat;

	int m_nEndnotesStart; // saftnstartN	Beginning endnote number (default is 1).
	
	enum _EndnotesRestart
	{
		er_none,
		er_saftnrestart, // saftnrestart	Restart endnote numbering each section.
		er_saftnrstcont, // saftnrstcont	Continuous endnote numbering (the default).
	}	m_eEndnotesRestart;
		
	enum _EndnotesFormat
	{
		ef_none,
		ef_saftnnar,			// saftnnar	Endnote numbering—Arabic numbering (1, 2, 3, …).
		ef_saftnnalc,			// saftnnalc	Endnote numbering—Alphabetical lowercase (a, b, c, …).
		ef_saftnnauc,			// saftnnauc	Endnote numbering—Alphabetical uppercase (A, B, C, …).
		ef_saftnnrlc,			// saftnnrlc	Endnote numbering—Roman lowercase (i, ii, iii, …).
		ef_saftnnruc,			// saftnnruc	Endnote numbering—Roman uppercase (I, II, III, …).
		ef_saftnnchi,			// saftnnchi	Endnote numbering—Chicago Manual of Style (*, †, ‡, §).
		ef_saftnnchosung,		// saftnnchosung	Endnote Korean numbering 1 (CHOSUNG).
		ef_saftnncnum,			// saftnncnum	Endnote Circle numbering (CIRCLENUM).
		ef_saftnndbnum,			// saftnndbnum	Endnote kanji numbering without the digit character (DBNUM1).
		ef_saftnndbnumd,		// saftnndbnumd	Endnote kanji numbering with the digit character (DBNUM2).
		ef_saftnndbnumt,		// saftnndbnumt	Endnote kanji numbering 3 (DBNUM3).
		ef_saftnndbnumk,		// saftnndbnumk	Endnote kanji numbering 4 (DBNUM4).
		ef_saftnndbar,			// saftnndbar	Endnote double-byte numbering (DBCHAR).
		ef_saftnnganada,		// saftnnganada	Endnote Korean numbering 2 (GANADA).
		ef_saftnngbnum,			// saftnngbnum	Endnote Chinese numbering 1 (GB1).
		ef_saftnngbnumd,		// saftnngbnumd	Endnote Chinese numbering 2 (GB2).
		ef_saftnngbnuml,		// saftnngbnuml	Endnote Chinese numbering 3 (GB3).
		ef_saftnngbnumk,		// saftnngbnumk	Endnote Chinese numbering 4 (GB4).
		ef_saftnnzodiac,		// saftnnzodiac	Endnote numbering—Chinese Zodiac numbering 1 (ZODIAC1).  
		ef_saftnnzodiacd,		// saftnnzodiacd	Endnote numbering—Chinese Zodiac numbering 2 (ZODIAC2).  
		ef_saftnnzodiacl,		// saftnnzodiacl	Endnote numbering—Chinese Zodiac numbering 3 (ZODIAC3).
	}	m_eEndnotesFormat;

//Line Numbering
	int m_nLineModulus;			// linemodN	Line-number modulus amount to increase each line number (default is 1).
	int m_nLineX;				// linexN	Distance from the line number to the left text margin in twips (default is 360).The automatic distance is 0. 
	int m_nLineStart;			// linestartsN	Beginning line number (default is 1).
		
	enum _LineNumberRestart
	{
		lnr_none,
		lnr_linerestart,		// linerestart	Line numbers restart at \linestartsN value.
		lnr_lineppage,			// lineppage	Line numbers restart on each page.
		lnr_linecont,			// linecont	Line numbers continue from the preceding section.
	}	m_eLineNumberRestart; 


//Page Information
	int m_nPageWidth;			// pgwsxnN	N is the page width in twips. A \sectd resets the value to that specified by \paperwN in the document properties.
	int m_nPageHeight;			// pghsxnN	N is the page height in twips. A \sectd resets the value to that specified by \paperhN in the document properties.
	int m_nMarginLeft;			// marglsxnN	N is the left margin of the page in twips. A \sectd resets the value to that specified by \marglN in the document properties.
	int m_nMarginRight;			// margrsxnN	N is the right margin of the page in twips. A \sectd resets the value to that specified by \margrN in the document properties.
	int m_nMarginTop;			// margtsxnN	N is the top margin of the page in twips. A \sectd resets the value to that specified by \margtN in the document properties.
	int m_nMarginBottom;		// margbsxnN	N is the bottom margin of the page in twips. A \sectd resets the value to that specified by \margbN in the document properties.
	int m_nGutterMarginWidth;	// guttersxnN	N is the width of the gutter margin for the section in twips. A \sectd resets the value to that specified by \gutterN from the document properties. If Facing Pages is turned off, the gutter is added to the left margin of all pages. If Facing Pages is turned on, the gutter is added to the left side of odd-numbered pages and the right side of even-numbered pages.
	int m_bSwitchMargin;		// margmirsxn	Switches margin definitions on left and right pages. Used in conjunction with \facingp.
	int m_bLandscapeFormat;		// lndscpsxn	Page orientation is in landscape format. To mix portrait and landscape sections within a document, the \landscape control should not be used so that the default for a section is portrait, which may be overridden by the \lndscpsxn control.
	int m_bTitlePage;			// titlepg	First page has a special format.
	int m_nHeaderTop;			// headeryN	Header is N twips from the top of the page (default is 720).
	int m_nFooterBottom;		// footeryN	Footer is N twips from the bottom of the page (default is 720).

//Page Numbers
	int m_nPageNumberStart;		// pgnstartsN	Beginning page number (default is 1).
	int m_bPageNumberContinuos;	// pgncont	Continuous page numbering (the default).
	int m_bPageNumberRestart;	// pgnrestart	Page numbers restart at \pgnstarts value.
	int m_nPageNumberX;			// pgnxN	Page number is N twips from the right margin (default is 720). This control word is understood but not used by current versions (6.0 or later) of Word.
	int m_nPageNumberY;			// pgnyN	Page number is N twips from the top margin (default is 720). This control word is understood but not used by current versions (6.0 or later) of Word.

	enum _PageNumberFormat 
	{
		pnf_none,
		pnf_pgndec,			// pgndec	Page-number format is decimal.
		pnf_pgnucrm,		// pgnucrm	Page-number format is uppercase Roman numeral.
		pnf_pgnlcrm			// pgnlcrm	Page-number format is lowercase Roman numeral.
	}	m_ePageNumberFormat;

//Vertical Alignment
	enum _VerticalAlignment
	{
		va_none,
		va_vertalt,				// vertalt	Text is top-aligned (the default).
		va_vertalb,				// vertalb	Text is bottom-aligned. Note: Word uses \vertal.
		va_vertalc,				// vertalc	Text is centered vertically.
		va_vertalj				// vertalj	Text is justified vertically.
	}	m_eVerticalAlignment;

//Text Flow
	int m_nTextFollow;			// stextflowN	Section property for specifying text flow:0	Text flows left to right and top to bottom,1	Text flows top to bottom and right to left, vertical,2	Text flows left to right and bottom to top,3	Text flows right to left and top to bottom,4	Text flows left to right and top to bottom, vertical,5	Text flows top to bottom and left to right, vertical (for Mongolian)

//Page Borders
	RtfBorder m_oBorderLeft;
	RtfBorder m_oBorderTop;
	RtfBorder m_oBorderRight;
	RtfBorder m_oBorderBottom;
	int m_nBorderArt;			// brdrartN	Page border art; the N argument is a value from 1 through 165 representing the number of the border.
	int m_nBorderMeasure ;		// pgbrdroptN	8	Page border measure from text. Always display in front option is set to off.,32	Page border measure from edge of page. Always display in front option is set to on.,40	Page border measure from edge of page. Always display in front option is set to off.
	int m_nBorderAlign ;		// pgbrdrsnap	Align paragraph borders and table edges with page border.

//footer - header
	TextItemContainerPtr m_oHeaderLeft;
	TextItemContainerPtr m_oHeaderFirst;
	TextItemContainerPtr m_oHeaderRight;
	TextItemContainerPtr m_oFooterLeft;
	TextItemContainerPtr m_oFooterFirst;
	TextItemContainerPtr m_oFooterRight;

    std::wstring m_sIDHeaderLeft; //для OOX
    std::wstring m_sIDHeaderFirst;
    std::wstring m_sIDHeaderRight;
    std::wstring m_sIDFooterLeft;
    std::wstring m_sIDFooterFirst;
    std::wstring m_sIDFooterRight;

	int m_nSrAuth;
	int m_nSrDate;

	RtfSectionPropertyPtr m_pOldSectionProp;

	RtfSectionProperty();

	int GetType();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);

private: 
    std::wstring SaveFile( TextItemContainerPtr oTarget, RenderParameter oRenderParameter, bool bHeader);
};

typedef boost::shared_ptr<RtfDocumentProperty>	RtfDocumentPropertyPtr;

class RtfSection : public TextItemContainer
{
public: 
	RtfSectionProperty m_oProperty;

	bool m_bFinalize;

	RtfSection();
	int GetType();

	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfSection> RtfSectionPtr;
