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

