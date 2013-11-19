#pragma once
#ifndef PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_
#define PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextUnderline : public BaseLimit
		{
		public:
			TextUnderline()
			{
				m_strValue = _T("sng");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("dash") == strValue) ||
					(_T("dashHeavy") == strValue) ||
					(_T("dashLong") == strValue) ||
					(_T("dashLongHeavy") == strValue) ||
					(_T("dbl") == strValue) ||
					(_T("dotDash") == strValue) ||
					(_T("dotDashHeavy") == strValue) ||
					(_T("dotDotDash") == strValue) ||
					(_T("dotDotDashHeavy") == strValue) ||
					(_T("dotted") == strValue) ||
					(_T("dottedHeavy") == strValue) ||
					(_T("heavy") == strValue) ||
					(_T("none") == strValue) ||
					(_T("sng") == strValue) ||
					(_T("wavy") == strValue) ||
					(_T("wavyDbl") == strValue) ||
					(_T("wavyHeavy") == strValue) ||
					(_T("words") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("dash") == m_strValue)
					return 0;
				if (_T("dashHeavy") == m_strValue)
					return 1;
				if (_T("dashLong") == m_strValue)
					return 2;
				if (_T("dashLongHeavy") == m_strValue)
					return 3;
				if (_T("dbl") == m_strValue)
					return 4;
				if (_T("dotDash") == m_strValue)
					return 5;
				if (_T("dotDashHeavy") == m_strValue)
					return 6;
				if (_T("dotDotDash") == m_strValue)
					return 7;
				if (_T("dotDotDashHeavy") == m_strValue)
					return 8;
				if (_T("dotted") == m_strValue)
					return 9;
				if (_T("dottedHeavy") == m_strValue)
					return 10;
				if (_T("heavy") == m_strValue)
					return 11;
				if (_T("none") == m_strValue)
					return 12;
				if (_T("sng") == m_strValue)
					return 13;
				if (_T("wavy") == m_strValue)
					return 14;
				if (_T("wavyDbl") == m_strValue)
					return 15;
				if (_T("wavyHeavy") == m_strValue)
					return 16;
				if (_T("words") == m_strValue)
					return 17;
				return 13;
			}

			virtual void SetBYTECode(const BYTE& src)
			{	
				switch (src)
				{
					case 0:		m_strValue = _T("dash"); break;
					case 1:		m_strValue = _T("dashHeavy"); break;
					case 2:		m_strValue = _T("dashLong"); break;
					case 3:		m_strValue = _T("dashLongHeavy"); break;
					case 4:		m_strValue = _T("dbl"); break;
					case 5:		m_strValue = _T("dotDash"); break;
					case 6:		m_strValue = _T("dotDashHeavy"); break;
					case 7:		m_strValue = _T("dotDotDash"); break;
					case 8:		m_strValue = _T("dotDotDashHeavy"); break;
					case 9:		m_strValue = _T("dotted"); break;
					case 10:	m_strValue = _T("dottedHeavy"); break;
					case 11:	m_strValue = _T("heavy"); break;
					case 12:	m_strValue = _T("none"); break;
					case 13:	m_strValue = _T("sng"); break;
					case 14:	m_strValue = _T("wavy"); break;
					case 15:	m_strValue = _T("wavyDbl"); break;
					case 16:	m_strValue = _T("wavyHeavy"); break;
					case 17:	m_strValue = _T("words"); break;
				default:
					m_strValue = _T("sng");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_