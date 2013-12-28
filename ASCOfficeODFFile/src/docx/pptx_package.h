#pragma once

#include "oox_package.h"
#include <cpdoccore/CPNoncopyable.h>
#include "pptx_comments.h"

namespace cpdoccore { 
namespace oox {

class pptx_xml_presentation;
class pptx_xml_theme;
typedef _CP_PTR(pptx_xml_theme) pptx_xml_theme_ptr;

namespace package {

class pptx_content_types_file : public content_types_file
{
public:
    pptx_content_types_file();
};


///////////////////////////////////////////////////////////////////////////////////
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
	void set_slide_ref(const std::wstring & ref_file_path,const std::wstring & ref_file_name);

public:    
    std::vector<slide_content_ptr> slides_;
    rels_files * rels_;

};
/// \class  slideLayouts_files
class slideLayouts_files  : public element
{
public:
    slideLayouts_files();

    void add_slide(slide_content_ptr sheet);

    virtual void write(const std::wstring & RootPath);

public:    
    std::vector<slide_content_ptr> slides_;
};
///////////////////////////////////////////////////////////////////////////////


class theme_elements : public element
{
public:
    theme_elements(pptx_xml_theme_ptr & theme);//потом до вектора
    virtual void write(const std::wstring & RootPath);

private:
    pptx_xml_theme_ptr & theme_;

};
/////////////////////////////////////////////////////////////////////////////////////////

/// \class  slideMasters_files
class slideMasters_files  : public element
{
public:
    slideMasters_files();

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
///////////////////////////////////////////////////////////

class ppt_comments_files;
typedef _CP_PTR(ppt_comments_files) ppt_comments_files_ptr;

/// \class ppt_comments
class ppt_comments_files: public element
{
public:
    virtual void write(const std::wstring & RootPath);

    ppt_comments_files(const std::vector<pptx_comment_elm> & elms) : comments_ ( elms )
    {
    }

    static ppt_comments_files_ptr create(const std::vector<pptx_comment_elm> & elms);

private:
    const std::vector<pptx_comment_elm> & comments_;

};
/// \class  ppt_charts_files
class ppt_charts_files  : public element
{
public:
	ppt_charts_files(){}

    void add_chart(chart_content_ptr chart);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<chart_content_ptr> charts_;

};
///////////////////////////////////////////////////////////////////////////////////////////////////////
class ppt_files : public element
{
public:
    ppt_files();

public:
    virtual void write(const std::wstring & RootPath);

    void set_presentation(pptx_xml_presentation & presentation);
    void set_themes(pptx_xml_theme_ptr & theme);
    
	void set_styles(element_ptr Element);
	void set_comments(element_ptr Element);

	void add_slide(slide_content_ptr sheet);
	void add_slideLayout(slide_content_ptr sheet);
	void add_slideMaster(slide_content_ptr sheet);
	
	void set_media(mediaitems & _Mediaitems);    
    void add_charts(chart_content_ptr chart);


private:
    rels_files rels_files_;
   
	slides_files slides_files_;    
	slideLayouts_files slideLayouts_files_;    
	slideMasters_files slideMasters_files_;
	
    ppt_charts_files charts_files_;
	//slides_files notesSlides_files_;
	//slides_files notesMasters_files_;
	//slides_files handoutMasters_files_;
 
	element_ptr theme_;
	element_ptr presentation_;

    element_ptr tableStyles_;

	element_ptr comments_;
    element_ptr media_;
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
