//
//  AscEditorPDFPrinter.cpp
//  PdfWriter
//
//  Created by Oleg Korshul on 04.03.15.
//  Copyright (c) 2015 Ascensio System. All rights reserved.
//

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
    pFontManager = oFonts.GenerateFontManager();
    
    oFonts.InitializeFromFolder(m_strFontsDirectory);
    pFontManager = oFonts.GenerateFontManager();
    
    CPdfRenderer oRender(&oFonts);
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
