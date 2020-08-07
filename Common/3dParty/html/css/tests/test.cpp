#include "../../../../../DesktopEditor/common/File.h"

#include "../src/CCssCalculator.h"
#include "../src/CCompiledStyle.h"

#include "../../katana-parser/src/katana.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

#include <Windows.h>

#include <codecvt>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"Russian");
    SetConsoleOutputCP( CP_UTF8 );

    std::wcout << L"-----BEGIN-----" << std::endl;

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    std::vector<std::pair<std::wstring, std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>>>> arTestDatas;

    std::vector<std::string> arSelectors1 = {"#forkongithub"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData1 = {arSelectors1,{{L"display",  L"block"},
                                                                                                      {L"height",   L"5,291667cm "},
                                                                                                      {L"overflow", L"hidden"},
                                                                                                      {L"position", L"absolute"},
                                                                                                      {L"right",    L"0"},
                                                                                                      {L"top",      L"0"},
                                                                                                      {L"width",    L"5,291667cm "}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/demo-page.css", pData1));

    std::vector<std::string> arSelectors2 = {".animate__animated.animate__faster"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData2 = {arSelectors2,{{L"-webkit-animation-duration",           L"calc(1s / 2)"},
                                                                                                      {L"animation-duration",                   L"calc(1s / 2)"},
                                                                                                      {L"-webkit-animation-duration",           L"calc(var(--animate-duration) / 2)"},
                                                                                                      {L"animation-duration",                   L"calc(var(--animate-duration) / 2)"},
                                                                                                      {L"background",                           L"black"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/test.css", pData2));

    std::vector<std::string> arSelectors3 = {".hvr-pulse-grow:focus"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData3 = {arSelectors3,{{L"-webkit-animation-name",               L"hvr-pulse-grow"},
                                                                                                      {L"animation-name",                       L"hvr-pulse-grow"},
                                                                                                      {L"-webkit-animation-duration",           L"0.3s"},
                                                                                                      {L"animation-duration",                   L"0.3s"},
                                                                                                      {L"-webkit-animation-timing-function",    L"linear"},
                                                                                                      {L"animation-timing-function",            L"linear"},
                                                                                                      {L"-webkit-animation-iteration-count",    L"infinite"},
                                                                                                      {L"animation-iteration-count",            L"infinite"},
                                                                                                      {L"-webkit-animation-direction",          L"alternate"},
                                                                                                      {L"animation-direction",                  L"alternate"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/hover.css", pData3));

    std::vector<std::string> arSelectors4 = {".collapser::before"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData4 = {arSelectors4,{{L"content",                  converter.from_bytes("\"»\"")},
                                                                                                      {L"font-size",                L"1.2em"},
                                                                                                      {L"margin-right",             L".2em"},
                                                                                                      {L"-moz-transition-property", L"-moz-transform"},
                                                                                                      {L"-moz-transition-duration", L".2s"},
                                                                                                      {L"-moz-transform-origin",    L"center 60%"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/functions.css", pData4));

    std::vector<std::string> arSelectors5 = {"#header"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData5 = {arSelectors5,{{L"margin",                   L"0,264583cm  2em 1cm 2%"},
                                                                                                      {L"font-family",              L"Verdana , Helvetica , \"Gill Sans\" , sans-serif"},
                                                                                                      {L"font-size",                L"0,264583cm "},
                                                                                                      {L"color",                    L"green"},
                                                                                                      {L"background-color",         L"rgba(0 , 128 , 0 , 0.7)"},
                                                                                                      {L"frequency",                L"30Hz"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/values.css", pData5));

    std::vector<std::string> arSelectors6 = {".calibre7", ".paragraph"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData6= {arSelectors6,{{L"display",           L"block"},
                                                                                                     {L"list-style-type",   L"square"},
                                                                                                     {L"margin-bottom",     L"0"},
                                                                                                     {L"margin-right",      L"0"},
                                                                                                     {L"margin-top",        L"0"},
                                                                                                     {L"text-indent",       L"2em"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/skazki.css", pData6));

    std::vector<std::string> arSelectors7 = {".calibre8", ".paragraph"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData7= {arSelectors7,{{L"background-color",      L"#E7E7E7"},
                                                                                                     {L"display",               L"block"},
                                                                                                     {L"font-size",             L"1.66667em"},
                                                                                                     {L"font-style",            L"normal"},
                                                                                                     {L"font-weight",           L"bold"},
                                                                                                     {L"line-height",           L"1.2"},
                                                                                                     {L"text-align",            L"left"},
                                                                                                     {L"border",                L"Black solid 0,026458cm "},
                                                                                                     {L"margin",                L"0.67em 0"},
                                                                                                     {L"text-indent",           L"2em"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/skazki.css", pData7));

    std::vector<std::string> arSelectors8 = {".calibre2", ".calibre3", ".calibre4"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData8= {arSelectors8,{{L"background-color",      L"#F1F1F1"},
                                                                                                     {L"display",               L"block"},
                                                                                                     {L"font-size",             L"1.29167em"},
                                                                                                     {L"font-style",            L"normal"},
                                                                                                     {L"font-weight",           L"bold"},
                                                                                                     {L"line-height",           L"1.2"},
                                                                                                     {L"text-align",            L"left"},
                                                                                                     {L"border",                L"currentColor inset 0,264583mm "},
                                                                                                     {L"margin",                L"0.5em auto"},
                                                                                                     {L"color",                 L"black"},
                                                                                                     {L"height",                L"0,529167mm "},
                                                                                                     {L"list-style-type",       L"disc"},
                                                                                                     {L"margin-bottom",         L"1em"},
                                                                                                     {L"margin-right",          L"0"},
                                                                                                     {L"margin-top",            L"1em"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/skazki.css", pData8));

    std::vector<std::string> arSelectors9 = {"#mocha-stats li"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData9= {arSelectors9,{{L"list-style",            L"none"},
                                                                                                     {L"display",               L"inline-block"},
                                                                                                     {L"margin",                L"0 1,322917mm "},
                                                                                                     {L"padding-top",           L"2,910417mm "}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/mocha.css", pData9));

    std::vector<std::string> arSelectors10 = {"#mocha .test .html-error"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData10= {arSelectors10,{{L"overflow",                 L"auto"},
                                                                                                       {L"color",                    L"black"},
                                                                                                       {L"line-height",              L"1.5"},
                                                                                                       {L"display",                  L"block"},
                                                                                                       {L"float",                    L"left"},
                                                                                                       {L"clear",                    L"left"},
                                                                                                       {L"font",                     L"3,175000mm  / 1.5 monaco , monospace"},
                                                                                                       {L"margin",                   L"1,322917mm "},
                                                                                                       {L"padding",                  L"3,968750mm "},
                                                                                                       {L"border",                   L"0,264583mm  solid #eee"},
                                                                                                       {L"max-width",                L"calc(100% - 11,112500mm )"},
                                                                                                       {L"max-height",               L"79,375000mm "},
                                                                                                       {L"word-wrap",                L"break-word"},
                                                                                                       {L"border-bottom-color",      L"#ddd"},
                                                                                                       {L"-webkit-box-shadow",       L"0 0,264583mm 0,793750mm  #eee"},
                                                                                                       {L"-moz-box-shadow",          L"0 0,264583mm 0,793750mm  #eee"},
                                                                                                       {L"box-shadow",               L"0 0,264583mm 0,793750mm  #eee"},
                                                                                                       {L"-webkit-border-radius",    L"0,793750mm "},
                                                                                                       {L"-moz-border-radius",       L"0,793750mm "},
                                                                                                       {L"border-radius",            L"0,793750mm "}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/mocha.css", pData10));

    std::vector<std::string> arSelectors11 = {"#forkongithub a"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData11 {arSelectors11,{{L"width",                    L"200px"},
                                                                                                      {L"position",                 L"absolute"},
                                                                                                      {L"top",                      L"60px"},
                                                                                                      {L"right",                    L"-60px"},
                                                                                                      {L"-webkit-transform",        L"rotate(45deg)"},
                                                                                                      {L"-ms-transform",            L"rotate(45deg)"},
                                                                                                      {L"transform",                L"rotate(45deg)"},
                                                                                                      {L"box-shadow",               L"4px 4px 10px rgba(0 , 0 , 0 , 0.8)"},
                                                                                                      {L"-webkit-transition",       L"0.5s"},
                                                                                                      {L"background",               L"#000"},
                                                                                                      {L"color",                    L"#fff"},
                                                                                                      {L"font-family",              L"arial , sans-serif"},
                                                                                                      {L"font-size",                L"1rem"},
                                                                                                      {L"font-weight",              L"bold"},
                                                                                                      {L"line-height",              L"2rem"},
                                                                                                      {L"padding",                  L"5px 40px"},
                                                                                                      {L"position",                 L"absolute"},
                                                                                                      {L"right",                    L"-60px"},
                                                                                                      {L"text-align",               L"center"},
                                                                                                      {L"text-decoration",          L"none"},
                                                                                                      {L"top",                      L"60px"},
                                                                                                      {L"transform",                L"rotate(45deg)"},
                                                                                                      {L"transition",               L"0.5s"},
                                                                                                      {L"width",                    L"200px"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/demo-page.css", pData11));


    std::vector<std::string> arSelectors12 = {".hvr-bounce-in", ".hvr-pop"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData12 {arSelectors12,{{L"display",                      L"inline-block"},
                                                                                                      {L"vertical-align",               L"middle"},
                                                                                                      {L"-webkit-transform",            L"perspective(1px) translateZ(0)"},
                                                                                                      {L"transform",                    L"perspective(1px) translateZ(0)"},
                                                                                                      {L"box-shadow",                   L"0 0 1px rgba(0 , 0 , 0 , 0)"},
                                                                                                      {L"-webkit-transition-duration",  L"0.5s"},
                                                                                                      {L"transition-duration",          L"0.5s"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/hover.css", pData12));

    std::vector<std::string> arSelectors13 = {".calibre9", ".calibre10", ".calibre11", ".calibre12", ".calibre13", ".calibre14"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData13 {arSelectors13,{{L"display",          L"block"},
                                                                                                      {L"line-height",      L"normal"},
                                                                                                      {L"text-align",       L"center"},
                                                                                                      {L"height",           L"auto"},
                                                                                                      {L"width",            L"auto"},
                                                                                                      {L"font-size",        L"0.75em"},
                                                                                                      {L"vertical-align",   L"super"},
                                                                                                      {L"margin",           L"1em 0.2em 1em 4em"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/skazki.css", pData13));

    std::vector<std::string> arSelectors14 = {"body", "#carbonads"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData14 {arSelectors14,{{L"background-color", L"hsl(0 , 0% , 98%)"},
                                                                                                      {L"border-radius",    L"0,041667in "},
                                                                                                      {L"box-shadow",       L"0 0 0 0,010417in  hsla(0 , 0% , 0% , .1)"},
                                                                                                      {L"color",            L"#333"},
                                                                                                      {L"display",          L"block"},
                                                                                                      {L"float",            L"none"},
                                                                                                      {L"font-family",      L"-apple-system , BlinkMacSystemFont , \"Segoe UI\" , Roboto , Oxygen-Sans , Ubuntu , Cantarell , \"Helvetica Neue\" , Helvetica , Arial , sans-serif"},
                                                                                                      {L"line-height",      L"1.5"},
                                                                                                      {L"margin",           L"0 auto"},
                                                                                                      {L"margin-bottom",    L"0,208333in "},
                                                                                                      {L"max-width",        L"3,437500in "},
                                                                                                      {L"overflow",         L"hidden"},
                                                                                                      {L"padding",          L"0,416667in 0 0,104167in  0"},
                                                                                                      {L"text-align",       L"center"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/demo-page.css", pData14));

    std::vector<std::string> arSelectors15 = {".carbon-img img", ".carbon-img"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData15 {arSelectors15,{{L"display",          L"inline-block"},
                                                                                                      {L"margin",           L"0"},
                                                                                                      {L"height",           L"auto"},
                                                                                                      {L"margin-bottom",    L"0,083333in "},
                                                                                                      {L"line-height",      L"1"},
                                                                                                      {L"max-width",        L"1,354167in "},
                                                                                                      {L"float",            L"left"},
                                                                                                      {L"width",            L"2,083333in "}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/demo-page.css", pData15));

    std::vector<std::string> arSelectors16 = {"p.Title", "  p"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData16 {arSelectors16,{{L"-epub-hyphens",    L"none"},
                                                                                                      {L"color",            L"#000000"},
                                                                                                      {L"font-family",      L"\"Minion Pro Disp\" , serif"},
                                                                                                      {L"font-size",        L"2.5em"},
                                                                                                      {L"font-style",       L"normal"},
                                                                                                      {L"font-variant",     L"small-caps"},
                                                                                                      {L"font-weight",      L"bold"},
                                                                                                      {L"line-height",      L"1.2"},
                                                                                                      {L"margin-bottom",    L"0"},
                                                                                                      {L"margin-left",      L"0"},
                                                                                                      {L"margin-right",     L"0"},
                                                                                                      {L"margin-top",       L"0"},
                                                                                                      {L"orphans",          L"1"},
                                                                                                      {L"page-break-after", L"auto"},
                                                                                                      {L"page-break-before",L"auto"},
                                                                                                      {L"text-align",       L"left"},
                                                                                                      {L"text-decoration",  L"none"},
                                                                                                      {L"text-indent",      L"0"},
                                                                                                      {L"text-transform",   L"none"},
                                                                                                      {L"widows",           L"1"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/idGeneratedStyles.css", pData16));

    std::vector<std::string> arSelectors17 = {"#test"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData17 {arSelectors17,{{L"background",   L"black"},
                                                                                                      {L"color",        L"yellow"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/test.css", pData17));

    std::vector<std::string> arSelectors18 = {"body"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData18 {arSelectors18,{{L"padding",   L"0,028935pt  0 0"},
                                                                                                      {L"font",      L"0,002025pt  / 1.3 \"Segoe UI\" , Arial , sans-serif"},
                                                                                                      {L"min-height",L"0,072338pt "},
                                                                                                      {L"margin",    L"0"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/styles.css", pData18));

    std::vector<std::string> arSelectors19 = {"#_idContainer000"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData19 {arSelectors19,{{L"display",  L"inline-block"},
                                                                                                      {L"height",   L"0,440972pc "},
                                                                                                      {L"width",    L"0,569444pc "}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/idGeneratedStyles.css", pData19));

    std::vector<std::string> arSelectors20 = {".hint--always.hint--top-left::after"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData20 {arSelectors20,{{L"-webkit-transform",    L"translateX(-100%) translateY(- 2,116667mm )"},
                                                                                                      {L"-moz-transform",       L"translateX(-100%) translateY(- 2,116667mm )"},
                                                                                                      {L"transform",            L"translateX(-100%) translateY(- 2,116667mm )"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/hint.css", pData20));

    std::vector<std::string> arSelectors21 = {"#main .container article.post > header h1.giga"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData21 {arSelectors21,{{L"background",    L"black"},
                                                                                                      {L"color",         L"#777"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/test.css", pData21));

    std::vector<std::string> arSelectors22 = {"#main .container + header h1"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData22 {arSelectors22,{{L"margin",    L"2,645833mm "},
                                                                                                      {L"padding",   L"10%"},
                                                                                                      {L"background",L"black"}}};

    arTestDatas.push_back(std::make_pair(L"../../../../cssFiles/test.css", pData22));


    std::vector<NSCSS::UnitMeasure> arUnitMeasure = {NSCSS::Cantimeter, NSCSS::Cantimeter, NSCSS::Millimeter, NSCSS::Cantimeter,
                                                     NSCSS::Cantimeter, NSCSS::Cantimeter, NSCSS::Cantimeter, NSCSS::Millimeter,
                                                     NSCSS::Millimeter, NSCSS::Millimeter, NSCSS::Default,    NSCSS::Default,
                                                     NSCSS::Pixel,      NSCSS::Inch,       NSCSS::Inch,       NSCSS::Point,
                                                     NSCSS::Default,    NSCSS::Point,      NSCSS::Peak,       NSCSS::Millimeter,
                                                     NSCSS::Millimeter, NSCSS::Millimeter, NSCSS::Millimeter};

    for (size_t i = 0; i < arTestDatas.size(); i++)
    {
        std::wstring sFilePath = NSFile::GetProcessDirectory() + arTestDatas[i].first;
        NSCSS::CCssCalculator oCSS;
        oCSS.AddStylesFromFile(sFilePath);

        NSCSS::CCompiledStyle oStyle = oCSS.GetCompiledStyle(arTestDatas[i].second.first, arUnitMeasure[i]);

        bool bError = false;
        std::wstring sError;

        if (oStyle.GetSize() == 0)
        {
            bError = true;
        }
        else if (oStyle.GetSize() == arTestDatas[i].second.second.size())
        {
            auto iter2 = arTestDatas[i].second.second.begin();
            for (auto iter1 = oStyle.GetBegin(); iter1 != oStyle.GetEnd(); iter1++)
            {
                if (iter1->first != iter2->first ||
                    iter1->second != iter2->second)
                {
                    sError += iter1->first + L" : " + iter1->second + L"  -  " + iter2->first + L" : " + iter2->second + L"\n";
                    bError = true;
                }
                iter2++;
            }
        }
        else
        {
            sError = std::to_wstring(i + 1);
            sError += L"\n-----FILE(" + arTestDatas[i].first + L")-----\n";
            for (auto iter = oStyle.GetBegin(); iter != oStyle.GetEnd(); iter++)
                sError += iter->first + L" : " + iter->second + L"\n";

            sError += L"-----DATA-----\n";
            for (auto iter = arTestDatas[i].second.second.begin(); iter != arTestDatas[i].second.second.end(); iter++)
                sError += iter->first + L" : " + iter->second + L"\n";

            bError = true;
        }

        std::string arSels;
        for (size_t j = 0; j < arTestDatas[i].second.first.size() - 1; j++)
            arSels += arTestDatas[i].second.first[j] + ',';
        arSels += arTestDatas[i].second.first[arTestDatas[i].second.first.size() - 1];

        std::wcout << i + 1 << " - " << arTestDatas[i].first;
        std::cout << " - " << arSels << " - ";

        if (bError)
        {
            std::cout << "FALSE" << std::endl;
            std::wcout << sError << std::endl;
        }
        else
            std::cout << "TRUE" << std::endl;

    }
    std::cout << "-----END-----" << std::endl;

// Тесты багов
/*
    // Стандарный katana_parser не считывает @page
    // Также, если заккомитить всё, что было до тестов бага, то всё будет крашиться с ошибкой "-1073741819"

    const char* css = "#page{margin:10px; padding:5pt} "
                      "@page{color:white;}";
    KatanaOutput* output = katana_parse(css, strlen(css), KatanaParserModeStylesheet);
    katana_dump_output(output);
    katana_destroy_output(output);

    // Из-за того, что стандарный katana_parser не считывает @page, не считывает и CCssCalculator

    std::wstring sFilePath = NSFile::GetProcessDirectory() + L"../../../../cssFiles/test.css";
    NSCSS::CCssCalculator oCSS;
    oCSS.AddStyles("@page {padding:0pt;margin:0pt;}");
    oCSS.Print();
*/
}
