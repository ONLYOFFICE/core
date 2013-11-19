#pragma once
#ifndef PPTX_LIMIT_TEXTSTRIKE_INCLUDE_H_
#define PPTX_LIMIT_TEXTSTRIKE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextStrike : public BaseLimit
		{
		public:
			TextStrike()
			{
				m_strValue = _T("noStrike");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("dblStrike") == strValue) ||
					(_T("noStrike") == strValue) ||
					(_T("sngStrike") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("dblStrike") == m_strValue)
					return 0;
				if (_T("noStrike") == m_strValue)
					return 1;
				if (_T("sngStrike") == m_strValue)
					return 2;
				return 1;
			}

			virtual void SetBYTECode(const BYTE& src)
			{	
				switch (src)
				{
				case 0:
					m_strValue = _T("dblStrike");
					break;
				case 2:
					m_strValue = _T("sngStrike");
					break;
				default:
					m_strValue = _T("noStrike");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTSTRIKE_INCLUDE_H_