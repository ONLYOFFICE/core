#pragma once

#include "../BiffStructure.h"
#include "OfficeArtCOLORREF.h"
#include "../FixedPoint.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class MSOLINEDASHING
{
public:
    static const std::wstring  ToString(const int raw_data)
	{
		switch (raw_data)
		{
			case 0x00000000:
				return L"msolineSolid";

			case 0x00000001:
				return L"msolineDashSys";

			case 0x00000002:
				return L"msolineDotSys";

			case 0x00000003:
				return L"msolineDashDotSys";

			case 0x00000004:
				return L"msolineDashDotDotSys";

			case 0x00000005:
				return L"msolineDotGEL";

			case 0x00000006:
				return L"msolineDashGEL";

			case 0x00000007:
				return L"msolineLongDashGEL";

			case 0x00000008:
				return L"msolineDashDotGEL";

			case 0x00000009:
				return L"msolineLongDashDotGEL";

			case 0x0000000A:
				return L"msolineLongDashDotDotGEL";
		}
		return L"";
	}
};


class MSOLINESTYLE
{
public:
    static const std::wstring  ToString(const int raw_data)
	{
		switch (raw_data)
		{
			case 0x00000000:
				return L"msolineSimple";

			case 0x00000001:
				return L"msolineDouble";

			case 0x00000002:
				return L"msolineThickThin";

			case 0x00000003:
				return L"msolineThinThick";

			case 0x00000004:
				return L"msolineTriple";
		}
		return L"";
	}
};



} // namespace XLS
