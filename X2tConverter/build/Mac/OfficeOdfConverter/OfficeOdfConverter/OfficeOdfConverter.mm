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

#import "OfficeOdfConverter.h"

#include "../../../../src/cextracttools.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../ASCOfficeOdfFileW/source/Oox2OdfConverter/Oox2OdfConverter.h"
//#include "../../ASCOfficeOdfFile/src/ConvertOO2OOX.h"

#include <string>
#include <fstream>
#include <iostream>

namespace NExtractTools
{
    static std::wstring nsstring_to_wstring(NSString* nsstring)
    {
        NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
        NSData* data = [nsstring dataUsingEncoding:encode];
        return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
    }
}

namespace NExtractTools
{
    // docx dir -> odt
    _UINT32 __docx_dir2odt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedODT = sTemp + FILE_SEPARATOR_STR + L"odt_unpacked";
        NSDirectory::CreateDirectory(sTempUnpackedODT);
        
        Oox2Odf::Converter converter(sDocxDir, L"text", params.getFontPath(), bTemplate);
        
        _UINT32 nRes = 0;
        try
        {
            std::wstring password    = params.getSavePassword();
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
    
    _UINT32 __xlsx_dir2ods (const std::wstring &sXlsxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedODS = sTemp + FILE_SEPARATOR_STR + L"ods_unpacked";
        NSDirectory::CreateDirectory(sTempUnpackedODS);
        
        Oox2Odf::Converter converter(sXlsxDir, L"spreadsheet", params.getFontPath(), bTemplate);
        
        _UINT32 nRes = 0;
        try
        {
            std::wstring password    = params.getSavePassword();
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
    
    // pptx_dir -> odp
    _UINT32 __pptx_dir2odp (const std::wstring &sPptxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedODP = sTemp + FILE_SEPARATOR_STR + _T("odp_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedODP);
        
        Oox2Odf::Converter converter(sPptxDir, _T("presentation"), params.getFontPath(), bTemplate);
        
        _UINT32 nRes = 0;
        try
        {
            std::wstring password    = params.getSavePassword();
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
    
    // docx -> odt
    _UINT32 __docx2odt (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedDOCX = sTemp + FILE_SEPARATOR_STR + _T("docx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedDOCX);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedDOCX, NULL, 0))
        {
            return __docx_dir2odt(sTempUnpackedDOCX, sTo, sTemp, params, bTemplate);
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // xlsx -> ods
    _UINT32 __xlsx2ods (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedXLSX = sTemp + FILE_SEPARATOR_STR + L"xlsx_unpacked";
        
        NSDirectory::CreateDirectory(sTempUnpackedXLSX);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedXLSX, NULL, 0))
        {
            return __xlsx_dir2ods(sTempUnpackedXLSX, sTo, sTemp, params, bTemplate);
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
    
    // pptx -> odp
    _UINT32 __pptx2odp (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
    {
        std::wstring sTempUnpackedPPTX = sTemp + FILE_SEPARATOR_STR + _T("pptx_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedPPTX);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedPPTX, NULL, 0))
        {
            return __pptx_dir2odp(sTempUnpackedPPTX, sTo, sTemp, params, bTemplate);
        }
        return AVS_FILEUTILS_ERROR_CONVERT;
    }
}

@implementation OfficeOdfConverter

- (int)docx2odt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(bool)isTemplate {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__docx2odt(from, to, temp, oInputParams, isTemplate);
}

- (int)xlsx2ods:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(bool)isTemplate {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__xlsx2ods(from, to, temp, oInputParams, isTemplate);
}

- (int)pptx2odp:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(bool)isTemplate {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__pptx2odp(from, to, temp, oInputParams, isTemplate);
}

@end
