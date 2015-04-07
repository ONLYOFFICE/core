#pragma once
#ifndef PPTX_LIMIT_TEXTWRAP_INCLUDE_H_
#define PPTX_LIMIT_TEXTWRAP_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextWrap : public BaseLimit
		{
		public:
			TextWrap()
			{
				m_strValue = _T("square");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none") == strValue) ||
					(_T("square") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("none") == m_strValue)
					return 0;
				if (_T("square") == m_strValue)
					return 1;
				return 1;
			}
			virtual void SetBYTECode(const BYTE& src)
			{
				if (0 == src)
					m_strValue = _T("none");
				else
					m_strValue = _T("square");
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTWRAP_INCLUDE_H_