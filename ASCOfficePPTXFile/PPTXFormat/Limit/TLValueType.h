#pragma once
#ifndef PPTX_LIMIT_TLVALUETYPE_INCLUDE_H_
#define PPTX_LIMIT_TLVALUETYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLValueType : public BaseLimit
		{
		public:
			TLValueType()
			{
				m_strValue = _T("num");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("num") == strValue) ||
					(_T("clr") == strValue) ||
					(_T("str") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLVALUETYPE_INCLUDE_H_