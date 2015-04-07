#pragma once
#ifndef PPTX_LIMIT_CONTENT_STATUS_INCLUDE_H_
#define PPTX_LIMIT_CONTENT_STATUS_INCLUDE_H_

#include "BaseLimit.h"



namespace PPTX
{
	namespace Limit
	{
		class ContentStatus : public BaseLimit
		{
		public:
			ContentStatus()
			{
				m_strValue = _T("Draft");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("Draft") == strValue) ||
					(_T("Reviewed") == strValue) ||
					(_T("Final") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_CONTENT_STATUS_INCLUDE_H_