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

#include "office_document.h"

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>

namespace cpdoccore { 

namespace odf_writer {

// manifest:encryption-data
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_encryption_data::ns = L"manifest";
const wchar_t * manifest_encryption_data::name = L"encryption-data";

void manifest_encryption_data::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"manifest", L"algorithm")
	{
        CP_CREATE_ELEMENT(algorithm_);
	}
	else if CP_CHECK_NAME(L"manifest", L"key-derivation")
	{
        CP_CREATE_ELEMENT(key_derivation_);
	}
	else if CP_CHECK_NAME(L"manifest", L"start-key-generation")
	{
        CP_CREATE_ELEMENT(start_key_generation_);
	}
}
void manifest_encryption_data::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeManifestAlgorithm)
	{
        algorithm_ = child_element;
	}
	else if (type_ == typeManifestKeyDerivation)
	{
        key_derivation_ = child_element;
	}
	else if (type_ == typeManifestStartKeyGeneration)
	{
        start_key_generation_ = child_element;
	}
}
void manifest_encryption_data::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR(L"manifest:checksum-type", checksum_type_); 
			CP_XML_ATTR(L"manifest:checksum", checksum_); 

			if (algorithm_)				algorithm_->serialize(CP_XML_STREAM());
			if (key_derivation_)		key_derivation_->serialize(CP_XML_STREAM());
			if (start_key_generation_)	start_key_generation_->serialize(CP_XML_STREAM());
		}
	}
}
// manifest:algorithm
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_algorithm::ns = L"manifest";
const wchar_t * manifest_algorithm::name = L"algorithm";

void manifest_algorithm::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {      
			CP_XML_ATTR(L"manifest:algorithm-name", algorithm_name_);
			CP_XML_ATTR(L"manifest:initialisation-vector", initialisation_vector_);
		}
	}

}
// manifest:key-derivation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_key_derivation::ns = L"manifest";
const wchar_t * manifest_key_derivation::name = L"key-derivation";

void manifest_key_derivation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"manifest:key-derivation-name", L"PBKDF2");
			CP_XML_ATTR(L"manifest:key-size", key_size_);
			CP_XML_ATTR(L"manifest:iteration-count", iteration_count_);
			CP_XML_ATTR(L"manifest:salt", salt_);	
		}
	}
}
// manifest:start-key-generation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_start_key_generation::ns = L"manifest";
const wchar_t * manifest_start_key_generation::name = L"start-key-generation";

void manifest_start_key_generation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR(L"manifest:start-key-generation-name", start_key_generation_name_);
			CP_XML_ATTR(L"manifest:key-size", key_size_);
		}
	}
}
}
}

