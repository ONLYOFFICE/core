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

#include "office_meta.h"

#include <xml/xmlchar.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

// office:meta
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_meta::ns = L"office";
const wchar_t * office_meta::name = L"meta";

void office_meta::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_meta::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (Ns==L"meta" && Name == L"generator")
	{
		CP_CREATE_ELEMENT(meta_generator_);
	}
	else if (Ns==L"meta" && Name == L"document-statistic")
	{
		CP_CREATE_ELEMENT(meta_document_statistic_);
	}
	else if (Ns==L"meta" && Name == L"user-defined")
	{
		CP_CREATE_ELEMENT(meta_user_defined_);
	}
	else if (Ns == L"meta" && Name == L"creation-date")
	{
		CP_CREATE_ELEMENT(meta_creation_date_);
	}
	else if (Ns == L"meta" && Name == L"initial-creator")
	{
		CP_CREATE_ELEMENT(meta_initial_creator_);
	}
	else if (Ns == L"meta" && Name == L"template")
	{
		CP_CREATE_ELEMENT(meta_template_);
	}
	else if (Ns == L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns == L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
	else if (Ns == L"dc" && Name == L"description")
	{
		CP_CREATE_ELEMENT(dc_description_);
	}
	else if (Ns == L"dc" && Name == L"language")
	{
		CP_CREATE_ELEMENT(dc_language_);
	}
	else if (Ns == L"dc" && Name == L"title")
	{
		CP_CREATE_ELEMENT(dc_title_);
	}
	else if (Ns == L"dc" && Name == L"subject")
	{
		CP_CREATE_ELEMENT(dc_subject_);
	}
	else
	{
	}
}
//-----------------------------------------------------------------------------------
const wchar_t * meta_common::ns = L"meta";
const wchar_t * meta_common::name = L"common";
void meta_common::add_text(const std::wstring & text)
{
	content_ = text;
}
const wchar_t * meta_generator::ns = L"meta";
const wchar_t * meta_generator::name = L"generator";

const wchar_t * meta_initial_creator::ns = L"meta";
const wchar_t * meta_initial_creator::name = L"initial-creator";

const wchar_t * meta_creation_date::ns = L"meta";
const wchar_t * meta_creation_date::name = L"creation-date";

const wchar_t * meta_template::ns = L"meta";
const wchar_t * meta_template::name = L"template";

const wchar_t * meta_keyword::ns = L"meta";
const wchar_t * meta_keyword::name = L"keyword";

const wchar_t * meta_editing_cycles::ns = L"meta";
const wchar_t * meta_editing_cycles::name = L"editing-cycles";

const wchar_t * dc_creator::ns = L"dc";
const wchar_t * dc_creator::name = L"creator";

const wchar_t * dc_description::ns = L"dc";
const wchar_t * dc_description::name = L"description";

const wchar_t * dc_language::ns = L"dc";
const wchar_t * dc_language::name = L"language";

const wchar_t * dc_date::ns = L"dc";
const wchar_t * dc_date::name = L"date";

const wchar_t * dc_title::ns = L"dc";
const wchar_t * dc_title::name = L"title";

const wchar_t * dc_subject::ns = L"dc";
const wchar_t * dc_subject::name = L"subject";
//------------------------------------------------------------------------------------
const wchar_t * meta_document_statistic::ns = L"meta";
const wchar_t * meta_document_statistic::name = L"document-statistic";

void meta_document_statistic::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"meta:table-count",		table_count_);
	CP_APPLY_ATTR(L"meta:image-count",		image_count_);
	CP_APPLY_ATTR(L"meta:object-count",		object_count_);
	CP_APPLY_ATTR(L"meta:page-count",		page_count_);
	CP_APPLY_ATTR(L"meta:paragraph-count",	paragraph_count_);
	CP_APPLY_ATTR(L"meta:word-count",		word_count_);
	CP_APPLY_ATTR(L"meta:character-count",	character_count_);
	CP_APPLY_ATTR(L"meta:ole-object-count", ole_object_count_);
	CP_APPLY_ATTR(L"meta:frame-count", frame_count_);
	CP_APPLY_ATTR(L"meta:row-count", row_count_);
	CP_APPLY_ATTR(L"meta:cell-count", cell_count_);
	CP_APPLY_ATTR(L"meta:non-whitespace-character-count", non_whitespace_character_count_);
	CP_APPLY_ATTR(L"meta:sentence-count", sentence_count_);
	CP_APPLY_ATTR(L"meta:syllable-count", syllable_count_);
}

// meta_user_defined
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * meta_user_defined::ns = L"meta";
const wchar_t * meta_user_defined::name = L"user-defined";

void meta_user_defined::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"meta:name", meta_name_, std::wstring(L""));
}

void meta_user_defined::add_text(const std::wstring & text)
{
    content_ = text;
}
}
}
