//
//  AscEditorPDFPrinter.cpp
//  PdfWriter
//
//  Created by Oleg Korshul on 04.03.15.
//  Copyright (c) 2015 Ascensio System. All rights reserved.
//

#include <stdio.h>
#include "./AscEditorPDFPrinter.h"

#include "../../../ASCOfficePDFWriter/PdfWriterLib/PdfWriterLib.h"
#include "../../common/File.h"

bool CAscEditorPDFPrinter::Print(std::string strBase64, std::wstring strDstFile)
{
    CPdfWriterLib oWriter;
    
    if (m_bUseFontsDirectory)
    {
        CString sFonts(m_strFontsDirectory.c_str());
        oWriter.SetFontDir(sFonts);
    }
    else
    {
        oWriter.InitializeFonts(m_bUseSystemFonts, m_arFontsDirectory);
    }
    
    HRESULT hRes = S_OK;
    try
    {
        hRes = oWriter.CreatePDF();
        if (S_OK != (hRes))
            throw "CreatePDF failed!";
        
        hRes = oWriter.SetPDFCompressionMode(15);
        if (S_OK != (hRes))
            throw "SetPDFCompressionMode(15) failed!";
        
        int nCountPages = 0;
        
        std::wstring sHypers = L"";
        
        int	len			= NSBase64::Base64DecodeGetRequiredLength(strBase64.length());
        BYTE* dstArray	= new BYTE[len];
        
        if (NSBase64::Base64Decode(strBase64.c_str(), strBase64.length(), dstArray, &len))
        {
            oWriter.OnlineWordToPdfInternal(dstArray, len, m_strImagesPath, sHypers, nCountPages, L"", 1);
            oWriter.SaveToFile(strDstFile);
        }
        
        RELEASEARRAYOBJECTS(dstArray);
    }
    catch (char *pcError)
    {
        //ATLTRACE2 (pcError);
    }
    
    return (hRes == S_OK) ? true : false;
}

// file downloader realize
#ifdef BOOL
#undef BOOL
#endif
#include "../../../Common/FileDownloader.h"
#import <Foundation/Foundation.h>

HRESULT CFileDownloader::DownloadFileAll(std::wstring sFileURL, std::wstring strFileOutput)
{
    NSString* pStringUrl = [ [ NSString alloc ]
                         initWithBytes : (char*)sFileURL.data()
                         length : sFileURL.length() * sizeof(wchar_t)
                         encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
    
    NSString* pStringOutput = [ [ NSString alloc ]
                            initWithBytes : (char*)strFileOutput.data()
                            length : strFileOutput.length() * sizeof(wchar_t)
                            encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
    
    NSURL  *url = [NSURL URLWithString:pStringUrl];
    NSData *urlData = [NSData dataWithContentsOfURL:url];
    if ( urlData )
    {
        [urlData writeToFile:pStringOutput atomically:YES];
        return S_OK;
    }
    return S_FALSE;
}
