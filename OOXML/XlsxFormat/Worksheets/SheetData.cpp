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

#include "../Workbook/Workbook.h"

#include "../../DocxFormat/VmlDrawing.h"
#include "../Styles/Styles.h"
#include "../SharedStrings/SharedStrings.h"
#include "DataValidation.h"
#include "../Comments/ThreadedComments.h"

#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../Binary/Sheets/Writer/CSVWriter.h"
#include "../../../DesktopEditor/common/StreamWriter.h"

#include "../../XlsbFormat/Biff12_unions/ACCELLTABLE.h"
#include "../../XlsbFormat/Biff12_records/RwDescent.h"
#include "../../XlsbFormat/Biff12_unions/CELLTABLE.h"
#include "../../XlsbFormat/Biff12_unions/CELL.h"
#include "../../XlsbFormat/Biff12_unions/CELLMETA.h"
#include "../../XlsbFormat/Biff12_unions/DATACELL.h"
#include "../../XlsbFormat/Biff12_unions/FMLACELL.h"
#include "../../XlsbFormat/Biff12_unions/SHRFMLACELL.h"
#include "../../XlsbFormat/Biff12_unions/TABLECELL.h"
#include "../../XlsbFormat/Biff12_records/CellMeta.h"
#include "../../XlsbFormat/Biff12_records/ValueMeta.h"
#include "../../XlsbFormat/Biff12_records/Cell.h"
#include "../../XlsbFormat/Biff12_records/Fmla.h"
#include "../../XlsbFormat/Biff12_structures/CellRef.h"

#include <boost/regex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace XLS;

namespace OOX
{
	namespace Spreadsheet
	{
		static const wchar_t* m_aLetters[] = 
		{ 
			L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"AA", L"AB", L"AC", L"AD", L"AE", L"AF", L"AG", L"AH", L"AI", L"AJ", L"AK", L"AL", L"AM", L"AN", L"AO", L"AP", L"AQ", L"AR", L"AS", L"AT", L"AU", L"AV", L"AW", L"AX", L"AY", L"AZ", L"BA", L"BB", L"BC", L"BD", L"BE", L"BF", L"BG", L"BH", L"BI", L"BJ", L"BK", L"BL", L"BM", L"BN", L"BO", L"BP", L"BQ", L"BR", L"BS", L"BT", L"BU", L"BV", L"BW", L"BX", L"BY", L"BZ", L"CA", L"CB", L"CC", L"CD", L"CE", L"CF", L"CG", L"CH", L"CI", L"CJ", L"CK", L"CL", L"CM", L"CN", L"CO", L"CP", L"CQ", L"CR", L"CS", L"CT", L"CU", L"CV", L"CW", L"CX", L"CY", L"CZ", L"DA", L"DB", L"DC", L"DD", L"DE", L"DF", L"DG", L"DH", L"DI", L"DJ", L"DK", L"DL", L"DM", L"DN", L"DO", L"DP", L"DQ", L"DR", L"DS", L"DT", L"DU", L"DV", L"DW", L"DX", L"DY", L"DZ", L"EA", L"EB", L"EC", L"ED", L"EE", L"EF", L"EG", L"EH", L"EI", L"EJ", L"EK", L"EL", L"EM", L"EN", L"EO", L"EP", L"EQ", L"ER", L"ES", L"ET", L"EU", L"EV", L"EW", L"EX", L"EY", L"EZ", L"FA", L"FB", L"FC", L"FD", L"FE", L"FF", L"FG", L"FH", L"FI", L"FJ", L"FK", L"FL", L"FM", L"FN", L"FO", L"FP", L"FQ", L"FR", L"FS", L"FT", L"FU", L"FV", L"FW", L"FX", L"FY", L"FZ", L"GA", L"GB", L"GC", L"GD", L"GE", L"GF", L"GG", L"GH", L"GI", L"GJ", L"GK", L"GL", L"GM", L"GN", L"GO", L"GP", L"GQ", L"GR", L"GS", L"GT", L"GU", L"GV", L"GW", L"GX", L"GY", L"GZ", L"HA", L"HB", L"HC", L"HD", L"HE", L"HF", L"HG", L"HH", L"HI", L"HJ", L"HK", L"HL", L"HM", L"HN", L"HO", L"HP", L"HQ", L"HR", L"HS", L"HT", L"HU", L"HV", L"HW", L"HX", L"HY", L"HZ", L"IA", L"IB", L"IC", L"ID", L"IE", L"IF", L"IG", L"IH", L"II", L"IJ", L"IK", L"IL", L"IM", L"IN", L"IO", L"IP", L"IQ", L"IR", L"IS", L"IT", L"IU", L"IV", L"IW", L"IX", L"IY", L"IZ", L"JA", L"JB", L"JC", L"JD", L"JE", L"JF", L"JG", L"JH", L"JI", L"JJ", L"JK", L"JL", L"JM", L"JN", L"JO", L"JP", L"JQ", L"JR", L"JS", L"JT", L"JU", L"JV", L"JW", L"JX", L"JY", L"JZ", L"KA", L"KB", L"KC", L"KD", L"KE", L"KF", L"KG", L"KH", L"KI", L"KJ", L"KK", L"KL", L"KM", L"KN", L"KO", L"KP", L"KQ", L"KR", L"KS", L"KT", L"KU", L"KV", L"KW", L"KX", L"KY", L"KZ", L"LA", L"LB", L"LC", L"LD", L"LE", L"LF", L"LG", L"LH", L"LI", L"LJ", L"LK", L"LL", L"LM", L"LN", L"LO", L"LP", L"LQ", L"LR", L"LS", L"LT", L"LU", L"LV", L"LW", L"LX", L"LY", L"LZ", L"MA", L"MB", L"MC", L"MD", L"ME", L"MF", L"MG", L"MH", L"MI", L"MJ", L"MK", L"ML", L"MM", L"MN", L"MO", L"MP", L"MQ", L"MR", L"MS", L"MT", L"MU", L"MV", L"MW", L"MX", L"MY", L"MZ", L"NA", L"NB", L"NC", L"ND", L"NE", L"NF", L"NG", L"NH", L"NI", L"NJ", L"NK", L"NL", L"NM", L"NN", L"NO", L"NP", L"NQ", L"NR", L"NS", L"NT", L"NU", L"NV", L"NW", L"NX", L"NY", L"NZ", L"OA", L"OB", L"OC", L"OD", L"OE", L"OF", L"OG", L"OH", L"OI", L"OJ", L"OK", L"OL", L"OM", L"ON", L"OO", L"OP", L"OQ", L"OR", L"OS", L"OT", L"OU", L"OV", L"OW", L"OX", L"OY", L"OZ", L"PA", L"PB", L"PC", L"PD", L"PE", L"PF", L"PG", L"PH", L"PI", L"PJ", L"PK", L"PL", L"PM", L"PN", L"PO", L"PP", L"PQ", L"PR", L"PS", L"PT", L"PU", L"PV", L"PW", L"PX", L"PY", L"PZ", L"QA", L"QB", L"QC", L"QD", L"QE", L"QF", L"QG", L"QH", L"QI", L"QJ", L"QK", L"QL", L"QM", L"QN", L"QO", L"QP", L"QQ", L"QR", L"QS", L"QT", L"QU", L"QV", L"QW", L"QX", L"QY", L"QZ", L"RA", L"RB", L"RC", L"RD", L"RE", L"RF", L"RG", L"RH", L"RI", L"RJ", L"RK", L"RL", L"RM", L"RN", L"RO", L"RP", L"RQ", L"RR", L"RS", L"RT", L"RU", L"RV", L"RW", L"RX", L"RY", L"RZ", L"SA", L"SB", L"SC", L"SD", L"SE", L"SF", L"SG", L"SH", L"SI", L"SJ", L"SK", L"SL", L"SM", L"SN", L"SO", L"SP", L"SQ", L"SR", L"SS", L"ST", L"SU", L"SV", L"SW", L"SX", L"SY", L"SZ", L"TA", L"TB", L"TC", L"TD", L"TE", L"TF", L"TG", L"TH", L"TI", L"TJ", L"TK", L"TL", L"TM", L"TN", L"TO", L"TP", L"TQ", L"TR", L"TS", L"TT", L"TU", L"TV", L"TW", L"TX", L"TY", L"TZ", L"UA", L"UB", L"UC", L"UD", L"UE", L"UF", L"UG", L"UH", L"UI", L"UJ", L"UK", L"UL", L"UM", L"UN", L"UO", L"UP", L"UQ", L"UR", L"US", L"UT", L"UU", L"UV", L"UW", L"UX", L"UY", L"UZ", L"VA", L"VB", L"VC", L"VD", L"VE", L"VF", L"VG", L"VH", L"VI", L"VJ", L"VK", L"VL", L"VM", L"VN", L"VO", L"VP", L"VQ", L"VR", L"VS", L"VT", L"VU", L"VV", L"VW", L"VX", L"VY", L"VZ", L"WA", L"WB", L"WC", L"WD", L"WE", L"WF", L"WG", L"WH", L"WI", L"WJ", L"WK", L"WL", L"WM", L"WN", L"WO", L"WP", L"WQ", L"WR", L"WS", L"WT", L"WU", L"WV", L"WW", L"WX", L"WY", 
			L"WZ", L"XA", L"XB", L"XC", L"XD", L"XE", L"XF", L"XG", L"XH", L"XI", L"XJ", L"XK", L"XL", L"XM", L"XN", L"XO", L"XP", L"XQ", L"XR", L"XS", L"XT", L"XU", L"XV", L"XW", L"XX", L"XY", L"XZ", L"YA", L"YB", L"YC", L"YD", L"YE", L"YF", L"YG", L"YH", L"YI", L"YJ", L"YK", L"YL", L"YM", L"YN", L"YO", L"YP", L"YQ", L"YR", L"YS", L"YT", L"YU", L"YV", L"YW", L"YX", L"YY", L"YZ", L"ZA", L"ZB", L"ZC", L"ZD", L"ZE", L"ZF", L"ZG", L"ZH", L"ZI", L"ZJ", L"ZK", L"ZL", L"ZM", L"ZN", L"ZO", L"ZP", L"ZQ", L"ZR", L"ZS", L"ZT", L"ZU", L"ZV", L"ZW", L"ZX", L"ZY", L"ZZ", L"AAA", L"AAB", L"AAC", L"AAD", L"AAE", L"AAF", L"AAG", L"AAH", L"AAI", L"AAJ", L"AAK", L"AAL", L"AAM", L"AAN", L"AAO", L"AAP", L"AAQ", L"AAR", L"AAS", L"AAT", L"AAU", L"AAV", L"AAW", L"AAX", L"AAY", L"AAZ", L"ABA", L"ABB", L"ABC", L"ABD", L"ABE", L"ABF", L"ABG", L"ABH", L"ABI", L"ABJ", L"ABK", L"ABL", L"ABM", L"ABN", L"ABO", L"ABP", L"ABQ", L"ABR", L"ABS", L"ABT", L"ABU", L"ABV", L"ABW", L"ABX", L"ABY", L"ABZ", L"ACA", L"ACB", L"ACC", L"ACD", L"ACE", L"ACF", L"ACG", L"ACH", L"ACI", L"ACJ", L"ACK", L"ACL", L"ACM", L"ACN", L"ACO", L"ACP", L"ACQ", L"ACR", L"ACS", L"ACT", L"ACU", L"ACV", L"ACW", L"ACX", L"ACY", L"ACZ", L"ADA", L"ADB", L"ADC", L"ADD", L"ADE", L"ADF", L"ADG", L"ADH", L"ADI", L"ADJ", L"ADK", L"ADL", L"ADM", L"ADN", L"ADO", L"ADP", L"ADQ", L"ADR", L"ADS", L"ADT", L"ADU", L"ADV", L"ADW", L"ADX", L"ADY", L"ADZ", L"AEA", L"AEB", L"AEC", L"AED", L"AEE", L"AEF", L"AEG", L"AEH", L"AEI", L"AEJ", L"AEK", L"AEL", L"AEM", L"AEN", L"AEO", L"AEP", L"AEQ", L"AER", L"AES", L"AET", L"AEU", L"AEV", L"AEW", L"AEX", L"AEY", L"AEZ", L"AFA", L"AFB", L"AFC", L"AFD", L"AFE", L"AFF", L"AFG", L"AFH", L"AFI", L"AFJ", L"AFK", L"AFL", L"AFM", L"AFN", L"AFO", L"AFP", L"AFQ", L"AFR", L"AFS", L"AFT", L"AFU", L"AFV", L"AFW", L"AFX", L"AFY", L"AFZ", L"AGA", L"AGB", L"AGC", L"AGD", L"AGE", L"AGF", L"AGG", L"AGH", L"AGI", L"AGJ", L"AGK", L"AGL", L"AGM", L"AGN", L"AGO", L"AGP", L"AGQ", L"AGR", L"AGS", L"AGT", L"AGU", L"AGV", L"AGW", L"AGX", L"AGY", L"AGZ", L"AHA", L"AHB", L"AHC", L"AHD", L"AHE", L"AHF", L"AHG", L"AHH", L"AHI", L"AHJ", L"AHK", L"AHL", L"AHM", L"AHN", L"AHO", L"AHP", L"AHQ", L"AHR", L"AHS", L"AHT", L"AHU", L"AHV", 
			L"AHW", L"AHX", L"AHY", L"AHZ", L"AIA", L"AIB", L"AIC", L"AID", L"AIE", L"AIF", L"AIG", L"AIH", L"AII", L"AIJ", L"AIK", L"AIL", L"AIM", L"AIN", L"AIO", L"AIP", L"AIQ", L"AIR", L"AIS", L"AIT", L"AIU", L"AIV", L"AIW", L"AIX", L"AIY", L"AIZ", L"AJA", L"AJB", L"AJC", L"AJD", L"AJE", L"AJF", L"AJG", L"AJH", L"AJI", L"AJJ", L"AJK", L"AJL", L"AJM", L"AJN", L"AJO", L"AJP", L"AJQ", L"AJR", L"AJS", L"AJT", L"AJU", L"AJV", L"AJW", L"AJX", L"AJY", L"AJZ", L"AKA", L"AKB", L"AKC", L"AKD", L"AKE", L"AKF", L"AKG", L"AKH", L"AKI", L"AKJ", L"AKK", L"AKL", L"AKM", L"AKN", L"AKO", L"AKP", L"AKQ", L"AKR", L"AKS", L"AKT", L"AKU", L"AKV", L"AKW", L"AKX", L"AKY", L"AKZ", L"ALA", L"ALB", L"ALC", L"ALD", L"ALE", L"ALF", L"ALG", L"ALH", L"ALI", L"ALJ", L"ALK", L"ALL", L"ALM", L"ALN", L"ALO", L"ALP", L"ALQ", L"ALR", L"ALS", L"ALT", L"ALU", L"ALV", L"ALW", L"ALX", L"ALY", L"ALZ", L"AMA", L"AMB", L"AMC", L"AMD", L"AME", L"AMF", L"AMG", L"AMH", L"AMI", L"AMJ", L"AMK", L"AML", L"AMM", L"AMN", L"AMO", L"AMP", L"AMQ", L"AMR", L"AMS", L"AMT", L"AMU", L"AMV", L"AMW", L"AMX", L"AMY", L"AMZ", L"ANA", L"ANB", L"ANC", L"AND", L"ANE", L"ANF", L"ANG", L"ANH", L"ANI", L"ANJ", L"ANK", L"ANL", L"ANM", L"ANN", L"ANO", L"ANP", L"ANQ", L"ANR", L"ANS", L"ANT", L"ANU", L"ANV", L"ANW", L"ANX", L"ANY", L"ANZ", L"AOA", L"AOB", L"AOC", L"AOD", L"AOE", L"AOF", L"AOG", L"AOH", L"AOI", L"AOJ", L"AOK", L"AOL", L"AOM", L"AON", L"AOO", L"AOP", L"AOQ", L"AOR", L"AOS", L"AOT", L"AOU", L"AOV", L"AOW", L"AOX", L"AOY", L"AOZ", L"APA", L"APB", L"APC", L"APD", L"APE", L"APF", L"APG", L"APH", L"API", L"APJ", L"APK", L"APL", L"APM", L"APN", L"APO", L"APP", L"APQ", L"APR", L"APS", L"APT", L"APU", L"APV", L"APW", L"APX", L"APY", L"APZ", L"AQA", L"AQB", L"AQC", L"AQD", L"AQE", L"AQF", L"AQG", L"AQH", L"AQI", L"AQJ", L"AQK", L"AQL", L"AQM", L"AQN", L"AQO", L"AQP", L"AQQ", L"AQR", L"AQS", L"AQT", L"AQU", L"AQV", L"AQW", L"AQX", L"AQY", L"AQZ", L"ARA", L"ARB", L"ARC", L"ARD", L"ARE", L"ARF", L"ARG", L"ARH", L"ARI", L"ARJ", L"ARK", L"ARL", L"ARM", L"ARN", L"ARO", L"ARP", L"ARQ", L"ARR", L"ARS", L"ART", L"ARU", L"ARV", L"ARW", L"ARX", L"ARY", L"ARZ", L"ASA", L"ASB", L"ASC", L"ASD", L"ASE", L"ASF", L"ASG", L"ASH", L"ASI", L"ASJ", L"ASK", L"ASL", L"ASM", L"ASN", L"ASO", L"ASP", L"ASQ", L"ASR", L"ASS", L"AST", L"ASU", L"ASV", L"ASW", L"ASX", L"ASY", L"ASZ", L"ATA", L"ATB", L"ATC", L"ATD", L"ATE", L"ATF", L"ATG", L"ATH", L"ATI", L"ATJ", L"ATK", L"ATL", L"ATM", L"ATN", L"ATO", L"ATP", L"ATQ", L"ATR", L"ATS", L"ATT", L"ATU", L"ATV", L"ATW", L"ATX", L"ATY", L"ATZ", L"AUA", L"AUB", L"AUC", L"AUD", L"AUE", L"AUF", L"AUG", L"AUH", L"AUI", L"AUJ", L"AUK", L"AUL", L"AUM", L"AUN", L"AUO", L"AUP", L"AUQ", L"AUR", L"AUS", L"AUT", L"AUU", L"AUV", L"AUW", L"AUX", L"AUY", L"AUZ", L"AVA", L"AVB", L"AVC", L"AVD", L"AVE", L"AVF", L"AVG", L"AVH", L"AVI", L"AVJ", L"AVK", L"AVL", L"AVM", L"AVN", L"AVO", L"AVP", L"AVQ", L"AVR", L"AVS", L"AVT", L"AVU", L"AVV", L"AVW", L"AVX", L"AVY", L"AVZ", L"AWA", L"AWB", L"AWC", L"AWD", L"AWE", L"AWF", L"AWG", L"AWH", L"AWI", L"AWJ", L"AWK", L"AWL", L"AWM", L"AWN", L"AWO", L"AWP", L"AWQ", L"AWR", L"AWS", L"AWT", L"AWU", L"AWV", L"AWW", L"AWX", L"AWY", L"AWZ", L"AXA", L"AXB", L"AXC", L"AXD", L"AXE", L"AXF", L"AXG", L"AXH", L"AXI", L"AXJ", L"AXK", L"AXL", L"AXM", L"AXN", L"AXO", L"AXP", L"AXQ", L"AXR", L"AXS", L"AXT", L"AXU", L"AXV", L"AXW", L"AXX", L"AXY", L"AXZ", L"AYA", L"AYB", L"AYC", L"AYD", L"AYE", L"AYF", L"AYG", L"AYH", L"AYI", L"AYJ", L"AYK", L"AYL", L"AYM", L"AYN", L"AYO", L"AYP", L"AYQ", L"AYR", L"AYS", L"AYT", L"AYU", L"AYV", L"AYW", L"AYX", L"AYY", L"AYZ", L"AZA", L"AZB", L"AZC", L"AZD", L"AZE", L"AZF", L"AZG", L"AZH", L"AZI", L"AZJ", L"AZK", L"AZL", L"AZM", L"AZN", L"AZO", L"AZP", L"AZQ", L"AZR", L"AZS", L"AZT", L"AZU", L"AZV", L"AZW", L"AZX", L"AZY", L"AZZ", L"BAA", L"BAB", L"BAC", L"BAD", L"BAE", L"BAF", L"BAG", L"BAH", L"BAI", L"BAJ", L"BAK", L"BAL", L"BAM", L"BAN", L"BAO", L"BAP", L"BAQ", L"BAR", L"BAS", L"BAT", L"BAU", L"BAV", L"BAW", L"BAX", L"BAY", L"BAZ", L"BBA", L"BBB", L"BBC", L"BBD", L"BBE", L"BBF", L"BBG", L"BBH", L"BBI", L"BBJ", L"BBK", L"BBL", L"BBM", L"BBN", L"BBO", L"BBP", L"BBQ", L"BBR", L"BBS", L"BBT", L"BBU", L"BBV", L"BBW", L"BBX", L"BBY", L"BBZ", L"BCA", L"BCB", L"BCC", L"BCD", L"BCE", L"BCF", L"BCG", L"BCH", L"BCI", L"BCJ", L"BCK", L"BCL", L"BCM", L"BCN",
			L"BCO", L"BCP", L"BCQ", L"BCR", L"BCS", L"BCT", L"BCU", L"BCV", L"BCW", L"BCX", L"BCY", L"BCZ", L"BDA", L"BDB", L"BDC", L"BDD", L"BDE", L"BDF", L"BDG", L"BDH", L"BDI", L"BDJ", L"BDK", L"BDL", L"BDM", L"BDN", L"BDO", L"BDP", L"BDQ", L"BDR", L"BDS", L"BDT", L"BDU", L"BDV", L"BDW", L"BDX", L"BDY", L"BDZ", L"BEA", L"BEB", L"BEC", L"BED", L"BEE", L"BEF", L"BEG", L"BEH", L"BEI", L"BEJ", L"BEK", L"BEL", L"BEM", L"BEN", L"BEO", L"BEP", L"BEQ", L"BER", L"BES", L"BET", L"BEU", L"BEV", L"BEW", L"BEX", L"BEY", L"BEZ", L"BFA", L"BFB", L"BFC", L"BFD", L"BFE", L"BFF", L"BFG", L"BFH", L"BFI", L"BFJ", L"BFK", L"BFL", L"BFM", L"BFN", L"BFO", L"BFP", L"BFQ", L"BFR", L"BFS", L"BFT", L"BFU", L"BFV", L"BFW", L"BFX", L"BFY", L"BFZ", L"BGA", L"BGB", L"BGC", L"BGD", L"BGE", L"BGF", L"BGG", L"BGH", L"BGI", L"BGJ", L"BGK", L"BGL", L"BGM", L"BGN", L"BGO", L"BGP", L"BGQ", L"BGR", L"BGS", L"BGT", L"BGU", L"BGV", L"BGW", L"BGX", L"BGY", L"BGZ", L"BHA", L"BHB", L"BHC", L"BHD", L"BHE", L"BHF", L"BHG", L"BHH", L"BHI", L"BHJ", L"BHK", L"BHL", L"BHM", L"BHN", L"BHO", L"BHP", L"BHQ", L"BHR", L"BHS", L"BHT", L"BHU", L"BHV", L"BHW", L"BHX", L"BHY", L"BHZ", L"BIA", L"BIB", L"BIC", L"BID", L"BIE", L"BIF", L"BIG", L"BIH", L"BII", L"BIJ", L"BIK", L"BIL", L"BIM", L"BIN", L"BIO", L"BIP", L"BIQ", L"BIR", L"BIS", L"BIT", L"BIU", L"BIV", L"BIW", L"BIX", L"BIY", L"BIZ", L"BJA", L"BJB", L"BJC", L"BJD", L"BJE", L"BJF", L"BJG", L"BJH", L"BJI", L"BJJ", L"BJK", L"BJL", L"BJM", L"BJN", L"BJO", L"BJP", L"BJQ", L"BJR", L"BJS", L"BJT", L"BJU", L"BJV", L"BJW", L"BJX", L"BJY", L"BJZ", L"BKA", L"BKB", L"BKC", L"BKD", L"BKE", L"BKF", L"BKG", L"BKH", L"BKI", L"BKJ", L"BKK", L"BKL", L"BKM", L"BKN", L"BKO", L"BKP", L"BKQ", L"BKR", L"BKS", L"BKT", L"BKU", L"BKV", L"BKW", L"BKX", L"BKY", L"BKZ", L"BLA", L"BLB", L"BLC", L"BLD", L"BLE", L"BLF", L"BLG", L"BLH", L"BLI", L"BLJ", L"BLK", L"BLL", L"BLM", L"BLN", L"BLO", L"BLP", L"BLQ", L"BLR", L"BLS", L"BLT", L"BLU", L"BLV", L"BLW", L"BLX", L"BLY", L"BLZ", L"BMA", L"BMB", L"BMC", L"BMD", L"BME", L"BMF", L"BMG", L"BMH", L"BMI", L"BMJ", L"BMK", L"BML", L"BMM", L"BMN", L"BMO", L"BMP", L"BMQ", L"BMR", L"BMS", L"BMT", L"BMU", L"BMV", L"BMW", L"BMX", L"BMY", L"BMZ", L"BNA", L"BNB", L"BNC", L"BND", L"BNE", L"BNF", L"BNG", L"BNH", L"BNI", L"BNJ", L"BNK", L"BNL", L"BNM", L"BNN", L"BNO", L"BNP", L"BNQ", L"BNR", L"BNS", L"BNT", L"BNU", L"BNV", L"BNW", L"BNX", L"BNY", L"BNZ", L"BOA", L"BOB", L"BOC", L"BOD", L"BOE", L"BOF", L"BOG", L"BOH", L"BOI", L"BOJ", L"BOK", L"BOL", L"BOM", L"BON", L"BOO", L"BOP", L"BOQ", L"BOR", L"BOS", L"BOT", L"BOU", L"BOV", L"BOW", L"BOX", L"BOY", L"BOZ", L"BPA", L"BPB", L"BPC", L"BPD", L"BPE", L"BPF", L"BPG", L"BPH", L"BPI", L"BPJ", L"BPK", L"BPL", L"BPM", L"BPN", L"BPO", L"BPP", L"BPQ", L"BPR", L"BPS", L"BPT", L"BPU", L"BPV", L"BPW", L"BPX", L"BPY", L"BPZ", L"BQA", L"BQB", L"BQC", L"BQD", L"BQE", L"BQF", L"BQG", L"BQH", L"BQI", L"BQJ", L"BQK", L"BQL", L"BQM", L"BQN", L"BQO", L"BQP", L"BQQ", L"BQR", L"BQS", L"BQT", L"BQU", L"BQV", L"BQW", L"BQX", L"BQY", L"BQZ", L"BRA", L"BRB", L"BRC", L"BRD", L"BRE", L"BRF", L"BRG", L"BRH", L"BRI", L"BRJ", L"BRK", L"BRL", L"BRM", L"BRN", L"BRO", L"BRP", L"BRQ", L"BRR", L"BRS", L"BRT", L"BRU", L"BRV", L"BRW", L"BRX", L"BRY", L"BRZ", L"BSA", L"BSB", L"BSC", L"BSD", L"BSE", L"BSF", L"BSG", L"BSH", L"BSI", L"BSJ", L"BSK", L"BSL", L"BSM", L"BSN", L"BSO", L"BSP", L"BSQ", L"BSR", L"BSS", L"BST", L"BSU", L"BSV", L"BSW", L"BSX", L"BSY", L"BSZ", L"BTA", L"BTB", L"BTC", L"BTD", L"BTE", L"BTF", L"BTG", L"BTH", L"BTI", L"BTJ", L"BTK", L"BTL", L"BTM", L"BTN", L"BTO", L"BTP", L"BTQ", L"BTR", L"BTS", L"BTT", L"BTU", L"BTV", L"BTW", L"BTX", L"BTY", L"BTZ", L"BUA", L"BUB", L"BUC", L"BUD", L"BUE", L"BUF", L"BUG", L"BUH", L"BUI", L"BUJ", L"BUK", L"BUL", L"BUM", L"BUN", L"BUO", L"BUP", L"BUQ", L"BUR", L"BUS", L"BUT", L"BUU", L"BUV", L"BUW", L"BUX", L"BUY", L"BUZ", L"BVA", L"BVB", L"BVC", L"BVD", L"BVE", L"BVF", L"BVG", L"BVH", L"BVI", L"BVJ", L"BVK", L"BVL", L"BVM", L"BVN", L"BVO", L"BVP", L"BVQ", L"BVR", L"BVS", L"BVT", L"BVU", L"BVV", L"BVW", L"BVX", L"BVY", L"BVZ", L"BWA", L"BWB", L"BWC", L"BWD", L"BWE", L"BWF", L"BWG", L"BWH", L"BWI", L"BWJ", L"BWK", L"BWL", L"BWM", L"BWN", L"BWO", L"BWP", L"BWQ", L"BWR", L"BWS", L"BWT", L"BWU", L"BWV", L"BWW", L"BWX", L"BWY", L"BWZ", L"BXA", L"BXB", L"BXC", L"BXD", L"BXE", L"BXF", L"BXG", L"BXH", L"BXI", L"BXJ", L"BXK", L"BXL", L"BXM", L"BXN",
			L"BXO", L"BXP", L"BXQ", L"BXR", L"BXS", L"BXT", L"BXU", L"BXV", L"BXW", L"BXX", L"BXY", L"BXZ", L"BYA", L"BYB", L"BYC", L"BYD", L"BYE", L"BYF", L"BYG", L"BYH", L"BYI", L"BYJ", L"BYK", L"BYL", L"BYM", L"BYN", L"BYO", L"BYP", L"BYQ", L"BYR", L"BYS", L"BYT", L"BYU", L"BYV", L"BYW", L"BYX", L"BYY", L"BYZ", L"BZA", L"BZB", L"BZC", L"BZD", L"BZE", L"BZF", L"BZG", L"BZH", L"BZI", L"BZJ", L"BZK", L"BZL", L"BZM", L"BZN", L"BZO", L"BZP", L"BZQ", L"BZR", L"BZS", L"BZT", L"BZU", L"BZV", L"BZW", L"BZX", L"BZY", L"BZZ", L"CAA", L"CAB", L"CAC", L"CAD", L"CAE", L"CAF", L"CAG", L"CAH", L"CAI", L"CAJ", L"CAK", L"CAL", L"CAM", L"CAN", L"CAO", L"CAP", L"CAQ", L"CAR", L"CAS", L"CAT", L"CAU", L"CAV", L"CAW", L"CAX", L"CAY", L"CAZ", L"CBA", L"CBB", L"CBC", L"CBD", L"CBE", L"CBF", L"CBG", L"CBH", L"CBI", L"CBJ", L"CBK", L"CBL", L"CBM", L"CBN", L"CBO", L"CBP", L"CBQ", L"CBR", L"CBS", L"CBT", L"CBU", L"CBV", L"CBW", L"CBX", L"CBY", L"CBZ", L"CCA", L"CCB", L"CCC", L"CCD", L"CCE", L"CCF", L"CCG", L"CCH", L"CCI", L"CCJ", L"CCK", L"CCL", L"CCM", L"CCN", L"CCO", L"CCP", L"CCQ", L"CCR", L"CCS", L"CCT", L"CCU", L"CCV", L"CCW", L"CCX", L"CCY", L"CCZ", L"CDA", L"CDB", L"CDC", L"CDD", L"CDE", L"CDF", L"CDG", L"CDH", L"CDI", L"CDJ", L"CDK", L"CDL", L"CDM", L"CDN", L"CDO", L"CDP", L"CDQ", L"CDR", L"CDS", L"CDT", L"CDU", L"CDV", L"CDW", L"CDX", L"CDY", L"CDZ", L"CEA", L"CEB", L"CEC", L"CED", L"CEE", L"CEF", L"CEG", L"CEH", L"CEI", L"CEJ", L"CEK", L"CEL", L"CEM", L"CEN", L"CEO", L"CEP", L"CEQ", L"CER", L"CES", L"CET", L"CEU", L"CEV", L"CEW", L"CEX", L"CEY", L"CEZ", L"CFA", L"CFB", L"CFC", L"CFD", L"CFE", L"CFF", L"CFG", L"CFH", L"CFI", L"CFJ", L"CFK", L"CFL", L"CFM", L"CFN", L"CFO", L"CFP", L"CFQ", L"CFR", L"CFS", L"CFT", L"CFU", L"CFV", L"CFW", L"CFX", L"CFY", L"CFZ", L"CGA", L"CGB", L"CGC", L"CGD", L"CGE", L"CGF", L"CGG", L"CGH", L"CGI", L"CGJ", L"CGK", L"CGL", L"CGM", L"CGN", L"CGO", L"CGP", L"CGQ", L"CGR", L"CGS", L"CGT", L"CGU", L"CGV", L"CGW", L"CGX", L"CGY", L"CGZ", L"CHA", L"CHB", L"CHC", L"CHD", L"CHE", L"CHF", L"CHG", L"CHH", L"CHI", L"CHJ", L"CHK", L"CHL", L"CHM", L"CHN", L"CHO", L"CHP", L"CHQ", L"CHR", L"CHS", L"CHT", L"CHU", L"CHV", L"CHW", L"CHX", L"CHY", L"CHZ", L"CIA",
			L"CIB", L"CIC", L"CID", L"CIE", L"CIF", L"CIG", L"CIH", L"CII", L"CIJ", L"CIK", L"CIL", L"CIM", L"CIN", L"CIO", L"CIP", L"CIQ", L"CIR", L"CIS", L"CIT", L"CIU", L"CIV", L"CIW", L"CIX", L"CIY", L"CIZ", L"CJA", L"CJB", L"CJC", L"CJD", L"CJE", L"CJF", L"CJG", L"CJH", L"CJI", L"CJJ", L"CJK", L"CJL", L"CJM", L"CJN", L"CJO", L"CJP", L"CJQ", L"CJR", L"CJS", L"CJT", L"CJU", L"CJV", L"CJW", L"CJX", L"CJY", L"CJZ", L"CKA", L"CKB", L"CKC", L"CKD", L"CKE", L"CKF", L"CKG", L"CKH", L"CKI", L"CKJ", L"CKK", L"CKL", L"CKM", L"CKN", L"CKO", L"CKP", L"CKQ", L"CKR", L"CKS", L"CKT", L"CKU", L"CKV", L"CKW", L"CKX", L"CKY", L"CKZ", L"CLA", L"CLB", L"CLC", L"CLD", L"CLE", L"CLF", L"CLG", L"CLH", L"CLI", L"CLJ", L"CLK", L"CLL", L"CLM", L"CLN", L"CLO", L"CLP", L"CLQ", L"CLR", L"CLS", L"CLT", L"CLU", L"CLV", L"CLW", L"CLX", L"CLY", L"CLZ", L"CMA", L"CMB", L"CMC", L"CMD", L"CME", L"CMF", L"CMG", L"CMH", L"CMI", L"CMJ", L"CMK", L"CML", L"CMM", L"CMN", L"CMO", L"CMP", L"CMQ", L"CMR", L"CMS", L"CMT", L"CMU", L"CMV", L"CMW", L"CMX", L"CMY", L"CMZ", L"CNA", L"CNB", L"CNC", L"CND", L"CNE", L"CNF", L"CNG", L"CNH", L"CNI", L"CNJ", L"CNK", L"CNL", L"CNM", L"CNN", L"CNO", L"CNP", L"CNQ", L"CNR", L"CNS", L"CNT", L"CNU", L"CNV", L"CNW", L"CNX", L"CNY", L"CNZ", L"COA", L"COB", L"COC", L"COD", L"COE", L"COF", L"COG", L"COH", L"COI", L"COJ", L"COK", L"COL", L"COM", L"CON", L"COO", L"COP", L"COQ", L"COR", L"COS", L"COT", L"COU", L"COV", L"COW", L"COX", L"COY", L"COZ", L"CPA", L"CPB", L"CPC", L"CPD", L"CPE", L"CPF", L"CPG", L"CPH", L"CPI", L"CPJ", L"CPK", L"CPL", L"CPM", L"CPN", L"CPO", L"CPP", L"CPQ", L"CPR", L"CPS", L"CPT", L"CPU", L"CPV", L"CPW", L"CPX", L"CPY", L"CPZ", L"CQA", L"CQB", L"CQC", L"CQD", L"CQE", L"CQF", L"CQG", L"CQH", L"CQI", L"CQJ", L"CQK", L"CQL", L"CQM", L"CQN", L"CQO", L"CQP", L"CQQ", L"CQR", L"CQS", L"CQT", L"CQU", L"CQV", L"CQW", L"CQX", L"CQY", L"CQZ", L"CRA", L"CRB", L"CRC", L"CRD", L"CRE", L"CRF", L"CRG", L"CRH", L"CRI", L"CRJ", L"CRK", L"CRL", L"CRM", L"CRN", L"CRO", L"CRP", L"CRQ", L"CRR", L"CRS", L"CRT", L"CRU", L"CRV", L"CRW", L"CRX", L"CRY", L"CRZ", L"CSA", L"CSB", L"CSC", L"CSD", L"CSE", L"CSF", L"CSG", L"CSH", L"CSI", L"CSJ", L"CSK", L"CSL", L"CSM", L"CSN", L"CSO", L"CSP", L"CSQ", L"CSR", L"CSS", L"CST", L"CSU", L"CSV", L"CSW", L"CSX", L"CSY", L"CSZ", L"CTA", L"CTB", L"CTC", L"CTD", L"CTE", L"CTF", L"CTG", L"CTH", L"CTI", L"CTJ", L"CTK", L"CTL", L"CTM", L"CTN", L"CTO", L"CTP", L"CTQ", L"CTR", L"CTS", L"CTT", L"CTU", L"CTV", L"CTW", L"CTX", L"CTY", L"CTZ", L"CUA", L"CUB", L"CUC", L"CUD", L"CUE", L"CUF", L"CUG", L"CUH", L"CUI", L"CUJ", L"CUK", L"CUL", L"CUM", L"CUN", L"CUO", L"CUP", L"CUQ", L"CUR", L"CUS", L"CUT", L"CUU", L"CUV", L"CUW", L"CUX", L"CUY", L"CUZ", L"CVA", L"CVB", L"CVC", L"CVD", L"CVE", L"CVF", L"CVG", L"CVH", L"CVI", L"CVJ", L"CVK", L"CVL", L"CVM", L"CVN", L"CVO", L"CVP", L"CVQ", L"CVR", L"CVS", L"CVT", L"CVU", L"CVV", L"CVW", L"CVX", L"CVY", L"CVZ", L"CWA", L"CWB", L"CWC", L"CWD", L"CWE", L"CWF", L"CWG", L"CWH", L"CWI", L"CWJ", L"CWK", L"CWL", L"CWM", L"CWN", L"CWO", L"CWP", L"CWQ", L"CWR", L"CWS", L"CWT", L"CWU", L"CWV", L"CWW", L"CWX", L"CWY", L"CWZ", L"CXA", L"CXB", L"CXC", L"CXD", L"CXE", L"CXF", L"CXG", L"CXH", L"CXI", L"CXJ", L"CXK", L"CXL", L"CXM", L"CXN", L"CXO", L"CXP", L"CXQ", L"CXR", L"CXS", L"CXT", L"CXU", L"CXV", L"CXW", L"CXX", L"CXY", L"CXZ", L"CYA", L"CYB", L"CYC", L"CYD", L"CYE", L"CYF", L"CYG", L"CYH", L"CYI", L"CYJ", L"CYK", L"CYL", L"CYM", L"CYN", L"CYO", L"CYP", L"CYQ", L"CYR", L"CYS", L"CYT", L"CYU", L"CYV", L"CYW", L"CYX", L"CYY", L"CYZ", L"CZA", L"CZB", L"CZC", L"CZD", L"CZE", L"CZF", L"CZG", L"CZH", L"CZI", L"CZJ", L"CZK", L"CZL", L"CZM", L"CZN", L"CZO", L"CZP", L"CZQ", L"CZR", L"CZS", L"CZT", L"CZU", L"CZV", L"CZW", L"CZX", L"CZY", L"CZZ", L"DAA", L"DAB", L"DAC", L"DAD", L"DAE", L"DAF", L"DAG", L"DAH", L"DAI", L"DAJ", L"DAK", L"DAL", L"DAM", L"DAN", L"DAO", L"DAP", L"DAQ", L"DAR", L"DAS", L"DAT", L"DAU", L"DAV", L"DAW", L"DAX", L"DAY", L"DAZ", L"DBA", L"DBB", L"DBC", L"DBD", L"DBE", L"DBF", L"DBG", L"DBH", L"DBI", L"DBJ", L"DBK", L"DBL", L"DBM", L"DBN", L"DBO", L"DBP", L"DBQ", L"DBR", L"DBS", L"DBT", L"DBU", L"DBV", L"DBW", L"DBX", L"DBY", L"DBZ", L"DCA", L"DCB", L"DCC", L"DCD", L"DCE", L"DCF", L"DCG", L"DCH", L"DCI", L"DCJ", L"DCK", L"DCL", L"DCM", L"DCN", L"DCO", L"DCP", L"DCQ", L"DCR", L"DCS", L"DCT", L"DCU", L"DCV", L"DCW", L"DCX", L"DCY", L"DCZ", L"DDA",
			L"DDB", L"DDC", L"DDD", L"DDE", L"DDF", L"DDG", L"DDH", L"DDI", L"DDJ", L"DDK", L"DDL", L"DDM", L"DDN", L"DDO", L"DDP", L"DDQ", L"DDR", L"DDS", L"DDT", L"DDU", L"DDV", L"DDW", L"DDX", L"DDY", L"DDZ", L"DEA", L"DEB", L"DEC", L"DED", L"DEE", L"DEF", L"DEG", L"DEH", L"DEI", L"DEJ", L"DEK", L"DEL", L"DEM", L"DEN", L"DEO", L"DEP", L"DEQ", L"DER", L"DES", L"DET", L"DEU", L"DEV", L"DEW", L"DEX", L"DEY", L"DEZ", L"DFA", L"DFB", L"DFC", L"DFD", L"DFE", L"DFF", L"DFG", L"DFH", L"DFI", L"DFJ", L"DFK", L"DFL", L"DFM", L"DFN", L"DFO", L"DFP", L"DFQ", L"DFR", L"DFS", L"DFT", L"DFU", L"DFV", L"DFW", L"DFX", L"DFY", L"DFZ", L"DGA", L"DGB", L"DGC", L"DGD", L"DGE", L"DGF", L"DGG", L"DGH", L"DGI", L"DGJ", L"DGK", L"DGL", L"DGM", L"DGN", L"DGO", L"DGP", L"DGQ", L"DGR", L"DGS", L"DGT", L"DGU", L"DGV", L"DGW", L"DGX", L"DGY", L"DGZ", L"DHA", L"DHB", L"DHC", L"DHD", L"DHE", L"DHF", L"DHG", L"DHH", L"DHI", L"DHJ", L"DHK", L"DHL", L"DHM", L"DHN", L"DHO", L"DHP", L"DHQ", L"DHR", L"DHS", L"DHT", L"DHU", L"DHV", L"DHW", L"DHX", L"DHY", L"DHZ", L"DIA", L"DIB", L"DIC", L"DID", L"DIE", L"DIF", L"DIG", L"DIH", L"DII", L"DIJ", L"DIK", L"DIL", L"DIM", L"DIN", L"DIO", L"DIP", L"DIQ", L"DIR", L"DIS", L"DIT", L"DIU", L"DIV", L"DIW", L"DIX", L"DIY", L"DIZ", L"DJA", L"DJB", L"DJC", L"DJD", L"DJE", L"DJF", L"DJG", L"DJH", L"DJI", L"DJJ", L"DJK", L"DJL", L"DJM", L"DJN", L"DJO", L"DJP", L"DJQ", L"DJR", L"DJS", L"DJT", L"DJU", L"DJV", L"DJW", L"DJX", L"DJY", L"DJZ", L"DKA", L"DKB", L"DKC", L"DKD", L"DKE", L"DKF", L"DKG", L"DKH", L"DKI", L"DKJ", L"DKK", L"DKL", L"DKM", L"DKN", L"DKO", L"DKP", L"DKQ", L"DKR", L"DKS", L"DKT", L"DKU", L"DKV", L"DKW", L"DKX", L"DKY", L"DKZ", L"DLA", L"DLB", L"DLC", L"DLD", L"DLE", L"DLF", L"DLG", L"DLH", L"DLI", L"DLJ", L"DLK", L"DLL", L"DLM", L"DLN", L"DLO", L"DLP", L"DLQ", L"DLR", L"DLS", L"DLT", L"DLU", L"DLV", L"DLW", L"DLX", L"DLY", L"DLZ", L"DMA", L"DMB", L"DMC", L"DMD", L"DME", L"DMF", L"DMG", L"DMH", L"DMI", L"DMJ", L"DMK", L"DML", L"DMM", L"DMN", L"DMO", L"DMP", L"DMQ", L"DMR", L"DMS", L"DMT", L"DMU", L"DMV", L"DMW", L"DMX", L"DMY", L"DMZ", L"DNA", L"DNB", L"DNC", L"DND", L"DNE", L"DNF", L"DNG", L"DNH", L"DNI", L"DNJ", L"DNK", L"DNL", L"DNM", L"DNN", L"DNO", L"DNP", L"DNQ", L"DNR", L"DNS", L"DNT", L"DNU", L"DNV", L"DNW", L"DNX", L"DNY", L"DNZ", L"DOA", L"DOB", L"DOC", L"DOD", L"DOE", L"DOF", L"DOG", L"DOH", L"DOI", L"DOJ", L"DOK", L"DOL", L"DOM", L"DON", L"DOO", L"DOP", L"DOQ", L"DOR", L"DOS", L"DOT", L"DOU", L"DOV", L"DOW", L"DOX", L"DOY", L"DOZ", L"DPA", L"DPB", L"DPC", L"DPD", L"DPE", L"DPF", L"DPG", L"DPH", L"DPI", L"DPJ", L"DPK", L"DPL", L"DPM", L"DPN", L"DPO", L"DPP", L"DPQ", L"DPR", L"DPS", L"DPT", L"DPU", L"DPV", L"DPW", L"DPX", L"DPY", L"DPZ", L"DQA", L"DQB", L"DQC", L"DQD", L"DQE", L"DQF", L"DQG", L"DQH", L"DQI", L"DQJ", L"DQK", L"DQL", L"DQM", L"DQN", L"DQO", L"DQP", L"DQQ", L"DQR", L"DQS", L"DQT", L"DQU", L"DQV", L"DQW", L"DQX", L"DQY", L"DQZ", L"DRA", L"DRB", L"DRC", L"DRD", L"DRE", L"DRF", L"DRG", L"DRH", L"DRI", L"DRJ", L"DRK", L"DRL", L"DRM", L"DRN", L"DRO", L"DRP", L"DRQ", L"DRR", L"DRS", L"DRT", L"DRU", L"DRV", L"DRW", L"DRX", L"DRY", L"DRZ", L"DSA", L"DSB", L"DSC", L"DSD", L"DSE", L"DSF", L"DSG", L"DSH", L"DSI", L"DSJ", L"DSK", L"DSL", L"DSM", L"DSN", L"DSO", L"DSP", L"DSQ", L"DSR", L"DSS", L"DST", L"DSU", L"DSV", L"DSW", L"DSX", L"DSY", L"DSZ", L"DTA", L"DTB", L"DTC", L"DTD", L"DTE", L"DTF", L"DTG", L"DTH", L"DTI", L"DTJ", L"DTK", L"DTL", L"DTM", L"DTN", L"DTO", L"DTP", L"DTQ", L"DTR", L"DTS", L"DTT", L"DTU", L"DTV", L"DTW", L"DTX", L"DTY", L"DTZ", L"DUA", L"DUB", L"DUC", L"DUD", L"DUE", L"DUF", L"DUG", L"DUH", L"DUI", L"DUJ", L"DUK", L"DUL", L"DUM", L"DUN", L"DUO", L"DUP", L"DUQ", L"DUR", L"DUS", L"DUT", L"DUU", L"DUV", L"DUW", L"DUX", L"DUY", L"DUZ", L"DVA", L"DVB", L"DVC", L"DVD", L"DVE", L"DVF", L"DVG", L"DVH", L"DVI", L"DVJ", L"DVK", L"DVL", L"DVM", L"DVN", L"DVO", L"DVP", L"DVQ", L"DVR", L"DVS", L"DVT", L"DVU", L"DVV", L"DVW", L"DVX", L"DVY", L"DVZ", L"DWA", L"DWB", L"DWC", L"DWD", L"DWE", L"DWF", L"DWG", L"DWH", L"DWI", L"DWJ", L"DWK", L"DWL", L"DWM", L"DWN", L"DWO", L"DWP", L"DWQ", L"DWR", L"DWS", L"DWT", L"DWU", L"DWV", L"DWW", L"DWX", L"DWY", L"DWZ", L"DXA", L"DXB", L"DXC", L"DXD", L"DXE", L"DXF", L"DXG", L"DXH", L"DXI", L"DXJ", L"DXK", L"DXL", L"DXM", L"DXN", L"DXO", L"DXP", L"DXQ", L"DXR", L"DXS", 
			L"DXT", L"DXU", L"DXV", L"DXW", L"DXX", L"DXY", L"DXZ", L"DYA", L"DYB", L"DYC", L"DYD", L"DYE", L"DYF", L"DYG", L"DYH", L"DYI", L"DYJ", L"DYK", L"DYL", L"DYM", L"DYN", L"DYO", L"DYP", L"DYQ", L"DYR", L"DYS", L"DYT", L"DYU", L"DYV", L"DYW", L"DYX", L"DYY", L"DYZ", L"DZA", L"DZB", L"DZC", L"DZD", L"DZE", L"DZF", L"DZG", L"DZH", L"DZI", L"DZJ", L"DZK", L"DZL", L"DZM", L"DZN", L"DZO", L"DZP", L"DZQ", L"DZR", L"DZS", L"DZT", L"DZU", L"DZV", L"DZW", L"DZX", L"DZY", L"DZZ", L"EAA", L"EAB", L"EAC", L"EAD", L"EAE", L"EAF", L"EAG", L"EAH", L"EAI", L"EAJ", L"EAK", L"EAL", L"EAM", L"EAN", L"EAO", L"EAP", L"EAQ", L"EAR", L"EAS", L"EAT", L"EAU", L"EAV", L"EAW", L"EAX", L"EAY", L"EAZ", L"EBA", L"EBB", L"EBC", L"EBD", L"EBE", L"EBF", L"EBG", L"EBH", L"EBI", L"EBJ", L"EBK", L"EBL", L"EBM", L"EBN", L"EBO", L"EBP", L"EBQ", L"EBR", L"EBS", L"EBT", L"EBU", L"EBV", L"EBW", L"EBX", L"EBY", L"EBZ", L"ECA", L"ECB", L"ECC", L"ECD", L"ECE", L"ECF", L"ECG", L"ECH", L"ECI", L"ECJ", L"ECK", L"ECL", L"ECM", L"ECN", L"ECO", L"ECP", L"ECQ", L"ECR", L"ECS", L"ECT", L"ECU", L"ECV", L"ECW", L"ECX", L"ECY", L"ECZ", L"EDA", L"EDB", L"EDC", L"EDD", L"EDE", L"EDF", L"EDG", L"EDH", L"EDI", L"EDJ", L"EDK", L"EDL", L"EDM", L"EDN", L"EDO", L"EDP", L"EDQ", L"EDR", L"EDS", L"EDT", L"EDU", L"EDV", L"EDW", L"EDX", L"EDY", L"EDZ", L"EEA", L"EEB", L"EEC", L"EED", L"EEE", L"EEF", L"EEG", L"EEH", L"EEI", L"EEJ", L"EEK", L"EEL", L"EEM", L"EEN", L"EEO", L"EEP", L"EEQ", L"EER", L"EES", L"EET", L"EEU", L"EEV", L"EEW", L"EEX", L"EEY", L"EEZ", L"EFA", L"EFB", L"EFC", L"EFD", L"EFE", L"EFF", L"EFG", L"EFH", L"EFI", L"EFJ", L"EFK", L"EFL", L"EFM", L"EFN", L"EFO", L"EFP", L"EFQ", L"EFR", L"EFS", L"EFT", L"EFU", L"EFV", L"EFW", L"EFX", L"EFY", L"EFZ", L"EGA", L"EGB", L"EGC", L"EGD", L"EGE", L"EGF", L"EGG", L"EGH", L"EGI", L"EGJ", L"EGK", L"EGL", L"EGM", L"EGN", L"EGO", L"EGP", L"EGQ", L"EGR", L"EGS", L"EGT", L"EGU", L"EGV", L"EGW", L"EGX", L"EGY", L"EGZ", L"EHA", L"EHB", L"EHC", L"EHD", L"EHE", L"EHF", L"EHG", L"EHH", L"EHI", L"EHJ", L"EHK", L"EHL", L"EHM", L"EHN", L"EHO", L"EHP", L"EHQ", L"EHR", L"EHS", L"EHT", L"EHU", L"EHV", L"EHW", L"EHX", L"EHY", L"EHZ", L"EIA", L"EIB", L"EIC", L"EID", L"EIE", L"EIF", L"EIG", L"EIH", L"EII", L"EIJ", L"EIK", L"EIL", L"EIM", L"EIN", L"EIO", L"EIP", L"EIQ", L"EIR", L"EIS", L"EIT", L"EIU", L"EIV", L"EIW", L"EIX", L"EIY", L"EIZ", L"EJA", L"EJB", L"EJC", L"EJD", L"EJE", L"EJF", L"EJG", L"EJH", L"EJI", L"EJJ", L"EJK", L"EJL", L"EJM", L"EJN", L"EJO", L"EJP", L"EJQ", L"EJR", L"EJS", L"EJT", L"EJU", L"EJV", L"EJW", L"EJX", L"EJY", L"EJZ", L"EKA", L"EKB", L"EKC", L"EKD", L"EKE", L"EKF", L"EKG", L"EKH", L"EKI", L"EKJ", L"EKK", L"EKL", L"EKM", L"EKN", L"EKO", L"EKP", L"EKQ", L"EKR", L"EKS", L"EKT", L"EKU", L"EKV", L"EKW", L"EKX", L"EKY", L"EKZ", L"ELA", L"ELB", L"ELC", L"ELD", L"ELE", L"ELF", L"ELG", L"ELH", L"ELI", L"ELJ", L"ELK", L"ELL", L"ELM", L"ELN", L"ELO", L"ELP", L"ELQ", L"ELR", L"ELS", L"ELT", L"ELU", L"ELV", L"ELW", L"ELX", L"ELY", L"ELZ", L"EMA", L"EMB", L"EMC", L"EMD", L"EME", L"EMF", L"EMG", L"EMH", L"EMI", L"EMJ", L"EMK", L"EML", L"EMM", L"EMN", L"EMO", L"EMP", L"EMQ", L"EMR", L"EMS", L"EMT", L"EMU", L"EMV", L"EMW", L"EMX", L"EMY", L"EMZ", L"ENA", L"ENB", L"ENC", L"END", L"ENE", L"ENF", L"ENG", L"ENH", L"ENI", L"ENJ", L"ENK", L"ENL", L"ENM", L"ENN", L"ENO", L"ENP", L"ENQ", L"ENR", L"ENS", L"ENT", L"ENU", L"ENV", L"ENW", L"ENX", L"ENY", L"ENZ", L"EOA", L"EOB", L"EOC", L"EOD", L"EOE", L"EOF", L"EOG", L"EOH", L"EOI", L"EOJ", L"EOK", L"EOL", L"EOM", L"EON", L"EOO", L"EOP", L"EOQ", L"EOR", L"EOS", L"EOT", L"EOU", L"EOV", L"EOW", L"EOX", L"EOY", L"EOZ", L"EPA", L"EPB", L"EPC", L"EPD", L"EPE", L"EPF", L"EPG", L"EPH", L"EPI", L"EPJ", L"EPK", L"EPL", L"EPM", L"EPN", L"EPO", L"EPP", L"EPQ", L"EPR", L"EPS", L"EPT", L"EPU", L"EPV", L"EPW", L"EPX", L"EPY", L"EPZ", L"EQA", L"EQB", L"EQC", L"EQD", L"EQE", L"EQF", L"EQG", L"EQH", L"EQI", L"EQJ", L"EQK", L"EQL", L"EQM", L"EQN", L"EQO", L"EQP", L"EQQ", L"EQR", L"EQS", L"EQT", L"EQU", L"EQV", L"EQW", L"EQX", L"EQY", L"EQZ", L"ERA", L"ERB", L"ERC", L"ERD", L"ERE", L"ERF", L"ERG", L"ERH", L"ERI", L"ERJ", L"ERK", L"ERL", L"ERM", L"ERN", L"ERO", L"ERP", L"ERQ", L"ERR", L"ERS", L"ERT", L"ERU", L"ERV", L"ERW", L"ERX", L"ERY", L"ERZ", L"ESA", L"ESB", L"ESC", L"ESD", L"ESE", L"ESF", L"ESG", L"ESH", L"ESI", L"ESJ", L"ESK", 
			L"ESL", L"ESM", L"ESN", L"ESO", L"ESP", L"ESQ", L"ESR", L"ESS", L"EST", L"ESU", L"ESV", L"ESW", L"ESX", L"ESY", L"ESZ", L"ETA", L"ETB", L"ETC", L"ETD", L"ETE", L"ETF", L"ETG", L"ETH", L"ETI", L"ETJ", L"ETK", L"ETL", L"ETM", L"ETN", L"ETO", L"ETP", L"ETQ", L"ETR", L"ETS", L"ETT", L"ETU", L"ETV", L"ETW", L"ETX", L"ETY", L"ETZ", L"EUA", L"EUB", L"EUC", L"EUD", L"EUE", L"EUF", L"EUG", L"EUH", L"EUI", L"EUJ", L"EUK", L"EUL", L"EUM", L"EUN", L"EUO", L"EUP", L"EUQ", L"EUR", L"EUS", L"EUT", L"EUU", L"EUV", L"EUW", L"EUX", L"EUY", L"EUZ", L"EVA", L"EVB", L"EVC", L"EVD", L"EVE", L"EVF", L"EVG", L"EVH", L"EVI", L"EVJ", L"EVK", L"EVL", L"EVM", L"EVN", L"EVO", L"EVP", L"EVQ", L"EVR", L"EVS", L"EVT", L"EVU", L"EVV", L"EVW", L"EVX", L"EVY", L"EVZ", L"EWA", L"EWB", L"EWC", L"EWD", L"EWE", L"EWF", L"EWG", L"EWH", L"EWI", L"EWJ", L"EWK", L"EWL", L"EWM", L"EWN", L"EWO", L"EWP", L"EWQ", L"EWR", L"EWS", L"EWT", L"EWU", L"EWV", L"EWW", L"EWX", L"EWY", L"EWZ", L"EXA", L"EXB", L"EXC", L"EXD", L"EXE", L"EXF", L"EXG", L"EXH", L"EXI", L"EXJ", L"EXK", L"EXL", L"EXM", L"EXN", L"EXO", L"EXP", L"EXQ", L"EXR", L"EXS", L"EXT", L"EXU", L"EXV", L"EXW", L"EXX", L"EXY", L"EXZ", L"EYA", L"EYB", L"EYC", L"EYD", L"EYE", L"EYF", L"EYG", L"EYH", L"EYI", L"EYJ", L"EYK", L"EYL", L"EYM", L"EYN", L"EYO", L"EYP", L"EYQ", L"EYR", L"EYS", L"EYT", L"EYU", L"EYV", L"EYW", L"EYX", L"EYY", L"EYZ", L"EZA", L"EZB", L"EZC", L"EZD", L"EZE", L"EZF", L"EZG", L"EZH", L"EZI", L"EZJ", L"EZK", L"EZL", L"EZM", L"EZN", L"EZO", L"EZP", L"EZQ", L"EZR", L"EZS", L"EZT", L"EZU", L"EZV", L"EZW", L"EZX", L"EZY", L"EZZ", L"FAA", L"FAB", L"FAC", L"FAD", L"FAE", L"FAF", L"FAG", L"FAH", L"FAI", L"FAJ", L"FAK", L"FAL", L"FAM", L"FAN", L"FAO", L"FAP", L"FAQ", L"FAR", L"FAS", L"FAT", L"FAU", L"FAV", L"FAW", L"FAX", L"FAY", L"FAZ", L"FBA", L"FBB", L"FBC", L"FBD", L"FBE", L"FBF", L"FBG", L"FBH", L"FBI", L"FBJ", L"FBK", L"FBL", L"FBM", L"FBN", L"FBO", L"FBP", L"FBQ", L"FBR", L"FBS", L"FBT", L"FBU", L"FBV", L"FBW", L"FBX", L"FBY", L"FBZ", L"FCA", L"FCB", L"FCC", L"FCD", L"FCE", L"FCF", L"FCG", L"FCH", L"FCI", L"FCJ", L"FCK", L"FCL", L"FCM", L"FCN", L"FCO", L"FCP", L"FCQ", L"FCR", L"FCS", L"FCT", L"FCU", L"FCV", L"FCW", L"FCX", L"FCY", L"FCZ", L"FDA", L"FDB", L"FDC", L"FDD", L"FDE", L"FDF",
			L"FDG", L"FDH", L"FDI", L"FDJ", L"FDK", L"FDL", L"FDM", L"FDN", L"FDO", L"FDP", L"FDQ", L"FDR", L"FDS", L"FDT", L"FDU", L"FDV", L"FDW", L"FDX", L"FDY", L"FDZ", L"FEA", L"FEB", L"FEC", L"FED", L"FEE", L"FEF", L"FEG", L"FEH", L"FEI", L"FEJ", L"FEK", L"FEL", L"FEM", L"FEN", L"FEO", L"FEP", L"FEQ", L"FER", L"FES", L"FET", L"FEU", L"FEV", L"FEW", L"FEX", L"FEY", L"FEZ", L"FFA", L"FFB", L"FFC", L"FFD", L"FFE", L"FFF", L"FFG", L"FFH", L"FFI", L"FFJ", L"FFK", L"FFL", L"FFM", L"FFN", L"FFO", L"FFP", L"FFQ", L"FFR", L"FFS", L"FFT", L"FFU", L"FFV", L"FFW", L"FFX", L"FFY", L"FFZ", L"FGA", L"FGB", L"FGC", L"FGD", L"FGE", L"FGF", L"FGG", L"FGH", L"FGI", L"FGJ", L"FGK", L"FGL", L"FGM", L"FGN", L"FGO", L"FGP", L"FGQ", L"FGR", L"FGS", L"FGT", L"FGU", L"FGV", L"FGW", L"FGX", L"FGY", L"FGZ", L"FHA", L"FHB", L"FHC", L"FHD", L"FHE", L"FHF", L"FHG", L"FHH", L"FHI", L"FHJ", L"FHK", L"FHL", L"FHM", L"FHN", L"FHO", L"FHP", L"FHQ", L"FHR", L"FHS", L"FHT", L"FHU", L"FHV", L"FHW", L"FHX", L"FHY", L"FHZ", L"FIA", L"FIB", L"FIC", L"FID", L"FIE", L"FIF", L"FIG", L"FIH", L"FII", L"FIJ", L"FIK", L"FIL", L"FIM", L"FIN", L"FIO", L"FIP", L"FIQ", L"FIR", L"FIS", L"FIT", L"FIU", L"FIV", L"FIW", L"FIX", L"FIY", L"FIZ", L"FJA", L"FJB", L"FJC", L"FJD", L"FJE", L"FJF", L"FJG", L"FJH", L"FJI", L"FJJ", L"FJK", L"FJL", L"FJM", L"FJN", L"FJO", L"FJP", L"FJQ", L"FJR", L"FJS", L"FJT", L"FJU", L"FJV", L"FJW", L"FJX", L"FJY", L"FJZ", L"FKA", L"FKB", L"FKC", L"FKD", L"FKE", L"FKF", L"FKG", L"FKH", L"FKI", L"FKJ", L"FKK", L"FKL", L"FKM", L"FKN", L"FKO", L"FKP", L"FKQ", L"FKR", L"FKS", L"FKT", L"FKU", L"FKV", L"FKW", L"FKX", L"FKY", L"FKZ", L"FLA", L"FLB", L"FLC", L"FLD", L"FLE", L"FLF", L"FLG", L"FLH", L"FLI", L"FLJ", L"FLK", L"FLL", L"FLM", L"FLN", L"FLO", L"FLP", L"FLQ", L"FLR", L"FLS", L"FLT", L"FLU", L"FLV", L"FLW", L"FLX", L"FLY", L"FLZ", L"FMA", L"FMB", L"FMC", L"FMD", L"FME", L"FMF", L"FMG", L"FMH", L"FMI", L"FMJ", L"FMK", L"FML", L"FMM", L"FMN", L"FMO", L"FMP", L"FMQ", L"FMR", L"FMS", L"FMT", L"FMU", L"FMV", L"FMW", L"FMX", L"FMY", L"FMZ", L"FNA", L"FNB", L"FNC", L"FND", L"FNE", L"FNF", L"FNG", L"FNH", L"FNI", L"FNJ", L"FNK", L"FNL", L"FNM", L"FNN", L"FNO", L"FNP", L"FNQ", L"FNR", L"FNS", L"FNT", L"FNU", L"FNV", L"FNW", L"FNX", L"FNY", L"FNZ", L"FOA", L"FOB", L"FOC", L"FOD", L"FOE", L"FOF", L"FOG", L"FOH", L"FOI", L"FOJ", L"FOK", L"FOL", L"FOM", L"FON", L"FOO", L"FOP", L"FOQ", L"FOR", L"FOS", L"FOT", L"FOU", L"FOV", L"FOW", L"FOX", L"FOY", L"FOZ", L"FPA", L"FPB", L"FPC", L"FPD", L"FPE", L"FPF", L"FPG", L"FPH", L"FPI", L"FPJ", L"FPK", L"FPL", L"FPM", L"FPN", L"FPO", L"FPP", L"FPQ", L"FPR", L"FPS", L"FPT", L"FPU", L"FPV", L"FPW", L"FPX", L"FPY", L"FPZ", L"FQA", L"FQB", L"FQC", L"FQD", L"FQE", L"FQF", L"FQG", L"FQH", L"FQI", L"FQJ", L"FQK", L"FQL", L"FQM", L"FQN", L"FQO", L"FQP", L"FQQ", L"FQR", L"FQS", L"FQT", L"FQU", L"FQV", L"FQW", L"FQX", L"FQY", L"FQZ", L"FRA", L"FRB", L"FRC", L"FRD", L"FRE", L"FRF", L"FRG", L"FRH", L"FRI", L"FRJ", L"FRK", L"FRL", L"FRM", L"FRN", L"FRO", L"FRP", L"FRQ", L"FRR", L"FRS", L"FRT", L"FRU", L"FRV", L"FRW", L"FRX", L"FRY", L"FRZ", L"FSA", L"FSB", L"FSC", L"FSD", L"FSE", L"FSF", L"FSG", L"FSH", L"FSI", L"FSJ", L"FSK", L"FSL", L"FSM", L"FSN", L"FSO", L"FSP", L"FSQ", L"FSR", L"FSS", L"FST", L"FSU", L"FSV", L"FSW", L"FSX", L"FSY", L"FSZ", L"FTA", L"FTB", L"FTC", L"FTD", L"FTE", L"FTF", L"FTG", L"FTH", L"FTI", L"FTJ", L"FTK", L"FTL", L"FTM", L"FTN", L"FTO", L"FTP", L"FTQ", L"FTR", L"FTS", L"FTT", L"FTU", L"FTV", L"FTW", L"FTX", L"FTY", L"FTZ", L"FUA", L"FUB", L"FUC", L"FUD", L"FUE", L"FUF", L"FUG", L"FUH", L"FUI", L"FUJ", L"FUK", L"FUL", L"FUM", L"FUN", L"FUO", L"FUP", L"FUQ", L"FUR", L"FUS", L"FUT", L"FUU", L"FUV", L"FUW", L"FUX", L"FUY", L"FUZ", L"FVA", L"FVB", L"FVC", L"FVD", L"FVE", L"FVF", L"FVG", L"FVH", L"FVI", L"FVJ", L"FVK", L"FVL", L"FVM", L"FVN", L"FVO", L"FVP", L"FVQ", L"FVR", L"FVS", L"FVT", L"FVU", L"FVV", L"FVW", L"FVX", L"FVY", L"FVZ", L"FWA", L"FWB", L"FWC", L"FWD", L"FWE", L"FWF", L"FWG", L"FWH", L"FWI", L"FWJ", L"FWK", L"FWL", L"FWM", L"FWN", L"FWO", L"FWP", L"FWQ", L"FWR", L"FWS", L"FWT", L"FWU", L"FWV", L"FWW", L"FWX", L"FWY", L"FWZ", L"FXA", L"FXB", L"FXC", L"FXD", L"FXE", L"FXF", L"FXG", L"FXH", L"FXI", L"FXJ", L"FXK", L"FXL", L"FXM", L"FXN", L"FXO", L"FXP", L"FXQ", L"FXR", L"FXS", L"FXT", L"FXU", L"FXV", L"FXW", L"FXX", 
			L"FXY", L"FXZ", L"FYA", L"FYB", L"FYC", L"FYD", L"FYE", L"FYF",
			L"FYG", L"FYH", L"FYI", L"FYJ", L"FYK", L"FYL", L"FYM", L"FYN", L"FYO", L"FYP", L"FYQ", L"FYR", L"FYS", L"FYT", L"FYU", L"FYV", L"FYW", L"FYX", L"FYY", L"FYZ", L"FZA", L"FZB", L"FZC", L"FZD", L"FZE", L"FZF", L"FZG", L"FZH", L"FZI", L"FZJ", L"FZK", L"FZL", L"FZM", L"FZN", L"FZO", L"FZP", L"FZQ", L"FZR", L"FZS", L"FZT", L"FZU", L"FZV", L"FZW", L"FZX", L"FZY", L"FZZ", L"GAA", L"GAB", L"GAC", L"GAD", L"GAE", L"GAF", L"GAG", L"GAH", L"GAI", L"GAJ", L"GAK", L"GAL", L"GAM", L"GAN", L"GAO", L"GAP", L"GAQ", L"GAR", L"GAS", L"GAT", L"GAU", L"GAV", L"GAW", L"GAX", L"GAY", L"GAZ", L"GBA", L"GBB", L"GBC", L"GBD", L"GBE", L"GBF", L"GBG", L"GBH", L"GBI", L"GBJ", L"GBK", L"GBL", L"GBM", L"GBN", L"GBO", L"GBP", L"GBQ", L"GBR", L"GBS", L"GBT", L"GBU", L"GBV", L"GBW", L"GBX", L"GBY", L"GBZ", L"GCA", L"GCB", L"GCC", L"GCD", L"GCE", L"GCF", L"GCG", L"GCH", L"GCI", L"GCJ", L"GCK", L"GCL", L"GCM", L"GCN", L"GCO", L"GCP", L"GCQ", L"GCR", L"GCS", L"GCT", L"GCU", L"GCV", L"GCW", L"GCX", L"GCY", L"GCZ", L"GDA", L"GDB", L"GDC", L"GDD", L"GDE", L"GDF", L"GDG", L"GDH", L"GDI", L"GDJ", L"GDK", L"GDL", L"GDM", L"GDN", L"GDO", L"GDP", L"GDQ", L"GDR", L"GDS", L"GDT", L"GDU", L"GDV", L"GDW", L"GDX", L"GDY", L"GDZ", L"GEA", L"GEB", L"GEC", L"GED", L"GEE", L"GEF", L"GEG", L"GEH", L"GEI", L"GEJ", L"GEK", L"GEL", L"GEM", L"GEN", L"GEO", L"GEP", L"GEQ", L"GER", L"GES", L"GET", L"GEU", L"GEV", L"GEW", L"GEX", L"GEY", L"GEZ", L"GFA", L"GFB", L"GFC", L"GFD", L"GFE", L"GFF", L"GFG", L"GFH", L"GFI", L"GFJ", L"GFK", L"GFL", L"GFM", L"GFN", L"GFO", L"GFP", L"GFQ", L"GFR", L"GFS", L"GFT", L"GFU", L"GFV", L"GFW", L"GFX", L"GFY", L"GFZ", L"GGA", L"GGB", L"GGC", L"GGD", L"GGE", L"GGF", L"GGG", L"GGH", L"GGI", L"GGJ", L"GGK", L"GGL", L"GGM", L"GGN", L"GGO", L"GGP", L"GGQ", L"GGR", L"GGS", L"GGT", L"GGU", L"GGV", L"GGW", L"GGX", L"GGY", L"GGZ", L"GHA", L"GHB", L"GHC", L"GHD", L"GHE", L"GHF", L"GHG", L"GHH", L"GHI", L"GHJ", L"GHK", L"GHL", L"GHM", L"GHN", L"GHO", L"GHP", L"GHQ", L"GHR", L"GHS", L"GHT", L"GHU", L"GHV", L"GHW", L"GHX", L"GHY", L"GHZ", L"GIA", L"GIB", L"GIC", L"GID", L"GIE", L"GIF", L"GIG", L"GIH", L"GII", L"GIJ", L"GIK", L"GIL", L"GIM", L"GIN", L"GIO", L"GIP", L"GIQ", L"GIR", L"GIS",
			L"GIT", L"GIU", L"GIV", L"GIW", L"GIX", L"GIY", L"GIZ", L"GJA", L"GJB", L"GJC", L"GJD", L"GJE", L"GJF", L"GJG", L"GJH", L"GJI", L"GJJ", L"GJK", L"GJL", L"GJM", L"GJN", L"GJO", L"GJP", L"GJQ", L"GJR", L"GJS", L"GJT", L"GJU", L"GJV", L"GJW", L"GJX", L"GJY", L"GJZ", L"GKA", L"GKB", L"GKC", L"GKD", L"GKE", L"GKF", L"GKG", L"GKH", L"GKI", L"GKJ", L"GKK", L"GKL", L"GKM", L"GKN", L"GKO", L"GKP", L"GKQ", L"GKR", L"GKS", L"GKT", L"GKU", L"GKV", L"GKW", L"GKX", L"GKY", L"GKZ", L"GLA", L"GLB", L"GLC", L"GLD", L"GLE", L"GLF", L"GLG", L"GLH", L"GLI", L"GLJ", L"GLK", L"GLL", L"GLM", L"GLN", L"GLO", L"GLP", L"GLQ", L"GLR", L"GLS", L"GLT", L"GLU", L"GLV", L"GLW", L"GLX", L"GLY", L"GLZ", L"GMA", L"GMB", L"GMC", L"GMD", L"GME", L"GMF", L"GMG", L"GMH", L"GMI", L"GMJ", L"GMK", L"GML", L"GMM", L"GMN", L"GMO", L"GMP", L"GMQ", L"GMR", L"GMS", L"GMT", L"GMU", L"GMV", L"GMW", L"GMX", L"GMY", L"GMZ", L"GNA", L"GNB", L"GNC", L"GND", L"GNE", L"GNF", L"GNG", L"GNH", L"GNI", L"GNJ", L"GNK", L"GNL", L"GNM", L"GNN", L"GNO", L"GNP", L"GNQ", L"GNR", L"GNS", L"GNT", L"GNU", L"GNV", L"GNW", L"GNX", L"GNY", L"GNZ", L"GOA", L"GOB", L"GOC", L"GOD", L"GOE", L"GOF", L"GOG", L"GOH", L"GOI", L"GOJ", L"GOK", L"GOL", L"GOM", L"GON", L"GOO", L"GOP", L"GOQ", L"GOR", L"GOS", L"GOT", L"GOU", L"GOV", L"GOW", L"GOX", L"GOY", L"GOZ", L"GPA", L"GPB", L"GPC", L"GPD", L"GPE", L"GPF", L"GPG", L"GPH", L"GPI", L"GPJ", L"GPK", L"GPL", L"GPM", L"GPN", L"GPO", L"GPP", L"GPQ", L"GPR", L"GPS", L"GPT", L"GPU", L"GPV", L"GPW", L"GPX", L"GPY", L"GPZ", L"GQA", L"GQB", L"GQC", L"GQD", L"GQE", L"GQF", L"GQG", L"GQH", L"GQI", L"GQJ", L"GQK", L"GQL", L"GQM", L"GQN", L"GQO", L"GQP", L"GQQ", L"GQR", L"GQS", L"GQT", L"GQU", L"GQV", L"GQW", L"GQX", L"GQY", L"GQZ", L"GRA", L"GRB", L"GRC", L"GRD", L"GRE", L"GRF", L"GRG", L"GRH", L"GRI", L"GRJ", L"GRK", L"GRL", L"GRM", L"GRN", L"GRO", L"GRP", L"GRQ", L"GRR", L"GRS", L"GRT", L"GRU", L"GRV", L"GRW", L"GRX", L"GRY", L"GRZ", L"GSA", L"GSB", L"GSC", L"GSD", L"GSE", L"GSF", L"GSG", L"GSH", L"GSI", L"GSJ", L"GSK", L"GSL", L"GSM", L"GSN", L"GSO", L"GSP", L"GSQ", L"GSR", L"GSS", L"GST", L"GSU", L"GSV", L"GSW", L"GSX", L"GSY", L"GSZ", L"GTA", L"GTB", L"GTC", L"GTD", L"GTE", L"GTF", 
			L"GTG", L"GTH", L"GTI", L"GTJ", L"GTK", L"GTL", L"GTM", L"GTN", L"GTO", L"GTP", L"GTQ", L"GTR", L"GTS", L"GTT", L"GTU", L"GTV", L"GTW", L"GTX", L"GTY", L"GTZ", L"GUA", L"GUB", L"GUC", L"GUD", L"GUE", L"GUF", L"GUG", L"GUH", L"GUI", L"GUJ", L"GUK", L"GUL", L"GUM", L"GUN", L"GUO", L"GUP", L"GUQ", L"GUR", L"GUS", L"GUT", L"GUU", L"GUV", L"GUW", L"GUX", L"GUY", L"GUZ", L"GVA", L"GVB", L"GVC", L"GVD", L"GVE", L"GVF", L"GVG", L"GVH", L"GVI", L"GVJ", L"GVK", L"GVL", L"GVM", L"GVN", L"GVO", L"GVP", L"GVQ", L"GVR", L"GVS", L"GVT", L"GVU", L"GVV", L"GVW", L"GVX", L"GVY", L"GVZ", L"GWA", L"GWB", L"GWC", L"GWD", L"GWE", L"GWF", L"GWG", L"GWH", L"GWI", L"GWJ", L"GWK", L"GWL", L"GWM", L"GWN", L"GWO", L"GWP", L"GWQ", L"GWR", L"GWS", L"GWT", L"GWU", L"GWV", L"GWW", L"GWX", L"GWY", L"GWZ", L"GXA", L"GXB", L"GXC", L"GXD", L"GXE", L"GXF", L"GXG", L"GXH", L"GXI", L"GXJ", L"GXK", L"GXL", L"GXM", L"GXN", L"GXO", L"GXP", L"GXQ", L"GXR", L"GXS", L"GXT", L"GXU", L"GXV", L"GXW", L"GXX", L"GXY", L"GXZ", L"GYA", L"GYB", L"GYC", L"GYD", L"GYE", L"GYF", L"GYG", L"GYH", L"GYI", L"GYJ", L"GYK", L"GYL", L"GYM", L"GYN", L"GYO", L"GYP", L"GYQ", L"GYR", L"GYS", L"GYT", L"GYU", L"GYV", L"GYW", L"GYX", L"GYY", L"GYZ", L"GZA", L"GZB", L"GZC", L"GZD", L"GZE", L"GZF", L"GZG", L"GZH", L"GZI", L"GZJ", L"GZK", L"GZL", L"GZM", L"GZN", L"GZO", L"GZP", L"GZQ", L"GZR", L"GZS", L"GZT", L"GZU", L"GZV", L"GZW", L"GZX", L"GZY", L"GZZ", L"HAA", L"HAB", L"HAC", L"HAD", L"HAE", L"HAF", L"HAG", L"HAH", L"HAI", L"HAJ", L"HAK", L"HAL", L"HAM", L"HAN", L"HAO", L"HAP", L"HAQ", L"HAR", L"HAS", L"HAT", L"HAU", L"HAV", L"HAW", L"HAX", L"HAY", L"HAZ", L"HBA", L"HBB", L"HBC", L"HBD", L"HBE", L"HBF", L"HBG", L"HBH", L"HBI", L"HBJ", L"HBK", L"HBL", L"HBM", L"HBN", L"HBO", L"HBP", L"HBQ", L"HBR", L"HBS", L"HBT", L"HBU", L"HBV", L"HBW", L"HBX", L"HBY", L"HBZ", L"HCA", L"HCB", L"HCC", L"HCD", L"HCE", L"HCF", L"HCG", L"HCH", L"HCI", L"HCJ", L"HCK", L"HCL", L"HCM", L"HCN", L"HCO", L"HCP", L"HCQ", L"HCR", L"HCS", L"HCT", L"HCU", L"HCV", L"HCW", L"HCX", L"HCY", L"HCZ", L"HDA", L"HDB", L"HDC", L"HDD", L"HDE", L"HDF", L"HDG", L"HDH", L"HDI", L"HDJ", L"HDK", L"HDL", L"HDM", L"HDN", L"HDO", L"HDP", L"HDQ", L"HDR", L"HDS", 
			L"HDT", L"HDU", L"HDV", L"HDW", L"HDX", L"HDY", L"HDZ", L"HEA", L"HEB", L"HEC", L"HED", L"HEE", L"HEF", L"HEG", L"HEH", L"HEI", L"HEJ", L"HEK", L"HEL", L"HEM", L"HEN", L"HEO", L"HEP", L"HEQ", L"HER", L"HES", L"HET", L"HEU", L"HEV", L"HEW", L"HEX", L"HEY", L"HEZ", L"HFA", L"HFB", L"HFC", L"HFD", L"HFE", L"HFF", L"HFG", L"HFH", L"HFI", L"HFJ", L"HFK", L"HFL", L"HFM", L"HFN", L"HFO", L"HFP", L"HFQ", L"HFR", L"HFS", L"HFT", L"HFU", L"HFV", L"HFW", L"HFX", L"HFY", L"HFZ", L"HGA", L"HGB", L"HGC", L"HGD", L"HGE", L"HGF", L"HGG", L"HGH", L"HGI", L"HGJ", L"HGK", L"HGL", L"HGM", L"HGN", L"HGO", L"HGP", L"HGQ", L"HGR", L"HGS", L"HGT", L"HGU", L"HGV", L"HGW", L"HGX", L"HGY", L"HGZ", L"HHA", L"HHB", L"HHC", L"HHD", L"HHE", L"HHF", L"HHG", L"HHH", L"HHI", L"HHJ", L"HHK", L"HHL", L"HHM", L"HHN", L"HHO", L"HHP", L"HHQ", L"HHR", L"HHS", L"HHT", L"HHU", L"HHV", L"HHW", L"HHX", L"HHY", L"HHZ", L"HIA", L"HIB", L"HIC", L"HID", L"HIE", L"HIF", L"HIG", L"HIH", L"HII", L"HIJ", L"HIK", L"HIL", L"HIM", L"HIN", L"HIO", L"HIP", L"HIQ", L"HIR", L"HIS", L"HIT", L"HIU", L"HIV", L"HIW", L"HIX", L"HIY", L"HIZ", L"HJA", L"HJB", L"HJC", L"HJD", L"HJE", L"HJF", L"HJG", L"HJH", L"HJI", L"HJJ", L"HJK", L"HJL", L"HJM", L"HJN", L"HJO", L"HJP", L"HJQ", L"HJR", L"HJS", L"HJT", L"HJU", L"HJV", L"HJW", L"HJX", L"HJY", L"HJZ", L"HKA", L"HKB", L"HKC", L"HKD", L"HKE", L"HKF", L"HKG", L"HKH", L"HKI", L"HKJ", L"HKK", L"HKL", L"HKM", L"HKN", L"HKO", L"HKP", L"HKQ", L"HKR", L"HKS", L"HKT", L"HKU", L"HKV", L"HKW", L"HKX", L"HKY", L"HKZ", L"HLA", L"HLB", L"HLC", L"HLD", L"HLE", L"HLF", L"HLG", L"HLH", L"HLI", L"HLJ", L"HLK", L"HLL", L"HLM", L"HLN", L"HLO", L"HLP", L"HLQ", L"HLR", L"HLS", L"HLT", L"HLU", L"HLV", L"HLW", L"HLX", L"HLY", L"HLZ", L"HMA", L"HMB", L"HMC", L"HMD", L"HME", L"HMF", L"HMG", L"HMH", L"HMI", L"HMJ", L"HMK", L"HML", L"HMM", L"HMN", L"HMO", L"HMP", L"HMQ", L"HMR", L"HMS", L"HMT", L"HMU", L"HMV", L"HMW", L"HMX", L"HMY", L"HMZ", L"HNA", L"HNB", L"HNC", L"HND", L"HNE", L"HNF", L"HNG", L"HNH", L"HNI", L"HNJ", L"HNK", L"HNL", L"HNM", L"HNN", L"HNO", L"HNP", L"HNQ", L"HNR", L"HNS", L"HNT", L"HNU", L"HNV", L"HNW", L"HNX", L"HNY", L"HNZ", L"HOA", L"HOB", L"HOC", L"HOD", L"HOE", L"HOF", 
			L"HOG", L"HOH", L"HOI", L"HOJ", L"HOK", L"HOL", L"HOM", L"HON", L"HOO", L"HOP", L"HOQ", L"HOR", L"HOS", L"HOT", L"HOU", L"HOV", L"HOW", L"HOX", L"HOY", L"HOZ", L"HPA", L"HPB", L"HPC", L"HPD", L"HPE", L"HPF", L"HPG", L"HPH", L"HPI", L"HPJ", L"HPK", L"HPL", L"HPM", L"HPN", L"HPO", L"HPP", L"HPQ", L"HPR", L"HPS", L"HPT", L"HPU", L"HPV", L"HPW", L"HPX", L"HPY", L"HPZ", L"HQA", L"HQB", L"HQC", L"HQD", L"HQE", L"HQF", L"HQG", L"HQH", L"HQI", L"HQJ", L"HQK", L"HQL", L"HQM", L"HQN", L"HQO", L"HQP", L"HQQ", L"HQR", L"HQS", L"HQT", L"HQU", L"HQV", L"HQW", L"HQX", L"HQY", L"HQZ", L"HRA", L"HRB", L"HRC", L"HRD", L"HRE", L"HRF", L"HRG", L"HRH", L"HRI", L"HRJ", L"HRK", L"HRL", L"HRM", L"HRN", L"HRO", L"HRP", L"HRQ", L"HRR", L"HRS", L"HRT", L"HRU", L"HRV", L"HRW", L"HRX", L"HRY", L"HRZ", L"HSA", L"HSB", L"HSC", L"HSD", L"HSE", L"HSF", L"HSG", L"HSH", L"HSI", L"HSJ", L"HSK", L"HSL", L"HSM", L"HSN", L"HSO", L"HSP", L"HSQ", L"HSR", L"HSS", L"HST", L"HSU", L"HSV", L"HSW", L"HSX", L"HSY", L"HSZ", L"HTA", L"HTB", L"HTC", L"HTD", L"HTE", L"HTF", L"HTG", L"HTH", L"HTI", L"HTJ", L"HTK", L"HTL", L"HTM", L"HTN", L"HTO", L"HTP", L"HTQ", L"HTR", L"HTS", L"HTT", L"HTU", L"HTV", L"HTW", L"HTX", L"HTY", L"HTZ", L"HUA", L"HUB", L"HUC", L"HUD", L"HUE", L"HUF", L"HUG", L"HUH", L"HUI", L"HUJ", L"HUK", L"HUL", L"HUM", L"HUN", L"HUO", L"HUP", L"HUQ", L"HUR", L"HUS", L"HUT", L"HUU", L"HUV", L"HUW", L"HUX", L"HUY", L"HUZ", L"HVA", L"HVB", L"HVC", L"HVD", L"HVE", L"HVF", L"HVG", L"HVH", L"HVI", L"HVJ", L"HVK", L"HVL", L"HVM", L"HVN", L"HVO", L"HVP", L"HVQ", L"HVR", L"HVS", L"HVT", L"HVU", L"HVV", L"HVW", L"HVX", L"HVY", L"HVZ", L"HWA", L"HWB", L"HWC", L"HWD", L"HWE", L"HWF", L"HWG", L"HWH", L"HWI", L"HWJ", L"HWK", L"HWL", L"HWM", L"HWN", L"HWO", L"HWP", L"HWQ", L"HWR", L"HWS", L"HWT", L"HWU", L"HWV", L"HWW", L"HWX", L"HWY", L"HWZ", L"HXA", L"HXB", L"HXC", L"HXD", L"HXE", L"HXF", L"HXG", L"HXH", L"HXI", L"HXJ", L"HXK", L"HXL", L"HXM", L"HXN", L"HXO", L"HXP", L"HXQ", L"HXR", L"HXS", L"HXT", L"HXU", L"HXV", L"HXW", L"HXX", L"HXY", L"HXZ", L"HYA", L"HYB", L"HYC", L"HYD", L"HYE", L"HYF", L"HYG", L"HYH", L"HYI", L"HYJ", L"HYK",
			L"HYL", L"HYM", L"HYN", L"HYO", L"HYP", L"HYQ", L"HYR", L"HYS", L"HYT", L"HYU", L"HYV", L"HYW", L"HYX", L"HYY", L"HYZ", L"HZA", L"HZB", L"HZC", L"HZD", L"HZE", L"HZF", L"HZG", L"HZH", L"HZI", L"HZJ", L"HZK", L"HZL", L"HZM", L"HZN", L"HZO", L"HZP", L"HZQ", L"HZR", L"HZS", L"HZT", L"HZU", L"HZV", L"HZW", L"HZX", L"HZY", L"HZZ", L"IAA", L"IAB", L"IAC", L"IAD", L"IAE", L"IAF", L"IAG", L"IAH", L"IAI", L"IAJ", L"IAK", L"IAL", L"IAM", L"IAN", L"IAO", L"IAP", L"IAQ", L"IAR", L"IAS", L"IAT", L"IAU", L"IAV", L"IAW", L"IAX", L"IAY", L"IAZ", L"IBA", L"IBB", L"IBC", L"IBD", L"IBE", L"IBF", L"IBG", L"IBH", L"IBI", L"IBJ", L"IBK", L"IBL", L"IBM", L"IBN", L"IBO", L"IBP", L"IBQ", L"IBR", L"IBS", L"IBT", L"IBU", L"IBV", L"IBW", L"IBX", L"IBY", L"IBZ", L"ICA", L"ICB", L"ICC", L"ICD", L"ICE", L"ICF", L"ICG", L"ICH", L"ICI", L"ICJ", L"ICK", L"ICL", L"ICM", L"ICN", L"ICO", L"ICP", L"ICQ", L"ICR", L"ICS", L"ICT", L"ICU", L"ICV", L"ICW", L"ICX", L"ICY", L"ICZ", L"IDA", L"IDB", L"IDC", L"IDD", L"IDE", L"IDF", L"IDG", L"IDH", L"IDI", L"IDJ", L"IDK", L"IDL", L"IDM", L"IDN", L"IDO", L"IDP", L"IDQ", L"IDR", L"IDS", L"IDT", L"IDU", L"IDV", L"IDW", L"IDX", L"IDY", L"IDZ", L"IEA", L"IEB", L"IEC", L"IED", L"IEE", L"IEF", L"IEG", L"IEH", L"IEI", L"IEJ", L"IEK", L"IEL", L"IEM", L"IEN", L"IEO", L"IEP", L"IEQ", L"IER", L"IES", L"IET", L"IEU", L"IEV", L"IEW", L"IEX", L"IEY", L"IEZ", L"IFA", L"IFB", L"IFC", L"IFD", L"IFE", L"IFF", L"IFG", L"IFH", L"IFI", L"IFJ", L"IFK", L"IFL", L"IFM", L"IFN", L"IFO", L"IFP", L"IFQ", L"IFR", L"IFS", L"IFT", L"IFU", L"IFV", L"IFW", L"IFX", L"IFY", L"IFZ", L"IGA", L"IGB", L"IGC", L"IGD", L"IGE", L"IGF", L"IGG", L"IGH", L"IGI", L"IGJ", L"IGK", L"IGL", L"IGM", L"IGN", L"IGO", L"IGP", L"IGQ", L"IGR", L"IGS", L"IGT", L"IGU", L"IGV", L"IGW", L"IGX", L"IGY", L"IGZ", L"IHA", L"IHB", L"IHC", L"IHD", L"IHE", L"IHF", L"IHG", L"IHH", L"IHI", L"IHJ", L"IHK", L"IHL", L"IHM", L"IHN", L"IHO", L"IHP", L"IHQ", L"IHR", L"IHS", L"IHT", L"IHU", L"IHV", L"IHW", L"IHX", L"IHY", L"IHZ", L"IIA", L"IIB", L"IIC", L"IID", L"IIE", L"IIF", L"IIG", L"IIH", L"III", L"IIJ", L"IIK", L"IIL", L"IIM", L"IIN", L"IIO", L"IIP", L"IIQ", L"IIR", L"IIS", L"IIT", L"IIU", L"IIV", L"IIW", L"IIX",
			L"IIY", L"IIZ", L"IJA", L"IJB", L"IJC", L"IJD", L"IJE", L"IJF", L"IJG", L"IJH", L"IJI", L"IJJ", L"IJK", L"IJL", L"IJM", L"IJN", L"IJO", L"IJP", L"IJQ", L"IJR", L"IJS", L"IJT", L"IJU", L"IJV", L"IJW", L"IJX", L"IJY", L"IJZ", L"IKA", L"IKB", L"IKC", L"IKD", L"IKE", L"IKF", L"IKG", L"IKH", L"IKI", L"IKJ", L"IKK", L"IKL", L"IKM", L"IKN", L"IKO", L"IKP", L"IKQ", L"IKR", L"IKS", L"IKT", L"IKU", L"IKV", L"IKW", L"IKX", L"IKY", L"IKZ", L"ILA", L"ILB", L"ILC", L"ILD", L"ILE", L"ILF", L"ILG", L"ILH", L"ILI", L"ILJ", L"ILK", L"ILL", L"ILM", L"ILN", L"ILO", L"ILP", L"ILQ", L"ILR", L"ILS", L"ILT", L"ILU", L"ILV", L"ILW", L"ILX", L"ILY", L"ILZ", L"IMA", L"IMB", L"IMC", L"IMD", L"IME", L"IMF", L"IMG", L"IMH", L"IMI", L"IMJ", L"IMK", L"IML", L"IMM", L"IMN", L"IMO", L"IMP", L"IMQ", L"IMR", L"IMS", L"IMT", L"IMU", L"IMV", L"IMW", L"IMX", L"IMY", L"IMZ", L"INA", L"INB", L"INC", L"IND", L"INE", L"INF", L"ING", L"INH", L"INI", L"INJ", L"INK", L"INL", L"INM", L"INN", L"INO", L"INP", L"INQ", L"INR", L"INS", L"INT", L"INU", L"INV", L"INW", L"INX", L"INY", L"INZ", L"IOA", L"IOB", L"IOC", L"IOD", L"IOE", L"IOF", L"IOG", L"IOH", L"IOI", L"IOJ", L"IOK", L"IOL", L"IOM", L"ION", L"IOO", L"IOP", L"IOQ", L"IOR", L"IOS", L"IOT", L"IOU", L"IOV", L"IOW", L"IOX", L"IOY", L"IOZ", L"IPA", L"IPB", L"IPC", L"IPD", L"IPE", L"IPF", L"IPG", L"IPH", L"IPI", L"IPJ", L"IPK", L"IPL", L"IPM", L"IPN", L"IPO", L"IPP", L"IPQ", L"IPR", L"IPS", L"IPT", L"IPU", L"IPV", L"IPW", L"IPX", L"IPY", L"IPZ", L"IQA", L"IQB", L"IQC", L"IQD", L"IQE", L"IQF", L"IQG", L"IQH", L"IQI", L"IQJ", L"IQK", L"IQL", L"IQM", L"IQN", L"IQO", L"IQP", L"IQQ", L"IQR", L"IQS", L"IQT", L"IQU", L"IQV", L"IQW", L"IQX", L"IQY", L"IQZ", L"IRA", L"IRB", L"IRC", L"IRD", L"IRE", L"IRF", L"IRG", L"IRH", L"IRI", L"IRJ", L"IRK", L"IRL", L"IRM", L"IRN", L"IRO", L"IRP", L"IRQ", L"IRR", L"IRS", L"IRT", L"IRU", L"IRV", L"IRW", L"IRX", L"IRY", L"IRZ", L"ISA", L"ISB", L"ISC", L"ISD", L"ISE", L"ISF", L"ISG", L"ISH", L"ISI", L"ISJ", L"ISK", L"ISL", L"ISM", L"ISN", L"ISO", L"ISP", L"ISQ", L"ISR", L"ISS", L"IST", L"ISU", L"ISV", L"ISW", L"ISX", L"ISY", L"ISZ", L"ITA", L"ITB", L"ITC", L"ITD", L"ITE", L"ITF", L"ITG", L"ITH", L"ITI", L"ITJ", L"ITK",
			L"ITL", L"ITM", L"ITN", L"ITO", L"ITP", L"ITQ", L"ITR", L"ITS", L"ITT", L"ITU", L"ITV", L"ITW", L"ITX", L"ITY", L"ITZ", L"IUA", L"IUB", L"IUC", L"IUD", L"IUE", L"IUF", L"IUG", L"IUH", L"IUI", L"IUJ", L"IUK", L"IUL", L"IUM", L"IUN", L"IUO", L"IUP", L"IUQ", L"IUR", L"IUS", L"IUT", L"IUU", L"IUV", L"IUW", L"IUX", L"IUY", L"IUZ", L"IVA", L"IVB", L"IVC", L"IVD", L"IVE", L"IVF", L"IVG", L"IVH", L"IVI", L"IVJ", L"IVK", L"IVL", L"IVM", L"IVN", L"IVO", L"IVP", L"IVQ", L"IVR", L"IVS", L"IVT", L"IVU", L"IVV", L"IVW", L"IVX", L"IVY", L"IVZ", L"IWA", L"IWB", L"IWC", L"IWD", L"IWE", L"IWF", L"IWG", L"IWH", L"IWI", L"IWJ", L"IWK", L"IWL", L"IWM", L"IWN", L"IWO", L"IWP", L"IWQ", L"IWR", L"IWS", L"IWT", L"IWU", L"IWV", L"IWW", L"IWX", L"IWY", L"IWZ", L"IXA", L"IXB", L"IXC", L"IXD", L"IXE", L"IXF", L"IXG", L"IXH", L"IXI", L"IXJ", L"IXK", L"IXL", L"IXM", L"IXN", L"IXO", L"IXP", L"IXQ", L"IXR", L"IXS", L"IXT", L"IXU", L"IXV", L"IXW", L"IXX", L"IXY", L"IXZ", L"IYA", L"IYB", L"IYC", L"IYD", L"IYE", L"IYF", L"IYG", L"IYH", L"IYI", L"IYJ", L"IYK", L"IYL", L"IYM", L"IYN", L"IYO", L"IYP", L"IYQ", L"IYR", L"IYS", L"IYT", L"IYU", L"IYV", L"IYW", L"IYX", L"IYY", L"IYZ", L"IZA", L"IZB", L"IZC", L"IZD", L"IZE", L"IZF", L"IZG", L"IZH", L"IZI", L"IZJ", L"IZK", L"IZL", L"IZM", L"IZN", L"IZO", L"IZP", L"IZQ", L"IZR", L"IZS", L"IZT", L"IZU", L"IZV", L"IZW", L"IZX", L"IZY", L"IZZ", L"JAA", L"JAB", L"JAC", L"JAD", L"JAE", L"JAF", L"JAG", L"JAH", L"JAI", L"JAJ", L"JAK", L"JAL", L"JAM", L"JAN", L"JAO", L"JAP", L"JAQ", L"JAR", L"JAS", L"JAT", L"JAU", L"JAV", L"JAW", L"JAX", L"JAY", L"JAZ", L"JBA", L"JBB", L"JBC", L"JBD", L"JBE", L"JBF", L"JBG", L"JBH", L"JBI", L"JBJ", L"JBK", L"JBL", L"JBM", L"JBN", L"JBO", L"JBP", L"JBQ", L"JBR", L"JBS", L"JBT", L"JBU", L"JBV", L"JBW", L"JBX", L"JBY", L"JBZ", L"JCA", L"JCB", L"JCC", L"JCD", L"JCE", L"JCF", L"JCG", L"JCH", L"JCI", L"JCJ", L"JCK", L"JCL", L"JCM", L"JCN", L"JCO", L"JCP", L"JCQ", L"JCR", L"JCS", L"JCT", L"JCU", L"JCV", L"JCW", L"JCX", L"JCY", L"JCZ", L"JDA", L"JDB", L"JDC", L"JDD", L"JDE", L"JDF", L"JDG", L"JDH", L"JDI", L"JDJ", L"JDK", L"JDL", L"JDM", L"JDN", L"JDO", L"JDP", L"JDQ", L"JDR", L"JDS", L"JDT", L"JDU", L"JDV", L"JDW", L"JDX",
			L"JDY", L"JDZ", L"JEA", L"JEB", L"JEC", L"JED", L"JEE", L"JEF", L"JEG", L"JEH", L"JEI", L"JEJ", L"JEK", L"JEL", L"JEM", L"JEN", L"JEO", L"JEP", L"JEQ", L"JER", L"JES", L"JET", L"JEU", L"JEV", L"JEW", L"JEX", L"JEY", L"JEZ", L"JFA", L"JFB", L"JFC", L"JFD", L"JFE", L"JFF", L"JFG", L"JFH", L"JFI", L"JFJ", L"JFK", L"JFL", L"JFM", L"JFN", L"JFO", L"JFP", L"JFQ", L"JFR", L"JFS", L"JFT", L"JFU", L"JFV", L"JFW", L"JFX", L"JFY", L"JFZ", L"JGA", L"JGB", L"JGC", L"JGD", L"JGE", L"JGF", L"JGG", L"JGH", L"JGI", L"JGJ", L"JGK", L"JGL", L"JGM", L"JGN", L"JGO", L"JGP", L"JGQ", L"JGR", L"JGS", L"JGT", L"JGU", L"JGV", L"JGW", L"JGX", L"JGY", L"JGZ", L"JHA", L"JHB", L"JHC", L"JHD", L"JHE", L"JHF", L"JHG", L"JHH", L"JHI", L"JHJ", L"JHK", L"JHL", L"JHM", L"JHN", L"JHO", L"JHP", L"JHQ", L"JHR", L"JHS", L"JHT", L"JHU", L"JHV", L"JHW", L"JHX", L"JHY", L"JHZ", L"JIA", L"JIB", L"JIC", L"JID", L"JIE", L"JIF", L"JIG", L"JIH", L"JII", L"JIJ", L"JIK", L"JIL", L"JIM", L"JIN", L"JIO", L"JIP", L"JIQ", L"JIR", L"JIS", L"JIT", L"JIU", L"JIV", L"JIW", L"JIX", L"JIY", L"JIZ", L"JJA", L"JJB", L"JJC", L"JJD", L"JJE", L"JJF", L"JJG", L"JJH", L"JJI", L"JJJ", L"JJK", L"JJL", L"JJM", L"JJN", L"JJO", L"JJP", L"JJQ", L"JJR", L"JJS", L"JJT", L"JJU", L"JJV", L"JJW", L"JJX", L"JJY", L"JJZ", L"JKA", L"JKB", L"JKC", L"JKD", L"JKE", L"JKF", L"JKG", L"JKH", L"JKI", L"JKJ", L"JKK", L"JKL", L"JKM", L"JKN", L"JKO", L"JKP", L"JKQ", L"JKR", L"JKS", L"JKT", L"JKU", L"JKV", L"JKW", L"JKX", L"JKY", L"JKZ", L"JLA", L"JLB", L"JLC", L"JLD", L"JLE", L"JLF", L"JLG", L"JLH", L"JLI", L"JLJ", L"JLK", L"JLL", L"JLM", L"JLN", L"JLO", L"JLP", L"JLQ", L"JLR", L"JLS", L"JLT", L"JLU", L"JLV", L"JLW", L"JLX", L"JLY", L"JLZ", L"JMA", L"JMB", L"JMC", L"JMD", L"JME", L"JMF", L"JMG", L"JMH", L"JMI", L"JMJ", L"JMK", L"JML", L"JMM", L"JMN", L"JMO", L"JMP", L"JMQ", L"JMR", L"JMS", L"JMT", L"JMU", L"JMV", L"JMW", L"JMX", L"JMY", L"JMZ", L"JNA", L"JNB", L"JNC", L"JND", L"JNE", L"JNF", L"JNG", L"JNH", L"JNI", L"JNJ", L"JNK", L"JNL", L"JNM", L"JNN", L"JNO", L"JNP", L"JNQ", L"JNR", L"JNS", L"JNT", L"JNU", L"JNV", L"JNW", L"JNX", L"JNY", L"JNZ", L"JOA", L"JOB", L"JOC", L"JOD", L"JOE", L"JOF", L"JOG", L"JOH", L"JOI", L"JOJ", L"JOK",
			L"JOL", L"JOM", L"JON", L"JOO", L"JOP", L"JOQ", L"JOR", L"JOS", L"JOT", L"JOU", L"JOV", L"JOW", L"JOX", L"JOY", L"JOZ", L"JPA", L"JPB", L"JPC", L"JPD", L"JPE", L"JPF", L"JPG", L"JPH", L"JPI", L"JPJ", L"JPK", L"JPL", L"JPM", L"JPN", L"JPO", L"JPP", L"JPQ", L"JPR", L"JPS", L"JPT", L"JPU", L"JPV", L"JPW", L"JPX", L"JPY", L"JPZ", L"JQA", L"JQB", L"JQC", L"JQD", L"JQE", L"JQF", L"JQG", L"JQH", L"JQI", L"JQJ", L"JQK", L"JQL", L"JQM", L"JQN", L"JQO", L"JQP", L"JQQ", L"JQR", L"JQS", L"JQT", L"JQU", L"JQV", L"JQW", L"JQX", L"JQY", L"JQZ", L"JRA", L"JRB", L"JRC", L"JRD", L"JRE", L"JRF", L"JRG", L"JRH", L"JRI", L"JRJ", L"JRK", L"JRL", L"JRM", L"JRN", L"JRO", L"JRP", L"JRQ", L"JRR", L"JRS", L"JRT", L"JRU", L"JRV", L"JRW", L"JRX", L"JRY", L"JRZ", L"JSA", L"JSB", L"JSC", L"JSD", L"JSE", L"JSF", L"JSG", L"JSH", L"JSI", L"JSJ", L"JSK", L"JSL", L"JSM", L"JSN", L"JSO", L"JSP", L"JSQ", L"JSR", L"JSS", L"JST", L"JSU", L"JSV", L"JSW", L"JSX", L"JSY", L"JSZ", L"JTA", L"JTB", L"JTC", L"JTD", L"JTE", L"JTF", L"JTG", L"JTH", L"JTI", L"JTJ", L"JTK", L"JTL", L"JTM", L"JTN", L"JTO", L"JTP", L"JTQ", L"JTR", L"JTS", L"JTT", L"JTU", L"JTV", L"JTW", L"JTX", L"JTY", L"JTZ", L"JUA", L"JUB", L"JUC", L"JUD", L"JUE", L"JUF", L"JUG", L"JUH", L"JUI", L"JUJ", L"JUK", L"JUL", L"JUM", L"JUN", L"JUO", L"JUP", L"JUQ", L"JUR", L"JUS", L"JUT", L"JUU", L"JUV", L"JUW", L"JUX", L"JUY", L"JUZ", L"JVA", L"JVB", L"JVC", L"JVD", L"JVE", L"JVF", L"JVG", L"JVH", L"JVI", L"JVJ", L"JVK", L"JVL", L"JVM", L"JVN", L"JVO", L"JVP", L"JVQ", L"JVR", L"JVS", L"JVT", L"JVU", L"JVV", L"JVW", L"JVX", L"JVY", L"JVZ", L"JWA", L"JWB", L"JWC", L"JWD", L"JWE", L"JWF", L"JWG", L"JWH", L"JWI", L"JWJ", L"JWK", L"JWL", L"JWM", L"JWN", L"JWO", L"JWP", L"JWQ", L"JWR", L"JWS", L"JWT", L"JWU", L"JWV", L"JWW", L"JWX", L"JWY", L"JWZ", L"JXA", L"JXB", L"JXC", L"JXD", L"JXE", L"JXF", L"JXG", L"JXH", L"JXI", L"JXJ", L"JXK", L"JXL", L"JXM", L"JXN", L"JXO", L"JXP", L"JXQ", L"JXR", L"JXS", L"JXT", L"JXU", L"JXV", L"JXW", L"JXX", L"JXY", L"JXZ", L"JYA", L"JYB", L"JYC", L"JYD", L"JYE", L"JYF", L"JYG", L"JYH", L"JYI", L"JYJ", L"JYK", L"JYL", L"JYM", L"JYN", L"JYO", L"JYP", L"JYQ", L"JYR", L"JYS", L"JYT", L"JYU", L"JYV", L"JYW", L"JYX",
			L"JYY", L"JYZ", L"JZA", L"JZB", L"JZC", L"JZD", L"JZE", L"JZF", L"JZG", L"JZH", L"JZI", L"JZJ", L"JZK", L"JZL", L"JZM", L"JZN", L"JZO", L"JZP", L"JZQ", L"JZR", L"JZS", L"JZT", L"JZU", L"JZV", L"JZW", L"JZX", L"JZY", L"JZZ", L"KAA", L"KAB", L"KAC", L"KAD", L"KAE", L"KAF", L"KAG", L"KAH", L"KAI", L"KAJ", L"KAK", L"KAL", L"KAM", L"KAN", L"KAO", L"KAP", L"KAQ", L"KAR", L"KAS", L"KAT", L"KAU", L"KAV", L"KAW", L"KAX", L"KAY", L"KAZ", L"KBA", L"KBB", L"KBC", L"KBD", L"KBE", L"KBF", L"KBG", L"KBH", L"KBI", L"KBJ", L"KBK", L"KBL", L"KBM", L"KBN", L"KBO", L"KBP", L"KBQ", L"KBR", L"KBS", L"KBT", L"KBU", L"KBV", L"KBW", L"KBX", L"KBY", L"KBZ", L"KCA", L"KCB", L"KCC", L"KCD", L"KCE", L"KCF", L"KCG", L"KCH", L"KCI", L"KCJ", L"KCK", L"KCL", L"KCM", L"KCN", L"KCO", L"KCP", L"KCQ", L"KCR", L"KCS", L"KCT", L"KCU", L"KCV", L"KCW", L"KCX", L"KCY", L"KCZ", L"KDA", L"KDB", L"KDC", L"KDD", L"KDE", L"KDF", L"KDG", L"KDH", L"KDI", L"KDJ", L"KDK", L"KDL", L"KDM", L"KDN", L"KDO", L"KDP", L"KDQ", L"KDR", L"KDS", L"KDT", L"KDU", L"KDV", L"KDW", L"KDX", L"KDY", L"KDZ", L"KEA", L"KEB", L"KEC", L"KED", L"KEE", L"KEF", L"KEG", L"KEH", L"KEI", L"KEJ", L"KEK", L"KEL", L"KEM", L"KEN", L"KEO", L"KEP", L"KEQ", L"KER", L"KES", L"KET", L"KEU", L"KEV", L"KEW", L"KEX", L"KEY", L"KEZ", L"KFA", L"KFB", L"KFC", L"KFD", L"KFE", L"KFF", L"KFG", L"KFH", L"KFI", L"KFJ", L"KFK", L"KFL", L"KFM", L"KFN", L"KFO", L"KFP", L"KFQ", L"KFR", L"KFS", L"KFT", L"KFU", L"KFV", L"KFW", L"KFX", L"KFY", L"KFZ", L"KGA", L"KGB", L"KGC", L"KGD", L"KGE", L"KGF", L"KGG", L"KGH", L"KGI", L"KGJ", L"KGK", L"KGL", L"KGM", L"KGN", L"KGO", L"KGP", L"KGQ", L"KGR", L"KGS", L"KGT", L"KGU", L"KGV", L"KGW", L"KGX", L"KGY", L"KGZ", L"KHA", L"KHB", L"KHC", L"KHD", L"KHE", L"KHF", L"KHG", L"KHH", L"KHI", L"KHJ", L"KHK", L"KHL", L"KHM", L"KHN", L"KHO", L"KHP", L"KHQ", L"KHR", L"KHS", L"KHT", L"KHU", L"KHV", L"KHW", L"KHX", L"KHY", L"KHZ", L"KIA", L"KIB", L"KIC", L"KID", L"KIE", L"KIF", L"KIG", L"KIH", L"KII", L"KIJ", L"KIK", L"KIL", L"KIM", L"KIN", L"KIO", L"KIP", L"KIQ", L"KIR", L"KIS", L"KIT", L"KIU", L"KIV", L"KIW", L"KIX", L"KIY", L"KIZ", L"KJA", L"KJB", L"KJC", L"KJD", L"KJE", L"KJF", L"KJG", L"KJH", L"KJI", L"KJJ", L"KJK",
			L"KJL", L"KJM", L"KJN", L"KJO", L"KJP", L"KJQ", L"KJR", L"KJS", L"KJT", L"KJU", L"KJV", L"KJW", L"KJX", L"KJY", L"KJZ", L"KKA", L"KKB", L"KKC", L"KKD", L"KKE", L"KKF", L"KKG", L"KKH", L"KKI", L"KKJ", L"KKK", L"KKL", L"KKM", L"KKN", L"KKO", L"KKP", L"KKQ", L"KKR", L"KKS", L"KKT", L"KKU", L"KKV", L"KKW", L"KKX", L"KKY", L"KKZ", L"KLA", L"KLB", L"KLC", L"KLD", L"KLE", L"KLF", L"KLG", L"KLH", L"KLI", L"KLJ", L"KLK", L"KLL", L"KLM", L"KLN", L"KLO", L"KLP", L"KLQ", L"KLR", L"KLS", L"KLT", L"KLU", L"KLV", L"KLW", L"KLX", L"KLY", L"KLZ", L"KMA", L"KMB", L"KMC", L"KMD", L"KME", L"KMF", L"KMG", L"KMH", L"KMI", L"KMJ", L"KMK", L"KML", L"KMM", L"KMN", L"KMO", L"KMP", L"KMQ", L"KMR", L"KMS", L"KMT", L"KMU", L"KMV", L"KMW", L"KMX", L"KMY", L"KMZ", L"KNA", L"KNB", L"KNC", L"KND", L"KNE", L"KNF", L"KNG", L"KNH", L"KNI", L"KNJ", L"KNK", L"KNL", L"KNM", L"KNN", L"KNO", L"KNP", L"KNQ", L"KNR", L"KNS", L"KNT", L"KNU", L"KNV", L"KNW", L"KNX", L"KNY", L"KNZ", L"KOA", L"KOB", L"KOC", L"KOD", L"KOE", L"KOF", L"KOG", L"KOH", L"KOI", L"KOJ", L"KOK", L"KOL", L"KOM", L"KON", L"KOO", L"KOP", L"KOQ", L"KOR", L"KOS", L"KOT", L"KOU", L"KOV", L"KOW", L"KOX", L"KOY", L"KOZ", L"KPA", L"KPB", L"KPC", L"KPD", L"KPE", L"KPF", L"KPG", L"KPH", L"KPI", L"KPJ", L"KPK", L"KPL", L"KPM", L"KPN", L"KPO", L"KPP", L"KPQ", L"KPR", L"KPS", L"KPT", L"KPU", L"KPV", L"KPW", L"KPX", L"KPY", L"KPZ", L"KQA", L"KQB", L"KQC", L"KQD", L"KQE", L"KQF", L"KQG", L"KQH", L"KQI", L"KQJ", L"KQK", L"KQL", L"KQM", L"KQN", L"KQO", L"KQP", L"KQQ", L"KQR", L"KQS", L"KQT", L"KQU", L"KQV", L"KQW", L"KQX", L"KQY", L"KQZ", L"KRA", L"KRB", L"KRC", L"KRD", L"KRE", L"KRF", L"KRG", L"KRH", L"KRI", L"KRJ", L"KRK", L"KRL", L"KRM", L"KRN", L"KRO", L"KRP", L"KRQ", L"KRR", L"KRS", L"KRT", L"KRU", L"KRV", L"KRW", L"KRX", L"KRY", L"KRZ", L"KSA", L"KSB", L"KSC", L"KSD", L"KSE", L"KSF", L"KSG", L"KSH", L"KSI", L"KSJ", L"KSK", L"KSL", L"KSM", L"KSN", L"KSO", L"KSP", L"KSQ", L"KSR", L"KSS", L"KST", L"KSU", L"KSV", L"KSW", L"KSX", L"KSY", L"KSZ", L"KTA", L"KTB", L"KTC", L"KTD", L"KTE", L"KTF", L"KTG", L"KTH", L"KTI", L"KTJ", L"KTK", L"KTL", L"KTM", L"KTN", L"KTO", L"KTP", L"KTQ", L"KTR", L"KTS", L"KTT", L"KTU", L"KTV", L"KTW", L"KTX", 
			L"KTY", L"KTZ", L"KUA", L"KUB", L"KUC", L"KUD", L"KUE", L"KUF", L"KUG", L"KUH", L"KUI", L"KUJ", L"KUK", L"KUL", L"KUM", L"KUN", L"KUO", L"KUP", L"KUQ", L"KUR", L"KUS", L"KUT", L"KUU", L"KUV", L"KUW", L"KUX", L"KUY", L"KUZ", L"KVA", L"KVB", L"KVC", L"KVD", L"KVE", L"KVF", L"KVG", L"KVH", L"KVI", L"KVJ", L"KVK", L"KVL", L"KVM", L"KVN", L"KVO", L"KVP", L"KVQ", L"KVR", L"KVS", L"KVT", L"KVU", L"KVV", L"KVW", L"KVX", L"KVY", L"KVZ", L"KWA", L"KWB", L"KWC", L"KWD", L"KWE", L"KWF", L"KWG", L"KWH", L"KWI", L"KWJ", L"KWK", L"KWL", L"KWM", L"KWN", L"KWO", L"KWP", L"KWQ", L"KWR", L"KWS", L"KWT", L"KWU", L"KWV", L"KWW", L"KWX", L"KWY", L"KWZ", L"KXA", L"KXB", L"KXC", L"KXD", L"KXE", L"KXF", L"KXG", L"KXH", L"KXI", L"KXJ", L"KXK", L"KXL", L"KXM", L"KXN", L"KXO", L"KXP", L"KXQ", L"KXR", L"KXS", L"KXT", L"KXU", L"KXV", L"KXW", L"KXX", L"KXY", L"KXZ", L"KYA", L"KYB", L"KYC", L"KYD", L"KYE", L"KYF", L"KYG", L"KYH", L"KYI", L"KYJ", L"KYK", L"KYL", L"KYM", L"KYN", L"KYO", L"KYP", L"KYQ", L"KYR", L"KYS", L"KYT", L"KYU", L"KYV", L"KYW", L"KYX", L"KYY", L"KYZ", L"KZA", L"KZB", L"KZC", L"KZD", L"KZE", L"KZF", L"KZG", L"KZH", L"KZI", L"KZJ", L"KZK", L"KZL", L"KZM", L"KZN", L"KZO", L"KZP", L"KZQ", L"KZR", L"KZS", L"KZT", L"KZU", L"KZV", L"KZW", L"KZX", L"KZY", L"KZZ", L"LAA", L"LAB", L"LAC", L"LAD", L"LAE", L"LAF", L"LAG", L"LAH", L"LAI", L"LAJ", L"LAK", L"LAL", L"LAM", L"LAN", L"LAO", L"LAP", L"LAQ", L"LAR", L"LAS", L"LAT", L"LAU", L"LAV", L"LAW", L"LAX", L"LAY", L"LAZ", L"LBA", L"LBB", L"LBC", L"LBD", L"LBE", L"LBF", L"LBG", L"LBH", L"LBI", L"LBJ", L"LBK", L"LBL", L"LBM", L"LBN", L"LBO", L"LBP", L"LBQ", L"LBR", L"LBS", L"LBT", L"LBU", L"LBV", L"LBW", L"LBX", L"LBY", L"LBZ", L"LCA", L"LCB", L"LCC", L"LCD", L"LCE", L"LCF", L"LCG", L"LCH", L"LCI", L"LCJ", L"LCK", L"LCL", L"LCM", L"LCN", L"LCO", L"LCP", L"LCQ", L"LCR", L"LCS", L"LCT", L"LCU", L"LCV", L"LCW", L"LCX", L"LCY", L"LCZ", L"LDA", L"LDB", L"LDC", L"LDD", L"LDE", L"LDF", L"LDG", L"LDH", L"LDI", L"LDJ", L"LDK", L"LDL", L"LDM", L"LDN", L"LDO", L"LDP", L"LDQ", L"LDR", L"LDS", L"LDT", L"LDU", L"LDV", L"LDW", L"LDX", L"LDY", L"LDZ", L"LEA", L"LEB", L"LEC", L"LED", L"LEE", L"LEF", L"LEG", L"LEH", L"LEI", L"LEJ", L"LEK", 
			L"LEL", L"LEM", L"LEN", L"LEO", L"LEP", L"LEQ", L"LER", L"LES", L"LET", L"LEU", L"LEV", L"LEW", L"LEX", L"LEY", L"LEZ", L"LFA", L"LFB", L"LFC", L"LFD", L"LFE", L"LFF", L"LFG", L"LFH", L"LFI", L"LFJ", L"LFK", L"LFL", L"LFM", L"LFN", L"LFO", L"LFP", L"LFQ", L"LFR", L"LFS", L"LFT", L"LFU", L"LFV", L"LFW", L"LFX", L"LFY", L"LFZ", L"LGA", L"LGB", L"LGC", L"LGD", L"LGE", L"LGF", L"LGG", L"LGH", L"LGI", L"LGJ", L"LGK", L"LGL", L"LGM", L"LGN", L"LGO", L"LGP", L"LGQ", L"LGR", L"LGS", L"LGT", L"LGU", L"LGV", L"LGW", L"LGX", L"LGY", L"LGZ", L"LHA", L"LHB", L"LHC", L"LHD", L"LHE", L"LHF", L"LHG", L"LHH", L"LHI", L"LHJ", L"LHK", L"LHL", L"LHM", L"LHN", L"LHO", L"LHP", L"LHQ", L"LHR", L"LHS", L"LHT", L"LHU", L"LHV", L"LHW", L"LHX", L"LHY", L"LHZ", L"LIA", L"LIB", L"LIC", L"LID", L"LIE", L"LIF", L"LIG", L"LIH", L"LII", L"LIJ", L"LIK", L"LIL", L"LIM", L"LIN", L"LIO", L"LIP", L"LIQ", L"LIR", L"LIS", L"LIT", L"LIU", L"LIV", L"LIW", L"LIX", L"LIY", L"LIZ", L"LJA", L"LJB", L"LJC", L"LJD", L"LJE", L"LJF", L"LJG", L"LJH", L"LJI", L"LJJ", L"LJK", L"LJL", L"LJM", L"LJN", L"LJO", L"LJP", L"LJQ", L"LJR", L"LJS", L"LJT", L"LJU", L"LJV", L"LJW", L"LJX", L"LJY", L"LJZ", L"LKA", L"LKB", L"LKC", L"LKD", L"LKE", L"LKF", L"LKG", L"LKH", L"LKI", L"LKJ", L"LKK", L"LKL", L"LKM", L"LKN", L"LKO", L"LKP", L"LKQ", L"LKR", L"LKS", L"LKT", L"LKU", L"LKV", L"LKW", L"LKX", L"LKY", L"LKZ", L"LLA", L"LLB", L"LLC", L"LLD", L"LLE", L"LLF", L"LLG", L"LLH", L"LLI", L"LLJ", L"LLK", L"LLL", L"LLM", L"LLN", L"LLO", L"LLP", L"LLQ", L"LLR", L"LLS", L"LLT", L"LLU", L"LLV", L"LLW", L"LLX", L"LLY", L"LLZ", L"LMA", L"LMB", L"LMC", L"LMD", L"LME", L"LMF", L"LMG", L"LMH", L"LMI", L"LMJ", L"LMK", L"LML", L"LMM", L"LMN", L"LMO", L"LMP", L"LMQ", L"LMR", L"LMS", L"LMT", L"LMU", L"LMV", L"LMW", L"LMX", L"LMY", L"LMZ", L"LNA", L"LNB", L"LNC", L"LND", L"LNE", L"LNF", L"LNG", L"LNH", L"LNI", L"LNJ", L"LNK", L"LNL", L"LNM", L"LNN", L"LNO", L"LNP", L"LNQ", L"LNR", L"LNS", L"LNT", L"LNU", L"LNV", L"LNW", L"LNX", L"LNY", L"LNZ", L"LOA", L"LOB", L"LOC", L"LOD", L"LOE", L"LOF", L"LOG", L"LOH", L"LOI", L"LOJ", L"LOK", L"LOL", L"LOM", L"LON", L"LOO", L"LOP", L"LOQ", L"LOR", L"LOS", L"LOT", L"LOU", L"LOV", L"LOW", L"LOX", 
			L"LOY", L"LOZ", L"LPA", L"LPB", L"LPC", L"LPD", L"LPE", L"LPF", L"LPG", L"LPH", L"LPI", L"LPJ", L"LPK", L"LPL", L"LPM", L"LPN", L"LPO", L"LPP", L"LPQ", L"LPR", L"LPS", L"LPT", L"LPU", L"LPV", L"LPW", L"LPX", L"LPY", L"LPZ", L"LQA", L"LQB", L"LQC", L"LQD", L"LQE", L"LQF", L"LQG", L"LQH", L"LQI", L"LQJ", L"LQK", L"LQL", L"LQM", L"LQN", L"LQO", L"LQP", L"LQQ", L"LQR", L"LQS", L"LQT", L"LQU", L"LQV", L"LQW", L"LQX", L"LQY", L"LQZ", L"LRA", L"LRB", L"LRC", L"LRD", L"LRE", L"LRF", L"LRG", L"LRH", L"LRI", L"LRJ", L"LRK", L"LRL", L"LRM", L"LRN", L"LRO", L"LRP", L"LRQ", L"LRR", L"LRS", L"LRT", L"LRU", L"LRV", L"LRW", L"LRX", L"LRY", L"LRZ", L"LSA", L"LSB", L"LSC", L"LSD", L"LSE", L"LSF", L"LSG", L"LSH", L"LSI", L"LSJ", L"LSK", L"LSL", L"LSM", L"LSN", L"LSO", L"LSP", L"LSQ", L"LSR", L"LSS", L"LST", L"LSU", L"LSV", L"LSW", L"LSX", L"LSY", L"LSZ", L"LTA", L"LTB", L"LTC", L"LTD", L"LTE", L"LTF", L"LTG", L"LTH", L"LTI", L"LTJ", L"LTK", L"LTL", L"LTM", L"LTN", L"LTO", L"LTP", L"LTQ", L"LTR", L"LTS", L"LTT", L"LTU", L"LTV", L"LTW", L"LTX", L"LTY", L"LTZ", L"LUA", L"LUB", L"LUC", L"LUD", L"LUE", L"LUF", L"LUG", L"LUH", L"LUI", L"LUJ", L"LUK", L"LUL", L"LUM", L"LUN", L"LUO", L"LUP", L"LUQ", L"LUR", L"LUS", L"LUT", L"LUU", L"LUV", L"LUW", L"LUX", L"LUY", L"LUZ", L"LVA", L"LVB", L"LVC", L"LVD", L"LVE", L"LVF", L"LVG", L"LVH", L"LVI", L"LVJ", L"LVK", L"LVL", L"LVM", L"LVN", L"LVO", L"LVP", L"LVQ", L"LVR", L"LVS", L"LVT", L"LVU", L"LVV", L"LVW", L"LVX", L"LVY", L"LVZ", L"LWA", L"LWB", L"LWC", L"LWD", L"LWE", L"LWF", L"LWG", L"LWH", L"LWI", L"LWJ", L"LWK", L"LWL", L"LWM", L"LWN", L"LWO", L"LWP", L"LWQ", L"LWR", L"LWS", L"LWT", L"LWU", L"LWV", L"LWW", L"LWX", L"LWY", L"LWZ", L"LXA", L"LXB", L"LXC", L"LXD", L"LXE", L"LXF", L"LXG", L"LXH", L"LXI", L"LXJ", L"LXK", L"LXL", L"LXM", L"LXN", L"LXO", L"LXP", L"LXQ", L"LXR", L"LXS", L"LXT", L"LXU", L"LXV", L"LXW", L"LXX", L"LXY", L"LXZ", L"LYA", L"LYB", L"LYC", L"LYD", L"LYE", L"LYF", L"LYG", L"LYH", L"LYI", L"LYJ", L"LYK", L"LYL", L"LYM", L"LYN", L"LYO", L"LYP", L"LYQ", L"LYR", L"LYS", L"LYT", L"LYU", L"LYV", L"LYW", L"LYX", L"LYY", L"LYZ", L"LZA", L"LZB", L"LZC",
			L"LZD", L"LZE", L"LZF", L"LZG", L"LZH", L"LZI", L"LZJ", L"LZK", L"LZL", L"LZM", L"LZN", L"LZO", L"LZP", L"LZQ", L"LZR", L"LZS", L"LZT", L"LZU", L"LZV", L"LZW", L"LZX", L"LZY", L"LZZ", L"MAA", L"MAB", L"MAC", L"MAD", L"MAE", L"MAF", L"MAG", L"MAH", L"MAI", L"MAJ", L"MAK", L"MAL", L"MAM", L"MAN", L"MAO", L"MAP", L"MAQ", L"MAR", L"MAS", L"MAT", L"MAU", L"MAV", L"MAW", L"MAX", L"MAY", L"MAZ", L"MBA", L"MBB", L"MBC", L"MBD", L"MBE", L"MBF", L"MBG", L"MBH", L"MBI", L"MBJ", L"MBK", L"MBL", L"MBM", L"MBN", L"MBO", L"MBP", L"MBQ", L"MBR", L"MBS", L"MBT", L"MBU", L"MBV", L"MBW", L"MBX", L"MBY", L"MBZ", L"MCA", L"MCB", L"MCC", L"MCD", L"MCE", L"MCF", L"MCG", L"MCH", L"MCI", L"MCJ", L"MCK", L"MCL", L"MCM", L"MCN", L"MCO", L"MCP", L"MCQ", L"MCR", L"MCS", L"MCT", L"MCU", L"MCV", L"MCW", L"MCX", L"MCY", L"MCZ", L"MDA", L"MDB", L"MDC", L"MDD", L"MDE", L"MDF", L"MDG", L"MDH", L"MDI", L"MDJ", L"MDK", L"MDL", L"MDM", L"MDN", L"MDO", L"MDP", L"MDQ", L"MDR", L"MDS", L"MDT", L"MDU", L"MDV", L"MDW", L"MDX", L"MDY", L"MDZ", L"MEA", L"MEB", L"MEC", L"MED", L"MEE", L"MEF", L"MEG", L"MEH", L"MEI", L"MEJ", L"MEK", L"MEL", L"MEM", L"MEN", L"MEO", L"MEP", L"MEQ", L"MER", L"MES", L"MET", L"MEU", L"MEV", L"MEW", L"MEX", L"MEY", L"MEZ", L"MFA", L"MFB", L"MFC", L"MFD", L"MFE", L"MFF", L"MFG", L"MFH", L"MFI", L"MFJ", L"MFK", L"MFL", L"MFM", L"MFN", L"MFO", L"MFP", L"MFQ", L"MFR", L"MFS", L"MFT", L"MFU", L"MFV", L"MFW", L"MFX", L"MFY", L"MFZ", L"MGA", L"MGB", L"MGC", L"MGD", L"MGE", L"MGF", L"MGG", L"MGH", L"MGI", L"MGJ", L"MGK", L"MGL", L"MGM", L"MGN", L"MGO", L"MGP", L"MGQ", L"MGR", L"MGS", L"MGT", L"MGU", L"MGV", L"MGW", L"MGX", L"MGY", L"MGZ", L"MHA", L"MHB", L"MHC", L"MHD", L"MHE", L"MHF", L"MHG", L"MHH", L"MHI", L"MHJ", L"MHK", L"MHL", L"MHM", L"MHN", L"MHO", L"MHP", L"MHQ", L"MHR", L"MHS", L"MHT", L"MHU", L"MHV", L"MHW", L"MHX", L"MHY", L"MHZ", L"MIA", L"MIB", L"MIC", L"MID", L"MIE", L"MIF", L"MIG", L"MIH", L"MII", L"MIJ", L"MIK", L"MIL", L"MIM", L"MIN", L"MIO", L"MIP", L"MIQ", L"MIR", L"MIS", L"MIT", L"MIU", L"MIV", L"MIW", L"MIX", L"MIY", L"MIZ", L"MJA", L"MJB", L"MJC", L"MJD", L"MJE", L"MJF", L"MJG", L"MJH", L"MJI", L"MJJ", L"MJK", L"MJL", L"MJM", L"MJN", L"MJO", L"MJP",
			L"MJQ", L"MJR", L"MJS", L"MJT", L"MJU", L"MJV", L"MJW", L"MJX", L"MJY", L"MJZ", L"MKA", L"MKB", L"MKC", L"MKD", L"MKE", L"MKF", L"MKG", L"MKH", L"MKI", L"MKJ", L"MKK", L"MKL", L"MKM", L"MKN", L"MKO", L"MKP", L"MKQ", L"MKR", L"MKS", L"MKT", L"MKU", L"MKV", L"MKW", L"MKX", L"MKY", L"MKZ", L"MLA", L"MLB", L"MLC", L"MLD", L"MLE", L"MLF", L"MLG", L"MLH", L"MLI", L"MLJ", L"MLK", L"MLL", L"MLM", L"MLN", L"MLO", L"MLP", L"MLQ", L"MLR", L"MLS", L"MLT", L"MLU", L"MLV", L"MLW", L"MLX", L"MLY", L"MLZ", L"MMA", L"MMB", L"MMC", L"MMD", L"MME", L"MMF", L"MMG", L"MMH", L"MMI", L"MMJ", L"MMK", L"MML", L"MMM", L"MMN", L"MMO", L"MMP", L"MMQ", L"MMR", L"MMS", L"MMT", L"MMU", L"MMV", L"MMW", L"MMX", L"MMY", L"MMZ", L"MNA", L"MNB", L"MNC", L"MND", L"MNE", L"MNF", L"MNG", L"MNH", L"MNI", L"MNJ", L"MNK", L"MNL", L"MNM", L"MNN", L"MNO", L"MNP", L"MNQ", L"MNR", L"MNS", L"MNT", L"MNU", L"MNV", L"MNW", L"MNX", L"MNY", L"MNZ", L"MOA", L"MOB", L"MOC", L"MOD", L"MOE", L"MOF", L"MOG", L"MOH", L"MOI", L"MOJ", L"MOK", L"MOL", L"MOM", L"MON", L"MOO", L"MOP", L"MOQ", L"MOR", L"MOS", L"MOT", L"MOU", L"MOV", L"MOW", L"MOX", L"MOY", L"MOZ", L"MPA", L"MPB", L"MPC", L"MPD", L"MPE", L"MPF", L"MPG", L"MPH", L"MPI", L"MPJ", L"MPK", L"MPL", L"MPM", L"MPN", L"MPO", L"MPP", L"MPQ", L"MPR", L"MPS", L"MPT", L"MPU", L"MPV", L"MPW", L"MPX", L"MPY", L"MPZ", L"MQA", L"MQB", L"MQC", L"MQD", L"MQE", L"MQF", L"MQG", L"MQH", L"MQI", L"MQJ", L"MQK", L"MQL", L"MQM", L"MQN", L"MQO", L"MQP", L"MQQ", L"MQR", L"MQS", L"MQT", L"MQU", L"MQV", L"MQW", L"MQX", L"MQY", L"MQZ", L"MRA", L"MRB", L"MRC", L"MRD", L"MRE", L"MRF", L"MRG", L"MRH", L"MRI", L"MRJ", L"MRK", L"MRL", L"MRM", L"MRN", L"MRO", L"MRP", L"MRQ", L"MRR", L"MRS", L"MRT", L"MRU", L"MRV", L"MRW", L"MRX", L"MRY", L"MRZ", L"MSA", L"MSB", L"MSC", L"MSD", L"MSE", L"MSF", L"MSG", L"MSH", L"MSI", L"MSJ", L"MSK", L"MSL", L"MSM", L"MSN", L"MSO", L"MSP", L"MSQ", L"MSR", L"MSS", L"MST", L"MSU", L"MSV", L"MSW", L"MSX", L"MSY", L"MSZ", L"MTA", L"MTB", L"MTC", L"MTD", L"MTE", L"MTF", L"MTG", L"MTH", L"MTI", L"MTJ", L"MTK", L"MTL", L"MTM", L"MTN", L"MTO", L"MTP", L"MTQ", L"MTR", L"MTS", L"MTT", L"MTU", L"MTV", L"MTW", L"MTX", L"MTY", L"MTZ", L"MUA", L"MUB", L"MUC",
			L"MUD", L"MUE", L"MUF", L"MUG", L"MUH", L"MUI", L"MUJ", L"MUK", L"MUL", L"MUM", L"MUN", L"MUO", L"MUP", L"MUQ", L"MUR", L"MUS", L"MUT", L"MUU", L"MUV", L"MUW", L"MUX", L"MUY", L"MUZ", L"MVA", L"MVB", L"MVC", L"MVD", L"MVE", L"MVF", L"MVG", L"MVH", L"MVI", L"MVJ", L"MVK", L"MVL", L"MVM", L"MVN", L"MVO", L"MVP", L"MVQ", L"MVR", L"MVS", L"MVT", L"MVU", L"MVV", L"MVW", L"MVX", L"MVY", L"MVZ", L"MWA", L"MWB", L"MWC", L"MWD", L"MWE", L"MWF", L"MWG", L"MWH", L"MWI", L"MWJ", L"MWK", L"MWL", L"MWM", L"MWN", L"MWO", L"MWP", L"MWQ", L"MWR", L"MWS", L"MWT", L"MWU", L"MWV", L"MWW", L"MWX", L"MWY", L"MWZ", L"MXA", L"MXB", L"MXC", L"MXD", L"MXE", L"MXF", L"MXG", L"MXH", L"MXI", L"MXJ", L"MXK", L"MXL", L"MXM", L"MXN", L"MXO", L"MXP", L"MXQ", L"MXR", L"MXS", L"MXT", L"MXU", L"MXV", L"MXW", L"MXX", L"MXY", L"MXZ", L"MYA", L"MYB", L"MYC", L"MYD", L"MYE", L"MYF", L"MYG", L"MYH", L"MYI", L"MYJ", L"MYK", L"MYL", L"MYM", L"MYN", L"MYO", L"MYP", L"MYQ", L"MYR", L"MYS", L"MYT", L"MYU", L"MYV", L"MYW", L"MYX", L"MYY", L"MYZ", L"MZA", L"MZB", L"MZC", L"MZD", L"MZE", L"MZF", L"MZG", L"MZH", L"MZI", L"MZJ", L"MZK", L"MZL", L"MZM", L"MZN", L"MZO", L"MZP", L"MZQ", L"MZR", L"MZS", L"MZT", L"MZU", L"MZV", L"MZW", L"MZX", L"MZY", L"MZZ", L"NAA", L"NAB", L"NAC", L"NAD", L"NAE", L"NAF", L"NAG", L"NAH", L"NAI", L"NAJ", L"NAK", L"NAL", L"NAM", L"NAN", L"NAO", L"NAP", L"NAQ", L"NAR", L"NAS", L"NAT", L"NAU", L"NAV", L"NAW", L"NAX", L"NAY", L"NAZ", L"NBA", L"NBB", L"NBC", L"NBD", L"NBE", L"NBF", L"NBG", L"NBH", L"NBI", L"NBJ", L"NBK", L"NBL", L"NBM", L"NBN", L"NBO", L"NBP", L"NBQ", L"NBR", L"NBS", L"NBT", L"NBU", L"NBV", L"NBW", L"NBX", L"NBY", L"NBZ", L"NCA", L"NCB", L"NCC", L"NCD", L"NCE", L"NCF", L"NCG", L"NCH", L"NCI", L"NCJ", L"NCK", L"NCL", L"NCM", L"NCN", L"NCO", L"NCP", L"NCQ", L"NCR", L"NCS", L"NCT", L"NCU", L"NCV", L"NCW", L"NCX", L"NCY", L"NCZ", L"NDA", L"NDB", L"NDC", L"NDD", L"NDE", L"NDF", L"NDG", L"NDH", L"NDI", L"NDJ", L"NDK", L"NDL", L"NDM", L"NDN", L"NDO", L"NDP", L"NDQ", L"NDR", L"NDS", L"NDT", L"NDU", L"NDV", L"NDW", L"NDX", L"NDY", L"NDZ", L"NEA", L"NEB", L"NEC", L"NED", L"NEE", L"NEF", L"NEG", L"NEH", L"NEI", L"NEJ", L"NEK", L"NEL", L"NEM", L"NEN", L"NEO", L"NEP", 
			L"NEQ", L"NER", L"NES", L"NET", L"NEU", L"NEV", L"NEW", L"NEX", L"NEY", L"NEZ", L"NFA", L"NFB", L"NFC", L"NFD", L"NFE", L"NFF", L"NFG", L"NFH", L"NFI", L"NFJ", L"NFK", L"NFL", L"NFM", L"NFN", L"NFO", L"NFP", L"NFQ", L"NFR", L"NFS", L"NFT", L"NFU", L"NFV", L"NFW", L"NFX", L"NFY", L"NFZ", L"NGA", L"NGB", L"NGC", L"NGD", L"NGE", L"NGF", L"NGG", L"NGH", L"NGI", L"NGJ", L"NGK", L"NGL", L"NGM", L"NGN", L"NGO", L"NGP", L"NGQ", L"NGR", L"NGS", L"NGT", L"NGU", L"NGV", L"NGW", L"NGX", L"NGY", L"NGZ", L"NHA", L"NHB", L"NHC", L"NHD", L"NHE", L"NHF", L"NHG", L"NHH", L"NHI", L"NHJ", L"NHK", L"NHL", L"NHM", L"NHN", L"NHO", L"NHP", L"NHQ", L"NHR", L"NHS", L"NHT", L"NHU", L"NHV", L"NHW", L"NHX", L"NHY", L"NHZ", L"NIA", L"NIB", L"NIC", L"NID", L"NIE", L"NIF", L"NIG", L"NIH", L"NII", L"NIJ", L"NIK", L"NIL", L"NIM", L"NIN", L"NIO", L"NIP", L"NIQ", L"NIR", L"NIS", L"NIT", L"NIU", L"NIV", L"NIW", L"NIX", L"NIY", L"NIZ", L"NJA", L"NJB", L"NJC", L"NJD", L"NJE", L"NJF", L"NJG", L"NJH", L"NJI", L"NJJ", L"NJK", L"NJL", L"NJM", L"NJN", L"NJO", L"NJP", L"NJQ", L"NJR", L"NJS", L"NJT", L"NJU", L"NJV", L"NJW", L"NJX", L"NJY", L"NJZ", L"NKA", L"NKB", L"NKC", L"NKD", L"NKE", L"NKF", L"NKG", L"NKH", L"NKI", L"NKJ", L"NKK", L"NKL", L"NKM", L"NKN", L"NKO", L"NKP", L"NKQ", L"NKR", L"NKS", L"NKT", L"NKU", L"NKV", L"NKW", L"NKX", L"NKY", L"NKZ", L"NLA", L"NLB", L"NLC", L"NLD", L"NLE", L"NLF", L"NLG", L"NLH", L"NLI", L"NLJ", L"NLK", L"NLL", L"NLM", L"NLN", L"NLO", L"NLP", L"NLQ", L"NLR", L"NLS", L"NLT", L"NLU", L"NLV", L"NLW", L"NLX", L"NLY", L"NLZ", L"NMA", L"NMB", L"NMC", L"NMD", L"NME", L"NMF", L"NMG", L"NMH", L"NMI", L"NMJ", L"NMK", L"NML", L"NMM", L"NMN", L"NMO", L"NMP", L"NMQ", L"NMR", L"NMS", L"NMT", L"NMU", L"NMV", L"NMW", L"NMX", L"NMY", L"NMZ", L"NNA", L"NNB", L"NNC", L"NND", L"NNE", L"NNF", L"NNG", L"NNH", L"NNI", L"NNJ", L"NNK", L"NNL", L"NNM", L"NNN", L"NNO", L"NNP", L"NNQ", L"NNR", L"NNS", L"NNT", L"NNU", L"NNV", L"NNW", L"NNX", L"NNY", L"NNZ", L"NOA", L"NOB", L"NOC", L"NOD", L"NOE", L"NOF", L"NOG", L"NOH", L"NOI", L"NOJ", L"NOK", L"NOL", L"NOM", L"NON", L"NOO", L"NOP", L"NOQ", L"NOR", L"NOS", L"NOT", L"NOU", L"NOV", L"NOW", L"NOX", L"NOY", L"NOZ", L"NPA", L"NPB", L"NPC", 
			L"NPD", L"NPE", L"NPF", L"NPG", L"NPH", L"NPI", L"NPJ", L"NPK", L"NPL", L"NPM", L"NPN", L"NPO", L"NPP", L"NPQ", L"NPR", L"NPS", L"NPT", L"NPU", L"NPV", L"NPW", L"NPX", L"NPY", L"NPZ", L"NQA", L"NQB", L"NQC", L"NQD", L"NQE", L"NQF", L"NQG", L"NQH", L"NQI", L"NQJ", L"NQK", L"NQL", L"NQM", L"NQN", L"NQO", L"NQP", L"NQQ", L"NQR", L"NQS", L"NQT", L"NQU", L"NQV", L"NQW", L"NQX", L"NQY", L"NQZ", L"NRA", L"NRB", L"NRC", L"NRD", L"NRE", L"NRF", L"NRG", L"NRH", L"NRI", L"NRJ", L"NRK", L"NRL", L"NRM", L"NRN", L"NRO", L"NRP", L"NRQ", L"NRR", L"NRS", L"NRT", L"NRU", L"NRV", L"NRW", L"NRX", L"NRY", L"NRZ", L"NSA", L"NSB", L"NSC", L"NSD", L"NSE", L"NSF", L"NSG", L"NSH", L"NSI", L"NSJ", L"NSK", L"NSL", L"NSM", L"NSN", L"NSO", L"NSP", L"NSQ", L"NSR", L"NSS", L"NST", L"NSU", L"NSV", L"NSW", L"NSX", L"NSY", L"NSZ", L"NTA", L"NTB", L"NTC", L"NTD", L"NTE", L"NTF", L"NTG", L"NTH", L"NTI", L"NTJ", L"NTK", L"NTL", L"NTM", L"NTN", L"NTO", L"NTP", L"NTQ", L"NTR", L"NTS", L"NTT", L"NTU", L"NTV", L"NTW", L"NTX", L"NTY", L"NTZ", L"NUA", L"NUB", L"NUC", L"NUD", L"NUE", L"NUF", L"NUG", L"NUH", L"NUI", L"NUJ", L"NUK", L"NUL", L"NUM", L"NUN", L"NUO", L"NUP", L"NUQ", L"NUR", L"NUS", L"NUT", L"NUU", L"NUV", L"NUW", L"NUX", L"NUY", L"NUZ", L"NVA", L"NVB", L"NVC", L"NVD", L"NVE", L"NVF", L"NVG", L"NVH", L"NVI", L"NVJ", L"NVK", L"NVL", L"NVM", L"NVN", L"NVO", L"NVP", L"NVQ", L"NVR", L"NVS", L"NVT", L"NVU", L"NVV", L"NVW", L"NVX", L"NVY", L"NVZ", L"NWA", L"NWB", L"NWC", L"NWD", L"NWE", L"NWF", L"NWG", L"NWH", L"NWI", L"NWJ", L"NWK", L"NWL", L"NWM", L"NWN", L"NWO", L"NWP", L"NWQ", L"NWR", L"NWS", L"NWT", L"NWU", L"NWV", L"NWW", L"NWX", L"NWY", L"NWZ", L"NXA", L"NXB", L"NXC", L"NXD", L"NXE", L"NXF", L"NXG", L"NXH", L"NXI", L"NXJ", L"NXK", L"NXL", L"NXM", L"NXN", L"NXO", L"NXP", L"NXQ", L"NXR", L"NXS", L"NXT", L"NXU", L"NXV", L"NXW", L"NXX", L"NXY", L"NXZ", L"NYA", L"NYB", L"NYC", L"NYD", L"NYE", L"NYF", L"NYG", L"NYH", L"NYI", L"NYJ", L"NYK", L"NYL", L"NYM", L"NYN", L"NYO", L"NYP", L"NYQ", L"NYR", L"NYS", L"NYT", L"NYU", L"NYV", L"NYW", L"NYX", L"NYY", L"NYZ", L"NZA", L"NZB", L"NZC", L"NZD", L"NZE", L"NZF", L"NZG", L"NZH", L"NZI", L"NZJ", L"NZK", L"NZL", L"NZM", L"NZN", L"NZO", L"NZP", 
			L"NZQ", L"NZR", L"NZS", L"NZT", L"NZU", L"NZV", L"NZW", L"NZX", L"NZY", L"NZZ", L"OAA", L"OAB", L"OAC", L"OAD", L"OAE", L"OAF", L"OAG", L"OAH", L"OAI", L"OAJ", L"OAK", L"OAL", L"OAM", L"OAN", L"OAO", L"OAP", L"OAQ", L"OAR", L"OAS", L"OAT", L"OAU", L"OAV", L"OAW", L"OAX", L"OAY", L"OAZ", L"OBA", L"OBB", L"OBC", L"OBD", L"OBE", L"OBF", L"OBG", L"OBH", L"OBI", L"OBJ", L"OBK", L"OBL", L"OBM", L"OBN", L"OBO", L"OBP", L"OBQ", L"OBR", L"OBS", L"OBT", L"OBU", L"OBV", L"OBW", L"OBX", L"OBY", L"OBZ", L"OCA", L"OCB", L"OCC", L"OCD", L"OCE", L"OCF", L"OCG", L"OCH", L"OCI", L"OCJ", L"OCK", L"OCL", L"OCM", L"OCN", L"OCO", L"OCP", L"OCQ", L"OCR", L"OCS", L"OCT", L"OCU", L"OCV", L"OCW", L"OCX", L"OCY", L"OCZ", L"ODA", L"ODB", L"ODC", L"ODD", L"ODE", L"ODF", L"ODG", L"ODH", L"ODI", L"ODJ", L"ODK", L"ODL", L"ODM", L"ODN", L"ODO", L"ODP", L"ODQ", L"ODR", L"ODS", L"ODT", L"ODU", L"ODV", L"ODW", L"ODX", L"ODY", L"ODZ", L"OEA", L"OEB", L"OEC", L"OED", L"OEE", L"OEF", L"OEG", L"OEH", L"OEI", L"OEJ", L"OEK", L"OEL", L"OEM", L"OEN", L"OEO", L"OEP", L"OEQ", L"OER", L"OES", L"OET", L"OEU", L"OEV", L"OEW", L"OEX", L"OEY", L"OEZ", L"OFA", L"OFB", L"OFC", L"OFD", L"OFE", L"OFF", L"OFG", L"OFH", L"OFI", L"OFJ", L"OFK", L"OFL", L"OFM", L"OFN", L"OFO", L"OFP", L"OFQ", L"OFR", L"OFS", L"OFT", L"OFU", L"OFV", L"OFW", L"OFX", L"OFY", L"OFZ", L"OGA", L"OGB", L"OGC", L"OGD", L"OGE", L"OGF", L"OGG", L"OGH", L"OGI", L"OGJ", L"OGK", L"OGL", L"OGM", L"OGN", L"OGO", L"OGP", L"OGQ", L"OGR", L"OGS", L"OGT", L"OGU", L"OGV", L"OGW", L"OGX", L"OGY", L"OGZ", L"OHA", L"OHB", L"OHC", L"OHD", L"OHE", L"OHF", L"OHG", L"OHH", L"OHI", L"OHJ", L"OHK", L"OHL", L"OHM", L"OHN", L"OHO", L"OHP", L"OHQ", L"OHR", L"OHS", L"OHT", L"OHU", L"OHV", L"OHW", L"OHX", L"OHY", L"OHZ", L"OIA", L"OIB", L"OIC", L"OID", L"OIE", L"OIF", L"OIG", L"OIH", L"OII", L"OIJ", L"OIK", L"OIL", L"OIM", L"OIN", L"OIO", L"OIP", L"OIQ", L"OIR", L"OIS", L"OIT", L"OIU", L"OIV", L"OIW", L"OIX", L"OIY", L"OIZ", L"OJA", L"OJB", L"OJC", L"OJD", L"OJE", L"OJF", L"OJG", L"OJH", L"OJI", L"OJJ", L"OJK", L"OJL", L"OJM", L"OJN", L"OJO", L"OJP", L"OJQ", L"OJR", L"OJS", L"OJT", L"OJU", L"OJV", L"OJW", L"OJX", L"OJY", L"OJZ", L"OKA", L"OKB", L"OKC", 
			L"OKD", L"OKE", L"OKF", L"OKG", L"OKH", L"OKI", L"OKJ", L"OKK", L"OKL", L"OKM", L"OKN", L"OKO", L"OKP", L"OKQ", L"OKR", L"OKS", L"OKT", L"OKU", L"OKV", L"OKW", L"OKX", L"OKY", L"OKZ", L"OLA", L"OLB", L"OLC", L"OLD", L"OLE", L"OLF", L"OLG", L"OLH", L"OLI", L"OLJ", L"OLK", L"OLL", L"OLM", L"OLN", L"OLO", L"OLP", L"OLQ", L"OLR", L"OLS", L"OLT", L"OLU", L"OLV", L"OLW", L"OLX", L"OLY", L"OLZ", L"OMA", L"OMB", L"OMC", L"OMD", L"OME", L"OMF", L"OMG", L"OMH", L"OMI", L"OMJ", L"OMK", L"OML", L"OMM", L"OMN", L"OMO", L"OMP", L"OMQ", L"OMR", L"OMS", L"OMT", L"OMU", L"OMV", L"OMW", L"OMX", L"OMY", L"OMZ", L"ONA", L"ONB", L"ONC", L"OND", L"ONE", L"ONF", L"ONG", L"ONH", L"ONI", L"ONJ", L"ONK", L"ONL", L"ONM", L"ONN", L"ONO", L"ONP", L"ONQ", L"ONR", L"ONS", L"ONT", L"ONU", L"ONV", L"ONW", L"ONX", L"ONY", L"ONZ", L"OOA", L"OOB", L"OOC", L"OOD", L"OOE", L"OOF", L"OOG", L"OOH", L"OOI", L"OOJ", L"OOK", L"OOL", L"OOM", L"OON", L"OOO", L"OOP", L"OOQ", L"OOR", L"OOS", L"OOT", L"OOU", L"OOV", L"OOW", L"OOX", L"OOY", L"OOZ", L"OPA", L"OPB", L"OPC", L"OPD", L"OPE", L"OPF", L"OPG", L"OPH", L"OPI", L"OPJ", L"OPK", L"OPL", L"OPM", L"OPN", L"OPO", L"OPP", L"OPQ", L"OPR", L"OPS", L"OPT", L"OPU", L"OPV", L"OPW", L"OPX", L"OPY", L"OPZ", L"OQA", L"OQB", L"OQC", L"OQD", L"OQE", L"OQF", L"OQG", L"OQH", L"OQI", L"OQJ", L"OQK", L"OQL", L"OQM", L"OQN", L"OQO", L"OQP", L"OQQ", L"OQR", L"OQS", L"OQT", L"OQU", L"OQV", L"OQW", L"OQX", L"OQY", L"OQZ", L"ORA", L"ORB", L"ORC", L"ORD", L"ORE", L"ORF", L"ORG", L"ORH", L"ORI", L"ORJ", L"ORK", L"ORL", L"ORM", L"ORN", L"ORO", L"ORP", L"ORQ", L"ORR", L"ORS", L"ORT", L"ORU", L"ORV", L"ORW", L"ORX", L"ORY", L"ORZ", L"OSA", L"OSB", L"OSC", L"OSD", L"OSE", L"OSF", L"OSG", L"OSH", L"OSI", L"OSJ", L"OSK", L"OSL", L"OSM", L"OSN", L"OSO", L"OSP", L"OSQ", L"OSR", L"OSS", L"OST", L"OSU", L"OSV", L"OSW", L"OSX", L"OSY", L"OSZ", L"OTA", L"OTB", L"OTC", L"OTD", L"OTE", L"OTF", L"OTG", L"OTH", L"OTI", L"OTJ", L"OTK", L"OTL", L"OTM", L"OTN", L"OTO", L"OTP", L"OTQ", L"OTR", L"OTS", L"OTT", L"OTU", L"OTV", L"OTW", L"OTX", L"OTY", L"OTZ", L"OUA", L"OUB", L"OUC", L"OUD", L"OUE", L"OUF", L"OUG", L"OUH", L"OUI", L"OUJ", L"OUK", L"OUL", L"OUM", L"OUN", L"OUO", L"OUP", 
			L"OUQ", L"OUR", L"OUS", L"OUT", L"OUU", L"OUV", L"OUW", L"OUX", L"OUY", L"OUZ", L"OVA", L"OVB", L"OVC", L"OVD", L"OVE", L"OVF", L"OVG", L"OVH", L"OVI", L"OVJ", L"OVK", L"OVL", L"OVM", L"OVN", L"OVO", L"OVP", L"OVQ", L"OVR", L"OVS", L"OVT", L"OVU", L"OVV", L"OVW", L"OVX", L"OVY", L"OVZ", L"OWA", L"OWB", L"OWC", L"OWD", L"OWE", L"OWF", L"OWG", L"OWH", L"OWI", L"OWJ", L"OWK", L"OWL", L"OWM", L"OWN", L"OWO", L"OWP", L"OWQ", L"OWR", L"OWS", L"OWT", L"OWU", L"OWV", L"OWW", L"OWX", L"OWY", L"OWZ", L"OXA", L"OXB", L"OXC", L"OXD", L"OXE", L"OXF", L"OXG", L"OXH", L"OXI", L"OXJ", L"OXK", L"OXL", L"OXM", L"OXN", L"OXO", L"OXP", L"OXQ", L"OXR", L"OXS", L"OXT", L"OXU", L"OXV", L"OXW", L"OXX", L"OXY", L"OXZ", L"OYA", L"OYB", L"OYC", L"OYD", L"OYE", L"OYF", L"OYG", L"OYH", L"OYI", L"OYJ", L"OYK", L"OYL", L"OYM", L"OYN", L"OYO", L"OYP", L"OYQ", L"OYR", L"OYS", L"OYT", L"OYU", L"OYV", L"OYW", L"OYX", L"OYY", L"OYZ", L"OZA", L"OZB", L"OZC", L"OZD", L"OZE", L"OZF", L"OZG", L"OZH", L"OZI", L"OZJ", L"OZK", L"OZL", L"OZM", L"OZN", L"OZO", L"OZP", L"OZQ", L"OZR", L"OZS", L"OZT", L"OZU", L"OZV", L"OZW", L"OZX", L"OZY", L"OZZ", L"PAA", L"PAB", L"PAC", L"PAD", L"PAE", L"PAF", L"PAG", L"PAH", L"PAI", L"PAJ", L"PAK", L"PAL", L"PAM", L"PAN", L"PAO", L"PAP", L"PAQ", L"PAR", L"PAS", L"PAT", L"PAU", L"PAV", L"PAW", L"PAX", L"PAY", L"PAZ", L"PBA", L"PBB", L"PBC", L"PBD", L"PBE", L"PBF", L"PBG", L"PBH", L"PBI", L"PBJ", L"PBK", L"PBL", L"PBM", L"PBN", L"PBO", L"PBP", L"PBQ", L"PBR", L"PBS", L"PBT", L"PBU", L"PBV", L"PBW", L"PBX", L"PBY", L"PBZ", L"PCA", L"PCB", L"PCC", L"PCD", L"PCE", L"PCF", L"PCG", L"PCH", L"PCI", L"PCJ", L"PCK", L"PCL", L"PCM", L"PCN", L"PCO", L"PCP", L"PCQ", L"PCR", L"PCS", L"PCT", L"PCU", L"PCV", L"PCW", L"PCX", L"PCY", L"PCZ", L"PDA", L"PDB", L"PDC", L"PDD", L"PDE", L"PDF", L"PDG", L"PDH", L"PDI", L"PDJ", L"PDK", L"PDL", L"PDM", L"PDN", L"PDO", L"PDP", L"PDQ", L"PDR", L"PDS", L"PDT", L"PDU", L"PDV", L"PDW", L"PDX", L"PDY", L"PDZ", L"PEA", L"PEB", L"PEC", L"PED", L"PEE", L"PEF", L"PEG", L"PEH", L"PEI", L"PEJ", L"PEK", L"PEL", L"PEM", L"PEN", L"PEO", L"PEP", L"PEQ", L"PER", L"PES", L"PET", L"PEU", L"PEV", L"PEW", L"PEX", L"PEY", L"PEZ", L"PFA", L"PFB", L"PFC", 
			L"PFD", L"PFE", L"PFF", L"PFG", L"PFH", L"PFI", L"PFJ", L"PFK", L"PFL", L"PFM", L"PFN", L"PFO", L"PFP", L"PFQ", L"PFR", L"PFS", L"PFT", L"PFU", L"PFV", L"PFW", L"PFX", L"PFY", L"PFZ", L"PGA", L"PGB", L"PGC", L"PGD", L"PGE", L"PGF", L"PGG", L"PGH", L"PGI", L"PGJ", L"PGK", L"PGL", L"PGM", L"PGN", L"PGO", L"PGP", L"PGQ", L"PGR", L"PGS", L"PGT", L"PGU", L"PGV", L"PGW", L"PGX", L"PGY", L"PGZ", L"PHA", L"PHB", L"PHC", L"PHD", L"PHE", L"PHF", L"PHG", L"PHH", L"PHI", L"PHJ", L"PHK", L"PHL", L"PHM", L"PHN", L"PHO", L"PHP", L"PHQ", L"PHR", L"PHS", L"PHT", L"PHU", L"PHV", L"PHW", L"PHX", L"PHY", L"PHZ", L"PIA", L"PIB", L"PIC", L"PID", L"PIE", L"PIF", L"PIG", L"PIH", L"PII", L"PIJ", L"PIK", L"PIL", L"PIM", L"PIN", L"PIO", L"PIP", L"PIQ", L"PIR", L"PIS", L"PIT", L"PIU", L"PIV", L"PIW", L"PIX", L"PIY", L"PIZ", L"PJA", L"PJB", L"PJC", L"PJD", L"PJE", L"PJF", L"PJG", L"PJH", L"PJI", L"PJJ", L"PJK", L"PJL", L"PJM", L"PJN", L"PJO", L"PJP", L"PJQ", L"PJR", L"PJS", L"PJT", L"PJU", L"PJV", L"PJW", L"PJX", L"PJY", L"PJZ", L"PKA", L"PKB", L"PKC", L"PKD", L"PKE", L"PKF", L"PKG", L"PKH", L"PKI", L"PKJ", L"PKK", L"PKL", L"PKM", L"PKN", L"PKO", L"PKP", L"PKQ", L"PKR", L"PKS", L"PKT", L"PKU", L"PKV", L"PKW", L"PKX", L"PKY", L"PKZ", L"PLA", L"PLB", L"PLC", L"PLD", L"PLE", L"PLF", L"PLG", L"PLH", L"PLI", L"PLJ", L"PLK", L"PLL", L"PLM", L"PLN", L"PLO", L"PLP", L"PLQ", L"PLR", L"PLS", L"PLT", L"PLU", L"PLV", L"PLW", L"PLX", L"PLY", L"PLZ", L"PMA", L"PMB", L"PMC", L"PMD", L"PME", L"PMF", L"PMG", L"PMH", L"PMI", L"PMJ", L"PMK", L"PML", L"PMM", L"PMN", L"PMO", L"PMP", L"PMQ", L"PMR", L"PMS", L"PMT", L"PMU", L"PMV", L"PMW", L"PMX", L"PMY", L"PMZ", L"PNA", L"PNB", L"PNC", L"PND", L"PNE", L"PNF", L"PNG", L"PNH", L"PNI", L"PNJ", L"PNK", L"PNL", L"PNM", L"PNN", L"PNO", L"PNP", L"PNQ", L"PNR", L"PNS", L"PNT", L"PNU", L"PNV", L"PNW", L"PNX", L"PNY", L"PNZ", L"POA", L"POB", L"POC", L"POD", L"POE", L"POF", L"POG", L"POH", L"POI", L"POJ", L"POK", L"POL", L"POM", L"PON", L"POO", L"POP", L"POQ", L"POR", L"POS", L"POT", L"POU", L"POV", L"POW", L"POX", L"POY", L"POZ", L"PPA", L"PPB", L"PPC", L"PPD", L"PPE", L"PPF", L"PPG", L"PPH",
			L"PPI", L"PPJ", L"PPK", L"PPL", L"PPM", L"PPN", L"PPO", L"PPP", L"PPQ", L"PPR", L"PPS", L"PPT", L"PPU", L"PPV", L"PPW", L"PPX", L"PPY", L"PPZ", L"PQA", L"PQB", L"PQC", L"PQD", L"PQE", L"PQF", L"PQG", L"PQH", L"PQI", L"PQJ", L"PQK", L"PQL", L"PQM", L"PQN", L"PQO", L"PQP", L"PQQ", L"PQR", L"PQS", L"PQT", L"PQU", L"PQV", L"PQW", L"PQX", L"PQY", L"PQZ", L"PRA", L"PRB", L"PRC", L"PRD", L"PRE", L"PRF", L"PRG", L"PRH", L"PRI", L"PRJ", L"PRK", L"PRL", L"PRM", L"PRN", L"PRO", L"PRP", L"PRQ", L"PRR", L"PRS", L"PRT", L"PRU", L"PRV", L"PRW", L"PRX", L"PRY", L"PRZ", L"PSA", L"PSB", L"PSC", L"PSD", L"PSE", L"PSF", L"PSG", L"PSH", L"PSI", L"PSJ", L"PSK", L"PSL", L"PSM", L"PSN", L"PSO", L"PSP", L"PSQ", L"PSR", L"PSS", L"PST", L"PSU", L"PSV", L"PSW", L"PSX", L"PSY", L"PSZ", L"PTA", L"PTB", L"PTC", L"PTD", L"PTE", L"PTF", L"PTG", L"PTH", L"PTI", L"PTJ", L"PTK", L"PTL", L"PTM", L"PTN", L"PTO", L"PTP", L"PTQ", L"PTR", L"PTS", L"PTT", L"PTU", L"PTV", L"PTW", L"PTX", L"PTY", L"PTZ", L"PUA", L"PUB", L"PUC", L"PUD", L"PUE", L"PUF", L"PUG", L"PUH", L"PUI", L"PUJ", L"PUK", L"PUL", L"PUM", L"PUN", L"PUO", L"PUP", L"PUQ", L"PUR", L"PUS", L"PUT", L"PUU", L"PUV", L"PUW", L"PUX", L"PUY", L"PUZ", L"PVA", L"PVB", L"PVC", L"PVD", L"PVE", L"PVF", L"PVG", L"PVH", L"PVI", L"PVJ", L"PVK", L"PVL", L"PVM", L"PVN", L"PVO", L"PVP", L"PVQ", L"PVR", L"PVS", L"PVT", L"PVU", L"PVV", L"PVW", L"PVX", L"PVY", L"PVZ", L"PWA", L"PWB", L"PWC", L"PWD", L"PWE", L"PWF", L"PWG", L"PWH", L"PWI", L"PWJ", L"PWK", L"PWL", L"PWM", L"PWN", L"PWO", L"PWP", L"PWQ", L"PWR", L"PWS", L"PWT", L"PWU", L"PWV", L"PWW", L"PWX", L"PWY", L"PWZ", L"PXA", L"PXB", L"PXC", L"PXD", L"PXE", L"PXF", L"PXG", L"PXH", L"PXI", L"PXJ", L"PXK", L"PXL", L"PXM", L"PXN", L"PXO", L"PXP", L"PXQ", L"PXR", L"PXS", L"PXT", L"PXU", L"PXV", L"PXW", L"PXX", L"PXY", L"PXZ", L"PYA", L"PYB", L"PYC", L"PYD", L"PYE", L"PYF", L"PYG", L"PYH", L"PYI", L"PYJ", L"PYK", L"PYL", L"PYM", L"PYN", L"PYO", L"PYP", L"PYQ", L"PYR", L"PYS", L"PYT", L"PYU", L"PYV", L"PYW", L"PYX", L"PYY", L"PYZ", L"PZA", L"PZB", L"PZC", L"PZD", L"PZE", L"PZF", L"PZG", L"PZH", L"PZI", L"PZJ", L"PZK", L"PZL", L"PZM", L"PZN", L"PZO", L"PZP", L"PZQ", L"PZR", L"PZS", L"PZT", L"PZU", 
			L"PZV", L"PZW", L"PZX", L"PZY", L"PZZ", L"QAA", L"QAB", L"QAC", L"QAD", L"QAE", L"QAF", L"QAG", L"QAH", L"QAI", L"QAJ", L"QAK", L"QAL", L"QAM", L"QAN", L"QAO", L"QAP", L"QAQ", L"QAR", L"QAS", L"QAT", L"QAU", L"QAV", L"QAW", L"QAX", L"QAY", L"QAZ", L"QBA", L"QBB", L"QBC", L"QBD", L"QBE", L"QBF", L"QBG", L"QBH", L"QBI", L"QBJ", L"QBK", L"QBL", L"QBM", L"QBN", L"QBO", L"QBP", L"QBQ", L"QBR", L"QBS", L"QBT", L"QBU", L"QBV", L"QBW", L"QBX", L"QBY", L"QBZ", L"QCA", L"QCB", L"QCC", L"QCD", L"QCE", L"QCF", L"QCG", L"QCH", L"QCI", L"QCJ", L"QCK", L"QCL", L"QCM", L"QCN", L"QCO", L"QCP", L"QCQ", L"QCR", L"QCS", L"QCT", L"QCU", L"QCV", L"QCW", L"QCX", L"QCY", L"QCZ", L"QDA", L"QDB", L"QDC", L"QDD", L"QDE", L"QDF", L"QDG", L"QDH", L"QDI", L"QDJ", L"QDK", L"QDL", L"QDM", L"QDN", L"QDO", L"QDP", L"QDQ", L"QDR", L"QDS", L"QDT", L"QDU", L"QDV", L"QDW", L"QDX", L"QDY", L"QDZ", L"QEA", L"QEB", L"QEC", L"QED", L"QEE", L"QEF", L"QEG", L"QEH", L"QEI", L"QEJ", L"QEK", L"QEL", L"QEM", L"QEN", L"QEO", L"QEP", L"QEQ", L"QER", L"QES", L"QET", L"QEU", L"QEV", L"QEW", L"QEX", L"QEY", L"QEZ", L"QFA", L"QFB", L"QFC", L"QFD", L"QFE", L"QFF", L"QFG", L"QFH", L"QFI", L"QFJ", L"QFK", L"QFL", L"QFM", L"QFN", L"QFO", L"QFP", L"QFQ", L"QFR", L"QFS", L"QFT", L"QFU", L"QFV", L"QFW", L"QFX", L"QFY", L"QFZ", L"QGA", L"QGB", L"QGC", L"QGD", L"QGE", L"QGF", L"QGG", L"QGH", L"QGI", L"QGJ", L"QGK", L"QGL", L"QGM", L"QGN", L"QGO", L"QGP", L"QGQ", L"QGR", L"QGS", L"QGT", L"QGU", L"QGV", L"QGW", L"QGX", L"QGY", L"QGZ", L"QHA", L"QHB", L"QHC", L"QHD", L"QHE", L"QHF", L"QHG", L"QHH", L"QHI", L"QHJ", L"QHK", L"QHL", L"QHM", L"QHN", L"QHO", L"QHP", L"QHQ", L"QHR", L"QHS", L"QHT", L"QHU", L"QHV", L"QHW", L"QHX", L"QHY", L"QHZ", L"QIA", L"QIB", L"QIC", L"QID", L"QIE", L"QIF", L"QIG", L"QIH", L"QII", L"QIJ", L"QIK", L"QIL", L"QIM", L"QIN", L"QIO", L"QIP", L"QIQ", L"QIR", L"QIS", L"QIT", L"QIU", L"QIV", L"QIW", L"QIX", L"QIY", L"QIZ", L"QJA", L"QJB", L"QJC", L"QJD", L"QJE", L"QJF", L"QJG", L"QJH", L"QJI", L"QJJ", L"QJK", L"QJL", L"QJM", L"QJN", L"QJO", L"QJP", L"QJQ", L"QJR", L"QJS", L"QJT", L"QJU", L"QJV", L"QJW", L"QJX", L"QJY", L"QJZ", L"QKA", L"QKB", L"QKC", L"QKD", L"QKE", L"QKF", L"QKG", L"QKH",
			L"QKI", L"QKJ", L"QKK", L"QKL", L"QKM", L"QKN", L"QKO", L"QKP", L"QKQ", L"QKR", L"QKS", L"QKT", L"QKU", L"QKV", L"QKW", L"QKX", L"QKY", L"QKZ", L"QLA", L"QLB", L"QLC", L"QLD", L"QLE", L"QLF", L"QLG", L"QLH", L"QLI", L"QLJ", L"QLK", L"QLL", L"QLM", L"QLN", L"QLO", L"QLP", L"QLQ", L"QLR", L"QLS", L"QLT", L"QLU", L"QLV", L"QLW", L"QLX", L"QLY", L"QLZ", L"QMA", L"QMB", L"QMC", L"QMD", L"QME", L"QMF", L"QMG", L"QMH", L"QMI", L"QMJ", L"QMK", L"QML", L"QMM", L"QMN", L"QMO", L"QMP", L"QMQ", L"QMR", L"QMS", L"QMT", L"QMU", L"QMV", L"QMW", L"QMX", L"QMY", L"QMZ", L"QNA", L"QNB", L"QNC", L"QND", L"QNE", L"QNF", L"QNG", L"QNH", L"QNI", L"QNJ", L"QNK", L"QNL", L"QNM", L"QNN", L"QNO", L"QNP", L"QNQ", L"QNR", L"QNS", L"QNT", L"QNU", L"QNV", L"QNW", L"QNX", L"QNY", L"QNZ", L"QOA", L"QOB", L"QOC", L"QOD", L"QOE", L"QOF", L"QOG", L"QOH", L"QOI", L"QOJ", L"QOK", L"QOL", L"QOM", L"QON", L"QOO", L"QOP", L"QOQ", L"QOR", L"QOS", L"QOT", L"QOU", L"QOV", L"QOW", L"QOX", L"QOY", L"QOZ", L"QPA", L"QPB", L"QPC", L"QPD", L"QPE", L"QPF", L"QPG", L"QPH", L"QPI", L"QPJ", L"QPK", L"QPL", L"QPM", L"QPN", L"QPO", L"QPP", L"QPQ", L"QPR", L"QPS", L"QPT", L"QPU", L"QPV", L"QPW", L"QPX", L"QPY", L"QPZ", L"QQA", L"QQB", L"QQC", L"QQD", L"QQE", L"QQF", L"QQG", L"QQH", L"QQI", L"QQJ", L"QQK", L"QQL", L"QQM", L"QQN", L"QQO", L"QQP", L"QQQ", L"QQR", L"QQS", L"QQT", L"QQU", L"QQV", L"QQW", L"QQX", L"QQY", L"QQZ", L"QRA", L"QRB", L"QRC", L"QRD", L"QRE", L"QRF", L"QRG", L"QRH", L"QRI", L"QRJ", L"QRK", L"QRL", L"QRM", L"QRN", L"QRO", L"QRP", L"QRQ", L"QRR", L"QRS", L"QRT", L"QRU", L"QRV", L"QRW", L"QRX", L"QRY", L"QRZ", L"QSA", L"QSB", L"QSC", L"QSD", L"QSE", L"QSF", L"QSG", L"QSH", L"QSI", L"QSJ", L"QSK", L"QSL", L"QSM", L"QSN", L"QSO", L"QSP", L"QSQ", L"QSR", L"QSS", L"QST", L"QSU", L"QSV", L"QSW", L"QSX", L"QSY", L"QSZ", L"QTA", L"QTB", L"QTC", L"QTD", L"QTE", L"QTF", L"QTG", L"QTH", L"QTI", L"QTJ", L"QTK", L"QTL", L"QTM", L"QTN", L"QTO", L"QTP", L"QTQ", L"QTR", L"QTS", L"QTT", L"QTU", L"QTV", L"QTW", L"QTX", L"QTY", L"QTZ", L"QUA", L"QUB", L"QUC", L"QUD", L"QUE", L"QUF", L"QUG", L"QUH", L"QUI", L"QUJ", L"QUK", L"QUL", L"QUM", L"QUN", L"QUO", L"QUP", L"QUQ", L"QUR", L"QUS", L"QUT", L"QUU", 
			L"QUV", L"QUW", L"QUX", L"QUY", L"QUZ", L"QVA", L"QVB", L"QVC", L"QVD", L"QVE", L"QVF", L"QVG", L"QVH", L"QVI", L"QVJ", L"QVK", L"QVL", L"QVM", L"QVN", L"QVO", L"QVP", L"QVQ", L"QVR", L"QVS", L"QVT", L"QVU", L"QVV", L"QVW", L"QVX", L"QVY", L"QVZ", L"QWA", L"QWB", L"QWC", L"QWD", L"QWE", L"QWF", L"QWG", L"QWH", L"QWI", L"QWJ", L"QWK", L"QWL", L"QWM", L"QWN", L"QWO", L"QWP", L"QWQ", L"QWR", L"QWS", L"QWT", L"QWU", L"QWV", L"QWW", L"QWX", L"QWY", L"QWZ", L"QXA", L"QXB", L"QXC", L"QXD", L"QXE", L"QXF", L"QXG", L"QXH", L"QXI", L"QXJ", L"QXK", L"QXL", L"QXM", L"QXN", L"QXO", L"QXP", L"QXQ", L"QXR", L"QXS", L"QXT", L"QXU", L"QXV", L"QXW", L"QXX", L"QXY", L"QXZ", L"QYA", L"QYB", L"QYC", L"QYD", L"QYE", L"QYF", L"QYG", L"QYH", L"QYI", L"QYJ", L"QYK", L"QYL", L"QYM", L"QYN", L"QYO", L"QYP", L"QYQ", L"QYR", L"QYS", L"QYT", L"QYU", L"QYV", L"QYW", L"QYX", L"QYY", L"QYZ", L"QZA", L"QZB", L"QZC", L"QZD", L"QZE", L"QZF", L"QZG", L"QZH", L"QZI", L"QZJ", L"QZK", L"QZL", L"QZM", L"QZN", L"QZO", L"QZP", L"QZQ", L"QZR", L"QZS", L"QZT", L"QZU", L"QZV", L"QZW", L"QZX", L"QZY", L"QZZ", L"RAA", L"RAB", L"RAC", L"RAD", L"RAE", L"RAF", L"RAG", L"RAH", L"RAI", L"RAJ", L"RAK", L"RAL", L"RAM", L"RAN", L"RAO", L"RAP", L"RAQ", L"RAR", L"RAS", L"RAT", L"RAU", L"RAV", L"RAW", L"RAX", L"RAY", L"RAZ", L"RBA", L"RBB", L"RBC", L"RBD", L"RBE", L"RBF", L"RBG", L"RBH", L"RBI", L"RBJ", L"RBK", L"RBL", L"RBM", L"RBN", L"RBO", L"RBP", L"RBQ", L"RBR", L"RBS", L"RBT", L"RBU", L"RBV", L"RBW", L"RBX", L"RBY", L"RBZ", L"RCA", L"RCB", L"RCC", L"RCD", L"RCE", L"RCF", L"RCG", L"RCH", L"RCI", L"RCJ", L"RCK", L"RCL", L"RCM", L"RCN", L"RCO", L"RCP", L"RCQ", L"RCR", L"RCS", L"RCT", L"RCU", L"RCV", L"RCW", L"RCX", L"RCY", L"RCZ", L"RDA", L"RDB", L"RDC", L"RDD", L"RDE", L"RDF", L"RDG", L"RDH", L"RDI", L"RDJ", L"RDK", L"RDL", L"RDM", L"RDN", L"RDO", L"RDP", L"RDQ", L"RDR", L"RDS", L"RDT", L"RDU", L"RDV", L"RDW", L"RDX", L"RDY", L"RDZ", L"REA", L"REB", L"REC", L"RED", L"REE", L"REF", L"REG", L"REH", L"REI", L"REJ", L"REK", L"REL", L"REM", L"REN", L"REO", L"REP", L"REQ", L"RER", L"RES", L"RET", L"REU", L"REV", L"REW", L"REX", L"REY", L"REZ", L"RFA", L"RFB", L"RFC", L"RFD", L"RFE", L"RFF", L"RFG", L"RFH", 
			L"RFI", L"RFJ", L"RFK", L"RFL", L"RFM", L"RFN", L"RFO", L"RFP", L"RFQ", L"RFR", L"RFS", L"RFT", L"RFU", L"RFV", L"RFW", L"RFX", L"RFY", L"RFZ", L"RGA", L"RGB", L"RGC", L"RGD", L"RGE", L"RGF", L"RGG", L"RGH", L"RGI", L"RGJ", L"RGK", L"RGL", L"RGM", L"RGN", L"RGO", L"RGP", L"RGQ", L"RGR", L"RGS", L"RGT", L"RGU", L"RGV", L"RGW", L"RGX", L"RGY", L"RGZ", L"RHA", L"RHB", L"RHC", L"RHD", L"RHE", L"RHF", L"RHG", L"RHH", L"RHI", L"RHJ", L"RHK", L"RHL", L"RHM", L"RHN", L"RHO", L"RHP", L"RHQ", L"RHR", L"RHS", L"RHT", L"RHU", L"RHV", L"RHW", L"RHX", L"RHY", L"RHZ", L"RIA", L"RIB", L"RIC", L"RID", L"RIE", L"RIF", L"RIG", L"RIH", L"RII", L"RIJ", L"RIK", L"RIL", L"RIM", L"RIN", L"RIO", L"RIP", L"RIQ", L"RIR", L"RIS", L"RIT", L"RIU", L"RIV", L"RIW", L"RIX", L"RIY", L"RIZ", L"RJA", L"RJB", L"RJC", L"RJD", L"RJE", L"RJF", L"RJG", L"RJH", L"RJI", L"RJJ", L"RJK", L"RJL", L"RJM", L"RJN", L"RJO", L"RJP", L"RJQ", L"RJR", L"RJS", L"RJT", L"RJU", L"RJV", L"RJW", L"RJX", L"RJY", L"RJZ", L"RKA", L"RKB", L"RKC", L"RKD", L"RKE", L"RKF", L"RKG", L"RKH", L"RKI", L"RKJ", L"RKK", L"RKL", L"RKM", L"RKN", L"RKO", L"RKP", L"RKQ", L"RKR", L"RKS", L"RKT", L"RKU", L"RKV", L"RKW", L"RKX", L"RKY", L"RKZ", L"RLA", L"RLB", L"RLC", L"RLD", L"RLE", L"RLF", L"RLG", L"RLH", L"RLI", L"RLJ", L"RLK", L"RLL", L"RLM", L"RLN", L"RLO", L"RLP", L"RLQ", L"RLR", L"RLS", L"RLT", L"RLU", L"RLV", L"RLW", L"RLX", L"RLY", L"RLZ", L"RMA", L"RMB", L"RMC", L"RMD", L"RME", L"RMF", L"RMG", L"RMH", L"RMI", L"RMJ", L"RMK", L"RML", L"RMM", L"RMN", L"RMO", L"RMP", L"RMQ", L"RMR", L"RMS", L"RMT", L"RMU", L"RMV", L"RMW", L"RMX", L"RMY", L"RMZ", L"RNA", L"RNB", L"RNC", L"RND", L"RNE", L"RNF", L"RNG", L"RNH", L"RNI", L"RNJ", L"RNK", L"RNL", L"RNM", L"RNN", L"RNO", L"RNP", L"RNQ", L"RNR", L"RNS", L"RNT", L"RNU", L"RNV", L"RNW", L"RNX", L"RNY", L"RNZ", L"ROA", L"ROB", L"ROC", L"ROD", L"ROE", L"ROF", L"ROG", L"ROH", L"ROI", L"ROJ", L"ROK", L"ROL", L"ROM", L"RON", L"ROO", L"ROP", L"ROQ", L"ROR", L"ROS", L"ROT", L"ROU", L"ROV", L"ROW", L"ROX", L"ROY", L"ROZ", L"RPA", L"RPB", L"RPC", L"RPD", L"RPE", L"RPF", L"RPG", L"RPH", L"RPI", L"RPJ", L"RPK", L"RPL", L"RPM", L"RPN", L"RPO", L"RPP", L"RPQ", L"RPR", L"RPS", L"RPT", L"RPU", 
			L"RPV", L"RPW", L"RPX", L"RPY", L"RPZ", L"RQA", L"RQB", L"RQC", L"RQD", L"RQE", L"RQF", L"RQG", L"RQH", L"RQI", L"RQJ", L"RQK", L"RQL", L"RQM", L"RQN", L"RQO", L"RQP", L"RQQ", L"RQR", L"RQS", L"RQT", L"RQU", L"RQV", L"RQW", L"RQX", L"RQY", L"RQZ", L"RRA", L"RRB", L"RRC", L"RRD", L"RRE", L"RRF", L"RRG", L"RRH", L"RRI", L"RRJ", L"RRK", L"RRL", L"RRM", L"RRN", L"RRO", L"RRP", L"RRQ", L"RRR", L"RRS", L"RRT", L"RRU", L"RRV", L"RRW", L"RRX", L"RRY", L"RRZ", L"RSA", L"RSB", L"RSC", L"RSD", L"RSE", L"RSF", L"RSG", L"RSH", L"RSI", L"RSJ", L"RSK", L"RSL", L"RSM", L"RSN", L"RSO", L"RSP", L"RSQ", L"RSR", L"RSS", L"RST", L"RSU", L"RSV", L"RSW", L"RSX", L"RSY", L"RSZ", L"RTA", L"RTB", L"RTC", L"RTD", L"RTE", L"RTF", L"RTG", L"RTH", L"RTI", L"RTJ", L"RTK", L"RTL", L"RTM", L"RTN", L"RTO", L"RTP", L"RTQ", L"RTR", L"RTS", L"RTT", L"RTU", L"RTV", L"RTW", L"RTX", L"RTY", L"RTZ", L"RUA", L"RUB", L"RUC", L"RUD", L"RUE", L"RUF", L"RUG", L"RUH", L"RUI", L"RUJ", L"RUK", L"RUL", L"RUM", L"RUN", L"RUO", L"RUP", L"RUQ", L"RUR", L"RUS", L"RUT", L"RUU", L"RUV", L"RUW", L"RUX", L"RUY", L"RUZ", L"RVA", L"RVB", L"RVC", L"RVD", L"RVE", L"RVF", L"RVG", L"RVH", L"RVI", L"RVJ", L"RVK", L"RVL", L"RVM", L"RVN", L"RVO", L"RVP", L"RVQ", L"RVR", L"RVS", L"RVT", L"RVU", L"RVV", L"RVW", L"RVX", L"RVY", L"RVZ", L"RWA", L"RWB", L"RWC", L"RWD", L"RWE", L"RWF", L"RWG", L"RWH", L"RWI", L"RWJ", L"RWK", L"RWL", L"RWM", L"RWN", L"RWO", L"RWP", L"RWQ", L"RWR", L"RWS", L"RWT", L"RWU", L"RWV", L"RWW", L"RWX", L"RWY", L"RWZ", L"RXA", L"RXB", L"RXC", L"RXD", L"RXE", L"RXF", L"RXG", L"RXH", L"RXI", L"RXJ", L"RXK", L"RXL", L"RXM", L"RXN", L"RXO", L"RXP", L"RXQ", L"RXR", L"RXS", L"RXT", L"RXU", L"RXV", L"RXW", L"RXX", L"RXY", L"RXZ", L"RYA", L"RYB", L"RYC", L"RYD", L"RYE", L"RYF", L"RYG", L"RYH", L"RYI", L"RYJ", L"RYK", L"RYL", L"RYM", L"RYN", L"RYO", L"RYP", L"RYQ", L"RYR", L"RYS", L"RYT", L"RYU", L"RYV", L"RYW", L"RYX", L"RYY", L"RYZ", L"RZA", L"RZB", L"RZC", L"RZD", L"RZE", L"RZF", L"RZG", L"RZH", L"RZI", L"RZJ", L"RZK", L"RZL", L"RZM", L"RZN", L"RZO", L"RZP", L"RZQ", L"RZR", L"RZS", L"RZT", L"RZU", L"RZV", L"RZW", L"RZX", L"RZY", L"RZZ", L"SAA", L"SAB", L"SAC", L"SAD", L"SAE", L"SAF", L"SAG", L"SAH", 
			L"SAI", L"SAJ", L"SAK", L"SAL", L"SAM", L"SAN", L"SAO", L"SAP", L"SAQ", L"SAR", L"SAS", L"SAT", L"SAU", L"SAV", L"SAW", L"SAX", L"SAY", L"SAZ", L"SBA", L"SBB", L"SBC", L"SBD", L"SBE", L"SBF", L"SBG", L"SBH", L"SBI", L"SBJ", L"SBK", L"SBL", L"SBM", L"SBN", L"SBO", L"SBP", L"SBQ", L"SBR", L"SBS", L"SBT", L"SBU", L"SBV", L"SBW", L"SBX", L"SBY", L"SBZ", L"SCA", L"SCB", L"SCC", L"SCD", L"SCE", L"SCF", L"SCG", L"SCH", L"SCI", L"SCJ", L"SCK", L"SCL", L"SCM", L"SCN", L"SCO", L"SCP", L"SCQ", L"SCR", L"SCS", L"SCT", L"SCU", L"SCV", L"SCW", L"SCX", L"SCY", L"SCZ", L"SDA", L"SDB", L"SDC", L"SDD", L"SDE", L"SDF", L"SDG", L"SDH", L"SDI", L"SDJ", L"SDK", L"SDL", L"SDM", L"SDN", L"SDO", L"SDP", L"SDQ", L"SDR", L"SDS", L"SDT", L"SDU", L"SDV", L"SDW", L"SDX", L"SDY", L"SDZ", L"SEA", L"SEB", L"SEC", L"SED", L"SEE", L"SEF", L"SEG", L"SEH", L"SEI", L"SEJ", L"SEK", L"SEL", L"SEM", L"SEN", L"SEO", L"SEP", L"SEQ", L"SER", L"SES", L"SET", L"SEU", L"SEV", L"SEW", L"SEX", L"SEY", L"SEZ", L"SFA", L"SFB", L"SFC", L"SFD", L"SFE", L"SFF", L"SFG", L"SFH", L"SFI", L"SFJ", L"SFK", L"SFL", L"SFM", L"SFN", L"SFO", L"SFP", L"SFQ", L"SFR", L"SFS", L"SFT", L"SFU", L"SFV", L"SFW", L"SFX", L"SFY", L"SFZ", L"SGA", L"SGB", L"SGC", L"SGD", L"SGE", L"SGF", L"SGG", L"SGH", L"SGI", L"SGJ", L"SGK", L"SGL", L"SGM", L"SGN", L"SGO", L"SGP", L"SGQ", L"SGR", L"SGS", L"SGT", L"SGU", L"SGV", L"SGW", L"SGX", L"SGY", L"SGZ", L"SHA", L"SHB", L"SHC", L"SHD", L"SHE", L"SHF", L"SHG", L"SHH", L"SHI", L"SHJ", L"SHK", L"SHL", L"SHM", L"SHN", L"SHO", L"SHP", L"SHQ", L"SHR", L"SHS", L"SHT", L"SHU", L"SHV", L"SHW", L"SHX", L"SHY", L"SHZ", L"SIA", L"SIB", L"SIC", L"SID", L"SIE", L"SIF", L"SIG", L"SIH", L"SII", L"SIJ", L"SIK", L"SIL", L"SIM", L"SIN", L"SIO", L"SIP", L"SIQ", L"SIR", L"SIS", L"SIT", L"SIU", L"SIV", L"SIW", L"SIX", L"SIY", L"SIZ", L"SJA", L"SJB", L"SJC", L"SJD", L"SJE", L"SJF", L"SJG", L"SJH", L"SJI", L"SJJ", L"SJK", L"SJL", L"SJM", L"SJN", L"SJO", L"SJP", L"SJQ", L"SJR", L"SJS", L"SJT", L"SJU", L"SJV", L"SJW", L"SJX", L"SJY", L"SJZ", L"SKA", L"SKB", L"SKC", L"SKD", L"SKE", L"SKF", L"SKG", L"SKH", L"SKI", L"SKJ", L"SKK", L"SKL", L"SKM", L"SKN", L"SKO", L"SKP", L"SKQ", L"SKR", L"SKS", L"SKT", L"SKU", 
			L"SKV", L"SKW", L"SKX", L"SKY", L"SKZ", L"SLA", L"SLB", L"SLC", L"SLD", L"SLE", L"SLF", L"SLG", L"SLH", L"SLI", L"SLJ", L"SLK", L"SLL", L"SLM", L"SLN", L"SLO", L"SLP", L"SLQ", L"SLR", L"SLS", L"SLT", L"SLU", L"SLV", L"SLW", L"SLX", L"SLY", L"SLZ", L"SMA", L"SMB", L"SMC", L"SMD", L"SME", L"SMF", L"SMG", L"SMH", L"SMI", L"SMJ", L"SMK", L"SML", L"SMM", L"SMN", L"SMO", L"SMP", L"SMQ", L"SMR", L"SMS", L"SMT", L"SMU", L"SMV", L"SMW", L"SMX", L"SMY", L"SMZ", L"SNA", L"SNB", L"SNC", L"SND", L"SNE", L"SNF", L"SNG", L"SNH", L"SNI", L"SNJ", L"SNK", L"SNL", L"SNM", L"SNN", L"SNO", L"SNP", L"SNQ", L"SNR", L"SNS", L"SNT", L"SNU", L"SNV", L"SNW", L"SNX", L"SNY", L"SNZ", L"SOA", L"SOB", L"SOC", L"SOD", L"SOE", L"SOF", L"SOG", L"SOH", L"SOI", L"SOJ", L"SOK", L"SOL", L"SOM", L"SON", L"SOO", L"SOP", L"SOQ", L"SOR", L"SOS", L"SOT", L"SOU", L"SOV", L"SOW", L"SOX", L"SOY", L"SOZ", L"SPA", L"SPB", L"SPC", L"SPD", L"SPE", L"SPF", L"SPG", L"SPH", L"SPI", L"SPJ", L"SPK", L"SPL", L"SPM", L"SPN", L"SPO", L"SPP", L"SPQ", L"SPR", L"SPS", L"SPT", L"SPU", L"SPV", L"SPW", L"SPX", L"SPY", L"SPZ", L"SQA", L"SQB", L"SQC", L"SQD", L"SQE", L"SQF", L"SQG", L"SQH", L"SQI", L"SQJ", L"SQK", L"SQL", L"SQM", L"SQN", L"SQO", L"SQP", L"SQQ", L"SQR", L"SQS", L"SQT", L"SQU", L"SQV", L"SQW", L"SQX", L"SQY", L"SQZ", L"SRA", L"SRB", L"SRC", L"SRD", L"SRE", L"SRF", L"SRG", L"SRH", L"SRI", L"SRJ", L"SRK", L"SRL", L"SRM", L"SRN", L"SRO", L"SRP", L"SRQ", L"SRR", L"SRS", L"SRT", L"SRU", L"SRV", L"SRW", L"SRX", L"SRY", L"SRZ", L"SSA", L"SSB", L"SSC", L"SSD", L"SSE", L"SSF", L"SSG", L"SSH", L"SSI", L"SSJ", L"SSK", L"SSL", L"SSM", L"SSN", L"SSO", L"SSP", L"SSQ", L"SSR", L"SSS", L"SST", L"SSU", L"SSV", L"SSW", L"SSX", L"SSY", L"SSZ", L"STA", L"STB", L"STC", L"STD", L"STE", L"STF", L"STG", L"STH", L"STI", L"STJ", L"STK", L"STL", L"STM", L"STN", L"STO", L"STP", L"STQ", L"STR", L"STS", L"STT", L"STU", L"STV", L"STW", L"STX", L"STY", L"STZ", L"SUA", L"SUB", L"SUC", L"SUD", L"SUE", L"SUF", L"SUG", L"SUH", L"SUI", L"SUJ", L"SUK", L"SUL", L"SUM", L"SUN", L"SUO", L"SUP", L"SUQ", L"SUR", L"SUS", L"SUT", L"SUU", L"SUV", L"SUW", L"SUX", L"SUY", 
			L"SUZ", L"SVA", L"SVB", L"SVC", L"SVD", L"SVE", L"SVF", L"SVG", L"SVH", L"SVI", L"SVJ", L"SVK", L"SVL", L"SVM", L"SVN", L"SVO", L"SVP", L"SVQ", L"SVR", L"SVS", L"SVT", L"SVU", L"SVV", L"SVW", L"SVX", L"SVY", L"SVZ", L"SWA", L"SWB", L"SWC", L"SWD", L"SWE", L"SWF", L"SWG", L"SWH", L"SWI", L"SWJ", L"SWK", L"SWL", L"SWM", L"SWN", L"SWO", L"SWP", L"SWQ", L"SWR", L"SWS", L"SWT", L"SWU", L"SWV", L"SWW", L"SWX", L"SWY", L"SWZ", L"SXA", L"SXB", L"SXC", L"SXD", L"SXE", L"SXF", L"SXG", L"SXH", L"SXI", L"SXJ", L"SXK", L"SXL", L"SXM", L"SXN", L"SXO", L"SXP", L"SXQ", L"SXR", L"SXS", L"SXT", L"SXU", L"SXV", L"SXW", L"SXX", L"SXY", L"SXZ", L"SYA", L"SYB", L"SYC", L"SYD", L"SYE", L"SYF", L"SYG", L"SYH", L"SYI", L"SYJ", L"SYK", L"SYL", L"SYM", L"SYN", L"SYO", L"SYP", L"SYQ", L"SYR", L"SYS", L"SYT", L"SYU", L"SYV", L"SYW", L"SYX", L"SYY", L"SYZ", L"SZA", L"SZB", L"SZC", L"SZD", L"SZE", L"SZF", L"SZG", L"SZH", L"SZI", L"SZJ", L"SZK", L"SZL", L"SZM", L"SZN", L"SZO", L"SZP", L"SZQ", L"SZR", L"SZS", L"SZT", L"SZU", L"SZV", L"SZW", L"SZX", L"SZY", L"SZZ", L"TAA", L"TAB", L"TAC", L"TAD", L"TAE", L"TAF", L"TAG", L"TAH", L"TAI", L"TAJ", L"TAK", L"TAL", L"TAM", L"TAN", L"TAO", L"TAP", L"TAQ", L"TAR", L"TAS", L"TAT", L"TAU", L"TAV", L"TAW", L"TAX", L"TAY", L"TAZ", L"TBA", L"TBB", L"TBC", L"TBD", L"TBE", L"TBF", L"TBG", L"TBH", L"TBI", L"TBJ", L"TBK", L"TBL", L"TBM", L"TBN", L"TBO", L"TBP", L"TBQ", L"TBR", L"TBS", L"TBT", L"TBU", L"TBV", L"TBW", L"TBX", L"TBY", L"TBZ", L"TCA", L"TCB", L"TCC", L"TCD", L"TCE", L"TCF", L"TCG", L"TCH", L"TCI", L"TCJ", L"TCK", L"TCL", L"TCM", L"TCN", L"TCO", L"TCP", L"TCQ", L"TCR", L"TCS", L"TCT", L"TCU", L"TCV", L"TCW", L"TCX", L"TCY", L"TCZ", L"TDA", L"TDB", L"TDC", L"TDD", L"TDE", L"TDF", L"TDG", L"TDH", L"TDI", L"TDJ", L"TDK", L"TDL", L"TDM", L"TDN", L"TDO", L"TDP", L"TDQ", L"TDR", L"TDS", L"TDT", L"TDU", L"TDV", L"TDW", L"TDX", L"TDY", L"TDZ", L"TEA", L"TEB", L"TEC", L"TED", L"TEE", L"TEF", L"TEG", L"TEH", L"TEI", L"TEJ", L"TEK", L"TEL", L"TEM", L"TEN", L"TEO", L"TEP", L"TEQ", L"TER", L"TES", L"TET", L"TEU", L"TEV", L"TEW", L"TEX", L"TEY", L"TEZ", L"TFA", L"TFB", L"TFC", L"TFD", L"TFE", L"TFF", L"TFG", L"TFH", L"TFI", L"TFJ", L"TFK", L"TFL", 
			L"TFM", L"TFN", L"TFO", L"TFP", L"TFQ", L"TFR", L"TFS", L"TFT", L"TFU", L"TFV", L"TFW", L"TFX", L"TFY", L"TFZ", L"TGA", L"TGB", L"TGC", L"TGD", L"TGE", L"TGF", L"TGG", L"TGH", L"TGI", L"TGJ", L"TGK", L"TGL", L"TGM", L"TGN", L"TGO", L"TGP", L"TGQ", L"TGR", L"TGS", L"TGT", L"TGU", L"TGV", L"TGW", L"TGX", L"TGY", L"TGZ", L"THA", L"THB", L"THC", L"THD", L"THE", L"THF", L"THG", L"THH", L"THI", L"THJ", L"THK", L"THL", L"THM", L"THN", L"THO", L"THP", L"THQ", L"THR", L"THS", L"THT", L"THU", L"THV", L"THW", L"THX", L"THY", L"THZ", L"TIA", L"TIB", L"TIC", L"TID", L"TIE", L"TIF", L"TIG", L"TIH", L"TII", L"TIJ", L"TIK", L"TIL", L"TIM", L"TIN", L"TIO", L"TIP", L"TIQ", L"TIR", L"TIS", L"TIT", L"TIU", L"TIV", L"TIW", L"TIX", L"TIY", L"TIZ", L"TJA", L"TJB", L"TJC", L"TJD", L"TJE", L"TJF", L"TJG", L"TJH", L"TJI", L"TJJ", L"TJK", L"TJL", L"TJM", L"TJN", L"TJO", L"TJP", L"TJQ", L"TJR", L"TJS", L"TJT", L"TJU", L"TJV", L"TJW", L"TJX", L"TJY", L"TJZ", L"TKA", L"TKB", L"TKC", L"TKD", L"TKE", L"TKF", L"TKG", L"TKH", L"TKI", L"TKJ", L"TKK", L"TKL", L"TKM", L"TKN", L"TKO", L"TKP", L"TKQ", L"TKR", L"TKS", L"TKT", L"TKU", L"TKV", L"TKW", L"TKX", L"TKY", L"TKZ", L"TLA", L"TLB", L"TLC", L"TLD", L"TLE", L"TLF", L"TLG", L"TLH", L"TLI", L"TLJ", L"TLK", L"TLL", L"TLM", L"TLN", L"TLO", L"TLP", L"TLQ", L"TLR", L"TLS", L"TLT", L"TLU", L"TLV", L"TLW", L"TLX", L"TLY", L"TLZ", L"TMA", L"TMB", L"TMC", L"TMD", L"TME", L"TMF", L"TMG", L"TMH", L"TMI", L"TMJ", L"TMK", L"TML", L"TMM", L"TMN", L"TMO", L"TMP", L"TMQ", L"TMR", L"TMS", L"TMT", L"TMU", L"TMV", L"TMW", L"TMX", L"TMY", L"TMZ", L"TNA", L"TNB", L"TNC", L"TND", L"TNE", L"TNF", L"TNG", L"TNH", L"TNI", L"TNJ", L"TNK", L"TNL", L"TNM", L"TNN", L"TNO", L"TNP", L"TNQ", L"TNR", L"TNS", L"TNT", L"TNU", L"TNV", L"TNW", L"TNX", L"TNY", L"TNZ", L"TOA", L"TOB", L"TOC", L"TOD", L"TOE", L"TOF", L"TOG", L"TOH", L"TOI", L"TOJ", L"TOK", L"TOL", L"TOM", L"TON", L"TOO", L"TOP", L"TOQ", L"TOR", L"TOS", L"TOT", L"TOU", L"TOV", L"TOW", L"TOX", L"TOY", L"TOZ", L"TPA", L"TPB", L"TPC", L"TPD", L"TPE", L"TPF", L"TPG", L"TPH", L"TPI", L"TPJ", L"TPK", L"TPL", L"TPM", L"TPN", L"TPO", L"TPP", L"TPQ", L"TPR", L"TPS", L"TPT", L"TPU", L"TPV", L"TPW", L"TPX", L"TPY", L"TPZ",
			L"TQA", L"TQB", L"TQC", L"TQD", L"TQE", L"TQF", L"TQG", L"TQH", L"TQI", L"TQJ", L"TQK", L"TQL", L"TQM", L"TQN", L"TQO", L"TQP", L"TQQ", L"TQR", L"TQS", L"TQT", L"TQU", L"TQV", L"TQW", L"TQX", L"TQY", L"TQZ", L"TRA", L"TRB", L"TRC", L"TRD", L"TRE", L"TRF", L"TRG", L"TRH", L"TRI", L"TRJ", L"TRK", L"TRL", L"TRM", L"TRN", L"TRO", L"TRP", L"TRQ", L"TRR", L"TRS", L"TRT", L"TRU", L"TRV", L"TRW", L"TRX", L"TRY", L"TRZ", L"TSA", L"TSB", L"TSC", L"TSD", L"TSE", L"TSF", L"TSG", L"TSH", L"TSI", L"TSJ", L"TSK", L"TSL", L"TSM", L"TSN", L"TSO", L"TSP", L"TSQ", L"TSR", L"TSS", L"TST", L"TSU", L"TSV", L"TSW", L"TSX", L"TSY", L"TSZ", L"TTA", L"TTB", L"TTC", L"TTD", L"TTE", L"TTF", L"TTG", L"TTH", L"TTI", L"TTJ", L"TTK", L"TTL", L"TTM", L"TTN", L"TTO", L"TTP", L"TTQ", L"TTR", L"TTS", L"TTT", L"TTU", L"TTV", L"TTW", L"TTX", L"TTY", L"TTZ", L"TUA", L"TUB", L"TUC", L"TUD", L"TUE", L"TUF", L"TUG", L"TUH", L"TUI", L"TUJ", L"TUK", L"TUL", L"TUM", L"TUN", L"TUO", L"TUP", L"TUQ", L"TUR", L"TUS", L"TUT", L"TUU", L"TUV", L"TUW", L"TUX", L"TUY", L"TUZ", L"TVA", L"TVB", L"TVC", L"TVD", L"TVE", L"TVF", L"TVG", L"TVH", L"TVI", L"TVJ", L"TVK", L"TVL", L"TVM", L"TVN", L"TVO", L"TVP", L"TVQ", L"TVR", L"TVS", L"TVT", L"TVU", L"TVV", L"TVW", L"TVX", L"TVY", L"TVZ", L"TWA", L"TWB", L"TWC", L"TWD", L"TWE", L"TWF", L"TWG", L"TWH", L"TWI", L"TWJ", L"TWK", L"TWL", L"TWM", L"TWN", L"TWO", L"TWP", L"TWQ", L"TWR", L"TWS", L"TWT", L"TWU", L"TWV", L"TWW", L"TWX", L"TWY", L"TWZ", L"TXA", L"TXB", L"TXC", L"TXD", L"TXE", L"TXF", L"TXG", L"TXH", L"TXI", L"TXJ", L"TXK", L"TXL", L"TXM", L"TXN", L"TXO", L"TXP", L"TXQ", L"TXR", L"TXS", L"TXT", L"TXU", L"TXV", L"TXW", L"TXX", L"TXY", L"TXZ", L"TYA", L"TYB", L"TYC", L"TYD", L"TYE", L"TYF", L"TYG", L"TYH", L"TYI", L"TYJ", L"TYK", L"TYL", L"TYM", L"TYN", L"TYO", L"TYP", L"TYQ", L"TYR", L"TYS", L"TYT", L"TYU", L"TYV", L"TYW", L"TYX", L"TYY", L"TYZ", L"TZA", L"TZB", L"TZC", L"TZD", L"TZE", L"TZF", L"TZG", L"TZH", L"TZI", L"TZJ", L"TZK", L"TZL", L"TZM", L"TZN", L"TZO", L"TZP", L"TZQ", L"TZR", L"TZS", L"TZT", L"TZU", L"TZV", L"TZW", L"TZX", L"TZY", L"TZZ", L"UAA", L"UAB", L"UAC", L"UAD", L"UAE", L"UAF", L"UAG", L"UAH", L"UAI", L"UAJ", L"UAK", L"UAL", L"UAM", 
			L"UAN", L"UAO", L"UAP", L"UAQ", L"UAR", L"UAS", L"UAT", L"UAU", L"UAV", L"UAW", L"UAX", L"UAY", L"UAZ", L"UBA", L"UBB", L"UBC", L"UBD", L"UBE", L"UBF", L"UBG", L"UBH", L"UBI", L"UBJ", L"UBK", L"UBL", L"UBM", L"UBN", L"UBO", L"UBP", L"UBQ", L"UBR", L"UBS", L"UBT", L"UBU", L"UBV", L"UBW", L"UBX", L"UBY", L"UBZ", L"UCA", L"UCB", L"UCC", L"UCD", L"UCE", L"UCF", L"UCG", L"UCH", L"UCI", L"UCJ", L"UCK", L"UCL", L"UCM", L"UCN", L"UCO", L"UCP", L"UCQ", L"UCR", L"UCS", L"UCT", L"UCU", L"UCV", L"UCW", L"UCX", L"UCY", L"UCZ", L"UDA", L"UDB", L"UDC", L"UDD", L"UDE", L"UDF", L"UDG", L"UDH", L"UDI", L"UDJ", L"UDK", L"UDL", L"UDM", L"UDN", L"UDO", L"UDP", L"UDQ", L"UDR", L"UDS", L"UDT", L"UDU", L"UDV", L"UDW", L"UDX", L"UDY", L"UDZ", L"UEA", L"UEB", L"UEC", L"UED", L"UEE", L"UEF", L"UEG", L"UEH", L"UEI", L"UEJ", L"UEK", L"UEL", L"UEM", L"UEN", L"UEO", L"UEP", L"UEQ", L"UER", L"UES", L"UET", L"UEU", L"UEV", L"UEW", L"UEX", L"UEY", L"UEZ", L"UFA", L"UFB", L"UFC", L"UFD", L"UFE", L"UFF", L"UFG", L"UFH", L"UFI", L"UFJ", L"UFK", L"UFL", L"UFM", L"UFN", L"UFO", L"UFP", L"UFQ", L"UFR", L"UFS", L"UFT", L"UFU", L"UFV", L"UFW", L"UFX", L"UFY", L"UFZ", L"UGA", L"UGB", L"UGC", L"UGD", L"UGE", L"UGF", L"UGG", L"UGH", L"UGI", L"UGJ", L"UGK", L"UGL", L"UGM", L"UGN", L"UGO", L"UGP", L"UGQ", L"UGR", L"UGS", L"UGT", L"UGU", L"UGV", L"UGW", L"UGX", L"UGY", L"UGZ", L"UHA", L"UHB", L"UHC", L"UHD", L"UHE", L"UHF", L"UHG", L"UHH", L"UHI", L"UHJ", L"UHK", L"UHL", L"UHM", L"UHN", L"UHO", L"UHP", L"UHQ", L"UHR", L"UHS", L"UHT", L"UHU", L"UHV", L"UHW", L"UHX", L"UHY", L"UHZ", L"UIA", L"UIB", L"UIC", L"UID", L"UIE", L"UIF", L"UIG", L"UIH", L"UII", L"UIJ", L"UIK", L"UIL", L"UIM", L"UIN", L"UIO", L"UIP", L"UIQ", L"UIR", L"UIS", L"UIT", L"UIU", L"UIV", L"UIW", L"UIX", L"UIY", L"UIZ", L"UJA", L"UJB", L"UJC", L"UJD", L"UJE", L"UJF", L"UJG", L"UJH", L"UJI", L"UJJ", L"UJK", L"UJL", L"UJM", L"UJN", L"UJO", L"UJP", L"UJQ", L"UJR", L"UJS", L"UJT", L"UJU", L"UJV", L"UJW", L"UJX", L"UJY", L"UJZ", L"UKA", L"UKB", L"UKC", L"UKD", L"UKE", L"UKF", L"UKG", L"UKH", L"UKI", L"UKJ", L"UKK", L"UKL", L"UKM", L"UKN", L"UKO", L"UKP", L"UKQ", L"UKR", L"UKS", L"UKT", L"UKU", L"UKV", L"UKW", L"UKX", L"UKY", L"UKZ", 
			L"ULA", L"ULB", L"ULC", L"ULD", L"ULE", L"ULF", L"ULG", L"ULH", L"ULI", L"ULJ", L"ULK", L"ULL", L"ULM", L"ULN", L"ULO", L"ULP", L"ULQ", L"ULR", L"ULS", L"ULT", L"ULU", L"ULV", L"ULW", L"ULX", L"ULY", L"ULZ", L"UMA", L"UMB", L"UMC", L"UMD", L"UME", L"UMF", L"UMG", L"UMH", L"UMI", L"UMJ", L"UMK", L"UML", L"UMM", L"UMN", L"UMO", L"UMP", L"UMQ", L"UMR", L"UMS", L"UMT", L"UMU", L"UMV", L"UMW", L"UMX", L"UMY", L"UMZ", L"UNA", L"UNB", L"UNC", L"UND", L"UNE", L"UNF", L"UNG", L"UNH", L"UNI", L"UNJ", L"UNK", L"UNL", L"UNM", L"UNN", L"UNO", L"UNP", L"UNQ", L"UNR", L"UNS", L"UNT", L"UNU", L"UNV", L"UNW", L"UNX", L"UNY", L"UNZ", L"UOA", L"UOB", L"UOC", L"UOD", L"UOE", L"UOF", L"UOG", L"UOH", L"UOI", L"UOJ", L"UOK", L"UOL", L"UOM", L"UON", L"UOO", L"UOP", L"UOQ", L"UOR", L"UOS", L"UOT", L"UOU", L"UOV", L"UOW", L"UOX", L"UOY", L"UOZ", L"UPA", L"UPB", L"UPC", L"UPD", L"UPE", L"UPF", L"UPG", L"UPH", L"UPI", L"UPJ", L"UPK", L"UPL", L"UPM", L"UPN", L"UPO", L"UPP", L"UPQ", L"UPR", L"UPS", L"UPT", L"UPU", L"UPV", L"UPW", L"UPX", L"UPY", L"UPZ", L"UQA", L"UQB", L"UQC", L"UQD", L"UQE", L"UQF", L"UQG", L"UQH", L"UQI", L"UQJ", L"UQK", L"UQL", L"UQM", L"UQN", L"UQO", L"UQP", L"UQQ", L"UQR", L"UQS", L"UQT", L"UQU", L"UQV", L"UQW", L"UQX", L"UQY", L"UQZ", L"URA", L"URB", L"URC", L"URD", L"URE", L"URF", L"URG", L"URH", L"URI", L"URJ", L"URK", L"URL", L"URM", L"URN", L"URO", L"URP", L"URQ", L"URR", L"URS", L"URT", L"URU", L"URV", L"URW", L"URX", L"URY", L"URZ", L"USA", L"USB", L"USC", L"USD", L"USE", L"USF", L"USG", L"USH", L"USI", L"USJ", L"USK", L"USL", L"USM", L"USN", L"USO", L"USP", L"USQ", L"USR", L"USS", L"UST", L"USU", L"USV", L"USW", L"USX", L"USY", L"USZ", L"UTA", L"UTB", L"UTC", L"UTD", L"UTE", L"UTF", L"UTG", L"UTH", L"UTI", L"UTJ", L"UTK", L"UTL", L"UTM", L"UTN", L"UTO", L"UTP", L"UTQ", L"UTR", L"UTS", L"UTT", L"UTU", L"UTV", L"UTW", L"UTX", L"UTY", L"UTZ", L"UUA", L"UUB", L"UUC", L"UUD", L"UUE", L"UUF", L"UUG", L"UUH", L"UUI", L"UUJ", L"UUK", L"UUL", L"UUM", L"UUN", L"UUO", L"UUP", L"UUQ", L"UUR", L"UUS", L"UUT", L"UUU", L"UUV", L"UUW", L"UUX", L"UUY", L"UUZ", L"UVA", L"UVB", L"UVC", L"UVD", L"UVE", L"UVF", L"UVG", L"UVH", L"UVI", L"UVJ", L"UVK", L"UVL", L"UVM", 
			L"UVN", L"UVO", L"UVP", L"UVQ", L"UVR", L"UVS", L"UVT", L"UVU", L"UVV", L"UVW", L"UVX", L"UVY", L"UVZ", L"UWA", L"UWB", L"UWC", L"UWD", L"UWE", L"UWF", L"UWG", L"UWH", L"UWI", L"UWJ", L"UWK", L"UWL", L"UWM", L"UWN", L"UWO", L"UWP", L"UWQ", L"UWR", L"UWS", L"UWT", L"UWU", L"UWV", L"UWW", L"UWX", L"UWY", L"UWZ", L"UXA", L"UXB", L"UXC", L"UXD", L"UXE", L"UXF", L"UXG", L"UXH", L"UXI", L"UXJ", L"UXK", L"UXL", L"UXM", L"UXN", L"UXO", L"UXP", L"UXQ", L"UXR", L"UXS", L"UXT", L"UXU", L"UXV", L"UXW", L"UXX", L"UXY", L"UXZ", L"UYA", L"UYB", L"UYC", L"UYD", L"UYE", L"UYF", L"UYG", L"UYH", L"UYI", L"UYJ", L"UYK", L"UYL", L"UYM", L"UYN", L"UYO", L"UYP", L"UYQ", L"UYR", L"UYS", L"UYT", L"UYU", L"UYV", L"UYW", L"UYX", L"UYY", L"UYZ", L"UZA", L"UZB", L"UZC", L"UZD", L"UZE", L"UZF", L"UZG", L"UZH", L"UZI", L"UZJ", L"UZK", L"UZL", L"UZM", L"UZN", L"UZO", L"UZP", L"UZQ", L"UZR", L"UZS", L"UZT", L"UZU", L"UZV", L"UZW", L"UZX", L"UZY", L"UZZ", L"VAA", L"VAB", L"VAC", L"VAD", L"VAE", L"VAF", L"VAG", L"VAH", L"VAI", L"VAJ", L"VAK", L"VAL", L"VAM", L"VAN", L"VAO", L"VAP", L"VAQ", L"VAR", L"VAS", L"VAT", L"VAU", L"VAV", L"VAW", L"VAX", L"VAY", L"VAZ", L"VBA", L"VBB", L"VBC", L"VBD", L"VBE", L"VBF", L"VBG", L"VBH", L"VBI", L"VBJ", L"VBK", L"VBL", L"VBM", L"VBN", L"VBO", L"VBP", L"VBQ", L"VBR", L"VBS", L"VBT", L"VBU", L"VBV", L"VBW", L"VBX", L"VBY", L"VBZ", L"VCA", L"VCB", L"VCC", L"VCD", L"VCE", L"VCF", L"VCG", L"VCH", L"VCI", L"VCJ", L"VCK", L"VCL", L"VCM", L"VCN", L"VCO", L"VCP", L"VCQ", L"VCR", L"VCS", L"VCT", L"VCU", L"VCV", L"VCW", L"VCX", L"VCY", L"VCZ", L"VDA", L"VDB", L"VDC", L"VDD", L"VDE", L"VDF", L"VDG", L"VDH", L"VDI", L"VDJ", L"VDK", L"VDL", L"VDM", L"VDN", L"VDO", L"VDP", L"VDQ", L"VDR", L"VDS", L"VDT", L"VDU", L"VDV", L"VDW", L"VDX", L"VDY", L"VDZ", L"VEA", L"VEB", L"VEC", L"VED", L"VEE", L"VEF", L"VEG", L"VEH", L"VEI", L"VEJ", L"VEK", L"VEL", L"VEM", L"VEN", L"VEO", L"VEP", L"VEQ", L"VER", L"VES", L"VET", L"VEU", L"VEV", L"VEW", L"VEX", L"VEY", L"VEZ", L"VFA", L"VFB", L"VFC", L"VFD", L"VFE", L"VFF", L"VFG", L"VFH", L"VFI", L"VFJ", L"VFK", L"VFL", L"VFM", L"VFN", L"VFO", L"VFP", L"VFQ", L"VFR", L"VFS", L"VFT", L"VFU", L"VFV", L"VFW", L"VFX", L"VFY", L"VFZ", 
			L"VGA", L"VGB", L"VGC", L"VGD", L"VGE", L"VGF", L"VGG", L"VGH", L"VGI", L"VGJ", L"VGK", L"VGL", L"VGM", L"VGN", L"VGO", L"VGP", L"VGQ", L"VGR", L"VGS", L"VGT", L"VGU", L"VGV", L"VGW", L"VGX", L"VGY", L"VGZ", L"VHA", L"VHB", L"VHC", L"VHD", L"VHE", L"VHF", L"VHG", L"VHH", L"VHI", L"VHJ", L"VHK", L"VHL", L"VHM", L"VHN", L"VHO", L"VHP", L"VHQ", L"VHR", L"VHS", L"VHT", L"VHU", L"VHV", L"VHW", L"VHX", L"VHY", L"VHZ", L"VIA", L"VIB", L"VIC", L"VID", L"VIE", L"VIF", L"VIG", L"VIH", L"VII", L"VIJ", L"VIK", L"VIL", L"VIM", L"VIN", L"VIO", L"VIP", L"VIQ", L"VIR", L"VIS", L"VIT", L"VIU", L"VIV", L"VIW", L"VIX", L"VIY", L"VIZ", L"VJA", L"VJB", L"VJC", L"VJD", L"VJE", L"VJF", L"VJG", L"VJH", L"VJI", L"VJJ", L"VJK", L"VJL", L"VJM", L"VJN", L"VJO", L"VJP", L"VJQ", L"VJR", L"VJS", L"VJT", L"VJU", L"VJV", L"VJW", L"VJX", L"VJY", L"VJZ", L"VKA", L"VKB", L"VKC", L"VKD", L"VKE", L"VKF", L"VKG", L"VKH", L"VKI", L"VKJ", L"VKK", L"VKL", L"VKM", L"VKN", L"VKO", L"VKP", L"VKQ", L"VKR", L"VKS", L"VKT", L"VKU", L"VKV", L"VKW", L"VKX", L"VKY", L"VKZ", L"VLA", L"VLB", L"VLC", L"VLD", L"VLE", L"VLF", L"VLG", L"VLH", L"VLI", L"VLJ", L"VLK", L"VLL", L"VLM", L"VLN", L"VLO", L"VLP", L"VLQ", L"VLR", L"VLS", L"VLT", L"VLU", L"VLV", L"VLW", L"VLX", L"VLY", L"VLZ", L"VMA", L"VMB", L"VMC", L"VMD", L"VME", L"VMF", L"VMG", L"VMH", L"VMI", L"VMJ", L"VMK", L"VML", L"VMM", L"VMN", L"VMO", L"VMP", L"VMQ", L"VMR", L"VMS", L"VMT", L"VMU", L"VMV", L"VMW", L"VMX", L"VMY", L"VMZ", L"VNA", L"VNB", L"VNC", L"VND", L"VNE", L"VNF", L"VNG", L"VNH", L"VNI", L"VNJ", L"VNK", L"VNL", L"VNM", L"VNN", L"VNO", L"VNP", L"VNQ", L"VNR", L"VNS", L"VNT", L"VNU", L"VNV", L"VNW", L"VNX", L"VNY", L"VNZ", L"VOA", L"VOB", L"VOC", L"VOD", L"VOE", L"VOF", L"VOG", L"VOH", L"VOI", L"VOJ", L"VOK", L"VOL", L"VOM", L"VON", L"VOO", L"VOP", L"VOQ", L"VOR", L"VOS", L"VOT", L"VOU", L"VOV", L"VOW", L"VOX", L"VOY", L"VOZ", L"VPA", L"VPB", L"VPC", L"VPD", L"VPE", L"VPF", L"VPG", L"VPH", L"VPI", L"VPJ", L"VPK", L"VPL", L"VPM", L"VPN", L"VPO", L"VPP", L"VPQ", L"VPR", L"VPS", L"VPT", L"VPU", L"VPV", L"VPW", L"VPX", L"VPY", L"VPZ", L"VQA", L"VQB", L"VQC", L"VQD", L"VQE", L"VQF", L"VQG", L"VQH", L"VQI", L"VQJ", L"VQK", L"VQL", L"VQM", 
			L"VQN", L"VQO", L"VQP", L"VQQ", L"VQR", L"VQS", L"VQT", L"VQU", L"VQV", L"VQW", L"VQX", L"VQY", L"VQZ", L"VRA", L"VRB", L"VRC", L"VRD", L"VRE", L"VRF", L"VRG", L"VRH", L"VRI", L"VRJ", L"VRK", L"VRL", L"VRM", L"VRN", L"VRO", L"VRP", L"VRQ", L"VRR", L"VRS", L"VRT", L"VRU", L"VRV", L"VRW", L"VRX", L"VRY", L"VRZ", L"VSA", L"VSB", L"VSC", L"VSD", L"VSE", L"VSF", L"VSG", L"VSH", L"VSI", L"VSJ", L"VSK", L"VSL", L"VSM", L"VSN", L"VSO", L"VSP", L"VSQ", L"VSR", L"VSS", L"VST", L"VSU", L"VSV", L"VSW", L"VSX", L"VSY", L"VSZ", L"VTA", L"VTB", L"VTC", L"VTD", L"VTE", L"VTF", L"VTG", L"VTH", L"VTI", L"VTJ", L"VTK", L"VTL", L"VTM", L"VTN", L"VTO", L"VTP", L"VTQ", L"VTR", L"VTS", L"VTT", L"VTU", L"VTV", L"VTW", L"VTX", L"VTY", L"VTZ", L"VUA", L"VUB", L"VUC", L"VUD", L"VUE", L"VUF", L"VUG", L"VUH", L"VUI", L"VUJ", L"VUK", L"VUL", L"VUM", L"VUN", L"VUO", L"VUP", L"VUQ", L"VUR", L"VUS", L"VUT", L"VUU", L"VUV", L"VUW", L"VUX", L"VUY", L"VUZ", L"VVA", L"VVB", L"VVC", L"VVD", L"VVE", L"VVF", L"VVG", L"VVH", L"VVI", L"VVJ", L"VVK", L"VVL", L"VVM", L"VVN", L"VVO", L"VVP", L"VVQ", L"VVR", L"VVS", L"VVT", L"VVU", L"VVV", L"VVW", L"VVX", L"VVY", L"VVZ", L"VWA", L"VWB", L"VWC", L"VWD", L"VWE", L"VWF", L"VWG", L"VWH", L"VWI", L"VWJ", L"VWK", L"VWL", L"VWM", L"VWN", L"VWO", L"VWP", L"VWQ", L"VWR", L"VWS", L"VWT", L"VWU", L"VWV", L"VWW", L"VWX", L"VWY", L"VWZ", L"VXA", L"VXB", L"VXC", L"VXD", L"VXE", L"VXF", L"VXG", L"VXH", L"VXI", L"VXJ", L"VXK", L"VXL", L"VXM", L"VXN", L"VXO", L"VXP", L"VXQ", L"VXR", L"VXS", L"VXT", L"VXU", L"VXV", L"VXW", L"VXX", L"VXY", L"VXZ", L"VYA", L"VYB", L"VYC", L"VYD", L"VYE", L"VYF", L"VYG", L"VYH", L"VYI", L"VYJ", L"VYK", L"VYL", L"VYM", L"VYN", L"VYO", L"VYP", L"VYQ", L"VYR", L"VYS", L"VYT", L"VYU", L"VYV", L"VYW", L"VYX", L"VYY", L"VYZ", L"VZA", L"VZB", L"VZC", L"VZD", L"VZE", L"VZF", L"VZG", L"VZH", L"VZI", L"VZJ", L"VZK", L"VZL", L"VZM", L"VZN", L"VZO", L"VZP", L"VZQ", L"VZR", L"VZS", L"VZT", L"VZU", L"VZV", L"VZW", L"VZX", L"VZY", L"VZZ", L"WAA", L"WAB", L"WAC", L"WAD", L"WAE", L"WAF", L"WAG", L"WAH", L"WAI", L"WAJ", L"WAK", L"WAL", L"WAM", L"WAN", L"WAO", L"WAP", L"WAQ", L"WAR", L"WAS", L"WAT", L"WAU", L"WAV", L"WAW", L"WAX", L"WAY", L"WAZ", 
			L"WBA", L"WBB", L"WBC", L"WBD", L"WBE", L"WBF", L"WBG", L"WBH", L"WBI", L"WBJ", L"WBK", L"WBL", L"WBM", L"WBN", L"WBO", L"WBP", L"WBQ", L"WBR", L"WBS", L"WBT", L"WBU", L"WBV", L"WBW", L"WBX", L"WBY", L"WBZ", L"WCA", L"WCB", L"WCC", L"WCD", L"WCE", L"WCF", L"WCG", L"WCH", L"WCI", L"WCJ", L"WCK", L"WCL", L"WCM", L"WCN", L"WCO", L"WCP", L"WCQ", L"WCR", L"WCS", L"WCT", L"WCU", L"WCV", L"WCW", L"WCX", L"WCY", L"WCZ", L"WDA", L"WDB", L"WDC", L"WDD", L"WDE", L"WDF", L"WDG", L"WDH", L"WDI", L"WDJ", L"WDK", L"WDL", L"WDM", L"WDN", L"WDO", L"WDP", L"WDQ", L"WDR", L"WDS", L"WDT", L"WDU", L"WDV", L"WDW", L"WDX", L"WDY", L"WDZ", L"WEA", L"WEB", L"WEC", L"WED", L"WEE", L"WEF", L"WEG", L"WEH", L"WEI", L"WEJ", L"WEK", L"WEL", L"WEM", L"WEN", L"WEO", L"WEP", L"WEQ", L"WER", L"WES", L"WET", L"WEU", L"WEV", L"WEW", L"WEX", L"WEY", L"WEZ", L"WFA", L"WFB", L"WFC", L"WFD", L"WFE", L"WFF", L"WFG", L"WFH", L"WFI", L"WFJ", L"WFK", L"WFL", L"WFM", L"WFN", L"WFO", L"WFP", L"WFQ", L"WFR", L"WFS", L"WFT", L"WFU", L"WFV", L"WFW", L"WFX", L"WFY", L"WFZ", L"WGA", L"WGB", L"WGC", L"WGD", L"WGE", L"WGF", L"WGG", L"WGH", L"WGI", L"WGJ", L"WGK", L"WGL", L"WGM", L"WGN", L"WGO", L"WGP", L"WGQ", L"WGR", L"WGS", L"WGT", L"WGU", L"WGV", L"WGW", L"WGX", L"WGY", L"WGZ", L"WHA", L"WHB", L"WHC", L"WHD", L"WHE", L"WHF", L"WHG", L"WHH", L"WHI", L"WHJ", L"WHK", L"WHL", L"WHM", L"WHN", L"WHO", L"WHP", L"WHQ", L"WHR", L"WHS", L"WHT", L"WHU", L"WHV", L"WHW", L"WHX", L"WHY", L"WHZ", L"WIA", L"WIB", L"WIC", L"WID", L"WIE", L"WIF", L"WIG", L"WIH", L"WII", L"WIJ", L"WIK", L"WIL", L"WIM", L"WIN", L"WIO", L"WIP", L"WIQ", L"WIR", L"WIS", L"WIT", L"WIU", L"WIV", L"WIW", L"WIX", L"WIY", L"WIZ", L"WJA", L"WJB", L"WJC", L"WJD", L"WJE", L"WJF", L"WJG", L"WJH", L"WJI", L"WJJ", L"WJK", L"WJL", L"WJM", L"WJN", L"WJO", L"WJP", L"WJQ", L"WJR", L"WJS", L"WJT", L"WJU", L"WJV", L"WJW", L"WJX", L"WJY", L"WJZ", L"WKA", L"WKB", L"WKC", L"WKD", L"WKE", L"WKF", L"WKG", L"WKH", L"WKI", L"WKJ", L"WKK", L"WKL", L"WKM", L"WKN", L"WKO", L"WKP", L"WKQ", L"WKR", L"WKS", L"WKT", L"WKU", L"WKV", L"WKW", L"WKX", L"WKY", L"WKZ", L"WLA", L"WLB", L"WLC", L"WLD", L"WLE", L"WLF", L"WLG", L"WLH", L"WLI", L"WLJ", L"WLK", L"WLL", L"WLM", 
			L"WLN", L"WLO", L"WLP", L"WLQ", L"WLR", L"WLS", L"WLT", L"WLU", L"WLV", L"WLW", L"WLX", L"WLY", L"WLZ", L"WMA", L"WMB", L"WMC", L"WMD", L"WME", L"WMF", L"WMG", L"WMH", L"WMI", L"WMJ", L"WMK", L"WML", L"WMM", L"WMN", L"WMO", L"WMP", L"WMQ", L"WMR", L"WMS", L"WMT", L"WMU", L"WMV", L"WMW", L"WMX", L"WMY", L"WMZ", L"WNA", L"WNB", L"WNC", L"WND", L"WNE", L"WNF", L"WNG", L"WNH", L"WNI", L"WNJ", L"WNK", L"WNL", L"WNM", L"WNN", L"WNO", L"WNP", L"WNQ", L"WNR", L"WNS", L"WNT", L"WNU", L"WNV", L"WNW", L"WNX", L"WNY", L"WNZ", L"WOA", L"WOB", L"WOC", L"WOD", L"WOE", L"WOF", L"WOG", L"WOH", L"WOI", L"WOJ", L"WOK", L"WOL", L"WOM", L"WON", L"WOO", L"WOP", L"WOQ", L"WOR", L"WOS", L"WOT", L"WOU", L"WOV", L"WOW", L"WOX", L"WOY", L"WOZ", L"WPA", L"WPB", L"WPC", L"WPD", L"WPE", L"WPF", L"WPG", L"WPH", L"WPI", L"WPJ", L"WPK", L"WPL", L"WPM", L"WPN", L"WPO", L"WPP", L"WPQ", L"WPR", L"WPS", L"WPT", L"WPU", L"WPV", L"WPW", L"WPX", L"WPY", L"WPZ", L"WQA", L"WQB", L"WQC", L"WQD", L"WQE", L"WQF", L"WQG", L"WQH", L"WQI", L"WQJ", L"WQK", L"WQL", L"WQM", L"WQN", L"WQO", L"WQP", L"WQQ", L"WQR", L"WQS", L"WQT", L"WQU", L"WQV", L"WQW", L"WQX", L"WQY", L"WQZ", L"WRA", L"WRB", L"WRC", L"WRD", L"WRE", L"WRF", L"WRG", L"WRH", L"WRI", L"WRJ", L"WRK", L"WRL", L"WRM", L"WRN", L"WRO", L"WRP", L"WRQ", L"WRR", L"WRS", L"WRT", L"WRU", L"WRV", L"WRW", L"WRX", L"WRY", L"WRZ", L"WSA", L"WSB", L"WSC", L"WSD", L"WSE", L"WSF", L"WSG", L"WSH", L"WSI", L"WSJ", L"WSK", L"WSL", L"WSM", L"WSN", L"WSO", L"WSP", L"WSQ", L"WSR", L"WSS", L"WST", L"WSU", L"WSV", L"WSW", L"WSX", L"WSY", L"WSZ", L"WTA", L"WTB", L"WTC", L"WTD", L"WTE", L"WTF", L"WTG", L"WTH", L"WTI", L"WTJ", L"WTK", L"WTL", L"WTM", L"WTN", L"WTO", L"WTP", L"WTQ", L"WTR", L"WTS", L"WTT", L"WTU", L"WTV", L"WTW", L"WTX", L"WTY", L"WTZ", L"WUA", L"WUB", L"WUC", L"WUD", L"WUE", L"WUF", L"WUG", L"WUH", L"WUI", L"WUJ", L"WUK", L"WUL", L"WUM", L"WUN", L"WUO", L"WUP", L"WUQ", L"WUR", L"WUS", L"WUT", L"WUU", L"WUV", L"WUW", L"WUX", L"WUY", L"WUZ", L"WVA", L"WVB", L"WVC", L"WVD", L"WVE", L"WVF", L"WVG", L"WVH", L"WVI", L"WVJ", L"WVK", L"WVL", L"WVM", L"WVN", L"WVO", L"WVP", L"WVQ", L"WVR", L"WVS", L"WVT", L"WVU", L"WVV", L"WVW", L"WVX", L"WVY", L"WVZ", 
			L"WWA", L"WWB", L"WWC", L"WWD", L"WWE", L"WWF", L"WWG", L"WWH", L"WWI", L"WWJ", L"WWK", L"WWL", L"WWM", L"WWN", L"WWO", L"WWP", L"WWQ", L"WWR", L"WWS", L"WWT", L"WWU", L"WWV", L"WWW", L"WWX", L"WWY", L"WWZ", L"WXA", L"WXB", L"WXC", L"WXD", L"WXE", L"WXF", L"WXG", L"WXH", L"WXI", L"WXJ", L"WXK", L"WXL", L"WXM", L"WXN", L"WXO", L"WXP", L"WXQ", L"WXR", L"WXS", L"WXT", L"WXU", L"WXV", L"WXW", L"WXX", L"WXY", L"WXZ", L"WYA", L"WYB", L"WYC", L"WYD", L"WYE", L"WYF", L"WYG", L"WYH", L"WYI", L"WYJ", L"WYK", L"WYL", L"WYM", L"WYN", L"WYO", L"WYP", L"WYQ", L"WYR", L"WYS", L"WYT", L"WYU", L"WYV", L"WYW", L"WYX", L"WYY", L"WYZ", L"WZA", L"WZB", L"WZC", L"WZD", L"WZE", L"WZF", L"WZG", L"WZH", L"WZI", L"WZJ", L"WZK", L"WZL", L"WZM", L"WZN", L"WZO", L"WZP", L"WZQ", L"WZR", L"WZS", L"WZT", L"WZU", L"WZV", L"WZW", L"WZX", L"WZY", L"WZZ", L"XAA", L"XAB", L"XAC", L"XAD", L"XAE", L"XAF", L"XAG", L"XAH", L"XAI", L"XAJ", L"XAK", L"XAL", L"XAM", L"XAN", L"XAO", L"XAP", L"XAQ", L"XAR", L"XAS", L"XAT", L"XAU", L"XAV", L"XAW", L"XAX", L"XAY", L"XAZ", L"XBA", L"XBB", L"XBC", L"XBD", L"XBE", L"XBF", L"XBG", L"XBH", L"XBI", L"XBJ", L"XBK", L"XBL", L"XBM", L"XBN", L"XBO", L"XBP", L"XBQ", L"XBR", L"XBS", L"XBT", L"XBU", L"XBV", L"XBW", L"XBX", L"XBY", L"XBZ", L"XCA", L"XCB", L"XCC", L"XCD", L"XCE", L"XCF", L"XCG", L"XCH", L"XCI", L"XCJ", L"XCK", L"XCL", L"XCM", L"XCN", L"XCO", L"XCP", L"XCQ", L"XCR", L"XCS", L"XCT", L"XCU", L"XCV", L"XCW", L"XCX", L"XCY", L"XCZ", L"XDA", L"XDB", L"XDC", L"XDD", L"XDE", L"XDF", L"XDG", L"XDH", L"XDI", L"XDJ", L"XDK", L"XDL", L"XDM", L"XDN", L"XDO", L"XDP", L"XDQ", L"XDR", L"XDS", L"XDT", L"XDU", L"XDV", L"XDW", L"XDX", L"XDY", L"XDZ", L"XEA", L"XEB", L"XEC", L"XED", L"XEE", L"XEF", L"XEG", L"XEH", L"XEI", L"XEJ", L"XEK", L"XEL", L"XEM", L"XEN", L"XEO", L"XEP", L"XEQ", L"XER", L"XES", L"XET", L"XEU", L"XEV", L"XEW", L"XEX", L"XEY", L"XEZ", L"XFA", L"XFB", L"XFC", L"XFD" };

//------------------------------------------------------------------------------
		static std::wstring getColAddress(size_t col)
		{//from 1
			col--;
			static const size_t r = ('Z' - 'A' + 1);
			std::string res;
			size_t r0 = col / r;

			if (r0 > 0)
			{
				const std::wstring rest = getColAddress(col - r * r0 + 1);
				const std::wstring res	= getColAddress(r0 - 1 + 1) + rest;
				return res;
			}
			else
				return std::wstring(1, (char)(L'A' + col));
		}

		static std::wstring getRowAddress(size_t row)
		{//from 1
			return std::to_wstring(row);
		}
//------------------------------------------------------------------------------
		static std::string getColAddressA(size_t col)
		{//from 1
			col--;
			static const size_t r = ('Z' - 'A' + 1);
			std::string res;
			size_t r0 = col / r;

			if (r0 > 0)
			{
				const std::string rest = getColAddressA(col - r * r0 + 1);
				const std::string res	= getColAddressA(r0 - 1 + 1) + rest;
				return res;
			}
			else
				return std::string(1, (char)(L'A' + col));
		}

		static std::string getRowAddressA(size_t row)
		{//from 1
			return std::to_string(row);
		}
//------------------------------------------------------------------------------
		static std::wstring getCellAddress(size_t row, size_t col, bool bAbsoluteRow = false, bool bAbsoluteCol = false)
		{
			return (bAbsoluteCol ? L"$" : L"") + getColAddress(col) + (bAbsoluteRow ? L"$" : L"") + getRowAddress(row);
		}
		static std::string getCellAddressA(size_t row, size_t col, bool bAbsoluteRow = false, bool bAbsoluteCol = false)
		{
			return (bAbsoluteCol ? "$" : "") + getColAddressA(col) + (bAbsoluteRow ? "$" : "") + getRowAddressA(row);
		}
//------------------------------------------------------------------------------
		static bool parseDate(const std::wstring & Date, double & Value)
		{
			// for example, "1899-12-31T05:37:46.665696"
			try
			{
				boost::wregex r(L"([\\d]+)-([\\d]+)-([\\d]+)(?:T([\\d]+):([\\d]+):([\\d]+)(?:\\.([\\d]+))?)?");
				boost::match_results<std::wstring::const_iterator> res;
				
				if (boost::regex_match(Date, res, r))
				{
					Value = 0;

					int Hours = 0, Minutes = 0, Sec = 0, FSec = 0;
					int Year, Month, Day;

					Year = boost::lexical_cast<int>(res[1].str());
					Month = boost::lexical_cast<int>(res[2].str());
					Day = boost::lexical_cast<int>(res[3].str());

					if (res[4].matched)
						Hours = boost::lexical_cast<int>(res[4].str());

					if (res[5].matched)
						Minutes = boost::lexical_cast<int>(res[5].str());

					if (res[6].matched)
						Sec = boost::lexical_cast<int>(res[6].str());

					if (res[7].matched)
						FSec = boost::lexical_cast<int>(res[7].str());

					if (Year < 1400 || Year >10000)
						return -1;
					if (Month < 1 || Month > 12)
						return -1;
					if (Day < 1 || Day > 31)
						return -1;

					boost::int64_t daysFrom1900 = boost::gregorian::date_duration(boost::gregorian::date(Year, Month, Day) - boost::gregorian::date(1900, 1, 1)).days() + 1;

					if (Year <= 1900 &&
						Month <= 2 &&
						Day <= 29)
					{
						Value = daysFrom1900;
					}
					else
					{
						Value = daysFrom1900 + 1;
					}

					if (Hours > 0 || Sec > 0 || Minutes > 0)
					{
						boost::posix_time::time_duration t(Hours, Minutes, 0);
						t += boost::posix_time::millisec(static_cast<boost::uint32_t>(Sec * 1000));
						boost::posix_time::time_duration day(24, 0, 0);

						const boost::uint64_t m1 = t.total_milliseconds();
						const boost::uint64_t m2 = day.total_milliseconds();
						Value += 1.0 * m1 / m2;						
					}

					return true;
				}
			}
			catch (...)
			{
			}
			return false;
		}
//-----------------------------------------------------------------------------------------------------

		class r1c1_formula_convert
		{
		public:
			static size_t base_row;
			static size_t base_col;

			r1c1_formula_convert()
			{}

			static std::wstring replace_ref(boost::wsmatch const & what)
			{
				const size_t sz = what.size();

				std::wstring result = what[0].str();

				std::wstring sCell = sz > 1 ? what[1].str() : L"";
				std::wstring sRow = sz > 2 ? what[2].str() : L"";
				std::wstring sCol = sz > 3 ? what[3].str() : L"";

				size_t row = boost::lexical_cast<size_t>(sRow);
				size_t col = boost::lexical_cast<size_t>(sCol);

				return getCellAddress(row, col, true, true);
			}
			static std::wstring replace_ref_from_base(boost::wsmatch const & what)
			{
				const size_t sz = what.size();

				std::wstring result = what[0].str();

				std::wstring s1 = sz > 1 ? what[1].str() : L"";
				std::wstring s2 = sz > 2 ? what[2].str() : L"";
				std::wstring s3 = sz > 3 ? what[3].str() : L"";
				std::wstring s4 = sz > 4 ? what[4].str() : L"";
				std::wstring s5 = sz > 5 ? what[5].str() : L"";

				//size_t row = sRow.empty() ? 0 : boost::lexical_cast<size_t>(sRow);
				//size_t col = sCol.empty() ? 0 : boost::lexical_cast<size_t>(sCol);

				size_t row = 0, col = 0;

				bool bAbsoluteCol = false, bAbsoluteRow = false;

				if (s2.empty())
				{
					row = base_row;
				}
				else if (0 == s2.find(L"["))
				{
					row = boost::lexical_cast<size_t>(s2.substr(1, s2.length() - 2)) + base_row;
				}
				else
				{
					row = boost::lexical_cast<size_t>(s2);
					bAbsoluteRow = true;
				}
				if (s4.empty())
				{
					col = base_col;
				}
				else if (0 == s4.find(L"["))
				{
					col = boost::lexical_cast<size_t>(s4.substr(1, s4.length() - 2)) + base_col;
				}
				else
				{
					col = boost::lexical_cast<size_t>(s4);
					bAbsoluteCol = true;
				}
				return getCellAddress(row, col, bAbsoluteRow, bAbsoluteCol);
			}


			std::wstring convert(const std::wstring& expr)
			{
				//boost::wregex findRef(L"(\R(\\d+)\C(\\d+))"); //easy
				boost::wregex findRefFromBase(L"(\R((\\[?\-?\\d+\\]?)?)\C((\\[?\-?\\d+\\]?)?))");

				//std::wstring result = boost::regex_replace(
				//	expr,
				//	findRef,
				//	&replace_ref,
				//	boost::match_default | boost::format_all);

				std::wstring result = boost::regex_replace(
					expr,
					findRefFromBase,
					&replace_ref_from_base,
					boost::match_default | boost::format_all);

				return result;
			}
		};
		size_t	r1c1_formula_convert::base_col = 1;
		size_t	r1c1_formula_convert::base_row = 1;

		CFormulaXLSB::CFormulaXLSB():m_oFormula(256),m_oRef(256),m_oR1(256),m_oR2(256)
		{
			Clean();
		}
		void CFormulaXLSB::Clean()
		{
			m_bIsInit = false;
			m_oFormula.Clean();
			m_oRef.Clean();
			m_oR1.Clean();
			m_oR2.Clean();

			m_oT.SetValue(SimpleTypes::Spreadsheet::cellformulatypeNormal);
			m_nSi = -1;

			m_oAca.FromBool(false);
			m_oBx.FromBool(false);
			m_oCa.FromBool(false);
			m_oDel1.FromBool(false);
			m_oDel2.FromBool(false);
			m_oDt2D.FromBool(false);
			m_oDtr.FromBool(false);
		}
		void CFormulaXLSB::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_bIsInit = true;
			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			m_oFormula.fromXML(oReader, false);
		}
		void CFormulaXLSB::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )

			if ( strcmp("t", wsName) == 0 )
			{
				m_oT.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("si", wsName) == 0 )
			{
				m_nSi = atoi(oReader.GetTextChar());
			}
			else if ( strcmp("ref", wsName) == 0 )
			{
				m_oRef.fromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("ca", wsName) == 0 )
			{
				m_oCa.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("aca", wsName) == 0 )
			{
				m_oAca.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("bx", wsName) == 0 )
			{
				m_oBx.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("del1", wsName) == 0 )
			{
				m_oDel1.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("del2", wsName) == 0 )
			{
				m_oDel2.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("dt2D", wsName) == 0 )
			{
				m_oDt2D.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("dtr", wsName) == 0 )
			{
				m_oDtr.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("r1", wsName) == 0 )
			{
				m_oR1.fromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("r2", wsName) == 0 )
			{
				m_oR2.fromStringA(oReader.GetTextChar());
			}

			WritingElement_ReadAttributes_EndChar( oReader )
		}
		_UINT32 CFormulaXLSB::getXLSBSize() const
		{
			_UINT32 nLen = 2+4+4;
			if(m_bIsInit)
			{
				nLen += 4 + 2 * m_oFormula.m_nLen;
				if(m_oR1.m_nLen > 0)
				{
					nLen += 4 + 2 * m_oR1.m_nLen;
				}
				if(m_oR2.m_nLen > 0)
				{
					nLen += 4 + 2 * m_oR2.m_nLen;
				}
				if(m_oRef.m_nLen > 0)
				{
					nLen += 4 + 2 * m_oRef.m_nLen;
				}
				if(m_nSi >= 0)
				{
					nLen += 4;
				}
			}
			return nLen;
		}
		_UINT16 CFormulaXLSB::toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream, bool bIsBlankFormula)
		{
			_UINT16 nFlags = 0;
			if(m_oCa.ToBool())
			{
				nFlags |= 0x2;
			}
			oStream.WriteUSHORT(nFlags);
			oStream.WriteULONG(0);//cce
			oStream.WriteULONG(0);//cb

			_UINT16 nFlagsExt = 0;
			nFlagsExt |= m_oT.GetValue();
			nFlagsExt |= 0x4;
			if(m_oAca.ToBool())
			{
				nFlagsExt |= 0x8;
			}
			if(m_oBx.ToBool())
			{
				nFlagsExt |= 0x10;
			}
			if(m_oDel1.ToBool())
			{
				nFlagsExt |= 0x20;
			}
			if(m_oDel2.ToBool())
			{
				nFlagsExt |= 0x40;
			}
			if(m_oDt2D.ToBool())
			{
				nFlagsExt |= 0x80;
			}
			if(m_oDtr.ToBool())
			{
				nFlagsExt |= 0x100;
			}
			if(m_oR1.m_nLen > 0)
			{
				nFlagsExt |= 0x200;
			}
			if(m_oR2.m_nLen > 0)
			{
				nFlagsExt |= 0x400;
			}
			if(m_oRef.m_nLen > 0)
			{
				nFlagsExt |= 0x800;
			}
			if(m_nSi >= 0)
			{
				nFlagsExt |= 0x1000;
			}
			if(bIsBlankFormula)
			{
				nFlagsExt |= 0x4000;
			}
			return nFlagsExt;
		}
		void CFormulaXLSB::toXLSBExt(NSBinPptxRW::CXlsbBinaryWriter& oStream)
		{
			if(!m_bIsInit)
				return;
			oStream.WriteStringData(m_oFormula.m_sBuffer, m_oFormula.m_nLen);
			if(m_oR1.m_nLen > 0)
			{
				oStream.WriteStringData(m_oR1.m_sBuffer, m_oR1.m_nLen);
			}
			if(m_oR2.m_nLen > 0)
			{
				oStream.WriteStringData(m_oR2.m_sBuffer, m_oR2.m_nLen);
			}
			if(m_oRef.m_nLen > 0)
			{
				oStream.WriteStringData(m_oRef.m_sBuffer, m_oRef.m_nLen);
			}
			if(m_nSi >= 0)
			{
				oStream.WriteULONG(m_nSi);
			}
		}

		CCellXLSB::CCellXLSB():m_oValue(256)
		{
			Clean();
		}
		void CCellXLSB::Clean()
		{
			m_nCol = 0;
			m_nStyle = 0;
			m_oType.SetValue(SimpleTypes::Spreadsheet::celltypeNumber);
			m_oShowPhonetic.FromBool(false);

			m_oValue.Clean();
			m_oFormula.Clean();
			m_oRichText.reset(NULL);
		}
		void CCellXLSB::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if(oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

				if ( strcmp("v", sName) == 0 )
				{
					m_oValue.fromXML(oReader, m_oType.GetValue());
				}
				else if ( strcmp("f", sName) == 0 )
					m_oFormula.fromXML(oReader);
				else if ( strcmp("is", sName) == 0 )
					m_oRichText = oReader;
			}
		}
		void CCellXLSB::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )

				if (strcmp("r", wsName) == 0)
				{
					if(CCell::parseRefColA(oReader.GetTextChar(), m_nCol))
					{
						m_nCol--;
					}
				}
				else if (strcmp("s", wsName) == 0)
				{
					m_nStyle = atol(oReader.GetTextChar());
				}
				else if (strcmp("t", wsName) == 0)
				{
					m_oType.FromStringA(oReader.GetTextChar());
				}
				else if (strcmp("ph", wsName) == 0)
				{
					m_oShowPhonetic.FromStringA(oReader.GetTextChar());
				}

				WritingElement_ReadAttributes_EndChar( oReader )
		}
		void CCellXLSB::toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream)
		{
			_INT16 nType = XLSB::rt_CellBlank;
			if(m_oFormula.m_bIsInit && SimpleTypes::Spreadsheet::celltypeSharedString == m_oType.GetValue())
			{
				m_oType.SetValue(SimpleTypes::Spreadsheet::celltypeError);
				m_oValue.Clean();
				m_oValue.m_bIsInit = true;
				m_oValue.m_nValue = 0x0F;//L"#VALUE!";
			}
			if(m_oValue.m_bIsInit)
			{
				switch(m_oType.GetValue())
				{
                    case SimpleTypes::Spreadsheet::celltypeNumber: nType = XLSB::rt_CellReal; break;
                    case SimpleTypes::Spreadsheet::celltypeSharedString: nType = XLSB::rt_CellIsst; break;
                    case SimpleTypes::Spreadsheet::celltypeError: nType = XLSB::rt_CellError; break;
                    case SimpleTypes::Spreadsheet::celltypeBool: nType = XLSB::rt_CellBool; break;
					case SimpleTypes::Spreadsheet::celltypeInlineStr:
                    case SimpleTypes::Spreadsheet::celltypeStr: nType = XLSB::rt_CellSt; break;
				}
			}
			bool bIsBlankFormula = false;
			if(XLSB::rt_CellBlank == nType && m_oFormula.m_bIsInit)
			{
                nType = XLSB::rt_CellSt;
				bIsBlankFormula = true;
			}

			_UINT32 nLen = 4+4+2;
			switch(nType)
			{
                case XLSB::rt_CellReal: nLen += 8; break;
                case XLSB::rt_CellIsst: nLen += 4; break;
                case XLSB::rt_CellSt: nLen += 4 + 2 * m_oValue.m_oValue.m_nLen;break;
                case XLSB::rt_CellError:
                case XLSB::rt_CellBool: nLen += 1; break;
			}
			if(m_oFormula.m_bIsInit)
			{
				nLen += m_oFormula.getXLSBSize();
                if(XLSB::rt_CellReal == nType)
				{
                    nType = XLSB::rt_FmlaNum;
				}
                else if(XLSB::rt_CellSt == nType)
				{
                    nType = XLSB::rt_FmlaString;
				}
                else if(XLSB::rt_CellBool == nType)
				{
                    nType = XLSB::rt_FmlaBool;
				}
                else if(XLSB::rt_CellError == nType)
				{
                    nType = XLSB::rt_FmlaError;
				}
				else if(XLSB::rt_CellBlank == nType)
				{
                    nType = XLSB::rt_FmlaString;
				}
			}
			if(m_oRichText.IsInit())
			{
				nLen += m_oRichText->getXLSBSize();
			}

			oStream.XlsbStartRecord(nType, nLen);
			oStream.WriteULONG(m_nCol & 0x3FFF);

			_UINT32 nStyle = m_nStyle;
			if (m_oShowPhonetic.ToBool())
			{
				nStyle |= 0x1000000;
			}
			oStream.WriteULONG(nStyle);
			//todo RkNumber
			switch(nType)
			{
                case XLSB::rt_CellReal:
                case XLSB::rt_FmlaNum:
					oStream.WriteDoubleReal(m_oValue.m_dValue);
				break;
                case XLSB::rt_CellIsst:
					oStream.WriteULONG(m_oValue.m_nValue);
				break;
                case XLSB::rt_CellSt:
                case XLSB::rt_FmlaString:
					oStream.WriteStringData(m_oValue.m_oValue.m_sBuffer, m_oValue.m_oValue.m_nLen);
				break;
                case XLSB::rt_CellError:
                case XLSB::rt_FmlaError:
                case XLSB::rt_CellBool:
                case XLSB::rt_FmlaBool:
					oStream.WriteBYTE(m_oValue.m_nValue);
				break;
			}

			_UINT16 nFlags = 0;
			if(m_oFormula.m_bIsInit)
			{
				nFlags = m_oFormula.toXLSB(oStream, bIsBlankFormula);
			}

			if(m_oRichText.IsInit())
			{
				nFlags |= 0x2000;
			}
			oStream.WriteUSHORT(nFlags);
			if(m_oFormula.m_bIsInit)
			{
				m_oFormula.toXLSBExt(oStream);
			}
			if(m_oRichText.IsInit())
			{
				m_oRichText->toXLSBExt(oStream);
			}

			oStream.XlsbEndRecord();
		}

		CRowXLSB::CRowXLSB()
		{
			Clean();
		}
		void CRowXLSB::Clean()
		{
			m_nR = 0;
			m_nS = 0;
			m_oCustomFormat.FromBool(false);
			m_dHt = 0;
			m_oHidden.FromBool(false);
			m_oCustomHeight.FromBool(false);
			m_nOutlineLevel = 0;
			m_oCollapsed.FromBool(false);
			m_oThickBot.FromBool(false);
			m_oThickTop.FromBool(false);
			m_oPh.FromBool(false);
		}
		void CRowXLSB::fromXMLToXLSB(XmlUtils::CXmlLiteReader& oReader, NSBinPptxRW::CXlsbBinaryWriter& oStream, CCellXLSB& oCell)
		{
			ReadAttributes( oReader );

			toXLSB(oStream);

			if ( oReader.IsEmptyNode() )
				return;

			int nLastCol = -1;
			oCell.m_nCol = nLastCol;
			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

				if ( strcmp("c", sName) == 0 )
				{
					nLastCol = oCell.m_nCol;
					oCell.Clean();
					oCell.m_nCol = nLastCol + 1;
					oCell.fromXML(oReader);
					oCell.toXLSB(oStream);
				}
			}
		}
		void CRowXLSB::toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream)
		{
            oStream.XlsbStartRecord(XLSB::rt_RowHdr, 17);
			oStream.WriteULONG(m_nR & 0xFFFFF);
			oStream.WriteULONG(m_nS);
			oStream.WriteUSHORT(((_UINT16)(m_dHt * 20)) & 0x1FFF);//pt to twips;

			BYTE byteExtra1 = 0;
			if(m_oThickTop.ToBool())
			{
				byteExtra1 |= 0x1;
			}
			if(m_oThickTop.ToBool())
			{
				byteExtra1 |= 0x2;
			}
			oStream.WriteBYTE(byteExtra1);
			BYTE byteExtra2 = m_nOutlineLevel & 0x7;
			if(m_oCollapsed.ToBool())
			{
				byteExtra2 |= 0x8;
			}
			if(m_oHidden.ToBool())
			{
				byteExtra2 |= 0x10;
			}
			if(m_oCustomHeight.ToBool())
			{
				byteExtra2 |= 0x20;
			}
			if(m_oCustomFormat.ToBool())
			{
				byteExtra2 |= 0x40;
			}
			oStream.WriteBYTE(byteExtra2);
			BYTE byteExtra3 = 0;
			if(m_oPh.ToBool())
			{
				byteExtra3 |= 0x1;
			}
			oStream.WriteBYTE(byteExtra3);
			//ccolspan
			oStream.WriteULONG(0);

			oStream.XlsbEndRecord();
		}
		void CRowXLSB::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			bool hasHt = false;
			WritingElement_ReadAttributes_StartChar( oReader )

			if ( strcmp("r", wsName) == 0 )\
			{
				m_nR = atoi(oReader.GetTextChar()) - 1;
			}
			else if ( strcmp("s", wsName) == 0 )\
			{
				m_nS = atoi(oReader.GetTextChar());
			}
			else if ( strcmp("customFormat", wsName) == 0 )\
			{
				m_oCustomFormat.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("ht", wsName) == 0 )\
			{
				hasHt = true;
				m_dHt = atof(oReader.GetTextChar());
			}
			else if ( strcmp("hidden", wsName) == 0 )\
			{
				m_oHidden.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("customHeight", wsName) == 0 )\
			{
				m_oCustomHeight.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("outlineLevel", wsName) == 0 )\
			{
				m_nOutlineLevel = atoi(oReader.GetTextChar());
			}
			else if ( strcmp("collapsed", wsName) == 0 )\
			{
				m_oCollapsed.FromStringA(oReader.GetTextChar());
			}
//			else if ( strcmp("x14ac:dyDescent", wsName) == 0 )\
//			{
//				m_dDyDescent = atof(oReader.GetTextChar());
//			}
			else if ( strcmp("thickBot", wsName) == 0 )\
			{
				m_oThickBot.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("thickTop", wsName) == 0 )\
			{
				m_oThickTop.FromStringA(oReader.GetTextChar());
			}
			else if ( strcmp("ph", wsName) == 0 )\
			{
				m_oPh.FromStringA(oReader.GetTextChar());
			}
			WritingElement_ReadAttributes_EndChar( oReader )

			//invalid combination for xlsb, in this case in xlsx ht is calculated by content
			if (m_oCustomHeight.ToBool() && !hasHt)
			{
				m_oCustomHeight.FromBool(false);
			}
		}

		CFormula::CFormula()
		{
		}
		CFormula::~CFormula()
		{
		}
		void CFormula::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CFormula::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText3();
		}
		std::wstring CFormula::toXML() const
		{
			return L"";
		}
		void CFormula::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<f"));
			WritingStringNullableAttrString(L"t", m_oT, m_oT->ToString());
			WritingStringNullableAttrBool(L"aca", m_oAca);
			WritingStringNullableAttrString(L"ref", m_oRef, m_oRef.get());
			WritingStringNullableAttrBool(L"dt2D", m_oDt2D);
			WritingStringNullableAttrBool(L"dtr", m_oDtr);
			WritingStringNullableAttrBool(L"del1", m_oDel1);
			WritingStringNullableAttrBool(L"del2", m_oDel2);
			WritingStringNullableAttrString(L"r1", m_oR1, m_oR1.get());
			WritingStringNullableAttrString(L"r2", m_oR2, m_oR2.get());
			WritingStringNullableAttrBool(L"ca", m_oCa);
			WritingStringNullableAttrInt(L"si", m_oSi, m_oSi->GetValue());
			WritingStringNullableAttrBool(L"bx", m_oBx);
			writer.WriteString(_T(">"));
			writer.WriteEncodeXmlString(m_sText);
			writer.WriteString(_T("</f>"));
		}
		void CFormula::fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream)
		{
			BYTE nFlags = oStream.GetUChar();
			oStream.Skip(1);
			if(0 != (nFlags & 0x2))
			{
				m_oCa.Init();
				m_oCa->FromBool(true);
			}
			oStream.GetULong();//cce
			oStream.GetULong();//cb
		}
		void CFormula::fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nFlags)
		{
			SimpleTypes::Spreadsheet::ECellFormulaType eType = SimpleTypes::Spreadsheet::ECellFormulaType(nFlags & 0x3);
			if(SimpleTypes::Spreadsheet::cellformulatypeNormal != eType)
			{
				m_oT.Init();
				m_oT->SetValue(eType);
			}
			m_sText = oStream.GetString2();
			if(0 != (nFlags & 0x8))
			{
				m_oAca.Init();
				m_oAca->FromBool(true);
			}
			if(0 != (nFlags & 0x10))
			{
				m_oBx.Init();
				m_oBx->FromBool(true);
			}
			if(0 != (nFlags & 0x20))
			{
				m_oDel1.Init();
				m_oDel1->FromBool(true);
			}
			if(0 != (nFlags & 0x40))
			{
				m_oDel2.Init();
				m_oDel2->FromBool(true);
			}
			if(0 != (nFlags & 0x80))
			{
				m_oDt2D.Init();
				m_oDt2D->FromBool(true);
			}
			if(0 != (nFlags & 0x100))
			{
				m_oDtr.Init();
				m_oDtr->FromBool(true);
			}
			if(0 != (nFlags & 0x200))
			{
				m_oR1.Init();
				m_oR1->append(oStream.GetString2());
			}
			if(0 != (nFlags & 0x400))
			{
				m_oR2.Init();
				m_oR2->append(oStream.GetString2());
			}
			if(0 != (nFlags & 0x800))
			{
				m_oRef.Init();
				m_oRef->append(oStream.GetString2());
			}
			if(0 != (nFlags & 0x1000))
			{
				m_oSi.Init();
				m_oSi->SetValue(oStream.GetULong());
				m_oT.Init();
				m_oT->SetValue(SimpleTypes::Spreadsheet::cellformulatypeShared);
			}
		}
        void CFormula::fromBin(BaseObjectPtr& obj, SimpleTypes::Spreadsheet::ECellFormulaType eType)
        {
            m_oT.Init();
            m_oT->SetValue(eType);

            switch (eType)
            {
                case SimpleTypes::Spreadsheet::ECellFormulaType::cellformulatypeNormal:
                    {
                        auto formula = dynamic_cast<XLSB::FmlaBase*>(obj.get());
                        m_sText = formula->formula.getAssembledFormula();
                        //m_oCa.Init();
                        //m_oCa = formula->grbitFlags.fAlwaysCalc;
						m_oT.reset();
                    }
                    break;
                case SimpleTypes::Spreadsheet::ECellFormulaType::cellformulatypeShared:
                    {
                        auto formula = dynamic_cast<XLSB::ShrFmla*>(obj.get());
                        m_sText = formula->formula.getAssembledFormula();
                        m_oRef.Init();
                        m_oRef = formula->rfx.toString();                        
                    }
                    break;
                case SimpleTypes::Spreadsheet::ECellFormulaType::cellformulatypeArray:
                    {
                        auto formula = dynamic_cast<XLSB::ArrFmla*>(obj.get());
                        m_sText = formula->formula.getAssembledFormula();
                        if(formula->fAlwaysCalc)
                        {
                            m_oAca.Init();
                            m_oAca = formula->fAlwaysCalc;
                        }
                        m_oRef.Init();
                        m_oRef = formula->rfx.toString();
                    }
                    break;
                case SimpleTypes::Spreadsheet::ECellFormulaType::cellformulatypeDataTable:
                    {
                        auto dataTable = dynamic_cast<XLSB::Table*>(obj.get());
                        //m_sText = dataTable->formula.getAssembledFormula();

                        if(dataTable->fAlwaysCalc)
                        {
                            m_oCa.Init();
                            m_oCa = dataTable->fAlwaysCalc;
                        }

                        m_oRef.Init();
                        m_oRef = dataTable->rfx.toString();

                        if(dataTable->fRw)
                        {
                            m_oDtr.Init();
                            m_oDtr = dataTable->fRw;
                        }

                        if(dataTable->fTbl2)
                        {
                            m_oDt2D.Init();
                            m_oDt2D = dataTable->fTbl2;
                        }

                        if(dataTable->fDeleted1)
                        {
                            m_oDel1.Init();
                            m_oDel1 = dataTable->fDeleted1;
                        }

                        if(dataTable->fDeleted2)
                        {
                            m_oDel2.Init();
                            m_oDel2 = dataTable->fDeleted2;
                        }

                        m_oR1.Init();
                        m_oR1 = dataTable->r1;

                        m_oR2.Init();
                        m_oR2 = dataTable->r2;
                    }
                    break;

            }
        }
		EElementType CFormula::getType () const
		{
			return et_x_Formula;
		}
		void CFormula::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )

				WritingElement_ReadAttributes_Read_ifChar		( oReader, "t",		m_oT )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "si",	m_oSi )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "aca",	m_oAca )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "bx",    m_oBx )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "ca",    m_oCa )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "del1",	m_oDel1 )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "del2",	m_oDel2 )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "dt2D",	m_oDt2D )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "dtr",	m_oDtr )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "r1",	m_oR1 )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "r2",	m_oR2 )
				WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "ref",	m_oRef )

			WritingElement_ReadAttributes_EndChar( oReader )
		}

		CCell::CCell(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCell::~CCell()
		{
		}
		void CCell::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCell::toXML() const
		{
			return _T("");
		}
        bool CCell::parseRef(std::wstring sRef, int& nRow, int& nCol)
		{
			std::string sResA(sRef.begin(), sRef.end());
			return parseRefA(sResA.c_str(), nRow, nCol);
		}
		bool CCell::parseRefA(const char* sRef, int& nRow, int& nCol)
		{
			bool bRes = false;
			nRow = 0;
			nCol = 0;
			size_t len = strlen(sRef);
			for(size_t i = 0; i < len; ++i)
			{
				if('A' <= sRef[i] && sRef[i] <= 'Z')
				{
					//'a's
					nCol = nCol * 26 + sRef[i] - 'A' + 1;
				}
				else
				{
					nRow = atoi(sRef + i);
					bRes = 0 != i;
					break;
				}
			}
			return bRes;
		}
		bool CCell::parseRefColA(const char* sRef, _UINT32& nCol)
		{
			nCol = 0;
			size_t len = strlen(sRef);
			size_t i = 0;
			while(i < len && 'A' <= sRef[i] && sRef[i] <= 'Z'){
				nCol = nCol * 26 + sRef[i] - 'A' + 1;
				i++;
			}
			return 0 != i;
		}
		std::wstring CCell::combineRef(int nRow, int nCol)
		{
			if (nCol < 0 || nCol > 16383)
			{
				nCol = 0;
			}
			return (m_aLetters[nCol] + std::to_wstring(nRow + 1));
		}
		void CCell::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			CXlsxFlat *pXlsxFlat = dynamic_cast<CXlsxFlat*>(this->m_pMainDocument);
			CXlsb *pXlsb = dynamic_cast<CXlsb*>(this->m_pMainDocument);

			int nBaseRow = (pXlsxFlat || pXlsb )? 0 : 1; // xml/xlsb->xlsx

			writer.WriteString(_T("<c"));
			if (m_oRow.IsInit() && m_oCol.IsInit())
			{
				int nCol = *m_oCol;
				if (nCol < 0 || nCol > 16383)
				{
					nCol = 0;
				}
				writer.WriteString(L" r=\"");
				writer.WriteString(m_aLetters[nCol]);
				writer.AddInt(*m_oRow + nBaseRow);
				writer.WriteString(L"\"");
			}
			else
			{
				WritingStringNullableAttrString(L"r", m_oRef, getRef());
			}
			WritingStringNullableAttrInt(L"s", m_oStyle, *m_oStyle);
			if(m_oType.IsInit() && SimpleTypes::Spreadsheet::celltypeNumber != m_oType->GetValue())
			{
				writer.WriteString(L" t=\"");
				writer.WriteString(m_oType->ToString());
				writer.WriteString(L"\"");
			}
			WritingStringNullableAttrInt(L"cm", m_oCellMetadata, m_oCellMetadata->GetValue());
			WritingStringNullableAttrInt(L"vm", m_oValueMetadata, m_oValueMetadata->GetValue());
			WritingStringNullableAttrBool(L"ph", m_oShowPhonetic);
			if(m_oFormula.IsInit() || m_oRichText.IsInit() || m_oValue.IsInit())
			{
				writer.WriteString(_T(">"));
				if(m_oFormula.IsInit())
					m_oFormula->toXML(writer);
				if(m_oRichText.IsInit())
				{
					writer.WriteString(_T("<is>"));
					m_oRichText->toXML2(writer);
					writer.WriteString(_T("</is>"));
				}
				if(m_oValue.IsInit())
					m_oValue->toXML2(writer, _T("v"));
				writer.WriteString(_T("</c>"));
			}
			else
				writer.WriteString(_T("/>"));
		}
		void CCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if (oReader.IsEmptyNode())
			{
				After2003Read();
				AfterRead();
				return;
			}

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

				if ( strcmp("v", sName) == 0)
					m_oValue = oReader;
				else if (strcmp("Data", sName) == 0)
				{
					CData data(oReader);

					m_oType = data.m_oType;
					m_oValue = data.m_oValue;
					m_oRichText = data.m_oRichText.GetPointerEmptyNullable();
				}
				else if (strcmp("Comment", sName) == 0)
				{
					CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
					if (xlsx_flat)
					{
						pCommentItem.Init();
						ReadComment(oReader, pCommentItem.GetPointer());
					}
				}
				else if ( strcmp("f", sName) == 0 )
					m_oFormula = oReader;
				else if ( strcmp("is", sName) == 0 )
					m_oRichText = oReader;
				else if ( strcmp("NamedCell", sName) == 0 )
				{//дублирование имен
				}
//o:SmartTags, x:PhoneticText
			}
			After2003Read();
			PrepareForBinaryWriter();
			AfterRead();
		}
		void CCell::ReadComment(XmlUtils::CXmlLiteReader& oReader, CCommentItem* pComment)
		{
			if (!pComment) return;

			WritingElement_ReadAttributes_StartChar( oReader )
				WritingElement_ReadAttributes_Read_ifChar ( oReader, "ss:Author", pComment->m_sAuthor )
			WritingElement_ReadAttributes_EndChar( oReader )

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode2( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
				if ( strcmp("Data", sName) == 0)
				{
					CData data_comment;
					data_comment.fromXML2(oReader);
					
					pComment->m_oText = data_comment.m_oRichText.GetPointerEmptyNullable();
				}
			}
		}
		void CCell::After2003Read()
		{
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			if (!xlsx_flat) return;
			
			CWorksheet* sheet = xlsx_flat->m_arWorksheets.back();
			
			std::map<int, std::map<int, unsigned int>>::iterator pFindRow = sheet->m_oSheetData->m_mapStyleMerges2003.find(xlsx_flat->m_nLastReadRow);
			std::map<int, unsigned int>::iterator pFind;

			if (false == iColIndex.IsInit())
			{
				xlsx_flat->m_nLastReadCol = (xlsx_flat->m_nLastReadCol < 0 ? 1 : xlsx_flat->m_nLastReadCol + 1);
			}
			else
			{
				int newCol = *iColIndex;			
				
				if (pFindRow != sheet->m_oSheetData->m_mapStyleMerges2003.end())
				{
					CCell *pCurrentCell = sheet->m_oSheetData->m_arrItems.back()->m_arrItems.back(); // == this
					sheet->m_oSheetData->m_arrItems.back()->m_arrItems.pop_back();

					while (xlsx_flat->m_nLastReadCol < newCol)
					{
						xlsx_flat->m_nLastReadCol = (xlsx_flat->m_nLastReadCol < 0 ? 1 : xlsx_flat->m_nLastReadCol + 1);

						pFind = pFindRow->second.find(xlsx_flat->m_nLastReadCol);
						if (pFind != pFindRow->second.end())
						{
							CCell *pCell = new CCell(this->m_pMainDocument);
							pCell->m_oRef = getCellAddressA(xlsx_flat->m_nLastReadRow, xlsx_flat->m_nLastReadCol);
							pCell->m_oStyle = pFind->second;
							pCell->m_oCol = xlsx_flat->m_nLastReadCol - 1;
							pCell->m_oRow = xlsx_flat->m_nLastReadRow;

							sheet->m_oSheetData->m_arrItems.back()->m_arrItems.push_back(pCell);

							pFindRow->second.erase(pFind);
						}
					}
					sheet->m_oSheetData->m_arrItems.back()->m_arrItems.push_back(pCurrentCell);
					pCurrentCell = NULL;
				}
				xlsx_flat->m_nLastReadCol = newCol;
			}
//---------------------------------------------------------------------------------------
			if (pCommentItem.IsInit())
			{
				pCommentItem->m_nRow = xlsx_flat->m_nLastReadRow - 1;
				pCommentItem->m_nCol = xlsx_flat->m_nLastReadCol - 1;

				std::wstring sId = std::to_wstring(*pCommentItem->m_nRow) + L"-" + std::to_wstring(*pCommentItem->m_nCol);

				sheet->m_mapComments[sId] = pCommentItem.GetPointerEmptyNullable();
			}
//---------------------------------------------------------------------------------------
			if (pFindRow != sheet->m_oSheetData->m_mapStyleMerges2003.end())
			{
				pFind = pFindRow->second.find(xlsx_flat->m_nLastReadCol);
				if (pFind != pFindRow->second.end())
				{
					m_oStyle = pFind->second;
					pFindRow->second.erase(pFind);
				}
			}
//---------------------------------------------------------------------------------------
			//m_oRef = "R" + std::to_string(xlsx_flat->m_nLastReadRow) + "C" + std::to_string(xlsx_flat->m_nLastReadCol);
			m_oRef = getCellAddressA(xlsx_flat->m_nLastReadRow, xlsx_flat->m_nLastReadCol);

			setRowCol(xlsx_flat->m_nLastReadRow, xlsx_flat->m_nLastReadCol - 1);

			if (m_oFormula.IsInit())
			{
				r1c1_formula_convert::base_row = xlsx_flat->m_nLastReadRow;
				r1c1_formula_convert::base_col = xlsx_flat->m_nLastReadCol;

				r1c1_formula_convert convert;

				m_oFormula->m_sText = convert.convert(m_oFormula->m_sText);
			}

			if (sStyleId.IsInit() && xlsx_flat->m_pStyles.IsInit())
			{
				std::map<std::wstring, size_t>::iterator pFind = xlsx_flat->m_pStyles->m_mapStyles2003.find(*sStyleId);
				if (pFind != xlsx_flat->m_pStyles->m_mapStyles2003.end())
				{
					m_oStyle = pFind->second;
				}
			}

			if (sHyperlink.IsInit())
			{
				if (false == sheet->m_oHyperlinks.IsInit())
				{
					sheet->m_oHyperlinks.Init();
				}
				CHyperlink *pHyperlink = new CHyperlink(m_pMainDocument);

				pHyperlink->m_oRef = std::wstring(m_oRef->begin(), m_oRef->end());
				pHyperlink->m_oDisplay = sHyperlink;

				if (sHyperlink->find(L"#") == 0)
				{
					sHyperlink = sHyperlink->substr(1);
					pHyperlink->m_oLocation = sHyperlink;
				}
				else
				{
					pHyperlink->m_oLink = sHyperlink;
				}
				pHyperlink->m_oRid.Init();
				pHyperlink->m_oRid->FromString(sheet->AddHyperlink(*sHyperlink).get());

				sheet->m_oHyperlinks->m_arrItems.push_back(pHyperlink);
			}
//---------------------------------------------------------------------------------------
			if (iAcross.IsInit() || iDown.IsInit())
			{
				//std::string Ref = m_oRef.get2() + ":R" + std::to_string(xlsx_flat->m_nLastReadRow + 1 + iDown.get_value_or(0)) + "C" + std::to_string(xlsx_flat->m_nLastReadCol + 1 + iAcross.get_value_or(0));
				std::string Ref = m_oRef.get2() + ":" + getCellAddressA(xlsx_flat->m_nLastReadRow + iDown.get_value_or(0),
					xlsx_flat->m_nLastReadCol + iAcross.get_value_or(0));

				if (false == sheet->m_oMergeCells.IsInit())
				{
					sheet->m_oMergeCells.Init();
				}
				CMergeCell *pMergeCell = new CMergeCell(m_pMainDocument);
				pMergeCell->m_oRef = std::wstring(Ref.begin(), Ref.end());

				sheet->m_oMergeCells->m_arrItems.push_back(pMergeCell);

				if (m_oStyle.IsInit())
				{
					std::map<unsigned int, bool>::iterator pFindContinues = xlsx_flat->m_pStyles->m_mapStylesContinues2003.find(*m_oStyle);
					if (pFindContinues != xlsx_flat->m_pStyles->m_mapStylesContinues2003.end())
					{
						for (int i = 0; i < iDown.get_value_or(0); ++i)
						{
							std::map<int, std::map<int, unsigned int>>::iterator pFind = sheet->m_oSheetData->m_mapStyleMerges2003.find(xlsx_flat->m_nLastReadRow + i + 1);

							if (pFind == sheet->m_oSheetData->m_mapStyleMerges2003.end())
							{
								std::map<int, unsigned int> mapColsStyle;
								mapColsStyle.insert(std::make_pair(xlsx_flat->m_nLastReadCol, *m_oStyle));
								
								sheet->m_oSheetData->m_mapStyleMerges2003.insert(std::make_pair(xlsx_flat->m_nLastReadRow + i + 1, mapColsStyle));
							}
							else
							{
								pFind->second.insert(std::make_pair(xlsx_flat->m_nLastReadCol, *m_oStyle));
							}
						}

						for (int i = 0; i < iAcross.get_value_or(0); ++i)
						{
							xlsx_flat->m_nLastReadCol++;
							
							CCell *pCell = new CCell(this->m_pMainDocument);
							pCell->m_oRef = getCellAddressA(xlsx_flat->m_nLastReadRow, xlsx_flat->m_nLastReadCol);
							pCell->m_oStyle = pFindContinues->first;
							pCell->m_oCol = xlsx_flat->m_nLastReadCol - 1;
							pCell->m_oRow = xlsx_flat->m_nLastReadRow;
							
							sheet->m_oSheetData->m_arrItems.back()->m_arrItems.push_back(pCell);
						}
					}
					else
						xlsx_flat->m_nLastReadCol += iAcross.get_value_or(0);
				}
				else
					xlsx_flat->m_nLastReadCol += iAcross.get_value_or(0);
			}
		}
		void CCell::PrepareForBinaryWriter()
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(m_pMainDocument);
			if (!xlsx) return;

			//for xml with empty cell reference
			int nRow = 0;
			int nCol = 0;
			getRowCol(nRow, nCol);
			xlsx->m_nLastReadCol = nCol > xlsx->m_nLastReadCol ? nCol : xlsx->m_nLastReadCol + 1;
			setRowCol(xlsx->m_nLastReadRow, xlsx->m_nLastReadCol);
		}
		void CCell::AfterRead()
		{
			CSharedStrings *pSharedStrings = NULL;
			
			CXlsx* xlsx = dynamic_cast<CXlsx*>(m_pMainDocument);
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			
			if (xlsx)
			{
				if (false == xlsx->m_arWorksheets.back()->m_bPrepareForBinaryWriter) return;

				if (!xlsx->m_pSharedStrings)
				{	// еще не прочитался rels
					xlsx->m_arWorksheets.back()->m_bPrepareForBinaryWriter = false;
					return;
				}

				pSharedStrings = xlsx->m_pSharedStrings;
			}
			else if (xlsx_flat)
			{
				if (false == xlsx_flat->m_pSharedStrings.IsInit())
				{
					xlsx_flat->m_pSharedStrings = new CSharedStrings(m_pMainDocument);
				}
				pSharedStrings = xlsx_flat->m_pSharedStrings.GetPointer();
			}

			if (!pSharedStrings) return;
//-----------------------------------------------------------------------------
			if (m_oType.IsInit())
			{
				if (m_oRichText.IsInit()/*SimpleTypes::Spreadsheet::celltypeInlineStr == m_oType->GetValue()*/)
				{
					OOX::Spreadsheet::CSi* pSi = m_oRichText.GetPointerEmptyNullable();
					if (NULL != pSi)
					{
						int nIndex = pSharedStrings->AddSi(pSi);
						//меняем значение ячейки
						m_oValue.Init();
						m_oValue->m_sText = std::to_wstring(nIndex);
						//меняем тип ячейки
						m_oType.Init();
						m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
					}
				}
				else if (SimpleTypes::Spreadsheet::celltypeStr == m_oType->GetValue() || SimpleTypes::Spreadsheet::celltypeError == m_oType->GetValue())
				{
					if (m_oValue.IsInit())
					{
						if (xlsx && !xlsx->m_pSharedStrings)
						{
							xlsx->CreateSharedStrings();
							pSharedStrings = xlsx->m_pSharedStrings;
						}
						//добавляем в SharedStrings
						CSi* pSi = new CSi();
						CText* pText = new CText();

						pText->m_sText = m_oValue->ToString();
						pSi->m_arrItems.push_back(pText);

						int nIndex = pSharedStrings->AddSi(pSi);

						//меняем значение ячейки
						m_oValue.Init();
						m_oValue->m_sText = std::to_wstring(nIndex);
						//меняем тип ячейки
						if (SimpleTypes::Spreadsheet::celltypeStr == m_oType->GetValue())
						{
							m_oType.Init();
							m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
						}
					}
					else
					{
						m_oValue.reset();
						m_oType.reset();
					}
				}
				else if (SimpleTypes::Spreadsheet::celltypeBool == m_oType->GetValue())
				{
					//обычно пишется 1/0, но встречается, что пишут true/false
					if (m_oValue.IsInit())
					{
						SimpleTypes::COnOff oOnOff;
						std::wstring sVal = m_oValue->ToString();
						oOnOff.FromString(sVal.c_str());
						m_oValue.Init();
						if (oOnOff.ToBool())
							m_oValue->m_sText = _T("1");
						else
							m_oValue->m_sText = _T("0");
					}
				}
			}
		}
		void CCell::fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, _UINT32 nRow)
		{
			LONG nEnd = oStream.XlsbReadRecordLength() + oStream.GetPos();

			m_oRow = nRow;
			m_oCol = (oStream.GetULong() & 0x3FFF);
			_UINT32 nStyleRef = oStream.GetULong();
			if(0 != (nStyleRef & 0xFFFFFF))
			{
				m_oStyle = (nStyleRef & 0xFFFFFF);
			}

			if(0 != (nStyleRef & 0x1000000))
			{
				m_oShowPhonetic.Init();
				m_oShowPhonetic->FromBool(true);
			}

            if (XLSB::rt_CellRk == nType)
			{
				m_oValue.Init();
				m_oValue->m_sText = std::to_wstring(oStream.GetULong());
			}
            else if (XLSB::rt_CellError == nType || XLSB::rt_FmlaError == nType)
			{
				m_oType.Init();
				m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeError);
				BYTE nError = oStream.GetUChar();
				m_oValue.Init();
				switch(nError)
				{
                    case 0x00: m_oValue->m_sText = L"#NULL!"; break;
                    case 0x07: m_oValue->m_sText = L"#DIV/0!"; break;
                    case 0x0F: m_oValue->m_sText = L"#VALUE!"; break;
                    case 0x17: m_oValue->m_sText = L"#REF!"; break;
                    case 0x1D: m_oValue->m_sText = L"#NAME?"; break;
                    case 0x24: m_oValue->m_sText = L"#NUM!"; break;
                    case 0x2A: m_oValue->m_sText = L"#N/A"; break;
                    case 0x2B: m_oValue->m_sText = L"#GETTING_DATA"; break;
				}
			}
            else if (XLSB::rt_CellBool == nType || XLSB::rt_FmlaBool == nType)
			{
				m_oType.Init();
				m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeBool);
				m_oValue.Init();
				m_oValue->m_sText = oStream.GetBool() ? L"1" : L"0";
			}
            else if (XLSB::rt_CellReal == nType || XLSB::rt_FmlaNum == nType)
			{
				m_oValue.Init();
				m_oValue->m_sText = OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(oStream.GetDoubleReal());
			}
            else if (XLSB::rt_CellSt == nType || XLSB::rt_FmlaString == nType)
			{
				m_oType.Init();
                m_oType->SetValue(XLSB::rt_CellSt == nType ? SimpleTypes::Spreadsheet::celltypeInlineStr : SimpleTypes::Spreadsheet::celltypeStr);
				m_oValue.Init();
				m_oValue->m_sText = oStream.GetString2();
			}
            else if (XLSB::rt_CellIsst == nType)
			{
				m_oType.Init();
				m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
				m_oValue.Init();
				m_oValue->m_sText = std::to_wstring(oStream.GetULong());
			}


            if (XLSB::rt_FmlaString <= nType && nType <= XLSB::rt_FmlaError)
			{
				m_oFormula.Init();
				m_oFormula->fromXLSB(oStream);
			}
			//todo it breaks xslb format
			_UINT16 nFlags = oStream.GetUShort();
			if(0 != (nFlags & 0x4))
			{
				if(!m_oFormula.IsInit())
				{
					m_oFormula.Init();
				}
				m_oFormula->fromXLSBExt(oStream, nFlags);
				if(0 != (nFlags & 0x4000))
				{
					m_oType.reset(NULL);
					m_oValue.reset(NULL);
				}
			}
			if(0 != (nFlags & 0x2000))
			{
				m_oType.Init();
				m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);

				m_oRichText.Init();
				m_oRichText->fromXLSBExt(oStream);
			}

			oStream.Seek(nEnd);
		}
        void CCell::fromBin(XLS::BaseObjectPtr& obj)
        {
            ReadAttributes(obj);           
        }
		void CCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )

				if (strcmp("r", wsName) == 0)
				{
					m_oRef = oReader.GetTextA();
				}
				else if (strcmp("ss:Formula", wsName) == 0)
				{
					m_oFormula.Init();
					m_oFormula->m_sText = oReader.GetText();
					m_oFormula->m_sText = m_oFormula->m_sText.substr(1);
					//convert R1C1 to ..
				}
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:Index", iColIndex )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:MergeAcross", iAcross )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:MergeDown", iDown )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:ArrayRange", sArrayRange )		
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:StyleID", sStyleId )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ss:HRef", sHyperlink )

				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "s", m_oStyle )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "t", m_oType )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "cm", m_oCellMetadata )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "vm", m_oValueMetadata )
				WritingElement_ReadAttributes_Read_else_ifChar ( oReader, "ph", m_oShowPhonetic )

			WritingElement_ReadAttributes_EndChar( oReader )

		}
        void CCell::ReadAttributes(BaseObjectPtr& obj)
        {
            auto ptr = static_cast<XLSB::CELL*>(obj.get());
            if(ptr != nullptr)
            {
                m_oRow = ptr->m_Row;
                auto pCELLMETA = static_cast<XLSB::CELLMETA*>(ptr->m_CELLMETA.get());
                if(pCELLMETA != nullptr)
                {
                    auto pCellMeta = static_cast<XLSB::CellMeta*>(pCELLMETA->m_BrtCellMeta.get());
                    if(pCellMeta != nullptr && pCellMeta->icmb)
                        m_oCellMetadata = pCellMeta->icmb;

                    auto pValueMeta = static_cast<XLSB::ValueMeta*>(pCELLMETA->m_BrtValueMeta.get());
                    if(pValueMeta != nullptr && pValueMeta->ivmb)
                        m_oValueMetadata = pValueMeta->ivmb;
                }

                XLSB::DATACELL* pDATACELL = nullptr;
                XLSB::TABLECELL* pTABLECELL = nullptr;
                XLSB::FMLACELL* pFMLACELL = nullptr;
                XLSB::SHRFMLACELL* pSHRFMLACELL = nullptr;

                switch(ptr->m_source->get_type())
                {
                    case XLS::typeDATACELL:
                        pDATACELL = static_cast<XLSB::DATACELL*>(ptr->m_source.get());
                    break;
                    case XLS::typeTABLECELL:
                        pTABLECELL = static_cast<XLSB::TABLECELL*>(ptr->m_source.get());
                    break;
                    case XLS::typeFMLACELL:
                        pFMLACELL = static_cast<XLSB::FMLACELL*>(ptr->m_source.get());
                    break;
                    case XLS::typeSHRFMLACELL:
                        pSHRFMLACELL = static_cast<XLSB::SHRFMLACELL*>(ptr->m_source.get());
                        pFMLACELL = static_cast<XLSB::FMLACELL*>(pSHRFMLACELL->_fmlacell.get());
                    break;
                }


                if(pDATACELL != nullptr || pTABLECELL != nullptr || pFMLACELL != nullptr)
                {
                    BiffRecord* pSource = nullptr;
                    XLSB::Cell oCell;
                    if(pDATACELL != nullptr)
                    {                                     
                        m_oCol = pDATACELL->m_Col;
                        pSource = static_cast<BiffRecord*>(pDATACELL->m_source.get());
                        if(pSource != nullptr)
                            oCell = dynamic_cast<XLSB::CellBase*>(pSource)->cell;
                    }
                    else if(pTABLECELL != nullptr)
                    {
                        m_oCol = pTABLECELL->m_Col;
                        pSource = static_cast<BiffRecord*>(pTABLECELL->m_source.get());
                        if(pSource != nullptr)
                            oCell = dynamic_cast<XLSB::CellBase*>(pSource)->cell;
                        if(pTABLECELL->m_BrtTable != nullptr)
                        {
                            m_oFormula.Init();
                            m_oFormula->fromBin(pTABLECELL->m_BrtTable, SimpleTypes::Spreadsheet::cellformulatypeDataTable);
                        }
                    }
                    else if(pFMLACELL != nullptr)
                    {
                        m_oCol = pFMLACELL->m_Col;                       
                        pSource = static_cast<BiffRecord*>(pFMLACELL->m_source.get());
                        if(pSource != nullptr)
                            oCell = dynamic_cast<XLSB::FmlaBase*>(pSource)->cell;
                        if(pFMLACELL->m_source != nullptr)
                        {
                            m_oFormula.Init();
                            m_oFormula->fromBin(pFMLACELL->m_source, SimpleTypes::Spreadsheet::cellformulatypeNormal);
							if (pFMLACELL->isShared)
							{
								m_oFormula->m_oSi = pFMLACELL->m_sharedIndex;
								m_oFormula->m_oT.Init();
								m_oFormula->m_oT->SetValue(SimpleTypes::Spreadsheet::cellformulatypeShared);
							}
                        }

                        if(pSHRFMLACELL != nullptr)
                        {
                            if(pSHRFMLACELL->m_source != nullptr)
                            {
                                m_oFormula.Init();
                                if(static_cast<BiffRecord*>(pSHRFMLACELL->m_source.get())->getTypeId() == XLSB::rt_ArrFmla)
                                {
                                    m_oFormula->fromBin(pSHRFMLACELL->m_source, SimpleTypes::Spreadsheet::cellformulatypeArray);
                                }
                                else if(static_cast<BiffRecord*>(pSHRFMLACELL->m_source.get())->getTypeId() == XLSB::rt_ShrFmla)
                                {
                                    m_oFormula->m_oSi = pSHRFMLACELL->m_sharedIndex;
                                    m_oFormula->fromBin(pSHRFMLACELL->m_source, SimpleTypes::Spreadsheet::cellformulatypeShared);
                                }
                            }

                        }

                    }

                    auto wRef = XLSB::RgceLoc(m_oRow.get(), m_oCol.get(), true, true).toString();
                    m_oRef = std::string(wRef.begin(), wRef.end());

                    if(pSource != nullptr)
                    {
                        if(oCell.fPhShow)
                            m_oShowPhonetic = oCell.fPhShow;
                        if(oCell.iStyleRef)
                            m_oStyle        = oCell.iStyleRef;
                        auto nType = pSource->getTypeId();
                        switch (nType)
                        {
                            case XLSB::rt_CellBlank:
                                break;
                            case XLSB::rt_CellRk:
                                {
                                    auto pCellRk = static_cast<XLSB::CellRk*>(pSource);
                                    //m_oType.Init();
                                    //m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeNumber);
                                    m_oValue.Init();
                                    m_oValue->m_sText         = pCellRk->value.value();
                                }
                                break;
                            case XLSB::rt_CellError:
                            case XLSB::rt_FmlaError:
                                {
                                    auto pError = XLSB::rt_CellError == nType ? static_cast<XLSB::CellError*>(pSource)->value : static_cast<XLSB::FmlaError*>(pSource)->value;
                                    m_oType.Init();
                                    m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeError);
                                    m_oValue.Init();
                                    switch(pError)
                                    {
                                        case 0x00: m_oValue->m_sText = L"#NULL!"; break;
                                        case 0x07: m_oValue->m_sText = L"#DIV/0!"; break;
                                        case 0x0F: m_oValue->m_sText = L"#VALUE!"; break;
                                        case 0x17: m_oValue->m_sText = L"#REF!"; break;
                                        case 0x1D: m_oValue->m_sText = L"#NAME?"; break;
                                        case 0x24: m_oValue->m_sText = L"#NUM!"; break;
                                        case 0x2A: m_oValue->m_sText = L"#N/A"; break;
                                        case 0x2B: m_oValue->m_sText = L"#GETTING_DATA"; break;
                                    }
                                }
                                break;
                            case XLSB::rt_CellBool:
                            case XLSB::rt_FmlaBool:
                                {
                                    auto pBool = XLSB::rt_CellBool == nType ? static_cast<XLSB::CellBool*>(pSource)->value : static_cast<XLSB::FmlaBool*>(pSource)->value;
                                    m_oType.Init();
                                    m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeBool);
                                    m_oValue.Init();
                                    m_oValue->m_sText = pBool ? L"1" : L"0";
                                }
                               break;
                            case XLSB::rt_CellReal:
                            case XLSB::rt_FmlaNum:
                                {
                                    //std::wstring str;
                                    auto pRealNum = XLSB::rt_CellReal == nType ? static_cast<XLSB::CellReal*>(pSource)->value.data.value : static_cast<XLSB::FmlaNum*>(pSource)->value.data.value;
                                    if(XLSB::rt_FmlaNum == nType)
                                    {
                                        //str = static_cast<XLSB::FmlaNum*>(pSource)->formula.getAssembledFormula();
                                    }
                                    //m_oType.Init();
                                    //m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeNumber);
                                    m_oValue.Init();
                                    m_oValue->m_sText = OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(pRealNum);
                                }
                                break;
                            case XLSB::rt_CellIsst:
                                {
                                    auto pCellIsst = static_cast<XLSB::CellIsst*>(pSource);
                                    m_oType.Init();
                                    m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
                                    m_oValue.Init();
                                    m_oValue->m_sText = std::to_wstring(pCellIsst->value);
                                }
                                break;
                            case XLSB::rt_CellSt:
                            case XLSB::rt_FmlaString:
                                {
                                    std::wstring pSt = L"";
                                    if(!(XLSB::rt_CellSt == nType ? static_cast<XLSB::CellSt*>(pSource)->value.value() : static_cast<XLSB::FmlaString*>(pSource)->value.value()).empty())
                                         pSt = XLSB::rt_CellSt == nType ? static_cast<XLSB::CellSt*>(pSource)->value.value() : static_cast<XLSB::FmlaString*>(pSource)->value.value();
                                    m_oType.Init();
                                    m_oType->SetValue(XLSB::rt_CellSt == nType ? SimpleTypes::Spreadsheet::celltypeInlineStr : SimpleTypes::Spreadsheet::celltypeStr);
                                    m_oValue.Init();
                                    m_oValue->m_sText = pSt;
                                }
                               break;
                        }
                    }
                }
            } 
        }
		EElementType CCell::getType () const
		{
			return et_x_Cell;
		}
		bool CCell::isInitRef() const
		{
			return m_oRef.IsInit() || (m_oRow.IsInit() && m_oCol.IsInit());
		}
		std::wstring CCell::getRef() const
		{
			if (m_oRef.IsInit())
			{
				const std::string& s = m_oRef.get();
				return std::wstring(s.begin(), s.end());
			}
			else if (m_oRow.IsInit() && m_oCol.IsInit())
			{
				return combineRef(*m_oRow, *m_oCol);
			}
			else
			{
				return L"A1";
			}
		}
		void CCell::setRef(const std::wstring& sRef)
		{
			m_oRef = std::string(sRef.begin(), sRef.end());
		}
		bool CCell::getRowCol(int& nRow, int& nCol) const
		{
			bool bRes = false;
			nRow = 0;
			nCol = 0;
			if (m_oRow.IsInit() && m_oCol.IsInit())
			{
				bRes = true;
				nRow = *m_oRow;
				nCol = *m_oCol;
			}
			else if (m_oRef.IsInit())
			{
				if (parseRefA(m_oRef->c_str(), nRow, nCol))
				{
					bRes = true;
					nRow--;
					nCol--;
				}
			}
			return bRes;
		}
		void CCell::setRowCol(int nRow, int nCol)
		{
			m_oRow = nRow;
			m_oCol= nCol;
		}
		bool CCell::parse3DRef(const std::wstring& sRef, std::wstring& workbook, std::wstring& sheetFrom, std::wstring& sheetTo, int& nRow1, int& nCol1, int& nRow2, int& nCol2)
		{
			bool bRes = false;
			int nIndex = (int)sRef.find('!');
			std::wstring sCellRef;

			if (std::wstring::npos != nIndex)
			{
				std::wstring sSheetPrefix = sRef.substr(0, nIndex);
				if (sSheetPrefix.length() > 0 && '\'' == sSheetPrefix[0] && '\'' == sSheetPrefix[sSheetPrefix.length() - 1])
				{
					sSheetPrefix = sSheetPrefix.substr(1, sSheetPrefix.length() - 2);
				}
				NSStringExt::Replace(sSheetPrefix, L"''", L"'");

				int nIndexWbStart = (int)sSheetPrefix.find('[');
				int nIndexWbEnd = (int)sSheetPrefix.find(']');

				if (-1 != nIndexWbStart && -1 != nIndexWbEnd)
				{
					workbook = sSheetPrefix.substr(nIndexWbStart + 1, nIndexWbEnd - nIndexWbStart - 1);
					sSheetPrefix = sSheetPrefix.substr(nIndexWbEnd + 1, sSheetPrefix.length() - nIndexWbEnd - 1);
				}
				int nIndexColon = (int)sSheetPrefix.find(':');
				if (-1 != nIndexColon)
				{
					sheetFrom = sSheetPrefix.substr(0, nIndexColon);
					sheetTo = sSheetPrefix.substr(nIndexColon + 1, sSheetPrefix.length() - nIndexColon - 1);
				}
				else
				{
					sheetFrom = sSheetPrefix;
				}
				sCellRef = sRef.substr(nIndex + 1, sRef.length() - nIndex - 1);
			}
			else
			{
				sCellRef = sRef;
			}
			NSStringExt::Replace(sCellRef, L"$", L"");
			int nIndexColon = (int)sCellRef.find(':');
			if (std::wstring::npos != nIndexColon)
			{
				bRes = parseRef(sCellRef.substr(0, nIndexColon), nRow1, nCol1) && parseRef(sCellRef.substr(nIndexColon + 1, sCellRef.length() - nIndexColon - 1), nRow2, nCol2);
			}
			else
			{
				bRes = parseRef(sCellRef, nRow1, nCol1);
				nRow2 = nRow1;
				nCol2 = nCol1;
			}
			return bRes;
		}

		CData::CData()
		{
		}
		CData::~CData()
		{
		}
		void CData::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CData::toXML() const
		{
			return L"";
		}
		void CData::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		EElementType CData::getType () const
		{
			return et_x_Data;
		}
		void CData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 ) return;

			WritingElement_ReadAttributes_StartChar( oReader )
				WritingElement_ReadAttributes_Read_ifChar ( oReader, "ss:Type", m_oType )
			WritingElement_ReadAttributes_EndChar( oReader )
		}
		void CData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if(SimpleTypes::Spreadsheet::celltypeStr != m_oType->GetValue())
			{
				m_oValue = oReader;
				if (SimpleTypes::Spreadsheet::celltypeDate == m_oType->GetValue())
				{
					double value = 0;
					if (parseDate(m_oValue->m_sText, value))
					{
						m_oValue->m_sText = std::to_wstring(value);
						m_oType.reset(); // по стилю
					}
				}
				return;
			}
			fromXML2(oReader);
		}
		void CData::dump(const std::wstring &text)
		{
			if (text.empty()) return;

			if (false == m_oRichText.IsInit())
				m_oRichText.Init();

			CText *pText = new CText();
			pText->m_sText = text;

			CRun *pRun = new CRun();
			pRun->m_arrItems.push_back(pText);

			if (bBold.IsInit() || bItalic.IsInit() || bUnderline.IsInit()|| nFontSize.IsInit() || bSubscript.IsInit() || bSuperscript.IsInit() ||
				(sColor.IsInit() && *sColor != L"#333333"))
			{
				pRun->m_oRPr = new CRPr();

				if (bBold.IsInit())
				{
					pRun->m_oRPr->m_oBold.Init();
					pRun->m_oRPr->m_oBold->m_oVal.FromBool(*bBold);
				}
				if (bItalic.IsInit())
				{
					pRun->m_oRPr->m_oItalic.Init();
					pRun->m_oRPr->m_oItalic->m_oVal.FromBool(*bItalic);
				}
				if (bUnderline.IsInit())
				{
					pRun->m_oRPr->m_oUnderline.Init(); pRun->m_oRPr->m_oUnderline->m_oUnderline.Init();
					pRun->m_oRPr->m_oUnderline->m_oUnderline->SetValue(SimpleTypes::Spreadsheet::underlineSingle);
				}
				if (sColor.IsInit())
				{
					pRun->m_oRPr->m_oColor.Init();
					pRun->m_oRPr->m_oColor->m_oRgb = new SimpleTypes::Spreadsheet::CHexColor(*sColor);
				}
				if (nFontSize.IsInit())
				{
					pRun->m_oRPr->m_oSz.Init(); pRun->m_oRPr->m_oSz->m_oVal.Init();
					pRun->m_oRPr->m_oSz->m_oVal->SetValue(*nFontSize);
				}
				if (bSubscript.IsInit() || bSuperscript.IsInit())
				{
					pRun->m_oRPr->m_oVertAlign.Init(); pRun->m_oRPr->m_oVertAlign->m_oVerticalAlign.Init();
					if (bSubscript.IsInit())	pRun->m_oRPr->m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::verticalalignrunSubscript);
					if (bSuperscript.IsInit())	pRun->m_oRPr->m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::verticalalignrunSuperscript);
				}
			}
			m_oRichText->m_arrItems.push_back(pRun);
		}
		void CData::fromXML2(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode2( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

				if ( strcmp("B", sName) == 0)
				{
					bBold = true;
					fromXML2(oReader);

					//dump(oReader.GetText());
					bBold.reset();
				}
				else if ( strcmp("I", sName) == 0)
				{
					bItalic = true;
					fromXML2(oReader);

					//dump(oReader.GetText());
					bItalic.reset();
				}
				else if ( strcmp("U", sName) == 0)
				{
					bUnderline = true;
					fromXML2(oReader);

					//dump(oReader.GetText());
					bUnderline.reset();
				}
				else if ( strcmp("Sub", sName) == 0)
				{
					bSubscript = true;
					fromXML2(oReader);

					//dump(oReader.GetText());
					bSubscript.reset();
				}
				else if ( strcmp("Sup", sName) == 0)
				{
					bSuperscript  = true;
					fromXML2(oReader);

					//dump(oReader.GetText());
					bSuperscript.reset();
				}
				else if ( strcmp("Font", sName) == 0)
				{
					WritingElement_ReadAttributes_Start_No_NS( oReader )
						WritingElement_ReadAttributes_Read_if ( oReader, L"Color",	sColor )
						WritingElement_ReadAttributes_Read_if ( oReader, L"Size",	nFontSize )
					WritingElement_ReadAttributes_End_No_NS( oReader )

					fromXML2(oReader);

					//dump(oReader.GetText());
					sColor.reset();
					nFontSize.reset();
				}
				else if (strcmp("#text", sName) == 0)
				{
					dump(oReader.GetText());
				}
			}
		}

		CRow::CRow(OOX::Document *pMain) : WritingElementWithChilds<CCell>(pMain)
		{
		}
		CRow::~CRow()
		{
		}
		void CRow::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRow::toXML() const
		{
			return _T("");
		}
		void CRow::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLStart(writer);

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			toXMLEnd(writer);
		}
		void CRow::toXMLStart(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<row"));
				WritingStringNullableAttrInt(L"r", m_oR, m_oR->GetValue());
				WritingStringNullableAttrInt(L"s", m_oS, m_oS->GetValue());
				WritingStringNullableAttrBool(L"customFormat", m_oCustomFormat);
				WritingStringNullableAttrDouble(L"ht", m_oHt, m_oHt->GetValue());
				WritingStringNullableAttrBool(L"hidden", m_oHidden);
				WritingStringNullableAttrBool(L"customHeight", m_oCustomHeight);
				WritingStringNullableAttrInt(L"outlineLevel", m_oOutlineLevel, m_oOutlineLevel->GetValue());
				WritingStringNullableAttrBool(L"collapsed", m_oCollapsed);
				WritingStringNullableAttrBool(L"thickTop", m_oThickTop);
				WritingStringNullableAttrBool(L"thickBot", m_oThickBot);
				WritingStringNullableAttrBool(L"ph", m_oPh);
			writer.WriteString(_T(">"));
		}
		void CRow::toXMLEnd(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("</row>"));
		}
		void CRow::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			CheckIndex();

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

				if ( strcmp("c", sName) == 0 || strcmp("Cell", sName) == 0)
				{
					CCell *pCell = new CCell(m_pMainDocument);
					if (pCell)
					{
						m_arrItems.push_back(pCell);
						pCell->fromXML(oReader);
					}
				}
			}
//----------------- 2003
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);

			if (xlsx_flat)
			{
				CWorksheet* sheet = xlsx_flat->m_arWorksheets.back();
				
				std::map<int, std::map<int, unsigned int>>::iterator pFind = sheet->m_oSheetData->m_mapStyleMerges2003.find(xlsx_flat->m_nLastReadRow);

				if (pFind != sheet->m_oSheetData->m_mapStyleMerges2003.end())
				{
					sheet->m_oSheetData->StyleFromMapStyleMerges2003(pFind->second);
					sheet->m_oSheetData->m_mapStyleMerges2003.erase(pFind);
				}
			}
		}
        void CRow::fromBin(XLS::BaseObjectPtr& obj)
        {
            ReadAttributes(obj);

            auto ptr = static_cast<XLSB::Parenthesis_CELLTABLE*>(obj.get());

            for (auto it = ptr->m_arCELL.begin(); it != ptr->m_arCELL.end();)
            {
              CCell *pCell = new CCell(m_pMainDocument);
              pCell->fromBin(*it);
              m_arrItems.push_back(pCell);

              it = ptr->m_arCELL.erase(it);
            }

            /*for(auto &CELL : ptr->m_arCELL)
            {
                CCell *pCell = new CCell(m_pMainDocument);
                pCell->fromBin(CELL);

                m_arrItems.push_back(pCell);
            }*/
        }
		void CRow::fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType)
		{
			LONG nEnd = oStream.XlsbReadRecordLength() + oStream.GetPos();

			m_oR.Init();
			m_oR->SetValue((oStream.GetULong() & 0xFFFFF) + 1);
			_UINT32 nS = oStream.GetULong();
			_UINT16 nHt = oStream.GetUShort() & 0x1FFF;
			BYTE byteExtra1 = oStream.GetUChar();
			if(0 != (byteExtra1 & 0x1))
			{
				m_oThickTop.Init();
				m_oThickTop->FromBool(true);
			}
			if(0 != (byteExtra1 & 0x2))
			{
				m_oThickBot.Init();
				m_oThickBot->FromBool(true);
			}
			BYTE byteExtra2 = oStream.GetUChar();
			BYTE nOutlineLevel = byteExtra2 & 0x7;
			if(nOutlineLevel > 0)
			{
				m_oOutlineLevel.Init();
				m_oOutlineLevel->SetValue(nOutlineLevel);
			}
			if(0 != (byteExtra2 & 0x8))
			{
				m_oCollapsed.Init();
				m_oCollapsed->FromBool(true);
			}
			if(0 != (byteExtra2 & 0x10))
			{
				m_oHidden.Init();
				m_oHidden->FromBool(true);
			}
			if(0 != (byteExtra2 & 0x20))
			{
				m_oCustomHeight.Init();
				m_oCustomHeight->FromBool(true);
			}
			if(0 != nHt || (m_oCustomHeight.IsInit() && m_oCustomHeight->ToBool()))
			{
				m_oHt.Init();
				m_oHt->SetValue(((double)nHt) / 20);//twips to pt
			}
			if(0 != (byteExtra2 & 0x40))
			{
				m_oCustomFormat.Init();
				m_oCustomFormat->FromBool(true);
			}
			if(m_oCustomFormat.IsInit() && m_oCustomFormat->ToBool())
			{
				m_oS.Init();
				m_oS->SetValue(nS);
			}
			BYTE byteExtra3 = oStream.GetUChar();
			if(0 != (byteExtra3 & 0x1))
			{
				m_oPh.Init();
				m_oPh->FromBool(true);
			}
			//todo ccolspan
			oStream.Seek(nEnd);
		}
		void CRow::CheckIndex()
		{
			CXlsx* xlsx = dynamic_cast<CXlsx*>(m_pMainDocument);
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			if (xlsx)
			{
				if (!m_oR.IsInit() || m_oR->GetValue() <= xlsx->m_nLastReadRow)
				{
					xlsx->m_nLastReadRow = xlsx->m_nLastReadRow + 1;
					m_oR.Init();
					m_oR->SetValue(xlsx->m_nLastReadRow);
				}
				else
				{
					xlsx->m_nLastReadRow = m_oR->GetValue();
				}
				xlsx->m_nLastReadCol = -1;
			}
			else if (xlsx_flat)
			{
				CWorksheet* sheet = xlsx_flat->m_arWorksheets.back();

				if (!m_oR.IsInit() || m_oR->GetValue() <= xlsx_flat->m_nLastReadRow)
				{
					xlsx_flat->m_nLastReadRow = xlsx_flat->m_nLastReadRow + 1;
					m_oR.Init();
					m_oR->SetValue(xlsx_flat->m_nLastReadRow);
				}
				else
				{
					unsigned int newRow = m_oR->GetValue();

					if (false == sheet->m_oSheetData->m_mapStyleMerges2003.empty())
					{
						CRow *pCurrentRow = sheet->m_oSheetData->m_arrItems.back(); // == this
						sheet->m_oSheetData->m_arrItems.pop_back();
						
						while (xlsx_flat->m_nLastReadRow < newRow)
						{
							xlsx_flat->m_nLastReadRow++;
							
							CRow *pRow = new CRow(m_pMainDocument);
							pRow->m_oR = xlsx_flat->m_nLastReadRow;

							sheet->m_oSheetData->m_arrItems.push_back(pRow);

							std::map<int, std::map<int, unsigned int>>::iterator pFind = sheet->m_oSheetData->m_mapStyleMerges2003.find(xlsx_flat->m_nLastReadRow);
							
							if (pFind != sheet->m_oSheetData->m_mapStyleMerges2003.end())
							{
								sheet->m_oSheetData->StyleFromMapStyleMerges2003(pFind->second);
								sheet->m_oSheetData->m_mapStyleMerges2003.erase(pFind);
							}
						}
						sheet->m_oSheetData->m_arrItems.push_back(pCurrentRow);
						pCurrentRow = NULL;
					}					
					xlsx_flat->m_nLastReadRow = newRow;
				}
				xlsx_flat->m_nLastReadCol = -1;
			}
		}
		void CRow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar( oReader )
				if ( strcmp("r", wsName) == 0 || strcmp("ss:Index", wsName) == 0)\
				{
					m_oR.Init();
					m_oR->SetValue(atoi(oReader.GetTextChar()));
				}
				else if ( strcmp("s", wsName) == 0 )\
				{
					m_oS.Init();
					m_oS->SetValue(atoi(oReader.GetTextChar()));
				}
				else if ( strcmp("customFormat", wsName) == 0 )\
				{
					m_oCustomFormat.Init();
					m_oCustomFormat->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("ht", wsName) == 0)
				{
					m_oHt.Init();
					m_oHt->SetValue(atof(oReader.GetTextChar()));
				}
				else if (strcmp("ss:Height", wsName) == 0 )
				{
					m_oHt.Init();
					m_oHt->SetValue(atof(oReader.GetTextChar()));
					
					m_oCustomHeight.Init();
					m_oCustomHeight->SetValue(SimpleTypes::onoffTrue);
				}
				else if ( strcmp("hidden", wsName) == 0 )\
				{
					m_oHidden.Init();
					m_oHidden->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("customHeight", wsName) == 0 )\
				{
					m_oCustomHeight.Init();
					m_oCustomHeight->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("outlineLevel", wsName) == 0 )\
				{
					m_oOutlineLevel.Init();
					m_oOutlineLevel->SetValue(atoi(oReader.GetTextChar()));
				}
				else if ( strcmp("collapsed", wsName) == 0 )\
				{
					m_oCollapsed.Init();
					m_oCollapsed->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("x14ac:dyDescent", wsName) == 0 )\
				{
					m_oDyDescent.Init();
					m_oDyDescent->SetValue(atof(oReader.GetTextChar()));
				}
				else if ( strcmp("thickBot", wsName) == 0 )\
				{
					m_oThickBot.Init();
					m_oThickBot->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("thickTop", wsName) == 0 )\
				{
					m_oThickTop.Init();
					m_oThickTop->FromStringA(oReader.GetTextChar());
				}
				else if ( strcmp("ph", wsName) == 0 )\
				{
					m_oPh.Init();
					m_oPh->FromStringA(oReader.GetTextChar());
				}
			WritingElement_ReadAttributes_EndChar( oReader )
		}
        void CRow::ReadAttributes(XLS::BaseObjectPtr& obj)
        {
            auto ptr = static_cast<XLSB::Parenthesis_CELLTABLE*>(obj.get());
            if(ptr != nullptr)
            {
                auto ptrRowHdr = static_cast<XLSB::RowHdr*>(ptr->m_BrtRowHdr.get());
                if(ptrRowHdr != nullptr)
                {
                    if(ptrRowHdr->fCollapsed)
                        m_oCollapsed                = ptrRowHdr->fCollapsed;

                    if(ptrRowHdr->fGhostDirty)
                        m_oCustomFormat             = ptrRowHdr->fGhostDirty;

                    if(ptrRowHdr->fUnsynced)
                        m_oCustomHeight             = ptrRowHdr->fUnsynced;

                    if(ptrRowHdr->fDyZero)
                        m_oHidden                   = ptrRowHdr->fDyZero;

                    m_oHt                           = ptrRowHdr->miyRw/20.;

                    if(ptrRowHdr->iOutLevel)
                        m_oOutlineLevel             = ptrRowHdr->iOutLevel;

                    if(ptrRowHdr->fPhonetic)
                        m_oPh                       = ptrRowHdr->fPhonetic;

                    m_oR                            = ptrRowHdr->rw + 1;

                    if(ptrRowHdr->ixfe_val)
                        m_oS                        = ptrRowHdr->ixfe_val;

                    if(ptrRowHdr->ixfe_val)
                        m_oThickBot                 = ptrRowHdr->fExDes;

                    if(ptrRowHdr->ixfe_val)
                        m_oThickTop                 = ptrRowHdr->fExAsc;
                }


                if(static_cast<XLSB::ACCELLTABLE*>(ptr->m_ACCELLTABLE.get()) != nullptr && static_cast<XLSB::ACCELLTABLE*>(ptr->m_ACCELLTABLE.get())->m_BrtRwDescent != nullptr)
                {
                    auto ptrRwDescent = static_cast<XLSB::RwDescent*>(static_cast<XLSB::ACCELLTABLE*>(ptr->m_ACCELLTABLE.get())->m_BrtRwDescent.get());
                    m_oDyDescent                = ptrRwDescent->dyDescent;
                }
            }
        }
		EElementType CRow::getType () const
		{
			return et_x_Row;
		}

		CSheetData::CSheetData(OOX::Document *pMain) : WritingElementWithChilds<CRow>(pMain)
		{
		}
		CSheetData::~CSheetData()
		{
		}
		void CSheetData::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheetData::toXML() const
		{
			return _T("");
		}
		void CSheetData::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLStart(writer);
			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			toXMLEnd(writer);
		}
		void CSheetData::toXMLStart(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<sheetData>"));
		}
		void CSheetData::toXMLEnd(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("</sheetData>"));
		}
        void CSheetData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"ss:StyleID", m_sStyleID)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:DefaultColumnWidth", m_dDefaultColumnWidth)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:DefaultRowHeight", m_dDefaultRowHeight)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:ExpandedColumnCount", m_nExpandedColumnCount)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:ExpandedRowCount", m_nExpandedRowCount)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"x:FullColumns", m_nFullColumns)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"x:FullRows", m_nFullRows)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CSheetData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;
			CXlsx* xlsx = dynamic_cast<CXlsx*>(m_pMainDocument);
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			
			if(xlsx)
			{
				xlsx->m_nLastReadRow = 0;
			}
			if (xlsx_flat)
			{
				xlsx_flat->m_nLastReadRow = 0;
			}
			if (xlsx && xlsx->m_pXlsbWriter)
			{
				int nLastRow = -1;
				CRowXLSB oRow;
				oRow.m_nR = nLastRow;
				CCellXLSB oCell;
				NSBinPptxRW::CXlsbBinaryWriter& oStream = *xlsx->m_pXlsbWriter;
				m_oXlsbPos.Init();
				m_oXlsbPos->SetValue(oStream.GetPositionAbsolute());

                oStream.XlsbStartRecord(XLSB::rt_BeginSheetData, 0);
				oStream.XlsbEndRecord();

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

					if ( strcmp("row", sName) == 0 )
					{
						nLastRow = oRow.m_nR;
						oRow.Clean();
						oRow.m_nR = nLastRow + 1;
						oRow.fromXMLToXLSB(oReader, *xlsx->m_pXlsbWriter, oCell);
					}
				}

                oStream.XlsbStartRecord(XLSB::rt_EndSheetData, 0);
				oStream.XlsbEndRecord();
			}
			else
			{
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());

					if ( strcmp("row", sName) == 0 || strcmp("Row", sName) == 0)
					{
						CRow *pRow = new CRow(m_pMainDocument);
						if (pRow)
						{
							m_arrItems.push_back(pRow);
							pRow->fromXML(oReader);
						}
					}
					else if (strcmp("Column", sName) == 0)
					{
						CCol *pColumn = new CCol(m_pMainDocument);
						if (pColumn && xlsx_flat)
						{
							pColumn->fromXML(oReader);

							CWorksheet* pWorksheet = xlsx_flat->m_arWorksheets.back();

							if (false == pWorksheet->m_oCols.IsInit())
							{
								pWorksheet->m_oCols.Init();
							}

							pWorksheet->m_oCols->m_arrItems.push_back(pColumn);	

							pColumn->m_oMin.Init();
							pColumn->m_oMin->SetValue(pWorksheet->m_oCols->m_arrItems.size());
							
							pColumn->m_oMax.Init();
							pColumn->m_oMax->SetValue(pWorksheet->m_oCols->m_arrItems.size());
						}
					}
				}
			}
		}
		void CSheetData::AfterRead()
		{
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			if (!xlsx_flat) return;
			
			CWorksheet* pWorksheet = xlsx_flat->m_arWorksheets.back();
			pWorksheet->m_oSheetFormatPr.Init();

			if (m_dDefaultColumnWidth.IsInit())
			{
				double pixDpi = *m_dDefaultColumnWidth / 72.0 * 96.; if (pixDpi < 5) pixDpi = 7; // ~
				double maxDigitSize = xlsx_flat->getMaxDigitSize().first;

				m_dDefaultColumnWidth = ((int)((pixDpi + 5) / maxDigitSize * 100. + 0.5)) / 100.;
			}

			pWorksheet->m_oSheetFormatPr->m_oDefaultColWidth = m_dDefaultColumnWidth;
			pWorksheet->m_oSheetFormatPr->m_oDefaultRowHeight = m_dDefaultRowHeight;

			if (false == pWorksheet->m_oCols.IsInit() && m_dDefaultColumnWidth.IsInit())
			{
				pWorksheet->m_oCols.Init();

				CCol *pColumn = new CCol(m_pMainDocument);

				pColumn->m_oMin = 1;
				pColumn->m_oMax = 16384;

				pColumn->m_oWidth.Init();
				pColumn->m_oWidth->SetValue(*m_dDefaultColumnWidth);

				pWorksheet->m_oCols->m_arrItems.push_back(pColumn);
			}
			for (std::map<int, std::map<int, unsigned int>>::iterator it = m_mapStyleMerges2003.begin(); it != m_mapStyleMerges2003.end(); ++it)
			{
				xlsx_flat->m_nLastReadRow = it->first;

				CRow *pRow = new CRow(m_pMainDocument);
				pRow->m_oR = xlsx_flat->m_nLastReadRow;

				pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
				StyleFromMapStyleMerges2003(it->second);
			}
			m_mapStyleMerges2003.clear();
		}
		void CSheetData::fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter)
		{
            oStream.XlsbSkipRecord();//XLSB::rt_BeginSheetData

			CRow* pRow = NULL;
			LONG nOldPos = -1;
			while (nOldPos != oStream.GetPos())
			{
				nOldPos = oStream.GetPos();
				nType = oStream.XlsbReadRecordType();
                if (XLSB::rt_CellBlank <= nType && nType <= XLSB::rt_FmlaError)
				{
					CCell oCell(m_pMainDocument);
					oCell.fromXLSB(oStream, nType, pRow->m_oR->GetValue() - 1);
					fromXLSBToXmlCell(oCell, pCSVWriter, oStreamWriter);
				}
                else if (XLSB::rt_RowHdr == nType)
				{
					fromXLSBToXmlRowEnd(pRow, pCSVWriter, oStreamWriter);
					RELEASEOBJECT(pRow);

					pRow = new CRow(m_pMainDocument);
					pRow->fromXLSB(oStream, nType);

					fromXLSBToXmlRowStart(pRow, pCSVWriter, oStreamWriter);
				}
                else if (XLSB::rt_EndSheetData == nType)
				{
					fromXLSBToXmlRowEnd(pRow, pCSVWriter, oStreamWriter);
					RELEASEOBJECT(pRow);
					oStream.XlsbSkipRecord();
					break;
				}
				else
				{
					oStream.XlsbSkipRecord();
				}
			}
			RELEASEOBJECT(pRow);
		}
		void CSheetData::fromXLSBToXmlCell (CCell& oCell, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter)
		{
			if (NULL == pCSVWriter)
			{
				oCell.toXML(oStreamWriter);
			}
			else
			{
				pCSVWriter->WriteCell(&oCell);
			}
		}
		void CSheetData::fromXLSBToXmlRowStart (CRow* pRow, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter)
		{
			if (NULL == pCSVWriter)
			{
				pRow->toXMLStart(oStreamWriter);
			}
			else
			{
				pCSVWriter->WriteRowStart(pRow);
			}
		}
		void CSheetData::fromXLSBToXmlRowEnd (CRow* pRow, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter)
		{
			if(pRow)
			{
				if (NULL == pCSVWriter)
				{
					pRow->toXMLEnd(oStreamWriter);
				}
				else
				{
					pCSVWriter->WriteRowEnd(pRow);
				}
			}
		}
		void CSheetData::StyleFromMapStyleMerges2003(std::map<int, unsigned int> &mapStyleMerges)
		{
			CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
			if (!xlsx_flat) return;

			CWorksheet* sheet = xlsx_flat->m_arWorksheets.back();

			std::map<int, unsigned int>::iterator it = mapStyleMerges.begin();
			while (it != mapStyleMerges.end())
			{
				CCell *pCell = new CCell(this->m_pMainDocument);
				pCell->m_oRef = getCellAddressA(xlsx_flat->m_nLastReadRow, it->first);
				pCell->m_oStyle = it->second;
				pCell->m_oCol = it->first - 1;
				pCell->m_oRow = xlsx_flat->m_nLastReadRow;

				sheet->m_oSheetData->m_arrItems.back()->m_arrItems.push_back(pCell);

				mapStyleMerges.erase(it);
				it = mapStyleMerges.begin();
			}
		}
		void CSheetData::fromBin(XLS::BaseObjectPtr& obj)
		{
			//ReadAttributes(obj);
			auto ptr = static_cast<XLSB::CELLTABLE*>(obj.get());

			for (auto it = ptr->m_arParenthesis_CELLTABLE.begin(); it != ptr->m_arParenthesis_CELLTABLE.end();)
			{
			  CRow *pRow = new CRow(m_pMainDocument);
			  pRow->fromBin(*it);
			  m_arrItems.push_back(pRow);

			  it = ptr->m_arParenthesis_CELLTABLE.erase(it);
			}

			/*for(auto &Parenthesis_CELLTABLE : ptr->m_arParenthesis_CELLTABLE)
			{
				CRow *pRow = new CRow(m_pMainDocument);
				pRow->fromBin(Parenthesis_CELLTABLE);

				m_arrItems.push_back(pRow);
			}*/
		}
		EElementType CSheetData::getType () const
		{
			return et_x_SheetData;
		}
//-----------------------------------------------------------------------------------------
        void CWorksheet::ReadWorksheetOptions(XmlUtils::CXmlLiteReader& oReader)
        {
            if ( oReader.IsEmptyNode() )
                return;

            CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(WritingElement::m_pMainDocument);

            if (!xlsx_flat) return;

            if (false == m_oSheetViews.IsInit())
            {
                m_oSheetViews.Init();
                m_oSheetViews->m_arrItems.push_back(new CSheetView());
            }
            nullable_int active_pane;
            nullable_bool bFreeze;
            nullable_int xSplit, ySplit;

            nullable_int active_pane_number;
            nullable_int left_column_visible;
            nullable_int page_break_zoom;
            std::map<int, nullable<CPane>> mapPanes;

            nullable_string sDataHeader, sDataFooter;

            int nDocumentDepth = oReader.GetDepth();
            while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
            {
                std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

                if (L"PageSetup" == sName)
                {
                    if (false == m_oPageSetup.IsInit()) m_oPageSetup.Init();

                    int nDocumentDepth1 = oReader.GetDepth();
                    while (oReader.ReadNextSiblingNode(nDocumentDepth1))
                    {
                        std::wstring sName1 = XmlUtils::GetNameNoNS(oReader.GetName());

                        if (L"Header" == sName1)
                        {
                            if (false == m_oPageMargins.IsInit()) m_oPageMargins.Init();

                            WritingElement_ReadAttributes_Start_No_NS(oReader)
                                WritingElement_ReadAttributes_Read_if(oReader, L"Margin", m_oPageMargins->m_oHeader)
                                WritingElement_ReadAttributes_Read_else_if(oReader, L"Data", sDataHeader)
                            WritingElement_ReadAttributes_End_No_NS(oReader)
                        }
                        else if (L"Footer" == sName1)
                        {
                            if (false == m_oPageMargins.IsInit()) m_oPageMargins.Init();

                            WritingElement_ReadAttributes_Start_No_NS(oReader)
                                WritingElement_ReadAttributes_Read_if(oReader, L"Margin", m_oPageMargins->m_oFooter)
                                WritingElement_ReadAttributes_Read_else_if(oReader, L"Data", sDataFooter)
                            WritingElement_ReadAttributes_End_No_NS(oReader)
                        }
                        else if (L"Layout" == sName1)
                        {
                            WritingElement_ReadAttributes_Start_No_NS(oReader)
                                WritingElement_ReadAttributes_Read_if(oReader, L"x:Orientation", m_oPageSetup->m_oOrientation)
                            WritingElement_ReadAttributes_End_No_NS(oReader)
                        }
                        else if (L"PageMargins" == sName1)
                        {
                            if (false == m_oPageMargins.IsInit()) m_oPageMargins.Init();

                            WritingElement_ReadAttributes_Start_No_NS(oReader)
                                WritingElement_ReadAttributes_Read_if(oReader, L"Top", m_oPageMargins->m_oTop)
                                WritingElement_ReadAttributes_Read_else_if(oReader, L"Left", m_oPageMargins->m_oLeft)
                                WritingElement_ReadAttributes_Read_else_if(oReader, L"Right", m_oPageMargins->m_oRight)
                                WritingElement_ReadAttributes_Read_else_if(oReader, L"Bottom", m_oPageMargins->m_oBottom)
                            WritingElement_ReadAttributes_End_No_NS(oReader)
                        }
                    }
                }
                else if (L"Panes" == sName)
                {
                    int nDocumentDepth1 = oReader.GetDepth();
                    while (oReader.ReadNextSiblingNode(nDocumentDepth1))
                    {
                        std::wstring sName1 = XmlUtils::GetNameNoNS(oReader.GetName());

                        if (L"Pane" == sName1)
                        {
                            nullable<CPane> pane; pane.Init();
                            nullable_int number;

                            int nDocumentDepth2 = oReader.GetDepth();
                            nullable_int col, row;
                            while (oReader.ReadNextSiblingNode(nDocumentDepth2))
                            {
                                std::wstring sName2 = XmlUtils::GetNameNoNS(oReader.GetName());
                                if (L"Number" == sName2)
                                {
                                    number = oReader.GetText2();
                                }
                                else if (L"ActiveRow" == sName2)
                                {
                                    row = oReader.GetText2();
                                }
                                else if (L"ActiveCol" == sName2)
                                {
                                    col = oReader.GetText2();
                                }
                                else if (L"RangeSelection" == sName2)
                                {
                                    r1c1_formula_convert::base_row = xlsx_flat->m_nLastReadRow;
                                    r1c1_formula_convert::base_col = xlsx_flat->m_nLastReadCol;

                                    r1c1_formula_convert convert;

                                    std::wstring ref = convert.convert(oReader.GetText2());

                                    m_oSheetViews->m_arrItems.back()->m_arrItems.push_back(new CSelection());
                                    m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oSqref = ref;

                                    size_t pos_split = ref.find(L":");
                                    m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oActiveCell =
                                        pos_split != std::wstring::npos ? ref.substr(0, pos_split) : ref;
                                }
                            }
                            if (col.IsInit() && row.IsInit())
                            {
                                if (m_oSheetViews->m_arrItems.back()->m_arrItems.empty())
                                    m_oSheetViews->m_arrItems.back()->m_arrItems.push_back(new CSelection());

                                m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oActiveCell = getCellAddress(*row + 1, *col + 1);

                                if (false == m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oSqref.IsInit())
                                {
                                    m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oSqref = m_oSheetViews->m_arrItems.back()->m_arrItems.back()->m_oActiveCell;
                                }
                            }
                            if (number.IsInit())
                            {
                                mapPanes.insert(std::make_pair(*number, pane));
                            }
                        }
                    }
                }
                else if (L"SplitHorizontal" == sName)
                {
                    ySplit = oReader.GetText2();
                }
                else if (L"SplitVertical" == sName)
                {
                    xSplit = oReader.GetText2();
                }
                else if (L"DoNotDisplayGridlines" == sName)
                {
                    m_oSheetViews->m_arrItems.back()->m_oShowGridLines.Init();
                    m_oSheetViews->m_arrItems.back()->m_oShowGridLines->FromBool(false);
                }
                else if (L"Selected" == sName)
                {
                    m_oSheetViews->m_arrItems.back()->m_oTabSelected.Init();
                    m_oSheetViews->m_arrItems.back()->m_oTabSelected->FromBool(true);
                }
                else if (L"FreezePanes" == sName)
                {
                    bFreeze = true;
                }
                else if (L"ActivePane" == sName)
                {
                    active_pane_number = oReader.GetText2();
                }
                else if (L"Print" == sName)
                {
                    if (false == m_oPageSetup.IsInit()) m_oPageSetup.Init();
                    if (false == m_oPrintOptions.IsInit()) m_oPrintOptions.Init();

                    int nDocumentDepth1 = oReader.GetDepth();
                    while (oReader.ReadNextSiblingNode(nDocumentDepth1))
                    {
                        std::wstring sName1 = XmlUtils::GetNameNoNS(oReader.GetName());

                        if (L"FitHeight" == sName1)
                        {
                            m_oPageSetup->m_oFitToHeight = oReader.GetText2();
                        }
                        else if (L"DraftQuality" == sName1)
                        {
                            m_oPageSetup->m_oDraft.Init();
                        }
                        else if (L"Gridlines" == sName1)
                        {
                            m_oPrintOptions->m_oGridLines = true;
                        }
                        else if (L"Scale" == sName1)
                        {
                            m_oPageSetup->m_oScale = oReader.GetText2();
                        }
                        else if (L"HorizontalResolution" == sName1)
                        {
                            m_oPageSetup->m_oHorizontalDpi = oReader.GetText2();
                        }
                        else if (L"VerticalResolution" == sName1)
                        {
                            m_oPageSetup->m_oVerticalDpi = oReader.GetText2();
                        }
                        else if (L"PaperSizeIndex" == sName1)
                        {
                            m_oPageSetup->m_oPaperSize = oReader.GetText2();
                        }
                    }
                }
                else if (L"FitToPage" == sName)
                {
                    if (!m_oSheetPr.IsInit()) m_oSheetPr.Init();
                    if (!m_oSheetPr->m_oPageSetUpPr.IsInit()) m_oSheetPr->m_oPageSetUpPr.Init();

                    m_oSheetPr->m_oPageSetUpPr->m_oFitToPage.Init();
                    m_oSheetPr->m_oPageSetUpPr->m_oFitToPage->FromBool(true);
                }
                else if (L"ProtectObjects" == sName)
                {

                }
                else if (L"ProtectScenarios" == sName)
                {

                }
                else if (L"ProtectContents" == sName)
                {

                }
                else if (L"LeftColumnVisible" == sName)
                {
                    left_column_visible = oReader.GetText2();
                }
                else if (L"PageBreakZoom" == sName)
                {
                    page_break_zoom = oReader.GetText2();
                }
                else if (L"DoNotDisplayColHeaders" == sName)
                {

                }
                else if (L"ViewableRange" == sName)
                {

                }
                else if (L"GridlineColor" == sName)
                {

                }
                else if (L"Unsynced" == sName)
                {

                }
                else if (L"DisplayPageBreak" == sName)
                {

                }
                else if (L"ShowPageBreakZoom" == sName)
                {
                    m_oSheetViews->m_arrItems.back()->m_oView = SimpleTypes::Spreadsheet::sheetviewPageBreakPreview;
                }
                else if (L"DefaultRowHeight" == sName)
                {

                }
                else if (L"DefaultColumnWidth" == sName)
                {

                }
                else if (L"Visible" == sName)
                {

                }
                else if (L"DisplayRightToLeft" == sName)
                {

                }
                else if (L"DisplayFormulas" == sName)
                {
                    m_oSheetViews->m_arrItems.back()->m_oShowFormulas = true;
                }
                else if (L"ActiveRow" == sName)
                {

                }
                else if (L"ActiveColumn" == sName)
                {

                }
                else if (L"TabColorIndex" == sName)
                {
                    if (false == m_oSheetPr.IsInit()) m_oSheetPr.Init();
                    m_oSheetPr->m_oTabColor.Init();  m_oSheetPr->m_oTabColor->m_oIndexed = oReader.GetText2();
                }
            }

            if (m_oPageMargins.IsInit())
            {
                if (!m_oPageMargins->m_oFooter.IsInit())
                {
                    m_oPageMargins->m_oFooter.Init();
                    m_oPageMargins->m_oFooter->SetValue(0.5);
                }
                if (!m_oPageMargins->m_oHeader.IsInit())
                {
                    m_oPageMargins->m_oHeader.Init();
                    m_oPageMargins->m_oHeader->SetValue(0.5);
                }
            }

            if (active_pane_number.IsInit())
            {
                std::map<int, nullable<CPane>>::iterator pFind = mapPanes.find(*active_pane_number);
                if (pFind != mapPanes.end())
                {
                    m_oSheetViews->m_arrItems.back()->m_oPane = pFind->second;
                }
            }
            if (m_oSheetViews->m_arrItems.back()->m_oPane.IsInit())
            {
                m_oSheetViews->m_arrItems.back()->m_oPane->m_oActivePane.Init();
                m_oSheetViews->m_arrItems.back()->m_oPane->m_oActivePane->SetValue(SimpleTypes::Spreadsheet::activepaneBottomLeft);
                if (bFreeze.IsInit())
                {
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oState.Init();
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oState->SetValue(SimpleTypes::Spreadsheet::panestateFrozen);
                }
                if (ySplit.IsInit())
                {
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oYSplit.Init();
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oYSplit->SetValue(*ySplit);
                }
                if (xSplit.IsInit())
                {
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oXSplit.Init();
                    m_oSheetViews->m_arrItems.back()->m_oPane->m_oXSplit->SetValue(*xSplit);
                }
            }
        }
//----------------------------------------------------------------------------------------
        void CDataValidation::fromXML(XmlUtils::CXmlLiteReader& oReader)
        {
            ReadAttributes( oReader );

            if ( oReader.IsEmptyNode() )
                return;

            int nCurDepth = oReader.GetDepth();
            while (oReader.ReadNextSiblingNode(nCurDepth))
            {
                std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
                if (L"formula1" == sName)
                {
                    m_oFormula1 = oReader;
                }
                else if (L"formula2" == sName)
                {
                    m_oFormula2 = oReader;
                }
                else if (L"sqref" == sName)
                {
                    m_oSqRef = oReader.GetText2();
                }
        //--------------------------------------------------- xml spreadsheet 2002
                else if (L"Range" == sName)
                {
                    r1c1_formula_convert::base_row = 1;
                    r1c1_formula_convert::base_col = 1;

                    r1c1_formula_convert convert;

                    m_oSqRef = convert.convert(oReader.GetText2());
                }
                else if (L"Type" == sName)
                {
                    m_oType = oReader.GetText2();

                    m_oAllowBlank.Init();
                    m_oAllowBlank->FromBool(true);

                    m_oShowInputMessage.Init();
                    m_oShowInputMessage->FromBool(true);
                }
                else if (L"Value" == sName)
                {
                    r1c1_formula_convert::base_row = 1;
                    r1c1_formula_convert::base_col = 1;

                    r1c1_formula_convert convert;

                    m_oFormula1 = new CDataValidationFormula(m_pMainDocument);
                    m_oFormula1->m_sText = convert.convert(oReader.GetText3());

                    //if (m_oFormula1->m_sText.find(L"!") == std::wstring::npos)
                    //{
                    //	CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);
                    //	if (xlsx_flat)
                    //	{
                    //		CSheet *pSheet = xlsx_flat->m_pWorkbook->m_oSheets->m_arrItems.back();
                    //		if (pSheet->m_oName.IsInit())
                    //		{
                    //			m_oFormula1->m_sText = *pSheet->m_oName + L"!" + m_oFormula1->m_sText;
                    //		}
                    //	}
                    //}
                }
            }
        }
//-----------------------------------------------------------------------------------------
        void CDefinedName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
        {
            nullable_string oRefersTo;
            WritingElement_ReadAttributes_Start( oReader )
                WritingElement_ReadAttributes_Read_if     ( oReader, L"comment",		m_oComment )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"customMenu",		m_oCustomMenu )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"description",	m_oDescription )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"function",		m_oFunction )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"functionGroupId",m_oFunctionGroupId )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"help",			m_oHelp )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"hidden",			m_oHidden )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"localSheetId",	m_oLocalSheetId )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"name",			m_oName )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"publishToServer",m_oPublishToServer )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"shortcutKey ",	m_oShortcutKey  )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"statusBar",		m_oStatusBar  )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"vbProcedure",	m_oVbProcedure  )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"workbookParameter",	m_oWorkbookParameter  )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"xlm",			m_oXlm  )

                WritingElement_ReadAttributes_Read_else_if( oReader, L"ss:Name",		m_oName )
                WritingElement_ReadAttributes_Read_else_if( oReader, L"ss:RefersTo",	oRefersTo )
            WritingElement_ReadAttributes_End( oReader )

            if (oRefersTo.IsInit())
            {
                r1c1_formula_convert::base_row = 1;
                r1c1_formula_convert::base_col = 1;

                r1c1_formula_convert convert;

                m_oRef = convert.convert(oRefersTo->substr(1));
            }
        }
		void CWorksheet::PrepareComments(OOX::Spreadsheet::CComments* pComments, OOX::Spreadsheet::CThreadedComments* pThreadedComments, OOX::Spreadsheet::CLegacyDrawingWorksheet* pLegacyDrawing)
		{
			OOX::CVmlDrawing* pVmlDrawing = NULL;
			if (NULL != pLegacyDrawing && pLegacyDrawing->m_oId.IsInit())
			{
				OOX::RId oRId(pLegacyDrawing->m_oId->GetValue());

				smart_ptr<OOX::File> oVmlDrawing = IFileContainer::Find(oRId);

				if (oVmlDrawing.IsInit() && OOX::FileTypes::VmlDrawing == oVmlDrawing->type())
				{
					pVmlDrawing = static_cast<OOX::CVmlDrawing*>(oVmlDrawing.GetPointer());
				}
			}
			//2.3.7.3.1 Reconciliation
			//if Corresponding placeholder is not found, Delete the entire comment thread.
			if (!pComments || !pVmlDrawing)
				return;
			std::unordered_map<std::wstring, int> mapCheckCopyThreadedComments;
			std::vector<std::wstring> & arAuthors = pComments->m_oAuthors->m_arrItems;

			if (pComments->m_oCommentList.IsInit())
			{
				std::vector<OOX::Spreadsheet::CComment*> & aComments = pComments->m_oCommentList->m_arrItems;

				for (size_t i = 0; i < aComments.size(); ++i)
				{
					OOX::Spreadsheet::CComment* pComment = aComments[i];

					if (!pComment) continue;

					bool bThreadedCommentCopy = false;
					OOX::Spreadsheet::CThreadedComment* pThreadedComment = NULL;
					if (pThreadedComments)
					{
						std::unordered_map<std::wstring, CThreadedComment*>::iterator pFind = pThreadedComments->m_mapTopLevelThreadedComments.end();

						bool isPlaceholder = false;
						if (pComment->m_oAuthorId.IsInit())
						{
							unsigned int nAuthorId = pComment->m_oAuthorId->GetValue();

							if (nAuthorId >= 0 && nAuthorId < arAuthors.size())
							{
								const std::wstring& sAuthor = arAuthors[nAuthorId];
								if (0 == sAuthor.compare(0, 3, L"tc="))
								{
									isPlaceholder = true;
									std::wstring sGUID = sAuthor.substr(3);
									//todo IsZero() is added to fix comments with zero ids(5.4.0)(bug 42947). Remove after few releases
									if (L"{00000000-0000-0000-0000-000000000000}" == sGUID && pComment->m_oRef.IsInit())
									{
										for (std::unordered_map<std::wstring, CThreadedComment*>::iterator it = pThreadedComments->m_mapTopLevelThreadedComments.begin(); it != pThreadedComments->m_mapTopLevelThreadedComments.end(); ++it)
										{
											if (it->second->ref.IsInit() && pComment->m_oRef->GetValue() == it->second->ref.get())
											{
												pFind = it;
												break;
											}
										}
									}
									else
									{
										pFind = pThreadedComments->m_mapTopLevelThreadedComments.find(sGUID);
									}

								}
							}
						}
						if (pThreadedComments->m_mapTopLevelThreadedComments.end() != pFind)
						{
							pThreadedComment = pFind->second;
							if (mapCheckCopyThreadedComments.end() != mapCheckCopyThreadedComments.find(pThreadedComment->id->ToString()))
							{
								bThreadedCommentCopy = true;
							}
							else
							{
								mapCheckCopyThreadedComments[pThreadedComment->id->ToString()] = 1;
							}
						}
						else if (isPlaceholder)
						{
							continue;
						}
					}

					if (pComment->m_oRef.IsInit() && pComment->m_oAuthorId.IsInit())
					{
						int nRow, nCol;
						if (CCell::parseRef(pComment->m_oRef->GetValue(), nRow, nCol))
						{
							CCommentItem* pCommentItem = new CCommentItem();
							pCommentItem->m_nRow = nRow - 1;
							pCommentItem->m_nCol = nCol - 1;

							unsigned int nAuthorId = pComment->m_oAuthorId->GetValue();

							if (nAuthorId >= 0 && nAuthorId < arAuthors.size())
							{
								pCommentItem->m_sAuthor = arAuthors[nAuthorId];
							}
							OOX::Spreadsheet::CSi* pSi = pComment->m_oText.GetPointerEmptyNullable();
							if (NULL != pSi)
								pCommentItem->m_oText.reset(pSi);

							pCommentItem->m_pThreadedComment = pThreadedComment;
							pCommentItem->m_bThreadedCommentCopy = bThreadedCommentCopy;

							std::wstring sNewId = std::to_wstring(pCommentItem->m_nRow.get()) + L"-" + std::to_wstring(pCommentItem->m_nCol.get());
							m_mapComments[sNewId] = pCommentItem;
						}
					}
				}
			}

			for (size_t i = 0; i < pVmlDrawing->m_arrItems.size(); ++i)
			{
				OOX::Vml::CShape* pShape = dynamic_cast<OOX::Vml::CShape*>(pVmlDrawing->m_arrItems[i]);

				if (pShape == NULL) continue;

				if (pShape->m_sId.IsInit())
				{//mark shape as used
					boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = pVmlDrawing->m_mapShapes.find(pShape->m_sId.get());
					if (pFind != pVmlDrawing->m_mapShapes.end())
					{
						pFind->second.bUsed = true;
					}
				}
				for (size_t j = 0; j < pShape->m_arrItems.size(); ++j)
				{
					OOX::WritingElement* pElem = pShape->m_arrItems[j];

					if (!pElem) continue;

					if (OOX::et_v_ClientData == pElem->getType())
					{
						OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pElem);
						if (pClientData->m_oRow.IsInit() && pClientData->m_oColumn.IsInit())
						{
							int nRow = pClientData->m_oRow->GetValue();
							int nCol = pClientData->m_oColumn->GetValue();
							std::wstring sId = std::to_wstring(nRow) + L"-" + std::to_wstring(nCol);

							std::map<std::wstring, CCommentItem*>::const_iterator pPair = m_mapComments.find(sId);
							if (pPair != m_mapComments.end())
							{
								CCommentItem* pCommentItem = pPair->second;
								if (pShape->m_sGfxData.IsInit())
									pCommentItem->m_sGfxdata = *pShape->m_sGfxData;
								std::vector<int> m_aAnchor;
								pClientData->getAnchorArray(m_aAnchor);
								if (8 <= m_aAnchor.size())
								{
									pCommentItem->m_nLeft = abs(m_aAnchor[0]);
									pCommentItem->m_nLeftOffset = abs(m_aAnchor[1]);
									pCommentItem->m_nTop = abs(m_aAnchor[2]);
									pCommentItem->m_nTopOffset = abs(m_aAnchor[3]);
									pCommentItem->m_nRight = abs(m_aAnchor[4]);
									pCommentItem->m_nRightOffset = abs(m_aAnchor[5]);
									pCommentItem->m_nBottom = abs(m_aAnchor[6]);
									pCommentItem->m_nBottomOffset = abs(m_aAnchor[7]);
								}
								pCommentItem->m_bMove = pClientData->m_oMoveWithCells;
								pCommentItem->m_bSize = pClientData->m_oSizeWithCells;
								pCommentItem->m_bVisible = pClientData->m_oVisible;

								if (pShape->m_oFillColor.IsInit())
								{
									BYTE r = pShape->m_oFillColor->Get_R();
									BYTE g = pShape->m_oFillColor->Get_G();
									BYTE b = pShape->m_oFillColor->Get_B();

									std::wstringstream sstream;
									sstream << boost::wformat(L"%02X%02X%02X") % r % g % b;

									pCommentItem->m_sFillColorRgb = sstream.str();
								}

								for (size_t k = 0; k < pShape->m_oStyle->m_arrProperties.size(); ++k)
								{
									if (pShape->m_oStyle->m_arrProperties[k] == NULL) continue;

									SimpleTypes::Vml::CCssProperty *oProperty = pShape->m_oStyle->m_arrProperties[k].get();
									if (SimpleTypes::Vml::cssptMarginLeft == oProperty->get_Type())
									{
										SimpleTypes::Vml::UCssValue oUCssValue = oProperty->get_Value();
										if (SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
										{
											SimpleTypes::CPoint oPoint;
											oPoint.FromPoints(oUCssValue.oValue.dValue);
											pCommentItem->m_dLeftMM = oPoint.ToMm();
										}
									}
									else if (SimpleTypes::Vml::cssptMarginTop == oProperty->get_Type())
									{
										SimpleTypes::Vml::UCssValue oUCssValue = oProperty->get_Value();
										if (SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
										{
											SimpleTypes::CPoint oPoint;
											oPoint.FromPoints(oUCssValue.oValue.dValue);
											pCommentItem->m_dTopMM = oPoint.ToMm();
										}
									}
									else if (SimpleTypes::Vml::cssptWidth == oProperty->get_Type())
									{
										SimpleTypes::Vml::UCssValue oUCssValue = oProperty->get_Value();
										if (SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
										{
											SimpleTypes::CPoint oPoint;
											oPoint.FromPoints(oUCssValue.oValue.dValue);
											pCommentItem->m_dWidthMM = oPoint.ToMm();
										}
									}
									else if (SimpleTypes::Vml::cssptHeight == oProperty->get_Type())
									{
										SimpleTypes::Vml::UCssValue oUCssValue = oProperty->get_Value();
										if (SimpleTypes::Vml::cssunitstypeUnits == oUCssValue.oValue.eType)
										{
											SimpleTypes::CPoint oPoint;
											oPoint.FromPoints(oUCssValue.oValue.dValue);
											pCommentItem->m_dHeightMM = oPoint.ToMm();
										}
									}
								}
							}
						}
					}
				}
			}
		}
	} //Spreadsheet
} // OOX
