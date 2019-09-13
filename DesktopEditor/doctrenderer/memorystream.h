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
#ifndef MEMORYSTREAM
#define MEMORYSTREAM

#include <string>
#include "../common/Types.h"
#include "../common/File.h"

#include "v8.h"
#include "libplatform/libplatform.h"

#if V8_MAJOR_VERSION < 7
#define ISOLATE_IF_7
#else
#define ISOLATE_IF_7 v8::Isolate::GetCurrent(),
#endif

class CV8Convert
{
public:
	static int ToInt(const v8::Local<v8::Value>& v)
	{
		return v->ToInt32(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Int32>())->Value();
	}
	static unsigned int ToUint(const v8::Local<v8::Value>& v)
	{
		return v->ToUint32(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Uint32>())->Value();
	}
	static double ToDouble(const v8::Local<v8::Value>& v)
	{
		return v->ToNumber(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Number>())->Value();
	}
	static bool ToBool(const v8::Local<v8::Value>& v)
	{
		return v->ToBoolean(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Boolean>())->Value();
	}
	static std::wstring ToString(v8::Local<v8::Value> v)
	{
	    v8::String::Utf8Value data(ISOLATE_IF_7 v);
	    if (NULL == *data)
		return L"";

	    return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(*data), data.length());
	}
	static std::string ToStringA(v8::Local<v8::Value> v)
	{
	    v8::String::Utf8Value data(ISOLATE_IF_7 v);
	    const char* p = (char*)*data;
	    if (NULL == p)
		return "";
	    return std::string(p);
	}
	static std::wstring GetSourceLine(const v8::Local<v8::Message>& m)
	{
		return ToString(m->GetSourceLine(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
	}
	static std::wstring GetMessage(const v8::Local<v8::Message>& m)
	{
		return ToString(m->Get());
	}
};

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

