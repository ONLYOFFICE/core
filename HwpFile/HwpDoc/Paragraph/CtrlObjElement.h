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

#ifndef CTRLOBJELEMENT_H
#define CTRLOBJELEMENT_H

#include "CtrlCommon.h"

namespace HWP
{
struct TMatrix
{
	double m_dM11; // e1
	double m_dM12; // e2
	double m_dM21; // e3
	double m_dM22; // e4
	double m_dDX;  // e5
	double m_dDY;  // e6

	TMatrix();
	TMatrix(double dM11, double dM12, double dM21, double dM22, double dDX, double dDY);
	void Multiply(const TMatrix& oMatrix);

	void ApplyToPoint(double& dX, double& dY) const;
	void ApplyToPoint(int& nX, int &nY) const;
	void ApplyToSize(double& dW, double& dH) const;
};

class CCtrlObjElement : public CCtrlCommon
{
	int m_nXGrpOffset;
	int m_nYGrpOffset;
	short m_shNGrp;
	short m_shVer;
	int m_nOrgWidth;
	int m_nOrgHeight;
	int m_nCurWidth;
	int m_nCurHeight;
	bool m_bHorzFlip;
	bool m_bVerFlip;
	short m_shRotat;
	int m_nXCenter;
	int m_nYCenter;
	short m_shMatCnt;
	VECTOR<TMatrix> m_arMatrixs;

	void ParseRotationInfo(CXMLReader& oReader, EHanType eType);
	void ParseRenderingInfo(CXMLReader& oReader, EHanType eType);

	void ParseHWPXChildren(CXMLReader& oReader);
	void ParseHWPMLElement(CXMLReader& oReader);
public:
	CCtrlObjElement();
	CCtrlObjElement(const HWP_STRING& sCtrlID);
	CCtrlObjElement(const CCtrlObjElement& oObjElement);
	CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);

	void ParseChildren(CXMLReader& oReader, EHanType eType);

	int GetCurWidth() const;
	int GetCurHeight() const;

	int GetOrgWidth() const;
	int GetOrgHeight() const;

	int GetFinalWidth() const;
	int GetFinalHeight() const;

	short GetGroupLevel() const;

	TMatrix GetFinalMatrix() const;

	bool HorzFlip() const;
	bool VertFlip() const;

	static int ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLOBJELEMENT_H
