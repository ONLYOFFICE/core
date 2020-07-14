/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef NATIVECONTROL
#define NATIVECONTROL

#include <map>
#include <iostream>

#include "../graphics/pro/Fonts.h"
#include "../graphics/Timer.h"
#include "../common/Directory.h"
#include "../common/Array.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "memorystream.h"
#include "../fontengine/application_generate_fonts_common.h"

#if defined(CreateDirectory)
#undef CreateDirectory
#endif

#if defined(GetTempPath)
#undef GetTempPath
#endif

#if defined(CreateFile)
#undef CreateFile
#endif

class CZipWorker
{
public:
    std::wstring m_sTmpFolder;

    std::vector<std::wstring> m_arFiles;

    std::wstring m_sWorkerFolder;

public:

    CZipWorker()
    {
        m_sWorkerFolder = L"";
    }

    ~CZipWorker()
    {
        Close();
    }

    void Close()
    {
        if (!m_sTmpFolder.empty())
            NSDirectory::DeleteDirectory(m_sTmpFolder);

        m_sTmpFolder = L"";

        m_arFiles.clear();
    }

    bool Open(const std::wstring& sFile)
    {
        m_sTmpFolder = m_sWorkerFolder + L"/nativeZip";
        COfficeUtils oUtils;
        NSDirectory::CreateDirectory(m_sTmpFolder);
        if (S_OK != oUtils.ExtractToDirectory(sFile, m_sTmpFolder, NULL, 0))
            return false;

        CheckDirectory();
        return true;
    }

    bool OpenBase64(const std::string& sData)
    {
        BYTE* pRawData = NULL;
        int nRawSize = 0;
        if (true != NSFile::CBase64Converter::Decode(sData.c_str(), (int)sData.length(), pRawData, nRawSize))
            return false;

        std::wstring sTmpFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSDirectory::GetTempPath(), L"ZIP");
        if (NSFile::CFileBinary::Exists(sTmpFile))
            NSFile::CFileBinary::Remove(sTmpFile);

        NSFile::CFileBinary oFile;
        oFile.CreateFileW(sTmpFile);
        oFile.WriteFile(pRawData, (DWORD)nRawSize);
        oFile.CloseFile();

        m_sTmpFolder = m_sWorkerFolder + L"/nativeZip";

        COfficeUtils oUtils;
        NSDirectory::CreateDirectory(m_sTmpFolder);
        if (S_OK != oUtils.ExtractToDirectory(sTmpFile, m_sTmpFolder, NULL, 0))
        {
            NSFile::CFileBinary::Remove(sTmpFile);
            return false;
        }

        NSFile::CFileBinary::Remove(sTmpFile);
        CheckDirectory();
        return true;
    }

    void CheckDirectory()
    {
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(m_sTmpFolder, true);

        url_correct2(m_sTmpFolder);
        int nStart = (int)m_sTmpFolder.length();
        for (std::vector<std::wstring>::iterator i = arFiles.begin(); i != arFiles.end(); i++)
        {
            std::wstring sTmp = *i;
            url_correct2(sTmp);

            m_arFiles.push_back(sTmp.substr(nStart + 1));
        }
    }

    void GetFileData(const std::wstring& strFile, BYTE*& pData, DWORD& dwLen)
    {
        NSFile::CFileBinary oFile;
        oFile.OpenFile(m_sTmpFolder + L"/" + strFile);
        dwLen = (DWORD)oFile.GetFileSize();
        pData = (BYTE*)malloc((size_t)dwLen);
        DWORD dwSizeRead = 0;
        oFile.ReadFile(pData, dwLen, dwSizeRead);
        oFile.CloseFile();
    }

private:

    void url_correct2(std::wstring& url)
    {
        NSCommon::string_replace(url, L"/./", L"/");

        size_t posn = 0;
        while (std::wstring::npos != (posn = url.find(L"/../")))
        {
            std::wstring::size_type pos2 = url.rfind(L"/", posn - 1);

            if (std::wstring::npos != pos2)
            {
                url.erase(pos2, posn - pos2 + 3);
            }
        }

        NSCommon::string_replace(url, L"\\\\", L"\\");
        NSCommon::string_replace(url, L"//", L"/");
        NSCommon::string_replace(url, L"\\", L"/");
    }
};

class CImagesWorker
{
private:
    std::wstring m_sFolder;

    int m_nIndex;
    std::map<std::wstring, std::wstring> m_mapImages;

public:
    CImagesWorker(const std::wstring& sFolder);
    std::wstring GetImageLocal(const std::wstring& sUrl);
    std::wstring GetImage(const std::wstring& sUrl);
};

class CNativeControl
{
private:
    std::wstring m_strFilePath;
    std::wstring m_strFileId;

public:
    CArray<std::wstring>* m_pChanges;

    std::wstring m_strFontsDirectory;

    std::map<std::wstring, std::wstring> m_map_fonts;
    std::wstring m_sDefaultFont;

    std::wstring m_strImagesDirectory;

    std::wstring m_strEditorType;

    int m_nCurrentChangesNumber;
    int m_nMaxChangesNumber;

    BYTE* m_pSaveBinary;
    int m_nSaveLen;

    int m_nSaveBinaryLen;
    std::string m_sHeader;

    std::map<std::wstring, bool> m_mapImagesInChanges;

    std::wstring m_sConsoleLogFile;

    std::wstring m_sChangesBuilderPath;
    int m_nCurrentChangesBuilderIndex;

    CZipWorker m_oZipWorker;

    // для добавления картинок -------------------------------------
    CImagesWorker* m_pWorker;

public:
    CMemoryStream* m_pStream;

    CNativeControl()
    {
        m_pStream = NULL;
        m_pChanges = NULL;

        m_nCurrentChangesNumber = -1;
        m_nMaxChangesNumber = -1;

        m_pSaveBinary = NULL;
        m_nSaveLen = 0;
        m_nSaveBinaryLen = 0;

        m_sConsoleLogFile = L"";

        m_nCurrentChangesBuilderIndex = 0;

        m_pWorker = NULL;
    }
    ~CNativeControl()
    {
        RELEASEOBJECT(m_pStream);
        m_pChanges = NULL;

        RELEASEARRAYOBJECTS(m_pSaveBinary);
        m_nSaveLen = 0;

        RELEASEOBJECT(m_pWorker);
    }

public:

    void Save_Alloc(int nLen)
    {
        m_nSaveLen = nLen;
        m_pSaveBinary = new BYTE[m_nSaveLen];
        memset(m_pSaveBinary, 0xFF, m_nSaveLen);
    }

    void Save_ReAlloc(int pos, int len)
    {
        BYTE* pOld = m_pSaveBinary;
        m_nSaveLen = len;
        m_pSaveBinary = new BYTE[m_nSaveLen];

        memcpy(m_pSaveBinary, pOld, pos);

        RELEASEARRAYOBJECTS(pOld);
    }

    void Save_End(const std::string& sHeader, int len)
    {
        m_sHeader = sHeader;
        m_nSaveBinaryLen = len;
    }

    void Save_Destroy()
    {
        RELEASEARRAYOBJECTS(m_pSaveBinary);
        m_nSaveLen = 0;
        m_nSaveBinaryLen = 0;
    }

public:
    void getFileData(const std::wstring& strFile, BYTE*& pData, DWORD& dwLen)
    {
        NSFile::CFileBinary oFile;
        oFile.OpenFile(strFile);
        dwLen = (DWORD)oFile.GetFileSize();
        pData = (BYTE*)malloc((size_t)dwLen);
        DWORD dwSizeRead = 0;
        oFile.ReadFile(pData, dwLen, dwSizeRead);
        oFile.CloseFile();
    }

    void SetFilePath(const std::wstring& strPath)
    {
        m_strFilePath = strPath;

        m_oZipWorker.m_sWorkerFolder = NSCommon::GetDirectoryName(strPath);
    }
    std::wstring GetFilePath()
    {
        return m_strFilePath;
    }

    void SetFileId(const std::wstring& strId)
    {
        m_strFileId = strId;
    }
    std::wstring GetFileId()
    {
        return m_strFileId;
    }

    void ConsoleLog(/*UTF8*/const std::string& strVal)
    {
#if 0
        if (!m_sConsoleLogFile.empty())
        {
            FILE* f = NSFile::CFileBinary::OpenFileNative(m_sConsoleLogFile, L"a+");
            fprintf(f, strVal.c_str());
            fprintf(f, "\n");
            fclose(f);
        }
#endif
        std::cout << strVal << std::endl;
    }

    void DumpRemoveChanges(const int& nDeleteIndex)
    {
        int nNaturalIndex = nDeleteIndex;

        // на каждое изменение две кавычки)
        nNaturalIndex <<= 1;

        // not cool realize
        BYTE* pData = NULL;
        DWORD dwSize = 0;
        bool bIsOk = NSFile::CFileBinary::ReadAllBytes(m_sChangesBuilderPath, &pData, dwSize);
        int nCounter = 0;

        int nSize = (int)dwSize;
        int nIndex = -1;
        for (int i = 0; i < nSize; i++)
        {
            if ('\"' == pData[i])
            {
                if (nCounter == nNaturalIndex)
                {
                    nIndex = i;
                    break;
                }
                ++nCounter;
            }
        }
        RELEASEARRAYOBJECTS(pData);

        if (-1 != nIndex)
        {
            NSFile::CFileBinary::Truncate(m_sChangesBuilderPath, nIndex);
        }
    }

    void DumpChanges(const std::string& sParam, int nDeleteIndex, int nCount)
    {
        if (nDeleteIndex < m_nCurrentChangesBuilderIndex)
        {
            // нужно удалить изменения
            DumpRemoveChanges(nDeleteIndex);
        }
        m_nCurrentChangesBuilderIndex = nDeleteIndex + nCount;

        if (nCount != 0)
        {
            FILE* _file = NSFile::CFileBinary::OpenFileNative(m_sChangesBuilderPath, L"a+");
            if (NULL != _file)
            {
                fprintf(_file, "\"");
                fprintf(_file, sParam.c_str());
                fprintf(_file, "\",");
                fclose(_file);
            }
        }
    }

    void CheckFonts()
    {
        if (0 == m_map_fonts.size())
        {
            NSFonts::IApplicationFonts* pApplication = NSFonts::NSApplication::Create();
            if (m_strFontsDirectory == L"")
                pApplication->Initialize();
            else
                pApplication->InitializeFromFolder(m_strFontsDirectory);

            std::vector<NSFonts::CFontInfo*>* pFonts = pApplication->GetList()->GetFonts();

            for (std::vector<NSFonts::CFontInfo*>::iterator i = pFonts->begin(); i < pFonts->end(); i++)
            {
                NSFonts::CFontInfo* pCurrent = *i;

                size_t pos1 = pCurrent->m_wsFontPath.find_last_of(wchar_t('/'));
                size_t pos2 = pCurrent->m_wsFontPath.find_last_of(wchar_t('\\'));

                size_t pos = std::wstring::npos;
                if (pos1 != std::wstring::npos)
                    pos = pos1;

                if (pos2 != std::wstring::npos)
                {
                    if (pos == std::wstring::npos)
                        pos = pos2;
                    else if (pos2 > pos)
                        pos = pos2;
                }

                if (pos != std::wstring::npos)
                {
                    m_map_fonts[pCurrent->m_wsFontPath.substr(pos + 1)] = pCurrent->m_wsFontPath;
                }
                else
                {
                    m_map_fonts[pCurrent->m_wsFontPath] = pCurrent->m_wsFontPath;
                }
            }

            NSFonts::CFontSelectFormat oFormat;
            oFormat.wsName = new std::wstring(L"Arial");

            m_sDefaultFont = L"";
            NSFonts::IFontManager* pManager = pApplication->GenerateFontManager();
            NSFonts::CFontInfo* pInfo = pManager->GetFontInfoByParams(oFormat);
            if (NULL != pInfo)
                m_sDefaultFont = pInfo->m_wsFontPath;

            RELEASEINTERFACE(pManager);
            RELEASEOBJECT(pApplication);
        }
    }
};

// wrap_methods -------------
CNativeControl* unwrap_nativeobject(v8::Handle<v8::Object> obj);

void _GetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args);
void _SetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args);
void _GetImagesPath(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetFontsDirectory(const v8::FunctionCallbackInfo<v8::Value>& args);
void _GetEditorType(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetChangesCount(const v8::FunctionCallbackInfo<v8::Value>& args);
void _GetChangesFile(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetFileId(const v8::FunctionCallbackInfo<v8::Value>& args);
void _SetFileId(const v8::FunctionCallbackInfo<v8::Value>& args);

void _CheckNextChange(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetFileArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetFontArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetFileString(const v8::FunctionCallbackInfo<v8::Value>& args);

void _Save_AllocNative(const v8::FunctionCallbackInfo<v8::Value>& args);

void _Save_ReAllocNative(const v8::FunctionCallbackInfo<v8::Value>& args);

void _Save_End(const v8::FunctionCallbackInfo<v8::Value>& args);

void _ConsoleLog(const v8::FunctionCallbackInfo<v8::Value>& args);

void _SaveChanges(const v8::FunctionCallbackInfo<v8::Value>& args);

void _GetImageUrl(const v8::FunctionCallbackInfo<v8::Value>& args);

/// ZIP -----
void _zipOpenFile(const v8::FunctionCallbackInfo<v8::Value>& args);
void _zipOpenFileBase64(const v8::FunctionCallbackInfo<v8::Value>& args);
void _zipGetFileAsString(const v8::FunctionCallbackInfo<v8::Value>& args);
void _zipGetFileAsBinary(const v8::FunctionCallbackInfo<v8::Value>& args);
void _zipCloseFile(const v8::FunctionCallbackInfo<v8::Value>& args);
/// ---------

void _AddImageInChanges(const v8::FunctionCallbackInfo<v8::Value>& args);

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplate(v8::Isolate* isolate);
v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplateBuilder(v8::Isolate* isolate);
// --------------------------

class CChangesWorker
{
private:
    BYTE*	m_pData;
    BYTE*	m_pDataCur;
    int		m_nLen;

    int		m_nMaxUnionSize = 100 * 1024 * 1024; // 100Mb
    v8::Local<v8::ArrayBuffer> m_oArrayBuffer;

    int		m_nFileType; // 0 - docx; 1 - excel

public:
    CChangesWorker()
    {
        m_pData = NULL;
        m_pDataCur = m_pData;
        m_nLen = 0;

        m_nFileType = 0;
    }
    ~CChangesWorker()
    {
        if (NULL != m_pData)
            delete[] m_pData;
    }

    void SetFormatChanges(const int& nFileType)
    {
        m_nFileType = nFileType;
    }

public:

    void CheckFiles(CArray<std::wstring>& oFiles)
    {
        int nMax = 0;
        int nLen = 0;

        int nCount = (int)oFiles.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);

            int nSize = (int)oFile.GetFileSize();

            if (nMax < nSize)
                nMax = nSize;

            nLen += nSize;
            oFile.CloseFile();
        }

        if (nLen <= m_nMaxUnionSize)
        {
            // все убралось - выделяем один кусок
            m_nLen = nLen + 4;
        }
        else
        {
            m_nLen = nMax + 4;
            if (m_nLen < m_nMaxUnionSize)
                m_nLen = m_nMaxUnionSize;
        }

        m_pData = new BYTE[m_nLen];
        m_pDataCur = m_pData;

        m_oArrayBuffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)m_pData, (size_t)m_nLen);
    }

    inline int Open(CArray<std::wstring>& oFiles, int nStart)
    {
        return Open_excel(oFiles, nStart);
    }

    int Open_docx(CArray<std::wstring>& oFiles, int nStart)
    {
        m_pDataCur = m_pData;
        m_pDataCur += 4;
        int nCountData = 0;

        int nCount = oFiles.GetCount();
        int nLenCurrect = 0;
        int i = nStart;
        for (; i < nCount; i++)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);

            int nLen = (int)oFile.GetFileSize();
            nLenCurrect += nLen;

            if (nLenCurrect > m_nLen)
                break;

            char* pData = new char[nLen];

            DWORD dwReadCount = 0;
            oFile.ReadFile((BYTE*)pData, nLen, dwReadCount);

            // parse data
            int nCur = 0;
            while (nCur < nLen)
            {
                // Id
                skip_name(pData, nCur, nLen);
                if (nCur >= nLen)
                    break;

                int nId = read_int(pData, nCur, nLen);
                *((int*)m_pDataCur) = nId;
                m_pDataCur += 4;

                // data
                skip_name(pData, nCur, nLen);
                skip_int2(pData, nCur, nLen);

                read_base64(pData, nCur, nLen);
                ++nCur;

                ++nCountData;
            }

            delete[]pData;
        }

        *((int*)m_pData) = nCountData;
        return i;
    }

    int Open_excel(CArray<std::wstring>& oFiles, int nStart)
    {
        m_pDataCur = m_pData;
        m_pDataCur += 4;
        int nCountData = 0;

        int nCount = oFiles.GetCount();
        int nLenCurrect = 0;
        int i = nStart;
        for (; i < nCount; i++)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);

            int nLen = (int)oFile.GetFileSize();
            nLenCurrect += nLen;

            if (nLenCurrect > m_nLen)
                break;

            char* pData = new char[nLen];

            DWORD dwReadCount = 0;
            oFile.ReadFile((BYTE*)pData, nLen, dwReadCount);

            // parse data
            int nCur = 0;
            while (nCur < nLen)
            {
                skip_int2(pData, nCur, nLen);

                if (nCur >= nLen)
                    break;

                read_base64(pData, nCur, nLen);
                ++nCur;

                ++nCountData;
            }

            delete[]pData;
        }

        *((int*)m_pData) = nCountData;
        return i;
    }

    v8::Local<v8::Uint8Array> GetData()
    {
        size_t len = (size_t)(m_pDataCur - m_pData);
        v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(m_oArrayBuffer, 0, len);
        return _array;
    }

public:
    void OpenFull(CArray<std::wstring>& oFiles)
    {
        // определяем размер
        int nCount = (int)oFiles.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);
            m_nLen += (int)oFile.GetFileSize();
            oFile.CloseFile();
        }

        m_pData = new BYTE[m_nLen];
        m_pDataCur = m_pData;

        m_pDataCur += 4;
        int nCountData = 0;

        for (int i = 0; i < nCount; i++)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);

            int nLen = (int)oFile.GetFileSize();
            char* pData = new char[nLen];

            DWORD dwReadCount = 0;
            oFile.ReadFile((BYTE*)pData, nLen, dwReadCount);

            // parse data
            int nCur = 0;
            while (nCur < nLen)
            {
                // Id
                skip_name(pData, nCur, nLen);
                if (nCur >= nLen)
                    break;

                int nId = read_int(pData, nCur, nLen);
                *((int*)m_pDataCur) = nId;
                m_pDataCur += 4;

                // data
                skip_name(pData, nCur, nLen);
                skip_int2(pData, nCur, nLen);

                read_base64(pData, nCur, nLen);
                ++nCur;

                ++nCountData;
            }

            delete[]pData;
        }

        *((int*)m_pData) = nCountData;
    }

    void OpenFull_excel(CArray<std::wstring>& oFiles)
    {
        // определяем размер
        int nCount = (int)oFiles.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);
            m_nLen += (int)oFile.GetFileSize();
            oFile.CloseFile();
        }

        m_pData = new BYTE[m_nLen];
        m_pDataCur = m_pData;

        m_pDataCur += 4;
        int nCountData = 0;

        for (int i = 0; i < nCount; i++)
        {
            NSFile::CFileBinary oFile;
            oFile.OpenFile(oFiles[i]);

            int nLen = (int)oFile.GetFileSize();
            char* pData = new char[nLen];

            DWORD dwReadCount = 0;
            oFile.ReadFile((BYTE*)pData, nLen, dwReadCount);

            // parse data
            int nCur = 0;
            while (nCur < nLen)
            {
                skip_int2(pData, nCur, nLen);

                if (nCur >= nLen)
                    break;

                read_base64(pData, nCur, nLen);
                ++nCur;

                ++nCountData;
            }

            delete[]pData;
        }

        *((int*)m_pData) = nCountData;
    }

    v8::Local<v8::Uint8Array> GetDataFull()
    {
        size_t len = (size_t)(m_pDataCur - m_pData);
        v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)m_pData, len);
        v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, len);
        return _array;
    }

    int OpenNative(std::wstring strFile)
    {
        NSFile::CFileBinary oFile;
        oFile.OpenFile(strFile);

        int nLen = (int)oFile.GetFileSize();
        char* pData = new char[nLen];

        DWORD dwReadCount = 0;
        oFile.ReadFile((BYTE*)pData, nLen, dwReadCount);

        int nCur = 0;

        // DOCY
        skip_int2(pData, nCur, nLen);
        // v
        skip_no_digit(pData, nCur, nLen);
        int nVersion = read_int2(pData, nCur, nLen);

        m_nLen = read_int2(pData, nCur, nLen);

        if (nVersion < 10)
        {
            m_pData = new BYTE[m_nLen];
            m_pDataCur = m_pData;
            read_base64_2(pData, nCur, nLen);

            delete[]pData;
        }
        else
        {
            m_nLen = nLen;
            m_pData = (BYTE*)pData;
            m_pDataCur = m_pData + m_nLen;
        }

        return nVersion;
    }

    inline void skip_name(const char* data, int& cur, const int& len)
    {
        int nCount = 0;
        while (cur < len)
        {
            if (data[cur] == '\"')
                ++nCount;

            ++cur;

            if (3 == nCount)
                break;
        }
    }

    inline int read_int(const char* data, int& cur, const int& len)
    {
        int res = 0;
        while (cur < len)
        {
            if (data[cur] == '\"')
            {
                ++cur;
                break;
            }

            res *= 10;
            res += (data[cur++] - '0');
        }

        return res;
    }
    inline void skip_int2(const char* data, int& cur, const int& len)
    {
        while (cur < len)
        {
            if (data[cur++] == ';')
                break;
        }
    }
    inline int read_int2(const char* data, int& cur, const int& len)
    {
        int res = 0;
        while (cur < len)
        {
            if (data[cur] == ';')
            {
                ++cur;
                break;
            }

            res *= 10;
            res += (data[cur++] - '0');
        }

        return res;
    }
    inline void skip_no_digit(const char* data, int& cur, const int& len)
    {
        while (cur < len)
        {
            char _c = data[cur];
            if (_c >= '0' && _c <= '9')
                break;
            ++cur;
        }
    }

    inline void read_base64(const char* data, int& cur, const int& len)
    {
        Base64Decode(data, cur, len);
    }
    inline void read_base64_2(const char* data, int& cur, const int& len)
    {
        Base64Decode2(data, cur, len);
    }

private:
    inline int DecodeBase64Char(unsigned int ch)
    {
        // returns -1 if the character is invalid
        // or should be skipped
        // otherwise, returns the 6-bit code for the character
        // from the encoding table
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 0;	// 0 range starts at 'A'
        if (ch >= 'a' && ch <= 'z')
            return ch - 'a' + 26;	// 26 range starts at 'a'
        if (ch >= '0' && ch <= '9')
            return ch - '0' + 52;	// 52 range starts at '0'
        if (ch == '+')
            return 62;
        if (ch == '/')
            return 63;
        return -1;
    }

    void Base64Decode(const char* data, int& cur, const int& len)
    {
        // walk the source buffer
        // each four character sequence is converted to 3 bytes
        // CRLFs and =, and any characters not in the encoding table
        // are skiped

        BYTE* pDataLen = m_pDataCur;
        m_pDataCur += 4;

        int nWritten = 0;
        while (cur < len && data[cur] != '\"')
        {
            DWORD dwCurr = 0;
            int i;
            int nBits = 0;
            for (i = 0; i<4; i++)
            {
                if (data[cur] == '\"')
                    break;
                int nCh = DecodeBase64Char(data[cur++]);
                if (nCh == -1)
                {
                    // skip this char
                    i--;
                    continue;
                }
                dwCurr <<= 6;
                dwCurr |= nCh;
                nBits += 6;
            }

            // dwCurr has the 3 bytes to write to the output buffer
            // left to right
            dwCurr <<= 24 - nBits;
            for (i = 0; i<nBits / 8; i++)
            {
                *m_pDataCur = (BYTE)((dwCurr & 0x00ff0000) >> 16);
                ++m_pDataCur;

                dwCurr <<= 8;
                nWritten++;
            }

        }

        *((int*)pDataLen) = nWritten;
    }

    void Base64Decode2(const char* data, int& cur, const int& len)
    {
        // walk the source buffer
        // each four character sequence is converted to 3 bytes
        // CRLFs and =, and any characters not in the encoding table
        // are skiped

        int nWritten = 0;
        while (cur < len && data[cur] != '\"')
        {
            DWORD dwCurr = 0;
            int i;
            int nBits = 0;
            for (i = 0; i<4; i++)
            {
                if (cur >= len)
                    break;
                int nCh = DecodeBase64Char(data[cur++]);
                if (nCh == -1)
                {
                    // skip this char
                    i--;
                    continue;
                }
                dwCurr <<= 6;
                dwCurr |= nCh;
                nBits += 6;
            }

            // dwCurr has the 3 bytes to write to the output buffer
            // left to right
            dwCurr <<= 24 - nBits;
            for (i = 0; i<nBits / 8; i++)
            {
                *m_pDataCur = (BYTE)((dwCurr & 0x00ff0000) >> 16);
                ++m_pDataCur;

                dwCurr <<= 8;
                nWritten++;
            }

        }
    }
};

//////////////////////////////////////////////////////////////////////////////

void CreateNativeObject(const v8::FunctionCallbackInfo<v8::Value>& args);
void CreateNativeObjectBuilder(const v8::FunctionCallbackInfo<v8::Value>& args);
void CreateNativeMemoryStream(const v8::FunctionCallbackInfo<v8::Value>& args);

#if 0
class CLoggerSpeed
{
public:
    DWORD m_dwTime;

public:
    CLoggerSpeed()
    {
        m_dwTime = NSTimers::GetTickCount();
    }
    void Lap(const std::string& details)
    {
        DWORD dwCur = NSTimers::GetTickCount();
        FILE* f = fopen("D:\\doctrenderer.speed", "a+");
        std::string sTmp = details + ": %d\n";
        fprintf(f, sTmp.c_str(), (int)(dwCur - m_dwTime));
        fclose(f);
        m_dwTime = dwCur;
    }
};

#define LOGGER_SPEED_START CLoggerSpeed __logger_speed;
#define LOGGER_SPEED_LAP(__logger_param) __logger_speed.Lap(#__logger_param);
#else
#define LOGGER_SPEED_START
#define LOGGER_SPEED_LAP(__logger_param)
#endif

//////////////////////////////////////////////////////////////////////////////
#if 0
class CSnapshotScript
{
public:
    bool m_bIsExist;
    v8::StartupData m_oStartupData;

    CSnapshotScript(const std::wstring& sDir = L"")
    {
        m_bIsExist = false;
        m_oStartupData.data = NULL;
        m_oStartupData.raw_size = 0;

        std::wstring sFile = sDir + L"/heap_snapshot.bin";
        if (NSFile::CFileBinary::Exists(sFile))
        {
            m_bIsExist = true;

            NSFile::CFileBinary oFile;
            oFile.OpenFile(sFile);
            m_oStartupData.raw_size = (int)oFile.GetFileSize();
            m_oStartupData.data = new char[m_oStartupData.raw_size];

            DWORD dwRead = 0;
            oFile.ReadFile((BYTE*)m_oStartupData.data, (DWORD)m_oStartupData.raw_size, dwRead);
            oFile.CloseFile();
        }
    }

    bool Generate(const std::string& sScript)
    {
        m_oStartupData = {NULL, 0};
        {
            v8::SnapshotCreator snapshot_creator;
            // Obtain an isolate provided by SnapshotCreator.
            v8::Isolate* isolate = snapshot_creator.GetIsolate();
            {
                v8::HandleScope scope(isolate);
                // Create a new context and optionally run some script.
                v8::Local<v8::Context> context = v8::Context::New(isolate);
                //v8::Context::Scope context_scope(context);

                if (!RunExtraCode(isolate, context, sScript.c_str(), "<embedded>"))
                    return false;

                // Add the possibly customized context to the SnapshotCreator.
                snapshot_creator.SetDefaultContext(context);
            }
            // Use the SnapshotCreator to create the snapshot blob.
            m_oStartupData = snapshot_creator.CreateBlob(v8::SnapshotCreator::FunctionCodeHandling::kClear);
        }
        return true;
    }

    void Save(const std::wstring& sDir)
    {
        if (m_oStartupData.data == NULL)
            return;

        std::wstring sFile = sDir + L"/heap_snapshot.bin";
        NSFile::CFileBinary oFile;
        oFile.CreateFile(sFile);
        oFile.WriteFile((BYTE*)m_oStartupData.data, (DWORD)m_oStartupData.raw_size);
        oFile.CloseFile();
    }

    bool RunExtraCode(v8::Isolate* isolate, v8::Local<v8::Context> context, const char* utf8_source, const char* name)
    {
        // Run custom script if provided.
        v8::TryCatch try_catch(isolate);

        v8::Local<v8::String> source_string;
        if (!v8::String::NewFromUtf8(isolate, utf8_source, v8::NewStringType::kNormal).ToLocal(&source_string))
            return false;

        v8::Local<v8::String> resource_name = v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked();

        v8::ScriptOrigin origin(resource_name);
        v8::ScriptCompiler::Source source(source_string, origin);
        v8::Local<v8::Script> script;

        bool bRet = v8::ScriptCompiler::Compile(context, &source).ToLocal(&script);

        if (try_catch.HasCaught())
        {
            std::string strCode        = to_cstringA(try_catch.Message()->GetSourceLine());
            std::string strException   = to_cstringA(try_catch.Message()->Get());
            return false;
        }

        script->Run();

        if (try_catch.HasCaught())
        {
            std::string strCode        = to_cstringA(try_catch.Message()->GetSourceLine());
            std::string strException   = to_cstringA(try_catch.Message()->Get());
            return false;
        }
        return true;
    }
};
#endif

#ifdef V8_OS_XP

class ExternalMallocArrayBufferAllocator : public v8::ArrayBuffer::Allocator
{
public:
    virtual void* Allocate(size_t length)
    {
        void* ret = malloc(length);
        memset(ret, 0, length);
        return ret;
    }
    virtual void* AllocateUninitialized(size_t length)
    {
        return malloc(length);
    }
    virtual void Free(void* data, size_t length)
    {
        free(data);
    }
};

#endif

//////////////////////////////////////////////////////////////////////////////
class CV8Initializer
{
private:
    v8::Platform* m_platform;
    v8::ArrayBuffer::Allocator* m_pAllocator;

public:
    CV8Initializer()
    {
        std::wstring sPrW = NSFile::GetProcessPath();
        std::string sPrA = U_TO_UTF8(sPrW);

        m_pAllocator = NULL;

#ifndef V8_OS_XP
        v8::V8::InitializeICUDefaultLocation(sPrA.c_str());
        v8::V8::InitializeExternalStartupData(sPrA.c_str());
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);
        v8::V8::Initialize();
#else
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);

        v8::V8::Initialize();
        v8::V8::InitializeICU();
#endif
    }
    ~CV8Initializer()
    {
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete m_platform;
        if (m_pAllocator)
            delete m_pAllocator;
    }

    v8::ArrayBuffer::Allocator* getAllocator()
    {
        return m_pAllocator;
    }

    v8::Isolate* CreateNew()
    {
        v8::Isolate::CreateParams create_params;
#ifndef V8_OS_XP
        m_pAllocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
#else
        m_pAllocator = new ExternalMallocArrayBufferAllocator();
#endif
        create_params.array_buffer_allocator = m_pAllocator;
        return v8::Isolate::New(create_params);
    }
};

class CV8Worker
{
private:
    static CV8Initializer* m_pInitializer;

public:
    CV8Worker() {}
    ~CV8Worker() {}

    static void Initialize();
    static void Dispose();

    static CV8Initializer* getInitializer();
};

bool Doct_renderer_SaveFile_ForBuilder(int nFormat, const std::wstring& strDstFile,
                               CNativeControl* pNative,
                               v8::Isolate* isolate,
                               v8::Local<v8::Context> context,
                               v8::Local<v8::Object>& global_js,
                               v8::Handle<v8::Value>* args,
                               v8::TryCatch& try_catch,
                               std::wstring& strError);

class CCacheDataScript
{
private:
    BYTE* Data;
    int Length;

    v8::ScriptCompiler::Source* Source;
    v8::ScriptCompiler::CachedData* CachedData;

    std::wstring Path;

public:
    CCacheDataScript(const std::wstring& sPath)
    {
        Data = NULL;
        Length = 0;

        if (!sPath.empty())
        {
            BYTE* _data = NULL;
            DWORD _data_length = 0;
            if (NSFile::CFileBinary::ReadAllBytes(sPath, &_data, _data_length))
            {
                Data = _data;
                Length = (int)_data_length;
            }
        }

        Source      = NULL;
        CachedData  = NULL;
        Path        = sPath;
    }
    ~CCacheDataScript()
    {
        //RELEASEOBJECT(Source);
        //RELEASEOBJECT(CachedData);
        RELEASEARRAYOBJECTS(Data);
    }

    v8::Local<v8::Script> Compile(const v8::Local<v8::Context>& _context, const v8::Local<v8::String>& source)
    {
        v8::Local<v8::Script> script;
        if (NULL == Data)
        {
            Source = new v8::ScriptCompiler::Source(source);
            script = v8::ScriptCompiler::Compile(_context, Source, v8::ScriptCompiler::kProduceCodeCache).ToLocalChecked();

            const v8::ScriptCompiler::CachedData* _cachedData = Source->GetCachedData();
            NSFile::CFileBinary oFileTest;
            if (oFileTest.CreateFileW(Path))
            {
                oFileTest.WriteFile(_cachedData->data, (DWORD)_cachedData->length);
                oFileTest.CloseFile();
            }
        }
        else
        {
            CachedData = new v8::ScriptCompiler::CachedData(Data, Length);
            Source = new v8::ScriptCompiler::Source(source, CachedData);
            script = v8::ScriptCompiler::Compile(_context, Source, v8::ScriptCompiler::kConsumeCodeCache).ToLocalChecked();
        }
        return script;
    }
};

#endif // NATIVECONTROL
