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
#include "../../Base/Nullable.h"

#include "../File.h"
#include "../../Common/ComplexTypes.h"
#include "../Logic/VmlOfficeDrawing.h"
#include "../FtnEdn.h"

namespace OOX
{
	namespace Settings
	{
		//--------------------------------------------------------------------------------
		// CWritingStyle 17.15.1.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWritingStyle : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWritingStyle)
			CWritingStyle()
			{
			}
			virtual ~CWritingStyle()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CWritingStyle::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:activeWritingStyle ";

				ComplexTypes_WriteAttribute ( L"w:lang=\"",       m_oLang );
				ComplexTypes_WriteAttribute2( L"w:vendorID=\"",   m_sVendorID );
				ComplexTypes_WriteAttribute2( L"w:dllVersion=\"", m_sDllVersion );
				ComplexTypes_WriteAttribute ( L"w:nlCheck=\"",    m_oNlCheck );
				ComplexTypes_WriteAttribute ( L"w:checkStyle=\"", m_oCheckSyle );
				ComplexTypes_WriteAttribute2( L"w:appName=\"",    m_sAppName );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_activeWritingStyle;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

            nullable<std::wstring>			m_sAppName;
			nullable<SimpleTypes::COnOff<>> m_oCheckSyle;
            nullable<std::wstring>			m_sDllVersion;
			nullable<SimpleTypes::CLang>    m_oLang;
			nullable<SimpleTypes::COnOff<>> m_oNlCheck;
            nullable<std::wstring>			m_sVendorID;
		};
		//--------------------------------------------------------------------------------
		// CAutoCaption 17.15.1.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAutoCaption : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAutoCaption)
			CAutoCaption()
			{
			}
			virtual ~CAutoCaption()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CAutoCaption::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:autoCaption ";

				ComplexTypes_WriteAttribute2( L"w:name=\"",    m_sName );
				ComplexTypes_WriteAttribute2( L"w:caption=\"", m_sCaption );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_autoCaption;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
            nullable<std::wstring> m_sCaption;
            nullable<std::wstring> m_sName;
		};
		//--------------------------------------------------------------------------------
		// CAutoCaptions 17.15.1.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAutoCaptions : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAutoCaptions)
			CAutoCaptions()
			{
			}
			virtual ~CAutoCaptions()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrAutoCaption.size(); nIndex++ )
				{
					if (m_arrAutoCaption[nIndex] ) delete m_arrAutoCaption[nIndex];
					m_arrAutoCaption[nIndex] = NULL;
				}
				m_arrAutoCaption.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CAutoCaptions::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring      toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_autoCaptions;
			}


		public:

			// Childs
			std::vector<OOX::Settings::CAutoCaption*> m_arrAutoCaption;
		};
		//--------------------------------------------------------------------------------
		// CCaption 17.15.1.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCaption : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCaption)
			CCaption()
			{
			}
			virtual ~CCaption()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCaption::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:caption w:name=\"" + m_sName + L"\" ";

				ComplexTypes_WriteAttribute( L"w:pos=\"",     m_oPos );
				ComplexTypes_WriteAttribute( L"w:chapNum=\"", m_oChapNum );
				ComplexTypes_WriteAttribute( L"w:heading=\"", m_oHeading );
				ComplexTypes_WriteAttribute( L"w:noLabel=\"", m_oNoLabel );
				ComplexTypes_WriteAttribute( L"w:numFmt=\"",  m_oNumFmt );
				ComplexTypes_WriteAttribute( L"w:sep=\"",     m_oSep );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_caption;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
			nullable<SimpleTypes::COnOff<>>         m_oChapNum;
			nullable<SimpleTypes::CDecimalNumber<>> m_oHeading;
            std::wstring                                 m_sName;
			nullable<SimpleTypes::COnOff<>>         m_oNoLabel;
			nullable<SimpleTypes::CNumberFormat<>>  m_oNumFmt;
			nullable<SimpleTypes::CCaptionPos<>>    m_oPos;
			nullable<SimpleTypes::CChapterSep<>>    m_oSep;
		};
		//--------------------------------------------------------------------------------
		// CCaptions 17.15.1.17 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCaptions : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCaptions)
			CCaptions()
			{
			}
			virtual ~CCaptions()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrCaption.size(); nIndex++ )
				{
					if ( m_arrCaption[nIndex] ) delete m_arrCaption[nIndex];
					m_arrCaption[nIndex] = NULL;
				}
				m_arrCaption.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCaptions::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring      toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_captions;
			}


		public:

			// Childs
			std::vector<OOX::Settings::CCaption*>  m_arrCaption;
			nullable<OOX::Settings::CAutoCaptions> m_oAutoCaptions;

		};
		//--------------------------------------------------------------------------------
		// CCharacterSpacing 17.15.1.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCharacterSpacing : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCharacterSpacing)
			CCharacterSpacing()
			{
			}
			virtual ~CCharacterSpacing()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCharacterSpacing::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:characterSpacingControl w:val=\"" + m_oVal.ToString() + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_characterSpacingControl;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
			SimpleTypes::CCharacterSpacing<> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// CCompatSetting 17.15.3.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCompatSetting : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCompatSetting)
			CCompatSetting()
			{
			}
			virtual ~CCompatSetting()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCompatSetting::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:compatSetting ";

				ComplexTypes_WriteAttribute2( L"w:name=\"", m_sName );
				ComplexTypes_WriteAttribute2( L"w:uri=\"",  m_sUri );
				ComplexTypes_WriteAttribute2( L"w:val=\"",  m_sVal );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_compatSetting;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
            nullable<std::wstring> m_sName;
            nullable<std::wstring> m_sUri;
            nullable<std::wstring> m_sVal;
		};
		//--------------------------------------------------------------------------------
		// CCompat 17.15.1.21 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCompat : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCompat)
			CCompat()
			{
			}
			virtual ~CCompat()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrCompatSettings.size(); nIndex++ )
				{
					if ( m_arrCompatSettings[nIndex] ) delete m_arrCompatSettings[nIndex];
					m_arrCompatSettings[nIndex] = NULL;
				}
				m_arrCompatSettings.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCompat::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring      toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_compat;
			}


		public:

			// TO DO: Добавить аттрибуты из 9.7.3 Part4

			// Childs
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseSingleBorderforContiguousCells;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oWpJustification;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoTabHangInd;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoLeading;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSpaceForUL;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoColumnBalance;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oBalanceSingleByteDoubleByteWidth;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoExtraLineSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotLeaveBackslashAlone;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUlTrailSpace;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotExpandShiftReturn;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSpacingInWholePoints;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oLineWrapLikeWord6;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintBodyTextBeforeHeader;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintColBlack;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oWpSpaceWidth;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oShowBreaksInFrames;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSubFontBySize;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSuppressBottomSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSuppressTopSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSuppressSpacingAtTopOfPage;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSuppressTopSpacingWP;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSuppressSpBfAfterPgBrk;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSwapBordersFacingPages;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oConvMailMergeEsc;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oTruncateFontHeightsLikeWP6;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oMwSmallCaps;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUsePrinterMetrics;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotSuppressParagraphBorders;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oWrapTrailSpaces;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oFootnoteLayoutLikeWW8;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oShapeLayoutLikeWW8;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAlignTablesRowByRow;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oForgetLastTabAlignment;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAdjustLineHeightInTable;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAutoSpaceLikeWord95;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoSpaceRaiseLower;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotUseHTMLParagraphAutoSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oLayoutRawTableWidth;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oLayoutTableRowsApart;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseWord97LineBreakRules;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotBreakWrappedTables;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotSnapToGridInCell;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSelectFldWithFirstOrLastChar;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oApplyBreakingRules;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotWrapTextWithPunct;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotUseEastAsianBreakRules;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseWord2002TableStyleRules;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oGrowAutofit;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseFELayout;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseNormalStyleForList;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotUseIndentAsNumberingTabStop;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseAltKinsokuLineBreakRules;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAllowSpaceOfSameStyleInTable;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotSuppressIndentation;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotAutofitConstrainedTables;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAutofitToFirstFixedWidthCell;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUnderlineTabInNumList;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDisplayHangulFixedWidth;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSplitPgBreakAndParaMark;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotVertAlignCellWithSp;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotBreakConstrainedForcedTable;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotVertAlignInTxbx;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseAnsiKerningPairs;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oCachedColBalance;

			std::vector<OOX::Settings::CCompatSetting*>                   m_arrCompatSettings;
		};
		//--------------------------------------------------------------------------------
		// CDocProtect 17.15.1.29 (Part 1) 
		//--------------------------------------------------------------------------------	
		class CDocProtect : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocProtect)
			CDocProtect()
			{
			}
			virtual ~CDocProtect()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocProtect::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_documentProtection;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

				// TO DO: Сделать парсер Base64
			}
		public:
			nullable<SimpleTypes::CCryptAlgoritmName<>> m_oAlgorithmName;
			nullable<SimpleTypes::CDocProtect<>>        m_oEdit;
			nullable_bool								m_oEnforcment;
			nullable_bool								m_oFormatting;
            nullable_string								m_sHashValue;
			nullable_string								m_sSaltValue;
			nullable_int								m_oSpinCount;
//ext
			nullable_string								m_oAlgIdExt; //long hex
			nullable_string								m_oAlgIdExtSource;
			nullable<SimpleTypes::CCryptAlgClass<>>		m_oCryptAlgorithmClass;
			nullable_int								m_oCryptAlgorithmSid;
			nullable<SimpleTypes::CCryptAlgType<>>		m_oCryptAlgorithmType;
			nullable_string								m_oCryptProvider;
			nullable<SimpleTypes::CCryptProv<>>			m_oCryptProviderType;
			nullable_string								m_oCryptProviderTypeExt; //long hex
			nullable_string								m_oCryptProviderTypeExtSource;
		};
		//--------------------------------------------------------------------------------
		// CDocType 17.15.1.30 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDocType : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocType)
			CDocType()
			{
			}
			virtual ~CDocType()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocType::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:documentType w:val=\"" + m_oVal.ToString() + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_documentType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
			SimpleTypes::CDocType<> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// CDocVar 17.15.1.31 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDocVar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocVar)
			CDocVar()
			{
			}
			virtual ~CDocVar()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocVar::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:docVar w:name=\"" + m_sName + L"\" w:val=\"" + m_sVal + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_docVar;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// Attributes
            std::wstring m_sName;
            std::wstring m_sVal;
		};
		//--------------------------------------------------------------------------------
		// CDocVars 17.15.1.32 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDocVars : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocVars)
			CDocVars()
			{
			}
			virtual ~CDocVars()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrDocVar.size(); nIndex++ )
				{
					if ( m_arrDocVar[nIndex] ) delete m_arrDocVar[nIndex];
					m_arrDocVar[nIndex] = NULL;
				}
				m_arrDocVar.clear();
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocVars::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_docVars;
			}


		public:

			// Childs
			std::vector<OOX::Settings::CDocVar*> m_arrDocVar;
		};
		//--------------------------------------------------------------------------------
		// CKinsoku 17.15.1.58 (Part 1)
		//--------------------------------------------------------------------------------	
		class CKinsoku : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CKinsoku)
			CKinsoku()
			{
			}
			virtual ~CKinsoku()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CKinsoku::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
			{
                std::wstring sResult;
				
				if ( et_w_noLineBreaksAfter == m_eType )
					sResult	= L"<w:noLineBreaksAfter w:lang=\"" + m_oLang.ToString() + L"\" w:val=\"" + m_sVal + L"\"/>";
				else if ( et_w_noLineBreaksBefore == m_eType ) 
					sResult	= L"<w:noLineBreaksBefore w:lang=\"" + m_oLang.ToString() + L"\" w:val=\"" + m_sVal + L"\"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
							if      ( L"w:lang" == wsName ) m_oLang = oReader.GetText();
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

		public:

			EElementType m_eType;

			SimpleTypes::CLang m_oLang;
            std::wstring m_sVal;
		};
		//--------------------------------------------------------------------------------
		// CProof 17.15.1.65 (Part 1)
		//--------------------------------------------------------------------------------	
		class CProof : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProof)
			CProof()
			{
			}
			virtual ~CProof()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CProof::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:proofState ";

				ComplexTypes_WriteAttribute ( L"w:spelling=\"", m_oSpelling );
				ComplexTypes_WriteAttribute ( L"w:grammar=\"",  m_oGrammar );

				sResult += L"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_proofState;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			nullable<SimpleTypes::CProof<>> m_oGrammar;
			nullable<SimpleTypes::CProof<>> m_oSpelling;
		};
		//--------------------------------------------------------------------------------
		// CReadingModeInkLockDown 17.15.1.66 (Part 1)
		//--------------------------------------------------------------------------------	
		class CReadingModeInkLockDown : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CReadingModeInkLockDown)
			CReadingModeInkLockDown()
			{
			}
			virtual ~CReadingModeInkLockDown()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CReadingModeInkLockDown::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<w:readModeInkLockDown ";

				sResult += L"w:w=\""        + m_oW.ToString() 
					  + L"\" w:h=\""        + m_oH.ToString() 
					  + L"\" w:fontSz=\""   + m_oFontSz.ToString() 
					  + L"\" w:actualPg=\"" + m_oActualPg.ToString() + L"\"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_readModeInkLockDown;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oActualPg;
			SimpleTypes::CDecimalNumberOrPercent         m_oFontSz;
			SimpleTypes::CPixelsMeasure<>                m_oH;
			SimpleTypes::CPixelsMeasure<>                m_oW;
		};
		//--------------------------------------------------------------------------------
		// CTrackChangesView 17.15.1.69 (Part 1)
		//--------------------------------------------------------------------------------	
		class CTrackChangesView : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTrackChangesView)
			CTrackChangesView()
			{
			}
			virtual ~CTrackChangesView()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:revisionView ";

				ComplexTypes_WriteAttribute ( L"w:markup=\"",         m_oMarkup );
				ComplexTypes_WriteAttribute ( L"w:comments=\"",       m_oComments );
				ComplexTypes_WriteAttribute ( L"w:insDel=\"",         m_oInsDel );
				ComplexTypes_WriteAttribute ( L"w:formatting=\"",     m_oFormatting );
				ComplexTypes_WriteAttribute ( L"w:inkAnnotations=\"", m_oInkAnnotataions );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_revisionView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
							if      ( L"w:comments"       == wsName ) m_oComments        = oReader.GetText();
							break;

						case 'f':
							if      ( L"w:formatting"     == wsName ) m_oFormatting      = oReader.GetText();
							break;

						case 'i':
							if      ( L"w:inkAnnotations" == wsName ) m_oInkAnnotataions = oReader.GetText();
							else if ( L"w:insDel"         == wsName ) m_oInsDel          = oReader.GetText();
							break;

						case 'm':
							if      ( L"w:markup"         == wsName ) m_oMarkup          = oReader.GetText();
							break;
						}

					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:
			nullable<SimpleTypes::COnOff<>> m_oComments;
			nullable<SimpleTypes::COnOff<>> m_oFormatting;
			nullable<SimpleTypes::COnOff<>> m_oInkAnnotataions;
			nullable<SimpleTypes::COnOff<>> m_oInsDel;
			nullable<SimpleTypes::COnOff<>> m_oMarkup;
		};
		//--------------------------------------------------------------------------------
		// CDocRsids 17.15.1.72 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDocRsids : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocRsids)
			CDocRsids()
			{
			}
			virtual ~CDocRsids()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrRsid.size(); nIndex++ )
				{
					if ( m_arrRsid[nIndex] ) delete m_arrRsid[nIndex];
					m_arrRsid[nIndex] = NULL;
				}
				m_arrRsid.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocRsids::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_rsids;
			}


		public:

			// Childs
			nullable<ComplexTypes::Word::CLongHexNumber>     m_oRsidRoot;
			std::vector<ComplexTypes::Word::CLongHexNumber*> m_arrRsid;
		};
		//--------------------------------------------------------------------------------
		// CSaveThroughXslt 17.15.1.76 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSaveThroughXslt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSaveThroughXslt)
			CSaveThroughXslt()
			{
			}
			virtual ~CSaveThroughXslt()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:saveThroughXslt ";

				ComplexTypes_WriteAttribute ( L"r:id=\"",         m_rId );
				ComplexTypes_WriteAttribute2( L"w:solutionID=\"", m_sSolutionID );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_saveThroughXslt;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if		(oReader, L"w:solutionID",	m_sSolutionID)
					WritingElement_ReadAttributes_Read_else_if	(oReader, L"r:id",			m_rId)
					WritingElement_ReadAttributes_Read_else_if	(oReader, L"relationships:id", m_rId)
				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::CRelationshipId> m_rId;
            nullable<std::wstring>                      m_sSolutionID;
		};
		//--------------------------------------------------------------------------------
		// CSmartTagType 17.15.1.81 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSmartTagType : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSmartTagType)
			CSmartTagType()
			{
			}
			virtual ~CSmartTagType()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSmartTagType::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:smartTagType ";

				ComplexTypes_WriteAttribute2( L"w:namespaceuri=\"", m_sNameSpaceUri );
				ComplexTypes_WriteAttribute2( L"w:name=\"",         m_sName );
				ComplexTypes_WriteAttribute2( L"w:url=\"",          m_sUrl );

				sResult += L"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_smartTagType;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

            nullable<std::wstring> m_sName;
            nullable<std::wstring> m_sNameSpaceUri;
            nullable<std::wstring> m_sUrl;
		};
		//--------------------------------------------------------------------------------
		// CStylePaneFilter 17.15.1.85 (Part 1)
		//--------------------------------------------------------------------------------	
		class CStylePaneFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStylePaneFilter)
			CStylePaneFilter()
			{
			}
			virtual ~CStylePaneFilter()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStylePaneFilter::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:stylePaneFormatFilter ";

				ComplexTypes_WriteAttribute( L"w:allStyles=\"",                    m_oAllStyles );
				ComplexTypes_WriteAttribute( L"w:alternateStyleNames=\"",          m_oAlternateStyleNames );
				ComplexTypes_WriteAttribute( L"w:clearFormatting=\"",              m_oClearFormatting );
				ComplexTypes_WriteAttribute( L"w:customStyles=\"",                 m_oCustomStyles );
				ComplexTypes_WriteAttribute( L"w:directFormattingOnNumbering=\"",  m_oDirectFormattingOnNumbering );
				ComplexTypes_WriteAttribute( L"w:directFormattingOnParagraphs=\"", m_oDirectFormattingOnParagraphs );
				ComplexTypes_WriteAttribute( L"w:directFormattingOnRuns=\"",       m_oDirectFormattingOnRuns );
				ComplexTypes_WriteAttribute( L"w:directFormattingOnTables=\"",     m_oDirectFormattingOnTables );
				ComplexTypes_WriteAttribute( L"w:headingStyles=\"",                m_oHeadingStyles );
				ComplexTypes_WriteAttribute( L"w:latentStyles=\"",                 m_oLatentStyles );
				ComplexTypes_WriteAttribute( L"w:numberingStyles=\"",              m_oNumberingStyles );
				ComplexTypes_WriteAttribute( L"w:stylesInUse=\"",                  m_oStylesInUse );
				ComplexTypes_WriteAttribute( L"w:tableStyles=\"",                  m_oTableStyles );
				ComplexTypes_WriteAttribute( L"w:top3HeadingStyles=\"",            m_oTop3HeadingStyles );
				ComplexTypes_WriteAttribute( L"w:visibleStyles=\"",                m_oVisibleStyles );

				sResult += L"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneFormatFilter;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			// TO DO: Добавить атрибуты из 9.7.2.4 Part4

			nullable<SimpleTypes::COnOff<>> m_oAllStyles;
			nullable<SimpleTypes::COnOff<>> m_oAlternateStyleNames;
			nullable<SimpleTypes::COnOff<>> m_oClearFormatting;
			nullable<SimpleTypes::COnOff<>> m_oCustomStyles;
			nullable<SimpleTypes::COnOff<>> m_oDirectFormattingOnNumbering;
			nullable<SimpleTypes::COnOff<>> m_oDirectFormattingOnParagraphs;
			nullable<SimpleTypes::COnOff<>> m_oDirectFormattingOnRuns;
			nullable<SimpleTypes::COnOff<>> m_oDirectFormattingOnTables;
			nullable<SimpleTypes::COnOff<>> m_oHeadingStyles;
			nullable<SimpleTypes::COnOff<>> m_oLatentStyles;
			nullable<SimpleTypes::COnOff<>> m_oNumberingStyles;
			nullable<SimpleTypes::COnOff<>> m_oStylesInUse;
			nullable<SimpleTypes::COnOff<>> m_oTableStyles;
			nullable<SimpleTypes::COnOff<>> m_oTop3HeadingStyles;
			nullable<SimpleTypes::COnOff<>> m_oVisibleStyles;
		};
		//--------------------------------------------------------------------------------
		// CStyleSort 17.15.1.86 (Part 1)
		//--------------------------------------------------------------------------------	
		class CStyleSort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStyleSort)
			CStyleSort()
			{
			}
			virtual ~CStyleSort()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStyleSort::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:stylePaneSortMethod w:val=\"" + m_oVal.ToString() + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneSortMethod;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( L"w:val" == wsName ) 
						m_oVal = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			SimpleTypes::CStyleSort<> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// CView 17.15.1.92 (Part 1)
		//--------------------------------------------------------------------------------	
		class CView : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CView)
			CView()
			{
			}
			virtual ~CView()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:view w:val=\"" + m_oVal.ToString() + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneSortMethod;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( L"w:val" == wsName ) 
						m_oVal = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			SimpleTypes::CView<> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// CWriteProtection 17.15.1.93 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWriteProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWriteProtection)
			CWriteProtection()
			{
			}
			virtual ~CWriteProtection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CWriteProtection::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_w_writeProtection;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

				// TO DO: Сделать парсер Base64
			}

		public:
			nullable<SimpleTypes::CCryptAlgoritmName<>> m_oAlgorithmName;
			nullable_string								m_sHashValue;
			nullable_bool								m_oRecommended;
			nullable_string								m_sSaltValue;
			nullable_int								m_oSpinCount;
//ext
			nullable_string								m_oAlgIdExt; //long hex
			nullable_string								m_oAlgIdExtSource;
			nullable<SimpleTypes::CCryptAlgClass<>>		m_oCryptAlgorithmClass;
			nullable_int								m_oCryptAlgorithmSid;
			nullable<SimpleTypes::CCryptAlgType<>>		m_oCryptAlgorithmType;
			nullable_string								m_oCryptProvider;
			nullable<SimpleTypes::CCryptProv<>>			m_oCryptProviderType;
			nullable_string								m_oCryptProviderTypeExt; //long hex
			nullable_string								m_oCryptProviderTypeExtSource;
		};
		//--------------------------------------------------------------------------------
		// CZoom 17.15.1.94 (Part 1)
		//--------------------------------------------------------------------------------	
		class CZoom : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CZoom)
			CZoom()
			{
			}
			virtual ~CZoom()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CZoom::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:zoom ";

				ComplexTypes_WriteAttribute ( L"w:val=\"", m_oVal );

				sResult += L"w:percent=\"" + m_oPercent.ToString() + L"\"/>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_zoom;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

			SimpleTypes::CDecimalNumberOrPercent m_oPercent;
			nullable<SimpleTypes::CZoom<>>       m_oVal;
		};
		//--------------------------------------------------------------------------------
		// CEdnDocProps 17.11.4 (Part 1)
		//--------------------------------------------------------------------------------
		class CEdnDocProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEdnDocProps)
			CEdnDocProps()
			{
			}
			virtual ~CEdnDocProps()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
				{
					if ( m_arrEndnote[nIndex] ) delete m_arrEndnote[nIndex];
					m_arrEndnote[nIndex] = NULL;
				}
				m_arrEndnote.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CEdnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:endnotePr>";

				for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
				{
					if (m_arrEndnote[nIndex])
						sResult += m_arrEndnote[nIndex]->toXML();
				}

				if ( m_oNumFmt.IsInit() )
				{
					sResult += L"<w:numFmt ";
					sResult += m_oNumFmt->ToString();
					sResult += L"/>";
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += L"<w:numRestart ";
					sResult += m_oNumRestart->ToString();
					sResult += L"/>";
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += L"<w:numStart ";
					sResult += m_oNumStart->ToString();
					sResult += L"/>";
				}

				if ( m_oPos.IsInit() )
				{
					sResult += L"<w:pos ";
					sResult += m_oPos->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:endnotePr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_endnotePr;
			}
		public:
			std::vector<OOX::CFtnEdnSepRef*             > m_arrEndnote;
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CEdnPos        > m_oPos;
		};
		//--------------------------------------------------------------------------------
		// CFtnDocProps 17.11.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CFtnDocProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFtnDocProps)
			CFtnDocProps()
			{
			}
			virtual ~CFtnDocProps()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
				{
					if ( m_arrFootnote[nIndex] ) delete m_arrFootnote[nIndex];
					m_arrFootnote[nIndex] = NULL;
				}
				m_arrFootnote.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFtnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:footnotePr>";

				for ( unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
				{
					if (m_arrFootnote[nIndex])
						sResult += m_arrFootnote[nIndex]->toXML();
				}

				if ( m_oNumFmt.IsInit() )
				{
					sResult += L"<w:numFmt ";
					sResult += m_oNumFmt->ToString();
					sResult += L"/>";
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += L"<w:numRestart ";
					sResult += m_oNumRestart->ToString();
					sResult += L"/>";
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += L"<w:numStart ";
					sResult += m_oNumStart->ToString();
					sResult += L"/>";
				}

				if ( m_oPos.IsInit() )
				{
					sResult += L"<w:pos ";
					sResult += m_oPos->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:footnotePr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_footnotePr;
			}
			std::vector<OOX::CFtnEdnSepRef*				> m_arrFootnote;
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CFtnPos        > m_oPos;
		};
	} // Settings
} // OOX

namespace OOX
{
	namespace Settings
	{
		//--------------------------------------------------------------------------------
		// CSchema 23.2.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSchema : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSchema)
			CSchema()
			{
			}
			virtual ~CSchema()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSchema::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = L"<sl:schema sl:uri=\"" + m_sUri
					+ L"\" sl:manifestLocation=\"" + m_sManifestLocation 
					+ L"\" sl:schemaLocation=\"" + m_sShemaLocation
					+ L"\" sl:schemaLanguage=\"" + m_sShemaLanguage + L"\"/>";
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_sl_schema;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		public:

            std::wstring m_sManifestLocation;
            std::wstring m_sShemaLanguage;
            std::wstring m_sShemaLocation;
            std::wstring m_sUri;

		};
		//--------------------------------------------------------------------------------
		// CSchemaLibrary 23.2.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSchemaLibrary : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSchemaLibrary)
			CSchemaLibrary()
			{
			}
			virtual ~CSchemaLibrary()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrSchema.size(); nIndex++ )
				{
					if ( m_arrSchema[nIndex] ) delete m_arrSchema[nIndex];
					m_arrSchema[nIndex] = NULL;
				}
				m_arrSchema.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSchemaLibrary::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_sl_schemaLibrary;
			}

			std::vector<OOX::Settings::CSchema*> m_arrSchema;
		};

		//--------------------------------------------------------------------------------
		// CShapeDefaults 9.7.2.1;9.7.2.2 (Part 4)
		//--------------------------------------------------------------------------------	
		class CShapeDefaults : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CShapeDefaults)
			CShapeDefaults()
			{
				m_eType = et_Unknown;
			}
			virtual ~CShapeDefaults()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

            virtual std::wstring toXML() const
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

			virtual EElementType getType() const
			{
				return m_eType;
			}


		public:

			EElementType m_eType;
		};
	} // namespace Settings

	//--------------------------------------------------------------------------------
	// CSettings 17.11.15.1.78
	//--------------------------------------------------------------------------------	
	class CSettings : public OOX::File, OOX::WritingElement
	{
	public:
		CSettings(OOX::Document *pMain) : OOX::File(pMain), OOX::WritingElement(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		
			if (docx)
			{
				if (docx->m_bGlossaryRead)	docx->m_oGlossary.settings = this;
				else						docx->m_oMain.settings = this;
			}
		}
		CSettings(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain), OOX::WritingElement(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			
			if (docx)
			{
				if (docx->m_bGlossaryRead)	docx->m_oGlossary.settings = this;
				else						docx->m_oMain.settings = this;
			}

			read( oPath );
		}
		CSettings(XmlUtils::CXmlNode& oNode) : File(NULL), WritingElement(NULL)
		{
			fromXML( oNode );
		}
		CSettings(XmlUtils::CXmlLiteReader& oReader) : File(NULL), WritingElement(NULL)
		{
			fromXML( oReader );
		}
		virtual ~CSettings()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrSmartTagType.size(); nIndex++ )
			{
				if ( m_arrSmartTagType[nIndex] )delete m_arrSmartTagType[nIndex];
				m_arrSmartTagType[nIndex] = NULL;
			}

			m_arrSmartTagType.clear();
		}
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			fromXML(oReader);
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
            std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
			
			sXml += toXML();
			
			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
			oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
		}
        virtual std::wstring toXML() const
		{
			std::wstring sXml = L"<w:settings xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:sl=\"http://schemas.openxmlformats.org/schemaLibrary/2006/main\" mc:Ignorable=\"w14 w15\">";

			// 1
			if ( m_oWriteProtection.IsInit() )
				sXml += m_oWriteProtection->toXML();

			// 2
			if ( m_oView.IsInit() )
				sXml += m_oView->toXML();

			// 3
			if ( m_oZoom.IsInit() )
				sXml += m_oZoom->toXML();

			// 4
			if ( m_oRemovePersonalInformation.IsInit() )
				sXml += L"<w:removePersonalInformation " + m_oRemovePersonalInformation->ToString() + L"/>";

			// 5
			if ( m_oRemoveDateAndTime.IsInit() )
				sXml += L"<w:removeDateAndTime " + m_oRemoveDateAndTime->ToString() + L"/>";

			// 6
			if ( m_oDoNotDisplayPageBoundaries.IsInit() )
				sXml += L"<w:doNotDisplayPageBoundaries " + m_oDoNotDisplayPageBoundaries->ToString() + L"/>";

			// 7
			if ( m_oDisplayBackgroundShape.IsInit() )
				sXml += L"<w:displayBackgroundShape " + m_oDisplayBackgroundShape->ToString() + L"/>";

			// 8
			if ( m_oPrintPostScriptOverText.IsInit() )
				sXml += L"<w:printPostScriptOverText " + m_oPrintPostScriptOverText->ToString() + L"/>";

			// 9
			if ( m_oPrintFractionalCharacterWidth.IsInit() )
				sXml += L"<w:printFractionalCharacterWidth " + m_oPrintFractionalCharacterWidth->ToString() + L"/>";

			// 10
			if ( m_oPrintFormsData.IsInit() )
				sXml += L"<w:printFormsData " + m_oPrintFormsData->ToString() + L"/>";

			// 11
			if ( m_oEmbedTrueTypeFonts.IsInit() )
				sXml += L"<w:embedTrueTypeFonts " + m_oEmbedTrueTypeFonts->ToString() + L"/>";

			// 12
			if ( m_oEmbedSystemFonts.IsInit() )
				sXml += L"<w:embedSystemFonts " + m_oEmbedSystemFonts->ToString() + L"/>";

			// 13
			if ( m_oSaveSubsetFonts.IsInit() )
				sXml += L"<w:saveSubsetFonts " + m_oSaveSubsetFonts->ToString() + L"/>";

			// 14
			if ( m_oSaveFormsData.IsInit() )
				sXml += L"<w:saveFormsData " + m_oSaveFormsData->ToString() + L"/>";

			// 15
			if ( m_oMirrorMargins.IsInit() )
				sXml += L"<w:mirrorMargins " + m_oMirrorMargins->ToString() + L"/>";

			// 16
			if ( m_oAlignBordersAndEdges.IsInit() )
				sXml += L"<w:alignBordersAndEdges " + m_oAlignBordersAndEdges->ToString() + L"/>";

			if ( m_oBordersDoNotSurroundHeader.IsInit() )
				sXml += L"<w:bordersDoNotSurroundHeader " + m_oBordersDoNotSurroundHeader->ToString() + L"/>";

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

			// 64
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

			// 99
			if ( m_oListSeparator.IsInit() )
				sXml += L"<w:listSeparator " + m_oListSeparator->ToString() + L"/>";

			sXml += L"</w:settings>";

			return sXml;
		}
		virtual EElementType getType() const
		{
			return et_w_settings;
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::Setting;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}


//------------------------------------------------------------------------------------------------------------------
		nullable<OOX::Settings::CWritingStyle>                        m_oActiveWritingStyle;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAlignBordersAndEdges;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAlwaysMergeEmptyNamespace;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAlwaysShowPlaceholderText;
		nullable<ComplexTypes::Word::String>                        m_oAttachedSchema;
		nullable<ComplexTypes::Word::CRel>                            m_oAttachedTemplate;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAutoFormatOverride;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAutoHyphenation;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oBookFoldPrinting;
		nullable<ComplexTypes::Word::CDecimalNumber>                  m_oBookFoldPrintingSheets;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oBookFoldRevPrinting;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oBordersDoNotSurroundFooter;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oBordersDoNotSurroundHeader;
		nullable<OOX::Settings::CCaptions>								m_oCaptions;
		nullable<OOX::Settings::CCharacterSpacing>						m_oCharacterSpacingControl;
		nullable<ComplexTypes::Word::String>							m_oClickAndTypeStyle;
		nullable<PPTX::Logic::ClrMap>									m_oClrSchemeMapping;
		nullable<OOX::Settings::CCompat>								m_oCompat;
		nullable<ComplexTypes::Word::CDecimalNumber>					m_oConsecutiveHyphenLimit;
		nullable<ComplexTypes::Word::String>							m_oDecimalSymbol;
		nullable<ComplexTypes::Word::String>							m_oDefaultTableStyle;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oDefaultTabStop;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDisplayBackgroundShape;
		nullable<ComplexTypes::Word::CDecimalNumber>                  m_oDisplayHorizontalDrawingGridEvery;
		nullable<ComplexTypes::Word::CDecimalNumber>                  m_oDisplayVerticalDrawingGridEvery;
		nullable<OOX::Settings::CDocProtect>                          m_oDocumentProtection;
		nullable<OOX::Settings::CDocType>                             m_oDocumentType;
		nullable<OOX::Settings::CDocVars>                             m_oDocVars;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotAutoCompressPictures;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotDemarcateInvalidXml;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotDisplayPageBoundaries;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotEmbedSmartTags;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotHyphenateCaps;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotIncludeSubdocsInStats;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotShadeFormData;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotTrackFormatting;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotTrackMoves;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotUseMarginsForDrawingGridOrigin;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDoNotValidateAgainstSchema;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oDrawingGridHorizontalOrigin;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oDrawingGridHorizontalSpacing;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oDrawingGridVerticalOrigin;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oDrawingGridVerticalSpacing;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oEmbedSystemFonts;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oEmbedTrueTypeFonts;
		nullable<OOX::Settings::CEdnDocProps>                         m_oEndnotePr;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oEvenAndOddHeaders;
		nullable<OOX::Settings::CFtnDocProps>                         m_oFootnotePr;
		nullable<ComplexTypes::Word::CEmpty>                          m_oForceUpgrade;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oFormsDesign;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oGutterAtTop;
		nullable<OOX::Settings::CShapeDefaults>                       m_oHdrShapeDefaults;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oHideGrammaticalErrors;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oHideSpellingErrors;
		nullable<ComplexTypes::Word::CTwipsMeasure>                   m_oHyphenationZone;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oIgnoreMixedContent;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oLinkStyles;
		nullable<ComplexTypes::Word::String>							m_oListSeparator;
	// TO DO: Settings::mailMerge
		nullable<OOX::Logic::CMathPr>								  m_oMathPr;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oMirrorMargins;
		nullable<OOX::Settings::CKinsoku>                             m_oNoLineBreaksAfter;
		nullable<OOX::Settings::CKinsoku>                             m_oNoLineBreaksBefore;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oNoPunctuationKerning;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintFormsData;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintFractionalCharacterWidth;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintPostScriptOverText;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPrintTwoOnOne;
		nullable<OOX::Settings::CProof>                               m_oProofState;
		nullable<OOX::Settings::CReadingModeInkLockDown>              m_oReadModeInkLockDown;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oRemoveDateAndTime;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oRemovePersonalInformation;
		nullable<OOX::Settings::CTrackChangesView>                    m_oRevisionView;
		nullable<OOX::Settings::CDocRsids>                            m_oRsids;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSaveFormsData;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSaveInvalidXml;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSavePreviewPicture;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSaveSubsetFonts;
		nullable<OOX::Settings::CSaveThroughXslt>                     m_oSaveThroughtXslt;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSaveXmlDataOnly;
		nullable<OOX::Settings::CSchemaLibrary>                       m_oSchemaLibrary;
		nullable<OOX::Settings::CShapeDefaults>                       m_oShapeDefaults;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oShowEnvelope;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oShowXMLTags;
		std::vector<OOX::Settings::CSmartTagType*>                    m_arrSmartTagType;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oStrictFirstAndLastChars;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oStyleLockQFSet;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oStyleLockTheme;
		nullable<OOX::Settings::CStylePaneFilter>                     m_oStylePaneFormatFilter;
		nullable<OOX::Settings::CStyleSort>                           m_oStylePaneSortMethod;
		nullable<ComplexTypes::Word::CDecimalNumberOrPrecent>         m_oSummaryLength;
		nullable<ComplexTypes::Word::CLanguage>                       m_oThemeFontLang;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oTrackRevisions;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUpdateFields;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUseXSLTWhenSaving;
		nullable<OOX::Settings::CView>                                m_oView;
		nullable<OOX::Settings::CWriteProtection>                     m_oWriteProtection;
		nullable<OOX::Settings::CZoom>                                m_oZoom;
	};
	class CSettingsCustom
	{
	public:
		static std::wstring GetSchemaUrl()
		{
			return L"http://schemas.onlyoffice.com/settingsCustom";
		}
		void FromXml(const std::wstring& sXml)
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
		std::wstring ToXml()
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
		bool IsEmpty()
		{
			return !(m_oSdtGlobalColor.IsInit() || m_oSdtGlobalShowHighlight.IsInit() || m_oSpecialFormsHighlight.IsInit());
		}
		nullable<ComplexTypes::Word::CColor> m_oSdtGlobalColor;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSdtGlobalShowHighlight;
		nullable<ComplexTypes::Word::CColor> m_oSpecialFormsHighlight;
	};
} // namespace OOX

