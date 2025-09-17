import {messageListeners, messageTypes} from "../common/message-const.ts";
import {isMessageType} from "../common/message-types.ts";
import browser from "webextension-polyfill";


const sendMessageToPopup = async (data: unknown) => {
    return await browser.runtime.sendMessage({type: messageTypes.GENERATE_KEYS, listener: messageListeners.popup, data: data});
};
const isOpenedPopup = async () => {
  const isOpenPopupItem = await browser.storage.session.get("isOpenPopup");
  return !!(isOpenPopupItem && isOpenPopupItem.isOpenPopup);
};
const waitClosingPopup = async () => {
    const isOpenPopup = await isOpenedPopup();
    if (!isOpenPopup) {
        return true;
    }
    return new Promise(resolve => {
        const handler = (change) => {
            if ("isOpenPopup" in change && !change.isOpenPopup.newValue) {
                browser.storage.session.onChanged.removeListener(handler);
                resolve(true);
            }
        };
        browser.storage.session.onChanged.addListener(handler);
    });
};
const openPopup = async () => {
    await waitClosingPopup();
    await browser.action.openPopup();
    return await new Promise(resolve => {
        const handler = (change) => {
            if (change.isOpenPopup) {
                browser.storage.session.onChanged.removeListener(handler);
                resolve(true);
            }
        };
        browser.storage.session.onChanged.addListener(handler);
    });
};
const generatePopupKeys = async () => {
    await openPopup();
    await sendMessageToPopup({test: "test"});
    return {data: true};
}

const isBackgroundMessage = (message: unknown) => {
  return isMessageType(message) && message.listener === messageListeners.background;
};


const messageHandler = (message: unknown) => {
    if (!isBackgroundMessage(message)) {
        return false;
    }
    switch (message.type) {
        case messageTypes.GENERATE_KEYS: {
            return generatePopupKeys();
        }
        default:
            return false;
    }
}

browser.runtime.onMessage.addListener(messageHandler);

browser.runtime.onConnect.addListener((port) => {
    if (port.name === "popup") {
        browser.storage.session.set({isOpenPopup: true});

        port.onDisconnect.addListener(() => {
            browser.storage.session.set({isOpenPopup: false});
        });
    }
});
export {messageHandler}
