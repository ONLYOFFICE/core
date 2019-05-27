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

#include "AscEditorPDFPrinter.h"

#include <stdio.h>
#include <math.h>

#include "../../common/File.h"
#include "../../common/Directory.h"
#include "../../fontengine/ApplicationFonts.h"
#include "../../../PdfWriter/PdfRenderer.h"

static std::wstring GetTempPath()
{
    NSString* sTempPath = NSTemporaryDirectory();
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [sTempPath dataUsingEncoding:encode];
    return std::wstring((wchar_t*)data.bytes, data.length / sizeof(wchar_t));
}

bool CAscEditorPDFPrinter::Print(std::string strBase64, std::wstring strDstFile)
{
    std::wstring sTempPath = GetTempPath();
    
    CApplicationFonts oFonts;
    CFontManager* pFontManager = NULL;
    
    std::vector<std::wstring> fonts;
    if (m_bUseSystemFonts)
    {
        fonts = oFonts.GetSetupFontFiles();
    }
    
    for (std::vector<std::wstring>::iterator iter = m_arFontsDirectory.begin(); iter != m_arFontsDirectory.end(); ++iter)
    {
        NSDirectory::GetFiles2(*iter, fonts);
    }
    
    oFonts.InitializeFromArrayFiles(fonts);
    pFontManager = (CFontManager*)oFonts.GenerateFontManager();
    
    oFonts.InitializeFromFolder(m_strFontsDirectory);
    pFontManager = (CFontManager*)oFonts.GenerateFontManager();
    
    CPdfRenderer oRender(&oFonts, m_bIsPDFA);
    oRender.SetTempFolder(GetTempPath());
    
    HRESULT hr = S_FALSE;
    int	len = NSBase64::Base64DecodeGetRequiredLength((int)strBase64.length());
    BYTE* dstArray	= new BYTE[len];
    if (NSBase64::Base64Decode(strBase64.c_str(), (int)strBase64.length(), dstArray, &len))
    {
        std::wstring sFile = m_strImagesPath + L"/________PDF________";
        
        NSFile::CFileBinary oFile;
        if (!oFile.CreateFileW(sFile))
        {            
            RELEASEARRAYOBJECTS(dstArray);
            RELEASEOBJECT(pFontManager);
            return false;
        }
        
        oFile.WriteFile(dstArray, len);
        oFile.CloseFile();
        
        hr = oRender.OnlineWordToPdfFromBinary(sFile, strDstFile);
        
        NSFile::CFileBinary::Remove(sFile);
    }
   
    RELEASEARRAYOBJECTS(dstArray);
    RELEASEOBJECT(pFontManager);
    
    return (hr == S_OK);
}
