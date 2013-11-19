#pragma once
#ifndef PPTX_LIMIT_LIGHTRIGTYPE_INCLUDE_H_
#define PPTX_LIMIT_LIGHTRIGTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class LightRigType : public BaseLimit
		{
		public:
			LightRigType()
			{
				m_strValue = _T("balanced");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("balanced") == strValue) ||
					(_T("brightRoom") == strValue) ||
					(_T("chilly") == strValue) ||
					(_T("contrasting") == strValue) ||
					(_T("flat") == strValue) ||
					(_T("flood") == strValue) ||
					(_T("freezing") == strValue) ||
					(_T("glow") == strValue) ||
					(_T("harsh") == strValue) ||
					(_T("legacyFlat1") == strValue) ||
					(_T("legacyFlat2") == strValue) ||
					(_T("legacyFlat3") == strValue) ||
					(_T("legacyFlat4") == strValue) ||
					(_T("legacyHarsh1") == strValue) ||
					(_T("legacyHarsh2") == strValue) ||
					(_T("legacyHarsh3") == strValue) ||
					(_T("legacyHarsh4") == strValue) ||
					(_T("legacyNormal1") == strValue) ||
					(_T("legacyNormal2") == strValue) ||
					(_T("legacyNormal3") == strValue) ||
					(_T("legacyNormal4") == strValue) ||
					(_T("morning") == strValue) ||
					(_T("soft") == strValue) ||
					(_T("sunrise") == strValue) ||
					(_T("sunset") == strValue) ||
					(_T("threePt") == strValue) ||
					(_T("twoPt") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_LIGHTRIGTYPE_INCLUDE_H_