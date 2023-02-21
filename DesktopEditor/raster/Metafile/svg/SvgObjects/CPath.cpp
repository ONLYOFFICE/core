#include "CPath.h"

#include <algorithm>

namespace SVG
{
	CPath::CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	CPath::~CPath()
	{
		for (IPathElement* pPathElement : m_arElements)
			delete pPathElement;
	}

	void CPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"d"))
			ReadFromString(mAttributes.at(L"d"));
	}

	bool CPath::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_arElements.empty())
			return false;

		StartPath(pRenderer, pDefs, bIsClip);

		for (IPathElement* oElement : m_arElements)
			oElement->Draw(pRenderer);

		EndPath(pRenderer, pDefs, bIsClip);

		return true;
	}

	void CPath::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath);
	}

	TBounds CPath::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.}, oTempBounds;

		for (const IPathElement* oElement : m_arElements)
		{
			oTempBounds = oElement->GetBounds();

			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

	void CPath::ReadFromString(const std::wstring &wsValue)
	{
		std::wstring::const_iterator oFirstPos = wsValue.begin();
		std::wstring::const_iterator oSecondPos = oFirstPos;

		IPathElement *pLastElement = NULL;
		IPathElement *pMoveElement = NULL;

		while (true)
		{
			oFirstPos = std::find_if(oSecondPos, wsValue.end(), iswalpha );

			if (wsValue.end() == oFirstPos)
				break;

			oSecondPos = std::find_if(oFirstPos + 1, wsValue.end(), iswalpha );

			switch(*oFirstPos)
			{
				case L'M':
				case L'm':
				{
					IPathElement *pElement = new CMoveElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;
					pMoveElement = pElement;

					break;
				}
				case L'L':
				case L'l':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CLineElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'H':
				case L'h':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CHLineElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'V':
				case L'v':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CVLineElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'C':
				case L'c':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CCBezierElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'S':
				case L's':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CSBezierElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'Q':
				case L'q':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CQBezierElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'T':
				case L't':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CTBezierElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'A':
				case L'a':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CArcElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pLastElement);

					m_arElements.push_back(pElement);

					pLastElement = pElement;

					break;
				}
				case L'Z':
				case L'z':
				{
					if (NULL == pMoveElement)
						return;

					IPathElement *pElement = new CCloseElement();
					pElement->ReadFromString(std::wstring(oFirstPos + 1, oSecondPos), iswlower(*oFirstPos), pMoveElement);

					m_arElements.push_back(pElement);

					pLastElement = pMoveElement;

					break;
				}
			}

			oFirstPos = oSecondPos;
		}
	}

}
