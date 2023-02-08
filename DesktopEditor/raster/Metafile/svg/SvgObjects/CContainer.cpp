#include "CContainer.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CContainer::CContainer(CObjectBase *pParent)
	    : CObjectBase(pParent)
	{}

	CContainer::CContainer(double dWidth, double dHeight, CObjectBase *pParent)
	    : CObjectBase(pParent), m_oWidth(dWidth), m_oHeight(dHeight)
	{}

	CContainer::~CContainer()
	{
		Clear();
	}

	void CContainer::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);
	}

	bool CContainer::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		std::wstring wsNodeName = oNode.GetName();

		if (L"svg" != wsNodeName &&
		    L"g"   != wsNodeName &&
		    L"xml" != wsNodeName)
			return false;

		Clear();

		SaveNodeData(oNode);

		return true;
	}

	bool CContainer::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		for (CObjectBase* pObject : m_arObjects)
			pObject->Draw(pRenderer);

		return true;
	}

	void CContainer::Clear()
	{
		m_arObjects.clear();
	}

	bool CContainer::Empty() const
	{
		return m_arObjects.empty();
	}

	SvgDigit CContainer::GetX() const
	{
		return m_oX;
	}

	NSCSS::NSProperties::CDigit CContainer::GetY() const
	{
		return m_oY;
	}

	SvgDigit CContainer::GetWidth() const
	{
		return m_oWidth;
	}

	NSCSS::NSProperties::CDigit CContainer::GetHeight() const
	{
		return m_oHeight;
	}

	void CContainer::AddObject(CObjectBase *pObject)
	{
		if (NULL != pObject)
			m_arObjects.push_back(pObject);
	}

	void CContainer::ApplyStyle(IRenderer *pRenderer, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{

	}
}
