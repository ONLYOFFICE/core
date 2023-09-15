#ifndef CUNITMEASURECONVERTER_H
#define CUNITMEASURECONVERTER_H

#include <string>

namespace NSCSS
{
	typedef enum
	{
		None,
		Percent,
		Pixel,
		Point,
		Cantimeter,
		Millimeter,
		Inch,
		Peak,
		Twips
	} UnitMeasure;

	class CUnitMeasureConverter
	{
		CUnitMeasureConverter();
	public:
		static double ConvertPx(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);
		static double ConvertCm(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);
		static double ConvertMm(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);
		static double ConvertIn(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);
		static double ConvertPt(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);
		static double ConvertPc(double dValue, UnitMeasure enUnitMeasure, unsigned short ushDPI);

		static std::wstring UMtoWString(UnitMeasure enFontSizeUM);
		static bool GetValue(const std::wstring& wsValue, double& dValue, UnitMeasure& enUnitMeasure);
		static void ReplaceEmValues(std::wstring& wsValue, double dFontSize, UnitMeasure enFontSizeUM);
	};
}

#endif // CUNITMEASURECONVERTER_H
