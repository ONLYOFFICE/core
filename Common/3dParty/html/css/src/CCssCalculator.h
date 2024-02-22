#ifndef CCSSCALCULATOR_H
#define CCSSCALCULATOR_H

#include "CssCalculator_global.h"
#include "CCompiledStyle.h"
#include "ConstValues.h"
#include <iostream>
#include <map>
#include <vector>

namespace NSCSS
{
	class CCssCalculator_Private;
	class CSSCALCULATOR_EXPORT CCssCalculator
	{
	protected:
		CCssCalculator_Private* m_pInternal;
	public:
		CCssCalculator();
		~CCssCalculator();

		CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const bool& bIsSettings = false, const UnitMeasure& unitMeasure = Point) const;
		bool GetCompiledStyle(CCompiledStyle& oStyle, const std::vector<CNode> &arSelectors, const bool& bIsSettings = false, const UnitMeasure& unitMeasure = Point) const;

		bool CalculatePageStyle(NSProperties::CPage& oPageData, const std::vector<CNode> &arSelectors);

		// void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
		void AddStyles        (const std::string&  sStyle);
		void AddStyles        (const std::wstring& wsStyle);
		void AddStylesFromFile(const std::wstring& wsFileName);

		void SetUnitMeasure(const UnitMeasure& nType);
		void SetDpi(const unsigned short int& nValue);
		void SetBodyTree(const CTree &oTree);

		UnitMeasure GetUnitMeasure() const;
		std::wstring GetEncoding()   const;
		unsigned short int GetDpi()  const;

		void Clear();
	};
}

#endif // CCSSCALCULATOR_H
