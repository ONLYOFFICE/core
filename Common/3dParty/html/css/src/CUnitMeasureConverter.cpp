#include "CUnitMeasureConverter.h"

#include <sstream>
#include <algorithm>
#include <vector>
#include <cwctype>

namespace NSCSS
{
	static size_t SkipDigits(const std::wstring& wsValue, size_t unPos)
	{
		while (unPos < wsValue.length() && iswdigit(wsValue[unPos]))
			++unPos;

		return unPos;
	}

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
				return dValue * 6. / (double)ushDPI; // 1 inch = 6 peak
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
				return dValue * 28.3465 ; // 1 cm = (2.54 / 72) points
			case NSCSS::Pixel:
				return dValue * (double)ushDPI / 2.54;
			case NSCSS::Millimeter:
				return dValue * 10.;
			case NSCSS::Inch:
				return dValue / 2.54; // 1 inch = 2.54 cm
			case NSCSS::Peak:
				return dValue * 2.36; // 2.36 = 6 / 2.54
			case NSCSS::Twips:
				return dValue * 567.; // 1 cm = (1440 / 2.54) twips
			default:
				return dValue;
		}
	}

	double CUnitMeasureConverter::ConvertMm(double dValue, NSCSS::UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		switch (enUnitMeasure)
		{
			case NSCSS::Point:
				return dValue * 2.8346; // 1 mm = (25.4 / 72) points
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
				return dValue * 2.54; // 1 inch = 2.54 cm
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
				return dValue / 72.; // 1 inch = 72 points
			case NSCSS::Peak:
				return dValue * 0.0833; // 0.0833 = 6 / 72 (1 point = 1/72 inch)
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
				return dValue * (double)ushDPI / 6.; // 1 inch = 6 peak
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
				return dValue * (double)ushDPI / 1440.; // 1 inch = 1440 twips
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

		const size_t unLength = wsValue.length();

		size_t unStart = 0, unEnd = 0;

		for (; unStart < unLength; ++unStart)
		{
			const size_t unPos = (L'-' == wsValue[unStart]) ? unStart + 1 : unStart;

			if (unPos + 1 < unLength && L'.' == wsValue[unPos] && iswdigit(wsValue[unPos + 1]))
			{
				unEnd = SkipDigits(wsValue, unPos + 2);
				break;
			}

			if (unPos < unLength && iswdigit(wsValue[unPos]))
			{
				unEnd = SkipDigits(wsValue, unPos + 1);

				if (unEnd + 1 < unLength && L'.' == wsValue[unEnd] && iswdigit(wsValue[unEnd + 1]))
					unEnd = SkipDigits(wsValue, unEnd + 2);

				break;
			}
		}

		if (unStart == unLength)
			return false;

		dValue = std::stod(wsValue.substr(unStart, unEnd - unStart));

		while (unEnd < unLength && iswspace(wsValue[unEnd]))
			++unEnd;

		static const std::vector<std::pair<std::wstring, UnitMeasure>> arUnits{{L"px", Pixel}, {L"pt", Point},   {L"cm", Cantimeter}, {L"mm", Millimeter}, {L"in", Inch},
		                                                                       {L"pc", Peak},  {L"%", Percent}, {L"em", Em},         {L"rem", Rem},        {L"tw", Twips}};

		enUnitMeasure = None;

		for (const std::pair<std::wstring, UnitMeasure>& oUnit : arUnits)
		{
			if (0 == wsValue.compare(unEnd, oUnit.first.length(), oUnit.first))
			{
				enUnitMeasure = oUnit.second;
				break;
			}
		}

		return true;
	}
}
