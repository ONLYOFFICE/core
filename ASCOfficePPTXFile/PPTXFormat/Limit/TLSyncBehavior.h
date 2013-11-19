#pragma once
#ifndef PPTX_LIMIT_TLSYNCBEHAVIOR_INCLUDE_H_
#define PPTX_LIMIT_TLSYNCBEHAVIOR_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLSyncBehavior : public BaseLimit
		{
		public:
			TLSyncBehavior()
			{
				m_strValue = _T("canSlip");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("canSlip") == strValue) ||
					(_T("locked") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLSYNCBEHAVIOR_INCLUDE_H_