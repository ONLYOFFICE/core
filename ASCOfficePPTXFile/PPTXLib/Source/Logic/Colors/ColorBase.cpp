#include "ColorBase.h"
#include "Parse.h"
#include <windef.h>

namespace PPTX
{
	namespace Logic
	{

		const double cd16 = 1.0/6.0;
		const double cd13 = 1.0/3.0;
		const double cd23 = 2.0/3.0;

		ColorBase::ColorBase()
		{
			alpha = 255;
			red = 0;
			green = 0;
			blue = 0;
			hue = 0;
			saturation = 0;
			luminance = 0;
		}


		ColorBase::~ColorBase()
		{
		}

		DWORD ColorBase::GetRGBA(DWORD RGBA)
		{
			return ApplyModifiers(ctRGBA);
		}

		DWORD ColorBase::GetARGB(DWORD ARGB)
		{
			return ApplyModifiers(ctARGB);
		}

		DWORD ColorBase::GetBGRA(DWORD BGRA)
		{
			return ApplyModifiers(ctBGRA);
		}

		DWORD ColorBase::GetABGR(DWORD ABGR)
		{
			return ApplyModifiers(ctABGR);
		}

		DWORD ColorBase::ApplyModifiers(const ColorType ct)const
		{
			unsigned char components[7];
			components[0] = alpha;
			components[1] = red;
			components[2] = green;
			components[3] = blue;
			components[4] = hue;
			components[5] = saturation;
			components[6] = luminance;

			for(std::list<ColorModifier>::const_iterator i = Modifiers->begin(); i != Modifiers->end(); i++)
			{
				double val = i->val.get_value_or(0)/100000.0;
				switch(i->name->operator[](0))
				{
				case 'a':
					if(i->name == "alpha")
					{
						if(val > 1)
							components[0] = 255;
						else
							components[0] = static_cast<unsigned char>(255*val);
						break;
					}
					//"alphaMod")
					//"alphaOff")
					break;
				case 'b':
					if(i->name == "blue")
					{
						if(val > 1)
							components[3] = 255;
						else
							components[3] = static_cast<unsigned char>(255*val);
						break;
					}
					if(i->name == "blueMod")
					{
						if(components[3]*val > 255)
							components[3] = 255;
						else 
							components[3] = static_cast<unsigned char>(components[3]*val);
						break;
					}
					if(i->name == "blueOff")
					{
						if(components[3] + val*255 < 0)
							components[3] = 0;
						else if(components[3] + val*255 > 255)
							components[3] = 255;
						else components[3] = static_cast<unsigned char>(components[3] + val*255);
						break;
					}
					break;
				case 'ñ':
					//comp
					break;
				case 'g':
					//gamma
					//gray
					if(i->name == "green")
					{
						if(val > 1)
							components[2] = 255;
						else
							components[2] = static_cast<unsigned char>(255*val);
						break;
					}
					if(i->name == "greenMod")
					{
						if(components[2]*val > 255)
							components[2] = 255;
						else 
							components[2] = static_cast<unsigned char>(components[2]*val);
						break;
					}
					if(i->name == "greenOff")
					{
						if(components[2] + val*255 < 0)
							components[2] = 0;
						else if(components[2] + val*255 > 255)
							components[2] = 255;
						else components[2] = static_cast<unsigned char>(components[2] + val*255);
						break;
					}
					break;
				case 'h':
					//hue
					//hueMod
					if(i->name == "hueOff")
					{
						RGB2HSL(&components[1], &components[4]);
						int res = components[4] + (val * 10.0) / 9.0;
						while(res > 240)
							res = res - 240;
						while(res < 0)
							res += 240;
						components[4] = static_cast<unsigned char>(res);
						HSL2RGB(&components[4], &components[1]);
						break;
					}
					break;
				case 'i':
					if(i->name == "inv")
					{
						components[1] = components[1] ^ 0xFF;
						components[2] = components[2] ^ 0xFF;
						components[3] = components[3] ^ 0xFF;
					}
					//invGamma
					break;
				case 'l':
					//lum
					if(i->name == "lumMod")
					{
						RGB2HSL(&components[1], &components[4]);
						if(components[6]*val > 240)
							components[6] = 240;
						else 
							components[6] = static_cast<unsigned char>(components[6]*val);
						HSL2RGB(&components[4], &components[1]);
						break;
					}
					if(i->name == "lumOff")
					{
						RGB2HSL(&components[1], &components[4]);
						int res = components[6] + val * 240;
						if(res > 240)
							res = 240;
						if(res < 0)
							res = 0;
						components[6] = static_cast<unsigned char>(res);
						HSL2RGB(&components[4], &components[1]);
						break;
					}
					break;
				case 'r':
					if(i->name == "red")
					{
						if(val > 1)
							components[1] = 255;
						else
							components[1] = static_cast<unsigned char>(255*val);
						break;
					}
					if(i->name == "redMod")
					{
						if(components[1]*val > 255)
							components[1] = 255;
						else 
							components[1] = static_cast<unsigned char>(components[1]*val);
						break;
					}
					if(i->name == "redOff")
					{
						if(components[1] + val*255 < 0)
							components[1] = 0;
						else if(components[1] + val*255 > 255)
							components[1] = 255;
						else components[1] = static_cast<unsigned char>(components[1] + val*255);
						break;
					}
					break;
				case 's':
					//sat
					if(i->name == "satMod")
					{
						RGB2HSL(&components[1], &components[4]);
						if(components[5]*val > 240)
							components[5] = 240;
						else 
							components[5] = static_cast<unsigned char>(components[5]*val);
						HSL2RGB(&components[4], &components[1]);
						break;
					}
					if(i->name == "satOff")
					{
						RGB2HSL(&components[1], &components[4]);
						int res = components[5] + val * 240;
						if(res > 240)
							res = 240;
						if(res < 0)
							res = 0;
						components[5] = static_cast<unsigned char>(res);
						HSL2RGB(&components[4], &components[1]);
						break;
					}
					if(i->name == "shade")
					{
						components[1] = static_cast<unsigned char>(components[1]*val);
						components[2] = static_cast<unsigned char>(components[2]*val);
						components[3] = static_cast<unsigned char>(components[3]*val);
						break;
					}
					break;
				case 't':
					if(i->name == "tint")
					{
						components[1] = static_cast<unsigned char>(255-(255-components[1])*val);
						components[2] = static_cast<unsigned char>(255-(255-components[2])*val);
						components[3] = static_cast<unsigned char>(255-(255-components[3])*val);
						break;
					}
					break;
				}
			}

			if(ct == ctARGB)
			{
				DWORD ARGB = components[0];
				ARGB = ARGB<<8;
				ARGB += components[1];
				ARGB = ARGB<<8;
				ARGB += components[2];
				ARGB = ARGB<<8;
				ARGB += components[3];
				return ARGB;
			}
			else if(ct == ctRGBA)
			{
				DWORD RGBA = components[1];
				RGBA = RGBA<<8;
				RGBA += components[2];
				RGBA = RGBA<<8;
				RGBA += components[3];
				RGBA = RGBA<<8;
				RGBA += components[0];
				return RGBA;
			}
			else if(ct == ctBGRA)
			{
				DWORD BGRA = components[3];
				BGRA = BGRA<<8;
				BGRA += components[2];
				BGRA = BGRA<<8;
				BGRA += components[1];
				BGRA = BGRA<<8;
				BGRA += components[0];
				return BGRA;
			}
			else if(ct == ctABGR)
			{
				DWORD ABGR = components[0];
				ABGR = ABGR<<8;
				ABGR += components[3];
				ABGR = ABGR<<8;
				ABGR += components[2];
				ABGR = ABGR<<8;
				ABGR += components[1];
				return ABGR;
			}
			return 0;
		}
		
		void ColorBase::SetRGB2HSL()
		{
			int iMin = min( red, min(green, blue));
			int iMax = max( red, max(green, blue));
			int iDelta = iMax - iMin;
			double dMax = ( iMax + iMin )/255.0;
			double dDelta = iDelta/255.0;
			double H,S,L;

			L = dMax / 2.0;

			if ( iDelta == 0 )                     //This is a gray, no chroma...
			{
			   H = 0.0;
			   S = 0.0;
			}
			else                                    //Chromatic data...
			{
				if ( L < 0.5 ) S = dDelta / dMax;
				else           S = dDelta / ( 2.0 - dMax );

				dDelta = dDelta * 1530.0;
				double dR = ( iMax - red ) / dDelta;
				double dG = ( iMax - green ) / dDelta;
				double dB = ( iMax - blue ) / dDelta;

				if      ( red == iMax ) H = dB - dG;
				else if ( green == iMax ) H = cd13 + dR - dB;
				else if ( blue == iMax ) H = cd23 + dG - dR;

				if ( H < 0.0 ) H += 1.0;
				if ( H > 1.0 ) H -= 1.0;
			}

			hue			= static_cast<unsigned char>(H * 240.0);
			saturation	= static_cast<unsigned char>(S * 240.0);
			luminance	= static_cast<unsigned char>(L * 240.0);
		}
		
		void ColorBase::SetHSL2RGB()
		{
			if ( saturation == 0 )
			{
			   red = luminance;
			   green = luminance;
			   blue = luminance;
			}
			else
			{
				double H = double(hue)/240.0;
				double S = double(saturation)/240.0;
				double L = double(luminance)/240.0;
				double v2;
				if ( L < 0.5 ) v2 = L * ( 1.0 + S );
				else           v2 = L + S - S*L;

				double v1 = 2.0 * L - v2;

				red		= static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H + cd13 ) );
				green	= static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H ) );
				blue	= static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H - cd13 ) );
			} 
		}

		void ColorBase::RGB2HSL(unsigned char* RGB, unsigned char* HSL)
		{
			int iMin = min( RGB[0], min(RGB[1], RGB[2]));
			int iMax = max( RGB[0], max(RGB[1], RGB[2]));
			int iDelta = iMax - iMin;
			double dMax = ( iMax + iMin )/255.0;
			double dDelta = iDelta/255.0;
			double H,S,L;

			L = dMax / 2.0;

			if ( iDelta == 0 )                     //This is a gray, no chroma...
			{
			   H = 0.0;
			   S = 0.0;
			}
			else                                    //Chromatic data...
			{
				if ( L < 0.5 ) S = dDelta / dMax;
				else           S = dDelta / ( 2.0 - dMax );

				dDelta = dDelta * 1530.0;
				double dR = ( iMax - RGB[0] ) / dDelta;
				double dG = ( iMax - RGB[1] ) / dDelta;
				double dB = ( iMax - RGB[2] ) / dDelta;

				if      ( RGB[0] == iMax ) H = dB - dG;
				else if ( RGB[1] == iMax ) H = cd13 + dR - dB;
				else if ( RGB[2] == iMax ) H = cd23 + dG - dR;

				if ( H < 0.0 ) H += 1.0;
				if ( H > 1.0 ) H -= 1.0;
			}

			HSL[0] = static_cast<unsigned char>(H * 240.0);
			HSL[1] = static_cast<unsigned char>(S * 240.0);
			HSL[2] = static_cast<unsigned char>(L * 240.0);
		}

		void ColorBase::HSL2RGB(unsigned char* HSL, unsigned char* RGB)
		{
			if ( HSL[1] == 0 )
			{
			   RGB[0] = HSL[2];
			   RGB[1] = HSL[2];
			   RGB[2] = HSL[2];
			}
			else
			{
				double H = double(HSL[0])/240.0;
				double S = double(HSL[1])/240.0;
				double L = double(HSL[2])/240.0;
				double v2;
				if ( L < 0.5 ) v2 = L * ( 1.0 + S );
				else           v2 = L + S - S*L;

				double v1 = 2.0 * L - v2;

				RGB[0] = static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H + cd13 ) );
				RGB[1] = static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H ) );
				RGB[2] = static_cast<unsigned char>(255 * Hue_2_RGB( v1, v2, H - cd13 ) );
			} 
		}

		double ColorBase::Hue_2_RGB(double v1,double v2,double vH)             //Function Hue_2_RGB
		{
		   if ( vH < 0.0 ) vH += 1.0;
		   if ( vH > 1.0 ) vH -= 1.0;
		   if ( vH < cd16 ) return v1 + ( v2 - v1 ) * 6.0 * vH;
		   if ( vH < 0.5 ) return v2;
		   if ( vH < cd23 ) return v1 + ( v2 - v1 ) * ( cd23 - vH ) * 6.0;
		   return v1;
		}
	
	} // namespace Logic
} // namespace PPTX