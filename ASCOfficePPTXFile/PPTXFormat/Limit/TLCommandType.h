#pragma once
#ifndef PPTX_LIMIT_TLCOMMANDTYPE_INCLUDE_H_
#define PPTX_LIMIT_TLCOMMANDTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLCommandType : public BaseLimit
		{
		public:
			TLCommandType()
			{
				m_strValue = _T("verb");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("call") == strValue) ||
					(_T("evt") == strValue) ||
					(_T("verb") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLCOMMANDTYPE_INCLUDE_H_