#include "Page.h"
#include "../resources/Constants.h"
#include "../resources/SingletonTemplate.h"
#include "../resources/utils.h"
#include <memory>

namespace NSDocxRenderer
{
	CPage::CPage(NSFonts::IApplicationFonts* pFonts)
	{
	}

	void CPage::Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
					 NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix,
					 Aggplus::CGraphicsPathSimpleConverter* pSimple, CFontStyleManager* pFontStyleManager, CFontManager *pFontManager,
					 CFontSelector* pFontSelector, CParagraphStyleManager* pParagraphStyleManager)
	{
		m_pFont     = pFont;
		m_pPen      = pPen;
		m_pBrush    = pBrush;
		m_pShadow   = pShadow;
		m_pEdgeText = pEdge;

		m_pTransform               = pMatrix;
		m_pSimpleGraphicsConverter = pSimple;

		m_pFontStyleManager = pFontStyleManager;
		m_pFontManager = pFontManager;
		m_pFontSelector = pFontSelector;
		m_pParagraphStyleManager = pParagraphStyleManager;

		m_pCurrentLine = nullptr;
//		m_pCurrentRow = nullptr;

		CShape::ResetRelativeHeight();
	}

	void CPage::BeginCommand(DWORD lType)
	{
		m_lLastCommand = m_lCurrentCommand;
		m_lCurrentCommand = lType;
	}

	void CPage::Clear()
	{
		m_arConts.clear();
		m_arTextLines.clear();
		m_arDiacriticalSymbols.clear();
		m_arImages.clear();
		m_arShapes.clear();

		ClearTables();
		m_pCurrentLine = nullptr;
//		m_pCurrentRow = nullptr;
		m_oVector.Clear();
	}

	void CPage::ClearTables()
	{
//		m_arPeaks.clear();
//		m_arCells.clear();
//		m_arRows.clear();
//		m_arTables.clear();
	}

	CPage::~CPage()
	{
		Clear();
	}

	void CPage::DeleteTextClipPage()
	{
		if (m_bIsDeleteTextClipPage)
			for (auto& line : m_arTextLines)
				if (line->m_dTop >= m_dHeight || line->m_dBaselinePos <= 0)
					line = nullptr;
	}

	// image commands
	void CPage::WriteImage(const std::shared_ptr<CImageInfo> pInfo, double& fX, double& fY, double& fWidth, double& fHeight)
	{
		auto pImage = std::make_shared<CShape>(pInfo, L"");
		pImage->m_eType = CShape::eShapeType::stPicture;

		double dRotation = m_pTransform->z_Rotation();

		if (fabs(dRotation) < 5.0)
		{
			double x1 = fX;
			double y1 = fY;
			double x2 = fX + fWidth;
			double y2 = fY + fHeight;

			m_pTransform->TransformPoint(x1, y1);
			m_pTransform->TransformPoint(x2, y2);

			if (x1 <= x2)
			{
				pImage->m_dLeft     = x1;
				pImage->m_dWidth    = x2 - x1;
			}
			else
			{
				pImage->m_dLeft     = x2;
				pImage->m_dWidth    = x1 - x2;
			}

			if (y1 <= y2)
			{
				pImage->m_dTop      = y1;
				pImage->m_dHeight   = y2 - y1;
			}
			else
			{
				pImage->m_dTop      = y2;
				pImage->m_dHeight   = y1 - y2;
			}

			pImage->m_dRotate = 0.0;
		}
		else
		{
			double x1 = fX;
			double y1 = fY;
			double x2 = fX + fWidth;
			double y2 = fY + fHeight;

			Aggplus::CMatrix oTemp  = *m_pTransform;

			double dCx = (x1 + x2) / 2;
			double dCy = (y1 + y2) / 2;
			m_pTransform->TransformPoint(dCx, dCy);
			oTemp.RotateAt(-dRotation, dCx, dCy, Aggplus::MatrixOrderAppend);

			oTemp.TransformPoint(x1, y1);
			oTemp.TransformPoint(x2, y2);

			if (x1 <= x2)
			{
				pImage->m_dLeft     = x1;
				pImage->m_dWidth    = x2 - x1;
			}
			else
			{
				pImage->m_dLeft     = x2;
				pImage->m_dWidth    = x1 - x2;
			}

			if (y1 <= y2)
			{
				pImage->m_dTop      = y1;
				pImage->m_dHeight   = y2 - y1;
			}
			else
			{
				pImage->m_dTop      = y2;
				pImage->m_dHeight   = y1 - y2;
			}

			pImage->m_dRotate = dRotation;
		}

		pImage->m_dBaselinePos = pImage->m_dTop + pImage->m_dHeight;
		pImage->m_dRight = pImage->m_dLeft + pImage->m_dWidth;

		m_arImages.push_back(pImage);
	}

	// path commands
	void CPage::MoveTo(double& dX, double& dY)
	{
		m_pTransform->TransformPoint(dX, dY);
		m_oVector.MoveTo(dX, dY);
	}

	void CPage::LineTo(double& dX, double& dY)
	{
		m_pTransform->TransformPoint(dX, dY);
		m_oVector.LineTo(dX, dY);
	}

	void CPage::CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
	{
		m_pTransform->TransformPoint(x1, y1);
		m_pTransform->TransformPoint(x2, y2);
		m_pTransform->TransformPoint(x3, y3);

		m_oVector.CurveTo(x1, y1, x2, y2, x3, y3);
	}

	void CPage::Start()
	{
	}

	void CPage::End()
	{
		m_oVector.End();
	}

	void CPage::Close()
	{
		m_oVector.Close();
	}


	void CPage::DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo)
	{
		double dLeft, dRight, dTop, dBottom;
		dLeft = m_oVector.GetLeft();
		dRight = m_oVector.GetRight();
		dTop = m_oVector.GetTop();
		dBottom = m_oVector.GetBottom();
		if ((dLeft <= dRight) && (dTop <= dBottom))
		{
			if (!m_arShapes.empty())
			{
				auto pLastShape = m_arShapes.back();

				if (pLastShape->m_dLeft == dLeft &&
						pLastShape->m_dTop == dTop &&
						pLastShape->m_dWidth == dRight - dLeft &&
						pLastShape->m_dHeight == dBottom - dTop)
				{
					if (0x00 != (lType & 0x01))
					{
						pLastShape->m_bIsNoStroke = false;
						pLastShape->m_oPen = *m_pPen;
					}
					if (0x00 != (lType >> 8))
					{
						pLastShape->m_bIsNoFill = false;
						pLastShape->m_oBrush = *m_pBrush;
					}
					return;
				}
			}

			auto pShape = std::make_shared<CShape>();

			if (pInfo)
			{
				pShape->m_pImageInfo = pInfo;
				pShape->m_eType = CShape::eShapeType::stVectorTexture;
			}
			else
			{
				pShape->m_eType = CShape::eShapeType::stVectorGraphics;
			}

			if (0x00 != (lType & 0x01))
			{
				pShape->m_bIsNoStroke = false;
				pShape->m_oPen = *m_pPen;
			}
			if (0x00 != (lType >> 8))
			{
				pShape->m_bIsNoFill = false;
				pShape->m_oBrush = *m_pBrush;
			}

			if (pShape->m_bIsNoStroke)
			{
				if ((fabs(dLeft - dRight) < 0.3) || (fabs(dTop - dBottom) < 0.3))
				{
					pShape->m_oPen.Color = m_pBrush->Color1;
					pShape->m_oPen.Alpha = m_pBrush->Alpha1;
				}
			}

			double dDeterminant = sqrt(fabs(m_pTransform->Determinant()));
			pShape->m_oPen.Size *= dDeterminant;

			pShape->SetVector(std::move(m_oVector));
			m_arShapes.push_back(pShape);
		}
	}

	void CPage::CollectTextData(const PUINT pUnicodes, const PUINT pGids, const UINT& nCount,
								const double& fX, const double& fY, const double& fWidth, const double& fHeight,
								const double& fBaseLineOffset, const bool& bIsPDFAnalyzer)
	{
		// 9 - \t
		if (pUnicodes != nullptr && nCount == 1 && (IsSpaceUtf32(*pUnicodes) || *pUnicodes == 9))
		{
			//note пробелы не нужны, добавляются при анализе
			return;
		}

		double dTextX = fX;
		double dTextY = fY;
		double dTextR = fX + fWidth;
		double dTextB = fY + fHeight;

		m_pTransform->TransformPoint(dTextX, dTextY);
		m_pTransform->TransformPoint(dTextR, dTextB);

		NSStringUtils::CStringUTF32 oText((uint32_t*)pUnicodes, nCount);

		if ((pUnicodes != nullptr) && (pGids != nullptr))
			for (unsigned int i = 0; i < nCount; ++i)
				if (!IsUnicodeSymbol(pUnicodes[i]))
					oText[i] = ' ';

		// иногда приходит неверный? размер, нужно перемерить (XPS)
		if(m_bIsRecalcFontSize)
		{
			m_pFont->Size *= ((m_pTransform->sx() + m_pTransform->sy()) / 2);
			m_bIsRecalcFontSize = false;
		}
		m_pFontManager->LoadFontByFile(*m_pFont);

		// закомментив это, все гуд
		//if (fabs(dTextW) < 0.01 || (dTextW > 10))
		//{

		double _x = 0;
		double _y = 0;
		double _w = 0;
		double _h = 0;

		if (nullptr != pGids)
		{
			m_pFontManager->SetStringGid(1);
			m_pFontManager->MeasureStringGids(pGids, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
		}
		else
		{
			// такого быть не должно (только из xps)
			m_pFontManager->SetStringGid(0);
			m_pFontManager->MeasureStringGids(pUnicodes, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
		}

		//}
		double dBaseLinePos = dTextY + fBaseLineOffset;

		auto pCont = std::make_shared<CContText>(m_pFontManager);

		auto oMetrics = m_pFontManager->GetFontMetrics();
		auto oParams = m_pFontManager->GetFontSelectParams();

		m_pFontSelector->SelectFont(oParams, oMetrics, oText);
		_h = m_pFontManager->GetFontHeight();

		pCont->m_dBaselinePos = dBaseLinePos;
		pCont->m_dTop         = pCont->m_dBaselinePos - _h - oMetrics.dBaselineOffset;
		pCont->m_dHeight      = pCont->m_dBaselinePos - pCont->m_dTop;

		pCont->m_dLeft        = dTextX;
		pCont->m_dWidth       = _w;
		pCont->m_dRight       = dTextX + _w;

		pCont->m_oText        = oText;

		// первичное получение стиля для текущего символа
		// при дальнейшем анализе может измениться
		pCont->m_pFontStyle = m_pFontStyleManager->GetOrAddFontStyle(*m_pBrush,
																		m_pFontSelector->GetSelectedName(),
																		m_pFont->Size,
																		m_pFontSelector->IsSelectedItalic(),
																		m_pFontSelector->IsSelectedBold());
		pCont->m_dSpaceWidthMM = m_pFontManager->GetSpaceWidthMM();
		m_pParagraphStyleManager->UpdateAvgFontSize(m_pFont->Size);

		// собираем отдельно, т.к. такие символы не имеют размера m_dWidth
		if (nCount == 1 && IsDiacriticalMark(*pUnicodes))
			m_arDiacriticalSymbols.push_back(pCont);
		else
			m_arConts.push_back(pCont);
	}

	void CPage::AddContToTextLine(std::shared_ptr<CContText> pCont)
	{
		if (nullptr == m_pCurrentLine)
		{
			auto pLine = std::make_shared<CTextLine>();
			m_pCurrentLine = pLine.get();
			m_pCurrentLine->AddCont(pCont);
			m_arTextLines.push_back(pLine);
			return;
		}

		if (fabs(m_pCurrentLine->m_dBaselinePos - pCont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			m_pCurrentLine->AddCont(pCont);
			return;
		}

		for (size_t i = 0; i < m_arTextLines.size(); ++i)
		{
			if (fabs(m_arTextLines[i]->m_dBaselinePos - pCont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
			{
				m_pCurrentLine = m_arTextLines[i].get();
				m_pCurrentLine->AddCont(pCont);
				return;
			}
		}

		auto pLine = std::make_shared<CTextLine>();
		m_pCurrentLine = pLine.get();
		m_pCurrentLine->AddCont(pCont);
		m_arTextLines.push_back(pLine);
	}

	void CPage::ProcessingAndRecordingOfPageData(NSStringUtils::CStringBuilder& oWriter, LONG lPagesCount, LONG lNumberPages)
	{
		// build text lines from m_arConts
		BuildTextLines();

		// analyze shapes (get type of lines etc)
		AnalyzeShapes();

		// analyze text lines and conts inside
		AnalyzeTextLines();

		// merge conts in text lines
		BuildLines();

		// build paragraphs from m_arTextLines
		BuildParagraphes();

		// merge shapes
		MergeShapes();

		// calc sizes on selected fonts for m_arConts
		CalcSelected();
		ToXml(oWriter);
		WriteSectionToFile(lPagesCount >= lNumberPages - 1, oWriter);
	}

	void CPage::BuildTextLines()
	{
		using cont_ptr = std::shared_ptr<CContText>;
		std::sort(m_arConts.begin(), m_arConts.end(), [] (const cont_ptr& a, const cont_ptr& b) {
			if(fabs(a->m_dBaselinePos - b->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
				return a->m_dLeft < b->m_dLeft;
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		for(auto&& cont : m_arConts)
			AddContToTextLine(cont);
	}

	void CPage::MergeShapes()
	{
		if(m_arShapes.empty())
			return;

		for(size_t i = 0; i < m_arShapes.size() - 1; i++)
		{
			auto& val = m_arShapes[i];
			auto& nextVal = m_arShapes[i + 1];

			if(val && nextVal)
				nextVal->TryMergeShape(val);
		}
	}

	void CPage::CalcSelected()
	{
		for(auto& cont : m_arConts)
			cont->CalcSelected();
	}

	void CPage::AnalyzeShapes()
	{
		//BuildTables();
		DetermineLinesType();
	}

//	void CPage::BuildTables()
//	{
//		//Графика таблиц парсится в условные 2 типа:
//		//1 - С выделение отдельных узлов (peak) в местах пересечения линий
//		//При этом графический шейп доходит до peak и прерывается. Каждая шейп-линия является стороной 1 ячейки
//		//2 - Без выделения узлов - каждая линия тамблицы захватывает несколько ячеек

//		//Текущая логика постороения таблиц реализовывает парсинг таблиц 1-го типа
//		//Для реализации 2го - нужно сначала создать peak в местах пересечения линий
//		//Основная причина почему так - нужно удалять все шейпы после парсинга

//		//todo пока плохо работает для таблиц с неполными ячейками. причина - не заполнены все основные параметры (m_dTop, m_dLeft...)
//		//todo реализовать парсинг таблиц 2-го типа
//		//todo необходимо в созданные cells добавить содержимое из m_arTextLines по геометрическому признаку (буквы должны находится внутри ячеек)
//		//todo зетем собрать TextLine и Paragraph для каждой ячейки
//		//todo написать функцию и логику для добавления таблицы в шейп - нужно для режима tatParagraphToShape

//		CollectPeaks();
//		CreatCells();
//		BuildRows();

//		CTable* pCurrTable = nullptr;
//		CRow* pFirstRow = nullptr;

//		if (!m_arRows.empty())
//		{
//			pCurrTable = new CTable();
//			m_arTables.push_back(pCurrTable);

//			pFirstRow = m_arRows.front();
//			pCurrTable->RecalcWithNewItem(pFirstRow);
//		}

//		for (size_t i = 1; i < m_arRows.size(); ++i)
//		{
//			auto pCurrRow = m_arRows[i];

//			eVerticalCrossingType eVType = pFirstRow->GetVerticalCrossingType(pCurrRow);
//			eHorizontalCrossingType eHType = pFirstRow->GetHorizontalCrossingType(pCurrRow);

//			bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext;
//			bool bIf2 = eHType == eHorizontalCrossingType::hctLeftAndRightBordersMatch;

//			if (bIf1 && bIf2)
//			{
//				pCurrTable->RecalcWithNewItem(pCurrRow);
//				pFirstRow = pCurrRow;
//			}
//			else
//			{
//				pCurrTable = new CTable();
//				m_arTables.push_back(pCurrTable);

//				pFirstRow = pCurrRow;
//				pCurrTable->RecalcWithNewItem(pFirstRow);
//			}
//		}

//		for (size_t i = 0; i < m_arTables.size(); ++i)
//		{
//			m_arTables[i]->CalculateColumnWidth();
//		}
//	}

//	void CPage::CollectPeaks()
//	{
//		for (size_t i = 0; i< m_arShapes.size(); ++i)
//		{
//			auto pCurrShape = m_arShapes[i];

//			if (pCurrShape->m_bIsNotNecessaryToUse)
//			{
//				continue;
//			}

//			//нашли вершину
//			if (pCurrShape->IsPeak())
//			{
//				CPeak* pCurrPeak = nullptr;

//				//ищем стороны
//				for (size_t j = 0; j < m_arShapes.size(); ++j)
//				{
//					auto pNextShape = m_arShapes[j];

//					if (pNextShape->m_bIsNotNecessaryToUse || !pNextShape->IsSide())
//					{
//						continue;
//					}

//					eVerticalCrossingType eVType = pCurrShape->GetVerticalCrossingType(pNextShape);
//					eHorizontalCrossingType eHType = pCurrShape->GetHorizontalCrossingType(pNextShape);

//					//проверяем, подходит ли сторона
//					bool bIf1 = eVType == eVerticalCrossingType::vctTopAndBottomBordersMatch &&
//							(eHType == eHorizontalCrossingType::hctCurrentLeftOfNext || eHType == eHorizontalCrossingType::hctCurrentRightOfNext);
//					bool bIf2 = eHType == eHorizontalCrossingType::hctLeftAndRightBordersMatch &&
//							(eVType == eVerticalCrossingType::vctCurrentAboveNext || eVType == eVerticalCrossingType::vctCurrentBelowNext);

//					if (bIf1 || bIf2)
//					{
//						if (!pCurrPeak)
//						{
//							pCurrPeak = new CPeak(pCurrShape);
//							pCurrShape->m_bIsNotNecessaryToUse = true;
//							pCurrShape->m_bIsUseInTable = true;
//							m_arPeaks.push_back(pCurrPeak);
//						}

//						if (eVType == eVerticalCrossingType::vctTopAndBottomBordersMatch)
//						{
//							if (eHType == eHorizontalCrossingType::hctCurrentLeftOfNext)
//							{
//								pCurrPeak->m_pLines[CPeak::dI] = pNextShape;
//								pNextShape->m_bIsUseInTable = true;
//							}
//							else if (eHType == eHorizontalCrossingType::hctCurrentRightOfNext)
//							{
//								pCurrPeak->m_pLines[CPeak::dIII] = pNextShape;
//								pNextShape->m_bIsUseInTable = true;
//							}
//						}
//						if (eHType == eHorizontalCrossingType::hctLeftAndRightBordersMatch)
//						{
//							if (eVType == eVerticalCrossingType::vctCurrentAboveNext)
//							{
//								pCurrPeak->m_pLines[CPeak::dIV] = pNextShape;
//								pNextShape->m_bIsUseInTable = true;
//							}
//							else if (eVType == eVerticalCrossingType::vctCurrentBelowNext)
//							{
//								pCurrPeak->m_pLines[CPeak::dII] = pNextShape;
//								pNextShape->m_bIsUseInTable = true;
//							}
//						}
//					}
//				}
//			}
//		}
//	}

//	void CPage::CreatCells()
//	{
//		//Cells
//		// II   |   I
//		//      |
//		//-----Peak---
//		//      |
//		// III  |   IV
//		//У каждого peak может быть до 4 углов, образованных выходящими линиями - обозначения согласно рисунку
//		CCell *pCellI, *pCellII, *pCellIII, *pCellIV;

//		for (size_t i = 0; i < m_arPeaks.size(); ++i)
//		{
//			CPeak* pPeak = m_arPeaks[i];
//			pCellI = nullptr; pCellII = nullptr; pCellIII = nullptr; pCellIV = nullptr;
//			//Lines from peak
//			// VI     II      V
//			//         |
//			// III <- Peak -> I
//			//         |
//			// VII     IV     VIII

//			//Corners
//			//Обозначение углов в ячейке
//			// IV------III
//			// |        |
//			// |  Cell  |
//			// |        |
//			// I--------II

//			bool bIsI   = pPeak->m_pLines[CPeak::dI]; //если true, то в направлении I есть линия
//			bool bIsII  = pPeak->m_pLines[CPeak::dII];
//			bool bIsIII = pPeak->m_pLines[CPeak::dIII];
//			bool bIsIV  = pPeak->m_pLines[CPeak::dIV];

//			//Если ячейки уже есть
//			//todo если ячейка неполная - могут отсутсвовать некоторые основные параметры (m_dLeft...)
//			//Скорее всего что в такой ячейке не все peak присутствуют в m_pPeaks и параметры не устанавливаются
//			//Добавить на это проверку и просто вычислить отсутсвующие параметры из известных
//			for (size_t j = 0; j < m_arCells.size(); ++j)
//			{
//				auto pSaveCell = m_arCells[j];
//				for (size_t k = 0; k < CCell::cNumCorners; ++k)
//				{
//					auto pSavePeak = pSaveCell->m_pPeaks[k];
//					if (pSavePeak)
//					{
//						if (pPeak->m_pLines[CPeak::dI] &&
//								pSavePeak->m_pLines[CPeak::dIII] &&
//								pPeak->m_pLines[CPeak::dI] == pSavePeak->m_pLines[CPeak::dIII])
//						{
//							if (k == CCell::cII)
//							{
//								pCellI = pSaveCell;
//								pCellI->SetParameters(pPeak, CCell::cI);
//							}
//							else if (k == CCell::cIII)
//							{
//								pCellIV = pSaveCell;
//								pCellIV->SetParameters(pPeak, CCell::cIV);
//							}
//						}

//						if (pPeak->m_pLines[CPeak::dII] &&
//								pSavePeak->m_pLines[CPeak::dIV] &&
//								pPeak->m_pLines[CPeak::dII] == pSavePeak->m_pLines[CPeak::dIV])
//						{
//							if (k == CCell::cIII)
//							{
//								pCellII = pSaveCell;
//								pCellII->SetParameters(pPeak, CCell::cII);
//							}
//							else if (k == CCell::cIV)
//							{
//								pCellI = pSaveCell;
//								pCellI->SetParameters(pPeak, CCell::cI);
//							}
//						}

//						if (pPeak->m_pLines[CPeak::dIII] &&
//								pSavePeak->m_pLines[CPeak::dI] &&
//								pPeak->m_pLines[CPeak::dIII] == pSavePeak->m_pLines[CPeak::dI])
//						{
//							if (k == CCell::cI)
//							{
//								pCellII = pSaveCell;
//								pCellII->SetParameters(pPeak, CCell::cII);
//							}
//							else if (k == CCell::cIV)
//							{
//								pCellIII = pSaveCell;
//								pCellIII->SetParameters(pPeak, CCell::cIII);
//							}
//						}

//						if (pPeak->m_pLines[CPeak::dIV] &&
//								pSavePeak->m_pLines[CPeak::dII] &&
//								pPeak->m_pLines[CPeak::dIV] == pSavePeak->m_pLines[CPeak::dII])
//						{
//							if (k == CCell::cI)
//							{
//								pCellIV = pSaveCell;
//								pCellIV->SetParameters(pPeak, CCell::cIV);
//							}
//							else if (k == CCell::cII)
//							{
//								pCellIII = pSaveCell;
//								pCellIII->SetParameters(pPeak, CCell::cIII);
//							}
//						}
//					}
//				}
//			}

//			//Если ячейка еще не построена по заданным углам - создаем ячейку
//			if (bIsI && bIsII)
//			{
//				if (!pCellI)
//				{
//					pCellI = new CCell();
//					pCellI->SetParameters(pPeak, CCell::cI);
//					m_arCells.push_back(pCellI);
//				}
//			}
//			if (bIsII && bIsIII)
//			{
//				if (!pCellII)
//				{
//					pCellII = new CCell();
//					pCellII->SetParameters(pPeak, CCell::cII);
//					m_arCells.push_back(pCellII);
//				}
//			}
//			if (bIsIII && bIsIV)
//			{
//				if (!pCellIII)
//				{
//					pCellIII = new CCell();
//					pCellIII->SetParameters(pPeak, CCell::cIII);
//					m_arCells.push_back(pCellIII);
//				}
//			}
//			if (bIsIV && bIsI)
//			{
//				if (!pCellIV)
//				{
//					pCellIV = new CCell();
//					pCellIV->SetParameters(pPeak, CCell::cIV);
//					m_arCells.push_back(pCellIV);
//				}
//			}
//		}

//		//удаляем использованные шейпы
//		for (size_t i = 0; i < m_arShapes.size(); ++i)
//		{
//			if (m_arShapes[i]->m_bIsUseInTable)
//			{
//				m_arShapes[i]->m_bIsNotNecessaryToUse = true;
//			}
//		}
//	}

//	void CPage::BuildRows()
//	{
//		//когда созданы все ячейки, собираем их в ряды
//		for (size_t i = 0; i < m_arCells.size(); ++i)
//		{
//			auto pCell = m_arCells[i];

//			if (pCell->m_bIsNotNecessaryToUse)
//			{
//				continue;
//			}

//			SelectCurrentRow(pCell);
//			m_pCurrentRow->RecalcWithNewItem(pCell);
//		}

//		CBaseItem::SortByBaseline(m_arRows);
//		for (size_t i = 0; i < m_arRows.size(); ++i)
//		{
//			CBaseItem::SortByLeft(m_arRows[i]->m_arCells);
//		}
//	}

//	void CPage::SelectCurrentRow(const CCell *pCell)
//	{
//		//логика аналогична AddContToTextLine
//		//todo c_dTHE_SAME_STRING_Y_PRECISION_MM - возможно слишком мала
//		if (nullptr == m_pCurrentRow)
//		{
//			auto pRow = new CRow();
//			m_pCurrentRow = pRow;
//			m_pCurrentRow->m_dBaselinePos = pCell->m_dBaselinePos;
//			m_arRows.push_back(pRow);
//			return;
//		}

//		if (fabs(m_pCurrentRow->m_dBaselinePos - pCell->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
//		{
//			return;
//		}

//		for (size_t i = 0; i < m_arRows.size(); ++i)
//		{
//			if (fabs(m_arRows[i]->m_dBaselinePos - pCell->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
//			{
//				m_pCurrentRow = m_arRows[i];
//				return;
//			}
//		}

//		auto pRow = new CRow();
//		m_pCurrentRow = pRow;
//		m_pCurrentRow->m_dBaselinePos = pCell->m_dBaselinePos;
//		m_arRows.push_back(pRow);
//		return;
//	}

	void CPage::DetermineLinesType()
	{
		//определяются типы только горизонтальных линий.
		//Входные данные представляют собой набор прямоугольников на одной линии
		//При определении типа линии используется крайний левый шейп, который
		//увеличивается в размере на ширину последующих за ним шейпов.
		//Последующие шейпы помечаются как m_bIsNotNecessaryToUse

		//todo добавить аналогичное определение для вертикальных линий
		//нужно для определения границ таблицы
		//note определение типов линий нужно сделать до распознования таблиц, когда линия несплошная

		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& curr_shape = m_arShapes[i];
			if (!curr_shape || curr_shape->m_dHeight > c_dMAX_LINE_HEIGHT_MM || // рассматриваем только тонкие объекты
				(curr_shape->m_eGraphicsType != eGraphicsType::gtRectangle &&
				curr_shape->m_eGraphicsType != eGraphicsType::gtCurve))
			{
				continue;
			}

			std::vector<std::shared_ptr<CShape>> curr_shapes;
			curr_shapes.push_back(curr_shape);

			for (size_t j = i+1; j < m_arShapes.size(); ++j)
			{
				auto& next_shape = m_arShapes[j];
				if (!next_shape || curr_shape->AreObjectsNoCrossingByVertically(next_shape.get())) // значительно ускоряет работу
					continue;

				bool bIf1 = curr_shape->IsCorrelated(next_shape);

				// довольно странное поведение - в зависимости от толщины линии информация о графике записывается в разные структуры
				bool bIf2 = curr_shape->m_oBrush.IsEqual(&next_shape->m_oBrush);
				bool bIf3 = curr_shape->m_oPen.IsEqual(&next_shape->m_oPen);

				// линия должна быть одного размера по высоте
				bool bIf4 = fabs(curr_shape->m_dHeight - next_shape->m_dHeight) < c_dGRAPHICS_ERROR_IN_LINES_MM;

				// все должно быть на одной линии
				bool bIf5 = fabs(curr_shape->m_dTop - next_shape->m_dTop) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5;

				if (bIf1 && (bIf2 || bIf3) && bIf4 && bIf5) //все должно быть на одной линии
					curr_shapes.push_back(next_shape);
			}

			if (curr_shapes.size() > 1)
			{
				using shape_ptr = std::shared_ptr<CShape>;
				std::sort(curr_shapes.begin(), curr_shapes.end(), [] (const shape_ptr& a, const shape_ptr& b) {
					return a->m_dLeft < b->m_dLeft;
				});

				curr_shape = curr_shapes[0];

				for (size_t k = 1; k < curr_shapes.size(); ++k)
				{
					auto& next_shape = curr_shapes[k];
					CShape::CheckLineType(curr_shape, next_shape, k == curr_shapes.size() - 1);

					if(!curr_shape)
					{
						curr_shape = next_shape;
						k++;
					}
				}
			}
			else if (curr_shapes.size() == 1)
				CShape::CheckLineType(curr_shapes[0]);

			curr_shapes.clear();
		}
	}

	void CPage::AnalyzeTextLines()
	{
		// вся логика основана на отсортированных списках объектов

		using line_ptr = std::shared_ptr<CTextLine>;
		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const line_ptr& a, const line_ptr& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		// analyze drop caps (creates shapes)
		AnalyzeDropCaps();

		// assign highlights to conts & delete shapes which is uses in highlights
		DetermineStrikeoutsUnderlinesHighlights();

		// diacritical symbols
		AddDiacriticalSymbols();

		// super/subscript
		MergeLinesByVertAlignType();

		// delete lines out of page
		DeleteTextClipPage();
	}

	void CPage::AnalyzeDropCaps()
	{
		double avg_font_size = m_pParagraphStyleManager->GetAvgFontSize();

		std::vector<std::pair<std::shared_ptr<CContText>&, std::shared_ptr<CTextLine>&>> possible_caps;
		std::vector<std::shared_ptr<CDropCap>> drop_caps;

		for(size_t i = 0; i < m_arTextLines.size(); i++)
		{
			auto& line = m_arTextLines[i];
			for(auto& cont : line->m_arConts)
				if(cont && cont->m_pFontStyle->dFontSize > 2 * avg_font_size && cont->m_oText.length() == 1)
					possible_caps.push_back({cont, line});
		}

		for(auto& possible_cap : possible_caps)
		{
			auto& drop_cap_cont = possible_cap.first;
			auto& drop_cap_line = possible_cap.second;

			size_t num_of_lines = 0;

			for(auto& line : m_arTextLines)
			{
				if(!line || line == drop_cap_line)
					continue;

				// буквица должна быть левее
				if(line->m_dLeft < drop_cap_cont->m_dLeft)
					continue;

				// если совпадает строка по высоте - берем ее и выходим
				if(fabs(line->m_dBaselinePos - drop_cap_cont->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
				{
					num_of_lines++;
					break;
				}

				if(line->m_dBaselinePos > drop_cap_cont->m_dBaselinePos)
					break;

				if(fabs(line->m_dTop - drop_cap_cont->m_dTop) > c_dTHE_SAME_STRING_Y_PRECISION_MM && line->m_dTop < drop_cap_cont->m_dTop)
					continue;

				num_of_lines++;
			}
			if(num_of_lines)
			{
				auto drop_cap = std::make_unique<CDropCap>();
				*static_cast<CBaseItem*>(drop_cap.get()) = *drop_cap_cont;
				drop_cap->nLines = num_of_lines;
				drop_cap->wsFont = drop_cap_cont->m_pFontStyle->wsFontName;
				drop_cap->wsText = drop_cap_cont->m_oText.ToStdWString();

				drop_cap->nFontSize = static_cast<LONG>(drop_cap_cont->m_pFontStyle->dFontSize * 2);
				drop_caps.push_back(std::move(drop_cap));

				drop_cap_cont = nullptr;
				if(drop_cap_line->IsCanBeDeleted())
					drop_cap_line = nullptr;

				if(drop_cap_line)
					drop_cap_line->RecalcSizes();
			}
		}

		// шейпы из буквиц
		for(auto&& drop_cap : drop_caps)
		{
			auto shape = std::make_shared<CShape>();
			shape->m_eType = CShape::eShapeType::stTextBox;

			// перемерим на подобранном шрифте
			NSStructures::CFont oFont;
			oFont.Name = drop_cap->wsFont;
			oFont.Size = static_cast<double>(drop_cap->nFontSize) / 2.0;
			m_pFontManager->LoadFontByName(oFont);

			double box_X;
			double box_Y;
			double box_W;
			double box_H;

			m_pFontManager->SetStringGid(0);
			m_pFontManager->MeasureString(drop_cap->wsText, 0, 0, box_X, box_Y, box_W, box_H, CFontManager::mtPosition);

			shape->m_dBaselinePos = drop_cap->m_dBaselinePos;
			shape->m_dHeight = box_H;
			shape->m_dTop = drop_cap->m_dBaselinePos - shape->m_dHeight;

			shape->m_dRight = drop_cap->m_dRight;
			shape->m_dLeft = drop_cap->m_dLeft;
			shape->m_dWidth = drop_cap->m_dWidth;

			shape->m_arOutputObjects.push_back(drop_cap);
			shape->m_bIsBehindDoc = false;
			m_arShapes.push_back(shape);
		}
	}
	void CPage::AnalyzeConts()
	{

		for (size_t uCurrLineIndex = 0; uCurrLineIndex < m_arTextLines.size(); ++uCurrLineIndex)
		{
			auto& pCurrLine = m_arTextLines[uCurrLineIndex];
			if (!pCurrLine)
				continue;

			for (size_t uCurrContIndex = 0; uCurrContIndex < pCurrLine->m_arConts.size(); ++uCurrContIndex)
			{
				auto& pCurrCont = pCurrLine->m_arConts[uCurrContIndex];
				if (!pCurrCont)
					continue;

				// берем вторую линию, если символ последний - то начиная со следуюущей, иначе с той же
				for (size_t uNextLineIndex = uCurrContIndex >= pCurrLine->m_arConts.size() - 1 ?
					 uCurrLineIndex + 1 : uCurrLineIndex; uNextLineIndex < m_arTextLines.size(); ++uNextLineIndex)
				{
					auto& pNextLine = m_arTextLines[uNextLineIndex];

					// значительно ускоряет работу, то есть если никак не перескается - некст
					if (!pNextLine || pCurrLine->AreObjectsNoCrossingByVertically(pNextLine.get()))
						continue;

					// посимвольно смотрим некст линию - если та же то следующий символ, если другая - то с нуля
					for (size_t uNextContIndex = uNextLineIndex != uCurrLineIndex ? 0 : uCurrContIndex + 1;
						 uNextContIndex < pNextLine->m_arConts.size(); ++uNextContIndex)
					{
						// берем символ во второй линии
						auto pNextCont = pNextLine->m_arConts[uNextContIndex];
						if (!pNextCont)
							continue;

						eVerticalCrossingType eVType = pCurrCont->GetVerticalCrossingType(pNextCont.get());
						eHorizontalCrossingType eHType = pCurrCont->GetHorizontalCrossingType(pNextCont.get());

						if (CContText::CheckFontEffects(pCurrCont, pNextCont, eVType, eHType))
						{
							pNextCont = nullptr;
							if(pNextLine->IsCanBeDeleted())
								pNextLine = nullptr;
							break;
						}

						if (CContText::CheckVertAlignTypeBetweenConts(pCurrCont, pNextCont, eVType, eHType))
						{
							pCurrLine->SetVertAlignType(pCurrCont->m_eVertAlignType);
							pNextLine->SetVertAlignType(pNextCont->m_eVertAlignType);
							if ((pCurrLine->m_eVertAlignType == eVertAlignType::vatSuperscript &&
								 pNextLine->m_eVertAlignType == eVertAlignType::vatBase) ||
									(pCurrLine->m_eVertAlignType == eVertAlignType::vatBase &&
									 pNextLine->m_eVertAlignType == eVertAlignType::vatSubscript))
							{
								pCurrLine->m_pLine = pNextLine;
							}
							break;
						}

						if (pCurrCont->IsDuplicate(pNextCont.get(), eVType))
						{
							pNextCont = nullptr;
							pCurrCont->m_iNumDuplicates++;
							break;
						}
					}

					if(pNextLine->IsCanBeDeleted())
						pNextLine = nullptr;
				}
			}
		}
	}

	void CPage::DetermineStrikeoutsUnderlinesHighlights()
	{
		//определение различных эффектов на основании взаимного расположения символов и шейпов
		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& pShape = m_arShapes[i];
			if (!pShape || pShape->m_eGraphicsType == eGraphicsType::gtNoGraphics)
				continue;

			for (size_t j = 0; j < m_arTextLines.size(); ++j)
			{
				auto& pCurrLine = m_arTextLines[j];

				if (!pCurrLine || (pCurrLine->AreObjectsNoCrossingByVertically(pShape.get()) &&
					(pCurrLine->m_dTop > pShape->m_dBaselinePos ||
					pCurrLine->m_dBaselinePos + pCurrLine->m_dHeight < pShape->m_dTop)))
				{
					continue;
				}

				for (size_t k = 0; k < pCurrLine->m_arConts.size(); ++k)
				{
					auto& pCurrCont = pCurrLine->m_arConts[k];
					if (!pCurrCont)
						continue;

					eVerticalCrossingType eVType = pCurrCont->GetVerticalCrossingType(pShape.get());
					eHorizontalCrossingType eHType = pCurrCont->GetHorizontalCrossingType(pShape.get());

					bool bIsComplicatedFigure = pShape->m_eGraphicsType != eGraphicsType::gtComplicatedFigure;
					bool bIsLineCrossingText = IsLineCrossingText(pShape.get(), pCurrCont.get(), eHType);
					bool bIsLineBelowText = IsLineBelowText(pShape.get(), pCurrCont.get(), eHType);
					bool bIsItHighlightingBackground = IsItHighlightingBackground(pShape.get(), pCurrCont.get(), eHType);

					if(bIsLineCrossingText)
					{
						pCurrCont->m_bIsStrikeoutPresent = true;
						if (pShape->m_eLineType == eLineType::ltDouble)
							pCurrCont->m_bIsDoubleStrikeout = true;
					}

					if(bIsLineBelowText)
					{
						pCurrCont->m_bIsUnderlinePresent = true;
						pCurrCont->m_eUnderlineType  = pShape->m_eLineType;
						pCurrCont->m_lUnderlineColor = pShape->m_dHeight > 0.3 ? pShape->m_oBrush.Color1 : pShape->m_oPen.Color;
					}

					if(bIsItHighlightingBackground)
					{
						//Удовлетворяет расположением и размером - привязываем указатель на картинку
						pCurrCont->m_pShape = pShape;
						pCurrCont->m_bIsHighlightPresent = true;
						pCurrCont->m_lHighlightColor = pShape->m_oBrush.Color1;
					}

					// проверили - удаляем
					if (bIsComplicatedFigure && (bIsLineCrossingText || bIsLineBelowText || bIsItHighlightingBackground))
						pShape = nullptr;

					if (!bIsComplicatedFigure)
					{
						bool bIf1 = pCurrCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;
						bool bIf2 = pCurrCont->m_bIsShadowPresent && pCurrCont->m_bIsOutlinePresent;
						bool bIf3 = eVType == eVerticalCrossingType::vctCurrentOutsideNext;
						bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentOutsideNext;
						bool bIf5 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext;

						if ((bIf1 || bIf2) && bIf3 && (bIf4 || bIf5))
						{
							if (!bIf2)
							{
								auto oBrush = pCurrCont->m_pFontStyle->oBrush;
								oBrush.Color1 = pShape->m_oPen.Color;

								pCurrCont->m_pFontStyle = m_pFontStyleManager->GetOrAddFontStyle(oBrush,
									pCurrCont->m_pFontStyle->wsFontName,
									pCurrCont->m_pFontStyle->dFontSize,
									pCurrCont->m_pFontStyle->bItalic,
									pCurrCont->m_pFontStyle->bBold);

								pCurrCont->m_bIsShadowPresent = true;
								pCurrCont->m_bIsOutlinePresent = true;
							}

							pShape = nullptr;
						}
					}
				}
			}
		}
	}

	bool CPage::IsLineCrossingText(const CShape *pShape, CContText *pCont, const eHorizontalCrossingType& eHType)
	{
		// Height - это максимально возможный размер символа. Больше реального размера.
		double dTopBorder = pCont->m_dTop + pCont->m_dHeight / 3;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle &&
				pShape->m_eLineType != eLineType::ltUnknown;

		// Условие пересечения по вертикали
		bool bIf2 = pShape->m_dTop > dTopBorder && pShape->m_dBaselinePos < pCont->m_dBaselinePos;

		// Условие пересечения по горизонтали
		bool bIf3 = eHType != eHorizontalCrossingType::hctUnknown &&
				eHType != eHorizontalCrossingType::hctCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		// Условие для размеров по высоте
		bool bIf4 = pShape->m_dHeight < pCont->m_dHeight &&
				pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

		return bIf1 && bIf2 && bIf3 && bIf4;
	}

	bool CPage::IsLineBelowText(const CShape *pShape, CContText *pCont, const eHorizontalCrossingType& eHType)
	{
		//todo распознавание работает не для всех размеров шрифтов - возможно 0.15 мало или улучшить логику
		bool bIf1 = (pShape->m_eGraphicsType == eGraphicsType::gtRectangle ||
					 pShape->m_eGraphicsType == eGraphicsType::gtCurve) &&
				pShape->m_eLineType != eLineType::ltUnknown;

		//Условие по вертикали
		bool bIf2 = fabs(pShape->m_dTop - pCont->m_dBaselinePos) < pCont->m_dHeight * 0.3;

		//Условие пересечения по горизонтали
		bool bIf3 = eHType != eHorizontalCrossingType::hctUnknown &&
				eHType != eHorizontalCrossingType::hctCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		//Условие для размеров по высоте
		bool bIf4 = pShape->m_dHeight < pCont->m_dHeight &&
				pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

		return bIf1 && bIf2 && bIf3 && bIf4;
	}

	bool CPage::IsItHighlightingBackground(const CShape *pShape, CContText* pCont, const eHorizontalCrossingType& eHType)
	{
		double dSomeBaseLine1 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.75;
		double dSomeBaseLine2 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.5;
		double dSomeBaseLine3 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.25;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;

		//Условие пересечения по вертикали
		bool bIf2 = (dSomeBaseLine1 > pShape->m_dTop && dSomeBaseLine1 < pShape->m_dBaselinePos &&
					 dSomeBaseLine2 > pShape->m_dTop && dSomeBaseLine2 < pShape->m_dBaselinePos &&
					 dSomeBaseLine3 > pShape->m_dTop && dSomeBaseLine3 < pShape->m_dBaselinePos);

		//Условие пересечения по горизонтали
		bool bIf3 = eHType != eHorizontalCrossingType::hctUnknown &&
				eHType != eHorizontalCrossingType::hctCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		//Цвета должны быть разными
		bool bIf4 = pCont->m_pFontStyle->oBrush.Color1 != pShape->m_oBrush.Color1;
		bool bIf5 = pShape->m_oBrush.Color1 == c_iBlackColor && pShape->m_oPen.Color == c_iWhiteColor;
		bool bIf6 = pShape->m_bIsNoFill == false;
		bool bIf7 = pShape->m_bIsNoStroke == true;

		return bIf1 && bIf2 && bIf3 && bIf4 && !bIf5 && bIf6 && bIf7;
	}

	void CPage::AddDiacriticalSymbols()
	{
		for (auto& d_sym : m_arDiacriticalSymbols)
		{
			if (!d_sym)
				continue;

			bool isBreak = false;

			for (auto& line : m_arTextLines)
			{
				if (!line || line->AreObjectsNoCrossingByVertically(d_sym.get()))
					continue;

				for (auto& cont : line->m_arConts)
				{
					if (!cont)
						continue;

					eVerticalCrossingType eVType = cont->GetVerticalCrossingType(d_sym.get());
					eHorizontalCrossingType eHType = cont->GetHorizontalCrossingType(d_sym.get());

					if (eVType != eVerticalCrossingType::vctNoCrossingCurrentAboveNext &&
							eVType != eVerticalCrossingType::vctNoCrossingCurrentBelowNext &&
							eHType != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
							eHType != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext)
					{
						bool bIf1 = eHType == eHorizontalCrossingType::hctCurrentOutsideNext;
						bool bIf2 = eHType == eHorizontalCrossingType::hctCurrentLeftOfNext;
						bool bIf3 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext;
						bool bIf4 = eHType == eHorizontalCrossingType::hctDublicate;
						bool bIf5 = eHType == eHorizontalCrossingType::hctRightBorderMatch;

						bool bIf6 = eVType == eVerticalCrossingType::vctCurrentBelowNext ||
								eVType == eVerticalCrossingType::vctCurrentAboveNext;
						bool bIf7 = eVType == eVerticalCrossingType::vctTopAndBottomBordersMatch;
						bool bIf8 = eVType == eVerticalCrossingType::vctDublicate;

						if ((bIf1 && bIf6) || (bIf2 && bIf7) || (bIf4 && bIf8) || (bIf5 && bIf7))
						{
							cont->m_oText += d_sym->m_oText;
						}
						else if (bIf3 && bIf7)
						{
							NSStringUtils::CStringUTF32 oText(d_sym->m_oText);
							oText += cont->m_oText;
							cont->m_oText = oText;
						}
						d_sym = nullptr;
						isBreak = true;
						break;
					}
				}
				if (isBreak)
					break;
			}
		}
	}

	void CPage::MergeLinesByVertAlignType()
	{
		for (auto& line : m_arTextLines)
		{
			if (!line)
				continue;

			if (line->m_eVertAlignType == eVertAlignType::vatSuperscript)
			{
				auto& pBaseLine = line->m_pLine;
				if (pBaseLine)
				{
					for (auto& pCont : line->m_arConts)
					{
						if (pBaseLine->m_dLeft > pCont->m_dLeft)
							pBaseLine->m_dLeft = pCont->m_dLeft;
						if (pBaseLine->m_dRight < pCont->m_dRight)
							pBaseLine->m_dRight = pCont->m_dRight;

						pBaseLine->m_dWidth = pBaseLine->m_dRight - pBaseLine->m_dLeft;
						pBaseLine->m_arConts.push_back(pCont);
						pCont = nullptr;
					}

					using cont_ptr = std::shared_ptr<CContText>;
					std::sort(pBaseLine->m_arConts.begin(), pBaseLine->m_arConts.end(), [] (const cont_ptr& a, const cont_ptr& b) {
						return a->m_dLeft < b->m_dLeft;
					});

					line = nullptr;
				}
			}
			else if (line->m_eVertAlignType == eVertAlignType::vatBase)
			{
				auto& pSubLine = line->m_pLine;
				if (pSubLine)
				{
					for (auto& pCont : pSubLine->m_arConts)
					{
						if (pCont == nullptr)
							continue;

						if (line->m_dLeft > pCont->m_dLeft)
							line->m_dLeft = pCont->m_dLeft;
						if (line->m_dRight < pCont->m_dRight)
							line->m_dRight = pCont->m_dRight;

						line->m_dWidth = line->m_dRight - line->m_dLeft;
						line->m_arConts.push_back(pCont);
						pCont = nullptr;
					}

					using cont_ptr = std::shared_ptr<CContText>;
					std::sort(line->m_arConts.begin(), line->m_arConts.end(), [] (const cont_ptr& a, const cont_ptr& b) {
						return a->m_dLeft < b->m_dLeft;
					});

					pSubLine = nullptr;
				}
			}
		}
	}

	void CPage::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		bool bIsTextShapePresent = false;

		for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
		{
			if (m_arOutputObjects[i]->m_eType == COutputObject::eOutputType::etShape)
			{
				bIsTextShapePresent = true;
				break;
			}
		}

		bool bIsNeedWP = bIsTextShapePresent || !m_arImages.empty() || !m_arShapes.empty();

		if (bIsNeedWP)
		{
			oWriter.WriteString(L"<w:p>");
			//note при удалении строки откуда-то добавляется <w:p/> в начале страницы (если есть графика и текст), что добавляет дополнительную строку и сдвигает текст
			oWriter.WriteString(L"<w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");
		}

		for (size_t i = 0; i < m_arImages.size(); ++i)
		{
			m_arImages[i]->ToXml(oWriter);
		}

		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			m_arShapes[i]->ToXml(oWriter);
		}

		if (bIsTextShapePresent)
		{
			for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
			{
				auto pObj = m_arOutputObjects[i];
				CShape* pSahpe = nullptr;
				if((pSahpe = dynamic_cast<CShape*>(pObj.get())) != nullptr)
					pSahpe->ToXml(oWriter);
			}
		}

		if (bIsNeedWP)
		{
			oWriter.WriteString(L"</w:p>");
		}

		for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
		{
			auto pObj = m_arOutputObjects[i];
			CParagraph* pParagraph = nullptr;
			if((pParagraph = dynamic_cast<CParagraph*>(pObj.get())) != nullptr)
				pParagraph->ToXml(oWriter);

//			CTable* pTable = nullptr;
//			if((pTable = dynamic_cast<CTable*>(pObj)) != nullptr)
//				pTable->ToXml(oWriter);
		}
	}

	void CPage::BuildLines()
	{
		for (size_t i = 0; i < m_arTextLines.size(); ++i)
		{
			auto& pCurrLine = m_arTextLines[i];
			if (!pCurrLine)
				continue;

			for (size_t j = 0; j < pCurrLine->m_arConts.size(); ++j)
			{
				auto& pCurrCont = pCurrLine->m_arConts[j];
				if (!pCurrCont)
					continue;

				if (pCurrCont->m_iNumDuplicates > 0)
					pCurrLine->m_iNumDuplicates = std::max(pCurrLine->m_iNumDuplicates, pCurrCont->m_iNumDuplicates);
			}
			pCurrLine->MergeConts();
		}
		DetermineDominantGraphics();
	}

	void CPage::DetermineDominantGraphics()
	{
		std::shared_ptr<CShape> pDominantShape = nullptr;

		for (size_t i = 0; i < m_arTextLines.size(); ++i)
		{
			auto pLine = m_arTextLines[i];
			if (!pLine)
				continue;

			for (size_t j = 0; j < pLine->m_arConts.size(); ++j)
			{
				auto pCont = pLine->m_arConts[j];
				if (!pCont)
					continue;

				if (pCont->m_pShape && pCont->m_pShape != pDominantShape)
				{
					if (pCont->m_pShape->m_dLeft < pCont->m_dLeft &&
							pCont->m_pShape->m_dRight > pCont->m_dRight)
					{
						if (!pDominantShape ||
								(pCont->m_pShape->m_dLeft < pDominantShape->m_dLeft &&
								 pCont->m_pShape->m_dRight > pDominantShape->m_dRight))
						{
							pDominantShape = pCont->m_pShape;
						}
					}
				}
			}

			pLine->m_pDominantShape = pDominantShape;
			pDominantShape = nullptr;
		}
	}
	void CPage::BuildParagraphes()
	{
		std::shared_ptr<CTextLine> pCurrLine, pNextLine, pNextNextLine, pPrevLine;
		double dCurrBeforeSpacing = 0, dNextBeforeSpacing = 0, dPrevBeforeSpacing = 0;
		double dBeforeSpacingWithShapes = 0;

		//note Все параграфы были сдвинуты на данное значение от верхнего края страницы
		double dPreviousStringBaseline = c_dCORRECTION_FOR_FIRST_PARAGRAPH;
		eVerticalCrossingType eCrossingType;

		bool bIf1, bIf2, bIf3, bIf4, bIf5, bIf6, bIf7;
		bool bIsNeedParagraphToShape = m_eTextAssociationType == TextAssociationType::tatParagraphToShape;

		size_t nIndexForCheking = c_nAntiZero;

		double avg_height = 0;
		size_t n = 0;

		for (size_t nIndex = 0; nIndex < m_arTextLines.size(); ++nIndex)
		{
			pCurrLine = m_arTextLines[nIndex];
			avg_height = (avg_height / (n + 1)) * n + (pCurrLine->m_dHeight / (n + 1));

			if (!pCurrLine)
				continue;

			if (m_eTextAssociationType == TextAssociationType::tatShapeLine)
			{
				CreateSingleLineShape(pCurrLine);
				continue;
			}

			dPrevBeforeSpacing = dCurrBeforeSpacing;
			dCurrBeforeSpacing = pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight - dPreviousStringBaseline;
			dPreviousStringBaseline = pCurrLine->m_dBaselinePos;

			//Если у текущей линии есть дубликаты, то создаем из них шейпы
			if (pCurrLine->m_iNumDuplicates > 0)
			{
				dBeforeSpacingWithShapes += dCurrBeforeSpacing + pCurrLine->m_dHeight;

				auto iNumDuplicates = pCurrLine->m_iNumDuplicates;
				CreateSingleLineShape(pCurrLine);
				while (iNumDuplicates > 0)
				{
					CreateSingleLineShape(pCurrLine);
					iNumDuplicates--;
				}
				continue;
			}

			if (m_eTextAssociationType == TextAssociationType::tatPlainLine)
			{
				CreateSingleLineParagraph(pCurrLine, m_dWidth, dCurrBeforeSpacing);
				continue;
			}

			pNextLine = GetNextTextLine(nIndex, &nIndexForCheking);
			if (bIsNeedParagraphToShape)
			{
				pPrevLine = GetPrevTextLine(nIndex);
			}

			//Если две линии пересекаются, то создаем из них шейпы
			if (pNextLine)
			{
				eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine.get());
				bool bIsPassed = false;
				double dCurrentAdditive = 0.0;

				switch (eCrossingType)
				{
				case eVerticalCrossingType::vctCurrentInsideNext:
				case eVerticalCrossingType::vctCurrentBelowNext:
					dCurrentAdditive = dCurrBeforeSpacing + pCurrLine->m_dHeight + pNextLine->m_dBaselinePos - pCurrLine->m_dBaselinePos;
					dPreviousStringBaseline = pNextLine->m_dBaselinePos;
					bIsPassed = true;
					break;
				case eVerticalCrossingType::vctCurrentOutsideNext:
				case eVerticalCrossingType::vctCurrentAboveNext:
				case eVerticalCrossingType::vctDublicate:
					dCurrentAdditive = dCurrBeforeSpacing + pCurrLine->m_dHeight;
					bIsPassed = true;
					break;
				default:
					break;
				}

				if (bIsPassed)
				{
					CreateSingleLineShape(pCurrLine);
					CreateSingleLineShape(pNextLine);

					dBeforeSpacingWithShapes += dCurrentAdditive;

					nIndex++;
					continue;
				}
			}

			dCurrBeforeSpacing += dBeforeSpacingWithShapes;
			dBeforeSpacingWithShapes = 0;

			bool bIsSingleLineParagraph = false;

			//Логика определения параметров для DetermineTextAlignmentType
			if (pNextLine)
			{
				dNextBeforeSpacing = pNextLine->m_dBaselinePos - pNextLine->m_dHeight - dPreviousStringBaseline;
				//dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(dPreviousStringBaseline);

				//Высота строк должна быть примерно одинаковой
				bIf1 = fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM;
				//расстрояние между строк тоже одинаково
				bIf2 = fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM;
				//или
				bIf3 = dCurrBeforeSpacing > dNextBeforeSpacing;
				//нет отступа
				bIf4 = fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;
				//есть отступ
				bIf5 = pCurrLine->m_dLeft > pNextLine->m_dLeft;
				//совпадают правые границы
				bIf6 = fabs(pCurrLine->m_dRight - pNextLine->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;

				size_t nNextIndex = nIndex+1;
				pNextNextLine = GetNextTextLine(nNextIndex);

				bIf7 = (pCurrLine->m_dWidth > pNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH) &&
						(pNextNextLine ? pCurrLine->m_dWidth > pNextNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH : true);

				if (pNextNextLine)
				{
					//double dNextNextBeforeSpacing = pNextNextLine->CalculateBeforeSpacing(pNextLine->m_dBaselinePos);
					double dNextNextBeforeSpacing =	pNextNextLine->m_dBaselinePos - pNextNextLine->m_dHeight - dPreviousStringBaseline;
					if (bIf1 && (bIf2 || bIf3))
					{
						if (fabs(dNextBeforeSpacing - dNextNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM)
						{
							if (fabs(pNextLine->m_dHeight - pNextNextLine->m_dHeight) >= c_dTHE_SAME_STRING_Y_PRECISION_MM)
								pNextNextLine = nullptr;
						}
						else
						{
							if (fabs(pNextLine->m_dHeight - pNextNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
							{
								if (fabs(dNextBeforeSpacing - dNextNextBeforeSpacing) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
									pNextNextLine = nullptr;
								else
									bIsSingleLineParagraph = true;
							}
							else
								pNextNextLine = nullptr;
						}
					}
				}
			}

			bool bIsUseNextNextLine = true;
			CParagraph::TextAlignmentType eTextAlignmentType = CParagraph::DetermineTextAlignmentType(
						pCurrLine, pNextLine, pNextNextLine, m_dWidth, bIsUseNextNextLine, bIsSingleLineParagraph);

			auto pParagraph = std::make_shared<CParagraph>();

			pParagraph->m_dLineHeight = avg_height;
			avg_height = 0;
			n = 0;

			pParagraph->m_eTextAlignmentType = eTextAlignmentType;

			if (pNextLine && !bIsSingleLineParagraph && bIf1 && (bIf2 || bIf3))
			{
				pParagraph->m_dLeft = std::min(pCurrLine->m_dLeft, pNextLine->m_dLeft);
				pParagraph->m_dLeftBorder = pParagraph->m_dLeft;
				pParagraph->m_dRight = std::max(pCurrLine->m_dRight, pNextLine->m_dRight);
				pParagraph->m_dRightBorder = m_dWidth - pParagraph->m_dRight;
				pParagraph->m_dWidth = pParagraph->m_dRight - pParagraph->m_dLeft;
				if (pParagraph->m_eTextAlignmentType != CParagraph::tatByCenter)
				{
					pParagraph->m_bIsNeedFirstLineIndent = pCurrLine->m_dLeft > pNextLine->m_dLeft ? true : false;
					pParagraph->m_dFirstLine = pCurrLine->m_dLeft - pNextLine->m_dLeft;
				}
			}
			else
			{
				pParagraph->m_dLeft = pCurrLine->m_dLeft;
				pParagraph->m_dLeftBorder = pParagraph->m_dLeft;
				pParagraph->m_dRight = pCurrLine->m_dRight;
				pParagraph->m_dRightBorder = m_dWidth - pParagraph->m_dRight;
				pParagraph->m_dWidth = pCurrLine->m_dWidth;

				pParagraph->m_bIsNeedFirstLineIndent = false;
				pParagraph->m_dFirstLine = 0;
			}

			pParagraph->m_dTop = pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight;
			pParagraph->m_dBaselinePos = pCurrLine->m_dBaselinePos;
			pParagraph->m_dHeight = pCurrLine->m_dHeight;

			//размер строк во всем параграфе
			// pParagraph->m_dLineHeight = avg_height; //pCurrLine->m_dHeight;
			pParagraph->m_dSpaceBefore = std::max(dCurrBeforeSpacing, 0.0);

			pParagraph->m_arLines.push_back(pCurrLine);
			pParagraph->m_nNumLines++;

			if (pNextLine && !bIsSingleLineParagraph && bIf1 && (bIf2 || bIf3) && (bIf4 || bIf5 || bIf6) && bIf7)
			{
				pParagraph->m_arLines.push_back(pNextLine);
				pParagraph->m_nNumLines++;

				if (pCurrLine->IsShadingPresent(pNextLine.get()))
				{
					pParagraph->m_bIsShadingPresent = true;
					pParagraph->m_lColorOfShadingFill = pCurrLine->m_pDominantShape->m_oBrush.Color1;
				}

				//сдвигаем рабочую точку
				nIndex++;
				pCurrLine = pNextLine;
				pNextLine = GetNextTextLine(nIndex, &nIndexForCheking);

				dPrevBeforeSpacing = dCurrBeforeSpacing;
				dCurrBeforeSpacing = (pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight) - dPreviousStringBaseline; //pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);;
				dPreviousStringBaseline = pCurrLine->m_dBaselinePos;
				double dCorrectionBeforeSpacing = dCurrBeforeSpacing;

				if (bIsUseNextNextLine)
				{
					if (pNextLine)
					{
						dNextBeforeSpacing = (pNextLine->m_dBaselinePos - pCurrLine->m_dHeight) - dPreviousStringBaseline; //pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);;
						eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine.get());

						bIf1 = fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM; //высота строк должна быть примерно одинаковой
						bIf2 = fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM; //расстрояние между строк тоже одинаково
						bIf3 = (eCrossingType == eVerticalCrossingType::vctUnknown ||
								eCrossingType == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
								eCrossingType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext);
						bIf4 = ((pParagraph->m_eTextAlignmentType == CParagraph::tatByLeftEdge && fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM) ||
								(pParagraph->m_eTextAlignmentType == CParagraph::tatByWidth && fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM && (fabs(pCurrLine->m_dRight - pNextLine->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM || pCurrLine->m_dRight > pNextLine->m_dRight)) ||
								(pParagraph->m_eTextAlignmentType == CParagraph::tatByRightEdge && fabs(pCurrLine->m_dRight - pNextLine->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM) ||
								(pParagraph->m_eTextAlignmentType == CParagraph::tatByCenter));
						bIf5 = (pCurrLine->m_dWidth > pNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH);
					}

					//проверим, подходят ли следующие строчки для текущего pParagraph
					while(pNextLine &&  bIf1 && bIf2 && bIf3 && bIf4 && bIf5)
					{
						pParagraph->m_arLines.push_back(pNextLine);
						pParagraph->m_nNumLines++;

						pParagraph->m_dLeft = std::min(pParagraph->m_dLeft, pNextLine->m_dLeft);
						pParagraph->m_dLeftBorder = pParagraph->m_dLeft;
						pParagraph->m_dRight = std::max(pParagraph->m_dRight, pNextLine->m_dRight);
						pParagraph->m_dRightBorder = m_dWidth - pParagraph->m_dRight;
						pParagraph->m_dWidth = pParagraph->m_dRight - pParagraph->m_dLeft;
						pParagraph->m_dBaselinePos = pNextLine->m_dBaselinePos;

						if (!pCurrLine->IsShadingPresent(pNextLine.get()))
						{
							pParagraph->m_bIsShadingPresent = false;
							pParagraph->m_lColorOfShadingFill = c_iWhiteColor;
						}

						//сдвигаем рабочую точку
						nIndex++;
						pCurrLine = pNextLine;
						pNextLine = GetNextTextLine(nIndex, &nIndexForCheking);

						dPrevBeforeSpacing = dCurrBeforeSpacing;
						dCurrBeforeSpacing = (pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight) - dPreviousStringBaseline; //pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);
						dPreviousStringBaseline = pCurrLine->m_dBaselinePos;
						dCorrectionBeforeSpacing = (dCorrectionBeforeSpacing + dCurrBeforeSpacing) / 2; //наверное лучше так... текст может быть уже, чем в оригинале

						if (pNextLine)
						{
							dNextBeforeSpacing = (pNextLine->m_dBaselinePos - pCurrLine->m_dHeight) - dPreviousStringBaseline; //pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);;
							eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine.get());

							bIf1 = fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM; //высота строк должна быть примерно одинаковой
							bIf2 = fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM; //расстрояние между строк тоже одинаково
							bIf3 = (eCrossingType == eVerticalCrossingType::vctUnknown ||
									eCrossingType == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
									eCrossingType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext);
							bIf4 = ((pParagraph->m_eTextAlignmentType == CParagraph::tatByLeftEdge && fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM) ||
									(pParagraph->m_eTextAlignmentType == CParagraph::tatByWidth && fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM && (fabs(pCurrLine->m_dRight - pNextLine->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM || pCurrLine->m_dRight > pNextLine->m_dRight)) ||
									(pParagraph->m_eTextAlignmentType == CParagraph::tatByRightEdge && fabs(pCurrLine->m_dRight - pNextLine->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM) ||
									(pParagraph->m_eTextAlignmentType == CParagraph::tatByCenter));
							bIf5 = (pCurrLine->m_dWidth > pNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH);
						}
					}
				}

				if (eCrossingType != eVerticalCrossingType::vctUnknown &&
						eCrossingType != eVerticalCrossingType::vctNoCrossingCurrentAboveNext &&
						eCrossingType != eVerticalCrossingType::vctNoCrossingCurrentBelowNext)
				{
					CreateSingleLineShape(pNextLine);
					nIndex++;
				}

				//коррекция
				pParagraph->m_dLineHeight += dCorrectionBeforeSpacing;
				pParagraph->m_dSpaceBefore = fabs(pParagraph->m_dSpaceBefore - dCorrectionBeforeSpacing);

				pParagraph->RemoveHighlightColor();
				pParagraph->MergeLines();
			}
			else
			{
				if (pCurrLine->m_pDominantShape)
				{
					pParagraph->m_bIsShadingPresent = true;
					pParagraph->m_lColorOfShadingFill = pCurrLine->m_pDominantShape->m_oBrush.Color1;
					pParagraph->RemoveHighlightColor();
				}
			}

			if (bIsNeedParagraphToShape)
			{
				bool bIsSameTypeText = pPrevLine && fabs(dPrevBeforeSpacing - dCurrBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM;
				CreateShapeFormParagraphs(pParagraph, bIsSameTypeText);
			}
			else
			{
				pParagraph->m_wsStyleId = m_pParagraphStyleManager->GetDefaultParagraphStyleId(*pParagraph);
				m_arOutputObjects.push_back(pParagraph);
			}

			if (nIndexForCheking != c_nAntiZero)
			{
				nIndex = nIndexForCheking - 1;
				nIndexForCheking = c_nAntiZero;
			}
		}

		if (bIsNeedParagraphToShape)
		{
			CorrectionObjectesInShapes(m_dWidth);
		}

		using output_ptr = std::shared_ptr<COutputObject>;
		std::sort(m_arOutputObjects.begin(), m_arOutputObjects.end(), [] (const output_ptr& a, const output_ptr& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});
	}

	void CPage::CreateSingleLineParagraph(std::shared_ptr<CTextLine> pLine, double dPageWidth, double pBeforeSpacing)
	{
		auto pParagraph = std::make_shared<CParagraph>();
		pParagraph->m_arLines.push_back(pLine);

		pParagraph->m_dLeft = pLine->m_dLeft;
		pParagraph->m_dTop = pLine->m_dBaselinePos - pLine->m_dHeight;
		pParagraph->m_dFirstLine = 0;
		pParagraph->m_dRight = pLine->m_dRight;
		pParagraph->m_dRightBorder = dPageWidth - pParagraph->m_dRight;
		pParagraph->m_dWidth = pLine->m_dWidth;
		pParagraph->m_dHeight = pLine->m_dHeight;
		if (pBeforeSpacing < 0)
		{
			pParagraph->m_dHeight += pBeforeSpacing;
		}

		pParagraph->m_dSpaceBefore = std::max(pBeforeSpacing, 0.0);

		if (pLine->m_pDominantShape)
		{
			pParagraph->m_bIsShadingPresent = true;
			pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
			pParagraph->RemoveHighlightColor();
		}

		m_arOutputObjects.push_back(std::dynamic_pointer_cast<COutputObject>(pParagraph));
	}

	void CPage::CreateSingleLineShape(std::shared_ptr<CTextLine> pLine)
	{
		auto pParagraph = std::make_shared<CParagraph>();

		pParagraph->m_arLines.push_back(pLine);
		pParagraph->m_dRightBorder = 0;

		if (pLine->m_pDominantShape)
		{
			pParagraph->m_bIsShadingPresent = true;
			pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
			pParagraph->RemoveHighlightColor();
		}

		auto pShape = std::make_shared<CShape>();
		pShape->m_arOutputObjects.push_back(pParagraph);
		pShape->m_eType = CShape::eShapeType::stTextBox;
		pShape->m_dLeft = pLine->m_dLeft;
		pShape->m_dTop = pLine->m_dBaselinePos - pLine->m_dHeight;
		pShape->m_dWidth = pLine->m_dWidth;
		pShape->m_dHeight = pLine->m_dHeight;
		pShape->m_bIsBehindDoc = false;

		m_arOutputObjects.push_back(pShape);
	}

	void CPage::CreateShapeFormParagraphs(std::shared_ptr<CParagraph> pParagraph, bool bIsSameTypeText)
	{
		if (!pParagraph)
			return;

		bool bIsShapesPresent = false;
		std::shared_ptr<CShape> pBackShape = nullptr;

		for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
		{
			if (m_arOutputObjects[i]->m_eType != COutputObject::eOutputType::etShape)
				continue;

			bIsShapesPresent = true;
			pBackShape = std::dynamic_pointer_cast<CShape>(m_arOutputObjects[i]);
		}

		std::shared_ptr<CShape> pShape;
		if (bIsSameTypeText && bIsShapesPresent)
		{
			pShape = pBackShape;
			pShape->m_dHeight = pParagraph->m_dLineHeight * pParagraph->m_nNumLines + pParagraph->m_dSpaceBefore;
		}
		else
		{
			pShape = std::make_shared<CShape>();
			pParagraph->m_dSpaceBefore = 0;
			pShape->m_dHeight = pParagraph->m_dLineHeight * pParagraph->m_nNumLines;
		}

		pShape->m_dLeft = pShape->m_dLeft > 0 ? std::min(pShape->m_dLeft, pParagraph->m_dLeft) : pParagraph->m_dLeft;
		pShape->m_dTop = pShape->m_dTop > 0 ? std::min(pShape->m_dTop, pParagraph->m_dTop) : pParagraph->m_dTop;
		pShape->m_dRight = pShape->m_dRight > 0 ? std::max(pShape->m_dRight, pParagraph->m_dRight) : pParagraph->m_dRight;
		pShape->m_dBaselinePos = pShape->m_dBaselinePos > 0 ? std::max(pShape->m_dBaselinePos, pParagraph->m_dBaselinePos) : pParagraph->m_dBaselinePos;
		pShape->m_dWidth = fabs(pShape->m_dRight - pShape->m_dLeft);

		pParagraph->m_dLeftBorder = 0;
		pParagraph->m_dRightBorder = 0;

		pShape->m_arOutputObjects.push_back(pParagraph);
		pShape->m_eType = CShape::eShapeType::stTextBox;
		pShape->m_bIsBehindDoc = false;

		if (!bIsSameTypeText)
		{
			m_arOutputObjects.push_back(pShape);
		}
	}

	void CPage::CorrectionObjectesInShapes(double dPageWidth)
	{
		for (size_t i = 0; i <  m_arOutputObjects.size(); ++i)
		{
			if (m_arOutputObjects[i]->m_eType != COutputObject::eOutputType::etShape)
			{
				continue;
			}

			auto pShape = std::dynamic_pointer_cast<CShape>(m_arOutputObjects[i]);

			if (!pShape ||
					pShape->m_eType != CShape::eShapeType::stTextBox ||
					pShape->m_arOutputObjects.empty())
			{
				continue;
			}

			for (size_t j = 0; j < pShape->m_arOutputObjects.size(); ++j)
			{
				auto pObj = pShape->m_arOutputObjects[j];

				switch(pObj->m_eType)
				{
				case COutputObject::eOutputType::etParagraph:
				{
					auto pParagraph = std::dynamic_pointer_cast<CParagraph>(m_arOutputObjects[i]);

					if (pParagraph->m_dLeft > pShape->m_dLeft && pParagraph->m_nNumLines == 1)
					{
						pParagraph->m_bIsNeedFirstLineIndent = true;
						pParagraph->m_dFirstLine = pParagraph->m_dLeft - pShape->m_dLeft;
						pParagraph->m_dLeft = 0;
					}

					pParagraph->m_dLeftBorder = pParagraph->m_dLeft > pShape->m_dLeft ? fabs(pParagraph->m_dLeft - pShape->m_dLeft) : 0;
					pParagraph->m_dRightBorder = pParagraph->m_dRight < pShape->m_dRight ? fabs(pShape->m_dRight - pParagraph->m_dRight) : 0;
				}
					break;
				default:
					break;
				}

			}
		}
	}

	std::shared_ptr<CTextLine> CPage::GetNextTextLine(size_t& nCurrentIndex, size_t* pIndexForCheking)
	{
		std::shared_ptr<CTextLine> pLine;
		for (size_t nIndex = nCurrentIndex + 1; nIndex < m_arTextLines.size(); ++nIndex)
		{
			pLine = m_arTextLines[nIndex];
			bool bIf1 = pLine == nullptr;
			bool bIf2 = pIndexForCheking && pLine->m_iNumDuplicates > 0;

			if (bIf1 || bIf2)
			{
				if (bIf2)
				{
					if (*pIndexForCheking == c_nAntiZero)
						*pIndexForCheking = nIndex;
				}

				nCurrentIndex++; //note изменяем входной индекс, чтобы не выбирать те же строки
				pLine = nullptr;
				continue;
			}
			else
				break;
		}
		return pLine;
	}

	std::shared_ptr<CTextLine> CPage::GetPrevTextLine(size_t nCurrentIndex)
	{
		std::shared_ptr<CTextLine> pLine = nullptr;

		if (nCurrentIndex)
		{
			for (size_t nIndex = nCurrentIndex - 1; nIndex > 0; --nIndex)
			{
				pLine = m_arTextLines[nIndex];

				if (!pLine)
				{
					pLine = nullptr;
					continue;
				}
				else
					break;
			}
		}
		return pLine;
	}


	void CPage::WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter)
	{
		// section
		int lWidthDx  = (int)(m_dWidth * c_dMMToDx);
		int lHeightDx = (int)(m_dHeight * c_dMMToDx);

		if (!bLastPage)
			oWriter.WriteString(L"<w:p><w:pPr><w:sectPr>");
		else
			oWriter.WriteString(L"<w:sectPr>");

		oWriter.WriteString(L"<w:pgSz w:w=\"");
		oWriter.AddInt((int)(m_dWidth * c_dMMToDx));
		oWriter.WriteString(L"\" w:h=\"");
		oWriter.AddInt((int)(m_dHeight * c_dMMToDx));
		oWriter.WriteString(L"\" w:orient=\"");
		(lWidthDx >= lHeightDx) ? oWriter.WriteString(L"landscape") : oWriter.WriteString(L"portrait");
		oWriter.WriteString(L"\"/>");

		if (!bLastPage)
			oWriter.WriteString(L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\"/></w:sectPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr></w:p>");
		else
			oWriter.WriteString(L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\" w:header=\"0\" w:footer=\"0\" w:gutter=\"0\"/></w:sectPr>");
	}
}
