import {messageTypes} from "../common/message-const.ts";
import type {Messages} from "../common/message-types.ts";

const messageHandler = (message: Messages,  ) => {
    switch (message.type) {
        case messageTypes.CHECK_ENGINE: {
            return Promise.resolve(true);
        }
        case messageTypes.GENERATE_KEYS: {
            return Promise.resolve({data: "key"});
        }
        default:
            return false;
    }
}

// eslint-disable-next-line @typescript-eslint/ban-ts-comment
// @ts-ignore
chrome.runtime.onMessage.addListener(messageHandler);
export {messageHandler}
