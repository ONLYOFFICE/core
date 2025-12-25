const storageManager = new StorageManager();

const generateKeyButton = document.getElementById('generate-key-button');
const publicKeyInput = document.getElementById('public-key-input');
const getKeyButton = document.getElementById('get-key-button');
const encryptKeyButton = document.getElementById('encrypt-data-button');
const decryptKeyButton = document.getElementById('decrypt-data-button');
const dataInput = document.getElementById('data-input');
const encryptDataInput = document.getElementById('encrypt-data-input');
const selectedKeyDiv = document.getElementById('selected-key');
const encryptedDataDiv = document.getElementById('encrypted-data');
const decryptedDataDiv = document.getElementById('decrypted-data');
const saveStorageButton = document.getElementById('save-storage-button');
const loadStorageButton = document.getElementById('load-storage-button');

let selectedKey = null;
generateKeyButton.addEventListener('click', (e) => {
	storageManager.generateKey().then(function (key) {
		selectedKeyDiv.textContent = key.getExportPublicKey().toBase64();
		selectedKey = key;
	});
});

getKeyButton.addEventListener('click', (e) => {
	storageManager.getKeyByPublicKey(Uint8Array.fromBase64(publicKeyInput.textContent)).then(function (key) {
		selectedKeyDiv.textContent = key.getExportPublicKey().toBase64();
		selectedKey = key;
	});
});

encryptKeyButton.addEventListener('click', (e) => {
	selectedKey.encrypt(dataInput.textContent).then(function (binaryData) {
		encryptedDataDiv.textContent = binaryData.toBase64();
	});
});

decryptKeyButton.addEventListener('click', (e) => {
	selectedKey.decrypt(Uint8Array.fromBase64(encryptDataInput.textContent)).then(function (binaryData) {
		decryptedDataDiv.textContent = binaryData.toBase64();
	});
});

saveStorageButton.addEventListener('click', (e) => {
	const exportKeys = storageManager.exportKeys();
	if (exportKeys.length === 0) {
		alert("Key storage is empty");
	} else {
		localStorage.setItem("tempKeyStorage", storageManager.exportKeys().toBase64());
		alert("Key storage is exported");
	}
});

loadStorageButton.addEventListener('click', (e) => {
	const item = localStorage.getItem("tempKeyStorage");
	if (item === null) {
		alert("localStorage is empty");
	} else {
		storageManager.importKeys(item).then(function () {
			alert("Key storage is loaded");
		});
	}
})
