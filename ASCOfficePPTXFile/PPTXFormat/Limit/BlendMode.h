#pragma once
#ifndef PPTX_LIMIT_BLENDMODE_INCLUDE_H_
#define PPTX_LIMIT_BLENDMODE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class BlendMode : public BaseLimit
		{
		public:
			BlendMode()
			{
				m_strValue = _T("screen");
			}

			_USE_STRING_OPERATOR
			
			virtual void set(const CString& strValue)
			{
				if ((_T("darken")	== strValue) ||
					(_T("lighten")	== strValue) ||
					(_T("mult")		== strValue) ||
					(_T("over")		== strValue) ||
					(_T("screen")	== strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("darken") == m_strValue)
					return 0;
				if (_T("lighten") == m_strValue)
					return 1;
				if (_T("mult") == m_strValue)
					return 2;
				if (_T("over") == m_strValue)
					return 3;
				if (_T("screen") == m_strValue)
					return 4;
				return 4;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_BLENDMODE_INCLUDE_H_