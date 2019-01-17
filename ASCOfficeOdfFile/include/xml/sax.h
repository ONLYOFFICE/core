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

