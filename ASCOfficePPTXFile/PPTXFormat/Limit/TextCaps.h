#pragma once
#ifndef PPTX_LIMIT_TEXTCAPS_INCLUDE_H_
#define PPTX_LIMIT_TEXTCAPS_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextCaps : public BaseLimit
		{
		public:
			TextCaps()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("all") == strValue) ||
					(_T("none") == strValue) ||
					(_T("small") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("all") == m_strValue)
					return 0;
				if (_T("small") == m_strValue)
					return 1;
				return 2;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("all");
					break;
				case 1:
					m_strValue = _T("small");
					break;
				default:
					m_strValue = _T("none");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTCAPS_INCLUDE_H_