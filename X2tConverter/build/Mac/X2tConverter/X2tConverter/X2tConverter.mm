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

#import "X2tConverter.h"

#include "version.h"

#include "cextracttools.h"
#include "ASCConverters.h"

#include <iostream>
#include <fstream>

#include "../../../../../UnicodeConverter/UnicodeConverter_Encodings.h"

static std::wstring nsstring_to_wstring(NSString* nsstring)
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [nsstring dataUsingEncoding:encode];
    return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
}

@implementation Encoding

@end

@implementation X2tConverter

- (int)sdk_docx2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::docx2doct_bin(from, to, temp, oInputParams);
}
- (int)sdk_docx2doct:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::docx2doct(from, to, temp, oInputParams);
}
- (int)sdk_doct_bin2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (nil != self.password && self.password.length > 0) {
        oInputParams.m_sSavePassword = new std::wstring(nsstring_to_wstring(self.password));
        
        std::wstring sResultDecryptFile = temp + FILE_SEPARATOR_STR + L"uncrypt_file.docx";
        
        if ((int)AVS_FILEUTILS_ERROR_CONVERT != NExtractTools::doct_bin2docx(from, sResultDecryptFile, temp, bFromChanges, themeDir, oInputParams)) {
            return oox2mscrypt(sResultDecryptFile, to, temp, oInputParams);
        }
        
        return AVS_FILEUTILS_ERROR_CONVERT;
        
    } else {
        return NExtractTools::doct_bin2docx(from, to, temp, bFromChanges, themeDir, oInputParams);
    }
}
- (int)sdk_doct2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::doct2docx(from, to, temp, bFromChanges, themeDir, oInputParams);
}

- (int)sdk_xlsx2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xlsx2xlst_bin(from, to, temp, oInputParams);
    
}
- (int)sdk_xlsx2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xlsx2xlst(from, to, temp, oInputParams);
}
- (int)sdk_xlst_bin2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (nil != self.password && self.password.length > 0) {
        oInputParams.m_sSavePassword = new std::wstring(nsstring_to_wstring(self.password));
        
        std::wstring sResultDecryptFile = temp + FILE_SEPARATOR_STR + L"uncrypt_file.xlsx";
        
        if ((int)AVS_FILEUTILS_ERROR_CONVERT != NExtractTools::xlst_bin2xlsx(from, sResultDecryptFile, temp, bFromChanges, themeDir, oInputParams)) {
            return oox2mscrypt(sResultDecryptFile, to, temp, oInputParams);
        }
        
        return AVS_FILEUTILS_ERROR_CONVERT;
        
    } else {
        return NExtractTools::xlst_bin2xlsx(from, to, temp, bFromChanges, themeDir, oInputParams);
    }
}
- (int)sdk_xlst2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    return NExtractTools::xlst2xlsx(from, to, temp, bFromChanges, themeDir, oInputParams);
}

- (int)sdk_pptx2pptt_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::pptx2pptt_bin(from, to, temp, oInputParams);
}
- (int)sdk_pptx2pptt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::pptx2pptt(from, to, temp, oInputParams);
}
- (int)sdk_pptt_bin2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    if (nil != self.password && self.password.length > 0) {
        oInputParams.m_sSavePassword = new std::wstring(nsstring_to_wstring(self.password));
        
        std::wstring sResultDecryptFile = temp + FILE_SEPARATOR_STR + L"uncrypt_file.pptx";
        
        if ((int)AVS_FILEUTILS_ERROR_CONVERT != NExtractTools::pptt_bin2pptx(from, sResultDecryptFile, temp, bFromChanges, themeDir, oInputParams)) {
            return oox2mscrypt(sResultDecryptFile, to, temp, oInputParams);
        }
        
        return AVS_FILEUTILS_ERROR_CONVERT;
        
    } else {
        return NExtractTools::pptt_bin2pptx(from, to, temp, bFromChanges, themeDir, oInputParams);
    }
}
- (int)sdk_pptt2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir{
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::pptt2pptx(from, to, temp, bFromChanges, themeDir, oInputParams);
}

- (int)sdk_csv2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::csv2xlst(from, to, temp, oInputParams);
}
- (int)sdk_csv2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::csv2xlsx(from, to, temp, oInputParams);
}
- (int)sdk_csv2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.delimiter != nil) {
        oInputParams.m_nCsvDelimiter = new int(self.delimiter.intValue);
    }
    if (self.encoding != nil) {
        oInputParams.m_nCsvTxtEncoding = new int(self.encoding.intValue);
    }
    
    oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
    oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET);
    
    return NExtractTools::csv2xlst_bin(from, to, oInputParams);
}
- (int)sdk_xlst2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    return NExtractTools::xlst2csv(from, to, temp, oInputParams);
}
- (int)sdk_xlsx2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    return NExtractTools::xlsx2csv(from, to, temp, oInputParams);
}

- (int)sdk_xls2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlsx(from, to, temp, oInputParams);
}
- (int)sdk_xls2xlsx_dir:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlsx_dir(from, to, temp, oInputParams);
}
- (int)sdk_xls2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlst(from, to, temp, oInputParams);
}
- (int)sdk_xls2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlst_bin(from, to, temp, oInputParams);
}
- (int)sdk_xls2xlsm:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlsm(from, to, temp, oInputParams);
}
- (int)sdk_xls2xlsm_dir:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xls2xlsm_dir(from, to, temp, oInputParams);
}

- (int)doct_bin2dotx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    
    int nRes = 0;
    
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    NSString* nsTemporaryFile = [NSString stringWithFormat:@"%@%@.docx", NSTemporaryDirectory(), [[NSUUID UUID] UUIDString]];
    std::wstring temporaryFile = nsstring_to_wstring(nsTemporaryFile);
    
    std::wstring sDocxDir = temp + _T("docx_unpacked");
    NSDirectory::DeleteDirectory(sDocxDir);
    NSDirectory::CreateDirectory(sDocxDir);
    
    nRes = doct_bin2docx_dir(from, temporaryFile, sDocxDir, false, temp, oInputParams);
    if(SUCCEEDED_X2T(nRes))
    {
        nRes = fromDocxDir(sDocxDir, to, AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX, temp, themeDir, bFromChanges, false, oInputParams);
    }
    
    return nRes;
}

- (int)xlst_bin2xltx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    
    int nRes = 0;
    
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    NSString* nsTemporaryFile = [NSString stringWithFormat:@"%@%@.xlsx", NSTemporaryDirectory(), [[NSUUID UUID] UUIDString]];
    std::wstring temporaryFile = nsstring_to_wstring(nsTemporaryFile);
    
    std::wstring sXlsxDir = temp + _T("xlsx_unpacked");
    NSDirectory::DeleteDirectory(sXlsxDir);
    NSDirectory::CreateDirectory(sXlsxDir);
    
    nRes = xlst_bin2xlsx_dir(from, temporaryFile, sXlsxDir, false, temp, oInputParams);
    if(SUCCEEDED_X2T(nRes))
    {
        nRes = fromXlsxDir(sXlsxDir, to, AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX, temp, themeDir, bFromChanges, false, oInputParams, std::wstring(L""));
    }
    
    return nRes;
}

- (int)pptt_bin2potx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    
    int nRes = 0;
    
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    NSString* nsTemporaryFile = [NSString stringWithFormat:@"%@%@.pptx", NSTemporaryDirectory(), [[NSUUID UUID] UUIDString]];
    std::wstring temporaryFile = nsstring_to_wstring(nsTemporaryFile);
    
    std::wstring sPptxDir = temp + _T("pptx_unpacked");
    NSDirectory::DeleteDirectory(sPptxDir);
    NSDirectory::CreateDirectory(sPptxDir);
    
    nRes = pptt_bin2pptx_dir(from, temporaryFile, sPptxDir, false, temp, oInputParams);
    if(SUCCEEDED_X2T(nRes))
    {
        nRes = fromPptxDir(sPptxDir, to, AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX, temp, themeDir, bFromChanges, false, oInputParams);
    }
    
    return nRes;
}

- (int)txt2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.encoding != nil) {
        oInputParams.m_nCsvTxtEncoding = new int(self.encoding.intValue);
    }
    
    oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT);
    oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_CANVAS_WORD);
    
    return NExtractTools::txt2doct_bin(from, to, temp, oInputParams);
}

- (int)txt2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.encoding != nil) {
        oInputParams.m_nCsvTxtEncoding = new int(self.encoding.intValue);
    }
    
    oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT);
    oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
    
    return NExtractTools::txt2docx(from, to, temp, oInputParams);
}

- (int)txt2docx_dir:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.encoding != nil) {
        oInputParams.m_nCsvTxtEncoding = new int(self.encoding.intValue);
    }
    
    oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT);
    oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
    
    return NExtractTools::txt2doct_bin(from, to, temp, oInputParams);
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

+ (NSArray <NSString *> *)delimiters {
    NSMutableArray *values = [[NSMutableArray alloc]init];
    
    [values addObject:[NSString stringWithFormat:@"\t"]];
    [values addObject:[NSString stringWithFormat:@";"]];
    [values addObject:[NSString stringWithFormat:@":"]];
    [values addObject:[NSString stringWithFormat:@","]];
    [values addObject:[NSString stringWithFormat:@" "]];
    
    return values;
}

+ (NSArray <Encoding *> *)encodingings {
    NSMutableArray *values = [[NSMutableArray alloc]init];
    for (int i = 0; i < UNICODE_CONVERTER_ENCODINGS_COUNT; ++i) {
        Encoding *encoding = [[Encoding alloc]init];
        encoding.index = NSUnicodeConverter::Encodings[i].Index;
        encoding.codePage = NSUnicodeConverter::Encodings[i].WindowsCodePage;
        encoding.name = [NSString stringWithFormat:@"%s", NSUnicodeConverter::Encodings[i].Name];
        encoding.displayName = [NSString stringWithFormat:@"%s", NSUnicodeConverter::Encodings[i].DisplayName];
        [values addObject:encoding];
    }
    
    return values;
}

+ (NSString *)version {
    return [[NSString alloc] initWithUTF8String:X2T_VERSION];
}

- (void)removeFileAt:(NSString *)path {
    NSError *error;
    if ([[NSFileManager defaultManager] isDeletableFileAtPath:path]) {
        BOOL success = [[NSFileManager defaultManager] removeItemAtPath:path error:&error];
        if (!success) {
            NSLog(@"Error removing file at path: %@", error.localizedDescription);
        }
    }
}

@end
