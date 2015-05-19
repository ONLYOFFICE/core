#include <stdio.h>
#include <string.h>
#include "agg_pixfmt_rgb.h"

enum frame_size
{
    frame_width = 320,
    frame_height = 200
};

// Writing the buffer to a .PPM file, assuming it has 
// RGB-structure, one byte per color component
//--------------------------------------------------
bool write_ppm(const unsigned char* buf, 
               unsigned width, 
               unsigned height, 
               const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd)
    {
        fprintf(fd, "P6 %d %d 255 ", width, height);
        fwrite(buf, 1, width * height * 3, fd);
        fclose(fd);
        return true;
    }
    return false;
}

// Draw a black frame around the rendering buffer
//--------------------------------------------------
template<class Ren>
void draw_black_frame(Ren& ren)
{
    unsigned i;
    agg::rgba8 c(0,0,0);
    for(i = 0; i < ren.height(); ++i)
    {
        ren.copy_pixel(0,               i, c);
        ren.copy_pixel(ren.width() - 1, i, c);
    }
    for(i = 0; i < ren.width(); ++i)
    {
        ren.copy_pixel(i, 0,                c);
        ren.copy_pixel(i, ren.height() - 1, c);
    }
}


int main()
{
    //--------------------
    // Allocate the buffer.
    // Clear the buffer, for now "manually"
    // Create the rendering buffer object
    // Create the Pixel Format renderer
    // Do something simple, draw a diagonal line
    // Write the buffer to agg_test.ppm
    // Free memory

    unsigned char* buffer = new unsigned char[frame_width * frame_height * 3];

    memset(buffer, 255, frame_width * frame_height * 3);

    agg::rendering_buffer rbuf(buffer, 
                               frame_width, 
                               frame_height, 
                               frame_width * 3);

    agg::pixfmt_rgb24 pixf(rbuf);

    unsigned i;
    for(i = 0; i < pixf.height()/2; ++i)
    {
        pixf.copy_pixel(i, i, agg::rgba8(127, 200, 98));
    }

    draw_black_frame(pixf);
    write_ppm(buffer, frame_width, frame_height, "agg_test.ppm");

    delete [] buffer;
    return 0;
}

