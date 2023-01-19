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

template< class InIt, class OutIt > inline
void utf8_encode( InIt in, const InIt end, OutIt out )
{
    while( !(in == end) ) {
        unsigned long wc = static_cast<wchar_t>(*in); ++in;
    over:
        if( wc < 0x80 ) {
            *out = static_cast<char>(wc); ++out;
            continue;
        }

        if( sizeof(wchar_t) == 2 && 
            wc >= 0xD800 && wc < 0xE000 ) 
        {//handle surrogates for UTF-16
            if( wc >= 0xDC00 ) { wc = '?'; goto over; }
            if( in == end ) return;
            unsigned long lo = static_cast<wchar_t>(*in); ++in;
            if( lo >= 0xDC00 && wc < 0xE000 ) {
                wc  = 0x10000 + ((wc & 0x3FF) << 10 | lo & 0x3FF);
            } else { 
                *out = '?'; ++out; wc = lo;
                goto over;
            }
        }

        char c; int shift; 
        if( wc < 0x800 )     { shift = 6;  c = ('\xFF' << 6); } else
        if( wc < 0x10000 )   { shift = 12; c = ('\xFF' << 5); } else
        if( wc < 0x200000 )  { shift = 18; c = ('\xFF' << 4); } else
        if( wc < 0x4000000 ) { shift = 24; c = ('\xFF' << 3); } else 
                             { shift = 30; c = ('\xFF' << 2); }
        do {
            c |= (wc >> shift) & 0x3f;
            *out = c; ++out;
            c = char(0x80); shift -= 6;
        } while( shift >= 0 );
    }
}

template< class InIt, class OutIt > inline
void utf8_decode( InIt in, const InIt end, OutIt out )
{
    int cnt;
    for(; !(in == end); ++in) {
        unsigned long wc = static_cast<unsigned char>(*in);
    over:
        if( wc & 0x80 ) {
            if( 0xC0 == (0xE0 & wc) ) { cnt = 1; wc &= ~0xE0; } else
            if( 0xE0 == (0xF0 & wc) ) { cnt = 2; wc &= ~0xF0; } else
            if( 0xF0 == (0xF8 & wc) ) { cnt = 3; wc &= ~0xF8; } else
            if( 0xF8 == (0xFC & wc) ) { cnt = 4; wc &= ~0xFC; } else
            if( 0xFC == (0xFE & wc) ) { cnt = 5; wc &= ~0xFE; } else
                { *out = wchar_t('?'); ++out; continue; };//invalid start code
            if( 0 == wc ) wc = ~0UL;//codepoint encoded with overlong sequence
            do {
                if( ++in == end ) return;
                unsigned char c = static_cast<unsigned char>(*in);
                if( 0x80 != (0xC0 & c) )
                    { *out = static_cast<wchar_t>(wc); ++out; wc = c; goto over; }
                wc <<= 6; wc |= c & ~0xC0;
            } while( --cnt );
            if( 0x80000000 & wc ) wc = '?';//codepoint exceeds unicode range
            if( sizeof(wchar_t) == 2 && wc > 0xFFFF )
            {//handle surrogates for UTF-16
                wc -= 0x10000;
                *out = static_cast<wchar_t>(0xD800 | ((wc >> 10) & 0x3FF)); ++out;
                *out = static_cast<wchar_t>(0xDC00 | (wc & 0x3FF)); ++out;
                continue;
            }
        }
        *out = static_cast<wchar_t>(wc); ++out;
    }
}