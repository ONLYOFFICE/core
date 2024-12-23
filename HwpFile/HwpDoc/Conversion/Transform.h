#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../HWPElements/HWPRecordBorderFill.h"

#include "Types.h"

namespace HWP
{
class CTransform
{
public:
	CTransform();

	static int TranslateHWP2Office(int nHWPValue)
	{
		return (int)(nHWPValue * ((double)21000 / 59529));
	}

	static short ToLineWidth(short shHWPThick)
	{
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
};
}

#endif // TRANSFORM_H
