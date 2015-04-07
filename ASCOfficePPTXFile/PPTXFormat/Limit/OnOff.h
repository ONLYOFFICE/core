#pragma once
#ifndef PPTX_LIMIT_ONOFF_INCLUDE_H_
#define PPTX_LIMIT_ONOFF_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class OnOff : public BaseLimit
		{
		public:
			OnOff()
			{
				m_strValue = _T("def");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("on") == strValue) ||
					(_T("off") == strValue) ||
					(_T("def") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("on") == m_strValue)
					return 0;
				if (_T("off") == m_strValue)
					return 1;
				if (_T("def") == m_strValue)
					return 2;
				return 2;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("on");
					break;
				case 1:
					m_strValue = _T("off");
					break;
				default:
					m_strValue = _T("def");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_ONOFF_INCLUDE_H_