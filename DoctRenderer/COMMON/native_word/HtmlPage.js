var g_dDpiX = 96.0;
var g_dDpiY = 96.0;

var g_dKoef_mm_to_pix = g_dDpiX / 25.4;
var g_dKoef_pix_to_mm = 25.4 / g_dDpiX;

var g_bIsMobile =  AscBrowser.isMobile;
var g_bIsMouseUpLockedSend = false;

var Page_Width     = 210;
var Page_Height    = 297;

var X_Left_Margin   = 30;  // 3   cm
var X_Right_Margin  = 15;  // 1.5 cm
var Y_Bottom_Margin = 20;  // 2   cm
var Y_Top_Margin    = 20;  // 2   cm

var Y_Default_Header = 12.5; // 1.25 cm расстояние от верха страницы до верха верхнего колонтитула
var Y_Default_Footer = 12.5; // 1.25 cm расстояние от низа страницы до низа нижнего колонтитула

var X_Left_Field   = X_Left_Margin;
var X_Right_Field  = Page_Width  - X_Right_Margin;
var Y_Bottom_Field = Page_Height - Y_Bottom_Margin;
var Y_Top_Field    = Y_Top_Margin;

var docpostype_Content     = 0x00;
var docpostype_FlowObjects = 0x01;
var docpostype_HdrFtr      = 0x02;

var selectionflag_Common        = 0x00;
var selectionflag_Numbering     = 0x01;
var selectionflag_DrawingObject = 0x002;

var orientation_Portrait  = 0x00;
var orientation_Landscape = 0x01;

var tableSpacingMinValue = 0.02;//0.02мм

if (AscBrowser.isIE || window.opera)
{
    // не убирать!!! это для ие. чтобы не селектились элементы
    document.onselectstart= function() {
        return false;
    }
}

function CEditorPage(api)
{
    this.Name = "";

    this.X = 0;
    this.Y = 0;
    this.Width      = 10;
    this.Height     = 10;
    
    this.m_nZoomValue       = 100;
    this.m_oBoundsController = new CBoundsController();
    this.m_nTabsType        = 0;

    this.m_oDrawingDocument = new CDrawingDocument();
    this.m_oDrawingDocument.m_oWordControl = this;

    this.m_oLogicDocument = null;

    this.m_oApi = api;
}