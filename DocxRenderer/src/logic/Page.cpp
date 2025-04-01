#include "Page.h"

#include <memory>
#include <map>
#include <set>

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
			// closing clip path if non-closed
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
		m_arTextLines.clear();
		m_arShapes.clear();
		m_arParagraphs.clear();
		m_arTables.clear();
		m_arOutputObjects.clear();
		m_oCurrVectorGraphics.Clear();
		m_oClipVectorGraphics.Clear();
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
				if (line && (line->m_dTop >= m_dHeight || line->m_dBot <= 0))
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
			}
			if (lType & c_nWindingFillMode || lType & c_nEvenOddFillMode)
			{
				s->m_bIsNoFill = false;
				s->m_oBrush = m_oBrush;
			}
			s->m_oPen = m_oPen;
			s->m_oPen.Size *= transform_det;
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

		auto info = pInfo;
		if (!info && m_bIsGradient)
		{
			// image with gradient must be closed
			if (!shape->m_oVector.IsEmpty() && shape->m_oVector.GetData().back().type != CVectorGraphics::ePathCommandType::pctClose)
				shape->m_oVector.Add({CVectorGraphics::ePathCommandType::pctClose, {}});

			long width_pix = static_cast<long>(shape->m_dWidth * c_dMMToPix);
			long height_pix = static_cast<long>(shape->m_dHeight * c_dMMToPix);

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

			auto shifted_vector = shape->m_oVector;
			Aggplus::CMatrix transform_matrix;
			transform_matrix.Translate(-shifted_vector.GetLeft(), -shifted_vector.GetTop());
			shifted_vector.Transform(transform_matrix);

			NSStructures::CBrush shifted_brush = m_oBrush;
			shifted_brush.Bounds.left = shifted_vector.GetLeft();
			shifted_brush.Bounds.right = shifted_vector.GetRight();
			shifted_brush.Bounds.bottom = shifted_vector.GetBottom();
			shifted_brush.Bounds.top = shifted_vector.GetTop();
			shifted_brush.m_oGradientInfo.transform(transform_matrix);

			NSGraphics::IGraphicsRenderer* g_renderer = NSGraphics::Create();
			g_renderer->CreateFromBgraFrame(frame.get());
			g_renderer->SetSwapRGB(false);
			g_renderer->put_Width(shape->m_dWidth);
			g_renderer->put_Height(shape->m_dHeight);
			g_renderer->RestoreBrush(shifted_brush);
			g_renderer->RestorePen(m_oPen);
			g_renderer->BeginCommand(c_nPathType);
			shifted_vector.DrawOnRenderer(g_renderer);
			g_renderer->DrawPath(c_nWindingFillMode);
			g_renderer->EndCommand(c_nPathType);

			Aggplus::CImage img;
			img.Create(data, width_pix, height_pix, stride, true);
			info = m_oManagers.pImageManager->WriteImage(&img, shape->m_dTop, shape->m_dBot, shape->m_dWidth, shape->m_dHeight);
			rotation = 0;
			image_vector = shape->m_oVector;

			m_bIsGradient = false;
			RELEASEINTERFACE(g_renderer)
		}
		shape->m_dRotation = rotation;

		if (info)
		{
			shape->m_pImageInfo = info;
			shape->m_eType = CShape::eShapeType::stVectorTexture;
			image_vector.RotateAt(-shape->m_dRotation, shape->m_oVector.GetCenter());

			shape->m_dImageBot = image_vector.GetBottom();
			shape->m_dImageTop = image_vector.GetTop();
			shape->m_dImageLeft = image_vector.GetLeft();
			shape->m_dImageRight = image_vector.GetRight();
		}
		else
			shape->m_eType = CShape::eShapeType::stVectorGraphics;

		if (!shape->IsOoxmlValid())
			return;

		// big white shape with page width & height skip
		if (fabs(shape->m_dHeight - m_dHeight) <= c_dSHAPE_X_OFFSET * 2 &&
		        fabs(shape->m_dWidth - m_dWidth) <= c_dSHAPE_X_OFFSET * 2 &&
		        shape->m_oBrush.Color1 == c_iWhiteColor)
			return;

		shape->m_nOrder = ++m_nShapeOrder;
		m_arShapes.push_back(shape);
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
		auto conts = m_oContBuilder.GetConts();
		auto diacs = m_oContBuilder.GetDiacs();

		m_arTextLines = BuildTextLines(conts);
		AddDiacriticalSymbols(diacs);

		// analyzing data. can take shapes and set them to nullptr
		AnalyzeShapes();
		AnalyzeTextLines();

		auto text_line_groups = BuildTextLineGroups();

		// building final objects
		m_arParagraphs = BuildParagraphs(text_line_groups);

		// if (m_bIsBuildTables)
		// m_arTables = BuildTables();

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

	std::vector<CPage::text_line_ptr_t> CPage::BuildTextLines(const std::vector<cont_ptr_t>& arConts)
	{
		text_line_ptr_t curr_line = nullptr;
		std::vector<text_line_ptr_t> text_lines;

		for (auto& cont : arConts)
		{
			if (!cont)
				continue;

			double avg_space_width = cont->m_pFontStyle->GetAvgSpaceWidth();
			double space_width =
			        avg_space_width != 0.0 ?
			            avg_space_width * c_dAVERAGE_SPACE_WIDTH_COEF :
			            cont->CalculateSpace() * c_dSPACE_WIDTH_COEF;

			if (curr_line && fabs(curr_line->m_dBot - cont->m_dBot) <= c_dTHE_SAME_STRING_Y_PRECISION_MM &&
			        fabs(curr_line->m_dRight - cont->m_dLeft) <= 2 * space_width)
			{
				curr_line->AddCont(cont);
				continue;
			}

			bool skip = false;
			for (size_t i = 0; i < text_lines.size(); ++i)
			{
				if (fabs(text_lines[i]->m_dBot - cont->m_dBot) <= c_dTHE_SAME_STRING_Y_PRECISION_MM &&
				        fabs(curr_line->m_dRight - cont->m_dLeft) <= 2 * space_width)
				{
					curr_line = text_lines[i];
					cont->m_bPossibleHorSplit = true;
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
			{
				text_lines.erase(text_lines.begin() + i, text_lines.begin() + i + 1);
				--i;
			}
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

	std::vector<CPage::ooxml_item_ptr_t> CPage::BuildOutputObjects()
	{
		std::vector<ooxml_item_ptr_t> output_objects;

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
					while (second_index < m_arParagraphs.size() && first_p->m_dBot > second_p->m_dTop)
						second_index++;
				}

				if (second_index - first_index == 1)
				{
					auto& p = m_arParagraphs[first_index];

					if (!prev_p)
						p->m_dSpaceBefore = p->m_dTop + c_dCORRECTION_FOR_FIRST_PARAGRAPH;
					else
						p->m_dSpaceBefore = p->m_dTop - prev_p->m_dBot;

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
				bool bIf5 = fabs(m_arShapes[i]->m_dBot - m_arShapes[j]->m_dBot) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5;

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
				        shape->m_dBot > this->m_dHeight * out_of_page_coeff ||
				        shape->m_dLeft < 0 ||
				        shape->m_dRight > this->m_dWidth * out_of_page_coeff;


				bool is_too_big = (!shape->m_bIsNoFill && (shape->m_dWidth > c_dSHAPE_TROUGH_MAX_MM && shape->m_dHeight > c_dSHAPE_TROUGH_MAX_MM));

				if (is_too_big || is_out_of_page)
					continue;

				m_oHorVerLinesCollector.AddVector(shape->m_oVector);
			}
	}

	void CPage::AnalyzeTextLines()
	{
		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const text_line_ptr_t& a, const text_line_ptr_t& b) {
			return a->m_dBot < b->m_dBot;
		});

		AnalyzeDropCaps();
		AnalyzeConts();
		AnalyzeEffects();

		GetHorVerLines();

		MergeTextLinesByVatType();
		DeleteTextClipPage();
		MergeConts();
		SplitLines();
		AnalyzeOverlapLines();

		for (auto& line : m_arTextLines)
			if (line && line->m_arConts.empty())
				line = nullptr;

		auto right = MoveNullptr(m_arTextLines.begin(), m_arTextLines.end());
		m_arTextLines.erase(right, m_arTextLines.end());

		std::sort(m_arTextLines.begin(), m_arTextLines.end(), [] (const text_line_ptr_t& a, const text_line_ptr_t& b) {
			return a->m_dBot < b->m_dBot;
		});
	}

	void CPage::AnalyzeDropCaps()
	{
		double avg_font_size = m_oManagers.pParagraphStyleManager->GetAvgFontSize();

		std::vector<std::pair<cont_ptr_t&, text_line_ptr_t&>> possible_caps;
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
			double shape_coverage = 0;
			if (!shape || shape->m_eGraphicsType == eGraphicsType::gtNoGraphics)
				continue;

			bool shape_used = false;
			for (size_t j = 0; j < m_arTextLines.size(); ++j)
			{
				auto& curr_line = m_arTextLines[j];
				if (!curr_line)
					continue;

				bool is_no_crossing_v = curr_line->AreObjectsNoCrossingByVertically(shape.get());
				bool is_higher = curr_line->m_dTop > shape->m_dBot;
				bool is_lower = curr_line->m_dBot + curr_line->m_dHeight < shape->m_dTop;
				if (is_no_crossing_v && (is_higher || is_lower))
					continue;

				for (size_t k = 0; k < curr_line->m_arConts.size(); ++k)
				{
					cont_ptr_t curr_cont = curr_line->m_arConts[k];
					if (!curr_cont)
						continue;

					bool is_line_width_equal = (curr_line->m_dWidth * 1.05 - shape->m_dWidth) > 0;
					bool is_line_height_equal = (curr_line->m_dHeight * 1.5 - shape->m_dHeight) > 0;

					bool is_crossing_text = IsLineCrossingText(shape, curr_cont) && is_line_width_equal;
					bool is_below_text = IsLineBelowText(shape, curr_cont) && is_line_width_equal;
					bool is_outline = IsOutline(shape, curr_cont);
					bool is_highlight = IsHighlight(shape, curr_cont) && is_line_height_equal && is_line_width_equal;

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

							shape_coverage += curr_cont->m_dWidth / shape->m_dWidth;
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
			if (shape_used && (shape_coverage == 0 || shape_coverage > 0.8))
				shape = nullptr;
		}
	}

	bool CPage::IsLineCrossingText(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept
	{
		auto h_type = pCont->CBaseItem::GetHorizontalCrossingType(pShape.get());
		double dTopBorder = pCont->m_dTop + pCont->m_dHeight / 3;
		double dBotBorder = pCont->m_dBot - pCont->m_dHeight / 6;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle &&
		        pShape->m_eLineType != eLineType::ltUnknown;

		// Условие пересечения по вертикали
		bool bIf2 = pShape->m_dTop > dTopBorder && pShape->m_dBot < dBotBorder;

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
		bool bIf2 = fabs(pShape->m_dBot - pCont->m_dBot) < max_diff;

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

		double dSomeBaseLine1 = pCont->m_dBot - pCont->m_dHeight * 0.70;
		double dSomeBaseLine2 = pCont->m_dBot - pCont->m_dHeight * 0.5;
		double dSomeBaseLine3 = pCont->m_dBot - pCont->m_dHeight * 0.3;

		bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;

		//Условие пересечения по вертикали
		bool bIf2 = (dSomeBaseLine1 > pShape->m_dTop && dSomeBaseLine1 < pShape->m_dBot &&
		             dSomeBaseLine2 > pShape->m_dTop && dSomeBaseLine2 < pShape->m_dBot &&
		             dSomeBaseLine3 > pShape->m_dTop && dSomeBaseLine3 < pShape->m_dBot);

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

	void CPage::AddDiacriticalSymbols(const std::vector<cont_ptr_t>& arDiac)
	{
		for (auto& d_sym : arDiac)
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
				text_line_ptr_t& base_line = line->m_pLine;
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

	bool CPage::IsVerticalLineBetween(base_item_ptr_t pFirst, base_item_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dRight, pSecond->m_dRight);
		double right = std::max(pFirst->m_dLeft, pSecond->m_dLeft);
		double top = std::min(pFirst->m_dTop, pSecond->m_dTop);
		double bot = std::max(pFirst->m_dBot, pSecond->m_dBot);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBot = bot + c_dGRAPHICS_ERROR_MM;

		return IsVerticalLineTrough(dummy_cont);
	}
	bool CPage::IsHorizontalLineBetween(base_item_ptr_t pFirst, base_item_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dLeft, pSecond->m_dLeft);
		double right = std::max(pFirst->m_dRight, pSecond->m_dRight);
		double top = std::min(pFirst->m_dBot, pSecond->m_dBot);
		double bot = std::max(pFirst->m_dTop, pSecond->m_dTop);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBot = bot + c_dGRAPHICS_ERROR_MM;

		return IsHorizontalLineTrough(dummy_cont);
	}
	bool CPage::IsVerticalLineBetween(text_line_ptr_t pFirst, text_line_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dRight, pSecond->m_dRight);
		double right = std::max(pFirst->m_dLeft, pSecond->m_dLeft);
		double top = std::min(pFirst->m_dTopWithMaxAscent, pSecond->m_dTopWithMaxAscent);
		double bot = std::max(pFirst->m_dBotWithMaxDescent, pSecond->m_dBotWithMaxDescent);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBot = bot + c_dGRAPHICS_ERROR_MM;

		return IsVerticalLineTrough(dummy_cont);
	}
	bool CPage::IsHorizontalLineBetween(text_line_ptr_t pFirst, text_line_ptr_t pSecond) const noexcept
	{
		double left = std::min(pFirst->m_dLeft, pSecond->m_dLeft);
		double right = std::max(pFirst->m_dRight, pSecond->m_dRight);
		double top = std::min(pFirst->m_dBotWithMaxDescent, pSecond->m_dBotWithMaxDescent);
		double bot = std::max(pFirst->m_dTopWithMaxAscent, pSecond->m_dTopWithMaxAscent);

		auto dummy_cont = std::make_shared<CContText>();
		dummy_cont->m_dLeft = left - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dRight = right + c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dTop = top - c_dGRAPHICS_ERROR_MM;
		dummy_cont->m_dBot = bot + c_dGRAPHICS_ERROR_MM;

		return IsHorizontalLineTrough(dummy_cont);
	}

	bool CPage::IsVerticalLineTrough(base_item_ptr_t pFirst) const noexcept
	{
		const auto& ver_lines = m_oHorVerLinesCollector.GetVertical();
		const auto height = pFirst->m_dBot - pFirst->m_dTop;
		const auto center = pFirst->m_dTop + height / 2;

		for (const auto& line : ver_lines)
			if (line.pos > pFirst->m_dLeft && line.pos < pFirst->m_dRight && line.min <= center && line.max >= center)
				return true;

		return false;
	}
	bool CPage::IsHorizontalLineTrough(base_item_ptr_t pFirst) const noexcept
	{
		const auto& hor_lines = m_oHorVerLinesCollector.GetHorizontal();
		const auto width = pFirst->m_dRight - pFirst->m_dLeft;
		const auto center = pFirst->m_dLeft + width / 2;

		for (const auto& line : hor_lines)
			if (line.pos > pFirst->m_dTop && line.pos < pFirst->m_dBot && line.min <= center && line.max >= center)
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

				if ((i != line->m_arConts.size() - 1 && line->m_arConts[i + 1]->m_bPossibleHorSplit && is_space)
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

					text_line_ptr_t line_first(new CTextLine());
					text_line_ptr_t line_second(new CTextLine());

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

	std::vector<std::shared_ptr<CTextLineGroup>> CPage::BuildTextLineGroups()
	{
		if (m_arTextLines.empty())
			return {};

		double curr_bot = std::numeric_limits<double>::max();
		std::vector<std::vector<text_line_ptr_t>> bot_aligned_text_lines;

		for (const auto& line : m_arTextLines)
		{
			if (fabs(line->m_dBotWithMaxDescent - curr_bot) < 4 * c_dTHE_SAME_STRING_Y_PRECISION_MM)
			{
				bot_aligned_text_lines.back().push_back(line);
			}
			else
			{
				bot_aligned_text_lines.push_back({});
				bot_aligned_text_lines.back().push_back(line);
				curr_bot = line->m_dBotWithMaxDescent;
			}
		}

		std::vector<std::shared_ptr<CTextLineGroup>> text_line_groups;
		std::vector<bool> ar_is_group_open;
		for (const auto& text_lines : bot_aligned_text_lines)
		{
			// lines [i] belongs group [j] (like a matrix)
			// only 1 [i] to 1 [j] on the same bot position
			std::vector<std::vector<bool>> lines_x_groups(text_lines.size());
			for (auto& lxg : lines_x_groups)
				lxg.resize(text_line_groups.size());

			for (size_t i = 0; i < text_lines.size(); ++i)
			{
				for (size_t j = 0; j < text_line_groups.size(); ++j)
				{
					if (!ar_is_group_open[j])
						continue;

					// line inside of the group
					if (CmpOrEqual(text_lines[i]->m_dLeft, text_line_groups[j]->m_dRight, c_dTHE_SAME_STRING_X_PRECISION_MM) &&
					        CmpOrEqual(text_line_groups[j]->m_dLeft, text_lines[i]->m_dRight, c_dTHE_SAME_STRING_X_PRECISION_MM))
					{
						lines_x_groups[i][j] = true;
					}
				}
			}

			for (size_t j = 0; j < text_line_groups.size(); ++j)
			{
				size_t lines_counter = 0;
				for (size_t i = 0; i < text_lines.size(); ++i)
					if (lines_x_groups[i][j])
						lines_counter++;

				// if lines_counter > 1 then group is overloaded - 1 group 1 text line
				if (lines_counter > 1)
					ar_is_group_open[j] = false;
			}

			std::vector<text_line_group_ptr_t> groups_add_later;

			for (size_t i = 0; i < text_lines.size(); ++i)
			{
				std::vector<size_t> groups_taken;
				for (size_t j = 0; j < text_line_groups.size(); ++j)
				{
					if (!ar_is_group_open[j])
						continue;

					if (lines_x_groups[i][j])
						groups_taken.push_back(j);
				}
				if (groups_taken.size() != 1)
				{
					for (const auto& taken : groups_taken)
						ar_is_group_open[taken] = false;

					auto group_new = std::make_shared<CTextLineGroup>();
					group_new->AddTextLine(text_lines[i]);
					groups_add_later.push_back(std::move(group_new));
					ar_is_group_open.push_back(true);
				}
				else
				{
					text_line_groups[groups_taken[0]]->AddTextLine(text_lines[i]);
				}
			}
			for (auto&& group : groups_add_later)
				text_line_groups.push_back(std::move(group));
		}
		return text_line_groups;
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

	std::vector<CPage::paragraph_ptr_t> CPage::BuildParagraphs(const std::vector<std::shared_ptr<CTextLineGroup>>& arTextLineGroups)
	{
		if (m_arTextLines.empty())
			return {};

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

			paragraph->m_dBot = paragraph->m_arTextLines.back()->m_dBot;
			paragraph->m_dTop = paragraph->m_arTextLines.front()->m_dTop;
			paragraph->m_dRight = max_right;
			paragraph->m_dLeft = min_left;

			paragraph->m_dWidth = paragraph->m_dRight - paragraph->m_dLeft;
			paragraph->m_dHeight = paragraph->m_dBot - paragraph->m_dTop;

			paragraph->m_dRightBorder = m_dWidth - paragraph->m_dRight;
			paragraph->m_dLeftBorder = min_left;

			paragraph->m_dLineHeight = paragraph->m_dHeight / paragraph->m_arTextLines.size();
			paragraph->m_bIsNeedFirstLineIndent = false;
			paragraph->m_dFirstLine = 0;
			paragraph->m_wsStyleId = m_oManagers.pParagraphStyleManager->GetDefaultParagraphStyleId(*paragraph);

			paragraph->MergeLines();

			// setting TextAlignmentType
			if (paragraph->m_arTextLines.size() > 1)
			{
				Position position_curr;
				position_curr.left   = true;
				position_curr.center = true;
				position_curr.right  = true;

				bool first_left = false;

				for (size_t index = 1; index < paragraph->m_arTextLines.size(); ++index)
				{
					auto& curr_line = paragraph->m_arTextLines[index];
					auto& prev_line = paragraph->m_arTextLines[index - 1];

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
					double left_diff = paragraph->m_arTextLines[0]->m_dLeft - paragraph->m_arTextLines[1]->m_dLeft;
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
		auto add_line = [&min_left, &max_right] (paragraph_ptr_t& paragraph, const text_line_ptr_t& curr_line) {
			min_left = std::min(min_left, curr_line->m_dLeft);
			max_right = std::max(max_right, curr_line->m_dRight);
			paragraph->m_arTextLines.push_back(curr_line);
		};

		auto build_paragraphs = [this, add_line, add_paragraph] (const std::vector<text_line_ptr_t>& text_lines) {
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
				ar_spacings[index] = text_lines[index + 1]->m_dTop - text_lines[index]->m_dBot;
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
			for (auto& g : arTextLineGroups)
				build_paragraphs(g->m_arTextLines);
		}

		std::sort(ar_paragraphs.begin(), ar_paragraphs.end(), [] (const paragraph_ptr_t& a, const paragraph_ptr_t& b) {
			return a->m_dBot < b->m_dBot;
		});

		return ar_paragraphs;
	}

	std::vector<std::vector<CTable::cell_ptr_t>> CPage::BuildCellGroups(const std::vector<CTable::cell_ptr_t>& arCells)
	{
		std::vector<std::vector<CTable::cell_ptr_t>> cell_groups;
		std::vector<CTable::cell_ptr_t> curr_group = {arCells[0]};

		double curr_bot = arCells[0]->m_dBot;
		for (size_t i = 1; i < arCells.size(); ++i)
		{
			const auto& cell = arCells[i];
			if (cell->m_dTop - curr_bot < c_dMAX_TABLE_LINE_WIDTH)
				curr_group.push_back(cell);
			else
			{
				cell_groups.push_back(std::move(curr_group));
				curr_group.push_back(cell);
			}
			curr_bot = cell->m_dBot;
		}
		cell_groups.push_back(curr_group);
		return cell_groups;
	}

	std::vector<CPage::table_ptr_t> CPage::BuildTables(const std::vector<std::shared_ptr<CTextLineGroup>>& arTextLineGroups)
	{
		auto graphical_cells = BuildGraphicalCells();
		auto text_cells = BuildTextCells(arTextLineGroups);

		std::vector<CTable::cell_ptr_t> cells;
		for (const auto& text_cell : text_cells)
		{
			auto cell_new = std::make_shared<CTable::CCell>();
			cell_new->RecalcWithNewItem(text_cell.get());
			cells.push_back(std::move(cell_new));
		}

		if (cells.empty())
			return {};

		auto cell_groups = BuildCellGroups(cells);

		auto add_if_no_exists = [] (double val, std::vector<double>& grid) {
			bool exists = false;
			for (const auto& curr : grid)
			{
				if (fabs(curr - val) < c_dMAX_TABLE_LINE_WIDTH)
				{
					exists = true;
					break;
				}
			}
			if (!exists)
				grid.push_back(val);
		};

		auto split_cell_vert = [] (const CTable::cell_ptr_t cell, double y) -> CTable::cell_ptr_t {
			CTable::cell_ptr_t cell_new = std::make_shared<CTable::CCell>();
			*cell_new = *cell;

			cell_new->m_dTop = y;
			cell->m_dBot = y;

			return cell_new;
		};

		// building a x and y grids
		for (const auto& group : cell_groups)
		{
			std::vector<double> grid_x;
			std::vector<double> grid_y;

			for (const auto& cell : group)
			{
				add_if_no_exists(cell->m_dLeft, grid_x);
				add_if_no_exists(cell->m_dTop, grid_y);
			}
			add_if_no_exists(group.back()->m_dRight, grid_x);
			add_if_no_exists(group.back()->m_dBot, grid_y);

			std::sort(grid_x.begin(), grid_x.end(), std::less<double>{});
			std::sort(grid_y.begin(), grid_y.end(), std::less<double>{});

			// GridSpan calc
			for (auto& cell : group)
			{
				size_t first = 0;
				size_t second = grid_x.size();
				for (size_t i = 0; i < grid_x.size(); ++i)
				{
					if (fabs(grid_x[i] - cell->m_dLeft) < c_dMAX_TABLE_LINE_WIDTH)
						first = i;
					else if (fabs(grid_x[i] - cell->m_dRight) < c_dMAX_TABLE_LINE_WIDTH)
						second = i;
				}
				cell->m_nGridSpan = second - first;
			}

			// merge vert calc
			for (auto& cell : group)
			{
				size_t first = 0;
				size_t second = grid_y.size();
				for (size_t i = 0; i < grid_y.size(); ++i)
				{
					if (fabs(grid_y[i] - cell->m_dTop) < c_dMAX_TABLE_LINE_WIDTH)
						first = i;
					else if (fabs(grid_y[i] - cell->m_dBot) < c_dMAX_TABLE_LINE_WIDTH)
						second = i;
				}
				while (++first < second)
				{
					auto cell_new = split_cell_vert(cell, grid_y[first]);
					cell_new->m_eVMerge = CTable::CCell::eVMerge::vmContinue;
					cells.push_back(cell_new);
				}
			}
		}

		std::sort(cells.begin(), cells.end(), [] (const CTable::cell_ptr_t& c1, const CTable::cell_ptr_t& c2) {
			if (fabs(c1->m_dBot - c2->m_dBot) < c_dMAX_TABLE_LINE_WIDTH)
				return c1->m_dLeft < c2->m_dLeft;
			return c1->m_dBot < c2->m_dBot;
		});

		// sets paragraphs into cells
		for (auto& cell : cells)
			for (auto& paragraph : m_arParagraphs)
			{
				if (!paragraph)
					continue;

				bool top = fabs(cell->m_dTop - paragraph->m_arTextLines.front()->m_dTopWithMaxAscent) < c_dGRAPHICS_ERROR_MM
				        || paragraph->m_arTextLines.front()->m_dTopWithMaxAscent > cell->m_dTop;
				bool bot = fabs(cell->m_dBot - paragraph->m_dBot) < c_dGRAPHICS_ERROR_MM
				        || paragraph->m_dBot < cell->m_dBot;
				bool left = fabs(cell->m_dLeft - paragraph->m_dLeft) < c_dGRAPHICS_ERROR_MM
				        || paragraph->m_dLeft > cell->m_dLeft;
				bool right = fabs(cell->m_dRight - paragraph->m_dRight) < c_dGRAPHICS_ERROR_MM
				        || paragraph->m_dRight < cell->m_dRight;
				if (top && bot && left && right)
				{
					paragraph->m_dLeftBorder = paragraph->m_dLeft - cell->m_dLeft;
					paragraph->m_dRightBorder = 0;
					if (!cell->m_arParagraphs.empty())
						paragraph->m_dSpaceBefore = paragraph->m_dTop - cell->m_arParagraphs.back()->m_dBot;
					else if (paragraph->m_dTop - cell->m_dTop > 0)
						paragraph->m_dSpaceBefore = paragraph->m_dTop - cell->m_dTop;
					else
						paragraph->m_dSpaceBefore = 0;
					paragraph->m_dSpaceAfter = 0;
					cell->AddParagraph(paragraph);
					paragraph = nullptr;
				}
			}

		auto right = MoveNullptr(m_arParagraphs.begin(), m_arParagraphs.end());
		m_arParagraphs.erase(right, m_arParagraphs.end());

		std::sort(m_arParagraphs.begin(), m_arParagraphs.end(), [] (const paragraph_ptr_t& p1, const paragraph_ptr_t& p2) {
			return p1->m_dBot < p2->m_dBot;
		});

		std::vector<CTable::row_ptr_t> rows;
		CTable::row_ptr_t curr_row = nullptr;
		for (auto& cell : cells)
		{
			if (!curr_row)
			{
				curr_row = std::make_shared<CTable::CRow>();
			}
			else if (fabs(curr_row->m_dBot - cell->m_dBot) > c_dMAX_TABLE_LINE_WIDTH)
			{
				rows.push_back(std::move(curr_row));
				curr_row = std::make_shared<CTable::CRow>();
			}
			curr_row->AddCell(cell);
		}
		if (!curr_row->IsEmpty())
			rows.push_back(std::move(curr_row));

		std::vector<table_ptr_t> tables;
		table_ptr_t curr_table = nullptr;
		for (const auto& row : rows)
		{
			if (!curr_table)
			{
				curr_table = std::make_shared<CTable>();
			}
			else if (fabs(curr_table->m_dBot - row->m_dTop) > c_dMAX_TABLE_LINE_WIDTH)
			{
				tables.push_back(std::move(curr_table));
				curr_table = std::make_shared<CTable>();
			}
			curr_table->AddRow(row);
		}
		if (!curr_table->IsEmpty())
			tables.push_back(std::move(curr_table));


		for (auto& t : tables)
			t->CalcGridCols();

		return tables;
	}

	std::vector<CTable::graphical_cell_ptr_t> CPage::BuildGraphicalCells()
	{
		struct Crossing;
		struct Line;

		enum class eLineDirection
		{
			ldTop,
			ldBot,
			ldLeft,
			ldRight
		};

		struct Line
		{
			Crossing* crossing;
			size_t shape_index;
			eLineDirection direction;
		};

		// crossing is a logical intersection between two lines
		// contains the lines from this crossing
		struct Crossing
		{
			Point p {};
			std::vector<Line> lines {};

			Crossing() = default;
			Crossing(const Point& _p, const std::vector<Line> _lines)
			{
				p = _p;
				lines = _lines;
			}
		};

		// returns index of the line with a direction
		auto get_line = [] (const eLineDirection& direction, const std::vector<Line>& lines) -> const Line* {
			for (size_t i = 0; i < lines.size(); ++i)
				if (lines[i].direction == direction)
					return &lines[i];
			return nullptr;
		};

		// goes into direction through crossings (like a graph).
		// add used shapes into out_indexes to remove it later
		auto go_direction_until = [&get_line] (Crossing* crossing,
		        const Point& p,
		        const eLineDirection& direction,
		        std::set<size_t>& out_indexes) -> bool {

			// difference depends on direction
			auto diff = [&crossing, &p, &direction] () -> double {
				if (direction == eLineDirection::ldRight) return p.x - crossing->p.x;
				if (direction == eLineDirection::ldLeft) return crossing->p.x - p.x;
				if (direction == eLineDirection::ldBot) return p.y - crossing->p.y;
				if (direction == eLineDirection::ldTop) return crossing->p.y - p.y;
			};

			while (diff() > c_dMAX_TABLE_LINE_WIDTH)
			{
				auto curr_line = get_line(direction, crossing->lines);
				if (!curr_line)
					return false;

				out_indexes.insert(curr_line->shape_index);
				crossing = curr_line->crossing;

				if (fabs(diff()) < c_dMAX_TABLE_LINE_WIDTH)
					return true;
			}
			return false;
		};

		// vector contains ptrs for easy exist-check
		std::vector<std::shared_ptr<Crossing>> crossings;
		auto find_crossing = [&crossings] (const Point& p) -> Crossing* {
			for (auto& crossing : crossings)
			{
				if (!crossing)
					continue;

				if (fabs(crossing->p.x - p.x) < c_dMAX_TABLE_LINE_WIDTH &&
				        fabs(crossing->p.y - p.y) < c_dMAX_TABLE_LINE_WIDTH)
					return crossing.get();
			}
			return nullptr;
		};

		auto precise_crossing_p = [] (Crossing* cr, const Point& p, const eLineDirection& direction) {
			if (cr->lines.size() == 1)
			{
				switch (cr->lines[0].direction)
				{
				case eLineDirection::ldLeft:
				case eLineDirection::ldRight:
					if (direction == eLineDirection::ldLeft || direction == eLineDirection::ldRight)
						cr->p.x = (cr->p.x + p.x) / 2;
					else
						cr->p.x = p.x;
					break;
				case eLineDirection::ldBot:
				case eLineDirection::ldTop:
					if (direction == eLineDirection::ldBot || direction == eLineDirection::ldTop)
						cr->p.y = (cr->p.y + p.y) / 2;
					else
						cr->p.y = p.y;
					break;
				default:
					break;
				}
			}
		};

		// check and adds points
		auto add_crossings = [&precise_crossing_p, &crossings, &find_crossing, this] (const Point& p1, const Point& p2, size_t index) {
			Crossing* crossing1 = find_crossing(p1);
			Crossing* crossing2 = find_crossing(p2);

			eLineDirection direction12;
			eLineDirection direction21;

			if (fabs(p1.y - p2.y) < c_dMAX_TABLE_LINE_WIDTH)
			{
				direction12 = p1.x > p2.x ? eLineDirection::ldLeft : eLineDirection::ldRight;
				direction21 = p1.x < p2.x ? eLineDirection::ldLeft : eLineDirection::ldRight;
			}

			else if (fabs(p1.x - p2.x) < c_dMAX_TABLE_LINE_WIDTH)
			{
				direction12 = p1.y > p2.y ? eLineDirection::ldTop : eLineDirection::ldBot;
				direction21 = p1.y < p2.y ? eLineDirection::ldTop : eLineDirection::ldBot;
			}

			// add empty crossing if no exists
			if (!crossing1)
			{
				Crossing cr;
				cr.p = p1;
				crossings.push_back(std::make_shared<Crossing>(cr));
				crossing1 = crossings.back().get();
			}
			if (!crossing2)
			{
				Crossing cr;
				cr.p = p2;
				crossings.push_back(std::make_shared<Crossing>(cr));
				crossing2 = crossings.back().get();
			}

			auto curr_crossing_eq = [&crossing1] (const Line& line) -> bool {
				return line.crossing == crossing1;
			};
			if (std::find_if(crossing2->lines.begin(), crossing2->lines.end(), curr_crossing_eq) == crossing2->lines.end())
			{
				precise_crossing_p(crossing2, p2, direction21);
				Line line {crossing1, index, direction21};
				crossing2->lines.push_back(std::move(line));
			}

			auto prev_crossing_eq = [&crossing2] (const Line& line) -> bool {
				return line.crossing == crossing2;
			};
			if (std::find_if(crossing1->lines.begin(), crossing1->lines.end(), prev_crossing_eq) == crossing1->lines.end())
			{
				precise_crossing_p(crossing1, p1, direction12);
				Line line {crossing2, index, direction12};
				crossing1->lines.push_back(std::move(line));
			}

		};

		// 2 main cases
		// 1. lines of tables as a big rectangles with lines (onlyoffice), work with path commands
		// 2. lines of tables as a small rectangels for a single line (adobe), work with entire shape
		// also word -> pdf adobe sets points as crossings of the table lines, so we can use it

		// check for adobe points
		std::vector<std::pair<const Crossing*, size_t>> points_delete_later;
		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& shape = m_arShapes[i];
			if (!shape || shape->m_eGraphicsType != eGraphicsType::gtRectangle || shape->m_pImageInfo != nullptr)
				continue;

			// possible point of crossing in a adobe table
			if (shape->m_dWidth < c_dMAX_TABLE_LINE_WIDTH && shape->m_dHeight < c_dMAX_TABLE_LINE_WIDTH)
			{
				Crossing cr;
				cr.p.x = shape->m_dLeft + shape->m_dWidth / 2;
				cr.p.y = shape->m_dTop + shape->m_dHeight;
				auto cr_ptr = std::make_shared<Crossing>(cr);
				crossings.push_back(cr_ptr);
				points_delete_later.push_back({cr_ptr.get(), i});
			}
		}

		for (size_t i = 0; i < m_arShapes.size(); ++i)
		{
			auto& shape = m_arShapes[i];
			if (!shape || shape->m_eGraphicsType != eGraphicsType::gtRectangle || shape->m_pImageInfo != nullptr)
				continue;

			bool is_done = false;
			if (shape->m_eSimpleLineType == eSimpleLineType::sltUnknown)
			{
				// possible point of crossing in a adobe table
				if (shape->m_dWidth < c_dMAX_TABLE_LINE_WIDTH && shape->m_dHeight < c_dMAX_TABLE_LINE_WIDTH)
				{
					is_done = true;
				}

				// vertical line
				else if (shape->m_dWidth < c_dMAX_TABLE_LINE_WIDTH && shape->m_dHeight > c_dMAX_TABLE_LINE_WIDTH)
				{
					Point p1(shape->m_dLeft + shape->m_dWidth / 2, shape->m_dTop);
					Point p2(shape->m_dLeft + shape->m_dWidth / 2, shape->m_dBot);
					add_crossings(p1, p2, i);
					is_done = true;
				}

				// horizontal line
				else if (shape->m_dWidth > c_dMAX_TABLE_LINE_WIDTH && shape->m_dHeight < c_dMAX_TABLE_LINE_WIDTH)
				{
					Point p1(shape->m_dLeft, shape->m_dTop + shape->m_dHeight / 2);
					Point p2(shape->m_dRight, shape->m_dTop + shape->m_dHeight / 2);
					add_crossings(p1, p2, i);
					is_done = true;
				}
			}

			if (is_done)
				continue;

			Point prev {};
			for (const auto& command : shape->m_oVector.GetData())
			{
				if (!command.points.empty())
				{
					if (command.type == CVectorGraphics::ePathCommandType::pctLine)
					{
						const auto& curr = command.points.back();

						// pure vertical / horizontal lines only
						// not small lines
						if (fabs(prev.x - curr.x) > c_dMAX_TABLE_LINE_WIDTH && fabs(prev.y - curr.y) > c_dMAX_TABLE_LINE_WIDTH ||
						        (fabs(prev.x - curr.x) < c_dMAX_TABLE_LINE_WIDTH && fabs(prev.y - curr.y) < c_dMAX_TABLE_LINE_WIDTH))
						{
							prev = curr;
							continue;
						}
						add_crossings(prev, curr, i);
					}
					prev = command.points.back();
				}
			}
		}

		// remove adobe points line crossing shapes if used
		for (auto& cr : points_delete_later)
			if (!cr.first->lines.empty())
				m_arShapes[cr.second] = nullptr;

		// remove empty crossings
		for (auto& cr : crossings)
			if (cr && cr->lines.empty())
				cr = nullptr;

		auto cr_right = MoveNullptr(crossings.begin(), crossings.end());
		crossings.erase(cr_right, crossings.end());

		// sorting guarantee creating cell once (by taking second cross j > i)
		std::sort(crossings.begin(), crossings.end(), [] (const std::shared_ptr<Crossing>& c1, const std::shared_ptr<Crossing>& c2) {
			if (fabs(c1->p.y - c2->p.y) < c_dMAX_TABLE_LINE_WIDTH)
				return c1->p.x < c2->p.x;
			return c1->p.y < c2->p.y;
		});

		std::vector<CTable::graphical_cell_ptr_t> graphical_cells;
		std::map<size_t, bool> remove_later;
		for (size_t i = 0; i < crossings.size(); ++i)
		{
			for (size_t j = i + 1; j < crossings.size(); ++j)
			{
				const auto& cr_first = crossings.at(i);
				const auto& cr_second = crossings.at(j);

				// first should be top left corner
				// and second right bot (not the same line ofc)
				if (fabs(cr_first->p.x - cr_second->p.x) < c_dMAX_TABLE_LINE_WIDTH ||
				        cr_first->p.x > cr_second->p.x ||
				        fabs(cr_first->p.y - cr_second->p.y) < c_dMAX_TABLE_LINE_WIDTH ||
				        cr_first->p.y > cr_second->p.y)
					continue;

				const Line* cr_f_top = get_line(eLineDirection::ldRight, cr_first->lines);
				const Line* cr_f_left = get_line(eLineDirection::ldBot, cr_first->lines);
				const Line* cr_s_bot = get_line(eLineDirection::ldLeft, cr_second->lines);
				const Line* cr_s_right = get_line(eLineDirection::ldTop, cr_second->lines);

				if (!cr_f_top || !cr_f_left || !cr_s_bot ||  !cr_s_right)
					continue;

				std::set<size_t> shape_indexes;
				bool is_connected = go_direction_until(cr_first.get(), cr_second->p, eLineDirection::ldRight, shape_indexes);
				is_connected &= go_direction_until(cr_first.get(), cr_second->p, eLineDirection::ldBot, shape_indexes);
				is_connected &= go_direction_until(cr_second.get(), cr_first->p, eLineDirection::ldLeft, shape_indexes);
				is_connected &= go_direction_until(cr_second.get(), cr_first->p, eLineDirection::ldTop, shape_indexes);

				if (!is_connected)
					continue;

				for (const auto& index : shape_indexes)
					remove_later[index] = true;

				auto graphical_cell = std::make_shared<CGraphicalCell>();
				graphical_cell->m_dLeft = cr_first->p.x;
				graphical_cell->m_dRight = cr_second->p.x;
				graphical_cell->m_dTop = cr_first->p.y;
				graphical_cell->m_dBot = cr_second->p.y;
				graphical_cell->m_dWidth = graphical_cell->m_dRight - graphical_cell->m_dLeft;
				graphical_cell->m_dHeight = graphical_cell->m_dBot - graphical_cell->m_dTop;
				graphical_cells.push_back(std::move(graphical_cell));
				break;
			}
		}

		return graphical_cells;
	}

	std::vector<CTable::text_cell_ptr_t> CPage::BuildTextCells(const std::vector<text_line_group_ptr_t>& arTextLineGroups)
	{
		std::vector<CTable::text_cell_ptr_t> text_cells;

		// -> <-
		for (const auto& text_line_group : arTextLineGroups)
		{
			auto text_cell_new = std::make_shared<CTextCell>();
			text_cell_new->RecalcWithNewItem(text_line_group.get());
			text_cells.push_back(std::move(text_cell_new));
		}

		// calc max gaps between groups vertically
		for (size_t i = 0; i < text_cells.size(); ++i)
		{
			for (size_t j = 0; j < text_cells.size(); ++j)
			{
				if (i == j)
					continue;

				if (!(text_cells[j]->m_dLeft > text_cells[i]->m_dRight &&
				      text_cells[j]->m_dRight < text_cells[i]->m_dLeft))
					text_cells[i]->m_dMaxPossibleBot = std::min(text_cells[i]->m_dMaxPossibleBot, text_cells[j]->m_dTop);
			}
		}
		return text_cells;
	}

	CPage::shape_ptr_t CPage::CreateSingleLineShape(text_line_ptr_t& pLine)
	{
		auto pParagraph = std::make_shared<CParagraph>();

		pParagraph->m_arTextLines.push_back(pLine);
		pParagraph->m_dLeft = pLine->m_dLeft;
		pParagraph->m_dTop = pLine->m_dTop;
		pParagraph->m_dBot = pLine->m_dBot;
		pParagraph->m_dWidth = pLine->m_dWidth;
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
		pShape->m_dBot = pParagraph->m_dBot;
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
		pShape->m_dBot = pParagraph->m_dBot;
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
}
