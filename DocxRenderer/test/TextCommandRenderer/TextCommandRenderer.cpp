/*
 *(c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License(AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "TextCommandRenderer.h"

#include <array>

#include "../../../DesktopEditor/graphics/pro/Graphics.h"

#include "Utils.h"

class CTextCommandRenderer::CTextCommandRendererImpl
{
public:
	CTextCommandRendererImpl() = delete;
	explicit CTextCommandRendererImpl(NSFonts::IApplicationFonts* pFonts);
	CTextCommandRendererImpl(const CTextCommandRendererImpl& other) = delete;
	CTextCommandRendererImpl(CTextCommandRendererImpl&& other) = delete;
	virtual ~CTextCommandRendererImpl();

	void BeginCommand(const DWORD& lType);
	void EndCommand  (const DWORD& lType);
	void AddText(double x, double y, double w, double h);
	void NewPage();
	void SetFolder(const std::wstring& wsFolder);

	Aggplus::CMatrix m_oTransform{};
	NSStructures::CFont m_oFont{};

	double m_dWidth = 0.0;
	double m_dHeight = 0.0;

	double m_dDpiX = c_dDpiX;
	double m_dDpiY = c_dDpiY;

private:
	void CreateFrame();
	void ResetMinMax();

	long GetNextColor();

	std::unique_ptr<NSGraphics::IGraphicsRenderer> m_pRenderer{nullptr};
	std::unique_ptr<CBgraFrame> m_pFrame{nullptr};
	std::unique_ptr<NSFonts::IFontManager> m_pManager{nullptr};

	double m_dXMin = c_dX_MIN_DEFAULT;
	double m_dYMin = c_dY_MIN_DEFAULT;

	double m_dXMax = c_dX_MAX_DEFAULT;
	double m_dYMax = c_dY_MAX_DEFAULT;

	double m_dPrevCenterX = 0;
	double m_dPrevCenterY = 0;
	bool m_bIsFirst = true;

	std::wstring m_wsFolder = L"./output";
	size_t m_nPage = 1;

	const double m_dPenSizeRect = 0.6;
	const double m_dPenSizeLine = 0.3;
	const double m_dPenSizeDot = 1;

	const std::wstring m_wsBaseFilename = L"page";
	const std::wstring m_wsBaseExt = L".png";
	const std::wstring m_wsBaseSep = L"/";

	size_t m_nCurrColorIndex = 0;
	const std::array<unsigned long, 20> m_arColors = {
	    0xFF400000, // Deep Red
	    0xFF800000, // Dark Red
	    0xFFFF0000, // Pure Red
	    0xFFFF4000, // Red-Orange
	    0xFFFF8000, // Orange
	    0xFFFFC000, // Light Orange
	    0xFFFFFF00, // Yellow
	    0xFFC0FF00, // Yellow-Green
	    0xFF80FF00, // Lime
	    0xFF40FF00, // Green-Yellow
	    0xFF00FF00, // Pure Green
	    0xFF00FF80, // Green-Cyan
	    0xFF00FFFF, // Cyan
	    0xFF0080FF, // Light Blue
	    0xFF0040FF, // Blue
	    0xFF0000FF, // Pure Blue
	    0xFF4000FF, // Blue-Purple
	    0xFF8000FF, // Purple
	    0xFFC000FF, // Violet
	    0xFFFF00FF  // Magenta
	};
};

CTextCommandRenderer::CTextCommandRendererImpl::CTextCommandRendererImpl(NSFonts::IApplicationFonts* pFonts)
    : m_pRenderer(NSGraphics::Create()), m_pManager(pFonts->GenerateFontManager())
{
	m_pManager->CreateOwnerCache(8);
}
CTextCommandRenderer::CTextCommandRendererImpl::~CTextCommandRendererImpl()
{
}
void CTextCommandRenderer::CTextCommandRendererImpl::BeginCommand(const DWORD& lType)
{
	if (lType == c_nTextType)
	{
		if (!m_pFrame) CreateFrame();
		ResetMinMax();
	}
}
void CTextCommandRenderer::CTextCommandRendererImpl::EndCommand(const DWORD& lType)
{
	if (lType == c_nPageType)
	{
		// if no text on the page
		if (!m_pFrame) CreateFrame();
		std::wstring filename = m_wsFolder + m_wsBaseSep + m_wsBaseFilename + std::to_wstring(m_nPage) + m_wsBaseExt;
		m_pFrame->SaveFile(filename, _CXIMAGE_FORMAT_PNG);
		m_pFrame = nullptr;
		++m_nPage;
	}
	else if (lType == c_nTextType)
	{
		// 1. draw rect of command
		double w = m_dXMax - m_dXMin;
		double h = m_dYMax - m_dYMin;

		m_pRenderer->BeginCommand(c_nPathType);
		m_pRenderer->AddRect(m_dXMin, m_dYMin, w, h);

		m_pRenderer->put_PenSize(m_dPenSizeRect);

		long color = GetNextColor();
		m_pRenderer->put_PenColor(color);

		m_pRenderer->Stroke();
		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndCommand(c_nPathType);

		// 2. draw line from prev command to curr
		double curr_center_x = m_dXMin + w / 2;
		double curr_center_y = m_dYMin + h / 2;

		if (m_bIsFirst)
		{
			m_bIsFirst = false;
			m_dPrevCenterX = curr_center_x;
			m_dPrevCenterY = curr_center_y;
			return;
		}

		m_pRenderer->BeginCommand(c_nPathType);
		m_pRenderer->PathCommandMoveTo(m_dPrevCenterX, m_dPrevCenterY);
		m_pRenderer->PathCommandLineTo(curr_center_x, curr_center_y);
		m_pRenderer->PathCommandClose();

		m_pRenderer->put_PenSize(m_dPenSizeLine);
		m_pRenderer->put_PenColor(0); // black color

		m_pRenderer->Stroke();
		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndCommand(c_nPathType);

		m_dPrevCenterX = curr_center_x;
		m_dPrevCenterY = curr_center_y;

		// 3. draw a point of center
		const double half_dot_size = m_dPenSizeDot / 2;
		const double dot_x = curr_center_x - half_dot_size;
		const double dot_y = curr_center_y - half_dot_size;
		const double dot_w = m_dPenSizeDot;
		const double dot_h = m_dPenSizeDot;

		m_pRenderer->BeginCommand(c_nPathType);
		m_pRenderer->AddRect(dot_x, dot_y, dot_w, dot_h);

		m_pRenderer->put_PenSize(m_dPenSizeLine);
		m_pRenderer->put_PenColor(0); // black color

		m_pRenderer->Stroke();
		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndCommand(c_nPathType);
	}
}
void CTextCommandRenderer::CTextCommandRendererImpl::AddText(double x, double y, double w, double h)
{
	MAYBE_UNUSED(h);

	m_pManager->LoadFontFromFile(m_oFont.Path, (int)m_oFont.FaceIndex, (float)m_oFont.Size, c_dDpiX, c_dDpiY);
	m_pManager->AfterLoad();

	double line_height = m_pManager->GetLineHeight();
	double em_height = m_pManager->GetUnitsPerEm();

	h = c_dPtToMM * (line_height * m_oFont.Size) / em_height;

	double x2 = x + w;
	double y2 = y + h;

	m_oTransform.TransformPoint(x, y);
	m_oTransform.TransformPoint(x2, y2);

	m_dXMin = std::min(m_dXMin, x);
	m_dYMin = std::min(m_dYMin, y);

	m_dXMax = std::max(m_dXMax, x2);
	m_dYMax = std::max(m_dYMax, y2);
}
void CTextCommandRenderer::CTextCommandRendererImpl::NewPage()
{
	ResetMinMax();
	m_nCurrColorIndex = 0;
	m_bIsFirst = true;
}
void CTextCommandRenderer::CTextCommandRendererImpl::SetFolder(const std::wstring& wsFolder)
{
	m_wsFolder = wsFolder;
}

void CTextCommandRenderer::CTextCommandRendererImpl::CreateFrame()
{
	long width_pix = static_cast<long>(m_dWidth * c_dMMToPix);
	long height_pix = static_cast<long>(m_dHeight * c_dMMToPix);

	const long step = 4;
	const long stride = -step * width_pix;

	m_pFrame = std::unique_ptr<CBgraFrame>(new CBgraFrame());
	size_t data_size = width_pix * height_pix * step;
	BYTE* data = new BYTE[data_size];

	// white and alpha is min (full transparent)
	for (long i = 0; i < width_pix * height_pix; ++i)
		reinterpret_cast<unsigned int*>(data)[i] = 0xffffffff;

	m_pFrame->put_Data(data);
	m_pFrame->put_Height(height_pix);
	m_pFrame->put_Width(width_pix);
	m_pFrame->put_Stride(stride);

	m_pRenderer->NewPage();
	m_pRenderer->CreateFromBgraFrame(m_pFrame.get());
	m_pRenderer->SetSwapRGB(true);
	m_pRenderer->put_Width(m_dWidth);
	m_pRenderer->put_Height(m_dHeight);
}
void CTextCommandRenderer::CTextCommandRendererImpl::ResetMinMax()
{
	m_dXMin = c_dX_MIN_DEFAULT;
	m_dYMin = c_dY_MIN_DEFAULT;

	m_dXMax = c_dX_MAX_DEFAULT;
	m_dYMax = c_dY_MAX_DEFAULT;
}
long CTextCommandRenderer::CTextCommandRendererImpl::GetNextColor()
{
	m_nCurrColorIndex = (m_nCurrColorIndex + 1) % m_arColors.size();
	return m_arColors[m_nCurrColorIndex];
}

CTextCommandRenderer::CTextCommandRenderer(NSFonts::IApplicationFonts* pFonts)
    : m_pImpl(std::unique_ptr<CTextCommandRendererImpl>(new CTextCommandRendererImpl(pFonts)))
{
}
CTextCommandRenderer::~CTextCommandRenderer()
{
}

// base settings
HRESULT CTextCommandRenderer::get_Type(LONG* lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::NewPage()
{
	m_pImpl->NewPage();
	return S_OK;
}
HRESULT CTextCommandRenderer::get_Height(double* dHeight)
{
	*dHeight = m_pImpl->m_dHeight;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_Height(const double& dHeight)
{
	m_pImpl->m_dHeight = dHeight;
	return S_OK;
}
HRESULT CTextCommandRenderer::get_Width(double* dWidth)
{
	*dWidth = m_pImpl->m_dWidth;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_Width(const double& dWidth)
{
	m_pImpl->m_dWidth = dWidth;
	return S_OK;
}
HRESULT CTextCommandRenderer::get_DpiX(double* dDpiX)
{
	*dDpiX = m_pImpl->m_dDpiX;
	return S_OK;
}
HRESULT CTextCommandRenderer::get_DpiY(double* dDpiY)
{
	*dDpiY = m_pImpl->m_dDpiY;
	return S_OK;
}

// pen settings
HRESULT CTextCommandRenderer::get_PenColor(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenColor(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenAlpha(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenAlpha(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenSize(double* dSize)
{
	MAYBE_UNUSED(dSize);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenSize(const double& dSize)
{
	MAYBE_UNUSED(dSize);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
	MAYBE_UNUSED(nDashStyle);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
	MAYBE_UNUSED(nDashStyle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
	MAYBE_UNUSED(nJoinStyle);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
	MAYBE_UNUSED(nJoinStyle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenDashOffset(double* dOffset)
{
	MAYBE_UNUSED(dOffset);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenDashOffset(const double& dOffset)
{
	MAYBE_UNUSED(dOffset);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenAlign(LONG* lAlign)
{
	MAYBE_UNUSED(lAlign);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenAlign(const LONG& lAlign)
{
	MAYBE_UNUSED(lAlign);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_PenMiterLimit(double* dMiter)
{
	MAYBE_UNUSED(dMiter);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_PenMiterLimit(const double& dMiter)
{
	MAYBE_UNUSED(dMiter);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	MAYBE_UNUSED(pPattern); MAYBE_UNUSED(lCount);
	return S_FALSE;
}

// brush settings
HRESULT CTextCommandRenderer::get_BrushType(LONG* lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushType(const LONG& lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushColor1(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushColor1(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushColor2(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushColor2(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
	MAYBE_UNUSED(wsPath);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
	MAYBE_UNUSED(wsPath);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushTextureMode(LONG* lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushTextureMode(const LONG& lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushLinearAngle(double* dAngle)
{
	MAYBE_UNUSED(dAngle);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushLinearAngle(const double& dAngle)
{
	MAYBE_UNUSED(dAngle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::BrushRect(const INT& nVal,
                                        const double& dLeft,
                                        const double& dTop,
                                        const double& dWidth,
                                        const double& dHeight)
{
	MAYBE_UNUSED(nVal); MAYBE_UNUSED(dLeft); MAYBE_UNUSED(dTop);
	MAYBE_UNUSED(dWidth); MAYBE_UNUSED(dHeight);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::BrushBounds(const double& dLeft,
                                          const double& dTop,
                                          const double& dWidth,
                                          const double& dHeight)
{
	MAYBE_UNUSED(dLeft); MAYBE_UNUSED(dTop); MAYBE_UNUSED(dWidth); MAYBE_UNUSED(dHeight);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::put_BrushGradientColors(LONG* pColors,
                                                      double* pPositions,
                                                      LONG lCount)
{
	MAYBE_UNUSED(pColors); MAYBE_UNUSED(pPositions); MAYBE_UNUSED(lCount);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushTextureImage(Aggplus::CImage** pImage)
{
	MAYBE_UNUSED(pImage);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushTextureImage(Aggplus::CImage* pImage)
{
	MAYBE_UNUSED(pImage);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::get_BrushTransform(Aggplus::CMatrix& oMatrix)
{
	MAYBE_UNUSED(oMatrix);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_BrushTransform(const Aggplus::CMatrix& oMatrix)
{
	MAYBE_UNUSED(oMatrix);
	return S_FALSE;
}

void CTextCommandRenderer::put_BrushGradInfo(void* pGradInfo)
{
	MAYBE_UNUSED(pGradInfo);
}

// font settings
HRESULT CTextCommandRenderer::get_FontName(std::wstring* wsName)
{
	*wsName = m_pImpl->m_oFont.Name;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontName(const std::wstring& wsName)
{
	m_pImpl->m_oFont.Name = wsName;
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontPath(std::wstring* wsPath)
{
	*wsPath = m_pImpl->m_oFont.Path;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontPath(const std::wstring& wsPath)
{
	m_pImpl->m_oFont.Path = wsPath;
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontSize(double* dSize)
{
	*dSize = m_pImpl->m_oFont.Size;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontSize(const double& dSize)
{
	m_pImpl->m_oFont.Size = dSize;
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_pImpl->m_oFont.GetStyle();
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontStyle(const LONG& lStyle)
{
	m_pImpl->m_oFont.SetStyle(lStyle);
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontStringGID(INT* bGid)
{
	*bGid = m_pImpl->m_oFont.StringGID;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontStringGID(const INT& bGid)
{
	m_pImpl->m_oFont.StringGID = bGid;
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_pImpl->m_oFont.CharSpace;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontCharSpace(const double& dSpace)
{
	m_pImpl->m_oFont.CharSpace = dSpace;
	return S_OK;
}

HRESULT CTextCommandRenderer::get_FontFaceIndex(int* lFaceIndex)
{
	*lFaceIndex = m_pImpl->m_oFont.FaceIndex;
	return S_OK;
}
HRESULT CTextCommandRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
	m_pImpl->m_oFont.FaceIndex = lFaceIndex;
	return S_OK;
}

// text commands
HRESULT CTextCommandRenderer::CommandDrawTextCHAR(const LONG& lUnicode,
                                                  const double& dX,
                                                  const double& dY,
                                                  const double& dW,
                                                  const double& dH)
{
	MAYBE_UNUSED(lUnicode);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::CommandDrawTextExCHAR(const LONG& lUnicode,
                                                    const LONG& lGid,
                                                    const double& dX,
                                                    const double& dY,
                                                    const double& dW,
                                                    const double& dH)
{
	MAYBE_UNUSED(lUnicode); MAYBE_UNUSED(lGid);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::CommandDrawText(const std::wstring& wsUnicodeText,
                                              const double& dX,
                                              const double& dY,
                                              const double& dW,
                                              const double& dH)
{
	MAYBE_UNUSED(wsUnicodeText);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::CommandDrawTextEx(const std::wstring& wsUnicodeText,
                                                const unsigned int* pGids,
                                                const unsigned int nGidsCount,
                                                const double& dX,
                                                const double& dY,
                                                const double& dW,
                                                const double& dH)
{
	MAYBE_UNUSED(wsUnicodeText); MAYBE_UNUSED(pGids); MAYBE_UNUSED(nGidsCount);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

// command type
HRESULT CTextCommandRenderer::BeginCommand(const DWORD& lType)
{
	m_pImpl->BeginCommand(lType);
	return S_OK;
}
HRESULT CTextCommandRenderer::EndCommand(const DWORD& lType)
{
	m_pImpl->EndCommand(lType);
	return S_OK;
}

// graphic commands
HRESULT CTextCommandRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
	MAYBE_UNUSED(pPoints); MAYBE_UNUSED(nCount);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandCurveTo(const double& dX1,
                                                 const double& dY1,
                                                 const double& dX2,
                                                 const double& dY2,
                                                 const double& dXe,
                                                 const double& dYe)
{
	MAYBE_UNUSED(dX1); MAYBE_UNUSED(dY1); MAYBE_UNUSED(dX2);
	MAYBE_UNUSED(dY2); MAYBE_UNUSED(dXe); MAYBE_UNUSED(dYe);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
	MAYBE_UNUSED(pPoints); MAYBE_UNUSED(nCount);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandArcTo(const double& dX,
                                               const double& dY,
                                               const double& dW,
                                               const double& dH,
                                               const double& dStartAngle,
                                               const double& dSweepAngle)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY); MAYBE_UNUSED(dW);
	MAYBE_UNUSED(dH); MAYBE_UNUSED(dStartAngle); MAYBE_UNUSED(dSweepAngle);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::PathCommandClose()
{
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandEnd()
{
	return S_FALSE;
}
HRESULT CTextCommandRenderer::DrawPath(const LONG& lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::PathCommandStart()
{
	return S_FALSE;
}

HRESULT CTextCommandRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::PathCommandTextCHAR(const LONG& lUnicode,
                                                  const double& dX,
                                                  const double& dY,
                                                  const double& dW,
                                                  const double& dH)
{
	MAYBE_UNUSED(lUnicode);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::PathCommandTextExCHAR(const LONG& lUnicode,
                                                    const LONG& lGid,
                                                    const double& dX,
                                                    const double& dY,
                                                    const double& dW,
                                                    const double& dH)
{
	MAYBE_UNUSED(lUnicode); MAYBE_UNUSED(lGid);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::PathCommandText(const std::wstring& wsUnicodeText,
                                              const double& dX,
                                              const double& dY,
                                              const double& dW,
                                              const double& dH)
{
	MAYBE_UNUSED(wsUnicodeText);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTextCommandRenderer::PathCommandTextEx(const std::wstring& wsUnicodeText,
                                                const unsigned int* pGids,
                                                const unsigned int nGidsCount,
                                                const double& dX,
                                                const double& dY,
                                                const double& dW,
                                                const double& dH)
{
	MAYBE_UNUSED(wsUnicodeText); MAYBE_UNUSED(pGids); MAYBE_UNUSED(nGidsCount);
	m_pImpl->AddText(dX, dY, dW, dH);
	return S_OK;
}

// image commands
HRESULT CTextCommandRenderer::DrawImage(IGrObject* pImage,
                                        const double& dX,
                                        const double& dY,
                                        const double& dW,
                                        const double& dH)
{
	MAYBE_UNUSED(pImage); MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	MAYBE_UNUSED(dW); MAYBE_UNUSED(dH);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::DrawImageFromFile(const std::wstring& wsImagePath,
                                                const double& dX,
                                                const double& dY,
                                                const double& dW,
                                                const double& dH,
                                                const BYTE& nAlpha)
{
	MAYBE_UNUSED(wsImagePath); MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	MAYBE_UNUSED(dW); MAYBE_UNUSED(dH); MAYBE_UNUSED(nAlpha);
	return S_FALSE;
}

// transform commands
HRESULT CTextCommandRenderer::SetTransform(const double& dM11,
                                           const double& dM12,
                                           const double& dM21,
                                           const double& dM22,
                                           const double& dX,
                                           const double& dY)
{
	m_pImpl->m_oTransform.SetElements(dM11, dM12, dM21, dM22, dX, dY);
	return S_OK;
}

HRESULT CTextCommandRenderer::GetTransform(double* dM11,
                                           double* dM12,
                                           double* dM21,
                                           double* dM22,
                                           double* dX,
                                           double* dY)
{
	MAYBE_UNUSED(dM11); MAYBE_UNUSED(dM12); MAYBE_UNUSED(dM21);
	MAYBE_UNUSED(dM22); MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::ResetTransform()
{
	m_pImpl->m_oTransform.Reset();
	return S_OK;
}

HRESULT CTextCommandRenderer::get_ClipMode(LONG* lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::put_ClipMode(const LONG& lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(lCommand);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(dCommand);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(sCommand);
	return S_FALSE;
}

HRESULT CTextCommandRenderer::IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type)
{
	MAYBE_UNUSED(type);
	return S_FALSE;
}
HRESULT CTextCommandRenderer::AdvancedCommand(IAdvancedCommand* command)
{
	MAYBE_UNUSED(command);
	return S_FALSE;
}

void CTextCommandRenderer::Do(IOfficeDrawingFile* pFile, const std::wstring& wsFolder)
{
	m_pImpl->SetFolder(wsFolder);
	int pages_count = pFile->GetPagesCount();
	for (int i = 0; i < pages_count; ++i)
		DrawPage(pFile, i);
}
void CTextCommandRenderer::DrawPage(IOfficeDrawingFile* pFile, int nPage)
{
	NewPage();

	double width, height, dpi_x, dpi_y;
	pFile->GetPageInfo(nPage, &width, &height, &dpi_x, &dpi_y);

	width  *= 25.4 / dpi_x;
	height *= 25.4 / dpi_y;

	put_Width(width);
	put_Height(height);

	pFile->DrawPageOnRenderer(this, nPage, nullptr);
}
