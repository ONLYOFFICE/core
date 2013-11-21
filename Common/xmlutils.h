#pragma once

#import <msxml3.dll> rename_namespace("XML")
#include <atlcoll.h>
#include <atlenc.h>

namespace XmlUtils
{
	// common
	static int GetDigit(TCHAR c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	static int GetColor(CString string)
	{
		// variables
		int blue = 0;
		int green = 0;
		int red = 0;

		CString color = string; color = color.Trim();
				
		if (color.Find(_T("0x"))!=-1)
			color.Delete(0,2);
		if (color.Find(_T("#"))!=-1)
			color.Delete(0,1);

		while (color.GetLength() < 6)
			color = _T("0") + color;

		red = 16*GetDigit(color[0]) + GetDigit(color[1]);
		green = 16*GetDigit(color[2]) + GetDigit(color[3]);
		blue = 16*GetDigit(color[4]) + GetDigit(color[5]);

		return RGB(red, green, blue);
	}
	static BOOL GetBoolean(CString string)
	{
		CString s = string; s.MakeLower();

		return (s == _T("true"));
	}
	static int GetInteger(CString string)
	{
		return _ttoi(string);
	}
	static double GetDouble(CString string)
	{
		double d = 0;

		_stscanf(string, _T(" %lf"), &d);

		return d;
	}
	static CString GetLower(CString string)
	{
		CString s = string; s.MakeLower();

		return s;
	}
		
	static CString GetNodeAttrib(XML::IXMLDOMNodePtr node, CString attrib, CString def = _T(""))
	{
		try
		{
			if (node == NULL || attrib.GetLength() < 1)
				return def;

			_bstr_t x = attrib;

			if (node->attributes == NULL || node->attributes->getNamedItem(x) == NULL)
				return def;

			CString s; s = node->attributes->getNamedItem(x)->text.GetBSTR();

			return s;
		}
		catch (...)
		{
		}

		return def;
	}

	static CString GetNodeAttribExt(XML::IXMLDOMNodePtr node, CString attrib, CString def = _T(""))
	{
		try
		{
			if (node == NULL || attrib.GetLength() < 1)
				return def;

			_bstr_t x = attrib;

			if (node->attributes == NULL || node->attributes->getNamedItem(x) == NULL)
				return def;

			VARIANT var;
			node->attributes->getNamedItem(x)->get_nodeValue(&var);

			if (VT_BSTR == var.vt)
			{
				CString s = (CString)var.bstrVal;
				VariantClear(&var);
				return s;
			}

			VariantClear(&var);
		}
		catch (...)
		{
		}

		return def;
	}

	static CString GetNodeText(XML::IXMLDOMNodePtr node, CString def = _T(""))
	{
		try
		{
			if (node == NULL)
				return def;

			CString s; s = node->text.GetBSTR();

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeXml(XML::IXMLDOMNodePtr node, CString subnode, CString def = _T(""))
	{
		try
		{
			if (node == NULL)
				return def;

			CString s; 

			if (subnode.GetLength() < 1)
				s = node->xml.GetBSTR();
			else
			{
				_bstr_t x = subnode;
				s = node->selectSingleNode(x)->xml.GetBSTR();
			}

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValue(XML::IXMLDOMNodePtr node, CString subnode, CString def = _T(""))
	{
		try
		{
			if (node == NULL || subnode.GetLength() < 1)
				return def;

			_bstr_t x = subnode;

			if (node->selectSingleNode(x) == NULL)
				return def;

			CString s; s = node->selectSingleNode(x)->text.GetBSTR();

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValueExt(XML::IXMLDOMNodePtr ptrNode, const CString &sDef = _T(""))
	{
		try
		{
			if (ptrNode == NULL)
				return sDef;

			CString sPath = _T("./text()");

			XML::IXMLDOMNodeList* pChildTextNodeList = NULL;
			BSTR bsPath = sPath.AllocSysString();
			ptrNode->raw_selectNodes(bsPath, &pChildTextNodeList);
			SysFreeString(bsPath);

			if (NULL!=pChildTextNodeList)
			{
				CString sRet; 
				long lCount = 0;
				pChildTextNodeList->get_length(&lCount);
				for (long lIndex = 0; lIndex < lCount; lIndex++)
				{
					XML::IXMLDOMNode *pChildTextNode = NULL;
					pChildTextNodeList->get_item(lIndex, &pChildTextNode);
					if (NULL==pChildTextNode)
						continue;
					VARIANT vt;
					pChildTextNode->get_nodeValue(&vt);
					pChildTextNode->Release();
					CString sValue; 
					if (VT_BSTR == vt.vt)
						sValue = vt.bstrVal;
					VariantClear(&vt);
					sRet += sValue;
				}
				pChildTextNodeList->Release();
				return sRet;
			}
		}
		catch (...)
		{
		}

		return sDef;
	}
	static BOOL GetNodeTextTyped(XML::IXMLDOMNodePtr node, _variant_t* variant, CString type = _T("bin.base64"))
	{
		try
		{
			if (node == NULL)
				return FALSE;

			BSTR bstrType = type.AllocSysString();

			node->put_dataType(bstrType);

			SysFreeString(bstrType);

			*variant = node->nodeTypedValue;

			return TRUE;
		}
		catch (...)
		{
		}

		return FALSE;
	}
		
	static CString GetNodeAttrib(XML::IXMLDOMNodeListPtr nodes, int index, CString attrib, CString def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeAttrib(nodes->item[index], attrib, def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeText(XML::IXMLDOMNodeListPtr nodes, int index, CString def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeText(nodes->item[index], def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeXml(XML::IXMLDOMNodeListPtr nodes, int index, CString def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeXml(nodes->item[index], def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static CString GetNodeValue(XML::IXMLDOMNodeListPtr nodes, int index, CString subnode, CString def = _T(""))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return def;

			CString s = GetNodeValue(nodes->item[index], subnode, def);

			return s;
		}
		catch (...)
		{
		}

		return def;
	}
	static BOOL GetNodeTextTyped(XML::IXMLDOMNodeListPtr nodes, int index, _variant_t* variant, CString type = _T("bin.base64"))
	{
		try
		{
			if (nodes == NULL || index < 0 || index >= nodes->length)
				return FALSE;

			return GetNodeTextTyped(nodes->item[index], variant, type);
		}
		catch (...)
		{
		}

		return FALSE;
	}
	
	// classes
	class CXmlWriter
	{
		CString m_str;
	
	public:

		CXmlWriter()
		{
			m_str.Empty();
		}
		
		CString GetXmlString()
		{
			return m_str;
		}
		void SetXmlString(CString strValue)
		{
			m_str = strValue;
		}
		
		BOOL SaveToFile(CString strFilePath, BOOL bEncodingToUTF8 = FALSE)
		{
			FILE* pFile = _tfopen(strFilePath, _T("wt"));

			if (!pFile)
				return FALSE;

			CStringA str; str = m_str;
			if (bEncodingToUTF8)
#ifdef _UNICODE
				str = EncodingUnicodeToUTF8();
#else
				str = EncodingASCIIToUTF8();
#endif

			fprintf(pFile, str);

			fclose(pFile);

			return TRUE;
		}
#ifdef _UNICODE
        CStringA EncodingUnicodeToUTF8()
        {
            int nLength = m_str.GetLength();

			// Encoding Unicode to UTF-8
			CStringA saStr; 
			WideCharToMultiByte(CP_UTF8, 0, m_str.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
			saStr.ReleaseBuffer();    
			return saStr;
        }
#else
		CString EncodingASCIIToUTF8()
        {
            int nLength = m_str.GetLength();

			wchar_t* pWStr = new wchar_t[nLength + 1];
			if (!pWStr)
				return _T("");

			// set end string
			pWStr[nLength] = 0;

			// Encoding ASCII to Unicode
            MultiByteToWideChar(CP_ACP, 0, m_str, nLength, pWStr, nLength);

            int nLengthW = (int) wcslen(pWStr);

			// Encoding Unicode to UTF-8
			CString cStr; 
            WideCharToMultiByte(CP_UTF8, 0, pWStr, nLengthW + 1, cStr.GetBuffer(nLengthW*3 + 1), nLengthW*3, NULL, NULL);
			cStr.ReleaseBuffer();
    
		    delete[] pWStr;

            return cStr;
        }
#endif


		void WriteString(CString strValue)
		{
			m_str += strValue;
		}
		void WriteInteger(int Value, int Base = 10)
		{
			char str[33];
			
			_itoa(Value, str, Base);

			m_str += str;
		}
		void WriteDouble(double Value)
		{
			CString str;
			
			str.Format(_T("%lf"), Value);

			m_str += str;
		}
		void WriteBoolean(BOOL Value)
		{
			if (Value)
				m_str += _T("true");
			else
				m_str += _T("false");
		}
		void WriteNodeBegin(CString strNodeName, BOOL bAttributed = FALSE)
		{
			m_str += _T("<") + strNodeName;

			if (!bAttributed)
				m_str += _T(">");
		}
		void WriteNodeEnd(CString strNodeName, BOOL bEmptyNode = FALSE, BOOL bEndNode = TRUE)
		{
			if (bEmptyNode)
			{
				if (bEndNode)
					m_str += _T(" />");
				else
					m_str += _T(">");
			}
			else
				m_str += _T("</") + strNodeName + _T(">");
		}
		void WriteNode(const CString &strNodeName, const CString &strNodeValue)
		{
			if (strNodeValue.GetLength() == 0)
				m_str += _T("<") + strNodeName + _T("/>");
			else
				m_str += _T("<") + strNodeName + _T(">") + strNodeValue + _T("</") + strNodeName + _T(">");
		}
		void WriteNode(CString strNodeName, int nValue, int nBase = 10, CString strTextBeforeValue = _T(""), CString strTextAfterValue = _T(""))
		{
			WriteNodeBegin(strNodeName);
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteNode(CString strNodeName, double dValue)
		{
			WriteNodeBegin(strNodeName);
			WriteDouble(dValue);
			WriteNodeEnd(strNodeName);
		}
		void WriteAttribute(CString strAttributeName, CString strAttributeValue)
		{
			m_str += _T(" ") + strAttributeName + _T("=\"") + strAttributeValue + _T("\"");
		}
		void WriteAttribute(CString strAttributeName, int nValue, int nBase = 10, CString strTextBeforeValue = _T(""), CString strTextAfterValue = _T(""))
		{
			WriteString(_T(" ") + strAttributeName + _T("="));
			WriteString(_T("\""));
			WriteString(strTextBeforeValue);
			WriteInteger(nValue, nBase);
			WriteString(strTextAfterValue);
			WriteString(_T("\""));
		}
		void WriteAttribute(CString strAttributeName, double dValue)
		{
			WriteString(_T(" ") + strAttributeName + _T("="));
			WriteString(_T("\""));
			WriteDouble(dValue);
			WriteString(_T("\""));
		}
	public:
		static void ReplaceSpecialCharacters (CString &sString)
		{
			sString.Replace (_T("&"), _T("&amp;"));
			sString.Replace (_T("<"), _T("&lt;"));
			sString.Replace (_T(">"), _T("&gt;"));
			sString.Replace (_T("\""), _T("&quot;"));
			sString.Replace (_T("'"), _T("&apos;"));
		}
	};
	class CXmlReader
	{
		CString m_str;
		XML::IXMLDOMDocumentPtr	m_pXmlDocument;
		XML::IXMLDOMNodeListPtr m_pXmlNodeList;

	protected:

		XML::IXMLDOMNodePtr m_pXmlNode;

	public:

		CXmlReader()
		{
			Clear();

			m_pXmlDocument = NULL;

			m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
		}
		CXmlReader(BOOL bCreateDocument)
		{
			Clear();

			m_pXmlDocument = NULL;

			if (bCreateDocument)
				m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
		}
		
		void Clear()
		{
			m_str.Empty();

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;
		}
		CString GetXmlString()
		{
			return m_str;
		}
		BOOL SetXmlString(CString strValue)
		{
			if (NULL == m_pXmlDocument)
				return FALSE;

			try
			{
				_bstr_t bstrXml; bstrXml = strValue;

				if (_T("") != strValue && VARIANT_TRUE == m_pXmlDocument->loadXML(bstrXml))
				{
					if (ReadRootNode())
					{
						m_str = strValue;

						return TRUE;
					}
				}
			}
			catch (...)
			{
				
			}

			Clear();

			return FALSE;
		}
	
		BOOL OpenFromXmlString(CString strXml)
		{
			return SetXmlString(strXml);
		}
		BOOL OpenFromXmlNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			m_pXmlNode = pXmlNode;

			if (pXmlNode == NULL)
				return FALSE;

			return TRUE;
		}
		BOOL OpenFromFile(CString strFilePath)
		{
			if (NULL == m_pXmlDocument)
				return FALSE;

			BSTR bstrFilePath = strFilePath.AllocSysString();
			BOOL bSuccess = FALSE;

			try
			{
				if (VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
					bSuccess = ReadRootNode();
			}
			catch (...)
			{
			}

			if (!bSuccess)
				Clear();

			SysFreeString(bstrFilePath);

			return bSuccess;
		}
	
		CString ReadNodeName()
		{
			if (NULL == m_pXmlNode)
				return _T("");

			CString sName;
			try
			{
				BSTR bsName;
				m_pXmlNode->get_nodeName(&bsName);
				sName = bsName;
				SysFreeString(bsName);
			}
			catch (...)
			{
			}			
			return sName;
		}
		BOOL ReadRootNode()
		{
			if (NULL == m_pXmlDocument)
				return FALSE;

			try
			{
				m_pXmlNode = m_pXmlDocument->firstChild;

				return TRUE;
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;

			return FALSE;
		}
		BOOL ReadRootNode(CString strRootNodeName)
		{
			if (_T("") == strRootNodeName)
				return ReadRootNode();

			if (NULL == m_pXmlDocument)
				return FALSE;

			try
			{
				_bstr_t bstrNode; bstrNode = strRootNodeName;

				m_pXmlNode = m_pXmlDocument->selectSingleNode(bstrNode);

				return TRUE;
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;
			m_pXmlNodeList = NULL;

			return FALSE;
		}
		BOOL ReadNode(CString strSubNodeName)
		{
			if (NULL == m_pXmlNode)
				return FALSE;

			try
			{
				_bstr_t bstrNode; bstrNode = strSubNodeName;

				XML::IXMLDOMElementPtr pNewNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL != pNewNode)
				{
					m_pXmlNode = pNewNode;
					return TRUE;
				}
			}
			catch (...)
			{
			}

			m_pXmlNode = NULL;

			return FALSE;
		}
		BOOL ReadNodeList(CString strSubNodesName)
		{
			if (NULL == m_pXmlNode)
				return FALSE;

			try
			{
				_bstr_t bstrNodes; bstrNodes = strSubNodesName;

				XML::IXMLDOMNodeListPtr pNewNodeList = m_pXmlNode->selectNodes(bstrNodes);

				if (NULL != pNewNodeList)
				{
					m_pXmlNodeList = pNewNodeList;
					return TRUE;
				}
			}
			catch (...)
			{
			}

			m_pXmlNodeList = NULL;

			return FALSE;
		}
	
		CString ReadNodeAttributeOrValue(CString strSubNodeName, CString def = _T(""))
		{
			CString strInvalidValue = _T("x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = ReadNodeAttribute(strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CString strNodeValue = ReadNodeValue(strSubNodeName, strInvalidValue);

			if (strNodeValue != strInvalidValue)
				return strNodeValue;

			return def;
		}
		CString ReadNodeValue(CString strSubNodeName, CString def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeValue(m_pXmlNode, strSubNodeName, def);
		}
		CString ReadNodeAttribute(CString strAttributeName, CString def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeAttrib(m_pXmlNode, strAttributeName, def);
		}
		CString ReadNodeText(CString def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeText(m_pXmlNode, def);
		}
		CString ReadNodeXml(CString def = _T(""))
		{
			if (NULL == m_pXmlNode)
				return def;

			return GetNodeXml(m_pXmlNode, def);
		}
		BOOL ReadNodeTextTyped(_variant_t* variant, CString type = _T("bin.base64"))
		{
			if (NULL == m_pXmlNode)
				return FALSE;

			return GetNodeTextTyped(m_pXmlNode, variant, type);
		}
		
		CString ReadNodeName(int nIndex)
		{
			if (NULL == m_pXmlNodeList)
				return _T("");

			CString sName;
			try
			{
				BSTR bsName;
				m_pXmlNodeList->item[nIndex]->get_nodeName(&bsName);
				sName = bsName;
				SysFreeString(bsName);
			}
			catch (...)
			{
			}			
			return sName;
		}
		CString ReadNodeAttributeOrValue(int nIndex, CString strSubNodeName, CString def = _T(""))
		{
			CString strInvalidValue = _T("dx(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = ReadNodeAttribute(nIndex, strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CString strNodeValue = ReadNodeValue(nIndex, strSubNodeName, strInvalidValue);

			if (strNodeValue != strInvalidValue)
				return strNodeValue;

			return def;
		}
		CString ReadNodeValue(int nIndex, CString strSubNodeName, CString def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeValue(m_pXmlNodeList, nIndex, strSubNodeName, def);
		}
		CString ReadNodeAttribute(int nIndex, CString strAttributeName, CString def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeAttrib(m_pXmlNodeList, nIndex, strAttributeName, def);
		}
		CString ReadNodeText(int nIndex, CString def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeText(m_pXmlNodeList, nIndex, def);
		}
		CString ReadNodeXml(int nIndex, CString def = _T(""))
		{
			if (NULL == m_pXmlNodeList)
				return def;

			return GetNodeXml(m_pXmlNodeList, nIndex, def);
		}
		BOOL ReadNodeTextTyped(int nIndex, _variant_t* variant, CString type = _T("bin.base64"))
		{
			if (NULL == m_pXmlNodeList)
				return FALSE;

			return GetNodeTextTyped(m_pXmlNodeList, nIndex, variant, type);
		}
		
		int GetLengthList()
		{
			try
			{
				if (NULL == m_pXmlNodeList)
					return 0;

				int length = m_pXmlNodeList->length;

				return length;
			}
			catch (...)
			{
			}

			return 0;
		}
		BOOL GetNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			pXmlNode = m_pXmlNode;

			return (m_pXmlNode != NULL);
		}
		BOOL GetNode(int nIndex, XML::IXMLDOMNodePtr& pXmlNode)
		{
			if (m_pXmlNodeList == NULL)
				return FALSE;

			pXmlNode = m_pXmlNodeList->item[nIndex];

			return TRUE;
		}
	};
};
	
//Расширение для сохранения в XML свойств интерфейса
namespace XmlUtils
{
	static LPCTSTR g_cpszSADescriptorNodeName	= _T("SADescriptor");
	static LPCTSTR g_cpszSADimsAttributeName	= _T("SADims");
	static LPCTSTR g_cpszSASizeNodeNameFormat	= _T("Size%d");
	static LPCTSTR g_cpszSADataNodeName			= _T("SAData");
	static LPCTSTR g_cpszVariantTypeNodeAttr	= _T("type");
	
	struct SDispatchProperty
	{
		SDispatchProperty()
			: m_sName(_T(""))
			//, m_lDISPID(0)
		{
		}
		SDispatchProperty(const CString &sName, const VARIANT &var)
			: m_sName(sName)
			, m_varValue(var)
		{
			
		}
		CString m_sName;
		//DISPID m_lDISPID;
		VARIANT m_varValue;
	};
	
	class CDispatchPropertyList 
		: public CAtlArray<SDispatchProperty>
	{
	public:
		CDispatchPropertyList()
		{
		}
		virtual ~CDispatchPropertyList()
		{
			size_t nCount = GetCount();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++)
			{
				if ((VT_BSTR==GetAt(nIndex).m_varValue.vt)&&(NULL!=GetAt(nIndex).m_varValue.bstrVal))
				{
					SysFreeString(GetAt(nIndex).m_varValue.bstrVal);
				}
				else if ((0!=(VT_ARRAY & GetAt(nIndex).m_varValue.vt))&&(NULL!=GetAt(nIndex).m_varValue.parray))
					SafeArrayDestroy(GetAt(nIndex).m_varValue.parray);
			}
		}
	};

	//Только свойства которые не имеют дополнительных параметров на вход
	static BOOL GetPropertyList(IDispatch *pDisp, CDispatchPropertyList &arProp, INVOKEKIND nType)
	{
		if (NULL==pDisp)
			return FALSE;
		ITypeInfo *pTypeInfo = NULL;
		pDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &pTypeInfo);
		if (NULL==pTypeInfo)
			return FALSE;
		
		TYPEATTR* pTypeAttr = NULL;
		HRESULT hRes = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if (S_OK!=hRes)
		{
			pTypeInfo->Release();
			return FALSE;
		}
		int nMethodCount = pTypeAttr->cFuncs;
		for (int i=0; i<nMethodCount; i++)
		{
			SDispatchProperty oPropRec;
			FUNCDESC* pFuncDesc;
			hRes = pTypeInfo->GetFuncDesc(i, &pFuncDesc); 
			if (S_OK!=hRes)
				continue;

			if (0!=pFuncDesc->cParams)
				continue;//СОХРАНЯЕМ ТОЛЬКО ТЕ СВОЙСТВА У КОТОРЫХ НЕТ ПАРАМЕТРОВ
			if (nType!=pFuncDesc->invkind)
				continue;

//				oPropRec.m_lDISPID = pFuncDesc->memid;
			BSTR bsName; 
			unsigned int nCount;
			hRes = pTypeInfo->GetNames(pFuncDesc->memid, &bsName, 1, &nCount); 
			if (S_OK!=hRes)
				continue;

			oPropRec.m_sName = bsName; 
			SysFreeString(bsName);

			{
				DISPPARAMS dispParam;
				dispParam.cArgs = 0;
				dispParam.cNamedArgs = 0;
				dispParam.rgdispidNamedArgs = NULL;
				dispParam.rgvarg = NULL;

				hRes = pDisp->Invoke(pFuncDesc->memid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, 
					&dispParam, &oPropRec.m_varValue, NULL, NULL);
			}
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			arProp.Add(oPropRec);
		}			
		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeInfo->Release();		
		return TRUE;
	}
	static BOOL ApplyPropertyList(IDispatch *pDisp, CDispatchPropertyList &arProp)
	{
		if (NULL==pDisp)
			return FALSE;
		
		long lCount = (long)arProp.GetCount();
		for (long i=0; i<lCount; i++)
		{
			DISPID dispid;
			CStringW swName; swName = arProp[i].m_sName; 
			OLECHAR FAR* szMember = swName.GetBuffer();
			pDisp->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

			DISPPARAMS dispParam;
			DISPID dispidNamed = DISPID_PROPERTYPUT;
			dispParam.cArgs = 1;
			dispParam.cNamedArgs = 1;
			dispParam.rgdispidNamedArgs = &dispidNamed;			
			dispParam.rgvarg = &arProp[i].m_varValue;

			HRESULT hr = pDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, 
				&dispParam, NULL, NULL, NULL);

		}
		return TRUE;
	}

	class CDispatchXmlWriter 
		: public CXmlWriter
	{
	public:
		CDispatchXmlWriter()
		{
		}
		~CDispatchXmlWriter()
		{
		}
		void WriteNode(const CString &sName, const CLSID &clsid)
		{
			CStringW swGUID;
			int nSize = StringFromGUID2(clsid, swGUID.GetBuffer(50), 50);
			swGUID.ReleaseBuffer(nSize);
			CString sGUID; sGUID = swGUID;
			CXmlWriter::WriteNode(sName,sGUID);
		}
		void WriteNode(LPSAFEARRAY psa)
		{
			if (NULL==psa)
				return;
			UINT nDim = psa->cDims;
			if (0==nDim)
				return;
			ULONG ulSADataSize = 1;
			for (UINT nIndex = 0; nIndex < nDim; nIndex++)
				ulSADataSize *= psa->rgsabound[nIndex].cElements;
			if (0==ulSADataSize)
				return;

			CString sTemp;
			CXmlWriter::WriteNodeBegin(g_cpszSADescriptorNodeName, TRUE);
			CXmlWriter::WriteAttribute(g_cpszSADimsAttributeName, (int)nDim);
			CXmlWriter::WriteString(_T(">"));
			for (UINT nIndex = 0; nIndex < nDim; nIndex++)
			{
				sTemp.Format(g_cpszSASizeNodeNameFormat,nIndex);
				CXmlWriter::WriteNode(sTemp, (long)(psa->rgsabound[nIndex].cElements));
			}
			CXmlWriter::WriteNodeEnd(g_cpszSADescriptorNodeName, FALSE);
			if (BinaryToBase64((LPBYTE)psa->pvData, ulSADataSize, sTemp))			
				CXmlWriter::WriteNode(g_cpszSADataNodeName, sTemp);
			else
				CXmlWriter::WriteNode(g_cpszSADataNodeName, _T(""));
		}
		void WriteNode(const CString &sName, const VARIANT &val)
		{
			if ((0!=(VT_VECTOR & val.vt)) || (0!=(VT_BYREF & val.vt)))
				return;
			if ((0!=(VT_ARRAY & val.vt))&&(VT_UI1 != (VT_TYPEMASK & val.vt)))
				return;

			CXmlWriter::WriteNodeBegin(sName, TRUE);
			CXmlWriter::WriteAttribute(g_cpszVariantTypeNodeAttr, val.vt);
			CXmlWriter::WriteString(_T(">"));
			if (0!=(VT_ARRAY & val.vt))
				WriteNode(val.parray);
			else
				WriteString(VariantToString(val));
			CXmlWriter::WriteNodeEnd(sName, FALSE);
		}
		void WritePropertyList(CDispatchPropertyList &arProp)
		{
			size_t nCount = arProp.GetCount();
			for (size_t nIndex = 0; nIndex<nCount; nIndex++)
			{
				const SDispatchProperty &oPropRec = arProp[nIndex];
				WriteNode(oPropRec.m_sName, oPropRec.m_varValue);
			}
		}
	protected:
		static BOOL BinaryToBase64(const BYTE* pData, long lSize, CString& sResult)
		{
			if ((NULL==pData) || (0==lSize))
				return FALSE;

			int nStrSize = Base64EncodeGetRequiredLength(lSize);

			CStringA saTemp;
			LPSTR pStrData = saTemp.GetBuffer(nStrSize + 1);
			BOOL bSuccess = Base64Encode(pData, lSize, pStrData, &nStrSize);
			
			pStrData[nStrSize] = '\0';
			saTemp.ReleaseBuffer();
			
			sResult = saTemp;
			return bSuccess;
		}

		static CString VariantToString(VARIANT val)
		{
			CString sVal = _T("");
			switch (VT_TYPEMASK & val.vt)
			{
			case VT_EMPTY:
				break;
			case VT_NULL:
				sVal = _T("");
				break;
			case VT_I2:
				sVal.Format(_T("%d"), val.iVal);
				break;
			case VT_I4:
				sVal.Format(_T("%d"), val.lVal);
				break;
			case VT_R4:
				sVal.Format(_T("%f"), val.fltVal);
				break;
			case VT_R8:
				sVal.Format(_T("%f"), val.dblVal);
				break;
			case VT_DATE:
				sVal.Format(_T("%f"), val.date);
				break;
			case VT_BSTR:
				sVal = val.bstrVal;
				break;
			case VT_BOOL:
				sVal = (VARIANT_TRUE == val.boolVal) ? _T("1") : _T("0");
				break;
			case VT_VARIANT:
				sVal = _T("");
				break;
			case VT_UNKNOWN:
				sVal = _T("");
				//можно сохранять пытаясь получить IDispatch интерфейс, но вроде нам он не надо
				break;
			case VT_I1:
				sVal.Format(_T("%d"), val.cVal);
				break;
			case VT_UI1:
				sVal.Format(_T("%d"), val.bVal);
				break;
			case VT_UI2:
				sVal.Format(_T("%d"), val.uiVal);
				break;
			case VT_UI4:
				sVal.Format(_T("%d"), val.ulVal);
				break;
			case VT_I8:
				sVal.Format(_T("%d"), val.llVal);
				break;
			case VT_UI8:
				sVal.Format(_T("%d"), val.ullVal);
				break;
			case VT_INT:
				sVal.Format(_T("%d"), val.intVal);
				break;
			case VT_UINT:
				sVal.Format(_T("%d"), val.uintVal);
				break;
			case VT_DECIMAL:
			case VT_CY:
			case VT_SAFEARRAY:
			case VT_VOID:
			case VT_HRESULT:
			case VT_CARRAY:
			case VT_USERDEFINED:
			case VT_LPSTR:
			case VT_LPWSTR:
			case VT_RECORD:
			case VT_INT_PTR:
			case VT_UINT_PTR:
			case VT_FILETIME:
			case VT_BLOB:
			case VT_STREAM:
			case VT_STORAGE:
			case VT_STREAMED_OBJECT:
			case VT_STORED_OBJECT:
			case VT_BLOB_OBJECT:
			case VT_CF:
			case VT_CLSID:
			case VT_VERSIONED_STREAM:
			case VT_BSTR_BLOB:
				sVal = _T("");
				break;
			}

			return sVal;
		}

	};
	class CDispatchXmlReader 
		: public XmlUtils::CXmlReader
	{
	public:
		CDispatchXmlReader()
		{
		}
		~CDispatchXmlReader()
		{
		}
		size_t GetPropertyList(const CString &strSubNodeName, CDispatchPropertyList &arProp)
		{
			if (NULL == m_pXmlNode)
				return 0;

			try
			{
				_bstr_t bstrNode; bstrNode = strSubNodeName;
				XML::IXMLDOMNodePtr pNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL==pNode)
					return 0;
				XML::IXMLDOMNodeListPtr pNodeList = pNode->selectNodes("*");

				if (NULL != pNodeList)
				{
					long lNodeCount = pNodeList->length;
					CString sNodeName, sType, sValue;
					VARIANT val;
					for (long lIndex = 0; lIndex<lNodeCount; lIndex++)
					{
						XML::IXMLDOMNodePtr ptrNode = pNodeList->item[lIndex];
						sNodeName = ptrNode->baseName.GetBSTR();

						sType = XmlUtils::GetNodeAttrib(ptrNode, g_cpszVariantTypeNodeAttr);
						sValue = ptrNode->text.GetBSTR();
						val.vt = _ttoi(sType);
						if ((0!=(VT_VECTOR & val.vt)) || (0!=(VT_BYREF & val.vt)))
							continue;
						if (0!=(VT_ARRAY & val.vt))
						{
							if (VT_UI1 != (VT_TYPEMASK & val.vt))
								continue;
							if (!GetSafeArray(ptrNode, val.parray))
								continue;
						}						
						else
						{
							if (!GetVariantValue(sType, sValue, val))
								continue;
						}
						arProp.Add(SDispatchProperty(sNodeName, val));						
					}					
				}
			}
			catch (...)
			{
			}
			return (arProp.GetCount());
		}
	protected:
		BOOL GetSafeArray(XML::IXMLDOMNodePtr ptrNode, LPSAFEARRAY &psa)
		{
			long lDims = 0;
			LPSAFEARRAYBOUND psabound = NULL;
			try
			{
				long lSize = 1;
				{
					_bstr_t bstrNode; bstrNode = g_cpszSADescriptorNodeName;
					XML::IXMLDOMElementPtr pDescriptionNode = ptrNode->selectSingleNode(bstrNode);
					CString sTemp = XmlUtils::GetNodeAttrib(pDescriptionNode, g_cpszSADimsAttributeName);
					lDims = _ttoi(sTemp);
					if (0==lDims)
						return FALSE;

					psabound = new SAFEARRAYBOUND[lDims];
					if (NULL==psabound)
						return FALSE;


					for (long i=0; i<lDims; i++)
					{
						sTemp.Format(g_cpszSASizeNodeNameFormat, i);
						sTemp = XmlUtils::GetNodeValue(pDescriptionNode, sTemp);
						psabound[i].lLbound = 0;
						psabound[i].cElements = _ttoi(sTemp);
						lSize *= (psabound[i].cElements);
					}
					if (0==lSize)
					{
						delete []psabound;
						return FALSE;
					}
				}
				psa = SafeArrayCreate(VT_UI1, lDims, psabound);
				if (NULL==psa)
					return FALSE;

				_bstr_t bstrNode; bstrNode = g_cpszSADataNodeName;
				XML::IXMLDOMElementPtr pDataNode = ptrNode->selectSingleNode(bstrNode);
				CString sTemp; 
				sTemp = pDataNode->text.GetBSTR();
				if (!Base64ToBinary(sTemp, (LPBYTE)psa->pvData, lSize))
				{
					delete []psabound;
					return FALSE;
				}
			}
			catch (...)
			{
				if (NULL != psabound)
					delete []psabound;
				return FALSE;
			}
			delete []psabound;
			return TRUE;
		}
		BOOL GetVariantValue(const CString &sType, const CString &sValue, VARIANT &val)
		{
			if ((sType.IsEmpty())||(sValue.IsEmpty()))
				return FALSE;
			val.vt = _ttoi(sType);

			BOOL bRet = TRUE;
			switch (VT_TYPEMASK & val.vt)
			{
			case VT_I2:
				val.iVal = _ttoi(sValue);
				break;
			case VT_I4:
				val.lVal = _ttoi(sValue);
				break;
			case VT_R4:
				val.fltVal = (float)_tstof(sValue);
				break;
			case VT_R8:
				val.dblVal = _tstof(sValue);
				break;
			case VT_DATE:
				val.date = _tstof(sValue);
				break;
			case VT_BSTR:
				val.bstrVal = sValue.AllocSysString();
				break;
			case VT_BOOL:
				val.boolVal = (0!=_ttoi(sValue)) ? VARIANT_TRUE : VARIANT_FALSE;
				break;
			case VT_I1:
				val.cVal = _ttoi(sValue);
				break;
			case VT_UI1:
				val.bVal = _ttoi(sValue);
				break;
			case VT_UI2:
				val.uiVal = _ttoi(sValue);
				break;
			case VT_UI4:
				val.ulVal = _ttoi(sValue);
				break;
			case VT_I8:
				val.llVal = _ttoi(sValue);
				break;
			case VT_UI8:
				val.ullVal = _ttoi(sValue);
				break;
			case VT_INT:
				val.intVal = _ttoi(sValue);
				break;
			case VT_UINT:
				val.uintVal = _ttoi(sValue);
				break;
			case VT_DECIMAL:
			case VT_CY:
			case VT_EMPTY:
			case VT_NULL:
			case VT_SAFEARRAY:
			case VT_VOID:
			case VT_HRESULT:
			case VT_CARRAY:
			case VT_USERDEFINED:
			case VT_LPSTR:
			case VT_LPWSTR:
			case VT_RECORD:
			case VT_INT_PTR:
			case VT_UINT_PTR:
			case VT_FILETIME:
			case VT_BLOB:
			case VT_STREAM:
			case VT_STORAGE:
			case VT_STREAMED_OBJECT:
			case VT_STORED_OBJECT:
			case VT_BLOB_OBJECT:
			case VT_CF:
			case VT_CLSID:
			case VT_VERSIONED_STREAM:
			case VT_BSTR_BLOB:
				bRet = FALSE;
				break;
			}

			return bRet;
		}
		static BOOL Base64ToBinary(const CString& sData, BYTE* pData, int nSize)
		{
			if ((NULL==pData) || (0==nSize))
				return FALSE;
//			LPSTR pStrData = sData.GetBuffer(nStrSize + 1);
			CStringA saData; saData = sData;
			return Base64Decode((LPCSTR)saData/*.GetBuffer()*/, saData.GetLength(), pData, &nSize);
		}
	};
};
	
// Расширение для простого чтения xml через классы
namespace XmlUtils
{
	class IXmlNode
	{
	public:

		virtual BOOL FromXmlNode(XML::IXMLDOMNodePtr& pXmlNode) = 0;
		virtual BOOL FromXmlString(const CString& strXml) = 0;
		virtual BOOL FromXmlFile(const CString& strXmlFilePath) = 0;
	};
	class CXmlNodes
	{
		XML::IXMLDOMNodeListPtr m_pXmlNodes;
		
	public:
		
		CXmlNodes()
		{
			Clear();
		}
		
		void Clear()
		{
			m_pXmlNodes = NULL;
		}
		BOOL IsValid()
		{
			return (NULL != m_pXmlNodes);
		}
		
		BOOL FromXmlNodes(XML::IXMLDOMNodeListPtr& pXmlNodes)
		{
			try
			{
				Clear();

				m_pXmlNodes = pXmlNodes;
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
		
		int GetCount()
		{
			if (!IsValid())
				return 0;

			return m_pXmlNodes->length;
		}
		BOOL GetAt(int nIndex, IXmlNode& oXmlNode)
		{
			if (nIndex < 0)
				return FALSE;

			try
			{
				XML::IXMLDOMNodePtr pXmlNode = NULL;

				pXmlNode = m_pXmlNodes->item[nIndex];

				return oXmlNode.FromXmlNode(pXmlNode);
			}
			catch (...)
			{
			}

			return FALSE;
		}
	};
	class CXmlNode : public IXmlNode
	{
		XML::IXMLDOMDocumentPtr	m_pXmlDocument; // для нода, созданного из строки
		XML::IXMLDOMNodePtr m_pXmlNode;
			
	private:
		
		void CreateDocument()
		{
			if (NULL == m_pXmlDocument)
				m_pXmlDocument.CreateInstance(CLSID_DOMDocument);
		}
		void ReadRootNode()
		{
			m_pXmlNode = m_pXmlDocument->firstChild;
		}
		void ReadRootNode(const CString& strRootNodeName)
		{
			if (strRootNodeName.IsEmpty())
				return ReadRootNode();

			_bstr_t bstrNode; bstrNode = strRootNodeName;

			m_pXmlNode = m_pXmlDocument->selectSingleNode(bstrNode);
		}
		
	public:
		
		CXmlNode()
		{
			Clear();
		}
		
		void Clear()
		{
			m_pXmlDocument = NULL;
			m_pXmlNode = NULL;
		}
		BOOL IsValid()
		{
			return (NULL != m_pXmlNode);
		}
		
		virtual BOOL FromXmlNode(XML::IXMLDOMNodePtr& pXmlNode)
		{
			try
			{
				Clear();

				m_pXmlNode = pXmlNode;
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
		virtual BOOL FromXmlString(const CString& strXml)
		{
			try
			{
				Clear();

				CreateDocument();

				_bstr_t bstrXml; bstrXml = strXml;

				if (!strXml.IsEmpty() && VARIANT_TRUE == m_pXmlDocument->loadXML(bstrXml))
					ReadRootNode();
			}
			catch (...)
			{
				Clear();
			}

			return IsValid();
		}
		virtual BOOL FromXmlFile(const CString& strXmlFilePath)
		{
			BSTR bstrFilePath = strXmlFilePath.AllocSysString();
			BOOL bSuccess = FALSE;

			try
			{
				Clear();

				CreateDocument();

				if (VARIANT_TRUE == m_pXmlDocument->load(bstrFilePath))
					bSuccess = TRUE;

				ReadRootNode();

			}
			catch (...)
			{
				bSuccess = FALSE;
			}

			SysFreeString(bstrFilePath);

			if (!bSuccess)
				Clear();

			return IsValid();
		}
		
		CString GetName()
		{
			if (!IsValid())
				return _T("");

			CString strName;

			try
			{
				BSTR bstrName;

				m_pXmlNode->get_nodeName(&bstrName);

				strName = bstrName;

				SysFreeString(bstrName);
			}
			catch (...)
			{
			}			

			return strName;
		}
		CString GetAttribute(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeAttrib(m_pXmlNode, strAttributeName, strDefaultValue);
		}
		CString GetAttributeOrValue(const CString& strSubNodeName, const CString& strDefaultValue = _T(""))
		{
			CString strInvalidValue = _T("x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = GetAttribute(strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CXmlNode oNode;
			if (GetNode(strSubNodeName, oNode))
				return oNode.GetText(strDefaultValue);

			return strDefaultValue;
		}
		CString GetXml(const CString strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeXml(m_pXmlNode, strDefaultValue);
		}
		CString GetText(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeText(m_pXmlNode, strDefaultValue);
		}
		BOOL GetTextTyped(_variant_t* pVariant, const CString& strNodeType = _T("bin.base64"))
		{
			if (!IsValid())
				return FALSE;

			return GetNodeTextTyped(m_pXmlNode, pVariant, strNodeType);
		}
		BOOL GetNode(const CString& strSubNodeName, CXmlNode& oXmlNode)
		{
			if (!IsValid())
				return FALSE;

			try
			{
				_bstr_t bstrNode; bstrNode = strSubNodeName;

				XML::IXMLDOMNodePtr pNewNode = m_pXmlNode->selectSingleNode(bstrNode);

				if (NULL != pNewNode)
					return oXmlNode.FromXmlNode(pNewNode);
			}
			catch (...)
			{
			}

			return FALSE;
		}
		BOOL GetNodes(const CString& strSubNodesName, CXmlNodes& oXmlNodes)
		{
			if (!IsValid())
				return FALSE;

			try
			{
				_bstr_t bstrNodes; bstrNodes = strSubNodesName;

				XML::IXMLDOMNodeListPtr pNewNodes = m_pXmlNode->selectNodes(bstrNodes);

				if (NULL != pNewNodes)
					return oXmlNodes.FromXmlNodes(pNewNodes);
			}
			catch (...)
			{
			}

			return FALSE;
		}

		CString GetTextExt(const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeValueExt(m_pXmlNode, strDefaultValue);
		}

		CString GetAttributeExt(const CString& strAttributeName, const CString& strDefaultValue = _T(""))
		{
			if (!IsValid())
				return strDefaultValue;

			return GetNodeAttribExt(m_pXmlNode, strAttributeName, strDefaultValue);
		}

		CString GetAttributeOrValueExt(const CString& strSubNodeName, const CString& strDefaultValue = _T(""))
		{
			CString strInvalidValue = _T("x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc");

			CString strAttributeValue = GetAttributeExt(strSubNodeName, strInvalidValue);

			if (strAttributeValue != strInvalidValue)
				return strAttributeValue;

			CXmlNode oNode;
			if (GetNode(strSubNodeName, oNode))
				return oNode.GetTextExt(strDefaultValue);

			return strDefaultValue;
		}
	};
}


//
namespace XmlUtils
{
	// Remove <?xml .. ?> node
	static CStringW RemoveXMLRootNode (CStringW aXML)
	{
		CStringW sRes;
		try
		{
			XML::IXMLDOMDocumentPtr	pXmlDocument = NULL;
			pXmlDocument.CreateInstance(CLSID_DOMDocument);

			_bstr_t bstrXml; bstrXml = aXML;

			if (VARIANT_TRUE == pXmlDocument->loadXML(bstrXml))
			{
				XML::IXMLDOMNodeListPtr ptrNodeList = pXmlDocument->childNodes;				
				for (int i = 0; i < ptrNodeList->length;)
				{
					XML::IXMLDOMNodePtr ptrNode = ptrNodeList->item[i];
					if (NODE_ELEMENT == ptrNode->nodeType)
						break;
					pXmlDocument->removeChild(ptrNode);
				}				
				sRes = pXmlDocument->xml.GetBSTR();
			}
		}
		catch (...)
		{
		}
		return sRes;
	}

	static CStringA ConvertToUTF8 (CStringW aUnicodeXML)
	{
		// delete prefix, if exist
		CStringW sXML = RemoveXMLRootNode(aUnicodeXML);

		// Add xml root node
		sXML.Insert(0, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");

		// Convert to UTF-8
		int iSize = WideCharToMultiByte (CP_UTF8, 0, sXML, -1, NULL, 0, NULL, NULL);
		if (iSize <= 0)
		{
			ATLTRACE2 ("ConvertToUTF8() error (size detection): 0x%x\n", GetLastError());
			return CStringA(sXML);	// Conversion to ANSI
		}
		CStringA sOutXML;
		if (0 == WideCharToMultiByte (CP_UTF8, 0, sXML, -1, sOutXML.GetBuffer(iSize + 1), iSize, NULL, NULL))
		{
			ATLTRACE2 ("ConvertToUTF8() error (utf-8 conversion): 0x%x\n", GetLastError());
			return CStringA(sXML);
		}
		sOutXML.ReleaseBuffer();
		return sOutXML;
	}
	// Check XMl Encoding (ANSI or UTF-8) and converting to WideString
	static CStringW ConvertToUnicode (CStringA aAnsiUtf8XML)
	{
		// find xml root element
		CString sRootElement; sRootElement = aAnsiUtf8XML;
		int nXMLLength = sRootElement.GetLength();
		int nStartPos (-1);
		int nEndPos (-1);
		CString sEncoding = _T("ascii");

		try
		{
			if (nXMLLength > 7)	// <?xml?>
			{
				sRootElement.MakeLower();
				nStartPos = sRootElement.Find(_T("<?xml"));
				if (nStartPos >= 0 && nStartPos < nXMLLength - 2)
				{
					nEndPos = sRootElement.Find(_T("?>"), nStartPos + 5);
					if (nEndPos > nStartPos && nEndPos < nXMLLength)
					{
						// Extract <?xml .. ?> node
						CString sXMLNode = sRootElement.Mid (nStartPos, nEndPos - nStartPos + 2);	// strlen("?>") = 2

						// Check encoding
						sXMLNode += _T("<test/>"); // Add extra node

						XML::IXMLDOMDocumentPtr	pXmlDocument = NULL;
						pXmlDocument.CreateInstance(CLSID_DOMDocument);
						_bstr_t bstrRootXml; bstrRootXml = sXMLNode;

						if (VARIANT_TRUE == pXmlDocument->loadXML(bstrRootXml))
						{
							XML::IXMLDOMNodeListPtr ptrNodeList = pXmlDocument->childNodes;
							for (int i = 0; i < ptrNodeList->length; i++)
							{
								XML::IXMLDOMNodePtr ptrNode = ptrNodeList->item[i];
								CString sName; sName = ptrNode->nodeName.GetBSTR(); sName.MakeLower();
								if (_T("xml") == sName)
								{
									sEncoding = XmlUtils::GetNodeAttrib(ptrNode, _T("encoding"), _T("ascii"));
									break;
								}
							}
						}	// if (VARIANT_TRUE == pXmlDocument->loadXML(bstrXml))
					}	// if (nEndPos > nStartPos && nEndPos < nXMLLength)
				}	// if (nStartPos > 0 && nStartPos < nXMLLength - 2)
			}
		}
		catch (...)
		{
			//MessageBox(NULL, CString (pStr), L"pXmlDocument->loadXML(bstrXml) exception", 0);
			return CStringW (aAnsiUtf8XML);
		}


		sEncoding.MakeLower();

		CStringW sw;
		if (_T("utf-8") == sEncoding)
		{
			int nSize = MultiByteToWideChar(CP_UTF8, 0, aAnsiUtf8XML.GetBuffer(), -1, NULL, 0);
			MultiByteToWideChar(CP_UTF8, 0, aAnsiUtf8XML.GetBuffer(), -1, sw.GetBuffer(nSize + 1), nSize);
			sw.ReleaseBuffer();
		}
		else
		{
			int nSize = MultiByteToWideChar(CP_ACP, 0, aAnsiUtf8XML.GetBuffer(), -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, aAnsiUtf8XML.GetBuffer(), -1, sw.GetBuffer(nSize + 1), nSize);
			sw.ReleaseBuffer();
		}

		return RemoveXMLRootNode(sw);
	}
}