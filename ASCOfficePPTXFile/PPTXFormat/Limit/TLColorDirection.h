#pragma once
#ifndef PPTX_LIMIT_TLCOLORDIRECTION_INCLUDE_H_
#define PPTX_LIMIT_TLCOLORDIRECTION_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLColorDirection : public BaseLimit
		{
		public:
			TLColorDirection()
			{
				m_strValue = _T("cw");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ccw") == strValue) ||
					(_T("cw") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLCOLORDIRECTION_INCLUDE_H_