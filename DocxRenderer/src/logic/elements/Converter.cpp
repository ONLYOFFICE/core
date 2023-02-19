#include "Converter.h"
#include "Shape.h"
#include "src/resources/utils.h"

namespace NSDocxRenderer
{
	//общая функция для сборки строк в любом текстовом объекте
	void CConverter::BuildLines(std::vector<CTextLine*>& rTextLines)
	{
		for (size_t i = 0; i < rTextLines.size(); ++i)
		{
			auto pCurrLine = rTextLines[i];

			if (pCurrLine->m_bIsNotNecessaryToUse)
			{
				continue;
			}

			for (size_t j = 0; j < pCurrLine->m_arConts.size(); ++j)
			{
				auto pCurrCont = pCurrLine->m_arConts[j];

				if (pCurrCont->m_bIsNotNecessaryToUse)
				{
					continue;
				}

				if (pCurrCont->m_iNumDuplicates > 0)
				{
					pCurrLine->m_iNumDuplicates = std::max(pCurrLine->m_iNumDuplicates, pCurrCont->m_iNumDuplicates);
				}
			}

			pCurrLine->MergeConts();
		}

		DetermineDominantGraphics(rTextLines);
	}

	void CConverter::DetermineDominantGraphics(std::vector<CTextLine*>& rTextLines)
	{
		CShape* pDominantShape = nullptr;

		for (size_t i = 0; i < rTextLines.size(); ++i)
		{
			auto pLine = rTextLines[i];
			if (pLine->m_bIsNotNecessaryToUse)
			{
				continue;
			}

			for (size_t j = 0; j < pLine->m_arConts.size(); ++j)
			{
				auto pCont = pLine->m_arConts[j];
				if (pCont->m_bIsNotNecessaryToUse)
				{
					continue;
				}

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

	void CConverter::BuildParagraphes(double dPageWidth, eTextAssociationType eType, CBaseItem::ElemType eBaseType,
									  std::vector<CTextLine*>& rTextLines,
									  std::vector<CBaseItem*>& rOutputObjects)
	{
		std::vector<CTable*> oStubVector; //просто объект-заглушка
		BuildParagraphes(dPageWidth, eType, eBaseType, rTextLines, oStubVector, rOutputObjects);
	}

	// eBaseType == etCell или etParagraph
	// eType == 2 - 5 из eTextAssociationType
	void CConverter::BuildParagraphes(double dPageWidth, eTextAssociationType eType,
									  CBaseItem::ElemType eBaseType, std::vector<CTextLine*>& rTextLines,
									  std::vector<CTable*>& rTables, std::vector<CBaseItem*> &rOutputObjects)
	{
		CTextLine* pCurrLine, *pNextLine, *pNextNextLine, *pPrevLine;
		double dCurrBeforeSpacing = 0, dNextBeforeSpacing = 0, dPrevBeforeSpacing = 0;
		double dBeforeSpacingWithShapes = 0;
		//note Все параграфы были сдвинуты на данное значение от верхнего края страницы
		double dPreviousStringBaseline = c_dCORRECTION_FOR_FIRST_PARAGRAPH;
		eVerticalCrossingType eCrossingType;

		bool bIf1, bIf2, bIf3, bIf4, bIf5, bIf6, bIf7;
		bool bIsNeedParagraphToShape = eType == eTextAssociationType::tatParagraphToShape && eBaseType == CBaseItem::ElemType::etParagraph;

		CTable* pCurrTable = nullptr;
		size_t nTableIndex = 0;

		size_t nIndexForCheking = c_nAntiZero;

		if (!rTables.empty())
		{
			CBaseItem::SortByBaseline(rTables);
			pCurrTable = rTables.front();
			nTableIndex = 0;

			//Если линий нет, то добавлем сразу
			if (rTextLines.empty())
			{
				for (size_t i = 0; i < rTables.size(); ++i)
				{
					if (bIsNeedParagraphToShape)
					{
						CreateShapeFormTable(pCurrTable, rOutputObjects);
					}
					else
					{
						rOutputObjects.push_back(rTables[i]);
					}
				}
			}
		}

		CBaseItem::SortByBaseline(rTextLines);

		for (size_t nIndex = 0; nIndex < rTextLines.size(); ++nIndex)
		{
			pCurrLine = rTextLines[nIndex];
			if (pCurrLine->m_bIsNotNecessaryToUse)
			{
				continue;
			}

			if (eType == eTextAssociationType::tatShapeLine)
			{
				CreateSingleLineShape(pCurrLine, rOutputObjects);
				continue;
			}


			while (pCurrTable)
			{
				eCrossingType = pCurrLine->GetVerticalCrossingType(pCurrTable);

				//добавляем таблицу в общий массив, если она идет после текущей строки
				if (eCrossingType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext)
				{
					if (bIsNeedParagraphToShape)
					{
						CreateShapeFormTable(pCurrTable, rOutputObjects);
					}
					else
					{
						rOutputObjects.push_back(pCurrTable);
					}
					dCurrBeforeSpacing = pCurrTable->CalculateBeforeSpacing(dPreviousStringBaseline);
					pCurrTable->m_dSpaceBefore = std::max(dCurrBeforeSpacing, 0.0);
					if (eType != eTextAssociationType::tatParagraphToShape || eBaseType != CBaseItem::ElemType::etParagraph)
					{
						pCurrTable->m_bIsNeedSpacing = true;
					}
					dPreviousStringBaseline = pCurrTable->m_dBaselinePos;
					pCurrTable = nullptr;
					//таблицы отсортированы, можно сразу взять следующую для проверки
					for (size_t i = nTableIndex + 1; i < rTables.size(); ++i)
					{
						pCurrTable = rTables[i];
					}
				}
				else
				{
					break;
				}
			}

			dPrevBeforeSpacing = dCurrBeforeSpacing;
			if (eBaseType == CBaseItem::ElemType::etCell && nIndex == 0)
			{
				dCurrBeforeSpacing = 0;
			}
			else
			{
				dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);
			}
			dPreviousStringBaseline = pCurrLine->m_dBaselinePos;

			//Если у текущей линии есть дубликаты, то создаем из них шейпы
			if (pCurrLine->m_iNumDuplicates > 0)
			{
				dBeforeSpacingWithShapes += dCurrBeforeSpacing + pCurrLine->m_dHeight;

				auto iNumDuplicates = pCurrLine->m_iNumDuplicates;
				CreateSingleLineShape(pCurrLine, rOutputObjects);
				while (iNumDuplicates > 0)
				{
					CreateSingleLineShape(pCurrLine, rOutputObjects);
					iNumDuplicates--;
				}
				continue;
			}

			if (eType == eTextAssociationType::tatPlainLine)
			{
				CreateSingleLineParagraph(pCurrLine, dPageWidth, &dCurrBeforeSpacing, rOutputObjects);
				continue;
			}

			pNextLine = GetNextTextLine(nIndex, rTextLines, &nIndexForCheking);
			if (bIsNeedParagraphToShape)
			{
				pPrevLine = GetPrevTextLine(nIndex, rTextLines);
			}

			//Если две линии пересекаются, то создаем из них шейпы
			if (pNextLine)
			{
				eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine);
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
					CreateSingleLineShape(pCurrLine, rOutputObjects);
					CreateSingleLineShape(pNextLine, rOutputObjects);

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
				dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(dPreviousStringBaseline);

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
				pNextNextLine = GetNextTextLine(nNextIndex, rTextLines);

				bIf7 = (pCurrLine->m_dWidth > pNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH) &&
						(pNextNextLine ? pCurrLine->m_dWidth > pNextNextLine->m_dWidth * c_dCOEFFICIENT_LENGTHS_LINES_IN_PARAGRAPH : true);

				if (pNextNextLine)
				{
					double dNextNextBeforeSpacing = pNextNextLine->CalculateBeforeSpacing(pNextLine->m_dBaselinePos);

					if (bIf1 && (bIf2 || bIf3))
					{
						if (fabs(dNextBeforeSpacing - dNextNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM)
						{
							if (fabs(pNextLine->m_dHeight - pNextNextLine->m_dHeight) >= c_dTHE_SAME_STRING_Y_PRECISION_MM)
							{
								pNextNextLine = nullptr;
							}
						}
						else
						{
							if (fabs(pNextLine->m_dHeight - pNextNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
							{
								if (dNextBeforeSpacing < dNextNextBeforeSpacing)
								{
									pNextNextLine = nullptr;
								}
								else
								{
									bIsSingleLineParagraph = true;
								}
							}
							else
							{
								pNextNextLine = nullptr;
							}
						}
					}
				}
			}

			bool bIsUseNextNextLine = true;
			CParagraph::TextAlignmentType eTextAlignmentType = CParagraph::DetermineTextAlignmentType(
						pCurrLine, pNextLine, pNextNextLine, dPageWidth, bIsUseNextNextLine, bIsSingleLineParagraph);

			auto pParagraph = new CParagraph();
			pParagraph->m_eTextAlignmentType = eTextAlignmentType;

			if (pNextLine && !bIsSingleLineParagraph && bIf1 && (bIf2 || bIf3))
			{
				pParagraph->m_dLeft = std::min(pCurrLine->m_dLeft, pNextLine->m_dLeft);
				pParagraph->m_dLeftBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : pParagraph->m_dLeft;
				pParagraph->m_dRight = std::max(pCurrLine->m_dRight, pNextLine->m_dRight);
				pParagraph->m_dRightBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : dPageWidth - pParagraph->m_dRight;
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
				pParagraph->m_dLeftBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : pParagraph->m_dLeft;
				pParagraph->m_dRight = pCurrLine->m_dRight;
				pParagraph->m_dRightBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : dPageWidth - pParagraph->m_dRight;
				pParagraph->m_dWidth = pCurrLine->m_dWidth;

				pParagraph->m_bIsNeedFirstLineIndent = false;
				pParagraph->m_dFirstLine = 0;
			}

			pParagraph->m_dTop = pCurrLine->m_dTop;
			pParagraph->m_dBaselinePos = pCurrLine->m_dBaselinePos;
			pParagraph->m_dHeight = pCurrLine->m_dHeight;

			//размер строк во всем параграфе
			pParagraph->m_dLineHeight= pCurrLine->m_dHeight;
			pParagraph->m_dSpaceBefore = std::max(dCurrBeforeSpacing, 0.0);

			pParagraph->m_arLines.push_back(pCurrLine);
			pParagraph->m_nNumLines++;

			if (pNextLine && !bIsSingleLineParagraph && bIf1 && (bIf2 || bIf3) && (bIf4 || bIf5 || bIf6) && bIf7)
			{
				pParagraph->m_arLines.push_back(pNextLine);
				pParagraph->m_nNumLines++;

				if (pCurrLine->IsShadingPresent(pNextLine))
				{
					pParagraph->m_bIsShadingPresent = true;
					pParagraph->m_lColorOfShadingFill = pCurrLine->m_pDominantShape->m_oBrush.Color1;
				}

				//сдвигаем рабочую точку
				nIndex++;
				pCurrLine = pNextLine;
				pNextLine = GetNextTextLine(nIndex, rTextLines, &nIndexForCheking);

				dPrevBeforeSpacing = dCurrBeforeSpacing;
				dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);
				dPreviousStringBaseline = pCurrLine->m_dBaselinePos;
				double dCorrectionBeforeSpacing = dCurrBeforeSpacing;

				if (bIsUseNextNextLine)
				{
					if (pNextLine)
					{
						dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(dPreviousStringBaseline);
						eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine);

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
						pParagraph->m_dLeftBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : pParagraph->m_dLeft;
						pParagraph->m_dRight = std::max(pParagraph->m_dRight, pNextLine->m_dRight);
						pParagraph->m_dRightBorder = eBaseType == CBaseItem::ElemType::etCell ? 0 : dPageWidth - pParagraph->m_dRight;
						pParagraph->m_dWidth = pParagraph->m_dRight - pParagraph->m_dLeft;
						pParagraph->m_dBaselinePos = pNextLine->m_dBaselinePos;

						if (!pCurrLine->IsShadingPresent(pNextLine))
						{
							pParagraph->m_bIsShadingPresent = false;
							pParagraph->m_lColorOfShadingFill = c_iWhiteColor;
						}

						//сдвигаем рабочую точку
						nIndex++;
						pCurrLine = pNextLine;
						pNextLine = GetNextTextLine(nIndex, rTextLines, &nIndexForCheking);

						dPrevBeforeSpacing = dCurrBeforeSpacing;
						dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(dPreviousStringBaseline);
						dPreviousStringBaseline = pCurrLine->m_dBaselinePos;
						dCorrectionBeforeSpacing = (dCorrectionBeforeSpacing + dCurrBeforeSpacing)/2; //наверное лучше так... текст может быть уже, чем в оригинале

						if (pNextLine)
						{
							dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(dPreviousStringBaseline);
							eCrossingType = pCurrLine->GetVerticalCrossingType(pNextLine);

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
					CreateSingleLineShape(pNextLine, rOutputObjects);
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
				CreateShapeFormParagraphs(pParagraph, bIsSameTypeText, dPageWidth, rOutputObjects);
			}
			else
			{
				rOutputObjects.push_back(pParagraph);
			}

			if (nIndexForCheking != c_nAntiZero)
			{
				nIndex = nIndexForCheking - 1;
				nIndexForCheking = c_nAntiZero;
			}
		}

		if (bIsNeedParagraphToShape)
		{
			CorrectionObjectesInShapes(rOutputObjects, dPageWidth);
		}

		std::sort(rOutputObjects.begin(), rOutputObjects.end(), [](CBaseItem* a, CBaseItem* b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});
	}

	void CConverter::CreateSingleLineParagraph(CTextLine *pLine, double dPageWidth,
											   const double *pBeforeSpacing, std::vector<CBaseItem*>& rOutputObjects)
	{
		auto pParagraph = new CParagraph();
		pParagraph->m_arLines.push_back(pLine);

		pParagraph->m_dLeft = pLine->m_dLeft;
		pParagraph->m_dTop = pLine->m_dTop;
		pParagraph->m_dFirstLine = 0;
		pParagraph->m_dRight = pLine->m_dRight;
		pParagraph->m_dRightBorder = dPageWidth - pParagraph->m_dRight;
		pParagraph->m_dWidth = pLine->m_dWidth;
		pParagraph->m_dHeight = pLine->m_dHeight;
		if (*pBeforeSpacing < 0)
		{
			pParagraph->m_dHeight += *pBeforeSpacing;
		}

		pParagraph->m_dSpaceBefore = std::max(*pBeforeSpacing, 0.0);

		if (pLine->m_pDominantShape)
		{
			pParagraph->m_bIsShadingPresent = true;
			pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
			pParagraph->RemoveHighlightColor();
		}

		rOutputObjects.push_back(pParagraph);
	}

	void CConverter::CreateSingleLineShape(CTextLine *pLine, std::vector<CBaseItem *> &rOutputObjects)
	{
		auto pParagraph = new CParagraph();
		pParagraph->m_arLines.push_back(pLine);
		pParagraph->m_dRightBorder = 0;

		if (pLine->m_pDominantShape)
		{
			pParagraph->m_bIsShadingPresent = true;
			pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
			pParagraph->RemoveHighlightColor();
		}

		auto pShape = new CShape();
		pShape->m_arOutputObjects.push_back(pParagraph);
		pShape->m_eType = CShape::eShapeType::stTextBox;
		pShape->m_dLeft = pLine->m_dLeft;
		pShape->m_dTop = pLine->m_dTop;
		pShape->m_dWidth = pLine->m_dWidth;
		pShape->m_dHeight = pLine->m_dHeight;
		pShape->m_bIsBehindDoc = false;

		rOutputObjects.push_back(pShape);
	}

	void CConverter::CreateShapeFormParagraphs(CParagraph* pParagraph, bool bIsSameTypeText, double dPageWidth,
											   std::vector<CBaseItem *> &rOutputObjects)
	{
		if (!pParagraph)
		{
			return;
		}

		bool bIsShapesPresent = false;
		CShape* pBackShape = nullptr;

		for (size_t i = 0; i <  rOutputObjects.size(); ++i)
		{
			if (rOutputObjects[i]->m_eType != CBaseItem::ElemType::etShape)
			{
				continue;
			}
			bIsShapesPresent = true;
			pBackShape = dynamic_cast<CShape*>(rOutputObjects[i]);
		}

		CShape* pShape;

		if (bIsSameTypeText && bIsShapesPresent)
		{
			pShape = pBackShape;
			pShape->m_dHeight += pParagraph->m_dLineHeight * pParagraph->m_nNumLines + pParagraph->m_dSpaceBefore;
		}
		else
		{
			pShape = new CShape();
			pParagraph->m_dSpaceBefore = 0;
			pShape->m_dHeight += pParagraph->m_dLineHeight * pParagraph->m_nNumLines;
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
			rOutputObjects.push_back(pShape);
		}
	}

	void CConverter::CreateShapeFormTable(CTable* pTable, std::vector<CBaseItem *> &rOutputObjects)
	{
		if (!pTable)
		{
			return;
		}

		CShape* pShape;

		pShape = new CShape();
		pShape->m_dHeight = pTable->m_dHeight;
		pShape->m_dLeft = pTable->m_dLeft;
		pShape->m_dTop =pTable->m_dTop;
		pShape->m_dRight = pTable->m_dRight;
		pShape->m_dBaselinePos = pTable->m_dBaselinePos;
		pShape->m_dWidth = fabs(pShape->m_dRight - pShape->m_dLeft);

		pShape->m_arOutputObjects.push_back(pTable);
		pShape->m_eType = CShape::eShapeType::stTextBox;
		pShape->m_bIsBehindDoc = false;

		rOutputObjects.push_back(pShape);
	}

	void CConverter::CorrectionObjectesInShapes(std::vector<CBaseItem *> &rOutputObjects, double dPageWidth)
	{
		for (size_t i = 0; i <  rOutputObjects.size(); ++i)
		{
			if (rOutputObjects[i]->m_eType != CBaseItem::ElemType::etShape)
			{
				continue;
			}

			auto pShape = dynamic_cast<CShape*>(rOutputObjects[i]);

			if (pShape->m_bIsNotNecessaryToUse ||
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
				case CBaseItem::ElemType::etParagraph:
				{
					auto pParagraph = dynamic_cast<CParagraph*>(pObj);

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
				case CBaseItem::ElemType::etTable:
				{
					auto pTable = dynamic_cast<CTable*>(pObj);
					//todo
				}
					break;
				default:
					break;
				}

			}
		}
	}

	CTextLine* CConverter::GetNextTextLine(size_t& nCurrentIndex, std::vector<CTextLine*>& rTextLines, size_t* pIndexForCheking)
	{
		CTextLine* pLine = nullptr;

		for (size_t nIndex = nCurrentIndex + 1; nIndex < rTextLines.size(); ++nIndex)
		{
			pLine = rTextLines[nIndex];
			bool bIf1 = pLine->m_bIsNotNecessaryToUse;
			bool bIf2 = pIndexForCheking && pLine->m_iNumDuplicates > 0;

			if (bIf1 || bIf2)
			{
				if (bIf2)
				{
					if (*pIndexForCheking == c_nAntiZero)
					{
						*pIndexForCheking = nIndex;
					}
				}

				nCurrentIndex++; //note изменяем входной индекс, чтобы не выбирать те же строки
				pLine = nullptr;
				continue;
			}
			else
			{
				break;
			}
		}
		return pLine;
	}

	CTextLine* CConverter::GetPrevTextLine(size_t nCurrentIndex, std::vector<CTextLine*>& rTextLines)
	{
		CTextLine* pLine = nullptr;

		if (nCurrentIndex)
		{
			for (size_t nIndex = nCurrentIndex - 1; nIndex > 0; --nIndex)
			{
				pLine = rTextLines[nIndex];

				if (pLine->m_bIsNotNecessaryToUse)
				{
					pLine = nullptr;
					continue;
				}
				else
				{
					break;
				}
			}
		}
		return pLine;
	}
}
