#pragma once
#ifndef PPTX_LIMIT_TRANSITIONSPEED_INCLUDE_H_
#define PPTX_LIMIT_TRANSITIONSPEED_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TransitionSpeed : public BaseLimit
		{
		public:
			TransitionSpeed()
			{
				m_strValue = _T("fast");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("fast") == strValue) ||
					(_T("med") == strValue) ||
					(_T("slow") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("fast") == m_strValue)
					return 0;
				if (_T("med") == m_strValue)
					return 1;
				if (_T("slow") == m_strValue)
					return 2;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("fast");
					break;
				case 1:
					m_strValue = _T("med");
					break;
				case 2:
					m_strValue = _T("slow");
					break;
				default:
					m_strValue = _T("fast");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TRANSITIONSPEED_INCLUDE_H_