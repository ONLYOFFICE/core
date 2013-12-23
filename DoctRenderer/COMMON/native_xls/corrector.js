CFontFileLoader.prototype.LoadFontNative = function()
{
    var __font_data_idx = g_fonts_streams.length;
    var _data = window.native.GetFontBinary(this.Id);
    g_fonts_streams[__font_data_idx] = new FT_Stream(_data, _data.length);
    this.SetStreamIndex(__font_data_idx);
    this.Status = 0;
};

BinaryFileReader.prototype.ReadNative = function(data)
{
	try
	{
		this.stream = new FT_Stream2(data, data.length);
		this.PreLoadPrepare();
		this.ReadMainTable();
		this.PostLoadPrepare();
	}
	catch(e)
	{
		if(e.message == g_sErrorCharCountMessage)
			return false;
		else
			throw e;
	}
	return true;
};

CChartAsGroup.prototype.draw = function(graphics, pageIndex)
{
    return;
};

function CCacheManager()
{
}

var NATIVE_DOCUMENT_TYPE = "spreadsheet";

var Asc = window["Asc"];
var spreadsheet_api = Asc["spreadsheet_api"];
var asc_CAdjustPrint = Asc["asc_CAdjustPrint"];

$.ready();

$.prototype.clone = function(_clone) { return _clone; };
$.prototype.clean = function() { return []; };
$.prototype.buildFragment = _return_empty_html_element;
$.prototype.init = function(selector, context, rootjQuery)
{
	// Handle $(""), $(null), or $(undefined)
	if ( !selector ) {
		return this;
	}

	// Handle $(DOMElement)
	if ( selector.nodeType ) {
		this.context = this[0] = selector;
		this.length = 1;
		return this;
	}
	
	return _null_object;
};

Asc["DrawingContext"].prototype.setCanvas = function (_canvas) {
    if (!_canvas)
        return;    

	this.canvas = new Array();
	this.canvas.push(_canvas);
	this.ctx = this.canvas[0].getContext("2d");
	if (this.ctx.mozImageSmoothingEnabled) {
		this.ctx.mozImageSmoothingEnabled = false;
	}
};

Asc["WorkbookView"].prototype.init = function (fontRenderingMode) {
	var self = this;
	
	var asc_DC = Asc["DrawingContext"];
	var asc_CE = Asc["CellEditor"];
	var asc_SR = Asc["StringRender"];

	// Init font managers rendering
	// Изначально мы инициализируем c_oAscFontRenderingModeType.hintingAndSubpixeling
	this.setFontRenderingMode(fontRenderingMode, /*isInit*/true);

    this.canvas = new Array(); this.canvas.push(document.createElement("canvas"));
	this.canvasOverlay = new Array(); this.canvasOverlay.push(document.createElement("canvas"));
			
	this.buffers.main = asc_DC({canvas: this.canvas[0], units: 1/*pt*/,
		fmgrGraphics: this.fmgrGraphics, font: this.m_oFont});
	this.buffers.overlay = asc_DC({canvas: this.canvasOverlay[0], units: 1/*pt*/,
		fmgrGraphics: this.fmgrGraphics, font: this.m_oFont});
	this.buffers.overlay.ctx.isOverlay = true;		// Для разруливания _activateOverlayCtx / _deactivateOverlayCtx
	
	this.drawingCtx = this.buffers.main;
	this.overlayCtx = this.buffers.overlay;
	
	// Shapes
	this.buffers.shapeCtx = new CGraphics();
	this.buffers.shapeCtx.init(this.drawingCtx.ctx, this.drawingCtx.getWidth(0), this.drawingCtx.getHeight(0), this.drawingCtx.getWidth(3), this.drawingCtx.getHeight(3));
	this.buffers.shapeCtx.m_oFontManager = this.fmgrGraphics[2];
	
	this.buffers.shapeOverlayCtx = new CGraphics();
	this.buffers.shapeOverlayCtx.init(this.overlayCtx.ctx, this.overlayCtx.getWidth(0), this.overlayCtx.getHeight(0), this.overlayCtx.getWidth(3), this.overlayCtx.getHeight(3));
	this.buffers.shapeOverlayCtx.m_oFontManager = this.fmgrGraphics[2];

	this.drawingCtxCharts	= asc_DC({units: 1/*pt*/, fmgrGraphics: this.fmgrGraphics, font: this.m_oFont});

	this.stringRender		= asc_SR(this.buffers.main);
	this.stringRender.setDefaultFont(this.defaultFont);

	// Мерить нужно только со 100% и один раз для всего документа
	this._calcMaxDigitWidth();

	this.cellEditor = null;

	this.clipboard.Api = this.Api;
	this.clipboard.init();
	return this;
};