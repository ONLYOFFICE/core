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

#include <iosfwd>

#include <iosfwd>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/officevaluetype.h"

namespace cpdoccore { 
namespace odf_writer {

//  base
class meta_common : public office_element_impl<meta_common>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaCommon;

	std::wstring content_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name) {}
	virtual void add_child_element(const office_element_ptr & child_element) {}
	virtual void add_text(const std::wstring & Text);

	virtual void serialize(std::wostream & _Wostream) = 0;
};
//  meta:initial-creator
class meta_initial_creator : public meta_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const ElementType type = typeOfficeMetaInitialCreator;
	
	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_initial_creator);
//  meta:keyword
class meta_keyword : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaKeyword;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_keyword);
//  meta:creation-date
class meta_creation_date : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaCreationDate;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_creation_date);
//  meta:generator
class meta_generator : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaGenerator;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_generator);
//  meta:template
class meta_template : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaTemplate;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_template);
//  meta:editing-cycles
class meta_editing_cycles : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaEditingCycles;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_editing_cycles);
//--------------------------------------------------------------------------
//  dc:creator
class dc_creator : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcCreator;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_creator);
//  dc:description
class dc_description : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcDescription;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_description);
//  dc:language
class dc_language : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcLanguage;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_language);
//  dc:date
class dc_date : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcDate;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_date);
//  dc:title
class dc_title : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcTitle;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_title);
//  dc:subject
class dc_subject : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcSubject;

	virtual ElementType get_type() const
	{
		return type;
	}
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(dc_subject);
//-----------------------------------------------------------------------------
//  meta:document-statistic
class meta_document_statistic : public office_element_impl<meta_document_statistic>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

	static const ElementType type = typeOfficeMetaDocumentStatistic;

	_CP_OPT(int)	image_count_;
	_CP_OPT(int)	object_count_;
	_CP_OPT(int)	page_count_;
	_CP_OPT(int)	paragraph_count_;
	_CP_OPT(int)	word_count_;
	_CP_OPT(int)	character_count_;
	_CP_OPT(int)	non_whitespace_character_count_;
	_CP_OPT(int)	draw_count_;
	_CP_OPT(int)	ole_object_count_;
	_CP_OPT(int)	frame_count_;
	_CP_OPT(int)	table_count_;
	_CP_OPT(int)	row_count_;
	_CP_OPT(int)	cell_count_;
	_CP_OPT(int)	sentence_count_;
	_CP_OPT(int)	syllable_count_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name) {}
	virtual void add_child_element(const office_element_ptr & child_element) {}	
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_document_statistic);

//  meta:user-defined
class meta_user_defined : public office_element_impl<meta_user_defined>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeOfficeMetaUserDefined;

	std::wstring meta_name_;
	std::wstring content_;
	_CP_OPT(odf_types::office_value_type) meta_value_type_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name) {}
	virtual void add_child_element(const office_element_ptr & child_element) {}	
	virtual void serialize(std::wostream & _Wostream);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_user_defined);


}
}