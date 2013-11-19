#pragma once
#ifndef PPTX_LIMIT_ANIMATIONDGMBUILD_INCLUDE_H_
#define PPTX_LIMIT_ANIMATIONDGMBUILD_INCLUDE_H_

#include "BaseLimit.h"

namespace PPTX
{
	namespace Limit
	{
		class AnimationDgmBuild : public BaseLimit
		{
		public:
			AnimationDgmBuild()
			{
				m_strValue = _T("allAtOnce");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((strValue == _T("allAtOnce")) || 
					(strValue == _T("lvlAtOnce")) || 
					(strValue == _T("lvlOne")) || 
					(strValue == _T("one")))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_ANIMATIONDGMBUILD_INCLUDE_H_