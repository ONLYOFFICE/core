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
#pragma once

//For the font character set and a Panose.

namespace ASCDocFileFormat
{
	namespace Constants
	{
		enum HorzMerge
		{
			horzMergeNotMerged = 0,
			horzMergeHorizontallyMerged = 1,
			horzMergeFirstCell2 = 2,
			horzMergeFirstCell3 = 3
		};

		enum TextFlow
		{
			grpfTFlrtb = 0x0000,
			grpfTFtbrl = 0x0001,
			grpfTFbtlr = 0x0003,
			grpfTFlrtbv = 0x0004,
			grpfTFtbrlv = 0x0005
		};

		enum VerticalMergeFlag
		{
			fvmClear = 0x00,
			fvmMerge = 0x01,
			fvmRestart = 0x03
		};

		enum VerticalAlign
		{
			vaTop = 0x00,
			vaCenter = 0x01,
			vaBottom = 0x02
		};

		enum Fts
		{
			ftsNil = 0x00,
			ftsAuto = 0x01,
			ftsPercent = 0x02,
			ftsDxa = 0x03,
			ftsDxaSys = 0x13 //???
		};

		enum Fbrc
		{
			fbrcTop = 0x01,
			fbrcLeft = 0x02,
			fbrcBottom = 0x04,
			fbrcRight = 0x08
		};

		enum Itl
		{
			itlNil = -1,
			itlNone = 0
		};

		enum CharacterPitch
		{
			characterPitchDefault = 0x00,
			characterPitchFixed = 0x01,
			characterPitchVariable = 0x02
		};

		enum FontFamilyType
		{
			fontFamilyTypeUnspecified = 0x00,
			fontFamilyTypeRomanSerif = 0x01,
			fontFamilyTypeSwissSansSerif = 0x02,
			fontFamilyTypeModernMonospace = 0x03,
			fontFamilyTypeScriptCursive = 0x04,
			fontFamilyTypeDecorativeFantasy = 0x05
		};

		enum StyleType
		{
			styleTypeParagraph = 1,
			styleTypeCharacter = 2,
			styleTypeTable = 3,
			styleTypeNumbering = 4
		};

		enum NumberingFormat
		{
			numberingFormatNone = 0x7FFF,
			numberingFormatBlackCircle = 0x0000,
			numberingFormatWhiteCircle = 0x0001,
			numberingFormatBlackSquare = 0x0002,
			numberingFormatWhiteSquare = 0x0003,
			numberingFormatFourRhombuses = 0x0004,
			numberingFormatRightArrow = 0x0005,
			numberingFormatDaw = 0x0006,
			numberingFormatArabianNumberDot = 0x0007,
			numberingFormatArabianNumberBracket = 0x0008,
			numberingFormatLargeRomanNumberDot = 0x0009,
			numberingFormatLargeLetterDot = 0x000A,
			numberingFormatSmallLetterBracket = 0x000B,
			numberingFormatSmallLetterDot = 0x000C,
			numberingFormatSmallRomanNumberDot = 0x000D
		};

		enum MSONFC
		{
			msonfcArabic = 0x00, //decimal
			msonfcUCRoman = 0x01, //upperRoman
			msonfcLCRoman = 0x02, //lowerRoman
			msonfcUCLetter = 0x03, //upperLetter
			msonfcLCLetter = 0x04, //lowerLetter
			msonfcOrdinal = 0x05, //ordinal
			msonfcCardtext = 0x06, //cardinalText
			msonfcOrdtext = 0x07, //ordinalText
			msonfcHex = 0x08, //hex
			msonfcChiManSty = 0x09, //chicago
			msonfcDbNum1 = 0x0A, //ideographDigital
			msonfcDbNum2 = 0x0B, //japaneseCounting
			msonfcAiueo = 0x0C, //Aiueo
			msonfcIroha = 0x0D, //Iroha
			msonfcDbChar = 0x0E, //decimalFullWidth
			msonfcSbChar = 0x0F, //decimalHalfWidth
			msonfcDbNum3 = 0x10, //japaneseLegal
			msonfcDbNum4 = 0x11, //japaneseDigitalTenThousand
			msonfcCirclenum = 0x12, //decimalEnclosedCircle
			msonfcDArabic = 0x13, //decimalFullWidth2
			msonfcDAiueo = 0x14, //aiueoFullWidth
			msonfcDIroha = 0x15, //irohaFullWidth
			msonfcArabicLZ = 0x16, //decimalZero
			msonfcBullet = 0x17, //bullet
			msonfcGanada = 0x18, //ganada
			msonfcChosung = 0x19, //chosung
			msonfcGB1 = 0x1A, //decimalEnclosedFullstop
			msonfcGB2 = 0x1B, //decimalEnclosedParen
			msonfcGB3 = 0x1C, //decimalEnclosedCircleChinese
			msonfcGB4 = 0x1D, //ideographEnclosedCircle
			msonfcZodiac1 = 0x1E, //ideographTraditional
			msonfcZodiac2 = 0x1F, //ideographZodiac
			msonfcZodiac3 = 0x20, //ideographZodiacTraditional
			msonfcTpeDbNum1 = 0x21, //taiwaneseCounting
			msonfcTpeDbNum2 = 0x22, //ideographLegalTraditional
			msonfcTpeDbNum3 = 0x23, //taiwaneseCountingThousand
			msonfcTpeDbNum4 = 0x24, //taiwaneseDigital
			msonfcChnDbNum1 = 0x25, //chineseCounting
			msonfcChnDbNum2 = 0x26, //chineseLegalSimplified
			msonfcChnDbNum3 = 0x27, //chineseCountingThousand
			msonfcChnDbNum4 = 0x28, //decimal
			msonfcKorDbNum1 = 0x29, //koreanDigital
			msonfcKorDbNum2 = 0x2A, //koreanCounting
			msonfcKorDbNum3 = 0x2B, //koreanLegal
			msonfcKorDbNum4 = 0x2C, //koreanDigital2
			msonfcHebrew1 = 0x2D, //hebrew1
			msonfcArabic1 = 0x2E, //arabicAlpha
			msonfcHebrew2 = 0x2F, //hebrew2
			msonfcArabic2 = 0x30, //arabicAbjad
			msonfcHindi1 = 0x31, //hindiVowels
			msonfcHindi2 = 0x32, //hindiConsonants
			msonfcHindi3 = 0x33, //hindiNumbers
			msonfcHindi4 = 0x34, //hindiCounting
			msonfcThai1 = 0x35, //thaiLetters
			msonfcThai2 = 0x36, //thaiNumbers
			msonfcThai3 = 0x37, //thaiCounting
			msonfcViet1 = 0x38, //vietnameseCounting
			msonfcNumInDash = 0x39, //numberInDash
			msonfcLCRus = 0x3A, //russianLower
			msonfcUCRus = 0x3B, //russianUpper
			msonfcNone = 0xFF //Specifies that the sequence shall not display any numbering.
		};

		enum LevelJustification
		{
			levelJustificationLeft = 0x0,
			levelJustificationCenter = 0x1,
			levelJustificationRight = 0x2
		};

		enum CharacterFollows
		{
			characterFollowsTab = 0x0,
			characterFollowsSpace = 0x1,
			characterFollowsNothing = 0x2
		};

		enum LFOField
		{
			lfoFieldNotUsed00 = 0x00,
			lfoFieldAUTONUMLGL = 0xFC,
			lfoFieldAUTONUMOUT = 0xFD,
			lfoFieldAUTONUM = 0xFE,
			lfoFieldNotUsedFF = 0xFF
		};

		enum SBkcOperand
		{
			bkcContinuous = 0x00, //Continuous section break. The next section starts on the next line.
			bkcNewColumn = 0x01, //New column section break. The next section starts in the next column.
			bkcNewPage = 0x02, //New page section break. The next section starts on the next page.
			bkcEvenPage = 0x03, //Even page section break. The next section starts on an even page.
			bkcOddPage = 0x04 //Odd page section break. The next section starts on an odd page.
		};

		enum SBOrientationOperand
		{
			dmOrientPortrait = 0x01, //Portrait orientation.
			dmOrientLandscape = 0x02 //Landscape orientation.
		};

		enum SClmOperand
		{
			clmUseDefault = 0x0000, //Specifies that document grid is disabled.
			clmCharsAndLines = 0x0001, //Specifies a document grid enforcing both character spacing and line pitch. The line pitch and character spacing are specified by sprmSDyaLinePitch and sprmSDxtCharSpace respectively.
			clmLinesOnly = 0x0002, //Specifies a document grid enforcing only line pitch. Line pitch is specified by sprmSDyaLinePitch.
			clmEnforceGrid = 0x0003 //Specifies a document grid enforcing both character spacing and line pitch. The line pitch and character spacing are specified by sprmSDyaLinePitch and sprmSDxtCharSpace respectively. Each full-width character MUST occupy its own grid square.
		};

		enum PictureDataFormat
		{
			MM_SHAPE = 0x0064, //Shape object
			MM_SHAPEFILE = 0x0066 //Shape file
		};

		enum SuperSubScript
		{
			superSubScriptNormalText = 0x00,
			superSubScriptSuperscript = 0x01,
			superSubScriptSubscript = 0x02
		};

		enum BordersToApply
		{
			bordersToApplyTopBorder					=	0x01,
			bordersToApplyLogicalLeftBorder			=	0x02,
			bordersToApplyBottomBorder				=	0x04,
			bordersToApplyLogicalRightBorder		=	0x08,
			bordersToApplyFromTopLeftToBottomRight	=	0x10,
			bordersToApplyFromTopRightToBottomLeft	=	0x20
		};

		enum flt
		{
			fltUnable = 0x01, //Specifies that the field was unable to be parsed.
			fltREFKeywordOmitted = 0x02, //Specifies that the field represents a REF field where the keyword has been omitted.
			fltREF = 0x03,
			fltFTNREF = 0x05,
			fltSET = 0x06,
			fltIF = 0x07,
			fltINDEX = 0x08,
			fltSTYLEREF = 0x0A,
			fltSEQ = 0x0C,
			fltTOC = 0x0D,
			fltINFO = 0x0E,
			fltTITLE = 0x0F,
			fltSUBJECT = 0x10,
			fltAUTHOR = 0x11,
			fltKEYWORDS = 0x12,
			fltCOMMENTS = 0x13,
			fltLASTSAVEDBY = 0x14,
			fltCREATEDATE = 0x15,
			fltSAVEDATE = 0x16,
			fltPRINTDATE = 0x17,
			fltREVNUM = 0x18,
			fltEDITTIME = 0x19,
			fltNUMPAGES = 0x1A,
			fltNUMWORDS = 0x1B,
			fltNUMCHARS = 0x1C,
			fltFILENAME = 0x1D,
			fltTEMPLATE = 0x1E,
			fltDATE = 0x1F,
			fltTIME = 0x20,
			fltPAGE = 0x21,
			fltEqual = 0x22,
			fltQUOTE = 0x23,
			fltINCLUDE = 0x24,
			fltPAGEREF = 0x25,
			fltASK = 0x26,
			fltFILLIN = 0x27,
			fltDATA = 0x28,
			fltNEXT = 0x29,
			fltNEXTIF = 0x2A,
			fltSKIPIF = 0x2B,
			fltMERGEREC = 0x2C,
			fltDDE = 0x2D,
			fltDDEAUTO = 0x2E,
			fltGLOSSARY = 0x2F,
			fltPRINT = 0x30,
			fltEQ = 0x31,
			fltGOTOBUTTON = 0x32,
			fltMACROBUTTON = 0x33,
			fltAUTONUMOUT = 0x34,
			fltAUTONUMLGL = 0x35,
			fltAUTONUM = 0x36,
			fltIMPORT = 0x37,
			fltLINK = 0x38,
			fltSYMBOL = 0x39,
			fltEMBED = 0x3A,
			fltMERGEFIELD = 0x3B,
			fltUSERNAME = 0x3C,
			fltUSERINITIALS = 0x3D,
			fltUSERADDRESS = 0x3E,
			fltBARCODE = 0x3F,
			fltDOCVARIABLE = 0x40,
			fltSECTION = 0x41,
			fltSECTIONPAGES = 0x42,
			fltINCLUDEPICTURE = 0x43,
			fltINCLUDETEXT = 0x44,
			fltFILESIZE = 0x45,
			fltFORMTEXT = 0x46,
			fltFORMCHECKBOX = 0x47,
			fltNOTEREF = 0x48,
			fltTOA = 0x49,
			fltMERGESEQ = 0x4B,
			fltAUTOTEXT = 0x4F,
			fltCOMPARE = 0x50,
			fltADDIN = 0x51,
			fltFORMDROPDOWN = 0x53,
			fltADVANCE = 0x54,
			fltDOCPROPERTY = 0x55,
			fltCONTROL = 0x57,
			fltHYPERLINK = 0x58,
			fltAUTOTEXTLIST = 0x59,
			fltLISTNUM = 0x5A,
			fltHTMLCONTROL = 0x5B,
			fltBIDIOUTLINE = 0x5C,
			fltADDRESSBLOCK = 0x5D,
			fltGREETINGLINE = 0x5E,
			fltSHAPE = 0x5F
		};

		enum fpc
		{
			fpcEndOfSection = 0,
			fpcBottomMargin = 1,
			fpcLastLine = 2
		};

		enum rncFtn
		{
			rncFtnPreviousSection = 0,
			rncFtnUniqueSection = 1,
			rncFtnUniquePage = 2
		};

		enum rncEdn
		{
			rncEdnPreviousSection = 0,
			rncEdnUniqueSection = 1,
			rncEdnUniquePage = 2  
		};

		enum epc
		{
			epcEndOfSection = 0,
			epcEndofTheCurrentDocument = 3
		};

		enum ViewingMode
		{
			viewingModeNone = 0,
			viewingModePrint = 1,
			viewingModeOutline = 2,
			viewingModeMasterPages = 3,
			viewingModeNormal = 4,
			viewingModeWeb = 5
		};

		enum ZoomType
		{
			zoomTypeNone = 0,
			zoomTypeFullPage = 1,
			zoomTypeBestFit = 2,
			zoomTypeTextFit = 3
		};

		enum TabJC
		{
			jcLeft = 0x0, //Left justification
			jcCenter = 0x1, //Center justification
			jcRight = 0x2, //Right justification
			jcDecimal = 0x3, //Specifies that the current tab stop results in a location in the document where all following text is aligned around the first decimal separator in the following text runs. If there is no decimal separator, text is aligned around the implicit decimal separator after the last digit of the first numerical value appearing in the following text. All text runs before the first decimal character appear before the tab stop, all text runs after it appear after the tab stop location.
			jcBar = 0x4, //Specifies that the current tab is a bar tab.
			jcList = 0x6 //Specifies that the current tab is a list tab.
		};

		enum TabLC
		{
			tlcNone = 0x0, //No leader.
			tlcDot = 0x1, //Dot leader.
			tlcHyphen = 0x2, //Dashed leader.
			tlcUnderscore = 0x3, //Underscore leader.
			tlcHeavy = 0x4, //Same as tlcUnderscore.
			tlcMiddleDot = 0x5, //Centered dot leader.
			tlcDefault = 0x7 //Same as tlcNone.
		};
	}
}