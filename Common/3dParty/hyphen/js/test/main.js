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
				console.log("In bytes: " + hyphens);

				// size of 1 symbol (1-4 bytes)
				var sizeOfSym = new Blob([text[i]]).size / text[i].length;
				var hword = "";

				var lpos = 0;
				for(var j = 0; j < hyphens.length; j++) {
					var pos = hyphens[j] / sizeOfSym;
					pos = pos | 0;
					hword += text[i].substr(lpos, pos - lpos);
					hword += '=';
					lpos = pos;
				}
				hword += text[i].substr(lpos, text[i].length - lpos);
				console.log(hword);
			}
		}
	   event.preventDefault();
	}
})(self);