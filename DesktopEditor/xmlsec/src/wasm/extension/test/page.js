const keyChain = window.Asc.Keychain;
var buttonTest1 = document.getElementById("test1");
buttonTest1.onclick = function() {
    document.dispatchEvent(new CustomEvent("customEventFromPage", { detail: { text: "Привет, расширение!" } }));
};

(async () => {
    let isInstalled = await keyChain.checkExistEngine();
    console.log("Check installed: " + isInstalled);
})();
