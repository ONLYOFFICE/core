#ifndef _ZIPBUFFER_H_
#define _ZIPBUFFER_H_

#include <string>
#include <vector>

#include "../../Common/kernel_config.h"
#include "../../DesktopEditor/common/Types.h"

// Работает с архивом в памяти
class KERNEL_DECL CZipBuffer
{
private:
    // Архивированные данные
    BYTE* m_zipFile;
    // Длина архивированных данных
    DWORD m_sizeZip;
    // Файл
    struct CFile
    {
        // Относительный путь файла в архиве
        std::string m_sPath;
        // Данные файла
        BYTE* m_pData;
        // Длина файла
        DWORD m_nLength;
        CFile(const std::string& sPath, BYTE* pData, DWORD nLength) : m_sPath(sPath), m_pData(pData), m_nLength(nLength) {}
    };
    // Вектор файлов в архиве
    std::vector<CFile> m_arrFiles;

    friend class CZipFolderMemory;
public:
    // Создает архив в памяти
    CZipBuffer()
    {
        create();
    }
    // Открывает архив в памяти, переданные данные необходимо освободить после использования класса
    CZipBuffer(BYTE* buffer, DWORD size)
    {
        open(buffer, size);
    }
    // Закрывает архив и очищает память
    ~CZipBuffer()
    {
        close();
    }

    // Создает архив в памяти
    void create();
    // Открывает архив в памяти, переданные данные необходимо освободить после использования класса
    void open(BYTE* buffer, DWORD size);
    // Закрывает архив и очищает память
    void close();
    // Перемещает файл в архиве
    void move(const std::string& sSrc, const std::string& sDst);

    // Возвращает вектор путей в архиве
    std::vector<std::string> getPaths();
    // Сохраняет архив в переданную память, полученные данные необходимо освободить
    void save(BYTE*& data, DWORD& length);
    // По относительно пути в архиве возвращает файл, полученные данные будут освобождены после использования класса
    void getFile(const std::string& sPath, BYTE*& data, DWORD& length);
    // По относительно пути в архиве добавляет файл,  переданные данные будут освобождены после использования класса
    void addFile   (const std::string& sPath, BYTE* data, DWORD length);
    // По относительно пути в архиве удаляет файл
    bool removeFile(const std::string& sPath);
};

#endif // _ZIPBUFFER_H_
