#pragma once
#ifndef PPTX_LIMIT_TLADDITIVE_INCLUDE_H_
#define PPTX_LIMIT_TLADDITIVE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLAdditive : public BaseLimit
		{
		public:
			TLAdditive()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("base") == strValue) ||
					(_T("mult") == strValue) ||
					(_T("none") == strValue) ||
					(_T("repl") == strValue) ||
					(_T("sum") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLADDITIVE_INCLUDE_H_