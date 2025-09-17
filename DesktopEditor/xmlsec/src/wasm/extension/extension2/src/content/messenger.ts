import type {Messages} from "../common/message-types.ts";
import browser from "webextension-polyfill";
import {messageListeners, onlyofficeClientChannel} from "../common/message-const.ts";

const sendToBackground = async (data: Messages) => {
    const backgroundData = {...data, listener: messageListeners.background};
    return await browser.runtime.sendMessage(backgroundData);
};
const sendToPage = (data: unknown) => {
    window.dispatchEvent(new CustomEvent(onlyofficeClientChannel, {detail: data}));
};

export {sendToBackground, sendToPage};