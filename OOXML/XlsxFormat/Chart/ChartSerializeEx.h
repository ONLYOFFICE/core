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
//Generated code
#pragma once

#include "ChartSerialize.h"

namespace OOX
{
namespace Spreadsheet
{
namespace ChartEx
{
//------------------------------------------------------------------------------------------------------
	class CFormula : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CFormula)	

		CFormula(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<SimpleTypes::Spreadsheet::CFormulaDirection>	m_dir;
		std::wstring											m_content;
	};
//------------------------------------------------------------------------------------------------------
	class CTextData : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CTextData)	

		CTextData(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<CFormula>	m_oF;
		nullable_string		m_oV;	
	};
//------------------------------------------------------------------------------------------------------
	class CText : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CText)	

		CText(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<PPTX::Logic::TxBody>	m_oRich;
		nullable<CTextData>				m_txData;
	};
//------------------------------------------------------------------------------------------------------
	class CTitle : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CTitle)	

		CTitle(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<CText>										m_tx;
		nullable<PPTX::Logic::TxBody>						m_oTxPr;
		nullable<PPTX::Logic::SpPr>							m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;

//for chart title only (but not axis title)
		nullable<SimpleTypes::Spreadsheet::CSidePos>		m_pos;
		nullable<SimpleTypes::Spreadsheet::CPosAlign>		m_align;
		nullable_bool										m_overlay;
	};
//------------------------------------------------------------------------------------------------------
	class CNumericValue: public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CNumericValue)	

		CNumericValue(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_int		m_idx;
		nullable_double		m_content;
	};
//------------------------------------------------------------------------------------------------------
	class CNumericLevel : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CNumericLevel)	

		CNumericLevel(){}
		virtual ~CNumericLevel();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		std::vector<CNumericValue*>	m_arPt;
		
		nullable_int				m_ptCount;
		nullable_string				m_formatCode;
		nullable_string				m_name;
	};
//------------------------------------------------------------------------------------------------------
	class CStringValue: public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CStringValue)	

		CStringValue(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_int		m_idx;
		std::wstring		m_content;
	};
//------------------------------------------------------------------------------------------------------
	class CStringLevel : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CStringLevel)	

		CStringLevel(){}
		virtual ~CStringLevel();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		std::vector<CStringValue*>	m_arPt;
		
		nullable_int				m_ptCount;
		nullable_string				m_name;
	};
//------------------------------------------------------------------------------
	class CSeriesElementVisibilities : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CSeriesElementVisibilities)

		CSeriesElementVisibilities(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_bool		m_connectorLines;
		nullable_bool		m_meanLine;
		nullable_bool		m_meanMarker;
		nullable_bool		m_nonoutliers;
		nullable_bool		m_outliers;
	};
//------------------------------------------------------------------------------
	class CDataLabelVisibilities : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CDataLabelVisibilities)

		CDataLabelVisibilities(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_bool		m_seriesName;
		nullable_bool		m_categoryName;
		nullable_bool		m_value;
	};
//------------------------------------------------------------------------------
	class CBinning : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CBinning)

		CBinning(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_double		m_binSize;
		nullable_int		m_binCount;

		nullable<SimpleTypes::Spreadsheet::CIntervalClosedSide>		m_intervalClosed;
		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_underflow;
		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_overflow;
	};
//------------------------------------------------------------------------------
	class CStatistics : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CStatistics)

		CStatistics(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<SimpleTypes::Spreadsheet::CQuartileMethod> m_quartileMethod;
	};
//------------------------------------------------------------------------------
	class CSubtotals : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CSubtotals)

		CSubtotals(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		std::vector<unsigned int>	m_arIdx;
	};
//------------------------------------------------------------------------------
	class CSeriesLayoutProperties : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CSeriesLayoutProperties)

		CSeriesLayoutProperties(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<ComplexTypes::Spreadsheet::CParentLabelLayout>	m_parentLabelLayout;
		nullable<ComplexTypes::Spreadsheet::CRegionLabelLayout>	m_regionLabelLayout;

		nullable<CSeriesElementVisibilities>					m_visibility;
		nullable_bool											m_aggregation; //empty node
		nullable<CBinning>										m_binning;
		nullable<CStatistics>									m_statistics;
		nullable<CSubtotals>									m_subtotals;
		nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;
		//todooo geography;
	};
//------------------------------------------------------------------------------
	class CNumberFormat : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CNumberFormat)

		CNumberFormat(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_string	m_formatCode;
		nullable_bool	m_sourceLinked;
	};
//------------------------------------------------------------------------------
	class CDataLabelHidden : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CDataLabelHidden)

		CDataLabelHidden(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		
		nullable_int	m_idx;
	};	
//------------------------------------------------------------------------------
	class CDataLabel : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CDataLabel)

		CDataLabel(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<CNumberFormat>								m_numFmt;
		nullable<PPTX::Logic::TxBody>						m_oTxPr;
		nullable<PPTX::Logic::SpPr>							m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
		nullable<CDataLabelVisibilities>					m_visibility;
		nullable_string										m_separator; //node

		nullable_int										m_idx;
		nullable<SimpleTypes::Spreadsheet::CDataLabelPos>	m_dataLabelPos;
	};
//------------------------------------------------------------------------------
	class CDataLabels : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CDataLabels)

		CDataLabels(){}
		virtual ~CDataLabels();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<CNumberFormat>								m_numFmt;
		nullable<PPTX::Logic::TxBody>						m_oTxPr;
		nullable<PPTX::Logic::SpPr>							m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
		nullable<CDataLabelVisibilities>					m_visibility;
		nullable_string										m_separator;
		std::vector<CDataLabel*>							m_arDataLabel;
		std::vector<CDataLabelHidden*>						m_arDataLabelHidden;
		
		nullable<SimpleTypes::Spreadsheet::CDataLabelPos>	m_pos;
	};
//------------------------------------------------------------------------------
	class CDataPoint : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CDataPoint)

		CDataPoint(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<PPTX::Logic::SpPr>							m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;

		nullable_int										m_idx;
	};
//------------------------------------------------------------------------------
	class CSeries : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CSeries)	

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		CSeries(){}
		virtual ~CSeries();

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		//CValueColors				m_valueColors;
		//CValueColorPositions		m_valueColorPositions;

		std::vector<CDataPoint*>						m_arDataPt;
		nullable<CDataLabels>							m_dataLabels;

		nullable<CSeriesLayoutProperties>				m_layoutPr;
		nullable<CText>									m_tx;
		std::vector<unsigned long>						m_arAxisId;
		nullable<ComplexTypes::CDecimalNumber>			m_dataId;
		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

		nullable<SimpleTypes::Spreadsheet::CSeriesLayout>	m_oLayoutId;
		nullable_bool										m_bHidden;
		nullable_int										m_nOwnerIdx;
		nullable_string										m_sUniqueId;
		nullable_int										m_nFormatIdx;
	};
//------------------------------------------------------------------------------
	class CCatScaling : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CCatScaling)	

		CCatScaling(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>> m_gapWidth;	//typedef ST_DoubleOrAutomatic ST_GapWidthRatio 
	};
//------------------------------------------------------------------------------
	class CValScaling : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CValScaling)	

		CValScaling() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_max;
		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_min;

		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_majorUnit; //ST_ValueAxisUnit == ST_DoubleOrAutomatic  
		nullable<SimpleTypes::Spreadsheet::CDoubleOrAutomatic<>>	m_minorUnit;
	};
//------------------------------------------------------------------------------
	class CAxisUnitsLabel : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CAxisUnitsLabel)

		CAxisUnitsLabel(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<CText>									m_tx;
		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<PPTX::Logic::TxBody>					m_oTxPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CAxisUnit : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CAxisUnit)	
		
		CAxisUnit(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		nullable<CAxisUnitsLabel>						m_unitsLabel;
		
		nullable<SimpleTypes::Spreadsheet::CAxisUnit>	m_unit;
	};
//------------------------------------------------------------------------------
	class CGridlines : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CGridlines)

		CGridlines(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<PPTX::Logic::SpPr>							m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;

		std::wstring m_name;
	};
//------------------------------------------------------------------------------
	class CTickMarks : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CTickMarks)	
		
		CTickMarks(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;		
		nullable<SimpleTypes::Spreadsheet::CTickMarksType>		m_type;
		std::wstring											m_name;
	};
//------------------------------------------------------------------------------
	class CAxis : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CAxis)	

		CAxis() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<CCatScaling>		m_catScaling;
		nullable<CValScaling>		m_valScaling;
		nullable<CTitle>			m_title;
		nullable<CAxisUnit>			m_units;

		nullable<CNumberFormat>		m_numFmt;
		nullable<CTickMarks>		m_majorTickMarks;
		nullable<CTickMarks>		m_minorTickMarks;
		nullable<CGridlines>		m_majorGridlines;
		nullable<CGridlines>		m_minorGridlines;

		nullable_bool				m_tickLabels;//blank node

		nullable<PPTX::Logic::SpPr>		m_oSpPr;
		nullable<PPTX::Logic::TxBody>	m_oTxPr;	
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

		nullable_uint					m_id;
		nullable_bool					m_hidden;
	};
//------------------------------------------------------------------------------
	class CPlotSurface : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPlotSurface)	

		CPlotSurface(){}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CPlotAreaRegion : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPlotAreaRegion)	

		CPlotAreaRegion(){}
		virtual ~CPlotAreaRegion();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<CPlotSurface>							m_plotSurface;
        std::vector<CSeries*>							m_arSeries;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CPlotArea : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CPlotArea)	

		CPlotArea(){}
		virtual ~CPlotArea();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		CPlotAreaRegion									m_plotAreaRegion;

        std::vector<CAxis*>								m_arAxis;
		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CLegend : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CLegend)	

		CLegend() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<PPTX::Logic::TxBody>					m_oTxPr;	
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

		nullable<SimpleTypes::Spreadsheet::CSidePos>	m_pos;
		nullable<SimpleTypes::Spreadsheet::CPosAlign>	m_align;
		nullable_bool									m_overlay;
	};
//------------------------------------------------------------------------------
	class CChart : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CChart)	

		CChart() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		CPlotArea										m_plotArea;

		nullable<CTitle>								m_title;
		nullable<CLegend>								m_legend;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};

//------------------------------------------------------------------------------
	class CDimension : public WritingElement
	{
	public:
		CDimension() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) = 0; 
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) = 0; 
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const = 0; 
		virtual std::wstring toXML() const = 0; 
		virtual EElementType getType () const = 0; 

		nullable<CFormula>		m_f;
		nullable_string 		m_nf;

		nullable<SimpleTypes::Spreadsheet::CDimensionType>		m_type;
		nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CNumDimension : public CDimension
	{
	public:
		WritingElement_AdditionConstructors(CNumDimension)	

		CNumDimension();
		virtual ~CNumDimension();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		std::vector<CNumericLevel*>	m_levelData; 
	};
//------------------------------------------------------------------------------
	class CStrDimension : public CDimension
	{
	public:
		WritingElement_AdditionConstructors(CStrDimension)	

		CStrDimension();
		virtual ~CStrDimension();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		std::vector<CStringLevel*>	m_levelData;  //todooo CStrVal (+name)

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
	};
//------------------------------------------------------------------------------
	class CData : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CData)	

		CData() {}
		virtual ~CData();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		std::vector<CDimension*>						m_arDimension;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		nullable_uint									m_id;
	};
//------------------------------------------------------------------------------
	class CExternalData : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CExternalData)
		CExternalData() {}

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType() const;
		
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		nullable_string m_id;
		nullable_bool m_autoUpdate;
	};
//------------------------------------------------------------------------------
	class CChartData : public WritingElement
	{
	public:
		WritingElement_AdditionConstructors(CChartData)	

		CChartData() {}
		virtual ~CChartData();

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		nullable<CExternalData>							m_externalData;
		std::vector<CData*>								m_arData;
		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
	};
//------------------------------------------------------------------------------
	class CChartSpace : public WritingElement
	{
	public:
		CChartSpace() {}
		WritingElement_AdditionConstructors(CChartSpace)	

		virtual void fromXML(XmlUtils::CXmlNode& node) {}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
		virtual std::wstring toXML() const { return L""; }
		virtual EElementType getType () const;

		CChartData										m_chartData;
		CChart											m_chart;

		nullable<PPTX::Logic::SpPr>						m_oSpPr;
		nullable<PPTX::Logic::TxBody>					m_oTxPr;	

		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		nullable<PPTX::Logic::ClrMap>					m_oClrMapOvr;

		//CT_PrintSettings*						m_printSettings;
		//CFormatOverrides*						_formatOverrides;
	};
}
}
}

