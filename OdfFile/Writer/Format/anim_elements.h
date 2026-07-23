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

#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/animation_attlists.h"

namespace cpdoccore { 
namespace odf_writer {

class anim_par : public office_element_impl<anim_par>// Parallel animations
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeAnimPar;
    

	odf_types::common_anim_smil_attlist		attlist_;
	odf_types::anim_par_attlist				par_attlist_;
  	office_element_ptr_array				anim_par_;
	office_element_ptr_array				anim_seq_array_;
	office_element_ptr_array				content_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_par);

class anim_seq : public office_element_impl<anim_seq>// Sequential animations
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeAnimSeq;    

	odf_types::common_anim_smil_attlist		attlist_;
	office_element_ptr_array				anim_par_array_;
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_seq);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:iterate 

class anim_transitionFilter : public office_element_impl<anim_transitionFilter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeAnimTransitionFilter;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
///////////////////////////////////////////////////////////	
	odf_types::common_anim_smil_attlist				common_attlist_;
	odf_types::anim_transition_filter_attlist		filter_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_transitionFilter);

class anim_audio : public office_element_impl<anim_audio>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeAnimAudio;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child){}

	virtual void serialize(std::wostream & strm);
///////////////////////////////////////////////////////////	
	odf_types::common_anim_smil_attlist		common_attlist_;
	odf_types::anim_audio_attlist			audio_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_audio);
//anim:command

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:set
class anim_set : public office_element_impl<anim_set>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;

	static const ElementType type = typeAnimSet;

	virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_child_element(const office_element_ptr& child) {}

	virtual void serialize(std::wostream& strm);
///////////////////////////////////////////////////////////	
	odf_types::common_anim_smil_attlist		common_attlist_;
	odf_types::anim_set_attlist				set_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_set);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//anim:animate
class anim_animate : public office_element_impl<anim_animate>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;

	static const ElementType type = typeAnimAnimate;

	virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_child_element(const office_element_ptr& child) {}

	virtual void serialize(std::wostream& strm);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_attlist				animate_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// anim:animateMotion
class anim_animate_motion : public office_element_impl<anim_animate_motion>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;

	static const ElementType type = typeAnimAnimateMotion;

	virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_child_element(const office_element_ptr& child) {}

	virtual void serialize(std::wostream& strm);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_motion_attlist		animate_motion_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_motion);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// anim:animateColor
class anim_animate_color: public office_element_impl<anim_animate_motion>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;

	static const ElementType type = typeAnimAnimateColor;

	virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_child_element(const office_element_ptr& child) {}

	virtual void serialize(std::wostream& strm);

	odf_types::common_anim_smil_attlist			common_attlist_;
	odf_types::anim_animate_color_attlist		color_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_color);

//////////////////////////////////////////////////////////////////////////
// anim:animateTransform
class anim_animate_transform : public office_element_impl<anim_animate_transform>
{
public:
	static const wchar_t* ns;
	static const wchar_t* name;

	static const ElementType type = typeAnimAnimateTransform;

	virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
	virtual void add_child_element(const office_element_ptr& child) {}

	virtual void serialize(std::wostream& strm);

	odf_types::common_anim_smil_attlist				common_attlist_;
	odf_types::anim_animate_transform_attlist		animate_transform_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_animate_transform);

}
}
