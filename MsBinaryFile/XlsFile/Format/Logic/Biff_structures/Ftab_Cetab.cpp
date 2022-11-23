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

#include "Ftab_Cetab.h"

namespace XLS
{


Ftab_Cetab::Ftab_Cetab()
{
}


Ftab_Cetab::Ftab_Cetab(const std::wstring& word)
:	func_index(recognizeFunction(word))
{
}


Ftab_Cetab::Ftab_Cetab(const unsigned short func_index_init)
:	func_index(func_index_init)
{
}


Ftab_Cetab::ValuesDetermination::ValuesDetermination()
{
	// Ftab values
	params_fixed.insert(ParamsFixed(0x0000, -1, L"COUNT"));
	params_fixed.insert(ParamsFixed(0x0001, -1, L"IF"));
	params_fixed.insert(ParamsFixed(0x0002, 1, L"ISNA"));
	params_fixed.insert(ParamsFixed(0x0003, 1, L"ISERROR"));
	params_fixed.insert(ParamsFixed(0x0004, -1, L"SUM"));
	params_fixed.insert(ParamsFixed(0x0005, -1, L"AVERAGE"));
	params_fixed.insert(ParamsFixed(0x0006, -1, L"MIN"));
	params_fixed.insert(ParamsFixed(0x0007, -1, L"MAX"));
	params_fixed.insert(ParamsFixed(0x0008, -1, L"ROW"));
	params_fixed.insert(ParamsFixed(0x0009, -1, L"COLUMN"));
	params_fixed.insert(ParamsFixed(0x000A, 0, L"NA"));
	params_fixed.insert(ParamsFixed(0x000B, -1, L"NPV"));
	params_fixed.insert(ParamsFixed(0x000C, -1, L"STDEV"));
	params_fixed.insert(ParamsFixed(0x000D, -1, L"DOLLAR"));
	params_fixed.insert(ParamsFixed(0x000E, -1, L"FIXED"));
	params_fixed.insert(ParamsFixed(0x000F, 1, L"SIN"));
	params_fixed.insert(ParamsFixed(0x0010, 1, L"COS"));
	params_fixed.insert(ParamsFixed(0x0011, 1, L"TAN"));
	params_fixed.insert(ParamsFixed(0x0012, 1, L"ATAN"));
	params_fixed.insert(ParamsFixed(0x0013, 0, L"PI"));
	params_fixed.insert(ParamsFixed(0x0014, 1, L"SQRT"));
	params_fixed.insert(ParamsFixed(0x0015, 1, L"EXP"));
	params_fixed.insert(ParamsFixed(0x0016, 1, L"LN"));
	params_fixed.insert(ParamsFixed(0x0017, 1, L"LOG10"));
	params_fixed.insert(ParamsFixed(0x0018, 1, L"ABS"));
	params_fixed.insert(ParamsFixed(0x0019, 1, L"INT"));
	params_fixed.insert(ParamsFixed(0x001A, 1, L"SIGN"));
	params_fixed.insert(ParamsFixed(0x001B, 2, L"ROUND"));
	params_fixed.insert(ParamsFixed(0x001C, -1, L"LOOKUP"));
	params_fixed.insert(ParamsFixed(0x001D, -1, L"INDEX"));
	params_fixed.insert(ParamsFixed(0x001E, 2, L"REPT"));
	params_fixed.insert(ParamsFixed(0x001F, 3, L"MID"));
	params_fixed.insert(ParamsFixed(0x0020, 1, L"LEN"));
	params_fixed.insert(ParamsFixed(0x0021, 1, L"VALUE"));
	params_fixed.insert(ParamsFixed(0x0022, 0, L"TRUE"));
	params_fixed.insert(ParamsFixed(0x0023, 0, L"FALSE"));
	params_fixed.insert(ParamsFixed(0x0024, -1, L"AND"));
	params_fixed.insert(ParamsFixed(0x0025, -1, L"OR"));
	params_fixed.insert(ParamsFixed(0x0026, 1, L"NOT"));
	params_fixed.insert(ParamsFixed(0x0027, 2, L"MOD"));
	params_fixed.insert(ParamsFixed(0x0028, 3, L"DCOUNT"));
	params_fixed.insert(ParamsFixed(0x0029, 3, L"DSUM"));
	params_fixed.insert(ParamsFixed(0x002A, 3, L"DAVERAGE"));
	params_fixed.insert(ParamsFixed(0x002B, 3, L"DMIN"));
	params_fixed.insert(ParamsFixed(0x002C, 3, L"DMAX"));
	params_fixed.insert(ParamsFixed(0x002D, 3, L"DSTDEV"));
	params_fixed.insert(ParamsFixed(0x002E, -1, L"VAR"));
	params_fixed.insert(ParamsFixed(0x002F, 3, L"DVAR"));
	params_fixed.insert(ParamsFixed(0x0030, 2, L"TEXT"));
	params_fixed.insert(ParamsFixed(0x0031, -1, L"LINEST"));
	params_fixed.insert(ParamsFixed(0x0032, -1, L"TREND"));
	params_fixed.insert(ParamsFixed(0x0033, -1, L"LOGEST"));
	params_fixed.insert(ParamsFixed(0x0034, -1, L"GROWTH"));
	params_fixed.insert(ParamsFixed(0x0035, 1, L"GOTO"));
	params_fixed.insert(ParamsFixed(0x0036, -1, L"HALT"));
	params_fixed.insert(ParamsFixed(0x0037, -1, L"RETURN"));
	params_fixed.insert(ParamsFixed(0x0038, -1, L"PV"));
	params_fixed.insert(ParamsFixed(0x0039, -1, L"FV"));
	params_fixed.insert(ParamsFixed(0x003A, -1, L"NPER"));
	params_fixed.insert(ParamsFixed(0x003B, -1, L"PMT"));
	params_fixed.insert(ParamsFixed(0x003C, -1, L"RATE"));
	params_fixed.insert(ParamsFixed(0x003D, 3, L"MIRR"));
	params_fixed.insert(ParamsFixed(0x003E, -1, L"IRR"));
	params_fixed.insert(ParamsFixed(0x003F, 0, L"RAND"));
	params_fixed.insert(ParamsFixed(0x0040, -1, L"MATCH"));
	params_fixed.insert(ParamsFixed(0x0041, 3, L"DATE"));
	params_fixed.insert(ParamsFixed(0x0042, 3, L"TIME"));
	params_fixed.insert(ParamsFixed(0x0043, 1, L"DAY"));
	params_fixed.insert(ParamsFixed(0x0044, 1, L"MONTH"));
	params_fixed.insert(ParamsFixed(0x0045, 1, L"YEAR"));
	params_fixed.insert(ParamsFixed(0x0046, -1, L"WEEKDAY"));
	params_fixed.insert(ParamsFixed(0x0047, 1, L"HOUR"));
	params_fixed.insert(ParamsFixed(0x0048, 1, L"MINUTE"));
	params_fixed.insert(ParamsFixed(0x0049, 1, L"SECOND"));
	params_fixed.insert(ParamsFixed(0x004A, 0, L"NOW"));
	params_fixed.insert(ParamsFixed(0x004B, 1, L"AREAS"));
	params_fixed.insert(ParamsFixed(0x004C, 1, L"ROWS"));
	params_fixed.insert(ParamsFixed(0x004D, 1, L"COLUMNS"));
	params_fixed.insert(ParamsFixed(0x004E, -1, L"OFFSET"));
	params_fixed.insert(ParamsFixed(0x004F, 2, L"ABSREF"));
	params_fixed.insert(ParamsFixed(0x0050, 2, L"RELREF"));
	params_fixed.insert(ParamsFixed(0x0051, -1, L"ARGUMENT"));
	params_fixed.insert(ParamsFixed(0x0052, -1, L"SEARCH"));
	params_fixed.insert(ParamsFixed(0x0053, 1, L"TRANSPOSE"));
	params_fixed.insert(ParamsFixed(0x0054, -1, L"ERROR"));
	params_fixed.insert(ParamsFixed(0x0055, 0, L"STEP"));
	params_fixed.insert(ParamsFixed(0x0056, 1, L"TYPE"));
	params_fixed.insert(ParamsFixed(0x0057, -1, L"ECHO"));
	params_fixed.insert(ParamsFixed(0x0058, -1, L"SET.NAME"));
	params_fixed.insert(ParamsFixed(0x0059, 0, L"CALLER"));
	params_fixed.insert(ParamsFixed(0x005A, 1, L"DEREF"));
	params_fixed.insert(ParamsFixed(0x005B, -1, L"WINDOWS"));
	params_fixed.insert(ParamsFixed(0x005C, -1, L"SERIES"));
	params_fixed.insert(ParamsFixed(0x005D, -1, L"DOCUMENTS"));
	params_fixed.insert(ParamsFixed(0x005E, 0, L"ACTIVE.CELL"));
	params_fixed.insert(ParamsFixed(0x005F, 0, L"SELECTION"));
	params_fixed.insert(ParamsFixed(0x0060, -1, L"RESULT"));
	params_fixed.insert(ParamsFixed(0x0061, 2, L"ATAN2"));
	params_fixed.insert(ParamsFixed(0x0062, 1, L"ASIN"));
	params_fixed.insert(ParamsFixed(0x0063, 1, L"ACOS"));
	params_fixed.insert(ParamsFixed(0x0064, -1, L"CHOOSE"));
	params_fixed.insert(ParamsFixed(0x0065, -1, L"HLOOKUP"));
	params_fixed.insert(ParamsFixed(0x0066, -1, L"VLOOKUP"));
	params_fixed.insert(ParamsFixed(0x0067, -1, L"LINKS"));
	params_fixed.insert(ParamsFixed(0x0068, -1, L"INPUT"));
	params_fixed.insert(ParamsFixed(0x0069, 1, L"ISREF"));
	params_fixed.insert(ParamsFixed(0x006A, 1, L"GET.FORMULA"));
	params_fixed.insert(ParamsFixed(0x006B, -1, L"GET.NAME"));
	params_fixed.insert(ParamsFixed(0x006C, 2, L"SET.VALUE"));
	params_fixed.insert(ParamsFixed(0x006D, -1, L"LOG"));
	params_fixed.insert(ParamsFixed(0x006E, -1, L"EXEC"));
	params_fixed.insert(ParamsFixed(0x006F, 1, L"CHAR"));
	params_fixed.insert(ParamsFixed(0x0070, 1, L"LOWER"));
	params_fixed.insert(ParamsFixed(0x0071, 1, L"UPPER"));
	params_fixed.insert(ParamsFixed(0x0072, 1, L"PROPER"));
	params_fixed.insert(ParamsFixed(0x0073, -1, L"LEFT"));
	params_fixed.insert(ParamsFixed(0x0074, -1, L"RIGHT"));
	params_fixed.insert(ParamsFixed(0x0075, 2, L"EXACT"));
	params_fixed.insert(ParamsFixed(0x0076, 1, L"TRIM"));
	params_fixed.insert(ParamsFixed(0x0077, 4, L"REPLACE"));
	params_fixed.insert(ParamsFixed(0x0078, -1, L"SUBSTITUTE"));
	params_fixed.insert(ParamsFixed(0x0079, 1, L"CODE"));
	params_fixed.insert(ParamsFixed(0x007A, -1, L"NAMES"));
	params_fixed.insert(ParamsFixed(0x007B, -1, L"DIRECTORY"));
	params_fixed.insert(ParamsFixed(0x007C, -1, L"FIND"));
	params_fixed.insert(ParamsFixed(0x007D, -1, L"CELL"));
	params_fixed.insert(ParamsFixed(0x007E, 1, L"ISERR"));
	params_fixed.insert(ParamsFixed(0x007F, 1, L"ISTEXT"));
	params_fixed.insert(ParamsFixed(0x0080, 1, L"ISNUMBER"));
	params_fixed.insert(ParamsFixed(0x0081, 1, L"ISBLANK"));
	params_fixed.insert(ParamsFixed(0x0082, 1, L"T"));
	params_fixed.insert(ParamsFixed(0x0083, 1, L"N"));
	params_fixed.insert(ParamsFixed(0x0084, -1, L"FOPEN"));
	params_fixed.insert(ParamsFixed(0x0085, 1, L"FCLOSE"));
	params_fixed.insert(ParamsFixed(0x0086, 1, L"FSIZE"));
	params_fixed.insert(ParamsFixed(0x0087, 1, L"FREADLN"));
	params_fixed.insert(ParamsFixed(0x0088, 2, L"FREAD"));
	params_fixed.insert(ParamsFixed(0x0089, 2, L"FWRITELN"));
	params_fixed.insert(ParamsFixed(0x008A, 2, L"FWRITE"));
	params_fixed.insert(ParamsFixed(0x008B, -1, L"FPOS"));
	params_fixed.insert(ParamsFixed(0x008C, 1, L"DATEVALUE"));
	params_fixed.insert(ParamsFixed(0x008D, 1, L"TIMEVALUE"));
	params_fixed.insert(ParamsFixed(0x008E, 3, L"SLN"));
	params_fixed.insert(ParamsFixed(0x008F, 4, L"SYD"));
	params_fixed.insert(ParamsFixed(0x0090, -1, L"DDB"));
	params_fixed.insert(ParamsFixed(0x0091, -1, L"GET.DEF"));
	params_fixed.insert(ParamsFixed(0x0092, -1, L"REFTEXT"));
	params_fixed.insert(ParamsFixed(0x0093, -1, L"TEXTREF"));
	params_fixed.insert(ParamsFixed(0x0094, -1, L"INDIRECT"));
	params_fixed.insert(ParamsFixed(0x0095, -1, L"REGISTER"));
	params_fixed.insert(ParamsFixed(0x0096, -1, L"CALL"));
	params_fixed.insert(ParamsFixed(0x0097, -1, L"ADD.BAR"));
	params_fixed.insert(ParamsFixed(0x0098, -1, L"ADD.MENU"));
	params_fixed.insert(ParamsFixed(0x0099, -1, L"ADD.COMMAND"));
	params_fixed.insert(ParamsFixed(0x009A, -1, L"ENABLE.COMMAND"));
	params_fixed.insert(ParamsFixed(0x009B, -1, L"CHECK.COMMAND"));
	params_fixed.insert(ParamsFixed(0x009C, -1, L"RENAME.COMMAND"));
	params_fixed.insert(ParamsFixed(0x009D, -1, L"SHOW.BAR"));
	params_fixed.insert(ParamsFixed(0x009E, -1, L"DELETE.MENU"));
	params_fixed.insert(ParamsFixed(0x009F, -1, L"DELETE.COMMAND"));
	params_fixed.insert(ParamsFixed(0x00A0, -1, L"GET.CHART.ITEM"));
	params_fixed.insert(ParamsFixed(0x00A1, 1, L"DIALOG.BOX"));
	params_fixed.insert(ParamsFixed(0x00A2, 1, L"CLEAN"));
	params_fixed.insert(ParamsFixed(0x00A3, 1, L"MDETERM"));
	params_fixed.insert(ParamsFixed(0x00A4, 1, L"MINVERSE"));
	params_fixed.insert(ParamsFixed(0x00A5, 2, L"MMULT"));
	params_fixed.insert(ParamsFixed(0x00A6, -1, L"FILES"));
	params_fixed.insert(ParamsFixed(0x00A7, -1, L"IPMT"));
	params_fixed.insert(ParamsFixed(0x00A8, -1, L"PPMT"));
	params_fixed.insert(ParamsFixed(0x00A9, -1, L"COUNTA"));
	params_fixed.insert(ParamsFixed(0x00AA, -1, L"CANCEL.KEY"));
	params_fixed.insert(ParamsFixed(0x00AB, -1, L"FOR"));
	params_fixed.insert(ParamsFixed(0x00AC, 1, L"WHILE"));
	params_fixed.insert(ParamsFixed(0x00AD, 0, L"BREAK"));
	params_fixed.insert(ParamsFixed(0x00AE, 0, L"NEXT"));
	params_fixed.insert(ParamsFixed(0x00AF, 2, L"INITIATE"));
	params_fixed.insert(ParamsFixed(0x00B0, 2, L"REQUEST"));
	params_fixed.insert(ParamsFixed(0x00B1, 3, L"POKE"));
	params_fixed.insert(ParamsFixed(0x00B2, 2, L"EXECUTE"));
	params_fixed.insert(ParamsFixed(0x00B3, 1, L"TERMINATE"));
	params_fixed.insert(ParamsFixed(0x00B4, -1, L"RESTART"));
	params_fixed.insert(ParamsFixed(0x00B5, -1, L"HELP"));
	params_fixed.insert(ParamsFixed(0x00B6, -1, L"GET.BAR"));
	params_fixed.insert(ParamsFixed(0x00B7, -1, L"PRODUCT"));
	params_fixed.insert(ParamsFixed(0x00B8, 1, L"FACT"));
	params_fixed.insert(ParamsFixed(0x00B9, -1, L"GET.CELL"));
	params_fixed.insert(ParamsFixed(0x00BA, 1, L"GET.WORKSPACE"));
	params_fixed.insert(ParamsFixed(0x00BB, -1, L"GET.WINDOW"));
	params_fixed.insert(ParamsFixed(0x00BC, -1, L"GET.DOCUMENT"));
	params_fixed.insert(ParamsFixed(0x00BD, 3, L"DPRODUCT"));
	params_fixed.insert(ParamsFixed(0x00BE, 1, L"ISNONTEXT"));
	params_fixed.insert(ParamsFixed(0x00BF, -1, L"GET.NOTE"));
	params_fixed.insert(ParamsFixed(0x00C0, -1, L"NOTE"));
	params_fixed.insert(ParamsFixed(0x00C1, -1, L"STDEVP"));
	params_fixed.insert(ParamsFixed(0x00C2, -1, L"VARP"));
	params_fixed.insert(ParamsFixed(0x00C3, 3, L"DSTDEVP"));
	params_fixed.insert(ParamsFixed(0x00C4, 3, L"DVARP"));
	params_fixed.insert(ParamsFixed(0x00C5, -1, L"TRUNC"));
	params_fixed.insert(ParamsFixed(0x00C6, 1, L"ISLOGICAL"));
	params_fixed.insert(ParamsFixed(0x00C7, 3, L"DCOUNTA"));
	params_fixed.insert(ParamsFixed(0x00C8, 1, L"DELETE.BAR"));
	params_fixed.insert(ParamsFixed(0x00C9, 1, L"UNREGISTER"));
	params_fixed.insert(ParamsFixed(0x00CC, -1, L"USDOLLAR"));
	params_fixed.insert(ParamsFixed(0x00CD, -1, L"FINDB"));
	params_fixed.insert(ParamsFixed(0x00CE, -1, L"SEARCHB"));
	params_fixed.insert(ParamsFixed(0x00CF, 4, L"REPLACEB"));
	params_fixed.insert(ParamsFixed(0x00D0, -1, L"LEFTB"));
	params_fixed.insert(ParamsFixed(0x00D1, -1, L"RIGHTB"));
	params_fixed.insert(ParamsFixed(0x00D2, 3, L"MIDB"));
	params_fixed.insert(ParamsFixed(0x00D3, 1, L"LENB"));
	params_fixed.insert(ParamsFixed(0x00D4, 2, L"ROUNDUP"));
	params_fixed.insert(ParamsFixed(0x00D5, 2, L"ROUNDDOWN"));
	params_fixed.insert(ParamsFixed(0x00D6, 1, L"ASC"));
	params_fixed.insert(ParamsFixed(0x00D7, 1, L"DBCS"));
	params_fixed.insert(ParamsFixed(0x00D8, -1, L"RANK"));
	params_fixed.insert(ParamsFixed(0x00DB, -1, L"ADDRESS"));
	params_fixed.insert(ParamsFixed(0x00DC, -1, L"DAYS360"));
	params_fixed.insert(ParamsFixed(0x00DD, 0, L"TODAY"));
	params_fixed.insert(ParamsFixed(0x00DE, -1, L"VDB"));
	params_fixed.insert(ParamsFixed(0x00DF, 0, L"ELSE"));
	params_fixed.insert(ParamsFixed(0x00E0, 1, L"ELSE.IF"));
	params_fixed.insert(ParamsFixed(0x00E1, 0, L"END.IF"));
	params_fixed.insert(ParamsFixed(0x00E2, -1, L"FOR.CELL"));
	params_fixed.insert(ParamsFixed(0x00E3, -1, L"MEDIAN"));
	params_fixed.insert(ParamsFixed(0x00E4, -1, L"SUMPRODUCT"));
	params_fixed.insert(ParamsFixed(0x00E5, 1, L"SINH"));
	params_fixed.insert(ParamsFixed(0x00E6, 1, L"COSH"));
	params_fixed.insert(ParamsFixed(0x00E7, 1, L"TANH"));
	params_fixed.insert(ParamsFixed(0x00E8, 1, L"ASINH"));
	params_fixed.insert(ParamsFixed(0x00E9, 1, L"ACOSH"));
	params_fixed.insert(ParamsFixed(0x00EA, 1, L"ATANH"));
	params_fixed.insert(ParamsFixed(0x00EB, 3, L"DGET"));
	params_fixed.insert(ParamsFixed(0x00EC, -1, L"CREATE.OBJECT"));
	params_fixed.insert(ParamsFixed(0x00ED, -1, L"VOLATILE"));
	params_fixed.insert(ParamsFixed(0x00EE, 0, L"LAST.ERROR"));
	params_fixed.insert(ParamsFixed(0x00EF, -1, L"CUSTOM.UNDO"));
	params_fixed.insert(ParamsFixed(0x00F0, -1, L"CUSTOM.REPEAT"));
	params_fixed.insert(ParamsFixed(0x00F1, -1, L"FORMULA.CONVERT"));
	params_fixed.insert(ParamsFixed(0x00F2, -1, L"GET.LINK.INFO"));
	params_fixed.insert(ParamsFixed(0x00F3, -1, L"TEXT.BOX"));
	params_fixed.insert(ParamsFixed(0x00F4, 1, L"INFO"));
	params_fixed.insert(ParamsFixed(0x00F5, 0, L"GROUP"));
	params_fixed.insert(ParamsFixed(0x00F6, -1, L"GET.OBJECT"));
	params_fixed.insert(ParamsFixed(0x00F7, -1, L"DB"));
	params_fixed.insert(ParamsFixed(0x00F8, -1, L"PAUSE"));
	params_fixed.insert(ParamsFixed(0x00FB, -1, L"RESUME"));
	params_fixed.insert(ParamsFixed(0x00FC, 2, L"FREQUENCY"));
	params_fixed.insert(ParamsFixed(0x00FD, -1, L"ADD.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x00FE, 1, L"DELETE.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x00FF, -1, L"USER_DEFINED_FUNCTION"));
	params_fixed.insert(ParamsFixed(0x0100, 1, L"RESET.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x0101, 1, L"EVALUATE"));
	params_fixed.insert(ParamsFixed(0x0102, -1, L"GET.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x0103, -1, L"GET.TOOL"));
	params_fixed.insert(ParamsFixed(0x0104, -1, L"SPELLING.CHECK"));
	params_fixed.insert(ParamsFixed(0x0105, 1, L"ERROR.TYPE"));
	params_fixed.insert(ParamsFixed(0x0106, -1, L"APP.TITLE"));
	params_fixed.insert(ParamsFixed(0x0107, -1, L"WINDOW.TITLE"));
	params_fixed.insert(ParamsFixed(0x0108, -1, L"SAVE.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x0109, 3, L"ENABLE.TOOL"));
	params_fixed.insert(ParamsFixed(0x010A, 3, L"PRESS.TOOL"));
	params_fixed.insert(ParamsFixed(0x010B, -1, L"REGISTER.ID"));
	params_fixed.insert(ParamsFixed(0x010C, -1, L"GET.WORKBOOK"));
	params_fixed.insert(ParamsFixed(0x010D, -1, L"AVEDEV"));
	params_fixed.insert(ParamsFixed(0x010E, -1, L"BETADIST"));
	params_fixed.insert(ParamsFixed(0x010F, 1, L"GAMMALN"));
	params_fixed.insert(ParamsFixed(0x0110, -1, L"BETAINV"));
	params_fixed.insert(ParamsFixed(0x0111, 4, L"BINOMDIST"));
	params_fixed.insert(ParamsFixed(0x0112, 2, L"CHIDIST"));
	params_fixed.insert(ParamsFixed(0x0113, 2, L"CHIINV"));
	params_fixed.insert(ParamsFixed(0x0114, 2, L"COMBIN"));
	params_fixed.insert(ParamsFixed(0x0115, 3, L"CONFIDENCE"));
	params_fixed.insert(ParamsFixed(0x0116, 3, L"CRITBINOM"));
	params_fixed.insert(ParamsFixed(0x0117, 1, L"EVEN"));
	params_fixed.insert(ParamsFixed(0x0118, 3, L"EXPONDIST"));
	params_fixed.insert(ParamsFixed(0x0119, 3, L"FDIST"));
	params_fixed.insert(ParamsFixed(0x011A, 3, L"FINV"));
	params_fixed.insert(ParamsFixed(0x011B, 1, L"FISHER"));
	params_fixed.insert(ParamsFixed(0x011C, 1, L"FISHERINV"));
	params_fixed.insert(ParamsFixed(0x011D, 2, L"FLOOR"));
	params_fixed.insert(ParamsFixed(0x011E, 4, L"GAMMADIST"));
	params_fixed.insert(ParamsFixed(0x011F, 3, L"GAMMAINV"));
	params_fixed.insert(ParamsFixed(0x0120, 2, L"CEILING"));
	params_fixed.insert(ParamsFixed(0x0121, 4, L"HYPGEOMDIST"));
	params_fixed.insert(ParamsFixed(0x0122, 3, L"LOGNORMDIST"));
	params_fixed.insert(ParamsFixed(0x0123, 3, L"LOGINV"));
	params_fixed.insert(ParamsFixed(0x0124, 3, L"NEGBINOMDIST"));
	params_fixed.insert(ParamsFixed(0x0125, 4, L"NORMDIST"));
	params_fixed.insert(ParamsFixed(0x0126, 1, L"NORMSDIST"));
	params_fixed.insert(ParamsFixed(0x0127, 3, L"NORMINV"));
	params_fixed.insert(ParamsFixed(0x0128, 1, L"NORMSINV"));
	params_fixed.insert(ParamsFixed(0x0129, 3, L"STANDARDIZE"));
	params_fixed.insert(ParamsFixed(0x012A, 1, L"ODD"));
	params_fixed.insert(ParamsFixed(0x012B, 2, L"PERMUT"));
	params_fixed.insert(ParamsFixed(0x012C, 3, L"POISSON"));
	params_fixed.insert(ParamsFixed(0x012D, 3, L"TDIST"));
	params_fixed.insert(ParamsFixed(0x012E, 4, L"WEIBULL"));
	params_fixed.insert(ParamsFixed(0x012F, 2, L"SUMXMY2"));
	params_fixed.insert(ParamsFixed(0x0130, 2, L"SUMX2MY2"));
	params_fixed.insert(ParamsFixed(0x0131, 2, L"SUMX2PY2"));
	params_fixed.insert(ParamsFixed(0x0132, 2, L"CHITEST"));
	params_fixed.insert(ParamsFixed(0x0133, 2, L"CORREL"));
	params_fixed.insert(ParamsFixed(0x0134, 2, L"COVAR"));
	params_fixed.insert(ParamsFixed(0x0135, 3, L"FORECAST"));
	params_fixed.insert(ParamsFixed(0x0136, 2, L"FTEST"));
	params_fixed.insert(ParamsFixed(0x0137, 2, L"INTERCEPT"));
	params_fixed.insert(ParamsFixed(0x0138, 2, L"PEARSON"));
	params_fixed.insert(ParamsFixed(0x0139, 2, L"RSQ"));
	params_fixed.insert(ParamsFixed(0x013A, 2, L"STEYX"));
	params_fixed.insert(ParamsFixed(0x013B, 2, L"SLOPE"));
	params_fixed.insert(ParamsFixed(0x013C, 4, L"TTEST"));
	params_fixed.insert(ParamsFixed(0x013D, -1, L"PROB"));
	params_fixed.insert(ParamsFixed(0x013E, -1, L"DEVSQ"));
	params_fixed.insert(ParamsFixed(0x013F, -1, L"GEOMEAN"));
	params_fixed.insert(ParamsFixed(0x0140, -1, L"HARMEAN"));
	params_fixed.insert(ParamsFixed(0x0141, -1, L"SUMSQ"));
	params_fixed.insert(ParamsFixed(0x0142, -1, L"KURT"));
	params_fixed.insert(ParamsFixed(0x0143, -1, L"SKEW"));
	params_fixed.insert(ParamsFixed(0x0144, -1, L"ZTEST"));
	params_fixed.insert(ParamsFixed(0x0145, 2, L"LARGE"));
	params_fixed.insert(ParamsFixed(0x0146, 2, L"SMALL"));
	params_fixed.insert(ParamsFixed(0x0147, 2, L"QUARTILE"));
	params_fixed.insert(ParamsFixed(0x0148, 2, L"PERCENTILE"));
	params_fixed.insert(ParamsFixed(0x0149, -1, L"PERCENTRANK"));
	params_fixed.insert(ParamsFixed(0x014A, -1, L"MODE"));
	params_fixed.insert(ParamsFixed(0x014B, 2, L"TRIMMEAN"));
	params_fixed.insert(ParamsFixed(0x014C, 2, L"TINV"));
	params_fixed.insert(ParamsFixed(0x014E, -1, L"MOVIE.COMMAND"));
	params_fixed.insert(ParamsFixed(0x014F, -1, L"GET.MOVIE"));
	params_fixed.insert(ParamsFixed(0x0150, -1, L"CONCATENATE"));
	params_fixed.insert(ParamsFixed(0x0151, 2, L"POWER"));
	params_fixed.insert(ParamsFixed(0x0152, -1, L"PIVOT.ADD.DATA"));
	params_fixed.insert(ParamsFixed(0x0153, -1, L"GET.PIVOT.TABLE"));
	params_fixed.insert(ParamsFixed(0x0154, -1, L"GET.PIVOT.FIELD"));
	params_fixed.insert(ParamsFixed(0x0155, -1, L"GET.PIVOT.ITEM"));
	params_fixed.insert(ParamsFixed(0x0156, 1, L"RADIANS"));
	params_fixed.insert(ParamsFixed(0x0157, 1, L"DEGREES"));
	params_fixed.insert(ParamsFixed(0x0158, -1, L"SUBTOTAL"));
	params_fixed.insert(ParamsFixed(0x0159, -1, L"SUMIF"));
	params_fixed.insert(ParamsFixed(0x015A, 2, L"COUNTIF"));
	params_fixed.insert(ParamsFixed(0x015B, 1, L"COUNTBLANK"));
	params_fixed.insert(ParamsFixed(0x015C, -1, L"SCENARIO.GET"));
	params_fixed.insert(ParamsFixed(0x015D, 1, L"OPTIONS.LISTS.GET"));
	params_fixed.insert(ParamsFixed(0x015E, 4, L"ISPMT"));
	params_fixed.insert(ParamsFixed(0x015F, 3, L"DATEDIF"));
	params_fixed.insert(ParamsFixed(0x0160, 1, L"DATESTRING"));
	params_fixed.insert(ParamsFixed(0x0161, 2, L"NUMBERSTRING"));
	params_fixed.insert(ParamsFixed(0x0162, -1, L"ROMAN"));
	params_fixed.insert(ParamsFixed(0x0163, -1, L"OPEN.DIALOG"));
	params_fixed.insert(ParamsFixed(0x0164, -1, L"SAVE.DIALOG"));
	params_fixed.insert(ParamsFixed(0x0165, -1, L"VIEW.GET"));
	params_fixed.insert(ParamsFixed(0x0166, -1, L"GETPIVOTDATA"));
	params_fixed.insert(ParamsFixed(0x0167, -1, L"HYPERLINK"));
	params_fixed.insert(ParamsFixed(0x0168, 1, L"PHONETIC"));
	params_fixed.insert(ParamsFixed(0x0169, -1, L"AVERAGEA"));
	params_fixed.insert(ParamsFixed(0x016A, -1, L"MAXA"));
	params_fixed.insert(ParamsFixed(0x016B, -1, L"MINA"));
	params_fixed.insert(ParamsFixed(0x016C, -1, L"STDEVPA"));
	params_fixed.insert(ParamsFixed(0x016D, -1, L"VARPA"));
	params_fixed.insert(ParamsFixed(0x016E, -1, L"STDEVA"));
	params_fixed.insert(ParamsFixed(0x016F, -1, L"VARA"));
	params_fixed.insert(ParamsFixed(0x0170, 1, L"BAHTTEXT"));
	params_fixed.insert(ParamsFixed(0x0171, 1, L"THAIDAYOFWEEK"));
	params_fixed.insert(ParamsFixed(0x0172, 1, L"THAIDIGIT"));
	params_fixed.insert(ParamsFixed(0x0173, 1, L"THAIMONTHOFYEAR"));
	params_fixed.insert(ParamsFixed(0x0174, 1, L"THAINUMSOUND"));
	params_fixed.insert(ParamsFixed(0x0175, 1, L"THAINUMSTRING"));
	params_fixed.insert(ParamsFixed(0x0176, 1, L"THAISTRINGLENGTH"));
	params_fixed.insert(ParamsFixed(0x0177, 1, L"ISTHAIDIGIT"));
	params_fixed.insert(ParamsFixed(0x0178, 1, L"ROUNDBAHTDOWN"));
	params_fixed.insert(ParamsFixed(0x0179, 1, L"ROUNDBAHTUP"));
	params_fixed.insert(ParamsFixed(0x017A, 1, L"THAIYEAR"));
	params_fixed.insert(ParamsFixed(0x017B, -1, L"RTD"));
    params_fixed.insert(ParamsFixed(0x017C, -1, L"CUBEVALUE"));
    params_fixed.insert(ParamsFixed(0x017D, -1, L"CUBEMEMBER"));
    params_fixed.insert(ParamsFixed(0x017E, -1, L"CUBEMEMBERPROPERTY"));
    params_fixed.insert(ParamsFixed(0x017F, -1, L"CUBERANKEDMEMBER"));
    params_fixed.insert(ParamsFixed(0x0180, 1, L"HEX2BIN"));
    params_fixed.insert(ParamsFixed(0x0181, 1, L"HEX2DEC"));
    params_fixed.insert(ParamsFixed(0x0182, 1, L"HEX2OCT"));
    params_fixed.insert(ParamsFixed(0x0183, 1, L"DEC2BIN"));
    params_fixed.insert(ParamsFixed(0x0184, 1, L"DEC2HEX"));
    params_fixed.insert(ParamsFixed(0x0185, 1, L"DEC2OCT"));
    params_fixed.insert(ParamsFixed(0x0186, 1, L"OCT2BIN"));
    params_fixed.insert(ParamsFixed(0x0187, 1, L"OCT2HEX"));
    params_fixed.insert(ParamsFixed(0x0188, 1, L"OCT2DEC"));
    params_fixed.insert(ParamsFixed(0x0189, 1, L"BIN2DEC"));
    params_fixed.insert(ParamsFixed(0x018A, 1, L"BIN2OCT"));
    params_fixed.insert(ParamsFixed(0x018B, 1, L"BIN2HEX"));
    params_fixed.insert(ParamsFixed(0x018C, 2, L"IMSUB"));
    params_fixed.insert(ParamsFixed(0x018D, 2, L"IMDIV"));
    params_fixed.insert(ParamsFixed(0x018E, 2, L"IMPOWER"));
    params_fixed.insert(ParamsFixed(0x018F, 1, L"IMABS"));
    params_fixed.insert(ParamsFixed(0x0190, 1, L"IMSQRT"));
    params_fixed.insert(ParamsFixed(0x0191, 1, L"IMLN"));
    params_fixed.insert(ParamsFixed(0x0192, 1, L"IMLOG2"));
    params_fixed.insert(ParamsFixed(0x0193, 1, L"IMLOG10"));
    params_fixed.insert(ParamsFixed(0x0194, 1, L"IMSIN"));
    params_fixed.insert(ParamsFixed(0x0195, 1, L"IMCOS"));
    params_fixed.insert(ParamsFixed(0x0196, 1, L"IMEXP"));
    params_fixed.insert(ParamsFixed(0x0197, 1, L"IMARGUMENT"));
    params_fixed.insert(ParamsFixed(0x0198, 1, L"IMCONJUGATE"));
    params_fixed.insert(ParamsFixed(0x0199, 1, L"IMAGINARY"));
    params_fixed.insert(ParamsFixed(0x019A, 1, L"IMREAL"));
    params_fixed.insert(ParamsFixed(0x019B, 2, L"COMPLEX"));
    params_fixed.insert(ParamsFixed(0x019C, -1, L"IMSUM"));
    params_fixed.insert(ParamsFixed(0x019D, -1, L"IMPRODUCT"));
    params_fixed.insert(ParamsFixed(0x019E, 4, L"SERIESSUM"));
    params_fixed.insert(ParamsFixed(0x019F, 1, L"FACTDOUBLE"));
    params_fixed.insert(ParamsFixed(0x01A0, 1, L"SQRTPI"));
    params_fixed.insert(ParamsFixed(0x01A1, 2, L"QUOTIENT"));
    params_fixed.insert(ParamsFixed(0x01A2, 1, L"DELTA"));
    params_fixed.insert(ParamsFixed(0x01A3, 1, L"GESTEP"));
    params_fixed.insert(ParamsFixed(0x01A4, 1, L"ISEVEN"));
    params_fixed.insert(ParamsFixed(0x01A5, 1, L"ISODD"));
    params_fixed.insert(ParamsFixed(0x01A6, 2, L"MROUND"));
    params_fixed.insert(ParamsFixed(0x01A7, 1, L"ERF"));
    params_fixed.insert(ParamsFixed(0x01A8, 1, L"ERFC"));
    params_fixed.insert(ParamsFixed(0x01A9, 2, L"BESSELJ"));
    params_fixed.insert(ParamsFixed(0x01AA, 2, L"BESSELK"));
    params_fixed.insert(ParamsFixed(0x01AB, 2, L"BESSELY"));
    params_fixed.insert(ParamsFixed(0x01AC, 2, L"BESSELI"));
    params_fixed.insert(ParamsFixed(0x01AD, 1, L"XIRR"));
    params_fixed.insert(ParamsFixed(0x01AE, 3, L"XNPV"));
    params_fixed.insert(ParamsFixed(0x01AF, 5, L"PRICEMAT"));
    params_fixed.insert(ParamsFixed(0x01B0, 5, L"YIELDMAT"));
    params_fixed.insert(ParamsFixed(0x01B1, 4, L"INTRATE"));
    params_fixed.insert(ParamsFixed(0x01B2, 4, L"RECEIVED"));
    params_fixed.insert(ParamsFixed(0x01B3, 4, L"DISC"));
    params_fixed.insert(ParamsFixed(0x01B4, 4, L"PRICEDISC"));
    params_fixed.insert(ParamsFixed(0x01B5, 4, L"YIELDDISC"));
    params_fixed.insert(ParamsFixed(0x01B6, 3, L"TBILLEQ"));
    params_fixed.insert(ParamsFixed(0x01B7, 3, L"TBILLPRICE"));
    params_fixed.insert(ParamsFixed(0x01B8, 3, L"TBILLYIELD"));
    params_fixed.insert(ParamsFixed(0x01B9, 6, L"PRICE"));
    params_fixed.insert(ParamsFixed(0x01BA, 6, L"YIELD"));
    params_fixed.insert(ParamsFixed(0x01BB, 2, L"DOLLARDE"));
    params_fixed.insert(ParamsFixed(0x01BC, 2, L"DOLLARFR"));
    params_fixed.insert(ParamsFixed(0x01BD, 2, L"NOMINAL"));
    params_fixed.insert(ParamsFixed(0x01BE, 2, L"EFFECT"));
    params_fixed.insert(ParamsFixed(0x01BF, 6, L"CUMPRINC"));
    params_fixed.insert(ParamsFixed(0x01C0, 6, L"CUMIPMT"));
    params_fixed.insert(ParamsFixed(0x01C1, 2, L"EDATE"));
    params_fixed.insert(ParamsFixed(0x01C2, 2, L"EOMONTH"));
    params_fixed.insert(ParamsFixed(0x01C3, 2, L"YEARFRAC"));
    params_fixed.insert(ParamsFixed(0x01C4, 3, L"COUPDAYBS"));
    params_fixed.insert(ParamsFixed(0x01C5, 3, L"COUPDAYS"));
    params_fixed.insert(ParamsFixed(0x01C6, 3, L"COUPDAYSNC"));
    params_fixed.insert(ParamsFixed(0x01C7, 3, L"COUPNCD"));
    params_fixed.insert(ParamsFixed(0x01C8, 3, L"COUPNUM"));
    params_fixed.insert(ParamsFixed(0x01C9, 3, L"COUPPCD"));
    params_fixed.insert(ParamsFixed(0x01CA, 5, L"DURATION"));
    params_fixed.insert(ParamsFixed(0x01CB, 5, L"MDURATION"));
    params_fixed.insert(ParamsFixed(0x01CC, 7, L"ODDLPRICE"));
    params_fixed.insert(ParamsFixed(0x01CD, 7, L"ODDLYIELD"));
    params_fixed.insert(ParamsFixed(0x01CE, 8, L"ODDFPRICE"));
    params_fixed.insert(ParamsFixed(0x01CF, 8, L"ODDFYIELD"));
    params_fixed.insert(ParamsFixed(0x01D0, 2, L"RANDBETWEEN"));
    params_fixed.insert(ParamsFixed(0x01D1, 1, L"WEEKNUM"));
    params_fixed.insert(ParamsFixed(0x01D2, 6, L"AMORDEGRC"));
    params_fixed.insert(ParamsFixed(0x01D3, 6, L"AMORLINC"));
    params_fixed.insert(ParamsFixed(0x01D5, 6, L"ACCRINT"));
    params_fixed.insert(ParamsFixed(0x01D6, 4, L"ACCRINTM"));
    params_fixed.insert(ParamsFixed(0x01D7, 2, L"WORKDAY"));
    params_fixed.insert(ParamsFixed(0x01D8, 2, L"NETWORKDAYS"));
    params_fixed.insert(ParamsFixed(0x01D9, 1, L"GCD"));
    params_fixed.insert(ParamsFixed(0x01DA, 1, L"MULTINOMIAL"));
    params_fixed.insert(ParamsFixed(0x01DB, 1, L"LCM"));
    params_fixed.insert(ParamsFixed(0x01DC, 2, L"FVSCHEDULE"));
    params_fixed.insert(ParamsFixed(0x01DD, 3, L"CUBEKPIMEMBER"));
    params_fixed.insert(ParamsFixed(0x01DE, 1, L"CUBESET"));
    params_fixed.insert(ParamsFixed(0x01DF, 1, L"CUBESETCOUNT"));
    params_fixed.insert(ParamsFixed(0x01E0, 2, L"IFERROR"));
    params_fixed.insert(ParamsFixed(0x01E1, 2, L"COUNTIFS"));
    params_fixed.insert(ParamsFixed(0x01E2, 3, L"SUMIFS"));
    params_fixed.insert(ParamsFixed(0x01E3, 2, L"AVERAGEIF"));
    params_fixed.insert(ParamsFixed(0x01E4, 3, L"AVERAGEIFS"));

// Cetab values
	params_fixed.insert(ParamsFixed(0x8000, -1, L"BEEP"));
	params_fixed.insert(ParamsFixed(0x8001, -1, L"OPEN"));
	params_fixed.insert(ParamsFixed(0x8002, -1, L"OPEN.LINKS"));
	params_fixed.insert(ParamsFixed(0x8003, 0, L"CLOSE.ALL"));
	params_fixed.insert(ParamsFixed(0x8004, 0, L"SAVE"));
	params_fixed.insert(ParamsFixed(0x8005, -1, L"SAVE.AS"));
	params_fixed.insert(ParamsFixed(0x8006, -1, L"FILE.DELETE"));
	params_fixed.insert(ParamsFixed(0x8007, -1, L"PAGE.SETUP"));
	params_fixed.insert(ParamsFixed(0x8008, -1, L"PRINT"));
	params_fixed.insert(ParamsFixed(0x8009, -1, L"PRINTER.SETUP"));
	params_fixed.insert(ParamsFixed(0x800A, 0, L"QUIT"));
	params_fixed.insert(ParamsFixed(0x800B, 0, L"NEW.WINDOW"));
	params_fixed.insert(ParamsFixed(0x800D, -1, L"WINDOW.SIZE"));
	params_fixed.insert(ParamsFixed(0x800E, -1, L"WINDOW.MOVE"));
	params_fixed.insert(ParamsFixed(0x800F, -1, L"FULL"));
	params_fixed.insert(ParamsFixed(0x8010, -1, L"CLOSE"));
	params_fixed.insert(ParamsFixed(0x8011, -1, L"RUN"));
	params_fixed.insert(ParamsFixed(0x8016, -1, L"SET.PRINT.AREA"));
	params_fixed.insert(ParamsFixed(0x8017, -1, L"SET.PRINT.TITLES"));
	params_fixed.insert(ParamsFixed(0x8018, 0, L"SET.PAGE.BREAK"));
	params_fixed.insert(ParamsFixed(0x8019, -1, L"REMOVE.PAGE.BREAK"));
	params_fixed.insert(ParamsFixed(0x801A, -1, L"FONT"));
	params_fixed.insert(ParamsFixed(0x801B, -1, L"DISPLAY"));
	params_fixed.insert(ParamsFixed(0x801C, -1, L"PROTECT.DOCUMENT"));
	params_fixed.insert(ParamsFixed(0x801D, -1, L"PRECISION"));
	params_fixed.insert(ParamsFixed(0x801E, -1, L"A1.R1C1"));
	params_fixed.insert(ParamsFixed(0x801F, 0, L"CALCULATE.NOW"));
	params_fixed.insert(ParamsFixed(0x8020, -1, L"CALCULATION"));
	params_fixed.insert(ParamsFixed(0x8022, -1, L"DATA.FIND"));
	params_fixed.insert(ParamsFixed(0x8023, -1, L"EXTRACT"));
	params_fixed.insert(ParamsFixed(0x8024, 0, L"DATA.DELETE"));
	params_fixed.insert(ParamsFixed(0x8025, 0, L"SET.DATABASE"));
	params_fixed.insert(ParamsFixed(0x8026, 0, L"SET.CRITERIA"));
	params_fixed.insert(ParamsFixed(0x8027, -1, L"SORT"));
	params_fixed.insert(ParamsFixed(0x8028, -1, L"DATA.SERIES"));
	params_fixed.insert(ParamsFixed(0x8029, -1, L"TABLE"));
	params_fixed.insert(ParamsFixed(0x802A, -1, L"FORMAT.NUMBER"));
	params_fixed.insert(ParamsFixed(0x802B, -1, L"ALIGNMENT"));
	params_fixed.insert(ParamsFixed(0x802C, -1, L"STYLE"));
	params_fixed.insert(ParamsFixed(0x802D, -1, L"BORDER"));
	params_fixed.insert(ParamsFixed(0x802E, -1, L"CELL.PROTECTION"));
	params_fixed.insert(ParamsFixed(0x802F, -1, L"COLUMN.WIDTH"));
	params_fixed.insert(ParamsFixed(0x8030, 0, L"UNDO"));
	params_fixed.insert(ParamsFixed(0x8031, -1, L"CUT"));
	params_fixed.insert(ParamsFixed(0x8032, -1, L"COPY"));
	params_fixed.insert(ParamsFixed(0x8033, -1, L"PASTE"));
	params_fixed.insert(ParamsFixed(0x8034, -1, L"CLEAR"));
	params_fixed.insert(ParamsFixed(0x8035, -1, L"PASTE.SPECIAL"));
	params_fixed.insert(ParamsFixed(0x8036, -1, L"EDIT.DELETE"));
	params_fixed.insert(ParamsFixed(0x8037, -1, L"INSERT"));
	params_fixed.insert(ParamsFixed(0x8038, 0, L"FILL.RIGHT"));
	params_fixed.insert(ParamsFixed(0x8039, 0, L"FILL.DOWN"));
	params_fixed.insert(ParamsFixed(0x803D, -1, L"DEFINE.NAME"));
	params_fixed.insert(ParamsFixed(0x803E, -1, L"CREATE.NAMES"));
	params_fixed.insert(ParamsFixed(0x803F, -1, L"FORMULA.GOTO"));
	params_fixed.insert(ParamsFixed(0x8040, -1, L"FORMULA.FIND"));
	params_fixed.insert(ParamsFixed(0x8041, 0, L"SELECT.LAST.CELL"));
	params_fixed.insert(ParamsFixed(0x8042, 0, L"SHOW.ACTIVE.CELL"));
	params_fixed.insert(ParamsFixed(0x8049, -1, L"COMBINATION"));
	params_fixed.insert(ParamsFixed(0x804A, 0, L"PREFERRED"));
	params_fixed.insert(ParamsFixed(0x804B, 0, L"ADD.OVERLAY"));
	params_fixed.insert(ParamsFixed(0x804C, -1, L"GRIDLINES"));
	params_fixed.insert(ParamsFixed(0x804D, -1, L"SET.PREFERRED"));
	params_fixed.insert(ParamsFixed(0x804E, -1, L"AXES"));
	params_fixed.insert(ParamsFixed(0x804F, -1, L"LEGEND"));
	params_fixed.insert(ParamsFixed(0x8050, -1, L"ATTACH.TEXT"));
	params_fixed.insert(ParamsFixed(0x8051, 0, L"ADD.ARROW"));
	params_fixed.insert(ParamsFixed(0x8052, 0, L"SELECT.CHART"));
	params_fixed.insert(ParamsFixed(0x8053, 0, L"SELECT.PLOT.AREA"));
	params_fixed.insert(ParamsFixed(0x8054, -1, L"PATTERNS"));
	params_fixed.insert(ParamsFixed(0x8055, -1, L"MAIN.CHART"));
	params_fixed.insert(ParamsFixed(0x8056, -1, L"OVERLAY"));
	params_fixed.insert(ParamsFixed(0x8057, -1, L"SCALE"));
	params_fixed.insert(ParamsFixed(0x8058, -1, L"FORMAT.LEGEND"));
	params_fixed.insert(ParamsFixed(0x8059, -1, L"FORMAT.TEXT"));
	params_fixed.insert(ParamsFixed(0x805A, 0, L"EDIT.REPEAT"));
	params_fixed.insert(ParamsFixed(0x805B, -1, L"PARSE"));
	params_fixed.insert(ParamsFixed(0x805C, 0, L"JUSTIFY"));
	params_fixed.insert(ParamsFixed(0x805D, 0, L"HIDE"));
	params_fixed.insert(ParamsFixed(0x805E, -1, L"UNHIDE"));
	params_fixed.insert(ParamsFixed(0x805F, -1, L"WORKSPACE"));
	params_fixed.insert(ParamsFixed(0x8060, -1, L"FORMULA"));
	params_fixed.insert(ParamsFixed(0x8061, -1, L"FORMULA.FILL"));
	params_fixed.insert(ParamsFixed(0x8062, -1, L"FORMULA.ARRAY"));
	params_fixed.insert(ParamsFixed(0x8063, 0, L"DATA.FIND.NEXT"));
	params_fixed.insert(ParamsFixed(0x8064, 0, L"DATA.FIND.PREV"));
	params_fixed.insert(ParamsFixed(0x8065, 0, L"FORMULA.FIND.NEXT"));
	params_fixed.insert(ParamsFixed(0x8066, 0, L"FORMULA.FIND.PREV"));
	params_fixed.insert(ParamsFixed(0x8067, -1, L"ACTIVATE"));
	params_fixed.insert(ParamsFixed(0x8068, -1, L"ACTIVATE.NEXT"));
	params_fixed.insert(ParamsFixed(0x8069, -1, L"ACTIVATE.PREV"));
	params_fixed.insert(ParamsFixed(0x806A, 0, L"UNLOCKED.NEXT"));
	params_fixed.insert(ParamsFixed(0x806B, 0, L"UNLOCKED.PREV"));
	params_fixed.insert(ParamsFixed(0x806C, -1, L"COPY.PICTURE"));
	params_fixed.insert(ParamsFixed(0x806D, -1, L"SELECT"));
	params_fixed.insert(ParamsFixed(0x806E, -1, L"DELETE.NAME"));
	params_fixed.insert(ParamsFixed(0x806F, -1, L"DELETE.FORMAT"));
	params_fixed.insert(ParamsFixed(0x8070, -1, L"VLINE"));
	params_fixed.insert(ParamsFixed(0x8071, -1, L"HLINE"));
	params_fixed.insert(ParamsFixed(0x8072, -1, L"VPAGE"));
	params_fixed.insert(ParamsFixed(0x8073, -1, L"HPAGE"));
	params_fixed.insert(ParamsFixed(0x8074, -1, L"VSCROLL"));
	params_fixed.insert(ParamsFixed(0x8075, -1, L"HSCROLL"));
	params_fixed.insert(ParamsFixed(0x8076, -1, L"ALERT"));
	params_fixed.insert(ParamsFixed(0x8077, -1, L"NEW"));
	params_fixed.insert(ParamsFixed(0x8078, -1, L"CANCEL.COPY"));
	params_fixed.insert(ParamsFixed(0x8079, 0, L"SHOW.CLIPBOARD"));
	params_fixed.insert(ParamsFixed(0x807A, -1, L"MESSAGE"));
	params_fixed.insert(ParamsFixed(0x807C, 0, L"PASTE.LINK"));
	params_fixed.insert(ParamsFixed(0x807D, -1, L"APP.ACTIVATE"));
	params_fixed.insert(ParamsFixed(0x807E, 0, L"DELETE.ARROW"));
	params_fixed.insert(ParamsFixed(0x807F, -1, L"ROW.HEIGHT"));
	params_fixed.insert(ParamsFixed(0x8080, -1, L"FORMAT.MOVE"));
	params_fixed.insert(ParamsFixed(0x8081, -1, L"FORMAT.SIZE"));
	params_fixed.insert(ParamsFixed(0x8082, -1, L"FORMULA.REPLACE"));
	params_fixed.insert(ParamsFixed(0x8083, -1, L"SEND.KEYS"));
	params_fixed.insert(ParamsFixed(0x8084, -1, L"SELECT.SPECIAL"));
	params_fixed.insert(ParamsFixed(0x8085, -1, L"APPLY.NAMES"));
	params_fixed.insert(ParamsFixed(0x8086, -1, L"REPLACE.FONT"));
	params_fixed.insert(ParamsFixed(0x8087, -1, L"FREEZE.PANES"));
	params_fixed.insert(ParamsFixed(0x8088, -1, L"SHOW.INFO"));
	params_fixed.insert(ParamsFixed(0x8089, -1, L"SPLIT"));
	params_fixed.insert(ParamsFixed(0x808A, -1, L"ON.WINDOW"));
	params_fixed.insert(ParamsFixed(0x808B, -1, L"ON.DATA"));
	params_fixed.insert(ParamsFixed(0x808C, -1, L"DISABLE.INPUT"));
	params_fixed.insert(ParamsFixed(0x808E, -1, L"OUTLINE"));
	params_fixed.insert(ParamsFixed(0x808F, 0, L"LIST.NAMES"));
	params_fixed.insert(ParamsFixed(0x8090, -1, L"FILE.CLOSE"));
	params_fixed.insert(ParamsFixed(0x8091, -1, L"SAVE.WORKBOOK"));
	params_fixed.insert(ParamsFixed(0x8092, 0, L"DATA.FORM"));
	params_fixed.insert(ParamsFixed(0x8093, -1, L"COPY.CHART"));
	params_fixed.insert(ParamsFixed(0x8094, -1, L"ON.TIME"));
	params_fixed.insert(ParamsFixed(0x8095, -1, L"WAIT"));
	params_fixed.insert(ParamsFixed(0x8096, -1, L"FORMAT.FONT"));
	params_fixed.insert(ParamsFixed(0x8097, 0, L"FILL.UP"));
	params_fixed.insert(ParamsFixed(0x8098, 0, L"FILL.LEFT"));
	params_fixed.insert(ParamsFixed(0x8099, 0, L"DELETE.OVERLAY"));
	params_fixed.insert(ParamsFixed(0x809B, -1, L"SHORT.MENUS"));
	params_fixed.insert(ParamsFixed(0x809F, -1, L"SET.UPDATE.STATUS"));
	params_fixed.insert(ParamsFixed(0x80A1, -1, L"COLOR.PALETTE"));
	params_fixed.insert(ParamsFixed(0x80A2, -1, L"DELETE.STYLE"));
	params_fixed.insert(ParamsFixed(0x80A3, -1, L"WINDOW.RESTORE"));
	params_fixed.insert(ParamsFixed(0x80A4, -1, L"WINDOW.MAXIMIZE"));
	params_fixed.insert(ParamsFixed(0x80A6, -1, L"CHANGE.LINK"));
	params_fixed.insert(ParamsFixed(0x80A7, 0, L"CALCULATE.DOCUMENT"));
	params_fixed.insert(ParamsFixed(0x80A8, -1, L"ON.KEY"));
	params_fixed.insert(ParamsFixed(0x80A9, 0, L"APP.RESTORE"));
	params_fixed.insert(ParamsFixed(0x80AA, -1, L"APP.MOVE"));
	params_fixed.insert(ParamsFixed(0x80AB, -1, L"APP.SIZE"));
	params_fixed.insert(ParamsFixed(0x80AC, 0, L"APP.MINIMIZE"));
	params_fixed.insert(ParamsFixed(0x80AD, 0, L"APP.MAXIMIZE"));
	params_fixed.insert(ParamsFixed(0x80AE, 0, L"BRING.TO.FRONT"));
	params_fixed.insert(ParamsFixed(0x80AF, 0, L"SEND.TO.BACK"));
	params_fixed.insert(ParamsFixed(0x80B9, -1, L"MAIN.CHART.TYPE"));
	params_fixed.insert(ParamsFixed(0x80BA, -1, L"OVERLAY.CHART.TYPE"));
	params_fixed.insert(ParamsFixed(0x80BB, -1, L"SELECT.END"));
	params_fixed.insert(ParamsFixed(0x80BC, -1, L"OPEN.MAIL"));
	params_fixed.insert(ParamsFixed(0x80BD, -1, L"SEND.MAIL"));
	params_fixed.insert(ParamsFixed(0x80BE, -1, L"STANDARD.FONT"));
	params_fixed.insert(ParamsFixed(0x80BF, -1, L"CONSOLIDATE"));
	params_fixed.insert(ParamsFixed(0x80C0, -1, L"SORT.SPECIAL"));
	params_fixed.insert(ParamsFixed(0x80C5, -1, L"VIEW.3D"));
	params_fixed.insert(ParamsFixed(0x80C6, -1, L"GOAL.SEEK"));
	params_fixed.insert(ParamsFixed(0x80C7, -1, L"WORKGROUP"));
	params_fixed.insert(ParamsFixed(0x80C8, -1, L"FILL.GROUP"));
	params_fixed.insert(ParamsFixed(0x80C9, -1, L"UPDATE.LINK"));
	params_fixed.insert(ParamsFixed(0x80CA, -1, L"PROMOTE"));
	params_fixed.insert(ParamsFixed(0x80CB, -1, L"DEMOTE"));
	params_fixed.insert(ParamsFixed(0x80CC, -1, L"SHOW.DETAIL"));
	params_fixed.insert(ParamsFixed(0x80CE, 0, L"UNGROUP"));
	params_fixed.insert(ParamsFixed(0x80CF, -1, L"OBJECT.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x80D0, -1, L"SAVE.NEW.OBJECT"));
	params_fixed.insert(ParamsFixed(0x80D1, 0, L"SHARE"));
	params_fixed.insert(ParamsFixed(0x80D2, -1, L"SHARE.NAME"));
	params_fixed.insert(ParamsFixed(0x80D3, 0, L"DUPLICATE"));
	params_fixed.insert(ParamsFixed(0x80D4, -1, L"APPLY.STYLE"));
	params_fixed.insert(ParamsFixed(0x80D5, -1, L"ASSIGN.TO.OBJECT"));
	params_fixed.insert(ParamsFixed(0x80D6, -1, L"OBJECT.PROTECTION"));
	params_fixed.insert(ParamsFixed(0x80D7, -1, L"HIDE.OBJECT"));
	params_fixed.insert(ParamsFixed(0x80D8, 0, L"SET.EXTRACT"));
	params_fixed.insert(ParamsFixed(0x80D9, -1, L"CREATE.PUBLISHER"));
	params_fixed.insert(ParamsFixed(0x80DA, -1, L"SUBSCRIBE.TO"));
	params_fixed.insert(ParamsFixed(0x80DB, -1, L"ATTRIBUTES"));
	params_fixed.insert(ParamsFixed(0x80DC, -1, L"SHOW.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x80DE, -1, L"PRINT.PREVIEW"));
	params_fixed.insert(ParamsFixed(0x80DF, -1, L"EDIT.COLOR"));
	params_fixed.insert(ParamsFixed(0x80E0, -1, L"SHOW.LEVELS"));
	params_fixed.insert(ParamsFixed(0x80E1, -1, L"FORMAT.MAIN"));
	params_fixed.insert(ParamsFixed(0x80E2, -1, L"FORMAT.OVERLAY"));
	params_fixed.insert(ParamsFixed(0x80E3, -1, L"ON.RECALC"));
	params_fixed.insert(ParamsFixed(0x80E4, -1, L"EDIT.SERIES"));
	params_fixed.insert(ParamsFixed(0x80E5, -1, L"DEFINE.STYLE"));
	params_fixed.insert(ParamsFixed(0x80F0, -1, L"LINE.PRINT"));
	params_fixed.insert(ParamsFixed(0x80F3, -1, L"ENTER.DATA"));
	params_fixed.insert(ParamsFixed(0x80FA, -1, L"MERGE.STYLES"));
	params_fixed.insert(ParamsFixed(0x80FB, -1, L"EDITION.OPTIONS"));
	params_fixed.insert(ParamsFixed(0x80FC, 0, L"PASTE.PICTURE"));
	params_fixed.insert(ParamsFixed(0x80FD, 0, L"PASTE.PICTURE.LINK"));
	params_fixed.insert(ParamsFixed(0x80FE, -1, L"SPELLING"));
	params_fixed.insert(ParamsFixed(0x8100, -1, L"ZOOM"));
	params_fixed.insert(ParamsFixed(0x8103, -1, L"INSERT.OBJECT"));
	params_fixed.insert(ParamsFixed(0x8104, -1, L"WINDOW.MINIMIZE"));
	params_fixed.insert(ParamsFixed(0x8109, -1, L"SOUND.NOTE"));
	params_fixed.insert(ParamsFixed(0x810A, -1, L"SOUND.PLAY"));
	params_fixed.insert(ParamsFixed(0x810B, -1, L"FORMAT.SHAPE"));
	params_fixed.insert(ParamsFixed(0x810C, -1, L"EXTEND.POLYGON"));
	params_fixed.insert(ParamsFixed(0x810D, -1, L"FORMAT.AUTO"));
	params_fixed.insert(ParamsFixed(0x8112, -1, L"FILL.AUTO"));
	params_fixed.insert(ParamsFixed(0x8114, -1, L"CUSTOMIZE.TOOLBAR"));
	params_fixed.insert(ParamsFixed(0x8115, -1, L"ADD.TOOL"));
	params_fixed.insert(ParamsFixed(0x8116, -1, L"EDIT.OBJECT"));
	params_fixed.insert(ParamsFixed(0x8117, -1, L"ON.DOUBLECLICK"));
	params_fixed.insert(ParamsFixed(0x8118, -1, L"ON.ENTRY"));
	params_fixed.insert(ParamsFixed(0x8119, -1, L"WORKBOOK.ADD"));
	params_fixed.insert(ParamsFixed(0x811A, -1, L"WORKBOOK.MOVE"));
	params_fixed.insert(ParamsFixed(0x811B, -1, L"WORKBOOK.COPY"));
	params_fixed.insert(ParamsFixed(0x811C, -1, L"WORKBOOK.OPTIONS"));
	params_fixed.insert(ParamsFixed(0x811D, -1, L"SAVE.WORKSPACE"));
	params_fixed.insert(ParamsFixed(0x8120, -1, L"CHART.WIZARD"));
	params_fixed.insert(ParamsFixed(0x8121, -1, L"DELETE.TOOL"));
	params_fixed.insert(ParamsFixed(0x8122, -1, L"MOVE.TOOL"));
	params_fixed.insert(ParamsFixed(0x8123, -1, L"WORKBOOK.SELECT"));
	params_fixed.insert(ParamsFixed(0x8124, -1, L"WORKBOOK.ACTIVATE"));
	params_fixed.insert(ParamsFixed(0x8125, -1, L"ASSIGN.TO.TOOL"));
	params_fixed.insert(ParamsFixed(0x8127, -1, L"COPY.TOOL"));
	params_fixed.insert(ParamsFixed(0x8128, -1, L"RESET.TOOL"));
	params_fixed.insert(ParamsFixed(0x8129, -1, L"CONSTRAIN.NUMERIC"));
	params_fixed.insert(ParamsFixed(0x812A, -1, L"PASTE.TOOL"));
	params_fixed.insert(ParamsFixed(0x812E, -1, L"WORKBOOK.NEW"));
	params_fixed.insert(ParamsFixed(0x8131, -1, L"SCENARIO.CELLS"));
	params_fixed.insert(ParamsFixed(0x8132, -1, L"SCENARIO.DELETE"));
	params_fixed.insert(ParamsFixed(0x8133, -1, L"SCENARIO.ADD"));
	params_fixed.insert(ParamsFixed(0x8134, -1, L"SCENARIO.EDIT"));
	params_fixed.insert(ParamsFixed(0x8135, -1, L"SCENARIO.SHOW"));
	params_fixed.insert(ParamsFixed(0x8136, 0, L"SCENARIO.SHOW.NEXT"));
	params_fixed.insert(ParamsFixed(0x8137, -1, L"SCENARIO.SUMMARY"));
	params_fixed.insert(ParamsFixed(0x8138, -1, L"PIVOT.TABLE.WIZARD"));
	params_fixed.insert(ParamsFixed(0x8139, -1, L"PIVOT.FIELD.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x813A, -1, L"PIVOT.FIELD"));
	params_fixed.insert(ParamsFixed(0x813B, -1, L"PIVOT.ITEM"));
	params_fixed.insert(ParamsFixed(0x813C, -1, L"PIVOT.ADD.FIELDS"));
	params_fixed.insert(ParamsFixed(0x813E, -1, L"OPTIONS.CALCULATION"));
	params_fixed.insert(ParamsFixed(0x813F, -1, L"OPTIONS.EDIT"));
	params_fixed.insert(ParamsFixed(0x8140, -1, L"OPTIONS.VIEW"));
	params_fixed.insert(ParamsFixed(0x8141, -1, L"ADDIN.MANAGER"));
	params_fixed.insert(ParamsFixed(0x8142, 0, L"MENU.EDITOR"));
	params_fixed.insert(ParamsFixed(0x8143, 0, L"ATTACH.TOOLBARS"));
	params_fixed.insert(ParamsFixed(0x8144, -1, L"ATTACH.TOOLBARS"));
	params_fixed.insert(ParamsFixed(0x8145, -1, L"OPTIONS.CHART"));
	params_fixed.insert(ParamsFixed(0x8148, -1, L"VBA.INSERT.FILE"));
	params_fixed.insert(ParamsFixed(0x814A, 0, L"VBA.PROCEDURE.DEFINITION"));
	params_fixed.insert(ParamsFixed(0x8150, -1, L"ROUTING.SLIP"));
	params_fixed.insert(ParamsFixed(0x8152, 0, L"ROUTE.DOCUMENT"));
	params_fixed.insert(ParamsFixed(0x8153, -1, L"MAIL.LOGON"));
	params_fixed.insert(ParamsFixed(0x8156, -1, L"INSERT.PICTURE"));
	params_fixed.insert(ParamsFixed(0x8157, -1, L"EDIT.TOOL"));
	params_fixed.insert(ParamsFixed(0x815E, -1, L"CHART.TREND"));
	params_fixed.insert(ParamsFixed(0x8160, -1, L"PIVOT.ITEM.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x8162, -1, L"WORKBOOK.INSERT"));
	params_fixed.insert(ParamsFixed(0x8163, -1, L"OPTIONS.TRANSITION"));
	params_fixed.insert(ParamsFixed(0x8164, -1, L"OPTIONS.GENERAL"));
	params_fixed.insert(ParamsFixed(0x8172, -1, L"FILTER.ADVANCED"));
	params_fixed.insert(ParamsFixed(0x8175, 0, L"MAIL.ADD.MAILER"));
	params_fixed.insert(ParamsFixed(0x8176, 0, L"MAIL.DELETE.MAILER"));
	params_fixed.insert(ParamsFixed(0x8177, 0, L"MAIL.REPLY"));
	params_fixed.insert(ParamsFixed(0x8178, 0, L"MAIL.REPLY.ALL"));
	params_fixed.insert(ParamsFixed(0x8179, 0, L"MAIL.FORWARD"));
	params_fixed.insert(ParamsFixed(0x817A, 0, L"MAIL.NEXT.LETTER"));
	params_fixed.insert(ParamsFixed(0x817B, -1, L"DATA.LABEL"));
	params_fixed.insert(ParamsFixed(0x817C, -1, L"INSERT.TITLE"));
	params_fixed.insert(ParamsFixed(0x817D, -1, L"FONT.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x817E, -1, L"MACRO.OPTIONS"));
	params_fixed.insert(ParamsFixed(0x817F, -1, L"WORKBOOK.HIDE"));
	params_fixed.insert(ParamsFixed(0x8180, -1, L"WORKBOOK.UNHIDE"));
	params_fixed.insert(ParamsFixed(0x8181, -1, L"WORKBOOK.DELETE"));
	params_fixed.insert(ParamsFixed(0x8182, -1, L"WORKBOOK.NAME"));
	params_fixed.insert(ParamsFixed(0x8186, -1, L"ADD.CHART.AUTOFORMAT"));
	params_fixed.insert(ParamsFixed(0x8187, -1, L"DELETE.CHART.AUTOFORMAT"));
	params_fixed.insert(ParamsFixed(0x8188, -1, L"CHART.ADD.DATA"));
	params_fixed.insert(ParamsFixed(0x8189, 0, L"AUTO.OUTLINE"));
	params_fixed.insert(ParamsFixed(0x818A, 0, L"TAB.ORDER"));
	params_fixed.insert(ParamsFixed(0x818B, -1, L"SHOW.DIALOG"));
	params_fixed.insert(ParamsFixed(0x818C, 0, L"SELECT.ALL"));
	params_fixed.insert(ParamsFixed(0x818D, 0, L"UNGROUP.SHEETS"));
	params_fixed.insert(ParamsFixed(0x818E, -1, L"SUBTOTAL.CREATE"));
	params_fixed.insert(ParamsFixed(0x818F, 0, L"SUBTOTAL.REMOVE"));
	params_fixed.insert(ParamsFixed(0x8190, -1, L"RENAME.OBJECT"));
	params_fixed.insert(ParamsFixed(0x819C, -1, L"WORKBOOK.SCROLL"));
	params_fixed.insert(ParamsFixed(0x819D, 0, L"WORKBOOK.NEXT"));
	params_fixed.insert(ParamsFixed(0x819E, 0, L"WORKBOOK.PREV"));
	params_fixed.insert(ParamsFixed(0x819F, -1, L"WORKBOOK.TAB.SPLIT"));
	params_fixed.insert(ParamsFixed(0x81A0, -1, L"FULL.SCREEN"));
	params_fixed.insert(ParamsFixed(0x81A1, -1, L"WORKBOOK.PROTECT"));
	params_fixed.insert(ParamsFixed(0x81A4, -1, L"SCROLLBAR.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81A5, -1, L"PIVOT.SHOW.PAGES"));
	params_fixed.insert(ParamsFixed(0x81A6, -1, L"TEXT.TO.COLUMNS"));
	params_fixed.insert(ParamsFixed(0x81A7, -1, L"FORMAT.CHARTTYPE"));
	params_fixed.insert(ParamsFixed(0x81A8, 0, L"LINK.FORMAT"));
	params_fixed.insert(ParamsFixed(0x81A9, -1, L"TRACER.DISPLAY"));
	params_fixed.insert(ParamsFixed(0x81AE, -1, L"TRACER.NAVIGATE"));
	params_fixed.insert(ParamsFixed(0x81AF, 0, L"TRACER.CLEAR"));
	params_fixed.insert(ParamsFixed(0x81B0, 0, L"TRACER.ERROR"));
	params_fixed.insert(ParamsFixed(0x81B1, -1, L"PIVOT.FIELD.GROUP"));
	params_fixed.insert(ParamsFixed(0x81B2, 0, L"PIVOT.FIELD.UNGROUP"));
	params_fixed.insert(ParamsFixed(0x81B3, -1, L"CHECKBOX.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81B4, -1, L"LABEL.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81B5, -1, L"LISTBOX.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81B6, -1, L"EDITBOX.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81B7, -1, L"PIVOT.REFRESH"));
	params_fixed.insert(ParamsFixed(0x81B8, -1, L"LINK.COMBO"));
	params_fixed.insert(ParamsFixed(0x81B9, -1, L"OPEN.TEXT"));
	params_fixed.insert(ParamsFixed(0x81BA, -1, L"HIDE.DIALOG"));
	params_fixed.insert(ParamsFixed(0x81BB, -1, L"SET.DIALOG.FOCUS"));
	params_fixed.insert(ParamsFixed(0x81BC, -1, L"ENABLE.OBJECT"));
	params_fixed.insert(ParamsFixed(0x81BD, -1, L"PUSHBUTTON.PROPERTIES"));
	params_fixed.insert(ParamsFixed(0x81BE, -1, L"SET.DIALOG.DEFAULT"));
	params_fixed.insert(ParamsFixed(0x81BF, -1, L"FILTER"));
	params_fixed.insert(ParamsFixed(0x81C0, 0, L"FILTER.SHOW.ALL"));
	params_fixed.insert(ParamsFixed(0x81C1, 0, L"CLEAR.OUTLINE"));
	params_fixed.insert(ParamsFixed(0x81C2, -1, L"FUNCTION.WIZARD"));
	params_fixed.insert(ParamsFixed(0x81C3, -1, L"ADD.LIST.ITEM"));
	params_fixed.insert(ParamsFixed(0x81C4, -1, L"SET.LIST.ITEM"));
	params_fixed.insert(ParamsFixed(0x81C5, -1, L"REMOVE.LIST.ITEM"));
	params_fixed.insert(ParamsFixed(0x81C6, -1, L"SELECT.LIST.ITEM"));
	params_fixed.insert(ParamsFixed(0x81C7, -1, L"SET.CONTROL.VALUE"));
	params_fixed.insert(ParamsFixed(0x81C8, -1, L"SAVE.COPY.AS"));
	params_fixed.insert(ParamsFixed(0x81CA, -1, L"OPTIONS.LISTS.ADD"));
	params_fixed.insert(ParamsFixed(0x81CB, -1, L"OPTIONS.LISTS.DELETE"));
	params_fixed.insert(ParamsFixed(0x81CC, -1, L"SERIES.AXES"));
	params_fixed.insert(ParamsFixed(0x81CD, -1, L"SERIES.X"));
	params_fixed.insert(ParamsFixed(0x81CE, -1, L"SERIES.Y"));
	params_fixed.insert(ParamsFixed(0x81CF, -1, L"ERRORBAR.X"));
	params_fixed.insert(ParamsFixed(0x81D0, -1, L"ERRORBAR.Y"));
	params_fixed.insert(ParamsFixed(0x81D1, -1, L"FORMAT.CHART"));
	params_fixed.insert(ParamsFixed(0x81D2, -1, L"SERIES.ORDER"));
	params_fixed.insert(ParamsFixed(0x81D3, 0, L"MAIL.LOGOFF"));
	params_fixed.insert(ParamsFixed(0x81D4, -1, L"CLEAR.ROUTING.SLIP"));
	params_fixed.insert(ParamsFixed(0x81D5, -1, L"APP.ACTIVATE.MICROSOFT"));
	params_fixed.insert(ParamsFixed(0x81D6, -1, L"MAIL.EDIT.MAILER"));
	params_fixed.insert(ParamsFixed(0x81D7, -1, L"ON.SHEET"));
	params_fixed.insert(ParamsFixed(0x81D8, -1, L"STANDARD.WIDTH"));
	params_fixed.insert(ParamsFixed(0x81D9, -1, L"SCENARIO.MERGE"));
	params_fixed.insert(ParamsFixed(0x81DA, -1, L"SUMMARY.INFO"));
	params_fixed.insert(ParamsFixed(0x81DB, 0, L"FIND.FILE"));
	params_fixed.insert(ParamsFixed(0x81DC, -1, L"ACTIVE.CELL.FONT"));
	params_fixed.insert(ParamsFixed(0x81DD, -1, L"ENABLE.TIPWIZARD"));
	params_fixed.insert(ParamsFixed(0x81DE, -1, L"VBA.MAKE.ADDIN"));
	params_fixed.insert(ParamsFixed(0x81E0, -1, L"INSERTDATATABLE"));
	params_fixed.insert(ParamsFixed(0x81E1, 0, L"WORKGROUP.OPTIONS"));
	params_fixed.insert(ParamsFixed(0x81E2, -1, L"MAIL.SEND.MAILER"));
	params_fixed.insert(ParamsFixed(0x81E5, -1, L"AUTOCORRECT"));
	params_fixed.insert(ParamsFixed(0x81E9, -1, L"POST.DOCUMENT"));
	params_fixed.insert(ParamsFixed(0x81EB, 0, L"PICKLIST"));
	params_fixed.insert(ParamsFixed(0x81ED, -1, L"VIEW.SHOW"));
	params_fixed.insert(ParamsFixed(0x81EE, -1, L"VIEW.DEFINE"));
	params_fixed.insert(ParamsFixed(0x81EF, -1, L"VIEW.DELETE"));
	params_fixed.insert(ParamsFixed(0x81FD, -1, L"SHEET.BACKGROUND"));
	params_fixed.insert(ParamsFixed(0x81FE, 0, L"INSERT.MAP.OBJECT"));
	params_fixed.insert(ParamsFixed(0x81FF, -1, L"OPTIONS.MENONO"));
	params_fixed.insert(ParamsFixed(0x8205, 0, L"MSOCHECKS"));
	params_fixed.insert(ParamsFixed(0x8206, 0, L"NORMAL"));
	params_fixed.insert(ParamsFixed(0x8207, 0, L"LAYOUT"));
	params_fixed.insert(ParamsFixed(0x8208, -1, L"RM.PRINT.AREA"));
	params_fixed.insert(ParamsFixed(0x8209, 0, L"CLEAR.PRINT.AREA"));
	params_fixed.insert(ParamsFixed(0x820A, 0, L"ADD.PRINT.AREA"));
	params_fixed.insert(ParamsFixed(0x820B, -1, L"MOVE.BRK"));
	params_fixed.insert(ParamsFixed(0x8221, -1, L"HIDECURR.NOTE"));
	params_fixed.insert(ParamsFixed(0x8222, -1, L"HIDEALL.NOTES"));
	params_fixed.insert(ParamsFixed(0x8223, -1, L"DELETE.NOTE"));
	params_fixed.insert(ParamsFixed(0x8224, -1, L"TRAVERSE.NOTES"));
	params_fixed.insert(ParamsFixed(0x8225, -1, L"ACTIVATE.NOTES"));
	params_fixed.insert(ParamsFixed(0x826C, 0, L"PROTECT.REVISIONS"));
	params_fixed.insert(ParamsFixed(0x826D, 0, L"UNPROTECT.REVISIONS"));
	params_fixed.insert(ParamsFixed(0x8287, -1, L"OPTIONS.ME"));
	params_fixed.insert(ParamsFixed(0x828D, -1, L"WEB.PUBLISH"));
	params_fixed.insert(ParamsFixed(0x829B, -1, L"NEWWEBQUERY"));
	params_fixed.insert(ParamsFixed(0x82A1, -1, L"PIVOT.TABLE.CHART"));
	params_fixed.insert(ParamsFixed(0x82F1, -1, L"OPTIONS.SAVE"));
	params_fixed.insert(ParamsFixed(0x82F3, -1, L"OPTIONS.SPELL"));
	params_fixed.insert(ParamsFixed(0x8328, -1, L"HIDEALL.INKANNOTS"));
}


// static
const std::wstring Ftab_Cetab::ValuesDetermination::getName(const unsigned short iftab)
{
	const ParamsFixedSet::index<sort_by_iftab>::type& iftab_index = getInst().params_fixed.get<sort_by_iftab>();
	const ParamsFixedSet::index<sort_by_iftab>::type::iterator found = iftab_index.find(iftab);
	if(iftab_index.end() != found)
	{
		return found->name;
	}
	return L"";
}


// static
const int Ftab_Cetab::ValuesDetermination::getParamsNum(const unsigned short iftab)
{
	const ParamsFixedSet::index<sort_by_iftab>::type& iftab_index = getInst().params_fixed.get<sort_by_iftab>();
	const ParamsFixedSet::index<sort_by_iftab>::type::iterator found = iftab_index.find(iftab);
	if(iftab_index.end() != found)
	{
		return found->params_num;
	}
	return -1;
}


// static
const unsigned short Ftab_Cetab::ValuesDetermination::getIndex(const std::wstring& func_name)
{
	const ParamsFixedSet::index<sort_by_name>::type& name_index = getInst().params_fixed.get<sort_by_name>();
	const ParamsFixedSet::index<sort_by_name>::type::iterator found = name_index.find(func_name);
	if(name_index.end() != found)
	{
		return found->iftab;
	}
	return 0xFFFF;
}


// static
const int Ftab_Cetab::ValuesDetermination::getParamsNum(const std::wstring& func_name)
{
	const ParamsFixedSet::index<sort_by_name>::type& name_index = getInst().params_fixed.get<sort_by_name>();
	const ParamsFixedSet::index<sort_by_name>::type::iterator found = name_index.find(func_name);
	if(name_index.end() != found)
	{
		return found->params_num;
	}
	return -1;
}


const std::wstring Ftab_Cetab::getFuncName() const
{
	return ValuesDetermination::getName(func_index);
}


const int Ftab_Cetab::getParamsNum() const
{
	return ValuesDetermination::getParamsNum(func_index);
}


const unsigned short Ftab_Cetab::getIndex() const
{
	return func_index;
}


const bool Ftab_Cetab::isMacro() const
{
	return 0 != (func_index & 0x8000);
}


// static
const unsigned short Ftab_Cetab::recognizeFunction(const std::wstring& word)
{
	return ValuesDetermination::getIndex(word);
}


} // namespace XLS

