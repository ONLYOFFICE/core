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

#import "OdfOfficeConverter.h"

#include "../../../../src/cextracttools.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../ASCOfficeOdfFile/src/ConvertOO2OOX.h"

#include <string>
#include <fstream>
#include <iostream>

namespace NExtractTools
{
    _UINT32 __processEncryptionError(_UINT32 hRes, const std::wstring &sFrom, InputParams& params)
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
    
    static std::wstring nsstring_to_wstring(NSString* nsstring)
    {
        NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
        NSData* data = [nsstring dataUsingEncoding:encode];
        return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
    }
}

namespace NExtractTools
{
    _UINT32 __odf2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        std::wstring sTempUnpackedOdf = sTemp + FILE_SEPARATOR_STR + _T("odf_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedOdf);
        
        COfficeUtils oCOfficeUtils(NULL);
        if (S_OK != oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
            return AVS_FILEUTILS_ERROR_CONVERT;;
        
        _UINT32 nRes = ConvertODF2OOXml(sTempUnpackedOdf, sTo, params.getFontPath(), sTemp, params.getPassword(), NULL);
        nRes = __processEncryptionError(nRes, sFrom, params);
        return nRes;
    }
    
    _UINT32 __odf2oox(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params)
    {
        std::wstring sTempUnpackedOox = sTemp + FILE_SEPARATOR_STR + _T("oox_unpacked");
        NSDirectory::CreateDirectory(sTempUnpackedOox);
        
        _UINT32 nRes = __odf2oox_dir(sFrom, sTempUnpackedOox, sTemp, params);
        if(SUCCEEDED_X2T(nRes))
        {
            COfficeUtils oCOfficeUtils(NULL);
            nRes = (S_OK == oCOfficeUtils.CompressFileOrDirectory(sTempUnpackedOox, sTo, true)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
        }
        return nRes;
    }
}

@implementation OdfOfficeConverter

- (int)odf2oox:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__odf2oox(from, to, temp, oInputParams);
}

- (int)odt2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__odf2oox(from, to, temp, oInputParams);
}

- (int)ods2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__odf2oox(from, to, temp, oInputParams);
}

- (int)odp2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = NExtractTools::nsstring_to_wstring(nsFrom);
    std::wstring to = NExtractTools::nsstring_to_wstring(nsTo);
    std::wstring temp = NExtractTools::nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(NExtractTools::nsstring_to_wstring(nsFontPath));
    oInputParams.m_bIsNoBase64 = new bool(self.isNoBase64);
    
    if (self.password) {
        oInputParams.m_sPassword = new std::wstring(NExtractTools::nsstring_to_wstring(self.password));
    }
    
    return NExtractTools::__odf2oox(from, to, temp, oInputParams);
}

@end
