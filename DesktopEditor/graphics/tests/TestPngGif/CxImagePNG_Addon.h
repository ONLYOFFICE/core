#ifndef CXIMAGEPNG_ADDON_H
#define CXIMAGEPNG_ADDON_H


#include "CxImage_Addon.h"
#include "../../cximage/png/pngpriv.h"

#define png_msOG PNG_CHUNK(109, 115,  79,  71)

class CxImagePNG_Addon: public CxImage_Addon
{
public:
    CxImagePNG_Addon(): CxImage_Addon() {}

    bool GetSpecializedGIFInfo(FILE* hFile);
    bool GetSpecializedGIFInfo(CxFile* hFile);
protected:
    void png_handle_msOG(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
    {
        png_byte buf[11];
        png_bytep data;
        png_size_t slength;
        png_int_32 gif_terminator = -1;

        png_crc_read(png_ptr, buf, 11);

        if (strncmp("MSOFFICE9.0", (const char*)buf, 11))
        {
            png_warning(png_ptr, "No GIF format in msOG chunk");
            return;
        }

        data = (png_bytep)png_malloc_warn(png_ptr, length - 10);

        if (data == NULL)
        {
           png_warning(png_ptr, "No memory to process msOG chunk");
           return;
        }

        slength = length - 11;
        png_crc_read(png_ptr, data, slength);

        if (png_crc_finish(png_ptr, 0))
        {
           png_free(png_ptr, data);
           data = NULL;
           return;
        }

        data[slength] = 0x00;

        if (strncmp("GIF89a", (const char*)data, 6))
        {
            png_free(png_ptr, data);
            data = NULL;
            png_warning(png_ptr, "No GIF format in msOG chunk");
            return;
        }

        for (int iter = 0; iter < slength; iter++)
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
            png_warning(png_ptr, "No GIF terminator in msOG chunk");
            return;
        }

        data[gif_terminator + 1] = 0x00;
        image_data = (png_bytep)png_malloc_warn(png_ptr, length);

        png_memcpy(image_data, data, length);
        image_data_size = length;

        png_free(png_ptr, data);
        data = NULL;
    }

    int png_read_chunks (png_structp png_ptr, png_infop info_ptr)
    {
        png_debug(1, "in png_read_end");

        if (png_ptr == NULL)
           return -1;

        /* Read and check the PNG file signature. */
        png_read_sig(png_ptr, info_ptr);

        for(;;)
        {
            png_uint_32 length = png_read_chunk_header(png_ptr);
            png_uint_32 chunk_name = png_ptr->chunk_name;

            if(chunk_name == png_msOG)
            {
                png_handle_msOG(png_ptr, info_ptr, length);
                return 1;
            }
            else if(chunk_name == png_IEND)
                return -1;
            else
                png_crc_finish(png_ptr, length);
        }
        return -1;
    }

    static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
        if (hFile == NULL || hFile->Read(data,1,length) != length) png_error(png_ptr, "Read Error");
    }

    static void user_error_fn(png_structp png_ptr,png_const_charp error_msg)
    {
        strncpy((char*)png_ptr->error_ptr,error_msg,255);
        longjmp(png_ptr->longjmp_buffer, 1);
    }
};


#endif // CXIMAGEPNG_ADDON_H
