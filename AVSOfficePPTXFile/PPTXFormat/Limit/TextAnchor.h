#pragma once
#ifndef PPTX_LIMIT_TEXTANCHOR_INCLUDE_H_
#define PPTX_LIMIT_TEXTANCHOR_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextAnchor : public BaseLimit
		{
		public:
			TextAnchor()
			{
				m_strValue = _T("t");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("b") == strValue) ||
					(_T("ctr") == strValue) ||
					(_T("dist") == strValue) ||
					(_T("just") == strValue) ||
					(_T("t") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("b") == m_strValue)
					return 0;
				if (_T("ctr") == m_strValue)
					return 1;
				if (_T("dist") == m_strValue)
					return 2;
				if (_T("just") == m_strValue)
					return 3;
				if (_T("t") == m_strValue)
					return 4;
				return 4;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("b");
					break;
				case 1:
					m_strValue = _T("ctr");
					break;
				case 2:
					m_strValue = _T("dist");
					break;
				case 3:
					m_strValue = _T("just");
					break;
				default:
					m_strValue = _T("t");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTANCHOR_INCLUDE_H_