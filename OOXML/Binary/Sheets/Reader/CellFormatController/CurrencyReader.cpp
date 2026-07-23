/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CurrencyReader.h"

#include <set>

const std::wstring CurrencyFormatTemplate = L"#,##0.00";

const std::set<std::wstring> CurrencySymbols = {
    L"\u0024",    // US Dollar
    L"\u20AC",    // Euro
    L"\u00A5",    // Japanese Yen
    L"\u00A3",    // British Pound Sterling
    L"\u0024\u0061",   // Australian Dollar
    L"\u0024\u0063",   // Canadian Dollar
    L"\u0046\u0072\u002E",  // Swiss Franc
    L"\u20BD",    // Russian Ruble
    L"\u20B8",    // Kazakh Tenge
    L"\u5143",   // Chinese Yuan
    L"\u20B1", // Philippine Peso
    L"\u20B9", // Indian Rupee
    L"\u20AA", // Israeli New Shekel
    L"\u20A7", // Spanish Peseta
    L"\u0631.\u0633.", // Saudi Riyal
    L"kr.",
    L"Ft",
    L"\u20A9", // Korean Won
    L"\u007a\u0142",// Polish Zloty
    L"R\u0024",
    L"kr",
    L"\u20BA", // Turkish Lira
    L"\u20B4",// Ukrainian Hryvnia
    L"\u20AB", // Vietnamese Dong
    L"\u20BC", // Azerbaijani Manat
    L"\u20AE", // Mongolian Tugrik
    L"\u043B\u0432.", // Bulgarian Lev
    L"NT\u0024",
    L"\u004B\u010D", // Czech Koruna
    L"Rp",
    L"\u062F.\u0639.‏", // Iraqi Dinar
    L"L",
    L"\u062C.\u0645.‏", // Egyptian Pound
    L"HK\u0024",
    L"\u062F.\u0644.‏", // Libyan Dinar
    L"Q",
    L"KM",
    L"\u062F.\u062C.‏", // Algerian Dinar
    L"\u20A1", // El Salvadoran and Costa Rican Colon
    L"B/.",
    L"\u062F.\u062A.‏", // Tunisian Dinar
    L"R",
    L"EC\u0024",
    L"\u0631.\u0639.‏", // Omani Rial
    L"Bs.S",
    L"\u0631.\u064A.‏", // Yemeni Rial
    L"FC",
    L"RSD",
    L"\u0434\u0438\u043D.", // Syrian Dinar
    L"\u062F.\u0627.‏", // UAE Dirham
    L"FCFA",
    L"\u0644.\u0644.‏", // Lebanese Pound
    L"US\u0024",
    L"CFA",
    L"\u062F.\u0643.‏", // Kuwaiti Dinar
    L"\u20B1", // Philippine Peso
    L"\u062F.\u0625.‏", // Jordanian Dinar
    L"DH",
    L"\u062F.\u0628.‏", // Bahraini Dinar
    L"\u20B2", // Paraguayan Guarani
    L"G",
    L"\u0631.\u0648.‏", // Qatari Riyal
    L"Bs",
    L"RM",
    L"C\u0024",

    /// additional symbols
    L"ADP", L"AED", L"AFA", L"AFN", L"ALL", L"AMD", L"ANG", L"AOA", L"ARS", L"ATS", L"AUD",
    L"AWG", L"AZM", L"AZN", L"BAM", L"BBD", L"BDT", L"BEF", L"BGL", L"BGN", L"BHD", L"BIF",
    L"BMD", L"BND", L"BOB", L"BOV", L"BRL", L"BSD", L"BTN", L"BWP", L"BYB", L"BYN", L"BYR", 
    L"BZD", L"CAD", L"CDF", L"CHE", L"CHF", L"CHW", L"CLF", L"CLP", L"CNY", L"COP", L"COU",
    L"CRC", L"CSD", L"CUC", L"CUP", L"CVE", L"CYP", L"CZK", L"DEM", L"DJF", L"DKK", L"DOP",
    L"DZD", L"ECS", L"ECV", L"EEK", L"EGP", L"ERN", L"ESP", L"ETB", L"EUR", L"FIM", L"FJD", 
    L"FKP", L"FRF", L"GBP", L"GEL", L"GHC", L"GHS", L"GIP", L"GMD", L"GNF", L"GRD", L"GTQ",
    L"GYD", L"HKD", L"HNL", L"HRK", L"HTG", L"HUF", L"IDR", L"IEP", L"ILS", L"INR", L"IQD",
    L"IRR", L"ISK", L"ITL", L"JMD", L"JOD", L"JPY", L"KAF", L"KES", L"KGS", L"KHR", L"KMF",
    L"KPW", L"KRW", L"KWD", L"KYD", L"KZT", L"LAK", L"LBP", L"LKR", L"LRD", L"LSL", L"LTL",
    L"LUF", L"LVL", L"LYD", L"MAD", L"MDL", L"MGA", L"MGF", L"MKD", L"MMK", L"MNT", L"MOP",
    L"MRO", L"MRU", L"MTL", L"MUR", L"MVR", L"MWK", L"MXN", L"MXV", L"MYR", L"MZM", L"MZN",
    L"NAD", L"NGN", L"NIO", L"NLG", L"NOK", L"NPR", L"NTD", L"NZD", L"OMR", L"PAB", L"PEN",
    L"PGK", L"PHP", L"PKR", L"PLN", L"PTE", L"PYG", L"QAR", L"ROL", L"RON", L"RSD", L"RUB",
    L"RUR", L"RWF", L"SAR", L"SBD", L"SCR", L"SDD", L"SDG", L"SDP", L"SEK", L"SGD", L"SHP",
    L"SIT", L"SKK", L"SLL", L"SOS", L"SPL", L"SRD", L"SRG", L"STD", L"SVC", L"SYP", L"SZL",
    L"THB", L"TJR", L"TJS", L"TMM", L"TMT", L"TND", L"TOP", L"TRL", L"TRY", L"TTD", L"TWD",
    L"TZS", L"UAH", L"UGX", L"USD", L"USN", L"USS", L"UYI", L"UYU", L"UZS", L"VEB", L"VEF", 
    L"VES", L"VND", L"VUV", L"WST", L"XAF", L"XAG", L"XAU", L"XB5", L"XBA", L"XBB", L"XBC",
    L"XBD", L"XCD", L"XDR", L"XFO", L"XFU", L"XOF", L"XPD", L"XPF", L"XPT", L"XTS", L"XXX",
    L"YER", L"YUM", L"ZAR", L"ZMK", L"ZMW", L"ZWD", L"ZWL", L"ZWN", L"ZWR"

};

bool CurrencyReader::CheckPostfix(const std::wstring &postfix)
{
    auto find = CurrencySymbols.find(postfix);
    return find != CurrencySymbols.end();
}

std::wstring CurrencyReader::GetCurrencyFormat(const std::wstring &inputFormat,const std::wstring &currency)
{
    std::wstring format;
    if(!inputFormat.empty())
    {
        format = inputFormat;
        if(currency.at(0) >= L'\u0600' &&  currency.at(0) <= L'\u06FF')
        {

            format += + L'\u200E';
            format += currency + L'\u200F';
            //format+= L'\u200E';
        }
        else
            format += currency;
    }
    else
    {
        format = CurrencyFormatTemplate;
        if(currency.at(0) >= L'\u0600' &&  currency.at(0) <= L'\u06FF')
        {

            format += + L'\u200E';
            format += currency + L'\u200F';
            //format+= L'\u200E';
        }
        else
            format += currency;
    }
    return format;
}
