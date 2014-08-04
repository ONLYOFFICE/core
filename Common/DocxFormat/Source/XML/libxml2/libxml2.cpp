#include "LIBXML2_LIB/stdafx.h"
#include "./libxml2.h"

#include <list>

namespace XmlUtils
{
	std::wstring CXmlNodeBase::GetXml()
	{
		std::wstring sXml = L"<";
		sXml += m_sName;
		
		std::map<std::string, std::string>::iterator p;
		for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
		{
			sXml += L" ";
			sXml += NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.c_str(), p->first.length(), TRUE);
			sXml += L"='";
			sXml += NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.c_str(), p->second.length(), TRUE);
			sXml += L"'";
		}

		sXml += L">";

		int nCount = m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			sXml += m_nodes[i]->GetXml();
		}

		sXml += m_sText;

		sXml += L"</";
		sXml += m_sName;
		sXml += L">";

		return sXml;
	}

	void CXmlNodeBase::GetXml(CStringWriter& oWriter)
	{
		oWriter.WriteString(L"<", 1);
		oWriter.WriteString(m_sName);
		
		std::map<std::string, std::string>::iterator p;
		for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
		{
			oWriter.WriteString(L" ", 1);
			oWriter.WriteString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.c_str(), p->first.length(), TRUE));
			oWriter.WriteString(L"='", 2);
			oWriter.WriteString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.c_str(), p->second.length(), TRUE));
			oWriter.WriteString(L"'", 1);
		}

		oWriter.WriteString(L">", 1);

		int nCount = m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			m_nodes[i]->GetXml(oWriter);
		}

		oWriter.WriteString(m_sText);
		
		oWriter.WriteString(L"</", 2);
		oWriter.WriteString(m_sName);
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
				std::string sName = GetNameA();
				while (!sName.empty())
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
					if ( 0 == xmlTextReaderRead(reader) )
						break;

					eNodeType = (XmlNodeType)xmlTextReaderNodeType(reader);

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
					if ( nCurDepth <= nDepth )
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
		m_pBase = oSrc.m_pBase;
		if (NULL != m_pBase)
			m_pBase->AddRef();
	}

	std::wstring CXmlNode::private_GetXml()
	{
		if (NULL == m_pBase)
			return L"";
		return m_pBase->GetXml();
	}
	std::wstring CXmlNode::private_GetXmlFast()
	{
		if (NULL == m_pBase)
			return L"";
		CStringWriter oWriter;
		m_pBase->GetXml(oWriter);
		return oWriter.GetData();
	}

	bool CXmlNode::FromXmlFile(const std::wstring& sFile)
	{
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

		m_pBase = m_pDocument->m_pNode;
		m_pBase->AddRef();

		return true;
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

		if (NULL != m_pDocument->m_pNode)
		{
			delete m_pDocument;
			return false;
		}

		m_pBase = m_pDocument->m_pNode;
		m_pBase->AddRef();

		return true;
	}
	bool CXmlNode::FromXmlString(const std::wstring& sString)
	{
		return FromXmlStringA(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sString.c_str(), (LONG)sString.length()));
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
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = 
			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), sName.length()));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return pFind->second;
	}

	std::wstring CXmlNode::GetAttribute(const std::string& sName, const std::wstring& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);
		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second.c_str(), pFind->second.length(), TRUE);
	}
	std::wstring CXmlNode::GetAttribute(const std::wstring& sName, const std::wstring& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = 
			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), sName.length()));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second.c_str(), pFind->second.length(), TRUE);
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
	int CXmlNode::GetAttributeInt(const std::wstring& sName, const int& _default)
	{
		if (!IsValid())
			return _default;

		std::map<std::string, std::string>::const_iterator pFind = 
			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), sName.length()));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atoi(pFind->second.c_str());
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

		std::map<std::string, std::string>::const_iterator pFind = 
			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length()));

		if (pFind == m_pBase->m_attributes.end())
			return _default;

		if (0 == pFind->second.length())
			return _default;

		return atof(pFind->second.c_str());
	}

	std::wstring CXmlNode::GetText()
	{
		return (IsValid() ? m_pBase->m_sText : L"");
	}

	bool CXmlNode::IsValid()
	{
		return (NULL != m_pBase);
	}

	CXmlNode CXmlNode::GetNode(const std::wstring& sName)
	{
		CXmlNode oNode;
		if (!IsValid())
			return oNode;

		int nCount = (int)m_pBase->m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			if (sName == m_pBase->m_nodes[i]->m_sName)
			{
				CXmlNodeBase* pBase = m_pBase->m_nodes[i];
				pBase->AddRef();
				oNode.m_pBase = pBase;
				break;
			}
		}

		return oNode;
	}
	CXmlNodes CXmlNode::GetNodes(const std::wstring& sName)
	{
		CXmlNodes oNodes;

		int nCount = (int)m_pBase->m_nodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			if (sName == m_pBase->m_nodes[i]->m_sName)
			{
				CXmlNode oNode;
				oNode.m_pBase = m_pBase->m_nodes[i];
				if (oNode.m_pBase)
					oNode.m_pBase->AddRef();
				oNodes.m_nodes.insert(oNodes.m_nodes.end(), oNode);
			}
		}

		return oNodes;
	}

	bool CXmlNode::GetNode(const std::wstring& sName, CXmlNode& oNode)
	{
		oNode = GetNode(sName);
		return oNode.IsValid();
	}
	bool CXmlNode::GetNodes(const std::wstring& sName, CXmlNodes& oNodes)
	{
		oNodes = GetNodes(sName);
		return (0 != oNodes.GetCount());
	}

	CXmlNode& CXmlNode::operator=(const CXmlNode& oSrc)
	{
		m_pBase = oSrc.m_pBase;
		if (NULL != m_pBase)
			m_pBase->AddRef();			
		
		return *this;
	}
}