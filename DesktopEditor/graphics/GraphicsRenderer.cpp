/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "GraphicsRenderer.h"
#include <algorithm>
#include "../raster/Metafile/MetaFile.h"

#if 0
static void LOGGING(char* buffer, ...)
{
    FILE* f = fopen("path_to_log.txt", "a+");

    va_list args;
    va_start(args, buffer);
    vfprintf(f, buffer, args);
    fprintf(f, "\n");

    fclose(f);
    va_end(args);
}
#endif

////////////////////////////////////////////////////////////////////////////////

Aggplus::CBrush* CGraphicsRenderer::CreateBrush(NSStructures::CBrush* pBrush)
{
	if (NULL == pBrush)
		return NULL;

    bool bIsSwappedRGB = false;
    if (m_pRenderer && m_pRenderer->m_bSwapRGB)
        bIsSwappedRGB = true;

	LONG Type = pBrush->Type;
	if ((0 == Type) || (c_BrushTypeSolid == Type))
	{
        Aggplus::CColor oColor((BYTE)(pBrush->Alpha1 * m_dGlobalAlpha), pBrush->Color1, bIsSwappedRGB);
		Aggplus::CBrushSolid* pNew = new Aggplus::CBrushSolid(oColor);

		return pNew;
	}
	else if	   ((c_BrushTypeHorizontal		== Type) ||
				(c_BrushTypeVertical		== Type) ||
				(c_BrushTypeDiagonal1		== Type) ||
				(c_BrushTypeDiagonal2		== Type) ||
				(c_BrushTypeCenter			== Type) ||
				(c_BrushTypePathGradient1	== Type) ||
				(c_BrushTypePathGradient2	== Type) ||
				(c_BrushTypeCylinderHor		== Type) ||
				(c_BrushTypeCylinderVer		== Type))
	{
        Aggplus::CColor o1((BYTE)(pBrush->Alpha1 * m_dGlobalAlpha), pBrush->Color1, bIsSwappedRGB);
        Aggplus::CColor o2((BYTE)(pBrush->Alpha2 * m_dGlobalAlpha), pBrush->Color2, bIsSwappedRGB);

		Aggplus::CBrushLinearGradient* pNew = new Aggplus::CBrushLinearGradient( Aggplus::RectF(0.0f, 0.0f, 1.0f, 1.0f), o1, o2, (float)pBrush->LinearAngle, TRUE );
		if( pNew )
		{
			pNew->SetRelativeCoords( TRUE );

            int nCountSubColors = pBrush->m_arrSubColors.size();
			if( nCountSubColors > 0 )
			{
				Aggplus::CColor* pColors = new Aggplus::CColor[nCountSubColors];
				float* pBlends = new float[nCountSubColors];

				if( pColors && pBlends )
				{
					for( int i = 0; i < nCountSubColors; i++ )
					{
						DWORD dwColor = (DWORD)pBrush->m_arrSubColors[i].color;
						BYTE a = (dwColor >> 24) & 0xFF;
						BYTE b = (dwColor >> 16) & 0xFF;
						BYTE g = (dwColor >> 8) & 0xFF;
						BYTE r = (dwColor) & 0xFF;

                        if (bIsSwappedRGB)
                        {
                            BYTE tmp = r;
                            r = b;
                            b = tmp;
                        }

						pColors[i] = Aggplus::CColor((BYTE)(a * m_dGlobalAlpha), b, g, r);
						pBlends[i] = (float)(pBrush->m_arrSubColors[i].position / 65536.0);
					}

					pNew->SetInterpolationColors( pColors, pBlends, nCountSubColors );
				}

				delete [] pColors;
				delete [] pBlends;
			}

			pNew->SetBounds(pBrush->Bounds);
		}

		if (pNew && c_BrushTypePathGradient2 == Type)
			pNew->m_bType = Aggplus::BrushTypePathGradient;

		return pNew;
	}
	else if (c_BrushTypeHatch1 == Type)
	{
        Aggplus::CColor o1((BYTE)(pBrush->Alpha1 * m_dGlobalAlpha), pBrush->Color1, bIsSwappedRGB);
        Aggplus::CColor o2((BYTE)(pBrush->Alpha2 * m_dGlobalAlpha), pBrush->Color2, bIsSwappedRGB);

		Aggplus::CBrushHatch* pNew = new Aggplus::CBrushHatch();
		pNew->m_dwColor1	= o1;
		pNew->m_dwColor2	= o2;
		pNew->m_name		= pBrush->TexturePath;

		pNew->Bounds		= pBrush->Bounds;

		return pNew;
	}
	else
	{
		Aggplus::CBrushTexture* pNew = new Aggplus::CBrushTexture(pBrush->TexturePath, /*(Aggplus::WrapMode)TextureMode*/Aggplus::WrapModeClamp);
		return pNew;
	}
}

CGraphicsRenderer::CGraphicsRenderer() : NSGraphics::IGraphicsRenderer()
{
	m_pRenderer = NULL;
	m_pPath		= NULL;

	m_dWidth	= 210;
	m_dHeight	= 190;

	m_dPixelsWidth	= 720;
	m_dPixelsHeight = 576;

	m_lCurrentCommandType	= 0;

	m_lCurrentClipMode	= 0;
	m_bIsSetupClip		= FALSE;

	m_pFontManager			= NULL;

	m_oInstalledFont.Name	= L"";

	m_pPixels	= NULL;
	m_pDIB		= NULL;

	m_pCache	= NULL;

	m_dGlobalAlpha			= 1.0;
    m_bGlobalAlphaEnabled	= false;

    m_dGammaStroke = -1;
}
CGraphicsRenderer::~CGraphicsRenderer()
{
	Clear();

	RELEASEOBJECT(m_pDIB);
	RELEASEINTERFACE(m_pFontManager);
	RELEASEINTERFACE(m_pCache);
}

void CGraphicsRenderer::SetImageCache(NSImages::IImageFilesCache* pCache)
{
	RELEASEINTERFACE(m_pCache);
    m_pCache = (CImageFilesCache*)pCache;
	ADDREFINTERFACE(m_pCache);
}

void CGraphicsRenderer::SetFontManager(NSFonts::IFontManager* pManager)
{
	RELEASEINTERFACE(m_pFontManager);		
	if (NULL == pManager)
	{
		m_pFontManager = new CFontManager();
		m_pFontManager->Initialize();
	}
	else
	{
        m_pFontManager = (CFontManager*)pManager;
		ADDREFINTERFACE(m_pFontManager);
	}
}
void CGraphicsRenderer::CheckFontManager()
{
	if (NULL == m_pFontManager)
		SetFontManager(NULL);
}

NSFonts::IFontManager* CGraphicsRenderer::GetFontManager()
{
	return m_pFontManager;
}
BYTE* CGraphicsRenderer::GetPixels(LONG& lWidth, LONG& lHeight)
{
	lWidth  = (LONG)m_dPixelsWidth;
	lHeight = (LONG)m_dPixelsHeight;
	return m_pPixels;
}

void CGraphicsRenderer::ClearInstallFont()
{
	m_oInstalledFont.Name = L"";
	m_oInstalledFont.Path = L"";
}
void CGraphicsRenderer::SetClipRect(double x, double y, double w, double h)
{
	m_pRenderer->SetClipRect3(x, y, w, h);
}

INT CGraphicsRenderer::CheckValidate(INT bOnlyGraphics)
{
	if (NULL == m_pRenderer)
		return FALSE;

	if (!bOnlyGraphics)
		return (NULL != m_pPath);

	return TRUE;
}
void CGraphicsRenderer::Clear()
{
	RELEASEOBJECT(m_pPath);
	RELEASEOBJECT(m_pRenderer);
}

void CGraphicsRenderer::UpdateSize()
{
	if (NULL == m_pRenderer)
		return;

	m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
	m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);

	m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
}

// тип рендерера-----------------------------------------------------------------------------
HRESULT CGraphicsRenderer::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nGrRenderer;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT CGraphicsRenderer::NewPage()
{
	// ну не влезло так не влезло
	return S_OK;
}
HRESULT CGraphicsRenderer::put_Height(const double& dHeight)
{
	m_dHeight = dHeight;
	if (NULL != m_pRenderer)
	{
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
	}
	return S_OK;
}
HRESULT CGraphicsRenderer::get_Height(double* dHeight)
{
	if (NULL == dHeight)
		return S_FALSE;

	*dHeight = m_dHeight;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_Width(const double& dWidth)
{
	m_dWidth = dWidth;
	if (NULL != m_pRenderer)
	{
		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
	}
	return S_OK;
}
HRESULT CGraphicsRenderer::get_Width(double* dWidth)
{
	if (NULL == dWidth)
		return S_FALSE;

	*dWidth = m_dWidth;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_DpiX(double* dDpiX)
{
	if (NULL == dDpiX)
		return S_FALSE;

	*dDpiX = 25.4 * m_dPixelsWidth / m_dWidth;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_DpiY(double* dDpiY)
{
	if (NULL == dDpiY)
		return S_FALSE;

	*dDpiY = 25.4 * m_dPixelsHeight / m_dHeight;
	return S_OK;
}

// pen --------------------------------------------------------------------------------------
HRESULT CGraphicsRenderer::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenColor(const LONG& lColor)
{
	m_oPen.Color = lColor;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenSize(const double& dSize)
{
	m_oPen.Size = dSize;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenDashStyle(const BYTE& val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenLineStartCap(const BYTE& val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenLineEndCap(const BYTE& val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenLineJoin(const BYTE& val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenDashOffset(double* dOffset)
{
	*dOffset = m_oPen.DashOffset;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenDashOffset(const double& dOffset)
{
	m_oPen.DashOffset = dOffset;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenAlign(LONG* lAlign)
{
	*lAlign = m_oPen.Align;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenAlign(const LONG& lAlign)
{
	m_oPen.Align = lAlign;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_PenMiterLimit(double* dOffset)
{
	*dOffset = m_oPen.MiterLimit;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_PenMiterLimit(const double& dOffset)
{
	m_oPen.MiterLimit = dOffset;
	return S_OK;
}
HRESULT CGraphicsRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	RELEASEARRAYOBJECTS((m_oPen.DashPattern));
	m_oPen.DashPattern = new double[lCount];
	memcpy(m_oPen.DashPattern, pPattern, lCount * sizeof(double));
	m_oPen.Count = lCount;
	return S_OK;
}

// brush ------------------------------------------------------------------------------------
HRESULT CGraphicsRenderer::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushType(const LONG& lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushColor1(const LONG& lColor)
{
	m_oBrush.Color1 = lColor;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.Color2 = lColor;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushTexturePath(std::wstring* bsPath)
{
	*bsPath = m_oBrush.TexturePath;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushTexturePath(const std::wstring& bsPath)
{
	m_oBrush.TexturePath = bsPath;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
HRESULT CGraphicsRenderer::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width = (float)width;
	m_oBrush.Rect.Height = (float)height;
	return S_OK;
}
HRESULT CGraphicsRenderer::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Bounds.left	= left;
	m_oBrush.Bounds.top		= top;
	m_oBrush.Bounds.right	= left + width;
	m_oBrush.Bounds.bottom	= top + height;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{
    m_oBrush.m_arrSubColors.clear();
	for (LONG i = 0; i < nCount; ++i)
	{
		NSStructures::CBrush::TSubColor color;
		color.color		= lColors[i];
		color.position	= (long)(pPositions[i] * 65536);
        m_oBrush.m_arrSubColors.push_back(color);
	}
	return S_OK;
}

// font -------------------------------------------------------------------------------------
HRESULT CGraphicsRenderer::get_FontName(std::wstring* bsName)
{
	*bsName = m_oFont.Name;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontName(const std::wstring& bsName)
{
	m_oFont.Name = bsName;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontPath(std::wstring* bsName)
{
	*bsName = m_oFont.Path;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontPath(const std::wstring& bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontSize(const double& dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontStringGID(INT* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontStringGID(const INT& bGID)
{
	m_oFont.StringGID = bGID;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontCharSpace(const double& dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
HRESULT CGraphicsRenderer::get_FontFaceIndex(int* lFaceIndex)
{
	*lFaceIndex = m_oFont.FaceIndex;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
	m_oFont.FaceIndex = lFaceIndex;
	return S_OK;
}

//-------- Функции для вывода текста --------------------------------------------------------
HRESULT CGraphicsRenderer::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;
	put_BrushType(c_BrushTypeSolid);
		
	_SetFont();

	Aggplus::CBrush* pBrush = CreateBrush(&m_oBrush);				
    m_pRenderer->DrawStringC(c, m_pFontManager, pBrush, x, y);
	
	RELEASEOBJECT(pBrush);

	return S_OK;
}
HRESULT CGraphicsRenderer::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;
	put_BrushType(c_BrushTypeSolid);
		
	_SetFont();

	Aggplus::CBrush* pBrush = CreateBrush(&m_oBrush);				
    m_pRenderer->DrawString(bsText, m_pFontManager, pBrush, x, y);
	
	RELEASEOBJECT(pBrush);

	return S_OK;
}
	
HRESULT CGraphicsRenderer::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{
	if (gid >= 0)
	{
		m_oFont.StringGID = TRUE;
        return CommandDrawTextCHAR(gid, x, y, w, h);
	}
	
	m_oFont.StringGID = FALSE;
    return CommandDrawTextCHAR(c, x, y, w, h);
}

HRESULT CGraphicsRenderer::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
    if (NULL != pGids && 0 != nGidsCount && !(1 == nGidsCount && 0 == *pGids))
	{
        m_oFont.StringGID = TRUE;
        if (c_nHyperlinkType == m_lCurrentCommandType)
            return S_OK;

        put_BrushType(c_BrushTypeSolid);

        _SetFont();

        Aggplus::CBrush* pBrush = CreateBrush(&m_oBrush);
        m_pRenderer->DrawString(pGids, nGidsCount, m_pFontManager, pBrush, x, y);

        RELEASEOBJECT(pBrush);
        return S_OK;
	}
		
    m_oFont.StringGID = FALSE;
    return CommandDrawText(bsUnicodeText, x, y, w, h);
}

//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT CGraphicsRenderer::BeginCommand(const DWORD& lType)
{
	m_lCurrentCommandType = lType;
	
	switch (lType)
	{
	case c_nPageType:
		{
			return NewPage();
		}
	default:
		break;
	};
	
	return S_OK;
}

HRESULT CGraphicsRenderer::EndCommand(const DWORD& lType)
{
	switch (lType)
	{
	case c_nClipType:
		{
			INT bIsIn = (c_nClipRegionTypeWinding == (0x0001 & m_lCurrentClipMode));

			m_pPath->SetRuler(bIsIn ? false : true);

			INT bIsIntersect = (c_nClipRegionIntersect == (0x0100 & m_lCurrentClipMode));
			m_pRenderer->CombineClip(m_pPath, bIsIntersect ? agg::sbool_and : agg::sbool_or);

			//m_pRenderer->SetClip(m_pPath);
			break;
		}
	case c_nResetClipType:
		{
			m_pRenderer->ResetClip();
			m_bIsSetupClip = FALSE;
			break;
		}
	default:
		break;
	};

	m_lCurrentCommandType = 0;
	return S_OK;
}

//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT CGraphicsRenderer::PathCommandMoveTo(const double& x, const double& y)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->MoveTo(x, y);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandLineTo(const double& x, const double& y)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->LineTo(x, y);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandLinesTo(double* points, const int& count)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->AddLines(points, count);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->CurveTo(x1, y1, x2, y2, x3, y3);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandCurvesTo(double* points, const int& count)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->AddBeziers(points, count);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->AddArc2(x, y, w, h, -startAngle, -sweepAngle);
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandClose()
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->CloseFigure();
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandEnd()
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->Reset();
	return S_OK;
}
HRESULT CGraphicsRenderer::DrawPath(const LONG& nType)
{
	if (!CheckValidate(TRUE))
		return S_FALSE;

	LONG lFillType = (nType & 0xFF00);
	INT bIsStroke = (0x01 == (nType & 0x01));

	switch (lFillType)
	{
	case c_nWindingFillMode:
		{
			m_pPath->SetRuler(false);

			CCacheImage* pCacheImage	= NULL;
			Aggplus::CBrush* pBrush		= NULL;
			
			if (m_oBrush.Type == c_BrushTypeTexture || m_oBrush.Type == c_BrushTypePattern)
			{
				Aggplus::WrapMode oMode = Aggplus::WrapModeClamp;
				switch (m_oBrush.TextureMode)
				{
				case c_BrushTextureModeTile:
					oMode = Aggplus::WrapModeTile;
					break;
				case c_BrushTextureModeTileCenter:
					oMode = Aggplus::WrapModeTile;
					break;
				default: 
					break;
				}

				Aggplus::CBrushTexture* pTextureBrush = NULL;
				
				if (NULL != m_pCache)
				{
                    pCacheImage = (CCacheImage*)m_pCache->Lock(m_oBrush.TexturePath);

					pTextureBrush = new Aggplus::CBrushTexture(pCacheImage->GetImage(), oMode);
				}
				else
				{
					pTextureBrush = new Aggplus::CBrushTexture(m_oBrush.TexturePath, oMode);
				}

				if( pTextureBrush )
				{
					if( m_oBrush.Type == c_BrushTypePattern )
					{
						pTextureBrush->m_bUsePattern = TRUE;
						pTextureBrush->m_colors[0] = Aggplus::CColor((BYTE)m_oBrush.Alpha1, m_oBrush.Color1);
						pTextureBrush->m_colors[1] = Aggplus::CColor((BYTE)m_oBrush.Alpha2, m_oBrush.Color2);
					}
                    
					pTextureBrush->Alpha = (BYTE)m_oBrush.TextureAlpha;
				}

				pBrush = pTextureBrush;
			}
			else
			{
				pBrush = CreateBrush(&m_oBrush);
			}

			m_pRenderer->FillPath(pBrush, m_pPath);
			RELEASEOBJECT(pBrush);
			RELEASEINTERFACE(pCacheImage);
			break;
		}
	case c_nEvenOddFillMode:
		{
			m_pPath->SetRuler(true);

			CCacheImage* pCacheImage	= NULL;
			Aggplus::CBrush* pBrush		= NULL;
			
			if (m_oBrush.Type == c_BrushTypeTexture || m_oBrush.Type == c_BrushTypePattern)
			{
				Aggplus::WrapMode oMode = Aggplus::WrapModeClamp;
				switch (m_oBrush.TextureMode)
				{
				case c_BrushTextureModeTile:
					oMode = Aggplus::WrapModeTile;
					break;
				case c_BrushTextureModeTileCenter:
					oMode = Aggplus::WrapModeTile;
					break;
				default:
					break;
				}
				Aggplus::CBrushTexture* pTextureBrush = NULL;
				
				if (NULL != m_pCache)
				{
                    pCacheImage = (CCacheImage*)m_pCache->Lock(m_oBrush.TexturePath);

					pTextureBrush = new Aggplus::CBrushTexture(pCacheImage->GetImage(), oMode);
				}
				else
				{
					pTextureBrush = new Aggplus::CBrushTexture(m_oBrush.TexturePath, oMode);
				}

				if( pTextureBrush )
				{
					if( m_oBrush.Type == c_BrushTypePattern )
					{
						pTextureBrush->m_bUsePattern = TRUE;
						pTextureBrush->m_colors[0] = Aggplus::CColor((BYTE)m_oBrush.Alpha1, m_oBrush.Color1);
						pTextureBrush->m_colors[1] = Aggplus::CColor((BYTE)m_oBrush.Alpha2, m_oBrush.Color2);
					}
				}

				pBrush = pTextureBrush;
			}
			else
			{
				pBrush = CreateBrush(&m_oBrush);
			}

			m_pRenderer->FillPath(pBrush, m_pPath);
			RELEASEOBJECT(pBrush);
			RELEASEINTERFACE(pCacheImage);

			break;
		}
	default:
		break;
	};

	if (bIsStroke)
	{
        m_pRenderer->DrawPath(&m_oPen, m_pPath, m_dGammaStroke);
	}
	
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandStart()
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->StartFigure();
	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandGetCurrentPoint(double* x, double* y)
{
	if (!CheckValidate())
		return S_FALSE;

	m_pPath->GetLastPoint(*x, *y);
	return S_OK;
}

// textpath
HRESULT CGraphicsRenderer::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{
	if (!CheckValidate())
		return S_FALSE;

	_SetFont();

	// так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
	float fSize = (float)(0.3528 * m_oFont.Size);
    m_pPath->AddStringC(c, m_pFontManager, x, y);

	return S_OK;
}
HRESULT CGraphicsRenderer::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	if (!CheckValidate())
		return S_FALSE;

	_SetFont();

	// так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
	float fSize = (float)(0.3528 * m_oFont.Size);
    m_pPath->AddString(bsText, m_pFontManager, x, y);

	return S_OK;
}

HRESULT CGraphicsRenderer::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{
	if (gid >= 0)
	{
		m_oFont.StringGID = TRUE;
        return PathCommandTextCHAR(gid, x, y, w, h);
	}
	
	m_oFont.StringGID = FALSE;
    return PathCommandTextCHAR(c, x, y, w, h);
}
HRESULT CGraphicsRenderer::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
    if (NULL != pGids)
	{
		m_oFont.StringGID = TRUE;
        if (!CheckValidate())
            return S_FALSE;

        _SetFont();

        // так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
        float fSize = (float)(0.3528 * m_oFont.Size);
        m_pPath->AddString(pGids, nGidsCount, m_pFontManager, x, y);

        return S_OK;
	}
	
	m_oFont.StringGID = FALSE;
    return PathCommandText(bsUnicodeText, x, y, w, h);
}

//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT CGraphicsRenderer::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
{
	if (!CheckValidate(TRUE) || NULL == pImage)
		return S_FALSE;

	m_pRenderer->DrawImage((Aggplus::CImage*)pImage, x, y, w, h);
	return S_OK;	
}
HRESULT CGraphicsRenderer::DrawImageFromFile(const std::wstring& bstrVal, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
{
#if 0
    MetaFile::CMetaFile oMetafile(m_pFontManager ? m_pFontManager->m_pApplication : NULL);
    if (oMetafile.LoadFromFile(bstrVal.c_str()))
    {
        this->Save();
        bool bRet = oMetafile.DrawOnRenderer(this, x, y, w, h);
        this->Restore();
        return bRet ? S_OK : S_FALSE;
    }
#endif

	CCacheImage* pCacheImage = NULL;
    if (NULL != m_pCache)
	{
        pCacheImage = (CCacheImage*)m_pCache->Lock(bstrVal);
	}
    else
    {
        pCacheImage = new CCacheImage(NULL, bstrVal);
    }

	if (NULL != pCacheImage)
	{
		Aggplus::CImage* pImage = pCacheImage->GetImage();
		m_pRenderer->DrawImage(pImage, x, y, w, h);
		RELEASEINTERFACE(pCacheImage);
	}	

	return S_OK;
}

// transform --------------------------------------------------------------------------------
HRESULT CGraphicsRenderer::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
{
	if (!CheckValidate())
		return S_FALSE;

	ApplyTransform(m1, m2, m3, m4, m5, m6);	
	return S_OK;
}
HRESULT CGraphicsRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	if (!CheckValidate())
		return S_FALSE;

	double buffer[6];
	Aggplus::CMatrix oMatrix;
	m_pRenderer->GetTransform()->GetElements(buffer);

	*pdA = (double)buffer[0];
	*pdB = (double)buffer[1];
	*pdC = (double)buffer[2];
	*pdD = (double)buffer[3];
	*pdE = (double)buffer[4];
	*pdF = (double)buffer[5];

	return S_OK;
}
HRESULT CGraphicsRenderer::ResetTransform()
{
	_ResetTransform();
	return S_OK;
}

// -----------------------------------------------------------------------------------------
HRESULT CGraphicsRenderer::get_ClipMode(LONG* plMode)
{
	*plMode = m_lCurrentClipMode;
	return S_OK;
}
HRESULT CGraphicsRenderer::put_ClipMode(const LONG& lMode)
{
	if (!CheckValidate())
		return S_FALSE;

	m_lCurrentClipMode = lMode;
	return S_OK;
}

// additiaonal params ----------------------------------------------------------------------
HRESULT CGraphicsRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
	return S_OK;
}
HRESULT CGraphicsRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
	return S_OK;
}
HRESULT CGraphicsRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	return S_OK;
}

HRESULT CGraphicsRenderer::StartConvertCoordsToIdentity()
{
	m_bUseTransformCoordsToIdentity = true;
	m_pPath->m_internal->m_pTransform = m_pRenderer->GetFullTransform();
	return S_OK;
}
HRESULT CGraphicsRenderer::EndConvertCoordsToIdentity()
{
	m_bUseTransformCoordsToIdentity = false;
	m_pPath->m_internal->m_pTransform = NULL;
	return S_OK;
}

// common
void CGraphicsRenderer::_SetFont()
{
	if (m_oInstalledFont.IsEqual(&m_oFont))
	{
		if (1 < m_dWidth)
		{
			double dPix = m_oFont.CharSpace * m_dPixelsWidth / m_dWidth;
			m_pFontManager->SetCharSpacing(dPix);
		}
		return;
	}

	if (1 < m_dWidth)
	{
		double dPix = m_oFont.CharSpace * m_dPixelsWidth / m_dWidth;
		m_pFontManager->m_fCharSpacing = dPix;
	}

	if (m_oFont.Path.empty())
	{
		m_pFontManager->LoadFontByName(m_oFont.Name, m_oFont.Size, m_oFont.GetStyle(), m_pRenderer->GetDpiX(), m_pRenderer->GetDpiY());
	}
	else
	{
		m_pFontManager->LoadFontFromFile(m_oFont.Path, m_oFont.FaceIndex, m_oFont.Size, m_pRenderer->GetDpiX(), m_pRenderer->GetDpiY());
	}
	m_pFontManager->m_oString.ResetCTM();
	m_pFontManager->SetStringGID(m_oFont.StringGID);
	m_pFontManager->SetCharSpacing(m_pFontManager->m_fCharSpacing);

	m_oInstalledFont = m_oFont;
}

void CGraphicsRenderer::ApplyTransform(const double& d1, const double& d2, const double& d3, const double& d4, const double& d5, const double& d6)
{
	Aggplus::CMatrix oMatrix(d1, d2, d3, d4, d5, d6);
	ApplyTransform(&oMatrix);
}

void CGraphicsRenderer::ApplyTransform(Aggplus::CMatrix* pMatrix)
{
	if (!CheckValidate())
		return;

	m_pRenderer->SetTransform(pMatrix);
	m_pRenderer->CalculateFullTransform();
}

void CGraphicsRenderer::_ResetTransform()
{
	if (!CheckValidate())
		return;
		
	ApplyTransform(1, 0, 0, 1, 0, 0);
}

void CGraphicsRenderer::CreateFromBgraFrame(CBgraFrame* pFrame)
{		
	Aggplus::CDoubleRect oRect;
	oRect.left		= 0;
	oRect.top		= 0;
	oRect.right		= pFrame->get_Width();
	oRect.bottom	= pFrame->get_Height();

	if (pFrame->get_Stride() > 0)
	{
		CreateFlip(pFrame->get_Data(), oRect, pFrame->get_Width(), pFrame->get_Height());
	}
	else
	{
		Create(pFrame->get_Data(), oRect, pFrame->get_Width(), pFrame->get_Height());
	}
}
void CGraphicsRenderer::Create(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib)
{
	LONG lRectLeft	= (LONG)oRect.left;
	LONG lRectTop	= (LONG)oRect.top;
	LONG lWidth		= (LONG)oRect.GetWidth();
	LONG lHeight	= (LONG)oRect.GetHeight();

	m_dPixelsWidth	= lWidth;
	m_dPixelsHeight = lHeight;

	LONG lStride	= 4 * lWidthControl;

	m_pPixels = pPixels + (4 * lRectLeft);
	m_pPixels += lStride * lRectTop;

	RELEASEOBJECT(m_pRenderer);
	RELEASEOBJECT(m_pPath);

	m_pRenderer = new Aggplus::CGraphics();
	m_pPath		= new Aggplus::CGraphicsPath();

    m_lClipLeft		= (std::max)(0, (int) lRectLeft);
    m_lClipTop		= (std::max)(0, (int) lRectTop);

	//LONG lClipRight		= min(lWidth, -lRectLeft + lWidthControl);
	//LONG lClipBottom	= min(lHeight, -lRectTop + lHeightControl);

    LONG lClipRight		= (std::min)((LONG)oRect.right, lWidthControl);
    LONG lClipBottom	= (std::min)((LONG)oRect.bottom, lHeightControl);

	m_lClipWidth	= lClipRight - m_lClipLeft;
	m_lClipHeight	= lClipBottom - m_lClipTop;

	m_pRenderer->Create2(pPixels, lWidthControl, lHeightControl, -4 * lWidthControl, 0, 
		m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight, oRect.GetWidth(), oRect.GetHeight(), pDib);

	Aggplus::CMatrix oBase(1, 0, 0, 1, oRect.left, oRect.top);
	m_pRenderer->SetBaseTransform(&oBase);

	m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
	m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
	m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
}
void CGraphicsRenderer::CreateFlip(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib)
{
	LONG lRectLeft	= (LONG)oRect.left;
	LONG lRectTop	= (LONG)oRect.top;
	LONG lWidth		= (LONG)oRect.GetWidth();
	LONG lHeight	= (LONG)oRect.GetHeight();

	m_dPixelsWidth	= lWidth;
	m_dPixelsHeight = lHeight;

	LONG lStride	= 4 * lWidthControl;

	RELEASEOBJECT(m_pRenderer);
	RELEASEOBJECT(m_pPath);

	m_pRenderer = new Aggplus::CGraphics();
	m_pPath		= new Aggplus::CGraphicsPath();

    m_lClipLeft		= (std::max) (0, (int) lRectLeft);
    m_lClipTop		= (std::max) (0, (int) lRectTop);

	//LONG lClipRight		= min(lWidth, -lRectLeft + lWidthControl);
	//LONG lClipBottom	= min(lHeight, -lRectTop + lHeightControl);

    LONG lClipRight		= (std::min) ((LONG)oRect.right, lWidthControl);
    LONG lClipBottom	= (std::min) ((LONG)oRect.bottom, lHeightControl);

	m_lClipWidth	= lClipRight - m_lClipLeft;
	m_lClipHeight	= lClipBottom - m_lClipTop;

	//m_pRenderer->Create(m_pPixels, lWidth, lHeight, -4 * lWidthControl, 0, colorModeRGBA8);
	m_pRenderer->Create2(pPixels, lWidthControl, lHeightControl, 4 * lWidthControl, 0, 
		m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight, oRect.GetWidth(), oRect.GetHeight(), pDib);

	Aggplus::CMatrix oBase(1, 0, 0, 1, oRect.left, oRect.top);
	m_pRenderer->SetBaseTransform(&oBase);

	m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
	m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
	m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
}

void CGraphicsRenderer::put_GlobalAlphaEnabled(const bool& bEnabled, const double& dVal)
{
	m_bGlobalAlphaEnabled = bEnabled;
	if (m_bGlobalAlphaEnabled)
		m_dGlobalAlpha = dVal;
	else
		m_dGlobalAlpha = 1.0;

	m_pRenderer->m_dGlobalAlpha = m_dGlobalAlpha;
}

void CGraphicsRenderer::AddRect(const double& x, const double& y, const double& w, const double& h)
{
	if (!CheckValidate())
		return;

	m_pPath->MoveTo(x, y);
	m_pPath->LineTo(x + w, y);
	m_pPath->LineTo(x + w, y + h);
	m_pPath->LineTo(x, y + h);
	m_pPath->CloseFigure();
}

void CGraphicsRenderer::SetGammaStroke(double value)
{
    m_dGammaStroke = value;
}

// SAVE/RESTORE section
class CGraphicsRenderer_State : public IGraphicsRenderer_State
{
public:
    CGraphicsRenderer_State() : IGraphicsRenderer_State()
    {
    }
    CGraphicsRenderer_State(const Aggplus::CGraphics_ClipState& oState) : IGraphicsRenderer_State(), m_oClipState(oState)
    {
    }
    virtual ~CGraphicsRenderer_State()
    {
    }

public:
    NSStructures::CPen              m_oPen;
    NSStructures::CBrush            m_oBrush;
    NSStructures::CFont             m_oFont;

    Aggplus::CMatrix                m_oTransform;

    double                          m_dGlobalAlpha;
    bool                            m_bGlobalAlphaEnabled;
    bool                            m_bIntegerGrid;

    Aggplus::CGraphics_ClipState    m_oClipState;
};

void CGraphicsRenderer::Save()
{
    if (!m_pRenderer)
        return;

    CGraphicsRenderer_State* pState = new CGraphicsRenderer_State(m_pRenderer->m_oClipState);
    pState->m_oPen      = m_oPen;
    pState->m_oBrush    = m_oBrush;
    pState->m_oFont     = m_oFont;

    pState->m_oTransform    = *m_pRenderer->GetTransform();

    pState->m_dGlobalAlpha          = m_dGlobalAlpha;
    pState->m_bGlobalAlphaEnabled   = m_bGlobalAlphaEnabled;

    pState->m_bIntegerGrid  = m_pRenderer->m_bIntegerGrid;

    m_arStates.push_back(pState);
}
void CGraphicsRenderer::Restore()
{
    if (!m_pRenderer)
        return;

    if (0 == m_arStates.size())
        return;

    CGraphicsRenderer_State* pState = (CGraphicsRenderer_State*)m_arStates.at(m_arStates.size() - 1);
    m_arStates.pop_back();

    m_oPen      = pState->m_oPen;
    m_oBrush    = pState->m_oBrush;
    m_oFont     = pState->m_oFont;

    ApplyTransform(&pState->m_oTransform);
    this->put_IntegerGrid(pState->m_bIntegerGrid);
    this->put_GlobalAlphaEnabled(pState->m_bGlobalAlphaEnabled, pState->m_dGlobalAlpha);

    m_pRenderer->ResetClip();
    for (std::vector<Aggplus::CGraphics_ClipStateRecord*>::iterator i = pState->m_oClipState.Records.begin(); i != pState->m_oClipState.Records.end(); i++)
    {
        Aggplus::CGraphics_ClipStateRecord* pRecord = *i;
        m_pRenderer->InternalClip(pRecord->Path, pRecord->Transform, pRecord->Operation);
    }

    RELEASEOBJECT(pState);
}
