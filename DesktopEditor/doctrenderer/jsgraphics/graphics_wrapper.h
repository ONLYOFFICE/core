#include "graphics.h"

double to_double(const v8::Local<v8::Value>& v);
bool   to_bool  (const v8::Local<v8::Value>& v);
int    to_int   (const v8::Local<v8::Value>& v);
int    to_uint  (const v8::Local<v8::Value>& v);

CJSGraphics* unwrap_Graphics(v8::Handle<v8::Object> obj);

void init_w                  (const v8::FunctionCallbackInfo<v8::Value>& args);
void EndDraw_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void put_GlobalAlpha_w       (const v8::FunctionCallbackInfo<v8::Value>& args);
void Start_GlobalAlpha_w     (const v8::FunctionCallbackInfo<v8::Value>& args);
void End_GlobalAlpha_w       (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_color_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_width_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_dash_w                (const v8::FunctionCallbackInfo<v8::Value>& args);
void b_color1_w              (const v8::FunctionCallbackInfo<v8::Value>& args);
void b_color2_w              (const v8::FunctionCallbackInfo<v8::Value>& args);
void transform_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void CalculateFullTransform_w(const v8::FunctionCallbackInfo<v8::Value>& args);
void _s_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _e_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _z_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _m_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _l_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _c_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void _c2_w                   (const v8::FunctionCallbackInfo<v8::Value>& args);
void ds_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void df_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void save_w                  (const v8::FunctionCallbackInfo<v8::Value>& args);
void restore_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void clip_w                  (const v8::FunctionCallbackInfo<v8::Value>& args);
void reset_w                 (const v8::FunctionCallbackInfo<v8::Value>& args);
void transform3_w            (const v8::FunctionCallbackInfo<v8::Value>& args);
void FreeFont_w              (const v8::FunctionCallbackInfo<v8::Value>& args);
void ClearLastFont_w         (const v8::FunctionCallbackInfo<v8::Value>& args);
void drawImage2_w            (const v8::FunctionCallbackInfo<v8::Value>& args);
void drawImage_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void GetFont_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void font_w                  (const v8::FunctionCallbackInfo<v8::Value>& args);
void SetFont_w               (const v8::FunctionCallbackInfo<v8::Value>& args);
void SetTextPr_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void SetFontSlot_w           (const v8::FunctionCallbackInfo<v8::Value>& args);
void GetTextPr_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void FillText_w              (const v8::FunctionCallbackInfo<v8::Value>& args);
void t_w                     (const v8::FunctionCallbackInfo<v8::Value>& args);
void FillText2_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void t2_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void FillTextCode_w          (const v8::FunctionCallbackInfo<v8::Value>& args);
void tg_w                    (const v8::FunctionCallbackInfo<v8::Value>& args);
void charspace_w             (const v8::FunctionCallbackInfo<v8::Value>& args);

void m_dDpiX_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);
void globalAlpha_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate);
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args);
