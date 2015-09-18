//
//  AscEditorPDFPrinter.h
//  PdfWriter
//
//  Created by Oleg Korshul on 04.03.15.
//  Copyright (c) 2015 Ascensio System. All rights reserved.
//

#ifndef PdfWriter_AscEditorPDFPrinter_h
#define PdfWriter_AscEditorPDFPrinter_h

#ifdef __OBJC__
#import <CoreFoundation/CoreFoundation.h>
#else
#include <objc/objc.h>
#endif
#import <Foundation/Foundation.h>

#include <string>
#include <vector>

class CAscEditorPDFPrinter
{
public:
    std::wstring m_strImagesPath;
    std::wstring m_strPresentationThemesPath;
    
    std::wstring m_strFontsDirectory;
    
    bool m_bUseFontsDirectory;
    bool m_bUseSystemFonts;
    std::vector<std::wstring> m_arFontsDirectory;
    
public:
    CAscEditorPDFPrinter()
    {
        m_strImagesPath             = L"";
        m_strPresentationThemesPath = L"";
        
        m_strFontsDirectory         = L"";
        
        m_bUseSystemFonts           = false;
        m_bUseFontsDirectory        = false;
    }
    
public:
    bool Print(const std::string base64Data, std::wstring strDstFile);
};

#endif
