#pragma once
#ifndef PPTX_LIMIT_EIGHTDIRECTIONVAL_INCLUDE_H_
#define PPTX_LIMIT_EIGHTDIRECTIONVAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class EightDirectionVal : public BaseLimit
		{
		public:
			EightDirectionVal()
			{
				m_strValue = _T("l");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ld") == strValue) ||
					(_T("lu") == strValue) ||
					(_T("rd") == strValue) ||
					(_T("ru") == strValue) ||
					(_T("d") == strValue) ||
					(_T("l") == strValue) ||
					(_T("r") == strValue) ||
					(_T("u") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_EIGHTDIRECTIONVAL_INCLUDE_H_