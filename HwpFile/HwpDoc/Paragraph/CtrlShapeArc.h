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

#ifndef CTRLSHAPEARC_H
#define CTRLSHAPEARC_H

#include "CtrlShapeEllipse.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeArc : public CCtrlGeneralShape
{
	EArcType m_eType;
	int m_nCenterX;
	int m_nCenterY;
	int m_nAxixX1;
	int m_nAxixY1;
	int m_nAxixX2;
	int m_nAxixY2;

	void ReadFromHWPX(CXMLReader& oReader);
	void ReadFromHWPML(CXMLReader& oReader);
public:
	CCtrlShapeArc();
	CCtrlShapeArc(const HWP_STRING& sCtrlID);
	CCtrlShapeArc(const CCtrlGeneralShape& oShape);
	CCtrlShapeArc(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeArc(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);

	EShapeType GetShapeType() const override;

	TPoint GetCenterPoint() const;
	TPoint GetFirstPoint()  const;
	TPoint GetSecondPoint() const;

	static int ParseElement(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEARC_H
