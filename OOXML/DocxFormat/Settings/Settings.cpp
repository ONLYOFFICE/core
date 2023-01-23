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
#include "../DocxFlat.h"
#include "Settings.h"
#include "../Endnote.h"
#include "../Footnote.h"
#include "../Math/oMathContent.h"

namespace OOX
{
namespace Settings
{
	//--------------------------------------------------------------------------------
	// CWritingStyle 17.15.1.2 (Part 1)
	//--------------------------------------------------------------------------------
	CWritingStyle::CWritingStyle()
	{
	}
	CWritingStyle::~CWritingStyle()
	{
	}
	EElementType CWritingStyle::getType() const
	{
		return OOX::et_w_activeWritingStyle;
	}
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

	//--------------------------------------------------------------------------------
	// CAutoCaption 17.15.1.7 (Part 1)
	//--------------------------------------------------------------------------------
	CAutoCaption::CAutoCaption()
	{
	}
	CAutoCaption::~CAutoCaption()
	{
	}
	EElementType CAutoCaption::getType() const
	{
		return OOX::et_w_autoCaption;
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

	//--------------------------------------------------------------------------------
	// CAutoCaptions 17.15.1.8 (Part 1)
	//--------------------------------------------------------------------------------
	CAutoCaptions::CAutoCaptions()
	{
	}
	CAutoCaptions::~CAutoCaptions()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrAutoCaption.size(); nIndex++ )
		{
			if (m_arrAutoCaption[nIndex] ) delete m_arrAutoCaption[nIndex];
			m_arrAutoCaption[nIndex] = NULL;
		}
		m_arrAutoCaption.clear();
	}
	EElementType CAutoCaptions::getType() const
	{
		return OOX::et_w_autoCaptions;
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

	//--------------------------------------------------------------------------------
	// CCaption 17.15.1.16 (Part 1)
	//--------------------------------------------------------------------------------
	CCaption::CCaption()
	{
	}
	CCaption::~CCaption()
	{
	}
	EElementType CCaption::getType() const
	{
		return OOX::et_w_caption;
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
	
	//--------------------------------------------------------------------------------
	// CCaptions 17.15.1.17 (Part 1)
	//--------------------------------------------------------------------------------
	CCaptions::CCaptions()
	{
	}
	CCaptions::~CCaptions()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrCaption.size(); nIndex++ )
		{
			if ( m_arrCaption[nIndex] ) delete m_arrCaption[nIndex];
			m_arrCaption[nIndex] = NULL;
		}
		m_arrCaption.clear();
	}
	EElementType CCaptions::getType() const
	{
		return OOX::et_w_captions;
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

	//--------------------------------------------------------------------------------
	// CCharacterSpacing 17.15.1.18 (Part 1)
	//--------------------------------------------------------------------------------
	CCharacterSpacing::CCharacterSpacing()
	{
	}
	CCharacterSpacing::~CCharacterSpacing()
	{
	}
	EElementType CCharacterSpacing::getType() const
	{
		return OOX::et_w_characterSpacingControl;
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

	//--------------------------------------------------------------------------------
	// CCompatSetting 17.15.3.4 (Part 1)
	//--------------------------------------------------------------------------------
	CCompatSetting::CCompatSetting()
	{
	}
	CCompatSetting::~CCompatSetting()
	{
	}
	EElementType CCompatSetting::getType() const
	{
		return OOX::et_w_compatSetting;
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
	
	//--------------------------------------------------------------------------------
	// CCompat 17.15.1.21 (Part 1)
	//--------------------------------------------------------------------------------
	CCompat::CCompat()
	{
	}
	CCompat::~CCompat()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrCompatSettings.size(); nIndex++ )
		{
			if ( m_arrCompatSettings[nIndex] ) delete m_arrCompatSettings[nIndex];
			m_arrCompatSettings[nIndex] = NULL;
		}
		m_arrCompatSettings.clear();
	}
	EElementType CCompat::getType() const
	{
		return OOX::et_w_compat;
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

	//--------------------------------------------------------------------------------
	// CDocProtect 17.15.1.29 (Part 1)
	//--------------------------------------------------------------------------------
	CDocProtect::CDocProtect()
	{
	}
	CDocProtect::~CDocProtect()
	{
	}
	EElementType CDocProtect::getType() const
	{
		return OOX::et_w_documentProtection;
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

		if (m_oCryptProviderType.IsInit() || m_oCryptAlgorithmSid.IsInit())
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

	//--------------------------------------------------------------------------------
	// CDocType 17.15.1.30 (Part 1)
	//--------------------------------------------------------------------------------
	CDocType::CDocType()
	{
	}
	CDocType::~CDocType()
	{
	}
	void CDocType::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CDocType::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CDocType::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CDocType::toXML() const
	{
		std::wstring sResult = L"<w:documentType w:val=\"" + m_oVal.ToString() + L"\"/>";
		return sResult;
	}
	EElementType CDocType::getType() const
	{
		return OOX::et_w_documentType;
	}
	void CDocType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

	//--------------------------------------------------------------------------------
	// CDocVar 17.15.1.31 (Part 1)
	//--------------------------------------------------------------------------------
	CDocVar::CDocVar()
	{
	}
	CDocVar::~CDocVar()
	{
	}
	void CDocVar::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CDocVar::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CDocVar::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CDocVar::toXML() const
	{
		std::wstring sResult = L"<w:docVar w:name=\"" + m_sName + L"\" w:val=\"" + m_sVal + L"\"/>";
		return sResult;
	}
	EElementType CDocVar::getType() const
	{
		return OOX::et_w_docVar;
	}
	void CDocVar::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'n':
						if      ( L"w:name" == wsName ) m_sName = oReader.GetText();
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

	//--------------------------------------------------------------------------------
	// CDocVars 17.15.1.32 (Part 1)
	//--------------------------------------------------------------------------------
	CDocVars::CDocVars()
	{
	}
	CDocVars::~CDocVars()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrDocVar.size(); nIndex++ )
		{
			if ( m_arrDocVar[nIndex] ) delete m_arrDocVar[nIndex];
			m_arrDocVar[nIndex] = NULL;
		}
		m_arrDocVar.clear();
	}
	void CDocVars::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CDocVars::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CDocVars::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:docVar" == sName )
			{
				OOX::Settings::CDocVar *oDV = new OOX::Settings::CDocVar(oReader);
				if (oDV) m_arrDocVar.push_back( oDV );
			}
		}
	}
	std::wstring CDocVars::toXML() const
	{
		std::wstring sResult = L"<w:docVars>";

		for (unsigned int nIndex = 0; nIndex < m_arrDocVar.size(); nIndex++ )
		{
			if (m_arrDocVar[nIndex])
				sResult += m_arrDocVar[nIndex]->toXML();
		}

		sResult += L"</w:docVars>";

		return sResult;
	}
	EElementType CDocVars::getType() const
		{
			return OOX::et_w_docVars;
		}

	//--------------------------------------------------------------------------------
	// CKinsoku 17.15.1.58 (Part 1)
	//--------------------------------------------------------------------------------
	CKinsoku::CKinsoku()
	{
	}
	CKinsoku::~CKinsoku()
	{
	}
	void CKinsoku::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CKinsoku::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CKinsoku::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		m_eType = et_Unknown;

		std::wstring sName = oReader.GetName();
		if ( L"w:noLineBreaksAfter" == sName )
			m_eType = et_w_noLineBreaksAfter;
		else if ( L"w:noLineBreaksBefore" == sName )
			m_eType = et_w_noLineBreaksBefore;
		else
			return;

		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CKinsoku::toXML() const
	{
		std::wstring sResult;

		if ( et_w_noLineBreaksAfter == m_eType )
			sResult	= L"<w:noLineBreaksAfter w:lang=\"" + m_sLang + L"\" w:val=\"" + m_sVal + L"\"/>";
		else if ( et_w_noLineBreaksBefore == m_eType )
			sResult	= L"<w:noLineBreaksBefore w:lang=\"" + m_sLang + L"\" w:val=\"" + m_sVal + L"\"/>";

		return sResult;
	}
	EElementType CKinsoku::getType() const
	{
		return m_eType;
	}
	void CKinsoku::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'l':
						if      ( L"w:lang" == wsName ) m_sLang = oReader.GetText();
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

	//--------------------------------------------------------------------------------
	// CProof 17.15.1.65 (Part 1)
	//--------------------------------------------------------------------------------
	CProof::CProof()
	{
	}
	CProof::~CProof()
	{
	}
	void CProof::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CProof::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CProof::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CProof::toXML() const
	{
		std::wstring sResult = L"<w:proofState ";

		ComplexTypes_WriteAttribute ( L"w:spelling=\"", m_oSpelling );
		ComplexTypes_WriteAttribute ( L"w:grammar=\"",  m_oGrammar );

		sResult += L"/>";
		return sResult;
	}
	EElementType CProof::getType() const
	{
		return OOX::et_w_proofState;
	}
	void CProof::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'g':
						if      ( L"w:grammar"  == wsName ) m_oGrammar  = oReader.GetText();
						break;

					case 's':
						if      ( L"w:spelling" == wsName ) m_oSpelling = oReader.GetText();
						break;
					}

				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}

	//--------------------------------------------------------------------------------
	// CReadingModeInkLockDown 17.15.1.66 (Part 1)
	//--------------------------------------------------------------------------------
	CReadingModeInkLockDown::CReadingModeInkLockDown()
	{
	}
	CReadingModeInkLockDown::~CReadingModeInkLockDown()
	{
	}
	void CReadingModeInkLockDown::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CReadingModeInkLockDown::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CReadingModeInkLockDown::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	EElementType CReadingModeInkLockDown::getType() const
	{
		return OOX::et_w_readModeInkLockDown;
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

	//--------------------------------------------------------------------------------
	// CTrackChangesView 17.15.1.69 (Part 1)
	//--------------------------------------------------------------------------------
	CTrackChangesView::CTrackChangesView()
	{
	}
	CTrackChangesView::~CTrackChangesView()
	{
	}
	void CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CTrackChangesView::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	EElementType CTrackChangesView::getType() const
	{
		return OOX::et_w_revisionView;
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

	//--------------------------------------------------------------------------------
	// CDocRsids 17.15.1.72 (Part 1)
	//--------------------------------------------------------------------------------
	CDocRsids::CDocRsids()
	{
	}
	CDocRsids::~CDocRsids()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrRsid.size(); nIndex++ )
		{
			if ( m_arrRsid[nIndex] ) delete m_arrRsid[nIndex];
			m_arrRsid[nIndex] = NULL;
		}
		m_arrRsid.clear();
	}
	void CDocRsids::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CDocRsids::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CDocRsids::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:rsid" == sName )
			{
				ComplexTypes::Word::CLongHexNumber *oRsid = new ComplexTypes::Word::CLongHexNumber(oReader);
				if (oRsid) m_arrRsid.push_back( oRsid );
			}
			else if ( L"w:rsidRoot" == sName )
				m_oRsidRoot = oReader;
		}
	}
	std::wstring CDocRsids::toXML() const
	{
		std::wstring sResult = L"<w:rsids>";

		if ( m_oRsidRoot.IsInit() )
		{
			sResult += L"<w:rsidRoot ";
			sResult += m_oRsidRoot->ToString();
			sResult += L"/>";
		}

		for ( unsigned int nIndex = 0; nIndex < m_arrRsid.size(); nIndex++ )
		{
			sResult += L"<w:rsid ";
			if (m_arrRsid[nIndex])
				sResult += m_arrRsid[nIndex]->ToString();
			sResult += L"/>";
		}

		sResult += L"</w:rsids>";

		return sResult;
	}
	EElementType CDocRsids::getType() const
		{
			return OOX::et_w_rsids;
		}

	//--------------------------------------------------------------------------------
	// CSaveThroughXslt 17.15.1.76 (Part 1)
	//--------------------------------------------------------------------------------
	CSaveThroughXslt::CSaveThroughXslt()
	{
	}
	CSaveThroughXslt::~CSaveThroughXslt()
	{
	}
	void CSaveThroughXslt::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CSaveThroughXslt::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CSaveThroughXslt::toXML() const
	{
		std::wstring sResult = L"<w:saveThroughXslt ";

		ComplexTypes_WriteAttribute ( L"r:id=\"", m_rId );
		sResult += m_sSolutionID.ToAttribute(L"w:solutionID");

		sResult += L"/>";

		return sResult;
	}
	EElementType CSaveThroughXslt::getType() const
	{
		return OOX::et_w_saveThroughXslt;
	}
	void CSaveThroughXslt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if		(oReader, L"w:solutionID",	m_sSolutionID)
				WritingElement_ReadAttributes_Read_else_if	(oReader, L"r:id",			m_rId)
				WritingElement_ReadAttributes_Read_else_if	(oReader, L"relationships:id", m_rId)
			WritingElement_ReadAttributes_End(oReader)
		}

	//--------------------------------------------------------------------------------
	// CSmartTagType 17.15.1.81 (Part 1)
	//--------------------------------------------------------------------------------
	CSmartTagType::CSmartTagType()
	{
	}
	CSmartTagType::~CSmartTagType()
	{
	}
	void CSmartTagType::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CSmartTagType::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CSmartTagType::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CSmartTagType::toXML() const
	{
		std::wstring sResult = L"<w:smartTagType ";

		sResult += m_sNameSpaceUri.ToAttribute( L"w:namespaceuri");
		sResult += m_sName.ToAttribute( L"w:name");
		sResult += m_sUrl.ToAttribute( L"w:url");

		sResult += L"/>";

		return sResult;
	}
	EElementType CSmartTagType::getType() const
	{
		return OOX::et_w_smartTagType;
	}
	void CSmartTagType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				if      ( L"w:name"         == wsName ) m_sName         = oReader.GetText();
				else if ( L"w:namespaceuri" == wsName ) m_sNameSpaceUri = oReader.GetText();
				else if ( L"w:url"          == wsName ) m_sUrl          = oReader.GetText();

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}

	//--------------------------------------------------------------------------------
	// CStylePaneFilter 17.15.1.85 (Part 1)
	//--------------------------------------------------------------------------------
	CStylePaneFilter::CStylePaneFilter()
	{
	}
	CStylePaneFilter::~CStylePaneFilter()
	{
	}
	void CStylePaneFilter::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CStylePaneFilter::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CStylePaneFilter::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	EElementType CStylePaneFilter::getType() const
	{
		return OOX::et_w_stylePaneFormatFilter;
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

	//--------------------------------------------------------------------------------
	// CWriteProtection 17.15.1.93 (Part 1)
	//--------------------------------------------------------------------------------
	CWriteProtection::CWriteProtection()
	{
	}
	CWriteProtection::~CWriteProtection()
	{
	}
	void CWriteProtection::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CWriteProtection::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CWriteProtection::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	EElementType CWriteProtection::getType() const
	{
		return OOX::et_w_writeProtection;
	}
	std::wstring CWriteProtection::toXML() const
	{
        std::wstring sResult = L"<w:writeProtection ";

		sResult += m_oRecommended.ToAttribute( L"w:recommended");

		if (m_oCryptProviderType.IsInit() || m_oCryptAlgorithmSid.IsInit())
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

	//--------------------------------------------------------------------------------
	// CZoom 17.15.1.94 (Part 1)
	//--------------------------------------------------------------------------------
	CZoom::CZoom()
	{
	}
	CZoom::~CZoom()
	{
	}
	void CZoom::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CZoom::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CZoom::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	std::wstring CZoom::toXML() const
	{
		std::wstring sResult = L"<w:zoom ";

		ComplexTypes_WriteAttribute ( L"w:val=\"", m_oVal );

		sResult += L"w:percent=\"" + m_oPercent.ToString() + L"\"/>";

		return sResult;
	}
	EElementType CZoom::getType() const
	{
		return OOX::et_w_zoom;
	}
	void CZoom::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'p':
						if      ( L"w:percent" == wsName ) m_oPercent = oReader.GetText();
						break;
					case 'v':
						if      ( L"w:val"     == wsName ) m_oVal     = oReader.GetText();
						break;
					}

				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

		}

	//--------------------------------------------------------------------------------
	// CEdnDocProps 17.11.4 (Part 1)
	//--------------------------------------------------------------------------------

	CEdnDocProps::CEdnDocProps(OOX::Document *pMain) : OOX::WritingElement(pMain)
	{
	}
	CEdnDocProps::~CEdnDocProps()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
		{
			if ( m_arrEndnote[nIndex] ) delete m_arrEndnote[nIndex];
			m_arrEndnote[nIndex] = NULL;
		}
		m_arrEndnote.clear();
	}
	void CEdnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CEdnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
	}
	EElementType CEdnDocProps::getType() const
		{
			return et_w_endnotePr;
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
				CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(WritingElement::m_pMainDocument);
				if (docx_flat)
				{
					CFtnEdn *pEndnote = new CFtnEdn(WritingElement::m_pMainDocument);
					pEndnote->fromXML(oReader);

					pEndnote->m_oId.Init();
					pEndnote->m_oId->SetValue((int)docx_flat->m_pEndnotes->m_arrEndnote.size() - 1);
					
					docx_flat->m_pSettings->m_oEndnotePr->m_arrEndnote.push_back(new CFtnEdnSepRef());
					docx_flat->m_pSettings->m_oEndnotePr->m_arrEndnote.back()->m_oId = pEndnote->m_oId;
					docx_flat->m_pSettings->m_oEndnotePr->m_arrEndnote.back()->m_eType = OOX::et_w_endnote;
					
					docx_flat->m_pEndnotes->m_arrEndnote.push_back(pEndnote);
					docx_flat->m_pEndnotes->m_mapEndnote.insert(std::make_pair(pEndnote->m_oId->GetValue(), pEndnote));
				}
				else
				{
					OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
					if (oFE) m_arrEndnote.push_back(oFE);
				}
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
	
	//--------------------------------------------------------------------------------
	// CFtnDocProps 17.11.12 (Part 1)
	//--------------------------------------------------------------------------------

	CFtnDocProps::CFtnDocProps(OOX::Document *pMain) : OOX::WritingElement(pMain)
	{
	}
	CFtnDocProps::~CFtnDocProps()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
		{
			if ( m_arrFootnote[nIndex] ) delete m_arrFootnote[nIndex];
			m_arrFootnote[nIndex] = NULL;
		}
		m_arrFootnote.clear();
	}
	void CFtnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CFtnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
	}
	EElementType CFtnDocProps::getType() const
		{
			return et_w_footnotePr;
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
				CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(WritingElement::m_pMainDocument);
				if (docx_flat)
				{
					CFtnEdn *pFootnote = new CFtnEdn(WritingElement::m_pMainDocument);
					pFootnote->fromXML(oReader);

					pFootnote->m_oId.Init();
					pFootnote->m_oId->SetValue((int)docx_flat->m_pFootnotes->m_arrFootnote.size() - 1);

					docx_flat->m_pSettings->m_oFootnotePr->m_arrFootnote.push_back(new CFtnEdnSepRef());
					docx_flat->m_pSettings->m_oFootnotePr->m_arrFootnote.back()->m_oId = pFootnote->m_oId;
					docx_flat->m_pSettings->m_oFootnotePr->m_arrFootnote.back()->m_eType = OOX::et_w_footnote;

					docx_flat->m_pFootnotes->m_arrFootnote.push_back(pFootnote);
					docx_flat->m_pFootnotes->m_mapFootnote.insert(std::make_pair(pFootnote->m_oId->GetValue(), pFootnote));
				}
				else
				{
					OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
					if (oFE) m_arrFootnote.push_back(oFE);
				}
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

	//--------------------------------------------------------------------------------
	// CSchema 23.2.1 (Part 1)
	//--------------------------------------------------------------------------------
	CSchema::CSchema()
	{
	}
	CSchema::~CSchema()
	{
	}
	void CSchema::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CSchema::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CSchema::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
	EElementType CSchema::getType() const
		{
			return OOX::et_sl_schema;
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

	//--------------------------------------------------------------------------------
	// CSchemaLibrary 23.2.2 (Part 1)
	//--------------------------------------------------------------------------------
	CSchemaLibrary::CSchemaLibrary()
	{
	}
	CSchemaLibrary::~CSchemaLibrary()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrSchema.size(); nIndex++ )
		{
			if ( m_arrSchema[nIndex] ) delete m_arrSchema[nIndex];
			m_arrSchema[nIndex] = NULL;
		}
		m_arrSchema.clear();
	}
	void CSchemaLibrary::fromXML(XmlUtils::CXmlNode& oNode)
	{
		// TO DO: Реализовать CSchemaLibrary::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CSchemaLibrary::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"sl:schema" == sName )
			{
				OOX::Settings::CSchema *oSchema = new OOX::Settings::CSchema(oReader);
				if (oSchema) m_arrSchema.push_back( oSchema );
			}
		}
	}
	std::wstring CSchemaLibrary::toXML() const
	{
		std::wstring sResult = L"<sl:schemaLibrary>";

		for ( unsigned int nIndex = 0; nIndex < m_arrSchema.size(); nIndex++ )
		{
			if (m_arrSchema[nIndex])
				sResult += m_arrSchema[nIndex]->toXML();
		}

		sResult += L"</sl:schemaLibrary>";

		return sResult;
	}
	EElementType CSchemaLibrary::getType() const
		{
			return OOX::et_sl_schemaLibrary;
		}

	//--------------------------------------------------------------------------------
	// CShapeDefaults 9.7.2.1;9.7.2.2 (Part 4)
	//--------------------------------------------------------------------------------
	CShapeDefaults::CShapeDefaults(EElementType type)
	{
		m_eType = type;
	}
	CShapeDefaults::~CShapeDefaults()
	{
	}
	void CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
	{
		m_eType = et_Unknown;
		// TO DO: Реализовать CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
	}
	void CShapeDefaults::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		m_eType = et_Unknown;
		std::wstring sName = oReader.GetName();

		if ( L"w:hdrShapeDefaults" == sName )
			m_eType = et_w_hdrShapeDefaults;
		else if ( L"w:shapeDefaults" == sName )
			m_eType = et_w_shapeDefaults;
		else
			return;

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			sName = oReader.GetName();

			WritingElement* pItem = NULL;

			wchar_t wChar0 = sName[0];
			if ( 'o' == wChar0 )
			{
				wchar_t wChar2 = sName[2]; // o:_
				switch ( wChar2 )
				{
				case 'b':
					if ( L"o:bottom" == sName )
						pItem = new OOX::VmlOffice::CStrokeChild( oReader );

					break;

				case 'c':
					if ( L"o:callout" == sName )
						pItem = new OOX::VmlOffice::CCallout( oReader );
					else if ( L"o:clippath" == sName )
						pItem = new OOX::VmlOffice::CClipPath( oReader );
					else if ( L"o:column" == sName )
						pItem = new OOX::VmlOffice::CStrokeChild( oReader );
					else if ( L"o:complex" == sName )
						pItem = new OOX::VmlOffice::CComplex( oReader );

					break;

				case 'd':
					if ( L"o:diagram" == sName )
						pItem = new OOX::VmlOffice::CDiagram( oReader );

					break;

				case 'e':
					if ( L"o:equationxml" == sName )
						pItem = new OOX::VmlOffice::CEquationXml( oReader );
					else if ( L"o:extrusion" == sName )
						pItem = new OOX::VmlOffice::CExtrusion( oReader );

					break;

				case 'f':
					if ( L"o:fill" == sName )
						pItem = new OOX::VmlOffice::CFill( oReader );

					break;

				case 'i':
					if ( L"o:ink" == sName )
						pItem = new OOX::VmlOffice::CInk( oReader );

					break;

				case 'l':
					if ( L"o:left" == sName )
						pItem = new OOX::VmlOffice::CStrokeChild( oReader );
					else if ( L"o:lock" == sName )
						pItem = new OOX::VmlOffice::CLock( oReader );

					break;

				case 'O':
					if ( L"o:OLEObject" == sName )
						pItem = new OOX::VmlOffice::COLEObject( oReader );

					break;

				case 'r':
					if ( L"o:right" == sName )
						pItem = new OOX::VmlOffice::CStrokeChild( oReader );

					break;

				case 's':
					if ( L"o:shapedefaults" == sName )
						pItem = new OOX::VmlOffice::CShapeDefaults( oReader );
					else if ( L"o:shapelayout" == sName )
						pItem = new OOX::VmlOffice::CShapeLayout( oReader );
					else if ( L"o:signatureline" == sName )
						pItem = new OOX::VmlOffice::CSignatureLine( oReader );
					else if ( L"o:skew" == sName )
						pItem = new OOX::VmlOffice::CSkew( oReader );

					break;

				case 't':
					if ( L"o:top" == sName )
						pItem = new OOX::VmlOffice::CStrokeChild( oReader );

					break;
				}
			}

			if ( pItem )
				m_arrItems.push_back( pItem );
		}
	}
	std::wstring CShapeDefaults::toXML() const
	{
		std::wstring sResult;

		if ( et_w_hdrShapeDefaults == m_eType )
			sResult = L"<w:hdrShapeDefaults>";
		else if ( et_w_shapeDefaults == m_eType )
			sResult = L"<w:shapeDefaults>";
		else
			return L"";

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (  m_arrItems[i] )
			{
				sResult += m_arrItems[i]->toXML();
			}
		}

		if ( et_w_hdrShapeDefaults == m_eType )
			sResult += L"</w:hdrShapeDefaults>";
		else if ( et_w_shapeDefaults == m_eType )
			sResult += L"</w:shapeDefaults>";

		return sResult;
	}
	EElementType CShapeDefaults::getType() const
		{
			return m_eType;
		}

} // namespace Settings

	//--------------------------------------------------------------------------------
	// CSettings 17.11.15.1.78
	//--------------------------------------------------------------------------------
	CSettings::CSettings(OOX::Document *pMain) : OOX::File(pMain), OOX::WritingElement(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.settings = this;
			else						docx->m_oMain.settings = this;
		}
	}
	CSettings::CSettings(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain), OOX::WritingElement(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.settings = this;
			else						docx->m_oMain.settings = this;
		}

		read( oPath );
	}
	CSettings::CSettings(XmlUtils::CXmlNode& oNode) : File(NULL), WritingElement(NULL)
	{
		fromXML( oNode );
	}
	CSettings::CSettings(XmlUtils::CXmlLiteReader& oReader) : File(NULL), WritingElement(NULL)
	{
		fromXML( oReader );
	}
	CSettings::~CSettings()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrSmartTagType.size(); nIndex++ )
		{
			if ( m_arrSmartTagType[nIndex] )delete m_arrSmartTagType[nIndex];
			m_arrSmartTagType[nIndex] = NULL;
		}

		m_arrSmartTagType.clear();
	}
	void CSettings::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		fromXML(oReader);
	}
	void CSettings::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	const OOX::FileType CSettings::type() const
	{
		return FileTypes::Setting;
	}
	const CPath CSettings::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CSettings::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
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
					else if (L"w:endnotePr" == sName)
					{
						m_oEndnotePr = new Settings::CEdnDocProps(WritingElement::m_pMainDocument);
						m_oEndnotePr->fromXML(oReader);
					}
					else if ( L"w:evenAndOddHeaders"          == sName ) m_oEvenAndOddHeaders          = oReader;
					break;
				case 'f':
					if (L"w:footnotePr" == sName)
					{
						m_oFootnotePr = new Settings::CFtnDocProps(WritingElement::m_pMainDocument);
						m_oFootnotePr->fromXML(oReader);
					}
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
		oContent.Registration( type().OverrideType(), oDirectory, oFilePath.GetFilename() );
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
	EElementType CSettings::getType() const
	{
		return et_w_settings;
	}

	//--------------------------------------------------------------------------------
	// CSettingsCustom
	//--------------------------------------------------------------------------------
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
	std::wstring CSettingsCustom::GetSchemaUrl()
	{
		return L"http://schemas.onlyoffice.com/settingsCustom";
	}
	bool CSettingsCustom::IsEmpty()
	{
		return !(m_oSdtGlobalColor.IsInit() || m_oSdtGlobalShowHighlight.IsInit() || m_oSpecialFormsHighlight.IsInit());
	}

} // namespace OOX

