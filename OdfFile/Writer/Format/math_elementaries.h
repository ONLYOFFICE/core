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

#include "math_elements.h"

namespace cpdoccore { 
namespace odf_writer {

class math_mstack : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMStack;

    


private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mstack);
//CP_REGISTER_OFFICE_ELEMENT3(math_mstack);

//--------------------------------------------------------------------

class math_msrow : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSRow;

    

private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msrow);
//CP_REGISTER_OFFICE_ELEMENT3(math_msrow);

//--------------------------------------------------------------------

class math_msline : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSLine;

	

private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

	office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msline);
//CP_REGISTER_OFFICE_ELEMENT3(math_msline);

//--------------------------------------------------------------------

class math_mlongdiv : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSLongDiv;

    

private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mlongdiv);
//CP_REGISTER_OFFICE_ELEMENT3(math_mlongdiv);

//--------------------------------------------------------------------

class math_mscarries : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSCarries;

    

private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mscarries);
//CP_REGISTER_OFFICE_ELEMENT3(math_mscarries);
//--------------------------------------------------------------------

class math_msgroup : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSGroup;

    

private:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msgroup);
//CP_REGISTER_OFFICE_ELEMENT3(math_msgroup);

//--------------------------------------------------------------------

class math_mscarry : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeMSCarry;

    

private:
    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child_element);
	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mscarry);
//CP_REGISTER_OFFICE_ELEMENT3(math_mscarry);

//--------------------------------------------------------------------
}
}
