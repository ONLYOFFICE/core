// xmlread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cpdoccore/xml/sax.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/serialize_attributes.h>
#include <cpdoccore/CPSharedPtr.h>
#include <iostream>
#include <sstream>
#include <fstream>

std::string wchar_to_ansi(const std::wstring & _String)
{
    ::setlocale(LC_ALL, "");
    size_t size = wcstombs(NULL, _String.c_str(), 0);
    std::string res;
    res.resize(size);
    size = wcstombs(&res[0], _String.c_str(), res.length());
    return res;
}

using namespace cpdoccore;

void skip_attributes(xml::sax_ptr & reader)
{
    reader->moveToAttrFirst();
    while (reader->moveToAttrNext());
}

void print_attributes(xml::sax_ptr & reader)
{
    if (reader->moveToAttrFirst())
    {
        for(;;)
        {
            if (!reader->attrDefault())
            {
                const std::string prefix = wchar_to_ansi(reader->namespacePrefix());
                const std::string name = wchar_to_ansi(reader->nodeLocalName());
                const std::string value = wchar_to_ansi(reader->value());

                std::cout << "attr: " << prefix << (prefix.size() ? ":" : "") << name << "=" << value << "\n";
            }

            if (!reader->moveToAttrNext())
                break;
        }
    }
}

void parse_attributes(xml::sax_ptr & reader)
{
    xml::attributes_wc_ptr attrb = xml::read_attributes( reader.get() );

    ::std::wostringstream strm;

    xml::serialize_attributes(strm, attrb.get() );
    std::cout << "attr: [" << wchar_to_ansi(strm.str()) << "]\n"; 
}

int TestRead(xml::sax_ptr reader)
{
    try 
    {
        xml::NodeType nodeType;
        while ( (nodeType = reader->next()) != xml::typeEOF )
        {
            switch(nodeType)
            {
            case xml::typeXmlDeclaration:
                parse_attributes(reader);
                break;

            case xml::typeElement:
                {
                    const std::string prefix = wchar_to_ansi(reader->namespacePrefix());
                    std::cout << "element: [" <<  prefix << (prefix.size() ? ":" : "") << wchar_to_ansi(reader->nodeLocalName()) << "]\n";
                }
                parse_attributes(reader);
                break;

            case xml::typeEndElement:
                {
                    const std::string prefix = wchar_to_ansi(reader->namespacePrefix());
                    std::cout << "end element: [" << prefix << (prefix.size() ? ":" : "") << ":" << wchar_to_ansi(reader->nodeLocalName()) << "]\n";
                }
                break;

            case xml::typeText:
            case xml::typeWhitespace:
                std::cout << "text: ["  << wchar_to_ansi(reader->value()) << "]\n";
                break;

            case xml::typeCDATA:
                std::cout << "CDATA: ["  << wchar_to_ansi(reader->value()) << "]\n";
                break;

            case xml::typeProcessingInstruction:
                std::cout << "instruction: ["  << wchar_to_ansi(reader->nodeLocalName()) << "] - [" << wchar_to_ansi(reader->value()) << "]\n";
                break;

            case xml::typeComment:
                std::cout << "comment: ["  << wchar_to_ansi(reader->value()) << "]\n";

            case xml::typeDocumentType:
                break;
            }
        }

    }
    catch( xml::sax::error & er)
    {
        std::cerr << "error " << er.code() << "\n"; 
    }
    catch(...)
    {
        std::cerr << "error " << "unexpected" << "\n"; 
    }
    return 0;

}

int TestReadFromFile(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        ::std::cerr << "Usage: xmlread.exe [InputXml]";
        return -1;
    }
    
    try 
    {
        xml::sax_ptr reader = xml::create_sax(argv[1]);
        return TestRead(reader);
    
    }
    catch( xml::sax::error & er)
    {
        std::cerr << "error " << er.code() << "\n"; 
    }
    catch(...)
    {
        std::cerr << "error " << "unexpected" << "\n"; 
    }
    return 0;
}

int  TestReadFromStream(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        ::std::cerr << "Usage: xmlread.exe [InputXml]";
        return -1;
    }

    std::ifstream stream(argv[1]);

    try 
    {
        xml::sax_ptr reader = xml::create_sax(stream);
        return TestRead(reader);

    }
    catch( xml::sax::error & er)
    {
        std::cerr << "error " << er.code() << "\n"; 
    }
    catch(...)
    {
        std::cerr << "error " << "unexpected" << "\n"; 
    }
    return 0;

    
    return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{    
    TestReadFromStream(argc, argv);
    char ch;
    std::cin >> ch;

	return 0;
}

