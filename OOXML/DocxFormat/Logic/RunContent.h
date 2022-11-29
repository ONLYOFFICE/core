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

#include "../../DocxFormat/WritingElement.h"
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"
#include "../../PPTXFormat/Logic/Xfrm.h"

// 2. Класс CRuby   17.3.3.25

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CBr 17.3.3.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBr)
			CBr(OOX::Document *pMain = NULL);
			virtual ~CBr();

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			SimpleTypes::CBrClear m_oClear;
			SimpleTypes::CBrType m_oType;

		};

		//--------------------------------------------------------------------------------
		// CContentPart 17.3.3.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CContentPart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CContentPart)
			CContentPart(OOX::Document *pMain = NULL);
			virtual ~CContentPart();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::wstring							m_namespace;
			nullable<PPTX::Logic::Xfrm>				m_oXfrm;
			//nullable<nvContentPartPr>				m_oNvContentPartPr;
			nullable<SimpleTypes::CRelationshipId > m_oId;

		};

		//--------------------------------------------------------------------------------
		// CCr 17.3.3.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCr)
			CCr(OOX::Document *pMain = NULL);
			virtual ~CCr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CDayLong 17.3.3.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDayLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDayLong)
			CDayLong(OOX::Document *pMain = NULL);
			virtual ~CDayLong();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CDayShort 17.3.3.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDayShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDayShort)
			CDayShort(OOX::Document *pMain = NULL);
			virtual ~CDayShort();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CDelText 17.3.3.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDelText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelText)
			CDelText(OOX::Document *pMain = NULL);
			virtual ~CDelText();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CXmlSpace> m_oSpace;

			// Value
            std::wstring                             m_sText;

		};

		//--------------------------------------------------------------------------------
		// CLastRenderedPageBreak 17.3.3.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CLastRenderedPageBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLastRenderedPageBreak)
			CLastRenderedPageBreak(OOX::Document *pMain = NULL);
			virtual ~CLastRenderedPageBreak();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CMonthLong 17.3.3.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class CMonthLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMonthLong)
			CMonthLong(OOX::Document *pMain = NULL);
			virtual ~CMonthLong();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CMonthShort 17.3.3.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CMonthShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMonthShort)
			CMonthShort(OOX::Document *pMain = NULL);
			virtual ~CMonthShort();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CNoBreakHyphen 17.3.3.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CNoBreakHyphen : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNoBreakHyphen)
			CNoBreakHyphen(OOX::Document *pMain = NULL);
			virtual ~CNoBreakHyphen();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CPgNum 17.3.3.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPgNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPgNum)
			CPgNum(OOX::Document *pMain = NULL);
			virtual ~CPgNum();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CPTab 17.3.3.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPTab : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPTab)
			CPTab(OOX::Document *pMain = NULL);
			virtual ~CPTab();
			
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CPTabAlignment > m_oAlignment;
			nullable<SimpleTypes::CPTabLeader    > m_oLeader;
			nullable<SimpleTypes::CPTabRelativeTo> m_oRelativeTo;

		};

		//--------------------------------------------------------------------------------
		// CRuby 17.3.3.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CRuby : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRuby)
			CRuby(OOX::Document *pMain = NULL);
			virtual ~CRuby();

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CSoftHyphen 17.3.3.29 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSoftHyphen : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSoftHyphen)
			CSoftHyphen(OOX::Document *pMain = NULL);
			virtual ~CSoftHyphen();
			
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CSym 17.3.3.30 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSym : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSym)
			CSym(OOX::Document *pMain = NULL);
			virtual ~CSym();
			
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CShortHexNumber>		m_oChar;
			nullable<std::wstring>						m_oFont;

		};

		//--------------------------------------------------------------------------------
		// CText 17.3.3.31 (Part 1)
		//--------------------------------------------------------------------------------	
		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CText)
			CText(OOX::Document *pMain = NULL);
			virtual ~CText();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static void ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable<SimpleTypes::CXmlSpace>& oSpace);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CXmlSpace> m_oSpace;

			// Value
            std::wstring                             m_sText;
		};

		//--------------------------------------------------------------------------------
		// CTab 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------	
		class CTab : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTab)
			CTab(OOX::Document *pMain = NULL);
			virtual ~CTab();

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CYearLong 17.3.3.33 (Part 1)
		//--------------------------------------------------------------------------------	
		class CYearLong : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CYearLong)
			CYearLong(OOX::Document *pMain = NULL);
			virtual ~CYearLong();
			
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CYearShort 17.3.3.32 (Part 1)
		//--------------------------------------------------------------------------------	
		class CYearShort : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CYearShort)
			CYearShort(OOX::Document *pMain = NULL);
			virtual ~CYearShort();
			
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CAnnotationRef 17.13.4.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CAnnotationRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAnnotationRef)
			CAnnotationRef(OOX::Document *pMain = NULL);
			virtual ~CAnnotationRef();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CCommentReference 17.13.4.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCommentReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCommentReference)
			CCommentReference(OOX::Document *pMain = NULL);
			virtual ~CCommentReference();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oId;
		};

		//--------------------------------------------------------------------------------
		// CContinuationSeparator 17.11.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CContinuationSeparator : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CContinuationSeparator)
			CContinuationSeparator(OOX::Document *pMain = NULL);
			virtual ~CContinuationSeparator();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CDelInstrText 17.16.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDelInstrText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelInstrText)
			CDelInstrText(OOX::Document *pMain = NULL);
			virtual ~CDelInstrText();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CXmlSpace> m_oSpace;

			// Value
            std::wstring                             m_sText;

		};

		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEndnoteRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEndnoteRef)
			CEndnoteRef(OOX::Document *pMain = NULL);
			virtual ~CEndnoteRef();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEndnoteReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEndnoteReference)
			CEndnoteReference(OOX::Document *pMain = NULL);
			virtual ~CEndnoteReference();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>			m_oCustomMarkFollows;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;

		};

		//--------------------------------------------------------------------------------
		// CEndnoteRef 17.11.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFootnoteRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFootnoteRef)
			CFootnoteRef(OOX::Document *pMain = NULL);
			virtual ~CFootnoteRef();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CEndnoteReference 17.11.14 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFootnoteReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFootnoteReference)
			CFootnoteReference(OOX::Document *pMain = NULL);
			virtual ~CFootnoteReference();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>			m_oCustomMarkFollows;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
		};

		//--------------------------------------------------------------------------------
		// CInstrText 17.16.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CInstrText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInstrText)
			CInstrText(OOX::Document *pMain = NULL);
			virtual ~CInstrText();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CXmlSpace> m_oSpace;

			// Value
            std::wstring                             m_sText;

		};

		//--------------------------------------------------------------------------------
		// CSeparator 17.11.23 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSeparator : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSeparator)
			CSeparator(OOX::Document *pMain = NULL);
			virtual ~CSeparator();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

	} // Logic
} // OOX
