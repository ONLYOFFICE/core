#include "graphics.h"
#include "../common/Base64.h"

#include <string>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace NSGraphics
{
	void CGraphics::init(NSNativeControl::CNativeControl* oNative, double width_px, double height_px, double width_mm, double height_mm)
	{
		m_sApplicationImagesDirectory = oNative->m_strImagesDirectory;
		m_sApplicationFontsDirectory  = oNative->m_strFontsDirectory;
#ifdef _DEBUG
		std::wcout << L"init "<< m_sApplicationImagesDirectory << L"  " << m_sApplicationFontsDirectory << L"  " << width_px << L"  " << height_px << L"  " << width_mm << L"  " << height_mm << std::endl;
#endif
		m_pApplicationFonts = NSFonts::NSApplication::Create();
		m_pApplicationFonts->InitializeFromFolder(m_sApplicationFontsDirectory.empty() ? NSFile::GetProcessDirectory() : m_sApplicationFontsDirectory);
		NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();

		m_pRenderer = NSGraphics::Create();
		m_pRenderer->SetFontManager(pManager);

		int nRasterW = (int)width_px;
		int nRasterH = (int)height_px;

		// TODO:
		int nLimit = 2000;
		int nMax = (nRasterW > nRasterH) ? nRasterW : nRasterH;
		if (nMax > nLimit)
		{
			double dKoef = (double)nLimit / nMax;
			nRasterW = (int)(0.5 + dKoef * nRasterW);
			nRasterH = (int)(0.5 + dKoef * nRasterH);

			if (nRasterW < 1) nRasterW = 0;
			if (nRasterH < 1) nRasterH = 0;
		}

		BYTE* pData = new BYTE[4 * nRasterW * nRasterH];
		unsigned int back = 0xffffff;
		unsigned int* pData32 = (unsigned int*)pData;
		unsigned int* pData32End = pData32 + nRasterW * nRasterH;
		while (pData32 < pData32End)
			*pData32++ = back;

		m_oFrame.put_Data(pData);
		m_oFrame.put_Width(nRasterW);
		m_oFrame.put_Height(nRasterH);
		m_oFrame.put_Stride(4 * nRasterW);

		m_pRenderer->CreateFromBgraFrame(&m_oFrame);
		m_pRenderer->SetSwapRGB(false);

		m_pRenderer->put_Width(width_mm);
		m_pRenderer->put_Height(height_mm);
	}
	void CGraphics::put_GlobalAlpha(bool enable, double alpha)
	{
#ifdef _DEBUG
		std::cout << "put_GlobalAlpha " << enable << "  " << alpha << std::endl;
#endif
		m_pRenderer->put_GlobalAlphaEnabled(enable, alpha);
	}
	void CGraphics::End_GlobalAlpha()
	{
#ifdef _DEBUG
		std::cout << "End_GlobalAlpha " << std::endl;
#endif
		bool bIsInteger = m_pRenderer->get_IntegerGrid();
		m_pRenderer->put_IntegerGrid(true);

		m_pRenderer->PathCommandEnd();
		b_color1(255, 255, 255, 140);

		m_pRenderer->AddRect(0.0, 0.0, m_pRenderer->GetPixW(), m_pRenderer->GetPixH());
		m_pRenderer->Fill();
		m_pRenderer->PathCommandEnd();

		m_pRenderer->put_IntegerGrid(bIsInteger);
	}
	void CGraphics::p_color(int r, int g, int b, int a)
	{
#ifdef _DEBUG
		std::cout << "p_color " << r << "  " << g << "  " << b << "  " << a << std::endl;
#endif
		m_pRenderer->put_PenColor(r | (g << 8) | (b << 16));
		m_pRenderer->put_PenAlpha(a);
	}
	void CGraphics::p_width(double w)
	{
#ifdef _DEBUG
		std::cout << "p_width " << w  << std::endl;
#endif
		m_pRenderer->put_PenSize(w / 1000.0);
	}
	void CGraphics::p_dash(size_t length, double* dash)
	{
#ifdef _DEBUG
		std::cout << "p_dash " << length << std::endl;
#endif
		if(length > 0)
		{
			double dDpiX = 0;
			m_pRenderer->get_DpiX(&dDpiX);
			for(size_t i = 0; i < length; i++)
				dash[i] *= (dDpiX / 25.4);

			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			m_pRenderer->PenDashPattern(dash, length);
		}
		else
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
	}
	void CGraphics::b_color1(int r, int g, int b, int a)
	{
#ifdef _DEBUG
		std::cout << "b_color1 " << r << "  " << g << "  " << b << "  " << a << std::endl;
#endif
		m_pRenderer->put_BrushType(c_BrushTypeSolid);
		m_pRenderer->put_BrushColor1(r | (g << 8) | (b << 16));
		m_pRenderer->put_BrushAlpha1(a);
	}
	void CGraphics::b_color2(int r, int g, int b, int a)
	{
#ifdef _DEBUG
		std::cout << "b_color2 " << r << "  " << g << "  " << b << "  " << a << std::endl;
#endif
		m_pRenderer->put_BrushColor2(r | (g << 8) | (b << 16));
		m_pRenderer->put_BrushAlpha2(a);
	}
	void CGraphics::transform(double sx, double shy, double shx, double sy, double tx, double ty)
	{
#ifdef _DEBUG
		std::cout << "transform " << sx << "  " << shy << "  " << shx << "  " << sy << "  " << tx << "  " << ty << std::endl;
#endif
		m_pRenderer->SetTransform(sx, shy, shx, sy, tx, ty);
	}
	void CGraphics::CalculateFullTransform()
	{
#ifdef _DEBUG
		std::cout << "CalculateFullTransform " << std::endl;
#endif
		m_pRenderer->CalculateFullTransform();
	}
	void CGraphics::_s()
	{
#ifdef _DEBUG
		std::cout << "_s " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();
	}
	void CGraphics::_e()
	{
#ifdef _DEBUG
		std::cout << "_e " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();
	}
	void CGraphics::_z()
	{
#ifdef _DEBUG
		std::cout << "_z " << std::endl;
#endif
		m_pRenderer->PathCommandClose();
	}
	void CGraphics::_m(double x, double y)
	{
#ifdef _DEBUG
		std::cout << "_m " << x << "  " << y << std::endl;
#endif
		if (!m_pRenderer->get_IntegerGrid())
			m_pRenderer->PathCommandMoveTo(x, y);
		else
		{
			m_pRenderer->GetFullTransform()->TransformPoint(x, y);
			m_pRenderer->PathCommandMoveTo((int)x + 0.5, (int)y + 0.5);
		}
	}
	void CGraphics::_l(double x, double y)
	{
#ifdef _DEBUG
		std::cout << "_l " << x << "  " << y << std::endl;
#endif
		if (!m_pRenderer->get_IntegerGrid())
			m_pRenderer->PathCommandLineTo(x, y);
		else
		{
			m_pRenderer->GetFullTransform()->TransformPoint(x, y);
			m_pRenderer->PathCommandLineTo((int)x + 0.5, (int)y + 0.5);
		}
	}
	void CGraphics::_c (double x1, double y1, double x2, double y2, double x3, double y3)
	{
#ifdef _DEBUG
		std::cout << "_c " << x1 << "  " << y1 << "  " << x2 << "  " << y2 << "  " << x3 << "  " << y3 << std::endl;
#endif
		if (!m_pRenderer->get_IntegerGrid())
			m_pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
		else
		{
			m_pRenderer->GetFullTransform()->TransformPoint(x1, y1);
			m_pRenderer->GetFullTransform()->TransformPoint(x2, y2);
			m_pRenderer->GetFullTransform()->TransformPoint(x3, y3);
			m_pRenderer->PathCommandCurveTo((int)x1 + 0.5, (int)y1 + 0.5, (int)x2 + 0.5, (int)y2 + 0.5, (int)x3 + 0.5, (int)y3 + 0.5);
		}
	}
	void CGraphics::_c2(double x1, double y1, double x2, double y2)
	{
#ifdef _DEBUG
		std::cout << "_c2 " << x1 << "  " << y1 << "  " << x2 << "  " << y2 << std::endl;
#endif
		if (!m_pRenderer->get_IntegerGrid())
			m_pRenderer->PathCommandCurveTo(x1, y1, x1, y1, x2, y2);
		else
		{
			m_pRenderer->GetFullTransform()->TransformPoint(x1, y1);
			m_pRenderer->GetFullTransform()->TransformPoint(x2, y2);
			m_pRenderer->PathCommandCurveTo((int)x1 + 0.5, (int)y1 + 0.5, (int)x1 + 0.5, (int)y1 + 0.5, (int)x2 + 0.5, (int)y2 + 0.5);
		}
	}
	void CGraphics::ds()
	{
#ifdef _DEBUG
		std::cout << "ds " << std::endl;
#endif
		m_pRenderer->Stroke();
	}
	void CGraphics::df()
	{
#ifdef _DEBUG
		std::cout << "df " << std::endl;
#endif
		m_pRenderer->Fill();
	}
	void CGraphics::save()
	{
#ifdef _DEBUG
		std::cout << "save " << std::endl;
#endif
		m_oFrame.SaveFile(m_sApplicationImagesDirectory + L"/img.png", _CXIMAGE_FORMAT_PNG);
	}
	void CGraphics::restore()
	{
#ifdef _DEBUG
		std::cout << "restore " << std::endl;
#endif
		m_pRenderer->BeginCommand(c_nResetClipType);
		m_pRenderer->EndCommand  (c_nResetClipType);
	}
	void CGraphics::clip()
	{
#ifdef _DEBUG
		std::cout << "clip " << std::endl;
#endif
		m_pRenderer->BeginCommand(c_nClipType);
		m_pRenderer->EndCommand  (c_nClipType);
	}
	void CGraphics::reset()
	{
#ifdef _DEBUG
		std::cout << "reset " << std::endl;
#endif
		m_pRenderer->ResetTransform();
	}
	void CGraphics::FreeFont()
	{
#ifdef _DEBUG
		std::cout << "FreeFont " << std::endl;
#endif
		m_pRenderer->CloseFont();
	}
	void CGraphics::ClearLastFont()
	{
#ifdef _DEBUG
		std::cout << "ClearLastFont " << std::endl;
#endif
		m_pRenderer->ClearInstallFont();
	}
	void CGraphics::drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha)
	{
		std::wstring strImage = (0 == img.find(L"theme") ? m_sApplicationThemesDirectory : m_sApplicationImagesDirectory) + L'/' + img;
#ifdef _DEBUG
		std::wcout << L"drawImage " << strImage << L"  " << x << "  " << y << L"  " << w << L"  " << h << L"  " << alpha << std::endl;
#endif
		m_pRenderer->DrawImageFromFile(strImage, x, y, w, h, alpha);
	}
	std::wstring CGraphics::GetFont()
	{
#ifdef _DEBUG
		std::cout << "GetFont " << std::endl;
#endif
		return m_pRenderer->GetFontManager()->GetName();
	}
	void CGraphics::SetFont(const std::wstring& name, int face, double size, int style)
	{
#ifdef _DEBUG
		std::wcout << L"SetFont " << name << L"  " << face << L"  " << size << L"  " << style << std::endl;
#endif
		double DpiX, DpiY;
		m_pRenderer->get_DpiX(&DpiX);
		m_pRenderer->get_DpiY(&DpiY);
		m_pRenderer->GetFontManager()->LoadFontByName(name, size, style, DpiX, DpiY);

		m_pRenderer->put_FontName     (name);
		m_pRenderer->put_FontFaceIndex(face);
		m_pRenderer->put_FontSize     (size);
		m_pRenderer->put_FontStyle    (style);
	}
	void CGraphics::FillText(double x, double y, int text)
	{
#ifdef _DEBUG
		std::wcout << L"FillText " << (wchar_t)text << L"  " << x << L"  " << y << std::endl;
#endif
		m_pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
	}
	void CGraphics::t(double x, double y, const std::wstring& text)
	{
#ifdef _DEBUG
		std::wcout << L"t " << text << L"  " << x << L"  " << y << std::endl;
#endif
		m_pRenderer->CommandDrawText(text, x, y, 0, 0);
	}
	void CGraphics::tg(int text, double x, double y)
	{
#ifdef _DEBUG
		std::wcout << L"tg " << text << L"  " << x << L"  " << y << std::endl;
#endif
		m_pRenderer->put_FontStringGID(TRUE);
		m_pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
		m_pRenderer->put_FontStringGID(FALSE);
	}
	void CGraphics::SetIntegerGrid(bool param)
	{
#ifdef _DEBUG
		std::cout << "SetIntegerGrid " << param << std::endl;
#endif
		m_pRenderer->put_IntegerGrid(param);
	}
	bool CGraphics::GetIntegerGrid()
	{
#ifdef _DEBUG
		std::cout << "GetIntegerGrid " << std::endl;
#endif
		return m_pRenderer->get_IntegerGrid();
	}
	void CGraphics::DrawStringASCII (const std::wstring& text, double x, double y)
	{
#ifdef _DEBUG
		std::wcout << L"DrawStringASCII " << text << L"  " << x << L"  " << y << std::endl;
#endif
		double DpiY;
		m_pRenderer->get_DpiY(&DpiY);

		SavePenBrush();

		b_color1(225, 225, 225, 255);
		m_pRenderer->GetFontManager()->LoadString2(text, x, y);
		TBBox oBox = m_pRenderer->GetFontManager()->MeasureString2();
		rect(x, y, oBox.fMinX, oBox.fMinY);
		df();
		ds();

		b_color1(68, 68, 68, 255);
		t(x + 10.0 * 25.4 / DpiY, y - 5.0 * 25.4 / DpiY, text);

		RestorePenBrush();
	}
	void CGraphics::DrawHeaderEdit(double yPos)
	{
#ifdef _DEBUG
		std::cout << "DrawHeaderEdit " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();

		Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
		double dPenSize = 0;
		m_pRenderer->get_PenSize(&dPenSize);

		double _width;
		m_pRenderer->get_Width(&_width);
		pFull->TransformPoint(_width, yPos);

		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);

		bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(true);

		m_pRenderer->put_PenSize(2);
		m_pRenderer->PathCommandStart();

		double dash[2] = { 6, 3 };
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		m_pRenderer->PenDashPattern(dash, 2);

		m_pRenderer->put_PenColor(0xBBBEC2);

		m_pRenderer->PathCommandMoveTo(0,      (int)(yPos));
		m_pRenderer->PathCommandLineTo(_width, (int)(yPos));

		m_pRenderer->Stroke();
		m_pRenderer->PathCommandEnd();

		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(false);

		m_pRenderer->put_PenSize(dPenSize);
		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	void CGraphics::DrawFooterEdit(double yPos)
	{
#ifdef _DEBUG
		std::cout << "DrawFooterEdit " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();

		Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
		double dPenSize = 0;
		m_pRenderer->get_PenSize(&dPenSize);

		double _width;
		m_pRenderer->get_Width(&_width);
		pFull->TransformPoint(_width, yPos);

		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);

		bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(true);

		m_pRenderer->put_PenSize(2);
		m_pRenderer->PathCommandStart();

		double dash[2] = { 6, 3 };
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		m_pRenderer->PenDashPattern(dash, 2);

		m_pRenderer->put_PenColor(0xBBBEC2);

		m_pRenderer->PathCommandMoveTo(0, (int)(yPos));
		m_pRenderer->PathCommandLineTo(_width, (int)(yPos));

		m_pRenderer->Stroke();
		m_pRenderer->PathCommandEnd();

		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(false);

		m_pRenderer->put_PenSize(dPenSize);
		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	void CGraphics::DrawLockParagraph (double x,  double y1, double y2)
	{
#ifdef _DEBUG
		std::cout << "DrawLockParagraph " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();

		Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
		double dPenSize = 0.0;
		m_pRenderer->get_PenSize(&dPenSize);
		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);
		m_pRenderer->put_PenColor(0x009C16);

		double _x  = x;
		double _xT = x;
		double _y1 = y1;
		double _y2 = y2;
		pFull->TransformPoint(_x, _y1);
		pFull->TransformPoint(_xT, _y2);

		_x  = ((int)_x);
		_xT = ((int)_xT);
		_y1 = ((int)_y1) + 0.5;
		_y2 = ((int)_y2) - 1.5;

		m_pRenderer->put_PenSize(1);
		m_pRenderer->PathCommandStart();

		double dash[2] = { 2.0, 2.0 };
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		m_pRenderer->PenDashPattern(dash, 2);

		if(fabs(_x - _xT) > 0.001)
		{
			m_pRenderer->PathCommandMoveTo(x, y1);
			m_pRenderer->PathCommandLineTo(x, y2);

			m_pRenderer->PathCommandMoveTo(x,       y1);
			m_pRenderer->PathCommandLineTo(x + 3.0, y1);

			m_pRenderer->PathCommandMoveTo(x,       y2);
			m_pRenderer->PathCommandLineTo(x + 3.0, y2);

			m_pRenderer->Stroke();
			m_pRenderer->PathCommandEnd();
		}
		else
		{
			bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
			if (!bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(true);

			m_pRenderer->PathCommandMoveTo(_x + 0.5, _y1 - 0.5);
			m_pRenderer->PathCommandLineTo(_x + 0.5, _y2 - 2.0);

			m_pRenderer->PathCommandMoveTo(_x,       _y1);
			m_pRenderer->PathCommandLineTo(_x + 3.0, _y1);

			m_pRenderer->PathCommandMoveTo(_x,       _y2);
			m_pRenderer->PathCommandLineTo(_x + 3.0, _y2);

			m_pRenderer->Stroke();
			m_pRenderer->PathCommandEnd();

			if (!bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(false);
		}

		m_pRenderer->put_PenSize(dPenSize);
		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	void CGraphics::DrawLockObjectRect(double x,  double y,  double w,  double h)
	{
#ifdef _DEBUG
		std::cout << "DrawLockObjectRect " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();

		double dPenSize = 0.0;
		m_pRenderer->get_PenSize(&dPenSize);
		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);
		m_pRenderer->put_PenColor(0x009C16);

		m_pRenderer->put_PenSize(1);

		double dash[2] = { 2.0, 2.0 };
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		m_pRenderer->PenDashPattern(dash, 2);

		double eps = 5.0;
		rect(x - eps, y - eps, w + eps, h + eps);

		m_pRenderer->Stroke();
		m_pRenderer->put_PenSize(dPenSize);
		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	void CGraphics::DrawEmptyTableLine(double x1, double y1, double x2, double y2)
	{
#ifdef _DEBUG
		std::cout << "DrawEmptyTableLine " << std::endl;
#endif
		m_pRenderer->PathCommandEnd();

		Aggplus::CMatrix* pFull = m_pRenderer->GetFullTransform();
		double dPenSize = 0;
		m_pRenderer->get_PenSize(&dPenSize);

		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);

		double _x1 = x1;
		double _y1 = y1;
		double _x2 = x2;
		double _y2 = y2;
		pFull->TransformPoint(_x1, _y1);
		pFull->TransformPoint(_x2, _y2);

		if (fabs(_x1 - _x2) < 0.001 || fabs(_y1 - _y2) < 0.001)
		{
			bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
			if (!bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(true);

			m_pRenderer->put_PenSize(1);
			m_pRenderer->PathCommandStart();

			double dash[2] = { 2, 2 };
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			m_pRenderer->PenDashPattern(dash, 2);

			m_pRenderer->put_PenColor(0xBFA28A);

			if (fabs(_x1 - _x2) < 0.001)
			{
				double _dx = ((int)_x1) + 0.5;
				double _dy1 = ((int)_y1);
				double _dy2 = ((int)_y2);
				m_pRenderer->PathCommandMoveTo(_dx, _dy1);
				m_pRenderer->PathCommandLineTo(_dx, _dy2);
			}
			else
			{
				double _dy = ((int)_y1) + 0.5;
				double _dx1 = ((int)_x1);
				double _dx2 = ((int)_x2);
				m_pRenderer->PathCommandMoveTo(_dx1, _dy);
				m_pRenderer->PathCommandLineTo(_dx2, _dy);
			}

			m_pRenderer->Stroke();
			m_pRenderer->PathCommandEnd();

			if (!bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(false);
		}
		else
		{
			bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
			if (bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(false);

			m_pRenderer->put_PenSize(0);
			m_pRenderer->PathCommandStart();

			double dash[2] = { 2, 2 };
			m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			m_pRenderer->PenDashPattern(dash, 2);

			m_pRenderer->put_PenColor(0xBFA28A);
			m_pRenderer->PathCommandMoveTo(x1, y1);
			m_pRenderer->PathCommandLineTo(x2, y2);
			m_pRenderer->Stroke();
			m_pRenderer->PathCommandEnd();

			if (bIsIntegerGrid)
				m_pRenderer->put_IntegerGrid(true);
		}

		m_pRenderer->put_PenSize(dPenSize);
		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	void CGraphics::DrawSpellingLine  (double y0, double x0, double x1, double w)
	{
#ifdef _DEBUG
		std::cout << "DrawSpellingLine " << std::endl;
#endif
		Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
		if (!m_pRenderer->get_IntegerGrid())
		{
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->put_IntegerGrid(true);
				m_pRenderer->drawHorLine(1, y0, x0, x1, w);
				m_pRenderer->put_IntegerGrid(false);
			}
			else
			{
				m_pRenderer->put_PenSize(w);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x0, y0);
				m_pRenderer->PathCommandLineTo(x1, y0);
				m_pRenderer->Stroke();
			}
		}
		else
		{
			if(pMatrix->IsIdentity2())
			{
				m_pRenderer->drawHorLine(1, y0, x0, x1, w);
			}
			else
			{
				m_pRenderer->put_IntegerGrid(false);
				m_pRenderer->put_PenSize(w);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x0, y0);
				m_pRenderer->PathCommandLineTo(x1, y0);
				m_pRenderer->Stroke();
				m_pRenderer->put_IntegerGrid(true);
			}

		}
	}
	void CGraphics::drawHorLine   (BYTE align, double y, double x, double r, double penW)
	{
		Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
		if (!m_pRenderer->get_IntegerGrid())
		{
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->put_IntegerGrid(true);
				m_pRenderer->drawHorLine(align, y, x, r, penW);
				m_pRenderer->put_IntegerGrid(false);
			}
			else
			{
				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, y);
				m_pRenderer->PathCommandLineTo(r, y);
				m_pRenderer->Stroke();
			}
		}
		else
		{
			if(pMatrix->IsIdentity2())
			{
				m_pRenderer->drawHorLine(align, y, x, r, penW);
			}
			else
			{
				m_pRenderer->put_IntegerGrid(false);
				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, y);
				m_pRenderer->PathCommandLineTo(r, y);
				m_pRenderer->Stroke();
				m_pRenderer->put_IntegerGrid(true);
			}

		}
	}
	void CGraphics::drawHorLine2  (BYTE align, double y, double x, double r, double penW)
	{
		Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
		if (!m_pRenderer->get_IntegerGrid())
		{
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->put_IntegerGrid(true);
				m_pRenderer->drawHorLine2(align, y, x, r, penW);
				m_pRenderer->put_IntegerGrid(false);
			}
			else
			{
				double _y1 = y - penW / 2;
				double _y2 = _y1 + 2 * penW;

				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, _y1);
				m_pRenderer->PathCommandLineTo(r, _y1);
				m_pRenderer->Stroke();

				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, _y2);
				m_pRenderer->PathCommandLineTo(r, _y2);
				m_pRenderer->Stroke();
			}
		}
		else
		{
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->drawHorLine2(align, y, x, r, penW);
			}
			else
			{
				m_pRenderer->put_IntegerGrid(false);
				double _y1 = y - penW / 2;
				double _y2 = _y1 + 2 * penW;

				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, _y1);
				m_pRenderer->PathCommandLineTo(r, _y1);
				m_pRenderer->Stroke();

				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, _y2);
				m_pRenderer->PathCommandLineTo(r, _y2);
				m_pRenderer->Stroke();
				m_pRenderer->put_IntegerGrid(true);
			}
		}
	}
	void CGraphics::drawVerLine   (BYTE align, double x, double y, double b, double penW)
	{
		if (!m_pRenderer->get_IntegerGrid())
		{
			Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->put_IntegerGrid(true);
				m_pRenderer->drawVerLine(align, x, y, b, penW);
				m_pRenderer->put_IntegerGrid(false);
			}
			else
			{
				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, y);
				m_pRenderer->PathCommandLineTo(x, b);
				m_pRenderer->Stroke();
			}
		}
		else
		{
			m_pRenderer->drawVerLine(align, x, y, b, penW);
		}
	}
	void CGraphics::drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW)
	{
		if (!m_pRenderer->get_IntegerGrid())
		{
			Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
			if (pMatrix->IsIdentity2())
			{
				m_pRenderer->put_IntegerGrid(true);
				m_pRenderer->drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
				m_pRenderer->put_IntegerGrid(false);
			}
			else
			{
				m_pRenderer->put_PenSize(penW);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->PathCommandMoveTo(x, y);
				m_pRenderer->PathCommandLineTo(r, y);
				m_pRenderer->Stroke();
			}
		}
		else
		{
			m_pRenderer->drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
		}
	}
	void CGraphics::rect       (double x, double y, double w, double h)
	{
		m_pRenderer->PathCommandEnd();
		if (m_pRenderer->get_IntegerGrid())
		{
			double r = x + w;
			double b = y + h;

			m_pRenderer->GetFullTransform()->TransformPoint(x, y);
			m_pRenderer->GetFullTransform()->TransformPoint(r, b);

			x = (int)(x + 0.5);
			y = (int)(y + 0.5);
			r = (int)(r + 0.5);
			b = (int)(b + 0.5);

			m_pRenderer->AddRect(x, y, r - x, b - y);
		}
		else
		{
			m_pRenderer->AddRect(x, y, w, h);
		}
	}
	void CGraphics::TableRect  (double x, double y, double w, double h)
	{
		m_pRenderer->PathCommandEnd();
		if (m_pRenderer->get_IntegerGrid())
		{
			double r = x + w;
			double b = y + h;

			m_pRenderer->GetFullTransform()->TransformPoint(x, y);
			m_pRenderer->GetFullTransform()->TransformPoint(r, b);

			x = (int)x;
			y = (int)y;
			r = (int)r;
			b = (int)b;

			m_pRenderer->AddRect(x, y, r - x + 1, b - y + 1);
		}
		else
		{
			m_pRenderer->AddRect(x, y, w, h);
		}
		m_pRenderer->Fill();
	}
	void CGraphics::AddClipRect(double x, double y, double w, double h)
	{
		CHist_Clip* _histClip = new CHist_Clip();

		double sx, shy, shx, sy, tx, ty;
		m_pRenderer->GetTransform(&sx, &shy, &shx, &sy, &tx, &ty);
		_histClip->Transform.SetElements(sx, shy, shx, sy, tx, ty);

		_histClip->IsIntegerGrid = m_pRenderer->get_IntegerGrid();
		_histClip->Rect.left     = x;
		_histClip->Rect.top      = y;
		_histClip->Rect.right    = x + w;
		_histClip->Rect.bottom   = y + h;

		m_oGrState.Clips.push_back(_histClip);

		StartClipPath();

		_s();
		_m(x, y);
		_l(x + w, y);
		_l(x + w, y + h);
		_l(x, y + h);
		_l(x, y);

		EndClipPath();
	}
	void CGraphics::RemoveClipRect()
	{
		if(m_oGrState.Clips.back())
		{
			delete m_oGrState.Clips.back();
			m_oGrState.Clips.pop_back();
		}
		restore();
	}
	void CGraphics::SetClip    (double x, double y, double w, double h)
	{
		rect(x, y, w, h);
		clip();
	}
	void CGraphics::drawMailMergeField(double x, double y, double w, double h)
	{
		b_color1(206, 212, 223, 204);
		rect(x, y, w, h);
		df();
		m_pRenderer->PathCommandEnd();
	}
	void CGraphics::drawSearchResult  (double x, double y, double w, double h)
	{
		b_color1(255, 238, 128, 255);
		rect(x, y, w, h);
		df();
		m_pRenderer->PathCommandEnd();
	}
	void CGraphics::SavePen()
	{
		CGrStatePen* pState = new CGrStatePen();
		m_pRenderer->SavePen(pState->m_oPen);
		m_oGrState.States.push_back(pState);
	}
	void CGraphics::RestorePen()
	{
		if(m_oGrState.States.empty())
			return;

		IGrState* pState = m_oGrState.States.back();
		if(pState->m_eType == gstPen)
		{
			m_pRenderer->RestorePen(((CGrStatePen*)pState)->m_oPen);
			m_oGrState.States.pop_back();
			RELEASEOBJECT(pState);
		}
	}
	void CGraphics::SaveBrush()
	{
		CGrStateBrush* pState = new CGrStateBrush();
		m_pRenderer->SaveBrush(pState->m_oBrush);
		m_oGrState.States.push_back(pState);
	}
	void CGraphics::RestoreBrush()
	{
		if (m_oGrState.States.empty())
			return;

		IGrState* pState = m_oGrState.States.back();
		if (pState->m_eType == gstBrush)
		{
			m_pRenderer->RestoreBrush(((CGrStateBrush*)pState)->m_oBrush);
			m_oGrState.States.pop_back();
			RELEASEOBJECT(pState);
		}
	}
	void CGraphics::SavePenBrush()
	{
		CGrStatePenBrush* pState = new CGrStatePenBrush();
		m_pRenderer->SavePen(pState->m_oPen);
		m_pRenderer->SaveBrush(pState->m_oBrush);
		m_oGrState.States.push_back(pState);
	}
	void CGraphics::RestorePenBrush()
	{
		if (m_oGrState.States.empty())
			return;

		IGrState* pState = m_oGrState.States.back();
		if (pState->m_eType == gstPenBrush)
		{
			m_pRenderer->RestorePen(((CGrStatePenBrush*)pState)->m_oPen);
			m_pRenderer->RestoreBrush(((CGrStatePenBrush*)pState)->m_oBrush);
			m_oGrState.States.pop_back();
			RELEASEOBJECT(pState);
		}
	}
	void CGraphics::SaveGrState()
	{
#ifdef _DEBUG
		std::cout << "SaveGrState " << std::endl;
#endif
		CGrStateState* pState = new CGrStateState();
		pState->IsIntegerGrid = m_pRenderer->get_IntegerGrid();
		pState->Clips = m_oGrState.Clips;

		double sx, shy, shx, sy, tx, ty;
		m_pRenderer->GetTransform(&sx, &shy, &shx, &sy, &tx, &ty);
		pState->Transform.SetElements(sx, shy, shx, sy, tx, ty);

		m_oGrState.Clips.clear();
		m_oGrState.States.push_back(pState);
	}
	void CGraphics::RestoreGrState()
	{
#ifdef _DEBUG
		std::cout << "RestoreGrState " << std::endl;
#endif
		if (m_oGrState.States.empty())
			return;

		IGrState* pState = m_oGrState.States.back();
		if (pState->m_eType != gstState)
			return;

		CGrStateState* pGrState = (CGrStateState*)pState;

		if (!m_oGrState.Clips.empty())
		{
			restore();

			for (IGrState* i : m_oGrState.States)
			{
				if (i->m_eType == gstState)
				{
					std::vector<CHist_Clip*>& arr = ((CGrStateState*)i)->Clips;

					for (CHist_Clip* j : arr)
					{
						Aggplus::CMatrix& oMatrix = j->Transform;
						transform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
						SetIntegerGrid(j->IsIntegerGrid);

						StartClipPath();

						double x = j->Rect.left;
						double y = j->Rect.top;
						double r = j->Rect.right;
						double b = j->Rect.bottom;

						_s();
						_m(x, y);
						_l(r, y);
						_l(r, b);
						_l(x, b);
						_l(x, y);

						EndClipPath();
					}
				}
			}
		}

		for (CHist_Clip* pClip : m_oGrState.Clips)
			RELEASEOBJECT(pClip);
		m_oGrState.Clips.clear();

		m_oGrState.Clips = pGrState->Clips;
		pGrState->Clips.clear();
		m_oGrState.States.pop_back();

		Aggplus::CMatrix& oMatrix = pGrState->Transform;
		transform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
		SetIntegerGrid(pGrState->IsIntegerGrid);

		RELEASEOBJECT(pState);
	}
	void CGraphics::StartClipPath()
	{
#ifdef _DEBUG
		std::cout << "StartClipPath " << std::endl;
#endif
		m_pRenderer->BeginCommand(c_nClipType);
	}
	void CGraphics::EndClipPath()
	{
#ifdef _DEBUG
		std::cout << "EndClipPath " << std::endl;
#endif
		m_pRenderer->EndCommand(c_nClipType);
	}
	bool CGraphics::StartCheckTableDraw()
	{
		if(!m_pRenderer->get_IntegerGrid())
		{
			Aggplus::CMatrix* pMatrix = m_pRenderer->GetTransformMatrix();
			if(pMatrix->IsIdentity2())
			{
				SaveGrState();
				m_pRenderer->put_IntegerGrid(true);
				return true;
			}
		}
		return false;
	}
	void CGraphics::SetTextClipRect(double _l, double _t, double _r, double _b)
	{
		AddClipRect(_l, _t, _r - _l, _b - _t);
	}
	void CGraphics::DrawFootnoteRect(double x, double y, double w, double h)
	{
		BYTE nPenDashStyle = 0;
		m_pRenderer->get_PenDashStyle(&nPenDashStyle);

		bool bIsIntegerGrid = m_pRenderer->get_IntegerGrid();
		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(true);

		double dash[2] = { 2.0, 2.0 };
		m_pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		m_pRenderer->PenDashPattern(dash, 2);

		m_pRenderer->PathCommandEnd();

		double l = x;
		double t = y;
		double r = x + w;
		double b = y + h;

		drawHorLineExt(1, t, l, r, 0, 0, 0);
		drawVerLine   (1, l, t, b, 0);
		drawVerLine   (1, r, t, b, 0);
		drawHorLineExt(1, b, l, r, 0, 0, 0);

		m_pRenderer->PathCommandEnd();
		m_pRenderer->Stroke();

		if (!bIsIntegerGrid)
			m_pRenderer->put_IntegerGrid(false);

		m_pRenderer->put_PenDashStyle(nPenDashStyle);
	}
	std::string CGraphics::toDataURL(std::wstring type)
	{
		std::wstring sPath = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_sApplicationImagesDirectory, L"img");
#ifdef _DEBUG
		std::wcout << "toDataURL " << sPath << std::endl;
#endif
		m_oFrame.SaveFile(sPath, _CXIMAGE_FORMAT_PNG);

		NSFile::CFileBinary oReader;
		if (oReader.OpenFile(sPath))
		{
			DWORD dwFileSize = oReader.GetFileSize();
			BYTE* pFileContent = new BYTE[dwFileSize];
			DWORD dwReaded;
			oReader.ReadFile(pFileContent, dwFileSize, dwReaded);
			oReader.CloseFile();

			NSFile::CFileBinary::Remove(sPath);
			int nEncodeLen = NSBase64::Base64EncodeGetRequiredLength(dwFileSize);
			BYTE* pImageData = new BYTE[nEncodeLen];
			if (TRUE == NSBase64::Base64Encode(pFileContent, dwFileSize, pImageData, &nEncodeLen))
				return "data:" + U_TO_UTF8(type) + ";base64, " + std::string((char*)pImageData, nEncodeLen);
		}
		return "";
	}
	CColor CGraphics::GetPenColor()
	{
		LONG color;
		LONG a;
		m_pRenderer->get_PenColor(&color);
		m_pRenderer->get_PenAlpha(&a);
		return {(int)(color & 0xFF), (int)((color >> 8) & 0xFF), (int)((color >> 16) & 0xFF), (int)a};
	}
	CColor CGraphics::GetBrushColor()
	{
		LONG color;
		LONG a;
		m_pRenderer->get_BrushColor1(&color);
		m_pRenderer->get_BrushAlpha1(&a);
		return {(int)(color & 0xFF), (int)((color >> 8) & 0xFF), (int)((color >> 16) & 0xFF), (int)a};
	}
	void CGraphics::put_brushTexture(std::wstring src, int type)
	{
		if (src.find(L"data:") == 0)
		{
			std::wstring strImage = m_sApplicationImagesDirectory + L"/texture.png";
			bool bIsOnlyOfficeHatch = false;
			if(src.find(L"onlyoffice_hatch") != std::wstring::npos)
				bIsOnlyOfficeHatch = true;
#ifdef _DEBUG
			std::wcout << L"put_brushTexture " << src << L"  "  << type << std::endl;
#endif
			src.erase(0, src.find(L',') + 1);

			std::string sBase64MultyByte(src.begin(), src.end());
			int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
			if(nDecodeLen == 0)
				return;
			BYTE* pImageData = new BYTE[nDecodeLen + 64];
			if (TRUE == NSBase64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(), pImageData, &nDecodeLen))
			{
				if(!bIsOnlyOfficeHatch)
				{
					NSFile::CFileBinary oImageWriter;
					if (oImageWriter.CreateFileW(strImage))
					{
						oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
						oImageWriter.CloseFile();
					}
				}
				else
				{
					int nSize = (int)sqrt(nDecodeLen >> 2);
					CBgraFrame oFrame;
					oFrame.put_Data(pImageData);
					oFrame.put_Width(nSize);
					oFrame.put_Height(nSize);
					oFrame.put_Stride(4 * nSize);
					oFrame.put_IsRGBA(true);
					oFrame.SaveFile(strImage, 4);
				}
				m_pRenderer->put_BrushType(c_BrushTypeTexture);
				m_pRenderer->put_BrushTexturePath(strImage);
				m_pRenderer->put_BrushTextureMode(type);
			}
		}
		else
		{
			std::wstring strImage = (0 == src.find(L"theme") ? m_sApplicationThemesDirectory : m_sApplicationImagesDirectory) + L'/' + src;
			std::wstring sName = strImage.substr(0, strImage.rfind(L'.') + 1);
			std::wstring sExt = src.substr(src.rfind(L'.') + 1);
			if (sExt == L"svg")
			{
				if (NSFile::CFileBinary::Exists(sName + L"wmf") && src.find(L"display") == 0)
					strImage = sName + L"wmf";
				else if (NSFile::CFileBinary::Exists(sName + L"emf") && src.find(L"display") == 0)
					strImage = sName + L"emf";

				MetaFile::IMetaFile* pMetafile = MetaFile::Create(m_pApplicationFonts);
				pMetafile->LoadFromFile(strImage.c_str());

				double x = 0, y = 0, w = 0, h = 0;
				pMetafile->GetBounds(&x, &y, &w, &h);

				sName += L"png";
				pMetafile->ConvertToRaster(sName.c_str(), 4, 1000);

				RELEASEOBJECT(pMetafile);
			}
			else
				sName += sExt;
#ifdef _DEBUG
			std::wcout << L"put_brushTexture " << sName << L"  " << type << std::endl;
#endif
			m_pRenderer->put_BrushType(c_BrushTypeTexture);
			m_pRenderer->put_BrushTexturePath(sName);
			m_pRenderer->put_BrushTextureMode(type);
		}
	}
	void CGraphics::put_brushTextureMode(int mode)
	{
#ifdef _DEBUG
		std::cout << "put_brushTextureMode " << mode << std::endl;
#endif
		m_pRenderer->put_BrushTextureMode(mode);
	}
	void CGraphics::put_BrushTextureAlpha(int a)
	{
#ifdef _DEBUG
		std::cout << "put_BrushTextureAlpha " << a << std::endl;
#endif
		m_pRenderer->put_BrushTextureAlpha(a == 0 ? 255 : a);
	}
	void CGraphics::put_BrushGradient(LONG* pColors, double* pPositions, size_t nCount, double x0, double y0, double x1, double y1, double r0, double r1)
	{
#ifdef _DEBUG
		std::cout << "put_BrushGradient " << nCount << "  " << x0 << "  " << y0 << "  " << x1 << "  " << y1 << "  " << r0 << "  " << r1 << std::endl;
		for (size_t i = 0; i < nCount; i++)
			std::cout << pPositions[i] << "  " << pColors[i] << "  ";
		std::cout << std::endl;
#endif
		if (std::isnan(r0))
		{
			// линейный
			double dAngle = 0;
			if (fabs(x1 - x0) >= FLT_EPSILON || fabs(y1 - y0) >= FLT_EPSILON)
				dAngle = atan2(y1 - y0, x1 - x0) * 180 / M_PI;
			m_pRenderer->put_BrushType(c_BrushTypePathGradient1);
			m_pRenderer->put_BrushGradientColors(pColors, pPositions, nCount);
			m_pRenderer->put_BrushLinearAngle(dAngle);
		}
		else
		{
			// радиальный
			m_pRenderer->put_BrushType(c_BrushTypePathGradient2);
			m_pRenderer->put_BrushGradientColors(pColors, pPositions, nCount);
		}
	}
	double CGraphics::TransformPointX(double x, double y)
	{
#ifdef _DEBUG
		std::cout << "TransformPointX " << std::endl;
#endif
		m_pRenderer->GetFullTransform()->TransformPoint(x, y);
		return x;
	}
	double CGraphics::TransformPointY(double x, double y)
	{
#ifdef _DEBUG
		std::cout << "TransformPointY " << std::endl;
#endif
		m_pRenderer->GetFullTransform()->TransformPoint(x, y);
		return y;
	}
	void CGraphics::put_LineJoin(int nJoin)
	{
#ifdef _DEBUG
		std::cout << "put_LineJoin " << std::endl;
#endif
		m_pRenderer->put_PenLineJoin(nJoin);
	}
	int CGraphics::GetLineJoin()
	{
#ifdef _DEBUG
		std::cout << "GetLineJoin " << std::endl;
#endif
		BYTE nRes;
		m_pRenderer->get_PenLineJoin(&nRes);
		return nRes;
	}
	void CGraphics::put_TextureBounds(double x, double y, double w, double h)
	{
#ifdef _DEBUG
		std::cout << "put_TextureBounds " << x << "  " << y << "  " << w << "  " << h << std::endl;
#endif
		if(m_pRenderer->get_IntegerGrid())
		{
			double r = x + w;
			double b = y + h;
			m_pRenderer->GetFullTransform()->TransformPoint(x, y);
			m_pRenderer->GetFullTransform()->TransformPoint(r, b);
			m_pRenderer->BrushBounds(x, y, r - x, b - y);
		}
		else
			m_pRenderer->BrushBounds(x, y, w, h);
	}
	double CGraphics::GetlineWidth()
	{
#ifdef _DEBUG
		std::cout << "GetlineWidth " << std::endl;
#endif
		double nRes;
		m_pRenderer->get_PenSize(&nRes);
		return nRes;
	}
	void CGraphics::DrawPath(int path)
	{
#ifdef _DEBUG
		std::cout << "DrawPath " << path << std::endl;
#endif
		if(path == 257)
		{
			m_pRenderer->DrawPath(256);
			m_pRenderer->DrawPath(1);
		}
		else
			m_pRenderer->DrawPath(path);
	}
	void CGraphics::CoordTransformOffset(double tx, double ty)
	{
#ifdef _DEBUG
		std::cout << "CoordTransformOffset " << tx << "  " << ty << std::endl;
#endif
		m_pRenderer->SetCoordTransformOffset(tx, ty);
	}
	CTransform CGraphics::GetTransform()
	{
		CTransform oRes;
		m_pRenderer->GetTransform(&oRes.sx, &oRes.shy, &oRes.shx, &oRes.sy, &oRes.tx, &oRes.ty);
		return oRes;
	}
}
