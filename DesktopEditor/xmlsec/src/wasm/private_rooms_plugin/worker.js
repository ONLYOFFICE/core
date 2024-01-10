(function(window, undefined){
	
	window.AscCrypto = window.AscCrypto || {};
	if (window.AscCrypto.CryptoWorker)
		return;

	var filePassword = "";

	window.AscCrypto.CryptoWorker = {};
	window.AscCrypto.CryptoWorker.User = null;

	// random password generation
	function _generatePassword()
	{
		let guidValues = [];
		if (!window.crypto || !window.crypto.getRandomValues)
		{
			for (let i = 0; i < 8; ++i)
				guidValues.push(((1 + Math.random()) * 0x10000) >> 0);
		}
		else
		{
			let tmp = new Uint16Array(8);
			window.crypto.getRandomValues(tmp);

			for (let i = 0; i < 8; ++i)
				guidValues.push(0x10000 + tmp[i]);
		}

		var index = 0;
		function s4() {
			return guidValues[index++].toString(16).substring(1);
		}
		return s4() + s4() + '-' + s4() + '-' + s4() + '-' + s4() + '-' + s4() + s4() + s4();
	}
	
	// create record for current user
	function _generateCurrentUserRecord = function()
	{
		if (!this.User)
			return null;
		return [{ publicKey : this.User[1].replace(/\n/g, "&#xA"), userId : this.User[2] }];
	};
	
	// read user info
	window.AscCrypto.CryptoWorker.init = function()
	{
		this.User = window.AscDesktopEditor.CryptoCloud_GetUserInfo();
	};	
	// create from passwords
	window.AscCrypto.CryptoWorker.generateDocInfo = function(users, password)
	{		
		if (!Array.isArray(users) || users.length === 0)
			users = _generateCurrentUserRecord();
			
		if (!Array.isArray(users))
			return "";
		
		var result = "ONLYOFFICE CryptoEngine (Version 1)\n\n";
		for (var i = 0, len = users.length; i < len; i++)
		{
			if (users[i].publicKey)
			{
				result += (users[i].userId + "\n");
				result += (window.AscDesktopEditor.CryproRSA_EncryptPublic(users[i].publicKey, password) + "\n\n");
			}				
		}
		return result;
	};
	// decrypt password from docinfo
	window.AscCrypto.CryptoWorker.readPassword = function(docinfo)
	{
		if (docinfo == null)
			docinfo = "";
		
		var user = this.User[2];
		var index = docinfo.indexOf(user);
		
		if (-1 == index)
			return "";
		
		var delimeter = "<!--break-->";
		var start = docinfo.indexOf(delimeter, index);
		if (-1 == index)
			return "";
		
		var end = docinfo.indexOf(delimeter, start + 1);
		if (-1 == end)
			return "";
		
		var encPassword = docinfo.substring(start + delimeter.length, end);
		return window.AscDesktopEditor.CryproRSA_DecryptPrivate(this.User[0], encPassword);
	};

	// запрос на новый пароль
	window.AscCrypto.CryptoWorker.createPassword = function(oldPassword)
	{
		return oldPassword ? oldPassword : _generatePassword();
	};	
	window.AscCrypto.CryptoWorker.generatePassword = function(oldPassword)
	{
		let _password = this.createPassword(oldPassword);
				
		window.AscDesktopEditor.cloudCryptoCommandMainFrame({ type: "getsharingkeys" }, function(obj){
			
			window.Asc.plugin.onSystemMessage({ 
				type : "generatePassword", 
				password : _password, 
				docinfo : (obj.keys && obj.keys.length) ? worker.generateDocInfo(obj.keys, _password) : ""
			});
			
		});
	};
	// шифрование изменений
	window.AscCrypto.CryptoWorker.cryptInit = function(password)
	{
		window.AscDesktopEditor.CryptoAES_Init(password);
	};
	window.AscCrypto.CryptoWorker.encrypt = function(data)
	{
		return window.AscDesktopEditor.CryptoAES_Encrypt(data);
	};
	window.AscCrypto.CryptoWorker.decrypt = function(data)
	{
		return window.AscDesktopEditor.CryptoAES_Decrypt(data);
	};
	
	// init
	window.AscCrypto.CryptoWorker.init();
	
})(window, undefined);
