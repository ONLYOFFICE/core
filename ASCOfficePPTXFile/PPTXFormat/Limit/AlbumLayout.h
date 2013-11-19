#pragma once
#ifndef PPTX_LIMIT_ALBUMLAYOUT_INCLUDE_H_
#define PPTX_LIMIT_ALBUMLAYOUT_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class AlbumLayout : public BaseLimit
		{
		public:
			AlbumLayout()
			{
				m_strValue = _T("fitToSlide");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("1pic") == strValue) || 
					(_T("1picTitle") == strValue) || 
					(_T("2pic") == strValue) ||
					(_T("2picTitle") == strValue) || 
					(_T("4pic") == strValue) || 
					(_T("4picTitle") == strValue) || 
					(_T("fitToSlide") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_ALBUMLAYOUT_INCLUDE_H_