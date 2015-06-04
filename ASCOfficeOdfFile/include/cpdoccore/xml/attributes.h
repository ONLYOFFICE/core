#pragma once

#include <string>
#include <vector>

#include "../../cpdoccore/CPSharedPtr.h"
#include "../../cpdoccore/CPOptional.h"
#include "../../cpdoccore/xml/xmlchar.h"

#include "../../logging.h"

#include <boost/variant.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace cpdoccore 
{
	namespace odf 
	{
		struct _property
		{
			_property(std::wstring n, std::wstring s){name_	=	n; val_	=	s;}
			_property(std::wstring n, bool b)	{name_	=	n; val_	=	b;}
			_property(std::wstring n, int i){name_	=	n; val_	=	i;}
			_property(std::wstring n, double d){name_	=	n;val_	=	d;}
		
			std::wstring name_;
			boost::variant<bool,std::wstring,double,int> val_;
		};

		template <class T>
        bool GetProperty(std::vector<_property> Heap, const std::wstring Name, T & Val)
		{
            typedef typename T::value_type T_value_type;

			Val.reset();
			BOOST_FOREACH(_property const & p, Heap)
			{
				if (p.name_ == Name )
				{
					try
					{
                        Val = boost::get<T_value_type>(p.val_);
					}
					catch(...)
					{
#ifdef _DEBUG
						_CP_LOG(warning) << L"[warning] : incorrect type convert \'" << Name << L"\'\n";
#endif
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
                      return optional_V_type (::boost::lexical_cast<V>( *val ) );
					//return common::read_string<V>( *val );
				}
				catch(...)
				{
#ifdef _DEBUG
					_CP_LOG(warning) << L"[warning] : could't read attribute \'" << QualifiedName << L"\'\n";
#endif
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
                ::boost::algorithm::to_lower(tmp);
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
