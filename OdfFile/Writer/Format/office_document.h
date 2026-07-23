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

#include <iosfwd>
#include <vector>
#include <CPOptional.h>
#include <xml/nodetype.h>
#include <xml/simple_xml_writer.h>

#include "office_elements_create.h"

#include "../../DataTypes/bool.h"
#include "../../DataTypes/length.h"

namespace cpdoccore { 
namespace odf_writer {

// manifest:encryption-data
class manifest_encryption_data : public office_element_impl<manifest_encryption_data>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeManifestEncryptionData;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

	std::wstring checksum_;
	std::wstring checksum_type_;

	office_element_ptr algorithm_;
	office_element_ptr key_derivation_;
	office_element_ptr start_key_generation_;
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_encryption_data);

// manifest:algorithm
class manifest_algorithm : public office_element_impl<manifest_algorithm>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeManifestAlgorithm;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	std::wstring algorithm_name_;
	std::wstring initialisation_vector_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_algorithm);

// manifest:key_derivation
class manifest_key_derivation : public office_element_impl<manifest_key_derivation>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeManifestKeyDerivation;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	std::wstring	key_derivation_name_;
	int				key_size_;
	int				iteration_count_;
	std::wstring	salt_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_key_derivation);

// manifest:start-key-generation
class manifest_start_key_generation : public office_element_impl<manifest_start_key_generation>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeManifestStartKeyGeneration;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	std::wstring	start_key_generation_name_;
	int				key_size_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_start_key_generation);

}
}
