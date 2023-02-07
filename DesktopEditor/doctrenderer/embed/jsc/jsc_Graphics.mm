#include "../GraphicsEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCGraphics <JSExport>
-(JSValue*) create : (JSValue*)Native : (JSValue*)width_px : (JSValue*)height_px : (JSValue*)width_mm : (JSValue*)height_mm;
-(JSValue*) Destroy;
-(JSValue*) EndDraw;
-(JSValue*) put_GlobalAlpha : (JSValue*)enable : (JSValue*)globalAlpha;
-(JSValue*) Start_GlobalAlpha;
-(JSValue*) End_GlobalAlpha;
// pen methods
-(JSValue*) p_color : (JSValue*)r : (JSValue*)g : (JSValue*)b : (JSValue*)a;
-(JSValue*) p_width : (JSValue*)w;
-(JSValue*) p_dash : (JSValue*)params;
// brush methods
-(JSValue*) b_color1 : (JSValue*)r : (JSValue*)g : (JSValue*)b :(JSValue*)a;
-(JSValue*) b_color2 : (JSValue*)r : (JSValue*)g : (JSValue*)b :(JSValue*)a;
-(JSValue*) transform : (JSValue*)sx : (JSValue*)shy : (JSValue*)shx : (JSValue*)sy : (JSValue*)tx : (JSValue*)ty;
-(JSValue*) CalculateFullTransform : (JSValue*)isInvertNeed;
// path commands
-(JSValue*) _s;
-(JSValue*) _e;
-(JSValue*) _z;
-(JSValue*) _m : (JSValue*)x : (JSValue*)y;
-(JSValue*) _l : (JSValue*)x : (JSValue*)y;
-(JSValue*) _c : (JSValue*)x1 : (JSValue*)y1 : (JSValue*)x2 : (JSValue*)y2 : (JSValue*)x3 : (JSValue*)y3;
-(JSValue*) _c2 : (JSValue*)x1 : (JSValue*)y1 : (JSValue*)x2 : (JSValue*)y2;
-(JSValue*) ds;
-(JSValue*) df;
// canvas state
-(JSValue*)save;
-(JSValue*)restore;
-(JSValue*)clip;
-(JSValue*)reset;
-(JSValue*)FreeFont;
-(JSValue*)ClearLastFont;
// images
-(JSValue*)drawImage2 : (JSValue*)img : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h : (JSValue*)alpha : (JSValue*)srcRect;
-(JSValue*)drawImage : (JSValue*)img : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h : (JSValue*)alpha : (JSValue*)srcRect : (JSValue*)nativeImage;
// text
-(JSValue*)GetFont;
-(JSValue*)font : (JSValue*)font_id : (JSValue*)font_size;
-(JSValue*)SetFont : (JSValue*)path : (JSValue*)face : (JSValue*)size : (JSValue*)style;
-(JSValue*)GetTextPr;
-(JSValue*)FillText : (JSValue*)x : (JSValue*)y : (JSValue*)text;
-(JSValue*)t : (JSValue*)x : (JSValue*)y : (JSValue*)_arr;
-(JSValue*)FillText2 : (JSValue*)x : (JSValue*)y : (JSValue*)text : (JSValue*)cropX : (JSValue*)cropW;
-(JSValue*)t2 : (JSValue*)text : (JSValue*)x : (JSValue*)y : (JSValue*)cropX : (JSValue*)cropW;
-(JSValue*)FillTextCode : (JSValue*)x : (JSValue*)y : (JSValue*)lUnicode;
-(JSValue*)tg : (JSValue*)text : (JSValue*)x : (JSValue*)y;
-(JSValue*)charspace : (JSValue*)space;
// private methods
-(JSValue*)private_FillGlyph : (JSValue*)pGlyph : (JSValue*)_bounds;
-(JSValue*)private_FillGlyphC : (JSValue*)pGlyph : (JSValue*)cropX : (JSValue*)cropW;
-(JSValue*)private_FillGlyph2 : (JSValue*)pGlyph;
-(JSValue*)SetIntegerGrid : (JSValue*)param;
-(JSValue*)GetIntegerGrid;
-(JSValue*)DrawStringASCII : (JSValue*)text : (JSValue*)x : (JSValue*)y;
-(JSValue*)DrawStringASCII2 : (JSValue*)text : (JSValue*)x : (JSValue*)y;
-(JSValue*)DrawHeaderEdit : (JSValue*)yPos : (JSValue*)lock_type : (JSValue*)sectionNum : (JSValue*)bIsRepeat : (JSValue*)type;
-(JSValue*)DrawFooterEdit : (JSValue*)yPos : (JSValue*)lock_type : (JSValue*)sectionNum : (JSValue*)bIsRepeat : (JSValue*)type;
-(JSValue*)DrawLockParagraph : (JSValue*)x : (JSValue*)y1 : (JSValue*)y2;
-(JSValue*)DrawLockObjectRect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)DrawEmptyTableLine : (JSValue*)x1 : (JSValue*)y1 : (JSValue*)x2 : (JSValue*)y2;
-(JSValue*)DrawSpellingLine : (JSValue*)y0 : (JSValue*)x0 : (JSValue*)x1 : (JSValue*)w;
// smart methods for horizontal / vertical lines
-(JSValue*)drawHorLine : (JSValue*)align : (JSValue*)y : (JSValue*)x : (JSValue*)r : (JSValue*)penW;
-(JSValue*)drawHorLine2 : (JSValue*)align : (JSValue*)y : (JSValue*)x : (JSValue*)r : (JSValue*)penW;
-(JSValue*)drawVerLine : (JSValue*)align : (JSValue*)x : (JSValue*)y : (JSValue*)b : (JSValue*)penW;
// мега крутые функции для таблиц
-(JSValue*)drawHorLineExt : (JSValue*)align : (JSValue*)y : (JSValue*)x : (JSValue*)r : (JSValue*)penW : (JSValue*)leftMW : (JSValue*)rightMW;
-(JSValue*)rect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)TableRect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
// функции клиппирования
-(JSValue*)AddClipRect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)RemoveClipRect;
-(JSValue*)SetClip : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)RemoveClip;
-(JSValue*)drawMailMergeField : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)drawSearchResult : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)drawFlowAnchor : (JSValue*)x : (JSValue*)y;
-(JSValue*)SavePen;
-(JSValue*)RestorePen;
-(JSValue*)SaveBrush;
-(JSValue*)RestoreBrush;
-(JSValue*)SavePenBrush;
-(JSValue*)RestorePenBrush;
-(JSValue*)SaveGrState;
-(JSValue*)RestoreGrState;
-(JSValue*)StartClipPath;
-(JSValue*)EndClipPath;
-(JSValue*)StartCheckTableDraw;
-(JSValue*)SetTextClipRect : (JSValue*)_l : (JSValue*)_t : (JSValue*)_r : (JSValue*)_b;
-(JSValue*)AddSmartRect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h : (JSValue*)pen_w;
-(JSValue*)CheckUseFonts2 : (JSValue*)_transform;
-(JSValue*)UncheckUseFonts2;
-(JSValue*)Drawing_StartCheckBounds : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)Drawing_EndCheckBounds;
-(JSValue*)DrawPresentationComment : (JSValue*)type : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)DrawPolygon : (JSValue*)oPath : (JSValue*)lineWidth : (JSValue*)shift;
-(JSValue*)DrawFootnoteRect : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
// new methods
-(JSValue*)toDataURL: (JSValue*)type;
-(JSValue*)GetPenColor;
-(JSValue*)GetBrushColor;
-(JSValue*)put_brushTexture : (JSValue*)src : (JSValue*)type;
-(JSValue*)put_brushTextureMode : (JSValue*)mode;
-(JSValue*)put_BrushTextureAlpha : (JSValue*)a;
-(JSValue*)put_BrushGradient : (JSValue*)colors : (JSValue*)n : (JSValue*)x0 : (JSValue*)y0 : (JSValue*)x1 : (JSValue*)y1 : (JSValue*)r0 : (JSValue*)r1;
-(JSValue*)TransformPointX : (JSValue*)x : (JSValue*)y;
-(JSValue*)TransformPointY : (JSValue*)x : (JSValue*)y;
-(JSValue*)put_LineJoin : (JSValue*)join;
-(JSValue*)get_LineJoin;
-(JSValue*)put_TextureBounds : (JSValue*)x : (JSValue*)y : (JSValue*)w : (JSValue*)h;
-(JSValue*)GetlineWidth;
-(JSValue*)DrawPath : (JSValue*)path;
-(JSValue*)CoordTransformOffset : (JSValue*)tx : (JSValue*)ty;
-(JSValue*)GetTransform;

@end

@interface CJSCGraphics : NSObject<IJSCGraphics, JSEmbedObjectProtocol>
{
@public
    CGraphicsEmbed* m_internal;
}
@end

@implementation CJSCGraphics

EMBED_OBJECT_WRAPPER_METHODS(CGraphicsEmbed)

FUNCTION_WRAPPER_JS_5(create, init)
FUNCTION_WRAPPER_JS(Destroy, Destroy)
FUNCTION_WRAPPER_JS(EndDraw, EndDraw)
FUNCTION_WRAPPER_JS_2(put_GlobalAlpha, put_GlobalAlpha)
FUNCTION_WRAPPER_JS(Start_GlobalAlpha, Start_GlobalAlpha)
FUNCTION_WRAPPER_JS(End_GlobalAlpha, End_GlobalAlpha)
// pen methods
FUNCTION_WRAPPER_JS_4(p_color, p_color)
FUNCTION_WRAPPER_JS_1(p_width, p_width)
FUNCTION_WRAPPER_JS_1(p_dash, p_dash)
// brush methods
FUNCTION_WRAPPER_JS_4(b_color1, b_color1)
FUNCTION_WRAPPER_JS_4(b_color2, b_color2)
FUNCTION_WRAPPER_JS_6(transform, transform)
FUNCTION_WRAPPER_JS_1(CalculateFullTransform, CalculateFullTransform)
// path commands
FUNCTION_WRAPPER_JS(_s, _s)
FUNCTION_WRAPPER_JS(_e, _e)
FUNCTION_WRAPPER_JS(_z, _z)
FUNCTION_WRAPPER_JS_2(_m, _m)
FUNCTION_WRAPPER_JS_2(_l,  _l)
FUNCTION_WRAPPER_JS_6(_c,  _c)
FUNCTION_WRAPPER_JS_4(_c2, _c2)
FUNCTION_WRAPPER_JS(ds, ds)
FUNCTION_WRAPPER_JS(df, df)
// canvas state
FUNCTION_WRAPPER_JS(save, save)
FUNCTION_WRAPPER_JS(restore, restore)
FUNCTION_WRAPPER_JS(clip, clip)
FUNCTION_WRAPPER_JS(reset, reset)
FUNCTION_WRAPPER_JS(FreeFont, FreeFont)
FUNCTION_WRAPPER_JS(ClearLastFont, ClearLastFont)
// images
FUNCTION_WRAPPER_JS_7(drawImage2, drawImage2)
FUNCTION_WRAPPER_JS_8(drawImage, drawImage)
// text
FUNCTION_WRAPPER_JS(GetFont, GetFont)
FUNCTION_WRAPPER_JS_2(font, font)
FUNCTION_WRAPPER_JS_4(SetFont, SetFont)
FUNCTION_WRAPPER_JS (GetTextPr, GetTextPr)
FUNCTION_WRAPPER_JS_3(FillText, FillText)
FUNCTION_WRAPPER_JS_3(t, t)
FUNCTION_WRAPPER_JS_5(FillText2, FillText2)
FUNCTION_WRAPPER_JS_5(t2, t2)
FUNCTION_WRAPPER_JS_3(FillTextCode, FillTextCode)
FUNCTION_WRAPPER_JS_3(tg, tg)
FUNCTION_WRAPPER_JS_1(charspace, charspace)
// private methods
FUNCTION_WRAPPER_JS_2(private_FillGlyph, private_FillGlyph)
FUNCTION_WRAPPER_JS_3(private_FillGlyphC, private_FillGlyphC)
FUNCTION_WRAPPER_JS_1(private_FillGlyph2, private_FillGlyph2)
FUNCTION_WRAPPER_JS_1(SetIntegerGrid, SetIntegerGrid)
FUNCTION_WRAPPER_JS (GetIntegerGrid, GetIntegerGrid)
FUNCTION_WRAPPER_JS_3(DrawStringASCII, DrawStringASCII)
FUNCTION_WRAPPER_JS_3(DrawStringASCII2, DrawStringASCII2)
FUNCTION_WRAPPER_JS_5(DrawHeaderEdit, DrawHeaderEdit)
FUNCTION_WRAPPER_JS_5(DrawFooterEdit, DrawFooterEdit)
FUNCTION_WRAPPER_JS_3(DrawLockParagraph, DrawLockParagraph)
FUNCTION_WRAPPER_JS_4(DrawLockObjectRect, DrawLockObjectRect)
FUNCTION_WRAPPER_JS_4(DrawEmptyTableLine, DrawEmptyTableLine)
FUNCTION_WRAPPER_JS_4(DrawSpellingLine, DrawSpellingLine)
// smart methods for horizontal / vertical lines
FUNCTION_WRAPPER_JS_5(drawHorLine, drawHorLine)
FUNCTION_WRAPPER_JS_5(drawHorLine2, drawHorLine2)
FUNCTION_WRAPPER_JS_5(drawVerLine, drawVerLine)
// мега крутые функции для таблиц
FUNCTION_WRAPPER_JS_7(drawHorLineExt, drawHorLineExt)
FUNCTION_WRAPPER_JS_4(rect, rect)
FUNCTION_WRAPPER_JS_4(TableRect, TableRect)
// функции клиппирования
FUNCTION_WRAPPER_JS_4(AddClipRect, AddClipRect)
FUNCTION_WRAPPER_JS (RemoveClipRect, RemoveClipRect)
FUNCTION_WRAPPER_JS_4(SetClip, SetClip)
FUNCTION_WRAPPER_JS(RemoveClip, RemoveClip)
FUNCTION_WRAPPER_JS_4(drawMailMergeField, drawMailMergeField)
FUNCTION_WRAPPER_JS_4(drawSearchResult, drawSearchResult)
FUNCTION_WRAPPER_JS_2(drawFlowAnchor, drawFlowAnchor)
FUNCTION_WRAPPER_JS(SavePen, SavePen)
FUNCTION_WRAPPER_JS(RestorePen, RestorePen)
FUNCTION_WRAPPER_JS(SaveBrush, SaveBrush)
FUNCTION_WRAPPER_JS(RestoreBrush, RestoreBrush)
FUNCTION_WRAPPER_JS(SavePenBrush, SavePenBrush)
FUNCTION_WRAPPER_JS(RestorePenBrush, RestorePenBrush)
FUNCTION_WRAPPER_JS(SaveGrState, SaveGrState)
FUNCTION_WRAPPER_JS(RestoreGrState, RestoreGrState)
FUNCTION_WRAPPER_JS(StartClipPath, StartClipPath)
FUNCTION_WRAPPER_JS(EndClipPath, EndClipPath)
FUNCTION_WRAPPER_JS(StartCheckTableDraw, StartCheckTableDraw)
FUNCTION_WRAPPER_JS_4(SetTextClipRect, SetTextClipRect)
FUNCTION_WRAPPER_JS_5(AddSmartRect, AddSmartRect)
FUNCTION_WRAPPER_JS_1(CheckUseFonts2, CheckUseFonts2)
FUNCTION_WRAPPER_JS(UncheckUseFonts2, UncheckUseFonts2)
FUNCTION_WRAPPER_JS_4(Drawing_StartCheckBounds, Drawing_StartCheckBounds)
FUNCTION_WRAPPER_JS(Drawing_EndCheckBounds, Drawing_EndCheckBounds)
FUNCTION_WRAPPER_JS_5(DrawPresentationComment, DrawPresentationComment)
FUNCTION_WRAPPER_JS_3(DrawPolygon, DrawPolygon)
FUNCTION_WRAPPER_JS_4(DrawFootnoteRect, DrawFootnoteRect)
// new methods
FUNCTION_WRAPPER_JS_1(toDataURL, toDataURL)
FUNCTION_WRAPPER_JS(GetPenColor, GetPenColor)
FUNCTION_WRAPPER_JS(GetBrushColor, GetBrushColor)
FUNCTION_WRAPPER_JS_2(put_brushTexture, put_brushTexture)
FUNCTION_WRAPPER_JS_1(put_brushTextureMode, put_brushTextureMode)
FUNCTION_WRAPPER_JS_1(put_BrushTextureAlpha, put_BrushTextureAlpha)
FUNCTION_WRAPPER_JS_8(put_BrushGradient, put_BrushGradient)
FUNCTION_WRAPPER_JS_2(TransformPointX, TransformPointX)
FUNCTION_WRAPPER_JS_2(TransformPointY, TransformPointY)
FUNCTION_WRAPPER_JS_1(put_LineJoin, put_LineJoin)
FUNCTION_WRAPPER_JS(get_LineJoin, get_LineJoin)
FUNCTION_WRAPPER_JS_4(put_TextureBounds, put_TextureBounds)
FUNCTION_WRAPPER_JS(GetlineWidth, GetlineWidth)
FUNCTION_WRAPPER_JS_1(DrawPath, DrawPath)
FUNCTION_WRAPPER_JS_2(CoordTransformOffset, CoordTransformOffset)
FUNCTION_WRAPPER_JS(GetTransform, GetTransform)

@end

void CGraphicsEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCGraphics alloc] init];
    };
}
