window.onload = function()
{
	var xmlCertificate = new window.XmlSignature();
	var XmlError = window.XmlSignatureError;

	function loadCert(data, password)
	{
		window.certFileData = data;
		var err = xmlCertificate.loadCert(window.certFileData, password);
		switch (err)
		{
			case XmlError.OPEN_SSL_WARNING_ERR:
			{
				alert("error");
				break;				
			}
			case XmlError.OPEN_SSL_WARNING_ALL_OK:
			{
				document.getElementById("key").style.display = "none";
				document.getElementById("password").style.display = "none";
				break;
			}
			case XmlError.OPEN_SSL_WARNING_OK:
			{
				document.getElementById("key").style.display = "";
				document.getElementById("password").style.display = "none";
				break;				
			}
			case XmlError.OPEN_SSL_WARNING_PASS:
			{
				document.getElementById("key").style.display = "none";
				document.getElementById("password").style.display = "";
				break;
			}
			default:
				break;
		}
	}

	function loadKey(data, password)
	{
		window.keyFileData = data;
		var err = xmlCertificate.loadKey(window.keyFileData, password);
		switch (err)
		{
			case XmlError.OPEN_SSL_WARNING_ERR:
			{
				alert("error");
				break;				
			}
			case XmlError.OPEN_SSL_WARNING_ALL_OK:
			case XmlError.OPEN_SSL_WARNING_OK:
			{
				document.getElementById("password").style.display = "none";
				break;				
			}
			case XmlError.OPEN_SSL_WARNING_PASS:
			{
				document.getElementById("password").style.display = "";
				break;
			}
			default:
				break;
		}
	}	

	document.getElementById('button_certificate').onchange = function() {
		var reader = new FileReader();
		reader.onload = function() {
			loadCert(this.result);
		};			
		reader.readAsArrayBuffer(this.files[0]);
	};

	document.getElementById('button_key').onchange = function() {
		var reader = new FileReader();
		reader.onload = function() {
			loadKey(this.result);
		};			
		reader.readAsArrayBuffer(this.files[0]);
	};

	document.getElementById('file_select').onchange = function() {
		var reader = new FileReader();
		reader.onload = function() {
			window.fileFileData = this.result;
		};			
		reader.readAsArrayBuffer(this.files[0]);
	};

	document.getElementById('button_password').onclick = function() {
		if (document.getElementById("key").style.display == "none")
			loadCert(window.certFileData, document.getElementById("area_password").value);
		else
			loadKey(window.keyFileData, document.getElementById("area_password").value);
	};

	document.getElementById('button_sign').onclick = function() {
		
		if (!window.fileFileData)
		{
			alert("Please select file");
			return;
		}

		function downloadBlob(data, fileName, mimeType) {
			var blob, url;
			blob = new Blob([data], {
			  type: mimeType
			});
			url = window.URL.createObjectURL(blob);
			downloadURL(url, fileName);
			setTimeout(function() {
			  return window.URL.revokeObjectURL(url);
			}, 1000);
		};
		  
		function downloadURL(data, fileName) {
			var a;
			a = document.createElement('a');
			a.href = data;
			a.download = fileName;
			document.body.appendChild(a);
			a.style = 'display: none';
			a.click();
			a.remove();
		};
		
		var result = xmlCertificate.sign(window.fileFileData);
		downloadBlob(result, 'sign_file.docx', 'application/octet-stream');

	};
	
};
