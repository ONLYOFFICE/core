#pragma once

#include <Gdiplus.h>

namespace ImageStudio
{
	namespace Colors
	{
		#define GetAValue(rgb)   (LOBYTE((rgb)>>24))
		#define RGBA(r, g, b, a) ((DWORD)(((BYTE)(r))| ((WORD)((BYTE)(g))<<8) | ((DWORD)((BYTE)(b))<<16) | ((DWORD)((BYTE)(a))<<24)))

		class Color
		{
		private:
			
			double m_dRed; // 0..1
			double m_dGreen; // 0..1
			double m_dBlue; // 0..1
			double m_dAlpha; // 0..1
						
		public:
			
			Color()
			{
				Reset();
			}
			void Reset()
			{
				m_dRed = 0;
				m_dGreen = 0;
				m_dBlue = 0;
				m_dAlpha = 1.0;
			}
			
			void FromCOLORREF(COLORREF nColor)
			{
				m_dRed = GetRValue(nColor)/255.0;
				m_dGreen = GetGValue(nColor)/255.0;
				m_dBlue = GetBValue(nColor)/255.0;
				m_dAlpha = 1.0;
			}
			void FromCOLORREF(COLORREF nColor, BYTE nAlpha)
			{
				m_dRed = GetRValue(nColor)/255.0;
				m_dGreen = GetGValue(nColor)/255.0;
				m_dBlue = GetBValue(nColor)/255.0;
				m_dAlpha = nAlpha/255.0;
			}
			void FromGdiPlusColor(Gdiplus::Color oColor)
			{
				m_dRed = oColor.GetR()/255.0;
				m_dGreen = oColor.GetG()/255.0;
				m_dBlue = oColor.GetB()/255.0;
				m_dAlpha = oColor.GetA()/255.0;
			}
			void FromGdiPlusColor(Gdiplus::ARGB nARGB)
			{
				Gdiplus::Color oColor;

				oColor.SetValue(nARGB);

				FromGdiPlusColor(oColor);
			}
			
			void ToCOLORREF(COLORREF& nColor)
			{
				nColor = RGB(GetRedB(), GetGreenB(), GetBlueB());
			}
			void ToHLS(float& fHue, float& fLightness, float& fSaturation)
			{
				RgbToHls((float)m_dRed, (float)m_dGreen, (float)m_dBlue, fHue, fLightness, fSaturation);
			}
			
			double GetRedD()
			{
				return m_dRed;
			}
			double GetGreenD()
			{
				return m_dGreen;
			}
			double GetBlueD()
			{
				return m_dBlue;
			}
			double GetAlphaD()
			{
				return m_dAlpha;
			}
			double GetRedB()
			{
				return GetByte(m_dRed);
			}
			double GetGreenB()
			{
				return GetByte(m_dGreen);
			}
			double GetBlueB()
			{
				return GetByte(m_dBlue);
			}
			double GetAlphaB()
			{
				return GetByte(m_dAlpha);
			}
			
			void SetRedD(double dValue)
			{
				m_dRed = SetDouble(dValue);
			}
			void SetGreenD(double dValue)
			{
				m_dGreen = SetDouble(dValue);
			}
			void SetBlueD(double dValue)
			{
				m_dBlue = SetDouble(dValue);
			}
			void SetAlphaD(double dValue)
			{
				m_dAlpha = SetDouble(dValue);
			}
			void SetRedB(BYTE nValue)
			{
				m_dRed = SetByte(nValue);
			}
			void SetGreenB(BYTE nValue)
			{
				m_dGreen = SetByte(nValue);
			}
			void SetBlueD(BYTE nValue)
			{
				m_dBlue = SetByte(nValue);
			}
			void SetAlphaD(BYTE nValue)
			{
				m_dAlpha = SetByte(nValue);
			}
			
		public:
			
			static void RgbToHls(float fRed, float fGreen, float fBlue, float& fHue, float& fLightness, float& fSaturation)
			{
				// variables
				float fMinValue, fMaxValue, fDeltaValue;
				BYTE nMaxComponent = 0; // red

				// compute minimal and maximal components values
				if (fRed > fGreen)
				{
					if (fRed > fBlue)	{ fMaxValue = fRed; nMaxComponent = 0; }
					else				{ fMaxValue = fBlue; nMaxComponent = 2; }

					if (fGreen < fBlue)	{ fMinValue = fGreen; }
					else				{ fMinValue = fBlue; }
				}
				else
				{
					if (fGreen > fBlue) { fMaxValue = fGreen; nMaxComponent = 1; }
					else				{ fMaxValue = fBlue; nMaxComponent = 2; }

					if (fRed < fBlue)	{ fMinValue = fRed; }
					else				{ fMinValue = fBlue; }
				}

				// compute lightness value
				fLightness = 0.5f*(fMaxValue + fMinValue);

				// compute additional useful parameter
				fDeltaValue = (fMaxValue - fMinValue);

				// check for achromatic case
				if (fabs(fDeltaValue) < 0.00001)
				{
					fSaturation = 0.0f;
					fHue = 0.0f;
				}
				else
				{
					// compute saturation value
					if (fLightness < 0.5f)			fSaturation = fDeltaValue / (fMaxValue + fMinValue);
					else							fSaturation = fDeltaValue / (2.0f - fMaxValue - fMinValue);

					// compute hue angle
					if (nMaxComponent == 0)			fHue = (fGreen - fBlue) / fDeltaValue;
					else if (nMaxComponent == 1)	fHue = 2.0f + (fBlue - fRed) / fDeltaValue;
					else							fHue = 4.0f + (fRed - fGreen) / fDeltaValue;

					fHue *= 60.0f;

					// clamp hue angle to 0..360 range
					if (fHue < 0.0f)	fHue += 360.0f;
					if (fHue > 360.0f)	fHue -= 360.0f;
				}
			}
			
		private:
			
			inline double Clamp(double dValue)
			{
				if (dValue < 0.0)
					return 0.0;
				if (dValue > 1.0)
					return 1.0;

				return dValue;
			}
			
			inline double SetDouble(double dValue)
			{
				return Clamp(dValue);
			}
			inline double SetByte(BYTE nValue)
			{
				return nValue/255.0;
			}
			inline double SetInteger(int nValue)
			{
				return Clamp(nValue/255.0);
			}
			
			inline BYTE GetByte(double dValue)
			{
				return (BYTE)(255*dValue);
			}
			inline int GetInteger(double dValue)
			{
				return (int)(255*dValue);
			}
		};
	}
}
