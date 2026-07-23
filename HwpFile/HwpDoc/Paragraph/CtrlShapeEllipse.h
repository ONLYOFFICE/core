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

#ifndef CTRLSHAPEELLIPSE_H
#define CTRLSHAPEELLIPSE_H

#include "CtrlGeneralShape.h"

namespace HWP
{
enum class EArcType
{
	NORMAL,
	PIE,
	CHORD
};

EArcType GetArcType(int nValue);
EArcType GetArcType(const std::string &sValue, EHanType eType);

class CCtrlShapeEllipse : public CCtrlGeneralShape
{
	bool m_bIntervalDirty;
	bool m_bHasArcProperty;
	EArcType m_eArcType;
	int m_nCenterX;
	int m_nCenterY;
	int m_nAxixX1;
	int m_nAxixY1;
	int m_nAxixX2;
	int m_nAxixY2;
	int m_nStartX1;
	int m_nStartY1;
	int m_nEndX1;
	int m_nEndY1;
	int m_nStartX2;
	int m_nStartY2;
	int m_nEndX2;
	int m_nEndY2;

	void ReadFromHWPX(CXMLReader& oReader);
	void ReadFromHWPML(CXMLReader& oReader);
public:
	CCtrlShapeEllipse();
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID);
	CCtrlShapeEllipse(const CCtrlGeneralShape& oShape);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEELLIPSE_H
