//#include <QCoreApplication>

#include "../../fontengine/ApplicationFonts.h"
#include "../../graphics/GraphicsRenderer.h"

int main(int argc, char *argv[])
{
    CApplicationFonts oFonts;
    oFonts.Initialize();

    CBgraFrame oFrame;
    int nRasterW = 300;
    int nRasterH = 300;

    double dWidthMM = 25.4 * nRasterW / 96;
    double dHeightMM = 25.4 * nRasterH / 96;

    if (false)
    {
        nRasterW *= 2;
        nRasterH *= 2;
    }

    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(-4 * nRasterW);

    BYTE* pDataRaster = new BYTE[4 * nRasterW * nRasterH];
    memset(pDataRaster, 0xFF, 4 * nRasterW * nRasterH);
    oFrame.put_Data(pDataRaster);

    CFontManager* pManager = oFonts.GenerateFontManager();
    pManager->SetSubpixelRendering(true, false);
    CImageFilesCache* pCache = new CImageFilesCache();

    CGraphicsRenderer oRenderer;
    oRenderer.SetFontManager(pManager);
    oRenderer.SetImageCache(pCache);

    oRenderer.CreateFromBgraFrame(&oFrame);
    oRenderer.SetSwapRGB(false);

    oRenderer.put_Width(dWidthMM);
    oRenderer.put_Height(dHeightMM);

    oRenderer.put_FontPath(L"D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\DesktopEditor\\ChromiumBasedEditors2\\app\\test\\src\\build\\win64\\Debug\\fonts\\fonts\\truetype\\xmind\\Lobster-Regular.ttf");
    oRenderer.put_FontFaceIndex(0);
    oRenderer.put_FontSize(18);

    oRenderer.CommandDrawText(L"Lobster", 10, dHeightMM / 2, 0, 0);

    oFrame.SaveFile(L"D:\\111.png", 4);

    return 0;
}
