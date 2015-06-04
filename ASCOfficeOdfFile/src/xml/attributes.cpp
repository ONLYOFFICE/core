#include <map>
#include <vector>
#include <string>
#include <iostream>

#include <boost/foreach.hpp>

#include "../../include/cpdoccore/xml/attributes.h"
#include "../../include/cpdoccore/xml/sax.h"

namespace cpdoccore {
namespace xml {

/// Вектор для обеспечения контроля доступа для всех прочитанных атрибутов
template <class T>
class attribute_debug_vector
{
public:
    typedef std::vector<T> internal_vector;
    typedef typename internal_vector::const_reference const_reference;
    typedef typename internal_vector::reference reference;

    void push_back(const T & val) 
    {
        access_.push_back(false);
        return vector_.push_back(val); 
    }
    
    const_reference operator[](size_t Index) const
    { 
        access_[Index] = true;
        return vector_[Index]; 
    }

    reference operator[](size_t Index)
    { 
        access_[Index] = true;
        return vector_[Index]; 
    }

    size_t size() const 
    {
        return vector_.size();
    }

    ~attribute_debug_vector()
    {
    }

    std::vector<bool> & access() { return access_; }
    const std::vector<bool> & access() const { return access_; }

private:
    mutable std::vector<bool> access_;
    internal_vector vector_;
};

class attributes_impl : public attributes
{
public:
    typedef attributes::value_type value_type;
    typedef attributes::key_value  key_value;

public:
    attributes_impl() : check_(true){};
    
    virtual ~attributes_impl() 
    {

    }

public:
    void add(const std::wstring & QualifiedName, const std::wstring & Value);
    
public:
    virtual value_type get(const std::wstring & QualifiedName) const ;
    virtual const key_value & at(size_t _Pos) const;
    virtual size_t size() const;
    
    virtual bool check() const
    {
        bool res = true;
#ifdef _DEBUG
        if (check_)
        {
            AttribMap::const_iterator i = attrib_.begin();
            for(;i != attrib_.end(); ++i)
            {
                if (!values_.access()[i->second])
                {
                    res = false;
                    std::wcerr << L"[attributes] : " << i->first << std::endl;            
                }
            }
        }
#endif
        return res;
    }
    
    virtual void reset_check() const
    {
        check_ = false;    
    }

private:
#ifdef _DEBUG
    typedef attribute_debug_vector< key_value > VectorVal;
#else
    typedef std::vector< key_value > VectorVal;
#endif

    typedef std::map<std::wstring, size_t> AttribMap;
    AttribMap attrib_;
    VectorVal values_;
    mutable bool check_;

};

void attributes_impl::add(const std::wstring & QualifiedName, const std::wstring & Value)
{
    typedef AttribMap::value_type attr_map_value_type;

    values_.push_back( key_value(QualifiedName, Value) );
    attrib_.insert( attr_map_value_type(QualifiedName, values_.size() - 1) );
}

attributes_impl::value_type attributes_impl::get(const std::wstring & QualifiedName) const
{
    typedef AttribMap::const_iterator attr_map_iterator;
    typedef attributes::value_type attr_str_type;

    attr_map_iterator i = attrib_.find( QualifiedName );

    if ( i != attrib_.end() )
        return attr_str_type( values_[i->second].second );
    else
        return attr_str_type();
}

const attributes_impl::key_value & attributes_impl::at(size_t _Pos) const
{
    return values_[_Pos];
}

size_t attributes_impl::size() const
{
    return attrib_.size();
}

attributes_wc_ptr read_attributes(sax * SaxReader)
{
    typedef attributes_impl attributes_impl_wc;
    _CP_PTR(attributes_impl_wc) attributes = boost::make_shared<attributes_impl_wc>();

    if (SaxReader->attrCount() > 0)
    {
		SaxReader->moveToAttrFirst();
        for(;;)
        {
            if (!SaxReader->attrDefault())
            {
                const std::wstring prefix	= SaxReader->namespacePrefix();
                const std::wstring name		= SaxReader->nodeLocalName();
                const std::wstring value	= SaxReader->value();

				if (prefix.length() < 1 && name.length() < 1 && value.length() < 1)
					break;

                attributes->add( prefix + (prefix.size() ? L":" : L"") + name, value);
            }
            if (!SaxReader->moveToAttrNext())
                break;
        }
    }

    SaxReader->moveToAttrOwner();

    return attributes;
}

}
}
