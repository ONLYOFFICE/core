import {sendToBackground, sendToPage} from "./messenger.ts";
import {messageListeners, onlyofficeClientChannel, onlyofficeExtensionChannel} from "../common/message-const.ts";
import type {MessageType} from "../common/message-types.ts";

window.addEventListener(onlyofficeExtensionChannel, (event: CustomEvent<MessageType>) => {
    sendToBackground(event.detail.data).then((response: unknown) => {
        sendToPage({id: event.detail.id, response: response});
    });
});
window.dispatchEvent(new CustomEvent(onlyofficeClientChannel, {detail: {isInitExtension: true}}));
