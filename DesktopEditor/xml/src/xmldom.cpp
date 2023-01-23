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
#include "../include/xmlutils.h"
#include "./xmllight_private.h"
#include <list>

namespace XmlUtils
{
    std::wstring GetNameNoNS(const std::wstring & strNodeName)
    {
        int nFind = (int)strNodeName.find(L":");
        if (-1 == nFind)
            return strNodeName;
        return strNodeName.substr(nFind + 1);
    }
	const char* GetNameNoNS(const char* strNodeName)
	{
		const char* find = strchr(strNodeName, ':');
		if (NULL != find)
		{
			return find + 1;
		}
		else
		{
			return strNodeName;
		}
	}
    std::wstring GetNamespace(const std::wstring& strNodeName)
    {
        int nFind = (int)strNodeName.find(L":");
        if (-1 == nFind)
            return L"";
        return strNodeName.substr(0, nFind);
    }
}

namespace XmlUtils
{
    class CXmlNodeBase : public IXmlDOMDocument
    {
    public:
        IXmlDOMDocument* m_pDocument;

        std::map<std::string, std::string> m_attributes;
        std::vector<CXmlNodeBase*> m_nodes;
        std::wstring m_sText;
        std::wstring m_sName;

    public:
        CXmlNodeBase();
        ~CXmlNodeBase();

        void AddRefDoc();
        void ReleaseDoc();

        std::wstring GetXml();
        void GetXml(NSStringUtils::CStringBuilder& oWriter);
    };

	CXmlNodeBase::CXmlNodeBase()
	{
		m_pDocument = NULL;
		m_sText = L"";
		m_sName = L"";
	}
	CXmlNodeBase::~CXmlNodeBase()
	{
		if (NULL != m_pDocument)
			m_pDocument->Release();

		int nCount = (int)m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			CXmlNodeBase* pNode = m_nodes[i];
			if (NULL != pNode)
				pNode->Release();
		}
	}

	void CXmlNodeBase::AddRefDoc()
	{
		if (NULL != m_pDocument)
			m_pDocument->AddRef();
	}
	void CXmlNodeBase::ReleaseDoc()
	{
		if (NULL != m_pDocument)
			m_pDocument->Release();
	}

	std::wstring CXmlNodeBase::GetXml()
	{
		NSStringUtils::CStringBuilder oWriter;
		GetXml(oWriter);
		return oWriter.GetData();
	}
	void CXmlNodeBase::GetXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<", 1);
		oWriter.WriteEncodeXmlString(m_sName.c_str());

		std::map<std::string, std::string>::iterator p;
		for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
		{
			oWriter.WriteString(L" ", 1);
			oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->first.c_str(), (LONG)p->first.length()).c_str());
			oWriter.WriteString(L"=\"", 2);
			oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->second.c_str(), (LONG)p->second.length()).c_str());
			oWriter.WriteString(L"\"", 1);
		}

		oWriter.WriteString(L">", 1);

		int nCount = m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			m_nodes[i]->GetXml(oWriter);
		}

		oWriter.WriteEncodeXmlString(m_sText.c_str());

		oWriter.WriteString(L"</", 2);
		oWriter.WriteEncodeXmlString(m_sName.c_str());
		oWriter.WriteString(L">", 1);
	}
}

namespace XmlUtils
{
	CXmlNodes::CXmlNodes() : m_nodes()
	{
	}
    CXmlNodes::~CXmlNodes()
    {
    }
	bool CXmlNodes::IsValid()
	{
		return true;
	}
	int CXmlNodes::GetCount()
	{
		return (int)m_nodes.size();
	}
	bool CXmlNodes::GetAt(int nIndex, CXmlNode& oXmlNode)
	{
		if (nIndex < 0 || nIndex >= GetCount())
			return false;

		oXmlNode = m_nodes[nIndex];
		return true;
	}
}

namespace XmlUtils
{
	class CXmlDOMDocument : public IXmlDOMDocument, public CXmlLiteReader_Private
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
				std::string sName = GetNameA();
				while (!sName.empty())
				{
					m_pCurrentNode->m_attributes.insert(std::make_pair(sName, GetTextA()));

					if (!MoveToNextAttribute())
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
			if (!IsEmptyElement())
			{
				XmlNodeType eNodeType = XmlNodeType_None;

				int nCurDepth = -1;
				// У закрывающего тэга глубина на 1 больше, чем у открывающего
				while (true)
				{
					if (1 != xmlTextReaderRead(reader))
						break;

					int nTempType = xmlTextReaderNodeType(reader);
					if (-1 == nTempType)
						break;
					eNodeType = (XmlNodeType)nTempType;

					nCurDepth = GetDepth();
					if (eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE)
					{
						m_pCurrentNode->m_sText += GetText();
					}
					else if (eNodeType == XmlNodeType_CDATA)
					{
						m_pCurrentNode->m_sText += GetText();
					}
					else if (eNodeType == XmlNodeType_Element)
					{
						WriteElement();
					}
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
					if (nCurDepth < nDepth)
						break;

					if (XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth)
						break;
				}
			}
		}
	};
}

namespace XmlUtils
{
	CXmlNode::CXmlNode()
	{
		m_pBase = NULL;
	}
	CXmlNode::CXmlNode(const CXmlNode& oSrc)
	{
		m_pBase = NULL;
		SetBase(oSrc.m_pBase);
	}
	CXmlNode::~CXmlNode()
	{
		Clear();
	}

	bool CXmlNode::FromXmlFile(const wchar_t* sFile, bool bRemoveRootNode)
	{
		return FromXmlFile(std::wstring(sFile), bRemoveRootNode);
	}
	bool CXmlNode::FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode)
	{
		//ignore bRemoveRootNode
		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
		bool bRes = m_pDocument->FromFile(sFile);

		if (false == bRes)
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

	bool CXmlNode::FromXmlStringA(const std::string& sString)
	{
		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
		bool bRes = m_pDocument->FromStringA(sString);

		if (false == bRes)
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
	bool CXmlNode::FromXmlString(const wchar_t* sString)
	{
		return FromXmlString(std::wstring(sString));		
	}
	bool CXmlNode::FromXmlString(const std::wstring& sString)
	{
		return FromXmlStringA(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sString.c_str(), (LONG)sString.length()));
	}

	bool CXmlNode::IsValid() const
	{
		return (NULL != m_pBase);
	}
	void CXmlNode::Clear()
	{
		this->SetBase(NULL);
	}
	std::wstring CXmlNode::GetName()
	{
		return (IsValid() ? m_pBase->m_sName : L"");
	}
	std::wstring CXmlNode::GetText()
	{
		return (IsValid() ? m_pBase->m_sText : L"");
	}
	bool CXmlNode::GetTextIfExist(std::wstring& sOutput)
	{
		bool bRes = false;
		if (IsValid() && !m_pBase->m_sText.empty())
		{
			sOutput = m_pBase->m_sText;
			bRes = true;
		}
		return bRes;
	}
	std::wstring CXmlNode::GetTextExt(const std::wstring& strDefaultValue)
	{
		//todo xml:space preserve
		std::wstring sRes;
		if (!GetTextIfExist(sRes))
			sRes = strDefaultValue;
		return sRes;
	}
	std::wstring CXmlNode::GetXml(const std::wstring& strDefaultValue)
	{
		return private_GetXmlFast(strDefaultValue);
	}

	std::wstring CXmlNode::ReadAttributeBase(const wchar_t* bstrName)
	{
		return GetAttribute(std::wstring(bstrName));
	}
	std::wstring CXmlNode::ReadAttribute(const std::wstring& strAttibuteName)
	{
		return GetAttribute(std::wstring(strAttibuteName));
	}

    void CXmlNode::ReadAllAttributesA(std::vector<std::string>& strNames, std::vector<std::string>& strValues)
    {
        if (!IsValid())
            return;

        std::map<std::string, std::string>::iterator p;
        for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            strNames.push_back(p->first);
            strValues.push_back(p->second);
        }
    }

    void CXmlNode::ReadAllAttributes(std::vector<std::wstring>& strNames, std::vector<std::wstring>& strValues)
    {
        if (!IsValid())
            return;

        std::map<std::string, std::string>::iterator p;
        for (p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            strNames.push_back  (NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->first.c_str(), (long)p->first.length()));
            strValues.push_back (NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)p->second.c_str(), (long)p->second.length()));
        }
    }

	std::string CXmlNode::GetAttributeA(const std::string& sName, const std::string& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return pFind->second;
	}
	std::string CXmlNode::GetAttributeA(const std::wstring& sName, const std::string& _default)
	{
		return GetAttributeA(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length()));
	}

	std::wstring CXmlNode::GetAttribute(const std::string& sName, const std::wstring& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFind->second.c_str(), (LONG)pFind->second.length());
	}
	std::wstring CXmlNode::GetAttribute(const std::wstring& sName, const std::wstring& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(U_TO_UTF8(sName));
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFind->second.c_str(), (LONG)pFind->second.length());
	}
	std::wstring CXmlNode::GetAttribute(const wchar_t* sName, const std::wstring& _default)
	{
		return GetAttribute(std::wstring(sName), _default);
	}

	std::wstring CXmlNode::GetAttributeBase(const wchar_t* strAttributeName, const std::wstring& strDefaultValue)
	{
		return GetAttribute(std::wstring(strAttributeName), strDefaultValue);
	}
	int CXmlNode::GetAttributesCount()
	{
		if (IsValid())
		{
			return m_pBase->m_attributes.size();
		}
		else 
			return 0;
	}
    void CXmlNode::GetAllAttributes(std::vector<std::wstring>& names, std::vector<std::wstring>& values)
    {
        for (std::map<std::string, std::string>::iterator p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            names.push_back(UTF8_TO_U(p->first));
            values.push_back(UTF8_TO_U(p->second));
        }
    }
    void CXmlNode::GetAllAttributes(std::vector<std::string>& names, std::vector<std::string>& values)
    {
        for (std::map<std::string, std::string>::iterator p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            names.push_back(p->first);
            values.push_back(p->second);
        }
    }
    void CXmlNode::GetAllAttributes(std::list<std::wstring>& names, std::list<std::wstring>& values)
    {
        for (std::map<std::string, std::string>::iterator p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            names.push_back(UTF8_TO_U(p->first));
            values.push_back(UTF8_TO_U(p->second));
        }
    }
    void CXmlNode::GetAllAttributes(std::list<std::string>& names, std::list<std::string>& values)
    {
        for (std::map<std::string, std::string>::iterator p = m_pBase->m_attributes.begin(); p != m_pBase->m_attributes.end(); ++p)
        {
            names.push_back(p->first);
            values.push_back(p->second);
        }
    }
	bool CXmlNode::GetAttributeIfExist(const std::wstring& sName, std::wstring& sOutput)
	{
		bool bRes = false;
		if (IsValid())
		{
			std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(U_TO_UTF8(sName));

			if (pFind != m_pBase->m_attributes.end())
			{
				sOutput = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFind->second.c_str(), (LONG)pFind->second.length());
				bRes = true;
			}
		}
		return bRes;
	}

	int CXmlNode::GetAttributeInt(const std::string& sName, const int& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atoi(pFind->second.c_str());
	}
	bool CXmlNode::SetAttributeInt(const std::wstring& sName, const int& value)
	{
		if (!IsValid())
			return false;

		std::map<std::string, std::string>::iterator pFind = m_pBase->m_attributes.find(U_TO_UTF8(sName));

		if (pFind == m_pBase->m_attributes.end())
			return false;

		pFind->second = std::to_string(value);

		return true;
	}
	int CXmlNode::GetAttributeInt(const std::wstring& sName, const int& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(U_TO_UTF8(sName));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atoi(pFind->second.c_str());
	}
	int CXmlNode::ReadAttributeInt(const std::wstring& str, const int& _default)
	{
		return GetAttributeInt(str, _default);
	}

	double CXmlNode::GetAttributeDouble(const std::string& sName, const double& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atof(pFind->second.c_str());
	}
	double CXmlNode::GetAttributeDouble(const std::wstring& sName, const double& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(U_TO_UTF8(sName));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atof(pFind->second.c_str());
	}
	double CXmlNode::ReadAttributeDouble(const std::wstring& str, const double& _default)
	{
		return GetAttributeDouble(str, _default);
	}

	std::wstring CXmlNode::ReadNodeTextBase(const wchar_t* bsName)
	{
		return ReadNodeText(std::wstring(bsName));
	}

	std::wstring CXmlNode::ReadNodeText(const std::wstring& strName)
	{
		return ReadValueString(strName);
	}
	std::wstring CXmlNode::ReadValueString(const std::wstring& sName, const std::wstring& nDef)
	{
		CXmlNode oTemp;
		if (GetNode(sName, oTemp))
			return oTemp.GetText();
		return L"";
	}
	int CXmlNode::ReadValueInt(const std::wstring& str, const int& nDef)
	{
		int nRes = nDef;
		CXmlNode oTemp;
		if (GetNode(str, oTemp))
		{
			std::wstring sText;
			if (oTemp.GetTextIfExist(sText))
            {
                try
                {
                    nRes = std::stoi(sText);
                }
                catch (...)
                {
                    nRes = nDef;
                }
            }
		}
		return nRes;
	}

	std::wstring CXmlNode::GetAttributeOrValue(const std::wstring& strAttributeName, const std::wstring& strDefaultValue)
	{
		std::wstring sRes;
		if (!GetAttributeIfExist(strAttributeName, sRes))
		{
            CXmlNode oTemp;
            if (GetNode(strAttributeName, oTemp))
                sRes = oTemp.GetText();
            else
                sRes = strDefaultValue;
		}
		return sRes;
	}

	CXmlNode CXmlNode::ReadNode(const wchar_t* strNodeName)
	{
		CXmlNode oNode;
		GetNode(std::wstring(strNodeName), oNode);
		return oNode;
	}
	CXmlNode CXmlNode::ReadNode(const std::wstring& strNodeName)
	{
		CXmlNode oNode;
		GetNode(strNodeName, oNode);
		return oNode;
	}
	CXmlNode CXmlNode::ReadNodeNoNS(const std::wstring& strNodeName)
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
	CXmlNodes CXmlNode::ReadNodesNoNS(const std::wstring& sName)
	{
		CXmlNodes oNodes;
		if (IsValid())
		{
			bool bGetAll = false;
			if (L"*" == sName)
				bGetAll = true;
			int nCount = (int)m_pBase->m_nodes.size();
			for (int i = 0; i < nCount; ++i)
			{
				if (bGetAll || sName == GetNameNoNS(m_pBase->m_nodes[i]->m_sName))
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

	CXmlNode CXmlNode::GetNode(const std::wstring& sName)
	{
		CXmlNode oNode;
		GetNode(sName, oNode);
		return oNode;
	}
	CXmlNodes CXmlNode::GetNodes(const std::wstring& sName)
	{
		CXmlNodes oNodes;
		if (IsValid())
		{
			bool bGetAll = false;
			if (L"*" == sName)
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
	bool CXmlNode::GetChilds(CXmlNodes& oXmlNodes)
	{
		bool bRes = false;
		if (IsValid())
		{
			int nCount = (int)m_pBase->m_nodes.size();
			if (nCount > 0)
			{
				bRes = true;
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

	bool CXmlNode::GetNode(const std::wstring& sName, CXmlNode& oNode)
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
	bool CXmlNode::GetNodes(const std::wstring& sName, CXmlNodes& oNodes)
	{
		oNodes = GetNodes(sName);
		return (0 != oNodes.GetCount());
	}

	CXmlNode& CXmlNode::operator=(const CXmlNode& oSrc)
	{
		this->SetBase(oSrc.m_pBase);
		return *this;
	}

	std::wstring CXmlNode::private_GetXml()
	{
		return private_GetXml(L"");
	}
	std::wstring CXmlNode::private_GetXml(const std::wstring& strDefaultValue)
	{
		if (NULL == m_pBase)
			return strDefaultValue;
		return m_pBase->GetXml();
	}
	std::wstring CXmlNode::private_GetXmlFast()
	{
		return private_GetXmlFast(L"");
	}
	std::wstring CXmlNode::private_GetXmlFast(const std::wstring& strDefaultValue)
	{
		if (NULL == m_pBase)
			return strDefaultValue;
		NSStringUtils::CStringBuilder oWriter;
		m_pBase->GetXml(oWriter);
		return oWriter.GetData();
	}    

	void CXmlNode::SetBase(CXmlNodeBase* pBase)
	{
		CXmlNodeBase* pBaseOld = m_pBase;
		m_pBase = pBase;
		if (NULL != m_pBase)
			m_pBase->AddRef();
		if (NULL != pBaseOld)
			pBaseOld->Release();
	}
    std::wstring CXmlNode::GetNamespace(const std::wstring& strNodeName)
    {
        int nFind = strNodeName.find(wchar_t(':'));
        if (-1 == nFind)
            return L"";
        return strNodeName.substr(0, nFind);
    }
    std::wstring CXmlNode::GetNameNoNS(const std::wstring& strNodeName)
    {
        int nFind = strNodeName.find(wchar_t(':'));
        if (-1 == nFind)
            return strNodeName;
        return strNodeName.substr(nFind + 1);
    }
}

namespace XmlUtils
{
    class CXmlBuffer
    {
    public:
        NSStringUtils::CStringBuilderA builder;

    public:
        CXmlBuffer()
        {
        }
        ~CXmlBuffer()
        {
        }
    };

    static int buffer_xmlBufferIOWrite(CXmlBuffer* buf, const char* buffer, int len)
    {
        buf->builder.WriteString(buffer, (size_t)len);
        return len;
    }

    static int buffer_xmlBufferIOClose(CXmlBuffer* buf)
    {
        XML_UNUSED(buf);
        return 0;
    }

    static int buffer_xmlC14NIsVisibleCallback(void * user_data, xmlNodePtr node, xmlNodePtr parent)
    {
        XML_UNUSED(user_data);
        XML_UNUSED(parent);
        if (node->type == XML_TEXT_NODE)
        {
            const char* cur = (char*)node->content;
            size_t size = strlen(cur);
            for (size_t i = 0; i < size; ++i, ++cur)
            {
                if (*cur != '\n' && *cur != '\r' && *cur != '\t')
                    return 1;
            }
            return 0;
        }
        return 1;
    }

    std::string NSXmlCanonicalizator::Execute(const std::string& sXml, int mode, bool withComments)
    {
#ifdef LIBXML_C14N_ENABLED
        xmlDocPtr xmlDoc = xmlParseMemory((char*)sXml.c_str(), (int)sXml.length());

        CXmlBuffer bufferC14N;
        xmlOutputBufferPtr _buffer = xmlOutputBufferCreateIO((xmlOutputWriteCallback)buffer_xmlBufferIOWrite,
                                                             (xmlOutputCloseCallback)buffer_xmlBufferIOClose,
                                                             &bufferC14N,
                                                             NULL);

        xmlC14NExecute(xmlDoc, buffer_xmlC14NIsVisibleCallback, NULL, mode, NULL, withComments ? 1 : 0, _buffer);

        xmlOutputBufferClose(_buffer);

        return bufferC14N.builder.GetData();
#else
		return "";
#endif
    }
    std::string NSXmlCanonicalizator::Execute(const std::wstring& sXmlFile, int mode, bool withComments)
    {
        std::string sXml;
        NSFile::CFileBinary::ReadAllTextUtf8A(sXmlFile, sXml);
        return Execute(sXml, mode, withComments);
    }
}

std::string XmlUtils::GetUtf8FromFileContent(unsigned char* pData, unsigned int len)
{
    if (4 > len)
        return std::string((char*)pData, (size_t)len);

    if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xFE && pData[3] == 0xBF)
    {
        return std::string((char*)pData + 3, (size_t)(len - 3));
    }

    char markerUtf16 = 0;
    if (pData[0] == 0xFF && pData[1] == 0xFE && !(pData[2] == 0x00 && pData[3] == 0x00))
        markerUtf16 = 1;
    if (pData[0] == 0xFE && pData[1] == 0xFF)
        markerUtf16 = 2;

    if (0 != markerUtf16)
    {
        int nCountSymbols = (len - 2) >> 1;
        int nCountSymbolsNatural = 0;

        unsigned int* pUnicodes = new unsigned int[nCountSymbols];
        unsigned char* pCurrent = pData + 2;

        for (int i = 0; i < nCountSymbols; ++i)
        {
            unsigned short nLeading = (markerUtf16 == 1) ? (pCurrent[0] | (pCurrent[1] << 8)) : (pCurrent[1] | (pCurrent[0] << 8));
            pCurrent += 2;

            if (nLeading < 0xD800 || nLeading > 0xDFFF)
            {
                pUnicodes[nCountSymbolsNatural++] = nLeading;
            }
            else
            {
                i++;
                if (i == nCountSymbols)
                    break;

                unsigned short nTrailing = (markerUtf16 == 1) ? (pCurrent[0] | (pCurrent[1] << 8)) : (pCurrent[1] | (pCurrent[0] << 8));
                pCurrent += 2;

                if (nTrailing >= 0xDC00 && nTrailing <= 0xDFFF)
                {
                    pUnicodes[nCountSymbolsNatural++] = 0x10000 + (((nLeading & 0x03FF) << 10) | (nTrailing & 0x03FF));
                }
            }
        }

        std::string sRet = NSStringExt::CConverter::GetUtf8FromUTF32(pUnicodes, nCountSymbolsNatural);

        RELEASEARRAYOBJECTS(pUnicodes);
        return sRet;
    }

    char markerUtf32 = 0;
    if (pData[0] == 0xFF && pData[1] == 0xFE && pData[2] == 0x00 && pData[3] == 0x00)
        markerUtf32 = 1;
    if (pData[0] == 0 && pData[1] == 0 && pData[2] == 0xFE && pData[3] == 0xFF)
        markerUtf32 = 2;

    if (0 != markerUtf16)
    {
        int nCountSymbols = (len - 4) >> 2;
        int nCountSymbolsNatural = 0;

        unsigned int* pUnicodes = new unsigned int[nCountSymbols];
        unsigned char* pCurrent = pData + 4;

        if (markerUtf32 == 1)
        {
            for (int i = 0; i < nCountSymbols; ++i, pCurrent += 4)
                pUnicodes[nCountSymbolsNatural++] = (pCurrent[0] | (pCurrent[1] << 8) | (pCurrent[2] << 16) | (pCurrent[3] << 24));
        }
        else
        {
            for (int i = 0; i < nCountSymbols; ++i, pCurrent += 4)
                pUnicodes[nCountSymbolsNatural++] = (pCurrent[3] | (pCurrent[2] << 8) | (pCurrent[1] << 16) | (pCurrent[0] << 24));
        }

        std::string sRet = NSStringExt::CConverter::GetUtf8FromUTF32(pUnicodes, nCountSymbolsNatural);

        RELEASEARRAYOBJECTS(pUnicodes);
        return sRet;
    }

    return std::string((char*)pData, (size_t)len);
}
