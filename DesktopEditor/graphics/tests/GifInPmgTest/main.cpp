#include <fstream>
#include <stdio.h>
#include <iostream>
#include "../../pro/Graphics.h"
#include "../../Image.h"
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../common/File.h"
#include "../../../cximage/png/png.h"
#include "../../../cximage/CxImage/xiofile.h"
#include "../../../cximage/CxImage/ximapng.h"

void PNGAPI user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
    if (hFile == NULL || hFile->Read(data,1,length) != length)
        png_error(png_ptr, "Read Error");
}

int main(int argc, char *argv[])
{
    std::string filepath = "C:\\Users\\mrkir\\core\\DesktopEditor\\graphics\\tests\\GifInPmgTest\\new.png";

    std::FILE* hfile;
    int err = fopen_s(&hfile, filepath.c_str(), "rb");
    if (err != 0)
    {
       return -1;
    }

    png_struct * png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == NULL)
        return -1;

    png_info * png_info_ptr = png_create_info_struct(png_ptr);
    if (png_info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
        return -1;
    }

    png_set_sig_bytes(png_ptr, 8);
    png_set_read_fn(png_ptr, hfile, user_read_data);

    png_read_info(png_ptr, png_info_ptr);


    std::string data = "heignt = " + std::to_string(png_get_image_height(png_ptr, png_info_ptr)) + '\n' + "widht = " +
           std::to_string(png_get_image_width(png_ptr, png_info_ptr)) + '\n' + "color type = " +
           std::to_string(png_get_color_type(png_ptr, png_info_ptr));

    png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
    fclose(hfile);


    NSFile::CFileBinary Fres;
    Fres.CreateFile(L"res.txt");
    Fres.WriteFile(data.c_str(), data.size());
    Fres.CloseFile();

    return 0;
}
