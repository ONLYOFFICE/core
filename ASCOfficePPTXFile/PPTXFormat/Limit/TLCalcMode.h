#pragma once
#ifndef PPTX_LIMIT_TLCALCMODE_INCLUDE_H_
#define PPTX_LIMIT_TLCALCMODE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLCalcMode : public BaseLimit
		{
		public:
			TLCalcMode()
			{
				m_strValue = _T("lin");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("lin") == strValue) ||
					(_T("fmla") == strValue) ||
					(_T("discrete") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLCALCMODE_INCLUDE_H_