import type {ContentResponseType, Messages} from "../common/message-types.ts";

const sendToBackground = async (data: Messages) => {

    // @ts-ignore
    return await chrome.runtime.sendMessage(data);
};
const sendToPage = (data: ContentResponseType) => {
    window.postMessage(data, "*");
};

export {sendToBackground, sendToPage};