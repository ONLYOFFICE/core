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

#include <string>
#include <vector>

#include "../CPSharedPtr.h"
#include "../CPOptional.h"
#include "xmlchar.h"

#include "../logging.h"

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore 
{
	namespace odf_reader 
	{
		struct _property
		{
			_property(std::wstring n, std::wstring s)	{ name_	=	n; val_	=	s;}
			_property(std::wstring n, bool b)			{ name_	=	n; val_	=	b;}
			_property(std::wstring n, int i)			{ name_	=	n; val_	=	i;}
			_property(std::wstring n, double d)			 {name_	=	n;val_	=	d;}
		
			std::wstring name_;
			boost::variant<bool, std::wstring, double, int> val_;
		};

		template <class T>
        bool GetProperty(const std::vector<_property> & Heap, const std::wstring Name, T & Val)
		{
            typedef typename T::value_type T_value_type;

			Val.reset();
			for (size_t i = 0 ; i < Heap.size(); i++)
			{
				if (Heap[i].name_ == Name )
				{
					try
					{
                        Val = boost::get<T_value_type>(Heap[i].val_);
					}
					catch(...)
					{
                        _CP_LOG << L"[warning] : incorrect type convert \'" << Name << L"\'\n";
					}
					return true;
				}
			}
			return false;
		}
    }
	namespace xml
	{

	class sax;

	class attributes 
	{
	public:
        typedef optional<std::wstring>::Type               value_type;
        typedef std::pair< std::wstring, std::wstring>   key_value;

	public:
        virtual value_type get(const std::wstring & QualifiedName) const = 0;

        template <class V> typename optional<V>::Type get_val(const std::wstring & QualifiedName);

        virtual const key_value & at(size_t _Pos) const = 0;
		virtual size_t size() const = 0;
		size_t length() const ;
		bool empty() const;

		virtual void reset_check() const = 0;
		virtual bool check() const = 0;
	    
		virtual ~attributes() = 0;

	};



    template <class V>
	class attributes_get_val_impl
	{
	public:

        typedef typename optional<V>::Type  optional_V_type;

        static typename optional<V>::Type get_val(attributes & attr, const std::wstring & QualifiedName)
		{
            attributes::value_type val = attr.get(QualifiedName);
			if (val)
			{
				try 
				{
                      return optional_V_type (boost::lexical_cast<V>( *val ) );
					//return common::read_string<V>( *val );
				}
				catch(...)
				{
                    _CP_LOG << L"[warning] : could't read attribute \'" << QualifiedName << L"\'\n";
				}
			}

            return optional_V_type();
		}
	};

    template<>
    class attributes_get_val_impl<bool>
	{
	public:

        typedef xml::xml_char<wchar_t> xml_char_value_type;

        static optional<bool>::Type get_val(attributes & attr, const std::wstring & QualifiedName)
		{
            attributes::value_type val = attr.get(QualifiedName);
			if (val)
			{
                std::wstring tmp = *val;
				tmp = XmlUtils::GetLower(tmp);
                return  optional<bool>::Type((tmp == xml_char_value_type::trueVal));
			}
			else
                return optional<bool>::Type();
		}
	};

    template<>
    class attributes_get_val_impl<char>
	{
	public:
        static optional<char>::Type get_val(attributes & attr, const std::wstring & QualifiedName)
		{
            attributes::value_type val = attr.get(QualifiedName);
			if (val)
			{
                std::wstring tmp = *val;
                return  optional<char>::Type( (tmp.size() > 0) ? tmp[0] : ' ');
			}
			else
                return optional<char>::Type();
		}
	};

    template<>
    class attributes_get_val_impl<wchar_t>
	{
	public:
        static optional<wchar_t>::Type get_val(attributes & attr, const std::wstring & QualifiedName)
		{
            attributes::value_type val = attr.get(QualifiedName);
			if (val)
			{
                std::wstring tmp = *val;
                return  optional<wchar_t>::Type( (tmp.size() > 0) ? tmp[0] : L' ');
			}
			else
                return optional<wchar_t>::Type();
		}
	};

	template <class V>
    inline typename optional<V>::Type attributes::get_val(const std::wstring & QualifiedName)
	{
        return attributes_get_val_impl<V>::get_val(*this, QualifiedName);
	}

    inline attributes::~attributes()
	{}

    inline size_t attributes::length() const
	{
		return this->size();
	}

    inline bool attributes::empty() const
	{
		return (this->size() == 0);
	}

	struct attributes_ptr
	{
        typedef shared_ptr< attributes >::Type Type;
	};



    typedef attributes attributes_wc;

    typedef shared_ptr< attributes_wc >::Type attributes_wc_ptr;

    template <class T>
    static bool _cp_apply_attribute(xml::attributes_wc_ptr attr, const std::wstring & QualifiedName, T & Val)
	{
		Val = attr->get_val<typename T::value_type>(QualifiedName);
		return (!!Val);
	}

    template <class T>
    static bool _cp_apply_attribute(xml::attributes_wc_ptr attr, const std::wstring & QualifiedName, T & Val, const T & Default)
	{
        typedef typename optional<T>::Type type_opt_t;
        type_opt_t tmp;
		try
		{
			tmp = attr->get_val<T>(QualifiedName);
			Val = tmp.get_value_or(Default);
		}catch(...)
		{
		}
		return (!!tmp);
	}

#if defined(_WIN32) || defined(_WIN64)
    #define CP_APPLY_ATTR(NAME, VAL, ...) _cp_apply_attribute(Attributes, (NAME), (VAL), __VA_ARGS__)
#else
    #define CP_APPLY_ATTR(NAME, VAL, ...) _cp_apply_attribute(Attributes, (NAME), (VAL), ##__VA_ARGS__)
#endif
	attributes_wc_ptr read_attributes(sax * SaxReader);

	}
}
