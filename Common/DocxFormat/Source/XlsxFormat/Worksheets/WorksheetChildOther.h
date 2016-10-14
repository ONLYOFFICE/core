/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_WORKSHEETCHILDSOTHER_FILE_INCLUDE_H_
#define OOX_WORKSHEETCHILDSOTHER_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPageMargins : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPageMargins)
			CPageMargins()
			{
			}
			virtual ~CPageMargins()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oLeft.IsInit() || m_oTop.IsInit() || m_oRight.IsInit() || m_oBottom.IsInit() || m_oHeader.IsInit() || m_oFooter.IsInit())
				{
					writer.WriteString(CString(_T("<pageMargins")));
					if(m_oLeft.IsInit())
					{
						CString sLeft;
						sLeft.Format(_T(" left=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oLeft->ToInches()));
						writer.WriteString(sLeft);
					}
					if(m_oRight.IsInit())
					{
						CString sRight;
						sRight.Format(_T(" right=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oRight->ToInches()));
						writer.WriteString(sRight);
					}
					if(m_oTop.IsInit())
					{
						CString sTop;
						sTop.Format(_T(" top=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oTop->ToInches()));
						writer.WriteString(sTop);
					}
					if(m_oBottom.IsInit())
					{
						CString sBottom;
						sBottom.Format(_T(" bottom=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oBottom->ToInches()));
						writer.WriteString(sBottom);
					}
					if(m_oHeader.IsInit())
					{
						CString sHeader;
						sHeader.Format(_T(" header=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oHeader->ToInches()));
						writer.WriteString(sHeader);
					}
					if(m_oFooter.IsInit())
					{
						CString sFooter;
						sFooter.Format(_T(" footer=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oFooter->ToInches()));
						writer.WriteString(sFooter);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_PageMargins;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("left"),      m_oLeft)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("top"),      m_oTop)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("right"),      m_oRight)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("bottom"),      m_oBottom)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("header"),      m_oHeader)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("footer"),      m_oFooter)

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
			WritingElementSpreadsheet_AdditionConstructors(CPageSetup)
			CPageSetup()
			{
			}
			virtual ~CPageSetup()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oPaperSize.IsInit() || m_oOrientation.IsInit())
				{
					writer.WriteString(CString(_T("<pageSetup")));
					if(m_oPaperSize.IsInit())
					{
						CString sPaperSize;
						sPaperSize.Format(_T(" paperSize=\"%ls\""), m_oPaperSize->ToString());
						writer.WriteString(sPaperSize);
					}
					if(m_oOrientation.IsInit())
					{
						CString sOrientation;
						sOrientation.Format(_T(" orientation=\"%ls\""), m_oOrientation->ToString());
						writer.WriteString(sOrientation);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_PageSetup;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("orientation"),      m_oOrientation)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("paperSize"),      m_oPaperSize)

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CPageOrientation<>>	m_oOrientation;
			nullable<SimpleTypes::Spreadsheet::CPageSize<>>	m_oPaperSize;
		};
		class CPrintOptions : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPrintOptions)
			CPrintOptions()
			{
			}
			virtual ~CPrintOptions()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oGridLines.IsInit() || m_oGridLinesSet.IsInit() || m_oHeadings.IsInit())
				{
					writer.WriteString(CString(_T("<printOptions")));
					if(m_oHeadings.IsInit())
					{
						CString sHeadings;
						sHeadings.Format(_T(" headings=\"%ls\""), m_oHeadings->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sHeadings);
					}
					if(m_oGridLines.IsInit())
					{
						CString sGridLines;
						sGridLines.Format(_T(" gridLines=\"%ls\""), m_oGridLines->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sGridLines);
					}
					if(m_oGridLinesSet.IsInit())
					{
						CString sGridLinesSet;
						sGridLinesSet.Format(_T(" gridLinesSet=\"%ls\""), m_oGridLinesSet->ToString2(SimpleTypes::onofftostring1));
						writer.WriteString(sGridLinesSet);
					}
					writer.WriteString(CString(_T("/>")));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_PrintOptions;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("gridLines"),      m_oGridLines)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("gridLinesSet"),      m_oGridLinesSet)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("headings"),      m_oHeadings)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("horizontalCentered"),      m_oHorizontalCentered)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("verticalCentered"),      m_oVerticalCentered)

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
			WritingElementSpreadsheet_AdditionConstructors(CDimension)
			CDimension()
			{
			}
			virtual ~CDimension()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Dimension;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef)

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<CString>	m_oRef;
		};

		class CSheetFormatPr : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSheetFormatPr)
			CSheetFormatPr()
			{
			}
			virtual ~CSheetFormatPr()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<sheetFormatPr"));
				if(m_oBaseColWidth.IsInit())
				{
					CString sVal; sVal.Format(_T(" baseColWidth=\"%d\""), m_oBaseColWidth->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oCustomHeight.IsInit())
				{
					CString sVal; sVal.Format(_T(" customHeight=\"%ls\""), m_oCustomHeight->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oDefaultColWidth.IsInit())
				{
					CString sVal; sVal.Format(_T(" defaultColWidth=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oDefaultColWidth->GetValue()));
					writer.WriteString(sVal);
				}
				if(m_oDefaultRowHeight.IsInit())
				{
					CString sVal; sVal.Format(_T(" defaultRowHeight=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oDefaultRowHeight->GetValue()));
					writer.WriteString(sVal);
				}
				if(m_oOutlineLevelCol.IsInit())
				{
					CString sVal; sVal.Format(_T(" outlineLevelCol=\"%d\""), m_oOutlineLevelCol->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oOutlineLevelRow.IsInit())
				{
					CString sVal; sVal.Format(_T(" outlineLevelRow=\"%d\""), m_oOutlineLevelRow->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oThickBottom.IsInit())
				{
					CString sVal; sVal.Format(_T(" thickBottom=\"%ls\""), m_oThickBottom->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oThickTop.IsInit())
				{
					CString sVal; sVal.Format(_T(" thickTop=\"%ls\""), m_oThickTop->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oZeroHeight.IsInit())
				{
					CString sVal; sVal.Format(_T(" zeroHeight=\"%ls\""), m_oZeroHeight->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}

				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_SheetFormatPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("baseColWidth"),      m_oBaseColWidth)	// ToDo Excel не воспринимает значения не uint (мы приводим к uint)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customHeight"),      m_oCustomHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultColWidth"),      m_oDefaultColWidth )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultRowHeight"),      m_oDefaultRowHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outlineLevelCol"),      m_oOutlineLevelCol )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outlineLevelRow"),      m_oOutlineLevelRow )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("thickBottom"),      m_oThickBottom )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("thickTop"),      m_oThickTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("zeroHeight"),      m_oZeroHeight )

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
			WritingElementSpreadsheet_AdditionConstructors(CPane)
			CPane()
			{
			}
			virtual ~CPane()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<pane"));
				if (m_oActivePane.IsInit())
				{
					writer.WriteString(L" activePane=\"");
					writer.WriteString(m_oActivePane->ToString());
					writer.WriteString(L"\"");
				}
				if (m_oState.IsInit())
				{
					writer.WriteString(L" state=\"");
					writer.WriteString(m_oState->ToString());
					writer.WriteString(L"\"");
				}
				if (m_oTopLeftCell.IsInit())
				{
					CString sVal; sVal.Format(_T(" topLeftCell=\"%ls\""), m_oTopLeftCell.get());
					writer.WriteString(sVal);
				}
				if (m_oXSplit.IsInit())
				{
					CString sVal; sVal.Format(_T(" xSplit=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oXSplit->GetValue()));
					writer.WriteString(sVal);
				}
				if (m_oYSplit.IsInit())
				{
					CString sVal; sVal.Format(_T(" ySplit=\"%ls\""), SpreadsheetCommon::WriteDouble(m_oYSplit->GetValue()));
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Pane;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("activePane")	, m_oActivePane)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("state")		, m_oState)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("topLeftCell")	, m_oTopLeftCell)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("xSplit")		, m_oXSplit)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("ySplit")		, m_oYSplit)	

				WritingElement_ReadAttributes_End( oReader )
			}

		public:
            nullable<SimpleTypes::Spreadsheet::CActivePane<>>   m_oActivePane;
            nullable<SimpleTypes::Spreadsheet::CPaneState<>>    m_oState;
            nullable<CString>                                   m_oTopLeftCell;
            nullable<SimpleTypes::CDouble>                      m_oXSplit;
            nullable<SimpleTypes::CDouble>                      m_oYSplit;
		};

		class CSelection : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSelection)
			CSelection()
			{
			}
			virtual ~CSelection()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<selection"));
				if (m_oActiveCell.IsInit())
				{
					CString sVal; sVal.Format(_T(" activeCell=\"%ls\""), m_oActiveCell.get());
					writer.WriteString(sVal);
				}
				if (m_oActiveCellId.IsInit())
				{
					CString sVal; sVal.Format(_T(" activeCellId=\"%d\""), m_oActiveCellId->GetValue());
					writer.WriteString(sVal);
				}
				if (m_oSqref.IsInit())
				{
					CString sVal; sVal.Format(_T(" sqref=\"%ls\""), m_oSqref.get());
					writer.WriteString(sVal);
				}
				if (m_oPane.IsInit())
				{
					writer.WriteString(L" pane=\"");
					writer.WriteString(m_oPane->ToString());
					writer.WriteString(L"\"");
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_Selection;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("activeCell")	, m_oActiveCell)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("activeCellId")	, m_oActiveCellId)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("sqref")		, m_oSqref)
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("pane")			, m_oPane)

				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<CString>									m_oActiveCell;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oActiveCellId;
			nullable<CString>									m_oSqref;
			nullable<SimpleTypes::Spreadsheet::CActivePane<>>	m_oPane;
		};

		//необработано:
		//<extLst>
		//<pivotSelection>
		class CSheetView : public WritingElementWithChilds<CSelection>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSheetView)
			CSheetView()
			{
			}
			virtual ~CSheetView()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<sheetView"));
				if(m_oColorId.IsInit())
				{
					CString sVal; sVal.Format(_T(" colorId=\"%d\""), m_oColorId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oDefaultGridColor.IsInit())
				{
					CString sVal; sVal.Format(_T(" defaultGridColor=\"%ls\""), m_oDefaultGridColor->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oRightToLeft.IsInit())
				{
					CString sVal; sVal.Format(_T(" rightToLeft=\"%ls\""), m_oRightToLeft->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowFormulas.IsInit())
				{
					CString sVal; sVal.Format(_T(" showFormulas=\"%ls\""), m_oShowFormulas->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowGridLines.IsInit())
				{
					CString sVal; sVal.Format(_T(" showGridLines=\"%ls\""), m_oShowGridLines->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowOutlineSymbols.IsInit())
				{
					CString sVal; sVal.Format(_T(" showOutlineSymbols=\"%ls\""), m_oShowOutlineSymbols->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowRowColHeaders.IsInit())
				{
					CString sVal; sVal.Format(_T(" showRowColHeaders=\"%ls\""), m_oShowRowColHeaders->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowRuler.IsInit())
				{
					CString sVal; sVal.Format(_T(" showRuler=\"%ls\""), m_oShowRuler->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowWhiteSpace.IsInit())
				{
					CString sVal; sVal.Format(_T(" showWhiteSpace=\"%ls\""), m_oShowWhiteSpace->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oShowZeros.IsInit())
				{
					CString sVal; sVal.Format(_T(" showZeros=\"%ls\""), m_oShowZeros->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oTabSelected.IsInit())
				{
					CString sVal; sVal.Format(_T(" tabSelected=\"%ls\""), m_oTabSelected->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oTopLeftCell.IsInit())
				{
					CString sVal; sVal.Format(_T(" topLeftCell=\"%ls\""), m_oTopLeftCell.get());
					writer.WriteString(sVal);
				}
				if(m_oView.IsInit())
				{
					CString sVal; sVal.Format(_T(" view=\"%ls\""), m_oView->ToString());
					writer.WriteString(sVal);
				}
				if(m_oWindowProtection.IsInit())
				{
					CString sVal; sVal.Format(_T(" windowProtection=\"%ls\""), m_oWindowProtection->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(sVal);
				}
				if(m_oWorkbookViewId.IsInit())
				{
					CString sVal; sVal.Format(_T(" workbookViewId=\"%d\""), m_oWorkbookViewId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oZoomScale.IsInit())
				{
					CString sVal; sVal.Format(_T(" zoomScale=\"%d\""), m_oZoomScale->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oZoomScaleNormal.IsInit())
				{
					CString sVal; sVal.Format(_T(" zoomScaleNormal=\"%d\""), m_oZoomScaleNormal->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oZoomScalePageLayoutView.IsInit())
				{
					CString sVal; sVal.Format(_T(" zoomScalePageLayoutView=\"%d\""), m_oZoomScalePageLayoutView->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oZoomScaleSheetLayoutView.IsInit())
				{
					CString sVal; sVal.Format(_T(" zoomScaleSheetLayoutView=\"%d\""), m_oZoomScaleSheetLayoutView->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));

				if (m_oPane.IsInit())
					m_oPane->toXML(writer);
				
				for(size_t i = 0 ; i < m_arrItems.size(); ++i)
				{
					m_arrItems[i]->toXML(writer);
				}

				writer.WriteString(_T("</sheetView>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

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
				return et_SheetView;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("colorId"),      m_oColorId)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultGridColor"),      m_oDefaultGridColor)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rightToLeft"),      m_oRightToLeft)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showFormulas"),      m_oShowFormulas)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showGridLines"),      m_oShowGridLines)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showOutlineSymbols"),      m_oShowOutlineSymbols)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showRowColHeaders"),      m_oShowRowColHeaders)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showRuler"),      m_oShowRuler)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showWhiteSpace"),      m_oShowWhiteSpace)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showZeros"),      m_oShowZeros)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tabSelected"),      m_oTabSelected)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("topLeftCell"),      m_oTopLeftCell)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("view"),      m_oView)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("windowProtection"),      m_oWindowProtection)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("workbookViewId"),      m_oWorkbookViewId)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("zoomScale"),      m_oZoomScale)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("zoomScaleNormal"),      m_oZoomScaleNormal)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("zoomScalePageLayoutView"),      m_oZoomScalePageLayoutView)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("zoomScaleSheetLayoutView"),      m_oZoomScaleSheetLayoutView)		

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
				nullable<CString>									m_oTopLeftCell;
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
			WritingElementSpreadsheet_AdditionConstructors(CSheetViews)
			CSheetViews()
			{
			}
			virtual ~CSheetViews()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(_T("<sheetViews>"));
					for( unsigned int i = 0, length = m_arrItems.size();  i< length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</sheetViews>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("sheetView") == sName )
						m_arrItems.push_back( new CSheetView( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_SheetViews;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
		};

		class CSheetPr : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSheetPr)
			CSheetPr()
			{
			}
			virtual ~CSheetPr()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (m_oTabColor.IsInit())
				{
					writer.WriteString(_T("<sheetPr>"));
					m_oTabColor->toXML2(writer, _T("tabColor"));
					writer.WriteString(_T("</sheetPr>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("tabColor") == sName )
						m_oTabColor = oReader;
					//необработано:
					//<outlinePr>
					//<pageSetUpPr>
				}
			}
			virtual EElementType getType () const
			{
				return et_SheetPr;
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

			nullable<CString>					m_oCodeName;
			nullable<SimpleTypes::COnOff<>>		m_oEnableFormatConditionsCalculation;
			nullable<SimpleTypes::COnOff<>>		m_oFilterMode;
			nullable<SimpleTypes::COnOff<>>		m_oPublished;
			nullable<SimpleTypes::COnOff<>>		m_oSyncHorizontal;
			nullable<CString>					m_oSyncRef;
			nullable<SimpleTypes::COnOff<>>		m_oSyncVertical;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEntry;
			nullable<SimpleTypes::COnOff<>>		m_oTransitionEvaluation;
		};
		class CHeaderFooterElement : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CHeaderFooterElement)
			CHeaderFooterElement()
			{
			}
			virtual ~CHeaderFooterElement()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			void toXML2(XmlUtils::CStringWriter& writer, CString sName) const
			{
				if (m_sText.IsEmpty())			return;

				writer.WriteString(_T("<"));
					writer.WriteString(sName);
				writer.WriteString(_T(">"));

				writer.WriteString(m_sText);
				
				writer.WriteString(_T("</"));				
					writer.WriteString(sName);
				writer.WriteString(_T(">"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual EElementType getType () const
			{
				return et_HeaderFooterElementWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			CString	m_sText;
		};

		class CHeaderFooter : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CHeaderFooter)
			CHeaderFooter()
			{
			}
			virtual ~CHeaderFooter()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<headerFooter"));
					if(m_oAlignWithMargins.IsInit())
					{
						CString sVal; sVal.Format(_T(" alignWithMargins=\"%ls\""), m_oAlignWithMargins->ToString());
						writer.WriteString(sVal);
					}
					if(m_oDifferentFirst.IsInit())
					{
						CString sVal; sVal.Format(_T(" differentFirst=\"%ls\""), m_oDifferentFirst->ToString());
						writer.WriteString(sVal);
					}
					if(m_oDifferentOddEven.IsInit())
					{
						CString sVal; sVal.Format(_T(" differentOddEven=\"%ls\""), m_oDifferentOddEven->ToString());
						writer.WriteString(sVal);
					}
					if(m_oScaleWithDoc.IsInit())
					{
						CString sVal; sVal.Format(_T(" scaleWithDoc=\"%ls\""), m_oScaleWithDoc->ToString());
						writer.WriteString(sVal);
					}
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
					if (m_oFirstFooter.IsInit())
					{
						m_oFirstFooter->toXML2(writer, _T("firstFooter"));
					}
					if (m_oFirstHeader.IsInit())
					{
						m_oFirstHeader->toXML2(writer, _T("firstHeader"));
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

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
				return et_HeaderFooterWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("alignWithMargins"),	m_oAlignWithMargins)
				WritingElement_ReadAttributes_Read_if		( oReader, _T("differentFirst"),	m_oDifferentFirst)
				WritingElement_ReadAttributes_Read_if		( oReader, _T("differentOddEven"),	m_oDifferentOddEven)
				WritingElement_ReadAttributes_Read_if		( oReader, _T("scaleWithDoc"),		m_oScaleWithDoc)
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
			WritingElementSpreadsheet_AdditionConstructors(CLegacyDrawingHFWorksheet)
			CLegacyDrawingHFWorksheet()
			{
			}
			virtual ~CLegacyDrawingHFWorksheet()
			{
			}

		public:
			virtual CString  toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(!m_oId.IsInit()) return;

				CString sVal = _T("<legacyDrawingHF r:id=\"") + m_oId->GetValue() ;

				if(m_oCfe.IsInit())
				{
					CString sVal; sVal.Format(_T(" cfe=\"%d\""), m_oCfe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oCff.IsInit())
				{
					CString sVal; sVal.Format(_T(" cff=\"%d\""), m_oCff->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oCfo.IsInit())
				{
					CString sVal; sVal.Format(_T(" cfo=\"%d\""), m_oCfo->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oChe.IsInit())
				{
					CString sVal; sVal.Format(_T(" che=\"%d\""), m_oChe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oChf.IsInit())
				{
					CString sVal; sVal.Format(_T(" chf=\"%d\""), m_oChf->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oCho.IsInit())
				{
					CString sVal; sVal.Format(_T(" cho=\"%d\""), m_oCho->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLfe.IsInit())
				{
					CString sVal; sVal.Format(_T(" lfe=\"%d\""), m_oLfe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLff.IsInit())
				{
					CString sVal; sVal.Format(_T(" lff=\"%d\""), m_oLff->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLhe.IsInit())
				{
					CString sVal; sVal.Format(_T(" lhe=\"%d\""), m_oLhe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLhf.IsInit())
				{
					CString sVal; sVal.Format(_T(" lLhf=\"%d\""), m_oLhf->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLho.IsInit())
				{
					CString sVal; sVal.Format(_T(" lho=\"%d\""), m_oLho->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRfe.IsInit())
				{
					CString sVal; sVal.Format(_T(" rfe=\"%d\""), m_oRfe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRff.IsInit())
				{
					CString sVal; sVal.Format(_T(" rff=\"%d\""), m_oRff->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRfo.IsInit())
				{
					CString sVal; sVal.Format(_T(" rfo=\"%d\""), m_oRfo->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRhe.IsInit())
				{
					CString sVal; sVal.Format(_T(" rhe=\"%d\""), m_oRhe->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRhf.IsInit())
				{
					CString sVal; sVal.Format(_T(" rhf=\"%d\""), m_oRhf->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oRho.IsInit())
				{
					CString sVal; sVal.Format(_T(" rho=\"%d\""), m_oRho->GetValue());
					writer.WriteString(sVal);
				}
				
				sVal += _T("\"/>");
				writer.WriteString(sVal);

			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_LegacyDrawingHFWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),    m_oId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cfe"),     m_oCfe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cff"),     m_oCff )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cfo"),     m_oCfo )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("che"),		m_oChe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("chf"),     m_oChf )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cho"),     m_oCho )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("lfe"),     m_oLfe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("lff"),     m_oLff )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("lhe"),     m_oLhe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("lhf"),     m_oLhf )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("lho"),     m_oLho )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rfe"),     m_oRfe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rff"),     m_oRff )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rfo"),     m_oRfo )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rhe"),     m_oRhe )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rhf"),     m_oRhf )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rho"),     m_oRho )

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
			WritingElementSpreadsheet_AdditionConstructors(CPictureWorksheet)
			CPictureWorksheet()
			{
			}
			virtual ~CPictureWorksheet()
			{
			}

		public:
			virtual CString  toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oId.IsInit())
				{
					CString sVal = _T("<picture r:id=\"") + m_oId->GetValue() + _T("\"/>");
					writer.WriteString(sVal);
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
				return et_PictureWorksheet;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oId;
		};

	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKSHEETCHILDSOTHER_FILE_INCLUDE_H_
