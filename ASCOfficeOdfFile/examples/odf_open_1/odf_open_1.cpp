// xmlread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cpdoccore/xml/sax.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/serialize_attributes.h>
#include <cpdoccore/CPSharedPtr.h>
#include <utf8cpp/utf8.h>
#include <boost/timer.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

#include <cpdoccore/../../src/odf/odf_document.h>

int _tmain(int argc, _TCHAR* argv[])
{    

    //const std::wstring odfFolder = L"f:/Projects/Documents/cpdocument/test/odf/SimpleList/";
    //const std::wstring odfFolder = L"f:/Projects/Documents/cpdocument/test/odf/TextProperties/";
    
    //const std::wstring odfFolder = L"f:/Projects/Documents/cpdocument/test/odf/generaltest/";
    const std::wstring odfFolder = L"f:/Projects/Documents/cpdocument/test/odf/ivanova/";

    boost::timer t;
    cpdoccore::odf::odf_document d(odfFolder);
    std::cout << "open: " << t.elapsed() << "\n";

    //std::wcout << "\n\n";

    {
        std::wstringstream out;
        //std::wofstream fOut(L"f:/Projects/Documents/cpdocument/bin/01.xml");
        t.restart();
        d.xml_to_stream(out);
        std::cout << "serialize: " << t.elapsed() << "\n";
        t.restart();
        std::wstring s = out.str();
        std::string res;
        utf8::utf16to8(s.begin(), s.end(), std::back_inserter(res));

        FILE * f = fopen("f:/Projects/Documents/cpdocument/bin/01.xml", "wb");
        fwrite(res.c_str(), res.size(), 1, f);
        fclose(f);
        std::cout << "save: " << t.elapsed() << "\n";
    }

    //std::wstringstream strm;
    //d.xml_to_stream(strm);
    //std::wstring xmlString = strm.str();

    char ch;
    std::cin >> ch;

	return 0;
}

