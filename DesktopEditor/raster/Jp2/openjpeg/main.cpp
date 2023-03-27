#include "./opj_bgraframe.h"
#include <algorithm>

int main(int argc, char **argv)
{
    CBgraFrame oFrame;
    if (openjpeg::Parse(L"PATH_TO_IMAGE_SRC", &oFrame, true))
    {
        int nW = oFrame.get_Width();
        int nH = oFrame.get_Height();
        int nMax = std::max(nW, nH);
        if (nMax > 5000)
        {
            double dKoef = 5000.0 / nMax;
            oFrame.Resize((int)(dKoef * nW), (int)(dKoef * nH), true);
        }

        oFrame.SaveFile(L"PATH_TO_IMAGE_DST", 4);
    }

    return 0;
}
