#ifndef _ZIPFOLDER_H_
#define _ZIPFOLDER_H_

#include "./../../../../OfficeUtils/src/ZipBuffer.h"
#include "./../../../common/File.h"
#include "./../../../common/Directory.h"

class IZipFolder
{
public:
    virtual bool read  (const std::wstring& path, BYTE*& data, DWORD& length) = 0;
    virtual void write (const std::wstring& path, BYTE*  data, DWORD  length) = 0;
    virtual void move  (const std::wstring& sSrc,   const std::wstring& sDst) = 0;
    virtual bool exists(const std::wstring& path) = 0;
    virtual void remove(const std::wstring& path) = 0;
    virtual void createDirectory(const std::wstring& path) = 0;
    virtual void writeZipFolder(BYTE* data, DWORD& length) = 0;
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion) = 0;

    virtual void writeXml(const std::wstring& path, const std::wstring& xml)
    {
        std::string sXmlUtf8 = U_TO_UTF8(xml);
        write(path, (BYTE*)sXmlUtf8.c_str(), (DWORD)sXmlUtf8.length());
    }
};

class CZipFolder : public IZipFolder
{
    std::wstring m_sFolder;
public:
    CZipFolder(const std::wstring& folder) : m_sFolder(folder) {}

    virtual bool read  (const std::wstring& path, BYTE*& data, DWORD& length)
    {
        if (NSFile::CFileBinary::Exists(path))
            return NSFile::CFileBinary::ReadAllBytes(path, &data, length);
        return false;
    }
    virtual void write (const std::wstring& path, BYTE*  data, DWORD  length)
    {
        NSFile::CFileBinary::Remove(path);
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(path);
        oFile.WriteFile(data, length);
        oFile.CloseFile();
    }
    virtual void move  (const std::wstring& sSrc,   const std::wstring& sDst)
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
    virtual void writeZipFolder(BYTE* data, DWORD& length)
    {
    }
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        return NSDirectory::GetFiles(path, bIsRecursion);
    }
};

class CZipFolderMemory : public IZipFolder
{
    CZipBuffer* m_zlib;
public:
    CZipFolderMemory(BYTE* data, DWORD length)
    {
        m_zlib = new CZipBuffer(data, length);
    }
    std::string normalPath(const std::wstring& path)
    {
        if (!path.empty() && path[0] == L'/')
            return U_TO_UTF8(path.substr(1));
        return U_TO_UTF8(path);
    }

    virtual bool read  (const std::wstring& path, BYTE*& data, DWORD& length)
    {
        std::string sPath = normalPath(path);
        std::pair<DWORD, BYTE*> oRes = m_zlib->getFile(sPath);
        if (oRes.first)
        {
            length = oRes.first;
            data = oRes.second;
            return true;
        }
        return false;
    }
    virtual void write (const std::wstring& path, BYTE*  data, DWORD  length)
    {
        std::string sPath = normalPath(path);
        m_zlib->addFile(sPath, data, length);
    }
    virtual void move  (const std::wstring& sSrc,   const std::wstring& sDst)
    {
        std::string sSrcPath = normalPath(sSrc);
        std::pair<DWORD, BYTE*> oFile = m_zlib->getFile(sSrcPath);
        m_zlib->removeFile(sSrcPath);
        m_zlib->addFile(normalPath(sDst), oFile.second, oFile.first);
    }
    virtual bool exists(const std::wstring& path)
    {
        std::string sPath = normalPath(path);
        std::vector<std::string> sPaths = m_zlib->getPaths();
        for (std::string& i : sPaths)
            if (i == sPath)
                return true;
        return false;
    }
    virtual void remove(const std::wstring& path)
    {
        std::string sPath = normalPath(path);
        m_zlib->removeFile(sPath);
    }
    virtual void createDirectory(const std::wstring& path)
    {
    }
    virtual void writeZipFolder(BYTE* data, DWORD& length)
    {
        std::pair<DWORD, BYTE*> oRes = m_zlib->save();
        length = oRes.first;
        data = oRes.second;
        m_zlib->close();
    }
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        std::string sPath = normalPath(path);
        std::vector<std::string> sPaths = m_zlib->getPaths();
        std::vector<std::wstring> sRes;

        for (std::string& i : sPaths)
        {
            if (bIsRecursion)
            {
                if (i.find(sPath) == 0)
                    sRes.push_back(L'/' + UTF8_TO_U(i));
            }
            else
            {
                size_t nFindDirectory = i.find(sPath);
                if (nFindDirectory == 0 && i.find_first_of("\\/", sPath.length()) == std::wstring::npos)
                    sRes.push_back(L'/' + UTF8_TO_U(i));
            }
        }
        return sRes;
    }
};

#endif //_ZIPFOLDER_H_
