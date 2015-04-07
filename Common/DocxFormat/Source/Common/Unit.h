#pragma once
#ifndef _UNIT_INCLUDE_H_
#define _UNIT_INCLUDE_H_

#include "../Base/Base.h"

AVSINLINE double Cm_To_Mm     (const double &dValue)
{
	return dValue * 10;
}

AVSINLINE double Cm_To_Pt     (const double &dValue)
{
	return dValue * 72 / 2.54;
}

AVSINLINE double Cm_To_Px     (const double &dValue)
{
	return dValue * 72 * 4 / 3 / 2.54;
}

AVSINLINE double Cm_To_Inch   (const double &dValue)
{
	return dValue / 2.54;
}



AVSINLINE double Cm_To_Dx     (const double &dValue)
{
	return dValue * 72 * 20 / 2.54;
}

AVSINLINE double Cm_To_Sx     (const double &dValue)
{
	return dValue * 72 * 100 * 1000 / 20;
}
AVSINLINE double Cm_To_Multi  (const double &dValue)
{
	return dValue * 72 * 20 / 2.54;
}



AVSINLINE double Cm_To_Emu    (const double &dValue)
{
	return dValue * 360000;
}



AVSINLINE double Mm_To_Cm     (const double &dValue)
{
	return dValue / 10;
}

AVSINLINE double Mm_To_Pt     (const double &dValue)
{
	return dValue * 72 / 10 / 2.54;
}

AVSINLINE double Mm_To_Px     (const double &dValue)
{
	return dValue * 72  * 4 / 3 / 10 / 2.54;
}

AVSINLINE double Mm_To_Inch   (const double &dValue)
{
	return dValue / 2.54 / 10;
}

AVSINLINE double Mm_To_Dx     (const double &dValue)
{
	return dValue * 72 * 20 / 10 / 2.54;
}
AVSINLINE double Mm_To_Sx     (const double &dValue)
{
	return dValue * 72 * 100 * 1000 / 10 / 20;
}
AVSINLINE double Mm_To_Multi  (const double &dValue)
{
	return dValue * 72 * 20 / 10 / 2.54;
}
AVSINLINE double Mm_To_Emu    (const double &dValue)
{
	return dValue * 36000;
}
AVSINLINE double Pt_To_Cm     (const double &dValue)
{
	return dValue * 2.54 / 72;
}

AVSINLINE double Pt_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 / 72;
}

AVSINLINE double Pt_To_Px     (const double &dValue)
{
	return dValue * 4 / 3;
}

AVSINLINE double Pt_To_Inch   (const double &dValue)
{
	return dValue / 72;
}

AVSINLINE double Pt_To_Dx     (const double &dValue)
{
	return dValue * 20;
}

AVSINLINE double Pt_To_Sx     (const double &dValue)
{
	return dValue * 2.54 * 100 * 1000 / 20;
}

AVSINLINE double Pt_To_Multi  (const double &dValue)
{
	return dValue * 20;
}
AVSINLINE double Pt_To_Emu    (const double &dValue)
{
	return dValue * 12700;
}
AVSINLINE double Px_To_Cm     (const double &dValue)
{
	return dValue * 2.54 * 3/ 72 / 4;
}
AVSINLINE double Px_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 * 3/ 72 /4;
}
AVSINLINE double Px_To_Pt     (const double &dValue)
{
	return dValue * 3 / 4;
}
AVSINLINE double Px_To_Inch   (const double &dValue)
{
	return dValue * 3 / 72 / 4;
}

AVSINLINE double Px_To_Dx     (const double &dValue)
{
	return dValue * 20 * 3 / 4;
}
AVSINLINE double Px_To_Sx     (const double &dValue)
{
	return dValue * 2.54 * 100 * 1000 * 3/ 20 / 4;
}

AVSINLINE double Px_To_Multi  (const double &dValue)
{
	return dValue * 20 * 3 / 4;
}

AVSINLINE double Px_To_Emu    (const double &dValue)
{
	return dValue * 9525;
}

AVSINLINE double Inch_To_Cm   (const double &dValue)
{
	return dValue * 2.54;
}
AVSINLINE double Inch_To_Mm   (const double &dValue)
{
	return dValue * 2.54 * 10;
}
AVSINLINE double Inch_To_Pt   (const double &dValue)
{
	return dValue * 72;
}

AVSINLINE double Inch_To_Px   (const double &dValue)
{
	return dValue * 72 * 4 / 3;
}

AVSINLINE double Inch_To_Dx   (const double &dValue)
{
	return dValue * 72 * 20;
}

AVSINLINE double Inch_To_Sx   (const double &dValue)
{
	return dValue * 1000 * 100 * 2.54 * 72 / 20;
}

AVSINLINE double Inch_To_Multi(const double &dValue)
{
	return dValue * 72 * 20;
}
AVSINLINE double Inch_To_Emu  (const double &dValue)
{
	return dValue * 914400;
}
AVSINLINE double Dx_To_Cm     (const double &dValue)
{
	return dValue * 2.54 / 72 / 20;
}
AVSINLINE double Dx_To_Mm     (const double &dValue)
{
	return dValue * 2.54 * 10 / 72 / 20;
}
AVSINLINE double Dx_To_Pt     (const double &dValue)
{
	return dValue / 20;
}

AVSINLINE double Dx_To_Px     (const double &dValue)
{
	return dValue * 4 / 3 / 20;
}

AVSINLINE double Dx_To_Inch   (const double &dValue)
{
	return dValue / 20 / 72;
}

AVSINLINE double Dx_To_Sx     (const double &dValue)
{
	return dValue * 635;
}

AVSINLINE double Dx_To_Multi  (const double &dValue)
{
	return dValue;
}

AVSINLINE double Dx_To_Emu    (const double &dValue)
{
	return dValue * 635;
}

AVSINLINE double Sx_To_Cm     (const double &dValue)
{
	return dValue * 20 / 72 / 100 / 1000;
}
AVSINLINE double Sx_To_Mm     (const double &dValue)
{
	return dValue * 20 / 72 / 100 / 1000 * 10;
}
AVSINLINE double Sx_To_Pt     (const double &dValue)
{
	return dValue * 20 / 100 / 1000 / 2.54;
}

AVSINLINE double Sx_To_Px     (const double &dValue)
{
	return dValue * 20 * 4 / 3 / 100 / 1000 / 2.54;
}

AVSINLINE double Sx_To_Inch   (const double &dValue)
{
	return dValue * 20 / 2.54 / 72 / 100 / 1000;
}

AVSINLINE double Sx_To_Dx     (const double &dValue)
{
	return dValue * 20 * 20 / 2.54 / 100 / 1000;
}

AVSINLINE double Sx_To_Multi  (const double &dValue)
{
	return dValue * 20 * 20 / 2.54 / 100 / 1000;
}

AVSINLINE double Sx_To_Emu    (const double &dValue)
{
	return dValue;
}

AVSINLINE double Multi_To_Cm  (const double &dValue)
{
	return dValue * 2.54 / 72 / 20;
}
AVSINLINE double Multi_To_Mm  (const double &dValue)
{
	return dValue * 2.54 * 10 / 72 / 20;
}
AVSINLINE double Multi_To_Pt  (const double &dValue)
{
	return dValue / 20;
}

AVSINLINE double Multi_To_Px  (const double &dValue)
{
	return dValue * 4 / 3 / 20;
}

AVSINLINE double Multi_To_Inch(const double &dValue)
{
	return dValue / 20 / 72;
}

AVSINLINE double Multi_To_Sx  (const double &dValue)
{
	return dValue * 635;
}

AVSINLINE double Multi_To_Dx  (const double &dValue)
{
	return dValue;
}




AVSINLINE double Multi_To_Emu (const double &dValue)
{
	return dValue * 635;
}




AVSINLINE double Emu_To_Cm    (const double &dValue)
{
	return dValue / 360000;
}
AVSINLINE double Emu_To_Mm    (const double &dValue)
{
	return dValue / 36000;
}
AVSINLINE double Emu_To_Pt    (const double &dValue)
{
	return dValue / 12700;
}
AVSINLINE double Emu_To_Twips    (const double &dValue)
{
	return dValue / 635;
}
AVSINLINE double Emu_To_Px    (const double &dValue)
{
	return dValue / 9525;
}

AVSINLINE double Emu_To_Inch  (const double &dValue)
{
	return dValue / 914400;
}

AVSINLINE double Emu_To_Sx    (const double &dValue)
{
	return dValue;
}

AVSINLINE double Emu_To_Dx    (const double &dValue)
{
	return dValue / 635;
}
AVSINLINE double Emu_To_Multi (const double &dValue)
{
	return dValue / 635;
}





#endif // _UNIT_INCLUDE_H_