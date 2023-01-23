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
/*

Схема работы с visitor:

для классов, которые могут быть посещены visitor-ом:
1. наследуем от base_visitable
2. объявляем внутри макрос CPDOCCORE_DEFINE_VISITABLE()

class example_visitable_element: public base_visitable 
{
// ... 
public:
    CPDOCCORE_DEFINE_VISITABLE();
// ... 
};

Для создания гостя:
1. наследуем гостя от base_visitor
2. наследуем гостя(визитор) от const_visitor<visitable_element1>/visitor<visitable_element1>,
                        const_visitor<visitable_element2>/visitor<visitable_element2>, и т.д.
   для каждого из элементов, котоыре может посетить визитор
3. объявляем для каждого из элементов метод
    virtual void visit(const visitable_element1& val);
    virtual void visit(const visitable_element2& val);
    и т.д.
    (в случае неконстатности - убираем const)


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

Для организации обхода:
1. Создаем визитор
2. Выполняем accept для корневого элемента

office_element * root;
/// ... 
table_round tableRoundVisitor;
root->accept(tableRoundVisitor);

Внимание! Визитор САМ определяет правила обхода внутренних вершин, т.е. внутри каждой функции посещения требуется написать например
    
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