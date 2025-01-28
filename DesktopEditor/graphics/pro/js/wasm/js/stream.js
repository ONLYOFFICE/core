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

function CBinaryReader(data, start, size)
{
	this.data = data;
	this.pos = start;
	this.limit = start + size;
}
CBinaryReader.prototype.readByte = function()
{
	let val = this.data[this.pos];
	this.pos += 1;
	return val;
};
CBinaryReader.prototype.readShort = function()
{
	let val = this.data[this.pos] | this.data[this.pos + 1] << 8;
	this.pos += 2;
	return val;
};
CBinaryReader.prototype.readInt = function()
{
	let val = this.data[this.pos] | this.data[this.pos + 1] << 8 | this.data[this.pos + 2] << 16 | this.data[this.pos + 3] << 24;
	this.pos += 4;
	return val;
};
CBinaryReader.prototype.readDouble = function()
{
	return this.readInt() / 100;
};
CBinaryReader.prototype.readDouble2 = function()
{
	return this.readInt() / 10000;
};
CBinaryReader.prototype.readDouble3 = function()
{
	return this.readInt() / 100000;
};
CBinaryReader.prototype.readString = function()
{
	let len = this.readInt();
	let val = String.prototype.fromUtf8(this.data, this.pos, len);
	this.pos += len;
	return val;
};
CBinaryReader.prototype.readString2 = function()
{
	let len = this.readShort();
	let val = "";
	for (let i = 0; i < len; ++i)
	{
		let c = this.readShort();
		val += String.fromCharCode(c);
	}
	return val;
};
CBinaryReader.prototype.readData = function()
{
	let len = this.readInt() - 4;
	let val = this.data.slice(this.pos, this.pos + len);
	this.pos += len;
	return val;
};
CBinaryReader.prototype.isValid = function()
{
	return (this.pos < this.limit) ? true : false;
};
CBinaryReader.prototype.Skip = function(nPos)
{
	this.pos += nPos;
};

function CBinaryWriter()
{
	this.size = 100000;
	this.dataSize = 0;
	this.buffer = new Uint8Array(this.size);
}
CBinaryWriter.prototype.checkAlloc = function(addition)
{
	if ((this.dataSize + addition) <= this.size)
		return;

	let newSize = Math.max(this.size * 2, this.size + addition);
	let newBuffer = new Uint8Array(newSize);
	newBuffer.set(this.buffer, 0);

	this.size = newSize;
	this.buffer = newBuffer;
};
CBinaryWriter.prototype.writeUint = function(value)
{
	this.checkAlloc(4);
	let val = (value>2147483647)?value-4294967296:value;
	this.buffer[this.dataSize++] = (val) & 0xFF;
	this.buffer[this.dataSize++] = (val >>> 8) & 0xFF;
	this.buffer[this.dataSize++] = (val >>> 16) & 0xFF;
	this.buffer[this.dataSize++] = (val >>> 24) & 0xFF;
};
CBinaryWriter.prototype.writeString = function(value)
{
	let valueUtf8 = value.toUtf8();
	this.checkAlloc(valueUtf8.length);
	this.buffer.set(valueUtf8, this.dataSize);
	this.dataSize += valueUtf8.length;
};
