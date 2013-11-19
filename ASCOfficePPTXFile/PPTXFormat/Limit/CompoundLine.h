#pragma once
#ifndef PPTX_LIMIT_COMPOUNDLINE_INCLUDE_H_
#define PPTX_LIMIT_COMPOUNDLINE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class CompoundLine : public BaseLimit
		{
		public:
			CompoundLine()
			{
				m_strValue = _T("sng");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("dbl") == strValue) ||
					(_T("sng") == strValue) ||
					(_T("thickThin") == strValue) ||
					(_T("thinThick") == strValue) ||
					(_T("tri") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("dbl") == m_strValue)
					return 0;
				if (_T("sng") == m_strValue)
					return 1;
				if (_T("thickThin") == m_strValue)
					return 2;
				if (_T("thinThick") == m_strValue)
					return 3;
				if (_T("tri") == m_strValue)
					return 4;
				return 1;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("dbl");
					break;
				case 2:
					m_strValue = _T("thickThin");
					break;
				case 3:
					m_strValue = _T("thinThick");
					break;
				case 4:
					m_strValue = _T("tri");
					break;
				default:
					m_strValue = _T("sng");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_COMPOUNDLINE_INCLUDE_H_