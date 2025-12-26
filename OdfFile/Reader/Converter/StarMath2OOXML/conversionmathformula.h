#ifndef CONVERSIONMATHFORMULA_H
#define CONVERSIONMATHFORMULA_H

#ifndef STARMATH_USE_DYNAMIC_LIBRARY
#define STARMATH_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define STARMATH_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "TFormulaSize.h"
#include <string>
#include <queue>

namespace StarMath
{
	class CParserStarMathString;
	class STARMATH_DECL_EXPORT CStarMathConverter
	{
		public:
			CStarMathConverter();
			~CStarMathConverter();
			void SetBaseFont(const std::wstring& wsNameFont);
			void SetBaseSize(const unsigned int& nSize);
			void SetBaseAlignment(const unsigned int& nAlignment);
			void SetBaseItalic(bool bItal);
			void SetBaseBold(bool bBold);
			std::queue<TFormulaSize> GetFormulaSize();
			std::wstring ConvertStarMathToOOXml(const std::wstring& wsFormula,const unsigned int& iTypeConversion = 0);
			std::wstring ConvertEQNToOOXml(const std::wstring& wsFormula);
		private:
			CParserStarMathString* m_pParser;
	};

}

#endif // CONVERSIONMATHFORMULA_H
