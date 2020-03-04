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

namespace OOX
{
	namespace Spreadsheet
	{
		class CPageMargins : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageMargins)
			CPageMargins()
			{
			}
			virtual ~CPageMargins()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oLeft.IsInit() || m_oTop.IsInit() || m_oRight.IsInit() || m_oBottom.IsInit() || m_oHeader.IsInit() || m_oFooter.IsInit())
				{
					writer.WriteString(L"<pageMargins");
					WritingStringNullableAttrDouble(L"left", m_oLeft, m_oLeft->ToInches());
					WritingStringNullableAttrDouble(L"right", m_oRight, m_oRight->ToInches());
					WritingStringNullableAttrDouble(L"top", m_oTop, m_oTop->ToInches());
					WritingStringNullableAttrDouble(L"bottom", m_oBottom, m_oBottom->ToInches());
					WritingStringNullableAttrDouble(L"header", m_oHeader, m_oHeader->ToInches());
					WritingStringNullableAttrDouble(L"footer", m_oFooter, m_oFooter->ToInches());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_PageMargins;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("left"),	m_oLeft)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("top"),		m_oTop)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("right"),	m_oRight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("bottom"),	m_oBottom)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("header"),	m_oHeader)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("footer"),	m_oFooter)
				WritingElement_ReadAttributes_End( oReader )
			}

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
			CPageSetup()
			{
			}
			virtual ~CPageSetup()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oPaperSize.IsInit() || m_oOrientation.IsInit())
				{
					writer.WriteString(L"<pageSetup");
					WritingStringNullableAttrBool(L"blackAndWhite", m_oBlackAndWhite);
					WritingStringNullableAttrString(L"cellComments", m_oCellComments, m_oCellComments->ToString());
					WritingStringNullableAttrInt(L"copies", m_oCopies, m_oCopies->GetValue());
					WritingStringNullableAttrBool(L"draft", m_oDraft);
					WritingStringNullableAttrString(L"errors", m_oErrors, m_oErrors->ToString());
					WritingStringNullableAttrInt(L"firstPageNumber", m_oFirstPageNumber, m_oFirstPageNumber->GetValue());
					WritingStringNullableAttrInt(L"fitToHeight", m_oFitToHeight, m_oFitToHeight->GetValue());
					WritingStringNullableAttrInt(L"fitToWidth", m_oFitToWidth, m_oFitToWidth->GetValue());
					WritingStringNullableAttrInt(L"horizontalDpi", m_oHorizontalDpi, m_oHorizontalDpi->GetValue());
					WritingStringNullableAttrString(L"r:id", m_oRId, m_oRId->ToString());
					WritingStringNullableAttrString(L"orientation", m_oOrientation, m_oOrientation->ToString());
					WritingStringNullableAttrString(L"pageOrder", m_oPageOrder, m_oPageOrder->ToString());
					WritingStringNullableAttrDouble(L"paperHeight", m_oPaperHeight, m_oPaperHeight->GetValue());
					WritingStringNullableAttrString(L"paperSize", m_oPaperSize, m_oPaperSize->ToString());
					WritingStringNullableAttrDouble(L"paperWidth", m_oPaperWidth, m_oPaperWidth->GetValue());
					WritingStringNullableAttrString(L"paperUnits", m_oPaperUnits, m_oPaperUnits->ToString());
					WritingStringNullableAttrInt(L"scale", m_oScale, m_oScale->GetValue());
					WritingStringNullableAttrBool(L"useFirstPageNumber", m_oUseFirstPageNumber);
					WritingStringNullableAttrBool(L"usePrinterDefaults", m_oUsePrinterDefaults);
					WritingStringNullableAttrInt(L"verticalDpi", m_oVerticalDpi, m_oVerticalDpi->GetValue());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_PageSetup;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("blackAndWhite"),	m_oBlackAndWhite)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cellComments"),	m_oCellComments)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("copies"),	m_oCopies)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("draft"),	m_oDraft)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("errors"),	m_oErrors)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("firstPageNumber"),	m_oFirstPageNumber)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fitToHeight"),	m_oFitToHeight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fitToWidth"),	m_oFitToWidth)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("horizontalDpi"),	m_oHorizontalDpi)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("r:id"),		m_oRId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("orientation"),	m_oOrientation)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("pageOrder"),	m_oPageOrder)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("paperHeight"),	m_oPaperHeight)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("paperSize"),	m_oPaperSize)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("paperWidth"),	m_oPaperWidth)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("paperUnits"),	m_oPaperUnits)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("scale"),	m_oScale)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("useFirstPageNumber"),	m_oUseFirstPageNumber)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("usePrinterDefaults"),	m_oUsePrinterDefaults)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("verticalDpi"),	m_oVerticalDpi)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::COnOff<>>							m_oBlackAndWhite;
			nullable<SimpleTypes::Spreadsheet::CCellComments<>>		m_oCellComments;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oCopies;
			nullable<SimpleTypes::COnOff<>>							m_oDraft;
			nullable<SimpleTypes::Spreadsheet::CPrintError<>>		m_oErrors;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFirstPageNumber;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFitToHeight;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oFitToWidth;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oHorizontalDpi;
			nullable<SimpleTypes::CRelationshipId>					m_oRId;
			nullable<SimpleTypes::CPageOrientation<>>				m_oOrientation;
			nullable<SimpleTypes::Spreadsheet::CPageOrder<>>		m_oPageOrder;
			nullable<SimpleTypes::CDouble>							m_oPaperHeight;
			nullable<SimpleTypes::Spreadsheet::CPageSize<>>			m_oPaperSize;
			nullable<SimpleTypes::CDouble>							m_oPaperWidth;
			nullable<SimpleTypes::Spreadsheet::CPageUnits<>>		m_oPaperUnits;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oScale;
			nullable<SimpleTypes::COnOff<>>							m_oUseFirstPageNumber;
			nullable<SimpleTypes::COnOff<>>							m_oUsePrinterDefaults;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oVerticalDpi;
		};
		class CPrintOptions : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPrintOptions)
			CPrintOptions()
			{
			}
			virtual ~CPrintOptions()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oGridLines.IsInit() || m_oGridLinesSet.IsInit() || m_oHeadings.IsInit())
				{
					writer.WriteString(L"<printOptions");
					WritingStringNullableAttrBool(L"headings",	m_oHeadings);
					WritingStringNullableAttrBool(L"gridLines",	m_oGridLines);
					WritingStringNullableAttrBool(L"gridLinesSet", m_oGridLinesSet);
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_PrintOptions;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("gridLines"),			m_oGridLines)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("gridLinesSet"),		m_oGridLinesSet)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("headings"),			m_oHeadings)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("horizontalCentered"),	m_oHorizontalCentered)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("verticalCentered"),	m_oVerticalCentered)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>	m_oGridLines;
			nullable<SimpleTypes::COnOff<>>	m_oGridLinesSet;
			nullable<SimpleTypes::COnOff<>>	m_oHeadings;
			nullable<SimpleTypes::COnOff<>>	m_oHorizontalCentered;
			nullable<SimpleTypes::COnOff<>>	m_oVerticalCentered;
		};
		class CDimension : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDimension)
			CDimension()
			{
			}
			virtual ~CDimension()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Dimension;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("ref"), m_oRef)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<std::wstring>	m_oRef;
		};

		class CSheetFormatPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetFormatPr)
			CSheetFormatPr()
			{
			}
			virtual ~CSheetFormatPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheetFormatPr"));
				WritingStringNullableAttrInt(L"baseColWidth", m_oBaseColWidth, m_oBaseColWidth->GetValue());
				WritingStringNullableAttrBool(L"customHeight", m_oCustomHeight);
				WritingStringNullableAttrDouble(L"defaultColWidth", m_oDefaultColWidth, m_oDefaultColWidth->GetValue());
				WritingStringNullableAttrDouble(L"defaultRowHeight", m_oDefaultRowHeight, m_oDefaultRowHeight->GetValue());
				WritingStringNullableAttrInt(L"outlineLevelCol", m_oOutlineLevelCol, m_oOutlineLevelCol->GetValue());
				WritingStringNullableAttrInt(L"outlineLevelRow", m_oOutlineLevelRow, m_oOutlineLevelRow->GetValue());
				WritingStringNullableAttrBool(L"thickBottom", m_oThickBottom);
				WritingStringNullableAttrBool(L"thickTop", m_oThickTop);
				WritingStringNullableAttrBool(L"zeroHeight", m_oZeroHeight);
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_SheetFormatPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("baseColWidth"),		m_oBaseColWidth)	// ToDo Excel не воспринимает значения не uint (мы приводим к uint)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("customHeight"),		m_oCustomHeight )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("defaultColWidth"),		m_oDefaultColWidth )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("defaultRowHeight"),	m_oDefaultRowHeight )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("outlineLevelCol"),		m_oOutlineLevelCol )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("outlineLevelRow"),		m_oOutlineLevelRow )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("thickBottom"),			m_oThickBottom )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("thickTop"),			m_oThickTop )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zeroHeight"),			m_oZeroHeight )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBaseColWidth;
				nullable<SimpleTypes::COnOff<>>					m_oCustomHeight;
				nullable<SimpleTypes::CDouble>					m_oDefaultColWidth;
				nullable<SimpleTypes::CDouble>					m_oDefaultRowHeight;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelCol;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelRow;
				nullable<SimpleTypes::COnOff<>>					m_oThickBottom;
				nullable<SimpleTypes::COnOff<>>					m_oThickTop;
				nullable<SimpleTypes::COnOff<>>					m_oZeroHeight;
		};
		class CPane : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPane)
			CPane()
			{
			}
			virtual ~CPane()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<pane"));
				WritingStringNullableAttrString(L"activePane", m_oActivePane, m_oActivePane->ToString());
				WritingStringNullableAttrString(L"state", m_oState, m_oState->ToString());
				WritingStringNullableAttrString(L"topLeftCell", m_oTopLeftCell, m_oTopLeftCell.get());
				WritingStringNullableAttrDouble(L"xSplit", m_oXSplit, m_oXSplit->GetValue());
				WritingStringNullableAttrDouble(L"ySplit", m_oYSplit, m_oYSplit->GetValue());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Pane;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("activePane")	, m_oActivePane)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("state")		, m_oState)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("topLeftCell")	, m_oTopLeftCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("xSplit")		, m_oXSplit)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("ySplit")		, m_oYSplit)	
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
            nullable<SimpleTypes::Spreadsheet::CActivePane<>>   m_oActivePane;
            nullable<SimpleTypes::Spreadsheet::CPaneState<>>    m_oState;
            nullable<std::wstring>								m_oTopLeftCell;
            nullable<SimpleTypes::CDouble>                      m_oXSplit;
            nullable<SimpleTypes::CDouble>                      m_oYSplit;
		};

		class CSelection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSelection)
			CSelection()
			{
			}
			virtual ~CSelection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<selection"));
				WritingStringNullableAttrString(L"activeCell", m_oActiveCell, m_oActiveCell.get());
				WritingStringNullableAttrInt(L"activeCellId", m_oActiveCellId, m_oActiveCellId->GetValue());
				WritingStringNullableAttrString(L"pane", m_oPane, m_oPane->ToString());
				WritingStringNullableAttrString(L"sqref", m_oSqref, m_oSqref.get());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Selection;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("activeCell")	, m_oActiveCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("activeCellId")	, m_oActiveCellId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("sqref")		, m_oSqref)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("pane")			, m_oPane)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<std::wstring>								m_oActiveCell;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oActiveCellId;
			nullable<std::wstring>								m_oSqref;
			nullable<SimpleTypes::Spreadsheet::CActivePane<>>	m_oPane;
		};

		//необработано:
		//<extLst>
		//<pivotSelection>
		class CSheetView : public WritingElementWithChilds<CSelection>
		{
		public:
			WritingElement_AdditionConstructors(CSheetView)
			CSheetView()
			{
			}
			virtual ~CSheetView()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheetView"));
				WritingStringNullableAttrInt(L"colorId", m_oColorId, m_oColorId->GetValue());
				WritingStringNullableAttrBool(L"defaultGridColor", m_oDefaultGridColor);
				WritingStringNullableAttrBool(L"rightToLeft", m_oRightToLeft);
				WritingStringNullableAttrBool(L"showFormulas", m_oShowFormulas);
				WritingStringNullableAttrBool(L"showGridLines", m_oShowGridLines);
				WritingStringNullableAttrBool(L"showOutlineSymbols", m_oShowOutlineSymbols);
				WritingStringNullableAttrBool(L"showRowColHeaders", m_oShowRowColHeaders);
				WritingStringNullableAttrBool(L"showRuler", m_oShowRuler);
				WritingStringNullableAttrBool(L"showWhiteSpace", m_oShowWhiteSpace);
				WritingStringNullableAttrBool(L"showZeros", m_oShowZeros)
				WritingStringNullableAttrBool(L"tabSelected", m_oTabSelected);
				WritingStringNullableAttrString(L"topLeftCell", m_oTopLeftCell, m_oTopLeftCell.get());
				WritingStringNullableAttrString(L"view", m_oView, m_oView->ToString());
				WritingStringNullableAttrBool(L"windowProtection", m_oWindowProtection);
				WritingStringNullableAttrInt(L"workbookViewId", m_oWorkbookViewId, m_oWorkbookViewId->GetValue());
				WritingStringNullableAttrInt(L"zoomScale", m_oZoomScale, m_oZoomScale->GetValue());
				WritingStringNullableAttrInt(L"zoomScaleNormal", m_oZoomScaleNormal, m_oZoomScaleNormal->GetValue());
				WritingStringNullableAttrInt(L"zoomScalePageLayoutView", m_oZoomScalePageLayoutView, m_oZoomScalePageLayoutView->GetValue());
				WritingStringNullableAttrInt(L"zoomScaleSheetLayoutView", m_oZoomScaleSheetLayoutView, m_oZoomScaleSheetLayoutView->GetValue());
				writer.WriteString(_T(">"));

				if (m_oPane.IsInit())
					m_oPane->toXML(writer);
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString(_T("</sheetView>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (_T("pane") == sName)
						m_oPane = oReader;
					if (_T("selection") == sName)
					{
						m_arrItems.push_back(new CSelection(oReader));
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SheetView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("colorId"),				m_oColorId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("defaultGridColor"),	m_oDefaultGridColor)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rightToLeft"),			m_oRightToLeft)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showFormulas"),		m_oShowFormulas)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showGridLines"),		m_oShowGridLines)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showOutlineSymbols"),	m_oShowOutlineSymbols)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showRowColHeaders"),	m_oShowRowColHeaders)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showRuler"),			m_oShowRuler)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showWhiteSpace"),		m_oShowWhiteSpace)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showZeros"),			m_oShowZeros)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("tabSelected"),			m_oTabSelected)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("topLeftCell"),			m_oTopLeftCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("view"),				m_oView)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("windowProtection"),	m_oWindowProtection)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("workbookViewId"),		m_oWorkbookViewId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zoomScale"),			m_oZoomScale)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zoomScaleNormal"),		m_oZoomScaleNormal)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zoomScalePageLayoutView"),		m_oZoomScalePageLayoutView)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zoomScaleSheetLayoutView"),	m_oZoomScaleSheetLayoutView)		
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<CPane>										m_oPane;

				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oColorId;
				nullable<SimpleTypes::COnOff<>>						m_oDefaultGridColor;
				nullable<SimpleTypes::COnOff<>>						m_oRightToLeft;
				nullable<SimpleTypes::COnOff<>>						m_oShowFormulas;
				nullable<SimpleTypes::COnOff<>>						m_oShowGridLines;
				nullable<SimpleTypes::COnOff<>>						m_oShowOutlineSymbols;
				nullable<SimpleTypes::COnOff<>>						m_oShowRowColHeaders;
				nullable<SimpleTypes::COnOff<>>						m_oShowRuler;
				nullable<SimpleTypes::COnOff<>>						m_oShowWhiteSpace;
				nullable<SimpleTypes::COnOff<>>						m_oShowZeros;
				nullable<SimpleTypes::COnOff<>>						m_oTabSelected;
				nullable<std::wstring>								m_oTopLeftCell;
				nullable<SimpleTypes::Spreadsheet::CSheetViewType<>>m_oView;
				nullable<SimpleTypes::COnOff<>>						m_oWindowProtection;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWorkbookViewId;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScale;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleNormal;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScalePageLayoutView;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleSheetLayoutView;
		};

		class CSheetViews : public WritingElementWithChilds<CSheetView>
		{
		public:
			WritingElement_AdditionConstructors(CSheetViews)
			CSheetViews()
			{
			}
			virtual ~CSheetViews()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.empty()) return;

				writer.WriteString(_T("<sheetViews>"));
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}
				
				writer.WriteString(_T("</sheetViews>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("sheetView") == sName )
						m_arrItems.push_back( new CSheetView( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SheetViews;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
		};

		class CPageSetUpPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageSetUpPr)
			CPageSetUpPr()
			{
			}
			virtual ~CPageSetUpPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<pageSetUpPr"));
				WritingStringNullableAttrBool(L"autoPageBreaks", m_oAutoPageBreaks);
				WritingStringNullableAttrBool(L"fitToPage", m_oFitToPage);
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual EElementType getType () const
			{
				return et_x_PageSetUpPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("autoPageBreaks"), m_oAutoPageBreaks )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fitToPage"),	m_oFitToPage )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>		m_oAutoPageBreaks;
			nullable<SimpleTypes::COnOff<>>		m_oFitToPage;
		};
		class COutlinePr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COutlinePr)
			COutlinePr()
			{
			}
			virtual ~COutlinePr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<outlinePr"));
				WritingStringNullableAttrBool(L"applyStyles", m_oApplyStyles);
				WritingStringNullableAttrBool(L"showOutlineSymbols", m_oShowOutlineSymbols);
				WritingStringNullableAttrBool(L"summaryBelow", m_oSummaryBelow);
				WritingStringNullableAttrBool(L"summaryRight", m_oSummaryRight);
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual EElementType getType () const
			{
				return et_x_OutlinePr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("applyStyles"), m_oApplyStyles )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("showOutlineSymbols"), m_oShowOutlineSymbols )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("summaryBelow"), m_oSummaryBelow )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("summaryRight"), m_oSummaryRight )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>		m_oApplyStyles;
			nullable<SimpleTypes::COnOff<>>		m_oShowOutlineSymbols;
			nullable<SimpleTypes::COnOff<>>		m_oSummaryBelow;
			nullable<SimpleTypes::COnOff<>>		m_oSummaryRight;
		};
		class CSheetPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetPr)
			CSheetPr()
			{
			}
			virtual ~CSheetPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheetPr"));
				WritingStringNullableAttrEncodeXmlString(L"codeName", m_oCodeName, m_oCodeName.get());
				WritingStringNullableAttrBool(L"enableFormatConditionsCalculation", m_oEnableFormatConditionsCalculation);
				WritingStringNullableAttrBool(L"filterMode", m_oFilterMode);
				WritingStringNullableAttrBool(L"published", m_oPublished);
				WritingStringNullableAttrBool(L"syncHorizontal", m_oSyncHorizontal);
				WritingStringNullableAttrEncodeXmlString(L"syncRef", m_oSyncRef, m_oSyncRef.get());
				WritingStringNullableAttrBool(L"syncVertical", m_oSyncVertical);
				WritingStringNullableAttrBool(L"transitionEntry", m_oTransitionEntry);
				WritingStringNullableAttrBool(L"transitionEvaluation", m_oTransitionEvaluation);
				writer.WriteString(_T(">"));
				if (m_oTabColor.IsInit())
				{
					m_oTabColor->toXML2(writer, _T("tabColor"));
				}
				if (m_oOutlinePr.IsInit())
				{
					m_oOutlinePr->toXML(writer);
				}
				if (m_oPageSetUpPr.IsInit())
				{
					m_oPageSetUpPr->toXML(writer);
				}
				writer.WriteString(_T("</sheetPr>"));

			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("tabColor") == sName )
						m_oTabColor = oReader;
					else if ( _T("pageSetUpPr") == sName )
						m_oPageSetUpPr = oReader;
					else if ( _T("outlinePr") == sName )
						m_oOutlinePr = oReader;
				}
			}
			virtual EElementType getType () const
			{
				return et_x_SheetPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("codeName"),							m_oCodeName )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("enableFormatConditionsCalculation"),	m_oEnableFormatConditionsCalculation )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("filterMode"),						m_oFilterMode )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("published"),							m_oPublished )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("syncHorizontal"),					m_oSyncHorizontal )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("syncRef"),							m_oSyncRef )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("syncVertical"),						m_oSyncVertical )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("transitionEntry"),					m_oTransitionEntry )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("transitionEvaluation"),				m_oTransitionEvaluation )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<CColor>					m_oTabColor;
			nullable<CPageSetUpPr>				m_oPageSetUpPr;
			nullable<COutlinePr>				m_oOutlinePr;

			nullable<std::wstring>				m_oCodeName;
			nullable<SimpleTypes::COnOff<>>		m_oEnableFormatConditionsCalculation;
			nullable<SimpleTypes::COnOff<>>		m_oFilterMode;
			nullable<SimpleTypes::COnOff<>>		m_oPublished;
			nullable<SimpleTypes::COnOff<>>		m_oSyncHorizontal;
			nullable<std::wstring>				m_oSyncRef;
			nullable<SimpleTypes::COnOff<>>		m_oSyncVertical;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEntry;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEvaluation;
		};
		class CHeaderFooterElement : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooterElement)
			CHeaderFooterElement()
			{
			}
			virtual ~CHeaderFooterElement()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				if (m_sText.empty()) return;

				writer.WriteString(_T("<"));
					writer.WriteString(sName);
				writer.WriteString(_T(">"));

				writer.WriteEncodeXmlString(m_sText);
				
				writer.WriteString(_T("</"));				
					writer.WriteString(sName);
				writer.WriteString(_T(">"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText3();
			}
			virtual EElementType getType () const
			{
				return et_x_HeaderFooterElementWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			std::wstring	m_sText;
		};

		class CHeaderFooter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaderFooter)
			CHeaderFooter()
			{
			}
			virtual ~CHeaderFooter()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<headerFooter"));
				WritingStringNullableAttrBool(L"alignWithMargins", m_oAlignWithMargins);
				WritingStringNullableAttrBool(L"differentFirst", m_oDifferentFirst);
				WritingStringNullableAttrBool(L"differentOddEven", m_oDifferentOddEven);
				WritingStringNullableAttrBool(L"scaleWithDoc", m_oScaleWithDoc);
				writer.WriteString(_T(">"));
					if (m_oOddHeader.IsInit())
					{
						m_oOddHeader->toXML2(writer, _T("oddHeader"));
					}
					if (m_oOddFooter.IsInit())
					{
						m_oOddFooter->toXML2(writer, _T("oddFooter"));
					}
					if (m_oEvenHeader.IsInit())
					{
						m_oEvenHeader->toXML2(writer, _T("evenHeader"));
					}					
					if (m_oEvenFooter.IsInit())
					{
						m_oEvenFooter->toXML2(writer, _T("evenFooter"));
					}
					if (m_oFirstHeader.IsInit())
					{
						m_oFirstHeader->toXML2(writer, _T("firstHeader"));
					}
					if (m_oFirstFooter.IsInit())
					{
						m_oFirstFooter->toXML2(writer, _T("firstFooter"));
					}
				writer.WriteString(_T("</headerFooter>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("evenFooter") == sName )
						m_oEvenFooter = oReader;
					else if ( _T("evenHeader") == sName )
						m_oEvenHeader = oReader;
					else if ( _T("firstFooter") == sName )
						m_oFirstFooter = oReader;
					else if ( _T("firstHeader") == sName )
						m_oFirstHeader = oReader;
					else if ( _T("oddFooter") == sName )
						m_oOddFooter = oReader;
					else if ( _T("oddHeader") == sName )
						m_oOddHeader = oReader;
				}
			}
			virtual EElementType getType () const
			{
				return et_x_HeaderFooterWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("alignWithMargins"),	m_oAlignWithMargins)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("differentFirst"),	m_oDifferentFirst)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("differentOddEven"),	m_oDifferentOddEven)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("scaleWithDoc"),		m_oScaleWithDoc)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<CHeaderFooterElement>		m_oEvenFooter;
			nullable<CHeaderFooterElement>		m_oEvenHeader;
			nullable<CHeaderFooterElement>		m_oFirstFooter;
			nullable<CHeaderFooterElement>		m_oFirstHeader;
			nullable<CHeaderFooterElement>		m_oOddFooter;
			nullable<CHeaderFooterElement>		m_oOddHeader;

			nullable<SimpleTypes::COnOff<>>		m_oAlignWithMargins;
			nullable<SimpleTypes::COnOff<>>		m_oDifferentFirst;
			nullable<SimpleTypes::COnOff<>>		m_oDifferentOddEven;
			nullable<SimpleTypes::COnOff<>>		m_oScaleWithDoc;

		};

		class CLegacyDrawingHFWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLegacyDrawingHFWorksheet)
			CLegacyDrawingHFWorksheet()
			{
			}
			virtual ~CLegacyDrawingHFWorksheet()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring  toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(!m_oId.IsInit()) return;

				writer.WriteString(L"<legacyDrawingHF");
				WritingStringAttrString(L"r:id", m_oId->ToString());
				WritingStringNullableAttrInt(L"cfe", m_oCfe, m_oCfe->GetValue());
				WritingStringNullableAttrInt(L"cff", m_oCff, m_oCff->GetValue());
				WritingStringNullableAttrInt(L"cfo", m_oCfo, m_oCfo->GetValue());
				WritingStringNullableAttrInt(L"che", m_oChe, m_oChe->GetValue());
				WritingStringNullableAttrInt(L"chf", m_oChf, m_oChf->GetValue());
				WritingStringNullableAttrInt(L"cho", m_oCho, m_oCho->GetValue());
				WritingStringNullableAttrInt(L"lfe", m_oLfe, m_oLfe->GetValue());
				WritingStringNullableAttrInt(L"lff", m_oLff, m_oLff->GetValue());
				WritingStringNullableAttrInt(L"lfo", m_oLfo, m_oLfo->GetValue());
				WritingStringNullableAttrInt(L"lhe", m_oLhe, m_oLhe->GetValue());
				WritingStringNullableAttrInt(L"lLhf", m_oLhf, m_oLhf->GetValue());
				WritingStringNullableAttrInt(L"lho", m_oLho, m_oLho->GetValue());
				WritingStringNullableAttrInt(L"rfe", m_oRfe, m_oRfe->GetValue());
				WritingStringNullableAttrInt(L"rff", m_oRff, m_oRff->GetValue());
				WritingStringNullableAttrInt(L"rfo", m_oRfo, m_oRfo->GetValue());
				WritingStringNullableAttrInt(L"rhe", m_oRhe, m_oRhe->GetValue());
				WritingStringNullableAttrInt(L"rhf", m_oRhf, m_oRhf->GetValue());
				WritingStringNullableAttrInt(L"rho", m_oRho, m_oRho->GetValue());
				writer.WriteString(L"/>");

			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_LegacyDrawingHFWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("r:id"),    m_oId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cfe"),     m_oCfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cff"),     m_oCff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cfo"),     m_oCfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("che"),		m_oChe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("chf"),     m_oChf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cho"),     m_oCho )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lfe"),     m_oLfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lff"),     m_oLff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lfo"),     m_oLfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lhe"),     m_oLhe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lhf"),     m_oLhf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lho"),     m_oLho )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rfe"),     m_oRfe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rff"),     m_oRff )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rfo"),     m_oRfo )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rhe"),     m_oRhe )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rhf"),     m_oRhf )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rho"),     m_oRho )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId >				m_oId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oChe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oChf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCho;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oLho;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRfe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRfo;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRhe;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRhf;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRho;
		};

		class CPictureWorksheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPictureWorksheet)
			CPictureWorksheet()
			{
			}
			virtual ~CPictureWorksheet()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring  toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oId.IsInit())
				{
					writer.WriteString(L"<picture");
					WritingStringAttrString(L"r:id", m_oId->ToString());
					writer.WriteString(L"/>");
				}
				
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_PictureWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("r:id"), m_oId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

		class CBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBreak)
			CBreak()
			{
			}
			virtual ~CBreak()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<brk"));
				WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
				WritingStringNullableAttrBool(L"man", m_oMan);
				WritingStringNullableAttrInt(L"max", m_oMax, m_oMax->GetValue());
				WritingStringNullableAttrInt(L"min", m_oMin, m_oMin->GetValue());
				WritingStringNullableAttrBool(L"pt", m_oPt);
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual EElementType getType () const
			{
				return et_x_Break;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),	m_oId)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("man"),	m_oMan)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("max"),	m_oMax)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("min"),	m_oMin)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("pt"),	m_oPt)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oId;
			nullable<SimpleTypes::COnOff<>>					m_oMan;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMin;
			nullable<SimpleTypes::COnOff<>>					m_oPt;
		};

		class CRowColBreaks : public WritingElementWithChilds<CBreak>
		{
		public:
			WritingElement_AdditionConstructors(CRowColBreaks)
			CRowColBreaks()
			{
			}
			virtual ~CRowColBreaks()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXML2(writer, L"rowBreaks");
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				writer.WriteString(L"<");
				writer.WriteString(sName);
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				WritingStringNullableAttrInt(L"manualBreakCount", m_oManualBreakCount, m_oManualBreakCount->GetValue());
				writer.WriteString(L">");
				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if ( m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</");
				writer.WriteString(sName);
				writer.WriteString(L">");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"brk" == sName )
						m_arrItems.push_back( new CBreak( oReader ));
				}
			}
			virtual EElementType getType () const
			{
				return et_x_RowColBreaks;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"count",	m_oCount)
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"manualBreakCount",	m_oManualBreakCount)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oManualBreakCount;
		};
		class CSheetProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheetProtection)
			CSheetProtection()
			{
			}
			virtual ~CSheetProtection()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetProtection");
					WritingStringNullableAttrString(L"password",	m_oPassword,		m_oPassword.get());
					WritingStringNullableAttrString(L"algorithmName",m_oAlgorithmName,	m_oAlgorithmName->ToString());
					WritingStringNullableAttrString(L"hashValue",	m_oHashValue,		m_oHashValue.get());
					WritingStringNullableAttrString(L"saltValue",	m_oSaltValue,		m_oSaltValue.get());
					WritingStringNullableAttrInt(L"spinCount",		m_oSpinCount,		m_oSpinCount->GetValue());
					WritingStringNullableAttrInt(L"autoFilter",		m_oAutoFilter,		m_oAutoFilter->GetValue());
					WritingStringNullableAttrInt(L"content",		m_oContent,			m_oContent->GetValue());
					WritingStringNullableAttrInt(L"deleteColumns",	m_oDeleteColumns,	m_oDeleteColumns->GetValue());
					WritingStringNullableAttrInt(L"deleteRows",		m_oDeleteRows,		m_oDeleteRows->GetValue());
					WritingStringNullableAttrInt(L"formatCells",	m_oFormatCells,		m_oFormatCells->GetValue());
					WritingStringNullableAttrInt(L"formatColumns",	m_oFormatColumns,	m_oFormatColumns->GetValue());
					WritingStringNullableAttrInt(L"formatRows",		m_oFormatRows,		m_oFormatRows->GetValue());
					WritingStringNullableAttrInt(L"insertColumns",	m_oInsertColumns,	m_oInsertColumns->GetValue());
					WritingStringNullableAttrInt(L"insertHyperlinks", m_oInsertHyperlinks, m_oInsertHyperlinks->GetValue());
					WritingStringNullableAttrInt(L"insertRows",		m_oInsertRows,		m_oInsertRows->GetValue());
					WritingStringNullableAttrInt(L"objects",		m_oObjects,			m_oObjects->GetValue());
					WritingStringNullableAttrInt(L"pivotTables",	m_oPivotTables,		m_oPivotTables->GetValue());
					WritingStringNullableAttrInt(L"scenarios",		m_oScenarios,		m_oScenarios->GetValue());
					WritingStringNullableAttrInt(L"selectLockedCells",	m_oSelectLockedCells,	m_oSelectLockedCells->GetValue());
					WritingStringNullableAttrInt(L"selectUnlockedCell",	m_oSelectUnlockedCell,	m_oSelectUnlockedCell->GetValue());
					WritingStringNullableAttrInt(L"sheet",			m_oSheet,			m_oSheet->GetValue());
					WritingStringNullableAttrInt(L"sort",			m_oSort,			m_oSort->GetValue());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_SheetProtection;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("password"),		m_oPassword)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("algorithmName"),	m_oAlgorithmName)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("hashValue"),		m_oHashValue)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("saltValue"),		m_oSaltValue)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("spinCount"),		m_oSpinCount)
					
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("autoFilter"),	m_oAutoFilter)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("content"),		m_oContent)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("deleteColumns"),	m_oDeleteColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("deleteRows"),	m_oDeleteRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("formatCells"),	m_oFormatCells)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("formatColumns"),	m_oFormatColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("formatRows"),	m_oFormatRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("insertColumns"),	m_oInsertColumns)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("insertHyperlinks"),m_oInsertHyperlinks)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("insertRows"),	m_oInsertRows)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("objects"),		m_oObjects)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("pivotTables"),	m_oPivotTables)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("scenarios"),		m_oScenarios)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("selectLockedCells"),m_oSelectLockedCells)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("selectUnlockedCell"),m_oSelectUnlockedCell)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("sheet"),			m_oSheet)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("sort"),			m_oSort)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::CCryptAlgoritmName<>>		m_oAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oSpinCount;
			nullable_string		m_oHashValue;
			nullable_string		m_oSaltValue;
			nullable_string		m_oPassword; //for old wrike protection

			nullable<SimpleTypes::COnOff<>>		m_oAutoFilter;
			nullable<SimpleTypes::COnOff<>>		m_oContent;
			nullable<SimpleTypes::COnOff<>>		m_oDeleteColumns;
			nullable<SimpleTypes::COnOff<>>		m_oDeleteRows;
			nullable<SimpleTypes::COnOff<>>		m_oFormatCells;
			nullable<SimpleTypes::COnOff<>>		m_oFormatColumns;
			nullable<SimpleTypes::COnOff<>>		m_oFormatRows;
			nullable<SimpleTypes::COnOff<>>		m_oInsertColumns;
			nullable<SimpleTypes::COnOff<>>		m_oInsertHyperlinks;
			nullable<SimpleTypes::COnOff<>>		m_oInsertRows;
			nullable<SimpleTypes::COnOff<>>		m_oObjects;
			nullable<SimpleTypes::COnOff<>>		m_oPivotTables;
			nullable<SimpleTypes::COnOff<>>		m_oScenarios;
			nullable<SimpleTypes::COnOff<>>		m_oSelectLockedCells;
			nullable<SimpleTypes::COnOff<>>		m_oSelectUnlockedCell;
			nullable<SimpleTypes::COnOff<>>		m_oSheet;
			nullable<SimpleTypes::COnOff<>>		m_oSort;
		};
		class CDataRef : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataRef)
			CDataRef()
			{
			}
			virtual ~CDataRef()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<sheetProtection");
					WritingStringAttrString(L"r:id", m_oId->ToString());
					WritingStringNullableAttrString(L"name",	m_oName,	m_oName.get());
					WritingStringNullableAttrString(L"ref",		m_oRef,		m_oRef.get());
					WritingStringNullableAttrString(L"sheet",	m_oSheet,	m_oSheet.get());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_DataRef;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("r:id"),		m_oId)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("name"),		m_oName)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("ref"),		m_oRef)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("sheet"),		m_oSheet)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::CRelationshipId>	m_oId;
			nullable_string							m_oName;
			nullable_string							m_oRef;
			nullable_string							m_oSheet;
		};
		class CDataRefs : public WritingElementWithChilds<CDataRef>
		{
		public:
			WritingElement_AdditionConstructors(CDataRefs)
			CDataRefs()
			{
			}
			virtual ~CDataRefs()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<dataRefs");
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(L">");
				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if ( m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}
				writer.WriteString(L"</dataRefs>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"dataRef" == sName )
						m_arrItems.push_back( new CDataRef( oReader ));
				}
			}
			virtual EElementType getType () const
			{
				return et_x_DataRefs;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"count",	m_oCount)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
		};
		class CDataConsolidate : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataConsolidate)
			CDataConsolidate()
			{
			}
			virtual ~CDataConsolidate()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<dataConsolidate");
					WritingStringNullableAttrString(L"function",	m_oFunction,	m_oFunction->ToString());
					WritingStringNullableAttrInt(L"link",			m_oLink,		m_oLink->GetValue());
					WritingStringNullableAttrInt(L"startLabels",	m_oStartLabels,	m_oStartLabels->GetValue());
					WritingStringNullableAttrInt(L"topLabels",		m_oTopLabels,	m_oTopLabels->GetValue());
				writer.WriteString(L">");

				writer.WriteString(L"</dataConsolidate>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"dataRefs" == sName )
						m_oDataRefs = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_DataConsolidate;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("function"),		m_oFunction)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("link"),			m_oLink)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("startLabels"),	m_oStartLabels)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("topLabels"),		m_oTopLabels)
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<SimpleTypes::Spreadsheet::CDataConsolidateFunction<>>	m_oFunction;
			nullable<SimpleTypes::COnOff<>>									m_oLink; 
			nullable<SimpleTypes::COnOff<>>									m_oStartLabels;
			nullable<SimpleTypes::COnOff<>>									m_oTopLabels;

			nullable<CDataRefs>	m_oDataRefs;

		};
	} //Spreadsheet
} // namespace OOX
