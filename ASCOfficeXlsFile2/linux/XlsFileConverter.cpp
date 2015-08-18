#include "../source/XlsXlsxConverter/ConvertXls2Xlsx.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
#endif

#include <iostream>
#include "../version.h"

static std::wstring utf8_to_unicode(const char *src)
{
    if (src == NULL) return _T("");
    std::string temp = src;

    unsigned int nLength = temp.length();

    UTF32 *pStrUtf32 = new UTF32 [nLength+1];
    memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength+1));


    UTF8 *pStrUtf8 = (UTF8 *) src;

    // this values will be modificated
    const UTF8 *pStrUtf8_Conv = pStrUtf8;
    UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF32 (&pStrUtf8_Conv,
                                         &pStrUtf8[nLength]
                    , &pStrUtf32_Conv
                    , &pStrUtf32 [nLength]
                    , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
                delete [] pStrUtf32;
                return L"";
            }
            std::wstring wsEntryName ((wchar_t *) pStrUtf32);

            delete [] pStrUtf32;
            return wsEntryName;
}


int main(int argc, char *argv[])
{
   // check arguments
    if (argc < 3)
    {
        // print out help topic

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "\t\tXls/Xlsx file converter. Version: " << STRVER  << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
        std::cout << "USAGE: ASCXlsConverter \"path_to_file_1\" \"path_to_file_2\" " << std::endl;
        std::cout << "WHERE:" << std::endl;
        std::cout << "\t\"path_to_file_1\" is a path to file to be converted" << std::endl;
        std::cout << "\t\"path_to_file_2\" is a path to the corresponding output file" << std::endl << std::endl;
       return 1;
    }
    std::wstring sArg1, sArg2, sExePath;

    sExePath    = utf8_to_unicode(argv [0]);
    sArg1       = utf8_to_unicode(argv [1]);
    sArg2       = utf8_to_unicode(argv [2]);


    {// xls->xlsx


        HRESULT hRes = ConvertXls2Xlsx(sArg1, sArg2, NULL);

        if (hRes != S_OK)return 2;

    }
    return 0;
}
