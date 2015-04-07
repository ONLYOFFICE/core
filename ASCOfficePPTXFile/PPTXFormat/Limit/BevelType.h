#pragma once
#ifndef PPTX_LIMIT_BEVELTYPE_INCLUDE_H_
#define PPTX_LIMIT_BEVELTYPE_INCLUDE_H_

#include "BaseLimit.h"

namespace PPTX
{
	namespace Limit
	{
		class BevelType : public BaseLimit
		{
		public:
			BevelType()
			{
				m_strValue = _T("circle");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("angle")		== strValue) ||
					(_T("artDeco")		== strValue) ||
					(_T("circle")		== strValue) ||
					(_T("convex")		== strValue) ||
					(_T("coolSlant")	== strValue) ||
					(_T("cross")		== strValue) ||
					(_T("divot")		== strValue) ||
					(_T("hardEdge")		== strValue) ||
					(_T("relaxedInset") == strValue) ||
					(_T("riblet")		== strValue) ||
					(_T("slope")		== strValue) ||
					(_T("softRound")	== strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("angle") == m_strValue)
					return 0;
				if (_T("artDeco") == m_strValue)
					return 1;
				if (_T("circle") == m_strValue)
					return 2;
				if (_T("convex") == m_strValue)
					return 3;
				if (_T("coolSlant") == m_strValue)
					return 4;
				if (_T("cross") == m_strValue)
					return 5;
				if (_T("divot") == m_strValue)
					return 6;
				if (_T("hardEdge") == m_strValue)
					return 7;
				if (_T("relaxedInset") == m_strValue)
					return 8;
				if (_T("riblet") == m_strValue)
					return 9;
				if (_T("slope") == m_strValue)
					return 10;
				if (_T("softRound") == m_strValue)
					return 11;
				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_BEVELTYPE_INCLUDE_H_