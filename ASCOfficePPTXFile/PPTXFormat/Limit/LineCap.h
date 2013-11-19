#pragma once
#ifndef PPTX_LIMIT_LINECAP_INCLUDE_H_
#define PPTX_LIMIT_LINECAP_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class LineCap : public BaseLimit
		{
		public:
			LineCap()
			{
				m_strValue = _T("flat");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("flat") == strValue) ||
					(_T("rnd") == strValue) ||
					(_T("sq") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("flat") == m_strValue)
					return 0;
				if (_T("rnd") == m_strValue)
					return 1;
				if (_T("sq") == m_strValue)
					return 2;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 1:
					m_strValue = _T("rnd");
					break;
				case 2:
					m_strValue = _T("sq");
					break;
				default:
					m_strValue = _T("flat");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_LINECAP_INCLUDE_H_