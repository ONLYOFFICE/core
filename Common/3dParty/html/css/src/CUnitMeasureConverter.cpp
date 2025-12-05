#include "CUnitMeasureConverter.h"

#include <sstream>
#include <algorithm>
#include <vector>
#include <regex>

namespace NSCSS
{
	CUnitMeasureConverter::CUnitMeasureConverter()
	{}

	double CUnitMeasureConverter::ConvertPx(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 72. / (double)ushDPI;
			case NSCSS::Cantimeter:
				return dValue / (double)ushDPI * 2.54;
			case NSCSS::Millimeter:
				return dValue / (double)ushDPI * 25.4;
			case NSCSS::Inch:
				return dValue / (double)ushDPI;
			case NSCSS::Peak:
				return dValue * 6. / (double)ushDPI; // 1 дюйм = 6 пик
			case NSCSS::Twips:
				return dValue * 1440. / (double)ushDPI;
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertCm(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 28.3465 ; // 1 см = (2.54 / 72) пункта
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 2.54;
			case NSCSS::Millimeter:
				return dValue * 10.;
			case NSCSS::Inch:
				return dValue / 2.54; // 1 дюйм = 2.54 см
			case NSCSS::Peak:
				return dValue * 2.36; // 2.36 = 6 / 2.54
			case NSCSS::Twips:
				return dValue * 567.; // 1 см = (1440 / 2.54) твипов
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertMm(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 2.8346; // 1 мм = (25.4 / 72) пункта
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 25.4;
			case NSCSS::Cantimeter:
				return dValue / 10.;
			case NSCSS::Inch:
				return dValue / 25.4;
			case NSCSS::Peak:
				return dValue * 0.236; // 0.236 = 6 / 25.4
			case NSCSS::Twips:
				return dValue * 56.7;
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertIn(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue / 72.;
			case NSCSS::Pixel:
				return dValue * (double)ushDPI;
			case NSCSS::Cantimeter:
				return dValue * 2.54; // 1 дюйм = 2.54 см
			case NSCSS::Millimeter:
				return dValue * 25.4;
			case NSCSS::Peak:
				return dValue * 6.;
			case NSCSS::Twips:
				return dValue * 1440.;
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertPt(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 72.;
			case NSCSS::Cantimeter:
				return dValue * 0.03528; // 0.03528 = 2.54 / 72
			case NSCSS::Millimeter:
				return dValue * 0.3528;
			case NSCSS::Inch:
				return dValue / 72.; // 1 дюйм = 72 пункта
			case NSCSS::Peak:
				return dValue * 0.0833; // 0.0833 = 6 / 72 (1 пункт = 1/72 дюйма)
			case NSCSS::Twips:
				return dValue * 20.; // 20 = 1440 / 72
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertPc(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 12.; // 12 = 72 / 6
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 6.; // 1 дюйм = 6 пика
			case NSCSS::Cantimeter:
				return dValue * 0.423; // 0.423 = 2.54 / 6
			case NSCSS::Millimeter:
				return dValue * 4.233; // 4.23 = 25.4 / 6
			case NSCSS::Inch:
				return dValue / 6.;
			case NSCSS::Twips:
				return dValue * 3.333; // 3.333 = 20 / 6
			default:
				return dValue;
		}
	}
	
	double CUnitMeasureConverter::ConvertTw(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 0.05; // 0.05 = 72. / 1440.
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 1440.; // 1 дюйм = 1440 твипов
			case NSCSS::Cantimeter:
				return dValue * 0.001764; // 0.001764 = 2.54 / 1440
			case NSCSS::Millimeter:
				return dValue * 0.01764;
			case NSCSS::Inch:
				return dValue / 1440.;
			case NSCSS::Peak:
				return dValue * 0.004167; // 0.004167 = 6 / 1440
			default:
				return dValue;
		}
	}

	bool CUnitMeasureConverter::GetValue(const std::wstring &wsValue, double &dValue, UnitMeasure &enUnitMeasure)
	{
		if (wsValue.empty() || wsValue.end() == std::find_if(wsValue.begin(), wsValue.end(), [](wchar_t wChar) { return iswdigit(wChar);}))
			return false;

		std::wregex oRegex(LR"((-?\.\d+|-?\d+(\.\d+)?)\s*(px|pt|cm|mm|in|pc|%|em|rem|tw)?)");
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
		else if (L"em" == oMatches[3])
			enUnitMeasure = Em;
		else if (L"rem" == oMatches[3])
			enUnitMeasure = Rem;
		else if (L"tw" == oMatches[3])
			enUnitMeasure = Twips;
		else
			enUnitMeasure = None;
		
		return true;
	}
}
