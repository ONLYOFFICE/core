#pragma once
#ifndef PPTX_LIMIT_TLNEXTAC_INCLUDE_H_
#define PPTX_LIMIT_TLNEXTAC_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLNextAc : public BaseLimit
		{
		public:
			TLNextAc()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none") == strValue) ||
					(_T("seek") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLNEXTAC_INCLUDE_H_