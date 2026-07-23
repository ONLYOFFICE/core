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

#include "math_elementaries.h"

#include <xml/xmlchar.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mstack::ns	= L"math";
const wchar_t * math_mstack::name	= L"mstack";
//----------------------------------------------------------------------------------------------------
void math_mstack::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mstack::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstack::oox_convert(oox::math_context & Context)
{//0* elements

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_msrow::ns		= L"math";
const wchar_t * math_msrow::name	= L"msrow";
//----------------------------------------------------------------------------------------------------
void math_msrow::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msrow::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msrow::oox_convert(oox::math_context & Context)
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_msline::ns	= L"math";
const wchar_t * math_msline::name	= L"msline";
//----------------------------------------------------------------------------------------------------
void math_msline::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msline::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msline::oox_convert(oox::math_context & Context)
{

}

//---------------------------------------------------------------
const wchar_t * math_msgroup::ns	= L"math";
const wchar_t * math_msgroup::name	= L"msgroup";
//----------------------------------------------------------------------------------------------------
void math_msgroup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msgroup::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msgroup::oox_convert(oox::math_context & Context)
{//0* elements

}

//---------------------------------------------------------------
const wchar_t * math_mlongdiv::ns	= L"math";
const wchar_t * math_mlongdiv::name	= L"mlongdiv";
//----------------------------------------------------------------------------------------------------
void math_mlongdiv::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mlongdiv::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mlongdiv::oox_convert(oox::math_context & Context)
{//3* elements

}

//---------------------------------------------------------------
const wchar_t * math_mscarry::ns	= L"math";
const wchar_t * math_mscarry::name	= L"mscarry";
//----------------------------------------------------------------------------------------------------
void math_mscarry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mscarry::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mscarry::oox_convert(oox::math_context & Context)
{

}

//---------------------------------------------------------------
const wchar_t * math_mscarries::ns		= L"math";
const wchar_t * math_mscarries::name	= L"mscarries";
//----------------------------------------------------------------------------------------------------
void math_mscarries::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mscarries::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mscarries::oox_convert(oox::math_context & Context)
{//0* elements

}
//---------------------------------------------------------------

}
}
