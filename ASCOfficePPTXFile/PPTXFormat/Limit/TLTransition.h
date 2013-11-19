#pragma once
#ifndef PPTX_LIMIT_TLTRANSITION_INCLUDE_H_
#define PPTX_LIMIT_TLTRANSITION_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLTransition : public BaseLimit
		{
		public:
			TLTransition()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("in") == strValue) ||
					(_T("out") == strValue) ||
					(_T("none") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLTRANSITION_INCLUDE_H_