#pragma once
#ifndef PPTX_LIMIT_TLCOLORSPACE_INCLUDE_H_
#define PPTX_LIMIT_TLCOLORSPACE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLColorSpace : public BaseLimit
		{
		public:
			TLColorSpace()
			{
				m_strValue = _T("rgb");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("rgb") == strValue) ||
					(_T("hsl") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLCOLORSPACE_INCLUDE_H_