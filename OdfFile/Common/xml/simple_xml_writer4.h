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
#include <memory>
#include <cassert>

#include "../../../OOXML/Base/Unit.h"

#define BufferMax 10485760

namespace cpdoccore
{
    namespace xml
    {
        class buffer
        {
        public:
            buffer() : cbs(BufferMax)
            { 
                buf = (wchar_t*)malloc(BufferMax * sizeof(wchar_t));
                tmp = buf;
            }
            buffer(size_t size) : cbs(size)
            { 
                buf = (wchar_t*)malloc(size * sizeof(wchar_t));
                tmp = buf;
            }
            ~buffer() 
            { 
                free(buf);
            }

            void AddData(const wchar_t& El)
            {
                *tmp++ = El;
                cbs--;
            }
            void AddData(const wchar_t* str)
            {
                wmemcpy(tmp, str, wcslen(str));
                tmp += wcslen(str);
                cbs -= wcslen(str);
                
            }
            bool If_add(size_t size)
            {
                if (cbs >= size + 1) return true;
                else return false;
            }
            const wchar_t* Get_Buffer() { return buf; }
            size_t Get_Buf_Size() { return cbs; }
        private:
            wchar_t* buf;
            wchar_t* tmp;
            size_t cbs;
        };

        class CBufferXml3
        {
        public:
            CBufferXml3() : cur_buf(0), buffer_size(0) 
            {
                std::shared_ptr<buffer> ptr = std::make_shared<buffer>();
                All_Buffer.emplace_back(ptr);
            }
            CBufferXml3(size_t buf_count) : cur_buf(-1), buffer_size(0)
            {
                All_Buffer.reserve(buf_count);
            }
            ~CBufferXml3() { All_Buffer.clear(); }

            void AddData(const wchar_t& El)
            {
                if (All_Buffer[cur_buf]->If_add(1))
                {
                    
                    All_Buffer[cur_buf]->AddData(El);
                    buffer_size++;
                }
                else
                {
                    All_Buffer[cur_buf]->AddData(L'\0');
                    cur_buf++;
                    std::shared_ptr<buffer> ptr = std::make_shared<buffer>();
                    All_Buffer.emplace_back(ptr);
                    All_Buffer[cur_buf]->AddData(El);
                    buffer_size++;
                }
            }
            void AddData(const wchar_t* str)
            {
                if (All_Buffer[cur_buf]->If_add(wcslen(str)))
                {
                    
                    All_Buffer[cur_buf]->AddData(str);
                    buffer_size += wcslen(str);
                }
                else
                {
                    All_Buffer[cur_buf]->AddData(L'\0');
                    cur_buf++;
                    std::shared_ptr<buffer> ptr;
                    if (wcslen(str) > BufferMax) ptr = std::make_shared<buffer>(wcslen(str));
                    else ptr = std::make_shared<buffer>();
                    All_Buffer.emplace_back(ptr); 
                    All_Buffer[cur_buf]->AddData(str);
                    buffer_size += wcslen(str);
                }
            }

            std::wstring str()
            {
                All_Buffer[cur_buf]->AddData(L'\0');
                std::shared_ptr<buffer> str = std::make_shared<buffer>(buffer_size);
                for (const auto& c : All_Buffer)
                {
                    str->AddData(const_cast<const wchar_t*>(c->Get_Buffer()));
                }
                return std::wstring(str->Get_Buffer(), buffer_size);
            }
            std::string utf8()
            {
                All_Buffer[cur_buf]->AddData(L'\0');
                std::string res;
                for (const auto& c : All_Buffer)
                {
                    std::wstring tmp(const_cast<const wchar_t*>(c->Get_Buffer()), wcslen(c->Get_Buffer()));
                    utf8::utf16to8(tmp.begin(), tmp.end(), std::back_inserter(res));
                }
                return res;
            }

            CBufferXml3 operator+(const CBufferXml3& other)
            {
                CBufferXml3 result(cur_buf + other.cur_buf + 2);
                if (All_Buffer[cur_buf]->Get_Buf_Size() > 0)
                {
                    All_Buffer[cur_buf]->AddData(L'\0');
                }
                for (const auto& c : All_Buffer)
                {
                    result.All_Buffer.emplace_back(c);
                    cur_buf++;
                }
                for (const auto& c : other.All_Buffer)
                {
                    result.All_Buffer.emplace_back(c);
                    cur_buf++;
                }
                result.buffer_size = buffer_size + other.buffer_size;
                return result;
            }

            CBufferXml3& operator=(const CBufferXml3& other)
            {
                for (const auto& c : other.All_Buffer)
                {
                    All_Buffer.emplace_back(c);
                    cur_buf++;
                }
                buffer_size = other.buffer_size;
                return *this;
            }

            CBufferXml3& operator+=(const CBufferXml3& other)
            {
                if (All_Buffer[cur_buf]->Get_Buf_Size() > 0)
                {
                    All_Buffer[cur_buf]->AddData(L'\0');
                }
                for (const auto& c : other.All_Buffer)
                {
                    All_Buffer.emplace_back(c);
                    cur_buf++;
                }
                buffer_size += other.buffer_size;
                return *this;
            }

            friend std::wostream& operator<<(std::wostream& os, const CBufferXml3& other)
            {
                if (other.All_Buffer[other.cur_buf]->Get_Buf_Size() > 0)
                {
                    other.All_Buffer[other.cur_buf]->AddData(L'\0');
                }
                for (const auto& c : other.All_Buffer)
                {
                    os << c->Get_Buffer();
                }
                return os;
            }

        public:
            std::vector<std::shared_ptr<buffer>> All_Buffer;
            size_t cur_buf;
            size_t buffer_size;
        };

        namespace writer
        {

            template <class T> struct chars4 { };

            template <>
            struct chars4<char>
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
            struct chars4<wchar_t>
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
            class element4;

            //
            //  xml::writer class
            //
            template <class T>
            class writer4
            {
            public:
                // writer must be bound to an ostream
                writer4(CBufferXml3& os, bool need_header = false) : level_(0), os_(os), need_header_(need_header) {}
                ~writer4(void) { assert(elements_.empty()); }

            private:
                size_t level_;
                CBufferXml3& os_;      // output stream
                bool need_header_;               // have we written an XML header yet?
                std::stack<element4<T>*> elements_;  // stack of open element tags


                // write XML header, if necessary
                writer4& header()
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
                writer4& putc(T c)
                {
                    os_.AddData(c);
                    return *this;
                }

                // write a string to the output stream
                writer4& puts(const T* str) {
                    os_.AddData(str);
                    return *this;
                }

                template <class V>
                friend class element4;
            };

            //
            //  xml::element class
            //
            template <class T>
            class element4
            {
            public:
                // create a new element tag, bound to an xml::writer
                element4(writer4<T>& wr, std::basic_string<T, std::char_traits<T>, std::allocator<T> > const& name) : wr_(wr), name_(name)
                {
                    wr_.level_++;
                    check_parent();
                    wr_.header().putc('<').puts(name_.c_str());
                    tagopen_ = true;
                    wr_.elements_.push(this);
                }

                // close the current element tag
                ~element4()
                {
                    if (!wr_.elements_.empty() && wr_.elements_.top() == this)
                    {
                        wr_.elements_.pop();
                        if (tagopen_)
                            wr_.putc(chars4<T>::slash)
                            .putc(chars4<T>::right_brocket);
                        else
                            wr_.putc(chars4<T>::left_brocket)
                            .putc(chars4<T>::slash)
                            .puts(name_.c_str())
                            .putc(chars4<T>::right_brocket);
                    }
                    wr_.level_--;
                }

                // write an attribute for the current element
                element4& attr(const T* name, const T* value)
                {
                    assert(name != 0);
                    assert(value != 0);
                    assert(tagopen_);
                    wr_.putc(chars4<T>::space)
                        .puts(name)
                        .putc(chars4<T>::eq)
                        .putc(chars4<T>::quote);
                    qputs(value);
                    wr_.putc(chars4<T>::quote);
                    return *this;
                }

                // attr() overload for std::string type
                element4& attr(const T* name, const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& value)
                {
                    return attr(name, value.c_str());
                }

                // attr() function template for all streamable types
                template <class V>
                element4& attr(const T* name, V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    attr(name, ss.str());
                    return *this;
                }

                // write text content for the current element
                element4& contents(const T* str)
                {
                    assert(str != 0);
                    check_parent();
                    qputs(str);
                    return *this;
                }

                // contents() overload for std::string type
                element4& contents(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return contents(str.c_str());
                }

                // contents() function template for all streamable types
                template <class V>
                element4& contents(V value)
                {
                    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > ss;
                    ss << value;
                    contents(ss.str());
                    return *this;
                }

                // write CDATA section
                element4& cdata(const T* str)
                {
                    assert(str != 0);
                    check_parent();

                    wr_.puts(chars4<T>::cdata_open());
                    wr_.puts(str);
                    wr_.puts(chars4<T>::cdata_close());
                    return *this;
                }

                // cdata() overload for std::string type
                element4& cdata(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
                {
                    return cdata(str.c_str());
                }

            private:
                writer4<T>& wr_;        // bound XML writer

                //const T* name_;     // name of current element
                const std::basic_string<T, std::char_traits<T>, std::allocator<T> > name_;
                bool tagopen_;      // is the element tag for this element still open?

                // write a string quoting characters which have meaning in xml
                element4& qputs(const T* str)
                {
                    for (; *str; ++str)
                    {
                        switch (*str)
                        {
                        case chars4<T>::amp:
                            wr_.puts(chars4<T>::amp_str()); break;

                        case chars4<T>::left_brocket:
                            wr_.puts(chars4<T>::left_brocket_str()); break;

                        case chars4<T>::right_brocket:
                            wr_.puts(chars4<T>::right_brocket_str()); break;

                        case chars4<T>::apos:
                            wr_.puts(chars4<T>::apos_str()); break;

                        case chars4<T>::quote:
                            wr_.puts(chars4<T>::quote_str()); break;

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
                        wr_.putc(chars4<T>::right_brocket);
                        wr_.elements_.top()->tagopen_ = false;
                    }
                }
            };

        }

    }

    typedef xml::writer::writer4<wchar_t> xml_writer4;
    typedef xml::writer::element4<wchar_t> xml_element4;

#define CP_XML_WRITER_4(BUFF) if (bool _b_ = false) {} else for (xml_writer4 _xml_wr_4((BUFF));!_b_;_b_=true)
#define CP_XML_NODE_4(NAME) if (bool _b_ = false) {} else for (xml_element4 _xml_node_4(_xml_wr_4, (NAME));!_b_;_b_=true)
#define CP_XML_ATTR_4(NAME, VAL) _xml_node_4.attr((NAME),(VAL))
#define CP_XML_ATTR2_4(NAME, VAL) _xml_node_4.attr((std::wstring(NAME).c_str()),(VAL))
#define CP_XML_CONTENT_4(VAL) _xml_node_4.contents((VAL))

#define CP_GET_XML_NODE_4() _xml_node_4

#define CP_ATTR_NODE_4 xml_element4 & _xml_node_4

#define CP_XML_ATTR_ENCODE_STRING_4(NAME, VAL) _xml_node_4.attr((NAME), XmlUtils::EncodeXmlString(VAL))

#define CP_XML_ATTR_OPT_4(NAME, VAL) if (VAL)CP_XML_ATTR(NAME, (*VAL))
#define CP_XML_ATTR_OPT_ENCODE_STRING_4(NAME, STR) if (STR)CP_XML_ATTR(NAME, XmlUtils::EncodeXmlString(*STR))

#define CP_XML_NODE_SIMPLE_4() std::wstring NS_NAME = std::wstring(ns) + std::wstring(L":") + std::wstring(name); CP_XML_NODE(NS_NAME)

#define CP_XML_NODE_SIMPLE_NONS_4() CP_XML_NODE(std::wstring(name))


}
