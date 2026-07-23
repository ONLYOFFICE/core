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


#include <string>
#include <exception>
#include <iosfwd>

#include "../CPSharedPtr.h"
#include "nodetype.h"

namespace cpdoccore {
namespace xml {

class sax;
typedef _CP_PTR(sax) sax_ptr;

class sax
{
public:

    struct error : virtual public std::exception
    {
    public:
        enum Code {
            no = 0,
            openFile,
            createReader,
            internalErr
        };

    public:
        error(Code _code) : code_(_code){};
        Code code() const { return code_; }

    private:
        Code code_;
    };

public:
    //virtual std::wstring baseURI() = 0;
    virtual unsigned int depth() = 0;

    // <p:abc/>, returns "abc".
    virtual std::wstring nodeLocalName() = 0;
    
    // <p:abc />, returns "p:abc".
    virtual std::wstring nodeQualifiedName() = 0;

    // <xyz:abc xmlns:xyz="u://1" />, it returns "xyz".
    virtual std::wstring namespacePrefix() = 0;

    //// Returns "u://1" for the element <xyz:abc xmlns:xyz="u://1" />.
    //virtual std::wstring namespaceUri() = 0;

    virtual NodeType		nodeType() = 0;
	virtual std::wstring	value() = 0;
	
    virtual unsigned int	attrCount() = 0;
	//virtual std::wstring	value() = 0;
	virtual bool			attrDefault() = 0;

    // <element attribute="123"></element> — false
    // <element attribute="123"/> - true
    virtual bool isEmptyElement() = 0;
    
    virtual bool moveToAttrFirst() = 0;
    virtual bool moveToAttrNext() = 0;
    virtual bool moveToAttrOwner() = 0;

    virtual NodeType next(int Depth) = 0;

    virtual ~sax() = 0;

};


inline sax::~sax() {};

sax_ptr create_sax(const wchar_t * FileName);

} // namespace xml
} // namespace cpdoccore

