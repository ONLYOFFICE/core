#pragma once
#ifndef PPTX_LIMIT_LINEENDSIZE_INCLUDE_H_
#define PPTX_LIMIT_LINEENDSIZE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class LineEndSize : public BaseLimit
		{
		public:
			LineEndSize()
			{
				m_strValue = _T("med");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("lg") == strValue) ||
					(_T("med") == strValue) ||
					(_T("sm") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("lg") == m_strValue)
					return 0;
				if (_T("med") == m_strValue)
					return 1;
				if (_T("sm") == m_strValue)
					return 2;				
				return 1;
			}
			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("lg");
					break;
				case 1:
					m_strValue = _T("med");
					break;
				case 2:
					m_strValue = _T("sm");
					break;
				default:
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_LINEENDSIZE_INCLUDE_H_