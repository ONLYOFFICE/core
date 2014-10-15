#pragma once
#ifndef PPTX_LOGIC_COLORBASE_INCLUDE_H_
#define PPTX_LOGIC_COLORBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "ColorModifier.h"

namespace PPTX
{
	const double cd16 = 1.0/6.0;
	const double cd13 = 1.0/3.0;
	const double cd23 = 2.0/3.0;

	namespace Logic
	{
		class ColorBase : public WrapperWritingElement
		{
		public:
			ColorBase()
			{
				alpha		= 255;
				red			= 0;
				green		= 0;
				blue		= 0;
				hue			= 0;
				saturation	= 0;
				luminance	= 0;
			}
			ColorBase(const ColorBase& oSrc)
			{
				*this = oSrc;
			}
			ColorBase& operator=(const ColorBase& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				alpha		= oSrc.alpha;
				red			= oSrc.red;
				green		= oSrc.green;
				blue		= oSrc.blue;
				hue			= oSrc.hue;
				saturation	= oSrc.saturation;
				luminance	= oSrc.luminance;
				return *this;
			}

			virtual ~ColorBase()
			{
			}

		public:
			//Эти функции возвращают цвет с учетом модификаторов
            virtual DWORD GetRGBA(DWORD RGBA) const
			{
				return ApplyModifiers(ctRGBA);
			}

            virtual DWORD GetARGB(DWORD ARGB) const
			{
				return ApplyModifiers(ctARGB);
			}

            virtual DWORD GetBGRA(DWORD BGRA) const
			{
				return ApplyModifiers(ctBGRA);
			}

            virtual DWORD GetABGR(DWORD ABGR) const
			{
				return ApplyModifiers(ctABGR);
			}

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, DWORD ARGB = 0)
			{
				return GetARGB(ARGB);
			}

			void SetRGB(const BYTE& R, const BYTE& G, const BYTE& B)
			{
				red = R;
				green = G;
				blue = B;
			}
			//Надо сделать примерно также для "origin color" и setter'ы
			//Нет, нет и нет!!! Setter'ы делать только в УниКолор, т.к. при установке цвета меняется его тип!

		protected:
			//origin color
			unsigned char alpha;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char hue;
			unsigned char saturation;
			unsigned char luminance;

			enum ColorType {ctRGBA, ctARGB, ctBGRA, ctABGR};
			virtual DWORD ApplyModifiers(const ColorType ct)const
			{
				unsigned char components[7];
				components[0] = alpha;
				components[1] = red;
				components[2] = green;
				components[3] = blue;
				components[4] = hue;
				components[5] = saturation;
				components[6] = luminance;

				size_t nCount = Modifiers.size();
				for (size_t i = 0; i != nCount; ++i)
				{
					const Logic::ColorModifier* colorMod = &Modifiers[i];
					double val = colorMod->val.get_value_or(0)/100000.0;

					CString name = XmlUtils::GetNameNoNS(colorMod->name);
					if (_T("") == name)
						continue;

					switch((CHAR)name[0])
					{
					case 'a':
						if(name == _T("alpha"))
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
						if(name == _T("blue"))
						{
							if(val > 1)
								components[3] = 255;
							else
								components[3] = static_cast<unsigned char>(255*val);
							break;
						}
						if(name == _T("blueMod"))
						{
							if(components[3]*val > 255)
								components[3] = 255;
							else 
								components[3] = static_cast<unsigned char>(components[3]*val);
							break;
						}
						if(name == _T("blueOff"))
						{
							if(components[3] + val*255 < 0)
								components[3] = 0;
							else if(components[3] + val*255 > 255)
								components[3] = 255;
							else components[3] = static_cast<unsigned char>(components[3] + val*255);
							break;
						}
						break;
					case 'с':
						//comp
						break;
					case 'g':
						//gamma
						//gray
						if(name == _T("green"))
						{
							if(val > 1)
								components[2] = 255;
							else
								components[2] = static_cast<unsigned char>(255*val);
							break;
						}
						if(name == _T("greenMod"))
						{
							if(components[2]*val > 255)
								components[2] = 255;
							else 
								components[2] = static_cast<unsigned char>(components[2]*val);
							break;
						}
						if(name == _T("greenOff"))
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
						if(name == _T("hueOff"))
						{
							RGB2HSL(&components[1], &components[4]);
							int res = (int)(components[4] + (val * 10.0) / 9.0);
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
						if(name == _T("inv"))
						{
							components[1] = components[1] ^ 0xFF;
							components[2] = components[2] ^ 0xFF;
							components[3] = components[3] ^ 0xFF;
						}
						//invGamma
						break;
					case 'l':
						//lum
						if(name == _T("lumMod"))
						{
							RGB2HSL(&components[1], &components[4]);
							if(components[6]*val > 240)
								components[6] = 240;
							else 
								components[6] = static_cast<unsigned char>(components[6]*val);
							HSL2RGB(&components[4], &components[1]);
							break;
						}
						if(name == _T("lumOff"))
						{
							RGB2HSL(&components[1], &components[4]);
							int res = (int)(components[6] + val * 240);
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
						if(name == _T("red"))
						{
							if(val > 1)
								components[1] = 255;
							else
								components[1] = static_cast<unsigned char>(255*val);
							break;
						}
						if(name == _T("redMod"))
						{
							if(components[1]*val > 255)
								components[1] = 255;
							else 
								components[1] = static_cast<unsigned char>(components[1]*val);
							break;
						}
						if(name == _T("redOff"))
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
						if(name == _T("satMod"))
						{
							RGB2HSL(&components[1], &components[4]);
							if(components[5]*val > 240)
								components[5] = 240;
							else 
								components[5] = static_cast<unsigned char>(components[5]*val);
							HSL2RGB(&components[4], &components[1]);
							break;
						}
						if(name == _T("satOff"))
						{
							RGB2HSL(&components[1], &components[4]);
							int res = (int)(components[5] + val * 240);
							if(res > 240)
								res = 240;
							if(res < 0)
								res = 0;
							components[5] = static_cast<unsigned char>(res);
							HSL2RGB(&components[4], &components[1]);
							break;
						}
						if(name == _T("shade"))
						{
							components[1] = static_cast<unsigned char>(components[1]*val);
							components[2] = static_cast<unsigned char>(components[2]*val);
							components[3] = static_cast<unsigned char>(components[3]*val);
							break;
						}
						break;
					case 't':
						if(name == _T("tint"))
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

			//Эти функции использовать для заполнения "origin color"
			void SetRGB2HSL()
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
			void SetHSL2RGB()
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


			//Эти использовать при применении модификаторов
			static void RGB2HSL(unsigned char* RGB, unsigned char* HSL)
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

			static void HSL2RGB(unsigned char* HSL, unsigned char* RGB)
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

			static double Hue_2_RGB(double v1,double v2,double vH)             //Function Hue_2_RGB
			{
			   if ( vH < 0.0 ) vH += 1.0;
			   if ( vH > 1.0 ) vH -= 1.0;
			   if ( vH < cd16 ) return v1 + ( v2 - v1 ) * 6.0 * vH;
			   if ( vH < 0.5 ) return v2;
			   if ( vH < cd23 ) return v1 + ( v2 - v1 ) * ( cd23 - vH ) * 6.0;
			   return v1;
			}
			const int HexChar2Int(const char value)
			{
				if (value >= '0' && value <= '9')
					return value - '0';
				if (value >= 'a' && value <= 'f')
					return 10 + value - 'a';
				if (value >= 'A' && value <= 'F')
					return 10 + value - 'A';
				return 0;
			}

			const int HexString2Int(const CString& value)
			{
				int summa = 0;
				for (int i = 0; i != value.GetLength(); ++i)
					summa += HexChar2Int((char)value[i]) << (4 * (value.GetLength() - i - 1));
				return summa;
			}
		public:
			std::vector<ColorModifier> Modifiers;

		public:
			void ReadModsFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _start = pReader->GetPos();
				LONG _end = _start + pReader->GetLong() + 4;

				ULONG _count = pReader->GetULong();
				for (ULONG i = 0; i < _count; ++i)
				{
					if (pReader->GetPos() > _end)
						break;

					pReader->Skip(1);

					LONG _s1 = pReader->GetPos();
					LONG _e1 = _s1 + pReader->GetLong() + 4;

					if (_s1 < _e1)
					{
						pReader->Skip(1);

						ColorModifier _mod;

						while (true)
						{
							BYTE _type = pReader->GetUChar();

							if (0 == _type)
							{
								CString _name = pReader->GetString2();
								int _find = _name.Find(_T(":"));
								if (_find >= 0 && _find < (_name.GetLength() - 1))
									_name = _name.Mid(_find + 1);

								_mod.name = _T("a:") + _name;
							}
							else if (1 == _type)
								_mod.val = pReader->GetLong();
							else if (NSBinPptxRW::g_nodeAttributeEnd == _type)
								break;
							else
								break;
						}

						Modifiers.push_back(_mod);
					}

					pReader->Seek(_e1);
				}

				pReader->Seek(_end);				
			}

			friend class UniColor;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COLORBASE_INCLUDE_H
