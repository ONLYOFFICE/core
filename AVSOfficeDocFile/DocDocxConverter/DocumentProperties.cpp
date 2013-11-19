#include "stdafx.h"
#include "DocumentProperties.h"

namespace DocFileFormat
{
	    WordDocumentProperties::~WordDocumentProperties()
		{
		  RELEASEOBJECT( this->doptypography );
          RELEASEOBJECT( this->dogrid );
          RELEASEOBJECT( this->asumyi );
		}

		/*========================================================================================================*/
		
		/// Parses the bytes to retrieve a DocumentProperties
		WordDocumentProperties::WordDocumentProperties(FileInformationBlock* fib, IStream* tableStream) : doptypography(NULL), dogrid(NULL), asumyi(NULL)
        {
			Initialize();
			
			VirtualStreamReader tStream( tableStream, fib->m_FibWord97.fcDop );
			
			//setDefaultCompatibilityOptions( fib->m_FibBase.nFib );

			unsigned int size = fib->m_FibWord97.lcbDop;
			byte* bytes = tStream.ReadBytes( size, true );

            try
            {
                if ( size > 0 )
                {
                    //split byte 0 and 1 into bits
                    this->fFacingPages = FormatUtils::GetBitFromBytes( bytes, 2, 0 );
                    this->fWindowControl = FormatUtils::GetBitFromBytes( bytes, 2, 1 );
                    this->fPMHMainDoc = FormatUtils::GetBitFromBytes( bytes, 2, 2 );
					this->grfSuppression = (short)FormatUtils::GetUIntFromBytesBits( bytes, 2, 3, 2 );
                    this->Fpc = (short)(short)FormatUtils::GetUIntFromBytesBits( bytes, 2, 5, 2 );

                    //split byte 2 and 3 into bits
                    this->rncFtn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 2 ), 2, 0, 2 );
                    this->nFtn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 2 ), 2, 2, 14 );

                    //split byte 4 and 5 into bits
                    this->fOutlineDirtySave = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 0 );
                    this->fOnlyMacPics = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 8 );
                    this->fOnlyWinPics = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 9 );
                    this->fLabelDoc = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 10 );
                    this->fHyphCapitals = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 11 );
                    this->fAutoHyphen = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 12 );
                    this->fFormNoFields = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 13 );
                    this->fLinkStyles = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 14 );
                    this->fRevMarking = FormatUtils::GetBitFromBytes( ( bytes + 4 ), 2, 15 );

                    //split byte 6 and 7 into bits
                    this->fBackup = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 0 );
                    this->fExactWords = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 1 );
                    this->fPagHidden = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 2 );
                    this->fPagResults = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 3 );
                    this->fLockAtn = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 4 );
                    this->fMirrorMargins = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 5 );
                    //bit 6 is reserved
                    this->fDflttrueType = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 7 );
                    this->fProtEnabled = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 8 );
                    this->fDispFormFldSel = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 9 );
                    this->fRMView = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 10 );
                    this->fRMPrint = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 11 );
                    //bit 12 and 13 are reserved
                    this->fLockRev = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 14 );
                    this->fEmbedFonts = FormatUtils::GetBitFromBytes( ( bytes + 6 ), 2, 15 );

                    //split byte 8 and 9 into bits
                    this->fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 0 );
                    this->fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 1 );
                    this->fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 2 );
                    this->fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 3 );
                    this->fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 4 );
                    this->fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 5 );
                    this->fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 6 );
                    this->fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 7 );
                    this->fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 8 );
                    this->fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 9 );
                    this->fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 10 );
                    this->fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 11 );

					this->dxaTab = FormatUtils::BytesToUInt16( bytes, 10, size );
                    this->dxaHotZ = FormatUtils::BytesToUInt16( bytes, 14, size );
                    this->cConsecHypLim = FormatUtils::BytesToUInt16( bytes, 16, size );

                    byte createdbytes[4];
                    memcpy( createdbytes, ( bytes + 20 ), 4 );
                    this->dttmCreated = DateAndTime( createdbytes, 4 );

                    byte revisedbytes[4];
                    memcpy( revisedbytes, ( bytes + 24 ), 4 );
                    this->dttmRevised = DateAndTime( revisedbytes, 4 );

                    byte printbytes[4];
                    memcpy( printbytes, ( bytes + 28 ), 4 );
                    this->dttmLastPrint = DateAndTime( printbytes, 4 );

					this->nRevision = FormatUtils::BytesToInt16( bytes, 32, size );
					this->tmEdited = FormatUtils::BytesToInt32( bytes, 34, size );
                    this->cWords = FormatUtils::BytesToInt32( bytes, 38, size );
                    this->cCh = FormatUtils::BytesToInt32( bytes, 42, size );
                    this->cPg = FormatUtils::BytesToInt16( bytes, 46, size );
                    this->cParas = FormatUtils::BytesToInt32( bytes, 48, size );

                    //split byte 52 and 53 into bits
                    this->rncEdn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 52 ), 2, 0, 2 );
                    this->nEdn = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 52 ), 2, 2, 14 );

                    //split byte 54 and 55 into bits
                    this->Epc = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 0, 2 );
                    this->nfcFtnRef = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 2, 4 );
                    this->nfcEdnRef = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 54 ), 2, 6, 4 );
                    this->fPrintFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 10 );
                    this->fSaveFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 11 );
                    this->fShadeFormData = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 12 );
                    //bits 13 and 14 are reserved
                    this->fWCFtnEdn = FormatUtils::GetBitFromBytes( ( bytes + 54 ), 2, 15 );

                    this->cLines = FormatUtils::BytesToInt32( bytes, 56, size );
                    this->cWordsFtnEdn = FormatUtils::BytesToInt32( bytes, 60, size );
                    this->cChFtnEdn = FormatUtils::BytesToInt32( bytes, 64, size );
                    this->cPgFtnEdn = FormatUtils::BytesToInt16( bytes, 68, size );
                    this->cParasFtnEdn = FormatUtils::BytesToInt32( bytes, 70, size );
                    this->cLinesFtnEdn = FormatUtils::BytesToInt32( bytes, 74, size );
                    this->lKeyProtDoc = FormatUtils::BytesToInt32( bytes, 78, size );

                    //split byte 82 and 83 into bits
                    this->wvkSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 0, 3 );
                    this->wScaleSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 3, 9 );
                    this->zkSaved = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 82 ), 2, 12, 2 );
                    this->fRotateFontW6 = FormatUtils::GetBitFromBytes( ( bytes + 82 ), 2, 14 );
                    this->iGutterPos = FormatUtils::GetBitFromBytes( ( bytes + 82 ), 2, 15 );

                    //compatibility options section
                    if ( size > 84 )
                    {
                        //split byte 84,85,86,87 into bits
                        this->fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 0 );
                        this->fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 1 );
                        this->fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 2 );
                        this->fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 3 );
                        this->fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 4 );
                        this->fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 5 );
                        this->fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 6 );
                        this->fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 7 );
                        this->fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 8 );
                        this->fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 9 );
                        this->fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 10 );
                        this->fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 11 );
                        //bits 12,13,14,15 are reserved
                        this->fSuppressTopSpacingMac5 = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 16 );
                        this->fTruncDxaExpand = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 17 );
                        this->fPrintBodyBeforeHdr = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 18 );
                        this->fNoLeading = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 19 );
                        //bits 20 is reserved
                        this->fMWSmallCaps = FormatUtils::GetBitFromBytes( ( bytes + 84 ), 4, 21 );
                        //bits 22-31 are reserved

                        if ( size > 88 )
                        {
                            this->adt = FormatUtils::BytesToUInt16( bytes, 88, size );

                            byte doptypoBytes[310];
                            memcpy( doptypoBytes, ( bytes + 90 ), 310 );
                            this->doptypography = new DocumentTypographyInfo( doptypoBytes, 310 );

                            byte dogridBytes[10];
                            memcpy( dogridBytes, ( bytes + 400 ), 10 );
                            this->dogrid = new DrawingObjectGrid( dogridBytes, 10 );

                            //split byte 410 and 411 into bits
                            //bit 0 is reserved
                            this->lvl = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 410 ), 2, 1, 4 );
                            this->fGramAllDone = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 5 );
                            this->fGramAllClean = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 6 );
                            this->fSubsetFonts = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 7 );
                            this->fHideLastVersion = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 8 );
                            this->fHtmlDoc = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 9 );
                            //bit 10 is reserved
                            this->fSnapBorder = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 11 );
                            this->fIncludeHeader = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 12 );
                            this->fIncludeFooter = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 13 );
                            this->fForcePageSizePag = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 14 );
                            this->fMinFontSizePag = FormatUtils::GetBitFromBytes( ( bytes + 410 ), 2, 15 );

                            //split byte 412 and 413 into bits
                            this->fHaveVersions = FormatUtils::GetBitFromBytes( ( bytes + 412 ), 2, 0 );
                            this->fAutoVersion = FormatUtils::GetBitFromBytes( ( bytes + 412 ), 2, 1 );
                            //other bits are reserved

                            byte asumybits[12];
                            memcpy( asumybits, ( bytes + 414 ), 12 );
                            this->asumyi = new AutoSummaryInfo( asumybits, 12 );

                            this->cChWS = FormatUtils::BytesToInt32( bytes, 426, size );
                            this->cChWSFtnEdn = FormatUtils::BytesToInt32( bytes, 430, size );
                            this->grfDocEvents = FormatUtils::BytesToInt32( bytes, 434, size );

                            //split bytes 438-441 in bits
                            this->fVirusPromted = FormatUtils::GetBitFromBytes( ( bytes + 438 ), 4, 0 );
                            this->fVirusLoadSafe = FormatUtils::GetBitFromBytes( ( bytes + 438 ), 4, 1 );
                            this->KeyVirusSession30 = (int)FormatUtils::GetUIntFromBytesBits( ( bytes + 438 ), 4, 2, 30 );

                            this->cDBC = FormatUtils::BytesToInt32( bytes, 480, size );
                            this->cDBCFtnEdn = FormatUtils::BytesToInt32( bytes, 484, size );
                            this->nfcEdnRef = FormatUtils::BytesToInt16( bytes, 492, size );
                            this->nfcFtnRef = FormatUtils::BytesToInt16( bytes, 494, size );
                            this->hpsZoonFontPag = FormatUtils::BytesToInt16( bytes, 496, size );
                            this->dywDispPag = FormatUtils::BytesToInt16( bytes, 498, size );

                            //WORD 2000, 2002, 2003 PART
                            if ( size > 500 )
                            {
                                this->ilvlLastBulletMain = bytes[500];
                                this->ilvlLastNumberMain = bytes[501];
                                this->istdClickTypePara = FormatUtils::BytesToInt16( bytes, 502, size );

                                //split byte 504 and 505 into bits
                                this->fLADAllDone = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 0 );
                                this->fEnvelopeVis = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 1 );
                                this->fMaybeTentativeListInDoc = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 2 );
                                this->fMaybeFitText = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 3 );
                                this->fRelyOnCss_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 9 );
                                this->fRelyOnVML_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 10 );
                                this->fAllowPNG_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 504 ), 2, 11 );
                                this->screenSize_WebOpt = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 504 ), 2, 12, 4 );

                                //split byte 506 and 507 into bits
                                this->fOrganizeInFolder_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 0 );
                                this->fUseLongFileNames_WebOpt = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 1 );
                                this->iPixelsPerInch_WebOpt = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 506 ), 2, 2, 10 );
                                this->fWebOptionsInit = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 12 );
                                this->fMaybeFEL = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 13 );
                                this->fCharLineUnits = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 14 );
                                this->fMaybeRTLTables = FormatUtils::GetBitFromBytes( ( bytes + 506 ), 2, 15 );

                                //split bytes 508,509,510,511 into bits
                                this->fNoTabForInd = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 0 );
                                this->fNoSpaceRaiseLower = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 1 );
                                this->fSuppressSpbfAfterPageBreak = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 2 );
                                this->fWrapTrailSpaces = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 3 );
                                this->fMapPrintTextColor = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 4 );
                                this->fNoColumnBalance = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 5 );
                                this->fConvMailMergeEsc = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 6 );
                                this->fSuppressTopSpacing = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 7 );
                                this->fOrigWordTableRules = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 8 );
                                this->fTransparentMetafiles = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 9 );
                                this->fShowBreaksInFrames = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 10 );
                                this->fSwapBordersFacingPgs = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 11 );
                                this->fLeaveBackslashAlone = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 12 );
                                this->fExpShRtn = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 13 );
                                this->fDntULTrlSpc = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 14 );
                                this->fDntBlnSbDbWid = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 15 );
                                this->fSuppressTopSpacingMac5 = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 16 );
                                this->fTruncDxaExpand = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 17 );
                                this->fPrintBodyBeforeHdr = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 18 );
                                this->fNoLeading = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 19 );
                                this->fMakeSpaceForUL = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 20 );
                                this->fMWSmallCaps = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 21 );
                                this->f2ptExtLeadingOnly = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 22 );
                                this->fTruncFontHeight = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 23 );
                                this->fSubOnSize = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 24 );
                                this->fLineWrapLikeWord6 = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 25 );
                                this->fWW6BorderRules = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 26 );
                                this->fExactOnTop = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 27 );
                                this->fExtraAfter = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 28 );
                                this->fWPSpace = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 29 );
                                this->fWPJust = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 30 );
                                this->fPrintMet = FormatUtils::GetBitFromBytes( ( bytes + 508 ), 4, 31 );

                                //split bytes 512,513,514,515 into bits
                                this->fSpLayoutLikeWW8 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 0 );
                                this->fFtnLayoutLikeWW8 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 1 );
                                this->fDontUseHTMLParagraphAutoSpacing = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 2 );
                                this->fDontAdjustLineHeightInTable = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 3 );
                                this->fForgetLastTabAlign = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 4 );
                                this->fUseAutoSpaceForFullWidthAlpha = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 5 );
                                this->fAlignTablesRowByRow = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 6 );
                                this->fLayoutRawTableWidth = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 7 );
                                this->fLayoutTableRowsApart = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 8 );
                                this->fUserWord97LineBreakingRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 9 );
                                this->fDontBreakWrappedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 10 );
                                this->fDontSnapToGridInCell = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 11 );
                                this->fDontAllowFieldEndSelect = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 12 );
                                this->fApplyBreakingRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 13 );
                                this->fDontWrapTextWithPunct = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 14 );
                                this->fDontUseAsianBreakRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 15 );
                                this->fUseWord2002TableStyleRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 16 );
                                this->fGrowAutofit = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 17 );
								this->fUseNormalStyleForList = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 18 );
								this->fDontUseIndentAsNumberingTabStop = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 19 );
								this->fFELineBreak11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 20 );
								this->fAllowSpaceOfSameStyleInTable = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 21 );
								this->fWW11IndentRules = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 22 );
								this->fDontAutofitConstrainedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 23 );
								this->fAutofitLikeWW11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 24 );
								this->fUnderlineTabInNumList = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 25 );
							    this->fHangulWidthLikeWW11 = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 26 );
								this->fSplitPgBreakAndParaMark = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 27 );
								this->fDontVertAlignCellWithSp = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 28 );
								this->fDontBreakConstrainedForcedTables = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 29 );
								this->fDontVertAlignInTxbx = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 30 );
								this->fWord11KerningPairs = FormatUtils::GetBitFromBytes( ( bytes + 512 ), 4, 31 );
								this->fCachedColBalance = FormatUtils::GetBitFromBytes( ( bytes + 516 ), 4, 0 );
                                
                                //bytes 517-539 are unused

                                //split bytes 540,541,542,543 into bits
                                this->verCompatPreW10 = (unsigned short)FormatUtils::GetUIntFromBytesBits( ( bytes + 540 ), 4, 0, 16 );
                                this->fNoMargPgvwSaved = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 16 );
                                this->fNoMargPgvWPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 17 );
                                this->fWebViewPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 18 );
                                this->fSeeDrawingsPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 19 );
                                this->fBulletProofed = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 20 );
                                this->fCorrupted = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 21 );
                                this->fSaveUim = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 22 );
                                this->fFilterPrivacy = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 23 );
                                this->fInFReplaceNoRM = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 24 );
                                this->fSeenRepairs = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 25 );
                                this->fHasXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 26 );
                                this->fSeeScriptAnchorsPag = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 27 );
                                this->fValidateXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 28 );
                                this->fSaveIfInvalidXML = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 29 );
                                this->fShowXMLErrors = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 30 );
                                this->fAlwaysMergeEmptyNamespace = FormatUtils::GetBitFromBytes( ( bytes + 540 ), 4, 31 );

                                this->cpMaxListCacheMainDoc = FormatUtils::BytesToInt32( bytes, 544, size );

                                //split bytes 548,549 into bits
                                this->fDoNotEmbedSystemFont = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 0 );
                                this->fWordCompact = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 1 );
                                this->fLiveRecover = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 2 );
                                this->fEmbedFactoids = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 3 );
                                this->fFactoidXML = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 4 );
                                this->fFactoidAllDone = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 5 );
                                this->fFolioPrint = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 6 );
                                this->fReverseFolio = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 7 );
                                this->iTextLineEnding = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 548 ), 2, 8, 3 );
                                this->fHideFcc = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 11 );
                                this->fAcetateShowMarkup = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 12 );
                                this->fAcetateShowAtn = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 13 );
                                this->fAcetateShowInsDel = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 14 );
                                this->fAcetateShowProps = FormatUtils::GetBitFromBytes( ( bytes + 548 ), 2, 15 );

                                this->istdTableDflt = FormatUtils::BytesToUInt16( bytes, 550, size );
                                this->verCompat = FormatUtils::BytesToUInt16( bytes, 552, size );
                                this->grfFmtFilter = FormatUtils::BytesToUInt16( bytes, 554, size );
                                this->iFolioPages = FormatUtils::BytesToInt16( bytes, 556, size );
                                this->cpgText = FormatUtils::BytesToUInt16( bytes, 558, size );
                                this->cpMinRMText = FormatUtils::BytesToInt32( bytes, 560, size );
                                this->cpMinRMFtn = FormatUtils::BytesToInt32( bytes, 564, size );
                                this->cpMinRMHdd = FormatUtils::BytesToInt32( bytes, 568, size );
                                this->cpMinRMAtn = FormatUtils::BytesToInt32( bytes, 572, size );
                                this->cpMinRMEdn = FormatUtils::BytesToInt32( bytes, 576, size );
                                this->cpMinRMTxbx = FormatUtils::BytesToInt32( bytes, 580, size );
                                this->cpMinRMHdrTxbx = FormatUtils::BytesToInt32( bytes, 584, size );
                                this->rsidRoot = FormatUtils::BytesToInt32( bytes, 588, size );

                                if ( size == 610 )
                                {
                                    //split bytes 592,593,594,595 into bits
                                    this->fTreatLockAtnAsReadOnly = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 0 );
                                    this->fStyleLock = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 1 );
                                    this->fAutoFmtOverride = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 2 );
                                    this->fRemoveWordML = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 3 );
                                    this->fApplyCustomXForm = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 4 );
                                    this->fStyeLockEnforced = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 5 );
                                    this->fFakeLockAtn = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 6 );
                                    this->fIgnoreMixedContent = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 7 );
                                    this->fShowPlaceholderText = FormatUtils::GetBitFromBytes( ( bytes + 592 ), 4, 8 );
                                    this->grf = FormatUtils::GetUIntFromBytesBits( ( bytes + 592 ), 4, 9, 23 );

                                    //split bytes 596 and 597 into bits
                                    this->fReadingModeInkLockDown = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 0 );
                                    this->fAcetateShowInkAtn = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 1 );
                                    this->fFilterDttm = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 2 );
                                    this->fEnforceDocProt = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 3 );
                                    this->iDocProtCur = (unsigned short)FormatUtils::GetUIntFromBytesBits( ( bytes + 596 ), 2, 4, 3 );
                                    this->fDispBkSpSaved = FormatUtils::GetBitFromBytes( ( bytes + 596 ), 2, 7 );

                                    this->dxaPageLock = FormatUtils::BytesToInt16( bytes, 598, size );
                                    this->dyaPageLock = FormatUtils::BytesToInt16( bytes, 600, size );
                                    this->pctFontLock = FormatUtils::BytesToInt32( bytes, 602, size );
                                    this->grfitbid = bytes[606];
                                    //byte 607 is unused
                                    this->ilfoMacAtCleanup = FormatUtils::BytesToUInt16( bytes, 608, size );
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
                this->fAlignTablesRowByRow = true;
                this->fLayoutTableRowsApart = true;
                this->fGrowAutofit = true;
                this->fDontWrapTextWithPunct = true;
                //ToDo: Don't autofit tables next to wrapped objects
                //ToDo: Don't break constrained tables forced onto the page
                this->fDontBreakWrappedTables = true;
                this->fDontSnapToGridInCell = true;
                this->fDontUseAsianBreakRules = true;
                this->fNoTabForInd = true;
                this->fDontUseHTMLParagraphAutoSpacing = true;
                this->fForgetLastTabAlign = true;
                this->fSpLayoutLikeWW8 = true;
                this->fFtnLayoutLikeWW8 = true;
                this->fLayoutRawTableWidth = true;
                this->fDontAllowFieldEndSelect = true;
                //ToDo: underline characters in numbered lists
                this->fUseWord2002TableStyleRules = true;
                this->fUserWord97LineBreakingRules = true;
            }
            else if ( nFib == Fib2000 )
            {
                //Word 2000 default settings

                this->fGrowAutofit = true;
                this->fDontWrapTextWithPunct = true;
                //ToDo: Don't autofit tables next to wrapped objects
                this->fDontBreakWrappedTables = true;
                this->fDontSnapToGridInCell = true;
                this->fDontUseAsianBreakRules = true;
                this->fNoTabForInd = true;
                this->fDontAllowFieldEndSelect = true;
                //ToDo: underline characters in numbered lists
                this->fUseWord2002TableStyleRules = true;
            }
            else if( nFib ==  Fib2002 )
            {
                //Word 2002 (XP)

                this->fGrowAutofit = true;
                //ToDo: Don't autofit tables next to wrapped objects
                this->fDontBreakWrappedTables = true;
                this->fNoTabForInd = true;
                this->fUseWord2002TableStyleRules = true;
            }
            else if ( nFib == Fib2003 )
            {
                //Word 2003

                //ToDo: Don't autofit tables next to wrapped objects
                this->fDontBreakWrappedTables = true;
                this->fNoTabForInd = true;

            }
            else if ( nFib < Fib1997 )
            {
                //throw new UnspportedFileVersionException();
            }
        }

		/*========================================================================================================*/

		void WordDocumentProperties::Initialize()
		{
		  this->fFacingPages = false;
          this->fWindowControl = false;
          this->fPMHMainDoc = false;
          this->grfSuppression = 0;
          this->Fpc = 0;
          this->grpfIhdt = 0;
          this->rncFtn = 0;
          this->nFtn = 0;
          this->fOutlineDirtySave = false;
          this->fOnlyMacPics = false;
          this->fOnlyWinPics = false;
          this->fLabelDoc = false;
          this->fHyphCapitals = false;
          this->fAutoHyphen = false;
          this->fFormNoFields = false;
          this->fLinkStyles = false;
          this->fRevMarking = false;
          this->fBackup = false;
          this->fExactWords = false;
          this->fPagHidden = false;
          this->fPagResults = false;
          this->fLockAtn = false;
          this->fMirrorMargins = false;
          this->fDflttrueType = false;
          this->fPagSuppressTopSpacing = false;
          this->fProtEnabled = false;
          this->fDispFormFldSel = false;
          this->fRMView = false;
          this->fRMPrint = false;
          this->fLockRev = false;
          this->fEmbedFonts = false;
          this->fNoTabForInd = false;
          this->fNoSpaceRaiseLower = false;
          this->fSuppressSpbfAfterPageBreak = false;
          this->fWrapTrailSpaces = false;
          this->fMapPrintTextColor = false;
          this->fNoColumnBalance = false;
          this->fConvMailMergeEsc = false;
          this->fSuppressTopSpacing = false;
          this->fOrigWordTableRules = false;
          this->fTransparentMetafiles = false;
          this->fShowBreaksInFrames = false;
          this->fSwapBordersFacingPgs = false;
          this->dxaTab = 0;
          this->wSpare = 0;
		  this->dxaHotZ = 0;
          this->cConsecHypLim = 0;
          this->wSpare2 = 0;
          this->nRevision = 0;
          this->tmEdited = 0;
          this->cWords = 0;
          this->cCh = 0;
          this->cPg = 0;
          this->cParas = 0;
          this->rncEdn = 0;
          this->nEdn = 0;
          this->Epc = 0;
          this->nfcFtnRef = 0;
          this->nfcEdnRef = 0;
          this->fPrintFormData = false;
          this->fSaveFormData = false;
          this->fShadeFormData = false;
          this->fWCFtnEdn = false;
          this->cLines = 0;
          this->cWordsFtnEdn = 0;
          this->cChFtnEdn = 0;
          this->cPgFtnEdn = 0;
          this->cParasFtnEdn = 0;
          this->cLinesFtnEdn = 0;
          this->lKeyProtDoc = 0;
          this->wvkSaved = 0;
          this->wScaleSaved = 0;
          this->zkSaved = 0;
          this->fRotateFontW6 = false;
          this->iGutterPos = false;
          this->fSuppressTopSpacingMac5 = false;
          this->fTruncDxaExpand = false;
          this->fPrintBodyBeforeHdr = false;
          this->fNoLeading = false;
          this->fMWSmallCaps = false;
          this->adt = 0;
		  this->doptypography = NULL;
          this->dogrid = NULL;
          this->lvl = 0;
          this->fGramAllDone = false;
          this->fGramAllClean = false;
          this->fSubsetFonts = false;
          this->fHideLastVersion = false;
          this->fHtmlDoc = false;
          this->fSnapBorder = false;
          this->fIncludeHeader = false;
          this->fIncludeFooter = false;
          this->fForcePageSizePag = false;
          this->fMinFontSizePag = false;
          this->fHaveVersions = false;
          this->fAutoVersion = false;
          this->asumyi = NULL;
          this->cChWS = 0;
          this->cChWSFtnEdn = 0;
          this->grfDocEvents = 0;
          this->fVirusPromted = false;
          this->fVirusLoadSafe = false;
          this->KeyVirusSession30 = 0;
          this->cDBC = 0;
          this->cDBCFtnEdn = 0;
          this->hpsZoonFontPag = 0;
          this->dywDispPag = 0;
          this->ilvlLastBulletMain = 0;
          this->ilvlLastNumberMain = 0;
          this->istdClickTypePara = 0;
          this->fLADAllDone = false;
          this->fEnvelopeVis = false;
          this->fMaybeTentativeListInDoc = false;
          this->fMaybeFitText = false;
          this->fRelyOnCss_WebOpt = false;
          this->fRelyOnVML_WebOpt = false;
          this->fAllowPNG_WebOpt = false;
          this->screenSize_WebOpt = 0;
          this->fOrganizeInFolder_WebOpt = false;
          this->fUseLongFileNames_WebOpt = false;
          this->iPixelsPerInch_WebOpt = 0;
          this->fWebOptionsInit = false;
          this->fMaybeFEL = false;
          this->fCharLineUnits = false;
          this->fMaybeRTLTables = false;
          this->fLeaveBackslashAlone = false;
          this->fExpShRtn = false;
          this->fDntULTrlSpc = false;
          this->fDntBlnSbDbWid = false;
          this->fMakeSpaceForUL = false;
          this->f2ptExtLeadingOnly = false;
          this->fTruncFontHeight = false;
          this->fSubOnSize = false;
          this->fLineWrapLikeWord6 = false;
          this->fWW6BorderRules = false;
          this->fExactOnTop = false;
          this->fExtraAfter = false;
          this->fWPSpace = false;
          this->fWPJust = false;
          this->fPrintMet = false;
          this->fSpLayoutLikeWW8 = false;
          this->fFtnLayoutLikeWW8 = false;
          this->fDontUseHTMLParagraphAutoSpacing = false;
          this->fDontAdjustLineHeightInTable = false;
          this->fForgetLastTabAlign = false;
          this->fUseAutoSpaceForFullWidthAlpha = false;
          this->fAlignTablesRowByRow = false;
          this->fLayoutRawTableWidth = false;
          this->fLayoutTableRowsApart = false;
          this->fUserWord97LineBreakingRules = false;
          this->fDontBreakWrappedTables = false;
          this->fDontSnapToGridInCell = false;
          this->fDontAllowFieldEndSelect = false;
          this->fApplyBreakingRules = false;
          this->fDontWrapTextWithPunct = false;
          this->fDontUseAsianBreakRules = false;
          this->fUseWord2002TableStyleRules = false;
          this->fGrowAutofit = false;
		  this->fUseNormalStyleForList = false;
		  this->fDontUseIndentAsNumberingTabStop = false;
		  this->fFELineBreak11 = false;
		  this->fAllowSpaceOfSameStyleInTable = false;
		  this->fWW11IndentRules = false;
		  this->fDontAutofitConstrainedTables = false;
		  this->fAutofitLikeWW11 = false;
		  this->fUnderlineTabInNumList = false;
		  this->fHangulWidthLikeWW11 = false;
		  this->fSplitPgBreakAndParaMark = false;
		  this->fDontVertAlignCellWithSp = false;
		  this->fDontBreakConstrainedForcedTables = false;
		  this->fDontVertAlignInTxbx = false;
		  this->fWord11KerningPairs = false;
		  this->fCachedColBalance = false;
          this->verCompatPreW10 = 0;
          this->fNoMargPgvwSaved = false;
          this->fNoMargPgvWPag = false;
          this->fWebViewPag = false;
          this->fSeeDrawingsPag = false;
          this->fBulletProofed = false;
          this->fCorrupted = false;
          this->fSaveUim = false;
          this->fFilterPrivacy = false;
          this->fInFReplaceNoRM = false;
          this->fSeenRepairs = false;
          this->fHasXML = false;
          this->fSeeScriptAnchorsPag = false;
          this->fValidateXML = false;
          this->fSaveIfInvalidXML = false;
          this->fShowXMLErrors = false;
          this->fAlwaysMergeEmptyNamespace = false;
          this->cpMaxListCacheMainDoc = 0;
          this->fDoNotEmbedSystemFont = false;
          this->fWordCompact = false;
          this->fLiveRecover = false;
          this->fEmbedFactoids = false;
          this->fFactoidXML = false;
          this->fFactoidAllDone = false;
          this->fFolioPrint = false;
          this->fReverseFolio = false;
          this->iTextLineEnding = false;
          this->fHideFcc = false;
          this->fAcetateShowMarkup = false;
          this->fAcetateShowAtn = false;
          this->fAcetateShowInsDel = false;
          this->fAcetateShowProps = false;
          this->istdTableDflt = 0;
          this->verCompat = 0;
          this->grfFmtFilter = 0;
          this->iFolioPages = 0;
          this->cpgText = 0;
          this->cpMinRMText = 0;
          this->cpMinRMFtn = 0;
          this->cpMinRMHdd = 0;
          this->cpMinRMAtn = 0;
          this->cpMinRMEdn = 0;
          this->cpMinRMTxbx = 0;
          this->cpMinRMHdrTxbx = 0;
          this->rsidRoot = 0;
          this->fTreatLockAtnAsReadOnly = false;
          this->fStyleLock = false;
          this->fAutoFmtOverride = false;
          this->fRemoveWordML = false;
          this->fApplyCustomXForm = false;
          this->fStyeLockEnforced = false;
          this->fFakeLockAtn = false;
          this->fIgnoreMixedContent = false;
          this->fShowPlaceholderText = false;
          this->grf = 0;
          this->fReadingModeInkLockDown = false;
          this->fAcetateShowInkAtn = false;
          this->fFilterDttm = false;
          this->fEnforceDocProt = false;
          this->iDocProtCur = 0;
          this->fDispBkSpSaved = false;
          this->dxaPageLock = 0;
          this->dyaPageLock = 0;
          this->pctFontLock = 0;
          this->grfitbid = 0;
          this->ilfoMacAtCleanup = 0;
		}
}