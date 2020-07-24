#include "../gumbo-parser/src/gumbo.h"
#include "../katana-parser/src/katana.h"

#include "../../../../DesktopEditor/common/File.h"

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
    std::wstring sFilePath = NSFile::GetProcessDirectory() + L"../../../../cssFiles/test.css";

    FILE *fFile = fopen(std::string(sFilePath.begin(), sFilePath.end()).c_str(), "r");
    KatanaOutput *output = katana_parse_in(fFile);

    CGetData data;
    data.GetOutputData(output);

    std::vector<std::string> arSel;
    arSel.push_back("h1");
    data.AddStyle(arSel, "color: blue;text-decoration:overline");

//    data.Print();



    std::vector<std::string> arSelectors;
    arSelectors.push_back("h1");
    arSelectors.push_back(".Author");
    arSelectors.push_back("#first");

    std::map<std::wstring, std::wstring> mDecl;

    mDecl = data.GetStyleW(arSelectors);

    for (auto iter = mDecl.begin(); iter != mDecl.end(); iter++)
    {
        std::wcout << iter->first << " : " << iter->second << std::endl;
    }

//    data.GetWeightSelector(".main[data-columns]::before");
//    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arDecls = data.GetDeclarations(L"div._idGenObjectLayout-1");

//    for (size_t i = 0; i < arDecls.size(); i++)
//    {
//        std::wcout << arDecls[i].first << std::endl;
//        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = arDecls[i].second;
//        std::wcout << L"{\n";
//        for (size_t j = 0; j < arDeclarations.size(); j++)
//            std::wcout << "   " << arDeclarations[j].first << " : " << arDeclarations[j].second << L";" <<std::endl;
//        std::wcout << L"}\n";
//    }

//    katana_dump_output(output);
    katana_destroy_output(output);
    fclose(fFile);
}
