/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "SmartPtr.h"
#include "../XML/xmlutils.h"

// чтобы не писать мега классы с мега приведением типов - 
// - напишем все по-простому. «ато будет все максимально быстро и 
// компилиться 3 часа не будет

namespace NSCommon
{
    template<class Type>
	class nullable_base
	{
	protected:
		Type* m_pPointer;

	public:
		nullable_base()
		{
			m_pPointer = NULL;
		}
		nullable_base(const nullable_base<Type>& oOther)
		{
			m_pPointer = NULL;
			if ( NULL != oOther.m_pPointer )
				m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );
		}
		virtual ~nullable_base()
		{
			RELEASEOBJECT(m_pPointer);
		}

	public:
		AVSINLINE Type& operator*()  { return *m_pPointer; }
		AVSINLINE Type* operator->() { return  m_pPointer; }

		AVSINLINE Type& operator*() const  { return *m_pPointer; }
		AVSINLINE Type* operator->() const { return  m_pPointer; }

		AVSINLINE const Type& get()const { return  *m_pPointer; } 
		AVSINLINE Type& get() { return  *m_pPointer; }

	public:
		nullable_base<Type>& operator=(const nullable_base<Type> &oOther)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oOther.m_pPointer )
				m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );

			return *this;
		}
		nullable_base<Type>& operator=(Type* pType)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer	= pType;
			return *this;
		}
		nullable_base<Type>& operator=(const Type& oSrc)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer	= new Type(oSrc);
			return *this;
		}

	public:
		AVSINLINE bool IsInit() const
		{ 
			return (NULL != m_pPointer); 
		}
		AVSINLINE bool is_init() const
		{
			return IsInit();
		}

		AVSINLINE void reset(Type* pType = NULL)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = pType;
		}
	};

    template<class Type>
	class nullable : public nullable_base<Type>
	{
	public:
		nullable() : nullable_base<Type>()
		{
		}
		nullable(const nullable<Type>& oOther)
		{
			if ( NULL == oOther.m_pPointer )
                this->m_pPointer = NULL;
			else
                this->m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );
		}
        nullable(const XmlUtils::CXmlNode& oNode)   // const modifier is important for gcc compiler in our case
        {
            if (oNode.IsValid())
                this->m_pPointer = new Type(const_cast<XmlUtils::CXmlNode&> (oNode));
            else
                this->m_pPointer = NULL;
        }
        
        nullable(XmlUtils::CXmlNode& oNode)
        {
            if (oNode.IsValid())
                this->m_pPointer = new Type(oNode);
            else
                this->m_pPointer = NULL;
        }
        
		nullable<Type>& operator=(XmlUtils::CXmlNode& oNode)
		{
            RELEASEOBJECT(this->m_pPointer);
			if (oNode.IsValid())
                this->m_pPointer = new Type(oNode);
			return *this;
		}
	#ifdef _USE_XMLLITE_READER_
		nullable<Type>& operator=(XmlUtils::CXmlLiteReader& oReader)
		{
            RELEASEOBJECT(this->m_pPointer);
			if (oReader.IsValid())
                this->m_pPointer = new Type(oReader);
			return *this;
		}
	#endif
		nullable<Type>& operator=(const wchar_t* cwsValue)
		{
            RELEASEOBJECT(this->m_pPointer);
			if (NULL != cwsValue)
                this->m_pPointer = new Type( cwsValue );
			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		nullable<Type>& operator=(const BSTR &value)
		{
            RELEASEOBJECT(this->m_pPointer);
			if (NULL != value)
                this->m_pPointer = new Type( value );
			return *this;
		}
#endif

        //nullable<Type>& operator=(std::wstring& cwsValue)
        //{
        //    RELEASEOBJECT(this->m_pPointer);
        //    this->m_pPointer = new Type( cwsValue.c_str() );
        //    return *this;
        //}

        //nullable<Type>& operator=(const std::wstring& cwsValue)
        //{
        //    RELEASEOBJECT(this->m_pPointer);
        //    this->m_pPointer = new Type( cwsValue.c_str() );
        //    return *this;
        //}

		nullable<Type>& operator=(const nullable<Type> &oOther)
		{
            RELEASEOBJECT(this->m_pPointer);

			if ( NULL != oOther.m_pPointer )
                this->m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );

			return *this;
		}
		nullable<Type>& operator=(Type* pType)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer	= pType;
			return *this;
		}
		nullable<Type>& operator=(Type& oSrc)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer	= new Type(oSrc);
			return *this;
		}
		nullable<Type>& operator=(const Type& oSrc)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer	= new Type(oSrc);
			return *this;
		}
		const bool operator==(const nullable<Type>& oOther) const
		{
            if ( !this->m_pPointer && !oOther.m_pPointer )
				return true;
            else if ( !this->m_pPointer || !oOther.m_pPointer )
				return false;

            return (*this->m_pPointer) == (*(oOther.m_pPointer));
		}

		const bool operator==(const Type& oOther) const
		{
            if ( !this->m_pPointer )
				return false;

            return (*this->m_pPointer) == oOther;
		}

        AVSINLINE Type& operator*()  { return *this->m_pPointer; }
        AVSINLINE Type* operator->() { return  this->m_pPointer; }

        AVSINLINE Type& operator*() const  { return *this->m_pPointer; }
        AVSINLINE Type* operator->() const { return  this->m_pPointer; }

        AVSINLINE const Type& get()const { return  *this->m_pPointer; }
        AVSINLINE Type& get2()const { return  *this->m_pPointer; }

		template<class T> const bool is()const
		{
            if (NULL == this->m_pPointer)
				return false;
            T* pResult = dynamic_cast<T*>(const_cast<Type*>(this->m_pPointer));
			return (NULL != pResult);
		}
		template<class T> const T& as()const
		{
            T* pResult = dynamic_cast<T*>(const_cast<Type*>(this->m_pPointer));
			return *pResult;
		}
		template<class T> T& as()
		{
            T* pResult = dynamic_cast<T*>(const_cast<Type*>(this->m_pPointer));
			return *pResult;
		}

		AVSINLINE bool Init()
		{
            RELEASEOBJECT(this->m_pPointer);

            this->m_pPointer = new Type;

            return this->IsInit();
		}
		Type* GetPointer() const
		{
            return this->m_pPointer;
		}
		//GetPointerEmptyNullable - небезопасная операция, использовать при крайней необходимости
		//Передает указатель и очищает nullable, в дальнейшем память надо удалять самостоятельно
		Type* GetPointerEmptyNullable()
		{
            Type* pOldPointer = this->m_pPointer;
            this->m_pPointer = NULL;
			return pOldPointer; 
		}
	};

	template<typename Type> 
	class nullable_limit : public nullable_base<Type>
	{
	public:
		nullable_limit() : nullable_base<Type>()
		{
		}

	public:

		AVSINLINE void operator=(const CString& value)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer = new Type();
            this->m_pPointer->_set(value);
		}
		AVSINLINE void operator=(Type* pType)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer	= pType;
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
            RELEASEOBJECT(this->m_pPointer);
			if (NULL != value)
			{
                this->m_pPointer = new Type();
                this->m_pPointer->_set((CString)value);
			}
		}
#endif
		AVSINLINE void operator=(const BYTE& value)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer = new Type();
            this->m_pPointer->SetBYTECode(value);
		}

		AVSINLINE void operator=(const Type& value)
		{
			*this = value.get();
		}

		nullable_limit<Type>& operator=(const nullable_limit<Type>& oSrc)
		{
            RELEASEOBJECT(this->m_pPointer);

			if ( NULL != oSrc.m_pPointer )
			{
                this->m_pPointer = new Type();
                this->m_pPointer->set(oSrc->get());
			}

			return *this;
		}

		AVSINLINE const CString& get_value_or(const CString& value) const
		{
            if (NULL == this->m_pPointer)
				return value;
            return this->m_pPointer->get();
		}
		AVSINLINE const CString& get_value() const
		{
            return this->m_pPointer->get();
		}

	public:
        AVSINLINE Type& operator*()  { return *this->m_pPointer; }
        AVSINLINE Type* operator->() { return  this->m_pPointer; }

        AVSINLINE Type& operator*() const  { return *this->m_pPointer; }
        AVSINLINE Type* operator->() const { return  this->m_pPointer; }

        AVSINLINE const Type& get()const { return  *this->m_pPointer; }
	};

	class nullable_int : public nullable_base<int>
	{
	public:
		nullable_int() : nullable_base<int>()
		{
		}

		AVSINLINE void normalize(const int& min, const int& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		AVSINLINE void normalize_positive()
		{
			if (IsInit())
			{
				if (*m_pPointer < 0)
					*m_pPointer = 0;
			}
		}
		nullable_int& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new int(XmlUtils::GetInteger(cwsValue));

			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
            RELEASEOBJECT(this->m_pPointer);
			
			if (NULL != value)
                this->m_pPointer = new int(XmlUtils::GetInteger(value));
		}
#endif
		AVSINLINE void operator=(const CString& value)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer = new int(XmlUtils::GetInteger(value));
		}
		AVSINLINE void operator=(const int& value)
		{
            RELEASEOBJECT(this->m_pPointer);
            this->m_pPointer = new int(value);
		}

		nullable_int& operator=(const nullable_int& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if (NULL != oSrc.m_pPointer )
				m_pPointer = new int(*oSrc);
			return *this;
		}

		AVSINLINE int get_value_or(const int& value) const
		{
			if (NULL == m_pPointer)
			{
				int ret = value;
				return ret;
			}
			return *m_pPointer;
		}

	public:
		AVSINLINE int& operator*()  { return *m_pPointer; }
		AVSINLINE int* operator->() { return  m_pPointer; }

		AVSINLINE int& operator*() const  { return *m_pPointer; }
		AVSINLINE int* operator->() const { return  m_pPointer; }

		AVSINLINE const int& get()const { return  *m_pPointer; }
	public:
		AVSINLINE CString toString() const
		{
			CString result;
			//if (IsInit())
			//	result.Format(_T("%d"), get());

			return result;
		}
	};
	class nullable_sizet : public nullable_base<size_t>
	{
	public:
		nullable_sizet() : nullable_base<size_t>()
		{
		}

		AVSINLINE void normalize(const size_t& max)
		{
			if (IsInit())
			{
				if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		nullable_sizet& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new size_t(XmlUtils::GetUInteger(cwsValue));

			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
			RELEASEOBJECT(m_pPointer);
			if (NULL != value)
				m_pPointer = new size_t(XmlUtils::GetUInteger(value));
		}
#endif
		AVSINLINE void operator=(const size_t& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new size_t(value);
		}

		nullable_sizet& operator=(const nullable_sizet& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oSrc.m_pPointer )
				m_pPointer = new size_t(*oSrc);

			return *this;
		}

		AVSINLINE size_t get_value_or(const size_t& value) const
		{
			if (NULL == m_pPointer)
			{
				size_t ret = value;
				return ret;
			}
			return *m_pPointer;
		}
	public:
		AVSINLINE size_t& operator*()  { return *m_pPointer; }
		AVSINLINE size_t* operator->() { return  m_pPointer; }

		AVSINLINE size_t& operator*() const  { return *m_pPointer; }
		AVSINLINE size_t* operator->() const { return  m_pPointer; }

		AVSINLINE const size_t& get()const { return  *m_pPointer; } 
	};
	class nullable_double : public nullable_base<double>
	{
	public:
		nullable_double() : nullable_base<double>()
		{
		}

		AVSINLINE void normalize(const double& min, const double& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		nullable_double& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new double(XmlUtils::GetDouble(cwsValue));

			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
			RELEASEOBJECT(m_pPointer);
			if (NULL != value)
				m_pPointer = new double(XmlUtils::GetDouble(value));
		}
#endif
		AVSINLINE void operator=(const double& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new double(value);
		}

		nullable_double& operator=(const nullable_double& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oSrc.m_pPointer )
				m_pPointer = new double(*oSrc);

			return *this;
		}

		AVSINLINE double get_value_or(const double& value) const
		{
			if (NULL == m_pPointer)
			{
				double ret = value;
				return ret;
			}
			return *m_pPointer;
		}

	public:
		AVSINLINE double& operator*()  { return *m_pPointer; }
		AVSINLINE double* operator->() { return  m_pPointer; }

		AVSINLINE double& operator*() const  { return *m_pPointer; }
		AVSINLINE double* operator->() const { return  m_pPointer; }

		AVSINLINE const double& get()const { return  *m_pPointer; } 
	};
	class nullable_bool : public nullable_base<bool>
	{
	public:
		nullable_bool() : nullable_base<bool>()
		{
		}
	protected:
		bool set(const CString& value)
		{
			if ((_T("true") == value) || (_T("1") == value))
				return true;
			return false;
		}
	public:
		nullable_bool& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new bool(XmlUtils::GetBoolean2(cwsValue));

			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
			RELEASEOBJECT(m_pPointer);
			if (NULL != value)
				m_pPointer = new bool(set((CString)value));
		}
#endif
		AVSINLINE void operator=(const bool& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new bool(value);
		}

		nullable_bool& operator=(const nullable_bool& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oSrc.m_pPointer )
				m_pPointer = new bool(*oSrc);

			return *this;
		}

		AVSINLINE bool get_value_or(const bool& value) const
		{
			if (NULL == m_pPointer)
			{
				bool ret = value;
				return ret;
			}
			return *m_pPointer;
		}

	public:
		AVSINLINE bool& operator*()  { return *m_pPointer; }
		AVSINLINE bool* operator->() { return  m_pPointer; }

		AVSINLINE bool& operator*() const  { return *m_pPointer; }
		AVSINLINE bool* operator->() const { return  m_pPointer; }

		AVSINLINE const bool& get()const { return  *m_pPointer; } 
	public:
		AVSINLINE CString toString() const
		{
			CString result;
			//if (IsInit())
			//	result =  get() ? _T("true") : _T("false");

			return result;
		}
	};
	class nullable_string : public nullable_base<CString>
	{
	public:
		nullable_string() : nullable_base<CString>()
		{
		}
		nullable_string(const nullable_string& oOther)
		{
			if ( NULL == oOther.m_pPointer )
				m_pPointer = NULL;
			else
				m_pPointer	= new CString( *oOther.m_pPointer );
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator=(const BSTR& value)
		{
			RELEASEOBJECT(m_pPointer);
			if (NULL != value)
				m_pPointer = new CString(value);
		}
#endif
		AVSINLINE void operator=(const CString& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new CString(value);
			
		}
		AVSINLINE void operator=(CString* value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = value;
		}
		nullable_string& operator=(const nullable_string& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oSrc.m_pPointer )
				m_pPointer = new CString(*oSrc);
			return *this;
		}

		AVSINLINE CString get_value_or(const CString& value) const
		{
			if (NULL == m_pPointer)
			{
				CString ret = value;
				return ret;
			}
			return *m_pPointer;
		}

	public:
		AVSINLINE CString& operator*()  { return *m_pPointer; }
		AVSINLINE CString* operator->() { return  m_pPointer; }

		AVSINLINE CString& operator*() const  { return *m_pPointer; }
		AVSINLINE CString* operator->() const { return  m_pPointer; }

		AVSINLINE CString& get()const { return  *m_pPointer; } 
	};
}
