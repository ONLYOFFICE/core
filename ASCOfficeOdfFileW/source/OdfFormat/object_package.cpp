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
		simple_element::simple_element(const std::wstring & FileName, const std::wstring & Content) : file_name_(FileName)
		{
			utf8::utf16to8(Content.begin(), Content.end(), std::back_inserter(content_utf8_));
		}

		void simple_element::write(const std::wstring & RootPath)
		{
			fs::ofstream file( fs::wpath(RootPath) / file_name_, std::ios_base::out | std::ios_base::binary );
			file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
			file << content_utf8_;
		}

		element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content)
		{
			return boost::make_shared<simple_element>(FileName, Content);
		}

////////////
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
			
			simple_element elm(L"mimetype", resStream.str());
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
					CP_XML_NODE(L"manifest:file-entry")
					{
						CP_XML_ATTR(L"manifest:full-path", L"/");
						CP_XML_ATTR(L"manifest:media-type", std::wstring(L"application/vnd.oasis.opendocument.") + type_);

					}

					rels_.serialize(CP_XML_STREAM());//картинки и медиа

					CP_XML_NODE(L"manifest:file-entry")
					{
						CP_XML_ATTR(L"manifest:full-path", L"content.xml");
						CP_XML_ATTR(L"manifest:media-type", L"text/xml");
					}			
					CP_XML_NODE(L"manifest:file-entry")
					{
						CP_XML_ATTR(L"manifest:full-path", L"styles.xml");
						CP_XML_ATTR(L"manifest:media-type", L"text/xml");
					}
					CP_XML_NODE(L"manifest:file-entry")
					{
						CP_XML_ATTR(L"manifest:full-path", L"meta.xml");
						CP_XML_ATTR(L"manifest:media-type", L"text/xml");
					}
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
					
		
					CP_XML_NODE(L"office:meta")
					{  
						CP_XML_NODE(L"meta:generator");
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

		media::media(mediaitems & _Mediaitems) : mediaitems_(_Mediaitems)
		{    
		}

		void media::write(const std::wstring & RootPath)
		{
			if (mediaitems_.count_media )return;

			fs::wpath path = fs::wpath(RootPath) / L"media";
			fs::create_directory(path);

			BOOST_FOREACH( mediaitems::item & item, mediaitems_.items() )
			{
				if (item.type == mediaitems::typeAudio || item.type == mediaitems::typeVideo)
				{
					fs::wpath file_name  = fs::wpath(item.href);
					fs::wpath file_name_out = fs::wpath(RootPath) / item.outputName;

					boost::filesystem::copy_file(item.href, file_name_out);
				}
			}

		}

		
		pictures::pictures(mediaitems & _Mediaitems) : mediaitems_(_Mediaitems)
		{    
		}

		void pictures::write(const std::wstring & RootPath)//folder by content.xml
		{
			if (mediaitems_.count_image )return;

			fs::wpath path = fs::wpath(RootPath) / L"Pictures";
			fs::create_directory(path);

			BOOST_FOREACH( mediaitems::item & item, mediaitems_.items() )
			{
				if (item.type == mediaitems::typeImage )
				{
					fs::wpath file_name  = fs::wpath(item.href);
					fs::wpath file_name_out = fs::wpath(RootPath) / item.outputName;

					boost::filesystem::copy_file(item.href, file_name_out);
				}
			}

		}


		void object_files::set_content(content_content_ptr & _Content)
		{
			content_.set_content(_Content);
			meta_ = element_ptr(new meta_file());
		}
		void object_files::set_media(mediaitems & _Mediaitems)
		{
			media_ = element_ptr( new media(_Mediaitems) );
		}
		void object_files::set_pictures(mediaitems & _Mediaitems)
		{
			pictures_ = element_ptr( new pictures(_Mediaitems) );
		}
		void object_files::set_styles(element_ptr Element)
		{
			styles_ = Element;
		}
		void object_files::write(const std::wstring & RootPath)
		{
			content_.write(RootPath);
		
			if (meta_)		meta_->write(RootPath);

			if (styles_)	styles_->write(RootPath);
			if (media_)		media_->write(RootPath);
			if (pictures_)	pictures_->write(RootPath);
		}

		void odf_document::add_object(element_ptr _object,bool root)
		{
			if (root)base_ = _object;
			else objects_.push_back(_object);
		}

		void odf_document::set_rels(rels  & r)
		{
			(dynamic_cast<manifect_file*>(manifest_.get()))->add_rels(r);
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
				std::wstring name = L"Object " + boost::lexical_cast<std::wstring>(++count);
				fs::wpath path = fs::wpath(RootPath) / name;
				fs::create_directory(path);
				
				item->write(path.string());
			}

			if (manifest_)	manifest_->write(RootPath);
			if (settings_)	settings_->write(RootPath);
			
		}
	}
}

}
