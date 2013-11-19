#include "precompiled_cpodf.h"
#include "pptx_package.h"

#include <utf8cpp/utf8.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace oox {
namespace package {

namespace fs = boost::filesystem;


pptx_content_types_file::pptx_content_types_file()
{
    content_type_.add_default(L"rels",  L"application/vnd.openxmlformats-package.relationships+xml");
    content_type_.add_default(L"xml",   L"application/xml");

    content_type_.add_default(L"jpg",     L"image/jpeg");
    content_type_.add_default(L"gif",     L"image/gif");
    content_type_.add_default(L"jpeg",    L"image/jpeg");
    content_type_.add_default(L"png",     L"image/png");
    content_type_.add_default(L"wmf",     L"image/x-wmf");
 
	content_type_.add_override(L"/_rels/.rels",			L"application/vnd.openxmlformats-package.relationships+xml");
    
	content_type_.add_override(L"/ppt/presentation.xml",L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
    content_type_.add_override(L"/ppt/tableStyles.xml",	L"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml");
  
	content_type_.add_override(L"/docProps/app.xml",	L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    content_type_.add_override(L"/docProps/core.xml",	L"application/vnd.openxmlformats-package.core-properties+xml");
}

pptx_document::pptx_document()
{
    ppt_files_.set_main_document(this);
   
	rels_file_ptr relFile = rels_file::create(L".rels");
  
	relFile->get_rels().add(relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument",			L"ppt/presentation.xml"));
    relFile->get_rels().add(relationship(L"rId2", L"http://schemas.openxmlformats.org/officedocument/2006/relationships/metadata/core-properties",	L"docProps/core.xml"));
    relFile->get_rels().add(relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties",		L"docProps/app.xml"));
   
	rels_files_.add_rel_file( relFile );
}

void pptx_document::write(const std::wstring & RootPath)
{
    ppt_files_.write(RootPath);
    docProps_files_.write(RootPath);
    content_type_.write(RootPath);
    rels_files_.write(RootPath);
}

////////////////////////////////////////////

slide_content::slide_content() : rels_(rels_file::create(L""))
{
        
}

_CP_PTR(slide_content) slide_content::create()
{
    return boost::make_shared<slide_content>();
}

void slide_content::add_rel(relationship const & r)
{
    rels_->get_rels().add(r);
}

void slide_content::add_rels(rels & r)
{
    BOOST_FOREACH(relationship & item, r.relationships())
	{
		rels_->get_rels().add(item);
	}
}
////////////

slides_files::slides_files()
{}

void slides_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slides_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"slides" ;
    fs::create_directory(path);

    size_t count = 0;
	
	//slides
	//slideMasters ??? 
	//slideLayouts ????
	//

    //BOOST_FOREACH(const slide_content_ptr & item, slides_)
    //{
    //    if (item)
    //    {
    //        count++;
    //        const std::wstring fileName = std::wstring(L"sheet") + boost::lexical_cast<std::wstring>(count) + L".xml";
    //        content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
    //        static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
    //        contentTypes.add_override(std::wstring(L"/xl/worksheets/") + fileName, kWSConType);

    //        if (rels_)
    //        {
    //            const std::wstring id = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(count);
    //            static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";
    //            const std::wstring fileRef = std::wstring(L"worksheets/") + fileName;
    //            rels_->add(id, kWSRel, fileRef);
    //        }

    //        item->get_rel_file()->set_file_name(fileName + L".rels"); 
    //        rels_files relFiles;
    //        relFiles.add_rel_file(item->get_rel_file());
    //        relFiles.write(BOOST_STRING_PATH(path));
    //        
    //        //item->get_rel_file()->write(path.string<std::wstring>());

    //        package::simple_element(fileName, item->str()).write(BOOST_STRING_PATH(path));
    //    }
    //}
}

////////////////////////////////////////////

ppt_files::ppt_files()
{
    rels_files_.add_rel_file(rels_file::create(L"presentation.xml.rels"));
}

void ppt_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"ppt";
    fs::create_directory(path);

    slides_files_.set_rels(&rels_files_);
    slides_files_.set_main_document( this->get_main_document() );
    slides_files_.write(BOOST_STRING_PATH(path));

	int index = 1;
    if (true)
    {
        //workbook_->hyperlinks->write(BOOST_STRING_PATH(path));
        rels_files_.add( relationship( L"hId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"ppt/presentation.xml" ) );
	}
	//if (sharedStrings_)
 //   {
 //       sharedStrings_->write(BOOST_STRING_PATH(path));
 //       rels_files_.add( relationship( L"shId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings", L"sharedStrings.xml" ) );
 //   }

    if (styles_)
    {
       styles_->write(BOOST_STRING_PATH(path));
       rels_files_.add( relationship( L"stId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"tableStyles.xml" ) );
    }

    if (presentation_)
    {
        presentation_->write(BOOST_STRING_PATH(path));
    }

    if (theme_)
    {
        theme_->write(BOOST_STRING_PATH(path));
    }

    if (media_)
    {
        media_->set_main_document(get_main_document());
        media_->write(BOOST_STRING_PATH(path));
    }

    //{
    //    charts_files_.set_main_document(get_main_document());
    //    charts_files_.write(BOOST_STRING_PATH(path));
    //}
	//if (drawings_)
 //   {
 //       drawings_->set_main_document(get_main_document());
 //       drawings_->write(BOOST_STRING_PATH(path));
 //   }
	//if (comments_)
 //   {
 //       comments_->set_main_document(get_main_document());
 //       comments_->write(BOOST_STRING_PATH(path));
 //   }
    rels_files_.write(BOOST_STRING_PATH(path));
}

void ppt_files::set_presentation(element_ptr Element)
{
    presentation_ = Element;
}

void ppt_files::set_styles(element_ptr Element)
{
    styles_ = Element;
}

//void ppt_files_files::set_sharedStrings(element_ptr Element)
//{
//    sharedStrings_ = Element;
//}

void ppt_files::add_slide(slide_content_ptr slide)
{
    slides_files_.add_slide(slide);
}

void ppt_files::set_media(mediaitems & _Mediaitems)
{
    media_ = element_ptr( new media(_Mediaitems) );
}
//void ppt_files::set_comments(element_ptr Element)
//{
//    comments_ = Element;
//}
//void ppt_files::set_drawings(element_ptr Element)
//{
//    drawings_ = Element;
//}
//void ppt_files::set_vml_drawings(element_ptr Element)
//{
//    vml_drawings_ = Element;
//}
//void ppt_files::add_charts(chart_content_ptr chart)
//{
//    charts_files_.add_chart(chart);
//}
////////////////////////////
//void xl_charts_files::add_chart(chart_content_ptr chart)
//{
//    charts_.push_back(chart);
//}
//void xl_charts_files::write(const std::wstring & RootPath)
//{
//    fs::wpath path = fs::wpath(RootPath) / L"charts";
//    fs::create_directory(path);
//
//    size_t count = 0;
//
//    BOOST_FOREACH(const chart_content_ptr & item, charts_)
//    {
//        if (item)
//        {
//            count++;
//            const std::wstring fileName = std::wstring(L"chart") + boost::lexical_cast<std::wstring>(count) + L".xml";
//            content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
//           
//			static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml";
//            contentTypes.add_override(std::wstring(L"/xl/charts/") + fileName, kWSConType);
//
//            package::simple_element(fileName, item->str()).write(BOOST_STRING_PATH(path));
//        }
//    }
//}
////////////////////////////
//xl_drawings_ptr xlsx_drawings::create(const std::vector<drawing_elm> & elms)
//{
//    return boost::make_shared<xlsx_drawings>(boost::ref(elms));
//}
//
//void xlsx_drawings::write(const std::wstring & RootPath)
//{
//    fs::wpath path = fs::wpath(RootPath) / L"drawings";
//    fs::create_directory(path);
//
//    BOOST_FOREACH(drawing_elm const & e, drawings_)
//    {
//        package::simple_element(e.filename, e.content).write(BOOST_STRING_PATH(path));        
//
//        rels_files relFiles;
//        rels_file_ptr r = rels_file::create(e.filename + L".rels");
//        e.drawings->dump_rels(r->get_rels());
//               
//                
//        relFiles.add_rel_file(r);
//        relFiles.write(BOOST_STRING_PATH(path));
//        
//
//        content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
//
//        const std::wstring kDrawingCT = L"application/vnd.openxmlformats-officedocument.drawing+xml";
//        contentTypes.add_override(L"/xl/drawings/" + e.filename, kDrawingCT);
//    }
//}

//////////////////////////////
//xl_comments_ptr xlsx_comments::create(const std::vector<comment_elm> & elms)
//{
//    return boost::make_shared<xlsx_comments>(boost::ref(elms));
//}
//
//void xlsx_comments::write(const std::wstring & RootPath)
//{
//    fs::wpath path = fs::wpath(RootPath);
//  
//	fs::wpath vml_path = fs::wpath(RootPath) / L"drawings";
//    fs::create_directory(vml_path);
//   
//	BOOST_FOREACH(comment_elm const & e, comments_)
//    {
//		content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
//
//		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml";
//        contentTypes.add_override(std::wstring(L"/xl/") + e.filename, kWSConType);
//			
//		package::simple_element(e.filename, e.content).write(BOOST_STRING_PATH(path));        
//		package::simple_element(e.vml_filename, e.vml_content).write(BOOST_STRING_PATH(vml_path));        
//	}
//}
//

}
}
}
