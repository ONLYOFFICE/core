#pragma once
#ifndef PPTX_LIMIT_TLACCUMULATE_INCLUDE_H_
#define PPTX_LIMIT_TLACCUMULATE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLAccumulate : public BaseLimit
		{
		public:
			TLAccumulate()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("always") == strValue) ||
					(_T("none") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLACCUMULATE_INCLUDE_H_