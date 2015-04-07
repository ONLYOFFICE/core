#pragma once
#include "Attributes.h"

namespace NSPresentationEditor
{
	static void CorrectColorPPT(LONG& lSchemeIndex)
	{
//0x00 //Background color
//0x01 //Text color
//0x02 //Shadow color
//0x03 //Title text color
//0x04 //Fill color
//0x05 //Accent 1 color
//0x06 //Accent 2 color
//0x07 //Accent 3 color
		switch (lSchemeIndex)
		{
		case 0:
			lSchemeIndex = 13;
			break;
		case 1:
			lSchemeIndex = 14;
			break;
		case 2:
			lSchemeIndex = 16;
			break;
		case 3:
			lSchemeIndex = 15;
			break;
		case 4:
			lSchemeIndex = 5;
			break;
		case 5:
			lSchemeIndex = 5;//6
			break;
		case 6:
			lSchemeIndex = 6;//11
			break;
		case 7:
			lSchemeIndex = 7;//12
			break;
		default:
			lSchemeIndex = -1;
			break;
		}
	}

	struct SPointAtom 
	{
		LONG X; 
		LONG Y; 

		CString ToString()
		{
			CString str = _T("");
			str.Format(_T("<Point info='(%d,%d)' />"), X, Y);
			return str;
		}
	}; 

	struct SRectAtom 
	{
		LONG Left; 
		LONG Top; 
		LONG Right; 
		LONG Bottom;

		CString ToString()
		{
			CString str = _T("");
			str.Format(_T("<Rect info='(%d,%d,%d,%d)' />"), Left, Top, Right, Bottom);
			return str;
		}
	};

	struct SSmallRectAtom 
	{
		SHORT Left; 
		SHORT Top; 
		SHORT Right; 
		SHORT Bottom;

		CString ToString()
		{
			CString str = _T("");
			str.Format(_T("Rect(%d,%d,%d,%d)"), Left, Top, Right, Bottom);
			return str;
		}
	};
	 
	struct SColorAtom 
	{ 
		BYTE R; 
		BYTE G; 
		BYTE B; 
		BYTE Index; 

		bool bPaletteIndex;
		bool bPaletteRGB;
		bool bSystemRGB;
		bool bSchemeIndex;
		bool bSysIndex;

		SColorAtom()
		{
			R = 0;
			G = 0;
			B = 0; 
			Index = 0;

			bPaletteIndex = false;
			bPaletteRGB = false;
			bSystemRGB = false;
			bSchemeIndex = false;
			bSysIndex = false;
		}

		SColorAtom(const SColorAtom& oSrc)
		{
			*this = oSrc;
		}

		SColorAtom& operator=(const SColorAtom& oSrc)
		{
			R = oSrc.R;
			G = oSrc.G;
			B = oSrc.B;

			Index = oSrc.Index;

			bPaletteIndex = oSrc.bPaletteIndex;
			bPaletteRGB = oSrc.bPaletteRGB;
			bSystemRGB = oSrc.bSystemRGB;
			bSchemeIndex = oSrc.bSchemeIndex;
			bSysIndex = oSrc.bSysIndex;

			return *this;
		}
		SColorAtom& operator=(const CColor& oSrc)
		{
			R = oSrc.R;
			G = oSrc.G;
			B = oSrc.B;

			return *this;
		}

		CString ToString()
		{
			CString str = _T("");
			str.Format(_T("<Color R='%d' G='%d' B='%d' index='%d' />"), R, G, B, Index);
			return str;
		}

		CString ToString(CString name)
		{
			CString str = _T("");
			str.Format(_T(" R='%d' G='%d' B='%d' index='%d' />"), R, G, B, Index);
			str = _T("<Color_") + name + str;
			return str;
		}

		DWORD ToValue()
		{
			DWORD dwVal = (R | (G << 8) | (B << 16));
			return dwVal;
		}
		DWORD ToValue_RGB()
		{
			DWORD dwVal = (B | (G << 8) | (R << 16));
			return dwVal;
		}

		DWORD ToValueProperty()
		{
			DWORD dwVal = 0;
			if (!bSchemeIndex)
			{
				dwVal = (R | (G << 8) | (B << 16));
			}
			else
			{
				dwVal = (R | 0x01000000);
			}
			return dwVal;
		}

		void FromValue(BYTE _R, BYTE _G, BYTE _B)
		{
			R = _R;
			G = _G;
			B = _B;
			Index = 0x00;
		}

		void FromValue(DWORD dwValue)
		{
			R = (BYTE)(dwValue);
			G = (BYTE)(dwValue >> 8);
			B = (BYTE)(dwValue >> 16);
			Index = (BYTE)(dwValue >> 24);

			bPaletteIndex = (0x01 == (Index & 0x01));
			bPaletteRGB = (0x02 == (Index & 0x02));
			bSystemRGB = (0x04 == (Index & 0x04));
			bSchemeIndex = (0x08 == (Index & 0x08));
			bSysIndex = (0x10 == (Index & 0x10));
		}

		void ToColor(CColor* pColor)
		{
			pColor->R = R;
			pColor->G = G;
			pColor->B = B;

			if (bSchemeIndex)
			{
				pColor->m_lSchemeIndex = R;
				CorrectColorPPT(pColor->m_lSchemeIndex);
			}
		}
	};

	struct STextRange
	{
		UINT Begin;
		UINT End;

		CString ToString()
		{
			CString str = _T("");
			str.Format(_T("TextRange(%d,%d)"), Begin, End);
			return str;
		}
	};

	class CElemInfo
	{
	public:
		bool m_bIsBackground;
		bool m_bIsChangeable;

		LONG m_lID;

	public:
		CElemInfo()
		{
			m_bIsBackground = false;
			m_bIsChangeable	= false;

			m_lID = 0;
		}
		CElemInfo(const CElemInfo& oSrc)
		{
			*this = oSrc;
		}
		CElemInfo& operator=(const CElemInfo& oSrc)
		{
			m_bIsBackground = oSrc.m_bIsBackground;
			m_bIsChangeable	= oSrc.m_bIsChangeable;
			m_lID			= oSrc.m_lID;

			return *this;
		}
	};
}