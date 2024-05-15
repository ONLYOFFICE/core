/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#include "AnnotRenderer.h"

#include "../../DesktopEditor/graphics/GraphicsPath.h"

#define MM_2_PT(X) ((X) * 72.0 / 25.4)
#define PT_2_MM(X) ((X) * 25.4 / 72.0)

#define LONG_2_BOOL(X) ((X) ? true : false)

namespace PdfWriter
{
CAnnotRenderer::CAnnotRenderer(NSFonts::IApplicationFonts* pAppFonts)
{
	m_pAppFonts = pAppFonts;
	m_bNeedUpdateTextFont = true;
}
CAnnotRenderer::~CAnnotRenderer()
{

}

// тип рендерера-----------------------------------------------------------------------------
HRESULT CAnnotRenderer::get_Type(LONG* lType)
{
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT CAnnotRenderer::NewPage()
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_Height(double* dHeight)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_Height(const double& dHeight)
{
	m_dPageHeight = dHeight;
	return S_OK;
}
HRESULT CAnnotRenderer::get_Width(double* dWidth)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_Width(const double& dWidth)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_DpiX(double* dDpiX)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_DpiY(double* dDpiY)
{
	return S_OK;
}

// pen --------------------------------------------------------------------------------------
HRESULT CAnnotRenderer::get_PenColor(LONG* lColor)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenColor(const LONG& lColor)
{
	m_oPen.SetColor(lColor);
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenAlpha(LONG* lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenAlpha(const LONG& lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenSize(double* dSize)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenSize(const double& dSize)
{
	m_oPen.SetSize(dSize);
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenDashOffset(double* dOffset)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenDashOffset(const double& dOffset)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenAlign(LONG* lAlign)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenAlign(const LONG& lAlign)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_PenMiterLimit(double* dMiter)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_PenMiterLimit(const double& dMiter)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	return S_OK;
}

// brush ------------------------------------------------------------------------------------
HRESULT CAnnotRenderer::get_BrushType(LONG* lType)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushType(const LONG& lType)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushColor1(LONG* lColor)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushColor1(const LONG& lColor)
{
	if (lColor != m_oBrush.GetColor1())
		m_oBrush.SetColor1(lColor);
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushColor2(LONG* lColor)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushColor2(const LONG& lColor)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushTextureImage(Aggplus::CImage** pImage)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushTextureImage(Aggplus::CImage* pImage)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushTextureMode(LONG* lMode)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushTextureMode(const LONG& lMode)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushTransform(Aggplus::CMatrix& oMatrix)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushTransform(const Aggplus::CMatrix& oMatrix)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_BrushLinearAngle(double* dAngle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushLinearAngle(const double& dAngle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
	return S_OK;
}
HRESULT CAnnotRenderer::BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount)
{
	return S_OK;
}

// font -------------------------------------------------------------------------------------
HRESULT CAnnotRenderer::get_FontName(std::wstring* wsName)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontName(const std::wstring& wsName)
{
	if (wsName != m_oFont.GetName())
	{
		m_oFont.SetName(wsName);
		m_bNeedUpdateTextFont = true;
	}
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontPath(std::wstring* wsPath)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontPath(const std::wstring& wsPath)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontSize(double* dSize)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontSize(const double& dSize)
{
	if (fabs(dSize - m_oFont.GetSize()) > 0.001)
		m_oFont.SetSize(dSize);
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontStyle(LONG* lStyle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontStyle(const LONG& lStyle)
{
	if (lStyle != m_oFont.GetStyle())
	{
		m_oFont.SetStyle(lStyle);
		m_bNeedUpdateTextFont = true;
	}
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontStringGID(INT* bGid)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontStringGID(const INT& bGid)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontCharSpace(double* dSpace)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontCharSpace(const double& dSpace)
{
	return S_OK;
}
HRESULT CAnnotRenderer::get_FontFaceIndex(int* lFaceIndex)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
	return S_OK;
}

//-------- Функции для вывода текста --------------------------------------------------------
HRESULT CAnnotRenderer::CommandDrawTextCHAR  (const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandDrawText      (const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandDrawTextCHAR2 (unsigned int* pUnicodes, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	unsigned char* pCodes = EncodeGID(unGid, pUnicodes, unUnicodeCount);
	if (!pCodes)
		return DrawTextToRenderer(&unGid, 1, dX, dY) ? S_OK : S_FALSE;
	return DrawText(pCodes, 2, dX, dY) ? S_OK : S_FALSE;
}

//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT CAnnotRenderer::BeginCommand(const DWORD& lType)
{
	return S_OK;
}
HRESULT CAnnotRenderer::EndCommand(const DWORD& dwType)
{
	/*
	if (c_nClipType == dwType)
	{
		m_oCommandManager.Flush();
		m_pPage->GrSave();
		m_lClipDepth++;
		UpdateTransform();

		m_oPath.Clip(m_pPage, c_nClipRegionTypeEvenOdd & m_lClipMode);
	}
	else if (c_nResetClipType == dwType)
	{
		m_oCommandManager.Flush();
		while (m_lClipDepth)
		{
			m_pPage->GrRestore();
			m_lClipDepth--;
		}
	}
	*/
	return S_OK;
}

//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT CAnnotRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
	m_oPath.MoveTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
	m_oPath.LineTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
	m_oPath.CurveTo(MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), MM_2_PT(dX2), MM_2_PT(m_dPageHeight - dY2), MM_2_PT(dXe), MM_2_PT(m_dPageHeight - dYe));
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandClose()
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandEnd()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CAnnotRenderer::DrawPath(const LONG& lType)
{
	/*
	m_oCommandManager.Flush();

	bool bStroke = LONG_2_BOOL(lType & c_nStroke);
	bool bFill   = LONG_2_BOOL(lType & c_nWindingFillMode);
	bool bEoFill = LONG_2_BOOL(lType & c_nEvenOddFillMode);

	m_pPage->GrSave();
	UpdateTransform();

	if (bStroke)
		UpdatePen();

	std::wstring sTextureOldPath = L"";
	std::wstring sTextureTmpPath = L"";

	if (bFill || bEoFill)
	{
		if (c_BrushTypeTexture == m_oBrush.GetType())
		{
			sTextureOldPath = m_oBrush.GetTexturePath();
			sTextureTmpPath = GetDownloadFile(sTextureOldPath, wsTempDirectory);

			if (!sTextureTmpPath.empty())
				m_oBrush.SetTexturePath(sTextureTmpPath);
		}

		UpdateBrush(pAppFonts, wsTempDirectory);
	}

	if (!m_pShading)
	{
		m_oPath.Draw(m_pPage, bStroke, bFill, bEoFill);
	}
	else
	{
		if (bFill || bEoFill)
		{
			m_pPage->GrSave();
			m_oPath.Clip(m_pPage, bEoFill);

			if (NULL != m_pShadingExtGrState)
				m_pPage->SetExtGrState(m_pShadingExtGrState);

			m_pPage->DrawShading(m_pShading);
			m_pPage->GrRestore();
		}

		if (bStroke)
			m_oPath.Draw(m_pPage, bStroke, false, false);
	}

	m_pPage->GrRestore();

	if (!sTextureTmpPath.empty())
	{
		m_oBrush.SetTexturePath(sTextureOldPath);

		if (NSFile::CFileBinary::Exists(sTextureTmpPath))
			NSFile::CFileBinary::Remove(sTextureTmpPath);
	}

	*/
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandStart()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandTextCHAR  (const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandText      (const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}

//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT CAnnotRenderer::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
	return S_OK;
}
HRESULT CAnnotRenderer::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	return S_OK;
}

// transform --------------------------------------------------------------------------------
HRESULT CAnnotRenderer::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
	m_oTransform.Set(dM11, dM12, dM21, dM22, dX, dY);
	return S_OK;
}
HRESULT CAnnotRenderer::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
	return S_OK;
}
HRESULT CAnnotRenderer::ResetTransform()
{
	return S_OK;
}

// clip ------------------------------------------------------------------------------------
HRESULT CAnnotRenderer::get_ClipMode(LONG* lMode)
{
	return S_OK;
}
HRESULT CAnnotRenderer::put_ClipMode(const LONG& lMode)
{
	return S_OK;
}

// additiaonal params ----------------------------------------------------------------------
HRESULT CAnnotRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
	return S_OK;
}
HRESULT CAnnotRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	return S_OK;
}

// внутренние функции ----------------------------------------------------------------------
bool CAnnotRenderer::DrawText(unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY)
{
	return true;
}
bool CAnnotRenderer::DrawTextToRenderer(const unsigned int* unGid, const unsigned int& unLen, const double& dX, const double& dY)
{
	Aggplus::CGraphicsPathSimpleConverter simplifier;
	simplifier.SetRenderer(this);
	m_pFontManager->LoadFontByName(m_oFont.GetName(), m_oFont.GetSize(), (int)m_oFont.GetStyle(), 72.0, 72.0);
	PathCommandEnd();
	if (simplifier.PathCommandText2(L"", (const int*)unGid, unLen, m_pFontManager, dX, dY, 0, 0))
	{
		DrawPath(c_nWindingFillMode);
		PathCommandEnd();
		return true;
	}
	return false;
}
bool CAnnotRenderer::UpdateFont()
{
	/*
	m_bNeedUpdateTextFont = false;
	std::wstring wsFontPath = m_oFont.GetPath();
	LONG lFaceIndex         = m_oFont.GetFaceIndex();
	if (L"" == wsFontPath)
	{
		if (!GetFontPath(m_oFont.GetName(), m_oFont.IsBold(), m_oFont.IsItalic(), wsFontPath, lFaceIndex))
		{
			m_pFont = NULL;
			return false;
		}
	}

	m_oFont.SetNeedDoBold(false);
	m_oFont.SetNeedDoItalic(false);

	m_pFont = NULL;
	if (L"" != wsFontPath)
	{
		m_pFont = GetFont(wsFontPath, lFaceIndex);
		if (m_pFont)
		{
			if (m_oFont.IsItalic() && !m_pFont->IsItalic())
				m_oFont.SetNeedDoItalic(true);

			if (m_oFont.IsBold() && !m_pFont->IsBold())
				m_oFont.SetNeedDoBold(true);
		}
		else
			return false;
	}
	*/
	return true;
}
void UpdateTransform()
{

}
unsigned char* CAnnotRenderer::EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unUnicodesCount)
{
	if (m_bNeedUpdateTextFont)
	{
		if (!UpdateFont())
			return NULL;
	}

	if (!m_pFont)
		return NULL;

	unsigned char* pCodes = new unsigned char[2];
	if (!pCodes)
		return NULL;

	unsigned short ushCode = m_pFont->EncodeGID(unGID, pUnicodes, unUnicodesCount);
	pCodes[0] = (ushCode >> 8) & 0xFF;
	pCodes[1] = ushCode & 0xFF;
	return pCodes;
}
}
