#pragma once
#ifndef PPTX_LIMIT_DGMBUILD_INCLUDE_H_
#define PPTX_LIMIT_DGMBUILD_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class DgmBuild : public BaseLimit
		{
		public:
			DgmBuild()
			{
				m_strValue = _T("sp");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("bg") == strValue) ||
					(_T("sp") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_DGMBUILD_INCLUDE_H_