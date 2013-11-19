#pragma once
#ifndef PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_
#define PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class SplitterBarState : public BaseLimit
		{
		public:
			SplitterBarState()
			{
				m_strValue = _T("restored");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("maximized") == strValue) ||
					(_T("minimized") == strValue) ||
					(_T("restored") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("maximized") == m_strValue)
					return 0;
				if (_T("minimized") == m_strValue)
					return 1;
				if (_T("restored") == m_strValue)
					return 2;
				return 2;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_