#pragma once
#ifndef PPTX_LIMIT_PARABUILDTYPE_INCLUDE_H_
#define PPTX_LIMIT_PARABUILDTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class ParaBuildType : public BaseLimit
		{
		public:
			ParaBuildType()
			{
				m_strValue = _T("whole");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("allAtOnce") == strValue) ||
					(_T("cust") == strValue) ||
					(_T("p") == strValue) ||
					(_T("whole") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PARABUILDTYPE_INCLUDE_H_