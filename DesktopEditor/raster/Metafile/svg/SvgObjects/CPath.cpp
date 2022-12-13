#include "CPath.h"

#include <algorithm>

namespace SVG
{
	CPath::CPath(CObjectBase *pParent)
	{

	}

	CPath::~CPath()
	{

	}

	bool CPath::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		std::wstring wsPoints = oNode.GetAttribute(L"d");

		if (wsPoints.empty())
			return false;

		ReadFromString(wsPoints);

		UpdateStyle(oNode);

		return true;
	}

	bool CPath::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer || m_arElements.empty())
			return false;

		ApplyStyle(pRenderer);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand ( c_nPathType );

		PathPoint oLastPoint = {0, 0};

		for (const CPathElement& oElement : m_arElements)
			oElement.Draw(pRenderer, oLastPoint);

		pRenderer->DrawPath (c_nWindingFillMode/* | c_nStroke*/);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd();

		return true;
	}

	void CPath::ApplyStyle(IRenderer *pRenderer)
	{
		if (NULL == pRenderer)
			return;

		pRenderer->put_BrushColor1(m_oStyle.GetFillL());
//		pRenderer->put_PenColor(m_oStyle.GetStrokeColor());
	}

	void CPath::ReadFromString(const std::wstring &wsValue)
	{
		std::wstring::const_iterator oFirstPos = wsValue.begin();
		std::wstring::const_iterator oSecondPos = oFirstPos;

		std::wstring wsMoveValue = L"";

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
					wsMoveValue = std::wstring(oFirstPos + 1, oSecondPos);

					CPathElement oElement(EPathElement::Move, iswlower(*oFirstPos));
					oElement.ReadFromString(wsMoveValue);

					m_arElements.push_back(oElement);

					break;
				}
				case L'L':
				case L'l':
				{
					CPathElement oElement(EPathElement::Line, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'H':
				case L'h':
				{
					CPathElement oElement(EPathElement::HorizantalL, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'V':
				case L'v':
				{
					CPathElement oElement(EPathElement::VerticalL, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'C':
				case L'c':
				{
					CPathElement oElement(EPathElement::Curve, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'S':
				case L's':
				{
					CPathElement oElement(EPathElement::SBezier, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'Q':
				case L'q':
				{
					CPathElement oElement(EPathElement::QBezier, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'T':
				case L't':
				{
					CPathElement oElement(EPathElement::QBezier, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'A':
				case L'a':
				{
					CPathElement oElement(EPathElement::Arc, iswlower(*oFirstPos));
					oElement.ReadFromString(std::wstring(oFirstPos + 1, oSecondPos));

					m_arElements.push_back(oElement);

					break;
				}
				case L'Z':
				case L'z':
				{
					CPathElement oElement(EPathElement::Close, iswlower(*oFirstPos));

					m_arElements.push_back(oElement);

					return;
				}
			}

			oFirstPos = oSecondPos;
		}
	}

}
