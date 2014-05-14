#include <vector>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPNoncopyable.h>

#include "odf_rels.h"

namespace cpdoccore 
{

namespace odf 
{
	class _mediaitems;//picture & media

	class rels;
	
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
			std::wostream & content() { return content_; }
			std::wstring str() { return content_.str(); }
			static _CP_PTR(content_simple) create();

		private:
			std::wstringstream content_;
		};

		class content_content;
		typedef _CP_PTR(content_content) content_content_ptr;
		
		class content_content : noncopyable
		{
		public:
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

			virtual void write(const std::wstring & RootPath) = 0;
			std::wstring local_path;
		private:
			element * element_;
		};

		inline element::~element()
		{}

	
		class simple_element : public element
		{
		public:
			simple_element(const std::wstring & FileName, const std::wstring & Content);
			static element_ptr create(const std::wstring & FileName, const std::wstring & Content);

			virtual void write(const std::wstring & RootPath);

		private:
			std::wstring file_name_;
			std::string content_utf8_;

		};

		class meta_file : public element
		{
		public:
			virtual void write(const std::wstring & RootPath);
		};

		class content_file : public element
		{
		public:
			void set_content(content_content_ptr & c){content_ = c;}
			virtual void write(const std::wstring & RootPath);
			
			content_content_ptr content_;
		};

		class styles_file : public element
		{
		public:
			void set_content(content_simple_ptr & c) {content_ = c;}
			virtual void write(const std::wstring & RootPath);
			content_simple_ptr content_;
		};

		class manifect_file : public element
		{
		public:
			manifect_file(std::wstring type);
			
			virtual void write(const std::wstring & RootPath);       
			void add_rels(rels & r); 

		private:
			rels rels_;
			std::wstring type_;

		};
		class mimetype_file : public element
		{
		public:
			mimetype_file(std::wstring type);
			
			virtual void write(const std::wstring & RootPath);       

		private:
			std::wstring type_;

		};
		class media : public element
		{
		public:
			media(_mediaitems & mediaitems);
			virtual void write(const std::wstring & RootPath);

		private:
			_mediaitems & mediaitems_;
		        
		};
		class pictures : public element
		{
		public:
			pictures(_mediaitems & mediaitems);
			virtual void write(const std::wstring & RootPath);

		private:
			_mediaitems & mediaitems_;
		        
		};
		class object_files : public element
		{
		public:
			object_files(){}
			
			void set_content(content_content_ptr & _Content);
			
			void set_styles (content_simple_ptr & _Styles);
			
			void set_media		(_mediaitems & mediaitems);    
			void set_pictures	(_mediaitems & mediaitems);    

			virtual void write(const std::wstring & RootPath);

		private:
			content_file	content_;			
			styles_file		styles_;
			
			element_ptr		meta_;
			element_ptr		media_;
			element_ptr		pictures_;
		};
		
		class odf_document : public element
		{
		public:
			odf_document(std::wstring type);
			
			void add_object(element_ptr _object,bool root=false);
			
			void set_rels(rels & r);
			
			virtual void write(const std::wstring & RootPath);

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