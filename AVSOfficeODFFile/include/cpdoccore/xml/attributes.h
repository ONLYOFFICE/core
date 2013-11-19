#ifndef _CPDOCCORE_XMLATTRIBUTES_H_
#define _CPDOCCORE_XMLATTRIBUTES_H_

#include <string>
#include <vector>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/common/readstring.h>
#include <boost_string.h>
#include <cpdoccore/xml/xmlchar.h>

#include <boost/variant.hpp>
#include <boost/foreach.hpp>

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
		bool GetProperty(std::vector<_property> Heap,const std::wstring Name, T & Val)
		{
			Val.reset();
			BOOST_FOREACH(_property const & p, Heap)
			{
				if (p.name_ == Name )
				{
					try
					{
						Val = boost::get<T::value_type>(p.val_);
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
	};
	namespace xml
	{

	class sax;

	template <class StringT>
	class attributes;

	template <class StringT>
	class attributes 
	{
	public:
		typedef typename _CP_OPT(StringT) value_type;
		typedef typename ::std::pair< StringT, StringT> key_value;

	public:
		virtual value_type get(const StringT & QualifiedName) const = 0;
		template <class V> typename _CP_OPT(V) get_val(const StringT & QualifiedName);
		virtual const key_value & at(size_t _Pos) const = 0;
		virtual size_t size() const = 0;
		size_t length() const ;
		bool empty() const;

		virtual void reset_check() const = 0;
		virtual bool check() const = 0;
	    
		virtual ~attributes() = 0;

	};

	template <class StringT, class V>
	class attributes_get_val_impl
	{
	public:
		static typename _CP_OPT(V) get_val(attributes<StringT> & attr, const StringT & QualifiedName)
		{
			attributes<StringT>::value_type val = attr.get(QualifiedName);     
			if (val)
			{
				try 
				{
					return common::read_string<V>( *val );
				}
				catch(...)
				{
#ifdef _DEBUG
					_CP_LOG(warning) << L"[warning] : could't read attribute \'" << QualifiedName << L"\'\n";
#endif
				}
			}

			return _CP_OPT(V)();
		}
	};

	template <class StringT>
	class attributes_get_val_impl<StringT, bool>
	{
	public:
		static typename _CP_OPT(bool) get_val(attributes<StringT> & attr, const StringT & QualifiedName)
		{
			attributes<StringT>::value_type val = attr.get(QualifiedName);     
			if (val)
			{
				StringT tmp = *val;
				::boost::algorithm::to_lower(tmp);
				return  _CP_OPT(bool)((tmp == xml::xml_char< StringT::value_type >::trueVal));
			}
			else
				return _CP_OPT(bool)();    
		}
	};

	template <class StringT>
	class attributes_get_val_impl<StringT, char>
	{
	public:
		static typename _CP_OPT(char) get_val(attributes<StringT> & attr, const StringT & QualifiedName)
		{
			attributes<StringT>::value_type val = attr.get(QualifiedName);     
			if (val)
			{
				StringT tmp = *val;
				return  _CP_OPT(char)( (tmp.size() > 0) ? tmp[0] : ' ');
			}
			else
				return _CP_OPT(char)();    
		}
	};

	template <class StringT>
	class attributes_get_val_impl<StringT, wchar_t>
	{
	public:
		static typename _CP_OPT(wchar_t) get_val(attributes<StringT> & attr, const StringT & QualifiedName)
		{
			attributes<StringT>::value_type val = attr.get(QualifiedName);     
			if (val)
			{
				StringT tmp = *val;
				return  _CP_OPT(wchar_t)( (tmp.size() > 0) ? tmp[0] : L' ');
			}
			else
				return _CP_OPT(wchar_t)();    
		}
	};

	template <class StringT>
	template <class V>
	inline typename _CP_OPT(V) attributes<StringT>::get_val(const StringT & QualifiedName)
	{
		return attributes_get_val_impl<StringT, V>::get_val(*this, QualifiedName);
	}

	template <class StringT>
	inline attributes<StringT>::~attributes()
	{}

	template <class StringT>
	inline size_t attributes<StringT>::length() const
	{
		return this->size();
	}

	template <class StringT>
	inline bool attributes<StringT>::empty() const
	{
		return (this->size() == 0);
	}

	template <class T>
	struct attributes_ptr
	{
		typedef typename shared_ptr< attributes<T> >::Type Type;
	};



	typedef attributes< ::std::string > attributes_c;
	typedef attributes< ::std::wstring > attributes_wc;

	typedef shared_ptr< attributes_c >::Type attributes_c_ptr;
	typedef shared_ptr< attributes_wc >::Type attributes_wc_ptr;

	template <class StringT, class T>
	static bool _cp_apply_attribute(xml::attributes_wc_ptr attr, const StringT & QualifiedName, T & Val)
	{
		Val = attr->get_val<typename T::value_type>(QualifiedName);
		return (!!Val);
	}

	template <class StringT, class T>
	static bool _cp_apply_attribute(xml::attributes_wc_ptr attr, const StringT & QualifiedName, T & Val, const T & Default)
	{
		_CP_OPT(T) tmp = attr->get_val<T>(QualifiedName);
		Val = tmp.get_value_or(Default);
		return (!!tmp);
	}

	#define CP_APPLY_ATTR(NAME, VAL, ...) _cp_apply_attribute(Attributes, (NAME), (VAL), __VA_ARGS__)

	attributes_wc_ptr read_attributes(sax * SaxReader);

	}
}

#endif // #ifndef _CPDOCCORE_XMLATTRIBUTES_H_
