#include "stdafx.h"

#include "SettingsMapping.h"

namespace DocFileFormat
{
	SettingsMapping::SettingsMapping (ConversionContext* ctx):  PropertiesMapping(&m_oXmlWriter)
	{
		_ctx = ctx;
	}

	void SettingsMapping::Apply (IVisitable* visited)
	{
		WordDocumentProperties* dop = static_cast<WordDocumentProperties*>( visited );

		this->_ctx->_docx->RegisterSettings();

		//start w:settings
		m_oXmlWriter.WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
		m_oXmlWriter.WriteNodeBegin( _T( "w:settings" ), TRUE );

		//write namespaces
		m_oXmlWriter.WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );

		m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE, FALSE );

		//zoom
		m_oXmlWriter.WriteNodeBegin( _T( "w:zoom" ), TRUE );
		m_oXmlWriter.WriteAttribute( _T( "w:percent" ), FormatUtils::IntToWideString( dop->wScaleSaved ).c_str() );

		if ( dop->zkSaved != 0 )
		{
			m_oXmlWriter.WriteAttribute( _T( "w:val" ), FormatUtils::MapValueToWideString( dop->zkSaved, &ZoomTypeMap[0][0], 3, 9 ).c_str() );
		}

		m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );

		//doc protection
		//<w:documentProtection w:edit="forms" w:enforcement="1"/>

		//embed system fonts
		if( !dop->fDoNotEmbedSystemFont )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:embedSystemFonts" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		//mirror margins
		if ( dop->fMirrorMargins )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:mirrorMargins" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		//evenAndOddHeaders 
		if ( dop->fFacingPages )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:evenAndOddHeaders" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		//proof state
		XMLTools::XMLElement<WCHAR> proofState( _T( "w:proofState" ) );

		if ( dop->fGramAllClean )
		{
			appendValueAttribute( &proofState, _T( "w:grammar" ), _T( "clean" ) );
		}

		if ( proofState.GetAttributeCount() > 0 )
		{
			m_oXmlWriter.WriteString( proofState.GetXMLString().c_str() );
		}

		//stylePaneFormatFilter
		if ( dop->grfFmtFilter != 0 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:stylePaneFormatFilter" ), TRUE );
			m_oXmlWriter.WriteAttribute( _T( "w:val" ), FormatUtils::IntToFormattedWideString( dop->grfFmtFilter, _T( "%04x" ) ).c_str() );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		//default tab stop
		m_oXmlWriter.WriteNodeBegin( _T( "w:defaultTabStop" ), TRUE );
		m_oXmlWriter.WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString( dop->dxaTab ).c_str() );
		m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );

		//drawing grid
		if( dop->dogrid != NULL )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:displayHorizontalDrawingGridEvery" ), TRUE );
			m_oXmlWriter.WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString( dop->dogrid->dxGridDisplay ).c_str() );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );

			m_oXmlWriter.WriteNodeBegin( _T( "w:displayVerticalDrawingGridEvery" ), TRUE );
			m_oXmlWriter.WriteAttribute( _T( "w:val" ), FormatUtils::IntToWideString( dop->dogrid->dyGridDisplay ).c_str() );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );

			if ( dop->dogrid->fFollowMargins == false )
			{
				m_oXmlWriter.WriteNodeBegin( _T( "w:doNotUseMarginsForDrawingGridOrigin" ), TRUE );
				m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
			}
		}

		//typography
		if ( dop->doptypography != NULL )
		{
			if ( dop->doptypography->fKerningPunct == false )
			{
				m_oXmlWriter.WriteNodeBegin( _T( "w:noPunctuationKerning" ), TRUE );
				m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
			}
		}

		//footnote properties
		XMLTools::XMLElement<WCHAR> footnotePr( _T( "w:footnotePr" ) );

		if ( dop->nFtn != 0 )
		{
			appendValueAttribute( &footnotePr, _T( "w:numStart" ), FormatUtils::IntToWideString( dop->nFtn ).c_str() );
		}

		if ( dop->rncFtn != 0 )
		{
			appendValueAttribute( &footnotePr, _T( "w:numRestart" ), FormatUtils::IntToWideString( dop->rncFtn ).c_str() );
		}

		if ( dop->Fpc != 0 )
		{
			appendValueAttribute( &footnotePr, _T( "w:pos" ), FormatUtils::MapValueToWideString( dop->Fpc, &FootnotePositionMap[0][0], 4, 12  ).c_str() );
		}

		if ( footnotePr.GetAttributeCount() > 0 )
		{
			m_oXmlWriter.WriteString( footnotePr.GetXMLString().c_str() );
		}


		writeCompatibilitySettings( dop );

		writeRsidList();

		//close w:settings
		m_oXmlWriter.WriteNodeEnd( _T( "w:settings" ) );

		this->_ctx->_docx->SettingsXML = wstring( m_oXmlWriter.GetXmlString() ); 
	}

	void SettingsMapping::writeRsidList()
	{
		//convert the rsid list
		m_oXmlWriter.WriteNodeBegin( _T( "w:rsids" ) );

		for ( set<wstring>::iterator iter = this->_ctx->AllRsids.begin(); iter != this->_ctx->AllRsids.end(); iter++ )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:rsid" ), TRUE );
			m_oXmlWriter.WriteAttribute( _T( "w:val" ), iter->c_str() );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		m_oXmlWriter.WriteNodeEnd( _T( "w:rsids" ) );
	}

	void SettingsMapping::writeCompatibilitySettings(const WordDocumentProperties* dop)
	{
		//compatibility settings
		m_oXmlWriter.WriteNodeBegin( _T( "w:compat" ) );

		//some settings must always be written

		if ( dop->fUseNormalStyleForList )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:useNormalStyleForList" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontUseIndentAsNumberingTabStop )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotUseIndentAsNumberingTabStop" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fFELineBreak11 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:useAltKinsokuLineBreakRules" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fAllowSpaceOfSameStyleInTable )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:allowSpaceOfSameStyleInTable" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fWW11IndentRules )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotSuppressIndentation" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontAutofitConstrainedTables )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotAutofitConstrainedTables" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fAutofitLikeWW11 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:autofitToFirstFixedWidthCell" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fUnderlineTabInNumList )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:underlineTabInNumList" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fHangulWidthLikeWW11 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:displayHangulFixedWidth" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSplitPgBreakAndParaMark )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:splitPgBreakAndParaMark" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontVertAlignCellWithSp )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotVertAlignCellWithSp" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontBreakConstrainedForcedTables )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotBreakConstrainedForcedTable" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontVertAlignInTxbx )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotVertAlignInTxbx" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fWord11KerningPairs )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:useAnsiKerningPairs" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fCachedColBalance )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:cachedColBalance" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		//others are saved in the file
		if ( !dop->fDontAdjustLineHeightInTable )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:adjustLineHeightInTable" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fAlignTablesRowByRow )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:alignTablesRowByRow" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fApplyBreakingRules )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:applyBreakingRules" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fUseAutoSpaceForFullWidthAlpha )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:autoSpaceLikeWord95" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( !dop->fDntBlnSbDbWid )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:balanceSingleByteDoubleByteWidth" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fConvMailMergeEsc )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:convMailMergeEsc" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontBreakWrappedTables )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotBreakWrappedTables" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( !dop->fExpShRtn )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotExpandShiftReturn" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( !dop->fLeaveBackslashAlone )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotLeaveBackslashAlone" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontSnapToGridInCell )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotSnapToGridInCell" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontUseAsianBreakRules )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotUseEastAsianBreakRules" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontUseHTMLParagraphAutoSpacing )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotUseHTMLParagraphAutoSpacing" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontWrapTextWithPunct )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:doNotWrapTextWithPunct" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fFtnLayoutLikeWW8 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:footnoteLayoutLikeWW8" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fForgetLastTabAlign )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:forgetLastTabAlignment" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fGrowAutofit )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:growAutofit" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fLayoutRawTableWidth )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:layoutRawTableWidth" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fLayoutTableRowsApart )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:layoutTableRowsApart" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fLineWrapLikeWord6 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:lineWrapLikeWord6" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fMWSmallCaps )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:mwSmallCaps" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fNoColumnBalance )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:noColumnBalance" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fNoLeading )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:noLeading" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fNoSpaceRaiseLower )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:noSpaceRaiseLower" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fNoTabForInd )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:noTabHangInd" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fPrintBodyBeforeHdr )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:printBodyTextBeforeHeader" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fMapPrintTextColor )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:printColBlack" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fDontAllowFieldEndSelect )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:selectFldWithFirstOrLastChar" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSpLayoutLikeWW8 )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:shapeLayoutLikeWW8" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fShowBreaksInFrames )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:showBreaksInFrames" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fMakeSpaceForUL )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:spaceForUL" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fTruncDxaExpand )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:spacingInWholePoints" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSubOnSize )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:subFontBySize" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSuppressSpbfAfterPageBreak )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:suppressSpBfAfterPgBrk" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSuppressTopSpacing )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:suppressTopSpacing" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fSwapBordersFacingPgs )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:swapBordersFacingPages" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( !dop->fDntULTrlSpc )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:ulTrailSpace" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fPrintMet )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:usePrinterMetrics" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fUseWord2002TableStyleRules )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:useWord2002TableStyleRules" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fUserWord97LineBreakingRules )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:useWord97LineBreakRules" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fWPJust )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:wpJustification" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fWPSpace )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:wpSpaceWidth" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		if ( dop->fWrapTrailSpaces )
		{
			m_oXmlWriter.WriteNodeBegin( _T( "w:wrapTrailSpaces" ), TRUE );
			m_oXmlWriter.WriteNodeEnd( _T( "" ), TRUE );
		}

		m_oXmlWriter.WriteNodeEnd( _T( "w:compat" ) );
	}
}