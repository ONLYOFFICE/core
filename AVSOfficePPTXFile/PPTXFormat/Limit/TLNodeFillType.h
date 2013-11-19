#pragma once
#ifndef PPTX_LIMIT_TLNODEFILLTYPE_INCLUDE_H_
#define PPTX_LIMIT_TLNODEFILLTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLNodeFillType : public BaseLimit
		{
		public:
			TLNodeFillType()
			{
				m_strValue = _T("hold");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("freeze") == strValue) ||
					(_T("hold") == strValue) ||
					(_T("remove") == strValue) ||
					(_T("transition") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLNODEFILLTYPE_INCLUDE_H_