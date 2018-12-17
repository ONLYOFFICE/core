#include <QCoreApplication>

#include <vector>
#include <tchar.h>

#include "../../common/Directory.h"
#include "../../fontengine/ApplicationFonts.h"
#include "../../../HtmlRenderer/include/ASCSVGWriter.h"
#include "MetaFile.h"

#if defined(_WIN64)
    #pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

    #if defined(_DEBUG)
        #pragma comment(lib, "../../../../build/lib/win_32/DEBUG/HtmlRenderer.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/DEBUG/graphics.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/DEBUG/kernel.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
    #else
        #pragma comment(lib, "../../../../build/lib/win_32/HtmlRenderer.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/graphics.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/kernel.lib")
        #pragma comment(lib, "../../../../build/lib/win_32/UnicodeConverter.lib")
    #endif
    #pragma comment(lib, "../../../../build/bin/icu/win_32/icuuc.lib")
#endif
std::vector<std::wstring> GetAllFilesInFolder(std::wstring wsFolder, std::wstring wsExt)
{
    std::vector<std::wstring> vwsNames;

    std::wstring wsSearchPath = wsFolder;
    wsSearchPath.append(L"*.");
    wsSearchPath.append(wsExt);

    WIN32_FIND_DATA oFindData;
    HANDLE hFind = ::FindFirstFile(wsSearchPath.c_str(), &oFindData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(oFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                vwsNames.push_back(oFindData.cFileName);
            }
        } while (::FindNextFile(hFind, &oFindData));
        ::FindClose(hFind);
    }
    return vwsNames;
}
void ConvertFileToRaster(MetaFile::IMetaFile *pMetaFile, std::wstring wsFilePath)
{
    pMetaFile->Close();

    std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"png");
    if (pMetaFile->LoadFromFile(wsFilePath.c_str()))
    {
        pMetaFile->ConvertToRaster(wsDstFilePath.c_str(), 4, 1000);

        pMetaFile->Close();
    }
}
//void ConvertFileToSVG(MetaFile::IMetaFile *pMetaFile, NSHtmlRenderer::CASCSVGWriter &oWriterSVG, std::wstring wsFilePath)
//{
//	pMetaFile->Close();
//
//	std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"svg");
//
//	if (pMetaFile->LoadFromFile(wsFilePath.c_str()))
//	{
//		double x = 0, y = 0, w = 0, h = 0;
//		pMetaFile->GetBounds(&x, &y, &w, &h);
//
//		double _max = (w >= h) ? w : h;
//		double dKoef = 100000.0 / _max;
//
//		int WW = (int)(dKoef * w + 0.5);
//		int HH = (int)(dKoef * h + 0.5);
//
//		oWriterSVG.Reset();
//		oWriterSVG.put_Width(WW);
//		oWriterSVG.put_Height(HH);
//
//		pMetaFile->DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);
//
//		oWriterSVG.SaveFile(wsDstFilePath);
//
//		pMetaFile->Close();
//	}
//}
void ConvertFolder(MetaFile::IMetaFile *pMetaFile, std::wstring wsFolderPath, const int nType)
{
    pMetaFile->Close();

    std::wstring sExt = L"*";

    //NSHtmlRenderer::CASCSVGWriter oWriterSVG;
    //oWriterSVG.SetFontManager(pMetaFile->get_FontManager());

    switch(nType)
    {
        case MetaFile::c_lMetaEmf: sExt = L"emf"; break;
        case MetaFile::c_lMetaWmf: sExt = L"wmf"; break;
        case MetaFile::c_lMetaSvm: sExt = L"svm"; break;
    }
    std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath,  sExt);

    for (size_t nIndex = 0; nIndex < vFiles.size(); nIndex++)
    {
        std::wstring wsFilePath = wsFolderPath;
        wsFilePath.append(vFiles.at(nIndex));

        //ConvertFileToSVG(pMetaFile, oWriterSVG, wsFilePath);
        ConvertFileToRaster(pMetaFile, wsFilePath);

        printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
    }
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NSFonts::IApplicationFonts *pFonts = NSFonts::NSApplication::Create();
    pFonts->Initialize();

    std::wstring	sMetafilesFolder	= L"D://test//_emf//image1.emf";
    int				nType				= MetaFile::c_lMetaEmf;
    if (argc > 1)
        sMetafilesFolder = argv[1];

    if (argc > 2)
    {
        try
        {
            nType = _wtoi(argv[2]);
        }
        catch(...)
        {
        }
    }

    MetaFile::IMetaFile *pMetaFile = MetaFile::Create(pFonts);

    if (NSFile::CFileBinary::Exists(sMetafilesFolder))
    {
        //NSHtmlRenderer::CASCSVGWriter oWriterSVG;
        //oWriterSVG.SetFontManager(pMetaFile->get_FontManager());

        ConvertFileToRaster(pMetaFile, sMetafilesFolder);
        //ConvertFileToSVG(pMetaFile, oWriterSVG, sMetafilesFolder);
    }
    else if (NSDirectory::Exists(sMetafilesFolder))
    {
        ConvertFolder(pMetaFile, sMetafilesFolder, nType);
    }

    return a.exec();
}
