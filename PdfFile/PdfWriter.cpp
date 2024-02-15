﻿/*
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
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

#include "PdfWriter.h"

#include "SrcWriter/Document.h"
#include "SrcWriter/Pages.h"
#include "SrcWriter/Image.h"
#include "SrcWriter/Font.h"
#include "SrcWriter/FontCidTT.h"
#include "SrcWriter/FontTT.h"
#include "SrcWriter/Destination.h"
#include "SrcWriter/Field.h"

#include "../../DesktopEditor/graphics/Image.h"
#include "../../DesktopEditor/graphics/structures.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Image.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/graphics/GraphicsPath.h"

#include "../../UnicodeConverter/UnicodeConverter.h"
#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#if defined(GetTempPath)
#undef GetTempPath
#endif

#if defined(CreateFile)
#undef CreateFile
#endif

#if defined(CreateDirectory)
#undef CreateDirectory
#endif

#define MM_2_PT(X) ((X) * 72.0 / 25.4)
#define PT_2_MM(X) ((X) * 25.4 / 72.0)

#define LONG_2_BOOL(X) ((X) ? true : false)

#ifdef DrawText
#undef DrawText
#endif

#ifdef LoadImage
#undef LoadImage
#endif

#define HI_SURROGATE_START  0xD800
#define HI_SURROGATE_END    0xDBFF
#define LO_SURROGATE_START  0xDC00
#define LO_SURROGATE_END    0xDFFF

// Этих типов браша нет в рендерере, мы их используем, когда конвертим из веба
static const long c_BrushTypeLinearGradient = 8001;
static const long c_BrushTypeRadialGradient = 8002;

void GetMetafileRasterSize(MetaFile::IMetaFile* pMetafile, const double& dWidth, int& nW, int& nH)
{
	double dNewW = std::max(10.0, dWidth) / 25.4 * 300;

	/* old version:
	nW = (int)nW;
	nH = -1;
	return;
	*/


	double dX, dY, dW, dH;
	pMetafile->GetBounds(&dX, &dY, &dW, &dH);

	if (dW < 0) dW = -dW;
	if (dH < 0) dH = -dH;

	if (dW < 0.1) dW = 0.1;
	if (dH < 0.1) dH = 0.1;

	double dNewH = dNewW * dH / dW;

	double dOneMaxSize = 2000;

	if (dNewW > dOneMaxSize || dNewH > dOneMaxSize)
	{
		if (dNewW > dNewH)
		{
			dNewH *= (dOneMaxSize / dNewW);
			dNewW = dOneMaxSize;
		}
		else
		{
			dW *= (dOneMaxSize / dNewH);
			dNewH = dOneMaxSize;
		}
	}

	if (dNewW < 1) dNewW = 1;
	if (dNewH < 1) dNewH = 1;

	nW = (int)dNewW;
	nH = (int)dNewH;
}

//----------------------------------------------------------------------------------------
//
// CPdfRenderer
//
//----------------------------------------------------------------------------------------
CPdfWriter::CPdfWriter(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA, IRenderer* pRenderer, bool bCreate) : m_oCommandManager(this)
{
	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = pAppFonts->GenerateFontManager();
	NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
	pMeasurerCache->SetStreams(pAppFonts->GetStreams());
	m_pFontManager->SetOwnerCache(pMeasurerCache);
	m_pRenderer = pRenderer;

	m_pDocument = new PdfWriter::CDocument();

	if (isPDFA)
		m_pDocument->SetPDFAConformanceMode(true);

	if (!m_pDocument || (bCreate && !m_pDocument->CreateNew()))
	{
		SetError();
		return;
	}

	m_pDocument->SetCompressionMode(COMP_ALL);

	m_bValid      = true;
	m_dPageHeight = 297;
	m_dPageWidth  = 210;
	m_pPage       = NULL;
	m_pFont       = NULL;

	m_unFieldsCounter          = 0;
	m_bNeedUpdateTextFont      = true;
	m_bNeedUpdateTextColor     = true;
	m_bNeedUpdateTextAlpha     = true;
	m_bNeedUpdateTextCharSpace = true;
	m_bNeedUpdateTextSize      = true;
}
CPdfWriter::~CPdfWriter()
{
	RELEASEOBJECT(m_pDocument);
	RELEASEINTERFACE(m_pFontManager);
}
void CPdfWriter::SetPassword(const std::wstring& wsPassword)
{
	if (!IsValid())
		return;

	m_pDocument->SetPasswords(wsPassword, wsPassword);
}
void CPdfWriter::SetDocumentID(const std::wstring& wsDocumentID)
{
	if (!IsValid())
		return;

	m_pDocument->SetDocumentID(wsDocumentID);
}
int CPdfWriter::SaveToFile(const std::wstring& wsPath)
{
	// TODO: Переделать на код ошибки
	if (!IsValid())
		return 1;

	m_oCommandManager.Flush();

	if (!m_pDocument->SaveToFile(wsPath))
		return 1;

	return 0;
}
void CPdfWriter::SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords)
{
	if (!IsValid())
		return;

	if (!wsTitle.empty())
		m_pDocument->SetTitle(U_TO_UTF8(wsTitle));
	if (!wsCreator.empty())
	{
		std::string sAuthor = U_TO_UTF8(wsCreator);
		NSStringUtils::string_replaceA(sAuthor, ";", ", ");
		m_pDocument->SetAuthor(sAuthor);
	}
	if (!wsSubject.empty())
		m_pDocument->SetSubject(U_TO_UTF8(wsSubject));
	if (!wsKeywords.empty())
		m_pDocument->SetKeywords(U_TO_UTF8(wsKeywords));
}
std::wstring CPdfWriter::GetTempFile(const std::wstring& wsDirectory)
{
	return NSFile::CFileBinary::CreateTempFileWithUniqueName(wsDirectory, L"PDF");
}
//----------------------------------------------------------------------------------------
// Функции для работы со страницей
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::NewPage()
{
	m_oCommandManager.Flush();

	if (!IsValid())
		return S_FALSE;

	m_pPage = m_pDocument->AddPage();

	if (!m_pPage)
	{
		SetError();
		return S_FALSE;
	}

	m_pPage->SetWidth(MM_2_PT(m_dPageWidth));
	m_pPage->SetHeight(MM_2_PT(m_dPageHeight));

	Reset();

	return S_OK;
}
HRESULT CPdfWriter::get_Height(double* dHeight)
{
	*dHeight = m_dPageHeight;
	return S_OK;
}
HRESULT CPdfWriter::put_Height(const double& dHeight)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageHeight = dHeight;
	m_pPage->SetHeight(MM_2_PT(dHeight));
	return S_OK;
}
HRESULT CPdfWriter::get_Width(double* dWidth)
{
	*dWidth = m_dPageWidth;
	return S_OK;
}
HRESULT CPdfWriter::put_Width(const double& dWidth)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageWidth = dWidth;
	m_pPage->SetWidth(MM_2_PT(dWidth));
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Pen
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.GetColor();
	return S_OK;
}
HRESULT CPdfWriter::put_PenColor(const LONG& lColor)
{
	m_oPen.SetColor(lColor);
	return S_OK;
}
HRESULT CPdfWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.GetAlpha();
	return S_OK;
}
HRESULT CPdfWriter::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.SetAlpha(lAlpha);
	return S_OK;
}
HRESULT CPdfWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.GetSize();
	return S_OK;
}
HRESULT CPdfWriter::put_PenSize(const double& dSize)
{
	m_oPen.SetSize(dSize);
	return S_OK;
}
HRESULT CPdfWriter::get_PenDashStyle(BYTE* nDashStyle)
{
	*nDashStyle = m_oPen.GetDashStyle();
	return S_OK;
}
HRESULT CPdfWriter::put_PenDashStyle(const BYTE& nDashStyle)
{
	m_oPen.SetDashStyle(nDashStyle);
	return S_OK;
}
HRESULT CPdfWriter::get_PenLineStartCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetStartCapStyle();
	return S_OK;
}
HRESULT CPdfWriter::put_PenLineStartCap(const BYTE& nCapStyle)
{
	m_oPen.SetStartCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfWriter::get_PenLineEndCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetEndCapStyle();
	return S_OK;
}
HRESULT CPdfWriter::put_PenLineEndCap(const BYTE& nCapStyle)
{
	m_oPen.SetEndCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfWriter::get_PenLineJoin(BYTE* nJoinStyle)
{
	*nJoinStyle = m_oPen.GetJoinStyle();
	return S_OK;
}
HRESULT CPdfWriter::put_PenLineJoin(const BYTE& nJoinStyle)
{
	m_oPen.SetJoinStyle(nJoinStyle);
	return S_OK;
}
HRESULT CPdfWriter::get_PenDashOffset(double* dOffset)
{
	*dOffset = m_oPen.GetDashOffset();
	return S_OK;
}
HRESULT CPdfWriter::put_PenDashOffset(const double& dOffset)
{
	m_oPen.SetDashOffset(dOffset);
	return S_OK;
}
HRESULT CPdfWriter::get_PenAlign(LONG* lAlign)
{
	*lAlign = m_oPen.GetAlign();
	return S_OK;
}
HRESULT CPdfWriter::put_PenAlign(const LONG& lAlign)
{
	m_oPen.SetAlign(lAlign);
	return S_OK;
}
HRESULT CPdfWriter::get_PenMiterLimit(double* dMiter)
{
	*dMiter = m_oPen.GetMiter();
	return S_OK;
}
HRESULT CPdfWriter::put_PenMiterLimit(const double& dMiter)
{
	m_oPen.SetMiter(dMiter);
	return S_OK;
}
HRESULT CPdfWriter::PenDashPattern(double* pPattern, LONG lCount)
{
	m_oPen.SetDashPattern(pPattern, lCount);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Brush
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.GetType();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushType(const LONG& lType)
{
	m_oBrush.SetType(lType);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.GetColor1();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushColor1(const LONG& lColor)
{
	if (lColor != m_oBrush.GetColor1())
	{
		m_oBrush.SetColor1(lColor);
		m_bNeedUpdateTextColor = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetAlpha1();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushAlpha1(const LONG& lAlpha)
{
	if (lAlpha != m_oBrush.GetAlpha1())
	{
		m_oBrush.SetAlpha1(lAlpha);
		m_bNeedUpdateTextAlpha = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.GetColor2();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.SetColor2(lColor);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetAlpha2();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.SetAlpha2(lAlpha);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushTexturePath(std::wstring* wsPath)
{
	*wsPath = m_oBrush.GetTexturePath();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushTexturePath(const std::wstring& wsPath)
{
	m_oBrush.SetTexturePath(wsPath);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.GetTextureMode();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.SetTextureMode(lMode);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushTextureAlpha(LONG* lAlpha)
{
	*lAlpha = m_oBrush.GetTextureAlpha();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushTextureAlpha(const LONG& lAlpha)
{
	m_oBrush.SetTextureAlpha(lAlpha);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.GetLinearAngle();
	return S_OK;
}
HRESULT CPdfWriter::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.SetLinearAngle(dAngle);
	return S_OK;
}
HRESULT CPdfWriter::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
	// Данными параметрами пользуемся, только если пришла команда EnableBrushRect, если команда не пришла, тогда
	// ориентируемся на границы пата.
	m_oBrush.SetBrushRect(nVal, dLeft, dTop, dWidth, dHeight);
	m_oBrush.EnableBrushRect(1 == nVal ? true : false);
	return S_OK;
}
HRESULT CPdfWriter::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG lCount)
{
	m_oBrush.SetGradientColors(lColors, pPositions, lCount);
	return S_OK;
}
HRESULT CPdfWriter::get_BrushTextureImage(Aggplus::CImage** pImage)
{
	// TODO:
	return S_OK;
}
HRESULT CPdfWriter::put_BrushTextureImage(Aggplus::CImage* pImage)
{
	// TODO:
	return S_OK;
}
HRESULT CPdfWriter::get_BrushTransform(Aggplus::CMatrix& oMatrix)
{
	// TODO:
	return S_OK;
}
HRESULT CPdfWriter::put_BrushTransform(const Aggplus::CMatrix& oMatrix)
{
	// TODO:
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со шрифтами
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::get_FontName(std::wstring* wsName)
{
	*wsName = m_oFont.GetName();
	return S_OK;
}
HRESULT CPdfWriter::put_FontName(const std::wstring& wsName)
{
	if (wsName != m_oFont.GetName())
	{
		m_oFont.SetName(wsName);
		m_bNeedUpdateTextFont = true;
	}

	return S_OK;
}
HRESULT CPdfWriter::get_FontPath(std::wstring* wsPath)
{
	*wsPath = m_oFont.GetPath();
	return S_OK;
}
HRESULT CPdfWriter::put_FontPath(const std::wstring& wsPath)
{
	if (wsPath != m_oFont.GetPath())
	{
		m_oFont.SetPath(wsPath);
		m_bNeedUpdateTextFont = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.GetSize();
	return S_OK;
}
HRESULT CPdfWriter::put_FontSize(const double& dSize)
{
	if (fabs(dSize - m_oFont.GetSize()) > 0.001)
	{
		m_oFont.SetSize(dSize);
		m_bNeedUpdateTextSize = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT CPdfWriter::put_FontStyle(const LONG& lStyle)
{
	if (lStyle != m_oFont.GetStyle())
	{
		m_oFont.SetStyle(lStyle);
		m_bNeedUpdateTextFont = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_FontStringGID(INT* bGid)
{
	*bGid = m_oFont.GetGid() ? 1 : 0;
	return S_OK;
}
HRESULT CPdfWriter::put_FontStringGID(const INT& bGid)
{
	m_oFont.SetGid(bGid ? true : false);
	return S_OK;
}
HRESULT CPdfWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.GetCharSpace();
	return S_OK;
}
HRESULT CPdfWriter::put_FontCharSpace(const double& dSpace)
{
	if (fabs(dSpace - m_oFont.GetCharSpace()) > 0.001)
	{
		m_oFont.SetCharSpace(dSpace);
		m_bNeedUpdateTextCharSpace = true;
	}
	return S_OK;
}
HRESULT CPdfWriter::get_FontFaceIndex(int* nFaceIndex)
{
	*nFaceIndex = (int)m_oFont.GetFaceIndex();
	return S_OK;
}
HRESULT CPdfWriter::put_FontFaceIndex(const int& nFaceIndex)
{
	if (nFaceIndex != m_oFont.GetFaceIndex())
	{
		m_oFont.SetFaceIndex(nFaceIndex);
		m_bNeedUpdateTextFont = true;
	}

	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для вывода текста
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid())
		return S_FALSE;

	unsigned int unUnicode = lUnicode;
	unsigned char* pCodes = EncodeString(&unUnicode, 1);
	return DrawText(pCodes, 2, dX, dY) ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid() || !wsUnicodeText.size())
		return S_FALSE;

	unsigned int unLen;
	unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unLen);
	if (!pUnicodes)
		return S_FALSE;

	unsigned char* pCodes = EncodeString(pUnicodes, unLen);
	delete[] pUnicodes;

	if (!pCodes)
		return S_FALSE;

	// Специальный случай для текста из Djvu, нам не нужно, чтобы он рисовался
	if (L"" == m_oFont.GetPath() && L"DjvuEmptyFont" == m_oFont.GetName())
	{
		if (m_bNeedUpdateTextFont)
		{
			m_oFont.SetName(L"Arial");
			UpdateFont();
			m_oFont.SetName(L"DjvuEmptyFont");
			if (!m_pFont)
				return S_FALSE;
		}

		double dFontSize = MM_2_PT(dH);
		double dStringWidth = 0;
		for (unsigned int unIndex = 0; unIndex < unLen; unIndex++)
		{
			unsigned short ushCode = (pCodes[2 * unIndex] << 8) + pCodes[2 * unIndex + 1];
			dStringWidth += m_pFont->GetWidth(ushCode) * dFontSize / 1000.0;
		}

		double dResultWidth = MM_2_PT(dW);

		CTransform& t = m_oTransform;
		m_oCommandManager.SetTransform(t.m11, -t.m12, -t.m21, t.m22, MM_2_PT(t.dx + t.m21 * m_dPageHeight), MM_2_PT(m_dPageHeight - m_dPageHeight * t.m22 - t.dy));

		CRendererTextCommand* pText = m_oCommandManager.AddText(pCodes, unLen * 2, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH));
		pText->SetFont(m_pFont);
		pText->SetSize(dFontSize);
		pText->SetMode(PdfWriter::textrenderingmode_Invisible);
		if (fabs(dStringWidth) > 0.001)
			pText->SetHorScaling(dResultWidth / dStringWidth * 100);

		return S_OK;
	}

	return DrawText(pCodes, unLen * 2, dX, dY) ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid())
		return S_FALSE;

	unsigned int unUnicode = lUnicode;
	unsigned int unGID     = lGid;
	unsigned char* pCodes = EncodeGID(unGID, &unUnicode, 1);
	if (!pCodes)
		return DrawTextToRenderer(&unGID, 1, dX, dY) ? S_OK : S_FALSE;
	return DrawText(pCodes, 2, dX, dY) ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int unGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid() || (!wsUnicodeText.size() && (!pGids || !unGidsCount)))
		return S_FALSE;

	unsigned int unLen = 0;
	unsigned int* pUnicodes = NULL;
	if (pGids && unGidsCount)
	{
		unLen = unGidsCount;
		if (wsUnicodeText.size())
		{
			unsigned int unUnicodeLen;
			pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unUnicodeLen);
			if (!pUnicodes || unUnicodeLen != unLen)
				RELEASEARRAYOBJECTS(pUnicodes);
		}

		if (!pUnicodes)
		{
			pUnicodes = new unsigned int[unLen];
			if (!pUnicodes)
				return S_FALSE;

			for (unsigned int unIndex = 0; unIndex < unLen; unIndex++)
				pUnicodes[unIndex] = pGids[unIndex];
		}
	}
	else
	{
		pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unLen);
		if (!pUnicodes)
			return S_FALSE;
	}

	unsigned char* pCodes = EncodeString(pUnicodes, unLen, pGids);
	RELEASEARRAYOBJECTS(pUnicodes);
	if (!pCodes)
		return DrawTextToRenderer(pGids, unGidsCount, dX, dY) ? S_OK : S_FALSE;
	return DrawText(pCodes, unLen * 2, dX, dY) ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::CommandDrawTextCHAR2(unsigned int* pUnicodes, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid())
		return S_FALSE;

	unsigned char* pCodes = EncodeGID(unGid, pUnicodes, unUnicodeCount);
	if (!pCodes)
		return DrawTextToRenderer(&unGid, 1, dX, dY) ? S_OK : S_FALSE;
	return DrawText(pCodes, 2, dX, dY) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------------------
// Маркеры команд
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::EndCommand(const DWORD& dwType, const LONG& lClipMode)
{
	if (!IsPageValid())
		return S_FALSE;

	// Здесь мы различаем лишь 2 команды: присоединить текущий пат к клипу и отменить клип
	if (c_nClipType == dwType)
	{
		m_oCommandManager.Flush();
		m_pPage->GrSave();
		m_lClipDepth++;
		UpdateTransform();

		m_oPath.Clip(m_pPage, c_nClipRegionTypeEvenOdd & lClipMode);
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
	else if (c_nPageType == dwType)
	{
		for (int nIndex = 0, nCount = m_vDestinations.size(); nIndex < nCount; ++nIndex)
		{
			TDestinationInfo& oInfo = m_vDestinations.at(nIndex);
			if (m_pDocument->GetPagesCount() > oInfo.unDestPage)
			{
				AddLink(oInfo.pPage, oInfo.dX, oInfo.dY, oInfo.dW, oInfo.dH, oInfo.dDestX, oInfo.dDestY, oInfo.unDestPage);
				m_vDestinations.erase(m_vDestinations.begin() + nIndex);
				nIndex--;
				nCount--;
			}
		}
	}

	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с патом
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::PathCommandStart()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CPdfWriter::PathCommandEnd()
{
	m_oPath.Clear();
	return S_OK;
}
HRESULT CPdfWriter::DrawPath(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const LONG& lType)
{
	m_oCommandManager.Flush();

	if (!IsPageValid())
		return S_FALSE;

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

	return S_OK;
}
HRESULT CPdfWriter::PathCommandMoveTo(const double& dX, const double& dY)
{
	m_oPath.MoveTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CPdfWriter::PathCommandLineTo(const double& dX, const double& dY)
{
	m_oPath.LineTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	return S_OK;
}
HRESULT CPdfWriter::PathCommandLinesTo(double* pPoints, const int& nCount)
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
HRESULT CPdfWriter::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
	m_oPath.CurveTo(MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), MM_2_PT(dX2), MM_2_PT(m_dPageHeight - dY2), MM_2_PT(dXe), MM_2_PT(m_dPageHeight - dYe));
	return S_OK;
}
HRESULT CPdfWriter::PathCommandCurvesTo(double* pPoints, const int& nCount)
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
HRESULT CPdfWriter::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
	m_oPath.ArcTo(MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH), dStartAngle, dSweepAngle);
	return S_OK;
}
HRESULT CPdfWriter::PathCommandClose()
{
	m_oPath.Close();
	return S_OK;
}
HRESULT CPdfWriter::PathCommandGetCurrentPoint(double* dX, double* dY)
{
	m_oPath.GetLastPoint(*dX, *dY);
	*dX = PT_2_MM(*dX);
	*dY = PT_2_MM(*dY);
	return S_OK;
}
HRESULT CPdfWriter::PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
	unsigned int unUnicode = lUnicode;
	bool bRes = PathCommandDrawText(&unUnicode, 1, dX, dY, NULL);
	return bRes ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
	unsigned int unLen;
	unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unLen);
	if (!pUnicodes)
		return S_FALSE;

	PathCommandDrawText(pUnicodes, unLen, dX, dY, NULL);
	delete[] pUnicodes;

	return S_OK;
}
HRESULT CPdfWriter::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
	unsigned int unUnicode = lUnicode;
	unsigned int unGid     = lGid;
	bool bRes = PathCommandDrawText(&unUnicode, 1, dX, dY, &unGid);
	return bRes ? S_OK : S_FALSE;
}
HRESULT CPdfWriter::PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int unGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
	if (!IsPageValid() || (!wsUnicodeText.size() && (!pGids || !unGidsCount)))
		return S_FALSE;

	unsigned int unLen = 0;
	unsigned int* pUnicodes = NULL;
	if (pGids && unGidsCount)
	{
		unLen = unGidsCount;
		if (wsUnicodeText.size())
		{
			unsigned int unUnicodeLen;
			pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unUnicodeLen);
			if (!pUnicodes || unUnicodeLen != unLen)
				RELEASEARRAYOBJECTS(pUnicodes);
		}

		if (!pUnicodes)
		{
			pUnicodes = new unsigned int[unLen];
			if (!pUnicodes)
				return S_FALSE;

			for (unsigned int unIndex = 0; unIndex < unLen; unIndex++)
				pUnicodes[unIndex] = pGids[unIndex];
		}
	}
	else
	{
		pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, unLen);
		if (!pUnicodes)
			return S_FALSE;
	}

	bool bRes = PathCommandDrawText(pUnicodes, unLen, dX, dY, pGids);
	RELEASEARRAYOBJECTS(pUnicodes);

	return bRes ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------------------
// Функции для вывода изображений
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
	m_oCommandManager.Flush();

	if (!IsPageValid() || !pImage)
		return S_OK;

	if (!DrawImage((Aggplus::CImage*)pImage, dX, dY, dW, dH, 255))
		return S_FALSE;

	return S_OK;
}
HRESULT CPdfWriter::DrawImageFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const std::wstring& wsImagePathSrc, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	m_oCommandManager.Flush();

	if (!IsPageValid())
		return S_OK;

	std::wstring sTempImagePath = GetDownloadFile(wsImagePathSrc, wsTempDirectory);
	std::wstring wsImagePath = sTempImagePath.empty() ? wsImagePathSrc : sTempImagePath;

	Aggplus::CImage* pAggImage = NULL;

	CImageFileFormatChecker oImageFormat(wsImagePath);
	if (_CXIMAGE_FORMAT_WMF == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_EMF == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_SVM == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_SVG == oImageFormat.eFileType)
	{
		// TODO: Реализовать отрисовку метафайлов по-нормальному
		MetaFile::IMetaFile* pMeta = MetaFile::Create(pAppFonts);
		pMeta->LoadFromFile(wsImagePath.c_str());

		std::wstring wsTempFile = GetTempFile(wsTempDirectory);
		int nW = 0, nH = 0;
		GetMetafileRasterSize(pMeta, dW, nW, nH);
		pMeta->ConvertToRaster(wsTempFile.c_str(), _CXIMAGE_FORMAT_PNG, nW, nH);

		RELEASEOBJECT(pMeta);

		pAggImage = new Aggplus::CImage(wsTempFile);
	}
	else
	{
		pAggImage = new Aggplus::CImage(wsImagePath);
	}

	HRESULT hRes = S_OK;
	if (!pAggImage || !DrawImage(pAggImage, dX, dY, dW, dH, nAlpha))
		hRes = S_FALSE;

	if (NSFile::CFileBinary::Exists(sTempImagePath))
		NSFile::CFileBinary::Remove(sTempImagePath);

	if (pAggImage)
		delete pAggImage;

	return hRes;
}
//----------------------------------------------------------------------------------------
// Функции для выставления преобразования
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
	m_oCommandManager.Flush();
	m_oTransform.Set(dM11, dM12, dM21, dM22, dX, dY);
	return S_OK;
}
HRESULT CPdfWriter::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
	*dM11 = m_oTransform.m11;
	*dM12 = m_oTransform.m12;
	*dM21 = m_oTransform.m21;
	*dM22 = m_oTransform.m22;
	*dX   = m_oTransform.dx;
	*dY   = m_oTransform.dy;
	return S_OK;
}
HRESULT CPdfWriter::ResetTransform()
{
	m_oCommandManager.Flush();
	m_oTransform.Reset();
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Дополнительные функции
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip)
{
	NSUnicodeConverter::CUnicodeConverter conv;
	PdfWriter::CAnnotation* pAnnot = m_pDocument->CreateUriLinkAnnot(PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)), conv.SASLprepToUtf8(wsUrl).c_str());
	m_pPage->AddAnnotation(pAnnot);
	pAnnot->SetBorder(0, 0, {});
	return S_OK;
}
HRESULT CPdfWriter::AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage)
{
	unsigned int unPagesCount = m_pDocument->GetPagesCount();
	if (unPagesCount == 0)
		return S_OK;

	PdfWriter::CPage* pPage = m_pDocument->GetPage(nPage);
	if (!pPage)
	{
		m_vDestinations.push_back(TDestinationInfo(m_pPage, dX, dY, dW, dH, dDestX, dDestY, nPage));
	}
	else
	{
		AddLink(m_pPage, dX, dY, dW, dH, dDestX, dDestY, nPage);
	}

	return S_OK;
}
HRESULT CPdfWriter::AddFormField(NSFonts::IApplicationFonts* pAppFonts, CFormFieldInfo* pFieldInfo, const std::wstring& wsTempDirectory)
{
	if (!m_pDocument || 0 == m_pDocument->GetPagesCount() || !pFieldInfo)
		return S_OK;
	CFormFieldInfo& oInfo = *pFieldInfo;

	PdfWriter::CFontCidTrueType* pCheckedFont   = NULL;
	PdfWriter::CFontCidTrueType* pUncheckedFont = NULL;
	if (oInfo.IsCheckBox())
	{
		const CFormFieldInfo::CCheckBoxFormPr* pPr = oInfo.GetCheckBoxPr();
		pCheckedFont   = GetFont(pPr->GetCheckedFontName(), false, false);
		pUncheckedFont = GetFont(pPr->GetUncheckedFontName(), false, false);
	}
	if ((oInfo.IsCheckBox() && (!pCheckedFont || !pUncheckedFont)) || oInfo.IsTextField() || oInfo.IsDropDownList() || oInfo.IsDateTime())
	{
		if (m_bNeedUpdateTextFont)
			UpdateFont();
		if (!m_pFont)
			return S_OK;
		if (oInfo.IsCheckBox())
		{
			if (!pCheckedFont)
				pCheckedFont = m_pFont;
			if (!pUncheckedFont)
				pUncheckedFont = m_pFont;
		}
	}

	PdfWriter::CFontTrueType* pFontTT = NULL;
	if (oInfo.IsTextField() || oInfo.IsDropDownList())
	{
		pFontTT = m_pDocument->CreateTrueTypeFont(m_pFont);
		if (!pFontTT)
			return S_OK;
	}

	double dX, dY, dW, dH;
	oInfo.GetBounds(dX, dY, dW, dH);

	PdfWriter::CFieldBase* pFieldBase = NULL;

	bool bRadioButton = false;
	if (oInfo.IsTextField())
	{
		PdfWriter::CTextField* pField = m_pDocument->CreateTextField();
		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}
	else if (oInfo.IsDropDownList())
	{
		PdfWriter::CChoiceField* pField = m_pDocument->CreateChoiceField();
		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}
	else if (oInfo.IsCheckBox())
	{
		const CFormFieldInfo::CCheckBoxFormPr* pPr = oInfo.GetCheckBoxPr();

		PdfWriter::CCheckBoxField* pField = NULL;
		std::wstring wsGroupName = pPr->GetGroupKey();
		if (L"" != wsGroupName)
		{
			bRadioButton = true;
			PdfWriter::CRadioGroupField* pRadioGroup = m_pDocument->GetRadioGroupField(wsGroupName);
			if (pRadioGroup)
				pField = pRadioGroup->CreateKid();
		}
		else
		{
			pField = m_pDocument->CreateCheckBoxField();
		}

		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}
	else if (oInfo.IsPicture())
	{
		PdfWriter::CPictureField* pField = m_pDocument->CreatePictureField();
		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}
	else if (oInfo.IsSignature())
	{
		PdfWriter::CSignatureField* pField = m_pDocument->CreateSignatureField();
		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}
	else if (oInfo.IsDateTime())
	{
		PdfWriter::CDateTimeField* pField = m_pDocument->CreateDateTimeField();
		pFieldBase = static_cast<PdfWriter::CFieldBase*>(pField);
	}

	if (!pFieldBase)
		return S_FALSE;

	// 0 - Right
	// 1 - Left
	// 2 - Center
	// 3 - Justify
	// 4 - Distributed
	unsigned int unAlign = oInfo.GetJc();
	if (0 == unAlign)
		pFieldBase->SetAlign(PdfWriter::CFieldBase::EFieldAlignType::Right);
	else if (2 == unAlign)
		pFieldBase->SetAlign(PdfWriter::CFieldBase::EFieldAlignType::Center);

	if (oInfo.HaveBorder())
	{
		unsigned char unR, unG, unB, unA;
		oInfo.GetBorderColor(unR, unG, unB, unA);

		pFieldBase->SetFieldBorder(PdfWriter::EBorderSubtype::border_subtype_Solid, PdfWriter::TRgb(unR, unG, unB), MM_2_PT(oInfo.GetBorderSize()), 0, 0, 0);
	}

	if (oInfo.HaveShd())
	{
		unsigned char unR, unG, unB, unA;
		oInfo.GetShdColor(unR, unG, unB, unA);
		pFieldBase->SetShd(PdfWriter::TRgb(unR, unG, unB));
	}

	pFieldBase->SetRequiredFlag(oInfo.IsRequired());
	pFieldBase->SetFieldHint(oInfo.GetHelpText());

	bool isBold   = m_oFont.IsBold();
	bool isItalic = m_oFont.IsItalic();

	if (oInfo.IsTextField())
	{
		const CFormFieldInfo::CTextFormPr* pPr = oInfo.GetTextFormPr();
		std::wstring wsValue = pPr->GetTextValue();

		unsigned int unLen = 0;
		unsigned int* pUnicodes = NULL;
		unsigned short* pCodes  = NULL;
		PdfWriter::CFontCidTrueType** ppFonts = NULL;
		bool bFont = GetFontData(pAppFonts, wsValue, m_pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}
		PdfWriter::CTextField* pField = dynamic_cast<PdfWriter::CTextField*>(pFieldBase);
		if (!pField)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}

		double _dY = m_pPage->GetHeight() - MM_2_PT(dY);
		double _dB = m_pPage->GetHeight() - MM_2_PT(dY + dH);

		double dMargin   = 2; // такой отступ используется в AdobeReader
		double dBaseLine = MM_2_PT(dH - oInfo.GetBaseLineOffset());
		double dShiftX   = dMargin;

		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX) - dMargin, _dY, MM_2_PT(dX + dW) + dMargin, _dB));

		pField->SetMaxLen(pPr->GetMaxCharacters());
		pField->SetCombFlag(pPr->IsComb());
		pField->SetAutoFit(pPr->IsAutoFit());
		pField->SetMultilineFlag(pPr->IsMultiLine());

		bool isComb = pPr->IsComb();
		unsigned int unAlign = oInfo.GetJc();
		double dFontSize = m_oFont.GetSize();

		TColor oColor      = m_oBrush.GetTColor1();
		bool isPlaceHolder = oInfo.IsPlaceHolder();

		PdfWriter::TRgb oNormalColor(oColor.r, oColor.g, oColor.b);
		PdfWriter::TRgb oPlaceHolderColor;
		oPlaceHolderColor.r = oNormalColor.r + (1.0 - oNormalColor.r) / 2.0;
		oPlaceHolderColor.g = oNormalColor.g + (1.0 - oNormalColor.g) / 2.0;
		oPlaceHolderColor.b = oNormalColor.b + (1.0 - oNormalColor.b) / 2.0;

		if (!isPlaceHolder)
			pField->SetTextValue(wsValue);

		if (!isComb && pPr->IsMultiLine())
		{
			unsigned short* pCodes2 = new unsigned short[unLen];
			unsigned int* pWidths   = new unsigned int[unLen];
			
			unsigned short ushSpaceCode   = 0xFFFF;
			unsigned short ushNewLineCode = 0xFFFE;
			for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
			{
				unsigned short ushCode = 0;
				if (0x0020 == pUnicodes[unIndex])
					ushCode = ushSpaceCode;
				else if (0x000D == pUnicodes[unIndex] || 0x000A == pUnicodes[unIndex])
					ushCode = ushNewLineCode;
				
				pCodes2[unIndex] = ushCode;
				pWidths[unIndex] = ppFonts[unIndex]->GetWidth(pCodes[unIndex]);
			}
			
			m_oLinesManager.Init(pCodes2, pWidths, unLen, ushSpaceCode, ushNewLineCode, pFontTT->GetLineHeight(), pFontTT->GetAscent());
			
			// TODO: Разобраться более детально по какой именно высоте идет в Adobe расчет
			//       пока временно оставим (H - 3 * margin)
			if (pPr->IsAutoFit())
				dFontSize = m_oLinesManager.ProcessAutoFit(MM_2_PT(dW), (MM_2_PT(dH) - 3 * dMargin));

			double dLineHeight = pFontTT->GetLineHeight() * dFontSize / 1000.0;

			m_oLinesManager.CalculateLines(dFontSize, MM_2_PT(dW));

			pField->StartTextAppearance(m_pFont, dFontSize, isPlaceHolder ? oPlaceHolderColor : oNormalColor, 1.0);

			unsigned int unLinesCount = m_oLinesManager.GetLinesCount();
			double dLineShiftY = MM_2_PT(dH) - pFontTT->GetLineHeight() * dFontSize / 1000.0 - dMargin;
			for (unsigned int unIndex = 0; unIndex < unLinesCount; ++unIndex)
			{
				unsigned int unLineStart = m_oLinesManager.GetLineStartPos(unIndex);
				double dLineShiftX = dShiftX;
				double dLineWidth = m_oLinesManager.GetLineWidth(unIndex, dFontSize);
				if (0 == unAlign)
					dLineShiftX += MM_2_PT(dW) - dLineWidth;
				else if (2 == unAlign)
					dLineShiftX += (MM_2_PT(dW) - dLineWidth) / 2;

				int nInLineCount = m_oLinesManager.GetLineEndPos(unIndex) - m_oLinesManager.GetLineStartPos(unIndex);
				if (nInLineCount > 0)
					pField->AddLineToTextAppearance(dLineShiftX, dLineShiftY, pCodes + unLineStart, nInLineCount, ppFonts + unLineStart, NULL);

				dLineShiftY -= dLineHeight;
			}

			pField->EndTextAppearance();

			m_oLinesManager.Clear();

			delete[] pCodes2;
			delete[] pWidths;
		}
		else
		{
			double* pShifts = NULL;
			unsigned int unShiftsCount = 0;

			if (isComb)
			{
				pField->SetDoNotScrollFlag(true);
				pField->SetDoNotSpellCheckFlag(true);
				pField->SetMultilineFlag(false);

				unShiftsCount = unLen;
				pShifts = new double[unShiftsCount];
				if (pShifts && unShiftsCount)
				{
					// Сдвиг нулевой для comb форм и не забываем, что мы к ширине добавили 2 * dMargin
					dShiftX = 0;
					unsigned int unCellsCount = std::max(unShiftsCount, pPr->GetMaxCharacters());
					double dPrevW = 0;
					double dCellW = (MM_2_PT(dW) + 2 * dMargin) / unCellsCount;

					if (0 == unAlign && unShiftsCount)
						dPrevW = (unCellsCount - unShiftsCount) * dCellW;

					for (unsigned int unIndex = 0; unIndex < unShiftsCount; ++unIndex)
					{
						unsigned short ushCode = pCodes[unIndex];
						double dGlyphWidth = ppFonts[unIndex]->GetGlyphWidth(ushCode) / 1000.0 * dFontSize;
						double dTempShift = (dCellW - dGlyphWidth) / 2;
						pShifts[unIndex] = dPrevW + dTempShift;
						dPrevW = dCellW - dTempShift;

					}
				}
				else
				{
					unShiftsCount = 0;
				}
			}
			else if (0 == unAlign || 2 == unAlign)
			{
				double dSumWidth = 0;
				for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
				{
					unsigned short ushCode = pCodes[unIndex];
					double dLetterWidth    = ppFonts[unIndex]->GetWidth(ushCode) / 1000.0 * dFontSize;
					dSumWidth += dLetterWidth;
				}

				if (0 == unAlign && MM_2_PT(dW) - dSumWidth > 0)
					dShiftX += MM_2_PT(dW) - dSumWidth;
				else if (2 == unAlign && (MM_2_PT(dW) - dSumWidth) / 2 > 0)
					dShiftX += (MM_2_PT(dW) - dSumWidth) / 2;
			}

			pField->SetTextAppearance(wsValue, pCodes, unLen, m_pFont, isPlaceHolder ? oPlaceHolderColor : oNormalColor, 1.0, m_oFont.GetSize(), dShiftX, dBaseLine, ppFonts, pShifts);
			RELEASEARRAYOBJECTS(pShifts);
		}

		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);

		pField->SetDefaultAppearance(pFontTT, m_oFont.GetSize(), PdfWriter::TRgb(oColor.r, oColor.g, oColor.b));

		std::wstring wsPlaceHolder = pPr->GetPlaceHolder();
		if (!wsPlaceHolder.empty())
		{
			unsigned int unMaxLen = pPr->GetMaxCharacters();
			if (unMaxLen && wsPlaceHolder.length() > unMaxLen)
				wsPlaceHolder = wsPlaceHolder.substr(0, unMaxLen);

			pField->SetPlaceHolderText(wsPlaceHolder, oNormalColor, oPlaceHolderColor);
		}

		pField->SetFormat(pPr->GetFormatPr());
	}
	else if (oInfo.IsDropDownList())
	{
		const CFormFieldInfo::CDropDownFormPr* pPr = oInfo.GetDropDownPr();
		std::wstring wsValue = pPr->GetTextValue();

		unsigned int unLen = 0;
		unsigned int* pUnicodes = NULL;
		unsigned short* pCodes  = NULL;
		PdfWriter::CFontCidTrueType** ppFonts = NULL;
		bool bFont = GetFontData(pAppFonts, wsValue, m_pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}

		PdfWriter::CChoiceField* pField = dynamic_cast<PdfWriter::CChoiceField*>(pFieldBase);
		if (!pField)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}

		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)));

		TColor oColor = m_oBrush.GetTColor1();

		PdfWriter::TRgb oNormalColor(oColor.r, oColor.g, oColor.b);
		PdfWriter::TRgb oPlaceHolderColor;
		oPlaceHolderColor.r = oNormalColor.r + (1.0 - oNormalColor.r) / 2.0;
		oPlaceHolderColor.g = oNormalColor.g + (1.0 - oNormalColor.g) / 2.0;
		oPlaceHolderColor.b = oNormalColor.b + (1.0 - oNormalColor.b) / 2.0;

		pField->SetTextValue(wsValue);
		pField->SetTextAppearance(wsValue, pCodes, unLen, m_pFont, oInfo.IsPlaceHolder() ? oPlaceHolderColor : oNormalColor, 1, m_oFont.GetSize(), 0, MM_2_PT(dH - oInfo.GetBaseLineOffset()), ppFonts);

		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);

		unsigned int unSelectedIndex = 0xFFFF;
		for (unsigned int unIndex = 0, unItemsCount = pPr->GetComboBoxItemsCount(); unIndex < unItemsCount; ++unIndex)
		{
			std::wstring wsItem = pPr->GetComboBoxItem(unIndex);
			pField->AddOption(wsItem);
			if (wsItem == wsValue)
				unSelectedIndex = unIndex;
		}

		pField->SetComboFlag(true);
		pField->SetEditFlag(pPr->IsEditComboBox());

		pField->SetDefaultAppearance(pFontTT, m_oFont.GetSize(), oInfo.IsPlaceHolder() ? oPlaceHolderColor : oNormalColor);

		if (!pPr->GetPlaceHolder().empty())
		{
			pField->SetPlaceHolderText(pPr->GetPlaceHolder(), oNormalColor, oPlaceHolderColor);

			if (!pPr->IsEditComboBox())
			{
				// Для drop-down list в 0 позиции мы добавили плейсхолдер
				if (oInfo.IsPlaceHolder())
					unSelectedIndex = 0;
				else if (0xFFFF != unSelectedIndex)
					unSelectedIndex++;
			}
		}

		if (!pPr->IsEditComboBox() && 0xFFFF != unSelectedIndex)
			pField->SetSelectedIndex(unSelectedIndex);
	}
	else if (oInfo.IsCheckBox())
	{
		const CFormFieldInfo::CCheckBoxFormPr* pPr = oInfo.GetCheckBoxPr();

		PdfWriter::CCheckBoxField* pField = dynamic_cast<PdfWriter::CCheckBoxField*>(pFieldBase);
		if (!pField)
			return S_FALSE;

		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)));
		pField->SetValue(pPr->IsChecked());

		unsigned int unCheckedSymbol   = pPr->GetCheckedSymbol();
		unsigned int unUncheckedSymbol = pPr->GetUncheckedSymbol();

		unsigned short ushCheckedCode   = pCheckedFont->EncodeUnicode(unCheckedSymbol);
		unsigned short ushUncheckedCode = pUncheckedFont->EncodeUnicode(unUncheckedSymbol);

		TColor oColor = m_oBrush.GetTColor1();
		pField->SetAppearance(L"", &ushCheckedCode, 1, pCheckedFont, L"", &ushUncheckedCode, 1, pUncheckedFont, PdfWriter::TRgb(oColor.r, oColor.g, oColor.b), 1, m_oFont.GetSize(), 0, MM_2_PT(dH - oInfo.GetBaseLineOffset()));
	}
	else if (oInfo.IsPicture())
	{
		const CFormFieldInfo::CPictureFormPr* pPr = oInfo.GetPicturePr();

		PdfWriter::CPictureField* pField = dynamic_cast<PdfWriter::CPictureField*>(pFieldBase);
		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)));
		pField->SetConstantProportions(pPr->IsConstantProportions());
		pField->SetRespectBorders(pPr->IsRespectBorders());
		pField->SetScaleType(static_cast<PdfWriter::CPictureField::EScaleType>(pPr->GetScaleType()));
		pField->SetShift(pPr->GetShiftX() / 1000.0, (1000 - pPr->GetShiftY()) / 1000.0);

		std::wstring wsPath = pPr->GetPicturePath();
		PdfWriter::CImageDict* pImage = NULL;
		if (wsPath.length())
		{
			Aggplus::CImage* pCImage = NULL;
			CImageFileFormatChecker oImageFormat(wsPath);
			if (_CXIMAGE_FORMAT_WMF == oImageFormat.eFileType ||
				_CXIMAGE_FORMAT_EMF == oImageFormat.eFileType ||
				_CXIMAGE_FORMAT_SVM == oImageFormat.eFileType ||
				_CXIMAGE_FORMAT_SVG == oImageFormat.eFileType)
			{
				MetaFile::IMetaFile* pMeta = MetaFile::Create(pAppFonts);
				pMeta->LoadFromFile(wsPath.c_str());

				std::wstring wsTempFile = GetTempFile(wsTempDirectory);
				int nW = 0, nH = 0;
				GetMetafileRasterSize(pMeta, dW, nW, nH);
				pMeta->ConvertToRaster(wsTempFile.c_str(), _CXIMAGE_FORMAT_PNG, nW, nH);

				RELEASEOBJECT(pMeta);

				pCImage = new Aggplus::CImage(wsTempFile);
			}
			else
				pCImage = new Aggplus::CImage(wsPath);

			pImage = LoadImage(pCImage, 255);
			RELEASEOBJECT(pCImage);
		}

		pField->SetAppearance(pImage);
	}
	else if (oInfo.IsSignature())
	{
		const CFormFieldInfo::CSignatureFormPr* pPr = oInfo.GetSignaturePr();

		PdfWriter::CSignatureField* pField = dynamic_cast<PdfWriter::CSignatureField*>(pFieldBase);
		if (!pField)
			return S_FALSE;

		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)));
		pField->SetName(pPr->GetName());
		pField->SetReason(pPr->GetReason());
		pField->SetContact(pPr->GetContact());
		pField->SetDate(pPr->GetDate());

		std::wstring wsPath = pPr->GetPicturePath();
		PdfWriter::CImageDict* pImage = NULL;
		if (!wsPath.empty())
		{
			Aggplus::CImage oImage(wsPath);
			pImage = LoadImage(&oImage, 255);
		}

		pField->SetAppearance(pImage);

		// TODO Реализовать, когда появится поддержка CSignatureField
		pField->SetCert();
	}
	else if (oInfo.IsDateTime())
	{
		const CFormFieldInfo::CDateTimeFormPr* pPr = oInfo.GetDateTimePr();
		
		std::wstring wsValue = pPr->GetValue();
		
		unsigned int unLen = 0;
		unsigned int* pUnicodes = NULL;
		unsigned short* pCodes  = NULL;
		PdfWriter::CFontCidTrueType** ppFonts = NULL;
		bool bFont = GetFontData(pAppFonts, wsValue, m_pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}
		
		PdfWriter::CDateTimeField* pField = dynamic_cast<PdfWriter::CDateTimeField*>(pFieldBase);
		if (!pField)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return S_FALSE;
		}
		
		pFieldBase->AddPageRect(m_pPage, PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)));
		
		TColor oColor = m_oBrush.GetTColor1();
		
		PdfWriter::TRgb oNormalColor(oColor.r, oColor.g, oColor.b);
		PdfWriter::TRgb oPlaceHolderColor;
		oPlaceHolderColor.r = oNormalColor.r + (1.0 - oNormalColor.r) / 2.0;
		oPlaceHolderColor.g = oNormalColor.g + (1.0 - oNormalColor.g) / 2.0;
		oPlaceHolderColor.b = oNormalColor.b + (1.0 - oNormalColor.b) / 2.0;
		
		pField->SetTextValue(wsValue);
		pField->SetTextAppearance(wsValue, pCodes, unLen, m_pFont, oInfo.IsPlaceHolder() ? oPlaceHolderColor : oNormalColor, 1, m_oFont.GetSize(), 0, MM_2_PT(dH - oInfo.GetBaseLineOffset()), ppFonts);
		
		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);

		pField->SetFormat(pPr->GetFormat());
	}

	// Выставляем имя в конце, потому что там возможно копирование настроек поля в новое родительское поле, поэтому к текущему моменту
	// все настройки должны быть выставлены
	if (!bRadioButton)
	{
		std::wstring wsKey = oInfo.GetKey();
		if (L"" != wsKey)
			pFieldBase->SetFieldName(wsKey);
		else
			pFieldBase->SetFieldName("F" + std::to_string(++m_unFieldsCounter));
	}

	return S_OK;
}
HRESULT CPdfWriter::AddAnnotField(NSFonts::IApplicationFonts* pAppFonts, CAnnotFieldInfo* pFieldInfo)
{
	if (!m_pDocument || 0 == m_pDocument->GetPagesCount() || !pFieldInfo)
		return S_OK;

	CAnnotFieldInfo& oInfo = *pFieldInfo;
	PdfWriter::CAnnotation* pAnnot = NULL;

	PdfWriter::CPage* pOrigPage = m_pDocument->GetPage(oInfo.GetPage());
	PdfWriter::CPage* pPage = m_pPage;

	int nID = oInfo.GetID();
	pAnnot = m_pDocument->GetAnnot(nID);

	if (pAnnot && pOrigPage && pPage != pOrigPage)
	{
		pOrigPage->DeleteAnnotation(nID);
		pPage->AddAnnotation(pAnnot);
	}

	BYTE nWidgetType =  0;
	if (oInfo.IsWidget())
	{
		CAnnotFieldInfo::CWidgetAnnotPr* pPr = oInfo.GetWidgetAnnotPr();
		nWidgetType = pPr->GetType();
	}

	if (!pAnnot)
	{
		if (oInfo.IsText())
			pAnnot = m_pDocument->CreateTextAnnot();
		else if (oInfo.IsInk())
			pAnnot = m_pDocument->CreateInkAnnot();
		else if (oInfo.IsLine())
			pAnnot = m_pDocument->CreateLineAnnot();
		else if (oInfo.IsTextMarkup())
			pAnnot = m_pDocument->CreateTextMarkupAnnot();
		else if (oInfo.IsSquareCircle())
			pAnnot = m_pDocument->CreateSquareCircleAnnot();
		else if (oInfo.IsPolygonLine())
			pAnnot = m_pDocument->CreatePolygonLineAnnot();
		else if (oInfo.IsPopup())
			pAnnot = m_pDocument->CreatePopupAnnot();
		else if (oInfo.IsFreeText())
			pAnnot = m_pDocument->CreateFreeTextAnnot();
		else if (oInfo.IsCaret())
			pAnnot = m_pDocument->CreateCaretAnnot();

		if (oInfo.IsWidget())
		{
			switch (nWidgetType)
			{
			case 26:
			{
				pAnnot = m_pDocument->CreateWidgetAnnot();
				break;
			}
			case 27:
			{
				pAnnot = m_pDocument->CreatePushButtonWidget();
				break;
			}
			case 28:
			case 29:
			{
				pAnnot = m_pDocument->CreateCheckBoxWidget();
				break;
			}
			case 30:
			{
				pAnnot = m_pDocument->CreateTextWidget();
				break;
			}
			case 31:
			case 32:
			{
				pAnnot = m_pDocument->CreateChoiceWidget();
				break;
			}
			case 33:
			{
				pAnnot = m_pDocument->CreateSignatureWidget();
				break;
			}
			}
		}

		if (pAnnot)
		{
			m_pDocument->AddAnnotation(nID, pAnnot);
			pPage->AddAnnotation(pAnnot);
		}
	}

	if (!pAnnot)
		return S_FALSE;
	double dX1, dY1, dX2, dY2;
	PdfWriter::CArrayObject* pPageBox = (PdfWriter::CArrayObject*)pPage->Get("CropBox");
	if (!pPageBox)
		pPageBox = (PdfWriter::CArrayObject*)pPage->Get("MediaBox");
	PdfWriter::CObjectBase* pD = pPageBox->Get(3);
	double dPageH = pD->GetType() == PdfWriter::object_type_NUMBER ? ((PdfWriter::CNumberObject*)pD)->Get() : ((PdfWriter::CRealObject*)pD)->Get();
	pD = pPageBox->Get(0);
	double dPageX = pD->GetType() == PdfWriter::object_type_NUMBER ? ((PdfWriter::CNumberObject*)pD)->Get() : ((PdfWriter::CRealObject*)pD)->Get();
	oInfo.GetBounds(dX1, dY1, dX2, dY2);
	pAnnot->SetRect({dPageX + dX1, dPageH - dY1, dPageX + dX2, dPageH - dY2});

	pAnnot->SetPage(pPage, pPage->GetWidth(), dPageH, dPageX);
	pAnnot->SetAnnotFlag(oInfo.GetAnnotFlag());
	pAnnot->SetDocument(m_pDocument);

	int nFlags = oInfo.GetFlag();
	if (nFlags & (1 << 0))
		pAnnot->SetNM(oInfo.GetNM());
	if (nFlags & (1 << 1))
		pAnnot->SetContents(oInfo.GetContents());
	if (nFlags & (1 << 2))
	{
		BYTE nS;
		double dI;
		oInfo.GetBE(nS, dI);
		pAnnot->SetBE(nS, dI);
	}
	if (nFlags & (1 << 3))
		pAnnot->SetC(oInfo.GetC());
	if (nFlags & (1 << 4))
	{
		BYTE nType;
		double dWidth;
		std::vector<double> arrDash;
		oInfo.GetBorder(nType, dWidth, arrDash);
		pAnnot->SetBorder(nType, dWidth, arrDash);
	}
	if (nFlags & (1 << 5))
		pAnnot->SetLM(oInfo.GetLM());

	if (oInfo.IsMarkup())
	{
		CAnnotFieldInfo::CMarkupAnnotPr* pPr = oInfo.GetMarkupAnnotPr();
		PdfWriter::CMarkupAnnotation* pMarkupAnnot = (PdfWriter::CMarkupAnnotation*)pAnnot;

		nFlags = pPr->GetFlag();

		PdfWriter::CAnnotation* pPopupAnnot = NULL;
		if (nFlags & (1 << 0))
		{
			int nPopupID = pPr->GetPopupID();
			if (nPopupID)
				pPopupAnnot = m_pDocument->GetAnnot(nPopupID);
			if (pOrigPage && pPage != pOrigPage)
			{
				pOrigPage->DeleteAnnotation(nPopupID);
				if (pPopupAnnot)
				{
					pPage->AddAnnotation(pPopupAnnot);
					pPopupAnnot->SetPage(pPage);
				}
			}
		}
		if (!pPopupAnnot && !oInfo.IsFreeText())
		{
			pPopupAnnot = pMarkupAnnot->CreatePopup();
			pPage->AddAnnotation(pPopupAnnot);
			pPopupAnnot->SetPage(pPage);
		}
		if (nFlags & (1 << 1))
			pMarkupAnnot->SetT(pPr->GetT());
		if (nFlags & (1 << 2))
			pMarkupAnnot->SetCA(pPr->GetCA());
		if (nFlags & (1 << 3))
		{
			// TODO
			// pMarkupAnnot->SetRC(pPr->GetRC());
		}
		if (nFlags & (1 << 4))
			pMarkupAnnot->SetCD(pPr->GetCD());
		if (nFlags & (1 << 5))
		{
			int nIRTID = pPr->GetIRTID();
			PdfWriter::CAnnotation* pIRTAnnot = m_pDocument->GetAnnot(nIRTID);
			if (pIRTAnnot)
				pMarkupAnnot->SetIRTID(pIRTAnnot);
			else
			{
				PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
				pBase->SetRef(nIRTID, 0);
				pMarkupAnnot->Add("IRT", new PdfWriter::CProxyObject(pBase, true));
			}
		}
		if (nFlags & (1 << 6))
			pMarkupAnnot->SetRT(pPr->GetRT());
		if (nFlags & (1 << 7))
			pMarkupAnnot->SetSubj(pPr->GetSubj());

		pMarkupAnnot->RemoveAP();

		if (oInfo.IsText())
		{
			CAnnotFieldInfo::CTextAnnotPr* pPr = oInfo.GetTextAnnotPr();
			PdfWriter::CTextAnnotation* pTextAnnot = (PdfWriter::CTextAnnotation*)pAnnot;

			pTextAnnot->SetOpen(pPr->IsOpen());
			if (nFlags & (1 << 16))
				pTextAnnot->SetName(pPr->GetName());
			if (nFlags & (1 << 17))
				pTextAnnot->SetStateModel(pPr->GetStateModel());
			if (nFlags & (1 << 18))
				pTextAnnot->SetState(pPr->GetState());
		}
		else if (oInfo.IsInk())
		{
			CAnnotFieldInfo::CInkAnnotPr* pPr = oInfo.GetInkAnnotPr();
			PdfWriter::CInkAnnotation* pInkAnnot = (PdfWriter::CInkAnnotation*)pAnnot;

			pInkAnnot->SetInkList(pPr->GetInkList());
		}
		else if (oInfo.IsLine())
		{
			CAnnotFieldInfo::CLineAnnotPr* pPr = oInfo.GetLineAnnotPr();
			PdfWriter::CLineAnnotation* pLineAnnot = (PdfWriter::CLineAnnotation*)pAnnot;

			double dLX1, dLY1, dLX2, dLY2;
			pPr->GetL(dLX1, dLY1, dLX2, dLY2);
			pLineAnnot->SetL(dLX1, dLY1, dLX2, dLY2);

			if (nFlags & (1 << 15))
			{
				BYTE nLE1, nLE2;
				pPr->GetLE(nLE1, nLE2);
				pLineAnnot->SetLE(nLE1, nLE2);
			}
			if (nFlags & (1 << 16))
				pLineAnnot->SetIC(pPr->GetIC());
			if (nFlags & (1 << 17))
				pLineAnnot->SetLL(pPr->GetLL());
			if (nFlags & (1 << 18))
				pLineAnnot->SetLLE(pPr->GetLLE());
			pLineAnnot->SetCap(pPr->IsCap());
			if (nFlags & (1 << 20))
				pLineAnnot->SetIT(pPr->GetIT());
			if (nFlags & (1 << 21))
				pLineAnnot->SetLLO(pPr->GetLLO());
			if (nFlags & (1 << 22))
				pLineAnnot->SetCP(pPr->GetCP());
			if (nFlags & (1 << 23))
			{
				double dCO1, dCO2;
				pPr->GetCO(dCO1, dCO2);
				pLineAnnot->SetCO(dCO1, dCO2);
			}

			pLineAnnot->SetAP();
		}
		else if (oInfo.IsTextMarkup())
		{
			CAnnotFieldInfo::CTextMarkupAnnotPr* pPr = oInfo.GetTextMarkupAnnotPr();
			PdfWriter::CTextMarkupAnnotation* pTextMarkupAnnot = (PdfWriter::CTextMarkupAnnotation*)pAnnot;

			pTextMarkupAnnot->SetSubtype(pPr->GetSubtype());
			pTextMarkupAnnot->SetQuadPoints(pPr->GetQuadPoints());
		}
		else if (oInfo.IsSquareCircle())
		{
			CAnnotFieldInfo::CSquareCircleAnnotPr* pPr = oInfo.GetSquareCircleAnnotPr();
			PdfWriter::CSquareCircleAnnotation* pSquareCircleAnnot = (PdfWriter::CSquareCircleAnnotation*)pAnnot;

			pSquareCircleAnnot->SetSubtype(pPr->GetSubtype());
			if (nFlags & (1 << 15))
			{
				double dRD1, dRD2, dRD3, dRD4;
				pPr->GetRD(dRD1, dRD2, dRD3, dRD4);
				pSquareCircleAnnot->SetRD(dRD1, dRD2, dRD3, dRD4);
			}
			if (nFlags & (1 << 16))
				pSquareCircleAnnot->SetIC(pPr->GetIC());
		}
		else if (oInfo.IsPolygonLine())
		{
			CAnnotFieldInfo::CPolygonLineAnnotPr* pPr = oInfo.GetPolygonLineAnnotPr();
			PdfWriter::CPolygonLineAnnotation* pPolygonLineAnnot = (PdfWriter::CPolygonLineAnnotation*)pAnnot;

			pPolygonLineAnnot->SetVertices(pPr->GetVertices());
			pPolygonLineAnnot->SetSubtype(pPr->GetSubtype());
			if (nFlags & (1 << 15))
			{
				BYTE nLE1, nLE2;
				pPr->GetLE(nLE1, nLE2);
				pPolygonLineAnnot->SetLE(nLE1, nLE2);
			}
			if (nFlags & (1 << 16))
				pPolygonLineAnnot->SetIC(pPr->GetIC());
			if (nFlags & (1 << 20))
				pPolygonLineAnnot->SetIT(pPr->GetIT());
		}
		else if (oInfo.IsFreeText())
		{
			CAnnotFieldInfo::CFreeTextAnnotPr* pPr = oInfo.GetFreeTextAnnotPr();
			PdfWriter::CFreeTextAnnotation* pFreeTextAnnot = (PdfWriter::CFreeTextAnnotation*)pAnnot;

			pFreeTextAnnot->SetQ(pPr->GetQ());
			if (nFlags & (1 << 15))
			{
				double dRD1, dRD2, dRD3, dRD4;
				pPr->GetRD(dRD1, dRD2, dRD3, dRD4);
				pFreeTextAnnot->SetRD(dRD1, dRD2, dRD3, dRD4);
			}
			if (nFlags & (1 << 16))
				pFreeTextAnnot->SetCL(pPr->GetCL());
			if (nFlags & (1 << 17))
				pFreeTextAnnot->SetDS(pPr->GetDS());
			if (nFlags & (1 << 18))
				pFreeTextAnnot->SetLE(pPr->GetLE());
			if (nFlags & (1 << 20))
				pFreeTextAnnot->SetIT(pPr->GetIT());
		}
		else if (oInfo.IsCaret())
		{
			CAnnotFieldInfo::CCaretAnnotPr* pPr = oInfo.GetCaretAnnotPr();
			PdfWriter::CCaretAnnotation* pCaretAnnot = (PdfWriter::CCaretAnnotation*)pAnnot;

			if (nFlags & (1 << 15))
			{
				double dRD1, dRD2, dRD3, dRD4;
				pPr->GetRD(dRD1, dRD2, dRD3, dRD4);
				pCaretAnnot->SetRD(dRD1, dRD2, dRD3, dRD4);
			}
			if (nFlags & (1 << 16))
				pCaretAnnot->SetSy(pPr->GetSy());
		}
	}
	else if (oInfo.IsPopup())
	{
		CAnnotFieldInfo::CPopupAnnotPr* pPr = oInfo.GetPopupAnnotPr();
		PdfWriter::CPopupAnnotation* pPopupAnnot = (PdfWriter::CPopupAnnotation*)pAnnot;

		nFlags = pPr->GetFlag();
		pPopupAnnot->SetOpen(pPr->IsOpen());
		if (nFlags & (1 << 1))
		{
			int nParentID = pPr->GetParentID();
			PdfWriter::CAnnotation* pParentAnnot = m_pDocument->GetAnnot(nParentID);
			if (pParentAnnot)
				pPopupAnnot->SetParentID(pParentAnnot);
		}
	}
	else if (oInfo.IsWidget())
	{
		CAnnotFieldInfo::CWidgetAnnotPr* pPr = oInfo.GetWidgetAnnotPr();
		PdfWriter::CWidgetAnnotation* pWidgetAnnot = (PdfWriter::CWidgetAnnotation*)pAnnot;

		std::wstring wsFontKey;
		if (nFlags & (1 << 2))
			wsFontKey = pPr->GetFontKey();

		std::wstring wsFontName = pPr->GetFontName();
		if (wsFontName == L"Times-Roman" || wsFontName == L"Times-Bold" || wsFontName == L"Times-BoldItalic" || wsFontName == L"Times-Italic")
			wsFontName = L"Times New Roman";
		int nStyle = pPr->GetFontStyle();
		double dFontSize = pPr->GetFontSizeAP();
		put_FontName(wsFontName);
		put_FontStyle(nStyle);
		put_FontSize(dFontSize);
		PdfWriter::CFontTrueType* pFontTT = NULL;

		if (nWidgetType != 28 && nWidgetType != 29)
		{
			if (m_bNeedUpdateTextFont)
				UpdateFont();
			if (m_pFont)
				pFontTT = m_pDocument->CreateTrueTypeFont(m_pFont);
			pWidgetAnnot->SetDA(pFontTT, pPr->GetFontSize(), dFontSize, pPr->GetTC());
		}

		BYTE nAlign = pPr->GetQ();
		if (nWidgetType != 27 && nWidgetType != 28 && nWidgetType != 29)
			pWidgetAnnot->SetQ(nAlign);
		int nWidgetFlag = pPr->GetFlag();
		pWidgetAnnot->SetSubtype(nWidgetType);
		pWidgetAnnot->SetFlag(nWidgetFlag);

		int nFlags = pPr->GetFlags();
		if (nFlags & (1 << 0))
			pWidgetAnnot->SetTU(pPr->GetTU());
		if (nFlags & (1 << 1))
			pWidgetAnnot->SetDS(pPr->GetDS());
		if (nFlags & (1 << 3))
			pWidgetAnnot->SetH(pPr->GetH());
		if (nFlags & (1 << 5))
			pWidgetAnnot->SetBC(pPr->GetBC());
		if (nFlags & (1 << 6))
			pWidgetAnnot->SetR(pPr->GetR());
		if (nFlags & (1 << 7))
			pWidgetAnnot->SetBG(pPr->GetBG());
		if (nFlags & (1 << 8))
			pWidgetAnnot->SetDV(pPr->GetDV());
		if (nFlags & (1 << 18))
			pWidgetAnnot->SetT(pPr->GetT());

		const std::vector<CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget*> arrActions = pPr->GetActions();
		for (CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* pAction : arrActions)
		{
			PdfWriter::CAction* pA = (PdfWriter::CAction*)m_pDocument->CreateAction(pAction->nActionType);
			if (!pA)
				continue;
			pA->SetType(pAction->wsType);

			switch (pAction->nActionType)
			{
			case 1:
			{
				PdfWriter::CActionGoTo* ppA = (PdfWriter::CActionGoTo*)pA;
				PdfWriter::CPage* pPageD = m_pDocument->GetPage(pAction->nInt1);
				PdfWriter::CDestination* pDest = m_pDocument->CreateDestination(pPageD);
				if (!pDest)
					break;
				ppA->SetDestination(pDest);

				PdfWriter::CArrayObject* pPageBoxD = (PdfWriter::CArrayObject*)pPageD->Get("CropBox");
				if (!pPageBoxD)
					pPageBoxD = (PdfWriter::CArrayObject*)pPageD->Get("MediaBox");
				if (!pPageBoxD)
					pPageBoxD = pPageBox;
				pD = pPageBoxD->Get(3);
				double dPageDH = pD->GetType() == PdfWriter::object_type_NUMBER ? ((PdfWriter::CNumberObject*)pD)->Get() : ((PdfWriter::CRealObject*)pD)->Get();
				pD = pPageBoxD->Get(0);
				double dPageDX = pD->GetType() == PdfWriter::object_type_NUMBER ? ((PdfWriter::CNumberObject*)pD)->Get() : ((PdfWriter::CRealObject*)pD)->Get();

				switch (pAction->nKind)
				{
				case 0:
				{
					pDest->SetXYZ(pAction->dD[0] + dPageDX, dPageDH - pAction->dD[1], pAction->dD[2]);
					break;
				}
				case 1:
				{
					pDest->SetFit();
					break;
				}
				case 2:
				{
					pDest->SetFitH(dPageDH - pAction->dD[1]);
					break;
				}
				case 3:
				{
					pDest->SetFitV(pAction->dD[0] + dPageDX);
					break;
				}
				case 4:
				{
					pDest->SetFitR(pAction->dD[0] + dPageDX, dPageDH - pAction->dD[1], pAction->dD[2] + dPageDX, dPageDH - pAction->dD[3]);
					break;
				}
				case 5:
				{
					pDest->SetFitB();
					break;
				}
				case 6:
				{
					pDest->SetFitBH(dPageDH - pAction->dD[1]);
					break;
				}
				case 7:
				{
					pDest->SetFitBV(pAction->dD[0] + dPageDX);
					break;
				}
				}
				break;
			}
			case 6:
			{
				PdfWriter::CActionURI* ppA = (PdfWriter::CActionURI*)pA;
				ppA->SetURI(pAction->wsStr1);
				break;
			}
			case 9:
			{
				PdfWriter::CActionHide* ppA = (PdfWriter::CActionHide*)pA;
				ppA->SetH(pAction->nKind);
				ppA->SetT(pAction->arrStr);
				break;
			}
			case 10:
			{
				PdfWriter::CActionNamed* ppA = (PdfWriter::CActionNamed*)pA;
				ppA->SetN(pAction->wsStr1);
				break;
			}
			case 12:
			{
				PdfWriter::CActionResetForm* ppA = (PdfWriter::CActionResetForm*)pA;
				ppA->SetFlags(pAction->nInt1);
				ppA->SetFields(pAction->arrStr);
				break;
			}
			case 14:
			{
				PdfWriter::CActionJavaScript* ppA = (PdfWriter::CActionJavaScript*)pA;
				ppA->SetJS(pAction->wsStr1);
				break;
			}
			}
			pWidgetAnnot->AddAction(pA);
		}

		bool isBold   = (nStyle & 1 ? true : false);
		bool isItalic = (nStyle & 2 ? true : false);

		if (oInfo.IsButtonWidget())
		{
			if (nWidgetType == 27)
			{
				CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr* pPr = oInfo.GetWidgetAnnotPr()->GetButtonWidgetPr();
				PdfWriter::CPushButtonWidget* pButtonWidget = (PdfWriter::CPushButtonWidget*)pAnnot;

				BYTE nTP = 0;
				if (nFlags & (1 << 13))
				{
					nTP = pPr->GetTP();
					pButtonWidget->SetTP(nTP);
				}

				int nIFFlags = pPr->GetIFFlag();
				pButtonWidget->SetIFFlag(nIFFlags);
				if (nIFFlags & (1 << 0))
				{
					if (nIFFlags & (1 << 1))
						pButtonWidget->SetSW(pPr->GetSW());
					if (nIFFlags & (1 << 2))
						pButtonWidget->SetS(pPr->GetS());
					if (nIFFlags & (1 << 3))
					{
						double d1, d2;
						pPr->GetA(d1, d2);
						pButtonWidget->SetA(d1, d2);
					}
				}

				if (nIFFlags & (1 << 5))
					pButtonWidget->SetI(pPr->GetI());
				if (nIFFlags & (1 << 6))
					pButtonWidget->SetRI(pPr->GetRI());
				if (nIFFlags & (1 << 7))
					pButtonWidget->SetIX(pPr->GetIX());

				// ВНЕШНИЙ ВИД
				pButtonWidget->SetFont(m_pFont, dFontSize, isBold, isItalic);
				if (nFlags & (1 << 10))
				{
					pButtonWidget->SetCA(pPr->GetCA());
					if (nTP == 0)
						DrawButtonWidget(pAppFonts, pButtonWidget, 0, NULL);
				}
				if (nFlags & (1 << 11))
				{
					pButtonWidget->SetRC(pPr->GetRC());
					if (nTP == 0)
						DrawButtonWidget(pAppFonts, pButtonWidget, 1, NULL);
				}
				if (nFlags & (1 << 12))
				{
					pButtonWidget->SetAC(pPr->GetAC());
					if (nTP == 0)
						DrawButtonWidget(pAppFonts, pButtonWidget, 2, NULL);
				}
			}
			else
			{
				CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr* pPrB = oInfo.GetWidgetAnnotPr()->GetButtonWidgetPr();
				PdfWriter::CCheckBoxWidget* pButtonWidget = (PdfWriter::CCheckBoxWidget*)pAnnot;

				std::wstring wsValue;
				if (nFlags & (1 << 14))
					pButtonWidget->SetAP_N_Yes(pPrB->GetAP_N_Yes());
				if (nFlags & (1 << 9))
				{
					wsValue = pPrB->GetV();
					pButtonWidget->SetV(wsValue);
				}

				std::wstring wsStyleValue = pButtonWidget->SetStyle(pPrB->GetStyle());

				// ВНЕШНИЙ ВИД
				// Если изменился текущий внешний вид
				if (pButtonWidget->Get("AP"))
				{
					if (!wsValue.empty())
						pButtonWidget->SwitchAP(U_TO_UTF8(wsValue));
					return S_OK;
				}

				if (m_bNeedUpdateTextFont)
					UpdateFont();

				if (m_pFont)
					pFontTT = m_pDocument->CreateTrueTypeFont(m_pFont);
				pWidgetAnnot->SetDA(pFontTT, pPr->GetFontSize(), dFontSize, pPr->GetTC());

				pButtonWidget->SetFont(m_pFont, dFontSize, isBold, isItalic);
				if (!wsStyleValue.empty())
				{
					double dMargin = 2;
					double dBaseLine = dY2 - dY1 - dFontSize - dMargin;
					pButtonWidget->SetAP(wsStyleValue, NULL, 0, 0, dBaseLine, NULL, NULL);
				}
			}
		}
		else if (oInfo.IsTextWidget())
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr* pPr = oInfo.GetWidgetAnnotPr()->GetTextWidgetPr();
			PdfWriter::CTextWidget* pTextWidget = (PdfWriter::CTextWidget*)pAnnot;

			std::wstring wsValue;
			bool bValue = false;
			if (nFlags & (1 << 9))
			{
				bValue = true;
				wsValue = pPr->GetV();
				pTextWidget->SetV(wsValue);
			}
			if (nFlags & (1 << 10))
				pTextWidget->SetMaxLen(pPr->GetMaxLen());
			if (nWidgetFlag & (1 << 25))
				pTextWidget->SetRV(pPr->GetRV());
			bool bAPValue = false;
			if (nFlags & (1 << 12))
			{
				bAPValue = true;
				wsValue = pPr->GetAPV();
				pTextWidget->SetAPV();
			}

			// ВНЕШНИЙ ВИД
			pTextWidget->SetFont(m_pFont, dFontSize, isBold, isItalic);
			if ((bValue && pTextWidget->Get("T")) || bAPValue)
				DrawTextWidget(pAppFonts, pTextWidget, wsValue);
		}
		else if (oInfo.IsChoiceWidget())
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr* pPr = oInfo.GetWidgetAnnotPr()->GetChoiceWidgetPr();
			PdfWriter::CChoiceWidget* pChoiceWidget = (PdfWriter::CChoiceWidget*)pAnnot;

			std::vector<std::wstring> arrValue;
			if (nFlags & (1 << 9))
			{
				arrValue.push_back(pPr->GetV());
				pChoiceWidget->SetV(arrValue.back());
			}
			if (nFlags & (1 << 10))
				pChoiceWidget->SetOpt(pPr->GetOpt());
			if (nFlags & (1 << 11))
				pChoiceWidget->SetTI(pPr->GetTI());
			if (nFlags & (1 << 12))
				arrValue[arrValue.size()] = pPr->GetAPV();
			if (nFlags & (1 << 13))
				pChoiceWidget->SetV(pPr->GetArrV());
			if (nFlags & (1 << 13))
				pChoiceWidget->SetI(pPr->GetI());

			// ВНЕШНИЙ ВИД
			pChoiceWidget->SetFont(m_pFont, dFontSize, isBold, isItalic);
			if (!arrValue.empty())
				DrawChoiceWidget(pAppFonts, pChoiceWidget, arrValue);
		}
		else if (oInfo.IsSignatureWidget())
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CSignatureWidgetPr* pPr = oInfo.GetWidgetAnnotPr()->GetSignatureWidgetPr();
			PdfWriter::CSignatureWidget* pSignatureWidget = (PdfWriter::CSignatureWidget*)pAnnot;
		}
	}

	return S_OK;
}
HRESULT CPdfWriter::AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength)
{
	return m_pDocument->AddMetaData(sMetaName, pMetaData, nMetaLength) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------------------
// Дополнительные функции Pdf рендерера
//----------------------------------------------------------------------------------------
HRESULT CPdfWriter::DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
	m_oCommandManager.Flush();

	if (!IsPageValid() || !pImageBuffer)
		return S_OK;

	m_pPage->GrSave();
	UpdateTransform();
	
	PdfWriter::CImageDict* pPdfImage = m_pDocument->CreateImage();
	pPdfImage->LoadBW(pImageBuffer, unWidth, unHeight);
	m_pPage->DrawImage(pPdfImage, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH));

	m_pPage->GrRestore();
	return S_OK;
}
HRESULT CPdfWriter::EnableBrushRect(const LONG& lEnable)
{
	m_oBrush.EnableBrushRect(LONG_2_BOOL(lEnable));
	return S_OK;
}
HRESULT CPdfWriter::SetLinearGradient(const double& dX0, const double& dY0, const double& dX1, const double& dY1)
{
	m_oBrush.SetType(c_BrushTypeLinearGradient);
	m_oBrush.SetLinearGradientPattern(dX0, dY0, dX1, dY1);
	return S_OK;
}
HRESULT CPdfWriter::SetRadialGradient(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
{
	m_oBrush.SetType(c_BrushTypeRadialGradient);
	m_oBrush.SetRadialGradientPattern(dX0, dY0, dR0, dX1, dY1, dR1);
	return S_OK;
}
HRESULT CPdfWriter::DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
	m_oCommandManager.Flush();

	if (!IsPageValid() || !pMaskBuffer || !pImage)
		return S_OK;

	PdfWriter::CImageDict* pPdfImage = LoadImage((Aggplus::CImage*)pImage, 255);
	if (!pPdfImage)
		return S_OK;

	m_pPage->GrSave();
	UpdateTransform();
	pPdfImage->LoadMask(pMaskBuffer, unMaskWidth, unMaskHeight);
	m_pPage->DrawImage(pPdfImage, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH));
	m_pPage->GrRestore();
	return S_OK;
}
HRESULT CPdfWriter::EditWidgetParents(NSFonts::IApplicationFonts* pAppFonts, CWidgetsInfo* pFieldInfo, const std::wstring& wsTempDirectory)
{
	if (!m_pDocument || 0 == m_pDocument->GetPagesCount() || !pFieldInfo)
		return S_OK;

	if (!m_pDocument->EditCO(pFieldInfo->GetCO()))
		return S_FALSE;

	std::vector<CWidgetsInfo::CParent*> arrParents = pFieldInfo->GetParents();
	for (CWidgetsInfo::CParent* pParent : arrParents)
	{
		PdfWriter::CDictObject* pParentObj = m_pDocument->GetParent(pParent->nID);
		if (!pParentObj)
			continue;

		std::vector<std::wstring> arrValue;

		int nFlags = pParent->nFlags;
		if (nFlags & (1 << 0))
			pParentObj->Add("T", new PdfWriter::CStringObject((U_TO_UTF8(pParent->sName)).c_str(), true));
		if (nFlags & (1 << 1))
		{
			std::string sV = U_TO_UTF8(pParent->sV);
			bool bName = !sV.empty() && (iswdigit(pParent->sV[0]) || sV == "Off");

			PdfWriter::CObjectBase* pKids = pParentObj->Get("Kids");
			if (pKids && pKids->GetType() == PdfWriter::object_type_ARRAY)
			{
				PdfWriter::CArrayObject* pAKids = (PdfWriter::CArrayObject*)pKids;
				for (int i = 0; i < pAKids->GetCount(); ++i)
				{
					PdfWriter::CObjectBase* pObj = pAKids->Get(i);
					if (pObj->GetType() != PdfWriter::object_type_DICT ||
						((PdfWriter::CDictObject*)pObj)->GetDictType() != PdfWriter::dict_type_ANNOTATION ||
						((PdfWriter::CAnnotation*)pObj)->GetAnnotationType() != PdfWriter::AnnotWidget)
						continue;
					PdfWriter::EWidgetType nType = ((PdfWriter::CWidgetAnnotation*)pObj)->GetWidgetType();
					if (nType == PdfWriter::WidgetCheckbox || nType == PdfWriter::WidgetRadiobutton)
					{
						PdfWriter::CCheckBoxWidget* pKid = dynamic_cast<PdfWriter::CCheckBoxWidget*>(pObj);
						if (pKid)
							pKid->SwitchAP(sV);
					}
					if (nType == PdfWriter::WidgetCombobox || nType == PdfWriter::WidgetListbox)
					{
						PdfWriter::CChoiceWidget* pKid = dynamic_cast<PdfWriter::CChoiceWidget*>(pObj);
						DrawChoiceWidget(pAppFonts, pKid, {pParent->sV});
						bName = false;
					}
					if (nType == PdfWriter::WidgetText)
					{
						PdfWriter::CTextWidget* pKid = dynamic_cast<PdfWriter::CTextWidget*>(pObj);
						if (!pKid->HaveAPV())
							DrawTextWidget(pAppFonts, pKid, pParent->sV);
						bName = false;
					}
				}
			}

			if (bName)
				pParentObj->Add("V", sV.c_str());
			else
				pParentObj->Add("V", new PdfWriter::CStringObject(sV.c_str(), true));
		}
		if (nFlags & (1 << 2))
			pParentObj->Add("DV", new PdfWriter::CStringObject((U_TO_UTF8(pParent->sDV)).c_str(), true));
		if (nFlags & (1 << 3))
		{
			PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
			pParentObj->Add("I", pArray);
			for (int i = 0; i < pParent->arrI.size(); ++i)
				pArray->Add(pParent->arrI[i]);
		}
		else
			pParentObj->Remove("I");
		if (nFlags & (1 << 4))
		{
			PdfWriter::CDictObject* pParentObj2 = m_pDocument->GetParent(pParent->nParentID);
			if (pParentObj2)
				pParentObj->Add("Parent", pParentObj2);
		}
		if (nFlags & (1 << 5))
		{
			PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
			pParentObj->Add("V", pArray);
			for (int i = 0; i < pParent->arrV.size(); ++i)
				pArray->Add(new PdfWriter::CStringObject(U_TO_UTF8(pParent->arrV[i]).c_str(), true));
			PdfWriter::CObjectBase* pKids = pParentObj->Get("Kids");
			if (pKids && pKids->GetType() == PdfWriter::object_type_ARRAY)
			{
				PdfWriter::CArrayObject* pAKids = (PdfWriter::CArrayObject*)pKids;
				for (int i = 0; i < pAKids->GetCount(); ++i)
				{
					PdfWriter::CObjectBase* pObj = pAKids->Get(i);
					if (pObj->GetType() != PdfWriter::object_type_DICT ||
						((PdfWriter::CDictObject*)pObj)->GetDictType() != PdfWriter::dict_type_ANNOTATION ||
						((PdfWriter::CAnnotation*)pObj)->GetAnnotationType() != PdfWriter::AnnotWidget)
						continue;
					PdfWriter::EWidgetType nType = ((PdfWriter::CWidgetAnnotation*)pObj)->GetWidgetType();
					if (nType == PdfWriter::WidgetCombobox || nType == PdfWriter::WidgetListbox)
					{
						PdfWriter::CChoiceWidget* pKid = dynamic_cast<PdfWriter::CChoiceWidget*>(pObj);
						DrawChoiceWidget(pAppFonts, pKid, pParent->arrV);
					}
				}
			}
		}
	}

	std::vector<std::wstring> arrBI = pFieldInfo->GetButtonImg();
	std::vector<PdfWriter::CXObject*> arrForm;
	for (int i = 0; i < arrBI.size(); ++i)
	{
		std::wstring wsPath = arrBI[i];
		if (wsPath.empty())
		{
			arrForm.push_back(NULL);
			continue;
		}
		std::wstring sTempImagePath = GetDownloadFile(wsPath, wsTempDirectory);
		std::wstring wsImagePath = sTempImagePath.empty() ? wsPath : sTempImagePath;

		Aggplus::CImage* pCImage = NULL;
		CImageFileFormatChecker oImageFormat(wsImagePath);
		if (_CXIMAGE_FORMAT_WMF == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_EMF == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_SVM == oImageFormat.eFileType ||
			_CXIMAGE_FORMAT_SVG == oImageFormat.eFileType)
		{
			MetaFile::IMetaFile* pMeta = MetaFile::Create(pAppFonts);
			pMeta->LoadFromFile(wsPath.c_str());

			std::wstring wsTempFile = GetTempFile(wsTempDirectory);
			int nW = 0, nH = 0;
			GetMetafileRasterSize(pMeta, 10, nW, nH);
			pMeta->ConvertToRaster(wsTempFile.c_str(), _CXIMAGE_FORMAT_PNG, nW, nH);

			RELEASEOBJECT(pMeta);

			pCImage = new Aggplus::CImage(wsTempFile);
		}
		else
			pCImage = new Aggplus::CImage(wsImagePath);

		PdfWriter::CImageDict* pImage = LoadImage(pCImage, 255);
		RELEASEOBJECT(pCImage);

		arrForm.push_back(m_pDocument->CreateForm(pImage, std::to_string(i)));
	}

	if (arrForm.empty())
		return S_OK;

	std::map<int, PdfWriter::CAnnotation*> mAnnots = m_pDocument->GetAnnots();
	for (auto it = mAnnots.begin(); it != mAnnots.end(); it++)
	{
		PdfWriter::CAnnotation* pAnnot = it->second;
		if (pAnnot->GetAnnotationType() != PdfWriter::AnnotWidget || ((PdfWriter::CWidgetAnnotation*)pAnnot)->GetWidgetType() != PdfWriter::WidgetPushbutton)
			continue;

		PdfWriter::CPushButtonWidget* pPBWidget = (PdfWriter::CPushButtonWidget*)pAnnot;
		if (pPBWidget->m_nI >= 0)
			DrawButtonWidget(pAppFonts, pPBWidget, 0, arrForm[pPBWidget->m_nI]);
		if (pPBWidget->m_nRI >= 0)
			DrawButtonWidget(pAppFonts, pPBWidget, 1, arrForm[pPBWidget->m_nRI]);
		if (pPBWidget->m_nIX >= 0)
			DrawButtonWidget(pAppFonts, pPBWidget, 2, arrForm[pPBWidget->m_nIX]);
	}

	return S_OK;
}
bool CPdfWriter::EditPage(PdfWriter::CPage* pNewPage)
{
	if (!IsValid())
		return false;
	m_oCommandManager.Flush();

	m_pPage = pNewPage;
	if (m_pPage)
	{
		m_dPageWidth  = PT_2_MM(m_pPage->GetWidth());
		m_dPageHeight = PT_2_MM(m_pPage->GetHeight());
		Reset();
		return true;
	}
	return false;
}
bool CPdfWriter::AddPage(int nPageIndex)
{
	if (!IsValid())
		return false;
	m_oCommandManager.Flush();

	m_pPage = m_pDocument->AddPage(nPageIndex);
	if (m_pPage)
	{
		m_pPage->SetWidth(MM_2_PT(m_dPageWidth));
		m_pPage->SetHeight(MM_2_PT(m_dPageHeight));
		Reset();
		return true;
	}
	return false;
}
bool CPdfWriter::EditClose()
{
	if (!IsValid())
		return false;
	m_oCommandManager.Flush();

	unsigned int nPagesCount = m_pDocument->GetPagesCount();
	for (int nIndex = 0, nCount = m_vDestinations.size(); nIndex < nCount; ++nIndex)
	{
		TDestinationInfo& oInfo = m_vDestinations.at(nIndex);
		if (nPagesCount > oInfo.unDestPage)
		{
			AddLink(oInfo.pPage, oInfo.dX, oInfo.dY, oInfo.dW, oInfo.dH, oInfo.dDestX, oInfo.dDestY, oInfo.unDestPage);
			m_vDestinations.erase(m_vDestinations.begin() + nIndex);
			nIndex--;
			nCount--;
		}
	}

	return true;
}
void CPdfWriter::PageRotate(int nRotate)
{
	if (m_pPage)
		m_pPage->SetRotate(nRotate);
}
void CPdfWriter::Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate)
{
	PdfWriter::CImageDict* pImage = NULL;
	if (!wsPicturePath.empty())
	{
		Aggplus::CImage oImage(wsPicturePath);
		pImage = LoadImage(&oImage, 255);
	}

	m_pDocument->Sign(PdfWriter::TRect(MM_2_PT(dX), m_pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), m_pPage->GetHeight() - MM_2_PT(dY + dH)),
					  pImage, pCertificate);
}
//----------------------------------------------------------------------------------------
// Внутренние функции
//----------------------------------------------------------------------------------------
PdfWriter::CImageDict* CPdfWriter::LoadImage(Aggplus::CImage* pImage, const BYTE& nAlpha)
{
	TColor oColor;
	int nImageW = abs((int)pImage->GetWidth());
	int nImageH = abs((int)pImage->GetHeight());
	BYTE* pData = pImage->GetData();
	int nStride = 4 * nImageW;

	// Картинки совсем маленьких размеров нельзя делать Jpeg2000
	bool bJpeg = false;
	if (nImageH < 100 || nImageW < 100 || m_pDocument->IsPDFA())
		bJpeg = true;

	if (nImageH <= 0 || nImageW <= 0)
		return NULL;

	// TODO: Пока не разберемся как в CxImage управлять параметрами кодирования нельзя писать в Jpeg2000,
	//       т.к. файлы получаются гораздо больше и конвертация идет намного дольше.
	bJpeg = true;

	// Пробегаемся по картинке и определяем есть ли у нас альфа-канал
	bool bAlpha = false;

	CBgraFrame oFrame;
	if (m_pDocument->IsPDFA())
	{
		BYTE* pCopyImage = new BYTE[4 * nImageW * nImageH];
		if (!pCopyImage)
			return NULL;

		::memcpy(pCopyImage, pData, 4 * nImageW * nImageH);

		BYTE* pDataMem = pCopyImage;
		for (int nIndex = 0, nSize = nImageW * nImageH; nIndex < nSize; nIndex++)
		{
			if (pDataMem[3] < 32)
			{
				pDataMem[0] = 255;
				pDataMem[1] = 255;
				pDataMem[2] = 255;
			}
			pDataMem += 4;
		}

		oFrame.put_Width(nImageW);
		oFrame.put_Height(nImageH);
		oFrame.put_Data(pCopyImage);// +4 * (nImageH - 1) * nImageW);
		oFrame.put_Stride(-4* nImageW);
	}
	else
	{
		BYTE* pDataMem = pData;
		for (int nIndex = 0, nSize = nImageW * nImageH; nIndex < nSize; nIndex++)
		{
			// making full-transparent pixels white
			if (pDataMem[3] == 0)
			{
				pDataMem[0] = 255;
				pDataMem[1] = 255;
				pDataMem[2] = 255;
			}

			if (!bAlpha && (pDataMem[3] < 255))
			{
				bAlpha = true;
			}

			pDataMem += 4;
		}
		oFrame.FromImage(pImage);
	}

	oFrame.SetJpegQuality(85.0);

	BYTE* pBuffer = NULL;
	int nBufferSize = 0;
	if (!oFrame.Encode(pBuffer, nBufferSize, bJpeg ? _CXIMAGE_FORMAT_JPG : _CXIMAGE_FORMAT_JP2))
		return NULL;

	if (!pBuffer || !nBufferSize)
		return NULL;

	PdfWriter::CImageDict* pPdfImage = m_pDocument->CreateImage();
	if (bAlpha || nAlpha < 255)
		pPdfImage->LoadSMask(pData, nImageW, nImageH, nAlpha, (pImage->GetStride() >= 0) ? false : true);

	if (bJpeg)
		pPdfImage->LoadJpeg(pBuffer, nBufferSize, nImageW, nImageH);
	else
		pPdfImage->LoadJpx(pBuffer, nBufferSize, nImageW, nImageH);

	free(pBuffer);

	return pPdfImage;
}
bool CPdfWriter::DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
	PdfWriter::CImageDict* pPdfImage = LoadImage(pImage, nAlpha);
	if (!pPdfImage)
		return false;

	m_pPage->GrSave();
	UpdateTransform();
	m_pPage->DrawImage(pPdfImage, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY - dH), MM_2_PT(dW), MM_2_PT(dH));
	m_pPage->GrRestore();
	
	return true;
}
bool CPdfWriter::DrawText(unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY)
{
	if (!pCodes || !unLen)
		return false;

	CTransform& t = m_oTransform;
	m_oCommandManager.SetTransform(t.m11, -t.m12, -t.m21, t.m22, MM_2_PT(t.dx + t.m21 * m_dPageHeight), MM_2_PT(m_dPageHeight - m_dPageHeight * t.m22 - t.dy));

	CRendererTextCommand* pText = m_oCommandManager.AddText(pCodes, unLen, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY));
	pText->SetFont(m_pFont);
	pText->SetSize(m_oFont.GetSize());
	pText->SetColor(m_oBrush.GetColor1());
	pText->SetAlpha((BYTE)m_oBrush.GetAlpha1());
	pText->SetCharSpace(MM_2_PT(m_oFont.GetCharSpace()));
	pText->SetNeedDoBold(m_oFont.IsNeedDoBold());
	pText->SetNeedDoItalic(m_oFont.IsNeedDoItalic());

	return true;
}
bool CPdfWriter::DrawTextToRenderer(const unsigned int* unGid, const unsigned int& unLen, const double& dX, const double& dY)
{
	// TODO pdf позволяет создание своего шрифта, но не следует это использовать для воссоздания шрифта запрещенного для редактирования или встраивания
	Aggplus::CGraphicsPathSimpleConverter simplifier;
	simplifier.SetRenderer(m_pRenderer);
	m_pFontManager->LoadFontByName(m_oFont.GetName(), m_oFont.GetSize(), (int)m_oFont.GetStyle(), 72.0, 72.0);
	PathCommandEnd();
	if (simplifier.PathCommandText2(L"", (const int*)unGid, unLen, m_pFontManager, dX, dY, 0, 0))
	{
		DrawPath(NULL, L"", c_nWindingFillMode);
		PathCommandEnd();
		return true;
	}
	return false;
}
bool CPdfWriter::PathCommandDrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids)
{
	unsigned char* pCodes = EncodeString(pUnicodes, unLen, pGids);
	if (!pCodes)
		return false;

	m_oPath.AddText(m_pFont, pCodes, unLen * 2, MM_2_PT(dX), MM_2_PT(m_dPageHeight - dY), m_oFont.GetSize(), MM_2_PT(m_oFont.GetCharSpace()));
	return true;
}
bool CPdfWriter::UpdateFont()
{
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
	return true;
}
bool CPdfWriter::GetFontPath(const std::wstring &wsFontName, const bool &bBold, const bool &bItalic, std::wstring& wsFontPath, LONG& lFaceIndex)
{
	bool bFind = false;
	for (int nIndex = 0, nCount = m_vFonts.size(); nIndex < nCount; nIndex++)
	{
		TFontInfo& oInfo = m_vFonts.at(nIndex);
		if (oInfo.wsFontName == wsFontName && oInfo.bBold == bBold && oInfo.bItalic == bItalic)
		{
			wsFontPath = oInfo.wsFontPath;
			lFaceIndex = oInfo.lFaceIndex;
			bFind = true;
			break;
		}
	}

	if (!bFind)
	{
		NSFonts::CFontSelectFormat oFontSelect;
		oFontSelect.wsName  = new std::wstring(wsFontName);
		oFontSelect.bItalic = new INT(bItalic ? 1 : 0);
		oFontSelect.bBold   = new INT(bBold ? 1 : 0);
		NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelect, false);
		if (!NSFonts::CFontInfo::CanEmbedForPreviewAndPrint(pFontInfo->m_usType))
		{
			return false;

			oFontSelect.Fill(pFontInfo);
			if (NULL != oFontSelect.usType)
				*oFontSelect.usType = NSFONTS_EMBEDDING_RIGHTS_PRINT_AND_PREVIEW;
			else
				oFontSelect.usType = new USHORT(NSFONTS_EMBEDDING_RIGHTS_PRINT_AND_PREVIEW);

			pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelect, false);
		}

		wsFontPath = pFontInfo->m_wsFontPath;
		lFaceIndex = pFontInfo->m_lIndex;

		m_vFonts.push_back(TFontInfo(wsFontName, bBold, bItalic, wsFontPath, lFaceIndex));
	}
	return true;
}
PdfWriter::CFontCidTrueType* CPdfWriter::GetFont(const std::wstring& wsFontPath, const LONG& lFaceIndex)
{
	PdfWriter::CFontCidTrueType* pFont = NULL;
	if (L"" != wsFontPath)
	{
		pFont = m_pDocument->FindCidTrueTypeFont(wsFontPath, lFaceIndex);
		if (pFont)
			return pFont;

		// TODO: Пока мы здесь предполагаем, что шрифты только либо TrueType, либо OpenType
		if (!m_pFontManager->LoadFontFromFile(wsFontPath, lFaceIndex, 10, 72, 72))
		{
			std::wcout << L"PDF Writer: Can't load fontfile " << wsFontPath.c_str() << "\n";
			return NULL;
		}

		std::wstring wsFontType = m_pFontManager->GetFontType();
		if (L"TrueType" == wsFontType || L"OpenType" == wsFontType || L"CFF" == wsFontType)
			pFont = m_pDocument->CreateCidTrueTypeFont(wsFontPath, lFaceIndex);
	}

	return pFont;
}
PdfWriter::CFontCidTrueType* CPdfWriter::GetFont(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic)
{
	std::wstring wsFontPath;
	LONG lFaceIndex;

	GetFontPath(wsFontName, bBold, bItalic, wsFontPath, lFaceIndex);
	return GetFont(wsFontPath, lFaceIndex);
}
bool CPdfWriter::GetFontData(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsValue, PdfWriter::CFontCidTrueType* pFont, bool bBold, bool bItalic,
							 unsigned int*& pUnicodes, unsigned int& unLen, unsigned short*& pCodes, PdfWriter::CFontCidTrueType**& ppFonts)
{
	pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsValue, unLen);
	if (!pUnicodes)
		return false;

	pCodes = new unsigned short[unLen];
	if (!pCodes)
	{
		RELEASEARRAYOBJECTS(pUnicodes);
		return false;
	}

	ppFonts = new PdfWriter::CFontCidTrueType*[unLen];
	if (!ppFonts)
	{
		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		return false;
	}

	for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
	{
		unsigned int unUnicode = pUnicodes[unIndex];

		if (!pFont->HaveChar(unUnicode))
		{
			std::wstring wsFontFamily = pAppFonts->GetFontBySymbol(unUnicode);
			PdfWriter::CFontCidTrueType* pTempFont = GetFont(wsFontFamily, bBold, bItalic);
			if (pTempFont)
			{
				pCodes[unIndex]  = pTempFont->EncodeUnicode(unUnicode);
				ppFonts[unIndex] = pTempFont;
				continue;
			}
		}
		pCodes[unIndex]  = pFont->EncodeUnicode(unUnicode);
		ppFonts[unIndex] = pFont;
	}

	return true;
}
void CPdfWriter::UpdateTransform()
{
	CTransform& t = m_oTransform;
	m_pPage->SetTransform(t.m11, -t.m12, -t.m21, t.m22, MM_2_PT(t.dx + t.m21 * m_dPageHeight), MM_2_PT(m_dPageHeight - m_dPageHeight * t.m22 - t.dy));
}
void CPdfWriter::UpdatePen()
{
	TColor oColor = m_oPen.GetTColor();
	m_pPage->SetStrokeColor(oColor.r, oColor.g, oColor.b);
	m_pPage->SetStrokeAlpha((unsigned char)m_oPen.GetAlpha());
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

	LONG lCapStyle = m_oPen.GetStartCapStyle();
	if (Aggplus::LineCapRound == lCapStyle)
		m_pPage->SetLineCap(PdfWriter::linecap_Round);
	else if (Aggplus::LineCapSquare == lCapStyle)
		m_pPage->SetLineCap(PdfWriter::linecap_ProjectingSquare);
	else
		m_pPage->SetLineCap(PdfWriter::linecap_Butt);

	LONG lJoinStyle = m_oPen.GetJoinStyle();
	if (Aggplus::LineJoinBevel == lJoinStyle)
		m_pPage->SetLineJoin(PdfWriter::linejoin_Bevel);
	else if (Aggplus::LineJoinMiter == lJoinStyle)
	{
		m_pPage->SetLineJoin(PdfWriter::linejoin_Miter);
		m_pPage->SetMiterLimit(MM_2_PT(m_oPen.GetMiter()));
	}
	else
		m_pPage->SetLineJoin(PdfWriter::linejoin_Round);
}
void CPdfWriter::UpdateBrush(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory)
{
	m_pShading = NULL;
	m_pShadingExtGrState = NULL;

	LONG lBrushType = m_oBrush.GetType();
	if (c_BrushTypeTexture == lBrushType)
	{
		std::wstring wsTexturePath = m_oBrush.GetTexturePath();
		CImageFileFormatChecker oImageFormat(wsTexturePath);

		PdfWriter::CImageDict* pImage = NULL;
		int nImageW = 0;
		int nImageH = 0;
		if (_CXIMAGE_FORMAT_JPG == oImageFormat.eFileType || _CXIMAGE_FORMAT_JP2 == oImageFormat.eFileType)
		{
			pImage = m_pDocument->CreateImage();
			CBgraFrame oFrame;
			oFrame.OpenFile(wsTexturePath);
			nImageH = oFrame.get_Height();
			nImageW = oFrame.get_Width();

			if (pImage)
			{
				if (_CXIMAGE_FORMAT_JPG == oImageFormat.eFileType)
					pImage->LoadJpeg(wsTexturePath.c_str(), nImageW, nImageH, oFrame.IsGrayScale());
				else
					pImage->LoadJpx(wsTexturePath.c_str(), nImageW, nImageH);
			}
		}
		else if (_CXIMAGE_FORMAT_WMF == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_EMF == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_SVM == oImageFormat.eFileType ||
				 _CXIMAGE_FORMAT_SVG == oImageFormat.eFileType)
		{
			// TODO: Реализовать отрисовку метафайлов по-нормальному
			MetaFile::IMetaFile* pMeta = MetaFile::Create(pAppFonts);
			pMeta->LoadFromFile(wsTexturePath.c_str());

			double dL, dR, dT, dB;
			m_oPath.GetBounds(dL, dT, dR, dB);

			double dW = 300.0 * (dR - dL) / 72;
			if (dW < 0) dW = -dW;
			double dH = 300.0 * (dB - dT) / 72;
			if (dH < 0) dH = -dH;

			if (dW < 1) dW = 1;
			if (dH < 1) dH = 1;

			double dMax = 2000;
			double dMin = 10;
			if (dW > dMax || dH > dMax)
			{
				double dMaxSrc = (dW > dH) ? dW : dH;
				dW *= (dMax / dMaxSrc);
				dH *= (dMax / dMaxSrc);
			}

			if (dW < dMin) dW = dMin;
			if (dH < dMin) dH = dMin;

			std::wstring wsTempFile = GetTempFile(wsTempDirectory);
			pMeta->ConvertToRaster(wsTempFile.c_str(), _CXIMAGE_FORMAT_PNG, (int)dW, (int)dH);

			RELEASEOBJECT(pMeta);

			Aggplus::CImage oImage(wsTempFile);
			nImageW = abs((int)oImage.GetWidth());
			nImageH = abs((int)oImage.GetHeight());
			pImage = LoadImage(&oImage, 255);
		}
		else
		{
			Aggplus::CImage oImage(wsTexturePath);
			nImageW = abs((int)oImage.GetWidth());
			nImageH = abs((int)oImage.GetHeight());
			pImage = LoadImage(&oImage, 255);
		}

		if (pImage)
		{
			BYTE nAlpha = m_oBrush.GetTextureAlpha();
			if (0xFF != nAlpha)
				pImage->AddTransparency(nAlpha);

			LONG lTextureMode = m_oBrush.GetTextureMode();

			double dW = 10;
			double dH = 10;

			double dL, dR, dT, dB;
			CBrushState::TBrushRect& oRect = m_oBrush.GetBrushRect();
			if (!oRect.bUse)
			{
				m_oPath.GetBounds(dL, dT, dR, dB);
			}
			else
			{
				dL = MM_2_PT(oRect.dLeft);
				dB = MM_2_PT(m_dPageHeight - oRect.dTop);
				dR = MM_2_PT(oRect.dLeft + oRect.dWidth);
				dT = MM_2_PT(m_dPageHeight - oRect.dTop - oRect.dHeight);
			}

			double dXStepSpacing = 0, dYStepSpacing = 0;
			if (c_BrushTextureModeStretch == lTextureMode)
			{
				// Растягиваем картинку по размерам пата
				dW = std::max(10.0, dR - dL);
				dH = std::max(10.0, dB - dT);

				// Чтобы избавиться от погрешностей из-за которых могут возникать полоски или обрезание картинки,
				// удвоим расстрояние между соседними тайлами. Плохого тут нет, т.к. нам нужен всего 1 тайл
				dXStepSpacing = dW;
				dYStepSpacing = dH;
			}
			else
			{
				// Размеры картинки заданы в пикселях. Размеры тайла - это размеры картинки в пунктах.
				dW = (double)nImageW * 72.0 / 96.0;
				dH = (double)nImageH * 72.0 / 96.0;
			}

			// Нам нужно, чтобы левый нижний угол границ нашего пата являлся точкой переноса для матрицы преобразования.
			PdfWriter::CMatrix* pMatrix = m_pPage->GetTransform();
			pMatrix->Apply(dL, dT);
			PdfWriter::CMatrix oPatternMatrix = *pMatrix;
			oPatternMatrix.x = dL;
			oPatternMatrix.y = dT;
			m_pPage->SetPatternColorSpace(m_pDocument->CreateImageTilePattern(dW, dH, pImage, &oPatternMatrix, PdfWriter::imagetilepatterntype_Default, dXStepSpacing, dYStepSpacing));
		}
	}
	else if (c_BrushTypeHatch1 == lBrushType)
	{
		std::wstring wsHatchType = m_oBrush.GetTexturePath();

		double dW = 8 * 72 / 96;
		double dH = 8 * 72 / 96;

		TColor oColor1 = m_oBrush.GetTColor1();
		TColor oColor2 = m_oBrush.GetTColor2();
		BYTE nAlpha1 = (BYTE)m_oBrush.GetAlpha1();
		BYTE nAlpha2 = (BYTE)m_oBrush.GetAlpha2();

		m_pPage->SetPatternColorSpace(m_pDocument->CreateHatchPattern(dW, dH, oColor1.r, oColor1.g, oColor1.b, nAlpha1, oColor2.r, oColor2.g, oColor2.b, nAlpha2, wsHatchType));
	}
	else if (c_BrushTypeRadialGradient == lBrushType || c_BrushTypeLinearGradient == lBrushType)
	{
		TColor* pGradientColors;
		double* pPoints;
		LONG lCount;

		m_oBrush.GetGradientColors(pGradientColors, pPoints, lCount);

		if (lCount > 0)
		{
			unsigned char* pColors = new unsigned char[3 * lCount];
			unsigned char* pAlphas = new unsigned char[lCount];
			if (pColors)
			{
				for (LONG lIndex = 0; lIndex < lCount; lIndex++)
				{
					pColors[3 * lIndex + 0] = pGradientColors[lIndex].r;
					pColors[3 * lIndex + 1] = pGradientColors[lIndex].g;
					pColors[3 * lIndex + 2] = pGradientColors[lIndex].b;
					pAlphas[lIndex]         = pGradientColors[lIndex].a;
				}

				if (c_BrushTypeLinearGradient == lBrushType)
				{
					double dX0, dY0, dX1, dY1;
					m_oBrush.GetLinearGradientPattern(dX0, dY0, dX1, dY1);
					m_pShading = m_pDocument->CreateAxialShading(m_pPage, MM_2_PT(dX0), MM_2_PT(m_dPageHeight - dY0), MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), pColors, pAlphas, pPoints, lCount, m_pShadingExtGrState);
				}
				else //if (c_BrushTypeRadialGradient == lBrushType)
				{
					double dX0, dY0, dR0, dX1, dY1, dR1;
					m_oBrush.GetRadialGradientPattern(dX0, dY0, dR0, dX1, dY1, dR1);
					m_pShading = m_pDocument->CreateRadialShading(m_pPage, MM_2_PT(dX0), MM_2_PT(m_dPageHeight - dY0), MM_2_PT(dR0), MM_2_PT(dX1), MM_2_PT(m_dPageHeight - dY1), MM_2_PT(dR1), pColors, pAlphas, pPoints, lCount, m_pShadingExtGrState);
				}
				delete[] pColors;
				delete[] pAlphas;
			}
		}
	}
	else// if (c_BrushTypeSolid == lBrushType)
	{
		TColor oColor1 = m_oBrush.GetTColor1();
		m_pPage->SetFillColor(oColor1.r, oColor1.g, oColor1.b);
		m_pPage->SetFillAlpha((unsigned char)m_oBrush.GetAlpha1());
	}
}
void CPdfWriter::Reset()
{
	m_oPen.Reset();
	m_oBrush.Reset();
	m_oFont.Reset();
	m_oPath.Clear();

	// clear font!!!
	m_oFont.SetName(L"");
	m_oFont.SetSize(-1);
	m_oFont.SetStyle(1 << 5);

	m_lClipDepth = 0;
}

void CPdfWriter::AddLink(PdfWriter::CPage* pPage, const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const unsigned int& unDestPage)
{
	PdfWriter::CPage* pDestPage = m_pDocument->GetPage(unDestPage);
	if (!pPage || !pDestPage)
		return;

	PdfWriter::CDestination* pDestination = m_pDocument->CreateDestination(pDestPage);
	if (!pDestination)
		return;

	pDestination->SetXYZ(MM_2_PT(dDestX), pDestPage->GetHeight() - MM_2_PT(dDestY), 0);
	PdfWriter::CAnnotation* pAnnot = m_pDocument->CreateLinkAnnot(PdfWriter::TRect(MM_2_PT(dX), pPage->GetHeight() - MM_2_PT(dY), MM_2_PT(dX + dW), pPage->GetHeight() - MM_2_PT(dY + dH)), pDestination);
	if (pAnnot && pPage)
		pPage->AddAnnotation(pAnnot);
	pAnnot->SetBorder(0, 0, {});
}
bool CPdfWriter::IsValid()
{
	return m_bValid;
}
bool CPdfWriter::IsPageValid()
{
	if (!IsValid() || !m_pPage)
		return false;

	return true;
}
void CPdfWriter::SetError()
{
	m_bValid = false;
}
unsigned char* CPdfWriter::EncodeString(const unsigned int *pUnicodes, const unsigned int& unCount, const unsigned int *pGIDs)
{
	if (m_bNeedUpdateTextFont)
	{
		if (!UpdateFont())
			return NULL;
	}

	if (!m_pFont)
		return NULL;

	unsigned char* pCodes = new unsigned char[unCount * 2];
	if (!pCodes)
		return NULL;

	for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
	{
		unsigned short ushCode;
		if (pGIDs)
			ushCode = m_pFont->EncodeGID(pGIDs[unIndex], &pUnicodes[unIndex], 1);
		else
			ushCode = m_pFont->EncodeUnicode(pUnicodes[unIndex]);

		pCodes[2 * unIndex + 0] = (ushCode >> 8) & 0xFF;
		pCodes[2 * unIndex + 1] = ushCode & 0xFF;
	}

	return pCodes;
}
unsigned char* CPdfWriter::EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unUnicodesCount)
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
std::wstring CPdfWriter::GetDownloadFile(const std::wstring& sUrl, const std::wstring& wsTempDirectory)
{
	std::wstring::size_type n1 = sUrl.find(L"www.");
	std::wstring::size_type n2 = sUrl.find(L"http://");
	std::wstring::size_type n3 = sUrl.find(L"ftp://");
	std::wstring::size_type n4 = sUrl.find(L"https://");
	std::wstring::size_type nMax = 3;

	bool bIsNeedDownload = false;
	if (n1 != std::wstring::npos && n1 < nMax)
		bIsNeedDownload = true;
	else if (n2 != std::wstring::npos && n2 < nMax)
		bIsNeedDownload = true;
	else if (n3 != std::wstring::npos && n3 < nMax)
		bIsNeedDownload = true;
	else if (n4 != std::wstring::npos && n4 < nMax)
		bIsNeedDownload = true;

	if (!bIsNeedDownload)
		return L"";

	std::wstring sTempFile = GetTempFile(wsTempDirectory);
	NSNetwork::NSFileTransport::CFileDownloader oDownloader(sUrl, false);
	oDownloader.SetFilePath(sTempFile);

	if (oDownloader.DownloadSync())
		return sTempFile;

	if (NSFile::CFileBinary::Exists(sTempFile))
		NSFile::CFileBinary::Remove(sTempFile);

	return L"";
}
void CPdfWriter::DrawTextWidget(NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CTextWidget* pTextWidget, const std::wstring& wsValue)
{
	if (!pAppFonts || !pTextWidget)
		return;
	PdfWriter::CFontCidTrueType* pFont = pTextWidget->GetFont();
	if (!pFont)
		return;
	PdfWriter::CFontTrueType* pFontTT = m_pDocument->CreateTrueTypeFont(pFont);
	double dFontSize = pTextWidget->GetFontSize();
	bool isBold      = pTextWidget->GetFontIsBold();
	bool isItalic    = pTextWidget->GetFontIsItalic();
	bool isComb      = pTextWidget->IsCombFlag();
	double dWidth    = pTextWidget->GetWidth();
	double dHeight   = pTextWidget->GetHeight();
	BYTE nAlign      = pTextWidget->GetQ();

	if (!pTextWidget->HaveBorder() && pTextWidget->HaveBC())
		pTextWidget->SetBorder(0, 1, {});
	double dShiftBorder = pTextWidget->GetBorderWidth();
	BYTE nType = pTextWidget->GetBorderType();
	if (nType == 1 || nType == 3)
		dShiftBorder *= 2;

	// Коды, шрифты, количество
	unsigned int unLen = 0;
	unsigned int* pUnicodes = NULL;
	unsigned short* pCodes  = NULL;
	PdfWriter::CFontCidTrueType** ppFonts = NULL;
	bool bFont = GetFontData(pAppFonts, wsValue, pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
	if (!bFont)
	{
		pTextWidget->SetEmptyAP();
		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);
		return;
	}

	if (!isComb && pTextWidget->IsMultiLine() && pFontTT)
	{
		unsigned short* pCodes2 = new unsigned short[unLen];
		unsigned int* pWidths   = new unsigned int[unLen];

		unsigned short ushSpaceCode   = 0xFFFF;
		unsigned short ushNewLineCode = 0xFFFE;
		for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
		{
			unsigned short ushCode = 0;
			if (0x0020 == pUnicodes[unIndex])
				ushCode = ushSpaceCode;
			else if (0x000D == pUnicodes[unIndex] || 0x000A == pUnicodes[unIndex])
				ushCode = ushNewLineCode;

			pCodes2[unIndex] = ushCode;
			pWidths[unIndex] = ppFonts[unIndex]->GetWidth(pCodes[unIndex]);
		}

		m_oLinesManager.Init(pCodes2, pWidths, unLen, ushSpaceCode, ushNewLineCode, pFontTT->GetLineHeight(), pFontTT->GetAscent());

		double dKoef = dFontSize / pFontTT->m_dUnitsPerEm;
		double dLineHeight = (pFontTT->m_dAscent + std::abs(pFontTT->m_dDescent)) * dKoef;

		m_oLinesManager.CalculateLines(dFontSize, dWidth);

		pTextWidget->StartAP();

		unsigned int unLinesCount = m_oLinesManager.GetLinesCount();
		double dLineShiftY = dHeight - dShiftBorder * 2 - dLineHeight;
		for (unsigned int unIndex = 0; unIndex < unLinesCount; ++unIndex)
		{
			unsigned int unLineStart = m_oLinesManager.GetLineStartPos(unIndex);
			double dLineShiftX = dShiftBorder * 2;
			double dLineWidth = m_oLinesManager.GetLineWidth(unIndex, dFontSize);
			if (2 == nAlign)
				dLineShiftX = dWidth - dLineWidth - dShiftBorder * 2;
			else if (1 == nAlign)
				dLineShiftX = (dWidth - dLineWidth) / 2;

			int nInLineCount = m_oLinesManager.GetLineEndPos(unIndex) - m_oLinesManager.GetLineStartPos(unIndex);
			if (nInLineCount > 0)
				pTextWidget->AddLineToAP(dLineShiftX, dLineShiftY, pCodes + unLineStart, nInLineCount, ppFonts + unLineStart, NULL);

			dLineShiftY -= dLineHeight;
		}

		pTextWidget->EndAP();

		m_oLinesManager.Clear();

		RELEASEARRAYOBJECTS(pCodes2);
		RELEASEARRAYOBJECTS(pWidths);
	}
	else
	{
		double* pShifts = NULL;
		unsigned int unShiftsCount = 0;
		double dShiftX = dShiftBorder * 2;
		if (dShiftX == 0)
			dShiftX = 2;

		if (isComb)
		{
			unShiftsCount = unLen;
			pShifts = new double[unShiftsCount];
			if (pShifts && unShiftsCount)
			{
				dShiftX = 0;
				unsigned int unCellsCount = std::max(unShiftsCount, pTextWidget->GetMaxLen());
				double dPrevW = 0;
				double dCellW = dWidth / unCellsCount;

				if (1 == nAlign)
				{
					unsigned int unCells = (unCellsCount - unShiftsCount) / 2;
					dPrevW = unCells * dCellW;
				}
				if (2 == nAlign)
					dPrevW = (unCellsCount - unShiftsCount) * dCellW;

				for (unsigned int unIndex = 0; unIndex < unShiftsCount; ++unIndex)
				{
					unsigned short ushCode = pCodes[unIndex];
					double dGlyphWidth = ppFonts[unIndex]->GetGlyphWidth(ushCode) / 1000.0 * dFontSize;
					double dTempShift = (dCellW - dGlyphWidth) / 2;
					pShifts[unIndex] = dPrevW + dTempShift;
					dPrevW = dCellW - dTempShift;
				}
			}
		}
		else if (1 == nAlign || 2 == nAlign)
		{
			double dLineWidth = 0;
			for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
			{
				unsigned short ushCode = pCodes[unIndex];
				double dLetterWidth    = ppFonts[unIndex]->GetWidth(ushCode) / 1000.0 * dFontSize;
				dLineWidth += dLetterWidth;
			}

			if (2 == nAlign)
				dShiftX = dWidth - dLineWidth - dShiftBorder * 2;
			else if (1 == nAlign)
				dShiftX = (dWidth - dLineWidth) / 2;
		}

		double dBaseLine = (dHeight - dFontSize) / 2.0 - dShiftBorder;
		if (pFontTT)
		{
			double dKoef = dFontSize / pFontTT->m_dUnitsPerEm;
			double dLineHeight = (pFontTT->m_dAscent + std::abs(pFontTT->m_dDescent)) * dKoef;;
			dBaseLine = (dHeight - dLineHeight) / 2.0 + std::abs(pFontTT->m_dDescent * dKoef);
		}

		pTextWidget->SetAP(wsValue, pCodes, unLen, dShiftX, dBaseLine, ppFonts, pShifts);
		RELEASEARRAYOBJECTS(pShifts);
	}

	RELEASEARRAYOBJECTS(pUnicodes);
	RELEASEARRAYOBJECTS(pCodes);
	RELEASEARRAYOBJECTS(ppFonts);
}
void CPdfWriter::DrawChoiceWidget(NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CChoiceWidget* pChoiceWidget, const std::vector<std::wstring>& arrValue)
{
	if (!pAppFonts || !pChoiceWidget)
		return;
	PdfWriter::CFontCidTrueType* pFont = pChoiceWidget->GetFont();
	if (!pFont)
		return;
	PdfWriter::CFontTrueType* pFontTT = m_pDocument->CreateTrueTypeFont(pFont);
	if (!pFontTT)
		return;
	double dFontSize = pChoiceWidget->GetFontSize();
	bool isBold      = pChoiceWidget->GetFontIsBold();
	bool isItalic    = pChoiceWidget->GetFontIsItalic();
	double dWidth    = pChoiceWidget->GetWidth();
	double dHeight   = pChoiceWidget->GetHeight();
	BYTE nAlign      = pChoiceWidget->GetQ();

	if (!pChoiceWidget->HaveBorder() && pChoiceWidget->HaveBC())
		pChoiceWidget->SetBorder(0, 1, {});
	double dShiftBorder = pChoiceWidget->GetBorderWidth();
	BYTE nType = pChoiceWidget->GetBorderType();
	if (nType == 1 || nType == 3)
		dShiftBorder *= 2;

	if (arrValue.empty())
	{
		pChoiceWidget->SetEmptyAP();
		return;
	}

	if (pChoiceWidget->GetWidgetType() == PdfWriter::WidgetCombobox)
	{
		std::wstring wsValue = pChoiceWidget->GetValue(arrValue.back());
		unsigned int unLen = 0;
		unsigned int* pUnicodes = NULL;
		unsigned short* pCodes  = NULL;
		PdfWriter::CFontCidTrueType** ppFonts = NULL;
		bool bFont = GetFontData(pAppFonts, wsValue, pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return;
		}

		double dShiftX = dShiftBorder * 2;
		if (dShiftX == 0)
			dShiftX = 2;
		if (1 == nAlign || 2 == nAlign)
		{
			double dSumWidth = 0;
			for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
			{
				unsigned short ushCode = pCodes[unIndex];
				double dLetterWidth    = ppFonts[unIndex]->GetWidth(ushCode) / 1000.0 * dFontSize;
				dSumWidth += dLetterWidth;
			}

			if (2 == nAlign)
				dShiftX = dWidth - dSumWidth - dShiftBorder * 2;
			else if (1 == nAlign)
				dShiftX = (dWidth - dSumWidth) / 2;
		}

		double dBaseLine = (dHeight - dFontSize) / 2.0 - dShiftBorder;
		if (pFontTT)
			dBaseLine = (dHeight - pFontTT->m_dHeight * dFontSize / pFontTT->m_dUnitsPerEm) / 2.0 + std::abs(pFontTT->m_dDescent * dFontSize / pFontTT->m_dUnitsPerEm);

		pChoiceWidget->SetAP(wsValue, pCodes, unLen, dShiftX, dBaseLine, ppFonts, NULL);

		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);
	}
	else // ListBox
	{
		std::wstring wsValue = pChoiceWidget->SetListBoxIndex(arrValue);
		unsigned int unLen = 0;
		unsigned int* pUnicodes = NULL;
		unsigned short* pCodes  = NULL;
		PdfWriter::CFontCidTrueType** ppFonts = NULL;
		bool bFont = GetFontData(pAppFonts, wsValue, pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return;
		}

		unsigned short* pCodes2 = new unsigned short[unLen];
		unsigned int* pWidths   = new unsigned int[unLen];

		unsigned short ushSpaceCode   = 0xFFFF;
		unsigned short ushNewLineCode = 0xFFFE;
		for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
		{
			unsigned short ushCode = 0;
			if (0x0020 == pUnicodes[unIndex])
				ushCode = ushSpaceCode;
			else if (0x000D == pUnicodes[unIndex] || 0x000A == pUnicodes[unIndex])
				ushCode = ushNewLineCode;

			pCodes2[unIndex] = ushCode;
			pWidths[unIndex] = ppFonts[unIndex]->GetWidth(pCodes[unIndex]);
		}

		m_oLinesManager.Init(pCodes2, pWidths, unLen, ushSpaceCode, ushNewLineCode, pFontTT->GetLineHeight(), pFontTT->GetAscent());

		m_oLinesManager.CalculateLines(dFontSize, dWidth);

		double dKoef = dFontSize / pFontTT->m_dUnitsPerEm;
		double dLineHeight = pFontTT->m_dHeight * dKoef;
		pChoiceWidget->SetListBoxHeight(dLineHeight);
		pChoiceWidget->StartAP();

		// double dKoef = dFontSize / pFontTT->m_dUnitsPerEm;
		// double dDescent = std::abs(pFontTT->m_dDescent * dFontSize / pFontTT->m_dUnitsPerEm);
		// double dAscent = dLineHeight - dDescent;
		// double dMidPoint = dAscent - pFontTT->m_dMinY * dKoef + dAscent - pFontTT->m_dMaxY * dKoef;
		// double dDiff = dLineHeight - dMidPoint;
		double dLineShiftY = dHeight - dShiftBorder - dLineHeight + std::abs(pFontTT->m_dDescent * dKoef);

		unsigned int unLinesCount = m_oLinesManager.GetLinesCount();
		for (unsigned int unIndex = 0; unIndex < unLinesCount; ++unIndex)
		{
			unsigned int unLineStart = m_oLinesManager.GetLineStartPos(unIndex);
			double dLineShiftX = dShiftBorder * 2;
			if (dLineShiftX == 0)
				dLineShiftX = 2;
			double dLineWidth = m_oLinesManager.GetLineWidth(unIndex, dFontSize);
			if (2 == nAlign)
				dLineShiftX = dWidth - dLineWidth - dShiftBorder * 2;
			else if (1 == nAlign)
				dLineShiftX = (dWidth - dLineWidth) / 2;

			int nInLineCount = m_oLinesManager.GetLineEndPos(unIndex) - m_oLinesManager.GetLineStartPos(unIndex);
			if (nInLineCount > 0)
				pChoiceWidget->AddLineToAP(dLineShiftX, dLineShiftY, pCodes + unLineStart, nInLineCount, ppFonts + unLineStart, NULL);

			dLineShiftY -= dLineHeight;
			if (dLineShiftY < 0)
				break;
		}

		pChoiceWidget->EndAP();

		m_oLinesManager.Clear();

		RELEASEARRAYOBJECTS(pCodes2);
		RELEASEARRAYOBJECTS(pWidths);

		RELEASEARRAYOBJECTS(pUnicodes);
		RELEASEARRAYOBJECTS(pCodes);
		RELEASEARRAYOBJECTS(ppFonts);
	}
}
void CPdfWriter::DrawButtonWidget(NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CPushButtonWidget* pButtonWidget, BYTE nAP, PdfWriter::CXObject* pForm)
{
	if (!pAppFonts || !pButtonWidget)
		return;

	double dShiftX = 0;
	double dShiftY = 0;
	double dLineW = 0;
	double dLineH = 0;
	unsigned int unLen = 0;
	unsigned int* pUnicodes = NULL;
	unsigned short* pCodes  = NULL;
	PdfWriter::CFontCidTrueType** ppFonts = NULL;
	BYTE nTP = pButtonWidget->GetTP();
	std::wstring wsValue;
	if (nAP == 0)
		wsValue = pButtonWidget->GetCA();
	else if (nAP == 1)
		wsValue = pButtonWidget->GetRC().empty() ? pButtonWidget->GetCA() : pButtonWidget->GetRC();
	else
		wsValue = pButtonWidget->GetAC().empty() ? pButtonWidget->GetCA() : pButtonWidget->GetAC();

	if (!pButtonWidget->HaveBorder() && pButtonWidget->HaveBC())
		pButtonWidget->SetBorder(0, 1, {});

	if (!wsValue.empty() && nTP != 1)
	{
		PdfWriter::CFontCidTrueType* pFont = pButtonWidget->GetFont();
		if (!pFont)
			return;
		PdfWriter::CFontTrueType* pFontTT = m_pDocument->CreateTrueTypeFont(pFont);
		double dFontSize = pButtonWidget->GetFontSize();
		bool isBold      = pButtonWidget->GetFontIsBold();
		bool isItalic    = pButtonWidget->GetFontIsItalic();
		double dWidth    = pButtonWidget->GetWidth();
		double dHeight   = pButtonWidget->GetHeight();

		double dShiftBorder = pButtonWidget->GetBorderWidth();
		BYTE nType = pButtonWidget->GetBorderType();
		if (nType == 1 || nType == 3)
			dShiftBorder *= 2;
		if (dShiftBorder == 0)
			dShiftBorder = 1;
		double dShiftRespectBorder = dShiftBorder / 2;
		bool bRespectBorder = pButtonWidget->GetRespectBorder();
		if (!bRespectBorder)
			dShiftBorder = 0;

		bool bFont = GetFontData(pAppFonts, wsValue, pFont, isBold, isItalic, pUnicodes, unLen, pCodes, ppFonts);
		if (!bFont)
		{
			RELEASEARRAYOBJECTS(pUnicodes);
			RELEASEARRAYOBJECTS(pCodes);
			RELEASEARRAYOBJECTS(ppFonts);
			return;
		}

		for (unsigned int unIndex = 0; unIndex < unLen; ++unIndex)
		{
			unsigned short ushCode = pCodes[unIndex];
			double dLetterWidth    = ppFonts[unIndex]->GetWidth(ushCode) / 1000.0 * dFontSize;
			dLineW += dLetterWidth;
		}

		if (pFontTT)
		{
			double dKoef = dFontSize / pFontTT->m_dUnitsPerEm;
			// TODO что-то между m_dMaxY-m_dMinY и m_dHeight, но не просто среднее
			dLineH = (pFontTT->m_dMaxY + std::abs(pFontTT->m_dMinY) + pFontTT->m_dHeight) / 2.0 * dKoef;
			// dLineH = (pFontTT->m_dMaxY + std::abs(pFontTT->m_dMinY)) * dKoef;
			// dLineH = pFontTT->m_dHeight * dKoef;
			// double dLineHeight = pFontTT->m_dHeight * dKoef;
			// double dDescent = std::abs(pFontTT->m_dDescent * dKoef);
			// double dAscent1 = pFontTT->m_dAscent * dKoef;
			// double dAscent = dLineHeight - dDescent;
			// double dMidPoint = dAscent - pFontTT->m_dMinY * dKoef + dAscent - pFontTT->m_dMaxY * dKoef;
			// double dDiff = dLineHeight - dMidPoint;

			if (nTP == 0 || nTP == 2 || nTP == 3 || nTP == 6)
				dShiftX = (dWidth - dLineW) / 2;
			else if (nTP == 4)
				dShiftX = dWidth - dLineW - dShiftBorder * 2 + dShiftRespectBorder;
			else if (nTP == 5)
				dShiftX = dShiftBorder * 2 + dShiftRespectBorder;

			if (nTP == 0 || nTP == 6 || nTP == 4 || nTP == 5)
				dShiftY = (dHeight - dLineH) / 2 + std::abs(pFontTT->m_dMinY * dKoef);
			else if (nTP == 3)
				dShiftY = dHeight - dShiftBorder * 2 - dShiftRespectBorder - dLineH + std::abs(pFontTT->m_dMinY * dKoef);
			else if (nTP == 2)
				dShiftY = dShiftBorder * 2 + std::abs(pFontTT->m_dMinY * dKoef);
		}
	}

	pButtonWidget->SetAP(pForm, nAP, pCodes, unLen, dShiftX, dShiftY, dLineW, dLineH, ppFonts);

	RELEASEARRAYOBJECTS(pUnicodes);
	RELEASEARRAYOBJECTS(pCodes);
	RELEASEARRAYOBJECTS(ppFonts);
}
