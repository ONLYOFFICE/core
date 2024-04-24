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

var Uint8Array = Array;

function jsc_toBase64(srcData)
{
	var arrayBase64 = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+", "/"];
	var nOffset = 0;
	var nSrcLen = srcData.length;

	var nWritten = 0;
	var nLen1    = (((nSrcLen / 3) >> 0) * 4);
	var nLen2    = (nLen1 / 76) >> 0;
	var nLen3    = 19;
	var srcInd   = 0;
	var dstStr   = [];

	var _s = "";
	for (var i = 0; i <= nLen2; i++)
	{
		if (i == nLen2)
			nLen3 = ((nLen1 % 76) / 4) >> 0;

		for (var j = 0; j < nLen3; j++)
		{
			var dwCurr = 0;
			for (var n = 0; n < 3; n++)
			{
				dwCurr |= srcData[srcInd++ + nOffset];
				dwCurr <<= 8;
			}

			_s = "";
			for (var k = 0; k < 4; k++)
			{
				var b = (dwCurr >>> 26) & 0xFF;
				_s += arrayBase64[b];
				dwCurr <<= 6;
				dwCurr &= 0xFFFFFFFF;
			}
			dstStr.push(_s);
		}
	}
	nLen2 = (nSrcLen % 3 != 0) ? (nSrcLen % 3 + 1) : 0;
	if (nLen2)
	{
		var dwCurr = 0;
		for (var n = 0; n < 3; n++)
		{
			if (n < (nSrcLen % 3))
				dwCurr |= srcData[srcInd++ + nOffset];
			dwCurr <<= 8;
		}

		_s = "";
		for (var k = 0; k < nLen2; k++)
		{
			var b = (dwCurr >>> 26) & 0xFF;
			_s += arrayBase64[b];
			dwCurr <<= 6;
		}

		nLen3 = (nLen2 != 0) ? 4 - nLen2 : 0;
		for (var j = 0; j < nLen3; j++)
		{
			_s += '=';
		}
		dstStr.push(_s);
	}

	return dstStr.join("");
};

function jsc_fromBase64(szSrc, len)
{
	function DecodeBase64Char(ch)
	{
	    if (ch >= 65 && ch <= 90)
	        return ch - 65;
	    if (ch >= 97 && ch <= 122)
	        return ch - 71;
	    if (ch >= 48 && ch <= 57)
	        return ch + 4;
	    if (ch == 43)
	        return 62;
	    if (ch == 47)
	        return 63;
	    return -1;
	}

    var srcLen = szSrc.length;
    var nWritten = 0;

    var arr = new Array(len === undefined ? srcLen : len);
    var dstPx = arr;
    var index = 0;
    var limit = 0;

    while (index < srcLen)
    {
        var dwCurr = 0;
        var i;
        var nBits = 0;
        for (i=0; i<4; i++)
        {
            if (index >= srcLen)
                break;
            var nCh = DecodeBase64Char(szSrc.charCodeAt(index++));
            if (nCh == -1)
            {
                i--;
                continue;
            }
            dwCurr <<= 6;
            dwCurr |= nCh;
            nBits += 6;
        }

        dwCurr <<= 24-nBits;
        limit = nBits>>>3;
        for (i=0; i<limit; i++)
        {
            dstPx[nWritten++] = ((dwCurr & 0x00ff0000) >>> 16);
            dwCurr <<= 8;
        }
    }

    return arr;
};