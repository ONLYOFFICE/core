#pragma once
#ifndef PPTX_LIMIT_CORNERDIRECTIONVAL_INCLUDE_H_
#define PPTX_LIMIT_CORNERDIRECTIONVAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class CornerDirectionVal : public BaseLimit
		{
		public:
			CornerDirectionVal()
			{
				m_strValue = _T("lu");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ld") == strValue) ||
					(_T("lu") == strValue) ||
					(_T("rd") == strValue) ||
					(_T("ru") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_CORNERDIRECTIONVAL_INCLUDE_H_