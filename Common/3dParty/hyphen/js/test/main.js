(function (window, undefined){
    var textarea = document.getElementById("textarea");
    var form = document.querySelector("form");

    var control = document.getElementById("dictionary-file");

    var application = undefined;
    var lang = undefined;

    control.addEventListener("change", function(event) {
        var file = control.files[0];
        lang = file.name;
        var reader = new FileReader();
        reader.readAsText(file);
    
        if(application == undefined) {
            application = hyphenCreateApplication(Module);
        }
    
        reader.onload = function() {
            hyphenLoadDictionary(Module, application, reader.result, lang);
        };
     
    }, false);
    
    form.onsubmit = function(event) {
        event.preventDefault();
        var text = textarea.value.split("\n").join(" ").split(" ");
    
        if(application == undefined) {
            application = hyphenCreateApplication(Module);
        }
    
        for(var i = 0; i < text.length; i++) {
            var hyphens = hyphenWord(Module, application, text[i].toLowerCase(), lang);
			console.log(hyphens);
        }
    }
})(self, undefined);