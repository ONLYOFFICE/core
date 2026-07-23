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
#pragma once

#include <string>
#include <map>

#include <boost/function.hpp>

#include <CPSharedPtr.h>

#include "office_elements.h"


namespace cpdoccore { 
namespace odf_writer {

class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

#define CP_REGISTER_OFFICE_ELEMENT2(A) \
namespace { \
    RegisterElement<A> RegisterElement##A;\
}

class office_element_creator
{
public:
    typedef boost::function< office_element_ptr () > CreateFuncImpl;
        
public:
    // Register element
    bool register_element(const std::wstring &ns, const std::wstring & name, CreateFuncImpl f);

    // Create element by name
    office_element_ptr create(const std::wstring & ns, const std::wstring & name, odf_conversion_context * Context = NULL, bool isRoot = false) const;

private:
    typedef std::map<std::wstring, CreateFuncImpl> MapType;
    MapType map_;

public:
    // Singleton implementation
    static office_element_creator * get();

private:
    office_element_creator();
    static office_element_creator * instance_;
};

/// Class that provides element registration
template <class T>
class RegisterElement
{
private:
    template <class V>
    class CreateImpl_
    {
    public:
        static typename shared_ptr<V>::Type create()
        {
            //return shared_ptr<T>::Type( new T() );
            return boost::make_shared<V>();
        }
    };

public:
    RegisterElement()
    {
        if (class_registered_++ == 0)
        {
            office_element_creator::get()->register_element(T::ns, T::name, &CreateImpl_<T>::create);            
        }
    };

    ~RegisterElement()
    {            
    };
private:
    static int class_registered_;    
};

template<class T> int RegisterElement<T>::class_registered_ = 0;

/// \brief  Create element
bool create_element(const std::wstring & Ns,
                    const std::wstring & Name,
                    office_element_ptr & _Element,
                    odf_conversion_context * Context,
                    bool isRoot = false);

bool create_element(const std::wstring & Ns,
                             const std::wstring & Name,
                             office_element_ptr_array & _Elements,
                             odf_conversion_context * Context,
                             bool isRoot = false);

#define CP_CREATE_ELEMENT_SIMPLE(ELEMENT) create_element(Ns, Name, (ELEMENT), Context)
#define CP_CREATE_ELEMENT(ELEMENT) create_element(Ns, Name, (ELEMENT), getContext())

#define _CPDOCCORE_CREATE_ELEMENT_ROOT(ELEMENT) create_element(Ns, Name, (ELEMENT), getContext(), true)

#define CP_CHECK_NAME(NS, NAME) ((NS) == Ns && (NAME) == Name)

void not_applicable_element(const office_element * CurrentElm, const std::wstring & Ns, const std::wstring & Name);
void not_applicable_element(const std::wstring & Current, const std::wstring & Ns, const std::wstring & Name);

#define CP_NOT_APPLICABLE_ELM() \
    not_applicable_element(this, Ns, Name)

#define CP_NOT_APPLICABLE_ELM_SIMPLE(A) \
    not_applicable_element(A, Ns, Name)
}
}
