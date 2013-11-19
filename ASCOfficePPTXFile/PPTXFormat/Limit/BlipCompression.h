#pragma once
#ifndef PPTX_LIMIT_BLIPCOMPRESSION_INCLUDE_H_
#define PPTX_LIMIT_BLIPCOMPRESSION_INCLUDE_H_

#include "BaseLimit.h"

namespace PPTX
{
	namespace Limit
	{
		class BlipCompression : public BaseLimit
		{
		public:
			BlipCompression()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("none")		== strValue) ||
					(_T("email")	== strValue) ||
					(_T("hqprint")	== strValue) ||
					(_T("print")	== strValue) ||
					(_T("screen")	== strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("none") == m_strValue)
					return 0;
				if (_T("email") == m_strValue)
					return 1;
				if (_T("hqprint") == m_strValue)
					return 2;
				if (_T("print") == m_strValue)
					return 3;
				if (_T("screen") == m_strValue)
					return 4;
				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_BLIPCOMPRESSION_INCLUDE_H_