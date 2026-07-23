/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "attributes.h"
#include "sax.h"

namespace cpdoccore {
namespace xml {

/// Vector for ensuring access control for all read attributes
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
                    //std::wcerr << L"[attributes] : " << i->first << std::endl;
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
