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

#ifndef CTRLGENERALSHAPE_H
#define CTRLGENERALSHAPE_H

#include "../HWPElements/HWPRecordBorderFill.h"
#include "CtrlObjElement.h"
#include "HWPPargraph.h"

namespace HWP
{
enum class EShapeType
{
	GeneralShape,
	Arc,
	ConnectLine,
	Curve,
	Ellipse,
	Line,
	Ole,
	Pic,
	Polygon,
	Rect,
	TextArt,
	Video,
	EqEdit,
	Container
};

class CCtrlGeneralShape : public CCtrlObjElement
{
	CHWPPargraph* m_pParent;

	int m_nLineColor;
	int m_nLineThick;
	ELineArrowStyle m_eLineHead;
	ELineArrowStyle m_eLineTail;
	ELineArrowSize m_eLineHeadSz;
	ELineArrowSize m_eLineTailSz;
	ELineStyle2 m_eLineStyle;
	HWP_BYTE m_chOutline;

	int m_nFillType;
	CFill* m_pFill;

	short m_shLeftSpace;
	short m_shRightSpace;
	short m_shTopSpace;
	short m_shBottomSpace;
	int m_nMaxTxtWidth;

	void InitData();

	friend class CCtrlShapeEllipse;
	friend class CCtrlShapePolygon;
	friend class CCtrlShapeRect;

	void ReadSubList(CXMLReader& oReader);
public:
	CCtrlGeneralShape();
	CCtrlGeneralShape(const HWP_STRING& sCtrlID);
	CCtrlGeneralShape(const CCtrlGeneralShape& oGeneralShape);
	CCtrlGeneralShape(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlGeneralShape(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);
	virtual ~CCtrlGeneralShape();

	ECtrlObjectType GetCtrlType() const override;
	virtual EShapeType GetShapeType() const;

	void ParseChildren(CXMLReader& oReader, EHanType eType);

	void SetParent(CHWPPargraph* pParent);
	CHWPPargraph* GetParent();

	const CFill* GetFill() const;

	ELineStyle2 GetLineStyle() const;
	int GetLineColor() const;
	int GetLineThick() const;

	ELineArrowStyle GetLineHeadStyle() const;
	ELineArrowSize  GetLineHeadSize () const;
	ELineArrowStyle GetLineTailStyle() const;
	ELineArrowSize  GetLineTailSize () const;

	static CCtrlGeneralShape* Parse(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderApend(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLGENERALSHAPE_H
