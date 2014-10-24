//#include "stdafx.h"
//#include "LIBXML2_LIB/stdafx.h"
#include "./libxml2.h"

#include <list>

namespace XmlUtils
{
	CString CXmlNodeBase::GetXml()
	{
		CStringWriter oWriter;
		GetXml(oWriter);
		return oWriter.GetData();
	}

	void CXmlNodeBase::GetXml(CStringWriter& oWriter)
	{
		oWriter.WriteString(L"<", 1);
		oWriter.WriteEncodeXmlString(m_sName.GetString());
		
		std::map<CStringA, CStringA>::iterator p;
		for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
		{
			oWriter.WriteString(L" ", 1);
			oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.GetString(), p->first.GetLength(), TRUE).c_str());
			oWriter.WriteString(L"=\"", 2);
			oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.GetString(), p->second.GetLength(), TRUE).c_str());
			oWriter.WriteString(L"\"", 1);
		}

		oWriter.WriteString(L">", 1);

		int nCount = m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			m_nodes[i]->GetXml(oWriter);
		}

		oWriter.WriteEncodeXmlString(m_sText.GetString());
		
		oWriter.WriteString(L"</", 2);
		oWriter.WriteEncodeXmlString(m_sName.GetString());
		oWriter.WriteString(L">", 1);
	}

	class CXmlDOMDocument : public IXmlDOMDocument, public CXmlLiteReader
	{
	public:
		CXmlNodeBase* m_pNode;

	private:
		CXmlNodeBase* m_pCurrentNode;
		std::list<CXmlNodeBase*> m_list;
	
	public:
		CXmlDOMDocument() : IXmlDOMDocument()
		{
			m_pNode = NULL;
			m_pCurrentNode = NULL;
		}
		virtual ~CXmlDOMDocument()
		{
		}

		void WriteElement()
		{
			if (NULL == m_pCurrentNode)
			{
				m_pNode = new CXmlNodeBase();
				m_pNode->m_pDocument = this;
				m_pNode->m_pDocument->AddRef();

				m_pCurrentNode = m_pNode;
			}
			else
			{
				// это m_pCurrentNode
				CXmlNodeBase* pNewBase = new CXmlNodeBase();
				pNewBase->m_pDocument = this;
				pNewBase->m_pDocument->AddRef();

				m_pCurrentNode->m_nodes.insert(m_pCurrentNode->m_nodes.end(), pNewBase);
				m_pCurrentNode = pNewBase;
			}

			m_pCurrentNode->m_sName = GetName();
			m_list.push_back(m_pCurrentNode);
			
			if (GetAttributesCount() > 0)
			{
				MoveToFirstAttribute();
				CStringA sName = GetNameA();
				while (!sName.IsEmpty())
				{
					m_pCurrentNode->m_attributes.insert(std::make_pair(sName, GetTextA()));

					if ( !MoveToNextAttribute() )
						break;
					
					sName = GetNameA();
				}
				MoveToElement();
			}
			if (IsEmptyNode())
			{
				m_list.pop_back();
						
				if (0 != m_list.size())
				{
					std::list<CXmlNodeBase*>::iterator iter = m_list.end();
					--iter;
					m_pCurrentNode = *iter;
				}
				else
				{
					m_pCurrentNode = m_pNode;
				}
			}			
		}

		void Parse()
		{
			ReadNextNode();

			WriteElement();

			int nDepth = GetDepth();
			if ( 0 == xmlTextReaderIsEmptyElement(reader) )
			{
				XmlNodeType eNodeType = XmlNodeType_None;

				int nCurDepth = -1;
				// У закрывающего тэга глубина на 1 больше, чем у открывающего
				while( TRUE )
				{
					if ( 1 != xmlTextReaderRead(reader) )
						break;

					int nTempType = xmlTextReaderNodeType(reader);
					if(-1 == nTempType)
						break;
					eNodeType = (XmlNodeType)nTempType;

					nCurDepth = GetDepth();
					if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
						m_pCurrentNode->m_sText += GetText();
					else if (eNodeType == XmlNodeType_Element)
						WriteElement();
					else if (eNodeType == XmlNodeType_EndElement)
					{
						m_list.pop_back();
						
						if (0 != m_list.size())
						{
							std::list<CXmlNodeBase*>::iterator iter = m_list.end();
							--iter;
							m_pCurrentNode = *iter;
						}
						else
						{
							m_pCurrentNode = m_pNode;
						}
					}

					nCurDepth = GetDepth();
					if ( nCurDepth < nDepth )
						break;

					if ( XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth )
						break;
				}
			}
		}
	};

	// CXmlNode
	CXmlNode::CXmlNode(const CXmlNode& oSrc)
	{
		m_pBase = NULL;
		SetBase(oSrc.m_pBase);
	}

	CString CXmlNode::private_GetXml()
	{
		return private_GetXml(L"");
	}
	CString CXmlNode::private_GetXml(const CString& strDefaultValue)
	{
		if (NULL == m_pBase)
			return strDefaultValue;
		return m_pBase->GetXml();
	}
	CString CXmlNode::private_GetXmlFast()
	{
		return private_GetXmlFast(L"");
	}
	CString CXmlNode::private_GetXmlFast(const CString& strDefaultValue)
	{
		if (NULL == m_pBase)
			return strDefaultValue;
		CStringWriter oWriter;
		m_pBase->GetXml(oWriter);
		return oWriter.GetData();
	}

	bool CXmlNode::FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode)
	{
		//ignore bRemoveRootNode
		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
		BOOL bRes = m_pDocument->FromFile(sFile);
		
		if (FALSE == bRes)
		{
			delete m_pDocument;
			return false;
		}
		
		m_pDocument->Parse();

		if (NULL == m_pDocument->m_pNode)
		{
			delete m_pDocument;
			return false;
		}
		//присваиваем m_pBase без AddRef, чтобы при удалении CXmlNode начался удаляться m_pBase(после конструктора RefCount==1, а если сделать AddRef то не удалится)
		m_pBase = m_pDocument->m_pNode;
		//после Parse все Node из m_pDocument сделали ему AddRef, поэтому можем вызвать Release(потому что напрямую нигде его не используем), а окончательно он удалится после удаления всех Node
		m_pDocument->Release();

		return true;
	}
	BOOL CXmlNode::FromXmlFile2(const CString& strXmlFilePath)
	{
		return FromXmlFile(strXmlFilePath.GetString()) ? TRUE : FALSE;
	}
	bool CXmlNode::FromXmlStringA(const std::string& sString)
	{
		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
		BOOL bRes = m_pDocument->FromStringA(sString);
		
		if (FALSE == bRes)
		{
			delete m_pDocument;
			return false;
		}
		
		m_pDocument->Parse();

		if (NULL == m_pDocument->m_pNode)
		{
			delete m_pDocument;
			return false;
		}
		//присваиваем m_pBase без AddRef, чтобы при удалении CXmlNode начался удаляться m_pBase(после конструктора RefCount==1, а если сделать AddRef то не удалится)
		m_pBase = m_pDocument->m_pNode;
		//после Parse все Node из m_pDocument сделали ему AddRef, поэтому можем вызвать Release(потому что напрямую нигде его не используем), а окончательно он удалится после удаления всех Node
		m_pDocument->Release();
		return true;
	}
	bool CXmlNode::FromXmlString(const std::wstring& sString)
	{
		return FromXmlStringA(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sString.c_str(), (LONG)sString.length()));
	}

	CString CXmlNode::GetText()
	{
		return (IsValid() ? m_pBase->m_sText : L"");
	}
	bool CXmlNode::GetTextIfExist(CString& sOutput)
	{
		bool bRes = false;
		if(IsValid() && !m_pBase->m_sText.IsEmpty())
		{
			sOutput = m_pBase->m_sText;
			bRes = true;
		}
		return bRes;
	}
	CString CXmlNode::GetTextExt(const CString& strDefaultValue)
	{
		//todo xml:space preserve
		CString sRes;
		if(!GetTextIfExist(sRes))
			sRes = strDefaultValue;
		return sRes;
	}
	CString CXmlNode::GetXml(const CString& strDefaultValue)
	{
		return private_GetXmlFast(strDefaultValue);
	}

	CString CXmlNode::GetName()
	{
		return (IsValid() ? m_pBase->m_sName : L"");
	}

    bool CXmlNode::IsValid() const
	{
		return (NULL != m_pBase);
	}
	void CXmlNode::Clear()
	{
		this->SetBase(NULL);
	}
	void CXmlNode::SetBase(CXmlNodeBase* pBase)
	{
		CXmlNodeBase* pBaseOld = m_pBase;
		m_pBase = pBase;
		if(NULL != m_pBase)
			m_pBase->AddRef();
		if (NULL != pBaseOld)
			pBaseOld->Release();
	}
	CString CXmlNode::ReadAttributeBase(const wchar_t* bstrName)
	{
		return GetAttribute(CString(bstrName));
	}
	
	CString CXmlNode::ReadAttribute(const CString& strAttibuteName)
	{
		return GetAttribute(strAttibuteName);
	}

	CStringA CXmlNode::GetAttributeA(const CStringA& sName, const CStringA& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = m_pBase->m_attributes.find(sName);
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return pFind->second;
	}
	CStringA CXmlNode::GetAttributeA(const CString& sName, const CStringA& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = 
			m_pBase->m_attributes.find(std_string2string(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength())));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return pFind->second;
	}

	CString CXmlNode::GetAttribute(const CStringA& sName, const CString& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = m_pBase->m_attributes.find(sName);
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return std_string2string(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second.GetString(), pFind->second.GetLength(), TRUE));
	}
	CString CXmlNode::GetAttribute(const CString& sName, const CString& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = 
			m_pBase->m_attributes.find(std_string2string(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength())));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return std_string2string(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second.GetString(), pFind->second.GetLength(), TRUE));
	}
	bool CXmlNode::GetAttributeIfExist(const CString& sName, CString& sOutput)
	{
		bool bRes = false;
		if (IsValid())
		{
			std::map<CStringA, CStringA>::const_iterator pFind = 
				m_pBase->m_attributes.find(std_string2string(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength())));

			if (pFind != m_pBase->m_attributes.end())
			{
				sOutput = std_string2string(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second.GetString(), pFind->second.GetLength(), TRUE));
				bRes = true;
			}
		}
		return bRes;
	}
	CString CXmlNode::GetAttribute(const wchar_t* sName, const CString& _default)
	{
		return GetAttribute(CString(sName), _default);
	}

	CString CXmlNode::GetAttributeBase(const wchar_t* strAttributeName, const CString& strDefaultValue)
	{
		return GetAttribute(CString(strAttributeName), strDefaultValue);
	}

	int CXmlNode::GetAttributeInt(const CStringA& sName, const int& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = m_pBase->m_attributes.find(sName);

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.GetLength())
			return _default;

		return atoi(pFind->second.GetString());
	}
	int CXmlNode::GetAttributeInt(const CString& sName, const int& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = 
			m_pBase->m_attributes.find(std_string2string(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength())));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.GetLength())
			return _default;

		return atoi(pFind->second.GetString());
	}
	int CXmlNode::ReadAttributeInt(const CString& str, const int& nDef)
	{
		return GetAttributeInt(str, nDef);
	}

	double CXmlNode::GetAttributeDouble(const CStringA& sName, const double& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = m_pBase->m_attributes.find(sName);

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.GetLength())
			return _default;

		return atof(pFind->second.GetString());
	}
	double CXmlNode::GetAttributeDouble(const CString& sName, const double& _default)
	{
		if (!IsValid())
			return _default;

		std::map<CStringA, CStringA>::const_iterator pFind = 
			m_pBase->m_attributes.find(std_string2string(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), (LONG)sName.GetLength())));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.GetLength())
			return _default;

		return atof(pFind->second.GetString());
	}

	CString CXmlNode::ReadNodeTextBase(const wchar_t* bsName)
	{
		return ReadNodeText(CString(bsName));
	}
	CString CXmlNode::ReadNodeText(const CString& strName)
	{
		return ReadValueString(strName);
	}
	CString CXmlNode::GetAttributeOrValue(const CString& strAttributeName, const CString& strDefaultValue)
	{
		CString sRes;
		if(!GetAttributeIfExist(strAttributeName, sRes))
		{
			if(!GetTextIfExist(sRes))
				sRes = strDefaultValue;
		}
		return sRes;
	}
	CString CXmlNode::ReadValueString(const CString& sName, const CString& nDef)
	{
		CString sRes;
		CXmlNode oTemp;
		if(GetNode(sName, oTemp))
			sRes = oTemp.GetText();
		return sRes;
	}
	int CXmlNode::ReadValueInt(const CString& str, const int& nDef)
	{
		int nRes = nDef;
		CXmlNode oTemp;
		if(GetNode(str, oTemp))
		{
			CString sText;
			if(oTemp.GetTextIfExist(sText))
				nRes = XmlUtils::GetInteger(sText);
		}
		return nRes;
	}

    CXmlNode CXmlNode::ReadNode(const TCHAR* strNodeName)
    {
        CXmlNode oNode;
        CString sNodeName = strNodeName;
        GetNode(sNodeName, oNode);
        return oNode;
    }
	CXmlNode CXmlNode::ReadNode(const CString& strNodeName)
	{
		CXmlNode oNode;
		GetNode(strNodeName, oNode);
		return oNode;
	}
	CXmlNode CXmlNode::ReadNodeNoNS(const CString& strNodeName)
	{
		CXmlNode node;
		if (IsValid())
		{
			int nCount = (int)m_pBase->m_nodes.size();
			for (int i = 0; i < nCount; ++i)
			{
				if (strNodeName == GetNameNoNS(m_pBase->m_nodes[i]->m_sName))
				{
					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
					node.SetBase(pBase);
					break;
				}
			}
		}
		return node;
	}

	CXmlNode CXmlNode::GetNode(const CString& sName)
	{
		CXmlNode oNode;
		GetNode(sName, oNode);
		return oNode;
	}
	bool CXmlNode::GetNode(const CString& sName, CXmlNode& oNode)
	{
		bool bRes = false;
		if (IsValid())
		{
			int nCount = (int)m_pBase->m_nodes.size();
			for (int i = 0; i < nCount; ++i)
			{
				if (sName == m_pBase->m_nodes[i]->m_sName)
				{
					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
					oNode.SetBase(pBase);
					bRes = true;
					break;
				}
			}
		}
		return bRes;
	}
	CXmlNodes CXmlNode::GetNodes(const CString& sName)
	{
		CXmlNodes oNodes;
		if (IsValid())
		{
			bool bGetAll = false;
			if(_T("*") == sName)
				bGetAll = true;
			int nCount = (int)m_pBase->m_nodes.size();
			for (int i = 0; i < nCount; ++i)
			{
				if (bGetAll || sName == m_pBase->m_nodes[i]->m_sName)
				{
					CXmlNode oNode;
					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
					oNode.SetBase(pBase);
					oNodes.m_nodes.insert(oNodes.m_nodes.end(), oNode);
				}
			}
		}

		return oNodes;
	}
	BOOL CXmlNode::GetChilds(CXmlNodes& oXmlNodes)
	{
		BOOL bRes = FALSE;
		if (IsValid())
		{
			int nCount = (int)m_pBase->m_nodes.size();
			if(nCount > 0)
			{
				bRes = TRUE;
				for (int i = 0; i < nCount; ++i)
				{
					CXmlNode oNode;
					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
					oNode.SetBase(pBase);
					oXmlNodes.m_nodes.insert(oXmlNodes.m_nodes.end(), oNode);
				}
			}
		}
		return bRes;
	}
	bool CXmlNode::GetNodes(const CString& sName, CXmlNodes& oNodes)
	{
		oNodes = GetNodes(sName);
		return (0 != oNodes.GetCount());
	}

	CXmlNode& CXmlNode::operator=(const CXmlNode& oSrc)
	{
		this->SetBase(oSrc.m_pBase);
		return *this;
	}
	CString CXmlNode::GetNamespace(const CString& strNodeName)
	{
		int nFind = strNodeName.Find(TCHAR(':'));
		if (-1 == nFind)
			return CString(_T(""));
		return strNodeName.Mid(0, nFind);
	}
	CString CXmlNode::GetNameNoNS(const CString& strNodeName)
	{
		int nFind= strNodeName.Find(TCHAR(':'));
		if (-1 == nFind)
			return strNodeName;
		return strNodeName.Mid(nFind + 1);
	}

    // CXmlNodes
    CXmlNodes::CXmlNodes() : m_nodes()
    {
    }
    BOOL CXmlNodes::IsValid()
    {
        return TRUE;
    }
    int CXmlNodes::GetCount()
    {
        return (int)m_nodes.size();
    }
    bool CXmlNodes::GetAt(int nIndex, CXmlNode& oXmlNode)
    {
        if (nIndex < 0 && nIndex >= GetCount())
            return false;

        oXmlNode = m_nodes[nIndex];
        return true;
    }
}
