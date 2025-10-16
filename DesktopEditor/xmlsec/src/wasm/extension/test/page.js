const keyChain = window.Asc.Keychain;
const input = document.getElementById("inputTest1");
const selectSign = document.getElementById("selectSign");
const selectedSignGuid = document.getElementById("guid");
const signText = document.getElementById("signText");
const verifyText = document.getElementById("verifyText");
const signValue = document.getElementById("signValue");
const verifyValue = document.getElementById("verifyValue");

let guid = null;
selectSign.addEventListener("click", async function(e) {
	guid = await keyChain.selectSignKeys();
	selectedSignGuid.innerText = `Selected key's guid: ${guid}`;
});

let base64Signature;
signText.addEventListener("click", (e) => {
	if (guid === null) {
		throw new Error("Please select guid");
	}
	const textForSign = input.value;
	const base64 = btoa(textForSign);
	(async () => {
		base64Signature = await keyChain.signData(base64, guid);
		signValue.innerText = `base64 sign: ${base64Signature}`;
	})();
});

verifyText.addEventListener("click", (e) => {
	if (guid === null) {
		throw new Error("Please select guid");
	}
	const textForVerify = input.value;
	const base64 = btoa(textForVerify);
	(async () => {
		const isVerify = await keyChain.verifyData(base64, base64Signature, guid);
		verifyValue.innerText = `Signature verification result: ${isVerify ? "the signature is valid" : "the signature is invalid"}`;
	})();
});

(async () => {
    let isInstalled = await keyChain.checkExistEngine();
    console.log("Check installed: " + isInstalled);
})();


