#include "./GraphicsEmbed.h"

JSSmart<CJSValue> CGraphicsEmbed::init(JSSmart<CJSValue> context, JSSmart<CJSValue> width_px, JSSmart<CJSValue> height_px, JSSmart<CJSValue> width_mm, JSSmart<CJSValue> height_mm)
{
    m_pInternal->init(context->toObject(), width_px->toDouble(), height_px->toDouble(), width_mm->toDouble(), height_mm->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::EndDraw() {}
JSSmart<CJSValue> CGraphicsEmbed::put_GlobalAlpha(JSSmart<CJSValue> enable, JSSmart<CJSValue> globalAlpha);
JSSmart<CJSValue> CGraphicsEmbed::Start_GlobalAlpha() {}
JSSmart<CJSValue> CGraphicsEmbed::End_GlobalAlpha();
JSSmart<CJSValue> CGraphicsEmbed::p_color(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
JSSmart<CJSValue> CGraphicsEmbed::p_width(JSSmart<CJSValue> w);
JSSmart<CJSValue> CGraphicsEmbed::p_dash(JSSmart<CJSValue> params);
JSSmart<CJSValue> CGraphicsEmbed::b_color1(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
JSSmart<CJSValue> CGraphicsEmbed::b_color2(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a);
JSSmart<CJSValue> CGraphicsEmbed::transform(JSSmart<CJSValue> sx, JSSmart<CJSValue> shy, JSSmart<CJSValue> shx, JSSmart<CJSValue> sy, JSSmart<CJSValue> tx, JSSmart<CJSValue> ty);
JSSmart<CJSValue> CGraphicsEmbed::CalculateFullTransform(JSSmart<CJSValue> isInvertNeed);
JSSmart<CJSValue> CGraphicsEmbed::_s();
JSSmart<CJSValue> CGraphicsEmbed::_e();
JSSmart<CJSValue> CGraphicsEmbed::_z();
JSSmart<CJSValue> CGraphicsEmbed::_m (JSSmart<CJSValue> x,  JSSmart<CJSValue> y);
JSSmart<CJSValue> CGraphicsEmbed::_l (JSSmart<CJSValue> x,  JSSmart<CJSValue> y);
JSSmart<CJSValue> CGraphicsEmbed::_c (JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2, JSSmart<CJSValue> x3, JSSmart<CJSValue> y3);
JSSmart<CJSValue> CGraphicsEmbed::_c2(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2);
JSSmart<CJSValue> CGraphicsEmbed::ds();
JSSmart<CJSValue> CGraphicsEmbed::df();
JSSmart<CJSValue> CGraphicsEmbed::save();
JSSmart<CJSValue> CGraphicsEmbed::restore();
JSSmart<CJSValue> CGraphicsEmbed::clip();
JSSmart<CJSValue> CGraphicsEmbed::reset();
JSSmart<CJSValue> CGraphicsEmbed::transform3(JSSmart<CJSValue> m, JSSmart<CJSValue> isNeedInvert);
JSSmart<CJSValue> CGraphicsEmbed::FreeFont();
JSSmart<CJSValue> CGraphicsEmbed::ClearLastFont();
JSSmart<CJSValue> CGraphicsEmbed::drawImage2(JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect);
JSSmart<CJSValue> CGraphicsEmbed::drawImage (JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect, JSSmart<CJSValue> nativeImage);
JSSmart<CJSValue> CGraphicsEmbed::GetFont();
JSSmart<CJSValue> CGraphicsEmbed::font(JSSmart<CJSValue> font_id, JSSmart<CJSValue> font_size);
JSSmart<CJSValue> CGraphicsEmbed::SetFont(JSSmart<CJSValue> font);
JSSmart<CJSValue> CGraphicsEmbed::SetTextPr(JSSmart<CJSValue> textPr, JSSmart<CJSValue> theme);
JSSmart<CJSValue> CGraphicsEmbed::SetFontSlot(JSSmart<CJSValue> slot, JSSmart<CJSValue> fontSizeKoef);
JSSmart<CJSValue> CGraphicsEmbed::GetTextPr();
JSSmart<CJSValue> CGraphicsEmbed::FillText(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text);
JSSmart<CJSValue> CGraphicsEmbed::t(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> isBounds);
JSSmart<CJSValue> CGraphicsEmbed::FillText2(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
JSSmart<CJSValue> CGraphicsEmbed::t2(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
JSSmart<CJSValue> CGraphicsEmbed::FillTextCode(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> lUnicode);
JSSmart<CJSValue> CGraphicsEmbed::tg(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y);
JSSmart<CJSValue> CGraphicsEmbed::charspace(JSSmart<CJSValue> space) {}
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyph (JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> _bounds);
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyphC(JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW);
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyph2(JSSmart<CJSValue> pGlyph);
JSSmart<CJSValue> CGraphicsEmbed::SetIntegerGrid(JSSmart<CJSValue> param);
JSSmart<CJSValue> CGraphicsEmbed::GetIntegerGrid();
JSSmart<CJSValue> CGraphicsEmbed::DrawStringASCII (JSSmart<CJSValue> name, JSSmart<CJSValue> size, JSSmart<CJSValue> bold, JSSmart<CJSValue> italic, JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> bIsHeader);
JSSmart<CJSValue> CGraphicsEmbed::DrawStringASCII2(JSSmart<CJSValue> name, JSSmart<CJSValue> size, JSSmart<CJSValue> bold, JSSmart<CJSValue> italic, JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> bIsHeader);
JSSmart<CJSValue> CGraphicsEmbed::DrawHeaderEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type);
JSSmart<CJSValue> CGraphicsEmbed::DrawFooterEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type);
JSSmart<CJSValue> CGraphicsEmbed::DrawLockParagraph (JSSmart<CJSValue> lock_type, JSSmart<CJSValue> x, JSSmart<CJSValue> y1, JSSmart<CJSValue> y2);
JSSmart<CJSValue> CGraphicsEmbed::DrawLockObjectRect(JSSmart<CJSValue> lock_type, JSSmart<CJSValue> x, JSSmart<CJSValue> y,  JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::DrawEmptyTableLine(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2);
JSSmart<CJSValue> CGraphicsEmbed::DrawSpellingLine  (JSSmart<CJSValue> y0, JSSmart<CJSValue> x0, JSSmart<CJSValue> x1, JSSmart<CJSValue> w);
JSSmart<CJSValue> CGraphicsEmbed::drawHorLine (JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW);
JSSmart<CJSValue> CGraphicsEmbed::drawHorLine2(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW);
JSSmart<CJSValue> CGraphicsEmbed::drawVerLine (JSSmart<CJSValue> align, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> b, JSSmart<CJSValue> penW);
JSSmart<CJSValue> CGraphicsEmbed::drawHorLineExt(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW, JSSmart<CJSValue> leftMW, JSSmart<CJSValue> rightMW);
JSSmart<CJSValue> CGraphicsEmbed::rect     (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::TableRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::AddClipRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::RemoveClipRect() {}
JSSmart<CJSValue> CGraphicsEmbed::SetClip(JSSmart<CJSValue> r);
JSSmart<CJSValue> CGraphicsEmbed::RemoveClip();
JSSmart<CJSValue> CGraphicsEmbed::drawCollaborativeChanges(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> Color);
JSSmart<CJSValue> CGraphicsEmbed::drawMailMergeField(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::drawSearchResult  (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::drawFlowAnchor    (JSSmart<CJSValue> x, JSSmart<CJSValue> y);
JSSmart<CJSValue> CGraphicsEmbed::SavePen();
JSSmart<CJSValue> CGraphicsEmbed::RestorePen();
JSSmart<CJSValue> CGraphicsEmbed::SaveBrush();
JSSmart<CJSValue> CGraphicsEmbed::RestoreBrush();
JSSmart<CJSValue> CGraphicsEmbed::SavePenBrush();
JSSmart<CJSValue> CGraphicsEmbed::RestorePenBrush();
JSSmart<CJSValue> CGraphicsEmbed::SaveGrState();
JSSmart<CJSValue> CGraphicsEmbed::RestoreGrState();
JSSmart<CJSValue> CGraphicsEmbed::StartClipPath() {}
JSSmart<CJSValue> CGraphicsEmbed::EndClipPath();
JSSmart<CJSValue> CGraphicsEmbed::StartCheckTableDraw();
JSSmart<CJSValue> CGraphicsEmbed::EndCheckTableDraw(JSSmart<CJSValue> bIsRestore);
JSSmart<CJSValue> CGraphicsEmbed::SetTextClipRect(JSSmart<CJSValue> _l, JSSmart<CJSValue> _t, JSSmart<CJSValue> _r, JSSmart<CJSValue> _b);
JSSmart<CJSValue> CGraphicsEmbed::AddSmartRect   (JSSmart<CJSValue> x,  JSSmart<CJSValue> y,  JSSmart<CJSValue> w,  JSSmart<CJSValue> h, JSSmart<CJSValue> pen_w);
JSSmart<CJSValue> CGraphicsEmbed::CheckUseFonts2 (JSSmart<CJSValue> _transform);
JSSmart<CJSValue> CGraphicsEmbed::UncheckUseFonts2();
JSSmart<CJSValue> CGraphicsEmbed::Drawing_StartCheckBounds(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h) {}
JSSmart<CJSValue> CGraphicsEmbed::Drawing_EndCheckBounds() {}
JSSmart<CJSValue> CGraphicsEmbed::DrawPresentationComment(JSSmart<CJSValue> type, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
JSSmart<CJSValue> CGraphicsEmbed::DrawPolygon(JSSmart<CJSValue> oPath, JSSmart<CJSValue> lineWidth, JSSmart<CJSValue> shift);
JSSmart<CJSValue> CGraphicsEmbed::DrawFootnoteRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h);
