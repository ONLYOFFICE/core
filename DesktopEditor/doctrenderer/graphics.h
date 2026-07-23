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

#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>

#include "../common/Types.h"
#include "../common/File.h"
#include "nativecontrol.h"
#include "../graphics/pro/Graphics.h"
#include "embed/GraphicsEmbed.h"

namespace NSGraphics
{
	enum EGrStateType
	{
		gstPen      = 0,
		gstBrush    = 1,
		gstPenBrush = 2,
		gstState    = 3,
		gstNone     = 4
	};

	struct IGrState
	{
	public:
		EGrStateType m_eType;
		IGrState() { m_eType = gstNone; }
		virtual ~IGrState() {}
	};

	struct CGrStatePen : public IGrState
	{
		NSStructures::CPen m_oPen;
		CGrStatePen() { m_eType = gstPen; }
	};

	struct CGrStateBrush : public IGrState
	{
		NSStructures::CBrush m_oBrush;
		CGrStateBrush() { m_eType = gstBrush; }
	};

	struct CGrStatePenBrush : public IGrState
	{
		NSStructures::CPen   m_oPen;
		NSStructures::CBrush m_oBrush;

		CGrStatePenBrush() { m_eType = gstPenBrush; }
	};

	struct CHist_Clip
	{
		Aggplus::CDoubleRect Rect;
		bool             IsIntegerGrid;
		Aggplus::CMatrix Transform;

		CHist_Clip() : IsIntegerGrid(false) {}
	};

	struct CGrStateState : public IGrState
	{
		Aggplus::CMatrix         Transform;
		bool                     IsIntegerGrid;
		std::vector<CHist_Clip*> Clips;

		CGrStateState()
		{
			m_eType = gstState;
			IsIntegerGrid = false;
		}
		~CGrStateState()
		{
			for(CHist_Clip* pClip : Clips)
				RELEASEOBJECT(pClip);
			Clips.clear();
		}
	};

	struct CGrState
	{
		std::vector<IGrState*>   States;
		std::vector<CHist_Clip*> Clips;

		CGrState() {}
		~CGrState()
		{
			for(IGrState* pState : States)
				RELEASEOBJECT(pState);
			States.clear();

			for(CHist_Clip* pState : Clips)
				RELEASEOBJECT(pState);
			Clips.clear();
		}
	};

	struct CColor
	{
		int r;
		int g;
		int b;
		int a;
	};

	struct CTransform
	{
		double sx;
		double shy;
		double shx;
		double sy;
		double tx;
		double ty;
	};

	class CGraphics
	{
	public:
		CGraphicsAppImage* m_pAppImage = nullptr;
		CBgraFrame m_oFrame;

	private:
		NSGraphics::IGraphicsRenderer* m_pRenderer = nullptr;
		CGrState   m_oGrState;

	public:
		CGraphics()
		{
		}
		~CGraphics()
		{
			Destroy();
		}

		void init(double width_px, double height_px, double width_mm, double height_mm);
		void Destroy()
		{
			int w, h;
			if (m_pAppImage && m_pAppImage->GetBits(w, h))
				m_oFrame.put_Data(NULL);

			RELEASEINTERFACE(m_pRenderer);
			RELEASEOBJECT(m_pAppImage);
		}
		void EndDraw() {}
		void put_GlobalAlpha(bool enable, double globalAlpha);
		void Start_GlobalAlpha() {}
		void End_GlobalAlpha();
		// pen methods
		void p_color(int r, int g, int b, int a);
		void p_width(double w);
		void p_dash(size_t length, double* dash);
		// brush methods
		void b_color1(int r, int g, int b, int a);
		void b_color2(int r, int g, int b, int a);
		void transform(double sx, double shy, double shx, double sy, double tx, double ty);
		void CalculateFullTransform();
		// path commands
		void _s();
		void _e();
		void _z();
		void _m (double x,  double y);
		void _l (double x,  double y);
		void _c (double x1, double y1, double x2, double y2, double x3, double y3);
		void _c2(double x1, double y1, double x2, double y2);
		void ds();
		void df();
		// canvas state
		void save();
		void restore();
		void clip();
		void reset();
		void FreeFont();
		void ClearLastFont();
		// images
		void drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha);
		// text
		std::wstring GetFont();
		void font() {}
		void SetFont(const std::wstring& name, int face, double size, int style);
		void GetTextPr() {}
		void FillText(double x, double y, int text);
		void t(double x, double y, const std::wstring& text);
		void tg(int text, double x, double y);
		void charspace() {}
		// private methods
		void private_FillGlyph()  {}
		void private_FillGlyphC() {}
		void private_FillGlyph2() {}
		void SetIntegerGrid(bool param);
		bool GetIntegerGrid();
		void DrawStringASCII(const std::wstring& text, double x, double y);
		void DrawHeaderEdit(double yPos);
		void DrawFooterEdit(double yPos);
		void DrawLockParagraph (double x,  double y1, double y2);
		void DrawLockObjectRect(double x,  double y,  double w,  double h);
		void DrawEmptyTableLine(double x1, double y1, double x2, double y2);
		void DrawSpellingLine  (double y0, double x0, double x1, double w);
		// smart methods for horizontal / vertical lines
		void drawHorLine (BYTE align, double y, double x, double r, double penW);
		void drawHorLine2(BYTE align, double y, double x, double r, double penW);
		void drawVerLine (BYTE align, double x, double y, double b, double penW);
		// advanced table functions
		void drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW);
		void rect     (double x, double y, double w, double h);
		void TableRect(double x, double y, double w, double h);
		// clipping functions
		void AddClipRect(double x, double y, double w, double h);
		void RemoveClipRect();
		void SetClip    (double x, double y, double w, double h);
		void drawMailMergeField      (double x, double y, double w, double h);
		void drawSearchResult        (double x, double y, double w, double h);
		void SavePen();
		void RestorePen();
		void SaveBrush();
		void RestoreBrush();
		void SavePenBrush();
		void RestorePenBrush();
		void SaveGrState();
		void RestoreGrState();
		void StartClipPath();
		void EndClipPath();
		bool StartCheckTableDraw();
		void SetTextClipRect(double _l, double _t, double _r, double _b);
		void DrawFootnoteRect(double x, double y,  double w,  double h);
		// new methods
		std::string toDataURL(std::wstring type);
		CColor GetPenColor();
		CColor GetBrushColor();
		void put_brushTexture(std::wstring src, int type);
		void put_brushTextureMode(int mode);
		void put_BrushTextureAlpha(int a);
		void put_BrushGradient(LONG* pColors, double* pPositions, size_t nCount, double x0, double y0, double x1, double y1, double r0, double r1);
		double TransformPointX(double x, double y);
		double TransformPointY(double x, double y);
		void put_LineJoin(int nJoin);
		int  GetLineJoin();
		void put_TextureBounds(double x, double y, double w, double h);
		double GetlineWidth();
		void DrawPath(int path);
		void CoordTransformOffset(double tx, double ty);
		CTransform GetTransform();

		// layer
		void CreateLayer(double opacity);
		void BlendLayer();
	};
}

#endif // CGRAPHICS_H
