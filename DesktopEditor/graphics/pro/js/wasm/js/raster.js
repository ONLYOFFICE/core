var printErr = undefined;
var print    = undefined;

var fetch = self.fetch;
var getBinaryPromise = null;
if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
{
    fetch = undefined; // fetch not support file:/// scheme
    getBinaryPromise = function() {
        var wasmPath = "ascdesktop://raster/" + wasmBinaryFile.substr(8);
        return new Promise(function (resolve, reject) {

            var xhr = new XMLHttpRequest();
            xhr.open('GET', wasmPath, true);
            xhr.responseType = 'arraybuffer';

            if (xhr.overrideMimeType)
                xhr.overrideMimeType('text/plain; charset=x-user-defined');
            else
                xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

            xhr.onload = function () {
                if (this.status == 200) {
                    resolve(new Uint8Array(this.response));
                }
            };
            xhr.send(null);
        });
    }
}
else
{
    getBinaryPromise = function() { return getBinaryPromise2(); }
}

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
    this.messages = [];
    this.ports = [];
    this.engine = 0;

    this.init = function()
    {
        if (0 == this.engine && self.engineInit)
            this.engine = this.createEngine();
    };
    this.checkMessage = function()
    {
        if (0 == this.messages.length || !self.engineInit)
            return;

        var m = this.messages[0];

        switch (m.type)
        {
            case "encode2RGBA":
            {
                this.Encode2RGBA(m);
                break;
            }
            case "decode":
            {
                this.Decode(m);
                break;
            }
            case "height"
            {
                this.height(m);
                break;
            }
            case "width"
            {
                this.width(m);
                break;
            }
            default:
                break;
        }
        this.messages.shift();
    };
    
    this.createEngine = function()
    {
        return Module._CxImage_Create();
    };
    this.destroyEngine = function()
    {
        Module._CxImage_Destroy(this.engine);
    };
    this.Encode2RGBA = function(data)
    {
    };
    this.Decode = function(data)
    {
    };
    this.height = function(data)
    {
        data.height = Module._CxImage_GetHeight(this.engine);
        this.sendAnswer(data);
    };
    this.width = function(data)
    {
        data.width = Module._CxImage_GetWidth(this.engine);
        this.sendAnswer(data);
    };
    this.sendAnswer = function(data)
    {
        if (self.raster.ports.length == 0)
        {
            self.postMessage(data);
        }
        else
        {
            var port = self.raster.ports.shift();
            port.postMessage(data);
        }
        setTimeout(function(){
            self.raster.checkMessage();
        }, 1);
    };
}
