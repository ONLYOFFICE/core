#pragma once
#ifndef PPTX_LIMIT_CONFORMANCE_INCLUDE_H_
#define PPTX_LIMIT_CONFORMANCE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class Conformance : public BaseLimit
		{
		public:
			Conformance()
			{
				m_strValue = _T("transitional");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("strict") == strValue) ||
					(_T("transitional") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("strict") == m_strValue)
					return 0;
				if (_T("transitional") == m_strValue)
					return 1;
				return 1;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_CONFORMANCE_INCLUDE_H_