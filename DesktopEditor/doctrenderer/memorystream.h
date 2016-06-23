#ifndef MEMORYSTREAM
#define MEMORYSTREAM

#include <string>
#include "../common/Types.h"
#include "../common/File.h"

#include "v8.h"
#include "libplatform/libplatform.h"

class CMemoryStream
{
private:
    // сам метафайл
    BYTE* m_pBuffer;
    BYTE* m_pBufferMem;

    size_t m_lSize;

public:
    CMemoryStream()
    {
        m_pBuffer		= NULL;
        m_pBufferMem	= NULL;

        m_lSize			= 0;
    }
    ~CMemoryStream()
    {
        RELEASEARRAYOBJECTS(m_pBuffer);
    }

    inline BYTE* GetData()
    {
        return m_pBuffer;
    }
    inline int GetSize()
    {
        return (int)(m_pBufferMem - m_pBuffer);
    }

    inline void Clear()
    {
        m_lSize		= 0;

        m_pBuffer		= NULL;
        m_pBufferMem	= NULL;
    }

    inline void ClearNoAttack()
    {
        m_pBufferMem = m_pBuffer;
    }

    inline void Copy(const CMemoryStream* pData, const size_t& nPos, const size_t& nLen)
    {
        CheckBufferSize(nLen);
        memcpy(m_pBufferMem, pData->m_pBuffer + nPos, nLen);
        m_pBufferMem += nLen;
    }

    inline void CheckBufferSize(size_t lPlus)
    {
        if (NULL != m_pBuffer)
        {
            size_t nNewSize = (m_pBufferMem - m_pBuffer) + lPlus;
            if (nNewSize >= m_lSize)
            {
                while (nNewSize >= m_lSize)
                {
                    m_lSize *= 2;
                }

                BYTE* pNew = new BYTE[m_lSize];
                memcpy(pNew, m_pBuffer, m_pBufferMem - m_pBuffer);

                m_pBufferMem	= pNew + (m_pBufferMem - m_pBuffer);

                RELEASEARRAYOBJECTS(m_pBuffer);
                m_pBuffer		= pNew;
            }
        }
        else
        {
            m_lSize			= 1000;
            m_pBuffer		= new BYTE[m_lSize];
            m_pBufferMem	= m_pBuffer;

            CheckBufferSize(lPlus);
        }
    }

    inline void WriteBYTE(const BYTE& lValue)
    {
        CheckBufferSize(sizeof(BYTE));
        *m_pBufferMem = lValue;
        m_pBufferMem += sizeof(BYTE);
    }
    inline void WriteLONG(const LONG& lValue)
    {
        CheckBufferSize(sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = lValue;
        m_pBufferMem += sizeof(LONG);
    }
    inline void WriteDouble(const double& dValue)
    {
        CheckBufferSize(sizeof(double));
        *((double*)(m_pBufferMem)) = dValue;
        m_pBufferMem += sizeof(double);
    }
    inline void WriteStringA(const char* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(USHORT));
        *((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
        m_pBufferMem += sizeof(USHORT);

        memcpy(m_pBufferMem, pData, nLen);
        m_pBufferMem += nLen;
    }
    inline void WriteStringA2(const char* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = (LONG)nLen;
        m_pBufferMem += sizeof(LONG);

        memcpy(m_pBufferMem, pData, nLen);
        m_pBufferMem += nLen;
    }

    inline void WriteString(const wchar_t* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(USHORT));
        *((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
        m_pBufferMem += sizeof(USHORT);

        int nLen2 = nLen << 1;
        memcpy(m_pBufferMem, pData, nLen2);
        m_pBufferMem += nLen2;
    }
    inline void WriteString2(const wchar_t* pData, int nLen)
    {
        int nLen2 = nLen << 1;

        CheckBufferSize(nLen2 + sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = (LONG)nLen2;
        m_pBufferMem += sizeof(LONG);

        memcpy(m_pBufferMem, pData, nLen2);
        m_pBufferMem += nLen2;
    }
};

// wrap_methods -------------
CMemoryStream* unwrap_memorystream(v8::Handle<v8::Object> obj);

void _ms_write_byte(const v8::FunctionCallbackInfo<v8::Value>& args);
void _ms_write_bool(const v8::FunctionCallbackInfo<v8::Value>& args);
void _ms_write_long(const v8::FunctionCallbackInfo<v8::Value>& args);
void _ms_write_double(const v8::FunctionCallbackInfo<v8::Value>& args);

void _ms_writestring1(const v8::FunctionCallbackInfo<v8::Value>& args);
void _ms_writestring2(const v8::FunctionCallbackInfo<v8::Value>& args);

void _ms_copy(const v8::FunctionCallbackInfo<v8::Value>& args);

void _ms_clearnoattack(const v8::FunctionCallbackInfo<v8::Value>& args);

void _ms_pos(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

v8::Handle<v8::ObjectTemplate> CreateMemoryStreamTemplate(v8::Isolate* isolate);

#endif // MEMORYSTREAM

