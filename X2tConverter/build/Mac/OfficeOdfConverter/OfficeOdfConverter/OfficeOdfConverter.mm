//
//  OfficeOdfConverter.m
//  OfficeOdfConverter
//
//  Created by alexey.musinov on 15/02/2019.
//  Copyright © 2019 Ascensio System SIA. All rights reserved.
//

#import "OfficeOdfConverter.h"

#include "../../../../src/cextracttools.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../ASCOfficeOdfFileW/source/Oox2OdfConverter/Oox2OdfConverter.h"

#include <string>
#include <fstream>
#include <iostream>

namespace NExtractTools
{
    // docx dir -> odt
    _UINT32 docx_dir2odt (const std::wstring &sDocxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
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
    
    _UINT32 xlsx_dir2ods (const std::wstring &sXlsxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
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
    _UINT32 pptx_dir2odp (const std::wstring &sPptxDir, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bTemplate)
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
}

static std::wstring nsstring_to_wstring(NSString* nsstring)
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [nsstring dataUsingEncoding:encode];
    return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
}


@implementation OfficeOdfConverter

- (int)docx2odt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::docx2odt(from, to, temp, oInputParams);
}

- (int)xlsx2ods:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::xlsx2ods(from, to, temp, oInputParams);
}

- (int)pptx2odp:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::pptx2odp(from, to, temp, oInputParams);
}

@end
