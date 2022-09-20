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
#include "../include/ASCSVGWriter.h"

#include "../../DesktopEditor/graphics/GraphicsPath.h"
#include "VectorGraphicsWriter2.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"

namespace NSHtmlRenderer
{
    CASCSVGWriter::CASCSVGWriter(bool bIsInitializeFonts)
	{
        m_dDpiX = 72;
        m_dDpiY	= 72;

		m_dWidth = 100;
		m_dHeight = 100;
		m_bIsRaster = false;

		m_lClipMode = c_nClipRegionTypeWinding;

		m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
		m_pSimpleGraphicsConverter->SetRenderer(this);

		m_dTransformAngle = 0.0;

		m_pFontManager = NULL;
		m_pPen = new NSStructures::CPen();
		m_pBrush = new NSStructures::CBrush();
		m_pFont = new NSStructures::CFont();
		m_pInstalledFont = new NSStructures::CFont();

		m_pBaseTransform = new Aggplus::CMatrix();
		m_pTransform = new Aggplus::CMatrix();
		m_pFullTransform = new Aggplus::CMatrix();

		//todo вроде как подбора нет, но может нужно передавать?
        m_pFontManager = NULL;
        if (bIsInitializeFonts)
        {
            m_pFontManager = NSFonts::NSFontManager::Create();
            m_pFontManager->Initialize();
        }

		m_pVectorWriter = new NSHtmlRenderer::CSVGGraphicsWriter();
		m_pVectorWriter->m_pSimpleConverter  = m_pSimpleGraphicsConverter;
		m_pVectorWriter->m_pFullTransform	= m_pFullTransform;
		m_pVectorWriter->SetSettings(m_pPen, m_pBrush, m_pSimpleGraphicsConverter);
	}
	CASCSVGWriter::~CASCSVGWriter()
	{
		RELEASEOBJECT(m_pSimpleGraphicsConverter);
		RELEASEOBJECT(m_pPen);
		RELEASEOBJECT(m_pBrush);
		RELEASEOBJECT(m_pFont);
		RELEASEOBJECT(m_pInstalledFont);
		RELEASEOBJECT(m_pBaseTransform);
		RELEASEOBJECT(m_pTransform);
		RELEASEOBJECT(m_pFullTransform);
		RELEASEOBJECT(m_pVectorWriter);

		RELEASEINTERFACE(m_pFontManager);
	}
	void CASCSVGWriter::Reset()
	{
        m_dDpiX = 72;
        m_dDpiY	= 72;

		m_dWidth = 100;
		m_dHeight = 100;
		m_bIsRaster = false;

		m_lClipMode = c_nClipRegionTypeWinding;

		RELEASEOBJECT(m_pSimpleGraphicsConverter);

		RELEASEOBJECT(m_pPen);
		RELEASEOBJECT(m_pBrush);
		RELEASEOBJECT(m_pFont);
		RELEASEOBJECT(m_pInstalledFont);
		RELEASEOBJECT(m_pBaseTransform);
		RELEASEOBJECT(m_pTransform);
		RELEASEOBJECT(m_pFullTransform);
		RELEASEOBJECT(m_pVectorWriter);

		m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
		m_pSimpleGraphicsConverter->SetRenderer(this);

		m_dTransformAngle = 0.0;

		m_pPen = new NSStructures::CPen();
		m_pBrush = new NSStructures::CBrush();
		m_pFont = new NSStructures::CFont();
		m_pInstalledFont = new NSStructures::CFont();

		m_pBaseTransform = new Aggplus::CMatrix();
		m_pTransform = new Aggplus::CMatrix();
		m_pFullTransform = new Aggplus::CMatrix();

		m_pVectorWriter = new NSHtmlRenderer::CSVGGraphicsWriter();
		m_pVectorWriter->m_pSimpleConverter  = m_pSimpleGraphicsConverter;
		m_pVectorWriter->m_pFullTransform	= m_pFullTransform;
		m_pVectorWriter->SetSettings(m_pPen, m_pBrush, m_pSimpleGraphicsConverter);
	}

	HRESULT CASCSVGWriter::get_Type(LONG* lType)
	{
		if (NULL == lType)
			return S_FALSE;

		*lType = c_nSVGRendrerer;
		return S_OK;
	}
	//-------- Функции для работы со страницей --------------------------------------------------
	HRESULT CASCSVGWriter::NewPage()
	{	
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_Height(double* dHeight)
	{
		*dHeight = m_dHeight;	
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_Height(const double& dHeight)
	{
		m_dHeight = dHeight;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_Width(double* dWidth)
	{
		*dWidth = m_dWidth;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_Width(const double& dWidth)
	{
		m_dWidth = dWidth;
		return S_OK;
	}

	HRESULT CASCSVGWriter::get_DpiX(double* dDpiX)
	{
		if (NULL != dDpiX)
			*dDpiX = m_dDpiX;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_DpiY(double* dDpiY)
	{
		if (NULL != dDpiY)
			*dDpiY = m_dDpiY;
		return S_OK;
	}
	// pen --------------------------------------------------------------------------------------
	HRESULT CASCSVGWriter::get_PenColor(LONG* lColor)
	{
		*lColor = m_pPen->Color;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenColor(const LONG& lColor)
	{
		m_pPen->Color = lColor;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenAlpha(LONG* lAlpha)
	{
		*lAlpha = m_pPen->Alpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenAlpha(const LONG& lAlpha)
	{
		m_pPen->Alpha = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenSize(double* dSize)
	{
		*dSize = m_pPen->Size;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenSize(const double& dSize)
	{
		m_pPen->Size = dSize;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenDashStyle(BYTE* val)
	{
		*val = m_pPen->DashStyle;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenDashStyle(const BYTE& val)
	{
		m_pPen->DashStyle = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenLineStartCap(BYTE* val)
	{
		*val = m_pPen->LineStartCap;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenLineStartCap(const BYTE& val)
	{
		m_pPen->LineStartCap = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenLineEndCap(BYTE* val)
	{
		*val = m_pPen->LineEndCap;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenLineEndCap(const BYTE& val)
	{
		m_pPen->LineEndCap = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenLineJoin(BYTE* val)
	{
		*val = m_pPen->LineJoin;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenLineJoin(const BYTE& val)
	{
		m_pPen->LineJoin = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenDashOffset(double* val)
	{
		*val = m_pPen->DashOffset;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenDashOffset(const double& val)
	{
		m_pPen->DashOffset = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenAlign(LONG* val)
	{
		*val = m_pPen->Align;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenAlign(const LONG& val)
	{
		m_pPen->Align = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_PenMiterLimit(double* val)
	{
		*val = m_pPen->MiterLimit;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_PenMiterLimit(const double& val)
	{
		m_pPen->MiterLimit = val;
		return S_OK;
	}
	HRESULT CASCSVGWriter::PenDashPattern(double* pPattern, LONG lCount)
	{
		if (NULL != pPattern)
		{
			m_pPen->SetDashPattern(pPattern, lCount);
		}

		return S_OK;
	}
	// brush ------------------------------------------------------------------------------------
	HRESULT CASCSVGWriter::get_BrushType(LONG* lType)
	{
		*lType = m_pBrush->Type;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushType(const LONG& lType)
	{
		m_pBrush->Type = lType;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushColor1(LONG* lColor)
	{
		*lColor = m_pBrush->Color1;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushColor1(const LONG& lColor)
	{
		m_pBrush->Color1 = lColor;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushAlpha1(LONG* lAlpha)
	{
		*lAlpha = m_pBrush->Alpha1;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushAlpha1(const LONG& lAlpha)
	{
		m_pBrush->Alpha1 = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushColor2(LONG* lColor)
	{
		*lColor = m_pBrush->Color2;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushColor2(const LONG& lColor)
	{
		m_pBrush->Color2 = lColor;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushAlpha2(LONG* lAlpha)
	{
		*lAlpha = m_pBrush->Alpha2;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushAlpha2(const LONG& lAlpha)
	{
		m_pBrush->Alpha2 = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushTexturePath(std::wstring* bsPath)
	{
		*bsPath = m_pBrush->TexturePath;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushTexturePath(const std::wstring& bsPath)
	{
		m_pBrush->TexturePath = bsPath;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushTextureMode(LONG* lMode)
	{
		*lMode = m_pBrush->TextureMode;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushTextureMode(const LONG& lMode)
	{
		m_pBrush->TextureMode = lMode;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
	{
		*lTxAlpha = m_pBrush->TextureAlpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushTextureAlpha(const LONG& lTxAlpha)
	{
		m_pBrush->TextureAlpha = lTxAlpha;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_BrushLinearAngle(double* dAngle)
	{
		*dAngle = m_pBrush->LinearAngle;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_BrushLinearAngle(const double& dAngle)
	{
		m_pBrush->LinearAngle = dAngle;
		return S_OK;
	}
	HRESULT CASCSVGWriter::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
	{
		m_pBrush->Rectable = val;
		m_pBrush->Rect.X = (float)left;
		m_pBrush->Rect.Y = (float)top;
		m_pBrush->Rect.Width  = (float)width;
		m_pBrush->Rect.Height = (float)height;	

		return S_OK;
	}
	HRESULT CASCSVGWriter::BrushBounds(const double& left, const double& top, const double& width, const double& height)
	{
		return S_OK;
	}

	HRESULT CASCSVGWriter::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
	{
		return S_OK;
	}

	// font -------------------------------------------------------------------------------------
	HRESULT CASCSVGWriter::get_FontName(std::wstring* bsName)
	{
		*bsName = m_pFont->Name;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontName(const std::wstring& bsName)
	{
		m_pFont->Name = bsName;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontPath(std::wstring* bsName)
	{
		*bsName = m_pFont->Path;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontPath(const std::wstring& bsName)
	{
		m_pFont->Path = bsName;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontSize(double* dSize)
	{
		*dSize = m_pFont->Size;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontSize(const double& dSize)
	{
		m_pFont->Size = dSize;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontStyle(LONG* lStyle)
	{
		*lStyle = m_pFont->GetStyle();
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontStyle(const LONG& lStyle)
	{
		m_pFont->SetStyle(lStyle);
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontStringGID(INT* bGID)
	{
		*bGID = m_pFont->StringGID;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontStringGID(const INT& bGID)
	{
		m_pFont->StringGID = bGID;
		m_pFontManager->SetStringGID(bGID);
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontCharSpace(double* dSpace)
	{
		*dSpace = m_pFont->CharSpace;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontCharSpace(const double& dSpace)
	{
		m_pFont->CharSpace = dSpace;
		return S_OK;
	}
	HRESULT CASCSVGWriter::get_FontFaceIndex(int* lFaceIndex)
	{
		*lFaceIndex = m_pFont->FaceIndex;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_FontFaceIndex(const int& lFaceIndex)
	{
		m_pFont->FaceIndex = lFaceIndex;
		return S_OK;
	}
	//-------- Функции для вывода текста --------------------------------------------------------
	HRESULT CASCSVGWriter::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_bIsRaster)
            return S_OK;

        m_bIsTextPath = true;
        PathCommandEnd();
        BeginCommand(c_nPathType);
        PathCommandTextCHAR(c, x, y, w, h);
        DrawPath(c_nWindingFillMode);
        EndCommand(c_nPathType);
        PathCommandEnd();
        m_bIsTextPath = false;
        return S_OK;
	}
	HRESULT CASCSVGWriter::CommandDrawText(const std::wstring& bsText,const double& x,const double& y,const double& w, const double& h)
	{
		if (c_nHyperlinkType == m_lCurrentCommandType)
			return S_OK;

		if (m_bIsRaster)
			return S_OK;

        m_bIsTextPath = true;
        PathCommandEnd();
        BeginCommand(c_nPathType);
        PathCommandText(bsText, x, y, w, h);
        DrawPath(c_nWindingFillMode);
        EndCommand(c_nPathType);
        PathCommandEnd();
        m_bIsTextPath = false;
        return S_OK;
	}
	HRESULT CASCSVGWriter::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
	{
        if (m_bIsRaster)
            return S_OK;

        m_bIsTextPath = true;
        PathCommandEnd();
        BeginCommand(c_nPathType);
        PathCommandTextExCHAR(c, gid, x, y, w, h);
        DrawPath(c_nWindingFillMode);
        EndCommand(c_nPathType);
        PathCommandEnd();
        m_bIsTextPath = false;
        return S_OK;
	}
	HRESULT CASCSVGWriter::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
	{
		if (m_bIsRaster)
			return S_OK;

        m_bIsTextPath = true;
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsUnicodeText, pGids,nGidsCount , x, y, w, h);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
        m_bIsTextPath = false;
		return S_OK;	
	}
	//-------- Маркеры для команд ---------------------------------------------------------------
	HRESULT CASCSVGWriter::BeginCommand(const DWORD& lType)
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nClipType == lType)
		{
			m_pVectorWriter->WritePathClip();
		}
		else if (c_nPathType == lType)
		{
			m_pVectorWriter->WritePathStart();
			m_pSimpleGraphicsConverter->PathCommandEnd();
		}	
		m_lCurrentCommandType = lType;
		return S_OK;
	}
	HRESULT CASCSVGWriter::EndCommand(const DWORD& lType)
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nClipType == lType)
		{
			m_pVectorWriter->WritePathClipEnd();
		}
		else if (c_nResetClipType == lType)
		{
			m_pVectorWriter->WritePathResetClip();
		}	
		m_lCurrentCommandType = -1;
		return S_OK;
	}
	//-------- Функции для работы с Graphics Path -----------------------------------------------
	HRESULT CASCSVGWriter::PathCommandMoveTo(const double& fX, const double& fY)
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			MoveTo(fX, fY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
		}
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandLineTo(const double& fX, const double& fY)
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			LineTo(fX, fY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
		}

		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandLinesTo(double* points, const int& count)
	{
		if (m_bIsRaster)
			return S_OK;

		m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandCurveTo(const double& fX1, const double& fY1, const double& fX2, const double& fY2, const double& fX3, const double& fY3)
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}

		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandCurvesTo(double* points, const int& count)
	{
		if (m_bIsRaster)
			return S_OK;

		m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandArcTo(const double&  fX, const double&  fY, const double&  fWidth, const double&  fHeight, const double&  fStartAngle, const double&  fSweepAngle)
	{
		if (m_bIsRaster)
			return S_OK;

		m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandClose()
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			Close();
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandClose();
		}

		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandEnd()
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			End();
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
		}

		return S_OK;
	}
	HRESULT CASCSVGWriter::DrawPath(const long&  nType)
	{
		switch (m_pBrush->Type)
		{
			case c_BrushTypeTexture:
			case c_BrushTypePathGradient1:
			case c_BrushTypePathGradient2:
			{
				m_bIsRaster = true;
				break;
			}
		default:
			break;
		}

		if (m_bIsRaster)
			return S_OK;

		m_pVectorWriter->WriteDrawPath(nType);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandStart()
	{
		if (m_bIsRaster)
			return S_OK;

		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			Start();
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_pSimpleGraphicsConverter->PathCommandStart();
		}

		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
	{
		m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
	{
        if (m_bIsRaster)
            return S_OK;

        int _c = (int)c;

        _SetFont();
		m_pSimpleGraphicsConverter->PathCommandText2(&_c, NULL, 1, m_pFontManager, x, y, w, h);
        return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandText(const std::wstring& bsText, const double& fX, const double& fY, const double& fWidth, const double& fHeight)
	{
		if (m_bIsRaster)
			return S_OK;

        _SetFont();
        m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, 0);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
	{
        if (m_bIsRaster)
            return S_OK;

        _SetFont();

        int _c = (int)c;
        int _g = (int)gid;

        m_pSimpleGraphicsConverter->PathCommandText2(&_c, &_g, 1, m_pFontManager, x, y, w, h);
		return S_OK;
	}
	HRESULT CASCSVGWriter::PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
	{
		if (m_bIsRaster)
			return S_OK;

        _SetFont();

        m_pSimpleGraphicsConverter->PathCommandText2(sText, (const int*)pGids, nGidsCount, m_pFontManager, x, y, w, h);
        return S_OK;
	}
	//-------- Функции для вывода изображений ---------------------------------------------------
	HRESULT CASCSVGWriter::DrawImage(IGrObject* pImage, const double& fX, const double& fY, const double& fWidth, const double& fHeight)
	{
		m_bIsRaster = true;
		/*
		double x = fX;
		double y = fY;

		double r = fX + fWidth;
		double b = fY + fHeight;

		if (0 == m_dTransformAngle)
		{
		m_pFullTransform->TransformPoint(x, y);
		m_pFullTransform->TransformPoint(r, b);
		}
		else
		{
		NSHtmlRenderer::CMatrix oTemp  = *m_pFullTransform;

		double dCx = (x + r) / 2;
		double dCy = (y + b) / 2;
		m_pFullTransform->TransformPoint(dCx, dCy);
		oTemp.RotateAt(-m_dTransformAngle, dCx, dCy, Aggplus::MatrixOrderAppend);

		oTemp.TransformPoint(x, y);
		oTemp.TransformPoint(r, b);
		}

		m_oWriter.WriteImage(pInterface, x, y, r - x, b - y, m_dTransformAngle);
		*/

		return S_OK;
	}
	HRESULT CASCSVGWriter::DrawImageFromFile(const std::wstring& sVal, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)
	{
		m_bIsRaster = true;
		/*
		double x = fX;
		double y = fY;

		double r = fX + fWidth;
		double b = fY + fHeight;

		if (0 == m_dTransformAngle)
		{
		m_pFullTransform->TransformPoint(x, y);
		m_pFullTransform->TransformPoint(r, b);
		}
		else
		{
		NSHtmlRenderer::CMatrix oTemp  = *m_pFullTransform;

		double dCx = (x + r) / 2;
		double dCy = (y + b) / 2;
		m_pFullTransform->TransformPoint(dCx, dCy);
		oTemp.RotateAt(-m_dTransformAngle, dCx, dCy, Aggplus::MatrixOrderAppend);

		oTemp.TransformPoint(x, y);
		oTemp.TransformPoint(r, b);
		}

		m_oWriter.WriteImage((std::wstring)bstrVal, x, y, r - x, b - y, m_pTransform->z_Rotation());
		*/

		return S_OK;
	}
	// transform --------------------------------------------------------------------------------
	HRESULT CASCSVGWriter::SetTransform(const double& dA, const double& dB, const double& dC, const double& dD, const double& dE, const double& dF)
	{
		Aggplus::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
		*m_pTransform = oTrans;

		CalculateFullTransform();

		return S_OK;
	}
	HRESULT CASCSVGWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
        *pdA = m_pTransform->sx();
        *pdB = m_pTransform->shy();
        *pdC = m_pTransform->shx();
        *pdD = m_pTransform->sy();
        *pdE = m_pTransform->tx();
        *pdF = m_pTransform->ty();
		return S_OK;
	}
	HRESULT CASCSVGWriter::ResetTransform(void)
	{
		m_pTransform->Reset();
		CalculateFullTransform();
		return S_OK;
	}
	// -----------------------------------------------------------------------------------------
	HRESULT CASCSVGWriter::get_ClipMode(LONG* plMode)
	{
		*plMode = m_lClipMode;
		return S_OK;
	}
	HRESULT CASCSVGWriter::put_ClipMode(const LONG& lMode)
	{
		m_lClipMode = lMode;
		m_pVectorWriter->SetClipMode(m_lClipMode);
		return S_OK;
	}
	// additiaonal params ----------------------------------------------------------------------
	HRESULT CASCSVGWriter::SaveFile(const std::wstring& strFileSave)
	{
		NSFile::CFileBinary oFile;
		if(oFile.CreateFileW(strFileSave))
        {
            NSStringUtils::CStringBuilder oBuilder;
            WriteFormatted(L"<svg width=\"", (int)(m_dWidth), L"px\" height=\"", (int)(m_dHeight), L"px\" viewBox=\"0 0 ",
                           (int)(m_dWidth * SVG_WRITER_SCALE), L" ", (int)(m_dHeight * SVG_WRITER_SCALE),
                           L"\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", &oBuilder);

            oFile.WriteStringUTF8(oBuilder.GetData());
            oFile.WriteStringUTF8(m_pVectorWriter->m_oSVG.m_oDocument.GetData());
            oFile.WriteStringUTF8(L"</svg>");
        }
		oFile.CloseFile();
		return S_OK;
	}
	HRESULT CASCSVGWriter::ReInit()
	{
        m_dDpiX = 72.0;
        m_dDpiY	= 72.0;

		m_dWidth = 100;
		m_dHeight = 100;
		m_bIsRaster = false;

		m_lClipMode = c_nClipRegionTypeWinding;

		RELEASEOBJECT(m_pSimpleGraphicsConverter);
		m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
		m_pSimpleGraphicsConverter->SetRenderer(this);

		m_dTransformAngle = 0.0;


		m_pVectorWriter->m_pSimpleConverter  = m_pSimpleGraphicsConverter;
		m_pVectorWriter->m_pFullTransform	= m_pFullTransform;
		m_pVectorWriter->SetSettings(m_pPen, m_pBrush, m_pSimpleGraphicsConverter);

		m_pPen->SetDefaultParams();
		m_pBrush->SetDefaultParams();
		m_pFont->SetDefaultParams();

		m_pBaseTransform->Reset();
		m_pTransform->Reset();
		m_pFullTransform->Reset();

		m_pVectorWriter->m_oSVG.ReInit();
		return S_OK;
	}
	HRESULT CASCSVGWriter::IsRaster(bool* bVal)
	{
        *bVal	= m_bIsRaster;
		return S_OK;
	}
	HRESULT CASCSVGWriter::GetSVGDataSize(LONG* lVal)
	{
		*lVal = m_pVectorWriter ?  m_pVectorWriter->m_oSVG.m_oDocument.GetCurSize() : 0;
		return S_OK;
	}	
// additiaonal params ----------------------------------------------------------------------
	HRESULT CASCSVGWriter::CommandLong(const LONG& lType, const LONG& lCommand)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGWriter::CommandDouble(const LONG& lType, const double& dCommand)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGWriter::CommandString(const LONG& lType, const std::wstring& sCommand)
	{
		//todo new command
		return S_OK;
	}
	// --------------------------------------------------------------------------------------------
    void CASCSVGWriter::SetFontManager(NSFonts::IFontManager* pFontManager)
	{
		if (NULL == pFontManager)
			return;

		RELEASEINTERFACE(m_pFontManager);
		m_pFontManager = pFontManager;
		ADDREFINTERFACE(m_pFontManager);
	}
	void CASCSVGWriter::CalculateFullTransform()
	{
		*m_pFullTransform	= *m_pBaseTransform;
		m_pFullTransform->Multiply(m_pTransform, Aggplus::MatrixOrderPrepend);

		m_dTransformAngle	= m_pTransform->z_Rotation();
	}

	inline void CASCSVGWriter::MoveTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_pFullTransform->TransformPoint(x, y);

		m_pVectorWriter->WritePathMoveTo(x, y);
	}
	inline void CASCSVGWriter::LineTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_pFullTransform->TransformPoint(x, y);

		m_pVectorWriter->WritePathLineTo(x, y);
	}
	inline void CASCSVGWriter::CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		double _x1 = x1;
		double _y1 = y1;
		m_pFullTransform->TransformPoint(_x1, _y1);

		double _x2 = x2;
		double _y2 = y2;
		m_pFullTransform->TransformPoint(_x2, _y2);

		double _x3 = x3;
		double _y3 = y3;
		m_pFullTransform->TransformPoint(_x3, _y3);

		m_pVectorWriter->WritePathCurveTo(_x1, _y1, _x2, _y2, _x3, _y3);
	}
	void CASCSVGWriter::Start()
	{
		m_pVectorWriter->WritePathStart();
	}
	void CASCSVGWriter::End()
	{
		m_pVectorWriter->WriteEndPath();
	}
	void CASCSVGWriter::Close()
	{
		m_pVectorWriter->WritePathClose();
	}

	void CASCSVGWriter::_SetFont()
	{
		double dPix = m_pFont->CharSpace * m_dDpiX / 25.4;

		if (m_pInstalledFont->IsEqual(m_pFont))
		{
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_pFont->StringGID);
		if (1 < m_dWidth)
		{
			m_pFontManager->SetCharSpacing(dPix);
		}

		double dSizeFont =  m_pFont->Size;
		m_dTextScale = 1.0;
		bool bIsTransform = false;
		if (dSizeFont > 1000)
		{
			bIsTransform = true;
			m_dTextScale = dSizeFont / 1000;
			dSizeFont = 1000;
		}

		if (m_pFont->Path.empty())
		{
			m_pFontManager->LoadFontByName(m_pFont->Name, (float)dSizeFont, m_pFont->GetStyle(), m_dDpiX, m_dDpiY);
		}
		else
		{
			m_pFontManager->LoadFontFromFile(m_pFont->Path, (float)dSizeFont, m_dDpiX, m_dDpiY, 0);
		}

        if (bIsTransform && NULL != m_pFontManager->GetFile())
		{
            m_pFontManager->GetFile()->SetFontMatrix(m_dTextScale, 0, 0, m_dTextScale, 0, 0);
            m_pFontManager->GetFile()->CheckTextMatrix();
		}

		*m_pInstalledFont = *m_pFont;
	}
}
