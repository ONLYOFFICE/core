// djvu wrapper
#include "../libdjvu/DjVuDocument.h"
#include "../libdjvu/DjVuImage.h"
#include "../libdjvu/GBitmap.h"
#include "../libdjvu/GScaler.h"
#include "../libdjvu/IFFByteStream.h"
#include "../libdjvu/BSByteStream.h"
#include "../libdjvu/DataPool.h"
#include "../libdjvu/DjVuText.h"
#include "../libdjvu/DjVmNav.h"
#include "../libdjvu/IW44Image.h"

class CDjvuFile
{
public:
    GP<DjVuDocument> m_doc;
};

extern "C"
{
    CDjvuFile* DJVU_Load(unsigned char* pData, unsigned int len);
    void DJVU_Close(CDjvuFile* file);
    int* DJVU_GetInfo(CDjvuFile* file);
    unsigned char* DJVU_GetPixmap(CDjvuFile* file, int page_index, int w, int h);
    void DJVU_Delete(unsigned char* pData);
}

CDjvuFile* DJVU_Load(unsigned char* pData, unsigned int len)
{
    GP<ByteStream> stream = ByteStream::create(pData, (size_t)len);
    CDjvuFile* file = new CDjvuFile();
    file->m_doc = DjVuDocument::create(stream);
    file->m_doc->wait_get_pages_num();
    int pages_count = file->m_doc->get_pages_num();
    if (0 == pages_count)
    {
        delete file;
        return NULL;
    }
    return file;
}

void DJVU_Close(CDjvuFile* file)
{
    delete file;
}

int* DJVU_GetInfo(CDjvuFile* file)
{
    int pages_count = file->m_doc->get_pages_num();
    int* buffer = new int[pages_count * 3 + 1];
    int buffer_index = 0;
    buffer[buffer_index++] = pages_count;
    for (int page = 0; page < pages_count; ++page)
    {
        int nW = 0;
        int nH = 0;
        int nDpi = 0;
        file->m_doc->ReadPageInfo(page, nW, nH, nDpi);
        buffer[buffer_index++] = nW;
        buffer[buffer_index++] = nH;
        buffer[buffer_index++] = nDpi;
    }
    return buffer;
}

unsigned char* DJVU_GetPixmap(CDjvuFile* file, int page_index, int w, int h)
{
    GP<DjVuImage> page = file->m_doc->get_page(page_index);

    int w_real = page->get_real_width();
    int h_real = page->get_real_height();

    unsigned char* pixmap = new unsigned char[4 * w * h];
    if (!pixmap)
        return NULL;

    if (page->is_legal_photo() || page->is_legal_compound())
    {
        GRect oRectAll(0, 0, w, h);
        GP<GPixmap> pImage = page->get_pixmap(oRectAll, oRectAll);

        unsigned char* pixmap_cur = pixmap;
        for (int j = h - 1; j >= 0; --j)
        {
            GPixel* pLine = pImage->operator [](j);

            for (int i = 0; i < w; ++i, pixmap_cur += 4, ++pLine)
            {
                pixmap_cur[0] = pLine->r;
                pixmap_cur[1] = pLine->g;
                pixmap_cur[2] = pLine->b;
                pixmap_cur[3] = 255;
            }
        }
    }
    else if (page->is_legal_bilevel())
    {
        GRect oRectAll(0, 0, w, h);
        GP<GBitmap> pBitmap = page->get_bitmap(oRectAll, oRectAll, 4);
        int nPaletteEntries = pBitmap->get_grays();

        unsigned int* palette = new unsigned int[nPaletteEntries];

        // Create palette for the bitmap
        int color = 0xff0000;
        int decrement = color / (nPaletteEntries - 1);
        for (int i = 0; i < nPaletteEntries; ++i)
        {
            unsigned char level = (unsigned char)(color >> 16);
            palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
            color -= decrement;
        }

        unsigned int* pixmap_cur = (unsigned int*)pixmap;
        for (int j = h - 1; j >= 0; --j)
        {
            unsigned char* pLine = pBitmap->operator [](j);

            for (int i = 0; i < w; ++i, ++pixmap_cur, ++pLine)
            {
                if (*pLine < nPaletteEntries)
                {
                    *pixmap_cur = palette[*pLine];
                }
                else
                {
                    *pixmap_cur = palette[0];
                }
            }
        }

        delete [] palette;
    }
    else
    {
        GRect oRectAll(0, 0, w, h);
        GP<GPixmap> pImage = page->get_pixmap(oRectAll, oRectAll);

        if (NULL != pImage)
        {
            unsigned char* pixmap_cur = pixmap;
            for (int j = h - 1; j >= 0; --j)
            {
                GPixel* pLine = pImage->operator [](j);

                for (int i = 0; i < w; ++i, pixmap_cur += 4, ++pLine)
                {
                    pixmap_cur[0] = pLine->r;
                    pixmap_cur[1] = pLine->g;
                    pixmap_cur[2] = pLine->b;
                    pixmap_cur[3] = 255;
                }
            }
        }
        else
        {
            GP<GBitmap> pBitmap = page->get_bitmap(oRectAll, oRectAll, 4);

            if (NULL != pBitmap)
            {
                int nPaletteEntries = pBitmap->get_grays();
                unsigned int* palette = new unsigned int[nPaletteEntries];

                // Create palette for the bitmap
                int color = 0xff0000;
                int decrement = color / (nPaletteEntries - 1);
                for (int i = 0; i < nPaletteEntries; ++i)
                {
                    unsigned char level = (unsigned char)(color >> 16);
                    palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
                    color -= decrement;
                }

                unsigned int* pixmap_cur = (unsigned int*)pixmap;
                for (int j = h - 1; j >= 0; --j)
                {
                    unsigned char* pLine = pBitmap->operator [](j);

                    for (int i = 0; i < w; ++i, ++pixmap_cur, ++pLine)
                    {
                        if (*pLine < nPaletteEntries)
                        {
                            *pixmap_cur = palette[*pLine];
                        }
                        else
                        {
                            *pixmap_cur = palette[0];
                        }
                    }
                }

                delete [] palette;
            }
        }
    }

    return pixmap;
}

void DJVU_Delete(unsigned char* pData)
{
    delete [] pData;
}

#ifdef WASM_MODE_DEBUG
#include <stdio.h>
int main(int argc, char *argv[])
{
    FILE* f = fopen("D:\\1.djvu", "rb");
    fseek(f, 0, SEEK_END);
    int file_size = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* file_data = new unsigned char[file_size];
    fread((void*)file_data, 1, file_size, f);
    fclose(f);

    CDjvuFile* file = DJVU_Load(file_data, file_size);
    delete [] file_data;

    if (file)
    {
        int* info = DJVU_GetInfo(file);

        int pages_count = *info;
        if (pages_count > 0)
        {
            unsigned char* pixmap = DJVU_GetPixmap(file, 0, info[1] * 96 / info[3], info[2] * 96 / info[3]);
            delete [] pixmap;
        }

        delete [] info;
    }

    return 0;
}
#endif
