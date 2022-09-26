(function (window, undefined) {

	window.hyphen.loadModule();

	var textarea = document.getElementById("textarea");
	var form = document.querySelector("form");
	var combobox = document.getElementById("combobox");

	combobox.value = "en_US";
	textarea.value = "expedition";


	form.onsubmit = function(event) {

		if(combobox.value == "") {
			return;
		}

		var lang = combobox.value;
		var text = textarea.value.split("\n").join(" ").split(" ");
	 
		var request = new XMLHttpRequest();
		var path = '../../../../../../dictionaries/' + lang + '/' + 'hyph_' + lang  + '.dic';

		request.responseType = 'arraybuffer';

		if (request.overrideMimeType) {
			request.overrideMimeType('text/plain; charset=x-user-defined');
		} else {
			request.setRequestHeader('Accept-Charset', 'x-user-defined');
		}
		request.open('GET', path, true);
		request.send(null);

		request.onload = function () {
			var dict = request.response;
			window.hyphen.loadDictionary(dict, lang);

			for(var i = 0; i < text.length; i++) {
				var hyphens = window.hyphen.hyphenWord(text[i].toLowerCase(), lang);

				let itemUtf8 = text[i].toUtf8(true);
				let start = 0;
				let hword = "";

				for(let j = 0, len = hyphens.length; j < len; j++) {
					hword += "".fromUtf8(itemUtf8, start, hyphens[j] - start);
					hword += "=";
					start = hyphens[j];
				}

				if (start < itemUtf8.length) {
					hword += "".fromUtf8(itemUtf8, start);
					hword += "=";
				}

				console.log(hword);
			}
		}
	   event.preventDefault();
	}
})(self);