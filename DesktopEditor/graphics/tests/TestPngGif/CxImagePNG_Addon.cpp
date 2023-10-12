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
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        return false;
    }

    // use custom I/O functions
    png_set_read_fn(png_ptr, hFile, /*(png_rw_ptr)*/user_read_data);
    png_set_error_fn(png_ptr,error,/*(png_error_ptr)*/user_error_fn,NULL);

    return png_read_chunks(png_ptr, info_ptr);
}
