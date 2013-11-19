#pragma once
#ifndef PPTX_LIMIT_TEXTVERTICALTYPE_INCLUDE_H_
#define PPTX_LIMIT_TEXTVERTICALTYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class TextVerticalType : public BaseLimit
		{
		public:
			TextVerticalType()
			{
				m_strValue = _T("horz");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("eaVert") == strValue) ||
					(_T("horz") == strValue) ||
					(_T("mongolianVert") == strValue) ||
					(_T("vert") == strValue) ||
					(_T("vert270") == strValue) ||
					(_T("wordArtVert") == strValue) ||
					(_T("wordArtVertRtl") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("eaVert") == m_strValue)
					return 0;
				if (_T("horz") == m_strValue)
					return 1;
				if (_T("mongolianVert") == m_strValue)
					return 2;
				if (_T("vert") == m_strValue)
					return 3;
				if (_T("vert270") == m_strValue)
					return 4;
				if (_T("wordArtVert") == m_strValue)
					return 5;
				if (_T("wordArtVertRtl") == m_strValue)
					return 6;
				return 1;
			}

			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0: m_strValue = _T("eaVert"); break;
				case 1: m_strValue = _T("horz"); break;
				case 2: m_strValue = _T("mongolianVert"); break;
				case 3: m_strValue = _T("vert"); break;
				case 4: m_strValue = _T("vert270"); break;
				case 5: m_strValue = _T("wordArtVert"); break;
				case 6: m_strValue = _T("wordArtVertRtl"); break;
				default:
					m_strValue = _T("horz");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTVERTICALTYPE_INCLUDE_H_