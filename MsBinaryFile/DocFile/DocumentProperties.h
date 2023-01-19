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

#include "FileInformationBlock.h"
#include "VirtualStreamReader.h"
#include "DateAndTime.h"
#include "AutoSummaryInfo.h"
#include "DrawingObjectGrid.h"
#include "DocumentTypographyInfo.h"

#include "IVisitable.h"

namespace DocFileFormat
{
    class IVisible;
    class FileInformationBlock;

	class WordDocumentProperties: public IVisitable
	{
    public:
        virtual ~WordDocumentProperties();
        // Parses the bytes to retrieve a DocumentProperties
        WordDocumentProperties( FileInformationBlock* fib, POLE::Stream* tableStream );

        friend class SettingsMapping;
	private:    
        // True when facing pages should be printed
		bool fFacingPages;
        // True when window control is in effect
		bool fWindowControl;
        // true when doc is a main doc for Print Merge Helper
		bool fPMHMainDoc;
        // Default line suppression storage:
        // 0 form letter line supression
        // 1 no line supression
        // (no longer used)
		short grfSuppression;
        // Footnote position code:
        // 0 print as endnotes
        // 1 print as bottom of page
        // 2 print immediately beneath text
		short Fpc;
        // No longer used
		short grpfIhdt;
        // Restart index for footnotes:
        // 0 dont restart note numbering
        // 1 restart for each section
        // 2 restart for each page
		short rncFtn;
        // Initial footnote number for document
		short nFtn;
        // When true, indicates that information in the hplcpad should
        // be refreshed since outline has been dirtied
		bool fOutlineDirtySave;
        // When true, Word believes all pictures recorded in the
        // document were created on a Macintosh
		bool fOnlyMacPics;
        // When true, Word believes all pictures recorded in the
        // document were created in Windows
		bool fOnlyWinPics;
        // When true, document was created as a print
        // merge labels document
		bool fLabelDoc;
        // When true, Word is allowed to hyphenate words that are capitalized
		bool fHyphCapitals;
        // When true, Word will hyphenate newly typed
        // text as a background task
		bool fAutoHyphen;
		bool fFormNoFields;
        // When true, Word will merge styles from its template
		bool fLinkStyles;
        // Whent true, Word will mark revisions as the document is edited
		bool fRevMarking;
        // When true, always make backup when document saved
		bool fBackup;
        // When true, the results of the last Word Count execution are still exactly correct
		bool fExactWords;
        // When true, hidden documents contents are displayed
		bool fPagHidden;
        // When true, field results are displayed, when false, field codes are displayed
		bool fPagResults;
        // When true, annotations are locked for editing
		bool fLockAtn;
        // When true, swap margins on left/right pages
		bool fMirrorMargins;
        // When true, use TrueType fonts by default
        // (flag obeyed only when doc was created by WinWord 2.x)
		bool fDflttrueType;
        // When true, file created with SUPPRESSTOPSPACING=YES in Win.ini
        // (flag obeyed only when doc was created by WinWord 2.x)
		bool fPagSuppressTopSpacing;
        // When true, document is protected from edit operations
		bool fProtEnabled;
        // When true, restrict selections to occur only within form fields
		bool fDispFormFldSel;
        // When true, show revision markings on screen
		bool fRMView;
        // When true, show revision markings when document is printed
		bool fRMPrint;
        // When true, the current revision marking state is locked
		bool fLockRev;
        // When true, document contains embedded TrueType fonts
		bool fEmbedFonts;
        // Compatibility option: when true, dont add automatic tab
        // stops for hanging indent
		bool fNoTabForInd;
        // Compatibility option: when true, dont add extra space
        // for raised or lowered characters
		bool fNoSpaceRaiseLower;
        // Compatibility option: when true, suppress the paragraph
        // Space Before and Space After options after a page break
		bool fSuppressSpbfAfterPageBreak;
        // Compatibility option: when true, wrap trailing spaces
        // at the end of a line to the next line
		bool fWrapTrailSpaces;
        // Compatibility option: when true, print colors as black
        // on non-color printer
		bool fMapPrintTextColor;
        // Compatibility option: when true, dont balance columns
        // for Continuous Section starts
		bool fNoColumnBalance;
		bool fConvMailMergeEsc;
        // Compatibility option: when true, suppress extra line
        // spacing at top of page
		bool fSuppressTopSpacing;
        // Compatibility option: when true, combine table borders
        // like Word 5.x for the Macintosh
		bool fOrigWordTableRules;
        // Compatibility option: when true, dont blank area
        // between metafile pictures
		bool fTransparentMetafiles;
        // Compatibility option: when true, show hard page or
        // column breaks in frames
		bool fShowBreaksInFrames;
        // Compatibility option: when true, swap left and right
        // pages on odd facing pages
		bool fSwapBordersFacingPgs;
        // Default tab width
		unsigned short dxaTab;
        // Reserved
		unsigned short wSpare;
        // Width of hyphenation hot zone measured in twips
		unsigned short dxaHotZ;
        // Number of lines allowed to have consecutive hyphens
		unsigned short cConsecHypLim;
        // Reserved
		unsigned short wSpare2;
 		unsigned short wSpare3;
       // Date and time document was created
		DateAndTime dttmCreated;
        // Date and time document was last revised
		DateAndTime dttmRevised;
        // Date and time document was last printed
		DateAndTime dttmLastPrint;
        // Number of times document has ben revised since its creation
		short nRevision;
        // Time document was last edited
		int tmEdited;
        // Count of words tallied by last Word Count execution
		int cWords;
        // Count of characters tallied by the last Word Count execution
		int cCh;
        // Count of pages tallied by the last Word Count execution
		short cPg;
        // Count of paragraphs tallied by the last Word Count execution
		int cParas;
        // Restart endnote number code:
        // 0 dont restart endnote numbering
        // 1 restart for each section
        // 2 restart for each page
		short rncEdn;
        // Beginning endnote number
		short nEdn;
        // Endnote position code:
        // 0 display endnotes at end of section
        // 3 display endnotes at the end of document
		short Epc;
        // Number format code for auto footnotes.
        // Use the Number Format Table.
        // Note: Only the first 16 values in the table can be used.
		short nfcFtnRef;
        // Number format code for auto endnotes.
        // Use the Number Format Table.
        // Note: Only the first 16 values in the table can be used.
		short nfcEdnRef;
        // Only print data inside of form fields
		bool fPrintFormData;
        // Only save document data that is inside of a form field
		bool fSaveFormData;
        // Shade form fields
		bool fShadeFormData;
        // When true, include footnotes and endnotes in Word Count
		bool fWCFtnEdn;
        // Count of lines tallied by last Word Count operation
		int cLines;
        // Count of words in footnotes and endnotes tallied by last
        // word count operation
		int cWordsFtnEdn;
        // Count of characters in footnotes and endnotes tallied by last
        // word count operation
		int cChFtnEdn;
        // Count of pages in footnotes and endnotes tallied by last
        // word count operation
		int cPgFtnEdn;
        // Count of paragraphs in footnotes and endnotes tallied by last
        // word count operation
		int cParasFtnEdn;
        // Count of lines in footnotes and endnotes tallied by last
        // word count operation
		int cLinesFtnEdn;
        // Document protection password key only valid if
        // fProtEnabled, fLockAtn or fLockRev is true
		int lKeyProtDoc;
        // Document view kind
        // 0 Normal view
        // 1 Outline view
        // 2 Page view
		short wvkSaved;
        // Zoom percentage
		short wScaleSaved;
        // Zoom type:
        // 0 None
        // 1 Full page
        // 2 Page width
		short zkSaved;
        // This is a vertical document
        // (Word 6 and 96 only)
		bool fRotateFontW6;
        // Gutter position for this doc:
        // 0 Side
        // 1 Top
		bool iGutterPos;
        // SUpress extra line spacing at top of page like Word 5.x for the Macintosh
		bool fSuppressTopSpacingMac5;
        // Expand/Codense by whole number of points
		bool fTruncDxaExpand;
        // Print body text before header/footer
		bool fPrintBodyBeforeHdr;
        // dont add leading (extra space) between rows of text
		bool fNoLeading;
        // USer larger small caps like Word 5.x for the Macintosh
		bool fMWSmallCaps;
        // Autoformat document type:
        // 0 for normal
        // 1 for letter
        // 2 for email
		unsigned short adt;
		DocumentTypographyInfo* doptypography;
		DrawingObjectGrid* dogrid;
        // Which outline levels are showing in outline view:
        // 0 heading 1 only
        // 4 headings 1 through 5
        // 9 all levels showing
		short lvl;
        // Document has been completely grammar checked
		bool fGramAllDone;
        // No grammar errors exist in document
		bool fGramAllClean;
        // If you are doing font embedding, you should only embed
        // the characters in the font that are used in the document
		bool fSubsetFonts;
        // Hide the version created for auto version
		bool fHideLastVersion;
        // This file is based upon an HTML file
		bool fHtmlDoc;
        // Snap table and page borders to page border
		bool fSnapBorder;
        // Place header inside page border
		bool fIncludeHeader;
        // Place footer inside page border
		bool fIncludeFooter;
        // Are we in online view
		bool fForcePageSizePag;
        // Are we auto-promoting fonts to >= hpsZoonFontPag?
		bool fMinFontSizePag;
        // Versioning is turned on
		bool fHaveVersions;
        // Auto versioning is enabled
		bool fAutoVersion;
        // Auto summary info
		AutoSummaryInfo* asumyi;
        // Count of characters with spaces
		int cChWS;
        // Count of characters with spaces in footnotes and endnotes
		int cChWSFtnEdn;
		int grfDocEvents;
        // Have we prompted for virus protection on this document?
		bool fVirusPromted;
        // If prompted, load safely for this document?
		bool fVirusLoadSafe;
        // Random session key to sign above bits for a Word session
		int KeyVirusSession30;
        // Count of double unsigned char characters
		int cDBC;
        // Count of double unsigned char characters in footnotes and endnotes
		int cDBCFtnEdn;
        // Minimum font size if fMinFontSizePag is true
		short hpsZoonFontPag;
        // Height of the window in online view during last repagination
		short dywDispPag;
        // Used internally by Word
		unsigned char ilvlLastBulletMain;
        // Used internally by Word
		unsigned char ilvlLastNumberMain;
        // Default paragraph style for click and type
		short istdClickTypePara;
        // When set to true, language of all text in doc has been auto-detected
		bool fLADAllDone;
        // When set to true, envelope is visible.
		bool fEnvelopeVis;
        // When set to true, doc may have a tentative list in it
		bool fMaybeTentativeListInDoc;
        // When set to 1, doc may have fit text
		bool fMaybeFitText;
        // When set to true, rely on CSS for formatting
		bool fRelyOnCss_WebOpt;
        // When set to true, Rely on VML for displaying graphics in browsers
		bool fRelyOnVML_WebOpt;
        // When set to 1, allow PNG as an output format for graphics
		bool fAllowPNG_WebOpt;
        // Target monitor screen size
		short screenSize_WebOpt;
        // When set to 1, organize supporting files in a folder
		bool fOrganizeInFolder_WebOpt;
        // Use long file names for supporting files
		bool fUseLongFileNames_WebOpt;
        // Target monitor resolution in pixels per inch
		short iPixelsPerInch_WebOpt;
        // When set to 1, the web options have been filled in
		bool fWebOptionsInit;
        // When set to 1, the document may have East Asian layouts
		bool fMaybeFEL;
        // When set to 1, there may be character unit indents or line unit
		bool fCharLineUnits;
        // When set to 1, there may be RTL Tables in this document
		bool fMaybeRTLTables;
        // Compatibility option: when set to true, do not convert
        // backslash characters into yen signs
		bool fLeaveBackslashAlone;
        // Compatibility option: when set to true, expand character
        // spaces on the line ending SHIFT+RETURN
		bool fExpShRtn;
        // Compatibility option: when set to true, don?€?t underline trailing spaces
		bool fDntULTrlSpc;
        // Compatibility option: when set to true, dont balance SBCS and DBCS characters
		bool fDntBlnSbDbWid;
        // Compatibility option: when set to true, add space for underlines.
		bool fMakeSpaceForUL;
        // Compatibility option: suppress extra line spacing like WordPerfect
		bool f2ptExtLeadingOnly;
        // Compatibility option: when set to true, truncate font height
		bool fTruncFontHeight;
        // Compatibility option: when set to true, substitute fonts based on size.
		bool fSubOnSize;
        // Compatibility option: when set to true, lines wrap like Word 6.0
		bool fLineWrapLikeWord6;
        // Compatibility option: when set to true, use Word 6.0/95/97 border rules.
		bool fWW6BorderRules;
        // Compatibility option: when set to true, dont center "exact line height" lines
		bool fExactOnTop;
        // Compatibility option: when set to true, suppress extra line spacing at bottom of page
		bool fExtraAfter;
        // Compatibility option: when set to true, set the width of a space like WordPerfect 5
		bool fWPSpace;
        // Compatibility option: when set to true, do full justification like WordPerfect 6.x
		bool fWPJust;
        // Compatibility option: when set to true, use printer metrics to lay out the document
		bool fPrintMet;
        // Compatibility option: when set to true, lay AutoShapes like Word 97
		bool fSpLayoutLikeWW8;
        // Compatibility option: when set to true, lay footnotes like Word 6.x/95/97.
		bool fFtnLayoutLikeWW8;
        // Compatibility option: when set to true, dont use HTML paragraph auto spacing
		bool fDontUseHTMLParagraphAutoSpacing;
        // Compatibility option: when set to true, dont adjust line height in tables
		bool fDontAdjustLineHeightInTable;
        // Compatibility option: when set to 1, forget last tab alignment
		bool fForgetLastTabAlign;
        // Compatibility option: when set to 1, use auto space like Word 95
		bool fUseAutoSpaceForFullWidthAlpha;
        // Compatibility option: when set to 1, align table rows independently
		bool fAlignTablesRowByRow;
        // Compatibility option: when set to 1, lay out tables with raw width
		bool fLayoutRawTableWidth;
        // Compatibility option: when set to 1, allow table rows to lay out apart
		bool fLayoutTableRowsApart;
        // Compatibility option: when set to 1, use Word 97 line breaking rules for East Asian text
		bool fUserWord97LineBreakingRules;
        // Compatibility option: Do not break wrapped tables across pages.
		bool fDontBreakWrappedTables;
        // Compatibility option: Do not snap text to grid while in a table with inline objects.
		bool fDontSnapToGridInCell;
        // Compatibility option: Select the entire field with the first or last character
		bool fDontAllowFieldEndSelect;
        // Compatibility option: Apply breaking rules
		bool fApplyBreakingRules;
        // Compatibility option: Do not allow hanging punctuation with character grid
		bool fDontWrapTextWithPunct;
        // Compatibility option: Do not use Asian break rules for line breaks with character grid.
		bool fDontUseAsianBreakRules;
        // Compatibility option: Use the Word 2002 table style rules.
        // Word 2002 places the top border of a column under the heading row,
        // rather than above it as Word 2003 does.
        // Word 2003 applies the top border of a column in a more intuitive place when
        // there is a header row in the table. This new behavior also fixes an issue with
        // shading not displaying correctly for cells using conditional formatting.
		bool fUseWord2002TableStyleRules;
        // Compatibility option:
        // Allow tables set to autofit to contents – to extend into the margins when in Print Layout.
        // Word 2003 does not allow this by default.
		bool fGrowAutofit;
		//Specifies whether to not automatically apply the list paragraph style to bulleted or numbered text.
		bool fUseNormalStyleForList;
		//Specifies whether to ignore the hanging indent when creating a tab stop after numbering.
		bool fDontUseIndentAsNumberingTabStop;
		//Specifies whether to use an alternate set of East Asian line breaking rules.
		bool fFELineBreak11;
		//Specifies whether to allow contextual spacing of paragraphs in tables.
		bool fAllowSpaceOfSameStyleInTable;
		//Specifies whether to not ignore floating objects when calculating paragraph indentation.
		bool fWW11IndentRules;
		//Specifies whether to not autofit tables such that they fit next to wrapped objects.
		bool fDontAutofitConstrainedTables;
		//Specifies whether to allow table columns to exceed the preferred widths of the constituent cells.
		bool fAutofitLikeWW11;
		//Specifies whether to underline the tab following numbering when both the numbering and the first character of the numbered paragraph are underlined.
		bool fUnderlineTabInNumList;
		//Specifies whether to always use fixed width for Hangul characters.
		bool fHangulWidthLikeWW11;
		//Specifies whether to always move paragraph marks to the page after a page break.
		bool fSplitPgBreakAndParaMark;
		//Specifies whether to not vertically align cells containing floating objects.
		bool fDontVertAlignCellWithSp;
		//Specifies whether to not break table rows around floating tables.
		bool fDontBreakConstrainedForcedTables;
		//Specifies whether to ignore vertical alignment in text boxes.
		bool fDontVertAlignInTxbx;
		//Specifies whether to use ANSI kerning pairs from fonts instead of the Unicode kerning pair info.
		bool fWord11KerningPairs;
		//Specifies whether to use cached paragraph information for column balancing.
		bool fCachedColBalance;
        // HTML I/O compatibility level
		unsigned short verCompatPreW10;
        // Page view option
		bool fNoMargPgvwSaved;
        // Page view option
		bool fNoMargPgvWPag;
        // Web View option
		bool fWebViewPag;
		bool fSeeDrawingsPag;
        // this doc was produced by the document BulletProofer
		bool fBulletProofed;
        // this doc was doctored by the Document Corrupter
		bool fCorrupted;
        // Save option: Embed linguistic in the doc
		bool fSaveUim;
        // Save option: Remove personal information on save
		bool fFilterPrivacy;
        // we are under FReplace (and not just FReplaceRM)
		bool fInFReplaceNoRM;
        // The user has seen the repairs made to the document
		bool fSeenRepairs;
        // XML: The document has XML
		bool fHasXML;
		bool fSeeScriptAnchorsPag;
        // XML option: Validate XML on save
		bool fValidateXML;
        // XML option: Save the document even if the XML is invalid
		bool fSaveIfInvalidXML;
        // XML option: Show any errors in the XML
		bool fShowXMLErrors;
        // we imported an XML file that had no namespace, so we have elements with no namespace and no schema
		bool fAlwaysMergeEmptyNamespace;
		int cpMaxListCacheMainDoc;
        // Do not embed system fonts in this document
		bool fDoNotEmbedSystemFont;
		bool fWordCompact;
		bool fLiveRecover;
        // Embed smart tags in the document
		bool fEmbedFactoids;
        // Save smart tags as XML properties
		bool fFactoidXML;
        // Done processing smart tags
		bool fFactoidAllDone;
        // Print option: Book fold
		bool fFolioPrint;
        // Print option: Reverse book fold
		bool fReverseFolio;
		short iTextLineEnding;
        // Do not keep track of formatting
		bool fHideFcc;
        // Track changes: show markup
		bool fAcetateShowMarkup;
        // Track changes: show annotations
		bool fAcetateShowAtn;
        // Track changes: show insertions and deletions
		bool fAcetateShowInsDel;
        // Track changes: show formatting
		bool fAcetateShowProps;
        // Default table style for the document
		unsigned short istdTableDflt;
        // Internal: Version compatibility for save
		unsigned short verCompat;
        // Internal: filter state for the Styles and Formatting Pane.
		unsigned short grfFmtFilter;
        // Book fold printing: sheets per booklet
		short iFolioPages;
		unsigned short cpgText;
        // Revision mark CP info
		int cpMinRMText;
        // Revision mark CP info
		int cpMinRMFtn;
        // Revision mark CP info
		int cpMinRMHdd;
        // Revision mark CP info
		int cpMinRMAtn;
        // Revision mark CP info
		int cpMinRMEdn;
        // Revision mark CP info
		int cpMinRMTxbx;
        // Revision mark CP info
		int cpMinRMHdrTxbx;
		int rsidRoot;
        // Document Protection: Treat lock for annotations as Read Only
		bool fTreatLockAtnAsReadOnly;
        // Document Protection: Style lockdown is turned on
		bool fStyleLock;
        // Document Protection: Allow AutoFormat to override style lockdown
		bool fAutoFmtOverride;
        // XML Option: Remove Word XML when saving; save only non-Word XML data.
		bool fRemoveWordML;
        // XML Option: Apply custom transform on Save
		bool fApplyCustomXForm;
        // Document Protection: Style lockdown is enforced
		bool fStyeLockEnforced;
        // Document Protection: Simulate locked for annotations in
        // older version when a document has style protection
		bool fFakeLockAtn;
        // XML Option: Ignore mixed content
		bool fIgnoreMixedContent;
        // XML Option: Show placeholder text for all empty XML elements
		bool fShowPlaceholderText;

		bool fWord97Doc = false;
		bool fStyleLockTheme = false;
		bool fStyleLockQFSet = false;

        // Reading mode: ink lock down
		bool fReadingModeInkLockDown;
        // Track changes: Show ink annotations
		bool fAcetateShowInkAtn;
        // Filter date and time
		bool fFilterDttm;
        // Enforce document protection
		bool fEnforceDocProt;
        // Doc protection level:
        // 0 Protect for track changes
        // 1 Comment protection
        // 2 Form protection
        // 3 Read Only
		unsigned short iDocProtCur;
		bool fDispBkSpSaved;
        // Reading Layout page size lockdown
		int dxaPageLock;
        // Reading Layout page size lockdown
		int dyaPageLock;
        // Reading Layout font lockdown
		int pctFontLock;
		unsigned char grfitbid;
        // Number of LFOs when CleanupLists last attempted cleaning
		unsigned short ilfoMacAtCleanup;

		void setDefaultCompatibilityOptions( FibVersion nFib );
		void Initialize();
	};
}
