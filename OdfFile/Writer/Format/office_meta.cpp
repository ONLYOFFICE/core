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

#include "office_meta.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf_writer {

//-----------------------------------------------------------------------------------
const wchar_t * meta_common::ns = L"meta";
const wchar_t * meta_common::name = L"common";
void meta_common::add_text(const std::wstring & text)
{
	content_ = text;
}
const wchar_t * meta_generator::ns = L"meta";
const wchar_t * meta_generator::name = L"generator";
void meta_generator::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * meta_initial_creator::ns = L"meta";
const wchar_t * meta_initial_creator::name = L"initial-creator";
void meta_initial_creator::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * meta_creation_date::ns = L"meta";
const wchar_t * meta_creation_date::name = L"creation-date";
void meta_creation_date::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << content_;
		}
	}
}
const wchar_t * meta_template::ns = L"meta";
const wchar_t * meta_template::name = L"template";
void meta_template::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * meta_keyword::ns = L"meta";
const wchar_t * meta_keyword::name = L"keyword";
void meta_keyword::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * meta_editing_cycles::ns = L"meta";
const wchar_t * meta_editing_cycles::name = L"editing-cycles";
void meta_editing_cycles::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << content_;
		}
	}
}
const wchar_t * dc_creator::ns = L"dc";
const wchar_t * dc_creator::name = L"creator";
void dc_creator::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * dc_description::ns = L"dc";
const wchar_t * dc_description::name = L"description";
void dc_description::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * dc_language::ns = L"dc";
const wchar_t * dc_language::name = L"language";
void dc_language::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << content_;
		}
	}
}
const wchar_t * dc_date::ns = L"dc";
const wchar_t * dc_date::name = L"date";
void dc_date::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << content_;
		}
	}
}
const wchar_t * dc_title::ns = L"dc";
const wchar_t * dc_title::name = L"title";
void dc_title::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
const wchar_t * dc_subject::ns = L"dc";
const wchar_t * dc_subject::name = L"subject";
void dc_subject::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
//------------------------------------------------------------------------------------
const wchar_t * meta_document_statistic::ns = L"meta";
const wchar_t * meta_document_statistic::name = L"document-statistic";

void meta_document_statistic::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR_OPT(L"meta:table-count", table_count_);
			CP_XML_ATTR_OPT(L"meta:image-count", image_count_);
			CP_XML_ATTR_OPT(L"meta:object-count", object_count_);
			CP_XML_ATTR_OPT(L"meta:page-count", page_count_);
			CP_XML_ATTR_OPT(L"meta:paragraph-count", paragraph_count_);
			CP_XML_ATTR_OPT(L"meta:word-count", word_count_);
			CP_XML_ATTR_OPT(L"meta:character-count", character_count_);
			CP_XML_ATTR_OPT(L"meta:ole-object-count", ole_object_count_);
			CP_XML_ATTR_OPT(L"meta:frame-count", frame_count_);
			CP_XML_ATTR_OPT(L"meta:row-count", row_count_);
			CP_XML_ATTR_OPT(L"meta:cell-count", cell_count_);
			CP_XML_ATTR_OPT(L"meta:non-whitespace-character-count", non_whitespace_character_count_);
			CP_XML_ATTR_OPT(L"meta:sentence-count", sentence_count_);
			CP_XML_ATTR_OPT(L"meta:syllable-count", syllable_count_);

		}
	}
}
// meta_user_defined
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * meta_user_defined::ns = L"meta";
const wchar_t * meta_user_defined::name = L"user-defined";
void meta_user_defined::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR(L"meta:name", meta_name_);
			CP_XML_ATTR(L"meta:value-type", meta_value_type_);

			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}

}
}
