/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef XLSX_BIN_READER_WRITER_DEFINES
#define XLSX_BIN_READER_WRITER_DEFINES

namespace BinXlsxRW
{
    const double g_dKoef_mm_to_pt           = 72 / (2.54 * 10);
    const double g_dKoef_mm_to_twips        = 20 * g_dKoef_mm_to_pt;
    const double g_dKoef_mm_to_emu          = 36000;
    const double g_dKoef_mm_to_eightpoint   = 8 * g_dKoef_mm_to_pt;

    const static wchar_t* g_sFormatSignature = L"XLSY";
	const int g_nFormatVersion = 2;
	const int g_nFormatVersionNoBase64 = 10;
	extern int g_nCurFormatVersion;

	namespace c_oFileTypes{enum c_oFileTypes
	{
		XLSX	= 1,
                CSV	= 2,
                JSON	= 3,
                XLSB	= 4
	};} 


	namespace c_oSerConstants{enum c_oSerConstants
	{
		ErrorFormat =  -2,
		ErrorUnknown =  -1,
		ReadOk = 0,
		ReadUnknown = 1,
		ErrorStream = 0x55
	};}
	namespace c_oSerPropLenType{enum c_oSerPropLenType
	{
		Null		= 0,
		Byte		= 1,
		Short		= 2,
		Three		= 3,
		Long		= 4,
		Double		= 5,
		Variable	= 6,
		Double64	= 7,
		Long64		= 8
	};}
	namespace c_oSerTableTypes{enum c_oSerTableTypes
	{
		Other = 0,
		SharedStrings = 1,
		Styles = 2,
		Workbook = 3,
		Worksheets = 4,
		CalcChain = 5,
		App = 6,
		Core = 7,
		PersonList = 8,
		CustomProperties = 9,
		Customs = 10
	};}

	namespace c_oSerStylesTypes{enum c_oSerStylesTypes
	{
		Borders = 0,
		Border = 1,
		CellXfs = 2,
		Xfs = 3,
		Fills = 4,
		Fill = 5,
		Fonts = 6,
		Font = 7,
		NumFmts = 8,
		NumFmt = 9,
		Dxfs = 10,
		Dxf = 11,
		TableStyles = 12,
		CellStyleXfs = 14,
		CellStyles = 15,
		CellStyle = 16,
		SlicerStyles = 17,
		ExtDxfs = 18
	};}
	namespace c_oSerBorderTypes{enum c_oSerBorderTypes
	{
		Bottom = 0,
		Diagonal = 1,
		End = 2,
		Horizontal = 3,
		Start = 4,
		Top = 5,
		Vertical = 6,
		DiagonalDown = 7,
		DiagonalUp = 8,
		Outline = 9
	};}
	namespace c_oSerBorderPropTypes{enum c_oSerBorderPropTypes
	{
		Color = 0,
		Style = 1
	};}
	namespace c_oSerXfsTypes{enum c_oSerXfsTypes
	{
		ApplyAlignment = 0,
		ApplyBorder = 1,
		ApplyFill = 2,
		ApplyFont = 3,
		ApplyNumberFormat = 4,
		ApplyProtection = 5,
		BorderId = 6,
		FillId = 7,
		FontId = 8,
		NumFmtId = 9,
		PivotButton = 10,
		QuotePrefix = 11,
		XfId = 12,
		Aligment = 13,
		Protection = 14
	};}
	namespace c_oSerProtectionTypes {enum c_oSerProtectionTypes
	{
		Hidden = 0,
		Locked = 1
	};}	
	namespace c_oSerAligmentTypes{enum c_oSerAligmentTypes
	{
		Horizontal = 0,
		Indent = 1,
		JustifyLastLine = 2,
		ReadingOrder = 3,
		RelativeIndent = 4,
		ShrinkToFit = 5,
		TextRotation = 6,
		Vertical = 7,
		WrapText = 8
	};}
	namespace c_oSerFillTypes{enum c_oSerFillTypes
	{
		Pattern = 0,
		PatternBgColor_deprecated = 1,
		PatternType = 2,
		PatternFgColor = 3,
		PatternBgColor = 4,
		Gradient = 5,
		GradientType = 6,
		GradientLeft = 7,
		GradientTop = 8,
		GradientRight = 9,
		GradientBottom = 10,
		GradientDegree = 11,
		GradientStop = 12,
		GradientStopPosition = 13,
		GradientStopColor = 14
	};}
	namespace c_oSerFontTypes{enum c_oSerFontTypes
	{
		Bold = 0,
		Color = 1,
		Italic = 3,
		RFont = 4,
		Strike = 5,
		Sz = 6,
		Underline = 7,
		VertAlign = 8,
		Scheme = 9
	};}
	namespace c_oSerNumFmtTypes{enum c_oSerNumFmtTypes
	{
		FormatCode = 0,
		NumFmtId = 1,
		FormatCode16 = 2
	};}
	namespace c_oSerSharedStringTypes{enum c_oSerSharedStringTypes
	{
		Si = 0,
		Run = 1,
		RPr = 2,
		Text = 3,
	};}
	namespace c_oSerWorkbookTypes{enum c_oSerWorkbookTypes
	{
		WorkbookPr = 0,
		BookViews = 1,
		WorkbookView = 2,
		DefinedNames = 3,
		DefinedName = 4,
		ExternalReferences = 5,
		ExternalReference = 6,
		PivotCaches = 7,
		PivotCache = 8,
		ExternalBook = 9,
		OleLink = 10,
		DdeLink = 11,
		VbaProject = 12,
		JsaProject = 13,
		Comments = 14,
		CalcPr = 15,
		Connections = 16,
		AppName = 17,
		SlicerCaches = 18,
		SlicerCachesExt = 19,
		SlicerCache = 20,
		Protection = 21,
		OleSize = 22,
		ExternalFileKey = 23,
		ExternalInstanceId = 24
	};}
	namespace c_oSerWorkbookProtection {enum c_oSerWorkbookProtection{
		AlgorithmName = 0,
		SpinCount = 1,
		HashValue = 2,
		SaltValue = 3,
		LockStructure = 4,
		LockWindows = 5,
		Password = 6
	}; }
	namespace c_oSerDbPrTypes{enum c_oSerDbPrTypes
	{
		Connection = 0,
		Command = 1,
		CommandType = 2,
		ServerCommand = 3
	};}
	namespace c_oSerTextPrTypes{enum c_oSerTextPrTypes
	{
		CharacterSet = 0,
		SourceFile = 1,
		Decimal = 2,
		Delimiter = 3,
		Thousands = 4,
		FirstRow = 5,
		Qualifier = 6,
		FileType = 7,
		Prompt = 8,
		Delimited = 9,
		Tab = 10,
		Space = 11,
		Comma = 12,
		Semicolon = 13,
		Consecutive = 14,
		TextFields = 15,
		CodePage = 16,
		TextField = 17,
		TextFieldType = 18,
		TextFieldPosition = 19
	};}
	namespace c_oSerOlapPrTypes{enum c_oSerOlapPrTypes
	{
		LocalConnection = 0,
		RowDrillCount = 1,
		Local = 2,
		LocalRefresh = 3,
		SendLocale = 4,
		ServerNumberFormat = 5,
		ServerFont = 6,
		ServerFontColor = 7
	};}
	namespace c_oSerWebPrTypes{enum c_oSerWebPrTypes
	{
		Url = 0,
		Post = 1,
		EditPage = 2,
		Xml = 3,
		SourceData = 4,
		Consecutive = 5,
		FirstRow = 6,
		Xl97 = 7,
		TextDates = 8,
		Xl2000 = 9,
		HtmlTables = 10,
		HtmlFormat = 11,
		Tables = 12
	};}
	namespace c_oSerRangePrTypes{enum c_oSerRangePrTypes
	{
		SourceName = 0
	};}
	namespace c_oSerConnectionsTypes{enum c_oSerConnectionsTypes
	{
		Connection = 0,
		Type = 1,
		Name = 2,
		Id = 3,
		Credentials = 4,
		Background = 5,
		Deleted = 6,
		Description = 7,
		Interval = 8,
		KeepAlive = 9,
		MinRefreshableVersion = 10,
		New = 11,
		OdcFile = 12,
		OnlyUseConnectionFile = 13,
		ReconnectionMethod = 14,
		RefreshedVersion = 15,
		RefreshOnLoad = 16,
		SaveData = 17,
		SavePassword = 18,
		SingleSignOnId = 19,
		SourceFile = 20,
		DbPr = 21,
		OlapPr = 22,
		TextPr = 23,
		WebPr = 24,
		RangePr = 25,
		IdExt = 26,
		UId = 27
	};}
	namespace c_oSerWorkbookVbaProjectTypes{enum c_oSerWorkbookVbaProjectTypes
	{
		Name = 0
	};}
	namespace c_oSerWorkbookPrTypes{enum c_oSerWorkbookPrTypes
	{
		Date1904 = 0,
		DateCompatibility = 1,
		HidePivotFieldList = 2,
		ShowPivotChartFilter = 3
	};}
	namespace c_oSerWorkbookViewTypes{enum c_oSerWorkbookViewTypes
	{
		ActiveTab = 0
	};}
	namespace c_oSerDefinedNameTypes{enum c_oSerDefinedNameTypes
	{
		Name = 0,
		Ref = 1,
        LocalSheetId = 2,
        Hidden = 3,
		Comment = 4
	};}
	namespace c_oSerCalcPrTypes{enum c_oSerCalcPrTypes
	{
		CalcId = 0,
		CalcMode = 1,
		FullCalcOnLoad = 2,
		RefMode = 3,
		Iterate = 4,
		IterateCount = 5,
		IterateDelta = 6,
		FullPrecision = 7,
		CalcCompleted = 8,
		CalcOnSave = 9,
		ConcurrentCalc = 10,
		ConcurrentManualCount = 11,
		ForceFullCalc = 12
	};}
	namespace c_oSerWorksheetsTypes{enum c_oSerWorksheetsTypes
	{
		Worksheet = 0,
		WorksheetProp = 1,
		Cols = 2,
		Col = 3,
		Dimension = 4,
		Hyperlinks = 5,
		Hyperlink = 6,
		MergeCells = 7,
		MergeCell = 8,
		SheetData = 9,
		Row = 10,
		SheetFormatPr = 11,
		Drawings = 12,
		Drawing = 13,
		PageMargins = 14,
		PageSetup = 15,
		PrintOptions = 16,
		Autofilter = 17,
		TableParts = 18,
		Comments = 19,
		Comment = 20,
		ConditionalFormatting = 21,
		SheetViews = 22,
		SheetView = 23,
		SheetPr = 24,
		SparklineGroups = 25,
		PivotTable = 26,
		HeaderFooter = 27,
		LegacyDrawingHF = 28,
		Picture = 29,
		RowBreaks = 30,
		ColBreaks = 31,
		DataValidations = 32,
		QueryTable = 33,
		Controls = 34,
		XlsbPos = 35,
		SortState = 36,
		Slicers = 37,
		SlicersExt = 38,
		Slicer = 39,
		NamedSheetView = 40,
		Protection = 41,
		ProtectedRanges = 42,
		ProtectedRange = 43,
		CellWatches = 44,
		CellWatch = 45,
		CellWatchR = 46
	};}
	namespace c_oSerWorksheetProtection {enum c_oSerWorksheetPropTypes
	{
		AlgorithmName = 0,
		SpinCount = 1,
		HashValue = 2,
		SaltValue = 3,
		Password = 4,
		AutoFilter = 5,
		Content = 6,
		DeleteColumns = 7,
		DeleteRows = 8,
		FormatCells = 9,
		FormatColumns = 10,
		FormatRows = 11,
		InsertColumns = 12,
		InsertHyperlinks = 13,
		InsertRows = 14,
		Objects = 15,
		PivotTables = 16,
		Scenarios = 17,
		SelectLockedCells = 18,
		SelectUnlockedCell = 19,
		Sheet = 20,
		Sort = 21
	};}
	namespace c_oSerProtectedRangeTypes {enum c_oSerProtectedRangeTypes {
		AlgorithmName = 0,
		SpinCount = 1,
		HashValue = 2,
		SaltValue = 3,
		Name = 4,
		SqRef= 5,
		SecurityDescriptor = 6
	};}
	namespace c_oSerWorksheetPropTypes{enum c_oSerWorksheetPropTypes
	{
		Name = 0,
		SheetId = 1,
		State = 2
	};}
	namespace c_oSerWorksheetColTypes{enum c_oSerWorksheetColTypes
	{
		BestFit		= 0,
		Hidden		= 1,
		Max			= 2,
		Min			= 3,
		Style		= 4,
		Width		= 5,
		CustomWidth = 6,
		OutLevel	= 7,
		Collapsed	= 8
	};}
	namespace c_oSerHyperlinkTypes{enum c_oSerWorksheetColTypes
	{
		Ref			= 0,
		Hyperlink	= 1,
		Location	= 2,
		Tooltip		= 3,
		Display		= 4
	};}
	namespace c_oSerSheetFormatPrTypes{enum c_oSerSheetFormatPrTypes
	{
		DefaultColWidth		= 0,
		DefaultRowHeight	= 1,
		BaseColWidth		= 2,
		CustomHeight		= 3,
		ZeroHeight			= 4,
		OutlineLevelCol		= 5,
		OutlineLevelRow		= 6
	};}
	namespace c_oSerRowTypes{enum c_oSerRowTypes
	{
		Row		= 0,
		Style	= 1,
		Height	= 2,
		Hidden	= 3,
		Cells	= 4,
		Cell	= 5,
		CustomHeight = 6,
		OutLevel	= 7,
		Collapsed	= 8
	};}
	namespace c_oSerControlTypes{enum c_oSerControlTypes
	{
		Control = 10,
		ControlAnchor = 11,
		ObjectType = 12,
		Name = 13,
		AltText = 14,
		AutoFill = 15,
		AutoLine = 16,
		AutoPict = 17,
		DefaultSize = 18,
		Disabled = 19,
		Locked = 20,
		Macro = 21,
		RecalcAlways = 22,
		Checked = 23,
		Colored = 24,
		DropLines = 25,
		DropStyle = 26,
		Dx = 27,
		FirstButton = 28,
		FmlaGroup = 29,
		FmlaLink = 30,
		FmlaRange = 31,
		FmlaTxbx = 32,
		Horiz = 33,
		Inc = 34,
		JustLastX = 35,
		LockText = 36,
		Max = 37,
		Min = 38,
		MultiSel = 39,
		NoThreeD = 40,
		NoThreeD2 = 41,
		Page = 42,
		Sel = 43,
		SelType = 44,
		TextHAlign = 45,
		TextVAlign = 46,
		Val = 47,
		WidthMin = 48,
		EditVal = 49,
		MultiLine = 50,
		VerticalBar = 51,
		PasswordEdit = 52,
		Text = 53,
		Print = 54,
		ItemLst = 55,
		Item = 56
	};}
	namespace c_oSerCellTypes{enum c_oSerCellTypes
	{
		Ref = 0,
		Style = 1,
		Type = 2,
		Value = 3,
		Formula = 4,
		RefRowCol = 5,
		ValueText = 6,
		ValueCache = 7
	};}
	namespace c_oSerFormulaTypes{enum c_oSerFormulaTypes
	{
		Aca = 0,
		Bx = 1,
		Ca = 2,
		Del1 = 3,
		Del2 = 4,
		Dt2D = 5,
		Dtr = 6,
		R1 = 7,
		R2 = 8,
		Ref = 9,
		Si = 10,
		T = 11,
		Text = 12
	};}
	namespace c_oSer_DrawingType{enum c_oSer_DrawingType
	{
		Type = 0,
		From = 1,
		To = 2,
		Pos = 3,
		//Pic = 4,
		//PicSrc = 5,
		//GraphicFrame = 6,
		//Chart = 7,
		Ext = 8,
		pptxDrawing = 9,
		//Chart2 = 10,
		//ObjectName = 11
		EditAs = 12,
		ClientData = 14
	};}
	namespace c_oSer_DrawingFromToType{enum c_oSer_DrawingFromToType
	{
		Col = 0,
		ColOff = 1,
		Row = 2,
		RowOff = 3
	};}
	namespace c_oSer_DrawingPosType{enum c_oSer_DrawingPosType
	{
		X = 0,
		Y = 1
	};}
	namespace c_oSer_DrawingExtType{enum c_oSer_DrawingExtType
	{
		Cx = 0,
		Cy = 1
	};}
	namespace c_oSer_DrawingClientDataType {enum c_oSer_DrawingClientDataType
	{
		fLocksWithSheet = 0,
		fPrintsWithSheet = 1
	};}
	namespace c_oSer_OtherType{enum c_oSer_OtherType
	{
		Media = 0,
		MediaItem = 1,
		MediaId = 2,
		MediaSrc = 3,
		EmbeddedFonts = 4,
		Theme = 5
	};}
	namespace c_oSer_CalcChainType{enum c_oSer_CalcChainType
	{
		CalcChainItem = 0,
		Array = 1,
		SheetId = 2,
		DependencyLevel = 3,
		Ref = 4,
		ChildChain = 5,
		NewThread = 6
	};}
	namespace c_oSerCustoms {enum c_oSerCustoms
	{
		Custom = 0,
		ItemId = 1,
		Uri = 2,
		Content = 3
	};}
	namespace c_oSer_ChartType{enum c_oSer_ChartType
	{
		Legend = 0,
		Title = 1,
		PlotArea = 2,
		Style = 3,
		TitlePptx = 4,
		ShowBorder = 5,
		SpPr = 6
	};}
	namespace c_oSer_ChartTitlePptxType{enum c_oSer_ChartTitlePptxType
	{
		TxPptx = 0,
		TxPrPptx = 1
	};}
	namespace c_oSer_ChartLegendType{enum c_oSer_ChartLegendType
	{
		LegendPos = 0,
		Overlay = 1,
		Layout = 2,
		LegendEntry = 3,
		TxPrPptx = 4
	};}
	namespace c_oSer_ChartLegendEntryType{enum c_oSer_ChartLegendEntryType
	{
		Delete = 0,
		Index = 1,
		TxPrPptx = 2
	};}
	namespace c_oSer_ChartLegendLayoutType{enum c_oSer_ChartLegendLayoutType
	{
		H = 0,
		HMode = 1,
		LayoutTarget = 2,
		W = 3,
		WMode = 4,
		X = 5,
		XMode = 6,
		Y = 7,
		YMode = 8
	};}
	namespace c_oSer_ChartPlotAreaType{enum c_oSer_ChartPlotAreaType
	{
		CatAx = 0,
		ValAx = 1,
		SerAx = 2,
		ValAxPos = 3,
		BasicChart = 4
	};}
	namespace c_oSer_ChartCatAxType{enum c_oSer_ChartCatAxType
	{
		Title = 0,
		MajorGridlines = 1,
		Delete = 2,
		AxPos = 3,
		TitlePptx = 4,
		TxPrPptx = 5
	};}
	namespace c_oSer_BasicChartType{enum c_oSer_BasicChartType
	{
		Type = 0,
		BarDerection = 1,
		Grouping = 2,
		Overlap = 3,
		Series = 4,
		Seria = 5,
		DataLabels = 6,
	};}
	namespace c_oSer_ChartSeriesType{enum c_oSer_ChartSeriesType
	{
		Val = 0,
		Tx = 1,
		Marker = 2,
		OutlineColor = 3,
		xVal = 4,
		TxRef = 5,
		Index = 6,
		Order = 7,
		DataLabels = 8,
		SpPr = 9,
		Cat = 10
	};}
	namespace c_oSer_ChartSeriesMarkerType{enum c_oSer_ChartSeriesMarkerType
	{
		Size = 0,
		Symbol = 1
	};}
	namespace c_oSer_ChartSeriesDataLabelsType{enum c_oSer_ChartSeriesDataLabelsType
	{
		ShowVal = 0,
		TxPrPptx = 1,
		ShowCatName = 2
	};}
	namespace c_oSer_ChartSeriesNumCacheType{enum c_oSer_ChartSeriesNumCacheType
	{
		Formula = 0,
		NumCache = 1,
		NumCacheVal = 2,
		NumCacheIndex = 3,
		NumCache2 = 4,
		NumCacheItem = 5
	};}
	namespace c_oSer_ColorObjectType{enum c_oSer_ColorObjectType
	{
		Rgb = 0,
		Type = 1,
		Theme = 2,
		Tint = 3
	};}
	namespace c_oSer_ColorType{enum c_oSer_ColorType
	{
		Auto = 0
	};}
	namespace c_oSer_PageMargins{enum c_oSer_PageMargins
	{
		Left = 0,
		Top = 1,
		Right = 2,
		Bottom = 3,
		Header = 4,
		Footer = 5
	};}
	namespace c_oSer_PageSetup{enum c_oSer_PageSetup
	{
		Orientation = 0,
		PaperSize = 1,
		BlackAndWhite = 2,
		CellComments = 3,
		Copies = 4,
		Draft = 5,
		Errors = 6,
		FirstPageNumber = 7,
		FitToHeight = 8,
		FitToWidth = 9,
		HorizontalDpi = 10,
		PageOrder = 11,
		PaperHeight = 12,
		PaperWidth = 13,
		PaperUnits = 14,
		Scale = 15,
		UseFirstPageNumber = 16,
		UsePrinterDefaults = 17,
		VerticalDpi = 18
	};}
	namespace c_oSer_PrintOptions{enum c_oSer_PrintOptions
	{
		GridLines = 0,
		Headings = 1
	};}
	namespace c_oSer_QueryTable{enum c_oSer_QueryTable
	{
		AdjustColumnWidth = 0,
		ApplyAlignmentFormats = 1,
		ApplyBorderFormats = 2,
		ApplyFontFormats = 3,
		ApplyNumberFormats = 4,
		ApplyPatternFormats = 5,
		ApplyWidthHeightFormats = 6,
		BackgroundRefresh = 7,			
		AutoFormatId = 8,
		ConnectionId = 9,			
		DisableEdit = 10,
		DisableRefresh = 11,
		FillFormulas = 12,
		FirstBackgroundRefresh = 13,
		GrowShrinkType = 14,
		Headers = 15,
		Intermediate = 16,
		Name = 17,
		PreserveFormatting = 18,
		RefreshOnLoad = 19,
		RemoveDataOnSave = 20,
		RowNumbers = 21,
		QueryTableRefresh = 22
	};}
	namespace c_oSer_QueryTableRefresh{enum c_oSer_QueryTableRefresh
	{
		NextId = 0,
		MinimumVersion = 1,			
		FieldIdWrapped = 2,
		HeadersInLastRefresh = 3,
		PreserveSortFilterLayout = 4,
		UnboundColumnsLeft = 5,
		UnboundColumnsRight = 6,			
		QueryTableFields = 7,
		QueryTableDeletedFields = 8,
		SortState = 9
	};}
	namespace c_oSer_QueryTableField{enum c_oSer_QueryTableField
	{
		QueryTableField = 0,			
		Id = 1,
		TableColumnId = 2,
		Name = 3,			
		RowNumbers = 4,
		FillFormulas = 5,
		DataBound = 6,
		Clipped = 7
	};}
	namespace c_oSer_QueryTableDeletedField{enum c_oSer_QueryTableDeletedField
	{
		QueryTableDeletedField = 0,			
		Name = 1
	};}
	namespace c_oSer_TablePart{enum c_oSer_TablePart
	{
		Table = 0,
		Ref = 1,
		TotalsRowCount = 2,
		DisplayName = 3,
		AutoFilter = 4,
		SortState = 5,
		TableColumns = 6,
		TableStyleInfo = 7,
		HeaderRowCount = 8,
		AltTextTable = 9,
		Name = 10,
		Comment = 11,
		ConnectionId = 12,
		Id = 13,
		DataCellStyle = 14,
		DataDxfId = 15,
		HeaderRowBorderDxfId = 16,
		HeaderRowCellStyle = 17,
		HeaderRowDxfId = 18,
		InsertRow = 19,
		InsertRowShift = 20,
		Published = 21,
		TableBorderDxfId = 22,
		TableType = 23,
		TotalsRowBorderDxfId = 24,
		TotalsRowCellStyle = 25,
		TotalsRowDxfId = 26,
		TotalsRowShown = 27,
		QueryTable =28
	};}
	namespace c_oSer_TableStyleInfo{enum c_oSer_TableStyleInfo
	{
		Name = 0,
		ShowColumnStripes = 1,
		ShowRowStripes = 2,
		ShowFirstColumn = 3,
		ShowLastColumn = 4
	};}
	namespace c_oSer_TableColumns{enum c_oSer_TableColumns
	{
		TableColumn = 0,
		Name = 1,
		DataDxfId = 2,
		TotalsRowLabel = 3,
		TotalsRowFunction = 4,
		TotalsRowFormula = 5,
		CalculatedColumnFormula = 6,
		DataCellStyle = 7,
		HeaderRowCellStyle = 8,
		HeaderRowDxfId = 9,
		Id = 10,
		QueryTableFieldId = 11,
		TotalsRowCellStyle = 12,
		TotalsRowDxfId = 13,
		UniqueName = 14
	};}
	namespace c_oSer_SortState{enum c_oSer_SortState
	{
		Ref = 0,
		CaseSensitive = 1,
		SortConditions = 2,
		SortCondition = 3,
		ConditionRef = 4,
		ConditionSortBy = 5,
		ConditionDescending = 6,
		ConditionDxfId = 7,
		ColumnSort = 8,
		SortMethod = 9
	};}
	namespace c_oSer_AutoFilter{enum c_oSer_AutoFilter
	{
		Ref = 0,
		FilterColumns = 1,
		FilterColumn = 2,
		SortState = 3
	};}
	namespace c_oSer_FilterColumn{enum c_oSer_FilterColumn
	{
		ColId = 0,
		Filters = 1,
		Filter = 2,
		DateGroupItem = 3,
		CustomFilters = 4,
		ColorFilter = 5,
		Top10 = 6,
		DynamicFilter = 7,
		HiddenButton = 8,
		ShowButton = 9,
		FiltersBlank = 10
	};}
	namespace c_oSer_Filter{enum c_oSer_Filter
	{
		Val = 0
	};}
	namespace c_oSer_DateGroupItem{enum c_oSer_DateGroupItem
	{
		DateTimeGrouping = 0,
		Day = 1,
		Hour = 2,
		Minute = 3,
		Month = 4,
		Second = 5,
		Year = 6
	};}
	namespace c_oSer_CustomFilters{enum c_oSer_CustomFilters
	{
		And = 0,
		CustomFilters = 1,
		CustomFilter = 2,
		Operator = 3,
		Val = 4
	};}
	namespace c_oSer_DynamicFilter{enum c_oSer_DynamicFilter
	{
		Type = 0,
		Val = 1,
		MaxVal = 2
	};}
	namespace c_oSer_ColorFilter{enum c_oSer_ColorFilter
	{
		CellColor = 0,
		DxfId = 1
	};}
	namespace c_oSer_Top10{enum c_oSer_Top10
	{
		FilterVal = 0,
		Percent = 1,
		Top = 2,
		Val = 3
	};}
	namespace c_oSer_Dxf{enum c_oSer_Dxf
	{
		Alignment = 0,
		Border = 1,
		Fill = 2,
		Font = 3,
		NumFmt = 4
	};}
	namespace c_oSer_TableStyles{enum c_oSer_TableStyles
	{
		DefaultTableStyle = 0,
		DefaultPivotStyle = 1,
		TableStyles = 2,
		TableStyle = 3
	};}
	namespace c_oSer_TableStyle{enum c_oSer_TableStyle
	{
		Name = 0,
		Pivot = 1,
		Table = 2,
		Elements = 3,
		Element = 4,
		DisplayName = 5
	};}
	namespace c_oSer_TableStyleElement{enum c_oSer_TableStyleElement
	{
		DxfId = 0,
		Size = 1,
		Type = 2
	};}
	namespace c_oSer_Comments{enum c_oSer_Comments
	{
		Row = 0,
		Col = 1,
		CommentDatas = 2,
		CommentData = 3,
		Left = 4,
		LeftOffset = 5,
		Top = 6,
		TopOffset = 7,
		Right = 8,
		RightOffset = 9,
		Bottom = 10,
		BottomOffset = 11,
		LeftMM = 12,
		TopMM = 13,
		WidthMM = 14,
		HeightMM = 15,
		MoveWithCells = 16,
		SizeWithCells = 17,
		ThreadedComment = 18
	};}
	namespace c_oSer_CommentData{enum c_oSer_CommentData
	{
		Text = 0,
		Time = 1,
		UserId = 2,
		UserName = 3,
		QuoteText = 4,
		Solved = 5,
		Document = 6,
		Replies = 7,
		Reply = 8,
		OOTime = 9,
		Guid = 10,
		UserData = 11
	};}
	namespace c_oSer_ThreadedComment{enum c_oSer_ThreadedComment
	{
		dT = 0,
		personId = 1,
		id = 2,
		done = 3,
		text = 4,
		mention = 5,
		reply = 6,
		mentionpersonId = 7,
		mentionId = 8,
		startIndex = 9,
		length = 10
	};}
	namespace c_oSer_Person{enum c_oSer_Person
	{
		person = 0,
		id = 1,
		providerId = 2,
		userId = 3,
		displayName = 4
	};}
	namespace c_oSer_ConditionalFormatting{enum c_oSer_ConditionalFormatting
	{
		Pivot = 0,
		SqRef = 1,
		ConditionalFormattingRule = 2
	};}
	namespace c_oSer_ConditionalFormattingRule{enum c_oSer_ConditionalFormattingRule
	{
		AboveAverage = 0,
		Bottom = 1,
		DxfId = 2,
		EqualAverage = 3,
		Operator = 4,
		Percent = 5,
		Priority = 6,
		Rank = 7,
		StdDev = 8,
		StopIfTrue = 9,
		Text = 10,
		strTimePeriod = 11,
		Type = 12,
		TimePeriod = 13,

		ColorScale = 14,
		DataBar = 15,
		FormulaCF = 16,
		IconSet = 17,
		Dxf = 18,
		IsExt = 19
	};}
	namespace c_oSer_ConditionalFormattingRuleColorScale{enum c_oSer_ConditionalFormattingRuleElement
	{
		CFVO = 0,
		Color = 1,
	};}
	namespace c_oSer_ConditionalFormattingDataBar{enum c_oSer_ConditionalFormattingDataBar
	{
		CFVO = 0,
		Color = 1,
		MaxLength = 2,
		MinLength = 3,
		ShowValue = 4,
		NegativeColor = 5,
		BorderColor = 6,
		AxisColor = 7,
		NegativeBorderColor = 8,
		AxisPosition = 9,
		Direction = 10,
		GradientEnabled = 11,
		NegativeBarColorSameAsPositive = 12,
		NegativeBarBorderColorSameAsPositive = 13
	};}
	namespace c_oSer_ConditionalFormattingIconSet{enum c_oSer_ConditionalFormattingIconSet
	{
		CFVO = 0,
		IconSet = 1,
		Percent = 2,
		Reverse = 3,
		ShowValue = 4,
		CFIcon = 5
	};}
	namespace c_oSer_ConditionalFormattingValueObject{enum c_oSer_ConditionalFormattingValueObject
	{
		Gte = 0,
		Type = 1,
		Val = 2,
		Formula = 3
	};}
	namespace c_oSer_ConditionalFormattingIcon{enum c_oSer_ConditionalFormattingIcon
	{
		iconSet = 0,
		iconId = 1
	};}
	namespace c_oSer_DataValidation{enum c_oSer_DataValidation
	{
		DataValidations = 0,
		DataValidation = 1,
		DisablePrompts = 2,
		XWindow = 3,
		YWindow = 4,
		Type = 5,
		AllowBlank = 6,
		Error = 7,
		ErrorTitle = 8,
		ErrorStyle = 9,
		ImeMode = 10,
		Operator = 11,
		Promt = 12,
		PromptTitle = 13,
		ShowDropDown = 14,
		ShowErrorMessage = 15,
		ShowInputMessage = 16,
		SqRef = 17,
		Formula1 = 18,
		Formula2 = 19
	};}
	namespace c_oSer_SheetView{enum c_oSer_SheetView
	{
		ColorId = 0,
		DefaultGridColor = 1,
		RightToLeft = 2,
		ShowFormulas = 3,
		ShowGridLines = 4,
		ShowOutlineSymbols = 5,
		ShowRowColHeaders = 6,
		ShowRuler = 7,
		ShowWhiteSpace = 8,
		ShowZeros = 9,
		TabSelected = 10,
		TopLeftCell = 11,
		View = 12,
		WindowProtection = 13,
		WorkbookViewId = 14,
		ZoomScale = 15,
		ZoomScaleNormal = 16,
		ZoomScalePageLayoutView = 17,
		ZoomScaleSheetLayoutView = 18,

		Pane = 19,
		Selection = 20
	};}
	namespace c_oSer_Pane{enum c_oSer_Pane
	{
		ActivePane = 0,
		State = 1,
		TopLeftCell = 2,
		XSplit = 3,
		YSplit = 4
	};}
	namespace c_oSer_Selection{enum c_oSer_Selection
	{
		ActiveCell = 0,
		ActiveCellId = 1,
		Sqref = 2,
		Pane = 3
	};}
	namespace c_oSer_CellStyle{enum c_oSer_CellStyle
	{
		BuiltinId = 0,
		CustomBuiltin = 1,
		Hidden = 2,
		ILevel = 3,
		Name = 4,
		XfId = 5
	};}
	namespace c_oSer_SheetPr{enum c_oSer_SheetPr
	{
		CodeName = 0,
		EnableFormatConditionsCalculation = 1,
		FilterMode = 2,
		Published = 3,
		SyncHorizontal = 4,
		SyncRef = 5,
		SyncVertical = 6,
		TransitionEntry = 7,
		TransitionEvaluation = 8,

		TabColor = 9,
		PageSetUpPr = 10,
		AutoPageBreaks = 11,
		FitToPage = 12,
		OutlinePr = 13,
		ApplyStyles = 14,
		ShowOutlineSymbols = 15,
		SummaryBelow = 16,
		SummaryRight = 17
	};}

    namespace c_oSer_Sparkline{enum c_oSer_Sparkline
    {
        SparklineGroup = 0,
        ManualMax = 1,
        ManualMin = 2,
        LineWeight = 3,
        Type = 4,
        DateAxis = 5,
        DisplayEmptyCellsAs = 6,
        Markers = 7,
        High = 8,
        Low = 9,
        First = 10,
        Last = 11,
        Negative = 12,
        DisplayXAxis = 13,
        DisplayHidden = 14,
        MinAxisType = 15,
        MaxAxisType = 16,
        RightToLeft = 17,
        ColorSeries = 18,
        ColorNegative = 19,
        ColorAxis = 20,
        ColorMarkers = 21,
        ColorFirst = 22,
        ColorLast = 23,
        ColorHigh = 24,
        ColorLow = 25,
        Ref = 26,
        Sparklines = 27,
        Sparkline = 28,
        SparklineRef = 29,
        SparklineSqRef = 30
    };}
	namespace c_oSer_AltTextTable{enum c_oSer_AltTextTable
	{
		AltText = 0,
		AltTextSummary = 1
	};}
	namespace c_oSer_PivotTypes{enum c_oSerPivotTypes
	{
		id = 0,
		cache = 1,
		record = 2,
		cacheId = 3,
		table = 4
	};}
	namespace c_oSer_ExternalLinkTypes{enum c_oSer_ExternalLinkTypes
	{
		Id = 0,
		SheetNames = 1,
		SheetName = 2,
		DefinedNames = 3,
		DefinedName = 4,
		DefinedNameName = 5,
		DefinedNameRefersTo = 6,
		DefinedNameSheetId = 7,
		SheetDataSet = 8,
		SheetData = 9,
		SheetDataSheetId = 10,
		SheetDataRefreshError = 11,
		SheetDataRow = 12,
		SheetDataRowR = 13,
		SheetDataRowCell = 14,
		SheetDataRowCellRef = 15,
		SheetDataRowCellType = 16,
		SheetDataRowCellValue = 17
	};}
	namespace c_oSer_OleLinkTypes{enum c_oSer_OleLinkTypes
	{
		Id = 0,
		ProgId = 1,
		OleItem = 2,
		Name = 3,
		Icon = 4,
		Advise = 5,
		PreferPic = 6
	};}
	namespace c_oSer_DdeLinkTypes{enum c_oSer_DdeLinkTypes
	{
		DdeService = 0,
		DdeTopic = 1,
		DdeItem = 2,
		Name = 3,
		Ole = 4,
		Advise = 5,
		PreferPic = 6,
		DdeValues = 7,
		DdeValuesRows = 8,
		DdeValuesCols = 9,
		DdeValue = 10,
		DdeValueType = 11,
		DdeValueVal = 12
	};}
	namespace c_oSer_HeaderFooter{enum c_oSer_HeaderFooter
	{
		AlignWithMargins = 0,
		DifferentFirst = 1,
		DifferentOddEven = 2,
		ScaleWithDoc = 3,
		EvenFooter = 4,
		EvenHeader = 5,
		FirstFooter = 6,
		FirstHeader = 7,
		OddFooter = 8,
		OddHeader = 9
	};}
	namespace c_oSer_RowColBreaks{enum c_oSer_RowColBreaks
	{
		Count = 0,
		ManualBreakCount = 1,
		Break = 2,
		Id = 3,
		Man = 4,
		Max = 5,
		Min = 6,
		Pt = 7
	};}
	namespace c_oSer_LegacyDrawingHF{enum c_oSer_LegacyDrawingHF
	{
		Drawings = 0,
		Drawing = 1,
		DrawingId = 2,
		DrawingShape = 3,
		Cfe = 4,
		Cff = 5,
		Cfo = 6,
		Che = 7,
		Chf = 8,
		Cho = 9,
		Lfe = 10,
		Lff = 11,
		Lfo = 12,
		Lhe = 13,
		Lhf = 14,
		Lho = 15,
		Rfe = 16,
		Rff = 17,
		Rfo = 18,
		Rhe = 19,
		Rhf = 20,
		Rho = 21
	};}
}

#endif //
