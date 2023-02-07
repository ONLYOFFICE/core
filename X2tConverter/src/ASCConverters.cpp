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
#include "cextracttools.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../Common/3dParty/pole/pole.h"

#include "../../OOXML/Binary/Document/DocWrapper/DocxSerializer.h"
#include "../../OOXML/Binary/Document/DocWrapper/XlsxSerializer.h"

#include "../../OOXML/XlsbFormat/Xlsb.h"
#include "../../OOXML/PPTXFormat/DrawingConverter/ASCOfficePPTXFile.h"

#include "../../RtfFile/Format/ConvertationManager.h"
#include "../../MsBinaryFile/DocFile/Main/DocFormatLib.h"
#include "../../TxtFile/Source/TxtXmlFile.h"
#include "../../MsBinaryFile/PptFile/Main/PPTFormatLib.h"
#include "../../OdfFile/Reader/Converter/ConvertOO2OOX.h"
#include "../../OdfFile/Writer/Converter/Oox2OdfConverter.h"

#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"

#include "../../PdfFile/PdfFile.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../DocxRenderer/DocxRenderer.h"
#include "../../HtmlRenderer/include/HTMLRenderer3.h"
#include "../../Fb2File/Fb2File.h"
#include "../../HtmlFile2/htmlfile2.h"
#include "../../EpubFile/CEpubFile.h"

#include "../../MsBinaryFile/XlsFile/Converter/ConvertXls2Xlsx.h"
#include "../../MsBinaryFile/Common/Vba/VbaReader.h"
#include "../../OfficeCryptReader/source/ECMACryptFile.h"

#include "../../OOXML/Binary/Sheets/Common/Common.h"
#include "../../OOXML/Binary/Sheets/Writer/CSVWriter.h"
#include "../../OOXML/Binary/Sheets/Reader/CSVReader.h"

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
	bool create_if_empty(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &signature)
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

	void CopyOOXOrigin(const std::wstring& sToDir, const std::wstring& sOOXDir, const std::wstring& sToFile, const std::wstring& sOOXFile)
	{
		//save Editor.xlsx for pivot
		std::wstring sEditorOOX = sToDir + FILE_SEPARATOR_STR + sToFile;
		if(sOOXFile.empty())
		{
			dir2zip(sOOXDir, sEditorOOX);
		}
		else
		{
			NSFile::CFileBinary::Copy(sOOXFile, sEditorOOX);
		}
	}
	_UINT32 addContentType(const std::wstring &sDir, const std::wstring &sCT)
	{
		_UINT32 nRes = 0;
		std::wstring sContentTypesPath = sDir + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
		if (NSFile::CFileBinary::Exists(sContentTypesPath))
		{
			std::wstring sData;
			if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
			{
				sData = string_replaceAll(sData, L"</Types>", sCT + L"</Types>");
				if (false == NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
					nRes = AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		return nRes;
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
	_UINT32 package2ooxml(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sResultOoxmlDir = sTemp + FILE_SEPARATOR_STR + _T("ooxml_unpacked");

		NSDirectory::CreateDirectory(sResultOoxmlDir);

		_UINT32 nRes = package2ooxml_dir(sFrom, sResultOoxmlDir, sTemp, params);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultOoxmlDir, sTo, sTemp, params);
		}

		return nRes;
	}
	_UINT32 package2bin_t(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sResultBin_tDir = sTemp + FILE_SEPARATOR_STR + _T("bin_t_unpacked");
		std::wstring sResultBin_tFileEditor = sResultBin_tDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		NSDirectory::CreateDirectory(sResultBin_tDir);

		_UINT32 nRes = package2bin(sFrom, sResultBin_tFileEditor, sTemp, params);

		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultBin_tDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}
	_UINT32 package2bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sResultOoxmlDir = sTemp + FILE_SEPARATOR_STR + _T("ooxml_unpacked");

		NSDirectory::CreateDirectory(sResultOoxmlDir);

		_UINT32 nRes = package2ooxml_dir(sFrom, sResultOoxmlDir, sTemp, params);

		if (SUCCEEDED_X2T(nRes))
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;

			COfficeFileFormatChecker OfficeFileFormatChecker;

			if (OfficeFileFormatChecker.isOOXFormatFile(sResultOoxmlDir, true))
			{
				switch (OfficeFileFormatChecker.nFileType)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
					nRes = docx_dir2doct_bin(sResultOoxmlDir, sTo, sTemp, params, L""); break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:

				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
					nRes = xlsx_dir2xlst_bin(sResultOoxmlDir, sTo, params, true, L""); break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
					nRes = pptx_dir2pptt_bin(sResultOoxmlDir, sTo, sTemp, params, L""); break;
				default: break;
				}
			}
		}

		return nRes;
	}
	_UINT32 package2ooxml_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		BinDocxRW::CDocxSerializer m_oCDocxSerializer;
		
		_UINT32 nRes = m_oCDocxSerializer.unpackageFile(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		
		return nRes;
	}
	// docxflat -> bin
    _UINT32 docxflat2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Save to file (from temp dir)
        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
        m_oCDocxSerializer.setFontDir(params.getFontPath());

        //bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
        _UINT32 nRes =  m_oCDocxSerializer.saveToFile (sTo, sFrom, params.getXmlOptions(), sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
    }
	// docx -> bin
    _UINT32 docx2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
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
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, sTemp, params);
				else
				{
					if (create_if_empty(sFrom, sTo, L"DOCY;v10;0;"))
						return 0;
				}
				return AVS_FILEUTILS_ERROR_CONVERT;
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;
		}
		return docx_dir2doct_bin(sTempUnpackedDOCX, sTo, sTemp, params, sFrom);
    }
	_UINT32 docx_dir2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sDocxFile)
    {
		_UINT32 nRes = S_OK;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX) && !sDocxFile.empty())
		{
			CopyOOXOrigin(sToDir, sFrom, L"origin.docx", sDocxFile);
		}
		else
		{
			// Save to file (from temp dir)
			BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
			m_oCDocxSerializer.setFontDir(params.getFontPath());

			//bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
			nRes =  m_oCDocxSerializer.saveToFile (sTo, sFrom, params.getXmlOptions(), sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
        return nRes;
    }
    // docxflat -> doct
    _UINT32 docxflat2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultDoctDir);

        _UINT32 nRes = docxflat2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

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
	// docx_dir -> doct
	_UINT32 docx_dir2doct(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sDocxFile)
	{
		// Extract docx to temp directory
		std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
		std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		NSDirectory::CreateDirectory(sResultDoctDir);

		_UINT32 nRes = docx_dir2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params, sDocxFile);

		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}

		return nRes;
	}
	// bin -> docx
	_UINT32 doct_bin2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        NSDirectory::CreateDirectory(sResultDocxDir);

		_UINT32 nRes = doct_bin2docx_dir(sFrom, sTo, sResultDocxDir, sThemeDir, params);

		if (SUCCEEDED_X2T(nRes) && params.m_nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == *params.m_nFormatTo || 
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == *params.m_nFormatTo || 
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == *params.m_nFormatTo)
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
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == *params.m_nFormatTo)
			{
				//std::wstring sCT = L"<Default Extension=\"oform\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.oform\"/>";
				//nRes = addContentType(sResultDocxDir, sCT);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == *params.m_nFormatTo)
			{
				//std::wstring sCT = L"<Default Extension=\"docxf\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.docxf\"/>";
				//nRes = addContentType(sResultDocxDir, sCT);
			}
		}
		if(SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultDocxDir, sTo, sTemp, params);
		}

        return nRes;
    }
    // bin -> docx dir
	_UINT32 doct_bin2docx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;
        std::wstring sTargetBin;
		if(params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
		}
        else
            sTargetBin = sFrom;

        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		m_oCDocxSerializer.setOFormEnabled(params.m_nFormatTo && (	*params.m_nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM ||
																	*params.m_nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF));
		m_oCDocxSerializer.setMacroEnabled(params.m_bMacro);
		m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
		m_oCDocxSerializer.setFontDir(params.getFontPath());

		std::wstring sXmlOptions;
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
	_UINT32 doct2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedDOCT);

        // unzip doct to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

		return doct_bin2docx(sTempDoctFileEditor, sTo, sTemp, sThemeDir, params);
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
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, sTemp, params);
				else
				{
					if (create_if_empty(sFrom, sTo, L"XLSY;v10;0;"))
						return 0;
					return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;		
		}

		return xlsx_dir2xlst_bin(sTempUnpackedXLSX, sTo, params, true, sFrom);
    }
	// xlsxflat -> bin
    _UINT32 xlsxflat2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		
		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        oCXlsxSerializer.setFontDir(params.getFontPath());

        return oCXlsxSerializer.saveToFile (sTo, sFrom, params.getXmlOptions());
    }

	_UINT32 xlsx_dir2xlst_bin (const std::wstring &sXlsxDir, const std::wstring &sTo, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile)
    {
		_UINT32 nRes = S_OK;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX) && !sXlsxFile.empty())
		{
			CopyOOXOrigin(sToDir, sXlsxDir, L"origin.xlsx", sXlsxFile);
		}
		else
		{
			BinXlsxRW::CXlsxSerializer oCXlsxSerializer;
			if (oCXlsxSerializer.hasPivot(sXlsxDir))
			{
				//save Editor.xlsx for pivot
				CopyOOXOrigin(sToDir, sXlsxDir, L"Editor.xlsx", sXlsxFile);
			}

			// Save to file (from temp dir)
			oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
			oCXlsxSerializer.setFontDir(params.getFontPath());

			nRes = oCXlsxSerializer.saveToFile (sTo, sXlsxDir, bXmlOptions ? params.getXmlOptions() : L"");
		}
		return nRes;
    }
	// xslx_dir -> xslt
	_UINT32 xlsx_dir2xlst(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile)
	{
		// Extract xlsx to temp directory
		std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
		std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		NSDirectory::CreateDirectory(sResultXlstDir);

		_UINT32 nRes = xlsx_dir2xlst_bin(sFrom, sResultXlstFileEditor, params, bXmlOptions, sXlsxFile);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
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
	// xlsxflat -> xlsx
	_UINT32 xlsxflat2xlsx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

		NSDirectory::CreateDirectory(sResultDocxDir);

		_UINT32 nRes = xlsxflat2xlsx_dir(sFrom, sResultDocxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, true))
				return 0;
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// xlsxflat -> xlsx dir
	_UINT32 xlsxflat2xlsx_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

		_UINT32 nRes = oCXlsxSerializer.xml2Xlsx(sFrom, sTo, params.getXmlOptions());

		return nRes;
	}
	// xlsxflat -> xlst
    _UINT32 xlsxflat2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sResultXlstDir);

        _UINT32 nRes = xlsxflat2xlst_bin(sFrom, sResultXlstFileEditor, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }
    // bin -> xslx
	_UINT32 xlst_bin2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        NSDirectory::CreateDirectory(sResultXlsxDir);

		_UINT32 nRes = xlst_bin2xlsx_dir(sFrom, sTo, sResultXlsxDir, sThemeDir, params);

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
			nRes = dir2zipMscrypt(sResultXlsxDir, sTo, sTemp, params);
		}
        return nRes;
    }
	_UINT32 xlst_bin2xlsx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;

        std::wstring sTargetBin;
		if(params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
		}
        else
            sTargetBin = sFrom;

        BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setMacroEnabled(params.m_bMacro);
		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

        std::wstring sXmlOptions = _T("");
        std::wstring sMediaPath;             // will be filled by 'CreateXlsxFolders' method
        std::wstring sEmbedPath;             // will be filled by 'CreateXlsxFolders' method

        oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, sTo, sMediaPath, sEmbedPath);

        if(SUCCEEDED_X2T(nRes))
        {
            nRes = oCXlsxSerializer.loadFromFile (sTargetBin, sTo, sXmlOptions, sMediaPath, sEmbedPath);
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);
        return nRes;
    }

    // xslt -> xslx
	_UINT32 xlst2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedXLST);

        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

		return xlst_bin2xlsx(sTempXlstFileEditor, sTo, sTemp, sThemeDir, params);
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
	_UINT32 xlsb2xlsx_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedXLSB = sTemp + FILE_SEPARATOR_STR + _T("xlsb_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSB);
		
		COfficeUtils oCOfficeUtils(NULL);
		_UINT32 nRes = oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSB, NULL, 0);
		if (SUCCEEDED_X2T(nRes))
		{
			OOX::Spreadsheet::CXlsb oXlsb;
			oXlsb.ReadNative(OOX::CPath(sTempUnpackedXLSB));
            oXlsb.PrepareSi();
            oXlsb.PrepareTableFormula();

            OOX::CContentTypes oContentTypes;
            oXlsb.SetPropForWriteSheet(sTo, oContentTypes);
            oXlsb.ReadSheetData(true);

			nRes = oXlsb.WriteNative(sTo, oContentTypes) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
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
				else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
					return mitcrypt2oot_bin(sFrom, sTo, sTemp, params);
				else
				{
					if (create_if_empty(sFrom, sTo, L"PPTY;v10;0;"))
						return 0;
					return AVS_FILEUTILS_ERROR_CONVERT;
				}
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;
		}

		return pptx_dir2pptt_bin(sTempUnpackedPPTX, sTo, sTemp, params, sFrom);
	}
	_UINT32 pptx_dir2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sPptxFile)
    {
		_UINT32 nRes = 0;
		std::wstring sToDir = NSDirectory::GetFolderPath(sTo);
		if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX) && !sPptxFile.empty())
		{
			CopyOOXOrigin(sToDir, sFrom, L"origin.pptx", sPptxFile);
		}
		else
		{
			// convert unzipped pptx to unzipped pptt
			CPPTXFile *pptx_file = new CPPTXFile();

			if (pptx_file)
			{
				pptx_file->SetIsNoBase64(params.getIsNoBase64());
				pptx_file->put_TempDirectory(sTemp);
				pptx_file->SetFontDir (params.getFontPath());
				nRes = (S_OK == pptx_file->OpenFileToPPTY (sFrom, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

				delete pptx_file;
			}
		}
        return nRes;
	}
	// pptx_dir -> pptt
	_UINT32 pptx_dir2pptt(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sPptxFile)
	{
		std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
		std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		NSDirectory::CreateDirectory(sResultPpttDir);

		_UINT32 nRes = pptx_dir2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params, sPptxFile);
		if (SUCCEEDED_X2T(nRes))
		{
			// zip pptt folder to output file
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultPpttDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
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
	_UINT32 pptt_bin2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        NSDirectory::CreateDirectory(sResultPptxDir);

		_UINT32 nRes = pptt_bin2pptx_dir(sFrom, sTo, sResultPptxDir, sThemeDir, params);
		
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
			nRes = dir2zipMscrypt(sResultPptxDir, sTo, sTemp, params);
		}
        return nRes;
	}
	_UINT32 pptt_bin2pptx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;

        std::wstring sTargetBin;
		if(params.getFromChanges())
		{
			params.setFromChanges(false);
			nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sThemeDir, sTargetBin, params);
		}
        else
            sTargetBin = sFrom;

        CPPTXFile *pptx_file = new CPPTXFile();

        HRESULT hr = S_OK;

        if (pptx_file)
        {
			pptx_file->SetMacroEnabled(params.m_bMacro);
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
	_UINT32 pptt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        // unzip pptt to temp folder
        std::wstring sTempUnpackedPPTT = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sTempUnpackedPPTT + FILE_SEPARATOR_STR + _T("Editor.bin");

        NSDirectory::CreateDirectory(sTempUnpackedPPTT);

        // unzip pptt to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

		return pptt_bin2pptx(sTempPpttFileEditor, sTo, sTemp, sThemeDir, params);
    }
    // zip dir
    _UINT32 dir2zip (const std::wstring &sFrom, const std::wstring &sTo, bool bSorted, int method, short level, bool bDateTime)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.CompressFileOrDirectory(sFrom, sTo, bSorted, method, level, bDateTime)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
	_UINT32 dir2zipMscrypt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		_UINT32 nRes = S_OK;
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
			nRes = dir2zip(sFrom, sTo, true);
		}
		return nRes;
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
        BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        oCXlsxSerializer.setFontDir(params.getFontPath());

		if (!params.m_nFormatFrom) params.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_UNKNOWN);
		if (AVS_OFFICESTUDIO_FILE_UNKNOWN == *params.m_nFormatFrom)
			*params.m_nFormatFrom = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
			
		_UINT32 nRes = oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sCSV, params.getXmlOptions());

        if (SUCCEEDED_X2T(nRes))
        {
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }
// csv -> xslx_dir
	_UINT32 csv2xlsx_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		OOX::Spreadsheet::CXlsx oXlsx;

		BYTE fileType;
		UINT nCodePage;
		std::wstring sDelimiter;
		BYTE saveFileType;
		SerializeCommon::ReadFileType(params.getXmlOptions(), fileType, nCodePage, sDelimiter, saveFileType);

		CSVReader csvReader;
		_UINT32 nRes = csvReader.Read(sFrom, oXlsx, nCodePage, sDelimiter);

		oXlsx.PrepareToWrite();

		OOX::CContentTypes oContentTypes;
		nRes = oXlsx.Write(sTo, oContentTypes) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;

		return nRes;
	}
// csv -> xslx
    _UINT32 csv2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
		std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
		NSDirectory::CreateDirectory(sResultXlsxDir);

		_UINT32 nRes = csv2xlsx_dir(sFrom, sResultXlsxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultXlsxDir, sTo, sTemp, params);
		}
		return nRes;
    }
    _UINT32 csv2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        oCXlsxSerializer.setFontDir(params.getFontPath());

        return oCXlsxSerializer.saveToFile(sTo, sFrom, params.getXmlOptions());
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

        BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
        oCXlsxSerializer.setFontDir(params.getFontPath());
		oCXlsxSerializer.setTempDir(sTemp);

        std::wstring sMediaPath;
        std::wstring sEmbedPath;

		params.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);

        return oCXlsxSerializer.loadFromFile (sTempXlstFileEditor, sCSV, params.getXmlOptions(), sMediaPath, sEmbedPath);
    }
	// xlsx_dir -> csv
	_UINT32 xlsx_dir2csv(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
		std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
		
		NSDirectory::CreateDirectory(sResultXlstDir);

		BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
		oCXlsxSerializer.setFontDir(params.getFontPath());

		std::wstring sXMLOptions = _T("<XmlOptions><fileOptions fileType=\"1\"/></XmlOptions>");
		_UINT32 nRes = oCXlsxSerializer.saveToFile(sResultXlstFileEditor, sFrom, sXMLOptions);
		if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sMediaPath;
			std::wstring sEmbedPath;

			sXMLOptions = _T("<XmlOptions><fileOptions fileType=\"2\"/></XmlOptions>");

			nRes = oCXlsxSerializer.loadFromFile(sResultXlstFileEditor, sTo, sXMLOptions, sMediaPath, sEmbedPath);
		}

		return nRes;
	}
	// xslx -> csv
	_UINT32 xlsx2csv(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedXLSX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
			return AVS_FILEUTILS_ERROR_CONVERT;

		return xlsx_dir2csv(sTempUnpackedXLSX, sTo, sTemp, params);
	}
	_UINT32 xlst_bin2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
	{
       _UINT32 nRes = 0;

       std::wstring sTargetBin;
	   if(params.getFromChanges())
	   {
		   params.setFromChanges(false);
		   nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
	   }
       else
           sTargetBin = sFrom;

       if(SUCCEEDED_X2T(nRes))
       {
           //todo сделать отдельный метод для сохранения в csv
           // Save to file (from temp dir)
           BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

		   oCXlsxSerializer.setIsNoBase64(params.getIsNoBase64());
           oCXlsxSerializer.setFontDir(params.getFontPath());

           std::wstring sToTemp = sTemp + FILE_SEPARATOR_STR + _T("output.csv");
           std::wstring sMediaPath;             // will be filled by 'CreateXlsxFolders' method
           std::wstring sEmbedPath;             // will be filled by 'CreateXlsxFolders' method
           std::wstring sXmlOptions  = params.getXmlOptions();

           oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, sTemp, sMediaPath, sEmbedPath);
           
           nRes = oCXlsxSerializer.loadFromFile(sTargetBin, sToTemp, sXmlOptions, sMediaPath, sEmbedPath);

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
        
		CPdfFile pdfWriter(pApplicationFonts);
		pdfWriter.CreatePdf(params.getIsPDFA());
		pdfWriter.SetTempDirectory(sTemp);
		pdfWriter.DocInfo(params.getTitle(), L"", L"", L"");

		CConvertFromBinParams oBufferParams;
		oBufferParams.m_sThemesDirectory = sThemeDir;
		
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
			nRet = S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFrom, sTo, &oBufferParams) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			nRet = S_OK == pdfWriter.OnlineWordToPdf(sFrom, sTo, &oBufferParams) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
        RELEASEOBJECT(pApplicationFonts);
        return nRet;
	}
	_UINT32 bin2image (const std::wstring &sTFileDir, BYTE* pBuffer, LONG lBufferLen, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
        NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
        initApplicationFonts(pApplicationFonts, params);
		NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
		imageWriter.SetMediaDirectory(sTFileDir);
		imageWriter.SetThemesDirectory(sThemeDir);
		imageWriter.SetInternalMediaDirectory(sDocxDir);
		imageWriter.SetTempDirectory(sTemp);
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
			std::wstring sFileAddon = L"image" + getExtentionByRasterFormat(imageWriter.GetRasterFormat());
			if (NULL == params.m_oThumbnail->zip || *(params.m_oThumbnail->zip))
			{
				sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
				NSDirectory::CreateDirectory(sThumbnailDir);
				imageWriter.SetFileName(sThumbnailDir + FILE_SEPARATOR_STR + sFileAddon);
			}
			else
			{
				if (!NSDirectory::Exists(sTo))
					NSDirectory::CreateDirectory(sTo);
				imageWriter.SetFileName(sTo + FILE_SEPARATOR_STR + sFileAddon);
			}
		}
		nRes = imageWriter.ConvertBuffer(pBuffer, lBufferLen) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		if(!sThumbnailDir.empty())
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		RELEASEOBJECT(pApplicationFonts);
		return nRes;
	}
	_UINT32 bin2imageBase64 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
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
			nRes = bin2image(sTFileDir, pBuffer, nBufferLen, sTo, sTemp, sThemeDir, params, sDocxDir);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}

		RELEASEARRAYOBJECTS(pBuffer);
		RELEASEARRAYOBJECTS(pFileContent);

		return nRes;
	}
	// doct_bin -> epub
	_UINT32 doct_bin2epub(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
		std::wstring sFileFromDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR + L"media";
		std::wstring sHtmlFile        = sTemp        + FILE_SEPARATOR_STR + L"index.html";
		if (!NSDirectory::Exists(sImagesDirectory))
			NSDirectory::CreateDirectory(sImagesDirectory);
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sHtmlFile, sImagesDirectory, sThemeDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			CEpubFile oFile;
			std::wstring sEpubTemp = sTemp + FILE_SEPARATOR_STR + L"tmp";
			NSDirectory::CreateDirectory(sEpubTemp);
			oFile.SetTempDirectory(sEpubTemp);
			if (S_FALSE == oFile.FromHtml(sHtmlFile, sTo, params.m_sTitle ? *params.m_sTitle : L""))
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	// doct_bin -> fb2
	_UINT32 doct_bin2fb(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
		std::wstring sFileFromDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR + L"media";
		std::wstring sHtmlFile        = sTemp        + FILE_SEPARATOR_STR + L"index.html";
		if (!NSDirectory::Exists(sImagesDirectory))
			NSDirectory::CreateDirectory(sImagesDirectory);
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sHtmlFile, sImagesDirectory, sThemeDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			CFb2File fb2File;
			fb2File.SetTmpDirectory(sTemp);
			if (S_FALSE == fb2File.FromHtml(sHtmlFile, sTo, params.m_sTitle ? *params.m_sTitle : L""))
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	// doct_bin -> html
	_UINT32 doct_bin2html(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
		std::wstring sFileFromDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR + L"media";
		std::wstring sHtmlFile        = sTemp        + FILE_SEPARATOR_STR + L"index.html";
		if (!NSDirectory::Exists(sImagesDirectory))
			NSDirectory::CreateDirectory(sImagesDirectory);
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sHtmlFile, sImagesDirectory, sThemeDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else if (!NSFile::CFileBinary::Copy(sHtmlFile, sTo))
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		return nRes;
	}
	// doct_bin -> html_zip
	_UINT32 doct_bin2html_zip(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
		std::wstring sFileFromDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR + L"media";
		std::wstring sHtmlFile        = sTemp        + FILE_SEPARATOR_STR + L"index.html";
		if (!NSDirectory::Exists(sImagesDirectory))
			NSDirectory::CreateDirectory(sImagesDirectory);
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sHtmlFile, sImagesDirectory, sThemeDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			COfficeUtils oZip;
			if (S_FALSE == oZip.CompressFileOrDirectory(sHtmlFile, sTo))
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	// doct_bin -> pdf
	_UINT32 doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
		std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sTFileDir + FILE_SEPARATOR_STR + _T("media");
		std::wstring sPdfBinFile = sTFileDir + FILE_SEPARATOR_STR + _T("pdf.bin");
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sPdfBinFile, sImagesDirectory, sThemeDir, -1, _T(""), params);
		std::wstring sResult;
		bool bRes = oDoctRenderer.Execute(sXml, sResult);
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
			initApplicationFonts(pApplicationFonts, params);

			CPdfFile pdfWriter(pApplicationFonts);
			pdfWriter.CreatePdf(params.getIsPDFA());
			pdfWriter.SetTempDirectory(sTemp);
			pdfWriter.DocInfo(params.getTitle(), L"", L"", L"");

			CConvertFromBinParams oBufferParams;
			oBufferParams.m_sThemesDirectory = sThemeDir;
			oBufferParams.m_sInternalMediaDirectory = sDocxDir;
			
			std::wstring documentID = params.getDocumentID();
			if (false == documentID.empty())
				pdfWriter.SetDocumentID(documentID);

			std::wstring password = params.getSavePassword();
			if (false == password.empty())
				pdfWriter.SetPassword(password);

			int nReg = (bPaid == false) ? 0 : 1;
			nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sPdfBinFile, sTo, &oBufferParams)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			RELEASEOBJECT(pApplicationFonts);
		}
		//удаляем sPdfBinFile, потому что он не в Temp
		if (NSFile::CFileBinary::Exists(sPdfBinFile))
			NSFile::CFileBinary::Remove(sPdfBinFile);
		return nRes;
	}
	// doct_bin -> image
	_UINT32 doct_bin2image(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params, const std::wstring& sDocxDir = L"")
	{
		_UINT32 nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
		std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = sTFileDir + FILE_SEPARATOR_STR + _T("media");
		std::wstring sPdfBinFile = sTFileDir + FILE_SEPARATOR_STR + _T("pdf.bin");
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sPdfBinFile, sImagesDirectory, sThemeDir, -1, _T(""), params);
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
				nRes = 0 == bin2image(sTFileDir, pData, nBytesCount, sTo, sTemp, sThemeDir, params, sDocxDir) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
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
		
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sResultPptxDir, sTo, sTemp, params);
		}
		return nRes;
	}
	_UINT32 ppt2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		COfficePPTFile pptFile;

		pptFile.put_TempDirectory(sTemp);
	   
		params.m_bMacro = false;
		long nRes = pptFile.LoadFromFile(sFrom, sTo, params.getPassword(), params.m_bMacro);
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
	   
	   params.m_bMacro = true;
		_UINT32 nRes = pptFile.LoadFromFile(sFrom, sTo, params.getPassword(), params.m_bMacro);
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

	   params.m_bMacro = true;
       _UINT32 nRes = pptFile.LoadFromFile(sFrom, sTempUnpackedPPTX, params.getPassword(), params.m_bMacro);
		
		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
        {
		  // convert unzipped pptx to unzipped pptt
		   CPPTXFile *pptx_file = new CPPTXFile();

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
	   
	   if (SUCCEEDED_X2T(nRes))
	   {
		   nRes = dir2zipMscrypt(sResultDocxDir, sTo, sTemp, params);
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
		rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

        if ( rtfConvert.ConvertRtfToOOX(sFrom, sResultDocxDir) == 0)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            m_oCDocxSerializer.setFontDir(params.getFontPath());

            std::wstring sXmlOptions;
            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, sXmlOptions, sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

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
	   rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

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
	   
	   if (SUCCEEDED_X2T(hRes))
	   {
		   hRes = dir2zipMscrypt(sResultDocxDir, sTo, sTemp, params);
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
	
		params.m_bMacro = false;

		_UINT32 hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), params.m_bMacro);
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
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		params.m_bMacro = true;

		_UINT32 hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), params.m_bMacro);
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
		docFile.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;;

		params.m_bMacro = true;

		_UINT32 nRes = docFile.LoadFromFile( sFrom, sResultDocxDir, params.getPassword(), params.m_bMacro);
 		
		nRes = processEncryptionError(nRes, sFrom, params);
        if (SUCCEEDED_X2T(nRes))
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            m_oCDocxSerializer.setFontDir(params.getFontPath());

			std::wstring xml_options = params.getXmlOptions();

            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, xml_options, sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;
        }
        return nRes;
	}
	_UINT32 docx_dir2doc (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
       return AVS_FILEUTILS_ERROR_CONVERT;
	}

	// doct -> rtf
	_UINT32 doct2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
	{
       // Extract docx to temp directory
       std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

       NSDirectory::CreateDirectory(sTempUnpackedDOCT);

       // unzip doct to folder
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

	   return doct_bin2rtf(sTempDoctFileEditor, sTo, sTemp, sThemeDir, params);
   }

	// bin -> rtf
	_UINT32 doct_bin2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
   {
       _UINT32 nRes = 0;
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + L"docx_unpacked";

       NSDirectory::CreateDirectory(sResultDocxDir);

       std::wstring sTargetBin;
	   if(params.getFromChanges())
	   {
		   params.setFromChanges(false);
		   nRes = apply_changes(sFrom, _T(""), NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
	   }
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
			   rtfConvert.m_nUserLCID = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

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
	   if (SUCCEEDED_X2T(nRes))
	   {
		   nRes = dir2zipMscrypt(sResultDocxDir, sTo, sTemp, params);
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

            _UINT32 res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, params.getXmlOptions(), sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 docx2txt(const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sDocxDir, sTempUnpackedDOCX, NULL, 0))
		{
			return docx_dir2txt(sTempUnpackedDOCX, sTo, sTemp, params);
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

	   _UINT32 nRes = 0;

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
		{
			NSDirectory::CreateDirectory(sTempUnpackedOox);

            nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTempUnpackedOox, params.getFontPath(), sTemp, params.getPassword());
			nRes = processEncryptionError(nRes, sFrom, params);
			if (SUCCEEDED_X2T(nRes))
			{
				COfficeFileFormatChecker OfficeFileFormatChecker;

				if (OfficeFileFormatChecker.isOOXFormatFile(sTempUnpackedOox, true))
				{
					switch (OfficeFileFormatChecker.nFileType)
					{
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
						{
							return docx_dir2doct_bin(sTempUnpackedOox, sTo, sTemp, params, L"");
						}break;
						case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
						case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
						case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
						case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
						case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
						{
							const std::wstring & sXmlOptions = params.getXmlOptions();
							return xlsx_dir2xlst_bin(sTempUnpackedOox, sTo, params, false, L"");
						}break;
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
						case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
						{
							return pptx_dir2pptt_bin(sTempUnpackedOox, sTo, sTemp, params, L"");
						}break;
						default:
						{
							nRes = AVS_FILEUTILS_ERROR_CONVERT;
						}break;
					}
				}
			}
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (create_if_empty(sFrom, sTo, L"DOCY;v10;0;"))
				nRes = 0;
		}
		return nRes;
	}
	_UINT32 otf2odf(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOdf);

	   COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

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
       
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sTempUnpackedOox, sTo, sTemp, params);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (create_if_empty(sFrom, sTo, L""))
				nRes = 0;
		}
		return nRes;
	}
	_UINT32 odf2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		_UINT32 nRes = 0;

       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       NSDirectory::CreateDirectory(sTempUnpackedOdf);

       COfficeUtils oCOfficeUtils(NULL);
	   if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
	   {
           nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTo, params.getFontPath(), sTemp, params.getPassword());
		   nRes = processEncryptionError(nRes, sFrom, params);
	   }
	   else
	   {
		   nRes = AVS_FILEUTILS_ERROR_CONVERT;
	   }
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

        _UINT32 nRes = ConvertODF2OOXml(sFrom, sTempUnpackedOox, params.getFontPath(), sTemp, params.getPassword());
		nRes = processEncryptionError(nRes, sFrom, params);
		if(SUCCEEDED_X2T(nRes))
		{
           BinDocxRW::CDocxSerializer m_oCDocxSerializer;

           m_oCDocxSerializer.setFontDir(params.getFontPath());

           nRes =  m_oCDocxSerializer.saveToFile (sTo, sTempUnpackedOox, params.getXmlOptions(), sTemp) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
        _UINT32 nRes = ConvertODF2OOXml(sFrom, sTo, params.getFontPath(), sTemp, params.getPassword());
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
	// docxflat -> docx
	_UINT32 docxflat2docx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + L"docx_unpacked";
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		if (m_oCDocxSerializer.convertFlat(sFrom, sTempUnpackedDOCX))
		{
			_UINT32 nRes = dir2zipMscrypt(sTempUnpackedDOCX, sTo, sTemp, params);
			if (SUCCEEDED_X2T(nRes))
				return S_OK;
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	// docxflat -> odt
	_UINT32 docxflat2odt(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedODT = sTemp + FILE_SEPARATOR_STR + L"odt_unpacked";
		NSDirectory::CreateDirectory(sTempUnpackedODT);

		Oox2Odf::Converter converter(sFrom, L"text", params.getFontPath(), false);

		_UINT32 nRes = 0;
		try
		{
			std::wstring password = params.getSavePassword();
			std::wstring documentID = params.getDocumentID();

			converter.convert();
			converter.write(sTempUnpackedODT, sTemp, password, documentID);

			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODT, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		catch (...)
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
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

		std::wstring password	= params.getSavePassword();
		std::wstring documentID = params.getDocumentID();
			
		converter.convert();
		converter.write(sTempUnpackedODS, sTemp, password, documentID);

		COfficeUtils oCOfficeUtils(NULL);
		nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODS, sTo, false, password.empty() ? Z_DEFLATED : 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

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
	_UINT32 mitcrypt2oox	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		//todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
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
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
				{
					return docx2doct_bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
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
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT:
				{
					return docxflat2doct_bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT:
				{
					const std::wstring & sXmlOptions = params.getXmlOptions();
					return xlsxflat2xlst_bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_PACKAGE:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_PACKAGE:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX_PACKAGE:
				{
					return package2bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
			}
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 mitcrypt2oot_bin	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		//todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
	}
	_UINT32 msVbaProject2Xml(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		CVbaReader vbaReader(sFrom, sTo);

		if (false == vbaReader.write())
			return AVS_FILEUTILS_ERROR_CONVERT;

		return 0;
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

		std::wstring sResultDecryptFile = sTemp	+ FILE_SEPARATOR_STR + L"uncrypt_file.oox";
		
        _UINT32 nRes = mscrypt2oox(sFrom, sResultDecryptFile, sTemp, params);

		if (!SUCCEEDED_X2T(nRes) && password.empty()) // qiaoshemei1 (1).xlsx - запрокченный xlsx 
        {
            if(!params.getDontSaveAdditional())
            {
                copyOrigin(sFrom, sTo);
            }
            return AVS_FILEUTILS_ERROR_CONVERT_DRM;
        } 
		nRes = processEncryptionError(nRes, sFrom, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeFileFormatChecker OfficeFileFormatChecker;

            if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
            {
                params.changeFormatFrom(OfficeFileFormatChecker.nFileType, OfficeFileFormatChecker.bMacroEnabled);
                switch (OfficeFileFormatChecker.nFileType)
                {
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
				{
                    return fromDocument(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, sTemp, params);
                }break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
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
	_UINT32 fromMitcrypt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		//todo
		return AVS_FILEUTILS_ERROR_CONVERT_DRM_UNSUPPORTED;
	}

	//html
	_UINT32 html_array2docx_dir (const std::vector<std::wstring> &arFiles, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        CHtmlFile2 oFile;
        oFile.SetTmpDirectory(sTemp);
        return (S_OK == oFile.OpenBatchHtml(arFiles, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 html2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::vector<std::wstring> arFiles;
		arFiles.push_back(sFrom);
		return html_array2docx_dir(arFiles, sTo, sTemp, params);
	}
	//html in container
	_UINT32 html_zip2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
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
		return 0 == html_array2docx_dir(arFiles, sTo, sTemp, params) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	//mht
	_UINT32 mht2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        CHtmlFile2 oFile;
        oFile.SetTmpDirectory(sTemp);
        return (S_OK == oFile.OpenMht(sFrom, sTo)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	//epub
	_UINT32 epub2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
        CEpubFile oFile;
        oFile.SetTempDirectory(sTemp);
        return (S_OK == oFile.Convert(sFrom, sTo, false)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 fb2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		CFb2File fb2File;
		fb2File.SetTmpDirectory(sTemp);
        return S_OK == fb2File.Open(sFrom, sTo) ? S_OK : AVS_FILEUTILS_ERROR_CONVERT;
	}
	_UINT32 fb2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sTempUnpackedDOCX);

		_UINT32 nRes = fb2docx_dir(sFrom, sTempUnpackedDOCX, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sTempUnpackedDOCX, sTo, sTemp, params);
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
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
	   case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
	   case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
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
       std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, eTypeTo, sFrom, sFileFromDir, sImagesDirectory, sThemeDir, -1, sMailMergeXml, params);
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
									std::wstring sTempUnencrypted = sTemp + FILE_SEPARATOR_STR + wsFilePathInFilename + L"_unencrypted";
									NSDirectory::CreateDirectory(sTempUnencrypted);

									nRes = dir2zipMscrypt(sTempDocx, sFilePathOut, sTempUnencrypted, params);
                               }
                           }
                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::PDF == eTypeTo)
                           {
                               sFilePathOut += _T(".pdf");

                               NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
                               initApplicationFonts(pApplicationFonts, params);
                               CPdfFile pdfWriter(pApplicationFonts);
                               pdfWriter.CreatePdf(params.getIsPDFA());
                               pdfWriter.SetTempDirectory(sTemp);
							   pdfWriter.DocInfo(params.getTitle(), L"", L"", L"");

							   CConvertFromBinParams oBufferParams;
							   oBufferParams.m_sThemesDirectory = sThemeDir;

								std::wstring documentID = params.getDocumentID();
								if (false == documentID.empty())
									pdfWriter.SetDocumentID(documentID);

								std::wstring password = params.getSavePassword();
								if (false == password.empty())
									pdfWriter.SetPassword(password);

                               int nReg = (bPaid == false) ? 0 : 1;
							   nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFilePathIn, sFilePathOut, &oBufferParams)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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

	std::string checkPrintPages(InputParams& params)
	{
		if (NULL == params.m_sJsonParams)
			return "";

		std::wstring::size_type posNativeOptions = params.m_sJsonParams->find(L"\"nativeOptions\"");
		if (std::wstring::npos == posNativeOptions)
			return "";

		std::wstring::size_type posNativePages = params.m_sJsonParams->find(L"\"pages\":\"", posNativeOptions);
		if (std::wstring::npos == posNativePages)
			return "";

		posNativePages += 9;
		std::wstring::size_type posNativePages2 = params.m_sJsonParams->find(L"\"", posNativePages);
		if (std::wstring::npos == posNativePages2)
			return "";

		std::wstring sPages = params.m_sJsonParams->substr(posNativePages, posNativePages2 - posNativePages);
		if (L"all" == sPages)
			return "";

		if (L"current" == sPages)
		{
			std::wstring::size_type posCurrentPage = params.m_sJsonParams->find(L"\"currentPage\":", posNativeOptions);
			if (std::wstring::npos == posCurrentPage)
				return "";

			posCurrentPage += 14;
			std::wstring::size_type posCurrentPage2 = params.m_sJsonParams->find(L",", posCurrentPage);
			std::wstring::size_type posCurrentPage3 = params.m_sJsonParams->find(L"}", posCurrentPage);

			if (std::wstring::npos == posCurrentPage2)
			{
				if (std::wstring::npos == posCurrentPage3)
					return "";
				posCurrentPage2 = posCurrentPage3;
			}
			else if (std::wstring::npos != posCurrentPage3 && posCurrentPage3 < posCurrentPage2)
				posCurrentPage2 = posCurrentPage3;

			if (std::wstring::npos == posCurrentPage2)
				return "";

			sPages = params.m_sJsonParams->substr(posCurrentPage, posCurrentPage2 - posCurrentPage);
		}

		return U_TO_UTF8(sPages);
	}

	std::vector<bool> getPrintPages(const std::string& sPages, int nPagesCount)
	{
		const char* buffer = sPages.c_str();

		size_t nCur = 0;
		size_t nLen = sPages.length();

		std::vector<bool> arPages;
		for (int i = 0; i < nPagesCount; ++i)
			arPages.push_back(false);

		while (nCur < nLen)
		{
			size_t cur = nCur;
			while (cur < nLen && buffer[cur] != ',')
				++cur;

			int nStart = 0;
			int nEnd = 0;

			size_t curRec = nCur;
			while (curRec < cur)
			{
				char c = buffer[curRec++];
				if (c >= '0' && c <= '9')
					nStart = 10 * nStart + (c - '0');

				if (c == '-')
					break;
			}

			if (nStart == 0)
				nStart = 1;

			if (curRec == cur)
				nEnd = nStart;
			else
			{
				while (curRec < cur)
				{
					char c = buffer[curRec++];
					if (c >= '0' && c <= '9')
						nEnd = 10 * nEnd + (c - '0');

					if (c == '-')
						break;
				}

				if (0 == nEnd || nEnd > nPagesCount)
					nEnd = nPagesCount;
			}

			for (int i = nStart; i <= nEnd; ++i)
				arPages[i - 1] = true;

			nCur = cur;
			if (nCur < nLen)
				++nCur;
		}

		return arPages;
	}

	_UINT32 PdfDjvuXpsToRenderer(IOfficeDrawingFile** ppReader, IRenderer* pRenderer, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, NSFonts::IApplicationFonts* pApplicationFonts, const std::string& sPages = "")
   {
       _UINT32 nRes = 0;
       IOfficeDrawingFile* pReader = NULL;
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
       {
           pReader = new CPdfFile(pApplicationFonts);
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

			   bool bIsUsePages = sPages.empty() ? false : true;
			   std::vector<bool> arPages;
			   if (bIsUsePages)
				   arPages = getPrintPages(sPages, nPagesCount);

               for (int i = 0; i < nPagesCount; ++i)
               {
				   if (bIsUsePages && !arPages[i])
					   continue;

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
                   CPdfFile* pPdfReader = static_cast<CPdfFile*>(pReader);
                   if(PdfFile::errorEncrypted == pPdfReader->GetError())
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
			pReader = new CPdfFile(pApplicationFonts);
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
				bool bIsZip = true;
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
					if(NULL != oThumbnail->zip)
					{
						bIsZip = *oThumbnail->zip;
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
					if (bIsZip)
					{
						sThumbnailDir = sTemp + FILE_SEPARATOR_STR + _T("thumbnails");
						NSDirectory::CreateDirectory(sThumbnailDir);
					}
					else
					{
						if (!NSDirectory::Exists(sTo))
							NSDirectory::CreateDirectory(sTo);
						sThumbnailDir = sTo;
					}
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
					else if (2 == nSaveType)
					{
						nRasterWCur = -1;
						nRasterHCur = -1;
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
				// zip
				if(!bIsOnlyFirst && bIsZip)
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
					CPdfFile* pPdfReader = static_cast<CPdfFile*>(pReader);
					if(PdfFile::errorEncrypted == pPdfReader->GetError())
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

	_UINT32 fromDocxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sDocxFile)
	{
		_UINT32 nRes = 0;
		std::wstring sFromWithChanges = sFrom;
		bool bIsNeedDoct = false;
		switch (nFormatTo)
		{
			// перечислить все "документные" форматы, которым нужна конвертация через doct
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:
			case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
				bIsNeedDoct = true;
				break;
			default:
				if (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)
					bIsNeedDoct = true;
				break;
		}

		if(0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo) && !bIsNeedDoct)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo || 
					AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || 
					AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo)
				{
					std::wstring sCTFrom = L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
					switch(*params.m_nFormatFrom)
					{
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM: sCTFrom = L"application/vnd.ms-word.document.macroEnabled.main+xml"; break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX: sCTFrom = L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml"; break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM: sCTFrom = L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml"; break;
					}
					std::wstring sCTTo;
					switch(nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM: sCTTo = L"application/vnd.ms-word.document.macroEnabled.main+xml"; break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX: sCTTo = L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml"; break;
					case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM: sCTTo = L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml"; break;
					}
					nRes = replaceContentType(sFromWithChanges, sCTFrom, sCTTo);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
				{
					std::wstring sCT= L"<Default Extension=\"oform\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.oform\"/>";
					nRes = addContentType(sFromWithChanges, sCT);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo)
				{
					std::wstring sCT = L"<Default Extension=\"docxf\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.docxf\"/>";
					nRes = addContentType(sFromWithChanges, sCT);
				}
				if(SUCCEEDED_X2T(nRes))
				{
					nRes = dir2zipMscrypt(sFromWithChanges, sTo, sTemp, params);
				}
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatTo)
			{
				nRes = docx_dir2doc(sFromWithChanges, sTo, sTemp, params);
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
			{
				nRes = docx_dir2odt(sFromWithChanges, sTo, sTemp, params, false);
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT == nFormatTo)
			{
				nRes = docx_dir2odt(sFromWithChanges, sTo, sTemp, params, true);
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormatTo)
			{
				nRes = docx_dir2rtf(sFromWithChanges, sTo, sTemp, params);
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatTo)
			{
				nRes = docx_dir2txt(sFromWithChanges, sTo, sTemp, params);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
		{
			nRes = docx_dir2doct_bin(sFromWithChanges, sTo, sTemp, params, sDocxFile);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
		{
			nRes = docx_dir2doct(sFromWithChanges, sTo, sTemp, params, sDocxFile);
		}
		else if (bIsNeedDoct)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX))
			{
				std::wstring sToRender = sDocxFile;
				if (sToRender.empty())
				{
					sToRender = sTemp + FILE_SEPARATOR_STR + _T("toRender.docx");
					nRes = dir2zip(sFromWithChanges, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
				if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatTo)
				{
					nRes = doct_bin2epub(eFromType, sToRender, sTo, sTemp, sThemeDir, params, sFrom);
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatTo)
				{
					nRes = doct_bin2fb(eFromType, sToRender, sTo, sTemp, sThemeDir, params, sFrom);
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatTo)
				{
					nRes = doct_bin2html(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatTo)
				{
					nRes = doct_bin2html_zip(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					nRes = doct_bin2image(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
			else
			{
				std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
				NSDirectory::CreateDirectory(sDoctDir);
				std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

				nRes = docx_dir2doct_bin(sFromWithChanges, sTFile, sTemp, params, sDocxFile);

				if(SUCCEEDED_X2T(nRes))
				{
					nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromDoctBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
	{
		_UINT32 nRes = 0;
		if(AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
		{
			std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
			nRes = dir2zip(sFromDir, sTo);
		}
		else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2epub(eFromType, sFrom, sTo, sTemp, sThemeDir, params);
		}
		else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2fb(eFromType, sFrom, sTo, sTemp, sThemeDir, params);
		}
		else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2html(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
		}
		else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2html_zip(eFromType, sFrom, sTo, sTemp, bPaid, sThemeDir, params);
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
			
			if (true == NSDirectory::CreateDirectory(sDocxDir))
			{
				params.m_bMacro = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo;

				nRes = doct_bin2docx_dir(sFrom, sTo, sDocxDir, sThemeDir, params);
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = fromDocxDir(sDocxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, L"");
				}
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
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
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT == nFormatFrom)
       {
           if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
           {
				nRes = docxflat2doct_bin(sFrom, sTo, sTemp, params);
           }
		   else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
		   {
				nRes = docxflat2odt(sFrom, sTo, sTemp, params);
		   }
		   else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo)
		   {
			   nRes = docxflat2docx(sFrom, sTo, sTemp, params);
		   }
		   else
           {
               std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
               NSDirectory::CreateDirectory(sDoctDir);
               std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
               
               if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT == nFormatFrom)
                   nRes = docxflat2doct_bin(sFrom, sTFile, sTemp, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
               
			   if(SUCCEEDED_X2T(nRes))
               {
				   nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
               }
           }
       }
       else
       {
		   std::wstring sDocxFile;
           std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
           NSDirectory::CreateDirectory(sDocxDir);
           
		   if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatFrom ||
			   AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatFrom ||
			   AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatFrom)
           {
			   sDocxFile = sFrom;
			   if(params.getFromChanges())
			   {
				   params.setFromChanges(false);
				   nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sDocxFile, params);
			   }
			   nRes = zip2dir(sDocxFile, sDocxDir);
           }
		   else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatFrom)
           {
			   if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo
				|| AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
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
			   if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo
				|| AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
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
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatFrom)
           {
               nRes = fb2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
           {
               nRes = epub2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
           {
               nRes = html2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatFrom)
           {
               nRes = html_zip2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_PACKAGE == nFormatFrom)
           {
               nRes = package2ooxml_dir(sFrom, sDocxDir, sTemp, params);
           }
		   else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
		   {
			   nRes = mht2docx_dir(sFrom, sDocxDir, sTemp, params);
		   }
		   else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           if(SUCCEEDED_X2T(nRes))
           {
			   nRes = fromDocxDir(sDocxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, sDocxFile);
           }
       }
       return nRes;
   }

	_UINT32 fromXlsbXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sXlsxFile)
	{
		_UINT32 nRes = S_OK;
		if(AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo)
		{
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, true, sXlsxFile);
		}
		else if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)
		{
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, true, sXlsxFile);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
		{
			nRes = xlsx_dir2xlst(sFrom, sTo, sTemp, params, true, sXlsxFile);
		}
		else if ((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) ||
						AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo || 
						AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX) &&
				((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) ||	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo))
			{
				std::wstring sToRender = sXlsxFile;
				if (sToRender.empty())
				{
					sToRender = sTemp + FILE_SEPARATOR_STR + _T("toRender.xlsx");
					nRes = dir2zip(sFrom, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
				if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					nRes = doct_bin2image(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
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
					nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sXlsxFile)
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
					switch(*params.m_nFormatFrom)
					{
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM: sCTFrom = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX: sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM: sCTFrom = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");break;
					}
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
					nRes = dir2zipMscrypt(sFrom, sTo, sTemp, params);
				}
           }
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatTo)
			{
				nRes = xlsx_dir2ods(sFrom, sTo, sTemp, params, false);
			}
			//else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
			//{
			//	nRes = xlsx_dir2csv(sFrom, sTo, sTemp, params);
			//}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatTo)
			{
				nRes = xlsx_dir2ods(sFrom, sTo, sTemp, params, true);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
	   }
	   else
	   {
		   nRes = fromXlsbXlsxDir(sFrom, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, sXlsxFile);
	   }
       return nRes;
   }
	_UINT32 fromXlstBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
       {
           std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
           nRes = dir2zip(sFromDir, sTo);
       }
       else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
       {
		   nRes = xlst_bin2csv(sFrom, sTo, sTemp, sThemeDir, params);
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
	   else if (0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo))
	   {
		   std::wstring sXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
		   if (true == NSDirectory::CreateDirectory(sXlsxDir))
		   {
		   	   params.m_bMacro = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo;
			   nRes = xlst_bin2xlsx_dir(sFrom, sTo, sXlsxDir, sThemeDir, params);
			   if (SUCCEEDED_X2T(nRes))
			   {
				   std::wstring sXlsxFile;
				   nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, sXlsxFile);
			   }
		   }
		   else
		   {
			   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		   }
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       return nRes;
   }
	_UINT32 fromSpreadsheet(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTo = *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if(NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;

		if (nFormatTo == AVS_OFFICESTUDIO_FILE_UNKNOWN)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}

		std::wstring sFontPath;
		if(NULL != params.m_sFontDir)
			sFontPath = *params.m_sFontDir;
		std::wstring sThemeDir;
		if(NULL != params.m_sThemeDir)
			sThemeDir = *params.m_sThemeDir;
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
		std::wstring sXlsxFile;
       if (	(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom) &&
			(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo ||
			 AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo))
	   {
		   nRes = csv2xlst_bin(sFrom, sTo, params);
	   }
       else 
       {
           std::wstring sXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
           NSDirectory::CreateDirectory(sXlsxDir);
		   
		   if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatFrom &&
			   !((AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo) || (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo) ||
				   (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)))
		   {
			   nRes = xlsb2xlsx_dir(sFrom, sXlsxDir, sTemp, params);
		   }
		   else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom ||
			   AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatFrom)
           {
				sXlsxFile = sFrom;
				if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom && params.getFromChanges())
				{
					params.setFromChanges(false);
					nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sXlsxFile, params);
				}
				nRes = zip2dir(sXlsxFile, sXlsxDir);
				if(!SUCCEEDED_X2T(nRes))
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
						else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
							nRes = mitcrypt2oox(sFrom, sTo, sTemp, params);
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
		   else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT == nFormatFrom)
		   {
			   nRes = xlsxflat2xlsx_dir(sFrom, sXlsxDir, sTemp, params);
		   }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatFrom)
           {
               nRes = odf2oox_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT == nFormatFrom)
           {
               nRes = odf_flat2oox_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_PACKAGE == nFormatFrom)
           {
               nRes = package2ooxml_dir(sFrom, sXlsxDir, sTemp, params);
           }
		   else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
		   {
			   nRes = csv2xlsx_dir(sFrom, sXlsxDir, sTemp, params);	
			   *params.m_nFormatFrom = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		   }
		   else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           if(SUCCEEDED_X2T(nRes))
           {
			   nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, sXlsxFile);
           }
       }
       return nRes;
   }

	_UINT32 fromPptxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sPptxFile)
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
					switch(*params.m_nFormatFrom)
					{
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM: sCTFrom = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX: sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX: sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM: sCTFrom = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM: sCTFrom = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");break;
					}
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
					nRes = dir2zipMscrypt(sFrom, sTo, sTemp, params);
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
			nRes = pptx_dir2pptt_bin(sFrom, sTo, sTemp, params, sPptxFile);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatTo)
		{
			nRes = pptx_dir2pptt(sFrom, sTo, sTemp, params, sPptxFile);
		}
		else if ( (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) ||
				 AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX))
			{
				std::wstring sToRender = sPptxFile;
				if (sToRender.empty())
				{
					sToRender = sTemp + FILE_SEPARATOR_STR + _T("toRender.pptx");
					nRes = dir2zip(sFrom, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
				if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					nRes = doct_bin2image(eFromType, sToRender, sTo, sTemp, bPaid, sThemeDir, params, sFrom);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
			else
			{
				std::wstring sPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
				NSDirectory::CreateDirectory(sPpttDir);
				std::wstring sTFile = sPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

				nRes = pptx_dir2pptt_bin(sFrom, sTFile, sTemp, params, sPptxFile);
				if(SUCCEEDED_X2T(nRes))
				{
				   nRes = fromPpttBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromPpttBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
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
		   
		   if (true == NSDirectory::CreateDirectory(sPptxDir))
		   {
		   	   params.m_bMacro = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo;
			   nRes = pptt_bin2pptx_dir(sFrom, sTo, sPptxDir, sThemeDir, params);
			   if (SUCCEEDED_X2T(nRes))
			   {
				   nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, L"");
			   }
		   }
		   else
		   {
			   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
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
		bool bPaid = true;
		if(NULL != params.m_bPaid)
			bPaid = *params.m_bPaid;

       _UINT32 nRes = 0;
	   std::wstring sPptxFile;
       std::wstring sPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       NSDirectory::CreateDirectory(sPptxDir);
	  
	   if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatFrom)
       {
		   sPptxFile = sFrom;
		   if(params.getFromChanges())
		   {
			   params.setFromChanges(false);
			   nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sThemeDir, sPptxFile, params);
		   }
		   nRes = zip2dir(sPptxFile, sPptxDir);
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
			   nRes = ppsm2pptm_dir(sFrom, sPptxDir, params);
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
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX_PACKAGE == nFormatFrom)
       {
           nRes = package2ooxml_dir(sFrom, sPptxDir, sTemp, params);
       }
	   else
           nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
       if(SUCCEEDED_X2T(nRes))
       {
		   nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, sPptxFile);
       }
       return nRes;
   }

	_UINT32 fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
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
				   nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
               else if(AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatFrom)
				   nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
               else if(AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatFrom)
				   nRes = fromPpttBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           }
       }
       return nRes;
   }

	_UINT32 fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
   {
       _UINT32 nRes = 0;
       NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
       initApplicationFonts(pApplicationFonts, params);
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
		   std::string sPages = checkPrintPages(params);

		   if(nFormatFrom == nFormatTo && !params.getIsPDFA() && params.getPassword() == params.getSavePassword() && sPages.empty())
           {
                NSFile::CFileBinary::Copy(sFrom, sTo);
           }
           else
           {
				CPdfFile pdfWriter(pApplicationFonts);
				pdfWriter.CreatePdf(params.getIsPDFA());
				pdfWriter.SetTempDirectory(sTemp);
				pdfWriter.DocInfo(params.getTitle(), L"", L"", L"");

				std::wstring documentID = params.getDocumentID();
				if (false == documentID.empty())
					pdfWriter.SetDocumentID(documentID);

				std::wstring password = params.getSavePassword();
				if (false == password.empty())
					pdfWriter.SetPassword(password);
              
				IOfficeDrawingFile* pReader = NULL;
				nRes = PdfDjvuXpsToRenderer(&pReader, &pdfWriter, sFrom, nFormatFrom, sTo, sTemp, params, pApplicationFonts, sPages);
				pdfWriter.SaveToFile(sTo);
				RELEASEOBJECT(pReader);
           }
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
       {
		   if (params.needConvertToOrigin(nFormatFrom))
		   {
			   copyOrigin(sFrom, *params.m_sFileTo);
		   }
		   else
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
       }
	   else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
	   {
		   IOfficeDrawingFile* pReader = NULL;
           nRes = PdfDjvuXpsToImage(&pReader, sFrom, nFormatFrom, sTo, sTemp, params, pApplicationFonts);
		   RELEASEOBJECT(pReader);
	   }
       else
       {
           IOfficeDrawingFile* pReader = NULL;
           switch (nFormatFrom)
           {
           case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
               pReader = new CPdfFile(pApplicationFonts);
               break;
           case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
               pReader = new CXpsFile(pApplicationFonts);
               break;
           case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:
               pReader = new CDjVuFile(pApplicationFonts);
               break;
           default:
               break;
           }

           if (pReader)
           {
               pReader->SetTempDirectory(sTemp);

               std::wstring sPassword = params.getPassword();
               pReader->LoadFromFile(sFrom, L"", sPassword, sPassword);

               CDocxRenderer oDocxRenderer(pApplicationFonts);

               NSDocxRenderer::TextAssociationType taType = NSDocxRenderer::tatPlainLine;
               if (params.m_oTextParams)
               {
                   InputParamsText* oTextParams = params.m_oTextParams;
                   if (oTextParams->m_nTextAssociationType)
                       //taType = static_cast<NSDocxRenderer::TextAssociationType>(*oTextParams->m_nTextAssociationType);
                   {
                       switch (*oTextParams->m_nTextAssociationType)
                       {
                       case 0:
                           taType = NSDocxRenderer::tatBlockChar;
                           break;
                       case 1:
                           taType = NSDocxRenderer::tatBlockLine;
                           break;
                       case 2:
                           taType = NSDocxRenderer::tatPlainLine;
                           break;
                       case 3:
                           taType = NSDocxRenderer::tatPlainParagraph;
                           break;
                       default:
                           break;
                       }
                   }
               }
               oDocxRenderer.SetTextAssociationType(taType);

               std::wstring sTempDirOut = sTemp + L"/output";
               if (!NSDirectory::Exists(sTempDirOut))
				   NSDirectory::CreateDirectory(sTempDirOut);

			   oDocxRenderer.SetTempFolder(sTempDirOut);
			   bool bIsOutCompress = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo && !params.hasSavePassword();
			   nRes = oDocxRenderer.Convert(pReader, sTo, bIsOutCompress);

			   if (nRes == S_OK && !bIsOutCompress)
				   nRes = fromDocxDir(sTempDirOut, sTo, nFormatTo, sTemp, sThemeDir, bPaid, params, L"");
           }
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
           RELEASEOBJECT(pReader);
       }
       RELEASEOBJECT(pApplicationFonts);
       return nRes;
   }
	_UINT32 fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params)
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

       _UINT32 hRes = xls2xlsx_dir(sFrom, sResultDocxDir, sTemp, params);
	   
	   if (SUCCEEDED_X2T(hRes))
	   {
		   hRes = dir2zipMscrypt(sResultDocxDir, sTo, sTemp, params);
	   }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
	_UINT32 xls2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
	{
		params.m_bMacro = false;

		int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;

		_UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, lcid, params.m_bMacro);
		
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
		params.m_bMacro = true;

		int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
		
		_UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, lcid, params.m_bMacro);

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

		params.m_bMacro = true;
		
		int lcid = (NULL != params.m_nLcid) ? *params.m_nLcid : -1;
		
		_UINT32 nRes = ConvertXls2Xlsx( sFrom, sResultXlsxDir, params.getPassword(), params.getFontPath(), sTemp, lcid, params.m_bMacro);

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			BinXlsxRW::CXlsxSerializer oCXlsxSerializer;

			oCXlsxSerializer.setFontDir(params.getFontPath());

			return oCXlsxSerializer.saveToFile (sTo, sResultXlsxDir, params.getXmlOptions());
		}
		return nRes;
	}
	_UINT32 html2doct_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);
		_UINT32 nRes = html2docx_dir(sFrom, sDocxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = (S_OK == docx_dir2doct_bin(sDocxDir, sTo, sTemp, params, L"")) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 html_zip2doct_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);
		_UINT32 nRes = html_zip2docx_dir(sFrom, sDocxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = (S_OK == docx_dir2doct_bin(sDocxDir, sTo, sTemp, params, L"")) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 html2doct(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
		std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		NSDirectory::CreateDirectory(sResultDoctDir);

		_UINT32 nRes = html2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

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

		_UINT32 nRes = html_zip2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
	_UINT32 html2docx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);

		_UINT32 nRes = html2docx_dir(sFrom, sDocxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			nRes = dir2zipMscrypt(sDocxDir, sTo, sTemp, params);
		}
		return nRes;
	}

	_UINT32 html_zip2docx(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		NSDirectory::CreateDirectory(sDocxDir);

		_UINT32 nRes = html_zip2docx_dir(sFrom, sDocxDir, sTemp, params);
		if (SUCCEEDED_X2T(nRes))
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sDocxDir, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
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

		NSDoctRenderer::CDocBuilder::Initialize();

		_UINT32 result = 0;
		switch(conversion)
		{
			case TCD_DOCX2DOCT:
			{
				result = docx2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCXFLAT2DOCT:
			{
				result = docxflat2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCXFLAT2DOCT_BIN:
			{
				result = docxflat2doct_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PKG2BIN_T:
			{
				result = package2bin_t(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PKG2BIN:
			{
				result = package2bin(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;			
			case TCD_PKG2OOXML:
			{
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
				result = package2ooxml(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCT2DOCX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2DOTX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2OFORM:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM);
				result = doct2docx(sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2DOCXF:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF);
				result = doct2docx(sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT2DOCM:
			{
				oInputParams.m_bMacro = true;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM);
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_XLSX2XLST:
			{
				result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
            case TCD_XLSB2XLST:
            {
                oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB);
                result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
            }break;
			case TCD_XLSXFLAT2XLST:
			{
				result = xlsxflat2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSXFLAT2XLST_BIN:
			{
				result = xlsxflat2xlst_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSXFLAT2XLSX:
			{
				result = xlsxflat2xlsx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST2XLSX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_XLST2XLSM:
			{
				oInputParams.m_bMacro = true;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_XLST2XLTX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX);
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_PPTX2PPTT:
			{
				result =  pptx2pptt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTT2PPTX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;	
			case TCD_PPTT2PPTM:
			{
				oInputParams.m_bMacro = true;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;	
			case TCD_PPTT2POTX:
			{
				oInputParams.m_bMacro = false;
				oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX);
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
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
				result =  dir2zip (sFileFrom, sFileTo, false, 8, -1, true);
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
				result =  doct_bin2docx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_XLSX2XLST_BIN:
			{
				result =  xlsx2xlst_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST_BIN2XLSX:
			{
				result =  xlst_bin2xlsx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_PPTX2PPTT_BIN:
			{
				result =  pptx2pptt_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTT_BIN2PPTX:
			{
				result =  pptt_bin2pptx (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
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
			case TCD_DOCX2TXT:
			{
				result = docx2txt(sFileFrom, sFileTo, sTempDir, oInputParams);
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
				result =  doct2rtf (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
			}break;
			case TCD_DOCT_BIN2RTF:
			{
				result =  doct_bin2rtf (sFileFrom, sFileTo, sTempDir, sThemeDir, oInputParams);
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
				result = fromT(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
			}break;
			case TCD_DOCT_BIN2:
			{
				result = fromDoctBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
			}break;
			case TCD_XLST_BIN2:
			{
				result = fromXlstBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
			}break;
			case TCD_PPTT_BIN2:
			{
				result = fromPpttBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
			}break;
			case TCD_CROSSPLATFORM2:
			{
				result = fromCrossPlatform(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
			}break;
			case TCD_CANVAS_PDF2:
			{
				result = fromCanvasPdf(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sThemeDir, bPaid, oInputParams);
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
			{
				result =  mscrypt2oot_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_MITCRYPT2:
			{
				result =  fromMitcrypt (sFileFrom, sFileTo, sTempDir, oInputParams);
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
			case TCD_FB22DOCX:
			{
				result = fb2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_VBAPROJECT2XML:
			{
				result = msVbaProject2Xml(sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
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

	void createJSCaches()
	{
		NSDoctRenderer::CDocBuilder::Initialize();

		NSDoctRenderer::CDoctrenderer oDoctRenderer;
		oDoctRenderer.CreateCache(L"", L"");

		NSDoctRenderer::CDocBuilder::Dispose();
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

