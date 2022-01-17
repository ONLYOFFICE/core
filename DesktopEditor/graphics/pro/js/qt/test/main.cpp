#include "../../../../../common/Types.h"
#include "../../../../../common/File.h"
#include "../../wasm/src/raster.h"

#include <string>
#include <iostream>

int main()
{
    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.png", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();

    CBgraFrame* img = Raster_Load(pData, nBytesCount);

    BYTE* pResInt = Raster_GetRGBA(img);
    int nCountInt = 4 * Raster_GetHeight(img) * Raster_GetWidth(img);

    for (int i = 0; i < (nCountInt > 100 ? 100 : nCountInt); i++)
        std::cout << (int)pResInt[i] << " ";

    img->SaveFile(NSFile::GetProcessDirectory() + L"/res.jpg", 3);
    Raster_Destroy(img);
    return 0;
}
