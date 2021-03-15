//module

self.raster = null;
function onMessageEvent(data, port)
{
    if (data.type == "init")
    {
        if (self.raster)
            return;
        self.raster = new Raster();
        self.raster.init();
        return;
    }

    if (!self.raster)
        return;

    self.raster.messages.push(data);
    if (port)
        self.raster.ports.push(port);

    if (1 < self.raster.messages.length)
    {
        // значит еще грузим что-то
        return;
    }

    self.raster.checkMessage();
}

self.onconnect = function(e)
{
    var port = e.ports[0];
    port.onmessage = function(e) {
        onMessageEvent(e.data, port);
    }    
};
self.onmessage = function(e)
{
    onMessageEvent(e.data);
};
self.engineInit = false;
self.onEngineInit = function()
{
	self.engineInit = true;
	if (self.raster)
	{
		self.raster.init();
		self.raster.checkMessage();
	}
};

function Raster()
{
	this.engine = 0;
	this.init = function()
	{
		if (0 == this.engine && self.engineInit)
			this.engine = this.createEngine();
	};
	this.CreateEngine = function()
	{
		return Module._CxImage_Create();
	};
}
