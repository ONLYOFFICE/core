#include "ximajpg.h"

#if CXIMAGE_SUPPORT_JPG

#include "../../../../../cximage/jpeg/jmorecfg.h"
#include "ximaiter.h"
#include <setjmp.h>

struct jpg_error_mgr
{
    struct jpeg_error_mgr pub;	/* "public" fields */
    jmp_buf setjmp_buffer;		/* for return to caller */
    char* buffer;				/* error message <CSC>*/
};
typedef jpg_error_mgr *jpg_error_ptr;

static void ima_jpeg_error_exit(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    jpg_error_ptr myerr = (jpg_error_ptr) cinfo->err;
    /* Create the message */
    myerr->pub.format_message (cinfo, myerr->buffer);
    /* Send it to stderr, adding a newline */
    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

CxImageJPG::CxImageJPG() : CxImage(CXIMAGE_FORMAT_JPG)
{
#if CXIMAGEJPG_SUPPORT_EXIF
    m_exif = NULL;
    memset(&info.ExifInfo, 0, sizeof(EXIFINFO));
#endif
}
CxImageJPG::~CxImageJPG()
{
#if CXIMAGEJPG_SUPPORT_EXIF
    if (m_exif) delete m_exif;
#endif
}
#if CXIMAGEJPG_SUPPORT_EXIF
bool CxImageJPG::DecodeExif(CxFile* hFile)
{
    m_exif = new CxExifInfo(&info.ExifInfo);
    if (m_exif)
    {
        int32_t pos = hFile->Tell();
        m_exif->DecodeExif(hFile);
        hFile->Seek(pos, SEEK_SET);
        return m_exif->m_exifinfo->IsExif;
    }
    return false;
}
#endif //CXIMAGEJPG_SUPPORT_EXIF
#if CXIMAGE_SUPPORT_DECODE
bool CxImageJPG::Decode(CxFile* hFile)
{
    bool is_exif = false;
#if CXIMAGEJPG_SUPPORT_EXIF
    is_exif = DecodeExif(hFile);
#endif

    CImageIterator iter(this);
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler. <CSC> */
    struct jpg_error_mgr jerr;
    jerr.buffer=info.szLastError;
    /* More stuff */
    JSAMPARRAY buffer;	/* Output row buffer */
    int32_t row_stride;		/* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */

    /* Step 1: allocate and initialize JPEG decompression object */
    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = ima_jpeg_error_exit;

    CxFileJpg src(hFile);

    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object, close the input file, and return.
        */
        jpeg_destroy_decompress(&cinfo);
        return 0;
    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */
    //jpeg_stdio_src(&cinfo, infile);
    cinfo.src = &src;

    /* Step 3: read file parameters with jpeg_read_header() */
    (void) jpeg_read_header(&cinfo, TRUE);

    /* Step 4 <chupeev> handle decoder options*/
    uint32_t dwCodecOptions = GetCodecOption(CXIMAGE_FORMAT_JPG); //[nm_114]
    if ((dwCodecOptions & DECODE_GRAYSCALE) != 0)
        cinfo.out_color_space = JCS_GRAYSCALE;
    if ((dwCodecOptions & DECODE_QUANTIZE) != 0) {
        cinfo.quantize_colors = TRUE;
        cinfo.desired_number_of_colors = GetJpegQuality();
    }
    if ((dwCodecOptions & DECODE_DITHER) != 0)
        cinfo.dither_mode = m_nDither;
    if ((dwCodecOptions & DECODE_ONEPASS) != 0)
        cinfo.two_pass_quantize = FALSE;
    if ((dwCodecOptions & DECODE_NOSMOOTH) != 0)
        cinfo.do_fancy_upsampling = FALSE;

//<DP>: Load true color images as RGB (no quantize)
/* Step 4: set parameters for decompression */
/*  if (cinfo.jpeg_color_space!=JCS_GRAYSCALE) {
 *	cinfo.quantize_colors = TRUE;
 *	cinfo.desired_number_of_colors = 128;
 *}
 */ //</DP>

    cinfo.scale_num = 1;
    // Set the scale <ignacio>
    cinfo.scale_denom = GetJpegScale();

    // Borrowed the idea from GIF implementation <ignacio>
    if (info.nEscape == -1) {
        // Return output dimensions only
        jpeg_calc_output_dimensions(&cinfo);
        head.biWidth = cinfo.output_width;
        head.biHeight = cinfo.output_height;
        info.dwType = CXIMAGE_FORMAT_JPG;
        jpeg_destroy_decompress(&cinfo);
        return true;
    }

    /* Step 5: Start decompressor */
    jpeg_start_decompress(&cinfo);

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    */
    //Create the image using output dimensions <ignacio>
    //Create(cinfo.image_width, cinfo.image_height, 8*cinfo.output_components, CXIMAGE_FORMAT_JPG);
    Create(cinfo.output_width, cinfo.output_height, 8*cinfo.output_components, CXIMAGE_FORMAT_JPG);

    if (!pDib) longjmp(jerr.setjmp_buffer, 1);  //<DP> check if the image has been created

    if (is_exif){
#if CXIMAGEJPG_SUPPORT_EXIF
    if ((info.ExifInfo.Xresolution != 0.0) && (info.ExifInfo.ResolutionUnit != 0))
        SetXDPI((int32_t)(info.ExifInfo.Xresolution/info.ExifInfo.ResolutionUnit));
    if ((info.ExifInfo.Yresolution != 0.0) && (info.ExifInfo.ResolutionUnit != 0))
        SetYDPI((int32_t)(info.ExifInfo.Yresolution/info.ExifInfo.ResolutionUnit));
#endif
    } else {
        switch (cinfo.density_unit)
        {
        case 0:	// [andy] fix for aspect ratio...
            if((cinfo.Y_density > 0) && (cinfo.X_density > 0))
                SetYDPI((int32_t)(GetXDPI()*(float(cinfo.Y_density)/float(cinfo.X_density))));
            break;
        case 2: // [andy] fix: cinfo.X/Y_density is pixels per centimeter
            SetXDPI((int32_t)floor(cinfo.X_density * 2.54 + 0.5));
            SetYDPI((int32_t)floor(cinfo.Y_density * 2.54 + 0.5));
            break;
        default:
            SetXDPI(cinfo.X_density);
            SetYDPI(cinfo.Y_density);
        }
    }

    if (cinfo.out_color_space==JCS_GRAYSCALE){
        SetGrayPalette();
        head.biClrUsed =256;
    } else {
        if (cinfo.quantize_colors){
            SetPalette(cinfo.actual_number_of_colors, cinfo.colormap[0], cinfo.colormap[1], cinfo.colormap[2]);
            head.biClrUsed=cinfo.actual_number_of_colors;
        } else {
            head.biClrUsed=0;
        }
    }

    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;

    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    iter.Upset();
    while (cinfo.output_scanline < cinfo.output_height) {

        if (info.nEscape) longjmp(jerr.setjmp_buffer, 1); // <vho> - cancel decoding

        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        // info.nProgress = (int32_t)(100*cinfo.output_scanline/cinfo.output_height);
        //<DP> Step 6a: CMYK->RGB */
        if ((cinfo.num_components==4)&&(cinfo.quantize_colors==FALSE)){
            uint8_t k,*dst,*src;
            dst = iter.GetRow();
            src = buffer[0];
            for(int32_t x3=0,x4=0; x3<(int32_t)info.dwEffWidth && x4<row_stride; x3+=3, x4+=4){
                k=src[x4+3];
                dst[x3]  =(uint8_t)((k * src[x4+2])/255);
                dst[x3+1]=(uint8_t)((k * src[x4+1])/255);
                dst[x3+2]=(uint8_t)((k * src[x4+0])/255);
            }
        } else {
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            iter.SetRow(buffer[0], row_stride);
        }
            iter.PrevRow();
    }

    /* Step 7: Finish decompression */
    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    //<DP> Step 7A: Swap red and blue components
    // not necessary if swapped red and blue definition in jmorecfg.h;ln322 <W. Morrison>
    if ((cinfo.num_components==3)&&(cinfo.quantize_colors==FALSE)){
        uint8_t* r0=GetBits();
        for(int32_t y=0;y<head.biHeight;y++){
            if (info.nEscape) longjmp(jerr.setjmp_buffer, 1); // <vho> - cancel decoding
            RGBtoBGR(r0,3*head.biWidth);
            r0+=info.dwEffWidth;
        }
    }

    /* Step 8: Release JPEG decompression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    /* And we're done! */
    return true;
}
#endif //CXIMAGE_SUPPORT_DECODE

#endif // CXIMAGE_SUPPORT_JPG

