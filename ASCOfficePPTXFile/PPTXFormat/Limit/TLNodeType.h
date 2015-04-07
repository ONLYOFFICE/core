#pragma once
#ifndef PPTX_LIMIT_TLNODETYPE_INCLUDE_H_
#define PPTX_LIMIT_TLNODETYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLNodeType : public BaseLimit
		{
		public:
			TLNodeType()
			{
				m_strValue = _T("tmRoot");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("afterEffect") == strValue) ||
					(_T("afterGroup") == strValue) ||
					(_T("clickEffect") == strValue) ||
					(_T("clickPar") == strValue) ||
					(_T("interactiveSeq") == strValue) ||
					(_T("mainSeq") == strValue) ||
					(_T("tmRoot") == strValue) ||
					(_T("withEffect") == strValue) ||
					(_T("withGroup") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLNODETYPE_INCLUDE_H_