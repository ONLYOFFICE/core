#pragma once

#ifndef PPTX_LIMIT_CHARTBUILDTYPE_INCLUDE_H_
#define PPTX_LIMIT_CHARTBUILDTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class ChartBuildType : public BaseLimit
		{
		public:
            ChartBuildType()
			{
				m_strValue = _T("allAtOnce");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("allAtOnce") == strValue) ||
					(_T("category") == strValue) ||
					(_T("categoryEl") == strValue) ||
					(_T("series") == strValue) ||
					(_T("seriesEl") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_CHARTBUILDTYPE_INCLUDE_H_
