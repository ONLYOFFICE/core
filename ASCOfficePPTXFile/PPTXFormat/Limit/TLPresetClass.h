#pragma once
#ifndef PPTX_LIMIT_TLPRESETCLASS_INCLUDE_H_
#define PPTX_LIMIT_TLPRESETCLASS_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLPresetClass : public BaseLimit
		{
		public:
			TLPresetClass()
			{
				m_strValue = _T("exit");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("emph") == strValue) ||
					(_T("entr") == strValue) ||
					(_T("exit") == strValue) ||
					(_T("mediacall") == strValue) ||
					(_T("path") == strValue) ||
					(_T("verb") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLPRESETCLASS_INCLUDE_H_