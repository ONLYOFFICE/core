#pragma once
#ifndef PPTX_LIMIT_VERTOVERFLOW_INCLUDE_H_
#define PPTX_LIMIT_VERTOVERFLOW_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class VertOverflow : public BaseLimit
		{
		public:
			VertOverflow()
			{
				m_strValue = _T("clip");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("clip") == strValue) ||
					(_T("ellipsis") == strValue) ||
					(_T("overflow") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("clip") == m_strValue)
					return 0;
				if (_T("ellipsis") == m_strValue)
					return 1;
				if (_T("overflow") == m_strValue)
					return 2;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				if (2 == src)
					m_strValue = _T("overflow");
				else if (1 == src)
					m_strValue = _T("ellipsis");
				else
					m_strValue = _T("clip");
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_VERTOVERFLOW_INCLUDE_H_