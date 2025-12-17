import {c_oAscObjectFactory} from "../factory";

export function BinaryWriter(bIsNoInit)
{
	this.data = null;
	this.len  = 0;
	this.pos  = 0;

	if (true !== bIsNoInit)
		this.Init();
}
BinaryWriter.prototype.Init = function(len)
{
	this.len  = (len === undefined) ? 1024 * 1024 * 5 : len;
	this.data = new Uint8Array(this.len);
	this.pos  = 0;
}

BinaryWriter.prototype.CheckSize          = function(count)
{
	if (this.pos + count >= this.len)
	{
		var oldData = this.data;

		this.len  = Math.max(this.len * 2, this.pos + ((3 * count / 2) >> 0));
		this.data = new Uint8Array(this.len);

		for (var i = 0; i < this.pos; i++)
			this.data[i] = oldData[i];
	}
}
BinaryWriter.prototype.GetData   = function()
{
	var len = this.GetCurPosition();
	var res = new Uint8Array(len);

	for (var i = 0; i < len; i++)
		res[i] = this.data[i];
	return res;
}
BinaryWriter.prototype.GetCurPosition     = function()
{
	return this.pos;
}
BinaryWriter.prototype.Seek               = function(nPos)
{
	this.pos = nPos;
}
BinaryWriter.prototype.Skip               = function(nDif)
{
	this.pos += nDif;
}
BinaryWriter.prototype.WriteBool          = function(val)
{
	this.CheckSize(1);
	if (false == val)
		this.data[this.pos++] = 0;
	else
		this.data[this.pos++] = 1;
}
BinaryWriter.prototype.WriteByte          = function(val)
{
	this.CheckSize(1);
	this.data[this.pos++] = val;
}
BinaryWriter.prototype.WriteLong          = function(val)
{
	this.CheckSize(4);
	this.data[this.pos++] = (val) & 0xFF;
	this.data[this.pos++] = (val >>> 8) & 0xFF;
	this.data[this.pos++] = (val >>> 16) & 0xFF;
	this.data[this.pos++] = (val >>> 24) & 0xFF;
}
BinaryWriter.prototype.WriteDouble        = function(val)
{
	this.CheckSize(4);
	var lval              = ((val * 100000) >> 0) & 0xFFFFFFFF;
	this.data[this.pos++] = (lval) & 0xFF;
	this.data[this.pos++] = (lval >>> 8) & 0xFF;
	this.data[this.pos++] = (lval >>> 16) & 0xFF;
	this.data[this.pos++] = (lval >>> 24) & 0xFF;
}
BinaryWriter.prototype.WriteString        = function(text)
{
	if ("string" != typeof text)
		text = text + "";

	var count = text.length & 0xFFFF;
	this.CheckSize(2 * count + 2);
	this.data[this.pos++] = count & 0xFF;
	this.data[this.pos++] = (count >>> 8) & 0xFF;
	for (var i = 0; i < count; i++)
	{
		var c                 = text.charCodeAt(i) & 0xFFFF;
		this.data[this.pos++] = c & 0xFF;
		this.data[this.pos++] = (c >>> 8) & 0xFF;
	}
};
BinaryWriter.prototype.WriteBuffer = function(data, _pos, count)
{
	this.CheckSize(count);
	for (var i = 0; i < count; i++)
	{
		this.data[this.pos++] = data[_pos + i];
	}
}
BinaryWriter.prototype.WriteItem = function(type, fWrite)
{
	this.WriteByte(type);
	this.WriteItemWithLength(fWrite);
};
BinaryWriter.prototype.WriteItemWithLength = function(fWrite)
{
	var nStart = this.WriteItemWithLengthStart();
	fWrite();
	this.WriteItemWithLengthEnd(nStart);
};
BinaryWriter.prototype.WriteItemWithLengthStart = function()
{
	var nStart = this.GetCurPosition();
	this.Skip(4);
	return nStart;
};
BinaryWriter.prototype.WriteItemWithLengthEnd = function(nStart)
{
	var nEnd = this.GetCurPosition();
	this.Seek(nStart);
	this.WriteLong(nEnd - nStart - 4);
	this.Seek(nEnd);
};

export function writeString(writer, str) {
	const isWrite = typeof str === "string";
	writer.WriteBool(isWrite);
	if (isWrite) {
		writer.WriteString(str);
	}
}
export function writeBool(writer, bool) {
	const isWrite = typeof bool === "boolean";
	writer.WriteBool(isWrite);
	if (isWrite) {
		writer.WriteBool(bool);
	}
}
export function writeLong(writer, int) {
	const isWrite = typeof int === "number";
	writer.WriteBool(isWrite);
	if (isWrite) {
		writer.WriteLong(int);
	}
}
export function writeBuffer(writer, buffer) {
	const isWrite = buffer instanceof Uint8Array;
	writer.WriteBool(isWrite);
	if (isWrite) {
		writer.WriteLong(buffer.length);
		writer.WriteBuffer(buffer, 0, buffer.length);
	}
}
export function writeObject(writer, object) {
	let isWrite = false;
	const type = object && object.getType();
	if (c_oAscObjectFactory[type]) {
		isWrite = true;
	}
	writer.WriteBool(isWrite);
	if (isWrite) {
		writer.WriteItem(type, function () {
			object.export(writer);
		});
	}
}