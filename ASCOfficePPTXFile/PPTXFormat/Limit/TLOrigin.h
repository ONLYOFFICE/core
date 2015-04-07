#pragma once
#ifndef PPTX_LIMIT_TLORIGIN_INCLUDE_H_
#define PPTX_LIMIT_TLORIGIN_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLOrigin : public BaseLimit
		{
		public:
			TLOrigin()
			{
				m_strValue = _T("parent");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("parent") == strValue) ||
					(_T("layout") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLORIGIN_INCLUDE_H_