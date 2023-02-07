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

	std::wstring CUnitMeasureConverter::ConvertUnitMeasure(const std::wstring &wsValue, double dPreviousValue, UnitMeasure enUnitMeasure, unsigned short ushDPI)
	{
		if (wsValue.empty())
			return std::wstring();

		std::wstring wsNewValue;

		std::vector<std::wstring> arValues = NSCSS::NS_STATIC_FUNCTIONS::GetWordsWithSigns(wsValue);

		for (std::wstring& wsValueTemp : arValues)
		{
			std::transform(wsValueTemp.begin(), wsValueTemp.end(), wsValueTemp.begin(), tolower);

			if (wsValueTemp == L"important")
			{
				wsNewValue += L"!important";
				continue;
			}

			size_t nPosGrid = wsValueTemp.find(L'#');

			if (nPosGrid != std::wstring::npos || !NSCSS::NS_STATIC_FUNCTIONS::NumberInWString(wsValueTemp))
			{
				if (!NSCSS::NS_STATIC_FUNCTIONS::ConvertAbsoluteValue(wsValueTemp, dPreviousValue))
				{
					wsNewValue += wsValueTemp;
					continue;
				}
			}

			double dValue = NSCSS::NS_STATIC_FUNCTIONS::ReadDouble(wsValueTemp);

			const size_t posPercent = wsValueTemp.find(L'%');

			if (posPercent != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(dValue * dPreviousValue / 100.);

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"px") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertPx(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"cm") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertCm(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"mm") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertMm(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"in") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertIn(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"pt") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertPt(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"pc") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(ConvertPc(dValue, enUnitMeasure, ushDPI));

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else if (wsValueTemp.find(L"em") != std::wstring::npos)
			{
				wsNewValue += std::to_wstring(dValue * dPreviousValue);

				if (wsValueTemp.find(L';') != std::wstring::npos)
					wsNewValue += L';';
				else if (arValues.size() > 1 && wsValueTemp.find(L':') == std::wstring::npos)
					wsNewValue += L' ';
			}
			else
			{
				wsNewValue += wsValueTemp;

				if (wsValueTemp.find(L";") != std::wstring::npos)
					wsNewValue += L';';

				continue;
			}

			if (wsValueTemp.back() != L';' && wsValueTemp.back() != L':' && wsValueTemp.back() != L' ')
				wsValueTemp += L' ';
		}

		return wsNewValue;
	}

	bool CUnitMeasureConverter::GetValue(const std::wstring &wsValue, double &dValue, UnitMeasure &enUnitMeasure)
	{
		std::wstring::const_iterator oFoundDigit = std::find_if(wsValue.begin(), wsValue.end(), std::iswdigit);

		if (wsValue.end() == oFoundDigit)
			return false;

		std::wistringstream(wsValue) >> dValue;

		std::wstring::const_iterator oFoundUM = std::find_if(oFoundDigit, wsValue.end(), [](wchar_t wcSymbol){ return std::iswalpha(wcSymbol) || L'%' == wcSymbol; });

		if (wsValue.end() != oFoundUM)
		{
			if (L'%' == *oFoundUM)
			{
				enUnitMeasure = Percent;
				return true;
			}

			std::wstring wsUnitMeasure(oFoundUM, oFoundUM + 2);

			if (L"px" == wsUnitMeasure)
				enUnitMeasure = Pixel;
			else if (L"pt" == wsUnitMeasure)
				enUnitMeasure = Point;
			else if (L"cm" == wsUnitMeasure)
				enUnitMeasure = Cantimeter;
			else if (L"mm" == wsUnitMeasure)
				enUnitMeasure = Millimeter;
			else if (L"in" == wsUnitMeasure)
				enUnitMeasure = Inch;
			else if (L"pc" == wsUnitMeasure)
				enUnitMeasure = Peak;
		}
		else
			enUnitMeasure = None;

		return true;
	}
}
