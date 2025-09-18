import {messageListeners, messageTypes} from "./message-const.ts";


export type MessagesType = {type: typeof messageTypes[keyof typeof messageTypes]};

export const isMessages = (arg: unknown): arg is MessagesType => {
    return !!(arg && typeof arg === "object" && "type" in arg && typeof arg.type === "string" && arg.type in messageTypes);
};

export type DispatchEventMessageType = {
    id?: number;
    data: MessagesType;
}
type Listeners = typeof messageListeners[keyof typeof messageListeners];
type ExtensionMessage<T extends Listeners = Listeners> = {
    data: MessagesType;
    listener: T;
};
export type BackgroundMessage = ExtensionMessage<typeof messageListeners.background>;
export type PopupMessage = ExtensionMessage<typeof messageListeners.popup>;

const isExtensionMessageType  = (arg: unknown): arg is ExtensionMessage => {
    return !!(arg && typeof arg === "object" && "data" in arg && isMessages(arg.data) && "listener" in arg && typeof arg.listener === "string");
};
export const isBackgroundMessageType = (arg: unknown): arg is BackgroundMessage => {
    return isExtensionMessageType(arg) && arg.listener === messageListeners.background;
};

export const isPopupMessageType = (arg: unknown): arg is PopupMessage => {
    return isExtensionMessageType(arg) && arg.listener === messageListeners.popup;
};