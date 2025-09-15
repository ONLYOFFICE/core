import {messageTypes} from "./message-const.ts";
import type {messageHandler} from "../background/background.ts";

type CheckEngineMessage = {
    type: typeof messageTypes.CHECK_ENGINE;
};

type GenerateKeysMessage = {
    type: typeof messageTypes.GENERATE_KEYS;
};

type Messages = CheckEngineMessage | GenerateKeysMessage;
type MessageType = {
    type?: string;
    id?: number;
    data: Messages
}

type BackgroundMessagePromise = ReturnType<typeof messageHandler>;
type BackgroundMessageAnswer = Awaited<BackgroundMessagePromise>;
type ContentResponseType = {
    type?: string;
    id?: number;
    response?: BackgroundMessageAnswer;
};

export type {MessageType, Messages, BackgroundMessageAnswer, ContentResponseType};