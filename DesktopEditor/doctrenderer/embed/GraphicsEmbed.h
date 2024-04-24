/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#ifndef _BUILD_NATIVE_GRAPHICS_EMBED_H_
#define _BUILD_NATIVE_GRAPHICS_EMBED_H_

#include "../graphics.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class CGraphicsEmbed : public CJSEmbedObject
{
public:
	NSGraphics::CGraphics* m_pInternal;

public:
	CGraphicsEmbed() : m_pInternal(new NSGraphics::CGraphics()) {}
	~CGraphicsEmbed() { RELEASEOBJECT(m_pInternal); }

	virtual void* getObject() override { return (void*)m_pInternal; }

public:
	JSSmart<CJSValue> create(JSSmart<CJSValue> Native, JSSmart<CJSValue> width_px, JSSmart<CJSValue> height_px, JSSmart<CJSValue> width_mm, JSSmart<CJSValue> height_mm);
	JSSmart<CJSValue> Destroy();
	JSSmart<CJSValue> EndDraw();
	JSSmart<CJSValue> put_GlobalAlpha(JSSmart<CJSValue> enable, JSSmart<CJSValue> globalAlpha);
	JSSmart<CJSValue> Start_GlobalAlpha();
	JSSmart<CJSValue> End_GlobalAlpha();
	// pen methods
	JSSmart<CJSValue> p_color(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
	JSSmart<CJSValue> p_width(JSSmart<CJSValue> w);
	JSSmart<CJSValue> p_dash(JSSmart<CJSValue> params);
	// brush methods
	JSSmart<CJSValue> b_color1(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
	JSSmart<CJSValue> b_color2(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
	JSSmart<CJSValue> transform(JSSmart<CJSValue> sx, JSSmart<CJSValue> shy, JSSmart<CJSValue> shx, JSSmart<CJSValue> sy, JSSmart<CJSValue> tx, JSSmart<CJSValue> ty);
	JSSmart<CJSValue> CalculateFullTransform(JSSmart<CJSValue> isInvertNeed);
	// path commands
	JSSmart<CJSValue> _s();
	JSSmart<CJSValue> _e();
	JSSmart<CJSValue> _z();
	JSSmart<CJSValue> _m (JSSmart<CJSValue> x,  JSSmart<CJSValue> y);
	JSSmart<CJSValue> _l (JSSmart<CJSValue> x,  JSSmart<CJSValue> y);
	JSSmart<CJSValue> _c (JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2, JSSmart<CJSValue> x3, JSSmart<CJSValue> y3);
	JSSmart<CJSValue> _c2(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2);
	JSSmart<CJSValue> ds();
	JSSmart<CJSValue> df();
	// canvas state
	JSSmart<CJSValue> save();
	JSSmart<CJSValue> restore();
	JSSmart<CJSValue> clip();
	JSSmart<CJSValue> reset();
	JSSmart<CJSValue> FreeFont();
	JSSmart<CJSValue> ClearLastFont();
	// images
	JSSmart<CJSValue> drawImage2(JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect);
	JSSmart<CJSValue> drawImage (JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect, JSSmart<CJSValue> nativeImage);
	// text
	JSSmart<CJSValue> GetFont();
	JSSmart<CJSValue> font(JSSmart<CJSValue> font_id, JSSmart<CJSValue> font_size);
	JSSmart<CJSValue> SetFont(JSSmart<CJSValue> path, JSSmart<CJSValue> face, JSSmart<CJSValue> size, JSSmart<CJSValue> style);
	JSSmart<CJSValue> GetTextPr();
	JSSmart<CJSValue> FillText(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text);
	JSSmart<CJSValue> t(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> _arr);
	JSSmart<CJSValue> FillText2(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
	JSSmart<CJSValue> t2(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
	JSSmart<CJSValue> FillTextCode(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> lUnicode);
	JSSmart<CJSValue> tg(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> charspace(JSSmart<CJSValue> space);
	// private methods
	JSSmart<CJSValue> private_FillGlyph (JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> _bounds);
	JSSmart<CJSValue> private_FillGlyphC(JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
	JSSmart<CJSValue> private_FillGlyph2(JSSmart<CJSValue> pGlyph);
	JSSmart<CJSValue> SetIntegerGrid(JSSmart<CJSValue> param);
	JSSmart<CJSValue> GetIntegerGrid();
	JSSmart<CJSValue> DrawStringASCII (JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> DrawStringASCII2(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> DrawHeaderEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type);
	JSSmart<CJSValue> DrawFooterEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type);
	JSSmart<CJSValue> DrawLockParagraph (JSSmart<CJSValue> x, JSSmart<CJSValue> y1, JSSmart<CJSValue> y2);
	JSSmart<CJSValue> DrawLockObjectRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y,  JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> DrawEmptyTableLine(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2);
	JSSmart<CJSValue> DrawSpellingLine  (JSSmart<CJSValue> y0, JSSmart<CJSValue> x0, JSSmart<CJSValue> x1, JSSmart<CJSValue> w);
	// smart methods for horizontal / vertical lines
	JSSmart<CJSValue> drawHorLine (JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW);
	JSSmart<CJSValue> drawHorLine2(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW);
	JSSmart<CJSValue> drawVerLine (JSSmart<CJSValue> align, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> b, JSSmart<CJSValue> penW);

	JSSmart<CJSValue> drawHorLineExt(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW, JSSmart<CJSValue> leftMW, JSSmart<CJSValue> rightMW);
	JSSmart<CJSValue> rect     (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> TableRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);

	JSSmart<CJSValue> AddClipRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> RemoveClipRect();
	JSSmart<CJSValue> SetClip(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> RemoveClip();
	JSSmart<CJSValue> drawMailMergeField(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> drawSearchResult  (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> drawFlowAnchor    (JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> SavePen();
	JSSmart<CJSValue> RestorePen();
	JSSmart<CJSValue> SaveBrush();
	JSSmart<CJSValue> RestoreBrush();
	JSSmart<CJSValue> SavePenBrush();
	JSSmart<CJSValue> RestorePenBrush();
	JSSmart<CJSValue> SaveGrState();
	JSSmart<CJSValue> RestoreGrState();
	JSSmart<CJSValue> StartClipPath();
	JSSmart<CJSValue> EndClipPath();
	JSSmart<CJSValue> StartCheckTableDraw();
	JSSmart<CJSValue> SetTextClipRect(JSSmart<CJSValue> _l, JSSmart<CJSValue> _t, JSSmart<CJSValue> _r, JSSmart<CJSValue> _b);
	JSSmart<CJSValue> AddSmartRect   (JSSmart<CJSValue> x,  JSSmart<CJSValue> y,  JSSmart<CJSValue> w,  JSSmart<CJSValue> h, JSSmart<CJSValue> pen_w);
	JSSmart<CJSValue> CheckUseFonts2 (JSSmart<CJSValue> _transform);
	JSSmart<CJSValue> UncheckUseFonts2();
	JSSmart<CJSValue> Drawing_StartCheckBounds(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> Drawing_EndCheckBounds();
	JSSmart<CJSValue> DrawPresentationComment(JSSmart<CJSValue> type, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> DrawPolygon(JSSmart<CJSValue> oPath, JSSmart<CJSValue> lineWidth, JSSmart<CJSValue> shift);
	JSSmart<CJSValue> DrawFootnoteRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	// new methods
	JSSmart<CJSValue> toDataURL(JSSmart<CJSValue> type);
	JSSmart<CJSValue> GetPenColor();
	JSSmart<CJSValue> GetBrushColor();
	JSSmart<CJSValue> put_brushTexture(JSSmart<CJSValue> src, JSSmart<CJSValue> type);
	JSSmart<CJSValue> put_brushTextureMode(JSSmart<CJSValue> mode);
	JSSmart<CJSValue> put_BrushTextureAlpha(JSSmart<CJSValue> a);
	JSSmart<CJSValue> put_BrushGradient(JSSmart<CJSValue> colors, JSSmart<CJSValue> n, JSSmart<CJSValue> x0, JSSmart<CJSValue> y0, JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> r0, JSSmart<CJSValue> r1);
	JSSmart<CJSValue> TransformPointX(JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> TransformPointY(JSSmart<CJSValue> x, JSSmart<CJSValue> y);
	JSSmart<CJSValue> put_LineJoin(JSSmart<CJSValue> join);
	JSSmart<CJSValue> get_LineJoin();
	JSSmart<CJSValue> put_TextureBounds(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
	JSSmart<CJSValue> GetlineWidth();
	JSSmart<CJSValue> DrawPath(JSSmart<CJSValue> path);
	JSSmart<CJSValue> CoordTransformOffset(JSSmart<CJSValue> tx, JSSmart<CJSValue> ty);
	JSSmart<CJSValue> GetTransform();

	// layer
	JSSmart<CJSValue> CreateLayer(JSSmart<CJSValue> opacity);
	JSSmart<CJSValue> BlendLayer();

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_GRAPHICS_EMBED_H_
