#include "Page.h"

#include <memory>
#include <map>

#include "../../../DesktopEditor/graphics/GraphicsPath.h"
#include "../../../DesktopEditor/graphics/pro/Graphics.h"

#include "../resources/Constants.h"
#include "../resources/utils.h"

namespace NSDocxRenderer
{
	CPage::CPage(NSFonts::IApplicationFonts* pAppFonts, const CManagers& oManagers) :
		m_oManagers(oManagers), m_oContBuilder(oManagers.pFontStyleManager, oManagers.pFontSelector)
	{
		m_pAppFonts = pAppFonts;
		CShape::ResetRelativeHeight();
	}

	void CPage::BeginCommand(DWORD lType)
	{
		m_lCurrentCommand = static_cast<LONG>(lType);
	}
	void CPage::EndCommand(DWORD lType)
	{
		m_lCurrentCommand = -1;

		if (lType == c_nResetClipType)
		{
			m_oClipVectorGraphics.Clear();
		}
		else if (lType == c_nClipType)
		{
			//closing clip path if non-closed
			if (!m_oCurrVectorGraphics.IsEmpty() && m_oCurrVectorGraphics.GetData().back().type != CVectorGraphics::ePathCommandType::pctClose)
				m_oCurrVectorGraphics.Add({CVectorGraphics::ePathCommandType::pctClose, {}});

			if (!m_oClipVectorGraphics.IsEmpty())
			{
				m_oClipVectorGraphics = CVectorGraphics::CalcBoolean(m_oClipVectorGraphics, m_oCurrVectorGraphics, m_lClipMode);
				m_oCurrVectorGraphics.Clear();
			}
			else
				m_oClipVectorGraphics = std::move(m_oCurrVectorGraphics);
		}

		if (!m_arLuminosityShapes.empty() && !m_arOneColorGradientShape.empty())
			FillLuminosityShapes();
	}

	void CPage::Clear()
	{
		m_oPen.SetDefaultParams();
		m_oBrush.SetDefaultParams();
		m_oFont.SetDefaultParams();
		m_oShadow.SetDefaultParams();
		m_oEdgeText.SetDefaultParams();
		m_oTransform.Reset();

		m_oHorVerLinesCollector.Clear();
		m_arConts.clear();
		m_arTextLines.clear();
		m_arDiacriticalSymbols.clear();
		m_arShapes.clear();
		m_arParagraphs.clear();
		m_arTables.clear();
		m_arOutputObjects.clear();
		m_oCurrVectorGraphics.Clear();
		m_oClipVectorGraphics.Clear();
		m_arCompleteObjectsXml.clear();
		m_arLuminosityShapes.clear();
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
		double rotation = m_oTransform.z_Rotation();

		Point p1(fX, fY);
		Point p2(fX + fWidth, fY + fHeight);

		m_oTransform.TransformPoint(p1.x, p1.y);
		m_oTransform.TransformPoint(p2.x, p2.y);

		Point c((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
		Aggplus::CMatrix rotate_matrix;
		rotate_matrix.RotateAt(-rotation, c.x, c.y, Aggplus::MatrixOrderAppend);

		rotate_matrix.TransformPoint(p1.x, p1.y);
		rotate_matrix.TransformPoint(p2.x, p2.y);

		Point p3(p1.x, p2.y);
		Point p4(p2.x, p1.y);

		rotate_matrix.RotateAt(2 * rotation, c.x, c.y, Aggplus::MatrixOrderAppend);
		rotate_matrix.TransformPoint(p1.x, p1.y);
		rotate_matrix.TransformPoint(p2.x, p2.y);
		rotate_matrix.TransformPoint(p3.x, p3.y);
		rotate_matrix.TransformPoint(p4.x, p4.y);

		m_oCurrVectorGraphics.Clear();
		m_oCurrVectorGraphics.MoveTo(p1.x, p1.y);
		m_oCurrVectorGraphics.LineTo(p3.x, p3.y);
		m_oCurrVectorGraphics.LineTo(p2.x, p2.y);
		m_oCurrVectorGraphics.LineTo(p4.x, p4.y);
		m_oCurrVectorGraphics.LineTo(p1.x, p1.y);
		m_oCurrVectorGraphics.Close();
		DrawPath(c_nWindingFillMode, pInfo);
	}

	// path commands
	void CPage::PathMoveTo(double& dX, double& dY)
	{
		m_oTransform.TransformPoint(dX, dY);
		m_oCurrVectorGraphics.MoveTo(dX, dY);
	}

	void CPage::PathLineTo(double& dX, double& dY)
	{
		m_oTransform.TransformPoint(dX, dY);
		m_oCurrVectorGraphics.LineTo(dX, dY);
	}

	void CPage::PathCurveTo(double& dX1, double& dY1, double& dX2, double& dY2, double& dX3, double& dY3)
	{
		m_oTransform.TransformPoint(dX1, dY1);
		m_oTransform.TransformPoint(dX2, dY2);
		m_oTransform.TransformPoint(dX3, dY3);
		m_oCurrVectorGraphics.CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);
	}

	void CPage::PathStart()
	{
		m_oCurrVectorGraphics.Clear();
	}

	void CPage::PathEnd()
	{
		m_oCurrVectorGraphics.Clear();
	}

	void CPage::PathClose()
	{
		m_oCurrVectorGraphics.Close();
	}

	// c_nStroke = 0x0001;
	// c_nWindingFillMode = 0x0100;
	// c_nEvenOddFillMode = 0x0200;
	void CPage::DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo)
	{
		// in case if text comes as a path with unicode 32 (space)
		if (m_oCurrVectorGraphics.IsEmpty())
			return;

		double rotation = m_oTransform.z_Rotation();
		double left = m_oCurrVectorGraphics.GetLeft();
		double right = m_oCurrVectorGraphics.GetRight();
		double top = m_oCurrVectorGraphics.GetTop();
		double bot = m_oCurrVectorGraphics.GetBottom();
		double transform_det = sqrt(fabs(m_oTransform.Determinant()));

		// save default image vector before clip to calc blipFill
		auto image_vector = m_oCurrVectorGraphics;

		auto set_fill_mode = [this, lType, &transform_det] (shape_ptr_t s) {
			if (lType & c_nStroke)
			{
				s->m_bIsNoStroke = false;
				s->m_oPen = m_oPen;
				s->m_oPen.Size *= transform_det;
			}
			if (lType & c_nWindingFillMode || lType & c_nEvenOddFillMode)
			{
				s->m_bIsNoFill = false;
				s->m_oBrush = m_oBrush;
			}
		};

		if (!m_arShapes.empty())
		{
			auto& last_shape = m_arShapes.back();
			if (last_shape->IsEqual(top, bot, left, right) && rotation == last_shape->m_dRotation)
			{
				set_fill_mode(last_shape);
				return;
			}
		}

		auto shape = std::make_shared<CShape>();
		shape->m_oPen.Size *= transform_det;
		set_fill_mode(shape);

		if (shape->m_bIsNoStroke)
		{
			if ((fabs(left - right) < 0.3) || (fabs(top - bot) < 0.3))
			{
				shape->m_oPen.Color = m_oBrush.Color1;
				shape->m_oPen.Alpha = m_oBrush.Alpha1;
			}
		}

		if (!m_oClipVectorGraphics.IsEmpty())
		{
			CVectorGraphics new_vector_graphics = CVectorGraphics::CalcBoolean(
				m_oCurrVectorGraphics,
				m_oClipVectorGraphics,
				m_lClipMode,
				lType);

			if (new_vector_graphics.IsEmpty())
			{
				m_oCurrVectorGraphics.Clear();
				return;
			}
			m_oCurrVectorGraphics = std::move(new_vector_graphics);
		}
		shape->SetVector(std::move(m_oCurrVectorGraphics));

		bool skip_shape = false;
		auto info = pInfo;
		if (!info && m_bIsGradient)
		{
			if (m_oBrush.m_oGradientInfo.checkLuminosity())
				m_arLuminosityShapes.push_back(shape);
			else if (m_oBrush.m_oGradientInfo.isOneColor())
			{
				m_arOneColorGradientShape.push_back(shape);
				skip_shape = true;
			}
			else
				DrawGradient(shape);

			m_bIsGradient = false;
		}
		shape->m_dRotation = rotation;

		if (info)
			DrawImage(shape, info, image_vector);
		else
			shape->m_eType = CShape::eShapeType::stVectorGraphics;


		if (!shape->IsOoxmlValid())
			return;

		// big white shape with page width & height skip
		if (fabs(shape->m_dHeight - m_dHeight) <= c_dSHAPE_X_OFFSET * 2 &&
			fabs(shape->m_dWidth - m_dWidth) <= c_dSHAPE_X_OFFSET * 2 &&
			shape->m_oBrush.Color1 == c_iWhiteColor)
			return;

		if (!skip_shape)
		{
			shape->m_nOrder = ++m_nShapeOrder;
			m_arShapes.push_back(shape);
		}
	}

	void CPage::AddText(
		const PUINT pUnicodes,
		const PUINT pGids,
		const UINT& nCount,
		const double& fX,
		const double& fY,
		const double& fWidth,
		const double& fHeight,
		const double& fBaseLineOffset)
	{
		// 9 - \t
		if (*pUnicodes == 9)
			return;

		double dTextX = fX;
		double dTextY = fY;
		double dTextR = fX + fWidth;
		double dTextB = fY + fHeight;

		m_oTransform.TransformPoint(dTextX, dTextY);
		m_oTransform.TransformPoint(dTextR, dTextB);

		NSStringUtils::CStringUTF32 oText((uint32_t*)pUnicodes, nCount);

		if ((pUnicodes != nullptr) && (pGids != nullptr))
			for (unsigned int i = 0; i < nCount; ++i)
				if (!CContText::IsUnicodeSymbol(pUnicodes[i]))
					oText[i] = ' ';

		// иногда приходит неверный? размер, нужно перемерить (XPS)
		if (m_bIsRecalcFontSize)
		{
			m_oFont.Size *= ((m_oTransform.sx() + m_oTransform.sy()) / 2);
			m_bIsRecalcFontSize = false;
		}
		if (!m_oManagers.pFontManager->LoadFontByFile(m_oFont))
			return;

		double _x = 0;
		double _y = 0;
		double _w = 0;
		double _h = 0;

		if (nullptr != pGids)
		{
			m_oManagers.pFontManager->SetStringGid(1);
			m_oManagers.pFontManager->MeasureStringGids(pGids, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
		}
		else
		{
			// такого быть не должно (только из xps)
			m_oManagers.pFontManager->SetStringGid(0);
			m_oManagers.pFontManager->MeasureStringGids(pUnicodes, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
		}

		_h = m_oManagers.pFontManager->GetFontHeight();

		double baseline = dTextY + fBaseLineOffset;
		double top = baseline - _h;
		double left = dTextX;
		double right = left + _w;

		// use forced fold option
		const auto& oParams = m_oManagers.pFontManager->GetFontSelectParams();
		bool bForcedBold = oParams.bDefaultBold;
		if (m_lCurrentCommand == c_nStrokeTextType && m_oFont.Bold)
			bForcedBold = true;

		m_oManagers.pParagraphStyleManager->UpdateAvgFontSize(m_oFont.Size);
		m_oContBuilder.AddUnicode(top, baseline, left, right, m_oFont, m_oBrush, m_oManagers.pFontManager, oText, bForcedBold, m_bUseDefaultFont, m_bWriteStyleRaw);
	}

	void CPage::Analyze()
	{
		// building objects from symbols
		m_arConts = m_oContBuilder.GetConts();
		m_arDiacriticalSymbols = MoveDiacriticalSymbols();
		m_arTextLines = BuildTextLines();

		// analyzing data. can take shapes and set them to nullptr
		AnalyzeShapes();
		AnalyzeTextLines();

		// building final objects
		m_arParagraphs = BuildParagraphs();

		// if (m_bIsBuildTables)
		// 	m_arTables = BuildTables();

		// post analyze
		CalcSelected();
		MergeShapes();
		CalcShapesRotation();

		m_arOutputObjects = BuildOutputObjects();
		// for (auto& t : m_arTables)
		// 	m_arOutputObjects.push_back(t);
	}

	void CPage::Record(NSStringUtils::CStringBuilder& oWriter, bool bIsLastPage)
	{
		ToXml(oWriter);
		WriteSectionToFile(bIsLastPage, oWriter);
	}

	std::vector<std::wstring> CPage::GetXmlShapes()
	{
		std::vector<std::wstring> xml_shapes;
		for (const auto& shape : m_arShapes)
		{
			if (!shape) continue;
			auto writer = new NSStringUtils::CStringBuilder();
			shape->ToXml(*writer);
			xml_shapes.push_back(writer->GetData());
			delete writer;
		}
		if (!m_arCompleteObjectsXml.empty())
			xml_shapes.insert(xml_shapes.end(), m_arCompleteObjectsXml.begin(), m_arCompleteObjectsXml.end());

		return xml_shapes;
	}
	std::vector<std::wstring> CPage::GetXmlShapesPptx()
	{
		ReorderShapesForPptx();

		std::vector<std::wstring> xml_shapes;
		for (const auto& shape : m_arShapes)
		{
			if (!shape) continue;
			auto writer = new NSStringUtils::CStringBuilder();
			shape->ToXmlPptx(*writer);
			xml_shapes.push_back(writer->GetData());
			delete writer;
		}
		if (!m_arCompleteObjectsXml.empty())
			xml_shapes.insert(xml_shapes.end(), m_arCompleteObjectsXml.begin(), m_arCompleteObjectsXml.end());

		return xml_shapes;
	}
	void CPage::AddCompleteXml(const std::wstring oXml)
	{
		m_arCompleteObjectsXml.push_back(oXml);
	}
	void CPage::ReorderShapesForPptx()
	{
		// переместим nullptr в конец и удалим
		auto right = MoveNullptr(m_arShapes.begin(), m_arShapes.end());
		m_arShapes.erase(right, m_arShapes.end());

		std::sort(m_arShapes.begin(), m_arShapes.end(), [] (const shape_ptr_t& s1, const shape_ptr_t& s2) {
			if (s1->m_bIsBehindDoc && !s2->m_bIsBehindDoc) return true;
			if (!s1->m_bIsBehindDoc && s2->m_bIsBehindDoc) return false;
			return s1->m_nOrder < s2->m_nOrder;
		});
	}

	std::vector<CPage::cont_ptr_t> CPage::MoveDiacriticalSymbols()
	{
		std::vector<cont_ptr_t> diac_symbols;
		for (auto& cont : m_arConts)
			if (cont && cont->IsDiacritical())
				diac_symbols.push_back(std::move(cont));
		return diac_symbols;
	}

	std::vector<CPage::line_ptr_t> CPage::BuildTextLines()
	{
		line_ptr_t curr_line = nullptr;
		std::vector<line_ptr_t> text_lines;

		for (auto& cont : m_arConts)
		{
			if (!cont)
				continue;

			if (curr_line && fabs(curr_line->m_dBaselinePos - cont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
			{
				curr_line->AddCont(cont);
				continue;
			}

			bool skip = false;
			for (size_t i = 0; i < text_lines.size(); ++i)
			{
				if (fabs(text_lines[i]->m_dBaselinePos - cont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
				{
					curr_line = text_lines[i];
					cont->m_bPossibleSplit = true;
					curr_line->AddCont(cont);
					skip = true;
				}
			}

			if (skip)
				continue;

			curr_line = std::make_shared<CTextLine>();
			curr_line->AddCont(cont);
			text_lines.push_back(curr_line);
		}

		for (size_t i = 0; i < text_lines.size(); i++)
		{
			bool only_spaces = true;
			for (auto& cont : text_lines[i]->m_arConts)
			{
				if (!cont->IsOnlySpaces())
				{
					only_spaces = false;
					break;
				}
			}
			if (only_spaces)
				text_lines.erase(text_lines.begin() + i, text_lines.begin() + i + 1);
		}
		return text_lines;
	}

	void CPage::MergeShapes()
	{
		if (m_arShapes.empty())
			return;

		std::sort(m_arShapes.begin(), m_arShapes.end(), [] (const shape_ptr_t& a, const shape_ptr_t& b) {
			if (!a) return false;
			if (!b) return true;
			return a->m_nOrder < b->m_nOrder;
		});

		using shape_ref_ptr_t = std::reference_wrapper<shape_ptr_t>;
		for (size_t i = 0; i < m_arShapes.size() - 1; i++)
		{
			shape_ref_ptr_t val = m_arShapes[i];
			shape_ref_ptr_t next_val = m_arShapes[i + 1];

			if (!val.get() || ! next_val.get())
				continue;

			next_val.get()->TryMergeShape(val.get());
		}
	}
	void CPage::CalcShapesRotation()
	{
		for (auto& shape : m_arShapes)
		{
			if (!shape || fabs(shape->m_dRotation) < c_dMIN_ROTATION) continue;
			shape->CalcNoRotVector();
		}
	}

	std::vector<CPage::item_ptr_t> CPage::BuildOutputObjects()
	{
		std::vector<item_ptr_t> output_objects;

		if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
		    m_eTextAssociationType == TextAssociationType::tatPlainLine)
		{
			CBaseItem* prev_p = nullptr;

			size_t first_index = 0;
			size_t second_index = first_index + 1;

			for (; first_index < m_arParagraphs.size(); first_index = second_index++)
			{
				auto& first_p = m_arParagraphs[first_index];
				if (second_index != m_arParagraphs.size())
				{
					auto& second_p = m_arParagraphs[second_index];
					while (second_index < m_arParagraphs.size() && first_p->m_dBaselinePos > second_p->m_dTop)
						second_index++;
				}

				if (second_index - first_index == 1)
				{
					auto& p = m_arParagraphs[first_index];

					if (!prev_p)
						p->m_dSpaceBefore = p->m_dTop + c_dCORRECTION_FOR_FIRST_PARAGRAPH;
					else
						p->m_dSpaceBefore = p->m_dTop - prev_p->m_dBaselinePos;

					output_objects.push_back(p);
					prev_p = p.get();
				}
				else
				{
					for (size_t j = first_index; j < second_index; ++j)
						m_arShapes.push_back(CreateSingleParagraphShape(m_arParagraphs[j]));
				}
			}
		}

		else if (m_eTextAssociationType == TextAssociationType::tatParagraphToShape ||
		         m_eTextAssociationType == TextAssociationType::tatShapeLine)
		{
			for (auto& p : m_arParagraphs)
				m_arShapes.push_back(CreateSingleParagraphShape(p));
		}

		return output_objects;
	}

	void CPage::CalcSelected()
	{
		for (auto& line : m_arTextLines)
			for (auto& cont : line->m_arConts)
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
		std::sort(m_arShapes.begin(), m_arShapes.end(), [] (const shape_ptr_t& a, const shape_ptr_t& b) {
			return a->m_dLeft < b->m_dLeft;
		});

		AnalyzeLinesType();
	}

	void CPage::AnalyzeLinesType()
	{
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
					if (!m_arShapes[j])
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

	void CPage::GetHorVerLines()
	{
		for (const auto& shape : m_arShapes)
			if (shape)
			{
				const double out_of_page_coeff = 1.1;
				bool is_out_of_page = shape->m_dTop < 0 ||
				                      shape->m_dBaselinePos > this->m_dHeight * out_of_page_coeff ||
				                      shape->m_dLeft < 0 ||
				                      shape->m_dRight > this->m_dWidth * out_of_page_coeff;


				bool is_too_big = (!shape->m_bIsNoFill && (shape->m_dWidth > c_dSHAPE_TROUGH_MAX_MM || shape->m_dHeight > c_dSHAPE_TROUGH_MAX_MM));

				if (is_too_big || is_out_of_page)
					continue;

				m_oHorVerLinesCollector.AddVector(shape->m_oVector);
			}
	}

	void CPage::AnalyzeTextLines()
	{
		// вся логика основана на отсортированных списках объектов
		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const line_ptr_t& a, const line_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		AnalyzeDropCaps();
		AnalyzeConts();
		AnalyzeEffects();

		GetHorVerLines();

		AddDiacriticalSymbols();
		MergeTextLinesByVatType();
		DeleteTextClipPage();
		MergeConts();
		SplitLines();
		AnalyzeOverlapLines();

		// переместим nullptr в конец и удалим
		auto right = MoveNullptr(m_arTextLines.begin(), m_arTextLines.end());
		m_arTextLines.erase(right, m_arTextLines.end());

		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const line_ptr_t& a, const line_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});
	}

	void CPage::AnalyzeDropCaps()
	{
		double avg_font_size = m_oManagers.pParagraphStyleManager->GetAvgFontSize();

		std::vector<std::pair<cont_ptr_t&, line_ptr_t&>> possible_caps;
		std::vector<cont_ptr_t> drop_caps;

		for (size_t i = 0; i < m_arTextLines.size(); i++)
		{
			auto& line = m_arTextLines[i];
			for (auto& cont : line->m_arConts)
				if (cont && cont->m_pFontStyle->dFontSize > 2 * avg_font_size && cont->GetText().length() == 1)
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
				if (fabs(line->m_dBotWithMaxDescent - drop_cap_cont->m_dBotWithDescent) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
				{
					num_of_lines++;
					break;
				}

				if (line->m_dBotWithMaxDescent < drop_cap_cont->m_dTopWithAscent)
					continue;

				if (line->m_dTopWithMaxAscent > drop_cap_cont->m_dBotWithDescent)
					break;

				num_of_lines++;
			}
			if (num_of_lines > 1)
			{
				drop_caps.push_back(std::move(drop_cap_cont));
				if (drop_cap_line->IsCanBeDeleted())
					drop_cap_line = nullptr;

				if (drop_cap_line)
					drop_cap_line->RecalcSizes();
			}
		}

		// шейпы из буквиц
		for (auto&& drop_cap : drop_caps)
		{
			drop_cap->CalcSelected();

			auto line = std::make_shared<CTextLine>();
			line->AddCont(drop_cap);

			auto shape = CreateSingleLineShape(line);
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
						if (!is_font_effect && CContText::CheckVertAlignTypeBetweenConts(pCurrCont, pNextCont, eVType, eHType))
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
						else if (!is_font_effect && pCurrCont->IsDuplicate(pNextCont.get(), eVType, eHType))
						{
							pNextCont = nullptr;
							pCurrCont->m_iNumDuplicates++;
							if (!pCurrCont->m_pFontStyle.get()->bBold)
							{
								CFontStyle font_style = *pCurrCont->m_pFontStyle;
								font_style.bBold = true;
								pCurrCont->m_pFontStyle = m_oManagers.pFontStyleManager->GetOrAddFontStyle(font_style);
							}
						}
					}
					if (pNextLine && pNextLine->IsCanBeDeleted())
						pNextLine = nullptr;
				}
			}
			if (pCurrLine && pCurrLine->IsCanBeDeleted())
				pCurrLine = nullptr;
		}
	}

	void CPage::AnalyzeEffects()
	{
		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& shape = m_arShapes[i];
			if (!shape || shape->m_eGraphicsType == eGraphicsType::gtNoGraphics)
				continue;

			bool shape_used = false;
			for (size_t j = 0; j < m_arTextLines.size(); ++j)
			{
				auto& curr_line = m_arTextLines[j];
				if (!curr_line)
					continue;

				bool is_no_crossing_v = curr_line->AreObjectsNoCrossingByVertically(shape.get());
				bool is_higher = curr_line->m_dTop > shape->m_dBaselinePos;
				bool is_lower = curr_line->m_dBaselinePos + curr_line->m_dHeight < shape->m_dTop;
				if (is_no_crossing_v && (is_higher || is_lower))
					continue;

				for (size_t k = 0; k < curr_line->m_arConts.size(); ++k)
				{
					cont_ptr_t curr_cont = curr_line->m_arConts[k];
					if (!curr_cont)
						continue;

					bool is_width_equal = (curr_line->m_dWidth * 1.05 - shape->m_dWidth) > 0;

					bool is_crossing_text = IsLineCrossingText(shape, curr_cont) && is_width_equal;
					bool is_below_text = IsLineBelowText(shape, curr_cont) && is_width_equal;
					bool is_outline = IsOutline(shape, curr_cont);
					bool is_highlight = IsHighlight(shape, curr_cont) && curr_line->m_dHeight * 1.5 > shape->m_dHeight && is_width_equal;

					bool is_smth_true = is_crossing_text || is_below_text || is_outline || is_highlight;
					if (is_smth_true)
					{
						// if crossing a part of cont - split it
						if (shape->m_dLeft > curr_cont->m_dLeft)
						{
							auto another_cont = curr_cont->Split(shape->m_dLeft);
							if (another_cont != nullptr)
							{
								curr_line->m_arConts.insert(curr_line->m_arConts.begin() + k, another_cont);
								++k;
								curr_cont = another_cont;
							}
						}
						if (shape->m_dRight < curr_cont->m_dRight)
						{
							auto another_cont = curr_cont->Split(shape->m_dRight);
							if (another_cont != nullptr)
							{
								curr_line->m_arConts.insert(curr_line->m_arConts.begin() + k + 1, another_cont);
								++k;
							}
						}

						if (is_crossing_text)
						{
							curr_cont->m_bIsStrikeoutPresent = true;
							if (shape->m_eLineType == eLineType::ltDouble)
								curr_cont->m_bIsDoubleStrikeout = true;
						}

						else if (is_below_text)
						{
							curr_cont->m_bIsUnderlinePresent = true;
							curr_cont->m_eUnderlineType  = shape->m_eLineType;
							curr_cont->m_lUnderlineColor = shape->m_dHeight > 0.3 ? shape->m_oBrush.Color1 : shape->m_oPen.Color;
						}

						else if (is_highlight)
						{
							curr_cont->m_pShape = shape;
							curr_cont->m_bIsHighlightPresent = true;
							curr_cont->m_lHighlightColor = shape->m_oBrush.Color1;
						}

						else if (is_outline)
						{
							auto oBrush = curr_cont->m_pFontStyle->oBrush;
							oBrush.Color1 = shape->m_oPen.Color;

							curr_cont->m_pFontStyle = m_oManagers.pFontStyleManager->GetOrAddFontStyle(
								oBrush,
								curr_cont->m_pFontStyle->wsFontName,
								curr_cont->m_pFontStyle->dFontSize,
								curr_cont->m_pFontStyle->bItalic,
								curr_cont->m_pFontStyle->bBold);

							curr_cont->m_bIsShadowPresent = true;
							curr_cont->m_bIsOutlinePresent = true;
						}
					}

					// проверили - удаляем
					if (is_smth_true)
						shape_used = true;
				}
			}
			if (shape_used)
				shape = nullptr;
		}
	}

	bool CPage::IsLineCrossingText(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept
	{
		auto h_type = pCont->CBaseItem::GetHorizontalCrossingType(pShape.get());
		double dTopBorder = pCont->m_dTop + pCont->m_dHeight / 3;
		double dBotBorder = pCont->m_dBaselinePos - pCont->m_dHeight / 6;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle &&
					pShape->m_eLineType != eLineType::ltUnknown;

		// Условие пересечения по вертикали
		bool bIf2 = pShape->m_dTop > dTopBorder && pShape->m_dBaselinePos < dBotBorder;

		// Условие пересечения по горизонтали
		bool bIf3 = h_type != eHorizontalCrossingType::hctUnknown &&
					h_type != eHorizontalCrossingType::hctCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		// Условие для размеров по высоте
		bool bIf4 = pShape->m_dHeight < pCont->m_dHeight &&
					pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

		return bIf1 && bIf2 && bIf3 && bIf4;
	}

	bool CPage::IsLineBelowText(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept
	{
		auto h_type = pCont->CBaseItem::GetHorizontalCrossingType(pShape.get());
		bool bIf1 = (pShape->m_eGraphicsType == eGraphicsType::gtRectangle ||
					 pShape->m_eGraphicsType == eGraphicsType::gtCurve) &&
					pShape->m_eLineType != eLineType::ltUnknown;

		//Условие по вертикали
		double max_diff = std::min(c_dGRAPHICS_ERROR_MM * 3, pCont->m_dHeight * 0.5);
		bool bIf2 = fabs(pShape->m_dBaselinePos - pCont->m_dBaselinePos) < max_diff;

		//Условие пересечения по горизонтали
		bool bIf3 = h_type != eHorizontalCrossingType::hctUnknown &&
					h_type != eHorizontalCrossingType::hctCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		//Условие для размеров по высоте
		bool bIf4 = pShape->m_dHeight < pCont->m_dHeight * 0.5 &&
					pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

		return bIf1 && bIf2 && bIf3 && bIf4;
	}

	bool CPage::IsHighlight(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept
	{
		auto h_type = pCont->CBaseItem::GetHorizontalCrossingType(pShape.get());

		double dSomeBaseLine1 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.70;
		double dSomeBaseLine2 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.5;
		double dSomeBaseLine3 = pCont->m_dBaselinePos - pCont->m_dHeight * 0.3;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;

		//Условие пересечения по вертикали
		bool bIf2 = (dSomeBaseLine1 > pShape->m_dTop && dSomeBaseLine1 < pShape->m_dBaselinePos &&
					 dSomeBaseLine2 > pShape->m_dTop && dSomeBaseLine2 < pShape->m_dBaselinePos &&
					 dSomeBaseLine3 > pShape->m_dTop && dSomeBaseLine3 < pShape->m_dBaselinePos);

		//Условие пересечения по горизонтали
		bool bIf3 = h_type != eHorizontalCrossingType::hctUnknown &&
					h_type != eHorizontalCrossingType::hctCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext &&
					h_type != eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;

		//Цвета должны быть разными
		bool bIf4 = pCont->m_pFontStyle->oBrush.Color1 != pShape->m_oBrush.Color1;
		bool bIf5 = pShape->m_oBrush.Color1 == c_iBlackColor && pShape->m_oPen.Color == c_iWhiteColor;
		bool bIf6 = pShape->m_bIsNoFill == false;
		bool bIf7 = pShape->m_bIsNoStroke == true;

		return bIf1 && bIf2 && bIf3 && bIf4 && !bIf5 && bIf6 && bIf7;
	}

	bool CPage::IsOutline(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept
	{
		auto h_type = pCont->CBaseItem::GetHorizontalCrossingType(pShape.get());
		auto v_type = pCont->CBaseItem::GetVerticalCrossingType(pShape.get());

		bool bIsNotComplicatedFigure = pShape->m_eGraphicsType != eGraphicsType::gtComplicatedFigure;
		if (!bIsNotComplicatedFigure)
		{
			bool bIf1 = pCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;
			bool bIf2 = v_type == eVerticalCrossingType::vctCurrentOutsideNext;
			bool bIf3 = h_type == eHorizontalCrossingType::hctCurrentOutsideNext;
			bool bIf4 = h_type == eHorizontalCrossingType::hctCurrentRightOfNext;

			if (bIf1 && bIf2 && (bIf3 || bIf4))
				return true;
		}
		return false;
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
							cont->AddSymBack(d_sym->GetText().at(0), 0);
						}
						else if (bIf3 && bIf7)
						{
							cont->AddSymFront(d_sym->GetText().at(0), 0);
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

	void CPage::MergeTextLinesByVatType()
	{
		for (auto& line : m_arTextLines)
		{
			if (!line)
				continue;

			if (line->m_eVertAlignType == eVertAlignType::vatSuperscript
				|| line->m_eVertAlignType == eVertAlignType::vatSubscript)
			{
				line_ptr_t& base_line = line->m_pLine;
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

	void CPage::ToXml(NSStringUtils::CStringBuilder& oWriter) const noexcept
	{
		bool bIsNeedWP = !m_arShapes.empty();

		if (bIsNeedWP)
		{
			oWriter.WriteString(L"<w:p>");
			//note при удалении строки откуда-то добавляется <w:p/> в начале страницы (если есть графика и текст), что добавляет дополнительную строку и сдвигает текст
			oWriter.WriteString(L"<w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");
		}

		for (const auto& shape : m_arShapes)
			if (shape)
				shape->ToXml(oWriter);

		if (bIsNeedWP)
		{
			oWriter.WriteString(L"</w:p>");
		}

		for (auto& obj : m_arOutputObjects)
			obj->ToXml(oWriter);
	}

	void CPage::MergeConts()
	{
		for (size_t i = 0; i < m_arTextLines.size(); ++i)
		{
			auto& curr_line = m_arTextLines[i];
			if (!curr_line) continue;
			curr_line->MergeConts();
		}
		DetermineDominantGraphics();
	}

	void CPage::DetermineDominantGraphics()
	{
		shape_ptr_t pDominantShape = nullptr;

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

	bool CPage::IsVerticalLineBetween(item_ptr_t pFirst, item_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dRight, pSecond->m_dRight);
		double right = std::max(pFirst->m_dLeft, pSecond->m_dLeft);
		double top = std::min(pFirst->m_dTop, pSecond->m_dTop);
		double bot = std::max(pFirst->m_dBaselinePos, pSecond->m_dBaselinePos);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBaselinePos = bot + c_dGRAPHICS_ERROR_MM;

		return IsVerticalLineTrough(dummy_cont);
	}
	bool CPage::IsHorizontalLineBetween(item_ptr_t pFirst, item_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dLeft, pSecond->m_dLeft);
		double right = std::max(pFirst->m_dRight, pSecond->m_dRight);
		double top = std::min(pFirst->m_dBaselinePos, pSecond->m_dBaselinePos);
		double bot = std::max(pFirst->m_dTop, pSecond->m_dTop);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBaselinePos = bot + c_dGRAPHICS_ERROR_MM;

		return IsHorizontalLineTrough(dummy_cont);
	}
	bool CPage::IsVerticalLineBetween(line_ptr_t pFirst, line_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dRight, pSecond->m_dRight);
		double right = std::max(pFirst->m_dLeft, pSecond->m_dLeft);
		double top = std::min(pFirst->m_dTopWithMaxAscent, pSecond->m_dTopWithMaxAscent);
		double bot = std::max(pFirst->m_dBotWithMaxDescent, pSecond->m_dBotWithMaxDescent);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBaselinePos = bot + c_dGRAPHICS_ERROR_MM;

		return IsVerticalLineTrough(dummy_cont);
	}
	bool CPage::IsHorizontalLineBetween(line_ptr_t pFirst, line_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dLeft, pSecond->m_dLeft);
		double right = std::max(pFirst->m_dRight, pSecond->m_dRight);
		double top = std::min(pFirst->m_dBotWithMaxDescent, pSecond->m_dBotWithMaxDescent);
		double bot = std::max(pFirst->m_dTopWithMaxAscent, pSecond->m_dTopWithMaxAscent);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBaselinePos = bot + c_dGRAPHICS_ERROR_MM;

		return IsHorizontalLineTrough(dummy_cont);
	}

	bool CPage::IsVerticalLineTrough(item_ptr_t pFirst) const noexcept
	{
		const auto& ver_lines = m_oHorVerLinesCollector.GetVertical();
		const auto height = pFirst->m_dBaselinePos - pFirst->m_dTop;
		const auto center = pFirst->m_dTop + height / 2;

		for (const auto& line : ver_lines)
			if (line.pos > pFirst->m_dLeft && line.pos < pFirst->m_dRight && line.min <= center && line.max >= center)
				return true;

		return false;
	}
	bool CPage::IsHorizontalLineTrough(item_ptr_t pFirst) const noexcept
	{
		const auto& hor_lines = m_oHorVerLinesCollector.GetHorizontal();
		const auto width = pFirst->m_dRight - pFirst->m_dLeft;
		const auto center = pFirst->m_dLeft + width / 2;

		for (const auto& line : hor_lines)
			if (line.pos > pFirst->m_dTop && line.pos < pFirst->m_dBaselinePos && line.min <= center && line.max >= center)
				return true;

		return false;
	}

	void CPage::SplitLines()
	{
		for (size_t index = 0; index < m_arTextLines.size(); ++index)
		{
			auto& line = m_arTextLines[index];

			if (!line)
				continue;

			bool next_line = false;
			for (size_t i = 0; i < line->m_arConts.size(); ++i)
			{
				bool is_space = line->m_arConts[i] && line->m_arConts[i]->GetText().ToStdWString() == L" ";
				bool is_cont_wide = line->m_arConts[i]->m_dWidth > c_dLINE_SPLIT_DISTANCE_MM;
				bool is_shape_trough = IsVerticalLineTrough(line->m_arConts[i]);

				if ((i != line->m_arConts.size() - 1 && line->m_arConts[i + 1]->m_bPossibleSplit && is_space)
					|| (is_space && is_cont_wide)
					|| is_shape_trough)
				{
					std::vector<cont_ptr_t> line_conts_first;
					std::vector<cont_ptr_t> line_conts_second;

					// taking last cont or not
					for (size_t j = 0; j < (is_space ? i : i + 1); ++j)
						if (line->m_arConts[j])
							line_conts_first.push_back(line->m_arConts[j]);

					for (size_t j = i + 1; j < line->m_arConts.size(); ++j)
						if (line->m_arConts[j])
							line_conts_second.push_back(line->m_arConts[j]);

					line_ptr_t line_first(new CTextLine());
					line_ptr_t line_second(new CTextLine());

					line_first->AddConts(line_conts_first);
					line_second->AddConts(line_conts_second);

					if (!line_second->m_arConts.empty())
						m_arTextLines.push_back(line_second);

					if (m_arTextLines[index]->m_arConts.empty())
					{
						m_arTextLines.erase(m_arTextLines.begin() + index, m_arTextLines.begin() + index + 1);
						index--;
					}
					else
						m_arTextLines[index] = line_first;

					next_line = true;
					break;
				}
			}
			if (next_line)
				continue;
		}
	}

	std::vector<std::vector<CPage::line_ptr_t>> CPage::GetLinesByGroups()
	{
		struct Group {
			double left{};
			double right{};
			double top{};
			double bot{};
			bool closed{false};
		};

		std::vector<std::vector<line_ptr_t>> line_groups;
		std::vector<Group> groups;

		for (auto& line : m_arTextLines)
		{
			bool is_found = false;
			bool is_create_new = false;
			size_t insert_index = 0;

			for (size_t index = 0; index < groups.size(); ++index)
			{
				auto& group = groups[index];
				bool is_crossing_h = !((line->m_dRight <= group.left) || (line->m_dLeft >= group.right));
				bool is_crossing_v = !((line->m_dBotWithMaxDescent <= group.top) || (line->m_dTopWithMaxAscent >= group.bot));

				if (!group.closed && is_crossing_h)
				{
					if (is_crossing_v)
					{
						groups[index].closed = true;
						continue;
					}
					if (!is_found && !is_create_new)
					{
						is_found = true;
						insert_index = index;
					}
					else
					{
						groups[insert_index].closed = true;
						groups[index].closed = true;
						is_create_new = true;
						is_found = false;
					}
				}
			}
			if (is_found)
			{
				groups[insert_index].left = std::min(groups[insert_index].left, line->m_dLeft);
				groups[insert_index].right = std::max(groups[insert_index].right, line->m_dRight);
				groups[insert_index].bot = std::max(groups[insert_index].bot, line->m_dBaselinePos);
				groups[insert_index].top = std::min(groups[insert_index].top, line->m_dTop);
				line_groups[insert_index].push_back(line);
			}
			else
			{
				Group new_group;
				new_group.left = line->m_dLeft;
				new_group.right = line->m_dRight;
				new_group.top = line->m_dTop;
				new_group.bot = line->m_dBaselinePos;
				new_group.closed = false;
				groups.push_back(new_group);

				std::vector<line_ptr_t> line_group;
				line_group.push_back(line);
				line_groups.push_back(line_group);
			}
		}
		return line_groups;
	}

	void CPage::AnalyzeOverlapLines()
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
					prev_line->CalcFirstWordWidth();
					curr_line->CalcFirstWordWidth();

					for (auto& cont : prev_line->m_arConts)
						cont->CalcSelected();

					for (auto& cont : curr_line->m_arConts)
						cont->CalcSelected();

					m_arShapes.push_back(CreateSingleLineShape(prev_line));
					m_arShapes.push_back(CreateSingleLineShape(curr_line));
					prev_line = nullptr;
					curr_line = nullptr;
					continue;
				}
			}
		}
	}

	std::vector<CPage::paragraph_ptr_t> CPage::BuildParagraphs()
	{
		if (m_arTextLines.empty())
			return {};

		auto line_groups = GetLinesByGroups();
		std::vector<paragraph_ptr_t> ar_paragraphs;

		double min_left{m_dWidth};
		double max_right{0.0};

		// совпадает ли left, right, center со строкой ниже
		struct Position {
			bool left{false};
			bool center{false};
			bool right{false};
		};

		// lamda to setup and add paragpraph
		auto add_paragraph = [this, &max_right, &min_left, &ar_paragraphs] (paragraph_ptr_t& paragraph) {

			paragraph->m_dBaselinePos = paragraph->m_arLines.back()->m_dBaselinePos;
			paragraph->m_dTop = paragraph->m_arLines.front()->m_dTop;
			paragraph->m_dRight = max_right + c_dERROR_OF_PARAGRAPH_BORDERS_MM;
			paragraph->m_dLeft = min_left;

			paragraph->m_dWidth = paragraph->m_dRight - paragraph->m_dLeft;
			paragraph->m_dHeight = paragraph->m_dBaselinePos - paragraph->m_dTop;

			paragraph->m_dRightBorder = m_dWidth - paragraph->m_dRight;
			paragraph->m_dLeftBorder = min_left;

			paragraph->m_dLineHeight = paragraph->m_dHeight / paragraph->m_arLines.size();
			paragraph->m_bIsNeedFirstLineIndent = false;
			paragraph->m_dFirstLine = 0;
			paragraph->m_wsStyleId = m_oManagers.pParagraphStyleManager->GetDefaultParagraphStyleId(*paragraph);

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
						first_left = fabs(curr_line->m_dLeft - prev_line->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;
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
					double left_diff = paragraph->m_arLines[0]->m_dLeft - paragraph->m_arLines[1]->m_dLeft;
					paragraph->m_bIsNeedFirstLineIndent = true;
					paragraph->m_dFirstLine = left_diff;

					if (left_diff < 0)
						paragraph->m_dLeftBorder -= left_diff;
				}
			}

			ar_paragraphs.push_back(std::move(paragraph));
			paragraph = std::make_shared<CParagraph>();

			min_left = m_dWidth;
			max_right = 0.0;
		};

		// lamda to add line and setup min_left/max_right
		auto add_line = [&min_left, &max_right] (paragraph_ptr_t& paragraph, const line_ptr_t& curr_line) {
			min_left = std::min(min_left, curr_line->m_dLeft);
			max_right = std::max(max_right, curr_line->m_dRight);
			paragraph->m_arLines.push_back(curr_line);
		};

		auto build_paragraphs = [this, add_line, add_paragraph] (const std::vector<line_ptr_t>& text_lines) {
			// ar_spacing[index]- расстояние строки до строки снизу
			// если 0.0 - строка последняя
			std::vector<double> ar_spacings(text_lines.size(), 0.0);

			// позиции относительно других линий
			std::vector<Position> ar_positions(text_lines.size());

			// требуется ли отступ
			std::vector<bool> ar_indents(text_lines.size(), false);

			// если ar_delims[index] == true, после строчки index нужно начинать новый параграф
			std::vector<bool> ar_delims(text_lines.size(), false);

			double avg_spacing{0.0};
			size_t avg_spacing_n{0};

			// параграф будет набиваться строчками
			auto paragraph = std::make_shared<CParagraph>();

			// calcs first word widths
			for (auto& line : text_lines)
				line->CalcFirstWordWidth();

			// calcs spacings & positions
			for (size_t index = 0; index < text_lines.size() - 1; ++index)
			{
				ar_spacings[index] = text_lines[index + 1]->m_dTop - text_lines[index]->m_dBaselinePos;
				avg_spacing = (avg_spacing / (avg_spacing_n + 1)) * avg_spacing_n + (ar_spacings[index] / (avg_spacing_n + 1));

				auto& left_curr = text_lines[index]->m_dLeft;
				auto& left_next = text_lines[index + 1]->m_dLeft;

				auto& right_curr = text_lines[index]->m_dRight;
				auto& right_next = text_lines[index + 1]->m_dRight;

				auto center_curr = (text_lines[index]->m_dLeft + text_lines[index]->m_dWidth / 2);
				auto center_next = (text_lines[index + 1]->m_dLeft + text_lines[index + 1]->m_dWidth / 2);

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
					if (same_double_top == same_double_bot)
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
			Position curr_position;

			for (size_t index = 0; index < ar_positions.size() - 1; ++index)
			{
				Position& position = ar_positions[index];

				auto& line_top = text_lines[index];
				auto& line_bot = text_lines[index + 1];

				if (index == 0 || ar_delims[index - 1])
				{
					is_first_line = true;
					curr_position = position;
				}
				else
				{
					is_first_line = false;
					curr_position.left &= position.left;
					curr_position.center &= position.center;
					curr_position.right &= position.right;
				}

				// первая строка может быть с отступом
				if (is_first_line)
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
				if (ar_indents[index])
					curr_position.left = true;

				bool is_unknown = !(curr_position.left || curr_position.right || curr_position.center);
				bool is_enum = false;
				if (is_unknown)
				{
					// bullet paragraphs
					if (!ar_delims[index])
					{
						double left_no_enum = line_top->GetLeftNoEnum();
						is_enum = left_no_enum != line_top->m_dLeft && fabs(left_no_enum - line_bot->m_dLeft) < c_dERROR_OF_PARAGRAPH_BORDERS_MM;
					}
					if (!is_enum)
						ar_delims[index] = true;
				}
			}

			// gap check
			//
			// bla-bla-bla
			// text bla-bla-bla-bla
			//
			// bla-bla-bla text
			// bla-bla-bla-bla

			double curr_max_right = text_lines[0]->m_dRight;
			double curr_min_left = text_lines[0]->m_dLeft;
			for (size_t index = 0; index < ar_positions.size() - 1; ++index)
			{
				Position position = ar_positions[index];
				auto& line_top = text_lines[index];
				auto& line_bot = text_lines[index + 1];

				if (ar_delims[index])
				{
					curr_max_right = line_bot->m_dRight;
					curr_min_left = line_bot->m_dLeft;
					continue;
				}

				cont_ptr_t cont = line_bot->m_arConts[0];
				double line_with_first_right = line_top->m_dRight + line_bot->m_dFirstWordWidth;
				double line_with_first_left = line_top->m_dLeft - line_bot->m_dFirstWordWidth;

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
					continue;
				else
				{
					ar_delims[index] = true;
					curr_max_right = line_bot->m_dRight;
					curr_min_left = line_bot->m_dLeft;
				}
			}

			// first symbol delim check (bullets etc...)
			for (size_t index = 0; index < text_lines.size() - 1; ++index)
			{
				const auto& first_sym = text_lines[index + 1]->m_arConts.front()->GetText().at(0);
				if (CContText::IsUnicodeBullet(first_sym))
					ar_delims[index] = true;
			}

			// если между линий шейп - делим
			for (size_t index = 0; index < ar_positions.size() - 1; ++index)
			{
				if (IsHorizontalLineBetween(text_lines[index], text_lines[index + 1]))
					ar_delims[index] = true;
			}

			// на основе ar_delims разбиваем на параграфы
			for (size_t index = 0; index < ar_delims.size(); ++index)
			{
				add_line(paragraph, text_lines[index]);
				if (ar_delims[index] || index == ar_delims.size() - 1)
					add_paragraph(paragraph);
			}
		};

		// 1 строчка в параграфе
		if (m_eTextAssociationType == TextAssociationType::tatPlainLine ||
			m_eTextAssociationType == TextAssociationType::tatShapeLine)
		{
			auto paragraph = std::make_shared<CParagraph>();
			for (auto& curr_line : m_arTextLines)
			{
				add_line(paragraph, curr_line);
				add_paragraph(paragraph);
			}
		}

		else if (m_eTextAssociationType == TextAssociationType::tatPlainParagraph ||
				 m_eTextAssociationType == TextAssociationType::tatParagraphToShape)
		{
			for (auto& g : line_groups)
				build_paragraphs(g);
		}

		std::sort(ar_paragraphs.begin(), ar_paragraphs.end(), [] (const paragraph_ptr_t& a, const paragraph_ptr_t& b) {
			return a->m_dBaselinePos < b->m_dBaselinePos;
		});

		return ar_paragraphs;
	}

	std::vector<CPage::table_ptr_t> CPage::BuildTables()
	{
		auto cells = BuildCells();
		auto rows = BuildRows(cells);
		std::vector<table_ptr_t> tables;

		table_ptr_t curr_table = nullptr;
		for (const auto& row : rows)
		{
			if (!curr_table)
			{
				curr_table = std::make_shared<CTable>();
			}
			else if (fabs(curr_table->m_dBaselinePos - row->m_dTop) > c_dGRAPHICS_ERROR_MM)
			{
				tables.push_back(std::move(curr_table));
				curr_table = std::make_shared<CTable>();
			}
			curr_table->AddRow(row);
		}
		if (!curr_table->IsEmpty())
			tables.push_back(std::move(curr_table));

		return tables;
	}
	std::vector<CTable::cell_ptr_t> CPage::BuildCells()
	{
		struct Crossing
		{
			Point p {};
			std::vector<Point> lines {};
			Crossing(const Point& _p, const std::vector<Point>& _lines)
			{
				p = _p;
				lines = _lines;
			}
		};
		std::vector<Crossing> crossings;
		auto find_crossing = [&crossings] (const Point& p) -> Crossing* {
			for (auto& crossing : crossings)
			{
				if (fabs(crossing.p.x - p.x) < c_dGRAPHICS_ERROR_MM &&
				        fabs(crossing.p.y - p.y) < c_dGRAPHICS_ERROR_MM)
					return &crossing;
			}
			return nullptr;
		};

		for (const auto& shape : m_arShapes)
		{
			if (!shape)
				continue;

			Point prev {};
			for (const auto& command : shape->m_oVector.GetData())
			{
				if (!command.points.empty())
				{
					if (command.type == CVectorGraphics::ePathCommandType::pctLine)
					{
						const auto& curr = command.points.back();
						auto prev_crossing = find_crossing(prev);
						auto curr_crossing = find_crossing(curr);

						// add empty vector if no exists
						if (!prev_crossing)
							crossings.push_back({prev, {curr}});
						else
							prev_crossing->lines.push_back(curr);

						if (!curr_crossing)
							crossings.push_back({curr, {prev}});
						else
							curr_crossing->lines.push_back(prev);

						prev = curr;
					}
					prev = command.points.back();
				}
			}
		}

		// sorting guarantee creating cell once (by taking second cross j > i)
		std::sort(crossings.begin(), crossings.end(), [] (const Crossing& c1, const Crossing& c2) {
			if (fabs(c1.p.y - c2.p.y) < c_dGRAPHICS_ERROR_MM)
				return c1.p.x < c2.p.x;
			return c1.p.y < c2.p.y;
		});

		std::vector<CTable::cell_ptr_t> cells;
		for (size_t i = 0; i < crossings.size(); ++i)
		{
			for (size_t j = i + 1; j < crossings.size(); ++j)
			{
				const auto& first = crossings.at(i);
				const auto& second = crossings.at(j);

				// first should be top left corner
				// and second right bot (not the same line ofc)
				if (fabs(first.p.x - second.p.x) < c_dGRAPHICS_ERROR_MM ||
				        first.p.x > second.p.x ||
				        fabs(first.p.y - second.p.y) < c_dGRAPHICS_ERROR_MM ||
				        first.p.y > second.p.y)
					continue;

				// 2 points should be the same
				size_t equals = 0;
				for (const auto& fl : first.lines)
					for (const auto& sl : second.lines)
						if (fabs(fl.x - sl.x) < c_dGRAPHICS_ERROR_MM && fabs(fl.y - sl.y) < c_dGRAPHICS_ERROR_MM)
							++equals;

				if (equals == 2)
				{
					auto cell = std::make_shared<CTable::CCell>();
					cell->m_dLeft = first.p.x;
					cell->m_dRight = second.p.x;
					cell->m_dTop = first.p.y;
					cell->m_dBaselinePos = second.p.y;
					cell->m_dWidth = cell->m_dRight - cell->m_dLeft;
					cell->m_dHeight = cell->m_dBaselinePos - cell->m_dTop;
					cells.push_back(cell);
					break;
				}
			}
		}

		// sets paragraphs into cells
		for (auto& cell : cells)
			for (auto& paragraph : m_arParagraphs)
			{
				if (!paragraph)
					continue;

				bool top = fabs(cell->m_dTop - paragraph->m_dTop) < 2 * c_dGRAPHICS_ERROR_MM || paragraph->m_dTop > cell->m_dTop;
				bool bot = fabs(cell->m_dBaselinePos - paragraph->m_dBaselinePos) < c_dGRAPHICS_ERROR_MM || paragraph->m_dBaselinePos < cell->m_dBaselinePos;
				bool left = fabs(cell->m_dLeft - paragraph->m_dLeft) < c_dGRAPHICS_ERROR_MM || paragraph->m_dLeft > cell->m_dLeft;
				bool right = fabs(cell->m_dRight - paragraph->m_dRight) < c_dGRAPHICS_ERROR_MM || paragraph->m_dRight < cell->m_dRight;
				if (top && bot && left && right)
				{
					cell->AddParagraph(paragraph);
					paragraph = nullptr;
				}
			}
		auto right = MoveNullptr(m_arParagraphs.begin(), m_arParagraphs.end());
		m_arParagraphs.erase(right, m_arParagraphs.end());
		std::sort(m_arParagraphs.begin(), m_arParagraphs.end(), [] (const paragraph_ptr_t& p1, const paragraph_ptr_t& p2) {
			return p1->m_dBaselinePos < p2->m_dBaselinePos;
		});
		return cells;
	}
	std::vector<CTable::row_ptr_t> CPage::BuildRows(std::vector<CTable::cell_ptr_t>& arCells)
	{
		std::sort(arCells.begin(), arCells.end(), [] (const CTable::cell_ptr_t c1, const CTable::cell_ptr_t& c2) {
			if (fabs(c1->m_dBaselinePos - c2->m_dBaselinePos) < c_dGRAPHICS_ERROR_MM)
				return c1->m_dLeft < c2->m_dLeft;
			return c1->m_dBaselinePos < c2->m_dBaselinePos;
		});

		std::vector<CTable::row_ptr_t> rows;
		CTable::row_ptr_t curr_row = nullptr;
		for (auto& cell : arCells)
		{
			if (!curr_row)
			{
				curr_row = std::make_shared<CTable::CRow>();
			}
			else if (fabs(curr_row->m_dBaselinePos - cell->m_dBaselinePos) > c_dGRAPHICS_ERROR_MM)
			{
				rows.push_back(std::move(curr_row));
				curr_row = std::make_shared<CTable::CRow>();
			}
			curr_row->AddCell(cell);
		}
		if (!curr_row->IsEmpty())
			rows.push_back(std::move(curr_row));

		return rows;
	}

	CPage::shape_ptr_t CPage::CreateSingleLineShape(line_ptr_t& pLine)
	{
		auto pParagraph = std::make_shared<CParagraph>();

		pParagraph->m_arLines.push_back(pLine);
		pParagraph->m_dLeft = pLine->m_dLeft;
		pParagraph->m_dTop = pLine->m_dTop;
		pParagraph->m_dBaselinePos = pLine->m_dBaselinePos;
		pParagraph->m_dWidth = pLine->m_dWidth + c_dERROR_OF_PARAGRAPH_BORDERS_MM;
		pParagraph->m_dHeight = pLine->m_dHeight;
		pParagraph->m_dRight = pLine->m_dRight;
		pParagraph->m_dLineHeight = pParagraph->m_dHeight;

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

	CPage::shape_ptr_t CPage::CreateSingleParagraphShape(paragraph_ptr_t& pParagraph)
	{
		auto pShape = std::make_shared<CShape>();

		pShape->m_dLeft = pParagraph->m_dLeft;
		pShape->m_dTop = pParagraph->m_dTop;
		pShape->m_dRight =  pParagraph->m_dRight;
		pShape->m_dBaselinePos = pParagraph->m_dBaselinePos;
		pShape->m_dHeight = pParagraph->m_dHeight;
		pShape->m_dWidth = pParagraph->m_dWidth;

		if (pParagraph->m_bIsNeedFirstLineIndent && pParagraph->m_dFirstLine < 0)
			pParagraph->m_dLeftBorder = -pParagraph->m_dFirstLine;
		else
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

	void CPage::WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter) const noexcept
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

	void CPage::DrawImage(shape_ptr_t pShape, std::shared_ptr<CImageInfo> oImgInfo, CVectorGraphics& imageVector)
	{
		pShape->m_pImageInfo = oImgInfo;
		pShape->m_eType = CShape::eShapeType::stVectorTexture;
		imageVector.RotateAt(-pShape->m_dRotation, pShape->m_oVector.GetCenter());

		pShape->m_dImageBot = imageVector.GetBottom();
		pShape->m_dImageTop = imageVector.GetTop();
		pShape->m_dImageLeft = imageVector.GetLeft();
		pShape->m_dImageRight = imageVector.GetRight();
	}

	void CPage::DrawGradient(shape_ptr_t pShape)
	{
		// image with gradient must be closed
		if (!pShape->m_oVector.IsEmpty() && pShape->m_oVector.GetData().back().type != CVectorGraphics::ePathCommandType::pctClose)
			pShape->m_oVector.Add({CVectorGraphics::ePathCommandType::pctClose, {}});

		long width_pix = static_cast<long>(pShape->m_dWidth * c_dMMToPix);
		long height_pix = static_cast<long>(pShape->m_dHeight * c_dMMToPix);

		if (width_pix == 0) width_pix = 1;
		if (height_pix == 0) height_pix = 1;

		const long step = 4;
		const long stride = -step * width_pix;

		std::unique_ptr<CBgraFrame> frame(new CBgraFrame());
		size_t data_size = width_pix * height_pix * step;
		BYTE* data = new BYTE[data_size];

		// white and alpha is min (full transparent)
		for (size_t i = 0; i < width_pix * height_pix; ++i)
			reinterpret_cast<unsigned int*>(data)[i] = 0x00ffffff;

		frame->put_Data(data);
		frame->put_Height(height_pix);
		frame->put_Width(width_pix);
		frame->put_Stride(stride);

		auto shifted_vector = pShape->m_oVector;
		Aggplus::CMatrix transform_matrix;
		transform_matrix.Translate(-shifted_vector.GetLeft(), -shifted_vector.GetTop());
		shifted_vector.Transform(transform_matrix);

		NSStructures::CBrush shifted_brush = pShape->m_oBrush;
		shifted_brush.Bounds.left = shifted_vector.GetLeft();
		shifted_brush.Bounds.right = shifted_vector.GetRight();
		shifted_brush.Bounds.bottom = shifted_vector.GetBottom();
		shifted_brush.Bounds.top = shifted_vector.GetTop();
		shifted_brush.m_oGradientInfo.transform(transform_matrix);

		NSGraphics::IGraphicsRenderer* g_renderer = NSGraphics::Create();
		g_renderer->CreateFromBgraFrame(frame.get());
		g_renderer->SetSwapRGB(false);
		g_renderer->put_Width(pShape->m_dWidth);
		g_renderer->put_Height(pShape->m_dHeight);
		g_renderer->RestoreBrush(shifted_brush);
		g_renderer->RestorePen(m_oPen);
		g_renderer->BeginCommand(c_nPathType);
		shifted_vector.DrawOnRenderer(g_renderer);
		g_renderer->DrawPath(c_nWindingFillMode);
		g_renderer->EndCommand(c_nPathType);

		Aggplus::CImage img;
		img.Create(data, width_pix, height_pix, stride, true);
		auto info = m_oManagers.pImageManager->WriteImage(&img, pShape->m_dTop, pShape->m_dBaselinePos, pShape->m_dWidth, pShape->m_dHeight);
		pShape->m_dRotation = 0;

		DrawImage(pShape, info, pShape->m_oVector);

		RELEASEINTERFACE(g_renderer)
	}

	agg::rgba8 CPage::GetFillColor()
	{
		bool is_one = true;
		agg::rgba8 color = m_arOneColorGradientShape[0]->m_oBrush.m_oGradientInfo.getFillColor();
		for (const auto& s : m_arOneColorGradientShape)
			if (s->m_oBrush.m_oGradientInfo.getFillColor() != color)
				is_one = false;

		if (is_one)
			return color;
		else
			return agg::rgba8();
	}

	void CPage::FillLuminosityShapes()
	{
		if (GetFillColor() == agg::rgba8::no_color())
		{
			for (auto itt = m_arOneColorGradientShape.begin(); itt != m_arOneColorGradientShape.end();)
			{
				for (auto it = m_arLuminosityShapes.begin(); it != m_arLuminosityShapes.end();)
				{
					if ((*it)->m_oVector.GetRight() > (*itt)->m_oVector.GetLeft() && (*it)->m_oVector.GetBottom() > (*itt)->m_oVector.GetTop() &&
						(*it)->m_oVector.GetLeft() < (*itt)->m_oVector.GetRight() && (*it)->m_oVector.GetTop() < (*itt)->m_oVector.GetBottom())
					{
						(*it)->m_oBrush.m_oGradientInfo.setFillColor((*itt)->m_oBrush.m_oGradientInfo.getFillColor());
						DrawGradient((*it));
						it = m_arLuminosityShapes.erase(it);
					}
					else
						++it;
				}
				itt = m_arOneColorGradientShape.erase(itt);
			}
		}
		else
		{
			auto fill_color = GetFillColor();
			for (auto& s : m_arLuminosityShapes)
			{
				s->m_oBrush.m_oGradientInfo.setFillColor(fill_color);
				DrawGradient(s);
			}

			m_arLuminosityShapes.clear();
		}
	}
}
