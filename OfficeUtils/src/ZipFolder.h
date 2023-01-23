#ifndef _ZIPFOLDER_H_
#define _ZIPFOLDER_H_

#include "ZipBuffer.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

class IFolder
{
public:
    enum IFolderType
    {
        iftFolder = 0,
        iftZip = 1
    };

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
    virtual ~IFolder() {}

    virtual IFolderType getType() = 0;
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
    virtual XmlUtils::CXmlLiteReader getReaderFromFile(const std::wstring& path) = 0;

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
    bool existsXml(const std::wstring& path)
    {
        if (exists(path))
            return true;

        std::vector<std::wstring> arPieces = getFiles(path, false);
        if (0 < arPieces.size())
        {
            std::vector<std::wstring>::iterator iter = arPieces.begin();
            while (iter != arPieces.end())
            {
                std::wstring::size_type len = iter->length();
                std::wstring::size_type pos = iter->rfind(L".piece");
                if (std::wstring::npos != pos && ((pos + 6) == len))
                {
                    return true;
                }
            }
        }

        return false;
    }
    std::string readXml(const std::wstring& path)
    {
        CBuffer* buffer = NULL;
        if (!read(path, buffer))
        {
            std::vector<std::wstring> arPieces = getFiles(path, false);
            if (0 < arPieces.size())
            {
                std::sort(arPieces.begin(), arPieces.end(), compareAsXmlPiece);
                std::vector<std::wstring>::iterator iter = arPieces.begin();
                while (iter != arPieces.end())
                {
                    std::wstring::size_type len = iter->length();
                    std::wstring::size_type pos = iter->rfind(L".piece");
                    if (std::wstring::npos != pos && ((pos + 6) == len))
                    {
                        iter++;
                        continue;
                    }
                    else
                    {
                        iter = arPieces.erase(iter);
                    }
                }
            }
            if (0 < arPieces.size())
            {
                std::string sResult;
                for (std::vector<std::wstring>::iterator iter = arPieces.begin(); iter != arPieces.end(); iter++)
                {
                    CBuffer* bufferPiece = NULL;
                    if (read(*iter, bufferPiece))
                    {
                        sResult += std::string((char*)bufferPiece->Buffer, (size_t)bufferPiece->Size);
                    }
                    delete bufferPiece;
                }
                return sResult;
            }

            return "";
        }
        std::string sXmlUtf8 = XmlUtils::GetUtf8FromFileContent(buffer->Buffer, (unsigned int)buffer->Size);
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

private:
    static bool compareAsXmlPiece(const std::wstring& a, const std::wstring& b)
    {
        size_t aLen = a.length();
        size_t bLen = b.length();

        size_t posA = 0;
        size_t posB = 0;

        int nPartA = 0;
        int nPartB = 0;

        size_t len = (aLen < bLen) ? aLen : bLen;
        if (2 > len)
            goto error;

        while (posA < len)
        {
            if (a[posA] != b[posA])
                break;
            ++posA;
        }

        if (0 == posA)
            goto error;

        posB = posA;

        // не ищем '['. просто первый неравный
        //if ('[' != a[posA - 1] || '[' != b[posB - 1])
        //    goto error;

        while (posA < aLen)
        {
            if (a[posA] < '0' || a[posA] > '9')
                break;
            nPartA = 10 * nPartA + (a[posA] - '0');
            ++posA;
        }
        if (posA == aLen || a[posA] != ']')
            goto error;

        while (posB < bLen)
        {
            if (b[posB] < '0' || b[posB] > '9')
                break;
            nPartB = 10 * nPartB + (b[posB] - '0');
            ++posB;
        }
        if (posB == bLen || b[posB] != ']')
            goto error;

        return nPartA < nPartB;

    error:
        return a < b;
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
    virtual IFolderType getType()
    {
        return iftFolder;
    }

    virtual std::wstring getFullFilePath(const std::wstring& path)
    {
        std::wstring full_path = path;
#ifdef _WIN32
        NSStringUtils::string_replace(full_path, L"\\", L"/");
#endif
        if (0 == full_path.find(m_sFolder))
            return full_path;
        if (full_path.empty())
            return m_sFolder;
        if (full_path[0] == L'/')
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
    virtual XmlUtils::CXmlLiteReader getReaderFromFile(const std::wstring& path)
    {
        XmlUtils::CXmlLiteReader oReader;
        oReader.FromFile(getFullFilePath(path));
        return oReader;
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
        std::string sPath = U_TO_UTF8(path);
        if (!sPath.empty() && sPath[0] == '/')
            return NSSystemPath::NormalizePath(sPath.substr(1));
        return NSSystemPath::NormalizePath(sPath);
    }

public:
    // Открывает архив, переданные данные необходимо освободить после использования класса
    CZipFolderMemory()
    {
        m_zlib = new CZipBuffer();
    }
    CZipFolderMemory(BYTE* data, DWORD length)
    {
        m_zlib = new CZipBuffer(data, length);
    }
    // Закрывает архив и очищает память
    ~CZipFolderMemory()
    {
        delete m_zlib;
    }

    virtual IFolderType getType()
    {
        return iftZip;
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
        m_zlib->addFile(sPath, data, length);
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
        return std::find_if(m_zlib->m_arrFiles.begin(), m_zlib->m_arrFiles.end(), [sPath](const CZipBuffer::CFile& file){ return file.m_sPath == sPath; }) != m_zlib->m_arrFiles.end();
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
    // Возвращает вектор путей расположенных в папке
    virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
    {
        std::string sPath = getLocalFilePathA(path);
        std::vector<std::wstring> sRes;

        for (const CZipBuffer::CFile& i : m_zlib->m_arrFiles)
        {
            if (bIsRecursion)
            {
                if (i.m_sPath.find(sPath) == 0)
                    sRes.push_back(L'/' + UTF8_TO_U(i.m_sPath));
            }
            else
            {
                size_t nFindDirectory = i.m_sPath.find(sPath);
                if (nFindDirectory == 0)
                {
                    nFindDirectory = i.m_sPath.find_first_of("\\/", sPath.length());
                    if (nFindDirectory != std::wstring::npos && i.m_sPath.find_first_of("\\/", nFindDirectory + 1) == std::wstring::npos)
                        sRes.push_back(L'/' + UTF8_TO_U(i.m_sPath));
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
    virtual XmlUtils::CXmlLiteReader getReaderFromFile(const std::wstring& path)
    {
        CBuffer* buffer = NULL;
        XmlUtils::CXmlLiteReader oReader;
        if (!read(path, buffer))
            return oReader;

        std::string sUtf8((char*)buffer->Buffer, (size_t)buffer->Size);
        oReader.FromStringA(sUtf8);
        delete buffer;
        return oReader;
    }
};

#endif //_ZIPFOLDER_H_
