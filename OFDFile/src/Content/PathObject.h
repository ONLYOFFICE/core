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

#ifndef PATHOBJECT_H
#define PATHOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/Color.h"

namespace OFD
{
class IPathElement
{
public:
	IPathElement(){};
	virtual ~IPathElement(){};

	static IPathElement* ReadFromArray(std::vector<std::string>& arValues) { return nullptr; };
	virtual void Draw(IRenderer* pRenderer) const = 0;
};

class CStartElement : public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CStartElement();
	static IPathElement* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	void Draw(IRenderer* pRenderer) const override;
};

class CMoveElement : public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CMoveElement();
	static CMoveElement* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	static CMoveElement* ReadFromNode(CXmlReader& oReader);
	virtual void Draw(IRenderer* pRenderer) const override;
};

class CLineElement :  public IPathElement
{
	double m_dX;
	double m_dY;
public:
	CLineElement();
	static CLineElement* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	static CLineElement* ReadFromNode(CXmlReader& oReader);
	void Draw(IRenderer* pRenderer) const override;
};

class CBezierCurve2Element : public IPathElement
{
	double m_dX1;
	double m_dY1;
	double m_dX2;
	double m_dY2;
public:
	CBezierCurve2Element();
	static CBezierCurve2Element* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	static CBezierCurve2Element* ReadFromNode(CXmlReader& oReader);
	void Draw(IRenderer* pRenderer) const override;
};

class CBezierCurveElement : public IPathElement
{
	double m_dX1;
	double m_dY1;
	double m_dX2;
	double m_dY2;
	double m_dX3;
	double m_dY3;
public:
	CBezierCurveElement();
	static CBezierCurveElement* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	static CBezierCurveElement* ReadFromNode(CXmlReader& oReader);
	void Draw(IRenderer* pRenderer) const override;
};

class CArcElement : public IPathElement
{
	double m_dRadiusX;
	double m_dRadiusY;
	double m_dAngle;
	bool   m_bLarge;
	bool   m_bSweep;
	double m_dX;
	double m_dY;
public:
	CArcElement();
	static CArcElement* ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd);
	static CArcElement* ReadFromNode(CXmlReader& oReader);
	void Draw(IRenderer* pRenderer) const override;
};

class CCloseElement : public IPathElement
{
public:
	CCloseElement();
	void Draw(IRenderer* pRenderer) const override;
};

class CPathObject : public IPageBlock, public CGraphicUnit
{
	bool m_bStroke;
	bool m_bFill;

	enum class ERule
	{
		NonZero,
		Even_Odd
	} m_eRule;

	CColor* m_pFillColor;
	CColor* m_pStrokeColor;

	std::vector<const IPathElement*> m_arElements;

	void AddElement(const IPathElement* pElement);
public:
	CPathObject(CXmlReader& oReader);
	~CPathObject();

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const override;

	#ifdef BUILDING_WASM_MODULE
	virtual void GetLinks(NSWasm::CData& oRes) const override;
	#endif
};
}

#endif // PATHOBJECT_H
