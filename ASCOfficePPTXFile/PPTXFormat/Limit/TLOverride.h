#pragma once
#ifndef PPTX_LIMIT_TLOVERRIDE_INCLUDE_H_
#define PPTX_LIMIT_TLOVERRIDE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLOverride : public BaseLimit
		{
		public:
			TLOverride()
			{
				m_strValue = _T("normal");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("childStyle") == strValue) ||
					(_T("normal") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLOVERRIDE_INCLUDE_H_