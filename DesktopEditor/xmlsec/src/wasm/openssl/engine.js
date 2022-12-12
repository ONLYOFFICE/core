(function(window, undefined){

	//desktop_fetch
	//string_utf8

	function CryptoJS()
	{
		this.isModuleInit = false;
	};
	CryptoJS.prototype.onLoad = function()
	{
		CryptoJS.prototype.isModuleInit = true;
	};
	CryptoJS.prototype.isModuleInit = false;

	window.Asc = window.Asc || {};
	window.Asc.cryptoJS = new CryptoJS();
	window.Asc.CryptoErrors = {
		OPEN_SSL_WARNING_OK 		: 0,
		OPEN_SSL_WARNING_ERR        : 1,
		OPEN_SSL_WARNING_ALL_OK     : 2,
		OPEN_SSL_WARNING_PASS       : 4,
		OPEN_SSL_WARNING_NOVERIFY   : 8
	};

	//module

	// AES
	CryptoJS.prototype.AES_Encrypt = function(password, message)
	{
		// generate salt & encrypt message
	};
	CryptoJS.prototype.AES_Encrypt = function(password, salt, message)
	{

	};

	CryptoJS.prototype.AES_Decrypt = function(password, message)
	{
	};

	// Signature
	CryptoJS.prototype.CreateED25519Keys = function()
	{

	};

	CryproJS.prototype.Sign = function(privateKey, message)
	{

	};

	CryproJS.prototype.Verify = function(publicKey, message)
	{

	};

	// Certificate & keys
	CryproJS.prototype.loadCert = function(data, password)
	{
	};
	CryproJS.prototype.loadKey = function(data, password)
	{
	};

})(window, undefined);

