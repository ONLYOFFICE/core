#include "MainWindow.h"
#include <QApplication>

#include "../../../../graphics/pro/Fonts.h"
#include "../../../../graphics/pro/Graphics.h"
#include "../../../../fontengine/ApplicationFontsWorker.h"

#include "../../../../raster/BgraFrame.h"
#include "../../../../common/Directory.h"


int main(int argc, char *argv[])
{
    // Check system fonts
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    pMetafile->LoadFromFile(L"C:\\Users\\Kirill.Polyakov.AVSMEDIA\\Desktop\\emf\\testEmf.emf");
    std::wstring wsNewFilePath = NSFile::GetProcessDirectory() + L"\\test.png";
    pMetafile->ConvertToRaster(wsNewFilePath.c_str(), 4, 1000);
    pMetafile->Release();

    pFonts->Release();

    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
