import {c_oAscObjectFactory} from "../factory";

var c_oSerConstants = {
	ErrorFormat: -2,
	ErrorUnknown: -1,
	ReadOk:0,
	ReadUnknown:1,
	ErrorStream:0x55
};

export function BinaryReader(data, size) {
	this.data = data;
	this.size = size;
	this.pos = 0;
	this.cur = 0;
}

BinaryReader.prototype.Seek = function(_pos) {
	if (_pos > this.size)
		return c_oSerConstants.ErrorStream;
	this.pos = _pos;
	return c_oSerConstants.ReadOk;
};
BinaryReader.prototype.Seek2 = function(_cur) {
	if (_cur > this.size)
		return c_oSerConstants.ErrorStream;
	this.cur = _cur;
	return c_oSerConstants.ReadOk;
};
BinaryReader.prototype.Skip = function(_skip) {
	if (_skip < 0)
		return c_oSerConstants.ErrorStream;
	return this.Seek(this.pos + _skip);
};
BinaryReader.prototype.Skip2 = function(_skip) {
	if (_skip < 0)
		return c_oSerConstants.ErrorStream;
	return this.Seek2(this.cur + _skip);
};
BinaryReader.prototype.SkipRecord = function() {
	var _len = this.GetLong();
	this.Skip2(_len);
};

BinaryReader.prototype.GetUChar = function() {
	if (this.cur >= this.size)
		return 0;
	return this.data[this.cur++];
};

BinaryReader.prototype.GetBool = function() {
	var Value = this.GetUChar();
	return ( Value == 0 ? false : true );
};

BinaryReader.prototype.GetLong = function() {
	if (this.cur + 3 >= this.size)
		return 0;
	return (this.data[this.cur++] | this.data[this.cur++] << 8 | this.data[this.cur++] << 16 | this.data[this.cur++] << 24);
};

BinaryReader.prototype.GetString = function() {
	var Len = this.GetLong();
	if (this.cur + 2 * Len > this.size)
		return "";
	var t = "";
	for (var i = 0; i + 1 < 2 * Len; i+=2) {
		var uni = this.data[this.cur + i];
		uni |= this.data[this.cur + i + 1] << 8;
		t += String.fromCharCode(uni);
	}
	this.cur += 2 * Len;
	return t;
};
BinaryReader.prototype.GetCurPos = function() {
	return this.cur;
};
BinaryReader.prototype.GetSize = function() {
	return this.size;
};

BinaryReader.prototype.GetDouble = function() {
	var dRes = 0.0;
	dRes |= this.GetUChar();
	dRes |= this.GetUChar() << 8;
	dRes |= this.GetUChar() << 16;
	dRes |= this.GetUChar() << 24;
	dRes /= 100000;
	return dRes;
};
BinaryReader.prototype.GetBuffer = function(length) {
	var res = new Uint8Array(length);
	for(var i = 0 ; i < length ;++i){
		res[i] = this.data[this.cur++]
	}
	return res;
};

export function readString(reader) {
	const isWrite = reader.GetBool();
	return isWrite ? reader.GetString() : null;
}

export function readBool(reader) {
	const isWrite = reader.GetBool();
	return isWrite ? reader.GetBool() : null;
}

export function readLong(reader) {
	const isWrite = reader.GetBool();
	return isWrite ? reader.GetLong() : null;
}

export function readBuffer(reader) {
	const isWrite = reader.GetBool();
	return isWrite ? reader.GetBuffer(reader.GetLong()) : null;
}

export function readObject(reader) {
	const isWrite = reader.GetBool();
	if (isWrite) {
		const type = reader.GetUChar();
		var nStart = reader.cur;
		var nEnd = nStart + reader.GetLong() + 4;
		if (c_oAscObjectFactory[type]) {
			const object = new c_oAscObjectFactory[type];
			object.import(reader);
			return object;
		}
		reader.Seek2(nEnd);
	}
	return null;
}