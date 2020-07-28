#include "../../../../../DesktopEditor/common/File.h"

#include "../src/CCssCalculator.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>


int main(int argc, char *argv[])
{
    std::vector<std::pair<std::wstring, std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>>>> arTestDatas;
    std::vector<std::string> arSelectors1 = {"#forkongithub"};
    std::pair<std::vector<std::string>, std::map<std::wstring, std::wstring>> pData1 = {arSelectors1,{{L"display",  L"block"},
                                                                                                      {L"height",   L"5.291667cm "},
                                                                                                      {L"overflow", L"hidden"},
                                                                                                      {L"position", L"absolute"},
                                                                                                      {L"right",    L"0"},
                                                                                                      {L"top",      L"0"},
                                                                                                      {L"width",    L"5.291667cm "}}};

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


    for (size_t i = 0; i < arTestDatas.size(); i++)
    {
        std::wstring sFilePath = NSFile::GetProcessDirectory() + arTestDatas[i].first;

        CCssCalculator oCSS;
        oCSS.AddStyles(sFilePath);

        std::map<std::wstring, std::wstring> mDecl = oCSS.GetCompiledStyleW(arTestDatas[i].second.first, Cantimeter);

        bool bError = false;

        if (mDecl.size() == arTestDatas[i].second.second.size())
        {
            auto iter2 = arTestDatas[i].second.second.begin();
            for (auto iter1 = mDecl.begin(); iter1 != mDecl.end(); iter1++)
            {
                if (iter1->first != iter2->first ||
                    iter1->second != iter2->second)
                {
                    bError = true;
                    break;
                }
                iter2++;
            }
        }
        else
            bError = true;

        std::string arSels;
        for (size_t j = 0; j < arTestDatas[i].second.first.size() - 1; j++)
            arSels += arTestDatas[i].second.first[j] + ',';
        arSels += arTestDatas[i].second.first[arTestDatas[i].second.first.size() - 1];

        std::wcout << arTestDatas[i].first;
        std::cout << " - " << arSels << " - ";

        if (bError)
            std::cout << "FALSE";
        else
            std::cout << "TRUE";
        std::cout << std::endl;


    }


}
