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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_activeWritingStyle;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
            nullable_string m_sAppName;
			nullable_bool m_oCheckSyle;
            nullable_string m_sDllVersion;
			nullable_string m_oLang;
			nullable_bool m_oNlCheck;
            nullable_string m_sVendorID;
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_autoCaption;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
            nullable_string m_sCaption;
            nullable_string m_sName;
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_autoCaptions;
			}

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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_caption;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_bool         m_oChapNum;
			nullable<SimpleTypes::CDecimalNumber<>> m_oHeading;
            std::wstring                                 m_sName;
			nullable_bool         m_oNoLabel;
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_captions;
			}

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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_characterSpacingControl;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_compatSetting;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
            nullable_string m_sName;
            nullable_string m_sUri;
            nullable_string m_sVal;
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_compat;
			}

			// TO DO: Добавить аттрибуты из 9.7.3 Part4

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
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_documentProtection;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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
            virtual std::wstring toXML() const
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
					sResult	= L"<w:noLineBreaksAfter w:lang=\"" + m_sLang + L"\" w:val=\"" + m_sVal + L"\"/>";
				else if ( et_w_noLineBreaksBefore == m_eType ) 
					sResult	= L"<w:noLineBreaksBefore w:lang=\"" + m_sLang + L"\" w:val=\"" + m_sVal + L"\"/>";

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
		public:
			EElementType m_eType;

			std::wstring m_sLang;
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
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_readModeInkLockDown;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_revisionView;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_bool m_oComments;
			nullable_bool m_oFormatting;
			nullable_bool m_oInkAnnotataions;
			nullable_bool m_oInsDel;
			nullable_bool m_oMarkup;
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

				ComplexTypes_WriteAttribute ( L"r:id=\"", m_rId );
				sResult += m_sSolutionID.ToAttribute(L"w:solutionID");

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
            nullable_string m_sSolutionID;
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

				sResult += m_sNameSpaceUri.ToAttribute( L"w:namespaceuri");
				sResult += m_sName.ToAttribute( L"w:name");
				sResult += m_sUrl.ToAttribute( L"w:url");

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
            nullable_string m_sName;
            nullable_string m_sNameSpaceUri;
            nullable_string m_sUrl;
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
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_stylePaneFormatFilter;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			// TO DO: Добавить атрибуты из 9.7.2.4 Part4

			nullable_bool m_oAllStyles;
			nullable_bool m_oAlternateStyleNames;
			nullable_bool m_oClearFormatting;
			nullable_bool m_oCustomStyles;
			nullable_bool m_oDirectFormattingOnNumbering;
			nullable_bool m_oDirectFormattingOnParagraphs;
			nullable_bool m_oDirectFormattingOnRuns;
			nullable_bool m_oDirectFormattingOnTables;
			nullable_bool m_oHeadingStyles;
			nullable_bool m_oLatentStyles;
			nullable_bool m_oNumberingStyles;
			nullable_bool m_oStylesInUse;
			nullable_bool m_oTableStyles;
			nullable_bool m_oTop3HeadingStyles;
			nullable_bool m_oVisibleStyles;
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
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_w_writeProtection;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_endnotePr;
			}
		public:
			std::vector<OOX::CFtnEdnSepRef*> m_arrEndnote;
			nullable<ComplexTypes::Word::CNumFmt> m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart> m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oNumStart;
			nullable<ComplexTypes::Word::CEdnPos> m_oPos;
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

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

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
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_sl_schema;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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
			CShapeDefaults(EElementType type)
			{
				m_eType = type;
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
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;
		virtual std::wstring toXML() const;
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

		void SetDefaults();
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
		void fromXML(const std::wstring& sXml);
		std::wstring toXML();
		bool IsEmpty()
		{
			return !(m_oSdtGlobalColor.IsInit() || m_oSdtGlobalShowHighlight.IsInit() || m_oSpecialFormsHighlight.IsInit());
		}
		
		nullable<ComplexTypes::Word::CColor> m_oSdtGlobalColor;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSdtGlobalShowHighlight;
		nullable<ComplexTypes::Word::CColor> m_oSpecialFormsHighlight;
	};
} // namespace OOX

