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
#pragma once

#include "SvmEnums.h"
#include "SvmObjects.h"
#include "../Common/CClip.h"

#include <map>
#include <vector>

namespace MetaFile
{
	class CSvmFile;
	class CSvmDC;

class CSvmPlayer
{
public:
	CSvmPlayer(CSvmFile *file);
    virtual ~CSvmPlayer();
	void	Clear();
	
	CSvmDC* SaveDC();
	CSvmDC* RestoreDC();
	CSvmDC* GetDC();

	void RegisterObject	(CSvmObjectBase* pObject);
	void SelectObject	(unsigned short ushIndex);
	void DeleteObject	(unsigned short ushIndex);
	
	CSvmObjectBase *GetLastObject (ESvmObjectType type);

	void Pop();
	void Push(int Flags);

	void SetRasterOp(int op);

private:
	void InitStockObjects();
	void InitStockBrush	(bool bNull, unsigned char r, unsigned char g, unsigned char b , unsigned char a);
	void InitStockPen	(bool bNull, unsigned char r, unsigned char g, unsigned char b);

	typedef std::map < unsigned int, CSvmObjectBase* > CSvmObjectMap;

	CSvmDC*						m_pDC;
	std::vector<CSvmDC*>		m_vDCStack;
	CSvmFile*					m_pFile;
	CSvmObjectMap				m_mObjects;
	int							m_nFlags;
	ESvnRasterOp				m_eRasterOp;
	unsigned short              m_ushIndex;
	std::vector<unsigned short> m_vAvailableIndexes;

};

class CSvmDC
{
public:

	CSvmDC();
	~CSvmDC();
	CSvmDC* Copy() const;

	ESvmMapUnit      GetMapModeUnit() const;
	void             SetMapMode(TSvmMapMode &mapMode, bool prefered = false);
	const TXForm&    GetTransform() const;
	const TXForm&    GetInverseTransform() const;
	void             MultiplyTransform(TXForm& oForm, unsigned int ulMode);
	void             SetTextColor(TSvmColor& oColor);
	const TSvmColor& GetTextColor() const;
	void             SetBrush(CSvmBrush* pBrush);
	void             RemoveBrush(CSvmBrush *pBrush);
	const CSvmBrush* GetBrush() const;
	void             SetFont(CSvmFont* pFont);
	void             RemoveFont(CSvmFont* pFont);
	const CSvmFont*  GetFont() const;
	void             SetTextAlign(unsigned int ulAlign);
	unsigned int     GetTextAlign() const;
	void             SetBgMode(unsigned int ulBgMode);
	unsigned int     GetBgMode() const;
	
	void             SetTextBgColor(TSvmColor& oColor);
	void             SetTextBgColor(TSvmColor* oColor);
	const TSvmColor& GetTextBgColor() const;
	
	void             SetMiterLimit(unsigned int ulMiter);
	unsigned int     GetMiterLimit() const;
	void             SetFillMode(unsigned int ulFillMode);
	unsigned int     GetFillMode() const;
	void             SetPen(CSvmPen* pPen);
	void             RemovePen(CSvmPen* pPen);
	const CSvmPen*   GetPen() const;
	void             SetStretchMode(unsigned int& oMode);
	unsigned int     GetStretchMode() const;
	void             SetRop2Mode(unsigned int& nMode);
	unsigned int     GetRop2Mode() const;
	//void            SetPalette(CSvmLogPalette* pPalette);
	//void            RemovePalette(CSvmLogPalette* pPalette);
	//CSvmLogPalette* GetPalette();
	void             SetCurPos(TSvmPoint& oPoint);
	void             SetCurPos(int lX, int lY);
	const TSvmPoint& GetCurPos() const;
	const CClip*     GetClip() const;
	//void            ClipToPath(CSvmPath* pPath, unsigned int unMode);
	void             SetArcDirection(unsigned int unDirection);
	unsigned int     GetArcDirection() const;

	double          m_dPixelWidth;
	double          m_dPixelHeight;
	
	double          m_dPixelWidthPrefered;
	double          m_dPixelHeightPrefered;

	TSvmMapMode     m_oMapMode;

private:

	void	SetPixelWidth(double dPixelW);
	void	SetPixelHeight(double dPixelH);

	CSvmBrush*		m_pBrush;
	CSvmPen*		m_pPen;
	CSvmFont*		m_pFont;
	//CSvmLogPalette* m_pPalette;
	TXForm			m_oTransform;
	TXForm			m_oInverseTransform;
	
	TSvmColor       m_oTextColor;
	TSvmColor       m_oTextBgColor;
	
	unsigned int    m_ulTextAlign;
	unsigned int    m_ulBgMode;
	unsigned int    m_ulMiterLimit;
	unsigned int    m_ulFillMode;
	unsigned int    m_ulStretchMode;
	unsigned int    m_ulRop2Mode;
	
	TSvmPoint		m_oCurPos;
	//CSvmClip        m_oClip;
	unsigned int    m_unArcDirection;
};

}
