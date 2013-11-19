#pragma once
#ifndef PPTX_LIMIT_PENALIGN_INCLUDE_H_
#define PPTX_LIMIT_PENALIGN_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class PenAlign : public BaseLimit
		{
		public:
			PenAlign()
			{
				m_strValue = _T("ctr");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("ctr") == strValue) ||
					(_T("in") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("ctr") == m_strValue)
					return 0;
				if (_T("in") == m_strValue)
					return 1;
				return 0;
			}

			virtual void SetBYTECode(const BYTE& src)
			{	
				if (1 == src)
					m_strValue = _T("in");
				else
					m_strValue = _T("ctr");
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PENALIGN_INCLUDE_H_