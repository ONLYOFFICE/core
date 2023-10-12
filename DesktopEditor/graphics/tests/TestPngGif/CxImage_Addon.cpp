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

BYTE* CxImage_Addon::GetData() const { return image_data; }
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

bool CxImagePNG_Addon::GetSpecializedGIFInfo(FILE* hFile)
{
    CxIOFile file(hFile);
    return GetSpecializedGIFInfo(&file);
}
bool CxImagePNG_Addon::GetSpecializedGIFInfo(CxFile* hFile)
{
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (void *)NULL, NULL, NULL);
    if (png_ptr == NULL)  return false;

    /* Allocate/initialize the memory for image information.  REQUIRED. */
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return false;
    }

    /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier. */
    if (setjmp(png_ptr->longjmp_buffer)) {
        /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        return false;
    }

    // use custom I/O functions
    png_set_read_fn(png_ptr, hFile, /*(png_rw_ptr)*/user_read_data);
    png_set_error_fn(png_ptr,error,/*(png_error_ptr)*/user_error_fn,NULL);

    return png_read_chunks(png_ptr, info_ptr);
}
