#pragma once
#ifndef PPTX_LIMIT_BASE_INCLUDE_H_
#define PPTX_LIMIT_BASE_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/Base/Base.h"

#if !defined(_WIN32) && !defined (_WIN64)
#include "../../../DesktopEditor/common/Types.h"
#define _USE_STRING_OPERATOR						\
    virtual void operator=(const CString& value)	\
    {												\
        set(value);									\
    }

#else
#define _USE_STRING_OPERATOR						\
    virtual void operator=(const CString& value)	\
    {												\
        set(value);									\
    }												\
    virtual void operator=(const BSTR& value)		\
    {												\
        set((CString)value);						\
    }
#endif // #if !defined(_WIN32) && !defined (_WIN64)



namespace PPTX
{
	namespace Limit
	{
		class BaseLimit
		{
		public:
			BaseLimit(const CString& str = _T("")) : m_strValue(str)
			{
			}

			BaseLimit(const BaseLimit& oSrc)
			{
				*this = oSrc;
			}

			BaseLimit& operator=(const BaseLimit& oSrc)
			{
				m_strValue = oSrc.m_strValue;
				return *this;
			}

			void operator=(const CString& str)
			{
				set(str);
			}
			AVSINLINE void _set(const CString& strValue)
			{
				set(strValue);
			}

		public:
			virtual void set(const CString& strValue)		= 0;

			AVSINLINE CString& get() 
			{
				return m_strValue;
			}
			AVSINLINE const CString& get() const 
			{
				return m_strValue;
			}

			virtual BYTE GetBYTECode() const
			{
				return 0;
			}
			virtual void SetBYTECode(const BYTE& src)
			{				
			}

		protected:
			CString m_strValue;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_BASE_INCLUDE_H_
