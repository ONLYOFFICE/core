CFontFileLoader.prototype.LoadFontNative = function()
{
    var __font_data_idx = g_fonts_streams.length;
    var _data = window.native.GetFontBinary(this.Id);
    g_fonts_streams[__font_data_idx] = new FT_Stream(_data, _data.length);
    this.SetStreamIndex(__font_data_idx);
    this.Status = 0;
};

BinaryFileReader.prototype.ReadNative = function(data)
{
	try
	{
		this.stream = new FT_Stream2(data, data.length);
		this.PreLoadPrepare();
		this.ReadMainTable();
		this.PostLoadPrepare();
	}
	catch(e)
	{
		if(e.message == g_sErrorCharCountMessage)
			return false;
		else
			throw e;
	}
	return true;
};

CChartAsGroup.prototype.draw = function(graphics, pageIndex)
{
    return;
};

function CCacheManager()
{
}

var NATIVE_DOCUMENT_TYPE = "spreadsheet";