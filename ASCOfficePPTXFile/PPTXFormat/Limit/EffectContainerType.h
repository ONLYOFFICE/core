#pragma once
#ifndef PPTX_LIMIT_EFFECTCONTAINERTYPE_INCLUDE_H_
#define PPTX_LIMIT_EFFECTCONTAINERTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class EffectContainerType : public BaseLimit
		{
		public:
			EffectContainerType()
			{
				m_strValue = _T("sib");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("sib") == strValue) ||
					(_T("tree") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("sib") == m_strValue)
					return 0;
				if (_T("tree") == m_strValue)
					return 1;
				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_EFFECTCONTAINERTYPE_INCLUDE_H_