/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/animation_attlists.h"

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

	odf_types::common_anim_smil_attlist		common_attlist_;
	odf_types::anim_par_attlist				par_attlist_;

  	
	office_element_ptr_array				anim_par_array_; 
	office_element_ptr_array				anim_seq_array_;
	office_element_ptr_array				content_;

	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	boost::optional<int> pptx_convert_preset_id();
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

	odf_types::common_anim_smil_attlist		attlist_;
	office_element_ptr_array				anim_par_array_;
   
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);


};
CP_REGISTER_OFFICE_ELEMENT2(anim_seq);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:iterate 
//-------------------------------------------------------------------------------

class anim_transitionFilter : public office_element_impl<anim_transitionFilter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeAnimTransitionFilter;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context & Context);

	odf_types::common_anim_smil_attlist				common_attlist_;
	odf_types::anim_transition_filter_attlist		filter_attlist_;

private:
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

	std::wstring	convert_filter();
	void			convert_slide_transition_filter(oox::pptx_conversion_context& Context);


};
CP_REGISTER_OFFICE_ELEMENT2(anim_transitionFilter);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class anim_audio : public office_element_impl<anim_audio>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeAnimAudio;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context & Context);

	odf_types::common_anim_smil_attlist		common_attlist_;
	odf_types::anim_audio_attlist			audio_attlist_;
private:
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );


};
CP_REGISTER_OFFICE_ELEMENT2(anim_audio);
//anim:command

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// anim:set

class anim_set : public office_element_impl<anim_set>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeAnimSet;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context& Context);

	odf_types::common_anim_smil_attlist		common_attlist_;
	odf_types::anim_set_attlist				set_attlist_;
private:
	virtual void add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_attributes(const xml::attributes_wc_ptr& Attributes);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_set);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// anim:animateMotion
class anim_animate_motion : public office_element_impl<anim_animate_motion>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeAnimAnimateMotion;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context& Context);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_motion_attlist		animate_motion_attlist_;
private:
	virtual void add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_attributes(const xml::attributes_wc_ptr& Attributes);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_motion);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// anim:animateColor
class anim_animate_color : public office_element_impl<anim_animate_color>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeAnimAnimateColor;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context& Context);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_color_attlist		animate_color_attlist_;
private:
	virtual void add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_attributes(const xml::attributes_wc_ptr& Attributes);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_color);

//////////////////////////////////////////////////////////////////////////
// anim:animate
class anim_animate : public office_element_impl<anim_animate>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeAnimAnimate;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context& Context);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_attlist				animate_attlist_;

private:
	virtual void add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_attributes(const xml::attributes_wc_ptr& Attributes);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate);

//////////////////////////////////////////////////////////////////////////
// anim:animateTransform
class anim_animate_transform : public office_element_impl<anim_animate_transform>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeAnimAnimateTransform;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void pptx_convert(oox::pptx_conversion_context& Context);

	odf_types::common_anim_smil_attlist				common_attlist_;
	odf_types::anim_animate_transform_attlist		animate_transform_attlist_;

private:
	virtual void add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_attributes(const xml::attributes_wc_ptr& Attributes);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_transform);
}
}
