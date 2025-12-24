const KeyStorage = KeyStorageLibrary.KeyStorage;


function StorageManager() {
	this.keyStorage = new KeyStorage();
}
StorageManager.prototype.generateKey = function (params) {
	const oThis = this;
	return this.askMasterPassword().then(function (masterPassword) {
		return oThis.keyStorage.generateKey(params, masterPassword);
	}).then(function (key) {
		oThis.keyStorage.addKeys([key]);
	});
};
StorageManager.prototype.getMasterPassword = function () {
	let item = window.localStorage.getItem("masterPassword");
	if (item !== undefined) {
		return item;
	}
	item = window.sessionStorage.getItem("masterPassword");
	if (item !== undefined) {
		return item;
	}
	return null;
};
StorageManager.prototype.changeMasterPassword = function () {
	const oldMasterPassword = prompt("Enter Old Master Password");
	const newMasterPassword = prompt("Enter New Master Password");
	this.keyStorage.changeMasterPassword(oldMasterPassword, newMasterPassword);
};
StorageManager.prototype.askMasterPassword = function () {
	let masterPassword = this.getMasterPassword();
	if (masterPassword === null) {
		masterPassword = prompt("Enter Master Password");
		localStorage.removeItem("masterPassword");
		sessionStorage.removeItem("masterPassword");
		if (confirm("Save your master password in localStorage?")) {
			localStorage.setItem("masterPassword", masterPassword);
		} else {
			sessionStorage.setItem("masterPassword", masterPassword);
		}
	}

	return Promise.resolve(masterPassword);
};
StorageManager.prototype.getKeyByPublicKey = function (binaryData) {
	return this.keyStorage.getKeyByPublicKey(binaryData);
};
StorageManager.prototype.exportKeys = function () {
	return this.keyStorage.export();
};
StorageManager.prototype.importKeys = function (binaryData) {
	const oThis = this;
	return this.askMasterPassword().then(function (masterPassword) {
		return oThis.keyStorage.import(binaryData, masterPassword);
	});
};
