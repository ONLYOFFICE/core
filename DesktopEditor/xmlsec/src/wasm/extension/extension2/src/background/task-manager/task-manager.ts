import browser from "webextension-polyfill";
import {messageTypes} from "../../common/message-const.ts";
import {generatePopupKeys} from "./tasks.ts";
import {isBackgroundMessageType} from "../../common/message-types.ts";

const initTaskManager = () => {
    browser.runtime.onMessage.addListener((message: unknown) => {
        if (!isBackgroundMessageType(message)) {
            return false;
        }
        const data = message.data;
        switch (data.type) {
            case messageTypes.GENERATE_KEYS: {
                return generatePopupKeys();
            }
            default:
                return false;
        }
    });
}

export default initTaskManager;