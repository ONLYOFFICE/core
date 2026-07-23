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

#ifndef CTRLSHAPEPIC_H
#define CTRLSHAPEPIC_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
enum class EImagePathType
{
	COMPOUND,
	OWPML,
	LINK,
	UNKNOWN
};

enum class EPicEffectType
{
	NONE,
	SHADOW,
	GLOW,
	SOFT_EDGE,
	REFLECT
};

class CPicColor
{
	int m_nSize;

	int m_nType;
	int m_nRGB;
public:
	CPicColor(CHWPStream& oBuffer, int nOff, int nSize);
	CPicColor(CXMLReader& oReader);
};

class CPicEffect
{
	EPicEffectType m_eType;
protected:
	int m_nSize;
public:
	CPicEffect(EPicEffectType eType);
	CPicEffect(int nType);

	int GetSize();
};

class CShadow : public CPicEffect
{
	int m_nStyle;
	int m_nTransparency;
	int m_nBlur;
	int m_nDirection;
	int m_nDistance;
	float m_fAngleX;
	float m_fAngleY;
	float m_fMagnifyX;
	float m_fMagnifyY;
	int m_nRotation;
	CPicColor *m_pColor;
public:
	CShadow(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CShadow(CXMLReader& oReader, EHanType eType);
	~CShadow();
};

class CNeon : public CPicEffect
{
	float m_fTransparency;
	float m_fRadius;
	CPicColor *m_pColor;
public:
	CNeon(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CNeon(CXMLReader& oReader, EHanType eType);
	~CNeon();
};

class CSoftEdge : public CPicEffect
{
	float m_fRadius;
public:
	CSoftEdge(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CSoftEdge(CXMLReader& oReader, EHanType eType);
};

class CReflect : public CPicEffect
{
	int m_nStyle;
	float m_fRadius;
	float m_fDirection;
	float m_fDistance;
	float m_fAngleX;
	float m_fAngleY;
	float m_fMagnifyX;
	float m_fMagnifyY;
	int m_nRotateStyle;
	float m_fStartTrans;
	float m_fStartPos;
	float m_fEndTrans;
	float m_fEndPos;
	float m_fOffsetDirection;
public:
	CReflect(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CReflect(CXMLReader& oReader, EHanType eType);
};

class CCtrlShapePic : public CCtrlGeneralShape
{
	int m_nBorderColor;
	int m_nBorderThick;
	int m_nBorderAttr;
	TPoint m_arBorderPoints[4];
	int m_nCropLeft;
	int m_nCropTop;
	int m_nCropRight;
	int m_nCropBottom;
	short m_arInnerSpaces[4];
	HWP_BYTE m_chBright;
	HWP_BYTE m_chContrast;
	HWP_BYTE m_chEffect;
	HWP_STRING m_sBinDataID;

	HWP_BYTE m_chBorderAlpha;
	int m_nInstanceID;
	int m_nPicEffectInfo;
	LIST<CPicEffect*> m_arPicEffect;

	int m_nIniPicWidth;
	int m_nIniPicHeight;
	HWP_BYTE m_chPicAlpha;

	void ReadFromHWPX(CXMLReader& oReader);
	void ReadFromHWPML(CXMLReader& oReader);

	void ReadImageClip(CXMLReader& oReader, EHanType eType);
	void ReadEffects(CXMLReader& oReader, EHanType eType);
	void ReadImage(CXMLReader& oReader, EHanType eType);
public:
	CCtrlShapePic();
	CCtrlShapePic(const HWP_STRING& sCtrlID);
	CCtrlShapePic(const CCtrlGeneralShape& oShape);
	CCtrlShapePic(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapePic(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);
	~CCtrlShapePic();

	EShapeType GetShapeType() const override;

	HWP_STRING GetBinDataID() const;
	int GetPicWidth() const;
	int GetPicHeight() const;

	ELineStyle2 GetBorderLineStyle() const;
	HWP_BYTE GetBorderCompoundLineType() const;
	int GetBorderColor() const;
	int GetBorderThick() const;

	int GetImageRectWidth() const;
	int GetIMageRectHeight() const;

	static int ParseElement(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEPIC_H
