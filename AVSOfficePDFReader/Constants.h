#ifndef _CONSTANTS_H
#define _CONSTANTS_H

//-------------------------------------------------------------------------------------------------------------------------------

// Поддерживаемая версия PDF файла
#define SupportedPDFVersionStr "1.7"
#define SupportedPDFVersionNum  1.7

//-------------------------------------------------------------------------------------------------------------------------------
// Paper size
//-------------------------------------------------------------------------------------------------------------------------------

// Стандартные размеры страницы (в пикселях (72dpi))
#ifndef A4_PAPER
#define DefaultPaperWidth  595    // ISO A4 (210x297 mm)
#define DefaultPaperHeight 842
#else
#define DefaultPaperWidth  612    // American letter (8.5x11")
#define DefaultPaperHeight 792
#endif

//-------------------------------------------------------------------------------------------------------------------------------
// Config file path
//-------------------------------------------------------------------------------------------------------------------------------

#if defined(VMS) || (defined(WIN32) && !defined(__CYGWIN32__))
#define UserConfigFile "pdfrc"
#define SysConfigFile  "pdfrc"
#else
#define UserConfigFile ".pdfrc"
#define SysConfigFile  ".pdfrc"
#endif


#endif /* _CONSTANTS_H */
