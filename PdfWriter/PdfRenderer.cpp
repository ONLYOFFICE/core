#include "PdfRenderer.h"

#include "Src/Document.h"
#include "Src/Pages.h"
#include "Src/Image.h"
#include "Src/Font.h"
#include "Src/FontCidTT.h"

#include "../DesktopEditor/graphics/Image.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/cximage/CxImage/ximage.h"
#include "../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../DesktopEditor/fontengine/FontManager.h"

#define MM_2_PT(X) ((X) * 72.0 / 25.4)
#define PT_2_MM(X) ((X) * 25.4 / 72.0)

#define LONG_2_BOOL(X) ((X) ? true : false)

#ifdef DrawText
#undef DrawText
#endif

using namespace PdfWriter;

#define HI_SURROGATE_START  0xD800
#define HI_SURROGATE_END    0xDBFF
#define LO_SURROGATE_START  0xDC00
#define LO_SURROGATE_END    0xDFFF

CPdfRenderer::CPdfRenderer(CApplicationFonts* pAppFonts)
{
	m_pAppFonts = pAppFonts;

	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = pAppFonts->GenerateFontManager();
	CFontsCache* pMeasurerCache = new CFontsCache();
	pMeasurerCache->SetStreams(pAppFonts->GetStreams());
	m_pFontManager->SetOwnerCache(pMeasurerCache);

	m_pDocument = new CDocument();
	if (!m_pDocument || !m_pDocument->CreateNew())
	{
		SetError();
		return;
	}

	m_bValid      = true;
	m_dPageHeight = 297;
	m_dPageWidth  = 210;
	m_pPage       = NULL;
	m_pFont       = NULL;
}
CPdfRenderer::~CPdfRenderer()
{
	RELEASEOBJECT(m_pDocument);
	RELEASEINTERFACE(m_pFontManager);
}
void CPdfRenderer::SaveToFile(const std::wstring& wsPath)
{
	if (!IsValid())
		return;

	m_pDocument->SaveToFile(wsPath);
}
//----------------------------------------------------------------------------------------
// Тип рендерера
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_Type(LONG* lType)
{
	*lType = c_nPDFWriter;
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со страницей
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::NewPage()
{
	if (!IsValid())
		return S_FALSE;

	m_pPage = m_pDocument->AddPage();

	if (!m_pPage)
	{
		SetError();
		return S_FALSE;
	}

	m_pPage->SetWidth(m_dPageWidth);
	m_pPage->SetHeight(m_dPageHeight);

	m_oPen.Reset();

	return S_OK;
}
HRESULT CPdfRenderer::get_Height(double* dHeight)
{
	*dHeight = m_dPageHeight;
	return S_OK;
}
HRESULT CPdfRenderer::put_Height(const double& dHeight)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageHeight = dHeight;
	m_pPage->SetHeight(MM_2_PT(dHeight));
	return S_OK;
}
HRESULT CPdfRenderer::get_Width(double* dWidth)
{
	*dWidth = m_dPageWidth;
	return S_OK;
}
HRESULT CPdfRenderer::put_Width(const double& dWidth)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageWidth = dWidth;
	m_pPage->SetWidth(MM_2_PT(dWidth));
	return S_OK;
}
HRESULT CPdfRenderer::get_DpiX(double* dDpiX)
{
	*dDpiX = 72;
	return S_OK;
}
HRESULT CPdfRenderer::get_DpiY(double* dDpiY)
{
	*dDpiY = 72;
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Pen
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.GetColor();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenColor(const LONG& lColor)
{
	m_oPen.SetColor(lColor);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.GetAlpha();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.SetAlpha(lAlpha);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenSize(double* dSize)
{
	*dSize = m_oPen.GetSize();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenSize(const double& dSize)
{
	m_oPen.SetSize(dSize);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
	*nDashStyle = m_oPen.GetDashStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
	m_oPen.SetDashStyle(nDashStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetStartCapStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
	m_oPen.SetStartCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetEndCapStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
	m_oPen.SetEndCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
	*nJoinStyle = m_oPen.GetJoinStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
	m_oPen.SetJoinStyle(nJoinStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenDashOffset(double* dOffset)
{
	*dOffset = m_oPen.GetDashOffset();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenDashOffset(const double& dOffset)
{
	m_oPen.SetDashOffset(dOffset);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenAlign(LONG* lAlign)
{
	*lAlign = m_oPen.GetAlign();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenAlign(const LONG& lAlign)
{
	m_oPen.SetAlign(lAlign);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenMiterLimit(double* dMiter)
{
	*dMiter = m_oPen.GetMiter();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenMiterLimit(const double& dMiter)
{
	m_oPen.SetMiter(dMiter);
	return S_OK;
}
HRESULT CPdfRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	m_oPen.SetDashPattern(pPattern, lCount);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Brush
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.GetType();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushType(const LONG& lType)
{
	m_oBrush.SetType(lType);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.GetColor1();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushColor1(const LONG& lColor)
{
	m_oBrush.SetColor1(lColor);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetAlpha1();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.SetAlpha1(lAlpha);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.GetColor2();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.SetColor2(lColor);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetAlpha2();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.SetAlpha2(lAlpha);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
	*wsPath = m_oBrush.GetTexturePath();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
	m_oBrush.SetTexturePath(wsPath);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.GetTextureMode();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.SetTextureMode(lMode);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetTextureAlpha();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
	m_oBrush.SetTextureAlpha(lAlpha);
	return S_OK;
}
HRESULT CPdfRenderer::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.GetLinearAngle();
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.SetLinearAngle(dAngle);
	return S_OK;
}
HRESULT CPdfRenderer::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	// TODO: Пока определяется все по границам пата
	return S_OK;
}
HRESULT CPdfRenderer::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	// TODO: Пока определяется все по границам пата
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG lCount)
{
	m_oBrush.SetShadingColors(lColors, pPositions, lCount);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со шрифтами
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_FontName(std::wstring* wsName)
{
	*wsName = m_oFont.GetName();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontName(const std::wstring& wsName)
{
	m_oFont.SetName(wsName);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontPath(std::wstring* wsPath)
{
	*wsPath = m_oFont.GetPath();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontPath(const std::wstring& wsPath)
{
	m_oFont.SetPath(wsPath);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontSize(double* dSize)
{
	*dSize = m_oFont.GetSize();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontSize(const double& dSize)
{
	m_oFont.SetSize(dSize);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontStringGID(INT* bGid)
{
	*bGid = m_oFont.GetGid() ? 1 : 0;
	return S_OK;
}
HRESULT CPdfRenderer::put_FontStringGID(const INT& bGid)
{
	m_oFont.SetGid(bGid ? true : false);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.GetCharSpace();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontCharSpace(const double& dSpace)
{
	m_oFont.SetCharSpace(dSpace);
	return S_OK;
}
HRESULT CPdfRenderer::get_FontFaceIndex(int* nFaceIndex)
{
	*nFaceIndex = (int)m_oFont.GetFaceIndex();
	return S_OK;
}
HRESULT CPdfRenderer::put_FontFaceIndex(const int& nFaceIndex)
{
	m_oFont.SetFaceIndex(nFaceIndex);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для вывода текста
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset)
{
	// TODO: Реализовать
	return S_OK;
}
HRESULT CPdfRenderer::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset)
{
	if (!IsPageValid() || !wsUnicodeText.size())
		return S_FALSE;

	m_pPage->GrSave();
	UpdateTransform();
	UpdateFont();
	UpdatePen();
	UpdateBrush();

	if (!m_pFont)
		return S_FALSE;

	unsigned int* pUnicodes = new unsigned int[wsUnicodeText.size()];
	if (!pUnicodes)
		return S_FALSE;

	unsigned int* pOutput = pUnicodes;
	unsigned int unLen = 0;
	if (2 == sizeof(wchar_t))
	{
		const wchar_t* wsEnd = wsUnicodeText.c_str() + wsUnicodeText.size();
		wchar_t* wsInput = (wchar_t*)wsUnicodeText.c_str();

		wchar_t wLeading, wTrailing;
		unsigned int unCode;
		while (wsInput < wsEnd)
		{
			wLeading = *wsInput++;
			if (wLeading < 0xD800 || wLeading > 0xDFFF)
			{
				pUnicodes[unLen++] = (unsigned int)wLeading;
			}
			else if (wLeading >= 0xDC00)
			{
				// Такого не должно быть
				continue;
			}
			else
			{
				unCode = (wLeading & 0x3FF) << 10;
				wTrailing = *wsInput++;
				if (wTrailing < 0xDC00 || wTrailing > 0xDFFF)
				{
					// Такого не должно быть
					continue;
				}
				else
				{
					pUnicodes[unLen++] = (unCode | (wTrailing & 0x3FF) + 0x10000);
				}
			}
		}
	}
	else
	{
		unLen = wsUnicodeText.size();
		for (unsigned int unIndex = 0; unIndex < unLen; unIndex++)
		{
			pUnicodes[unIndex] = (unsigned int)wsUnicodeText.at(unIndex);
		}		
	}

	unsigned char* pCodes = m_pFont->EncodeString(pUnicodes, unLen);
	delete[] pUnicodes;

	m_pPage->BeginText();

	m_pPage->SetFontAndSize(m_pFont, m_oFont.GetSize());
	m_pPage->DrawText(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), pCodes, unLen * 2);

	m_pPage->EndText();
	m_pPage->GrRestore();

	return S_OK;
}
HRESULT CPdfRenderer::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	// TODO: Реализовать
	return S_OK;
}
HRESULT CPdfRenderer::CommandDrawTextEx(const std::wstring& wsUnicodeText, const std::wstring& wsGidText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	// TODO: Реализовать
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Маркеры команд
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::BeginCommand(const DWORD& dwType)
{
	// Здесь мы ничего не делаем
	return S_OK;
}
HRESULT CPdfRenderer::EndCommand(const DWORD& dwType)
{
	// Здесь мы различаем лишь 2 команды: присоединить текущий пат к клипу и отменить клип
	// TODO: Реализовать
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с патом
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::PathCommandStart()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandEnd()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CPdfRenderer::DrawPath(const LONG& lType)
{
	if (!IsPageValid())
		return S_FALSE;

	m_pPage->GrSave();
	UpdateTransform();
	UpdatePen();
	UpdateBrush();

	// TODO: Реализовать

	bool bStroke = LONG_2_BOOL(lType & c_nStroke);
	bool bFill   = LONG_2_BOOL(lType & c_nWindingFillMode);
	bool bEoFill = LONG_2_BOOL(lType & c_nEvenOddFillMode);

	m_oPath.Draw(m_pPage, bStroke, bFill, bEoFill);
	m_pPage->GrRestore();

	return S_OK;
}
HRESULT CPdfRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
	m_oPath.MoveTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
	m_oPath.LineTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
	if (nCount < 4 || !pPoints)
		return S_OK;

	if (!m_oPath.IsMoveTo())
		m_oPath.MoveTo(MM_2_PT(pPoints[0]), MM_2_PT(m_dPageHeight - pPoints[1]));

	int nPointsCount = (nCount / 2) - 1;
	for (int nIndex = 1; nIndex <= nPointsCount; ++nIndex)
	{
		m_oPath.LineTo(MM_2_PT(pPoints[nIndex * 2]), MM_2_PT(m_dPageHeight - pPoints[nIndex * 2 + 1]));
	}

	return S_OK;
}
HRESULT CPdfRenderer::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
	m_oPath.CurveTo(MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), MM_2_PT(dX2), MM_2_PT(m_dPageHeight - dY2), MM_2_PT(dXe), MM_2_PT(m_dPageHeight - dYe));
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
	if (nCount < 8 || !pPoints)
		return S_OK;

	if (!m_oPath.IsMoveTo())
		m_oPath.MoveTo(MM_2_PT(pPoints[0]), MM_2_PT(m_dPageHeight - pPoints[1]));

	int nPointsCount = (nCount - 2) / 6;
	double* pCur = pPoints + 2;
	for (int nIndex = 0; nIndex <= nPointsCount; ++nIndex, pCur += 6)
	{
		m_oPath.CurveTo(MM_2_PT(pCur[0]), MM_2_PT(m_dPageHeight - pCur[1]), MM_2_PT(pCur[2]), MM_2_PT(m_dPageHeight - pCur[3]), MM_2_PT(pCur[4]), MM_2_PT(m_dPageHeight - pCur[5]));
	}

	return S_OK;
}
HRESULT CPdfRenderer::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
	m_oPath.ArcTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH), dStartAngle, dSweepAngle);
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandClose()
{
	m_oPath.Close();
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	m_oPath.GetLastPoint(*dX, *dY);
	*dX = PT_2_MM(*dX);
	*dY = PT_2_MM(*dY);
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset)
{
	m_oPath.AddText(m_oFont, lUnicode, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), MM_2_PT(dW), MM_2_PT(dH), MM_2_PT(dBaselineOffset));
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandText(const std::wstring& wsText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset)
{
	m_oPath.AddText(m_oFont, wsText, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), MM_2_PT(dW), MM_2_PT(dH), MM_2_PT(dBaselineOffset));
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	m_oPath.AddText(m_oFont, lUnicode, lGid, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), MM_2_PT(dW), MM_2_PT(dH), MM_2_PT(dBaselineOffset), dwFlags);
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandTextEx(const std::wstring& wsUnicodeText, const std::wstring& wsGidText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	m_oPath.AddText(m_oFont, wsUnicodeText, wsGidText, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), MM_2_PT(dW), MM_2_PT(dH), MM_2_PT(dBaselineOffset), dwFlags);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для вывода изображений
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid() || !pImage)
		return S_OK;

	if (!DrawImage((Aggplus::CImage*)pImage, dX, dY, dW, dH, 255))
		return S_FALSE;

	return S_OK;
}
HRESULT CPdfRenderer::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	if (!IsPageValid())
		return S_OK;

	Aggplus::CImage oAggImage(wsImagePath);
	if (!DrawImage(&oAggImage, dX, dY, dW, dH, nAlpha))
		return S_FALSE;

	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для выставления преобразования
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
	m_oTransform.Set(dM11, dM12, dM21, dM22, dX, dY);
	return S_OK;
}
HRESULT CPdfRenderer::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
	*dM11 = m_oTransform.m11;
	*dM12 = m_oTransform.m12;
	*dM21 = m_oTransform.m21;
	*dM22 = m_oTransform.m22;
	*dX   = m_oTransform.dx;
	*dY   = m_oTransform.dy;
	return S_OK;
}
HRESULT CPdfRenderer::ResetTransform()
{
	m_oTransform.Reset();
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Тип клипа
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_ClipMode(LONG* lMode)
{
	*lMode = m_lClipMode;
	return S_OK;
}
HRESULT CPdfRenderer::put_ClipMode(const LONG& lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Дополнительные функции
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
	return S_OK;
}
HRESULT CPdfRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
	return S_OK;
}
HRESULT CPdfRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Дополнительные функции Pdf рендерера
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::CommandDrawTextPdf(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const std::wstring& wsSrcCodeText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	return S_OK;
}
HRESULT CPdfRenderer::PathCommandTextPdf(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const std::wstring& bsSrcCodeText, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaselineOffset, const DWORD& dwFlags)
{
	return S_OK;
}
HRESULT CPdfRenderer::DrawImage1bpp(Pix* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid() || !pImageBuffer)
		return S_OK;

	m_pPage->GrSave();
	UpdateTransform();
	
	CImageDict* pPdfImage = m_pDocument->CreateImage();
	pPdfImage->LoadBW(pImageBuffer, unWidth, unHeight);
	m_pPage->DrawImage(pPdfImage, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH));

	m_pPage->GrRestore();
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Внутренние функции
//----------------------------------------------------------------------------------------
bool CPdfRenderer::DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	int nImageW = abs((int)pImage->GetWidth());
	int nImageH = abs((int)pImage->GetHeight());
	BYTE* pData = pImage->GetData();
	int nStride = 4 * nImageW;

	// Пробегаемся по картинке и определяем есть ли у нас альфа-канал
	bool bAlpha = false;
	for (int nIndex = 0, nSize = nImageW * nImageH; nIndex < nSize; nIndex++)
	{
		if (pData[4 * nIndex + 3] < 255)
		{
			bAlpha = true;
			break;
		}
	}

	CxImage oCxImage;
	if (!oCxImage.CreateFromArray(pData, nImageW, nImageH, 32, nStride, (nStride >= 0) ? true : false))
		return false;

	BYTE* pBuffer = NULL;
	int nBufferSize = 0;
	if (!oCxImage.Encode(pBuffer, nBufferSize, CXIMAGE_FORMAT_JP2))
		return false;

	if (!pBuffer || !nBufferSize)
		return false;

	CImageDict* pPdfImage = m_pDocument->CreateImage();
	if (bAlpha || nAlpha < 255)
		pPdfImage->LoadSMask(pData, nImageW, nImageH, nAlpha);

	pPdfImage->LoadJpx(pBuffer, nBufferSize, nImageW, nImageH);
	m_pPage->GrSave();
	UpdateTransform();
	m_pPage->DrawImage(pPdfImage, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH));
	m_pPage->GrRestore();
	free(pBuffer);

	return true;
}
void CPdfRenderer::UpdateFont()
{
	std::wstring& wsFontPath = m_oFont.GetPath();
	LONG lFaceIndex = m_oFont.GetFaceIndex();
	if (L"" == wsFontPath)
	{
		CFontSelectFormat oFontSelect;
		oFontSelect.wsName = new std::wstring(m_oFont.GetName());
		oFontSelect.bItalic = new INT(m_oFont.IsItalic() ? 1 : 0);
		oFontSelect.bBold   = new INT(m_oFont.IsBold() ? 1 : 0);
		CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelect);

		wsFontPath = pFontInfo->m_wsFontPath;
		lFaceIndex = pFontInfo->m_lIndex;
	}

	m_pFont = NULL;
	if (L"" != wsFontPath)
	{
		// TODO: Пока мы здесь предполагаем, что шрифты только либо TrueType, либо OpenType
		LONG lFaceIndex = m_oFont.GetFaceIndex();
		m_pFontManager->LoadFontFromFile(wsFontPath, lFaceIndex, 10, 72, 72);
		std::wstring wsFontType = m_pFontManager->GetFontType();
		if (L"TrueType" == wsFontType || L"OpenType" == wsFontType || L"CFF" == wsFontType)
			m_pFont = m_pDocument->CreateTrueTypeFont(wsFontPath, lFaceIndex);
	}
}
void CPdfRenderer::UpdateTransform()
{
	CTransform& t = m_oTransform;
	m_pPage->Concat(t.m11, -t.m12, -t.m21, t.m22, MM_2_PT(t.dx + t.m21 * m_dPageHeight), MM_2_PT(m_dPageHeight - m_dPageHeight * t.m22 - t.dy));
}
void CPdfRenderer::UpdatePen()
{
	TColor& oColor = m_oPen.GetTColor();
	m_pPage->SetStrokeColor(oColor.r, oColor.g, oColor.b);
	m_pPage->SetLineWidth(MM_2_PT(m_oPen.GetSize()));
	
	LONG lDashCount = 0;
	double* pDashPattern = NULL;
	
	LONG lDashStyle = m_oPen.GetDashStyle();
	if (Aggplus::DashStyleSolid == lDashStyle)
	{
		// Ничего не делаем
	}
	else if (Aggplus::DashStyleCustom == lDashStyle)
	{
		double *pDashPatternMM = m_oPen.GetDashPattern(lDashCount);
		if (pDashPatternMM && lDashCount)
		{
			pDashPattern = new double[lDashCount];
			if (pDashPattern)
			{
				for (LONG lIndex = 0; lIndex < lDashCount; lIndex++)
				{
					pDashPattern[lIndex] = MM_2_PT(pDashPatternMM[lIndex]);
				}
			}
		}
	}
	else
	{
		// TODO: Реализовать другие типы пунктирных линий
	}

	if (pDashPattern && lDashCount)
	{
		m_pPage->SetDash(pDashPattern, lDashCount, MM_2_PT(m_oPen.GetDashOffset()));
		delete[] pDashPattern;
	}


	// TODO: Реализовать
}
void CPdfRenderer::UpdateBrush()
{
	TColor& oColor1 = m_oBrush.GetTColor1();
	m_pPage->SetFillColor(oColor1.r, oColor1.g, oColor1.b);
	// TODO: Реализовать
}



void CPdfRenderer::CPath::Draw(PdfWriter::CPage* pPage, bool bStroke, bool bFill, bool bEoFill)
{
	for (int nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
	{
		CPathCommandBase* pCommand = m_vCommands.at(nIndex);
		pCommand->Draw(pPage);
	}

	if (bStroke && !bFill && !bEoFill)
		pPage->Stroke();
	else if (bStroke && bFill)
		pPage->FillStroke();
	else if (bStroke && bEoFill)
		pPage->EoFill();
	else if (bFill)
		pPage->Fill();
	else if (bEoFill)
		pPage->EoFill();
	else
		pPage->EndPath();
}
void CPdfRenderer::CPath::CPathMoveTo::Draw(PdfWriter::CPage* pPage)
{
	pPage->MoveTo(x, y);
}
void CPdfRenderer::CPath::CPathLineTo::Draw(PdfWriter::CPage* pPage)
{
	pPage->LineTo(x, y);
}
void CPdfRenderer::CPath::CPathCurveTo::Draw(PdfWriter::CPage* pPage)
{
	pPage->CurveTo(x1, y1, x2, y2, xe, ye);
}
void CPdfRenderer::CPath::CPathArcTo::Draw(PdfWriter::CPage* pPage)
{
	pPage->EllipseArcTo(x + w / 2, y + h / 2, w / 2, h / 2, startAngle, sweepAngle);
}
void CPdfRenderer::CPath::CPathClose::Draw(PdfWriter::CPage* pPage)
{
	pPage->ClosePath();
}
void CPdfRenderer::CPath::CPathTextChar::Draw(PdfWriter::CPage* pPage)
{
	// TODO: Реализовать
}
void CPdfRenderer::CPath::CPathText::Draw(PdfWriter::CPage* pPage)
{
	// TODO: Реализовать
}
void CPdfRenderer::CPath::CPathTextExChar::Draw(PdfWriter::CPage* pPage)
{
	// TODO: Реализовать
}
void CPdfRenderer::CPath::CPathTextEx::Draw(PdfWriter::CPage* pPage)
{
	// TODO: Реализовать
}