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

(function(window, undefined) {

window['AscFonts'] = window['AscFonts'] || {};
let AscFonts = window['AscFonts'];

//string_utf8

//module

AscFonts.checkSystemFonts = async function()
{
	self.queryLocalFonts()
		.then(async function (fonts) {
            AscFonts.systemFonts = fonts;
            Module["_parseSystemFonts"](fonts.length);
        })
        .catch(function (err) {
            console.error(err.name, err.message);
        });
};

AscFonts.getSystemFont = async function(index)
{
	let buffer = await (await AscFonts.systemFonts[index].blob()).arrayBuffer();
	let array8 = new Uint8Array(buffer);
	var pointer = Module["_malloc"](array8.length);
	Module["HEAP8"].set(array8, pointer);
	return Module["_createFontBinary"](index, pointer, array8.length);

	AscFonts.systemFonts[index].blob()
		.then(async function (blob) {
			let buffer = await blob.arrayBuffer();
			let array8 = new Uint8Array(buffer);

			var pointer = Module["_malloc"](array8.length);
			Module["HEAP8"].set(array8, pointer);
			
			return Module["_createFontBinary"](index, pointer, array8.length);
		});
};

})(window, undefined);
