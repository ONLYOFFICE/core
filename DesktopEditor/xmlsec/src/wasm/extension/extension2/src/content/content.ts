import {sendToBackground, sendToPage} from "./messenger.ts";
import {messageTypes, onlyofficeChannels} from "../common/message-const.ts";
import {
    type DispatchEventMessageType,
} from "../common/message-types.ts";

window.addEventListener(onlyofficeChannels.onlyofficeExtensionChannel, (event: CustomEvent<DispatchEventMessageType>) => {
    sendToBackground(event.detail.data).then((response: unknown) => {
            sendToPage({id: event.detail.id, data: response});
    });
});
window.dispatchEvent(new CustomEvent<DispatchEventMessageType>(onlyofficeChannels.onlyofficeClientChannel, {detail: {data: {type: messageTypes.ENGINE_IS_EXIST}}}));
