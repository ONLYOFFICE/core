var buttonTest1 = document.getElementById("test1");
buttonTest1.onclick = function()
{
    document.dispatchEvent(new CustomEvent("customEventFromPage", { detail: { text: "Привет, расширение!" } }));
}

window.pluginEngine = {};
window.pluginEngine.onMessageFromPlugin = function(e) {
    console.log(e);
};

document.addEventListener("olala", (event) => {
    console.log("Расширение отправило:", event.detail);
});

(async () => {
    let isInstalled = await Asc.Keychain.checkExistEngine();
    console.log("Check installed: " + isInstalled);
})();
