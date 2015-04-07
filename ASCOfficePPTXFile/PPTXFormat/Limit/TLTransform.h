#pragma once
#ifndef PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_
#define PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLTransform : public BaseLimit
		{
		public:
			TLTransform()
			{
				m_strValue = _T("pt");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("img") == strValue) ||
					(_T("pt") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_