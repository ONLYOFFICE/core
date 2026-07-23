/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "PathObject.h"

#include "../Utils/Utils.h"
#include "../Types/DrawParam.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

namespace OFD
{
CPathObject::CPathObject(CXmlReader& oReader)
    : IPageBlock(oReader), CGraphicUnit(oReader),
	  m_bStroke(true), m_bFill(false), m_eRule(ERule::NonZero),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr)
{
	if (oReader.IsEmptyElement())
		return;

	if (oReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oReader.GetName();

			if (L"Stroke" == wsAttributeName)
				m_bStroke = oReader.GetBoolean(true);
			else if (L"Fill" == wsAttributeName)
				m_bFill = oReader.GetBoolean(true);
			else if (L"Rule" == wsAttributeName)
			{
				if (L"Even-odd" == oReader.GetText())
					m_eRule = ERule::Even_Odd;
				else
					m_eRule = ERule::NonZero;
			}
		} while (oReader.MoveToNextAttribute());

		oReader.MoveToElement();
	}

	const int nDepth = oReader.GetDepth();
	std::string sNodeName;

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oReader.GetNameA();
		if ("ofd:FillColor" == sNodeName)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new CColor(oReader);
		}
		else if ("ofd:StrokeColor" == sNodeName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oReader);
		}
		else if ("ofd:AbbreviatedData" == sNodeName)
		{
			std::vector<std::string> arValues{Split(oReader.GetText2A(), ' ')};

			std::vector<std::string>::const_iterator itElement = arValues.cbegin();

			char chElementName;

			while (arValues.cend() != itElement)
			{
				if (arValues.front().length() != 1)
				{
					++itElement;
					continue;
				}

				chElementName = (*itElement)[0];
				++itElement;

				switch (chElementName)
				{
					case 'S':
					{
						AddElement(CStartElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'M':
					{
						AddElement(CMoveElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'L':
					{
						AddElement(CLineElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'Q':
					{
						AddElement(CBezierCurve2Element::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'B':
					{
						AddElement(CBezierCurveElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'A':
					{
						AddElement(CArcElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'C':
					{
						AddElement(new CCloseElement());
						break;
					}
					default:
						continue;
				}
			}
		}
		else
			ReadChildren(oReader);
	}
}

CPathObject::~CPathObject()
{
	for (const IPathElement* pElement : m_arElements)
		delete pElement;
}

void CPathObject::AddElement(const IPathElement* pElement)
{
	if (nullptr != pElement)
		m_arElements.push_back(pElement);
}

void CPathObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer || m_arElements.empty())
		return;

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

	pRenderer->BeginCommand(c_nPathType);
	pRenderer->PathCommandStart();

	for (const IPathElement* pElement : m_arElements)
		pElement->Draw(pRenderer);

	int nEndType = -1;

	if (m_bStroke)
		nEndType = c_nStroke;

	if (m_bFill)
	{
		switch (m_eRule)
		{
			case ERule::NonZero:
			{
				nEndType = (-1 == nEndType ? c_nWindingFillMode : nEndType | c_nWindingFillMode);
				break;
			}
			case ERule::Even_Odd:
			{
				nEndType = (-1 == nEndType ? c_nEvenOddFillMode : nEndType | c_nEvenOddFillMode);
				break;
			}
		}
	}

	const CRes* pPublicRes{oCommonData.GetPublicRes()};
	std::vector<const CDrawParam*> arDrawParams{pPublicRes->GetDrawParams()};

	if (m_bFill)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);

		if (nullptr != m_pFillColor)
		{
			pRenderer->put_BrushColor1(m_pFillColor->ToInt(pPublicRes));
			pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());
		}
		else
		{
			pRenderer->put_BrushColor1(0);

			if (EPageType::TemplatePage == ePageType)
				for (const CDrawParam* pDrawParam : arDrawParams)
					if (pDrawParam->ApplyFillColor(pRenderer, pPublicRes))
						break;
		}
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);

	if(m_bStroke)
	{
		if (nullptr != m_pStrokeColor)
		{
			pRenderer->put_PenColor(m_pStrokeColor->ToInt(pPublicRes));
			pRenderer->put_PenAlpha(m_pStrokeColor->GetAlpha());
		}
		else
		{
			pRenderer->put_PenColor(0);

			if (EPageType::TemplatePage == ePageType)
				for (const CDrawParam* pDrawParam : arDrawParams)
					if (pDrawParam->ApplyStrokeColor(pRenderer, pPublicRes))
						break;
		}
	}
	else
		pRenderer->put_PenSize(0.);

	if (-1 != nEndType)
		pRenderer->DrawPath(nEndType);

	pRenderer->PathCommandEnd();
	pRenderer->EndCommand(c_nPathType);

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
}

#ifdef BUILDING_WASM_MODULE
void CPathObject::GetLinks(NSWasm::CData& oRes) const
{
	CGraphicUnit::GetLinks(oRes);
}
#endif

CStartElement::CStartElement()
	: m_dX(0.), m_dY(0.)
{}

IPathElement* CStartElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CStartElement *pElement = new CStartElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

void CStartElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandMoveTo(m_dX, m_dY);
}

CMoveElement::CMoveElement()
	: m_dX(0.), m_dY(0.)
{}

CMoveElement* CMoveElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CMoveElement *pElement = new CMoveElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CMoveElement* CMoveElement::ReadFromNode(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	CMoveElement* pMove = new CMoveElement();

	if (nullptr == pMove)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	do
	{
		if ("Point1" == oReader.GetNameA())
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pMove->m_dX = arDatas[0];
			pMove->m_dY = arDatas[1];

			break;
		}
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pMove;
}

void CMoveElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandMoveTo(m_dX, m_dY);
}

CLineElement::CLineElement()
	: m_dX(0.), m_dY(0.)
{}

CLineElement* CLineElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CLineElement *pElement = new CLineElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CLineElement* CLineElement::ReadFromNode(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	CLineElement* pLine = new CLineElement();

	if (nullptr == pLine)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	do
	{
		if ("Point1" == oReader.GetNameA())
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pLine->m_dX = arDatas[0];
			pLine->m_dY = arDatas[1];

			break;
		}
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pLine;
}

void CLineElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandLineTo(m_dX, m_dY);
}

CBezierCurve2Element::CBezierCurve2Element()
	: m_dX1(0.), m_dY1(0.), m_dX2(0.), m_dY2(0.)
{}

CBezierCurve2Element* CBezierCurve2Element::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 4)
		return nullptr;

	CBezierCurve2Element *pElement = new CBezierCurve2Element();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX1) && StringToDouble(*itBegin++, pElement->m_dY1) &&
	    StringToDouble(*itBegin++, pElement->m_dX2) && StringToDouble(*itBegin++, pElement->m_dY2))
		return pElement;

	delete pElement;
	return nullptr;
}

CBezierCurve2Element* CBezierCurve2Element::ReadFromNode(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	CBezierCurve2Element* pBezierCurve2 = new CBezierCurve2Element();

	if (nullptr == pBezierCurve2)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	do
	{
		if ("Point1" == oReader.GetNameA())
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pBezierCurve2->m_dX1 = arDatas[0];
			pBezierCurve2->m_dY1 = arDatas[1];
		}
		else if ("Point2" == oReader.GetNameA())
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pBezierCurve2->m_dX2 = arDatas[0];
			pBezierCurve2->m_dY2 = arDatas[1];
		}
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pBezierCurve2;
}

void CBezierCurve2Element::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	double dX = 0, dY = 0;
	pRenderer->PathCommandGetCurrentPoint(&dX, &dY);
	pRenderer->PathCommandCurveTo(dX, dY, m_dX1, m_dY1, m_dX2, m_dY2);
}

CBezierCurveElement::CBezierCurveElement()
	: m_dX1(0.), m_dY1(0.), m_dX2(0.), m_dY2(0.), m_dX3(0.), m_dY3(0.)
{}

CBezierCurveElement* CBezierCurveElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 6)
		return nullptr;

	CBezierCurveElement *pElement = new CBezierCurveElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX1) && StringToDouble(*itBegin++, pElement->m_dY1) &&
	    StringToDouble(*itBegin++, pElement->m_dX2) && StringToDouble(*itBegin++, pElement->m_dY2) &&
	    StringToDouble(*itBegin++, pElement->m_dX3) && StringToDouble(*itBegin++, pElement->m_dY3))
		return pElement;

	delete pElement;
	return nullptr;
}

CBezierCurveElement* CBezierCurveElement::ReadFromNode(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	CBezierCurveElement* pBezierCurve = new CBezierCurveElement();

	if (nullptr == pBezierCurve)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	std::string sName;

	do
	{
		sName = oReader.GetNameA();

		if ("Point1" == sName)
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pBezierCurve->m_dX1 = arDatas[0];
			pBezierCurve->m_dY1 = arDatas[1];
		}
		else if ("Point2" == sName)
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pBezierCurve->m_dX2 = arDatas[0];
			pBezierCurve->m_dY2 = arDatas[1];
		}
		else if ("Point2" == sName)
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pBezierCurve->m_dX3 = arDatas[0];
			pBezierCurve->m_dY3 = arDatas[1];
		}
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pBezierCurve;
}

void CBezierCurveElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandCurveTo(m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3);
}

CArcElement::CArcElement()
	: m_dRadiusX(0.), m_dRadiusY(0.), m_dAngle(0.), m_bLarge(false), m_bSweep(false), m_dX(0.), m_dY(0.)
{}

CArcElement* CArcElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 7)
		return nullptr;

	CArcElement *pElement = new CArcElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble (*itBegin++, pElement->m_dRadiusX) && StringToDouble (*itBegin++, pElement->m_dRadiusY) &&
	    StringToDouble (*itBegin++, pElement->m_dAngle)   && StringToBoolean(*itBegin++, pElement->m_bLarge)   &&
	    StringToBoolean(*itBegin++, pElement->m_bSweep)   && StringToDouble (*itBegin++, pElement->m_dX)       &&
	    StringToDouble (*itBegin++, pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CArcElement* CArcElement::ReadFromNode(CXmlReader& oReader)
{
	if (!oReader.MoveToFirstAttribute())
		return nullptr;

	CArcElement* pArc = new CArcElement();

	if (nullptr == pArc)
	{
		oReader.MoveToElement();
		return nullptr;
	}

	std::string sName;

	do
	{
		sName = oReader.GetNameA();

		if ("SweepDirection" == sName)
			pArc->m_bSweep = oReader.GetBoolean(true);
		else if ("LargeArc" == sName)
			pArc->m_bLarge = oReader.GetBoolean(true);
		else if ("RotationAngle" == sName)
			pArc->m_dAngle = oReader.GetDouble(true);
		else if ("EllipseSize" == sName)
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pArc->m_dRadiusX = arDatas[0];
			pArc->m_dRadiusY = arDatas[1];
		}
		else if ("EndPoint" == sName)
		{
			const std::vector<double> arDatas{oReader.GetArrayDoubles(true)};

			if (2 > arDatas.size())
				continue;

			pArc->m_dX = arDatas[0];
			pArc->m_dY = arDatas[1];
		}
	}while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	return pArc;
}

void CArcElement::Draw(IRenderer* pRenderer) const
{
	// if (nullptr != pRenderer)
		// pRenderer->PathCommandArcTo(m_dX, m_dY, m_dRadiusX * 2., m_dRadiusY * 2., )
}

CCloseElement::CCloseElement()
{}

void CCloseElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandClose();
}
}
