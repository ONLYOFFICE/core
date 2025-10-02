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
#ifndef _BUILD_GRAPHICSPATH_H_
#define _BUILD_GRAPHICSPATH_H_

#include "AggPlusEnums.h"
#include "./config.h"

#include "IRenderer.h"

#include "Matrix.h"
#include "./pro/Fonts.h"

namespace Aggplus
{
	class CGraphicsPath_private;
	class GRAPHICS_DECL CGraphicsPath : public NSFonts::ISimpleGraphicsPath
	{
	public:
		CGraphicsPath();
		CGraphicsPath(const CGraphicsPath& other) noexcept;
		CGraphicsPath(CGraphicsPath&& other) noexcept;
		CGraphicsPath(const std::vector<CGraphicsPath>& paths) noexcept;
		virtual ~CGraphicsPath();

		CGraphicsPath* Clone();

		Status Reset();
		void SetRuler(bool bEvenOdd);

		Status StartFigure();
		Status CloseFigure();
		bool Is_poly_closed() const;
		Status MoveTo(double x, double y);
		Status LineTo(double x, double y);
		Status CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);

		// методы, которые просто будем сводить к трем основным
		Status AddLine(double x1, double y1, double x2, double y2);
		Status AddLines(double* pPoints, int nCount);
		Status AddBezier(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
		Status AddBeziers(double* pPoints, int nCount);
		Status AddCurve(double* pPoints, int nCount);
		Status AddEllipse(double x, double y, double width, double height);
		Status AddRectangle(double x, double y, double width, double height);
		Status AddRoundRectangle(double x, double y, double width, double height, double cx, double cy);
		Status AddPolygon(double* pPoints, int nCount);
		Status AddPath(const CGraphicsPath& oPath);
		Status AddArc(double x, double y, double width, double height, double startAngle, double sweepAngle);

		ULONG GetPointCount() const;
		Status GetPathPoints(PointF* points, int count) const;
		Status GetLastPoint(double& x, double& y);
		Status GetPathPoints(double* points, int count) const;
		void GetBounds(double& left, double& top, double& width, double& height) const;
		void GetBoundsAccurate(double& left, double& top, double& width, double& height) const;

		Status Transform(const CMatrix* matrix);
		virtual bool _MoveTo(double x, double y);
		virtual bool _LineTo(double x, double y);
		virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
		virtual bool _Close();

		Status AddString(const std::wstring& strText, NSFonts::IFontManager* pFont, double x, double y);
		Status AddString(const unsigned int* pGids, const unsigned int nGidsCount, NSFonts::IFontManager* pFont, double x, double y);
		Status AddStringC(const LONG& lText, NSFonts::IFontManager* pFont, double x, double y);
		void z_Stroke(const double& size);
		void Widen(const double& size, const Aggplus::LineJoin& join, const CMatrix* matrix, float flatness);

		int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
		double AngToEllPrm(double fAngle, double fXRad, double fYRad);
		int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
		int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double* pfXCur, double* pfYCur, INT bClockDirection = FALSE);
		int Ellipse(double fX, double fY, double fXRad, double fYRad);
		Status AddArc2(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		bool IsPointInPath(const double& x, const double& y);

		// Methods for Path Clip
		unsigned GetCloseCount() const noexcept;
		unsigned GetMoveCount()	 const noexcept;
		bool	 IsClockwise()	 const noexcept;
		bool	 IsMovePoint(unsigned idx)	const noexcept;
		bool	 IsCurvePoint(unsigned idx) const noexcept;
		bool	 IsLinePoint(unsigned idx)	const noexcept;
		bool	 IsClosePoint(unsigned idx) const noexcept;
		double	 GetArea() const noexcept;
		double	 GetArea(unsigned idx, bool isCurve) const noexcept;
		std::vector<PointD>  GetPoints(unsigned idx, unsigned count) const noexcept;
		std::vector<CGraphicsPath> GetSubPaths() const;

		CGraphicsPath& operator=(const CGraphicsPath& other)	noexcept;
		CGraphicsPath& operator=(CGraphicsPath&& other)			noexcept;
		bool operator==(const CGraphicsPath& other)	noexcept;

	public:
		CGraphicsPath_private* m_internal;
	};

	class CGraphicsPathSimpleConverter_private;
	class GRAPHICS_DECL CGraphicsPathSimpleConverter : public NSFonts::ISimpleGraphicsPath
	{
	private:
		IRenderer* m_pRenderer;
		CGraphicsPathSimpleConverter_private* m_internal;

	public:
		CGraphicsPathSimpleConverter();
		~CGraphicsPathSimpleConverter();

	public:
		void SetRenderer(IRenderer* pRenderer);
		IRenderer* GetRenderer(INT bIsAddref = FALSE);

	public:
		bool PathCommandMoveTo(double fX, double fY);
		bool PathCommandLineTo(double fX, double fY);
		bool PathCommandLinesTo(double* pPoints, LONG lCount);
		bool PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
		bool PathCommandCurvesTo(double* pData, LONG lCount);
		bool PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		bool PathCommandClose();
		bool PathCommandEnd();
		bool PathCommandStart();
		bool PathCommandGetCurrentPoint(double* fX, double* fY);
		bool PathCommandText(const std::wstring& bsText, NSFonts::IFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
		bool
		PathCommandTextEx(std::wstring& bsText, std::wstring& bsGidText, NSFonts::IFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);

		bool PathCommandText2(const int* pUnicodes, const int* pGids, const int& nCount, NSFonts::IFontManager* pManager, const double& x, const double& y, const double& w, const double& h);
		bool PathCommandText2(const std::wstring& sUnicodes, const int* pGids, const int& nCount, NSFonts::IFontManager* pManager, const double& x, const double& y, const double& w, const double& h);

		bool PathCommandGetBounds(double& left, double& top, double& width, double& height);

	public:
		virtual bool _MoveTo(double x, double y);
		virtual bool _LineTo(double x, double y);
		virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
		virtual bool _Close();
		bool _Reset();
		bool _Start();

	protected:
		bool AddString(const std::wstring& bstrText, NSFonts::IFontManager* pFont, double x, double y);

		int EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
		double AngToEllPrm(double fAngle, double fXRad, double fYRad);
		int EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection);
		int EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double* pfXCur, double* pfYCur, INT bClockDirection = FALSE);
		int Ellipse(double fX, double fY, double fXRad, double fYRad);

		bool AddArc(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);

		bool Is_poly_closed();
	};

	enum BooleanOpType
	{
		Intersection   = 1,
		Union          = 0,
		Subtraction    = 2,
		Exclusion      = 3
	};

	GRAPHICS_DECL CGraphicsPath CalcBooleanOperation(const CGraphicsPath& path1, const CGraphicsPath& path2, BooleanOpType op, long fillType = c_nWindingFillMode, bool isLuminosity = false);

} // namespace Aggplus

#endif // _BUILD_GRAPHICSPATH_H_
