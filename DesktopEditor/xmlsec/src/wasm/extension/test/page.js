const keyChain = window.Asc.Keychain;
const input = document.getElementById("inputTest1");
const selectSign = document.getElementById("selectSign");
const selectedSignGuid = document.getElementById("guid");
const signText = document.getElementById("signText");
const signValue = document.getElementById("signValue");

let guid = null;
selectSign.addEventListener("click", async function(e) {
	guid = await keyChain.selectSignKeys();
	selectedSignGuid.innerText = `Выбранный guid ключа: ${guid}`;
});

signText.addEventListener("click", async function(e) {
	if (guid === null) {
		throw new Error("Please select guid");
	}
	const textForSign = input.innerText;
	const base64 = btoa(textForSign);
	const signBase64 = await keyChain.signData(base64, guid);
	signValue.innerText = signBase64;
});



(async () => {
    let isInstalled = await keyChain.checkExistEngine();
    console.log("Check installed: " + isInstalled);
})();


