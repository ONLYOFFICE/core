//----------------------------------------------------------------------------
// GraphicsPath class  - Version 0.1.2 - 07-11-2004
// Marlon Mircevski
//----------------------------------------------------------------------------

//#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "ap_GraphicsPath.h"
#include "ap_Graphics.h"
#include "ap_Pen.h"
//#include "ap_FontFamily.h"

//#define UNICODE
//#define _UNICODE

#include "ap_Font.h"
//#include "agg_font_win32_tt.h"



#pragma warning(disable : 4786)
//#pragma warning(disable : 4097)

#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_contour.h"
#include "agg_conv_unclose_polygon.h"

#include "agg_bezier_arc.h"
#include "agg_conv_curve.h"

namespace Aggplus
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GraphicsPath::GraphicsPath()
{
}

GraphicsPath::~GraphicsPath()
{
}

GraphicsPath *GraphicsPath::Clone()
{
 GraphicsPath *pNew=new GraphicsPath;
 pNew->m_agg_ps=m_agg_ps;
 return(pNew);
}

Status GraphicsPath::Reset() { m_agg_ps.remove_all(); return Ok; }

Status GraphicsPath::StartFigure() { m_agg_ps.start_new_path(); return Ok; }
Status GraphicsPath::CloseFigure() { m_agg_ps.close_polygon(); return Ok; }

/*
void GraphicsPath::GetLastPoint(PointF *pfLast) const
{
 double x, y;
 m_agg_ps.last_vertex(&x, &y);
 pfLast->X=(float)x;
 pfLast->Y=(float)y;
}
*/

void GraphicsPath::z_MoveTo(float X, float Y) { m_agg_ps.move_to(X, Y); }
void GraphicsPath::z_LineTo(float X, float Y) { m_agg_ps.line_to(X, Y); }

int GraphicsPath::GetPointCount() const
{
 unsigned int nPointCount=0;
 unsigned int nTotal = m_agg_ps.total_vertices();
 double x, y;
 for(unsigned int i=0;i<nTotal;i++)
  {
   unsigned int nTip=m_agg_ps.vertex(i, &x, &y);
   if(nTip)
    {
     if(!(nTip&agg::path_flags_close))
      {
       nPointCount++;
      }
    }
  }
 return nPointCount;
}

Status GraphicsPath::GetPathPoints(PointF* points, int count) const
{
 int nTotal = m_agg_ps.total_vertices();
 double x, y;
 int i=0, k=0;
 while(k<count && i<nTotal)
  {
   unsigned int nTip=m_agg_ps.vertex(i, &x, &y);
   if(nTip)
    {
     if(!(nTip&agg::path_flags_close))
      {
       points[k].X=REAL(x);
       points[k].Y=REAL(y);
       k++;
      }
    }
   i++;
  }

 return Ok;
}

Status GraphicsPath::AddLine(REAL x1, REAL y1, REAL x2, REAL y2)
{
 if(z_is_poly_closed()) m_agg_ps.move_to(x1, y1);
 else                   m_agg_ps.line_to(x1, y1);
 m_agg_ps.line_to(x2, y2);
 return Ok;
}

Status GraphicsPath::AddBezier(REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4)
{
 if(z_is_poly_closed()) m_agg_ps.move_to(x1, y1);
 else                   m_agg_ps.line_to(x1, y1);
 m_agg_ps.curve4(x2, y2, x3, y3, x4, y4);
 return Ok;
}

Status GraphicsPath::AddRectangle(const RectF& rect)
{
 m_agg_ps.move_to(rect.X, rect.Y);
 m_agg_ps.line_to(rect.X+rect.Width, rect.Y);
 m_agg_ps.line_to(rect.X+rect.Width, rect.Y+rect.Height);
 m_agg_ps.line_to(rect.X, rect.Y+rect.Height);
 m_agg_ps.close_polygon();
 return Ok;
}

bool GraphicsPath::z_is_poly_closed()
{
 if(!m_agg_ps.total_vertices()) return(true);

 double x, y;
 unsigned int nTip=m_agg_ps.last_vertex(&x, &y);
 if(nTip&agg::path_flags_close) return(true);
 return(false);
}

Status GraphicsPath::AddArc(REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
{
 if(sweepAngle>=360.0f)
  {
   //ZTODO: to emulate Gdi+ behavior
  }
 agg::bezier_arc arc(x+width/2.00, y+height/2.00, width/2.00, height/2.00, agg::deg2rad(startAngle), agg::deg2rad(sweepAngle));

 //2.3 m_agg_ps.add_path(arc, 0,  !z_is_poly_closed());

 if(z_is_poly_closed()) m_agg_ps.concat_path(arc, 0);
 else                   m_agg_ps.join_path(arc, 0);
 
 return Ok;
}

Status GraphicsPath::AddEllipse(REAL x, REAL y, REAL width, REAL height)
{
 agg::bezier_arc arc(x+width/2.00, y+height/2.00, width/2.00, height/2.00, 0.00, 2.00*agg::pi);
 //2.3 m_agg_ps.add_path(arc, 0, true);
 m_agg_ps.join_path(arc, 0);
 return Ok;
}


void GraphicsPath::GetBounds(RectF *pRct)
{
 unsigned int nTotal = m_agg_ps.total_vertices();
 if(nTotal)
  {
   agg::rect_d bounds(1e100, 1e100, -1e100, -1e100);
   double x, y;
   for(unsigned int i=0;i<nTotal;i++)
    {
     unsigned int nTip=m_agg_ps.vertex(i, &x, &y);
     if(agg::is_vertex(nTip))
      {
       if(x < bounds.x1) bounds.x1 = x;
       if(y < bounds.y1) bounds.y1 = y;
       if(x > bounds.x2) bounds.x2 = x;
       if(y > bounds.y2) bounds.y2 = y;
      }

     /*
     if(nTip)
      {
       if(!(nTip&agg::path_flags_close))
        {
         if(x < bounds.x1) bounds.x1 = x;
         if(y < bounds.y1) bounds.y1 = y;
         if(x > bounds.x2) bounds.x2 = x;
         if(y > bounds.y2) bounds.y2 = y;
        }
      }
      */
    }
   pRct->X=(float)bounds.x1;
   pRct->Y=(float)bounds.y1;
   pRct->Width=(float)(bounds.x2-bounds.x1);
   pRct->Height=(float)(bounds.y2-bounds.y1);
  }
 else { *pRct=RectF(0,0,0,0); }
}

Status GraphicsPath::Transform(const Matrix* matrix)
{
 if(matrix)
  {
   agg::path_storage p2(m_agg_ps);
   agg::conv_transform<agg::path_storage> trans(p2, matrix->m_agg_mtx);
   m_agg_ps.remove_all();
   //2.3 m_agg_ps.add_path(trans, 0, false);
   m_agg_ps.concat_path(trans, 0);
   
  }
 return Ok;
}

BOOL GraphicsPath::IsVisible(REAL x, REAL y, const Graphics* g)
{
 assert(g==NULL); //ZTODO

 agg::rasterizer_scanline_aa<> ras;
 typedef agg::conv_curve<agg::path_storage> conv_crv_type;

 agg::path_storage p_copy(m_agg_ps);
 conv_crv_type p3(p_copy);

 //2.3 ras.add_path(p3);
 m_agg_ps.join_path(p3, 0);
 return(ras.hit_test((int)x, (int)y));
}

void GraphicsPath::Widen(const Pen* Pen, const Matrix* matrix, float flatness)
{
 assert(Pen);
 assert(matrix==NULL && flatness==0.00); //ZTODO

 typedef agg::conv_curve<agg::path_storage> conv_crv_type;

 typedef agg::conv_contour<conv_crv_type> Path_Conv_Contour;

 conv_crv_type crv(m_agg_ps);
 Path_Conv_Contour pg(crv);

 pg.miter_limit(0.50);
 //?pg.miter_limit_theta(0.05);
 //?pg.approximation_scale(2.00);

 pg.width(Pen->GetWidth());

 agg::line_join_e LineJoin;
 switch(Pen->GetLineJoin())
  {
   case LineJoinMiter       : LineJoin=agg::miter_join; break;
   case LineJoinBevel       : LineJoin=agg::bevel_join; break;
   default:
   case LineJoinRound       : LineJoin=agg::round_join; break;
   case LineJoinMiterClipped: LineJoin=agg::miter_join_revert; break;
  }
 pg.line_join(LineJoin);

 pg.auto_detect_orientation(false);

 agg::path_storage psNew;
 //2.3 psNew.add_path(pg, 0, false);
 //m_agg_ps.concat_path(pg, 0);
 m_agg_ps.concat_path(pg, 0);
 m_agg_ps=psNew;
}

void GraphicsPath::z_Stroke(const Pen* Pen)
{
 assert(Pen);
 typedef agg::conv_stroke<agg::path_storage> Path_Conv_Stroke;
 Path_Conv_Stroke pg(m_agg_ps);
 pg.line_join(agg::round_join);
 pg.line_cap(agg::round_cap);
 pg.approximation_scale(25.00);
 //pg.miter_limit(0.50); //dali?

 pg.width(Pen->GetWidth());
 //pg.auto_detect_orientation(true);
 
 agg::path_storage psNew;
 //2.3 psNew.add_path(pg, 0, false);
 psNew.concat_path(pg, 0);

 m_agg_ps=psNew;
}


#define SPC WCHAR(' ')
#define RET WCHAR('\n')
Status GraphicsPath::AddStringFont(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *format)
{
 return aggplus_flat_add_string_byfont_getbounds(this, string, length, font, layoutRect, format, NULL, NULL, NULL);
}

Status aggplus_flat_add_string_byfont_getbounds(GraphicsPath *path, const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *format,
                                                   RectF *Bounds, int *codepointsFitted, int *linesFilled)
{
 if(!font || !font->IsAvailable()) { return InvalidParameter; }
 StringFormat *pDefault_format=NULL;
 if(!format) { pDefault_format=new StringFormat; format=pDefault_format; }

 if(length<0) length=(Aggplus::INT)wcslen(string);
 StringAlignment alignment=format->GetAlignment();

 //double m_weight_value=0.00;
 double f_em_height=font->GetEmHeight();
 double f_em_linespacing = font->GetLineSpacing();
 //double f_em_ascent = font->GetCellAscent();
 double f_font_size=font->get_res_unit_to_pixel()*font->GetSize();
 double f_line_Advance=f_font_size * f_em_linespacing / f_em_height;

 float f_txt_max_width=(float)(layoutRect.Width-2.0*font->get_1_6_em());
 float f_txt_max_height=(float)(layoutRect.Y+layoutRect.Height);

 float f_text_offpoint=layoutRect.X;
 if(     alignment==StringAlignmentNear)   {  }
 else if(alignment==StringAlignmentCenter) { f_text_offpoint+=f_txt_max_width/2.00f; }
 else if(alignment==StringAlignmentFar)    { f_text_offpoint+=f_txt_max_width; }

 if(layoutRect.Width<=0.0f) { f_txt_max_width=REAL_MAX; } //Infinite
 if(layoutRect.Height<=0.0f) { f_txt_max_height=REAL_MAX; } //Infinite

 double dblOrgY=layoutRect.Y;

 float bnd_x1=REAL_MAX, bnd_x2=REAL_MIN;
 float bnd_y1=REAL_MAX, bnd_y2=REAL_MIN;
 bool break_by_newline=true;
 bnd_y1=(REAL)dblOrgY;

 int num_lines=0;
 int num_chars=0;
 while(length>0 && dblOrgY+f_line_Advance<f_txt_max_height)
  {
   if(!break_by_newline)
    {
     while(length>0 && *string==SPC) { string++; length--; num_chars++; } //strip spaces from begin.
    }
   if(!length) break;
   int nLinChars, nAdvanceChars;
   float fLinStart, fLinWidth;
   aggplus_flat_measure_fit_chars(font, string, length, f_txt_max_width, &fLinStart, &fLinWidth, &nLinChars, &nAdvanceChars, &break_by_newline);
   if(nLinChars>0)
    {
     assert(nAdvanceChars);
     PointF origin(f_text_offpoint, (REAL)dblOrgY);
     
     if(     alignment==StringAlignmentNear)   {  }
     else if(alignment==StringAlignmentCenter) { origin.X-=fLinWidth/2.00f; }
     else if(alignment==StringAlignmentFar)    { origin.X-=fLinWidth; }
     else {  } //StringAlignmentNear
     
     if(bnd_x1>origin.X) bnd_x1=origin.X;
     if(bnd_x2<origin.X+fLinWidth) bnd_x2=origin.X+fLinWidth;
     //if(bnd_y2<layoutRect.Y+dblOrgY) bnd_y2=layoutRect.Y+dblOrgY;

     //if we are to mimic gdiplus we don't use fLinStart left edge correction, else ->
     origin.X-=fLinStart;
     if(path) aggplus_flat_add_string_byfont_singleline(path, string, nLinChars, font, origin);
    }
   else if(nAdvanceChars==0) { nAdvanceChars++; } //just skip
   string+=nAdvanceChars;
   length-=nAdvanceChars;
   num_lines++;
   num_chars+=nAdvanceChars;

   /*
   if(length==0 && nLinChars==0)
    {
     assert(FALSE);
    }
    */

   dblOrgY+=f_line_Advance;
   if(bnd_y2<dblOrgY) bnd_y2=(float)dblOrgY;

   /*
   if(nLinChars>0)
    {
     if(bnd_y2<layoutRect.Y+dblOrgY) bnd_y2=layoutRect.Y+dblOrgY;
    }
   else
    {
     //assert(FALSE);
    }
    */
  }

 if(codepointsFitted) *codepointsFitted=num_chars;
 if(linesFilled) { if(num_chars) *linesFilled=num_lines; else *linesFilled=0; }
 if(Bounds)
  {
   if(bnd_x1<REAL_MAX && bnd_y1<REAL_MAX && bnd_x2>REAL_MIN && bnd_y2>REAL_MIN)
    {
     Bounds->X=bnd_x1;
     Bounds->Width=(Aggplus::REAL)(bnd_x2-bnd_x1 +2.0*font->get_1_6_em()); //Eh!
     Bounds->Y=bnd_y1;
     Bounds->Height=bnd_y2-bnd_y1;
    }
   else
    {
     Bounds->X=Bounds->Y=Bounds->Width=Bounds->Height=0.0f;
    }
  }

 if(pDefault_format) delete(pDefault_format);
 //AddRectangle(layoutRect);
 return Ok;
}

void aggplus_flat_add_string_byfont_singleline(GraphicsPath *path, const WCHAR* string, int length, const Font* font, const PointF &origin)
{
 if(!path) { assert(FALSE);return; }
 if(!font || !font->IsAvailable()) { assert(FALSE);return; }

 //z_Convert_wcharp_to_charp cText(string);
 //if(length<0) length=cText.m_nLength;
 if(length<0) length=(int)wcslen(string);
 //const char* p = cText.m_pcStr;
 const WCHAR* p = string;

 font_manager_type fman(*(font->get_agg_font_engine()));

 // Pipeline to process the vectors glyph paths (curves + contour)
 agg::conv_curve<font_manager_type::path_adaptor_type> m_curves(fman.path_adaptor());
 agg::conv_contour<agg::conv_curve<font_manager_type::path_adaptor_type> > m_contour(m_curves);

 double m_weight_value=0.00;
 double f_em_height=font->GetEmHeight();
 double f_em_linespacing = font->GetLineSpacing();
 double f_em_ascent = font->GetCellAscent();
 double f_font_size=font->get_res_unit_to_pixel()*font->GetSize();
 double f_line_Advance=f_font_size * f_em_linespacing / f_em_height;

 double x0 = origin.X+ font->get_1_6_em(); //for :) 1/6em see http://support.microsoft.com/default.aspx?scid=kb;en-us;307208
 double y0 = origin.Y+ f_font_size*f_em_ascent/f_em_height;
 {
 double x = x0;
 double y = y0;
 int nLines=0;
 while(*p && length)
  {
   const agg::glyph_cache* glyph = fman.glyph(*p);
   if(glyph)
    {
     //TRACE("%04X\n", *p);
     assert(glyph->data_type==agg::glyph_data_outline);
     // increment pen position
     switch(*p)
      {
       case L'\r': assert(FALSE); //Invalid, should be single line
            break;
       case RET: assert(FALSE);  //Invalid, should be single line
            nLines++;
            x = x0;
            y=y0+double(nLines)*f_line_Advance;
            break;
       default:
        fman.add_kerning(&x, &y);

        //It initializes the embedded adaptors that interpret glyph data.
        //You don't need to do it unless you need to actually render the glyph.
        fman.init_embedded_adaptors(glyph, x, y); 
        /* 2.3
        if(fabs(m_weight_value) <= 0.01) { path->z_get_agg_path_storage()->add_path(m_curves, 0, false); }
        else                             { path->z_get_agg_path_storage()->add_path(m_contour, 0, false); }
        */

        if(fabs(m_weight_value) <= 0.01) { path->z_get_agg_path_storage()->concat_path(m_curves, 0); }
        else                             { path->z_get_agg_path_storage()->concat_path(m_contour, 0); }

        x += glyph->advance_x;
        y += glyph->advance_y;
        break;
      }
     
     
    }
   ++p;
   length--;
  }
    
 }
  
}

}

//Changes
//0.1.0 27-10-2004
//GraphicsPath::AddStringFontFormatted with StringAlignment
//Font::z_measure_fit_chars changes - stripping of spaces at the end
//0.1.1 03-11-2004
//GraphicsPath::GetPointCount
//GraphicsPath::GetPathPoints
//GraphicsPath::AddBezier
//GraphicsPath::AddArc fixed
//GraphicsPath::AddEllipse fixed
//0.1.2 07-11-2004
//GraphicsPath::AddStringFont
//added flat func. aggplus_flat_add_string_byfont_singleline
//added flat func. aggplus_flat_add_string_byfont_getbounds
