#ifndef _ZIPFOLDER_H_
#define _ZIPFOLDER_H_

#include "./../../common/File.h"
#include "./../../common/Directory.h"
#include "./../../../../OfficeUtils/js/wasm/src/base.h"
#include "./../../../../OfficeUtils/js/wasm/src/engine.h"

class IZipFolder
{
public:
    virtual bool read  (const std::wstring& path, BYTE* data, DWORD& length) = 0;
    virtual void write (const std::wstring& path, BYTE* data, DWORD  length) = 0;
    virtual void move  (const std::wstring& sSrc,  const std::wstring& sDst) = 0;
    virtual bool exists(const std::wstring& path) = 0;
    virtual void remove(const std::wstring& path) = 0;
    virtual void createDirectory(const std::wstring& path) = 0;
    virtual void writeZipFolder (const std::wstring& path) = 0;
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion) = 0;
};

#if defined(CreateDirectory)
#undef CreateDirectory
#endif

class CZipFolder : public IZipFolder
{
    std::wstring m_sFolder;
public:
    CZipFolder(const std::wstring& folder) : m_sFolder(folder) {}

    virtual bool read  (const std::wstring& path, BYTE* data, DWORD& length)
    {
        if (NSFile::CFileBinary::Exists(path))
            return NSFile::CFileBinary::ReadAllBytes(path, &data, length);
        return false;
    }
    virtual void write (const std::wstring& path, BYTE* data, DWORD  length)
    {
        NSFile::CFileBinary::Remove(path);
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(path);
        oFile.WriteFile(data, length);
        oFile.CloseFile();
    }
    virtual void move  (const std::wstring& sSrc,  const std::wstring& sDst)
    {
        NSFile::CFileBinary::Move(sSrc, sDst);
    }
    virtual bool exists(const std::wstring& path)
    {
        return NSFile::CFileBinary::Exists(path);
    }
    virtual void remove(const std::wstring& path)
    {
        NSFile::CFileBinary::Remove(path);
    }
    virtual void createDirectory(const std::wstring& path)
    {
        if (!NSDirectory::Exists(path))
            NSDirectory::CreateDirectory(path);
    }
    virtual void writeZipFolder (const std::wstring& path)
    {
    }
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        return NSDirectory::GetFiles(path, bIsRecursion);
    }
};

class CZipFolderMemory : public IZipFolder
{
    struct CFile
    {
        std::wstring m_sPath;
        BYTE* m_pData;
        DWORD m_nLength;
        CFile(const std::wstring& sPath, BYTE* pData, DWORD nLength) : m_sPath(sPath), m_pData(pData), m_nLength(nLength) {}
    };

    std::vector<CFile> arrFiles;
public:
    CZipFolderMemory(BYTE* data, DWORD length)
    {
        Zlib* zlib = Zlib_Load(data, length);
        BYTE* sPaths = Zlib_GetPathsInArchive(zlib);
        DWORD nLength = GetLength(sPaths);
        DWORD i = 4;
        nLength -= 4;

        while (i < nLength)
        {
            DWORD nPathLength = GetLength(sPaths + i);
            i += 4;

            BYTE* sFile = Zlib_GetFileFromArchive(zlib, std::string((char*)(sPaths + i), nPathLength).c_str());

            arrFiles.push_back(CFile(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(sPaths + i, nPathLength), sFile + 4, GetLength(sFile)));
            i += nPathLength;
        }
        RELEASEARRAYOBJECTS(sPaths);
    }

    virtual bool read  (const std::wstring& path, BYTE* data, DWORD& length)
    {
        for (CFile& oFile : arrFiles)
        {
            if (oFile.m_sPath == path)
            {
                data   = oFile.m_pData;
                length = oFile.m_nLength;
                return true;
            }
        }
        return false;
    }
    virtual void write (const std::wstring& path, BYTE* data, DWORD  length)
    {
        bool bExists = false;
        for (CFile& oFile : arrFiles)
        {
            if (oFile.m_sPath == path)
            {
                RELEASEARRAYOBJECTS(oFile.m_pData);
                oFile.m_pData = data;
                oFile.m_nLength = length;
                bExists = true;
                break;
            }
        }
        if (!bExists)
            arrFiles.push_back(CFile(path, data, length));
    }
    virtual void move  (const std::wstring& sSrc,  const std::wstring& sDst)
    {
        for (CFile& oFile : arrFiles)
        {
            if (oFile.m_sPath == sSrc)
            {
                oFile.m_sPath = sDst;
                break;
            }
        }
    }
    virtual bool exists(const std::wstring& path)
    {
        for (CFile& oFile : arrFiles)
            if (oFile.m_sPath == path)
                return true;
        return false;
    }
    virtual void remove(const std::wstring& path)
    {
        for (std::vector<CFile>::iterator i = arrFiles.begin(); i != arrFiles.end(); i++)
        {
            if (i->m_sPath == path)
            {
                arrFiles.erase(i);
                break;
            }
        }
    }
    virtual void createDirectory(const std::wstring& path)
    {
    }
    virtual void writeZipFolder (const std::wstring& path)
    {
        CData* oData = new CData();
        oData->SkipLen();
        for (CFile& oFile : arrFiles)
        {
            BYTE* pData = NULL;
            LONG   nLen = 0;
            NSFile::CUtf8Converter::GetUtf8StringFromUnicode(oFile.m_sPath.c_str(), oFile.m_sPath.length(), pData, nLen);
            oData->WriteString(pData, nLen);
            oData->WriteString(oFile.m_pData, oFile.m_nLength);
        }
        oData->WriteLen();

        Zlib* zipFile = Zlib_Create();
        BYTE* oRes = Zlib_CompressFiles(zipFile, oData->GetBuffer());

        NSFile::CFileBinary oFile;
        oFile.CreateFileW(path);
        oFile.WriteFile(oRes + 4, GetLength(oRes));
        oFile.CloseFile();
    }
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        std::vector<std::wstring> sRes;

        for (CFile& oFile : arrFiles)
        {
            if (bIsRecursion)
            {
                if (oFile.m_sPath.find(path) == 0)
                    sRes.push_back(oFile.m_sPath);
            }
            else
            {
                size_t nFindDirectory = oFile.m_sPath.find(path);
                if (nFindDirectory == 0 && oFile.m_sPath.find_first_of(L"\\/", path.length()) == std::wstring::npos)
                    sRes.push_back(oFile.m_sPath);
            }
        }

        return sRes;
    }

    DWORD GetLength(BYTE* x)
    {
        return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
    }
};

#endif //_ZIPFOLDER_H_
