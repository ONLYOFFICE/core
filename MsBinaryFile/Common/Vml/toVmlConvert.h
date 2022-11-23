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
#pragma once

#include <vector>

#include "Common.h"
#include "../../../DesktopEditor/graphics/GraphicsPath.h"

class COOXToVMLGeometry : public IRenderer
{
private:
	class _CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		_CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		~_CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

        AVSINLINE void AddSize(size_t nSize);

		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

        AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
        AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

        AVSINLINE std::wstring GetData()
		{
			std::wstring str(m_pData, (int)m_lSizeCur);
			return str;
		}

        AVSINLINE void AddCharNoCheck(const WCHAR& wc)
		{
			*m_pDataCur++ = wc;
			++m_lSizeCur;
		}
        AVSINLINE void AddIntNoCheck(int val);
        AVSINLINE void AddStringNoCheck(const wchar_t* pData, const int& len)
		{
			memcpy(m_pDataCur, pData, len << 1);
			m_pDataCur += len;
			m_lSizeCur += len;
		}
        AVSINLINE void AddSpaceNoCheck()
		{
			*m_pDataCur = WCHAR(' ');
			++m_pDataCur;
			++m_lSizeCur;
		}
	};


public:
	COOXToVMLGeometry();
	~COOXToVMLGeometry();
// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType){ return S_OK; }
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage(){ return S_OK; }
	virtual HRESULT get_Height(double* dHeight){ return S_OK; }
	virtual HRESULT put_Height(const double& dHeight)
	{
		m_dScaleY = dHeight;
		return S_OK;
	}
	virtual HRESULT get_Width(double* dWidth){ return S_OK; }
	virtual HRESULT put_Width(const double& dWidth)
	{
		m_dScaleX = dWidth;
		return S_OK; 
	}
	virtual HRESULT get_DpiX(double* dDpiX){ return S_OK; }
	virtual HRESULT get_DpiY(double* dDpiY){ return S_OK; }

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor){ return S_OK; }
	virtual HRESULT put_PenColor(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_PenAlpha(LONG* lAlpha){ return S_OK; }
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)
	{
		m_bIsStrokePart = (0 != lAlpha) ? true : false;
		return S_OK; 
	}
	virtual HRESULT get_PenSize(double* dSize){ return S_OK; }
	virtual HRESULT put_PenSize(const double& dSize){ return S_OK; }
	virtual HRESULT get_PenDashStyle(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenDashStyle(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineStartCap(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineStartCap(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineEndCap(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineEndCap(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineJoin(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineJoin(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenDashOffset(double* dOffset){ return S_OK; }
	virtual HRESULT put_PenDashOffset(const double& dOffset){ return S_OK; }
	virtual HRESULT get_PenAlign(LONG* lAlign){ return S_OK; }
	virtual HRESULT put_PenAlign(const LONG& lAlign){ return S_OK; }
	virtual HRESULT get_PenMiterLimit(double* dOffset){ return S_OK; }
	virtual HRESULT put_PenMiterLimit(const double& dOffset){ return S_OK; }
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount){ return S_OK; }

// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType){ return S_OK; }
	virtual HRESULT put_BrushType(const LONG& lType){ return S_OK; }
	virtual HRESULT get_BrushColor1(LONG* lColor){ return S_OK; }
	virtual HRESULT put_BrushColor1(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha){ return S_OK; }
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)
	{
		m_bIsFillPart = (0 != lAlpha) ? true : false;
		return S_OK;
	}
	virtual HRESULT get_BrushColor2(LONG* lColor){ return S_OK; }
	virtual HRESULT put_BrushColor2(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha){ return S_OK; }
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha){ return S_OK; }
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath){ return S_OK; }
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath){ return S_OK; }
	virtual HRESULT get_BrushTextureMode(LONG* lMode){ return S_OK; }
	virtual HRESULT put_BrushTextureMode(const LONG& lMode){ return S_OK; }
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha){ return S_OK; }
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha){ return S_OK; }
	virtual HRESULT get_BrushLinearAngle(double* dAngle){ return S_OK; }
	virtual HRESULT put_BrushLinearAngle(const double& dAngle){ return S_OK; }
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height){ return S_OK; }
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height){ return S_OK; }

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount){ return S_OK; }

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName){ return S_OK; }
	virtual HRESULT put_FontName(const std::wstring& bsName){ return S_OK; }
	virtual HRESULT get_FontPath(std::wstring* bsName){ return S_OK; }
	virtual HRESULT put_FontPath(const std::wstring& bsName){ return S_OK; }
	virtual HRESULT get_FontSize(double* dSize){ return S_OK; }
	virtual HRESULT put_FontSize(const double& dSize){ return S_OK; }
	virtual HRESULT get_FontStyle(LONG* lStyle){ return S_OK; }
	virtual HRESULT put_FontStyle(const LONG& lStyle){ return S_OK; }
	virtual HRESULT get_FontStringGID(INT* bGID){ return S_OK; }
	virtual HRESULT put_FontStringGID(const INT& bGID){ return S_OK; }
	virtual HRESULT get_FontCharSpace(double* dSpace){ return S_OK; }
	virtual HRESULT put_FontCharSpace(const double& dSpace){ return S_OK; }
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex){ return S_OK; }
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex){ return S_OK; }

//-------- Функции для вывода текста --------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)
	{
		m_lCurrentCommandType = lType;
		return S_OK; 
	}
	virtual HRESULT EndCommand(const DWORD& lType);

//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y);
	virtual HRESULT PathCommandLineTo(const double& x, const double& y);
	virtual HRESULT PathCommandLinesTo(double* points, const int& count);
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count);
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
	virtual HRESULT PathCommandClose();
	virtual HRESULT PathCommandEnd(){ return S_OK; }
	virtual HRESULT DrawPath(const LONG& nType){ return S_OK; }
	virtual HRESULT PathCommandStart(){ return S_OK; }
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y);

    virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h){ return S_OK; }
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255){ return S_OK; }	

// transform --------------------------------------------------------------------------------
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6){ return S_OK; }
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF){ return S_OK; }
	virtual HRESULT ResetTransform(){ return S_OK; }

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode){ return S_OK; }
	virtual HRESULT put_ClipMode(const LONG& lMode){ return S_OK; }

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand){ return S_OK; }
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand){ return S_OK; }
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand){ return S_OK; }

// additiaonal params ----------------------------------------------------------------------
	HRESULT NewShape();
	HRESULT ResultPath(std::wstring* bstrVal);
private:
	
	Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleGraphicsConverter;
	
	bool m_bIsFillPart;
	bool m_bIsStrokePart;

	double m_dScaleX;
	double m_dScaleY;

	LONG m_lCountPathCommands;

	_CStringWriter m_oWriter;

	LONG m_lCurrentCommandType;	// текущая команда
	
	inline void MoveTo(const double& dX, const double& dY)
	{
		m_oWriter.AddSize(30);
		m_oWriter.AddCharNoCheck((WCHAR)'m');
		m_oWriter.AddIntNoCheck(int(dX + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(dY + 0.5));

		m_lCountPathCommands++;
	}
	inline void LineTo(const double& dX, const double& dY)
	{
		m_oWriter.AddSize(30);
		m_oWriter.AddCharNoCheck((WCHAR)'l');
		m_oWriter.AddIntNoCheck(int(dX + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(dY + 0.5));

		m_lCountPathCommands++;
	}
	inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		m_oWriter.AddSize(90);
		m_oWriter.AddCharNoCheck((WCHAR)'c');
		m_oWriter.AddIntNoCheck(int(x1 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y1 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(x2 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y2 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(x3 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y3 + 0.5));

		m_lCountPathCommands++;
	}
	void Start()
	{
		m_lCountPathCommands = 0;
	}
	void End()
	{
		m_lCountPathCommands = 0;
	}
	void Close()
	{
		if (1 >= m_lCountPathCommands)
		{
			// 2007 office имеет проблемы с путями вида moveto-close.
			return;
		}

		m_oWriter.AddCharNoCheck((WCHAR)'x');
		m_lCountPathCommands++;
	}	
};

