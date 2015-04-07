#pragma once
#ifndef PPTX_LIMIT_PRSTDASHVAL_INCLUDE_H_
#define PPTX_LIMIT_PRSTDASHVAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class PrstDashVal : public BaseLimit
		{
		public:
			PrstDashVal()
			{
				m_strValue = _T("solid");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("dash") == strValue) ||
					(_T("dashDot") == strValue) ||
					(_T("dot") == strValue) ||
					(_T("lgDash") == strValue) ||
					(_T("lgDashDot") == strValue) ||
					(_T("lgDashDotDot") == strValue) ||
					(_T("solid") == strValue) ||
					(_T("sysDash") == strValue) ||
					(_T("sysDashDot") == strValue) ||
					(_T("sysDashDotDot") == strValue) ||
					(_T("sysDot") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("dash") == m_strValue)
					return 0;
				if (_T("dashDot") == m_strValue)
					return 1;
				if (_T("dot") == m_strValue)
					return 2;
				if (_T("lgDash") == m_strValue)
					return 3;
				if (_T("lgDashDot") == m_strValue)
					return 4;
				if (_T("lgDashDotDot") == m_strValue)
					return 5;
				if (_T("solid") == m_strValue)
					return 6;
				if (_T("sysDash") == m_strValue)
					return 7;
				if (_T("sysDashDot") == m_strValue)
					return 8;
				if (_T("sysDashDotDot") == m_strValue)
					return 9;
				if (_T("sysDot") == m_strValue)
					return 10;
				return 6;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PRSTDASHVAL_INCLUDE_H_