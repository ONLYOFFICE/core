import browser from "webextension-polyfill";
export const getStorageMasterPassword = async () => {
    const masterPassword = await browser.storage.local.get('masterPassword');
    if (masterPassword && typeof masterPassword.masterPassword === 'string') {
        return masterPassword.masterPassword;
    }
    return null;
}
export const compareWithOldMasterPassword = async (checkPassword: string) => {
    const masterPassword = await getStorageMasterPassword();
    return masterPassword === checkPassword;
}
export const setStorageMasterPassword = (masterPassword: string) => {
    browser.storage.local.set({masterPassword});
}

export const checkIsStorageLogged = async () => {
    const masterPassword = await getStorageMasterPassword();
    if (masterPassword) {
        return true;
    }
    return getChangedProperty("masterPassword");
};
const getChangedProperty = (key: string) => {
    return new Promise((resolve) => {
        browser.storage.local.onChanged.addListener(function handler(change) {
            if (change[key]) {
                browser.storage.local.onChanged.removeListener(handler);
                resolve(change[key].newValue);
            }
        });
    });
};
export const getChangedDefaultGuid = () => {
    return getChangedProperty("default-sign-guid");
};
export const setDefaultSignGuid = (guid: string) => {
    return browser.storage.local.set({"default-sign-guid": guid});
}

export const initCheckOpenedPopup = () => {
    const port = browser.runtime.connect({ name: "popup" });
    port.postMessage({ opened: true });
};