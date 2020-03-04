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

#include "ASCConverters.h"
//todo убрать ошибки компиляции если переместить include ниже
#include "../../PdfWriter/OnlineOfficeBinToPdf.h"
#include "cextracttools.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../Common/3dParty/pole/pole.h"

#include "../../ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"
#include "../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"
#include "../../ASCOfficePPTXFile/ASCOfficePPTXFile.h"
#include "../../ASCOfficeRtfFile/RtfFormatLib/source/ConvertationManager.h"
#include "../../ASCOfficeDocFile/DocFormatLib/DocFormatLib.h"
#include "../../ASCOfficeTxtFile/TxtXmlFormatLib/Source/TxtXmlFile.h"
#include "../../ASCOfficePPTFile/PPTFormatLib/PPTFormatLib.h"
#include "../../ASCOfficeOdfFile/src/ConvertOO2OOX.h"
#include "../../ASCOfficeOdfFileW/source/Oox2OdfConverter/Oox2OdfConverter.h"
#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../PdfReader/Src/ErrorConstants.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../HtmlRenderer/include/HTMLRenderer3.h"
#include "../../HtmlFile/HtmlFile.h"
#include "../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ConvertXls2Xlsx.h"
#include "../../OfficeCryptReader/source/ECMACryptFile.h"

#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Directory.h"

#include <iostream>
#include <fstream>

namespace NExtractTools
{
    void initApplicationFonts(NSFonts::IApplicationFonts* pApplicationFonts, InputParams& params)
    {
		std::wstring sFontPath = params.getFontPath();
        
		if(sFontPath.empty())
            pApplicationFonts->Initialize();
        else
            pApplicationFonts->InitializeFromFolder(sFontPath);
    }
	std::wstring getExtentionByRasterFormat(int format)
	{
		std::wstring sExt;
		switch(format)
		{
			case 1:
				sExt = L".bmp";
			break;
			case 2:
				sExt = L".gif";
			break;
			case 3:
				sExt = L".jpg";
			break;
			default:
				sExt = L".png";
			break;
		}
		return sExt;
	}
	_UINT32 replaceContentType(const std::wstring &sDir, const std::wstring &sCTFrom, const std::wstring &sCTTo)
	{
        _UINT32 nRes = 0;
		std::wstring sContentTypesPath = sDir + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
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
	_UINT32 processEncryptionError(_UINT32 hRes, const std::wstring &sFrom, InputParams& params)
	{
		if (AVS_ERROR_DRM == hRes)
		{
			if(!params.getDontSaveAdditional())
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
	// docx -> bin
    _UINT32 docx2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        NSDirectory::CreateDirectory(sTempUnpackedDOCX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
		{
			//check crypt 
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
					return mscrypt2oot_bin(sFrom, sTo, sTemp, params);
				else
				{
					//вместо docx другой формат!!
				}
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;
		}

        return docx_dir2doct_bin(sTempUnpackedDOCX, sTo, params);
    }
    _UINT32 docx_dir2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCDocxSerializer.setFontDir(params.getFontPath());

        //bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
        _UINT32 nRes =  m_oCDocxSerializer.saveToFile (sTo, sFrom, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

//        sleep(3*60);

        return nRes;
    }

    // docx -> doct
    _UINT32 docx2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = docx2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

	// bin -> docx
    _UINT32 doct_bin2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        NSDirectory::CreateDirectory(sResultDocxDir);

        _UINT32 nRes = doct_bin2docx_dir(sFrom, sTo, sResultDocxDir, bFromChanges, sThemeDir, params);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
				std::wstring sCTTo;
				switch(*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM: sCTTo = _T("application/vnd.ms-word.document.macroEnabled.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM: sCTTo = _T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");break;
				}
				nRes = replaceContentType(sResultDocxDir, sCTFrom, sCTTo);
			}
		}
		if(SUCCEEDED_X2T(nRes))
		{
			if(params.hasSavePassword())
			{
				std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.docx");
				nRes = dir2zip(sResultDocxDir, sToMscrypt);
				if(SUCCEEDED_X2T(nRes))
				{
					nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
				}
			}
			else
			{
				nRes = dir2zip(sResultDocxDir, sTo);
			}
		}

        return nRes;
    }
    // bin -> docx dir
    _UINT32 doct_bin2docx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;
        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
		m_oCDocxSerializer.setFontDir(params.getFontPath());

        std::wstring sXmlOptions = _T("");
        std::wstring sThemePath;             // will be filled by 'CreateDocxFolders' method
        std::wstring sMediaPath;             // will be filled by 'CreateDocxFolders' method
        std::wstring sEmbedPath;             // will be filled by 'CreateDocxFolders' method

        m_oCDocxSerializer.CreateDocxFolders (sTo, sThemePath, sMediaPath, sEmbedPath);

        if (SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCDocxSerializer.loadFromFile (sTargetBin, sTo, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);
        return nRes;
    }

    // doct -> docx
    _UINT32 doct2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedDOCT);

        // unzip doct to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return doct_bin2docx(sTempDoctFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }
	// dotx -> docx
	_UINT32 dotx2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedDOCX);

       _UINT32 nRes = dotx2docx_dir(sFrom, sTempUnpackedDOCX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedDOCX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 dotx2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// docm -> docx
	_UINT32 docm2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedDOCX);

       _UINT32 nRes = docm2docx_dir(sFrom, sTempUnpackedDOCX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedDOCX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docm2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
		{
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
					std::wstring sCTFrom = _T("application/vnd.ms-word.document.macroEnabled.main+xml");
					std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
					sData = string_replaceAll(sData, sCTFrom, sCTTo);

 					sCTFrom = L"<Override PartName=\"/word/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

 					sCTFrom = L"<Override PartName=\"/word/vbaData.xml\" ContentType=\"application/vnd.ms-word.vbaData+xml\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sDocumentRelsPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"document.xml.rels";
			if(NSFile::CFileBinary::Exists(sDocumentRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sDocumentRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sDocumentRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);
			
			std::wstring sVbaProjectRelsPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"vbaProject.bin.rels";
			NSFile::CFileBinary::Remove(sVbaProjectRelsPath);
 			
			std::wstring sVbaDataPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"vbaData.xml";
			NSFile::CFileBinary::Remove(sVbaDataPath);
      }
       return 0;
	}
	// dotm -> docx
	_UINT32 dotm2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedDOCX);

       _UINT32 nRes = dotm2docx_dir(sFrom, sTempUnpackedDOCX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedDOCX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 dotm2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
		{
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
					std::wstring sCTFrom = _T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");
					std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
					sData = string_replaceAll(sData, sCTFrom, sCTTo);

 					sCTFrom = L"<Override PartName=\"/word/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

 					sCTFrom = L"<Override PartName=\"/word/vbaData.xml\" ContentType=\"application/vnd.ms-word.vbaData+xml\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sDocumentRelsPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"document.xml.rels";
			if(NSFile::CFileBinary::Exists(sDocumentRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sDocumentRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sDocumentRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);
			
			std::wstring sVbaProjectRelsPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"vbaProject.bin.rels";
			NSFile::CFileBinary::Remove(sVbaProjectRelsPath);
 			
			std::wstring sVbaDataPath = sTo + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"vbaData.xml";
			NSFile::CFileBinary::Remove(sVbaDataPath);
      }
       return 0;
	}
	// dotm -> docm
	_UINT32 dotm2docm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedDOCM = sTemp + FILE_SEPARATOR_STR + _T("docm_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedDOCM);

       _UINT32 nRes = dotm2docm_dir(sFrom, sTempUnpackedDOCM, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedDOCM, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 dotm2docm_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");
                   std::wstring sCTTo = _T("application/vnd.ms-word.document.macroEnabled.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
    // xslx -> bin
    _UINT32 xlsx2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        NSDirectory::CreateDirectory(sTempUnpackedXLSX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
		{
			//check crypt 
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
				{
					// test protect
					bool isOldPassword = params.hasPassword();
					const std::wstring sOldPassword = params.getPassword();
					
					if (isOldPassword) delete params.m_sPassword;
					params.m_sPassword = new std::wstring(L"VelvetSweatshop");

					_UINT32 nRes = mscrypt2oot_bin(sFrom, sTo, sTemp, params);
					if (SUCCEEDED_X2T(nRes))
					{
						return nRes;
					}
					else
					{
						delete params.m_sPassword;
						if (isOldPassword)
							params.m_sPassword = new std::wstring(sOldPassword);
						return mscrypt2oot_bin(sFrom, sTo, sTemp, params);
					}					
				}
				else
				{
					//вместо xlsx другой формат!!
				}
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;		
		}

		return xlsx_dir2xlst_bin(sTempUnpackedXLSX, sTo, params, true, sFrom);
    }
	_UINT32 xlsx_dir2xlst_bin (const std::wstring &sXlsxDir, const std::wstring &sTo, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile)
    {
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
		if (m_oCXlsxSerializer.hasPivot(sXlsxDir))
		{
			//save Editor.xlsx for pivot
			std::wstring sEditorXLSX = sToDir + FILE_SEPARATOR_STR + _T("Editor.xlsx");
			if(sXlsxFile.empty())
			{
				dir2zip(sXlsxDir, sEditorXLSX);
			}
			else
			{
				NSFile::CFileBinary::Copy(sXlsxFile, sEditorXLSX);
			}
		}

        // Save to file (from temp dir)
		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCXlsxSerializer.setFontDir(params.getFontPath());

        return m_oCXlsxSerializer.saveToFile (sTo, sXlsxDir, bXmlOptions ? params.getXmlOptions() : L"");
    }

    // xslx -> xslt
    _UINT32 xlsx2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultXlstDir);

		_UINT32 nRes = xlsx2xlst_bin(sFrom, sResultXlstFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

    // bin -> xslx
    _UINT32 xlst_bin2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        NSDirectory::CreateDirectory(sResultXlsxDir);

        _UINT32 nRes = xlst_bin2xlsx_dir(sFrom, sTo, sResultXlsxDir, bFromChanges, sThemeDir, params);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
				std::wstring sCTTo;
				switch(*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM: sCTTo = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM: sCTTo = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");break;
				}
				nRes = replaceContentType(sResultXlsxDir, sCTFrom, sCTTo);
			}
		}
		if(SUCCEEDED_X2T(nRes))
		{
			if(params.hasSavePassword())
			{
				std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.xlsx");
				nRes = dir2zip(sResultXlsxDir, sToMscrypt);
				if(SUCCEEDED_X2T(nRes))
				{
					nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
				}
			}
			else
			{
				nRes = dir2zip(sResultXlsxDir, sTo);
			}
		}
        return nRes;
    }
    _UINT32 xlst_bin2xlsx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;

        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		m_oCXlsxSerializer.setFontDir(params.getFontPath());

        std::wstring sXmlOptions = _T("");
        std::wstring sMediaPath;             // will be filled by 'CreateXlsxFolders' method
        std::wstring sEmbedPath;             // will be filled by 'CreateXlsxFolders' method

        m_oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, sTo, sMediaPath, sEmbedPath);

        if(SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCXlsxSerializer.loadFromFile (sTargetBin, sTo, sXmlOptions, sMediaPath, sEmbedPath);
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);
        return nRes;
    }

    // xslt -> xslx
    _UINT32 xlst2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedXLST);

        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return xlst_bin2xlsx(sTempXlstFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }
	// xltx -> xlsx
	_UINT32 xltx2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedXLSX);

       _UINT32 nRes = xltx2xlsx_dir(sFrom, sTempUnpackedXLSX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 xltx2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// xlsm -> xlsx
	_UINT32 xlsm2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedXLSX);

       _UINT32 nRes = xlsm2xlsx_dir(sFrom, sTempUnpackedXLSX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 xlsm2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = L"application/vnd.ms-excel.sheet.macroEnabled.main+xml";
                   std::wstring sCTTo = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

					sCTFrom = L"<Override PartName=\"/xl/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");
					
				   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
                   {
                       return AVS_FILEUTILS_ERROR_CONVERT;
                   }
				}
			}
			std::wstring sWorkbookRelsPath = sTo + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"workbook.xml.rels";
			if(NSFile::CFileBinary::Exists(sWorkbookRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sWorkbookRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sWorkbookRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);
		}
		return 0;
	}
	// xltm -> xlsx
	_UINT32 xltm2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedXLSX);

       _UINT32 nRes = xltm2xlsx_dir(sFrom, sTempUnpackedXLSX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 xltm2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = L"application/vnd.ms-excel.template.macroEnabled.main+xml";
                   std::wstring sCTTo = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

					sCTFrom = L"<Override PartName=\"/xl/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");
					
				   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
                   {
                       return AVS_FILEUTILS_ERROR_CONVERT;
                   }
				}
			}
			std::wstring sWorkbookRelsPath = sTo + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"workbook.xml.rels";
			if(NSFile::CFileBinary::Exists(sWorkbookRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sWorkbookRelsPath, sData))
               {
					size_t pos = sData.find(L"vbaProject.bin");
					if (pos != std::wstring::npos)
					{
						size_t pos1 = (int)sData.rfind(L"<", pos);
						size_t pos2 = (int)sData.find(L">", pos);

						if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
						{
							sData.erase(sData.begin() + pos1, sData.begin() + pos2 + 1);
						}
					}							
					if(NSFile::CFileBinary::SaveToFile(sWorkbookRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);
		}
		return 0;
	}
	// xltm -> xlsm
	_UINT32 xltm2xlsm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedXLSM = sTemp + FILE_SEPARATOR_STR + _T("xlsm_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedXLSM);

       _UINT32 nRes = xltm2xlsm_dir(sFrom, sTempUnpackedXLSM, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSM, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 xltm2xlsm_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
    // pptx -> bin
    _UINT32 pptx2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // unzip pptx to temp folder
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked")+ FILE_SEPARATOR_STR;  // leading slash is very important!

        NSDirectory::CreateDirectory(sTempUnpackedPPTX);

        // unzip pptx to folder
        COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
		{
			//check crypt 
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
					return mscrypt2oot_bin(sFrom, sTo, sTemp, params);
				//вместо pptx другой формат!!
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;
		}

        return pptx_dir2pptt_bin(sTempUnpackedPPTX, sTo, sTemp, params);
	}
    _UINT32 pptx_dir2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // convert unzipped pptx to unzipped pptt
        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

        _UINT32 nRes = 0;

        if (pptx_file)
        {
			pptx_file->SetIsNoBase64(params.getIsNoBase64());
            pptx_file->put_TempDirectory(sTemp);
            pptx_file->SetFontDir (params.getFontPath());
            nRes = (S_OK == pptx_file->OpenFileToPPTY (sFrom, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

            delete pptx_file;
        }

        return nRes;
	}
    // pptx -> pptt
    _UINT32 pptx2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultPpttDir);

        _UINT32 nRes = pptx2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            // zip pptt folder to output file
			COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
        return nRes;
    }

    // bin -> pptx
    _UINT32 pptt_bin2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        NSDirectory::CreateDirectory(sResultPptxDir);

		_UINT32 nRes = pptt_bin2pptx_dir(sFrom, sTo, sResultPptxDir, bFromChanges, sThemeDir, params);
		
		if(SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == *params.m_nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == *params.m_nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == *params.m_nFormatTo)
			{
				std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
				std::wstring sCTTo;
				switch(*params.m_nFormatTo)
				{
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM: sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX: sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM: sCTTo = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM: sCTTo = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");break;
				}
				nRes = replaceContentType(sResultPptxDir, sCTFrom, sCTTo);
			}
		}
		if(SUCCEEDED_X2T(nRes))
		{
			if(params.hasSavePassword())
			{
				std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.pptx");
				nRes = dir2zip(sResultPptxDir, sToMscrypt);
				if(SUCCEEDED_X2T(nRes))
				{
					nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
				}
			}
			else
			{
				nRes = dir2zip(sResultPptxDir, sTo);
			}
		}
        return nRes;
	}
    _UINT32 pptt_bin2pptx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;

        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

        HRESULT hr = S_OK;

        if (pptx_file)
        {
			pptx_file->SetIsNoBase64(params.getIsNoBase64());
            pptx_file->SetFontDir(params.getFontPath());
            nRes = (S_OK == pptx_file->ConvertPPTYToPPTX(sTargetBin, sTo, sThemeDir)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

            delete pptx_file;
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);

        return nRes;
	}
	// pptt -> pptx
    _UINT32 pptt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // unzip pptt to temp folder
        std::wstring sTempUnpackedPPTT = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sTempUnpackedPPTT + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedPPTT);

        // unzip pptt to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return pptt_bin2pptx(sTempPpttFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }
    // zip dir
    _UINT32 dir2zip (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.CompressFileOrDirectory(sFrom, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // unzip dir
    _UINT32 zip2dir (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // csv -> xslt
    _UINT32 csv2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sCSV = sFrom;
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultXlstDir);

        COfficeUtils oCOfficeUtils(NULL);
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCXlsxSerializer.setFontDir(params.getFontPath());

        _UINT32 nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sCSV, params.getXmlOptions());

        if (SUCCEEDED_X2T(nRes))
        {
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

    // csv -> xslx
    _UINT32 csv2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sCSV = sFrom;
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedXLSX);
        NSDirectory::CreateDirectory(sResultXlstDir);

        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCXlsxSerializer.setFontDir(params.getFontPath());

        COfficeUtils oCOfficeUtils(NULL);

        std::wstring sMediaPath;
        std::wstring sEmbedPath;

        _UINT32 nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sCSV, params.getXmlOptions());
        if (SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCXlsxSerializer.loadFromFile(sResultXlstFileEditor, sTempUnpackedXLSX, params.getXmlOptions(), sMediaPath, sEmbedPath);
            if (SUCCEEDED_X2T(nRes))
            {
                nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            }
        }

        return nRes;
    }
    _UINT32 csv2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCXlsxSerializer.setFontDir(params.getFontPath());

        return m_oCXlsxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions());
    }
    // xlst -> csv
	_UINT32 xlst2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sCSV = sTo;
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");
     
        NSDirectory::CreateDirectory(sTempUnpackedXLST);

        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;


        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCXlsxSerializer.setFontDir(params.getFontPath());

        std::wstring sMediaPath;
        std::wstring sEmbedPath;

        return m_oCXlsxSerializer.loadFromFile (sTempXlstFileEditor, sCSV, params.getXmlOptions(), sMediaPath, sEmbedPath);
    }
	// xslx -> csv
	_UINT32 xlsx2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sCSV = sTo;
       std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
       std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
       std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sTempUnpackedXLSX);
       NSDirectory::CreateDirectory(sResultXlstDir);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

       // Save to file (from temp dir)
       BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

	   m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
       m_oCXlsxSerializer.setFontDir(params.getFontPath());

       std::wstring sXMLOptions = _T("<XmlOptions><fileOptions fileType=\"1\"/></XmlOptions>");
       _UINT32 nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sTempUnpackedXLSX, sXMLOptions);
       if (SUCCEEDED_X2T(nRes))
       {
           std::wstring sMediaPath;
           std::wstring sEmbedPath;

		   sXMLOptions = _T("<XmlOptions><fileOptions fileType=\"2\"/></XmlOptions>");

           nRes = m_oCXlsxSerializer.loadFromFile (sResultXlstFileEditor, sCSV, sXMLOptions, sMediaPath, sEmbedPath);
       }

       return nRes;
	}
	_UINT32 xlst_bin2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, InputParams& params)
	{
       _UINT32 nRes = 0;

       std::wstring sTargetBin;
       if(bFromChanges)
           nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
       else
           sTargetBin = sFrom;

       if(SUCCEEDED_X2T(nRes))
       {
           //todo сделать отдельный метод для сохранения в csv
           // Save to file (from temp dir)
           BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		   m_oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
           m_oCXlsxSerializer.setFontDir(params.getFontPath());

           std::wstring sToTemp = sTemp + FILE_SEPARATOR_STR + _T("output.csv");
           std::wstring sMediaPath;             // will be filled by 'CreateXlsxFolders' method
           std::wstring sEmbedPath;             // will be filled by 'CreateXlsxFolders' method
           std::wstring sXmlOptions  = params.getXmlOptions();

           m_oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, sTemp, sMediaPath, sEmbedPath);
           
           nRes = m_oCXlsxSerializer.loadFromFile(sTargetBin, sToTemp, sXmlOptions, sMediaPath, sEmbedPath);

           //пишем в Temp и копируем, чтобы не возникало лишних файлов рядом с sTo, а лучше перейти на отдельный метод
           if(SUCCEEDED_X2T(nRes))
           {
               NSFile::CFileBinary::Copy(sToTemp, sTo);
           }
       }
       return nRes;
   }
	// bin -> pdf
	_UINT32 bin2pdf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
	{
        NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
        initApplicationFonts(pApplicationFonts, params);
        
		CPdfRenderer pdfWriter(pApplicationFonts, params.getIsPDFA());
		
		pdfWriter.SetTempFolder(sTemp);
		pdfWriter.SetThemesPlace(sThemeDir);
		
		std::wstring documentID = params.getDocumentID();
		if (false == documentID.empty())
			pdfWriter.SetDocumentID(documentID);

		std::wstring password = params.getSavePassword();
		if (false == password.empty())
			pdfWriter.SetPassword(password);
		
		int nReg = (bPaid == false) ? 0 : 1;
        _UINT32 nRet = 0;
		if (params.getIsNoBase64())
		{
            nRet = S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
            nRet = S_OK == pdfWriter.OnlineWordToPdf(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
        RELEASEOBJECT(pApplicationFonts);
        return nRet;
	}
	_UINT32 bin2image (const std::wstring &sTFileDir, BYTE* pBuffer, LONG lBufferLen, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
	{
		_UINT32 nRes = 0;
        NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
        initApplicationFonts(pApplicationFonts, params);
		NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
        imageWriter.SetHtmlPlace(sTFileDir);
        imageWriter.SetThemesPlace(sThemeDir);
        imageWriter.SetTempDir(sTemp);
        imageWriter.SetApplication(pApplicationFonts);
		if(NULL != params.m_oThumbnail)
		{
			InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
			if(NULL != oThumbnail->format)
			{
                imageWriter.SetRasterFormat(*oThumbnail->format);
			}
			if(NULL != oThumbnail->aspect)
			{
                imageWriter.SetSaveType(*oThumbnail->aspect);
			}
			if(NULL != oThumbnail->first)
			{
                imageWriter.SetIsOnlyFirst(*oThumbnail->first);
			}
			if(NULL != oThumbnail->width)
			{
                imageWriter.SetRasterW(*oThumbnail->width);
			}
			if(NULL != oThumbnail->height)
			{
                imageWriter.SetRasterH(*oThumbnail->height);
			}
		}
		std::wstring sThumbnailDir;
        if(imageWriter.GetIsOnlyFirst())
		{
            imageWriter.SetFileName(sTo);
		}
		else
		{
			sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
			NSDirectory::CreateDirectory(sThumbnailDir);
            imageWriter.SetFileName(sThumbnailDir + FILE_SEPARATOR_STR + L"image" + getExtentionByRasterFormat(imageWriter.GetRasterFormat()));
		}
		nRes = imageWriter.ConvertBuffer(pBuffer, lBufferLen) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        if(!imageWriter.GetIsOnlyFirst())
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
        RELEASEOBJECT(pApplicationFonts);
		return nRes;
	}
	_UINT32 bin2imageBase64 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
	{
		_UINT32 nRes = 0;
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(sFrom))
			return AVS_FILEUTILS_ERROR_CONVERT;

		DWORD dwFileSize = oFile.GetFileSize();
		BYTE* pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		DWORD dwReaded;
		oFile.ReadFile(pFileContent, dwFileSize, dwReaded);
		oFile.CloseFile();

		int   nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
		BYTE* pBuffer    = new BYTE[nBufferLen];
		if (!pBuffer)
		{
			RELEASEARRAYOBJECTS(pFileContent);
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		if (NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen))
		{
			std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
			nRes = bin2image(sTFileDir, pBuffer, nBufferLen, sTo, sTemp, sThemeDir, params);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}

		RELEASEARRAYOBJECTS(pBuffer);
		RELEASEARRAYOBJECTS(pFileContent);

		return nRes;
	}
   //doct_bin -> pdf
	_UINT32 doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
       _UINT32 nRes = 0;
       NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
       std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
       std::wstring sImagesDirectory = sTFileDir + FILE_SEPARATOR_STR + _T("media");
       std::wstring sPdfBinFile = sTFileDir + FILE_SEPARATOR_STR + _T("pdf.bin");
	   NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
       std::wstring sXml = getDoctXml(eFromType, eToType, sTFileDir, sPdfBinFile, sImagesDirectory, sThemeDir, -1, _T(""), params);
       std::wstring sResult;
       bool bRes = oDoctRenderer.Execute(sXml, sResult);
       if (-1 != sResult.find(_T("error")))
       {
           std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       else
       {
           NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
           initApplicationFonts(pApplicationFonts, params);
          
		   CPdfRenderer pdfWriter(pApplicationFonts, params.getIsPDFA());
           
			pdfWriter.SetTempFolder(sTemp);
			pdfWriter.SetThemesPlace(sThemeDir);
			
			std::wstring documentID = params.getDocumentID();
			if (false == documentID.empty())
				pdfWriter.SetDocumentID(documentID);

			std::wstring password = params.getSavePassword();
			if (false == password.empty())
				pdfWriter.SetPassword(password);
          
		   int nReg = (bPaid == false) ? 0 : 1;
           nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sPdfBinFile, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
           RELEASEOBJECT(pApplicationFonts);
       }
       //удаляем sPdfBinFile, потому что он не в Temp
       if (NSFile::CFileBinary::Exists(sPdfBinFile))
           NSFile::CFileBinary::Remove(sPdfBinFile);
       return nRes;
   }

	//doct_bin -> image
	_UINT32 doct_bin2image(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
        std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sTFileDir + FILE_SEPARATOR_STR + _T("media");
		std::wstring sPdfBinFile = sTFileDir + FILE_SEPARATOR_STR + _T("pdf.bin");
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
		std::wstring sXml = getDoctXml(eFromType, eToType, sTFileDir, sPdfBinFile, sImagesDirectory, sThemeDir, -1, _T(""), params);
		std::wstring sResult;
		bool bRes = oDoctRenderer.Execute(sXml, sResult);
		if (-1 != sResult.find(_T("error")))
		{
			std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			BYTE* pData = NULL;
			DWORD nBytesCount;
			if (NSFile::CFileBinary::ReadAllBytes(sPdfBinFile, &pData, nBytesCount))
			{
				nRes = 0 == bin2image(sTFileDir, pData, nBytesCount, sTo, sTemp, sThemeDir, params) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
				RELEASEARRAYOBJECTS(pData);
				}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
			}
			}
		//delete sPdfBinFile, because it is not in Temp
		if (NSFile::CFileBinary::Exists(sPdfBinFile))
			NSFile::CFileBinary::Remove(sPdfBinFile);
		return nRes;
	}

	// ppsx -> pptx
	_UINT32 ppsx2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       _UINT32 nRes = ppsx2pptx_dir(sFrom, sTempUnpackedPPTX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 ppsx2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
	// pptm -> pptx
	_UINT32 pptm2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       _UINT32 nRes = pptm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 pptm2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

 					sCTFrom = L"<Override PartName=\"/ppt/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
				}
			}
			std::wstring sPresentationRelsPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"presentation.xml.rels";
			if(NSFile::CFileBinary::Exists(sPresentationRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sPresentationRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sPresentationRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);

       }
       return 0;
   }
	// potm -> pptx
	_UINT32 potm2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       _UINT32 nRes = potm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 potm2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

 					sCTFrom = L"<Override PartName=\"/ppt/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
				}
			}
			std::wstring sPresentationRelsPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"presentation.xml.rels";
			if(NSFile::CFileBinary::Exists(sPresentationRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sPresentationRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sPresentationRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);

       }
       return 0;
   }
	// ppsm -> pptx
	_UINT32 ppsm2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       _UINT32 nRes = ppsm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 ppsm2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

 					sCTFrom = L"<Override PartName=\"/ppt/vbaProject.bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					sCTFrom = L"<Default Extension=\"bin\" ContentType=\"application/vnd.ms-office.vbaProject\"/>";
					sData = string_replaceAll(sData, sCTFrom, L"");

					if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
				}
			}
			std::wstring sPresentationRelsPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"_rels" + FILE_SEPARATOR_STR + L"presentation.xml.rels";
			if(NSFile::CFileBinary::Exists(sPresentationRelsPath))
			{
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sPresentationRelsPath, sData))
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
					if(NSFile::CFileBinary::SaveToFile(sPresentationRelsPath, sData, true) == false)
					{
						return AVS_FILEUTILS_ERROR_CONVERT;
					}
			   }
			}
			std::wstring sVbaProjectPath = sTo + FILE_SEPARATOR_STR + L"ppt" + FILE_SEPARATOR_STR + L"vbaProject.bin";
			NSFile::CFileBinary::Remove(sVbaProjectPath);

       }
       return 0;
   }
	// potx -> pptx
	_UINT32 potx2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       _UINT32 nRes = potx2pptx_dir(sFrom, sTempUnpackedPPTX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTX, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 potx2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");
                   std::wstring sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// potm -> pptm
	_UINT32 potm2pptm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTM = sTemp + FILE_SEPARATOR_STR + _T("pptm_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTM);

       _UINT32 nRes = potm2pptm_dir(sFrom, sTempUnpackedPPTM, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTM, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 potm2pptm_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// ppsm -> pptm
	_UINT32 ppsm2pptm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
	   std::wstring sTempUnpackedPPTM = sTemp + FILE_SEPARATOR_STR + _T("pptm_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedPPTM);

       _UINT32 nRes = ppsm2pptm_dir(sFrom, sTempUnpackedPPTM, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPTM, sTo, true))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 ppsm2pptm_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
	{
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
       {
           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
           if(NSFile::CFileBinary::Exists(sContentTypesPath))
           {
               std::wstring sData;
               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
               {
                   std::wstring sCTFrom = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");
                   std::wstring sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");

                   sData = string_replaceAll(sData, sCTFrom, sCTTo);

                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
                   {
                       return 0;
                   }
               }
           }
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// ppt -> pptx
	_UINT32 ppt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");

       NSDirectory::CreateDirectory(sResultPptxDir);

       _UINT32 nRes = ppt2pptx_dir(sFrom, sResultPptxDir, sTemp, params);

		nRes = processEncryptionError(nRes, sFrom, params);
		if(SUCCEEDED_X2T(nRes))
		{
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultPptxDir, sTo, true))
               return 0;
		}	
		return nRes;
	}
	_UINT32 ppt2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		COfficePPTFile pptFile;

		pptFile.put_TempDirectory(sTemp);
	   
		bool bMacros = false;
		long nRes = pptFile.LoadFromFile(sFrom, sTo, params.getPassword(), bMacros);
		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}
	// ppt -> pptm
	_UINT32 ppt2pptm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");

       NSDirectory::CreateDirectory(sResultPptxDir);

       _UINT32 nRes = ppt2pptm_dir(sFrom, sResultPptxDir, sTemp, params);

       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultPptxDir, sTo, true))
               return 0;
		}
		return nRes;
	}
	_UINT32 ppt2pptm_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       COfficePPTFile pptFile;

       pptFile.put_TempDirectory(sTemp);
	   
		bool bMacros = true;
		_UINT32 nRes = pptFile.LoadFromFile(sFrom, sTo, params.getPassword(), bMacros);
		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}
	// ppt -> pptt
	_UINT32 ppt2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
       std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultPpttDir);

       _UINT32 nRes = ppt2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
 		
		if (SUCCEEDED_X2T(nRes))
		{
           // zip pptt folder to output file
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
	// ppt -> pptt_bin
	_UINT32 ppt2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       // unzip pptx to temp folder
       std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked")+ FILE_SEPARATOR_STR;  // leading slash is very important!

       NSDirectory::CreateDirectory(sTempUnpackedPPTX);

       COfficePPTFile pptFile;

       pptFile.put_TempDirectory(sTemp);

	   bool bMacros = true;
       _UINT32 nRes = pptFile.LoadFromFile(sFrom, sTempUnpackedPPTX, params.getPassword(), bMacros);
		
		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
        {
		  // convert unzipped pptx to unzipped pptt
		   CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

		   if (pptx_file)
		   {
			   pptx_file->SetFontDir (params.getFontPath());
			   nRes = (S_OK == pptx_file->OpenFileToPPTY (sTempUnpackedPPTX, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

			   delete pptx_file;
		   }
		   return nRes;

        }
        return nRes;
   }

	// pptx -> odp
	_UINT32 pptx2odp (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
	{
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedPPTX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
        {
            return pptx_dir2odp(sTempUnpackedPPTX, sTo, sTemp, params, false); //add template ???
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// pptx_dir -> odp
	_UINT32 pptx_dir2odp (const std::wstring &sPptxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
	{
		std::wstring sTempUnpackedODP = sTemp + FILE_SEPARATOR_STR + _T("odp_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedODP);

		Oox2Odf::Converter converter(sPptxDir, _T("presentation"), params.getFontPath(), bTemplate);

       _UINT32 nRes = 0;
		try
		{
			std::wstring password	= params.getSavePassword();
			std::wstring documentID = params.getDocumentID();
			
			converter.convert();
			converter.write(sTempUnpackedODP, sTemp, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODP, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		catch(...)
		{
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	// rtf -> docx
	_UINT32 rtf2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       NSDirectory::CreateDirectory(sResultDocxDir);
       _UINT32 nRes = rtf2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
	_UINT32 rtf2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        RtfConvertationManager rtfConvert;
        
		rtfConvert.m_sTempFolder = sTemp;
		rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
       
		return 0 == rtfConvert.ConvertRtfToOOX(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }

	// rtf -> doct
	_UINT32 rtf2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       // Extract docx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

       _UINT32 nRes = rtf2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

	// rtf -> doct_bin
	_UINT32 rtf2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        NSDirectory::CreateDirectory(sResultDocxDir);
        RtfConvertationManager rtfConvert;

        rtfConvert.m_sTempFolder = sTemp;

        if ( rtfConvert.ConvertRtfToOOX(sFrom, sResultDocxDir) == 0)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            m_oCDocxSerializer.setFontDir(params.getFontPath());

            std::wstring sXmlOptions;
            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, sXmlOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }

	// docx -> rtf
	_UINT32 docx2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
		{
            return docx_dir2rtf(sTempUnpackedDOCX, sTo, sTemp, params);
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx_dir2rtf(const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       //docx folder to rtf
       RtfConvertationManager rtfConvert;

       rtfConvert.m_sTempFolder = sTemp;

       if (rtfConvert.ConvertOOXToRtf(sTo, sDocxDir) == 0)
           return 0;
       return AVS_FILEUTILS_ERROR_CONVERT;
   }

	// doc -> docx
	_UINT32 doc2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       NSDirectory::CreateDirectory(sResultDocxDir);

       _UINT32 hRes = doc2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(hRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
               return 0;
       }
       else if (AVS_ERROR_DRM == hRes)
       {
           if(!params.getDontSaveAdditional())
           {
               copyOrigin(sFrom, *params.m_sFileTo);
           }
           return AVS_FILEUTILS_ERROR_CONVERT_DRM;
       }
       else if (AVS_ERROR_PASSWORD == hRes)
       {
           return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 doc2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        COfficeDocFile docFile;
		
		docFile.m_sTempFolder = sTemp;		
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
	
		bool bMacros = false;

		_UINT32 hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), bMacros, NULL);
		if (AVS_ERROR_DRM == hRes)
		{
			if(!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, *params.m_sFileTo);
			}
			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		else if (AVS_ERROR_PASSWORD == hRes)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}
		return 0 == hRes ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }

	// doc -> docm
	_UINT32 doc2docm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       NSDirectory::CreateDirectory(sResultDocxDir);

       _UINT32 hRes = doc2docm_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(hRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
               return 0;
       }
       else if (AVS_ERROR_DRM == hRes)
       {
           if(!params.getDontSaveAdditional())
           {
               copyOrigin(sFrom, *params.m_sFileTo);
           }
           return AVS_FILEUTILS_ERROR_CONVERT_DRM;
       }
       else if (AVS_ERROR_PASSWORD == hRes)
       {
           return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 doc2docm_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        COfficeDocFile docFile;
		docFile.m_sTempFolder = sTemp;
		
		bool bMacros = true;

		_UINT32 hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), bMacros, NULL);
		if (AVS_ERROR_DRM == hRes)
		{
			if(!params.getDontSaveAdditional())
			{
				copyOrigin(sFrom, *params.m_sFileTo);
			}
			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
		}
		else if (AVS_ERROR_PASSWORD == hRes)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}
		return 0 == hRes ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }

	// doc -> doct
	_UINT32 doc2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       // Extract docx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

       _UINT32 nRes = doc2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
	}

	// doc -> doct_bin
	_UINT32 doc2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        NSDirectory::CreateDirectory(sResultDocxDir);

        COfficeDocFile docFile;
		docFile.m_sTempFolder = sTemp;

		bool bMacros = true;

		_UINT32 nRes = docFile.LoadFromFile( sFrom, sResultDocxDir, params.getPassword(), bMacros, NULL);
 		
		nRes = processEncryptionError(nRes, sFrom, params);
        if (SUCCEEDED_X2T(nRes))
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            m_oCDocxSerializer.setFontDir(params.getFontPath());

			std::wstring xml_options = params.getXmlOptions();

            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, xml_options) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;
        }
        return nRes;
	}
	_UINT32 docx_dir2doc (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       return AVS_FILEUTILS_ERROR_CONVERT;
       COfficeDocFile docFile;
       return /*S_OK == docFile.SaveToFile(sTo, sDocxDir, NULL) ? 0 : */AVS_FILEUTILS_ERROR_CONVERT;
	}

	// doct -> rtf
	_UINT32 doct2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
	{
       // Extract docx to temp directory
       std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sTempUnpackedDOCT);

       // unzip doct to folder
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

       return doct_bin2rtf(sTempDoctFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
   }

	// bin -> rtf
	_UINT32 doct_bin2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
   {
       _UINT32 nRes = 0;
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + L"docx_unpacked";

       NSDirectory::CreateDirectory(sResultDocxDir);

       std::wstring sTargetBin;
       if(bFromChanges)
           nRes = apply_changes(sFrom, _T(""), NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
       else
       sTargetBin = sFrom;

       BinDocxRW::CDocxSerializer m_oCDocxSerializer;

        m_oCDocxSerializer.setFontDir(params.getFontPath());

       std::wstring sXmlOptions = _T("");
       std::wstring sThemePath;             // will be filled by 'CreateDocxFolders' method
       std::wstring sMediaPath;             // will be filled by 'CreateDocxFolders' method
       std::wstring sEmbedPath;             // will be filled by 'CreateDocxFolders' method

       m_oCDocxSerializer.CreateDocxFolders (sResultDocxDir, sThemePath, sMediaPath, sEmbedPath);

       if (SUCCEEDED_X2T(nRes))
       {
           nRes = m_oCDocxSerializer.loadFromFile (sTargetBin, sResultDocxDir, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
           if (SUCCEEDED_X2T(nRes))
           {
               //docx folder to rtf
               RtfConvertationManager rtfConvert;

               rtfConvert.m_sTempFolder = sTemp;

               nRes = rtfConvert.ConvertOOXToRtf(sTo, sResultDocxDir);
           }
       }
       //удаляем EditorWithChanges, потому что он не в Temp
       if (sFrom != sTargetBin)
           NSFile::CFileBinary::Remove(sTargetBin);
       return nRes;
   }
	// txt -> docx
	_UINT32 txt2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       NSDirectory::CreateDirectory(sResultDocxDir);
       _UINT32 nRes = txt2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 txt2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       CTxtXmlFile txtFile;
	   
	   std::wstring xml_options = params.getXmlOptions();

	   return txtFile.txt_LoadFromFile(sFrom, sTo, xml_options);
   }
	// txt -> doct
	_UINT32 txt2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

       _UINT32 nRes = txt2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

	// txt -> doct_bin
	_UINT32 txt2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        NSDirectory::CreateDirectory(sResultDocxDir);

        CTxtXmlFile txtFile;

       // txtFile.m_sTempFolder = sTemp);

		if ( txtFile.txt_LoadFromFile(sFrom, sResultDocxDir, params.getXmlOptions()) == 0)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            m_oCDocxSerializer.setFontDir(params.getFontPath());

            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx_dir2txt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		CTxtXmlFile txtFile;

		return txtFile.txt_SaveToFile(sTo, sDocxDir, params.getXmlOptions());
	}
	//odf
	_UINT32 odf2oot(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

       _UINT32 nRes = odf2oot_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

	_UINT32 odf2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
   {
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");

       NSDirectory::CreateDirectory(sTempUnpackedOdf);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return -3;

       NSDirectory::CreateDirectory(sTempUnpackedOox);

		_UINT32 nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTempUnpackedOox, params.getFontPath(), sTemp, params.getPassword(), NULL);
		nRes = processEncryptionError(nRes, sFrom, params);
		if(SUCCEEDED_X2T(nRes))
		{
           BinDocxRW::CDocxSerializer m_oCDocxSerializer;

           m_oCDocxSerializer.setFontDir(params.getFontPath());

           nRes =  m_oCDocxSerializer.saveToFile (sTo, sTempUnpackedOox, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 otf2odf(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOdf);

	   COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;;

		_UINT32 nRes = ConvertOTF2ODF(sTempUnpackedOdf);
		if(SUCCEEDED_X2T(nRes))
		{
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOdf, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}

	_UINT32 odf2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOox);

       _UINT32 nRes = odf2oox_dir(sFrom, sTempUnpackedOox, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOox, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 odf2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOdf);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;;

	   _UINT32 nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTo, params.getFontPath(), sTemp, params.getPassword(), NULL);
	   nRes = processEncryptionError(nRes, sFrom, params);
	   return nRes;
	}
	//odf flat
	_UINT32 odf_flat2oot(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

       _UINT32 nRes = odf_flat2oot_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
	}

	_UINT32 odf_flat2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");

       NSDirectory::CreateDirectory(sTempUnpackedOox);

		_UINT32 nRes = ConvertODF2OOXml(sFrom, sTempUnpackedOox, params.getFontPath(), sTemp, params.getPassword(), NULL);
		nRes = processEncryptionError(nRes, sFrom, params);
		if(SUCCEEDED_X2T(nRes))
		{
           BinDocxRW::CDocxSerializer m_oCDocxSerializer;

           m_oCDocxSerializer.setFontDir(params.getFontPath());

           nRes =  m_oCDocxSerializer.saveToFile (sTo, sTempUnpackedOox, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}

       return nRes;
	}
	_UINT32 odf_flat2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOox);

       _UINT32 nRes = odf_flat2oox_dir(sFrom, sTempUnpackedOox, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOox, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
	}
	_UINT32 odf_flat2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		_UINT32 nRes = ConvertODF2OOXml(sFrom, sTo, params.getFontPath(), sTemp, params.getPassword(), NULL);
		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}
	// docx -> odt
	_UINT32 docx2odt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
   {
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedDOCX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
        {
            return docx_dir2odt(sTempUnpackedDOCX, sTo, sTemp, params, false); //add Template ????
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }
	// docx dir -> odt
	_UINT32 docx_dir2odt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
   {
       std::wstring sTempUnpackedODT = sTemp + FILE_SEPARATOR_STR + L"odt_unpacked";
       NSDirectory::CreateDirectory(sTempUnpackedODT);

	   Oox2Odf::Converter converter(sDocxDir, L"text", params.getFontPath(), bTemplate);

       _UINT32 nRes = 0;
       try
       {
			std::wstring password	= params.getSavePassword();			
			std::wstring documentID = params.getDocumentID();

			converter.convert();
			converter.write(sTempUnpackedODT, sTemp, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODT, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       }
	   catch(...)
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
	// xlsx -> ods
	_UINT32 xlsx2ods (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
   {
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + L"xlsx_unpacked";

        NSDirectory::CreateDirectory(sTempUnpackedXLSX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
        {
            return xlsx_dir2ods(sTempUnpackedXLSX, sTo, sTemp, params, false); //add Template ???
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }

	_UINT32 xlsx_dir2ods (const std::wstring &sXlsxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
   {
       std::wstring sTempUnpackedODS = sTemp + FILE_SEPARATOR_STR + L"ods_unpacked";
       NSDirectory::CreateDirectory(sTempUnpackedODS);
	
	   Oox2Odf::Converter converter(sXlsxDir, L"spreadsheet", params.getFontPath(), bTemplate);
     
	   _UINT32 nRes = 0;
       try
       {
			std::wstring password	= params.getSavePassword();
			std::wstring documentID = params.getDocumentID();
			
			converter.convert();
			converter.write(sTempUnpackedODS, sTemp, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODS, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       }
	   catch(...)
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
	}

	_UINT32 mscrypt2oot (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultOotDir			= sTemp			+ FILE_SEPARATOR_STR + _T("oot_unpacked");
        std::wstring sResultOotFileEditor	= sResultOotDir	+ FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultOotDir);

        _UINT32 nRes = mscrypt2oot_bin(sFrom, sResultOotFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultOotDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

		return nRes;
	}
	_UINT32 mscrypt2oox	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring password = params.getPassword();

		ECMACryptFile cryptReader;
		bool bDataIntegrity = false;

        if (cryptReader.DecryptOfficeFile(sFrom, sTo, password, bDataIntegrity) == false)
		{
             if (password.empty())	return AVS_FILEUTILS_ERROR_CONVERT_DRM;
            else					return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}

		if (bDataIntegrity == false)
		{
			//было несанкционированое вешательство в файл
		}

		return 0;
	}
	_UINT32 mscrypt2oot_bin	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		//decrypt to temp file
         std::wstring password = params.getPassword();
       std::wstring sResultDecryptFile = sTemp	+ FILE_SEPARATOR_STR + L"uncrypt_file.oox";

		ECMACryptFile cryptReader;
		bool bDataIntegrity = false;

		if (cryptReader.DecryptOfficeFile(sFrom, sResultDecryptFile, password, bDataIntegrity) == false)
		{
             if (password.empty())	return AVS_FILEUTILS_ERROR_CONVERT_DRM;
            else					return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}

		if (bDataIntegrity == false)
		{
			//было несанкционированое вешательство в файл
		}

		COfficeFileFormatChecker OfficeFileFormatChecker;

		if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
        {
            switch (OfficeFileFormatChecker.nFileType)
			{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
				{
					return docx2doct_bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
				{
					const std::wstring & sXmlOptions = params.getXmlOptions();
					return xlsx2xlst_bin(sResultDecryptFile, sTo, sTemp, params);
                }break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
				{
					return pptx2pptt_bin(sResultDecryptFile, sTo, sTemp,params);
				}break;
			}
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 oox2mscrypt	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring password	= params.getSavePassword();
		std::wstring documentID = params.getDocumentID();

		ECMACryptFile cryptReader;

        if (cryptReader.EncryptOfficeFile(sFrom, sTo, password, documentID) == false)
		{
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return 0;
	}
    _UINT32 fromMscrypt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring password = params.getPassword();

        if (password.empty())
        {
            if(!params.getDontSaveAdditional())
            {
                copyOrigin(sFrom, sTo);
            }
            return AVS_FILEUTILS_ERROR_CONVERT_DRM;
        }

		std::wstring sResultDecryptFile = sTemp	+ FILE_SEPARATOR_STR + L"uncrypt_file.oox";
		
        _UINT32 nRes = mscrypt2oox(sFrom, sResultDecryptFile, sTemp, params);
 
		nRes = processEncryptionError(nRes, sFrom, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeFileFormatChecker OfficeFileFormatChecker;

            if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
            {
                params.changeFormatFrom(OfficeFileFormatChecker.nFileType);
                switch (OfficeFileFormatChecker.nFileType)
                {
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
				{
                    return fromDocument(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, sTemp, params);
                }break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
                {
                    return fromSpreadsheet(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, sTemp, params);
                }break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
				{
                    return fromPresentation(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, sTemp, params);
                }break;
                }
            }
        }
        return nRes;
	}
 	//html
	_UINT32 html2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::vector<std::wstring> arFiles;
       arFiles.push_back(sFrom);

       CHtmlFile oHtmlFile;
       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
       return 0 == oHtmlFile.Convert(arFiles, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
 	//html in container
	_UINT32 html_zip2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::vector<std::wstring> arFiles;
		
		POLE::Storage storage(sFrom.c_str());
		if (storage.open())
		{
			POLE::Stream stream(&storage, L"WordDocument");	
			
			POLE::uint64 size_stream = stream.size();
			unsigned char *buffer = new unsigned char[size_stream];
			if (buffer)
			{
				stream.read(buffer, size_stream);
				std::wstring sTempHtml = sTemp + FILE_SEPARATOR_STR + L"tempHtml.html";

				NSFile::CFileBinary file;

                if (file.CreateFileW(sTempHtml))
				{
					file.WriteFile(buffer, size_stream);
					file.CloseFile();
				
					arFiles.push_back(sTempHtml);
				}
				delete []buffer;
			}
		}
		else //in zip
		{
		}				
		CHtmlFile oHtmlFile;
		std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
		return 0 == oHtmlFile.Convert(arFiles, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	//mht
	_UINT32 mht2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       CHtmlFile oHtmlFile;
       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
       return 0 == oHtmlFile.ConvertMht(sFrom, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 epub2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sEpubDir = sTemp + FILE_SEPARATOR_STR + _T("epub_unpacked");
       NSDirectory::CreateDirectory(sEpubDir);
       _UINT32 nRes = zip2dir(sFrom, sEpubDir);
       if(SUCCEEDED_X2T(nRes))
       {
           CHtmlFile oHtmlFile;
           std::wstring sMetaInfo;
           std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
           nRes = 0 == oHtmlFile.ConvertEpub(sEpubDir, sMetaInfo, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
	// mailmerge
	_UINT32 convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
       if(NULL == oMailMergeSend.mailFormat || NULL == oMailMergeSend.recordFrom || NULL == oMailMergeSend.recordTo)
           return AVS_FILEUTILS_ERROR_CONVERT;
       
	   _UINT32 nRes = 0;
       
	   std::wstring sFileFromDir = NSSystemPath::GetDirectoryName(sFrom);
       std::wstring sFileToDir = NSSystemPath::GetDirectoryName(sTo);
       std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR +_T("media");
       NSDoctRenderer::DoctRendererFormat::FormatFile eTypeTo;
       switch(*oMailMergeSend.mailFormat)
       {
       case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:	
       case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:	
		   eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;	break;
       case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
		   eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;	break;
       case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP:
		   eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;	break;
       default:
		   eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;	break;
       }
       std::wstring sJsonPath = sFileFromDir + FILE_SEPARATOR_STR +_T("Editor.json");
       int recordTo = *oMailMergeSend.recordFrom + 4;
       if(recordTo > *oMailMergeSend.recordTo)
           recordTo = *oMailMergeSend.recordTo;
	   NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
       std::wstring sMailMergeXml = getMailMergeXml(sJsonPath, *oMailMergeSend.recordFrom, recordTo, *oMailMergeSend.to);
       //посылаем выходную папку sFileFromDir, чтобы файлы лежали на одном уровне с папкой media, важно для дальнейшей конвертации в docx, pdf
       std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, eTypeTo, sFileFromDir, sFileFromDir, sImagesDirectory, sThemeDir, -1, sMailMergeXml, params);
       std::wstring sResult;
       oDoctRenderer.Execute(sXml, sResult);
       if (-1 != sResult.find(_T("error")))
       {
           std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       else
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
           XmlUtils::CXmlNode oRoot;
           if(TRUE == oRoot.FromXmlString(sResult))
           {
               XmlUtils::CXmlNode oMailMergeFields = oRoot.ReadNode(_T("MailMergeFields"));
               XmlUtils::CXmlNodes oXmlNodes;
               if(TRUE == oMailMergeFields.GetChilds(oXmlNodes))
               {
                   for(int i = 0; i < oXmlNodes.GetCount(); ++i)
                   {
                       XmlUtils::CXmlNode oXmlNode;
                       if(oXmlNodes.GetAt(i, oXmlNode))
                       {
                           std::wstring sFilePathIn             = oXmlNode.GetAttribute(L"path");
                           std::wstring wsFilePathInFilename    = NSSystemPath::GetFileName(sFilePathIn);
                           std::wstring sFilePathOut            = sFileToDir + FILE_SEPARATOR_STR + wsFilePathInFilename;
                           if (NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT == eTypeTo)
                           {
                               sFilePathOut += L".docx";

                               std::wstring sTempDocx = sTemp + FILE_SEPARATOR_STR + wsFilePathInFilename + L"_DOCX";
                               NSDirectory::CreateDirectory(sTempDocx);

							   BinDocxRW::CDocxSerializer m_oCDocxSerializer;
							   m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
                               m_oCDocxSerializer.setFontDir(params.getFontPath());

                               std::wstring sXmlOptions;
                               std::wstring sThemePath;             // will be filled by 'CreateDocxFolders' method
                               std::wstring sMediaPath;             // will be filled by 'CreateDocxFolders' method
                               std::wstring sEmbedPath;             // will be filled by 'CreateDocxFolders' method

                               m_oCDocxSerializer.CreateDocxFolders (sTempDocx, sThemePath, sMediaPath, sEmbedPath);
                               nRes = m_oCDocxSerializer.loadFromFile (sFilePathIn, sTempDocx, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                               if(SUCCEEDED_X2T(nRes))
                               {
                                   COfficeUtils oCOfficeUtils(NULL);
                                   nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempDocx, sFilePathOut, true)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                               }
                           }
                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::PDF == eTypeTo)
                           {
                               sFilePathOut += _T(".pdf");

                               NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
                               initApplicationFonts(pApplicationFonts, params);
							   CPdfRenderer pdfWriter(pApplicationFonts, params.getIsPDFA());
                               pdfWriter.SetTempFolder(sTemp);
                               pdfWriter.SetThemesPlace(sThemeDir);
                               int nReg = (bPaid == false) ? 0 : 1;
                               nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFilePathIn, sFilePathOut)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                               RELEASEOBJECT(pApplicationFonts);
                           }
                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::HTML == eTypeTo)
                           {
                               sFilePathOut += _T(".html");
                               bool bCopy = NSFile::CFileBinary::Copy(sFilePathIn,sFilePathOut);
                               nRes = bCopy ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                           }
                           if(0 != nRes)
                               break;
                           sResult = string_replaceAll(sResult, L"\"" + sFilePathIn + L"\"", L"\"" + NSSystemPath::GetFileName(sFilePathOut) + L"\"");
                       }
                   }
               }
           }
           if(SUCCEEDED_X2T(nRes))
               NSFile::CFileBinary::SaveToFile(sTo, sResult);
       }
       return nRes;
   }
    _UINT32 PdfDjvuXpsToRenderer(IOfficeDrawingFile** ppReader, IRenderer* pRenderer, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, NSFonts::IApplicationFonts* pApplicationFonts)
   {
       _UINT32 nRes = 0;
       IOfficeDrawingFile* pReader = NULL;
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
       {
           pReader = new PdfReader::CPdfReader(pApplicationFonts);
       }
       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFormatFrom)
       {
           pReader = new CDjVuFile(pApplicationFonts);
       }
       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFormatFrom)
       {
           pReader = new CXpsFile(pApplicationFonts);
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       if(SUCCEEDED_X2T(nRes))
       {
           *ppReader = pReader;
           pReader->SetTempDirectory(sTemp);

           std::wstring sPassword = params.getPassword();

           bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);
           if(bResult)
           {
               int nPagesCount = pReader->GetPagesCount();
               for (int i = 0; i < nPagesCount; ++i)
               {
                   pRenderer->NewPage();
                   pRenderer->BeginCommand(c_nPageType);

                   double dPageDpiX, dPageDpiY;
                   double dWidth, dHeight;
                   pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

                   dWidth  *= 25.4 / dPageDpiX;
                   dHeight *= 25.4 / dPageDpiY;

                   pRenderer->put_Width(dWidth);
                   pRenderer->put_Height(dHeight);

                   pReader->DrawPageOnRenderer(pRenderer, i, NULL);

                   pRenderer->EndCommand(c_nPageType);
               }

           }
           else
           {
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
               if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
               {
                   PdfReader::CPdfReader* pPdfReader = static_cast<PdfReader::CPdfReader*>(pReader);
                   if(PdfReader::errorEncrypted == pPdfReader->GetError())
                   {
                       if(sPassword.empty())
                       {
                           if(!params.getDontSaveAdditional())
                           {
                               copyOrigin(sFrom, *params.m_sFileTo);
                           }
                           nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
                       }
                       else
                       {
                           nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
                       }
                   }
               }
           }
       }
       return nRes;
   }

    _UINT32 PdfDjvuXpsToImage(IOfficeDrawingFile** ppReader, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, NSFonts::IApplicationFonts* pApplicationFonts)
	{
		_UINT32 nRes = 0;
		IOfficeDrawingFile* pReader = NULL;
		if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
		{
			pReader = new PdfReader::CPdfReader(pApplicationFonts);
		}
		else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFormatFrom)
		{
			pReader = new CDjVuFile(pApplicationFonts);
		}
		else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFormatFrom)
		{
			pReader = new CXpsFile(pApplicationFonts);
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		if(SUCCEEDED_X2T(nRes))
		{
			*ppReader = pReader;
			pReader->SetTempDirectory(sTemp);

			std::wstring sPassword = params.getPassword();

			bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);
			if(bResult)
			{
				//default as in CMetafileToRenderterRaster
				int nRasterFormat = 4;
				int nSaveType = 2;
				bool bIsOnlyFirst = true;
				int nRasterW = 100;
				int nRasterH = 100;
				if(NULL != params.m_oThumbnail)
				{
					InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
					if(NULL != oThumbnail->format)
					{
						nRasterFormat = *oThumbnail->format;
					}
					if(NULL != oThumbnail->aspect)
					{
						nSaveType = *oThumbnail->aspect;
					}
					if(NULL != oThumbnail->first)
					{
						bIsOnlyFirst = *oThumbnail->first;
					}
					if(NULL != oThumbnail->width)
					{
						nRasterW = *oThumbnail->width;
					}
					if(NULL != oThumbnail->height)
					{
						nRasterH = *oThumbnail->height;
					}
				}
				std::wstring sThumbnailDir;
				std::wstring sFileToExt;
				if (!bIsOnlyFirst)
				{
					sThumbnailDir = sTemp + FILE_SEPARATOR_STR + _T("thumbnails");
                    NSDirectory::CreateDirectory(sThumbnailDir);
					sFileToExt = getExtentionByRasterFormat(nRasterFormat);
				}
				int nPagesCount = pReader->GetPagesCount();
				if (bIsOnlyFirst)
					nPagesCount = 1;
				for (int i = 0; i < nPagesCount; ++i)
				{
					int nRasterWCur = nRasterW;
					int nRasterHCur = nRasterH;

					if (1 == nSaveType)
					{
						double dPageDpiX, dPageDpiY;
						double dWidth, dHeight;
						pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

						double dKoef1 = nRasterWCur / dWidth;
						double dKoef2 = nRasterHCur / dHeight;
						if (dKoef1 > dKoef2)
							dKoef1 = dKoef2;

						nRasterWCur = (int)(dWidth * dKoef1 + 0.5);
						nRasterHCur = (int)(dHeight * dKoef1 + 0.5);
					}
					std::wstring sFileTo;
					if (bIsOnlyFirst)
					{
						sFileTo = sTo;
					}
					else
					{
						sFileTo = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + std::to_wstring(i+1) + sFileToExt;
					}
					pReader->ConvertToRaster(i, sFileTo, nRasterFormat, nRasterWCur, nRasterHCur);
				}
				//zip
				if(!bIsOnlyFirst)
				{
					COfficeUtils oCOfficeUtils(NULL);
					nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
				if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
				{
					PdfReader::CPdfReader* pPdfReader = static_cast<PdfReader::CPdfReader*>(pReader);
					if(PdfReader::errorEncrypted == pPdfReader->GetError())
					{
						if(sPassword.empty())
						{
							if(!params.getDontSaveAdditional())
							{
								copyOrigin(sFrom, *params.m_sFileTo);
							}
							nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
						}
						else
						{
							nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
						}
					}
				}
			}
		}
		return nRes;
	}

	_UINT32 fromDocxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo))
       {
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo)
				{
					std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
					std::wstring sCTTo;
					switch(nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM: sCTTo = _T("application/vnd.ms-word.document.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM: sCTTo = _T("application/vnd.ms-word.template.macroEnabledTemplate.main+xml");break;
					}
					nRes = replaceContentType(sFrom, sCTFrom, sCTTo);
				}
				if(SUCCEEDED_X2T(nRes))
				{
					if(params.hasSavePassword())
					{
						std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.docx");
						nRes = dir2zip(sFrom, sToMscrypt);
						if(SUCCEEDED_X2T(nRes))
						{
							nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
						}
					}
					else
					{
						nRes = dir2zip(sFrom, sTo);
					}
				}
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatTo)
           {
               nRes = docx_dir2doc(sFrom, sTo, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
           {
               nRes = docx_dir2odt(sFrom, sTo, sTemp, params, false);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT == nFormatTo)
           {
               nRes = docx_dir2odt(sFrom, sTo, sTemp, params, true);
           }
		   else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormatTo)
           {
               nRes = docx_dir2rtf(sFrom, sTo, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatTo)
           {
               nRes = docx_dir2txt(sFrom, sTo, sTemp, params);
           }
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       }
       else if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
       {
           nRes = docx_dir2doct_bin(sFrom, sTo, params);
       }
       else
       {
           std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
           NSDirectory::CreateDirectory(sDoctDir);
           std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		   nRes = docx_dir2doct_bin(sFrom, sTFile, params);
          
		   if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }
	_UINT32 fromDoctBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
       {
           std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
           nRes = dir2zip(sFromDir, sTo);
       }
       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
           nRes = doct_bin2pdf(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
           nRes = doct_bin2image(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo))
       {
           std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
           NSDirectory::CreateDirectory(sDocxDir);
           nRes = doct_bin2docx_dir(sFrom, sTo, sDocxDir, bFromChanges, sThemeDir, params);
           if(SUCCEEDED_X2T(nRes))
           {
                nRes = fromDocxDir(sDocxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       else
            nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       return nRes;
   }
	_UINT32 fromDocument(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTo	= *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;
		std::wstring sFontPath;
		if(NULL != params.m_sFontDir)
			sFontPath = *params.m_sFontDir;
		std::wstring sThemeDir;
		if(NULL != params.m_sThemeDir)
			sThemeDir = *params.m_sThemeDir;
		bool bFromChanges = false;
		if(NULL != params.m_bFromChanges)
			bFromChanges = *params.m_bFromChanges;
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
       {
           if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
           {
               std::wstring sDoctDir = NSDirectory::GetFolderPath(sTo);
               
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
                   nRes = html2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
                   nRes = mht2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
                   nRes = epub2doct_dir(sFrom, sDoctDir, sTemp, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           }
           else
           {
               std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
               NSDirectory::CreateDirectory(sDoctDir);
               std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
               
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
                   nRes = html2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
                   nRes = mht2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
                   nRes = epub2doct_dir(sFrom, sDoctDir, sTemp, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
               
			   if(SUCCEEDED_X2T(nRes))
               {
				   nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               }
           }
       }
       else
       {
           std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
           NSDirectory::CreateDirectory(sDocxDir);
           
		   if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatFrom)
           {
               nRes = zip2dir(sFrom, sDocxDir);
           }
		   else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatFrom)
           {
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo)
			   {
				   nRes = docm2docx_dir(sFrom, sDocxDir, params);
			   }
			   else 
			   {
				   nRes = zip2dir(sFrom, sDocxDir);
			   }
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatFrom)
           {
               nRes = dotx2docx_dir(sFrom, sDocxDir, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatFrom)
           {
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo)
			   {
				   nRes = dotm2docx_dir(sFrom, sDocxDir, params);
			   }
			   else
			   {
				   nRes = dotm2docm_dir(sFrom, sDocxDir, params);
			   }
           }
		   else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatFrom)
           {
               nRes = doc2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT == nFormatFrom)
           {
               nRes = odf2oox_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT == nFormatFrom)
           {
               nRes = odf_flat2oox_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormatFrom)
           {
               nRes = rtf2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom)
           {
               nRes = txt2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromDocxDir(sDocxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }

	_UINT32 fromXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params, const std::wstring &sXlsxFile)
   {
       _UINT32 nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo) && AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV != nFormatTo)
       {
			if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo)
				{
					std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
					std::wstring sCTTo;
					switch(nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM: sCTTo = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM: sCTTo = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");break;
					}
					nRes = replaceContentType(sFrom, sCTFrom, sCTTo);
				}
				if(SUCCEEDED_X2T(nRes))
				{
					if(params.hasSavePassword())
					{
						std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.xlsx");
						nRes = dir2zip(sFrom, sToMscrypt);
						if(SUCCEEDED_X2T(nRes))
						{
							nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
						}
					}
					else
					{
						nRes = dir2zip(sFrom, sTo);
					}
				}
           }
           //else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormatTo)
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatTo)
           {
               nRes = xlsx_dir2ods(sFrom, sTo, sTemp, params, false);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatTo)
           {
               nRes = xlsx_dir2ods(sFrom, sTo, sTemp, params, true);
           }
		   else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       }
       else if(AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo)
       {
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, true, sXlsxFile);
       }
       else if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)
       {
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, true, sXlsxFile);
       }
       else
       {
           std::wstring sXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
           NSDirectory::CreateDirectory(sXlstDir);
           std::wstring sTFile = sXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
           if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
				nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params, false, sXlsxFile);
           else
				nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params, true, sXlsxFile);
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }
	_UINT32 fromXlstBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
       {
           std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
           nRes = dir2zip(sFromDir, sTo);
       }
       else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
       {
           nRes = xlst_bin2csv(sFrom, sTo, sTemp, sThemeDir, bFromChanges, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
           nRes = doct_bin2pdf(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
           nRes = doct_bin2image(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo))
       {
           std::wstring sXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
           NSDirectory::CreateDirectory(sXlsxDir);
           nRes = xlst_bin2xlsx_dir(sFrom, sTo, sXlsxDir, bFromChanges, sThemeDir, params);
           if(SUCCEEDED_X2T(nRes))
           {
				std::wstring sXlsxFile;
				nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params, sXlsxFile);
           }
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       return nRes;
   }
	_UINT32 fromSpreadsheet(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTo	= *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;
		std::wstring sFontPath;
		if(NULL != params.m_sFontDir)
			sFontPath = *params.m_sFontDir;
		std::wstring sThemeDir;
		if(NULL != params.m_sThemeDir)
			sThemeDir = *params.m_sThemeDir;
		bool bFromChanges = false;
		if(NULL != params.m_bFromChanges)
			bFromChanges = *params.m_bFromChanges;
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
       {
           if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo || AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo)
           {
               nRes = csv2xlst_bin(sFrom, sTo, params);
           }
           else
           {
               std::wstring sXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
               NSDirectory::CreateDirectory(sXlstDir);
               std::wstring sTFile = sXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
               nRes = csv2xlst_bin(sFrom, sTFile, params);
               if(SUCCEEDED_X2T(nRes))
               {
                   //зануляем sXmlOptions чтобы, при конвертации xlst bin -> xlsx не перепутать с csv
                   nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               }
           }
       }
       else
       {
		   std::wstring sXlsxFile;
           std::wstring sXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
           NSDirectory::CreateDirectory(sXlsxDir);
		   if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom)
           {
				nRes = zip2dir(sFrom, sXlsxDir);
				if(SUCCEEDED_X2T(nRes))
				{
					sXlsxFile = sFrom;
				}
				else
				{
					//check crypt 
					COfficeFileFormatChecker OfficeFileFormatChecker;
					if (OfficeFileFormatChecker.isOfficeFile(sFrom))
					{
						if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
						{
							std::wstring sResultDecryptFile = sTemp	+ FILE_SEPARATOR_STR + L"uncrypt_file.oox";
							// test protect
							bool isOldPassword = params.hasPassword();
							const std::wstring sOldPassword = params.getPassword();
							
							if (isOldPassword) delete params.m_sPassword;
							params.m_sPassword = new std::wstring(L"VelvetSweatshop");

							nRes = mscrypt2oox(sFrom, sResultDecryptFile, sTemp, params);
							if(SUCCEEDED_X2T(nRes))
							{
								nRes = zip2dir(sResultDecryptFile, sXlsxDir);
								if(SUCCEEDED_X2T(nRes))
								{
									sXlsxFile = sResultDecryptFile;
								}
							}
							else
							{
								delete params.m_sPassword;
								if (isOldPassword)
									params.m_sPassword = new std::wstring(sOldPassword);
							}
						}
					}
				}
           }
		   else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatFrom)
		   {
			   if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo)
			   {
				   nRes = xlsm2xlsx_dir(sFrom, sXlsxDir, params);
			   }
			   else
			   {
				   nRes = zip2dir(sFrom, sXlsxDir);
			   }
		   }
		   else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatFrom)
		   {
               nRes = xltx2xlsx_dir(sFrom, sXlsxDir, params);
		   }
		   else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatFrom)
		   {
			   if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo)
			   {
				   nRes = xltm2xlsx_dir(sFrom, sXlsxDir, params);
			   }
			   else
			   {
				   nRes = xltm2xlsm_dir(sFrom, sXlsxDir, params);
			   }
		   }
		   else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormatFrom)
           {
               nRes = xls2xlsx_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatFrom)
           {
               nRes = odf2oox_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT == nFormatFrom)
           {
               nRes = odf_flat2oox_dir(sFrom, sXlsxDir, sTemp, params);
           }
		   else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           if(SUCCEEDED_X2T(nRes))
           {
			   nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params, sXlsxFile);
           }
       }
       return nRes;
   }

	_UINT32 fromPptxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
	{
		_UINT32 nRes = 0;
		if (0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatTo))
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo)
				{
					std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
					std::wstring sCTTo;
					switch(nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM: sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX: sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX: sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM: sCTTo = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM: sCTTo = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");break;
					}
					nRes = replaceContentType(sFrom, sCTFrom, sCTTo);
				}
				if(SUCCEEDED_X2T(nRes))
				{
					if(params.hasSavePassword())
					{
						std::wstring sToMscrypt = sTemp + FILE_SEPARATOR_STR + _T("tomscrypt.pptx");
						nRes = dir2zip(sFrom, sToMscrypt);
						if(SUCCEEDED_X2T(nRes))
						{
							nRes = oox2mscrypt(sToMscrypt, sTo, sTemp, params);
						}
					}
					else
					{
						nRes = dir2zip(sFrom, sTo);
					}
				}
			}
			//else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatTo)
			else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatTo)
			{
               nRes = pptx_dir2odp(sFrom, sTo, sTemp, params, false);
			}
			else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP == nFormatTo)
			{
               nRes = pptx_dir2odp(sFrom, sTo, sTemp, params, true);
			}
			else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else if(AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == nFormatTo)
		{
			nRes = pptx_dir2pptt_bin(sFrom, sTo, sTemp, params);
		}
		else
		{
			std::wstring sPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
			NSDirectory::CreateDirectory(sPpttDir);
			std::wstring sTFile = sPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

			nRes = pptx_dir2pptt_bin(sFrom, sTFile, sTemp, params);
			if(SUCCEEDED_X2T(nRes))
			{
			   nRes = fromPpttBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
			}
		}
		return nRes;
	}
	_UINT32 fromPpttBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
	{
       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatTo)
       {
           std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
           nRes = dir2zip(sFromDir, sTo);
       }
       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
           nRes = doct_bin2pdf(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
       {
           NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
           nRes = doct_bin2image(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatTo))
       {
           std::wstring sPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
           NSDirectory::CreateDirectory(sPptxDir);
           nRes = pptt_bin2pptx_dir(sFrom, sTo, sPptxDir, bFromChanges, sThemeDir, params);
           if(SUCCEEDED_X2T(nRes))
           {
                nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       return nRes;
   }
	_UINT32 fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTo	= *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;
		std::wstring sFontPath;
		if(NULL != params.m_sFontDir)
			sFontPath = *params.m_sFontDir;
		std::wstring sThemeDir;
		if(NULL != params.m_sThemeDir)
			sThemeDir = *params.m_sThemeDir;
		bool bFromChanges = false;
		if(NULL != params.m_bFromChanges)
			bFromChanges = *params.m_bFromChanges;
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
       std::wstring sPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sPptxDir);
	  
	   if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatFrom)
       {
           nRes = zip2dir(sFrom, sPptxDir);
       }
	   else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatFrom)
	   {
		   if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
		   {
			   nRes = pptm2pptx_dir(sFrom, sPptxDir, params);
		   }
		   else
		   {
			   nRes = zip2dir(sFrom, sPptxDir);
		   }
	   }
	   else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatFrom)
	   {
		   if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
		   {
			   nRes = ppsm2pptx_dir(sFrom, sPptxDir, params);
		   }
		   else
		   {
			   nRes = zip2dir(sFrom, sPptxDir);
		   }
	   }
	   else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatFrom)
       {
           nRes = ppt2pptx_dir(sFrom, sPptxDir, sTemp, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatFrom || AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP == nFormatFrom)
       {
           nRes = odf2oox_dir(sFrom, sPptxDir, sTemp, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT == nFormatFrom)
       {
           nRes = odf_flat2oox_dir(sFrom, sPptxDir, sTemp, params);
       }
	   else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatFrom)
       {
           nRes = ppsx2pptx_dir(sFrom, sPptxDir, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatFrom)
       {
           nRes = potx2pptx_dir(sFrom, sPptxDir, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatFrom)
       {
		   if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
		   {
			   nRes = potm2pptx_dir(sFrom, sPptxDir, params);
		   }
		   else
		   {
			   nRes = potm2pptm_dir(sFrom, sPptxDir, params);
		   }
       }
	   else
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       if(SUCCEEDED_X2T(nRes))
       {
           nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
       }
       return nRes;
   }

	_UINT32 fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
       {
           std::wstring sTDir = NSDirectory::GetFolderPath(sTo);
           nRes = zip2dir(sFrom, sTDir);
       }
       else
       {
           std::wstring sTDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
           NSDirectory::CreateDirectory(sTDir);
           std::wstring sTFile = sTDir + FILE_SEPARATOR_STR + _T("Editor.bin");
           nRes = zip2dir(sFrom, sTDir);
           if(SUCCEEDED_X2T(nRes))
           {
               if(AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatFrom)
                   nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               else if(AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatFrom)
                   nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               else if(AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatFrom)
                   nRes = fromPpttBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           }
       }
       return nRes;
   }
	_UINT32 fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
       initApplicationFonts(pApplicationFonts, params);
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           if(nFormatFrom == nFormatTo && !params.getIsPDFA())
           {
                NSFile::CFileBinary::Copy(sFrom, sTo);
           }
           else
           {
				CPdfRenderer pdfWriter(pApplicationFonts, params.getIsPDFA());
				pdfWriter.SetTempFolder(sTemp);
				pdfWriter.SetTempFolder(sTemp);
              
				IOfficeDrawingFile* pReader = NULL;
				nRes = PdfDjvuXpsToRenderer(&pReader, &pdfWriter, sFrom, nFormatFrom, sTo, sTemp, params, pApplicationFonts);
				pdfWriter.SaveToFile(sTo);
				RELEASEOBJECT(pReader);
           }
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
       {
           std::wstring sToDir = NSSystemPath::GetDirectoryName(sTo);
           if(!params.getDontSaveAdditional())
           {
               //save origin to print
               copyOrigin(sFrom, *params.m_sFileTo);
           }
           NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
           oHtmlRenderer.CreateOfficeFile(sToDir);
           IOfficeDrawingFile* pReader = NULL;
           nRes = PdfDjvuXpsToRenderer(&pReader, &oHtmlRenderer, sFrom, nFormatFrom, sTo, sTemp, params, pApplicationFonts);
           oHtmlRenderer.CloseFile(params.getIsNoBase64());
           RELEASEOBJECT(pReader);
       }
	   else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
	   {
		   IOfficeDrawingFile* pReader = NULL;
           nRes = PdfDjvuXpsToImage(&pReader, sFrom, nFormatFrom, sTo, sTemp, params, pApplicationFonts);
		   RELEASEOBJECT(pReader);
	   }
       else
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       }
       RELEASEOBJECT(pApplicationFonts);
       return nRes;
   }
	_UINT32 fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           nRes = bin2pdf(sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
       {
           nRes = bin2imageBase64(sFrom, sTo, sTemp, sThemeDir, params);
       }
       else
       {
            nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       }
       return nRes;
   }

	// xls -> xlsx
	_UINT32 xls2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

       NSDirectory::CreateDirectory(sResultDocxDir);

       _UINT32 nRes = xls2xlsx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
	_UINT32 xls2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
	   bool bMacros = false;

	   int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
       
	   _UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, lcid, bMacros);
		
	   nRes = processEncryptionError(nRes, sFrom, params);
       return nRes;
   }
	// xls -> xlsm
	_UINT32 xls2xlsm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       std::wstring sResultXlsmDir = sTemp + FILE_SEPARATOR_STR + _T("xlsm_unpacked");

       NSDirectory::CreateDirectory(sResultXlsmDir);

       _UINT32 nRes = xls2xlsm_dir(sFrom, sResultXlsmDir, sTemp, params);

		if(SUCCEEDED_X2T(nRes))
		{
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlsmDir, sTo, true))
               return 0;
		}
		return nRes;
	}
	_UINT32 xls2xlsm_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		bool bMacros = true;
		
		_UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, NULL, bMacros);

		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
   }
	// xls -> xlst
	_UINT32 xls2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // Extract xlsx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sResultDoctDir);

	   _UINT32 nRes = xls2xlst_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

	// xls -> xlst_bin
	_UINT32 xls2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        NSDirectory::CreateDirectory(sResultXlsxDir);

		bool bMacros = true;
		_UINT32 nRes = ConvertXls2Xlsx( sFrom, sResultXlsxDir, params.getPassword(), params.getFontPath(), sTemp, NULL, bMacros);

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
        {
            BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

             m_oCXlsxSerializer.setFontDir(params.getFontPath());

			return m_oCXlsxSerializer.saveToFile (sTo, sResultXlsxDir, params.getXmlOptions());
        }
        return nRes;
   }
	_UINT32 html2doct_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sResultDoctDir = NSFile::GetDirectoryName(sTo);

        _UINT32 nRes = html2doct_dir(sFrom, sResultDoctDir, sTemp, params);
		
		return nRes;
	}
	_UINT32 html_zip2doct_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sResultDoctDir = NSFile::GetDirectoryName(sTo);

        _UINT32 nRes = html_zip2doct_dir(sFrom, sResultDoctDir, sTemp, params);
		
		return nRes;
	}
	_UINT32 html2doct(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = html2doct_dir(sFrom, sResultDoctDir, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
		return nRes;
	}
	_UINT32 html_zip2doct(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = html_zip2doct_dir(sFrom, sResultDoctDir, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
		return nRes;
	}
    _UINT32 html2docx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = html2doct_dir(sFrom, sResultDoctDir, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
			NSDirectory::CreateDirectory(sDocxDir);

			nRes = doct_bin2docx_dir(sResultDoctFileEditor, L"", sDocxDir, false, L"", params);
			if (SUCCEEDED_X2T(nRes))
			{
				COfficeUtils oCOfficeUtils(NULL);
				nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sDocxDir, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		return nRes;
	}

    _UINT32 html_zip2docx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = html_zip2doct_dir(sFrom, sResultDoctDir, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
			NSDirectory::CreateDirectory(sDocxDir);

			nRes = doct_bin2docx_dir(sResultDoctFileEditor, L"", sDocxDir, false, L"", params);
			if (SUCCEEDED_X2T(nRes))
			{
				COfficeUtils oCOfficeUtils(NULL);
				nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sDocxDir, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		return nRes;
	}

//------------------------------------------------------------------------------------------------------------------
	_UINT32 detectMacroInFile(InputParams& oInputParams)
	{
        _UINT32 nRes = 0;// no macro
		std::wstring sFileFrom	= *oInputParams.m_sFileFrom;

		COfficeFileFormatChecker OfficeFileFormatChecker;

		if (OfficeFileFormatChecker.isOfficeFile(sFileFrom))
        {
           if (OfficeFileFormatChecker.bMacroEnabled)
		   {
			   nRes = AVS_ERROR_MACRO; 
		   }
		}

		return nRes;
	}
	_UINT32 fromInputParams(InputParams& oInputParams)
	{
		TConversionDirection conversion  = oInputParams.getConversionDirection();
		std::wstring sFileFrom	= *oInputParams.m_sFileFrom;
		std::wstring sFileTo	= *oInputParams.m_sFileTo;

		int nFormatFrom = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != oInputParams.m_nFormatFrom)
			nFormatFrom = *oInputParams.m_nFormatFrom;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != oInputParams.m_nFormatTo)
			nFormatTo = *oInputParams.m_nFormatTo;

		if (TCD_ERROR == conversion)
		{
			if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
				return AVS_FILEUTILS_ERROR_CONVERT_NEED_PARAMS;
			else{
				// print out conversion direction error
				std::cerr << "Couldn't recognize conversion direction from an argument" << std::endl;
				return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}

		if (sFileFrom.empty() || sFileTo.empty())
		{
			std::cerr << "Empty sFileFrom or sFileTo" << std::endl;
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}

		// get conversion direction from file extensions
		if (TCD_AUTO == conversion)
		{
			conversion = getConversionDirectionFromExt (sFileFrom, sFileTo);
		}

		if (TCD_ERROR == conversion)
		{
			// print out conversion direction error
			std::cerr << "Couldn't automatically recognize conversion direction from extensions" << std::endl;
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		bool bFromChanges = false;
		if(NULL != oInputParams.m_bFromChanges)
			bFromChanges = *oInputParams.m_bFromChanges;
		bool bPaid = true;
		if(NULL != oInputParams.m_bPaid)
			bPaid = *oInputParams.m_bPaid;
		std::wstring sThemeDir;
		if(NULL != oInputParams.m_sThemeDir)
			sThemeDir = *oInputParams.m_sThemeDir;
		InputParamsMailMerge* oMailMerge = NULL;
		if(NULL != oInputParams.m_oMailMergeSend)
			oMailMerge = oInputParams.m_oMailMergeSend;

		bool bExternalTempDir = false;
		std::wstring sTempDir;
		if (NULL != oInputParams.m_sTempDir)
		{
			bExternalTempDir = true;
			sTempDir = *oInputParams.m_sTempDir;
		}
		else
		{
			sTempDir = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetFolderPath(sFileTo));
		}
		if (sTempDir.empty())
		{
			std::cerr << "Couldn't create temp folder" << std::endl;
			return AVS_FILEUTILS_ERROR_UNKNOWN;
		}

		if (!oInputParams.checkInputLimits())
		{
			return AVS_FILEUTILS_ERROR_CONVERT_LIMITS;
		}

		_UINT32 result = 0;
		switch(conversion)
		{
			case TCD_DOCX2DOCT:
			{
				result = docx2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCT2DOCX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2DOTX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2DOCM:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_XLSX2XLST:
			{
				result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST2XLSX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_XLST2XLSM:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_XLST2XLTX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_PPTX2PPTT:
			{
				result =  pptx2pptt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTT2PPTX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;	
			case TCD_PPTT2PPTM:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;	
			case TCD_PPTT2POTX:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;	
			case TCD_DOTX2DOCX:
			{
				result =  dotx2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCM2DOCX:
			{
				result =  docm2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOTM2DOCX:
			{
				result =  dotm2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOTM2DOCM:
			{
				result =  dotm2docm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLTX2XLSX:
			{
				result =  xltx2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSM2XLSX:
			{
				result =  xltx2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLTM2XLSX:
			{
				result =  xltm2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLTM2XLSM:
			{
				result =  xltm2xlsm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPSX2PPTX:
			{
				result =  ppsx2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_POTX2PPTX:
			{
				result =  potx2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_POTM2PPTX:
			{
				result =  potm2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPSM2PPTX:
			{
				result =  ppsm2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_POTM2PPTM:
			{
				result =  potm2pptm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPSM2PPTM:
			{
				result =  ppsm2pptm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTM2PPTX:
			{
				result =  pptm2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_ZIPDIR:
			{
				result =  dir2zip (sFileFrom, sFileTo);
			}break;
			case TCD_UNZIPDIR:
			{
				result =  zip2dir (sFileFrom, sFileTo);
			}break;
			case TCD_CSV2XLSX:
			{
				result =  csv2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_CSV2XLST:
			{
				result =  csv2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSX2CSV:
			{
				result =  xlsx2csv (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST2CSV:
			{
				result =  xlst2csv (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCX2DOCT_BIN:
			{
				result = docx2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCT_BIN2DOCX:
			{
				result =  doct_bin2docx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_XLSX2XLST_BIN:
			{
				result =  xlsx2xlst_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST_BIN2XLSX:
			{
				result =  xlst_bin2xlsx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_PPTX2PPTT_BIN:
			{
				result =  pptx2pptt_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTT_BIN2PPTX:
			{
				result =  pptt_bin2pptx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_BIN2PDF:
			{
				result =  bin2pdf (sFileFrom, sFileTo, sTempDir, bPaid, sThemeDir, oInputParams);
			}break;
			case TCD_BIN2T:
			{
                result =  dir2zip (NSDirectory::GetFolderPath(sFileFrom), sFileTo);
			}break;
			case TCD_T2BIN:
			{
                result =  zip2dir (sFileFrom, NSDirectory::GetFolderPath(sFileTo));
			}break;
			case TCD_PPT2PPTX:
			{
				result =  ppt2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
            }break;
			case TCD_PPT2PPTM:
			{
				result =  ppt2pptm (sFileFrom, sFileTo, sTempDir, oInputParams);
            }break;
			case TCD_PPT2PPTT:
			{
				result =  ppt2pptt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPT2PPTT_BIN:
			{
				result =  ppt2pptt_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_RTF2DOCX:
			{
				result =  rtf2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_RTF2DOCT:
			{
				result = rtf2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_RTF2DOCT_BIN:
			{
				result = rtf2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCX2RTF:
			{
				result =  docx2rtf (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOC2DOCX:
			{
				result =  doc2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOC2DOCM:
			{
				result =  doc2docm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCT2RTF:
			{
				result =  doct2rtf (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT_BIN2RTF:
			{
				result =  doct_bin2rtf (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_TXT2DOCX:
			{
				result =  txt2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_TXT2DOCT:
			{
				result = txt2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_TXT2DOCT_BIN:
			{
				result = txt2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLS2XLSX:
			{
				result =  xls2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLS2XLSM:
			{
				result =  xls2xlsm (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLS2XLST:
			{
				result = xls2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLS2XLST_BIN:
			{
				result = xls2xlst_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_OTF2ODF:
			{
				result =  otf2odf (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_ODF2OOX:
			{
				result =  odf2oox (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_ODF2OOT:
			{
				result = odf2oot (sFileFrom, sFileTo,  sTempDir, oInputParams);
			}break;
			case TCD_ODF2OOT_BIN:
			{
				result = odf2oot_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
            }break;
			case TCD_ODF_FLAT2OOX:
			{
				result =  odf_flat2oox (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_ODF_FLAT2OOT:
			{
				result = odf_flat2oot (sFileFrom, sFileTo,  sTempDir, oInputParams);
			}break;
			case TCD_ODF_FLAT2OOT_BIN:
			{
				result = odf_flat2oot_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
            }break;
			case TCD_DOCX2ODT:
			{
				result =  docx2odt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSX2ODS:
			{
				result =  xlsx2ods (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTX2ODP:
			{
				result =  pptx2odp (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_MAILMERGE:
			{
				result = convertmailmerge(*oMailMerge, sFileFrom, sFileTo, sTempDir, bPaid, sThemeDir, oInputParams);
			}break;
			case TCD_DOCUMENT2:
			{
				result = fromDocument(sFileFrom, nFormatFrom, sTempDir, oInputParams);
			}break;
			case TCD_SPREADSHEET2:
			{
				result = fromSpreadsheet(sFileFrom, nFormatFrom, sTempDir, oInputParams);
			}break;
			case TCD_PRESENTATION2:
			{
				result = fromPresentation(sFileFrom, nFormatFrom, sTempDir, oInputParams);
			}break;
			case TCD_T2:
			{
				result = fromT(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_DOCT_BIN2:
			{
				result = fromDoctBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_XLST_BIN2:
			{
				result = fromXlstBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_PPTT_BIN2:
			{
				result = fromPpttBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_CROSSPLATFORM2:
			{
				result = fromCrossPlatform(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_CANVAS_PDF2:
			{
				result = fromCanvasPdf(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bFromChanges, bPaid, oInputParams);
			}break;
			case TCD_MSCRYPT2:
			{
				result = fromMscrypt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_MSCRYPT2_RAW:
			{
				result = mscrypt2oox(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_2MSCRYPT_RAW:
			{
				result = oox2mscrypt(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_MSCRYPT2DOCT:
			case TCD_MSCRYPT2XLST:
			case TCD_MSCRYPT2PPTT:
			{
				result = mscrypt2oot (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_MSCRYPT2BIN:
				result =  mscrypt2oot_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			{
			}break;
			case TCD_HTML2DOCX:
			{
				result = html2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_HTMLZIP2DOCX:
			{
				result = html_zip2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_HTML2DOCT:
			{
				result = html2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_HTMLZIP2DOCT:
			{
				result = html_zip2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_HTML2DOCT_BIN:
			{
				result = html2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_HTMLZIP2DOCT_BIN:
			{
				result = html_zip2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;			
			//TCD_FB22DOCX,
			//TCD_FB22DOCT,
			//TCD_FB22DOCT_BIN,

			//TCD_EPUB2DOCX,
			//TCD_EPUB2DOCT,
			//TCD_EPUB2DOCT_BIN,
			default:
			{
				result = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}break;
		}

		// delete temp dir
		if (!bExternalTempDir)
		{
			NSDirectory::DeleteDirectory(sTempDir);
		}

		//clean up v8
		NSDoctRenderer::CDocBuilder::Dispose();
		if (SUCCEEDED_X2T(result) && oInputParams.m_bOutputConvertCorrupted)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_CORRUPTED;
		}
		else
		{
			return result;
		}
	}

	_UINT32 FromFile(const std::wstring& file)
	{
		InputParams oInputParams;
		if(oInputParams.FromXmlFile(file))
		{
			return fromInputParams(oInputParams);
		}
		else
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
	}

	_UINT32 FromXml(const std::wstring& xml)
	{
		InputParams oInputParams;
		if(oInputParams.FromXml(xml))
		{
			return fromInputParams(oInputParams);
		}
		else
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
	}
}

