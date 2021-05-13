#include "ZipBuffer.h"
#include "ZipUtilsCP.h"
#include "zlib-1.2.11/contrib/minizip/ioapibuf.h"

#include <algorithm> // для std::min в get_file_in_archive

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

void CZipBuffer::create()
{
    m_zipFile = NULL;
    m_sizeZip = 0;
}
void CZipBuffer::open(BYTE* buffer, DWORD size)
{
    m_zipFile = buffer;
    m_sizeZip = size;

    // Получаем пути в архиве
    BUFFER_IO* buf = new BUFFER_IO;
    buf->buffer = buffer;
    buf->nSize  = size;
    unzFile uf  = unzOpenHelp(buf);

    do
    {
        unz_file_info file_info;
        unzGetCurrentFileInfo(uf, &file_info, NULL, 0, NULL, 0, NULL, 0);
        if (file_info.uncompressed_size != 0)
            m_arrFiles.push_back(CFile(get_filename_from_unzfile(uf), NULL, 0));
    } while (UNZ_OK == unzGoToNextFile(uf));
}
void CZipBuffer::close()
{
    RELEASEARRAYOBJECTS(m_zipFile);
    m_arrFiles.clear();
}

std::vector<std::string> CZipBuffer::getPaths()
{
    std::vector<std::string> oRes;
    for (CFile& oFile : m_arrFiles)
        oRes.push_back(oFile.m_sPath);
    return oRes;
}
std::pair<DWORD, BYTE*> CZipBuffer::save()
{
    BUFFER_IO* buf = new BUFFER_IO;
    buf->bGrow = 1;
    buf->nCurrentPos = 0;
    zipFile zip_file_handle = zipOpenHelp(buf);

    for (CFile& oFile : m_arrFiles)
    {
        if (ZIP_OK != zipOpenNewFileInZip( zip_file_handle, oFile.m_sPath.c_str(), NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, -1 ) ||
            ZIP_OK != zipWriteInFileInZip(zip_file_handle, oFile.m_pData, oFile.m_nLength) ||
            ZIP_OK != zipCloseFileInZip(zip_file_handle))
        {
            zipClose(zip_file_handle, NULL);
            return make_pair(m_sizeZip, m_zipFile);
        }
    }
    zipClose(zip_file_handle, NULL);

    m_zipFile = buf->buffer;
    m_sizeZip = buf->nSize;
    return make_pair(m_sizeZip, m_zipFile);
}
std::pair<DWORD, BYTE*> CZipBuffer::getFile(const std::string& sPath)
{
    BUFFER_IO* buf = new BUFFER_IO;
    buf->buffer = m_zipFile;
    buf->nSize  = m_sizeZip;
    unzFile uf  = unzOpenHelp(buf);

    BYTE* file = new BYTE;
    DWORD nFileSize = 0;
    get_file_in_archive(uf, sPath.c_str(), &file, nFileSize);
    unzClose(uf);
    return make_pair(nFileSize, file);
}
void CZipBuffer::addFile   (const std::string& sPath, BYTE* data, DWORD length)
{
    bool bExists = false;
    for (CFile& oFile : m_arrFiles)
    {
        if (oFile.m_sPath == sPath)
        {
            RELEASEARRAYOBJECTS(oFile.m_pData);
            oFile.m_pData = data;
            oFile.m_nLength = length;
            bExists = true;
            break;
        }
    }
    if (!bExists)
        m_arrFiles.push_back(CFile(sPath, data, length));
}
bool CZipBuffer::removeFile(const std::string& sPath)
{
    for (std::vector<CFile>::iterator i = m_arrFiles.begin(); i != m_arrFiles.end(); i++)
    {
        if (i->m_sPath == sPath)
        {
            m_arrFiles.erase(i);
            return true;
        }
    }
    return false;
}
