#pragma once

#include "oox_package.h"

#include <cpdoccore/CPNoncopyable.h>

#include "xlsx_drawings.h"
#include "xlsx_comments.h"

class CApplicationFonts;

namespace cpdoccore { 
namespace oox {
namespace package {

class xlsx_content_types_file : public content_types_file
{
public:
    xlsx_content_types_file();
};

class sheet_content;
typedef _CP_PTR(sheet_content) sheet_content_ptr;

class sheet_content : noncopyable
{
public:
    sheet_content();
    std::wostream & content() { return content_; }
    void add_rel(relationship const & r);
	void add_rels(rels & r);
    rels_file_ptr get_rel_file() { return rels_; }
    std::wstring str() { return content_.str(); }
    static _CP_PTR(sheet_content) create();

private:
    std::wstringstream content_;
    rels_file_ptr rels_;
};

/// \class  sheets_files
class sheets_files  : public element
{
public:
    sheets_files();

    void add_sheet(sheet_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);

public:    
    std::vector<sheet_content_ptr> sheets_;
    rels_files * rels_;

};

/// \class  xl_charts_files
class xl_charts_files  : public element
{
public:
	xl_charts_files(){}

    void add_chart(chart_content_ptr chart);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<chart_content_ptr> charts_;

};
///////////////////////////////////////////////////////////

class xl_comments;
typedef _CP_PTR(xl_comments) xl_comments_ptr;

/// \class xl_comments
class xl_comments: public element
{
public:
    virtual void write(const std::wstring & RootPath);

    xl_comments(const std::vector<comment_elm> & elms) : comments_ ( elms )
    {
    }

    static xl_comments_ptr create(const std::vector<comment_elm> & elms);

private:
    const std::vector<comment_elm> & comments_;

};

class xl_drawings;
typedef _CP_PTR(xl_drawings) xl_drawings_ptr;

/// \class xl_drawings
class xl_drawings: public element
{
public:
    virtual void write(const std::wstring & RootPath);
    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    xl_drawings(const std::vector<drawing_elm> & elms) : drawings_ ( elms )
    {
    }

    static xl_drawings_ptr create(const std::vector<drawing_elm> & elms);

private:
    const std::vector<drawing_elm> & drawings_;
    rels_files * rels_;

};
//class xl_vml_drawings;
//typedef _CP_PTR(xl_vml_drawings) xl_vml_drawings_ptr;
//
///// \class xl_drawings
//class xl_vml_drawings: public element
//{
//public:
//    virtual void write(const std::wstring & RootPath);
//    xl_vml_drawings(const std::vector<comment_elm> & elms) : vml_drawings_ ( comment_elm )
//    {
//    }
//
//    static xl_vml_drawings_ptr create(const std::vector<comment_elm> & elms);
//
//private:
//    const std::vector<comment_elm> & vml_drawings_;
//
//};

/// \class  xl_files
class xl_files : public element
{
public:
    xl_files();

public:
    virtual void write(const std::wstring & RootPath);

    void set_workbook(element_ptr Element);
    void set_styles(element_ptr Element);
    void set_sharedStrings(element_ptr Element);
    void add_sheet(sheet_content_ptr sheet);
    void set_media(mediaitems & _Mediaitems, CApplicationFonts *pAppFonts);    
    void set_drawings(element_ptr Element);
	void set_vml_drawings(element_ptr Element);
	void set_comments(element_ptr Element);
    void add_charts(chart_content_ptr chart);

private:
    rels_files rels_files_;
    sheets_files sheets_files_;
    xl_charts_files charts_files_;
    element_ptr theme_;
    element_ptr workbook_;

    element_ptr styles_;
    element_ptr sharedStrings_;
    element_ptr media_;
    element_ptr drawings_;
	element_ptr vml_drawings_;
	element_ptr comments_;

};

/// \class  xlsx_document
class xlsx_document : public document
{
public:
    xlsx_document();

public:
    virtual void write(const std::wstring & RootPath);
    virtual content_types_file & content_type() { return content_type_; }
    xl_files & get_xl_files() { return xl_files_; }

private:
    xlsx_content_types_file content_type_;
    xl_files				xl_files_;
    docProps_files			docProps_files_;
    rels_files				rels_files_;

};


}
}
}
