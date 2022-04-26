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
#include "Utils.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/common/Types.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/graphics/IRenderer.h"
#include <math.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

#define IsNumber(X) \
		('0' == (X)\
	  || '1' == (X)\
      || '2' == (X)\
      || '3' == (X)\
      || '4' == (X)\
      || '5' == (X)\
      || '6' == (X)\
      || '7' == (X)\
      || '8' == (X)\
      || '9' == (X)\
      || '-' == (X)\
	  || '.' == (X)\
      || 'e' == (X)\
	  || 'E' == (X))

#define GetChar(STRING, POS) STRING[POS++] 
#define LookChar(STRING, POS) STRING[POS] 

#ifndef SQR
#define SQR(X) ((X)*(X))
#endif

#ifndef SQRT
#define SQRT(X) (sqrt(X))
#endif

namespace XPSEllipse
{
	double AngToEllPrm     (double fAngle, double fXRad, double fYRad)
	{
		// Функция для перевода реального угла в параметрическое задание эллписа
		// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
		// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
		return atan2(sin(fAngle) / fYRad, cos(fAngle) / fXRad);
	}
	void   CurveTo         (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double dCX1, double dCY1, double dCX2, double dCY2, double dX2, double dY2)
	{
		oTransform.TransformPoint(dCX1, dCY1);
		oTransform.TransformPoint(dCX2, dCY2);
		oTransform.TransformPoint(dX2, dY2);
		pRenderer->PathCommandCurveTo(xpsUnitToMM(dCX1), xpsUnitToMM(dCY1), xpsUnitToMM(dCX2), xpsUnitToMM(dCY2), xpsUnitToMM(dX2), xpsUnitToMM(dY2));
	}
	void   LineTo          (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double dX, double dY)
	{
		oTransform.TransformPoint(dX, dY);
		pRenderer->PathCommandLineTo(xpsUnitToMM(dX), xpsUnitToMM(dY));
	}
	void   MoveTo          (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double dX, double dY)
	{
		oTransform.TransformPoint(dX, dY);
		pRenderer->PathCommandMoveTo(xpsUnitToMM(dX), xpsUnitToMM(dY));
	}
	void   EllipseArc3     (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, bool bClockDirection)
	{
		// Рассчитаем начальную, конечную и контрольные точки
		double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
		double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

		double fAlpha = sin(dAngle2 - dAngle1) * (sqrt(4.0 + 3.0 * tan((dAngle2 - dAngle1) / 2.0) * tan((dAngle2 - dAngle1) / 2.0)) - 1.0) / 3.0;

		double fKoef = 1;

		fX1 = fX + fXRad * cos(dAngle1);
		fY1 = fY + fYRad * sin(dAngle1);

		fX2 = fX + fXRad * cos(dAngle2);
		fY2 = fY + fYRad * sin(dAngle2);

		fCX1 = fX1 - fAlpha * fXRad * sin(dAngle1);
		fCY1 = fY1 + fAlpha * fYRad * cos(dAngle1);

		fCX2 = fX2 + fAlpha * fXRad * sin(dAngle2);
		fCY2 = fY2 - fAlpha * fYRad * cos(dAngle2);

		if (!bClockDirection)
		{
			CurveTo(pRenderer, oTransform, fCX1, fCY1, fCX2, fCY2, fX2, fY2);

			*pfXCur = fX2;
			*pfYCur = fY2;
		}
		else
		{
			CurveTo(pRenderer, oTransform, fCX2, fCY2, fCX1, fCY1, fX1, fY1);

			*pfXCur = fX1;
			*pfYCur = fY1;
		}
	}
	void   EllipseArc2     (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, bool bClockDirection)
	{
		// переведем углы в радианы
		double dAngle1 = fAngle1 * 3.141592 / 180;
		double dAngle2 = fAngle2 * 3.141592 / 180;

		// Выясним в каких четвертях находятся начальная и конечная точки
		unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1;
		unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
        nSecondPointQuard = std::min((unsigned int)4, std::max((unsigned int)1, nSecondPointQuard));
        nFirstPointQuard  = std::min((unsigned int)4, std::max((unsigned int)1, nFirstPointQuard));
		// Проведем линию в начальную точку дуги
		double fStartX = 0.0, fStartY = 0.0, fEndX = 0.0, fEndY = 0.0;

		fStartX = fX + fXRad * cos(AngToEllPrm(dAngle1, fXRad, fYRad));
		fStartY = fY + fYRad * sin(AngToEllPrm(dAngle1, fXRad, fYRad));

		LineTo(pRenderer, oTransform, fStartX, fStartY);

		// Дальше рисуем по четверям

		double fCurX = fStartX, fCurY = fStartY;
		double dStartAngle = dAngle1;
		double dEndAngle = 0;

		if (!bClockDirection)
		{
			for (unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++)
			{
				if (nIndex == nSecondPointQuard)
					dEndAngle = dAngle2;
				else
					dEndAngle = (90 * (nIndex)) * 3.141592f / 180;
				if (!(nIndex == nFirstPointQuard))
					dStartAngle = (90 * (nIndex - 1)) * 3.141592f / 180;

				EllipseArc3(pRenderer, oTransform, fX, fY, fXRad, fYRad, AngToEllPrm(dStartAngle, fXRad, fYRad), AngToEllPrm(dEndAngle, fXRad, fYRad), &fEndX, &fEndY, false);
			}
		}
		else
		{
			for (unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex--)
			{
				if (nIndex == nFirstPointQuard)
					dStartAngle = dAngle1;
				else
					dStartAngle = (90 * (nIndex)) * 3.141592f / 180;
				if (!(nIndex == nSecondPointQuard))
					dEndAngle = (90 * (nIndex - 1)) * 3.141592f / 180;
				else
					dEndAngle = dAngle2;

				EllipseArc3(pRenderer, oTransform, fX, fY, fXRad, fYRad, AngToEllPrm(dStartAngle, fXRad, fYRad), AngToEllPrm(dEndAngle, fXRad, fYRad), &fEndX, &fEndY, false);
			}
		}
	}
	void   EllipseArc      (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, bool bClockDirection)
	{
		while (fAngle1 < 0)
			fAngle1 += 360;

		while (fAngle1 > 360)
			fAngle1 -= 360;

		while (fAngle2 < 0)
			fAngle2 += 360;

		while (fAngle2 >= 360)
			fAngle2 -= 360;

		if (!bClockDirection)
		{
			if (fAngle1 <= fAngle2)
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, fAngle1, fAngle2, false);
			else
			{
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, fAngle1, 360, false);
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, 0, fAngle2, false);
			}
		}
		else
		{
			if (fAngle1 >= fAngle2)
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, fAngle1, fAngle2, true);
			else
			{
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, fAngle1, 0, true);
				EllipseArc2(pRenderer, oTransform, fX, fY, fXRad, fYRad, 360, fAngle2, true);
			}
		}
	}
	void   Ellipse         (IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double fX, double fY, double fXRad, double fYRad)
	{
		MoveTo(pRenderer, oTransform, fX - fXRad, fY);

		double c_fKappa = 0.552;
		CurveTo(pRenderer, oTransform, fX - fXRad, fY + fYRad * c_fKappa, fX - fXRad * c_fKappa, fY + fYRad, fX, fY + fYRad);
		CurveTo(pRenderer, oTransform, fX + fXRad * c_fKappa, fY + fYRad, fX + fXRad, fY + fYRad * c_fKappa, fX + fXRad, fY);
		CurveTo(pRenderer, oTransform, fX + fXRad, fY - fYRad * c_fKappa, fX + fXRad * c_fKappa, fY - fYRad, fX, fY - fYRad);
		CurveTo(pRenderer, oTransform, fX - fXRad * c_fKappa, fY - fYRad, fX - fXRad, fY - fYRad * c_fKappa, fX - fXRad, fY);
	}
	void   AppendEllipseArc(IRenderer* pRenderer, Aggplus::CMatrix& oTransform, double fX, double fY, double fXRad, double fYRad, double fStartAngle, double fEndAngle, bool bClockDirection)
	{
		if (fXRad <= 0 || fYRad <= 0)
			return;

		if (fabs(fEndAngle - fStartAngle) >= 360) // Целый эллипс
		{
			Ellipse(pRenderer, oTransform, fX, fY, fXRad, fYRad);
		}
		else // Дуга эллипса
		{
			EllipseArc(pRenderer, oTransform, fX, fY, fXRad, fYRad, fStartAngle, fEndAngle, bClockDirection);
		}
	}
	inline void GetEllipseKoefs        (const double& dX1, const double& dY1, const double& dRadX, const double& dRadY, double& dA1, double& dB1, double& dA2, double& dB2, double& dC2, double& dRoot)
	{
		// Y = A1 * X + B1 
		dA1 = -dX1 / dY1 * SQR(dRadY) / SQR(dRadX);
		dB1 = (SQR(dX1) / SQR(dRadX) + SQR(dY1) / SQR(dRadY)) * SQR(dRadY) / (2 * dY1);

		// Получаем квадратное уравнение A2 * X^2 + B2 * X + C2 = 0 (причем A2 != 0 в нашей ситуации)
		dA2 = SQR(dA1) / SQR(dRadY) + 1 / SQR(dRadX);
		dB2 = 2 * dA1 * dB1 / SQR(dRadY);
		dC2 = SQR(dB1) / SQR(dRadY) - 1;

		dRoot = (SQR(dB2) - 4 * dA2 * dC2);
	}
	inline bool GetEllipsesCommonCase  (const double& dX1, const double& dY1, double& dRadX, double& dRadY, double& dCx1, double& dCy1, double& dCx2, double& dCy2)
	{
		double dA1, dB1, dA2, dB2, dC2, dRoot;
		GetEllipseKoefs(dX1, dY1, dRadX, dRadY, dA1, dB1, dA2, dB2, dC2, dRoot);
		if (dRoot < 0)
		{
			double dK1 = (SQR(dA1) + SQR(dRadY) / SQR(dRadX)); // dK1 всегда положительное (поэтому спокойно на него делим)
			double dK2 = 2 * dA1 * dB1;
			double dK3 = SQR(dB1);
			double dK4 = dK3 - SQR(dK2) / (4 * dK1);
			if (dK4 < 0)
				return false; // Такого быть не должно

			double dNewRadY = SQRT(dK4);
			double dKoef = dNewRadY / dRadY;
			if (dKoef > 1)
			{
				dRadX *= dKoef;
				dRadY *= dKoef;
				GetEllipseKoefs(dX1, dY1, dRadX, dRadY, dA1, dB1, dA2, dB2, dC2, dRoot);
			}
		}

		if (dRoot < -0.001)
		{			
			// Такого быть не должно
			return false;
		}
		else if (dRoot < 0)
		{
			dCx1 = -dB2 / (2 * dA2);
			dCx2 = dCx1;
		}
		else
		{
			dCx1 = (-dB2 + SQRT(dRoot)) / (2 * dA2);
			dCx2 = (-dB2 - SQRT(dRoot)) / (2 * dA2);
		}

		dCy1 = dA1 * dCx1 + dB1;
		dCy2 = dA1 * dCx2 + dB1;
		return true;
	}
	inline bool GetEllipsesVerticalCase(const double& dX1, double& dRadX, double& dRadY, double& dCx1, double& dCy1, double& dCx2, double& dCy2)
	{
		if (abs(dX1) < 0.001)
			return false;

		// Центры искомых эллипсов лежат на вертикальной прямой			
		dCx1 = dX1 / 2;
		dCx2 = dCx1;

		double dRoot = SQR(dRadY) - SQR(dRadY) / SQR(dRadX) * SQR(dCx1);
		if (dRoot < 0)
		{
			double dNewRadX = dX1 / 2;
			double dKoef = dNewRadX / dRadX;
			if (dKoef > 1)
			{
				dRadX *= dKoef;
				dRadY *= dKoef;
				dRoot = SQR(dRadY) - SQR(dRadY) / SQR(dRadX) * SQR(dCx1);
			}
		}
		
		if (dRoot < -0.001)
		{
			// Такого быть не должно
			return false;
		}
		else if (dRoot < 0)
		{
			dCy1 = 0;
			dCy2 = 0;
		}
		else
		{
			dCy1 = SQRT(dRoot);
			dCy2 = -dCy1;
		}
		return true;
	}
	inline bool GetEllipses            (const double& dX1, const double& dY1, double& dRadX, double& dRadY, double& dCx1, double& dCy1, double& dCx2, double& dCy2)
	{
		if (abs(dY1) > 0.001)
			return GetEllipsesCommonCase(dX1, dY1, dRadX, dRadY, dCx1, dCy1, dCx2, dCy2);
		else
			return GetEllipsesVerticalCase(dX1, dRadX, dRadY, dCx1, dCy1, dCx2, dCy2);
	}
}

namespace XPS
{

	static inline double GetEllipseAngle(const double& dCx, const double& dCy, const double& dRadX, const double dRadY, const double& dX, const double& dY)
	{
		// Определим квадрант
		int nQuarter = -1;
		if (dX >= dCx)
		{
			if (dY <= dCy)
				nQuarter = 3;
			else
				nQuarter = 0;
		}
		else
		{
			if (dY <= dCy)
				nQuarter = 2;
			else
				nQuarter = 1;
		}

		double dDist = SQRT(SQR(dX - dCx) + SQR(dY - dCy));
		double dRadAngle = asin(abs(dY - dCy) / dDist);

		double dAngle = dRadAngle * 180 / 3.14159265358979323846;
		switch (nQuarter)
		{
			case 1: dAngle = 180 - dAngle; break;
			case 2: dAngle = 180 + dAngle; break;
			case 3: dAngle = 360 - dAngle; break;
		}

		return dAngle;
	}
	static inline bool   SkipWhiteSpaces(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		while (1)
		{
			if (nPos >= nLen)
				return false;
			wchar_t wChar = LookChar(wsString, nPos);
			if (' ' == wChar)
			{
				nPos++;
				continue;
			}
			else
				break;
		}

		return true;
	}
	static inline bool   SkipWhiteSpacesAndCommas(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		while (1)
		{
			if (nPos >= nLen)
				return false;
			wchar_t wChar = LookChar(wsString, nPos);
			if (' ' == wChar || ',' == wChar)
			{
				nPos++;
				continue;
			}
			else
				break;
		}

		return true;
	}
	static inline double GetDouble(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
			return 0.0;

		wchar_t wChar = GetChar(wsString, nPos);
		if (IsNumber(wChar))
		{
			bool bNegative = false;
			int nInt = 0;
			double dFloat = 0.0, dScale = 0.1;

			if ('-' == wChar)
			{
				bNegative = true;
			}
			else if ('.' == wChar)
			{
				goto doReal;
			}
			else
			{
				nInt = wChar - '0';
			}

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (isdigit(wChar))
				{
					nPos++;
					nInt = nInt * 10 + (wChar - '0');
				}
				else if ('.' == wChar)
				{
					nPos++;
					dFloat = (double)nInt;
					goto doReal;
				}
				else if ('e' == wChar || 'E' == wChar)
				{
					nPos++;
					dFloat = (double)nInt;
					goto doExponent;
				}
				else
				{
					break;
				}
			}

			return (bNegative ? (double)(-nInt) : (double)nInt);

		doReal:

			while (1)
			{
				wChar = LookChar(wsString, nPos);

				if ('e' == wChar || 'E' == wChar)
				{
					nPos++;
					goto doExponent;
				}
				else if (!isdigit(wChar))
					break;

				nPos++;
				dFloat = dFloat + dScale * (wChar - '0');
				dScale *= 0.1;
			}

			return (bNegative ? (double)(-dFloat) : (double)dFloat);

		doExponent:

			wChar = GetChar(wsString, nPos);
			bool bNegativeExponent = false;
			int nExp = 0;
			if ('-' == wChar)
			{
				bNegativeExponent = true;
			}
			else if ('+' == wChar)
			{
				bNegativeExponent = false;
			}
			else
			{
				nExp = wChar - '0';
			}

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (!isdigit(wChar))
					break;

				nPos++;
				nExp = nExp * 10 + (wChar - '0');
			}

			dFloat = (bNegative ? (double)(-dFloat) : (double)dFloat);
            nExp = std::max(20, std::min(0, nExp));

			while (nExp)
			{
				if (bNegativeExponent)
					dFloat /= 10;
				else
					dFloat *= 10;

				nExp--;
			}

			return dFloat;			
		}

		return 0.0;
	}
	static inline int    GetInt   (const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpaces(wsString, nPos, nLen))
			return 0;

		wchar_t wChar = GetChar(wsString, nPos);
		if (IsNumber(wChar))
		{
			bool bNegative = false;
			int nInt = 0;

			if ('-' == wChar)
			{
				bNegative = true;
			}
			else
			{
				nInt = wChar - '0';
			}

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (isdigit(wChar))
				{
					nPos++;
					nInt = nInt * 10 + (wChar - '0');
				}
				else
				{
					break;
				}
			}

			return (bNegative ? -nInt : nInt);		
		}

		return 0;
	}
	static inline bool   GetBool(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
			return false;

		int nBufPos = 0;
		wchar_t pBuffer[32];
		memset(pBuffer, 0x00, sizeof(wchar_t) * 32);

		wchar_t wChar = LookChar(wsString, nPos);
		while (' ' != wChar)
		{
			pBuffer[nBufPos++] = wChar;
			nPos++;

			if (nPos >= nLen)
				break;

			wChar = LookChar(wsString, nPos);
		}

		CWString wsBool(pBuffer, false, nBufPos);
		if ((wsBool == L"true") || (wsBool == L"t") || (wsBool == L"1") || (wsBool == L"on"))
			return true;

		return false;
	}
	static inline void   AppendArcTo(const wchar_t* wsString, int& nPos, const int& nLen, double& dCurX, double& dCurY, IRenderer* pRenderer,bool bRelative)
	{
		double dRadX  = GetDouble(wsString, nPos, nLen);
		double dRadY  = GetDouble(wsString, nPos, nLen);
		double dAngle = GetDouble(wsString, nPos, nLen);

		bool bIsLarge   = !GetBool(wsString, nPos, nLen);
		bool bClockwise = !GetBool(wsString, nPos, nLen);

		double dX0 = dCurX;
		double dY0 = dCurY;
		double dX1 = GetDouble(wsString, nPos, nLen);
		double dY1 = GetDouble(wsString, nPos, nLen);

		if (bRelative)
		{
			dX1 += dCurX;
			dY1 += dCurY;
		}

		double _dX1 = dX1;
		double _dY1 = dY1;
		if (abs(dRadX) < 0.001 || abs(dRadY) < 0.001)
		{
			pRenderer->PathCommandLineTo(_dX1, _dY1);
			dCurX = dX1;
			dCurY = dY1;
			return;
		}

		// Точку X0, Y0 переносим в начало координат и поворачиваем на обратный угол, чтобы искомые эллипсы встали правильно
		Aggplus::CMatrix oTransform, oInverse;
		oTransform.Rotate(-dAngle);
		oTransform.Translate(-dX0, -dY0);
		oInverse.Translate(dX0, dY0);
		oInverse.Rotate(dAngle);

		double dTestX = 0, dTestY = 0;
		oTransform.TransformPoint(dX1, dY1);
		oInverse.TransformPoint(dTestX, dTestY);

		double dCx1, dCy1, dCx2, dCy2;
		// Ищем эллипсы, точками пересечения которых являются (0, 0) и (dX1, dY1)
		if (!XPSEllipse::GetEllipses(dX1, dY1, dRadX, dRadY, dCx1, dCy1, dCx2, dCy2))
		{
			// Такого не должно быть
			pRenderer->PathCommandLineTo(_dX1, _dY1);
			dCurX = _dX1;
			dCurY = _dY1;
			return;
		}

		//double dTest1 = SQR(dCx1) / SQR(dRadX) + SQR(dCy1) / SQR(dRadY);
		//double dTest2 = SQR(dCx1 - dX1) / SQR(dRadX) + SQR(dCy1 - dY1) / SQR(dRadY);
		//double dTest3 = SQR(dCx2) / SQR(dRadX) + SQR(dCy2) / SQR(dRadY);
		//double dTest4 = SQR(dCx2 - dX1) / SQR(dRadX) + SQR(dCy2 - dY1) / SQR(dRadY);

		// Теперь у нас есть 2 эллипса, нужно определить дугу, которую нам и надо отрисовать. 
		// Для начала найдем углы начальной и конечной точек для обоих эллипсов.
		double dAngleStart1 = GetEllipseAngle(dCx1, dCy1, dRadX, dRadY, 0, 0);
		double dAngleEnd1   = GetEllipseAngle(dCx1, dCy1, dRadX, dRadY, dX1, dY1);

		if (dAngleEnd1 < dAngleStart1)
			dAngleEnd1 += 360;

		double dAngleStart2 = GetEllipseAngle(dCx2, dCy2, dRadX, dRadY, 0, 0);
		double dAngleEnd2   = GetEllipseAngle(dCx2, dCy2, dRadX, dRadY, dX1, dY1);

		if (dAngleEnd2 < dAngleStart2)
			dAngleEnd2 += 360;

		double dCx, dCy, dAngleStart, dAngleEnd;
		if ((dAngleEnd1 - dAngleStart1 > 180 && ((bIsLarge && !bClockwise) || (!bIsLarge && bClockwise)))
			|| (dAngleEnd1 - dAngleStart1 <= 180 && ((bIsLarge && bClockwise) || (!bIsLarge && !bClockwise))))
		{
			dCx         = dCx1;
			dCy         = dCy1;
			dAngleStart = dAngleStart1;
			dAngleEnd   = dAngleEnd1;
		}
		else
		{
			dCx         = dCx2;
			dCy         = dCy2;
			dAngleStart = dAngleStart2;
			dAngleEnd   = dAngleEnd2;
		}

		// Аппроксимируем эллипс кривыми Безье
		XPSEllipse::AppendEllipseArc(pRenderer, oInverse, dCx, dCy, dRadX, dRadY, dAngleStart, dAngleEnd, bClockwise);
		dCurX = _dX1;
		dCurY = _dY1;
	}
}

namespace XPS
{	
	int    GetDigit(wchar_t wChar)
	{
		if (wChar >= '0' && wChar <= '9')
			return (int)(wChar - '0');
		if (wChar >= 'a' && wChar <= 'f')
			return 10 + (int)(wChar - 'a');
		if (wChar >= 'A' && wChar <= 'F')
			return 10 + (int)(wChar - 'A');

		return 0;
	}
	bool   IsAlpha(wchar_t wChar)
	{
		return (((wChar >= 'A') && (wChar <= 'Z')) || ((wChar >= 'a') && (wChar <= 'z')));
	}
	double GetDouble(const CWString& wsString)
	{
		std::wstring sTmp(wsString.c_str(), wsString.size());
		return std::stod(sTmp);
	}
	double GetDouble(const std::wstring& wsString)
	{
        return std::stod(wsString.c_str());
	}
	int    GetInteger(const std::wstring& wsString)
	{
        return std::stoi(wsString.c_str());
	}
	bool   GetBool(const std::wstring& wsString)
	{
		std::wstring wsStr = wsString;
		NSStringExt::ToLower(wsStr);
		if ((wsStr == L"true") || (wsStr == L"t") || (wsStr == L"1") || (wsStr == L"on"))
			return true;

		return false;
	}
	void   GetBgra(const std::wstring& wsString, int& nBgr, int& nAlpha)
	{
		if (L'#' == wsString[0])
		{
			std::wstring wsStr = wsString.substr(1);

			while (wsStr.length() < 6)
				wsStr = L"0" + wsStr;

			while (wsStr.length() < 8)
				wsStr = L"F" + wsStr;

			nAlpha = GetDigit(wsStr[0]);
			nAlpha = nAlpha << 4;
			nAlpha += GetDigit(wsStr[1]);

			nBgr = GetDigit(wsStr[6]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[7]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[4]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[5]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[2]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[3]);
		}
		else if (L's' == wsString[0] && L'c' == wsString[1] && L'#' == wsString[2])
		{
			std::wstring wsStr = wsString.substr(3);
			std::vector<std::wstring> arrElements = NSStringExt::Split(wsStr, L',');

			if (3 == arrElements.size())
			{
				nAlpha = 255;
                nBgr = (((int)(std::min(GetDouble(arrElements[2]), 1.0) * 255)) << 16) + (((int)(std::min(GetDouble(arrElements[1]), 1.0) * 255)) << 8) + ((int)(std::min(GetDouble(arrElements[0]), 1.0) * 255));
			}
			else if (4 == arrElements.size())
			{
				nAlpha = GetDouble(arrElements[0]) * 255;
                nBgr = (((int)(std::min(GetDouble(arrElements[3]), 1.0) * 255)) << 16) + (((int)(std::min(GetDouble(arrElements[2]), 1.0) * 255)) << 8) + ((int)(std::min(GetDouble(arrElements[1]), 1.0) * 255));
			}
		}
		else
			return;
	}
    unsigned char GetCapStyle(const std::string& wsCapStyle)
	{
		BYTE nCapStyle = Aggplus::LineCapFlat;
        if (wsCapStyle == "Flat")
			nCapStyle = Aggplus::LineCapFlat;
        else if (wsCapStyle == "Round")
			nCapStyle = Aggplus::LineCapRound;
        else if (wsCapStyle == "Square")
			nCapStyle = Aggplus::LineCapSquare;
        else if (wsCapStyle == "Triangle")
			nCapStyle = Aggplus::LineCapTriangle;

		return nCapStyle;
	}
	std::wstring NormalizePath(const std::wstring& wsPath)
	{		
#ifdef WIN32
		std::wstring wsResult = wsPath;
		NSStringExt::Replace(wsResult, L"/", L"\\");
		while (std::wstring::npos != wsResult.find(L"\\\\"))
		{			
			NSStringExt::Replace(wsResult, L"\\\\", L"\\");
		}
		return wsResult;
#else
		std::wstring wsResult = wsPath;
		NSStringExt::Replace(wsResult, L"//", L"/");
        NSStringExt::Replace(wsResult, L"//", L"/");
		return wsResult;
#endif
	}
	std::wstring GetPath(const std::wstring& wsPath)
	{
		std::wstring wsResult;
		wsResult = wsPath.substr(0, wsPath.find_last_of('/') + 1);
		return wsResult;
	}
	std::wstring GetFileName(const std::wstring& wsPath)
	{
		int nCommaPos = wsPath.find_last_of(L'.');
		int nSlashPos = wsPath.find_last_of(L'/');

		if (std::wstring::npos == nCommaPos)
			nCommaPos = wsPath.length();
		if (std::wstring::npos == nSlashPos)
			nSlashPos = -1;

		if (nCommaPos < nSlashPos)
			return L"";

		std::wstring wsResult = wsPath.substr(nSlashPos + 1, nCommaPos - nSlashPos - 1);
		return wsResult;
	}
	std::wstring GetFileExtension(const std::wstring& wsPath)
	{
		int nCommaPos = wsPath.find_last_of(L'.');

		if (std::wstring::npos == nCommaPos)
			return L"";

		std::wstring wsResult = wsPath.substr(nCommaPos + 1);
		return wsResult;
	}
	std::wstring RemoveNamespace(const std::wstring& wsString)
	{
		std::wstring wsResult;

		int nPos = wsString.find(L":");
		if (std::wstring::npos != nPos)
			wsResult = wsString.substr(nPos + 1);
		else
			wsResult = wsString;

		return wsResult;
	}
	std::vector<std::vector<std::wstring>> Split(const std::wstring& wsString, wchar_t wDelim1, wchar_t wDelim2)
	{
		std::vector<std::vector<std::wstring>> arrResult;
		std::vector<std::wstring> arrStrings = NSStringExt::Split(wsString, wDelim1);
		int nCount = arrStrings.size();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			std::vector<std::wstring> arrStr = NSStringExt::Split(arrStrings[nIndex], wDelim2);
			arrResult.push_back(arrStr);
		}
		return arrResult;
	}
	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, std::wstring& wsAttr)
	{
		if (oReader.GetAttributesCount() <= 0)
			return;

		if (!oReader.MoveToFirstAttribute())
			return;

		std::wstring wsName = oReader.GetName();

		while (!wsName.empty())
		{
			if (wsAttrName == wsName)
			{
				wsAttr = oReader.GetText();
				break;
			}
			if (!oReader.MoveToNextAttribute())
				break;

			wsName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, CWString& wsAttr)
	{
		if (oReader.GetAttributesCount() <= 0)
			return;

		if (!oReader.MoveToFirstAttribute())
			return;

		CWString wsName = oReader.GetName();
		while (!wsName.empty())
		{
			if (wsName == wsAttrName)
			{
				wsAttr.create(oReader.GetText(), true);
				break;
			}
			if (!oReader.MoveToNextAttribute())
				break;

			wsName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	bool VmlToRenderer(const CWString& _wsString, IRenderer* pRenderer)
	{
		bool bWinding = false;

		const wchar_t* wsString = _wsString.c_str();
		int nLen = _wsString.size();
		int nPos = 0;

		double dCurX = 0.0, dCurY = 0.0;
		double dCpX = 0.0, dCpY = 0.0;

		bool bPrevCommandIsCurve = false;
		wchar_t wChar = 0x00;
		while (nPos < nLen)
		{
			if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
				break;

			wChar = GetChar(wsString, nPos);
			switch (wChar)
			{
				case 'F':
				{
					if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
						break;

					bWinding = GetBool(wsString, nPos, nLen);
					bPrevCommandIsCurve = false;
					break;
				}
				case 'm':
				{
					dCurX += GetDouble(wsString, nPos, nLen);
					dCurY += GetDouble(wsString, nPos, nLen);
					pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
					bPrevCommandIsCurve = false;
					break;
				}
				case 'M':
				{
					dCurX = GetDouble(wsString, nPos, nLen);
					dCurY = GetDouble(wsString, nPos, nLen);
					pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
					bPrevCommandIsCurve = false;
					break;
				}
				case 'L':
				case 'l':
				case 'H':
				case 'h':
				case 'V':
				case 'v':
				{
					while (true)
					{
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							switch (wChar)
							{
								case 'L':
								{
									dCurX = GetDouble(wsString, nPos, nLen);
									dCurY = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'l':
								{
									dCurX += GetDouble(wsString, nPos, nLen);
									dCurY += GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'H':
								{
									dCurX = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'h':
								{
									dCurX += GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'V':
								{
									dCurY = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'v':
								{
									dCurY += GetDouble(wsString, nPos, nLen);
									break;
								}
							}

							pRenderer->PathCommandLineTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
						}
						else
							break;
					}

					bPrevCommandIsCurve = false;
					break;
				}
				case 'A':
				case 'a':
				{
					while (true)
					{
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
							AppendArcTo(wsString, nPos, nLen, dCurX, dCurY, pRenderer, 'a' == wChar ? true : false);
						else
							break;
					}
					bPrevCommandIsCurve = false;
					break;
				}
				case 'C':
				case 'c':
				{
					while (true)
					{
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dX2, dY2;
							if ('c' == wChar)
							{
								dX1 = dCurX + GetDouble(wsString, nPos, nLen);
								dY1 = dCurY + GetDouble(wsString, nPos, nLen);
								dX2 = dCurX + GetDouble(wsString, nPos, nLen);
								dY2 = dCurY + GetDouble(wsString, nPos, nLen);
								dCurX += GetDouble(wsString, nPos, nLen);
								dCurY += GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX1 = GetDouble(wsString, nPos, nLen);
								dY1 = GetDouble(wsString, nPos, nLen);
								dX2 = GetDouble(wsString, nPos, nLen);
								dY2 = GetDouble(wsString, nPos, nLen);
								dCurX = GetDouble(wsString, nPos, nLen);
								dCurY = GetDouble(wsString, nPos, nLen);
							}
							pRenderer->PathCommandCurveTo(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX2), xpsUnitToMM(dY2), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
							dCpX = dX2;
							dCpY = dY2;
							bPrevCommandIsCurve = true;
						}
						else
							break;
					}
					break;
				}
				case 'Q':
				case 'q':
				{
					while (true)
					{
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dEndX, dEndY;

							if ('q' == wChar)
							{
								dX1   = dCurX + GetDouble(wsString, nPos, nLen);
								dY1   = dCurY + GetDouble(wsString, nPos, nLen);
								dEndX = dCurX + GetDouble(wsString, nPos, nLen);
								dEndY = dCurY + GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX1   = GetDouble(wsString, nPos, nLen);
								dY1   = GetDouble(wsString, nPos, nLen);
								dEndX = GetDouble(wsString, nPos, nLen);
								dEndY = GetDouble(wsString, nPos, nLen);
							}

							double dCx1 = dCurX / 3 + 2 * dX1 / 3;
							double dCy1 = dCurY / 3 + 2 * dY1 / 3;

							double dCx2 = dEndX / 3 + 2 * dX1 / 3;
							double dCy2 = dEndY / 3 + 2 * dY1 / 3;

							pRenderer->PathCommandCurveTo(xpsUnitToMM(dCx1), xpsUnitToMM(dCy1), xpsUnitToMM(dCx2), xpsUnitToMM(dCy2), xpsUnitToMM(dEndX), xpsUnitToMM(dEndY));

							dCurX = dEndX;
							dCurY = dEndY;
						}
						else
							break;
					}
					bPrevCommandIsCurve = false;
					break;
				}
				case 'S':
				case 's':
				{
					while (true)
					{
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dX2, dY2;
							if (!bPrevCommandIsCurve)
							{
								dX1 = dCurX;
								dY1 = dCurY;
							}
							else
							{
								dX1 = 2 * dCurX - dCpX;
								dY1 = 2 * dCurY - dCpY;
							}

							if ('s' == wChar)
							{
								dX2 = dCurX + GetDouble(wsString, nPos, nLen);
								dY2 = dCurX + GetDouble(wsString, nPos, nLen);
								dCurX += GetDouble(wsString, nPos, nLen);
								dCurY += GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX2 = GetDouble(wsString, nPos, nLen);
								dY2 = GetDouble(wsString, nPos, nLen);
								dCurX = GetDouble(wsString, nPos, nLen);
								dCurY = GetDouble(wsString, nPos, nLen);
							}

							pRenderer->PathCommandCurveTo(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX2), xpsUnitToMM(dY2), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
							dCpX = dX2;
							dCpY = dY2;
							bPrevCommandIsCurve = true;
						}
						else
							break;
					}
					break;
				}
				case 'Z':
				case 'z':
				{
					pRenderer->PathCommandClose();
					bPrevCommandIsCurve = false;
					break;
				}
			}
		}

		return bWinding;
	}
	bool GetNextGlyph(const wchar_t* wsIndices, int& nIndicesPos, const int& nIndicesLen, unsigned short* pUtf16, int& nUtf16Pos, const int& nUtf16Len, TIndicesEntry& oEntry)
	{
		oEntry.Reset();
		if (!wsIndices || nIndicesPos >= nIndicesLen)
		{
			if (!pUtf16 || nUtf16Pos >= nUtf16Len)
				return false;

			oEntry.nUnicode = pUtf16[nUtf16Pos++];
			oEntry.bUnicode = true;
			return true;
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return false;

		wchar_t wChar = LookChar(wsIndices, nIndicesPos);

		int nCodeUnitCount = 1, nGlyphCount = 1;
		if ('(' == wChar)
		{
			nIndicesPos++;
			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (IsNumber(wChar))
				nCodeUnitCount = GetInt(wsIndices, nIndicesPos, nIndicesLen);
			else
				nCodeUnitCount = 1;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (':' == wChar)
			{
				nIndicesPos++;
				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return false;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (IsNumber(wChar))
					nGlyphCount = GetInt(wsIndices, nIndicesPos, nIndicesLen);
				else
					nGlyphCount = 1;
			}
			else
				return false;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (')' != wChar)
				return false; // Такого не должно быть

			nIndicesPos++;
		}
		else if (oEntry.vRemainUnicodes.size() > 0)
		{
			nCodeUnitCount = 0;
			nGlyphCount    = 0;
		}


		if (nCodeUnitCount > 0 && nGlyphCount > 0)
		{
			oEntry.vRemainUnicodes.clear();
			// Нам нужно прочитать сколько реальных юникодных значений лежит в 
			// промежутке [pUnicode + nUnicodePos, pUnicode + nUnicodePos + nCodeUnitCount]
			int nUnicodesCount = 0;
			unsigned int* pUnicodes = NULL;
            nCodeUnitCount = std::min(nUtf16Len - nUtf16Pos, nCodeUnitCount);
			if (nCodeUnitCount)
			{
				pUnicodes = new unsigned int[nCodeUnitCount];
				if (!pUnicodes)
					return false;

				unsigned short ushLeading, ushTraling;
				unsigned int unCode;
				int nCodeUnitPos = 0;
				while (nCodeUnitPos < nCodeUnitCount)
				{
					ushLeading = pUtf16[nUtf16Pos + nCodeUnitPos++];
					if (ushLeading < 0xD800 || ushLeading > 0xDFFF)
					{
						pUnicodes[nUnicodesCount++] = (unsigned int)ushLeading;
					}
					else if (ushLeading >= 0xDC00)
					{
						// Такого не должно быть
						continue;
					}
					else
					{
						if (nCodeUnitPos >= nCodeUnitCount)
							break;

						unCode = (ushLeading & 0x3FF) << 10;
						ushTraling =  pUtf16[nUtf16Pos + nCodeUnitPos++];
						if (ushTraling < 0xDC00 || ushTraling > 0xDFFF)
						{
							// Такого не должно быть
							continue;
						}
						else
						{
							pUnicodes[nUnicodesCount++] = (unCode | (ushTraling & 0x3FF) + 0x10000);
						}
					}
				}
			}

			// Равномерно распределяем юникоды по глифам, в идеале их количество должно совпадать.
			// Если юникодов больше, то лишние удаляем, если их меньше, то недостающие заполняем пробелами.
            nUnicodesCount = std::min(nUnicodesCount, nGlyphCount);
			for (int nIndex = 0; nIndex < nGlyphCount; nIndex++)
			{
				if (nIndex < nUnicodesCount)
					oEntry.vRemainUnicodes.push_back(pUnicodes[nIndex]);
				else
					oEntry.vRemainUnicodes.push_back(0x20);
			}

			if (pUnicodes)
				delete[] pUnicodes;

			nUtf16Pos += nCodeUnitCount;
		}

		if (oEntry.vRemainUnicodes.size() <= 0)
			return false;

		// Теперь мы читаем ровно 1 глиф с возможными метриками
		oEntry.nUnicode = oEntry.vRemainUnicodes.at(0);
		oEntry.vRemainUnicodes.erase(oEntry.vRemainUnicodes.begin());

		wChar = LookChar(wsIndices, nIndicesPos);
		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return false;

		if (IsNumber(wChar))
		{
			oEntry.nGid = GetInt(wsIndices, nIndicesPos, nIndicesLen);
			oEntry.bGid = true;
		}
		else
		{
			oEntry.bGid = false;
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return true;

		wChar = LookChar(wsIndices, nIndicesPos);
		if (',' == wChar)
		{
			nIndicesPos++;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return true;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (IsNumber(wChar))
			{
				oEntry.dAdvance = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
				oEntry.bAdvance = true;
			}
			else
			{
				oEntry.bAdvance = false;
			}

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return true;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (',' == wChar)
			{
				nIndicesPos++;
				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return true;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (IsNumber(wChar))
				{
					oEntry.dHorOffset = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
					oEntry.bHorOffset = true;
				}
				else
				{
					oEntry.bHorOffset = false;
				}

				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return true;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (',' == wChar)
				{
					nIndicesPos++;
					if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
						return true;

					wChar = LookChar(wsIndices, nIndicesPos);
					if (IsNumber(wChar))
					{
						oEntry.dVerOffset = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
						oEntry.bVerOffset = true;
					}
					else
					{
						oEntry.bVerOffset = false;
					}
				}
			}
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return true;

		wChar = LookChar(wsIndices, nIndicesPos);
		if (';' == wChar)
		{
			nIndicesPos++;
			return true;
		}
		else
			return false; // Такого не должно быть
	}
	void ReadMatrixTransform(XmlUtils::CXmlLiteReader& oReader, CWString& wsTransform, CWString* pwsKey)
	{
		if (oReader.GetAttributesCount() <= 0)
			return;

		if (!oReader.MoveToFirstAttribute())
			return;

		CWString wsAttrName = oReader.GetName();
		while (!wsAttrName.empty())
		{
			if (wsAttrName == L"Matrix")
				wsTransform.create(oReader.GetText(), true);
			else if (wsAttrName == L"x:Key" && pwsKey)
				pwsKey->create(oReader.GetText(), true);

			if (!oReader.MoveToNextAttribute())
				break;

			wsAttrName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	void ReadTransform      (XmlUtils::CXmlLiteReader& oReader, CWString& wsTransform, CWString* pwsKey)
	{
		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"MatrixTransform")
			{
				ReadMatrixTransform(oReader, wsTransform, pwsKey);
			}
		}
	}
	void ReadPathGeometry   (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTransform, CWString* pwsKey)
	{
		bool bEvenOdd = true;
		CWString wsAttrName;
		if (oReader.MoveToFirstAttribute())
		{
			wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{
				if (wsAttrName == L"x:Key" && pwsKey)
					pwsKey->create(oReader.GetText(), true);
				else if (wsAttrName == L"Figures")
					wsData.create(oReader.GetText(), true);
				else if (wsAttrName == L"Transform")
					wsTransform.create(oReader.GetText(), true);
				else if (wsAttrName == L"FillRule")
				{
					CWString wsFillingRule = oReader.GetText();
					bEvenOdd = wsFillingRule == L"EvenOdd" ? true : false;
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}

			oReader.MoveToElement();
		}

		if (oReader.IsEmptyNode())
			return;

		std::wstring _wsData;
		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"PathGeometry.Transform" && wsTransform.empty())
				ReadTransform(oReader, wsTransform);
			else if (wsNodeName == L"PathFigure" && wsData.empty())
				ReadPathFigure(oReader, _wsData, bEvenOdd);
		}
		wsData.create(_wsData.c_str(), true);
	}
	void ReadPathFigure     (XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData, bool bEvenOdd)
	{
		// TODO: Улучшить здесь сложение строк и хождение по атрибутам
		if (oReader.IsEmptyNode())
			return;

		if (!bEvenOdd)
			wsData += L"F 1";

		std::wstring wsStartPoint;
		std::wstring wsIsClosed;
		ReadAttribute(oReader, L"StartPoint", wsStartPoint);
		ReadAttribute(oReader, L"IsClosed", wsIsClosed);
		wsData += L" M " + wsStartPoint;

		std::wstring wsNodeName;
		std::wstring wsText;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			wsText.empty();
			if (L"PolyLineSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" L " + wsText;
			}
			else if (L"PolyBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" C " + wsText;
			}
			else if (L"PolyQuadraticBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" Q " + wsText;
			}
			else if (L"ArcSegment" == wsNodeName)
			{
				std::wstring wsSize, wsRotationAngle, wsIsLargeArc, wsSweepDirection, wsPoint, wsIsStroked;
				if (oReader.MoveToFirstAttribute())
				{
					std::wstring wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (L"Size" == wsAttrName)
							wsSize = oReader.GetText();
						else if (L"RotationAngle" == wsAttrName)
							wsRotationAngle = oReader.GetText();
						else if (L"IsLargeArc" == wsAttrName)
							wsIsLargeArc = oReader.GetText();
						else if (L"SweepDirection" == wsAttrName)
							wsSweepDirection = oReader.GetText();
						else if (L"Point" == wsAttrName)
							wsPoint = oReader.GetText();

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}
					oReader.MoveToElement();
				}

				wsData += L" A " + wsSize + L" " + wsRotationAngle + L" ";
				if (GetBool(wsIsLargeArc))
					wsData += L"0 ";
				else
					wsData += L"1 ";

				if (L"Counterclockwise" == wsSweepDirection)
					wsData += L"0 ";
				else
					wsData += L"1 ";

				wsData += wsPoint;
			}
		}

		if (GetBool(wsIsClosed))
			wsData += L" Z ";
	}
	void ReadGradientStops  (XmlUtils::CXmlLiteReader& oReader, std::vector<LONG>& vColors, std::vector<double>& vPositions, const double& dOpacity)
	{
		if (oReader.IsEmptyNode())
			return;

		CWString wsNodeName, wsAttrName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"GradientStop")
			{				
				double dPos = 0;
				LONG lColor = 0;
				if (oReader.MoveToFirstAttribute())
				{
					wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (wsAttrName == L"Color")
						{
							int nBgr, nAlpha;
							ReadSTColor(oReader.GetText(), nBgr, nAlpha);
							nAlpha *= dOpacity;
							lColor = (nAlpha << 24 & 0xFF000000) | (nBgr & 0xFFFFFF);
						}
						else if (wsAttrName == L"Offset")
						{
							ReadSTDouble(oReader.GetText(), dPos);
						}

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}

					oReader.MoveToElement();
				}
				vColors.push_back(lColor);
				vPositions.push_back(dPos);
			}
		}
	}
	void ReadClip           (XmlUtils::CXmlLiteReader& oReader, CWString& wsClip)
	{
		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetNameNoNS();
			if (wsNodeName == L"PathGeometry")
			{
				CWString wsTransform;
				ReadPathGeometry(oReader, wsClip, wsTransform);
			}
		}
	}

	void ReadSTPoint(const CWString& wsString, double& dX, double& dY)
	{
		int nCommaPos = 0;
		while (nCommaPos < wsString.size())
		{
			if (wsString[nCommaPos] == ',')
				break;

			nCommaPos++;
		}

		if (nCommaPos >= wsString.size())
		{
			CWString wsX = wsString.c_str();
			dX = GetDouble(wsX);
			dY = 0;
		}
		else
		{
			CWString wsX((wchar_t*)wsString.c_str(), false, nCommaPos);
			CWString wsY((wchar_t*)(wsString.c_str() + nCommaPos + 1), false, wsString.size() - nCommaPos - 1);
			dX = GetDouble(wsX);
			dY = GetDouble(wsY);
		}
	}
	void ReadSTColor(const CWString& wsString, int& nBgr, int& nAlpha)
	{
		int nLen = wsString.size();
		if (nLen <= 0)
			return;

		const wchar_t* pBuffer = wsString.c_str();
		if (L'#' == pBuffer[0])
		{
			nLen--;
			pBuffer++;

			if (6 != nLen && 8 != nLen)
				return;

			if (8 == nLen)
			{
				nAlpha = GetDigit(*pBuffer++);
				nAlpha <<= 4;
				nAlpha += GetDigit(*pBuffer++);
			}
			else
			{
				nAlpha = 255;
			}

			nBgr = GetDigit(pBuffer[4]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[5]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[2]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[3]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[0]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[1]);
		}
		else if (nLen >= 3 && L's' == pBuffer[0] && L'c' == pBuffer[1] && L'#' == pBuffer[2])
		{
			int nPos = 3;
			if (nPos >= nLen)
				return;

			CWString wsString2;
			wsString2.create(pBuffer + 3, false);

			std::vector<CWString> vElements = wsString2.split(',');
			if (3 == vElements.size())
			{
				nAlpha = 255;
                nBgr   = (((int)(std::min(GetDouble(vElements[2]), 1.0) * 255)) << 16) + (((int)(std::min(GetDouble(vElements[1]), 1.0) * 255)) << 8) + ((int)(std::min(GetDouble(vElements[0]), 1.0) * 255));
			}
			else if (4 == vElements.size())
			{
                nAlpha = (int)(std::min(GetDouble(vElements[0]), 1.0) * 255);
                nBgr   = (((int)(std::min(GetDouble(vElements[3]), 1.0) * 255)) << 16) + (((int)(std::min(GetDouble(vElements[2]), 1.0) * 255)) << 8) + ((int)(std::min(GetDouble(vElements[1]), 1.0) * 255));
			}
		}
	}
	void ReadSTColor(const CWString& wsString, LONG& lColor)
	{
		int nBgr, nAlpha;
		ReadSTColor(wsString, nBgr, nAlpha);
		lColor = (nAlpha << 24 & 0xFF000000) | (nBgr & 0xFFFFFF);
	}
	void ReadSTDouble(const CWString& wsString, double& dValue)
	{
		int nPos = 0;
		dValue = GetDouble(wsString.c_str(), nPos, wsString.size());
	}
}
