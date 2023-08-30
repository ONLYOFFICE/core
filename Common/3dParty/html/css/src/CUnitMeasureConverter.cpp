#include "CUnitMeasureConverter.h"

#include <sstream>
#include <algorithm>
#include <vector>

#include "StaticFunctions.h"

namespace NSCSS
{
	CUnitMeasureConverter::CUnitMeasureConverter()
	{}

	double CUnitMeasureConverter::ConvertPx(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Pixel:
				return dValue;
			case NSCSS::Point:
				return 72. / (double)ushDPI * dValue;
			case NSCSS::Cantimeter:
				return dValue / (double)ushDPI * 2.54;
			case NSCSS::Millimeter:
				return dValue / (double)ushDPI * 25.4;
			case NSCSS::Inch:
				return 1. / (double)ushDPI * dValue;
			case NSCSS::Peak:
				return 0.16667 / (double)ushDPI * dValue;
		}

		return 0.;
	}

	double CUnitMeasureConverter::ConvertCm(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return 28.35 * dValue;
			case NSCSS::Pixel:
				return (double)ushDPI / 2.54 * dValue;
			case NSCSS::Cantimeter:
				return dValue;
			case NSCSS::Millimeter:
				return dValue * 10.;
			case NSCSS::Inch:
				return dValue / 2.54f;
			case NSCSS::Peak:
				return 2.36 * dValue;
		}

		return 0.;
	}

	double CUnitMeasureConverter::ConvertMm(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return 2.835 * dValue;
			case NSCSS::Pixel:
				return (double)ushDPI / 25.4 * dValue;
			case NSCSS::Cantimeter:
				return dValue / 10.;
			case NSCSS::Millimeter:
				return dValue;
			case NSCSS::Inch:
				return dValue / 25.4;
			case NSCSS::Peak:
				return 0.236 * dValue;
		}

		return 0.;
	}

	double CUnitMeasureConverter::ConvertIn(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue / 6.;
			case NSCSS::Pixel:
				return dValue * (double)ushDPI;
			case NSCSS::Cantimeter:
				return dValue * 2.54;
			case NSCSS::Millimeter:
				return dValue * 25.4;
			case NSCSS::Inch:
				return dValue;
			case NSCSS::Peak:
				return dValue / 72.;
		}

		return 0.;
	}

	double CUnitMeasureConverter::ConvertPt(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue;
			case NSCSS::Pixel:
				return (double)ushDPI / 72. * dValue;
			case NSCSS::Cantimeter:
				return dValue * 0.03528;
			case NSCSS::Millimeter:
				return dValue * 0.3528;
			case NSCSS::Inch:
				return dValue / 72.;
			case NSCSS::Peak:
				return dValue / 12.;
		}

		return 0.;
	}

	double CUnitMeasureConverter::ConvertPc(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 12.;
			case NSCSS::Pixel:
				return (double)ushDPI / 6. * dValue;
			case NSCSS::Cantimeter:
				return dValue * 0.423;
			case NSCSS::Millimeter:
				return dValue * 4.23;
			case NSCSS::Inch:
				return dValue / 6.;
			case NSCSS::Peak:
				return dValue;
		}

		return 0.;
	}
	bool CUnitMeasureConverter::GetValue(const std::wstring &wsValue, double &dValue, UnitMeasure &enUnitMeasure)
	{
		std::wregex oRegex(LR"((\.\d+|\d+(\.\d+)?)\s*(px|pt|cm|mm|in|pc|%|rem)?)");
		std::wsmatch oMatches;

		if(!std::regex_search(wsValue, oMatches, oRegex))
			return false;

		dValue = stod(oMatches[1]);

		if (L"px" == oMatches[3])
			enUnitMeasure = Pixel;
		else if (L"pt" == oMatches[3])
			enUnitMeasure = Point;
		else if (L"cm" == oMatches[3])
			enUnitMeasure = Cantimeter;
		else if (L"mm" == oMatches[3])
			enUnitMeasure = Millimeter;
		else if (L"in" == oMatches[3])
			enUnitMeasure = Inch;
		else if (L"pc" == oMatches[3])
			enUnitMeasure = Peak;
		else if (L"%" == oMatches[3])
			enUnitMeasure = Percent;
		else if (L"rem" == oMatches[3])
		{
			enUnitMeasure = Percent;
			dValue *= 100.;
		}
		else
			enUnitMeasure = None;
		
		return true;
	}
}
