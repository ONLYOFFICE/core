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
#include "Settings.h"

namespace OOX
{
namespace Settings
{
	void CWritingStyle::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CWritingStyle::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CWritingStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CWritingStyle::toXML() const
	{
        std::wstring sResult = L"<w:activeWritingStyle ";

		sResult += m_oLang.ToAttribute( L"w:lang");
		sResult += m_sVendorID.ToAttribute( L"w:vendorID");
		sResult += m_sDllVersion.ToAttribute( L"w:dllVersion");
		sResult += m_oNlCheck.ToAttribute( L"w:nlCheck" );
		sResult += m_oCheckSyle.ToAttribute( L"w:checkStyle");
		sResult += m_sAppName.ToAttribute( L"w:appName" );

		sResult += L"/>";

		return sResult;
	}
	void CWritingStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'a':
					if      ( L"w:appName"    == wsName ) m_sAppName    = oReader.GetText();
					break;

				case 'c':
					if      ( L"w:checkStyle" == wsName ) m_oCheckSyle  = oReader.GetText();
					break;

				case 'd':
					if      ( L"w:dllVersion" == wsName ) m_sDllVersion = oReader.GetText();
					break;

				case 'l':
					if      ( L"w:lang"       == wsName ) m_oLang       = oReader.GetText();
					break;

				case 'n':
					if      ( L"w:nlCheck"    == wsName ) m_oNlCheck    = oReader.GetText();
					break;

				case 'v':
					if      ( L"w:vendorID"   == wsName ) m_sVendorID   = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

	void CAutoCaption::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CAutoCaption::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CAutoCaption::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CAutoCaption::toXML() const
	{
        std::wstring sResult = L"<w:autoCaption ";

		sResult += m_sName.ToAttribute( L"w:name");
		sResult += m_sCaption.ToAttribute( L"w:caption");

		sResult += L"/>";

		return sResult;
	}
	void CAutoCaption::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		// Читаем атрибуты
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'c':
					if      ( L"w:caption" == wsName ) m_sCaption = oReader.GetText();
					break;

				case 'n':
					if      ( L"w:name"    == wsName ) m_sName    = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

	void CAutoCaptions::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CAutoCaptions::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CAutoCaptions::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:autoCaption" == sName )
			{
				OOX::Settings::CAutoCaption *oAC = new OOX::Settings::CAutoCaption(oReader);
				if (oAC) m_arrAutoCaption.push_back( oAC );
			}
		}
	}
    std::wstring CAutoCaptions::toXML() const
	{
        std::wstring sResult = L"<w:autoCaptions>";

		for ( unsigned int nIndex = 0; nIndex < m_arrAutoCaption.size(); nIndex++ )
		{
			if (m_arrAutoCaption[nIndex])
				sResult += m_arrAutoCaption[nIndex]->toXML();
		}

		sResult += L"</w:autoCaptions>";

		return sResult;
	}

	void CCaption::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CCaption::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CCaption::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CCaption::toXML() const
	{
        std::wstring sResult = L"<w:caption w:name=\"" + m_sName + L"\" ";

		ComplexTypes_WriteAttribute( L"w:pos=\"",     m_oPos );
		sResult += m_oChapNum.ToAttribute( L"w:chapNum");
		ComplexTypes_WriteAttribute( L"w:heading=\"", m_oHeading );
		sResult += m_oNoLabel.ToAttribute( L"w:noLabel");
		ComplexTypes_WriteAttribute( L"w:numFmt=\"",  m_oNumFmt );
		ComplexTypes_WriteAttribute( L"w:sep=\"",     m_oSep );

		sResult += L"/>";

		return sResult;
	}
	void CCaption::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		// Читаем атрибуты
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'c':
					if      ( L"w:chapNum" == wsName ) m_oChapNum = oReader.GetText();
					break;

				case 'h':
					if      ( L"w:heading" == wsName ) m_oHeading = oReader.GetText();
					break;

				case 'n':
					if      ( L"w:name"    == wsName ) m_sName    = oReader.GetText();
					else if ( L"w:noLabel" == wsName ) m_oNoLabel = oReader.GetText();
					else if ( L"w:numFmt"  == wsName ) m_oNumFmt  = oReader.GetText();
					break;

				case 'p':
					if      ( L"w:pos"     == wsName ) m_oPos     = oReader.GetText();
					break;

				case 's':
					if      ( L"w:sep"     == wsName ) m_oSep     = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}
	
	void CCaptions::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CCaptions::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CCaptions::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:caption" == sName )
			{
				OOX::Settings::CCaption *oC = new OOX::Settings::CCaption(oReader);
				if (oC) m_arrCaption.push_back( oC );
			}
			else if ( L"w:autoCaptions" == sName )
				m_oAutoCaptions = oReader;
		}
	}
    std::wstring CCaptions::toXML() const
	{
        std::wstring sResult = L"<w:captions>";

		for ( unsigned int nIndex = 0; nIndex < m_arrCaption.size(); nIndex++ )
		{
			if (m_arrCaption[nIndex])
				sResult += m_arrCaption[nIndex]->toXML();
		}

		if ( m_oAutoCaptions.IsInit() )
			sResult += m_oAutoCaptions->toXML();

		sResult += L"</w:captions>";

		return sResult;
	}

	void CCharacterSpacing::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CCharacterSpacing::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CCharacterSpacing::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CCharacterSpacing::toXML() const
	{
        std::wstring sResult = L"<w:characterSpacingControl w:val=\"" + m_oVal.ToString() + L"\"/>";
		return sResult;
	}
	void CCharacterSpacing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'v':
					if      ( L"w:val" == wsName ) m_oVal = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

	void CCompatSetting::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CCompatSetting::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CCompatSetting::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CCompatSetting::toXML() const
	{
        std::wstring sResult = L"<w:compatSetting ";

		sResult += m_sName.ToAttribute( L"w:name");
		sResult += m_sUri.ToAttribute( L"w:uri");
		sResult += m_sVal.ToAttribute( L"w:val" );

		sResult += L"/>";

		return sResult;
	}
	void CCompatSetting::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		// Читаем атрибуты
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'n':
					if      ( L"w:name" == wsName ) m_sName = oReader.GetText();
					break;

				case 'u':
					if      ( L"w:uri"  == wsName ) m_sUri  = oReader.GetText();
					break;

				case 'v':
					if      ( L"w:val"  == wsName ) m_sVal  = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}
	
	void CCompat::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CCompat::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CCompat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:useSingleBorderforContiguousCells" == sName )
				m_oUseSingleBorderforContiguousCells = oReader;
			else if ( L"w:wpJustification" == sName )
				m_oWpJustification = oReader;
			else if ( L"w:noTabHangInd" == sName )
				m_oNoTabHangInd = oReader;
			else if ( L"w:noLeading" == sName )
				m_oNoLeading = oReader;
			else if ( L"w:spaceForUL" == sName )
				m_oSpaceForUL = oReader;
			else if ( L"w:noColumnBalance" == sName )
				m_oNoColumnBalance = oReader;
			else if ( L"w:balanceSingleByteDoubleByteWidth" == sName )
				m_oBalanceSingleByteDoubleByteWidth = oReader;
			else if ( L"w:noExtraLineSpacing" == sName )
				m_oNoExtraLineSpacing = oReader;
			else if ( L"w:doNotLeaveBackslashAlone" == sName )
				m_oDoNotLeaveBackslashAlone = oReader;
			else if ( L"w:ulTrailSpace" == sName )
				m_oUlTrailSpace = oReader;
			else if ( L"w:doNotExpandShiftReturn" == sName )
				m_oDoNotExpandShiftReturn = oReader;
			else if ( L"w:spacingInWholePoints" == sName )
				m_oSpacingInWholePoints = oReader;
			else if ( L"w:lineWrapLikeWord6" == sName )
				m_oLineWrapLikeWord6 = oReader;
			else if ( L"w:printBodyTextBeforeHeader" == sName )
				m_oPrintBodyTextBeforeHeader = oReader;
			else if ( L"w:printColBlack" == sName )
				m_oPrintColBlack = oReader;
			else if ( L"w:wpSpaceWidth" == sName )
				m_oWpSpaceWidth = oReader;
			else if ( L"w:showBreaksInFrames" == sName )
				m_oShowBreaksInFrames = oReader;
			else if ( L"w:subFontBySize" == sName )
				m_oSubFontBySize = oReader;
			else if ( L"w:suppressBottomSpacing" == sName )
				m_oSuppressBottomSpacing = oReader;
			else if ( L"w:suppressTopSpacing" == sName )
				m_oSuppressTopSpacing = oReader;
			else if ( L"w:suppressSpacingAtTopOfPage" == sName )
				m_oSuppressSpacingAtTopOfPage = oReader;
			else if ( L"w:suppressTopSpacingWP" == sName )
				m_oSuppressTopSpacingWP = oReader;
			else if ( L"w:suppressSpBfAfterPgBrk" == sName )
				m_oSuppressSpBfAfterPgBrk = oReader;
			else if ( L"w:swapBordersFacingPages" == sName )
				m_oSwapBordersFacingPages = oReader;
			else if ( L"w:convMailMergeEsc" == sName )
				m_oConvMailMergeEsc = oReader;
			else if ( L"w:truncateFontHeightsLikeWP6" == sName )
				m_oTruncateFontHeightsLikeWP6 = oReader;
			else if ( L"w:mwSmallCaps" == sName )
				m_oMwSmallCaps = oReader;
			else if ( L"w:usePrinterMetrics" == sName )
				m_oUsePrinterMetrics = oReader;
			else if ( L"w:doNotSuppressParagraphBorders" == sName )
				m_oDoNotSuppressParagraphBorders = oReader;
			else if ( L"w:wrapTrailSpaces" == sName )
				m_oWrapTrailSpaces = oReader;
			else if ( L"w:footnoteLayoutLikeWW8" == sName )
				m_oFootnoteLayoutLikeWW8 = oReader;
			else if ( L"w:shapeLayoutLikeWW8" == sName )
				m_oShapeLayoutLikeWW8 = oReader;
			else if ( L"w:alignTablesRowByRow" == sName )
				m_oAlignTablesRowByRow = oReader;
			else if ( L"w:forgetLastTabAlignment" == sName )
				m_oForgetLastTabAlignment = oReader;
			else if ( L"w:adjustLineHeightInTable" == sName )
				m_oAdjustLineHeightInTable = oReader;
			else if ( L"w:autoSpaceLikeWord95" == sName )
				m_oAutoSpaceLikeWord95 = oReader;
			else if ( L"w:noSpaceRaiseLower" == sName )
				m_oNoSpaceRaiseLower = oReader;
			else if ( L"w:doNotUseHTMLParagraphAutoSpacing" == sName )
				m_oDoNotUseHTMLParagraphAutoSpacing = oReader;
			else if ( L"w:layoutRawTableWidth" == sName )
				m_oLayoutRawTableWidth = oReader;
			else if ( L"w:layoutTableRowsApart" == sName )
				m_oLayoutTableRowsApart = oReader;
			else if ( L"w:useWord97LineBreakRules" == sName )
				m_oUseWord97LineBreakRules = oReader;
			else if ( L"w:doNotBreakWrappedTables" == sName )
				m_oDoNotBreakWrappedTables = oReader;
			else if ( L"w:doNotSnapToGridInCell" == sName )
				m_oDoNotSnapToGridInCell = oReader;
			else if ( L"w:selectFldWithFirstOrLastChar" == sName )
				m_oSelectFldWithFirstOrLastChar = oReader;
			else if ( L"w:applyBreakingRules" == sName )
				m_oApplyBreakingRules = oReader;
			else if ( L"w:doNotWrapTextWithPunct" == sName )
				m_oDoNotWrapTextWithPunct = oReader;
			else if ( L"w:doNotUseEastAsianBreakRules" == sName )
				m_oDoNotUseEastAsianBreakRules = oReader;
			else if ( L"w:useWord2002TableStyleRules" == sName )
				m_oUseWord2002TableStyleRules = oReader;
			else if ( L"w:growAutofit" == sName )
				m_oGrowAutofit = oReader;
			else if ( L"w:useFELayout" == sName )
				m_oUseFELayout = oReader;
			else if ( L"w:useNormalStyleForList" == sName )
				m_oUseNormalStyleForList = oReader;
			else if ( L"w:doNotUseIndentAsNumberingTabStop" == sName )
				m_oDoNotUseIndentAsNumberingTabStop = oReader;
			else if ( L"w:useAltKinsokuLineBreakRules" == sName )
				m_oUseAltKinsokuLineBreakRules = oReader;
			else if ( L"w:allowSpaceOfSameStyleInTable" == sName )
				m_oAllowSpaceOfSameStyleInTable = oReader;
			else if ( L"w:doNotSuppressIndentation" == sName )
				m_oDoNotSuppressIndentation = oReader;
			else if ( L"w:doNotAutofitConstrainedTables" == sName )
				m_oDoNotAutofitConstrainedTables = oReader;
			else if ( L"w:autofitToFirstFixedWidthCell" == sName )
				m_oAutofitToFirstFixedWidthCell = oReader;
			else if ( L"w:underlineTabInNumList" == sName )
				m_oUnderlineTabInNumList = oReader;
			else if ( L"w:displayHangulFixedWidth" == sName )
				m_oDisplayHangulFixedWidth = oReader;
			else if ( L"w:splitPgBreakAndParaMark" == sName )
				m_oSplitPgBreakAndParaMark = oReader;
			else if ( L"w:doNotVertAlignCellWithSp" == sName )
				m_oDoNotVertAlignCellWithSp = oReader;
			else if ( L"w:doNotBreakConstrainedForcedTable" == sName )
				m_oDoNotBreakConstrainedForcedTable = oReader;
			else if ( L"w:doNotVertAlignInTxbx" == sName )
				m_oDoNotVertAlignInTxbx = oReader;
			else if ( L"w:useAnsiKerningPairs" == sName )
				m_oUseAnsiKerningPairs = oReader;
			else if ( L"w:cachedColBalance" == sName )
				m_oCachedColBalance = oReader;
			else if ( L"w:compatSetting" == sName )
			{
				OOX::Settings::CCompatSetting *oCS = new OOX::Settings::CCompatSetting(oReader);
				if (oCS)m_arrCompatSettings.push_back( oCS );
			}
		}
	}
    std::wstring CCompat::toXML() const
	{
        std::wstring sResult = L"<w:compat>";

		WritingElement_WriteNode_1( L"<w:useSingleBorderforContiguousCells ",      m_oUseSingleBorderforContiguousCells );
		WritingElement_WriteNode_1( L"<w:wpJustification ",      m_oWpJustification );
		WritingElement_WriteNode_1( L"<w:noTabHangInd ",      m_oNoTabHangInd );
		WritingElement_WriteNode_1( L"<w:noLeading ",      m_oNoLeading );
		WritingElement_WriteNode_1( L"<w:spaceForUL ",      m_oSpaceForUL );
		WritingElement_WriteNode_1( L"<w:noColumnBalance ",      m_oNoColumnBalance );
		WritingElement_WriteNode_1( L"<w:balanceSingleByteDoubleByteWidth ",      m_oBalanceSingleByteDoubleByteWidth );
		WritingElement_WriteNode_1( L"<w:noExtraLineSpacing ",      m_oNoExtraLineSpacing );
		WritingElement_WriteNode_1( L"<w:doNotLeaveBackslashAlone ",      m_oDoNotLeaveBackslashAlone );
		WritingElement_WriteNode_1( L"<w:ulTrailSpace ",      m_oUlTrailSpace );
		WritingElement_WriteNode_1( L"<w:doNotExpandShiftReturn ",      m_oDoNotExpandShiftReturn );
		WritingElement_WriteNode_1( L"<w:spacingInWholePoints ",      m_oSpacingInWholePoints );
		WritingElement_WriteNode_1( L"<w:lineWrapLikeWord6 ",      m_oLineWrapLikeWord6 );
		WritingElement_WriteNode_1( L"<w:printBodyTextBeforeHeader ",      m_oPrintBodyTextBeforeHeader );
		WritingElement_WriteNode_1( L"<w:printColBlack ",      m_oPrintColBlack );
		WritingElement_WriteNode_1( L"<w:wpSpaceWidth ",      m_oWpSpaceWidth );
		WritingElement_WriteNode_1( L"<w:showBreaksInFrames ",      m_oShowBreaksInFrames );
		WritingElement_WriteNode_1( L"<w:subFontBySize ",      m_oSubFontBySize );
		WritingElement_WriteNode_1( L"<w:suppressBottomSpacing ",      m_oSuppressBottomSpacing );
		WritingElement_WriteNode_1( L"<w:suppressTopSpacing ",      m_oSuppressTopSpacing );
		WritingElement_WriteNode_1( L"<w:suppressSpacingAtTopOfPage ",      m_oSuppressSpacingAtTopOfPage );
		WritingElement_WriteNode_1( L"<w:suppressTopSpacingWP ",      m_oSuppressTopSpacingWP );
		WritingElement_WriteNode_1( L"<w:suppressSpBfAfterPgBrk ",      m_oSuppressSpBfAfterPgBrk );
		WritingElement_WriteNode_1( L"<w:swapBordersFacingPages ",      m_oSwapBordersFacingPages );
		WritingElement_WriteNode_1( L"<w:convMailMergeEsc ",      m_oConvMailMergeEsc );
		WritingElement_WriteNode_1( L"<w:truncateFontHeightsLikeWP6 ",      m_oTruncateFontHeightsLikeWP6 );
		WritingElement_WriteNode_1( L"<w:mwSmallCaps ",      m_oMwSmallCaps );
		WritingElement_WriteNode_1( L"<w:usePrinterMetrics ",      m_oUsePrinterMetrics );
		WritingElement_WriteNode_1( L"<w:doNotSuppressParagraphBorders ",      m_oDoNotSuppressParagraphBorders );
		WritingElement_WriteNode_1( L"<w:wrapTrailSpaces ",      m_oWrapTrailSpaces );
		WritingElement_WriteNode_1( L"<w:footnoteLayoutLikeWW8 ",      m_oFootnoteLayoutLikeWW8 );
		WritingElement_WriteNode_1( L"<w:shapeLayoutLikeWW8 ",      m_oShapeLayoutLikeWW8 );
		WritingElement_WriteNode_1( L"<w:alignTablesRowByRow ",      m_oAlignTablesRowByRow );
		WritingElement_WriteNode_1( L"<w:forgetLastTabAlignment ",      m_oForgetLastTabAlignment );
		WritingElement_WriteNode_1( L"<w:adjustLineHeightInTable ",      m_oAdjustLineHeightInTable );
		WritingElement_WriteNode_1( L"<w:autoSpaceLikeWord95 ",      m_oAutoSpaceLikeWord95 );
		WritingElement_WriteNode_1( L"<w:noSpaceRaiseLower ",      m_oNoSpaceRaiseLower );
		WritingElement_WriteNode_1( L"<w:doNotUseHTMLParagraphAutoSpacing ",      m_oDoNotUseHTMLParagraphAutoSpacing );
		WritingElement_WriteNode_1( L"<w:layoutRawTableWidth ",      m_oLayoutRawTableWidth );
		WritingElement_WriteNode_1( L"<w:layoutTableRowsApart ",      m_oLayoutTableRowsApart );
		WritingElement_WriteNode_1( L"<w:useWord97LineBreakRules ",      m_oUseWord97LineBreakRules );
		WritingElement_WriteNode_1( L"<w:doNotBreakWrappedTables ",      m_oDoNotBreakWrappedTables );
		WritingElement_WriteNode_1( L"<w:doNotSnapToGridInCell ",      m_oDoNotSnapToGridInCell );
		WritingElement_WriteNode_1( L"<w:selectFldWithFirstOrLastChar ",      m_oSelectFldWithFirstOrLastChar );
		WritingElement_WriteNode_1( L"<w:applyBreakingRules ",      m_oApplyBreakingRules );
		WritingElement_WriteNode_1( L"<w:doNotWrapTextWithPunct ",      m_oDoNotWrapTextWithPunct );
		WritingElement_WriteNode_1( L"<w:doNotUseEastAsianBreakRules ",      m_oDoNotUseEastAsianBreakRules );
		WritingElement_WriteNode_1( L"<w:useWord2002TableStyleRules ",      m_oUseWord2002TableStyleRules );
		WritingElement_WriteNode_1( L"<w:growAutofit ",      m_oGrowAutofit );
		WritingElement_WriteNode_1( L"<w:useFELayout ",      m_oUseFELayout );
		WritingElement_WriteNode_1( L"<w:useNormalStyleForList ",      m_oUseNormalStyleForList );
		WritingElement_WriteNode_1( L"<w:doNotUseIndentAsNumberingTabStop ",      m_oDoNotUseIndentAsNumberingTabStop );
		WritingElement_WriteNode_1( L"<w:useAltKinsokuLineBreakRules ",      m_oUseAltKinsokuLineBreakRules );
		WritingElement_WriteNode_1( L"<w:allowSpaceOfSameStyleInTable ",      m_oAllowSpaceOfSameStyleInTable );
		WritingElement_WriteNode_1( L"<w:doNotSuppressIndentation ",      m_oDoNotSuppressIndentation );
		WritingElement_WriteNode_1( L"<w:doNotAutofitConstrainedTables ",      m_oDoNotAutofitConstrainedTables );
		WritingElement_WriteNode_1( L"<w:autofitToFirstFixedWidthCell ",      m_oAutofitToFirstFixedWidthCell );
		WritingElement_WriteNode_1( L"<w:underlineTabInNumList ",      m_oUnderlineTabInNumList );
		WritingElement_WriteNode_1( L"<w:displayHangulFixedWidth ",      m_oDisplayHangulFixedWidth );
		WritingElement_WriteNode_1( L"<w:splitPgBreakAndParaMark ",      m_oSplitPgBreakAndParaMark );
		WritingElement_WriteNode_1( L"<w:doNotVertAlignCellWithSp ",      m_oDoNotVertAlignCellWithSp );
		WritingElement_WriteNode_1( L"<w:doNotBreakConstrainedForcedTable ",      m_oDoNotBreakConstrainedForcedTable );
		WritingElement_WriteNode_1( L"<w:doNotVertAlignInTxbx ",      m_oDoNotVertAlignInTxbx );
		WritingElement_WriteNode_1( L"<w:useAnsiKerningPairs ",      m_oUseAnsiKerningPairs );
		WritingElement_WriteNode_1( L"<w:cachedColBalance ",      m_oCachedColBalance );

		for ( unsigned int nIndex = 0; nIndex < m_arrCompatSettings.size(); nIndex++ )
		{
			if (m_arrCompatSettings[nIndex])
				sResult += m_arrCompatSettings[nIndex]->toXML();
		}

		sResult += L"</w:compat>";

		return sResult;
	}

	void CDocProtect::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CDocProtect::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CDocProtect::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    std::wstring CDocProtect::toXML() const
	{
        std::wstring sResult = L"<w:documentProtection ";

		ComplexTypes_WriteAttribute ( L"w:edit=\"", m_oEdit );
		sResult += m_oFormatting.ToAttribute( L"w:formatting");
		sResult += m_oEnforcment.ToAttribute( L"w:enforcement");

		if (m_oCryptProviderType.IsInit())
		{
			ComplexTypes_WriteAttribute(L"w:cryptProviderType=\"", m_oCryptProviderType);
			sResult += m_oAlgIdExt.ToAttribute(L"w:algIdEx");
			sResult += m_oAlgIdExtSource.ToAttribute(L"w:algIdExtSource");
			ComplexTypes_WriteAttribute(L"w:cryptAlgorithmClass=\"", m_oCryptAlgorithmClass);
			ComplexTypes_WriteAttribute(L"w:cryptAlgorithmType=\"", m_oCryptAlgorithmType);
			sResult += m_oCryptAlgorithmSid.ToAttribute(L"w:cryptAlgorithmSid");
			sResult += m_oCryptProvider.ToAttribute(L"w:cryptProvider");
			sResult += m_oCryptProviderTypeExt.ToAttribute(L"w:cryptProviderTypeExt");
			sResult += m_oCryptProviderTypeExtSource.ToAttribute(L"w:cryptProviderTypeExtSource");

			sResult += m_oSpinCount.ToAttribute(L"w:cryptSpinCount");
			sResult += m_sHashValue.ToAttribute(L"w:hash");
			sResult += m_sSaltValue.ToAttribute(L"w:salt");
		}
		else
		{
			ComplexTypes_WriteAttribute(L"w:algorithmName=\"", m_oAlgorithmName);
			sResult += m_sHashValue.ToAttribute(L"w:hashValue");
			sResult += m_sSaltValue.ToAttribute(L"w:saltValue");
			sResult += m_oSpinCount.ToAttribute(L"w:spinCount");
		}
		sResult += L"/>";

		return sResult;
	}

	void CDocProtect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'a':
					if      ( L"w:algorithmName" == wsName ) m_oAlgorithmName = oReader.GetText();
					else if (L"w:algIdExt" == wsName) m_oAlgIdExt = oReader.GetText();
					else if (L"w:algIdExtSource" == wsName)	 m_oAlgIdExtSource = oReader.GetText();
					break;
				case 'c':
					if (L"w:cryptProviderType" == wsName) m_oCryptProviderType = oReader.GetText();
					else if (L"w:cryptProvider" == wsName)	m_oCryptProvider = oReader.GetText();
					else if (L"w:cryptProviderTypeExt" == wsName)	m_oCryptProviderTypeExt = oReader.GetText();
					else if (L"w:cryptProviderTypeExtSource" == wsName)	m_oCryptProviderTypeExtSource = oReader.GetText();
					else if (L"w:cryptAlgorithmSid" == wsName) m_oCryptAlgorithmSid = oReader.GetText();
					else if (L"w:cryptAlgorithmType" == wsName) m_oCryptAlgorithmType = oReader.GetText();
					else if (L"w:cryptAlgorithmClass" == wsName) m_oCryptAlgorithmClass = oReader.GetText();
					else if (L"w:cryptSpinCount" == wsName) m_oSpinCount = oReader.GetText();
					break;
				case 'e':
					if      ( L"w:edit"          == wsName ) m_oEdit          = oReader.GetText();
					else if ( L"w:enforcement"   == wsName ) m_oEnforcment    = oReader.GetText();
					break;
				case 'f':
					if      ( L"w:formatting"    == wsName ) m_oFormatting    = oReader.GetText();
					break;
				case 'h':
					if      ( L"w:hashValue"     == wsName ) m_sHashValue     = oReader.GetText();
					else if (L"w:hash" == wsName) m_sHashValue = oReader.GetText();
					break;
				case 's':
					if      ( L"w:saltValue"     == wsName ) m_sSaltValue     = oReader.GetText();
					else if (L"w:salt"			 == wsName)	m_sSaltValue	= oReader.GetText();
					else if ( L"w:spinCount"     == wsName ) m_oSpinCount     = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

	std::wstring CReadingModeInkLockDown::toXML() const
	{
		std::wstring sResult = L"<w:readModeInkLockDown ";

		sResult += L"w:w=\"" + m_oW.ToString()
			+ L"\" w:h=\"" + m_oH.ToString()
			+ L"\" w:fontSz=\"" + m_oFontSz.ToString()
			+ L"\" w:actualPg=\"" + m_oActualPg.ToString() + L"\"/>";

		return sResult;
	}
	void CReadingModeInkLockDown::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'a':
					if      ( L"w:actualPg"  == wsName ) m_oActualPg = oReader.GetText();
					break;

				case 'f':
					if      ( L"w:fontSz"    == wsName ) m_oFontSz   = oReader.GetText();
					break;

				case 'h':
					if      ( L"w:h"         == wsName ) m_oH        = oReader.GetText();
					break;

				case 'w':
					if      ( L"w:w"         == wsName ) m_oW        = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

    std::wstring CTrackChangesView::toXML() const
	{
        std::wstring sResult = L"<w:revisionView ";

		sResult += m_oMarkup.ToAttribute( L"w:markup");
		sResult += m_oComments.ToAttribute( L"w:comments");
		sResult += m_oInsDel.ToAttribute( L"w:insDel");
		sResult += m_oFormatting.ToAttribute( L"w:formatting");
		sResult += m_oInkAnnotataions.ToAttribute( L"w:inkAnnotations");

		sResult += L"/>";

		return sResult;
	}
	void CTrackChangesView::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'c':
					if      ( L"w:comments" == wsName ) m_oComments = oReader.GetText();
					break;

				case 'f':
					if      ( L"w:formatting" == wsName ) m_oFormatting = oReader.GetText();
					break;

				case 'i':
					if      ( L"w:inkAnnotations" == wsName ) m_oInkAnnotataions = oReader.GetText();
					else if ( L"w:insDel" == wsName ) m_oInsDel = oReader.GetText();
					break;

				case 'm':
					if      ( L"w:markup" == wsName ) m_oMarkup = oReader.GetText();
					break;
				}
			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

    std::wstring CStylePaneFilter::toXML() const
	{
        std::wstring sResult = L"<w:stylePaneFormatFilter ";

		sResult += m_oAllStyles.ToAttribute( L"w:allStyles");
		sResult += m_oAlternateStyleNames.ToAttribute( L"w:alternateStyleNames");
		sResult += m_oClearFormatting.ToAttribute( L"w:clearFormatting");
		sResult += m_oCustomStyles.ToAttribute( L"w:customStyles");
		sResult += m_oDirectFormattingOnNumbering.ToAttribute( L"w:directFormattingOnNumbering");
		sResult += m_oDirectFormattingOnParagraphs.ToAttribute( L"w:directFormattingOnParagraphs");
		sResult += m_oDirectFormattingOnRuns.ToAttribute( L"w:directFormattingOnRuns");
		sResult += m_oDirectFormattingOnTables.ToAttribute( L"w:directFormattingOnTables");
		sResult += m_oHeadingStyles.ToAttribute( L"w:headingStyles");
		sResult += m_oLatentStyles.ToAttribute( L"w:latentStyles");
		sResult += m_oNumberingStyles.ToAttribute( L"w:numberingStyles");
		sResult += m_oStylesInUse.ToAttribute( L"w:stylesInUse");
		sResult += m_oTableStyles.ToAttribute( L"w:tableStyles");
		sResult += m_oTop3HeadingStyles.ToAttribute( L"w:top3HeadingStyles");
		sResult += m_oVisibleStyles.ToAttribute( L"w:visibleStyles");

		sResult += L"/>";
		return sResult;
	}
	void CStylePaneFilter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'a':
					if      ( L"w:allStyles"           == wsName ) m_oAllStyles           = oReader.GetText();
					else if ( L"w:alternateStyleNames" == wsName ) m_oAlternateStyleNames = oReader.GetText();
					break;

				case 'c':
					if      ( L"w:clearFormatting" == wsName ) m_oClearFormatting = oReader.GetText();
					else if ( L"w:customStyles"    == wsName ) m_oCustomStyles    = oReader.GetText();
					break;

				case 'd':
					if      ( L"w:directFormattingOnNumbering"  == wsName ) m_oDirectFormattingOnNumbering  = oReader.GetText();
					else if ( L"w:directFormattingOnParagraphs" == wsName ) m_oDirectFormattingOnParagraphs = oReader.GetText();
					else if ( L"w:directFormattingOnRuns"       == wsName ) m_oDirectFormattingOnRuns       = oReader.GetText();
					else if ( L"w:directFormattingOnTables"     == wsName ) m_oDirectFormattingOnTables     = oReader.GetText();
					break;

				case 'h':
					if      ( L"w:headingStyles" == wsName ) m_oHeadingStyles = oReader.GetText();
					break;

				case 'l':
					if      ( L"w:latentStyles" == wsName ) m_oLatentStyles = oReader.GetText();
					break;

				case 'n':
					if      ( L"w:numberingStyles" == wsName ) m_oNumberingStyles = oReader.GetText();
					break;

				case 's':
					if      ( L"w:stylesInUse" == wsName ) m_oStylesInUse = oReader.GetText();
					break;

				case 't':
					if      ( L"w:tableStyles"       == wsName ) m_oTableStyles       = oReader.GetText();
					else if ( L"w:top3HeadingStyles" == wsName ) m_oTop3HeadingStyles = oReader.GetText();
					break;

				case 'v':
					if      ( L"w:visibleStyles" == wsName ) m_oVisibleStyles = oReader.GetText();
					break;
				}

			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

    std::wstring CWriteProtection::toXML() const
	{
        std::wstring sResult = L"<w:writeProtection ";

		sResult += m_oRecommended.ToAttribute( L"w:recommended");

		if (m_oCryptProviderType.IsInit())
		{
			ComplexTypes_WriteAttribute(L"w:cryptProviderType=\"", m_oCryptProviderType);
			sResult += m_oAlgIdExt.ToAttribute(L"w:algIdEx");
			sResult += m_oAlgIdExtSource.ToAttribute(L"w:algIdExtSource");
			ComplexTypes_WriteAttribute(L"w:cryptAlgorithmClass=\"", m_oCryptAlgorithmClass);
			ComplexTypes_WriteAttribute(L"w:cryptAlgorithmType=\"", m_oCryptAlgorithmType);
			sResult += m_oCryptAlgorithmSid.ToAttribute(L"w:cryptAlgorithmSid");
			sResult += m_oCryptProvider.ToAttribute(L"w:cryptProvider");
			sResult += m_oCryptProviderTypeExt.ToAttribute(L"w:cryptProviderTypeExt");
			sResult += m_oCryptProviderTypeExtSource.ToAttribute(L"w:cryptProviderTypeExtSource");

			sResult += m_oSpinCount.ToAttribute(L"w:cryptSpinCount");
			sResult += m_sHashValue.ToAttribute(L"w:hash");
			sResult += m_sSaltValue.ToAttribute(L"w:salt");
		}
		else
		{
			ComplexTypes_WriteAttribute(L"w:algorithmName=\"", m_oAlgorithmName);
			sResult += m_sHashValue.ToAttribute(L"w:hashValue");
			sResult += m_sSaltValue.ToAttribute(L"w:saltValue");
			sResult += m_oSpinCount.ToAttribute(L"w:spinCount");
		}

		sResult += L"/>";

		return sResult;
	}
	void CWriteProtection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			wchar_t wsChar0 = wsName[0]; 
			wchar_t wsChar2 = wsName[2]; // w:_

			if ( 'w' == wsChar0 )
			{
				switch ( wsChar2 )
				{
				case 'a':
					if ( L"w:algorithmName" == wsName )	 m_oAlgorithmName = oReader.GetText();
					else if (L"w:algIdExt" == wsName)	 m_oAlgIdExt = oReader.GetText();
					else if (L"w:algIdExtSource" == wsName)	 m_oAlgIdExtSource = oReader.GetText();
					break;
				case 'c':
					if      ( L"w:cryptProviderType"	== wsName ) m_oCryptProviderType	= oReader.GetText();
					else if (L"w:cryptProvider"			== wsName)	m_oCryptProvider = oReader.GetText();
					else if (L"w:cryptProviderTypeExt" == wsName)	m_oCryptProviderTypeExt = oReader.GetText();
					else if (L"w:cryptProviderTypeExtSource" == wsName)	m_oCryptProviderTypeExtSource = oReader.GetText();
					else if	( L"w:cryptAlgorithmSid"	== wsName ) m_oCryptAlgorithmSid	= oReader.GetText();
					else if	( L"w:cryptAlgorithmType"	== wsName ) m_oCryptAlgorithmType	= oReader.GetText();
					else if	( L"w:cryptAlgorithmClass"	== wsName ) m_oCryptAlgorithmClass	= oReader.GetText();
					else if	( L"w:cryptSpinCount"		== wsName ) m_oSpinCount			= oReader.GetText();
					break;
				case 'h':
					if      ( L"w:hashValue"	== wsName ) m_sHashValue     = oReader.GetText();
					else if	( L"w:hash"			== wsName ) m_sHashValue     = oReader.GetText();
					break;
				case 'r':
					if      ( L"w:recommended"   == wsName ) m_oRecommended   = oReader.GetText();
					break;
				case 's':
					if      ( L"w:saltValue"	== wsName ) m_sSaltValue     = oReader.GetText();
					else if ( L"w:salt"			== wsName ) m_sSaltValue     = oReader.GetText();
					else if ( L"w:spinCount"	== wsName ) m_oSpinCount     = oReader.GetText();
					break;
				}
			}

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}

	void CEdnDocProps::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = oReader.GetName();
			if ( L"w:numFmt" == sName )
				m_oNumFmt = oReader;
			else if ( L"w:numRestart" == sName )
				m_oNumRestart = oReader;
			else if ( L"w:numStart" == sName )
				m_oNumStart = oReader;
			else if ( L"w:pos" == sName )
				m_oPos = oReader;
			else if ( L"w:endnote" == sName )
			{
				OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
				if (oFE) m_arrEndnote.push_back( oFE );
			}
		}
	}
    std::wstring CEdnDocProps::toXML() const
	{
        std::wstring sResult = L"<w:endnotePr>";

		if (m_oPos.IsInit())
		{
			sResult += L"<w:pos ";
			sResult += m_oPos->ToString();
			sResult += L"/>";
		}	
		if ( m_oNumFmt.IsInit() )
		{
			sResult += L"<w:numFmt ";
			sResult += m_oNumFmt->ToString();
			sResult += L"/>";
		}
		if ( m_oNumStart.IsInit() )
		{
			sResult += L"<w:numStart ";
			sResult += m_oNumStart->ToString();
			sResult += L"/>";
		}
		if ( m_oNumRestart.IsInit() )
		{
			sResult += L"<w:numRestart ";
			sResult += m_oNumRestart->ToString();
			sResult += L"/>";
		}
		for (unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++)
		{
			if (m_arrEndnote[nIndex])
				sResult += m_arrEndnote[nIndex]->toXML();
		}
		sResult += L"</w:endnotePr>";

		return sResult;
	}
	
	void CFtnDocProps::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nParentDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nParentDepth ) )
		{
			std::wstring sName = oReader.GetName();
			if ( L"w:numFmt" == sName )
				m_oNumFmt = oReader;
			else if ( L"w:numRestart" == sName )
				m_oNumRestart = oReader;
			else if ( L"w:numStart" == sName )
				m_oNumStart = oReader;
			else if ( L"w:pos" == sName )
				m_oPos = oReader;
			else if ( L"w:footnote" == sName )
			{
				OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
				if (oFE) m_arrFootnote.push_back( oFE );
			}
		}
	}
    std::wstring CFtnDocProps::toXML() const
	{
        std::wstring sResult = L"<w:footnotePr>";

		if (m_oPos.IsInit())
		{
			sResult += L"<w:pos ";
			sResult += m_oPos->ToString();
			sResult += L"/>";
		}
		if ( m_oNumFmt.IsInit() )
		{
			sResult += L"<w:numFmt ";
			sResult += m_oNumFmt->ToString();
			sResult += L"/>";
		}
		if (m_oNumStart.IsInit())
		{
			sResult += L"<w:numStart ";
			sResult += m_oNumStart->ToString();
			sResult += L"/>";
		}
		if ( m_oNumRestart.IsInit() )
		{
			sResult += L"<w:numRestart ";
			sResult += m_oNumRestart->ToString();
			sResult += L"/>";
		}
		for (unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++)
		{
			if (m_arrFootnote[nIndex])
				sResult += m_arrFootnote[nIndex]->toXML();
		}
		sResult += L"</w:footnotePr>";

		return sResult;
	}

    std::wstring CSchema::toXML() const
	{
        std::wstring sResult = L"<sl:schema sl:uri=\"" + m_sUri
			+ L"\" sl:manifestLocation=\"" + m_sManifestLocation 
			+ L"\" sl:schemaLocation=\"" + m_sShemaLocation
			+ L"\" sl:schemaLanguage=\"" + m_sShemaLanguage + L"\"/>";
		return sResult;
	}
	void CSchema::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.GetAttributesCount() <= 0 )
			return;
				
		if ( !oReader.MoveToFirstAttribute() )
			return;
				
		std::wstring wsName = oReader.GetName();
		while( !wsName.empty() )
		{
			if ( L"sl:uri" == wsName ) 
				m_sUri = oReader.GetText();
			else if ( L"sl:schemaLocation" == wsName ) 
				m_sShemaLocation = oReader.GetText();
			else if ( L"sl:schemaLanguage" == wsName ) 
				m_sShemaLanguage = oReader.GetText();
			else if ( L"sl:manifestLocation" == wsName ) 
				m_sManifestLocation = oReader.GetText();

			if ( !oReader.MoveToNextAttribute() )
				break;

			wsName = oReader.GetName();
		}
		oReader.MoveToElement();
	}	
} // namespace Settings

	void CSettings::SetDefaults()
	{
		m_oDefaultTabStop.Init();
		m_oDefaultTabStop->m_oVal.Init();
		m_oDefaultTabStop->m_oVal->FromTwips(708);

		m_oZoom.Init();
		m_oZoom->m_oPercent.SetValue(100);
		
		m_oCharacterSpacingControl.Init();
		m_oCharacterSpacingControl->m_oVal.SetValue(SimpleTypes::charspacingDoNotCompress);

		m_oThemeFontLang.Init();
		m_oThemeFontLang->m_oEastAsia = L"zh-CN";
		m_oThemeFontLang->m_oVal = L"en-US";

		m_oShapeDefaults = new Settings::CShapeDefaults(et_w_shapeDefaults);
		
		OOX::VmlOffice::CShapeDefaults *vDefault = new OOX::VmlOffice::CShapeDefaults();
		vDefault->m_oExt.Init(); vDefault->m_oExt->SetValue(SimpleTypes::extEdit);
		vDefault->m_oSpIdMax.SetValue(1026);
		m_oShapeDefaults->m_arrItems.push_back(vDefault);

		OOX::VmlOffice::CShapeLayout *vLayout = new OOX::VmlOffice::CShapeLayout();
		vLayout->m_oExt.Init(); vLayout->m_oExt->SetValue(SimpleTypes::extEdit);
		vLayout->m_oIdMap.Init(); vLayout->m_oIdMap->m_sData = L"1"; 
		vLayout->m_oIdMap->m_oExt.Init(); vLayout->m_oIdMap->m_oExt->SetValue(SimpleTypes::extEdit);
		m_oShapeDefaults->m_arrItems.push_back(vLayout);
	}
	void CSettings::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode()) return;

		std::wstring sName = oReader.GetName();

		if ( L"w:settings" != sName && L"w:docPr" != sName) return;

		int nStylesDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
		{
			sName = oReader.GetName();

			wchar_t wChar0 = sName[0];
			wchar_t wChar2 = sName[2];

			if ( 'w' == wChar0 )
			{
				switch ( wChar2 )
				{
				case 'a':
					if      ( L"w:activeWritingStyle"         == sName ) m_oActiveWritingStyle         = oReader;
					else if ( L"w:alignBordersAndEdges"       == sName ) m_oAlignBordersAndEdges       = oReader;
					else if ( L"w:alwaysMergeEmptyNamespace"  == sName ) m_oAlwaysMergeEmptyNamespace  = oReader;
					else if ( L"w:alwaysShowPlaceholderText"  == sName ) m_oAlwaysShowPlaceholderText  = oReader;
					else if ( L"w:attachedSchema"             == sName ) m_oAttachedSchema             = oReader;
					else if ( L"w:attachedTemplate"           == sName ) m_oAttachedTemplate           = oReader;
					else if ( L"w:autoFormatOverride"         == sName ) m_oAutoFormatOverride         = oReader;
					else if ( L"w:autoHyphenation"            == sName ) m_oAutoHyphenation            = oReader;
					break;
				case 'b':
					if      ( L"w:bookFoldPrinting"           == sName ) m_oBookFoldPrinting           = oReader;
					else if ( L"w:bookFoldPrintingSheets"     == sName ) m_oBookFoldPrintingSheets     = oReader;
					else if ( L"w:bookFoldRevPrinting"        == sName ) m_oBookFoldRevPrinting        = oReader;
					else if ( L"w:bordersDoNotSurroundFooter" == sName ) m_oBordersDoNotSurroundFooter = oReader;
					else if ( L"w:bordersDoNotSurroundHeader" == sName ) m_oBordersDoNotSurroundHeader = oReader;
					break;
				case 'c':
					if      ( L"w:captions"                   == sName ) m_oCaptions                   = oReader;
					else if ( L"w:characterSpacingControl"    == sName ) m_oCharacterSpacingControl    = oReader;
					else if ( L"w:clickAndTypeStyle"          == sName ) m_oClickAndTypeStyle          = oReader;
					else if ( L"w:clrSchemeMapping"           == sName ) m_oClrSchemeMapping           = oReader;
					else if ( L"w:compat"                     == sName ) m_oCompat                     = oReader;
					else if ( L"w:consecutiveHyphenLimit"     == sName ) m_oConsecutiveHyphenLimit     = oReader;
					break;
				case 'd':
					if      ( L"w:decimalSymbol"              == sName ) m_oDecimalSymbol              = oReader;
					else if ( L"w:defaultTableStyle"          == sName ) m_oDefaultTableStyle          = oReader;
					else if ( L"w:defaultTabStop"             == sName ) m_oDefaultTabStop             = oReader;
					else if ( L"w:displayBackgroundShape"     == sName ) m_oDisplayBackgroundShape     = oReader;
					else if ( L"w:displayHorizontalDrawingGridEvery" == sName ) m_oDisplayHorizontalDrawingGridEvery = oReader;
					else if ( L"w:displayVerticalDrawingGridEvery"   == sName ) m_oDisplayVerticalDrawingGridEvery   = oReader;
					else if ( L"w:documentProtection"         == sName ) m_oDocumentProtection         = oReader;
					else if ( L"w:documentType"               == sName ) m_oDocumentType               = oReader;
					else if ( L"w:docVars"                    == sName ) m_oDocVars                    = oReader;
					else if ( L"w:doNotAutoCompressPictures"  == sName ) m_oDoNotAutoCompressPictures  = oReader;
					else if ( L"w:doNotDemarcateInvalidXml"   == sName ) m_oDoNotDemarcateInvalidXml   = oReader;
					else if ( L"w:doNotDisplayPageBoundaries" == sName ) m_oDoNotDisplayPageBoundaries = oReader;
					else if ( L"w:doNotEmbedSmartTags"        == sName ) m_oDoNotEmbedSmartTags        = oReader;
					else if ( L"w:doNotHyphenateCaps"         == sName ) m_oDoNotHyphenateCaps         = oReader;
					else if ( L"w:doNotIncludeSubdocsInStats" == sName ) m_oDoNotIncludeSubdocsInStats = oReader;
					else if ( L"w:doNotShadeFormData"         == sName ) m_oDoNotShadeFormData         = oReader;
					else if ( L"w:doNotTrackFormatting"       == sName ) m_oDoNotTrackFormatting       = oReader;
					else if ( L"w:doNotTrackMoves"            == sName ) m_oDoNotTrackMoves            = oReader;
					else if ( L"w:doNotUseMarginsForDrawingGridOrigin" == sName ) m_oDoNotUseMarginsForDrawingGridOrigin = oReader;
					else if ( L"w:doNotValidateAgainstSchema" == sName ) m_oDoNotValidateAgainstSchema = oReader;
					else if ( L"w:drawingGridHorizontalOrigin"  == sName ) m_oDrawingGridHorizontalOrigin  = oReader;
					else if ( L"w:drawingGridHorizontalSpacing" == sName ) m_oDrawingGridHorizontalSpacing = oReader;
					else if ( L"w:drawingGridVerticalOrigin"  == sName ) m_oDrawingGridVerticalOrigin  = oReader;
					else if ( L"w:drawingGridVerticalSpacing" == sName ) m_oDrawingGridVerticalSpacing = oReader;
					break;
				case 'e':
					if      ( L"w:embedSystemFonts"           == sName ) m_oEmbedSystemFonts           = oReader;
					else if ( L"w:embedTrueTypeFonts"         == sName ) m_oEmbedTrueTypeFonts         = oReader;
					else if ( L"w:endnotePr"                  == sName ) m_oEndnotePr                  = oReader;
					else if ( L"w:evenAndOddHeaders"          == sName ) m_oEvenAndOddHeaders          = oReader;
					break;
				case 'f':
					if      ( L"w:footnotePr"                 == sName ) m_oFootnotePr                 = oReader;
					else if ( L"w:forceUpgrade"               == sName ) m_oForceUpgrade               = oReader;
					else if ( L"w:formsDesign"                == sName ) m_oFormsDesign                = oReader;
					break;
				case 'g':
					if      ( L"w:gutterAtTop"                == sName ) m_oGutterAtTop                = oReader;
					break;
				case 'h':
					if      ( L"w:hdrShapeDefaults"           == sName ) m_oHdrShapeDefaults           = oReader;
					else if ( L"w:hideGrammaticalErrors"      == sName ) m_oHideGrammaticalErrors      = oReader;
					else if ( L"w:hideSpellingErrors"         == sName ) m_oHideSpellingErrors         = oReader;
					else if ( L"w:hyphenationZone"            == sName ) m_oHyphenationZone            = oReader;
					break;
				case 'i':
					if      ( L"w:ignoreMixedContent"         == sName ) m_oIgnoreMixedContent         = oReader;
					break;
				case 'l':
					if      ( L"w:linkStyles"                 == sName ) m_oLinkStyles                 = oReader;
					else if ( L"w:listSeparator"              == sName ) m_oListSeparator              = oReader;
					break;
				case 'm':
					if      ( L"w:mirrorMargins"              == sName ) m_oMirrorMargins              = oReader;
					break;
				case 'n':
					if      ( L"w:noLineBreaksAfter"          == sName ) m_oNoLineBreaksAfter          = oReader;
					else if ( L"w:noLineBreaksBefore"         == sName ) m_oNoLineBreaksBefore         = oReader;
					else if ( L"w:noPunctuationKerning"       == sName ) m_oNoPunctuationKerning       = oReader;
					break;
				case 'p':
					if      ( L"w:printFormsData"             == sName ) m_oPrintFormsData             = oReader;
					else if ( L"w:printFractionalCharacterWidth" == sName ) m_oPrintFractionalCharacterWidth = oReader;
					else if ( L"w:printPostScriptOverText"    == sName ) m_oPrintPostScriptOverText    = oReader;
					else if ( L"w:printTwoOnOne"              == sName ) m_oPrintTwoOnOne              = oReader;
					else if ( L"w:proofState"                 == sName ) m_oProofState                 = oReader;
					break;
				case 'r':
					if      ( L"w:readModeInkLockDown"        == sName ) m_oReadModeInkLockDown        = oReader;
					else if ( L"w:removeDateAndTime"          == sName ) m_oRemoveDateAndTime          = oReader;
					else if ( L"w:removePersonalInformation"  == sName ) m_oRemovePersonalInformation  = oReader;
					else if ( L"w:revisionView"               == sName ) m_oRevisionView               = oReader;
					else if ( L"w:rsids"                      == sName ) m_oRsids                      = oReader;
					break;
				case 's':
					if      ( L"w:saveFormsData"              == sName ) m_oSaveFormsData              = oReader;
					else if ( L"w:saveInvalidXml"             == sName ) m_oSaveInvalidXml             = oReader;
					else if ( L"w:savePreviewPicture"         == sName ) m_oSavePreviewPicture         = oReader;
					else if ( L"w:saveSubsetFonts"            == sName ) m_oSaveSubsetFonts            = oReader;
					else if ( L"w:saveThroughXslt"            == sName ) m_oSaveThroughtXslt           = oReader;
					else if ( L"w:saveXmlDataOnly"            == sName ) m_oSaveXmlDataOnly            = oReader;
					else if ( L"w:shapeDefaults"              == sName ) m_oShapeDefaults              = oReader;
					else if ( L"w:showEnvelope"               == sName ) m_oShowEnvelope               = oReader;
					else if ( L"w:showXMLTags"                == sName ) m_oShowXMLTags                = oReader;
					else if ( L"w:smartTagType"               == sName )
					{
						OOX::Settings::CSmartTagType *oSTT = new OOX::Settings::CSmartTagType(oReader);
						if (oSTT) m_arrSmartTagType.push_back( oSTT );
					}
					else if ( L"w:strictFirstAndLastChars"    == sName ) m_oStrictFirstAndLastChars    = oReader;
					else if ( L"w:styleLockQFSet"             == sName ) m_oStyleLockQFSet             = oReader;
					else if ( L"w:styleLockTheme"             == sName ) m_oStyleLockTheme             = oReader;
					else if ( L"w:stylePaneFormatFilter"      == sName ) m_oStylePaneFormatFilter      = oReader;
					else if ( L"w:stylePaneSortMethod"        == sName ) m_oStylePaneSortMethod        = oReader;
					else if ( L"w:summaryLength"              == sName ) m_oSummaryLength              = oReader;
					break;
				case 't':
					if      ( L"w:themeFontLang"              == sName ) m_oThemeFontLang              = oReader;
					else if ( L"w:trackRevisions"             == sName ) m_oTrackRevisions             = oReader;
					break;
				case 'u':
					if      ( L"w:updateFields"               == sName ) m_oUpdateFields               = oReader;
					else if ( L"w:useXSLTWhenSaving"          == sName ) m_oUseXSLTWhenSaving          = oReader;
					break;
				case 'v':
					if      ( L"w:view"                       == sName ) m_oView                       = oReader;
					break;
				case 'w':
					if      ( L"w:writeProtection"            == sName ) m_oWriteProtection            = oReader;
					break;
				case 'z':
					if      ( L"w:zoom"                       == sName ) m_oZoom                       = oReader;
					break;
				}
			}
			else if ( 's' == wChar0 )
			{
				if ( L"sl:schemaLibrary" == sName )
					m_oSchemaLibrary = oReader;
			}
			else if ( 'm' == wChar0 )
			{
				if ( L"m:mathPr" == sName )
					m_oMathPr = oReader;
			}
		}
	}
	void CSettings::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

		sXml += toXML();

		CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
		oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
	}
	std::wstring CSettings::toXML() const
	{
		std::wstring sXml = L"<w:settings xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:sl=\"http://schemas.openxmlformats.org/schemaLibrary/2006/main\" mc:Ignorable=\"w14 w15\">";

		if ( m_oWriteProtection.IsInit() )
			sXml += m_oWriteProtection->toXML();

		if ( m_oView.IsInit() )
			sXml += m_oView->toXML();

		if ( m_oZoom.IsInit() )
			sXml += m_oZoom->toXML();

		if ( m_oRemovePersonalInformation.IsInit() )
			sXml += L"<w:removePersonalInformation " + m_oRemovePersonalInformation->ToString() + L"/>";

		if ( m_oRemoveDateAndTime.IsInit() )
			sXml += L"<w:removeDateAndTime " + m_oRemoveDateAndTime->ToString() + L"/>";

		if ( m_oDoNotDisplayPageBoundaries.IsInit() )
			sXml += L"<w:doNotDisplayPageBoundaries " + m_oDoNotDisplayPageBoundaries->ToString() + L"/>";

		if ( m_oDisplayBackgroundShape.IsInit() )
			sXml += L"<w:displayBackgroundShape " + m_oDisplayBackgroundShape->ToString() + L"/>";

		if ( m_oPrintPostScriptOverText.IsInit() )
			sXml += L"<w:printPostScriptOverText " + m_oPrintPostScriptOverText->ToString() + L"/>";

		if ( m_oPrintFractionalCharacterWidth.IsInit() )
			sXml += L"<w:printFractionalCharacterWidth " + m_oPrintFractionalCharacterWidth->ToString() + L"/>";

		if ( m_oPrintFormsData.IsInit() )
			sXml += L"<w:printFormsData " + m_oPrintFormsData->ToString() + L"/>";

		if ( m_oEmbedTrueTypeFonts.IsInit() )
			sXml += L"<w:embedTrueTypeFonts " + m_oEmbedTrueTypeFonts->ToString() + L"/>";

		if ( m_oEmbedSystemFonts.IsInit() )
			sXml += L"<w:embedSystemFonts " + m_oEmbedSystemFonts->ToString() + L"/>";

		if ( m_oSaveSubsetFonts.IsInit() )
			sXml += L"<w:saveSubsetFonts " + m_oSaveSubsetFonts->ToString() + L"/>";

		if ( m_oSaveFormsData.IsInit() )
			sXml += L"<w:saveFormsData " + m_oSaveFormsData->ToString() + L"/>";

		if ( m_oMirrorMargins.IsInit() )
			sXml += L"<w:mirrorMargins " + m_oMirrorMargins->ToString() + L"/>";

		if ( m_oAlignBordersAndEdges.IsInit() )
			sXml += L"<w:alignBordersAndEdges " + m_oAlignBordersAndEdges->ToString() + L"/>";

		if ( m_oBordersDoNotSurroundHeader.IsInit() )
			sXml += L"<w:bordersDoNotSurroundHeader " + m_oBordersDoNotSurroundHeader->ToString() + L"/>";

		if (m_oBordersDoNotSurroundFooter.IsInit())
			sXml += L"<w:bordersDoNotSurroundFooter " + m_oBordersDoNotSurroundFooter->ToString() + L"/>";

		if ( m_oGutterAtTop.IsInit() )
			sXml += L"<w:gutterAtTop " + m_oGutterAtTop->ToString() + L"/>";

		if ( m_oHideSpellingErrors.IsInit() )
			sXml += L"<w:hideSpellingErrors " + m_oHideSpellingErrors->ToString() + L"/>";

		if ( m_oHideGrammaticalErrors.IsInit() )
			sXml += L"<w:hideGrammaticalErrors " + m_oHideGrammaticalErrors->ToString() + L"/>";

		if ( m_oActiveWritingStyle.IsInit() )
			sXml += m_oActiveWritingStyle->toXML();

		if ( m_oProofState.IsInit() )
			sXml += m_oProofState->toXML();

		if ( m_oFormsDesign.IsInit() )
			sXml += L"<w:formsDesign " + m_oFormsDesign->ToString() + L"/>";

		if ( m_oAttachedTemplate.IsInit() )
			sXml += L"<w:attachedTemplate " + m_oAttachedTemplate->ToString() + L"/>";

		if ( m_oLinkStyles.IsInit() )
			sXml += L"<w:linkStyles " + m_oLinkStyles->ToString() + L"/>";

		if ( m_oStylePaneFormatFilter.IsInit() )
			sXml += m_oStylePaneFormatFilter->toXML();

		if ( m_oStylePaneSortMethod.IsInit() )
			sXml += m_oStylePaneSortMethod->toXML();

		if ( m_oDocumentType.IsInit() )
			sXml += m_oDocumentType->toXML();

		// TO DO: Здесь вставить запись w:mailMerge

		if ( m_oRevisionView.IsInit() )
			sXml += m_oRevisionView->toXML();

		if ( m_oTrackRevisions.IsInit() )
			sXml += L"<w:trackRevisions " + m_oTrackRevisions->ToString() + L"/>";

		if ( m_oDoNotTrackMoves.IsInit() )
			sXml += L"<w:doNotTrackMoves " + m_oDoNotTrackMoves->ToString() + L"/>";

		if ( m_oDoNotTrackFormatting.IsInit() )
			sXml += L"<w:doNotTrackFormatting " + m_oDoNotTrackFormatting->ToString() + L"/>";

		if ( m_oDocumentProtection.IsInit() )
			sXml += m_oDocumentProtection->toXML();

		if ( m_oAutoFormatOverride.IsInit() )
			sXml += L"<w:autoFormatOverride " + m_oAutoFormatOverride->ToString() + L"/>";

		if ( m_oStyleLockTheme.IsInit() )
			sXml += L"<w:styleLockTheme " + m_oStyleLockTheme->ToString() + L"/>";

		if ( m_oStyleLockQFSet.IsInit() )
			sXml += L"<w:styleLockQFSet " + m_oStyleLockQFSet->ToString() + L"/>";

		if ( m_oDefaultTabStop.IsInit() )
			sXml += L"<w:defaultTabStop " + m_oDefaultTabStop->ToString() + L"/>";

		if ( m_oAutoHyphenation.IsInit() )
			sXml += L"<w:autoHyphenation " + m_oAutoHyphenation->ToString() + L"/>";

		if ( m_oConsecutiveHyphenLimit.IsInit() )
			sXml += L"<w:consecutiveHyphenLimit " + m_oConsecutiveHyphenLimit->ToString() + L"/>";

		if ( m_oHyphenationZone.IsInit() )
			sXml += L"<w:hyphenationZone " + m_oHyphenationZone->ToString() + L"/>";

		if ( m_oDoNotHyphenateCaps.IsInit() )
			sXml += L"<w:doNotHyphenateCaps " + m_oDoNotHyphenateCaps->ToString() + L"/>";

		if ( m_oShowEnvelope.IsInit() )
			sXml += L"<w:showEnvelope " + m_oShowEnvelope->ToString() + L"/>";

		if ( m_oSummaryLength.IsInit() )
			sXml += L"<w:summaryLength " + m_oSummaryLength->ToString() + L"/>";

		if ( m_oClickAndTypeStyle.IsInit() )
			sXml += L"<w:clickAndTypeStyle " + m_oClickAndTypeStyle->ToString() + L"/>";

		if ( m_oDefaultTableStyle.IsInit() )
			sXml += L"<w:defaultTableStyle " + m_oDefaultTableStyle->ToString() + L"/>";

		if ( m_oEvenAndOddHeaders.IsInit() )
			sXml += L"<w:evenAndOddHeaders " + m_oEvenAndOddHeaders->ToString() + L"/>";

		if ( m_oBookFoldRevPrinting.IsInit() )
			sXml += L"<w:bookFoldRevPrinting " + m_oBookFoldRevPrinting->ToString() + L"/>";

		if ( m_oBookFoldPrinting.IsInit() )
			sXml += L"<w:bookFoldPrinting " + m_oBookFoldPrinting->ToString() + L"/>";

		if ( m_oBookFoldPrintingSheets.IsInit() )
			sXml += L"<w:bookFoldPrintingSheets " + m_oBookFoldPrintingSheets->ToString() + L"/>";

		if ( m_oDrawingGridHorizontalSpacing.IsInit() )
			sXml += L"<w:drawingGridHorizontalSpacing " + m_oDrawingGridHorizontalSpacing->ToString() + L"/>";

		if ( m_oDrawingGridVerticalSpacing.IsInit() )
			sXml += L"<w:drawingGridVerticalSpacing " + m_oDrawingGridVerticalSpacing->ToString() + L"/>";

		if ( m_oDisplayHorizontalDrawingGridEvery.IsInit() )
			sXml += L"<w:displayHorizontalDrawingGridEvery " + m_oDisplayHorizontalDrawingGridEvery->ToString() + L"/>";

		if ( m_oDisplayVerticalDrawingGridEvery.IsInit() )
			sXml += L"<w:displayVerticalDrawingGridEvery " + m_oDisplayVerticalDrawingGridEvery->ToString() + L"/>";

		if ( m_oDoNotUseMarginsForDrawingGridOrigin.IsInit() )
			sXml += L"<w:doNotUseMarginsForDrawingGridOrigin " + m_oDoNotUseMarginsForDrawingGridOrigin->ToString() + L"/>";

		if ( m_oDrawingGridHorizontalOrigin.IsInit() )
			sXml += L"<w:drawingGridHorizontalOrigin " + m_oDrawingGridHorizontalOrigin->ToString() + L"/>";

		if ( m_oDrawingGridVerticalOrigin.IsInit() )
			sXml += L"<w:drawingGridVerticalOrigin " + m_oDrawingGridVerticalOrigin->ToString() + L"/>";

		if ( m_oDoNotShadeFormData.IsInit() )
			sXml += L"<w:doNotShadeFormData " + m_oDoNotShadeFormData->ToString() + L"/>";

		if ( m_oNoPunctuationKerning.IsInit() )
			sXml += L"<w:noPunctuationKerning " + m_oNoPunctuationKerning->ToString() + L"/>";

		if ( m_oCharacterSpacingControl.IsInit() )
			sXml += m_oCharacterSpacingControl->toXML();

		if ( m_oPrintTwoOnOne.IsInit() )
			sXml += L"<w:printTwoOnOne " + m_oPrintTwoOnOne->ToString() + L"/>";

		if ( m_oStrictFirstAndLastChars.IsInit() )
			sXml += L"<w:strictFirstAndLastChars " + m_oStrictFirstAndLastChars->ToString() + L"/>";

		if ( m_oNoLineBreaksAfter.IsInit() )
			sXml += m_oNoLineBreaksAfter->toXML();

		if ( m_oNoLineBreaksBefore.IsInit() )
			sXml += m_oNoLineBreaksBefore->toXML();

		if ( m_oSavePreviewPicture.IsInit() )
			sXml += L"<w:savePreviewPicture " + m_oSavePreviewPicture->ToString() + L"/>";

		if ( m_oDoNotValidateAgainstSchema.IsInit() )
			sXml += L"<w:doNotValidateAgainstSchema " + m_oDoNotValidateAgainstSchema->ToString() + L"/>";

		if ( m_oSaveInvalidXml.IsInit() )
			sXml += L"<w:saveInvalidXml " + m_oSaveInvalidXml->ToString() + L"/>";

		if ( m_oIgnoreMixedContent.IsInit() )
			sXml += L"<w:ignoreMixedContent " + m_oIgnoreMixedContent->ToString() + L"/>";

		if ( m_oAlwaysShowPlaceholderText.IsInit() )
			sXml += L"<w:alwaysShowPlaceholderText " + m_oAlwaysShowPlaceholderText->ToString() + L"/>";

		if ( m_oDoNotDemarcateInvalidXml.IsInit() )
			sXml += L"<w:doNotDemarcateInvalidXml " + m_oDoNotDemarcateInvalidXml->ToString() + L"/>";

		if ( m_oSaveXmlDataOnly.IsInit() )
			sXml += L"<w:saveXmlDataOnly " + m_oSaveXmlDataOnly->ToString() + L"/>";

		if ( m_oUseXSLTWhenSaving.IsInit() )
			sXml += L"<w:useXSLTWhenSaving " + m_oUseXSLTWhenSaving->ToString() + L"/>";

		if ( m_oSaveThroughtXslt.IsInit() )
			sXml += m_oSaveThroughtXslt->toXML();

		if ( m_oShowXMLTags.IsInit() )
			sXml += L"<w:showXMLTags " + m_oShowXMLTags->ToString() + L"/>";

		if ( m_oAlwaysMergeEmptyNamespace.IsInit() )
			sXml += L"<w:alwaysMergeEmptyNamespace " + m_oAlwaysMergeEmptyNamespace->ToString() + L"/>";

		if ( m_oUpdateFields.IsInit() )
			sXml += L"<w:updateFields " + m_oUpdateFields->ToString() + L"/>";

		if ( m_oHdrShapeDefaults.IsInit() )
			sXml += m_oHdrShapeDefaults->toXML();

		if ( m_oFootnotePr.IsInit() )
			sXml += m_oFootnotePr->toXML();

		if ( m_oEndnotePr.IsInit() )
			sXml += m_oEndnotePr->toXML();

		if ( m_oCompat.IsInit() )
			sXml += m_oCompat->toXML();

		if ( m_oDocVars.IsInit() )
			sXml += m_oDocVars->toXML();

		if ( m_oRsids.IsInit() )
			sXml += m_oRsids->toXML();

		if ( m_oMathPr.IsInit() )
			sXml += m_oMathPr->toXML();

		if ( m_oAttachedSchema.IsInit() )
			sXml += L"<w:attachedSchema " + m_oAttachedSchema->ToString() + L"/>";

		if ( m_oThemeFontLang.IsInit() )
			sXml += L"<w:themeFontLang " + m_oThemeFontLang->ToString() + L"/>";

		if ( m_oClrSchemeMapping.IsInit() )
			sXml += m_oClrSchemeMapping->toXML();

		if ( m_oDoNotIncludeSubdocsInStats.IsInit() )
			sXml += L"<w:doNotIncludeSubdocsInStats " + m_oDoNotIncludeSubdocsInStats->ToString() + L"/>";

		if ( m_oDoNotAutoCompressPictures.IsInit() )
			sXml += L"<w:doNotAutoCompressPictures " + m_oDoNotAutoCompressPictures->ToString() + L"/>";

		if ( m_oForceUpgrade.IsInit() )
			sXml += L"<<w:forceUpgrade/>";

		if ( m_oCaptions.IsInit() )
			sXml += m_oCaptions->toXML();

		if ( m_oReadModeInkLockDown.IsInit() )
			sXml += m_oReadModeInkLockDown->toXML();

		for ( unsigned int nIndex = 0; nIndex < m_arrSmartTagType.size(); nIndex++ )
		{
			if (m_arrSmartTagType[nIndex])
				sXml += m_arrSmartTagType[nIndex]->toXML();
		}

		if ( m_oSchemaLibrary.IsInit() )
			sXml += m_oSchemaLibrary->toXML();

		if ( m_oShapeDefaults.IsInit() )
			sXml += m_oShapeDefaults->toXML();

		if ( m_oDoNotEmbedSmartTags.IsInit() )
			sXml += L"<w:doNotEmbedSmartTags " + m_oDoNotEmbedSmartTags->ToString() + L"/>";

		if ( m_oDecimalSymbol.IsInit() )
			sXml += L"<w:decimalSymbol " + m_oDecimalSymbol->ToString() + L"/>";

		if ( m_oListSeparator.IsInit() )
			sXml += L"<w:listSeparator " + m_oListSeparator->ToString() + L"/>";

		sXml += L"</w:settings>";
		return sXml;
	}
	
	void CSettingsCustom::fromXML(const std::wstring& sXml)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromString(sXml) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w:settings" == sName && !oReader.IsEmptyNode() )
		{
			int nStylesDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
			{
				sName = oReader.GetName();

				if ( L"w:SdtGlobalColor" == sName )
					m_oSdtGlobalColor = oReader;
				else if ( L"w:SdtGlobalShowHighlight" == sName )
					m_oSdtGlobalShowHighlight = oReader;
				else if ( L"w:SpecialFormsHighlight" == sName )
					m_oSpecialFormsHighlight = oReader;
			}
		}
	}
	std::wstring CSettingsCustom::toXML()
	{
		std::wstring sXml;
		sXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">";
		if(m_oSdtGlobalColor.IsInit())
		{
			sXml += L"<w:SdtGlobalColor ";
			sXml += m_oSdtGlobalColor->ToString();
			sXml += L"/>";
		}
		if(m_oSdtGlobalShowHighlight.IsInit())
		{
			sXml += L"<w:SdtGlobalShowHighlight ";
			sXml += m_oSdtGlobalShowHighlight->ToString();
			sXml += L"/>";
		}
		if(m_oSpecialFormsHighlight.IsInit())
		{
			sXml += L"<w:SpecialFormsHighlight ";
			sXml += m_oSpecialFormsHighlight->ToString();
			sXml += L"/>";
		}
		sXml += L"</w:settings>";
		return sXml;
	}

} // namespace OOX

