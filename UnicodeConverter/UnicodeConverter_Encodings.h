#ifndef _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H
#define _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H

namespace NSUnicodeConverter
{
    struct EncodindId
    {
        int Index;
        int WindowsCodePage;
        const char* Name;
        const char* DisplayName;
    };

    #define UNICODE_CONVERTER_ENCODINGS_COUNT 52
    static const EncodindId Encodings[UNICODE_CONVERTER_ENCODINGS_COUNT] =
    {
        { 0,    28596, "ISO-8859-6",       "Arabic (ISO 8859-6)" },
        { 1,    720,   "DOS-720",          "Arabic (OEM 720)" },
        { 2,    1256,  "windows-1256",     "Arabic (Windows)" },

        { 3,    28594, "ISO-8859-4",       "Baltic (ISO 8859-4)" },
        { 4,    28603, "ISO-8859-13",      "Baltic (ISO 8859-13)" },
        { 5,    775,   "IBM775",           "Baltic (OEM 775)" },
        { 6,    1257,  "windows-1257",     "Baltic (Windows)" },

        { 7,    28604, "ISO-8859-14",      "Celtic (ISO 8859-14)" },

        { 8,    28595, "ISO-8859-5",       "Cyrillic (ISO 8859-5)" },
        { 9,    20866, "KOI8-R",           "Cyrillic (KOI8-R)" },
        { 10,   21866, "KOI8-U",           "Cyrillic (KOI8-U)" },
        { 11,   10007, "x-mac-cyrillic",   "Cyrillic (Mac)" },
        { 12,   855,   "IBM855",           "Cyrillic (OEM 855)" },
        { 13,   866,   "cp866",            "Cyrillic (OEM 866)" },
        { 14,   1251,  "windows-1251",     "Cyrillic (Windows)" },

        { 15,   852,   "IBM852",           "Central European (OEM 852)" },
        { 16,   1250,  "windows-1250",     "Central European (Windows)" },

        { 17,   950,   "Big5",             "Chinese (Big5 Traditional)" },
        { 18,   936,   "GB2312",           "Central (GB2312 Simplified)" },

        { 19,   28592, "ISO-8859-2",       "Eastern European (ISO 8859-2)" },

        { 20,   28597, "ISO-8859-7",       "Greek (ISO 8859-7)" },
        { 21,   737,   "IBM737",           "Greek (OEM 737)" },
        { 22,   869,   "IBM869",           "Greek (OEM 869)" },
        { 23,   1253,  "windows-1253",     "Greek (Windows)" },

        { 24,   28598, "ISO-8859-8",       "Hebrew (ISO 8859-8)" },
        { 25,   862,   "DOS-862",          "Hebrew (OEM 862)" },
        { 26,   1255,  "windows-1255",     "Hebrew (Windows)" },

        { 27,   932,   "Shift_JIS",        "Japanese (Shift-JIS)" },

        { 28,   949,   "KS_C_5601-1987",   "Korean (Windows)" },
        { 29,   51949, "EUC-KR",           "Korean (EUC)" },

        { 30,   861,   "IBM861",           "North European (Icelandic OEM 861)" },
        { 31,   865,   "IBM865",           "North European (Nordic OEM 865)" },

        { 32,   874,   "windows-874",      "Thai (TIS-620)" },

        { 33,   28593, "ISO-8859-3",       "Turkish (ISO 8859-3)" },
        { 34,   28599, "ISO-8859-9",       "Turkish (ISO 8859-9)" },
        { 35,   857,   "IBM857",           "Turkish (OEM 857)" },
        { 36,   1254,  "windows-1254",     "Turkish (Windows)" },

        { 37,   28591, "ISO-8859-1",       "Western European (ISO-8859-1)" },
        { 38,   28605, "ISO-8859-15",      "Western European (ISO-8859-15)" },
        { 39,   850,   "IBM850",           "Western European (OEM 850)" },
        { 40,   858,   "IBM858",           "Western European (OEM 858)" },
        { 41,   860,   "IBM860",           "Western European (OEM 860 : Portuguese)" },
        { 42,   863,   "IBM863",           "Western European (OEM 863 : French)" },
        { 43,   437,   "IBM437",           "Western European (OEM-US)" },
        { 44,   1252,  "windows-1252",     "Western European (Windows)" },

        { 45,   1258,  "windows-1258",     "Vietnamese (Windows)" },

        { 46,   65001, "UTF-8",            "Unicode (UTF-8)" },
        { 47,   65000, "UTF-7",            "Unicode (UTF-7)" },

        { 48,   1200, "UTF-16",            "Unicode (UTF-16)" },
        { 49,   1201, "UTF-16BE",          "Unicode (UTF-16 Big Endian)" },

        { 50,   12000, "UTF-32",           "Unicode (UTF-32)" },
        { 51,   12001, "UTF-32BE",         "Unicode (UTF-32 Big Endian)" }
    };
}

#endif // _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H
