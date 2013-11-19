#pragma once
#ifndef PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_
#define PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class PlaceholderSize : public BaseLimit
		{
		public:
			PlaceholderSize()
			{
				m_strValue = _T("full");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("full") == strValue) ||
					(_T("half") == strValue) ||
					(_T("quarter") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("full") == m_strValue)
					return 0;
				if (_T("half") == m_strValue)
					return 1;
				if (_T("quarter") == m_strValue)
					return 2;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{		
				switch (src)
				{
				case 1:
					m_strValue = _T("half");
					break;
				case 2:
					m_strValue = _T("quarter");
					break;
				default:
					m_strValue = _T("full");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_