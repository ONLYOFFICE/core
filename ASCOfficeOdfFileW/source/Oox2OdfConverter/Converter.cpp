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

#include "Oox2OdfConverter.h"

#include "Converter.h"

#include "../utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"
#include "PptxConverter.h"

#include "../OdfFormat/office_document.h"
#include "../OdfFormat/odf_conversion_context.h"
#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"

#include "../../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Presentation.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Logic/Vml.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/CxnSp.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Pic.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/SmartArt.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/AlphaModFix.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Grayscl.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Effects/Duotone.h"

#include "../../../Common/DocxFormat/Source/XlsxFormat/Worksheets/Sparkline.h"
#include "../../../OfficeCryptReader/source/CryptTransform.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"

using namespace cpdoccore;

namespace Oox2Odf
{
    Converter::Converter(const std::wstring & path, const std::wstring  & type, const std::wstring & fontsPath, bool bTemplate)
    { 
		impl_ = NULL;
		
        if (type == _T("text"))			impl_ = new DocxConverter(path, bTemplate);
        if (type == _T("spreadsheet"))	impl_ = new XlsxConverter(path, bTemplate);
        if (type == _T("presentation"))	impl_ = new PptxConverter(path, bTemplate);

        if (impl_)
            impl_->set_fonts_directory(fontsPath);
	}

	Converter::~Converter() 
    {
		if (impl_ )delete impl_ ;
	}

    void Converter::convert()
    {
		if (!impl_)return;
		impl_->convertDocument();
    }
    void Converter::write(const std::wstring & out_path, const std::wstring & temp_path, const std::wstring & password, const std::wstring & documentID) const
    {
		if (!impl_)return;
		return impl_->write(out_path, temp_path, password, documentID);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OoxConverter::write(const std::wstring & out_path, const std::wstring & temp_path, const std::wstring & password, const std::wstring & documentID)
{
	if (!output_document)return;

	if (false == documentID.empty())
	{
		output_document->add_binary(L"documentID", NSFile::CUtf8Converter::GetUtf8StringFromUnicode(documentID));
	}
	
	if (password.empty())
	{
		output_document->write(out_path, false);
	}
	else
	{
		//encrypt files
		std::wstring temp_folder = NSDirectory::CreateDirectoryWithUniqueName(temp_path);
		output_document->write(temp_folder, true);
	
		encrypt_document(password, temp_folder, out_path);

		output_document->write_manifest(out_path);
		
		NSDirectory::DeleteDirectory(temp_folder);
	}
}
std::wstring EncodeBase64(const std::string & value)
{
	int nLength = 0;
	char *pData = NULL;
	std::wstring result;

	NSFile::CBase64Converter::Encode((BYTE*)value.c_str(), (int)value.length(), pData, nLength, NSBase64::B64_BASE64_FLAG_NOCRLF);
	if (pData)
	{
		result = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pData, nLength);
		delete []pData; pData = NULL;
	}
	return result;
}
bool OoxConverter::encrypt_document (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath)
{
	odf_writer::package::manifect_file* manifest =  output_document->get_manifest();
	if (!manifest) return false;

	odf_writer::rels *rels = manifest->get_rels();

	for (size_t i = 0; i < rels->relationships_.size(); i++)
	{
		if (rels->relationships_[i].target_ == L"/") continue;

		std::wstring inp_file_name = srcPath + FILE_SEPARATOR_STR + rels->relationships_[i].target_;
		std::wstring out_file_name = dstPath + FILE_SEPARATOR_STR + rels->relationships_[i].target_;

		if (std::wstring::npos != rels->relationships_[i].target_.find(L"/"))
		{
			std::vector<std::wstring> refs;
			boost::algorithm::split(refs, rels->relationships_[i].target_, boost::algorithm::is_any_of(L"/"), boost::algorithm::token_compress_on);

			std::wstring folder = dstPath;
			for (size_t j = 0; j < refs.size() - 1; j++)
			{
				folder += FILE_SEPARATOR_STR + refs[j];
				NSDirectory::CreateDirectory(folder);
			}
		}
		
		encrypt_file(password, inp_file_name, out_file_name, rels->relationships_[i].encryption_, rels->relationships_[i].size_);
	}
	return true;
}
bool OoxConverter::encrypt_file (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath, std::wstring &encrypt_info, int &size)
{
	CRYPT::ODFEncryptor		encryptor;
	CRYPT::_odfCryptData	cryptData;

	std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvMethodEncrypt);

	if (sApplication == L"Weak")
	{
//-----------------------
//blowfish
		cryptData.cipherAlgorithm		= CRYPT_METHOD::Blowfish_CFB;
		cryptData.start_hashAlgorithm	= CRYPT_METHOD::SHA1;
		cryptData.start_hashSize		= 20;

		cryptData.spinCount	= 1024;
		cryptData.keySize	= 7;//16;

		cryptData.checksum_size = 1024;
		cryptData.checksum_hashAlgorithm = CRYPT_METHOD::SHA1;
	}
	else
	{
//-----------------------
//aes
		cryptData.cipherAlgorithm		= CRYPT_METHOD::AES_CBC;
		cryptData.start_hashAlgorithm	= CRYPT_METHOD::SHA256;
		cryptData.start_hashSize		= 32;

		cryptData.spinCount	= 100000;
		cryptData.keySize	= 32;

		cryptData.checksum_size = 1024;
		cryptData.checksum_hashAlgorithm = CRYPT_METHOD::SHA256;
	}
//-----------------------
	NSFile::CFileBinary file;

	if (false == file.OpenFile(srcPath))
		return false;

	DWORD	size_inp = 0;
	size = file.GetFileSize();

	unsigned char* data_inp	= new unsigned char[size];
	unsigned char* data_out	= NULL;
	
	file.ReadFile(data_inp, size, size_inp); 
	file.CloseFile();	
//------------------------------------------------------------------------------------------
	encryptor.SetCryptData(cryptData);
	
	int size_out = encryptor.Encrypt(password, data_inp, size_inp, data_out);
	delete []data_inp;
	
	encryptor.GetCryptData(cryptData);
//------------------------------------------------------------------------------------------------------------
	if (!data_out) return false;

	if (false == file.CreateFileW(dstPath)) return false;
	
	file.WriteFile(data_out, size_out);
	file.CloseFile();
	delete []data_out;
//------------------------------------------------------------------------------------------
	odf_writer::office_element_ptr encryption_elm;
	odf_writer::create_element (L"manifest", L"encryption-data", encryption_elm, odf_context());

	encryption_elm->create_child_element(L"manifest", L"algorithm");
	encryption_elm->create_child_element(L"manifest", L"key-derivation");
	encryption_elm->create_child_element(L"manifest", L"start-key-generation");

	odf_writer::manifest_encryption_data* encryption_data = dynamic_cast<odf_writer::manifest_encryption_data*>(encryption_elm.get());
	
	if (!encryption_data) return false;

	odf_writer::manifest_algorithm*				algorithm = dynamic_cast<odf_writer::manifest_algorithm*>(encryption_data->algorithm_.get());
	odf_writer::manifest_key_derivation*		key_derivation = dynamic_cast<odf_writer::manifest_key_derivation*>(encryption_data->key_derivation_.get());
	odf_writer::manifest_start_key_generation*	start_key_generation = dynamic_cast<odf_writer::manifest_start_key_generation*>(encryption_data->start_key_generation_.get());

	if (key_derivation)
	{
		key_derivation->salt_ = EncodeBase64(cryptData.saltValue);	
		key_derivation->iteration_count_ = cryptData.spinCount;	
		key_derivation->key_size_ = cryptData.keySize;	
	}
//------------------------------------------------------------------------------------------
	if (start_key_generation)
	{
		switch(cryptData.start_hashAlgorithm)
		{
		case CRYPT_METHOD::SHA1:	start_key_generation->start_key_generation_name_ = L"SHA1"; break;
		case CRYPT_METHOD::SHA256:	start_key_generation->start_key_generation_name_ = L"http://www.w3.org/2000/09/xmldsig#sha256"; break;
		case CRYPT_METHOD::SHA512:	start_key_generation->start_key_generation_name_ = L"http://www.w3.org/2000/09/xmldsig#sha512"; break;
		}
		start_key_generation->key_size_ = cryptData.start_hashSize;
	}
//------------------------------------------------------------------------------------------
	if (algorithm)
	{
		algorithm->initialisation_vector_ = EncodeBase64(cryptData.initializationVector);

		switch(cryptData.cipherAlgorithm)
		{		
		case CRYPT_METHOD::AES_CBC:			algorithm->algorithm_name_ = L"http://www.w3.org/2001/04/xmlenc#aes256-cbc"; break;
		case CRYPT_METHOD::Blowfish_CFB:	algorithm->algorithm_name_ = L"Blowfish CFB"; break;
		}
	}
//------------------------------------------------------------------------------------------
	if (encryption_data)
	{
		encryption_data->checksum_= EncodeBase64(cryptData.checksum);

		switch(cryptData.checksum_hashAlgorithm)
		{	
		case CRYPT_METHOD::SHA1:	encryption_data->checksum_type_ = L"SHA1"; break;
		case CRYPT_METHOD::SHA256:	encryption_data->checksum_type_ = L"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0#sha256"; break;
		case CRYPT_METHOD::SHA512:	encryption_data->checksum_type_ = L"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0#sha512"; break;
		}
		if (cryptData.checksum_size == 1024)
		{
			if (cryptData.checksum_hashAlgorithm == CRYPT_METHOD::SHA1)
				encryption_data->checksum_type_ += L"/1K";
			else
				encryption_data->checksum_type_ += L"-1k";
		}
	}
//------------------------------------------------------------------------------------------
	std::wstringstream strm;
	encryption_elm->serialize(strm);

	encrypt_info = strm.str();
	
	return true;
}

void OoxConverter::set_fonts_directory(const std::wstring &fontsPath)
{
	if (odf_context() == NULL) return;

    odf_context()->set_fonts_directory(fontsPath);
}

void OoxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	try
	{
		if (oox_unknown == NULL)return;

		switch(oox_unknown->getType())
		{
			case OOX::et_graphicFrame:
			{
				convert(dynamic_cast<PPTX::Logic::GraphicFrame*>(oox_unknown));
			}break;
			case OOX::et_pic:
			{
				convert(dynamic_cast<PPTX::Logic::Pic*>(oox_unknown));
			}break;
			case OOX::et_dgm_DiagrammParts:
			{
				convert(dynamic_cast<PPTX::Logic::SmartArt*>(oox_unknown));
			}break;
			case OOX::et_a_Shape:
			case OOX::et_p_Shape:
			case OOX::et_dsp_Shape:
			case OOX::et_w_Shape:
			{
				convert(dynamic_cast<PPTX::Logic::Shape*>(oox_unknown));
			}break;
			case OOX::et_p_ShapeTree:
			case OOX::et_a_GroupShape:
			case OOX::et_w_GroupShape:
			case OOX::et_lc_LockedCanvas:
			{
				convert(dynamic_cast<PPTX::Logic::SpTree *>(oox_unknown));				
			}break;
			case OOX::et_cxnSp:
			{
				convert(dynamic_cast<PPTX::Logic::CxnSp*>(oox_unknown));
			}break;
			case OOX::et_a_prstGeom:
			{
				convert(dynamic_cast<PPTX::Logic::PrstGeom*>(oox_unknown));
			}break;
			case OOX::et_a_custGeom:
			{
				convert(dynamic_cast<PPTX::Logic::CustGeom*>(oox_unknown));
			}break;
			case OOX::et_a_ahXY:
			{
				convert(dynamic_cast<PPTX::Logic::AhXY*>(oox_unknown));
			}break;
			case OOX::et_a_ahPolar:
			{
				convert(dynamic_cast<PPTX::Logic::AhPolar*>(oox_unknown));
			}break;
			case OOX::et_a_lnTo:
			{
				convert(dynamic_cast<PPTX::Logic::LineTo*>(oox_unknown));
			}break;
			case OOX::et_a_moveTo:
			{
				convert(dynamic_cast<PPTX::Logic::MoveTo*>(oox_unknown));
			}break;
			case OOX::et_a_arcTo:
			{
				convert(dynamic_cast<PPTX::Logic::ArcTo*>(oox_unknown));
			}break;
			case OOX::et_a_quadBezTo:
			{
				convert(dynamic_cast<PPTX::Logic::QuadBezTo*>(oox_unknown));
			}break;
			case OOX::et_a_cubicBezTo:
			{
				convert(dynamic_cast<PPTX::Logic::CubicBezTo*>(oox_unknown));
			}break;
			case OOX::et_a_close:
			{
				convert(dynamic_cast<PPTX::Logic::Close*>(oox_unknown));
			}break;
			case OOX::et_p_r:
			case OOX::et_a_r:
			{
				convert(dynamic_cast<PPTX::Logic::Run*>(oox_unknown));
			}break;
			case OOX::et_p_br:
			case OOX::et_a_br:
			{
				convert(dynamic_cast<PPTX::Logic::Br*>(oox_unknown));
			}break;
			case OOX::et_a_fld:
			case OOX::et_p_fld:
			{
				convert(dynamic_cast<PPTX::Logic::Fld*>(oox_unknown));
			}break;
			case OOX::et_p_MathPara:
			{
				convert(dynamic_cast<PPTX::Logic::MathParaWrapper*>(oox_unknown));
			}break;
			case OOX::et_a_effectStyle:
			{
				convert(dynamic_cast<PPTX::Logic::EffectStyle*>(oox_unknown));
			}break;
			case OOX::et_a_alphaModFix:
			{
				convert(dynamic_cast<PPTX::Logic::AlphaModFix*>(oox_unknown));
			}break;
			case OOX::et_a_blur:
			{
				convert(dynamic_cast<PPTX::Logic::Blur*>(oox_unknown));
			}break;
			case OOX::et_a_fillOverlay:
			{
				convert(dynamic_cast<PPTX::Logic::FillOverlay*>(oox_unknown));
			}break;
			case OOX::et_a_glow:
			{
				convert(dynamic_cast<PPTX::Logic::Glow*>(oox_unknown));
			}break;
			case OOX::et_a_innerShdw:
			{
				convert(dynamic_cast<PPTX::Logic::InnerShdw*>(oox_unknown));
			}break;
			case OOX::et_a_outerShdw:
			{
				convert(dynamic_cast<PPTX::Logic::OuterShdw*>(oox_unknown));
			}break;
			case OOX::et_a_reflection:
			{
				convert(dynamic_cast<PPTX::Logic::Reflection*>(oox_unknown));
			}break;
			case OOX::et_a_softEdge:
			{
				convert(dynamic_cast<PPTX::Logic::SoftEdge*>(oox_unknown));
			}break;
			case OOX::et_a_grayscl:
			{
				convert(dynamic_cast<PPTX::Logic::Grayscl*>(oox_unknown));
			}break;
			case OOX::et_a_duotone:
			{
				convert(dynamic_cast<PPTX::Logic::Duotone*>(oox_unknown));
			}break;
			case OOX::et_a_effectDag:
			{
				convert(dynamic_cast<PPTX::Logic::EffectDag*>(oox_unknown));
			}break;
			case OOX::et_v_imagedata:
			{
				convert(dynamic_cast<OOX::Vml::CImageData*>(oox_unknown));
			}break;
			case OOX::et_v_textbox:
			{
				convert(dynamic_cast<OOX::Vml::CTextbox*>(oox_unknown));
			}break;
			case OOX::et_v_background:
			{
				convert(dynamic_cast<OOX::Vml::CBackground*>(oox_unknown));
			}break;
			case OOX::et_v_path:
			{
				convert(dynamic_cast<OOX::Vml::CPath*>(oox_unknown));
			}break;	
				case OOX::et_v_textpath:
			{
				convert(dynamic_cast<OOX::Vml::CTextPath*>(oox_unknown));
			}break;	
			case OOX::et_v_fill:
			{
				convert(dynamic_cast<OOX::Vml::CFill*>(oox_unknown));
			}break;
			case OOX::et_v_stroke:
			{
				convert(dynamic_cast<OOX::Vml::CStroke*>(oox_unknown));
			}break;
			case OOX::et_v_formulas:
			{
				convert(dynamic_cast<OOX::Vml::CFormulas*>(oox_unknown));
			}break;
			case OOX::et_v_shadow:
			{
				convert(dynamic_cast<OOX::Vml::CShadow*>(oox_unknown));
			}break;
			case OOX::et_v_image:
			{
				convert(dynamic_cast<OOX::Vml::CImage*>(oox_unknown));
			}break;
			case OOX::et_wd_wrap:
			{
				convert(dynamic_cast<OOX::VmlWord::CWrap*>(oox_unknown));
			}break;
			case OOX::et_a_extLst:
			{
				convert(dynamic_cast<OOX::Drawing::COfficeArtExtensionList*>(oox_unknown));
			}break;
			case OOX::et_a_ext:
			{
				convert(dynamic_cast<OOX::Drawing::COfficeArtExtension*>(oox_unknown));
			}break;
			// "ненужные" элементы
			case OOX::et_w_softHyphen:
			case OOX::et_w_proofErr:
			case OOX::et_w_proofState:
			{
				//бяка
			}break;
			default:
			{
                _CP_LOG << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
			}
		}
	}catch(...)
	{
        _CP_LOG << L"[error] :  no convert element(" << (oox_unknown ? oox_unknown->getType() : -1 ) << L")\n";
	}
}
std::wstring OoxConverter::find_link_by (smart_ptr<OOX::File> & oFile, int type)
{
	if (!oFile.IsInit()) return L"";
	
    std::wstring ref;
	if (type == 1 && OOX::FileTypes::Image == oFile->type())
	{
		OOX::Image* pImage = dynamic_cast<OOX::Image*>(oFile.GetPointer());

		if (pImage)
		{
			ref = pImage->filename().GetPath();
		}
	}
	if (type == 2 && OOX::FileTypes::HyperLink == oFile->type())
	{
		OOX::HyperLink* pHyperlink = dynamic_cast<OOX::HyperLink*>(oFile.GetPointer());
		
		if (pHyperlink && pHyperlink->bHyperlink)
		{
			ref = pHyperlink->Uri().GetPath();
		}
	}
	if (type == 3)
	{
		OOX::Media* pMedia = dynamic_cast<OOX::Media*>(oFile.GetPointer());

		if (pMedia)
		{
			ref = pMedia->filename().GetPath();
		}
	}
	if (type == 4)
	{
		OOX::OleObject* pOleObject = dynamic_cast<OOX::OleObject*>(oFile.GetPointer());

		if (pOleObject)
		{
			ref = pOleObject->filename().GetPath();
		}
	}
	return ref;
}

bool OoxConverter::convert(std::wstring sSchemeColor, DWORD & argb)
{
	PPTX::Theme * theme= oox_theme();
	if (!theme)return false;

	std::map<std::wstring, PPTX::Logic::UniColor>::iterator pFind = theme->themeElements.clrScheme.Scheme.find(sSchemeColor);

	if (pFind != theme->themeElements.clrScheme.Scheme.end())
	{
		PPTX::Logic::UniColor & color = pFind->second;
		
		argb = color.GetARGB(); 

		return true;
	}
	return false;
}

void OoxConverter::convert(double oox_font_size,  _CP_OPT(odf_types::font_size) & odf_font_size)
{
	 _CP_OPT(odf_types::length) odf_length;

	 odf_length = odf_types::length(oox_font_size, odf_types::length::pt);
	 
	 if (odf_length)
		 odf_font_size = odf_types::font_size(odf_length.get());
}
void OoxConverter::convert(OOX::JsaProject *jsaProject)
{
	if (!jsaProject) return;

	std::string content;
	NSFile::CFileBinary file;
	
	file.OpenFile(jsaProject->filename().GetPath());
	
	DWORD size = file.GetFileSize();
	content.resize(size);

	file.ReadFile((BYTE*)content.c_str(), size, size);
	file.CloseFile();
	
	content[size] = 0;

	output_document->add_binary(L"jsaProject.bin", content);
}
}
