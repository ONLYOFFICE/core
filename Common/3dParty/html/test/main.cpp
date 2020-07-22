#include "../gumbo-parser/src/gumbo.h"
#include "../katana-parser/src/katana.h"

#include "CGetData.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>


int main(int argc, char *argv[])
{
//        const char* filename = "C:/Users/kiril/Desktop/OnlyOffice/core/EpubFile/test/part1.xhtml";

//        std::ifstream in(filename, std::ios::in | std::ios::binary);
//        if (!in)
//        {
//          std::cout << "File " << filename << " not found!\n";
//          exit(EXIT_FAILURE);
//        }

//        {
//        std::string contents;
//        in.seekg(0, std::ios::end);
//        contents.resize(in.tellg());
//        in.seekg(0, std::ios::beg);
//        in.read(&contents[0], contents.size());
//        in.close();

//        GumboOptions options = kGumboDefaultOptions;

//        GumboOutput* output = gumbo_parse_with_options(&options, contents.data(), contents.length());
//        std::string indent_chars = "  ";
//        std::cout << prettyprint(output->document, 0, indent_chars) << std::endl;
//        gumbo_destroy_output(&kGumboDefaultOptions, output);
//        return 0;
//    }
    FILE *fFile = fopen("C:/Users/kiril/Desktop/OnlyOffice/core/EpubFile/test/stylesheet.css", "r");
    KatanaOutput *output = katana_parse_in(fFile);

    CGetData data;
//    data.GetOutputData(output);
//    data.Print();

    katana_dump_output(output);
    katana_destroy_output(output);
    fclose(fFile);
}
