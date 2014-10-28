#pragma once
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

namespace NSPresentationEditor
{
    static std::wstring	g_bstr_nodeopen				= L"<";
    static std::wstring	g_bstr_nodeclose			= L">";
    static std::wstring	g_bstr_nodeopen_slash		= L"</";
    static std::wstring	g_bstr_nodeclose_slash		= L"/>";
    static std::wstring	g_bstr_node_space			= L" ";
    static std::wstring	g_bstr_node_equal			= L"=";
    static std::wstring	g_bstr_node_quote			= L"\"";
    static std::wstring	g_bstr_boolean_true			= L"true";
    static std::wstring	g_bstr_boolean_false		= L"false";
	
	inline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	inline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}

	class CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		~CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

        AVSINLINE void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
                m_lSize = (std::max)(nSize, (size_t) 1000);
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
        AVSINLINE void WriteString(const wchar_t* pString, size_t& nLen)
		{
			AddSize(nLen);
			// memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			// wstring has 4 bytes length (not 2 in WIN32) in linux/macos
			memcpy(m_pDataCur, pString, (2 == sizeof (wchar_t)) ? (nLen << 1) : (nLen << 2));
			
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
#ifdef _WIN32
        AVSINLINE void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			WriteString(bsString.GetBSTR(), nLen);
		}
#endif // #ifdef _WIN32
        AVSINLINE void WriteString(std::wstring& wString)
		{
			size_t nLen = wString.length();
			WriteString(wString.c_str(), nLen);
		}
        AVSINLINE void WriteString(const CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();
			
			#ifdef _UNICODE
			CString str = sString;
			WriteString(str.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}

        AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

        AVSINLINE void Write(CStringWriter& oWriter)
		{
			WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}

		inline void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		inline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		CString GetData()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}

		void CorrectUnicodeString()
		{
			BYTE m_mapUnicode[0x10000];
			memset(m_mapUnicode, 0, 0x10000);

			m_mapUnicode[0x0009] = 1;
			m_mapUnicode[0x000A] = 1;
			m_mapUnicode[0x000D] = 1;

			memset(m_mapUnicode + 0x0020, 1, 0xD7FF - 0x0020 + 1);
			memset(m_mapUnicode + 0xE000, 1, 0xFFFD - 0xE000 + 1);

			wchar_t* pMemory = m_pData;
			while (pMemory < m_pDataCur)
			{
				if (!m_mapUnicode[*pMemory])
					*pMemory = wchar_t(' ');
				++pMemory;
			}
		}
	};

	class CXmlWriter
	{
		CStringWriter m_oWriter;
	
	public:

		CXmlWriter() : m_oWriter()
		{
		}
		
		inline CString GetXmlString()
		{
			return m_oWriter.GetData();
		}

		// write value
        AVSINLINE void WriteString(CString strValue)
		{
			m_oWriter.WriteString(strValue);
		}
        AVSINLINE void WriteDouble(const double& val)
		{
			CString str = _T("");
			str.Format(_T("%lf"), val);
			m_oWriter.WriteString(str);
		}
        AVSINLINE void WriteLONG(const long& val)
		{
			CString str = _T("");
			str.Format(_T("%d"), val);
			m_oWriter.WriteString(str);
		}
        AVSINLINE void WriteDWORD(const DWORD& val)
		{
			CString str = _T("");
			str.Format(_T("%u"), val);
			m_oWriter.WriteString(str);
		}
        AVSINLINE void WriteDWORD_hex(const DWORD& val)
		{
			CString str = _T("");
			str.Format(_T("%x"), val);
			m_oWriter.WriteString(str);
		}
		// write attribute
        AVSINLINE void WriteAttributeString(const CString& strAttributeName, const CString& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			m_oWriter.WriteString(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
        AVSINLINE void WriteAttributeDouble(const CString& strAttributeName, const double& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDouble(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
        AVSINLINE void WriteAttributeLONG(const CString& strAttributeName, const LONG& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteLONG(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
        AVSINLINE void WriteAttributeDWORD(const CString& strAttributeName, const DWORD& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDWORD(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
        AVSINLINE void WriteAttributeDWORD_hex(const CString& strAttributeName, const DWORD& val)
		{
			m_oWriter.WriteString(g_bstr_node_space);
			m_oWriter.WriteString(strAttributeName);
			m_oWriter.WriteString(g_bstr_node_equal);
			m_oWriter.WriteString(g_bstr_node_quote);
			WriteDWORD_hex(val);
			m_oWriter.WriteString(g_bstr_node_quote);
		}
		// document methods
        AVSINLINE void WriteNodeBegin(CString strNodeName, BOOL bAttributed = FALSE)
		{
			m_oWriter.WriteString(g_bstr_nodeopen);
			m_oWriter.WriteString(strNodeName);
			
			if (!bAttributed)
				m_oWriter.WriteString(g_bstr_nodeclose);
		}
        AVSINLINE void WriteNodeEnd(CString strNodeName, BOOL bEmptyNode = FALSE, BOOL bEndNode = TRUE)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_oWriter.WriteString(g_bstr_nodeclose_slash);
				else
					m_oWriter.WriteString(g_bstr_nodeclose);
			}
			else
			{
				m_oWriter.WriteString(g_bstr_nodeopen_slash);
				m_oWriter.WriteString(strNodeName);
				m_oWriter.WriteString(g_bstr_nodeclose);
			}
		}
		// write node values
        AVSINLINE void WriteNodeValueString(const CString& strNodeName, const CString& val)
		{
			WriteNodeBegin(strNodeName);
			WriteString(val);
			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValueBool(const CString& strNodeName, const bool& val)
		{
			WriteNodeBegin(strNodeName);
			
			if (val)
				WriteString(_T("1"));
			else
				WriteString(_T("0"));

			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValueDouble(const CString& strNodeName, const double& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDouble(val);
			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValueLONG(const CString& strNodeName, const LONG& val)
		{
			WriteNodeBegin(strNodeName);
			WriteLONG(val);
			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValueDWORD(const CString& strNodeName, const DWORD& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDWORD(val);
			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValueDWORD_hex(const CString& strNodeName, const DWORD& val)
		{
			WriteNodeBegin(strNodeName);
			WriteDWORD_hex(val);
			WriteNodeEnd(strNodeName);
		}

		
		BOOL SaveToFile(CString strFilePath, BOOL bEncodingToUTF8 = FALSE)
		{
			CString strData = m_oWriter.GetData();
			if (!bEncodingToUTF8)
			{
				CFile oFile;
				oFile.CreateFile(strFilePath);
				oFile.WriteFile((void*)strData.GetBuffer(), strData.GetLength());
				oFile.CloseFile();
			}
			else
			{
				CDirectory::SaveToFile(strFilePath, strData);
			}
			
			return TRUE;
		}
	};
}
