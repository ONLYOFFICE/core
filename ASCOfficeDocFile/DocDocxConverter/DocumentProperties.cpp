
#include "DocumentProperties.h"

namespace DocFileFormat
{
	    WordDocumentProperties::~WordDocumentProperties()
		{
          RELEASEOBJECT( doptypography );
          RELEASEOBJECT( dogrid );
          RELEASEOBJECT( asumyi );
		}

		/*========================================================================================================*/
		
		/// Parses the bytes to retrieve a DocumentProperties
		WordDocumentProperties::WordDocumentProperties(FileInformationBlock* fib, POLE::Stream* tableStream) : doptypography(NULL), dogrid(NULL), asumyi(NULL)
        {
			Initialize();
			
			VirtualStreamReader tStream( tableStream, fib->m_FibWord97.fcDop );
			
			//setDefaultCompatibilityOptions( fib->m_FibBase.nFib );

			unsigned int size = fib->m_FibWord97.lcbDop;
			unsigned char* bytes = tStream.ReadBytes( size, true );

            try
            {
                if ( size > 0 )
                {
                    //split unsigned char 0 and 1 into bits
                    fFacingPages = FormatUtils::GetBitFromBytes( bytes, 2, 0 );
                    fWindowControl = FormatUtils::GetBitFromBytes( bytes, 2, 1 );
                    fPMHMainDoc = FormatUtils::GetBitFromBytes( bytes, 2, 2 );
                    grfSuppression = (short)FormatUtils::GetUIntFromBytesBits( bytes, 2, 3, 2 );
                    Fpc = (short)(short)FormatUtils::GetUIntFromBytesBits( bytes, 2, 5, 2 );

                    //split unsigned char 2 and 3 into bits
                    rncFtn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 2 ), 2, 0, 2 );
                    nFtn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 2 ), 2, 2, 14 );

                    //split unsigned char 4 and 5 into bits
                    fOutlineDirtySave = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 0 );
                    fOnlyMacPics = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 8 );
                    fOnlyWinPics = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 9 );
                    fLabelDoc = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 10 );
                    fHyphCapitals = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 11 );
                    fAutoHyphen = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 12 );
                    fFormNoFields = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 13 );
                    fLinkStyles = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 14 );
                    fRevMarking = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 15 );

                    //split unsigned char 6 and 7 into bits
                    fBackup = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 0 );
                    fExactWords = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 1 );
                    fPagHidden = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 2 );
                    fPagResults = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 3 );
                    fLockAtn = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 4 );
                    fMirrorMargins = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 5 );
                    //bit 6 is reserved
                    fDflttrueType = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 7 );
                    fProtEnabled = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 8 );
                    fDispFormFldSel = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 9 );
                    fRMView = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 10 );
                    fRMPrint = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 11 );
                    //bit 12 and 13 are reserved
                    fLockRev = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 14 );
                    fEmbedFonts = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 15 );

                    //split unsigned char 8 and 9 into bits
                    fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 0 );
                    fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 1 );
                    fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 2 );
                    fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 3 );
                    fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 4 );
                    fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 5 );
                    fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 6 );
                    fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 7 );
                    fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 8 );
                    fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 9 );
                    fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 10 );
                    fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 11 );

                    dxaTab = FormatUtils::BytesToUInt16( bytes, 10, size );
                    dxaHotZ = FormatUtils::BytesToUInt16( bytes, 14, size );
                    cConsecHypLim = FormatUtils::BytesToUInt16( bytes, 16, size );

                    unsigned char createdbytes[4];
                    memcpy( createdbytes, ( bytes + 20 ), 4 );
                    dttmCreated = DateAndTime( createdbytes, 4 );

                    unsigned char revisedbytes[4];
                    memcpy( revisedbytes, ( bytes + 24 ), 4 );
                    dttmRevised = DateAndTime( revisedbytes, 4 );

                    unsigned char printbytes[4];
                    memcpy( printbytes, ( bytes + 28 ), 4 );
                    dttmLastPrint = DateAndTime( printbytes, 4 );

                    nRevision = FormatUtils::BytesToInt16( bytes, 32, size );
                    tmEdited = FormatUtils::BytesToInt32( bytes, 34, size );
                    cWords = FormatUtils::BytesToInt32( bytes, 38, size );
                    cCh = FormatUtils::BytesToInt32( bytes, 42, size );
                    cPg = FormatUtils::BytesToInt16( bytes, 46, size );
                    cParas = FormatUtils::BytesToInt32( bytes, 48, size );

                    //split unsigned char 52 and 53 into bits
                    rncEdn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 52 ), 2, 0, 2 );
                    nEdn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 52 ), 2, 2, 14 );

                    //split unsigned char 54 and 55 into bits
                    Epc = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 0, 2 );
                    nfcFtnRef = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 2, 4 );
                    nfcEdnRef = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 6, 4 );
                    fPrintFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 10 );
                    fSaveFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 11 );
                    fShadeFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 12 );
                    //bits 13 and 14 are reserved
                    fWCFtnEdn = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 15 );

                    cLines = FormatUtils::BytesToInt32( bytes, 56, size );
                    cWordsFtnEdn = FormatUtils::BytesToInt32( bytes, 60, size );
                    cChFtnEdn = FormatUtils::BytesToInt32( bytes, 64, size );
                    cPgFtnEdn = FormatUtils::BytesToInt16( bytes, 68, size );
                    cParasFtnEdn = FormatUtils::BytesToInt32( bytes, 70, size );
                    cLinesFtnEdn = FormatUtils::BytesToInt32( bytes, 74, size );
                    lKeyProtDoc = FormatUtils::BytesToInt32( bytes, 78, size );

                    //split unsigned char 82 and 83 into bits
                    wvkSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 0, 3 );
                    wScaleSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 3, 9 );
                    zkSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 12, 2 );
                    fRotateFontW6 = FormatUtils::GetBitFromBytes( ( bytes + 82 ), 2, 14 );
                    iGutterPos = FormatUtils::GetBitFromBytes( ( bytes + 82 ), 2, 15 );

                    //compatibility options section
                    if ( size > 84 )
                    {
                        //split unsigned char 84,85,86,87 into bits
                        fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 0 );
                        fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 1 );
                        fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 2 );
                        fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 3 );
                        fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 4 );
                        fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 5 );
                        fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 6 );
                        fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 7 );
                        fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 8 );
                        fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 9 );
                        fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 10 );
                        fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 11 );
                        //bits 12,13,14,15 are reserved
                        fSuppressTopSpacingMac5 = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 16 );
                        fTruncDxaExpand = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 17 );
                        fPrintBodyBeforeHdr = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 18 );
                        fNoLeading = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 19 );
                        //bits 20 is reserved
                        fMWSmallCaps = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 21 );
                        //bits 22-31 are reserved

                        if ( size > 88 )
                        {
                            adt = FormatUtils::BytesToUInt16( bytes, 88, size );

                            unsigned char doptypoBytes[310];
                            memcpy( doptypoBytes, ( bytes + 90 ), 310 );
                            doptypography = new DocumentTypographyInfo( doptypoBytes, 310 );

                            unsigned char dogridBytes[10];
                            memcpy( dogridBytes, ( bytes + 400 ), 10 );
                            dogrid = new DrawingObjectGrid( dogridBytes, 10 );

                            //split unsigned char 410 and 411 into bits
                            //bit 0 is reserved
                            lvl = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 410 ), 2, 1, 4 );
                            fGramAllDone = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 5 );
                            fGramAllClean = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 6 );
                            fSubsetFonts = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 7 );
                            fHideLastVersion = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 8 );
                            fHtmlDoc = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 9 );
                            //bit 10 is reserved
                            fSnapBorder = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 11 );
                            fIncludeHeader = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 12 );
                            fIncludeFooter = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 13 );
                            fForcePageSizePag = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 14 );
                            fMinFontSizePag = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 15 );

                            //split unsigned char 412 and 413 into bits
                            fHaveVersions = FormatUtils::GetBitFromBytes( ( bytes + 412 ), 2, 0 );
                            fAutoVersion = FormatUtils::GetBitFromBytes( ( bytes + 412 ), 2, 1 );
                            //other bits are reserved

                            unsigned char asumybits[12];
                            memcpy( asumybits, ( bytes + 414 ), 12 );
                            asumyi = new AutoSummaryInfo( asumybits, 12 );

                            cChWS = FormatUtils::BytesToInt32( bytes, 426, size );
                            cChWSFtnEdn = FormatUtils::BytesToInt32( bytes, 430, size );
                            grfDocEvents = FormatUtils::BytesToInt32( bytes, 434, size );

                            //split bytes 438-441 in bits
                            fVirusPromted = FormatUtils::GetBitFromBytes( ( bytes + 438 ), 4, 0 );
                            fVirusLoadSafe = FormatUtils::GetBitFromBytes( ( bytes + 438 ), 4, 1 );
                            KeyVirusSession30 = (int)FormatUtils::GetUIntFromBytesBits( ( bytes + 438 ), 4, 2, 30 );

                            cDBC = FormatUtils::BytesToInt32( bytes, 480, size );
                            cDBCFtnEdn = FormatUtils::BytesToInt32( bytes, 484, size );
                            nfcEdnRef = FormatUtils::BytesToInt16( bytes, 492, size );
                            nfcFtnRef = FormatUtils::BytesToInt16( bytes, 494, size );
                            hpsZoonFontPag = FormatUtils::BytesToInt16( bytes, 496, size );
                            dywDispPag = FormatUtils::BytesToInt16( bytes, 498, size );

                            //WORD 2000, 2002, 2003 PART
                            if ( size > 500 )
                            {
                                ilvlLastBulletMain = bytes[500];
                                ilvlLastNumberMain = bytes[501];
                                istdClickTypePara = FormatUtils::BytesToInt16( bytes, 502, size );

                                //split unsigned char 504 and 505 into bits
                                fLADAllDone = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 0 );
                                fEnvelopeVis = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 1 );
                                fMaybeTentativeListInDoc = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 2 );
                                fMaybeFitText = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 3 );
                                fRelyOnCss_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 9 );
                                fRelyOnVML_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 10 );
                                fAllowPNG_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 11 );
                                screenSize_WebOpt = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 504 ), 2, 12, 4 );

                                //split unsigned char 506 and 507 into bits
                                fOrganizeInFolder_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 0 );
                                fUseLongFileNames_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 1 );
                                iPixelsPerInch_WebOpt = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 506 ), 2, 2, 10 );
                                fWebOptionsInit = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 12 );
                                fMaybeFEL = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 13 );
                                fCharLineUnits = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 14 );
                                fMaybeRTLTables = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 15 );

                                //split bytes 508,509,510,511 into bits
                                fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 0 );
                                fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 1 );
                                fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 2 );
                                fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 3 );
                                fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 4 );
                                fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 5 );
                                fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 6 );
                                fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 7 );
                                fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 8 );
                                fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 9 );
                                fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 10 );
                                fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 11 );
                                fLeaveBackslashAlone = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 12 );
                                fExpShRtn = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 13 );
                                fDntULTrlSpc = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 14 );
                                fDntBlnSbDbWid = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 15 );
                                fSuppressTopSpacingMac5 = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 16 );
                                fTruncDxaExpand = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 17 );
                                fPrintBodyBeforeHdr = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 18 );
                                fNoLeading = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 19 );
                                fMakeSpaceForUL = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 20 );
                                fMWSmallCaps = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 21 );
                                f2ptExtLeadingOnly = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 22 );
                                fTruncFontHeight = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 23 );
                                fSubOnSize = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 24 );
                                fLineWrapLikeWord6 = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 25 );
                                fWW6BorderRules = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 26 );
                                fExactOnTop = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 27 );
                                fExtraAfter = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 28 );
                                fWPSpace = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 29 );
                                fWPJust = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 30 );
                                fPrintMet = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 31 );

                                //split bytes 512,513,514,515 into bits
                                fSpLayoutLikeWW8 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 0 );
                                fFtnLayoutLikeWW8 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 1 );
                                fDontUseHTMLParagraphAutoSpacing = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 2 );
                                fDontAdjustLineHeightInTable = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 3 );
                                fForgetLastTabAlign = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 4 );
                                fUseAutoSpaceForFullWidthAlpha = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 5 );
                                fAlignTablesRowByRow = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 6 );
                                fLayoutRawTableWidth = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 7 );
                                fLayoutTableRowsApart = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 8 );
                                fUserWord97LineBreakingRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 9 );
                                fDontBreakWrappedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 10 );
                                fDontSnapToGridInCell = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 11 );
                                fDontAllowFieldEndSelect = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 12 );
                                fApplyBreakingRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 13 );
                                fDontWrapTextWithPunct = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 14 );
                                fDontUseAsianBreakRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 15 );
                                fUseWord2002TableStyleRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 16 );
                                fGrowAutofit = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 17 );
                                fUseNormalStyleForList = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 18 );
                                fDontUseIndentAsNumberingTabStop = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 19 );
                                fFELineBreak11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 20 );
                                fAllowSpaceOfSameStyleInTable = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 21 );
                                fWW11IndentRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 22 );
                                fDontAutofitConstrainedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 23 );
                                fAutofitLikeWW11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 24 );
                                fUnderlineTabInNumList = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 25 );
                                fHangulWidthLikeWW11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 26 );
                                fSplitPgBreakAndParaMark = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 27 );
                                fDontVertAlignCellWithSp = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 28 );
                                fDontBreakConstrainedForcedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 29 );
                                fDontVertAlignInTxbx = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 30 );
                                fWord11KerningPairs = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 31 );
                                fCachedColBalance = FormatUtils::GetBitFromBytes( ( bytes + 516 ), 4, 0 );
                                
                                //bytes 517-539 are unused

                                //split bytes 540,541,542,543 into bits
                                verCompatPreW10 = (unsigned short)FormatUtils::GetUIntFromBytesBits( ( bytes + 540 ), 4, 0, 16 );
                                fNoMargPgvwSaved = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 16 );
                                fNoMargPgvWPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 17 );
                                fWebViewPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 18 );
                                fSeeDrawingsPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 19 );
                                fBulletProofed = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 20 );
                                fCorrupted = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 21 );
                                fSaveUim = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 22 );
                                fFilterPrivacy = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 23 );
                                fInFReplaceNoRM = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 24 );
                                fSeenRepairs = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 25 );
                                fHasXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 26 );
                                fSeeScriptAnchorsPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 27 );
                                fValidateXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 28 );
                                fSaveIfInvalidXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 29 );
                                fShowXMLErrors = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 30 );
                                fAlwaysMergeEmptyNamespace = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 31 );

                                cpMaxListCacheMainDoc = FormatUtils::BytesToInt32( bytes, 544, size );

                                //split bytes 548,549 into bits
                                fDoNotEmbedSystemFont = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 0 );
                                fWordCompact = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 1 );
                                fLiveRecover = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 2 );
                                fEmbedFactoids = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 3 );
                                fFactoidXML = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 4 );
                                fFactoidAllDone = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 5 );
                                fFolioPrint = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 6 );
                                fReverseFolio = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 7 );
                                iTextLineEnding = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 548 ), 2, 8, 3 );
                                fHideFcc = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 11 );
                                fAcetateShowMarkup = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 12 );
                                fAcetateShowAtn = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 13 );
                                fAcetateShowInsDel = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 14 );
                                fAcetateShowProps = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 15 );

                                istdTableDflt = FormatUtils::BytesToUInt16( bytes, 550, size );
                                verCompat = FormatUtils::BytesToUInt16( bytes, 552, size );
                                grfFmtFilter = FormatUtils::BytesToUInt16( bytes, 554, size );
                                iFolioPages = FormatUtils::BytesToInt16( bytes, 556, size );
                                cpgText = FormatUtils::BytesToUInt16( bytes, 558, size );
                                cpMinRMText = FormatUtils::BytesToInt32( bytes, 560, size );
                                cpMinRMFtn = FormatUtils::BytesToInt32( bytes, 564, size );
                                cpMinRMHdd = FormatUtils::BytesToInt32( bytes, 568, size );
                                cpMinRMAtn = FormatUtils::BytesToInt32( bytes, 572, size );
                                cpMinRMEdn = FormatUtils::BytesToInt32( bytes, 576, size );
                                cpMinRMTxbx = FormatUtils::BytesToInt32( bytes, 580, size );
                                cpMinRMHdrTxbx = FormatUtils::BytesToInt32( bytes, 584, size );
                                rsidRoot = FormatUtils::BytesToInt32( bytes, 588, size );

                                if ( size == 610 )
                                {
                                    //split bytes 592,593,594,595 into bits
                                    fTreatLockAtnAsReadOnly = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 0 );
                                    fStyleLock = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 1 );
                                    fAutoFmtOverride = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 2 );
                                    fRemoveWordML = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 3 );
                                    fApplyCustomXForm = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 4 );
                                    fStyeLockEnforced = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 5 );
                                    fFakeLockAtn = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 6 );
                                    fIgnoreMixedContent = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 7 );
                                    fShowPlaceholderText = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 8 );
                                    grf = FormatUtils::GetUIntFromBytesBits( ( bytes + 592 ), 4, 9, 23 );

                                    //split bytes 596 and 597 into bits
                                    fReadingModeInkLockDown = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 0 );
                                    fAcetateShowInkAtn = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 1 );
                                    fFilterDttm = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 2 );
                                    fEnforceDocProt = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 3 );
                                    iDocProtCur = (unsigned short)FormatUtils::GetUIntFromBytesBits( ( bytes + 596 ), 2, 4, 3 );
                                    fDispBkSpSaved = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 7 );

                                    dxaPageLock = FormatUtils::BytesToInt16( bytes, 598, size );
                                    dyaPageLock = FormatUtils::BytesToInt16( bytes, 600, size );
                                    pctFontLock = FormatUtils::BytesToInt32( bytes, 602, size );
                                    grfitbid = bytes[606];
                                    //unsigned char 607 is unused
                                    ilfoMacAtCleanup = FormatUtils::BytesToUInt16( bytes, 608, size );
                                }
                            }
                        }
                    }
                }
            }
            catch (/*Exception*/...)
            {
              //this DOP was probably not written by Word
              //TraceLogger.Warning("Unexpected length of DOP ({0} bytes) in input file.", (int)fib.lcbDop);
            }

			RELEASEARRAYOBJECTS( bytes );
        }

		/*========================================================================================================*/

		void WordDocumentProperties::setDefaultCompatibilityOptions( FibVersion nFib )
        {
            if( nFib == Fib1997 )
            {
                //Word 97 default settings
                fAlignTablesRowByRow = true;
                fLayoutTableRowsApart = true;
                fGrowAutofit = true;
                fDontWrapTextWithPunct = true;
                //ToDo: Don't autofit tables next to wrapped objects
                //ToDo: Don't break constrained tables forced onto the page
                fDontBreakWrappedTables = true;
                fDontSnapToGridInCell = true;
                fDontUseAsianBreakRules = true;
                fNoTabForInd = true;
                fDontUseHTMLParagraphAutoSpacing = true;
                fForgetLastTabAlign = true;
                fSpLayoutLikeWW8 = true;
                fFtnLayoutLikeWW8 = true;
                fLayoutRawTableWidth = true;
                fDontAllowFieldEndSelect = true;
                //ToDo: underline characters in numbered lists
                fUseWord2002TableStyleRules = true;
                fUserWord97LineBreakingRules = true;
            }
            else if ( nFib == Fib2000 )
            {
                //Word 2000 default settings

                fGrowAutofit = true;
                fDontWrapTextWithPunct = true;
                //ToDo: Don't autofit tables next to wrapped objects
                fDontBreakWrappedTables = true;
                fDontSnapToGridInCell = true;
                fDontUseAsianBreakRules = true;
                fNoTabForInd = true;
                fDontAllowFieldEndSelect = true;
                //ToDo: underline characters in numbered lists
                fUseWord2002TableStyleRules = true;
            }
            else if( nFib ==  Fib2002 )
            {
                //Word 2002 (XP)

                fGrowAutofit = true;
                //ToDo: Don't autofit tables next to wrapped objects
                fDontBreakWrappedTables = true;
                fNoTabForInd = true;
                fUseWord2002TableStyleRules = true;
            }
            else if ( nFib == Fib2003 )
            {
                //Word 2003

                //ToDo: Don't autofit tables next to wrapped objects
                fDontBreakWrappedTables = true;
                fNoTabForInd = true;

            }
            else if ( nFib < Fib1997 )
            {
                //throw new UnspportedFileVersionException();
            }
        }

		/*========================================================================================================*/

		void WordDocumentProperties::Initialize()
		{
          fFacingPages = false;
          fWindowControl = false;
          fPMHMainDoc = false;
          grfSuppression = 0;
          Fpc = 0;
          grpfIhdt = 0;
          rncFtn = 0;
          nFtn = 0;
          fOutlineDirtySave = false;
          fOnlyMacPics = false;
          fOnlyWinPics = false;
          fLabelDoc = false;
          fHyphCapitals = false;
          fAutoHyphen = false;
          fFormNoFields = false;
          fLinkStyles = false;
          fRevMarking = false;
          fBackup = false;
          fExactWords = false;
          fPagHidden = false;
          fPagResults = false;
          fLockAtn = false;
          fMirrorMargins = false;
          fDflttrueType = false;
          fPagSuppressTopSpacing = false;
          fProtEnabled = false;
          fDispFormFldSel = false;
          fRMView = false;
          fRMPrint = false;
          fLockRev = false;
          fEmbedFonts = false;
          fNoTabForInd = false;
          fNoSpaceRaiseLower = false;
          fSuppressSpbfAfterPageBreak = false;
          fWrapTrailSpaces = false;
          fMapPrintTextColor = false;
          fNoColumnBalance = false;
          fConvMailMergeEsc = false;
          fSuppressTopSpacing = false;
          fOrigWordTableRules = false;
          fTransparentMetafiles = false;
          fShowBreaksInFrames = false;
          fSwapBordersFacingPgs = false;
          dxaTab = 0;
          wSpare = 0;
          dxaHotZ = 0;
          cConsecHypLim = 0;
          wSpare2 = 0;
          nRevision = 0;
          tmEdited = 0;
          cWords = 0;
          cCh = 0;
          cPg = 0;
          cParas = 0;
          rncEdn = 0;
          nEdn = 0;
          Epc = 0;
          nfcFtnRef = 0;
          nfcEdnRef = 0;
          fPrintFormData = false;
          fSaveFormData = false;
          fShadeFormData = false;
          fWCFtnEdn = false;
          cLines = 0;
          cWordsFtnEdn = 0;
          cChFtnEdn = 0;
          cPgFtnEdn = 0;
          cParasFtnEdn = 0;
          cLinesFtnEdn = 0;
          lKeyProtDoc = 0;
          wvkSaved = 0;
          wScaleSaved = 0;
          zkSaved = 0;
          fRotateFontW6 = false;
          iGutterPos = false;
          fSuppressTopSpacingMac5 = false;
          fTruncDxaExpand = false;
          fPrintBodyBeforeHdr = false;
          fNoLeading = false;
          fMWSmallCaps = false;
          adt = 0;
          doptypography = NULL;
          dogrid = NULL;
          lvl = 0;
          fGramAllDone = false;
          fGramAllClean = false;
          fSubsetFonts = false;
          fHideLastVersion = false;
          fHtmlDoc = false;
          fSnapBorder = false;
          fIncludeHeader = false;
          fIncludeFooter = false;
          fForcePageSizePag = false;
          fMinFontSizePag = false;
          fHaveVersions = false;
          fAutoVersion = false;
          asumyi = NULL;
          cChWS = 0;
          cChWSFtnEdn = 0;
          grfDocEvents = 0;
          fVirusPromted = false;
          fVirusLoadSafe = false;
          KeyVirusSession30 = 0;
          cDBC = 0;
          cDBCFtnEdn = 0;
          hpsZoonFontPag = 0;
          dywDispPag = 0;
          ilvlLastBulletMain = 0;
          ilvlLastNumberMain = 0;
          istdClickTypePara = 0;
          fLADAllDone = false;
          fEnvelopeVis = false;
          fMaybeTentativeListInDoc = false;
          fMaybeFitText = false;
          fRelyOnCss_WebOpt = false;
          fRelyOnVML_WebOpt = false;
          fAllowPNG_WebOpt = false;
          screenSize_WebOpt = 0;
          fOrganizeInFolder_WebOpt = false;
          fUseLongFileNames_WebOpt = false;
          iPixelsPerInch_WebOpt = 0;
          fWebOptionsInit = false;
          fMaybeFEL = false;
          fCharLineUnits = false;
          fMaybeRTLTables = false;
          fLeaveBackslashAlone = false;
          fExpShRtn = false;
          fDntULTrlSpc = false;
          fDntBlnSbDbWid = false;
          fMakeSpaceForUL = false;
          f2ptExtLeadingOnly = false;
          fTruncFontHeight = false;
          fSubOnSize = false;
          fLineWrapLikeWord6 = false;
          fWW6BorderRules = false;
          fExactOnTop = false;
          fExtraAfter = false;
          fWPSpace = false;
          fWPJust = false;
          fPrintMet = false;
          fSpLayoutLikeWW8 = false;
          fFtnLayoutLikeWW8 = false;
          fDontUseHTMLParagraphAutoSpacing = false;
          fDontAdjustLineHeightInTable = false;
          fForgetLastTabAlign = false;
          fUseAutoSpaceForFullWidthAlpha = false;
          fAlignTablesRowByRow = false;
          fLayoutRawTableWidth = false;
          fLayoutTableRowsApart = false;
          fUserWord97LineBreakingRules = false;
          fDontBreakWrappedTables = false;
          fDontSnapToGridInCell = false;
          fDontAllowFieldEndSelect = false;
          fApplyBreakingRules = false;
          fDontWrapTextWithPunct = false;
          fDontUseAsianBreakRules = false;
          fUseWord2002TableStyleRules = false;
          fGrowAutofit = false;
          fUseNormalStyleForList = false;
          fDontUseIndentAsNumberingTabStop = false;
          fFELineBreak11 = false;
          fAllowSpaceOfSameStyleInTable = false;
          fWW11IndentRules = false;
          fDontAutofitConstrainedTables = false;
          fAutofitLikeWW11 = false;
          fUnderlineTabInNumList = false;
          fHangulWidthLikeWW11 = false;
          fSplitPgBreakAndParaMark = false;
          fDontVertAlignCellWithSp = false;
          fDontBreakConstrainedForcedTables = false;
          fDontVertAlignInTxbx = false;
          fWord11KerningPairs = false;
          fCachedColBalance = false;
          verCompatPreW10 = 0;
          fNoMargPgvwSaved = false;
          fNoMargPgvWPag = false;
          fWebViewPag = false;
          fSeeDrawingsPag = false;
          fBulletProofed = false;
          fCorrupted = false;
          fSaveUim = false;
          fFilterPrivacy = false;
          fInFReplaceNoRM = false;
          fSeenRepairs = false;
          fHasXML = false;
          fSeeScriptAnchorsPag = false;
          fValidateXML = false;
          fSaveIfInvalidXML = false;
          fShowXMLErrors = false;
          fAlwaysMergeEmptyNamespace = false;
          cpMaxListCacheMainDoc = 0;
          fDoNotEmbedSystemFont = false;
          fWordCompact = false;
          fLiveRecover = false;
          fEmbedFactoids = false;
          fFactoidXML = false;
          fFactoidAllDone = false;
          fFolioPrint = false;
          fReverseFolio = false;
          iTextLineEnding = false;
          fHideFcc = false;
          fAcetateShowMarkup = false;
          fAcetateShowAtn = false;
          fAcetateShowInsDel = false;
          fAcetateShowProps = false;
          istdTableDflt = 0;
          verCompat = 0;
          grfFmtFilter = 0;
          iFolioPages = 0;
          cpgText = 0;
          cpMinRMText = 0;
          cpMinRMFtn = 0;
          cpMinRMHdd = 0;
          cpMinRMAtn = 0;
          cpMinRMEdn = 0;
          cpMinRMTxbx = 0;
          cpMinRMHdrTxbx = 0;
          rsidRoot = 0;
          fTreatLockAtnAsReadOnly = false;
          fStyleLock = false;
          fAutoFmtOverride = false;
          fRemoveWordML = false;
          fApplyCustomXForm = false;
          fStyeLockEnforced = false;
          fFakeLockAtn = false;
          fIgnoreMixedContent = false;
          fShowPlaceholderText = false;
          grf = 0;
          fReadingModeInkLockDown = false;
          fAcetateShowInkAtn = false;
          fFilterDttm = false;
          fEnforceDocProt = false;
          iDocProtCur = 0;
          fDispBkSpSaved = false;
          dxaPageLock = 0;
          dyaPageLock = 0;
          pctFontLock = 0;
          grfitbid = 0;
          ilfoMacAtCleanup = 0;
		}
}
//кодировка 1251
