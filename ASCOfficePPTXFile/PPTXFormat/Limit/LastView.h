#pragma once
#ifndef PPTX_LIMIT_LASTVIEW_INCLUDE_H_
#define PPTX_LIMIT_LASTVIEW_INCLUDE_H_

#include "BaseLimit.h"



namespace PPTX
{
	namespace Limit
	{
		class LastView : public BaseLimit
		{
		public:
			LastView()
			{
				m_strValue = _T("sldThumbnailView");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("handoutView") == strValue) ||
					(_T("notesMasterView") == strValue) ||
					(_T("notesView") == strValue) ||
					(_T("outlineView") == strValue) ||
					(_T("sldMasterView") == strValue) ||
					(_T("sldSorterView") == strValue) ||
					(_T("sldThumbnailView") == strValue) ||
					(_T("sldView") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("handoutView") == m_strValue)
					return 0;
				if (_T("notesMasterView") == m_strValue)
					return 1;
				if (_T("notesView") == m_strValue)
					return 2;
				if (_T("outlineView") == m_strValue)
					return 3;
				if (_T("sldMasterView") == m_strValue)
					return 4;
				if (_T("sldSorterView") == m_strValue)
					return 5;
				if (_T("sldThumbnailView") == m_strValue)
					return 6;
				if (_T("sldView") == m_strValue)
					return 7;
				return 6;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_LASTVIEW_INCLUDE_H_