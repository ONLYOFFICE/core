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

#include <vector>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/common/readdocelement.h>

#include "office_elements_type.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../docx/docx_conversion_context.h"
#include "../docx/xlsxconversioncontext.h"
#include "../docx/pptx_conversion_context.h"

#include "visitor.h"
#include "../conversionelement.h"

namespace cpdoccore {
namespace odf_reader {

class document_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;
typedef weak_ptr<office_element>::Type office_element_weak_ptr;
typedef ::std::vector<office_element_ptr> office_element_ptr_array;

class office_element : public xml::element<wchar_t>,
    public common::read_doc_element,
    public base_visitable,
    public oox::conversion_element
{    
public:
    office_element() : is_root_(false), context_(NULL) {}

    virtual ElementType get_type() const = 0;
    virtual ~office_element() = 0;
    void set_root(bool isRoot) { is_root_ = isRoot; }
    bool is_root() const { return is_root_; }

    virtual void afterCreate() {};
    virtual void afterReadContent() {};
   
    CPDOCCORE_DEFINE_VISITABLE();

    void setContext(document_context * Context) { context_ = Context; }

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const
    {
        _CP_LOG << L"[warning] use base text_to_stream\n";
        return _Wostream;
    }

    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const
    {
        _CP_LOG << L"[warning] use base xml_to_stream\n";
        return _Wostream;
    }
    document_context * getContext() { return context_; }
//
//protected:
    const document_context * getContext() const { return context_; }

private:
    bool is_root_;
    document_context * context_;
};

#define CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_ public:\
    virtual const wchar_t * get_ns() const { return ns; }\
    virtual const wchar_t * get_name() const { return name; }\
    virtual xml::NodeType get_xml_type() const { return xml_type; }\
    virtual ElementType get_type() const { return type; }



inline office_element::~office_element()
{
}

template <class Element>
class office_element_impl : public virtual office_element
{
// xml::element impl
public:
    virtual const wchar_t * get_ns() const
    { 
        return Element::ns;
    }
    
    virtual const wchar_t * get_name() const
    { 
        return Element::name;
    }

    virtual xml::NodeType get_xml_type() const
    { 
        return Element::xml_type;
    }

private:
    virtual void add_text(const std::wstring & Text)
    {
        std::wstring text = Text;
        boost::algorithm::erase_all(text, L" ");
        boost::algorithm::erase_all(text, L"\t");
        boost::algorithm::erase_all(text, L"\n");
        boost::algorithm::erase_all(text, L"\r");
        //CP_ASSERT(false);
    }

// office_element impl
public:
    virtual ElementType get_type() const
    { 
        return Element::type;
    }
   
};


}
}

