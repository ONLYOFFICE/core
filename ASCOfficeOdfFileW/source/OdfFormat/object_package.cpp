#include "precompiled_cpodf.h"

#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <utf8cpp/utf8.h>
#include <cpdoccore/common/boost_filesystem_version.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "object_package.h"
#include "mediaitems.h"

namespace cpdoccore 
{
namespace odf
{
	namespace package 
	{

		namespace fs = boost::filesystem;
		simple_element::simple_element(const std::wstring & FileName, const std::wstring & Content, bool utf8) : file_name_(FileName), utf8_(utf8)
		{
			if (utf8_)
			{
				utf8::utf16to8(Content.begin(), Content.end(), std::back_inserter(content_utf8_));
			}else
				content_utf8_ = std::string( Content.begin(), Content.end());
		}

		void simple_element::write(const std::wstring & RootPath)
		{
			fs::ofstream file( fs::wpath(RootPath) / file_name_, std::ios_base::out | std::ios_base::binary );
			
			if (utf8_) 
				file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
			file << content_utf8_;
		}

		element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content, bool utf8)
		{
			return boost::make_shared<simple_element>(FileName, Content, utf8);
		}

////////////
		content_simple_ptr content_simple::create()
		{
			return boost::make_shared<content_simple>();
		}
		content_content_ptr content_content::create()
		{
			return boost::make_shared<content_content>();
		}
///////////////
		void manifect_file::add_rels(rels  & r)
		{
			BOOST_FOREACH(relationship & item, r.relationships())
			{
				rels_.add(item);
			}
		}
		manifect_file::manifect_file(std::wstring t)
		{
			type_ = t;
		}
		mimetype_file::mimetype_file(std::wstring t)
		{
			type_ = t;
		}	
		void mimetype_file::write(const std::wstring & RootPath)
		{
			std::wstringstream resStream;
			
			resStream << L"application/vnd.oasis.opendocument.";
			resStream << type_;

			simple_element elm(L"mimetype", resStream.str(),false);
			elm.write(RootPath);
		}
		void manifect_file::write(const std::wstring & RootPath)
		{
			std::wstringstream resStream;
			
			CP_XML_WRITER(resStream)
			{
				CP_XML_NODE(L"manifest:manifest")
				{  
					CP_XML_ATTR(L"xmlns:manifest", L"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0");

					rels_.serialize(CP_XML_STREAM());
				}
			}
			fs::wpath path = fs::wpath(RootPath) / L"META-INF";
			fs::create_directory(path);
			simple_element elm(L"manifest.xml", resStream.str());
			elm.write(path.string());
		}

		void meta_file::write(const std::wstring & RootPath)
		{
			std::wstringstream resStream;
			CP_XML_WRITER(resStream)
			{
				CP_XML_NODE(L"office:document-meta")
				{  
					CP_XML_ATTR(L"xmlns:office", L"urn:oasis:names:tc:opendocument:xmlns:office:1.0");
					CP_XML_ATTR(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
					CP_XML_ATTR(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
					CP_XML_ATTR(L"xmlns:meta", L"urn:oasis:names:tc:opendocument:xmlns:meta:1.0");
					CP_XML_ATTR(L"xmlns:ooo", L"http://openoffice.org/2004/office");
					CP_XML_ATTR(L"xmlns:text", L"urn:oasis:names:tc:opendocument:xmlns:text:1.0");
					CP_XML_ATTR(L"xmlns:grddl", L"http://www.w3.org/2003/g/data-view#");
					
					CP_XML_ATTR(L"xmlns:presentation", L"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0");					
					CP_XML_ATTR(L"xmlns:smil", L"urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0");
					CP_XML_ATTR(L"xmlns:anim", L"urn:oasis:names:tc:opendocument:xmlns:animation:1.0");
					CP_XML_ATTR(L"xmlns:chartooo", L"http://openoffice.org/2010/chart");
					CP_XML_ATTR(L"office:version",		L"1.2");		
		
					CP_XML_NODE(L"office:meta")
					{  
						CP_XML_NODE(L"meta:generator")
						{
							CP_XML_STREAM() << L"ONLYOFFICE Online Editor";
						}
						CP_XML_NODE(L"meta:initial-creator");
						CP_XML_NODE(L"meta:creation-date");
						CP_XML_NODE(L"meta:editing-cycles");
						CP_XML_NODE(L"meta:editing-duration");
						CP_XML_NODE(L"dc:date");
						CP_XML_NODE(L"dc:creator");
						CP_XML_NODE(L"meta:document-statistic");
					}
				}
			}
		    
			simple_element elm(L"meta.xml", resStream.str());
			elm.write(RootPath);
		}
		///////////////////////////

		media::media(_mediaitems & mediaitems) : mediaitems_(mediaitems)
		{    
		}

		void media::write(const std::wstring & RootPath)
		{
			if (mediaitems_.count_media < 1)return;

			fs::wpath path = fs::wpath(RootPath) / L"media";
			fs::create_directory(path);

			BOOST_FOREACH( _mediaitems::item & item, mediaitems_.items() )
			{
				if (item.type == _mediaitems::typeAudio || item.type == _mediaitems::typeVideo)
				{
					fs::wpath file_name  = fs::wpath(item.oox_ref);
					fs::wpath file_name_out = fs::wpath(RootPath) / item.odf_ref;

					boost::filesystem::copy_file(file_name, file_name_out);
				}
			}

		}

		
		pictures::pictures(_mediaitems & mediaitems) : mediaitems_(mediaitems)
		{    
		}

		void pictures::write(const std::wstring & RootPath)//folder by content.xml
		{
			if (mediaitems_.count_image < 1 )return;

			fs::wpath path = fs::wpath(RootPath) / L"Pictures";
			fs::create_directory(path);

			BOOST_FOREACH( _mediaitems::item & item, mediaitems_.items() )
			{
				if (item.type == _mediaitems::typeImage && item.oox_ref.length()>0)
				{
					fs::wpath file_name  = fs::wpath(item.oox_ref);
					fs::wpath file_name_out = fs::wpath(RootPath) / item.odf_ref;

					try
					{
						boost::filesystem::copy_file(file_name, file_name_out);
					}catch (...)
					{
					}
				}
			}

		}


		void object_files::set_content(content_content_ptr & _content)
		{
			content_.set_content(_content);
			meta_ = element_ptr(new meta_file());
		}
		void object_files::set_media(_mediaitems & mediaitems)
		{
			media_ = element_ptr( new media(mediaitems) );
		}
		void object_files::set_pictures(_mediaitems & mediaitems)
		{
			pictures_ = element_ptr( new pictures(mediaitems) );
		}
		void object_files::set_styles(content_simple_ptr & _content)
		{
			styles_.set_content(_content);
		}
		void object_files::write(const std::wstring & RootPath)
		{
			content_.write(RootPath);		
			styles_.write(RootPath);
			
			if (meta_)		meta_->write(RootPath);

			if (media_)		media_->write(RootPath);
			if (pictures_)	pictures_->write(RootPath);
		}

		void odf_document::add_object(element_ptr _object, bool root)
		{
			rels  r;
			if (root)
			{
				base_ = _object;
			}
			else 
			{
				objects_.push_back(_object);
			}
			dynamic_cast<manifect_file*>(manifest_.get())->add_rels(r);
		}

		void odf_document::set_rels(rels  & r)
		{
			dynamic_cast<manifect_file*>(manifest_.get())->add_rels(r);
		}

		odf_document::odf_document(std::wstring type)
		{
			manifest_	=	element_ptr(new manifect_file(type));
			mimetype_	=	element_ptr(new mimetype_file(type));
		}
		void odf_document::write(const std::wstring & RootPath)
		{
			if (base_)base_->write(RootPath);
			
			long count = 0;
			BOOST_FOREACH(const element_ptr & item, objects_)
			{				
				fs::wpath path = fs::wpath(RootPath) / item->local_path;
				fs::create_directory(path);
				
				item->write(path.string());
			}
			if (manifest_)	manifest_->write(RootPath);
			if (mimetype_)  mimetype_->write(RootPath);
			if (settings_)	settings_->write(RootPath);			
		}
		
		void content_file::write(const std::wstring & RootPath)
		{
			std::wstringstream resStream;
			CP_XML_WRITER(resStream)
			{
				CP_XML_NODE(L"office:document-content")
				{  
					CP_XML_ATTR(L"xmlns:office",		L"urn:oasis:names:tc:opendocument:xmlns:office:1.0" );
					CP_XML_ATTR(L"xmlns:style",			L"urn:oasis:names:tc:opendocument:xmlns:style:1.0" );
					CP_XML_ATTR(L"xmlns:text",			L"urn:oasis:names:tc:opendocument:xmlns:text:1.0" );
					CP_XML_ATTR(L"xmlns:table",			L"urn:oasis:names:tc:opendocument:xmlns:table:1.0" );
					CP_XML_ATTR(L"xmlns:draw",			L"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" );
					CP_XML_ATTR(L"xmlns:fo",			L"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" );
					CP_XML_ATTR(L"xmlns:xlink",			L"http://www.w3.org/1999/xlink" );
					CP_XML_ATTR(L"xmlns:dc",			L"http://purl.org/dc/elements/1.1/" );
					CP_XML_ATTR(L"xmlns:meta",			L"urn:oasis:names:tc:opendocument:xmlns:meta:1.0" );
					CP_XML_ATTR(L"xmlns:number",		L"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" );
					CP_XML_ATTR(L"xmlns:presentation",	L"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0" );
					CP_XML_ATTR(L"xmlns:svg",			L"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"); 
					CP_XML_ATTR(L"xmlns:chart",			L"urn:oasis:names:tc:opendocument:xmlns:chart:1.0" );
					CP_XML_ATTR(L"xmlns:dr3d",			L"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" );
					CP_XML_ATTR(L"xmlns:math",			L"http://www.w3.org/1998/Math/MathML" );
					CP_XML_ATTR(L"xmlns:form",			L"urn:oasis:names:tc:opendocument:xmlns:form:1.0" );
					CP_XML_ATTR(L"xmlns:script",		L"urn:oasis:names:tc:opendocument:xmlns:script:1.0" );
					CP_XML_ATTR(L"xmlns:ooo",			L"http://openoffice.org/2004/office" );
					CP_XML_ATTR(L"xmlns:ooow",			L"http://openoffice.org/2004/writer" );
					CP_XML_ATTR(L"xmlns:oooc",			L"http://openoffice.org/2004/calc" );
					CP_XML_ATTR(L"xmlns:dom",			L"http://www.w3.org/2001/xml-events" );
					CP_XML_ATTR(L"xmlns:xforms",		L"http://www.w3.org/2002/xforms");
					CP_XML_ATTR(L"xmlns:xsd",			L"http://www.w3.org/2001/XMLSchema" );
					CP_XML_ATTR(L"xmlns:xsi",			L"http://www.w3.org/2001/XMLSchema-instance" );
					CP_XML_ATTR(L"xmlns:rpt",			L"http://openoffice.org/2005/report" );
					CP_XML_ATTR(L"xmlns:of",			L"urn:oasis:names:tc:opendocument:xmlns:of:1.2" );
					CP_XML_ATTR(L"xmlns:xhtml",			L"http://www.w3.org/1999/xhtml" );
					CP_XML_ATTR(L"xmlns:grddl",			L"http://www.w3.org/2003/g/data-view#" );
					CP_XML_ATTR(L"xmlns:officeooo",		L"http://openoffice.org/2009/office" );
					CP_XML_ATTR(L"xmlns:textooo",		L"http://openoffice.org/2013/office" ); 
					CP_XML_ATTR(L"xmlns:tableooo",		L"http://openoffice.org/2009/table" );
					CP_XML_ATTR(L"xmlns:drawooo",		L"http://openoffice.org/2010/draw" );
					CP_XML_ATTR(L"xmlns:chartooo",		L"http://openoffice.org/2010/chart" );
					CP_XML_ATTR(L"xmlns:calcext",		L"urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0" );
					CP_XML_ATTR(L"xmlns:field",			L"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0" );
					CP_XML_ATTR(L"xmlns:formx",			L"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0" );
					CP_XML_ATTR(L"xmlns:loext",			L"urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0" ); 
					CP_XML_ATTR(L"xmlns:css3t",			L"http://www.w3.org/TR/css3-text/" );
					CP_XML_ATTR(L"office:version",		L"1.2");				
	
					//CP_XML_NODE(L"office:scripts");
					if (content_)
					{
						CP_XML_STREAM() << content_->styles_str();
					}
					CP_XML_NODE(L"office:body")
					{
						if (content_)
						{
							CP_XML_STREAM() << content_->content_str();
						}
					}
				}
			}
		    
			simple_element elm(L"content.xml", resStream.str());
			elm.write(RootPath);
		}
		void styles_file::write(const std::wstring & RootPath)
		{
			std::wstringstream resStream;
			CP_XML_WRITER(resStream)
			{
				CP_XML_NODE(L"office:document-styles")
				{  
					CP_XML_ATTR(L"xmlns:office",		L"urn:oasis:names:tc:opendocument:xmlns:office:1.0" );
					CP_XML_ATTR(L"xmlns:style",			L"urn:oasis:names:tc:opendocument:xmlns:style:1.0" );
					CP_XML_ATTR(L"xmlns:text",			L"urn:oasis:names:tc:opendocument:xmlns:text:1.0" );
					CP_XML_ATTR(L"xmlns:table",			L"urn:oasis:names:tc:opendocument:xmlns:table:1.0" );
					CP_XML_ATTR(L"xmlns:draw",			L"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" );
					CP_XML_ATTR(L"xmlns:fo",			L"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" );
					CP_XML_ATTR(L"xmlns:xlink",			L"http://www.w3.org/1999/xlink" );
					CP_XML_ATTR(L"xmlns:dc",			L"http://purl.org/dc/elements/1.1/" );
					CP_XML_ATTR(L"xmlns:meta",			L"urn:oasis:names:tc:opendocument:xmlns:meta:1.0" );
					CP_XML_ATTR(L"xmlns:number",		L"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" );
					CP_XML_ATTR(L"xmlns:presentation",	L"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0" );
					CP_XML_ATTR(L"xmlns:svg",			L"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"); 
					CP_XML_ATTR(L"xmlns:chart",			L"urn:oasis:names:tc:opendocument:xmlns:chart:1.0" );
					CP_XML_ATTR(L"xmlns:dr3d",			L"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" );
					CP_XML_ATTR(L"xmlns:math",			L"http://www.w3.org/1998/Math/MathML" );
					CP_XML_ATTR(L"xmlns:form",			L"urn:oasis:names:tc:opendocument:xmlns:form:1.0" );
					CP_XML_ATTR(L"xmlns:script",		L"urn:oasis:names:tc:opendocument:xmlns:script:1.0" );
					CP_XML_ATTR(L"xmlns:ooo",			L"http://openoffice.org/2004/office" );
					CP_XML_ATTR(L"xmlns:ooow",			L"http://openoffice.org/2004/writer" );
					CP_XML_ATTR(L"xmlns:oooc",			L"http://openoffice.org/2004/calc" );
					CP_XML_ATTR(L"xmlns:dom",			L"http://www.w3.org/2001/xml-events" );
					CP_XML_ATTR(L"xmlns:xforms",		L"http://www.w3.org/2002/xforms");
					CP_XML_ATTR(L"xmlns:xsd",			L"http://www.w3.org/2001/XMLSchema" );
					CP_XML_ATTR(L"xmlns:xsi",			L"http://www.w3.org/2001/XMLSchema-instance" );
					CP_XML_ATTR(L"xmlns:rpt",			L"http://openoffice.org/2005/report" );
					CP_XML_ATTR(L"xmlns:of",			L"urn:oasis:names:tc:opendocument:xmlns:of:1.2" );
					CP_XML_ATTR(L"xmlns:xhtml",			L"http://www.w3.org/1999/xhtml" );
					CP_XML_ATTR(L"xmlns:grddl",			L"http://www.w3.org/2003/g/data-view#" );
					CP_XML_ATTR(L"xmlns:officeooo",		L"http://openoffice.org/2009/office" );
					CP_XML_ATTR(L"xmlns:textooo",		L"http://openoffice.org/2013/office" ); 
					CP_XML_ATTR(L"xmlns:tableooo",		L"http://openoffice.org/2009/table" );
					CP_XML_ATTR(L"xmlns:drawooo",		L"http://openoffice.org/2010/draw" );
					CP_XML_ATTR(L"xmlns:chartooo",		L"http://openoffice.org/2010/chart" );
					CP_XML_ATTR(L"xmlns:calcext",		L"urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0" );
					CP_XML_ATTR(L"xmlns:field",			L"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0" );
					CP_XML_ATTR(L"xmlns:formx",			L"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0" );
					CP_XML_ATTR(L"xmlns:loext",			L"urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0" ); 
					CP_XML_ATTR(L"xmlns:css3t",			L"http://www.w3.org/TR/css3-text/" );
					CP_XML_ATTR(L"office:version",		L"1.2");		
					
					if (content_)
					{
						CP_XML_STREAM() << content_->str();
					}
					
				}
			}
		    
			simple_element elm(L"styles.xml", resStream.str());
			elm.write(RootPath);
		}
	}
}

}
