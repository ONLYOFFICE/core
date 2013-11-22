// GraphicsPath.h: interface for the GraphicsPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_GRAPHICSPATH_H__18F4FB65_E427_4604_90A9_ADFC5EBFB533__INCLUDED_)
#define Aggplus_GRAPHICSPATH_H__18F4FB65_E427_4604_90A9_ADFC5EBFB533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"
#include "ap_StringFormat.h"

#include "agg_path_storage.h"

namespace Aggplus
{
class Matrix;
class Pen;
//ZTODO: class FontFamily;
class Font;
class GraphicsPath;

Status aggplus_flat_add_string_byfont_getbounds(GraphicsPath *path,
                                                const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *format,
                                                RectF *Bounds, int *codepointsFitted, int *linesFilled);
void aggplus_flat_add_string_byfont_singleline(GraphicsPath *path, const WCHAR* string, int length, const Font* font, const PointF &origin);

class GraphicsPath
{
friend class Graphics;
public:
	GraphicsPath();
	virtual ~GraphicsPath();
 GraphicsPath *Clone();

 Status Reset();

 Status AddRectangle(const RectF& rect);
 Status AddEllipse(const RectF& rect) { return AddEllipse(rect.X, rect.Y, rect.Width, rect.Height); }
 Status AddEllipse(REAL x, REAL y, REAL width, REAL height);

 Status AddArc(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
 Status AddArc(const RectF& rect, REAL startAngle, REAL sweepAngle)
  {
   return AddArc(rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle); }

 Status StartFigure();
 Status CloseFigure();
 //void GetLastPoint(PointF *pfLast) const;
 
 Status AddStringFont(const WCHAR *string, INT length, const Font *font, const PointF &origin, const StringFormat *format)
  {
   RectF rect(origin.X, origin.Y, 0.0f, 0.0f);
   return AddStringFont(string, length, font, rect, format);
  }
 Status AddStringFont(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *format);


 Status AddLine(REAL x1, REAL y1, REAL x2, REAL y2);
 Status AddLine(const PointF& pt1, const PointF& pt2) { return AddLine(pt1.X, pt1.Y, pt2.X, pt2.Y); }
 
 Status AddBezier(REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
 Status AddBezier(const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4)
  { return AddBezier(pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, pt4.X, pt4.Y); }

 int GetPointCount() const;
 Status GetPathPoints(PointF* points, int count) const;


 void GetBounds(RectF *pRct);
 Status Transform(const Matrix* matrix);

 BOOL IsVisible(REAL x, REAL y, const Graphics* g=NULL);

 //ZTODO
 void Widen(const Pen* Pen, const Matrix* matrix=NULL, float flatness=0.00);
 //ZTODO End
 

 //Non GdiPlus
 void z_MoveTo(float X, float Y);
 void z_LineTo(float X, float Y);
 void z_LineTo(const PointF & pf) { z_LineTo(pf.X, pf.Y); }
 void z_Stroke(const Pen* Pen);
 bool z_is_poly_closed();

 agg::path_storage *z_get_agg_path_storage() { return(&m_agg_ps); }
protected:
 agg::path_storage m_agg_ps;

};


}
#endif // !defined(Aggplus_GRAPHICSPATH_H__18F4FB65_E427_4604_90A9_ADFC5EBFB533__INCLUDED_)
