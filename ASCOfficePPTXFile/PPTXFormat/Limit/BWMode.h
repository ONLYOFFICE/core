#pragma once
#ifndef PPTX_LIMIT_BWMODE_INCLUDE_H_
#define PPTX_LIMIT_BWMODE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class BWMode : public BaseLimit
		{
		public:
			BWMode()
			{
				m_strValue = _T("auto");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("auto")			== strValue) ||
					(_T("black")		== strValue) ||
					(_T("blackGray")	== strValue) ||
					(_T("blackWhite")	== strValue) ||
					(_T("clr")			== strValue) ||
					(_T("gray")			== strValue) ||
					(_T("grayWhite")	== strValue) ||
					(_T("hidden")		== strValue) ||
					(_T("invGray")		== strValue) ||
					(_T("ltGray")		== strValue) ||
					(_T("white")		== strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("auto") == m_strValue)
					return 0;
				if (_T("black") == m_strValue)
					return 1;
				if (_T("blackGray") == m_strValue)
					return 2;
				if (_T("blackWhite") == m_strValue)
					return 3;
				if (_T("clr") == m_strValue)
					return 4;
				if (_T("gray") == m_strValue)
					return 5;
				if (_T("grayWhite") == m_strValue)
					return 6;
				if (_T("hidden") == m_strValue)
					return 7;
				if (_T("invGray") == m_strValue)
					return 8;
				if (_T("ltGray") == m_strValue)
					return 9;
				if (_T("white") == m_strValue)
					return 10;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0: m_strValue = _T("auto"); break;
				case 1: m_strValue = _T("black"); break;
				case 2: m_strValue = _T("blackGray"); break;
				case 3: m_strValue = _T("blackWhite"); break;
				case 4: m_strValue = _T("clr"); break;
				case 5: m_strValue = _T("gray"); break;
				case 6: m_strValue = _T("grayWhite"); break;
				case 7: m_strValue = _T("hidden"); break;
				case 8: m_strValue = _T("invGray"); break;
				case 9: m_strValue = _T("ltGray"); break;
				case 10: m_strValue = _T("white"); break;
				default:
					m_strValue = _T("auto");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_BWMODE_INCLUDE_H_