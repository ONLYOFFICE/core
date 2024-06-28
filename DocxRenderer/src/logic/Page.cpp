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

	void CPage::Init(NSStructures::CFont* pFont,
		NSStructures::CPen* pPen,
		NSStructures::CBrush* pBrush,
		NSStructures::CShadow* pShadow,
		NSStructures::CEdgeText* pEdge,
		Aggplus::CMatrix* pMatrix,
		Aggplus::CGraphicsPathSimpleConverter* pSimple,
		CFontStyleManager* pFontStyleManager,
		CFontManager *pFontManager,
		CFontSelector* pFontSelector,
		CParagraphStyleManager* pParagraphStyleManager)
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
		m_arOutputObjects.clear();

		m_pCurrentLine = nullptr;
		m_oVector.Clear();

		m_arCompleteObjectsXml.clear();
	}

	CPage::~CPage()
	{
		Clear();
	}

	void CPage::DeleteTextClipPage()
	{
		if (m_bIsDeleteTextClipPage)
			for (auto& line : m_arTextLines)
				if (line && (line->m_dTop >= m_dHeight || line->m_dBaselinePos <= 0))
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

	void CPage::PathStart()
	{
	}

	void CPage::PathEnd()
	{
		m_oVector.End();
	}

	void CPage::PathClose()
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
				auto& pLastShape = m_arShapes.back();

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

	void CPage::CollectTextData(const PUINT pUnicodes,
		const PUINT pGids,
		const UINT& nCount,
		const double& fX,
		const double& fY,
		const double& fWidth,
		const double& fHeight,
		const double& fBaseLineOffset)
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

		// use forced fold option
		bool bForcedBold = oParams.bDefaultBold;
		if (m_lCurrentCommand == c_nStrokeTextType && m_pFont->Bold)
			bForcedBold = true;

		m_pFontSelector->SelectFont(oParams, oMetrics, oText);
		_h = m_pFontManager->GetFontHeight();

		pCont->m_dBaselinePos = dBaseLinePos;
		pCont->m_dTop         = pCont->m_dBaselinePos - _h;
		pCont->m_dHeight      = pCont->m_dBaselinePos - pCont->m_dTop;

		pCont->m_dLeft        = dTextX;
		pCont->m_dWidth       = _w;
		pCont->m_dRight       = dTextX + _w;

		pCont->m_oText        = oText;

		double font_size = m_pFont->Size;
		double em_height = oMetrics.dEmHeight;
		double ratio = font_size / em_height * c_dPixToMM;

		pCont->m_dTopWithAscent = pCont->m_dBaselinePos - (oMetrics.dAscent * ratio) - oMetrics.dBaselineOffset;
		pCont->m_dBotWithDescent = pCont->m_dBaselinePos + (oMetrics.dDescent * ratio) - oMetrics.dBaselineOffset;

		// первичное получение стиля для текущего символа
		// при дальнейшем анализе может измениться
		pCont->m_pFontStyle = m_pFontStyleManager->GetOrAddFontStyle(*m_pBrush,
			m_pFontSelector->GetSelectedName(),
			m_pFont->Size,
			m_pFontSelector->IsSelectedItalic(),
			m_pFontSelector->IsSelectedBold() || bForcedBold);

		pCont->m_dSpaceWidthMM = m_pFontManager->GetSpaceWidthMM();

		if (m_bUseDefaultFont)
		{
			pCont->m_oSelectedFont.Path = m_pFont->Path;
			pCont->m_oSelectedFont.Size = m_pFont->Size;
			pCont->m_oSelectedFont.FaceIndex = m_pFont->FaceIndex;
		}
		else
		{
			pCont->m_oSelectedFont.Name = m_pFontSelector->GetSelectedName();
			pCont->m_oSelectedFont.Size = m_pFont->Size;
			pCont->m_oSelectedFont.Bold = m_pFontSelector->IsSelectedBold();
			pCont->m_oSelectedFont.Italic = m_pFontSelector->IsSelectedItalic();
		}
		pCont->m_bWriteStyleRaw = m_bWriteStyleRaw;
		m_pParagraphStyleManager->UpdateAvgFontSize(m_pFont->Size);
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

	void CPage::Analyze()
	{
		// build m_arDiacriticalSymbols
		BuildDiacriticalSymbols();

		// build text lines from m_arConts
		BuildTextLines();

		// analyze shapes (get type of lines etc)
		AnalyzeShapes();

		// analyze text lines and conts inside
		AnalyzeTextLines();

		// merge conts in text lines
		BuildLines();

		// calc sizes on selected fonts for m_arConts
		CalcSelected();

		// build paragraphs from m_arTextLines
		BuildParagraphes();

		// merge shapes
		MergeShapes();
	}

	void CPage::Record(NSStringUtils::CStringBuilder& oWriter, bool bIsLastPage)
	{
		ToXml(oWriter);
		WriteSectionToFile(bIsLastPage, oWriter);
	}

	void CPage::BuildDiacriticalSymbols()
	{
		for (size_t i = 0; i < m_arConts.size(); i++)
			if (m_arConts[i] && m_arConts[i]->m_oText.length() == 1 && IsDiacriticalMark(m_arConts[i]->m_oText[0]))
				m_arDiacriticalSymbols.push_back(std::move(m_arConts[i]));

	}
	void CPage::BuildTextLines()
	{
		using cont_ptr_t = std::shared_ptr<CContText>;
		std::sort(m_arConts.begin(), m_arConts.end(), [] (const cont_ptr_t& a, const cont_ptr_t& b) {
			if(!a) return false;
			if(!b) return true;

			if (fabs(a->m_dBaselinePos - b->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
				return a->m_dLeft < b->m_dLeft;

			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		for (auto& cont : m_arConts)
			if (cont)
				AddContToTextLine(cont);
	}

	void CPage::MergeShapes()
	{
		if (m_arShapes.empty())
			return;

		using shape_ref_ptr_t = std::reference_wrapper<std::shared_ptr<CShape>>;
		for (size_t i = 0; i < m_arShapes.size() - 1; i++)
		{
			shape_ref_ptr_t val = m_arShapes[i];
			shape_ref_ptr_t next_val = m_arShapes[i + 1];

			if(!val.get() || ! next_val.get())
				continue;

			next_val.get()->TryMergeShape(val.get());
		}
	}

	void CPage::CalcSelected()
	{
		for (auto& cont : m_arConts)
		{
			if (cont && cont->m_oSelectedSizes.dHeight == 0.0 && cont->m_oSelectedSizes.dWidth == 0.0)
			{
				if (m_bUseDefaultFont)
				{
					cont->m_oSelectedSizes.dHeight = cont->m_dHeight;
					cont->m_oSelectedSizes.dWidth = cont->m_dWidth;
				}
				else
				{
					cont->CalcSelected();
				}
			}
		}
	}

	void CPage::AnalyzeShapes()
	{
		//BuildTables();
		DetermineLinesType();
	}

	void CPage::DetermineLinesType()
	{
		using shape_ptr_t = std::shared_ptr<CShape>;
		std::sort(m_arShapes.begin(), m_arShapes.end(), [] (const shape_ptr_t& a, const shape_ptr_t& b) {
			return a->m_dLeft < b->m_dLeft;
		});

		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			if (!m_arShapes[i] || m_arShapes[i]->m_dHeight > c_dMAX_LINE_HEIGHT_MM || // рассматриваем только тонкие объекты
				(m_arShapes[i]->m_eGraphicsType != eGraphicsType::gtRectangle &&
				m_arShapes[i]->m_eGraphicsType != eGraphicsType::gtCurve))
			{
				continue;
			}

			std::vector<size_t> curr_shape_indexes;
			curr_shape_indexes.push_back(i);

			for (size_t j = i + 1; j < m_arShapes.size(); ++j)
			{
				if (!m_arShapes[j] || m_arShapes[i]->AreObjectsNoCrossingByVertically(m_arShapes[j].get())) // значительно ускоряет работу
					continue;

				bool bIf1 = m_arShapes[i]->IsCorrelated(m_arShapes[j]);

				// довольно странное поведение - в зависимости от толщины линии информация о графике записывается в разные структуры
				bool bIf2 = m_arShapes[i]->m_oBrush.IsEqual(&m_arShapes[j]->m_oBrush);
				bool bIf3 = m_arShapes[i]->m_oPen.IsEqual(&m_arShapes[j]->m_oPen);

				// линия должна быть одного размера по высоте
				bool bIf4 = fabs(m_arShapes[i]->m_dHeight - m_arShapes[j]->m_dHeight) < c_dGRAPHICS_ERROR_IN_LINES_MM;

				// все должно быть на одной линии
				bool bIf5 = fabs(m_arShapes[i]->m_dBaselinePos - m_arShapes[j]->m_dBaselinePos) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5;

				if (bIf1 && (bIf2 || bIf3) && bIf4 && bIf5) // все должно быть на одной линии
					curr_shape_indexes.push_back(j);
			}

			if (curr_shape_indexes.size() > 1)
			{
				size_t j = 0;
				for (size_t k = 1; k < curr_shape_indexes.size(); ++k)
				{
					auto& first_shape = m_arShapes[curr_shape_indexes[j]];
					auto& second_shape = m_arShapes[curr_shape_indexes[k]];

					CShape::CheckLineType(first_shape, second_shape, k == curr_shape_indexes.size() - 1);
					if(!m_arShapes[j])
					{
						j = k;
						k++;
					}
				}
			}
			else if (curr_shape_indexes.size() == 1)
				CShape::CheckLineType(m_arShapes[curr_shape_indexes[0]]);

			curr_shape_indexes.clear();
		}
	}

	void CPage::AnalyzeTextLines()
	{
		// вся логика основана на отсортированных списках объектов
		using line_ptr_t = std::shared_ptr<CTextLine>;
		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const line_ptr_t& a, const line_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		// analyze drop caps (creates shapes)
		AnalyzeDropCaps();

		// analyze conts in text lines
		AnalyzeConts();

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

		for (size_t i = 0; i < m_arTextLines.size(); i++)
		{
			auto& line = m_arTextLines[i];
			for (auto& cont : line->m_arConts)
				if (cont && cont->m_pFontStyle->dFontSize > 2 * avg_font_size && cont->m_oText.length() == 1)
					possible_caps.push_back({cont, line});
		}

		for (auto& possible_cap : possible_caps)
		{
			auto& drop_cap_cont = possible_cap.first;
			auto& drop_cap_line = possible_cap.second;

			size_t num_of_lines = 0;

			for(auto& line : m_arTextLines)
			{
				if (!line || line == drop_cap_line)
					continue;

				// буквица должна быть левее
				if (line->m_dLeft < drop_cap_cont->m_dLeft)
					continue;

				// если совпадает строка по высоте - берем ее и выходим
				if (fabs(line->m_dBaselinePos - drop_cap_cont->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
				{
					num_of_lines++;
					break;
				}

				if (line->m_dBaselinePos > drop_cap_cont->m_dBaselinePos)
					break;

				if (fabs(line->m_dTop - drop_cap_cont->m_dTop) > c_dTHE_SAME_STRING_Y_PRECISION_MM && line->m_dTop < drop_cap_cont->m_dTop)
					continue;

				num_of_lines++;
			}
			if (num_of_lines > 1)
			{
				auto drop_cap = std::make_shared<CDropCap>();
				*static_cast<CBaseItem*>(drop_cap.get()) = *drop_cap_cont;
				drop_cap->nLines = num_of_lines;
				drop_cap->wsFont = drop_cap_cont->m_pFontStyle->wsFontName;
				drop_cap->wsText = drop_cap_cont->m_oText.ToStdWString();

				drop_cap->nFontSize = static_cast<LONG>(drop_cap_cont->m_pFontStyle->dFontSize * 2);
				drop_caps.push_back(std::move(drop_cap));

				drop_cap_cont = nullptr;
				if (drop_cap_line->IsCanBeDeleted())
					drop_cap_line = nullptr;

				if (drop_cap_line)
					drop_cap_line->RecalcSizes();
			}
		}

		// шейпы из буквиц
		for (auto&& drop_cap : drop_caps)
		{
			auto shape = std::make_shared<CShape>();
			shape->m_eType = CShape::eShapeType::stTextBox;

			// перемерим на подобранном шрифте
			NSStructures::CFont oFont;
			oFont.Name = drop_cap->wsFont;
			oFont.Size = static_cast<double>(drop_cap->nFontSize) / 2.0;
			m_pFontManager->LoadFontByName(oFont);

			auto metrics = m_pFontManager->GetFontMetrics();
			auto h = m_pFontManager->GetFontHeight();

			shape->m_dTop = drop_cap->m_dTop;
			shape->m_dBaselinePos = drop_cap->m_dTop + h;
			shape->m_dHeight = shape->m_dBaselinePos - shape->m_dTop;

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
						if (!pCurrCont)
							break;

						// берем символ во второй линии
						auto& pNextCont = pNextLine->m_arConts[uNextContIndex];
						if (!pNextCont)
							continue;

						eVerticalCrossingType eVType = pCurrCont->GetVerticalCrossingType(pNextCont.get());
						eHorizontalCrossingType eHType = pCurrCont->GetHorizontalCrossingType(pNextCont.get());

						bool is_font_effect = CContText::CheckFontEffects(pCurrCont, pNextCont, eVType, eHType);
						if(!is_font_effect && CContText::CheckVertAlignTypeBetweenConts(pCurrCont, pNextCont, eVType, eHType))
						{
							pCurrLine->SetVertAlignType(pCurrCont->m_eVertAlignType);
							pNextLine->SetVertAlignType(pNextCont->m_eVertAlignType);
							if ((pCurrLine->m_eVertAlignType == eVertAlignType::vatSuperscript &&
								 pNextLine->m_eVertAlignType == eVertAlignType::vatBase) ||
									(pCurrLine->m_eVertAlignType == eVertAlignType::vatBase &&
									 pNextLine->m_eVertAlignType == eVertAlignType::vatSubscript))
							{
								pCurrLine->m_pLine = pNextLine;
								pNextLine->m_pLine = pCurrLine;
							}
						}
						else if(!is_font_effect && pCurrCont->IsDuplicate(pNextCont.get(), eVType))
						{
							pNextCont = nullptr;
							pCurrCont->m_iNumDuplicates++;
						}
					}
					if(pNextLine && pNextLine->IsCanBeDeleted())
						pNextLine = nullptr;
				}
			}
			if(pCurrLine && pCurrLine->IsCanBeDeleted())
				pCurrLine = nullptr;
		}
	}

	void CPage::DetermineStrikeoutsUnderlinesHighlights()
	{
		// определение различных эффектов на основании взаимного расположения символов и шейпов
		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& shape = m_arShapes[i];
			if (!shape || shape->m_eGraphicsType == eGraphicsType::gtNoGraphics)
				continue;

			bool shape_used = false;

			for (size_t j = 0; j < m_arTextLines.size(); ++j)
			{
				auto& pCurrLine = m_arTextLines[j];
				if (!pCurrLine || (pCurrLine->AreObjectsNoCrossingByVertically(shape.get()) &&
					(pCurrLine->m_dTop > shape->m_dBaselinePos ||
					pCurrLine->m_dBaselinePos + pCurrLine->m_dHeight < shape->m_dTop)))
				{
					continue;
				}

				for (size_t k = 0; k < pCurrLine->m_arConts.size(); ++k)
				{
					if(!shape)
						break;

					auto& curr_cont = pCurrLine->m_arConts[k];
					if (!curr_cont)
						continue;

					eVerticalCrossingType eVType = curr_cont->CBaseItem::GetVerticalCrossingType(shape.get());
					eHorizontalCrossingType eHType = curr_cont->GetHorizontalCrossingType(shape.get());

					bool bIsNotComplicatedFigure = shape->m_eGraphicsType != eGraphicsType::gtComplicatedFigure;
					bool bIsLineCrossingText = IsLineCrossingText(shape.get(), curr_cont.get(), eHType);
					bool bIsItHighlightingBackground = IsItHighlightingBackground(shape.get(), curr_cont.get(), eHType);
					bool bIsLineBelowText = IsLineBelowText(shape.get(), curr_cont.get(), eHType);

					if (bIsLineCrossingText)
					{
						curr_cont->m_bIsStrikeoutPresent = true;
						if (shape->m_eLineType == eLineType::ltDouble)
							curr_cont->m_bIsDoubleStrikeout = true;
					}

					else if (bIsItHighlightingBackground)
					{
						//Удовлетворяет расположением и размером - привязываем указатель на картинку
						curr_cont->m_pShape = shape;
						curr_cont->m_bIsHighlightPresent = true;
						curr_cont->m_lHighlightColor = shape->m_oBrush.Color1;
					}

					else if (bIsLineBelowText)
					{
						curr_cont->m_bIsUnderlinePresent = true;
						curr_cont->m_eUnderlineType  = shape->m_eLineType;
						curr_cont->m_lUnderlineColor = shape->m_dHeight > 0.3 ? shape->m_oBrush.Color1 : shape->m_oPen.Color;
					}

					// проверили - удаляем
					if (bIsNotComplicatedFigure && (bIsLineCrossingText || bIsLineBelowText || bIsItHighlightingBackground))
						shape_used = true;

					if (!bIsNotComplicatedFigure)
					{
						bool bIf1 = curr_cont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;
						bool bIf2 = curr_cont->m_bIsShadowPresent && curr_cont->m_bIsOutlinePresent;
						bool bIf3 = eVType == eVerticalCrossingType::vctCurrentOutsideNext;
						bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentOutsideNext;
						bool bIf5 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext;

						if ((bIf1 || bIf2) && bIf3 && (bIf4 || bIf5))
						{
							if (!bIf2)
							{
								auto oBrush = curr_cont->m_pFontStyle->oBrush;
								oBrush.Color1 = shape->m_oPen.Color;

								curr_cont->m_pFontStyle = m_pFontStyleManager->GetOrAddFontStyle(oBrush,
									curr_cont->m_pFontStyle->wsFontName,
									curr_cont->m_pFontStyle->dFontSize,
									curr_cont->m_pFontStyle->bItalic,
									curr_cont->m_pFontStyle->bBold);

								curr_cont->m_bIsShadowPresent = true;
								curr_cont->m_bIsOutlinePresent = true;
							}
							shape_used = true;
						}
					}
				}
			}
			if(shape_used)
				shape = nullptr;
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
		bool bIf1 = (pShape->m_eGraphicsType == eGraphicsType::gtRectangle ||
					 pShape->m_eGraphicsType == eGraphicsType::gtCurve) &&
				pShape->m_eLineType != eLineType::ltUnknown;

		//Условие по вертикали
		double max_diff = std::min(c_dGRAPHICS_ERROR_MM * 3, pCont->m_dHeight * 0.5);
		bool bIf2 = fabs(pShape->m_dBaselinePos - pCont->m_dBaselinePos) < max_diff;

		//Условие пересечения по горизонтали
		bool bIf3 = eHType != eHorizontalCrossingType::hctUnknown &&
				eHType != eHorizontalCrossingType::hctCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
				eHType != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		//Условие для размеров по высоте
		bool bIf4 = pShape->m_dHeight < pCont->m_dHeight * 0.5 &&
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

			if (line->m_eVertAlignType == eVertAlignType::vatSuperscript
					|| line->m_eVertAlignType == eVertAlignType::vatSubscript)
			{
				std::shared_ptr<CTextLine>& base_line = line->m_pLine;
				if (base_line)
				{
					for (auto& pCont : line->m_arConts)
					{
						if (!pCont)
							continue;
						if (base_line->m_dLeft > pCont->m_dLeft)
							base_line->m_dLeft = pCont->m_dLeft;
						if (base_line->m_dRight < pCont->m_dRight)
							base_line->m_dRight = pCont->m_dRight;

						base_line->m_dWidth = base_line->m_dRight - base_line->m_dLeft;
						base_line->m_arConts.push_back(pCont);
						pCont = nullptr;
					}
					line = nullptr;
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

		for (const auto& image : m_arImages)
			if (image)
				image->ToXml(oWriter);


		for (const auto& shape : m_arShapes)
			if (shape)
				shape->ToXml(oWriter);

		if (bIsNeedWP)
		{
			oWriter.WriteString(L"</w:p>");
		}

		for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
		{
			auto& pObj = m_arOutputObjects[i];
			CParagraph* pParagraph = nullptr;
			if((pParagraph = dynamic_cast<CParagraph*>(pObj.get())) != nullptr)
				pParagraph->ToXml(oWriter);
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
		auto no_crossing = [] (const eHorizontalCrossingType& h_type, const eVerticalCrossingType& v_type) {
			return h_type == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
				h_type == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext ||
				v_type == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
				v_type == eVerticalCrossingType::vctNoCrossingCurrentBelowNext;
		};

		// линии из которых сделаем шейпы
		for (size_t index = 0; index < m_arTextLines.size(); ++index)
		{
			auto& curr_line = m_arTextLines[index];
			if (!curr_line)
				continue;

			// если у текущей линии есть дубликаты, то создаем из них шейпы
			if (curr_line->m_iNumDuplicates > 0)
			{
				size_t duplicates = curr_line->m_iNumDuplicates;
				m_arShapes.push_back(CreateSingleLineShape(curr_line));
				while (duplicates > 0)
				{
					m_arShapes.push_back(CreateSingleLineShape(curr_line));
					duplicates--;
				}
				continue;
			}

			// если линия пересекается с предыдущей линией
			if (index && m_arTextLines[index - 1])
			{
				auto& prev_line = m_arTextLines[index - 1];

				if (!prev_line)
					continue;

				auto h_type = curr_line->GetHorizontalCrossingType(prev_line.get());
				auto v_type = curr_line->GetVerticalCrossingType(prev_line.get());

				if (!no_crossing(h_type, v_type))
				{
					m_arShapes.push_back(CreateSingleLineShape(prev_line));
					m_arShapes.push_back(CreateSingleLineShape(curr_line));
					prev_line = nullptr;
					curr_line = nullptr;
					continue;
				}
			}
		}


		if (m_arTextLines.empty())
			return;

		// переместим nullptr в конец и удалим
		auto right = MoveNullptr(m_arTextLines.begin(), m_arTextLines.end());
		m_arTextLines.erase(right, m_arTextLines.end());

		using line_ptr_t = std::shared_ptr<CTextLine>;
		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const line_ptr_t& a, const line_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		if (m_arTextLines.empty())
			return;

		auto build = [this] (const std::vector<std::shared_ptr<CTextLine>>& text_lines) {
			std::vector<std::shared_ptr<CParagraph>> ar_paragraphs;

			double avg_spacing{0.0};
			size_t avg_spacing_n{0};

			double min_left{m_dWidth};
			double max_right{0.0};

			// совпадает ли left, right, center со строкой ниже
			struct Position {
				bool left{false};
				bool center{false};
				bool right{false};
			};

			// параграф будет набиваться строчками
			auto paragraph = std::make_shared<CParagraph>();

			// lamda to setup and add paragpraph
			auto add_paragraph = [this, &max_right, &min_left, &ar_paragraphs] (std::shared_ptr<CParagraph>& paragraph) {

				paragraph->m_dBaselinePos = paragraph->m_arLines.back()->m_dBaselinePos;
				paragraph->m_dTop = paragraph->m_arLines.front()->m_dTop;
				paragraph->m_dRight = max_right + c_dERROR_OF_PARAGRAPH_BORDERS_MM;
				paragraph->m_dLeft = min_left;

				paragraph->m_dWidth = paragraph->m_dRight - paragraph->m_dLeft;
				paragraph->m_dHeight = paragraph->m_dBaselinePos - paragraph->m_dTop;

				paragraph->m_dRightBorder = m_dWidth - max_right;
				paragraph->m_dLeftBorder = min_left;

				paragraph->m_dLineHeight = paragraph->m_dHeight / paragraph->m_arLines.size();
				paragraph->m_bIsNeedFirstLineIndent = false;
				paragraph->m_dFirstLine = 0;
				paragraph->m_wsStyleId = m_pParagraphStyleManager->GetDefaultParagraphStyleId(*paragraph);

				paragraph->MergeLines();

				// setting TextAlignmentType
				if (paragraph->m_arLines.size() > 1)
				{
					Position position_curr;
					position_curr.left   = true;
					position_curr.center = true;
					position_curr.right  = true;

					bool first_left = false;

					for (size_t index = 1; index < paragraph->m_arLines.size(); ++index)
					{
						auto& curr_line = paragraph->m_arLines[index];
						auto& prev_line = paragraph->m_arLines[index - 1];

						// indent check
						if (index == 1)
						{
							first_left = fabs(curr_line->m_dLeft - prev_line->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;

							// первая строчка левее правой
							if (!first_left && prev_line->m_dLeft < curr_line->m_dLeft)
								position_curr.left = false;
						}
						else
							position_curr.left &= fabs(curr_line->m_dLeft - prev_line->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;

						position_curr.right &= fabs(curr_line->m_dRight - prev_line->m_dRight) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;

						auto center_curr = curr_line->m_dLeft + curr_line->m_dWidth / 2;
						auto center_prev = prev_line->m_dLeft + prev_line->m_dWidth / 2;

						position_curr.center &= fabs(center_curr - center_prev) < c_dCENTER_POSITION_ERROR_MM;
					}
					if (position_curr.left && position_curr.right && first_left)
						paragraph->m_eTextAlignmentType = CParagraph::tatByWidth;
					else if (position_curr.left)
						paragraph->m_eTextAlignmentType = CParagraph::tatByLeft;
					else if (position_curr.right)
						paragraph->m_eTextAlignmentType = CParagraph::tatByRight;
					else if (position_curr.center)
						paragraph->m_eTextAlignmentType = CParagraph::tatByCenter;

					// indent check
					if (paragraph->m_eTextAlignmentType == CParagraph::tatByLeft && !first_left)
					{
						paragraph->m_bIsNeedFirstLineIndent = true;
						paragraph->m_dFirstLine = paragraph->m_arLines[0]->m_dLeft - paragraph->m_dLeft;
					}
				}
				if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
						m_eTextAssociationType == TextAssociationType::tatPlainLine)
				{
					if (ar_paragraphs.empty())
						paragraph->m_dSpaceBefore = paragraph->m_dTop + c_dCORRECTION_FOR_FIRST_PARAGRAPH;
					else
						paragraph->m_dSpaceBefore = paragraph->m_dTop - ar_paragraphs.back()->m_dBaselinePos;
				}

				ar_paragraphs.push_back(std::move(paragraph));
				paragraph = std::make_shared<CParagraph>();

				min_left = m_dWidth;
				max_right = 0.0;
			};

			// lamda to add line and setup min_left/max_right
			auto add_line = [&min_left, &max_right] (std::shared_ptr<CParagraph>& paragraph, std::shared_ptr<CTextLine>& curr_line) {
				min_left = std::min(min_left, curr_line->m_dLeft);
				max_right = std::max(max_right, curr_line->m_dRight);
				paragraph->m_arLines.push_back(curr_line);
			};

			// 1 строчка в параграфе
			if (m_eTextAssociationType == TextAssociationType::tatPlainLine ||
					m_eTextAssociationType == TextAssociationType::tatShapeLine)
			{
				for (auto& curr_line : m_arTextLines)
				{
					add_line(paragraph, curr_line);
					add_paragraph(paragraph);
				}
			}

			else if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
					m_eTextAssociationType == TextAssociationType::tatParagraphToShape)
			{
				// ar_spacing[index]- расстояние строки до строки снизу
				// если 0.0 - строка последняя
				std::vector<double> ar_spacings(m_arTextLines.size(), 0.0);

				// позиции относительно других линий
				std::vector<Position> ar_positions(m_arTextLines.size());

				// требуется ли отступ
				std::vector<bool> ar_indents(m_arTextLines.size(), false);

				// если ar_delims[index] == true, после строчки index нужно начинать новый параграф
				std::vector<bool> ar_delims(m_arTextLines.size(), false);

				// calcs spacings & positions
				for (size_t index = 0; index < m_arTextLines.size() - 1; ++index)
				{
					ar_spacings[index] = m_arTextLines[index + 1]->m_dBaselinePos - m_arTextLines[index]->m_dTop;
					avg_spacing = (avg_spacing / (avg_spacing_n + 1)) * avg_spacing_n + (ar_spacings[index] / (avg_spacing_n + 1));

					auto& left_curr = m_arTextLines[index]->m_dLeft;
					auto& left_next = m_arTextLines[index + 1]->m_dLeft;

					auto& right_curr = m_arTextLines[index]->m_dRight;
					auto& right_next = m_arTextLines[index + 1]->m_dRight;

					auto center_curr = (m_arTextLines[index]->m_dLeft + m_arTextLines[index]->m_dWidth / 2);
					auto center_next = (m_arTextLines[index + 1]->m_dLeft + m_arTextLines[index + 1]->m_dWidth / 2);

					if (fabs(center_curr - center_next) < c_dCENTER_POSITION_ERROR_MM)
						ar_positions[index].center = true;
					if (fabs(left_curr - left_next) < c_dERROR_OF_PARAGRAPH_BORDERS_MM)
						ar_positions[index].left = true;
					if (fabs(right_curr - right_next) < c_dERROR_OF_PARAGRAPH_BORDERS_MM)
						ar_positions[index].right = true;
				}

				// spacing check
				for (size_t index = 0; index < ar_spacings.size(); ++index)
				{
					double spacing_top = 0.0;
					double spacing_bot = 0.0;

					if (index != 0) spacing_top = ar_spacings[index - 1];
					spacing_bot = ar_spacings[index];

					if (spacing_top == 0.0) spacing_top = spacing_bot;
					if (spacing_bot == 0.0) spacing_bot = spacing_top;

					if (spacing_bot > c_dLINE_DISTANCE_MAX_MM)
						ar_delims[index] = true;
					else if (fabs(spacing_top - spacing_bot) < c_dLINE_DISTANCE_ERROR_MM)
						ar_delims[index] = false;
					else
					{
						// берем доп строчки сверху и снизу для анализа
						bool same_double_top = false;
						bool same_double_bot = false;

						if (index > 1)
						{
							double spacing_top_next = ar_spacings[index - 2];
							if (fabs(spacing_top - spacing_top_next) < c_dLINE_DISTANCE_ERROR_MM)
								same_double_top = true;
						}
						if (index < ar_spacings.size() - 1)
						{
							double spacing_bot_next = ar_spacings[index + 1];
							if (fabs(spacing_bot - spacing_bot_next) < c_dLINE_DISTANCE_ERROR_MM)
								same_double_bot = true;
						}

						// если анализ доп строчек ничего не дал - разбиваем наиболее "вероятным" способом
						if ((same_double_top == same_double_bot))
						{
							if (spacing_top > spacing_bot)
								ar_delims[index - 1] = true;
							else if (spacing_top < spacing_bot)
								ar_delims[index] = true;
						}
						// прикрепляем строчку к верху или низу
						else
						{
							if (same_double_top)
								ar_delims[index] = true;
							else if (same_double_bot)
								ar_delims[index - 1] = true;
						}
					}
				}

				// alignment check
				bool is_first_line = false;
				for (size_t index = 0; index < ar_positions.size() - 1; ++index)
				{
					Position& position = ar_positions[index];

					auto& line_top = m_arTextLines[index];
					auto& line_bot = m_arTextLines[index + 1];

					if (index == 0 || ar_delims[index - 1])
						is_first_line = true;
					else
						is_first_line = false;

					// первая строка может быть с отступом
					if (is_first_line && line_bot->m_dLeft < line_top->m_dLeft)
					{
						// если больше трех линий - проверим третью
						if (index < ar_positions.size() - 2)
						{
							if (!ar_delims[index] && !ar_delims[index + 1] && ar_positions[index + 1].left)
								ar_indents[index] = true;
							else if (!ar_delims[index] && ar_delims[index + 1])
								ar_indents[index] = true;
						}
						else
							ar_indents[index] = true;
					}

					bool is_unknown = !((position.left || ar_indents[index]) || position.right || position.center);
					if (is_unknown)
						ar_delims[index] = true;
				}

				// gap check
				//
				// bla-bla-bla
				// text bla-bla-bla-bla
				//
				// bla-bla-bla text
				// bla-bla-bla-bla

				double curr_max_right = m_arTextLines[0]->m_dRight;
				double curr_min_left = m_arTextLines[0]->m_dLeft;
				for (size_t index = 0; index < ar_positions.size() - 1; ++index)
				{
					Position position = ar_positions[index];
					auto& line_top = m_arTextLines[index];
					auto& line_bot = m_arTextLines[index + 1];

					if (ar_delims[index])
					{
						curr_max_right = line_bot->m_dRight;
						curr_min_left = line_bot->m_dLeft;
						continue;
					}

					std::shared_ptr<CContText> cont = line_bot->m_arConts[0];
					double line_with_first_right = line_top->m_dRight + cont->m_dFirstWordWidth;
					double line_with_first_left = line_top->m_dLeft - cont->m_dFirstWordWidth;

					curr_max_right = std::max(curr_max_right, line_bot->m_dRight);
					curr_min_left = std::min(curr_min_left, line_bot->m_dLeft);

					double diff = 0;

					if (position.right)
						diff = line_with_first_left - curr_min_left;
					else if (position.left || ar_indents[index])
						diff = curr_max_right - line_with_first_right;
					else if (position.center)
						continue;

					if (diff <= 0)
					{
//						if (diff > -c_dERROR_GAP)
//						{
//							auto& last_cont = line_top->m_arConts[line_top->m_arConts.size() - 1];
//							last_cont->m_bIsAddBrEnd = true;
//						}
//						else
							continue;
					}
					else
					{
						ar_delims[index] = true;
						curr_max_right = line_bot->m_dRight;
						curr_min_left = line_bot->m_dLeft;
					}
				}

				// на основе ar_delims разбиваем на параграфы
				for (size_t index = 0; index < ar_delims.size(); ++index)
				{
					add_line(paragraph, m_arTextLines[index]);
					if (ar_delims[index] || index == ar_delims.size() - 1)
						add_paragraph(paragraph);
				}
			}

			return ar_paragraphs;
		};

		std::vector<std::shared_ptr<CParagraph>> ar_paragraphs;

		if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
				m_eTextAssociationType == TextAssociationType::tatPlainLine)
		{
			ar_paragraphs = build(m_arTextLines);
		}

		else if (m_eTextAssociationType == TextAssociationType::tatParagraphToShape ||
				m_eTextAssociationType == TextAssociationType::tatShapeLine)
		{
			ar_paragraphs = build(m_arTextLines);
		}

		using paragraph_ptr_t = std::shared_ptr<CParagraph>;
		std::sort(ar_paragraphs.begin(), ar_paragraphs.end(), [] (const paragraph_ptr_t& a, const paragraph_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
				m_eTextAssociationType == TextAssociationType::tatPlainLine)
		{
			for(auto&& p : ar_paragraphs)
				m_arOutputObjects.push_back(std::move(p));
		}

		else if (m_eTextAssociationType == TextAssociationType::tatParagraphToShape ||
				m_eTextAssociationType == TextAssociationType::tatShapeLine)
		{
			for(auto&& p : ar_paragraphs)
				m_arShapes.push_back(CreateSingleParagraphShape(p));
		}
	}

	std::shared_ptr<CShape> CPage::CreateSingleLineShape(std::shared_ptr<CTextLine>& pLine)
	{
		auto pParagraph = std::make_shared<CParagraph>();

		pParagraph->m_arLines.push_back(pLine);
		pParagraph->m_dLeft = pLine->m_dLeft;
		pParagraph->m_dTop = pLine->m_dTop;
		pParagraph->m_dBaselinePos = pLine->m_dBaselinePos;
		pParagraph->m_dWidth = pLine->m_dWidth + c_dERROR_OF_PARAGRAPH_BORDERS_MM;
		pParagraph->m_dHeight = pLine->m_dHeight;
		pParagraph->m_dRight = pLine->m_dRight;

		if (pLine->m_pDominantShape)
		{
			pParagraph->m_bIsShadingPresent = true;
			pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
			pParagraph->RemoveHighlightColor();
		}

		auto pShape = std::make_shared<CShape>();
		pShape->m_arOutputObjects.push_back(pParagraph);
		pShape->m_eType = CShape::eShapeType::stTextBox;

		pShape->m_dLeft = pParagraph->m_dLeft;
		pShape->m_dTop = pParagraph->m_dTop;
		pShape->m_dBaselinePos = pParagraph->m_dBaselinePos;
		pShape->m_dWidth = pParagraph->m_dWidth;
		pShape->m_dHeight = pParagraph->m_dHeight;
		pShape->m_dRight = pParagraph->m_dRight;
		pShape->m_bIsBehindDoc = false;

		return pShape;
	}

	std::shared_ptr<CShape> CPage::CreateSingleParagraphShape(std::shared_ptr<CParagraph>& pParagraph)
	{
		std::shared_ptr<CShape> pShape;

		pShape = std::make_shared<CShape>();

		pShape->m_dLeft = pParagraph->m_dLeft;
		pShape->m_dTop = pParagraph->m_dTop;
		pShape->m_dRight =  pParagraph->m_dRight;
		pShape->m_dBaselinePos = pParagraph->m_dBaselinePos;
		pShape->m_dHeight = pParagraph->m_dHeight;
		pShape->m_dWidth = pParagraph->m_dWidth;

		pParagraph->m_dLeftBorder = 0;
		pParagraph->m_dRightBorder = 0;

		// first correction fix
		if (pParagraph->m_dSpaceBefore > 0) pParagraph->m_dSpaceBefore = 0;

		pParagraph->m_dSpaceAfter = 0;

		pShape->m_arOutputObjects.push_back(pParagraph);
		pShape->m_eType = CShape::eShapeType::stTextBox;
		pShape->m_bIsBehindDoc = false;

		return pShape;
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
