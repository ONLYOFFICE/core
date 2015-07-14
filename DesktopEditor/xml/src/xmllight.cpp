#include "../include/xmlutils.h"
#include "./xmllight_private.h"

namespace XmlUtils
{
	CXmlLiteReader::CXmlLiteReader()
	{
		m_pInternal = new CXmlLiteReader_Private();
	}
	CXmlLiteReader::~CXmlLiteReader()
	{
		delete m_pInternal;
	}

	void CXmlLiteReader::Clear()
	{
		m_pInternal->Clear();
	}
	bool CXmlLiteReader::IsValid()
	{
		return m_pInternal->IsValid();
	}

	bool CXmlLiteReader::FromFile(const wchar_t* sFilePath)
	{
		return m_pInternal->FromFile(sFilePath);
	}
	bool CXmlLiteReader::FromFile(const std::wstring& sFilePath)
	{
		return m_pInternal->FromFile(sFilePath);
	}
	bool CXmlLiteReader::FromString(const wchar_t* sXml)
	{
		return m_pInternal->FromString(sXml);
	}
	bool CXmlLiteReader::FromString(const std::wstring& sXml)
	{
		return m_pInternal->FromString(sXml);
	}
	bool CXmlLiteReader::FromStringA(const std::string& sXml)
	{
		return m_pInternal->FromStringA(sXml);
	}

	bool CXmlLiteReader::Read(XmlNodeType &oNodeType)
	{
		return m_pInternal->Read(oNodeType);
	}
	bool CXmlLiteReader::ReadNextNode()
	{
		return m_pInternal->ReadNextNode();
	}
	bool CXmlLiteReader::ReadNextSiblingNode(int nDepth)
	{
		return m_pInternal->ReadNextSiblingNode(nDepth);
	}
	bool CXmlLiteReader::ReadNextSiblingNode2(int nDepth)
	{
		return m_pInternal->ReadNextSiblingNode2(nDepth);
	}
	bool CXmlLiteReader::ReadTillEnd(int nDepth)
	{
		return m_pInternal->ReadTillEnd(nDepth);
	}

	std::wstring CXmlLiteReader::GetName()
	{
		return m_pInternal->GetName();
	}
	std::string CXmlLiteReader::GetNameA()
	{
		return m_pInternal->GetNameA();
	}
	int CXmlLiteReader::GetDepth()
	{
		return m_pInternal->GetDepth();
	}
	bool CXmlLiteReader::IsEmptyNode()
	{
		return m_pInternal->IsEmptyNode();
	}

	std::wstring CXmlLiteReader::GetText()
	{
		return m_pInternal->GetText();
	}
	std::string CXmlLiteReader::GetTextA()
	{
		return m_pInternal->GetTextA();
	}

	std::wstring CXmlLiteReader::GetText2()
	{
		return m_pInternal->GetText2();
	}
	std::string CXmlLiteReader::GetText2A()
	{
		return m_pInternal->GetText2A();
	}

	std::wstring CXmlLiteReader::GetOuterXml()
	{
		return m_pInternal->GetOuterXml();
	}
	std::wstring CXmlLiteReader::GetInnerXml()
	{
		return m_pInternal->GetInnerXml();
	}

	int CXmlLiteReader::GetAttributesCount()
	{
		return m_pInternal->GetAttributesCount();
	}
	bool CXmlLiteReader::MoveToFirstAttribute()
	{
		return m_pInternal->MoveToFirstAttribute();
	}
	bool CXmlLiteReader::MoveToNextAttribute()
	{
		return m_pInternal->MoveToNextAttribute();
	}
	bool CXmlLiteReader::MoveToElement()
	{
		return m_pInternal->MoveToElement();
	}

	bool CXmlLiteReader::IsEmptyElement()
	{
		return m_pInternal->IsEmptyElement();
	}
}