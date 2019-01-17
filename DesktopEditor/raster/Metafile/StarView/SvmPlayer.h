/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "SvmEnums.h"
#include "SvmObjects.h"
#include "SvmClip.h"

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
	CSvmDC* Copy();

	ESvmMapUnit		GetMapModeUnit();
	void            SetMapMode(TSvmMapMode &mapMode, bool prefered = false);
	TXForm*			GetTransform();
	TXForm*			GetInverseTransform();
	void            MultiplyTransform(TXForm& oForm, unsigned int ulMode);
	void            SetTextColor(TSvmColor& oColor);
	TSvmColor&      GetTextColor();
	void            SetBrush(CSvmBrush* pBrush);
	void            RemoveBrush(CSvmBrush *pBrush);
	CSvmBrush*		GetBrush();
	void            SetFont(CSvmFont* pFont);
	void            RemoveFont(CSvmFont* pFont);
	CSvmFont*		GetFont();
	void            SetTextAlign(unsigned int ulAlign);
	unsigned int    GetTextAlign();
	void            SetBgMode(unsigned int ulBgMode);
	unsigned int    GetBgMode();
	
	void            SetTextBgColor(TSvmColor& oColor);
	void			SetTextBgColor(TSvmColor* oColor);
	TSvmColor&      GetTextBgColor();
	
	void            SetMiterLimit(unsigned int ulMiter);
	unsigned int    GetMiterLimit();
	void            SetFillMode(unsigned int ulFillMode);
	unsigned int    GetFillMode();
	void            SetPen(CSvmPen* pPen);
	void            RemovePen(CSvmPen* pPen);
	CSvmPen*		GetPen();
	void            SetStretchMode(unsigned int& oMode);
	unsigned int    GetStretchMode();
	void            SetRop2Mode(unsigned int& nMode);
	unsigned int    GetRop2Mode();
	//void            SetPalette(CSvmLogPalette* pPalette);
	//void            RemovePalette(CSvmLogPalette* pPalette);
	//CSvmLogPalette* GetPalette();
	void            SetCurPos(TSvmPoint& oPoint);
	void            SetCurPos(int lX, int lY);
	TSvmPoint&      GetCurPos();
	CSvmClip*       GetClip();
	//void            ClipToPath(CSvmPath* pPath, unsigned int unMode);
	void            SetArcDirection(unsigned int unDirection);
	unsigned int    GetArcDirection();

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