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
#ifndef _CPDOCCORE_VISITOR_H_INCLUDED_
#define _CPDOCCORE_VISITOR_H_INCLUDED_

/*

Visitor pattern usage:

For classes that can be visited by a visitor:
1. inherit from base_visitable
2. declare the CPDOCCORE_DEFINE_VISITABLE() macro inside

class example_visitable_element: public base_visitable
{
// ...
public:
    CPDOCCORE_DEFINE_VISITABLE();
// ...
};

To create a visitor:
1. inherit visitor from base_visitor
2. inherit visitor from const_visitor<visitable_element1>/visitor<visitable_element1>,
                        const_visitor<visitable_element2>/visitor<visitable_element2>, etc.
   for each element that the visitor can visit
3. declare a method for each element
    virtual void visit(const visitable_element1& val);
    virtual void visit(const visitable_element2& val);
    etc.
    (in case of non-const - remove const)


class table_round : public base_visitor,
    public const_visitor<office_body>,
    public const_visitor<office_spreadsheet>,
    public const_visitor<table_table>
{
    virtual void visit(const office_body& val)
    {
    // ...
    }

    virtual void visit(const office_spreadsheet& val)
    {
    // ...
    }

    virtual void visit(const table_table& val)
    {
    // ...
    }
};

To organize traversal:
1. Create a visitor
2. Execute accept for the root element

office_element * root;
/// ... 
table_round tableRoundVisitor;
root->accept(tableRoundVisitor);

Note! The visitor ITSELF determines the rules for traversing internal nodes, i.e. inside each visit function you need to write for example
    
    virtual void visit(const office_body& val)
    {
        val.content_->accept(*this);        
    }

*/

namespace cpdoccore { 
    
    class base_visitor
    {
    public:
        virtual ~base_visitor() {}    
        virtual void on_not_impl(std::string const & message) = 0;
    };

    template <class T>
    class visitor
    {
    public:
        virtual void visit(T&) = 0;
       // virtual void visit(T const& t) = 0;
    };

    template <class T>
    class const_visitor
    {
    public:
        virtual void visit(T const& t) = 0;
    };


    class base_visitable
    {
    public:
        virtual ~base_visitable() {};
        virtual void accept(base_visitor &) = 0;
        virtual void accept(base_visitor &) const = 0;
    protected:
        template <class T>
        static void accept_impl(T& visited, base_visitor & guest)
        {
            if (visitor<T> * p = dynamic_cast< visitor<T> * >(&guest))
            {
                return p->visit(visited);
            }
            else
            {
                guest.on_not_impl(typeid(visited).name());
            }
        }

        template <class T>
        static void accept_impl(T const& visited, base_visitor & guest)
        {
            if (const_visitor<T> * p = dynamic_cast< const_visitor<T> * >(&guest))
            {
                return p->visit(visited);
            }
            else
            {
                guest.on_not_impl(typeid(visited).name());
            }
        }
    };


#define CPDOCCORE_DEFINE_VISITABLE() \
    virtual void accept(base_visitor& guest)\
    {\
        return accept_impl(*this, guest);\
    }\
    virtual void accept(base_visitor& guest) const\
    {\
        return accept_impl(*this, guest);\
    }

}

#endif
