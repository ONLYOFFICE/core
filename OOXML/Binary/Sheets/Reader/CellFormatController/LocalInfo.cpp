/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "LocalInfo.h"
#include <map>

namespace lcInfo
{

std::vector<std::vector<std::wstring>> MonthNames{};
std::vector<std::vector<std::wstring>> ShortMonthNames{};
std::map<_INT32, LocalInfo> InfoMap {};

//инициализируем большие переменные только при необходимости
void InitLocalInfo()
{
    InfoMap = std::map<_INT32, LocalInfo>
    {
        {-1, LocalInfo{-1, L"default", L"-", L"215", 0, 3}},
        {1, LocalInfo{1, L"ar", L"/", L"134", 28, 13}},
        {4, LocalInfo{4, L"zh-Hans", L"/", L"520", 3, 3}},
        {5, LocalInfo{5, L"cs", L".", L"135", 4, 3}},
        {6, LocalInfo{6, L"da", L"-", L"135",  5, 3}},
        {7, LocalInfo{7, L"de", L".", L"135", 6, 3}},
        {8, LocalInfo{8, L"el", L"/", L"025", 7, 3}},
        {9, LocalInfo{9, L"en", L"/", L"205", 0, 3}},
        {10, LocalInfo{10, L"es", L"/", L"135", 2, 3}},
        {11, LocalInfo{11, L"fi", L".", L"025", 8, 6}},
        {12, LocalInfo{12, L"fr", L"/", L"135", 9, 4}},
        {14, LocalInfo{14, L"hu", L".", L"531", 10, 5}},
        {16, LocalInfo{16, L"it", L"/", L"135", 11, 3}},
        {17, LocalInfo{17, L"ja", L"/", L"531", 12, 2}},
        {18, LocalInfo{18, L"ko", L"-", L"531", 13, 2}},
        {21, LocalInfo{21, L"pl", L".", L"135", 14, 3}},
        {22, LocalInfo{22, L"pt", L"/", L"135", 15, 3}},
        {25, LocalInfo{25, L"ru", L".", L"135", 1, 3}},
        {29, LocalInfo{29, L"sv", L"-", L"531", 16, 3}},
        {31, LocalInfo{31, L"tr", L".", L"035", 17, 3}},
        {33, LocalInfo{33, L"id", L"/", L"135", 18, 3}},
        {34, LocalInfo{34, L"uk", L".", L"135", 19, 3}},
        {36, LocalInfo{36, L"sl", L".", L"035", 20, 3}},
        {38, LocalInfo{38, L"lv", L".", L"135", 21, 5}},
        {39, LocalInfo{39, L"lt", L"-", L"531", 22, 5}},
        {42, LocalInfo{42, L"vi", L"/", L"135", 23, 5}},
        {44, LocalInfo{44, L"az", L".", L"135", 24, 3}},
        {63, LocalInfo{63, L"kk", L".", L"135", 25, 3}},
        {80, LocalInfo{80, L"mn", L".", L"531", 26, 8}},
        {1025, LocalInfo{1025, L"ar-SA", L"/", L"134", 28, 13}},
        {1026, LocalInfo{1026, L"bg-BG", L".", L"025", 27, 3}},
        {1028, LocalInfo{1028, L"zh-TW", L"/", L"520", 3, 2}},
        {1029, LocalInfo{1029, L"cs-CZ", L",", L"135", 4, 3}},
        {1030, LocalInfo{1030, L"da-DK", L"-", L"135", 5, 3}},
        {1031, LocalInfo{1031, L"de-DE", L"-", L"135", 6, 3}},
        {1032, LocalInfo{1032, L"el-GR", L"/", L"025", 7, 3}},
        {1033, LocalInfo{1033, L"en-US", L"/", L"205", 0, 3}},
        {1035, LocalInfo{1035, L"fi-FI", L".", L"025", 8, 3}},
        {1036, LocalInfo{1036, L"fr-FR", L"/", L"135", 9, 4}},
        {1038, LocalInfo{1038, L"hu-HU", L".", L"531", 10, 3}},
        {1040, LocalInfo{1040, L"it-IT", L"/", L"135", 11, 3}},
        {1041, LocalInfo{1041, L"ja-JP", L"/", L"135", 12, 2}},
        {1042, LocalInfo{1042, L"ko-KR", L"-", L"531", 13, 2}},
        {1043, LocalInfo{1043, L"nl-NL", L"-", L"025", 29, 3}},
        {1045, LocalInfo{1045, L"pl-PL", L".", L"135", 14, 3}},
        {1046, LocalInfo{1046, L"pt-BR", L"/", L"135", 15, 3}},
        {1049, LocalInfo{1049, L"ru-RU", L".", L"135", 1, 3}},
        {1050, LocalInfo{1050, L"hr-HR", L".", L"025", 30, 3}},
        {1051, LocalInfo{1051, L"sk-SK", L".", L"025", 31, 3}},
        {1053, LocalInfo{1053, L"sv-SE", L"-", L"531", 16, 3}},
        {1055, LocalInfo{1055, L"tr-TR", L".", L"035", 17, 3}},
        {1057, LocalInfo{1057, L"id-ID", L"/", L"135", 18, 3}},
        {1058, LocalInfo{1058, L"uk-UA", L".", L"135", 19, 3}},
        {1060, LocalInfo{1060, L"sl-SI", L".", L"035", 20, 3}},
        {1062, LocalInfo{1062, L"lv-LV", L".", L"135", 21, 5}},
        {1063, LocalInfo{1063, L"lt-LT", L"-", L"531", 22, 5}},
        {1066, LocalInfo{1066, L"vi-VN", L"/", L"135", 23, 5}},
        {1068, LocalInfo{1068, L"az-Latn-AZ", L".", L"135", 24, 3}},
        {1087, LocalInfo{1087, L"kk-KZ", L".", L"135", 25, 3}},
        {1104, LocalInfo{1104, L"mn-MN", L".", L"531", 26, 3}},
        {2049, LocalInfo{2049, L"ar-IQ", L"/", L"135", 32, 12}},
        {2052, LocalInfo{2052, L"zh-CN", L"/", L"520", 3, 3}},
        {2055, LocalInfo{2055, L"de-CH", L".", L"135", 6, 3}},
        {2057, LocalInfo{2057, L"en-GB", L"/", L"135", 0, 3}},
        {2058, LocalInfo{2058, L"es-MX", L"/", L"135", 2, 3}},
        {2060, LocalInfo{2060, L"fr-BE", L"-", L"134", 9, 4}},
        {2064, LocalInfo{2064, L"it-CH", L".", L"135", 11, 3}},
        {2070, LocalInfo{2070, L"pt-PT", L"/", L"135", 15, 3}},
        {2073, LocalInfo{2073, L"ru-MD", L".", L"135", 1, 3}},
        {2077, LocalInfo{2077, L"sv-FI", L"-", L"135", 16, 3}},
        {2092, LocalInfo{2092, L"az-Cyrl-AZ", L".", L"135", 33, 4}},
        {2128, LocalInfo{2128, L"mn-Mong-CN", L"/", L"520", 34, 22}},
        {3073, LocalInfo{3073, L"ar-EG", L"/", L"135", 35, 3}},
        {3076, LocalInfo{3076, L"zh-HK", L"/", L"025", 3, 3}},
        {3079, LocalInfo{3079, L"de-AT", L".", L"135", 6, 3}},
        {3081, LocalInfo{3081, L"en-AU", L"/", L"035", 0, 3}},
        {3082, LocalInfo{3082, L"es-ES", L"/", L"135", 2, 3}},
        {3084, LocalInfo{3084, L"fr-CA", L"-", L"531", 9, 4}},
        {3152, LocalInfo{3152, L"mn-Mong-MN", L"/", L"520", 34, 22}},
        {4097, LocalInfo{4097, L"ar-LY", L"/", L"135", 35, 3}},
        {4100, LocalInfo{4100, L"zh-SG", L"/", L"025", 3, 3}},
        {4103, LocalInfo{4103, L"de-LU", L".", L"135", 6, 3}},
        {4105, LocalInfo{4105, L"en-CA", L"-", L"531", 0, 3}},
        {4106, LocalInfo{4106, L"es-GT", L"/", L"035", 2, 3}},
        {4108, LocalInfo{4108, L"fr-CH", L"/", L"035", 9, 4}},
        {4122, LocalInfo{4122, L"hr-BA", L".", L"025", 30, 3}},
        {5121, LocalInfo{5121, L"ar-DZ", L"-", L"135", 35, 3}},
        {5124, LocalInfo{5124, L"zh-MO", L"/", L"025", 3, 3}},
        {5127, LocalInfo{5127, L"de-LI", L".", L"135", 6, 3}},
        {5129, LocalInfo{5129, L"en-NZ", L"/", L"035", 0, 3}},
        {5130, LocalInfo{5130, L"es-CR", L"/", L"025", 2, 3}},
        {5132, LocalInfo{5132, L"fr-LU", L"/", L"025", 9, 4}},
        {6153, LocalInfo{6153, L"en-IE", L"/", L"135", 0, 3}},
        {6154, LocalInfo{6154, L"es-PA", L"/", L"315", 2, 3}},
        {6156, LocalInfo{6156, L"fr-MC", L"/", L"135", 9, 4}},
        {7169, LocalInfo{7169, L"ar-TN", L"-", L"135", 35, 3}},
        {7177, LocalInfo{7177, L"en-ZA", L"/", L"531", 0, 3}},
        {7178, LocalInfo{7178, L"es-DO", L"/", L"025", 2, 3}},
        {7180, LocalInfo{7180, L"fr-029", L"/", L"135", 9, 4}},
        {8193, LocalInfo{8193, L"ar-OM", L"/", L"135", 35, 4}},
        {8201, LocalInfo{8201, L"en-JM", L"/", L"025", 0, 3}},
        {8202, LocalInfo{8202, L"es-VE", L"/", L"025", 2, 3}},
        {8204, LocalInfo{8204, L"fr-RE", L"/", L"135", 9, 4}},
        {9217, LocalInfo{9217, L"ar-YE", L"/", L"135", 35, 3}},
        {9225, LocalInfo{9225, L"en-029", L"/", L"135", 0, 3}},
        {9226, LocalInfo{9226, L"es-CO", L"/", L"035", 2, 3}},
        {9228, LocalInfo{9228, L"fr-CD", L"/", L"135", 9, 4}},
        {9242, LocalInfo{9242, L"sr-Latn-RS", L".", L"025", 36, 4}},
        {10250, LocalInfo{10250, L"es-PE", L"/", L"035", 2, 3}},
        {10252, LocalInfo{10252, L"fr-SN", L"/", L"135", 9, 4}},
        {10266, LocalInfo{10266, L"sr-Cyrl-RS", L".", L"135", 37, 4}},
        {11265, LocalInfo{11265, L"ar-JO", L"/", L"135", 32, 4}},
        {11273, LocalInfo{11273, L"en-TT", L"/", L"135", 0, 3}},
        {11274, LocalInfo{11274, L"es-AR", L"/", L"025", 2, 3}},
        {11276, LocalInfo{11276, L"fr-CM", L"/", L"135", 9, 4}},
        {12289, LocalInfo{12289, L"ar-LB", L"/", L"135", 32, 4}},
        {12297, LocalInfo{12297, L"en-ZW", L"/", L"025", 0, 3}},
        {12298, LocalInfo{12298, L"es-EC", L"/", L"025", 2, 3}},
        {12300, LocalInfo{12300, L"fr-CI", L"/", L"135", 9, 4}},
        {13313, LocalInfo{13313, L"ar-KW", L"/", L"135", 35, 4}},
        {13321, LocalInfo{13321, L"en-PH", L"/", L"135", 0, 3}},
        {13322, LocalInfo{13322, L"es-EC", L"-", L"135", 2, 3}},
        {13324, LocalInfo{13324, L"fr-ML", L"/", L"135", 9, 4}},
        {14337, LocalInfo{14337, L"ar-AE", L"/", L"135", 35, 4}},
        {14345, LocalInfo{14345, L"en-ID", L"/", L"135", 0, 3}},
        {14346, LocalInfo{14346, L"es-UY", L"/", L"025", 2, 3}},
        {14348, LocalInfo{14348, L"fr-MA", L"/", L"135", 9, 4}},
        {15361, LocalInfo{15361, L"ar-BH", L"/", L"135", 35, 4}},
        {15369, LocalInfo{15369, L"en-HK", L"/", L"025", 0, 3}},
        {15370, LocalInfo{15370, L"es-PY", L"/", L"025", 2, 3}},
        {15372, LocalInfo{15372, L"fr-HT", L"/", L"135", 9, 4}},
        {16385, LocalInfo{16385, L"ar-QA", L"/", L"135", 35, 4}},
        {16393, LocalInfo{16393, L"en-IN", L"-", L"135", 0, 3}},
        {16394, LocalInfo{16394, L"es-BO", L"/", L"025", 2, 3}},
        {17417, LocalInfo{17417, L"en-MY", L"/", L"025", 0, 3}},
        {17418, LocalInfo{17418, L"es-SV", L"/", L"025", 2, 3}},
        {18441, LocalInfo{18441, L"en-SG", L"/", L"025", 0, 3}},
        {18442, LocalInfo{18442, L"es-HN", L"/", L"025", 2, 3}},
        {19466, LocalInfo{19466, L"es-NI", L"/", L"025", 2, 3}},
        {20490, LocalInfo{20490, L"es-PR", L"/", L"315", 2, 3}},
        {21514, LocalInfo{21514, L"es-US", L"/", L"205", 2, 3}},
        {22538, LocalInfo{22538, L"es-419", L"/", L"025", 2, 3}},
        {23562, LocalInfo{23562, L"es-CU", L"/", L"025", 2, 3}},
        {27674, LocalInfo{27674, L"sr-Cyrl", L".", L"135", 37, 4}},
        {27674, LocalInfo{27674, L"sr-Latn", L".", L"025", 36, 4}},
        {29740, LocalInfo{29740, L"az-Cyrl", L".", L"135", 33, 4}},
        {30724, LocalInfo{30724, L"zh", L"/", L"520", 3, 3}},
        {30764, LocalInfo{30764, L"az-Latn", L".", L"135", 24, 3}},
        {30800, LocalInfo{30800, L"mn-Cyrl", L".", L"531", 26, 8}},
        {31748, LocalInfo{31748, L"zh-Hant", L"/", L"025", 3, 3}},
        {31824, LocalInfo{31824, L"mn-Mong", L"/", L"520", 34, 22}}
    };
    MonthNames = std::vector<std::vector<std::wstring>>
    {
		{L"january", L"february", L"march", L"april", L"may", L"june", L"july", L"august", L"september", L"october", L"november", L"december"},
        {L"январь", L"февраль", L"март", L"апрель", L"май", L"июнь", L"июль", L"август", L"сентябрь", L"октябрь", L"ноябрь", L"декабрь"},
        {L"enero", L"febrero", L"marzo", L"abril", L"mayo", L"junio", L"julio", L"agosto", L"septiembre", L"octubre", L"noviembre", L"diciembre"},
        {L"一月", L"二月", L"三月", L"四月", L"五月", L"六月", L"七月", L"八月", L"九月", L"十月", L"十一月", L"十二月"},
        {L"leden", L"únor", L"březen", L"duben", L"květen", L"červen", L"červenec", L"srpen", L"září", L"říjen", L"listopad", L"prosinec"},
        {L"januar", L"februar", L"marts", L"april", L"maj", L"juni", L"juli", L"august", L"september", L"oktober", L"november", L"december"},
        {L"Januar", L"Februar", L"März", L"April", L"Mai", L"Juni", L"Juli", L"August", L"September", L"Oktober", L"November", L"Dezember"},
        {L"Ιανουάριος", L"Φεβρουάριος", L"Μάρτιος", L"Απρίλιος", L"Μάιος", L"Ιούνιος", L"Ιούλιος", L"Αύγουστος", L"Σεπτέμβριος", L"Οκτώβριος", L"Νοέμβριος", L"Δεκέμβριος"},
        {L"tammikuu", L"helmikuu", L"maaliskuu", L"huhtikuu", L"toukokuu", L"kesäkuu", L"heinäkuu", L"elokuu", L"syyskuu", L"lokakuu", L"marraskuu", L"joulukuu"},
        {L"janvier", L"février", L"mars", L"avril", L"mai", L"juin", L"juillet", L"août", L"septembre", L"octobre", L"novembre", L"décembre"},
        {L"január", L"február", L"március", L"április", L"május", L"június", L"július", L"augusztus", L"szeptember", L"október", L"november", L"december"},
        {L"gennaio", L"febbraio", L"marzo", L"aprile", L"maggio", L"giugno", L"luglio", L"agosto", L"settembre", L"ottobre", L"novembre", L"dicembre"},
        {L"1月", L"2月", L"3月", L"4月", L"5月", L"6月", L"7月", L"8月", L"9月", L"10月", L"11月", L"12月"},
        {L"1월", L"2월", L"3월", L"4월", L"5월", L"6월", L"7월", L"8월", L"9월", L"10월", L"11월", L"12월"},
        {L"styczeń", L"luty", L"marzec", L"kwiecień", L"maj", L"czerwiec", L"lipiec", L"sierpień", L"wrzesień", L"październik", L"listopad", L"grudzień"},
        {L"janeiro", L"fevereiro", L"março", L"abril", L"maio", L"junho", L"julho", L"agosto", L"setembro", L"outubro", L"novembro", L"dezembro"},
        {L"januari", L"februari", L"mars", L"april", L"maj", L"juni", L"juli", L"augusti", L"september", L"oktober", L"november", L"december"},
        {L"Ocak", L"Şubat", L"Mart", L"Nisan", L"Mayıs", L"Haziran", L"Temmuz", L"Ağustos", L"Eylül", L"Ekim", L"Kasım", L"Aralık"},
        {L"Januari", L"Februari", L"Maret", L"April", L"Mei", L"Juni", L"Juli", L"Agustus", L"September", L"Oktober", L"November", L"Desember"},
        {L"січень", L"лютий", L"березень", L"квітень", L"травень", L"червень", L"липень", L"серпень", L"вересень", L"жовтень", L"листопад", L"грудень"},
        {L"januar", L"februar", L"marec", L"april", L"maj", L"junij", L"julij", L"avgust", L"september", L"oktober", L"november", L"december"},
        {L"janvāris", L"februāris", L"marts", L"aprīlis", L"maijs", L"jūnijs", L"jūlijs", L"augusts", L"septembris", L"oktobris", L"novembris", L"decembris"},
        {L"sausis", L"vasaris", L"kovas", L"balandis", L"gegužė", L"birželis", L"liepa", L"rugpjūtis", L"rugsėjis", L"spalis", L"lapkritis", L"gruodis"},
        {L"Tháng Giêng", L"Tháng Hai", L"Tháng Ba", L"Tháng Tư", L"Tháng Năm", L"Tháng Sáu", L"Tháng Bảy", L"Tháng Tám", L"Tháng Chín", L"Tháng Mười", L"Tháng Mười Một", L"Tháng Mười Hai"},
        {L"Yanvar", L"Fevral", L"Mart", L"Aprel", L"May", L"İyun", L"İyul", L"Avqust", L"Sentyabr", L"Oktyabr", L"Noyabr", L"Dekabr"},
        {L"Қаңтар", L"Ақпан", L"Наурыз", L"Сәуір", L"Мамыр", L"Маусым", L"Шілде", L"Тамыз", L"Қыркүйек", L"Қазан", L"Қараша", L"Желтоқсан"},
        {L"Нэгдүгээр сар", L"Хоёрдугаар сар", L"Гуравдугаар сар", L"Дөрөвдүгээр сар", L"Тавдугаар сар", L"Зургаадугаар сар", L"Долоодугаар сар", L"Наймдугаар сар", L"Есдүгээр сар", L"Аравдугаар сар", L"Арван нэгдүгээр сар", L"Арван хоёрдугаар сар"},
        {L"януари", L"февруари", L"март", L"април", L"май", L"юни", L"юли", L"август", L"септември", L"октомври", L"ноември", L"декември"},
        {L"ذو الحجة", L"ذو القعدة", L"شوال", L"رمضان", L"شعبان", L"رجب", L"جمادى الثانية", L"جمادى الأولى", L"ربيع الثاني", L"ربيع الأول", L"صفر", L"محرم"},
        {L"januari", L"februari", L"maart", L"april", L"mei", L"juni", L"juli", L"augustus", L"september", L"oktober", L"november", L"december"},
        {L"siječanj", L"veljača", L"ožujak", L"travanj", L"svibanj", L"lipanj", L"srpanj", L"kolovoz", L"rujan", L"listopad", L"studeni", L"prosinac"},
        {L"január", L"február", L"marec", L"apríl", L"máj", L"jún", L"júl", L"august", L"september", L"október", L"november", L"december"},
        {L"كانون الأول", L"تشرين الثاني", L"تشرين الأول", L"أيلول", L"آب", L"تموز", L"حزيران", L"أيار", L"نيسان", L"آذار", L"شباط", L"كانون الثاني"},
        {L"jанвар", L"феврал", L"март", L"апрел", L"мај", L"ијун", L"ијул", L"август", L"сентјабр", L"октјабр", L"нојабр", L"декабр"},
        {L"ᠨᠢᠭᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠬᠤᠶ᠋ᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠭᠤᠷᠪᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠦᠷᠪᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠠᠪᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠵᠢᠷᠭᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠤᠯᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠨᠠᠢᠮᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠶᠢᠰᠦᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠨ ᠨᠢᠭᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠨ ᠬᠤᠶ᠋ᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ"},
        {L"ديسمبر", L"نوفمبر", L"أكتوبر", L"سبتمبر", L"أغسطس", L"يوليو", L"يونيو", L"مايو", L"أبريل", L"مارس", L"فبراير", L"يناير"},
        {L"januar", L"februar", L"mart", L"april", L"maj", L"jun", L"jul", L"avgust", L"septembar", L"oktobar", L"novembar", L"decembar"},
        {L"јануар", L"фебруар", L"март", L"април", L"мај", L"јун", L"јул", L"август", L"септембар", L"октобар", L"новембар", L"децембар"}
    };
    ShortMonthNames = std::vector<std::vector<std::wstring>>
    {
        {L"jan", L"feb", L"mar", L"apr", L"may", L"jun", L"jul", L"aug", L"sep", L"oct", L"nov", L"dec"},
        {L"янв", L"фев", L"мар", L"апр", L"май", L"июн", L"июл", L"авг", L"сен", L"окт", L"ноя", L"дек"},
        {L"ene", L"feb", L"mar", L"abr", L"may", L"jun", L"jul", L"ago", L"sep", L"oct", L"nov", L"dic"},
        {L"1月", L"2月", L"3月", L"4月", L"5月", L"6月", L"7月", L"8月", L"9月", L"10月", L"11月", L"12月"},
        {L"led", L"úno", L"bře", L"dub", L"kvě", L"čvn", L"čvc", L"srp", L"zář", L"říj", L"lis", L"pro"},
        {L"jan", L"feb", L"mar", L"apr", L"maj", L"jun", L"jul", L"aug", L"sep", L"okt", L"nov", L"dec"},
        {L"Jan", L"Feb", L"Mrz", L"Apr", L"Mai", L"Jun", L"Jul", L"Aug", L"Sep", L"Okt", L"Nov", L"Dez"},
        {L"Ιαν", L"Φεβ", L"Μαρ", L"Απρ", L"Μαϊ", L"Ιουν", L"Ιουλ", L"Αυγ", L"Σεπ", L"Οκτ", L"Νοε", L"Δεκ"},
        {L"tammi", L"helmi", L"maalis", L"huhti", L"touko", L"kesä", L"heinä", L"elo", L"syys", L"loka", L"marras", L"joulu"},
        {L"janv", L"févr", L"mars", L"avr", L"mai", L"juin", L"juil", L"août", L"sept", L"oct", L"nov", L"déc"},
        {L"jan", L"febr", L"márc", L"ápr", L"máj", L"jún", L"júl", L"aug", L"szept", L"okt", L"nov", L"dec"},
        {L"gen", L"feb", L"mar", L"apr", L"mag", L"giu", L"lug", L"ago", L"set", L"ott", L"nov", L"dic"},
        {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12"},
        {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12"},
        {L"sty", L"lut", L"mar", L"kwi", L"maj", L"cze", L"lip", L"sie", L"wrz", L"paź", L"lis", L"gru"},
        {L"jan", L"fev", L"mar", L"abr", L"mai", L"jun", L"jul", L"ago", L"set", L"out", L"nov", L"dez"},
        {L"jan", L"feb", L"mar", L"apr", L"maj", L"jun", L"jul", L"aug", L"sep", L"okt", L"nov", L"dec"},
        {L"Oca", L"Şub", L"Mar", L"Nis", L"May", L"Haz", L"Tem", L"Ağu", L"Eyl", L"Eki", L"Kas", L"Ara"},
        {L"Jan", L"Feb", L"Mar", L"Apr", L"Mei", L"Jun", L"Jul", L"Agu", L"Sep", L"Okt", L"Nov", L"Des"},
        {L"Січ", L"Лют", L"Бер", L"Кві", L"Тра", L"Чер", L"Лип", L"Сер", L"Вер", L"Жов", L"Лис", L"Гру"},
        {L"jan", L"feb", L"mar", L"apr", L"maj", L"jun", L"jul", L"avg", L"sep", L"okt", L"nov", L"dec"},
        {L"janv", L"febr", L"marts", L"apr", L"maijs", L"jūn", L"jūl", L"aug", L"sept", L"okt", L"nov", L"dec"},
        {L"saus", L"vas", L"kov", L"bal", L"geg", L"birž", L"liep", L"rugp", L"rugs", L"spal", L"lapkr", L"gruod"},
        {L"Thg1", L"Thg2", L"Thg3", L"Thg4", L"Thg5", L"Thg6", L"Thg7", L"Thg8", L"Thg9", L"Thg10", L"Thg11", L"Thg12"},
        {L"yan", L"fev", L"mar", L"apr", L"may", L"iyn", L"iyl", L"avq", L"sen", L"okt", L"noy", L"dek"},
        {L"қаң", L"ақп", L"нау", L"сәу", L"мам", L"мау", L"шіл", L"там", L"қыр", L"қаз", L"қар", L"жел"},
        {L"1-р сар", L"2-р сар", L"3-р сар", L"4-р сар", L"5-р сар", L"6-р сар", L"7-р сар", L"8-р сар", L"9-р сар", L"10-р сар", L"11-р сар", L"12-р сар"},
        {L"яну", L"фев", L"мар", L"апр", L"май", L"юни", L"юли", L"авг", L"сеп", L"окт", L"ное", L"дек"},
        {L"ذو الحجة", L"ذو القعدة", L"شوال", L"رمضان", L"شعبان", L"رجب", L"جمادى الثانية", L"جمادى الأولى", L"ربيع الثاني", L"ربيع الأول", L"صفر", L"محرم"},
        {L"jan", L"feb", L"mrt", L"apr", L"mei", L"jun", L"jul", L"aug", L"sep", L"okt", L"nov", L"dec"},
        {L"sij", L"vlj", L"ožu", L"tra", L"svi", L"lip", L"srp", L"kol", L"ruj", L"lis", L"stu", L"pro"},
        {L"jan", L"feb", L"mar", L"apr", L"máj", L"jún", L"júl", L"aug", L"sep", L"okt", L"nov", L"dec"},
        {L"كانون الأول", L"تشرين الثاني", L"تشرين الأول", L"أيلول", L"آب", L"تموز", L"حزيران", L"أيار", L"نيسان", L"آذار", L"شباط", L"كانون الثاني"},
        {L"Јан", L"Фев", L"Мар", L"Апр", L"Мај", L"Ијун", L"Ијул", L"Авг", L"Сен", L"Окт", L"Ноя", L"Дек"},
        {L"ᠨᠢᠭᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠬᠤᠶ᠋ᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠭᠤᠷᠪᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠦᠷᠪᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠠᠪᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠵᠢᠷᠭᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠲᠤᠯᠤᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠨᠠᠢᠮᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠶᠢᠰᠦᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠨ ᠨᠢᠭᠡᠳᠦᠭᠡᠷ ᠰᠠᠷ᠎ᠠ", L"ᠠᠷᠪᠠᠨ ᠬᠤᠶ᠋ᠠᠳᠤᠭᠠᠷ ᠰᠠᠷ᠎ᠠ"},
        {L"ديسمبر", L"نوفمبر", L"أكتوبر", L"سبتمبر", L"أغسطس", L"يوليو", L"يونيو", L"مايو", L"أبريل", L"مارس", L"فبراير", L"يناير"},
        {L"jan", L"feb", L"mar", L"apr", L"maj", L"jun", L"jul", L"avg", L"sep", L"okt", L"nov", L"dec"},
        {L"јан.", L"феб.", L"март", L"апр.", L"мај", L"јун", L"јул", L"авг.", L"септ.", L"окт.", L"нов.", L"дец."}
    };
}

LocalInfo getLocalInfo(const _INT32 lcid)
{
    if(InfoMap.empty())
        InitLocalInfo();
    auto result = InfoMap.find(lcid);
    if(result == InfoMap.end())
        return InfoMap.at(-1);
    return InfoMap.at(lcid);
}


//LocalInfo methods

//todo сборка кода формата из сокращенного формата и разделителя
std::wstring LocalInfo::GetShortDateFormat()
{
    std::wstring result;
    for(auto i = 0; i < ShortDatePattern.size(); i++ )
    {
        switch (ShortDatePattern[i])
        {
            case L'0':
                result+= L"d";
                break;
            case L'1':
                result+= L"dd";
                break;
            case L'2':
                result+= L"m";
                break;
            case L'3':
                result+= L"mm";
                break;
            case L'4':
                result+= L"yy";
                break;
            case L'5':
                result+= L"yyyy";
                break;
        };
        if (i != ShortDatePattern.size() - 1)
            result+= DateSeparator;
    }
        return result;
}

std::vector<std::wstring> LocalInfo::GetMonthNames(const _INT16 &index, const bool isShortName)
{
    if(isShortName)
    {
        if(index < ShortMonthNames.size() && index >= 0)
            return ShortMonthNames.at(index);
        return std::vector<std::wstring>{};
    }
    if(index < MonthNames.size() && index >= 0)
        return MonthNames.at(index);
    return std::vector<std::wstring>{};
}

_INT16 LocalInfo::GetMonthNumber(const std::wstring &monthName, const bool isShortName)
{
    auto months = GetMonthNames(MonthNamesIndex, isShortName);
    for(auto i = 0; i < months.size(); i++)
        if(months.at(i) == monthName)
            return i;
    return -1;
}
std::wstring LocalInfo::GetLocMonthName(const _INT16 &index, bool shortName)
{
    std::vector<std::wstring> months;
    std::wstring monthName = L"";
    months = GetMonthNames(MonthNamesIndex, shortName);
    if(months.size() > index)
        monthName = months.at(index);
    if(shortName)
    {
        if (MonthNamesIndex == 2 || MonthNamesIndex == 10 || MonthNamesIndex == 20|| MonthNamesIndex == 22 || MonthNamesIndex == 25)
                monthName+= L".";
    }
    return monthName;
}
}
