#pragma once
#ifndef PPTX_LIMIT_FRAMESHAPE_INCLUDE_H_
#define PPTX_LIMIT_FRAMESHAPE_INCLUDE_H_

#include "BaseLimit.h"



namespace PPTX
{
	namespace Limit
	{
		class FrameShape : public BaseLimit
		{
		public:
			FrameShape()
			{
				m_strValue = _T("frameStyle1");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("frameStyle1") == strValue) ||
					(_T("frameStyle2") == strValue) ||
					(_T("frameStyle3") == strValue) ||
					(_T("frameStyle4") == strValue) ||
					(_T("frameStyle5") == strValue) ||
					(_T("frameStyle6") == strValue) ||
					(_T("frameStyle7") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FRAMESHAPE_INCLUDE_H_