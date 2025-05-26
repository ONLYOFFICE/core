#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iomanip>
#include <sstream>

namespace HWP
{
namespace Transform
{
	inline int HWPINT2OOXML(int nValue)
	{
		return static_cast<int>((double)nValue * 21000. / 59529.);
	}
	inline int HWPUINT2OOXML(int nValue)
	{
		return static_cast<int>((double)nValue * 127.); // 914400. / 7200. = 127
	}

	inline int HWPUINT2Twips(int nValue)
	{
		return static_cast<int>((double)nValue / 7200. * 1440.);
	}

	inline std::wstring IntColorToHEX(int nColor)
	{
		std::wstringstream oSStream;

		oSStream << std::uppercase << std::hex << std::setw(6) << std::setfill(L'0') << nColor;

		return oSStream.str();
	}

	short LineWidth2Pt(short shHWPThick)
	{
		//TODO:: проверить
		// return (short)std::ceil((double)shHWPThick * 4 * 25.4 / 72.);

		switch(shHWPThick)
		{
			case 0: // 0.1mm
				return 2;
			case 1: // 0.12mm
				return 3;
			case 2: // 0.15mm
				return 4;
			case 3: // 0.2mm
				return 5;
			case 4: // 0.25mm
				return 6;
			case 5: // 0.3mm
				return 7;
			case 6: // 0.4mm
				return 9;
			case 7: // 0.5mm
				return 11;
			case 8: // 0.6mm
				return 14;
			case 9: // 0.7mm
				return 16;
			case 10: // 1.0mm
				return 23;
			case 11: // 1.5mm
				return 34;
			case 12: // 2.0mm
				return 46;
			case 13: // 3.0mm
				return 69;
			case 14: // 4.0mm
				return 92 ;
			case 15: // 5.0mm
				return 115;
			default:
				return 0;
		}
	}
}
}

#endif // TRANSFORM_H
