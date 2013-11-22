//----------------------------------------------------------------------------
// Graphics class  - Version 0.1.2 - 01-04-2006
// Marlon Mircevski
//----------------------------------------------------------------------------
#include "stdafx.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4097)

#include "agg_pixfmt_rgba.h"
#include "agg_conv_curve.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_conv_transform.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_pattern_rgba.h"
#include "agg_image_accessors.h"
#include "agg_span_allocator.h"

typedef agg::span_interpolator_linear<> interpolator_type_linear;
typedef agg::span_allocator<agg::rgba8> span_alloc_type;

#include <math.h>
#include "ap_Graphics.h"
#include "ap_GraphicsPath.h"
#include "ap_Pen.h"
#include "ap_Brush.h"
#include "ap_Font.h"

//if this is defined, you cannot mix simple Gdi and Aggplus drawing calls
//but code works faster
#define def_AP_Update_Dc_at_End

namespace Aggplus
{

		Graphics::Graphics(DWORD dwWidth, DWORD dwHeight, int stride, LPVOID pMemBuffer) : m_dwConfigFlags(0)
		{
			Create(dwWidth, dwHeight, stride, pMemBuffer);
		}

		Graphics::Graphics(Image* image) : m_dwConfigFlags(0)
		{
			if(!image || image->GetLastStatus()!=Ok) 
			{ 
				assert(FALSE); return; 
			}
			Create(image->GetWidth(), image->GetHeight(), image->m_nStride, image->m_pImgData);
			assert(m_dwConfigFlags&def_flag_G_ExtBuffer); //Just check...
		}

		Graphics::Graphics(HDC hdc) : m_dwConfigFlags(0)
		{
			RECT clipBox;
			::GetClipBox(hdc, &clipBox);
			int nW=clipBox.right-clipBox.left;
			int nH=clipBox.bottom-clipBox.top;
			assert(nW>0 && nH>0);
			Create(nW, nH, -4*nW, NULL);
			m_dwConfigFlags|=def_flag_G_FromHDC;
			m_fromhdc_HDC=hdc;
			m_fromhdc_X=clipBox.left;
			m_fromhdc_Y=clipBox.top;
		}

		Graphics *Graphics::FromHDC(HDC hdc) { return(new Graphics(hdc)); }

		void Graphics::Create(DWORD dwWidth, DWORD dwHeight, int stride, LPVOID pMemBuffer)
		{
			m_PageUnit=UnitDisplay;

			m_dwWidth=dwWidth;
			m_dwHeight=dwHeight;
			if(pMemBuffer)
			{
				m_dwConfigFlags|=def_flag_G_ExtBuffer; //External buffer, no delete on destructor
			}
			 
			m_frame_buffer.create(m_dwWidth, m_dwHeight, false, stride, pMemBuffer);
			
			m_fromhdc_HDC=NULL;
			m_fromhdc_X=m_fromhdc_Y=0;

			m_rasterizer.get_rasterizer().gamma(agg::gamma_none());
		}

		Graphics::~Graphics()
		{
			#ifdef def_AP_Update_Dc_at_End
				if(m_dwConfigFlags & def_flag_G_FromHDC)
				{
					BITMAPINFO hDib;
					memset(&hDib, 0, sizeof(BITMAPINFO));
					hDib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					hDib.bmiHeader.biWidth = m_dwWidth;
					hDib.bmiHeader.biHeight = m_dwHeight;
					hDib.bmiHeader.biPlanes = 1;
					hDib.bmiHeader.biBitCount = 32;
					hDib.bmiHeader.biCompression = BI_RGB;
					hDib.bmiHeader.biSizeImage = m_dwWidth * m_dwHeight * 4;
					#ifndef _X_NODCAlpha_
						LPVOID pBits;
						HBITMAP hBitmap = CreateDIBSection(m_fromhdc_HDC, &hDib, DIB_RGB_COLORS, &pBits, NULL, 0);

						if(!hBitmap) 
							assert(FALSE);
						else
						{
							memcpy(pBits, m_frame_buffer.get_internal_buf(), m_dwWidth * m_dwHeight * 4);
							HDC mdc=CreateCompatibleDC( m_fromhdc_HDC );
							if(!mdc) 
								assert(FALSE);
							else
							{
								HBITMAP hbmOld=(HBITMAP)::SelectObject(mdc, hBitmap);
								BLENDFUNCTION bln;
								bln.BlendOp=AC_SRC_OVER;
								bln.BlendFlags=0;
								bln.SourceConstantAlpha=255;
								bln.AlphaFormat=AC_SRC_ALPHA;
								AlphaBlend(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight, mdc, 0, 0, m_dwWidth, m_dwHeight, bln);
								//BitBlt(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight, mdc, 0, 0, SRCCOPY);
								::SelectObject(mdc, hbmOld);
								//::GdiFlush();
								DeleteDC(mdc);
							}
							::DeleteObject(hBitmap);
						}
					#else
						SetDIBitsToDevice(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight,
												0, 0, 0, m_dwHeight,
												m_frame_buffer.get_internal_buf(), &hDib, DIB_RGB_COLORS);
					#endif //_X_NODCAlpha_
				}
			#endif //def_AP_Update_Dc_at_End
			  
		}

		Graphics *Graphics::FromImage(Image *image) { return(new Graphics(image)); }

		void Graphics::zBuffer_Begin()
		{
		//Experimental!
		#ifndef def_AP_Update_Dc_at_End
			if(m_dwConfigFlags & def_flag_G_FromHDC)
			{
				m_frame_buffer.ren_base().clear(agg::rgba8(0,0,0,0));
			}
		#endif //!def_AP_Update_Dc_at_End
		}

		void Graphics::zBuffer_End()
		{
		//Experimental!
		#ifndef def_AP_Update_Dc_at_End
			if(m_dwConfigFlags & def_flag_G_FromHDC)
			{
					BITMAPINFO hDib;
					memset(&hDib, 0, sizeof(BITMAPINFO));
					hDib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					hDib.bmiHeader.biWidth = m_dwWidth;
					hDib.bmiHeader.biHeight = m_dwHeight;
					hDib.bmiHeader.biPlanes = 1;
					hDib.bmiHeader.biBitCount = 32;
					hDib.bmiHeader.biCompression = BI_RGB;
					hDib.bmiHeader.biSizeImage = m_dwWidth * m_dwHeight * 4;
					#ifndef _X_NODCAlpha_
						LPVOID pBits;
						HBITMAP hBitmap = CreateDIBSection(m_fromhdc_HDC, &hDib, DIB_RGB_COLORS, &pBits, NULL, 0);

						if(!hBitmap) 
							assert(FALSE);
						else
						{
							memcpy(pBits, m_frame_buffer.get_internal_buf(), m_dwWidth * m_dwHeight * 4);
							HDC mdc=CreateCompatibleDC( m_fromhdc_HDC );
							if(!mdc) 
								assert(FALSE);
							else
							{
								HBITMAP hbmOld=(HBITMAP)::SelectObject(mdc, hBitmap);
								BLENDFUNCTION bln;
								bln.BlendOp=AC_SRC_OVER;
								bln.BlendFlags=0;
								bln.SourceConstantAlpha=255;
								bln.AlphaFormat=AC_SRC_ALPHA;
								AlphaBlend(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight, mdc, 0, 0, m_dwWidth, m_dwHeight, bln);
								//BitBlt(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight, mdc, 0, 0, SRCCOPY);
								::SelectObject(mdc, hbmOld);
								//::GdiFlush();
								DeleteDC(mdc);
							}
							::DeleteObject(hBitmap);
						}
					#else
						SetDIBitsToDevice(m_fromhdc_HDC, m_fromhdc_X, m_fromhdc_Y, m_dwWidth, m_dwHeight,
													0, 0, 0, m_dwHeight,
													m_frame_buffer.get_internal_buf(), &hDib, DIB_RGB_COLORS);
					#endif //_X_NODCAlpha_
			}
		#endif //!def_AP_Update_Dc_at_End

		}

		Status Graphics::Clear(const Color &color)
		{
			if(m_ClipRect.IsEmptyArea())
			{
				zBuffer_Begin();//for fromHDC
				m_frame_buffer.ren_base().clear(color.GetAggColor());
				zBuffer_End(); //for fromHDC
			}
			else
			{
				BrushSolid sbrush(color);
				GraphicsPath Path;
				RectF dct(0,0,(float)m_dwWidth,(float)m_dwHeight);
				Path.AddRectangle(dct);
				FillPath(&sbrush, &Path);
			}
			return Ok;
		}

		Status Graphics::DrawPath(const Pen* pen, const GraphicsPath* path) //ZTODO: const GraphicsPath* path
		{
			//TRACE("DrawPath\n");
			assert(pen);
			m_rasterizer.get_rasterizer().reset();
			//butt_cap, square_cap, round_cap
			//pg.line_cap(agg::vcgen_stroke::round_cap);

			agg::line_join_e LineJoin;
			switch(pen->GetLineJoin())
			{
				case LineJoinMiter       : LineJoin=agg::miter_join; break;
				case LineJoinBevel       : LineJoin=agg::bevel_join; break;
				default:
				case LineJoinRound       : LineJoin=agg::round_join; break;
				case LineJoinMiterClipped: LineJoin=agg::miter_join_revert; break;
			}

			REAL fWidth=pen->GetWidth();
			double dblMiterLimit=0.50;
			
			agg::path_storage path_copy(path->m_agg_ps);
			typedef agg::conv_curve<agg::path_storage> conv_crv_type;
			
			conv_crv_type c_c_path(path_copy);
			 

			if(pen->GetDashStyle()==DashStyleSolid)
			{
				typedef agg::conv_stroke<conv_crv_type> Path_Conv_StrokeN;
				Path_Conv_StrokeN pgN(c_c_path);
				//typedef agg::conv_stroke<agg::path_storage> Path_Conv_StrokeN;
				//Path_Conv_StrokeN pgN(path->m_agg_ps);
				pgN.line_join(LineJoin);
				pgN.miter_limit(dblMiterLimit);
				pgN.width(fWidth);
				typedef agg::conv_transform<Path_Conv_StrokeN> transStroke;
				transStroke trans(pgN, m_MatrixGfx.m_agg_mtx);
				m_rasterizer.get_rasterizer().add_path(trans);
			}
			else
			{
				typedef agg::conv_dash<conv_crv_type> Path_Conv_Dash;
				Path_Conv_Dash poly2_dash(c_c_path);
				

				typedef agg::conv_stroke<Path_Conv_Dash> Path_Conv_StrokeD;
				Path_Conv_StrokeD pgD(poly2_dash);

				switch(pen->GetDashStyle())
					{
					case DashStyleDash:
						poly2_dash.add_dash(3.00*fWidth, fWidth);
						break;
					case DashStyleDot:
						poly2_dash.add_dash(fWidth, fWidth);
						break;
					case DashStyleDashDot:
						poly2_dash.add_dash(3.00*fWidth, fWidth);
						poly2_dash.add_dash(fWidth, fWidth);
						break;
					case DashStyleDashDotDot:
						poly2_dash.add_dash(3.00*fWidth, fWidth);
						poly2_dash.add_dash(fWidth, fWidth);
						poly2_dash.add_dash(fWidth, fWidth);
						break;
					default:
					case DashStyleCustom:break;
					}
				pgD.line_join(LineJoin);
				pgD.miter_limit(dblMiterLimit);
				pgD.width(fWidth);

				agg::conv_transform<Path_Conv_StrokeD> trans(pgD, m_MatrixGfx.m_agg_mtx);
				m_rasterizer.get_rasterizer().add_path(trans);
			}

			DoFillPath(pen->GetBrush());
			return Ok;
		}

		Status Graphics::FillPath(const Brush* brush, const GraphicsPath* path)
		{
			assert(brush);
			m_rasterizer.get_rasterizer().reset();

			agg::path_storage p2(path->m_agg_ps);
			typedef agg::conv_transform<agg::path_storage> trans_type;
			trans_type trans(p2, m_MatrixGfx.m_agg_mtx);

			typedef agg::conv_curve<trans_type> conv_crv_type;
			conv_crv_type c_c_path(trans);
			 
			m_rasterizer.get_rasterizer().add_path(c_c_path);
			DoFillPath(brush);
			return Ok;
		}

		void Graphics::DoFillPathSolid(Color dwColor)
		{
				zBuffer_Begin();//for fromHDC
				//###
				typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
				solid_renderer_type ren_fine(m_frame_buffer.ren_base());
				ren_fine.color(dwColor.GetAggColor());
				agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ren_fine);

				//ZTODO: Region Clipping
				/*
				if(m_RegionClip.Dali_e_prazen()) { agg::render_scanlines(m_agg_RAS, SL, ren_fine); }
				else //DoClip
				{
					{
					//Test View Path
					rasterizer_type ras2;
					ras2.gamma(agg::gamma_none());
					agg::path_storage CLpath(m_RegionClip.m_agg_ps);
					agg::trans_affine mtx;
					mtx.multiply(*m_MatrixGfx.m_pmtx);
					agg::conv_transform<agg::path_storage> mxpath(CLpath, mtx);

					ras2.add_path(mxpath);
					//ras2.add_path(CLpath);
					agg::render_scanlines(ras2, sl, ren_fine);
					}
				     

				//scanline_type sl;
				scanline_type sl1;
				scanline_type sl2;
				agg::scanline_storage_aa8 storage;
				agg::scanline_storage_aa8 storage1;
				agg::scanline_storage_aa8 storage2;

				agg::render_scanlines(m_agg_RAS, sl1, storage1);

					{
					//Path
					rasterizer_type ras2;
					ras2.gamma(agg::gamma_none());
				     
					agg::path_storage CLpath(m_RegionClip.m_agg_ps);
					agg::trans_affine mtx;
					mtx.multiply(m_RegionMatrix.m_agg_mtx);//;(*m_MatrixGfx.m_pmtx);
					agg::conv_transform<agg::path_storage> mxpath(CLpath, mtx);

					ras2.add_path(mxpath);
					//ras2.add_path(CLpath);
					agg::render_scanlines(ras2, sl2, storage2);
					}

					agg::sbool_op_e op=agg::sbool_and;
					//if(m_xClip->m_boolMode==Region::boolModeExclude) { op=agg::sbool_a_minus_b; }

					agg::sbool_combine_shapes_aa(op, storage1, storage2, sl1, sl2, SL, storage);
					agg::render_scanlines(storage, SL, ren_fine);
				     
					}
					*/
				zBuffer_End(); //for fromHDC
		}

		void Graphics::DoFillPathGradient(BrushLinearGradient *pBrush)
		{
			zBuffer_Begin();//for fromHDC
			//###
			int mins[2];
			int maxs[2];
			mins[0] = m_rasterizer.get_rasterizer().min_x();
			mins[1] = m_rasterizer.get_rasterizer().min_y();
			maxs[0] = m_rasterizer.get_rasterizer().max_x();
			maxs[1] = m_rasterizer.get_rasterizer().max_y();
			int WR = maxs[0]-mins[0];
			int HR = maxs[1]-mins[1];

			RectF Rct((float)mins[0], (float)mins[1], (float)WR, (float)HR);      //pBrush->GetRectangle(&Rct);
			
			Matrix mGrdMtx;

			float brRot = pBrush->GetRotate();
			if (WR>0 && brRot>0.0f)
			{
				int nAng=int(brRot)%360;
				if((nAng>=90 && nAng<=180) || (nAng>=270 && nAng<=360))
				{
					mGrdMtx.Translate(WR, 0.00);//, MatrixOrderAppend);
					WR=-WR;
				}
				double dblAng=agg::deg2rad(brRot);
				double Dx=WR*cos(dblAng)+HR*sin(dblAng);
				float fScale=(float)(fabs(Dx/WR));

				mGrdMtx.Rotate(brRot);
				mGrdMtx.Scale(fScale, fScale);
			}

			//mGrdMtx.Scale(0.2, 1.0);

			Matrix mImgMtx;
			mImgMtx.Translate(Rct.X, Rct.Y);
			mImgMtx.Multiply(&mGrdMtx);

			double dblStart=0.0;
			double dblEnd=Rct.Width;

			agg::trans_affine mtx_Work(mImgMtx.m_agg_mtx);
			mtx_Work.multiply(m_MatrixGfx.m_agg_mtx);
			mtx_Work.invert();
			interpolator_type_linear span_interpolator(mtx_Work); // Span interpolator

			span_alloc_type span_allocator;                  // Span Allocator
			typedef agg::gradient_reflect_adaptor<agg::gradient_radial> gr_adaptor;
			agg::gradient_radial gradient_x;
			gr_adaptor gr_x(gradient_x);
			gr_adaptor* gr_ptr = &gr_x;
			typedef agg::span_gradient<agg::rgba8, 
										interpolator_type_linear,        
										gr_adaptor, 
										BrushLinearGradient > gradient_span_gen;
			typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
			gradient_span_alloc    span_alloc;
			//agg::gradient_linear_color<agg::rgba8> colors(pBrush->m_dwColorA.GetAggColor(), pBrush->m_dwColorB.GetAggColor());
			gradient_span_gen span_gen(span_interpolator, *gr_ptr, *pBrush, dblStart, dblEnd);
			typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
			// The gradient renderer
			renderer_gradient_type ren_gradient(m_frame_buffer.ren_base(), span_alloc, span_gen);
			agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ren_gradient);
			zBuffer_End(); //for fromHDC
		}


		void Graphics::DoFillPathTextureClampSz(const Matrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride)
		{
			assert((unsigned)abs(nImgStride)>=4*dwImgWidth);
			zBuffer_Begin();//for fromHDC

			span_alloc_type span_allocator;                  // Span Allocator

			typedef agg::pixfmt_rgba32     pixfmt;
			typedef agg::image_accessor_clip<pixfmt> img_source_type;
			typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
			typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
			agg::trans_affine mtx_Work(mImgMtx.m_agg_mtx);
			mtx_Work.multiply(m_MatrixGfx.m_agg_mtx);
			mtx_Work.invert();
			interpolator_type_linear interpolator(mtx_Work);
			{
				agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
				pixfmt          img_pixf(PatRendBuff);
				img_source_type img_src(img_pixf, agg::rgba(0, 0, 0, 0));
				span_gen_type sg(img_src, interpolator);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
			}
			 

			zBuffer_End(); //for fromHDC
		}

		void Graphics::DoFillPath(const Brush* Brush)
		{
			if(!Brush) 
			{ assert(FALSE); return; }

			if(Brush->m_bType==BrushTypeSolidColor)
			{
				Color clr;((BrushSolid *)Brush)->GetColor(&clr);
				DoFillPathSolid(clr);
			}
			else 
				if(Brush->m_bType==BrushTypeHatchFill)
				{
					assert(FALSE); //ZTODO
				}
				else 
					if(Brush->m_bType==BrushTypeTextureFill)
					{
						BrushTexture *ptxBrush=(BrushTexture *)Brush;
						LPVOID pImgBuff=ptxBrush->PatternFinalize();
						if(pImgBuff)
						{
						 DWORD dwImgWidth=ptxBrush->PatternGetWidth();
						 DWORD dwImgHeight=ptxBrush->PatternGetHeight();
						 if(pImgBuff && dwImgWidth && dwImgHeight)
						 {
							if(ptxBrush->m_wrapMode==WrapModeClamp)
							{
								DoFillPathTextureClampSz(ptxBrush->m_mtx, pImgBuff, dwImgWidth, dwImgHeight, ptxBrush->PatternGetStride());
							}
							else
							{
								agg::trans_affine mtx_Work(ptxBrush->m_mtx.m_agg_mtx);
								mtx_Work.multiply(m_MatrixGfx.m_agg_mtx);
								mtx_Work.invert();

								span_alloc_type span_allocator;                  // Span Allocator
								interpolator_type_linear interpolator(mtx_Work);
						  
								agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, dwImgWidth * 4);

								zBuffer_Begin();//for fromHDC

								if(ptxBrush->m_wrapMode==WrapModeTileFlipX)
								{
										typedef agg::pixfmt_rgba32     pixfmt;
										//image_accessor_wrap
										typedef agg::wrap_mode_reflect wrap_x_type;
										typedef agg::wrap_mode_repeat wrap_y_type;
										typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
										typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
										typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

										pixfmt          img_pixf(PatRendBuff);
										img_source_type img_src(img_pixf);
										span_gen_type sg(img_src, interpolator);
										renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
										agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
								}
								else 
									if(ptxBrush->m_wrapMode==WrapModeTileFlipY)
									{
											typedef agg::pixfmt_rgba32     pixfmt;
											//image_accessor_wrap
											typedef agg::wrap_mode_repeat wrap_x_type;
											typedef agg::wrap_mode_reflect wrap_y_type;
											typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
											typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
											typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

											pixfmt          img_pixf(PatRendBuff);
											img_source_type img_src(img_pixf);
											span_gen_type sg(img_src, interpolator);
											renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
											agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
									}
									else 
										if(ptxBrush->m_wrapMode==WrapModeTileFlipXY)
										{
												typedef agg::pixfmt_rgba32     pixfmt;
												//image_accessor_wrap
												typedef agg::wrap_mode_reflect wrap_x_type;
												typedef agg::wrap_mode_reflect wrap_y_type;
												typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
												typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
												typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

												pixfmt          img_pixf(PatRendBuff);
												img_source_type img_src(img_pixf);
												span_gen_type sg(img_src, interpolator);
												renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
												agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
										}
										else //Repeat
										{
												typedef agg::pixfmt_rgba32     pixfmt;
												//image_accessor_wrap
												typedef agg::wrap_mode_repeat wrap_x_type;
												typedef agg::wrap_mode_repeat wrap_y_type;
												typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
												typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
												typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

												pixfmt          img_pixf(PatRendBuff);
												img_source_type img_src(img_pixf);
												span_gen_type sg(img_src, interpolator);
												renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
												agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
										}

									zBuffer_End(); //for fromHDC
								}
							}
						}
				}
				else 
					if(Brush->m_bType==BrushTypePathGradient)
					{
						assert(FALSE); //ZTODO
					}
					else 
						if(Brush->m_bType==BrushTypeLinearGradient)
						{
							DoFillPathGradient((BrushLinearGradient *)Brush);
						}

		}

		void Graphics::ClipBoxSet(const RectF& rect)
		{
				m_ClipRect=rect; m_ClipMatrix=m_MatrixGfx;
				if(!m_ClipRect.IsEmptyArea())
				{
					double x1=m_ClipRect.X;
					double y1=m_ClipRect.Y;
					double x2=x1+m_ClipRect.Width;
					double y2=y1+m_ClipRect.Height;
					m_ClipMatrix.m_agg_mtx.transform(&x1, &y1);
					m_ClipMatrix.m_agg_mtx.transform(&x2, &y2);
					m_rasterizer.get_rasterizer().clip_box(x1, y1, x2, y2);
				}
				else
				{
					m_rasterizer.get_rasterizer().reset_clipping();
				}
		}

		Status Graphics::FillRectangle(const Brush* brush, REAL x, REAL y, REAL width, REAL height)
		{
				GraphicsPath Path;
				RectF rct(x, y, width, height);
				Path.AddRectangle(rct);
				return FillPath(brush, &Path);
		}

		void Graphics::DrawRectangle(const Pen* pen, float x, float y, float width, float height)
		{
				GraphicsPath Path;
				RectF rct(x, y, width, height);
				Path.AddRectangle(rct);
				DrawPath(pen, &Path);
		}

		Status Graphics::DrawBezier(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4)
		{
				GraphicsPath Path;
				Path.AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);
				return DrawPath(pen, &Path);
		}

		Status Graphics::DrawImage(Image* image, REAL x, REAL y, REAL width, REAL height)
		{
				if(!image || image->GetLastStatus()!=Ok) return UnknownImageFormat;
				if(width==0.00 || height==0.00) return InvalidParameter;
				m_rasterizer.get_rasterizer().reset();

				agg::path_storage p2;
				p2.move_to(x, y);p2.line_to(x+width, y);p2.line_to(x+width, y+height);p2.line_to(x, y+height);p2.close_polygon();

				agg::conv_transform<agg::path_storage> trans(p2, m_MatrixGfx.m_agg_mtx);  // Global Affine transformer

				m_rasterizer.get_rasterizer().add_path(trans);

				void *pImgBuff=image->m_pImgData;
				DWORD dwImgWidth=image->GetWidth();
				DWORD dwImgHeight=image->GetHeight();

				Matrix ImgMtx;
				ImgMtx.Translate(x, y);
				ImgMtx.Scale(width/float(dwImgWidth), height/float(dwImgHeight));
				DoFillPathTextureClampSz(ImgMtx, pImgBuff, dwImgWidth, dwImgHeight, image->m_nStride);

				return(Ok);
		}

		Status Graphics::DrawSVGImage(SVGImage* image, REAL x, REAL y, REAL width, REAL height)
		{
			agg::svg::renderer_rgba   renderer(get_rasterizer(), get_frame_buffer());

			get_rasterizer().gamma(1.0);
			image->m_settings.gamma(1.0);

			image->m_attributes.window(x, y,  2*(x+width), 2*(y+height));

			agg::svg::rendering_interpreter rin(  image->m_pipeline, image->m_attributes, renderer, 
												  image->m_id2elem_map,image->m_gradient_lut_cache);
			//#ifdef EXPAND_PATHS
			//	rin.expand(expand);
			//#endif
			rin.ignore_viewBox(true);

			image->m_storage.traverse(rin);

			assert(0 == image->m_attributes.num_sessions());

			if (image->m_settings.gamma() != 1.0)
			{
				get_frame_buffer().pixfmt().apply_gamma_inv(image->m_settings.gamma_lut());
			}	

			get_rasterizer().reset_clipping();

			return(Ok);
		}

		//Not Implemented OK, check image dpi!
		void Graphics::DrawImage(Image* image, float x, float y, float srcx, float srcy, float srcwidth, float srcheight, Unit srcUnit)
		{
				if(!image || image->GetLastStatus()!=Ok) return;
				if(srcwidth==0.00 || srcheight==0.00) return;
				m_rasterizer.get_rasterizer().reset();

				agg::path_storage p2;
				p2.move_to(x, y);p2.line_to(x+srcwidth, y);p2.line_to(x+srcwidth, y+srcheight);p2.line_to(x, y+srcheight);p2.close_polygon();

				agg::conv_transform<agg::path_storage> trans(p2, m_MatrixGfx.m_agg_mtx);  // Global Affine transformer
				//ras.reset();
				m_rasterizer.get_rasterizer().add_path(trans);

				void *pImgBuff=image->m_pImgData;
				DWORD dwImgWidth=image->GetWidth();
				DWORD dwImgHeight=image->GetHeight();

				Matrix ImgMtx;
				ImgMtx.Translate(x, y);
				DoFillPathTextureClampSz(ImgMtx, pImgBuff, dwImgWidth, dwImgHeight, image->m_nStride);
		}

		Status Graphics::DrawLine(const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2)
		{
				GraphicsPath Path;
				Path.AddLine(x1, y1, x2, y2);
				return DrawPath(pen, &Path);
		}

		//Cliping
		/*
		void Graphics::tmpSetClip(const RectF& rect)//, CombineMode mode)
		{
		GraphicsPath Path;
		Path.MoveTo(rect.X, rect.Y);
		Path.LineTo(rect.X+rect.Width, rect.Y);
		Path.LineTo(rect.X+rect.Width, rect.Y+rect.Height);
		Path.LineTo(rect.X, rect.Y+rect.Height);
		Path.CloseFigure();
		//SetClip(&Path, CombineModeReplace);
		tmpSetClip(&Path);
		}
		*/
		/*
		void Graphics::tmpSetClip(const GraphicsPath *Path)//, CombineMode mode)
		{
		m_RegionClip=Region(Path);
		m_RegionMatrix=m_MatrixGfx;
		*/
		/*
		switch(mode)
		{
		default:
		case CombineModeReplace:
				ResetClip();
				m_RegionClip=Region(Path);
				break;
		case CombineModeIntersect:
		case CombineModeUnion:       // 2
		case CombineModeXor:         // 3
		case CombineModeComplement:  // 5 (Exclude From)
				assert(FALSE);
				break;
		case CombineModeExclude:     // 4
				m_RegionClip.NapraviPrazen();
				//###

				m_RegionClip=Region(Path);
				m_RegionClip.m_boolMode=Region::boolModeExclude;
				break;
		}  
		TRACE("ZTODO: Graphics::SetClip GraphicsPath\n");
		//assert(FALSE);
		*/
		/*
		}
		*/
		/*
		void Graphics::tmpSetClip(const Region *ClipRegion)//, CombineMode mode)
		{
		m_RegionClip=*ClipRegion;
		m_RegionMatrix=m_MatrixGfx;
		*/
		/*
		assert(mode==CombineModeReplace);
		if(ClipRegion->Dali_e_prazen())
		{

		ResetClip();
		} //prikrpa, necini
		else
		{
		GraphicsPath path;
		path.m_agg_ps=ClipRegion->m_agg_ps;
		SetClip(&path, mode);

		}
		TRACE("ZTODO: Graphics::SetClip:ClipRegion\n");
		*/
		/*
		}
		*/

		/*
		void Graphics::tmpResetClip()
		{
		m_RegionClip.NapraviPrazen();
		}

		void Graphics::tmpGetClip(Region *ClipRegion)
		{
		(*ClipRegion)=m_RegionClip;
		}
		*/

		Status Graphics::DrawString(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *stringFormat, const Brush *brush)
		{
				GraphicsPath Path;
				Path.AddStringFont(string, length, font, layoutRect, stringFormat);
				return FillPath(brush, &Path);
		}

		/*
		//In Work
		Status Graphics::MeasureString_Exact(const WCHAR* string, INT length, const Font* font, const PointF &origin, RectF* bndBoxGP, RectF* bndBoxReal) const
		{
		assert(font);
		RectF fRectReal, fRectBox;
		font->z_MeasureLineRel(string, length, &fRectReal, &fRectBox);

		fRectBox.Offset(origin);
		fRectBox.Width+=(float)font->z_get_1_6_em(); //for :) 1/6em see http://support.microsoft.com/default.aspx?scid=kb;en-us;307208
		fRectBox.Width+=(float)font->z_get_1_6_em(); //for :) 1/6em see http://support.microsoft.com/default.aspx?scid=kb;en-us;307208
		*bndBoxGP=fRectBox;

		fRectReal.Offset(origin);
		*bndBoxReal=fRectReal;

		return Ok;
		}
		*/

		Status Graphics::MeasureString(const WCHAR *string, INT length, const Font *font, const RectF &layoutRect, const StringFormat *stringFormat, RectF *boundingBox,
							INT *codepointsFitted, INT *linesFilled) const
		{
			return aggplus_flat_add_string_byfont_getbounds(NULL, string, length, font, layoutRect, stringFormat, boundingBox, codepointsFitted, linesFilled);
		}



} 
