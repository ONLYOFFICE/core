#include <iostream>
#include "pic.h"
#include "../../../graphics/Image.h"

int main(int argc, char *argv[])
{    
    Image* pImage = new Image;
    AquireImage(pImage);

    const char* filepath = "C:\\Users\\mrkir\\Downloads\\Telegram Desktop\\image.pict";

    if (!DecodePICT(filepath, pImage))
    {
        std::cout << pImage->error << std::endl;
    }
    else
    {
        std::wstring fres = L"image.bmp";

        Aggplus::CImage* cimg = new Aggplus::CImage;

        cimg->Create(pImage->ppixels, pImage->m_nWidth, pImage->m_nHeight, pImage->number_channels * pImage->m_nWidth);
        cimg->SaveFile(fres, 1);
    }

    return 0;
}
