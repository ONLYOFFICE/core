#pragma once
#ifndef PPTX_LIMIT_COLORSCHEMEINDEX_INCLUDE_H_
#define PPTX_LIMIT_COLORSCHEMEINDEX_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class ColorSchemeIndex : public BaseLimit
		{
		public:
			ColorSchemeIndex()
			{
				m_strValue =  _T("accent1");
			}
			
			_USE_STRING_OPERATOR

			virtual void set(const CString& strValue)
			{
				if ((_T("accent1") == strValue) ||
					(_T("accent2") == strValue) ||
					(_T("accent3") == strValue) ||
					(_T("accent4") == strValue) ||
					(_T("accent5") == strValue) ||
					(_T("accent6") == strValue) ||
					(_T("dk1") == strValue) ||
					(_T("dk2") == strValue) ||
					(_T("folHlink") == strValue) ||
					(_T("hlink") == strValue) ||
					(_T("lt1") == strValue) ||
					(_T("lt2") == strValue))
				{
					m_strValue = strValue;
				}
				else
				{
					// word clrmap
					if (_T("light1") == strValue)
						m_strValue = _T("lt1");
					else if (_T("light2") == strValue)
						m_strValue = _T("lt2");
					else if (_T("dark1") == strValue)
						m_strValue = _T("dk1");
					else if (_T("dark2") == strValue)
						m_strValue = _T("dk2");
					else if (_T("hyperlink") == strValue)
						m_strValue = _T("hlink");
					else if (_T("followedHyperlink") == strValue)
						m_strValue = _T("folHlink");
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("accent1") == m_strValue)
					return 0;
				if (_T("accent2") == m_strValue)
					return 1;
				if (_T("accent3") == m_strValue)
					return 2;
				if (_T("accent4") == m_strValue)
					return 3;
				if (_T("accent5") == m_strValue)
					return 4;
				if (_T("accent6") == m_strValue)
					return 5;
				if (_T("bg1") == m_strValue)
					return 6;
				if (_T("bg2") == m_strValue)
					return 7;
				if (_T("dk1") == m_strValue)
					return 8;
				if (_T("dk2") == m_strValue)
					return 9;
				if (_T("folHlink") == m_strValue)
					return 10;
				if (_T("hlink") == m_strValue)
					return 11;
				if (_T("lt1") == m_strValue)
					return 12;
				if (_T("lt2") == m_strValue)
					return 13;
				if (_T("phClr") == m_strValue)
					return 14;
				if (_T("tx1") == m_strValue)
					return 15;
				if (_T("tx2") == m_strValue)
					return 16;
				return 0;
			}

			void SetStringCode(const BYTE& val)
			{
				switch (val)
				{
				case 0:
					m_strValue = _T("accent1");
					break;
				case 1:
					m_strValue = _T("accent2");
					break;
				case 2:
					m_strValue = _T("accent3");
					break;
				case 3:
					m_strValue = _T("accent4");
					break;
				case 4:
					m_strValue = _T("accent5");
					break;
				case 5:
					m_strValue = _T("accent6");
					break;
				case 8:
					m_strValue = _T("dk1");
					break;
				case 9:
					m_strValue = _T("dk2");
					break;
				case 10:
					m_strValue = _T("folHlink");
					break;
				case 11:
					m_strValue = _T("hlink");
					break;
				case 12:
					m_strValue = _T("lt1");
					break;
				case 13:
					m_strValue = _T("lt2");
					break;
				default:
					m_strValue = _T("accent1");
					break;
				}				
			}

		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_COLORSCHEMEINDEX_INCLUDE_H_