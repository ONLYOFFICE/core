(function (window, undefined) {

	window.hyphen.loadModule();

	var textarea = document.getElementById("textarea");
	var form = document.querySelector("form");
	var combobox = document.getElementById("combobox");

	if (true)
	{
		combobox.value = "en_US";
		textarea.value = "expedition";
	}

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
				console.log(hyphens);
			}
		}
	   event.preventDefault();
	}
})(self);