#pragma once
#ifndef PPTX_LIMIT_TLPATHEDITMODE_INCLUDE_H_
#define PPTX_LIMIT_TLPATHEDITMODE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLPathEditMode : public BaseLimit
		{
		public:
			TLPathEditMode()
			{
				m_strValue = _T("relative");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("fixed") == strValue) ||
					(_T("relative") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLPATHEDITMODE_INCLUDE_H_