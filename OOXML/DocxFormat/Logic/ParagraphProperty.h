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
#ifndef OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"

#include "./../WritingElement.h"

#include "RunProperty.h"
#include "SectionProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// NumPr 17.13.1.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumPr)

			CNumPr();
			virtual ~CNumPr();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CDecimalNumber > m_oIlvl;
			nullable<ComplexTypes::Word::CTrackChange   > m_oIns;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumID;

		};

		//--------------------------------------------------------------------------------
		// PBdr 17.13.1.24 (Part 1)
		//--------------------------------------------------------------------------------
		class CPBdr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPBdr)

			CPBdr();
			virtual ~CPBdr();

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;


			virtual EElementType getType() const;

			static const CPBdr Merge(const CPBdr& oPrev, const CPBdr& oCurrent);

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}

		public:
			nullable<ComplexTypes::Word::CBorder > m_oBar;
			nullable<ComplexTypes::Word::CBorder > m_oBetween;
			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oLeft;
			nullable<ComplexTypes::Word::CBorder > m_oRight;
			nullable<ComplexTypes::Word::CBorder > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// PPrChange 17.13.5.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CParagraphProperty;

		class CPPrChange : public WritingElement
		{
		public:
			CPPrChange();
			CPPrChange(XmlUtils::CXmlNode& oNode);
			CPPrChange(XmlUtils::CXmlLiteReader& oReader);
			const CPPrChange& operator =(const XmlUtils::CXmlNode &oNode);
			const CPPrChange& operator =(const XmlUtils::CXmlLiteReader& oReader);
			virtual ~CPPrChange();

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>						m_sAuthor;
			nullable<SimpleTypes::CDateTime>			m_oDate;
			nullable<SimpleTypes::CDecimalNumber>		m_oId;
			nullable<std::wstring>						m_sUserId;

			nullable<OOX::Logic::CParagraphProperty>	m_pParPr;
		};

		//--------------------------------------------------------------------------------
		// Tabs 17.3.1.38 (Part 1)
		//--------------------------------------------------------------------------------
		class CTabs : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTabs)

			CTabs(OOX::Document *pMain = NULL);
			virtual ~CTabs();
			CTabs(const CTabs& oSrc);

			const CTabs& operator =(const CTabs &oTabs);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			std::vector<ComplexTypes::Word::CTabStop*> m_arrTabs;
		};

		//--------------------------------------------------------------------------------
		// CParagraphProperty
		//--------------------------------------------------------------------------------
		class CParagraphProperty : public WritingElement
		{
		public:
			CParagraphProperty(OOX::Document *pMain = NULL);
			CParagraphProperty(XmlUtils::CXmlNode& oNode);
			CParagraphProperty(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CParagraphProperty();

			const CParagraphProperty& operator =(const XmlUtils::CXmlNode &oNode);
			const CParagraphProperty& operator =(const XmlUtils::CXmlLiteReader &oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CParagraphProperty Merge(const CParagraphProperty& oPrev, const CParagraphProperty& oCurrent);

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}

			//--------------------------------------------------------------------------------------------------------

			bool                                                           m_bPPrChange;

            nullable<ComplexTypes::Word::COnOff2>	m_oAdjustRightInd;
            nullable<ComplexTypes::Word::COnOff2>	m_oAutoSpaceDE;
            nullable<ComplexTypes::Word::COnOff2>	m_oAutoSpaceDN;
            nullable<ComplexTypes::Word::COnOff2>	m_oBidi;
			nullable<ComplexTypes::Word::CCnf>								m_oCnfStyle;
            nullable<ComplexTypes::Word::COnOff2>	m_oContextualSpacing;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oDivID;
			nullable<ComplexTypes::Word::CFramePr>							m_oFramePr;
			nullable<ComplexTypes::Word::CInd>								m_oInd;
			nullable<ComplexTypes::Word::CJc>								m_oJc;
            nullable<ComplexTypes::Word::COnOff2>	m_oKeepLines;
            nullable<ComplexTypes::Word::COnOff2>	m_oKeepNext;
            nullable<ComplexTypes::Word::COnOff2>	m_oKinsoku;
            nullable<ComplexTypes::Word::COnOff2>	m_oMirrorIndents;
			nullable<OOX::Logic::CNumPr>									m_oNumPr;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oOutlineLvl;
            nullable<ComplexTypes::Word::COnOff2>	m_oOverflowPunct;
            nullable<ComplexTypes::Word::COnOff2>	m_oPageBreakBefore;
			nullable<OOX::Logic::CPBdr>										m_oPBdr;
			nullable<OOX::Logic::CPPrChange>								m_oPPrChange;
			nullable<ComplexTypes::Word::String>							m_oPStyle;
			nullable<OOX::Logic::CRunProperty>								m_oRPr;
			nullable<OOX::Logic::CSectionProperty>							m_oSectPr;
			nullable<ComplexTypes::Word::CShading>							m_oShd;
            nullable<ComplexTypes::Word::COnOff2>	m_oSnapToGrid;
			nullable<ComplexTypes::Word::CSpacing>							m_oSpacing;
            nullable<ComplexTypes::Word::COnOff2>	m_oSuppressAutoHyphens;
            nullable<ComplexTypes::Word::COnOff2>	m_oSuppressLineNumbers;
            nullable<ComplexTypes::Word::COnOff2>	m_oSuppressOverlap;
			nullable<OOX::Logic::CTabs>										m_oTabs;
			nullable<ComplexTypes::Word::CTextAlignment>					m_oTextAlignment;
			nullable<ComplexTypes::Word::CTextboxTightWrap>					m_oTextboxTightWrap;
			nullable<ComplexTypes::Word::CTextDirection>					m_oTextDirection;
            nullable<ComplexTypes::Word::COnOff2>	m_oTopLinePunct;
            nullable<ComplexTypes::Word::COnOff2>	m_oWidowControl;
            nullable<ComplexTypes::Word::COnOff2>	m_oWordWrap;
		};

	} // namespace Logic
} // namespace OOX
#endif // OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_
