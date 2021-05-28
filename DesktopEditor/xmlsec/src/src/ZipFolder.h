#ifndef _ZIPFOLDER_H_
#define _ZIPFOLDER_H_

#include "./../../../../OfficeUtils/src/ZipBuffer.h"
#include "./../../../common/File.h"
#include "./../../../common/Directory.h"
#include "../../../xml/include/xmlutils.h"
#include "./../include/XmlCertificate.h"

class IFolder
{
public:
    class CBuffer
    {
    public:
        BYTE* Buffer;
        DWORD Size;

    private:
        bool m_bIsDestroy;
        CBuffer(BYTE* data = NULL, DWORD size = 0, bool destroy = false)
        {
            Buffer = data;
            Size = size;
            m_bIsDestroy = destroy;
        }
    public:
        ~CBuffer()
        {
            if (m_bIsDestroy && Buffer != NULL)
                delete [] Buffer;
        }

        void UnsetDestroy()
        {
            m_bIsDestroy = false;
        }

        friend class IFolder;
        friend class CFolderSystem;
        friend class CZipFolderMemory;
    };


public:
    // полный путь по локальному
    virtual std::wstring getFullFilePath(const std::wstring& path) = 0;
    // локальный путь по полному (без первого '/')
    virtual std::wstring getLocalFilePath(const std::wstring& path) = 0;
    // чтение файла в буффер. промежуточный класс нужен, чтобы
    // одна реализация могли отдавать память напрямую, а другая выделять и не хранить у себя
    virtual bool read(const std::wstring& path, CBuffer*& buffer) = 0;
    // запись данных в файл
    virtual void write(const std::wstring& path, BYTE* data, DWORD length) = 0;
    // работа с файлами
    virtual void move(const std::wstring& src, const std::wstring& dst) = 0;
    virtual bool exists(const std::wstring& path) = 0;
    virtual void remove(const std::wstring& path) = 0;
    // работа с директориями
    virtual void createDirectory(const std::wstring& path) = 0;
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool recursion) = 0;
    // финализация
    virtual CBuffer* finalize() { return NULL; }
    // чтение ноды
    virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path) = 0;

    // вспомогательные функции
    void writeXml(const std::wstring& path, const std::wstring& xml)
    {
        std::string sXmlUtf8 = U_TO_UTF8(xml);
        write(path, (BYTE*)sXmlUtf8.c_str(), (DWORD)sXmlUtf8.length());
    }
    void writeXmlA(const std::wstring& path, const std::string& xml)
    {
        write(path, (BYTE*)xml.c_str(), (DWORD)xml.length());
    }
    std::string readXml(const std::wstring& path)
    {
        CBuffer* buffer = NULL;
        if (!read(path, buffer))
            return "";
        std::string sXmlUtf8((char*)buffer->Buffer, (size_t)buffer->Size);
        delete buffer;
        return sXmlUtf8;
    }
    std::string getFileBase64(const std::wstring& path)
    {
        CBuffer* buffer = NULL;
        if (!read(path, buffer))
            return "";

        char* pData = NULL;
        int nLen = 0;
        NSFile::CBase64Converter::Encode(buffer->Buffer, (int)buffer->Size, pData, nLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

        std::string sRet(pData, (size_t)nLen);

        RELEASEARRAYOBJECTS(pData);
        delete buffer;

        return sRet;
    }
    std::string getFileHash(const std::wstring& path, ICertificate* certificate, int nAlg = -1)
    {
        CBuffer* buffer = NULL;
        if (!read(path, buffer))
            return "";

        std::string sRet = certificate->GetHash(buffer->Buffer, buffer->Size, (nAlg == -1) ? certificate->GetHashAlg() : nAlg);
        delete buffer;
        return sRet;
    }
    std::wstring getFileHashW(const std::wstring& path, ICertificate* certificate, int nAlg = -1)
    {
        std::string sTmp = getFileHash(path, certificate, nAlg);
        return UTF8_TO_U(sTmp);
    }
};

class CFolderSystem : public IFolder
{
    std::wstring m_sFolder;

    void correct_folder(std::wstring& folder)
    {
        if (!folder.empty() && folder[folder.length() - 1] == '/')
            folder.erase(folder.length() - 1, 1);
    }

public:
    CFolderSystem(const std::wstring& folder)
    {
        m_sFolder = folder;
#ifdef _WIN32
        NSStringUtils::string_replace(m_sFolder, L"\\", L"/");        
#endif
        correct_folder(m_sFolder);
    }

    virtual std::wstring getFullFilePath(const std::wstring& path)
    {
        std::wstring full_path = path;
#ifdef _WIN32
        NSStringUtils::string_replace(full_path, L"\\", L"/");
#endif
        if (0 == full_path.find(m_sFolder))
            return full_path;
        if (!full_path.empty() && full_path[0] == L'/')
            return m_sFolder + full_path;
        return m_sFolder + L"/" + full_path;
    }
    virtual std::wstring getLocalFilePath(const std::wstring& path)
    {
        std::wstring local_path = path;
#ifdef _WIN32
        NSStringUtils::string_replace(local_path, L"\\", L"/");
#endif
        if (0 == local_path.find(m_sFolder))
            return local_path.substr(m_sFolder.length() + 1);
        if (!local_path.empty() && local_path[0] == L'/')
            return local_path.substr(1);
        return local_path;
    }

    virtual bool read(const std::wstring& path, CBuffer*& buffer)
    {
        buffer = NULL;
        std::wstring sPath = getFullFilePath(path);
        if (NSFile::CFileBinary::Exists(sPath))
        {
            BYTE* pData = NULL;
            DWORD nSize = 0;
            if (NSFile::CFileBinary::ReadAllBytes(sPath, &pData, nSize))
            {
                buffer = new CBuffer(pData, nSize, true);
                return true;
            }
        }
        return false;
    }
    virtual void write(const std::wstring& path, BYTE* data, DWORD length)
    {
        std::wstring sPath = getFullFilePath(path);
        NSFile::CFileBinary::Remove(sPath);
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(sPath);
        oFile.WriteFile(data, length);
        oFile.CloseFile();
    }
    virtual void move(const std::wstring& sSrc,   const std::wstring& sDst)
    {
        NSFile::CFileBinary::Move(getFullFilePath(sSrc), getFullFilePath(sDst));
    }
    virtual bool exists(const std::wstring& path)
    {
        return NSFile::CFileBinary::Exists(getFullFilePath(path));
    }
    virtual void remove(const std::wstring& path)
    {
        NSFile::CFileBinary::Remove(getFullFilePath(path));
    }
    virtual void createDirectory(const std::wstring& path)
    {
        std::wstring sPath = getFullFilePath(path);
        if (!NSDirectory::Exists(sPath))
            NSDirectory::CreateDirectory(sPath);
    }
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        std::wstring folder = getFullFilePath(path);
        correct_folder(folder);
        std::vector<std::wstring> files = NSDirectory::GetFiles(folder, bIsRecursion);
#ifdef _WIN32
        for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
        {
            NSStringUtils::string_replace(*i, L"\\", L"/");
        }
#endif
        return files;
    }
    virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path)
    {
        XmlUtils::CXmlNode node;
        node.FromXmlFile(getFullFilePath(path));
        return node;
    }
};

// Работает с архивом в памяти
class CZipFolderMemory : public IFolder
{
    CZipBuffer* m_zlib;

protected:
    // Конвертирует wstring -> string и убирает '/' в начале, т.к. пути относительные архива
    std::string getLocalFilePathA(const std::wstring& path)
    {
        if (!path.empty() && path[0] == L'/')
            return U_TO_UTF8(path.substr(1));
        return U_TO_UTF8(path);
    }

public:
    // Открывает архив, переданные данные необходимо освободить после использования класса
    CZipFolderMemory(BYTE* data, DWORD length)
    {
        m_zlib = new CZipBuffer(data, length);
    }
    // Закрывает архив и очищает память
    ~CZipFolderMemory()
    {
        delete m_zlib;
    }

    // Относительный путь до файла в архиве
    virtual std::wstring getFullFilePath(const std::wstring& path)
    {
        return path;
    }
    // Относительный путь до файла в архиве без '/' в начале
    virtual std::wstring getLocalFilePath(const std::wstring& path)
    {
        if (!path.empty() && path[0] == L'/')
            return path.substr(1);
        return path;
    }

    // Читает файл по относительному пути в архиве, полученные данные необходимо освободить
    virtual bool read(const std::wstring& path, CBuffer*& buffer)
    {
        buffer = NULL;
        std::string sPath = getLocalFilePathA(path);
        BYTE* pData = NULL;
        DWORD nSize = 0;
        m_zlib->getFile(sPath, pData, nSize);
        if (nSize)
        {
            buffer = new CBuffer(pData, nSize, false);
            return true;
        }
        return false;
    }
    // Пишет файл по относительному пути в архиве, переданные данные необходимо освободить
    virtual void write(const std::wstring& path, BYTE* data, DWORD length)
    {
        std::string sPath = getLocalFilePathA(path);
        BYTE* copyData = new BYTE[length];
        memcpy(copyData, data, length);
        m_zlib->addFile(sPath, copyData, length);
    }
    // Перемещает файл в архиве
    virtual void move(const std::wstring& sSrc, const std::wstring& sDst)
    {
        m_zlib->move(getLocalFilePathA(sSrc), getLocalFilePathA(sDst));
    }
    // Содержится ли файл в архиве
    virtual bool exists(const std::wstring& path)
    {
        std::string sPath = getLocalFilePathA(path);
        std::vector<std::string> sPaths = m_zlib->getPaths();
        return std::find(sPaths.begin(), sPaths.end(), sPath) != sPaths.end();
    }
    // Удаляет файл по относительному пути в архиве
    virtual void remove(const std::wstring& path)
    {
        std::string sPath = getLocalFilePathA(path);
        m_zlib->removeFile(sPath);
    }
    // Создавать директорию в архиве не требуется
    virtual void createDirectory(const std::wstring& path)
    {
    }    
    // Возвращает вектор путей в архиве до файлов расположенной в папке
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        std::string sPath = getLocalFilePathA(path);
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
                if (nFindDirectory == 0)
                {
                    nFindDirectory = i.find_first_of("\\/", sPath.length());
                    if (nFindDirectory != std::wstring::npos && i.find_first_of("\\/", nFindDirectory + 1) == std::wstring::npos)
                        sRes.push_back(L'/' + UTF8_TO_U(i));
                }
            }
        }
        return sRes;
    }
    // Возвращает архивированные данные и закрывает архив, полученные данные необходимо освободить
    virtual CBuffer* finalize()
    {
        BYTE* data = NULL;
        DWORD length = 0;
        m_zlib->save(data, length);
        m_zlib->close();
        return new CBuffer(data, length, true);
    }
    // Читает файл по относительному пути в архиве и формирует из него CXmlNode
    virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path)
    {
        CBuffer* buffer = NULL;
        XmlUtils::CXmlNode node;
        if (!read(path, buffer))
            return node;

        std::string sUtf8((char*)buffer->Buffer, (size_t)buffer->Size);
        node.FromXmlStringA(sUtf8);
        delete buffer;
        return node;
    }
};

#endif //_ZIPFOLDER_H_
