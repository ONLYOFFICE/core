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

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Column 17.6.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CColumn : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CColumn)
			CColumn();
			virtual ~CColumn();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTwipsMeasure > m_oSpace;
			nullable<SimpleTypes::CTwipsMeasure > m_oW;
		};

		//--------------------------------------------------------------------------------
		// DocGrid 17.6.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CDocGrid : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDocGrid)
			CDocGrid();
			virtual ~CDocGrid();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber>	m_oCharSpace;
			nullable<SimpleTypes::CDecimalNumber>	m_oLinePitch;
			nullable<SimpleTypes::CDocGrid>			m_oType;
		};

		//--------------------------------------------------------------------------------
		// HdrFtrRef 17.10.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CHdrFtrRef : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHdrFtrRef)
			CHdrFtrRef();
			virtual ~CHdrFtrRef();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId>		m_oId;
			nullable<SimpleTypes::CHdrFtr>				m_oType;
		};

		//--------------------------------------------------------------------------------
		// LineNumber 17.6.8 (Part 1)
		//--------------------------------------------------------------------------------
		class CLineNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLineNumber)
			CLineNumber();
			virtual ~CLineNumber();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber      > m_oCountBy;
			nullable<SimpleTypes::CTwipsMeasure       > m_oDistance;
			nullable<SimpleTypes::CLineNumberRestart  > m_oRestart;
			nullable<SimpleTypes::CDecimalNumber      > m_oStart;
		};

		//--------------------------------------------------------------------------------
		// PaperSource 17.6.9 (Part 1)
		//--------------------------------------------------------------------------------
		class CPaperSource : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPaperSource)
			CPaperSource();
			virtual ~CPaperSource();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oFirst;
			nullable<SimpleTypes::CDecimalNumber> m_oOther;
		};

		//--------------------------------------------------------------------------------
		// PageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageBorder : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageBorder)
			CPageBorder();
			virtual ~CPageBorder();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHexColor                       > m_oColor;
			nullable<SimpleTypes::COnOff                          > m_oFrame;
			nullable<SimpleTypes::CRelationshipId                 > m_oId;
			nullable<SimpleTypes::COnOff                          > m_oShadow;
			nullable<SimpleTypes::CPointMeasure                   > m_oSpace;
			nullable<SimpleTypes::CEighthPointMeasure             > m_oSz;
			nullable<SimpleTypes::CThemeColor                     > m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber                 > m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber                 > m_oThemeTint;
			nullable<SimpleTypes::CBorder                         > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// BottomPageBorder 17.6.2 (Part 1)
		//--------------------------------------------------------------------------------
		class CBottomPageBorder : public CPageBorder
		{
		public:
			ComplexTypes_AdditionConstructors(CBottomPageBorder)
			CBottomPageBorder();
			virtual ~CBottomPageBorder();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId                 > m_oBottomLeft;
			nullable<SimpleTypes::CRelationshipId                 > m_oBottomRight;
		};

		//--------------------------------------------------------------------------------
		// TopPageBorder 17.6.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CTopPageBorder : public CPageBorder
		{
		public:
			ComplexTypes_AdditionConstructors(CTopPageBorder)
			CTopPageBorder();
			virtual ~CTopPageBorder();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId                 > m_oTopLeft;
			nullable<SimpleTypes::CRelationshipId                 > m_oTopRight;
		};

		//--------------------------------------------------------------------------------
		// PageMar 17.6.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageMar : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageMar)
			CPageMar();
			virtual ~CPageMar();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CSignedTwipsMeasure> m_oBottom;
			nullable<SimpleTypes::CTwipsMeasure> m_oFooter;
			nullable<SimpleTypes::CTwipsMeasure> m_oGutter;
			nullable<SimpleTypes::CTwipsMeasure> m_oHeader;
			nullable<SimpleTypes::CTwipsMeasure> m_oLeft;
			nullable<SimpleTypes::CTwipsMeasure> m_oRight;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// PageNumber 17.6.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageNumber : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageNumber)
			CPageNumber();
			virtual ~CPageNumber();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CChapterSep      > m_oChapSep;
			nullable<SimpleTypes::CDecimalNumber   > m_oChapStyle;
			nullable<SimpleTypes::CNumberFormat    > m_oFmt;
			nullable<SimpleTypes::CDecimalNumber   > m_oStart;
		};

		//--------------------------------------------------------------------------------
		// PageSz 17.6.13 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageSz : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPageSz)
			CPageSz();
			virtual ~CPageSz();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber     > m_oCode;
			nullable<SimpleTypes::CTwipsMeasure      > m_oH;
			nullable<SimpleTypes::CPageOrientation   > m_oOrient;
			nullable<SimpleTypes::CTwipsMeasure      > m_oW;
		};

		//--------------------------------------------------------------------------------
		// SectType 17.6.22 (Part 1)
		//--------------------------------------------------------------------------------
		class CSectType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSectType)
			CSectType();
			virtual ~CSectType();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CSectionMark> m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Columns 17.6.4 (Part 1)
		//--------------------------------------------------------------------------------
		class CColumns : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColumns)
			CColumns();
			virtual ~CColumns();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff             > m_oEqualWidth;
			nullable<SimpleTypes::CDecimalNumber     > m_oNum;
			nullable<SimpleTypes::COnOff             > m_oSep;
			nullable<SimpleTypes::CTwipsMeasure      > m_oSpace;

			std::vector<ComplexTypes::Word::CColumn *> m_arrColumns;
		};

		//--------------------------------------------------------------------------------
		// EdnProps 17.11.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CEdnProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEdnProps)
			CEdnProps();
			virtual ~CEdnProps();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CEdnPos        > m_oPos;
		};

		//--------------------------------------------------------------------------------
		// FtnProps 17.11.11 (Part 1)
		//--------------------------------------------------------------------------------
		class CFtnProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFtnProps)
			CFtnProps();
			virtual ~CFtnProps();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CNumFmt        > m_oNumFmt;
			nullable<ComplexTypes::Word::CNumRestart    > m_oNumRestart;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumStart;
			nullable<ComplexTypes::Word::CFtnPos        > m_oPos;
		};

		//--------------------------------------------------------------------------------
		// PageBorders 17.6.10 (Part 1)
		//--------------------------------------------------------------------------------
		class CPageBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageBorders)
			CPageBorders();
			virtual ~CPageBorders();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CPageBorderDisplay > m_oDisplay;
			nullable<SimpleTypes::CPageBorderOffset  > m_oOffsetFrom;
			nullable<SimpleTypes::CPageBorderZOrder  > m_oZOrder;

			// Childs
			nullable<ComplexTypes::Word::CBottomPageBorder > m_oBottom;
			nullable<ComplexTypes::Word::CPageBorder       > m_oLeft;
			nullable<ComplexTypes::Word::CPageBorder       > m_oRight;
			nullable<ComplexTypes::Word::CTopPageBorder    > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// SectPrChange 17.13.5.32 (Part 1)
		//--------------------------------------------------------------------------------
		class CSectionProperty;

		class CSectPrChange : public WritingElement
		{
		public:
			CSectPrChange();
			CSectPrChange(XmlUtils::CXmlNode &oNode);
			CSectPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CSectPrChange();
			virtual void    fromXML(XmlUtils::CXmlNode& oNode);
			virtual void    fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<std::wstring                  > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber   > m_oId;
			nullable<std::wstring                  > m_sUserId;

			// Childs
			nullable<CSectionProperty>               m_pSecPr;
		};

		//--------------------------------------------------------------------------------
		// SectionProperty
		//--------------------------------------------------------------------------------
		class CSectionProperty : public WritingElement
		{
		public:
			CSectionProperty(OOX::Document *pMain = NULL);
			CSectionProperty(XmlUtils::CXmlNode &oNode);
			CSectionProperty(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CSectionProperty();

			virtual void ClearItems();
			const CSectionProperty& operator =(const XmlUtils::CXmlNode &oNode);
			const CSectionProperty& operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			bool m_bSectPrChange;

			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidRPr;
			nullable<SimpleTypes::CLongHexNumber                       > m_oRsidSect;

			nullable<ComplexTypes::Word::COnOff2>		m_oBidi;
			nullable<OOX::Logic::CColumns>										m_oCols;
			nullable<ComplexTypes::Word::CDocGrid>								m_oDocGrid;
			nullable<OOX::Logic::CEdnProps>										m_oEndnotePr;
			std::vector<ComplexTypes::Word::CHdrFtrRef*>						m_arrFooterReference;
			nullable<OOX::Logic::CFtnProps>										m_oFootnotePr;
			nullable<ComplexTypes::Word::COnOff2>		m_oFormProt;
			std::vector<ComplexTypes::Word::CHdrFtrRef*>						m_arrHeaderReference;
			nullable<ComplexTypes::Word::CLineNumber>							m_oLnNumType;
			nullable<ComplexTypes::Word::COnOff2>		m_oNoEndnote;
			nullable<ComplexTypes::Word::CPaperSource>							m_oPaperSrc;
			nullable<OOX::Logic::CPageBorders>									m_oPgBorders;
			nullable<ComplexTypes::Word::CPageMar>								m_oPgMar;
			nullable<ComplexTypes::Word::CPageNumber>							m_oPgNumType;
			nullable<ComplexTypes::Word::CPageSz>								m_oPgSz;
			nullable<ComplexTypes::Word::CRel>									m_oPrinterSettings;
			nullable<ComplexTypes::Word::COnOff2>		m_oRtlGutter;
			nullable<OOX::Logic::CSectPrChange>									m_oSectPrChange;
			nullable<ComplexTypes::Word::CTextDirection>						m_oTextDirection;
			nullable<ComplexTypes::Word::COnOff2 >		m_oTitlePg;
			nullable<ComplexTypes::Word::CSectType>								m_oType;
			nullable<ComplexTypes::Word::CVerticalJc>							m_oVAlign;
		};

	} // namespace Logic
}// namespace OOX

