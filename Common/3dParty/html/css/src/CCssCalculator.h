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

		bool CalculateCompiledStyle(std::vector<CNode>& arSelectors) const;

		std::wstring CalculateStyleId(const CNode& oNode);
		bool CalculatePageStyle(NSProperties::CPage& oPageData, const std::vector<CNode> &arSelectors);

		// void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
		void AddStyles        (const std::string&  sStyle);
		void AddStyles        (const std::wstring& wsStyle);
		void AddStylesFromFile(const std::wstring& wsFileName);

		void SetDpi(const unsigned short int& nValue);

		std::wstring GetEncoding()   const;
		unsigned short int GetDpi()  const;

		void Clear();
	};
}

#endif // CCSSCALCULATOR_H
