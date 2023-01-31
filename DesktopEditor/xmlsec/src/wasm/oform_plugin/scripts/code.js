/**
 *
 * (c) Copyright Ascensio System SIA 2020
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
(function(window, undefined)
{

	(function()
	{
		let url = "./engine/engine";
		let useWasm = false;
		let webAsmObj = window["WebAssembly"];
		if (typeof webAsmObj === "object" && typeof webAsmObj["Memory"] === "function")
		{
			if ((typeof webAsmObj["instantiateStreaming"] === "function") || (typeof webAsmObj["instantiate"] === "function"))
				useWasm = true;
		}

		url += (useWasm ? ".js" : "_ie.js");

		let script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = url;
		document.head.appendChild(script);
	})();

	var CurrentModeType = {
		Sign: 0,
		Generate: 1,
		Change: 2
	}
	var currentMode = CurrentModeType.Sign;
	var currentSignData = "";

	function CKeyChainInfo()
	{
		// object:
		// { "id1" : {"type" : "rec_type", "value" : "rec_value"}, "id2" : {...}, ... }

		this.value = {};
	}

	CKeyChainInfo.prototype.set = function(value)
	{
		this.value = value ? value : {};
	};
	CKeyChainInfo.prototype.getSignInfo = function()
	{
		let keyInfo = null;
		for (let prop in this.value)
		{
			if (!this.value.hasOwnProperty(prop))
				continue;

			let rec = this.value[prop];
			if (rec.key === "keySign" && rec.value && rec.value.date)
			{
				if (!keyInfo)
					keyInfo = rec.value;
				else
				{
					let dateOld = new Date(keyInfo.date);
					let dateNew = new Date(rec.value.date);

					if (dateNew > dateOld)
						keyInfo = rec.value;
				}
			}
		}
		return keyInfo;
	};
	CKeyChainInfo.prototype.getCurrentPassword = function()
	{
		let elem = document.getElementById("old_password_value");
		if (!elem || elem.value === "")
			return "";
		return elem.value;
	};
	CKeyChainInfo.prototype.getNewPassword = function()
	{
		let elem = document.getElementById("new_password_value");
		if (!elem || elem.value === "")
			return "";
		return elem.value;
	};
	CKeyChainInfo.prototype.changePassword = function(oldPassword, newPassword)
	{
		let newValue = {};

		for (let prop in this.value)
		{
			if (!this.value.hasOwnProperty(prop))
				continue;

			let rec = this.value[prop];
			switch (rec.key)
			{
				case "keySign":
				case "keyCrypt":
				{
					let newKeyEnc = window.cryptoJS.changePassword(rec.value.privateKey,
						oldPassword, newPassword, rec.value.salt);

					if (!newKeyEnc)
						return null;

					rec.value.privateKey = newKeyEnc;

					newValue[prop] = rec;
					break;
				}
				default:
				{
					//newValue[prop] = rec;
					break;
				}
			}
		}

		return newValue;
	};
	CKeyChainInfo.prototype.checkPassword = function(password)
	{
		for (let prop in this.value)
		{
			if (!this.value.hasOwnProperty(prop))
				continue;

			let rec = this.value[prop];
			switch (rec.key)
			{
				case "keySign":
				case "keyCrypt":
				{
					let newKeyEnc = window.cryptoJS.changePassword(rec.value.privateKey,
						password, "test", rec.value.salt);

					if (!newKeyEnc)
						return false;

					break;
				}
				default:
				{
					//newValue[prop] = rec;
					break;
				}
			}
		}

		return true;
	};

	var currentKeychain = new CKeyChainInfo();

	window.Asc.plugin.init = function(text)
	{
		currentSignData = text;
		onTabSelect(CurrentModeType.Sign);

		document.getElementById("id_mode_sign").onclick = function()
		{
			onTabSelect(0);
		};
		document.getElementById("id_mode_generate").onclick = function()
		{
			onTabSelect(1);
		};
		document.getElementById("id_mode_change").onclick = function()
		{
			onTabSelect(2);
		};

		this.executeMethod("GetKeychainStorageInfo", [{ type: "get", value: ["keySign", "keyCrypt"] }], function(obj)
		{
			currentKeychain.set(obj);
			if (!currentKeychain.getSignInfo())
				onTabSelect(CurrentModeType.Generate);
		});
	};

	window.Asc.plugin.button = function(id)
	{
		if (!window.cryptoJS.isModuleInit)
		{
			// TODO: wait...
			console.log("please wait when the module will be loaded...");
			this.executeCommand("close", "");
			return;
		}

		if (0 !== id)
		{
			this.executeMethod("OnSignWithKeychain", [undefined], function()
			{
				this.executeCommand("close", "");
			});

			return;
		}

		switch (currentMode)
		{
			case CurrentModeType.Sign:
			{
				let password = currentKeychain.getCurrentPassword();
				if ("" === password)
				{
					document.getElementById("old_input_error_id").style.display = "block";
					console.log("check password!");
					return;
				}

				document.getElementById("old_input_error_id").style.display = "none";

				let currentKey = currentKeychain.getSignInfo();
				if (!currentKey)
				{
					onTabSelect(CurrentModeType.Generate);
					console.log("please generate one key pair!");
					return;
				}

				let signData = window.cryptoJS.sign(currentKey.privateKey, password, currentKey.salt, currentSignData);
				if (!signData)
				{
					document.getElementById("old_input_error_id").style.display = "block";
					console.log("check password!");
					return;
				}

				let signDataReturn = {
					data: signData,
					key: "pem:" + currentKey.publicKey
				};

				this.executeMethod("OnSignWithKeychain", [signDataReturn], function(isOK)
				{

					window.Asc.plugin.executeCommand("close", "");

				});

				break;
			}
			case CurrentModeType.Generate:
			{
				let password = currentKeychain.getCurrentPassword();
				if ("" === password || false === currentKeychain.checkPassword(password))
				{
					document.getElementById("old_input_error_id").style.display = "block";
					console.log("check password!");
					return;
				}

				document.getElementById("old_input_error_id").style.display = "none";
				let keyItem = window.cryptoJS.generateKeys(password);

				let item = {
					type: "add",
					value : [{
						key : "keySign",
						value : {
							privateKey: keyItem.privateKey,
							publicKey: keyItem.publicKey,
							salt: keyItem.salt,

							private: ["privateKey"]
						}
					}],
					callback : ["keySign"]
				};

				this.executeMethod("SetKeychainStorageInfo", [item], function(obj)
				{

					currentKeychain.set(obj);
					if (currentKeychain.getSignInfo())
						onTabSelect(CurrentModeType.Sign);
					else
					{
						console.log("storage error");
					}

				});

				break;
			}
			case CurrentModeType.Change:
			{
				if (!currentKeychain.getSignInfo())
				{
					onTabSelect(CurrentModeType.Generate);
					console.log("please generate one key pair!");
					return;
				}

				let oldPassword = currentKeychain.getCurrentPassword();
				let newPassword = currentKeychain.getNewPassword();

				if (oldPassword === "")
				{
					document.getElementById("old_input_error_id").style.display = "block";
					return;
				}
				if (newPassword === "")
				{
					document.getElementById("new_input_error_id").style.display = "block";
					return;
				}

				document.getElementById("old_input_error_id").style.display = "none";
				document.getElementById("new_input_error_id").style.display = "none";

				replaceItems = currentKeychain.changePassword(oldPassword, newPassword);
				if (replaceItems)
				{
					this.executeMethod("SetKeychainStorageInfo", [{type: "replace", value : replaceItems, callback : ["keySign"]}], function(obj)
					{

						currentKeychain.set(obj);
						if (currentKeychain.getSignInfo())
							onTabSelect(CurrentModeType.Sign);
						else
						{
							console.log("storage error");
						}

					});
				}
				else
				{
					document.getElementById("old_input_error_id").style.display = "block";
					console.log("wrong password!");
				}

				break;
			}
			default:
				break;
		}
	};

	function _translateElem(id, text)
	{
		let _elem = document.getElementById(id);
		if (_elem) _elem.innerHTML = window.Asc.plugin.tr(text);
	}

	window.Asc.plugin.onTranslate = function()
	{
		_translateElem("id_text_sign",     "Sign");
		_translateElem("id_text_generate", "Generate");
		_translateElem("id_text_change",   "Change Password");
		_translateElem("id_old_password",  "Password");
		_translateElem("id_new_password",  "New Password");
	};

	window.Asc.plugin.onThemeChanged = function(theme)
	{
		window.Asc.plugin.onThemeChangedBase(theme);

		let separators = document.getElementsByClassName("tab_separator");
		for (let i = 0, len = separators.length; i < len; i++)
			separators[i].style.backgroundColor = window.Asc.plugin.theme["text-normal"];

		let setections = document.getElementsByClassName("tab_select");
		for (let i = 0, len = setections.length; i < len; i++)
			setections[i].style.backgroundColor = window.Asc.plugin.theme["text-normal"];
	};

	function onTabSelect(mode)
	{
		document.getElementById("old_input_error_id").style.display = "none";
		document.getElementById("new_input_error_id").style.display = "none";

		currentMode = mode;
		switch (currentMode)
		{
			case CurrentModeType.Sign:
				document.getElementById("id_mode_sign").classList.add("tab_mode_select");
				document.getElementById("id_mode_generate").classList.remove("tab_mode_select");
				document.getElementById("id_mode_change").classList.remove("tab_mode_select");
				document.getElementById("id_tab_select").style.transform = "translateX(calc(6.5%*5))";
				break;
			case CurrentModeType.Generate:
				document.getElementById("id_mode_sign").classList.remove("tab_mode_select");
				document.getElementById("id_mode_generate").classList.add("tab_mode_select");
				document.getElementById("id_mode_change").classList.remove("tab_mode_select");
				document.getElementById("id_tab_select").style.transform = "translateX(calc(39.5%*5))";
				break;
			case CurrentModeType.Change:
				document.getElementById("id_mode_sign").classList.remove("tab_mode_select");
				document.getElementById("id_mode_generate").classList.remove("tab_mode_select");
				document.getElementById("id_mode_change").classList.add("tab_mode_select");
				document.getElementById("id_tab_select").style.transform = "translateX(calc(72.5%*5))";
				break;
			default:
				break;
		}

		switch (mode)
		{
			case CurrentModeType.Sign:
			case CurrentModeType.Generate:
				document.getElementById("id_new_password").style.display = "none";
				document.getElementById("id_new_password_div").style.display = "none";
				window.Asc.plugin.resizeWindow(500, 110, 500, 110, 0, 0);
				break;
			case CurrentModeType.Change:
				document.getElementById("id_new_password").style.display = "block";
				document.getElementById("id_new_password_div").style.display = "block";
				window.Asc.plugin.resizeWindow(500, 160, 500, 160, 0, 0);
				break;
			default:
				break;
		}
	};

})(window, undefined);
