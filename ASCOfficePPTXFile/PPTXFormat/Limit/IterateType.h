#pragma once
#ifndef PPTX_LIMIT_ITERATETYPE_INCLUDE_H_
#define PPTX_LIMIT_ITERATETYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class IterateType : public BaseLimit
		{
		public:
			IterateType()
			{
				m_strValue = _T("el");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("el") == strValue) ||
					(_T("lt") == strValue) ||
					(_T("wd") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_ITERATETYPE_INCLUDE_H_