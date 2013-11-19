#pragma once
#ifndef PPTX_LIMIT_INOUTDIRECTIONVAL_INCLUDE_H_
#define PPTX_LIMIT_INOUTDIRECTIONVAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class InOutDirectionVal : public BaseLimit
		{
		public:
			InOutDirectionVal()
			{
				m_strValue = _T("out");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("in") == strValue) ||
					(_T("out") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_INOUTDIRECTIONVAL_INCLUDE_H_