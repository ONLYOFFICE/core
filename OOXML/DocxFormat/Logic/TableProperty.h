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
#include "../../Common/ComplexTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// JcTable 17.4.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CJcTable : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CJcTable)

			CJcTable();
			virtual ~CJcTable();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CJcTable> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TblLayoutType 17.4.54 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblLayoutType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblLayoutType)

			CTblLayoutType();
			virtual ~CTblLayoutType();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTblLayoutType> m_oType;
		};

		//--------------------------------------------------------------------------------
		// TblLook 17.4.56 (Part 1) + 9.3.11 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblLook : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblLook)

			CTblLook();
			virtual ~CTblLook();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

			const bool IsFirstRow() const;
			const bool IsLastRow() const;
			const bool IsFirstColumn() const;
			const bool IsLastColumn () const;
			const bool IsNoHBand    () const;
			const bool IsNoVBand    () const;

			const int GetValue() const;
			void SetValue(int nVal);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff            > m_oFirstColumn;
			nullable<SimpleTypes::COnOff            > m_oFirstRow;
			nullable<SimpleTypes::COnOff            > m_oLastColumn;
			nullable<SimpleTypes::COnOff            > m_oLastRow;
			nullable<SimpleTypes::COnOff            > m_oNoHBand;
			nullable<SimpleTypes::COnOff            > m_oNoVBand;
			nullable<SimpleTypes::CShortHexNumber   > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TblOverlap 17.4.56 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblOverlap : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblOverlap)

			CTblOverlap();
			virtual ~CTblOverlap();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		public:
			nullable<SimpleTypes::CTblOverlap> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// TblPPr 17.4.58 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPPr : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblPPr)

			CTblPPr();
			virtual ~CTblPPr();

			void PrepareAfterRead();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTwipsMeasure       > m_oBottomFromText;
			nullable<SimpleTypes::CHAnchor            > m_oHorzAnchor;
			nullable<SimpleTypes::CTwipsMeasure       > m_oLeftFromText;
			nullable<SimpleTypes::CTwipsMeasure       > m_oRightFromText;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTblpX;
			nullable<SimpleTypes::CXAlign             > m_oTblpXSpec;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTblpY;
			nullable<SimpleTypes::CYAlign             > m_oTblpYSpec;
			nullable<SimpleTypes::CTwipsMeasure       > m_oTopFromText;
			nullable<SimpleTypes::CVAnchor            > m_oVertAnchor;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TblBorders 17.4.39 (Part 1) + 9.3.2 (Part 4) + 9.3.6 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTblBorders)

			CTblBorders();
			virtual ~CTblBorders();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTblBorders Merge(const CTblBorders& oPrev, const CTblBorders& oCurrent);

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

		public:
			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oEnd;
			nullable<ComplexTypes::Word::CBorder > m_oInsideH;
			nullable<ComplexTypes::Word::CBorder > m_oInsideV;
			nullable<ComplexTypes::Word::CBorder > m_oStart;
			nullable<ComplexTypes::Word::CBorder > m_oTop;

		};

		//--------------------------------------------------------------------------------
		// TblCellMar 17.4.43 (Part 1) + 9.3.4 (Part 4) + 9.3.8 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblCellMar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTblCellMar)

			CTblCellMar();
			virtual ~CTblCellMar();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTblCellMar Merge(const CTblCellMar& oPrev, const CTblCellMar& oCurrent);

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
		public:

			nullable<ComplexTypes::Word::CTblWidth > m_oBottom;
			nullable<ComplexTypes::Word::CTblWidth > m_oEnd;
			nullable<ComplexTypes::Word::CTblWidth > m_oStart;
			nullable<ComplexTypes::Word::CTblWidth > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// TblPrChange 17.13.5.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableProperty;

		class CTblPrChange : public WritingElement
		{
		public:
			CTblPrChange(OOX::Document *pMain = NULL);
			CTblPrChange(XmlUtils::CXmlNode &oNode);
			CTblPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblPrChange();
			const CTblPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			nullable<CTableProperty>				m_pTblPr;
		};

		//--------------------------------------------------------------------------------
		// TableProperty
		//--------------------------------------------------------------------------------
		class CTableProperty : public WritingElement
		{
		public:
			CTableProperty(OOX::Document *pMain = NULL);
			CTableProperty(XmlUtils::CXmlNode &oNode);
			CTableProperty(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTableProperty();

			const CTableProperty& operator=(const XmlUtils::CXmlNode &oNode);
			const CTableProperty& operator=(const XmlUtils::CXmlLiteReader& oReader);


			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTableProperty Merge(const CTableProperty& oPrev, const CTableProperty& oCurrent);

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
			bool                                                           m_bTblPrChange;

			nullable<ComplexTypes::Word::COnOff2 > m_oBidiVisual;
			nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
			nullable<ComplexTypes::Word::CShading                        > m_oShade;
			nullable<OOX::Logic::CTblBorders                             > m_oTblBorders;
			nullable<ComplexTypes::Word::String                        > m_oTblCaption;
			nullable<OOX::Logic::CTblCellMar                             > m_oTblCellMar;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
			nullable<ComplexTypes::Word::String                        > m_oTblDescription;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblInd;
			nullable<ComplexTypes::Word::CTblLayoutType                  > m_oTblLayout;
			nullable<ComplexTypes::Word::CTblLook                        > m_oTblLook;
			nullable<ComplexTypes::Word::CTblOverlap                     > m_oTblOverlap;
			nullable<ComplexTypes::Word::CTblPPr                         > m_oTblpPr;
			nullable<OOX::Logic::CTblPrChange                            > m_oTblPrChange;
			nullable<ComplexTypes::Word::String                        > m_oTblStyle;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleColBandSize;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleRowBandSize;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblW;
		};

	} // namespace Logic
} // namespace OOX

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Height 17.4.81 (Part 1)
		//--------------------------------------------------------------------------------
		class CHeight : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHeight)

			CHeight();
			virtual ~CHeight();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHeightRule >		m_oHRule;
			nullable<SimpleTypes::CTwipsMeasure >	m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TrPrChange 17.13.5.37 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableRowProperties;

		class CTrPrChange : public WritingElement
		{
		public:
			CTrPrChange(OOX::Document *pMain = NULL);
			CTrPrChange(XmlUtils::CXmlNode &oNode);
			CTrPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTrPrChange();
			const CTrPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTrPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			// Childs
			nullable<CTableRowProperties>           m_pTrPr;
		};

		//--------------------------------------------------------------------------------
		// TableRowProperties
		//--------------------------------------------------------------------------------
		class CTableRowProperties : public WritingElement
		{
		public:
			CTableRowProperties(OOX::Document *pMain = NULL);
			CTableRowProperties(XmlUtils::CXmlNode &oNode);
			CTableRowProperties(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTableRowProperties();

			const CTableRowProperties& operator =(const XmlUtils::CXmlNode &oNode);
			const CTableRowProperties& operator =(const XmlUtils::CXmlLiteReader& oReader);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTableRowProperties Merge(const CTableRowProperties& oPrev, const CTableRowProperties& oCurrent);

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
			bool                                                           m_bTrPrChange;

			nullable<ComplexTypes::Word::COnOff2 > m_oCantSplit;
			nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oDel;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oDivId;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridAfter;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridBefore;
			nullable<ComplexTypes::Word::COnOff2 > m_oHidden;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oIns;
			nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
			nullable<ComplexTypes::Word::COnOff2 > m_oTblHeader;
			nullable<ComplexTypes::Word::CHeight                         > m_oTblHeight;
			nullable<OOX::Logic::CTrPrChange                             > m_oTrPrChange;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oWAfter;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oWBefore;
		};

	} // namespace Logic
} // namespace OOX

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// CellMergeTrackChange 17.13.5.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CCellMergeTrackChange : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCellMergeTrackChange)

			CCellMergeTrackChange();
			virtual ~CCellMergeTrackChange();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring                     > m_sAuthor;
			nullable<SimpleTypes::CDateTime           > m_oDate;
			nullable<SimpleTypes::CDecimalNumber      > m_oId;
			nullable<SimpleTypes::CAnnotationVMerge   > m_oVMerge;
			nullable<SimpleTypes::CAnnotationVMerge   > m_oVMergeOrig;
			nullable<std::wstring                     > m_sUserId;

		};

		//--------------------------------------------------------------------------------
		// HMerge 17.4.22 (Part 1)
		//--------------------------------------------------------------------------------
		class CHMerge : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHMerge)

			CHMerge();
			virtual ~CHMerge();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CMerge> m_oVal;

		};

		//--------------------------------------------------------------------------------
		// VMerge 17.4.85 (Part 1)
		//--------------------------------------------------------------------------------
		class CVMerge : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVMerge)

			CVMerge();
			virtual ~CVMerge();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CMerge> m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Headers 17.13.1.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CHeaders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaders)

			CHeaders(OOX::Document *pMain = NULL);
			virtual ~CHeaders();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			std::vector<ComplexTypes::Word::String *> m_arrHeaders;
		};

		//--------------------------------------------------------------------------------
		// TcBorders 17.4.67 (Part 1) + 9.3.1 (Part 4) + 9.3.5 (Part 4)
		//--------------------------------------------------------------------------------
		class CTcBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTcBorders)

			CTcBorders();
			virtual ~CTcBorders();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTcBorders Merge(const CTcBorders& oPrev, const CTcBorders& oCurrent);

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

		public:
			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oEnd;
			nullable<ComplexTypes::Word::CBorder > m_oInsideH;
			nullable<ComplexTypes::Word::CBorder > m_oInsideV;
			nullable<ComplexTypes::Word::CBorder > m_oStart;
			nullable<ComplexTypes::Word::CBorder > m_oTL2BR;
			nullable<ComplexTypes::Word::CBorder > m_oTop;
			nullable<ComplexTypes::Word::CBorder > m_oTR2BL;
		};

		//--------------------------------------------------------------------------------
		// TcMar 17.4.69 (Part 1) + 9.3.3 (Part 4) + 9.3.7 (Part 4)
		//--------------------------------------------------------------------------------
		class CTcMar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTcMar)

			CTcMar();
			virtual ~CTcMar();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTcMar Merge(const CTcMar& oPrev, const CTcMar& oCurrent);

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

			nullable<ComplexTypes::Word::CTblWidth > m_oBottom;
			nullable<ComplexTypes::Word::CTblWidth > m_oEnd;
			nullable<ComplexTypes::Word::CTblWidth > m_oStart;
			nullable<ComplexTypes::Word::CTblWidth > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// TblPrChange 17.13.5.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableCellProperties;

		class CTcPrChange : public WritingElement
		{
		public:
			CTcPrChange(OOX::Document *pMain = NULL);
			CTcPrChange(XmlUtils::CXmlNode &oNode);
			CTcPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTcPrChange();

			const CTcPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTcPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			nullable<CTableCellProperties>			m_pTcPr;
		};

		//--------------------------------------------------------------------------------
		// TableCellProperties
		//--------------------------------------------------------------------------------
		class CTableCellProperties : public WritingElement
		{
		public: 
			CTableCellProperties(OOX::Document *pMain = NULL);
			CTableCellProperties(XmlUtils::CXmlNode &oNode);
			CTableCellProperties(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTableCellProperties();

			const CTableCellProperties& operator =(const XmlUtils::CXmlNode &oNode);
			const CTableCellProperties& operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTableCellProperties Merge(const CTableCellProperties& oPrev, const CTableCellProperties& oCurrent);

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

		public:

			bool m_bTcPrChange;

			nullable<ComplexTypes::Word::CTrackChange                    > m_oCellDel;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oCellIns;
			nullable<ComplexTypes::Word::CCellMergeTrackChange           > m_oCellMerge;
			nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridSpan;
			nullable<OOX::Logic::CHeaders                                > m_oHeaders;
			nullable<ComplexTypes::Word::COnOff2 > m_oHideMark;
			nullable<ComplexTypes::Word::CHMerge                         > m_oHMerge;
			nullable<ComplexTypes::Word::COnOff2 > m_oNoWrap;
			nullable<ComplexTypes::Word::CShading                        > m_oShd;
			nullable<OOX::Logic::CTcBorders                              > m_oTcBorders;
			nullable<ComplexTypes::Word::COnOff2 > m_oTcFitText;
			nullable<OOX::Logic::CTcMar                                  > m_oTcMar;
			nullable<OOX::Logic::CTcPrChange                             > m_oTcPrChange;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTcW;
			nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
			nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;
			nullable<ComplexTypes::Word::CVMerge                         > m_oVMerge;

		};

	} // namespace Logic
} // namespace OOX

