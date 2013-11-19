#pragma once
#ifndef PPTX_LIMIT_LINEENDTYPE_INCLUDE_H_
#define PPTX_LIMIT_LINEENDTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class LineEndType : public BaseLimit
		{
		public:
			LineEndType()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none") == strValue) ||
					(_T("arrow") == strValue) ||
					(_T("diamond") == strValue) ||
					(_T("oval") == strValue) ||
					(_T("stealth") == strValue) ||
					(_T("triangle") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("none") == m_strValue)
					return 0;
				if (_T("arrow") == m_strValue)
					return 1;
				if (_T("diamond") == m_strValue)
					return 2;
				if (_T("oval") == m_strValue)
					return 3;
				if (_T("stealth") == m_strValue)
					return 4;
				if (_T("triangle") == m_strValue)
					return 5;
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
					m_strValue = _T("arrow");
					break;
				case 2:
					m_strValue = _T("diamond");
					break;
				case 3:
					m_strValue = _T("oval");
					break;
				case 4:
					m_strValue = _T("stealth");
					break;
				case 5:
					m_strValue = _T("triangle");
					break;
				default:
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_LINEENDTYPE_INCLUDE_H_