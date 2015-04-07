#pragma once
#ifndef PPTX_LIMIT_PATHFILLMODE_INCLUDE_H_
#define PPTX_LIMIT_PATHFILLMODE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class PathFillMode : public BaseLimit
		{
		public:
			PathFillMode()
			{
				m_strValue = _T("none");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("darken") == strValue) ||
					(_T("darkenLess") == strValue) ||
					(_T("lighten") == strValue) ||
					(_T("lightenLess") == strValue) ||
					(_T("none") == strValue) ||
					(_T("norm") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("darken") == m_strValue)
					return 0;
				if (_T("darkenLess") == m_strValue)
					return 1;
				if (_T("lighten") == m_strValue)
					return 2;
				if (_T("lightenLess") == m_strValue)
					return 3;
				if (_T("none") == m_strValue)
					return 4;
				if (_T("norm") == m_strValue)
					return 5;
				return 4;
			}
			virtual void SetBYTECode(const BYTE& src)
			{
				switch (src)
				{
				case 0:
					m_strValue = _T("darken");
					break;
				case 1:
					m_strValue = _T("darkenLess");
					break;
				case 2:
					m_strValue = _T("lighten");
					break;
				case 3:
					m_strValue = _T("lightenLess");
					break;
				case 4:
					m_strValue = _T("none");
					break;
				case 5:
					m_strValue = _T("norm");
					break;
				default:
					m_strValue = _T("none");
					break;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PATHFILLMODE_INCLUDE_H_