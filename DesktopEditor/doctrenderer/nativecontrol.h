#ifndef NATIVECONTROL
#define NATIVECONTROL

#include "memorystream.h"
#include <map>
#include "../fontengine/ApplicationFonts.h"

// string convert
static std::wstring to_cstring(v8::Local<v8::Value> v)
{
    v8::String::Utf8Value data(v);
    if (NULL == *data)
        return L"";

    return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(*data), data.length());
}

static std::string to_cstringA(v8::Local<v8::Value> v)
{
    v8::String::Utf8Value data(v);
    const char* p = (char*)*data;
    if (NULL == p)
        return "";
    return std::string(p);
}

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
#if 1
        FILE* f = fopen("C:/log.txt", "a+");
        fprintf(f, strVal.c_str());
        fprintf(f, "\n");
        fclose(f);
#endif
    }

    void CheckSystemFonts()
    {
        if (L"" == m_strFontsDirectory && 0 == m_map_fonts.size())
        {
            CApplicationFonts oApplication;
            oApplication.Initialize();
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
CNativeControl* unwrap_nativeobject(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CNativeControl*>(field->Value());
}

void _GetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->GetFilePath());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}
void _SetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->SetFilePath(to_cstring(args[0]));
}

void _GetFontsDirectory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_strFontsDirectory);
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}

void _GetEditorType(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_strEditorType);
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}

void _GetChangesCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    int nCount = 0;
    if (pNative->m_pChanges != NULL)
        nCount = (int)pNative->m_pChanges->GetCount();
    args.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), nCount));
}
void _GetChangesFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    if (args.Length() < 1)
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    v8::Local<v8::Int32> intValue = args[0]->ToInt32();
    int nIndex = (int)intValue->Value();

    std::string strFile = "";
    if (pNative->m_pChanges != NULL)
        strFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_pChanges->operator []((int)nIndex));

    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), strFile.c_str()));
}

void _GetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->GetFileId());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}
void _SetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->SetFileId(to_cstring(args[0]));
}

void _CheckNextChange(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    pNative->m_nCurrentChangesNumber++;
    if (-1 != pNative->m_nMaxChangesNumber)
    {
        if (pNative->m_nCurrentChangesNumber >= pNative->m_nMaxChangesNumber)
        {
            args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), false));
            return;
        }
    }
    args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), true));
}

void _GetFileArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->getFileData(to_cstring(args[0]), pData, len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)len);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

    args.GetReturnValue().Set(_array);
}

void _GetFontArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    std::wstring strDir = pNative->m_strFontsDirectory;

    if (strDir.length() != 0)
    {
        strDir += L"/";
        strDir += to_cstring(args[0]);
    }
    else
    {
        std::map<std::wstring, std::wstring>::iterator pair = pNative->m_map_fonts.find(to_cstring(args[0]));
        if (pair == pNative->m_map_fonts.end())
            strDir = pair->second;
        else
            strDir = pNative->m_sDefaultFont;
    }

    pNative->getFileData(strDir, pData, len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)len);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

    args.GetReturnValue().Set(_array);
}

void _GetFileString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->getFileData(to_cstring(args[0]), pData, len);

    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (char*)pData, v8::String::kNormalString, len));
}

void _Save_AllocNative(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    v8::Local<v8::Int32> intValue = args[0]->ToInt32();
    int nLen = (int)intValue->Value();

    pNative->Save_Alloc(nLen);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pNative->m_pSaveBinary, (size_t)pNative->m_nSaveLen);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)pNative->m_nSaveLen);
    args.GetReturnValue().Set(_array);
}

void _Save_ReAllocNative(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    int _pos = args[0]->Int32Value();
    int _len = args[1]->Int32Value();

    pNative->Save_ReAlloc(_pos, _len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pNative->m_pSaveBinary, (size_t)pNative->m_nSaveLen);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)pNative->m_nSaveLen);
    args.GetReturnValue().Set(_array);
}

void _Save_End(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sHeader = to_cstringA(args[0]);
    int _len = args[1]->Int32Value();

    pNative->Save_End(sHeader, _len);
}

void _ConsoleLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->ConsoleLog(to_cstringA(args[0]));
}

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplate(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // прописываем функции - методы объекта
    result->Set(v8::String::NewFromUtf8(current, "SetFilePath"), v8::FunctionTemplate::New(current, _SetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "GetFilePath"), v8::FunctionTemplate::New(current, _GetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "SetFileId"), v8::FunctionTemplate::New(current, _SetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileId"), v8::FunctionTemplate::New(current, _GetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileBinary"), v8::FunctionTemplate::New(current, _GetFileArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontBinary"), v8::FunctionTemplate::New(current, _GetFontArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontsDirectory"), v8::FunctionTemplate::New(current, _GetFontsDirectory));
    result->Set(v8::String::NewFromUtf8(current, "GetFileString"), v8::FunctionTemplate::New(current, _GetFileString));

    result->Set(v8::String::NewFromUtf8(current, "GetEditorType"), v8::FunctionTemplate::New(current, _GetEditorType));
    result->Set(v8::String::NewFromUtf8(current, "CheckNextChange"), v8::FunctionTemplate::New(current, _CheckNextChange));

    result->Set(v8::String::NewFromUtf8(current, "GetCountChanges"), v8::FunctionTemplate::New(current, _GetChangesCount));
    result->Set(v8::String::NewFromUtf8(current, "GetChangesFile"), v8::FunctionTemplate::New(current, _GetChangesFile));

    result->Set(v8::String::NewFromUtf8(current, "Save_AllocNative"), v8::FunctionTemplate::New(current, _Save_AllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_ReAllocNative"), v8::FunctionTemplate::New(current, _Save_ReAllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_End"), v8::FunctionTemplate::New(current, _Save_End));

    result->Set(v8::String::NewFromUtf8(current, "ConsoleLog"), v8::FunctionTemplate::New(current, _ConsoleLog));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}
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

static void enableTypedArrays()
{
    v8::V8::SetArrayBufferAllocator(new MallocArrayBufferAllocator());
}

enum CommandType
{
    ctPenXML						= 0,
    ctPenColor						= 1,
    ctPenAlpha						= 2,
    ctPenSize						= 3,
    ctPenDashStyle					= 4,
    ctPenLineStartCap				= 5,
    ctPenLineEndCap				    = 6,
    ctPenLineJoin					= 7,
    ctPenDashPatern				    = 8,
    ctPenDashPatternCount			= 9,
    ctPenDashOffset				    = 10,
    ctPenAlign						= 11,
    ctPenMiterLimit				    = 12,

    // brush
    ctBrushXML						= 20,
    ctBrushType					    = 21,
    ctBrushColor1					= 22,
    ctBrushColor2					= 23,
    ctBrushAlpha1					= 24,
    ctBrushAlpha2					= 25,
    ctBrushTexturePath				= 26,
    ctBrushTextureAlpha			    = 27,
    ctBrushTextureMode				= 28,
    ctBrushRectable				    = 29,
    ctBrushRectableEnabled 		    = 30,
    ctBrushGradient                 = 31,

    // font
    ctFontXML						= 40,
    ctFontName						= 41,
    ctFontSize						= 42,
    ctFontStyle					    = 43,
    ctFontPath						= 44,
    ctFontGID						= 45,
    ctFontCharSpace				    = 46,

    // shadow
    ctShadowXML					    = 50,
    ctShadowVisible				    = 51,
    ctShadowDistanceX				= 52,
    ctShadowDistanceY				= 53,
    ctShadowBlurSize				= 54,
    ctShadowColor					= 55,
    ctShadowAlpha					= 56,

    // edge
    ctEdgeXML						= 70,
    ctEdgeVisible					= 71,
    ctEdgeDistance					= 72,
    ctEdgeColor					    = 73,
    ctEdgeAlpha					    = 74,

    // text
    ctDrawText						= 80,
    ctDrawTextEx					= 81,

    // pathcommands
    ctPathCommandMoveTo			    = 91,
    ctPathCommandLineTo			    = 92,
    ctPathCommandLinesTo			= 93,
    ctPathCommandCurveTo			= 94,
    ctPathCommandCurvesTo			= 95,
    ctPathCommandArcTo		        = 96,
    ctPathCommandClose				= 97,
    ctPathCommandEnd				= 98,
    ctDrawPath						= 99,
    ctPathCommandStart				= 100,
    ctPathCommandGetCurrentPoint	= 101,
    ctPathCommandText				= 102,
    ctPathCommandTextEx			    = 103,

    // image
    ctDrawImage					    = 110,
    ctDrawImageFromFile			    = 111,

    ctSetParams					    = 120,

    ctBeginCommand					= 121,
    ctEndCommand					= 122,

    ctSetTransform					= 130,
    ctResetTransform				= 131,

    ctClipMode						= 140,

    ctCommandLong1					= 150,
    ctCommandDouble1				= 151,
    ctCommandString1				= 152,
    ctCommandLong2					= 153,
    ctCommandDouble2				= 154,
    ctCommandString2				= 155,

    ctPageWidth                     = 200,
    ctPageHeight                    = 201,

    ctPageStart                     = 202,
    ctPageEnd                       = 203,

    ctError						    = 255
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


#endif // NATIVECONTROL

