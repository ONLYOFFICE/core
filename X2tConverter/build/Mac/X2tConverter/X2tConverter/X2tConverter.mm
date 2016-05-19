//
//  X2tConverter.m
//  X2tConverter
//
//  Created by alexey.musinov on 25.03.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import "X2tConverter.h"

#include <iostream>
#include <fstream>

#include "../../../../core/Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../../../core/ASCOfficeDocxFile2/DocWrapper/DocxSerializer.h"
#include "../../../../core/ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"
#include "../../../../core/ASCOfficePPTXFile/ASCOfficePPTXFile.h"
#include "../../../../core/OfficeUtils/src/OfficeUtils.h"
#include "../../../../core/Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../../../core/Common/DocxFormat/Source/XML/stringcommon.h"
#include "../../../../core/Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../../../core/Common/OfficeFileErrorDescription.h"
#include "../../../../core/Common/OfficeFileFormats.h"

namespace NExtractTools
{
#define SUCCEEDED_X2T(nRes) (0 == (nRes) || AVS_FILEUTILS_ERROR_CONVERT_CORRUPTED == (nRes))
    
    // docx -> bin
    static int docx2doct_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        // Save to file (from temp dir)
        BinDocxRW::CDocxSerializer m_oCDocxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCDocxSerializer.setFontDir(sFontPath1);
        
        //bool bRes = m_oCDocxSerializer.saveToFile (sResDoct, sSrcDocx, sTemp);
        CString sXMLOptions;
        bool res =  m_oCDocxSerializer.saveToFile (std_string2string(sTo), std_string2string(sTempUnpackedDOCX), sXMLOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        
        //        sleep(3*60);
        
        return res;
    }
    
    // docx -> doct
    static int docx2doct (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        // Extract docx to temp directory
        std::wstring sResultDoctDir = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sResultDoctFileEditor = sResultDoctDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sResultDoctDir);
        
        int nRes = docx2doct_bin(sFrom, sResultDoctFileEditor, sTemp, sFontPath);
        
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultDoctDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        
        return nRes;
        
    }
    
    // bin -> docx
    static int doct_bin2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        int nRes = 0;
        CString sResultDocxDir = std_string2string(sTemp) + FILE_SEPARATOR_STR + _T("docx_unpacked");
        
        FileSystem::Directory::CreateDirectory(sResultDocxDir);
        
        std::wstring sTargetBin;
//#if !defined(_WIN32) && !defined (_WIN64)
//        if(bFromChanges)
//            nRes = apply_changes(sFrom, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, sFontPath, sThemeDir, sTargetBin);
//        else
//#endif
        sTargetBin = sFrom;
        
        BinDocxRW::CDocxSerializer m_oCDocxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCDocxSerializer.setFontDir(sFontPath1);
        
        CString sXMLOptions = _T("");
        CString sThemePath;             // will be filled by 'CreateDocxFolders' method
        CString sMediaPath;             // will be filled by 'CreateDocxFolders' method
        CString sEmbedPath;             // will be filled by 'CreateDocxFolders' method
        
        m_oCDocxSerializer.CreateDocxFolders (sResultDocxDir, sThemePath, sMediaPath, sEmbedPath);
        
        if (SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCDocxSerializer.loadFromFile (std_string2string(sTargetBin), sResultDocxDir, sXMLOptions, sThemePath, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            if (SUCCEEDED_X2T(nRes))
            {
                // compress
                COfficeUtils oCOfficeUtils(NULL);
                nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (string2std_string(sResultDocxDir), sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            }
        }
        return nRes;
        
    }
    
    // doct -> docx
    static int doct2docx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        // Extract docx to temp directory
        std::wstring sTempUnpackedDOCT = sTemp + FILE_SEPARATOR_STR + _T("doct_unpacked");
        std::wstring sTempDoctFileEditor = sTempUnpackedDOCT + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedDOCT);
        
        // unzip doct to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        return doct_bin2docx(sTempDoctFileEditor, sTo, sTemp, sFontPath, bFromChanges, sThemeDir);
        
    }
    
    // xslx -> bin
    int xlsx2xlst_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
        CString sXMLOptions = _T("");
        CString sTo1 = std_string2string(sTo);
        return m_oCXlsxSerializer.saveToFile (sTo1, std_string2string(sTempUnpackedXLSX), sXMLOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // xslx -> xslt
    static int xlsx2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        // Extract xlsx to temp directory
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sResultXlstDir);
        
        int nRes = xlsx2xlst_bin(sFrom, sResultXlstFileEditor, sTemp, sFontPath);
        if (SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        
        return nRes;
    }
    
    // bin -> xslx
    static int xlst_bin2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        int nRes = 0;
        // Extract xlsx to temp directory
        std::wstring sResultXlsxDir = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        
        FileSystem::Directory::CreateDirectory(sResultXlsxDir);
        
        std::wstring sTargetBin;
//#if !defined(_WIN32) && !defined (_WIN64)
//        if(bFromChanges)
//            nRes = apply_changes(sFrom, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, sFontPath, sThemeDir, sTargetBin);
//        else
//#endif
        sTargetBin = sFrom;
        
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
        CString sXMLOptions = _T("");
        CString sMediaPath;             // will be filled by 'CreateXlsxFolders' method
        CString sEmbedPath;             // will be filled by 'CreateXlsxFolders' method
        
        m_oCXlsxSerializer.CreateXlsxFolders (sXMLOptions, std_string2string(sResultXlsxDir), sMediaPath, sEmbedPath);
        
        if(SUCCEEDED_X2T(nRes))
        {
            nRes = m_oCXlsxSerializer.loadFromFile (std_string2string(sTargetBin), std_string2string(sResultXlsxDir), sXMLOptions, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            if (SUCCEEDED_X2T(nRes))
            {
                // compress
                COfficeUtils oCOfficeUtils(NULL);
                nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultXlsxDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            }
        }
        return nRes;
    }
    
    // xslt -> xslx
    static int xlst2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        // Extract xlsx to temp directory
        std::wstring sTempUnpackedXLST = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sTempXlstFileEditor = sTempUnpackedXLST + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedXLST);
        
        // unzip xlst to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLST, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        return xlst_bin2xlsx(sTempXlstFileEditor, sTo, sTemp, sFontPath, bFromChanges, sThemeDir);
    }
    
    static int pptt_bin2pptx_dir (const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        int nRes = 0;
        
        std::wstring sTargetBin;
//        if(bFromChanges)
//            nRes = apply_changes(sFrom, sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, sFontPath, sThemeDir, sTargetBin);
//        else
            sTargetBin = sFrom;
        
        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);
        
        HRESULT hr = S_OK;
        
        if (pptx_file)
        {
#if defined(_WIN32) || defined (_WIN64)
            BSTR bstrFontPath		= SysAllocString(sFontPath.c_str());
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
            pptx_file->SetFontDir(sFontPath);
            
            nRes = (S_OK == pptx_file->ConvertPPTYToPPTX(sTargetBin, sTo, sThemeDir)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
#endif
            delete pptx_file;
        }
        
        return nRes;
    }
    
    // pptx -> bin
    static int pptx2pptt_bin (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        // unzip pptx to temp folder
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked")+ FILE_SEPARATOR_STR;  // leading slash is very important!
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedPPTX);
        
        // unzip pptx to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        // convert unzipped pptx to unzipped pptt
        CPPTXFile *pptx_file = new CPPTXFile(NULL, NULL, NULL, NULL);
        
        int nRes = 0;
        
        if (pptx_file)
        {
#if defined(_WIN32) || defined (_WIN64)
            BSTR bstrFontPath			= SysAllocString(sFontPath.c_str());
            BSTR bstrTempUnpackedPPTX	= SysAllocString(sTempUnpackedPPTX.c_str());
            BSTR bstrTo					= SysAllocString(sTo.c_str());
            
            pptx_file->SetFontDir (bstrFontPath);
            nRes = (S_OK == pptx_file->OpenFileToPPTY (bstrTempUnpackedPPTX, bstrTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            
            SysFreeString(bstrFontPath);
            SysFreeString(bstrTempUnpackedPPTX);
            SysFreeString(bstrTo);
#else
            pptx_file->SetFontDir (sFontPath);
            nRes = (S_OK == pptx_file->OpenFileToPPTY (sTempUnpackedPPTX, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
#endif
            delete pptx_file;
        }
        
        return nRes;
    }
    // pptx -> pptt
    static int pptx2pptt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        std::wstring sResultPpttDir = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sResultPpttDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sResultPpttDir);
        
        int nRes = pptx2pptt_bin(sFrom, sTempPpttFileEditor, sTemp, sFontPath);
        if (SUCCEEDED_X2T(nRes))
        {
            // zip pptt folder to output file
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPpttDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
    
    // bin -> pptx
    static int pptt_bin2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        int nRes = 0;
        std::wstring sResultPptxDir = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        
        FileSystem::Directory::CreateDirectory(sResultPptxDir);
        
        nRes = pptt_bin2pptx_dir(sFrom, sTo, sResultPptxDir, sFontPath, bFromChanges, sThemeDir);
        if (!SUCCEEDED_X2T(nRes))
            return nRes;
        
        // zip pptx folder to output file
        COfficeUtils oCOfficeUtils(NULL);
        nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory (sResultPptxDir,sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        
        return nRes;
    }
    // pptt -> pptx
    static int pptt2pptx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir)
    {
        // unzip pptt to temp folder
        std::wstring sTempUnpackedPPTT = sTemp + FILE_SEPARATOR_STR + _T("pptt_unpacked");
        std::wstring sTempPpttFileEditor = sTempUnpackedPPTT + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedPPTT);
        
        // unzip pptt to folder
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTT, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;
        
        return pptt_bin2pptx(sTempPpttFileEditor, sTo, sTemp, sFontPath, bFromChanges, sThemeDir);
    }
    // zip dir
    static int dir2zip (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.CompressFileOrDirectory(sFrom, sTo, -1)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // unzip dir
    static int zip2dir (const std::wstring &sFrom, const std::wstring &sTo)
    {
        COfficeUtils oCOfficeUtils(NULL);
        return (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTo, NULL, 0)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // csv -> xslt
    static int csv2xlst (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & XmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        CString sCSV = std_string2string(sFrom);
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sResultXlstDir);
        
        COfficeUtils oCOfficeUtils(NULL);
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
        CString sXMLOptions = std_string2string(XmlOptions);
        
        int nRes = m_oCXlsxSerializer.saveToFile (std_string2string(sResultXlstFileEditor), sCSV, sXMLOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        if (SUCCEEDED_X2T(nRes))
        {
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sResultXlstDir, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        
        return nRes;
    }
    
    // csv -> xslx
    static int csv2xlsx (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & XmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath)
    {
        CString sCSV = std_string2string(sFrom);
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + _T("xlsx_unpacked");
        std::wstring sResultXlstDir = sTemp + FILE_SEPARATOR_STR + _T("xlst_unpacked");
        std::wstring sResultXlstFileEditor = sResultXlstDir + FILE_SEPARATOR_STR + _T("Editor.bin");
        
        FileSystem::Directory::CreateDirectory(sTempUnpackedXLSX);
        FileSystem::Directory::CreateDirectory(sResultXlstDir);
        
        // Save to file (from temp dir)
        BinXlsxRW::CXlsxSerializer m_oCXlsxSerializer;
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
        COfficeUtils oCOfficeUtils(NULL);
        
        CString sXMLOptions = std_string2string(XmlOptions);
        CString sMediaPath;
        CString sEmbedPath;
        
        int nRes = m_oCXlsxSerializer.saveToFile (std_string2string(sResultXlstFileEditor), sCSV, sXMLOptions) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
        if (SUCCEEDED_X2T(nRes))
        {
            sXMLOptions = _T("");
            nRes = m_oCXlsxSerializer.loadFromFile(std_string2string(sResultXlstFileEditor), std_string2string(sTempUnpackedXLSX), sXMLOptions, sMediaPath, sEmbedPath) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            if (SUCCEEDED_X2T(nRes))
            {
                nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedXLSX, sTo, -1)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
            }
        }
        
        return nRes;
    }
    // xlst -> csv
    static int xlst2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath)
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
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
        CString sMediaPath;
        CString sEmbedPath;
        
        return m_oCXlsxSerializer.loadFromFile (std_string2string(sTempXlstFileEditor), sCSV, std_string2string(sXmlOptions), sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
    }
    // xslx -> csv
    static int xlsx2csv (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath)
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
        
        CString sFontPath1 = std_string2string(sFontPath);
        
        m_oCXlsxSerializer.setFontDir(sFontPath1);
        
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
}

static std::wstring nsstring_to_wstring(NSString* nsstring)
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [nsstring dataUsingEncoding:encode];
    return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
}

@implementation X2tConverter

- (int)sdk_docx2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::docx2doct_bin(from, to, temp, fontpath);
}
- (int)sdk_docx2doct:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::docx2doct(from, to, temp, fontpath);
}
- (int)sdk_doct_bin2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::doct_bin2docx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_doct2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::doct2docx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_xlsx2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::xlsx2xlst_bin(from, to, temp, fontpath);
    
}
- (int)sdk_xlsx2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::xlsx2xlst(from, to, temp, fontpath);
}
- (int)sdk_xlst_bin2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::xlst_bin2xlsx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_xlst2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::xlst2xlsx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_pptx2pptt_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::pptx2pptt_bin(from, to, temp, fontpath);
}
- (int)sdk_pptx2pptt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::pptx2pptt(from, to, temp, fontpath);
}
- (int)sdk_pptt_bin2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir{
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::pptt_bin2pptx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_pptt2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir{
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    return NExtractTools::pptt2pptx(from, to, temp, fontpath, bFromChanges, themeDir);
}
- (int)sdk_csv2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring options = nsstring_to_wstring(xmlOptions);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::csv2xlst(from, to, temp, options, fontpath);
}
- (int)sdk_csv2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring options = nsstring_to_wstring(xmlOptions);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::csv2xlsx(from, to, temp, options, fontpath);
}
- (int)sdk_xlst2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring options = nsstring_to_wstring(xmlOptions);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::xlst2csv(from, to, temp, options, fontpath);
}
- (int)sdk_xlsx2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring options = nsstring_to_wstring(xmlOptions);
    std::wstring fontpath = nsstring_to_wstring(nsFontPath);
    
    return NExtractTools::xlsx2csv(from, to, temp, options, fontpath);
}

- (int)sdk_dir2zip:(NSString*)nsFrom nsTo:(NSString*)nsTo {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    
    return NExtractTools::dir2zip(from, to);
}
- (int)sdk_zip2dir:(NSString*)nsFrom nsTo:(NSString*)nsTo {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    
    return NExtractTools::zip2dir(from, to);
}

@end
