#pragma once
#ifndef PPTX_LIMIT_VARIANT_TYPE_INCLUDE_H_
#define PPTX_LIMIT_VARIANT_TYPE_INCLUDE_H_

#include "BaseLimit.h"



namespace PPTX
{
	namespace Limit
	{
		class VariantType : public BaseLimit
		{
		public:
			VariantType()
			{
				m_strValue = _T("lpstr");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("lpstr") == strValue) ||
					(_T("i4") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_VARIANT_TYPE_INCLUDE_H_