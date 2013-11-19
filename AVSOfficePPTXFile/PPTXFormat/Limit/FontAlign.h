#pragma once
#ifndef PPTX_LIMIT_FONTALIGN_INCLUDE_H_
#define PPTX_LIMIT_FONTALIGN_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class FontAlign : public BaseLimit
		{
		public:
			FontAlign()
			{
				m_strValue = _T("auto");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("auto") == strValue) ||
					(_T("b") == strValue) ||
					(_T("base") == strValue) ||
					(_T("ctr") == strValue) ||
					(_T("t") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("auto") == m_strValue)
					return 0;
				if (_T("b") == m_strValue)
					return 1;
				if (_T("base") == m_strValue)
					return 2;
				if (_T("ctr") == m_strValue)
					return 3;
				if (_T("t") == m_strValue)
					return 4;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& val)
			{
				switch (val)
				{
				case 0:
					m_strValue = _T("auto");
					break;
				case 1:
					m_strValue = _T("b");
					break;
				case 2:
					m_strValue = _T("base");
					break;
				case 3:
					m_strValue = _T("ctr");
					break;
				case 4:
					m_strValue = _T("t");
					break;
				default:
					m_strValue = _T("auto");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FONTALIGN_INCLUDE_H_