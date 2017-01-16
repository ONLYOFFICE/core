/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
//#include "stdafx.h"
//#include "LIBXML2_LIB/stdafx.h"
#include "./libxml2.h"

#include <list>

namespace XmlUtils
{
	//std::wstring CXmlNodeBase::GetXml()
	//{
    //	CStringWriter oWriter;
	//	GetXml(oWriter);
	//	return oWriter.GetData();
	//}

    void CXmlNodeBase::GetXml(CStringWriter& oWriter)
	{
		oWriter.WriteString(L"<", 1);
        oWriter.WriteEncodeXmlString(m_sName.c_str());
		
		std::map<std::string, std::string>::iterator p;
		for (p = m_attributes.begin(); p != m_attributes.end(); ++p)
		{
			oWriter.WriteString(L" ", 1);
            oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->first.c_str(), p->first.length(), 1).c_str());
			oWriter.WriteString(L"=\"", 2);
            oWriter.WriteEncodeXmlString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(p->second.c_str(), p->second.length(), 1).c_str());
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

			m_pCurrentNode->m_sName = GetName().c_str();
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
                while( true )
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

//	// CXmlNode
//	CXmlNode::CXmlNode(const CXmlNode& oSrc)
//	{
//		m_pBase = NULL;
//		SetBase(oSrc.m_pBase);
//	}

//	std::wstring CXmlNode::private_GetXml()
//	{
//		return private_GetXml(L"");
//	}
//	std::wstring CXmlNode::private_GetXml(const std::wstring& strDefaultValue)
//	{
//		if (NULL == m_pBase)
//			return strDefaultValue;
//		return m_pBase->GetXml();
//	}
//	std::wstring CXmlNode::private_GetXmlFast()
//	{
//		return private_GetXmlFast(L"");
//	}
//	std::wstring CXmlNode::private_GetXmlFast(const std::wstring& strDefaultValue)
//	{
//		if (NULL == m_pBase)
//			return strDefaultValue;
//        CStringWriter oWriter;
//		m_pBase->GetXml(oWriter);
//		return oWriter.GetData();
//	}

//	bool CXmlNode::FromXmlFile(const std::wstring& sFile, bool bRemoveRootNode)
//	{
//		//ignore bRemoveRootNode
//		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
//        bool bRes = m_pDocument->FromFile(sFile);
		
//        if (false == bRes)
//		{
//			delete m_pDocument;
//			return false;
//		}
		
//		m_pDocument->Parse();

//		if (NULL == m_pDocument->m_pNode)
//		{
//			delete m_pDocument;
//			return false;
//		}
//		//присваиваем m_pBase без AddRef, чтобы при удалении CXmlNode начался удаляться m_pBase(после конструктора RefCount==1, а если сделать AddRef то не удалится)
//		m_pBase = m_pDocument->m_pNode;
//		//после Parse все Node из m_pDocument сделали ему AddRef, поэтому можем вызвать Release(потому что напрямую нигде его не используем), а окончательно он удалится после удаления всех Node
//		m_pDocument->Release();

//		return true;
//	}
//    bool CXmlNode::FromXmlFile2(const std::wstring& strXmlFilePath)
//	{
//        return FromXmlFile(strXmlFilePath.GetString()) ? true : false;
//	}
//	bool CXmlNode::FromXmlStringA(const std::string& sString)
//	{
//		CXmlDOMDocument* m_pDocument = new CXmlDOMDocument();
//        bool bRes = m_pDocument->FromStringA(sString);
		
//        if (false == bRes)
//		{
//			delete m_pDocument;
//			return false;
//		}
		
//		m_pDocument->Parse();

//		if (NULL == m_pDocument->m_pNode)
//		{
//			delete m_pDocument;
//			return false;
//		}
//		//присваиваем m_pBase без AddRef, чтобы при удалении CXmlNode начался удаляться m_pBase(после конструктора RefCount==1, а если сделать AddRef то не удалится)
//		m_pBase = m_pDocument->m_pNode;
//		//после Parse все Node из m_pDocument сделали ему AddRef, поэтому можем вызвать Release(потому что напрямую нигде его не используем), а окончательно он удалится после удаления всех Node
//		m_pDocument->Release();
//		return true;
//	}
//	bool CXmlNode::FromXmlString(const std::wstring& sString)
//	{
//		return FromXmlStringA(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sString.c_str(), (LONG)sString.length()));
//	}

//	std::wstring CXmlNode::GetText()
//	{
//		return (IsValid() ? m_pBase->m_sText : L"");
//	}
//	bool CXmlNode::GetTextIfExist(std::wstring& sOutput)
//	{
//		bool bRes = false;
//		if(IsValid() && !m_pBase->m_sText.empty())
//		{
//			sOutput = m_pBase->m_sText;
//			bRes = true;
//		}
//		return bRes;
//	}
//	std::wstring CXmlNode::GetTextExt(const std::wstring& strDefaultValue)
//	{
//		//todo xml:space preserve
//		std::wstring sRes;
//		if(!GetTextIfExist(sRes))
//			sRes = strDefaultValue;
//		return sRes;
//	}
//	std::wstring CXmlNode::GetXml(const std::wstring& strDefaultValue)
//	{
//		return private_GetXmlFast(strDefaultValue);
//	}

//	std::wstring CXmlNode::GetName()
//	{
//		return (IsValid() ? m_pBase->m_sName : L"");
//	}

//    bool CXmlNode::IsValid() const
//	{
//		return (NULL != m_pBase);
//	}
//	void CXmlNode::Clear()
//	{
//		SetBase(NULL);
//	}
//	void CXmlNode::SetBase(CXmlNodeBase* pBase)
//	{
//		CXmlNodeBase* pBaseOld = m_pBase;
//		m_pBase = pBase;
//		if(NULL != m_pBase)
//			m_pBase->AddRef();
//		if (NULL != pBaseOld)
//			pBaseOld->Release();
//	}
//	std::wstring CXmlNode::ReadAttributeBase(const wchar_t* bstrName)
//	{
//		return GetAttribute(std::wstring(bstrName));
//	}
	
//	std::wstring CXmlNode::ReadAttribute(const std::wstring& strAttibuteName)
//	{
//		return GetAttribute(strAttibuteName);
//	}

//	std::string CXmlNode::GetAttributeA(const std::string& sName, const std::string& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.Find(sName);
//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		return pFind->second;
//	}
//	std::string CXmlNode::GetAttributeA(const std::wstring& sName, const std::string& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind =
//			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength()));

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		return pFind->second;
//	}

//	std::wstring CXmlNode::GetAttribute(const std::string& sName, const std::wstring& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.Find(sName);
//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//        return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second, pFind->second.length(), true);
//	}
//	std::wstring CXmlNode::GetAttribute(const std::wstring& sName, const std::wstring& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind =
//            m_pBase->m_attributes.Find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName, sName.l()));

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//        return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second, pFind->second.length(), true);
//	}
//	bool CXmlNode::GetAttributeIfExist(const std::wstring& sName, std::wstring& sOutput)
//	{
//		bool bRes = false;
//		if (IsValid())
//		{
//			std::map<std::string, std::string>::const_iterator pFind =
//				m_pBase->m_attributes.Find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength()));

//			if (pFind != m_pBase->m_attributes.end())
//			{
//                sOutput = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pFind->second, pFind->second.length(), true);
//				bRes = true;
//			}
//		}
//		return bRes;
//	}
//	std::wstring CXmlNode::GetAttribute(const wchar_t* sName, const std::wstring& _default)
//	{
//		return GetAttribute(std::wstring(sName), _default);
//	}

//	std::wstring CXmlNode::GetAttributeBase(const wchar_t* strAttributeName, const std::wstring& strDefaultValue)
//	{
//		return GetAttribute(std::wstring(strAttributeName), strDefaultValue);
//	}

//	int CXmlNode::GetAttributeInt(const std::string& sName, const int& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		if (0 == pFind->second.GetLength())
//			return _default;

//		return atoi(pFind->second.GetString());
//	}
//	int CXmlNode::GetAttributeInt(const std::wstring& sName, const int& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind =
//			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), sName.GetLength()));

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		if (0 == pFind->second.GetLength())
//			return _default;

//		return atoi(pFind->second.GetString());
//	}
//	int CXmlNode::ReadAttributeInt(const std::wstring& str, const int& nDef)
//	{
//		return GetAttributeInt(str, nDef);
//	}
//	double CXmlNode::ReadAttributeDouble(const std::wstring& str, const double& nDef)
//	{
//		return GetAttributeDouble(str, nDef);
//	}
//	double CXmlNode::GetAttributeDouble(const std::string& sName, const double& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind = m_pBase->m_attributes.find(sName);

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		if (0 == pFind->second.GetLength())
//			return _default;

//		return atof(pFind->second.GetString());
//	}
//	double CXmlNode::GetAttributeDouble(const std::wstring& sName, const double& _default)
//	{
//		if (!IsValid())
//			return _default;

//		std::map<std::string, std::string>::const_iterator pFind =
//			m_pBase->m_attributes.find(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.GetString(), (LONG)sName.GetLength()));

//		if (pFind == m_pBase->m_attributes.end())
//			return _default;

//		if (0 == pFind->second.GetLength())
//			return _default;

//		return atof(pFind->second.GetString());
//	}

//	std::wstring CXmlNode::ReadNodeTextBase(const wchar_t* bsName)
//	{
//		return ReadNodeText(std::wstring(bsName));
//	}
//	std::wstring CXmlNode::ReadNodeText(const std::wstring& strName)
//	{
//		return ReadValueString(strName);
//	}
//	std::wstring CXmlNode::GetAttributeOrValue(const std::wstring& strAttributeName, const std::wstring& strDefaultValue)
//	{
//		std::wstring sRes;
//		if(!GetAttributeIfExist(strAttributeName, sRes))
//		{
//			if(!GetTextIfExist(sRes))
//				sRes = strDefaultValue;
//		}
//		return sRes;
//	}
//	std::wstring CXmlNode::ReadValueString(const std::wstring& sName, const std::wstring& nDef)
//	{
//		std::wstring sRes;
//		CXmlNode oTemp;
//		if(GetNode(sName, oTemp))
//			sRes = oTemp.GetText();
//		return sRes;
//	}
//	int CXmlNode::ReadValueInt(const std::wstring& str, const int& nDef)
//	{
//		int nRes = nDef;
//		CXmlNode oTemp;
//		if(GetNode(str, oTemp))
//		{
//			std::wstring sText;
//			if(oTemp.GetTextIfExist(sText))
//				nRes = XmlUtils::GetInteger(sText);
//		}
//		return nRes;
//	}

//    CXmlNode CXmlNode::ReadNode(const TCHAR* strNodeName)
//    {
//        CXmlNode oNode;
//        std::wstring sNodeName = strNodeName;
//        GetNode(sNodeName, oNode);
//        return oNode;
//    }
//	CXmlNode CXmlNode::ReadNode(const std::wstring& strNodeName)
//	{
//		CXmlNode oNode;
//		GetNode(strNodeName, oNode);
//		return oNode;
//	}
//	CXmlNode CXmlNode::ReadNodeNoNS(const std::wstring& strNodeName)
//	{
//		CXmlNode node;
//		if (IsValid())
//		{
//			int nCount = (int)m_pBase->m_nodes.size();
//			for (int i = 0; i < nCount; ++i)
//			{
//				std::wstring str = GetNameNoNS(m_pBase->m_nodes[i]->m_sName.GetBuffer());
//				if (strNodeName == std::wstring(str.c_str()))
//				{
//					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
//					node.SetBase(pBase);
//					break;
//				}
//			}
//		}
//		return node;
//	}

//	CXmlNode CXmlNode::GetNode(const std::wstring& sName)
//	{
//		CXmlNode oNode;
//		GetNode(sName, oNode);
//		return oNode;
//	}
//	bool CXmlNode::GetNode(const std::wstring& sName, CXmlNode& oNode)
//	{
//		bool bRes = false;
//		if (IsValid())
//		{
//			int nCount = (int)m_pBase->m_nodes.size();
//			for (int i = 0; i < nCount; ++i)
//			{
//				if (sName == m_pBase->m_nodes[i]->m_sName)
//				{
//					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
//					oNode.SetBase(pBase);
//					bRes = true;
//					break;
//				}
//			}
//		}
//		return bRes;
//	}
//	CXmlNodes CXmlNode::GetNodes(const std::wstring& sName)
//	{
//		CXmlNodes oNodes;
//		if (IsValid())
//		{
//			bool bGetAll = false;
//			if(_T("*") == sName)
//				bGetAll = true;
//			int nCount = (int)m_pBase->m_nodes.size();
//			for (int i = 0; i < nCount; ++i)
//			{
//				if (bGetAll || sName == m_pBase->m_nodes[i]->m_sName)
//				{
//					CXmlNode oNode;
//					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
//					oNode.SetBase(pBase);
//					oNodes.m_nodes.insert(oNodes.m_nodes.end(), oNode);
//				}
//			}
//		}

//		return oNodes;
//	}
//    bool CXmlNode::GetChilds(CXmlNodes& oXmlNodes)
//	{
//        bool bRes = false;
//		if (IsValid())
//		{
//			int nCount = (int)m_pBase->m_nodes.size();
//			if(nCount > 0)
//			{
//                bRes = true;
//				for (int i = 0; i < nCount; ++i)
//				{
//					CXmlNode oNode;
//					CXmlNodeBase* pBase = m_pBase->m_nodes[i];
//					oNode.SetBase(pBase);
//					oXmlNodes.m_nodes.insert(oXmlNodes.m_nodes.end(), oNode);
//				}
//			}
//		}
//		return bRes;
//	}
//	bool CXmlNode::GetNodes(const std::wstring& sName, CXmlNodes& oNodes)
//	{
//		oNodes = GetNodes(sName);
//		return (0 != oNodes.GetCount());
//	}

//	CXmlNode& CXmlNode::operator=(const CXmlNode& oSrc)
//	{
//		this->SetBase(oSrc.m_pBase);
//		return *this;
//	}
//	std::wstring CXmlNode::GetNamespace(const std::wstring& strNodeName)
//	{
//		int nFind = strNodeName.find(TCHAR(':'));
//		if (-1 == nFind)
//			return std::wstring(_T(""));
//		return strNodeName.Mid(0, nFind);
//	}
//	//std::wstring CXmlNode::GetNameNoNS(const std::wstring& strNodeName)
//	//{
//	//	int nFind= strNodeName.find(TCHAR(':'));
//	//	if (-1 == nFind)
//	//		return strNodeName;
//	//	return strNodeName.Mid(nFind + 1);
//	//}
//	std::wstring CXmlNode::GetNameNoNS(const std::wstring& strNodeName)
//	{
//		int nFind= strNodeName.find(TCHAR(':'));
//		if (-1 == nFind)
//			return strNodeName;
//		return strNodeName.substr(nFind + 1);
//	}
//    // CXmlNodes
//    CXmlNodes::CXmlNodes() : m_nodes()
//    {
//    }
//    bool CXmlNodes::IsValid()
//    {
//        return true;
//    }
//    int CXmlNodes::GetCount()
//    {
//        return (int)m_nodes.size();
//    }
//    bool CXmlNodes::GetAt(int nIndex, CXmlNode& oXmlNode)
//    {
//        if (nIndex < 0 || nIndex >= GetCount())
//            return false;

//        oXmlNode = m_nodes[nIndex];
//        return true;
//    }
}
