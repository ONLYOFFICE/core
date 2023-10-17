(function (window, undefined) {

    window.hyphen.loadModule();

    var langs = {
        "az_Latn_AZ"     : 1068,
        "bg_BG"          : 1026,
        "ca_ES"          : 1027,
        "ca_ES_valencia" : 2051,
        "cs_CZ"          : 1029,
        "da_DK"          : 1030,
        "de_AT"          : 3079,
        "de_CH"          : 2055,
        "de_DE"          : 1031,
        "el_GR"          : 1032,
        "en_AU"          : 3081,
        "en_CA"          : 4105,
        "en_GB"          : 2057,
        "en_US"          : 1033,
        "en_ZA"          : 7177,
        "es_ES"          : 3082,
        "eu_ES"          : 1069,
        "fr_FR"          : 1036,
        "gl_ES"          : 1110,
        "hr_HR"          : 1050,
        "hu_HU"          : 1038,
        "id_ID"          : 1057,
        "it_IT"          : 1040,
        "kk_KZ"          : 1087,
        "ko_KR"          : 1042,
        "lb_LU"          : 1134,
        "lt_LT"          : 1063,
        "lv_LV"          : 1062,
        "mn_MN"          : 1104,
        "nb_NO"          : 1044,
        "nl_NL"          : 1043,
        "nn_NO"          : 2068,
        "oc_FR"          : 1154,
        "pl_PL"          : 1045,
        "pt_BR"          : 1046,
        "pt_PT"          : 2070,
        "ro_RO"          : 1048,
        "ru_RU"          : 1049,
        "sk_SK"          : 1051,
        "sl_SI"          : 1060,
        "sr_Cyrl_RS"     : 10266,
        "sr_Latn_RS"     : 9242,
        "sv_SE"          : 1053,
        "tr_TR"          : 1055,
        "uk_UA"          : 1058,
        "uz_Cyrl_UZ"     : 2115,
        "uz_Latn_UZ"     : 1091,
        "vi_VN"          : 1066,
        "nl_NL"          : 2067
    };

	var textarea = document.getElementById("textarea");
	var form = document.querySelector("form");
	var combobox = document.getElementById("combobox");

	combobox.value = "en_US";
	textarea.value = "expedition";

	form.onsubmit = function(event)
	{
	    if(combobox.value == "")
	        return;

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

	    request.onload = function ()
	    {
	        var dict = request.response;
	        var langInt = langs[lang];
	        window.hyphen.loadDictionary(langInt, dict);

	        for (var i = 0; i < text.length; i++) 
	        {
	            var hyphens = window.hyphen.hyphenWord(langInt, text[i].toLowerCase());

	            let itemUtf8 = text[i].toUtf8(true);
	            let start = 0;
	            let hword = "";

	            for (let j = 0, len = hyphens.length; j < len; j++) 
	            {
	                hword += "".fromUtf8(itemUtf8, start, hyphens[j] - start);
	                hword += "=";
	                start = hyphens[j];
	            }

	            if (start < itemUtf8.length) 
	            {
	                hword += "".fromUtf8(itemUtf8, start);
	                hword += "=";
	            }

	            console.log(hword);
	        }
	    }
	    event.preventDefault();
	}
})(self);
