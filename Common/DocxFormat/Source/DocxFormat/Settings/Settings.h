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
#ifndef OOX_SETTINGS_INCLUDE_H_
#define OOX_SETTINGS_INCLUDE_H_

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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CWritingStyle::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:activeWritingStyle ");

				ComplexTypes_WriteAttribute ( _T("w:lang=\""),       m_oLang );
				ComplexTypes_WriteAttribute2( _T("w:vendorID=\""),   m_sVendorID );
				ComplexTypes_WriteAttribute2( _T("w:dllVersion=\""), m_sDllVersion );
				ComplexTypes_WriteAttribute ( _T("w:nlCheck=\""),    m_oNlCheck );
				ComplexTypes_WriteAttribute ( _T("w:checkStyle=\""), m_oCheckSyle );
				ComplexTypes_WriteAttribute2( _T("w:appName=\""),    m_sAppName );

				sResult += _T("/>");

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
							if      ( _T("w:appName")    == wsName ) m_sAppName    = oReader.GetText();
							break;

						case 'c':
							if      ( _T("w:checkStyle") == wsName ) m_oCheckSyle  = oReader.GetText();
							break;

						case 'd':
							if      ( _T("w:dllVersion") == wsName ) m_sDllVersion = oReader.GetText();
							break;

						case 'l':
							if      ( _T("w:lang")       == wsName ) m_oLang       = oReader.GetText();
							break;

						case 'n':
							if      ( _T("w:nlCheck")    == wsName ) m_oNlCheck    = oReader.GetText();
							break;

						case 'v':
							if      ( _T("w:vendorID")   == wsName ) m_sVendorID   = oReader.GetText();
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
                std::wstring sResult = _T("<w:autoCaption ");

				ComplexTypes_WriteAttribute2( _T("w:name=\""),    m_sName );
				ComplexTypes_WriteAttribute2( _T("w:caption=\""), m_sCaption );

				sResult += _T("/>");

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
							if      ( _T("w:caption") == wsName ) m_sCaption = oReader.GetText();
							break;

						case 'n':
							if      ( _T("w:name")    == wsName ) m_sName    = oReader.GetText();
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

					if ( _T("w:autoCaption") == sName )
					{
						OOX::Settings::CAutoCaption *oAC = new OOX::Settings::CAutoCaption(oReader);
						if (oAC) m_arrAutoCaption.push_back( oAC );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:autoCaptions>");

				for ( unsigned int nIndex = 0; nIndex < m_arrAutoCaption.size(); nIndex++ )
				{
					if (m_arrAutoCaption[nIndex])
						sResult += m_arrAutoCaption[nIndex]->toXML();
				}

				sResult += _T("</w:autoCaptions>");

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
                std::wstring sResult = _T("<w:caption w:name=\"") + m_sName + _T("\" ");

				ComplexTypes_WriteAttribute( _T("w:pos=\""),     m_oPos );
				ComplexTypes_WriteAttribute( _T("w:chapNum=\""), m_oChapNum );
				ComplexTypes_WriteAttribute( _T("w:heading=\""), m_oHeading );
				ComplexTypes_WriteAttribute( _T("w:noLabel=\""), m_oNoLabel );
				ComplexTypes_WriteAttribute( _T("w:numFmt=\""),  m_oNumFmt );
				ComplexTypes_WriteAttribute( _T("w:sep=\""),     m_oSep );

				sResult += _T("/>");

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
							if      ( _T("w:chapNum") == wsName ) m_oChapNum = oReader.GetText();
							break;

						case 'h':
							if      ( _T("w:heading") == wsName ) m_oHeading = oReader.GetText();
							break;

						case 'n':
							if      ( _T("w:name")    == wsName ) m_sName    = oReader.GetText();
							else if ( _T("w:noLabel") == wsName ) m_oNoLabel = oReader.GetText();
							else if ( _T("w:numFmt")  == wsName ) m_oNumFmt  = oReader.GetText();
							break;

						case 'p':
							if      ( _T("w:pos")     == wsName ) m_oPos     = oReader.GetText();
							break;

						case 's':
							if      ( _T("w:sep")     == wsName ) m_oSep     = oReader.GetText();
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

					if ( _T("w:caption") == sName )
					{
						OOX::Settings::CCaption *oC = new OOX::Settings::CCaption(oReader);
						if (oC) m_arrCaption.push_back( oC );
					}
					else if ( _T("w:autoCaptions") == sName )
						m_oAutoCaptions = oReader;
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:captions>");

				for ( unsigned int nIndex = 0; nIndex < m_arrCaption.size(); nIndex++ )
				{
					if (m_arrCaption[nIndex])
						sResult += m_arrCaption[nIndex]->toXML();
				}

				if ( m_oAutoCaptions.IsInit() )
					sResult += m_oAutoCaptions->toXML();

				sResult += _T("</w:captions>");

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
                std::wstring sResult = _T("<w:characterSpacingControl w:val=\"") + m_oVal.ToString() + _T("\"/>");
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
							if      ( _T("w:val") == wsName ) m_oVal = oReader.GetText();
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
                std::wstring sResult = _T("<w:compatSetting ");

				ComplexTypes_WriteAttribute2( _T("w:name=\""), m_sName );
				ComplexTypes_WriteAttribute2( _T("w:uri=\""),  m_sUri );
				ComplexTypes_WriteAttribute2( _T("w:val=\""),  m_sVal );

				sResult += _T("/>");

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
							if      ( _T("w:name") == wsName ) m_sName = oReader.GetText();
							break;

						case 'u':
							if      ( _T("w:uri")  == wsName ) m_sUri  = oReader.GetText();
							break;

						case 'v':
							if      ( _T("w:val")  == wsName ) m_sVal  = oReader.GetText();
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

					if ( _T("w:useSingleBorderforContiguousCells") == sName )
						m_oUseSingleBorderforContiguousCells = oReader;
					else if ( _T("w:wpJustification") == sName )
						m_oWpJustification = oReader;
					else if ( _T("w:noTabHangInd") == sName )
						m_oNoTabHangInd = oReader;
					else if ( _T("w:noLeading") == sName )
						m_oNoLeading = oReader;
					else if ( _T("w:spaceForUL") == sName )
						m_oSpaceForUL = oReader;
					else if ( _T("w:noColumnBalance") == sName )
						m_oNoColumnBalance = oReader;
					else if ( _T("w:balanceSingleByteDoubleByteWidth") == sName )
						m_oBalanceSingleByteDoubleByteWidth = oReader;
					else if ( _T("w:noExtraLineSpacing") == sName )
						m_oNoExtraLineSpacing = oReader;
					else if ( _T("w:doNotLeaveBackslashAlone") == sName )
						m_oDoNotLeaveBackslashAlone = oReader;
					else if ( _T("w:ulTrailSpace") == sName )
						m_oUlTrailSpace = oReader;
					else if ( _T("w:doNotExpandShiftReturn") == sName )
						m_oDoNotExpandShiftReturn = oReader;
					else if ( _T("w:spacingInWholePoints") == sName )
						m_oSpacingInWholePoints = oReader;
					else if ( _T("w:lineWrapLikeWord6") == sName )
						m_oLineWrapLikeWord6 = oReader;
					else if ( _T("w:printBodyTextBeforeHeader") == sName )
						m_oPrintBodyTextBeforeHeader = oReader;
					else if ( _T("w:printColBlack") == sName )
						m_oPrintColBlack = oReader;
					else if ( _T("w:wpSpaceWidth") == sName )
						m_oWpSpaceWidth = oReader;
					else if ( _T("w:showBreaksInFrames") == sName )
						m_oShowBreaksInFrames = oReader;
					else if ( _T("w:subFontBySize") == sName )
						m_oSubFontBySize = oReader;
					else if ( _T("w:suppressBottomSpacing") == sName )
						m_oSuppressBottomSpacing = oReader;
					else if ( _T("w:suppressTopSpacing") == sName )
						m_oSuppressTopSpacing = oReader;
					else if ( _T("w:suppressSpacingAtTopOfPage") == sName )
						m_oSuppressSpacingAtTopOfPage = oReader;
					else if ( _T("w:suppressTopSpacingWP") == sName )
						m_oSuppressTopSpacingWP = oReader;
					else if ( _T("w:suppressSpBfAfterPgBrk") == sName )
						m_oSuppressSpBfAfterPgBrk = oReader;
					else if ( _T("w:swapBordersFacingPages") == sName )
						m_oSwapBordersFacingPages = oReader;
					else if ( _T("w:convMailMergeEsc") == sName )
						m_oConvMailMergeEsc = oReader;
					else if ( _T("w:truncateFontHeightsLikeWP6") == sName )
						m_oTruncateFontHeightsLikeWP6 = oReader;
					else if ( _T("w:mwSmallCaps") == sName )
						m_oMwSmallCaps = oReader;
					else if ( _T("w:usePrinterMetrics") == sName )
						m_oUsePrinterMetrics = oReader;
					else if ( _T("w:doNotSuppressParagraphBorders") == sName )
						m_oDoNotSuppressParagraphBorders = oReader;
					else if ( _T("w:wrapTrailSpaces") == sName )
						m_oWrapTrailSpaces = oReader;
					else if ( _T("w:footnoteLayoutLikeWW8") == sName )
						m_oFootnoteLayoutLikeWW8 = oReader;
					else if ( _T("w:shapeLayoutLikeWW8") == sName )
						m_oShapeLayoutLikeWW8 = oReader;
					else if ( _T("w:alignTablesRowByRow") == sName )
						m_oAlignTablesRowByRow = oReader;
					else if ( _T("w:forgetLastTabAlignment") == sName )
						m_oForgetLastTabAlignment = oReader;
					else if ( _T("w:adjustLineHeightInTable") == sName )
						m_oAdjustLineHeightInTable = oReader;
					else if ( _T("w:autoSpaceLikeWord95") == sName )
						m_oAutoSpaceLikeWord95 = oReader;
					else if ( _T("w:noSpaceRaiseLower") == sName )
						m_oNoSpaceRaiseLower = oReader;
					else if ( _T("w:doNotUseHTMLParagraphAutoSpacing") == sName )
						m_oDoNotUseHTMLParagraphAutoSpacing = oReader;
					else if ( _T("w:layoutRawTableWidth") == sName )
						m_oLayoutRawTableWidth = oReader;
					else if ( _T("w:layoutTableRowsApart") == sName )
						m_oLayoutTableRowsApart = oReader;
					else if ( _T("w:useWord97LineBreakRules") == sName )
						m_oUseWord97LineBreakRules = oReader;
					else if ( _T("w:doNotBreakWrappedTables") == sName )
						m_oDoNotBreakWrappedTables = oReader;
					else if ( _T("w:doNotSnapToGridInCell") == sName )
						m_oDoNotSnapToGridInCell = oReader;
					else if ( _T("w:selectFldWithFirstOrLastChar") == sName )
						m_oSelectFldWithFirstOrLastChar = oReader;
					else if ( _T("w:applyBreakingRules") == sName )
						m_oApplyBreakingRules = oReader;
					else if ( _T("w:doNotWrapTextWithPunct") == sName )
						m_oDoNotWrapTextWithPunct = oReader;
					else if ( _T("w:doNotUseEastAsianBreakRules") == sName )
						m_oDoNotUseEastAsianBreakRules = oReader;
					else if ( _T("w:useWord2002TableStyleRules") == sName )
						m_oUseWord2002TableStyleRules = oReader;
					else if ( _T("w:growAutofit") == sName )
						m_oGrowAutofit = oReader;
					else if ( _T("w:useFELayout") == sName )
						m_oUseFELayout = oReader;
					else if ( _T("w:useNormalStyleForList") == sName )
						m_oUseNormalStyleForList = oReader;
					else if ( _T("w:doNotUseIndentAsNumberingTabStop") == sName )
						m_oDoNotUseIndentAsNumberingTabStop = oReader;
					else if ( _T("w:useAltKinsokuLineBreakRules") == sName )
						m_oUseAltKinsokuLineBreakRules = oReader;
					else if ( _T("w:allowSpaceOfSameStyleInTable") == sName )
						m_oAllowSpaceOfSameStyleInTable = oReader;
					else if ( _T("w:doNotSuppressIndentation") == sName )
						m_oDoNotSuppressIndentation = oReader;
					else if ( _T("w:doNotAutofitConstrainedTables") == sName )
						m_oDoNotAutofitConstrainedTables = oReader;
					else if ( _T("w:autofitToFirstFixedWidthCell") == sName )
						m_oAutofitToFirstFixedWidthCell = oReader;
					else if ( _T("w:underlineTabInNumList") == sName )
						m_oUnderlineTabInNumList = oReader;
					else if ( _T("w:displayHangulFixedWidth") == sName )
						m_oDisplayHangulFixedWidth = oReader;
					else if ( _T("w:splitPgBreakAndParaMark") == sName )
						m_oSplitPgBreakAndParaMark = oReader;
					else if ( _T("w:doNotVertAlignCellWithSp") == sName )
						m_oDoNotVertAlignCellWithSp = oReader;
					else if ( _T("w:doNotBreakConstrainedForcedTable") == sName )
						m_oDoNotBreakConstrainedForcedTable = oReader;
					else if ( _T("w:doNotVertAlignInTxbx") == sName )
						m_oDoNotVertAlignInTxbx = oReader;
					else if ( _T("w:useAnsiKerningPairs") == sName )
						m_oUseAnsiKerningPairs = oReader;
					else if ( _T("w:cachedColBalance") == sName )
						m_oCachedColBalance = oReader;
					else if ( _T("w:compatSetting") == sName )
					{
						OOX::Settings::CCompatSetting *oCS = new OOX::Settings::CCompatSetting(oReader);
						if (oCS)m_arrCompatSettings.push_back( oCS );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:compat>");

				WritingElement_WriteNode_1( _T("<w:useSingleBorderforContiguousCells "),      m_oUseSingleBorderforContiguousCells );
				WritingElement_WriteNode_1( _T("<w:wpJustification "),      m_oWpJustification );
				WritingElement_WriteNode_1( _T("<w:noTabHangInd "),      m_oNoTabHangInd );
				WritingElement_WriteNode_1( _T("<w:noLeading "),      m_oNoLeading );
				WritingElement_WriteNode_1( _T("<w:spaceForUL "),      m_oSpaceForUL );
				WritingElement_WriteNode_1( _T("<w:noColumnBalance "),      m_oNoColumnBalance );
				WritingElement_WriteNode_1( _T("<w:balanceSingleByteDoubleByteWidth "),      m_oBalanceSingleByteDoubleByteWidth );
				WritingElement_WriteNode_1( _T("<w:noExtraLineSpacing "),      m_oNoExtraLineSpacing );
				WritingElement_WriteNode_1( _T("<w:doNotLeaveBackslashAlone "),      m_oDoNotLeaveBackslashAlone );
				WritingElement_WriteNode_1( _T("<w:ulTrailSpace "),      m_oUlTrailSpace );
				WritingElement_WriteNode_1( _T("<w:doNotExpandShiftReturn "),      m_oDoNotExpandShiftReturn );
				WritingElement_WriteNode_1( _T("<w:spacingInWholePoints "),      m_oSpacingInWholePoints );
				WritingElement_WriteNode_1( _T("<w:lineWrapLikeWord6 "),      m_oLineWrapLikeWord6 );
				WritingElement_WriteNode_1( _T("<w:printBodyTextBeforeHeader "),      m_oPrintBodyTextBeforeHeader );
				WritingElement_WriteNode_1( _T("<w:printColBlack "),      m_oPrintColBlack );
				WritingElement_WriteNode_1( _T("<w:wpSpaceWidth "),      m_oWpSpaceWidth );
				WritingElement_WriteNode_1( _T("<w:showBreaksInFrames "),      m_oShowBreaksInFrames );
				WritingElement_WriteNode_1( _T("<w:subFontBySize "),      m_oSubFontBySize );
				WritingElement_WriteNode_1( _T("<w:suppressBottomSpacing "),      m_oSuppressBottomSpacing );
				WritingElement_WriteNode_1( _T("<w:suppressTopSpacing "),      m_oSuppressTopSpacing );
				WritingElement_WriteNode_1( _T("<w:suppressSpacingAtTopOfPage "),      m_oSuppressSpacingAtTopOfPage );
				WritingElement_WriteNode_1( _T("<w:suppressTopSpacingWP "),      m_oSuppressTopSpacingWP );
				WritingElement_WriteNode_1( _T("<w:suppressSpBfAfterPgBrk "),      m_oSuppressSpBfAfterPgBrk );
				WritingElement_WriteNode_1( _T("<w:swapBordersFacingPages "),      m_oSwapBordersFacingPages );
				WritingElement_WriteNode_1( _T("<w:convMailMergeEsc "),      m_oConvMailMergeEsc );
				WritingElement_WriteNode_1( _T("<w:truncateFontHeightsLikeWP6 "),      m_oTruncateFontHeightsLikeWP6 );
				WritingElement_WriteNode_1( _T("<w:mwSmallCaps "),      m_oMwSmallCaps );
				WritingElement_WriteNode_1( _T("<w:usePrinterMetrics "),      m_oUsePrinterMetrics );
				WritingElement_WriteNode_1( _T("<w:doNotSuppressParagraphBorders "),      m_oDoNotSuppressParagraphBorders );
				WritingElement_WriteNode_1( _T("<w:wrapTrailSpaces "),      m_oWrapTrailSpaces );
				WritingElement_WriteNode_1( _T("<w:footnoteLayoutLikeWW8 "),      m_oFootnoteLayoutLikeWW8 );
				WritingElement_WriteNode_1( _T("<w:shapeLayoutLikeWW8 "),      m_oShapeLayoutLikeWW8 );
				WritingElement_WriteNode_1( _T("<w:alignTablesRowByRow "),      m_oAlignTablesRowByRow );
				WritingElement_WriteNode_1( _T("<w:forgetLastTabAlignment "),      m_oForgetLastTabAlignment );
				WritingElement_WriteNode_1( _T("<w:adjustLineHeightInTable "),      m_oAdjustLineHeightInTable );
				WritingElement_WriteNode_1( _T("<w:autoSpaceLikeWord95 "),      m_oAutoSpaceLikeWord95 );
				WritingElement_WriteNode_1( _T("<w:noSpaceRaiseLower "),      m_oNoSpaceRaiseLower );
				WritingElement_WriteNode_1( _T("<w:doNotUseHTMLParagraphAutoSpacing "),      m_oDoNotUseHTMLParagraphAutoSpacing );
				WritingElement_WriteNode_1( _T("<w:layoutRawTableWidth "),      m_oLayoutRawTableWidth );
				WritingElement_WriteNode_1( _T("<w:layoutTableRowsApart "),      m_oLayoutTableRowsApart );
				WritingElement_WriteNode_1( _T("<w:useWord97LineBreakRules "),      m_oUseWord97LineBreakRules );
				WritingElement_WriteNode_1( _T("<w:doNotBreakWrappedTables "),      m_oDoNotBreakWrappedTables );
				WritingElement_WriteNode_1( _T("<w:doNotSnapToGridInCell "),      m_oDoNotSnapToGridInCell );
				WritingElement_WriteNode_1( _T("<w:selectFldWithFirstOrLastChar "),      m_oSelectFldWithFirstOrLastChar );
				WritingElement_WriteNode_1( _T("<w:applyBreakingRules "),      m_oApplyBreakingRules );
				WritingElement_WriteNode_1( _T("<w:doNotWrapTextWithPunct "),      m_oDoNotWrapTextWithPunct );
				WritingElement_WriteNode_1( _T("<w:doNotUseEastAsianBreakRules "),      m_oDoNotUseEastAsianBreakRules );
				WritingElement_WriteNode_1( _T("<w:useWord2002TableStyleRules "),      m_oUseWord2002TableStyleRules );
				WritingElement_WriteNode_1( _T("<w:growAutofit "),      m_oGrowAutofit );
				WritingElement_WriteNode_1( _T("<w:useFELayout "),      m_oUseFELayout );
				WritingElement_WriteNode_1( _T("<w:useNormalStyleForList "),      m_oUseNormalStyleForList );
				WritingElement_WriteNode_1( _T("<w:doNotUseIndentAsNumberingTabStop "),      m_oDoNotUseIndentAsNumberingTabStop );
				WritingElement_WriteNode_1( _T("<w:useAltKinsokuLineBreakRules "),      m_oUseAltKinsokuLineBreakRules );
				WritingElement_WriteNode_1( _T("<w:allowSpaceOfSameStyleInTable "),      m_oAllowSpaceOfSameStyleInTable );
				WritingElement_WriteNode_1( _T("<w:doNotSuppressIndentation "),      m_oDoNotSuppressIndentation );
				WritingElement_WriteNode_1( _T("<w:doNotAutofitConstrainedTables "),      m_oDoNotAutofitConstrainedTables );
				WritingElement_WriteNode_1( _T("<w:autofitToFirstFixedWidthCell "),      m_oAutofitToFirstFixedWidthCell );
				WritingElement_WriteNode_1( _T("<w:underlineTabInNumList "),      m_oUnderlineTabInNumList );
				WritingElement_WriteNode_1( _T("<w:displayHangulFixedWidth "),      m_oDisplayHangulFixedWidth );
				WritingElement_WriteNode_1( _T("<w:splitPgBreakAndParaMark "),      m_oSplitPgBreakAndParaMark );
				WritingElement_WriteNode_1( _T("<w:doNotVertAlignCellWithSp "),      m_oDoNotVertAlignCellWithSp );
				WritingElement_WriteNode_1( _T("<w:doNotBreakConstrainedForcedTable "),      m_oDoNotBreakConstrainedForcedTable );
				WritingElement_WriteNode_1( _T("<w:doNotVertAlignInTxbx "),      m_oDoNotVertAlignInTxbx );
				WritingElement_WriteNode_1( _T("<w:useAnsiKerningPairs "),      m_oUseAnsiKerningPairs );
				WritingElement_WriteNode_1( _T("<w:cachedColBalance "),      m_oCachedColBalance );

				for ( unsigned int nIndex = 0; nIndex < m_arrCompatSettings.size(); nIndex++ )
				{
					if (m_arrCompatSettings[nIndex])
						sResult += m_arrCompatSettings[nIndex]->toXML();
				}

				sResult += _T("</w:compat>");

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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocProtect::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:documentProtection ");

				ComplexTypes_WriteAttribute ( _T("w:edit=\""),          m_oEdit );
				ComplexTypes_WriteAttribute ( _T("w:formatting=\""),    m_oFormatting );
				ComplexTypes_WriteAttribute ( _T("w:enforcement=\""),   m_oEnforcment );
				ComplexTypes_WriteAttribute ( _T("w:algorithmName=\""), m_oAlgorithmName );
				ComplexTypes_WriteAttribute2( _T("w:hashValue=\""),     m_sHashValue );
				ComplexTypes_WriteAttribute2( _T("w:saltValue=\""),     m_sSaltValue );
				ComplexTypes_WriteAttribute ( _T("w:spinCount=\""),     m_oSpinCount );

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_documentProtection;
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
							if      ( _T("w:algorithmName") == wsName ) m_oAlgorithmName = oReader.GetText();
							break;

						case 'e':
							if      ( _T("w:edit")          == wsName ) m_oEdit          = oReader.GetText();
							else if ( _T("w:enforcement")   == wsName ) m_oEnforcment    = oReader.GetText();
							break;

						case 'f':
							if      ( _T("w:formatting")    == wsName ) m_oFormatting    = oReader.GetText();
							break;
						case 'h':
							if      ( _T("w:hashValue")     == wsName ) m_sHashValue     = oReader.GetText();
							break;
						case 's':
							if      ( _T("w:saltValue")     == wsName ) m_sSaltValue     = oReader.GetText();
							else if ( _T("w:spinCount")     == wsName ) m_oSpinCount     = oReader.GetText();
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

			// TO DO: Добавить атрибуты из 9.7.2.3 Part4

			// Attributes
			nullable<SimpleTypes::CCryptAlgoritmName<>> m_oAlgorithmName;
			nullable<SimpleTypes::CDocProtect<>>        m_oEdit;
			nullable<SimpleTypes::COnOff<>>             m_oEnforcment;
			nullable<SimpleTypes::COnOff<>>             m_oFormatting;
            nullable<std::wstring>                           m_sHashValue;
            nullable<std::wstring>                           m_sSaltValue;
			nullable<SimpleTypes::CDecimalNumber<>>     m_oSpinCount;
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocType::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:documentType w:val=\"") + m_oVal.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_documentType;
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
							if      ( _T("w:val") == wsName ) m_oVal = oReader.GetText();
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

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocVar::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:docVar w:name=\"") + m_sName + _T("\" w:val=\"") + m_sVal + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_docVar;
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
							if      ( _T("w:name") == wsName ) m_sName = oReader.GetText();
							break;

						case 'v':
							if      ( _T("w:val")  == wsName ) m_sVal  = oReader.GetText();
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

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocVars::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("w:docVar") == sName )
					{
						OOX::Settings::CDocVar *oDV = new OOX::Settings::CDocVar(oReader);
						if (oDV) m_arrDocVar.push_back( oDV );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:docVars>");

				for (unsigned int nIndex = 0; nIndex < m_arrDocVar.size(); nIndex++ )
				{
					if (m_arrDocVar[nIndex])
						sResult += m_arrDocVar[nIndex]->toXML();
				}

				sResult += _T("</w:docVars>");

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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CKinsoku::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;

				std::wstring sName = oReader.GetName();
				if ( _T("w:noLineBreaksAfter") == sName )
					m_eType = et_w_noLineBreaksAfter;
				else if ( _T("w:noLineBreaksBefore") == sName )
					m_eType = et_w_noLineBreaksBefore;
				else
					return;

				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult;
				
				if ( et_w_noLineBreaksAfter == m_eType )
					sResult	= _T("<w:noLineBreaksAfter w:lang=\"") + m_oLang.ToString() + _T("\" w:val=\"") + m_sVal + _T("\"/>");
				else if ( et_w_noLineBreaksBefore == m_eType ) 
					sResult	= _T("<w:noLineBreaksBefore w:lang=\"") + m_oLang.ToString() + _T("\" w:val=\"") + m_sVal + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
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
						case 'l':
							if      ( _T("w:lang") == wsName ) m_oLang = oReader.GetText();
							break;

						case 'v':
							if      ( _T("w:val")  == wsName ) m_sVal  = oReader.GetText();
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

			EElementType       m_eType;

			// Attributes
			SimpleTypes::CLang m_oLang;
            std::wstring            m_sVal;
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CProof::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:proofState ");

				ComplexTypes_WriteAttribute ( _T("w:spelling=\""), m_oSpelling );
				ComplexTypes_WriteAttribute ( _T("w:grammar=\""),  m_oGrammar );

				sResult += _T("/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_proofState;
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
						case 'g':
							if      ( _T("w:grammar")  == wsName ) m_oGrammar  = oReader.GetText();
							break;

						case 's':
							if      ( _T("w:spelling") == wsName ) m_oSpelling = oReader.GetText();
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

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CReadingModeInkLockDown::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:readModeInkLockDown ");

				sResult += _T("w:w=\"")        + m_oW.ToString() 
					  + _T("\" w:h=\"")        + m_oH.ToString() 
					  + _T("\" w:fontSz=\"")   + m_oFontSz.ToString() 
					  + _T("\" w:actualPg=\"") + m_oActualPg.ToString() + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_readModeInkLockDown;
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
							if      ( _T("w:actualPg")  == wsName ) m_oActualPg = oReader.GetText();
							break;

						case 'f':
							if      ( _T("w:fontSz")    == wsName ) m_oFontSz   = oReader.GetText();
							break;

						case 'h':
							if      ( _T("w:h")         == wsName ) m_oH        = oReader.GetText();
							break;

						case 'w':
							if      ( _T("w:w")         == wsName ) m_oW        = oReader.GetText();
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

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:revisionView ");

				ComplexTypes_WriteAttribute ( _T("w:markup=\""),         m_oMarkup );
				ComplexTypes_WriteAttribute ( _T("w:comments=\""),       m_oComments );
				ComplexTypes_WriteAttribute ( _T("w:insDel=\""),         m_oInsDel );
				ComplexTypes_WriteAttribute ( _T("w:formatting=\""),     m_oFormatting );
				ComplexTypes_WriteAttribute ( _T("w:inkAnnotations=\""), m_oInkAnnotataions );

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_revisionView;
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
							if      ( _T("w:comments")       == wsName ) m_oComments        = oReader.GetText();
							break;

						case 'f':
							if      ( _T("w:formatting")     == wsName ) m_oFormatting      = oReader.GetText();
							break;

						case 'i':
							if      ( _T("w:inkAnnotations") == wsName ) m_oInkAnnotataions = oReader.GetText();
							else if ( _T("w:insDel")         == wsName ) m_oInsDel          = oReader.GetText();
							break;

						case 'm':
							if      ( _T("w:markup")         == wsName ) m_oMarkup          = oReader.GetText();
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CDocRsids::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("w:rsid") == sName )
					{
						ComplexTypes::Word::CLongHexNumber *oRsid = new ComplexTypes::Word::CLongHexNumber(oReader);
						if (oRsid) m_arrRsid.push_back( oRsid );
					}
					else if ( _T("w:rsidRoot") == sName )
						m_oRsidRoot = oReader;
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:rsids>");

				if ( m_oRsidRoot.IsInit() )
				{
					sResult += _T("<w:rsidRoot ");
					sResult += m_oRsidRoot->ToString();
					sResult += _T("/>");
				}

				for ( unsigned int nIndex = 0; nIndex < m_arrRsid.size(); nIndex++ )
				{
					sResult += _T("<w:rsid ");
					if (m_arrRsid[nIndex])
						sResult += m_arrRsid[nIndex]->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:rsids>");

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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTrackChangesView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:saveThroughXslt ");

				ComplexTypes_WriteAttribute ( _T("r:id=\""),         m_rId );
				ComplexTypes_WriteAttribute2( _T("w:solutionID=\""), m_sSolutionID );

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_saveThroughXslt;
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
					if      ( _T("r:id")         == wsName ) m_rId         = oReader.GetText();
					else if ( _T("w:solutionID") == wsName ) m_sSolutionID = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSmartTagType::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:smartTagType ");

				ComplexTypes_WriteAttribute2( _T("w:namespaceuri=\""), m_sNameSpaceUri );
				ComplexTypes_WriteAttribute2( _T("w:name=\""),         m_sName );
				ComplexTypes_WriteAttribute2( _T("w:url=\""),          m_sUrl );

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_smartTagType;
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
					if      ( _T("w:name")         == wsName ) m_sName         = oReader.GetText();
					else if ( _T("w:namespaceuri") == wsName ) m_sNameSpaceUri = oReader.GetText();
					else if ( _T("w:url")          == wsName ) m_sUrl          = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStylePaneFilter::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:stylePaneFormatFilter ");

				ComplexTypes_WriteAttribute( _T("w:allStyles=\""),                    m_oAllStyles );
				ComplexTypes_WriteAttribute( _T("w:alternateStyleNames=\""),          m_oAlternateStyleNames );
				ComplexTypes_WriteAttribute( _T("w:clearFormatting=\""),              m_oClearFormatting );
				ComplexTypes_WriteAttribute( _T("w:customStyles=\""),                 m_oCustomStyles );
				ComplexTypes_WriteAttribute( _T("w:directFormattingOnNumbering=\""),  m_oDirectFormattingOnNumbering );
				ComplexTypes_WriteAttribute( _T("w:directFormattingOnParagraphs=\""), m_oDirectFormattingOnParagraphs );
				ComplexTypes_WriteAttribute( _T("w:directFormattingOnRuns=\""),       m_oDirectFormattingOnRuns );
				ComplexTypes_WriteAttribute( _T("w:directFormattingOnTables=\""),     m_oDirectFormattingOnTables );
				ComplexTypes_WriteAttribute( _T("w:headingStyles=\""),                m_oHeadingStyles );
				ComplexTypes_WriteAttribute( _T("w:latentStyles=\""),                 m_oLatentStyles );
				ComplexTypes_WriteAttribute( _T("w:numberingStyles=\""),              m_oNumberingStyles );
				ComplexTypes_WriteAttribute( _T("w:stylesInUse=\""),                  m_oStylesInUse );
				ComplexTypes_WriteAttribute( _T("w:tableStyles=\""),                  m_oTableStyles );
				ComplexTypes_WriteAttribute( _T("w:top3HeadingStyles=\""),            m_oTop3HeadingStyles );
				ComplexTypes_WriteAttribute( _T("w:visibleStyles=\""),                m_oVisibleStyles );

				sResult += _T("/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneFormatFilter;
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
							if      ( _T("w:allStyles")           == wsName ) m_oAllStyles           = oReader.GetText();
							else if ( _T("w:alternateStyleNames") == wsName ) m_oAlternateStyleNames = oReader.GetText();
							break;

						case 'c':
							if      ( _T("w:clearFormatting") == wsName ) m_oClearFormatting = oReader.GetText();
							else if ( _T("w:customStyles")    == wsName ) m_oCustomStyles    = oReader.GetText();
							break;

						case 'd':
							if      ( _T("w:directFormattingOnNumbering")  == wsName ) m_oDirectFormattingOnNumbering  = oReader.GetText();
							else if ( _T("w:directFormattingOnParagraphs") == wsName ) m_oDirectFormattingOnParagraphs = oReader.GetText();
							else if ( _T("w:directFormattingOnRuns")       == wsName ) m_oDirectFormattingOnRuns       = oReader.GetText();
							else if ( _T("w:directFormattingOnTables")     == wsName ) m_oDirectFormattingOnTables     = oReader.GetText();
							break;

						case 'h':
							if      ( _T("w:headingStyles") == wsName ) m_oHeadingStyles = oReader.GetText();
							break;

						case 'l':
							if      ( _T("w:latentStyles") == wsName ) m_oLatentStyles = oReader.GetText();
							break;

						case 'n':
							if      ( _T("w:numberingStyles") == wsName ) m_oNumberingStyles = oReader.GetText();
							break;

						case 's':
							if      ( _T("w:stylesInUse") == wsName ) m_oStylesInUse = oReader.GetText();
							break;

						case 't':
							if      ( _T("w:tableStyles")       == wsName ) m_oTableStyles       = oReader.GetText();
							else if ( _T("w:top3HeadingStyles") == wsName ) m_oTop3HeadingStyles = oReader.GetText();
							break;

						case 'v':
							if      ( _T("w:visibleStyles") == wsName ) m_oVisibleStyles = oReader.GetText();
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

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStyleSort::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:stylePaneSortMethod w:val=\"") + m_oVal.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneSortMethod;
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
					if ( _T("w:val") == wsName ) 
						m_oVal = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CView::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:view w:val=\"") + m_oVal.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneSortMethod;
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
					if ( _T("w:val") == wsName ) 
						m_oVal = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CWriteProtection::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:writeProtection ");

				ComplexTypes_WriteAttribute ( _T("w:recommended=\""),   m_oRecommended );
				ComplexTypes_WriteAttribute ( _T("w:algorithmName=\""), m_oAlgorithmName );
				ComplexTypes_WriteAttribute2( _T("w:hashValue=\""),     m_sHashValue );
				ComplexTypes_WriteAttribute2( _T("w:saltValue=\""),     m_sSaltValue );
				ComplexTypes_WriteAttribute ( _T("w:spinCount=\""),     m_oSpinCount );

				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_writeProtection;
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
							if      ( L"w:algorithmName" == wsName ) m_oAlgorithmName = oReader.GetText();
							break;
						case 'c':
							if      ( L"w:cryptProviderType"	== wsName ) m_oCryptProviderType	= oReader.GetText();
							else if	( L"w:cryptAlgorithmSid"	== wsName ) m_oAlgorithmName		= oReader.GetText();
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
			nullable<std::wstring>	m_oCryptProviderType;
			nullable<std::wstring>	m_oCryptAlgorithmType;
			nullable<std::wstring>	m_oCryptAlgorithmClass;

			nullable<SimpleTypes::CCryptAlgoritmName<>> m_oAlgorithmName;
            nullable<std::wstring>						m_sHashValue;
			nullable<SimpleTypes::COnOff<>>             m_oRecommended;
            nullable<std::wstring>						m_sSaltValue;
			nullable<SimpleTypes::CDecimalNumber<>>     m_oSpinCount;
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CZoom::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:zoom ");

				ComplexTypes_WriteAttribute ( _T("w:val=\""), m_oVal );

				sResult += _T("w:percent=\"") + m_oPercent.ToString() + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_zoom;
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
						case 'p':
							if      ( _T("w:percent") == wsName ) m_oPercent = oReader.GetText();
							break;
						case 'v':
							if      ( _T("w:val")     == wsName ) m_oVal     = oReader.GetText();
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
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CEdnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("w:numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("w:numRestart") == sName )
						m_oNumRestart = oReader;
					else if ( _T("w:numStart") == sName )
						m_oNumStart = oReader;
					else if ( _T("w:pos") == sName )
						m_oPos = oReader;
					else if ( _T("w:endnote") == sName )
					{
						OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
						if (oFE) m_arrEndnote.push_back( oFE );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:endnotePr>");

				for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
				{
					if (m_arrEndnote[nIndex])
						sResult += m_arrEndnote[nIndex]->toXML();
				}

				if ( m_oNumFmt.IsInit() )
				{
					sResult += _T("<w:numFmt ");
					sResult += m_oNumFmt->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += _T("<w:numRestart ");
					sResult += m_oNumRestart->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += _T("<w:numStart ");
					sResult += m_oNumStart->ToString();
					sResult += _T("/>");
				}

				if ( m_oPos.IsInit() )
				{
					sResult += _T("<w:pos ");
					sResult += m_oPos->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:endnotePr>");

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

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFtnDocProps::fromXML(XmlUtils::CXmlNode& oNode)
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("w:numFmt") == sName )
						m_oNumFmt = oReader;
					else if ( _T("w:numRestart") == sName )
						m_oNumRestart = oReader;
					else if ( _T("w:numStart") == sName )
						m_oNumStart = oReader;
					else if ( _T("w:pos") == sName )
						m_oPos = oReader;
					else if ( _T("w:footnote") == sName )
					{
						OOX::CFtnEdnSepRef *oFE = new OOX::CFtnEdnSepRef(oReader);
						if (oFE) m_arrFootnote.push_back( oFE );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:footnotePr>");

				for ( unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
				{
					if (m_arrFootnote[nIndex])
						sResult += m_arrFootnote[nIndex]->toXML();
				}

				if ( m_oNumFmt.IsInit() )
				{
					sResult += _T("<w:numFmt ");
					sResult += m_oNumFmt->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumRestart.IsInit() )
				{
					sResult += _T("<w:numRestart ");
					sResult += m_oNumRestart->ToString();
					sResult += _T("/>");
				}

				if ( m_oNumStart.IsInit() )
				{
					sResult += _T("<w:numStart ");
					sResult += m_oNumStart->ToString();
					sResult += _T("/>");
				}

				if ( m_oPos.IsInit() )
				{
					sResult += _T("<w:pos ");
					sResult += m_oPos->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:footnotePr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_footnotePr;
			}
		public:

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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSchema::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<sl:schema sl:uri=\"") + m_sUri
					+ _T("\" sl:manifestLocation=\"") + m_sManifestLocation 
					+ _T("\" sl:schemaLocation=\"") + m_sShemaLocation
					+ _T("\" sl:schemaLanguage=\"") + m_sShemaLanguage + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_sl_schema;
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
					if ( _T("sl:uri") == wsName ) 
						m_sUri = oReader.GetText();
					else if ( _T("sl:schemaLocation") == wsName ) 
						m_sShemaLocation = oReader.GetText();
					else if ( _T("sl:schemaLanguage") == wsName ) 
						m_sShemaLanguage = oReader.GetText();
					else if ( _T("sl:manifestLocation") == wsName ) 
						m_sManifestLocation = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSchemaLibrary::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("sl:schema") == sName )
					{
						OOX::Settings::CSchema *oSchema = new OOX::Settings::CSchema(oReader);
						if (oSchema) m_arrSchema.push_back( oSchema );
					}
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<sl:schemaLibrary>");

				for ( unsigned int nIndex = 0; nIndex < m_arrSchema.size(); nIndex++ )
				{
					if (m_arrSchema[nIndex])
						sResult += m_arrSchema[nIndex]->toXML();
				}

				sResult += _T("</sl:schemaLibrary>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_sl_schemaLibrary;
			}


		public:

			// Childs
			std::vector<OOX::Settings::CSchema*> m_arrSchema;
		};
	} // Settings
} // OOX

namespace OOX
{
	namespace Settings
	{
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

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{			
				m_eType = et_Unknown;
				std::wstring sName = oReader.GetName();

				if ( _T("w:hdrShapeDefaults") == sName )
					m_eType = et_w_hdrShapeDefaults;
				else if ( _T("w:shapeDefaults") == sName )
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
							if ( _T("o:bottom") == sName )
								pItem = new OOX::VmlOffice::CStrokeChild( oReader );

							break;

						case 'c':
							if ( _T("o:callout") == sName )
								pItem = new OOX::VmlOffice::CCallout( oReader );
							else if ( _T("o:clippath") == sName )
								pItem = new OOX::VmlOffice::CClipPath( oReader );
							else if ( _T("o:column") == sName )
								pItem = new OOX::VmlOffice::CStrokeChild( oReader );
							else if ( _T("o:complex") == sName )
								pItem = new OOX::VmlOffice::CComplex( oReader );

							break;

						case 'd':
							if ( _T("o:diagram") == sName )
								pItem = new OOX::VmlOffice::CDiagram( oReader );

							break;

						case 'e':
							if ( _T("o:equationxml") == sName )
								pItem = new OOX::VmlOffice::CEquationXml( oReader );
							else if ( _T("o:extrusion") == sName )
								pItem = new OOX::VmlOffice::CExtrusion( oReader );

							break;

						case 'f':
							if ( _T("o:fill") == sName )
								pItem = new OOX::VmlOffice::CFill( oReader );

							break;

						case 'i':
							if ( _T("o:ink") == sName )
								pItem = new OOX::VmlOffice::CInk( oReader );

							break;

						case 'l':
							if ( _T("o:left") == sName )
								pItem = new OOX::VmlOffice::CStrokeChild( oReader );
							else if ( _T("o:lock") == sName )
								pItem = new OOX::VmlOffice::CLock( oReader );

							break;

						case 'O':
							if ( _T("o:OLEObject") == sName )
								pItem = new OOX::VmlOffice::COLEObject( oReader );

							break;

						case 'r':
							if ( _T("o:right") == sName )
								pItem = new OOX::VmlOffice::CStrokeChild( oReader );

							break;

						case 's':
							if ( _T("o:shapedefaults") == sName )
								pItem = new OOX::VmlOffice::CShapeDefaults( oReader );
							else if ( _T("o:shapelayout") == sName )
								pItem = new OOX::VmlOffice::CShapeLayout( oReader );
							else if ( _T("o:signatureline") == sName )
								pItem = new OOX::VmlOffice::CSignatureLine( oReader );
							else if ( _T("o:skew") == sName )
								pItem = new OOX::VmlOffice::CSkew( oReader );

							break;

						case 't':
							if ( _T("o:top") == sName )
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
					sResult = _T("<w:hdrShapeDefaults>");
				else if ( et_w_shapeDefaults == m_eType )
					sResult = _T("<w:shapeDefaults>");
				else
					return _T("");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				if ( et_w_hdrShapeDefaults == m_eType )
					sResult += _T("</w:hdrShapeDefaults>");
				else if ( et_w_shapeDefaults == m_eType )
					sResult += _T("</w:shapeDefaults>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return m_eType;
			}


		public:

			EElementType m_eType;

			// Childs

		};
	} // Settings
} // OOX


namespace OOX
{
	//--------------------------------------------------------------------------------
	// CSettings 17.11.15.1.78
	//--------------------------------------------------------------------------------	
	class CSettings : public OOX::File
	{
	public:
		CSettings(OOX::Document *pMain) : OOX::File(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx) docx->m_pSettings = this;			
		}
		CSettings(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx) docx->m_pSettings = this;			

			read( oPath );
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

	public:
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = oReader.GetName();
			if ( _T("w:settings") == sName && !oReader.IsEmptyNode() )
			{
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
							if      ( _T("w:activeWritingStyle")         == sName ) m_oActiveWritingStyle         = oReader;
							else if ( _T("w:alignBordersAndEdges")       == sName ) m_oAlignBordersAndEdges       = oReader;
							else if ( _T("w:alwaysMergeEmptyNamespace")  == sName ) m_oAlwaysMergeEmptyNamespace  = oReader;
							else if ( _T("w:alwaysShowPlaceholderText")  == sName ) m_oAlwaysShowPlaceholderText  = oReader;
							else if ( _T("w:attachedSchema")             == sName ) m_oAttachedSchema             = oReader;
							else if ( _T("w:attachedTemplate")           == sName ) m_oAttachedTemplate           = oReader;
							else if ( _T("w:autoFormatOverride")         == sName ) m_oAutoFormatOverride         = oReader;
							else if ( _T("w:autoHyphenation")            == sName ) m_oAutoHyphenation            = oReader;
							break;
						case 'b':
							if      ( _T("w:bookFoldPrinting")           == sName ) m_oBookFoldPrinting           = oReader;
							else if ( _T("w:bookFoldPrintingSheets")     == sName ) m_oBookFoldPrintingSheets     = oReader;
							else if ( _T("w:bookFoldRevPrinting")        == sName ) m_oBookFoldRevPrinting        = oReader;
							else if ( _T("w:bordersDoNotSurroundFooter") == sName ) m_oBordersDoNotSurroundFooter = oReader;
							else if ( _T("w:bordersDoNotSurroundHeader") == sName ) m_oBordersDoNotSurroundHeader = oReader;
							break;
						case 'c':
							if      ( _T("w:captions")                   == sName ) m_oCaptions                   = oReader;
							else if ( _T("w:characterSpacingControl")    == sName ) m_oCharacterSpacingControl    = oReader;
							else if ( _T("w:clickAndTypeStyle")          == sName ) m_oClickAndTypeStyle          = oReader;
							else if ( _T("w:clrSchemeMapping")           == sName ) m_oClrSchemeMapping           = oReader;
							else if ( _T("w:compat")                     == sName ) m_oCompat                     = oReader;
							else if ( _T("w:consecutiveHyphenLimit")     == sName ) m_oConsecutiveHyphenLimit     = oReader;
							break;
						case 'd':
							if      ( _T("w:decimalSymbol")              == sName ) m_oDecimalSymbol              = oReader;
							else if ( _T("w:defaultTableStyle")          == sName ) m_oDefaultTableStyle          = oReader;
							else if ( _T("w:defaultTabStop")             == sName ) m_oDefaultTabStop             = oReader;
							else if ( _T("w:displayBackgroundShape")     == sName ) m_oDisplayBackgroundShape     = oReader;
							else if ( _T("w:displayHorizontalDrawingGridEvery") == sName ) m_oDisplayHorizontalDrawingGridEvery = oReader;
							else if ( _T("w:displayVerticalDrawingGridEvery")   == sName ) m_oDisplayVerticalDrawingGridEvery   = oReader;
							else if ( _T("w:documentProtection")         == sName ) m_oDocumentProtection         = oReader;
							else if ( _T("w:documentType")               == sName ) m_oDocumentType               = oReader;
							else if ( _T("w:docVars")                    == sName ) m_oDocVars                    = oReader;
							else if ( _T("w:doNotAutoCompressPictures")  == sName ) m_oDoNotAutoCompressPictures  = oReader;
							else if ( _T("w:doNotDemarcateInvalidXml")   == sName ) m_oDoNotDemarcateInvalidXml   = oReader;
							else if ( _T("w:doNotDisplayPageBoundaries") == sName ) m_oDoNotDisplayPageBoundaries = oReader;
							else if ( _T("w:doNotEmbedSmartTags")        == sName ) m_oDoNotEmbedSmartTags        = oReader;
							else if ( _T("w:doNotHyphenateCaps")         == sName ) m_oDoNotHyphenateCaps         = oReader;
							else if ( _T("w:doNotIncludeSubdocsInStats") == sName ) m_oDoNotIncludeSubdocsInStats = oReader;
							else if ( _T("w:doNotShadeFormData")         == sName ) m_oDoNotShadeFormData         = oReader;
							else if ( _T("w:doNotTrackFormatting")       == sName ) m_oDoNotTrackFormatting       = oReader;
							else if ( _T("w:doNotTrackMoves")            == sName ) m_oDoNotTrackMoves            = oReader;
							else if ( _T("w:doNotUseMarginsForDrawingGridOrigin") == sName ) m_oDoNotUseMarginsForDrawingGridOrigin = oReader;
							else if ( _T("w:doNotValidateAgainstSchema") == sName ) m_oDoNotValidateAgainstSchema = oReader;
							else if ( _T("w:drawingGridHorizontalOrigin")  == sName ) m_oDrawingGridHorizontalOrigin  = oReader;
							else if ( _T("w:drawingGridHorizontalSpacing") == sName ) m_oDrawingGridHorizontalSpacing = oReader;
							else if ( _T("w:drawingGridVerticalOrigin")  == sName ) m_oDrawingGridVerticalOrigin  = oReader;
							else if ( _T("w:drawingGridVerticalSpacing") == sName ) m_oDrawingGridVerticalSpacing = oReader;
							break;
						case 'e':
							if      ( _T("w:embedSystemFonts")           == sName ) m_oEmbedSystemFonts           = oReader;
							else if ( _T("w:embedTrueTypeFonts")         == sName ) m_oEmbedTrueTypeFonts         = oReader;
							else if ( _T("w:endnotePr")                  == sName ) m_oEndnotePr                  = oReader;
							else if ( _T("w:evenAndOddHeaders")          == sName ) m_oEvenAndOddHeaders          = oReader;
							break;
						case 'f':
							if      ( _T("w:footnotePr")                 == sName ) m_oFootnotePr                 = oReader;
							else if ( _T("w:forceUpgrade")               == sName ) m_oForceUpgrade               = oReader;
							else if ( _T("w:formsDesign")                == sName ) m_oFormsDesign                = oReader;
							break;
						case 'g':
							if      ( _T("w:gutterAtTop")                == sName ) m_oGutterAtTop                = oReader;
							break;
						case 'h':
							if      ( _T("w:hdrShapeDefaults")           == sName ) m_oHdrShapeDefaults           = oReader;
							else if ( _T("w:hideGrammaticalErrors")      == sName ) m_oHideGrammaticalErrors      = oReader;
							else if ( _T("w:hideSpellingErrors")         == sName ) m_oHideSpellingErrors         = oReader;
							else if ( _T("w:hyphenationZone")            == sName ) m_oHyphenationZone            = oReader;
							break;
						case 'i':
							if      ( _T("w:ignoreMixedContent")         == sName ) m_oIgnoreMixedContent         = oReader;
							break;
						case 'l':
							if      ( _T("w:linkStyles")                 == sName ) m_oLinkStyles                 = oReader;
							else if ( _T("w:listSeparator")              == sName ) m_oListSeparator              = oReader;
							break;
						case 'm':
							if      ( _T("w:mirrorMargins")              == sName ) m_oMirrorMargins              = oReader;
							break;
						case 'n':
							if      ( _T("w:noLineBreaksAfter")          == sName ) m_oNoLineBreaksAfter          = oReader;
							else if ( _T("w:noLineBreaksBefore")         == sName ) m_oNoLineBreaksBefore         = oReader;
							else if ( _T("w:noPunctuationKerning")       == sName ) m_oNoPunctuationKerning       = oReader;
							break;
						case 'p':
							if      ( _T("w:printFormsData")             == sName ) m_oPrintFormsData             = oReader;
							else if ( _T("w:printFractionalCharacterWidth") == sName ) m_oPrintFractionalCharacterWidth = oReader;
							else if ( _T("w:printPostScriptOverText")    == sName ) m_oPrintPostScriptOverText    = oReader;
							else if ( _T("w:printTwoOnOne")              == sName ) m_oPrintTwoOnOne              = oReader;
							else if ( _T("w:proofState")                 == sName ) m_oProofState                 = oReader;
							break;
						case 'r':
							if      ( _T("w:readModeInkLockDown")        == sName ) m_oReadModeInkLockDown        = oReader;
							else if ( _T("w:removeDateAndTime")          == sName ) m_oRemoveDateAndTime          = oReader;
							else if ( _T("w:removePersonalInformation")  == sName ) m_oRemovePersonalInformation  = oReader;
							else if ( _T("w:revisionView")               == sName ) m_oRevisionView               = oReader;
							else if ( _T("w:rsids")                      == sName ) m_oRsids                      = oReader;
							break;
						case 's':
							if      ( _T("w:saveFormsData")              == sName ) m_oSaveFormsData              = oReader;
							else if ( _T("w:saveInvalidXml")             == sName ) m_oSaveInvalidXml             = oReader;
							else if ( _T("w:savePreviewPicture")         == sName ) m_oSavePreviewPicture         = oReader;
							else if ( _T("w:saveSubsetFonts")            == sName ) m_oSaveSubsetFonts            = oReader;
							else if ( _T("w:saveThroughXslt")            == sName ) m_oSaveThroughtXslt           = oReader;
							else if ( _T("w:saveXmlDataOnly")            == sName ) m_oSaveXmlDataOnly            = oReader;
							else if ( _T("w:shapeDefaults")              == sName ) m_oShapeDefaults              = oReader;
							else if ( _T("w:showEnvelope")               == sName ) m_oShowEnvelope               = oReader;
							else if ( _T("w:showXMLTags")                == sName ) m_oShowXMLTags                = oReader;
							else if ( _T("w:smartTagType")               == sName )
							{
								OOX::Settings::CSmartTagType *oSTT = new OOX::Settings::CSmartTagType(oReader);
								if (oSTT) m_arrSmartTagType.push_back( oSTT );
							}
							else if ( _T("w:strictFirstAndLastChars")    == sName ) m_oStrictFirstAndLastChars    = oReader;
							else if ( _T("w:styleLockQFSet")             == sName ) m_oStyleLockQFSet             = oReader;
							else if ( _T("w:styleLockTheme")             == sName ) m_oStyleLockTheme             = oReader;
							else if ( _T("w:stylePaneFormatFilter")      == sName ) m_oStylePaneFormatFilter      = oReader;
							else if ( _T("w:stylePaneSortMethod")        == sName ) m_oStylePaneSortMethod        = oReader;
							else if ( _T("w:summaryLength")              == sName ) m_oSummaryLength              = oReader;
							break;
						case 't':
							if      ( _T("w:themeFontLang")              == sName ) m_oThemeFontLang              = oReader;
							else if ( _T("w:trackRevisions")             == sName ) m_oTrackRevisions             = oReader;
							break;
						case 'u':
							if      ( _T("w:updateFields")               == sName ) m_oUpdateFields               = oReader;
							else if ( _T("w:useXSLTWhenSaving")          == sName ) m_oUseXSLTWhenSaving          = oReader;
							break;
						case 'v':
							if      ( _T("w:view")                       == sName ) m_oView                       = oReader;
							break;
						case 'w':
							if      ( _T("w:writeProtection")            == sName ) m_oWriteProtection            = oReader;
							break;
						case 'z':
							if      ( _T("w:zoom")                       == sName ) m_oZoom                       = oReader;
							break;
						}
					}
					else if ( 's' == wChar0 )
					{
						if ( _T("sl:schemaLibrary") == sName )
							m_oSchemaLibrary = oReader;
					}
					else if ( 'm' == wChar0 )
					{
						if ( _T("m:mathPr") == sName )
							m_oMathPr = oReader;
					}
				}
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
            std::wstring sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:settings xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:sl=\"http://schemas.openxmlformats.org/schemaLibrary/2006/main\" mc:Ignorable=\"w14 w15\">");

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
				sXml += _T("<w:removePersonalInformation ") + m_oRemovePersonalInformation->ToString() + _T("/>");

			// 5
			if ( m_oRemoveDateAndTime.IsInit() )
				sXml += _T("<w:removeDateAndTime ") + m_oRemoveDateAndTime->ToString() + _T("/>");

			// 6
			if ( m_oDoNotDisplayPageBoundaries.IsInit() )
				sXml += _T("<w:doNotDisplayPageBoundaries ") + m_oDoNotDisplayPageBoundaries->ToString() + _T("/>");

			// 7
			if ( m_oDisplayBackgroundShape.IsInit() )
				sXml += _T("<w:displayBackgroundShape ") + m_oDisplayBackgroundShape->ToString() + _T("/>");

			// 8
			if ( m_oPrintPostScriptOverText.IsInit() )
				sXml += _T("<w:printPostScriptOverText ") + m_oPrintPostScriptOverText->ToString() + _T("/>");

			// 9
			if ( m_oPrintFractionalCharacterWidth.IsInit() )
				sXml += _T("<w:printFractionalCharacterWidth ") + m_oPrintFractionalCharacterWidth->ToString() + _T("/>");

			// 10
			if ( m_oPrintFormsData.IsInit() )
				sXml += _T("<w:printFormsData ") + m_oPrintFormsData->ToString() + _T("/>");

			// 11
			if ( m_oEmbedTrueTypeFonts.IsInit() )
				sXml += _T("<w:embedTrueTypeFonts ") + m_oEmbedTrueTypeFonts->ToString() + _T("/>");

			// 12
			if ( m_oEmbedSystemFonts.IsInit() )
				sXml += _T("<w:embedSystemFonts ") + m_oEmbedSystemFonts->ToString() + _T("/>");

			// 13
			if ( m_oSaveSubsetFonts.IsInit() )
				sXml += _T("<w:saveSubsetFonts ") + m_oSaveSubsetFonts->ToString() + _T("/>");

			// 14
			if ( m_oSaveFormsData.IsInit() )
				sXml += _T("<w:saveFormsData ") + m_oSaveFormsData->ToString() + _T("/>");

			// 15
			if ( m_oMirrorMargins.IsInit() )
				sXml += _T("<w:mirrorMargins ") + m_oMirrorMargins->ToString() + _T("/>");

			// 16
			if ( m_oAlignBordersAndEdges.IsInit() )
				sXml += _T("<w:alignBordersAndEdges ") + m_oAlignBordersAndEdges->ToString() + _T("/>");

			if ( m_oBordersDoNotSurroundHeader.IsInit() )
				sXml += _T("<w:bordersDoNotSurroundHeader ") + m_oBordersDoNotSurroundHeader->ToString() + _T("/>");

				sXml += _T("<w:bordersDoNotSurroundFooter ") + m_oBordersDoNotSurroundFooter->ToString() + _T("/>");

			if ( m_oGutterAtTop.IsInit() )
				sXml += _T("<w:gutterAtTop ") + m_oGutterAtTop->ToString() + _T("/>");

			if ( m_oHideSpellingErrors.IsInit() )
				sXml += _T("<w:hideSpellingErrors ") + m_oHideSpellingErrors->ToString() + _T("/>");

			if ( m_oHideGrammaticalErrors.IsInit() )
				sXml += _T("<w:hideGrammaticalErrors ") + m_oHideGrammaticalErrors->ToString() + _T("/>");

			if ( m_oActiveWritingStyle.IsInit() )
				sXml += m_oActiveWritingStyle->toXML();

			if ( m_oProofState.IsInit() )
				sXml += m_oProofState->toXML();

			if ( m_oFormsDesign.IsInit() )
				sXml += _T("<w:formsDesign ") + m_oFormsDesign->ToString() + _T("/>");

			if ( m_oAttachedTemplate.IsInit() )
				sXml += _T("<w:attachedTemplate ") + m_oAttachedTemplate->ToString() + _T("/>");

			if ( m_oLinkStyles.IsInit() )
				sXml += _T("<w:linkStyles ") + m_oLinkStyles->ToString() + _T("/>");

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
				sXml += _T("<w:trackRevisions ") + m_oTrackRevisions->ToString() + _T("/>");

			if ( m_oDoNotTrackMoves.IsInit() )
				sXml += _T("<w:doNotTrackMoves ") + m_oDoNotTrackMoves->ToString() + _T("/>");

			if ( m_oDoNotTrackFormatting.IsInit() )
				sXml += _T("<w:doNotTrackFormatting ") + m_oDoNotTrackFormatting->ToString() + _T("/>");

			if ( m_oDocumentProtection.IsInit() )
				sXml += m_oDocumentProtection->toXML();

			if ( m_oAutoFormatOverride.IsInit() )
				sXml += _T("<w:autoFormatOverride ") + m_oAutoFormatOverride->ToString() + _T("/>");

			if ( m_oStyleLockTheme.IsInit() )
				sXml += _T("<w:styleLockTheme ") + m_oStyleLockTheme->ToString() + _T("/>");

			if ( m_oStyleLockQFSet.IsInit() )
				sXml += _T("<w:styleLockQFSet ") + m_oStyleLockQFSet->ToString() + _T("/>");

			if ( m_oDefaultTabStop.IsInit() )
				sXml += _T("<w:defaultTabStop ") + m_oDefaultTabStop->ToString() + _T("/>");

			if ( m_oAutoHyphenation.IsInit() )
				sXml += _T("<w:autoHyphenation ") + m_oAutoHyphenation->ToString() + _T("/>");

			if ( m_oConsecutiveHyphenLimit.IsInit() )
				sXml += _T("<w:consecutiveHyphenLimit ") + m_oConsecutiveHyphenLimit->ToString() + _T("/>");

			if ( m_oHyphenationZone.IsInit() )
				sXml += _T("<w:hyphenationZone ") + m_oHyphenationZone->ToString() + _T("/>");

			if ( m_oDoNotHyphenateCaps.IsInit() )
				sXml += _T("<w:doNotHyphenateCaps ") + m_oDoNotHyphenateCaps->ToString() + _T("/>");

			if ( m_oShowEnvelope.IsInit() )
				sXml += _T("<w:showEnvelope ") + m_oShowEnvelope->ToString() + _T("/>");

			if ( m_oSummaryLength.IsInit() )
				sXml += _T("<w:summaryLength ") + m_oSummaryLength->ToString() + _T("/>");

			if ( m_oClickAndTypeStyle.IsInit() )
				sXml += _T("<w:clickAndTypeStyle ") + m_oClickAndTypeStyle->ToString() + _T("/>");

			if ( m_oDefaultTableStyle.IsInit() )
				sXml += _T("<w:defaultTableStyle ") + m_oDefaultTableStyle->ToString() + _T("/>");

			if ( m_oEvenAndOddHeaders.IsInit() )
				sXml += _T("<w:evenAndOddHeaders ") + m_oEvenAndOddHeaders->ToString() + _T("/>");

			if ( m_oBookFoldRevPrinting.IsInit() )
				sXml += _T("<w:bookFoldRevPrinting ") + m_oBookFoldRevPrinting->ToString() + _T("/>");

			if ( m_oBookFoldPrinting.IsInit() )
				sXml += _T("<w:bookFoldPrinting ") + m_oBookFoldPrinting->ToString() + _T("/>");

			if ( m_oBookFoldPrintingSheets.IsInit() )
				sXml += _T("<w:bookFoldPrintingSheets ") + m_oBookFoldPrintingSheets->ToString() + _T("/>");

			if ( m_oDrawingGridHorizontalSpacing.IsInit() )
				sXml += _T("<w:drawingGridHorizontalSpacing ") + m_oDrawingGridHorizontalSpacing->ToString() + _T("/>");

			if ( m_oDrawingGridVerticalSpacing.IsInit() )
				sXml += _T("<w:drawingGridVerticalSpacing ") + m_oDrawingGridVerticalSpacing->ToString() + _T("/>");

			if ( m_oDisplayHorizontalDrawingGridEvery.IsInit() )
				sXml += _T("<w:displayHorizontalDrawingGridEvery ") + m_oDisplayHorizontalDrawingGridEvery->ToString() + _T("/>");

			if ( m_oDisplayVerticalDrawingGridEvery.IsInit() )
				sXml += _T("<w:displayVerticalDrawingGridEvery ") + m_oDisplayVerticalDrawingGridEvery->ToString() + _T("/>");

			if ( m_oDoNotUseMarginsForDrawingGridOrigin.IsInit() )
				sXml += _T("<w:doNotUseMarginsForDrawingGridOrigin ") + m_oDoNotUseMarginsForDrawingGridOrigin->ToString() + _T("/>");

			if ( m_oDrawingGridHorizontalOrigin.IsInit() )
				sXml += _T("<w:drawingGridHorizontalOrigin ") + m_oDrawingGridHorizontalOrigin->ToString() + _T("/>");

			if ( m_oDrawingGridVerticalOrigin.IsInit() )
				sXml += _T("<w:drawingGridVerticalOrigin ") + m_oDrawingGridVerticalOrigin->ToString() + _T("/>");

			if ( m_oDoNotShadeFormData.IsInit() )
				sXml += _T("<w:doNotShadeFormData ") + m_oDoNotShadeFormData->ToString() + _T("/>");

			if ( m_oNoPunctuationKerning.IsInit() )
				sXml += _T("<w:noPunctuationKerning ") + m_oNoPunctuationKerning->ToString() + _T("/>");

			if ( m_oCharacterSpacingControl.IsInit() )
				sXml += m_oCharacterSpacingControl->toXML();

			if ( m_oPrintTwoOnOne.IsInit() )
				sXml += _T("<w:printTwoOnOne ") + m_oPrintTwoOnOne->ToString() + _T("/>");

			if ( m_oStrictFirstAndLastChars.IsInit() )
				sXml += _T("<w:strictFirstAndLastChars ") + m_oStrictFirstAndLastChars->ToString() + _T("/>");

			// 64
			if ( m_oNoLineBreaksAfter.IsInit() )
				sXml += m_oNoLineBreaksAfter->toXML();

			if ( m_oNoLineBreaksBefore.IsInit() )
				sXml += m_oNoLineBreaksBefore->toXML();

			if ( m_oSavePreviewPicture.IsInit() )
				sXml += _T("<w:savePreviewPicture ") + m_oSavePreviewPicture->ToString() + _T("/>");

			if ( m_oDoNotValidateAgainstSchema.IsInit() )
				sXml += _T("<w:doNotValidateAgainstSchema ") + m_oDoNotValidateAgainstSchema->ToString() + _T("/>");

			if ( m_oSaveInvalidXml.IsInit() )
				sXml += _T("<w:saveInvalidXml ") + m_oSaveInvalidXml->ToString() + _T("/>");

			if ( m_oIgnoreMixedContent.IsInit() )
				sXml += _T("<w:ignoreMixedContent ") + m_oIgnoreMixedContent->ToString() + _T("/>");

			if ( m_oAlwaysShowPlaceholderText.IsInit() )
				sXml += _T("<w:alwaysShowPlaceholderText ") + m_oAlwaysShowPlaceholderText->ToString() + _T("/>");

			if ( m_oDoNotDemarcateInvalidXml.IsInit() )
				sXml += _T("<w:doNotDemarcateInvalidXml ") + m_oDoNotDemarcateInvalidXml->ToString() + _T("/>");

			if ( m_oSaveXmlDataOnly.IsInit() )
				sXml += _T("<w:saveXmlDataOnly ") + m_oSaveXmlDataOnly->ToString() + _T("/>");

			if ( m_oUseXSLTWhenSaving.IsInit() )
				sXml += _T("<w:useXSLTWhenSaving ") + m_oUseXSLTWhenSaving->ToString() + _T("/>");

			if ( m_oSaveThroughtXslt.IsInit() )
				sXml += m_oSaveThroughtXslt->toXML();

			if ( m_oShowXMLTags.IsInit() )
				sXml += _T("<w:showXMLTags ") + m_oShowXMLTags->ToString() + _T("/>");

			if ( m_oAlwaysMergeEmptyNamespace.IsInit() )
				sXml += _T("<w:alwaysMergeEmptyNamespace ") + m_oAlwaysMergeEmptyNamespace->ToString() + _T("/>");

			if ( m_oUpdateFields.IsInit() )
				sXml += _T("<w:updateFields ") + m_oUpdateFields->ToString() + _T("/>");

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
				sXml += _T("<w:attachedSchema ") + m_oAttachedSchema->ToString() + _T("/>");

			if ( m_oThemeFontLang.IsInit() )
				sXml += _T("<w:themeFontLang ") + m_oThemeFontLang->ToString() + _T("/>");

			if ( m_oClrSchemeMapping.IsInit() )
				sXml += m_oClrSchemeMapping->toXML();

			if ( m_oDoNotIncludeSubdocsInStats.IsInit() )
				sXml += _T("<w:doNotIncludeSubdocsInStats ") + m_oDoNotIncludeSubdocsInStats->ToString() + _T("/>");

			if ( m_oDoNotAutoCompressPictures.IsInit() )
				sXml += _T("<w:doNotAutoCompressPictures ") + m_oDoNotAutoCompressPictures->ToString() + _T("/>");

			if ( m_oForceUpgrade.IsInit() )
				sXml += _T("<<w:forceUpgrade/>");

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
				sXml += _T("<w:doNotEmbedSmartTags ") + m_oDoNotEmbedSmartTags->ToString() + _T("/>");

			if ( m_oDecimalSymbol.IsInit() )
				sXml += _T("<w:decimalSymbol ") + m_oDecimalSymbol->ToString() + _T("/>");

			// 99
			if ( m_oListSeparator.IsInit() )
				sXml += _T("<w:listSeparator ") + m_oListSeparator->ToString() + _T("/>");

			sXml += _T("</w:settings>");
			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
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
			if ( _T("w:settings") == sName && !oReader.IsEmptyNode() )
			{
				int nStylesDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("w:SdtGlobalColor") == sName )
						m_oSdtGlobalColor = oReader;
					else if ( _T("w:SdtGlobalShowHighlight") == sName )
						m_oSdtGlobalShowHighlight = oReader;
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
			sXml += L"</w:settings>";
			return sXml;
		}
		bool IsEmpty()
		{
			return !(m_oSdtGlobalColor.IsInit() || m_oSdtGlobalShowHighlight.IsInit());
		}
		nullable<ComplexTypes::Word::CColor> m_oSdtGlobalColor;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSdtGlobalShowHighlight;
	};
} // namespace OOX

#endif /* OOX_SETTINGS_INCLUDE_H_ */
