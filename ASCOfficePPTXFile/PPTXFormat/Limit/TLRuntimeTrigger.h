#pragma once
#ifndef PPTX_LIMIT_TLRUNTIMETRIGGER_INCLUDE_H_
#define PPTX_LIMIT_TLRUNTIMETRIGGER_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLRuntimeTrigger : public BaseLimit
		{
		public:
			TLRuntimeTrigger()
			{
				m_strValue = _T("first");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("all") == strValue) ||
					(_T("first") == strValue) ||
					(_T("last") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLRUNTIMETRIGGER_INCLUDE_H_