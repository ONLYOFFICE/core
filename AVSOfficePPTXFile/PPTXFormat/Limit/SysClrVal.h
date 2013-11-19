#pragma once
#ifndef PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_
#define PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class SysClrVal : public BaseLimit
		{
		public:
			SysClrVal()
			{
				m_strValue = _T("window");
	 		}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("3dDkShadow") == strValue) ||
					(_T("3dLight") == strValue) ||
					(_T("activeBorder") == strValue) ||
					(_T("activeCaption") == strValue) ||
					(_T("appWorkspace") == strValue) ||
					(_T("background") == strValue) ||
					(_T("btnFace") == strValue) ||
					(_T("btnHighlight") == strValue) ||
					(_T("btnShadow") == strValue) ||
					(_T("btnText") == strValue) ||
					(_T("captionText") == strValue) ||
					(_T("gradientActiveCaption") == strValue) ||
					(_T("gradientInactiveCaption") == strValue) ||
					(_T("grayText") == strValue) ||
					(_T("highlight") == strValue) ||
					(_T("highlightText") == strValue) ||
					(_T("hotLight") == strValue) ||
					(_T("inactiveBorder") == strValue) ||
					(_T("inactiveCaption") == strValue) ||
					(_T("inactiveCaptionText") == strValue) ||
					(_T("infoBk") == strValue) ||
					(_T("infoText") == strValue) ||
					(_T("menu") == strValue) ||
					(_T("menuBar") == strValue) ||
					(_T("menuHighlight") == strValue) ||
					(_T("menuText") == strValue) ||
					(_T("scrollBar") == strValue) ||
					(_T("window") == strValue) ||
					(_T("windowFrame") == strValue) ||
					(_T("windowText") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_