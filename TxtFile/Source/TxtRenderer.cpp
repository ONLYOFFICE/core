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

#include "TxtRenderer.h"

#include <vector>

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/common/StringExt.h"
#include "../../../DesktopEditor/common/StringUTF32.h"

static inline bool IsUnicodeSymbol(unsigned int cSym)
{
	bool is_unicode =
	        (( 0x0009 == cSym) || (0x000A == cSym ) || (0x000D == cSym ) ||
	         (( 0x0020 <= cSym) && (0xD7FF >= cSym )) || ((0xE000 <= cSym) && (cSym <= 0xFFFD )) ||
	         (( 0x10000 <= cSym) && cSym));

	return is_unicode;
}

static inline bool IsUnicodeDiacriticalMark(unsigned int cSym)
{
	return 0x0300 <= cSym && 0x036F >= cSym;
}

class CTxtRenderer::CTxtRendererImpl
{
public:
	CTxtRendererImpl() = default;
	CTxtRendererImpl(const CTxtRendererImpl& other) = delete;
	CTxtRendererImpl(CTxtRendererImpl&& other) = delete;
	virtual ~CTxtRendererImpl();

	void BeginCommand(const DWORD& lType);
	void EndCommand  (const DWORD& lType);
	void NewPage     ();
	void Save        (const std::wstring& wsDstTxtFile);
	void AddText     (const unsigned int* pUnicodes, unsigned int nLen, double dX, double dY, double dW, double dH);

	Aggplus::CMatrix    m_oTransform{};
	NSStructures::CFont m_oFont{};

	double m_dWidth = 0.0;
	double m_dHeight = 0.0;

	double m_dDpiX = c_dDpiX;
	double m_dDpiY = c_dDpiY;

	bool m_bIsFontSizeDecrease = false;

private:
	struct CTextLine
	{
		double dTop;
		double dBot;
		double dLeft;
		double dRight;
		std::wstring wsData;

		bool IsOnlySpaces() const
		{
			for (const auto& sym : wsData)
				if (sym != L' ')
					return false;
			return true;
		}
	};
	std::vector<std::shared_ptr<CTextLine>> m_arCurrPageTextLines{};
	std::vector<std::wstring> m_arTxtData{};
	std::shared_ptr<CTextLine> m_pCurrLine{nullptr};
};

CTxtRenderer::CTxtRendererImpl::~CTxtRendererImpl()
{
}
void CTxtRenderer::CTxtRendererImpl::BeginCommand(const DWORD& lType)
{
}
void CTxtRenderer::CTxtRendererImpl::EndCommand(const DWORD& lType)
{
	if (lType == c_nPageType)
	{
		for (auto& line : m_arCurrPageTextLines)
			    if (!line->IsOnlySpaces())
					m_arTxtData.push_back(std::move(line->wsData));
		m_arTxtData.push_back(L" ");
	}
}
void CTxtRenderer::CTxtRendererImpl::NewPage()
{
	m_oTransform.Reset();
	m_arCurrPageTextLines.clear();
	m_pCurrLine = nullptr;
}
void CTxtRenderer::CTxtRendererImpl::Save(const std::wstring& wsDstTxtFile)
{
	NSStringUtils::CStringBuilder str_builder;
	str_builder.AddSize(1000);
	for (const auto& txt_str : m_arTxtData)
	{
		str_builder.WriteString(txt_str);
		str_builder.WriteString(L"\n");
	}
	NSFile::CFileBinary::SaveToFile(wsDstTxtFile, str_builder.GetData());
	m_arTxtData.clear();
}
void CTxtRenderer::CTxtRendererImpl::AddText(const unsigned int* pUnicodes, unsigned int nLen, double dX, double dY, double dW, double dH)
{
	// 9 - \t
	if (nLen == 1 && (*pUnicodes == 9 || IsUnicodeDiacriticalMark(*pUnicodes)))
		return;

	double l = dX;
	double b = dY;
	double t = dY - dH;
	double r = dX + dW;

	m_oTransform.TransformPoint(l, b);
	m_oTransform.TransformPoint(r, t);

	// out of page
	if (l > m_dWidth || r < 0 || b > m_dHeight || t < 0)
		return;

	double w = r - l;

	NSStringUtils::CStringUTF32 text_utf32(pUnicodes, nLen);
	if (pUnicodes != nullptr)
		for (unsigned int i = 0; i < nLen; ++i)
			if (!IsUnicodeSymbol(pUnicodes[i]))
				text_utf32[i] = ' ';

	const double split_distance = w * c_dSPLIT_WIDTH_COEF;
	const double space_distance = w * c_dSPACE_WIDTH_COEF;
	double y_precision = c_dY_PRECISION_MM;
	if (m_bIsFontSizeDecrease) y_precision *= 3; // for posible super/sub script

	std::wstring new_text = text_utf32.ToStdWString();
	if (m_pCurrLine != nullptr && fabs(m_pCurrLine->dBot - b) < y_precision)
	{
		// some_text+more_text
		if (fabs(m_pCurrLine->dRight - l) < split_distance && r - m_pCurrLine->dRight > c_dX_PRECISION_MM)
		{
			if (fabs(m_pCurrLine->dRight - l) > space_distance)
				new_text = L" " + new_text;

			m_pCurrLine->wsData += new_text;
			m_pCurrLine->dRight = r;
		}
		// more_text+some_text
		else if (fabs(m_pCurrLine->dLeft - r) < split_distance && m_pCurrLine->dLeft - l > c_dX_PRECISION_MM)
		{
			if (fabs(m_pCurrLine->dLeft - r) > space_distance)
				new_text += L" ";

			m_pCurrLine->wsData = new_text + m_pCurrLine->wsData;
			m_pCurrLine->dLeft = l;
		}
	}
	else
	{
		m_pCurrLine = std::make_shared<CTextLine>();
		m_pCurrLine->dTop = t;
		m_pCurrLine->dBot = b;
		m_pCurrLine->dLeft = l;
		m_pCurrLine->dRight = r;
		m_pCurrLine->wsData.reserve(100);
		m_pCurrLine->wsData = new_text;
		m_arCurrPageTextLines.push_back(m_pCurrLine);
	}
}

CTxtRenderer::CTxtRenderer()
    : m_pImpl(std::unique_ptr<CTxtRendererImpl>(new CTxtRendererImpl()))
{
}
CTxtRenderer::~CTxtRenderer()
{
}

// base settings
HRESULT CTxtRenderer::get_Type(LONG* lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTxtRenderer::NewPage()
{
	m_pImpl->NewPage();
	return S_OK;
}
HRESULT CTxtRenderer::get_Height(double* dHeight)
{
	*dHeight = m_pImpl->m_dHeight;
	return S_OK;
}
HRESULT CTxtRenderer::put_Height(const double& dHeight)
{
	m_pImpl->m_dHeight = dHeight;
	return S_OK;
}
HRESULT CTxtRenderer::get_Width(double* dWidth)
{
	*dWidth = m_pImpl->m_dWidth;
	return S_OK;
}
HRESULT CTxtRenderer::put_Width(const double& dWidth)
{
	m_pImpl->m_dWidth = dWidth;
	return S_OK;
}
HRESULT CTxtRenderer::get_DpiX(double* dDpiX)
{
	*dDpiX = m_pImpl->m_dDpiX;
	return S_OK;
}
HRESULT CTxtRenderer::get_DpiY(double* dDpiY)
{
	*dDpiY = m_pImpl->m_dDpiY;
	return S_OK;
}

// pen settings
HRESULT CTxtRenderer::get_PenColor(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenColor(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenAlpha(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenAlpha(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenSize(double* dSize)
{
	MAYBE_UNUSED(dSize);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenSize(const double& dSize)
{
	MAYBE_UNUSED(dSize);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
	MAYBE_UNUSED(nDashStyle);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
	MAYBE_UNUSED(nDashStyle);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
	MAYBE_UNUSED(nCapStyle);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
	MAYBE_UNUSED(nJoinStyle);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
	MAYBE_UNUSED(nJoinStyle);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenDashOffset(double* dOffset)
{
	MAYBE_UNUSED(dOffset);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenDashOffset(const double& dOffset)
{
	MAYBE_UNUSED(dOffset);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenAlign(LONG* lAlign)
{
	MAYBE_UNUSED(lAlign);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenAlign(const LONG& lAlign)
{
	MAYBE_UNUSED(lAlign);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_PenMiterLimit(double* dMiter)
{
	MAYBE_UNUSED(dMiter);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_PenMiterLimit(const double& dMiter)
{
	MAYBE_UNUSED(dMiter);
	return S_FALSE;
}

HRESULT CTxtRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	MAYBE_UNUSED(pPattern); MAYBE_UNUSED(lCount);
	return S_FALSE;
}

// brush settings
HRESULT CTxtRenderer::get_BrushType(LONG* lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushType(const LONG& lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushColor1(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushColor1(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushColor2(LONG* lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushColor2(const LONG& lColor)
{
	MAYBE_UNUSED(lColor);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
	MAYBE_UNUSED(wsPath);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
	MAYBE_UNUSED(wsPath);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushTextureMode(LONG* lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushTextureMode(const LONG& lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
	MAYBE_UNUSED(lAlpha);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushLinearAngle(double* dAngle)
{
	MAYBE_UNUSED(dAngle);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushLinearAngle(const double& dAngle)
{
	MAYBE_UNUSED(dAngle);
	return S_FALSE;
}

HRESULT CTxtRenderer::BrushRect(const INT& nVal,
                                        const double& dLeft,
                                        const double& dTop,
                                        const double& dWidth,
                                        const double& dHeight)
{
	MAYBE_UNUSED(nVal); MAYBE_UNUSED(dLeft); MAYBE_UNUSED(dTop);
	MAYBE_UNUSED(dWidth); MAYBE_UNUSED(dHeight);
	return S_FALSE;
}

HRESULT CTxtRenderer::BrushBounds(const double& dLeft,
                                          const double& dTop,
                                          const double& dWidth,
                                          const double& dHeight)
{
	MAYBE_UNUSED(dLeft); MAYBE_UNUSED(dTop); MAYBE_UNUSED(dWidth); MAYBE_UNUSED(dHeight);
	return S_FALSE;
}

HRESULT CTxtRenderer::put_BrushGradientColors(LONG* pColors,
                                                      double* pPositions,
                                                      LONG lCount)
{
	MAYBE_UNUSED(pColors); MAYBE_UNUSED(pPositions); MAYBE_UNUSED(lCount);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushTextureImage(Aggplus::CImage** pImage)
{
	MAYBE_UNUSED(pImage);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushTextureImage(Aggplus::CImage* pImage)
{
	MAYBE_UNUSED(pImage);
	return S_FALSE;
}

HRESULT CTxtRenderer::get_BrushTransform(Aggplus::CMatrix& oMatrix)
{
	MAYBE_UNUSED(oMatrix);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_BrushTransform(const Aggplus::CMatrix& oMatrix)
{
	MAYBE_UNUSED(oMatrix);
	return S_FALSE;
}

void CTxtRenderer::put_BrushGradInfo(void* pGradInfo)
{
	MAYBE_UNUSED(pGradInfo);
}

// font settings
HRESULT CTxtRenderer::get_FontName(std::wstring* wsName)
{
	*wsName = m_pImpl->m_oFont.Name;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontName(const std::wstring& wsName)
{
	m_pImpl->m_oFont.Name = wsName;
	return S_OK;
}

HRESULT CTxtRenderer::get_FontPath(std::wstring* wsPath)
{
	*wsPath = m_pImpl->m_oFont.Path;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontPath(const std::wstring& wsPath)
{
	m_pImpl->m_oFont.Path = wsPath;
	return S_OK;
}

HRESULT CTxtRenderer::get_FontSize(double* dSize)
{
	*dSize = m_pImpl->m_oFont.Size;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontSize(const double& dSize)
{
	if (m_pImpl->m_oFont.Size * c_dFONT_SIZE_DECREASE_COEF > dSize)
		m_pImpl->m_bIsFontSizeDecrease = true;

	if (m_pImpl->m_oFont.Size < dSize)
		m_pImpl->m_bIsFontSizeDecrease = false;

	m_pImpl->m_oFont.Size = dSize;
	return S_OK;
}

HRESULT CTxtRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_pImpl->m_oFont.GetStyle2();
	return S_OK;
}
HRESULT CTxtRenderer::put_FontStyle(const LONG& lStyle)
{
	m_pImpl->m_oFont.SetStyle(lStyle);
	return S_OK;
}

HRESULT CTxtRenderer::get_FontStringGID(INT* bGid)
{
	*bGid = m_pImpl->m_oFont.StringGID;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontStringGID(const INT& bGid)
{
	m_pImpl->m_oFont.StringGID = bGid;
	return S_OK;
}

HRESULT CTxtRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_pImpl->m_oFont.CharSpace;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontCharSpace(const double& dSpace)
{
	m_pImpl->m_oFont.CharSpace = dSpace;
	return S_OK;
}

HRESULT CTxtRenderer::get_FontFaceIndex(int* lFaceIndex)
{
	*lFaceIndex = m_pImpl->m_oFont.FaceIndex;
	return S_OK;
}
HRESULT CTxtRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
	m_pImpl->m_oFont.FaceIndex = lFaceIndex;
	return S_OK;
}

// text commands
HRESULT CTxtRenderer::CommandDrawTextCHAR(const LONG& lUnicode,
                                                  const double& dX,
                                                  const double& dY,
                                                  const double& dW,
                                                  const double& dH)
{
	m_pImpl->AddText(reinterpret_cast<const unsigned int*>(&lUnicode), 1, dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTxtRenderer::CommandDrawTextExCHAR(const LONG& lUnicode,
                                                    const LONG& lGid,
                                                    const double& dX,
                                                    const double& dY,
                                                    const double& dW,
                                                    const double& dH)
{
	m_pImpl->AddText(reinterpret_cast<const unsigned int*>(&lUnicode), 1, dX, dY, dW, dH);
	return S_OK;
}

HRESULT CTxtRenderer::CommandDrawText(const std::wstring& wsUnicodeText,
                                              const double& dX,
                                              const double& dY,
                                              const double& dW,
                                              const double& dH)
{
	unsigned int len = 0;
	unsigned int* unicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, len);

	if (len == 0)
		return S_OK;

	m_pImpl->AddText(unicodes, len, dX, dY, dW, dH);
	delete [] unicodes;
	return S_OK;
}

HRESULT CTxtRenderer::CommandDrawTextEx(const std::wstring& wsUnicodeText,
                                                const unsigned int* pGids,
                                                const unsigned int nGidsCount,
                                                const double& dX,
                                                const double& dY,
                                                const double& dW,
                                                const double& dH)
{
	unsigned int len = 0;
	unsigned int* unicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, len);

	if (len == 0)
		return S_OK;

	m_pImpl->AddText(unicodes, len, dX, dY, dW, dH);
	delete[] unicodes;
	return S_OK;
}

// command type
HRESULT CTxtRenderer::BeginCommand(const DWORD& lType)
{
	m_pImpl->BeginCommand(lType);
	return S_OK;
}
HRESULT CTxtRenderer::EndCommand(const DWORD& lType)
{
	m_pImpl->EndCommand(lType);
	return S_OK;
}

// graphic commands
HRESULT CTxtRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
	MAYBE_UNUSED(pPoints); MAYBE_UNUSED(nCount);
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandCurveTo(const double& dX1,
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

HRESULT CTxtRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
	MAYBE_UNUSED(pPoints); MAYBE_UNUSED(nCount);
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandArcTo(const double& dX,
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

HRESULT CTxtRenderer::PathCommandClose()
{
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandEnd()
{
	return S_FALSE;
}
HRESULT CTxtRenderer::DrawPath(const LONG& lType)
{
	MAYBE_UNUSED(lType);
	return S_FALSE;
}
HRESULT CTxtRenderer::PathCommandStart()
{
	return S_FALSE;
}

HRESULT CTxtRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	return S_FALSE;
}

HRESULT CTxtRenderer::PathCommandTextCHAR(const LONG& lUnicode,
                                                  const double& dX,
                                                  const double& dY,
                                                  const double& dW,
                                                  const double& dH)
{
	// TODO
	return S_OK;
}

HRESULT CTxtRenderer::PathCommandTextExCHAR(const LONG& lUnicode,
                                                    const LONG& lGid,
                                                    const double& dX,
                                                    const double& dY,
                                                    const double& dW,
                                                    const double& dH)
{
	return S_OK;
}

HRESULT CTxtRenderer::PathCommandText(const std::wstring& wsUnicodeText,
                                              const double& dX,
                                              const double& dY,
                                              const double& dW,
                                              const double& dH)
{
	return S_OK;
}

HRESULT CTxtRenderer::PathCommandTextEx(const std::wstring& wsUnicodeText,
                                                const unsigned int* pGids,
                                                const unsigned int nGidsCount,
                                                const double& dX,
                                                const double& dY,
                                                const double& dW,
                                                const double& dH)
{
	return S_OK;
}

// image commands
HRESULT CTxtRenderer::DrawImage(IGrObject* pImage,
                                        const double& dX,
                                        const double& dY,
                                        const double& dW,
                                        const double& dH)
{
	MAYBE_UNUSED(pImage); MAYBE_UNUSED(dX); MAYBE_UNUSED(dY);
	MAYBE_UNUSED(dW); MAYBE_UNUSED(dH);
	return S_FALSE;
}

HRESULT CTxtRenderer::DrawImageFromFile(const std::wstring& wsImagePath,
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
HRESULT CTxtRenderer::SetTransform(const double& dM11,
                                           const double& dM12,
                                           const double& dM21,
                                           const double& dM22,
                                           const double& dX,
                                           const double& dY)
{
	m_pImpl->m_oTransform.SetElements(dM11, dM12, dM21, dM22, dX, dY);
	return S_OK;
}

HRESULT CTxtRenderer::GetTransform(double* dM11,
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

HRESULT CTxtRenderer::ResetTransform()
{
	m_pImpl->m_oTransform.Reset();
	return S_OK;
}

HRESULT CTxtRenderer::get_ClipMode(LONG* lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}
HRESULT CTxtRenderer::put_ClipMode(const LONG& lMode)
{
	MAYBE_UNUSED(lMode);
	return S_FALSE;
}

HRESULT CTxtRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(lCommand);
	return S_FALSE;
}
HRESULT CTxtRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(dCommand);
	return S_FALSE;
}
HRESULT CTxtRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	MAYBE_UNUSED(lType); MAYBE_UNUSED(sCommand);
	return S_FALSE;
}

HRESULT CTxtRenderer::IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type)
{
	MAYBE_UNUSED(type);
	return S_FALSE;
}
HRESULT CTxtRenderer::AdvancedCommand(IAdvancedCommand* command)
{
	MAYBE_UNUSED(command);
	return S_FALSE;
}

void CTxtRenderer::Convert(IOfficeDrawingFile* pFile, const std::wstring& wsDstTxtFile)
{
	int pages_count = pFile->GetPagesCount();
	for (int i = 0; i < pages_count; ++i)
		DrawPage(pFile, i);

	m_pImpl->Save(wsDstTxtFile);
}
void CTxtRenderer::DrawPage(IOfficeDrawingFile* pFile, int nPage)
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
