#include "GraphicsEmbed.h"

JSSmart<CJSValue> CGraphicsEmbed::init(JSSmart<CJSValue> Native, JSSmart<CJSValue> width_px, JSSmart<CJSValue> height_px, JSSmart<CJSValue> width_mm, JSSmart<CJSValue> height_mm)
{
    m_pInternal->init((NSNativeControl::CNativeControl*)Native->toObjectSmart()->getNative()->getObject(), width_px->toDouble(), height_px->toDouble(), width_mm->toDouble(), height_mm->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::Destroy()
{
    m_pInternal->Destroy();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::EndDraw()
{
    m_pInternal->EndDraw();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::put_GlobalAlpha(JSSmart<CJSValue> enable, JSSmart<CJSValue> globalAlpha)
{
    m_pInternal->put_GlobalAlpha(enable->toBool(), globalAlpha->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::Start_GlobalAlpha()
{
    m_pInternal->Start_GlobalAlpha();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::End_GlobalAlpha()
{
    m_pInternal->End_GlobalAlpha();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::p_color(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a)
{
    m_pInternal->p_color(r->toInt32(), g->toInt32(), b->toInt32(), a->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::p_width(JSSmart<CJSValue> w)
{
    m_pInternal->p_width(w->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::p_dash(JSSmart<CJSValue> params)
{
    JSSmart<CJSArray> items = params->toArray();
    size_t length = items->getCount();
    double* dash = NULL;
    if(length > 0)
        dash = new double[length];
    for(size_t i = 0; i < length; i++)
        dash[i] = items->get(i)->toDouble();
    m_pInternal->p_dash(length, dash);
    RELEASEARRAYOBJECTS(dash);
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::b_color1(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a)
{
    m_pInternal->b_color1(r->toInt32(), g->toInt32(), b->toInt32(), a->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::b_color2(JSSmart<CJSValue> r, JSSmart<CJSValue> g, JSSmart<CJSValue> b, JSSmart<CJSValue> a)
{
    m_pInternal->b_color2(r->toInt32(), g->toInt32(), b->toInt32(), a->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::transform(JSSmart<CJSValue> sx, JSSmart<CJSValue> shy, JSSmart<CJSValue> shx, JSSmart<CJSValue> sy, JSSmart<CJSValue> tx, JSSmart<CJSValue> ty)
{
    m_pInternal->transform(sx->toDouble(), shy->toDouble(), shx->toDouble(), sy->toDouble(), tx->toDouble(), ty->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::CalculateFullTransform(JSSmart<CJSValue> isInvertNeed)
{
    m_pInternal->CalculateFullTransform();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_s()
{
    m_pInternal->_s();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_e()
{
    m_pInternal->_e();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_z()
{
    m_pInternal->_z();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_m (JSSmart<CJSValue> x,  JSSmart<CJSValue> y)
{
    m_pInternal->_m(x->toDouble(), y->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_l (JSSmart<CJSValue> x,  JSSmart<CJSValue> y)
{
    m_pInternal->_l(x->toDouble(), y->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_c (JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2, JSSmart<CJSValue> x3, JSSmart<CJSValue> y3)
{
    m_pInternal->_c(x1->toDouble(), y1->toDouble(), x2->toDouble(), y2->toDouble(), x3->toDouble(), y3->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::_c2(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2)
{
    m_pInternal->_c2(x1->toDouble(), y1->toDouble(), x2->toDouble(), y2->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::ds()
{
    m_pInternal->ds();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::df()
{
    m_pInternal->df();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::save()
{
    m_pInternal->save();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::restore()
{
    m_pInternal->restore();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::clip()
{
    m_pInternal->clip();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::reset()
{
    m_pInternal->reset();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::FreeFont()
{
    m_pInternal->FreeFont();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::ClearLastFont()
{
    m_pInternal->ClearLastFont();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawImage2(JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect)
{
    m_pInternal->drawImage(img->toStringW(), x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble(), alpha->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawImage (JSSmart<CJSValue> img, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> alpha, JSSmart<CJSValue> srcRect, JSSmart<CJSValue> nativeImage)
{
    m_pInternal->drawImage(img->toStringW(), x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble(), alpha->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::GetFont()
{
    return CJSContext::createString(m_pInternal->GetFont());
}
JSSmart<CJSValue> CGraphicsEmbed::font(JSSmart<CJSValue> font_id, JSSmart<CJSValue> font_size)
{
    m_pInternal->font();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SetFont(JSSmart<CJSValue> path, JSSmart<CJSValue> face, JSSmart<CJSValue> size, JSSmart<CJSValue> style)
{
    m_pInternal->SetFont(path->toStringW(), face->toInt32(), size->toDouble(), style->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::GetTextPr()
{
    m_pInternal->GetTextPr();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::FillText(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text)
{
    m_pInternal->FillText(x->toDouble(), y->toDouble(), text->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::t(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> _arr)
{
    m_pInternal->t(x->toDouble(), y->toDouble(), _arr->toStringW());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::FillText2(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> text, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW)
{
    m_pInternal->FillText(x->toDouble(), y->toDouble(), text->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::t2(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> _arr, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW)
{
    m_pInternal->t(x->toDouble(), y->toDouble(), _arr->toStringW());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::FillTextCode(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> lUnicode)
{
    m_pInternal->FillText(x->toDouble(), y->toDouble(), lUnicode->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::tg(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    m_pInternal->tg(text->toInt32(), x->toDouble(), y->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::charspace(JSSmart<CJSValue> space)
{
    m_pInternal->charspace();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyph (JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> _bounds)
{
    m_pInternal->private_FillGlyph();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyphC(JSSmart<CJSValue> pGlyph, JSSmart<CJSValue> cropX, JSSmart<CJSValue> cropW)
{
    m_pInternal->private_FillGlyphC();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::private_FillGlyph2(JSSmart<CJSValue> pGlyph)
{
    m_pInternal->private_FillGlyph2();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SetIntegerGrid(JSSmart<CJSValue> param)
{
    m_pInternal->SetIntegerGrid(param->toBool());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::GetIntegerGrid()
{
    return CJSContext::createBool(m_pInternal->GetIntegerGrid());
}
JSSmart<CJSValue> CGraphicsEmbed::DrawStringASCII (JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    m_pInternal->DrawStringASCII(text->toStringW(), x->toDouble(), y->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawStringASCII2(JSSmart<CJSValue> text, JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    m_pInternal->DrawStringASCII(text->toStringW(), x->toDouble(), y->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawHeaderEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type)
{
    m_pInternal->DrawHeaderEdit(yPos->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawFooterEdit(JSSmart<CJSValue> yPos, JSSmart<CJSValue> lock_type, JSSmart<CJSValue> sectionNum, JSSmart<CJSValue> bIsRepeat, JSSmart<CJSValue> type)
{
    m_pInternal->DrawFooterEdit(yPos->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawLockParagraph (JSSmart<CJSValue> x, JSSmart<CJSValue> y1, JSSmart<CJSValue> y2)
{
    m_pInternal->DrawLockParagraph(x->toDouble(), y1->toDouble(), y2->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawLockObjectRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y,  JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->DrawLockObjectRect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawEmptyTableLine(JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> x2, JSSmart<CJSValue> y2)
{
    m_pInternal->DrawEmptyTableLine(x1->toDouble(), y1->toDouble(), x2->toDouble(), y2->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawSpellingLine  (JSSmart<CJSValue> y0, JSSmart<CJSValue> x0, JSSmart<CJSValue> x1, JSSmart<CJSValue> w)
{
    m_pInternal->DrawSpellingLine(y0->toDouble(), x0->toDouble(), x1->toDouble(), w->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawHorLine (JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW)
{
    m_pInternal->drawHorLine(align->toInt32(), y->toDouble(), x->toDouble(), r->toDouble(), penW->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawHorLine2(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW)
{
    m_pInternal->drawHorLine2(align->toInt32(), y->toDouble(), x->toDouble(), r->toDouble(), penW->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawVerLine (JSSmart<CJSValue> align, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> b, JSSmart<CJSValue> penW)
{
    m_pInternal->drawVerLine(align->toInt32(), x->toDouble(), y->toDouble(), b->toDouble(), penW->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawHorLineExt(JSSmart<CJSValue> align, JSSmart<CJSValue> y, JSSmart<CJSValue> x, JSSmart<CJSValue> r, JSSmart<CJSValue> penW, JSSmart<CJSValue> leftMW, JSSmart<CJSValue> rightMW)
{
    m_pInternal->drawHorLineExt(align->toInt32(), y->toDouble(), x->toDouble(), r->toDouble(), penW->toDouble(), leftMW->toDouble(), rightMW->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::rect     (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->rect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::TableRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->TableRect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::AddClipRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->AddClipRect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RemoveClipRect()
{
    m_pInternal->RemoveClipRect();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SetClip(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->SetClip(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RemoveClip()
{
    m_pInternal->restore();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawMailMergeField(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->drawMailMergeField(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawSearchResult  (JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->drawSearchResult(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::drawFlowAnchor    (JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SavePen()
{
    m_pInternal->SavePen();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RestorePen()
{
    m_pInternal->RestorePen();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SaveBrush()
{
    m_pInternal->SaveBrush();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RestoreBrush()
{
    m_pInternal->RestoreBrush();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SavePenBrush()
{
    m_pInternal->SavePenBrush();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RestorePenBrush()
{
    m_pInternal->RestorePenBrush();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::SaveGrState()
{
    m_pInternal->SaveGrState();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::RestoreGrState()
{
    m_pInternal->RestoreGrState();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::StartClipPath()
{
    m_pInternal->StartClipPath();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::EndClipPath()
{
    m_pInternal->EndClipPath();
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::StartCheckTableDraw()
{
    return CJSContext::createBool(m_pInternal->StartCheckTableDraw());
}
JSSmart<CJSValue> CGraphicsEmbed::SetTextClipRect(JSSmart<CJSValue> _l, JSSmart<CJSValue> _t, JSSmart<CJSValue> _r, JSSmart<CJSValue> _b)
{
    m_pInternal->SetTextClipRect(_l->toDouble(), _t->toDouble(), _r->toDouble(), _b->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::AddSmartRect   (JSSmart<CJSValue> x,  JSSmart<CJSValue> y,  JSSmart<CJSValue> w,  JSSmart<CJSValue> h, JSSmart<CJSValue> pen_w)
{
    m_pInternal->rect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::CheckUseFonts2 (JSSmart<CJSValue> _transform)
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::UncheckUseFonts2()
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::Drawing_StartCheckBounds(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::Drawing_EndCheckBounds()
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawPresentationComment(JSSmart<CJSValue> type, JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawPolygon(JSSmart<CJSValue> oPath, JSSmart<CJSValue> lineWidth, JSSmart<CJSValue> shift)
{
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::DrawFootnoteRect(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->DrawFootnoteRect(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::toDataURL(JSSmart<CJSValue> type)
{
    return CJSContext::createString(m_pInternal->toDataURL(type->toStringW()));
}
JSSmart<CJSValue> CGraphicsEmbed::GetPenColor()
{
    JSSmart<CJSObject> e = CJSContext::createObject();
    NSGraphics::CColor res = m_pInternal->GetPenColor();
    e->set("R", res.r);
    e->set("G", res.g);
    e->set("B", res.b);
    e->set("A", res.a);
    return e->toValue();
}
JSSmart<CJSValue> CGraphicsEmbed::GetBrushColor()
{
    JSSmart<CJSObject> e = CJSContext::createObject();
    NSGraphics::CColor res = m_pInternal->GetBrushColor();
    e->set("R", res.r);
    e->set("G", res.g);
    e->set("B", res.b);
    e->set("A", res.a);
    return e->toValue();
}
JSSmart<CJSValue> CGraphicsEmbed::put_brushTexture(JSSmart<CJSValue> src, JSSmart<CJSValue> type)
{
    m_pInternal->put_brushTexture(src->toStringW(), type->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::put_brushTextureMode(JSSmart<CJSValue> mode)
{
    m_pInternal->put_brushTextureMode(mode->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::put_BrushTextureAlpha(JSSmart<CJSValue> a)
{
    m_pInternal->put_BrushTextureAlpha(a->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::put_BrushGradient(JSSmart<CJSValue> colors, JSSmart<CJSValue> transparent, JSSmart<CJSValue> x0, JSSmart<CJSValue> y0, JSSmart<CJSValue> x1, JSSmart<CJSValue> y1, JSSmart<CJSValue> r0, JSSmart<CJSValue> r1)
{
    JSSmart<CJSArray> items = colors->toArray();
    size_t length = items->getCount() / 5;
    LONG* color = NULL;
    double* pos = NULL;
    if (length > 0)
    {
        color = new LONG[length];
        pos = new double[length];
    }
    LONG R, G, B, A;
    for (size_t i = 0; i < 5 * length; i++)
    {
        JSSmart<CJSValue> item = items->get(i);
        size_t nDel = i % 5;
        switch (nDel)
        {
        case 0: pos[i / 5] = item->toDouble() / 100000.0; break;
        case 1: R = item->toInt32(); break;
        case 2: G = item->toInt32(); break;
        case 3: B = item->toInt32(); break;
        case 4:
            A = item->toInt32();
            if (!transparent->isNull())
                A = transparent->toInt32();
            color[i / 5] = R | (G << 8) | (B << 16) | (A << 24);
            break;
        }
    }

    double _x0 = x0->toDouble() / 100000.0;
    double _y0 = y0->toDouble() / 100000.0;
    double _x1 = x1->toDouble() / 100000.0;
    double _y1 = y1->toDouble() / 100000.0;
    double _r0 = r0->toDouble() / 100000.0;
    double _r1 = r1->toDouble() / 100000.0;

    m_pInternal->put_BrushGradient(color, pos, length, _x0, _y0, _x1, _y1, _r0, _r1);

    RELEASEARRAYOBJECTS(color);
    RELEASEARRAYOBJECTS(pos);
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::TransformPointX(JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    return CJSContext::createDouble(m_pInternal->TransformPointX(x->toDouble(), y->toDouble()));
}
JSSmart<CJSValue> CGraphicsEmbed::TransformPointY(JSSmart<CJSValue> x, JSSmart<CJSValue> y)
{
    return CJSContext::createDouble(m_pInternal->TransformPointY(x->toDouble(), y->toDouble()));
}
JSSmart<CJSValue> CGraphicsEmbed::put_LineJoin(JSSmart<CJSValue> join)
{
    m_pInternal->put_LineJoin(join->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::get_LineJoin()
{
    return CJSContext::createInt(m_pInternal->GetLineJoin());
}
JSSmart<CJSValue> CGraphicsEmbed::put_TextureBounds(JSSmart<CJSValue> x, JSSmart<CJSValue> y, JSSmart<CJSValue> w, JSSmart<CJSValue> h)
{
    m_pInternal->put_TextureBounds(x->toDouble(), y->toDouble(), w->toDouble(), h->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::GetlineWidth()
{
    return CJSContext::createDouble(m_pInternal->GetlineWidth());
}
JSSmart<CJSValue> CGraphicsEmbed::DrawPath(JSSmart<CJSValue> path)
{
    m_pInternal->DrawPath(path->toInt32());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::CoordTransformOffset(JSSmart<CJSValue> tx, JSSmart<CJSValue> ty)
{
    m_pInternal->CoordTransformOffset(tx->toDouble(), ty->toDouble());
    return NULL;
}
JSSmart<CJSValue> CGraphicsEmbed::GetTransform()
{
    JSSmart<CJSObject> e = CJSContext::createObject();
    NSGraphics::CTransform res = m_pInternal->GetTransform();
    e->set("sx",  res.sx);
    e->set("shx", res.shx);
    e->set("shy", res.shy);
    e->set("sy",  res.sy);
    e->set("tx",  res.tx);
    e->set("ty",  res.ty);
    return e->toValue();
}
