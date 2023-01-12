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
#ifndef _BUILD_EDITOR_DEFINES_CROSSPLATFORM_H_
#define _BUILD_EDITOR_DEFINES_CROSSPLATFORM_H_

#define g_bDate1904 = false;

#define CellValueType_Number                                0
#define CellValueType_String                                1
#define CellValueType_Bool                                  2
#define CellValueType_Error                                 3

//NumFormat defines
#define	c_oAscNumFormatType_General                         0
#define c_oAscNumFormatType_Custom                          1
#define c_oAscNumFormatType_Text                            2
#define c_oAscNumFormatType_Number                          3
#define c_oAscNumFormatType_Integer                         4
#define c_oAscNumFormatType_Scientific                      5
#define c_oAscNumFormatType_Currency                        6
#define c_oAscNumFormatType_Date                            7
#define c_oAscNumFormatType_Time                            8
#define c_oAscNumFormatType_Percent                         9
#define c_oAscNumFormatType_Fraction                        10
	 
#define c_oAscDrawingLayerType_BringToFront                 0
#define c_oAscDrawingLayerType_BringForward                 1
#define c_oAscDrawingLayerType_SendToBack                   2
#define c_oAscDrawingLayerType_SendBackward                 3

#define c_oAscTransactionState_No                           -1
#define c_oAscTransactionState_Start                        0
#define c_oAscTransactionState_Stop                         1
							
#define c_oAscCellAnchorType_cellanchorAbsolute             0
#define c_oAscCellAnchorType_cellanchorOneCell              1
#define c_oAscCellAnchorType_cellanchorTwoCell              2

#define c_oAscChartDefines_defaultChartWidth                478
#define c_oAscChartDefines_defaultChartHeight               286
	
#define c_oAscLineDrawingRule_Left                          0
#define c_oAscLineDrawingRule_Center                        1
#define c_oAscLineDrawingRule_Right                         2
#define c_oAscLineDrawingRule_Top                           0
#define c_oAscLineDrawingRule_Bottom                        2
	
#define align_Right                                         0
#define align_Left                                          1
#define align_Center                                        2
#define align_Justify                                       3

#define linerule_AtLeast                                    0
#define linerule_Auto                                       1
#define linerule_Exact                                      2

#define shd_Clear                                           0
#define shd_Nil                                             1

#define vertalign_Baseline                                  0
#define vertalign_SuperScript                               1
#define vertalign_SubScript                                 2
#define hdrftr_Header                                       0x01
#define hdrftr_Footer                                       0x02

#define c_oAscChartTitleShowSettings_none                   0
#define c_oAscChartTitleShowSettings_overlay                1
#define c_oAscChartTitleShowSettings_noOverlay              2

#define c_oAscChartHorAxisLabelShowSettings_none            0
#define c_oAscChartHorAxisLabelShowSettings_noOverlay       1

#define c_oAscChartVertAxisLabelShowSettings_none			0
#define c_oAscChartVertAxisLabelShowSettings_rotated		1
#define c_oAscChartVertAxisLabelShowSettings_vertical		2
#define c_oAscChartVertAxisLabelShowSettings_horizontal		3

#define c_oAscChartLegendShowSettings_none                  0
#define c_oAscChartLegendShowSettings_left                  1
#define c_oAscChartLegendShowSettings_top                   2
#define c_oAscChartLegendShowSettings_right                 3
#define c_oAscChartLegendShowSettings_bottom                4
#define c_oAscChartLegendShowSettings_leftOverlay           5
#define c_oAscChartLegendShowSettings_rightOverlay          6
#define c_oAscChartLegendShowSettings_layout                7

#define c_oAscChartDataLabelsPos_none                       0
#define c_oAscChartDataLabelsPos_b                          1
#define c_oAscChartDataLabelsPos_bestFit                    2
#define c_oAscChartDataLabelsPos_ctr                        3
#define c_oAscChartDataLabelsPos_inBase                     4
#define c_oAscChartDataLabelsPos_inEnd                      5
#define c_oAscChartDataLabelsPos_l                          6
#define c_oAscChartDataLabelsPos_outEnd                     7
#define c_oAscChartDataLabelsPos_r                          8
#define c_oAscChartDataLabelsPos_t                          9

#define c_oAscChartCatAxisSettings_none                     0
#define c_oAscChartCatAxisSettings_leftToRight              1
#define c_oAscChartCatAxisSettings_rightToLeft              2
#define c_oAscChartCatAxisSettings_noLabels                 3

#define c_oAscChartValAxisSettings_none                     0
#define c_oAscChartValAxisSettings_byDefault                1
#define c_oAscChartValAxisSettings_thousands                2
#define c_oAscChartValAxisSettings_millions                 3
#define c_oAscChartValAxisSettings_billions                 4
#define c_oAscChartValAxisSettings_log                      5

#define c_oAscAxisTypeSettings_vert                         0
#define c_oAscAxisTypeSettings_hor                          1

#define c_oAscGridLinesSettings_none                        0
#define c_oAscGridLinesSettings_major                       1
#define c_oAscGridLinesSettings_minor                       2
#define c_oAscGridLinesSettings_majorMinor                  3

#define c_oAscChartTypeSettings_barNormal                   0
#define c_oAscChartTypeSettings_barStacked                  1
#define c_oAscChartTypeSettings_barStackedPer               2
#define c_oAscChartTypeSettings_barNormal3d                 3
#define c_oAscChartTypeSettings_barStacked3d                4
#define c_oAscChartTypeSettings_barStackedPer3d             5
#define c_oAscChartTypeSettings_barNormal3dPerspective      6
#define c_oAscChartTypeSettings_lineNormal                  7
#define c_oAscChartTypeSettings_lineStacked                 8
#define c_oAscChartTypeSettings_lineStackedPer              9
#define c_oAscChartTypeSettings_lineNormalMarker            10
#define c_oAscChartTypeSettings_lineStackedMarker           11
#define c_oAscChartTypeSettings_lineStackedPerMarker        12
#define c_oAscChartTypeSettings_line3d                      13
#define c_oAscChartTypeSettings_pie                         14
#define c_oAscChartTypeSettings_pie3d                       15
#define c_oAscChartTypeSettings_hBarNormal                  16
#define c_oAscChartTypeSettings_hBarStacked                 17
#define c_oAscChartTypeSettings_hBarStackedPer              18
#define c_oAscChartTypeSettings_hBarNormal3d                19
#define c_oAscChartTypeSettings_hBarStacked3d               20
#define c_oAscChartTypeSettings_hBarStackedPer3d            21
#define c_oAscChartTypeSettings_areaNormal                  22
#define c_oAscChartTypeSettings_areaStacked                 23
#define c_oAscChartTypeSettings_areaStackedPer              24
#define c_oAscChartTypeSettings_doughnut                    25
#define c_oAscChartTypeSettings_stock                       26
#define c_oAscChartTypeSettings_scatter                     27
#define c_oAscChartTypeSettings_scatterLine                 28
#define c_oAscChartTypeSettings_scatterLineMarker           29
#define c_oAscChartTypeSettings_scatterMarker               30
#define c_oAscChartTypeSettings_scatterNone                 31
#define c_oAscChartTypeSettings_scatterSmooth               32
#define c_oAscChartTypeSettings_scatterSmoothMarker         33
#define c_oAscChartTypeSettings_unknown                     34

#define c_oAscValAxisRule_auto                              0
#define c_oAscValAxisRule_fixed                             1

#define c_oAscValAxUnits_none                               0
#define c_oAscValAxUnits_BILLIONS                           1
#define c_oAscValAxUnits_HUNDRED_MILLIONS                   2
#define c_oAscValAxUnits_HUNDREDS                           3
#define c_oAscValAxUnits_HUNDRED_THOUSANDS                  4
#define c_oAscValAxUnits_MILLIONS                           5
#define c_oAscValAxUnits_TEN_MILLIONS                       6
#define c_oAscValAxUnits_TEN_THOUSANDS                      7
#define c_oAscValAxUnits_TRILLIONS                          8
#define c_oAscValAxUnits_CUSTOM                             9
#define c_oAscValAxUnits_THOUSANDS                          10

#define c_oAscTickMark_TICK_MARK_CROSS                      0
#define c_oAscTickMark_TICK_MARK_IN                         1
#define c_oAscTickMark_TICK_MARK_NONE                       2
#define c_oAscTickMark_TICK_MARK_OUT                        3

#define c_oAscTickLabelsPos_TICK_LABEL_POSITION_HIGH		0
#define c_oAscTickLabelsPos_TICK_LABEL_POSITION_LOW			1
#define c_oAscTickLabelsPos_TICK_LABEL_POSITION_NEXT_TO		2
#define c_oAscTickLabelsPos_TICK_LABEL_POSITION_NONE		3

#define c_oAscCrossesRule_auto                              0
#define c_oAscCrossesRule_maxValue                          1
#define c_oAscCrossesRule_value                             2
#define c_oAscCrossesRule_minValue                          3

#define c_oAscHorAxisType_auto                              0
#define c_oAscHorAxisType_date                              1
#define c_oAscHorAxisType_text                              2

#define c_oAscBetweenLabelsRule_auto                        0
#define c_oAscBetweenLabelsRule_manual                      1

#define c_oAscLabelsPosition_byDivisions                    0
#define c_oAscLabelsPosition_betweenDivisions               1

#define c_oAscAxisType_auto                                 0
#define c_oAscAxisType_date                                 1
#define c_oAscAxisType_text                                 2
#define c_oAscAxisType_cat                                  3
#define c_oAscAxisType_val                                  4

#define c_oAscHAnchor_Margin                                0x00
#define c_oAscHAnchor_Page                                  0x01
#define c_oAscHAnchor_Text                                  0x02
#define c_oAscHAnchor_PageInternal                          0xFF // только для внутреннего использования

#define c_oAscXAlign_Center                                 0x00
#define c_oAscXAlign_Inside                                 0x01
#define c_oAscXAlign_Left                                   0x02
#define c_oAscXAlign_Outside                                0x03
#define c_oAscXAlign_Right                                  0x04

#define c_oAscYAlign_Bottom                                 0x00
#define c_oAscYAlign_Center                                 0x01
#define c_oAscYAlign_Inline                                 0x02
#define c_oAscYAlign_Inside                                 0x03
#define c_oAscYAlign_Outside                                0x04
#define c_oAscYAlign_Top                                    0x05

#define c_oAscVAnchor_Margin                                0x00
#define c_oAscVAnchor_Page                                  0x01
#define c_oAscVAnchor_Text                                  0x02

#define c_oAscRelativeFromH_Character                       0x00
#define c_oAscRelativeFromH_Column                          0x01
#define c_oAscRelativeFromH_InsideMargin                    0x02
#define c_oAscRelativeFromH_LeftMargin                      0x03
#define c_oAscRelativeFromH_Margin                          0x04
#define c_oAscRelativeFromH_OutsideMargin                   0x05
#define c_oAscRelativeFromH_Page                            0x06
#define c_oAscRelativeFromH_RightMargin                     0x07

#define c_oAscRelativeFromV_BottomMargin                    0x00
#define c_oAscRelativeFromV_InsideMargin                    0x01
#define c_oAscRelativeFromV_Line                            0x02
#define c_oAscRelativeFromV_Margin                          0x03
#define c_oAscRelativeFromV_OutsideMargin                   0x04
#define c_oAscRelativeFromV_Page                            0x05
#define c_oAscRelativeFromV_Paragraph                       0x06
#define c_oAscRelativeFromV_TopMargin                       0x07

// image wrap style
#define c_oAscWrapStyle_Inline			0
#define c_oAscWrapStyle_Flow			1

// math
#define c_oAscLimLoc_SubSup				0x00
#define c_oAscLimLoc_UndOvr				0x01

#define c_oAscMathJc_Center				0x00
#define c_oAscMathJc_CenterGroup		0x01
#define c_oAscMathJc_Left				0x02
#define c_oAscMathJc_Right				0x03

#define c_oAscTopBot_Bot				0x00
#define c_oAscTopBot_Top				0x01

#define c_oAscScript_DoubleStruck		0x00
#define c_oAscScript_Fraktur			0x01
#define c_oAscScript_Monospace			0x02
#define c_oAscScript_Roman				0x03
#define c_oAscScript_SansSerif			0x04
#define c_oAscScript_Script				0x05

#define c_oAscShp_Centered				0x00
#define c_oAscShp_Match					0x01

#define c_oAscSty_Bold					0x00
#define c_oAscSty_BoldItalic			0x01
#define c_oAscSty_Italic				0x02
#define c_oAscSty_Plain					0x03

#define c_oAscFType_Bar					0x00
#define c_oAscFType_Lin					0x01
#define c_oAscFType_NoBar				0x02
#define c_oAscFType_Skw					0x03

#define c_oAscBrkBin_After				0x00
#define c_oAscBrkBin_Before				0x01
#define c_oAscBrkBin_Repeat				0x02

#define c_oAscBrkBinSub_PlusMinus		0x00
#define c_oAscBrkBinSub_MinusPlus		0x01
#define c_oAscBrkBinSub_MinusMinus		0x02

// Толщина бордера
#define c_oAscBorderWidth_None			0	// 0px
#define c_oAscBorderWidth_Thin			1	// 1px
#define c_oAscBorderWidth_Medium		2	// 2px
#define c_oAscBorderWidth_Thick			3	// 3px

// Располагаются в порядке значимости для отрисовки
#define c_oAscBorderStyles_None					0
#define c_oAscBorderStyles_Double				1
#define c_oAscBorderStyles_Hair					2
#define c_oAscBorderStyles_DashDotDot			3
#define c_oAscBorderStyles_DashDot				4
#define c_oAscBorderStyles_Dotted				5
#define c_oAscBorderStyles_Dashed				6
#define c_oAscBorderStyles_Thin					7
#define c_oAscBorderStyles_MediumDashDotDot		8
#define c_oAscBorderStyles_SlantDashDot			9
#define c_oAscBorderStyles_MediumDashDot		10
#define c_oAscBorderStyles_MediumDashed			11
#define c_oAscBorderStyles_Medium				12
#define c_oAscBorderStyles_Thick				13

// PageOrientation
#define c_oAscPageOrientation_PagePortrait		1
#define c_oAscPageOrientation_PageLandscape		2

/**
 * lock types
 * @const
 */
#define c_oAscLockTypes_kLockTypeNone			1	// никто не залочил данный объект
#define c_oAscLockTypes_kLockTypeMine			2	// данный объект залочен текущим пользователем
#define c_oAscLockTypes_kLockTypeOther			3	// данный объект залочен другим(не текущим) пользователем
#define c_oAscLockTypes_kLockTypeOther2			4	// данный объект залочен другим(не текущим) пользователем (обновления уже пришли)
#define c_oAscLockTypes_kLockTypeOther3			5	// данный объект был залочен (обновления пришли) и снова стал залочен

#define c_oAscFormatPainterState_kOff			0
#define c_oAscFormatPainterState_kOn			1
#define c_oAscFormatPainterState_kMultiple		2


#define c_oAscZoomType_Current		0
#define c_oAscZoomType_FitWidth		1
#define c_oAscZoomType_FitPage		2

#define c_oAscLockLockTypeNone                      1 // никто не залочил данный объект
#define c_oAscLockLockTypeMine                      2 // данный объект залочен текущим пользователем
#define c_oAscLockLockTypeOther                     3 // данный объект залочен другим(не текущим) пользователем
#define c_oAscLockLockTypeOther2                    4 // данный объект залочен другим(не текущим) пользователем (обновления уже пришли)
#define c_oAscLockLockTypeOther3                    5 // данный объект был залочен (обновления пришли) и снова стал залочен

#define c_oAscAsyncActionType_Information			0
#define c_oAscAsyncActionType_BlockInteraction		1

#define c_oAscAsyncAction_Open					0	// открытие документа
#define c_oAscAsyncAction_Save					1
#define c_oAscAsyncAction_LoadDocumentFonts		2	// загружаем фонты документа (сразу после открытия)
#define c_oAscAsyncAction_LoadDocumentImages	3	// загружаем картинки документа (сразу после загрузки шрифтов)
#define c_oAscAsyncAction_ LoadFont				4	// подгрузка нужного шрифта
#define c_oAscAsyncAction_LoadImage				5	// подгрузка картинки
#define c_oAscAsyncAction_DownloadAs			6
#define c_oAscAsyncAction_Print					7	// конвертация в PDF и сохранение у пользователя
#define c_oAscAsyncAction_UploadImage			8
#define c_oAscAsyncAction_ApplyChanges			9	// применение изменений от другого пользователя.
#define c_oAscAsyncAction_PrepareToSave			10	// Подготовка к сохранению

//files type for Saving & DownloadAs
#define c_oAscFileType_INNER 		0x0041
#define c_oAscFileType_DOCX 		0x0041
#define c_oAscFileType_DOC 			0x0042
#define c_oAscFileType_ODT			0x0043
#define c_oAscFileType_RTF			0x0044
#define c_oAscFileType_TXT			0x0045
#define c_oAscFileType_HTML_ZIP 	0x0803
#define c_oAscFileType_MHT			0x0047
#define c_oAscFileType_PDF	 		0x0201
#define c_oAscFileType_EPUB 		0x0048
#define c_oAscFileType_FB2	 		0x0049
#define c_oAscFileType_MOBI 		0x004a
#define c_oAscFileType_DOCY 		0x1001

// Right = 0; Left = 1; Center = 2; Justify = 3;
#define c_oAscAlignType_LEFT		0
#define c_oAscAlignType_CENTER		1
#define c_oAscAlignType_RIGHT		2
#define c_oAscAlignType_JUSTIFY		3
#define c_oAscAlignType_TOP			4
#define c_oAscAlignType_MIDDLE		5
#define c_oAscAlignType_BOTTOM		6

#define c_oAscWrapStyle2_Inline			0
#define c_oAscWrapStyle2_Square			1
#define c_oAscWrapStyle2_Tight			2
#define c_oAscWrapStyle2_Through		3
#define c_oAscWrapStyle2_TopAndBottom	4
#define c_oAscWrapStyle2_Behind			5
#define c_oAscWrapStyle2_InFront		6

/*Error level & ID*/
#define c_oAscError_Level_Critical                          -1
#define c_oAscError_Level_NoCritical                        0

#define c_oAscError_ID_ServerSaveComplete                   3
#define c_oAscError_ID_ConvertationProgress                 2
#define c_oAscError_ID_DownloadProgress                     1
#define c_oAscError_ID_No                                   0
#define c_oAscError_ID_Unknown                              -1
#define c_oAscError_ID_ConvertationTimeout                  -2
#define c_oAscError_ID_ConvertationError                    -3
#define c_oAscError_ID_DownloadError                        -4
#define c_oAscError_ID_UnexpectedGuid                       -5
#define c_oAscError_ID_Database                             -6
#define c_oAscError_ID_FileRequest                          -7
#define c_oAscError_ID_FileVKey                             -8
#define c_oAscError_ID_UplImageSize                         -9
#define c_oAscError_ID_UplImageExt                          -10
#define c_oAscError_ID_UplImageFileCount                    -11
#define c_oAscError_ID_NoSupportClipdoard                   -12
#define c_oAscError_ID_UplImageUrl                          -13

#define c_oAscError_ID_StockChartError                      -17
#define c_oAscError_ID_CoAuthoringDisconnect                -18
#define c_oAscError_ID_ConvertationPassword                 -19
#define c_oAscError_ID_VKeyEncrypt                          -20
#define c_oAscError_ID_KeyExpire                            -21
#define c_oAscError_ID_UserCountExceed                      -22

#define c_oAscError_ID_SplitCellMaxRows                     -30
#define c_oAscError_ID_SplitCellMaxCols                     -31
#define c_oAscError_ID_SplitCellRowsDivider                 -32
#define c_oAscError_ID_MobileUnexpectedCharCount            -35
#define c_oAscError_ID_MailMergeLoadFile                    -40
#define c_oAscError_ID_MailMergeSaveFile                    -41
#define c_oAscError_ID_AutoFilterDataRangeError             -50
#define c_oAscError_ID_AutoFilterChangeFormatTableError     -51
#define c_oAscError_ID_AutoFilterChangeError                -52
#define c_oAscError_ID_AutoFilterMoveToHiddenRangeError     -53
#define c_oAscError_ID_LockedAllError                       -54
#define c_oAscError_ID_LockedWorksheetRename                -55
#define c_oAscError_ID_FTChangeTableRangeError              -56
#define c_oAscError_ID_FTRangeIncludedOtherTables           -57

#define c_oAscError_ID_PasteMaxRangeError                   -64
#define c_oAscError_ID_PastInMergeAreaError                 -65

#define c_oAscError_ID_DataRangeError                       -72
#define c_oAscError_ID_CannotMoveRange                      -71

#define c_oAscError_ID_MaxDataSeriesError                   -80
#define c_oAscError_ID_CannotFillRange                      -81

#define c_oAscError_ID_UserDrop                             -100
#define c_oAscError_ID_Warning                              -101

#define c_oAscError_ID_FrmlWrongCountParentheses            -300
#define c_oAscError_ID_FrmlWrongOperator                    -301
#define c_oAscError_ID_FrmlWrongMaxArgument                 -302
#define c_oAscError_ID_FrmlWrongCountArgument               -303
#define c_oAscError_ID_FrmlWrongFunctionName                -304
#define c_oAscError_ID_FrmlAnotherParsingError              -305
#define c_oAscError_ID_FrmlWrongArgumentRange               -306
#define c_oAscError_ID_FrmlOperandExpected                  -307
#define c_oAscError_ID_FrmlParenthesesCorrectCount          -308
#define c_oAscError_ID_FrmlWrongReferences                  -309

#define c_oAscError_ID_InvalidReferenceOrName               -310
#define c_oAscError_ID_LockCreateDefName                    -311

#define c_oAscError_ID_OpenWarning                          500

#define c_oAscError_ID_EvaluteJSError                       -1000

#define c_oAscTypeSelectElement_Paragraph			0
#define c_oAscTypeSelectElement_Table				1
#define c_oAscTypeSelectElement_Image				2
#define c_oAscTypeSelectElement_Header				3
#define c_oAscTypeSelectElement_Hyperlink			4
#define c_oAscTypeSelectElement_SpellCheck			5
#define c_oAscTypeSelectElement_Shape				6
#define c_oAscTypeSelectElement_Slide				7
#define c_oAscTypeSelectElement_Chart				8
#define c_oAscTypeSelectElement_Math                9
#define c_oAscTypeSelectElement_MailMerge           10
#define c_oAscTypeSelectElement_ContentControl      11

#define c_oAscTableBordersType_LEFT					0
#define c_oAscTableBordersType_TOP					1
#define c_oAscTableBordersType_RIGHT				2
#define c_oAscTableBordersType_BOTTOM				3
#define c_oAscTableBordersType_VERTLINE				4
#define c_oAscTableBordersType_HORIZONTLINE			5
#define c_oAscTableBordersType_INSIDE				6
#define c_oAscTableBordersType_OUTSIDE				7
#define c_oAscTableBordersType_ALL					8

#define FONT_THUMBNAIL_HEIGHT						26

#define c_oAscStyleImage_Default					0
#define c_oAscStyleImage_Document					1

#define c_oAscLineDrawingRule_Left					0
#define c_oAscLineDrawingRule_Center				1
#define c_oAscLineDrawingRule_Right					2
#define c_oAscLineDrawingRule_Top					0
#define c_oAscLineDrawingRule_Bottom				2

// Chart defines
#define c_oAscChartType_line						0
#define c_oAscChartType_bar							1
#define c_oAscChartType_hbar						2
#define c_oAscChartType_area						3
#define c_oAscChartType_pie							4
#define c_oAscChartType_scatter						5
#define c_oAscChartType_stock						6

#define c_oAscChartSubType_normal					0
#define c_oAscChartSubType_stacked					1
#define c_oAscChartSubType_stackedPer				2

#define vertalign_Baseline			0
#define vertalign_SuperScript		1
#define vertalign_SubScript			2
#define hdrftr_Header				0x01
#define hdrftr_Footer				0x02

#define hdrftr_Default				0x01
#define hdrftr_Even					0x02
#define hdrftr_First				0x03

#define c_oAscTableSelectionType_Cell		0
#define c_oAscTableSelectionType_Row		1
#define c_oAscTableSelectionType_Column		2
#define c_oAscTableSelectionType_Table		3

#define linerule_AtLeast	0
#define linerule_Auto		1
#define linerule_Exact		2

#define shd_Clear			0
#define shd_Nil				1

#define c_oAscHyperlinkType_InternalLink            0
#define c_oAscHyperlinkType_WebLink                 1
#define c_oAscHyperlinkType_RangeLink               2

#define c_oAscContextMenuTypes_Common				0	// Обычное контекстное меню
#define c_oAscContextMenuTypes_ChangeHdrFtr			1	// Специальное контестное меню для попадания в колонтитул

#define c_oAscMouseMoveDataTypes_Common				0
#define c_oAscMouseMoveDataTypes_Hyperlink			1
#define c_oAscMouseMoveDataTypes_LockedObject		2

#define c_oAscMouseMoveLockedObjectType_Common		0
#define c_oAscMouseMoveLockedObjectType_Header		1
#define c_oAscMouseMoveLockedObjectType_Footer		2

#define c_oAscCollaborativeMarksShowType_None				-1
#define c_oAscCollaborativeMarksShowType_All				0
#define c_oAscCollaborativeMarksShowType_LastChanges		1

#define c_oAscAlignH_Center		0x00
#define c_oAscAlignH_Inside		0x01
#define c_oAscAlignH_Left		0x02
#define c_oAscAlignH_Outside	0x03
#define c_oAscAlignH_Right		0x04

#define c_oAscChangeLevel_BringToFront			0x00
#define c_oAscChangeLevel_BringForward			0x01
#define c_oAscChangeLevel_SendToBack			0x02
#define c_oAscChangeLevel_BringBackward			0x03

#define c_oAscAlignV_Bottom						0x00
#define c_oAscAlignV_Center						0x01
#define c_oAscAlignV_Inside						0x02
#define c_oAscAlignV_Outside					0x03
#define c_oAscAlignV_Top						0x04

#define c_oAscVertAlignJc_Top					0x00
#define c_oAscVertAlignJc_Center				0x01
#define c_oAscVertAlignJc_Bottom				0x02

#define c_oAscTableLayout_AutoFit				0x00
#define c_oAscTableLayout_Fixed					0x01

#define c_oAscColor_COLOR_TYPE_SRGB				1
#define c_oAscColor_COLOR_TYPE_PRST				2
#define c_oAscColor_COLOR_TYPE_SCHEME			3

#define c_oAscFill_FILL_TYPE_BLIP				1
#define c_oAscFill_FILL_TYPE_NOFILL				2
#define c_oAscFill_FILL_TYPE_SOLID				3
#define c_oAscFill_FILL_TYPE_GRAD				4
#define c_oAscFill_FILL_TYPE_PATT				5

#define c_oAscFillGradType_GRAD_LINEAR			1
#define c_oAscFillGradType_GRAD_PATH			2

#define c_oAscFillBlipType_STRETCH				1
#define c_oAscFillBlipType_TILE					2

#define c_oAscStrokeType_STROKE_NONE			0
#define c_oAscStrokeType_STROKE_COLOR			1

#define c_oAscAlignShapeType_ALIGN_LEFT			0
#define c_oAscAlignShapeType_ALIGN_RIGHT		1
#define c_oAscAlignShapeType_ALIGN_TOP			2
#define c_oAscAlignShapeType_ALIGN_BOTTOM		3
#define c_oAscAlignShapeType_ALIGN_CENTER		4
#define c_oAscAlignShapeType_ALIGN_MIDDLE		5

#define c_oAscVerticalTextAlign_TEXT_ALIGN_BOTTOM		0	// (Text Anchor Enum ( Bottom ))
#define c_oAscVerticalTextAlign_TEXT_ALIGN_CTR			1	// (Text Anchor Enum ( Center ))
#define c_oAscVerticalTextAlign_TEXT_ALIGN_DIST			2	// (Text Anchor Enum ( Distributed ))
#define c_oAscVerticalTextAlign_TEXT_ALIGN_JUST			3	// (Text Anchor Enum ( Justified ))
#define c_oAscVerticalTextAlign_TEXT_ALIGN_TOP			4	// Top

#define c_oAscLineJoinType_Round				1
#define c_oAscLineJoinType_Bevel				2
#define c_oAscLineJoinType_Miter				3

#define c_oAscLineCapType_Flat					0
#define c_oAscLineCapType_Round					1
#define c_oAscLineCapType_Square				2

#define c_oAscLineBeginType_None				0
#define c_oAscLineBeginType_Arrow				1
#define c_oAscLineBeginType_Diamond				2
#define c_oAscLineBeginType_Oval				3
#define c_oAscLineBeginType_Stealth				4
#define c_oAscLineBeginType_Triangle			5

#define c_oAscLineBeginSize_small_small			0
#define c_oAscLineBeginSize_small_mid			1
#define c_oAscLineBeginSize_small_large			2
#define c_oAscLineBeginSize_mid_small			3
#define c_oAscLineBeginSize_mid_mid				4
#define c_oAscLineBeginSize_mid_large			5
#define c_oAscLineBeginSize_large_small			6
#define c_oAscLineBeginSize_large_mid			7
#define c_oAscLineBeginSize_large_large			8

#define TABLE_STYLE_WIDTH_PIX					70
#define TABLE_STYLE_HEIGHT_PIX					50

#define c_oAscDropCap_None						0
#define c_oAscDropCap_Drop						1
#define c_oAscDropCap_Margin					2

#define c_oAsc_wrap_Around						0x01
#define c_oAsc_wrap_Auto						0x02
#define c_oAsc_wrap_None						0x03
#define c_oAsc_wrap_NotBeside					0x04
#define c_oAsc_wrap_Through						0x05
#define c_oAsc_wrap_Tight						0x06

#define c_oAscSectionBreakType_NextPage			0x00
#define c_oAscSectionBreakType_OddPage			0x01
#define c_oAscSectionBreakType_EvenPage			0x02
#define c_oAscSectionBreakType_Continuous		0x03
#define c_oAscSectionBreakType_Column			0x04

#define c_oAscRestriction_None                  0x00
#define c_oAscRestriction_OnlyForms             0x01
#define c_oAscRestriction_OnlyComments          0x02
#define c_oAscRestriction_OnlySignatures        0x04
#define c_oAscRestriction_View                  0x80

#define c_oAscAdvancedOptionsID_CSV             0
#define c_oAscAdvancedOptionsID_TXT             1
#define c_oAscAdvancedOptionsID_DRM             2

#define c_oAscContentControlSpecificTypeNone          0
#define c_oAscContentControlSpecificTypeCheckBox      1
#define c_oAscContentControlSpecificTypePicture       2
#define c_oAscContentControlSpecificTypeComboBox      3
#define c_oAscContentControlSpecificTypeDropDownList  4
#define c_oAscContentControlSpecificTypeDateTime      5
#define c_oAscContentControlSpecificTypeTOC           10

#define c_oAscEDocProtectComments 0
#define c_oAscEDocProtectForms 1
#define c_oAscEDocProtectNone 2
#define c_oAscEDocProtectReadOnly 3
#define c_oAscEDocProtectTrackedChanges 4

#define INSERT_PAGE_NUM_PARAM(AlignV, AlignH)	((AlignV << 16) | AlignH)

// MENU COMMANDS
#define ASC_MENU_EVENT_TYPE_TEXTPR					1
#define ASC_MENU_EVENT_TYPE_PARAPR					2
#define ASC_MENU_EVENT_TYPE_UNDO					3
#define ASC_MENU_EVENT_TYPE_REDO					4
#define ASC_MENU_EVENT_TYPE_LOADDOCUMENT			5
#define ASC_MENU_EVENT_TYPE_STACK_OBJECTS			6
#define ASC_MENU_EVENT_TYPE_HEADERFOOTER			7
#define ASC_MENU_EVENT_TYPE_HYPERLINK				8
#define ASC_MENU_EVENT_TYPE_IMAGE					9
#define ASC_MENU_EVENT_TYPE_TABLE					10
#define ASC_MENU_EVENT_TYPE_PARAGRAPHSTYLES			11
#define ASC_MENU_EVENT_TYPE_TABLESTYLES				12
#define ASC_MENU_EVENT_TYPE_INCREASEPARAINDENT		13
#define ASC_MENU_EVENT_TYPE_DECREASEPARAINDENT		14
#define ASC_MENU_EVENT_TYPE_TABLEMERGECELLS			15
#define ASC_MENU_EVENT_TYPE_TABLESPLITCELLS			16
#define ASC_MENU_EVENT_TYPE_SECTION					17
#define ASC_MENU_EVENT_TYPE_SHAPE                   18
#define ASC_MENU_EVENT_TYPE_SLIDE                   20
#define ASC_MENU_EVENT_TYPE_CHART                   21
#define ASC_MENU_EVENT_TYPE_MATH                    22

// insert commands
#define ASC_MENU_EVENT_TYPE_INSERT_IMAGE			50
#define ASC_MENU_EVENT_TYPE_INSERT_TABLE			51
#define ASC_MENU_EVENT_TYPE_INSERT_HYPERLINK		52
#define ASC_MENU_EVENT_TYPE_INSERT_SHAPE			53
#define ASC_MENU_EVENT_TYPE_INSERT_PAGEBREAK		54
#define ASC_MENU_EVENT_TYPE_INSERT_LINEBREAK		55
#define ASC_MENU_EVENT_TYPE_INSERT_PAGENUMBER		56
#define ASC_MENU_EVENT_TYPE_INSERT_SECTIONBREAK		57

// hyperlink
#define ASC_MENU_EVENT_TYPE_CAN_ADD_HYPERLINK		58
#define ASC_MENU_EVENT_TYPE_REMOVE_HYPERLINK		59

// undo/redo
#define ASC_MENU_EVENT_TYPE_CAN_UNDO				60
#define ASC_MENU_EVENT_TYPE_CAN_REDO				61

#define ASC_MENU_EVENT_TYPE_SEARCH_FINDTEXT			62
#define ASC_MENU_EVENT_TYPE_SEARCH_REPLACETEXT		63
#define ASC_MENU_EVENT_TYPE_SEARCH_SELECTRESULTS	64
#define ASC_MENU_EVENT_TYPE_SEARCH_ISSELECTRESULTS	65

#define ASC_MENU_EVENT_TYPE_DOCUMETN_MODIFITY		66

// statictic
#define ASC_MENU_EVENT_TYPE_STATISTIC_START         67
#define ASC_MENU_EVENT_TYPE_STATISTIC_STOP          68
#define ASC_MENU_EVENT_TYPE_STATISTIC_END           69
#define ASC_MENU_EVENT_TYPE_STATISTIC_INFO          70

#define ASC_MENU_EVENT_TYPE_TABLE_INSERTDELETE_ROWCOLUMN          71

#define ASC_MENU_EVENT_TYPE_KEYBOARD_SHOW           100
#define ASC_MENU_EVENT_TYPE_KEYBOARD_UNSHOW         101

#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_SHOW        102
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_UNSHOW      103
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_COMMANDS_AVAILIBLE    104

#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_COPY		110
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_CUT			111
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_PASTE		112
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_DELETE		113
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_SELECT		114
#define ASC_MENU_EVENT_TYPE_CONTEXTMENU_SELECTALL	115

#define ASC_MENU_EVENT_TYPE_DOCUMENT_BASE64         200

#define ASC_MENU_EVENT_TYPE_DOCUMENT_CHARTSTYLES    201

#define ASC_MENU_EVENT_TYPE_DOCUMENT_PDFBASE64      202
#define ASC_MENU_EVENT_TYPE_DOCUMENT_PDFBASE64_PRINT    203

#define ASC_MENU_EVENT_TYPE_USER_SCROLL_PAGE        300
#define ASC_MENU_EVENT_TYPE_USER_ZOOM               301

#define ASC_MENU_EVENT_TYPE_INSERT_CHART            400
#define ASC_MENU_EVENT_TYPE_INSERT_SCREEN_IMAGE     401
#define ASC_MENU_EVENT_TYPE_ADD_CHART_DATA          440
#define ASC_MENU_EVENT_TYPE_GET_CHART_DATA          450
#define ASC_MENU_EVENT_TYPE_SET_CHART_DATA          460

#define ASC_MENU_EVENT_TYPE_ERROR                   500

#define ASC_EVENT_TYPE_DROP_OPERATION_COPY_TEXT     600

#define ASC_MENU_EVENT_TYPE_COLOR_SCHEMES           2404    //  CAscColorSchemes
#define ASC_MENU_EVENT_TYPE_CHANGE_COLOR_SCHEME     2415    //  SET(int)
#define ASC_MENU_EVENT_TYPE_GET_COLOR_SCHEME        2416    //  GET(int)
#define ASC_MENU_EVENT_TYPE_THEMECOLORS             2417

#define ASC_MENU_EVENT_TYPE_CHANGE_MOBILE_MODE      2500

#define ASC_MENU_EVENT_TYPE_GO_TO_INTERNAL_LINK     5000

#define ASC_SOCKET_EVENT_TYPE_OPEN                  10000
#define ASC_SOCKET_EVENT_TYPE_ON_CLOSE              10010
#define ASC_SOCKET_EVENT_TYPE_MESSAGE               10020
#define ASC_SOCKET_EVENT_TYPE_ON_DISCONNECT         11010
#define ASC_SOCKET_EVENT_TYPE_TRY_RECONNECT         11020

#define ASC_COAUTH_EVENT_TYPE_PARTICIPANTS_CHANGED  20101
#define ASC_COAUTH_EVENT_TYPE_LOST_CONNECTION       20102
#define ASC_COAUTH_EVENT_TYPE_DROP_CONNECTION       20103
#define ASC_COAUTH_EVENT_TYPE_ERROR_CONNECTION      20104

#define ASC_COAUTH_EVENT_TYPE_INSERT_URL_IMAGE      21000
#define ASC_COAUTH_EVENT_TYPE_LOAD_URL_IMAGE        21001
#define ASC_COAUTH_EVENT_TYPE_REPLACE_URL_IMAGE     21002

#define ASC_MENU_EVENT_TYPE_ADVANCED_OPTIONS        22000
#define ASC_MENU_EVENT_TYPE_SET_PASSWORD            22001
#define ASC_MENU_EVENT_TYPE_SET_TRANSLATIONS        22002

#define ASC_MENU_EVENT_TYPE_ON_EDIT_TEXT            22003

#define ASC_EVENT_TYPE_SPELLCHECK_MESSAGE           22004
#define ASC_EVENT_TYPE_SPELLCHECK_TURN_ON           22005

#define ASC_MENU_EVENT_TYPE_DO_NONPRINTING_DISPLAY  22006


#define ASC_MENU_EVENT_RUN_JS_SCRIPT                22007
#define ASC_MENU_EVENT_RUN_JS_SCRIPT_FUNCTION       22008

// Comments
#define ASC_MENU_EVENT_TYPE_ADD_COMMENT                     23001
#define ASC_MENU_EVENT_TYPE_ADD_COMMENTS                    23002
#define ASC_MENU_EVENT_TYPE_REMOVE_COMMENT                  23003
#define ASC_MENU_EVENT_TYPE_CHANGE_COMMENTS                 23004
#define ASC_MENU_EVENT_TYPE_REMOVE_COMMENTS                 23005
#define ASC_MENU_EVENT_TYPE_CHANGE_COMMENT_DATA             23006
#define ASC_MENU_EVENT_TYPE_LOCK_COMMENT                    23007
#define ASC_MENU_EVENT_TYPE_UNLOCK_COMMENT                  23008
#define ASC_MENU_EVENT_TYPE_SHOW_COMMENT                    23009
#define ASC_MENU_EVENT_TYPE_HIDE_COMMENT                    23010
#define ASC_MENU_EVENT_TYPE_UPDATE_COMMENT_POSITION         23011
#define ASC_MENU_EVENT_TYPE_DOCUMENT_PLACE_CHANGED          23012
#define ASC_MENU_EVENT_TYPE_DO_SELECT_COMMENT               23101
#define ASC_MENU_EVENT_TYPE_DO_SHOW_COMMENT                 23102
#define ASC_MENU_EVENT_TYPE_DO_SELECT_COMMENTS              23103
#define ASC_MENU_EVENT_TYPE_DO_DESELECT_COMMENTS            23104
#define ASC_MENU_EVENT_TYPE_DO_ADD_COMMENT                  23105
#define ASC_MENU_EVENT_TYPE_DO_REMOVE_COMMENT               23106
#define ASC_MENU_EVENT_TYPE_DO_REMOVE_ALL_COMMENTS          23107
#define ASC_MENU_EVENT_TYPE_DO_CHANGE_COMMENT               23108
#define ASC_MENU_EVENT_TYPE_DO_CAN_ADD_QUOTED_COMMENT       23109

// Track reviews
#define ASC_MENU_EVENT_TYPE_SHOW_REVISIONS_CHANGE           24001

#define ASC_MENU_EVENT_TYPE_DO_SET_TRACK_REVISIONS          24101
#define ASC_MENU_EVENT_TYPE_DO_BEGIN_VIEWMODE_IN_REVIEW     24102
#define ASC_MENU_EVENT_TYPE_DO_END_VIEWMODE_IN_REVIEW       24103
#define ASC_MENU_EVENT_TYPE_DO_ACCEPT_ALL_CHANGES           24104
#define ASC_MENU_EVENT_TYPE_DO_REJECT_ALL_CHANGES           24105
#define ASC_MENU_EVENT_TYPE_DO_GET_PREV_REVISIONS_CHANGE    24106
#define ASC_MENU_EVENT_TYPE_DO_GET_NEXT_REVISIONS_CHANGE    24107
#define ASC_MENU_EVENT_TYPE_DO_ACCEPT_CHANGES               24108
#define ASC_MENU_EVENT_TYPE_DO_REJECT_CHANGES               24109
#define ASC_MENU_EVENT_TYPE_DO_FOLLOW_REVISION_MOVE         24110

// Universal call
#define ASC_MENU_EVENT_TYPE_DO_API_FUNCTION_CALL            25001

// Fill forms
#define ASC_MENU_EVENT_TYPE_SHOW_CONTENT_CONTROLS_ACTIONS   26001
#define ASC_MENU_EVENT_TYPE_HIDE_CONTENT_CONTROLS_ACTIONS   26002
#define ASC_MENU_EVENT_TYPE_DO_SET_CONTENTCONTROL_PICTURE   26003
#define ASC_MENU_EVENT_TYPE_DO_SET_CONTENTCONTROL_PICTURE_URL   26004

// Footnote
#define ASC_MENU_EVENT_TYPE_SET_FOOTNOTE_PROP               27001

// Others
#define ASC_MENU_EVENT_TYPE_FOCUS_OBJECT                    26101
#define ASC_MENU_EVENT_TYPE_LONGACTION_BEGIN                26102
#define ASC_MENU_EVENT_TYPE_LONGACTION_END                  26103
#define ASC_MENU_EVENT_TYPE_API_ERROR                       26104

// Document Processing

#define ASC_EVENT_TYPE_OPEN_DOCUMENT_PROCESSING_BEGIN       60100
#define ASC_EVENT_TYPE_OPEN_DOCUMENT_PROCESSING_END         60101
#define ASC_EVENT_TYPE_OPEN_DOCUMENT_ERROR                  60102

#endif //_BUILD_EDITOR_DEFINES_CROSSPLATFORM_H_
