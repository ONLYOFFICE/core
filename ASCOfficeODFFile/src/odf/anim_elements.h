#pragma once

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore { 
namespace odf {



//anim:par
class anim_par : public office_element_impl<anim_par>//Параллельные анимации
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeAnimPar;
    CPDOCCORE_DEFINE_VISITABLE();

  	office_element_ptr		 anim_par_;
	office_element_ptr_array anim_seq_array_;
	office_element_ptr_array content_;

	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

};
CP_REGISTER_OFFICE_ELEMENT2(anim_par);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:seq
class anim_seq : public office_element_impl<anim_seq>//Последовательные анимации
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeAnimSeq;
    CPDOCCORE_DEFINE_VISITABLE();

	office_element_ptr_array anim_par_array_;
   
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);


};
CP_REGISTER_OFFICE_ELEMENT2(anim_seq);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:iterate 
//class anim_iterate : public office_element_impl<anim_iterate>//Итеративные анимации

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim-transition-filter-attlist
class anim_transition_filter_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
	
	_CP_OPT(std::wstring)	smil_direction_;
	_CP_OPT(std::wstring)	smil_subtype_; 
	_CP_OPT(std::wstring)	smil_type_;
	_CP_OPT(std::wstring)	smil_dur_;
	_CP_OPT(std::wstring)	smil_mode_;
	_CP_OPT(color)			smil_fadeColor;
}

//anim:transitionFilter
class anim_transitionFilter : public office_element_impl<anim_transitionFilter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeAnimTransitionFilter;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context & Context);
///////////////////////////////////////////////////////////	
	common_anim_smil_attlist		common_anim_smil_attlist_;
	anim_transition_filter_attlist	anim_transition_filter_attlist_;


private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );


};
CP_REGISTER_OFFICE_ELEMENT2(anim_transitionFilter);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:audio
//anim:command

}
}
