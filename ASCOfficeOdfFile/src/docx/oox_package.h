#ifndef _CPDOCCORE_OOX_PACKAGE_H_75f74d15
#define _CPDOCCORE_OOX_PACKAGE_H_75f74d15

#include <vector>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPNoncopyable.h>

#include "docx_content_type.h"
#include "docx_rels.h"

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

namespace cpdoccore { 
namespace oox {

class mediaitems;

namespace package {

class element;
typedef shared_ptr<element>::Type element_ptr;
typedef std::vector<element_ptr> element_ptr_array;

class document;


/// \class  element
class element
{
public:
    virtual ~element() = 0;
    void set_main_document(document * _document) { document_ = _document; }
    document * get_main_document() { return document_; }

public:
    virtual void write(const std::wstring & RootPath) = 0;

private:
    document * document_;
};

inline element::~element()
{}

class content_types_file : public element
{
public:
    content_types_file();
    virtual void write(const std::wstring & RootPath);
    content_type & get_content_type() { return content_type_; }

protected:
    content_type content_type_;

private:
    std::wstring filename_;
};

class simple_element : public element
{
public:
    simple_element(const std::wstring & FileName, const std::wstring & Content);
    static element_ptr create(const std::wstring & FileName, const std::wstring & Content);

public:
    virtual void write(const std::wstring & RootPath);

private:
    std::wstring file_name_;
    std::string content_utf8_;

};

class rels_file;
typedef boost::shared_ptr<rels_file> rels_file_ptr;

/// \class rels_file
class rels_file : public element
{
public:
    rels_file(std::wstring const & FileName);
    void set_file_name(std::wstring const & fileName) { filename_ = fileName;} 
    static rels_file_ptr create(std::wstring const & FileName);

public:
    virtual void write(const std::wstring & RootPath);

    rels & get_rels() { return rels_; }
    const rels & get_rels() const { return rels_; }

    bool empty() { return rels_.empty(); }

private:
    std::wstring filename_;
    rels rels_;
};

/// \class rels_files
class rels_files : public element
{
public:
    virtual void write(const std::wstring & RootPath);
    void add_rel_file(rels_file_ptr RelFile) {rels_file_ = RelFile; } ;
    void add(relationship const & r);
    void add(std::wstring const & Id,
        std::wstring const & Type,
        std::wstring const & Target,
        std::wstring const & TargetMode = L"");

    rels_file * get_rel_file() { return rels_file_.get(); }
    
private:
    rels_file_ptr rels_file_;
};


////////////////////////////////////////
///\class chart_files
class chart_content;
typedef _CP_PTR(chart_content) chart_content_ptr;

class chart_content : noncopyable
{
public:
    chart_content();
    std::wostream & content() { return content_; }
    void add_rel(relationship const & r);
    rels_file_ptr get_rel_file() { return rels_; }
    std::wstring str() { return content_.str(); }
    static _CP_PTR(chart_content) create();
private:
    std::wstringstream content_;
    rels_file_ptr rels_;
};
/////////////////////////////////
class document : public element
{
public:
    virtual content_types_file & content_type() = 0; 
};

class core_file : public element
{
public:
    virtual void write(const std::wstring & RootPath);
};

class app_file : public element
{
public:
    virtual void write(const std::wstring & RootPath);
};

class docProps_files : public element
{
public:
    docProps_files();

public:
    virtual void write(const std::wstring & RootPath);

private:
    core_file core_;
    app_file app_;

};

/// \class media
class media : public element
{
public:
    media(mediaitems & _Mediaitems);

public:
    virtual void write(const std::wstring & RootPath);

private:
    mediaitems & mediaitems_;
        
};

/// \class charts
class charts : public element
{
public:
    charts(mediaitems & _ChartsItems);

public:
    virtual void write(const std::wstring & RootPath);

private:
    mediaitems & chartsitems_;
        
};

} // namespace package
} // namespace oox
} // namespace cpdoccore

#endif
