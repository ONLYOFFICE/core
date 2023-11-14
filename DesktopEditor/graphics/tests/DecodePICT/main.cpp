#include <iostream>
#include "pict.h"
#include "../../../graphics/Image.h"

int main(int argc, char *argv[])
{    
    Image* pImage = new Image;
    AquireImage(pImage);

    const char* filepath = "C:\\Users\\mrkir\\Downloads\\Telegram Desktop\\image1000.pict";

    if (!DecodePICT(filepath, pImage))
    {
        std::cout << pImage->error << std::endl;
    }
    else
    {
        std::cout << pImage->m_nHeight << std::endl;
        std::cout << pImage->m_nWidth << std::endl;
        std::cout << pImage->m_ndepth << std::endl;
        std::cout << pImage->colors << std::endl;
    }

    std::wstring fres = L"result.bmp";

    Aggplus::CImage* cimg = new Aggplus::CImage;

    cimg->Create(pImage->ppixels, pImage->m_nWidth, pImage->m_nHeight, 4 * pImage->m_nWidth);
    cimg->SaveFile(fres, 1);

    return 0;
}
