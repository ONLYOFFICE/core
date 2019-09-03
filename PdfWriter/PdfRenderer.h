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
#ifndef _PDF_WRITER_PDFRENDERER_H
#define _PDF_WRITER_PDFRENDERER_H

#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/Image.h"
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#ifndef PDFWRITER_USE_DYNAMIC_LIBRARY
#define PDFWRITER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFWRITER_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace PdfWriter
{
	class CDocument;
	class CPage;
	class CFontCidTrueType;
	class CImageDict;
	class CShading;
	class CExtGrState;
	class CFontDict;
}

namespace Aggplus
{
	class CImage;
}

class CRendererCommandBase;
class CRendererTextCommand;

class PDFWRITER_DECL_EXPORT CPdfRenderer : public IRenderer
{
public:
	CPdfRenderer(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false);
	~CPdfRenderer();
	void         SaveToFile(const std::wstring& wsPath);
	void         SetPassword(const std::wstring& wsPassword);
	void		 SetDocumentID(const std::wstring& wsDocumentID);
	void         SetTempFolder(const std::wstring& wsPath);
	std::wstring GetTempFile();
	void         SetThemesPlace(const std::wstring& wsThemesPlace);
	std::wstring GetThemesPlace();
	//----------------------------------------------------------------------------------------
	// Тип рендерера
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType);
	//----------------------------------------------------------------------------------------
	// Функции для работы со страницей
	//----------------------------------------------------------------------------------------
	virtual HRESULT NewPage();
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth);
	virtual HRESULT get_DpiX(double* dDpiX);
	virtual HRESULT get_DpiY(double* dDpiY);
	//----------------------------------------------------------------------------------------
	// Функции для работы с Pen
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor);
	virtual HRESULT put_PenColor(const LONG& lColor);
	virtual HRESULT get_PenAlpha(LONG* lAlpha);
	virtual HRESULT put_PenAlpha(const LONG& lAlpha);
	virtual HRESULT get_PenSize(double* dSize);
	virtual HRESULT put_PenSize(const double& dSize);
	virtual HRESULT get_PenDashStyle(BYTE* nDashStyle);
	virtual HRESULT put_PenDashStyle(const BYTE& nDashStyle);
	virtual HRESULT get_PenLineStartCap(BYTE* nCapStyle);
	virtual HRESULT put_PenLineStartCap(const BYTE& nCapStyle);
	virtual HRESULT get_PenLineEndCap(BYTE* nCapStyle);
	virtual HRESULT put_PenLineEndCap(const BYTE& nCapStyle);
	virtual HRESULT get_PenLineJoin(BYTE* nJoinStyle);
	virtual HRESULT put_PenLineJoin(const BYTE& nJoinStyle);
	virtual HRESULT get_PenDashOffset(double* dOffset);
	virtual HRESULT put_PenDashOffset(const double& dOffset);
	virtual HRESULT get_PenAlign(LONG* lAlign);
	virtual HRESULT put_PenAlign(const LONG& lAlign);
	virtual HRESULT get_PenMiterLimit(double* dMiter);
	virtual HRESULT put_PenMiterLimit(const double& dMiter);
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);
	//----------------------------------------------------------------------------------------
	// Функции для работы с Brush
	//----------------------------------------------------------------------------------------
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
	virtual HRESULT get_BrushTexturePath(std::wstring* wsPath);
	virtual HRESULT put_BrushTexturePath(const std::wstring& wsPath);
	virtual HRESULT get_BrushTextureMode(LONG* lMode);
	virtual HRESULT put_BrushTextureMode(const LONG& lMode);
	virtual HRESULT get_BrushTextureAlpha(LONG* lAlpha);
	virtual HRESULT put_BrushTextureAlpha(const LONG& lAlpha);
	virtual HRESULT get_BrushLinearAngle(double* dAngle);
	virtual HRESULT put_BrushLinearAngle(const double& dAngle);
	virtual HRESULT BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
	virtual HRESULT BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
	virtual HRESULT put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount);
	//----------------------------------------------------------------------------------------
	// Функции для работы со шрифтами
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* wsName);
	virtual HRESULT put_FontName(const std::wstring& wsName);
	virtual HRESULT get_FontPath(std::wstring* wsPath);
	virtual HRESULT put_FontPath(const std::wstring& wsPath);
	virtual HRESULT get_FontSize(double* dSize);
	virtual HRESULT put_FontSize(const double& dSize);
	virtual HRESULT get_FontStyle(LONG* lStyle);
	virtual HRESULT put_FontStyle(const LONG& lStyle);
	virtual HRESULT get_FontStringGID(INT* bGid);
	virtual HRESULT put_FontStringGID(const INT& bGid);
	virtual HRESULT get_FontCharSpace(double* dSpace);
	virtual HRESULT put_FontCharSpace(const double& dSpace);
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);
	//----------------------------------------------------------------------------------------
	// Функции для вывода текста
	//----------------------------------------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Маркеры команд
	//----------------------------------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType);
	virtual HRESULT EndCommand(const DWORD& lType);
	//----------------------------------------------------------------------------------------
	// Функции для работы с патом
	//----------------------------------------------------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& dX, const double& dY);
	virtual HRESULT PathCommandLineTo(const double& dX, const double& dY);
	virtual HRESULT PathCommandLinesTo(double* pPoints, const int& nCount);
	virtual HRESULT PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe);
	virtual HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount);
	virtual HRESULT PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle);
	virtual HRESULT PathCommandClose();
	virtual HRESULT PathCommandEnd();
	virtual HRESULT DrawPath(const LONG& lType);
	virtual HRESULT PathCommandStart();
	virtual HRESULT PathCommandGetCurrentPoint(double* dX, double* dY);
	virtual HRESULT PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Функции для вывода изображений
	//----------------------------------------------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
	//----------------------------------------------------------------------------------------
	// Функции для выставления преобразования
	//----------------------------------------------------------------------------------------
	virtual HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
	virtual HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
	virtual HRESULT ResetTransform();
	//----------------------------------------------------------------------------------------
	// Тип клипа
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* lMode);
	virtual HRESULT put_ClipMode(const LONG& lMode);
	//----------------------------------------------------------------------------------------
	// Дополнительные функции
	//----------------------------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);
	//----------------------------------------------------------------------------------------
	// Дополнительные функции Pdf рендерера
	//----------------------------------------------------------------------------------------
	HRESULT CommandDrawTextPdf(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const std::wstring& wsSrcCodeText, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT PathCommandTextPdf(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const std::wstring& bsSrcCodeText, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT EnableBrushRect(const LONG& lEnable);
	HRESULT SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2);
	HRESULT SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2);
    HRESULT OnlineWordToPdf          (const std::wstring& wsSrcFile, const std::wstring& wsDstFile, const bool& bIsUsePicker = false);
    HRESULT OnlineWordToPdfFromBinary(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, const bool& bIsUsePicker = false);
	HRESULT DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH);

    NSFonts::IApplicationFonts* GetApplicationFonts();

private:

	void OnlineWordToPdfInternal(BYTE* dstArray, LONG lLen, const std::wstring& wsHtmlPlace, std::wstring& wsHypers, int& nCountPages, const std::wstring& wsTempLogo, LONG lReg);
	PdfWriter::CImageDict* LoadImage(Aggplus::CImage* pImage, const BYTE& nAlpha);
	bool DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha);
	bool DrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids = NULL);
	bool PathCommandDrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids = NULL);
	void UpdateFont();
	void UpdateTransform();
	void UpdatePen();
	void UpdateBrush();
	bool IsValid()
	{
            return m_bValid;
	}
	bool IsPageValid()
	{
            if (!IsValid() || !m_pPage)
                return false;

            return true;
	}
	void SetError()
	{
            m_bValid = false;;
	}

private:

	struct TFontInfo
	{
		TFontInfo(const std::wstring& fontName, const bool& bold, const bool& italic, const std::wstring& fontPath, const LONG& faceIndex)
		{
			wsFontName = fontName;
			bBold      = bold;
			bItalic    = italic;
			wsFontPath = fontPath;
			lFaceIndex = faceIndex;
		}

		std::wstring wsFontName;
		bool         bBold;
		bool         bItalic;
		std::wstring wsFontPath;
		LONG         lFaceIndex;
	};
	struct TColor
	{
		TColor()
		{
			lColor = 0;
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}
		TColor(const LONG& _lColor)
		{
			Set(_lColor);
		}

		void Set(const LONG& _lColor)
		{
			lColor = _lColor;

			r = (unsigned char)(lColor & 0xFF);
			g = (unsigned char)((lColor >> 8) & 0xFF);
			b = (unsigned char)((lColor >> 16) & 0xFF);
			a = (unsigned char)((lColor >> 24) & 0xFF);
		}
		void operator=(const LONG& _lColor)
		{
			Set(_lColor);
		}
		void Set(BYTE _r, BYTE _g, BYTE _b, BYTE _a = 255)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;

			lColor = (LONG)(((LONG)r) | ((LONG)g << 8) | ((LONG)b << 16) | ((LONG)a << 24));
		}

		LONG lColor;
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;
	};
	class CPenState
	{
	public:

		CPenState()
		{
			m_pDashPattern = NULL;
			Reset();
		}
		~CPenState()
		{
			if (m_pDashPattern)
				delete[] m_pDashPattern;
		}
		inline LONG   GetColor()
		{
			return m_oColor.lColor;
		}
		inline void   SetColor(const LONG& lColor)
		{
			m_oColor.Set(lColor);
		}
		inline TColor GetTColor()
		{
			return m_oColor;
		}
		inline LONG   GetAlpha()
		{
			return m_nAlpha;
		}
		inline void   SetAlpha(const LONG& lAlpha)
		{
            m_nAlpha = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
		}
		inline double GetSize()
		{
			return m_dSize;
		}
		inline void   SetSize(const double& dSize)
		{
			m_dSize = dSize;
		}
		inline BYTE   GetDashStyle()
		{
			return m_nDashStyle;
		}
		inline void   SetDashStyle(const BYTE& nDashStyle)
		{
			m_nDashStyle = nDashStyle;
		}
		inline BYTE   GetStartCapStyle()
		{
			return m_nStartCapStyle;
		}
		inline void   SetStartCapStyle(const BYTE& nCapStyle)
		{
			m_nStartCapStyle = nCapStyle;
		}
		inline BYTE   GetEndCapStyle()
		{
			return m_nEndCapStyle;
		}
		inline void   SetEndCapStyle(const BYTE& nCapStyle)
		{
			m_nEndCapStyle = nCapStyle;
		}
		inline BYTE   GetJoinStyle()
		{
			return m_nJoinStyle;
		}
		inline void   SetJoinStyle(const BYTE& nJoinStyle)
		{
			m_nJoinStyle = nJoinStyle;
		}
		inline double GetDashOffset()
		{
			return m_dDashOffset;
		}
		inline void   SetDashOffset(const double& dOffset)
		{
			m_dDashOffset = dOffset;
		}
		inline LONG   GetAlign()
		{
			return m_lAlign;
		}
		inline void   SetAlign(const LONG& lAlign)
		{
			m_lAlign = lAlign;
		}
		inline double GetMiter()
		{
			return m_dMiter;
		}
		inline void   SetMiter(const double& dMiter)
		{
			m_dMiter = dMiter;
		}
		inline void   SetDashPattern(const double* pPattern, const LONG& lSize)
		{
			if (m_pDashPattern)
			{
				delete[] m_pDashPattern;
				m_pDashPattern = NULL;
			}

			m_lDashPatternSize = 0;

			if (!pPattern || !lSize)
			{
				m_lDashPatternSize = 0;
				m_pDashPattern     = NULL;
			}
			else
			{
				// Избавляемся от нулей, потому что все pdf-ридеры плохо их воспринимают
				std::vector<double> vPattern;
				for (LONG lIndex = 0; lIndex < lSize; lIndex++)
				{
					if (lIndex > 1 && fabs(pPattern[lIndex]) < 0.001)
					{
						if (0 == lIndex % 2)
						{
							if (fabs(pPattern[lIndex + 1]) < 0.001)
							{
								lIndex++;
							}
							else if (lIndex + 1 < lSize)
							{
								size_t nPatternSize = vPattern.size();
								vPattern.at(nPatternSize - 1) = vPattern.at(nPatternSize - 1) + pPattern[lIndex + 1];
								lIndex++;
							}
						}
						else
						{
							size_t nPatternSize = vPattern.size();
							vPattern.at(nPatternSize - 2) = vPattern.at(nPatternSize - 2) + vPattern.at(nPatternSize - 1);
							vPattern.pop_back();
						}
					}
					else
					{
						vPattern.push_back(pPattern[lIndex]);
					}
				}

				size_t nPatternSize = vPattern.size();
				if (nPatternSize > 0)
				{
					m_pDashPattern = new double[nPatternSize];
					if (m_pDashPattern)
					{
						for (int nIndex = 0; nIndex < nPatternSize; nIndex++)
						{
							m_pDashPattern[nIndex] = vPattern.at(nIndex);
						}
						m_lDashPatternSize = nPatternSize;
					}
				}				
			}
		}
		inline double*GetDashPattern(LONG& lSize)
		{
			lSize = m_lDashPatternSize;
			return m_pDashPattern;
		}

		void Reset()
		{
			if (m_pDashPattern)
				delete[] m_pDashPattern;

			m_oColor.Set(0);
			m_dSize  = 0;
			m_nAlpha = 255;
			m_nStartCapStyle = Aggplus::LineCapRound;
			m_nEndCapStyle   = Aggplus::LineCapRound;
			m_nJoinStyle     = Aggplus::LineJoinRound;

			m_lAlign = 0;
			m_dMiter = 0;

			m_nDashStyle       = Aggplus::DashStyleSolid;
			m_lDashPatternSize = 0;
			m_pDashPattern     = NULL;

            m_dDashOffset = 0;
		}

	private:

		double m_dSize;
		TColor m_oColor;
		BYTE   m_nAlpha;
		BYTE   m_nStartCapStyle;
		BYTE   m_nEndCapStyle;
		BYTE   m_nJoinStyle;

		LONG   m_lAlign;
		double m_dMiter;

		BYTE   m_nDashStyle;
		double m_dDashOffset;
		double*m_pDashPattern;
		LONG   m_lDashPatternSize;

	};
	class CBrushState
	{
	public:
		CBrushState()
		{
			m_pShadingColors      = NULL;
			m_pShadingPoints      = NULL;
			m_lShadingPointsCount = 0;
			Reset();
		}
		~CBrushState()
		{
			if (m_pShadingColors)
				delete[] m_pShadingColors;
			if (m_pShadingPoints)
				delete[] m_pShadingPoints;
		}
		void                Reset();
		inline LONG         GetType()
		{
			return m_lType;
		}
		inline void         SetType(const LONG& lType)
		{
			m_lType = lType;
		}
		inline LONG         GetColor1()
		{
			return m_oColor1.lColor;
		}
		inline TColor       GetTColor1()
		{
			return m_oColor1;
		}
		inline void         SetColor1(const LONG& lColor)
		{
			m_oColor1.Set(lColor);
		}
		inline LONG         GetColor2()
		{
			return m_oColor2.lColor;
		}
		inline TColor       GetTColor2()
		{
			return m_oColor2;
		}
		inline void         SetColor2(const LONG& lColor)
		{
			m_oColor2.Set(lColor);
		}
		inline LONG         GetAlpha1()
		{
			return m_nAlpha1;
		}
		inline void         SetAlpha1(const LONG& lAlpha)
		{
            m_nAlpha1 = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
		}
		inline LONG         GetAlpha2()
		{
			return m_nAlpha2;
		}
		inline void         SetAlpha2(const LONG& lAlpha)
		{
            m_nAlpha2 = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
		}
		inline std::wstring GetTexturePath()
		{
			return m_wsTexturePath;
		}
		inline void         SetTexturePath(const std::wstring& wsPath)
		{
			m_wsTexturePath = wsPath;
		}
		inline LONG         GetTextureMode()
		{
			return m_lTextureMode;
		}
		inline void         SetTextureMode(const LONG& lMode)
		{
			m_lTextureMode = lMode;
		}
		inline BYTE         GetTextureAlpha()
		{
			return m_nTextureAlpha;
		}
		inline void         SetTextureAlpha(const LONG& lAlpha)
		{
            m_nTextureAlpha = (BYTE)(std::max)(0, (std::min)(255, (int)lAlpha));
		}
		inline double       GetLinearAngle()
		{
			return m_dLinearAngle;
		}
		inline void         SetLinearAngle(const double& dAngle)
		{
			m_dLinearAngle = dAngle;
		}
		inline void         SetGradientColors(LONG* pColors, double* pPoints, const LONG& lCount)
		{
			// Мы создаем упорядоченный по возрастанию массив, причем первая и последняя точки должны быть 0 и 1 соответственно.
			if (m_pShadingColors)
			{
				delete[] m_pShadingColors;
				m_pShadingColors = NULL;
			}

			if (m_pShadingPoints)
			{
				delete[] m_pShadingPoints;
				m_pShadingPoints = NULL;
			}

			if (!pColors || !pPoints || !lCount)
				return;

			// Проверим вырожденный случай, когда задана либо 1 точка, либо несколько точек с одинковым значением
			bool bIrregular = false;
			if (1 == lCount)
			{
				bIrregular = true;
			}
			else
			{
				bIrregular = true;
				for (LONG lIndex = 0; lIndex < lCount; lIndex++)
				{
					double dPoint1 = pPoints[lIndex];
					for (LONG lIndex2 = lIndex + 1; lIndex2 < lCount; lIndex2++)
					{
						double dPoint2 = pPoints[lIndex2];
						if (fabs(dPoint2 - dPoint1) > 0.00001)
						{
							bIrregular = false;
							break;
						}
					}

					if (!bIrregular)
						break;
				}
			}

			if (bIrregular)
			{
				if (1 == lCount)
				{
					m_pShadingPoints = new double[2];
					m_pShadingColors = new TColor[2];

					if (!m_pShadingColors || !m_pShadingColors)
						return;

					m_pShadingPoints[0] = 0.0;
					m_pShadingColors[0] = pColors[0];
					m_pShadingPoints[1] = 1.0;
					m_pShadingColors[1] = pColors[0];
					m_lShadingPointsCount = 2;
				}
				else
				{
					if (pPoints[0] < 0)
					{
						m_pShadingPoints = new double[2];
						m_pShadingColors = new TColor[2];

						if (!m_pShadingColors || !m_pShadingColors)
							return;

						m_pShadingPoints[0] = 0.0;
						m_pShadingColors[0] = pColors[lCount - 1];
						m_pShadingPoints[1] = 1.0;
						m_pShadingColors[1] = pColors[lCount - 1];
						m_lShadingPointsCount = 2;
					}
					else if (pPoints[0] > 1)
					{
						m_pShadingPoints = new double[2];
						m_pShadingColors = new TColor[2];

						if (!m_pShadingColors || !m_pShadingColors)
							return;

						m_pShadingPoints[0] = 0.0;
						m_pShadingColors[0] = pColors[0];
						m_pShadingPoints[1] = 1.0;
						m_pShadingColors[1] = pColors[0];
						m_lShadingPointsCount = 2;
					}
					else
					{
						m_pShadingPoints = new double[4];
						m_pShadingColors = new TColor[4];

						if (!m_pShadingColors || !m_pShadingColors)
							return;

						m_pShadingPoints[0] = 0.0;
						m_pShadingColors[0] = pColors[0];
						m_pShadingPoints[1] = pPoints[0];
						m_pShadingColors[1] = pColors[0];
						m_pShadingPoints[2] = pPoints[lCount - 1];
						m_pShadingColors[2] = pColors[lCount - 1];
						m_pShadingPoints[3] = 1.0;
						m_pShadingColors[3] = pColors[lCount - 1];
						m_lShadingPointsCount = 4;
					}
				}
			}
			else
			{
				std::vector<TColorAndPoint> vPoints;
				for (LONG lIndex = 0; lIndex < lCount; lIndex++)
				{
					vPoints.push_back(TColorAndPoint(pColors[lIndex], pPoints[lIndex]));
				}
				std::sort(vPoints.begin(), vPoints.end(), TColorAndPoint::Compare);

				LONG lMinIn = -1, lMaxIn = -1, lMinOut = -1, lMaxOut = -1;
				for (LONG lIndex = 0; lIndex < lCount; lIndex++)
				{
					double dPoint = vPoints.at(lIndex).dPoint;
					if (0 <= dPoint && dPoint <= 1)
					{
						if (-1 == lMinIn || dPoint < vPoints.at(lMinIn).dPoint)
							lMinIn = lIndex;

						if (-1 == lMaxIn || dPoint > vPoints.at(lMaxIn).dPoint)
							lMaxIn = lIndex;
					}
					else if (dPoint < 0)
					{
						if (-1 == lMinOut || dPoint > vPoints.at(lMinOut).dPoint)
							lMinOut = lIndex;
					}
					else// if (dPoint > 1)
					{
						if (-1 == lMaxOut || dPoint < vPoints.at(lMaxOut).dPoint)
							lMaxOut = lIndex;
					}
				}

				LONG lBeginIndex = lMinIn;
				LONG lEndIndex   = lMaxIn;

				bool bNeed0 = true, bNeed1 = true;
				if (-1 != lMinIn && vPoints.at(lMinIn).dPoint < 0.001)
				{
					bNeed0 = false;
					lBeginIndex = lMinIn;
					vPoints.at(lMinIn).dPoint = 0;
				}
				else if (-1 != lMinOut && vPoints.at(lMinOut).dPoint > -0.001)
				{
					bNeed0 = false;
					lBeginIndex = lMinOut;
					vPoints.at(lMinOut).dPoint = 0;
				}

				if (-1 != lMaxIn && vPoints.at(lMaxIn).dPoint > 0.999)
				{
					bNeed1 = false;
					lEndIndex = lMaxIn;
					vPoints.at(lEndIndex).dPoint = 1;
				}
				else if (-1 != lMaxOut && vPoints.at(lMaxOut).dPoint < 1.001)
				{
					bNeed1 = false;
					lEndIndex = lMaxOut;
					vPoints.at(lEndIndex).dPoint = 1;
				}

				std::vector<TColorAndPoint> vResPoints;
				if (bNeed0)
				{
					LONG lIndex0, lIndex1;
					if (-1 != lMinOut)
					{
						if (-1 != lMinIn)
						{
							lIndex0 = lMinOut;
							lIndex1 = lMinIn;
						}
						else if (-1 != lMaxOut)
						{
							lIndex0 = lMinOut;
							lIndex1 = lMaxOut;
						}
						else
						{
							lIndex0 = lMinIn - 1;
							lIndex1 = lMinIn;
						}
					}
					else
					{
						if (-1 != lMinIn)
						{
							lIndex0 = lMinIn;
							lIndex1 = lMinIn + 1;
						}
						else
						{
							lIndex0 = lMaxOut;
							lIndex1 = lMaxOut + 1;
						}
					}

					LONG lColor0 = TColorAndPoint::GetLinearApprox(vPoints.at(lIndex0), vPoints.at(lIndex1), 0);
					vResPoints.push_back(TColorAndPoint(lColor0, 0));
				}

				if (-1 != lBeginIndex && -1 != lEndIndex)
				{
					for (LONG lIndex = lBeginIndex; lIndex <= lEndIndex; lIndex++)
					{
						vResPoints.push_back(vPoints.at(lIndex));
					}
				}

				if (bNeed1)
				{
					LONG lIndex0, lIndex1;
					if (-1 != lMaxOut)
					{
						if (-1 != lMaxIn)
						{
							lIndex0 = lMaxIn;
							lIndex1 = lMaxOut;
						}
						else if (-1 != lMinOut)
						{
							lIndex0 = lMinOut;
							lIndex1 = lMaxOut;
						}
						else
						{
							lIndex0 = lMaxOut;
							lIndex1 = lMaxOut + 1;
						}
					}
					else
					{
						if (-1 != lMaxIn)
						{
							lIndex0 = lMaxIn - 1;
							lIndex1 = lMaxIn;
						}
						else
						{
							lIndex0 = lMinOut - 1;
							lIndex1 = lMinOut;
						}
					}

					LONG lColor1 = TColorAndPoint::GetLinearApprox(vPoints.at(lIndex0), vPoints.at(lIndex1), 1);
					vResPoints.push_back(TColorAndPoint(lColor1, 1));
				}

				size_t lResCount = vResPoints.size();
				if (lResCount == 0)
					return;

				m_pShadingColors      = new TColor[lResCount];
				m_pShadingPoints      = new double[lResCount];
				m_lShadingPointsCount = lResCount;

				if (!m_pShadingColors || !m_pShadingPoints)
					return;

				for (LONG lIndex = 0; lIndex < lResCount; lIndex++)
				{
					m_pShadingColors[lIndex] = vResPoints.at(lIndex).lColor;
					m_pShadingPoints[lIndex] = vResPoints.at(lIndex).dPoint;
				}
			}
		}
		inline void         SetBrushRect(const int& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
		{
			m_oRect.nVal    = nVal;
			m_oRect.dLeft   = dLeft;
			m_oRect.dTop    = dTop;
			m_oRect.dWidth  = dWidth;
			m_oRect.dHeight = dHeight;
		}
		inline void         EnableBrushRect(bool bEnable)
		{
			m_oRect.bUse = bEnable;
		}
		inline void         SetLinearGradientPattern(const double& dX0, const double& dY0, const double& dX1, const double& dY1)
		{
			m_pShadingPattern[0] = dX0;
			m_pShadingPattern[1] = dY0;
			m_pShadingPattern[2] = dX1;
			m_pShadingPattern[3] = dY1;
		}
		inline void         SetRadialGradientPattern(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
		{
			m_pShadingPattern[0] = dX0;
			m_pShadingPattern[1] = dY0;
			m_pShadingPattern[2] = dR0;
			m_pShadingPattern[3] = dX1;
			m_pShadingPattern[4] = dY1;
			m_pShadingPattern[5] = dR1;
		}
		inline void         GetLinearGradientPattern(double& dX0, double& dY0, double& dX1, double& dY1)
		{
			dX0 = m_pShadingPattern[0];
			dY0 = m_pShadingPattern[1];
			dX1 = m_pShadingPattern[2];
			dY1 = m_pShadingPattern[3];
		}
		inline void         GetRadialGradientPattern(double& dX0, double& dY0, double& dR0, double& dX1, double& dY1, double& dR1)
		{
			dX0 = m_pShadingPattern[0];
			dY0 = m_pShadingPattern[1];
			dR0 = m_pShadingPattern[2];
			dX1 = m_pShadingPattern[3];
			dY1 = m_pShadingPattern[4];
			dR1 = m_pShadingPattern[5];
		}
		inline void         GetGradientColors(TColor*& pColors, double*& pPoints, LONG& lCount)
		{
			pColors = m_pShadingColors;
			pPoints = m_pShadingPoints;
			lCount  = m_lShadingPointsCount;
		}

	private:

		struct TColorAndPoint
		{
			TColorAndPoint()
			{
				lColor = 0;
				dPoint = 0;
				bUse   = false;
			}
			TColorAndPoint(const LONG& color, const double& point)
			{
				lColor = color;
				dPoint = point;
				bUse   = true;
			}

			static bool Compare(const TColorAndPoint& oFirst, const TColorAndPoint& oSecond)
			{
				return (oFirst.dPoint < oSecond.dPoint);
			}
			static LONG GetLinearApprox(const TColorAndPoint& oPoint1, const TColorAndPoint& oPoint2, const double& dDstPoint)
			{
				double dPoint1 = oPoint1.dPoint;
				double dPoint2 = oPoint2.dPoint;
				LONG lColor1 = oPoint1.lColor;
				LONG lColor2 = oPoint2.lColor;

				double dDiff = dPoint2 - dPoint1;
				if (fabs(dDiff) < 0)
					return lColor1;

				TColor oColor1 = lColor1;
				TColor oColor2 = lColor2;

                BYTE r = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.r + (oColor2.r - oColor1.r) / dDiff * (dDstPoint - dPoint1))));
                BYTE g = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.g + (oColor2.g - oColor1.g) / dDiff * (dDstPoint - dPoint1))));
                BYTE b = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.b + (oColor2.b - oColor1.b) / dDiff * (dDstPoint - dPoint1))));
                BYTE a = (BYTE)(std::max)(0, (std::min)(255, (int)(oColor1.a + (oColor2.a - oColor1.a) / dDiff * (dDstPoint - dPoint1))));

				TColor oResColor;
				oResColor.Set(r, g, b, a);
				return oResColor.lColor;
			}

			LONG   lColor;
			double dPoint;
			bool   bUse;
		};
		struct TBrushRect
		{
			TBrushRect()
			{
				Reset();
			}

			void Reset()
			{
				bUse    = false;
				nVal    = 0;
				dLeft   = 0;
				dTop    = 0;
				dWidth  = 0;
				dHeight = 0;
			}

			bool   bUse;
			int    nVal;
			double dLeft;
			double dTop;
			double dWidth;
			double dHeight; 
		};

	private:

		LONG         m_lType;			         
		TColor       m_oColor1;
		TColor       m_oColor2;
		BYTE         m_nAlpha1;
		BYTE         m_nAlpha2;
		std::wstring m_wsTexturePath;
		LONG         m_lTextureMode;
		BYTE         m_nTextureAlpha;
		double       m_dLinearAngle;
		TBrushRect   m_oRect;

		TColor*      m_pShadingColors;
		double*      m_pShadingPoints;
		LONG         m_lShadingPointsCount;
		double       m_pShadingPattern[6]; // У линейного градиента x0, y0, x1, y1 (2 не используются), у радиального x0, y0, r0, x1, y1, r1
	};
	class CFontState
	{
	public:

		CFontState() : m_wsPath(L""), m_wsName(L"Arial"), m_lStyle(0), m_bBold(false), m_bItalic(false), m_dCharSpace(0), 
			m_lFaceIndex(0), m_dSize(10), m_bGid(false), m_bNeedDoBold(false), m_bNeedDoItalic(false)
		{
		}

		void Reset()
		{
			m_wsPath     = L"";
			m_wsName     = L"Arial";
			m_lStyle     = 0;
			m_bBold      = false;
			m_bItalic    = false;
			m_dCharSpace = 0;
			m_lFaceIndex = 0;
			m_dSize      = 10;
			m_bGid       = false;

			m_bNeedDoBold   = false;
			m_bNeedDoItalic = false;
		}

		inline std::wstring GetName()
		{
			return m_wsName;
		}
		inline void         SetName(const std::wstring& wsName)
		{
			m_wsName = wsName;
		}
		inline std::wstring GetPath()
		{
			return m_wsPath;
		}
		inline void         SetPath(const std::wstring& wsPath)
		{
			m_wsPath = wsPath;
		}
		inline double       GetSize()
		{
			return m_dSize;
		}
		inline void         SetSize(const double& dSize)
		{
			m_dSize = dSize;
		}
		inline LONG         GetFaceIndex()
		{
			return m_lFaceIndex;
		}
		inline void         SetFaceIndex(const LONG& lFaceIndex)
		{
			m_lFaceIndex = lFaceIndex;
		}
		inline LONG         GetStyle()
		{
			return m_lStyle;
		}
		inline void         SetStyle(const LONG& lStyle)
		{
			m_lStyle  = lStyle;
			m_bBold   = (lStyle & 1 ? true : false);
			m_bItalic = (lStyle & 2 ? true : false);
		}
		inline bool         GetGid()
		{
			return m_bGid;
		}
		inline void         SetGid(const bool& bGid)
		{
			m_bGid = bGid;
		}
		inline double       GetCharSpace()
		{
			return m_dCharSpace;
		}
		inline void         SetCharSpace(const double& dCharSpace)
		{
			m_dCharSpace = dCharSpace;
		}
		inline bool         IsBold()
		{
			return m_bBold;
		}
		inline bool         IsItalic()
		{
			return m_bItalic;
		}
		inline void         SetNeedDoItalic(const bool& bNeedDoItalic)
		{
			m_bNeedDoItalic = bNeedDoItalic;
		}
		inline void         SetNeedDoBold(const bool& bNeedDoBold)
		{
			m_bNeedDoBold = bNeedDoBold;
		}
		inline bool         IsNeedDoItalic()
		{
			return m_bNeedDoItalic;
		}
		inline bool         IsNeedDoBold()
		{
			return m_bNeedDoBold;
		}

	private:

		std::wstring m_wsName;
		std::wstring m_wsPath;
		double       m_dSize;
		bool         m_bGid;
		LONG         m_lFaceIndex;
		LONG         m_lStyle;
		bool         m_bBold;
		bool         m_bItalic;
		double       m_dCharSpace;
		bool         m_bNeedDoItalic;
		bool         m_bNeedDoBold;
	};
	class CPath
	{
	private:

		enum EPathCommandType
		{
			rendererpathcommand_Unknown    = 0x00,
			rendererpathcommand_MoveTo     = 0x01,
			rendererpathcommand_LineTo     = 0x02,
			rendererpathcommand_CurveTo    = 0x03,
			rendererpathcommand_ArcTo      = 0x04,
			rendererpathcommand_Close      = 0x05,
			rendererpathcommand_TextChar   = 0x06,
			rendererpathcommand_Text       = 0x07,
			rendererpathcommand_TextExChar = 0x08,
			rendererpathcommand_TextEx     = 0x09
		};
		class CPathCommandBase
		{
		public:
			CPathCommandBase()
			{
			}
			virtual ~CPathCommandBase()
			{
			}
			virtual void Draw(PdfWriter::CPage* pPage) = 0;
			virtual void UpdateBounds(double& dL, double& dT, double& dR, double& dB) = 0;
			virtual void GetLastPoint(double& dX, double& dY) = 0;
			virtual EPathCommandType GetType() = 0;
		};
		class CPathMoveTo : public CPathCommandBase
		{
		public:
			CPathMoveTo(const double& dX, const double& dY)
			{
				x = dX;
				y = dY;
			}
			void GetLastPoint(double& dX, double& dY)
			{
				dX = x;
				dY = y;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_MoveTo;
			}

		public:

			double x;
			double y;
		};
		class CPathLineTo : public CPathCommandBase
		{
		public:
			CPathLineTo(const double& dX, const double& dY)
			{
				x = dX;
				y = dY;
			}
			void GetLastPoint(double& dX, double& dY)
			{
				dX = x;
				dY = y;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_LineTo;
			}

		public:

			double x;
			double y;
		};
		class CPathCurveTo : public CPathCommandBase
		{
		public:
			CPathCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
			{
				x1 = dX1;
				y1 = dY1;
				x2 = dX2;
				y2 = dY2;
				xe = dXe;
				ye = dYe;
			}
			void GetLastPoint(double& dX, double& dY)
			{
				dX = xe;
				dY = ye;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_CurveTo;
			}

		public:

			double x1;
			double y1;
			double x2;
			double y2;
			double xe;
			double ye;
		};
		class CPathArcTo : public CPathCommandBase
		{
		public:
			CPathArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
			{
				x = dX;
				y = dY;
				w = dW;
				h = dH;
				startAngle = dStartAngle;
				sweepAngle = dSweepAngle;
			}
			void GetLastPoint(double& dX, double& dY)
			{
				// TODO: Надо грамотно пересчитать
				dX = x;
				dY = y;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_ArcTo;
			}

		public:

			double x;
			double y;
			double w;
			double h;
			double startAngle;
			double sweepAngle;
		};
		class CPathClose : public CPathCommandBase
		{
		public:
			CPathClose()
			{
			}
			void GetLastPoint(double& dX, double& dY)
			{
				// TODO: Надо грамотно пересчитать
				dX = 0;
				dY = 0;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_Close;
			}
		};
		class CPathText : public CPathCommandBase
		{
		public:
			CPathText(PdfWriter::CFontDict* pFont, unsigned char* pCodes, const unsigned int& unCodesCount, const double& dX, const double& dY, const double& dSize, const double& dCharSpace)
			{
				font       = pFont;
				codes      = pCodes;
				codesCount = unCodesCount;
				x          = dX;
				y          = dY;
				fontSize   = dSize;
				charSpace  = dCharSpace;
			}
			~CPathText()
			{
				RELEASEARRAYOBJECTS(codes);
			}
			void GetLastPoint(double& dX, double& dY)
			{
				dX = x;
				dY = y;
			}
			void Draw(PdfWriter::CPage* pPage);
			void UpdateBounds(double& dL, double& dT, double& dR, double& dB);
			EPathCommandType GetType()
			{
				return rendererpathcommand_Text;
			}

		public:

			PdfWriter::CFontDict* font;
			unsigned char*        codes;
			unsigned int          codesCount;
			double                x;
			double                y;
			double                fontSize;
			double                charSpace;
		};

	public:

		CPath()
		{
			m_bIsMoveTo = false;
		}
		~CPath()
		{
			Clear();
		}

		bool MoveTo(const double& dX, const double& dY)
		{
			m_bIsMoveTo = true;
			return Add(new CPathMoveTo(dX, dY));
		}
		bool LineTo(const double& dX, const double& dY)
		{
			if (!m_bIsMoveTo)
				MoveTo(dX, dY);

			return Add(new CPathLineTo(dX, dY));
		}
		bool CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
		{
			if (!m_bIsMoveTo)
				MoveTo(dX1, dY1);

			return Add(new CPathCurveTo(dX1, dY1, dX2, dY2, dXE, dYE));
		}
		bool ArcTo(double dX, double dY, double dW, double dH, double dStart, double dSweep)
		{
			if (!m_bIsMoveTo)
				MoveTo(dX, dY);

			return Add(new CPathArcTo(dX, dY, dW, dH, dStart, dSweep));
		}
		bool AddText(PdfWriter::CFontDict* pFont, unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY, const double& dSize, const double& dCharSpace)
		{
			return Add(new CPathText(pFont, pCodes, unLen, dX, dY, dSize, dCharSpace));
		}
		bool Close()
		{
			return Add(new CPathClose());
		}
		void Clear()
		{
			for (size_t nIndex = 0, nCount = m_vCommands.size(); nIndex < nCount; nIndex++)
			{
				CPathCommandBase* pCommand = m_vCommands.at(nIndex);
				delete pCommand;
			}
			m_vCommands.clear();
			m_bIsMoveTo = false;
		}
		bool IsMoveTo()
		{
			return m_bIsMoveTo;
		}
		void GetLastPoint(double& dX, double& dY);
		void Draw(PdfWriter::CPage* pPage, bool bStroke, bool bFill, bool bEoFill);
		void Clip(PdfWriter::CPage* pPage, bool bEvenOdd = false);
		void GetBounds(double& dL, double& dT, double& dR, double& dB);

	private:

		bool Add(CPathCommandBase* pCommand)
		{
			if (pCommand)
			{
				m_vCommands.push_back(pCommand);
				return true;
			}

			return false;
		}


	public:

		std::vector<CPathCommandBase*> m_vCommands;
		bool                           m_bIsMoveTo;
	};
	class CTransform
	{
	public:

		CTransform()
		{
			Reset();
		}
		void operator=(const CTransform& oT)
		{
			m11 = oT.m11;
			m12 = oT.m12;
			m21 = oT.m21;
			m22 = oT.m22;
			dx  = oT.dx;
			dy  = oT.dy;
		}
		void Reset()
		{
			m11 = 1.0;
			m12 = 0.0;
			m21 = 0.0;
			m22 = 1.0;
			dx  = 0;
			dy  = 0;
		}
		bool IsIdentity() const
		{
			if (fabs(m11 - 1) < 0.001
				&& fabs(m12) < 0.001
				&& fabs(m21) < 0.001
				&& fabs(m22 - 1) < 0.001
				&& fabs(dx) < 0.001
				&& fabs(dy) < 0.001)
				return true;

			return false;
		}
		void Set(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
		{
			m11 = dM11;
			m12 = dM12;
			m21 = dM21;
			m22 = dM22;
			dx  = dX;
			dy  = dY;
		}

	public:

		double m11;
		double m12;
		double m21;
		double m22;
		double dx;
		double dy;
	};
    class PDFWRITER_DECL_EXPORT CCommandManager
	{
	public:
		CCommandManager(CPdfRenderer* pRenderer);
		~CCommandManager();
		CRendererTextCommand* AddText(unsigned char* pCodes, unsigned int nLen, const double& dX, const double& dY);
		void Flush();
		void SetTransform(const CTransform& oTransform);
		void SetTransform(const double& m11, const double& m12, const double& m21, const double& m22, const double& dx, const double& dy);
	private:
		void Add(CRendererCommandBase* pCommand);		
		void Clear();
	private:
		CPdfRenderer*                      m_pRenderer;
		std::vector<CRendererCommandBase*> m_vCommands;
		CTransform                         m_oTransform;
	};
	struct TImageInfo
	{
		std::wstring           wsPath;
		BYTE                   nAlpha;
		int                    nWidth;
		int                    nHeight;
		PdfWriter::CImageDict* pImage;
	};

private:

	NSFonts::IApplicationFonts*  m_pAppFonts;
	NSFonts::IFontManager*       m_pFontManager;
	std::wstring                 m_wsTempFolder;
	std::wstring                 m_wsThemesPlace;
								 
	PdfWriter::CDocument*        m_pDocument;
	PdfWriter::CPage*            m_pPage;
	PdfWriter::CFontCidTrueType* m_pFont;
	PdfWriter::CShading*         m_pShading;
	PdfWriter::CExtGrState*      m_pShadingExtGrState;
								 
	bool                         m_bNeedUpdateTextFont;
	bool                         m_bNeedUpdateTextColor;
	bool                         m_bNeedUpdateTextAlpha;
	bool                         m_bNeedUpdateTextCharSpace;
	bool                         m_bNeedUpdateTextSize;

	CCommandManager              m_oCommandManager;

	CPenState                    m_oPen;
	CBrushState                  m_oBrush;
	CFontState                   m_oFont;
	CPath                        m_oPath;
	CTransform                   m_oTransform;
	LONG                         m_lClipMode;
	double                       m_dPageHeight;
	double                       m_dPageWidth;
	LONG                         m_lClipDepth;
	std::vector<TFontInfo>       m_vFonts;
								 
	bool                         m_bValid;
								 
	int                          m_nPagesCount;
	int                          m_nCounter; // TODO: для теста, убрать потом
};

#endif // _PDF_WRITER_PDFRENDERER_H
