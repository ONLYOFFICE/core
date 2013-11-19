#pragma once

#include "oox_package.h"
#include <cpdoccore/CPNoncopyable.h>
//#include "pptx_drawings.h"
//#include "pptx_comments.h"

namespace cpdoccore { 
namespace oox {
namespace package {

class pptx_content_types_file : public content_types_file
{
public:
    pptx_content_types_file();
};

class slide_content;
typedef _CP_PTR(slide_content) slide_content_ptr;

class slide_content : noncopyable
{
public:
    slide_content();
    std::wostream & content() { return content_; }
    void add_rel(relationship const & r);
	void add_rels(rels & r);
    rels_file_ptr get_rel_file() { return rels_; }
    std::wstring str() { return content_.str(); }
    static _CP_PTR(slide_content) create();

private:
    std::wstringstream content_;
    rels_file_ptr rels_;
};

/// \class  slides_files
class slides_files  : public element
{
public:
    slides_files();

    void add_slide(slide_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);

public:    
    std::vector<slide_content_ptr> slides_;
    rels_files * rels_;

};

///// \class  xl_charts_files
//class xl_charts_files  : public element
//{
//public:
//	xl_charts_files(){}
//
//    void add_chart(chart_content_ptr chart);
//	virtual void write(const std::wstring & RootPath);
//    
//    std::vector<chart_content_ptr> charts_;
//
//};
/////////////////////////////////////////////////////////////
//
//class xlsx_comments;
//typedef _CP_PTR(xlsx_comments) xl_comments_ptr;
//
///// \class xlsx_comments
//class xlsx_comments: public element
//{
//public:
//    virtual void write(const std::wstring & RootPath);
//
//    xlsx_comments(const std::vector<comment_elm> & elms) : comments_ ( elms )
//    {
//    }
//
//    static xl_comments_ptr create(const std::vector<comment_elm> & elms);
//
//private:
//    const std::vector<comment_elm> & comments_;
//
//};
//
//class xlsx_drawings;
//typedef _CP_PTR(xlsx_drawings) xl_drawings_ptr;
//
///// \class xlsx_drawings
//class xlsx_drawings: public element
//{
//public:
//    virtual void write(const std::wstring & RootPath);
//    void set_rels(rels_files * rels)
//    {
//        rels_ = rels;
//    }
//
//    xlsx_drawings(const std::vector<drawing_elm> & elms) : drawings_ ( elms )
//    {
//    }
//
//    static xl_drawings_ptr create(const std::vector<drawing_elm> & elms);
//
//private:
//    const std::vector<drawing_elm> & drawings_;
//    rels_files * rels_;
//
//};

/// \class  ppt_files
class ppt_files : public element
{
public:
    ppt_files();

public:
    virtual void write(const std::wstring & RootPath);

    void set_presentation(element_ptr Element);
    void set_styles(element_ptr Element);
    //void set_sharedStrings(element_ptr Element);
    void add_slide(slide_content_ptr sheet);
    void set_media(mediaitems & _Mediaitems);    
 //   void set_drawings(element_ptr Element);
	//void set_vml_drawings(element_ptr Element);
	//void set_comments(element_ptr Element);
 //   void add_charts(chart_content_ptr chart);

private:
    rels_files rels_files_;
    slides_files slides_files_;
    
	element_ptr theme_;
	element_ptr presentation_;

    element_ptr styles_;
 //   xl_charts_files charts_files_;
 //   element_ptr sharedStrings_;
    element_ptr media_;
 //   element_ptr drawings_;
	//element_ptr vml_drawings_;
	//element_ptr comments_;
};

/// \class  xlsx_document
class pptx_document : public document
{
public:
    pptx_document();

public:
    virtual void write(const std::wstring & RootPath);
    virtual content_types_file & content_type() { return content_type_; }
    ppt_files & get_ppt_files() { return ppt_files_; }

private:
    pptx_content_types_file content_type_;
   
	ppt_files ppt_files_;
    docProps_files docProps_files_;
    rels_files rels_files_;

};

}
}
}
