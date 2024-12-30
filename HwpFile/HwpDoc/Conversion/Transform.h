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
		return (short)std::ceil((double)shHWPThick * 4 * 25.4 / 72.);

		switch(shHWPThick)
		{
			case 0: // 0.1mm
				return 10;
			case 1: // 0.12mm
				return 12;
			case 2: // 0.15mm
				return 15;
			case 3: // 0.2mm
				return 20;
			case 4: // 0.25mm
				return 25;
			case 5: // 0.3mm
				return 30;
			case 6: // 0.4mm
				return 40;
			case 7: // 0.5mm
				return 50;
			case 8: // 0.6mm
				return 60;
			case 9: // 0.7mm
				return 70;
			case 10: // 1.0mm
				return 100;
			case 11: // 1.5mm
				return 150;
			case 12: // 2.0mm
				return 200;
			case 13: // 3.0mm
				return 300;
			case 14: // 4.0mm
				return 400;
			case 15: // 5.0mm
				return 500;
			default:
				return 0;
		}
	}
}
}

#endif // TRANSFORM_H
