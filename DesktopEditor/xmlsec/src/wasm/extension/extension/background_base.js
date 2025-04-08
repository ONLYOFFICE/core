function KeyStorage()
{
    
}

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    console.log("reseived in background:", message);

    // Отображение всплывающего окна
    chrome.action.openPopup();

    // Посылаем событие обратно на страницу
    if (sender.tab) {
        chrome.scripting.executeScript({
            target: { tabId: sender.tab.id },
            func: (msg) => {
                document.dispatchEvent(new CustomEvent("customEventFromExtension", { detail: msg }));
            },
            args: [message]
        });
    }

    sendResponse({ status: "received" });
});
