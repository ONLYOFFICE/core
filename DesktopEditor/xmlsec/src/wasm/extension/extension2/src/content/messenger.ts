import type {BackgroundMessage, DispatchEventMessageType, MessagesType, PopupMessage} from "../common/message-types.ts";
import browser from "webextension-polyfill";
import {messageListeners, onlyofficeChannels} from "../common/message-const.ts";

export const sendToBackground = async (data: MessagesType) => {
    const backgroundData: BackgroundMessage = {data, listener: messageListeners.background};
    return await browser.runtime.sendMessage(backgroundData);
};

export const sendToPopup = async (data: MessagesType) => {
    const sendData: PopupMessage = {listener: messageListeners.popup, data};
    return await browser.runtime.sendMessage(sendData);
};

export const sendToPage = (data: DispatchEventMessageType) => {
    window.dispatchEvent(new CustomEvent<DispatchEventMessageType>(onlyofficeChannels.onlyofficeClientChannel, {detail: data}));
};