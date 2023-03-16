#include "CPath.h"

#include <algorithm>

namespace SVG
{
    #define LASTELEMENT(array) (array.empty()) ? NULL : array.back()

	CPath::CPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		ReadFromString(oNode.GetAttribute(L"d"));
	}

	CPath::~CPath()
	{
		for (IPathElement* pPathElement : m_arElements)
			delete pPathElement;
	}

	void CPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CPath::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_arElements.empty())
			return false;

		StartPath(pRenderer, pDefs, bIsClip);

		for (const IPathElement* oElement : m_arElements)
			oElement->Draw(pRenderer);

		EndPath(pRenderer, pDefs, bIsClip);

		return true;
	}

	CPath *CPath::Copy() const
	{
		CPath* pNew = new CPath(*this);
		pNew->m_arElements.clear();

		for(const IPathElement* oElement : m_arElements)
			pNew->m_arElements.push_back(oElement->Copy());

		return pNew;
	}

	IPathElement *CPath::operator[](int nIndex) const
	{
		if (m_arElements.empty())
			return NULL;

		return m_arElements[(nIndex >= 0) ? nIndex : m_arElements.size() + nIndex];
	}

	void CPath::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath, true);
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

		IPathElement *pMoveElement = NULL;
		IPathElement *pElement     = NULL;

		while (true)
		{
			oFirstPos = std::find_if(oSecondPos, wsValue.end(), iswalpha );

			if (wsValue.end() == oFirstPos)
				break;

			oSecondPos = std::find_if(oFirstPos + 1, wsValue.end(), iswalpha );

			std::vector<double> arValues = StrUtils::ReadDoubleValues(std::wstring(oFirstPos + 1, oSecondPos));

			switch(*oFirstPos)
			{
				case L'M':
				case L'm':
				{
					pElement = IPathElement::CreateFromArray<CMoveElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));

					if (NULL != pElement)
					{
						pMoveElement = pElement;

						if (arValues.size() > 1)
						{
							pElement = IPathElement::CreateFromArray<CLineElement>(arValues, iswlower(*oFirstPos), pMoveElement);
							if (NULL != pElement)
								m_arElements.push_back(pMoveElement);
						}
					}

					break;
				}
				case L'L':
				case L'l':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CLineElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'H':
				case L'h':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CHLineElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'V':
				case L'v':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CVLineElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'C':
				case L'c':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CCBezierElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'S':
				case L's':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CSBezierElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'Q':
				case L'q':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CQBezierElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'T':
				case L't':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CTBezierElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'A':
				case L'a':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CArcElement>(arValues, iswlower(*oFirstPos), LASTELEMENT(m_arElements));
					break;
				}
				case L'Z':
				case L'z':
				{
					if (NULL == pMoveElement)
						return;

					pElement = IPathElement::CreateFromArray<CCloseElement>(arValues, iswlower(*oFirstPos), pMoveElement);
					if (NULL != pElement)
						pMoveElement = NULL;
					break;
				}
			}

			if (NULL != pElement)
				m_arElements.push_back(pElement);

			oFirstPos = oSecondPos;
		}
	}

}
