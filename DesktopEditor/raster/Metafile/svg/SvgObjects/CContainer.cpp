#include "CContainer.h"

#include "../CSvgFile.h"
#include "../CSvgParser.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	CGraphicsContainer::CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	void CGraphicsContainer::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
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

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer)
			return false;

		for (const CSvgGraphicsObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pDefs, bIsClip);

		return true;
	}

	TRect CGraphicsContainer::GetWindow() const
	{
		return m_oWindow;
	}

	TRect CGraphicsContainer::GetViewBox() const
	{
		return m_oViewBox;
	}

	CSvgGraphicsObject *CGraphicsContainer::GetObject(const std::wstring &wsId) const
	{
		if (wsId.empty() || m_arObjects.empty())
			return NULL;

		std::wstring wsNewId = wsId;

		size_t unFound = wsNewId.find(L'#');

		if (std::wstring::npos != unFound)
			wsNewId.erase(0, unFound + 1);

		std::transform(wsNewId.begin(), wsNewId.end(), wsNewId.begin(), std::towlower);

		std::vector<CSvgGraphicsObject*>::const_iterator oFound =std::find_if(m_arObjects.begin(), m_arObjects.end(), [&wsNewId](CSvgGraphicsObject* pObject){ if (wsNewId == pObject->GetId()) return true; else return false;});

		if (m_arObjects.end() != oFound)
			return *oFound;

		return NULL;
	}

	void CGraphicsContainer::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{}

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		for (const CSvgGraphicsObject* pObject : m_arObjects)
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
