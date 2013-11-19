#pragma once
#ifndef PPTX_LIMIT_TEXTALIGN_INCLUDE_H_
#define PPTX_LIMIT_TEXTALIGN_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextAlign : public BaseLimit
		{
		public:
			TextAlign()
			{
				m_strValue = _T("l");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ctr") == strValue) ||
					(_T("dist") == strValue) ||
					(_T("just") == strValue) ||
					(_T("justLow") == strValue) ||
					(_T("l") == strValue) ||
					(_T("r") == strValue) ||
					(_T("thaiDist") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("ctr") == m_strValue)
					return 0;
				if (_T("dist") == m_strValue)
					return 1;
				if (_T("just") == m_strValue)
					return 2;
				if (_T("justLow") == m_strValue)
					return 3;
				if (_T("l") == m_strValue)
					return 4;
				if (_T("r") == m_strValue)
					return 5;
				if (_T("thaiDist") == m_strValue)
					return 6;
				return 4;
			}

			virtual void SetBYTECode(const BYTE& val)
			{
				switch (val)
				{
				case 0:
					m_strValue = _T("ctr");
					break;
				case 1:
					m_strValue = _T("dist");
					break;
				case 2:
					m_strValue = _T("just");
					break;
				case 3:
					m_strValue = _T("justLow");
					break;
				case 4:
					m_strValue = _T("l");
					break;
				case 5:
					m_strValue = _T("r");
					break;
				case 6:
					m_strValue = _T("thaiDist");
					break;
				default:
					m_strValue = _T("l");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTALIGN_INCLUDE_H_