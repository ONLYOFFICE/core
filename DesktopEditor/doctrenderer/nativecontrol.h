#ifndef NATIVECONTROL
#define NATIVECONTROL

#include "memorystream.h"
#include <map>
#include "../fontengine/ApplicationFonts.h"
#include <iostream>

// string convert
std::wstring to_cstring(v8::Local<v8::Value> v);
std::string to_cstringA(v8::Local<v8::Value> v);

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
    }
    ~CNativeControl()
    {
        RELEASEOBJECT(m_pStream);
        m_pChanges = NULL;

        RELEASEARRAYOBJECTS(m_pSaveBinary);
        m_nSaveLen = 0;
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
            CApplicationFonts oApplication;
            if (m_strFontsDirectory == L"")
                oApplication.Initialize();
            else
                oApplication.InitializeFromFolder(m_strFontsDirectory);

            CArray<CFontInfo*>* pFonts = oApplication.GetList()->GetFonts();

            int nCount = pFonts->GetCount();
            for (int nIndex = 0; nIndex < nCount; ++nIndex)
            {
                CFontInfo* pCurrent = pFonts->operator [](nIndex);

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

            CFontSelectFormat oFormat;
            oFormat.wsName = new std::wstring(L"Arial");

            m_sDefaultFont = L"";
            CFontInfo* pInfo = oApplication.GenerateFontManager()->GetFontInfoByParams(oFormat);
            if (NULL != pInfo)
                m_sDefaultFont = pInfo->m_wsFontPath;
        }
    }
};

// wrap_methods -------------
CNativeControl* unwrap_nativeobject(v8::Handle<v8::Object> obj);

void _GetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args);
void _SetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args);

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

void _AddImageInChanges(const v8::FunctionCallbackInfo<v8::Value>& args);

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplate(v8::Isolate* isolate);
v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplateBuilder(v8::Isolate* isolate);
// --------------------------

// create work with arraytypes
class MallocArrayBufferAllocator : public v8::ArrayBuffer::Allocator
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

        m_pData = new BYTE[m_nLen];
        m_pDataCur = m_pData;

        read_base64_2(pData, nCur, nLen);

        delete[]pData;

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

//////////////////////////////////////////////////////////////////////////////
class CV8Initializer
{
private:
    v8::Platform* m_platform;
    MallocArrayBufferAllocator m_oAllocator;

public:
    CV8Initializer() : m_oAllocator()
    {
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);

        v8::V8::Initialize();
        v8::V8::InitializeICU();

#ifndef NEW_V8_ENGINE
        v8::V8::SetArrayBufferAllocator(&m_oAllocator);
#endif
    }
    ~CV8Initializer()
    {
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete m_platform;
    }

    v8::ArrayBuffer::Allocator* getAllocator()
    {
        return &m_oAllocator;
    }

    v8::Isolate* CreateNew()
    {
#ifdef NEW_V8_ENGINE
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = &m_oAllocator;
        return v8::Isolate::New(create_params);
#else
        return v8::Isolate::New();
#endif
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

#endif // NATIVECONTROL
