#pragma once
#ifndef PPTX_LIMIT_FONTSTYLEINDEX_INCLUDE_H_
#define PPTX_LIMIT_FONTSTYLEINDEX_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class FontStyleIndex : public BaseLimit
		{
		public:
			FontStyleIndex()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("major") == strValue) ||
					(_T("minor") == strValue) ||
					(_T("none") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("major") == m_strValue)
					return 0;
				if (_T("minor") == m_strValue)
					return 1;
				if (_T("none") == m_strValue)
					return 2;
				return 2;
			}

			virtual void SetBYTECode(const BYTE& src)
			{		
				switch (src)
				{
				case 0:
					m_strValue = _T("major");
					break;
				case 1:
					m_strValue = _T("minor");
					break;
				default:
					m_strValue = _T("none");
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FONTSTYLEINDEX_INCLUDE_H_