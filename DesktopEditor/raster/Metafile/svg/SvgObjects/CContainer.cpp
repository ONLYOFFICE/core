#include "CContainer.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CContainer::CContainer(CObjectBase *pParent)
	    : CObjectBase(pParent)
	{}

	CContainer::CContainer(double dWidth, double dHeight, CObjectBase *pParent)
	    : CObjectBase(pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	CContainer::~CContainer()
	{
		Clear();
	}

	void CContainer::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oWindow.m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oWindow.m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oWindow.m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oWindow.m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"viewBox"))
		{
			std::vector<double> arValues = StrUtils::ReadDoubleValues(mAttributes.at(L"viewBox"));

			m_oViewBox.m_oX      = arValues[0];
			m_oViewBox.m_oY      = arValues[1];
			m_oViewBox.m_oWidth  = arValues[2];
			m_oViewBox.m_oHeight = arValues[3];
		}
	}

	bool CContainer::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		std::wstring wsNodeName = oNode.GetName();

		if (L"svg"     != wsNodeName &&
		    L"g"       != wsNodeName &&
		    L"xml"     != wsNodeName &&
		    L"defs"    != wsNodeName &&
		    L"pattern" != wsNodeName)
			return false;

		Clear();

		SaveNodeData(oNode);

		return true;
	}

	bool CContainer::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		if (NULL == pRenderer)
			return false;

		for (CObjectBase* pObject : m_arObjects)
			pObject->Draw(pRenderer, pDefs);

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

	TRect CContainer::GetWindow() const
	{
		return m_oWindow;
	}

	TRect CContainer::GetViewBox() const
	{
		return m_oViewBox;
	}

	void CContainer::AddObject(CObjectBase *pObject)
	{
		if (NULL != pObject)
			m_arObjects.push_back(pObject);
	}

	void CContainer::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{

	}

	TBounds CContainer::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.}, oTempBounds;

		for (const CObjectBase* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds();
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}
}
