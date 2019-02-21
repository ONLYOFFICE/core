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
#include "../../ASCOfficeRtfFile/RtfFormatLib/source/ConvertationManager.h"
#include "../../ASCOfficeTxtFile/TxtXmlFormatLib/Source/TxtXmlFile.h"
#include "../../ASCOfficePPTFile/PPTFormatLib/PPTFormatLib.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../PdfReader/Src/ErrorConstants.h"
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
    bool replaceContentType(const std::wstring &sDir, const std::wstring &sCTFrom, const std::wstring &sCTTo)
    {
        bool res = false;
        std::wstring sContentTypesPath = sDir + FILE_SEPARATOR_STR + _T("[Content_Types].xml");
        if (NSFile::CFileBinary::Exists(sContentTypesPath))
        {
            std::wstring sData;
            if (NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
            {
                sData = string_replaceAll(sData, sCTFrom, sCTTo);
                res = NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true);
            }
        }
        return res;
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
        
        if (SUCCEEDED_X2T(nRes))
        {
            // compress
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultDocxDir, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
    // bin -> docx dir
    _UINT32 doct_bin2docx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;
        std::wstring sTargetBin;
        //if(bFromChanges)
        //    nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
        //else
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
                    return mscrypt2oot_bin(sFrom, sTo, sTemp, params);
                else
                {
                    //вместо xlsx другой формат!!
                }
            }
            else return AVS_FILEUTILS_ERROR_CONVERT;        }
        
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
        if (SUCCEEDED_X2T(nRes))
        {
            // compress
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultXlsxDir, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
    _UINT32 xlst_bin2xlsx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        _UINT32 nRes = 0;
        
        std::wstring sTargetBin;
        //if(bFromChanges)
        //    nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
        //else
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
        
        int nRes = xlsm2xlsx_dir(sFrom, sTempUnpackedXLSX, params);
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
        
        int nRes = xltm2xlsx_dir(sFrom, sTempUnpackedXLSX, params);
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
        
        int nRes = xltm2xlsm_dir(sFrom, sTempUnpackedXLSM, params);
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
        
        int nRes = 0;
        
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
        
        int nRes = pptx2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
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
        int nRes = 0;
        std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        
        NSDirectory::CreateDirectory(sResultPptxDir);
        
        nRes = pptt_bin2pptx_dir(sFrom, sTo, sResultPptxDir, bFromChanges, sThemeDir, params);
        if (!SUCCEEDED_X2T(nRes))
            return nRes;
        
        // zip pptx folder to output file
        COfficeUtils oCOfficeUtils(NULL);
        nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPptxDir,sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        
        return nRes;
    }
    _UINT32 pptt_bin2pptx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params)
    {
        int nRes = 0;
        
        std::wstring sTargetBin;
        //if(bFromChanges)
        //    nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sThemeDir, sTargetBin, params);
        //else
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
        
        int nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sCSV, params.getXmlOptions());
        
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
        
        int nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sCSV, params.getXmlOptions());
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
        
        std::wstring sXMLOptions = _T("");
        int nRes = m_oCXlsxSerializer.saveToFile (sResultXlstFileEditor, sTempUnpackedXLSX, sXMLOptions);
        if (SUCCEEDED_X2T(nRes))
        {
            std::wstring sMediaPath;
            std::wstring sEmbedPath;
            
            nRes = m_oCXlsxSerializer.loadFromFile (sResultXlstDir, sCSV, sXMLOptions, sMediaPath, sEmbedPath);
        }
        
        return nRes;
    }
    _UINT32 xlst_bin2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, InputParams& params)
    {
        int nRes = 0;
        
        std::wstring sTargetBin;
        //if(bFromChanges)
        //    nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sThemeDir, sTargetBin, params);
        //else
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
        long nRes = 0;
//        CApplicationFonts oApplicationFonts;
//        initApplicationFonts(oApplicationFonts, params);
//        NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
//        imageWriter.wsHtmlPlace = sTFileDir;
//        imageWriter.wsThemesPlace = sThemeDir;
//        imageWriter.wsTempDir = sTemp;
//        imageWriter.appFonts = &oApplicationFonts;
//        if(NULL != params.m_oThumbnail)
//        {
//            InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
//            if(NULL != oThumbnail->format)
//            {
//                imageWriter.m_nRasterFormat = *oThumbnail->format;
//            }
//            if(NULL != oThumbnail->aspect)
//            {
//                imageWriter.m_nSaveType = *oThumbnail->aspect;
//            }
//            if(NULL != oThumbnail->first)
//            {
//                imageWriter.m_bIsOnlyFirst = *oThumbnail->first;
//            }
//            if(NULL != oThumbnail->width)
//            {
//                imageWriter.m_nRasterW = *oThumbnail->width;
//            }
//            if(NULL != oThumbnail->height)
//            {
//                imageWriter.m_nRasterH = *oThumbnail->height;
//            }
//        }
//        std::wstring sThumbnailDir;
//        if(imageWriter.m_bIsOnlyFirst)
//        {
//            imageWriter.m_sFileName = sTo;
//        }
//        else
//        {
//            sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
//            NSDirectory::CreateDirectory(sThumbnailDir);
//            imageWriter.m_sFileName = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + getExtentionByRasterFormat(imageWriter.m_nRasterFormat);
//        }
//        nRes = imageWriter.ConvertBuffer(pBuffer, lBufferLen) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//        if(!imageWriter.m_bIsOnlyFirst)
//        {
//            COfficeUtils oCOfficeUtils(NULL);
//            nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//        }
        return nRes;
    }
    _UINT32 bin2imageBase64 (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params)
    {
        long nRes = 0;
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
       int nRes = 0;
//       NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
//       
//	   std::wstring sTFileDir			= NSDirectory::GetFolderPath(sFrom);
//       
//	   std::wstring sImagesDirectory	= sTFileDir + FILE_SEPARATOR_STR + L"media";
//       std::wstring sPdfBinFile			= sTFileDir + FILE_SEPARATOR_STR + L"pdf.bin";
//
//	   NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
//       std::wstring sXml = getDoctXml(eFromType, eToType, sTFileDir, sPdfBinFile, sImagesDirectory, sThemeDir, -1, L"", params);
//       std::wstring sResult;
//       bool bRes = oDoctRenderer.Execute(sXml, sResult);
//       if (-1 != sResult.find(L"error"))
//       {
//           std::wcerr << L"DoctRenderer:" << sResult << std::endl;
//           nRes = AVS_FILEUTILS_ERROR_CONVERT;
//       }
//       else
//       {
//           CApplicationFonts oApplicationFonts;
//           initApplicationFonts(oApplicationFonts, params);
//           CPdfRenderer pdfWriter(&oApplicationFonts);
//           pdfWriter.SetTempFolder(sTemp);
//           pdfWriter.SetThemesPlace(sThemeDir);
//           int nReg = (bPaid == false) ? 0 : 1;
//           nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sPdfBinFile, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//       }
//       //удаляем sPdfBinFile, потому что он не в Temp
//       if (NSFile::CFileBinary::Exists(sPdfBinFile))
//           NSFile::CFileBinary::Remove(sPdfBinFile);
       return nRes;
   }
	//doct_bin -> image
	_UINT32 doct_bin2image(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
	{
		int nRes = 0;
//		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
//		
//		std::wstring sTFileDir = NSDirectory::GetFolderPath(sFrom);
//		
//		std::wstring sImagesDirectory	= sTFileDir + FILE_SEPARATOR_STR + L"media";
//		std::wstring sPdfBinFile		= sTFileDir + FILE_SEPARATOR_STR + L"pdf.bin";
//
//		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
//		std::wstring sXml = getDoctXml(eFromType, eToType, sTFileDir, sPdfBinFile, sImagesDirectory, sThemeDir, -1, L"", params);
//		std::wstring sResult;
//		bool bRes = oDoctRenderer.Execute(sXml, sResult);
//		if (-1 != sResult.find(L"error"))
//		{
//			std::wcerr << L"DoctRenderer:" << sResult << std::endl;
//			nRes = AVS_FILEUTILS_ERROR_CONVERT;
//		}
//		else
//		{
//			CApplicationFonts oApplicationFonts;
//			initApplicationFonts(oApplicationFonts, params);
//			NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
//			imageWriter.wsHtmlPlace = sTFileDir;
//			imageWriter.wsThemesPlace = sThemeDir;
//			imageWriter.wsTempDir = sTemp;
//			imageWriter.appFonts = &oApplicationFonts;
//			if(NULL != params.m_oThumbnail)
//			{
//				InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
//				if(NULL != oThumbnail->format)
//				{
//					imageWriter.m_nRasterFormat = *oThumbnail->format;
//				}
//				if(NULL != oThumbnail->aspect)
//				{
//					imageWriter.m_nSaveType = *oThumbnail->aspect;
//				}
//				if(NULL != oThumbnail->first)
//				{
//					imageWriter.m_bIsOnlyFirst = *oThumbnail->first;
//				}
//				if(NULL != oThumbnail->width)
//				{
//					imageWriter.m_nRasterW = *oThumbnail->width;
//				}
//				if(NULL != oThumbnail->height)
//				{
//					imageWriter.m_nRasterH = *oThumbnail->height;
//				}
//			}
//			std::wstring sThumbnailDir;
//			if(imageWriter.m_bIsOnlyFirst)
//			{
//				imageWriter.m_sFileName = sTo;
//			}
//			else
//			{
//				sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
//				NSDirectory::CreateDirectory(sThumbnailDir);
//				imageWriter.m_sFileName = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + getExtentionByRasterFormat(imageWriter.m_nRasterFormat);
//			}
//			BYTE* pData;
//			DWORD nBytesCount;
//			NSFile::CFileBinary::ReadAllBytes(sPdfBinFile, &pData, nBytesCount);
//			nRes = imageWriter.ConvertBuffer(pData, nBytesCount) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//			if(!imageWriter.m_bIsOnlyFirst)
//			{
//				COfficeUtils oCOfficeUtils(NULL);
//				nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo, -1) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//			}
//		}
		return nRes;
	}

   // ppsx -> pptx
   _UINT32 ppsx2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
//	   std::wstring sTempUnpackedPPSX = sTemp + FILE_SEPARATOR_STR + L"ppsx_unpacked";
//	   NSDirectory::CreateDirectory(sTempUnpackedPPSX);
//
//       int nRes = ppsx2pptx_dir(sFrom, sTempUnpackedPPSX, params);
//       if(SUCCEEDED_X2T(nRes))
//       {
//           COfficeUtils oCOfficeUtils(NULL);
//           if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedPPSX, sTo, -1))
//               return 0;
//       }
	   return AVS_FILEUTILS_ERROR_CONVERT;
   }
   _UINT32 ppsx2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params)
   {
//       COfficeUtils oCOfficeUtils(NULL);
//       if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0))
//       {
//           std::wstring sContentTypesPath = sTo + FILE_SEPARATOR_STR + L"[Content_Types].xml";
//           if(NSFile::CFileBinary::Exists(sContentTypesPath))
//           {
//               std::wstring sData;
//               if(NSFile::CFileBinary::ReadAllTextUtf8(sContentTypesPath, sData))
//               {
//                   std::wstring sCTFrom = L"application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml";
//                   std::wstring sCTTo	= L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml";
//
//                   sData = string_replaceAll(sData, sCTFrom, sCTTo);
//
//                   if(NSFile::CFileBinary::SaveToFile(sContentTypesPath, sData, true))
//                   {
//                       return 0;
//                   }
//               }
//           }
//       }
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
    // pptm -> pptx
    _UINT32 pptm2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedPPTX);
        
        int nRes = pptm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
        
        int nRes = potm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
        
        int nRes = ppsm2pptx_dir(sFrom, sTempUnpackedPPTX, params);
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
                    int pos = sData.find(L"vbaProject.bin");
                    if (pos > 0)
                    {
                        int pos1 = sData.rfind(L"<", pos);
                        int pos2 = sData.find(L">", pos);
                        
                        if (pos1 > 0 && pos2 > 0)
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
        
        int nRes = potx2pptx_dir(sFrom, sTempUnpackedPPTX, params);
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
        
        int nRes = potm2pptm_dir(sFrom, sTempUnpackedPPTM, params);
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
        
        int nRes = ppsm2pptm_dir(sFrom, sTempUnpackedPPTM, params);
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
        
        int hRes = ppt2pptx_dir(sFrom, sResultPptxDir, sTemp, params);
        
        if(SUCCEEDED_X2T(hRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultPptxDir, sTo, true))
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
    _UINT32 ppt2pptx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        //COfficePPTFile pptFile;
        
        //pptFile.put_TempDirectory(sTemp);
        //return S_OK == pptFile.LoadFromFile(sFrom, sTo, params.getPassword()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
   // ppt -> pptt
   _UINT32 ppt2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       //std::wstring sResultPpttDir		= sTemp				+ FILE_SEPARATOR_STR + L"pptt_unpacked";
       //std::wstring sTempPpttFileEditor = sResultPpttDir	+ FILE_SEPARATOR_STR + L"Editor.bin";

       //NSDirectory::CreateDirectory(sResultPpttDir);

       //int nRes = ppt2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, params);
       //if (SUCCEEDED_X2T(nRes))
       //{
       //    // zip pptt folder to output file
       //    COfficeUtils oCOfficeUtils(NULL);
       //    nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
       //}
       //return nRes;
       
       return AVS_FILEUTILS_ERROR_CONVERT;
   }
   // ppt -> pptt_bin
   _UINT32 ppt2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       // unzip pptx to temp folder
//       std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + L"pptx_unpacked"+ FILE_SEPARATOR_STR;  // leading slash is very important!
//
//       NSDirectory::CreateDirectory(sTempUnpackedPPTX);
//
//       COfficePPTFile pptFile;
//       
//       pptFile.put_TempDirectory(sTemp);
//       
//       if ( pptFile.LoadFromFile(sFrom, sTempUnpackedPPTX, params.getPassword()) != S_OK) return AVS_FILEUTILS_ERROR_CONVERT;
//
//       // convert unzipped pptx to unzipped pptt
//       CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);
//
//       int nRes = 0;
//
//       if (pptx_file)
//       {
//#if defined(_WIN32) || defined (_WIN64)
//           BSTR bstrFontPath			= SysAllocString(params.getFontPath().c_str());
//           BSTR bstrTempUnpackedPPTX	= SysAllocString(sTempUnpackedPPTX.c_str());
//           BSTR bstrTo					= SysAllocString(sTo.c_str());
//
//           pptx_file->SetFontDir (bstrFontPath);
//           nRes = (S_OK == pptx_file->OpenFileToPPTY (bstrTempUnpackedPPTX, bstrTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//
//           SysFreeString(bstrFontPath);
//           SysFreeString(bstrTempUnpackedPPTX);
//           SysFreeString(bstrTo);
//#else
//           pptx_file->SetFontDir (params.getFontPath());
//           nRes = (S_OK == pptx_file->OpenFileToPPTY (sTempUnpackedPPTX, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//#endif
//           delete pptx_file;
//       }
//
//       return nRes;
       
       return AVS_FILEUTILS_ERROR_CONVERT;
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
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    // rtf -> docx
    _UINT32 rtf2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        
        NSDirectory::CreateDirectory(sResultDocxDir);
        int nRes = rtf2docx_dir(sFrom, sResultDocxDir, sTemp, params);
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
        return S_OK == rtfConvert.ConvertRtfToOOX(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }

    // rtf -> doct
    _UINT32 rtf2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        NSDirectory::CreateDirectory(sResultDoctDir);
        
        int nRes = rtf2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);
        
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
        
        if ( rtfConvert.ConvertRtfToOOX(sFrom, sResultDocxDir) == S_OK)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;
            
            m_oCDocxSerializer.setFontDir(params.getFontPath());
            
            std::wstring sXmlOptions;
            int res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, sXmlOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
            
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
        
        if (rtfConvert.ConvertOOXToRtf(sTo, sDocxDir) == S_OK)
            return 0;
        return AVS_FILEUTILS_ERROR_CONVERT;
    }

    // doc -> docx
    _UINT32 doc2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        
        NSDirectory::CreateDirectory(sResultDocxDir);
        
        long hRes = doc2docx_dir(sFrom, sResultDocxDir, sTemp, params);
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
//        COfficeDocFile docFile;
//		docFile.m_sTempFolder = sTemp;
//		long hRes = docFile.LoadFromFile( sFrom, sTo, params.getPassword(), NULL);
//		if (AVS_ERROR_DRM == hRes)
//		{
//			if(!params.getDontSaveAdditional())
//			{
//				copyOrigin(sFrom, *params.m_sFileTo);
//			}
//			return AVS_FILEUTILS_ERROR_CONVERT_DRM;
//		}
//		else if (AVS_ERROR_PASSWORD == hRes)
//		{
//			return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
//		}
//		return 0 == hRes ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       return AVS_FILEUTILS_ERROR_CONVERT;
   }

    // doc -> doct
    _UINT32 doc2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        // Extract docx to temp directory
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        NSDirectory::CreateDirectory(sResultDoctDir);
        
        long nRes = doc2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);
        
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
//        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + L"docx_unpacked";
//
//        NSDirectory::CreateDirectory(sResultDocxDir);
//
//        COfficeDocFile docFile;
//		docFile.m_sTempFolder = sTemp;
//
//        long nRes = docFile.LoadFromFile( sFrom, sResultDocxDir, params.getPassword(), NULL);
//
//        if (SUCCEEDED_X2T(nRes))
//        {
//            BinDocxRW::CDocxSerializer m_oCDocxSerializer;
//
//            m_oCDocxSerializer.setFontDir(params.getFontPath());
//
//			std::wstring xml_options = params.getXmlOptions();
//
//			int res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, xml_options) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//
//            return res;
//        }
//        else if (AVS_ERROR_DRM == nRes)
//        {
//            if(!params.getDontSaveAdditional())
//            {
//                copyOrigin(sFrom, *params.m_sFileTo);
//            }
//            return AVS_FILEUTILS_ERROR_CONVERT_DRM;
//        }
//        else if (AVS_ERROR_PASSWORD == nRes)
//        {
//            return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
//        }
        return AVS_FILEUTILS_ERROR_CONVERT;
   }
   _UINT32 docx_dir2doc (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       return AVS_FILEUTILS_ERROR_CONVERT;
       //COfficeDocFile docFile;
       //return /*S_OK == docFile.SaveToFile(sTo, sDocxDir, NULL) ? 0 : */AVS_FILEUTILS_ERROR_CONVERT;
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
        int nRes = 0;
        std::wstring sResultDocxDir = sTemp + FILE_SEPARATOR_STR + L"docx_unpacked";
        
        NSDirectory::CreateDirectory(sResultDocxDir);
        
        std::wstring sTargetBin;
        //if(bFromChanges)
        //     nRes = apply_changes(sFrom, _T(""), NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sThemeDir, sTargetBin, params);
       //else
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
                
                if (rtfConvert.ConvertOOXToRtf(sTo, sResultDocxDir) == S_OK)
                    return 0;
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
        int nRes = txt2docx_dir(sFrom, sResultDocxDir, sTemp, params);
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
        
        return S_OK == txtFile.txt_LoadFromFile(sFrom, sTo, xml_options) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    // txt -> doct
    _UINT32 txt2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        NSDirectory::CreateDirectory(sResultDoctDir);
        
        int nRes = txt2doct_bin(sFrom, sResultDoctFileEditor, sTemp, params);
        
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
        
        if ( txtFile.txt_LoadFromFile(sFrom, sResultDocxDir, params.getXmlOptions()) == S_OK)
        {
            BinDocxRW::CDocxSerializer m_oCDocxSerializer;
            
            m_oCDocxSerializer.setFontDir(params.getFontPath());
            
            int res =  m_oCDocxSerializer.saveToFile (sTo, sResultDocxDir, params.getXmlOptions()) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
            
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
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    _UINT32 odf2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    _UINT32 odf2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    _UINT32 odf2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    //odf flat
    _UINT32 odf_flat2oot(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    _UINT32 odf_flat2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    _UINT32 odf_flat2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    _UINT32 odf_flat2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // docx -> odt
    _UINT32 docx2odt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    // docx dir -> odt
    _UINT32 docx_dir2odt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    // xlsx -> ods
    _UINT32 xlsx2ods (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params )
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    _UINT32 xlsx_dir2ods (const std::wstring &sXlsxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    _UINT32 mscrypt2oot (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        std::wstring sResultOotDir            = sTemp            + FILE_SEPARATOR_STR + _T("oot_unpacked");
        std::wstring sResultOotFileEditor    = sResultOotDir    + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        NSDirectory::CreateDirectory(sResultOotDir);
        
        int nRes = mscrypt2oot_bin(sFrom, sResultOotFileEditor, sTemp, params);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultOotDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        
        return nRes;
    }
    _UINT32 mscrypt2oox     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        std::wstring password = params.getPassword();
        
        ECMACryptFile cryptReader;
        bool bDataIntegrity = false;
        
        if (cryptReader.DecryptOfficeFile(sFrom, sTo, password, bDataIntegrity) == false)
        {
            if (password.empty())    return AVS_FILEUTILS_ERROR_CONVERT_DRM;
            else                    return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
        }
        
        if (bDataIntegrity == false)
        {
            //было несанкционированое вешательство в файл
        }
        
        return S_OK;
    }
    _UINT32 mscrypt2oot_bin     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        //decrypt to temp file
        std::wstring sResultDecryptFile = sTemp    + FILE_SEPARATOR_STR + L"uncrypt_file.oox";
        
        ECMACryptFile cryptReader;
        bool bDataIntegrity = false;
        
        if (cryptReader.DecryptOfficeFile(sFrom, sResultDecryptFile, params.getPassword(), bDataIntegrity) == false)
            return AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
        
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
    _UINT32 oox2mscrypt     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        std::wstring password    = params.getSavePassword();
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
        
        std::wstring sResultDecryptFile = sTemp    + FILE_SEPARATOR_STR + L"uncrypt_file.oox";
        
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
   int html2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
       std::vector<std::wstring> arFiles;
       arFiles.push_back(sFrom);

//       CHtmlFile oHtmlFile;
//       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
//       return 0 == oHtmlFile.Convert(arFiles, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       return 0;
   }
   //mht
   int mht2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
//       CHtmlFile oHtmlFile;
//       std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
//       return 0 == oHtmlFile.ConvertMht(sFrom, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
       return 0;
   }
   int epub2doct_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
   {
//       std::wstring sEpubDir = sTemp + FILE_SEPARATOR_STR + L"epub_unpacked";
//       NSDirectory::CreateDirectory(sEpubDir);
//       int nRes = zip2dir(sFrom, sEpubDir);
//       if(SUCCEEDED_X2T(nRes))
//       {
//           CHtmlFile oHtmlFile;
//           std::wstring sMetaInfo;
//           std::wstring sPathInternal = NULL != params.m_sHtmlFileInternalPath ? *params.m_sHtmlFileInternalPath : L"";
//           nRes = 0 == oHtmlFile.ConvertEpub(sEpubDir, sMetaInfo, sTo, sPathInternal) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//       }
//       return nRes;
       return 0;
   }
   // mailmerge
   _UINT32 convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params)
   {
//       if(NULL == oMailMergeSend.mailFormat || NULL == oMailMergeSend.recordFrom || NULL == oMailMergeSend.recordTo)
//           return AVS_FILEUTILS_ERROR_CONVERT;
//       int nRes = 0;
//       std::wstring sFileFromDir		= NSSystemPath::GetDirectoryName(sFrom);
//       std::wstring sFileToDir			= NSSystemPath::GetDirectoryName(sTo);
//       std::wstring sImagesDirectory	= sFileFromDir + FILE_SEPARATOR_STR +L"media";
//
//       NSDoctRenderer::DoctRendererFormat::FormatFile eTypeTo;
//       switch(*oMailMergeSend.mailFormat)
//       {
//       case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:		eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;	break;
//       case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:	eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;	break;
//       case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP:		eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;	break;
//       default:											eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;	break;
//       }
//       std::wstring sJsonPath = sFileFromDir + FILE_SEPARATOR_STR + L"Editor.json";
//       int recordTo = *oMailMergeSend.recordFrom + 4;
//       if(recordTo > *oMailMergeSend.recordTo)
//           recordTo = *oMailMergeSend.recordTo;
//	   NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
//       std::wstring sMailMergeXml = getMailMergeXml(sJsonPath, *oMailMergeSend.recordFrom, recordTo, *oMailMergeSend.to);
//       //посылаем выходную папку sFileFromDir, чтобы файлы лежали на одном уровне с папкой media, важно для дальнейшей конвертации в docx, pdf
//       std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, eTypeTo, sFileFromDir, sFileFromDir, sImagesDirectory, sThemeDir, -1, sMailMergeXml, params);
//       std::wstring sResult;
//       oDoctRenderer.Execute(sXml, sResult);
//       if (-1 != sResult.find(L"error"))
//       {
//           std::wcerr << L"DoctRenderer:" << sResult << std::endl;
//           nRes = AVS_FILEUTILS_ERROR_CONVERT;
//       }
//       else
//       {
//           nRes = AVS_FILEUTILS_ERROR_CONVERT;
//           XmlUtils::CXmlNode oRoot;
//           if(TRUE == oRoot.FromXmlString(sResult))
//           {
//               XmlUtils::CXmlNode oMailMergeFields = oRoot.ReadNode(L"MailMergeFields");
//               XmlUtils::CXmlNodes oXmlNodes;
//               if(TRUE == oMailMergeFields.GetChilds(oXmlNodes))
//               {
//                   for(int i = 0; i < oXmlNodes.GetCount(); ++i)
//                   {
//                       XmlUtils::CXmlNode oXmlNode;
//                       if(oXmlNodes.GetAt(i, oXmlNode))
//                       {
//                           std::wstring sFilePathIn				= oXmlNode.GetAttribute(L"path");
//                           std::wstring wsFilePathInFilename	= NSSystemPath::GetFileName(sFilePathIn);
//                           std::wstring sFilePathOut			= sFileToDir + FILE_SEPARATOR_STR + wsFilePathInFilename;
//                           
//						   if (NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT == eTypeTo)
//                           {
//                               sFilePathOut += L".docx";
//
//                               std::wstring sTempDocx = sTemp + FILE_SEPARATOR_STR + wsFilePathInFilename + L"_DOCX";
//                               NSDirectory::CreateDirectory(sTempDocx);
//
//                               BinDocxRW::CDocxSerializer m_oCDocxSerializer;
//
//                               m_oCDocxSerializer.setFontDir(params.getFontPath());
//
//                               std::wstring sXmlOptions = L"";
//                               std::wstring sThemePath;             // will be filled by 'CreateDocxFolders' method
//                               std::wstring sMediaPath;             // will be filled by 'CreateDocxFolders' method
//                               std::wstring sEmbedPath;             // will be filled by 'CreateDocxFolders' method
//
//                               m_oCDocxSerializer.CreateDocxFolders (sTempDocx, sThemePath, sMediaPath, sEmbedPath);
//                               nRes = m_oCDocxSerializer.loadFromFile (sFilePathIn, sTempDocx, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//                               if(SUCCEEDED_X2T(nRes))
//                               {
//                                   COfficeUtils oCOfficeUtils(NULL);
//                                   nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempDocx, sFilePathOut, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//                               }
//                           }
//                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::PDF == eTypeTo)
//                           {
//                               sFilePathOut += L".pdf";
//
//                               CApplicationFonts oApplicationFonts;
//                               initApplicationFonts(oApplicationFonts, params);
//                               CPdfRenderer pdfWriter(&oApplicationFonts);
//                               pdfWriter.SetTempFolder(sTemp);
//                               pdfWriter.SetThemesPlace(sThemeDir);
//                               int nReg = (bPaid == false) ? 0 : 1;
//                               nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFilePathIn, sFilePathOut)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//                           }
//                           else if (NSDoctRenderer::DoctRendererFormat::FormatFile::HTML == eTypeTo)
//                           {
//                               sFilePathOut += L".html";
//                               bool bCopy = NSFile::CFileBinary::Copy(sFilePathIn,sFilePathOut);
//                               nRes = bCopy ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
//                           }
//                           if(0 != nRes)
//                               break;
//                           sResult = string_replaceAll(sResult, L"\"" + sFilePathIn + L"\"", L"\"" + NSSystemPath::GetFileName(sFilePathOut) + L"\"");
//                       }
//                   }
//               }
//           }
//           if(SUCCEEDED_X2T(nRes))
//               NSFile::CFileBinary::SaveToFile(sTo, sResult);
//       }
//       return nRes;
       return 0;
   }
//   _UINT32 PdfDjvuXpsToRenderer(IOfficeDrawingFile** ppReader, IRenderer* pRenderer, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, CApplicationFonts* pApplicationFonts)
//   {
//       int nRes = 0;
//       IOfficeDrawingFile* pReader = NULL;
//       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
//       {
//           //pReader = new PdfReader::CPdfReader(pApplicationFonts);
//       }
//       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFormatFrom)
//       {
//           //pReader = new CDjVuFile(pApplicationFonts);
//       }
//       else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFormatFrom)
//       {
//           //pReader = new CXpsFile(pApplicationFonts);
//       }
//       else
//           nRes = AVS_FILEUTILS_ERROR_CONVERT;
//       if(SUCCEEDED_X2T(nRes))
//       {
//           *ppReader = pReader;
//           pReader->SetTempDirectory(sTemp);
//
//           std::wstring sPassword = params.getPassword();
//
//           bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);
//           if(bResult)
//           {
//               int nPagesCount = pReader->GetPagesCount();
//               for (int i = 0; i < nPagesCount; ++i)
//               {
//                   pRenderer->NewPage();
//                   pRenderer->BeginCommand(c_nPageType);
//
//                   double dPageDpiX, dPageDpiY;
//                   double dWidth, dHeight;
//                   pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
//
//                   dWidth  *= 25.4 / dPageDpiX;
//                   dHeight *= 25.4 / dPageDpiY;
//
//                   pRenderer->put_Width(dWidth);
//                   pRenderer->put_Height(dHeight);
//
//                   pReader->DrawPageOnRenderer(pRenderer, i, NULL);
//
//                   pRenderer->EndCommand(c_nPageType);
//               }
//
//           }
//           else
//           {
//               nRes = AVS_FILEUTILS_ERROR_CONVERT;
//               if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
//               {
//                   PdfReader::CPdfReader* pPdfReader = static_cast<PdfReader::CPdfReader*>(pReader);
//                   if(PdfReader::errorEncrypted == pPdfReader->GetError())
//                   {
//                       if(sPassword.empty())
//                       {
//                           if(!params.getDontSaveAdditional())
//                           {
//                               copyOrigin(sFrom, *params.m_sFileTo);
//                           }
//                           nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
//                       }
//                       else
//                       {
//                           nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
//                       }
//                   }
//               }
//           }
//       }
//       return nRes;
//   }

//	_UINT32 PdfDjvuXpsToImage(IOfficeDrawingFile** ppReader, const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, CApplicationFonts* pApplicationFonts)
//	{
//		int nRes = 0;
//		IOfficeDrawingFile* pReader = NULL;
//		if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
//		{
//			//pReader = new PdfReader::CPdfReader(pApplicationFonts);
//		}
//		else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFormatFrom)
//		{
//			//pReader = new CDjVuFile(pApplicationFonts);
//		}
//		else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFormatFrom)
//		{
//			//pReader = new CXpsFile(pApplicationFonts);
//		}
//		else
//			nRes = AVS_FILEUTILS_ERROR_CONVERT;
//		if(SUCCEEDED_X2T(nRes))
//		{
//			*ppReader = pReader;
//			pReader->SetTempDirectory(sTemp);
//
//			std::wstring sPassword = params.getPassword();
//
//			bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);
//			if(bResult)
//			{
//				//default as in CMetafileToRenderterRaster
//				int nRasterFormat = 4;
//				int nSaveType = 2;
//				bool bIsOnlyFirst = true;
//				int nRasterW = 100;
//				int nRasterH = 100;
//				if(NULL != params.m_oThumbnail)
//				{
//					InputParamsThumbnail* oThumbnail = params.m_oThumbnail;
//					if(NULL != oThumbnail->format)
//					{
//						nRasterFormat = *oThumbnail->format;
//					}
//					if(NULL != oThumbnail->aspect)
//					{
//						nSaveType = *oThumbnail->aspect;
//					}
//					if(NULL != oThumbnail->first)
//					{
//						bIsOnlyFirst = *oThumbnail->first;
//					}
//					if(NULL != oThumbnail->width)
//					{
//						nRasterW = *oThumbnail->width;
//					}
//					if(NULL != oThumbnail->height)
//					{
//						nRasterH = *oThumbnail->height;
//					}
//				}
//				std::wstring sThumbnailDir;
//				std::wstring sFileToExt;
//				if (!bIsOnlyFirst)
//				{
//					sThumbnailDir = sTemp + FILE_SEPARATOR_STR + L"thumbnails";
//					NSDirectory::CreateDirectory(sThumbnailDir);
//					sFileToExt = getExtentionByRasterFormat(nRasterFormat);
//				}
//				int nPagesCount = pReader->GetPagesCount();
//				if (bIsOnlyFirst)
//					nPagesCount = 1;
//				for (int i = 0; i < nPagesCount; ++i)
//				{
//					int nRasterWCur = nRasterW;
//					int nRasterHCur = nRasterH;
//
//					if (1 == nSaveType)
//					{
//						double dPageDpiX, dPageDpiY;
//						double dWidth, dHeight;
//						pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
//
//						double dKoef1 = nRasterWCur / dWidth;
//						double dKoef2 = nRasterHCur / dHeight;
//						if (dKoef1 > dKoef2)
//							dKoef1 = dKoef2;
//
//						nRasterWCur = (int)(dWidth * dKoef1 + 0.5);
//						nRasterHCur = (int)(dHeight * dKoef1 + 0.5);
//					}
//					std::wstring sFileTo;
//					if (bIsOnlyFirst)
//					{
//						sFileTo = sTo;
//					}
//					else
//					{
//						sFileTo = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + std::to_wstring(i+1) + sFileToExt;
//					}
//					pReader->ConvertToRaster(i, sFileTo, nRasterFormat, nRasterWCur, nRasterHCur);
//				}
//				//zip
//				if(!bIsOnlyFirst)
//				{
//					COfficeUtils oCOfficeUtils(NULL);
//					nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo, -1) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//				}
//			}
//			else
//			{
//				nRes = AVS_FILEUTILS_ERROR_CONVERT;
//				if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
//				{
//					PdfReader::CPdfReader* pPdfReader = static_cast<PdfReader::CPdfReader*>(pReader);
//					if(PdfReader::errorEncrypted == pPdfReader->GetError())
//					{
//						if(sPassword.empty())
//						{
//							if(!params.getDontSaveAdditional())
//							{
//								copyOrigin(sFrom, *params.m_sFileTo);
//							}
//							nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
//						}
//						else
//						{
//							nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
//						}
//					}
//				}
//			}
//		}
//		return nRes;
//	}

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
                    nRes = replaceContentType(sFrom, sCTFrom, sCTTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
                nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
        return nRes;
    }
    _UINT32 fromDocument(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sTo    = *params.m_sFileTo;
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
                    nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
                nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
                    nRes = replaceContentType(sFrom, sCTFrom, sCTTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
                nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
        int nRes = 0;
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
        nRes = AVS_FILEUTILS_ERROR_CONVERT;
        return nRes;
    }
    _UINT32 fromSpreadsheet(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sTo    = *params.m_sFileTo;
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
                sXlsxFile = sFrom;
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
                nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
                    nRes = replaceContentType(sFrom, sCTFrom, sCTTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
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
                nRes = AVS_FILEUTILS_ERROR_CONVERT;
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
        int nRes = 0;
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
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
        return nRes;
    }
    _UINT32 fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params)
    {
        std::wstring sTo    = *params.m_sFileTo;
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
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
        if(SUCCEEDED_X2T(nRes))
        {
            nRes = fromPptxDir(sPptxDir, sTo, nFormatTo, sTemp, sThemeDir, bFromChanges, bPaid, params);
        }
        return nRes;
    }

    _UINT32 fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
    {
        int nRes = 0;
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
                    nRes = AVS_FILEUTILS_ERROR_CONVERT;
            }
        }
        return nRes;
    }
   _UINT32 fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
   {
       int nRes = 0;
       NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
       initApplicationFonts(pApplicationFonts, params);
       if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
       {
           if(nFormatFrom == nFormatTo)
           {
                NSFile::CFileBinary::Copy(sFrom, sTo);
           }
           else
           {
//               CPdfRenderer pdfWriter(&oApplicationFonts);
//               pdfWriter.SetTempFolder(sTemp);
//               IOfficeDrawingFile* pReader = NULL;
//               nRes = PdfDjvuXpsToRenderer(&pReader, &pdfWriter, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
//               pdfWriter.SaveToFile(sTo);
//               RELEASEOBJECT(pReader);
           }
       }
       else if(0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
       {
//           std::wstring sToDir = NSSystemPath::GetDirectoryName(sTo);
//           if(!params.getDontSaveAdditional())
//           {
//               //save origin to print
//               copyOrigin(sFrom, *params.m_sFileTo);
//           }
//           NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
//           oHtmlRenderer.CreateOfficeFile(sToDir);
//           IOfficeDrawingFile* pReader = NULL;
//           nRes = PdfDjvuXpsToRenderer(&pReader, &oHtmlRenderer, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
//           oHtmlRenderer.CloseFile();
//           RELEASEOBJECT(pReader);
       }
	   else if(0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
	   {
//		   IOfficeDrawingFile* pReader = NULL;
//		   nRes = PdfDjvuXpsToImage(&pReader, sFrom, nFormatFrom, sTo, sTemp, params, &oApplicationFonts);
//		   RELEASEOBJECT(pReader);
	   }
       else
       {
           nRes = AVS_FILEUTILS_ERROR_CONVERT;
       }
       return nRes;
   }
    _UINT32 fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params)
    {
        int nRes = 0;
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
            nRes = AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }

    // xls -> xlsx
    _UINT32 xls2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    _UINT32 xls2xlsx_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {return 0;
//        bool bMacros = false;
//
//        _UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, NULL, bMacros);
//
//        nRes = processEncryptionError(nRes, sFrom, params);
//        return nRes;
    }
    // xls -> xlsm
    _UINT32 xls2xlsm (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {
//        std::wstring sResultXlsmDir = sTemp + FILE_SEPARATOR_STR + _T("xlsm_unpacked");
//
//        NSDirectory::CreateDirectory(sResultXlsmDir);
//
//        _UINT32 nRes = xls2xlsm_dir(sFrom, sResultXlsmDir, sTemp, params);
//
//        if(SUCCEEDED_X2T(nRes))
//        {
//            COfficeUtils oCOfficeUtils(NULL);
//            if(S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlsmDir, sTo, true))
                return 0;
//        }
//        return nRes;
    }
    _UINT32 xls2xlsm_dir (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {return 0;
//        bool bMacros = true;
//
//        _UINT32 nRes = ConvertXls2Xlsx( sFrom, sTo, params.getPassword(), params.getFontPath(), sTemp, NULL, bMacros);
//
//        nRes = processEncryptionError(nRes, sFrom, params);
//        return nRes;
    }
    // xls -> xlst
    _UINT32 xls2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    { return 0;
        // Extract xlsx to temp directory
//        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
//        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
//
//        NSDirectory::CreateDirectory(sResultDoctDir);
//
//        _UINT32 nRes = xls2xlst_bin(sFrom, sResultDoctFileEditor, sTemp, params);
//
//        if (SUCCEEDED_X2T(nRes))
//        {
//            COfficeUtils oCOfficeUtils(NULL);
//            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
//        }
//
//        return nRes;
    }
    
    // xls -> xlst_bin
    _UINT32 xls2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params)
    {return 0;
//        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
//
//        NSDirectory::CreateDirectory(sResultXlsxDir);
//
//        bool bMacros = true;
//        _UINT32 nRes = ConvertXls2Xlsx( sFrom, sResultXlsxDir, params.getPassword(), params.getFontPath(), sTemp, NULL, bMacros);
//
//        nRes = processEncryptionError(nRes, sFrom, params);
//        if (SUCCEEDED_X2T(nRes))
//        {
//            BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
//
//            m_oCXlsxSerializer.setFontDir(params.getFontPath());
//
//            return m_oCXlsxSerializer.saveToFile (sTo, sResultXlsxDir, params.getXmlOptions());
//        }
//        return nRes;
    }
    
}

