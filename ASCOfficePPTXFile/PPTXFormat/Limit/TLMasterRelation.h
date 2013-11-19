#pragma once
#ifndef PPTX_LIMIT_TLMASTERRELATION_INCLUDE_H_
#define PPTX_LIMIT_TLMASTERRELATION_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLMasterRelation : public BaseLimit
		{
		public:
			TLMasterRelation()
			{
				m_strValue = _T("sameClick");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("lastClick") == strValue) ||
					(_T("nextClick") == strValue) ||
					(_T("sameClick") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLMASTERRELATION_INCLUDE_H_