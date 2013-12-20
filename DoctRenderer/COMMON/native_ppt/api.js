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
var documentFormatSave = c_oAscFileType.PPTX;//пока не во что другое сохранять не можем.

var c_oSerFormat =
{
    Version:1,
    Signature: "PPTY"
};


function CErrorData()
{
    this.Value = 0;
}

CErrorData.prototype.put_Value = function(v){ this.Value = v; };
CErrorData.prototype.get_Value = function() { return this.Value; };

function asc_docs_api(name)
{
	/************ private!!! **************/
    this.HtmlElementName = name;
    
    this.ThemeLoader = new CThemeLoader();
    this.ThemeLoader.Api = this;

    this.DocumentUrl = "";
    this.DocumentName = "";
	this.DocInfo = null;
    this.bNoSendComments = false;

    this.isApplyChangesOnOpen = false;
    this.isApplyChangesOnOpenEnabled = true;

    this.IsSupportEmptyPresentation = true;
        
    this.ShowParaMarks = false;
	this.isAddSpaceBetweenPrg = false;
    this.isPageBreakBefore = false;
    this.isKeepLinesTogether = false;

    this.isMobileVersion = false;
    this.isPaintFormat = false;
    this.isViewMode = false;
    this.isShowTableEmptyLine = true;

    this.bInit_word_control = false;
	this.isDocumentModify = false;

    this.isImageChangeUrl = false;
    this.isShapeImageChangeUrl = false;
    this.isSlideImageChangeUrl = false;
	
    this.isPasteFonts_Images = false;

    this.isLoadNoCutFonts = false;

    // На этапе сборки значение переменной ASC_DOCS_API_USE_EMBEDDED_FONTS может менятся.
	// По дефолту встроенные шрифты использоваться не будут, как и при любом значении
	// ASC_DOCS_API_USE_EMBEDDED_FONTS, кроме "true"(написание от регистра не зависит).
    this.isUseEmbeddedCutFonts = ("true" == ASC_DOCS_API_USE_EMBEDDED_FONTS.toLowerCase());

    this.noCreatePoint = false;
    this.pasteCallback = null;
    this.pasteImageMap = null;
    this.EndActionLoadImages = 0;

    this.isSaveFonts_Images = false;
    this.saveImageMap = null;
    this.canSave = true;//Флаг нужен чтобы не происходило сохранение пока не завершится предыдущее сохранение

    this.ServerIdWaitComplete = false;
    this.ServerImagesWaitComplete = false;

    this.ParcedDocument = false;
	this.isStartCoAuthoringOnEndLoad = false;	// Подсоединились раньше, чем документ загрузился

    this.DocumentOrientation = orientation_Portrait ? true : false;

    this.SelectedObjectsStack = new Array();

    this.OpenDocumentProgress = new CDocOpenProgress();
    this._lastConvertProgress = 0;

    // CoAuthoring and Chat
    this.User = undefined;
    this.CoAuthoringApi = null;
    this.isCoAuthoringEnable = true;
    /**************************************/
	// AutoSave
	this.autoSaveGap = 0;				// Интервал автосохранения (0 - означает, что автосохранения нет) в милесекундах
	this.autoSaveTimeOutId = null;		// Идентификатор таймаута
	this.isAutoSave = false;			// Флаг, означает что запущено автосохранение
	this.autoSaveGapAsk = 5000;			// Константа для повторного запуска автосохранения, если не смогли сделать сразу lock (только при автосохранении) в милесекундах

	this.canSave = true;				//Флаг нужен чтобы не происходило сохранение пока не завершится предыдущее сохранение

	if(typeof ChartStyleManager !== "undefined")
		this.chartStyleManager = new ChartStyleManager();
	else
		this.chartStyleManager = null;
	if(typeof ChartPreviewManager !== "undefined")	
		this.chartPreviewManager = new ChartPreviewManager();
	else
		this.chartPreviewManager = null;
	if(typeof asc_CChartTranslate !== "undefined")	
		this.chartTranslate = new asc_CChartTranslate();
	else
		this.chartTranslate = null;

    this.tableStylesIdCounter = 0;
    //выставляем тип copypaste
    g_bIsDocumentCopyPaste = false;	
	var oThis = this;	

    if (window.editor == undefined)
    {
        window["editor"] = this;
		editor = window.editor;
    }

    this.WordControl = new CEditorPage(this);
    this.WordControl.Name = this.HtmlElementName;

    this.WordControl.m_oLogicDocument   = new CPresentation(this.WordControl.m_oDrawingDocument);
    this.WordControl.m_oDrawingDocument.m_oLogicDocument = this.WordControl.m_oLogicDocument;
}

/////////////////////////////////////////////////////////////////////////
///////////////////CoAuthoring and Chat api//////////////////////////////
/////////////////////////////////////////////////////////////////////////

function CChatMessage(user, message)
{
    this.UserId = (undefined != user_id) ? user_id : null;
    this.Message = (undefined != message) ? message : null;
}
CChatMessage.prototype.get_UserId = function() { return this.UserId; }
CChatMessage.prototype.get_Message = function() { return this.Message; }

/*
 ToDo Register Callback OnCoAuthoringChatReceiveMessage return object CChatMessage (возможно возвращается МАСС�?В CChatMessage со всеми сообщениями)
 ToDo Register Callback OnCoAuthoringConnectUser возвращается userId
 ToDo Register Callback OnCoAuthoringDisconnectUser возвращается userId
 */
// Init CoAuthoring
asc_docs_api.prototype._coAuthoringInit = function () 
{
}


asc_docs_api.prototype.pre_Save = function(_images)
{
}

asc_docs_api.prototype.sync_CollaborativeChanges = function()
{
}

// Set CoAuthoring server url
asc_docs_api.prototype._coAuthoringSetServerUrl = function (url) {
};
// server disconnect
asc_docs_api.prototype.asc_coAuthoringDisconnect = function () {
};
// send chart message
asc_docs_api.prototype.asc_coAuthoringChatSendMessage = function (message) {
};
// get chart messages, возвращается массив CChatMessage
asc_docs_api.prototype.asc_coAuthoringChatGetMessages = function () {
};
// get users, возвращается массив users
asc_docs_api.prototype.asc_coAuthoringGetUsers = function () {
};
/////////////////////////////////////////////////////////////////////////
////////////////////////////AutoSave api/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
asc_docs_api.prototype.autoSaveInit = function (autoSaveGap) {
};

asc_docs_api.prototype.asyncServerIdStartLoaded = function () {
};

asc_docs_api.prototype.asyncServerIdEndLoaded = function () {
};

// Эвент о пришедщих изменениях
asc_docs_api.prototype.syncCollaborativeChanges = function () {
};


asc_docs_api.prototype.SetCollaborativeMarksShowType = function(Type)
{
}

asc_docs_api.prototype.GetCollaborativeMarksShowType = function(Type)
{
}

asc_docs_api.prototype.Clear_CollaborativeMarks = function()
{
}

///////////////////////////////////////////
asc_docs_api.prototype.SetUnchangedDocument = function()
{
}

asc_docs_api.prototype.isDocumentModified = function()
{
}

asc_docs_api.prototype.sync_BeginCatchSelectedElements = function()
{
    if (0 != this.SelectedObjectsStack.length)
        this.SelectedObjectsStack.splice(0, this.SelectedObjectsStack.length);
}
asc_docs_api.prototype.sync_EndCatchSelectedElements = function(options)
{
    if ( this.chartStyleManager && this.chartPreviewManager && ( !this.chartStyleManager.isReady() || !this.chartPreviewManager.isReady() ))
    {
        for ( var i = 0; i < this.SelectedObjectsStack.length; i++ )
        {
            if ( this.SelectedObjectsStack[i].Value.ChartProperties )
            {
                this.chartStyleManager.init(options);
                this.chartPreviewManager.init(options);
                this.asc_fireCallback("asc_onUpdateChartStyles");
                break;
            }
        }
    }
    this.asc_fireCallback("asc_onFocusObject", this.SelectedObjectsStack);
}
asc_docs_api.prototype.getSelectedElements = function()
{
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
		case c_oAscTypeSelectElement.Shape: oUnkTypeObj = obj;
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
asc_docs_api.prototype.SetThemesPath = function(path)
{
}

asc_docs_api.prototype.CreateCSS = function()
{    
}

asc_docs_api.prototype.CreateComponents = function()
{   
}

asc_docs_api.prototype.InitEditor = function()
{
}

asc_docs_api.prototype.SetInterfaceDrawImagePlaceShape = function(div_id)
{
}
asc_docs_api.prototype.SetInterfaceDrawImagePlaceSlide = function(div_id)
{
}
asc_docs_api.prototype.SetInterfaceDrawImagePlace = function()
{}

asc_docs_api.prototype.OpenDocument2 = function(url, gObject)
{
	this.InitEditor();
	this.DocumentUrl = url;
	this.DocumentType = 2;
	var _loader = new BinaryPPTYLoader();
    _loader.Api = this;
    _loader.Load(gObject, this.WordControl.m_oLogicDocument);
	this.LoadedObject = 1;
}

asc_docs_api.prototype.OpenDocument = function(url, gObject)
{
	this.InitEditor();
	this.DocumentUrl = url;
	this.DocumentType = 2;
	var _loader = new BinaryPPTYLoader();
    _loader.Api = this;
    _loader.Load2(gObject, this.WordControl.m_oLogicDocument);
	this.LoadedObject = 1;
}

asc_docs_api.prototype.get_DocumentName = function()
{
	return this.DocumentName;
}

asc_docs_api.prototype.OfflineAppDocumentStartLoad = function()
{
}

asc_docs_api.prototype.OfflineAppDocumentEndLoad = function()
{    
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
		_callbacks[name] = []
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

asc_docs_api.prototype.asc_fireCallback = function(name)
{
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
asc_docs_api.prototype.get_TextProps = function()
{
	var Doc = this.WordControl.m_oLogicDocument;
	var ParaPr = Doc.Get_Paragraph_ParaPr();
	var TextPr = Doc.Get_Paragraph_TextPr();

	// return { ParaPr: ParaPr, TextPr : TextPr };
	return new CParagraphAndTextProp (ParaPr, TextPr);	// uncomment if this method will be used externally. 20/03/2012 uncommented for testers
}

// -------
// тут методы, замены евентов
asc_docs_api.prototype.get_PropertyEditorShapes = function()
{
}
asc_docs_api.prototype.get_PropertyEditorFonts = function()
{
}
asc_docs_api.prototype.get_PropertyStandartTextures = function()
{
}
asc_docs_api.prototype.get_PropertyEditorThemes = function()
{
}

// -------

// -------
asc_docs_api.prototype.get_ContentCount = function()
{
}

asc_docs_api.prototype.select_Element = function(Index)
{
}

asc_docs_api.prototype.UpdateTextPr = function(TextPr)
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

asc_docs_api.prototype.sync_VerticalTextAlign = function(align)
{
}

asc_docs_api.prototype.UpdateParagraphProp = function(ParaPr){	
}
/*----------------------------------------------------------------*/
/*functions for working with clipboard, document*/
/*TODO: Print,Undo,Redo,Copy,Cut,Paste,Share,Save,DownloadAs,ReturnToDocuments(вернуться на предыдущую страницу) & callbacks for these functions*/
asc_docs_api.prototype.asc_Print = function(){
}
asc_docs_api.prototype.Undo = function(){
	this.WordControl.m_oLogicDocument.Document_Undo();
}
asc_docs_api.prototype.Redo = function(){
	this.WordControl.m_oLogicDocument.Document_Redo();
}
asc_docs_api.prototype.Copy = function(){
}
asc_docs_api.prototype.Update_ParaTab = function(Default_Tab, ParaTabs){
}
asc_docs_api.prototype.Cut = function(){
}
asc_docs_api.prototype.Paste = function(){
}
asc_docs_api.prototype.Share = function(){

}
asc_docs_api.prototype.asc_Save = function (isAutoSave) {
};
asc_docs_api.prototype.asc_OnSaveEnd = function (isDocumentSaved) {
};
asc_docs_api.prototype.processSavedFile = function(url, bInner){
}
asc_docs_api.prototype.asc_DownloadAs = function(typeFile){//передаем число соответствующее своему формату.
}
asc_docs_api.prototype.Resize = function(){
}
asc_docs_api.prototype.AddURL = function(url){

}
asc_docs_api.prototype.Help = function(){

}
asc_docs_api.prototype.ClearCache = function(){
}
asc_docs_api.prototype.startGetDocInfo = function(){
}
asc_docs_api.prototype.stopGetDocInfo = function(){
}
asc_docs_api.prototype.sync_DocInfoCallback = function(obj){
}
asc_docs_api.prototype.sync_GetDocInfoStartCallback = function(){
}
asc_docs_api.prototype.sync_GetDocInfoStopCallback = function(){
}
asc_docs_api.prototype.sync_GetDocInfoEndCallback = function(){
}
asc_docs_api.prototype.sync_CanUndoCallback = function(bCanUndo)
{
}
asc_docs_api.prototype.sync_CanRedoCallback = function(bCanRedo)
{
}


/*callbacks*/
/*asc_docs_api.prototype.sync_CursorLockCallBack = function(isLock){
	this.asc_fireCallback("asc_onCursorLock",isLock);
}*/
asc_docs_api.prototype.sync_PrintCallBack = function(){
}
asc_docs_api.prototype.sync_UndoCallBack = function(){
}
asc_docs_api.prototype.sync_RedoCallBack = function(){
}
asc_docs_api.prototype.sync_CopyCallBack = function(){
}
asc_docs_api.prototype.sync_CutCallBack = function(){
}
asc_docs_api.prototype.sync_PasteCallBack = function(){
}
asc_docs_api.prototype.sync_ShareCallBack = function(){
}
asc_docs_api.prototype.sync_SaveCallBack = function(){
}
asc_docs_api.prototype.sync_DownloadAsCallBack = function(){
}
asc_docs_api.prototype.sync_StartAction = function(type, id){
	//this.AsyncAction
	this.asc_fireCallback("asc_onStartAction", type, id);
}
asc_docs_api.prototype.sync_EndAction = function(type, id){
	//this.AsyncAction
	this.asc_fireCallback("asc_onEndAction", type, id);
}
asc_docs_api.prototype.sync_AddURLCallback = function(){
}
asc_docs_api.prototype.sync_ErrorCallback = function(errorID,errorLevel){
}
asc_docs_api.prototype.sync_HelpCallback = function(url){
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
asc_docs_api.prototype.startSearchText = function(what){// "what" means word(s) what we search
}

asc_docs_api.prototype.goToNextSearchResult = function()
{
}


asc_docs_api.prototype.gotoSearchResultText = function(navigator){//переход к результату.
}
asc_docs_api.prototype.stopSearchText = function(){
}
asc_docs_api.prototype.findText = function(text, scanForward){
}

asc_docs_api.prototype.asc_searchEnabled = function(bIsEnabled)
{
    // пустой метод
}

asc_docs_api.prototype.asc_findText = function(text, isNext, isMatchCase)
{
}
// returns: CSearchResult
asc_docs_api.prototype.sync_SearchFoundCallback = function(obj){
}
asc_docs_api.prototype.sync_SearchStartCallback = function(){
}
asc_docs_api.prototype.sync_SearchStopCallback = function(){
}
asc_docs_api.prototype.sync_SearchEndCallback = function(){
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
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Bold : value } ) );
}
asc_docs_api.prototype.put_TextPrItalic = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Italic : value } ) );
}
asc_docs_api.prototype.put_TextPrUnderline = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Underline : value } ) );
}
asc_docs_api.prototype.put_TextPrStrikeout = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Strikeout : value } ) );
}
asc_docs_api.prototype.put_PrLineSpacing = function(Type, Value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphSpacing( { LineRule : Type,  Line : Value } );
}
asc_docs_api.prototype.put_LineSpacingBeforeAfter = function(type,value)//"type == 0" means "Before", "type == 1" means "After"
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	switch (type)
    {
		case 0:
			this.WordControl.m_oLogicDocument.Set_ParagraphSpacing( {Before : value}); break;
		case 1:
			this.WordControl.m_oLogicDocument.Set_ParagraphSpacing( {After : value}); break;
	}
}
asc_docs_api.prototype.FontSizeIn = function()
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Paragraph_IncDecFontSize(true);
}
asc_docs_api.prototype.FontSizeOut = function()
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Paragraph_IncDecFontSize(false);
}

/*callbacks*/
asc_docs_api.prototype.sync_BoldCallBack = function(isBold){
}
asc_docs_api.prototype.sync_ItalicCallBack = function(isItalic){
}
asc_docs_api.prototype.sync_UnderlineCallBack = function(isUnderline){
}
asc_docs_api.prototype.sync_StrikeoutCallBack = function(isStrikeout){
}
asc_docs_api.prototype.sync_TextPrFontFamilyCallBack = function(FontFamily){
}	
asc_docs_api.prototype.sync_TextPrFontSizeCallBack = function(FontSize){
}	
asc_docs_api.prototype.sync_PrLineSpacingCallBack = function(LineSpacing){
}


asc_docs_api.prototype.sync_InitEditorFonts = function(gui_fonts){
}
asc_docs_api.prototype.sync_InitEditorThemes = function(gui_editor_themes, gui_document_themes){
}
asc_docs_api.prototype.sync_InitEditorTableStyles = function(styles){
};

asc_docs_api.prototype.onSaveCallback = function (e) {	
};

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
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphAlign(value);
}
// 0- baseline, 2-subscript, 1-superscript
asc_docs_api.prototype.put_TextPrBaseline = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { VertAlign : value } ) );
}
/* 	Маркированный список Type = 0
		нет         - SubType = -1
		черная точка - SubType = 1
		круг         - SubType = 2
		квадрат      - SubType = 3
		картинка     - SubType = -1
		4 ромба      - SubType = 4
		ч/б стрелка  - SubType = 5
		галка        - SubType = 6
	
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
		нет            - SubType = -1
		1)a)i)        - SubType = 1
		1.1.1         - SubType = 2
		маркированный - SubType = 3 
*/
asc_docs_api.prototype.put_ListType = function(type, subtype)
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
asc_docs_api.prototype.put_Style = function(name)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphStyle(name);
}
asc_docs_api.prototype.put_ShowParaMarks = function(isShow){
}
asc_docs_api.prototype.get_ShowParaMarks = function(){
    return this.ShowParaMarks;
}
asc_docs_api.prototype.put_ShowTableEmptyLine = function(isShow)
{
}
asc_docs_api.prototype.get_ShowTableEmptyLine = function(){
    return this.isShowTableEmptyLine;
}

asc_docs_api.prototype.put_KeepLines = function(isKeepLines){
	this.isKeepLinesTogether = isKeepLines;
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Set_ParagraphKeepLines(isKeepLines);
	this.sync_KeepLinesCallback(isKeepLines);
}
asc_docs_api.prototype.put_AddSpaceBetweenPrg = function(isSpacePrg){
	this.isAddSpaceBetweenPrg = isSpacePrg;
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Set_ParagraphContextualSpacing(isSpacePrg);
}
asc_docs_api.prototype.put_ShapeFillColor = function(is_flag, r, g, b)
{
	if (false === is_flag)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        var Unifill = new CUniFill();
        Unifill.fill = new CNoFill();
        Unifill.calculate();
        this.WordControl.m_oLogicDocument.changeShapeFill( Unifill );
    }
	else
	{
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        var Unifill = new CUniFill();
        Unifill.fill = new CSolidFill();
        Unifill.fill.color = new CRGBColor();
        Unifill.fill.color.RGBA = { R : r, G : g, B: b, A : 255};
        Unifill.calculate();
		this.WordControl.m_oLogicDocument.changeShapeFill( Unifill );
	}
}
asc_docs_api.prototype.ShapeApply = function(prop)
{    
}

asc_docs_api.prototype.setStartPointHistory = function(){this.noCreatePoint = true; History.Create_NewPoint();};
asc_docs_api.prototype.setEndPointHistory   = function(){this.noCreatePoint = false; };
asc_docs_api.prototype.SetSlideProps = function(prop)
{    
}

asc_docs_api.prototype.put_LineCap = function(_cap)
{
    this.WordControl.m_oLogicDocument.putLineCap(_cap);
}
asc_docs_api.prototype.put_LineJoin = function(_join)
{
    this.WordControl.m_oLogicDocument.putLineJoin(_join);
}

asc_docs_api.prototype.put_LineBeginStyle = function(_style)
{
    this.WordControl.m_oLogicDocument.putLineBeginStyle(_style);
}
asc_docs_api.prototype.put_LineBeginSize = function(_size)
{
    this.WordControl.m_oLogicDocument.putLineBeginSize(_size);
}

asc_docs_api.prototype.put_LineEndStyle = function(_style)
{
    this.WordControl.m_oLogicDocument.putLineEndStyle(_style);
}
asc_docs_api.prototype.put_LineEndSize = function(_size)
{
    this.WordControl.m_oLogicDocument.putLineEndSize(_size);
}

asc_docs_api.prototype.put_TextColor2 = function(r, g, b)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { Color : { r : r, g : g, b: b}  } ) );
}
asc_docs_api.prototype.put_TextColor = function(color)
{
    var _unifill = new CUniFill();
    _unifill.fill = new CSolidFill();
    _unifill.fill.color = CorrectUniColor(color, _unifill.fill.color);
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaTextPr( { unifill : _unifill } ) );
}
asc_docs_api.prototype.put_ParagraphShade = function(is_flag, r, g, b)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	if (false === is_flag)
		this.WordControl.m_oLogicDocument.Set_ParagraphShd( { Value : shd_Nil  }  );
	else
	{
		this.WordControl.m_oLogicDocument.Set_ParagraphShd( { Value : shd_Clear, Color : { r : r, g : g, b: b} } );
	}
}
asc_docs_api.prototype.put_PrIndent = function(value,levelValue)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { Left : value, ChangeLevel: levelValue } );
}
asc_docs_api.prototype.IncreaseIndent = function()
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { ChangeLevel: 1 } );
    this.WordControl.m_oLogicDocument.Document_UpdateInterfaceState();
}
asc_docs_api.prototype.DecreaseIndent = function()
{
    this.WordControl.m_oLogicDocument.Set_ParagraphIndent( {ChangeLevel: -1 } );
    this.WordControl.m_oLogicDocument.Document_UpdateInterfaceState();
}
asc_docs_api.prototype.put_PrIndentRight = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { Right : value } );
}
asc_docs_api.prototype.put_PrFirstLineIndent = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Set_ParagraphIndent( { FirstLine : value } );
}
asc_docs_api.prototype.getFocusObject = function(){//возвратит тип элемента - параграф c_oAscTypeSelectElement.Paragraph, изображение c_oAscTypeSelectElement.Image, таблица c_oAscTypeSelectElement.Table, колонтитул c_oAscTypeSelectElement.Header.

}

/*callbacks*/
asc_docs_api.prototype.sync_VerticalAlign = function(typeBaseline){
	this.asc_fireCallback("asc_onVerticalAlign",typeBaseline);
}
asc_docs_api.prototype.sync_PrAlignCallBack = function(value){
	this.asc_fireCallback("asc_onPrAlign",value);
}
asc_docs_api.prototype.sync_ListType = function(NumPr){
	this.asc_fireCallback("asc_onListType", new CListType( NumPr ) );
}
asc_docs_api.prototype.sync_TextColor = function(Color){
	this.asc_fireCallback("asc_onTextColor", new CColor( Color.r, Color.g, Color.b ));
}
asc_docs_api.prototype.sync_TextColor2 = function(unifill, _color)
{
    if (unifill.fill == null)
        return;
    else if (unifill.fill.type == FILL_TYPE_SOLID)
    {
        var color = CreateAscColor(unifill.fill.color);
        color.put_r(_color.r);
        color.put_g(_color.g);
        color.put_b(_color.b);
        this.asc_fireCallback("asc_onTextColor", color);
    }
    else if (unifill.fill.type == FILL_TYPE_GRAD)
    {
        var color = CreateAscColor(unifill.fill.colors[0].color);
        color.put_r(_color.r);
        color.put_g(_color.g);
        color.put_b(_color.b);
        this.asc_fireCallback("asc_onTextColor", color);
    }
    else
    {
        var color = new CAscColor();
        color.put_r(_color.r);
        color.put_g(_color.g);
        color.put_b(_color.b);
        this.asc_fireCallback("asc_onTextColor", color);
    }
}
asc_docs_api.prototype.sync_TextHighLight = function(HighLight){
	this.asc_fireCallback("asc_onTextHighLight", new CColor( HighLight.r, HighLight.g, HighLight.b ) );
}
asc_docs_api.prototype.sync_ParaStyleName = function(Name){
	this.asc_fireCallback("asc_onParaStyleName",Name);
}
asc_docs_api.prototype.sync_ParaSpacingLine = function(SpacingLine){
	this.asc_fireCallback("asc_onParaSpacingLine", new CParagraphSpacing( SpacingLine ));
}
asc_docs_api.prototype.sync_PageBreakCallback = function(isBreak){
	this.asc_fireCallback("asc_onPageBreak",isBreak);
}
asc_docs_api.prototype.sync_KeepLinesCallback = function(isKeepLines){
	this.asc_fireCallback("asc_onKeepLines",isKeepLines);
}
asc_docs_api.prototype.sync_ShowParaMarksCallback = function(){
	this.asc_fireCallback("asc_onShowParaMarks");
}
asc_docs_api.prototype.sync_SpaceBetweenPrgCallback = function(){
	this.asc_fireCallback("asc_onSpaceBetweenPrg");
}
asc_docs_api.prototype.sync_PrPropCallback = function(prProp){
    var _len = this.SelectedObjectsStack.length;
    if (_len > 0)
    {
        if (this.SelectedObjectsStack[_len - 1].Type == c_oAscTypeSelectElement.Paragraph)
        {
            this.SelectedObjectsStack[_len - 1].Value = new CParagraphProp( prProp );
            return;
        }
    }

    this.SelectedObjectsStack[this.SelectedObjectsStack.length] = new CSelectedObject( c_oAscTypeSelectElement.Paragraph, new CParagraphProp( prProp ) );
}

asc_docs_api.prototype.SetDrawImagePlaceParagraph = function(element_id, props)
{
}

/*----------------------------------------------------------------*/
/*functions for working with page*/
asc_docs_api.prototype.change_PageOrient = function(isPortrait)
{
    this.WordControl.m_oDrawingDocument.m_bIsUpdateDocSize = true;

    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    if (isPortrait)
    {
        this.WordControl.m_oLogicDocument.Set_DocumentOrientation(orientation_Portrait);
        this.DocumentOrientation = orientation_Portrait ? true : false ;
    }
    else
    {
        this.WordControl.m_oLogicDocument.Set_DocumentOrientation(orientation_Landscape);
        this.DocumentOrientation = orientation_Landscape ? true : false;
    }
	this.sync_PageOrientCallback(!editor.get_DocumentOrientation());
}
asc_docs_api.prototype.get_DocumentOrientation = function()
{
	return this.DocumentOrientation;
}
asc_docs_api.prototype.change_DocSize = function(width,height)
{
    this.WordControl.m_oDrawingDocument.m_bIsUpdateDocSize = true;

    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    if (this.DocumentOrientation == orientation_Portrait)
        this.WordControl.m_oLogicDocument.Set_DocumentPageSize(width, height);
    else
        this.WordControl.m_oLogicDocument.Set_DocumentPageSize(height, width);
}
asc_docs_api.prototype.put_AddPageBreak = function()
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Paragraph_Add( new ParaNewLine( break_Page ) );
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
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Document_AddPageNum( where, align );
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.put_HeadersAndFooters = function(where,options)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Document_AddHdrFtr( where, options );
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.rem_HeadersAndFooters = function(where,options)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Document_RemoveHdrFtr( where, options );
}

// "where" где нижний или верхний, "options" опции колонтитула
asc_docs_api.prototype.put_HeadersAndFootersDistance = function(value)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
	this.WordControl.m_oLogicDocument.Document_SetHdrFtrDistance(value);
}

asc_docs_api.prototype.HeadersAndFooters_DifferentFirstPage = function(isOn){
	if ( isOn )
	{
		this.put_HeadersAndFooters( hdrftr_Footer, hdrftr_First );
		this.put_HeadersAndFooters( hdrftr_Header, hdrftr_First );
	}
	else
	{
		this.rem_HeadersAndFooters( hdrftr_Footer, hdrftr_First );
		this.rem_HeadersAndFooters( hdrftr_Header, hdrftr_First );
	}
}
asc_docs_api.prototype.HeadersAndFooters_DifferentOddandEvenPage = function(isOn){
	if ( isOn )
	{
		this.put_HeadersAndFooters( hdrftr_Footer, hdrftr_Even );
		this.put_HeadersAndFooters( hdrftr_Header, hdrftr_Even );
	}
	else
	{
		this.rem_HeadersAndFooters( hdrftr_Footer, hdrftr_Even );
		this.rem_HeadersAndFooters( hdrftr_Header, hdrftr_Even );
	}
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
	this.asc_fireCallback("asc_onDocSize",width,height);
}
asc_docs_api.prototype.sync_PageOrientCallback = function(isPortrait){
	this.asc_fireCallback("asc_onPageOrient",isPortrait);
}
asc_docs_api.prototype.sync_HeadersAndFootersPropCallback = function(hafProp){
    this.SelectedObjectsStack[this.SelectedObjectsStack.length] = new CSelectedObject( c_oAscTypeSelectElement.Header, new CHeaderProp( hafProp ) );
}

/*----------------------------------------------------------------*/
/*functions for working with table*/
asc_docs_api.prototype.put_Table = function(col,row)
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Add_FlowTable(col,row);
    }
}
asc_docs_api.prototype.addRowAbove = function(count)
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_AddRow(true);
    }
}
asc_docs_api.prototype.addRowBelow = function(count)
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_AddRow(false);
    }
}
asc_docs_api.prototype.addColumnLeft = function(count)
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_AddCol(true);
    }
}
asc_docs_api.prototype.addColumnRight = function(count)
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_AddCol(false);
    }
}
asc_docs_api.prototype.remRow = function()
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_RemoveRow();
    }
}
asc_docs_api.prototype.remColumn = function()
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_RemoveCol();
    }
}
asc_docs_api.prototype.remTable = function()
{
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Table_RemoveTable();
    }
}
asc_docs_api.prototype.selectRow = function()
{
    this.WordControl.m_oLogicDocument.Table_Select( c_oAscTableSelectionType.Row );
}
asc_docs_api.prototype.selectColumn = function()
{
    this.WordControl.m_oLogicDocument.Table_Select( c_oAscTableSelectionType.Column );
}
asc_docs_api.prototype.selectCell = function()
{
    this.WordControl.m_oLogicDocument.Table_Select( c_oAscTableSelectionType.Cell );
}
asc_docs_api.prototype.selectTable = function()
{
    this.WordControl.m_oLogicDocument.Table_Select( c_oAscTableSelectionType.Table );
}
asc_docs_api.prototype.setColumnWidth = function(width){

}
asc_docs_api.prototype.setRowHeight = function(height){

}
asc_docs_api.prototype.set_TblDistanceFromText = function(left,top,right,bottom){
	
}
asc_docs_api.prototype.CheckBeforeMergeCells = function()
{
    return this.WordControl.m_oLogicDocument.Table_CheckMerge();
}
asc_docs_api.prototype.CheckBeforeSplitCells = function()
{
    return this.WordControl.m_oLogicDocument.Table_CheckSplit();
}
asc_docs_api.prototype.MergeCells = function()
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Table_MergeCells();
}
asc_docs_api.prototype.SplitCell = function(Cols, Rows)
{
    this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
    this.WordControl.m_oLogicDocument.Table_SplitCell(Cols, Rows);
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
asc_docs_api.prototype.set_TableBackground = function(Color)
{

}
asc_docs_api.prototype.set_AlignCell = function(align){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
	switch(align)
	{
		case c_oAscAlignType.LEFT : break;
		case c_oAscAlignType.CENTER : break;
		case c_oAscAlignType.RIGHT : break;
	}
}
asc_docs_api.prototype.set_TblAlign = function(align){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
	switch(align)
	{
		case c_oAscAlignType.LEFT : break;
		case c_oAscAlignType.CENTER : break;
		case c_oAscAlignType.RIGHT : break;
	}
}
asc_docs_api.prototype.set_SpacingBetweenCells = function(isOn,spacing){// c_oAscAlignType.RIGHT, c_oAscAlignType.LEFT, c_oAscAlignType.CENTER
	if(isOn){
	
	}
}


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
    var doc = this.WordControl.m_oLogicDocument;
    if(doc.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        this.WordControl.m_oLogicDocument.Create_NewHistoryPoint();
        this.WordControl.m_oLogicDocument.Set_TableProps(obj);
    }
}
/*callbacks*/
asc_docs_api.prototype.sync_AddTableCallback = function(){
	this.asc_fireCallback("asc_onAddTable");
}
asc_docs_api.prototype.sync_AlignCellCallback = function(align){
	this.asc_fireCallback("asc_onAlignCell",align);
}	
asc_docs_api.prototype.sync_TblPropCallback = function(tblProp){
    this.SelectedObjectsStack[this.SelectedObjectsStack.length] = new CSelectedObject( c_oAscTypeSelectElement.Table, new CTableProp( tblProp ));
}
asc_docs_api.prototype.sync_TblWrapStyleChangedCallback = function(style){
	this.asc_fireCallback("asc_onTblWrapStyleChanged",style);
}
asc_docs_api.prototype.sync_TblAlignChangedCallback = function(style){
	this.asc_fireCallback("asc_onTblAlignChanged",style);
}

/*----------------------------------------------------------------*/
/*functions for working with images*/
asc_docs_api.prototype.ChangeImageFromFile = function()
{
    this.isImageChangeUrl = true;
    this.AddImage();
}
asc_docs_api.prototype.ChangeShapeImageFromFile = function()
{
    this.isShapeImageChangeUrl = true;
    this.AddImage();
}
asc_docs_api.prototype.ChangeSlideImageFromFile = function()
{
    this.isSlideImageChangeUrl = true;
    this.AddImage();
}

asc_docs_api.prototype.AddImage = function(){    
}

asc_docs_api.prototype.StartAddShape = function(prst, is_apply)
{
}


asc_docs_api.prototype.canGroup = function()
{
    return this.WordControl.m_oLogicDocument.canGroup();
}

asc_docs_api.prototype.canUnGroup = function()
{
    return this.WordControl.m_oLogicDocument.canUnGroup();
}

asc_docs_api.prototype.AddImageUrl = function(url){
};
asc_docs_api.prototype.AddImageUrlAction = function(url){    
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
asc_docs_api.prototype.ImgApply = function(obj){    
}
asc_docs_api.prototype.ChartApply = function(obj)
{
    if(this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Drawing_Props) === false)
    {
        History.Create_NewPoint();
        this.WordControl.m_oLogicDocument.ChartApply(obj);
    }
}
asc_docs_api.prototype.set_Size = function(width, height){

}
asc_docs_api.prototype.set_ConstProportions = function(isOn){
	if (isOn){
	
	}
	else{
	
	}
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
/*callbacks*/
asc_docs_api.prototype.sync_AddImageCallback = function(){
	this.asc_fireCallback("asc_onAddImage");
}
asc_docs_api.prototype.sync_ImgPropCallback = function(imgProp){
    var type = imgProp.ChartProperties ? c_oAscTypeSelectElement.Chart : c_oAscTypeSelectElement.Image;
    var prop = type === c_oAscTypeSelectElement.Chart ? new CAscChartProp(imgProp) : new CImgProperty( imgProp );
    this.SelectedObjectsStack[this.SelectedObjectsStack.length] = new CSelectedObject( type, prop );
}
asc_docs_api.prototype.sync_ImgWrapStyleChangedCallback = function(style){
	this.asc_fireCallback("asc_onImgWrapStyleChanged",style);
}

asc_docs_api.prototype.SetDrawingFreeze = function(bIsFreeze)
{
}

/*----------------------------------------------------------------*/
/*functions for working with zoom & navigation*/
asc_docs_api.prototype.zoomIn = function(){
    this.WordControl.zoom_In();
}
asc_docs_api.prototype.zoomOut = function(){
    this.WordControl.zoom_Out();
}
asc_docs_api.prototype.zoomFitToPage = function(){
    this.WordControl.zoom_FitToPage();
}
asc_docs_api.prototype.zoomFitToWidth = function(){
    this.WordControl.zoom_FitToWidth();
}
asc_docs_api.prototype.zoomCustomMode = function(){
    this.WordControl.m_nZoomType = 0;
    this.WordControl.zoom_Fire();
}
asc_docs_api.prototype.zoom100 = function(){
    this.WordControl.m_nZoomValue = 100;
    this.WordControl.zoom_Fire();
}
asc_docs_api.prototype.zoom = function(percent){
    this.WordControl.m_nZoomValue = percent;
    this.WordControl.zoom_Fire(0);
}	
asc_docs_api.prototype.goToPage = function(number){
	this.WordControl.GoToPage(number);
}
asc_docs_api.prototype.getCountPages = function(){
	return this.WordControl.m_oDrawingDocument.SlidesCount;
}
asc_docs_api.prototype.getCurrentPage = function(){
	return this.WordControl.m_oDrawingDocument.SlideCurrent;
}
/*callbacks*/
asc_docs_api.prototype.sync_zoomChangeCallback = function(percent,type){	//c_oAscZoomType.Current, c_oAscZoomType.FitWidth, c_oAscZoomType.FitPage
	this.asc_fireCallback("asc_onZoomChange",percent,type);
}
asc_docs_api.prototype.sync_countPagesCallback = function(count){
	this.asc_fireCallback("asc_onCountPages",count);
}
asc_docs_api.prototype.sync_currentPageCallback = function(number){
	this.asc_fireCallback("asc_onCurrentPage",number);
}

asc_docs_api.prototype.sync_SendThemeColors = function(colors,standart_colors)
{
    this.asc_fireCallback("asc_onSendThemeColors",colors,standart_colors);
}

asc_docs_api.prototype.sync_SendThemeColorSchemes = function(param)
{
    this.asc_fireCallback("asc_onSendThemeColorSchemes",param);
}

asc_docs_api.prototype.ChangeColorScheme = function(index_scheme)
{
    var _count_defaults = g_oUserColorScheme.length;
    if (index_scheme < _count_defaults)
    {
        var _obj = g_oUserColorScheme[index_scheme];
        var scheme = new ClrScheme();
		scheme.name = _obj["name"];
        var _c = null;

        _c = _obj["dk1"];
        scheme.colors[8] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["lt1"];
        scheme.colors[12] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["dk2"];
        scheme.colors[9] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["lt2"];
        scheme.colors[13] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent1"];
        scheme.colors[0] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent2"];
        scheme.colors[1] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent3"];
        scheme.colors[2] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent4"];
        scheme.colors[3] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent5"];
        scheme.colors[4] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["accent6"];
        scheme.colors[5] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["hlink"];
        scheme.colors[11] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        _c = _obj["folHlink"];
        scheme.colors[10] = CreateUniColorRGB(_c["R"], _c["G"], _c["B"]);

        this.WordControl.m_oLogicDocument.changeColorScheme(scheme);
    }
    else
    {
        index_scheme -= _count_defaults;
        if (null == this.WordControl.MasterLayouts)
            return;

        var theme = this.WordControl.MasterLayouts.Theme;
        if (null == theme)
            return;

        if (index_scheme < 0 || index_scheme >= theme.extraClrSchemeLst.length)
            return;

        this.WordControl.m_oLogicDocument.changeColorScheme(theme.extraClrSchemeLst[index_scheme].clrScheme);
    }

    this.WordControl.m_oDrawingDocument.CheckGuiControlColors();
}

/*----------------------------------------------------------------*/
asc_docs_api.prototype.asc_enableKeyEvents = function(value){
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

// работа с шрифтами
asc_docs_api.prototype.asyncFontsDocumentStartLoaded = function()
{	
}
asc_docs_api.prototype.GenerateStyles = function()
{
    return;
}
asc_docs_api.prototype.asyncFontsDocumentEndLoaded = function()
{    
}

asc_docs_api.prototype.asyncImagesDocumentStartLoaded = function()
{
}
asc_docs_api.prototype.asyncImagesDocumentEndLoaded = function()
{    
}

asc_docs_api.prototype.asc_getComments = function()
{
    var comms = [];
    if (null == this.WordControl.m_oLogicDocument)
        return comms;

    var _slides = this.WordControl.m_oLogicDocument.Slides;
    var _slidesCount = _slides.length;
    for (var i = 0; i < _slidesCount; i++)
    {
        var _comments = _slides[i].slideComments.comments;
        var _commentsCount = _comments.length;

        for (var j = 0; j < _commentsCount; j++)
        {
            var _id = _comments[j].Get_Id();
            var _ascCommentData = new asc_CCommentData(_comments[j].Data);

            comms.push({ "Id" : _id, "Comment" : _ascCommentData });
        }
    }
    return comms;
}

asc_docs_api.prototype.OpenDocumentEndCallback = function()
{
    this.bNoSendComments = false;
    var bIsScroll = false;
    
    this.WordControl.m_oLogicDocument.RecalculateAfterOpen();
    var presentation = this.WordControl.m_oLogicDocument;

    presentation.DrawingDocument.OnEndRecalculate();
};

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

asc_docs_api.prototype.get_PresentationWidth = function()
{
    if (this.WordControl.m_oLogicDocument == null)
        return 0;
    return this.WordControl.m_oLogicDocument.Width;
}
asc_docs_api.prototype.get_PresentationHeight = function()
{
    if (this.WordControl.m_oLogicDocument == null)
        return 0;
    return this.WordControl.m_oLogicDocument.Height;
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

asc_docs_api.prototype.pre_SaveCallback = function()
{
}

asc_docs_api.prototype.initEvents2MobileAdvances = function()
{
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

asc_docs_api.prototype.changeSlideSize = function(width, height)
{
}

asc_docs_api.prototype.AddSlide = function(layoutIndex)
{
    this.WordControl.m_oLogicDocument.addNextSlide(layoutIndex);
}
asc_docs_api.prototype.DeleteSlide = function()
{
    var _delete_array = this.WordControl.Thumbnails.GetSelectedArray();

    if (!this.IsSupportEmptyPresentation)
    {
        if (_delete_array.length == this.WordControl.m_oDrawingDocument.SlidesCount)
            _delete_array.splice(0, 1);
    }

    if (_delete_array.length != 0)
    {
        this.WordControl.m_oLogicDocument.deleteSlides(_delete_array);
    }
}
asc_docs_api.prototype.DublicateSlide = function()
{
    var selectionArray = this.WordControl.Thumbnails.GetSelectedArray();
    var _presentation = this.WordControl.m_oLogicDocument;

    var div = document.createElement("div");
    var copy_processor = new CopyProcessor(editor, div);
    copy_processor.oPresentationWriter.End_UseFullUrl();
    copy_processor.Start();
    var oPasteProcessor = new PasteProcessor(this, true, true, false);
    //oPasteProcessor.End_UseFullUrl();
    History.Create_NewPoint();
    oPasteProcessor.Start(copy_processor.ElemToSelect, copy_processor.ElemToSelect, true);
    this.WordControl.m_oLogicDocument.Document_UpdateInterfaceState();
}

asc_docs_api.prototype.SelectAllSlides = function(layoutType)
{
    var drDoc = this.WordControl.m_oDrawingDocument;
    var slidesCount = drDoc.SlidesCount;

    for (var i = 0; i < slidesCount; i++)
    {
        this.WordControl.Thumbnails.m_arrPages[i].IsSelected = true;
    }
    this.WordControl.Thumbnails.OnUpdateOverlay();
}

asc_docs_api.prototype.AddShape = function(shapetype)
{
}
asc_docs_api.prototype.ChangeShapeType = function(shapetype)
{
    History.Create_NewPoint();
    this.WordControl.m_oLogicDocument.changeShapeType(shapetype);
}
asc_docs_api.prototype.AddText = function()
{
}

asc_docs_api.prototype.groupShapes = function()
{
    this.WordControl.m_oLogicDocument.groupShapes();
}

asc_docs_api.prototype.unGroupShapes = function()
{
    this.WordControl.m_oLogicDocument.unGroupShapes();
}

asc_docs_api.prototype.setVerticalAlign = function(align)
{
    History.Create_NewPoint();
    this.WordControl.m_oLogicDocument.setVerticalAlign(align);
}

asc_docs_api.prototype.sync_MouseMoveStartCallback = function()
{
    this.asc_fireCallback("asc_onMouseMoveStart");
}

asc_docs_api.prototype.sync_MouseMoveEndCallback = function()
{
    this.asc_fireCallback("asc_onMouseMoveEnd");
}

asc_docs_api.prototype.sync_MouseMoveCallback = function(Data)
{
    this.asc_fireCallback("asc_onMouseMove", Data );
}

asc_docs_api.prototype.ShowThumbnails = function(bIsShow)
{
    if (bIsShow)
    {
        this.WordControl.Splitter1Pos = this.WordControl.OldSplitter1Pos;
        if (this.WordControl.Splitter1Pos == 0)
            this.WordControl.Splitter1Pos = 70;
        this.WordControl.OnResizeSplitter();
    }
    else
    {
        var old = this.WordControl.OldSplitter1Pos;
        this.WordControl.Splitter1Pos = 0;
        this.WordControl.OnResizeSplitter();
        this.WordControl.OldSplitter1Pos = old;
    }
}

asc_docs_api.prototype.syncOnThumbnailsShow = function()
{
    var bIsShow = true;
    if (0 == this.WordControl.Splitter1Pos)
        bIsShow = false;

    this.asc_fireCallback("asc_onThumbnailsShow", bIsShow);
}



//-----------------------------------------------------------------
// Функции для работы с гиперссылками
//-----------------------------------------------------------------
asc_docs_api.prototype.can_AddHyperlink = function()
{
    //if ( true === CollaborativeEditing.Get_GlobalLock() )
    //    return false;

    var bCanAdd = this.WordControl.m_oLogicDocument.Hyperlink_CanAdd();
    if ( true === bCanAdd )
        return this.WordControl.m_oLogicDocument.Get_SelectedText(true);

    return false;
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
        this.ToolTip = (undefined != obj.ToolTip) ? obj.ToolTip : null;
    }
    else
    {
        this.Text    = null;
        this.Value   = "";
        this.ToolTip = null;
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


asc_docs_api.prototype.GoToFooter = function(pageNumber)
{
}
asc_docs_api.prototype.sync_shapePropCallback = function(pr)
{
}

asc_docs_api.prototype.sync_slidePropCallback = function(slide)
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

asc_docs_api.prototype.sync_PaintFormatCallback = function(value)
{
}
asc_docs_api.prototype.ClearFormating = function()
{
}
asc_docs_api.prototype.SetDeviceInputHelperId = function(idKeyboard)
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
    if (this.WordControl.m_oLogicDocument != null)
        return true;
    return false;
}

asc_docs_api.prototype.can_AddHyperlink = function()
{
}
asc_docs_api.prototype.add_Hyperlink = function(HyperProps)
{
}
asc_docs_api.prototype.sync_HyperlinkClickCallback = function(Url)
{    
}

asc_docs_api.prototype.UpdateInterfaceState = function()
{
}

asc_docs_api.prototype.OnMouseUp = function(x, y)
{
}
asc_docs_api.prototype.OnHandleMessage = function(event)
{
}
asc_docs_api.prototype.asyncImageEndLoaded2 = null;

asc_docs_api.prototype.ChangeTheme = function(indexTheme)
{
}

asc_docs_api.prototype.StartLoadTheme = function()
{
}
asc_docs_api.prototype.EndLoadTheme = function(theme_load_info)
{
}

asc_docs_api.prototype.ChangeLayout = function(layout_index)
{
}

asc_docs_api.prototype.put_ShapesAlign = function(type)
{    
};
asc_docs_api.prototype.DistributeHorizontally = function()
{
};
asc_docs_api.prototype.DistributeVertically = function()
{
};
asc_docs_api.prototype.shapes_alignLeft = function()
{
};

asc_docs_api.prototype.shapes_alignRight = function()
{
};

asc_docs_api.prototype.shapes_alignTop = function()
{
};

asc_docs_api.prototype.shapes_alignBottom = function()
{
};

asc_docs_api.prototype.shapes_alignCenter = function()
{
};

asc_docs_api.prototype.shapes_alignMiddle = function()
{
};

asc_docs_api.prototype.shapes_bringToFront = function()
{
};

asc_docs_api.prototype.shapes_bringForward = function()
{
};

asc_docs_api.prototype.shapes_bringToBack = function()
{
};

asc_docs_api.prototype.shapes_bringBackward = function()
{
};

asc_docs_api.prototype.sync_endDemonstration = function()
{
};
asc_docs_api.prototype.sync_DemonstrationSlideChanged = function(slideNum)
{
};

asc_docs_api.prototype.StartDemonstration = function(div_id, slidestart_num)
{
}

asc_docs_api.prototype.EndDemonstration = function()
{
}

asc_docs_api.prototype.DemonstrationPlay = function()
{
}

asc_docs_api.prototype.DemonstrationPause = function()
{
}

asc_docs_api.prototype.DemonstrationEndShowMessage = function(message)
{
}

asc_docs_api.prototype.DemonstrationNextSlide = function()
{
}

asc_docs_api.prototype.DemonstrationPrevSlide = function()
{
}

asc_docs_api.prototype.DemonstrationGoToSlide = function(slideNum)
{
}

asc_docs_api.prototype.ApplySlideTiming = function(oTiming)
{
}
asc_docs_api.prototype.SlideTimingApplyToAll = function()
{ 
}
asc_docs_api.prototype.SlideTransitionPlay = function()
{
}

asc_docs_api.prototype.SetTextBoxInputMode = function(bIsEA)
{
}
asc_docs_api.prototype.GetTextBoxInputMode = function()
{
}
asc_docs_api.prototype.asc_setCoAuthoringEnable = function (isCoAuthoringEnable)
{
}

asc_docs_api.prototype.sync_EndAddShape = function()
{
}

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
        graphicObject.chart.themeColors.push( oNewColor );
    }
    return graphicObject;
}

asc_docs_api.prototype.asc_addChartDrawingObject = function(chartBinary)
{
    /**/

    // Приводим бинарик к объекту типа CChartAsGroup и добавляем объект
    if ( isObject(chartBinary) )
    {
        var binary = chartBinary["binary"];
        //if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Drawing_Props) )
        {
            History.Create_NewPoint();
            this.WordControl.m_oLogicDocument.addChart(binary);
        }
    }
}

asc_docs_api.prototype.asc_editChartDrawingObject = function(chartBinary)
{
    /**/

    // Находим выделенную диаграмму и накатываем бинарник
    if ( isObject(chartBinary) )
    {
        var binary = chartBinary["binary"];
        if ( false === this.WordControl.m_oLogicDocument.Document_Is_SelectionLocked(changestype_Drawing_Props) )
        {
            History.Create_NewPoint();
            this.WordControl.m_oLogicDocument.Edit_Chart(binary);
        }
    }
}

asc_docs_api.prototype.asc_getChartPreviews = function(chartType, chartSubType)
{
    if ( this.chartPreviewManager.isReady() ) {
        return this.chartPreviewManager.getChartPreviews(chartType, chartSubType);
    }
}

asc_docs_api.prototype.sync_closeChartEditor = function()
{
    this.asc_fireCallback("asc_onCloseChartEditor");
}

//-----------------------------------------------------------------
// События контекстного меню
//-----------------------------------------------------------------

function CContextMenuData()
{
    this.Type  = c_oAscContextMenuTypes.Main;
    this.X_abs = 0;
    this.Y_abs = 0;
    this.IsSlideSelect = true;
}

CContextMenuData.prototype.get_Type  = function()  { return this.Type; }
CContextMenuData.prototype.get_X = function()  { return this.X_abs; }
CContextMenuData.prototype.get_Y = function()  { return this.Y_abs; }
CContextMenuData.prototype.get_IsSlideSelect = function()  { return this.IsSlideSelect; }

asc_docs_api.prototype.sync_ContextMenuCallback = function(Data)
{
    this.asc_fireCallback("asc_onContextMenu", Data);
}

var cCharDelimiter = String.fromCharCode(5);

function _getFullImageSrc(src)
{
    if (0 == src.indexOf("theme"))
        return editor.ThemeLoader.ThemesUrl + src;
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