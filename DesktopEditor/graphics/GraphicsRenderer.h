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
#ifndef _BUILD_GRAPHICS_RENDERER_H_
#define _BUILD_GRAPHICS_RENDERER_H_

#include "Graphics.h"
#include "ImageFilesCache.h"
#include "../raster/BgraFrame.h"
#include "./pro/Graphics.h"

class IGraphicsRenderer_State
{
public:
    int Type;

    IGraphicsRenderer_State()
    {
        // пока не используется
        Type = 0;
    }
    virtual ~IGraphicsRenderer_State()
    {
    }
};

class CGraphicsRenderer : public NSGraphics::IGraphicsRenderer
{
private:
	CFontManager*			m_pFontManager;
	Aggplus::CGraphics*		m_pRenderer;
	Aggplus::CGraphicsPath*	m_pPath;

	NSStructures::CPen		m_oPen;
	NSStructures::CBrush	m_oBrush;
	NSStructures::CFont		m_oFont;
	NSStructures::CFont		m_oInstalledFont;

	double m_dWidth;
	double m_dHeight;

	double m_dPixelsWidth;
	double m_dPixelsHeight;

	DWORD m_lCurrentCommandType;

	LONG  m_lCurrentClipMode;
	INT  m_bIsSetupClip;

	// область отсечения
	LONG	m_lClipLeft;
	LONG	m_lClipTop;
	LONG	m_lClipWidth;
	LONG	m_lClipHeight;

	// пикселы
	BYTE*			m_pPixels;
	Aggplus::CDIB*	m_pDIB;

	CImageFilesCache* m_pCache;

	double m_dGlobalAlpha;
    bool m_bGlobalAlphaEnabled;

    double m_dGammaStroke;

    std::vector<IGraphicsRenderer_State*> m_arStates;

public:
	CGraphicsRenderer();
    virtual ~CGraphicsRenderer();

public:
    virtual void SetImageCache(NSImages::IImageFilesCache* pCache);
    virtual void SetFontManager(NSFonts::IFontManager* pManager = NULL);
    virtual void CheckFontManager();
	
    virtual NSFonts::IFontManager* GetFontManager();
	BYTE* GetPixels(LONG& lWidth, LONG& lHeight);

	virtual void ClearInstallFont();
	void SetClipRect(double x, double y, double w, double h);

protected:
	INT CheckValidate(INT bOnlyGraphics = FALSE);
	void Clear();

	void UpdateSize();

public:
	void SavePen(NSStructures::CPen& oPen) { oPen = m_oPen; }
	void RestorePen(const NSStructures::CPen& oPen) { m_oPen = oPen; }

	void SaveBrush(NSStructures::CBrush& oBrush) { oBrush = m_oBrush; }
	void RestoreBrush(const NSStructures::CBrush& oBrush) { m_oBrush = oBrush; }
    virtual void SetSwapRGB(bool bValue){ if (m_pRenderer) m_pRenderer->m_bSwapRGB = bValue; }
    virtual void SetTileImageDpi(const double& dDpi) { if (m_pRenderer) m_pRenderer->m_dDpiTile = dDpi; }

    void Save();
    void Restore();

public:
// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType);
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage();
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth);
	virtual HRESULT get_DpiX(double* dDpiX);
	virtual HRESULT get_DpiY(double* dDpiY);

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor);
	virtual HRESULT put_PenColor(const LONG& lColor);
	virtual HRESULT get_PenAlpha(LONG* lAlpha);
	virtual HRESULT put_PenAlpha(const LONG& lAlpha);
	virtual HRESULT get_PenSize(double* dSize);
	virtual HRESULT put_PenSize(const double& dSize);
	virtual HRESULT get_PenDashStyle(BYTE* val);
	virtual HRESULT put_PenDashStyle(const BYTE& val);
	virtual HRESULT get_PenLineStartCap(BYTE* val);
	virtual HRESULT put_PenLineStartCap(const BYTE& val);
	virtual HRESULT get_PenLineEndCap(BYTE* val);
	virtual HRESULT put_PenLineEndCap(const BYTE& val);
	virtual HRESULT get_PenLineJoin(BYTE* val);
	virtual HRESULT put_PenLineJoin(const BYTE& val);
	virtual HRESULT get_PenDashOffset(double* dOffset);
	virtual HRESULT put_PenDashOffset(const double& dOffset);
	virtual HRESULT get_PenAlign(LONG* lAlign);
	virtual HRESULT put_PenAlign(const LONG& lAlign);
	virtual HRESULT get_PenMiterLimit(double* dOffset);
	virtual HRESULT put_PenMiterLimit(const double& dOffset);
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

// brush ------------------------------------------------------------------------------------
    virtual void put_BrushGradInfo(const NSStructures::GradientInfo &_ginfo) override {
        m_oBrush.m_oGradientInfo = _ginfo;
    }

	virtual HRESULT get_BrushType(LONG* lType);
	virtual HRESULT put_BrushType(const LONG& lType);
	virtual HRESULT get_BrushColor1(LONG* lColor);
	virtual HRESULT put_BrushColor1(const LONG& lColor);
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha);
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha);
	virtual HRESULT get_BrushColor2(LONG* lColor);
	virtual HRESULT put_BrushColor2(const LONG& lColor);
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha);
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha);
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath);
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath);
	virtual HRESULT get_BrushTextureMode(LONG* lMode);
	virtual HRESULT put_BrushTextureMode(const LONG& lMode);
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha);
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha);
	virtual HRESULT get_BrushLinearAngle(double* dAngle);
	virtual HRESULT put_BrushLinearAngle(const double& dAngle);
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height);
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height);
	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount);

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName);
	virtual HRESULT put_FontName(const std::wstring& bsName);
	virtual HRESULT get_FontPath(std::wstring* bsName);
	virtual HRESULT put_FontPath(const std::wstring& bsName);
	virtual HRESULT get_FontSize(double* dSize);
	virtual HRESULT put_FontSize(const double& dSize);
	virtual HRESULT get_FontStyle(LONG* lStyle);
	virtual HRESULT put_FontStyle(const LONG& lStyle);
	virtual HRESULT get_FontStringGID(INT* bGID);
	virtual HRESULT put_FontStringGID(const INT& bGID);
	virtual HRESULT get_FontCharSpace(double* dSpace);
	virtual HRESULT put_FontCharSpace(const double& dSpace);
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);

//-------- Функции для вывода текста --------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

    virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType);
	virtual HRESULT EndCommand(const DWORD& lType);

//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y);
	virtual HRESULT PathCommandLineTo(const double& x, const double& y);
	virtual HRESULT PathCommandLinesTo(double* points, const int& count);
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count);
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
	virtual HRESULT PathCommandClose();
	virtual HRESULT PathCommandEnd();
	virtual HRESULT DrawPath(const LONG& nType);
	virtual HRESULT PathCommandStart();
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y);

    virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

    virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h);
	virtual HRESULT DrawImageFromFile(const std::wstring& sFile, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255);

// transform --------------------------------------------------------------------------------
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
	virtual HRESULT ResetTransform();

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode);
	virtual HRESULT put_ClipMode(const LONG& lMode);

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

	virtual HRESULT StartConvertCoordsToIdentity();
	virtual HRESULT EndConvertCoordsToIdentity();

	void put_GlobalAlphaEnabled(const bool& bEnabled, const double& dVal);
	inline void put_IntegerGrid(const bool& bEnabled)
	{ 
		if (!m_pRenderer) 
			return; 
		m_pRenderer->m_bIntegerGrid = bEnabled; 
	}
	inline bool get_IntegerGrid()
	{
		if (!m_pRenderer) 
			return false; 
		return m_pRenderer->m_bIntegerGrid;
	}
	void AddRect(const double& x, const double& y, const double& w, const double& h);
	inline void SetFontAttack()
	{
		_SetFont();
	}
	virtual void put_BlendMode(const unsigned int nBlendMode) override;

public:
    virtual void CloseFont()
    {
        if (NULL != m_pFontManager)
            m_pFontManager->CloseFont();
        m_oInstalledFont.SetDefaultParams();
        m_oInstalledFont.Name = L"";        
    }

    void SetGammaStroke(double value);

protected:
	void _SetFont();
	void ApplyTransform(const double& d1, const double& d2, const double& d3, const double& d4, const double& d5, const double& d6);
	void ApplyTransform(Aggplus::CMatrix* pMatrix);
	void _ResetTransform();
	Aggplus::CBrush* CreateBrush(NSStructures::CBrush* pBrush);

public:
    virtual void CreateFromBgraFrame(CBgraFrame* pFrame);
	void Create(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib = NULL);
	void CreateFlip(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib = NULL);

	inline Aggplus::CMatrix* GetFullTransform()
	{
		return m_pRenderer->GetFullTransform();
	}
	inline Aggplus::CMatrix* GetTransformMatrix()
	{
		return m_pRenderer->GetTransform();
	}
    virtual void SetCoordTransformOffset(double dOffsetX, double dOffsetY)
	{
		Aggplus::CMatrix* pCoord = m_pRenderer->GetCoordTransform();
		pCoord->m_internal->m_agg_mtx.tx = dOffsetX;
		pCoord->m_internal->m_agg_mtx.ty = dOffsetY;

		m_pRenderer->CalculateFullTransform();
	}
	inline void CalculateFullTransform()
	{
		m_pRenderer->CalculateFullTransform();
	}

public:
	inline void PathCommandRect(double x, double y, double w, double h)
	{
		PathCommandMoveTo(x, y);
		PathCommandLineTo(x + w, y);
		PathCommandLineTo(x + w, y + h);
		PathCommandLineTo(x, y + h);
		PathCommandClose();
	}
	inline Aggplus::CMatrix* GetCoordTransform()
	{
		return m_pRenderer->GetCoordTransform();
	}
	inline void Fill()
	{
		DrawPath(c_nWindingFillMode);
	}
	inline void Stroke()
	{
		DrawPath(1);
	}

	inline double GetPixW() { return m_pRenderer->GetPixW(); }
	inline double GetPixH() { return m_pRenderer->GetPixH(); }

	// smart methods
	void drawHorLine(BYTE align, double y, double x, double r, double penW)
    {
		int pen_w = (int)((m_pRenderer->GetDpiX() * penW / 25.4) + 0.5);
        if (0 == pen_w)
            pen_w = 1;

		double __x	= x;
		double __y1 = y;
		double __r	= r;
		double __y2	= y;
		Aggplus::CMatrix* pMatrix = GetFullTransform();
		pMatrix->TransformPoint(__x, __y1);
		pMatrix->TransformPoint(__r, __y2);

        double _x = (int)__x;
		double _r = (int)__r + 1;

		m_oPen.Size = pen_w;

        switch (align)
        {
            case 0:
            {
				double _top = (int)__y1 + 0.5;
				_top = _top + pen_w / 2.0 - 0.5;
				m_pPath->Reset();
				m_pPath->MoveTo(_x, _top);
				m_pPath->LineTo(_r, _top);
				Stroke();
                break;
            }
            case 1:
            {
                // center
                double _center = (int)__y1 + 0.5;

                m_pPath->Reset();
                if (0 == (pen_w % 2))
                {
					m_pPath->MoveTo(_x, _center - 0.5);
					m_pPath->LineTo(_r, _center - 0.5);
                }
                else
                {
                    m_pPath->MoveTo(_x, _center);
					m_pPath->LineTo(_r, _center);
                }
                Stroke();

                break;
            }
            case 2:
            {
				double _bottom = (int)__y1 + 0.5;
				_bottom = _bottom - pen_w / 2.0 + 0.5;
				m_pPath->Reset();
				m_pPath->MoveTo(_x, _bottom);
				m_pPath->LineTo(_r, _bottom);
				Stroke();
                break;
            }
        }
    }

	void drawHorLine2(BYTE align, double y, double x, double r, double penW)
	{
		int pen_w = (int)((m_pRenderer->GetDpiX() * penW / 25.4) + 0.5);
        if (0 == pen_w)
            pen_w = 1;

		double __x	= x;
		double __y1 = y;
		double __r	= r;
		double __y2	= y;
		Aggplus::CMatrix* pMatrix = GetFullTransform();
		pMatrix->TransformPoint(__x, __y1);
		pMatrix->TransformPoint(__r, __y2);

        double _x = (int)__x;
        double _r = (int)__r + 1;

        switch (align)
        {
            case 0:
            {
                // top
                double _top = (int)__y1 + 0.5;

                double _pos1 = _top + pen_w / 2.0 - 0.5 - pen_w;
                double _pos2 = _pos1 + pen_w * 2.0;

				m_pPath->Reset();
				m_pPath->MoveTo(_x, _pos1);
				m_pPath->LineTo(_r, _pos1);
				Stroke();

				m_pPath->Reset();
				m_pPath->MoveTo(_x, _pos2);
				m_pPath->LineTo(_r, _pos2);
				Stroke();
                break;
            }
            case 1:
            {
                // center
                // TODO:

                break;
            }
            case 2:
            {
                // bottom
                // TODO:

                break;
            }
        }
	}

	void drawVerLine(BYTE align, double x, double y, double b, double penW)
	{
		int pen_w = (int)((m_pRenderer->GetDpiX() * penW / 25.4) + 0.5);
        if (0 == pen_w)
            pen_w = 1;

		double __x1 = x;
		double __y	= y;
		double __x2	= x;
		double __b	= b;

		Aggplus::CMatrix* pMatrix = GetFullTransform();
		pMatrix->TransformPoint(__x1, __y);
		pMatrix->TransformPoint(__x2, __b);

		double _y = (int)__y;
		double _b = (int)__b + 1;

		m_oPen.Size = pen_w;
		switch (align)
        {
            case 0:
            {
                // left
                double _left = (int)__x1 + 0.5;
				_left = _left + pen_w / 2.0 - 0.5;

				m_pPath->Reset();
				m_pPath->MoveTo(_left, _y);
				m_pPath->LineTo(_left, _b);
				Stroke();
                break;
            }
            case 1:
            {
                // center
				double _center = (int)__x1 + 0.5;
                
				m_pPath->Reset();
                if (0 == (pen_w % 2))
                {
					m_pPath->MoveTo(_center - 0.5, _y);
					m_pPath->LineTo(_center - 0.5, _b);                    
                }
                else
                {
					m_pPath->MoveTo(_center, _y);
					m_pPath->LineTo(_center, _b);                    
                }
				Stroke();

                break;
            }
            case 2:
            {
				// right
				double _right = (int)__x1 + 0.5;
				_right = _right - pen_w / 2.0 + 0.5;

				m_pPath->Reset();
				m_pPath->MoveTo(_right, _y);
				m_pPath->LineTo(_right, _b);
				Stroke();
                break;
            }
        }
	}

	void drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW)
	{
		int pen_w = (int)((m_pRenderer->GetDpiX() * penW / 25.4) + 0.5);
        if (0 == pen_w)
            pen_w = 1;

		double __x	= x;
		double __y1 = y;
		double __r	= r;
		double __y2	= y;
		Aggplus::CMatrix* pMatrix = GetFullTransform();
		pMatrix->TransformPoint(__x, __y1);
		pMatrix->TransformPoint(__r, __y2);

        double _x = (int)__x + 0.5;
        double _r = (int)__r + 0.5;

        if (leftMW != 0)
        {
            double _center = _x;

			int pen_mw = (int)((m_pRenderer->GetDpiX() * fabs(leftMW) * 2 / 25.4) + 0.5);
			if (0 == pen_mw)
				pen_mw = 1;

            if (leftMW < 0)
            {
				_x = _center - (pen_mw >> 1);                
            }
            else
            {
				_x = _center + ((pen_mw - 1) >> 1);
            }
        }
        if (rightMW != 0)
        {
            double _center = _r;

			int pen_mw = (int)((m_pRenderer->GetDpiX() * fabs(rightMW) * 2 / 25.4) + 0.5);
			if (0 == pen_mw)
				pen_mw = 1;

            if (rightMW < 0)
            {
				_r = _center - (pen_mw >> 1);
            }
            else
            {
				_r = _center + ((pen_mw - 1) >> 1);
            }
        }

		m_oPen.Size = pen_w;

        _x -= 0.5;
        _r += 0.5;

        switch (align)
        {
            case 0:
            {
                // top
				double _top = (int)__y1 + 0.5;
                _top = _top + pen_w / 2.0 - 0.5;

				m_pPath->Reset();
				m_pPath->MoveTo(_x, _top);
				m_pPath->LineTo(_r, _top);
				Stroke();
                break;
            }
            case 1:
            {
                // center
				double _center = (int)__y1 + 0.5;
                
				m_pPath->Reset();
                if (0 == (pen_w % 2))
                {
					m_pPath->MoveTo(_x, _center - 0.5);
					m_pPath->LineTo(_r, _center - 0.5);
                }
                else
                {
					m_pPath->MoveTo(_x, _center);
					m_pPath->LineTo(_r, _center);
                }
                Stroke();
                break;
            }
            case 2:
            {
				// bottom
				double _bottom = (int)__y1 + 0.5;
                _bottom = _bottom - pen_w / 2.0 + 0.5;

				m_pPath->Reset();
				m_pPath->MoveTo(_x, _bottom);
				m_pPath->LineTo(_r, _bottom);
				Stroke();
                break;
            }
        }
	}
};

#endif // _BUILD_GRAPHICS_RENDERER_H_
