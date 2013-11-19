#pragma once
#ifndef PPTX_LIMIT_PRESETSHADOWVAL_INCLUDE_H_
#define PPTX_LIMIT_PRESETSHADOWVAL_INCLUDE_H_

#include "BaseLimit.h"



namespace PPTX
{
	namespace Limit
	{
		class PresetShadowVal : public BaseLimit
		{
		public:
			PresetShadowVal()
			{
				m_strValue = _T("shdw1");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("shdw1") == strValue) ||
					(_T("shdw2") == strValue) ||
					(_T("shdw3") == strValue) ||
					(_T("shdw4") == strValue) ||
					(_T("shdw5") == strValue) ||
					(_T("shdw6") == strValue) ||
					(_T("shdw7") == strValue) ||
					(_T("shdw8") == strValue) ||
					(_T("shdw9") == strValue) ||
					(_T("shdw10") == strValue) ||
					(_T("shdw11") == strValue) ||
					(_T("shdw12") == strValue) ||
					(_T("shdw13") == strValue) ||
					(_T("shdw14") == strValue) ||
					(_T("shdw15") == strValue) ||
					(_T("shdw16") == strValue) ||
					(_T("shdw17") == strValue) ||
					(_T("shdw18") == strValue) ||
					(_T("shdw19") == strValue) ||
					(_T("shdw20") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("shdw1") == m_strValue)
					return 0;
				if (_T("shdw2") == m_strValue)
					return 1;
				if (_T("shdw3") == m_strValue)
					return 2;
				if (_T("shdw4") == m_strValue)
					return 3;
				if (_T("shdw5") == m_strValue)
					return 4;
				if (_T("shdw6") == m_strValue)
					return 5;
				if (_T("shdw7") == m_strValue)
					return 6;
				if (_T("shdw8") == m_strValue)
					return 7;
				if (_T("shdw9") == m_strValue)
					return 8;
				if (_T("shdw10") == m_strValue)
					return 9;
				if (_T("shdw11") == m_strValue)
					return 10;
				if (_T("shdw12") == m_strValue)
					return 11;
				if (_T("shdw13") == m_strValue)
					return 12;
				if (_T("shdw14") == m_strValue)
					return 13;
				if (_T("shdw15") == m_strValue)
					return 14;
				if (_T("shdw16") == m_strValue)
					return 15;
				if (_T("shdw17") == m_strValue)
					return 16;
				if (_T("shdw18") == m_strValue)
					return 17;
				if (_T("shdw19") == m_strValue)
					return 18;
				if (_T("shdw20") == m_strValue)
					return 19;
				
				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PRESETSHADOWVAL_INCLUDE_H_