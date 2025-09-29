export const messageTypes = {
    GENERATE_KEYS: "GENERATE_KEYS",
    POPUP_IS_OPENED: "POPUP_IS_OPENED",
    WAIT_ENTER_PASSWORD: "WAIT_ENTER_PASSWORD",
    SELECT_SIGN_KEYS: "SELECT_SIGN_KEYS",
    SIGN_DATA: "SIGN_DATA",
    ENCRYPT: "ENCRYPT",
    ENGINE_IS_EXIST: "ENGINE_IS_EXIST"
} as const;

export const messageListeners = {
    background: "background",
    popup: "popup",
} as const;

export const onlyofficeChannels = {
    onlyofficeExtensionChannel: "onlyoffice-sign-extension-channel",
    onlyofficeClientChannel: "onlyoffice-sign-client-channel",
} as const;