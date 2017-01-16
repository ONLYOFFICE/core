/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "../../ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"
#include "../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"
#include "../../ASCOfficePPTXFile/ASCOfficePPTXFile.h"
#include "../../ASCOfficeRtfFile/RtfFormatLib/source/ConvertationManager.h"
#include "../../ASCOfficeRtfFile/RtfFormatLib/source/ConvertationManager.h"
#include "../../ASCOfficeDocFile/DocFormatLib/DocFormatLib.h"
#include "../../ASCOfficeTxtFile/TxtXmlFormatLib/Source/TxtXmlFile.h"
#include "../../ASCOfficePPTFile/PPTFormatLib/PPTFormatLib.h"
#include "../../ASCOfficeOdfFile/src/ConvertOO2OOX.h"
#include "../../ASCOfficeOdfFileW/source/Oox2OdfConverter/Oox2OdfConverter.h"
#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../PdfReader/Src/ErrorConstants.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../HtmlRenderer/include/HTMLRenderer3.h"
#include "../../HtmlFile/HtmlFile.h"
#include "../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ConvertXls2Xlsx.h"
#include "../../OfficeCryptReader/source/ECMACryptReader.h"

#include <iostream>
#include <fstream>

namespace NExtractTools
{
    void initApplicationFonts(CApplicationFonts& oApplicationFonts, InputParams& params)
    {
		std::wstring sFontPath = params.getFontPath();
        
		if(sFontPath.empty())
            oApplicationFonts.Initialize();
        else
            oApplicationFonts.InitializeFromFolder(sFontPath);
    }
	// docx -> bin
    int docx2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);

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
    int docx_dir2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

		CString sFontPath = std_string2string(params.getFontPath());
        m_oCDocxSerializer.setFontDir(sFontPath);

        //bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
		int nRes =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sFrom), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

//        sleep(3*60);

        return nRes;
    }

    // docx -> doct
    int docx2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sResultDoctDir);

        int nRes = docx2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

	// bin -> docx
    int doct_bin2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultDocxDir);

        nRes = doct_bin2docx_dir(sFrom, sTo, sResultDocxDir, bFromChanges, sThemeDir, params);

        if (SUCCEEDED_X2T(nRes))
        {
            // compress
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultDocxDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
    // bin -> docx dir
    int doct_bin2docx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;
        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        BinDocxRW::CDocxSerializer m_oCDocxSerializer;

        CString sFontPath = std_string2string(params.getFontPath());
        m_oCDocxSerializer.setFontDir(sFontPath);

        CString sXmlOptions = _T("");
        CString sThemePath;             // will be filled by 'CreateDocxFolders' method
		CString sMediaPath;             // will be filled by 'CreateDocxFolders' method
		CString sEmbedPath;             // will be filled by 'CreateDocxFolders' method

        CString sToDir2 = std_string2string(sTo);
        m_oCDocxSerializer.CreateDocxFolders (sToDir2, sThemePath, sMediaPath, sEmbedPath);

        if (SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCDocxSerializer.loadFromFile (std_string2string(sTargetBin), sToDir2, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);
        return nRes;
    }

    // doct -> docx
    int doct2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sTempUnpackedDOCT);

        // unzip doct to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return doct_bin2docx(sTempDoctFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }

    // xslx -> bin
    int xlsx2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
		{
			//check crypt 
			COfficeFileFormatChecker OfficeFileFormatChecker;
			if (OfficeFileFormatChecker.isOfficeFile(sFrom))
			{
				if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
					return mscrypt2oot_bin(sFrom, sTo, sTemp, params);
				else
				{
					//вместо xlsx другой формат!!
				}
			}
			else return AVS_FILEUTILS_ERROR_CONVERT;		}

        return xlsx_dir2xlst_bin(sTempUnpackedXLSX, sTo, params);
    }
    int xlsx_dir2xlst_bin (const std::wstring &sXlsxDir, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		CString sFontPath = std_string2string(params.getFontPath());
        m_oCXlsxSerializer.setFontDir(sFontPath);

        CString sTo1 = std_string2string(sTo);
		return m_oCXlsxSerializer.saveToFile (sTo1, std_string2string(sXlsxDir), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // xslx -> xslt
    int xlsx2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sResultXlstDir);

		int nRes = xlsx2xlst_bin(sFrom, sResultXlstFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

    // bin -> xslx
    int xlst_bin2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;
        // Extract xlsx to temp directory
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultXlsxDir);

        nRes = xlst_bin2xlsx_dir(sFrom, sTo, sResultXlsxDir, bFromChanges, sThemeDir, params);
        if (SUCCEEDED_X2T(nRes))
        {
            // compress
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultXlsxDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
    int xlst_bin2xlsx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;

        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

        CString sFontPath = std_string2string(params.getFontPath());
        m_oCXlsxSerializer.setFontDir(sFontPath);

        CString sXmlOptions = _T("");
        CString sMediaPath;             // will be filled by 'CreateXlsxFolders' method
		CString sEmbedPath;             // will be filled by 'CreateXlsxFolders' method

        m_oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, std_string2string(sTo), sMediaPath, sEmbedPath);

        if(SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCXlsxSerializer.loadFromFile (std_string2string(sTargetBin), std_string2string(sTo), sXmlOptions, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);
        return nRes;
    }

    // xslt -> xslx
    int xlst2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sTempUnpackedXLST);

        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return xlst_bin2xlsx(sTempXlstFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }
    // pptx -> bin
    int pptx2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // unzip pptx to temp folder
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked")+ FILE_SEPARATOR_STR;  // leading slash is very important!

        FileSystem::Directory::CreateDirectory(sTempUnpackedPPTX);

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

        return pptx_dir2pptt_bin(sTempUnpackedPPTX, sTo, params);
	}
    int pptx_dir2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // convert unzipped pptx to unzipped pptt
        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

        int nRes = 0;

        if (pptx_file)
        {
#if defined(_WIN32) || defined (_WIN64)
			BSTR bstrFontPath			= SysAllocString(params.getFontPath().c_str());
            BSTR bstrTempUnpackedPPTX	= SysAllocString(sFrom.c_str());
            BSTR bstrTo					= SysAllocString(sTo.c_str());

            pptx_file->SetFontDir (bstrFontPath);
            nRes = (S_OK == pptx_file->OpenFileToPPTY (bstrTempUnpackedPPTX, bstrTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

            SysFreeString(bstrFontPath);
            SysFreeString(bstrTempUnpackedPPTX);
            SysFreeString(bstrTo);
#else
            pptx_file->SetFontDir (params.getFontPath());
            nRes = (S_OK == pptx_file->OpenFileToPPTY (sFrom, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
#endif
            delete pptx_file;
        }

        return nRes;
	}
    // pptx -> pptt
    int pptx2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sResultPpttDir);

        int nRes = pptx2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            // zip pptt folder to output file
			COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
        return nRes;
    }

    // bin -> pptx
    int pptt_bin2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;
        std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultPptxDir);

        nRes = pptt_bin2pptx_dir(sFrom, sTo, sResultPptxDir, bFromChanges, sThemeDir, params);
        if (!SUCCEEDED_X2T(nRes))
            return nRes;

        // zip pptx folder to output file
        COfficeUtils oCOfficeUtils(NULL);
        nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPptxDir,sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

        return nRes;
	}
    int pptt_bin2pptx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;

        std::wstring sTargetBin;
        if(bFromChanges)
            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sThemeDir, sTargetBin, params);
        else
            sTargetBin = sFrom;

        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

        HRESULT hr = S_OK;

        if (pptx_file)
        {
#if defined(_WIN32) || defined (_WIN64)
            BSTR bstrFontPath		= SysAllocString(params.getFontPath().c_str());
            BSTR bstrTargetBin		= SysAllocString(sTargetBin.c_str());
            BSTR bstrResultPptxDir	= SysAllocString(sTo.c_str());
            BSTR bstrThemeDir	= SysAllocString(sThemeDir.c_str());

            pptx_file->SetFontDir (bstrFontPath);
            nRes = (S_OK == pptx_file->ConvertPPTYToPPTX(bstrTargetBin, bstrResultPptxDir, bstrThemeDir)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

            SysFreeString(bstrFontPath);
            SysFreeString(bstrTargetBin);
            SysFreeString(bstrResultPptxDir);
            SysFreeString(bstrThemeDir);
#else
            pptx_file->SetFontDir(params.getFontPath());

            nRes = (S_OK == pptx_file->ConvertPPTYToPPTX(sTargetBin, sTo, sThemeDir)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
#endif
            delete pptx_file;
        }
        //удаляем EditorWithChanges, потому что он не в Temp
        if (sFrom != sTargetBin)
            NSFile::CFileBinary::Remove(sTargetBin);

        return nRes;
	}
	// pptt -> pptx
    int pptt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        // unzip pptt to temp folder
        std::wstring sTempUnpackedPPTT = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sTempUnpackedPPTT + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sTempUnpackedPPTT);

        // unzip pptt to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;

        return pptt_bin2pptx(sTempPpttFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
    }
    // zip dir
    int dir2zip (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.CompressFileOrDirectory(sFrom, sTo, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // unzip dir
    int zip2dir (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // csv -> xslt
    int csv2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        CString sCSV = std_string2string(sFrom);
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sResultXlstDir);

        COfficeUtils oCOfficeUtils(NULL);
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

        CString sFontPath = std_string2string(params.getFontPath());
        m_oCXlsxSerializer.setFontDir(sFontPath);

        int nRes = m_oCXlsxSerializer.saveToFile (std_string2string(sResultXlstFileEditor), sCSV, std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        if (SUCCEEDED_X2T(nRes))
        {
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

        return nRes;
    }

    // csv -> xslx
    int csv2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        CString sCSV = std_string2string(sFrom);
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);
        FileSystem::Directory::CreateDirectory(sResultXlstDir);

        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

        CString sFontPath = std_string2string(params.getFontPath());
        m_oCXlsxSerializer.setFontDir(sFontPath);

        COfficeUtils oCOfficeUtils(NULL);

        CString sMediaPath;
		CString sEmbedPath;

		int nRes = m_oCXlsxSerializer.saveToFile (std_string2string(sResultXlstFileEditor), sCSV, std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        if (SUCCEEDED_X2T(nRes))
        {
			nRes = m_oCXlsxSerializer.loadFromFile(std_string2string(sResultXlstFileEditor), std_string2string(sTempUnpackedXLSX), std_string2string(params.getXmlOptions()), sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            if (SUCCEEDED_X2T(nRes))
            {
                nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            }
        }

        return nRes;
    }
    int csv2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
    {
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

        CString sFontPath = std_string2string(params.getFontPath());       
		m_oCXlsxSerializer.setFontDir(sFontPath);

        return m_oCXlsxSerializer.saveToFile(std_string2string(sTo), std_string2string(sFrom), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    // xlst -> csv
   int xlst2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        CString sCSV = std_string2string(sTo);
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");
     
		FileSystem::Directory::CreateDirectory(sTempUnpackedXLST);

        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;


        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

		CString sFontPath = std_string2string(params.getFontPath());
        m_oCXlsxSerializer.setFontDir(sFontPath);

        CString sMediaPath;
        CString sEmbedPath;

        return m_oCXlsxSerializer.loadFromFile (std_string2string(sTempXlstFileEditor), sCSV, std_string2string(params.getXmlOptions()), sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
   // xslx -> csv
   int xlsx2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       CString sCSV = std_string2string(sTo);
       std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
       std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
       std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);
       FileSystem::Directory::CreateDirectory(sResultXlstDir);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

       // Save to file (from temp dir)
       BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

	   CString sFontPath = std_string2string(params.getFontPath());
       m_oCXlsxSerializer.setFontDir(sFontPath);

       CString sXMLOptions = _T("");
	   int nRes = m_oCXlsxSerializer.saveToFile (std_string2string(sResultXlstFileEditor), std_string2string(sTempUnpackedXLSX), sXMLOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       if (SUCCEEDED_X2T(nRes))
       {
           CString sMediaPath;
           CString sEmbedPath;

		   nRes = m_oCXlsxSerializer.loadFromFile (std_string2string(sResultXlstDir), sCSV, sXMLOptions, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }
   int xlst_bin2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, InputParams& params)
   {
       int nRes = 0;

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

           CString sFontPath = std_string2string(params.getFontPath());
           m_oCXlsxSerializer.setFontDir(sFontPath);

           std::wstring sToTemp = sTemp + FILE_SEPARATOR_STR + _T("output.csv");
           CString sMediaPath;             // will be filled by 'CreateXlsxFolders' method
           CString sEmbedPath;             // will be filled by 'CreateXlsxFolders' method          
           CString sXmlOptions  = std_string2string(params.getXmlOptions());

           m_oCXlsxSerializer.CreateXlsxFolders (sXmlOptions, std_string2string(sTemp), sMediaPath, sEmbedPath);
           
           nRes = m_oCXlsxSerializer.loadFromFile(std_string2string(sTargetBin), std_string2string(sToTemp), sXmlOptions, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

           //пишем в Temp и копируем, чтобы не возникало лишних файлов рядом с sTo, а лучше перейти на отдельный метод
           if(SUCCEEDED_X2T(nRes))
           {
               NSFile::CFileBinary::Copy(sToTemp, sTo);
           }
       }
       return nRes;
   }
   // bin -> pdf
   int bin2pdf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
       CApplicationFonts oApplicationFonts;
       initApplicationFonts(oApplicationFonts, params);
       CPdfRenderer pdfWriter(&oApplicationFonts);
       pdfWriter.SetTempFolder(sTemp);
       pdfWriter.SetThemesPlace(sThemeDir);
       int nReg = (bPaid == false) ? 0 : 1;
       return S_OK == pdfWriter.OnlineWordToPdf(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
	int bin2image (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
	{
		long nRes = 0;
		std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
		CApplicationFonts oApplicationFonts;
		initApplicationFonts(oApplicationFonts, params);
		NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
		imageWriter.wsHtmlPlace = sTFileDir;
		imageWriter.wsThemesPlace = sThemeDir;
		imageWriter.wsTempDir = sTemp;
		imageWriter.appFonts = &oApplicationFonts;
		if(NULL != params.m_oThumbnail)
		{
			InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
			if(NULL != oThumbnail->format)
			{
				imageWriter.m_nRasterFormat = *oThumbnail->format;
			}
			if(NULL != oThumbnail->aspect)
			{
				imageWriter.m_nSaveType = *oThumbnail->aspect;
			}
			if(NULL != oThumbnail->first)
			{
				imageWriter.m_bIsOnlyFirst = *oThumbnail->first;
			}
			if(NULL != oThumbnail->width)
			{
				imageWriter.m_nRasterW = *oThumbnail->width;
			}
			if(NULL != oThumbnail->height)
			{
				imageWriter.m_nRasterH = *oThumbnail->height;
			}
		}
		std::wstring sThumbnailDir;
		if(imageWriter.m_bIsOnlyFirst)
		{
			imageWriter.m_sFileName = sTo;
		}
		else
		{
			sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
			NSDirectory::CreateDirectory(sThumbnailDir);
			imageWriter.m_sFileName = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + getExtentionByRasterFormat(imageWriter.m_nRasterFormat);
		}
		BYTE* pData;
		DWORD nBytesCount;
		NSFile::CFileBinary::ReadAllBytes(sFrom, &pData, nBytesCount);
		nRes = imageWriter.ConvertBuffer(pData, nBytesCount) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		if(!imageWriter.m_bIsOnlyFirst)
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo, -1) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		return nRes;
	}
   //doct_bin -> pdf
   int doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
       int nRes = 0;
       NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
       std::wstring sTFileDir = FileSystem::Directory::GetFolderPath(sFrom);
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
           CApplicationFonts oApplicationFonts;
           initApplicationFonts(oApplicationFonts, params);
           CPdfRenderer pdfWriter(&oApplicationFonts);
           pdfWriter.SetTempFolder(sTemp);
           pdfWriter.SetThemesPlace(sThemeDir);
           int nReg = (bPaid == false) ? 0 : 1;
           nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sPdfBinFile, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }
       //удаляем sPdfBinFile, потому что он не в Temp
       if (NSFile::CFileBinary::Exists(sPdfBinFile))
           NSFile::CFileBinary::Remove(sPdfBinFile);
       return nRes;
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
	//doct_bin -> image
	int doct_bin2image(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
	{
		int nRes = 0;
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
		std::wstring sTFileDir = FileSystem::Directory::GetFolderPath(sFrom);
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
			nRes = 0 == bin2image(sPdfBinFile, sTo, sTemp, sThemeDir, params) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		//delete sPdfBinFile, because it is not in Temp
		if (NSFile::CFileBinary::Exists(sPdfBinFile))
			NSFile::CFileBinary::Remove(sPdfBinFile);
		return nRes;
	}

   // ppsx -> pptx
   int ppsx2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
	   std::wstring sTempUnpackedPPSX = sTemp + FILE_SEPARATOR_STR + _T("ppsx_unpacked");
	   FileSystem::Directory::CreateDirectory(sTempUnpackedPPSX);

       int nRes = ppsx2pptx_dir(sFrom, sTempUnpackedPPSX, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPSX, sTo, -1))
               return 0;
       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int ppsx2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
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

   // ppt -> pptx
   int ppt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultPptxDir);

       int nRes = ppt2pptx_dir(sFrom, sResultPptxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultPptxDir, sTo, -1))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int ppt2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       COfficePPTFile pptFile;

       pptFile.put_TempDirectory(sTemp);
       return S_OK == pptFile.LoadFromFile(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
   // ppt -> pptt
   int ppt2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
       std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultPpttDir);

       int nRes = ppt2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
       if (SUCCEEDED_X2T(nRes))
       {
           // zip pptt folder to output file
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
   // ppt -> pptt_bin
   int ppt2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // unzip pptx to temp folder
       std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked")+ FILE_SEPARATOR_STR;  // leading slash is very important!

       FileSystem::Directory::CreateDirectory(sTempUnpackedPPTX);

       COfficePPTFile pptFile;

       pptFile.put_TempDirectory(sTemp);

       if ( pptFile.LoadFromFile(sFrom, sTempUnpackedPPTX) != S_OK) return AVS_FILEUTILS_ERROR_CONVERT;

       // convert unzipped pptx to unzipped pptt
       CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);

       int nRes = 0;

       if (pptx_file)
       {
#if defined(_WIN32) || defined (_WIN64)
           BSTR bstrFontPath			= SysAllocString(params.getFontPath().c_str());
           BSTR bstrTempUnpackedPPTX	= SysAllocString(sTempUnpackedPPTX.c_str());
           BSTR bstrTo					= SysAllocString(sTo.c_str());

           pptx_file->SetFontDir (bstrFontPath);
           nRes = (S_OK == pptx_file->OpenFileToPPTY (bstrTempUnpackedPPTX, bstrTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

           SysFreeString(bstrFontPath);
           SysFreeString(bstrTempUnpackedPPTX);
           SysFreeString(bstrTo);
#else
           pptx_file->SetFontDir (params.getFontPath());
           nRes = (S_OK == pptx_file->OpenFileToPPTY (sTempUnpackedPPTX, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
#endif
           delete pptx_file;
       }

       return nRes;
   }

   // rtf -> docx
   int rtf2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultDocxDir);
       int nRes = rtf2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, -1))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int rtf2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        RtfConvertationManager rtfConvert;
        rtfConvert.m_sTempFolder = sTemp;
        return S_OK == rtfConvert.ConvertRtfToOOX(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }

   // rtf -> doct
   int rtf2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // Extract docx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultDoctDir);

       int nRes = rtf2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

   // rtf -> doct_bin
   int rtf2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultDocxDir);
        RtfConvertationManager rtfConvert;

        rtfConvert.m_sTempFolder = sTemp;

        if ( rtfConvert.ConvertRtfToOOX(sFrom, sResultDocxDir) == S_OK)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

			CString sFontPath = std_string2string(params.getFontPath());

            m_oCDocxSerializer.setFontDir(sFontPath);

            CString sXmlOptions;
            int res =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sResultDocxDir), sXmlOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }

   // docx -> rtf
   int docx2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
		std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
		FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);

		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
		{
            return docx_dir2rtf(sTempUnpackedDOCX, sTo, sTemp, params);
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int docx_dir2rtf(const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       //docx folder to rtf
       RtfConvertationManager rtfConvert;

       rtfConvert.m_sTempFolder = sTemp;

       if (rtfConvert.ConvertOOXToRtf(sTo, sDocxDir) == S_OK)
           return 0;
       return AVS_FILEUTILS_ERROR_CONVERT;
   }

   // doc -> docx
   int doc2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultDocxDir);

       long hRes = doc2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(hRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, -1))
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
   int doc2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        COfficeDocFile docFile;
		docFile.m_sTempFolder = sTemp;
		long hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), NULL);
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
   int doc2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // Extract docx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultDoctDir);

       long nRes = doc2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

   // doc -> doct_bin
   int doc2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultDocxDir);

        COfficeDocFile docFile;
		docFile.m_sTempFolder = sTemp;

        long nRes = docFile.LoadFromFile( sFrom, sResultDocxDir, params.getPassword(), NULL);

        if (SUCCEEDED_X2T(nRes))
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            CString sFontPath = std_string2string(params.getFontPath());
            m_oCDocxSerializer.setFontDir(sFontPath);

			std::wstring xml_options = params.getXmlOptions();

			int res =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sResultDocxDir), std_string2string(xml_options)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;
        }
        else if (AVS_ERROR_DRM == nRes)
        {
            if(!params.getDontSaveAdditional())
            {
                copyOrigin(sFrom, *params.m_sFileTo);
            }
            return AVS_FILEUTILS_ERROR_CONVERT_DRM;
        }
        else if (AVS_ERROR_PASSWORD == nRes)
        {
            return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int docx_dir2doc (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       return AVS_FILEUTILS_ERROR_CONVERT;
       COfficeDocFile docFile;
       return /*S_OK == docFile.SaveToFile(sTo, sDocxDir, NULL) ? 0 : */AVS_FILEUTILS_ERROR_CONVERT;
   }

   // doct -> rtf
   int doct2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
   {
       // Extract docx to temp directory
       std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sTempUnpackedDOCT);

       // unzip doct to folder
       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;

       return doct_bin2rtf(sTempDoctFileEditor, sTo, sTemp, bFromChanges, sThemeDir, params);
   }

   // bin -> rtf
   int doct_bin2rtf (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
   {
       int nRes = 0;
       CString sResultDocxDir = std_string2string(sTemp) + FILE_SEPARATOR_STR + _T("docx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultDocxDir);

       std::wstring sTargetBin;
       if(bFromChanges)
           nRes = apply_changes(sFrom, _T(""), NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
       else
       sTargetBin = sFrom;

       BinDocxRW::CDocxSerializer m_oCDocxSerializer;

       CString sFontPath = std_string2string(params.getFontPath());
       m_oCDocxSerializer.setFontDir(sFontPath);

       CString sXmlOptions = _T("");
       CString sThemePath;             // will be filled by 'CreateDocxFolders' method
       CString sMediaPath;             // will be filled by 'CreateDocxFolders' method
	   CString sEmbedPath;             // will be filled by 'CreateDocxFolders' method

       m_oCDocxSerializer.CreateDocxFolders (sResultDocxDir, sThemePath, sMediaPath, sEmbedPath);

       if (SUCCEEDED_X2T(nRes))
       {
           nRes = m_oCDocxSerializer.loadFromFile (std_string2string(sTargetBin), sResultDocxDir, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
           if (SUCCEEDED_X2T(nRes))
           {
               //docx folder to rtf
               RtfConvertationManager rtfConvert;

               rtfConvert.m_sTempFolder = sTemp;

               if (rtfConvert.ConvertOOXToRtf(sTo, string2std_string(sResultDocxDir)) == S_OK)
                   return 0;
           }
       }
       //удаляем EditorWithChanges, потому что он не в Temp
       if (sFrom != sTargetBin)
           NSFile::CFileBinary::Remove(sTargetBin);
       return nRes;
   }
   // txt -> docx
   int txt2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultDocxDir);
       int nRes = txt2docx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, -1))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int txt2docx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       CTxtXmlFile txtFile;
	   
	   std::wstring xml_options = params.getXmlOptions();

	   return S_OK == txtFile.txt_LoadFromFile(sFrom, sTo, xml_options) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
   // txt -> doct
   int txt2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultDoctDir);

       int nRes = txt2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

   // txt -> doct_bin
   int txt2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultDocxDir);

        CTxtXmlFile txtFile;

       // txtFile.m_sTempFolder = std_string2string(sTemp);

		if ( txtFile.txt_LoadFromFile(sFrom, sResultDocxDir, params.getXmlOptions()) == S_OK)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;

            CString sFontPath = std_string2string(params.getFontPath());
            m_oCDocxSerializer.setFontDir(sFontPath);

			int res =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sResultDocxDir), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int docx_dir2txt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
		CTxtXmlFile txtFile;

		return S_OK == txtFile.txt_SaveToFile(sTo, sDocxDir, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }

   int odf2oot(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
   {
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultDoctDir);

       int nRes = odf2oot_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

   int odf2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
   {
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");

       FileSystem::Directory::CreateDirectory(sTempUnpackedOdf);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return -3;

       FileSystem::Directory::CreateDirectory(sTempUnpackedOox);

       if (S_OK == ConvertOO2OOX(sTempUnpackedOdf, sTempUnpackedOox, params.getFontPath(), false, NULL))
       {
           BinDocxRW::CDocxSerializer m_oCDocxSerializer;

           CString sFontPath = std_string2string(params.getFontPath());
           m_oCDocxSerializer.setFontDir(sFontPath);

		   int res =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sTempUnpackedOox), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

           return res;
       }

       return AVS_FILEUTILS_ERROR_CONVERT;
   }

   int odf2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
   {
       std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");
       FileSystem::Directory::CreateDirectory(sTempUnpackedOox);

       int nRes = odf2oox_dir(sFrom, sTempUnpackedOox, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOox, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
   int odf2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
   {
       std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
       FileSystem::Directory::CreateDirectory(sTempUnpackedOdf);

       COfficeUtils oCOfficeUtils(NULL);
       if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
           return AVS_FILEUTILS_ERROR_CONVERT;;

       return S_OK == ConvertOO2OOX(sTempUnpackedOdf, sTo, params.getFontPath(), false, NULL) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
   // docx -> odt
   int docx2odt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
   {
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
        {
            return docx_dir2odt(sTempUnpackedDOCX, sTo, sTemp, params);
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }
   // docx dir -> odt
   int docx_dir2odt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
   {
       std::wstring sTempUnpackedODT = sTemp + FILE_SEPARATOR_STR + _T("odt_unpacked");
       FileSystem::Directory::CreateDirectory(sTempUnpackedODT);

       Oox2Odf::Converter converter(sDocxDir, _T("text"), params.getFontPath(),  NULL);

       int nRes = 0;
       try
       {
           converter.convert();
           converter.write(sTempUnpackedODT);

           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODT, sTo, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       }catch(...)
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
   // xlsx -> ods
   int xlsx2ods (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
   {
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);

        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
        {
            return xlsx_dir2ods(sTempUnpackedXLSX, sTo, sTemp, params);
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }

   int xlsx_dir2ods (const std::wstring &sXlsxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sTempUnpackedODS = sTemp + FILE_SEPARATOR_STR + _T("ods_unpacked");
       FileSystem::Directory::CreateDirectory(sTempUnpackedODS);

       try
       {
           Oox2Odf::Converter converter(sXlsxDir, _T("spreadsheet"),params.getFontPath(), NULL);

           converter.convert();
           converter.write(sTempUnpackedODS);

           COfficeUtils oCOfficeUtils(NULL);
           int nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedODS, sTo, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

           return nRes;
       }catch(...)
       {
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
	}

	int mscrypt2oot (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring sResultOotDir			= sTemp			+ FILE_SEPARATOR_STR + _T("oot_unpacked");
        std::wstring sResultOotFileEditor	= sResultOotDir	+ FILE_SEPARATOR_STR + _T("Editor.bin");

        FileSystem::Directory::CreateDirectory(sResultOotDir);

        int nRes = mscrypt2oot_bin(sFrom, sResultOotFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultOotDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }

		return nRes;
	}
	int mscrypt2oox	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
        std::wstring password = params.getPassword();
        //decrypt to sTo
		ECMACryptReader cryptReader;
        if (cryptReader.DecryptOfficeFile(sFrom, sTo, password) == false)
		{
             if (password.empty())	return AVS_FILEUTILS_ERROR_CONVERT_DRM;
            else					return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
		}

		return S_OK;
	}
	int mscrypt2oot_bin	 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
	{
		//decrypt to temp file
        std::wstring sResultDecryptFile = sTemp	+ FILE_SEPARATOR_STR + L"uncrypt_file.oox";

		ECMACryptReader cryptReader;
		if (cryptReader.DecryptOfficeFile(sFrom, sResultDecryptFile, params.getPassword()) == false)
			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;

		COfficeFileFormatChecker OfficeFileFormatChecker;

		if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
        {
            switch (OfficeFileFormatChecker.nFileType)
			{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				{
					return docx2doct_bin(sResultDecryptFile, sTo, sTemp, params);
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				{
					const std::wstring & sXmlOptions = params.getXmlOptions();
					return xlsx2xlst_bin(sResultDecryptFile, sTo, sTemp, params);
                }break;
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
				{
					return pptx2pptt_bin(sResultDecryptFile, sTo, sTemp,params);
				}break;
			}
		}
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
    int fromMscrypt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
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
		
        long nRes = mscrypt2oox(sFrom, sResultDecryptFile, sTemp, params);

        if (SUCCEEDED_X2T(nRes))
        {
            COfficeFileFormatChecker OfficeFileFormatChecker;

            if (OfficeFileFormatChecker.isOfficeFile(sResultDecryptFile))
            {
                params.changeFormatFrom(OfficeFileFormatChecker.nFileType);
                switch (OfficeFileFormatChecker.nFileType)
                {
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
                {
                    return fromDocument(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, sTemp, params);
                }break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
                {
                    return fromSpreadsheet(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, sTemp, params);
                }break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
                {
                    return fromPresentation(sResultDecryptFile, AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, sTemp, params);
                }break;
                }
            }
        }
        else if (AVS_ERROR_DRM == nRes)
        {
            if(!params.getDontSaveAdditional())
            {
                copyOrigin(sFrom, *params.m_sFileTo);
            }
            return AVS_FILEUTILS_ERROR_CONVERT_DRM;
        }
        else if (AVS_ERROR_PASSWORD == nRes)
        {
            return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
        }
        return nRes;
	}
   //html
   int html2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::vector<std::wstring> arFiles;
       arFiles.push_back(sFrom);

       CHtmlFile oHtmlFile;
       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
       return 0 == oHtmlFile.Convert(arFiles, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
   //mht
   int mht2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       CHtmlFile oHtmlFile;
       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
       return 0 == oHtmlFile.ConvertMht(sFrom, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
   }
   int epub2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sEpubDir = sTemp + FILE_SEPARATOR_STR + _T("epub_unpacked");
       FileSystem::Directory::CreateDirectory(sEpubDir);
       int nRes = zip2dir(sFrom, sEpubDir);
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
   int convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
       if(NULL == oMailMergeSend.mailFormat || NULL == oMailMergeSend.recordFrom || NULL == oMailMergeSend.recordTo)
           return AVS_FILEUTILS_ERROR_CONVERT;
       int nRes = 0;
       std::wstring sFileFromDir = NSSystemPath::GetDirectoryName(sFrom);
       std::wstring sFileToDir = NSSystemPath::GetDirectoryName(sTo);
       std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR +_T("media");
       NSDoctRenderer::DoctRendererFormat::FormatFile eTypeTo;
       switch(*oMailMergeSend.mailFormat)
       {
       case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;break;
       case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;break;
       case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP:eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;break;
       default:eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;break;
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
           if(TRUE == oRoot.FromXmlString(std_string2string(sResult)))
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
                           std::wstring sFilePathIn = string2std_string(oXmlNode.GetAttribute(_T("path")));
                           std::wstring wsFilePathInFilename = NSSystemPath::GetFileName(sFilePathIn);
                           std::wstring sFilePathOut = sFileToDir + FILE_SEPARATOR_STR + wsFilePathInFilename;
                           if (NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT == eTypeTo)
                           {
                               sFilePathOut += _T(".docx");

                               std::wstring sTempDocx = sTemp + FILE_SEPARATOR_STR + wsFilePathInFilename + _T("_DOCX");
                               FileSystem::Directory::CreateDirectory(sTempDocx);

                               BinDocxRW::CDocxSerializer m_oCDocxSerializer;

                               CString sFontPath = std_string2string(params.getFontPath());
                               m_oCDocxSerializer.setFontDir(sFontPath);

                               CString sXmlOptions = _T("");
                               CString sThemePath;             // will be filled by 'CreateDocxFolders' method
                               CString sMediaPath;             // will be filled by 'CreateDocxFolders' method
                               CString sEmbedPath;             // will be filled by 'CreateDocxFolders' method

                               m_oCDocxSerializer.CreateDocxFolders (std_string2string(sTempDocx), sThemePath, sMediaPath, sEmbedPath);
                               nRes = m_oCDocxSerializer.loadFromFile (std_string2string(sFilePathIn), std_string2string(sTempDocx), sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                               if(SUCCEEDED_X2T(nRes))
                               {
                                   COfficeUtils oCOfficeUtils(NULL);
                                   nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempDocx, sFilePathOut, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
                               }
                           }
                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::PDF == eTypeTo)
                           {
                               sFilePathOut += _T(".pdf");

                               CApplicationFonts oApplicationFonts;
                               initApplicationFonts(oApplicationFonts, params);
                               CPdfRenderer pdfWriter(&oApplicationFonts);
                               pdfWriter.SetTempFolder(sTemp);
                               pdfWriter.SetThemesPlace(sThemeDir);
                               int nReg = (bPaid == false) ? 0 : 1;
                               nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFilePathIn, sFilePathOut)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
   int PdfDjvuXpsToRenderer(IOfficeDrawingFile** ppReader, IRenderer* pRenderer, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, CApplicationFonts* pApplicationFonts)
   {
       int nRes = 0;
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

	int PdfDjvuXpsToImage(IOfficeDrawingFile** ppReader, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, CApplicationFonts* pApplicationFonts)
	{
		int nRes = 0;
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
					FileSystem::Directory::CreateDirectory(sThumbnailDir);
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
					nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo, -1) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
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

   int fromDocxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo))
       {
           if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo)
           {
               nRes = dir2zip(sFrom, sTo);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatTo)
           {
               nRes = docx_dir2doc(sFrom, sTo, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
           {
               nRes = docx_dir2odt(sFrom, sTo, sTemp, params);
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
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       else if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
       {
           nRes = docx_dir2doct_bin(sFrom, sTo, params);
       }
       else
       {
           std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
           FileSystem::Directory::CreateDirectory(sDoctDir);
           std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

		   nRes = docx_dir2doct_bin(sFrom, sTFile, params);
          
		   if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }
   int fromDoctBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
       {
           std::wstring sFromDir = FileSystem::Directory::GetFolderPath(sFrom);
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
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
       return nRes;
   }
   int fromDocument(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
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

       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
       {
           if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
           {
               std::wstring sDoctDir = FileSystem::Directory::GetFolderPath(sTo);
               
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
                   nRes = html2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
                   nRes = mht2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
                   nRes = epub2doct_dir(sFrom, sDoctDir, sTemp, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT;
           }
           else
           {
               std::wstring sDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
               FileSystem::Directory::CreateDirectory(sDoctDir);
               std::wstring sTFile = sDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
               
			   if(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
                   nRes = html2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
                   nRes = mht2doct_dir(sFrom, sDoctDir, sTemp, params);
               else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
                   nRes = epub2doct_dir(sFrom, sDoctDir, sTemp, params);
               else
                   nRes = AVS_FILEUTILS_ERROR_CONVERT;
               
			   if(SUCCEEDED_X2T(nRes))
               {
				   nRes = fromDoctBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
               }
           }
       }
       else
       {
           std::wstring sDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
           FileSystem::Directory::CreateDirectory(sDocxDir);
           if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatFrom)
           {
               nRes = zip2dir(sFrom, sDocxDir);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatFrom)
           {
               nRes = doc2docx_dir(sFrom, sDocxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatFrom)
           {
               nRes = odf2oox_dir(sFrom, sDocxDir, sTemp, params);
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
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromDocxDir(sDocxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }

   int fromXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo) && AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV != nFormatTo)
       {
           if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo)
           {
               nRes = dir2zip(sFrom, sTo);
           }
           //else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormatTo)
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatTo)
           {
               nRes = xlsx_dir2ods(sFrom, sTo, sTemp, params);
           }
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       else if(AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo)
       {
           nRes = xlsx_dir2xlst_bin(sFrom, sTo, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)
       {
           nRes = xlsx_dir2xlst_bin(sFrom, sTo, params);
       }
       else
       {
           std::wstring sXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
           FileSystem::Directory::CreateDirectory(sXlstDir);
           std::wstring sTFile = sXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
           if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
               nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params);
           else
               nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params);
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromXlstBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }
   int fromXlstBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
       {
           std::wstring sFromDir = FileSystem::Directory::GetFolderPath(sFrom);
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
                nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       return nRes;
   }
   int fromSpreadsheet(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
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

       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
       {
           if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)
           {
               nRes = csv2xlst_bin(sFrom, sTo, params);
           }
           else
           {
               std::wstring sXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
               FileSystem::Directory::CreateDirectory(sXlstDir);
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
           std::wstring sXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
           FileSystem::Directory::CreateDirectory(sXlsxDir);
           if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom)
           {
               nRes = zip2dir(sFrom, sXlsxDir);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormatFrom)
           {
               nRes = xls2xlsx_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatFrom)
           {
               nRes = odf2oox_dir(sFrom, sXlsxDir, sTemp, params);
           }
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromXlsxDir(sXlsxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }

   int fromPptxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatTo))
       {
           if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo)
           {
               nRes = dir2zip(sFrom, sTo);
           }
           //else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatTo)
           //else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatTo)
           else
               nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       else if(AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == nFormatTo)
       {
           nRes = pptx_dir2pptt_bin(sFrom, sTo, params);
       }
       else
       {
           std::wstring sPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
           FileSystem::Directory::CreateDirectory(sPpttDir);
           std::wstring sTFile = sPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");
         
		   nRes = pptx_dir2pptt_bin(sFrom, sTFile, params);
           if(SUCCEEDED_X2T(nRes))
           {
               nRes = fromPpttBin(sTFile, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
           }
       }
       return nRes;
   }
   int fromPpttBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatTo)
       {
           std::wstring sFromDir = FileSystem::Directory::GetFolderPath(sFrom);
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
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       return nRes;
   }
   int fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
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

       int nRes = 0;
       std::wstring sPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
       FileSystem::Directory::CreateDirectory(sPptxDir);
       if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatFrom)
       {
           nRes = zip2dir(sFrom, sPptxDir);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatFrom)
       {
           nRes = ppt2pptx_dir(sFrom, sPptxDir, sTemp, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatFrom)
       {
           nRes = odf2oox_dir(sFrom, sPptxDir, sTemp, params);
       }
       else if(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatFrom)
       {
           nRes = ppsx2pptx_dir(sFrom, sPptxDir, params);
       }
       else
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       if(SUCCEEDED_X2T(nRes))
       {
           nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
       }
       return nRes;
   }

   int fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
       {
           std::wstring sTDir = FileSystem::Directory::GetFolderPath(sTo);
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
                   nRes = AVS_FILEUTILS_ERROR_CONVERT;
           }
       }
       return nRes;
   }
   int fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       CApplicationFonts oApplicationFonts;
       initApplicationFonts(oApplicationFonts, params);
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           if(nFormatFrom == nFormatTo)
           {
                NSFile::CFileBinary::Copy(sFrom, sTo);
           }
           else
           {
               CPdfRenderer pdfWriter(&oApplicationFonts);
               pdfWriter.SetTempFolder(sTemp);
               IOfficeDrawingFile* pReader = NULL;
               nRes = PdfDjvuXpsToRenderer(&pReader, &pdfWriter, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
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
           nRes = PdfDjvuXpsToRenderer(&pReader, &oHtmlRenderer, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
           oHtmlRenderer.CloseFile();
           RELEASEOBJECT(pReader);
       }
	   else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
	   {
		   IOfficeDrawingFile* pReader = NULL;
		   nRes = PdfDjvuXpsToImage(&pReader, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
		   RELEASEOBJECT(pReader);
	   }
       else
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
   int fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           nRes = bin2pdf(sFrom, sTo, sTemp, bPaid, sThemeDir, params);
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
       {
           nRes = bin2image(sFrom, sTo, sTemp, sThemeDir, params);
       }
       else
       {
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }

   // xls -> xlsx
   int xls2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

       FileSystem::Directory::CreateDirectory(sResultDocxDir);

       int nRes = xls2xlsx_dir(sFrom, sResultDocxDir, sTemp, params);
       if(SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDocxDir, sTo, -1))
               return 0;
       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
   int xls2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       long hRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), NULL);
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

   // xls -> xlst
   int xls2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // Extract xlsx to temp directory
       std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
       std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");

       FileSystem::Directory::CreateDirectory(sResultDoctDir);

	   int nRes = xls2xlst_bin(sFrom, sResultDoctFileEditor, sTemp, params);

       if (SUCCEEDED_X2T(nRes))
       {
           COfficeUtils oCOfficeUtils(NULL);
           nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       }

       return nRes;
   }

   // xls -> xlst_bin
   int xls2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");

        FileSystem::Directory::CreateDirectory(sResultXlsxDir);

		if (ConvertXls2Xlsx( sFrom, sResultXlsxDir, params.getPassword(), params.getFontPath(), NULL)== S_OK)
        {
            BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;

            CString sFontPath = std_string2string(params.getFontPath());
            m_oCXlsxSerializer.setFontDir(sFontPath);

            CString sTo1 = std_string2string(sTo);

			int res =  m_oCXlsxSerializer.saveToFile (sTo1, std_string2string(sResultXlsxDir), std_string2string(params.getXmlOptions())) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;

            return res;

        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }

	int fromInputParams(InputParams& oInputParams)
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

		std::wstring sTempDir = FileSystem::Directory::GetFolderPath(sFileFrom) + FILE_SEPARATOR_STR + _T("Temp");
		NSDirectory::CreateDirectory(sTempDir);

		if (sTempDir.empty())
		{
			std::cerr << "Couldn't create temp folder" << std::endl;
			return AVS_FILEUTILS_ERROR_UNKNOWN;
		}

		int result = 0;
		switch(conversion)
		{
			case TCD_DOCX2DOCT:
			{
				result = docx2doct (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_DOCT2DOCX:
			{
				result =  doct2docx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_XLSX2XLST:
			{
				result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLST2XLSX:
			{
				result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
			}break;
			case TCD_PPTX2PPTT:
			{
				result =  pptx2pptt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPTT2PPTX:
			{
				result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, bFromChanges, sThemeDir, oInputParams);
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
				result =  dir2zip (FileSystem::Directory::GetFolderPath(sFileFrom), sFileTo);
			}break;
			case TCD_T2BIN:
			{
				result =  zip2dir (sFileFrom, FileSystem::Directory::GetFolderPath(sFileTo));
			}break;
			case TCD_PPSX2PPTX:
			{
				result =  ppsx2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_PPT2PPTX:
			{
				result =  ppt2pptx (sFileFrom, sFileTo, sTempDir, oInputParams);
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
			case TCD_XLS2XLST:
			{
				result = xls2xlst (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLS2XLST_BIN:
			{
				result = xls2xlst_bin (sFileFrom, sFileTo, sTempDir, oInputParams);
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
			case TCD_DOCX2ODT:
			{
				result =  docx2odt (sFileFrom, sFileTo, sTempDir, oInputParams);
			}break;
			case TCD_XLSX2ODS:
			{
				result =  xlsx2ods (sFileFrom, sFileTo, sTempDir, oInputParams);
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
		}

		// delete temp dir
		NSDirectory::DeleteDirectory(sTempDir);

		//clean up v8
		NSDoctRenderer::CDocBuilder::Dispose();
		return result;
	}

	int FromFile(const std::wstring& file)
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

	int FromXml(const std::wstring& xml)
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

