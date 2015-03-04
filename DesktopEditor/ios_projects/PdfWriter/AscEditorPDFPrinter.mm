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

bool CAscEditorPDFPrinter::Print(unsigned char *pSrcData, unsigned int nSrcLen, std::wstring strDstFile)
{
    return false;
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
