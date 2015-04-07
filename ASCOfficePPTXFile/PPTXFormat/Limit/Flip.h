#pragma once
#ifndef PPTX_LIMIT_FLIP_INCLUDE_H_
#define PPTX_LIMIT_FLIP_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class Flip : public BaseLimit
		{
		public:
			Flip()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none") == strValue) ||
					(_T("x") == strValue) ||
					(_T("xy") == strValue) ||
					(_T("y") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("none") == m_strValue)
					return 0;
				if (_T("x") == m_strValue)
					return 1;
				if (_T("y") == m_strValue)
					return 2;
				if (_T("xy") == m_strValue)
					return 3;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("none");
					break;
				case 1:
					m_strValue = _T("x");
					break;
				case 2:
					m_strValue = _T("y");
					break;
				case 3:
					m_strValue = _T("xy");
					break;
				default:
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FLIP_INCLUDE_H_