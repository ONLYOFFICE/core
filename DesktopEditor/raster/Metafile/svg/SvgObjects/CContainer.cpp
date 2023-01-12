#include "CContainer.h"

#include "CHeader.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CContainer::CContainer(CObjectBase *pParent)
	    : CObjectBase(pParent), m_pHeader(NULL)
	{}

	CContainer::~CContainer()
	{
		Clear();
	}

	bool CContainer::ReadHeader(XmlUtils::CXmlNode &oElement)
	{
		std::wstring wsNodeName = oElement.GetName();

		if (L"svg" != wsNodeName &&
		    L"g"   != wsNodeName &&
		    L"xml" != wsNodeName)
			return false;

		Clear();

		m_pHeader = new CHeader(m_pParent);
		return m_pHeader->ReadFromXmlNode(oElement);
	}

	bool CContainer::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		return false;
	}

	bool CContainer::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CSvgParser &oParser, CSvgFile* pFile)
	{
		return oParser.LoadFromXmlNode(oNode, this, pFile);
	}

	bool CContainer::Draw(IRenderer *pRenderer, const CGeneralStyle *pBaseStyle) const
	{
		if (NULL == pRenderer)
			return false;

		for (CObjectBase* pObject : m_arObjects)
			pObject->Draw(pRenderer, pBaseStyle);

		return true;
	}

	void CContainer::Clear()
	{
		RELEASEOBJECT(m_pHeader);
		m_arObjects.clear();
	}

	bool CContainer::Empty() const
	{
		return m_arObjects.empty();
	}

	CHeader *CContainer::GetHeader() const
	{
		return m_pHeader;
	}

	void CContainer::AddObject(CObjectBase *pObject)
	{
		if (NULL != pObject)
			m_arObjects.push_back(pObject);
	}

	void CContainer::ApplyStyle(IRenderer *pRenderer, int &nTypePath, const CGeneralStyle *pBaseStyle) const
	{

	}
}
