//
//  AscEditorPDFPrinter.h
//  PdfWriter
//
//  Created by Oleg Korshul on 04.03.15.
//  Copyright (c) 2015 Ascensio System. All rights reserved.
//

#ifndef PdfWriter_AscEditorPDFPrinter_h
#define PdfWriter_AscEditorPDFPrinter_h
#include <string>

class CAscEditorPDFPrinter
{
public:
    std::wstring m_strImagesPath;
    std::wstring m_strPresentationThemesPath;
    
    bool m_bUseSystemFonts;
    std::wstring m_strFontsDirectory;
    
public:
    CAscEditorPDFPrinter()
    {
        m_strImagesPath             = L"";
        m_strPresentationThemesPath = L"";
        
        m_bUseSystemFonts           = false;
        m_strFontsDirectory         = L"";
    }
    
public:
    bool Print(const std::string base64Data, std::wstring strDstFile);
};

#endif
