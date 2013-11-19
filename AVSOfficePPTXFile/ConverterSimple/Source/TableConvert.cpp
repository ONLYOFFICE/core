#include "stdafx.h"
#include "./../Converter.h"
#include "../../PPTXFormat/Logic/Table/Table.h"
#include "../../PPTXFormat/TableStyles.h"
#include "../../PPTXFormat/Theme.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"
#include "../../PPTXFormat/Logic/TcTxStyle.h"

#include "gdiplus.h"
#pragma comment(lib, "gdiplus.lib")

namespace PPTX2EditorSimple
{
	void Converter::TableConvert(const PPTX::Logic::GraphicFrame& pOle, NSPresentationEditor::CSlide& pEContainer)
	{
		//boost::shared_ptr<PPTX::Presentation> Presentation = boost::shared_dynamic_cast<PPTX::Presentation, OOX::File>(m_Folder.get(OOX::FileTypes::Presentation));
		smart_ptr<PPTX::TableStyles> StylesFile = pOle.parentFileAs<PPTX::Slide>().TableStyles;//boost::shared_dynamic_cast<PPTX::TableStyles, OOX::File>(Presentation->get(OOX::FileTypes::TableStyles));
		smart_ptr<PPTX::Theme>		ThemeFile	= pOle.parentFileAs<PPTX::Slide>().Theme;
		StylesFile->SetTheme(ThemeFile);

		CString StyleId = _T("");
		TableCellData CellData;
		CellData.colsCount = (int)pOle.Table->TableCols.GetCount();
		CellData.rowsCount = (int)pOle.Table->TableRows.GetCount();

		if(pOle.Table->TableProperties.is_init())
		{
			StyleId				= pOle.Table->TableProperties->TableStyleId.get_value_or(_T("")/**StylesFile->def*/);
			CellData.band1H		= pOle.Table->TableProperties->BandRow.get_value_or(false);
			CellData.band1V		= pOle.Table->TableProperties->BandCol.get_value_or(false);
			CellData.firstCol	= pOle.Table->TableProperties->FirstCol.get_value_or(false);
			CellData.firstRow	= pOle.Table->TableProperties->FirstRow.get_value_or(false);
			CellData.lastCol	= pOle.Table->TableProperties->LastCol.get_value_or(false);
			CellData.lastRow	= pOle.Table->TableProperties->LastRow.get_value_or(false);
		}
		else
		{
			StyleId				= _T("")/**StylesFile->def*/;
			CellData.band1H		= false;
			CellData.band1V		= false;
			CellData.firstCol	= false;
			CellData.firstRow	= false;
			CellData.lastCol	= false;
			CellData.lastRow	= false;
		}

		PPTX::Logic::TableStyle Style;
		if (StyleId != _T(""))
		{
			CAtlMap<CString, PPTX::Logic::TableStyle>::CPair* pPair = StylesFile->Styles.Lookup(StyleId);
			if (NULL != pPair)
				Style = pPair->m_value;
		}

		NSPresentationEditor::CShapeElement* lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

		RECT rect;
		pOle.GetRect(rect);
		lpShapeElement->m_rcBoundsOriginal.left		= (double)rect.left;
		lpShapeElement->m_rcBoundsOriginal.right	= (double)rect.right;
		lpShapeElement->m_rcBoundsOriginal.top		= (double)rect.top;
		lpShapeElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		lpShapeElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth / 36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/ 36000;

		lpShapeElement->m_dStartTime	= 0;
		lpShapeElement->m_dEndTime		= pEContainer.m_dDuration;

		lpShapeElement->m_oShape.m_oBrush.Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.Alpha2 = 0;
		if(Style.tblBg.is_init())
		{
			PPTX::Logic::UniFill Fill;
			PPTX::Logic::UniColor Color;
			Fill = Style.tblBg->GetFillStyle(Color);
			FillToBrush(Fill, Color.GetBGRA(), lpShapeElement->m_oShape.m_oBrush);
		}
		//else
		//{
		//	lpShapeElement->m_oShape.m_oBrush.m_Alpha1 = 0;
		//	lpShapeElement->m_oShape.m_oBrush.m_Alpha2 = 0;
		//}

		lpShapeElement->m_oShape.m_oPen.Alpha = 255;
		lpShapeElement->m_oShape.m_oPen.Color = 0x0;

		pEContainer.m_arElements.Add(lpShapeElement);

		int cells = CellData.colsCount * CellData.rowsCount;
		NSPresentationEditor::CShapeElement** TableOfCells = new NSPresentationEditor::CShapeElement*[cells];
		int* TableOfHeights = new int[cells];
		int* Heights = new int[CellData.rowsCount];

		for(int i = 0; i < CellData.rowsCount; i++)
		{
			Heights[i] = pOle.Table->TableRows[i].Height;
			for(int j = 0; j < CellData.colsCount; j++)
			{
				TableOfCells[i * CellData.colsCount + j] = NULL;
				TableOfHeights[i * CellData.colsCount + j] = 0;
			}
		}

		int x = (int)(lpShapeElement->m_rcBoundsOriginal.left);
		//int y = lpShapeElement->m_rcBoundsOriginal.top;
		for(int i = 0; i < CellData.rowsCount; i++)
		{
			x = (int)lpShapeElement->m_rcBoundsOriginal.left;
			for(int j = 0; j < (int)pOle.Table->TableRows[i].Cells.GetCount(); ++j)
			{
				PPTX::Logic::TableCell cell = pOle.Table->TableRows[i].Cells[j];

				if(!cell.HMerge.get_value_or(false))// && (!cell.VMerge.get_value_or(false)))
				{
					int w = 0;//*pOle.Table->TableCols->at(j).Width;
					int GridSpan = cell.GridSpan.get_value_or(1);
					for(int k = 0; k < GridSpan; k++)
					{
						w += pOle.Table->TableCols[j + k].Width;
					}

					int h = 0;//*pOle.Table->TableRows->at(i).Height;
					int RowSpan = cell.RowSpan.get_value_or(1);
					for(int k = 0; k < RowSpan; k++)
					{
						h += pOle.Table->TableRows[i + k].Height;
					}

					CellData.x			= x;
					//CellData.y		= y;
					CellData.width		= w;
					CellData.height		= h;
					CellData.colNumber	= j;
					CellData.rowNumber	= i;
					if((CellData.lastCol) && (CellData.colNumber + GridSpan >= CellData.colsCount))
					{
						CellData.colNumber = CellData.colsCount - 1;
					}
					if((CellData.lastRow) && (CellData.rowNumber + RowSpan >= CellData.rowsCount))
					{
						CellData.rowNumber = CellData.rowsCount - 1;
					}

					if(!cell.VMerge.get_value_or(false))
					{
						int num = i * CellData.colsCount + j;
						TableOfCells[num] = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);
						int res = TableCellConvert(cell, pEContainer, *TableOfCells[num], Style, CellData);

						if((RowSpan <= 1) && (Heights[i] < res))
							Heights[i] = res;
						TableOfHeights[num] = max(Heights[i], res);

						//TableOfCells[num]->m_rcBoundsOriginal.bottom = CellData.y + CellData.height;
					}
					x += w;
				}
			}
			//y += Heights[i];
		}

		for(int i = 0; i < CellData.rowsCount; i++)
		{
			for(int j = 0; j < (int)pOle.Table->TableRows[i].Cells.GetCount(); ++j)
			{
				PPTX::Logic::TableCell cell = pOle.Table->TableRows[i].Cells[j];

				if(!cell.HMerge.get_value_or(false))
				{
					int h = 0;
					int RowSpan = cell.RowSpan.get_value_or(1);
					for(int k = 0; k < RowSpan; k++)
					{
						h += Heights[i + k];
					}

					if(!cell.VMerge.get_value_or(false))
					{
						int num = i * CellData.colsCount + j;

						if(TableOfHeights[num] < h)
							TableOfHeights[num] = h;
						else
							Heights[i + RowSpan - 1] += TableOfHeights[num] - h;
					}
				}
			}
		}

		for(int i = 0; i < CellData.rowsCount; i++)
		{
			for(int j = 0; j < (int)pOle.Table->TableRows[i].Cells.GetCount(); ++j)
			{
				PPTX::Logic::TableCell cell = pOle.Table->TableRows[i].Cells[j];

				if(!cell.HMerge.get_value_or(false))
				{
					int h = 0;
					int RowSpan = cell.RowSpan.get_value_or(1);
					for(int k = 0; k < RowSpan; k++)
					{
						h += Heights[i + k];
					}

					if(!cell.VMerge.get_value_or(false))
					{
						int num = i * CellData.colsCount + j;

						if(TableOfHeights[num] < h)
							TableOfHeights[num] = h;
					}
				}
			}
		}

		int y = (int)(lpShapeElement->m_rcBoundsOriginal.top);
		for(int i = 0; i < CellData.rowsCount; i++)
		{
			for(int j = 0; j < (int)pOle.Table->TableRows[i].Cells.GetCount(); ++j)
			{
				PPTX::Logic::TableCell cell = pOle.Table->TableRows[i].Cells[j];

				if((!cell.HMerge.get_value_or(false)) && (!cell.VMerge.get_value_or(false)))
				{
					int num = i * CellData.colsCount + j;

					TableOfCells[num]->m_rcBoundsOriginal.top = y;
					//TableOfCells[num]->m_rcBoundsOriginal.bottom = y + max(TableOfHeights[num], Heights[i]);
					TableOfCells[num]->m_rcBoundsOriginal.bottom = y + TableOfHeights[num];

					pEContainer.m_arElements.Add(TableOfCells[num]);
				}
			}
			y += Heights[i];
		}

		delete []TableOfCells;
		delete []TableOfHeights;
		delete []Heights;
	}

	int Converter::TableCellConvert(const PPTX::Logic::TableCell& pTableCell, NSPresentationEditor::CSlide& pEContainer, NSPresentationEditor::CShapeElement& lpShapeElement, const PPTX::Logic::TableStyle& Style, const TableCellData& CellData)
	{
		int result = CellData.height;

		if(pTableCell.CellProperties.is_init())
		{
			lpShapeElement.m_oShape.m_dTextMarginX		+= pTableCell.CellProperties->MarL.get_value_or(91440) / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginY		+= pTableCell.CellProperties->MarT.get_value_or(45720) / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginRight	+= pTableCell.CellProperties->MarR.get_value_or(91440) / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginBottom += pTableCell.CellProperties->MarB.get_value_or(45720) / 36000.0;
		}
		else
		{
			lpShapeElement.m_oShape.m_dTextMarginX		+= 91440 / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginY		+= 45720 / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginRight	+= 91440 / 36000.0;
			lpShapeElement.m_oShape.m_dTextMarginBottom += 45720 / 36000.0;
		}

		lpShapeElement.m_rcBoundsOriginal.left = CellData.x;
		//lpShapeElement.m_rcBoundsOriginal.top = CellData.y;
		lpShapeElement.m_rcBoundsOriginal.right = CellData.x + CellData.width;
		//lpShapeElement.m_rcBoundsOriginal.bottom = CellData.y + CellData.height;

		lpShapeElement.m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpShapeElement.m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpShapeElement.m_oMetric.m_lMillimetresHor	= pEContainer.m_lOriginalWidth/36000;
		lpShapeElement.m_oMetric.m_lMillimetresVer	= pEContainer.m_lOriginalHeight/36000;

		lpShapeElement.m_dStartTime = 0;
		lpShapeElement.m_dEndTime	= pEContainer.m_dDuration;

		bool filled = false;
		if((pTableCell.CellProperties.is_init()) && (pTableCell.CellProperties->Fill.is_init()))
		{
			lpShapeElement.m_oShape.m_oBrush.Alpha1 = 0;
			lpShapeElement.m_oShape.m_oBrush.Alpha2 = 0;
			FillToBrush(pTableCell.CellProperties->Fill, 0, lpShapeElement.m_oShape.m_oBrush);
			filled = true;
		}

		bool StyleInit = false;
		PPTX::Logic::UniFill Fill;
		PPTX::Logic::UniColor Color;
		PPTX::Logic::TcTxStyle TextStyle;
		int shiftVert = 0;
		int shiftHorz = 0;
		int horzStart = 0;
		int vertStart = 0;
		int horzEnd = CellData.colsCount-1;
		int vertEnd = CellData.rowsCount-1;

		if(Style.wholeTbl.is_init())
		{
			Fill += Style.wholeTbl->GetFillStyle(Color);
			StyleInit = true;
			if(Style.wholeTbl->tcTxStyle.is_init())
				TextStyle = Style.wholeTbl->tcTxStyle.get();
		}

		if((CellData.firstCol) && (Style.firstCol.is_init()))
		{
			shiftHorz = 1;
			horzStart++;
		}
		if((CellData.firstRow) && (Style.firstRow.is_init()))
		{
			shiftVert = 1;
			vertStart++;
		}
		if((CellData.lastCol) && (Style.lastCol.is_init()))
			horzEnd--;
		if((CellData.lastRow) && (Style.lastRow.is_init()))
			vertEnd--;

		if((CellData.band1H) && (Style.band1H.is_init()) && ((CellData.rowNumber + shiftVert) % 2 == 0)
			&& (CellData.rowNumber >= vertStart) && (CellData.rowNumber <= vertEnd))
		{
			Fill += Style.band1H->GetFillStyle(Color);
			StyleInit = true;
			if(Style.band1H->tcTxStyle.is_init())
				TextStyle = Style.band1H->tcTxStyle.get();
		}

		if((CellData.band1V) && (Style.band1V.is_init()) && ((CellData.colNumber + shiftHorz) % 2 == 0)
			&& (CellData.colNumber >= horzStart) && (CellData.colNumber <= horzEnd))
		{
			Fill += Style.band1V->GetFillStyle(Color);
			StyleInit = true;
			if(Style.band1V->tcTxStyle.is_init())
				TextStyle = Style.band1V->tcTxStyle.get();
		}

		if((CellData.firstCol) && (Style.firstCol.is_init()) && (CellData.colNumber == 0))
		{
			Fill += Style.firstCol->GetFillStyle(Color);
			StyleInit = true;
			if(Style.firstCol->tcTxStyle.is_init())
				TextStyle = Style.firstCol->tcTxStyle.get();
		}
		else if((CellData.lastCol) && (Style.lastCol.is_init()) && (CellData.colNumber == CellData.colsCount - 1))
		{
			Fill += Style.lastCol->GetFillStyle(Color);
			StyleInit = true;
			if(Style.lastCol->tcTxStyle.is_init())
				TextStyle = Style.lastCol->tcTxStyle.get();
		}

		if((CellData.firstRow) && (Style.firstRow.is_init()) && (CellData.rowNumber == 0))
		{
			Fill += Style.firstRow->GetFillStyle(Color);
			StyleInit = true;
			if(Style.firstRow->tcTxStyle.is_init())
				TextStyle = Style.firstRow->tcTxStyle.get();
		}
		else if((CellData.lastRow) && (Style.lastRow.is_init()) && (CellData.rowNumber == CellData.rowsCount - 1))
		{
			Fill += Style.lastRow->GetFillStyle(Color);
			StyleInit = true;
			if(Style.lastRow->tcTxStyle.is_init())
				TextStyle = Style.lastRow->tcTxStyle.get();
		}

		if(!filled)
		{
			lpShapeElement.m_oShape.m_oBrush.Alpha1 = 0;
			lpShapeElement.m_oShape.m_oBrush.Alpha2 = 0;
			if(StyleInit)
			{
				FillToBrush(Fill, Color.GetBGRA(), lpShapeElement.m_oShape.m_oBrush);
			}
			//else
			//{
			//	lpShapeElement.m_oShape.m_oBrush.m_Alpha1 = 0;
			//	lpShapeElement.m_oShape.m_oBrush.m_Alpha2 = 0;
			//}
		}

		if(pTableCell.TxBody.is_init())
		{
			CString ShapeText = pTableCell.TxBody->GetText();

			PPTX::Logic::ShapeProperties ShapeProps;
			pTableCell.GetShapeProperties(ShapeProps);

			PPTX::Logic::RunProperties RunProperty;
			RunProperty.b = (TextStyle.b.get_value_or(_T("def")) == _T("on"));
			RunProperty.i = (TextStyle.i.get_value_or(_T("def")) == _T("on"));
			if(TextStyle.FontRef.is_init())
			{
				PPTX::Logic::FontRef ref = TextStyle.FontRef.get();
				if(TextStyle.Color.is_init())
					ref.Color = TextStyle.Color;
				ShapeProps.FillFontRef(ref);
			}
			PPTX::Logic::TextParagraphPr ParagraphProperty;
			ParagraphProperty.defRPr = RunProperty;
			PPTX::Logic::TextListStyle ListStyle;
			for(int i = 0; i < 10; i++)
				ListStyle.levels[i] = ParagraphProperty;
			ShapeProps.FillFromTextListStyle(ListStyle);

			if((ShapeText != _T("")) && (ShapeText != _T("\n")))
				TextParse(pTableCell.TxBody.get(), lpShapeElement.m_oShape.m_oText, ShapeProps, 1.0, 0.0);

			result = TableCellHeight(pTableCell, CellData, ShapeProps);
		}

		lpShapeElement.m_oShape.m_oPen.Alpha = 255;
		lpShapeElement.m_oShape.m_oPen.Color = 0x0;

		return result;
	}

	int Converter::TableCellHeight(const PPTX::Logic::TableCell& pTableCell, const TableCellData& CellData, const PPTX::Logic::ShapeProperties& TextProps)
	{
		int result = CellData.height;

		int WidthInEmu = CellData.width;

		if(pTableCell.CellProperties.is_init())
		{
			WidthInEmu = WidthInEmu - pTableCell.CellProperties->MarR.get_value_or(91440);
			WidthInEmu = WidthInEmu - pTableCell.CellProperties->MarL.get_value_or(91440);
		}
		else
			WidthInEmu = WidthInEmu - (91440 + 91440);

		int TextHeight = TableCellTextHeight(pTableCell, WidthInEmu, TextProps);

		if(pTableCell.CellProperties.is_init())
		{
			TextHeight += pTableCell.CellProperties->MarB.get_value_or(45720);
			TextHeight += pTableCell.CellProperties->MarT.get_value_or(45720);
		}
		else
			TextHeight += (45720 + 45720);

		if(result < TextHeight)
			result = TextHeight;
		return result;
	}

	int Converter::TableCellTextHeight(const PPTX::Logic::TableCell& pTableCell, const int WidthInEmu, const PPTX::Logic::ShapeProperties& TextProps)
	{
		CAtlArray<int> Heights;
		Heights.RemoveAll();
		CAtlArray<int> Widths;
		Widths.RemoveAll();
		int TextHeight = 0;
		int TextSize = 0;

		if(pTableCell.TxBody->Paragrs.GetCount() == 0)
		{
			TextSize = TextProps.GetRunSize(0) / 100;
			TextHeight = TextSize * 12700;
			if(TextSize == 1)
				TextHeight += 12700;
			else
				TextHeight += TextSize * 2540;
			Heights.Add(TextHeight);
		}
		else
		{
			HDC dc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
			Gdiplus::Graphics graphic(dc);
			graphic.SetPageUnit(Gdiplus::UnitPoint);
			
			int FirstLevel = 0;
			int FirstBefore = 0;
			int LastAfter = 0;
			if(pTableCell.TxBody->Paragrs[0].pPr.is_init())
				FirstLevel = pTableCell.TxBody->Paragrs[0].pPr->lvl.get_value_or(0);
			FirstBefore = TextProps.GetParagraphSpcBef(FirstLevel, pTableCell.TxBody->Paragrs[0].pPr) * 127;
			if(FirstBefore < 0)
			{
				Widths.Add(WidthInEmu);
				Heights.Add( FirstBefore );
			}

			size_t nParagrsCount = pTableCell.TxBody->Paragrs.GetCount();
			for (size_t nIndex = 0; nIndex < nParagrsCount; ++nIndex)
			{
				const PPTX::Logic::Paragraph& CurrentParagraph = pTableCell.TxBody->Paragrs[nIndex];
				int level = 0;
				if(CurrentParagraph.pPr.is_init())
					level = CurrentParagraph.pPr->lvl.get_value_or(0);

				int SpaceBefore = max(0, -TextProps.GetParagraphSpcBef(level, CurrentParagraph.pPr));
				int SpaceAfter = max(0, -TextProps.GetParagraphSpcAft(level, CurrentParagraph.pPr));
				if(SpaceBefore + SpaceAfter > 0)
				{
					Widths.Add(WidthInEmu);
					Heights.Add((SpaceBefore + SpaceAfter) * 127);
				}
				LastAfter = SpaceAfter;

				if(CurrentParagraph.RunElems.GetCount() == 0)
				{
					TextSize = TextProps.GetRunSize(level, CurrentParagraph.endParaRPr, CurrentParagraph.pPr) / 100;
					TextHeight = TextSize * 12700;
					if(TextSize == 1)
						TextHeight += 12700;
					else
						TextHeight += TextSize * 2540;
					Heights.Add(TextHeight);
					Widths.Add(WidthInEmu);
				}
				else
				{
					int Margin = TextProps.GetParagraphLeftMargin(level, CurrentParagraph.pPr);
					int Indent = TextProps.GetParagraphIndent(level, CurrentParagraph.pPr);
					int LineSpacing = TextProps.GetParagraphLnSpc(level, CurrentParagraph.pPr);
					int LineSpacing2 = LineSpacing;
					bool BulletPlace = TextProps.HasParagraphBullet(level, CurrentParagraph.pPr);
					bool WordBreak = TextProps.GetParagraphLatinLnBrk(level, CurrentParagraph.pPr);
					int StartPosition = max(0, Margin + Indent);
					Widths.Add(StartPosition);
					Heights.Add(0);
					int StartLine = (int)Heights.GetCount();

					size_t nCountRun = CurrentParagraph.RunElems.GetCount();
					for (size_t nIndexRun = 0; nIndexRun < nCountRun; ++nIndexRun)
					{
						const PPTX::Logic::RunElem* j = &CurrentParagraph.RunElems[nIndexRun];
						if(j->is<PPTX::Logic::Run>())
						{
							PPTX::Logic::Run CurrentRun = j->as<PPTX::Logic::Run>();
							Gdiplus::RectF ResultRect;
							LOGFONTA logfont;
							logfont.lfHeight = -TextProps.GetRunSize(level, CurrentRun.rPr, CurrentParagraph.pPr) / 100;
							logfont.lfWidth = 0;
							logfont.lfEscapement = 0;
							logfont.lfOrientation = 0;
							logfont.lfWeight = TextProps.GetRunBold(level, CurrentRun.rPr, CurrentParagraph.pPr)? 700 : 400;
							logfont.lfItalic = TextProps.GetRunItalic(level, CurrentRun.rPr, CurrentParagraph.pPr)? TRUE : FALSE;
							logfont.lfUnderline = (TextProps.GetRunUnderline(level, CurrentRun.rPr, CurrentParagraph.pPr) == _T("none"))? FALSE : TRUE;
							logfont.lfStrikeOut = (TextProps.GetRunStrike(level, CurrentRun.rPr, CurrentParagraph.pPr) == _T("noStrike"))? FALSE : TRUE;
							logfont.lfCharSet = DEFAULT_CHARSET;
							logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
							logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
							logfont.lfQuality = DEFAULT_QUALITY;
							logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

							ZeroMemory(logfont.lfFaceName, LF_FACESIZE);
							
							
							CStringA strA = (CStringA)TextProps.GetRunFont(level, CurrentRun.rPr, CurrentParagraph.pPr);
							memcpy(logfont.lfFaceName, strA.GetBuffer(), min(LF_FACESIZE - 1, strA.GetLength()));
							
							logfont.lfFaceName[LF_FACESIZE - 1] = 0;

							Gdiplus::Font Font(dc, &logfont);
							CString Text = j->GetText();
							int TextLength = Text.GetLength();
							int TextSize = -logfont.lfHeight;
							int TextHeight = TextSize * 12700;
							if(TextSize == 1)
								TextHeight += 12700;
							else
								TextHeight += TextSize * 2540;

							if(LineSpacing > 0)
							{
								TextHeight = (int)(TextHeight * ((double)LineSpacing / 100.0));
								LineSpacing2 = TextHeight;
							}
							else
							{
								if(StartLine == (int)Heights.GetCount())
								{
									TextHeight = (int)(3352.8 * TextSize + 95.25 * (-LineSpacing));
								}
								else
									TextHeight = (-LineSpacing) * 127;
								LineSpacing2 = (-LineSpacing) * 127;
							}

							if(WordBreak)
							{
								for(int symbol = 0; symbol < TextLength; symbol++ )
								{
									WCHAR oTextUnicode = (WCHAR)Text[symbol];
									
									if(Text[symbol] != L' ')
										graphic.MeasureString(&oTextUnicode, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericTypographic(), &ResultRect);
									else
										graphic.MeasureString(&oTextUnicode, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericDefault(), &ResultRect);
									int TextWidth = (int)(ResultRect.Width * 12700);
									if(BulletPlace)// && (level > 0))
									{
										wchar_t point[2] = L"*";
										graphic.MeasureString(point, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericTypographic(), &ResultRect);
										TextWidth += (int)(ResultRect.Width * 12700);
										BulletPlace = false;
									}
									int& right = Widths[Widths.GetCount() - 1];
									if(right + TextWidth <= WidthInEmu)
									{
										right += TextWidth;

										int& bottom = Heights[Heights.GetCount() - 1];
										if(TextHeight > bottom)
											bottom = TextHeight;
									}
									else
									{
										right = WidthInEmu;
										Widths.Add(min(Margin + TextWidth, WidthInEmu));
										Heights.Add(TextHeight);
										TextHeight = LineSpacing2;
									}
								}
							}
							else
							{
								int WordWidth = 0;
								for(int symbol = 0; symbol < TextLength; symbol++ )
								{
									WCHAR oTextUnicode = (WCHAR)Text[symbol];

									if(oTextUnicode != L' ')
										graphic.MeasureString(&oTextUnicode, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericTypographic(), &ResultRect);
									else
										graphic.MeasureString(&oTextUnicode, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericDefault(), &ResultRect);

									int TextWidth = (int)(ResultRect.Width * 12700);
									if(BulletPlace)
									{
										wchar_t point[2] = L"*";
										graphic.MeasureString(point, 1, &Font, Gdiplus::PointF(0.0, 0.0), Gdiplus::StringFormat::GenericTypographic(), &ResultRect);
										TextWidth += (int)(ResultRect.Width * 12700);
										BulletPlace = false;
										Widths[Widths.GetCount() - 1] += TextWidth;
										WordWidth += TextWidth;
										continue;
									}

									if(Widths[Widths.GetCount() - 1] + TextWidth <= WidthInEmu)
									{
										Widths[Widths.GetCount() - 1] += TextWidth;
										if(TextHeight > Heights[Heights.GetCount() - 1])
											Heights[Heights.GetCount() - 1] = TextHeight;
										WordWidth += TextWidth;
									}
									else
									{
										if(Text[symbol] == L' ')
											Widths[Widths.GetCount() - 1] += TextWidth;
										else
										{
											if((WordWidth + StartPosition) == Widths[Widths.GetCount() - 1])
											{
												Widths[Widths.GetCount() - 1] = WidthInEmu;
												Widths.Add(min(Margin + TextWidth, WidthInEmu));
												StartPosition = Margin;
												Heights.Add(TextHeight);
												WordWidth = TextWidth;
												TextHeight = LineSpacing2;
											}
											else
											{
												Widths[Widths.GetCount() - 1] = WidthInEmu;
												Widths.Add(min(Margin + WordWidth + TextWidth, WidthInEmu));
												StartPosition = Margin;
												Heights.Add(TextHeight);
												WordWidth += TextWidth;
												TextHeight = LineSpacing2;
											}
										}
									}

									if((Text[symbol] == L' ') || (Text[symbol] == L'-'))
										WordWidth = 0;
								}
							}
						}
						else if(j->is<PPTX::Logic::Br>())
							Widths[Widths.GetCount() - 1] = WidthInEmu;
					}
					Widths[Widths.GetCount() - 1] = WidthInEmu;
				}
			}

			if(LastAfter > 0)
			{
				Widths.Add(WidthInEmu);
				Heights.Add( -LastAfter * 127);
			}

			DeleteDC(dc);
		}

		int ResultHeight = 0;
		for(size_t _i = 0; _i < Heights.GetCount(); _i)
			ResultHeight += Heights[_i];
		return ResultHeight;
	}
}