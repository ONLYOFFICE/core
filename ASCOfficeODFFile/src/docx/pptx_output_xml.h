#pragma once

#include <iosfwd>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPNoncopyable.h>

#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

class pptx_xml_slide;
typedef _CP_PTR(pptx_xml_slide) pptx_xml_slide_ptr;

class pptx_xml_slide: noncopyable
{
public:
    pptx_xml_slide(std::wstring const & name,std::wstring const & id);
    ~pptx_xml_slide();
public:
    std::wstring name() const;
    std::wstring rId() const;
   
    std::wostream & Data();
    std::wostream & Background();
	rels & Rels();

	//slideTiming
    
    void write_to(std::wostream & strm);

    static pptx_xml_slide_ptr create(std::wstring const & name,int id);

private:
	std::wstring name_;
    std::wstringstream  slideData_;
    std::wstringstream  slideBackground_;
	std::wstring rId_;

	rels rels_;
};
////////////////////////////////////////////////////////////////////////////////////
class pptx_xml_presentation: noncopyable
{
public:
	pptx_xml_presentation(){}
	~pptx_xml_presentation(){}

	std::wstringstream  & slidesData(){return slidesData_;}
    std::wstringstream  & slideMastersData(){return slideMastersData_;}
    std::wstringstream  & notesSlidesData(){return notesSlidesData_;}

    std::wstringstream  & slidesProperties(){return slidesProperties_;}
    std::wstringstream  & notesSlidesSize(){return notesSlidesSize_;}

	void write_to(std::wostream & strm);

private:
    std::wstringstream  slidesProperties_;
	std::wstringstream  notesSlidesSize_;


    std::wstringstream  slidesData_;
    std::wstringstream  slideMastersData_;
    std::wstringstream  notesSlidesData_;
	std::wstringstream  handoutMasterData_;
};

//////////////////////////////////////////////////////////////////////////////////////
class pptx_xml_slideLayout;
typedef _CP_PTR(pptx_xml_slideLayout) pptx_xml_slideLayout_ptr;

class pptx_xml_slideLayout: noncopyable
{
public:
    pptx_xml_slideLayout(std::wstring const & id);
    ~pptx_xml_slideLayout();
public:
    std::wstring rId() const;
   
    std::wostream & Data();
	rels & Rels();

	//slideTiming
    
    void write_to(std::wostream & strm);

    static pptx_xml_slideLayout_ptr create(int id);

private:
    std::wstringstream  slideLayoutData_;
	std::wstring rId_;

	rels rels_;
};
///////////////////////////////////////////////////////////////////////////////////////////
class pptx_xml_slideMaster;
typedef _CP_PTR(pptx_xml_slideMaster) pptx_xml_slideMaster_ptr;

class pptx_xml_slideMaster: noncopyable
{
public:
    pptx_xml_slideMaster(std::wstring const & id);
    ~pptx_xml_slideMaster();
public:
    std::wstring rId() const;
   
    std::wostream & Data();
    std::wostream & Background();
	rels & Rels();

	void add_layout(int id, const std::wstring & rId);
 	void add_theme(int id, const std::wstring & tId);
   
    void write_to(std::wostream & strm);

    static pptx_xml_slideMaster_ptr create(int id);

private:
    std::wstringstream  slideMasterData_;
    std::wstringstream  slideMasterBackground_;
	
	std::vector<std::wstring> layoutsId_;
	std::wstring rId_;

	std::wstring themeId_;

	rels rels_;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
class pptx_xml_theme;
typedef _CP_PTR(pptx_xml_theme) pptx_xml_theme_ptr;

class pptx_xml_theme: noncopyable
{
public:
	pptx_xml_theme(std::wstring const & name, int id){name_ = name; id_ = id;}
	~pptx_xml_theme(){}
public:
	std::wstring & name(){return name_;}
  	int & id(){return id_;}
 
	std::wstringstream & clrSchemeData(){return clrSchemeData_;}
	std::wstringstream & fontSchemeData(){return fontSchemeData_;}
	std::wstringstream & fmtSchemeData(){return fmtSchemeData_;}

    void write_to(std::wostream & strm);

    static pptx_xml_theme_ptr create(std::wstring const & name, int id);

private:
    std::wstringstream  clrSchemeData_;
    std::wstringstream  fmtSchemeData_;
    std::wstringstream  fontSchemeData_;
	
	std::wstring name_;
	int id_;
};
}
}
