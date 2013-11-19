#pragma once
#ifndef PPTX_LIMIT_TEXTTABALIGNTYPE_INCLUDE_H_
#define PPTX_LIMIT_TEXTTABALIGNTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextTabAlignType : public BaseLimit
		{
		public:
			TextTabAlignType()
			{
				m_strValue = _T("r");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ctr") == strValue) ||
					(_T("dec") == strValue) ||
					(_T("l") == strValue) ||
					(_T("r") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("ctr") == m_strValue)
					return 0;
				if (_T("dec") == m_strValue)
					return 1;
				if (_T("l") == m_strValue)
					return 2;
				if (_T("r") == m_strValue)
					return 3;
				return 2;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("ctr");
					break;
				case 1:
					m_strValue = _T("dec");
					break;
				case 2:
					m_strValue = _T("l");
					break;
				case 3:
					m_strValue = _T("r");
					break;
				default:
					m_strValue = _T("l");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTTABALIGNTYPE_INCLUDE_H_