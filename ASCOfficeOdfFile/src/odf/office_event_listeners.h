#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/common_attlists.h"

namespace cpdoccore { 
namespace odf {

/// \brief  office:event-listeners
class office_event_listeners : public office_element_impl<office_event_listeners>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeEventListeners;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array presentation_event_listeners_;
    office_element_ptr_array script_event_listeners_;

    
};

CP_REGISTER_OFFICE_ELEMENT2(office_event_listeners);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// presentation-event-listener-attlist
class presentation_event_listener_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	common_xlink_attlist common_xlink_attlist_;

	_CP_OPT(std::wstring)	script_event_name_;
	_CP_OPT(std::wstring)	presentation_action_;
	//presentation:verb
	//presentation:start-scale
	//presentation:speed
	//presentation:direction
	//presentation:effect
};

/// \brief  presentation:event-listeners_
class presentation_event_listener : public office_element_impl<presentation_event_listener>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationEventListener;
    CPDOCCORE_DEFINE_VISITABLE();
    
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    //office_element_ptr_array content_;
	office_element_ptr presentation_sound_;
	presentation_event_listener_attlist presentation_event_listener_attlist_;

    
};

CP_REGISTER_OFFICE_ELEMENT2(presentation_event_listener);


/// \brief  script:event-listeners_
class script_event_listener : public office_element_impl<presentation_event_listener>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeScriptEventListener;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array content_;

    
};

CP_REGISTER_OFFICE_ELEMENT2(script_event_listener);


}
}