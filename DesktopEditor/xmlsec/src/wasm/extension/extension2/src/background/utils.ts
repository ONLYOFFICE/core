import browser from "webextension-polyfill";
import getCrypto from "../common/crypto.ts";

export const initCheckOpenedPopup = () => {
    browser.runtime.onConnect.addListener((port) => {
        if (port.name === "popup") {
            browser.storage.session.set({isOpenPopup: true});

            port.onDisconnect.addListener(() => {
                browser.storage.session.set({isOpenPopup: false});
            });
        }
    });
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
        browser.storage.session.onChanged.addListener(function handler(change) {
            if (change.isOpenPopup && !change.isOpenPopup.newValue) {
                browser.storage.session.onChanged.removeListener(handler);
                resolve(true);
            }
        });
    });
};

export const openPopup = async () => {
    await waitClosingPopup();
    await browser.action.openPopup();
    return new Promise(resolve => {
        browser.storage.session.onChanged.addListener(function handler(change) {
            if (change.isOpenPopup && change.isOpenPopup.newValue) {
                browser.storage.session.onChanged.removeListener(handler);
                resolve(true);
            }
        });
    });
};

export const getGUID = () => {
    const crypto = getCrypto();
    return `{${crypto.randomUUID()}}`;
}