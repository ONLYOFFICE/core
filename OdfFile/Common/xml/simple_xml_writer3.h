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
#include <stack>
#include <vector>
#include <cassert>

#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore
{

    namespace xml
    {
        class CBufferXml2
        {
        public:
            CBufferXml2() 
            {
                buffer_size = 0;
                curr_buff = -1;
                cbs = 0;
                buf = NULL;
            }
            ~CBufferXml2() 
            {
                buffer.clear();
                buffer_size = 0;
                curr_buff = -1;
                cbs = 0;
                buf = NULL;
            }
            void AddBuf(size_t data_size)
            {
                size_t size = std::max((int)data_size, 1000);
                buffer.emplace_back((wchar_t*)malloc(size * sizeof(wchar_t)));
                curr_buff++;
                buf = buffer[curr_buff];
                cbs = size;
            }
            void AddData(const wchar_t& El)
            {  
                if (cbs == 0)
                {
                    this->AddBuf(1);
                }
                if (cbs == 1)
                {
                    *buf = L'\0';
                    this->AddBuf(1);
                }
                *buf++ = El;
                buffer_size++;
                cbs--;
            }
            void AddData(const wchar_t* str)
            {  
                if (cbs == 0)
                {
                    this->AddBuf(wcslen(str));
                }
                if (cbs <= wcslen(str))
                {
                    *buf = L'\0';
                    this->AddBuf(wcslen(str));
                }
                wmemcpy(buf, str, wcslen(str));
                buf += wcslen(str);
                buffer_size += wcslen(str);
                cbs -= wcslen(str);
            }
            std::wstring str()
            {
                *buf = L'\0';
                wchar_t* str = (wchar_t*)malloc(buffer_size * sizeof(wchar_t));
                wchar_t* cur = str;
                for (const auto& a : buffer)
                {
                    wmemcpy(cur, a, wcslen(a));
                    cur += wcslen(a);
                }
                return std::wstring(str, buffer_size);
            }
            std::string utf8()
            {
                *buf = L'\0';
                std::string res;
                for (const auto& a : buffer)
                {
                    std::wstring tmp(a, wcslen(a));
                    utf8::utf16to8(tmp.begin(), tmp.end(), std::back_inserter(res));
                }
                return res;
            }
        private:
            std::vector<wchar_t*> buffer;
            wchar_t* buf;
            size_t buffer_size;
            size_t curr_buff;
            size_t cbs;
        };

        namespace writer
        {

            template <class T> struct chars3 { };

            template <>
            struct chars3<char>
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
            struct chars3<wchar_t>
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
            class element3;

            //
            //  xml::writer class
            //
            template <class T>
            class writer3
            {
            public:
                // writer must be bound to an ostream
                writer3(CBufferXml2& os, bool need_header = false) : level_(0), os_(os), need_header_(need_header) {}
                ~writer3(void) { assert(elements_.empty()); }

            private:
                size_t level_;
                CBufferXml2& os_;      // output stream
                bool need_header_;               // have we written an XML header yet?
                std::stack<element3<T>*> elements_;  // stack of open element tags


                // write XML header, if necessary
                writer3& header()
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
                writer3& putc(T c)
                {
                    os_.AddData(c);
                    return *this;
                }

                // write a string to the output stream
                writer3& puts(const T* str) {
                    os_.AddData(str);
                    return *this;
                }

                template <class V>
                friend class element3;
            };

            //
            //  xml::element class
            //
            template <class T>
            class element3
            {
            public:
                // create a new element tag, bound to an xml::writer
                element3(writer3<T>& wr, std::basic_string<T, std::char_traits<T>, std::allocator<T> > const& name) : wr_(wr), name_(name)
                {
                    wr_.level_++;
                    check_parent();
                    wr_.header().putc('<').puts(name_.c_str());
                    tagopen_ = true;
                    wr_.elements_.push(this);
                }

                // close the current element tag
                ~element3()
                {
                    if (!wr_.elements_.empty() && wr_.elements_.top() == this)
                    {
                        wr_.elements_.pop();
                        if (tagopen_)
                            wr_.putc(chars3<T>::slash)
                            .putc(chars3<T>::right_brocket);
                        else
                            wr_.putc(chars3<T>::left_brocket)
                            .putc(chars3<T>::slash)
                            .puts(name_.c_str())
                            .putc(chars3<T>::right_brocket);
                    }
                    wr_.level_--;
                }

                // write an attribute for the current element
                element3& attr(const T* name, const T* value)
                {
                    assert(name != 0);
                    assert(value != 0);
                    assert(tagopen_);
                    wr_.putc(chars3<T>::space)
                        .puts(name)
                        .putc(chars3<T>::eq)
                        .putc(chars3<T>::quote);
                    qputs(value);
                    wr_.putc(chars3<T>::quote);
                    return *this;
                }

                // attr() overload for std::string type
                element3& attr(const T* name, const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& value)
                {
                    return attr(name, value.c_str());
                }

                // attr() function template for all streamable types
                template <class V>
                element3& attr(const T* name, V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    attr(name, ss.str());
                    return *this;
                }

                // write text content for the current element
                element3& contents(const T* str)
                {
                    assert(str != 0);
                    check_parent();
                    qputs(str);
                    return *this;
                }

                // contents() overload for std::string type
                element3& contents(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return contents(str.c_str());
                }

                // contents() function template for all streamable types
                template <class V>
                element3& contents(V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    contents(ss.str());
                    return *this;
                }

                // write CDATA section
                element3& cdata(const T* str)
                {
                    assert(str != 0);
                    check_parent();
                   
                    wr_.puts(chars3<T>::cdata_open());
                    wr_.puts(str);
                    wr_.puts(chars3<T>::cdata_close());
                    return *this;
                }

                // cdata() overload for std::string type
                element3& cdata(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return cdata(str.c_str());
                }

            private:
                writer3<T>& wr_;        // bound XML writer

                //const T* name_;     // name of current element
                const std::basic_string<T, std::char_traits<T>, std::allocator<T> > name_;
                bool tagopen_;      // is the element tag for this element still open?

                // write a string quoting characters which have meaning in xml
                element3& qputs(const T* str)
                {
                    for (; *str; ++str)
                    {
                        switch (*str)
                        {
                        case chars3<T>::amp:
                            wr_.puts(chars3<T>::amp_str()); break;

                        case chars3<T>::left_brocket:
                            wr_.puts(chars3<T>::left_brocket_str()); break;

                        case chars3<T>::right_brocket:
                            wr_.puts(chars3<T>::right_brocket_str()); break;

                        case chars3<T>::apos:
                            wr_.puts(chars3<T>::apos_str()); break;

                        case chars3<T>::quote:
                            wr_.puts(chars3<T>::quote_str()); break;

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
                        wr_.putc(chars3<T>::right_brocket);
                        wr_.elements_.top()->tagopen_ = false;
                    }
                }
            };

        }

    }

    typedef xml::writer::writer3<wchar_t> xml_writer3;
    typedef xml::writer::element3<wchar_t> xml_element3;

#define CP_XML_WRITER_3(BUFF) if (bool _b_ = false) {} else for (xml_writer3 _xml_wr_3((BUFF));!_b_;_b_=true)
#define CP_XML_NODE_3(NAME) if (bool _b_ = false) {} else for (xml_element3 _xml_node_3(_xml_wr_3, (NAME));!_b_;_b_=true)
#define CP_XML_ATTR_3(NAME, VAL) _xml_node_3.attr((NAME),(VAL))
#define CP_XML_ATTR2_3(NAME, VAL) _xml_node_3.attr((std::wstring(NAME).c_str()),(VAL))
#define CP_XML_CONTENT_3(VAL) _xml_node_3.contents((VAL))

#define CP_GET_XML_NODE_3() _xml_node_3

#define CP_ATTR_NODE_3 xml_element3 & _xml_node_3

#define CP_XML_ATTR_ENCODE_STRING_3(NAME, VAL) _xml_node_3.attr((NAME), XmlUtils::EncodeXmlString(VAL))

#define CP_XML_ATTR_OPT_3(NAME, VAL) if (VAL)CP_XML_ATTR(NAME, (*VAL))
#define CP_XML_ATTR_OPT_ENCODE_STRING_3(NAME, STR) if (STR)CP_XML_ATTR(NAME, XmlUtils::EncodeXmlString(*STR))

#define CP_XML_NODE_SIMPLE_3() std::wstring NS_NAME = std::wstring(ns) + std::wstring(L":") + std::wstring(name); CP_XML_NODE(NS_NAME)

#define CP_XML_NODE_SIMPLE_NONS_3() CP_XML_NODE(std::wstring(name))


}

