import browser from "webextension-polyfill";
export const getStorageMasterPassword = async () => {
    const masterPassword = await browser.storage.local.get('masterPassword');
    if (masterPassword && typeof masterPassword.masterPassword === 'string') {
        return masterPassword.masterPassword;
    }
    return null;
}
export const setStorageMasterPassword = (masterPassword: string) => {
    browser.storage.local.set({masterPassword});
}

export const checkIsStorageLogged = async () => {
    const masterPassword = await getStorageMasterPassword();
    if (masterPassword) {
        return true;
    }
    return new Promise((resolve) => {
        browser.storage.local.onChanged.addListener(function handler(change) {
            if (change.masterPassword) {
                browser.storage.local.onChanged.removeListener(handler);
                resolve(true);
            }
        });
    });
};

export const initCheckOpenedPopup = () => {
    const port = browser.runtime.connect({ name: "popup" });
    port.postMessage({ opened: true });
};