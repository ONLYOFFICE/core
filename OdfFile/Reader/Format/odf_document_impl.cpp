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

#include <iostream>

#include "odf_document_impl.h"

#include "odf_document.h"

#include <CPString.h>
#include <xml/sax.h>

#include <boost/algorithm/string.hpp>

#include "abstract_xml.h"
#include "odfcontext.h"

#include "office_body.h"
#include "office_document.h"
#include "office_elements.h"
#include "office_text.h"
#include "office_spreadsheet.h"
#include "office_presentation.h"
#include "office_chart.h"
#include "office_annotation.h"
#include "office_settings.h"
#include "office_scripts.h"
#include "office_forms.h"
#include "office_event_listeners.h"
#include "office_meta.h"

#include "styles.h"
#include "style_regions.h"
#include "style_presentation.h"

#include "templates.h"

#include "math_elements.h"
#include "math_elementaries.h"
#include "math_token_elements.h"
#include "math_table_elements.h"
#include "math_limit_elements.h"
#include "math_layout_elements.h"

#include "paragraph_elements.h"
#include "text_elements.h"

#include "table_calculation_settings.h"
#include "number_style.h"
#include "list.h"
#include "font_face.h"
#include "table.h"
#include "draw_common.h"

#include "draw_page.h"

#include "documentcontext.h"

#include "../../../OfficeCryptReader/source/CryptTransform.h"

#define PROGRESSEVENT_ID 0

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

content_xml_t_ptr odf_document::Impl::read_file_content(xml::sax * reader_owner)
{
	if (!reader_owner) return content_xml_t_ptr();

    content_xml_t_ptr result( new content_xml_t() );
    
	const std::wstring namespacePrefix	= reader_owner->namespacePrefix();
	const std::wstring localName		= reader_owner->nodeLocalName();
	
	result->add_child_element(reader_owner, namespacePrefix, localName);		

    return result;
}
std::string odf_document::Impl::read_binary(const std::wstring & Path)
{
	std::string result;

	NSFile::CFileBinary file;	
	if (file.OpenFile(Path))
	{	
		DWORD size = file.GetFileSize();
		result.resize(size);

		file.ReadFile((BYTE*)result.c_str(), size, size);
		file.CloseFile();

		result[size] = 0;
	}
	return result;
}

content_xml_t_ptr odf_document::Impl::read_file_content(const std::wstring & Path)
{
	cpdoccore::xml::sax_ptr Reader = cpdoccore::xml::create_sax( Path.c_str());
    content_xml_t_ptr result( new content_xml_t() );
    
	const std::wstring namespacePrefix	= Reader->namespacePrefix();
	const std::wstring localName		= Reader->nodeLocalName();
	
	result->add_child_element(Reader.get(), namespacePrefix, localName);		

    return result;    
}
odf_document::Impl::Impl(xml::sax * Reader, const std::wstring & tempPath) : context_(new odf_read_context())
{
	office_mime_type_ = 0;

	tmp_folder_original_ = tempPath;

	content_xml_ = read_file_content(Reader);
	
	if (content_xml_)
	{
		_CP_LOG << L"[info] parse fonts" << std::endl;
		parse_fonts(content_xml_->get_content());
		
		_CP_LOG << L"[info] parse styles" << std::endl;
		parse_styles(content_xml_->get_content());

		_CP_LOG << L"[info] parse manifest" << std::endl;
		parse_manifests(content_xml_->get_content());

		_CP_LOG << L"[info] parse meta" << std::endl;
		parse_meta(content_xml_->get_content());

		_CP_LOG << L"[info] parse settings" << std::endl;
		parse_settings(content_xml_->get_content());
			
		tmp_folder_ = NSDirectory::CreateDirectoryWithUniqueName(tempPath);
	}
}
odf_document::Impl::Impl(const std::wstring & srcPath, const std::wstring & tempPath, const std::wstring & password) : context_(new odf_read_context())
{
    office_mime_type_ = 0;

    tmp_folder_original_ = tempPath;

    if (NSDirectory::Exists(srcPath))
    {
        base_folder_ = srcPath;

        std::wstring manifest_xml	= srcPath + FILE_SEPARATOR_STR + L"META-INF" + FILE_SEPARATOR_STR + L"manifest.xml";
        std::wstring mimetype_xml	= srcPath + FILE_SEPARATOR_STR + L"mimetype";

        _CP_LOG << L"[info] read mimetype" << std::endl;
        NSFile::CFileBinary::ReadAllTextUtf8(mimetype_xml, mimetype_content_file_);

        _CP_LOG << L"[info] read manifest.xml" << std::endl;
        manifest_xml_ = read_file_content(manifest_xml);

        _CP_LOG << L"[info] parse manifest" << std::endl;
        parse_manifests(manifest_xml_ ? manifest_xml_->get_content() : NULL);

        if (!office_mime_type_)
        {
            office_mime_type_ = GetMimetype(mimetype_content_file_);
        }

        if (false == map_encryptions_.empty())
        {
            if (password.empty())
            {
                bError = true;
                return;
            }

            //decrypt files
            tmp_folder_ = NSDirectory::CreateDirectoryWithUniqueName(tempPath);

            bError = !decrypt_folder(password, base_folder_, tmp_folder_);

            if (bError)
                return;

            base_folder_ = tmp_folder_;
        }

        std::wstring content_xml	= base_folder_ + FILE_SEPARATOR_STR + L"content.xml";
        std::wstring styles_xml		= base_folder_ + FILE_SEPARATOR_STR + L"styles.xml";
        std::wstring meta_xml		= base_folder_ + FILE_SEPARATOR_STR + L"meta.xml";
        std::wstring settings_xml	= base_folder_ + FILE_SEPARATOR_STR + L"settings.xml";

        std::wstring jsaProject_bin	= base_folder_ + FILE_SEPARATOR_STR + L"jsaProject.bin";

//-----------------------------------------------------------------------------------------------------
          _CP_LOG << L"[info] read settings.xml" << std::endl;
        settings_xml_ = read_file_content(settings_xml);

        _CP_LOG << L"[info] read content.xml" << std::endl;
        content_xml_ = read_file_content(content_xml);

        _CP_LOG << L"[info] read styles.xml" << std::endl;
        styles_xml_ = read_file_content(styles_xml);

        _CP_LOG << L"[info] read meta.xml" << std::endl;
        meta_xml_ = read_file_content(meta_xml);

        jsaProject_bin_ = read_binary(jsaProject_bin);
//----------------------------------------------------------------------------------------
        _CP_LOG << L"[info] parse fonts" << std::endl;
        parse_fonts(content_xml_ ? content_xml_->get_content() : NULL);

        _CP_LOG << L"[info] parse styles" << std::endl;
        parse_styles(styles_xml_ ? styles_xml_->get_content() : NULL);

        _CP_LOG << L"[info] parse settings" << std::endl;
        parse_settings(settings_xml_ ? settings_xml_->get_content() : NULL);

        _CP_LOG << L"[info] parse meta" << std::endl;
        parse_meta(meta_xml_ ? meta_xml_->get_content() : NULL);
    }
    else
    {
        _CP_LOG << L"[info] read flat document" << std::endl;
        content_xml_ = read_file_content(srcPath);

        if (content_xml_)
        {
            _CP_LOG << L"[info] parse fonts" << std::endl;
            parse_fonts(content_xml_->get_content());

            _CP_LOG << L"[info] parse styles" << std::endl;
            parse_styles(content_xml_->get_content());

            _CP_LOG << L"[info] parse manifest" << std::endl;
            parse_manifests(content_xml_->get_content());

            _CP_LOG << L"[info] parse settings" << std::endl;
            parse_settings(content_xml_->get_content());

            tmp_folder_ = NSDirectory::CreateDirectoryWithUniqueName(tempPath);
        }
    }
}
odf_document::Impl::~Impl()
{
	if (!tmp_folder_.empty())
		NSDirectory::DeleteDirectory(tmp_folder_);
}

bool odf_document::Impl::decrypt_folder (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath)
{
	std::vector<std::wstring> arFiles		= NSDirectory::GetFiles(srcPath, false);
	std::vector<std::wstring> arDirectories	= NSDirectory::GetDirectories(srcPath);
	
	bool result = true;
	for (size_t i = 0; i < arFiles.size(); ++i)
	{
		result = false;
		std::wstring sFileName = NSFile::GetFileName(arFiles[i]);
		
		std::map<std::wstring, std::pair<office_element_ptr, int>>::iterator pFind;
		if (false == map_encryptions_.empty())
		{
			pFind = map_encryptions_.find(arFiles[i]);
			if ( pFind != map_encryptions_.end() )
			{
				result = decrypt_file(password, arFiles[i], dstPath + FILE_SEPARATOR_STR + sFileName, pFind->second.first, pFind->second.second);
				
				if (false == result)
					break;
			}
		}
		if (!result && false == map_encryptions_extra_.empty())
		{
			pFind = map_encryptions_extra_.find(arFiles[i]);
			if ( pFind != map_encryptions_.end() )
			{
				result = decrypt_file(password, arFiles[i], dstPath + FILE_SEPARATOR_STR + sFileName, pFind->second.first, pFind->second.second);
				
				if (false == result)
					break;
			}
		}
		if (!result) 
		{
			NSFile::CFileBinary::Copy(arFiles[i], dstPath + FILE_SEPARATOR_STR + sFileName);
			result = true;
		}
	}
	for (size_t i = 0; result && i < arDirectories.size(); ++i)
	{
		std::wstring sDirName = NSFile::GetFileName(arDirectories[i]);
		
		NSDirectory::CreateDirectory(dstPath + FILE_SEPARATOR_STR + sDirName);

		result = decrypt_folder(password, arDirectories[i], dstPath + FILE_SEPARATOR_STR + sDirName);
	}
	return result;
}
bool odf_document::Impl::decrypt_file (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath, office_element_ptr element, int file_size )
{
	manifest_encryption_data* encryption_data = dynamic_cast<manifest_encryption_data*>(element.get());
	if (!encryption_data) return false;

	manifest_algorithm*				algorithm = dynamic_cast<manifest_algorithm*>(encryption_data->algorithm_.get());
	manifest_key_derivation*		key_derivation = dynamic_cast<manifest_key_derivation*>(encryption_data->key_derivation_.get());
	manifest_start_key_generation*	start_key_generation = dynamic_cast<manifest_start_key_generation*>(encryption_data->start_key_generation_.get());

	CRYPT::_odfCryptData	cryptData;
	
	if (key_derivation)
	{
		cryptData.saltValue	= DecodeBase64(key_derivation->salt_);	
		cryptData.spinCount = key_derivation->iteration_count_;	
		cryptData.keySize	= key_derivation->key_size_;	
	}
//------------------------------------------------------------------------------------------
	if (start_key_generation)
	{
		if (std::wstring::npos != start_key_generation->start_key_generation_name_.find(L"sha"))
		{
			if (std::wstring::npos != start_key_generation->start_key_generation_name_.find(L"512"))
			{
				cryptData.start_hashAlgorithm = CRYPT_METHOD::SHA512;
			}
			if (std::wstring::npos != start_key_generation->start_key_generation_name_.find(L"256"))
			{
				cryptData.start_hashAlgorithm = CRYPT_METHOD::SHA256;
			}
		}
		cryptData.start_hashSize = start_key_generation->key_size_;
	}
//------------------------------------------------------------------------------------------
	if (algorithm)
	{
		cryptData.initializationVector = DecodeBase64(algorithm->initialisation_vector_);

		if (std::wstring::npos != algorithm->algorithm_name_.find(L"aes"))
		{
			if (std::wstring::npos != algorithm->algorithm_name_.find(L"cbc"))
				cryptData.cipherAlgorithm	= CRYPT_METHOD::AES_CBC;
			else
				cryptData.cipherAlgorithm	= CRYPT_METHOD::AES_ECB;//??
		}
		else if (std::wstring::npos != algorithm->algorithm_name_.find(L"blowfish"))
		{
			cryptData.cipherAlgorithm	= CRYPT_METHOD::Blowfish_CFB;
		}
	}
//------------------------------------------------------------------------------------------
	if (encryption_data)
	{
		cryptData.checksum = DecodeBase64(encryption_data->checksum_);

		cryptData.checksum_hashAlgorithm = CRYPT_METHOD::SHA1;
		if (std::wstring::npos != encryption_data->checksum_type_.find(L"sha"))
		{
			if (std::wstring::npos != encryption_data->checksum_type_.find(L"512"))
			{
				cryptData.checksum_hashAlgorithm = CRYPT_METHOD::SHA512;
			}
			if (std::wstring::npos != encryption_data->checksum_type_.find(L"256"))
			{
				cryptData.checksum_hashAlgorithm = CRYPT_METHOD::SHA256;
			}
		}

		size_t nPosChecksumSize = encryption_data->checksum_type_.find(L"-");
		if (std::wstring::npos == nPosChecksumSize)
			nPosChecksumSize = encryption_data->checksum_type_.find(L"/");
		if (std::wstring::npos != nPosChecksumSize)
		{
			std::wstring strSize = encryption_data->checksum_type_.substr(nPosChecksumSize + 1);
			if (strSize == L"1k")
			{
				cryptData.checksum_size = 1024;
			}
			else
			{
				//???
			}
		}
	}

	NSFile::CFileBinary file_inp;

	if (false == file_inp.OpenFile(srcPath))
		return false;

	DWORD dwSizeRead = 0;
	_UINT64 lengthRead = file_inp.GetFileSize();

	unsigned char* data		= new unsigned char[lengthRead];
	unsigned char* data_out	= NULL;
	
	file_inp.ReadFile(data, lengthRead, dwSizeRead); 
	file_inp.CloseFile();
//------------------------------------------------------------------------------------------
	CRYPT::ODFDecryptor decryptor;
	decryptor.SetCryptData(cryptData);
	
	bool result = decryptor.Decrypt(password, data, dwSizeRead, data_out, file_size);
	delete []data;

//------------------------------------------------------------------------------------------------------------
	if (result && data_out)
	{
		NSFile::CFileBinary file_out;
        file_out.CreateFileW(dstPath);
		file_out.WriteFile(data_out, file_size);
		file_out.CloseFile();
	
		delete []data_out;			
	}
	return result;
}
const std::wstring & odf_document::Impl::get_temp_folder() const 
{
	return tmp_folder_original_; 
}
const std::wstring & odf_document::Impl::get_folder() const 
{
	if (!base_folder_.empty())	return base_folder_; 
	else return tmp_folder_;
}


void odf_document::Impl::parse_fonts(office_element *element)
{
    do 
    {
        office_document_base * document = dynamic_cast<office_document_base *>( element );
        if (!document)
        {
            _CP_LOG << L"[warning] empty document\n";
            break;
        }

        office_font_face_decls* fontFaceDecls = dynamic_cast<office_font_face_decls*>( document->office_font_face_decls_.get() );
        
        if (!fontFaceDecls)
        {
            _CP_LOG << L"[warning] empty font face decls\n";
            break;
        }

		for (size_t i = 0; i < fontFaceDecls->style_font_face_.size(); i++)
		{
			office_element_ptr & elm = fontFaceDecls->style_font_face_[i];

            style_font_face* fontFace = dynamic_cast<style_font_face*>( elm.get() );
            if (!fontFace)
            {
                _CP_LOG << L"[warning] read font face error\n";
                continue;
            }

            const std::wstring styleName = fontFace->style_name_.get_value_or(L"");
            const std::wstring fontNameTmp = fontFace->svg_font_family_.get_value_or(L"");
            std::wstring fontName = L"";

            std::vector<std::wstring> fontNames;
            boost::algorithm::split(fontNames, fontNameTmp, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
            if (fontNames.empty() || fontNames[0].empty())
                fontName = styleName;
            else
                fontName = fontNames[0];

            std::wstring altName = fontNames.size() >= 2 ? fontNames[1] : L"";

            const std::wstring charset = fontFace->style_font_charset_.get_value_or(L"") == L"x-symbol" ? L"02" : L"00";
            
			std::wstring fontFamily 
                = fontFace->style_font_family_generic_.get_value_or( font_family_generic(font_family_generic::System) ).get_type() 
                == font_family_generic::System ?
                L"auto" : boost::lexical_cast<std::wstring>(*fontFace->style_font_family_generic_);
            
			const std::wstring fontPitch = fontFace->style_font_pitch_ ? boost::lexical_cast<std::wstring>(*fontFace->style_font_pitch_) : L"";

            boost::algorithm::trim_if(fontName, boost::algorithm::is_any_of("'"));

            font_instance_ptr fontInstance( new font_instance (styleName, fontName, charset, fontFamily, fontPitch, altName) );

            context_->fontContainer().add_font( fontInstance );
        }
    }
    while (0);
}
int odf_document::Impl::GetMimetype(std::wstring value)
{
	if (std::wstring::npos != value.find(L"application/vnd.oasis.opendocument.text"))
	{
		return 1;
	}
	else if (std::wstring::npos != value.find(L"application/vnd.oasis.opendocument.spreadsheet"))
	{
		return 2;
	}
	else if (std::wstring::npos != value.find(L"application/vnd.oasis.opendocument.presentation"))
	{
		return 3;
	}
	else if (std::wstring::npos != value.find(L"application/vnd.sun.xml.writer"))
	{
		return 4;
	}
	else if (std::wstring::npos != value.find(L"application/vnd.sun.xml.calc"))
	{
		return 5;
	}
	else if (std::wstring::npos != value.find(L"application/vnd.sun.xml.impress"))
	{
		return 6;
	}
	return 0;
}
void odf_document::Impl::parse_manifests(office_element *element)
{
    office_document_base * document = dynamic_cast<office_document_base *>( element );
    
	if (!document)return;

	for (size_t i = 0; i < document->manifests_.size(); i++)
	{	
		office_element_ptr & elm = document->manifests_[i];

		manifest_entry * entry = dynamic_cast<manifest_entry *>(elm.get());
		if (!entry)continue;
		
		if (entry->encryption_data_)
		{
			std::wstring file_path = entry->full_path_;

			XmlUtils::replace_all( file_path, L"/", FILE_SEPARATOR_STR);
			file_path = base_folder_ + FILE_SEPARATOR_STR + file_path;

			if (0 == entry->full_path_.find(L"Basic/")) //Cuaderno de notas 1.2.ods
			{
				map_encryptions_extra_.insert(std::make_pair(file_path, std::make_pair(entry->encryption_data_, entry->size)));
			}
			else
			{
				map_encryptions_.insert(std::make_pair(file_path, std::make_pair(entry->encryption_data_, entry->size)));
			}
		}

		if (entry->full_path_ == L"/")
		{
			office_mime_type_ = GetMimetype(entry->media_type_);
		}
	}
	if (!office_mime_type_ && document->office_mimetype_)
	{
		office_mime_type_ = GetMimetype(*document->office_mimetype_);
	}
}

void odf_document::Impl::parse_settings(office_element *element)
{
	office_document_base * document = dynamic_cast<office_document_base *>( element );
	if (!document)	return;

	office_settings * settings = dynamic_cast<office_settings*>(document->office_settings_.get());
	if (!settings)	return;

	for (size_t i = 0; i < settings->content_.size(); i++)
	{	
		office_element_ptr & elm = settings->content_[i];

		settings_config_item_set * item_set = dynamic_cast<settings_config_item_set *>(elm.get());
		if (!item_set)continue;

		if (item_set->config_name_ == L"ooo:configuration-settings")
		{
			for (size_t j = 0; j < item_set->content_.size(); j++)
			{	
				office_element_ptr & elm_sett = item_set->content_[j];
				settings_config_item * sett = dynamic_cast<settings_config_item *>(elm_sett.get());
				if (!sett)continue;

				context_->Settings().add(sett->config_name_, sett->content_);
			}
		}
		else if (item_set->config_name_ == L"ooo:view-settings")
		{
			for (size_t j = 0; j < item_set->content_.size(); j++)
			{	
				office_element_ptr & elm_sett = item_set->content_[j];
	
				settings_config_item * sett = dynamic_cast<settings_config_item *>(elm_sett.get());
				if (sett)
					context_->Settings().add_view(sett->config_name_, sett->content_);
				else
				{
					settings_config_item_map_indexed *map_sett = dynamic_cast<settings_config_item_map_indexed *>(elm_sett.get());
					if ((map_sett) && (map_sett->config_name_ == L"Views"))
					{
						for (size_t i = 0; i < map_sett->content_.size(); i++)
						{
							settings_config_item_map_entry *entry = dynamic_cast<settings_config_item_map_entry *>(map_sett->content_[i].get());
							if (!entry) continue;
							
							context_->Settings().start_view();
							for (size_t j = 0; j < entry->content_.size(); j++)
							{
								settings_config_item * sett = dynamic_cast<settings_config_item *>(entry->content_[j].get());
								if (sett)
									context_->Settings().add_view(sett->config_name_, sett->content_);
								
								settings_config_item_map_named *map_v = dynamic_cast<settings_config_item_map_named *>(entry->content_[j].get());
								if (map_v)
								{
									for (size_t n = 0; n < map_v->content_.size(); n++)
									{
										settings_config_item_map_entry *entry_v = dynamic_cast<settings_config_item_map_entry *>(map_v->content_[n].get());
										if (!entry_v) continue;
										
										context_->Settings().start_table_view(entry_v->config_name_);
										
										for (size_t k = 0; k < entry_v->content_.size(); k++)
										{
											sett = dynamic_cast<settings_config_item *>(entry_v->content_[k].get());
											if (sett)
												context_->Settings().add_view(sett->config_name_, sett->content_);
										}
										context_->Settings().end_table_view();
									}
								}
							}
							context_->Settings().end_view();
						}
					}
				}
			}
		}	
	}
}
void odf_document::Impl::parse_meta(office_element *element)
{
	office_document_base * document = dynamic_cast<office_document_base *>( element );
	if (!document)	return;

	office_meta * meta = dynamic_cast<office_meta*>(document->office_meta_.get());
	if (!meta)	return;

	for (size_t i = 0; i < meta->meta_user_defined_.size(); i++)
	{	
		meta_user_defined * user_defined = dynamic_cast<meta_user_defined*>(meta->meta_user_defined_[i].get());
		if (!user_defined) continue;

		if (user_defined->meta_name_.empty()) continue;
		
		context_->DocProps().add_user_defined(user_defined->meta_name_, user_defined->content_);
	}

	dc_creator * dc_creator_ = dynamic_cast<dc_creator*>(meta->dc_creator_.get());
	if (dc_creator_)
		context_->DocProps().dc_creator_ = dc_creator_->content_;

	dc_date * dc_date_ = dynamic_cast<dc_date*>(meta->dc_date_.get());
	if (dc_date_)
		context_->DocProps().dc_date_ = dc_date_->content_;

	dc_description * dc_description_ = dynamic_cast<dc_description*>(meta->dc_description_.get());
	if (dc_description_)
		context_->DocProps().dc_description_ = dc_description_->content_;

	dc_language * dc_language_ = dynamic_cast<dc_language*>(meta->dc_language_.get());
	if (dc_language_)
		context_->DocProps().dc_language_ = dc_language_->content_;

		dc_subject * dc_subject_ = dynamic_cast<dc_subject*>(meta->dc_subject_.get());
	if (dc_subject_)
		context_->DocProps().dc_subject_ = dc_subject_->content_;
	
	dc_title * dc_title_ = dynamic_cast<dc_title*>(meta->dc_title_.get());
	if (dc_title_)
		context_->DocProps().dc_title_ = dc_title_->content_;
	
	meta_generator * meta_generator_ = dynamic_cast<meta_generator*>(meta->meta_generator_.get());
	if (meta_generator_)
		context_->DocProps().application_ = meta_generator_->content_;
	
	meta_template * meta_template_ = dynamic_cast<meta_template*>(meta->meta_template_.get());
	if (meta_template_)
		context_->DocProps().template_ = meta_generator_->content_;

	meta_creation_date * meta_creation_date_ = dynamic_cast<meta_creation_date*>(meta->meta_creation_date_.get());
	if (meta_creation_date_)
		context_->DocProps().creation_date_ = meta_creation_date_->content_;
	
	meta_keyword * meta_keyword_ = dynamic_cast<meta_keyword*>(meta->meta_keyword_.get());
	if (meta_keyword_)
		context_->DocProps().keyword_ = meta_keyword_->content_;
	meta_document_statistic * meta_document_statistic_ = dynamic_cast<meta_document_statistic*>(meta->meta_document_statistic_.get());
	if (meta_document_statistic_)
	{
		context_->DocProps().character_count_ = meta_document_statistic_->character_count_;
		context_->DocProps().page_count_ = meta_document_statistic_->page_count_;
		context_->DocProps().paragraph_count_ = meta_document_statistic_->paragraph_count_;
		context_->DocProps().word_count_ = meta_document_statistic_->word_count_;
		context_->DocProps().non_whitespace_character_count_ = meta_document_statistic_->non_whitespace_character_count_;
	}
	meta_editing_cycles * meta_editing_cycles_ = dynamic_cast<meta_editing_cycles*>(meta->meta_editing_cycles_.get());
	if (meta_editing_cycles_)
		context_->DocProps().revision_ = XmlUtils::GetInteger(meta_editing_cycles_->content_);
}

void odf_document::Impl::parse_styles(office_element *element)
{
	text_outline_style *outlineStyle = NULL;
	text_linenumbering_configuration *linenumberingStyle = NULL;
    
	do
    {
        office_document_base * document = dynamic_cast<office_document_base *>( element );
        if (!document)
        {
            _CP_LOG << L"[warning] empty styles\n";
            break;
        }
       
        // parse automatic styles - эти стили используют объекты которые в оазис находятся в этом же документе
		//переопределяем имя - иначе при поиске может возникнуть коллизия.
        do
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>( document->office_automatic_styles_.get() );
            if (!automaticStyles)
            {
                _CP_LOG << L"[warning] error reading automatic styles\n";
                break;
            }

            // parse page layout
			for (size_t i = 0; i < automaticStyles->style_page_layout_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->style_page_layout_[i];

				style_page_layout * pageLayout = dynamic_cast<style_page_layout *>(elm.get());
                if (!pageLayout)
                {
                    _CP_LOG << L"[warning] error reading page layout\n";
                    continue;
                }

                context_->pageLayoutContainer().add_page_layout(pageLayout);
            } // end parse page layout

			for (size_t i = 0; i < automaticStyles->styles_.style_style_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->styles_.style_style_[i];

				style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

				context_->styleContainer().add_style
					(	L"common:" + styleInst->style_name_,
						styleInst->style_display_name_.get_value_or(L""),
						&(styleInst->content_),
						true,
						false,
						styleInst->style_parent_style_name_.get_value_or(L""),
						styleInst->style_next_style_name_.get_value_or(L""),
						styleInst->style_data_style_name_.get_value_or(L""),
						styleInst->style_percentage_data_style_name_.get_value_or(L""),
						styleInst->style_class_.get_value_or(L""),
						styleInst->style_list_style_name_,
						styleInst->style_list_level_,
						styleInst->style_default_outline_level_
                    );
            }
            // list styles
			for (size_t i = 0; i < automaticStyles->styles_.text_list_style_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->styles_.text_list_style_[i];

				text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                if (!listStyle)
                {
                    _CP_LOG << L"[warning] error list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }
        }
        while(0); 

        // parse master pages
        do
        {
            office_master_styles * masterStyles = dynamic_cast<office_master_styles *>( document->office_master_styles_.get() );
            if (!masterStyles)
            {
                _CP_LOG << L"[warning] error reading master styles\n";
                break;
            }

			for (size_t i = 0; i < masterStyles->style_master_page_.size(); i++)
			{	
				office_element_ptr & elm = masterStyles->style_master_page_[i];

				style_master_page * masterPage = dynamic_cast<style_master_page *>(elm.get());
                if (!masterPage)
                {
                    _CP_LOG << L"[warning] error reading master page\n";
                    continue;
                }

                const std::wstring styleName = masterPage->attlist_.style_name_.get_value_or(L"");
                const std::wstring pageLayoutName = masterPage->attlist_.style_page_layout_name_.get_value_or(L"");

                context_->pageLayoutContainer().add_master_page(styleName, pageLayoutName, masterPage);
            }        
        }
        while(0); // end parse master pages

        // parse styles
        do 
        {
            office_styles * docStyles = dynamic_cast<office_styles *>(document->office_styles_.get());
            if (!docStyles)
            {
                _CP_LOG << L"[warning] error reading styles\n";
                break;
            }
            // default styles
			for (size_t i = 0; i < docStyles->style_default_style_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->style_default_style_[i];

				default_style * styleInst = dynamic_cast<default_style *>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading default style\n";
                    continue;
                }

                context_->styleContainer().add_style(L"",
					L"",
                    &(styleInst->content_),
                    false,
                    true,
                    L"",
                    L"",
					L"",
                    L"",
					L"default");                                            
            }
			for (size_t i = 0; i < docStyles->style_presentation_page_layout_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->style_presentation_page_layout_[i];

				style_presentation_page_layout * pageLayout = dynamic_cast<style_presentation_page_layout *>(elm.get());

                if (!pageLayout)
                    continue;

				context_->pageLayoutContainer().add_presentation_page_layout(pageLayout->style_name_.get_value_or(L""),pageLayout);
            }
        
            // common styles
			for (size_t i = 0; i < docStyles->styles_.style_style_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->styles_.style_style_[i];

				style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_ && !styleInst->style_master_page_name_->empty())
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
					styleInst->style_display_name_.get_value_or(L""),
                    &(styleInst->content_),
                    false,
                    false,
                    styleInst->style_parent_style_name_.get_value_or(L""),
                    styleInst->style_next_style_name_.get_value_or(L""),
                    styleInst->style_data_style_name_.get_value_or(L""),
					styleInst->style_percentage_data_style_name_.get_value_or(L""),
					styleInst->style_class_.get_value_or(L""),
					styleInst->style_list_style_name_,
					styleInst->style_list_level_,
					styleInst->style_default_outline_level_
                    );
            }

            // list styles
			for (size_t i = 0; i < docStyles->styles_.text_list_style_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->styles_.text_list_style_[i];

                text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                context_->listStyleContainer().add_list_style(listStyle);
            }

			if (docStyles->text_outline_style_)
			{
                outlineStyle = dynamic_cast<text_outline_style *>(docStyles->text_outline_style_.get());   
				context_->listStyleContainer().add_outline_style(outlineStyle);
			}
			if (docStyles->text_linenumbering_configuration_)
			{
				linenumberingStyle = dynamic_cast<text_linenumbering_configuration *>(docStyles->text_linenumbering_configuration_.get());
				context_->pageLayoutContainer().add_linenumbering(linenumberingStyle);
			}
			for (size_t i = 0; i < docStyles->text_notes_configuration_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->text_notes_configuration_[i];

				const text_notes_configuration * conf = dynamic_cast<const text_notes_configuration *>(elm.get());
                if (!conf)
                    continue;
                
                context_->noteConfiguration().add( conf->noteclass_.get_type(), conf );
                //text::note_configuration
                                
            }

			for (size_t i = 0; i < docStyles->styles_.number_styles_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->styles_.number_styles_[i];

				const number_style_base * style = dynamic_cast<const number_style_base *>(elm.get());

                if (!style)
                    continue;

                context_->numberStyles().add(style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_gradient_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_gradient_[i];

				draw_gradient * style = dynamic_cast<draw_gradient *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"gradient:" + style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_fill_image_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_fill_image_[i];

				draw_fill_image * style = dynamic_cast<draw_fill_image *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"bitmap:" + style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_opacity_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_opacity_[i];

				draw_opacity * style = dynamic_cast<draw_opacity *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"opacity:" + style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_hatch_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_hatch_[i];

				draw_hatch * style = dynamic_cast<draw_hatch *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"hatch:" + style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->templates_.table_templates_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->templates_.table_templates_[i];

				table_table_template * style = dynamic_cast<table_table_template *>(elm.get());

                if (!style)
                    continue;

				context_->Templates().add(L"table:" + style->get_text_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_marker_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_marker_[i];

				draw_marker * style = dynamic_cast<draw_marker *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"marker:" + style->get_style_name(), elm);
            }
			for (size_t i = 0; i < docStyles->draw_styles_.draw_stroke_dash_.size(); i++)
			{	
				office_element_ptr & elm = docStyles->draw_styles_.draw_stroke_dash_[i];

				draw_stroke_dash * style = dynamic_cast<draw_stroke_dash *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"stroke_dash:" + style->get_style_name(), elm);
            }
		}
        while(0); // end parse styles
    }
    while (0);

    do
    {
        if (!content_xml_)
        {
            _CP_LOG << L"[warning] empty content xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( content_xml_->get_content() );
        if (!document)
        {
            _CP_LOG << L"[warning] empty document\n";
            break;
        }

        // parse automatic styles
        do 
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>(document->office_automatic_styles_.get());
            if (!automaticStyles)
            {
                _CP_LOG << L"[warning] error reading automatic styles\n";
                break;
            }

			for (size_t i = 0; i < automaticStyles->styles_.style_style_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->styles_.style_style_[i];

				style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_/* && !styleInst->style_master_page_name_->empty()*/)
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
					styleInst->style_display_name_.get_value_or(L""),
                    &(styleInst->content_),
                    true,
                    false,
                    styleInst->style_parent_style_name_.get_value_or(L""),
                    styleInst->style_next_style_name_.get_value_or(L""),
                    styleInst->style_data_style_name_.get_value_or(L""),
					styleInst->style_percentage_data_style_name_.get_value_or(L""),
					styleInst->style_class_.get_value_or(L""),
					styleInst->style_list_style_name_,
					styleInst->style_list_level_,
					styleInst->style_default_outline_level_
                    );
            }

			for (size_t i = 0; i < automaticStyles->styles_.text_list_style_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->styles_.text_list_style_[i];

				text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                if (!listStyle)
                {
                    _CP_LOG << L"[warning] error reading list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }

			for (size_t i = 0; i < automaticStyles->styles_.number_styles_.size(); i++)
			{	
				office_element_ptr & elm = automaticStyles->styles_.number_styles_[i];

				const number_style_base * style = dynamic_cast<const number_style_base *>(elm.get());

                if (!style)  continue;

                context_->numberStyles().add(style->get_style_name(), elm);
            }

        }
        while (0); // end parse automatic styles
    }
    while (false);

	context_->listStyleContainer().add_outline_style(outlineStyle);
}

bool odf_document::Impl::docx_convert(oox::docx_conversion_context & Context)
{
	Context.process_styles();	

    Context.process_fonts();
    Context.process_headers_footers();

    Context.start_document();
	
	if (content_xml_)
        content_xml_->docx_convert(Context);
   
	Context.end_document();

    // мы обрабатываем стили списка после того как сконвертировали документ,
    // так как в процессе конвертации документа у нас могу добавиться стили — 
    // в случае если используется text:start-value (начинаем нумерацию заново)
    Context.process_list_styles();

	Context.add_jsaProject(jsaProject_bin_);

	return true;
}
bool odf_document::Impl::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	try
    {
		_CP_LOG << L"[info] convert content" << std::endl;
       
		Context.start_document();
     
		if (content_xml_)
            content_xml_->xlsx_convert(Context);

        Context.end_document();

        _CP_LOG << L"[info] process styles" << std::endl;
        Context.process_styles();
      
		Context.add_jsaProject(jsaProject_bin_);
    }
    catch(boost::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(boost::diagnostic_information(ex))) << std::endl;
        throw;
    }
    catch(std::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(ex.what())) << L"\n";
        throw;
    }
    catch(...)
    {
        _CP_LOG << L"\n[error]: undefined\n";
        throw;
    }

	return true;
}

bool odf_document::Impl::pptx_convert(oox::pptx_conversion_context & Context) 
{
    try
    {
        _CP_LOG << L"[info] convert content" << std::endl;
	
		Context.start_document();
       
		if (content_xml_)
            content_xml_->pptx_convert(Context);
		
		Context.process_layouts();
		
		Context.process_master_pages();

        Context.end_document();
		
		Context.add_jsaProject(jsaProject_bin_);
	}
    catch(boost::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(boost::diagnostic_information(ex))) << std::endl;
        throw;
    }
    catch(std::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(ex.what())) << std::endl;
        throw;
    }
    catch(...)
    {
        _CP_LOG << L"\n[error]: undefined\n";
        throw;
    }
	return true;
}

odf_read_context & odf_document::Impl::odf_context() 
{ 
    return *context_.get(); 
}


const office_element * odf_document::Impl::get_content() const
{
    return content_xml_->get_content();
}

office_element * odf_document::Impl::get_content()
{
    return content_xml_->get_content();
}

}
}
