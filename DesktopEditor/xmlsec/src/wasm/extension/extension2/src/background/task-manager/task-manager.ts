import browser from "webextension-polyfill";
import {messageTypes} from "../../common/message-const.ts";
import {generatePopupKeys, selectSignKeys} from "./tasks.ts";
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
            case messageTypes.SELECT_SIGN_KEYS: {
                return selectSignKeys();
            }
            // todo
            // case messageTypes.SIGN_DATA: {
            //     return signData(data.base64Data, data.guid);
            // }
            default:
                return false;
        }
    });
}

export default initTaskManager;