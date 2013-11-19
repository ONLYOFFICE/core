#pragma once
#ifndef PPTX_LIMIT_FILLPATH_INCLUDE_H_
#define PPTX_LIMIT_FILLPATH_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class FillPath : public BaseLimit
		{
		public:
			FillPath()
			{
				m_strValue = _T("circle");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("circle") == strValue) ||
					(_T("rect") == strValue) ||
					(_T("shape") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("circle") == m_strValue)
					return 0;
				if (_T("rect") == m_strValue)
					return 1;
				if (_T("shape") == m_strValue)
					return 2;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("circle");
					break;
				case 1:
					m_strValue = _T("rect");
					break;
				case 2:
					m_strValue = _T("shape");
					break;
				default:
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FILLPATH_INCLUDE_H_