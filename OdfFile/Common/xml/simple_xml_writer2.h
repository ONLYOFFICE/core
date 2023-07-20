/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include <vector>
#include <stack>
#include <cassert>

#include "../../../OOXML/Base/Unit.h"



namespace cpdoccore
{

    namespace xml
    {
        template<class A>
        class CBufferXml
        {
        public:
            CBufferXml() : currbuf(0), bufcount(5)
            {
                for (int i = 0; i < bufcount; i++)
                {
                    std::vector<A> mbuf;
                    buffer.push_back(mbuf);
                    buffer[0].reserve(1000);
                }
            }
            ~CBufferXml() { buffer.clear(); }
            size_t Get_currbuf() { return currbuf; }
            std::vector<std::vector<A>> Get_buffer() { return buffer; }
            void push(const A& value) 
            {
                if (buffer.empty() || (buffer[currbuf].size() >= buffer[currbuf].capacity() && currbuf + 1 == bufcount))
                {
                    std::vector<A> mbuf;
                    mbuf.reserve(100);
                    buffer.push_back(mbuf);
                    bufcount++;
                    currbuf++;
                }
                else if (buffer[currbuf].size() >= buffer[currbuf].capacity()) { currbuf++; }
                buffer[currbuf].push_back(value);
            }
            void clear() 
            { 
                for (std::vector<A> b : buffer) b.clear(); 
                currbuf = 0;
            }
            CBufferXml& operator=(CBufferXml other)
            {
                this->clear();
                for (std::vector<A> b : other.Get_buffer())
                {
                    for (A c : b)
                    {
                        this->push(c);
                    }
                }
                return *this;
            }
            CBufferXml operator+(CBufferXml other)
            {
                CBufferXml result;
                for (std::vector<A> b : this->buffer)
                {
                    for (A c : b)
                    {
                        result.push(c);
                    }
                }
                for (std::vector<A> b : other.Get_buffer())
                {
                    for (A c : b)
                    {
                        result.push(c);
                    }
                }
                return result;
            }
            CBufferXml& operator+=(CBufferXml other)
            {
                for (std::vector<A> b : other.Get_buffer())
                {
                    for (A c : b)
                    {
                        this->push(c);
                    }
                }
                return *this;
            }
            friend std::wostream& operator<<(std::wostream& out, CBufferXml buf)
            {
                for (std::vector<A> b : buf.Get_buffer())
                {
                    for (A c : b)
                    {
                        out << c;
                    }
                }
                return out;
            }
            std::wstring str()
            {
                std::wstring strres;
                for (std::vector<A> b : buffer)
                {
                    if (!b.empty())
                    {
                        for (A c : b)
                        {
                            strres += c;
                        }
                    }
                }
                this->clear();
                return strres;
            }
        private:
            std::vector<std::vector<A>> buffer;
            size_t currbuf;
            int bufcount;
        };

        namespace writer
        {

            template <class T> struct chars2 { };

            template <>
            struct chars2<char>
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
                static const char* cdata_open() { return "<![CDATA["; }
                static const char* cdata_close() { return "]]>"; }
                static const char* amp_str() { return "&amp;"; }
                static const char* left_brocket_str() { return "&lt;"; }
                static const char* right_brocket_str() { return "&gt;"; }
                static const char* apos_str() { return "&apos;"; }
                static const char* quote_str() { return "&quot;"; }
            };

            template <>
            struct chars2<wchar_t>
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
                static const wchar_t* cdata_open() { return L"<![CDATA["; }
                static const wchar_t* cdata_close() { return L"]]>"; }
                static const wchar_t* amp_str() { return L"&"/* L"&amp;"*/; }
                static const wchar_t* left_brocket_str() { return L"&lt;"; }
                static const wchar_t* right_brocket_str() { return L"&gt;"; }
                static const wchar_t* apos_str() { return L"&apos;"; }
                static const wchar_t* quote_str() { return L"&quot;"; }
            };

            template <class V>
            class element2;

            //
            //  xml::writer class
            //
            template <class T>
            class writer2
            {
            public:
                // writer must be bound to an ostream
                writer2(CBufferXml<T>& os, bool need_header = false) : level_(0), os_(os), need_header_(need_header) {}
                ~writer2(void) { assert(elements_.empty()); }

            private:
                size_t level_;
                CBufferXml<T>& os_;      // output buffer
                bool need_header_;               // have we written an XML header yet?
                std::stack<element2<T>*> elements_;  // stack of open element tags


                // write XML header, if necessary
                writer2& header()
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
                writer2& putc(T c)
                {
                    os_.push(c);
                    return *this;
                }

                // write a string to the output stream
                writer2& puts(const T* str) {
                    for (int i = 0; i < std::wcslen(str); i++) { os_.push(str[i]); }
                    return *this;
                }

                template <class V>
                friend class element2;
            };

            //
            //  xml::element class
            //
            template <class T>
            class element2
            {
            public:
                // create a new element tag, bound to an xml::writer
                element2(writer2<T>& wr, std::basic_string<T, std::char_traits<T>, std::allocator<T> > const& name) : wr_(wr), name_(name)
                {
                    wr_.level_++;
                    check_parent();
                    wr_.header().putc('<').puts(name_.c_str());
                    tagopen_ = true;
                    wr_.elements_.push(this);
                }

                // close the current element tag
                ~element2()
                {
                    if (!wr_.elements_.empty() && wr_.elements_.top() == this)
                    {
                        wr_.elements_.pop();
                        if (tagopen_)
                            wr_.putc(chars2<T>::slash)
                            .putc(chars2<T>::right_brocket);
                        else
                            wr_.putc(chars2<T>::left_brocket)
                            .putc(chars2<T>::slash)
                            .puts(name_.c_str())
                            .putc(chars2<T>::right_brocket);
                    }
                    wr_.level_--;
                }

                // write an attribute for the current element
                element2& attr(const T* name, const T* value)
                {
                    assert(name != 0);
                    assert(value != 0);
                    assert(tagopen_);
                    wr_.putc(chars2<T>::space)
                        .puts(name)
                        .putc(chars2<T>::eq)
                        .putc(chars2<T>::quote);
                    qputs(value);
                    wr_.putc(chars2<T>::quote);
                    return *this;
                }

                // attr() overload for std::string type
                element2& attr(const T* name, const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& value)
                {
                    return attr(name, value.c_str());
                }

                // attr() function template for all streamable types
                template <class V>
                element2& attr(const T* name, V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    attr(name, ss.str());
                    return *this;
                }

                // write text content for the current element
                element2& contents(const T* str)
                {
                    assert(str != 0);
                    check_parent();
                    qputs(str);
                    return *this;
                }

                // contents() overload for std::string type
                element2& contents(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return contents(str.c_str());
                }

                // contents() function template for all streamable types
                template <class V>
                element2& contents(V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    contents(ss.str());
                    return *this;
                }

                CBufferXml<T> buffer()
                {
                    check_parent();
                    return wr_.os_;
                }

                // write CDATA section
                element2& cdata(const T* str)
                {
                    assert(str != 0);
                    check_parent();
                    wr_.puts(chars2<T>::cdata_open());
                    wr_.puts(str);
                    wr_.puts(chars2<T>::cdata_close());
                    return *this;
                }

                // cdata() overload for std::string type
                element2& cdata(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return cdata(str.c_str());
                }

            private:
                writer2<T>& wr_;        // bound XML writer
                //const T* name_;     // name of current element
                const std::basic_string<T, std::char_traits<T>, std::allocator<T> > name_;
                bool tagopen_;      // is the element tag for this element still open?

                // write a string quoting characters which have meaning in xml
                element2& qputs(const T* str)
                {
                    for (; *str; ++str)
                    {
                        switch (*str)
                        {
                        case chars2<T>::amp:
                            wr_.puts(chars2<T>::amp_str()); break;

                        case chars2<T>::left_brocket:
                            wr_.puts(chars2<T>::left_brocket_str()); break;

                        case chars2<T>::right_brocket:
                            wr_.puts(chars2<T>::right_brocket_str()); break;

                        case chars2<T>::apos:
                            wr_.puts(chars2<T>::apos_str()); break;

                        case chars2<T>::quote:
                            wr_.puts(chars2<T>::quote_str()); break;

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
                        wr_.putc(chars2<T>::right_brocket);
                        wr_.elements_.top()->tagopen_ = false;
                    }
                }
            };

        }

    }

    typedef xml::writer::writer2<wchar_t> xml_writer2;
    typedef xml::writer::element2<wchar_t> xml_element2;

#define CP_XML_WRITER_2(BUFF) if (bool _b_ = false) {} else for (xml_writer2 _xml_wr_2((BUFF));!_b_;_b_=true)
#define CP_XML_NODE_2(NAME) if (bool _b_ = false) {} else for (xml_element2 _xml_node_2(_xml_wr_2, (NAME));!_b_;_b_=true)
#define CP_XML_ATTR_2(NAME, VAL) _xml_node_2.attr((NAME),(VAL))
#define CP_XML_ATTR2_2(NAME, VAL) _xml_node_2.attr((std::wstring(NAME).c_str()),(VAL))
#define CP_XML_CONTENT_2(VAL) _xml_node_2.contents((VAL))
#define CP_XML_BUFFER_2() _xml_node_2.buffer()

#define CP_GET_XML_NODE_2() _xml_node_2

#define CP_ATTR_NODE_2 xml_element2 & _xml_node_2

#define CP_XML_ATTR_ENCODE_STRING_2(NAME, VAL) _xml_node_2.attr((NAME), XmlUtils::EncodeXmlString(VAL))

#define CP_XML_ATTR_OPT_2(NAME, VAL) if (VAL)CP_XML_ATTR_2(NAME, (*VAL))
#define CP_XML_ATTR_OPT_ENCODE_STRING_2(NAME, STR) if (STR)CP_XML_ATTR_2(NAME, XmlUtils::EncodeXmlString(*STR))

#define CP_XML_NODE_SIMPLE_2() std::wstring NS_NAME = std::wstring(ns) + std::wstring(L":") + std::wstring(name); CP_XML_NODE_2(NS_NAME)

#define CP_XML_NODE_SIMPLE_NONS_2() CP_XML_NODE_2(std::wstring(name))


}
