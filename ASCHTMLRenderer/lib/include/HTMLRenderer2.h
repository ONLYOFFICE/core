#ifndef _ASC_HTMLRENDERER2_H_
#define _ASC_HTMLRENDERER2_H_

#include "../../../DesktopEditor/graphics/IRenderer.h"

class CASCHTMLRenderer2_Private;
class CASCHTMLRenderer2 : public IRenderer
{
public:
// тип рендерера-----------------------------------------------------------------------------
    virtual HRESULT get_Type(LONG* lType);
//-------- Функции для работы со страницей --------------------------------------------------
    virtual HRESULT NewPage();
    virtual HRESULT get_Height(double* dHeight);
    virtual HRESULT put_Height(const double& dHeight);
    virtual HRESULT get_Width(double* dWidth);
    virtual HRESULT put_Width(const double& dWidth);
    virtual HRESULT get_DpiX(double* dDpiX);
    virtual HRESULT get_DpiY(double* dDpiY);

// pen --------------------------------------------------------------------------------------
    virtual HRESULT get_PenColor(LONG* lColor);
    virtual HRESULT put_PenColor(const LONG& lColor);
    virtual HRESULT get_PenAlpha(LONG* lAlpha);
    virtual HRESULT put_PenAlpha(const LONG& lAlpha);
    virtual HRESULT get_PenSize(double* dSize);
    virtual HRESULT put_PenSize(const double& dSize);
    virtual HRESULT get_PenDashStyle(BYTE* val);
    virtual HRESULT put_PenDashStyle(const BYTE& val);
    virtual HRESULT get_PenLineStartCap(BYTE* val);
    virtual HRESULT put_PenLineStartCap(const BYTE& val);
    virtual HRESULT get_PenLineEndCap(BYTE* val);
    virtual HRESULT put_PenLineEndCap(const BYTE& val);
    virtual HRESULT get_PenLineJoin(BYTE* val);
    virtual HRESULT put_PenLineJoin(const BYTE& val);
    virtual HRESULT get_PenDashOffset(double* dOffset);
    virtual HRESULT put_PenDashOffset(const double& dOffset);
    virtual HRESULT get_PenAlign(LONG* lAlign);
    virtual HRESULT put_PenAlign(const LONG& lAlign);
    virtual HRESULT get_PenMiterLimit(double* dOffset);
    virtual HRESULT put_PenMiterLimit(const double& dOffset);
    virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

// brush ------------------------------------------------------------------------------------
    virtual HRESULT get_BrushType(LONG* lType);
    virtual HRESULT put_BrushType(const LONG& lType);
    virtual HRESULT get_BrushColor1(LONG* lColor);
    virtual HRESULT put_BrushColor1(const LONG& lColor);
    virtual HRESULT get_BrushAlpha1(LONG* lAlpha);
    virtual HRESULT put_BrushAlpha1(const LONG& lAlpha);
    virtual HRESULT get_BrushColor2(LONG* lColor);
    virtual HRESULT put_BrushColor2(const LONG& lColor);
    virtual HRESULT get_BrushAlpha2(LONG* lAlpha);
    virtual HRESULT put_BrushAlpha2(const LONG& lAlpha);
    virtual HRESULT get_BrushTexturePath(std::wstring* bsPath);
    virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath);
    virtual HRESULT get_BrushTextureMode(LONG* lMode);
    virtual HRESULT put_BrushTextureMode(const LONG& lMode);
    virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha);
    virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha);
    virtual HRESULT get_BrushLinearAngle(double* dAngle);
    virtual HRESULT put_BrushLinearAngle(const double& dAngle);
    virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height);
    virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height);

    virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount);

// font -------------------------------------------------------------------------------------
    virtual HRESULT get_FontName(std::wstring* bsName);
    virtual HRESULT put_FontName(const std::wstring& bsName);
    virtual HRESULT get_FontPath(std::wstring* bsName);
    virtual HRESULT put_FontPath(const std::wstring& bsName);
    virtual HRESULT get_FontSize(double* dSize);
    virtual HRESULT put_FontSize(const double& dSize);
    virtual HRESULT get_FontStyle(LONG* lStyle);
    virtual HRESULT put_FontStyle(const LONG& lStyle);
    virtual HRESULT get_FontStringGID(INT* bGID);
    virtual HRESULT put_FontStringGID(const INT& bGID);
    virtual HRESULT get_FontCharSpace(double* dSpace);
    virtual HRESULT put_FontCharSpace(const double& dSpace);
    virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
    virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);

//-------- Функции для вывода текста --------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);
    virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);

    virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);
    virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);

//-------- Маркеры для команд ---------------------------------------------------------------
    virtual HRESULT BeginCommand(const DWORD& lType);
    virtual HRESULT EndCommand(const DWORD& lType);

//-------- Функции для работы с Graphics Path -----------------------------------------------
    virtual HRESULT PathCommandMoveTo(const double& x, const double& y);
    virtual HRESULT PathCommandLineTo(const double& x, const double& y);
    virtual HRESULT PathCommandLinesTo(double* points, const int& count);
    virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
    virtual HRESULT PathCommandCurvesTo(double* points, const int& count);
    virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
    virtual HRESULT PathCommandClose();
    virtual HRESULT PathCommandEnd();
    virtual HRESULT DrawPath(const LONG& nType);
    virtual HRESULT PathCommandStart();
    virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y);

    virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);
    virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);

    virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);
    virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);

//-------- Функции для вывода изображений ---------------------------------------------------
    virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255);

// transform --------------------------------------------------------------------------------
    virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
    virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
    virtual HRESULT ResetTransform();

// -----------------------------------------------------------------------------------------
    virtual HRESULT get_ClipMode(LONG* plMode);
    virtual HRESULT put_ClipMode(const LONG& lMode);

// additiaonal params ----------------------------------------------------------------------
    virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
    virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
    virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

// owner params ----------------------------------------------------------------------
    virtual HRESULT get_Mode(LONG *plMode);
    virtual HRESULT put_Mode(LONG lMode);

    virtual HRESULT CreateOfficeFile(std::wstring bsFileName);
    virtual HRESULT CloseFile();

    virtual HRESULT SetAdditionalParam(std::string sParamName, int nValue);

protected:
    CASCHTMLRenderer2_Private* m_pInternal;
};

#endif // _ASC_HTMLRENDERER2_H_
