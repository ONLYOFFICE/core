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
