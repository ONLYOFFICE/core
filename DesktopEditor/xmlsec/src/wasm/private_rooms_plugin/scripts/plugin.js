(function(window, undefined){

	const decryptHeader = "DECRYPTED;";
	const encryptHeader = "ENCRYPTED;";

	const decryptHeaderLen = decryptHeader.length;
	const encryptHeaderLen = encryptHeader.length;	
	
	var currentPassword = "";
	window.Asc.plugin.init = function(obj)
    {
    	if (!obj)
    		return;

    	switch (obj.type)
		{
			case "generatePassword":
			{
				this.executeMethod("OnEncryption", [{ type : "generatePassword", password : generate_password() }]);
				break;
			}
			case "getPasswordByFile":
			{
				this.executeMethod("OnEncryption", [{ type : "getPasswordByFile", password : getPasswordByFile(obj.hash) }]);
				break;
			}
			case "setPasswordByFile":
			{
				this.executeMethod("StartAction", ["Block", "Save to localstorage..."], function() {
					setPasswordByFile(obj.hash, obj.password);
					
					setTimeout(function() {
						// send end action with delay
						window.Asc.plugin.executeMethod("EndAction", ["Block", "Save to localstorage..."]);					
					}, 200);
				});
				break;
			}
			case "encryptData":
			{
				var check = { valid : true };
				for (var i = 0; i < obj.data.length; i++)
					obj.data[i] = obj.data[i].encryptData(check);
				
				this.executeMethod("OnEncryption", [{ type : "encryptData", data : obj.data, check: check.valid }]);
				break;
			}
			case "decryptData":
			{
				var check = { valid : true };
				for (var i = 0; i < obj.data.length; i++)
				{
					if (obj.data[i]["change"])
						obj.data[i]["change"] = obj.data[i]["change"].decryptData(check);
					else
						obj.data[i] = obj.data[i].decryptData(check);
				}
				
				this.executeMethod("OnEncryption", [{ type : "decryptData", data : obj.data, check: check.valid }]);
				break;
			}
			default:
				break;
		}
    };

	window.Asc.plugin.button = function(id)
    {
        this.executeCommand("close", "");
    };

})(window, undefined);
