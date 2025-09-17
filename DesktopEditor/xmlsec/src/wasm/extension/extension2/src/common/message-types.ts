import {messageListeners, messageTypes} from "./message-const.ts";



type Messages = typeof messageTypes[keyof typeof messageTypes];
type MessageType = {
    type?: string;
    id?: number;
    data: Messages;
    listener: typeof messageListeners[keyof typeof messageListeners];
}


const isMessageType = (message: unknown): message is MessageType => {
    return !!(message && typeof message === "object" && "type" in message && typeof message.type === "string");
};
export {isMessageType};
export type {MessageType, Messages};