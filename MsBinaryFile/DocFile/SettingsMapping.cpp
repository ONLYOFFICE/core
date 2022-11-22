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


#include "SettingsMapping.h"
#include "../../OfficeCryptReader/source/CryptTransform.h"

namespace DocFileFormat
{
	std::string DecodeBase64(const std::string & value)
	{
		int nLength = 0;
		unsigned char *pData = NULL;
		std::string result;

		NSFile::CBase64Converter::Decode(value.c_str(), value.length(), pData, nLength);
		if (pData)
		{
			result = std::string((char*)pData, nLength);
			delete []pData; pData = NULL;
		}
		return result;
	}
	std::wstring EncodeBase64(const std::string & value)
	{
		int nLength = 0;
		char *pData = NULL;
		std::string result;

		NSFile::CBase64Converter::Encode((BYTE*)value.c_str(), value.length(), pData, nLength, NSBase64::B64_BASE64_FLAG_NOCRLF);
		if (pData)
		{
			result = std::string(pData, nLength);
			delete []pData; pData = NULL;
		}
		return std::wstring(result.begin(), result.end());
	}
	SettingsMapping::SettingsMapping (ConversionContext* ctx):  PropertiesMapping(&m_oXmlWriter)
	{
		_ctx = ctx;
	}

	void SettingsMapping::Apply (IVisitable* visited)
	{
		WordDocumentProperties* dop = static_cast<WordDocumentProperties*>( visited );

		_ctx->_docx->RegisterSettings();

		//start w:settings
		m_oXmlWriter.WriteNodeBegin( L"w:settings",  TRUE );
		//write namespaces
		m_oXmlWriter.WriteAttribute( L"xmlns:w",  OpenXmlNamespaces::WordprocessingML );
		m_oXmlWriter.WriteNodeEnd( L"",  TRUE, FALSE );

		if (_ctx->_doc->FIB->m_FibBase.fWriteReservation)
		{
			m_oXmlWriter.WriteNodeBegin( L"w:writeProtection",  TRUE );
			WideString* passw = static_cast<WideString*>(_ctx->_doc->AssocNames->operator[]( 17 ));
			if (passw && false == passw->empty())
			{
				CRYPT::_ecmaWriteProtectData data;
				
				CRYPT::ECMAWriteProtect protect;
				protect.SetCryptData(data);
				protect.SetPassword(*passw);

				protect.Generate();
				protect.GetCryptData(data);

				//m_oXmlWriter.WriteAttribute	( L"w:cryptProviderType", L"rsaAES");
				//m_oXmlWriter.WriteAttribute	( L"w:cryptAlgorithmSid", 14); //sha-512
				//m_oXmlWriter.WriteAttribute	( L"w:cryptAlgorithmType", L"typeAny");
				//m_oXmlWriter.WriteAttribute	( L"w:cryptAlgorithmClass", L"hash");
				//m_oXmlWriter.WriteAttribute	( L"w:cryptSpinCount", data.spinCount);
				//m_oXmlWriter.WriteAttribute	( L"w:hash", EncodeBase64(data.hashValue));
				//m_oXmlWriter.WriteAttribute	( L"w:salt", EncodeBase64(data.saltValue));
				m_oXmlWriter.WriteAttribute	( L"w:algorithmName", L"SHA-512");
				m_oXmlWriter.WriteAttribute	( L"w:spinCount", data.spinCount);
				m_oXmlWriter.WriteAttribute	( L"w:hashValue", EncodeBase64(data.hashValue));
				m_oXmlWriter.WriteAttribute	( L"w:saltValue", EncodeBase64(data.saltValue));
			}
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE, TRUE );
		}
		//zoom
		m_oXmlWriter.WriteNodeBegin	( L"w:zoom",  TRUE );
		m_oXmlWriter.WriteAttribute	( L"w:percent",  FormatUtils::IntToWideString( dop->wScaleSaved > 0 ? dop->wScaleSaved : 100 ) );

		if ( dop->zkSaved != 0 )
		{
			m_oXmlWriter.WriteAttribute( L"w:val",  FormatUtils::MapValueToWideString( dop->zkSaved, &ZoomTypeMap[0][0], 3, 9 ) );
		}
		m_oXmlWriter.WriteNodeEnd( L"",  TRUE );

		if (dop->fDispBkSpSaved)
		{
			m_oXmlWriter.WriteNodeBegin	( L"w:displayBackgroundShape",  TRUE );
			m_oXmlWriter.WriteNodeEnd	( L"",  TRUE );
		}

		//doc protection
		//<w:documentProtection w:edit="forms" w:enforcement="1"/>

		//embed system fonts
		if( !dop->fDoNotEmbedSystemFont )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:embedSystemFonts",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		//mirror margins
		if ( dop->fMirrorMargins )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:mirrorMargins",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		//evenAndOddHeaders 
		if ( dop->fFacingPages )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:evenAndOddHeaders",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		//proof state
        XMLTools::XMLElement proofState( L"w:proofState" );

		if ( dop->fGramAllClean )
		{
			appendValueAttribute( &proofState, L"w:grammar",  L"clean" );
		}

		if ( proofState.GetAttributeCount() > 0 )
		{
			m_oXmlWriter.WriteString( proofState.GetXMLString() );
		}

		//stylePaneFormatFilter
		if ( dop->grfFmtFilter != 0 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:stylePaneFormatFilter",  TRUE );
			m_oXmlWriter.WriteAttribute( L"w:val",  FormatUtils::IntToFormattedWideString( dop->grfFmtFilter, L"%04x" ) );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		//default tab stop
		m_oXmlWriter.WriteNodeBegin( L"w:defaultTabStop",  TRUE );
		m_oXmlWriter.WriteAttribute( L"w:val",  FormatUtils::IntToWideString( dop->dxaTab ) );
		m_oXmlWriter.WriteNodeEnd( L"",  TRUE );

		//drawing grid
		if( dop->dogrid != NULL )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:displayHorizontalDrawingGridEvery",  TRUE );
			m_oXmlWriter.WriteAttribute( L"w:val",  FormatUtils::IntToWideString( dop->dogrid->dxGridDisplay ) );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );

			m_oXmlWriter.WriteNodeBegin( L"w:displayVerticalDrawingGridEvery",  TRUE );
			m_oXmlWriter.WriteAttribute( L"w:val",  FormatUtils::IntToWideString( dop->dogrid->dyGridDisplay ) );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );

			if ( dop->dogrid->fFollowMargins == false )
			{
				m_oXmlWriter.WriteNodeBegin( L"w:doNotUseMarginsForDrawingGridOrigin",  TRUE );
				m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
			}
		}

		//typography
		if ( dop->doptypography != NULL )
		{
			if ( dop->doptypography->fKerningPunct == false )
			{
				m_oXmlWriter.WriteNodeBegin( L"w:noPunctuationKerning",  TRUE );
				m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
			}
		}

		//footnote properties
        XMLTools::XMLElement footnotePr( L"w:footnotePr" );

		if ( dop->nFtn != 0 )
		{
			appendValueAttribute( &footnotePr, L"w:numStart",  FormatUtils::IntToWideString( dop->nFtn ) );
		}

		if ( dop->rncFtn != 0 )
		{
			appendValueAttribute( &footnotePr, L"w:numRestart",  FormatUtils::IntToWideString( dop->rncFtn ) );
		}

		if ( dop->Fpc != 0 )
		{
			appendValueAttribute( &footnotePr, L"w:pos",  FormatUtils::MapValueToWideString( dop->Fpc, &FootnotePositionMap[0][0], 4, 12  ) );
		}

		if ( footnotePr.GetAttributeCount() > 0 )
		{
			m_oXmlWriter.WriteString( footnotePr.GetXMLString() );
		}


		writeCompatibilitySettings( dop );

		writeRsidList();

		//close w:settings
		m_oXmlWriter.WriteNodeEnd( L"w:settings" );

		this->_ctx->_docx->SettingsXML = std::wstring( m_oXmlWriter.GetXmlString() ); 
	}

	void SettingsMapping::writeRsidList()
	{
		//convert the rsid list
		m_oXmlWriter.WriteNodeBegin( L"w:rsids" );

		for ( std::set<std::wstring>::iterator iter = this->_ctx->AllRsids.begin(); iter != this->_ctx->AllRsids.end(); iter++ )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:rsid",  TRUE );
			m_oXmlWriter.WriteAttribute( L"w:val",  iter->c_str() );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		m_oXmlWriter.WriteNodeEnd( L"w:rsids" );
	}

	void SettingsMapping::writeCompatibilitySettings(const WordDocumentProperties* dop)
	{
		//compatibility settings
		m_oXmlWriter.WriteNodeBegin( L"w:compat" );

		m_oXmlWriter.WriteNodeBegin( L"w:compatSetting", TRUE);
			m_oXmlWriter.WriteAttribute( L"w:name",  L"compatibilityMode" );
			m_oXmlWriter.WriteAttribute( L"w:uri",  L"http://schemas.microsoft.com/office/word" );
			m_oXmlWriter.WriteAttribute( L"w:val",  L"11" );
		m_oXmlWriter.WriteNodeEnd( L"",  TRUE );

		//some settings must always be written

		if ( dop->fUseNormalStyleForList )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:useNormalStyleForList",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontUseIndentAsNumberingTabStop )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotUseIndentAsNumberingTabStop",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fFELineBreak11 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:useAltKinsokuLineBreakRules",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fAllowSpaceOfSameStyleInTable )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:allowSpaceOfSameStyleInTable",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fWW11IndentRules )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotSuppressIndentation",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontAutofitConstrainedTables )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotAutofitConstrainedTables",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fAutofitLikeWW11 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:autofitToFirstFixedWidthCell",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fUnderlineTabInNumList )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:underlineTabInNumList",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fHangulWidthLikeWW11 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:displayHangulFixedWidth",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSplitPgBreakAndParaMark )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:splitPgBreakAndParaMark",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontVertAlignCellWithSp )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotVertAlignCellWithSp",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontBreakConstrainedForcedTables )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotBreakConstrainedForcedTable",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontVertAlignInTxbx )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotVertAlignInTxbx",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fWord11KerningPairs )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:useAnsiKerningPairs",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fCachedColBalance )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:cachedColBalance",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		//others are saved in the file
		if ( !dop->fDontAdjustLineHeightInTable )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:adjustLineHeightInTable",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fAlignTablesRowByRow )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:alignTablesRowByRow",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fApplyBreakingRules )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:applyBreakingRules",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fUseAutoSpaceForFullWidthAlpha )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:autoSpaceLikeWord95",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( !dop->fDntBlnSbDbWid )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:balanceSingleByteDoubleByteWidth",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fConvMailMergeEsc )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:convMailMergeEsc",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontBreakWrappedTables )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotBreakWrappedTables",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( !dop->fExpShRtn )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotExpandShiftReturn",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( !dop->fLeaveBackslashAlone )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotLeaveBackslashAlone",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontSnapToGridInCell )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotSnapToGridInCell",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontUseAsianBreakRules )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotUseEastAsianBreakRules",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontUseHTMLParagraphAutoSpacing )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotUseHTMLParagraphAutoSpacing",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontWrapTextWithPunct )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:doNotWrapTextWithPunct",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fFtnLayoutLikeWW8 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:footnoteLayoutLikeWW8",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fForgetLastTabAlign )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:forgetLastTabAlignment",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fGrowAutofit )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:growAutofit",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fLayoutRawTableWidth )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:layoutRawTableWidth",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fLayoutTableRowsApart )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:layoutTableRowsApart",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fLineWrapLikeWord6 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:lineWrapLikeWord6",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fMWSmallCaps )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:mwSmallCaps",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fNoColumnBalance )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:noColumnBalance",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fNoLeading )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:noLeading",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fNoSpaceRaiseLower )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:noSpaceRaiseLower",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fNoTabForInd )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:noTabHangInd",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fPrintBodyBeforeHdr )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:printBodyTextBeforeHeader",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fMapPrintTextColor )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:printColBlack",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fDontAllowFieldEndSelect )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:selectFldWithFirstOrLastChar",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSpLayoutLikeWW8 )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:shapeLayoutLikeWW8",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fShowBreaksInFrames )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:showBreaksInFrames",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fMakeSpaceForUL )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:spaceForUL",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fTruncDxaExpand )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:spacingInWholePoints",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSubOnSize )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:subFontBySize",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSuppressSpbfAfterPageBreak )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:suppressSpBfAfterPgBrk",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSuppressTopSpacing )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:suppressTopSpacing",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fSwapBordersFacingPgs )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:swapBordersFacingPages",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( !dop->fDntULTrlSpc )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:ulTrailSpace",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fPrintMet )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:usePrinterMetrics",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fUseWord2002TableStyleRules )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:useWord2002TableStyleRules",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fUserWord97LineBreakingRules )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:useWord97LineBreakRules",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fWPJust )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:wpJustification",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fWPSpace )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:wpSpaceWidth",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		if ( dop->fWrapTrailSpaces )
		{
			m_oXmlWriter.WriteNodeBegin( L"w:wrapTrailSpaces",  TRUE );
			m_oXmlWriter.WriteNodeEnd( L"",  TRUE );
		}

		m_oXmlWriter.WriteNodeEnd( L"w:compat" );
	}
}
