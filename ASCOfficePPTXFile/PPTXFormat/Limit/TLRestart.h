#pragma once
#ifndef PPTX_LIMIT_TLRESTART_INCLUDE_H_
#define PPTX_LIMIT_TLRESTART_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLRestart : public BaseLimit
		{
		public:
			TLRestart()
			{
				m_strValue = _T("never");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("always") == strValue) ||
					(_T("never") == strValue) ||
					(_T("whenNotActive") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLRESTART_INCLUDE_H_