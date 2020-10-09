#include "graphics.h"

double to_double(const v8::Local<v8::Value>& v);
bool   to_bool  (const v8::Local<v8::Value>& v);
int    to_int   (const v8::Local<v8::Value>& v);

CJSGraphics* unwrap_Graphics(v8::Handle<v8::Object> obj);

void init_w             (const v8::FunctionCallbackInfo<v8::Value>& args);
void EndDraw_w          (const v8::FunctionCallbackInfo<v8::Value>& args);
void put_GlobalAlpha_w  (const v8::FunctionCallbackInfo<v8::Value>& args);
void Start_GlobalAlpha_w(const v8::FunctionCallbackInfo<v8::Value>& args);
void End_GlobalAlpha_w  (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_color_w          (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_width_w          (const v8::FunctionCallbackInfo<v8::Value>& args);
void p_dash_w           (const v8::FunctionCallbackInfo<v8::Value>& args);

void m_dDpiX_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);
void globalAlpha_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info);

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate);
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args);
