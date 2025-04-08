chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    document.getElementById("message").innerText = JSON.stringify(message);
});

document.addEventListener("DOMContentLoaded", (event) => {
    
    document.getElementById("testButton").onclick = function(e) {
        chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
            if (tabs.length === 0) return;
            
            chrome.tabs.sendMessage(tabs[0].id, { action: "popupMessage", text: "Привет от Popup!" }, (response) => {
                console.log("Ответ от контентного скрипта:", response);
            });
        });
    };

});
