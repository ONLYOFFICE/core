#pragma once
#ifndef PPTX_LIMIT_DGMBUILDTYPE_INCLUDE_H_
#define PPTX_LIMIT_DGMBUILDTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class DgmBuildType : public BaseLimit
		{
		public:
			DgmBuildType()
			{
				m_strValue = _T("whole");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("allAtOnce") == strValue) ||
					(_T("breadthByLvl") == strValue) ||
					(_T("breadthByNode") == strValue) ||
					(_T("ccw") == strValue) ||
					(_T("ccwIn") == strValue) ||
					(_T("ccwOut") == strValue) ||
					(_T("cust") == strValue) ||
					(_T("cw") == strValue) ||
					(_T("cwIn") == strValue) ||
					(_T("cwOut") == strValue) ||
					(_T("depthByBranch") == strValue) ||
					(_T("depthByNode") == strValue) ||
					(_T("down") == strValue) ||
					(_T("inByRing") == strValue) ||
					(_T("outByRing") == strValue) ||
					(_T("up") == strValue) ||
					(_T("whole") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_DGMBUILDTYPE_INCLUDE_H_