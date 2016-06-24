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

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/common_attlists.h"
#include "datatypes/smil_transitiontype.h"

namespace cpdoccore { 
namespace odf_reader {



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
	_CP_OPT(odf_types::smil_transition_type)	smil_type_;
	_CP_OPT(std::wstring)	smil_mode_;
	_CP_OPT(odf_types::color)			smil_fadeColor_;
	_CP_OPT(odf_types::clockvalue)		smil_dur_;
};

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
	odf_types::common_anim_smil_attlist		common_anim_smil_attlist_;
	anim_transition_filter_attlist			anim_transition_filter_attlist_;


private:
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );


};
CP_REGISTER_OFFICE_ELEMENT2(anim_transitionFilter);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:audio
//anim:command

}
}
