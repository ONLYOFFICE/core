const messageTypes = {
    GENERATE_KEYS: "GENERATE_KEYS",
    POPUP_IS_OPENED: "POPUP_IS_OPENED",
} as const;
const messageListeners = {
    background: "background",
    popup: "popup",
} as const;
const onlyofficeExtensionChannel = "onlyoffice-sign-extension-channel";
const onlyofficeClientChannel = "onlyoffice-sign-client-channel";
export {messageTypes, onlyofficeExtensionChannel, onlyofficeClientChannel, messageListeners};