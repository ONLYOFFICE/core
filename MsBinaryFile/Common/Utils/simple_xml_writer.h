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
#include <iostream>
#include <sstream>
#include <stack>
#include <cassert>
#include <boost/optional/optional_io.hpp>

namespace xlscore
{


namespace xml 
{

namespace writer 
{
    template <class T> struct chars { };
    
    template <>
    struct chars<char>
    {
        static const char eq = '=';
        static const char quote = '\"';
        static const char space = ' ';
        static const char left_brocket = '<';
        static const char right_brocket = '>';
        static const char slash = '/';
        static const char colon = ':';
        static const char amp = '&';
        static const char apos = '\'';
        static const char a = '\x0a';

        static const char * cdata_open() { return "<![CDATA["; }
        static const char * cdata_close() { return "]]>"; }
        static const char * amp_str(){ return "&amp;"; }
        static const char * left_brocket_str() { return "&lt;"; }
        static const char * right_brocket_str() { return "&gt;"; }
        static const char * apos_str() { return "&apos;"; }
        static const char * quote_str() { return "&quot;"; }
		static const char * a_str() { return "&#10;"; } 
    };

    template <>
    struct chars<wchar_t>
    {
        static const wchar_t eq = L'=';
        static const wchar_t quote = L'\"';
        static const wchar_t space = L' ';
        static const wchar_t left_brocket = L'<';
        static const wchar_t right_brocket = L'>';
        static const wchar_t slash = L'/';
        static const wchar_t colon = L':';
        static const wchar_t amp = L'&';
        static const wchar_t apos = L'\'';
        static const wchar_t a = L'\x0a';

		static const wchar_t * cdata_open() { return L"<![CDATA["; }
        static const wchar_t * cdata_close() { return L"]]>"; }
        static const wchar_t * amp_str(){ return L"&amp;"; }
        static const wchar_t * left_brocket_str() { return L"&lt;"; }
        static const wchar_t * right_brocket_str() { return L"&gt;"; }
        static const wchar_t * apos_str() { return L"&apos;"; }
        static const wchar_t * quote_str() { return L"&quot;"; }
 		static const wchar_t * a_str() { return L"&#10;"; } 
   };

    template <class V>
    class element;

    //
    //  xml::writer class
    //
    template <class T>
    class writer
    {
    public:
        // writer must be bound to an ostream
        writer(std::basic_ostream<T>& os, bool need_header = false) : level_(0), os_(os), need_header_(need_header) {}
        ~writer(void) { assert(elements_.empty()); }

    private:
        size_t level_;
        std::basic_ostream<T>& os_;      // output stream
        bool need_header_;               // have we written an XML header yet?
        std::stack<element<T>*> elements_;  // stack of open element tags
        

        // write XML header, if necessary
        writer& header() 
        {
            if (need_header_) 
            {
                // TODO
                //os_ << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
                need_header_ = false;
            }
            return *this;
        }

        // write a single character to the output stream
        writer& putc(T c) 
        {
            os_.put(c);
            return *this;
        }

        // write a string to the output stream
        writer& puts(const T* str) {
            os_ << str;
            return *this;
        }

        template <class V>
        friend class element;
    };

    //
    //  xml::element class
    //
    template <class T>
    class element
    {
    public:
        // create a new element tag, bound to an xml::writer
        element(writer<T>& wr, std::basic_string<T, std::char_traits<T>, std::allocator<T> > const & name) : wr_(wr), name_(name)
        {
            wr_.level_++;
            check_parent();
            wr_.header().putc('<').puts(name_.c_str());
            tagopen_ = true;
            wr_.elements_.push(this);
        }

        // close the current element tag
        ~element() 
        {
            if (!wr_.elements_.empty() && wr_.elements_.top() == this) 
            {
                wr_.elements_.pop();
                if (tagopen_)
                    wr_.putc(chars<T>::slash)
                    .putc(chars<T>::right_brocket);
                else
                    wr_.putc(chars<T>::left_brocket)
                    .putc(chars<T>::slash)
                    .puts(name_.c_str())
                    .putc(chars<T>::right_brocket);
            }
            wr_.level_--;
        }

        // write an attribute for the current element
        element& attr(const T* name, const T* value) 
        {
            assert(name != 0);
            assert(value != 0);
            assert(tagopen_);
            wr_.putc(chars<T>::space)
                .puts(name)
                .putc(chars<T>::eq)
                .putc(chars<T>::quote);
            qputs(value);
            wr_.putc(chars<T>::quote);
            return *this;
        }

        // attr() overload for std::string type
        element& attr(const T* name, const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& value) 
        { 
            return attr(name, value.c_str()); 
        }

        // attr() function template for all streamable types
        template <class V>
        element& attr(const T* name, V value) 
        {
            std::basic_stringstream<T, std::char_traits<T>,	std::allocator<T> > ss;
            ss << value;
            attr(name, ss.str());
            return *this;
        }

        // write text content for the current element
        element& contents(const T* str) 
        {
            assert(str != 0);
            check_parent();
            qputs(str);
            return *this;
        }

        // contents() overload for std::string type
        element& contents(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str) 
        { 
            return contents(str.c_str()); 
        }

        // contents() function template for all streamable types
        template <class V>
        element& contents(V value) 
        {
            std::basic_stringstream<T, std::char_traits<T>,	std::allocator<T> > ss;
            ss << value;
            contents(ss.str());
            return *this;
        }

        std::basic_ostream<T>& stream()
        {
            check_parent();
            return wr_.os_;            
        }

        // write CDATA section
        element& cdata(const T * str) 
        {
            assert(str != 0);
            check_parent();
            wr_.puts(chars<T>::cdata_open());
            wr_.puts(str);
            wr_.puts(chars<T>::cdata_close());
            return *this;
        }

        // cdata() overload for std::string type
        element& cdata(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str) 
        { 
            return cdata(str.c_str()); 
        }

    private:
        writer<T>& wr_;        // bound XML writer
        //const T* name_;     // name of current element
        const std::basic_string<T, std::char_traits<T>, std::allocator<T> > name_;
        bool tagopen_;      // is the element tag for this element still open?

        // write a string quoting characters which have meaning in xml
        element& qputs(const T* str) 
        {
            for (; *str; ++str)
            {
                switch (*str) 
                {
                case chars<T>::amp: 
                    wr_.puts(chars<T>::amp_str()); break;

                case chars<T>::left_brocket:
                    wr_.puts(chars<T>::left_brocket_str()); break;

                case chars<T>::right_brocket: 
                    wr_.puts(chars<T>::right_brocket_str()); break;

                case chars<T>::apos: 
                    wr_.puts(chars<T>::apos_str()); break;

                case chars<T>::quote: 
                    wr_.puts(chars<T>::quote_str()); break;

                 case chars<T>::a: 
                    wr_.puts(chars<T>::a_str()); break;
               default: 
                    wr_.putc(*str); break;
                }
            }
            return *this;
        }

        // check to see if we have a parent tag which needs to be closed
        void check_parent() 
        {
            if (!wr_.elements_.empty() && wr_.elements_.top()->tagopen_) 
            {
                wr_.putc(chars<T>::right_brocket);
                wr_.elements_.top()->tagopen_ = false;
            }
        }
    };

}

}

}
typedef xlscore::xml::writer::writer<wchar_t> xml_writer;
typedef xlscore::xml::writer::element<wchar_t> xml_element;

#define CP_XML_WRITER(STRM) if (bool _b_ = false) {} else for (xml_writer _xml_wr_((STRM));!_b_;_b_=true)
#define CP_XML_NODE(NAME) if (bool _b_ = false) {} else for (xml_element _xml_node_(_xml_wr_, (NAME));!_b_;_b_=true)
#define CP_XML_ATTR(NAME, VAL) _xml_node_.attr((NAME),(VAL))
#define CP_XML_CONTENT(VAL) _xml_node_.contents((VAL))
#define CP_XML_STREAM() _xml_node_.stream()

#define CP_GET_XML_NODE() _xml_node_

//#define CP_ATTR_NODE xml_element & _xml_node_
#define CP_ATTR_NODE xlscore::xml::writer::element<wchar_t> & _xml_node_


#define CP_XML_ATTR_OPT(NAME, VAL) if (VAL) CP_XML_ATTR(NAME, (*VAL))
#define CP_XML_ATTR_STR(NAME, VAL) if (!VAL.empty()) CP_XML_ATTR((NAME), (VAL))
#define CP_XML_ATTR_NULLABLE(NAME, VAL) if (VAL.IsInit()) CP_XML_ATTR(NAME, (*VAL))
#define CP_XML_ATTR_NULLABLE2(NAME, VAL) if (VAL.IsInit()) CP_XML_ATTR(NAME, (VAL->ToString()))

#define CP_XML_NODE_SIMPLE() std::wstring NS_NAME = std::wstring(ns) + std::wstring(L":") + std::wstring(name); CP_XML_NODE(NS_NAME)

