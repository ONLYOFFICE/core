const keyChain = window.Asc.Keychain;
var buttonTest1 = document.getElementById("test1");
buttonTest1.onclick = async function() {
	await keyChain.generateEncryptKeys();
	console.log("Ключи сгенерировали");
};

(async () => {
    let isInstalled = await keyChain.checkExistEngine();
    console.log("Check installed: " + isInstalled);
})();
