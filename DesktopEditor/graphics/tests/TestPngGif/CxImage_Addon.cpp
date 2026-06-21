#include "CxImage_Addon.h"
#include "CxImagePNG_Addon.h"

CxImage_Addon::CxImage_Addon()
{
    image_data = NULL;
    image_data_size = 0;
}

CxImage_Addon::~CxImage_Addon()
{
    if(image_data != NULL)
    {
        free(image_data);
    }
}

BYTE* CxImage_Addon::GetData() { return image_data; }
size_t CxImage_Addon::GetDataSize() const { return image_data_size; }
const char* CxImage_Addon::GetError() { return error; }

bool CxImage_Addon::GetSpecializedGIFInfo(FILE* hFile, uint32_t imagetype)
{
    CxIOFile file(hFile);
    return GetSpecializedGIFInfo(&file, imagetype);
}
bool CxImage_Addon::GetSpecializedGIFInfo(CxFile* hFile, uint32_t imagetype)
{
    if (hFile == NULL){
        strcpy(error,CXIMAGE_ERR_NOFILE);
        return false;
    }

    uint32_t pos = hFile->Tell();

#ifdef CXIMAGE_SUPPORT_PNG
    if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PNG==imagetype){
        CxImagePNG_Addon* newima = new CxImagePNG_Addon();
        if (!newima)
            return false;
        if (newima->GetSpecializedGIFInfo(hFile))
        {
            Transfer(*newima);
            delete newima;
            return true;
        } else {
            strcpy(error, newima->GetError());
            hFile->Seek(pos,SEEK_SET);
            delete newima;
            if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
                return false;
        }
    }
#endif

    strcpy(error, "GetInfo: Unknown wrong format");
    return false;
}

void CxImage_Addon::Transfer(CxImage_Addon& from)
{
    image_data_size = from.GetDataSize();
    image_data = (BYTE*)malloc(image_data_size);

    memcpy(image_data, from.GetData(), image_data_size);
    strcpy(error, from.GetError());
}
