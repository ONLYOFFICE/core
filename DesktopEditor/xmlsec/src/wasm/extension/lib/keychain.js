(function(window, undefined){
 
    var isWaitContentScript = true;
    document.addEventListener("DOMContentLoaded", () => {
        isWaitContentScript = false;
    });

    async function waitContentScript()
    {
        if (!isWaitContentScript)
            return;

        return new Promise(resolve => (function(){
            
            if (!isWaitContentScript)
                resolve();

            var interval = setInterval(function(){
                if (!isWaitContentScript)
                {
                    clearInterval(interval);
                    resolve();
                }
            });

        })());
    }

    var KeychainVersion = 1;
    var KeychainKeyVersion = 2;

    var Purpose = {
        Encode : 0,
        Sign   : 1
    };

    var Algorithm = {
        ed25519 : 0,
        x25519  : 1
    };

    function Key()
    {
        this.version = KeychainKeyVersion;

        this.privateKey = {
            data : "",
            salt : ""
        };
        this.publicKey = "";

        this.date;
        this.valid = true;        
    }

    function Keychain()
    {
        this.version = KeychainVersion;
        this.keys = [];
    }

    Keychain.prototype.checkExistEngine = async function() 
    {
        await waitContentScript();
        let result = await EngineWorker.isExist();
        return !!result;
    };

    Keychain.prototype.generateEncryptKeys = async function() 
    {
        await waitContentScript();
        let key = await EngineWorker.sendMessage({
            type : "generateEncryptKeys"
        });
        return key;
    };

    function EngineWorkerImpl()
    {
        this.messageCheckId = "onlyoffice-engine-check";
        this.messagesCheck = [];

        this.messageId = "onlyoffice-engine-data";
        this.messages = [];

        this.waitingCheckTime = 1000;
        this.waitingTime = 3000;

        document.addEventListener(this.messageCheckId + "-content", (event) => {
            EngineWorker._onReceiveMessage(true, false, event.detail);
        });

        document.addEventListener(this.messageId + "-content", (event) => {
            EngineWorker._onReceiveMessage(false, false, event.detail);
        });
    }

    EngineWorkerImpl.prototype._onReceiveMessage = function(isCheck, isFromTimeout, data)
    {
        let queue = isCheck ? this.messagesCheck : this.messages;
        if (0 === queue.length) {
            console.log("Error: message does not work!");
            return;
        }
        if (true !== isFromTimeout)
            clearTimeout(queue[0].id);

        queue[0].func(data);       
        queue.splice(0, 1);
    };

    EngineWorkerImpl.prototype._sendMessage = function(data, resolve)
    {
        let isData = (undefined !== data) ? true : false;
        let queue = isData ? this.messages : this.messagesCheck;
        let interval = isData ? this.waitingTime : this.waitingCheckTime;
        queue.push({
            func: resolve,
            id: setTimeout(function(){
                EngineWorker._onReceiveMessage(!isData, true);
            }, interval)
        });
        document.dispatchEvent(new CustomEvent((isData ? this.messageId : this.messageCheckId) + "-page", { detail: { data: data } }));
    };

    EngineWorkerImpl.prototype.isExist = async function()
    {
        return new Promise(resolve => (function(){
            EngineWorker._sendMessage(undefined, resolve);
        })());
    };

    EngineWorkerImpl.prototype.sendMessage = async function(data)
    {
        return new Promise(resolve => (function(){
            EngineWorker._sendMessage(data, resolve);
        })());
    };

    var EngineWorker = new EngineWorkerImpl();

    window.Asc = window.Asc || {};
    window.Asc.Keychain = new Keychain();

})(window, undefined);