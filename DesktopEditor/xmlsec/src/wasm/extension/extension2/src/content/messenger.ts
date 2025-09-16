import type {Messages} from "../common/message-types.ts";
import browser from "webextension-polyfill";

const sendToBackground = async (data: Messages) => {
    return await browser.runtime.sendMessage(data);
};
const sendToPage = (data: unknown) => {
    window.postMessage(data, "*");
};

export {sendToBackground, sendToPage};