/// \file   attributes.cpp
/// \author nikolay.pogorskiy@gmail.com 

#include "precompiled_cpxml.h"

#include "../../include/cpdoccore/xml/attributes.h"
#include "../../include/cpdoccore/xml/sax.h"

#include <boost/foreach.hpp>
#include <map>
#include <vector>
#include <string>
#include <iostream>

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

template <class StringT>
class attributes_impl : public attributes<StringT> 
{
public:
    typedef typename attributes<StringT>::value_type value_type;
    typedef typename attributes<StringT>::key_value key_value;

public:
    attributes_impl() : check_(true){};
    
    virtual ~attributes_impl() 
    {

    }

public:
    void add(const StringT & QualifiedName, const StringT & Value);
    
public:
    virtual value_type get(const StringT & QualifiedName) const ;
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

    typedef std::map<StringT, size_t> AttribMap;
    AttribMap attrib_;
    VectorVal values_;
    mutable bool check_;

};

template <class StringT>
void attributes_impl<StringT>::add(const StringT & QualifiedName, const StringT & Value)
{
    values_.push_back( key_value(QualifiedName, Value) );
    attrib_.insert( AttribMap::value_type(QualifiedName, values_.size() - 1) );
}

template <class StringT>
typename attributes_impl<StringT>::value_type attributes_impl<StringT>::get(const StringT & QualifiedName) const 
{
    AttribMap::const_iterator i = attrib_.find( QualifiedName );
    if ( i != attrib_.end() )
        return attributes<StringT>::value_type( values_[i->second].second );
    else
        return attributes<StringT>::value_type();
}

template <class StringT>
typename const attributes_impl<StringT>::key_value & attributes_impl<StringT>::at(size_t _Pos) const
{
    return values_[_Pos];
}

template <class StringT>
size_t attributes_impl<StringT>::size() const
{
    return attrib_.size();
}

attributes_wc_ptr read_attributes(sax * SaxReader)
{
    typedef attributes_impl< ::std::wstring > attributes_impl_wc;
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
