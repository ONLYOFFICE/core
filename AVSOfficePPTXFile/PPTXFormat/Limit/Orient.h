#pragma once
#ifndef PPTX_LIMIT_ORIENT_INCLUDE_H_
#define PPTX_LIMIT_ORIENT_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class Orient : public BaseLimit
		{
		public:
			Orient()
			{
				m_strValue = _T("vert");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("horz") == strValue) ||
					(_T("vert") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("horz") == m_strValue)
					return 0;
				return 1;
			}

			virtual void SetBYTECode(const BYTE& src)
			{		
				if (0 == src)
					m_strValue = _T("horz");
				else
					m_strValue = _T("vert");
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_ORIENT_INCLUDE_H_