#include "base.h"
#include "engine.h"

#include <algorithm> // для std::min в get_file_in_archive
#include <vector>

unzFile unzOpenHelp(BUFFER_IO* buffer)
{
    zlib_filefunc_def ffunc;
    fill_buffer_filefunc(&ffunc, buffer);
    return unzOpen2(NULL, &ffunc);
}
zipFile zipOpenHelp(BUFFER_IO* buffer)
{
    zlib_filefunc_def ffunc;
    fill_buffer_filefunc(&ffunc, buffer);
    return zipOpen2(NULL, APPEND_STATUS_CREATE, NULL, &ffunc);
}
unsigned int GetLength(BYTE* x)
{
    return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
}

// begin from (ZipUtilsCP.cpp)
bool current_file_is_find(unzFile uf, const char* filename)
{
    char filename_inzip[256];
    int err = UNZ_OK;

    unz_file_info file_info;

    err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
    if (err != UNZ_OK)
        return false;

    std::string name(filename_inzip);
    //todooo есть ли необходимость свести все к нижнему ???
    return strcmp(filename, name.c_str()) == 0;
}
bool get_file(unzFile unzip_file_handle, BYTE* arr, uInt array_size)
{
    if (UNZ_OK == unzOpenCurrentFile(unzip_file_handle))
    {
        int data_read_size = unzReadCurrentFile(unzip_file_handle, arr, array_size);

        unzCloseCurrentFile(unzip_file_handle);
        return data_read_size == array_size;
    }
    return false;
}
static const std::string get_filename_from_unzfile(unzFile unzip_file_handle)
{
    char filename_inzip[256];

    if (UNZ_OK == unzGetCurrentFileInfo(unzip_file_handle, NULL, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0))
        return std::string(filename_inzip);

    return "";
}
bool get_file_in_archive(unzFile unzip_file_handle, const char* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
{
    if (NULL == fileInBytes)
        return false;
    (*fileInBytes) = NULL;
    do
    {
        if (current_file_is_find(unzip_file_handle, filePathInZip) == true)
        {
          unz_file_info file_info;
          unzGetCurrentFileInfo(unzip_file_handle, &file_info, NULL, 0, NULL, 0, NULL, 0);

          if (nFileSize > 0)
              nFileSize = (std::min)(nFileSize, file_info.uncompressed_size);
          else
              nFileSize = file_info.uncompressed_size;

          (*fileInBytes) = new BYTE[nFileSize];
          get_file(unzip_file_handle, (*fileInBytes), nFileSize);
          return true;
        }
        // else just skip the erroneous file
    } while (UNZ_OK == unzGoToNextFile(unzip_file_handle));

    return false;
}
// end from (ZipUtilsCP.cpp)

void* Zlib_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void  Zlib_Free(void* p)
{
    if (p) ::free(p);
}

Zlib* Zlib_Create()
{
    return new Zlib();
}
Zlib* Zlib_Load(unsigned char* buffer, unsigned long size)
{
    return new Zlib(buffer, size);
}
void  Zlib_Destroy(Zlib* p)
{
    if (p)
    {
        delete p;
    }
}

unsigned char* Zlib_GetPathsInArchive(Zlib* p)
{
    if (p && p->isInit())
    {
        BUFFER_IO* buf = new BUFFER_IO;
        buf->buffer = p->buffer;
        buf->nSize  = p->size;
        unzFile uf = unzOpenHelp(buf);

        p->m_oPaths.ClearNoAttack();
        p->m_oPaths.SkipLen();
        do
        {
            unz_file_info file_info;
            unzGetCurrentFileInfo(uf, &file_info, NULL, 0, NULL, 0, NULL, 0);
            if (file_info.uncompressed_size != 0)
            {
                std::string sPath = get_filename_from_unzfile(uf);
                p->m_oPaths.WriteString((unsigned char*)sPath.c_str(), sPath.length());
            }
        } while (UNZ_OK == unzGoToNextFile(uf));
        p->m_oPaths.WriteLen();
        return p->m_oPaths.GetBuffer();
    }
    return NULL;
}
unsigned char* Zlib_GetFileFromArchive(Zlib* p, const char* path)
{
    if (p && p->isInit())
    {
        BUFFER_IO* buf = new BUFFER_IO;
        buf->buffer = p->buffer;
        buf->nSize  = p->size;
        unzFile uf = unzOpenHelp(buf);

        unsigned char* file = new BYTE;
        unsigned long  nFileSize = 0;
        bool fileIsIn = get_file_in_archive(uf, path, &file, nFileSize);

        unzClose(uf);
        if (fileIsIn)
        {
            p->m_oFile.Clear();
            p->m_oFile.WriteString(file, nFileSize);
            return p->m_oFile.GetBuffer();
        }
    }
    return NULL;
}
unsigned char* Zlib_CompressFiles(Zlib* p,   unsigned char* tree)
{
    if (p && tree)
    {
        BUFFER_IO* buf = new BUFFER_IO;
        buf->bGrow = 1;
        buf->nCurrentPos = 0;
        zipFile zip_file_handle = zipOpenHelp(buf);

        unsigned int nLength = GetLength(tree);
        unsigned int i = 4;
        nLength -= 4;

        while (i < nLength)
        {
            unsigned int nPathLength = GetLength(tree + i);
            i += 4;
            std::string sPath((char*)(tree + i), nPathLength);
            i += nPathLength;
            unsigned int nFileLength = GetLength(tree + i);
            i += 4;

            if (ZIP_OK != zipOpenNewFileInZip( zip_file_handle, sPath.c_str(), NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, -1 ) ||
                ZIP_OK != zipWriteInFileInZip(zip_file_handle, tree + i, nFileLength) ||
                ZIP_OK != zipCloseFileInZip(zip_file_handle))
            {
                zipClose(zip_file_handle, NULL);
                return NULL;
            }
            i += nFileLength;
        }
        zipClose(zip_file_handle, NULL);
        p->buffer = buf->buffer;
        p->size   = buf->nSize;

        CData* oRes = new CData;
        oRes->WriteString(buf->buffer, buf->nSize);
        return oRes->GetBuffer();
    }
    return NULL;
}
