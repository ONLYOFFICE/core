#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"

class CGraphicsFileDrawing
{
private:
    CBgraFrame* m_pFrame;
    CGraphicsRenderer* m_pGraphics;
public:
    CGraphicsFileDrawing(double width_px, double height_px, double width_mm, double height_mm)
    {
        int nRasterW = (int)width_px;
        int nRasterH = (int)height_px;
        BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

        unsigned int back = 0xffffff;
        unsigned int* pData32 = (unsigned int*)pData;
        unsigned int* pData32End = pData32 + nRasterW * nRasterH;
        while (pData32 < pData32End)
            *pData32++ = back;

        m_pFrame = new CBgraFrame();
        m_pFrame->put_IsRGBA(true);
        m_pFrame->put_Data(pData);
        m_pFrame->put_Width(nRasterW);
        m_pFrame->put_Height(nRasterH);

        m_pGraphics = (CGraphicsRenderer*)NSGraphics::Create();
        m_pGraphics->CreateFromBgraFrame(m_pFrame);
        m_pGraphics->SetSwapRGB(true);
        m_pGraphics->put_Width(width_mm);
        m_pGraphics->put_Height(height_mm);

        NSFonts::IApplicationFonts* m_pApplicationFonts = NSFonts::NSApplication::Create();
        NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();
        m_pGraphics->SetFontManager(pManager);
    }
    ~CGraphicsFileDrawing()
    {
        RELEASEOBJECT(m_pGraphics);
        RELEASEOBJECT(m_pFrame);
    }

    int GetPagesCount()
    {
        return 1;
    }
    int GetPageHeight(int nPageIndex)
    {
        return m_pFrame->get_Height();
    }
    int GetPageWidth (int nPageIndex)
    {
        return m_pFrame->get_Width();
    }
    BYTE* GetPage    (int nPageIndex, int nRasterW, int nRasterH)
    {
        return m_pFrame->get_Data();
    }

    CGraphicsRenderer* GetGraphicsForTest()
    {
        return m_pGraphics;
    }
    CBgraFrame*        GetFrameForTest()
    {
        return m_pFrame;
    }
};

#endif // _WASM_GRAPHICS_
