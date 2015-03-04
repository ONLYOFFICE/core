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
    
public:
    CAscEditorPDFPrinter()
    {
        m_strImagesPath             = L"";
        m_strPresentationThemesPath = L"";
    }
    
public:
    bool Print(unsigned char* pSrcData, unsigned int nSrcLen, std::wstring strDstFile);
};

#endif
