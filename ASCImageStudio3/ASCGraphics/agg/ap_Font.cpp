
//#include "stdafx.h"
#include "ap_Font.h"

namespace Aggplus
{

		Font::Font()
		{
			Construct(NULL, 0.00, 0, UnitPixel);
		}

		Font::Font(const WCHAR *familyName, REAL emSize, INT style, Unit unit)
		{
			Construct(familyName, emSize, style, unit);//, Unit unit
		}


		void Font::Construct(const WCHAR* familyName, float emSize, int style, Unit unit)//
		{
				m_agg_pFeng=NULL;

				m_femSize=0.00;

				m_hDC=::GetDC(NULL);
				
				if(emSize<1.0f) 
				{ 
					/*assert(FALSE);*/ return; 
				} //Not implemented
				if(!familyName || !(*familyName)) 
				{ 
					/*assert(FALSE);*/ return; 
				} //finished
				m_Unit=unit;

				m_agg_pFeng=new font_engine_type(m_hDC);

				m_agg_pFeng->hinting(true);
				m_agg_pFeng->flip_y(true);
				m_agg_pFeng->height(emSize);

				float ffSize=emSize;

				switch(m_Unit)
				{
				case UnitDisplay   : delete(m_agg_pFeng);m_agg_pFeng=NULL; return; break; //INVALID! 1 -- Variable -- for PageTransform only
				case UnitWorld     :         // 0 -- World coordinate (non-physical unit)
				case UnitPixel     :  break; // 2 -- Each unit is one device pixel.
				default:
				case UnitPoint     : m_agg_pFeng->resolution(96);break; //ffSize=(emSize*96.0)/72.0; break; //100 // 3 -- Each unit is a printer's point, or 1/72 inch.
				
				}

				m_agg_pFeng->width(0.00);
			//m_agg_pFeng->transform(agg::trans_affine_rotation(agg::deg2rad(00.0)));

				agg::glyph_rendering gren = agg::glyph_ren_outline;

				//z_Convert_wcharp_to_charp cFamName(familyName);
				const WCHAR* pSour = familyName;
				int n_Length=WideCharToMultiByte(CP_ACP, 0, pSour, -1, NULL, NULL, NULL, NULL);
				char *pcStr=NULL;
				if(n_Length)
				{
					pcStr=new char[n_Length];
					WideCharToMultiByte(CP_ACP, 0, pSour, -1, pcStr, n_Length, NULL, NULL);
				}

				int nweight_=FW_REGULAR;
				if(style&FontStyleBold) nweight_=FW_BOLD;
				//lstrlen
				BOOL bSucc=m_agg_pFeng->create_font(pcStr /*familyName*/ /*cFamName.m_pcStr*/, gren, ffSize, 0.00, nweight_,
												(style&FontStyleItalic)!=0,
												//(style&FontStyleUnderline)!=0,
												//(style&FontStyleStrikeout)!=0,
												DEFAULT_CHARSET, FF_DONTCARE);
				if(pcStr) { delete(pcStr); }
				//RUSSIAN_CHARSET, ANSI_CHARSET
				//assert(bSucc);
				//if(!bSucc) bSucc=m_agg_pFeng->create_font(cuStr.GetString(), gren, emSize, 0.00, FW_DONTCARE, false, false, false, DEFAULT_CHARSET, FF_DONTCARE);
				if(bSucc)
				{
					int nSave_dc=SaveDC(m_hDC);
					SetMapMode(m_hDC, MM_ISOTROPIC);
					//SetWindowExtEx(m_hDC, 65500, 65500, NULL); //SetViewportExtEx(m_hDC, 100, 100, NULL);
					SetWindowExtEx(m_hDC, 20480, 20480, NULL);
					SetViewportExtEx(m_hDC, 1200, -1200, NULL);

					//Default Values for Arial
					int otmMacAscent=31;
					int otmMacDescent=-7;
					UINT otmMacLineGap=1;
					UINT otmEMSquare=2048;
					LONG tmAscent=34;
					{
						OUTLINETEXTMETRIC Otm;memset(&Otm, 0, sizeof(OUTLINETEXTMETRIC));
						Otm.otmSize=sizeof(OUTLINETEXTMETRIC);
						if(!GetOutlineTextMetrics(m_hDC, sizeof(OUTLINETEXTMETRIC), &Otm)) 
						{ 
							/*assert(FALSE);*/ 
						} //FAILED
						else
						{
							otmMacAscent=Otm.otmMacAscent;
							otmMacDescent=Otm.otmMacDescent;
							otmMacLineGap=Otm.otmMacLineGap;
							otmEMSquare=Otm.otmEMSquare;
							tmAscent=Otm.otmTextMetrics.tmAscent;
						}
					}
				

					int n_mac_Ascn=otmMacAscent;
					int n_mac_Desc=abs(otmMacDescent);
					int n_mac_LGap=otmMacLineGap;

					m_nEmHeight=(unsigned short)otmEMSquare;
					int ntxtAscent=tmAscent;
					m_nCellAscent=(n_mac_Ascn*m_nEmHeight)/ntxtAscent;
					m_nCellDescent=(n_mac_Desc*m_nEmHeight)/ntxtAscent;
					m_nLineSpacing=((n_mac_Ascn+n_mac_Desc+n_mac_LGap)*m_nEmHeight)/ntxtAscent;

					m_femSize=emSize;
					RestoreDC(m_hDC, nSave_dc);
				}
				else
				{
					delete(m_agg_pFeng);m_agg_pFeng=NULL;
				}

		}


		Font::~Font()
		{
			if(m_agg_pFeng) 
				delete(m_agg_pFeng);
			::ReleaseDC(NULL, m_hDC); m_hDC=0;
		}

		BOOL Font::IsAvailable() const
		{
			return(m_agg_pFeng!=NULL);
		}

		REAL Font::GetHeight(REAL dpi) const
		{
			double dblLineSpacing=GetLineSpacing();
			double dblEmHeight=GetEmHeight();
			//double dblResolution=m_agg_pFeng->resolution();
			double dblMult=1.00;
			if(m_Unit!=UnitPixel) 
				dblMult=dpi/72.00;

			return REAL(dblLineSpacing*(m_femSize/dblEmHeight)*dblMult);
		}

		float Font::get_res_unit_to_pixel() const
		{
			switch(m_Unit)
			{
			case UnitPixel:return(1.0f);
			default:
			case UnitPoint:return(float(96.0/72.0));
			}
		}

		void Font::MeasureChars2(const WCHAR* string, int length, const PointF &origin, MeasureCharsRectF *rcArray) const
		{
			if(!IsAvailable()) 
			{ /*assert(FALSE);*/return; }

			if(length<0) 
				length=(long)wcslen(string);

			const WCHAR* p = string;

			unsigned num_glyphs = 0;

			typedef agg::font_cache_manager<font_engine_type> font_manager_type;

			font_manager_type m_fman(*m_agg_pFeng);

			//double m_weight_value=0.00;
			double f_em_height = GetEmHeight();
			double f_em_ascent = GetCellAscent();
			double f_font_size=get_res_unit_to_pixel()*GetSize();

			double m_height_value=f_font_size*f_em_ascent/f_em_height;
			{
					double x = origin.X;//+z_get_1_6_em();
					double y0 = origin.Y+m_height_value;// - m_height_value - 10.0;
					double y = y0;
					 
					//bool bFirst=false;

					while(*p && length)
					{
						const agg::glyph_cache* glyph = m_fman.glyph(*p);
						if(*p!='\n' && glyph)
						{
							m_fman.add_kerning(&x, &y);
							//assert(glyph->data_type==agg::glyph_data_outline);

							//if(!bFirst) { x-=glyph->bounds.x1; bFirst=true; }
							rcArray[num_glyphs].X=float(x+glyph->bounds.x1);//+glyph->bounds.x1);
							//rcArray[num_glyphs].X=float(x+glyph->bounds.x1);//+glyph->bounds.x1);
							rcArray[num_glyphs].Y=float(y+glyph->bounds.y1);
							rcArray[num_glyphs].Width=float(glyph->bounds.x2-glyph->bounds.x1);
							rcArray[num_glyphs].Height=float(glyph->bounds.y2-glyph->bounds.y1);
							rcArray[num_glyphs].StartX=float(x);
							rcArray[num_glyphs].Advance=float(glyph->advance_x);
							// increment pen position
							x += glyph->advance_x;
							y += glyph->advance_y;
							++num_glyphs;
						}
						++p;
						length--;
					}
			    
			}

		}

		//ZTODO: Test new function
		void aggplus_flat_measure_fit_chars(const Font *font, const WCHAR* string, int length, float fMaxWidth, float *pfLinStart, float *pfLinWidth, int *pnLinChars, int *pnAdvanceChars, bool *break_by_newline)
		{
			if(!font->IsAvailable()) 
			{ 
				/*assert(FALSE);*/return; 
			}

			if(length<0) length=(long)wcslen(string);

			if(!length) { *pfLinStart=*pfLinWidth=0.00;*pnLinChars=0; return; }

			*pfLinStart=0.0f;
			*break_by_newline=false;

			const WCHAR* p = string;

			typedef agg::font_cache_manager<font_engine_type> font_manager_type;
			font_manager_type m_fman(*font->get_agg_font_engine());

			int n_last_fit_nonspace=0;
			double f_last_fit_nonspace_advance_width=0.00;
			double f_last_fit_nonspace_blckbox_width=0.00;
			int n_last_fit_allchars=0;
			double f_last_fit_allchars_advance_width=0.00;
			double f_last_fit_allchars_blckbox_width=0.00;
			int n_plus_advance=0;

			double f_prev_fit_advance_width=0.00;
			double f_prev_fit_blckbox_width=0.00;
			double f_next_fit_advance_width=0.00;
			double f_next_fit_blckbox_width=0.00;

			double y = 0.00;
			WCHAR prev_char=0;//' ';
			bool prev_char_is_white=false;
			bool prev_char_is_begin_white=false;
			int q=0;
			bool bStop=false;
			while(*p)
			{
				bool char_is_white=false;
				bool char_is_begin_white=false;

				const agg::glyph_cache* glyph = m_fman.glyph(*p);
				if(!glyph) break;
				//assert(glyph->data_type==agg::glyph_data_outline);

				if(*p=='\n')
				{
					n_last_fit_nonspace=q;
					f_last_fit_nonspace_blckbox_width=f_next_fit_blckbox_width;
					n_plus_advance++;
					*break_by_newline=true;
					bStop=true;
				}
				else if(*p=='\r')
				{
					//Ignore it
					n_plus_advance++;
				}
				else
				{
					if(!q) 
						*pfLinStart=(float)glyph->bounds.x1;
					//else?
					m_fman.add_kerning(&f_next_fit_advance_width, &y);

					//***
					f_next_fit_blckbox_width=f_next_fit_advance_width;
					f_next_fit_advance_width+=glyph->advance_x;
					f_next_fit_blckbox_width+=glyph->bounds.x2;
					q++;
					//***
				     
					switch(*p)
					{
					case ' ':
						//char_is_white=true;
						char_is_begin_white=true;
						if(n_last_fit_allchars==0)
						{
							n_last_fit_nonspace=q;
							f_last_fit_nonspace_advance_width=f_next_fit_advance_width;
							f_last_fit_nonspace_blckbox_width=f_next_fit_blckbox_width;
							//Check this:
							n_last_fit_allchars=q;
							f_last_fit_allchars_advance_width=f_next_fit_advance_width;
							f_last_fit_allchars_blckbox_width=f_next_fit_blckbox_width;
						}
						else
						{
							if(!prev_char_is_begin_white)
							{
								n_last_fit_nonspace=n_last_fit_allchars;
								f_last_fit_nonspace_advance_width=f_last_fit_allchars_advance_width;
								f_last_fit_nonspace_blckbox_width=f_last_fit_allchars_blckbox_width;
							}

						}
						break;
					//ZTODO
					case '(':case '[':case '{': //ZTODO: spec case
						char_is_begin_white=true;
					case '-':case '!':case '?':case '%':case ')':case ']':case '}':
						char_is_white=true;
						//break;
					default:
						if(f_next_fit_blckbox_width>=fMaxWidth)
						{
							//case when single word don't fit
							if(n_last_fit_nonspace==0)
							{
								n_last_fit_nonspace=n_last_fit_allchars;
								f_last_fit_nonspace_blckbox_width=f_last_fit_allchars_blckbox_width;
							}
							bStop=true;
						}
						else
						{
							if(char_is_begin_white)
							{
								if(!prev_char_is_begin_white)
								{
									n_last_fit_nonspace=n_last_fit_allchars;
									f_last_fit_nonspace_advance_width=f_last_fit_allchars_advance_width;
									f_last_fit_nonspace_blckbox_width=f_last_fit_allchars_blckbox_width;
								}
								char_is_white=false;
							}
							else if(prev_char_is_white)
							{
								n_last_fit_nonspace=n_last_fit_allchars;
								f_last_fit_nonspace_advance_width=f_last_fit_allchars_advance_width;
								f_last_fit_nonspace_blckbox_width=f_last_fit_allchars_blckbox_width;
							}
				            

							n_last_fit_allchars=q;
							f_last_fit_allchars_advance_width=f_next_fit_advance_width;
							f_last_fit_allchars_blckbox_width=f_next_fit_blckbox_width;
						}

						break;
					}
					f_prev_fit_blckbox_width=f_prev_fit_advance_width;
					f_prev_fit_advance_width+=glyph->advance_x;
					f_prev_fit_blckbox_width+=glyph->bounds.x2;
				}
				if(bStop) 
					break;

				length--;

				if(!length)
				{
					n_last_fit_nonspace=n_last_fit_allchars;
					f_last_fit_nonspace_blckbox_width=f_last_fit_allchars_blckbox_width;
					break;
				}
				prev_char=*p;
				prev_char_is_white=char_is_white;
				prev_char_is_begin_white=char_is_begin_white;

				++p;
				y += glyph->advance_y;
			}
		 
			*pnLinChars=n_last_fit_nonspace;
			*pnAdvanceChars=n_last_fit_nonspace+n_plus_advance;
			*pfLinWidth=(float)f_last_fit_nonspace_blckbox_width;
		}

		double Font::get_1_6_em() const
		{
		return (GetSize()*get_res_unit_to_pixel())/6.00;

		}

}

