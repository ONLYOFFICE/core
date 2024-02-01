/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#pragma once

#include "../ASCConverters.h"
#include "../cextracttools.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace NExtractTools
{
	// COMMON FUNCTIONS
	NSFonts::IApplicationFonts* createApplicationFonts(InputParams& params)
	{
		NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();

		std::wstring sFontPath = params.getFontPath();

		if (sFontPath.empty())
			pFonts->Initialize();
		else
			pFonts->InitializeFromFolder(sFontPath);

		return pFonts;
	}

	std::wstring getExtentionByRasterFormat(int format)
	{
		switch (format)
		{
		case 1:  return L".bmp";
		case 2:  return L".gif";
		case 3:  return L".jpg";
		default: return L".png";
		}
		return L"";
	}

	std::wstring combinePath(const std::wstring& sDir, const std::wstring& sName)
	{
		return sDir + FILE_SEPARATOR_STR + sName;
	}

	_UINT32 processEncryptionError(_UINT32 hRes, const std::wstring& sFrom, InputParams& params)
	{
		if (AVS_ERROR_DRM == hRes)
		{
			if (!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, *params.m_sFileTo);
			}
			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		else if (AVS_ERROR_PASSWORD == hRes)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}
		return hRes;
	}

	bool create_if_empty(const std::wstring& sFrom, const std::wstring& sTo, const std::wstring& signature)
	{
		bool res = false;
		NSFile::CFileBinary file;
		if (file.OpenFile(sFrom))
		{
			long file_size = file.GetFileSize();
			file.CloseFile();

			if (file_size == 0)
			{
				file.CreateFileW(sTo);

				if (false == signature.empty())
				{
					file.WriteStringUTF8(signature);
				}
				file.CloseFile();

				res = true;
			}
		}
		return res;
	}

	// Add record to [Content_Types].xml
	_UINT32 addContentType(const std::wstring& sDir, const std::wstring& sCT)
	{
		_UINT32 nRes = 0;
		std::wstring sContentTypesPath = combinePath(sDir, L"[Content_Types].xml");
		if (NSFile::CFileBinary::Exists(sContentTypesPath))
		{
			std::wstring sData;
			if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
			{
				if (std::wstring::npos == sData.find(sCT))
				{
					sData = string_replaceAll(sData, L"</Types>", sCT + L"</Types>");
					if (false == NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
						nRes = AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
		}
		return nRes;
	}

	// Replace record to [Content_Types].xml
	_UINT32 replaceContentType(const std::wstring& sDir, const std::wstring& sCTFrom, const std::wstring& sCTTo)
	{
		_UINT32 nRes = 0;
		std::wstring sContentTypesPath = combinePath(sDir, L"[Content_Types].xml");
		if (NSFile::CFileBinary::Exists(sContentTypesPath))
		{
			std::wstring sData;
			if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
			{
				sData = string_replaceAll(sData, sCTFrom, sCTTo);
				if (false == NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
					nRes = AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		return nRes;
	}

	// zip methods
	_UINT32 dir2zip(const std::wstring& sFrom, const std::wstring& sTo, bool bSorted, int method, short level, bool bDateTime)
	{
		COfficeUtils oCOfficeUtils(NULL);
		return (S_OK == oCOfficeUtils.CompressFileOrDirectory(sFrom, sTo, bSorted, method, level, bDateTime)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 zip2dir(const std::wstring& sFrom, const std::wstring& sTo)
	{
		COfficeUtils oCOfficeUtils(NULL);
		return (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}

	// compress ooxml directory to archive and encrypt if needed.
	_UINT32 dir2zipMscrypt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		if (params.hasSavePassword())
		{
			std::wstring sToMscrypt = combinePath(convertParams.m_sTempDir, L"tomscrypt.docx");
			nRes = dir2zip(sFrom, sToMscrypt);
			if (SUCCEEDED_X2T(nRes))
			{
				nRes = oox2mscrypt(sToMscrypt, sTo, params, convertParams);
			}
		}
		else
		{
			nRes = dir2zip(sFrom, sTo, true);
		}
		return nRes;
	}

	// Copy file sFromDir or sFromFile => sToDir/sToFile (compress directory, if sFromFile is empty)
	_UINT32 CopyOOXOrigin(const std::wstring& sToDir, const std::wstring& sFromDir, const std::wstring& sToFile, const std::wstring& sFromFile)
	{
		std::wstring sDstFile = combinePath(sToDir, sToFile);
		if (sFromFile.empty())
			return dir2zip(sFromDir, sDstFile);
		return NSFile::CFileBinary::Copy(sFromFile, sDstFile) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}

	namespace NSCommon
	{
		enum class OOXML_DOCUMENT_TYPE
		{
			Word  = 0,
			Sheet = 1,
			Slide = 2
		};

		enum class OOXML_DOCUMENT_SUBTYPE
		{
			Main     = 0,
			Template = 1,
			Show     = 2
		};

		// convert any format to internal format
		// 1) create temp directory with name prefix
		// 2) convert to internal format with func2dir method
		// 3) compress to zip archive
		_UINT32 format2oot(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
						   const std::wstring& prefix, CONVERT_FUNC func2dir)
		{
			std::wstring sOOTDir        = combinePath(convertParams.m_sTempDir, prefix + L"_unpacked");
			std::wstring sOOTFileEditor = combinePath(sOOTDir, L"Editor.bin");
			NSDirectory::CreateDirectory(sOOTDir);

			_UINT32 nRes = func2dir(sFrom, sOOTFileEditor, params, convertParams);

			if (SUCCEEDED_X2T(nRes))
			{
				COfficeUtils oCOfficeUtils(NULL);
				nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sOOTDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			}

			return nRes;
		}

		// convert internal format to any format
		// 1) create temp directory with name prefix
		// 2) extract internal format to directory [1]
		// 3) convert to any format with func method
		_UINT32 oot2format(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
						   const std::wstring& prefix, CONVERT_FUNC func)
		{
			std::wstring sOOTDir        = combinePath(convertParams.m_sTempDir, prefix + L"_unpacked");
			std::wstring sOOTFileEditor = combinePath(sOOTDir, L"Editor.bin");
			NSDirectory::CreateDirectory(sOOTDir);

			// unzip doct to folder
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sOOTDir, NULL, 0))
				return AVS_FILEUTILS_ERROR_CONVERT;

			return func(sOOTFileEditor, sTo, params, convertParams);
		}

		// convert ooxml format to another ooxml format
		// 1) create temp directory with name prefix
		// 2) extract sFrom format to directory [1]
		// 3) convert to ooxml format with func method
		// 4) compress ooxml format to archive
		// example: docm => docx
		_UINT32 ooxml2ooxml(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
							const std::wstring& prefix, CONVERT_FUNC func)
		{
			std::wstring sUnpackedResult = combinePath(convertParams.m_sTempDir, prefix + L"_unpacked");
			NSDirectory::CreateDirectory(sUnpackedResult);

			_UINT32 nRes = func(sFrom, sUnpackedResult, params, convertParams);
			if (SUCCEEDED_X2T(nRes))
			{
				COfficeUtils oCOfficeUtils(NULL);
				nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sUnpackedResult, sTo, true)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
			}
			return nRes;
		}

		// extract zip file to folder and replace content type
		_UINT32 ooxml2ooxml_replace_content_type(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
												 const std::wstring& sSourceCT, const std::wstring& sDestCT)
		{
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
			{
				std::wstring sContentTypesPath = combinePath(sTo, L"[Content_Types].xml");
				if (NSFile::CFileBinary::Exists(sContentTypesPath))
				{
					std::wstring sData;
					if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
					{
						sData = string_replaceAll(sData, sSourceCT, sDestCT);
						if (NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
							return 0;
					}
				}
			}
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		// remove macroses from ooxml directory
		// differences: main/template/show, document/sheet/slide
		_UINT32 ooxmlm_dir2ooxml_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
									 const OOXML_DOCUMENT_TYPE& type, const OOXML_DOCUMENT_SUBTYPE& documentType)
		{
			std::wstring sFolder = L"";
			std::wstring sMainFile = L"";
			if (OOXML_DOCUMENT_TYPE::Word == type)
			{
				sFolder = L"word";
				sMainFile = L"document";
			}
			else if (OOXML_DOCUMENT_TYPE::Sheet == type)
			{
				sFolder = L"xl";
				sMainFile = L"workbook";
			}
			else if (OOXML_DOCUMENT_TYPE::Slide == type)
			{
				sFolder = L"ppt";
				sMainFile = L"presentation";
			}

			if (sFolder.empty())
				return AVS_FILEUTILS_ERROR_CONVERT;

			std::wstring sContentTypesPath = combinePath(sTo, L"[Content_Types].xml");
			if (NSFile::CFileBinary::Exists(sContentTypesPath))
			{
				std::wstring sData;
				if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
				{
					std::wstring sCTFrom = L"";
					std::wstring sCTTo = L"";

					if (OOXML_DOCUMENT_TYPE::Word == type)
					{
						if (OOXML_DOCUMENT_SUBTYPE::Main == documentType)
						{
							sCTFrom = L"application/vnd.ms-word.document.macroEnabled.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
						}
						else
						{
							sCTFrom = L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
						}
					}
					else if (OOXML_DOCUMENT_TYPE::Sheet == type)
					{
						if (OOXML_DOCUMENT_SUBTYPE::Main == documentType)
						{
							sCTFrom = L"application/vnd.ms-excel.sheet.macroEnabled.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
						}
						else
						{
							sCTFrom = L"application/vnd.ms-excel.template.macroEnabled.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
						}
					}
					else if (OOXML_DOCUMENT_TYPE::Slide == type)
					{
						if (OOXML_DOCUMENT_SUBTYPE::Main == documentType)
						{
							sCTFrom = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
							sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
						}
						else if (OOXML_DOCUMENT_SUBTYPE::Template == documentType)
						{
							sCTFrom = L"application/vnd.ms-powerpoint.template.macroEnabled.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml";
						}
						else
						{
							sCTFrom = L"application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml";
							sCTTo   = L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml";
						}
					}

					sData = string_replaceAll(sData, sCTFrom, sCTTo);

					sCTFrom = L"<Override PartName=\"/" + sFolder + L"/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Override PartName=\"/" + sFolder + L"/vbaData.xml\" ContentType=\"application/vnd.ms-word.vbaData+xml\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if (NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
						return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			std::wstring sDocumentRelsPath = sTo + FILE_SEPARATOR_STR + sFolder + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + sMainFile + L".xml.rels";
			if (NSFile::CFileBinary::Exists(sDocumentRelsPath))
			{
				std::wstring sData;
				if (NSFile::CFileBinary::ReadAllTextUtf8(sDocumentRelsPath, sData))
				{
					size_t pos = sData.find(L"vbaProject.bin");
					if (pos != std::wstring::npos)
					{
						size_t pos1 = sData.rfind(L"<", pos);
						size_t pos2 = sData.find(L">", pos);

						if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
						{
							sData.erase(sData.begin() + pos1, sData.begin() + pos2 + 1);
						}
					}
					if (NSFile::CFileBinary::SaveToFile(sDocumentRelsPath, sData, true) == false)
						return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + sFolder + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);

			std::wstring sVbaProjectRelsPath = sTo + FILE_SEPARATOR_STR + sFolder + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"vbaProject.bin.rels";
			NSFile::CFileBinary::Remove(sVbaProjectRelsPath);

			std::wstring sVbaDataPath = sTo + FILE_SEPARATOR_STR + sFolder + FILE_SEPARATOR_STR + L"vbaData.xml";
			NSFile::CFileBinary::Remove(sVbaDataPath);
			return 0;
		}

		// extract ooxml file with macroses to folder and remove macroses
		_UINT32 ooxmlm2ooml_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
								const OOXML_DOCUMENT_TYPE& type, const OOXML_DOCUMENT_SUBTYPE& documentType)
		{
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
			{
				return ooxmlm_dir2ooxml_dir(sFrom, sTo, params, convertParams, type, documentType);
			}
			return 0;
		}

		// convert any format to ooxml format
		// 1) create temp directory with name prefix
		// 2) convert to ooxml directory [1] with funcDir method
		// 3) compress to ooxml format and encrypt it if needed
		_UINT32 format2ooxml(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
							 const std::wstring& prefix, CONVERT_FUNC func2dir, bool bIsCrypt = true)
		{
			std::wstring sOOXMLDir = combinePath(convertParams.m_sTempDir, prefix + L"_unpacked");
			NSDirectory::CreateDirectory(sOOXMLDir);

			_UINT32 hRes = func2dir(sFrom, sOOXMLDir, params, convertParams);

			if (SUCCEEDED_X2T(hRes))
			{
				if (bIsCrypt)
					hRes = dir2zipMscrypt(sOOXMLDir, sTo, params, convertParams);
				else
				{
					COfficeUtils oCOfficeUtils(NULL);
					hRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sOOXMLDir, sTo, true)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else if (AVS_ERROR_DRM == hRes)
			{
				if (!params.getDontSaveAdditional())
				{
					copyOrigin(sFrom, *params.m_sFileTo);
				}
				return AVS_FILEUTILS_ERROR_CONVERT_DRM;
			}
			else if (AVS_ERROR_PASSWORD == hRes)
			{
				return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
			}
			return hRes;
		}

		// convert ooxml file to any format
		// 1) create temp directory with name prefix
		// 2) extract ooxml to directory [1]
		// 3) convert directory to any format with func_dir2format method
		_UINT32 ooxml2format(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
							 const std::wstring& prefix, CONVERT_FUNC func_dir2format)
		{
			std::wstring sOOXMLDir = combinePath(convertParams.m_sTempDir, prefix + L"_unpacked");
			NSDirectory::CreateDirectory(sOOXMLDir);

			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sOOXMLDir, NULL, 0))
			{
				return func_dir2format(sOOXMLDir, sTo, params, convertParams);
			}
			return AVS_FILEUTILS_ERROR_CONVERT;
		}
	}
}
