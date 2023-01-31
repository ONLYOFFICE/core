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

#include <vector>
#include <CPOptional.h>
#include <CPSharedPtr.h>
#include <logging.h>
#include <boost/noncopyable.hpp>
#include <odf_elements_type.h>

namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class office_element;

typedef shared_ptr<office_element>::Type office_element_ptr;
typedef std::vector<office_element_ptr> office_element_ptr_array;

class office_element : boost::noncopyable 
{    
public:
    office_element() : context_(NULL) {}
    virtual ~office_element() = 0;

	virtual const wchar_t * get_ns() const = 0;
	virtual const wchar_t * get_name() const = 0;
	virtual ElementType get_type() const = 0;

	virtual void serialize(std::wostream & _Wostream) = 0 ;

	void setContext(odf_conversion_context * Context) { context_ = Context; }
    odf_conversion_context * getContext() { return context_; }
    const odf_conversion_context * getContext() const { return context_; }

	virtual void add_child_element( const office_element_ptr & child) = 0;
	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name)=0;

    virtual void add_text(const std::wstring & Text) {}

	void set_root(bool root){is_root_ = root;}
////////////////////////
    virtual std::wostream & serialize(std::wostream & _Wostream) const
    {
        _CP_LOG << L"[warning] use base serialize\n";
        return _Wostream;
    }
	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const
	{
		_CP_LOG << L"[warning] use base text_to_stream\n";
		return _Wostream;
	}
private:
    bool is_root_;	
	odf_conversion_context * context_;
};

inline office_element::~office_element()
{
}

template <class Element>
class office_element_impl : public virtual office_element
{
public:
    virtual const wchar_t * get_ns() const
    { 
        return Element::ns;
    }    
    virtual const wchar_t * get_name() const
    { 
        return Element::name;
    }
    virtual void add_child_element( const office_element_ptr & child)
    {
        _CP_LOG << L"Non add child in " << Element::ns << L":" << Element::name << std::endl;
    }
    virtual ElementType get_type() const
    { 
        return Element::type;
    }
   
};


}
}

