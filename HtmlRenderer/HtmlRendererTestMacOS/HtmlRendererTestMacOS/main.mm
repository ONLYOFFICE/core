//
//  main.m
//  HtmlRendererTestMacOS
//
//  Created by Oleg Korshul on 23.11.15.
//  Copyright © 2015 Ascensio System. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "../../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../../PdfReader/PdfReader.h"
#include "../../../DjVuFile/DjVu.h"
#include "../../../XpsFile/XpsFile.h"
#include "../../../PdfWriter/PdfRenderer.h"
#include "../../../HtmlRenderer/include/HTMLRenderer3.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        CApplicationFonts oFonts;
        oFonts.Initialize();

        //std::wstring sFile = L"/Users/Oleg/Desktop/activex/TEST_FILES/Android intro(2p).pdf";
        //std::wstring sFile = L"/Users/Oleg/Desktop/activex/TEST_FILES/SQL. Справочник.djvu";
        std::wstring sFile = L"/Users/Oleg/Desktop/activex/TEST_FILES/bankomats.xps";
        std::wstring sDst = L"/Users/Oleg/Desktop/activex/TEST_FILES/Document";
        
#if 0
        PdfReader::CPdfReader oReader(&oFonts);
        oReader.SetTempFolder(sDst.c_str());
#endif
        
#if 0
        CDjVuFile oReader;
#endif
    
#if 1
        CXpsFile oReader(&oFonts);
        oReader.SetTempFolder(sDst.c_str());
#endif
        
        bool bResult = oReader.LoadFromFile(sFile.c_str());
        
#if 0
        NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
        oHtmlRenderer.CreateOfficeFile(sDst);
#else
        CPdfRenderer oHtmlRenderer(&oFonts);
        oHtmlRenderer.SetTempFolder(sDst);
#endif
        
        int nPagesCount = oReader.GetPagesCount();
        for (int i = 0; i < nPagesCount; ++i)
        {
            oHtmlRenderer.NewPage();
            oHtmlRenderer.BeginCommand(c_nPageType);
            
            double dPageDpiX, dPageDpiY;
            double dWidth, dHeight;
            oReader.GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
            
            dWidth  *= 25.4 / dPageDpiX;
            dHeight *= 25.4 / dPageDpiY;
            
            oHtmlRenderer.put_Width(dWidth);
            oHtmlRenderer.put_Height(dHeight);
            
            oReader.DrawPageOnRenderer(&oHtmlRenderer, i, NULL);
            
            oHtmlRenderer.EndCommand(c_nPageType);
        }
        
#if 0
        oHtmlRenderer.CloseFile();
#else
        oHtmlRenderer.SaveToFile(L"/Users/Oleg/Desktop/activex/TEST_FILES/1.pdf");
#endif
    
    }
    return 0;
}
