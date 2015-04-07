#pragma once
#ifndef PPTX_LIMIT_RECTALIGN_INCLUDE_H_
#define PPTX_LIMIT_RECTALIGN_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class RectAlign : public BaseLimit
		{
		public:
			RectAlign()
			{
				m_strValue = _T("tl");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("b") == strValue) ||
					(_T("bl") == strValue) ||
					(_T("br") == strValue) ||
					(_T("ctr") == strValue) ||
					(_T("l") == strValue) ||
					(_T("r") == strValue) ||
					(_T("t") == strValue) ||
					(_T("tl") == strValue) ||
					(_T("tr") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("b") == m_strValue)
					return 0;
				if (_T("bl") == m_strValue)
					return 1;
				if (_T("br") == m_strValue)
					return 2;
				if (_T("ctr") == m_strValue)
					return 3;
				if (_T("l") == m_strValue)
					return 4;
				if (_T("r") == m_strValue)
					return 5;
				if (_T("t") == m_strValue)
					return 6;
				if (_T("tl") == m_strValue)
					return 7;
				if (_T("tr") == m_strValue)
					return 8;
				return 7;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_RECTALIGN_INCLUDE_H_