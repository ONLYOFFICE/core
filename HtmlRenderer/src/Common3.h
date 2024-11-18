/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#ifndef _ASC_HTMLRENDERER_COMMON3_H_
#define _ASC_HTMLRENDERER_COMMON3_H_

#include <unordered_map>

namespace NSHtmlRenderer
{

unsigned int PUNCTUATION_FLAG_BASE               = 0x0001;
unsigned int PUNCTUATION_FLAG_CANT_BE_AT_BEGIN   = 0x0010;
unsigned int PUNCTUATION_FLAG_CANT_BE_AT_END     = 0x0020;
unsigned int PUNCTUATION_FLAG_EAST_ASIAN         = 0x0100;
unsigned int PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E = 0x0002;
unsigned int PUNCTUATION_FLAG_CANT_BE_AT_END_E   = 0x0004;

std::unordered_map<unsigned int, unsigned int> g_aPunctuation;

bool isPuctuation(unsigned int nUnicode)
{
	if (g_aPunctuation.empty())
	{
		g_aPunctuation[0x0021] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // !
		g_aPunctuation[0x0022] = PUNCTUATION_FLAG_BASE;                                     // "
		g_aPunctuation[0x0023] = PUNCTUATION_FLAG_BASE;                                     // #
		g_aPunctuation[0x0024] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // $
		g_aPunctuation[0x0025] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // %
		g_aPunctuation[0x0026] = PUNCTUATION_FLAG_BASE;                                     // &
		g_aPunctuation[0x0027] = PUNCTUATION_FLAG_BASE;                                     // '
		g_aPunctuation[0x0028] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // (
		g_aPunctuation[0x0029] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // )
		g_aPunctuation[0x002A] = PUNCTUATION_FLAG_BASE;                                     // *
		g_aPunctuation[0x002B] = PUNCTUATION_FLAG_BASE;                                     // +
		g_aPunctuation[0x002C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ,
		g_aPunctuation[0x002D] = PUNCTUATION_FLAG_BASE;                                     // -
		g_aPunctuation[0x002E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // .
		g_aPunctuation[0x002F] = PUNCTUATION_FLAG_BASE;                                     // /
		g_aPunctuation[0x003A] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // :
		g_aPunctuation[0x003B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ;
		g_aPunctuation[0x003C] = PUNCTUATION_FLAG_BASE;                                     // <
		g_aPunctuation[0x003D] = PUNCTUATION_FLAG_BASE;                                     // =
		g_aPunctuation[0x003E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // >
		g_aPunctuation[0x003F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ?
		g_aPunctuation[0x0040] = PUNCTUATION_FLAG_BASE;                                     // @
		g_aPunctuation[0x005B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // [
		g_aPunctuation[0x005C] = PUNCTUATION_FLAG_BASE;                                     // "\"
		g_aPunctuation[0x005D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ]
		g_aPunctuation[0x005E] = PUNCTUATION_FLAG_BASE;                                     // ^
		g_aPunctuation[0x005F] = PUNCTUATION_FLAG_BASE;                                     // _
		g_aPunctuation[0x0060] = PUNCTUATION_FLAG_BASE;                                     // `
		g_aPunctuation[0x007B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // {
		g_aPunctuation[0x007C] = PUNCTUATION_FLAG_BASE;                                     // |
		g_aPunctuation[0x007D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // }
		g_aPunctuation[0x007E] = PUNCTUATION_FLAG_BASE;                                     // ~

		g_aPunctuation[0x00A1] = PUNCTUATION_FLAG_BASE;                                     // ¡
		g_aPunctuation[0x00A2] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ¢
		g_aPunctuation[0x00A3] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // £
		g_aPunctuation[0x00A4] = PUNCTUATION_FLAG_BASE;                                     // ¤
		g_aPunctuation[0x00A5] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // ¥
		g_aPunctuation[0x00A6] = PUNCTUATION_FLAG_BASE;                                     // ¦
		g_aPunctuation[0x00A7] = PUNCTUATION_FLAG_BASE;                                     // §
		g_aPunctuation[0x00A8] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ¨
		g_aPunctuation[0x00A9] = PUNCTUATION_FLAG_BASE;                                     // ©
		g_aPunctuation[0x00AA] = PUNCTUATION_FLAG_BASE;                                     // ª
		g_aPunctuation[0x00AB] = PUNCTUATION_FLAG_BASE;                                     // «
		g_aPunctuation[0x00AC] = PUNCTUATION_FLAG_BASE;                                     // ¬
		g_aPunctuation[0x00AD] = PUNCTUATION_FLAG_BASE;                                     // ­
		g_aPunctuation[0x00AE] = PUNCTUATION_FLAG_BASE;                                     // ®
		g_aPunctuation[0x00AF] = PUNCTUATION_FLAG_BASE;                                     // ¯
		g_aPunctuation[0x00B0] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // °
		g_aPunctuation[0x00B1] = PUNCTUATION_FLAG_BASE;                                     // ±
		g_aPunctuation[0x00B4] = PUNCTUATION_FLAG_BASE;                                     // ´
		g_aPunctuation[0x00B6] = PUNCTUATION_FLAG_BASE;                                     // ¶
		g_aPunctuation[0x00B7] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ·
		g_aPunctuation[0x00B8] = PUNCTUATION_FLAG_BASE;                                     // ¸
		g_aPunctuation[0x00BA] = PUNCTUATION_FLAG_BASE;                                     // º
		g_aPunctuation[0x00BB] = PUNCTUATION_FLAG_BASE;                                     // »
		g_aPunctuation[0x00BB] = PUNCTUATION_FLAG_BASE;                                     // »
		g_aPunctuation[0x00BF] = PUNCTUATION_FLAG_BASE;                                     // ¿

		g_aPunctuation[0x2010] = PUNCTUATION_FLAG_BASE;                                     // ‐
		g_aPunctuation[0x2011] = PUNCTUATION_FLAG_BASE;                                     // ‑
		g_aPunctuation[0x2012] = PUNCTUATION_FLAG_BASE;                                     // ‒
		g_aPunctuation[0x2013] = PUNCTUATION_FLAG_BASE;                                     // –
		g_aPunctuation[0x2014] = PUNCTUATION_FLAG_BASE;                                     // —
		g_aPunctuation[0x2015] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ―
		g_aPunctuation[0x2016] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ‖
		g_aPunctuation[0x2017] = PUNCTUATION_FLAG_BASE;                                     // ‗
		g_aPunctuation[0x2018] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // ‘
		g_aPunctuation[0x2019] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ’
		g_aPunctuation[0x201A] = PUNCTUATION_FLAG_BASE;                                     // ‚
		g_aPunctuation[0x201B] = PUNCTUATION_FLAG_BASE;                                     // ‛
		g_aPunctuation[0x201C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // “
		g_aPunctuation[0x201D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ”
		g_aPunctuation[0x201E] = PUNCTUATION_FLAG_BASE;                                     // „
		g_aPunctuation[0x201F] = PUNCTUATION_FLAG_BASE;                                     // ‟
		g_aPunctuation[0x2020] = PUNCTUATION_FLAG_BASE;                                     // †
		g_aPunctuation[0x2021] = PUNCTUATION_FLAG_BASE;                                     // ‡
		g_aPunctuation[0x2022] = PUNCTUATION_FLAG_BASE;                                     // •
		g_aPunctuation[0x2023] = PUNCTUATION_FLAG_BASE;                                     // ‣
		g_aPunctuation[0x2024] = PUNCTUATION_FLAG_BASE;                                     // ․
		g_aPunctuation[0x2025] = PUNCTUATION_FLAG_BASE;                                     // ‥
		g_aPunctuation[0x2026] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // …
		g_aPunctuation[0x2027] = PUNCTUATION_FLAG_BASE;                                     // ‧
		g_aPunctuation[0x2030] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ‰
		g_aPunctuation[0x2031] = PUNCTUATION_FLAG_BASE;                                     // ‱
		g_aPunctuation[0x2032] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ′
		g_aPunctuation[0x2033] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ″
		g_aPunctuation[0x2034] = PUNCTUATION_FLAG_BASE;                                     // ‴
		g_aPunctuation[0x2035] = PUNCTUATION_FLAG_BASE;                                     // ‵
		g_aPunctuation[0x2036] = PUNCTUATION_FLAG_BASE;                                     // ‶
		g_aPunctuation[0x2037] = PUNCTUATION_FLAG_BASE;                                     // ‷
		g_aPunctuation[0x2038] = PUNCTUATION_FLAG_BASE;                                     // ‸
		g_aPunctuation[0x2039] = PUNCTUATION_FLAG_BASE;                                     // ‹
		g_aPunctuation[0x203A] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ›
		g_aPunctuation[0x203B] = PUNCTUATION_FLAG_BASE;                                     // ※
		g_aPunctuation[0x203C] = PUNCTUATION_FLAG_BASE;                                     // ‼
		g_aPunctuation[0x203D] = PUNCTUATION_FLAG_BASE;                                     // ‽
		g_aPunctuation[0x203E] = PUNCTUATION_FLAG_BASE;                                     // ‾
		g_aPunctuation[0x203F] = PUNCTUATION_FLAG_BASE;                                     // ‿
		g_aPunctuation[0x2040] = PUNCTUATION_FLAG_BASE;                                     // ⁀
		g_aPunctuation[0x2041] = PUNCTUATION_FLAG_BASE;                                     // ⁁
		g_aPunctuation[0x2042] = PUNCTUATION_FLAG_BASE;                                     // ⁂
		g_aPunctuation[0x2043] = PUNCTUATION_FLAG_BASE;                                     // ⁃
		g_aPunctuation[0x2044] = PUNCTUATION_FLAG_BASE;                                     // ⁄
		g_aPunctuation[0x2045] = PUNCTUATION_FLAG_BASE;                                     // ⁅
		g_aPunctuation[0x2046] = PUNCTUATION_FLAG_BASE;                                     // ⁆
		g_aPunctuation[0x2047] = PUNCTUATION_FLAG_BASE;                                     // ⁇
		g_aPunctuation[0x2048] = PUNCTUATION_FLAG_BASE;                                     // ⁈
		g_aPunctuation[0x2049] = PUNCTUATION_FLAG_BASE;                                     // ⁉
		g_aPunctuation[0x204A] = PUNCTUATION_FLAG_BASE;                                     // ⁊
		g_aPunctuation[0x204B] = PUNCTUATION_FLAG_BASE;                                     // ⁋
		g_aPunctuation[0x204C] = PUNCTUATION_FLAG_BASE;                                     // ⁌
		g_aPunctuation[0x204D] = PUNCTUATION_FLAG_BASE;                                     // ⁍
		g_aPunctuation[0x204E] = PUNCTUATION_FLAG_BASE;                                     // ⁎
		g_aPunctuation[0x204F] = PUNCTUATION_FLAG_BASE;                                     // ⁏
		g_aPunctuation[0x2050] = PUNCTUATION_FLAG_BASE;                                     // ⁐
		g_aPunctuation[0x2051] = PUNCTUATION_FLAG_BASE;                                     // ⁑
		g_aPunctuation[0x2052] = PUNCTUATION_FLAG_BASE;                                     // ⁒
		g_aPunctuation[0x2053] = PUNCTUATION_FLAG_BASE;                                     // ⁓
		g_aPunctuation[0x2054] = PUNCTUATION_FLAG_BASE;                                     // ⁔
		g_aPunctuation[0x2055] = PUNCTUATION_FLAG_BASE;                                     // ⁕
		g_aPunctuation[0x2056] = PUNCTUATION_FLAG_BASE;                                     // ⁖
		g_aPunctuation[0x2057] = PUNCTUATION_FLAG_BASE;                                     // ⁗
		g_aPunctuation[0x2058] = PUNCTUATION_FLAG_BASE;                                     // ⁘
		g_aPunctuation[0x2059] = PUNCTUATION_FLAG_BASE;                                     // ⁙
		g_aPunctuation[0x205A] = PUNCTUATION_FLAG_BASE;                                     // ⁚
		g_aPunctuation[0x205B] = PUNCTUATION_FLAG_BASE;                                     // ⁛
		g_aPunctuation[0x205C] = PUNCTUATION_FLAG_BASE;                                     // ⁜
		g_aPunctuation[0x205D] = PUNCTUATION_FLAG_BASE;                                     // ⁝
		g_aPunctuation[0x205E] = PUNCTUATION_FLAG_BASE;                                     // ⁞
		g_aPunctuation[0x2420] = PUNCTUATION_FLAG_BASE;                                     // ␠
		g_aPunctuation[0x2421] = PUNCTUATION_FLAG_BASE;                                     // ␡
		g_aPunctuation[0x2422] = PUNCTUATION_FLAG_BASE;                                     // ␢
		g_aPunctuation[0x2423] = PUNCTUATION_FLAG_BASE;                                     // ␣
		g_aPunctuation[0x2424] = PUNCTUATION_FLAG_BASE;                                     // ␤
		g_aPunctuation[0x2425] = PUNCTUATION_FLAG_BASE;                                     // ␥
		g_aPunctuation[0x2426] = PUNCTUATION_FLAG_BASE;                                     // ␦

		// Не смотря на то что следующий набор символов идет в блоке CJK Symbols and Punctuation
		// Word не считает их как EastAsian script (w:lang->w:eastAsian)

		g_aPunctuation[0x3001] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 、
		g_aPunctuation[0x3002] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 。
		g_aPunctuation[0x3003] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 〃
		g_aPunctuation[0x3004] = PUNCTUATION_FLAG_BASE;                                     // 〄
		g_aPunctuation[0x3005] = PUNCTUATION_FLAG_BASE;                                     // 々
		g_aPunctuation[0x3006] = PUNCTUATION_FLAG_BASE;                                     // 〆
		g_aPunctuation[0x3007] = PUNCTUATION_FLAG_BASE;                                     // 〇
		g_aPunctuation[0x3008] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 〈
		g_aPunctuation[0x3009] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 〉
		g_aPunctuation[0x300A] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 《
		g_aPunctuation[0x300B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 》
		g_aPunctuation[0x300C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 「
		g_aPunctuation[0x300D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 」
		g_aPunctuation[0x300E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 『
		g_aPunctuation[0x300F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 』
		g_aPunctuation[0x3010] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 【
		g_aPunctuation[0x3011] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 】
		g_aPunctuation[0x3012] = PUNCTUATION_FLAG_BASE;                                     // 〒
		g_aPunctuation[0x3013] = PUNCTUATION_FLAG_BASE;                                     // 〓
		g_aPunctuation[0x3014] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   //〔
		g_aPunctuation[0x3015] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 〕
		g_aPunctuation[0x3016] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   //〖
		g_aPunctuation[0x3017] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 〗
		g_aPunctuation[0x3018] = PUNCTUATION_FLAG_BASE;                                     // 〘
		g_aPunctuation[0x3019] = PUNCTUATION_FLAG_BASE;                                     // 〙
		g_aPunctuation[0x301A] = PUNCTUATION_FLAG_BASE;                                     // 〚
		g_aPunctuation[0x301B] = PUNCTUATION_FLAG_BASE;                                     // 〛
		g_aPunctuation[0x301C] = PUNCTUATION_FLAG_BASE;                                     // 〜
		g_aPunctuation[0x301D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_END;   // 〝
		g_aPunctuation[0x301E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // 〞
		g_aPunctuation[0x301F] = PUNCTUATION_FLAG_BASE;                                     // 〟

		g_aPunctuation[0xFF01] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ！
		g_aPunctuation[0xFF02] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ＂
		g_aPunctuation[0xFF03] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＃
		g_aPunctuation[0xFF04] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END;   // ＄
		g_aPunctuation[0xFF05] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ％
		g_aPunctuation[0xFF06] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＆
		g_aPunctuation[0xFF07] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ＇
		g_aPunctuation[0xFF08] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // （
		g_aPunctuation[0xFF09] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // )
		g_aPunctuation[0xFF0A] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＊
		g_aPunctuation[0xFF0B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＋
		g_aPunctuation[0xFF0C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ，
		g_aPunctuation[0xFF0D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // －
		g_aPunctuation[0xFF0E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ．
		g_aPunctuation[0xFF0F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ／
		g_aPunctuation[0xFF1A] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ：
		g_aPunctuation[0xFF1B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ；
		g_aPunctuation[0xFF1C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＜
		g_aPunctuation[0xFF1D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＝
		g_aPunctuation[0xFF1E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＞
		g_aPunctuation[0xFF1F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ？
		g_aPunctuation[0xFF20] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＠
		g_aPunctuation[0xFF3B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // ［
		g_aPunctuation[0xFF3C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＼
		g_aPunctuation[0xFF3D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ］
		g_aPunctuation[0xFF3E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＾
		g_aPunctuation[0xFF3F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ＿
		g_aPunctuation[0xFF40] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ｀
		g_aPunctuation[0xFF5B] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END | PUNCTUATION_FLAG_CANT_BE_AT_END_E;   // ｛
		g_aPunctuation[0xFF5C] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ｜
		g_aPunctuation[0xFF5D] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN_E; // ｝
		g_aPunctuation[0xFF5E] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ～
		g_aPunctuation[0xFF5F] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ｟
		g_aPunctuation[0xFF60] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ｠
		g_aPunctuation[0xFF61] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ｡
		g_aPunctuation[0xFF62] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ｢
		g_aPunctuation[0xFF63] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ｣
		g_aPunctuation[0xFF64] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ､
		g_aPunctuation[0xFF65] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ･
		g_aPunctuation[0xFFE0] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_BEGIN; // ￠
		g_aPunctuation[0xFFE1] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END;   // ￡
		g_aPunctuation[0xFFE2] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￢
		g_aPunctuation[0xFFE3] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￣
		g_aPunctuation[0xFFE4] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￤
		g_aPunctuation[0xFFE5] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN | PUNCTUATION_FLAG_CANT_BE_AT_END;   // ￥
		g_aPunctuation[0xFFE6] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￦
		g_aPunctuation[0xFFE8] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￨
		g_aPunctuation[0xFFE9] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￩
		g_aPunctuation[0xFFEA] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￪
		g_aPunctuation[0xFFEB] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￫
		g_aPunctuation[0xFFEC] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￬
		g_aPunctuation[0xFFED] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￭
		g_aPunctuation[0xFFEE] = PUNCTUATION_FLAG_BASE | PUNCTUATION_FLAG_EAST_ASIAN;                                     // ￮
	}

	return g_aPunctuation.find(nUnicode) != g_aPunctuation.end();
}

}

#endif // _ASC_HTMLRENDERER_COMMON3_H_
