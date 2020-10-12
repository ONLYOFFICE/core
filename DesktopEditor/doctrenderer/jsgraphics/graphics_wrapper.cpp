#include "graphics_wrapper.h"

#include <string>

static double      to_double(const v8::Local<v8::Value>& v)
{
    return v->ToNumber(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Number>())->Value();
}
static bool        to_bool  (const v8::Local<v8::Value>& v)
{
    return v->ToBoolean(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Boolean>())->Value();
}
static int         to_int   (const v8::Local<v8::Value>& v)
{
    return v->ToInt32(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Int32>())->Value();
}
static int         to_uint  (const v8::Local<v8::Value>& v)
{
    return v->ToUint32(v8::Isolate::GetCurrent()->GetCurrentContext()).FromMaybe(v8::Local<v8::Uint32>())->Value();
}
static std::string to_string(const v8::Local<v8::Value>& v)
{
    v8::String::Utf8Value data(v);
    const char* p = *data;
    if (p == NULL) return std::string();
    return std::string(p);
}

CJSGraphics* unwrap_Graphics(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CJSGraphics*>(field->Value());
}

void init_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->init(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void EndDraw_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndDraw();
}
void put_GlobalAlpha_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->put_GlobalAlpha(to_bool(args[0]), to_int(args[1]));
}
void Start_GlobalAlpha_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Start_GlobalAlpha();
}
void End_GlobalAlpha_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->End_GlobalAlpha();
}
void p_color_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_color(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void p_width_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_width(to_int(args[0]));
}
void p_dash_w                  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_dash(args[0]);
}
void b_color1_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->b_color1(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void b_color2_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->b_color2(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void transform_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 6)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->transform(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_double(args[5]));
}
void CalculateFullTransform_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->CalculateFullTransform(to_bool(args[0]));
}
void _s_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_s();
}
void _e_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_e();
}
void _z_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_z();
}
void _m_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_m(to_double(args[0]), to_double(args[1]));
}
void _l_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_l(to_double(args[0]), to_double(args[1]));
}
void _c_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 6)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_c(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_double(args[5]));
}
void _c2_w                     (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_c2(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void ds_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->ds();
}
void df_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->df();
}
void save_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->save();
}
void restore_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->restore();
}
void clip_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->clip();
}
void reset_w                   (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->reset();
}
void transform3_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->transform3(args[0], to_bool(args[1]));
}
void FreeFont_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FreeFont();
}
void ClearLastFont_w           (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->ClearLastFont();
}
void drawImage2_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 7)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawImage2(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_int(args[5]), args[6]);
}
void drawImage_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawImage(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_int(args[5]), args[6], args[7]);
}
void GetFont_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetFont();
}
void font_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->font(to_uint(args[0]), to_int(args[1]));
}
void SetFont_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetFont(args[0]);
}
void SetTextPr_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    v8::Local<v8::Value> args0 = args[0];
    v8::Local<v8::Value> args1 = args[1];
    pGraphics->SetTextPr(&args0, &args1);
}
void SetFontSlot_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetFontSlot(args[0], to_double(args[1]));
}
void GetTextPr_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetTextPr();
}
void FillText_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillText(to_double(args[0]), to_double(args[1]), args[2]);
}
void t_w                       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->t(args[0], to_double(args[1]), to_double(args[2]), to_bool(args[3]));
}
void FillText2_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillText2(to_double(args[0]), to_double(args[1]), args[2], to_double(args[3]), to_double(args[4]));
}
void t2_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->t2(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void FillTextCode_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillTextCode(to_double(args[0]), to_double(args[1]), args[2]);
}
void tg_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->tg(args[0], to_double(args[1]), to_double(args[2]));
}
void charspace_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->charspace(args[0]);
}
void private_FillGlyph_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyph(args[0], args[1]);
}
void private_FillGlyphC_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyphC(args[0], to_double(args[1]), to_double(args[2]));
}
void private_FillGlyph2_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyph2(args[0]);
}
void SetIntegerGrid_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetIntegerGrid(to_bool(args[0]));
}
void GetIntegerGrid_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetIntegerGrid();
}
void DrawStringASCII_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawStringASCII(to_string(args[0]), to_int(args[1]), to_bool(args[2]), to_bool(args[3]), args[4], to_double(args[5]), to_double(args[6]), to_bool(args[7]));
}
void DrawStringASCII2_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawStringASCII2(to_string(args[0]), to_int(args[1]), to_bool(args[2]), to_bool(args[3]), args[4], to_double(args[5]), to_double(args[6]), to_bool(args[7]));
}
void DrawHeaderEdit_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawHeaderEdit(to_double(args[0]), args[1], to_int(args[2]), to_bool(args[3]), args[4]);
}
void DrawFooterEdit_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawFooterEdit(to_double(args[0]), args[1], to_int(args[2]), to_bool(args[3]), args[4]);
}
void DrawLockParagraph_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawLockParagraph(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void DrawLockObjectRect_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawLockObjectRect(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void DrawEmptyTableLine_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawEmptyTableLine(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void DrawSpellingLine_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawSpellingLine(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawHorLine_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLine(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawHorLine2_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLine2(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawVerLine_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawVerLine(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawHorLineExt_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 7)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLineExt(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]), to_double(args[5]), to_double(args[6]));
}
void rect_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->rect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void TableRect_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->TableRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void AddClipRect_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->AddClipRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void RemoveClipRect_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RemoveClipRect();
}
void SetClip_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetClip(args[0]);
}
void RemoveClip_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RemoveClip();
}
void drawCollaborativeChanges_w(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawCollaborativeChanges(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), args[4]);
}
void drawMailMergeField_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawMailMergeField(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawSearchResult_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawSearchResult(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawFlowAnchor_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawFlowAnchor(to_double(args[0]), to_double(args[1]));
}
void SavePen_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SavePen();
}
void RestorePen_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestorePen();
}
void SaveBrush_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SaveBrush();
}
void RestoreBrush_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestoreBrush();
}
void SavePenBrush_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SavePenBrush();
}
void RestorePenBrush_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestorePenBrush();
}
void SaveGrState_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SaveGrState();
}
void RestoreGrState_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestoreGrState();
}
void StartClipPath_w           (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->StartClipPath();
}
void EndClipPath_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndClipPath();
}
void StartCheckTableDraw_w     (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->StartCheckTableDraw();
}
void EndCheckTableDraw_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndCheckTableDraw(to_bool(args[0]));
}
void SetTextClipRect_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetTextClipRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void AddSmartRect_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->AddSmartRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]),  to_int(args[4]));
}
void CheckUseFonts2_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->CheckUseFonts2(args[0]);
}
void UncheckUseFonts2_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->UncheckUseFonts2();
}
void Drawing_StartCheckBounds_w(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Drawing_StartCheckBounds(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void Drawing_EndCheckBounds_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Drawing_EndCheckBounds();
}
void DrawPresentationComment_w (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawPresentationComment(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void DrawPolygon_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawPolygon(args[0], to_int(args[1]), to_double(args[2]));
}
void DrawFootnoteRect_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawFootnoteRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}

void m_dDpiX_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dDpiX_get()));
}
void globalAlpha_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), pGraphics->globalAlpha_get()));
}

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
{
    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // свойства
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiX"), m_dDpiX_get_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "globalAlpha"), globalAlpha_get_w);

    // методы
    result->Set(current, "init",                     v8::FunctionTemplate::New(current, init_w));
    result->Set(current, "EndDraw",                  v8::FunctionTemplate::New(current, EndDraw_w));
    result->Set(current, "put_GlobalAlpha",          v8::FunctionTemplate::New(current, put_GlobalAlpha_w));
    result->Set(current, "Start_GlobalAlpha",        v8::FunctionTemplate::New(current, Start_GlobalAlpha_w));
    result->Set(current, "End_GlobalAlpha",          v8::FunctionTemplate::New(current, End_GlobalAlpha_w));
    result->Set(current, "p_color",                  v8::FunctionTemplate::New(current, p_color_w));
    result->Set(current, "p_width",                  v8::FunctionTemplate::New(current, p_width_w));
    result->Set(current, "p_dash",                   v8::FunctionTemplate::New(current, p_dash_w));
    result->Set(current, "b_color1",                 v8::FunctionTemplate::New(current, b_color1_w));
    result->Set(current, "b_color2",                 v8::FunctionTemplate::New(current, b_color2_w));
    result->Set(current, "transform",                v8::FunctionTemplate::New(current, transform_w));
    result->Set(current, "CalculateFullTransform",   v8::FunctionTemplate::New(current, CalculateFullTransform_w));
    result->Set(current, "_s",                       v8::FunctionTemplate::New(current, _s_w));
    result->Set(current, "_e",                       v8::FunctionTemplate::New(current, _e_w));
    result->Set(current, "_z",                       v8::FunctionTemplate::New(current, _z_w));
    result->Set(current, "_m",                       v8::FunctionTemplate::New(current, _m_w));
    result->Set(current, "_l",                       v8::FunctionTemplate::New(current, _l_w));
    result->Set(current, "_c",                       v8::FunctionTemplate::New(current, _c_w));
    result->Set(current, "_c2",                      v8::FunctionTemplate::New(current, _c2_w));
    result->Set(current, "ds",                       v8::FunctionTemplate::New(current, ds_w));
    result->Set(current, "df",                       v8::FunctionTemplate::New(current, df_w));
    result->Set(current, "save",                     v8::FunctionTemplate::New(current, save_w));
    result->Set(current, "restore",                  v8::FunctionTemplate::New(current, restore_w));
    result->Set(current, "clip",                     v8::FunctionTemplate::New(current, clip_w));
    result->Set(current, "reset",                    v8::FunctionTemplate::New(current, reset_w));
    result->Set(current, "transform3",               v8::FunctionTemplate::New(current, transform3_w));
    result->Set(current, "FreeFont",                 v8::FunctionTemplate::New(current, FreeFont_w));
    result->Set(current, "ClearLastFont",            v8::FunctionTemplate::New(current, ClearLastFont_w));
    result->Set(current, "drawImage2",               v8::FunctionTemplate::New(current, drawImage2_w));
    result->Set(current, "drawImage",                v8::FunctionTemplate::New(current, drawImage_w));
    result->Set(current, "GetFont",                  v8::FunctionTemplate::New(current, GetFont_w));
    result->Set(current, "font",                     v8::FunctionTemplate::New(current, font_w));
    result->Set(current, "SetFont",                  v8::FunctionTemplate::New(current, SetFont_w));
    result->Set(current, "SetTextPr",                v8::FunctionTemplate::New(current, SetTextPr_w));
    result->Set(current, "SetFontSlot",              v8::FunctionTemplate::New(current, SetFontSlot_w));
    result->Set(current, "GetTextPr",                v8::FunctionTemplate::New(current, GetTextPr_w));
    result->Set(current, "FillText",                 v8::FunctionTemplate::New(current, FillText_w));
    result->Set(current, "t",                        v8::FunctionTemplate::New(current, t_w));
    result->Set(current, "FillText2",                v8::FunctionTemplate::New(current, FillText2_w));
    result->Set(current, "t2",                       v8::FunctionTemplate::New(current, t2_w));
    result->Set(current, "FillTextCode",             v8::FunctionTemplate::New(current, FillTextCode_w));
    result->Set(current, "tg",                       v8::FunctionTemplate::New(current, tg_w));
    result->Set(current, "charspace",                v8::FunctionTemplate::New(current, charspace_w));
    result->Set(current, "private_FillGlyph",        v8::FunctionTemplate::New(current, private_FillGlyph_w));
    result->Set(current, "private_FillGlyphC",       v8::FunctionTemplate::New(current, private_FillGlyphC_w));
    result->Set(current, "private_FillGlyph2",       v8::FunctionTemplate::New(current, private_FillGlyph2_w));
    result->Set(current, "SetIntegerGrid",           v8::FunctionTemplate::New(current, SetIntegerGrid_w));
    result->Set(current, "GetIntegerGrid",           v8::FunctionTemplate::New(current, GetIntegerGrid_w));
    result->Set(current, "DrawStringASCII",          v8::FunctionTemplate::New(current, DrawStringASCII_w));
    result->Set(current, "DrawStringASCII2",         v8::FunctionTemplate::New(current, DrawStringASCII2_w));
    result->Set(current, "DrawHeaderEdit",           v8::FunctionTemplate::New(current, DrawHeaderEdit_w));
    result->Set(current, "DrawFooterEdit",           v8::FunctionTemplate::New(current, DrawFooterEdit_w));
    result->Set(current, "DrawLockParagraph",        v8::FunctionTemplate::New(current, DrawLockParagraph_w));
    result->Set(current, "DrawLockObjectRect",       v8::FunctionTemplate::New(current, DrawLockObjectRect_w));
    result->Set(current, "DrawEmptyTableLine",       v8::FunctionTemplate::New(current, DrawEmptyTableLine_w));
    result->Set(current, "DrawSpellingLine",         v8::FunctionTemplate::New(current, DrawSpellingLine_w));
    result->Set(current, "drawHorLine",              v8::FunctionTemplate::New(current, drawHorLine_w));
    result->Set(current, "drawHorLine2",             v8::FunctionTemplate::New(current, drawHorLine2_w));
    result->Set(current, "drawVerLine",              v8::FunctionTemplate::New(current, drawVerLine_w));
    result->Set(current, "drawHorLineExt",           v8::FunctionTemplate::New(current, drawHorLineExt_w));
    result->Set(current, "rect",                     v8::FunctionTemplate::New(current, rect_w));
    result->Set(current, "TableRect",                v8::FunctionTemplate::New(current, TableRect_w));
    result->Set(current, "AddClipRect",              v8::FunctionTemplate::New(current, AddClipRect_w));
    result->Set(current, "RemoveClipRect",           v8::FunctionTemplate::New(current, RemoveClipRect_w));
    result->Set(current, "SetClip",                  v8::FunctionTemplate::New(current, SetClip_w));
    result->Set(current, "RemoveClip",               v8::FunctionTemplate::New(current, RemoveClip_w));
    result->Set(current, "drawCollaborativeChanges", v8::FunctionTemplate::New(current, drawCollaborativeChanges_w));
    result->Set(current, "drawMailMergeField",       v8::FunctionTemplate::New(current, drawMailMergeField_w));
    result->Set(current, "drawSearchResult",         v8::FunctionTemplate::New(current, drawSearchResult_w));
    result->Set(current, "drawFlowAnchor",           v8::FunctionTemplate::New(current, drawFlowAnchor_w));
    result->Set(current, "SavePen",                  v8::FunctionTemplate::New(current, SavePen_w));
    result->Set(current, "RestorePen",               v8::FunctionTemplate::New(current, RestorePen_w));
    result->Set(current, "SaveBrush",                v8::FunctionTemplate::New(current, SaveBrush_w));
    result->Set(current, "RestoreBrush",             v8::FunctionTemplate::New(current, RestoreBrush_w));
    result->Set(current, "SavePenBrush",             v8::FunctionTemplate::New(current, SavePenBrush_w));
    result->Set(current, "RestorePenBrush",          v8::FunctionTemplate::New(current, RestorePenBrush_w));
    result->Set(current, "SaveGrState",              v8::FunctionTemplate::New(current, SaveGrState_w));
    result->Set(current, "RestoreGrState",           v8::FunctionTemplate::New(current, RestoreGrState_w));
    result->Set(current, "StartClipPath",            v8::FunctionTemplate::New(current, StartClipPath_w));
    result->Set(current, "EndClipPath",              v8::FunctionTemplate::New(current, EndClipPath_w));
    result->Set(current, "StartCheckTableDraw",      v8::FunctionTemplate::New(current, StartCheckTableDraw_w));
    result->Set(current, "EndCheckTableDraw",        v8::FunctionTemplate::New(current, EndCheckTableDraw_w));
    result->Set(current, "SetTextClipRect",          v8::FunctionTemplate::New(current, SetTextClipRect_w));
    result->Set(current, "AddSmartRect",             v8::FunctionTemplate::New(current, AddSmartRect_w));
    result->Set(current, "CheckUseFonts2",           v8::FunctionTemplate::New(current, CheckUseFonts2_w));
    result->Set(current, "UncheckUseFonts2",         v8::FunctionTemplate::New(current, UncheckUseFonts2_w));
    result->Set(current, "Drawing_StartCheckBounds", v8::FunctionTemplate::New(current, Drawing_StartCheckBounds_w));
    result->Set(current, "Drawing_EndCheckBounds",   v8::FunctionTemplate::New(current, Drawing_EndCheckBounds_w));
    result->Set(current, "DrawPresentationComment",  v8::FunctionTemplate::New(current, DrawPresentationComment_w));
    result->Set(current, "DrawPolygon",              v8::FunctionTemplate::New(current, DrawPolygon_w));
    result->Set(current, "DrawFootnoteRect",         v8::FunctionTemplate::New(current, DrawFootnoteRect_w));

    return result;
}
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> oUser = CreateGraphicsTemplate(isolate);
    CJSGraphics* pGraphics = new CJSGraphics();

    v8::Local<v8::Object> obj = oUser->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pGraphics));

    args.GetReturnValue().Set(obj);
}
