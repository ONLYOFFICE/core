(function(window, undefined){

    function Engine()
	{
	}

	Engine.prototype.generateKeys = async function(alg, password, salt) 
    {
    };

    Engine.prototype.changePassword = async function(privateKey, passwordOld, passwordNew, salt) 
    {
    };

    Engine.prototype.sign = async function(privateKey, password, salt, xml) 
    {
    };

    // ENCRYPT
    Engine.prototype.decrypt = async function(privateKeyEnc, password, salt, data) 
    {
    };

    Engine.prototype.encrypt = async function(publicKey, data) 
    {
    };

	window.cryptoJS = new Engine();

})(window, undefined);

