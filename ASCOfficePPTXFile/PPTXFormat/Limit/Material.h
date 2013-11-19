#pragma once
#ifndef PPTX_LIMIT_MATERIAL_INCLUDE_H_
#define PPTX_LIMIT_MATERIAL_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class Material : public BaseLimit
		{
		public:
			Material()
			{
				m_strValue = _T("clear");
			}

			_USE_STRING_OPERATOR
				
			virtual void set(const CString& strValue)
			{
				if ((_T("clear") == strValue) ||
					(_T("dkEdge") == strValue) ||
					(_T("flat") == strValue) ||
					(_T("legacyMatte") == strValue) ||
					(_T("legacyMetal") == strValue) ||
					(_T("legacyPlastic") == strValue) ||
					(_T("legacyWireframe") == strValue) ||
					(_T("matte") == strValue) ||
					(_T("metal") == strValue) ||
					(_T("plastic") == strValue) ||
					(_T("powder") == strValue) ||
					(_T("softEdge") == strValue) ||
					(_T("softmetal") == strValue) ||
					(_T("translucentPowder") == strValue) ||
					(_T("warmMatte") == strValue))
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("clear") == m_strValue)
					return 0;
				if (_T("dkEdge") == m_strValue)
					return 1;
				if (_T("flat") == m_strValue)
					return 2;
				if (_T("legacyMatte") == m_strValue)
					return 3;
				if (_T("legacyMetal") == m_strValue)
					return 4;
				if (_T("legacyPlastic") == m_strValue)
					return 5;
				if (_T("legacyWireframe") == m_strValue)
					return 6;
				if (_T("matte") == m_strValue)
					return 7;
				if (_T("metal") == m_strValue)
					return 8;
				if (_T("plastic") == m_strValue)
					return 9;
				if (_T("powder") == m_strValue)
					return 10;
				if (_T("softEdge") == m_strValue)
					return 11;
				if (_T("softmetal") == m_strValue)
					return 12;
				if (_T("translucentPowder") == m_strValue)
					return 13;
				if (_T("warmMatte") == m_strValue)
					return 14;

				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_MATERIAL_INCLUDE_H_