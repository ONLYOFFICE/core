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

#include "../WritingElement.h"

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/ComplexTypes.h"

#include "../../PPTXFormat/Logic/Ln.h"
#include "../../PPTXFormat/Logic/Effects/Reflection.h"
#include "../../PPTXFormat/Logic/Effects/Glow.h"
#include "../../PPTXFormat/Logic/Scene3d.h"
#include "../../PPTXFormat/Logic/Sp3d.h"

namespace OOX
{
	namespace Logic
	{
		class CRun;
		class CRunProperty;

		class CStylisticSets : public WritingElementWithChilds<ComplexTypes::Word::CStylisticSet>
		{
		public:
			WritingElement_AdditionConstructors(CStylisticSets)
			CStylisticSets(OOX::Document *pMain = NULL);
			virtual ~CStylisticSets();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// RPrChange 17.3.2.31 (Part 1)
		//--------------------------------------------------------------------------------	

		class CRPrChange : public WritingElement
		{
		public:
			CRPrChange();
			CRPrChange(XmlUtils::CXmlNode& oNode);
			CRPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CRPrChange();
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

            nullable<std::wstring>					m_sAuthor;
            nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
            nullable<std::wstring>					m_sUserId;

            nullable<CRunProperty>					m_pRunPr;
		};

		class CRunProperty : public WritingElement
		{
		public:
			CRunProperty();
			virtual ~CRunProperty();
			CRunProperty(const XmlUtils::CXmlNode &oNode);
			CRunProperty(const XmlUtils::CXmlLiteReader& oReader);

			const CRunProperty& operator=(const XmlUtils::CXmlNode &oNode);
			const CRunProperty& operator=(const XmlUtils::CXmlLiteReader& oReader);

			bool IsNoEmpty();
			void Clear();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXML(XmlUtils::CXmlLiteReader& oReader, CRun* pRun);
            virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual EElementType getType() const;

			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
			static const CRunProperty Merge(const CRunProperty& oPrev, const CRunProperty& oCurrent);

			bool m_bRPRChange = false; // Является ли данный w:rPr дочерним по отношению к w:rPrChange

			nullable<ComplexTypes::Word::COnOff2>	m_oBold;
			nullable<ComplexTypes::Word::COnOff2>	m_oBoldCs;
			nullable<ComplexTypes::Word::CBorder>							m_oBdr;
			nullable<ComplexTypes::Word::COnOff2>	m_oCaps;
			nullable<ComplexTypes::Word::CColor>							m_oColor;
			nullable<ComplexTypes::Word::COnOff2>	m_oCs;
			nullable<ComplexTypes::Word::CTrackChange>						m_oDel;
			nullable<ComplexTypes::Word::COnOff2>	m_oDStrike;
			nullable<ComplexTypes::Word::CEastAsianLayout>					m_oEastAsianLayout;
			nullable<ComplexTypes::Word::CTextEffect>						m_oEffect;
			nullable<ComplexTypes::Word::CEm>								m_oEm;
			nullable<ComplexTypes::Word::COnOff2>	m_oEmboss;
			nullable<ComplexTypes::Word::CFitText>							m_oFitText;
			nullable<ComplexTypes::Word::CHighlight>						m_oHighlight;
			nullable<ComplexTypes::Word::CTrackChange>						m_oIns;
			nullable<ComplexTypes::Word::COnOff2>	m_oItalic;
			nullable<ComplexTypes::Word::COnOff2>	m_oItalicCs;
			nullable<ComplexTypes::Word::COnOff2>	m_oImprint;
			nullable<ComplexTypes::Word::CHpsMeasure>						m_oKern;
			nullable<ComplexTypes::Word::CLanguage>							m_oLang;
			nullable<ComplexTypes::Word::COnOff2>	m_oNoProof;
			nullable<ComplexTypes::Word::COnOff2>	m_oMath;
			nullable<ComplexTypes::Word::CTrackChange>						m_oMoveFrom;
			nullable<ComplexTypes::Word::CTrackChange>						m_oMoveTo;
			nullable<ComplexTypes::Word::COnOff2>	m_oOutline;
			nullable<ComplexTypes::Word::CSignedHpsMeasure>					m_oPosition;
			nullable<ComplexTypes::Word::CFonts>							m_oRFonts;
			nullable<OOX::Logic::CRPrChange>								m_oRPrChange;
            nullable<ComplexTypes::Word::String>							m_oRStyle;
			nullable<ComplexTypes::Word::COnOff2>	m_oRtL;
			nullable<ComplexTypes::Word::COnOff2>	m_oShadow;
			nullable<ComplexTypes::Word::CShading>							m_oShd;
			nullable<ComplexTypes::Word::COnOff2>	m_oSmallCaps;
			nullable<ComplexTypes::Word::COnOff2>	m_oSnapToGrid;
			nullable<ComplexTypes::Word::CSignedTwipsMeasure>				m_oSpacing;
			nullable<ComplexTypes::Word::COnOff2>	m_oSpecVanish;
			nullable<ComplexTypes::Word::COnOff2>	m_oStrike;
			nullable<ComplexTypes::Word::CHpsMeasure>						m_oSz;
			nullable<ComplexTypes::Word::CHpsMeasure>						m_oSzCs;
			nullable<ComplexTypes::Word::CUnderline>						m_oU;
			nullable<ComplexTypes::Word::COnOff2>	m_oVanish;
			nullable<ComplexTypes::Word::CVerticalAlignRun>					m_oVertAlign;
			nullable<ComplexTypes::Word::CTextScale>						m_oW;
			nullable<ComplexTypes::Word::COnOff2>	m_oWebHidden;
	//ext		
			PPTX::Logic::UniFill											m_oTextFill;			
            nullable<PPTX::Logic::Ln>										m_oTextOutline;
			nullable<ComplexTypes::Word::CLigatures>						m_oLigatures;
			nullable<ComplexTypes::Word::CNumSpacing>						m_oNumSpacing;
			nullable<ComplexTypes::Word::CNumForm>							m_oNumForm;
			nullable<CStylisticSets>										m_oStylisticSets;
			nullable<ComplexTypes::Word::COnOff2>	m_oCntxtAlts;
			nullable<PPTX::Logic::Reflection>								m_oReflection;
			nullable<PPTX::Logic::Glow>										m_oGlow;
			nullable<PPTX::Logic::OuterShdw>								m_oShadowExt;
			nullable<PPTX::Logic::Sp3d>										m_oProps3d;
			nullable<PPTX::Logic::Scene3d>									m_oScene3d;

			OOX::WritingElement*											m_pText = NULL; //temp ... for Run object -> XpertdocOnlineDemoEn.docx
		};

	} // namespace Logic
} // namespace OOX

