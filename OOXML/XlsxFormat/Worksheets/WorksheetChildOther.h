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

#include "../CommonInclude.h"
#include "../Styles/rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CProtectedRange : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProtectedRange)
            WritingElement_XlsbConstructors(CProtectedRange)
			CProtectedRange();
			virtual ~CProtectedRange();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CCryptAlgoritmName>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oSpinCount;
			nullable_string				m_oHashValue;
			nullable_string				m_oSaltValue;
			nullable_string				m_oName;
			nullable_string				m_oSqref;
			std::vector<std::wstring>	m_arSecurityDescriptors;
		};

		class CProtectedRanges : public WritingElementWithChilds<CProtectedRange>
		{
		public:
			WritingElement_AdditionConstructors(CProtectedRanges)
            WritingElement_XlsbVectorConstructors(CProtectedRanges)
			CProtectedRanges();
			virtual ~CProtectedRanges();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			virtual EElementType getType() const;
		};

		class CCellWatch : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCellWatch)
			WritingElement_XlsbConstructors(CCellWatch)
			CCellWatch();
			virtual ~CCellWatch();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string m_oR;
		};

		class CCellWatches : public WritingElementWithChilds<CCellWatch>
		{
		public:
			WritingElement_AdditionConstructors(CCellWatches)
			WritingElement_XlsbVectorConstructors(CCellWatches)
			CCellWatches();
			virtual ~CCellWatches();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);


			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			virtual EElementType getType() const;

		};

		class CPageMargins : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageMargins)
            WritingElement_XlsbConstructors(CPageMargins)
			CPageMargins();
			virtual ~CPageMargins();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CInch>	m_oLeft;
			nullable<SimpleTypes::CInch>	m_oTop;
			nullable<SimpleTypes::CInch>	m_oRight;
			nullable<SimpleTypes::CInch>	m_oBottom;
			nullable<SimpleTypes::CInch>	m_oHeader;
			nullable<SimpleTypes::CInch>	m_oFooter;
		};

		class CPageSetup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageSetup)
            WritingElement_XlsbConstructors(CPageSetup)
			CPageSetup();
			virtual ~CPageSetup();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::COnOff>							m_oBlackAndWhite;
			nullable<SimpleTypes::Spreadsheet::CCellComments>		m_oCellComments;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oCopies;
			nullable<SimpleTypes::COnOff>							m_oDraft;
			nullable<SimpleTypes::Spreadsheet::CPrintError>			m_oErrors;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oFirstPageNumber;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oFitToHeight;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oFitToWidth;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oHorizontalDpi;
			nullable<SimpleTypes::CRelationshipId>					m_oRId;
			nullable<SimpleTypes::CPageOrientation>					m_oOrientation;
			nullable<SimpleTypes::Spreadsheet::CPageOrder>			m_oPageOrder;
			nullable<SimpleTypes::CDouble>							m_oPaperHeight;
			nullable<SimpleTypes::Spreadsheet::CPageSize>			m_oPaperSize;
			nullable<SimpleTypes::CDouble>							m_oPaperWidth;
			nullable<SimpleTypes::Spreadsheet::CPageUnits>			m_oPaperUnits;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oScale;
			nullable<SimpleTypes::COnOff>							m_oUseFirstPageNumber;
			nullable<SimpleTypes::COnOff>							m_oUsePrinterDefaults;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oVerticalDpi;
		};

		class CPrintOptions : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPrintOptions)
            WritingElement_XlsbConstructors(CPrintOptions)
			CPrintOptions();
			virtual ~CPrintOptions();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>	m_oGridLines;
			nullable<SimpleTypes::COnOff>	m_oGridLinesSet;
			nullable<SimpleTypes::COnOff>	m_oHeadings;
			nullable<SimpleTypes::COnOff>	m_oHorizontalCentered;
			nullable<SimpleTypes::COnOff>	m_oVerticalCentered;
		};

		class CDimension : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDimension)
            WritingElement_XlsbConstructors(CDimension)
			CDimension();
			virtual ~CDimension();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable_string	m_oRef;
		};

		class CSheetFormatPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetFormatPr)
            WritingElement_XlsbConstructors(CSheetFormatPr)
			CSheetFormatPr();
			virtual ~CSheetFormatPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
				nullable_uint		m_oBaseColWidth;
				nullable_bool		m_oCustomHeight;
				nullable_double		m_oDefaultColWidth;
				nullable_double		m_oDefaultRowHeight;
				nullable_uint		m_oOutlineLevelCol;
				nullable_uint		m_oOutlineLevelRow;
				nullable_bool		m_oThickBottom;
				nullable_bool		m_oThickTop;
				nullable_bool		m_oZeroHeight;
		};

		class CPane : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPane)
            WritingElement_XlsbConstructors(CPane)
			CPane();
			virtual ~CPane();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CActivePane>		m_oActivePane;
			nullable<SimpleTypes::Spreadsheet::CPaneState>		m_oState;
			nullable_string										m_oTopLeftCell;
			nullable<SimpleTypes::CDouble>						m_oXSplit;
			nullable<SimpleTypes::CDouble>						m_oYSplit;
		};

		class CSelection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSelection)
            WritingElement_XlsbConstructors(CSelection)
			CSelection();
			virtual ~CSelection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable_string										m_oActiveCell;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oActiveCellId;
			nullable_string										m_oSqref;
			nullable<SimpleTypes::Spreadsheet::CActivePane>		m_oPane;
		};

		//необработано:
		//<extLst>
		//<pivotSelection>
		class CSheetView : public WritingElementWithChilds<CSelection>
		{
		public:
			WritingElement_AdditionConstructors(CSheetView)
            WritingElement_XlsbConstructors(CSheetView)
			CSheetView();
			virtual ~CSheetView();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
				nullable<CPane>										m_oPane;

				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oColorId;
				nullable<SimpleTypes::COnOff>						m_oDefaultGridColor;
				nullable<SimpleTypes::COnOff>						m_oRightToLeft;
				nullable<SimpleTypes::COnOff>						m_oShowFormulas;
				nullable<SimpleTypes::COnOff>						m_oShowGridLines;
				nullable<SimpleTypes::COnOff>						m_oShowOutlineSymbols;
				nullable<SimpleTypes::COnOff>						m_oShowRowColHeaders;
				nullable<SimpleTypes::COnOff>						m_oShowRuler;
				nullable<SimpleTypes::COnOff>						m_oShowWhiteSpace;
				nullable<SimpleTypes::COnOff>						m_oShowZeros;
				nullable<SimpleTypes::COnOff>						m_oTabSelected;
				nullable_string										m_oTopLeftCell;
				nullable<SimpleTypes::Spreadsheet::CSheetViewType>	m_oView;
				nullable<SimpleTypes::COnOff>						m_oWindowProtection;
				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oWorkbookViewId;
				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oZoomScale;
				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oZoomScaleNormal;
				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oZoomScalePageLayoutView;
				nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oZoomScaleSheetLayoutView;
		};

		class CSheetViews : public WritingElementWithChilds<CSheetView>
		{
		public:
			WritingElement_AdditionConstructors(CSheetViews)
            WritingElement_XlsbConstructors(CSheetViews)
			CSheetViews();
			virtual ~CSheetViews();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;
		};

		class CPageSetUpPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageSetUpPr)
            WritingElement_XlsbConstructors(CPageSetUpPr)
			CPageSetUpPr();
			virtual ~CPageSetUpPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>		m_oAutoPageBreaks;
			nullable<SimpleTypes::COnOff>		m_oFitToPage;
		};

		class COutlinePr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COutlinePr)
            WritingElement_XlsbConstructors(COutlinePr)
			COutlinePr();
			virtual ~COutlinePr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>		m_oApplyStyles;
			nullable<SimpleTypes::COnOff>		m_oShowOutlineSymbols;
			nullable<SimpleTypes::COnOff>		m_oSummaryBelow;
			nullable<SimpleTypes::COnOff>		m_oSummaryRight;
		};

		class CSheetPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetPr)
            WritingElement_XlsbConstructors(CSheetPr)
			CSheetPr();
			virtual ~CSheetPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<Spreadsheet::CColor>		m_oTabColor;
			nullable<CPageSetUpPr>				m_oPageSetUpPr;
			nullable<COutlinePr>				m_oOutlinePr;

			nullable_string						m_oCodeName;
			nullable<SimpleTypes::COnOff>		m_oEnableFormatConditionsCalculation;
			nullable<SimpleTypes::COnOff>		m_oFilterMode;
			nullable<SimpleTypes::COnOff>		m_oPublished;
			nullable<SimpleTypes::COnOff>		m_oSyncHorizontal;
			nullable_string						m_oSyncRef;
			nullable<SimpleTypes::COnOff>		m_oSyncVertical;
			nullable<SimpleTypes::COnOff>		m_oTransitionEntry;
			nullable<SimpleTypes::COnOff>		m_oTransitionEvaluation;
		};

		class CHeaderFooterElement : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooterElement)
			CHeaderFooterElement();
			virtual ~CHeaderFooterElement();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::wstring	m_sText;
		};

		class CHeaderFooter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooter)
            WritingElement_XlsbConstructors(CHeaderFooter)
			CHeaderFooter();
			virtual ~CHeaderFooter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<CHeaderFooterElement>		m_oEvenFooter;
			nullable<CHeaderFooterElement>		m_oEvenHeader;
			nullable<CHeaderFooterElement>		m_oFirstFooter;
			nullable<CHeaderFooterElement>		m_oFirstHeader;
			nullable<CHeaderFooterElement>		m_oOddFooter;
			nullable<CHeaderFooterElement>		m_oOddHeader;

			nullable<SimpleTypes::COnOff>		m_oAlignWithMargins;
			nullable<SimpleTypes::COnOff>		m_oDifferentFirst;
			nullable<SimpleTypes::COnOff>		m_oDifferentOddEven;
			nullable<SimpleTypes::COnOff>		m_oScaleWithDoc;

		};

		class CLegacyDrawingHFWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLegacyDrawingHFWorksheet)
            WritingElement_XlsbConstructors(CLegacyDrawingHFWorksheet)
			CLegacyDrawingHFWorksheet();
			virtual ~CLegacyDrawingHFWorksheet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring  toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId >				m_oId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCff;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oChe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oChf;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCho;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLff;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oLho;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRff;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oRho;
		};

		class CPictureWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPictureWorksheet)
            WritingElement_XlsbConstructors(CPictureWorksheet)
			CPictureWorksheet();
			virtual ~CPictureWorksheet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring  toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

		class CBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBreak)
            WritingElement_XlsbConstructors(CBreak)
			CBreak();
			virtual ~CBreak();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oId;
			nullable<SimpleTypes::COnOff>					m_oMan;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMin;
			nullable<SimpleTypes::COnOff>					m_oPt;
		};

		class CRowColBreaks : public WritingElementWithChilds<CBreak>
		{
		public:
			WritingElement_AdditionConstructors(CRowColBreaks)
            WritingElement_XlsbConstructors(CRowColBreaks)
			CRowColBreaks();
			virtual ~CRowColBreaks();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oManualBreakCount;
		};

		class CSheetProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetProtection)
            WritingElement_XlsbConstructors(CSheetProtection)
			CSheetProtection();
			virtual ~CSheetProtection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::CCryptAlgoritmName>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oSpinCount;
			nullable_string		m_oHashValue;
			nullable_string		m_oSaltValue;
			nullable_string		m_oPassword; //for old wrike protection

			nullable<SimpleTypes::COnOff>		m_oAutoFilter;
			nullable<SimpleTypes::COnOff>		m_oContent;
			nullable<SimpleTypes::COnOff>		m_oDeleteColumns;
			nullable<SimpleTypes::COnOff>		m_oDeleteRows;
			nullable<SimpleTypes::COnOff>		m_oFormatCells;
			nullable<SimpleTypes::COnOff>		m_oFormatColumns;
			nullable<SimpleTypes::COnOff>		m_oFormatRows;
			nullable<SimpleTypes::COnOff>		m_oInsertColumns;
			nullable<SimpleTypes::COnOff>		m_oInsertHyperlinks;
			nullable<SimpleTypes::COnOff>		m_oInsertRows;
			nullable<SimpleTypes::COnOff>		m_oObjects;
			nullable<SimpleTypes::COnOff>		m_oPivotTables;
			nullable<SimpleTypes::COnOff>		m_oScenarios;
			nullable<SimpleTypes::COnOff>		m_oSelectLockedCells;
			nullable<SimpleTypes::COnOff>		m_oSelectUnlockedCells;
			nullable<SimpleTypes::COnOff>		m_oSheet;
			nullable<SimpleTypes::COnOff>		m_oSort;
		};

		class CDataRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataRef)
            WritingElement_XlsbConstructors(CDataRef)
			CDataRef();
			virtual ~CDataRef();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CRelationshipId>	m_oId;
			nullable_string							m_oName;
			nullable_string							m_oRef;
			nullable_string							m_oSheet;
		};

		class CDataRefs : public WritingElementWithChilds<CDataRef>
		{
		public:
			WritingElement_AdditionConstructors(CDataRefs)
            WritingElement_XlsbConstructors(CDataRefs)
			CDataRefs();
			virtual ~CDataRefs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
		};

		class CDataConsolidate : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataConsolidate)
            WritingElement_XlsbConstructors(CDataConsolidate)
			CDataConsolidate();
			virtual ~CDataConsolidate();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CDataConsolidateFunction>	m_oFunction;
			nullable<SimpleTypes::COnOff>									m_oLink;
			nullable<SimpleTypes::COnOff>									m_oStartLabels;
			nullable<SimpleTypes::COnOff>									m_oTopLabels;

			nullable<CDataRefs>	m_oDataRefs;

		};
	} //Spreadsheet
} // namespace OOX
