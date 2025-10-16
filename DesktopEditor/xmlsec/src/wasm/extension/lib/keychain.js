(function (window) {
	// 1) Ввод мастер пароля в popup
	// 2) Генерация ключей - генерируем, генерируем соль и храним публичный, шифрованный AES-GCM приватный, соль, дата.
	// 3) Смена мастер пароля.
	// 4) Подпись, проверка подписи
	// 5) Шифрование, проверка шифрования.
	// 6) Отзыв ключа.
	// 7) импорт/экспорт всех данных

	// Написать тестовый пример. Ставим расширение. Можем вызвать попап и сгенерировать и отозвать ключ,
	// поменять пароль, ввести пароль (первый раз), импорт, экспорт.
	// Связка странички с расширением.

	const pluginMessenger = window.Asc.pluginMessenger;
	const messageTypes = {
		CHECK_ENGINE: "CHECK_ENGINE",
		GENERATE_KEYS: "GENERATE_KEYS",
		SELECT_SIGN_KEYS: "SELECT_SIGN_KEYS",
		SIGN_DATA: "SIGN_DATA",
		VERIFY_DATA: "VERIFY_DATA",
	};
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
		let result = await pluginMessenger.isInitExtension();
		return !!result;
	};

	Keychain.prototype.generateEncryptKeys = async function()
	{
		let key = await pluginMessenger.postMessage({
			type : messageTypes.GENERATE_KEYS,
		});
		return key;
	};

	Keychain.prototype.selectSignKeys = async function() {
		return pluginMessenger.postMessage({
			type : messageTypes.SELECT_SIGN_KEYS
		});
	};
	Keychain.prototype.signData = async function(base64, guid) {
		return pluginMessenger.postMessage({
			type : messageTypes.SIGN_DATA,
			base64Data: base64,
			guid: guid
		});
	};
	Keychain.prototype.verifyData = async function(base64Data, base64Signature, guid) {
		return pluginMessenger.postMessage({
			type : messageTypes.VERIFY_DATA,
			base64Data: base64Data,
			base64Signature: base64Signature,
			guid: guid
		});
	};
	window.Asc = window.Asc || {};
	window.Asc.Keychain = new Keychain();
})(window);
