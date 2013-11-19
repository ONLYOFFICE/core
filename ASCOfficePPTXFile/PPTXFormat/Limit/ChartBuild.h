#pragma once
#ifndef PPTX_LIMIT_CHARTBUILD_INCLUDE_H_
#define PPTX_LIMIT_CHARTBUILD_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class ChartBuild : public BaseLimit
		{
		public:
			ChartBuild()
			{
				m_strValue = _T("allPts");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("allPts") == strValue) ||
					(_T("category") == strValue) ||
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

#endif // PPTX_LIMIT_CHARTBUILD_INCLUDE_H_