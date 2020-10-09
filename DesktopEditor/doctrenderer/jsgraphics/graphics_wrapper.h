#include "graphics.h"

double to_double(const v8::Local<v8::Value>& v);
bool   to_bool  (const v8::Local<v8::Value>& v);
int    to_int   (const v8::Local<v8::Value>& v);

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
void _ds_w                   (const v8::FunctionCallbackInfo<v8::Value>& args);
void _df_w                   (const v8::FunctionCallbackInfo<v8::Value>& args);

void m_dDpiX_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);
void globalAlpha_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate);
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args);
