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

#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/smil_transitiontype.h"

namespace cpdoccore { 
namespace odf_writer {

class anim_par : public office_element_impl<anim_par>//Параллельные анимации
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeAnimPar;
    

	odf_types::common_anim_smil_attlist		attlist_;
  	office_element_ptr						anim_par_;
	office_element_ptr_array				anim_seq_array_;
	office_element_ptr_array				content_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);
};
CP_REGISTER_OFFICE_ELEMENT2(anim_par);

class anim_seq : public office_element_impl<anim_seq>//Последовательные анимации
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
class anim_audio_attlist
{
public:
	void serialize(CP_ATTR_NODE);
	
	_CP_OPT(std::wstring)			xlink_href_;
	_CP_OPT(std::wstring)			anim_audio_level_; 
};

class anim_transition_filter_attlist
{
public:
	void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)						smil_subtype_; 
	_CP_OPT(odf_types::smil_transition_type)	smil_type_;
	_CP_OPT(std::wstring)						smil_mode_;
	_CP_OPT(odf_types::color)					smil_fadeColor_;
};


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
	odf_types::common_anim_smil_attlist		common_attlist_;
	anim_transition_filter_attlist			filter_attlist_;
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
	anim_audio_attlist						audio_attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(anim_audio);
//anim:command

}
}
