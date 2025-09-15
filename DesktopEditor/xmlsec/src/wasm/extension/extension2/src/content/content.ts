import {sendToBackground, sendToPage} from "./messenger.ts";
import {onlyofficeChannel} from "../common/message-const.ts";
import type {BackgroundMessageAnswer, MessageType} from "../common/message-types.ts";

window.addEventListener("message", (event: MessageEvent<MessageType>) => {
    if (event.data.type === onlyofficeChannel) {
        sendToBackground(event.data.data).then((response: BackgroundMessageAnswer) => {
            sendToPage({id: event.data.id, type: onlyofficeChannel, response: response});
        });
    }
});
