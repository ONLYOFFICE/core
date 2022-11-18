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

#include <utf8cpp/utf8.h>
#include <xml/simple_xml_writer.h>

#include "object_package.h"
#include "mediaitems.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"

#include "../../../OOXML/SystemUtility/SystemUtility.h"
#include "../../../Common/3dParty/cryptopp/osrng.h"

namespace cpdoccore 
{
namespace odf_writer
{
	namespace package 
	{
		std::string GetUtf8StringFromUnicode_4bytes(const wchar_t* pUnicodes, size_t lCount)
		{
			std::string res;
			res.resize(6 * lCount + 1);

			BYTE* pData = (BYTE*)res.c_str();
			BYTE* pCodesCur = pData;

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;

				if (code < 0x80)
				{
					*pCodesCur++ = (BYTE)code;
				}
				else if (code < 0x0800)
				{
					*pCodesCur++ = 0xC0 | (code >> 6);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x10000)
				{
					*pCodesCur++ = 0xE0 | (code >> 12);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x1FFFFF)
				{
					*pCodesCur++ = 0xF0 | (code >> 18);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x3FFFFFF)
				{
					*pCodesCur++ = 0xF8 | (code >> 24);
					*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x7FFFFFFF)
				{
					*pCodesCur++ = 0xFC | (code >> 30);
					*pCodesCur++ = 0x80 | (code >> 24 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
			}
			*pCodesCur++ = 0;
			res.resize(pCodesCur - pData);

			return res;
		}

		std::string GetUtf8StringFromUnicode_2bytes(const wchar_t* pUnicodes, size_t lCount)
		{
			std::string res;
			res.resize(6 * lCount + 1);

			BYTE* pData = (BYTE*)res.c_str();
			BYTE* pCodesCur = pData;

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;
				if (code >= 0xD800 && code <= 0xDFFF && pCur < pEnd)
				{
					code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *pCur++));
				}

				if (code < 0x80)
				{
					*pCodesCur++ = (BYTE)code;
				}
				else if (code < 0x0800)
				{
					*pCodesCur++ = 0xC0 | (code >> 6);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x10000)
				{
					*pCodesCur++ = 0xE0 | (code >> 12);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x1FFFFF)
				{
					*pCodesCur++ = 0xF0 | (code >> 18);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x3FFFFFF)
				{
					*pCodesCur++ = 0xF8 | (code >> 24);
					*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x7FFFFFFF)
				{
					*pCodesCur++ = 0xFC | (code >> 30);
					*pCodesCur++ = 0x80 | ((code >> 24) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
			}
			*pCodesCur = 0;
			res.resize(pCodesCur - pData);
			
			return res;
		}

		std::string GetUtf8StringFromUnicode(const wchar_t* pUnicodes, size_t lCount)
		{
			if (sizeof(WCHAR) == 2)
			{
				return GetUtf8StringFromUnicode_2bytes(pUnicodes, lCount);
			}
			else
			{
				return GetUtf8StringFromUnicode_4bytes(pUnicodes, lCount);
			}
		}
		simple_element::simple_element(const std::wstring & FileName, const std::wstring & Content, bool utf8) : file_name_(FileName), utf8_(utf8)
		{
			if (utf8_)
			{
				if (Content.length() > 10 * 1024 * 1024)
				{
					size_t pos = 0;
					while (pos < Content.length())
					{
						size_t sz = 2 * 1024 * 1024;
						if (sz + pos > Content.length())
							sz = Content.length() - pos;
						if (sz < 1)
							break;

						content_utf8_ += NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(Content.c_str() + pos, sz);

						pos += sz;
					}
				}
				else 
					content_utf8_ = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(Content.c_str(), Content.length());
			}else
				content_utf8_ = std::string( Content.begin(), Content.end());
		}
		simple_element::simple_element(const std::wstring & FileName, wchar_t* Content, size_t Size, bool utf8)
		{
			if (utf8_)
			{
				if (Size > 10 * 1024 * 1024)
				{
					size_t pos = 0;
					while (pos < Size)
					{
						size_t sz = 2 * 1024 * 1024;
						if (sz + pos > Size)
							sz = Size - pos;
						if (sz < 1)
							break;

						content_utf8_ += NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(Content + pos, sz);

						pos += sz;
					}
				}
				else
					content_utf8_ = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(Content, Size);
			}
			else
			{
				std::wstring str(Content, Size);
				content_utf8_ = std::string(str.begin(), str.end());
			}
		}
		simple_element::simple_element(const std::wstring & FileName, std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> & streambuf, size_t Size, bool utf8)
		{
		}
		simple_element::simple_element(const std::wstring & FileName, const std::string & Content) : file_name_(FileName), utf8_(false), content_utf8_(Content)
		{
		}
		void simple_element::write(const std::wstring & RootPath, bool add_padding)
		{
			NSFile::CFileBinary file;
			
			if (file.CreateFileW( RootPath + FILE_SEPARATOR_STR + file_name_ ) == true)
			{
				DWORD content_utf8_size = (DWORD)content_utf8_.length();
				
				if (utf8_) 
				{
					std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
					file.WriteFile((BYTE*)root.c_str(), (DWORD)root.length());


					if (add_padding)
					{
						CryptoPP::RandomPool prng;
						
						CryptoPP::SecByteBlock padding(1024 + (content_utf8_size % 2));
						CryptoPP::OS_GenerateRandomBlock(false, padding, padding.size());
						if (prng.CanIncorporateEntropy())
						{
							prng.IncorporateEntropy(padding, padding.size());
						}

						std::string padding_start	= "<!-- ";
						std::string padding_end		= "-->";
						
						file.WriteFile((BYTE*)padding_start.c_str(), (DWORD)padding_start.length());
//--------------------------------
						int nLength = 0;
						char *pData = NULL;
						NSFile::CBase64Converter::Encode(padding.data(), padding.size(), pData, nLength, NSBase64::B64_BASE64_FLAG_NOCRLF);
						if (pData)
						{
							file.WriteFile((BYTE*)pData, nLength);
							delete []pData; pData = NULL;
						}
//--------------------------------
						file.WriteFile((BYTE*)padding_end.c_str(), (DWORD)padding_end.length());
						
					}
				}
				file.WriteFile((BYTE*)content_utf8_.c_str(), content_utf8_size);

				file.CloseFile();
			}
		}

		element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content, bool utf8)
		{
			return boost::make_shared<simple_element>(FileName, Content, utf8);
		}
//-------------------------------------------------------------------------------
		content_simple_ptr content_simple::create()
		{
			return boost::make_shared<content_simple>();
		}
		content_content_ptr content_content::create()
		{
			return boost::make_shared<content_content>();
		}
//-------------------------------------------------------------------------------
		void manifect_file::add_rels(rels  & r)
		{
			for (size_t i = 0; i < r.relationships_.size(); i++)
			{
				rels_.add(r.relationships_[i]);
			}
		}
		manifect_file::manifect_file(const std::wstring & type)
		{
			type_ = type;
		}
		binary_file::binary_file(const std::wstring &file_name, const std::string &value)
		{
			file_name_	= file_name;
			value_		= value;
		}	
		void binary_file::write(const std::wstring & RootPath, bool add_padding)
		{
			simple_element elm(file_name_, value_);
			elm.write(RootPath, false);
		}
		mimetype_file::mimetype_file(const std::wstring & type)
		{
			type_ = type;
		}	
		void mimetype_file::write(const std::wstring & RootPath, bool add_padding)
		{
			std::wstringstream resStream;
			
			resStream << L"application/vnd.oasis.opendocument.";
			resStream << type_;

			simple_element elm(L"mimetype", resStream.str(),false);
			elm.write(RootPath, false);
		}
		void manifect_file::write(const std::wstring & RootPath, bool add_padding)
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
			std::wstring path = RootPath + FILE_SEPARATOR_STR + L"META-INF";
            NSDirectory::CreateDirectory(path);
			simple_element elm(L"manifest.xml", resStream.str());
			elm.write(path, false);
		}

		void meta_file::write(const std::wstring & RootPath, bool add_padding)
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
					CP_XML_ATTR(L"office:version", L"1.2");		
		
					CP_XML_NODE(L"office:meta")
					{  
						CP_XML_NODE(L"meta:generator")
						{
							std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
							if (sApplication.empty())
								sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
							CP_XML_STREAM() << sApplication;
#if defined(INTVER)
							std::string s = VALUE2STR(INTVER);
							CP_XML_STREAM() << L"/" << std::wstring (s.begin(), s.end() );
#endif					
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
			elm.write(RootPath, add_padding);
		}
//-------------------------------------------------------------------------------
		media_files::media_files(_mediaitems & mediaitems, const std::wstring internal_folder, int type) : mediaitems_(mediaitems), type_(type), folder_(internal_folder)
		{    
		}

		void media_files::write(const std::wstring & RootPath, bool add_padding)
		{
			OOX::CPath path (RootPath + (folder_.empty() ? L"" : FILE_SEPARATOR_STR) + folder_);
            NSDirectory::CreateDirectory(path.GetPath());

			std::vector< _mediaitems::item >  & items =  mediaitems_.items();
			
			for (size_t i = 0; i < items.size(); i++)
			{
				if (items[i].type == type_)
				{
					OOX::CPath file_name_inp ( items[i].oox_ref);
					OOX::CPath file_name_out ( RootPath + FILE_SEPARATOR_STR + items[i].odf_ref); //ref содержит уже folder_

					NSFile::CFileBinary::Copy(file_name_inp.GetPath(), file_name_out.GetPath());
				}
			}

		}		
//-------------------------------------------------------------------------------
		void object_files::set_content(content_content_ptr & _content, bool bRootNode)
		{
			content.set_content(_content, bRootNode);
			meta = element_ptr(new meta_file());
		}
		void object_files::set_mediaitems(_mediaitems & mediaitems)
		{
			if (mediaitems.count_image > 0)
			{
				pictures = element_ptr( new media_files(mediaitems, L"Pictures", 1) );
			}
			if (mediaitems.count_media > 0)
			{
				media = element_ptr( new media_files(mediaitems, L"Media", 2) );
			}
			if (mediaitems.count_object > 0)
			{
				oleObjects = element_ptr( new media_files(mediaitems, L"", 3) );
			}
			if (mediaitems.count_object > 0)
			{
				imageObjects = element_ptr( new media_files(mediaitems, L"ObjectReplacements", 4) );
			}
		}
		void object_files::set_styles(content_simple_ptr & _content)
		{
			styles.set_content(_content);
		}
		void object_files::set_settings(content_simple_ptr & _content)
		{
			settings.set_content(_content);
		}
		void object_files::write(const std::wstring & RootPath, bool add_padding)
		{
			content.write(RootPath, add_padding);		
			styles.write(RootPath, add_padding);
			settings.write(RootPath, add_padding);
			
			if (meta)			meta->write(RootPath, add_padding);

			if (media)			media->write(RootPath, add_padding);
			if (pictures)		pictures->write(RootPath, add_padding);
			if (oleObjects)		oleObjects->write(RootPath, add_padding);
			if (imageObjects)	imageObjects->write(RootPath, add_padding);
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
		void odf_document::add_binary(const std::wstring &file_name, const std::string &value)
		{
			objects_.push_back(element_ptr(new binary_file(file_name, value)));
			
			rels rels_;
			rels_.add(relationship(std::wstring(L"application/binary"), file_name));			
			set_rels(rels_);
		}

		odf_document::odf_document(std::wstring type, bool bTemplate)
		{
			manifest_	=	element_ptr(new manifect_file(type + (bTemplate ? L"-template" : L"")));
			mimetype_	=	element_ptr(new mimetype_file(type + (bTemplate ? L"-template" : L"")));
		}
		
		std::wstring odf_document::get_type()
		{
			return dynamic_cast<manifect_file*>(manifest_.get())->get_type();
		}

		void odf_document::write_manifest(const std::wstring & RootPath)
		{
			if (mimetype_)	
				mimetype_->write(RootPath);
			if (manifest_)	
				manifest_->write(RootPath);
		}
		void odf_document::write(const std::wstring & RootPath, bool add_padding)
		{
			if (base_)base_->write(RootPath, add_padding);
			
			for (size_t i = 0; i < objects_.size(); i++)
			{		
				std::wstring path = RootPath + FILE_SEPARATOR_STR + objects_[i]->local_path;
                NSDirectory::CreateDirectory(path);
				
				objects_[i]->write(path, add_padding);
			}
			if (manifest_)	manifest_->write(RootPath, add_padding);
			if (mimetype_)  mimetype_->write(RootPath, add_padding);
			if (settings_)	settings_->write(RootPath, add_padding);			
		}
		
		void content_file::write(const std::wstring & RootPath, bool add_padding)
		{
			if (false == bXmlRootNodeWrite)
			{
				if (content_)
				{
					simple_element elm(L"content.xml", content_->content_str());
					content_.reset();

					elm.write(RootPath, add_padding);
				}
			}
			else
			{
				std::wstringstream resStream;
				
				CP_XML_WRITER(resStream)
				{
					CP_XML_NODE(L"office:document-content")
					{
						CP_XML_ATTR(L"xmlns:meta", L"urn:oasis:names:tc:opendocument:xmlns:meta:1.0");
						CP_XML_ATTR(L"xmlns:office", L"urn:oasis:names:tc:opendocument:xmlns:office:1.0");
						CP_XML_ATTR(L"xmlns:draw", L"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0");
						CP_XML_ATTR(L"xmlns:ooo", L"http://openoffice.org/2004/office");
						CP_XML_ATTR(L"xmlns:fo", L"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0");
						CP_XML_ATTR(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
						CP_XML_ATTR(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
						CP_XML_ATTR(L"xmlns:style", L"urn:oasis:names:tc:opendocument:xmlns:style:1.0");
						CP_XML_ATTR(L"xmlns:text", L"urn:oasis:names:tc:opendocument:xmlns:text:1.0");
						CP_XML_ATTR(L"xmlns:dr3d", L"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0");
						CP_XML_ATTR(L"xmlns:svg", L"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0");
						CP_XML_ATTR(L"xmlns:chart", L"urn:oasis:names:tc:opendocument:xmlns:chart:1.0");
						CP_XML_ATTR(L"xmlns:rpt", L"http://openoffice.org/2005/report");
						CP_XML_ATTR(L"xmlns:table", L"urn:oasis:names:tc:opendocument:xmlns:table:1.0");
						CP_XML_ATTR(L"xmlns:number", L"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0");
						CP_XML_ATTR(L"xmlns:ooow", L"http://openoffice.org/2004/writer");
						CP_XML_ATTR(L"xmlns:oooc", L"http://openoffice.org/2004/calc");
						CP_XML_ATTR(L"xmlns:of", L"urn:oasis:names:tc:opendocument:xmlns:of:1.2");
						CP_XML_ATTR(L"xmlns:css3t", L"http://www.w3.org/TR/css3-text/");
						CP_XML_ATTR(L"xmlns:tableooo", L"http://openoffice.org/2009/table");
						CP_XML_ATTR(L"xmlns:calcext", L"urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0");
						CP_XML_ATTR(L"xmlns:drawooo", L"http://openoffice.org/2010/draw");
						CP_XML_ATTR(L"xmlns:loext", L"urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0");
						CP_XML_ATTR(L"xmlns:grddl", L"http://www.w3.org/2003/g/data-view#");
						CP_XML_ATTR(L"xmlns:field", L"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0");
						CP_XML_ATTR(L"xmlns:math", L"http://www.w3.org/1998/Math/MathML");
						CP_XML_ATTR(L"xmlns:form", L"urn:oasis:names:tc:opendocument:xmlns:form:1.0");
						CP_XML_ATTR(L"xmlns:script", L"urn:oasis:names:tc:opendocument:xmlns:script:1.0");
						CP_XML_ATTR(L"xmlns:dom", L"http://www.w3.org/2001/xml-events");
						CP_XML_ATTR(L"xmlns:xforms", L"http://www.w3.org/2002/xforms");
						CP_XML_ATTR(L"xmlns:xsd", L"http://www.w3.org/2001/XMLSchema");
						CP_XML_ATTR(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");
						CP_XML_ATTR(L"xmlns:formx", L"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0");
						CP_XML_ATTR(L"xmlns:xhtml", L"http://www.w3.org/1999/xhtml");
						CP_XML_ATTR(L"xmlns:presentation", L"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0");
						CP_XML_ATTR(L"xmlns:smil", L"urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0");
						CP_XML_ATTR(L"xmlns:anim", L"urn:oasis:names:tc:opendocument:xmlns:animation:1.0");
						CP_XML_ATTR(L"xmlns:officeooo", L"http://openoffice.org/2009/office");
						CP_XML_ATTR(L"xmlns:chartooo", L"http://openoffice.org/2010/chart");
						CP_XML_ATTR(L"office:version", L"1.2");


						if ((content_) && (content_->styles_.rdbuf()->in_avail() != 0))
						{
							content_->styles_.flush();
							CP_XML_STREAM() << content_->styles_.rdbuf();
							content_->styles_.clear();
						}
						CP_XML_NODE(L"office:body")
						{
							if ((content_) && (content_->content_.rdbuf()->in_avail() != 0))
							{
								content_->content_.flush();
								CP_XML_STREAM() << content_->content_.rdbuf();
								content_->content_.clear();
							}
						}
					}
				}				
				simple_element elm(L"content.xml", resStream.str());
				elm.write(RootPath, add_padding);
			}
		}
		void styles_file::write(const std::wstring & RootPath, bool add_padding)
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
					CP_XML_ATTR(L"xmlns:smil",			L"urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0");
					CP_XML_ATTR(L"xmlns:anim",			L"urn:oasis:names:tc:opendocument:xmlns:animation:1.0" );
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
					
					if ((content_) && (content_->content().rdbuf()->in_avail() != 0))
					{
						content_->content().flush();
						CP_XML_STREAM() << content_->content().rdbuf();
					}
					
				}
			}
			content_.reset();

			simple_element elm(L"styles.xml", resStream.str());
			elm.write(RootPath, add_padding);
		}
		void settings_file::write(const std::wstring & RootPath, bool add_padding)
		{
			std::wstringstream resStream;
			CP_XML_WRITER(resStream)
			{
				CP_XML_NODE(L"office:document-settings")
				{  
					//CP_XML_ATTR(L"office:version",	L"1.2" );
					CP_XML_ATTR(L"xmlns:ooo",		L"http://openoffice.org/2004/office" );
					CP_XML_ATTR(L"xmlns:config",	L"urn:oasis:names:tc:opendocument:xmlns:config:1.0" ); 
					CP_XML_ATTR(L"xmlns:xlink",		L"http://www.w3.org/1999/xlink" ); 
					CP_XML_ATTR(L"xmlns:office",	L"urn:oasis:names:tc:opendocument:xmlns:office:1.0" );
					
					if ((content_) && (content_->content().rdbuf()->in_avail() != 0))
					{
						content_->content().flush();
						CP_XML_STREAM() << content_->content().rdbuf();
					}
					
				}
			}
			content_.reset();

			simple_element elm(L"settings.xml", resStream.str());
			elm.write(RootPath, add_padding);
		}
	}
}

}
