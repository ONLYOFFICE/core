#ifndef TYPES_H
#define TYPES_H

#include <cwchar>
#include <string>

namespace HWP
{
	struct TBorderLine
	{
		int m_nColor;
		int m_nInnerLineWidth;
		int m_nOuterLineWudth;
		int nLineDistance;
	};

	struct TColor
	{
		unsigned char m_uchRed;
		unsigned char m_uchGreen;
		unsigned char m_uchBlue;

		TColor()
			: m_uchRed(0x00), m_uchGreen(0x00), m_uchBlue(0x00)
		{}

		TColor(int nColor)
		{
			m_uchRed   = (nColor >> 16 ) & 0xFF;
			m_uchGreen = (nColor >> 8  ) & 0xFF;
			m_uchBlue  = (nColor >> 0  ) & 0xFF;
		}

		TColor(unsigned char uchRed, unsigned char uchGreen, unsigned char chBlue)
			: m_uchRed(uchRed), m_uchGreen(uchGreen), m_uchBlue(chBlue)
		{}

		bool operator!=(const TColor& oColor) const
		{
			return m_uchRed != oColor.m_uchRed || m_uchGreen != oColor.m_uchGreen || m_uchBlue != oColor.m_uchBlue;
		}

		std::wstring ToHEX() const
		{
			const int tempLen = 7;
			wchar_t arTemp[tempLen];

			swprintf(arTemp, tempLen, L"%02X%02X%02X", m_uchRed, m_uchGreen, m_uchBlue);

			return std::wstring(arTemp, 6);
		}

	};

	struct TRelationship
	{
		std::wstring m_wsID;
		std::wstring m_wsType;
		std::wstring m_wsTarget;
	};
}

#endif // TYPES_H
