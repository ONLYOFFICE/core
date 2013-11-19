#pragma once
#ifndef PPTX_LIMIT_TLTRIGGEREVENT_INCLUDE_H_
#define PPTX_LIMIT_TLTRIGGEREVENT_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TLTriggerEvent : public BaseLimit
		{
		public:
			TLTriggerEvent()
			{
				m_strValue = _T("begin");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("begin") == strValue) ||
					(_T("end") == strValue) ||
					(_T("onBegin") == strValue) ||
					(_T("onClick") == strValue) ||
					(_T("onDblClick") == strValue) ||
					(_T("onEnd") == strValue) ||
					(_T("onMouseOut") == strValue) ||
					(_T("onMouseOver") == strValue) ||
					(_T("onNext") == strValue) ||
					(_T("onPrev") == strValue) ||
					(_T("onStopAudio") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLTRIGGEREVENT_INCLUDE_H_