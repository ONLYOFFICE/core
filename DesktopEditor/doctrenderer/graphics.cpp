#include "graphics.h"

#include <string>
#include <cmath>

namespace NSGraphics
{
void CGraphics::init(double width_px, double height_px, double width_mm, double height_mm)
{
    NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();

    m_pRenderer = NSGraphics::Create();
    m_pRenderer->SetFontManager(pManager);

    int nRasterW = (int)width_px;
    int nRasterH = (int)height_px;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    // дефолтный фон должен быть прозрачным, а не белым
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame oFrame;
    oFrame.put_Data(pData);
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(4 * nRasterW);

    m_pRenderer->CreateFromBgraFrame(&oFrame);
    m_pRenderer->SetSwapRGB(false);

    m_pRenderer->put_Width(width_mm);
    m_pRenderer->put_Height(height_mm);
}

void CGraphics::put_GlobalAlpha(bool enable, double alpha)
{
    pRenderer->put_GlobalAlphaEnabled(enable, alpha);
}

void CGraphics::End_GlobalAlpha()
{
    bool bIsInteger = pRenderer->get_IntegerGrid();
    pRenderer->put_IntegerGrid(true);

    pRenderer->PathCommandEnd();
    b_color1(255, 255, 255, 140);

    pRenderer->AddRect(0.0, 0.0, pRenderer->GetPixW(), pRenderer->GetPixH());
    pRenderer->Fill();
    pRenderer->PathCommandEnd();

    pRenderer->put_IntegerGrid(bIsInteger);
}

void CGraphics::p_color(int r, int g, int b, int a)
{
    pRenderer->put_PenColor(r | (g << 8) | (b << 16));
    pRenderer->put_PenAlpha(a);
}

void CGraphics::p_width(double w)
{
    pRenderer->put_PenSize(w / 1000.0);
}

void CGraphics::p_dash(size_t length, double* dash)
{
    if(length > 0)
    {
        for(size_t i = 0; i < length; i++)
            dash[i] = dash[i] * 72.0 / 25.4 * 2;

        pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
        pRenderer->PenDashPattern(dash, length);
    }
    else
        pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
}

void CGraphics::b_color1(int r, int g, int b, int a)
{    
    pRenderer->put_BrushType(c_BrushTypeSolid);
    pRenderer->put_BrushColor1(r | (g << 8) | (b << 16));
    pRenderer->put_BrushAlpha1(a);
}

void CGraphics::b_color2(int r, int g, int b, int a)
{
    pRenderer->put_BrushColor2(r | (g << 8) | (b << 16));
    pRenderer->put_BrushAlpha2(a);
}

void CGraphics::transform(double sx, double shy, double shx, double sy, double tx, double ty)
{
    pRenderer->SetTransform(sx, shy, shx, sy, tx, ty);
}

void CGraphics::_s()
{
    pRenderer->PathCommandEnd();
}

void CGraphics::_e()
{
    pRenderer->PathCommandEnd();
}

void CGraphics::_z()
{
    pRenderer->PathCommandClose();
}

void CGraphics::_m(double x, double y)
{
    if (!pRenderer->get_IntegerGrid())
        pRenderer->PathCommandMoveTo(x, y);
    else
    {
        pRenderer->GetFullTransform()->TransformPoint(x, y);
        pRenderer->PathCommandMoveTo((int)x + 0.5, (int)y + 0.5);
    }
}

void CGraphics::_l(double x, double y)
{
    if (!pRenderer->get_IntegerGrid())
        pRenderer->PathCommandLineTo(x, y);
    else
    {
        pRenderer->GetFullTransform()->TransformPoint(x, y);
        pRenderer->PathCommandLineTo((int)x + 0.5, (int)y + 0.5);
    }
}

void CGraphics::_c(double x1, double y1, double x2, double y2, double x3, double y3)
{
    pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
}

void CGraphics::_c2(double x1, double y1, double x2, double y2)
{
    pRenderer->PathCommandCurveTo(x1, y1, x1, y1, x2, y2);
}

void CGraphics::ds()
{
    pRenderer->Stroke();
}

void CGraphics::df()
{
    pRenderer->Fill();
}

void CGraphics::restore()
{
    pRenderer->BeginCommand(c_nResetClipType);
    pRenderer->EndCommand(c_nResetClipType);
}

void CGraphics::clip()
{
    pRenderer->BeginCommand(c_nClipType);
    pRenderer->EndCommand(c_nClipType);
}

void CGraphics::reset()
{
    pRenderer->ResetTransform();
}

void CGraphics::transform3(double sx, double shy, double shx, double sy, double tx, double ty, bool isNeedInvert)
{
    pRenderer->SetTransform(sx, shy, shx, sy, tx, ty);
}

void CGraphics::drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha)
{
    std::wstring strImage = (0 == img.find(L"theme") ? m_sThemesDirectory : m_sImagesDirectory) + img;
    pRenderer->DrawImageFromFile(strImage, x, y, w, h, alpha);
}

void CGraphics::SetFont(const std::wstring& path, int face, double size, int style)
{
    pRenderer->put_FontPath((std::string::npos == path.find(L'/') ? m_sFontsDirectory : L"") + path);
    pRenderer->put_FontFaceIndex(face);
    pRenderer->put_FontSize(size);
    pRenderer->put_FontStyle(style);
}

void CGraphics::FillText(double x, double y, int text)
{
    pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
}

void CGraphics::FillText2(double x, double y, int text, double cropX, double cropW)
{
    pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
}

void CGraphics::FillTextCode(double x, double y, int lUnicode)
{
    pRenderer->CommandDrawTextCHAR(lUnicode, x, y, 0, 0);
}

void CGraphics::tg(int text, double x, double y)
{
    pRenderer->put_FontStringGID(TRUE);
    pRenderer->CommandDrawTextCHAR(text, x, y, 0, 0);
    pRenderer->put_FontStringGID(FALSE);
}

void CGraphics::SetIntegerGrid(bool param)
{
    pRenderer->put_IntegerGrid(param);
}

void CGraphics::DrawHeaderEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type)
{
    /*
    var _y = this.m_oFullTransform.TransformPointY(0,yPos);
    _y = (_y >> 0) + 0.5;
    var _x = 0;
    var _wmax = this.m_lWidthPix;

    var _w1 = 6;
    var _w2 = 3;

    var _lineWidth = 1;
    var _isRetina = AscBrowser.isRetina;
    if (_isRetina && !editor.WordControl.bIsRetinaSupport)
        _isRetina = false;

    if (_isRetina)
    {
        _y >>= 0;
        _lineWidth = 2;
    }


    var ctx = this.m_oContext;

    switch (lock_type)
    {
    case locktype_None:
    case locktype_Mine:
    {
        //this.p_color(155, 187, 277, 255);
        //ctx.lineWidth = 2;
        // GlobalSkin.RulerOutline
        this.p_color(0xBB, 0xBE, 0xC2, 255);
        ctx.lineWidth = _lineWidth;
        break;
    }
    case locktype_Other:
    case locktype_Other2:
    {
        this.p_color(238, 53, 37, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
        break;
    }
    default:
    {
        this.p_color(155, 187, 277, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
    }
    }

    if (_isRetina)
    {
        _w1 = (_w1 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _w2 = (_w2 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var bIsNoIntGrid = this.m_bIntegerGrid;

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(true);

    this._s();
    while (true)
    {
        if (_x > _wmax)
            break;
        ctx.moveTo(_x,_y);
        _x+=_w1;
        ctx.lineTo(_x,_y);
        _x+=_w2;
    }
    this.ds();

    var _header_text = AscCommon.translateManager.getValue("Header");
    if (-1 != sectionNum)
        _header_text += (AscCommon.translateManager.getValue(" -Section ") + (sectionNum + 1) + "-");

    if (type)
    {
        if (type.bFirst)
            _header_text = AscCommon.translateManager.getValue("First Page ") + _header_text;
        else if (EvenAndOddHeaders)
        {
            if (type.bEven)
                _header_text = AscCommon.translateManager.getValue("Even Page ") + _header_text;
            else
                _header_text = AscCommon.translateManager.getValue("Odd Page ") + _header_text;
        }
    }

    var _fontSize = _isRetina ? ((9 * AscCommon.AscBrowser.retinaPixelRatio) >> 0) : 9;
    this.DrawStringASCII("Courier New", _fontSize, false, false, _header_text, 2, yPos, true);

    if (bIsRepeat)
        this.DrawStringASCII2("Courier New", _fontSize, false, false, AscCommon.translateManager.getValue("Same as Previous"), 2, yPos, true);

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawFooterEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type)
{
    /*
    var _y = this.m_oFullTransform.TransformPointY(0,yPos);
    _y = (_y >> 0) + 0.5;
    var _x = 0;
    var _w1 = 6;
    var _w2 = 3;

    var _lineWidth = 1;
    var _isRetina = AscBrowser.isRetina;
    if (_isRetina && !editor.WordControl.bIsRetinaSupport)
        _isRetina = false;

    if (_isRetina)
    {
        _y >>= 0;
        _lineWidth = 2;
    }

    var ctx = this.m_oContext;

    switch (lock_type)
    {
    case locktype_None:
    case locktype_Mine:
    {
        //this.p_color(155, 187, 277, 255);
        //ctx.lineWidth = 2;
        // GlobalSkin.RulerOutline
        this.p_color(0xBB, 0xBE, 0xC2, 255);
        ctx.lineWidth = _lineWidth;
        break;
    }
    case locktype_Other:
    case locktype_Other2:
    {
        this.p_color(238, 53, 37, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
        break;
    }
    default:
    {
        this.p_color(155, 187, 277, 255);
        ctx.lineWidth = _lineWidth;
        _w1 = 2;
        _w2 = 1;
    }
    }

    if (_isRetina)
    {
        _w1 = (_w1 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        _w2 = (_w2 * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
    }

    var _wmax = this.m_lWidthPix;

    var bIsNoIntGrid = this.m_bIntegerGrid;

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(true);

    this._s();
    while (true)
    {
        if (_x > _wmax)
            break;
        ctx.moveTo(_x,_y);
        _x+=_w1;
        ctx.lineTo(_x,_y);
        _x+=_w2;
    }
    this.ds();

    var _header_text = AscCommon.translateManager.getValue("Footer");
    if (-1 != sectionNum)
        _header_text += (AscCommon.translateManager.getValue(" -Section ") + (sectionNum + 1) + "-");

    if (type)
    {
        if (type.bFirst)
            _header_text = AscCommon.translateManager.getValue("First Page ") + _header_text;
        else if (EvenAndOddHeaders)
        {
            if (type.bEven)
                _header_text = AscCommon.translateManager.getValue("Even Page ") + _header_text;
            else
                _header_text = AscCommon.translateManager.getValue("Odd Page ") + _header_text;
        }
    }

    var _fontSize = _isRetina ? ((9 * AscCommon.AscBrowser.retinaPixelRatio) >> 0) : 9;
    this.DrawStringASCII("Courier New", _fontSize, false, false, _header_text, 2, yPos, false);

    if (bIsRepeat)
        this.DrawStringASCII2("Courier New", _fontSize, false, false, AscCommon.translateManager.getValue("Same as Previous"), 2, yPos, false);

    if (false == bIsNoIntGrid)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawLockParagraph(const v8::Local<v8::Value>& lock_type, double x, double y1, double y2)
{
    /*
    if (lock_type == locktype_None || editor.WordControl.m_oDrawingDocument.IsLockObjectsEnable === false || editor.isViewMode || (lock_type === locktype_Mine && true === AscCommon.CollaborativeEditing.Is_Fast()))
        return;

    if (lock_type == locktype_Mine)
    {
        this.p_color(22, 156, 0, 255);
        //this.p_color(155, 187, 277, 255);
    }
    else
        this.p_color(238, 53, 37, 255);

    var _x = this.m_oFullTransform.TransformPointX(x, y1) >> 0;
    var _xT = this.m_oFullTransform.TransformPointX(x, y2) >> 0;
    var _y1 = (this.m_oFullTransform.TransformPointY(x, y1) >> 0) + 0.5;
    var _y2 = (this.m_oFullTransform.TransformPointY(x, y2) >> 0) - 1.5;

    var ctx = this.m_oContext;
    if (_x != _xT)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);
        this.p_width(1000 * dKoefMMToPx);

        var w_dot = 2 * dKoefMMToPx;
        var w_dist = 1 * dKoefMMToPx;
        var w_len_indent = 3;

        var _interf = editor.WordControl.m_oDrawingDocument.AutoShapesTrack;

        this._s();
        _interf.AddLineDash(ctx, x, y1, x, y2, w_dot, w_dist);
        _interf.AddLineDash(ctx, x, y1, x + w_len_indent, y1, w_dot, w_dist);
        _interf.AddLineDash(ctx, x, y2, x + w_len_indent, y2, w_dot, w_dist);

        this.ds();
        return;
    }

    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);

    ctx.lineWidth = 1;

    var w_dot = 2;
    var w_dist = 1;
    var w_len_indent = (3 * this.m_oCoordTransform.sx) >> 0;

    this._s();

    var y_mem = _y1 - 0.5;
    while (true)
    {
        if ((y_mem + w_dot) > _y2)
            break;
        ctx.moveTo(_x + 0.5,y_mem);
        y_mem+=w_dot;
        ctx.lineTo(_x + 0.5,y_mem);
        y_mem+=w_dist;
    }

    var x_max = _x + w_len_indent;

    var x_mem = _x;
    while (true)
    {
        if (x_mem > x_max)
            break;
        ctx.moveTo(x_mem,_y1);
        x_mem+=w_dot;
        ctx.lineTo(x_mem,_y1);
        x_mem+=w_dist;
    }
    x_mem = _x;
    while (true)
    {
        if (x_mem > x_max)
            break;
        ctx.moveTo(x_mem,_y2);
        x_mem+=w_dot;
        ctx.lineTo(x_mem,_y2);
        x_mem+=w_dist;
    }

    this.ds();

    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawLockObjectRect(const v8::Local<v8::Value>& lock_type, double x, double y, double w, double h)
{
    /*
    if (editor.isViewMode || this.IsThumbnail || lock_type == locktype_None || this.IsDemonstrationMode || (lock_type === locktype_Mine && true === AscCommon.CollaborativeEditing.Is_Fast()))
        return;

    if (editor.WordControl.m_oDrawingDocument.IsLockObjectsEnable === false && lock_type == locktype_Mine)
        return;

    if (lock_type == locktype_Mine)
    {
        this.p_color(22, 156, 0, 255);
        //this.p_color(155, 187, 277, 255);
    }
    else
        this.p_color(238, 53, 37, 255);

    var ctx = this.m_oContext;

    var _m = this.m_oTransform;
    if (_m.sx != 1.0 || _m.shx != 0.0 || _m.shy != 0.0 || _m.sy != 1.0)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);
        this.p_width(1000 * dKoefMMToPx);

        var w_dot = 2 * dKoefMMToPx;
        var w_dist = 1 * dKoefMMToPx;

        var _interf = editor.WordControl.m_oDrawingDocument.AutoShapesTrack;

        var eps = 5 * dKoefMMToPx;
        var _x = x - eps;
        var _y = y - eps;
        var _r = x + w + eps;
        var _b = y + h + eps;

        this._s();
        _interf.AddRectDash(ctx, _x, _y, _r, _y, _x, _b, _r, _b, w_dot, w_dist, true);
        this._s();
        return;
    }

    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);

    ctx.lineWidth = 1;

    var w_dot = 2;
    var w_dist = 2;

    var eps = 5;

    var _x = (this.m_oFullTransform.TransformPointX(x, y) >> 0) - eps + 0.5;
    var _y = (this.m_oFullTransform.TransformPointY(x, y) >> 0) - eps + 0.5;

    var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) >> 0) + eps + 0.5;
    var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) >> 0) + eps + 0.5;

    this._s();

    for (var i = _x; i < _r; i += w_dist)
    {
        ctx.moveTo(i, _y);
        i += w_dot;

        if (i > _r)
            i = _r;

        ctx.lineTo(i, _y);
    }
    for (var i = _y; i < _b; i += w_dist)
    {
        ctx.moveTo(_r, i);
        i += w_dot;

        if (i > _b)
            i = _b;

        ctx.lineTo(_r, i);
    }
    for (var i = _r; i > _x; i -= w_dist)
    {
        ctx.moveTo(i, _b);
        i -= w_dot;

        if (i < _x)
            i = _x;

        ctx.lineTo(i, _b);
    }
    for (var i = _b; i > _y; i -= w_dist)
    {
        ctx.moveTo(_x, i);
        i -= w_dot;

        if (i < _y)
            i = _y;

        ctx.lineTo(_x, i);
    }

    this.ds();

    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawEmptyTableLine(double x1, double y1, double x2, double y2)
{
    /*
    if ((!editor.isShowTableEmptyLine || editor.isViewMode) && (editor.isShowTableEmptyLineAttack === false))
        return;

    var _x1 = this.m_oFullTransform.TransformPointX(x1,y1);
    var _y1 = this.m_oFullTransform.TransformPointY(x1,y1);

    var _x2 = this.m_oFullTransform.TransformPointX(x2,y2);
    var _y2 = this.m_oFullTransform.TransformPointY(x2,y2);

    _x1 = (_x1 >> 0) + 0.5;
    _y1 = (_y1 >> 0) + 0.5;
    _x2 = (_x2 >> 0) + 0.5;
    _y2 = (_y2 >> 0) + 0.5;

    this.p_color(138, 162, 191, 255);
    var ctx = this.m_oContext;

    if (_x1 != _x2 && _y1 != _y2)
    {
        // значит какой-то трансформ
        var dKoefMMToPx = 1 / Math.max(this.m_oCoordTransform.sx, 0.001);

        this.p_width(1000 * dKoefMMToPx);
        this._s();
        editor.WordControl.m_oDrawingDocument.AutoShapesTrack.AddLineDash(ctx, x1, y1, x2, y2, 2 * dKoefMMToPx, 2 * dKoefMMToPx);
        this.ds();
        return;
    }

    ctx.lineWidth = 1;
    var bIsInt = this.m_bIntegerGrid;
    if (!bIsInt)
        this.SetIntegerGrid(true);
    if (_x1 == _x2)
    {
        var _y = Math.min(_y1, _y2) + 0.5;
        var _w1 = 2;
        var _w2 = 2;
        var _wmax = Math.max(_y1, _y2) - 0.5;
        this._s();
        while (true)
        {
            if (_y > _wmax)
                break;
            ctx.moveTo(_x1,_y);
            _y+=_w1;
            if (_y > _wmax)
            {
                ctx.lineTo(_x1,_y - _w1 + 1);
            }
            else
            {
                ctx.lineTo(_x1,_y);
            }
            _y+=_w2;
        }
        this.ds();
    }
    else if (_y1 == _y2)
    {
        var _x = Math.min(_x1, _x2) + 0.5;
        var _w1 = 2;
        var _w2 = 2;
        var _wmax = Math.max(_x1, _x2) - 0.5;
        this._s();
        while (true)
        {
            if (_x > _wmax)
                break;
            ctx.moveTo(_x,_y1);
            _x+=_w1;

            if (_x > _wmax)
            {
                ctx.lineTo(_x - _w2 + 1,_y1);
            }
            else
            {
                ctx.lineTo(_x,_y1);
            }
            _x+=_w2;
        }
        this.ds();
    }
    else
    {
        // значит какой-то трансформ
        this._s();
        editor.WordControl.m_oDrawingDocument.AutoShapesTrack.AddLineDash(ctx, _x1, _y1, _x2, _y2, 2, 2);
        this.ds();
    }
    if (!bIsInt)
        this.SetIntegerGrid(false);
    */
}

void CGraphics::DrawSpellingLine(double y0, double x0, double x1, double w)
{
    /*
    if (!editor.isViewMode)
                this.drawHorLine(0, y0, x0, x1, w );
    */
}

void CGraphics::drawHorLine(int align, double y, double x, double r, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLine(align, y, x, r, penW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(r, y);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5 - 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;

    ctx.setTransform(1,0,0,1,0,0);

    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _top + pen_w / 2 - 0.5);
        ctx.lineTo(_r, _top + pen_w / 2 - 0.5);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_x, _center - 0.5);
            ctx.lineTo(_r, _center - 0.5);
        }
        else
        {
            ctx.moveTo(_x, _center);
            ctx.lineTo(_r, _center);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // bottom
        var _bottom = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _bottom - pen_w / 2 + 0.5);
        ctx.lineTo(_r, _bottom - pen_w / 2 + 0.5);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::drawHorLine2(int align, double y, double x, double r, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLine2(align, y, x, r, penW);
            this.SetIntegerGrid(false);
            return;
        }

        var _y1 = y - penW / 2;
        var _y2 = _y1 + 2 * penW;

        this.p_width(penW * 1000);
        this._s();
        this._m(x, _y1);
        this._l(r, _y1);
        this.ds();

        this._s();
        this._m(x, _y2);
        this._l(r, _y2);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5 - 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        var _pos1 = _top + pen_w / 2 - 0.5 - pen_w;
        var _pos2 = _pos1 + pen_w * 2;

        ctx.beginPath();
        ctx.moveTo(_x, _pos1);
        ctx.lineTo(_r, _pos1);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(_x, _pos2);
        ctx.lineTo(_r, _pos2);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        // TODO:

        break;
    }
    case 2:
    {
        // bottom
        // TODO:

        break;
    }
    }
    */
}

void CGraphics::drawVerLine(int align, double x, double y, double b, int penW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawVerLine(align, x, y, b, penW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(x, b);
        this.ds();
        return;
    }

    var pen_w = ((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0;
    if (0 == pen_w)
        pen_w = 1;

    var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5 - 0.5;
    var _b = (this.m_oFullTransform.TransformPointY(x,b) >> 0) + 0.5 + 0.5;

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    switch (align)
    {
    case 0:
    {
        // left
        var _left = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_left + pen_w / 2 - 0.5, _y);
        ctx.lineTo(_left + pen_w / 2 - 0.5, _b);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_center - 0.5, _y);
            ctx.lineTo(_center - 0.5, _b);
        }
        else
        {
            ctx.moveTo(_center, _y);
            ctx.lineTo(_center, _b);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // right
        var _right = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_right - pen_w / 2 + 0.5, _y);
        ctx.lineTo(_right - pen_w / 2 + 0.5, _b);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::drawHorLineExt(int align, double y, double x, double r, int penW, double leftMW, double rightMW)
{
    /*
    var _check_transform = global_MatrixTransformer.IsIdentity2(this.m_oTransform);
    if (!this.m_bIntegerGrid || !_check_transform)
    {
        if (_check_transform)
        {
            this.SetIntegerGrid(true);
            this.drawHorLineExt(align, y, x, r, penW, leftMW, rightMW);
            this.SetIntegerGrid(false);
            return;
        }

        this.p_width(penW * 1000);
        this._s();
        this._m(x, y);
        this._l(r, y);
        this.ds();
        return;
    }

    var pen_w = Math.max(((this.m_dDpiX * penW / g_dKoef_in_to_mm) + 0.5) >> 0, 1);

    var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
    var _r = (this.m_oFullTransform.TransformPointX(r,y) >> 0) + 0.5;

    if (leftMW != 0)
    {
        var _center = _x;
        var pen_mw = Math.max(((this.m_dDpiX * Math.abs(leftMW) * 2 / g_dKoef_in_to_mm) + 0.5) >> 0, 1);
        if (leftMW < 0)
        {
            if ((pen_mw % 2) == 0)
            {
                _x = _center - (pen_mw / 2);
            }
            else
            {
                _x = _center - ((pen_mw / 2) >> 0);
            }
        }
        else
        {
            if ((pen_mw % 2) == 0)
            {
                _x = _center + ((pen_mw / 2) - 1.0);
            }
            else
            {
                _x = _center + ((pen_mw / 2) >> 0);
            }
        }
    }
    if (rightMW != 0)
    {
        var _center = _r;
        var pen_mw = Math.max(((this.m_dDpiX * Math.abs(rightMW) * 2 / g_dKoef_in_to_mm) + 0.5) >> 0, 1);
        if (rightMW < 0)
        {
            if ((pen_mw % 2) == 0)
            {
                _r = _center - (pen_mw / 2);
            }
            else
            {
                _r = _center - ((pen_mw / 2) >> 0);
            }
        }
        else
        {
            if ((pen_mw % 2) == 0)
            {
                _r = _center + (pen_mw / 2) - 1.0;
            }
            else
            {
                _r = _center + ((pen_mw / 2) >> 0);
            }
        }
    }

    var ctx = this.m_oContext;
    ctx.lineWidth = pen_w;

    _x -= 0.5;
    _r += 0.5;

    switch (align)
    {
    case 0:
    {
        // top
        var _top = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _top + pen_w / 2 - 0.5);
        ctx.lineTo(_r, _top + pen_w / 2 - 0.5);
        ctx.stroke();

        break;
    }
    case 1:
    {
        // center
        var _center = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        if (0 == (pen_w % 2))
        {
            ctx.moveTo(_x, _center - 0.5);
            ctx.lineTo(_r, _center - 0.5);
        }
        else
        {
            ctx.moveTo(_x, _center);
            ctx.lineTo(_r, _center);
        }
        ctx.stroke();

        break;
    }
    case 2:
    {
        // bottom
        var _bottom = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;

        ctx.beginPath();
        ctx.moveTo(_x, _bottom - pen_w / 2 + 0.5);
        ctx.lineTo(_r, _bottom - pen_w / 2 + 0.5);
        ctx.stroke();

        break;
    }
    }
    */
}

void CGraphics::rect(double x, double y, double w, double h)
{
    /*
    var ctx = this.m_oContext;
    ctx.beginPath();

    if (this.m_bIntegerGrid)
    {
        if (AscCommon.global_MatrixTransformer.IsIdentity2(this.m_oFullTransform))
        {
            var _x = (this.m_oFullTransform.TransformPointX(x, y) + 0.5) >> 0;
            var _y = (this.m_oFullTransform.TransformPointY(x, y) + 0.5) >> 0;
            var _r = (this.m_oFullTransform.TransformPointX(x + w, y) + 0.5) >> 0;
            var _b = (this.m_oFullTransform.TransformPointY(x, y + h) + 0.5) >> 0;

            ctx.rect(_x, _y, _r - _x, _b - _y);
        }
        else
        {
            var x1 = this.m_oFullTransform.TransformPointX(x, y);
            var y1 = this.m_oFullTransform.TransformPointY(x, y);
            var x2 = this.m_oFullTransform.TransformPointX(x + w, y);
            var y2 = this.m_oFullTransform.TransformPointY(x + w, y);
            var x3 = this.m_oFullTransform.TransformPointX(x + w, y + h);
            var y3 = this.m_oFullTransform.TransformPointY(x + w, y + h);
            var x4 = this.m_oFullTransform.TransformPointX(x, y + h);
            var y4 = this.m_oFullTransform.TransformPointY(x, y + h);

            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.lineTo(x3, y3);
            ctx.lineTo(x4, y4);
            ctx.closePath();
        }
    }
    else
    {
        ctx.rect(x, y, w, h);
    }
    */
}

void CGraphics::TableRect(double x, double y, double w, double h)
{
    /*
    var ctx = this.m_oContext;

    if (this.m_bIntegerGrid)
    {
        var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
        var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
        var _r = (this.m_oFullTransform.TransformPointX(x+w,y) >> 0) + 0.5;
        var _b = (this.m_oFullTransform.TransformPointY(x,y+h) >> 0) + 0.5;

        ctx.fillRect(_x - 0.5, _y - 0.5, _r - _x + 1, _b - _y + 1);
    }
    else
    {
        ctx.fillRect(x, y, w, h);
    }
    */
}

void CGraphics::AddClipRect(double x, double y, double w, double h)
{
    /*
    var __rect = new AscCommon._rect();
    __rect.x = x;
    __rect.y = y;
    __rect.w = w;
    __rect.h = h;
    this.GrState.AddClipRect(__rect);
    */
}

void CGraphics::SetClip(const v8::Local<v8::Value>& r)
{
    /*
    var ctx = this.m_oContext;
    ctx.save();

    ctx.beginPath();
    if (!global_MatrixTransformer.IsIdentity(this.m_oTransform))
    {
        ctx.rect(r.x, r.y, r.w, r.h);
    }
    else
    {
        var _x = (this.m_oFullTransform.TransformPointX(r.x,r.y) + 1) >> 0;
        var _y = (this.m_oFullTransform.TransformPointY(r.x,r.y) + 1) >> 0;
        var _r = (this.m_oFullTransform.TransformPointX(r.x+r.w,r.y) - 1) >> 0;
        var _b = (this.m_oFullTransform.TransformPointY(r.x,r.y+r.h) - 1) >> 0;

        ctx.rect(_x, _y, _r - _x + 1, _b - _y + 1);
    }

    this.clip();
    ctx.beginPath();
    */
}

void CGraphics::RemoveClip()
{
    /*
    this.m_oContext.restore();
    this.m_oContext.save();

    this.m_bPenColorInit = false;
    this.m_bBrushColorInit = false;

    if (this.m_oContext.globalAlpha != this.globalAlpha)
        this.m_oContext.globalAlpha = this.globalAlpha;
    */
}

void CGraphics::drawCollaborativeChanges(double x, double y, double w, double h, const v8::Local<v8::Value>& Color)
{
    /*
    this.b_color1( Color.r, Color.g, Color.b, 255 );
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawMailMergeField(double x, double y, double w, double h)
{
    /*
    this.b_color1(206, 212, 223, 204);
    //this.b_color1(216, 221, 230, 255);
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawSearchResult(double x, double y, double w, double h)
{
    /*
    this.b_color1( 255, 238, 128, 255 );
    this.rect( x, y, w, h );
    this.df();
    */
}

void CGraphics::drawFlowAnchor(double x, double y)
{
    /*
    if (!AscCommon.g_flow_anchor || !AscCommon.g_flow_anchor.asc_complete || (!editor || !editor.ShowParaMarks))
        return;

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }

    var _x = this.m_oFullTransform.TransformPointX(x,y) >> 0;
    var _y = this.m_oFullTransform.TransformPointY(x,y) >> 0;

    this.m_oContext.drawImage(AscCommon.g_flow_anchor, _x, _y);

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CGraphics::SavePen()
{
    // this.GrState.SavePen();
}

void CGraphics::RestorePen()
{
    // this.GrState.RestorePen();
}

void CGraphics::SaveBrush()
{
    // this.GrState.SaveBrush();
}

void CGraphics::RestoreBrush()
{
    // this.GrState.RestoreBrush();
}

void CGraphics::SavePenBrush()
{
    // this.GrState.SavePenBrush();
}

void CGraphics::RestorePenBrush()
{
    // this.GrState.RestorePenBrush();
}

void CGraphics::SaveGrState()
{
    // this.GrState.SaveGrState();
}

void CGraphics::RestoreGrState()
{
    // this.GrState.RestoreGrState();
}

void CGraphics::EndClipPath()
{
    // this.m_oContext.clip();
}

void CGraphics::StartCheckTableDraw()
{
    /*
    if (!this.m_bIntegerGrid && global_MatrixTransformer.IsIdentity2(this.m_oTransform))
    {
        this.SaveGrState();
        this.SetIntegerGrid(true);
        return true;
    }
    return false;
    */
}

void CGraphics::EndCheckTableDraw(bool bIsRestore)
{
    if (bIsRestore)
        RestoreGrState();
}

void CGraphics::SetTextClipRect(double _l, double _t, double _r, double _b)
{
    /*
    this.TextClipRect = {
        l : (_l * this.m_oCoordTransform.sx) >> 0,
        t : (_t * this.m_oCoordTransform.sy) >> 0,
        r : (_r * this.m_oCoordTransform.sx) >> 0,
        b : (_b * this.m_oCoordTransform.sy) >> 0
    };
    */
}

void CGraphics::AddSmartRect(double x, double y, double w, double h, int pen_w)
{
    /*
    if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
    {
        // проверим - может все-таки можно нарисовать как надо?
        var r = x + w;
        var b = y + h;
        var dx1 = this.m_oFullTransform.TransformPointX(x, y);
        var dy1 = this.m_oFullTransform.TransformPointY(x, y);

        var dx2 = this.m_oFullTransform.TransformPointX(r, y);
        var dy2 = this.m_oFullTransform.TransformPointY(r, y);

        var dx3 = this.m_oFullTransform.TransformPointX(x, b);
        var dy3 = this.m_oFullTransform.TransformPointY(x, b);

        var dx4 = this.m_oFullTransform.TransformPointX(r, b);
        var dy4 = this.m_oFullTransform.TransformPointY(r, b);

        var _eps = 0.001;
        var bIsClever = false;
        var _type = 1;
        if (Math.abs(dx1 - dx3) < _eps &&
                Math.abs(dx2 - dx4) < _eps &&
                Math.abs(dy1 - dy2) < _eps &&
                Math.abs(dy3 - dy4) < _eps)
        {
            bIsClever = true;
            _type = 1;
        }
        if (!bIsClever &&
                Math.abs(dx1 - dx2) < _eps &&
                Math.abs(dx3 - dx4) < _eps &&
                Math.abs(dy1 - dy3) < _eps &&
                Math.abs(dy2 - dy4) < _eps)
        {
            _type = 2;
            bIsClever = true;
        }

        if (!bIsClever)
        {
            this.ds();
            return;
        }

        var _xI = (_type == 1) ? Math.min(dx1, dx2) : Math.min(dx1, dx3);
        var _rI = (_type == 1) ? Math.max(dx1, dx2) : Math.max(dx1, dx3);
        var _yI = (_type == 1) ? Math.min(dy1, dy3) : Math.min(dy1, dy2);
        var _bI = (_type == 1) ? Math.max(dy1, dy3) : Math.max(dy1, dy2);

        var bIsSmartAttack = false;
        if (!this.m_bIntegerGrid)
        {
            this.SetIntegerGrid(true);
            bIsSmartAttack = true;

            if (this.dash_no_smart)
            {
                for (var index = 0; index < this.dash_no_smart.length; index++)
                    this.dash_no_smart[index] = (this.m_oCoordTransform.sx * this.dash_no_smart[index] + 0.5) >> 0;

                this.m_oContext.setLineDash(this.dash_no_smart);
                this.dash_no_smart = null;
            }
        }

        var _pen_w = (pen_w * this.m_oCoordTransform.sx + 0.5) >> 0;
        if (0 >= _pen_w)
            _pen_w = 1;

        this._s();

        if ((_pen_w & 0x01) == 0x01)
        {
            var _x = (_xI >> 0) + 0.5;
            var _y = (_yI >> 0) + 0.5;
            var _r = (_rI >> 0) + 0.5;
            var _b = (_bI >> 0) + 0.5;

            this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
        }
        else
        {
            var _x = (_xI + 0.5) >> 0;
            var _y = (_yI + 0.5) >> 0;
            var _r = (_rI + 0.5) >> 0;
            var _b = (_bI + 0.5) >> 0;

            this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
        }

        this.m_oContext.lineWidth = _pen_w;
        this.ds();

        if (bIsSmartAttack)
        {
            this.SetIntegerGrid(false);
        }
        return;
    }

    var bIsSmartAttack = false;
    if (!this.m_bIntegerGrid)
    {
        this.SetIntegerGrid(true);
        bIsSmartAttack = true;

        if (this.dash_no_smart)
        {
            for (var index = 0; index < this.dash_no_smart.length; index++)
                this.dash_no_smart[index] = (this.m_oCoordTransform.sx * this.dash_no_smart[index] + 0.5) >> 0;

            this.m_oContext.setLineDash(this.dash_no_smart);
            this.dash_no_smart = null;
        }
    }

    var _pen_w = (pen_w * this.m_oCoordTransform.sx + 0.5) >> 0;
    if (0 >= _pen_w)
        _pen_w = 1;

    this._s();

    if ((_pen_w & 0x01) == 0x01)
    {
        var _x = (this.m_oFullTransform.TransformPointX(x, y) >> 0) + 0.5;
        var _y = (this.m_oFullTransform.TransformPointY(x, y) >> 0) + 0.5;
        var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) >> 0) + 0.5;
        var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) >> 0) + 0.5;

        this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
    }
    else
    {
        var _x = (this.m_oFullTransform.TransformPointX(x, y) + 0.5) >> 0;
        var _y = (this.m_oFullTransform.TransformPointY(x, y) + 0.5) >> 0;
        var _r = (this.m_oFullTransform.TransformPointX(x+w, y+h) + 0.5) >> 0;
        var _b = (this.m_oFullTransform.TransformPointY(x+w, y+h) + 0.5) >> 0;

        this.m_oContext.rect(_x, _y, _r - _x, _b - _y);
    }

    this.m_oContext.lineWidth = _pen_w;
    this.ds();

    if (bIsSmartAttack)
    {
        this.SetIntegerGrid(false);
    }
    */
}

void CGraphics::CheckUseFonts2(const v8::Local<v8::Value>& _transform)
{
    /*
    if (!global_MatrixTransformer.IsIdentity2(_transform))
    {
        if (!AscCommon.g_fontManager2)
        {
            AscCommon.g_fontManager2 = new AscFonts.CFontManager();
            AscCommon.g_fontManager2.Initialize(true);
        }

        this.m_oFontManager2 = AscCommon.g_fontManager2;

        if (null == this.m_oLastFont2)
            this.m_oLastFont2 = new AscCommon.CFontSetup();

        this.IsUseFonts2 = true;
    }
    */
}

void CGraphics::UncheckUseFonts2()
{
    // IsUseFonts2 = false;
}

void CGraphics::DrawPresentationComment(const v8::Local<v8::Value>& type, double x, double y, double w, double h)
{
    /*
    if (this.IsThumbnail || this.IsDemonstrationMode)
        return;

    if (this.m_bIntegerGrid)
    {
        if (AscCommon.g_comment_image && AscCommon.g_comment_image.asc_complete === true)
        {
            var _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0);
            var _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0);

            var _index = 0;
            if ((type & 0x02) == 0x02)
                _index = 2;
            if ((type & 0x01) == 0x01)
                _index += 1;

            if (this.IsRetina)
                _index += 4;

            var _offset = AscCommon.g_comment_image_offsets[_index];
            this.m_oContext.drawImage(AscCommon.g_comment_image, _offset[0], _offset[1], _offset[2], _offset[3], _x, _y, _offset[2], _offset[3]);
        }
    }
    else
    {
        this.SetIntegerGrid(true);
        this.DrawPresentationComment(type, x, y, w, h);
        this.SetIntegerGrid(false);
    }
    */
}

void CGraphics::DrawPolygon(const v8::Local<v8::Value>& oPath, int lineWidth, double shift)
{
    /*
    this.m_oContext.lineWidth = lineWidth;
    this.m_oContext.beginPath();

    var Points    = oPath.Points;

    var nCount = Points.length;
    // берем предпоследнюю точку, т.к. последняя совпадает с первой
    var PrevX = Points[nCount - 2].X, PrevY = Points[nCount - 2].Y;
    var _x = Points[nCount - 2].X,
            _y = Points[nCount - 2].Y;
    var StartX, StartY ;

    for (var nIndex = 0; nIndex < nCount; nIndex++)
    {
        if(PrevX > Points[nIndex].X)
        {
            _y = Points[nIndex].Y - shift;
        }
        else if(PrevX < Points[nIndex].X)
        {
            _y  = Points[nIndex].Y + shift;
        }

        if(PrevY < Points[nIndex].Y)
        {
            _x = Points[nIndex].X - shift;
        }
        else if(PrevY > Points[nIndex].Y)
        {
            _x = Points[nIndex].X + shift;
        }

        PrevX = Points[nIndex].X;
        PrevY = Points[nIndex].Y;

        if(nIndex > 0)
        {
            if (1 == nIndex)
            {
                StartX = _x;
                StartY = _y;
                this._m(_x, _y);
            }
            else
            {
                this._l(_x, _y);
            }
        }
    }

    this._l(StartX, StartY);
    this.m_oContext.closePath();
    this.m_oContext.stroke();
    this.m_oContext.beginPath();
    */
}

void CGraphics::DrawFootnoteRect(double x, double y, double w, double h)
{
    /*
    var _old = this.m_bIntegerGrid;
    if (!_old)
        this.SetIntegerGrid(true);

    this.p_dash([1, 1]);

    this._s();

    var l = x;
    var t = y;
    var r = x + w;
    var b = y + h;

    this.drawHorLineExt(c_oAscLineDrawingRule.Top, t, l, r, 0, 0, 0);
    this.drawVerLine(c_oAscLineDrawingRule.Right, l, t, b, 0);
    this.drawVerLine(c_oAscLineDrawingRule.Left, r, t, b, 0);
    this.drawHorLineExt(c_oAscLineDrawingRule.Top, b, l, r, 0, 0, 0);

    this.ds();
    this._s();

    this.p_dash(null);

    if (!_old)
        this.SetIntegerGrid(false);
    */
}
}
