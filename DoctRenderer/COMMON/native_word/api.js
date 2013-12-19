/** @define {boolean} */
var ASC_DOCS_API_DEBUG = true;

var ASC_DOCS_API_USE_EMBEDDED_FONTS = "@@ASC_DOCS_API_USE_EMBEDDED_FONTS";

var documentId = undefined;
var documentUrl = 'null';
var documentTitle = 'null';
var documentTitleWithoutExtention = 'null';
var documentFormat = 'null';
var documentVKey = null;
var documentOrigin = "";
var documentFormatSave = c_oAscFileType.DOCX;
var documentFormatSaveTxtCodepage = 65001;//utf8

function CDocOpenProgress()
{
    this.Type = c_oAscAsyncAction.Open;

    this.FontsCount = 0;
    this.CurrentFont = 0;

    this.ImagesCount = 0;
    this.CurrentImage = 0;
}

CDocOpenProgress.prototype.get_Type = function(){return this.Type}
CDocOpenProgress.prototype.get_FontsCount = function(){return this.FontsCount}
CDocOpenProgress.prototype.get_CurrentFont = function(){return this.CurrentFont}
CDocOpenProgress.prototype.get_ImagesCount = function(){return this.ImagesCount}
CDocOpenProgress.prototype.get_CurrentImage = function(){return this.CurrentImage}

function CDocInfo (obj){
	if(obj){
		if (typeof obj.Id != 'undefined'){
			this.Id = obj.Id;
		}
		if (typeof obj.Url != 'undefined'){
			this.Url = obj.Url;
		}
		if (typeof obj.Title != 'undefined'){
			this.Title = obj.Title;
		}
		if (typeof obj.Format != 'undefined'){
			this.Format = obj.Format;
		}
		if (typeof obj.VKey != 'undefined'){
			this.VKey = obj.VKey;
		}
        if (typeof obj.UserId != 'undefined'){
            this.UserId = obj.UserId;
        }
		if (typeof obj.UserName != 'undefined'){
			this.UserName = obj.UserName;
		}
		if (typeof obj.Options != 'undefined'){
			this.Options = obj.Options;
		}
        if (obj.OfflineApp === true)
            this.OfflineApp = true;
	}
	else{
		this.Id = null;
		this.Url = null;
		this.Title = null;
		this.Format = null;
		this.VKey = null;
        this.UserId = null;
		this.UserName = null;
		this.Options = null;
	}
}
CDocInfo.prototype.get_Id = function(){return this.Id}
CDocInfo.prototype.put_Id = function(v){this.Id = v;}
CDocInfo.prototype.get_Url = function(){return this.Url;}
CDocInfo.prototype.put_Url = function(v){this.Url = v;}
CDocInfo.prototype.get_Title = function(){return this.Title;}
CDocInfo.prototype.put_Title = function(v){this.Title = v;}
CDocInfo.prototype.get_Format = function(){return this.Format;}
CDocInfo.prototype.put_Format = function(v){this.Format = v;}
CDocInfo.prototype.get_VKey = function(){return this.VKey;}
CDocInfo.prototype.put_VKey = function(v){this.VKey = v;}
CDocInfo.prototype.get_OfflineApp = function(){return this.OfflineApp;}
CDocInfo.prototype.put_OfflineApp = function(v){this.OfflineApp = v;}
CDocInfo.prototype.get_UserId = function(){return this.UserId;}
CDocInfo.prototype.put_UserId = function(v){this.UserId = v;}
CDocInfo.prototype.get_UserName = function(){return this.UserName;}
CDocInfo.prototype.put_UserName = function(v){this.UserName = v;}
CDocInfo.prototype.get_Options = function(){return this.Options;}
CDocInfo.prototype.put_Options = function(v){this.Options = v;}

function CListType(obj)
{
	if (obj)
	{
		this.Type = (undefined == obj.Type) ? null : obj.Type;
		this.SubType = (undefined == obj.Type) ? null : obj.SubType;
	}
	else
	{
		this.Type = null;
		this.SubType = null;
	}
}
CListType.prototype.get_ListType = function() { return this.Type; }
CListType.prototype.get_ListSubType = function() { return this.SubType; }

function CAscSection()
{
    this.PageWidth = 0;
    this.PageHeight = 0;

    this.MarginLeft = 0;
    this.MarginRight = 0;
    this.MarginTop = 0;
    this.MarginBottom = 0;
}
CAscSection.prototype.get_PageWidth = function() { return this.PageWidth; }
CAscSection.prototype.get_PageHeight = function() { return this.PageHeight; }
CAscSection.prototype.get_MarginLeft = function() { return this.MarginLeft; }
CAscSection.prototype.get_MarginRight = function() { return this.MarginRight; }
CAscSection.prototype.get_MarginTop = function() { return this.MarginTop; }
CAscSection.prototype.get_MarginBottom = function() { return this.MarginBottom; }

function CImagePositionH(obj)
{
    if ( obj )
    {
        this.RelativeFrom = ( undefined === obj.RelativeFrom ) ? undefined : obj.RelativeFrom;
        this.UseAlign     = ( undefined === obj.UseAlign     ) ? undefined : obj.UseAlign;
        this.Align        = ( undefined === obj.Align        ) ? undefined : obj.Align;
        this.Value        = ( undefined === obj.Value        ) ? undefined : obj.Value;
    }
    else
    {
        this.RelativeFrom = undefined;
        this.UseAlign     = undefined;
        this.Align        = undefined;
        this.Value        = undefined;
    }
}

CImagePositionH.prototype.get_RelativeFrom = function()  { return this.RelativeFrom; }
CImagePositionH.prototype.put_RelativeFrom = function(v) { this.RelativeFrom = v; }
CImagePositionH.prototype.get_UseAlign = function()  { return this.UseAlign; }
CImagePositionH.prototype.put_UseAlign = function(v) { this.UseAlign = v; }
CImagePositionH.prototype.get_Align = function()  { return this.Align; }
CImagePositionH.prototype.put_Align = function(v) { this.Align = v; }
CImagePositionH.prototype.get_Value = function()  { return this.Value; }
CImagePositionH.prototype.put_Value = function(v) { this.Value = v; }

function CImagePositionV(obj)
{
    if ( obj )
    {
        this.RelativeFrom = ( undefined === obj.RelativeFrom ) ? undefined : obj.RelativeFrom;
        this.UseAlign     = ( undefined === obj.UseAlign     ) ? undefined : obj.UseAlign;
        this.Align        = ( undefined === obj.Align        ) ? undefined : obj.Align;
        this.Value        = ( undefined === obj.Value        ) ? undefined : obj.Value;
    }
    else
    {
        this.RelativeFrom = undefined;
        this.UseAlign     = undefined;
        this.Align        = undefined;
        this.Value        = undefined;
    }
}

CImagePositionV.prototype.get_RelativeFrom = function()  { return this.RelativeFrom; }
CImagePositionV.prototype.put_RelativeFrom = function(v) { this.RelativeFrom = v; }
CImagePositionV.prototype.get_UseAlign = function()  { return this.UseAlign; }
CImagePositionV.prototype.put_UseAlign = function(v) { this.UseAlign = v; }
CImagePositionV.prototype.get_Align = function()  { return this.Align; }
CImagePositionV.prototype.put_Align = function(v) { this.Align = v; }
CImagePositionV.prototype.get_Value = function()  { return this.Value; }
CImagePositionV.prototype.put_Value = function(v) { this.Value = v; }

function CPosition( obj )
{
	if (obj)
	{
		this.X = (undefined == obj.X) ? null : obj.X;
		this.Y = (undefined == obj.Y) ? null : obj.Y;
	}
	else
	{
		this.X = null;
		this.Y = null;
	}
}
CPosition.prototype.get_X = function() { return this.X; }
CPosition.prototype.put_X = function(v) { this.X = v; }
CPosition.prototype.get_Y = function() { return this.Y; }
CPosition.prototype.put_Y = function(v) { this.Y = v; }

function CPaddings( obj )
{
	if ( obj )
	{
		this.Left = (undefined == obj.Left) ? null : obj.Left;
		this.Top = (undefined == obj.Top) ? null : obj.Top;
		this.Bottom = (undefined == obj.Bottom) ? null : obj.Bottom;
		this.Right = (undefined == obj.Right) ? null : obj.Right;
	}
	else
	{
		this.Left = null;
		this.Top = null;
		this.Bottom = null;
		this.Right = null;
	}
}

CPaddings.prototype.get_Left = function() { return this.Left; }
CPaddings.prototype.put_Left = function(v) { this.Left = v; }
CPaddings.prototype.get_Top = function() { return this.Top; }
CPaddings.prototype.put_Top = function(v) { this.Top = v; }
CPaddings.prototype.get_Bottom = function() { return this.Bottom; }
CPaddings.prototype.put_Bottom = function(v) { this.Bottom = v; }
CPaddings.prototype.get_Right = function() { return this.Right; }
CPaddings.prototype.put_Right = function(v) { this.Right = v; }

function CImageSize( width, height, isCorrect )
{
	this.Width = (undefined == width) ? 0.0 : width;
	this.Height = (undefined == height) ? 0.0 : height;
    this.IsCorrect = isCorrect;
}

CImageSize.prototype.get_ImageWidth = function() { return this.Width; }
CImageSize.prototype.get_ImageHeight = function() { return this.Height; }
CImageSize.prototype.get_IsCorrect = function() { return this.IsCorrect; }

function CImgProperty( obj )
{
	if( obj )
	{
        this.CanBeFlow = (undefined != obj.CanBeFlow) ? obj.CanBeFlow : true;

		this.Width         = (undefined != obj.Width        ) ? obj.Width                          : undefined;
		this.Height        = (undefined != obj.Height       ) ? obj.Height                         : undefined;
		this.WrappingStyle = (undefined != obj.WrappingStyle) ? obj.WrappingStyle                  : undefined;
		this.Paddings      = (undefined != obj.Paddings     ) ? new CPaddings (obj.Paddings)       : undefined;
		this.Position      = (undefined != obj.Position     ) ? new CPosition (obj.Position)       : undefined;
        this.AllowOverlap  = (undefined != obj.AllowOverlap ) ? obj.AllowOverlap                   : undefined;
        this.PositionH     = (undefined != obj.PositionH    ) ? new CImagePositionH(obj.PositionH) : undefined;
        this.PositionV     = (undefined != obj.PositionV    ) ? new CImagePositionV(obj.PositionV) : undefined;

        this.Internal_Position = (undefined != obj.Internal_Position) ? obj.Internal_Position : null;

		this.ImageUrl = (undefined != obj.ImageUrl) ? obj.ImageUrl : null;
        this.Locked   = (undefined != obj.Locked) ? obj.Locked : false;


        this.ChartProperties = (undefined != obj.ChartProperties) ? obj.ChartProperties : null;
        this.ShapeProperties = (undefined != obj.ShapeProperties) ? /*CreateAscShapePropFromProp*/(obj.ShapeProperties) : null;

        this.ChangeLevel = (undefined != obj.ChangeLevel) ? obj.ChangeLevel : null;
        this.Group = (obj.Group != undefined) ? obj.Group : null;

        this.fromGroup = obj.fromGroup != undefined ? obj.fromGroup : null;
        this.severalCharts = obj.severalCharts != undefined ? obj.severalCharts : false;
        this.severalChartTypes = obj.severalChartTypes != undefined ? obj.severalChartTypes : undefined;
        this.severalChartStyles = obj.severalChartStyles != undefined ? obj.severalChartStyles : undefined;
        this.verticalTextAlign = obj.verticalTextAlign != undefined ? obj.verticalTextAlign : undefined;
	}
	else
	{
        this.CanBeFlow = true;
		this.Width         = undefined;
		this.Height        = undefined;
		this.WrappingStyle = undefined;
		this.Paddings      = undefined;
		this.Position      = undefined;
        this.PositionH     = undefined;
        this.PositionV     = undefined;
        this.Internal_Position = null;
        this.ImageUrl = null;
        this.Locked   = false;

        this.ChartProperties = null;
        this.ShapeProperties = null;
        this.ImageProperties = null;

        this.ChangeLevel = null;
        this.Group = null;
        this.fromGroup = null;
        this.severalCharts = false;
        this.severalChartTypes = undefined;
        this.severalChartStyles = undefined;
        this.verticalTextAlign = undefined;
	}
}


CImgProperty.prototype.get_ChangeLevel = function() { return this.ChangeLevel; };
CImgProperty.prototype.put_ChangeLevel = function(v) { this.ChangeLevel = v; };

CImgProperty.prototype.get_CanBeFlow = function() { return this.CanBeFlow; }
CImgProperty.prototype.get_Width = function() { return this.Width; }
CImgProperty.prototype.put_Width = function(v) { this.Width = v; }
CImgProperty.prototype.get_Height = function() { return this.Height; }
CImgProperty.prototype.put_Height = function(v) { this.Height = v; }
CImgProperty.prototype.get_WrappingStyle = function() { return this.WrappingStyle; }
CImgProperty.prototype.put_WrappingStyle = function(v) { this.WrappingStyle = v; }
// Возвращается объект класса CPaddings
CImgProperty.prototype.get_Paddings = function() { return this.Paddings; }
// Аргумент объект класса CPaddings
CImgProperty.prototype.put_Paddings = function(v) { this.Paddings = v; }
CImgProperty.prototype.get_AllowOverlap = function() {return this.AllowOverlap;}
CImgProperty.prototype.put_AllowOverlap = function(v) {this.AllowOverlap = v;}
// Возвращается объект класса CPosition
CImgProperty.prototype.get_Position = function() { return this.Position; }
// Аргумент объект класса CPosition
CImgProperty.prototype.put_Position = function(v) { this.Position = v; }
CImgProperty.prototype.get_PositionH = function()  { return this.PositionH; }
CImgProperty.prototype.put_PositionH = function(v) { this.PositionH = v; }
CImgProperty.prototype.get_PositionV = function()  { return this.PositionV; }
CImgProperty.prototype.put_PositionV = function(v) { this.PositionV = v; }
CImgProperty.prototype.get_Value_X = function(RelativeFrom) { if ( null != this.Internal_Position ) return this.Internal_Position.Calculate_X_Value(RelativeFrom);  return 0; }
CImgProperty.prototype.get_Value_Y = function(RelativeFrom) { if ( null != this.Internal_Position ) return this.Internal_Position.Calculate_Y_Value(RelativeFrom);  return 0; }

CImgProperty.prototype.get_ImageUrl = function() { return this.ImageUrl; }
CImgProperty.prototype.put_ImageUrl = function(v) { this.ImageUrl = v; }
CImgProperty.prototype.get_Group = function() { return this.Group; }
CImgProperty.prototype.put_Group = function(v) { this.Group = v; }
CImgProperty.prototype.get_FromGroup = function() { return this.fromGroup; }
CImgProperty.prototype.put_FromGroup = function(v) { this.fromGroup = v; }

CImgProperty.prototype.get_isChartProps = function() { return this.isChartProps; }
CImgProperty.prototype.put_isChartPross = function(v) { this.isChartProps = v; }


CImgProperty.prototype.get_SeveralCharts = function() { return this.severalCharts; }
CImgProperty.prototype.put_SeveralCharts = function(v) { this.severalCharts = v; }
CImgProperty.prototype.get_SeveralChartTypes = function() { return this.severalChartTypes; }
CImgProperty.prototype.put_SeveralChartTypes = function(v) { this.severalChartTypes = v; }

CImgProperty.prototype.get_SeveralChartStyles = function() { return this.severalChartStyles; }
CImgProperty.prototype.put_SeveralChartStyles = function(v) { this.severalChartStyles = v; }

CImgProperty.prototype.get_VerticalTextAlign = function() { return this.verticalTextAlign; };
CImgProperty.prototype.put_VerticalTextAlign = function(v) { this.verticalTextAlign = v; };

CImgProperty.prototype.get_OriginSize = function(api)
{
    var _image = api.ImageLoader.map_image_index[_getFullImageSrc(this.ImageUrl)];
    if (_image != undefined && _image.Image != null && _image.Status == ImageLoadStatus.Complete)
    {
        var _w = Math.max(1, Page_Width - (X_Left_Margin + X_Right_Margin));
        var _h = Math.max(1, Page_Height - (Y_Top_Margin + Y_Bottom_Margin));

        var bIsCorrect = false;
        if (_image.Image != null)
        {
            var __w = Math.max(parseInt(_image.Image.width * g_dKoef_pix_to_mm), 1);
            var __h = Math.max(parseInt(_image.Image.height * g_dKoef_pix_to_mm), 1);

            var dKoef = Math.max(__w / _w, __h / _h);
            if (dKoef > 1)
            {
                _w = Math.max(5, __w / dKoef);
                _h = Math.max(5, __h / dKoef);

                bIsCorrect = true;
            }
            else
            {
                _w = __w;
                _h = __h;
            }
        }

        return new CImageSize( parseInt(_w), parseInt(_h), bIsCorrect);
    }
    return new CImageSize( 50, 50, false );
}
CImgProperty.prototype.get_Locked = function() { return this.Locked; }

CImgProperty.prototype.get_ChartProperties = function()
{
    return this.ChartProperties;
};

CImgProperty.prototype.put_ChartProperties = function(v)
{
    this.ChartProperties = v;
};

CImgProperty.prototype.get_ShapeProperties = function()
{
    return this.ShapeProperties;
};

CImgProperty.prototype.put_ShapeProperties = function(v)
{
    this.ShapeProperties = v;
};

function CHeaderProp( obj )
{
	/*{
		Type : hdrftr_Footer (hdrftr_Header),
		Position : 12.5,
		DifferentFirst : true/false,
		DifferentEvenOdd : true/false,
	}*/
	if( obj )
	{
		this.Type = (undefined != obj.Type) ? obj.Type: null;
		this.Position = (undefined != obj.Position) ? obj.Position : null;
		this.DifferentFirst = (undefined != obj.DifferentFirst) ? obj.DifferentFirst : null;
		this.DifferentEvenOdd = (undefined != obj.DifferentEvenOdd) ? obj.DifferentEvenOdd : null;
        this.Locked = (undefined != obj.Locked) ? obj.Locked : false;
	}
	else
	{
		this.Type = hdrftr_Footer;
		this.Position = 12.5;
		this.DifferentFirst = false;
		this.DifferentEvenOdd = false;
        this.Locked = false;
	}
}

CHeaderProp.prototype.get_Type = function(){ return this.Type; }
CHeaderProp.prototype.put_Type = function(v){ this.Type = v; }
CHeaderProp.prototype.get_Position = function(){ return this.Position; }
CHeaderProp.prototype.put_Position = function(v){ this.Position = v; }
CHeaderProp.prototype.get_DifferentFirst = function(){ return this.DifferentFirst; }
CHeaderProp.prototype.put_DifferentFirst = function(v){ this.DifferentFirst = v; }
CHeaderProp.prototype.get_DifferentEvenOdd = function(){ return this.DifferentEvenOdd; }
CHeaderProp.prototype.put_DifferentEvenOdd = function(v){ this.DifferentEvenOdd = v; }
CHeaderProp.prototype.get_Locked = function() { return this.Locked; }

function CSelectedObject( type, val )
{
	this.Type = (undefined != type) ? type : null;
	this.Value = (undefined != val) ? val : null;
}
// Возвращает тип объекта c_oAscTypeSelectElement
CSelectedObject.prototype.get_ObjectType = function() { return this.Type; }

// Возвращает собственно сам объект класса в зависимости от типа
// c_oAscTypeSelectElement.Paragraph CParagraphProp
// c_oAscTypeSelectElement.Image CImgProperty
// c_oAscTypeSelectElement.Table CTableProp
// c_oAscTypeSelectElement.Header CHeaderProp
CSelectedObject.prototype.get_ObjectValue = function() { return this.Value; }

CStyleImage.prototype.get_ThumbnailOffset = function() { return this.ThumbnailOffset; }
CStyleImage.prototype.get_Type = function() { return this.Type; }
CStyleImage.prototype.get_Name = function() { return this.Name; }

CFont.prototype.asc_getFontId = function() { return this.id; }
CFont.prototype.asc_getFontName = function() { return this.name; }
CFont.prototype.asc_getFontThumbnail = function() { return this.thumbnail; }
CFont.prototype.asc_getFontType = function() { return this.type; }

var DocumentPageSize = new function() {
    this.oSizes = [{name:"US Letter", w_mm: 215.9, h_mm: 279.4, w_tw: 12240, h_tw: 15840},
        {name:"US Legal", w_mm: 215.9, h_mm: 355.6, w_tw: 12240, h_tw: 20160},
        {name:"A4", w_mm: 210, h_mm: 297, w_tw: 11907, h_tw: 16839},
        {name:"A5", w_mm: 148.1, h_mm: 209.9, w_tw: 8391, h_tw: 11907},
        {name:"B5", w_mm: 176, h_mm: 250.1, w_tw: 9979, h_tw: 14175},
        {name:"Envelope #10", w_mm: 104.8, h_mm: 241.3, w_tw: 5940, h_tw: 13680},
        {name:"Envelope DL", w_mm: 110.1, h_mm: 220.1, w_tw: 6237, h_tw: 12474},
        {name:"Tabloid", w_mm: 279.4, h_mm: 431.7, w_tw: 15842, h_tw: 24477},
        {name:"A3", w_mm: 297, h_mm: 420.1, w_tw: 16840, h_tw: 23820},
        {name:"Tabloid Oversize", w_mm: 304.8, h_mm: 457.1, w_tw: 17282, h_tw: 25918},
        {name:"ROC 16K", w_mm: 196.8, h_mm: 273, w_tw: 11164, h_tw: 15485},
        {name:"Envelope Coukei 3", w_mm: 119.9, h_mm: 234.9, w_tw: 6798, h_tw: 13319},
        {name:"Super B/A3", w_mm: 330.2, h_mm: 482.5, w_tw: 18722, h_tw: 27358}
    ];
    this.sizeEpsMM = 0.5;
    this.getSize = function(widthMm, heightMm)
    {
        for( index in this.oSizes)
        {
            var item = this.oSizes[index];
            if(Math.abs(widthMm - item.w_mm) < this.sizeEpsMM && Math.abs(heightMm - item.h_mm) < this.sizeEpsMM)
                return item;
        }
        return {w_mm: widthMm, h_mm: heightMm};
    };
};

// пользоваться так:
// подрубить его последним из скриптов к страничке
// и вызвать, после подгрузки (конец метода OnInit <- Drawing/HtmlPage.js)
// var _api = new asc_docs_api();
// _api.init(oWordControl);

function asc_docs_api(name)
{
    History    = new CHistory();
    g_oTableId = new CTableId();

	/************ private!!! **************/
    this.HtmlElementName = name;

    this.LoadedObject = null;
    this.DocumentType = 0; // 0 - empty, 1 - test, 2 - document (from json)

    this.DocumentUrl = "";
    this.DocumentName = "";
	this.DocInfo = null;
	this.InterfaceLocale = null;
        
    this.ShowParaMarks = false;
	this.isAddSpaceBetweenPrg = false;
    this.isPageBreakBefore = false;
    this.isKeepLinesTogether = false;

    this.isMobileVersion = false;
    this.isPaintFormat = false;
    this.isMarkerFormat = false;
    this.isViewMode = false;
    this.isStartAddShape = false;
    this.addShapePreset = "";
    this.isShowTableEmptyLine = true;
    this.isShowTableEmptyLineAttack = false;

    this.isApplyChangesOnOpen = false;
    this.isApplyChangesOnOpenEnabled = true;

    // CoAuthoring and Chat
    this.User = undefined;
    this.CoAuthoringApi = null;
	this.isCoAuthoringEnable = true;
    this.isCoMarksDraw = false;

	// Spell Checking
	this.SpellCheckApi = null;
	this.isSpellCheckEnable = true;
	
	// Chart
	this.chartTranslate = new asc_CChartTranslate();

    /**************************************/
    this.DocumentOrientation = orientation_Portrait ? true : false;

    this.SelectedObjectsStack = new Array();
    this.noCreatePoint = false;

    this.CurrentTranslate = translations_map["en"];
    this.CollaborativeMarksShowType = c_oAscCollaborativeMarksShowType.All;

    g_bIsDocumentCopyPaste = true;

	this.chartStyleManager = new ChartStyleManager();
	this.chartPreviewManager = new ChartPreviewManager();

    this.IsLongActionCurrent = false;
    this.ParcedDocument = false;
	this.isStartCoAuthoringOnEndLoad = false;	// Подсоединились раньше, чем документ загрузился
	
	this.isDocumentModify = false;
	
	var oThis = this;

    if (window.editor == undefined)
    {
        window["editor"] = this;
		editor = window.editor;
    }

    this.WordControl = new CEditorPage(this);
    this.WordControl.Name = this.HtmlElementName;

    this.WordControl.m_oLogicDocument   = new CDocument(this.WordControl.m_oDrawingDocument);
    this.WordControl.m_oDrawingDocument.m_oLogicDocument = this.WordControl.m_oLogicDocument;
}

asc_docs_api.prototype.LoadFontsFromServer = function(_fonts)
{
}

asc_docs_api.prototype.SetCollaborativeMarksShowType = function(Type)
{
    this.CollaborativeMarksShowType = Type;
}

asc_docs_api.prototype.GetCollaborativeMarksShowType = function(Type)
{
    return this.CollaborativeMarksShowType;
}

asc_docs_api.prototype.Clear_CollaborativeMarks = function()
{
}

asc_docs_api.prototype.SetLanguage = function(langId)
{
}

asc_docs_api.prototype.TranslateStyleName = function(style_name)
{
}

asc_docs_api.prototype.SetUnchangedDocument = function()
{
    History.Reset_SavedIndex();

    this.isDocumentModify = false;

    this.asc_fireCallback("asc_onDocumentModifiedChanged");
}

asc_docs_api.prototype.isDocumentModified = function()
{
    return this.isDocumentModify;
}

asc_docs_api.prototype.sync_BeginCatchSelectedElements = function()
{
    if (0 != this.SelectedObjectsStack.length)
        this.SelectedObjectsStack.splice(0, this.SelectedObjectsStack.length);
}
asc_docs_api.prototype.sync_EndCatchSelectedElements = function()
{
	if ( (this.chartStyleManager &&  this.chartPreviewManager)&&(!this.chartStyleManager.isReady() || !this.chartPreviewManager.isReady()) )
	{
		for ( var i = 0; i < this.SelectedObjectsStack.length; i++ )
		{
			if ( this.SelectedObjectsStack[i].Value.ChartProperties )
			{
                var is_on = History.Is_On();
                if(is_on)
                {
                    History.TurnOff();
                }
				this.chartStyleManager.init();
				this.chartPreviewManager.init();
				this.asc_fireCallback("asc_onUpdateChartStyles");
                if(is_on)
                {
                    History.TurnOn();
                }
				break;
			}
		}
	}
    this.asc_fireCallback("asc_onFocusObject", this.SelectedObjectsStack);
}
asc_docs_api.prototype.getSelectedElements = function(bUpdate)
{
    if ( true === bUpdate )
        editor.WordControl.m_oLogicDocument.Document_UpdateInterfaceState();

    return this.SelectedObjectsStack;
}
asc_docs_api.prototype.sync_ChangeLastSelectedElement = function(type, obj)
{			
	var oUnkTypeObj = null;
			
	switch( type )
	{
		case c_oAscTypeSelectElement.Paragraph: oUnkTypeObj = new CParagraphProp( obj );
			break;
		case c_oAscTypeSelectElement.Image: oUnkTypeObj = new CImgProperty( obj );
			break;
		case c_oAscTypeSelectElement.Table: oUnkTypeObj = new CTableProp( obj );
			break;
		case c_oAscTypeSelectElement.Header: oUnkTypeObj = new CHeaderProp( obj );
			break;
	}
			
    var _i = this.SelectedObjectsStack.length - 1;
    var bIsFound = false;
    while (_i >= 0)
    {
        if (this.SelectedObjectsStack[_i].Type == type)
        {

            this.SelectedObjectsStack[_i].Value = oUnkTypeObj;
            bIsFound = true;
            break;
        }
        _i--;
    }

    if (!bIsFound)
    {
        this.SelectedObjectsStack[this.SelectedObjectsStack.length] = new CSelectedObject( type, oUnkTypeObj );
    }
}

asc_docs_api.prototype.Init = function()
{
}
asc_docs_api.prototype.asc_getEditorPermissions = function()
{
}

asc_docs_api.prototype.asc_getEditorPermissionsCallback = function(incomeObject)
{	
}
asc_docs_api.prototype.asc_setDocInfo = function(c_DocInfo)
{
}
asc_docs_api.prototype.asc_setLocale = function(val)
{
}
asc_docs_api.prototype.LoadDocument = function(c_DocInfo)
{
}

asc_docs_api.prototype.SetFontsPath = function(path)
{
}

asc_docs_api.prototype.SetTextBoxInputMode = function(bIsEA)
{
}
asc_docs_api.prototype.GetTextBoxInputMode = function()
{
}

asc_docs_api.prototype.ChangeReaderMode = function()
{
}
asc_docs_api.prototype.SetReaderModeOnly = function()
{
}

asc_docs_api.prototype.IncreaseReaderFontSize = function()
{
}
asc_docs_api.prototype.DecreaseReaderFontSize = function()
{
}

asc_docs_api.prototype.CreateCSS = function()
{   
}

asc_docs_api.prototype.CreateComponents = function()
{    
}

asc_docs_api.prototype.GetCopyPasteDivId = function()
{
    return "";
}

asc_docs_api.prototype.ContentToHTML = function(bIsRet)
{
}

asc_docs_api.prototype.InitEditor = function()
{
}

asc_docs_api.prototype.SetInterfaceDrawImagePlaceShape = function(div_id)
{
}

asc_docs_api.prototype.InitViewer = function()
{
}

asc_docs_api.prototype.OpenDocument2 = function(url, gObject)
{
	this.DocumentUrl = url;
	this.DocumentType = 2;
	this.LoadedObjectDS = Common_CopyObj(this.WordControl.m_oLogicDocument.Get_Styles().Style);

	g_oIdCounter.Set_Load(true);

	var openParams = {checkFileSize: this.isMobileVersion, charCount: 0, parCount: 0};
	var oBinaryFileReader = new BinaryFileReader(this.WordControl.m_oLogicDocument, openParams);
	if(oBinaryFileReader.Read(gObject))
	{
		g_oIdCounter.Set_Load(false);
		this.LoadedObject = 1;

		this.sync_EndAction(c_oAscAsyncActionType.BlockInteraction, c_oAscAsyncAction.Open);

		// проверяем какие шрифты нужны
		this.WordControl.m_oDrawingDocument.CheckFontNeeds();
	}
}

asc_docs_api.prototype.OpenDocument = function(url, gObject)
{
	this.DocumentUrl = url;
	this.DocumentType = 2;
	this.LoadedObjectDS = Common_CopyObj(this.WordControl.m_oLogicDocument.Get_Styles().Style);

	g_oIdCounter.Set_Load(true);

	var openParams = {checkFileSize: this.isMobileVersion, charCount: 0, parCount: 0};
	var oBinaryFileReader = new BinaryFileReader(this.WordControl.m_oLogicDocument, openParams);
	if(oBinaryFileReader.ReadNative(gObject))
	{
		g_oIdCounter.Set_Load(false);
		this.LoadedObject = 1;

		this.sync_EndAction(c_oAscAsyncActionType.BlockInteraction, c_oAscAsyncAction.Open);

		// проверяем какие шрифты нужны
		this.WordControl.m_oDrawingDocument.CheckFontNeeds();
	}
}
asc_docs_api.prototype.get_DocumentName = function()
{
	return this.DocumentName;
}
// Callbacks
/* все имена callback'оф начинаются с On. Пока сделаны:
	OnBold,
	OnItalic,
	OnUnderline,
	OnTextPrBaseline(возвращается расположение строки - supstring, superstring, baseline),
	OnPrAlign(выравнивание по ширине, правому краю, левому краю, по центру),
	OnListType( возвращается CListType )

	фейк-функции ожидающие TODO:
	Print,Undo,Redo,Copy,Cut,Paste,Share,Save,Download & callbacks
	OnFontName, OnFontSize, OnLineSpacing

	OnFocusObject( возвращается массив CSelectedObject )
	OnInitEditorStyles( возвращается CStylesPainter )
	OnSearchFound( возвращается CSearchResult );
	OnParaSpacingLine( возвращается CParagraphSpacing )
	OnLineSpacing( не используется? )
	OnTextColor( возвращается CColor )
	OnTextHightLight( возвращается CColor )
	OnInitEditorFonts( возвращается массив объектов СFont )
	OnFontFamily( возвращается CTextFontFamily )
*/
var _callbacks = {};

asc_docs_api.prototype.asc_registerCallback = function(name, callback) {
	if (!_callbacks.hasOwnProperty(name))
		_callbacks[name] = [];
	_callbacks[name].push(callback);
}

asc_docs_api.prototype.asc_unregisterCallback = function(name, callback) {
	if (_callbacks.hasOwnProperty(name)) {
		for (var i = _callbacks[name].length - 1; i >= 0 ; --i) {
			if (_callbacks[name][i] == callback)
				_callbacks[name].splice(i, 1);
		}
	}
		_callbacks[name] = []
	_callbacks[name].push(callback);
}

asc_docs_api.prototype.asc_fireCallback = function(name) {
	if (_callbacks.hasOwnProperty(name))
    {
		for (var i = 0; i < _callbacks[name].length; ++i)
        {
			_callbacks[name][i].apply(this || window, Array.prototype.slice.call(arguments, 1));
		}
        return true;
	}
    return false;
}

asc_docs_api.prototype.asc_checkNeedCallback = function(name) {
    if (_callbacks.hasOwnProperty(name))
    {
        return true;
    }
    return false;
}

// get functions
// Возвращает
//{
// ParaPr :
// {
//    ContextualSpacing : false,            // Удалять ли интервал между параграфами одинакового стиля
//
//    Ind :
//    {
//        Left      : 0,                    // Левый отступ
//        Right     : 0,                    // Правый отступ
//        FirstLine : 0                     // Первая строка
//    },
//
//    Jc : align_Left,                      // Прилегание параграфа
//
//    KeepLines : false,                    // переносить параграф на новую страницу,
//                                          // если на текущей он целиком не убирается
//    KeepNext  : false,                    // переносить параграф вместе со следующим параграфом
//
//    PageBreakBefore : false,              // начинать параграф с новой страницы
//
//    Spacing :
//    {
//        Line     : 1.15,                  // Расстояние между строками внутри абзаца
//        LineRule : linerule_Auto,         // Тип расстрояния между строками
//        Before   : 0,                     // Дополнительное расстояние до абзаца
//        After    : 10 * g_dKoef_pt_to_mm  // Дополнительное расстояние после абзаца
//    },
//
//    Shd :
//    {
//        Value : shd_Nil,
//        Color :
//        {
//            r : 255,
//            g : 255,
//            b : 255
//        }
//    },
//
//    WidowControl : true,                  // Запрет висячих строк
//
//    Tabs : []
// },
//
// TextPr :
// {
//    Bold       : false,
//    Italic     : false,
//    Underline  : false,
//    Strikeout  : false,
//    FontFamily :
//    {
//        Name  : "Times New Roman",
//        Index : -1
//    },
//    FontSize   : 12,
//    Color      :
//    {
//        r : 0,
//        g : 0,
//        b : 0
//    },
//    VertAlign : vertalign_Baseline,
//    HighLight : highlight_None
// }
//}

// Paragraph spacing properties
function CParagraphInd (obj)
{
	if (obj)
	{
		this.Left      = (undefined != obj.Left     ) ? obj.Left      : null; // Левый отступ
		this.Right     = (undefined != obj.Right    ) ? obj.Right     : null; // Правый отступ
		this.FirstLine = (undefined != obj.FirstLine) ? obj.FirstLine : null; // Первая строка
	}
	else
	{
		this.Left      = undefined; // Левый отступ
		this.Right     = undefined; // Правый отступ
		this.FirstLine = undefined; // Первая строка
	}
}
CParagraphInd.prototype.get_Left = function () { return this.Left; }
CParagraphInd.prototype.put_Left = function (v) { this.Left = v; }
CParagraphInd.prototype.get_Right = function () { return this.Right; }
CParagraphInd.prototype.put_Right = function (v) { this.Right = v; }
CParagraphInd.prototype.get_FirstLine = function () { return this.FirstLine; }
CParagraphInd.prototype.put_FirstLine = function (v) { this.FirstLine = v; }


// Paragraph spacing properties
function CParagraphSpacing (obj)
{
	if (obj)
	{
		this.Line     = (undefined != obj.Line    ) ? obj.Line     : null; // Расстояние между строками внутри абзаца
		this.LineRule = (undefined != obj.LineRule) ? obj.LineRule : null; // Тип расстрояния между строками
		this.Before   = (undefined != obj.Before  ) ? obj.Before   : null; // Дополнительное расстояние до абзаца
		this.After    = (undefined != obj.After   ) ? obj.After    : null; // Дополнительное расстояние после абзаца
	}
	else
	{
		this.Line     = undefined; // Расстояние между строками внутри абзаца
		this.LineRule = undefined; // Тип расстрояния между строками
		this.Before   = undefined; // Дополнительное расстояние до абзаца
		this.After    = undefined; // Дополнительное расстояние после абзаца
	}
}
CParagraphSpacing.prototype.get_Line = function ()
{
	return this.Line;
}
CParagraphSpacing.prototype.get_LineRule = function ()
{
	return this.LineRule;
}
CParagraphSpacing.prototype.get_Before = function ()
{
	return this.Before;
}
CParagraphSpacing.prototype.get_After = function ()
{
	return this.After;
}

// Paragraph shd properties
function CParagraphShd (obj)
{
	if (obj)
	{
		this.Value = (undefined != obj.Value) ? obj.Value : null;
		this.Color = (undefined != obj.Color && null != obj.Color) ? CreateAscColorCustom( obj.Color.r, obj.Color.g, obj.Color.b ) : null;
	}
	else
	{
		this.Value = shd_Nil;
		this.Color = CreateAscColorCustom(255, 255, 255);
	}
}
CParagraphShd.prototype.get_Value = function (){ return this.Value; }
CParagraphShd.prototype.put_Value = function (v){ this.Value = v; }
CParagraphShd.prototype.get_Color = function (){ return this.Color; }
CParagraphShd.prototype.put_Color = function (v){ this.Color = (v) ? v : null; }

function CParagraphTab(Pos, Value)
{
    this.Pos   = Pos;
    this.Value = Value;
}
CParagraphTab.prototype.get_Value = function (){ return this.Value; }
CParagraphTab.prototype.put_Value = function (v){ this.Value = v; }
CParagraphTab.prototype.get_Pos = function (){ return this.Pos; }
CParagraphTab.prototype.put_Pos = function (v){ this.Pos = v; }

function CParagraphTabs (obj)
{
    this.Tabs = new Array();

    if ( undefined != obj )
    {
        var Count = obj.Tabs.length;
        for (var Index = 0; Index < Count; Index++)
        {
            this.Tabs.push( new CParagraphTab(obj.Tabs[Index].Pos, obj.Tabs[Index].Value) );
        }
    }
}
CParagraphTabs.prototype.get_Count = function (){ return this.Tabs.length; }
CParagraphTabs.prototype.get_Tab = function (Index){ return this.Tabs[Index]; }
CParagraphTabs.prototype.add_Tab = function (Tab){ this.Tabs.push(Tab) }
CParagraphTabs.prototype.clear = function (){ this.Tabs.length = 0; }

function CParagraphFrame(obj)
{
    if ( obj )
    {
        this.FromDropCapMenu = false;

        this.DropCap = ( dropcap_None === obj.DropCap ? c_oAscDropCap.None : ( dropcap_Drop === obj.DropCap ? c_oAscDropCap.Drop : ( dropcap_Margin === obj.DropCap ? c_oAscDropCap.Margin : undefined ) ) );
        this.H       = obj.H;
        this.HAnchor = obj.HAnchor;
        this.HRule   = ( heightrule_AtLeast ===  obj.HRule ? linerule_AtLeast : ( heightrule_Auto === obj.HRule  ? linerule_Auto : ( heightrule_Exact === obj.HRule ? linerule_Exact : undefined ) ) );
        this.HSpace  = obj.HSpace;
        this.Lines   = obj.Lines;
        this.VAnchor = obj.VAnchor;
        this.VSpace  = obj.VSpace;
        this.W       = obj.W;
        this.Wrap    = ( wrap_Around === obj.Wrap ? true : ( wrap_None === obj.Wrap ? false : undefined ) );
        this.X       = obj.X;
        this.XAlign  = obj.XAlign;
        this.Y       = obj.Y;
        this.YAlign  = obj.YAlign;
        this.Brd     = (undefined != obj.Brd     && null != obj.Brd) ? new CParagraphBorders (obj.Brd) : null;
        this.Shd     = (undefined != obj.Shd     && null != obj.Shd)     ? new CParagraphShd (obj.Shd) : null;
        this.FontFamily = (undefined != obj.FontFamily && null != obj.FontFamily) ? new CTextFontFamily (obj.FontFamily) : null;
    }
    else
    {
        this.FromDropCapMenu = false;

        this.DropCap = undefined;
        this.H       = undefined;
        this.HAnchor = undefined;
        this.HRule   = undefined;
        this.HSpace  = undefined;
        this.Lines   = undefined;
        this.VAnchor = undefined;
        this.VSpace  = undefined;
        this.W       = undefined;
        this.Wrap    = undefined;
        this.X       = undefined;
        this.XAlign  = undefined;
        this.Y       = undefined;
        this.YAlign  = undefined;
        this.Shd     = null;
        this.Brd     = null;
        this.FontFamily = null;
    }
}

CParagraphFrame.prototype.get_DropCap = function () { return this.DropCap; }
CParagraphFrame.prototype.put_DropCap = function (v) { this.DropCap = v; }
CParagraphFrame.prototype.get_H = function () { return this.H; }
CParagraphFrame.prototype.put_H = function (v) { this.H = v; }
CParagraphFrame.prototype.get_HAnchor = function () { return this.HAnchor; }
CParagraphFrame.prototype.put_HAnchor = function (v) { this.HAnchor = v; }
CParagraphFrame.prototype.get_HRule = function () { return this.HRule; }
CParagraphFrame.prototype.put_HRule = function (v) { this.HRule = v; }
CParagraphFrame.prototype.get_HSpace = function () { return this.HSpace; }
CParagraphFrame.prototype.put_HSpace = function (v) { this.HSpace = v; }
CParagraphFrame.prototype.get_Lines = function () { return this.Lines; }
CParagraphFrame.prototype.put_Lines = function (v) { this.Lines = v; }
CParagraphFrame.prototype.get_VAnchor = function () { return this.VAnchor; }
CParagraphFrame.prototype.put_VAnchor = function (v) { this.VAnchor = v; }
CParagraphFrame.prototype.get_VSpace = function () { return this.VSpace; }
CParagraphFrame.prototype.put_VSpace = function (v) { this.VSpace = v; }
CParagraphFrame.prototype.get_W = function () { return this.W; }
CParagraphFrame.prototype.put_W = function (v) { this.W = v; }
CParagraphFrame.prototype.get_Wrap = function () { return this.Wrap; }
CParagraphFrame.prototype.put_Wrap = function (v) { this.Wrap = v; }
CParagraphFrame.prototype.get_X = function () { return this.X; }
CParagraphFrame.prototype.put_X = function (v) { this.X = v; }
CParagraphFrame.prototype.get_XAlign = function () { return this.XAlign; }
CParagraphFrame.prototype.put_XAlign = function (v) { this.XAlign = v; }
CParagraphFrame.prototype.get_Y = function () { return this.Y; }
CParagraphFrame.prototype.put_Y = function (v) { this.Y = v; }
CParagraphFrame.prototype.get_YAlign = function () { return this.YAlign; }
CParagraphFrame.prototype.put_YAlign = function (v) { this.YAlign = v; }
CParagraphFrame.prototype.get_Borders = function () { return this.Brd; }
CParagraphFrame.prototype.put_Borders = function (v) { this.Brd = v; }
CParagraphFrame.prototype.get_Shade = function () { return this.Shd; }
CParagraphFrame.prototype.put_Shade = function (v) { this.Shd = v; }
CParagraphFrame.prototype.get_FontFamily = function () { return this.FontFamily; }
CParagraphFrame.prototype.put_FontFamily = function (v) { this.FontFamily = v; }
CParagraphFrame.prototype.put_FromDropCapMenu = function (v) { this.FromDropCapMenu = v; }

asc_docs_api.prototype.put_FramePr = function(Obj)
{
}

asc_docs_api.prototype.asyncFontEndLoaded_DropCap = function(Obj)
{
}

asc_docs_api.prototype.asc_addDropCap = function(bInText)
{
}

asc_docs_api.prototype.removeDropcap = function(bDropCap)
{
}

function CParagraphProp (obj)
{
	if (obj)
	{
		this.ContextualSpacing = (undefined != obj.ContextualSpacing)              ? obj.ContextualSpacing : null;
		this.Ind               = (undefined != obj.Ind     && null != obj.Ind)     ? new CParagraphInd (obj.Ind) : null;
		this.KeepLines         = (undefined != obj.KeepLines)                      ? obj.KeepLines : null;
        this.KeepNext          = (undefined != obj.KeepNext)                       ? obj.KeepNext  : undefined;
        this.WidowControl      = (undefined != obj.WidowControl                    ? obj.WidowControl : undefined );
		this.PageBreakBefore   = (undefined != obj.PageBreakBefore)                ? obj.PageBreakBefore : null;
		this.Spacing           = (undefined != obj.Spacing && null != obj.Spacing) ? new CParagraphSpacing (obj.Spacing) : null;
		this.Brd               = (undefined != obj.Brd     && null != obj.Brd)     ? new CParagraphBorders (obj.Brd) : null;
		this.Shd               = (undefined != obj.Shd     && null != obj.Shd)     ? new CParagraphShd (obj.Shd) : null;
        this.Tabs              = (undefined != obj.Tabs)                           ? new CParagraphTabs(obj.Tabs) : undefined;
        this.DefaultTab        = Default_Tab_Stop;
        this.Locked            = (undefined != obj.Locked  && null != obj.Locked ) ? obj.Locked : false;
        this.CanAddTable       = (undefined != obj.CanAddTable )                   ? obj.CanAddTable : true;
        this.FramePr           = (undefined != obj.FramePr )                       ? new CParagraphFrame( obj.FramePr ) : undefined;
        this.CanAddDropCap     = (undefined != obj.CanAddDropCap )                 ? obj.CanAddDropCap : false;

        this.Subscript         = (undefined != obj.Subscript)                      ? obj.Subscript   : undefined;
        this.Superscript       = (undefined != obj.Superscript)                    ? obj.Superscript : undefined;
        this.SmallCaps         = (undefined != obj.SmallCaps)                      ? obj.SmallCaps   : undefined;
        this.AllCaps           = (undefined != obj.AllCaps)                        ? obj.AllCaps     : undefined;
        this.Strikeout         = (undefined != obj.Strikeout)                      ? obj.Strikeout   : undefined;
        this.DStrikeout        = (undefined != obj.DStrikeout)                     ? obj.DStrikeout  : undefined;
        this.TextSpacing       = (undefined != obj.TextSpacing)                    ? obj.TextSpacing     : undefined;
        this.Position          = (undefined != obj.Position)                       ? obj.Position    : undefined;
	}
	else
	{
		//ContextualSpacing : false,            // Удалять ли интервал между параграфами одинакового стиля
		//
		//    Ind :
		//    {
		//        Left      : 0,                    // Левый отступ
		//        Right     : 0,                    // Правый отступ
		//        FirstLine : 0                     // Первая строка
		//    },
		//
		//    Jc : align_Left,                      // Прилегание параграфа
		//
		//    KeepLines : false,                    // переносить параграф на новую страницу,
		//                                          // если на текущей он целиком не убирается
		//    KeepNext  : false,                    // переносить параграф вместе со следующим параграфом
		//
		//    PageBreakBefore : false,              // начинать параграф с новой страницы

		this.ContextualSpacing = undefined;
		this.Ind               = new CParagraphInd ();
		this.KeepLines         = undefined;
        this.KeepNext          = undefined;
        this.WidowControl      = undefined;
		this.PageBreakBefore   = undefined;
		this.Spacing           = new CParagraphSpacing ();
		this.Brd               = undefined;
        this.Shd               = undefined;
        this.Locked            = false;
        this.CanAddTable       = true;
        this.Tabs              = undefined;
        this.CanAddDropCap     = false;

        this.Subscript         = undefined;
        this.Superscript       = undefined;
        this.SmallCaps         = undefined;
        this.AllCaps           = undefined;
        this.Strikeout         = undefined;
        this.DStrikeout        = undefined;
        this.TextSpacing       = undefined;
        this.Position          = undefined;
    }
}

CParagraphProp.prototype.get_ContextualSpacing = function () { return this.ContextualSpacing; }
CParagraphProp.prototype.put_ContextualSpacing = function (v) { this.ContextualSpacing = v; }
CParagraphProp.prototype.get_Ind = function () { return this.Ind; }
CParagraphProp.prototype.put_Ind = function (v) { this.Ind = v; }
CParagraphProp.prototype.get_KeepLines = function () { return this.KeepLines; }
CParagraphProp.prototype.put_KeepLines = function (v) { this.KeepLines = v; }
CParagraphProp.prototype.get_KeepNext = function () { return this.KeepNext; }
CParagraphProp.prototype.put_KeepNext = function (v) { this.KeepNext = v; }
CParagraphProp.prototype.get_PageBreakBefore = function (){ return this.PageBreakBefore; }
CParagraphProp.prototype.put_PageBreakBefore = function (v){ this.PageBreakBefore = v; }
CParagraphProp.prototype.get_WidowControl = function (){ return this.WidowControl; }
CParagraphProp.prototype.put_WidowControl = function (v){ this.WidowControl = v; }
CParagraphProp.prototype.get_Spacing = function () { return this.Spacing; }
CParagraphProp.prototype.put_Spacing = function (v) { this.Spacing = v; }
CParagraphProp.prototype.get_Borders = function () { return this.Brd; }
CParagraphProp.prototype.put_Borders = function (v) { this.Brd = v; }
CParagraphProp.prototype.get_Shade = function () { return this.Shd; }
CParagraphProp.prototype.put_Shade = function (v) { this.Shd = v; }
CParagraphProp.prototype.get_Locked = function() { return this.Locked; }
CParagraphProp.prototype.get_CanAddTable = function() { return this.CanAddTable; }
CParagraphProp.prototype.get_Subscript = function () { return this.Subscript; }
CParagraphProp.prototype.put_Subscript = function (v) { this.Subscript = v; }
CParagraphProp.prototype.get_Superscript = function () { return this.Superscript; }
CParagraphProp.prototype.put_Superscript = function (v) { this.Superscript = v; }
CParagraphProp.prototype.get_SmallCaps = function () { return this.SmallCaps; }
CParagraphProp.prototype.put_SmallCaps = function (v) { this.SmallCaps = v; }
CParagraphProp.prototype.get_AllCaps = function () { return this.AllCaps; }
CParagraphProp.prototype.put_AllCaps = function (v) { this.AllCaps = v; }
CParagraphProp.prototype.get_Strikeout = function () { return this.Strikeout; }
CParagraphProp.prototype.put_Strikeout = function (v) { this.Strikeout = v; }
CParagraphProp.prototype.get_DStrikeout = function () { return this.DStrikeout; }
CParagraphProp.prototype.put_DStrikeout = function (v) { this.DStrikeout = v; }
CParagraphProp.prototype.get_TextSpacing = function () { return this.TextSpacing; }
CParagraphProp.prototype.put_TextSpacing = function (v) { this.TextSpacing = v; }
CParagraphProp.prototype.get_Position = function () { return this.Position; }
CParagraphProp.prototype.put_Position = function (v) { this.Position = v; }
CParagraphProp.prototype.get_Tabs = function () { return this.Tabs; }
CParagraphProp.prototype.put_Tabs = function (v) { this.Tabs = v; }
CParagraphProp.prototype.get_DefaultTab = function () { return this.DefaultTab; }
CParagraphProp.prototype.put_DefaultTab = function (v) { this.DefaultTab = v; }
CParagraphProp.prototype.get_FramePr = function () { return this.FramePr; }
CParagraphProp.prototype.put_FramePr = function (v) { this.FramePr = v; }
CParagraphProp.prototype.get_CanAddDropCap = function() { return this.CanAddDropCap; }

// Paragraph properties
function CParagraphPropEx (obj)
{
	if (obj)
	{
		this.ContextualSpacing = (undefined != obj.ContextualSpacing) ? obj.ContextualSpacing : null;
		this.Ind = (undefined != obj.Ind && null != obj.Ind) ? new CParagraphInd (obj.Ind) : null;
		this.Jc = (undefined != obj.Jc) ? obj.Jc : null;
		this.KeepLines = (undefined != obj.KeepLines) ? obj.KeepLines : null;
		this.KeepNext = (undefined != obj.KeepNext) ? obj.KeepNext : null;
		this.PageBreakBefore = (undefined != obj.PageBreakBefore) ? obj.PageBreakBefore : null;
		this.Spacing = (undefined != obj.Spacing && null != obj.Spacing) ? new CParagraphSpacing (obj.Spacing) : null;
		this.Shd = (undefined != obj.Shd && null != obj.Shd) ? new CParagraphShd (obj.Shd) : null;
		this.WidowControl = (undefined != obj.WidowControl) ? obj.WidowControl : null;                  // Запрет висячих строк
		this.Tabs = obj.Tabs;
	}
	else
	{
		//ContextualSpacing : false,            // Удалять ли интервал между параграфами одинакового стиля
		//
		//    Ind :
		//    {
		//        Left      : 0,                    // Левый отступ
		//        Right     : 0,                    // Правый отступ
		//        FirstLine : 0                     // Первая строка
		//    },
		//
		//    Jc : align_Left,                      // Прилегание параграфа
		//
		//    KeepLines : false,                    // переносить параграф на новую страницу,
		//                                          // если на текущей он целиком не убирается
		//    KeepNext  : false,                    // переносить параграф вместе со следующим параграфом
		//
		//    PageBreakBefore : false,              // начинать параграф с новой страницы
		this.ContextualSpacing = false;
		this.Ind = new CParagraphInd ();
		this.Jc = align_Left;
		this.KeepLines = false;
		this.KeepNext = false;
		this.PageBreakBefore = false;
		this.Spacing = new CParagraphSpacing ();
		this.Shd = new CParagraphShd ();
		this.WidowControl = true;                  // Запрет висячих строк
		this.Tabs = null;
	}
}
CParagraphPropEx.prototype.get_ContextualSpacing = function ()
{
	return this.ContextualSpacing;
}
CParagraphPropEx.prototype.get_Ind = function ()
{
	return this.Ind;
}
CParagraphPropEx.prototype.get_Jc = function ()
{
	return this.Jc;
}
CParagraphPropEx.prototype.get_KeepLines = function ()
{
	return this.KeepLines;
}
CParagraphPropEx.prototype.get_KeepNext = function ()
{
	return this.KeepNext;
}
CParagraphPropEx.prototype.get_PageBreakBefore = function ()
{
	return this.PageBreakBefore;
}
CParagraphPropEx.prototype.get_Spacing = function ()
{
	return this.Spacing;
}
CParagraphPropEx.prototype.get_Shd = function ()
{
	return this.Shd;
}
CParagraphPropEx.prototype.get_WidowControl = function ()
{
	return this.WidowControl;
}
CParagraphPropEx.prototype.get_Tabs = function ()
{
	return this.Tabs;
}

// Text properties
// TextPr :
// {
//    Bold       : false,
//    Italic     : false,
//    Underline  : false,
//    Strikeout  : false,
//    FontFamily :
//    {
//        Name  : "Times New Roman",
//        Index : -1
//    },
//    FontSize   : 12,
//    Color      :
//    {
//        r : 0,
//        g : 0,
//        b : 0
//    },
//    VertAlign : vertalign_Baseline,
//    HighLight : highlight_None
// }
function CTextFontFamily (obj)
{
	if (obj)
	{
		this.Name = (undefined != obj.Name) ? obj.Name : null; 		// "Times New Roman"
		this.Index = (undefined != obj.Index) ? obj.Index : null;	// -1
	}
	else
	{
		this.Name = "Times New Roman";
		this.Index = -1;
	}
}
CTextFontFamily.prototype.get_Name = function ()
{
	return this.Name;
}
CTextFontFamily.prototype.get_Index = function ()
{
	return this.Index;
}
// CTextProp
function CTextProp (obj)
{
	if (obj)
	{
		this.Bold       = (undefined != obj.Bold) ? obj.Bold : null;
		this.Italic     = (undefined != obj.Italic) ? obj.Italic : null;
		this.Underline  = (undefined != obj.Underline) ? obj.Underline : null;
		this.Strikeout  = (undefined != obj.Strikeout) ? obj.Strikeout : null;
		this.FontFamily = (undefined != obj.FontFamily && null != obj.FontFamily) ? new CTextFontFamily (obj.FontFamily) : null;
		this.FontSize   = (undefined != obj.FontSize) ? obj.FontSize : null;
		this.Color      = (undefined != obj.Color && null != obj.Color) ? CreateAscColorCustom(obj.Color.r, obj.Color.g, obj.Color.b) : null;
		this.VertAlign  = (undefined != obj.VertAlign) ? obj.VertAlign : null;
		this.HighLight  = (undefined != obj.HighLight) ? obj.HighLight == highlight_None ? obj.HighLight : new CColor (obj.HighLight.r, obj.HighLight.g, obj.HighLight.b) : null;
        this.DStrikeout = (undefined != obj.DStrikeout) ? obj.DStrikeout : null;
        this.Spacing    = (undefined != obj.Spacing)    ? obj.Spacing    : null;
        this.Caps       = (undefined != obj.Caps)       ? obj.Caps       : null;
        this.SmallCaps  = (undefined != obj.SmallCaps)  ? obj.SmallCaps  : null;
	}
	else
	{
		//    Bold       : false,
		//    Italic     : false,
		//    Underline  : false,
		//    Strikeout  : false,
		//    FontFamily :
		//    {
		//        Name  : "Times New Roman",
		//        Index : -1
		//    },
		//    FontSize   : 12,
		//    Color      :
		//    {
		//        r : 0,
		//        g : 0,
		//        b : 0
		//    },
		//    VertAlign : vertalign_Baseline,
		//    HighLight : highlight_None
		this.Bold       = false;
		this.Italic     = false;
		this.Underline  = false;
		this.Strikeout  = false;
		this.FontFamily = new CTextFontFamily();
		this.FontSize   = 12;
		this.Color      = CreateAscColorCustom(0, 0, 0);
		this.VertAlign  = vertalign_Baseline;
		this.HighLight  = highlight_None;
        this.DStrikeout = false;
        this.Spacing    = 0;
        this.Caps       = false;
        this.SmallCaps  = false;
    }
}
CTextProp.prototype.get_Bold = function ()
{
	return this.Bold;
}
CTextProp.prototype.get_Italic = function ()
{
	return this.Italic;
}
CTextProp.prototype.get_Underline = function ()
{
	return this.Underline;
}
CTextProp.prototype.get_Strikeout = function ()
{
	return this.Strikeout;
}
CTextProp.prototype.get_FontFamily = function ()
{
	return this.FontFamily;
}
CTextProp.prototype.get_FontSize = function ()
{
	return this.FontSize;
}
CTextProp.prototype.get_Color = function ()
{
	return this.Color;
}
CTextProp.prototype.get_VertAlign = function ()
{
	return this.VertAlign;
}
CTextProp.prototype.get_HighLight = function ()
{
	return this.HighLight;
}

CTextProp.prototype.get_Spacing = function ()
{
    return this.Spacing;
}

CTextProp.prototype.get_DStrikeout = function ()
{
    return this.DStrikeout;
}

CTextProp.prototype.get_Caps = function ()
{
    return this.Caps;
}

CTextProp.prototype.get_SmallCaps = function ()
{
    return this.SmallCaps;
}


// paragraph and text properties objects container
function CParagraphAndTextProp (paragraphProp, textProp)
{
	this.ParaPr = (undefined != paragraphProp && null != paragraphProp) ? new CParagraphPropEx (paragraphProp) : null;
	this.TextPr = (undefined != textProp && null != textProp) ? new CTextProp (textProp) : null;
}
CParagraphAndTextProp.prototype.get_ParaPr = function ()
{
	return this.ParaPr;
}
CParagraphAndTextProp.prototype.get_TextPr = function ()
{
	return this.TextPr;
}

//
asc_docs_api.prototype.get_TextProps = function()
{
	var Doc = this.WordControl.m_oLogicDocument;
	var ParaPr = Doc.Get_Paragraph_ParaPr();
	var TextPr = Doc.Get_Paragraph_TextPr();

	// return { ParaPr: ParaPr, TextPr : TextPr };
	return new CParagraphAndTextProp (ParaPr, TextPr);	// uncomment if this method will be used externally. 20/03/2012 uncommented for testers
}

asc_docs_api.prototype.get_ContentCount = function()
{
	return this.WordControl.m_oLogicDocument.Content.length;
}

asc_docs_api.prototype.select_Element = function(Index)
{
}

asc_docs_api.prototype.UpdateTextPr = function(TextPr)
{
	if ( "undefined" != typeof(TextPr) )
	{
		var oTextPrMap =
        {
			Bold       : function(oThis, v){ oThis.sync_BoldCallBack(v); },
			Italic     : function(oThis, v){ oThis.sync_ItalicCallBack(v); },
			Underline  : function(oThis, v){ oThis.sync_UnderlineCallBack(v); },
			Strikeout  : function(oThis, v){ oThis.sync_StrikeoutCallBack(v); },
			FontSize   : function(oThis, v){ oThis.sync_TextPrFontSizeCallBack(v); },
			FontFamily : function(oThis, v){ oThis.sync_TextPrFontFamilyCallBack(v); },
			VertAlign  : function(oThis, v){ oThis.sync_VerticalAlign(v); },
			Color      : function(oThis, v){ oThis.sync_TextColor(v); },
			HighLight  : function(oThis, v){ oThis.sync_TextHighLight(v); },
            Spacing    : function(oThis, v){ oThis.sync_TextSpacing(v); },
            DStrikeout : function(oThis, v){ oThis.sync_TextDStrikeout(v); },
            Caps       : function(oThis, v){ oThis.sync_TextCaps(v); },
            SmallCaps  : function(oThis, v){ oThis.sync_TextSmallCaps(v); },
            Position   : function(oThis, v){ oThis.sync_TextPosition(v); },
            Lang       : function(oThis, v){ oThis.sync_TextLangCallBack(v);}
		}

        for ( var Item in oTextPrMap )
        {
            oTextPrMap[Item]( this, TextPr[Item] );
        }
	}
}
asc_docs_api.prototype.UpdateParagraphProp = function(ParaPr)
{
}

/*----------------------------------------------------------------*/
/*functions for working with clipboard, document*/
/*TODO: Print,Undo,Redo,Copy,Cut,Paste,Share,Save,DownloadAs,ReturnToDocuments(вернуться на предыдущую страницу) & callbacks for these functions*/
asc_docs_api.prototype.asc_Print = function()
{	
}
asc_docs_api.prototype.Undo = function()
{
}
asc_docs_api.prototype.Redo = function()
{
}
asc_docs_api.prototype.Copy = function()
{
}
asc_docs_api.prototype.Update_ParaTab = function(Default_Tab, ParaTabs)
{
}
asc_docs_api.prototype.Cut = function()
{
}
asc_docs_api.prototype.Paste = function()
{    
}
asc_docs_api.prototype.Share = function(){
}
asc_docs_api.prototype.asc_Save = function (isAutoSave) {
};

asc_docs_api.prototype.asc_OnSaveEnd = function (isDocumentSaved) {
};

function safe_Apply_Changes()
{
    try
    {
        CollaborativeEditing.Apply_Changes();
    }
    catch(err)
    {
    }
}

asc_docs_api.prototype.asc_DownloadAs = function(typeFile){
}
asc_docs_api.prototype.Resize = function(){
}
asc_docs_api.prototype.AddURL = function(url){
}
asc_docs_api.prototype.Help = function(){
}
asc_docs_api.prototype.ClearCache = function(){
}

asc_docs_api.prototype.SetFontRenderingMode = function(mode)
{
}
asc_docs_api.prototype.processSavedFile = function(url, bInner)
{
}
asc_docs_api.prototype.startGetDocInfo = function(){	
}
asc_docs_api.prototype.stopGetDocInfo = function(){
}
asc_docs_api.prototype.sync_DocInfoCallback = function(obj){
	this.asc_fireCallback( "asc_onDocInfo", new CDocInfoProp(obj));
}
asc_docs_api.prototype.sync_GetDocInfoStartCallback = function(){
	this.asc_fireCallback("asc_onGetDocInfoStart");
}
asc_docs_api.prototype.sync_GetDocInfoStopCallback = function(){
	this.asc_fireCallback("asc_onGetDocInfoStop");
}
asc_docs_api.prototype.sync_GetDocInfoEndCallback = function(){
	this.asc_fireCallback("asc_onGetDocInfoEnd");
}
asc_docs_api.prototype.sync_CanUndoCallback = function(bCanUndo)
{
    this.asc_fireCallback("asc_onCanUndo", bCanUndo);
}
asc_docs_api.prototype.sync_CanRedoCallback = function(bCanRedo)
{
    this.asc_fireCallback("asc_onCanRedo", bCanRedo);
}

asc_docs_api.prototype.setStartPointHistory = function(){this.noCreatePoint = true; History.Create_NewPoint();};
asc_docs_api.prototype.setEndPointHistory   = function(){this.noCreatePoint = false; };

function CDocInfoProp(obj)
{
	if(obj){
		this.PageCount = obj.PageCount;
		this.WordsCount = obj.WordsCount;
		this.ParagraphCount = obj.ParagraphCount;
		this.SymbolsCount = obj.SymbolsCount;
		this.SymbolsWSCount = obj.SymbolsWSCount;
	}
	else {
		this.PageCount = -1;
		this.WordsCount = -1;
		this.ParagraphCount = -1;
		this.SymbolsCount = -1;
		this.SymbolsWSCount = -1;
	}
}
CDocInfoProp.prototype.get_PageCount = function(){ return this.PageCount; }
CDocInfoProp.prototype.put_PageCount = function(v){ this.PageCount = v; }
CDocInfoProp.prototype.get_WordsCount = function(){ return this.WordsCount; }
CDocInfoProp.prototype.put_WordsCount = function(v){ this.WordsCount = v; }
CDocInfoProp.prototype.get_ParagraphCount = function(){ return this.ParagraphCount; }
CDocInfoProp.prototype.put_ParagraphCount = function(v){ this.ParagraphCount = v; }
CDocInfoProp.prototype.get_SymbolsCount = function(){ return this.SymbolsCount; }
CDocInfoProp.prototype.put_SymbolsCount = function(v){ this.SymbolsCount = v; }
CDocInfoProp.prototype.get_SymbolsWSCount = function(){ return this.SymbolsWSCount; }
CDocInfoProp.prototype.put_SymbolsWSCount = function(v){ this.SymbolsWSCount = v; }

/*callbacks*/
/*asc_docs_api.prototype.sync_CursorLockCallBack = function(isLock){
	this.asc_fireCallback("asc_onCursorLock",isLock);
}*/
asc_docs_api.prototype.sync_PrintCallBack = function(){
	this.asc_fireCallback("asc_onPrint");
}
asc_docs_api.prototype.sync_UndoCallBack = function(){
	this.asc_fireCallback("asc_onUndo");
}
asc_docs_api.prototype.sync_RedoCallBack = function(){
	this.asc_fireCallback("asc_onRedo");
}
asc_docs_api.prototype.sync_CopyCallBack = function(){
	this.asc_fireCallback("asc_onCopy");
}
asc_docs_api.prototype.sync_CutCallBack = function(){
	this.asc_fireCallback("asc_onCut");
}
asc_docs_api.prototype.sync_PasteCallBack = function(){
	this.asc_fireCallback("asc_onPaste");
}
asc_docs_api.prototype.sync_ShareCallBack = function(){
	this.asc_fireCallback("asc_onShare");
}
asc_docs_api.prototype.sync_SaveCallBack = function(){
	this.asc_fireCallback("asc_onSave");
}
asc_docs_api.prototype.sync_DownloadAsCallBack = function(){
	this.asc_fireCallback("asc_onDownload");
}
asc_docs_api.prototype.sync_StartAction = function(type, id){
	//this.AsyncAction
	this.asc_fireCallback("asc_onStartAction", type, id);
	//console.log("asc_onStartAction: type = " + type + " id = " + id);

    if (c_oAscAsyncActionType.BlockInteraction == type)
        this.IsLongActionCurrent = true;
}
asc_docs_api.prototype.sync_EndAction = function(type, id){
	//this.AsyncAction
	this.asc_fireCallback("asc_onEndAction", type, id);
	//console.log("asc_onEndAction: type = " + type + " id = " + id);

    if (c_oAscAsyncActionType.BlockInteraction == type)
        this.IsLongActionCurrent = false;
}
asc_docs_api.prototype.sync_AddURLCallback = function(){
	this.asc_fireCallback("asc_onAddURL");
}
asc_docs_api.prototype.sync_ErrorCallback = function(errorID,errorLevel){
	this.asc_fireCallback("asc_onError",errorID,errorLevel);
}
asc_docs_api.prototype.sync_HelpCallback = function(url){
	this.asc_fireCallback("asc_onHelp",url);
}
asc_docs_api.prototype.sync_UpdateZoom = function(zoom){
	this.asc_fireCallback("asc_onZoom", zoom);
}
asc_docs_api.prototype.sync_StatusMessage = function(message){
	this.asc_fireCallback("asc_onMessage", message);
}
asc_docs_api.prototype.ClearPropObjCallback = function(prop){//колбэк предшествующий приходу свойств объекта, prop а всякий случай
	this.asc_fireCallback("asc_onClearPropObj", prop);
}

/*----------------------------------------------------------------*/
/*functions for working with headers*/
/*
	структура заголовков, предварительно, выглядит так
	{
		headerText: "Header1",//заголовок
		pageNumber: 0, //содержит номер страницы, где находится искомая последовательность
		X: 0,//координаты по OX начала последовательности на данной страницы
		Y: 0,//координаты по OY начала последовательности на данной страницы
		level: 0//уровень заголовка
	}
	заголовки приходят либо в списке, либо последовательно.
*/
// CHeader
function CHeader (obj)
{
	if (obj)
	{
		this.headerText = (undefined != obj.headerText) ? obj.headerText : null;	//заголовок
		this.pageNumber = (undefined != obj.pageNumber) ? obj.pageNumber : null;	//содержит номер страницы, где находится искомая последовательность
		this.X = (undefined != obj.X) ? obj.X : null;								//координаты по OX начала последовательности на данной страницы
		this.Y = (undefined != obj.Y) ? obj.Y : null;								//координаты по OY начала последовательности на данной страницы
		this.level = (undefined != obj.level) ? obj.level : null;					//позиция заголовка
	}
	else
	{
		this.headerText = null;				//заголовок
		this.pageNumber = null;				//содержит номер страницы, где находится искомая последовательность
		this.X = null;						//координаты по OX начала последовательности на данной страницы
		this.Y = null;						//координаты по OY начала последовательности на данной страницы
		this.level = null;					//позиция заголовка
	}
}
CHeader.prototype.get_headerText = function ()
{
	return this.headerText;
}
CHeader.prototype.get_pageNumber = function ()
{
	return this.pageNumber;
}
CHeader.prototype.get_X = function ()
{
	return this.X;
}
CHeader.prototype.get_Y = function ()
{
	return this.Y;
}
CHeader.prototype.get_Level = function ()
{
	return this.level;
}
var _fakeHeaders = [
	new CHeader ({headerText: "Header1", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header2", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header3", pageNumber: 0, X: 0, Y: 0, level: 2}),
	new CHeader ({headerText: "Header4", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 2}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 3}),
	new CHeader ({headerText: "Header3", pageNumber: 0, X: 0, Y: 0, level: 4}),
	new CHeader ({headerText: "Header3", pageNumber: 0, X: 0, Y: 0, level: 5}),
	new CHeader ({headerText: "Header3", pageNumber: 0, X: 0, Y: 0, level: 6}),
	new CHeader ({headerText: "Header4", pageNumber: 0, X: 0, Y: 0, level: 7}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 8}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 2}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 3}),
	new CHeader ({headerText: "Header6", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 0}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 1}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 0}),
	new CHeader ({headerText: "Header5", pageNumber: 0, X: 0, Y: 0, level: 0})
]

asc_docs_api.prototype.CollectHeaders = function(){
}
asc_docs_api.prototype.GetActiveHeader = function(){

}
asc_docs_api.prototype.gotoHeader = function(page, X, Y){
}
asc_docs_api.prototype.sync_ChangeActiveHeaderCallback = function (position, header){
}
asc_docs_api.prototype.sync_ReturnHeadersCallback = function (headers){
}
/*----------------------------------------------------------------*/
/*functions for working with search*/
/*
	структура поиска, предварительно, выглядит так
	{
		text: "...<b>слово поиска</b>...",
		pageNumber: 0, //содержит номер страницы, где находится искомая последовательность
		X: 0,//координаты по OX начала последовательности на данной страницы
		Y: 0//координаты по OY начала последовательности на данной страницы
	}
*/

asc_docs_api.prototype.asc_searchEnabled = function(bIsEnabled)
{
}

asc_docs_api.prototype.asc_findText = function(text, isNext, isMatchCase)
{
}

asc_docs_api.prototype.asc_replaceText = function(text, replaceWith, isReplaceAll, isMatchCase)
{    
}

asc_docs_api.prototype.asc_selectSearchingResults = function(bShow)
{
}

asc_docs_api.prototype.asc_isSelectSearchingResults = function()
{
}

asc_docs_api.prototype.sync_ReplaceAllCallback = function(ReplaceCount, OverallCount)
{
}

asc_docs_api.prototype.sync_SearchEndCallback = function()
{
}
/*----------------------------------------------------------------*/
/*functions for working with font*/
/*setters*/
asc_docs_api.prototype.put_TextPrFontName = function(name)
{
}
asc_docs_api.prototype.put_TextPrFontSize = function(size)
{
}
asc_docs_api.prototype.put_TextPrBold = function(value)
{
}
asc_docs_api.prototype.put_TextPrItalic = function(value)
{
}
asc_docs_api.prototype.put_TextPrUnderline = function(value)
{
}
asc_docs_api.prototype.put_TextPrStrikeout = function(value)
{
}
asc_docs_api.prototype.put_TextPrDStrikeout = function(value)
{
}
asc_docs_api.prototype.put_TextPrSpacing = function(value)
{
}

asc_docs_api.prototype.put_TextPrCaps = function(value)
{
}

asc_docs_api.prototype.put_TextPrSmallCaps = function(value)
{
}


asc_docs_api.prototype.put_TextPrPosition = function(value)
{
}

asc_docs_api.prototype.put_TextPrLang = function(value)
{
}


asc_docs_api.prototype.put_PrLineSpacing = function(Type, Value)
{
}
asc_docs_api.prototype.put_LineSpacingBeforeAfter = function(type,value)//"type == 0" means "Before", "type == 1" means "After"
{
}
asc_docs_api.prototype.FontSizeIn = function()
{
}
asc_docs_api.prototype.FontSizeOut = function()
{
}
// Object:
// {
//    Bottom :
//    {
//        Color : { r : 0, g : 0, b : 0 },
//        Value : border_Single,
//        Size  : 0.5 * g_dKoef_pt_to_mm
//        Space : 0
//    },
//    Left :
//    {
//        ....
//    }
//    Right :
//    {
//        ....
//    }
//    Top :
//    {
//        ....
//    }
//    },
//    Between :
//    {
//        ....
//    }
// }

function CParagraphBorders (obj)
{
	if (obj)
	{
		this.Left = (undefined != obj.Left && null != obj.Left) ? new CBorder (obj.Left) : null;
		this.Top = (undefined != obj.Top && null != obj.Top) ? new CBorder (obj.Top) : null;
		this.Right = (undefined != obj.Right && null != obj.Right) ? new CBorder (obj.Right) : null;
		this.Bottom = (undefined != obj.Bottom && null != obj.Bottom) ? new CBorder (obj.Bottom) : null;
		this.Between = (undefined != obj.Between && null != obj.Between) ? new CBorder (obj.Between) : null;
	}
	else
	{
		this.Left = null;
		this.Top = null;
		this.Right = null;
		this.Bottom = null;
		this.Between = null;
	}
}
CParagraphBorders.prototype.get_Left = function(){return this.Left; }
CParagraphBorders.prototype.put_Left = function(v){this.Left = (v) ? new CBorder (v) : null;}
CParagraphBorders.prototype.get_Top = function(){return this.Top; }
CParagraphBorders.prototype.put_Top = function(v){this.Top = (v) ? new CBorder (v) : null;}
CParagraphBorders.prototype.get_Right = function(){return this.Right; }
CParagraphBorders.prototype.put_Right = function(v){this.Right = (v) ? new CBorder (v) : null;}
CParagraphBorders.prototype.get_Bottom = function(){return this.Bottom; }
CParagraphBorders.prototype.put_Bottom = function(v){this.Bottom = (v) ? new CBorder (v) : null;}
CParagraphBorders.prototype.get_Between = function(){return this.Between; }
CParagraphBorders.prototype.put_Between = function(v){this.Between = (v) ? new CBorder (v) : null;}

asc_docs_api.prototype.put_Borders = function(Obj)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphBorders( Obj );
    }
}
/*callbacks*/
asc_docs_api.prototype.sync_BoldCallBack = function(isBold){
	this.asc_fireCallback("asc_onBold",isBold);
}
asc_docs_api.prototype.sync_ItalicCallBack = function(isItalic){
	this.asc_fireCallback("asc_onItalic",isItalic);
}
asc_docs_api.prototype.sync_UnderlineCallBack = function(isUnderline){
	this.asc_fireCallback("asc_onUnderline",isUnderline);
}
asc_docs_api.prototype.sync_StrikeoutCallBack = function(isStrikeout){
	this.asc_fireCallback("asc_onStrikeout",isStrikeout);
}
asc_docs_api.prototype.sync_TextPrFontFamilyCallBack = function(FontFamily)
{
    if ( undefined != FontFamily )
	    this.asc_fireCallback("asc_onFontFamily", new CTextFontFamily( FontFamily ));
    else
        this.asc_fireCallback("asc_onFontFamily", new CTextFontFamily( { Name : "", Index : -1 } ));
}
asc_docs_api.prototype.sync_TextPrFontSizeCallBack = function(FontSize){
	this.asc_fireCallback("asc_onFontSize",FontSize);
}
asc_docs_api.prototype.sync_PrLineSpacingCallBack = function(LineSpacing){
    this.asc_fireCallback("asc_onLineSpacing", new CParagraphInd( LineSpacing ) );
}
asc_docs_api.prototype.sync_InitEditorFonts = function(gui_fonts){
    this._gui_fonts = gui_fonts;
}
asc_docs_api.prototype.sync_InitEditorStyles = function(styles_painter){
    this._gui_styles = styles_painter;
}
asc_docs_api.prototype.sync_InitEditorTableStyles = function(styles, is_retina_enabled){
    this.asc_fireCallback("asc_onInitTableTemplates",styles, is_retina_enabled);
}


/*----------------------------------------------------------------*/
/*functions for working with paragraph*/
/*setters*/
// Right = 0; Left = 1; Center = 2; Justify = 3; or using enum that written above

/* структура для параграфа
	Ind :
   	{
       	Left      : 0,                    // Левый отступ
       	Right     : 0,                    // Правый отступ
     	FirstLine : 0                     // Первая строка
   	}
   	Spacing :
   	{
       	Line     : 1.15,                  // Расстояние между строками внутри абзаца
       	LineRule : linerule_Auto,         // Тип расстрояния между строками
       	Before   : 0,                     // Дополнительное расстояние до абзаца
       	After    : 10 * g_dKoef_pt_to_mm  // Дополнительное расстояние после абзаца
   	},
   	KeepLines : false,                    // переносить параграф на новую страницу,
                                         // если на текущей он целиком не убирается
   	PageBreakBefore : false
*/

asc_docs_api.prototype.paraApply = function(Props)
{    
}

asc_docs_api.prototype.put_PrAlign = function(value)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphAlign(value);
    }
}
// 0- baseline, 2-subscript, 1-superscript
asc_docs_api.prototype.put_TextPrBaseline = function(value)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Content) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { VertAlign : value } ) );
    }
}
/*
    Во всех случаях SubType = 0 означает, что нажали просто на кнопку
    c выбором типа списка, без выбора подтипа.

 	Маркированный список Type = 0
		нет          - SubType = -1
		черная точка - SubType = 1
		круг         - SubType = 2
		квадрат      - SubType = 3
		картинка     - SubType = -1
		4 ромба      - SubType = 4
		ч/б стрелка  - SubType = 5
		галка        - SubType = 6
		ромб         - SubType = 7

	Нумерованный список Type = 1
		нет - SubType = -1
		1.  - SubType = 1
		1)  - SubType = 2
		I.  - SubType = 3
		A.  - SubType = 4
		a)  - SubType = 5
		a.  - SubType = 6
		i.  - SubType = 7

	Многоуровневый список Type = 2
		нет           - SubType = -1
		1)a)i)        - SubType = 1
		1.1.1         - SubType = 2
		маркированный - SubType = 3
*/
asc_docs_api.prototype.put_ListType = function(type, subtype)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        var NumberInfo =
        {
            Type    : 0,
            SubType : -1
        };

        NumberInfo.Type = type;
        NumberInfo.SubType = subtype;
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphNumbering( NumberInfo );
    }
}
asc_docs_api.prototype.put_Style = function(name)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphStyle(name);
    }
}

asc_docs_api.prototype.SetDeviceInputHelperId = function(idKeyboard)
{
}

asc_docs_api.prototype.put_ShowParaMarks = function(isShow)
{
}
asc_docs_api.prototype.get_ShowParaMarks = function(){
}
asc_docs_api.prototype.put_ShowTableEmptyLine = function(isShow)
{
}
asc_docs_api.prototype.get_ShowTableEmptyLine = function(){
}
asc_docs_api.prototype.put_PageBreak = function(isBreak)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.isPageBreakBefore = isBreak;
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphPageBreakBefore(isBreak);
        this.sync_PageBreakCallback(isBreak);
    }
}

asc_docs_api.prototype.put_WidowControl = function(bValue)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphWidowControl(bValue);
        this.sync_WidowControlCallback(bValue);
    }
}

asc_docs_api.prototype.put_KeepLines = function(isKeepLines)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.isKeepLinesTogether = isKeepLines;
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphKeepLines(isKeepLines);
        this.sync_KeepLinesCallback(isKeepLines);
    }
}

asc_docs_api.prototype.put_KeepNext = function(isKeepNext)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphKeepNext(isKeepNext);
        this.sync_KeepNextCallback(isKeepNext);
    }
}

asc_docs_api.prototype.put_AddSpaceBetweenPrg = function(isSpacePrg)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.isAddSpaceBetweenPrg = isSpacePrg;
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphContextualSpacing(isSpacePrg);
    }
}
asc_docs_api.prototype.put_LineHighLight = function(is_flag, r, g, b)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Content) )
    {
        if (false === is_flag)
        {
            this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
            this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { HighLight : highlight_None  } ) );
        }
        else
        {
            this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
            this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { HighLight :  { r : r, g : g, b: b}  } ) );
        }
    }
}
asc_docs_api.prototype.put_TextColor = function(color)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Content) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Color :  { r : color.get_r(), g : color.get_g(), b : color.get_b(), Auto : color.get_auto() }  } ) );

        if ( true === this.isMarkerFormat )
            this.sync_MarkerFormatCallback( false );
    }
}
asc_docs_api.prototype.put_ParagraphShade = function(is_flag, color)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        if (false === is_flag)
            this.WordControl.m_oLogicDocument.Set_ParagraphShd( { Value : shd_Nil  }  );
        else
        {
            this.WordControl.m_oLogicDocument.Set_ParagraphShd( { Value : shd_Clear, Color : { r : color.get_r(), g : color.get_g(), b : color.get_b() } } );
        }
    }
}
asc_docs_api.prototype.put_PrIndent = function(value,levelValue)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { Left : value, ChangeLevel: levelValue } );
    }
}
asc_docs_api.prototype.IncreaseIndent = function()
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Paragraph_IncDecIndent( true );
    }
}
asc_docs_api.prototype.DecreaseIndent = function()
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Paragraph_IncDecIndent( false );
    }
}
asc_docs_api.prototype.put_PrIndentRight = function(value)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { Right : value } );
    }
}
asc_docs_api.prototype.put_PrFirstLineIndent = function(value)
{
    if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Paragraph_Properties) )
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	    this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { FirstLine : value } );
    }
}
asc_docs_api.prototype.put_Margins = function(left, top, right, bottom)
{
	this.WordControl.m_oLogicDocument.Set_DocumentMargin( { Left : left, Top : top, Right : right, Bottom : bottom });
}
asc_docs_api.prototype.getFocusObject = function(){//возвратит тип элемента - параграф c_oAscTypeSelectElement.Paragraph, изображение c_oAscTypeSelectElement.Image, таблица c_oAscTypeSelectElement.Table, колонтитул c_oAscTypeSelectElement.Header.

}

/*callbacks*/
asc_docs_api.prototype.sync_VerticalAlign = function(typeBaseline){
}
asc_docs_api.prototype.sync_PrAlignCallBack = function(value){
}
asc_docs_api.prototype.sync_ListType = function(NumPr){
}
asc_docs_api.prototype.sync_TextColor = function(Color)
{
}
asc_docs_api.prototype.sync_TextHighLight = function(HighLight)
{
}
asc_docs_api.prototype.sync_TextSpacing = function(Spacing)
{
}
asc_docs_api.prototype.sync_TextDStrikeout = function(Value)
{
}
asc_docs_api.prototype.sync_TextCaps = function(Value)
{
}
asc_docs_api.prototype.sync_TextSmallCaps = function(Value)
{
}
asc_docs_api.prototype.sync_TextPosition = function(Value)
{
}
asc_docs_api.prototype.sync_TextLangCallBack = function(Lang)
{
}
asc_docs_api.prototype.sync_ParaStyleName = function(Name){
}
asc_docs_api.prototype.sync_ParaSpacingLine = function(SpacingLine)
{
}
asc_docs_api.prototype.sync_PageBreakCallback = function(isBreak){
}

asc_docs_api.prototype.sync_WidowControlCallback = function(bValue)
{
}

asc_docs_api.prototype.sync_KeepNextCallback = function(bValue)
{
}

asc_docs_api.prototype.sync_KeepLinesCallback = function(isKeepLines){
}
asc_docs_api.prototype.sync_ShowParaMarksCallback = function(){
}
asc_docs_api.prototype.sync_SpaceBetweenPrgCallback = function(){
}
asc_docs_api.prototype.sync_PrPropCallback = function(prProp){
}

asc_docs_api.prototype.sync_EndAddShape = function()
{
}

asc_docs_api.prototype.SetDrawingFreeze = function(bIsFreeze)
{
}

/*----------------------------------------------------------------*/
/*functions for working with page*/
asc_docs_api.prototype.change_PageOrient = function(isPortrait)
{
}
asc_docs_api.prototype.get_DocumentOrientation = function()
{
}
asc_docs_api.prototype.change_DocSize = function(width,height)
{
}

asc_docs_api.prototype.get_DocumentWidth = function()
{
    return Page_Width;
}

asc_docs_api.prototype.get_DocumentHeight = function()
{
    return Page_Height;
}

asc_docs_api.prototype.put_AddPageBreak = function()
{
}
asc_docs_api.prototype.Update_ParaInd = function( Ind ){
}
asc_docs_api.prototype.Internal_Update_Ind_FirstLine = function(FirstLine,Left){
}
asc_docs_api.prototype.Internal_Update_Ind_Left = function(Left){
}
asc_docs_api.prototype.Internal_Update_Ind_Right = function(Right){
}

// "where" где нижний или верхний, align выравнивание
asc_docs_api.prototype.put_PageNum = function(where,align)
{
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.put_HeadersAndFooters = function(where,options)
{
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.rem_HeadersAndFooters = function(where,options)
{
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.put_HeadersAndFootersDistance = function(value)
{
}

asc_docs_api.prototype.HeadersAndFooters_DifferentFirstPage = function(isOn)
{
}
asc_docs_api.prototype.HeadersAndFooters_DifferentOddandEvenPage = function(isOn)
{
}

/*структура для передачи настроек колонтитулов
	{
		Type : hdrftr_Footer (hdrftr_Header),
		Position : 12.5,
		DifferentFirst : true/false,
		DifferentEvenOdd : true/false,
	}
*/
/*callback*/
asc_docs_api.prototype.sync_DocSizeCallback = function(width,height){
}
asc_docs_api.prototype.sync_PageOrientCallback = function(isPortrait){
}
asc_docs_api.prototype.sync_HeadersAndFootersPropCallback = function(hafProp)
{
}

/*----------------------------------------------------------------*/
/*functions for working with table*/
asc_docs_api.prototype.put_Table = function(col,row,isFlow)
{
}
asc_docs_api.prototype.addRowAbove = function(count)
{
}
asc_docs_api.prototype.addRowBelow = function(count)
{
}
asc_docs_api.prototype.addColumnLeft = function(count)
{
}
asc_docs_api.prototype.addColumnRight = function(count)
{
}
asc_docs_api.prototype.remRow = function()
{
}
asc_docs_api.prototype.remColumn = function()
{
}
asc_docs_api.prototype.remTable = function()
{
}
asc_docs_api.prototype.selectRow = function()
{
}
asc_docs_api.prototype.selectColumn = function()
{
}
asc_docs_api.prototype.selectCell = function()
{
}
asc_docs_api.prototype.selectTable = function()
{
}
asc_docs_api.prototype.setColumnWidth = function(width){

}
asc_docs_api.prototype.setRowHeight = function(height){

}
asc_docs_api.prototype.set_TblDistanceFromText = function(left,top,right,bottom){

}
asc_docs_api.prototype.CheckBeforeMergeCells = function()
{
}
asc_docs_api.prototype.CheckBeforeSplitCells = function()
{
}
asc_docs_api.prototype.MergeCells = function()
{
}
asc_docs_api.prototype.SplitCell = function(Cols, Rows)
{
}
asc_docs_api.prototype.widthTable = function(width){

}
asc_docs_api.prototype.put_CellsMargin = function(left,top,right,bottom){

}
asc_docs_api.prototype.set_TblWrap = function(type){

}
asc_docs_api.prototype.set_TblIndentLeft = function(spacing){

}
asc_docs_api.prototype.set_Borders = function(typeBorders,size,Color){//если size == 0 то границы нет.

}
asc_docs_api.prototype.set_TableBackground = function(Color){

}
asc_docs_api.prototype.set_AlignCell = function(align){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
}
asc_docs_api.prototype.set_TblAlign = function(align){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
}
asc_docs_api.prototype.set_SpacingBetweenCells = function(isOn,spacing){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
}

// CBackground
// Value : тип заливки(прозрачная или нет),
// Color : { r : 0, g : 0, b : 0 }
function CBackground (obj)
{
	if (obj)
	{
		this.Color = (undefined != obj.Color && null != obj.Color) ? CreateAscColorCustom(obj.Color.r, obj.Color.g, obj.Color.b) : null;
		this.Value = (undefined != obj.Value) ? obj.Value : null;
	}
	else
	{
		this.Color = CreateAscColorCustom(0, 0, 0);
		this.Value = 1;
	}
}
CBackground.prototype.get_Color = function (){return this.Color;}
CBackground.prototype.put_Color = function (v){this.Color = (v) ? v: null;}
CBackground.prototype.get_Value = function (){return this.Value;}
CBackground.prototype.put_Value = function (v){this.Value = v;}

function CTablePositionH(obj)
{
    if ( obj )
    {
        this.RelativeFrom = ( undefined === obj.RelativeFrom ) ? c_oAscHAnchor.Margin : obj.RelativeFrom;
        this.UseAlign     = ( undefined === obj.UseAlign     ) ? false                : obj.UseAlign;
        this.Align        = ( undefined === obj.Align        ) ? undefined            : obj.Align;
        this.Value        = ( undefined === obj.Value        ) ? 0                    : obj.Value;
    }
    else
    {
        this.RelativeFrom = c_oAscHAnchor.Column;
        this.UseAlign     = false;
        this.Align        = undefined;
        this.Value        = 0;
    }
}

CTablePositionH.prototype.get_RelativeFrom = function()  { return this.RelativeFrom; }
CTablePositionH.prototype.put_RelativeFrom = function(v) { this.RelativeFrom = v; }
CTablePositionH.prototype.get_UseAlign = function()  { return this.UseAlign; }
CTablePositionH.prototype.put_UseAlign = function(v) { this.UseAlign = v; }
CTablePositionH.prototype.get_Align = function()  { return this.Align; }
CTablePositionH.prototype.put_Align = function(v) { this.Align = v; }
CTablePositionH.prototype.get_Value = function()  { return this.Value; }
CTablePositionH.prototype.put_Value = function(v) { this.Value = v; }

function CTablePositionV(obj)
{
    if ( obj )
    {
        this.RelativeFrom = ( undefined === obj.RelativeFrom ) ? c_oAscVAnchor.Text : obj.RelativeFrom;
        this.UseAlign     = ( undefined === obj.UseAlign     ) ? false              : obj.UseAlign;
        this.Align        = ( undefined === obj.Align        ) ? undefined          : obj.Align;
        this.Value        = ( undefined === obj.Value        ) ? 0                  : obj.Value;
    }
    else
    {
        this.RelativeFrom = c_oAscVAnchor.Text;
        this.UseAlign     = false;
        this.Align        = undefined;
        this.Value        = 0;
    }
}

CTablePositionV.prototype.get_RelativeFrom = function()  { return this.RelativeFrom; }
CTablePositionV.prototype.put_RelativeFrom = function(v) { this.RelativeFrom = v; }
CTablePositionV.prototype.get_UseAlign = function()  { return this.UseAlign; }
CTablePositionV.prototype.put_UseAlign = function(v) { this.UseAlign = v; }
CTablePositionV.prototype.get_Align = function()  { return this.Align; }
CTablePositionV.prototype.put_Align = function(v) { this.Align = v; }
CTablePositionV.prototype.get_Value = function()  { return this.Value; }
CTablePositionV.prototype.put_Value = function(v) { this.Value = v; }

function CTablePropLook(obj)
{
    this.FirstCol = false;
    this.FirstRow = false;
    this.LastCol  = false;
    this.LastRow  = false;
    this.BandHor  = false;
    this.BandVer  = false;

    if ( obj )
    {
        this.FirstCol = ( undefined === obj.m_bFirst_Col ? false : obj.m_bFirst_Col );
        this.FirstRow = ( undefined === obj.m_bFirst_Row ? false : obj.m_bFirst_Row );
        this.LastCol  = ( undefined === obj.m_bLast_Col  ? false : obj.m_bLast_Col );
        this.LastRow  = ( undefined === obj.m_bLast_Row  ? false : obj.m_bLast_Row );
        this.BandHor  = ( undefined === obj.m_bBand_Hor  ? false : obj.m_bBand_Hor );
        this.BandVer  = ( undefined === obj.m_bBand_Ver  ? false : obj.m_bBand_Ver );
    }
}

CTablePropLook.prototype.get_FirstCol = function() {return this.FirstCol;}
CTablePropLook.prototype.put_FirstCol = function(v) {this.FirstCol = v;}
CTablePropLook.prototype.get_FirstRow = function() {return this.FirstRow;}
CTablePropLook.prototype.put_FirstRow = function(v) {this.FirstRow = v;}
CTablePropLook.prototype.get_LastCol = function() {return this.LastCol;}
CTablePropLook.prototype.put_LastCol = function(v) {this.LastCol = v;}
CTablePropLook.prototype.get_LastRow = function() {return this.LastRow;}
CTablePropLook.prototype.put_LastRow = function(v) {this.LastRow = v;}
CTablePropLook.prototype.get_BandHor = function() {return this.BandHor;}
CTablePropLook.prototype.put_BandHor = function(v) {this.BandHor = v;}
CTablePropLook.prototype.get_BandVer = function() {return this.BandVer;}
CTablePropLook.prototype.put_BandVer = function(v) {this.BandVer = v;}

function CTableProp (tblProp)
{
	if (tblProp)
	{
        this.CanBeFlow = (undefined != tblProp.CanBeFlow ? tblProp.CanBeFlow : false );
        this.CellSelect = (undefined != tblProp.CellSelect ? tblProp.CellSelect : false );
        this.CellSelect = (undefined != tblProp.CellSelect) ? tblProp.CellSelect : false;
		this.TableWidth = (undefined != tblProp.TableWidth) ? tblProp.TableWidth : null;
		this.TableSpacing = (undefined != tblProp.TableSpacing) ? tblProp.TableSpacing : null;
		this.TableDefaultMargins = (undefined != tblProp.TableDefaultMargins && null != tblProp.TableDefaultMargins) ? new CPaddings (tblProp.TableDefaultMargins) : null;

		this.CellMargins = (undefined != tblProp.CellMargins && null != tblProp.CellMargins) ? new CMargins (tblProp.CellMargins) : null;

		this.TableAlignment = (undefined != tblProp.TableAlignment) ? tblProp.TableAlignment : null;
		this.TableIndent = (undefined != tblProp.TableIndent) ? tblProp.TableIndent : null;
		this.TableWrappingStyle = (undefined != tblProp.TableWrappingStyle) ? tblProp.TableWrappingStyle : null;

		this.TablePaddings = (undefined != tblProp.TablePaddings && null != tblProp.TablePaddings) ? new CPaddings (tblProp.TablePaddings) : null;

		this.TableBorders = (undefined != tblProp.TableBorders && null != tblProp.TableBorders) ? new CBorders (tblProp.TableBorders) : null;
		this.CellBorders = (undefined != tblProp.CellBorders && null != tblProp.CellBorders) ? new CBorders (tblProp.CellBorders) : null;
		this.TableBackground = (undefined != tblProp.TableBackground && null != tblProp.TableBackground) ? new CBackground (tblProp.TableBackground) : null;
		this.CellsBackground = (undefined != tblProp.CellsBackground && null != tblProp.CellsBackground) ? new CBackground (tblProp.CellsBackground) : null;
		this.Position = (undefined != tblProp.Position && null != tblProp.Position) ? new CPosition (tblProp.Position) : null;
        this.PositionH = ( undefined != tblProp.PositionH && null != tblProp.PositionH ) ? new CTablePositionH(tblProp.PositionH) : undefined;
        this.PositionV = ( undefined != tblProp.PositionV && null != tblProp.PositionV ) ? new CTablePositionV(tblProp.PositionV) : undefined;
        this.Internal_Position = ( undefined != tblProp.Internal_Position ) ? tblProp.Internal_Position : undefined;

        this.ForSelectedCells = (undefined != tblProp.ForSelectedCells) ? tblProp.ForSelectedCells : true;
        this.TableStyle = (undefined != tblProp.TableStyle) ? tblProp.TableStyle : null;
        this.TableLook = (undefined != tblProp.TableLook) ? new CTablePropLook(tblProp.TableLook) : null;
        this.RowsInHeader = (undefined != tblProp.RowsInHeader) ? tblProp.RowsInHeader : 0;
        this.CellsVAlign = (undefined != tblProp.CellsVAlign) ? tblProp.CellsVAlign :c_oAscVertAlignJc.Top;
        this.AllowOverlap = (undefined != tblProp.AllowOverlap) ? tblProp.AllowOverlap : undefined;
        this.TableLayout  = tblProp.TableLayout;
        this.Locked = (undefined != tblProp.Locked) ? tblProp.Locked : false;
	}
	else
	{
		//Все свойства класса CTableProp должны быть undefined если они не изменялись
        //this.CanBeFlow = false;
        this.CellSelect = false; //обязательное свойство
		/*this.TableWidth = null;
		this.TableSpacing = null;
		this.TableDefaultMargins = new CPaddings ();

		this.CellMargins = new CMargins ();

		this.TableAlignment = 0;
		this.TableIndent = 0;
		this.TableWrappingStyle = c_oAscWrapStyle.Inline;

		this.TablePaddings = new CPaddings ();

		this.TableBorders = new CBorders ();
		this.CellBorders = new CBorders ();
		this.TableBackground = new CBackground ();
		this.CellsBackground = new CBackground ();;
		this.Position = new CPosition ();
		this.ForSelectedCells = true;*/

        this.Locked = false;
	}
}

CTableProp.prototype.get_Width = function (){return this.TableWidth;}
CTableProp.prototype.put_Width = function (v){this.TableWidth = v;}
CTableProp.prototype.get_Spacing = function (){return this.TableSpacing;}
CTableProp.prototype.put_Spacing = function (v){this.TableSpacing = v;}
CTableProp.prototype.get_DefaultMargins = function (){return this.TableDefaultMargins;}
CTableProp.prototype.put_DefaultMargins = function (v){this.TableDefaultMargins = v;}
CTableProp.prototype.get_CellMargins = function (){return this.CellMargins;}
CTableProp.prototype.put_CellMargins = function (v){ this.CellMargins = v;}
CTableProp.prototype.get_TableAlignment = function (){return this.TableAlignment;}
CTableProp.prototype.put_TableAlignment = function (v){this.TableAlignment = v;}
CTableProp.prototype.get_TableIndent = function (){return this.TableIndent;}
CTableProp.prototype.put_TableIndent = function (v){this.TableIndent = v;}
CTableProp.prototype.get_TableWrap = function (){return this.TableWrappingStyle;}
CTableProp.prototype.put_TableWrap = function (v){this.TableWrappingStyle = v;}
CTableProp.prototype.get_TablePaddings = function (){return this.TablePaddings;}
CTableProp.prototype.put_TablePaddings = function (v){this.TablePaddings = v;}
CTableProp.prototype.get_TableBorders = function (){return this.TableBorders;}
CTableProp.prototype.put_TableBorders = function (v){this.TableBorders = v;}
CTableProp.prototype.get_CellBorders = function (){return this.CellBorders;}
CTableProp.prototype.put_CellBorders = function (v){this.CellBorders = v;}
CTableProp.prototype.get_TableBackground = function (){return this.TableBackground;}
CTableProp.prototype.put_TableBackground = function (v){this.TableBackground = v;}
CTableProp.prototype.get_CellsBackground = function (){return this.CellsBackground;}
CTableProp.prototype.put_CellsBackground = function (v){this.CellsBackground = v;}
CTableProp.prototype.get_Position = function (){return this.Position;}
CTableProp.prototype.put_Position = function (v){this.Position = v;}
CTableProp.prototype.get_PositionH = function(){return this.PositionH;}
CTableProp.prototype.put_PositionH = function(v){this.PositionH = v;}
CTableProp.prototype.get_PositionV = function(){return this.PositionV;}
CTableProp.prototype.put_PositionV = function(v){this.PositionV = v;}
CTableProp.prototype.get_Value_X = function(RelativeFrom) { if ( undefined != this.Internal_Position ) return this.Internal_Position.Calculate_X_Value(RelativeFrom);  return 0; }
CTableProp.prototype.get_Value_Y = function(RelativeFrom) { if ( undefined != this.Internal_Position ) return this.Internal_Position.Calculate_Y_Value(RelativeFrom);  return 0; }
CTableProp.prototype.get_ForSelectedCells = function (){return this.ForSelectedCells;}
CTableProp.prototype.put_ForSelectedCells = function (v){this.ForSelectedCells = v;}
CTableProp.prototype.put_CellSelect = function(v){this.CellSelect = v;}
CTableProp.prototype.get_CellSelect = function(){return this.CellSelect};
CTableProp.prototype.get_CanBeFlow = function(){return this.CanBeFlow;}
CTableProp.prototype.get_RowsInHeader = function(){return this.RowsInHeader;};
CTableProp.prototype.put_RowsInHeader = function(v){this.RowsInHeader = v;};
CTableProp.prototype.get_Locked = function() { return this.Locked; }
CTableProp.prototype.get_CellsVAlign = function() { return this.CellsVAlign; }
CTableProp.prototype.put_CellsVAlign = function(v){ this.CellsVAlign = v; }
CTableProp.prototype.get_TableLook = function() {return this.TableLook;}
CTableProp.prototype.put_TableLook = function(v){this.TableLook = v;}
CTableProp.prototype.get_TableStyle = function() {return this.TableStyle;}
CTableProp.prototype.put_TableStyle = function(v){this.TableStyle = v;}
CTableProp.prototype.get_AllowOverlap = function() {return this.AllowOverlap;}
CTableProp.prototype.put_AllowOverlap = function(v){this.AllowOverlap = v;}
CTableProp.prototype.get_TableLayout = function() {return this.TableLayout;}
CTableProp.prototype.put_TableLayout = function(v){this.TableLayout = v;}

function CBorders (obj)
{
	if (obj)
	{
		this.Left = (undefined != obj.Left && null != obj.Left) ? new CBorder (obj.Left) : null;
		this.Top = (undefined != obj.Top && null != obj.Top) ? new CBorder (obj.Top) : null;
		this.Right = (undefined != obj.Right && null != obj.Right) ? new CBorder (obj.Right) : null;
		this.Bottom = (undefined != obj.Bottom && null != obj.Bottom) ? new CBorder (obj.Bottom) : null;
		this.InsideH = (undefined != obj.InsideH && null != obj.InsideH) ? new CBorder (obj.InsideH) : null;
		this.InsideV = (undefined != obj.InsideV && null != obj.InsideV) ? new CBorder (obj.InsideV) : null;
	}
	//Все свойства класса CBorders должны быть undefined если они не изменялись
	/*else
	{
		this.Left = null;
		this.Top = null;
		this.Right = null;
		this.Bottom = null;
		this.InsideH = null;
		this.InsideV = null;
	}*/
}
CBorders.prototype.get_Left = function(){return this.Left; }
CBorders.prototype.put_Left = function(v){this.Left = (v) ? new CBorder (v) : null;}
CBorders.prototype.get_Top = function(){return this.Top; }
CBorders.prototype.put_Top = function(v){this.Top = (v) ? new CBorder (v) : null;}
CBorders.prototype.get_Right = function(){return this.Right; }
CBorders.prototype.put_Right = function(v){this.Right = (v) ? new CBorder (v) : null;}
CBorders.prototype.get_Bottom = function(){return this.Bottom; }
CBorders.prototype.put_Bottom = function(v){this.Bottom = (v) ? new CBorder (v) : null;}
CBorders.prototype.get_InsideH = function(){return this.InsideH; }
CBorders.prototype.put_InsideH = function(v){this.InsideH = (v) ? new CBorder (v) : null;}
CBorders.prototype.get_InsideV = function(){return this.InsideV; }
CBorders.prototype.put_InsideV = function(v){this.InsideV = (v) ? new CBorder (v) : null;}

function CBorder (obj)
{
	if (obj)
	{
		this.Color = (undefined != obj.Color && null != obj.Color) ? CreateAscColorCustom(obj.Color.r, obj.Color.g, obj.Color.b) : null;
		this.Size = (undefined != obj.Size) ? obj.Size : null;
		this.Value = (undefined != obj.Value) ? obj.Value : null;
		this.Space = (undefined != obj.Space) ? obj.Space : null;
	}
	else
	{
		this.Color = CreateAscColorCustom(0,0,0);
		this.Size  = 0.5 * g_dKoef_pt_to_mm;
		this.Value = border_Single;
		this.Space = 0;
	}
}
CBorder.prototype.get_Color = function(){return this.Color; }
CBorder.prototype.put_Color = function(v){this.Color = v;}
CBorder.prototype.get_Size = function(){return this.Size; }
CBorder.prototype.put_Size = function(v){this.Size = v;}
CBorder.prototype.get_Value = function(){return this.Value; }
CBorder.prototype.put_Value = function(v){this.Value = v;}
CBorder.prototype.get_Space = function(){return this.Space; }
CBorder.prototype.put_Space = function(v){this.Space = v;}
CBorder.prototype.get_ForSelectedCells = function(){return this.ForSelectedCells; }
CBorder.prototype.put_ForSelectedCells = function(v){this.ForSelectedCells = v;}

// CMargins
function CMargins (obj)
{
	if (obj)
	{
		this.Left = (undefined != obj.Left) ? obj.Left : null;
		this.Right = (undefined != obj.Right) ? obj.Right : null;
		this.Top = (undefined != obj.Top) ? obj.Top : null;
		this.Bottom = (undefined != obj.Bottom) ? obj.Bottom : null;
		this.Flag = (undefined != obj.Flag) ? obj.Flag : null;
	}
	else
	{
		this.Left = null;
		this.Right = null;
		this.Top = null;
		this.Bottom = null;
		this.Flag = null;
	}
}
CMargins.prototype.get_Left = function(){return this.Left; }
CMargins.prototype.put_Left = function(v){this.Left = v;}
CMargins.prototype.get_Right = function(){return this.Right; }
CMargins.prototype.put_Right = function(v){this.Right = v;}
CMargins.prototype.get_Top = function(){return this.Top; }
CMargins.prototype.put_Top = function(v){this.Top = v;}
CMargins.prototype.get_Bottom = function(){return this.Bottom; }
CMargins.prototype.put_Bottom = function(v){this.Bottom = v;}
CMargins.prototype.get_Flag = function(){return this.Flag; }
CMargins.prototype.put_Flag = function(v){this.Flag = v;}

/*
	{
	    TableWidth   : null - галочка убрана, либо заданное значение в мм
	    TableSpacing : null - галочка убрана, либо заданное значение в мм

	    TableDefaultMargins :  // маргины для всей таблицы(значение по умолчанию)
	    {
	        Left   : 1.9,
	        Right  : 1.9,
	        Top    : 0,
	        Bottom : 0
	    }

	    CellMargins :
        {
            Left   : 1.9, (null - неопределенное значение)
            Right  : 1.9, (null - неопределенное значение)
            Top    : 0,   (null - неопределенное значение)
            Bottom : 0,   (null - неопределенное значение)
            Flag   : 0 - У всех выделенных ячеек значение берется из TableDefaultMargins
                     1 - У выделенных ячеек есть ячейки с дефолтовыми значениями, и есть со своими собственными
                     2 - У всех ячеек свои собственные значения
        }

        TableAlignment : 0, 1, 2 (слева, по центру, справа)
        TableIndent : значение в мм,
        TableWrappingStyle : 0, 1 (inline, flow)
        TablePaddings:
        {
             Left   : 3.2,
             Right  : 3.2,
             Top    : 0,
             Bottom : 0
        }

        TableBorders : // границы таблицы
        {
            Bottom :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Left :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Right :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Top :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            InsideH :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            InsideV :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            }
        }

        CellBorders : // границы выделенных ячеек
        {
			ForSelectedCells : true,

            Bottom :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Left :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Right :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            Top :
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            InsideH : // данного элемента может не быть, если у выделенных ячеек
                      // нет горизонтальных внутренних границ
            {
                Color : { r : 0, g : 0, b : 0 },
                 Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            },

            InsideV : // данного элемента может не быть, если у выделенных ячеек
                      // нет вертикальных внутренних границ
            {
                Color : { r : 0, g : 0, b : 0 },
                Value : border_Single,
                Size  : 0.5 * g_dKoef_pt_to_mm
				Space :
            }
        }

        TableBackground :
        {
            Value : тип заливки(прозрачная или нет),
            Color : { r : 0, g : 0, b : 0 }
        }
        CellsBackground : null если заливка не определена для выделенных ячеек
        {
            Value : тип заливки(прозрачная или нет),
            Color : { r : 0, g : 0, b : 0 }
        }

		Position:
		{
			X:0,
			Y:0
		}
	}
*/
asc_docs_api.prototype.tblApply = function(obj)
{
}
/*callbacks*/
asc_docs_api.prototype.sync_AddTableCallback = function(){
}
asc_docs_api.prototype.sync_AlignCellCallback = function(align){
}
asc_docs_api.prototype.sync_TblPropCallback = function(tblProp)
{
}
asc_docs_api.prototype.sync_TblWrapStyleChangedCallback = function(style){
}
asc_docs_api.prototype.sync_TblAlignChangedCallback = function(style){
}

/*----------------------------------------------------------------*/
/*functions for working with images*/
asc_docs_api.prototype.ChangeImageFromFile = function()
{
}
asc_docs_api.prototype.ChangeShapeImageFromFile = function()
{
}

asc_docs_api.prototype.AddImage = function(){	
}

asc_docs_api.prototype.AddImageUrl2 = function(url)
{
}

asc_docs_api.prototype.AddImageUrl = function(url, imgProp)
{
}
asc_docs_api.prototype.AddImageUrlAction = function(url, imgProp)
{    
}
/* В качестве параметра  передается объект класса CImgProperty, он же приходит на OnImgProp
CImgProperty заменяет пережнюю структуру:
если параметр не имеет значения то передвать следует null, напримере inline-картинок: в качестве left,top,bottom,right,X,Y,ImageUrl необходимо передавать null.
	{
		Width: 0,
		Height: 0,
		WrappingStyle: 0,
		Paddings: { Left : 0, Top : 0, Bottom: 0, Right: 0 },
		Position : {X : 0, Y : 0},
		ImageUrl : ""
	}
*/
asc_docs_api.prototype.ImgApply = function(obj)
{    
}
asc_docs_api.prototype.set_Size = function(width, height){

}
asc_docs_api.prototype.set_ConstProportions = function(isOn){
}
asc_docs_api.prototype.set_WrapStyle = function(type){

}
asc_docs_api.prototype.deleteImage = function(){

}
asc_docs_api.prototype.set_ImgDistanceFromText = function(left,top,right,bottom){

}
asc_docs_api.prototype.set_PositionOnPage = function(X,Y){//расположение от начала страницы

}
asc_docs_api.prototype.get_OriginalSizeImage = function(){
}

asc_docs_api.prototype.ShapeApply = function(shapeProps)
{    
}
/*callbacks*/
asc_docs_api.prototype.sync_AddImageCallback = function(){
}
asc_docs_api.prototype.sync_ImgPropCallback = function(imgProp)
{
}
asc_docs_api.prototype.sync_ImgWrapStyleChangedCallback = function(style){
}

//-----------------------------------------------------------------
// События контекстного меню
//-----------------------------------------------------------------

function CContextMenuData( obj )
{
    if( obj )
    {
        this.Type  = ( undefined != obj.Type ) ? obj.Type : c_oAscContextMenuTypes.Common;
        this.X_abs = ( undefined != obj.X_abs ) ? obj.X_abs : 0;
        this.Y_abs = ( undefined != obj.Y_abs ) ? obj.Y_abs : 0;

        switch ( this.Type )
        {
            case c_oAscContextMenuTypes.ChangeHdrFtr :
            {
                this.PageNum = ( undefined != obj.PageNum ) ? obj.PageNum : 0;
                this.Header  = ( undefined != obj.Header  ) ? obj.Header  : true;

                break;
            }
        }
    }
    else
    {
        this.Type  = c_oAscContextMenuTypes.Common;
        this.X_abs = 0;
        this.Y_abs = 0;
    }
}

CContextMenuData.prototype.get_Type  = function()  { return this.Type; }
CContextMenuData.prototype.get_X = function()  { return this.X_abs; }
CContextMenuData.prototype.get_Y = function()  { return this.Y_abs; }
CContextMenuData.prototype.get_PageNum = function()  { return this.PageNum; }
CContextMenuData.prototype.is_Header = function()  { return this.Header; }

asc_docs_api.prototype.sync_ContextMenuCallback = function(Data)
{
}

//-----------------------------------------------------------------
// События движения мыши
//-----------------------------------------------------------------
function CMouseMoveData( obj )
{
    if( obj )
    {
        this.Type  = ( undefined != obj.Type ) ? obj.Type : c_oAscMouseMoveDataTypes.Common;
        this.X_abs = ( undefined != obj.X_abs ) ? obj.X_abs : 0;
        this.Y_abs = ( undefined != obj.Y_abs ) ? obj.Y_abs : 0;

        switch ( this.Type )
        {
            case c_oAscMouseMoveDataTypes.Hyperlink :
            {
                this.Hyperlink = ( undefined != obj.PageNum ) ? obj.PageNum : 0;
                break;
            }

            case c_oAscMouseMoveDataTypes.LockedObject :
            {
                this.UserId           = ( undefined != obj.UserId ) ? obj.UserId : "";
                this.HaveChanges      = ( undefined != obj.HaveChanges ) ? obj.HaveChanges : false;
                this.LockedObjectType = ( undefined != obj.LockedObjectType ) ? obj.LockedObjectType : c_oAscMouseMoveLockedObjectType.Common;
                break;
            }
        }
    }
    else
    {
        this.Type  = c_oAscMouseMoveDataTypes.Common;
        this.X_abs = 0;
        this.Y_abs = 0;
    }
}

CMouseMoveData.prototype.get_Type  = function()  { return this.Type; }
CMouseMoveData.prototype.get_X = function()  { return this.X_abs; }
CMouseMoveData.prototype.get_Y = function()  { return this.Y_abs; }
CMouseMoveData.prototype.get_Hyperlink = function()  { return this.Hyperlink; }
CMouseMoveData.prototype.get_UserId = function() { return this.UserId; };
CMouseMoveData.prototype.get_HaveChanges = function() { return this.HaveChanges; };
CMouseMoveData.prototype.get_LockedObjectType = function() { return this.LockedObjectType; };

asc_docs_api.prototype.sync_MouseMoveStartCallback = function()
{
}

asc_docs_api.prototype.sync_MouseMoveEndCallback = function()
{
}

asc_docs_api.prototype.sync_MouseMoveCallback = function(Data)
{
}

//-----------------------------------------------------------------
// Функции для работы с гиперссылками
//-----------------------------------------------------------------
asc_docs_api.prototype.can_AddHyperlink = function()
{
}

// HyperProps - объект CHyperlinkProperty
asc_docs_api.prototype.add_Hyperlink = function(HyperProps)
{
}

// HyperProps - объект CHyperlinkProperty
asc_docs_api.prototype.change_Hyperlink = function(HyperProps)
{
}

asc_docs_api.prototype.remove_Hyperlink = function()
{
}

function CHyperlinkProperty( obj )
{
    if( obj )
    {
        this.Text    = (undefined != obj.Text   ) ? obj.Text    : null;
        this.Value   = (undefined != obj.Value  ) ? obj.Value   : "";
        this.ToolTip = (undefined != obj.ToolTip) ? obj.ToolTip : "";
    }
    else
    {
        this.Text    = null;
        this.Value   = "";
        this.ToolTip = "";
    }
}

CHyperlinkProperty.prototype.get_Value   = function()  { return this.Value; }
CHyperlinkProperty.prototype.put_Value   = function(v) { this.Value = v; }
CHyperlinkProperty.prototype.get_ToolTip = function()  { return this.ToolTip; }
CHyperlinkProperty.prototype.put_ToolTip = function(v) { this.ToolTip = v; }
CHyperlinkProperty.prototype.get_Text    = function()  { return this.Text; }
CHyperlinkProperty.prototype.put_Text    = function(v) { this.Text = v; }

asc_docs_api.prototype.sync_HyperlinkPropCallback = function(hyperProp)
{
}

asc_docs_api.prototype.sync_HyperlinkClickCallback = function(Url)
{
}

asc_docs_api.prototype.sync_CanAddHyperlinkCallback = function(bCanAdd)
{
}

asc_docs_api.prototype.sync_DialogAddHyperlink = function()
{
}

asc_docs_api.prototype.sync_DialogAddHyperlink = function()
{
}

//-----------------------------------------------------------------
// Функции для работы с орфографией
//-----------------------------------------------------------------
function asc_CSpellCheckProperty( Word, Checked, Variants, ParaId, ElemId )
{
    this.Word     = Word;
    this.Checked  = Checked;
    this.Variants = Variants;

    this.ParaId   = ParaId;
    this.ElemId   = ElemId;
}

asc_CSpellCheckProperty.prototype.get_Word     = function()  { return this.Word; }
asc_CSpellCheckProperty.prototype.get_Checked  = function()  { return this.Checked; }
asc_CSpellCheckProperty.prototype.get_Variants = function()  { return this.Variants; }

asc_docs_api.prototype.sync_SpellCheckCallback = function(Word, Checked, Variants, ParaId, ElemId)
{
}

asc_docs_api.prototype.sync_SpellCheckVariantsFound = function()
{
}

asc_docs_api.prototype.asc_replaceMisspelledWord = function(Word, SpellCheckProperty)
{
}

asc_docs_api.prototype.asc_ignoreMisspelledWord = function(SpellCheckProperty, bAll)
{
}

asc_docs_api.prototype.asc_setDefaultLanguage = function(Lang)
{
}

asc_docs_api.prototype.asc_getDefaultLanguage = function()
{
}
//-----------------------------------------------------------------
// Функции для работы с комментариями
//-----------------------------------------------------------------
function asc_CCommentData( obj )
{
    if( obj )
    {
        this.m_sText      = (undefined != obj.m_sText     ) ? obj.m_sText      : "";
        this.m_sTime      = (undefined != obj.m_sTime     ) ? obj.m_sTime      : "";
        this.m_sUserId    = (undefined != obj.m_sUserId   ) ? obj.m_sUserId    : "";
        this.m_sQuoteText = (undefined != obj.m_sQuoteText) ? obj.m_sQuoteText : null;
        this.m_bSolved    = (undefined != obj.m_bSolved   ) ? obj.m_bSolved    : false;
        this.m_sUserName  = (undefined != obj.m_sUserName ) ? obj.m_sUserName  : "";
        this.m_aReplies   = new Array();
        if ( undefined != obj.m_aReplies )
        {
            var Count = obj.m_aReplies.length;
            for ( var Index = 0; Index < Count; Index++ )
            {
                var Reply = new asc_CCommentData( obj.m_aReplies[Index] );
                this.m_aReplies.push( Reply );
            }
        }
    }
    else
    {
        this.m_sText      = "";
        this.m_sTime      = "";
        this.m_sUserId    = "";
        this.m_sQuoteText = null;
        this.m_bSolved    = false;
        this.m_sUserName  = "";
        this.m_aReplies   = new Array();
    }
}

asc_CCommentData.prototype.asc_getText         = function()  { return this.m_sText; }
asc_CCommentData.prototype.asc_putText         = function(v) { this.m_sText = v; }
asc_CCommentData.prototype.asc_getTime         = function()  { return this.m_sTime; }
asc_CCommentData.prototype.asc_putTime         = function(v) { this.m_sTime = v; }
asc_CCommentData.prototype.asc_getUserId       = function()  { return this.m_sUserId; }
asc_CCommentData.prototype.asc_putUserId       = function(v) { this.m_sUserId = v; }
asc_CCommentData.prototype.asc_getUserName     = function()  { return this.m_sUserName; }
asc_CCommentData.prototype.asc_putUserName     = function(v) { this.m_sUserName = v; }
asc_CCommentData.prototype.asc_getQuoteText    = function()  { return this.m_sQuoteText; };
asc_CCommentData.prototype.asc_putQuoteText    = function(v) { this.m_sQuoteText = v; };
asc_CCommentData.prototype.asc_getSolved       = function()  { return this.m_bSolved; };
asc_CCommentData.prototype.asc_putSolved       = function(v) { this.m_bSolved = v; };
asc_CCommentData.prototype.asc_getReply        = function(i) { return this.m_aReplies[i]; }
asc_CCommentData.prototype.asc_addReply        = function(v) { this.m_aReplies.push( v ); }
asc_CCommentData.prototype.asc_getRepliesCount = function(v) { return this.m_aReplies.length; }


asc_docs_api.prototype.asc_showComments = function()
{
}

asc_docs_api.prototype.asc_hideComments = function()
{
}

asc_docs_api.prototype.asc_addComment = function(AscCommentData)
{
}

asc_docs_api.prototype.asc_removeComment = function(Id)
{
}

asc_docs_api.prototype.asc_changeComment = function(Id, AscCommentData)
{
}

asc_docs_api.prototype.asc_selectComment = function(Id)
{
}

asc_docs_api.prototype.asc_showComment = function(Id)
{
}

asc_docs_api.prototype.can_AddQuotedComment = function()
{
}

asc_docs_api.prototype.sync_RemoveComment = function(Id)
{
}

asc_docs_api.prototype.sync_AddComment = function(Id, CommentData)
{
}

asc_docs_api.prototype.sync_ShowComment = function(Id, X, Y)
{
}

asc_docs_api.prototype.sync_HideComment = function()
{
}

asc_docs_api.prototype.sync_UpdateCommentPosition = function(Id, X, Y)
{
}

asc_docs_api.prototype.sync_ChangeCommentData = function(Id, CommentData)
{
}

asc_docs_api.prototype.sync_LockComment = function(Id, UserId)
{
}

asc_docs_api.prototype.sync_UnLockComment = function(Id)
{
}

asc_docs_api.prototype.asc_getComments = function()
{
}
//-----------------------------------------------------------------
asc_docs_api.prototype.sync_LockHeaderFooters = function()
{
}

asc_docs_api.prototype.sync_LockDocumentProps = function()
{
}

asc_docs_api.prototype.sync_UnLockHeaderFooters = function()
{
}

asc_docs_api.prototype.sync_UnLockDocumentProps = function()
{
}

asc_docs_api.prototype.sync_CollaborativeChanges = function()
{
}

asc_docs_api.prototype.sync_LockDocumentSchema = function()
{
}

asc_docs_api.prototype.sync_UnLockDocumentSchema = function()
{
}


/*----------------------------------------------------------------*/
/*functions for working with zoom & navigation*/
asc_docs_api.prototype.zoomIn = function(){
}
asc_docs_api.prototype.zoomOut = function(){
}
asc_docs_api.prototype.zoomFitToPage = function(){
}
asc_docs_api.prototype.zoomFitToWidth = function(){
}
asc_docs_api.prototype.zoomCustomMode = function(){
}
asc_docs_api.prototype.zoom100 = function(){
}
asc_docs_api.prototype.zoom = function(percent){
}
asc_docs_api.prototype.goToPage = function(number){
}
asc_docs_api.prototype.getCountPages = function(){
}
asc_docs_api.prototype.getCurrentPage = function(){
}
/*callbacks*/
asc_docs_api.prototype.sync_zoomChangeCallback = function(percent,type){	//c_oAscZoomType.Current, c_oAscZoomType.FitWidth, c_oAscZoomType.FitPage
}
asc_docs_api.prototype.sync_countPagesCallback = function(count){
}
asc_docs_api.prototype.sync_currentPageCallback = function(number){
}

asc_docs_api.prototype.async_SaveToPdf = function(){
}

asc_docs_api.prototype.async_SaveToPdf_PartCallback = function(part64, is_end)
{
}
asc_docs_api.prototype.async_SaveToPdf_Progress = function(progress)
{
}

/*----------------------------------------------------------------*/
asc_docs_api.prototype.asc_enableKeyEvents = function(value){
}

asc_docs_api.prototype.asyncServerIdStartLoaded = function()
{
}

asc_docs_api.prototype.asyncServerIdEndLoaded = function()
{
}

// работа с шрифтами
asc_docs_api.prototype.asyncFontsDocumentStartLoaded = function()
{
}
asc_docs_api.prototype.GenerateStyles = function()
{
}
asc_docs_api.prototype.asyncFontsDocumentEndLoaded = function()
{    
}

asc_docs_api.prototype.CreateFontsCharMap = function()
{
}

asc_docs_api.prototype.sync_SendThemeColors = function(colors,standart_colors)
{
}
asc_docs_api.prototype.sync_SendThemeColorSchemes = function(param)
{
}

asc_docs_api.prototype.ChangeColorScheme = function(index_scheme)
{
}

asc_docs_api.prototype.asyncImagesDocumentStartLoaded = function()
{
	// евент о заморозке не нужен... оно и так заморожено
	// просто нужно вывести информацию в статус бар (что началась загрузка картинок)
}
asc_docs_api.prototype.asyncImagesDocumentEndLoaded = function()
{    
}

asc_docs_api.prototype.OpenDocumentEndCallback = function()
{
    if (null == this.WordControl.m_oLogicDocument)
        return;

    var Document = this.WordControl.m_oLogicDocument;

    if (this.isApplyChangesOnOpenEnabled)
    {
        this.isApplyChangesOnOpenEnabled = false;
        if (CollaborativeEditing.m_bUse == true)
        {
            this.isApplyChangesOnOpen = true;
            CollaborativeEditing.Apply_Changes();
            CollaborativeEditing.Release_Locks();
            return;
        }
    }

    //Recalculate HdrFtr
    if(Document.HdrFtr && Document.HdrFtr.Content && Document.HdrFtr.Content.length > 0 && Document.HdrFtr.Content[0].Header)
    {
        var Header = Document.HdrFtr.Content[0].Header;
        if(null != Header.First)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Header.First } );
        if(null != Header.Even)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Header.Even } );
        if(null != Header.Odd)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Header.Odd } );
    }
    if(Document.HdrFtr && Document.HdrFtr.Content && Document.HdrFtr.Content.length > 0 && Document.HdrFtr.Content[0].Footer)
    {
        var Footer = Document.HdrFtr.Content[0].Footer;
        if(null != Footer.First)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Footer.First } );
        if(null != Footer.Even)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Footer.Even } );
        if(null != Footer.Odd)
            History.RecalcData_Add( { Type : historyrecalctype_HdrFtr, Data : Footer.Odd } );
    }

    History.RecalcData_Add( { Type : historyrecalctype_Inline, Data : { Pos : 0, PageNum : 0 } } );

    //Recalculate для Document
    Document.CurPos.ContentPos = 0;

    var hdr = Document.HdrFtr.Content[0].Header;
    var ftr = Document.HdrFtr.Content[0].Footer;
    var drawing_objects = Document.DrawingObjects;
    if(hdr.First != null || ftr.First != null)
    {
        drawing_objects.firstPage = new HeaderFooterGraphicObjects();
    }
    if(hdr.Even != null || ftr.Even != null)
    {
        if((hdr.Even != null && hdr.Even == hdr.First) || (ftr.Even != null && ftr.Even == ftr.First))
            drawing_objects.evenPage = drawing_objects.firstPage;
        else
            drawing_objects.evenPage = new HeaderFooterGraphicObjects();
    }
    if(hdr.Odd != null || ftr.Odd != null)
    {
        if((hdr.Odd != null && hdr.Odd == hdr.First) || (ftr.Odd != null && ftr.Odd == ftr.First))
            drawing_objects.oddPage = drawing_objects.firstPage;
        else if((hdr.Odd != null && hdr.Odd == hdr.Even)|| (ftr.Odd != null && ftr.Odd == ftr.Even))
            drawing_objects.oddPage = drawing_objects.evenPage;
        else
            drawing_objects.oddPage = new HeaderFooterGraphicObjects();
    }


    Document.DrawingObjects.calculateAfterOpen();
    Document.DrawingObjects.calculateAfterChangeTheme();

    Document.Recalculate();    
}

asc_docs_api.prototype.UpdateInterfaceState = function()
{
}

asc_docs_api.prototype.asyncFontStartLoaded = function()
{
}
asc_docs_api.prototype.asyncFontEndLoaded = function(fontinfo)
{
}

asc_docs_api.prototype.asyncImageStartLoaded = function()
{
    // здесь прокинуть евент о заморозке меню
}
asc_docs_api.prototype.asyncImageEndLoaded = function(_image)
{    
}

asc_docs_api.prototype.asyncImageEndLoadedBackground = function(_image)
{
}
asc_docs_api.prototype.IsAsyncOpenDocumentImages = function()
{
}

asc_docs_api.prototype.SendOpenProgress = function()
{
}

asc_docs_api.prototype.sync_SendProgress = function(Percents)
{
}

asc_docs_api.prototype.pre_Paste = function(_fonts, _images, callback)
{    
}

asc_docs_api.prototype.pre_Save = function(_images)
{
}

asc_docs_api.prototype.SyncLoadImages = function(_images)
{    
}
asc_docs_api.prototype.SyncLoadImages_callback = function()
{
}

asc_docs_api.prototype.pre_SaveCallback = function()
{
}

asc_docs_api.prototype.initEvents2MobileAdvances = function()
{
    //this.WordControl.initEvents2MobileAdvances();
}
asc_docs_api.prototype.ViewScrollToX = function(x)
{
}
asc_docs_api.prototype.ViewScrollToY = function(y)
{
}
asc_docs_api.prototype.GetDocWidthPx = function()
{
}
asc_docs_api.prototype.GetDocHeightPx = function()
{
}
asc_docs_api.prototype.ClearSearch = function()
{
}
asc_docs_api.prototype.GetCurrentVisiblePage = function()
{
};

// Выставление интервала автосохранения (0 - означает, что автосохранения нет)
asc_docs_api.prototype.asc_setAutoSaveGap = function (autoSaveGap) {
};

asc_docs_api.prototype.SetMobileVersion = function(val)
{
};

asc_docs_api.prototype.GoToHeader = function(pageNumber)
{
}

asc_docs_api.prototype.GoToFooter = function(pageNumber)
{
}

asc_docs_api.prototype.ExitHeader_Footer = function(pageNumber)
{
}

asc_docs_api.prototype.GetCurrentPixOffsetY = function()
{
}

asc_docs_api.prototype.SetPaintFormat = function(value)
{
}

asc_docs_api.prototype.ChangeShapeType = function(value)
{
}

asc_docs_api.prototype.sync_PaintFormatCallback = function(value)
{
}
asc_docs_api.prototype.SetMarkerFormat = function(value, is_flag, r, g, b)
{
}

asc_docs_api.prototype.sync_MarkerFormatCallback = function(value)
{
}

asc_docs_api.prototype.StartAddShape = function(sPreset, is_apply)
{
}

asc_docs_api.prototype.sync_StartAddShapeCallback = function(value)
{
}

asc_docs_api.prototype.CanGroup = function()
{
}

asc_docs_api.prototype.CanUnGroup = function()
{
}

asc_docs_api.prototype.CanChangeWrapPolygon = function()
{
}

asc_docs_api.prototype.StartChangeWrapPolygon = function()
{
}


asc_docs_api.prototype.ClearFormating = function()
{
}

asc_docs_api.prototype.GetSectionInfo = function()
{
}

asc_docs_api.prototype.SetViewMode = function( isViewMode )
{
}

asc_docs_api.prototype.SetUseEmbeddedCutFonts = function(bUse)
{
}

asc_docs_api.prototype.IsNeedDefaultFonts = function()
{
}

asc_docs_api.prototype.OnMouseUp = function(x, y)
{
}
asc_docs_api.prototype.OnHandleMessage = function(event)
{
}
asc_docs_api.prototype.asyncImageEndLoaded2 = null;

asc_docs_api.prototype.OfflineAppDocumentStartLoad = function()
{
}

asc_docs_api.prototype.OfflineAppDocumentEndLoad = function()
{
}

asc_docs_api.prototype.SetDrawImagePlaceParagraph = function(element_id, props)
{
}

asc_docs_api.prototype.asc_setCoAuthoringEnable = function (isCoAuthoringEnable)
{
}

var cCharDelimiter = String.fromCharCode(5);

function getURLParameter(name) {
    return (RegExp(name + '=' + '(.+?)(&|$)').exec(location.search)||[,null])[1];
}

function spellCheck (editor, rdata) {
}

function _onSpellCheck_Callback2 (response)
{
}

function _addImageUrl2 (url)
{
}
function _isDocumentModified2 ()
{
}
function _asc_Save2 ()
{
}
function _asc_SavingEnd ()
{
}
function  _asc_scrollTo (x,y)
{
}
function _getFullImageSrc(src)
{
	var start = src.substring(0, 6);
    if(0 != src.indexOf("http:") && 0 != src.indexOf("data:") && 0 != src.indexOf("https:") && 0 != src.indexOf("ftp:") && 0 != src.indexOf("file:"))
    {
        if (0 == src.indexOf(editor.DocumentUrl))
            return src;
        return editor.DocumentUrl + "media/" + src;
    }
	else
		return src;
};
function _mapAscServerErrorToAscError(nServerError)
{
	var nRes = c_oAscError.ID.Unknown;
	switch(nServerError)
	{
		case c_oAscServerError.NoError : nRes = c_oAscError.ID.No;break;
		case c_oAscServerError.TaskQueue :
		case c_oAscServerError.TaskResult : nRes = c_oAscError.ID.Database;break;
		case c_oAscServerError.ConvertDownload : nRes = c_oAscError.ID.DownloadError;break;
		case c_oAscServerError.ConvertTimeout : nRes = c_oAscError.ID.ConvertationTimeout;break;
		case c_oAscServerError.ConvertMS_OFFCRYPTO : nRes = c_oAscError.ID.ConvertationPassword;break;
		case c_oAscServerError.ConvertUnknownFormat :
		case c_oAscServerError.ConvertReadFile :
		case c_oAscServerError.Convert : nRes = c_oAscError.ID.ConvertationError;break;
		case c_oAscServerError.UploadContentLength : nRes = c_oAscError.ID.UplImageSize;break;
		case c_oAscServerError.UploadExtension : nRes = c_oAscError.ID.UplImageExt;break;
		case c_oAscServerError.UploadCountFiles : nRes = c_oAscError.ID.UplImageFileCount;break;
		case c_oAscServerError.VKey : nRes = c_oAscError.ID.FileVKey;break;
		case c_oAscServerError.VKeyEncrypt : nRes = c_oAscError.ID.VKeyEncrypt;break;
		case c_oAscServerError.VKeyKeyExpire : nRes = c_oAscError.ID.KeyExpire;break;
		case c_oAscServerError.VKeyUserCountExceed : nRes = c_oAscError.ID.UserCountExceed;break;
		case c_oAscServerError.Storage :
		case c_oAscServerError.StorageFileNoFound :
		case c_oAscServerError.StorageRead :
		case c_oAscServerError.StorageWrite :
		case c_oAscServerError.StorageRemoveDir :
		case c_oAscServerError.StorageCreateDir :
		case c_oAscServerError.StorageGetInfo :
		case c_oAscServerError.Upload :
		case c_oAscServerError.ReadRequestStream :
		case c_oAscServerError.Unknown : nRes = c_oAscError.ID.Unknown;break;
	}
	return nRes;
}

function CErrorData()
{
    this.Value = 0;
}

CErrorData.prototype.put_Value = function(v){ this.Value = v; };
CErrorData.prototype.get_Value = function() { return this.Value; };
//test

// Вставка диаграмм
asc_docs_api.prototype.asc_getChartObject = function()
{	
	this.isChartEditor = true;		// Для совместного редактирования

	var oColor, oNewColor;
	var graphicObject = this.WordControl.m_oLogicDocument.Get_ChartObject();
	for (var i = 0; i < this.WordControl.m_oDrawingDocument.GuiControlColorsMap.length; i++) {
		oColor = this.WordControl.m_oDrawingDocument.GuiControlColorsMap[i];
		oNewColor = new CRGBColor();
		oNewColor.RGBA.R = oColor.r;
		oNewColor.RGBA.G = oColor.g;
		oNewColor.RGBA.B = oColor.b;
		graphicObject.chart.themeColors.push(oNewColor);
	}
    return graphicObject;
}

asc_docs_api.prototype.asc_addChartDrawingObject = function(chartBinary)
{    
}

asc_docs_api.prototype.asc_editChartDrawingObject = function(chartBinary)
{
}

asc_docs_api.prototype.asc_getChartPreviews = function(chartType, chartSubType)
{
}

asc_docs_api.prototype.sync_closeChartEditor = function()
{
}

asc_docs_api.prototype.asc_setDrawCollaborationMarks = function (bDraw)
{
}

asc_docs_api.prototype.asc_AddMath = function(Type)
{
}

