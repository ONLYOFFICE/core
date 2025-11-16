#include "CxImagePNG_Addon.h"

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
        png_destroy_read_struct(&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
        return false;
    }

    // use custom I/O functions
    png_set_read_fn(png_ptr, hFile, /*(png_rw_ptr)*/user_read_data);
    png_set_error_fn(png_ptr,error,/*(png_error_ptr)*/user_error_fn,NULL);

    png_ptr->flags = 0x8000;

    png_read_info(png_ptr, info_ptr);
    uint8_t *row_pointers=NULL;
    row_pointers = new uint8_t[info_ptr->rowbytes + 8];
    int y = 0;
    do
    {
        png_read_row(png_ptr, row_pointers, NULL);
        y++;
    } while(y < png_ptr->height);

    png_read_end(png_ptr, info_ptr);

    for (png_size_t i = 0; i < info_ptr->unknown_chunks_num; i++)
    {
        if(!strncmp("MSOFFICE9.0", (const char*)info_ptr->unknown_chunks[i].data, 11))
        {
            png_size_t length = info_ptr->unknown_chunks[i].size - 11;
            png_int_32 gif_terminator = -1;
            png_bytep data = (png_bytep)png_malloc(png_ptr, length + 1);

            memcpy(data, info_ptr->unknown_chunks[i].data + 11, length);
            data[length] = 0x00;
            png_free(png_ptr, info_ptr->unknown_chunks[i].data);

            for (int iter = 0; iter < length; iter++)
            {
                if (data[iter] == 0x3B)
                {
                    gif_terminator = iter;
                }
            }

            if (gif_terminator == -1)
            {
                png_free(png_ptr, data);
                data = NULL;
                strcpy(error, "No terminator in GIF imformation chunk");
                return false;
            }

            data[gif_terminator + 1] = 0x00;
            image_data_size = gif_terminator + 1;
            image_data = (BYTE*)malloc(image_data_size);
            memcpy(image_data, data, image_data_size);

            png_free(png_ptr, data);
            data = NULL;

            return true;
        } else {
            png_free(png_ptr, info_ptr->unknown_chunks[i].data);
        }
    }

    strcpy(error, "No GIF information chunk");
    return false;
}
