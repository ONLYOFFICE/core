// Graphics.h: interface for the Graphics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_GRAPHICS_H__6C05FEC0_22F3_490D_ABD4_3205EA5DCA7A__INCLUDED_)
#define Aggplus_GRAPHICS_H__6C05FEC0_22F3_490D_ABD4_3205EA5DCA7A__INCLUDED_

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"
#include "ap_Color.h"
#include "ap_Matrix.h"
#include "ap_StringFormat.h"

#pragma warning(disable : 4100)

#include "include\agg_basics.h"

#include "include\agg_color_rgba.h"
#include "include\agg_pixfmt_rgba.h"
#include "include\agg_rasterizer_scanline_aa.h"
#include "include\agg_renderer_scanline.h"
#include "include\agg_scanline_u.h"


#include "svg/agg_svg_exception.h"
#include "svg/agg_svg_parser.h"
#include "svg/agg_svg_indexation_interpreter.h"
#include "svg/agg_svg_rendering_interpreter.h"
#include "svg/agg_svg_attributes_map.h"
#include "svg/agg_svg_rasterizer.h"


typedef agg::rendering_buffer rendering_buffer_type;
typedef agg::pixfmt_rgba32 pixformat_type;

typedef agg::blender_rgba< agg::svg::color_type, agg::svg::component_order >                                blender_type;
typedef agg::pixfmt_alpha_blend_rgba< blender_type, agg::rendering_buffer, agg::svg::pixel_type > pixfmt_type;

typedef agg::renderer_base<pixfmt_type> base_renderer_type;
typedef agg::scanline_u8 scanline_type;

#define def_flag_G_ExtBuffer 0x0001
#define def_flag_G_FromHDC   0x0002


namespace Aggplus
{
		class GraphicsPath;
		class Pen;
		class Brush;
		class BrushLinearGradient;
		class Image;
		class SVGImage;
		//class Bitmap;
		class StringFormat;
		class Font;

		class Graphics  
		{
		public:
			Graphics(DWORD dwWidth, DWORD dwHeight, int stride = 0, LPVOID pMemBuffer = NULL);
			Graphics(Image* image);
			Graphics(HDC hdc);
			static Graphics *FromHDC(HDC hdc);

			virtual ~Graphics();

			static Graphics *FromImage(Image *image);
		
			Status Clear(const Color &color);

			Status DrawPath(const Pen* pen, const GraphicsPath* path); 

			Status FillPath(const Brush* brush, const GraphicsPath* path);

			Status FillRectangle(const Brush* brush, REAL x, REAL y, REAL width, REAL height);
			Status FillRectangle(const Brush* brush, const RectF& rect) { return FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height); }

			void DrawRectangle(const Pen* pen, float x, float y, float width, float height);
			void DrawRectangle(const Pen* pen, const RectF& rect) { DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height); }

			Status DrawBezier(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
			Status DrawBezier(const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4)
				{ return DrawBezier(pen, pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, pt4.X,pt4.Y); }

			void   DrawImage(Image* image, float x, float y, float srcx, float srcy, float srcwidth, float srcheight, Unit srcUnit=UnitPixel);
			Status DrawImage(Image* image, REAL x, REAL y, REAL width, REAL height);

			Status DrawSVGImage(SVGImage* image, REAL x, REAL y, REAL width, REAL height);

			//Draw Line
			Status DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2);
			Status DrawLine(const Pen* pen, const PointF& pt1, const PointF& pt2) { return DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y); }
			 
			void ResetTransform() { m_MatrixGfx.Reset(); }
			void GetTransform(Matrix* matrix) { (*matrix)=m_MatrixGfx; }
			void SetTransform(const Matrix* matrix) { m_MatrixGfx=(*matrix); }

			void TranslateTransform(float dx, float dy, MatrixOrder order=MatrixOrderPrepend) { m_MatrixGfx.Translate(dx, dy, order); }
			void ScaleTransform(float sx, float sy, MatrixOrder order=MatrixOrderPrepend) { m_MatrixGfx.Scale(sx, sy, order); }
			void RotateTransform(float angle, MatrixOrder order=MatrixOrderPrepend) { m_MatrixGfx.Rotate(angle, order); }

			Status DrawString(const WCHAR *string, INT length, const Font *font, const PointF& origin, const Brush *brush)
			{
				RectF rect(origin.X, origin.Y, 0.0f, 0.0f);
				StringFormat stringFormat; //default
				return DrawString(string, length, font, rect, &stringFormat, brush);
			}

			Status DrawString(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *stringFormat, const Brush *brush);
			Status DrawString(const WCHAR *string, INT length, const Font *font, const PointF &origin, const StringFormat *stringFormat, const Brush *brush)
			{
				RectF rect(origin.X, origin.Y, 0.0f, 0.0f);
				return DrawString(string, length, font, rect, stringFormat, brush);
			}

			Status MeasureString(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *stringFormat, RectF *boundingBox,
							INT *codepointsFitted = 0, INT *linesFilled = 0) const;
			Status MeasureString(const WCHAR *string, INT length, const Font *font, const SizeF &layoutRectSize, const StringFormat *stringFormat, SizeF *size,
							INT *codepointsFitted = 0, INT *linesFilled = 0) const
			{
				RectF layoutRect(0, 0, layoutRectSize.Width, layoutRectSize.Height);
				RectF   boundingBox;
				Status  status;
				if(!size) 
					{ return InvalidParameter; }
				status = MeasureString(string, length, font, layoutRect, stringFormat, &boundingBox, codepointsFitted, linesFilled);
				if(size && status == Ok) 
					{ size->Width  = boundingBox.Width; size->Height = boundingBox.Height; }
				return status;
			}
			Status MeasureString(const WCHAR *string, INT length, const Font *font, const PointF &origin, const StringFormat *stringFormat, RectF *boundingBox) const
			{
				RectF rect(origin.X, origin.Y, 0.0f, 0.0f);
				return MeasureString(string, length, font, rect, stringFormat, boundingBox, NULL, NULL);
			}
			Status MeasureString(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, RectF *boundingBox) const
			{ 
				return MeasureString(string, length, font, layoutRect, NULL, boundingBox, NULL, NULL); 
			}
			Status MeasureString(const WCHAR *string, INT length, const Font *font, const PointF &origin, RectF *boundingBox) const
			{
				RectF rect(origin.X, origin.Y, 0.0f, 0.0f);
				return MeasureString(string, length, font, rect, NULL, boundingBox, NULL, NULL);
			}

			Unit GetPageUnit(void) const { return(m_PageUnit); }

			//Dummy methods
			Status SetTextRenderingHint(TextRenderingHint newMode) { return Ok; }
			Status SetSmoothingMode(SmoothingMode smoothingMode) { return Ok; }
			Status SetCompositingQuality(CompositingQuality compositingQuality) { return Ok; }
			Status SetInterpolationMode(InterpolationMode interpolationMode) { return Ok; }

		protected:
			void Create(DWORD dwWidth, DWORD dwHeight, int stride, LPVOID pMemBuffer);
			void DoFillPath(const Brush* Brush);
			void DoFillPathSolid(Color dwColor);
			void DoFillPathGradient(BrushLinearGradient *pBrush);
			void DoFillPathTextureClampSz(const Matrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride);
			
			void zBuffer_Begin();
			void zBuffer_End();
		public:
			void ClipBoxSet(const RectF& rect);
			void ClipBoxGet(RectF *prect) { (*prect)=m_ClipRect; }
			void ClipBoxReset() { m_ClipRect.X=m_ClipRect.Y=m_ClipRect.Width=m_ClipRect.Height=0.00; }

		protected:
			DWORD m_dwConfigFlags;
			DWORD m_dwWidth, m_dwHeight;
			HDC   m_fromhdc_HDC;
			int   m_fromhdc_X, m_fromhdc_Y;

			Unit   m_PageUnit;
			Matrix m_MatrixGfx;  
			
			RectF  m_ClipRect;    //Simple clipping rect
			Matrix m_ClipMatrix; //Clipping rect matrix
			
			agg::svg::frame_buffer_rgba       m_frame_buffer;
			agg::svg::rasterizer              m_rasterizer;

		public:

			agg::svg::frame_buffer_rgba&   get_frame_buffer() {return m_frame_buffer;}
			agg::svg::rasterizer&          get_rasterizer()   {return m_rasterizer;}

		};


} //namespace Aggplus

#endif // !defined(Aggplus_GRAPHICS_H__6C05FEC0_22F3_490D_ABD4_3205EA5DCA7A__INCLUDED_)
