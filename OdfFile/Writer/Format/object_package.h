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

#include <vector>

#include <CPSharedPtr.h>
#include <CPNoncopyable.h>

#include "odf_rels.h"

namespace cpdoccore 
{

namespace odf_writer 
{
	class _mediaitems;//picture & media
	
	namespace package 
	{
		class element;
		typedef shared_ptr<element>::Type element_ptr;
		typedef std::vector<element_ptr> element_ptr_array;

		class content_simple;
		typedef _CP_PTR(content_simple) content_simple_ptr;
		
		class content_simple : noncopyable
		{
		public:
			content_simple(){}
			static _CP_PTR(content_simple) create();
			std::wstringstream & content() { return content_;}
		private:
			std::wstringstream content_;
		};

		class content_content;
		typedef _CP_PTR(content_content) content_content_ptr;
		
		class content_content : noncopyable
		{
		public:
			friend class content_file;

			content_content(){}
			std::wostream & content() { return content_; }
			std::wostream & styles() { return styles_; }
			std::wstring content_str() { return content_.str(); }
			std::wstring styles_str() { return styles_.str(); }
			static _CP_PTR(content_content) create();

		private:
			std::wstringstream content_;
			std::wstringstream styles_;
		};

		class element
		{
		public:
			virtual ~element() = 0;

			virtual void write(const std::wstring & RootPath, bool add_padding = false) = 0;
			std::wstring local_path;
		};

		inline element::~element()
		{}

	
		class simple_element : public element
		{
		public:
			simple_element(const std::wstring & FileName, const std::string & Content);
			simple_element(const std::wstring & FileName, const std::wstring & Content, bool utf8 = true);
			
			simple_element(const std::wstring & FileName, wchar_t* Content, size_t Size, bool utf8 = true);

			simple_element(const std::wstring & FileName, std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> & streambuf, size_t Size, bool utf8 = true);

			static element_ptr create(const std::wstring & FileName, const std::wstring & Content, bool utf8 = true);
			static element_ptr create(const std::wstring & FileName, const std::string & Content);

			virtual void write(const std::wstring & RootPath, bool add_padding = false);

		private:
			std::wstring	file_name_;
			std::string		content_utf8_;
			bool			utf8_;

		};

		class meta_file : public element
		{
		public:
			virtual void write(const std::wstring & RootPath, bool add_padding = false);
		};

		class content_file : public element
		{
		private:
			bool bXmlRootNodeWrite = true;
		public:
			void set_content(content_content_ptr & c, bool bRootNode = true) { content_ = c; bXmlRootNodeWrite = bRootNode; }
			virtual void write(const std::wstring & RootPath, bool add_padding = false);
			
			content_content_ptr content_;
		};

		class styles_file : public element
		{
		public:
			void set_content(content_simple_ptr & c) {content_ = c;}
			virtual void write(const std::wstring & RootPath, bool add_padding = false);
			content_simple_ptr content_;
		};
		
		class settings_file : public element
		{
		public:
			void set_content(content_simple_ptr & c) {content_ = c;}
			virtual void write(const std::wstring & RootPath, bool add_padding = false);
			content_simple_ptr content_;
		};
		
		class manifect_file : public element
		{
		public:
			manifect_file(const std::wstring & type);
			
			virtual void write(const std::wstring & RootPath, bool add_padding = false);       
			void add_rels(rels & r); 

			std::wstring get_type() {return type_;}

			rels *get_rels() {return &rels_;}

		private:
			rels rels_;
			std::wstring type_;

		};
		class binary_file : public element
		{
		public:
			binary_file(const std::wstring &file_name, const std::string &value);
			
			virtual void write(const std::wstring & RootPath, bool add_padding = false);       

		private:
			std::string value_;
			std::wstring file_name_;

		};
		class mimetype_file : public element
		{
		public:
			mimetype_file(const std::wstring & type);
			
			virtual void write(const std::wstring & RootPath, bool add_padding = false);       

		private:
			std::wstring type_;

		};
		class media_files : public element
		{
		public:
			media_files(_mediaitems & mediaitems, const std::wstring internal_folder, int type);
			virtual void write(const std::wstring & RootPath, bool add_padding = false);

		private:
			_mediaitems&	mediaitems_;
			int				type_;
			std::wstring	folder_;
		        
		};
		class object_files : public element
		{
		public:
			object_files() {}
			
			void set_content	(content_content_ptr & _content, bool bRootNode = true);
			
			void set_styles		(content_simple_ptr & _styles);
			void set_settings	(content_simple_ptr & _settings);
			
			void set_mediaitems	(_mediaitems & mediaitems);    

			virtual void write(const std::wstring & RootPath, bool add_padding = false);

		private:
			content_file	content;			
			settings_file	settings;
			styles_file		styles;
			
			element_ptr		meta;

			element_ptr		media;
			element_ptr		pictures;
			element_ptr		oleObjects;
			element_ptr		imageObjects;
		};		
		class odf_document : public element
		{
		public:
			odf_document(std::wstring type, bool bTemplate);
			
			void add_object(element_ptr _object,bool root = false);
			void add_binary(const std::wstring &file_name, const std::string &value);
			
			void set_rels(rels & r);

			std::wstring get_type();
			
			virtual void write(const std::wstring & RootPath, bool add_padding);
			void write_manifest(const std::wstring & RootPath);

			manifect_file* get_manifest() {return dynamic_cast<manifect_file*>(manifest_.get());}
		private:
			element_ptr					base_;
			std::vector<element_ptr>	objects_;

			element_ptr					mimetype_;
			element_ptr					settings_;
			element_ptr					manifest_;

		};
	};
}

}
