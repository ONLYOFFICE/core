#pragma once
#ifndef PPTX_LIMIT_TLPREVAC_INCLUDE_H_
#define PPTX_LIMIT_TLPREVAC_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLPrevAc : public BaseLimit
		{
		public:
			TLPrevAc()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none") == strValue) ||
					(_T("skipTimed") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLPREVAC_INCLUDE_H_