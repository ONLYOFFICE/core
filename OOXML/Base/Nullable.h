/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "Base.h"
#include "Unit.h"
#include "SmartPtr.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/BaseObject.h"

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
		Type& operator*()  { return *m_pPointer; }
		Type* operator->() { return  m_pPointer; }

		Type& operator*() const  { return *m_pPointer; }
		Type* operator->() const { return  m_pPointer; }

		const Type& get()const { return  *m_pPointer; }
		Type& get() { return  *m_pPointer; }

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
		bool IsInit() const
		{
			return (NULL != m_pPointer);
		}
		bool is_init() const
		{
			return IsInit();
		}

		void reset(Type* pType = NULL)
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

		nullable(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsValid())
				this->m_pPointer = new Type(oReader);
			else
				this->m_pPointer = NULL;
		}

		nullable(XLS::BaseObjectPtr& obj)
		{
			if (obj != nullptr)
				this->m_pPointer = new Type(obj);
			else
				this->m_pPointer = NULL;
		}

		nullable(std::vector<XLS::BaseObjectPtr>& obj)
		{
			if (!obj.empty())
				this->m_pPointer = new Type(obj);
			else
				this->m_pPointer = NULL;
		}

		nullable<Type>& operator=(XLS::BaseObjectPtr& obj)
		{
			RELEASEOBJECT(this->m_pPointer);
			if (obj != nullptr)
				this->m_pPointer = new Type(obj);
			return *this;
		}

		nullable<Type>& operator=(std::vector<XLS::BaseObjectPtr>& obj)
		{
			RELEASEOBJECT(this->m_pPointer);
			if (!obj.empty())
				this->m_pPointer = new Type(obj);
			return *this;
		}

		nullable<Type>& operator=(XmlUtils::CXmlNode& oNode)
		{
			RELEASEOBJECT(this->m_pPointer);
			if (oNode.IsValid())
				this->m_pPointer = new Type(oNode);
			return *this;
		}
		nullable<Type>& operator=(XmlUtils::CXmlLiteReader& oReader)
		{
			RELEASEOBJECT(this->m_pPointer);
			if (oReader.IsValid())
				this->m_pPointer = new Type(oReader);
			return *this;
		}
		nullable<Type>& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(this->m_pPointer);
			if (NULL != cwsValue)
				this->m_pPointer = new Type( cwsValue );
			return *this;
		}

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

		Type& operator*()  { return *this->m_pPointer; }
		Type* operator->() { return  this->m_pPointer; }

		Type& operator*() const  { return *this->m_pPointer; }
		Type* operator->() const { return  this->m_pPointer; }

		const Type& get()const { return  *this->m_pPointer; }
		Type& get2()const { return  *this->m_pPointer; }

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

		bool Init()
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

		void operator=(const std::wstring& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new Type();
			this->m_pPointer->_set(value);
		}
		void operator=(Type* pType)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer	= pType;
		}
		void operator=(const BYTE& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new Type();
			this->m_pPointer->SetBYTECode(value);
		}

		void operator=(const Type& value)
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

		const std::wstring& get_value_or(const std::wstring& value) const
		{
			if (NULL == this->m_pPointer)
				return value;
			return this->m_pPointer->get();
		}
		const std::wstring& get_value() const
		{
			return this->m_pPointer->get();
		}

		Type& operator*()  { return *this->m_pPointer; }
		Type* operator->() { return  this->m_pPointer; }

		Type& operator*() const  { return *this->m_pPointer; }
		Type* operator->() const { return  this->m_pPointer; }

		const Type& get()const { return  *this->m_pPointer; }
	};

	class nullable_int : public nullable_base<int>
	{
	public:
		nullable_int() : nullable_base<int>()
		{
		}
		void normalize(const int& min, const int& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		void normalize_positive()
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
		void operator=(const std::wstring& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new int(XmlUtils::GetInteger(value));
		}
		void operator=(const int& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new int(value);
		}
		std::wstring ToString()  const
		{
			return std::to_wstring(*m_pPointer);
		}
		nullable_int& operator=(const nullable_int& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if (NULL != oSrc.m_pPointer )
				m_pPointer = new int(*oSrc);
			return *this;
		}
		const bool operator==(const nullable_int& oOther) const
		{
			if ( !this->m_pPointer )
				return false;

			return (*this->m_pPointer) == *oOther;
		}
		const bool operator==(const int& oOther) const
		{
			if ( !this->m_pPointer )
				return false;

			return (*this->m_pPointer) == oOther;
		}
		int get_value_or(const int& value) const
		{
			if (NULL == m_pPointer)
			{
				int ret = value;
				return ret;
			}
			return *m_pPointer;
		}
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + std::to_wstring(*m_pPointer) + L"\" ";
			}
			return L"";
		}
		int& operator*()  { return *m_pPointer; }
		int* operator->() { return  m_pPointer; }

		int& operator*() const  { return *m_pPointer; }
		int* operator->() const { return  m_pPointer; }

		const int& get()const { return  *m_pPointer; }
	};
	class nullable_uint : public nullable_base<unsigned int>
	{
	public:
		nullable_uint() : nullable_base<unsigned int>()
		{
		}

		void normalize(const unsigned int& min, const unsigned int& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		void normalize_positive()
		{
			if (IsInit())
			{
				if ((int)(*m_pPointer) < 0)
					*m_pPointer = 0;
			}
		}
		nullable_uint& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new unsigned int(XmlUtils::GetUInteger(cwsValue));

			return *this;
		}
		void operator=(const std::wstring& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new unsigned int(XmlUtils::GetUInteger(value));
		}
		void operator=(const unsigned int& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new unsigned int(value);
		}

		nullable_uint& operator=(const nullable_uint& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if (NULL != oSrc.m_pPointer )
				m_pPointer = new unsigned int(*oSrc);
			return *this;
		}

		unsigned int get_value_or(const unsigned int& value) const
		{
			if (NULL == m_pPointer)
			{
				unsigned int ret = value;
				return ret;
			}
			return *m_pPointer;
		}
		unsigned int& operator*()  { return *m_pPointer; }
		unsigned int* operator->() { return  m_pPointer; }

		unsigned int& operator*() const  { return *m_pPointer; }
		unsigned int* operator->() const { return  m_pPointer; }

		const unsigned int& get()const { return  *m_pPointer; }
	};
	class nullable_int64 : public nullable_base<_INT64>
	{
	public:
		nullable_int64() : nullable_base<_INT64>()
		{
		}
		void normalize(const _INT64& min, const _INT64& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		void normalize_positive()
		{
			if (IsInit())
			{
				if (*m_pPointer < 0)
					*m_pPointer = 0;
			}
		}
		nullable_int64& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new _INT64(XmlUtils::GetInteger64(cwsValue));

			return *this;
		}
		void operator=(const std::wstring& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new _INT64(XmlUtils::GetInteger64(value));
		}
		void operator=(const _INT64& value)
		{
			RELEASEOBJECT(this->m_pPointer);
			this->m_pPointer = new _INT64(value);
		}

		nullable_int64& operator=(const nullable_int64& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if (NULL != oSrc.m_pPointer )
				m_pPointer = new _INT64(*oSrc);
			return *this;
		}
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + std::to_wstring(*m_pPointer) + L"\" ";
			}
			return L"";
		}
		_INT64 get_value_or(const _INT64& value) const
		{
			if (NULL == m_pPointer)
			{
				int ret = (int)value;
				return ret;
			}
			return *m_pPointer;
		}
		_INT64& operator*()  { return *m_pPointer; }
		_INT64* operator->() { return  m_pPointer; }

		_INT64& operator*() const  { return *m_pPointer; }
		_INT64* operator->() const { return  m_pPointer; }

		const _INT64& get()const { return  *m_pPointer; }
	};
	class nullable_sizet : public nullable_base<size_t>
	{
	public:
		nullable_sizet() : nullable_base<size_t>()
		{
		}

		void normalize(const size_t& max)
		{
			if (IsInit())
			{
				if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		nullable_sizet& operator=(const std::wstring & sValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( !sValue.empty() )
				m_pPointer = new size_t((size_t)XmlUtils::GetUInteger(sValue));

			return *this;
		}
		void operator=(const size_t& value)
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
		size_t get_value_or(const size_t& value) const
		{
			if (NULL == m_pPointer)
			{
				size_t ret = value;
				return ret;
			}
			return *m_pPointer;
		}
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + std::to_wstring(*m_pPointer) + L"\" ";
			}
			return L"";
		}
		size_t& operator*()  { return *m_pPointer; }
		size_t* operator->() { return  m_pPointer; }

		size_t& operator*() const  { return *m_pPointer; }
		size_t* operator->() const { return  m_pPointer; }

		const size_t& get()const { return  *m_pPointer; }
	};
	class nullable_double : public nullable_base<double>
	{
	public:
		nullable_double() : nullable_base<double>()
		{
		}
		void normalize(const double& min, const double& max)
		{
			if (IsInit())
			{
				if (*m_pPointer < min)
					*m_pPointer = min;
				else if (*m_pPointer > max)
					*m_pPointer = max;
			}
		}
		nullable_double& operator=(const std::wstring & sValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( !sValue.empty() )
				m_pPointer = new double(XmlUtils::GetDouble(sValue));

			return *this;
		}
		void operator=(const double& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new double(value);
		}
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + std::to_wstring(*m_pPointer) + L"\" ";
			}
			return L"";
		}
		nullable_double& operator=(const nullable_double& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != oSrc.m_pPointer )
				m_pPointer = new double(*oSrc);

			return *this;
		}
		double get_value_or(const double& value) const
		{
			if (NULL == m_pPointer)
			{
				double ret = value;
				return ret;
			}
			return *m_pPointer;
		}
		double& operator*()  { return *m_pPointer; }
		double* operator->() { return  m_pPointer; }

		double& operator*() const  { return *m_pPointer; }
		double* operator->() const { return  m_pPointer; }

		const double& get()const { return  *m_pPointer; }
	};
	class nullable_bool : public nullable_base<bool>
	{
	public:
		nullable_bool() : nullable_base<bool>()
		{
		}
	protected:
		bool set(const std::wstring& value)
		{
			if ((L"true" == value) || (L"1" == value))
				return true;
			return false;
		}
	public:
		nullable_bool& operator=(const std::wstring &sValue)
		{
			RELEASEOBJECT(m_pPointer);

			if (!sValue.empty() )
				m_pPointer = new bool(XmlUtils::GetBoolean2(sValue));

			return *this;
		}
		nullable_bool& operator=(const wchar_t* cwsValue)
		{
			RELEASEOBJECT(m_pPointer);

			if ( NULL != cwsValue )
				m_pPointer = new bool(XmlUtils::GetBoolean2(cwsValue));

			return *this;
		}
		void operator=(const bool& value)
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
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + (*m_pPointer ? L"1" : L"0") + L"\" ";
			}
			return L"";
		}
		bool get_value_or(const bool& value) const
		{
			if (NULL == m_pPointer)
			{
				bool ret = value;
				return ret;
			}
			return *m_pPointer;
		}

		bool& operator*()  { return *m_pPointer; }
		bool* operator->() { return  m_pPointer; }

		bool& operator*() const  { return *m_pPointer; }
		bool* operator->() const { return  m_pPointer; }

		const bool& get()const { return  *m_pPointer; }

	};
	class nullable_string : public nullable_base<std::wstring>
	{
	public:
		nullable_string() : nullable_base<std::wstring>()
		{
		}
		nullable_string(const nullable_string& oOther)
		{
			if (NULL == oOther.m_pPointer)
				m_pPointer = NULL;
			else
				m_pPointer = new std::wstring(*oOther.m_pPointer);
		}
		void operator=(const std::wstring& value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = new std::wstring(value);

		}
		void operator=(std::wstring* value)
		{
			RELEASEOBJECT(m_pPointer);
			m_pPointer = value;
		}
		nullable_string& operator=(const nullable_string& oSrc)
		{
			RELEASEOBJECT(m_pPointer);

			if (NULL != oSrc.m_pPointer)
				m_pPointer = new std::wstring(*oSrc);
			return *this;
		}
		const bool operator==(const nullable_string& oOther) const
		{
			if (!this->m_pPointer)
				return false;

			return (*this->m_pPointer) == *oOther;
		}
		const bool operator==(const std::wstring& oOther) const
		{
			if (!this->m_pPointer)
				return false;

			return (*this->m_pPointer) == oOther;
		}
		std::wstring get_value_or(const std::wstring& value) const
		{
			if (NULL == m_pPointer)
			{
				std::wstring ret = value;
				return ret;
			}
			return *m_pPointer;
		}
		std::wstring ToAttribute(const std::wstring & name)  const
		{
			if (m_pPointer)
			{
				return name + L"=\"" + (*m_pPointer) + L"\" ";
			}
			return L"";
		}
		std::wstring& operator*()  { return *m_pPointer; }
		std::wstring* operator->() { return  m_pPointer; }

		std::wstring& operator*() const  { return *m_pPointer; }
		std::wstring* operator->() const { return  m_pPointer; }

		std::wstring& get()const { return  *m_pPointer; }
	};
}
