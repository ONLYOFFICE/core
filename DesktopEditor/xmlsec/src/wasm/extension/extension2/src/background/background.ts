import {messageTypes} from "../common/message-const.ts";
import {isMessages} from "../common/message-types.ts";
import browser from "webextension-polyfill";


const messageHandler = (message: unknown) => {
    if (!isMessages(message)) {
        return false;
    }
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

browser.runtime.onMessage.addListener(messageHandler);
export {messageHandler}
