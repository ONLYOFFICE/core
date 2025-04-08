function sendEventToPage(id, data) {
    document.dispatchEvent(new CustomEvent("olala", { detail: data }));
}

var ENGINE_VERSION = 1;
var ENGINE_MESSAGE_CHECK = "onlyoffice-engine-check";
var ENGINE_MESSAGE_DATA = "onlyoffice-engine-data";

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (message.action === "popupMessage") {
        console.log("Сообщение из popup:", message.text);

        if (window.pluginEngine && window.pluginEngine.onMessageFromPlugin)
            window.pluginEngine.onMessageFromPlugin(message);

        sendResponse({ status: "Сообщение получено!" });

        sendEventToPage({ message: "Привет от content.js" });
    }
});

// event from page with info about engine (is exist, version...)
document.addEventListener(ENGINE_MESSAGE_CHECK + "-page", (event) => {
    document.dispatchEvent(new CustomEvent(ENGINE_MESSAGE_CHECK + "-content", { detail: { version : ENGINE_VERSION } }));
});

// event from page with action (proxy to background)
document.addEventListener(ENGINE_MESSAGE_DATA + "-page", (event) => {
    chrome.runtime.sendMessage({
        id : ENGINE_MESSAGE_DATA + "-engine",
        data : event.detail
    });    
});

document.addEventListener(ENGINE_MESSAGE_DATA + "-engine", (event) => {
    document.dispatchEvent(new CustomEvent(ENGINE_MESSAGE_DATA + "-content", event.detail));
});
