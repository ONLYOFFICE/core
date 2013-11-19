#pragma once
#ifndef PPTX_LIMIT_TLCHARTSUBELEMENT_INCLUDE_H_
#define PPTX_LIMIT_TLCHARTSUBELEMENT_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLChartSubElement : public BaseLimit
		{
		public:
			TLChartSubElement()
			{
				m_strValue = _T("series");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("category") == strValue) ||
					(_T("gridLegend") == strValue) ||
					(_T("ptInCategory") == strValue) ||
					(_T("ptInSeries") == strValue) ||
					(_T("series") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLCHARTSUBELEMENT_INCLUDE_H_